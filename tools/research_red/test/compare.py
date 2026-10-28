#!/usr/bin/env python3
"""Before/after picture comparison for the playthrough tester (#18).

Compares a test run's screenshots with an earlier run's, scenario by
scenario and picture by picture (s0001.png with s0001.png). The emulator and
its clock are repeatable, so an unchanged scene gives the very same pictures;
anything that differs is a real change (or a scene that now takes longer,
which shifts every later picture).

  python3 compare.py RUN_DIR                     # against the last tagged build (git describe)
  python3 compare.py RUN_DIR --tag v0.0.19       # against that tag's runs
  python3 compare.py RUN_DIR --against OLD_RUN   # against one run folder
  python3 compare.py RUN_DIR --show lab [s0040]  # before/after slider in the live viewer

The baseline for each scenario is the newest clean run (folder name
<date>_<version>_<commit>, no "-dirty") of a commit at or before the tag
that played that scenario, so partial runs count too.

Writes into RUN_DIR:
  COMPARE.md       a line per scenario: same / changed (how many pictures, the first one)
  compare.html     the changed pictures: before | after | changes marked in pink
  _compare/        the marked pictures
and adds a "Pictures compared" section to REPORT.md. Needs Pillow.
"""
import argparse
import html
import os
import re
import subprocess
import sys

from PIL import Image, ImageChops

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(os.path.dirname(os.path.dirname(HERE)))
RUN_NAME = re.compile(r"^(\d{4}-\d{2}-\d{2}_\d{4})_(v[\w.]+)_([0-9a-f]+)(-dirty)?$")

PIXEL_TOLERANCE = 24      # a colour channel must move more than this to count
CHANGED_SHARE = 0.002     # and more than 0.2% of the screen (77 of 240x160 pixels)
SHOWN_PER_SCENE = 6       # changed pictures shown per scenario in compare.html


def git(*args):
    return subprocess.run(["git", "-C", REPO, *args], capture_output=True, text=True).stdout.strip()


def last_tag():
    return git("describe", "--tags", "--abbrev=0", "--match", "v*")


class Baselines:
    """Finds, per scenario, the newest clean run at or before a tag."""

    def __init__(self, runs_dir, tag, skip):
        self.runs = []
        ancestor = {}
        for name in sorted(os.listdir(runs_dir), reverse=True):    # newest first
            m = RUN_NAME.match(name)
            if not m or m[4] or name == skip:
                continue
            commit = m[3]
            if commit not in ancestor:
                ancestor[commit] = subprocess.run(
                    ["git", "-C", REPO, "merge-base", "--is-ancestor", commit, tag],
                    capture_output=True).returncode == 0
            if ancestor[commit]:
                self.runs.append(os.path.join(runs_dir, name))

    def find(self, scenario):
        for run in self.runs:
            if os.path.isfile(os.path.join(run, scenario, "runner.log")):
                return run
        return None


class Fixed:
    def __init__(self, run):
        self.run = run

    def find(self, scenario):
        return self.run if os.path.isdir(os.path.join(self.run, scenario)) else None


def shots(folder):
    return sorted(f for f in os.listdir(folder) if f.endswith(".png"))


def changed_pixels(before, after):
    """None if the two pictures look the same, else a mask of what changed."""
    if open(before, "rb").read() == open(after, "rb").read():
        return None
    a, b = Image.open(before).convert("RGB"), Image.open(after).convert("RGB")
    if a.size != b.size:
        return Image.new("L", b.size, 255)
    diff = ImageChops.difference(a, b).convert("L").point(lambda v: 255 if v > PIXEL_TOLERANCE else 0)
    count = diff.histogram()[255]
    return diff if count > CHANGED_SHARE * b.size[0] * b.size[1] else None


def marked(after, mask, out):
    """The new picture, dimmed, with the changed pixels in pink."""
    pic = Image.open(after).convert("RGB")
    dim = Image.blend(pic, Image.new("RGB", pic.size, (0, 0, 0)), 0.55)
    dim.paste((255, 64, 160), mask=mask)
    dim.resize((pic.size[0] * 2, pic.size[1] * 2), Image.NEAREST).save(out)


