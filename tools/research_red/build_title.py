#!/usr/bin/env python3
"""Research Red: the title screen picture (#12).

One full-screen picture (240x160, up to 224 colours, 8 bits per pixel) drawn
here from the game's own art: notebook paper (the interface's notebook theme),
the Research Red logo, and Slowpoke wrapped in a lab coat like a blanket
(Slowpoke's front sprite with a coat drawn over its back). "Press Start" and
the version are printed by the game on top (src/rr_title.c).

Writes graphics/research_red/title/title.png (indexed, 8bpp; make turns it into
title.8bpp) and title.pal (palette colours 0-223; 224-255 stay free for the
text), plus a 3x preview with the text mocked in:

  python3 tools/research_red/build_title.py [--preview OUT.png]
"""
import os
import sys

from PIL import Image, ImageDraw, ImageFont

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
OUT = os.path.join(REPO, "graphics/research_red/title")
FONT = "/usr/share/fonts/truetype/dejavu/DejaVuSerif-Bold.ttf"
W, H = 240, 160
COLOURS = 224

PAPER = (248, 240, 216)
RULE = (214, 224, 232)
MARGIN = (226, 160, 150)
HOLE = (232, 216, 176)
HOLE_SHADOW = (208, 184, 136)
INK = (88, 64, 48)
RED = (200, 56, 48)
RED_DARK = (144, 32, 32)
RED_LIGHT = (240, 112, 96)
SHADOW = (232, 216, 176)
COAT = (250, 250, 250)
COAT_SHADE = (214, 218, 228)
COAT_LINE = (72, 72, 88)
PEN = (64, 96, 200)


def paper():
    im = Image.new("RGB", (W, H), PAPER)
    d = ImageDraw.Draw(im)
    for y in range(52, H, 12):            # ruled lines below the logo
        d.line((0, y, W, y), fill=RULE)
    d.line((22, 0, 22, H), fill=MARGIN)   # the red margin line
    for y in (28, 80, 132):               # binder holes
        d.ellipse((6, y - 5, 16, y + 5), fill=HOLE_SHADOW)
        d.ellipse((7, y - 4, 15, y + 3), fill=HOLE)
    return im


def logo(im):
    """'Research Red': crisp serif letters, a red gradient, a dark outline and
    a soft drop shadow."""
    font = ImageFont.truetype(FONT, 23)
    text = "Research Red"
    probe = ImageDraw.Draw(Image.new("1", (1, 1)))
    l, t, r, b = probe.textbbox((0, 0), text, font=font)
    tw, th = r - l, b - t
    mask = Image.new("1", (tw + 8, th + 8), 0)
    md = ImageDraw.Draw(mask)
    md.fontmode = "1"
    md.text((4 - l, 4 - t), text, font=font, fill=1)
    x0, y0 = 22 + (W - 22 - mask.width) // 2, 12
    px = mask.load()
    out = im.load()

    def solid(x, y):
        return 0 <= x < mask.width and 0 <= y < mask.height and px[x, y]

    for y in range(mask.height):
        for x in range(mask.width):
            if not solid(x, y):
                if solid(x - 2, y - 2):
                    out[x0 + x, y0 + y] = SHADOW
                if any(solid(x + dx, y + dy) for dx in (-1, 0, 1) for dy in (-1, 0, 1)):
                    out[x0 + x, y0 + y] = RED_DARK
    for y in range(mask.height):
        for x in range(mask.width):
            if solid(x, y):
                f = y / max(1, mask.height - 1)
                c = RED_LIGHT if f < 0.38 else RED
                if not solid(x, y - 1):
                    c = RED_LIGHT
                out[x0 + x, y0 + y] = c
    # an underline swash, like a title written in a notebook
    d = ImageDraw.Draw(im)
    y = y0 + mask.height + 1
    d.line((x0 + 10, y, x0 + mask.width - 10, y), fill=RED_DARK, width=1)
    d.line((x0 + 14, y + 2, x0 + mask.width - 22, y + 2), fill=MARGIN, width=1)


