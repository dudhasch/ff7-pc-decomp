"""Build ff7.exe from the sources and check it against the retail image.

    python tools/build.py                       # compile, archive, link, compare
    python tools/build.py --jobs 4              # fewer parallel compiles
    python tools/build.py --toolchain D:\\vc5    # a toolchain folder elsewhere

The build is the 1997 one, as far as the image shows it:

1. Every object of config/objects.tsv is compiled by Visual C++ 5.0 with the Visual Studio 97 SP3
   compiler passes, at /Od (no optimisation), /MT (static CRT), with the per-file switches the
   table lists.
2. The objects of each source directory are archived into a static library with LIB.EXE.
3. LINK 5.10 links the 12 explicit objects, the icon resource and the libraries, with LIBCMT and the
   import libraries, in retail's library order. LINK takes from each library only the members
   something references, in its own search order, which is what places most of the image.
4. The result is compared byte for byte with your retail ff7.exe.

Two details of the link are not visible in source:

* LINK 5.10 folds identical COMDATs and picks the survivor with the C runtime's qsort. The copy of
  LINK used here imports MSVCR5.dll, the unchanged VS97 SP3 msvcrt.dll 5.00.7303 next to it, instead
  of the host's MSVCRT.dll, so the choice does not depend on the Windows version. Making that copy
  changes one byte per module (the import name) and nothing else.
* LINK stamps the image with the current time. tools/link_clock.py runs it with its clock stopped at
  retail's stamp, 1998-09-15 18:14:07 UTC.

Exit status: 0 when the image is byte-identical to retail, 1 when it differs, 2 on a build error.
"""

from __future__ import annotations

import argparse
import concurrent.futures
import csv
import hashlib
import os
import shutil
import struct
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "tools"))
import link_clock  # noqa: E402
import resources  # noqa: E402

RETAIL_SHA256 = "a33ccad7554f648c382a4dc0a5829c7fd4ce7ff56f6406b26fc671569875bb90"

# The common compiler switches. CMake's default release switches came first in the original
# command lines and the project's own after them; a later switch overrides an earlier one, so this
# compiles at /Od with /GX- and /MT. Kept in this order because it is the order the bytes were
# checked with.
COMMON = "/X /DWIN32 /D_WINDOWS /Zm1000 /GX /Zi /O2 /Ob1 /DNDEBUG /MD /W3 /GX- /MT /Od".split()

LINK_FLAGS = ["/nologo", "/INCREMENTAL:NO", "/BASE:0x400000", "/SUBSYSTEM:WINDOWS", "/ALIGN:4096", "/OPT:REF"]
GAME = "<game libraries>"
# Retail's library order, read off its import table and code layout: the DevStudio default list
# without the libraries the game does not use, winmm/ddraw/msacm32 first, the game libraries after
# LIBCMT, then libcpmt and OLDNAMES, then OpenGL, DirectSound/DirectInput and the GUID libraries.
LIBRARIES = ("winmm", "ddraw", "msacm32", "kernel32", "user32", "gdi32", "advapi32", "ole32", "libcmt", GAME,
             "libcpmt", "oldnames", "opengl32", "dsound", "dinput", "strmiids", "amstrmid", "dxguid")
# Libraries named by path; the others are found through /LIBPATH (the VC5 lib directory).
LIBRARY_PATHS = {"ddraw": "dx5/lib/ddraw.lib", "msacm32": "vc5/lib/msacm32.lib", "dsound": "dx5/lib/dsound.lib",
                 "dinput": "dx5/lib/dinput.lib", "strmiids": "dxmedia/lib/strmiids.lib",
                 "amstrmid": "dxmedia/lib/amstrmid.lib", "dxguid": "dx5/lib/dxguid.lib"}

# The LINK 5.10 modules and their SHA-256 once their MSVCRT.dll import reads MSVCR5.dll.
LINKER_MODULES = {
    "link.exe": "5f4bc61d45b47f12bde4ae270e6f4e6f284e40a1ee83ee591b7d6bb1ed8ece84",
    "MSPDB50.DLL": "700ce96eca0bd6a820985f13ecd4904b97a1630a6dc4d8845f2fe07571028381",
    "MSDIS100.DLL": "4a949b2179fb8aa2a7b50a9841761be6b081a1efa8b1d889ce5d1560dd55b495",
    "MSVCP50.DLL": "b497a238b00c05336cb960a11716758fdbebdc79976f1b626c3a3053ce2b2527",
    "CVTRES.EXE": "10d3b2ac775a435b3877249eb693809887e19bee2e0373df2a8d6cfab105dba8",
}


class BuildError(Exception):
    pass


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def check_toolchain(tc: Path) -> None:
    """Every file of config/toolchain.sha256 must be present with its hash."""
    problems = []
    for line in (REPO / "config" / "toolchain.sha256").read_text(encoding="utf-8").splitlines():
        if not line.strip() or line.startswith("#"):
            continue
        digest, rel = line.split(None, 1)
        p = tc / rel
        if not p.is_file():
            problems.append(f"missing {rel}")
        elif sha256(p) != digest:
            problems.append(f"differs: {rel}")
    if problems:
        shown = "\n  ".join(problems[:15]) + (f"\n  ... and {len(problems) - 15} more" if len(problems) > 15 else "")
        raise BuildError(f"the toolchain in {tc} is not the one the build expects:\n  {shown}\n"
                         "(see the README; --skip-toolchain-check builds anyway)")


