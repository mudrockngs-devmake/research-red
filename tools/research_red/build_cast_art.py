#!/usr/bin/env python3
"""Research Red: build the cast art set from the owner's full-body reference art.

For every reference image this writes graphics/research_red/cast/<name>/:
  battle.png    64x64 full-body trainer pic (FRLG style)
  portrait.png  64x64 head-and-chest dialogue portrait
  call.png      32x32 face close-up for video calls
All are 4-bit indexed PNGs, max 16 colors, index 0 = transparent (pure green).
Also writes a contact sheet (cast/contact_sheet.png) for review.

Usage: build_cast_art.py [REF_DIR]
Default REF_DIR: /mnt/e/Workspace/romhacks/research-red/art/ref/ResearchRed_ref_art
The reference art itself stays outside the repo.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import rrpng  # noqa: E402

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
REF_DIR = "/mnt/e/Workspace/romhacks/research-red/art/ref/ResearchRed_ref_art"
OUT_DIR = os.path.join(REPO, "graphics/research_red/cast")
GREEN = (0, 255, 0)
ALPHA_CUT = 200

# Poses where an arm sits above or beside the head fool the automatic head
# finder; give (head centre x, top of head y) in reference-image pixels.
HEAD_OVERRIDES = {
    "james": (500, 125),     # raised arm above the head
    "giovanni": (530, 30),   # raised hand beside the head
    "blaine": (540, 70),     # outstretched arm with a Poke Ball
}

# ref file -> cast name. Defaults: Ivy = lab coat, Jenny = casual; alternates get a suffix.
CAST = {
    "01_erika": "erika", "02_agatha": "agatha", "03_player_m": "player_m", "04_player_f": "player_f",
    "05_ivy_labcoat": "ivy", "06_jenny_police": "jenny_police", "07_jenny_casual": "jenny",
    "08_elm": "elm", "09_ivy_swimsuit": "ivy_swimsuit", "10_delia": "delia", "11_oak": "oak",
    "12_daisy": "daisy", "13_dj_mary": "dj_mary", "14_brock": "brock", "15_surge": "surge",
    "16_koga": "koga", "17_blaine": "blaine", "18_giovanni": "giovanni", "19_lance": "lance",
    "20_bruno": "bruno", "21_misty": "misty", "22_prof_westwood": "prof_westwood", "23_bill": "bill",
    "24_lorelei": "lorelei", "25_melody": "melody", "26_sabrina": "sabrina", "27_lisa": "lisa",
    "28_mrs_hale": "mrs_hale", "29_prof_hale": "prof_hale", "30_jessie": "jessie", "31_james": "james",
    "32_prof_mudrock": "prof_mudrock",
}


def load_ref(path):
    """RGBA rows; images without transparency get their dark background flood-filled away."""
    w, h, rows = rrpng.read_png(path)
    if any(p[3] < 128 for p in rows[0]):
        return w, h, rows
    rows = [list(r) for r in rows]
    seen = bytearray(w * h)
    stack = [(x, y) for x in range(w) for y in (0, h - 1)] + [(x, y) for y in range(h) for x in (0, w - 1)]
    while stack:
        x, y = stack.pop()
        if not (0 <= x < w and 0 <= y < h) or seen[y * w + x]:
            continue
        seen[y * w + x] = 1
        p = rows[y][x]
        if max(p[:3]) < 70:  # dark background and its dim glow
            rows[y][x] = (0, 0, 0, 0)
            stack += [(x + 1, y), (x - 1, y), (x, y + 1), (x, y - 1)]
    return w, h, rows


def figure_box(w, h, rows):
    ys = [y for y in range(0, h, 2) if any(p[3] >= ALPHA_CUT for p in rows[y][::2])]
    y0, y1 = min(ys), max(ys) + 2
    xs = [x for x in range(0, w, 2) if any(rows[y][x][3] >= ALPHA_CUT for y in range(y0, y1, 4))]
    x0, x1 = min(xs), max(xs) + 2
    # head centre: mean x of opaque pixels in the top 12% of the figure
    hx = [x for y in range(y0, y0 + (y1 - y0) * 12 // 100, 4) for x in range(x0, x1, 2) if rows[y][x][3] >= ALPHA_CUT]
    head_x = sum(hx) // len(hx) if hx else (x0 + x1) // 2
    head_top = y0
    return x0, y0, x1, y1, head_x, head_top


def downscale(rows, w, h, cx0, cy0, cw, ch, ow, oh):
    """Area-average the crop (cx0, cy0, cw x ch) to ow x oh; None = transparent."""
    sx, sy = cw / ow, ch / oh
    out = [[None] * ow for _ in range(oh)]
    for oy in range(oh):
        a0 = cy0 + int(oy * sy)
        a1 = cy0 + max(int(oy * sy) + 1, int((oy + 1) * sy))
        for ox in range(ow):
            b0 = cx0 + int(ox * sx)
            b1 = cx0 + max(int(ox * sx) + 1, int((ox + 1) * sx))
            tot = n = r = g = b = 0
            for yy in range(max(a0, 0), min(a1, h)):
                row = rows[yy]
                for xx in range(max(b0, 0), min(b1, w)):
                    p = row[xx]
                    tot += 1
                    if p[3] >= ALPHA_CUT:
                        n += 1
                        r += p[0]
                        g += p[1]
                        b += p[2]
            if tot and n * 2 >= tot:
                out[oy][ox] = (r // n, g // n, b // n)
    return out


def outline(img, border_is_edge):
    oh, ow = len(img), len(img[0])
    out = [row[:] for row in img]
    for y in range(oh):
        for x in range(ow):
            c = img[y][x]
            if c is None:
                continue
            for dy, dx in ((-1, 0), (1, 0), (0, -1), (0, 1)):
                yy, xx = y + dy, x + dx
                if not (0 <= yy < oh and 0 <= xx < ow):
                    if border_is_edge:
                        break
                    continue
                if img[yy][xx] is None:
                    break
            else:
                continue
            out[y][x] = (c[0] // 5, c[1] // 5, c[2] // 6)
    return out


def gba(c):
    return tuple(min(255, (v >> 3) * 255 // 31) for v in c)


def median_cut(colors, n):
    boxes = [colors]
    while len(boxes) < n:
        boxes.sort(key=len, reverse=True)
        box = boxes.pop(0)
        if len(box) < 2:
            boxes.append(box)
            break
        rng = [max(c[i] for c in box) - min(c[i] for c in box) for i in range(3)]
        ch = rng.index(max(rng))
        box.sort(key=lambda c: c[ch])
        boxes += [box[:len(box) // 2], box[len(box) // 2:]]
    return [tuple(sum(c[i] for c in b) // len(b) for i in range(3)) for b in boxes if b]


def save_indexed(path, img, size_w, size_h, ox0, oy0):
    opaque = [gba(c) for row in img for c in row if c is not None]
    pal = [gba(c) for c in median_cut(opaque, 15)] if opaque else []
    cache = {}

    def nearest(c):
        if c not in cache:
            cache[c] = 1 + min(range(len(pal)), key=lambda i: sum((c[k] - pal[i][k]) ** 2 for k in range(3)))
        return cache[c]
    idx = [[0] * size_w for _ in range(size_h)]
    for y, row in enumerate(img):
        for x, c in enumerate(row):
            if c is not None and 0 <= oy0 + y < size_h and 0 <= ox0 + x < size_w:
                idx[oy0 + y][ox0 + x] = nearest(gba(c))
    palette = [GREEN] + pal
    rrpng.write_png_indexed4(path, size_w, size_h, idx, palette)
    return [[palette[i] for i in r] for r in idx]


def build(ref_path, out_dir, name=""):
    w, h, rows = load_ref(ref_path)
    x0, y0, x1, y1, head_x, head_top = figure_box(w, h, rows)
    if name in HEAD_OVERRIDES:
        head_x, head_top = HEAD_OVERRIDES[name]
    fh = y1 - y0
    os.makedirs(out_dir, exist_ok=True)
    res = {}

    # battle: whole figure, 60 px tall, bottom-aligned in 64x64
    bh = 60
    bw = min(64, max(1, round((x1 - x0) * bh / fh)))
    img = outline(downscale(rows, w, h, x0, y0, x1 - x0, fh, bw, bh), True)
    res["battle"] = save_indexed(os.path.join(out_dir, "battle.png"), img, 64, 64, (64 - bw) // 2, 64 - bh - 1)

    # portrait: square crop from just above the head down ~40% of the figure
    side = int(fh * 0.40)
    cy0 = head_top - side // 20
    cx0 = head_x - side // 2
    img = outline(downscale(rows, w, h, cx0, cy0, side, side, 64, 64), False)
    res["portrait"] = save_indexed(os.path.join(out_dir, "portrait.png"), img, 64, 64, 0, 0)

    # call: face close-up, square crop of the head (~20% of the figure)
    side = int(fh * 0.20)
    cy0 = head_top - side // 8  # extra headroom so tall hair is not clipped
    cx0 = head_x - side // 2
    img = outline(downscale(rows, w, h, cx0, cy0, side, side, 32, 32), False)
    res["call"] = save_indexed(os.path.join(out_dir, "call.png"), img, 32, 32, 0, 0)
    return res


def main():
    ref_dir = sys.argv[1] if len(sys.argv) > 1 else REF_DIR
    sheet = []
    for ref, name in CAST.items():
        path = os.path.join(ref_dir, ref + ".png")
        if not os.path.exists(path):
            print("missing", path)
            continue
        sheet.append((name, build(path, os.path.join(OUT_DIR, name), name)))
        print("built", name)

    # contact sheet: one column per character (battle, portrait, call), 2x scale, 8 per row
    s, cw, chh, per = 2, 64 * 2 + 8, (64 + 64 + 32) * 2 + 16, 8
    W, H = per * cw, ((len(sheet) + per - 1) // per) * chh
    rows = [[(40, 40, 40)] * W for _ in range(H)]
    for i, (name, res) in enumerate(sheet):
        bx, by = (i % per) * cw + 4, (i // per) * chh + 4
        for img in (res["battle"], res["portrait"], res["call"]):
            for y in range(len(img) * s):
                for x in range(len(img[0]) * s):
                    rows[by + y][bx + x] = img[y // s][x // s]
            by += len(img) * s + 4
    rrpng.write_png_rgb(os.path.join(OUT_DIR, "contact_sheet.png"), W, H, rows)


if __name__ == "__main__":
    main()
