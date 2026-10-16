#!/usr/bin/env python3
"""Make a BPS patch. Usage: python3 create_bps.py SOURCE TARGET OUTPUT"""
import hashlib
import struct
import sys
import zlib
from pathlib import Path

MIN_RUN = 8


def num(v):
    out = bytearray()
    while True:
        b = v & 0x7F
        v >>= 7
        if v == 0:
            out.append(0x80 | b)
            return bytes(out)
        out.append(b)
        v -= 1


def run_at(src, tgt, pos):
    limit = min(len(src), len(tgt))
    end = pos
    while end < limit and src[end] == tgt[end]:
        end += 1
    return end - pos


def make_bps(src, tgt):
    p = bytearray(b"BPS1") + num(len(src)) + num(len(tgt)) + num(0)
    pos = 0
    while pos < len(tgt):
        r = run_at(src, tgt, pos)
        if r >= MIN_RUN:
            p += num(((r - 1) << 2) | 0)
            pos += r
            continue
        start = pos
        pos += 1
        while pos < len(tgt) and run_at(src, tgt, pos) < MIN_RUN:
            pos += 1
        p += num(((pos - start - 1) << 2) | 1) + tgt[start:pos]
    p += struct.pack("<I", zlib.crc32(src) & 0xFFFFFFFF)
    p += struct.pack("<I", zlib.crc32(tgt) & 0xFFFFFFFF)
    p += struct.pack("<I", zlib.crc32(p) & 0xFFFFFFFF)
    return bytes(p)


def main():
    if len(sys.argv) != 4:
        print(__doc__)
        return 1
    src = Path(sys.argv[1]).read_bytes()
    tgt = Path(sys.argv[2]).read_bytes()
    patch = make_bps(src, tgt)
    Path(sys.argv[3]).write_bytes(patch)
    for label, data in (("Source", src), ("Target", tgt), ("Patch", patch)):
        print(f"{label}: {len(data):,} bytes, SHA-1 {hashlib.sha1(data).hexdigest()}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
