#!/usr/bin/env python3
"""Run a period LINK with its clock frozen at one second.

LINK 5.x stamps the image's COFF TimeDateStamp with the C runtime's time(NULL) and
has no switch that takes a value. The retail stamp,
905883247 (1998-09-15 18:14:07 UTC), therefore comes from LINK's own clock read:

    python tools/link_clock.py --time 905883247 -- link.exe /nologo @a.rsp ...

The clock is faked for this one process only. The system clock and every other
process are untouched, and no linker or runtime file is changed on disk:

1. LINK starts under the Windows debug API (DEBUG_ONLY_THIS_PROCESS).
2. At the loader's breakpoint (the WOW64 one for a 32-bit LINK under a 64-bit
   Python), the process's import of `time` is already resolved. Its IAT slot is
   pointed at a 16-byte stub in a fresh page of LINK's memory that returns the
   frozen value (and stores it through the argument, as time() does).
3. The debugger detaches and LINK runs to completion on its own.

Every time() call in LINK then returns the same second, as a stopped clock would.
The tool refuses to run if LINK does not import `time` exactly once, if the slot
is not resolved at the loader's breakpoint, or if the patch cannot be read back.
It changes nothing about LINK's inputs or switches; the caller checks the stamp
in the image it produced (tools/build.py).

Windows only (the debug API).
"""

from __future__ import annotations

import argparse
import ctypes
import struct
import subprocess
import sys
from ctypes import wintypes
from pathlib import Path

RETAIL_TIME = 905883247  # 0x35FEAE6F, the COFF TimeDateStamp of retail ff7.exe

DEBUG_ONLY_THIS_PROCESS = 0x00000002
EXCEPTION_DEBUG_EVENT, CREATE_PROCESS_DEBUG_EVENT, EXIT_PROCESS_DEBUG_EVENT = 1, 3, 5
LOAD_DLL_DEBUG_EVENT = 6
DBG_CONTINUE, DBG_EXCEPTION_NOT_HANDLED = 0x00010002, 0x80010001
STATUS_BREAKPOINT, STATUS_WX86_BREAKPOINT = 0x80000003, 0x4000001F
MEM_COMMIT_RESERVE, PAGE_EXECUTE_READ, PAGE_READWRITE = 0x3000, 0x20, 0x04
INFINITE = 0xFFFFFFFF


def import_slot(image: bytes, function: str = "time") -> tuple[int, int, int]:
    """(image base, IAT slot RVA, on-disk slot value) of `function`, imported by name exactly once."""
    pe = struct.unpack_from("<I", image, 0x3C)[0]
    if image[pe:pe + 4] != b"PE\0\0" or struct.unpack_from("<H", image, pe + 24)[0] != 0x10B:
        raise ValueError("not a 32-bit PE image")
    base = struct.unpack_from("<I", image, pe + 24 + 28)[0]
    nsec = struct.unpack_from("<H", image, pe + 6)[0]
    optsize = struct.unpack_from("<H", image, pe + 20)[0]
    sections = [struct.unpack_from("<IIII", image, pe + 24 + optsize + 40 * i + 8) for i in range(nsec)]

    def offset(rva):
        for vsize, va, rsize, rptr in sections:
            if va <= rva < va + max(vsize, rsize):
                return rva - va + rptr
        raise ValueError(f"RVA {rva:#x} outside every section")

    imports = struct.unpack_from("<I", image, pe + 24 + 104)[0]
    found = []
    desc = offset(imports)
    while True:
        lookup, _stamp, _chain, name, iat = struct.unpack_from("<IIIII", image, desc)
        if name == 0:
            break
        names, slots = offset(lookup or iat), iat
        while True:
            entry = struct.unpack_from("<I", image, names)[0]
            if entry == 0:
                break
            if not entry & 0x80000000:
                start = offset(entry) + 2
                if image[start:image.index(b"\0", start)].decode("latin-1") == function:
                    found.append((slots, struct.unpack_from("<I", image, offset(slots))[0]))
            names += 4
            slots += 4
        desc += 20
    if len(found) != 1:
        raise ValueError(f"expected one import of {function}, found {len(found)}")
    return base, found[0][0], found[0][1]


def stub(seconds: int) -> bytes:
    """x86 cdecl time(time_t *t): store `seconds` through t when it is not NULL, return it."""
    return (b"\xB8" + struct.pack("<I", seconds)      # mov eax, seconds
            + b"\x8B\x4C\x24\x04"                     # mov ecx, [esp+4]
            + b"\x85\xC9"                             # test ecx, ecx
            + b"\x74\x02"                             # jz done
            + b"\x89\x01"                             # mov [ecx], eax
            + b"\xC3")                                # done: ret


