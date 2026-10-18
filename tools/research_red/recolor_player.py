#!/usr/bin/env python3
"""Research Red: temporary player overworld sprites made by recoloring
FireRed's Red and Leaf sheets (until custom sprites exist).

  Red  -> red cap reads as red hair; the red jacket becomes a white lab coat
  Leaf -> brown hair becomes red; her top becomes a white lab coat

Writes graphics/research_red/player_ow/<sheet>.png; the INCBINs in
src/data/object_events/object_event_graphics.h point there.
Usage: python3 tools/research_red/recolor_player.py [--preview OUT.png]
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import rrpng  # noqa: E402

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SRC = os.path.join(REPO, "graphics/object_events/pics/people")
OUT = os.path.join(REPO, "graphics/research_red/player_ow")
SHEETS = ["normal", "bike", "fish", "item", "surf", "surf_run", "vs_seeker_bike", "watering"]

# player.pal indices
BROWN, SKIN_LIGHT, SKIN, BROWN_DARK, BLUE, NAVY, BLUE_GRAY, RED_DARK = 1, 2, 3, 4, 5, 6, 7, 8
WHITE, LIGHT_GRAY, RED_LIGHT, RED_MID, YELLOW_LIGHT, YELLOW, OUTLINE = 9, 10, 11, 12, 13, 14, 15
TRANSPARENT = 0
REDS = (RED_DARK, RED_LIGHT, RED_MID)
COAT = {RED_LIGHT: WHITE, RED_MID: LIGHT_GRAY, RED_DARK: BLUE_GRAY}
FACE_OR_HAIR = (BROWN, SKIN_LIGHT, SKIN, BROWN_DARK, NAVY)


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


def recolor_red(rows):
    """Per column: everything above the first face/hair pixel is the cap. The cap
    (including its white band and gray brim) becomes all red, reading as red
    hair; red below it is the jacket and becomes a white lab coat."""
    w, h = len(rows[0]), len(rows)
    for x in range(w):
        in_cap, seen_red = True, False
        for y in range(h):
            i = rows[y][x]
            if i in (TRANSPARENT, OUTLINE):
                continue
            if in_cap and i in FACE_OR_HAIR:
                in_cap = False
            if in_cap:
                if i in REDS:
                    seen_red = True
                elif seen_red and i == WHITE:
                    rows[y][x] = RED_LIGHT
                elif seen_red and i == LIGHT_GRAY:
                    rows[y][x] = RED_MID
            elif i in COAT:
                rows[y][x] = COAT[i]


def recolor_leaf(rows, clothes=True):
    """Brown hair becomes red; below the face (first skin pixel in the column)
    her blue top becomes a white coat and her red skirt becomes blue jeans.
    clothes=False (bike/surf sheets) changes only the hair, so the bike and the
    surf Pokemon keep their colors."""
    w, h = len(rows[0]), len(rows)
    for x in range(w):
        below_face = False
        for y in range(h):
            i = rows[y][x]
            if i in (SKIN, SKIN_LIGHT):
                below_face = True
            if i == BROWN_DARK:
                rows[y][x] = RED_MID
            elif i == BROWN:
                rows[y][x] = RED_LIGHT
            elif not clothes:
                continue
            elif below_face and i == BLUE:
                rows[y][x] = WHITE
            elif below_face and i == NAVY:
                rows[y][x] = LIGHT_GRAY
            elif below_face and i in (RED_MID, RED_LIGHT):
                rows[y][x] = BLUE


def main():
    os.makedirs(OUT, exist_ok=True)
    previews = []
    for who, fn in (("red", recolor_red), ("green", recolor_leaf)):
        for sheet in SHEETS:
            name = f"{who}_{sheet}.png"
            w, h, rows, pal = load_indexed(os.path.join(SRC, name))
            before = [r[:] for r in rows]
            if who == "green":
                fn(rows, clothes=sheet in ("normal", "item", "fish", "watering"))
            else:
                fn(rows)
            rrpng.write_png_indexed4(os.path.join(OUT, name), w, h, rows, pal[:16])
            if sheet in ("normal", "bike", "surf"):
                previews.append((before, rows, pal))
    if "--preview" in sys.argv:
        out = sys.argv[sys.argv.index("--preview") + 1]
        s = 3
        W = max(len(p[0][0]) for p in previews) * s
        img = []
        for before, after, pal in previews:
            for rows in (before, after):
                for y in range(32 * s):
                    img.append([pal[rows[y // s][x // s]] if x // s < len(rows[0]) else (40, 40, 40) for x in range(W)])
        rrpng.write_png_rgb(out, W, len(img), img)


if __name__ == "__main__":
    main()
