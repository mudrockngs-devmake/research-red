#!/usr/bin/env python3
"""Research Red: interface colour themes (#73).

Rewrites the interface palettes (window frames, the dialogue box, menu text,
the battle text box) for one theme, so a build can be screenshotted and
compared. Layouts and graphics stay the same: only colours change.

  python3 tools/research_red/ui_theme.py --list
  python3 tools/research_red/ui_theme.py notebook     # apply a theme, then make firered
  python3 tools/research_red/ui_theme.py --restore    # back to the original, then make firered

The .gbapal files are generated (not in git) from a tracked .png or .pal, so the
tool edits those sources, deletes the generated .gbapal and touches the C files
that include them; `make` then rebuilds everything that changed.

Palette slots (FireRed's layout): 1 window background, 2 text, 3 text shadow,
4-9 coloured text (left alone), 10-15 the window frame. Battle text box: 15 the
box, 13-14 its trim, 1/5 the text.
"""
import os
import struct
import sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

# Each theme: paper (window background), ink (text), shadow, six frame colours
# (slots 10-15: light to dark), battle box, battle trim (dark, light).
THEMES = {
    "notebook": {
        "about": "Field notebook: cream paper, brown ink, tan frames, a leather battle box with brass trim",
        "paper": "#f8f0d8", "ink": "#584030", "shadow": "#d8c8a8",
        "frame": ["#f8f0d8", "#e8d8b0", "#d0b888", "#f0e4c4", "#b09060", "#806040"],
        "battle": "#5a4632", "trim": ["#a07838", "#d8b868"],
        # HP boxes (healthbox.pal): paper, shading and brass instead of teal;
        # the HP bar and status colours (9-15) stay as they are
        "healthbox": {2: "#f8f0d8", 3: "#e0cfa8", 4: "#c8a868", 5: "#a08a6a", 6: "#6e5a44", 7: "#3b2f25", 8: "#5a4632"},
        # FIGHT/BAG menu and the move list (textbox2.pal): brown and tan
        # instead of blue-purple, cream paper, brown ink
        "battlemenu": {1: "#5a4632", 2: "#806040", 3: "#b09060", 4: "#3b2f25", 5: "#f8f0d8",
                       6: "#6e5a44", 7: "#e8d8b0", 8: "#6a5848", 9: "#d8c8a8", 10: "#c8a878", 11: "#6e5040"},
    },
    "meadow": {
        "about": "Pallet meadow: soft white-green, forest ink, green frames, a deep green battle box",
        "paper": "#f4f8ec", "ink": "#3e5040", "shadow": "#c8d8c0",
        "frame": ["#f4f8ec", "#e0ecd0", "#b8d4a8", "#ecf4e0", "#70a068", "#4c7850"],
        "battle": "#2f5a46", "trim": ["#a08838", "#d0c068"],
    },
    "sunset": {
        "about": "Sunset lab: warm peach, plum-brown ink, coral frames, a plum battle box",
        "paper": "#fcf0e6", "ink": "#5a3c3c", "shadow": "#e0c8c0",
        "frame": ["#fcf0e6", "#f4dcd0", "#e8b8a8", "#f8e8e0", "#b87068", "#8c5050"],
        "battle": "#5a3a52", "trim": ["#a87840", "#e0b870"],
    },
}

# Window palettes: (path, frame slots used?)
WINDOW_PALETTES = [
    ("graphics/text_window/menu_message.gbapal", True),   # the dialogue box
    ("graphics/text_window/type1.gbapal", True),          # menu frame (Options: Frame type 1)
    ("graphics/text_window/stdpal_1.gbapal", True),
    ("graphics/text_window/stdpal_2.gbapal", True),
    ("graphics/text_window/stdpal_3.gbapal", True),
    ("graphics/text_window/stdpal_4.gbapal", True),
    ("graphics/interface/std_menu.gbapal", False),        # menu text
]
BATTLE_PALETTES = ["graphics/battle_interface/textbox1.gbapal"]
# Optional per-theme extras: {slot: colour} for these palettes
EXTRA_PALETTES = {
    "healthbox": "graphics/battle_interface/healthbox.gbapal",
    "battlemenu": "graphics/battle_interface/textbox2.gbapal",
}


