#!/usr/bin/env python3
"""Research Red: read an indexed (4- or 8-bit) PNG as palette indices."""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import rrpng  # noqa: E402


def load_indexed(path):
    """Return (w, h, rows of palette indices, palette)."""
    import re
    import struct
    import zlib
    data = open(path, "rb").read()
    pos, idat, plte = 8, b"", b""
    while pos < len(data):
        n, t = struct.unpack(">I4s", data[pos:pos + 8])
        c = data[pos + 8:pos + 8 + n]
        if t == b"IHDR":
            w, h, bd = struct.unpack(">IIB", c[:9])
        elif t == b"PLTE":
            plte = c
        elif t == b"IDAT":
            idat += c
        pos += 12 + n
    stride = (w * bd + 7) // 8
    raw = rrpng._unfilter(zlib.decompress(idat), stride, 1, h)
    rows = []
    for y in range(h):
        r = []
        for x in range(w):
            b = raw[y * stride + x * bd // 8]
            r.append((b >> (4 - 4 * (x % 2))) & 15 if bd == 4 else b)
        rows.append(r)
    pal = [tuple(plte[i:i + 3]) for i in range(0, len(plte), 3)]
    return w, h, rows, pal