def compare_scene(run_dir, base_dir, scenario):
    new_dir, old_dir = os.path.join(run_dir, scenario), os.path.join(base_dir, scenario)
    new, old = shots(new_dir), set(shots(old_dir))
    common = [f for f in new if f in old]
    changed = []
    for f in common:
        mask = changed_pixels(os.path.join(old_dir, f), os.path.join(new_dir, f))
        if mask is not None:
            changed.append((f, mask))
    return {
        "scenario": scenario, "base": base_dir, "compared": len(common), "changed": changed,
        "only_new": [f for f in new if f not in old], "only_old": sorted(old - set(new)),
    }


def write_outputs(run_dir, results, against):
    out = os.path.join(run_dir, "_compare")
    os.makedirs(out, exist_ok=True)
    differ = [r for r in results if r["changed"] or r["only_new"] or r["only_old"]]

    md = [f"# Pictures compared with {against}\n",
          "Same = every picture matches the earlier run. A scene that now takes longer "
          "shifts all its later pictures, so many changes can mean one longer scene.\n",
          "| Scenario | Result | Compared with |", "|---|---|---|"]
    for r in results:
        md.append(f"| {r['scenario']} | {summary(r)} | {os.path.basename(r['base']) if r['base'] else '-'} |")
    open(os.path.join(run_dir, "COMPARE.md"), "w").write("\n".join(md) + "\n")

    cards = []
    for r in differ:
        rel_base = os.path.relpath(r["base"], run_dir)
        rows = []
        for f, mask in r["changed"][:SHOWN_PER_SCENE]:
            mark = f"{r['scenario']}_{f}"
            marked(os.path.join(run_dir, r["scenario"], f), mask, os.path.join(out, mark))
            rows.append(
                f'<div class="row"><div class="name">{f}</div>'
                f'<figure><img src="{rel_base}/{r["scenario"]}/{f}"><figcaption>before</figcaption></figure>'
                f'<figure><img src="{r["scenario"]}/{f}"><figcaption>after</figcaption></figure>'
                f'<figure><img src="_compare/{mark}"><figcaption>what changed</figcaption></figure></div>')
        more = len(r["changed"]) - SHOWN_PER_SCENE
        if more > 0:
            rows.append(f'<p class="note">and {more} more changed pictures</p>')
        cards.append(f'<section><h2>{html.escape(r["scenario"])}</h2><p class="note">{html.escape(summary(r))}. '
                     f'Before: {html.escape(os.path.basename(r["base"]))}</p>{"".join(rows)}</section>')
    same = len(results) - len(differ)
    body = "".join(cards) or "<p>Every picture matches. Nothing looks different.</p>"
    open(os.path.join(run_dir, "compare.html"), "w").write(PAGE.format(
        title=html.escape(f"Pictures compared with {against}"),
        intro=f"{len(differ)} scenes look different, {same} look the same.", body=body))

    report = os.path.join(run_dir, "REPORT.md")
    if os.path.exists(report):
        text = open(report).read().split("\n## Pictures compared")[0].rstrip("\n")
        lines = [f"\n\n## Pictures compared with {against}\n",
                 f"{len(differ)} scenes look different, {same} the same "
                 "(details: COMPARE.md, pictures: compare.html).\n"]
        lines += [f"- {r['scenario']}: {summary(r)}" for r in differ]
        open(report, "w").write(text + "\n".join(lines) + "\n")
    return differ


def summary(r):
    if not r["base"]:
        return "new scenario (nothing to compare with)"
    parts = []
    if r["changed"]:
        parts.append(f"changed: {len(r['changed'])} of {r['compared']} pictures, first {r['changed'][0][0]}")
    if r["only_new"]:
        parts.append(f"{len(r['only_new'])} more pictures than before")
    if r["only_old"]:
        parts.append(f"{len(r['only_old'])} fewer pictures than before")
    return "; ".join(parts) or "same"