def import_name_offset(data: bytes, dll: bytes) -> int:
    """File offset of the import descriptor name `dll` in a PE image; exactly one must exist."""
    pe = struct.unpack_from("<I", data, 0x3C)[0]
    nsec, opt_size = struct.unpack_from("<H", data, pe + 6)[0], struct.unpack_from("<H", data, pe + 20)[0]
    opt = pe + 24
    import_rva = struct.unpack_from("<I", data, opt + 104)[0]
    sections = [struct.unpack_from("<IIII", data, opt + opt_size + 40 * i + 8) for i in range(nsec)]

    def offset(rva):
        for vsize, va, rsize, raw in sections:
            if va <= rva < va + max(vsize, rsize):
                return raw + rva - va
        raise BuildError(f"RVA {rva:#x} is outside every section")

    found, at = [], offset(import_rva)
    while any(struct.unpack_from("<5I", data, at)):
        name_at = offset(struct.unpack_from("<5I", data, at)[3])
        if data[name_at:data.index(b"\0", name_at)].lower() == dll.lower():
            found.append(name_at)
        at += 20
    if len(found) != 1:
        raise BuildError(f"expected one import of {dll.decode()}, found {len(found)}")
    return found[0]


def stage_linker(tc: Path, out: Path) -> Path:
    """LINK 5.10 and its DLLs, importing MSVCR5.dll (the period msvcrt.dll) instead of MSVCRT.dll."""
    if out.exists():
        shutil.rmtree(out)
    out.mkdir(parents=True)
    for name, want in LINKER_MODULES.items():
        data = (tc / "link510" / name).read_bytes()
        at = import_name_offset(data, b"MSVCRT.dll")
        data = data[:at] + b"MSVCR5.dll" + data[at + 10:]
        (out / name).write_bytes(data)
        if hashlib.sha256(data).hexdigest() != want and not ARGS.skip_toolchain_check:
            raise BuildError(f"the staged {name} is not the expected LINK 5.10 module")
    shutil.copyfile(tc / "msvcrt500" / "msvcrt.dll", out / "MSVCR5.dll")
    return out / "link.exe"


def read_objects() -> list[dict]:
    lines = [l for l in (REPO / "config" / "objects.tsv").read_text(encoding="utf-8").splitlines()
             if l and not l.startswith("#")]
    return list(csv.DictReader(lines, delimiter="\t"))


def object_path(build: Path, source: str) -> Path:
    return build / "obj" / (source + ".obj")


def compile_one(cl: Path, includes: list[str], env: dict, build: Path, row: dict) -> tuple[str, int, str]:
    src = REPO / row["source"]
    obj = object_path(build, row["source"])
    obj.parent.mkdir(parents=True, exist_ok=True)
    cmd = [str(cl), "/nologo", "/TP", *includes, *COMMON, *row["options"].split(),
           f"/Fo{obj}", f"/Fd{obj.with_suffix('.pdb')}", "-c", str(src)]
    res = subprocess.run(cmd, cwd=str(build), env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                         universal_newlines=True, errors="replace")
    return row["source"], res.returncode, res.stdout


def compile_all(tc: Path, build: Path, rows: list[dict], jobs: int, env: dict) -> None:
    cl = tc / "vc5" / "bin" / "CL.EXE"
    includes = [f"-I{tc / 'dx5' / 'inc'}", f"-I{REPO / 'include'}", f"-I{tc / 'vc5' / 'include'}"]
    failed = []
    with concurrent.futures.ThreadPoolExecutor(max_workers=jobs) as pool:
        futures = [pool.submit(compile_one, cl, includes, env, build, r) for r in rows]
        for n, fut in enumerate(concurrent.futures.as_completed(futures), 1):
            source, rc, text = fut.result()
            if rc:
                failed.append(source)
                print(text.rstrip())
            if n % 50 == 0 or n == len(rows):
                print(f"  compiled {n}/{len(rows)}", flush=True)
    if failed:
        raise BuildError(f"{len(failed)} file(s) did not compile: " + ", ".join(failed[:10]))


def archive(tc: Path, build: Path, rows: list[dict], env: dict) -> list[Path]:
    """One static library per `library` value, members in table order; libraries in first-row order."""
    libdir = build / "lib"
    libdir.mkdir(parents=True, exist_ok=True)
    order: list[str] = []
    members: dict[str, list[str]] = {}
    for r in rows:
        if r["library"] == "explicit":
            continue
        if r["library"] not in members:
            order.append(r["library"])
            members[r["library"]] = []
        members[r["library"]].append(r["source"])
    libs = []
    for name in order:
        rsp = libdir / f"{name}.rsp"
        rsp.write_text("".join(f'"{object_path(build, s)}"\n' for s in members[name]), encoding="utf-8", newline="\n")
        lib = libdir / f"{name}.lib"
        if lib.exists():
            lib.unlink()
        res = subprocess.run([str(tc / "vc5" / "bin" / "LIB.EXE"), "/nologo", f"/OUT:{lib}", f"@{rsp}"],
                             env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, universal_newlines=True)
        if res.returncode:
            raise BuildError(f"LIB failed for {name}:\n{res.stdout}")
        libs.append(lib)
    return libs