class STARTUPINFO(ctypes.Structure):
    _fields_ = [("cb", wintypes.DWORD), ("lpReserved", wintypes.LPWSTR), ("lpDesktop", wintypes.LPWSTR),
                ("lpTitle", wintypes.LPWSTR), ("dwX", wintypes.DWORD), ("dwY", wintypes.DWORD),
                ("dwXSize", wintypes.DWORD), ("dwYSize", wintypes.DWORD), ("dwXCountChars", wintypes.DWORD),
                ("dwYCountChars", wintypes.DWORD), ("dwFillAttribute", wintypes.DWORD),
                ("dwFlags", wintypes.DWORD), ("wShowWindow", wintypes.WORD), ("cbReserved2", wintypes.WORD),
                ("lpReserved2", ctypes.c_void_p), ("hStdInput", wintypes.HANDLE),
                ("hStdOutput", wintypes.HANDLE), ("hStdError", wintypes.HANDLE)]


class PROCESS_INFORMATION(ctypes.Structure):
    _fields_ = [("hProcess", wintypes.HANDLE), ("hThread", wintypes.HANDLE),
                ("dwProcessId", wintypes.DWORD), ("dwThreadId", wintypes.DWORD)]


def kernel32():
    k = ctypes.WinDLL("kernel32", use_last_error=True)
    k.CreateProcessW.argtypes = [wintypes.LPCWSTR, wintypes.LPWSTR, ctypes.c_void_p, ctypes.c_void_p,
                                 wintypes.BOOL, wintypes.DWORD, ctypes.c_void_p, wintypes.LPCWSTR,
                                 ctypes.POINTER(STARTUPINFO), ctypes.POINTER(PROCESS_INFORMATION)]
    k.WaitForDebugEvent.argtypes = [ctypes.c_void_p, wintypes.DWORD]
    k.ContinueDebugEvent.argtypes = [wintypes.DWORD, wintypes.DWORD, wintypes.DWORD]
    k.DebugActiveProcessStop.argtypes = [wintypes.DWORD]
    k.DebugSetProcessKillOnExit.argtypes = [wintypes.BOOL]
    k.ReadProcessMemory.argtypes = [wintypes.HANDLE, ctypes.c_void_p, ctypes.c_void_p, ctypes.c_size_t,
                                    ctypes.POINTER(ctypes.c_size_t)]
    k.WriteProcessMemory.argtypes = k.ReadProcessMemory.argtypes
    k.VirtualAllocEx.argtypes = [wintypes.HANDLE, ctypes.c_void_p, ctypes.c_size_t, wintypes.DWORD, wintypes.DWORD]
    k.VirtualAllocEx.restype = ctypes.c_void_p
    k.VirtualProtectEx.argtypes = [wintypes.HANDLE, ctypes.c_void_p, ctypes.c_size_t, wintypes.DWORD,
                                   ctypes.POINTER(wintypes.DWORD)]
    k.FlushInstructionCache.argtypes = [wintypes.HANDLE, ctypes.c_void_p, ctypes.c_size_t]
    k.GetStdHandle.restype = wintypes.HANDLE
    k.CloseHandle.argtypes = [wintypes.HANDLE]
    k.WaitForSingleObject.argtypes = [wintypes.HANDLE, wintypes.DWORD]
    k.GetExitCodeProcess.argtypes = [wintypes.HANDLE, ctypes.POINTER(wintypes.DWORD)]
    return k


