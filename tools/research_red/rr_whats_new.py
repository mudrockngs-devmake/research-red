#!/usr/bin/env python3
"""Research Red: the what's-new report for a release (#75).

Builds a folder for the owner and testers:
  <workspace>/whats-new/<version>/report.html     opens in any browser
  <workspace>/whats-new/<version>/screenshots/    the same pictures, named by change
  <workspace>/whats-new/<version>/report.pdf      a PDF copy for phones (via Edge on Windows)

The page lists every change with its screenshots, how to try it, a checkbox and
a notes box. At the end, "Your playtest" asks what a tester's report usually
covers (setup, how far, ratings, bugs one by one, text, art, balance, story,
pacing, ideas). Ticks and notes are kept in that browser; "Save my notes"
downloads everything as a text file to drop in the _emu/notes folder
(rr_archive_test.sh files it with the round).

The list of changes lives in tools/research_red/whats_new/<version>.json:
  {"version": "v0.0.16", "intro": "...", "save": "Save: old saves work",
   "sections": [{"title": "...", "items": [
       {"id": "ranch-pager", "title": "...", "what": "...", "try": "...",
        "shots": ["pager_fly/f002.png", ...]}]}]}
Screenshots come from a smoke-test run (test-runs/<run>/<scenario>/<shot>.png);
the newest run is used unless --run is given. Missing shots are listed, not fatal.

Usage:
  python3 tools/research_red/rr_whats_new.py v0.0.16 [--run <test-run folder>]
"""
import html
import json
import os
import shutil
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
WORKSPACE = "/mnt/e/Workspace/romhacks/research-red"


def newest_run():
    runs = os.path.join(WORKSPACE, "test-runs")
    dirs = [os.path.join(runs, d) for d in os.listdir(runs) if os.path.isdir(os.path.join(runs, d))]
    return max(dirs, key=os.path.getmtime)


