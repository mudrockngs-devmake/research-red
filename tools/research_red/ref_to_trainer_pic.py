#!/usr/bin/env python3
"""Research Red: turn a full-body reference image (RGBA, transparent
background) into a 64x64, 16-color trainer front pic.

Usage: ref_to_trainer_pic.py IN.png OUT.png [--height 60] [--colors 15] [--preview PREVIEW.png]

Steps: crop to the opaque figure, area-average down to --height pixels,
darken the silhouette edge into an outline, median-cut to --colors colors
(index 0 is transparent), then bottom-align and center in 64x64.
The result is a starting point; hand cleanup in an editor still helps.
"""
import argparse
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import rrpng  # noqa: E402

SIZE = 64
ALPHA_CUT = 200          # glow/antialias below this counts as background
TRANSPARENT = (115, 197, 164)  # FRLG trainer-pic background green (index 0)


def gba(c):
    """Round to the GBA's 5-bit color channels."""
    return tuple(min(255, (v >> 3) * 255 // 31) for v in c)


def median_cut(colors, n):
    boxes = [colors]
    while len(boxes) < n:
        boxes.sort(key=len, reverse=True)
        box = boxes.pop(0)
        if len(box) < 2:
            boxes.append(box)
            break
        ranges = [max(c[i] for c in box) - min(c[i] for c in box) for i in range(3)]
        ch = ranges.index(max(ranges))
        box.sort(key=lambda c: c[ch])
        mid = len(box) // 2
        boxes += [box[:mid], box[mid:]]
    return [tuple(sum(c[i] for c in b) // len(b) for i in range(3)) for b in boxes if b]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("src")
    ap.add_argument("dst")
    ap.add_argument("--height", type=int, default=60)
    ap.add_argument("--colors", type=int, default=15)
    ap.add_argument("--preview")
    a = ap.parse_args()

    w, h, rows = rrpng.read_png(a.src)
    ys = [y for y in range(h) if any(p[3] >= ALPHA_CUT for p in rows[y])]
    xs = [x for x in range(w) if any(rows[y][x][3] >= ALPHA_CUT for y in ys[::4])]
    x0, x1, y0, y1 = min(xs), max(xs) + 1, min(ys), max(ys) + 1
    scale = (y1 - y0) / a.height
    ow = min(SIZE, max(1, round((x1 - x0) / scale)))
    oh = a.height

    # Area-average each output pixel from the opaque source pixels it covers
    small = [[None] * ow for _ in range(oh)]
    for oy in range(oh):
        sy0, sy1 = y0 + int(oy * scale), y0 + max(int(oy * scale) + 1, int((oy + 1) * scale))
        for ox in range(ow):
            sx0, sx1 = x0 + int(ox * scale), x0 + max(int(ox * scale) + 1, int((ox + 1) * scale))
            tot, n, r, g, b = 0, 0, 0, 0, 0
            for sy in range(sy0, min(sy1, h)):
                row = rows[sy]
                for sx in range(sx0, min(sx1, w)):
                    p = row[sx]
                    tot += 1
                    if p[3] >= ALPHA_CUT:
                        n += 1
                        r += p[0]
                        g += p[1]
                        b += p[2]
            if tot and n * 2 >= tot:
                small[oy][ox] = (r // n, g // n, b // n)

    # Outline: opaque pixels touching transparency become a dark version of themselves
    out = [row[:] for row in small]
    for y in range(oh):
        for x in range(ow):
            c = small[y][x]
            if c is None:
                continue
            edge = any(
                not (0 <= y + dy < oh and 0 <= x + dx < ow) or small[y + dy][x + dx] is None
                for dy, dx in ((-1, 0), (1, 0), (0, -1), (0, 1)))
            if edge:
                out[y][x] = (c[0] // 5, c[1] // 5, c[2] // 6)

    opaque = [gba(c) for row in out for c in row if c is not None]
    pal = [gba(c) for c in median_cut(opaque, a.colors)]

    def nearest(c):
        return min(range(len(pal)), key=lambda i: sum((c[k] - pal[i][k]) ** 2 for k in range(3))) + 1

    idx = [[0] * SIZE for _ in range(SIZE)]
    ox0, oy0 = (SIZE - ow) // 2, SIZE - oh - 1
    cache = {}
    for y in range(oh):
        for x in range(ow):
            c = out[y][x]
            if c is not None:
                g = gba(c)
                if g not in cache:
                    cache[g] = nearest(g)
                idx[oy0 + y][ox0 + x] = cache[g]
    palette = [TRANSPARENT] + pal
    rrpng.write_png_indexed4(a.dst, SIZE, SIZE, idx, palette)

    if a.preview:
        s = 4
        prev = [[palette[idx[y // s][x // s]] for x in range(SIZE * s)] for y in range(SIZE * s)]
        rrpng.write_png_rgb(a.preview, SIZE * s, SIZE * s, prev)


if __name__ == "__main__":
    main()
