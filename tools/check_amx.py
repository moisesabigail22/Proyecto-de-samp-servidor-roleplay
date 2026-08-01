#!/usr/bin/env python3
"""Validate SA-MP/Open.mp AMX headers before deploying a gamemode.

This catches corrupted or unsupported P-code files early instead of letting the
server fail at runtime with: Run time error 17: Invalid/unsupported P-code file
format.
"""
from __future__ import annotations

import struct
import sys
from pathlib import Path

AMX_MAGIC = 0xF1E0
MIN_HEADER_SIZE = 24


def check(path: Path) -> int:
    data = path.read_bytes()
    if len(data) < MIN_HEADER_SIZE:
        print(f"FAIL {path}: file is too small to be a valid AMX ({len(data)} bytes)")
        return 1

    size, magic = struct.unpack_from("<IH", data, 0)
    file_version = data[6]
    amx_version = data[7]
    flags, defsize = struct.unpack_from("<HH", data, 8)

    errors: list[str] = []
    if magic != AMX_MAGIC:
        errors.append(f"bad magic 0x{magic:04X}, expected 0x{AMX_MAGIC:04X}")
    if size != len(data):
        errors.append(f"header size {size} does not match actual size {len(data)}")
    if file_version != 8 or amx_version != 8:
        errors.append(f"unsupported AMX versions file={file_version} amx={amx_version}; SA-MP expects 8/8")
    if defsize != 8:
        errors.append(f"unexpected public/native definition size {defsize}; SA-MP expects 8")

    if errors:
        print(f"FAIL {path}:")
        for error in errors:
            print(f"  - {error}")
        return 1

    print(
        f"OK {path}: size={size} magic=0x{magic:04X} "
        f"file_version={file_version} amx_version={amx_version} flags=0x{flags:04X}"
    )
    return 0


def main(argv: list[str]) -> int:
    if len(argv) < 2:
        print("Usage: tools/check_amx.py gamemodes/rpgm.amx [more.amx ...]")
        return 2
    return max(check(Path(arg)) for arg in argv[1:])


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