PAGE = """<!doctype html>
<html lang="en"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Research Red {version}: what's new</title>
<style>
:root {{ --paper:#fbf6ea; --ink:#3b2f25; --muted:#7a6a58; --line:#e3d6bd; --accent:#b0503c; --card:#fffdf7; }}
@media (prefers-color-scheme: dark) {{ :root {{ --paper:#1f1b17; --ink:#efe6d6; --muted:#b5a690; --line:#3a332b; --accent:#e08a6f; --card:#29241f; }} }}
* {{ box-sizing: border-box; }}
body {{ margin:0; background:var(--paper); color:var(--ink); font:16px/1.5 system-ui, sans-serif; }}
main {{ max-width: 980px; margin: 0 auto; padding: 24px 16px 64px; }}
h1 {{ margin: 0 0 4px; font-size: 1.7rem; }}
h2 {{ margin: 36px 0 8px; font-size: 1.25rem; border-bottom: 2px solid var(--line); padding-bottom: 4px; }}
.lead {{ color: var(--muted); margin: 0 0 12px; }}
.save {{ display:inline-block; background:var(--card); border:1px solid var(--line); border-radius:8px; padding:6px 10px; font-weight:600; }}
.bar {{ position: sticky; top: 0; background: var(--paper); padding: 10px 0; border-bottom: 1px solid var(--line); display:flex; gap:12px; align-items:center; flex-wrap:wrap; z-index:2; }}
.bar button + button {{ background: none; color: var(--accent); border: 1px solid var(--accent); }}
.bar button {{ background: var(--accent); color: #fff; border: 0; border-radius: 8px; padding: 8px 14px; font-weight: 600; cursor: pointer; }}
.bar input {{ border:1px solid var(--line); border-radius:8px; padding:7px 10px; background:var(--card); color:var(--ink); }}
.item {{ background: var(--card); border: 1px solid var(--line); border-radius: 12px; padding: 14px 16px; margin: 14px 0; }}
.panel {{ background: var(--card); border: 1px solid var(--line); border-radius: 12px; padding: 14px 16px; margin: 14px 0; }}
.item.done {{ opacity: .72; }}
.head {{ display:flex; gap:10px; align-items:flex-start; }}
.head input {{ width: 22px; height: 22px; margin-top: 3px; accent-color: var(--accent); flex: none; }}
.head h3 {{ margin: 0; font-size: 1.05rem; }}
.what {{ margin: 6px 0 0 32px; }}
.try {{ margin: 6px 0 0 32px; color: var(--muted); }}
.try b {{ color: var(--ink); }}
.shots {{ display:flex; gap:8px; flex-wrap:wrap; margin: 10px 0 0 32px; }}
.shots img {{ width: 240px; max-width: 100%; image-rendering: pixelated; border-radius: 6px; border: 1px solid var(--line); }}
textarea {{ width: calc(100% - 32px); margin: 10px 0 0 32px; min-height: 52px; border:1px solid var(--line); border-radius: 8px; padding: 8px; background: var(--paper); color: var(--ink); font: inherit; }}
.missing {{ margin: 6px 0 0 32px; color: var(--accent); font-size: .9rem; }}
.general label {{ display:block; font-weight:600; margin: 14px 0 0; }}
.general .hint {{ display:block; font-weight:400; color: var(--muted); font-size: .9rem; }}
.general textarea, .general .bug textarea {{ width: 100%; margin: 6px 0 0; }}
.general input[type=text], .general select {{ width: 100%; max-width: 420px; margin-top: 6px; border:1px solid var(--line); border-radius:8px; padding:7px 10px; background:var(--paper); color:var(--ink); font: inherit; }}
.grid {{ display:grid; grid-template-columns: repeat(auto-fit, minmax(240px, 1fr)); gap: 0 16px; }}
.bug {{ border: 1px dashed var(--line); border-radius: 10px; padding: 4px 12px 12px; margin: 12px 0; }}
.bug .remove, #addbug {{ background: none; border: 1px solid var(--accent); color: var(--accent); border-radius: 8px; padding: 5px 10px; cursor: pointer; font: inherit; margin-top: 10px; }}
.bar a {{ color: var(--accent); }}
@media (max-width: 600px) {{ .what, .try, .shots, .missing {{ margin-left: 0; }} textarea {{ width: 100%; margin-left: 0; }} .shots img {{ width: 100%; }} }}
@media print {{ .bar {{ display:none; }} }}
</style></head>
<body><main>
<h1>Research Red {version}: what's new</h1>
<p class="lead">{intro}</p>
<p><span class="save">{save}</span></p>
<div class="bar"><input id="who" placeholder="Your name"><span id="count"></span>
<a href="#playtest">Your playtest</a><button id="export">Save my notes</button><button id="backup" title="Everything any Research Red report saved in this browser, as one file">Backup everything</button></div>
{sections}
{general}
</main>
<script>
const KEY = "rr-whats-new-{version}";
let state = {{}};
try {{ state = JSON.parse(localStorage.getItem(KEY) || "{{}}"); }} catch (e) {{}}
function save() {{ try {{ localStorage.setItem(KEY, JSON.stringify(state)); }} catch (e) {{}} count(); }}
function count() {{
  const boxes = document.querySelectorAll(".item input[type=checkbox]");
  let n = 0; boxes.forEach(b => {{ if (b.checked) n++; }});
  document.getElementById("count").textContent = n + " of " + boxes.length + " checked";
}}
document.getElementById("who").value = state.who || "";
document.getElementById("who").addEventListener("input", e => {{ state.who = e.target.value; save(); }});
document.querySelectorAll(".item").forEach(item => {{
  const id = item.dataset.id, box = item.querySelector("input"), notes = item.querySelector("textarea");
  const s = state[id] || {{}};
  box.checked = !!s.ok; notes.value = s.notes || ""; item.classList.toggle("done", box.checked);
  box.addEventListener("change", () => {{ state[id] = Object.assign(state[id] || {{}}, {{ok: box.checked}}); item.classList.toggle("done", box.checked); save(); }});
  notes.addEventListener("input", () => {{ state[id] = Object.assign(state[id] || {{}}, {{notes: notes.value}}); save(); }});
}});
// "Your playtest": one value per field in state.general, bug cards in state.bugs
state.general = state.general || {{}};
state.bugs = state.bugs || [];
document.querySelectorAll("[data-g]").forEach(el => {{
  el.value = state.general[el.dataset.g] || "";
  el.addEventListener("input", () => {{ state.general[el.dataset.g] = el.value; save(); }});
}});
const BUG_FIELDS = ["where", "what", "again", "how_bad", "shot"];
function drawBugs() {{
  const list = document.getElementById("bugs");
  list.innerHTML = "";
  state.bugs.forEach((bug, i) => {{
    const card = document.getElementById("bug-template").content.firstElementChild.cloneNode(true);
    card.querySelector(".num").textContent = "Bug " + (i + 1);
    BUG_FIELDS.forEach(f => {{
      const el = card.querySelector("[data-b=" + f + "]");
      el.value = bug[f] || "";
      el.addEventListener("input", () => {{ bug[f] = el.value; save(); }});
    }});
    card.querySelector(".remove").addEventListener("click", () => {{
      if (confirm("Remove bug " + (i + 1) + "?")) {{ state.bugs.splice(i, 1); save(); drawBugs(); }}
    }});
    list.appendChild(card);
  }});
}}
document.getElementById("addbug").addEventListener("click", () => {{ state.bugs.push({{}}); save(); drawBugs(); }});
drawBugs();
document.getElementById("export").addEventListener("click", () => {{
  let out = "Research Red {version} test notes" + (state.who ? " from " + state.who : "") + "\\n\\n";
  const indent = t => "    " + t.replace(/\\n/g, "\\n    ");
  out += "== Changes in this build ==\\n";
  document.querySelectorAll(".item").forEach(item => {{
    const s = state[item.dataset.id] || {{}};
    out += (s.ok ? "[x] " : "[ ] ") + item.querySelector("h3").textContent + "\\n";
    if (s.notes) out += indent(s.notes) + "\\n";
  }});
  out += "\\n== Your playtest ==\\n";
  document.querySelectorAll("[data-g]").forEach(el => {{
    const v = (state.general[el.dataset.g] || "").trim();
    if (v) out += el.dataset.label + ":\\n" + indent(v) + "\\n";
  }});
  out += "\\n== Bugs (" + state.bugs.length + ") ==\\n";
  state.bugs.forEach((b, i) => {{
    out += "Bug " + (i + 1) + ": " + (b.where || "(where?)") + "\\n";
    if (b.what) out += indent(b.what) + "\\n";
    out += "    How often: " + (b.again || "-") + " | How bad: " + (b.how_bad || "-")
         + (b.shot ? " | Screenshot: " + b.shot : "") + "\\n";
  }});
  // anything saved under a name this page doesn't show (e.g. a change renamed since)
  const shown = new Set([...document.querySelectorAll(".item")].map(i => i.dataset.id).concat(["who", "general", "bugs"]));
  const extra = Object.keys(state).filter(k => !shown.has(k) && (state[k].notes || state[k].ok));
  if (extra.length) {{
    out += "\\n== Other saved notes ==\\n";
    extra.forEach(k => {{ out += k + (state[k].ok ? " [x]" : "") + "\\n" + (state[k].notes ? indent(state[k].notes) + "\\n" : ""); }});
  }}
  const a = document.createElement("a");
  a.href = URL.createObjectURL(new Blob([out], {{type: "text/plain"}}));
  a.download = "research-red-{version}-notes" + (state.who ? "-" + state.who.replace(/\\W+/g, "-") : "") + ".txt";
  a.click();
}});
// Backup: every report's saved answers in this browser (all versions), as JSON
document.getElementById("backup").addEventListener("click", () => {{
  const all = {{}};
  try {{
    for (let i = 0; i < localStorage.length; i++) {{
      const k = localStorage.key(i);
      if (k.startsWith("rr-whats-new-")) all[k] = JSON.parse(localStorage.getItem(k) || "{{}}");
    }}
  }} catch (e) {{ alert("This browser won't let the page read its saved notes: " + e); return; }}
  const a = document.createElement("a");
  a.href = URL.createObjectURL(new Blob([JSON.stringify(all, null, 2)], {{type: "application/json"}}));
  a.download = "research-red-notes-backup.json";
  a.click();
}});
count();
</script>
</body></html>
"""


