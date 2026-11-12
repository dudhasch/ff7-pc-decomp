"""Rebuild ff7.exe's resource object (the game icon) with the period RC and CVTRES.

The retail image has two resource leaves: a 32x32 8-bit icon (type 3, ID 1) and its icon group
(type 14, ID 101), both language 1033. No icon is committed: this script takes both leaves from
your own retail ff7.exe, writes them out as an .ico, compiles resources/ff7.rc with RC 5.00.1472.1
and converts the result with CVTRES 5.00.1668.1 into ff7_rsrc.obj, which the link takes.

    python tools/resources.py --exe ff7.exe --bin toolchain/vc5/bin --out build/resources
"""

import argparse
import shutil
import struct
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
RT_ICON, RT_GROUP_ICON = 3, 14
ICON_ID, GROUP_ID, LANG_EN_US = 1, 101, 1033


def u16(data, off):
    return struct.unpack_from("<H", data, off)[0]


def u32(data, off):
    return struct.unpack_from("<I", data, off)[0]


def rsrc_section(exe: bytes):
    """(raw bytes, RVA) of the image's .rsrc section."""
    pe = u32(exe, 0x3C)
    if exe[:2] != b"MZ" or exe[pe:pe + 4] != b"PE\0\0":
        raise SystemExit("not a PE image")
    nsec, optsize = u16(exe, pe + 6), u16(exe, pe + 20)
    table = pe + 24 + optsize
    for i in range(nsec):
        name, vsize, va, _rawsize, rawptr = struct.unpack_from("<8sIIII", exe, table + 40 * i)
        if name.rstrip(b"\0") == b".rsrc":
            return exe[rawptr:rawptr + vsize], va
    raise SystemExit("the image has no .rsrc section")


def resource_leaves(rsrc: bytes, rva: int) -> dict:
    """(type, id, language) -> payload for every leaf of the resource tree."""
    leaves = {}

    def walk(off, path):
        ids = u16(rsrc, off + 14) + u16(rsrc, off + 12)
        for i in range(ids):
            key, target = struct.unpack_from("<II", rsrc, off + 16 + 8 * i)
            if target & 0x80000000:
                walk(target & 0x7FFFFFFF, path + (key,))
            else:
                data_rva, size = struct.unpack_from("<II", rsrc, target)
                leaves[path + (key,)] = rsrc[data_rva - rva:data_rva - rva + size]

    walk(0, ())
    return leaves


def icon_file(exe: bytes) -> bytes:
    """An .ico made of the image's icon leaf, described by its icon-group leaf."""
    leaves = resource_leaves(*rsrc_section(exe))
    group = leaves[(RT_GROUP_ICON, GROUP_ID, LANG_EN_US)]
    icon = leaves[(RT_ICON, ICON_ID, LANG_EN_US)]
    width, height, colors, reserved, planes, bits, size, icon_id = struct.unpack_from("<BBBBHHIH", group, 6)
    if icon_id != ICON_ID or size != len(icon):
        raise SystemExit("the icon group does not describe the icon")
    # The ICO entry restates the group entry with the image's file offset (22), so RC rebuilds the
    # same group from it.
    return (struct.pack("<HHH", 0, 1, 1)
            + struct.pack("<BBBBHHII", width, height, colors, reserved, planes, bits, size, 22)
            + icon)


def find(bin_dir: Path, name: str) -> Path:
    for p in bin_dir.iterdir():
        if p.name.lower() == name:
            return p
    raise SystemExit(f"{name} is not in {bin_dir}")


def run(cmd, cwd):
    res = subprocess.run([str(c) for c in cmd], cwd=str(cwd), stdout=subprocess.PIPE,
                         stderr=subprocess.STDOUT, universal_newlines=True)
    if res.returncode:
        raise SystemExit(f"{Path(cmd[0]).name} exited {res.returncode}:\n{res.stdout}")


def build(exe: Path, bin_dir: Path, out: Path) -> Path:
    out.mkdir(parents=True, exist_ok=True)
    (out / "ff7.ico").write_bytes(icon_file(exe.read_bytes()))
    shutil.copyfile(REPO / "resources" / "ff7.rc", out / "ff7.rc")
    run([find(bin_dir, "rc.exe"), "/fo", "ff7.res", "ff7.rc"], out)
    run([find(bin_dir, "cvtres.exe"), "/nologo", "/readonly", "/machine:ix86", "/out:ff7_rsrc.obj", "ff7.res"], out)
    return out / "ff7_rsrc.obj"


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--exe", type=Path, required=True, help="your retail ff7.exe")
    ap.add_argument("--bin", type=Path, required=True, help="the VC5 bin directory (RC, RCDLL, CVTRES)")
    ap.add_argument("--out", type=Path, required=True)
    args = ap.parse_args()
    print(build(args.exe, args.bin, args.out))
    return 0


if __name__ == "__main__":
    sys.exit(main())
