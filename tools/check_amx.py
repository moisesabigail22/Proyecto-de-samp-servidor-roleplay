#!/usr/bin/env python3
from pathlib import Path
import struct
import sys

MAGIC = 0xF1E0

def check(path):
    data = Path(path).read_bytes()
    if len(data) < 24:
        print(f"FAIL {path}: archivo muy pequeno")
        return 1
    size, magic = struct.unpack_from("<IH", data, 0)
    file_version = data[6]
    amx_version = data[7]
    if magic != MAGIC or size != len(data) or file_version != 8 or amx_version != 8:
        print(f"FAIL {path}: AMX invalido o incompatible")
        return 1
    print(f"OK {path}: magic=0x{magic:04X} version={file_version}/{amx_version}")
    return 0

if __name__ == "__main__":
    sys.exit(max(check(p) for p in sys.argv[1:]))
