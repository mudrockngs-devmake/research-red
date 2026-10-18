#!/usr/bin/env python3
"""Research Red: graphics for the intro video call (Discord-style, as in the
animatic). Writes graphics/research_red/intro_call/:

  bg.png / bg.bin    the call window: bezel, header bar (red "live" dot,
                     signal bars), Oak's video tile and the player's tile.
                     Tiles + a 32x32 tilemap using BG palette slot 1.
  circle_<who>.png   32x32 round avatars for the header (from cast/<who>/call.png;
                     Slowpoke from its front sprite; Elm has a second frame,
                     the extreme close-up of one eye)
  ring.png           the ring drawn over an avatar (white, or gold when speaking)

Palette indices the game changes at runtime (src/rr_intro_call.c):
  5 live dot, 6-9 signal bars, 10 Oak tile border, 11 player tile border.

Usage: python3 tools/research_red/build_call_ui.py [--preview OUT.png]
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import rrpng  # noqa: E402
from png_indexed import load_indexed  # noqa: E402

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
OUT = os.path.join(REPO, "graphics/research_red/intro_call")
CAST = os.path.join(REPO, "graphics/research_red/cast")

W, H = 240, 160
BG_PAL = [
    (17, 17, 17),     # 0 backdrop
    (43, 43, 51),     # 1 bezel
    (30, 43, 64),     # 2 call background
    (47, 49, 54),     # 3 video tile
    (24, 25, 28),     # 4 header bar
    (229, 57, 53),    # 5 live dot (blinks)
    (255, 255, 255),  # 6 signal bar 1
    (255, 255, 255),  # 7 signal bar 2
    (90, 90, 100),    # 8 signal bar 3
    (90, 90, 100),    # 9 signal bar 4
    (58, 63, 88),     # 10 Oak tile border (gold while Oak speaks)
    (58, 63, 88),     # 11 player tile border
    (36, 38, 42),     # 12 tile shading
    (0, 0, 0), (0, 0, 0), (0, 0, 0),
]
OAK_TILE = (8, 35, 123, 110)      # x0, y0, x1, y1 (inclusive)
PLAYER_TILE = (128, 35, 231, 110)


def rounded_rect(img, x0, y0, x1, y1, color, r=2):
    for y in range(y0, y1 + 1):
        for x in range(x0, x1 + 1):
            dx = min(x - x0, x1 - x)
            dy = min(y - y0, y1 - y)
            if dx < r and dy < r and (r - dx) + (r - dy) > r + 1:
                continue
            img[y][x] = color


def build_bg():
    img = [[0] * W for _ in range(H)]
    rounded_rect(img, 0, 0, W - 1, H - 1, 1, 6)
    rounded_rect(img, 3, 3, W - 4, H - 4, 2, 4)
    rounded_rect(img, 4, 4, W - 5, 31, 4, 3)
    for y in range(15, 20):                       # live dot
        for x in range(10, 15):
            if (x - 12) ** 2 + (y - 17) ** 2 <= 5:
                img[y][x] = 5
    for i in range(4):                            # signal bars
        h = 3 + i * 2
        for y in range(23 - h, 23):
            for x in range(212 + i * 4, 215 + i * 4):
                img[y][x] = 6 + i
    for (x0, y0, x1, y1), border in ((OAK_TILE, 10), (PLAYER_TILE, 11)):
        rounded_rect(img, x0, y0, x1, y1, border, 3)
        rounded_rect(img, x0 + 1, y0 + 1, x1 - 1, y1 - 1, 3, 2)
        for y in range(y1 - 12, y1):              # darker strip behind the name
            for x in range(x0 + 2, x1 - 1):
                img[y][x] = 12
    return img


def write_tiles(img, pal, name):
    tiles, tilemap = [], []
    for ty in range(32):
        for tx in range(32):
            if ty >= H // 8 or tx >= W // 8:
                tilemap.append(0)
                continue
            t = tuple(img[ty * 8 + y][tx * 8 + x] for y in range(8) for x in range(8))
            if t not in tiles:
                tiles.append(t)
            tilemap.append(tiles.index(t) | (1 << 12))   # BG palette slot 1
    cols = 16
    rows_n = (len(tiles) + cols - 1) // cols
    sheet = [[0] * (cols * 8) for _ in range(rows_n * 8)]
    for i, t in enumerate(tiles):
        for y in range(8):
            for x in range(8):
                sheet[(i // cols) * 8 + y][(i % cols) * 8 + x] = t[y * 8 + x]
    rrpng.write_png_indexed4(os.path.join(OUT, name + ".png"), cols * 8, rows_n * 8, sheet, pal)
    with open(os.path.join(OUT, name + ".bin"), "wb") as f:
        for e in tilemap:
            f.write(bytes((e & 0xFF, e >> 8)))
    return len(tiles)


def circle_mask(rows, radius=13.5):
    out = []
    for y in range(32):
        out.append([rows[y][x] if (x - 15.5) ** 2 + (y - 15.5) ** 2 <= radius ** 2 else 0 for x in range(32)])
    return out


def scale_crop(rows, x0, y0, size):
    """Crop a size x size square and scale it to 32x32 (nearest neighbour)."""
    return [[rows[y0 + y * size // 32][x0 + x * size // 32] for x in range(32)] for y in range(32)]


def build_circles():
    made = {}
    for who in ("jenny", "ivy"):
        w, h, rows, pal = load_indexed(os.path.join(CAST, who, "call.png"))
        made[who] = ([circle_mask(rows)], pal)
    # Elm: normal, then an extreme close-up of his left eye (from the portrait)
    w, h, rows, pal = load_indexed(os.path.join(CAST, "elm", "call.png"))
    w2, h2, prow, ppal = load_indexed(os.path.join(CAST, "elm", "portrait.png"))
    if ppal[:16] == pal[:16]:
        zoom_src = prow
    else:   # re-index the portrait crop into the call palette
        def near(c):
            return min(range(1, 16), key=lambda i: sum((a - b) ** 2 for a, b in zip(c, pal[i])))
        lut = [0] + [near(ppal[i]) for i in range(1, len(ppal))]
        zoom_src = [[lut[i] for i in r] for r in prow]
    made["elm"] = ([circle_mask(rows), circle_mask(scale_crop(zoom_src, 23, 22, 12))], pal)
    # Slowpoke: its head, from the front sprite
    w, h, rows, pal = load_indexed(os.path.join(REPO, "graphics/pokemon/slowpoke/anim_front.png"))
    made["slowpoke"] = ([circle_mask(scale_crop(rows, 8, 18, 26))], pal)
    for who, (frames, pal) in made.items():
        sheet = [r for f in frames for r in f]
        rrpng.write_png_indexed4(os.path.join(OUT, f"circle_{who}.png"), 32, 32 * len(frames), sheet,
                                 (list(pal) + [(0, 0, 0)] * 16)[:16])
    ring = [[1 if 13.5 ** 2 < (x - 15.5) ** 2 + (y - 15.5) ** 2 <= 15.2 ** 2 else 0 for x in range(32)] for y in range(32)]
    rrpng.write_png_indexed4(os.path.join(OUT, "ring.png"), 32, 32, ring,
                             [(0, 255, 0), (255, 255, 255)] + [(0, 0, 0)] * 14)
    return made


def main():
    os.makedirs(OUT, exist_ok=True)
    bg = build_bg()
    n = write_tiles(bg, BG_PAL, "bg")
    made = build_circles()
    print(f"call bg: {n} tiles; circles: {', '.join(made)}")
    if "--preview" in sys.argv:
        img = [[BG_PAL[i] for i in r] for r in bg]

        def blit(path, x0, y0, frame=0, size=None):
            w, h, rows, pal = load_indexed(path)
            size = size or w
            for y in range(size):
                for x in range(size):
                    i = rows[frame * size + y][x]
                    if i and 0 <= y0 + y < H and 0 <= x0 + x < W:
                        img[y0 + y][x0 + x] = pal[i]
        blit(os.path.join(CAST, "oak/portrait.png"), 14, 40)
        blit(os.path.join(REPO, "graphics/research_red/pokemon/slowpoke/portrait.png"), 58, 46)
        blit(os.path.join(CAST, "player_m/portrait.png"), 148, 40)
        for i, who in enumerate(("slowpoke", "elm", "jenny", "ivy")):
            blit(os.path.join(OUT, f"circle_{who}.png"), 102 + i * 26, 1, 1 if who == "elm" else 0, 32)
            blit(os.path.join(OUT, "ring.png"), 102 + i * 26, 1)
        s = 3
        big = [[img[y // s][x // s] for x in range(W * s)] for y in range(H * s)]
        rrpng.write_png_rgb(sys.argv[sys.argv.index("--preview") + 1], W * s, H * s, big)


if __name__ == "__main__":
    main()
