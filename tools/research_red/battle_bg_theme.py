#!/usr/bin/env python3
"""Research Red: battle background colours per location (#74).

Recolours FireRed's existing battle backgrounds so each place feels like
itself. Only the palettes (.pal) change: same tiles, same maps, same number
of colours, so nothing can overflow. Which background a place uses is in
src/battle_setup.c (BattleSetup_GetEnvironmentId; Viridian Forest -> Long grass).

  python3 tools/research_red/battle_bg_theme.py           # apply, then make firered
  python3 tools/research_red/battle_bg_theme.py --restore # back to FireRed's colours
  python3 tools/research_red/battle_bg_theme.py --preview <out dir>   # render every background

  routes (grass)       warmer, sunnier greens and a cream sky
  Viridian Forest      long grass, deep forest greens
  towns and paths      plain: a sandy platform under a soft sky
  Oak's Lab, homes     building: a warm wooden floor
  Gym leaders          leader: Pewter stone
  sea and ponds        richer blues
  caves                richer earth
"""
import colorsys
import os
import struct
import subprocess
import sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
T = "graphics/battle_terrain/"


def hls_shift(dh=0.0, sat=1.0, light=1.0, only=None):
    """A colour rule: shift hue, scale saturation and lightness (optionally only for hues in `only`)."""
    def f(rgb):
        h, l, s = colorsys.rgb_to_hls(*(c / 255 for c in rgb))
        if only and not (only[0] <= h <= only[1]):
            return rgb
        h = (h + dh) % 1.0
        s, l = min(1, s * sat), min(1, l * light)
        return tuple(round(c * 255) for c in colorsys.hls_to_rgb(h, l, s))
    return f


def ramp(dark, light, lo=0.78, hi=1.0):
    """A colour rule: map brightness onto a two-colour ramp (keeps the shading)."""
    def hexrgb(h):
        return tuple(int(h[i:i + 2], 16) for i in (1, 3, 5))
    d, li = hexrgb(dark), hexrgb(light)

    def f(rgb):
        lum = (0.299 * rgb[0] + 0.587 * rgb[1] + 0.114 * rgb[2]) / 255
        t = max(0.0, min(1.0, (lum - lo) / (hi - lo)))
        return tuple(round(d[i] + (li[i] - d[i]) * t) for i in range(3))
    return f


def chain(*fs):
    def f(rgb):
        for g in fs:
            rgb = g(rgb)
        return rgb
    return f


GREEN = (0.18, 0.45)
SKY = (0.48, 0.70)

# palette file -> colour rule
THEME = {
    T + "grass/terrain.pal": chain(hls_shift(-0.02, 1.15, 0.97, GREEN), hls_shift(-0.45, 0.45, 1.0, SKY)),
    T + "longgrass/terrain.pal": chain(ramp("#2c4a30", "#b4cc98", lo=0.45, hi=0.95), hls_shift(0.0, 1.1, 1.0)),
    T + "indoor/plain.pal": chain(hls_shift(-0.13, 0.5, 0.98, GREEN), hls_shift(0.0, 0.8, 1.0, SKY)),
    T + "building/terrain.pal": ramp("#a07850", "#f6e8cc"),
    T + "indoor/leader.pal": ramp("#7c6c5c", "#ece2d0"),
    T + "water/terrain.pal": hls_shift(0.0, 1.25, 0.95),
    T + "pond/terrain.pal": hls_shift(0.0, 1.2, 0.95),
    T + "cave/terrain.pal": hls_shift(-0.01, 1.15, 0.93),
}
MAGENTA = (255, 0, 255)


def read_pal(path):
    words = open(path).read().split()
    n = int(words[2])
    v = list(map(int, words[3:3 + 3 * n]))
    return [tuple(v[i:i + 3]) for i in range(0, 3 * n, 3)]


def write_pal(path, colours):
    lines = ["JASC-PAL", "0100", str(len(colours))] + ["%d %d %d" % c for c in colours]
    open(path, "w", newline="\r\n").write("\n".join(lines) + "\n")


def gba(c):
    """Round to what the GBA can show (5 bits per channel)."""
    return tuple((v >> 3) << 3 for v in c)


def apply():
    for rel, rule in THEME.items():
        path = os.path.join(REPO, rel)
        pal = read_pal(path)
        out = []
        for i, c in enumerate(pal):
            # slot 0 of each 16 is transparent; magenta marks unused slots
            out.append(c if i % 16 == 0 or c == MAGENTA or c == (0, 0, 0) else gba(rule(c)))
        write_pal(path, out)
        gbapal = path[:-4] + ".gbapal"
        if os.path.exists(gbapal):
            os.remove(gbapal)  # generated from the .pal by make
    os.utime(os.path.join(REPO, "src/battle_bg.c"))
    print("battle background colours applied: run make firered")


def restore():
    subprocess.run(["git", "-C", REPO, "checkout", "--"] + list(THEME), check=True)
    for rel in THEME:
        g = os.path.join(REPO, rel[:-4] + ".gbapal")
        if os.path.exists(g):
            os.remove(g)
    os.utime(os.path.join(REPO, "src/battle_bg.c"))
    print("FireRed's battle background colours restored: run make firered")


def preview(out_dir):
    """Render each background (tiles + map + palette) to <out_dir>/<name>.png."""
    from PIL import Image
    os.makedirs(out_dir, exist_ok=True)
    jobs = [("grass", "grass/terrain"), ("forest", "longgrass/terrain"), ("plain", "indoor/plain"),
            ("building", "building/terrain"), ("leader", "indoor/leader"), ("water", "water/terrain"),
            ("pond", "pond/terrain"), ("cave", "cave/terrain")]
    for name, rel in jobs:
        folder = os.path.join(REPO, T, rel.split("/")[0])
        tiles = Image.open(os.path.join(folder, "terrain.png"))
        tmap = open(os.path.join(folder, "terrain.bin"), "rb").read()
        pal = read_pal(os.path.join(REPO, T, rel + ".pal"))
        img = Image.new("RGB", (240, 112))
        px, out, tw = tiles.load(), img.load(), tiles.width // 8
        for i in range(len(tmap) // 2):
            e = struct.unpack_from("<H", tmap, 2 * i)[0]
            tx, ty = (i % 32) * 8, (i // 32) * 8
            if tx >= 240 or ty >= 112:
                continue
            tile, hf, vf, pn = e & 0x3FF, (e >> 10) & 1, (e >> 11) & 1, (e >> 12) & 15
            sx, sy = (tile % tw) * 8, (tile // tw) * 8
            for y in range(8):
                for x in range(8):
                    k = (pn - 2) * 16 + px[sx + (7 - x if hf else x), sy + (7 - y if vf else y)]
                    out[tx + x, ty + y] = pal[k] if 0 <= k < len(pal) else MAGENTA
        img.save(os.path.join(out_dir, name + ".png"))
    print("previews in", out_dir)


if __name__ == "__main__":
    if "--restore" in sys.argv:
        restore()
    elif "--preview" in sys.argv:
        preview(sys.argv[sys.argv.index("--preview") + 1])
    else:
        apply()