# "Your playtest": the parts of a tester's report that aren't about one change.
# (key, label, hint); kind "text" = one line, "choice" = a menu, else a box
SETUP = [
    ("device", "Emulator or device", "e.g. mGBA 0.10.5 on PC, a phone emulator, real hardware", "text", None),
    ("save", "Which save", "", "choice", ["New game", "Old save from an earlier build", "Both"]),
    ("reached", "How far you got", "e.g. beat Brock, stopped at the lunch", "text", None),
    ("time", "Time played (roughly)", "", "text", None),
    ("speed", "Game speed", "", "choice", ["Normal speed", "Fast-forward now and then", "Fast-forward most of the time"]),
    ("fun", "How fun was this build?", "1 = a chore, 5 = couldn't stop", "choice", ["1", "2", "3", "4", "5"]),
    ("difficulty", "Difficulty", "", "choice", ["Too easy", "A bit easy", "About right", "A bit hard", "Too hard"]),
]
BOXES = [
    ("text", "Text", "Typos, lines cut off, ALL CAPS left over, a line that doesn't sound like the character. Say where and quote it."),
    ("visuals", "Art and visuals", "Sprites, portraits, colours, tiles, flicker, anything that looks off."),
    ("sound", "Music and sound", "Missing, wrong or too loud."),
    ("balance", "Balance", "Levels and level caps, trainers too hard or too easy, money, items, catch rates, wild Pokémon."),
    ("story", "Story and characters", "Scenes that landed, ones that didn't, anything out of character or confusing."),
    ("pacing", "Pacing and getting lost", "Parts that dragged or felt rushed; where you didn't know what to do next."),
    ("controls", "Menus and controls", "Anything awkward, slow or easy to press by mistake."),
    ("favourite", "Favourite moments", ""),
    ("ideas", "Ideas and wishes", "Things you'd add or change."),
    ("other", "Anything else", ""),
]
BUG_TEMPLATE = """<template id="bug-template"><div class="bug"><label class="num"></label>
<label>Where<span class="hint">Map and what you were doing, e.g. Route 22, right after the double battle</span>
<input type="text" data-b="where"></label>
<label>What happened<span class="hint">And what you expected instead</span><textarea data-b="what"></textarea></label>
<div class="grid">
<label>Does it happen again?<select data-b="again"><option></option><option>Every time</option><option>Sometimes</option><option>Only once</option><option>Didn't try</option></select></label>
<label>How bad<select data-b="how_bad"><option></option><option>Crash or freeze</option><option>Blocks progress</option><option>Annoying</option><option>Just looks wrong</option></select></label>
</div>
<label>Screenshot file<span class="hint">The name from _emu\\screenshots, e.g. ResearchRed-12.png</span><input type="text" data-b="shot"></label>
<button class="remove" type="button">Remove this bug</button></div></template>"""


