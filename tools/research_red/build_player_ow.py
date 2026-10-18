#!/usr/bin/env python3
"""Research Red: the player's walking/running overworld sheets.

  Male   -> Professor Birch (pokeemerald, graphics/research_red/player_ow/src/prof_birch.png)
  Female -> Reporter (FireRed's reporter_f)

Both are re-indexed to FireRed's player palette (player.pal) so they share it
with the untouched Red/Leaf bike, surf and fishing sheets. Each output sheet has
20 frames in the order the player's pic table expects: 0-8 walking, 9-17
running (the walk frames again; neither source has run frames), 18-19 spare.

Writes graphics/research_red/player_ow/{male,female}_normal.png; the INCBINs in
src/data/object_events/object_event_graphics.h point there.
Usage: python3 tools/research_red/build_player_ow.py [--preview OUT.png]
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import rrpng  # noqa: E402
from png_indexed import load_indexed  # noqa: E402

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
OUT = os.path.join(REPO, "graphics/research_red/player_ow")

# source index -> player.pal index (picked by hand: Birch's brown hair becomes
# the player's red hair, his green shirt becomes blue; the Reporter's pink
# outfit becomes red)
SOURCES = {
    "male": (os.path.join(OUT, "src/prof_birch.png"),
             [0, 2, 2, 3, 4, 10, 10, 6, 7, 5, 6, 11, 12, 8, 9, 15]),
    "female": (os.path.join(REPO, "graphics/object_events/pics/people/reporter_f.png"),
               [0, 2, 2, 3, 4, 7, 6, 15, 11, 12, 8, 10, 10, 6, 9, 15]),
}
# frame order: walk 0-8, run south (9-11), north (12-14), west (15-17), spare
FRAMES = [0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 3, 4, 1, 5, 6, 2, 7, 8, 0, 0]


def read_pal(path):
    lines = open(path).read().split("\n")[3:19]
    return [tuple(map(int, line.split())) for line in lines]


def main():
    pal = read_pal(os.path.join(REPO, "graphics/object_events/palettes/player.pal"))
    previews = []
    for who, (src, remap) in SOURCES.items():
        w, h, rows, _ = load_indexed(src)
        out = [[remap[rows[y][f * 16 + x]] for f in FRAMES for x in range(16)] for y in range(h)]
        rrpng.write_png_indexed4(os.path.join(OUT, f"{who}_normal.png"), 16 * len(FRAMES), h, out, pal)
        previews.append(out)
    if "--preview" in sys.argv:
        s = 4
        img = []
        for rows in previews:
            for y in range(32 * s):
                img.append([(200, 220, 200) if rows[y // s][x // s] == 0 else pal[rows[y // s][x // s]]
                            for x in range(16 * 9 * s)])
        rrpng.write_png_rgb(sys.argv[sys.argv.index("--preview") + 1], 16 * 9 * s, len(img), img)


if __name__ == "__main__":
    main()
