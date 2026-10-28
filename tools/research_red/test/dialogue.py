#!/usr/bin/env python3
"""Research Red: the dialogue check, run on every smoke-test scene (#120, #76).

While a scene plays, the runner records (rr_run "watch" / "watchtext"):
  - the message text as it starts (gStringVar4)
  - whose portrait is up (sShownSpeaker in src/rr_portrait.c)
  - whether the text box is open (sBoxOpen) and still printing (sMessageBoxType)

Afterwards judge() reads that timeline and reports:
  FAIL  a line that starts "Elm:" shows someone else's portrait (a stale or
        wrong picture, like Jenny's face on Elm's line in the v0.0.19 playtest)
  FAIL  narration (no "Name:") still shows a cast member's portrait
  FAIL  the text box closed while its text was still printing (a box that
        vanishes early, #76)
  WARN  a speaker's portrait was skipped (no room for the picture)

Speaker numbers follow graphics/research_red/portraits.txt (line order).
"""
import os
import re

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
TEXT_BYTES = 48       # enough for any "Name:" prefix
SETTLE = 12           # frames for the portrait to follow a new line
PLAYER = 0xFF         # sShownSpeaker for the player (SPEAKER_PLAYER + 1)
PRINTING = (2, 3)     # sMessageBoxType while text is still printing (normal, auto-scroll);
                      # it drops to 0 once printed, while the box waits for a button
_charmap = None
_speakers = None


def charmap():
    """byte -> character, from charmap.txt (single-byte entries)."""
    global _charmap
    if _charmap is None:
        _charmap = {}
        for line in open(os.path.join(REPO, "charmap.txt"), encoding="utf-8"):
            m = re.match(r"^'(.)'\s*=\s*([0-9A-F]{2})\s*$", line)
            if m and int(m[2], 16) not in _charmap:
                _charmap[int(m[2], 16)] = m[1]
    return _charmap


def speakers():
    """Speaker names in portraits.txt order (index = sShownSpeaker - 1)."""
    global _speakers
    if _speakers is None:
        _speakers = []
        for line in open(os.path.join(REPO, "graphics/research_red/portraits.txt"), encoding="utf-8"):
            if line.strip() and not line.startswith("#"):
                _speakers.append(line.split("|")[0].strip())
    return _speakers


def decode(hexstr):
    """The text up to the end marker (0xFF), unknown bytes as '?'."""
    cm, out = charmap(), []
    for i in range(0, len(hexstr), 2):
        b = int(hexstr[i:i + 2], 16)
        if b == 0xFF:
            break
        out.append(cm.get(b, "?"))
    return "".join(out)


def watch_lines(G):
    """Runner lines that start the recording (put in every scene's prelude)."""
    return [f"watchtext msg {G.sym('gStringVar4'):#x} {TEXT_BYTES}",
            f"watch speaker {G.sym('sShownSpeaker'):#x}:1",
            f"watch box {G.sym('sBoxOpen'):#x}:1",
            f"watch printer {G.sym('sMessageBoxType'):#x}:1"]


def speaker_of(text):
    """'Elm' for 'Elm: Hello', None for narration."""
    m = re.match(r"^([A-Z][\w. ]{0,15}):", text)
    return m[1] if m else None


def judge(log):
    """Runner output -> (fails, warns), each a list of sentences."""
    events = []   # (frame, kind, value)
    for line in log.splitlines():
        p = line.split()
        if p and p[0] == "WATCH" and p[1] in ("speaker", "box", "printer"):
            events.append((int(p[2]), p[1], int(p[3])))
        elif p and p[0] == "WATCHTEXT" and p[1] == "msg":
            events.append((int(p[2]), "msg", decode(p[3]) if len(p) > 3 else ""))
    events.sort(key=lambda e: e[0])

    def value_at(kind, frame, default=0):
        v = default
        for f, k, val in events:
            if f > frame:
                break
            if k == kind:
                v = val
        return v

    names = speakers()
    fails, warns, seen = [], [], set()
    for f, kind, val in events:
        if kind == "msg":
            # judge a line only if it's in the field text box
            if not any(k == "box" and v == 1 and f <= ff <= f + 2 for ff, k, v in events) \
                    and value_at("box", f) != 1:
                continue
            shown = value_at("speaker", f + SETTLE)
            who = speaker_of(val)
            line = val.replace("\n", " ")[:40]
            if who in names:
                want = names.index(who) + 1
                if shown == 0:
                    msg = f"no portrait for {who}'s line \"{line}\" (frame {f})"
                    if msg not in seen:
                        warns.append(msg)
                elif shown != want:
                    other = "the player" if shown == PLAYER else names[shown - 1] if shown <= len(names) else shown
                    fails.append(f"{who}'s line \"{line}\" shows {other}'s portrait (frame {f})")
            elif who is None and 0 < shown <= len(names):
                fails.append(f"narration \"{line}\" still shows {names[shown - 1]}'s portrait (frame {f})")
            seen.add(line)
        elif kind == "box" and val == 0:
            # closed: was the text still printing just before?
            if value_at("printer", f - 1) in PRINTING and value_at("box", f - 1) == 1:
                fails.append(f"the text box closed while its text was still printing (frame {f}): "
                             f"\"{value_at('msg', f, '')[:40]}\"")
    return fails, warns