def slowpoke_in_coat():
    """Slowpoke's front sprite (the game's own, 64x64) with a lab coat drawn
    over its back, cropped and doubled."""
    src = Image.open(os.path.join(REPO, "graphics/pokemon/slowpoke/anim_front.png")).convert("RGBA").crop((0, 0, 64, 64))
    bg = src.getpixel((0, 0))
    px = src.load()
    for y in range(64):
        for x in range(64):
            if px[x, y][:3] == bg[:3]:
                px[x, y] = (0, 0, 0, 0)
    coat = Image.new("RGBA", (64, 64), (0, 0, 0, 0))
    d = ImageDraw.Draw(coat)
    # the coat, draped from the neck to the start of the tail
    body = [(27, 26), (30, 22), (38, 21), (44, 24), (48, 27), (50, 31),
            (50, 44), (46, 46), (40, 45), (34, 46), (28, 45), (25, 41), (25, 31)]
    d.polygon(body, fill=COAT, outline=COAT_LINE)
    d.polygon([(26, 39), (49, 39), (49, 43), (46, 45), (40, 44), (34, 45), (28, 44), (26, 41)], fill=COAT_SHADE)
    d.line([(49, 31), (49, 43)], fill=COAT_SHADE)
    # collar and lapels by the neck
    d.line([(28, 25), (31, 31)], fill=COAT_LINE)
    d.line([(34, 22), (31, 31)], fill=COAT_LINE)
    d.line([(31, 31), (31, 38)], fill=COAT_SHADE)
    # a pocket with a pen
    d.rectangle((38, 33, 43, 37), outline=COAT_LINE)
    d.line([(40, 30), (40, 34)], fill=PEN)
    # buttons
    for y in (33, 36):
        d.point((33, y), fill=COAT_LINE)
    src.alpha_composite(coat)
    src = src.crop((8, 18, 64, 50))
    return src.resize((src.width * 2, src.height * 2), Image.NEAREST)


def build():
    im = paper()
    logo(im)
    mon = slowpoke_in_coat()
    x, y = (W - mon.width) // 2 + 8, 58
    d = ImageDraw.Draw(im)
    d.ellipse((x + 12, y + mon.height - 10, x + mon.width - 4, y + mon.height + 2), fill=SHADOW)
    im.paste(mon, (x, y), mon)
    q = im.quantize(colors=COLOURS, method=Image.Quantize.MEDIANCUT, dither=Image.Dither.NONE)
    pal = q.getpalette()[:COLOURS * 3]
    pal += [0, 0, 0] * (256 - COLOURS)
    q.putpalette(pal)
    return im, q


def write_pal(path, pal):
    pal = list(pal) + [0] * (768 - len(pal))
    lines = ["JASC-PAL", "0100", "256"] + ["%d %d %d" % tuple(pal[i:i + 3]) for i in range(0, 768, 3)]
    open(path, "w", newline="\r\n").write("\n".join(lines) + "\n")


def main():
    os.makedirs(OUT, exist_ok=True)
    rgb, q = build()
    q.save(os.path.join(OUT, "title.png"))
    write_pal(os.path.join(OUT, "title.pal"), q.getpalette()[:768])
    for gen in ("title.8bpp", "title.gbapal"):
        p = os.path.join(OUT, gen)
        if os.path.exists(p):
            os.remove(p)   # generated: make rebuilds them
    if "--preview" in sys.argv:
        prev = rgb.copy()
        d = ImageDraw.Draw(prev)
        d.text((92, 128), "Press Start", fill=INK)
        d.text((150, 146), "v0.0.19 test build", fill=INK)
        prev.resize((W * 3, H * 3), Image.NEAREST).save(sys.argv[sys.argv.index("--preview") + 1])
    print("title picture written: graphics/research_red/title/title.png (+ .pal)")


if __name__ == "__main__":
    main()
