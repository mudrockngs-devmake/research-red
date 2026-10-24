#!/usr/bin/env python3
"""Research Red Art Kit: hand-finish sprites in Aseprite.

  python3 tools/research_red/art/aseprite.py open ow:elm          # an overworld sheet (grid 16x32)
  python3 tools/research_red/art/aseprite.py open battle:jenny    # a battle picture (battle_fr, grid 64x64)
  python3 tools/research_red/art/aseprite.py open back:ivy        # a back picture draft
  python3 tools/research_red/art/aseprite.py open portrait:oak    # a dialogue portrait
  python3 tools/research_red/art/aseprite.py open some/file.png   # anything else
  python3 tools/research_red/art/aseprite.py import ow:elm        # bring the edit back into the game
  python3 tools/research_red/art/aseprite.py list                 # what's out for editing

`open` copies the picture to E:\\Workspace\\romhacks\\research-red\\art\\edit\\
and opens it in Aseprite (Windows) zoomed, with the frame grid showing. Draw,
then File > Save (keep it a PNG, or save an .aseprite with layers: both work).

`import` checks the edit (size, 16 colours, colour 0 = see-through), puts it
where hand-made art wins, and rebuilds what depends on it:
  ow:<recipe>      -> graphics/research_red/overworld/bases/hand_<recipe>.png, the
                      recipe gets  hand = "bases/hand_<recipe>.png"  (then ow_maker install)
  battle:<name>    -> graphics/research_red/cast_handmade/<name>/battle_fr.png (then build_cast_art)
  back:<name>      -> cast_handmade/<name>/back.png (then back_maker + build_back_pics)
  portrait:<name>  -> cast_handmade/<name>/portrait.png (then build_cast_art)
Then build the game as usual (make firered), and look at it in a smoke test.
"""
import os
import re
import shutil
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import artlib  # noqa: E402

ASEPRITE = os.environ.get("RR_ASEPRITE", "/mnt/e/SteamLibrary/steamapps/common/Aseprite/Aseprite.exe")
EDIT = "/mnt/e/Workspace/romhacks/research-red/art/edit"
CAST = os.path.join(artlib.REPO, "graphics/research_red/cast")
HANDMADE = os.path.join(artlib.REPO, "graphics/research_red/cast_handmade")


def win(path):
    return subprocess.run(["wslpath", "-w", path], capture_output=True, text=True).stdout.strip()


def target(spec):
    """'ow:elm' -> (kind, name, source png, grid w, grid h, edit file name)."""
    if ":" in spec and not spec.startswith("/"):
        kind, name = spec.split(":", 1)
    else:
        kind, name = "file", spec
    if kind == "ow":
        import ow_maker
        r = ow_maker.load_recipes()[name]
        src = os.path.join(artlib.OW_DIR, r["hand"]) if r.get("hand") else ow_maker.output_png(name, r)
        return kind, name, src, 16, 32, "ow_%s.png" % name
    if kind in ("battle", "back", "portrait"):
        file = {"battle": "battle_fr.png", "back": "back.png", "portrait": "portrait.png"}[kind]
        hand = os.path.join(HANDMADE, name, file)
        src = hand if os.path.exists(hand) else os.path.join(CAST, name, file)
        return kind, name, src, 64, 64, "%s_%s.png" % (kind, name)
    src = os.path.abspath(name)
    return "file", os.path.basename(src)[:-4], src, 16, 16, os.path.basename(src)


def lua_open(path, gw, gh):
    p = win(path).replace("\\", "\\\\")
    return """local spr = app.open("%s")
if spr then
  spr.gridBounds = Rectangle(0, 0, %d, %d)
  pcall(function() app.preferences.document(spr).show.grid = true end)
  pcall(function() app.command.FitScreen() end)
end
""" % (p, gw, gh)


def cmd_open(spec):
    kind, name, src, gw, gh, editname = target(spec)
    if not os.path.exists(src):
        raise SystemExit("nothing to open at %s" % src)
    os.makedirs(EDIT, exist_ok=True)
    dst = os.path.join(EDIT, editname)
    if not os.path.exists(dst) and not os.path.exists(dst[:-4] + ".aseprite"):
        shutil.copy(src, dst)
    elif not os.path.exists(dst):
        dst = dst[:-4] + ".aseprite"
    script = os.path.join(EDIT, "_open.lua")
    open(script, "w").write(lua_open(dst, gw, gh))
    # Aseprite runs on Windows; started on its own so the terminal stays free
    subprocess.Popen([ASEPRITE, "--script", win(script)], cwd="/mnt/c",
                     stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, start_new_session=True)
    print("opened in Aseprite: %s (grid %dx%d)" % (win(dst), gw, gh))
    print("when you're done: save, then  python3 tools/research_red/art/aseprite.py import %s" % spec)


