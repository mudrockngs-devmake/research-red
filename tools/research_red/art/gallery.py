#!/usr/bin/env python3
"""Research Red Art Kit: gallery.html, one page with every preview.

  python3 tools/research_red/art/gallery.py

Writes gallery.html into the toolkit folder (E:\\Workspace\\romhacks\\
research-red\\art\\toolkit\\), linking the previews the other tools made:
overworld walks and runs, the overview beside the ref art, battle picture
comparisons, map-tool test shots, the sprite library. Open it in a browser.
"""
import glob
import html
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import artlib  # noqa: E402

try:
    import tomllib
    RECIPES = tomllib.load(open(os.path.join(artlib.OW_DIR, "recipes.toml"), "rb"))
except Exception:
    RECIPES = {}

CSS = """
:root { --bg:#f6f4ef; --card:#fffdf8; --ink:#2b2a28; --muted:#6c675f; --line:#e2ddd2; --accent:#b4432f; }
@media (prefers-color-scheme: dark) { :root { --bg:#1d1c1a; --card:#262522; --ink:#ece8e0; --muted:#a9a399; --line:#3a3833; --accent:#e07a5f; } }
* { box-sizing:border-box; }
body { margin:0; background:var(--bg); color:var(--ink); font:15px/1.5 system-ui, sans-serif; }
main { max-width:1100px; margin:0 auto; padding:24px 16px 64px; }
h1 { font-size:26px; margin:0 0 4px; } h2 { font-size:19px; margin:36px 0 10px; border-bottom:1px solid var(--line); padding-bottom:6px; }
p.lead, .muted { color:var(--muted); }
nav a { margin-right:14px; color:var(--accent); text-decoration:none; }
.grid { display:grid; grid-template-columns:repeat(auto-fill, minmax(250px, 1fr)); gap:12px; }
.card { background:var(--card); border:1px solid var(--line); border-radius:10px; padding:10px; }
.card h3 { font-size:15px; margin:0 0 4px; } .card p { font-size:13px; margin:4px 0 0; color:var(--muted); }
img { image-rendering:pixelated; max-width:100%; height:auto; display:block; }
.wide img { width:100%; }
.decide { background:var(--card); border-left:4px solid var(--accent); padding:10px 14px; border-radius:6px; }
a { color:var(--accent); }
code { background:var(--line); padding:1px 5px; border-radius:4px; font-size:13px; }
"""


def rel(path):
    return os.path.relpath(path, artlib.TOOLKIT_OUT).replace(os.sep, "/")


def img(path, alt=""):
    return '<img src="%s" alt="%s" loading="lazy">' % (html.escape(rel(path)), html.escape(alt))


def main():
    out = artlib.TOOLKIT_OUT
    ow = os.path.join(out, "overworld")
    parts = ["<!doctype html><html lang='en'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'>"
             "<title>Art Kit Gallery</title><style>%s</style></head><body><main>" % CSS]
    parts.append("<h1>Research Red: Art Kit gallery</h1><p class='lead'>Everything the art tools made, in one place. "
                 "Rebuild with <code>python3 tools/research_red/art/gallery.py</code>.</p>")
    parts.append("<nav><a href='#overworld'>Overworld</a><a href='#battle'>Battle pictures</a><a href='#maps'>Map tools</a>"
                 "<a href='#library'>Sprite library</a><a href='#checks'>Checks</a></nav>")

    parts.append("<h2 id='overworld'>Overworld sprites</h2>")
    if os.path.exists(os.path.join(ow, "overview.png")):
        parts.append("<div class='card wide'><h3>Every sprite beside its reference art</h3>%s</div>" % img(os.path.join(ow, "overview.png"), "overview"))
    parts.append("<p class='muted'>Walking (and running for the players), as the game plays it: down, up, left, right.</p><div class='grid'>")
    for gif in sorted(glob.glob(os.path.join(ow, "*_walk.gif"))):
        name = os.path.basename(gif)[:-9]
        note = RECIPES.get(name, {}).get("notes", "")
        run = os.path.join(ow, name + "_run.gif")
        frames = os.path.join(ow, name + "_frames.png")
        extra = (" " + img(run, name + " running")) if os.path.exists(run) else ""
        link = ("<p><a href='%s'>before / after frames</a></p>" % rel(frames)) if os.path.exists(frames) else ""
        parts.append("<div class='card'><h3>%s</h3>%s%s<p>%s</p>%s</div>" % (html.escape(name), img(gif, name), extra, html.escape(note), link))
    parts.append("</div>")

    parts.append("<h2 id='battle'>Battle pictures</h2><div class='grid'>")
    for f, title, note in (("leaders_before_after.png", "Gym leaders and Elite Four", "FireRed's picture, then the new one from the reference art (in the game now)."),
                           ("cast_full_vs_frlg.png", "Story cast: full body (now) or FireRed framing (option)", "The cast keeps the full-body look for now; say if you'd like the bigger framing."),
                           ("back_made_throw.png", "Back-pic throw draft (battle_kit backpic)", "Follows Red's lean; the throwing arm still needs drawing."),
                           ("pikachu_shiny.png", "Shiny palette (battle_kit shiny)", "Example: colours turned round the colour wheel.")):
        p = os.path.join(out, "battle", f)
        if os.path.exists(p):
            parts.append("<div class='card'><h3>%s</h3>%s<p>%s</p></div>" % (title, img(p, title), note))
    parts.append("</div>")

    parts.append("<h2 id='maps'>Map tools</h2>")
    test = os.path.join(out, "tiles", "test")
    if os.path.isdir(test):
        parts.append("<p class='muted'>Test in a scratch copy (not in the game): a potted plant and a seedling bench drawn as PNGs, "
                     "added with <code>tile_kit.py</code>; a door and a greenhouse made with <code>building.py</code>; the automated test walked in.</p><div class='grid'>")
        for f in sorted(glob.glob(os.path.join(test, "*.png"))):
            parts.append("<div class='card'><h3>%s</h3>%s</div>" % (html.escape(os.path.basename(f)), img(f)))
        parts.append("</div>")
    tiles = [f for f in sorted(glob.glob(os.path.join(out, "tiles", "*.png")))]
    if tiles:
        parts.append("<div class='grid'>")
        for f in tiles:
            parts.append("<div class='card'><h3>%s</h3>%s</div>" % (html.escape(os.path.basename(f)), img(f)))
        parts.append("</div>")

    parts.append("<h2 id='library'>Sprite library</h2><div class='grid'>")
    for f in sorted(glob.glob(os.path.join(out, "library", "*.png"))):
        parts.append("<div class='card'><h3>%s</h3><a href='%s'>%s</a></div>" % (html.escape(os.path.basename(f)), rel(f), img(f)))
    cat = os.path.join(out, "library", "catalogue.md")
    if os.path.exists(cat):
        parts.append("<div class='card'><h3>catalogue.md</h3><p>Which OBJ_EVENT_GFX_ uses which picture, and how many frames it has.</p><p><a href='%s'>open</a></p></div>" % rel(cat))
    parts.append("</div>")

    parts.append("<h2 id='checks'>Checks</h2>")
    st = os.path.join(out, "checks", "walkgen_selftest.png")
    if os.path.exists(st):
        parts.append("<div class='card wide'><h3>Walk maker self-test</h3><p>Left: a sheet's real walk. Right: the walk made from its standing frames only.</p>%s</div>" % img(st))
    parts.append("</main></body></html>")
    path = os.path.join(out, "gallery.html")
    with open(path, "w") as f:
        f.write("\n".join(parts))
    print("gallery ->", path)


if __name__ == "__main__":
    main()
