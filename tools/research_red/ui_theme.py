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
box, 13-14 its trim, 10 the text and 12 its shadow (src/battle_message.c, #92;
the original white 1 / grey 6 are also used by the action menu).
"""
import os
import struct
import sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

# Each theme: paper (window background), ink (text), shadow, six frame colours
# (slots 10-15: light to dark), battle box, battle trim (dark, light).
THEMES = {
    "notebook": {
        "about": "Field notebook: light tan paper, dark brown ink, brown frames; one paper colour on every box (owner, 2026-10-30)",
        "paper": "#ecdcb8", "ink": "#32261c", "shadow": "#cdb88e",
        "frame": ["#ecdcb8", "#dcc89c", "#bfa272", "#e4d3ac", "#967448", "#684c30"],
        # battle message / "What will X do?" box: the same paper and ink as the dialogue box
        "battle": "#ecdcb8", "trim": ["#967448", "#bfa272"],
        # slot 1 was white: it is the paper of the FIGHT menu and the move list
        "battletext": {1: "#ecdcb8", 6: "#685870", 10: "#32261c", 12: "#cdb88e"},
        # HP boxes (healthbox.pal): paper, shading and brass instead of teal;
        # the HP bar and status colours (9-15) stay as they are
        "healthbox": {2: "#ecdcb8", 3: "#d6bd8c", 4: "#b89a68", 5: "#8c7656", 6: "#5e4a36", 7: "#32261c", 8: "#4e3c2a"},
        # FIGHT/BAG menu and the move list (textbox2.pal): brown and tan
        # instead of blue-purple, tan paper, dark ink
        "battlemenu": {1: "#50392a", 2: "#684c30", 3: "#967448", 4: "#32261c", 5: "#ecdcb8",
                       6: "#5e4a36", 7: "#dcc89c", 8: "#584636", 9: "#cdb88e", 10: "#bfa272", 11: "#5e4030"},
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

# Own-colour screens, for the notebook theme only (owner, 2026-10-30): whites and pale
# fills become the paper ramp; blues stay as the contrast colour, as do type badges,
# HP bars, Pokemon, the map, PC wallpapers and the Trainer Card (its colour is its rank).
# {palette: {(line, slot): colour}}
P, S1, S2, S3, DEEP, MID, DARK, INK = ("#ecdcb8", "#dcc89c", "#cdb88e", "#bfa272",
                                       "#967448", "#7c5c38", "#684c30", "#32261c")


def _dex():
    c = {(0, 1): P, (0, 2): S1, (0, 3): S2, (0, 4): S3, (0, 5): DEEP, (0, 6): S1, (0, 7): DEEP,
         (0, 8): MID, (0, 9): DARK, (0, 10): S1, (0, 11): MID}
    for line in (1, 2):  # text: paper, shadows, ink
        c.update({(line, 1): P, (line, 3): S2, (line, 4): S3, (line, 5): "#5e4a36",
                  (line, 6): INK, (line, 7): INK, (line, 8): INK, (line, 9): P})
    return c


def _summary():
    c = {(0, 14): P, (6, 14): P, (0, 2): S1, (6, 2): S1, (0, 6): S3, (6, 6): S3}
    for line in (0, 6):  # lavender and aqua header strips
        c.update({(line, 3): S1, (line, 8): S2, (line, 9): S3, (line, 10): S2, (line, 11): S3})
    for line in (1, 5):
        c.update({(line, 3): S2, (line, 5): S3, (line, 6): S1, (line, 7): S2})
    c.update({(3, 8): S1, (3, 11): S3, (3, 12): DEEP})
    for line in (1, 5):  # the frames and text paper
        c.update({(line, 1): P, (line, 2): S1, (line, 10): P, (line, 11): S3, (line, 12): S2,
                  (line, 13): S2, (line, 14): S1, (line, 15): P})
    for line in (2, 3, 4):  # the page tints: one paper instead of peach, yellow, green
        c.update({(line, 1): S3, (line, 2): S2, (line, 3): S1, (line, 4): DEEP, (line, 5): P,
                  (line, 6): P, (line, 7): S1, (line, 14): P})
    return c


BAG = {(0, 8): P, (0, 9): P, (0, 10): DEEP, (0, 12): P, (0, 13): INK}
TM = {(0, 7): S1, (0, 8): P, (0, 9): P, (0, 11): P, (0, 13): INK, (0, 14): S2, (0, 15): S3}
WHITE_WINDOW = {(0, 0): P, (0, 1): P, (0, 2): INK, (0, 3): S2}  # grey text washes out on tan: dark ink
SCREENS = {
    "graphics/pokedex/hgss/palette_default.gbapal": _dex(),
    "graphics/pokedex/hgss/palette_national.gbapal": _dex(),
    "graphics/pokedex/hgss/palette_search_results.gbapal": _dex(),
    "graphics/party_menu/bg.gbapal": {(line, s): col for line in (1, 2) for s, col in
                                      {2: DEEP, 3: DARK, 4: "#a58455", 5: MID, 10: P, 11: S2, 12: DEEP}.items()},
    "graphics/summary_screen/bg.gbapal": _summary(),
    "graphics/summary_screen/text_header.gbapal": {(0, 0): P, (0, 1): P, (0, 2): INK, (0, 3): S2, (0, 10): S3, (0, 11): S1},
    "graphics/misc/option_menu.gbapal": WHITE_WINDOW,
    "graphics/shop_menu/shop_menu.gbapal": {(0, 0): P, (0, 1): DEEP, (0, 2): S3, (0, 3): S1, (0, 4): P, (0, 5): P, (0, 6): INK},
    "graphics/item_menu/bg.gbapal": BAG,
    "graphics/item_menu/bg_female.gbapal": BAG,
    "graphics/item_menu/bag_window_pal.gbapal": WHITE_WINDOW,
    "graphics/tm_case/menu_male.gbapal": TM,
    "graphics/tm_case/menu_female.gbapal": TM,
    "graphics/naming_screen/keyboard.gbapal": WHITE_WINDOW,
    "graphics/naming_screen/menu.gbapal": {(0, 1): P, (0, 4): S3, (0, 5): S2, (0, 6): S1},
    "graphics/interface/learn_move.gbapal": {(0, 1): S1, (0, 2): P, (0, 3): S1, (0, 4): DEEP, (0, 5): P, (0, 14): P},
    "graphics/dexnav/gui.gbapal": {(0, 1): P},
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


SOURCES_OF_INCLUDES = ["src/graphics.c", "src/text_window.c", "src/menu.c", "src/battle_interface.c", "src/option_menu.c", "src/pokedex_plus_hgss.c", "src/tm_case.c", "src/dexnav.c", "src/battle_gfx_sfx_util.c"]


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
    return targets + list(SCREENS) + ["graphics/battle_interface/textbox1.gbapal", "graphics/battle_interface/textbox.gbapal"] + list(EXTRA_PALETTES.values())


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
        for slot, col in t.get("battletext", {}).items():
            c[slot] = to555(col)
        write(path, c)
    for key, rel in EXTRA_PALETTES.items():
        if key in t:
            path = os.path.join(REPO, rel)
            c = read(path)
            for slot, col in t[key].items():
                c[slot] = to555(col)
            write(path, c)
    if theme == "notebook":
        for rel, slots in SCREENS.items():
            path = os.path.join(REPO, rel)
            c = read(path)
            for (line, slot), col in slots.items():
                c[line * 16 + slot] = to555(col)
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