def general_section():
    """The "Your playtest" form at the end of every report."""
    def field(key, label, hint, kind, choices=None):
        attrs = f'data-g="{key}" data-label="{html.escape(label)}"'
        if kind == "text":
            control = f'<input type="text" {attrs}>'
        elif kind == "choice":
            control = f'<select {attrs}><option></option>' + "".join(f"<option>{html.escape(c)}</option>" for c in choices) + "</select>"
        else:
            control = f"<textarea {attrs}></textarea>"
        tip = f'<span class="hint">{html.escape(hint)}</span>' if hint else ""
        return f"<label>{html.escape(label)}{tip}{control}</label>"
    setup = "".join(field(*f) for f in SETUP)
    boxes = "".join(field(key, label, hint, "box") for key, label, hint in BOXES)
    return (f'<h2 id="playtest">Your playtest</h2><div class="panel general">'
            '<p class="what" style="margin-left:0">Everything that isn\'t about one change above. Fill in what you '
            'like and skip the rest; it is kept in this browser as you type. When you\'re done, press '
            '<b>Save my notes</b> and put the file in <code>E:\\Workspace\\romhacks\\_emu\\notes\\</code>.</p>'
            f'<div class="grid">{setup}</div>'
            '<label>Bugs<span class="hint">One card per problem, crashes and freezes too. '
            'Press F12 in mGBA for a screenshot.</span></label><div id="bugs"></div>'
            '<button id="addbug" type="button">Add a bug</button>'
            f'{boxes}</div>{BUG_TEMPLATE}')


