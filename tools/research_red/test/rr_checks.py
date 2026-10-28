#!/usr/bin/env python3
"""Research Red: checks that run on EVERY smoke-test scene (and the playthrough).

smoke.py hands these to the tester (Suite.always_start / always_end / judges),
so no scene has to ask for them:

  dialogue    each "Name:" line shows that speaker's portrait; narration shows
              none; no text box closes while its text is still printing
              (dialogue.py; #120, #76)
  leftovers   when the scene ends, nothing a battle or a scene switches on for
              a while is still on: the AI playing your side (outside test
              mode), "no whiteout" (#106-style leaks)
  brightness  how bright every screenshot is (mean, 0-255), saved as
              brightness.json in the scene's folder; a scene can demand a range
              with bright(shot, lo, hi) (day/night, #108)

Each check reads game memory or the pictures, never the screenshots' timing.
"""
import json
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import dialogue  # noqa: E402
from contact_sheet import read_png  # noqa: E402

TEST_MODE_MARK = "# rr: test-mode"   # smoke.py writes it when a scene turns on test mode (debug 9)

# Flags that must be off once a scene hands control back (outside test mode)
LEFTOVER_FLAGS = {
    "FLAG_RR_NO_WHITEOUT": "'no whiteout' is still on after the battle (it's set around story battles only)",
    "FLAG_RR_AI_VS_AI": "the AI is still playing your side after the scene (outside test mode)",
}


# --- dialogue -----------------------------------------------------------------
def dialogue_start(G):
    return dialogue.watch_lines(G)


def dialogue_judge(out, sdir, script):
    return dialogue.judge(out)


# --- leftovers ----------------------------------------------------------------
def leftovers_end(G):
    # judged only if the scene ended with no script running (some scenes stop mid-battle on purpose)
    lines = [f"peek leftover_script {G.sym('sGlobalScriptContextStatus'):#x}",
             f"# leftover-idle {G.const('script_shutdown')}"]
    for flag in LEFTOVER_FLAGS:
        addr, bit = G.flag_addr(flag)
        lines.append(f"peek leftover_{flag} {addr}")
        lines.append(f"# leftover-bit {flag} {bit}")
    return lines


def leftovers_judge(out, sdir, script):
    fails = []
    idle = re.search(r"^# leftover-idle (\d+)$", script, re.M)
    status = re.search(r"^PEEK leftover_script (\d+)$", out, re.M)
    if not (idle and status and status[1] == idle[1]):
        return [], []
    bits = dict(re.findall(r"^# leftover-bit (\S+) (\d+)$", script, re.M))
    for flag, why in LEFTOVER_FLAGS.items():
        if flag == "FLAG_RR_AI_VS_AI" and TEST_MODE_MARK in script:
            continue
        m = re.search(rf"^PEEK leftover_{flag} (\d+)$", out, re.M)
        if m and flag in bits and int(m[1]) & int(bits[flag]):
            fails.append(why)
    return fails, []


# --- brightness ---------------------------------------------------------------
def luma(path):
    """Mean brightness of a screenshot, 0 (black) to 255 (white)."""
    w, h, px = read_png(path)
    vals = [0.299 * c[0] + 0.587 * c[1] + 0.114 * c[2] for row in px[::4] for c in row[::4]]
    return sum(vals) / len(vals)


def bright(shot, lo, hi):
    """Script lines: FAIL unless screenshot `shot` (taken earlier) has a mean
    brightness between lo and hi (0-255)."""
    return [f"# expect-bright {shot} {lo} {hi}"]


def brightness_judge(out, sdir, script):
    fails, values = [], {}
    for f in sorted(os.listdir(sdir)):
        if f.endswith(".png"):
            values[f[:-4]] = round(luma(os.path.join(sdir, f)), 1)
    json.dump(values, open(os.path.join(sdir, "brightness.json"), "w"), indent=0)
    for shot, lo, hi in re.findall(r"^# expect-bright (\S+) (\S+) (\S+)$", script, re.M):
        v = values.get(shot)
        if v is None:
            fails.append(f"{shot}.png missing (brightness check)")
        elif not float(lo) <= v <= float(hi):
            fails.append(f"{shot}.png brightness {v} is outside {lo}-{hi}")
    return fails, []


ALWAYS_START = [dialogue_start]
ALWAYS_END = [leftovers_end]
JUDGES = [dialogue_judge, leftovers_judge, brightness_judge]
