#!/usr/bin/env python3
"""Research Red: battle pictures of two people fighting as a team (#107).

Elm and Jenny, and Oak and Daisy, are one trainer in battle (one team of six,
one AI) shown as one picture of the pair, in singles and doubles alike, the
same way your side shows you and Ivy together (#7). Owner's pick 2026-10-28:
head to thighs (FireRed's own framing, like build_cast_art's battle_frlg), the
partner a step behind and overlapping.

Built from the same full-body reference art as the cast pictures, with the
cast builder's own steps (tools/research_red/build_cast_art.py), into
graphics/research_red/cast/pairs/<pair>/battle.png: 64x64, 15 colours plus
transparent (the pair shares one palette).

Your side the same way (owner 2026-10-28): you in front, Ivy a step behind,
pairs/player_{m,f}_ivy/back.png, 5 frames (64x320) for the Poke Ball throw,
with the lean from tools/research_red/build_back_pics.py. The game shows it
unless FLAG_RR_BACK_PIC_FACING is set (Debug > Scripts > 14: the #7 picture).

Usage: build_pair_art.py [REF_DIR]
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import build_cast_art as B  # noqa: E402

OUT_DIR = os.path.join(B.REPO, "graphics/research_red/cast/pairs")
# pair -> (in front, a step behind): reference art names as in build_cast_art.CAST
PAIRS = {
    "elm_jenny": ("08_elm", "07_jenny_casual"),
    "oak_daisy": ("11_oak", "12_daisy"),
}
# your side: back pictures with the throw frames
DUOS = {
    "player_m_ivy": ("03_player_m", "05_ivy_labcoat"),
    "player_f_ivy": ("04_player_f", "05_ivy_labcoat"),
}
THROW_LEAN = [(0, 0), (-4, 2), (-4, 1), (-4, 0), (-5, 2)]  # as build_back_pics.py: you lean, Ivy stays
CROP = 0.60      # head to thighs: this much of the figure from the head down
HEIGHT = 63      # that part fills the picture's height
BEHIND = 2       # the partner stands this many pixels higher (further back)
FIT = 60         # overlap them until the pair is this wide
DUO_FIT = 52     # your side stands closer, like Elm and Jenny (owner, #133)
BIGGER = {"03_player_m": 0.56}  # a smaller crop shows this figure a little bigger (owner, #133)


def figure(ref_dir, ref):
    """The figure from the head down to CROP of its height, HEIGHT px tall:
    rows of RGB (or None for clear), cropped to what's drawn."""
    w, h, rows = B.load_ref(os.path.join(ref_dir, ref + ".png"))
    x0, y0, x1, y1, _, _ = B.figure_box(w, h, rows)
    ch = int((y1 - y0) * BIGGER.get(ref, CROP))
    ow = max(1, round((x1 - x0) * HEIGHT / ch))
    img = B.outline(B.downscale(rows, w, h, x0, y0, x1 - x0, ch, ow, HEIGHT, keep_dark=B.KEEP_DARK), False)
    cols = [x for x in range(ow) if any(img[y][x] is not None for y in range(HEIGHT))]
    return [row[cols[0]:cols[-1] + 1] for row in img]


def build_pair(ref_dir, front, behind, lean=(0, 0), fit=FIT):
    f, b = figure(ref_dir, front), figure(ref_dir, behind)
    fw, bw = len(f[0]), len(b[0])
    overlap = max(0, fw + bw - fit)
    x0 = (64 - (fw + bw - overlap)) // 2
    canvas = [[None] * 64 for _ in range(64)]

    def paste(img, ox, oy):
        for y, row in enumerate(img):
            for x, c in enumerate(row):
                if c is not None and 0 <= oy + y < 64 and 0 <= ox + x < 64:
                    canvas[oy + y][ox + x] = c
    paste(b, x0 + fw - overlap, 64 - HEIGHT - BEHIND)   # the partner first, so the front one covers them
    paste(f, x0 + lean[0], 64 - HEIGHT + lean[1])
    return canvas


def main():
    ref_dir = sys.argv[1] if len(sys.argv) > 1 else B.REF_DIR
    for pair, (front, behind) in PAIRS.items():
        out = os.path.join(OUT_DIR, pair)
        os.makedirs(out, exist_ok=True)
        B.save_indexed(os.path.join(out, "battle.png"), build_pair(ref_dir, front, behind), 64, 64, 0, 0)
        print("built", pair)
    for duo, (front, behind) in DUOS.items():
        out = os.path.join(OUT_DIR, duo)
        os.makedirs(out, exist_ok=True)
        # one palette for all 5 frames: stack them, then save as one sheet
        sheet = [row for lean in THROW_LEAN for row in build_pair(ref_dir, front, behind, lean, DUO_FIT)]
        B.save_indexed(os.path.join(out, "back.png"), sheet, 64, 64 * len(THROW_LEAN), 0, 0)
        print("built", duo)


if __name__ == "__main__":
    main()