def link(tc: Path, build: Path, rows: list[dict], libs: list[Path], rsrc: Path, linker: Path, env: dict) -> Path:
    exe, mapfile = build / "ff7.exe", build / "ff7.map"
    lines = LINK_FLAGS + [f'/LIBPATH:"{tc / "vc5" / "lib"}"', f'/MAP:"{mapfile}"', f'/OUT:"{exe}"', "/VERBOSE"]
    lines += [f'"{object_path(build, r["source"])}"' for r in rows if r["library"] == "explicit"]
    lines.append(f'"{rsrc}"')
    for name in LIBRARIES:
        if name == GAME:
            lines += [f'"{lib}"' for lib in libs]
        elif name in LIBRARY_PATHS:
            lines.append(f'"{tc / LIBRARY_PATHS[name]}"')
        else:
            lines.append(f'"{name}.lib"')
    rsp = build / "link.rsp"
    rsp.write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")
    if exe.exists():
        exe.unlink()
    cmd = [sys.executable, str(REPO / "tools" / "link_clock.py"), "--", str(linker), "/nologo", f"@{rsp}"]
    with (build / "link.log").open("w", encoding="latin1") as log:
        rc = subprocess.run(cmd, cwd=str(build), env=env, stdout=log, stderr=subprocess.STDOUT).returncode
    if rc:
        errors = [l for l in (build / "link.log").read_text(encoding="latin1").splitlines() if "error" in l.lower()]
        raise BuildError(f"LINK exited {rc}; see {build / 'link.log'}\n" + "\n".join(errors[:20]))
    return exe


def compare(exe: Path, retail: Path) -> int:
    a, b = exe.read_bytes(), retail.read_bytes()
    digest = hashlib.sha256(a).hexdigest()
    print(f"{exe}: {len(a):,} bytes, SHA-256 {digest}")
    if digest == RETAIL_SHA256 and a == b:
        print("byte-identical to retail ff7.exe")
        return 0
    diff = sum(1 for x, y in zip(a, b) if x != y) + abs(len(a) - len(b))
    print(f"differs from retail in {diff:,} bytes")
    return 1


def main() -> int:
    global ARGS
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--toolchain", type=Path, default=REPO / "toolchain", help="toolchain folder (default: toolchain/)")
    ap.add_argument("--exe", type=Path, default=REPO / "ff7.exe", help="your retail ff7.exe (default: ff7.exe)")
    ap.add_argument("--build", type=Path, default=REPO / "build", help="output folder (default: build/)")
    ap.add_argument("--jobs", type=int, default=os.cpu_count() or 4, help="parallel compiles")
    ap.add_argument("--skip-toolchain-check", action="store_true", help="build with files of other hashes")
    ARGS = args = ap.parse_args()
    if sys.platform != "win32":
        print("error: the period toolchain and tools/link_clock.py need Windows", file=sys.stderr)
        return 2
    tc, build = args.toolchain.resolve(), args.build.resolve()
    try:
        if not args.exe.is_file():
            raise BuildError(f"{args.exe} not found: the icon and the comparison need your retail ff7.exe")
        if sha256(args.exe) != RETAIL_SHA256:
            raise BuildError(f"{args.exe} is not the 1998 retail ff7.exe 1.00 (SHA-256 {RETAIL_SHA256})")
        if not args.skip_toolchain_check:
            check_toolchain(tc)
        build.mkdir(parents=True, exist_ok=True)
        env = dict(os.environ)
        env["PATH"] = str(tc / "vc5" / "bin") + os.pathsep + env.get("PATH", "")
        env["INCLUDE"] = env["LIB"] = ""
        rows = read_objects()
        print(f"compiling {len(rows)} objects with {args.jobs} jobs")
        compile_all(tc, build, rows, args.jobs, env)
        print("building the icon resource")
        rsrc = resources.build(args.exe, tc / "vc5" / "bin", build / "resources")
        libs = archive(tc, build, rows, env)
        print(f"linking {sum(r['library'] == 'explicit' for r in rows)} objects and {len(libs)} game libraries")
        linker = stage_linker(tc, build / "linker")
        exe = link(tc, build, rows, libs, rsrc, linker, env)
    except BuildError as e:
        print(f"error: {e}", file=sys.stderr)
        return 2
    stamp = struct.unpack_from("<I", exe.read_bytes(), struct.unpack_from("<I", exe.read_bytes(), 0x3C)[0] + 8)[0]
    if stamp != link_clock.RETAIL_TIME:
        print(f"warning: the image's timestamp is {stamp}, not retail's {link_clock.RETAIL_TIME}")
    return compare(exe, args.exe)


ARGS = None

if __name__ == "__main__":
    sys.exit(main())
