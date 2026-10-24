#!/usr/bin/env python3
"""Research Red: the player's battle back sprites (#7).

Your side of a battle shows the player and Ivy together, seen from behind:
each one's back view (graphics/research_red/cast/<name>/back.png, drafts made
by tools/research_red/art/back_maker.py; a hand-drawn cast_handmade/<name>/back.png
wins), or their front-facing portrait if there is no back view. Ivy a little
behind on the left, the player in front on the right, one 64x64 picture with
16 colours.

FireRed's Red / Leaf back pics are 5 frames (the ball throw): the player leans
the way Red does (THROW_LEAN). For the game's
  graphics/trainers/back_pics/{red,leaf}_back_pic.png and
  graphics/trainers/palettes/{red,leaf}_back_pic.pal
(male player -> Red's slot, female -> Leaf's). They're written as drafts to
graphics/research_red/drafts/; tools/research_red/art/art_gate.py apply copies
them into the game once the owner approves "back_pics" (art_kit.toml).

  python3 tools/research_red/build_back_pics.py [--preview OUT.png]
"""
import os
import sys

from PIL import Image

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
CAST = os.path.join(REPO, "graphics/research_red/cast")
sys.path.insert(0, os.path.join(REPO, "tools/research_red/art"))
import artlib  # noqa: E402  (pixel grids: colour 0 = see-through; the in-game switch)
KEY = (0, 255, 0)  # the portraits' transparent green
FRAMES = 5


def cutout(name, scale):
    """A portrait with its green background made transparent, scaled."""
    im = Image.open(os.path.join(CAST, name, "portrait.png")).convert("RGBA")
    px = im.load()
    for y in range(im.height):
        for x in range(im.width):
            r, g, b, a = px[x, y]
            if (r, g, b) == KEY or (g > 200 and r < 60 and b < 60):
                px[x, y] = (0, 0, 0, 0)
    size = int(64 * scale)
    return im.resize((size, size), Image.NEAREST)


# How far the player leans in each frame of the Poke Ball throw: FireRed's
# Red moves his head (0,0) (-8,4) (-7,1) (-7,-1) (-10,5) (measured with
# tools/research_red/art/battle_kit.py); half that, so the player doesn't
# bump into Ivy.
THROW_LEAN = [(0, 0), (-4, 2), (-4, 1), (-4, 0), (-5, 2)]


def back_view(name, scale):
    """The character seen from behind (cast/<name>/back.png, made by
    tools/research_red/art/back_maker.py or drawn by hand), or the
    front-facing portrait while there is no back view yet."""
    path = os.path.join(CAST, name, "back.png")
    if not os.path.exists(path):
        return cutout(name, scale)
    im = artlib.to_rgba(artlib.load_grid(path))
    im = im.crop(im.getbbox())
    # back views are upper bodies: scale by height (scale = share of the 64 rows)
    k = 64 * scale / im.height
    return im.resize((max(1, round(im.width * k)), round(im.height * k)), Image.NEAREST)


def duo(player, lean=(0, 0)):
    canvas = Image.new("RGBA", (64, 64), (0, 0, 0, 0))
    have_backs = os.path.exists(os.path.join(CAST, player, "back.png"))
    ivy = back_view("ivy", 0.84 if have_backs else 0.80)        # behind, left
    you = back_view(player, 0.97 if have_backs else 0.86)       # in front, right
    canvas.alpha_composite(ivy, (-4, 64 - ivy.height))
    canvas.alpha_composite(you, (64 - you.width + 6 + lean[0], 64 - you.height + lean[1]))
    return canvas


def to_indexed(rgba):
    """16 colours, index 0 transparent (FireRed's backdrop green)."""
    opaque = Image.new("RGB", rgba.size, (0, 0, 0))
    opaque.paste(rgba, mask=rgba.split()[3])
    q = opaque.quantize(colors=15, method=Image.Quantize.MEDIANCUT, dither=Image.Dither.NONE)
    pal = q.getpalette()[:15 * 3]
    out = Image.new("P", rgba.size, 0)
    src, alpha = q.load(), rgba.split()[3].load()
    dst = out.load()
    for y in range(rgba.height):
        for x in range(rgba.width):
            dst[x, y] = 0 if alpha[x, y] < 128 else src[x, y] + 1
    out.putpalette([115, 197, 164] + pal + [0, 0, 0] * (256 - 16))
    return out


def write_pal(path, img):
    p = img.getpalette()[:48]
    lines = ["JASC-PAL", "0100", "16"] + ["%d %d %d" % tuple(p[i:i + 3]) for i in range(0, 48, 3)]
    open(path, "w", newline="\r\n").write("\n".join(lines) + "\n")


def main():
    previews = []
    pics = pals = artlib.DRAFTS_DIR
    os.makedirs(pics, exist_ok=True)
    for player, slot in (("player_m", "red"), ("player_f", "leaf")):
        # one palette for all 5 frames: build them together, then split
        tall = Image.new("RGBA", (64, 64 * FRAMES), (0, 0, 0, 0))
        for i in range(FRAMES):
            tall.alpha_composite(duo(player, THROW_LEAN[i]), (0, 64 * i))
        sheet = to_indexed(tall)
        frame = sheet.crop((0, 0, 64, 64))
        sheet.save(os.path.join(pics, f"{slot}_back_pic.png"))
        write_pal(os.path.join(pals, f"{slot}_back_pic.pal"), frame)
        previews.append(frame.convert("RGB"))
    if "--preview" in sys.argv:
        out = sys.argv[sys.argv.index("--preview") + 1]
        sheet = Image.new("RGB", (64 * len(previews) + 8, 64), (60, 60, 60))
        for i, p in enumerate(previews):
            sheet.paste(p, (i * 72, 0))
        sheet.resize((sheet.width * 4, sheet.height * 4), Image.NEAREST).save(out)
    print("back pic drafts written to %s (red = male player + Ivy, leaf = female player + Ivy);"
          " art_gate.py apply puts them in the game once approved" % os.path.relpath(pics, REPO))


if __name__ == "__main__":
    main()