def to555(hexcol):
    r, g, b = (int(hexcol[i:i + 2], 16) for i in (1, 3, 5))
    return (r >> 3) | ((g >> 3) << 5) | ((b >> 3) << 10)


def read(path):
    data = open(path, "rb").read()
    return list(struct.unpack("<%dH" % (len(data) // 2), data))


SOURCES_OF_INCLUDES = ["src/graphics.c", "src/text_window.c", "src/menu.c", "src/battle_interface.c", "src/battle_gfx_sfx_util.c"]


def rgb(v):
    return ((v & 31) << 3, ((v >> 5) & 31) << 3, ((v >> 10) & 31) << 3)


def write(path, colours):
    """Write the theme into the tracked source (.pal or .png) of a generated .gbapal."""
    base = path[:-len(".gbapal")]
    if os.path.exists(base + ".pal"):
        lines = ["JASC-PAL", "0100", str(len(colours))] + ["%d %d %d" % rgb(v) for v in colours]
        open(base + ".pal", "w", newline="\r\n").write("\n".join(lines) + "\n")
    elif os.path.exists(base + ".png"):
        from PIL import Image
        img = Image.open(base + ".png")
        flat = [x for v in colours for x in rgb(v)]
        img.putpalette(flat + img.getpalette()[len(flat):])
        img.save(base + ".png")
    else:
        raise SystemExit(f"no .pal or .png source for {path}")
    if os.path.exists(path):
        os.remove(path)  # generated: make rebuilds it from the source


def touch_includers():
    for rel in SOURCES_OF_INCLUDES:
        os.utime(os.path.join(REPO, rel))


def generated_targets():
    targets = [rel for rel, _ in WINDOW_PALETTES]
    # textbox.gbapal is textbox1 + textbox2 (Makefile): theme textbox1, rebuild both
    return targets + ["graphics/battle_interface/textbox1.gbapal", "graphics/battle_interface/textbox.gbapal"] + list(EXTRA_PALETTES.values())


def restore():
    import subprocess
    sources = []
    for rel in generated_targets():
        base = os.path.join(REPO, rel[:-len(".gbapal")])
        for ext in (".pal", ".png"):
            if os.path.exists(base + ext):
                sources.append(os.path.relpath(base + ext, REPO))
                break
        if os.path.exists(os.path.join(REPO, rel)):
            os.remove(os.path.join(REPO, rel))
    subprocess.run(["git", "-C", REPO, "checkout", "--"] + sorted(set(sources)), check=True)
    touch_includers()
    print("restored the original interface colours: run make firered")


def apply(theme):
    import subprocess
    t = THEMES[theme]
    # start from the original colours: generate any missing .gbapal first
    subprocess.run(["make", "-C", REPO] + generated_targets(), check=True, capture_output=True)
    for rel, frame in WINDOW_PALETTES:
        path = os.path.join(REPO, rel)
        c = read(path)
        if rel.endswith("std_menu.gbapal"):
            c[0] = to555(t["paper"])
        c[1], c[2], c[3] = to555(t["paper"]), to555(t["ink"]), to555(t["shadow"])
        if frame:
            for i, col in enumerate(t["frame"]):
                c[10 + i] = to555(col)
        write(path, c)
    for rel in BATTLE_PALETTES:
        path = os.path.join(REPO, rel)
        c = read(path)
        c[15] = to555(t["battle"])
        c[13], c[14] = to555(t["trim"][0]), to555(t["trim"][1])
        write(path, c)
    for key, rel in EXTRA_PALETTES.items():
        if key in t:
            path = os.path.join(REPO, rel)
            c = read(path)
            for slot, col in t[key].items():
                c[slot] = to555(col)
            write(path, c)
    combined = os.path.join(REPO, "graphics/battle_interface/textbox.gbapal")
    if os.path.exists(combined):
        os.remove(combined)
    touch_includers()
    print(f"applied '{theme}': {t['about']} (run make firered; --restore undoes it)")


def main():
    if len(sys.argv) >= 2 and sys.argv[1] == "--restore":
        restore()
        return
    if len(sys.argv) < 2 or sys.argv[1] == "--list":
        for name, t in THEMES.items():
            print(f"{name:10} {t['about']}")
        return
    apply(sys.argv[1])


if __name__ == "__main__":
    main()