PAGE = """<!doctype html><html><head><meta charset="utf-8"><title>Pictures compared</title>
<meta name="viewport" content="width=device-width, initial-scale=1"><style>
:root {{ --bg:#f4f1ea; --fg:#2b2b2b; --card:#fff; --muted:#6b6b6b; --line:#ddd6c8; }}
@media (prefers-color-scheme: dark) {{ :root {{ --bg:#16202a; --fg:#e6e6e6; --card:#1f2c38; --muted:#9aa8b4; --line:#2e3e4d; }} }}
body {{ background:var(--bg); color:var(--fg); font:15px/1.5 system-ui, sans-serif; margin:0; padding:16px; }}
h1 {{ font-size:22px; margin:0 0 4px; }} h2 {{ font-size:18px; margin:0 0 4px; }}
section {{ background:var(--card); border:1px solid var(--line); border-radius:10px; padding:14px; margin:14px 0; }}
.row {{ display:flex; flex-wrap:wrap; gap:10px; align-items:flex-start; border-top:1px solid var(--line); padding:10px 0; }}
.name {{ width:100%; font-weight:600; }}
figure {{ margin:0; }} figcaption {{ color:var(--muted); font-size:13px; }}
img {{ width:480px; max-width:100%; image-rendering:pixelated; display:block; border-radius:4px; }}
.note {{ color:var(--muted); margin:0 0 6px; }}
</style></head><body><h1>{title}</h1><p class="note">{intro} Pink marks the pixels that changed.</p>{body}</body></html>
"""


def compare_run(run_dir, tag=None, against=None, quiet=False):
    """Compare a whole run; returns the scenarios that differ (also used by smoke.py)."""
    run_dir = os.path.abspath(run_dir)
    if against:
        finder, label = Fixed(os.path.abspath(against)), os.path.basename(os.path.abspath(against))
    else:
        tag = tag or last_tag()
        if not tag:
            print("compare: no v* tag to compare with")
            return []
        finder, label = Baselines(os.path.dirname(run_dir), tag, os.path.basename(run_dir)), tag
    scenarios = sorted(d for d in os.listdir(run_dir) if os.path.isfile(os.path.join(run_dir, d, "runner.log")))
    results = []
    for s in scenarios:
        base = finder.find(s)
        if base:
            results.append(compare_scene(run_dir, base, s))
        else:
            results.append({"scenario": s, "base": None, "compared": 0, "changed": [], "only_new": [], "only_old": []})
    differ = write_outputs(run_dir, results, label)
    if not quiet:
        print(f"Pictures vs {label}: {len(differ)} of {len(results)} scenes look different"
              + (": " + ", ".join(r["scenario"] for r in differ) if differ else "")
              + f". Page: {os.path.join(run_dir, 'compare.html')}")
    return differ


def show(run_dir, scenario, shot, tag, against):
    """Push a before/after pair to the live viewer (its slider)."""
    run_dir = os.path.abspath(run_dir)
    if against:
        base = Fixed(os.path.abspath(against)).find(scenario)
    else:
        base = Baselines(os.path.dirname(run_dir), tag or last_tag(), os.path.basename(run_dir)).find(scenario)
    if not base:
        sys.exit(f"no earlier run of {scenario} to compare with")
    if not shot:
        r = compare_scene(run_dir, base, scenario)
        if not r["changed"]:
            sys.exit(f"{scenario}: every picture matches")
        shot = r["changed"][0][0]
    shot = shot if shot.endswith(".png") else shot + ".png"
    subprocess.run([sys.executable, os.path.join(REPO, "tools/research_red/viewer/viewer.py"), "show",
                    os.path.join(base, scenario, shot), os.path.join(run_dir, scenario, shot),
                    "--title", f"{scenario} {shot}: before / after"])


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("run_dir")
    ap.add_argument("--tag", help="compare with this tag's runs (default: the last v* tag)")
    ap.add_argument("--against", help="compare with this one run folder")
    ap.add_argument("--show", nargs="+", metavar=("SCENARIO", "SHOT"), help="open a before/after in the viewer")
    a = ap.parse_args()
    if a.show:
        show(a.run_dir, a.show[0], a.show[1] if len(a.show) > 1 else None, a.tag, a.against)
    else:
        compare_run(a.run_dir, a.tag, a.against)


if __name__ == "__main__":
    main()