def run(argv: list[str], seconds: int) -> int:
    k = kernel32()
    exe = Path(argv[0])
    base, slot_rva, disk_value = import_slot(exe.read_bytes())
    slot = base + slot_rva

    si = STARTUPINFO()
    si.cb = ctypes.sizeof(si)
    si.dwFlags = 0x100  # STARTF_USESTDHANDLES
    si.hStdInput, si.hStdOutput, si.hStdError = (k.GetStdHandle(n) for n in (-10, -11, -12))
    pi = PROCESS_INFORMATION()
    cmdline = ctypes.create_unicode_buffer(subprocess.list2cmdline(argv))
    if not k.CreateProcessW(str(exe), cmdline, None, None, True, DEBUG_ONLY_THIS_PROCESS, None, None,
                            ctypes.byref(si), ctypes.byref(pi)):
        raise OSError(ctypes.get_last_error(), f"cannot start {exe}")
    event = ctypes.create_string_buffer(256)
    ptr = 8 if ctypes.sizeof(ctypes.c_void_p) == 8 else 4
    union = 16 if ptr == 8 else 12
    patched = None

    def read(addr, size):
        buf, got = ctypes.create_string_buffer(size), ctypes.c_size_t()
        if not k.ReadProcessMemory(pi.hProcess, addr, buf, size, ctypes.byref(got)) or got.value != size:
            raise OSError(ctypes.get_last_error(), f"cannot read {size} bytes at {addr:#x}")
        return buf.raw

    def write(addr, data):
        got = ctypes.c_size_t()
        if not k.WriteProcessMemory(pi.hProcess, addr, data, len(data), ctypes.byref(got)) or got.value != len(data):
            raise OSError(ctypes.get_last_error(), f"cannot write {len(data)} bytes at {addr:#x}")

    def install():
        current = struct.unpack("<I", read(slot, 4))[0]
        if current == disk_value:
            return None  # not resolved yet (e.g. the native breakpoint of a WOW64 process)
        code = stub(seconds)
        page = None
        for hint in range(0x10000000, 0x70000000, 0x01000000):  # a 32-bit address for the IAT
            page = k.VirtualAllocEx(pi.hProcess, hint, 0x1000, MEM_COMMIT_RESERVE, PAGE_READWRITE)
            if page:
                break
        if not page or page >= 1 << 31:
            raise OSError("no 32-bit page for the clock stub")
        write(page, code)
        old = wintypes.DWORD()
        if not k.VirtualProtectEx(pi.hProcess, page, 0x1000, PAGE_EXECUTE_READ, ctypes.byref(old)):
            raise OSError(ctypes.get_last_error(), "cannot protect the clock stub")
        k.FlushInstructionCache(pi.hProcess, page, len(code))
        if not k.VirtualProtectEx(pi.hProcess, slot, 4, PAGE_READWRITE, ctypes.byref(old)):
            raise OSError(ctypes.get_last_error(), "cannot unprotect the IAT slot")
        write(slot, struct.pack("<I", page))
        restore = wintypes.DWORD()
        k.VirtualProtectEx(pi.hProcess, slot, 4, old.value, ctypes.byref(restore))
        if read(slot, 4) != struct.pack("<I", page) or read(page, len(code)) != code:
            raise OSError("the clock patch did not read back")
        return {"slot": slot, "resolved": current, "stub": page}

    try:
        while True:
            if not k.WaitForDebugEvent(event, INFINITE):
                raise OSError(ctypes.get_last_error(), "WaitForDebugEvent")
            code, pid, tid = struct.unpack_from("<III", event.raw, 0)
            status = DBG_CONTINUE
            if code == CREATE_PROCESS_DEBUG_EVENT or code == LOAD_DLL_DEBUG_EVENT:
                handle = struct.unpack_from("<Q" if ptr == 8 else "<I", event.raw, union)[0]
                if handle:
                    k.CloseHandle(handle)
            elif code == EXIT_PROCESS_DEBUG_EVENT:
                k.ContinueDebugEvent(pid, tid, DBG_CONTINUE)
                break
            elif code == EXCEPTION_DEBUG_EVENT:
                exc = struct.unpack_from("<I", event.raw, union)[0]
                if exc in (STATUS_BREAKPOINT, STATUS_WX86_BREAKPOINT):
                    if patched is None:
                        patched = install()
                else:
                    status = DBG_EXCEPTION_NOT_HANDLED
            k.ContinueDebugEvent(pid, tid, status)
            if patched is not None:
                k.DebugSetProcessKillOnExit(False)
                if not k.DebugActiveProcessStop(pi.dwProcessId):
                    raise OSError(ctypes.get_last_error(), "cannot detach from LINK")
                break
        if patched is None:
            raise OSError("LINK ran without its time() import being resolved at a loader breakpoint")
        k.WaitForSingleObject(pi.hProcess, INFINITE)
        exit_code = wintypes.DWORD()
        k.GetExitCodeProcess(pi.hProcess, ctypes.byref(exit_code))
        return exit_code.value
    except BaseException:
        k.DebugSetProcessKillOnExit(True)
        raise
    finally:
        k.CloseHandle(pi.hThread)
        k.CloseHandle(pi.hProcess)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--time", type=int, default=RETAIL_TIME, help="seconds since 1970 (default: retail's stamp)")
    ap.add_argument("command", nargs=argparse.REMAINDER, help="-- LINK and its arguments")
    args = ap.parse_args()
    command = args.command[1:] if args.command[:1] == ["--"] else args.command
    if not command:
        ap.error("no linker command")
    if sys.platform != "win32":
        raise SystemExit("link_clock.py needs the Windows debug API")
    return run(command, args.time)


if __name__ == "__main__":
    raise SystemExit(main())