# A PDF copy next to the page, for easy reading on a phone: Windows' Edge (or
# Chrome) prints the page headless. Skipped with a note if neither is there.
BROWSERS = ["/mnt/c/Program Files (x86)/Microsoft/Edge/Application/msedge.exe",
            "/mnt/c/Program Files/Google/Chrome/Application/chrome.exe"]


def write_pdf(out):
    browser = next((b for b in BROWSERS if os.path.exists(b)), None)
    if browser is None:
        print("  (no PDF: Edge or Chrome on Windows not found)")
        return
    win = subprocess.run(["wslpath", "-w", out], capture_output=True, text=True).stdout.strip()
    url = "file:///" + win.replace("\\", "/") + "/report.html"
    subprocess.run([browser, "--headless", "--disable-gpu", "--no-pdf-header-footer",
                    f"--print-to-pdf={win}\\report.pdf", url],
                   capture_output=True, timeout=180, cwd="/tmp")
    if os.path.exists(os.path.join(out, "report.pdf")):
        print(f"pdf:    {out}/report.pdf")
    else:
        print("  (the PDF wasn't written)")


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        sys.exit(1)
    version = sys.argv[1]
    run = sys.argv[sys.argv.index("--run") + 1] if "--run" in sys.argv else newest_run()
    spec = json.load(open(os.path.join(HERE, "whats_new", version + ".json"), encoding="utf-8"))
    out = os.path.join(WORKSPACE, "whats-new", version)
    shots_dir = os.path.join(out, "screenshots")
    if os.path.isdir(shots_dir):
        shutil.rmtree(shots_dir)
    os.makedirs(shots_dir)

    missing = []
    sections = []
    for section in spec["sections"]:
        parts = [f"<h2>{html.escape(section['title'])}</h2>"]
        for item in section["items"]:
            imgs = []
            for n, shot in enumerate(item.get("shots", []), 1):
                src = os.path.join(run, shot)
                if not os.path.exists(src):
                    missing.append(f"{item['id']}: {shot}")
                    continue
                name = f"{item['id']}-{n}.png"
                shutil.copy(src, os.path.join(shots_dir, name))
                imgs.append(f'<img src="screenshots/{name}" alt="{html.escape(item["title"])}, picture {n}">')
            parts.append(
                f'<div class="item" data-id="{item["id"]}"><div class="head"><input type="checkbox" '
                f'aria-label="Checked"><h3>{html.escape(item["title"])}</h3></div>'
                f'<p class="what">{html.escape(item["what"])}</p>'
                + (f'<p class="try"><b>Try it:</b> {html.escape(item["try"])}</p>' if item.get("try") else "")
                + (f'<div class="shots">{"".join(imgs)}</div>' if imgs else "")
                + '<textarea placeholder="Notes: what happened, anything odd?"></textarea></div>')
        sections.append("\n".join(parts))

    page = PAGE.format(version=version, intro=html.escape(spec["intro"]), save=html.escape(spec["save"]),
                       sections="\n".join(sections), general=general_section())
    open(os.path.join(out, "report.html"), "w", encoding="utf-8").write(page)
    print(f"report: {out}/report.html ({len(os.listdir(shots_dir))} screenshots from {os.path.basename(run)})")
    write_pdf(out)
    for m in missing:
        print(f"  missing shot: {m}")


if __name__ == "__main__":
    main()
