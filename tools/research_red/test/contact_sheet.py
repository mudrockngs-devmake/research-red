#!/usr/bin/env python3
"""Research Red: read the runner's screenshots and lay them out in a grid.

  python3 contact_sheet.py OUT.png SHOT.png...   (up to 30, evenly sampled)
"""
import os
import struct
import sys
import zlib

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import rrpng  # noqa: E402

COLUMNS = 5
MAX_SHOTS = 30
GAP = 4


def read_png(path):
    """8-bit RGB PNG (as written by rr_run) -> (w, h, rows of (r, g, b))."""
    d = open(path, "rb").read()
    pos, idat = 8, b""
    while pos < len(d):
        n, t = struct.unpack(">I4s", d[pos:pos + 8])
        c = d[pos + 8:pos + 8 + n]
        if t == b"IHDR":
            w, h = struct.unpack(">II", c[:8])
        elif t == b"IDAT":
            idat += c
        pos += 12 + n
    raw = zlib.decompress(idat)
    stride = w * 3 + 1
    out, prev = [], bytearray(w * 3)
    for y in range(h):
        f = raw[y * stride]
        line = bytearray(raw[y * stride + 1:(y + 1) * stride])
        for i in range(len(line)):
            a = line[i - 3] if i >= 3 else 0
            b = prev[i]
            c = prev[i - 3] if i >= 3 else 0
            if f == 1:
                line[i] = (line[i] + a) & 255
            elif f == 2:
                line[i] = (line[i] + b) & 255
            elif f == 3:
                line[i] = (line[i] + (a + b) // 2) & 255
            elif f == 4:
                pa, pb, pc = abs(b - c), abs(a - c), abs(a + b - 2 * c)
                pr = a if pa <= pb and pa <= pc else (b if pb <= pc else c)
                line[i] = (line[i] + pr) & 255
        out.append([tuple(line[x * 3:x * 3 + 3]) for x in range(w)])
        prev = line
    return w, h, out


def write_sheet(out_path, paths):
    if len(paths) > MAX_SHOTS:   # keep the first and last, sample the rest evenly
        step = (len(paths) - 1) / (MAX_SHOTS - 1)
        paths = [paths[round(i * step)] for i in range(MAX_SHOTS)]
    imgs = [read_png(p) for p in paths]
    w, h = 240, 160
    cols = min(COLUMNS, len(imgs))
    rows = (len(imgs) + cols - 1) // cols
    W, H = cols * (w + GAP) - GAP, rows * (h + GAP) - GAP
    canvas = [[(20, 20, 30)] * W for _ in range(H)]
    for k, (iw, ih, px) in enumerate(imgs):
        ox, oy = (k % cols) * (w + GAP), (k // cols) * (h + GAP)
        for y in range(min(ih, h)):
            canvas[oy + y][ox:ox + min(iw, w)] = px[y][:w]
    rrpng.write_png_rgb(out_path, W, H, canvas)


if __name__ == "__main__":
    write_sheet(sys.argv[1], sys.argv[2:])
