#!/usr/bin/env python3
"""Research Red (#5, #4): small pictures for the Pokédex banter cutscene.

Writes graphics/research_red/banter/emotes.png: three 16x16 speech bubbles
stacked top to bottom (! ? and a music note), 4-bit indexed, index 0
transparent. src/rr_portrait.c pops one above a speaker on some lines.

  python3 tools/research_red/build_banter_fx.py
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import rrpng  # noqa: E402

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
OUT = os.path.join(REPO, "graphics/research_red/banter/emotes.png")

PALETTE = [(0, 255, 0), (40, 32, 40), (252, 252, 248), (208, 56, 48), (56, 96, 200), (120, 72, 176),
           (200, 200, 208)] + [(0, 0, 0)] * 9

# a rounded bubble with a little tail (1 outline, 2 fill, 6 shading)
BUBBLE = [
    "................",
    "...1111111111...",
    "..122222222221..",
    ".12222222222221.",
    ".12222222222221.",
    ".12222222222221.",
    ".12222222222221.",
    ".12222222222221.",
    ".12222222222221.",
    ".12222222222221.",
    ".16222222222261.",
    "..166222222661..",
    "...11112211111..",
    "......1221......",
    ".......11.......",
    "................",
]
# the marks, drawn into the bubble (3 red, 4 blue, 5 purple)
MARKS = {
    "!": ["......33......", "......33......", "......33......", "......33......", "......33......",
          "..............", "......33......"],
    "?": [".....4444.....", "....44..44....", ".......44.....", "......44......", "......44......",
          "..............", "......44......"],
    "note": ["......5555....", "......5..5....", "......5..5....", "......5..5....", "....555.555...",
             "...5555555....", "....55..55...."],
}


def frame(mark):
    rows = [[int(c) if c != "." else 0 for c in r] for r in BUBBLE]
    for y, r in enumerate(MARKS[mark]):
        for x, c in enumerate(r):
            if c != ".":
                rows[3 + y][1 + x] = int(c)
    return rows


def main():
    rows = frame("!") + frame("?") + frame("note")
    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    rrpng.write_png_indexed4(OUT, 16, 48, rows, PALETTE)
    print("emote bubbles written:", os.path.relpath(OUT, REPO))


if __name__ == "__main__":
    main()
