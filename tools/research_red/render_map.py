#!/usr/bin/env python3
"""Research Red: render a map layout to a PNG (no third-party modules).

Usage: render_map.py LAYOUT_ID OUT.png [--grid]
  LAYOUT_ID  e.g. LAYOUT_PALLET_TOWN_PROFESSOR_OAKS_LAB (see data/layouts/layouts.json)
  --grid     draw a coordinate grid every 16 px (labels every 2 tiles are not drawn;
             count from the top-left, which is (0,0))

Reads built tileset data (tiles.4bpp, .gbapal), so run `make` first.
"""
import json
import os
import re
import struct
import sys
import zlib

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
NUM_TILES_IN_PRIMARY = 640
NUM_METATILES_IN_PRIMARY = 640
NUM_PALS_IN_PRIMARY = 7


def tileset_dir(symbol):
    """Map gTileset_X to its data folder via src/data/tilesets/metatiles.h."""
    name = symbol.replace("gTileset_", "")
    text = open(os.path.join(REPO, "src/data/tilesets/metatiles.h")).read()
    m = re.search(r"gMetatiles_%s\[\] = INCBIN_U16\(\"([^\"]+)/metatiles.bin\"\)" % name, text)
    return os.path.join(REPO, m.group(1))


def load_palettes(folder):
    pals = []
    for i in range(16):
        path = os.path.join(folder, "palettes", "%02d.gbapal" % i)
        raw = open(path, "rb").read() if os.path.exists(path) else bytes(32)
        cols = []
        for c in struct.unpack("<16H", raw[:32]):
            cols.append(((c & 31) * 255 // 31, ((c >> 5) & 31) * 255 // 31, ((c >> 10) & 31) * 255 // 31))
        pals.append(cols)
    return pals


def load_tiles(folder):
    raw = open(os.path.join(folder, "tiles.4bpp"), "rb").read()
    tiles = []
    for t in range(len(raw) // 32):
        px = []
        for b in raw[t * 32:(t + 1) * 32]:
            px += [b & 15, b >> 4]
        tiles.append(px)
    return tiles


def write_png(path, w, h, rgb_rows):
    raw = b"".join(b"\x00" + bytes(r) for r in rgb_rows)

    def chunk(t, b):
        return struct.pack(">I", len(b)) + t + b + struct.pack(">I", zlib.crc32(t + b))
    png = b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 2, 0, 0, 0))
    png += chunk(b"IDAT", zlib.compress(raw, 9)) + chunk(b"IEND", b"")
    open(path, "wb").write(png)


def main():
    layout_id, out = sys.argv[1], sys.argv[2]
    grid = "--grid" in sys.argv
    layouts = json.load(open(os.path.join(REPO, "data/layouts/layouts.json")))["layouts"]
    lay = next(l for l in layouts if l.get("id") == layout_id)
    w, h = lay["width"], lay["height"]
    blocks = struct.unpack("<%dH" % (w * h), open(os.path.join(REPO, lay["blockdata_filepath"]), "rb").read())
    pdir, sdir = tileset_dir(lay["primary_tileset"]), tileset_dir(lay["secondary_tileset"])
    ptiles, stiles = load_tiles(pdir), load_tiles(sdir)
    ppals, spals = load_palettes(pdir), load_palettes(sdir)
    pmeta = struct.unpack("<%dH" % (os.path.getsize(os.path.join(pdir, "metatiles.bin")) // 2), open(os.path.join(pdir, "metatiles.bin"), "rb").read())
    smeta = struct.unpack("<%dH" % (os.path.getsize(os.path.join(sdir, "metatiles.bin")) // 2), open(os.path.join(sdir, "metatiles.bin"), "rb").read())

    img = [[(0, 0, 0)] * (w * 16) for _ in range(h * 16)]
    for my in range(h):
        for mx in range(w):
            mid = blocks[my * w + mx] & 0x3FF
            meta = pmeta[mid * 8:mid * 8 + 8] if mid < NUM_METATILES_IN_PRIMARY else smeta[(mid - NUM_METATILES_IN_PRIMARY) * 8:(mid - NUM_METATILES_IN_PRIMARY) * 8 + 8]
            for layer in range(2):
                for i in range(4):
                    if len(meta) < 8:
                        continue
                    e = meta[layer * 4 + i]
                    tid, hf, vf, pal = e & 0x3FF, (e >> 10) & 1, (e >> 11) & 1, e >> 12
                    tiles = ptiles if tid < NUM_TILES_IN_PRIMARY else stiles
                    tid = tid if tid < NUM_TILES_IN_PRIMARY else tid - NUM_TILES_IN_PRIMARY
                    if tid >= len(tiles):
                        continue
                    cols = ppals[pal] if pal < NUM_PALS_IN_PRIMARY else spals[pal]
                    ox, oy = mx * 16 + (i & 1) * 8, my * 16 + (i >> 1) * 8
                    for py in range(8):
                        for pxx in range(8):
                            sx, sy = (7 - pxx) if hf else pxx, (7 - py) if vf else py
                            c = tiles[tid][sy * 8 + sx]
                            if c or layer == 0:
                                img[oy + py][ox + pxx] = cols[c]
    if grid:
        for y in range(h * 16):
            for x in range(w * 16):
                if x % 16 == 0 or y % 16 == 0:
                    r, g, b = img[y][x]
                    img[y][x] = (255, g // 2, b // 2)
    write_png(out, w * 16, h * 16, [sum((list(p) for p in row), []) for row in img])


if __name__ == "__main__":
    main()
