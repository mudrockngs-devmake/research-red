#!/usr/bin/env python3
"""Research Red: the what's-new report for a release (#75).

Builds a folder for the owner and testers:
  <workspace>/whats-new/<version>/report.html     opens in any browser
  <workspace>/whats-new/<version>/screenshots/    the same pictures, named by change

The page lists every change with its screenshots, how to try it, a checkbox and
a notes box. Ticks and notes are kept in that browser; "Save my notes" downloads
them as a text file to send back.

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
.bar button {{ background: var(--accent); color: #fff; border: 0; border-radius: 8px; padding: 8px 14px; font-weight: 600; cursor: pointer; }}
.bar input {{ border:1px solid var(--line); border-radius:8px; padding:7px 10px; background:var(--card); color:var(--ink); }}
.item {{ background: var(--card); border: 1px solid var(--line); border-radius: 12px; padding: 14px 16px; margin: 14px 0; }}
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
@media (max-width: 600px) {{ .what, .try, .shots, .missing {{ margin-left: 0; }} textarea {{ width: 100%; margin-left: 0; }} .shots img {{ width: 100%; }} }}
@media print {{ .bar {{ display:none; }} }}
</style></head>
<body><main>
<h1>Research Red {version}: what's new</h1>
<p class="lead">{intro}</p>
<p><span class="save">{save}</span></p>
<div class="bar"><input id="who" placeholder="Your name"><span id="count"></span>
<button id="export">Save my notes</button></div>
{sections}
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
document.getElementById("export").addEventListener("click", () => {{
  let out = "Research Red {version} test notes" + (state.who ? " from " + state.who : "") + "\\n\\n";
  document.querySelectorAll(".item").forEach(item => {{
    const s = state[item.dataset.id] || {{}};
    out += (s.ok ? "[x] " : "[ ] ") + item.querySelector("h3").textContent + "\\n";
    if (s.notes) out += "    " + s.notes.replace(/\\n/g, "\\n    ") + "\\n";
  }});
  const a = document.createElement("a");
  a.href = URL.createObjectURL(new Blob([out], {{type: "text/plain"}}));
  a.download = "research-red-{version}-notes" + (state.who ? "-" + state.who.replace(/\\W+/g, "-") : "") + ".txt";
  a.click();
}});
count();
</script>
</body></html>
"""


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
                       sections="\n".join(sections))
    open(os.path.join(out, "report.html"), "w", encoding="utf-8").write(page)
    print(f"report: {out}/report.html ({len(os.listdir(shots_dir))} screenshots from {os.path.basename(run)})")
    for m in missing:
        print(f"  missing shot: {m}")


if __name__ == "__main__":
    main()