def to_png(edit_base):
    """The edit as a PNG: an .aseprite file is flattened by Aseprite itself."""
    png, ase = edit_base + ".png", edit_base + ".aseprite"
    if os.path.exists(ase) and (not os.path.exists(png) or os.path.getmtime(ase) > os.path.getmtime(png)):
        subprocess.run([ASEPRITE, "-b", win(ase), "--save-as", win(png)], cwd="/mnt/c", check=True,
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    if not os.path.exists(png):
        raise SystemExit("no edit found (%s)" % png)
    return png


def clean(png, w=None, h=None):
    """Check the edit and store it as a tidy 16-colour picture (colour 0 = see-through)."""
    g = artlib.load_grid(png)
    if w and (len(g[0]) % w or len(g) != h):
        raise SystemExit("%s is %dx%d; expected %d-pixel-wide frames, %d tall" % (png, len(g[0]), len(g), w, h))
    g = [[None if p is None else artlib.gba_colour(p) for p in row] for row in g]
    n = len(artlib.colours(g))
    if n > 15:
        print("  note: %d colours; merged the closest down to 15 (check it still looks right)" % n)
        g = artlib.reduce_colours(g, 15)
    return g


def run(*cmd):
    subprocess.run([sys.executable] + list(cmd), cwd=artlib.REPO, check=True)


def cmd_import(spec):
    kind, name, src, gw, gh, editname = target(spec)
    png = to_png(os.path.join(EDIT, editname[:-4]))
    if kind == "ow":
        import ow_maker
        g = clean(png, 16, 32)
        rel = "bases/hand_%s.png" % name
        artlib.save_indexed(g, os.path.join(artlib.OW_DIR, rel))
        path = ow_maker.RECIPES
        text = open(path).read()
        m = re.search(r"\n\[%s\]\n" % re.escape(name), text)
        if not m:
            raise SystemExit("no recipe [%s]" % name)
        block_end = text.find("\n[", m.end())
        block = text[m.end():block_end if block_end > 0 else len(text)]
        if re.search(r"^hand\s*=", block, re.M):
            block = re.sub(r"^hand\s*=.*$", 'hand = "%s"' % rel, block, flags=re.M)
        else:
            block = 'hand = "%s"                                  # hand-finished in Aseprite\n' % rel + block
        text = text[:m.end()] + block + (text[block_end:] if block_end > 0 else "")
        open(path, "w").write(text)
        run(os.path.join(HERE, "ow_maker.py"), "install")
    elif kind in ("battle", "back", "portrait"):
        g = clean(png, 64, 64)
        file = {"battle": "battle_fr.png", "back": "back.png", "portrait": "portrait.png"}[kind]
        out = os.path.join(HANDMADE, name, file)
        os.makedirs(os.path.dirname(out), exist_ok=True)
        artlib.save_indexed(g, out)
        if kind == "back":
            run(os.path.join(HERE, "back_maker.py"), name)
            run(os.path.join(artlib.REPO, "tools/research_red/build_back_pics.py"))
        else:
            run(os.path.join(artlib.REPO, "tools/research_red/build_cast_art.py"))
    else:
        raise SystemExit("plain files aren't imported automatically: copy %s where it belongs" % png)
    print("imported %s; next: make firered -j6, then look at it in a smoke test" % spec)


def cmd_list():
    if not os.path.isdir(EDIT):
        print("nothing out for editing")
        return
    for f in sorted(os.listdir(EDIT)):
        if f.endswith((".png", ".aseprite")):
            kind, _, rest = f.partition("_")
            print("  %-30s -> import %s:%s" % (f, kind, rest.rsplit(".", 1)[0]))


def main(argv):
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__)
        return 0
    if argv[0] == "open":
        cmd_open(argv[1])
    elif argv[0] == "import":
        cmd_import(argv[1])
    elif argv[0] == "list":
        cmd_list()
    else:
        print(__doc__)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
