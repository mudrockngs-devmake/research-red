#!/usr/bin/env python3
"""Research Red Art Kit: the gate between the art workshop and the game.

graphics/research_red/art_kit.toml lists every piece that will get new art
and its stage (placeholder -> draft -> hand-finished -> approved). Only
APPROVED pieces are in the game; this tool makes the game files match that
list, and the checks make sure nothing slips in by accident.

  python3 tools/research_red/art/art_gate.py status            # every piece: stage, in the game?
  python3 tools/research_red/art/art_gate.py set ow:elm hand-finished ["note"]
  python3 tools/research_red/art/art_gate.py test ow:elm on     # in the game for testing, NOT approved (off = the original back)
  python3 tools/research_red/art/art_gate.py apply             # game files = the approved + testing list
  python3 tools/research_red/art/art_gate.py tracker           # docs/ART_TRACKER.md (+ copy on E:)
  python3 tools/research_red/art/art_gate.py verdicts          # take the owner's viewer verdicts into the tracker
  python3 tools/research_red/art/art_gate.py check             # rr_check: game matches, tracker complete and current

Approving is the owner's call only (docs/ART_TOOLS.md "Putting the art in the game").
"""
import filecmp
import glob
import json
import os
import re
import shutil
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import artlib  # noqa: E402

REPO = artlib.REPO
TRAINERS_H = os.path.join(REPO, "src/data/graphics/trainers.h")
TRACKER_MD = os.path.join(REPO, "docs/ART_TRACKER.md")
TRACKER_COPY = "/mnt/e/Workspace/romhacks/research-red/ART_TRACKER.md"
ANSWERS = "/mnt/e/Workspace/romhacks/research-red/viewer/answers"

# battle:<cast> -> (the trainers.h picture name, what the game used before)
BATTLE = {
    "player_m": ("Red", "graphics/research_red/cast/player_m/battle"),
    "player_f": ("Leaf", "graphics/research_red/cast/player_f/battle"),
    "elm": ("RR_Elm", "graphics/research_red/cast/elm/battle"),
    "oak": ("RR_Oak", "graphics/research_red/cast/oak/battle"),
    "daisy": ("RR_Daisy", "graphics/research_red/cast/daisy/battle"),
    "delia": ("RR_Delia", "graphics/research_red/cast/delia/battle"),
    "jenny": ("RR_Jenny", "graphics/research_red/cast/jenny/battle"),
    "brock": ("LeaderBrock", "graphics/trainers/{}/leader_brock"),
    "misty": ("LeaderMisty", "graphics/trainers/{}/leader_misty"),
    "surge": ("LeaderLtSurge", "graphics/trainers/{}/leader_lt_surge"),
    "erika": ("LeaderErika", "graphics/trainers/{}/leader_erika"),
    "koga": ("LeaderKoga", "graphics/trainers/{}/leader_koga"),
    "sabrina": ("LeaderSabrina", "graphics/trainers/{}/leader_sabrina"),
    "blaine": ("LeaderBlaine", "graphics/trainers/{}/leader_blaine"),
    "giovanni": ("LeaderGiovanni", "graphics/trainers/{}/leader_giovanni"),
    "lorelei": ("EliteFourLorelei", "graphics/trainers/{}/elite_four_lorelei"),
    "bruno": ("EliteFourBruno", "graphics/trainers/{}/elite_four_bruno"),
    "agatha": ("EliteFourAgatha", "graphics/trainers/{}/elite_four_agatha"),
    "lance": ("EliteFourLance", "graphics/trainers/{}/elite_four_lance"),
}
# files a piece swaps wholesale: (game file, approved version, placeholder copy)
PLAYERS = [
    ("graphics/research_red/player_ow/male_normal.png", "graphics/research_red/player_ow/male_normal.png", "male_normal.png"),
    ("graphics/research_red/player_ow/female_normal.png", "graphics/research_red/player_ow/female_normal.png", "female_normal.png"),
    ("graphics/object_events/palettes/player.pal", "graphics/research_red/overworld/player_warm.pal", "player.pal"),
]
BACK = [("graphics/trainers/back_pics/%s_back_pic.png", "%s_back_pic.png"),
        ("graphics/trainers/palettes/%s_back_pic.pal", "%s_back_pic.pal")]


def battle_paths(cast):
    """(picture, palette) the game should use for battle:<cast>, without extensions."""
    name, old = BATTLE[cast]
    if artlib.in_game("battle:" + cast):
        base = "graphics/research_red/cast/%s/battle_fr" % cast
        return base, base
    if "{}" in old:  # FireRed's own: front_pics/x_front_pic + palettes/x
        return old.format("front_pics") + "_front_pic", old.format("palettes")
    return old, old


def battle_lines(text):
    """The paths trainers.h uses now for each battle piece: {cast: (pic, pal)}."""
    found = {}
    for cast, (name, _) in BATTLE.items():
        pic = re.search(r'gTrainerFrontPic_%s\[\] = INCBIN_U32\("([^"]+)\.4bpp\.smol"\)' % name, text)
        pal = re.search(r'gTrainerPalette_%s\[\] = INCBIN_U16\("([^"]+)\.gbapal"\)' % name, text)
        found[cast] = (pic.group(1) if pic else None, pal.group(1) if pal else None)
    return found


def apply_battle():
    text = open(TRAINERS_H).read()
    for cast, (name, _) in BATTLE.items():
        pic, pal = battle_paths(cast)
        note = " // Research Red: Art Kit (battle:%s)" % cast if artlib.in_game("battle:" + cast) else ""
        text = re.sub(r'(gTrainerFrontPic_%s\[\] = INCBIN_U32\(")[^"]+(\.4bpp\.smol"\);)[^\n]*' % name,
                      lambda m: m.group(1) + pic + m.group(2) + (note or _keep_comment(cast, "pic")), text)
        text = re.sub(r'(gTrainerPalette_%s\[\] = INCBIN_U16\(")[^"]+(\.gbapal"\);)[^\n]*' % name,
                      lambda m: m.group(1) + pal + m.group(2) + (note or _keep_comment(cast, "pal")), text)
    open(TRAINERS_H, "w").write(text)


def _keep_comment(cast, which):
    # the comments these lines had before the Art Kit (v0.0.19)
    return " // Research Red (#24)" if cast == "player_f" else ""


def apply_swaps():
    """The players (sheets + player.pal) and the back pics: approved file or the placeholder."""
    for game, new, keep in PLAYERS:
        src = os.path.join(REPO, new) if artlib.in_game("ow:players") else os.path.join(artlib.PLACEHOLDER_DIR, keep)
        if os.path.abspath(src) != os.path.join(REPO, game):
            shutil.copyfile(src, os.path.join(REPO, game))
    for slot in ("red", "leaf"):
        for game, keep in BACK:
            src = os.path.join(artlib.DRAFTS_DIR if artlib.in_game("back_pics") else artlib.PLACEHOLDER_DIR, keep % slot)
            shutil.copyfile(src, os.path.join(REPO, game % slot))
    for gen in glob.glob(os.path.join(REPO, "graphics/trainers/back_pics/*_back_pic.4bpp")) + \
            glob.glob(os.path.join(REPO, "graphics/trainers/palettes/*_back_pic.gbapal")) + \
            [os.path.join(REPO, "graphics/object_events/palettes/player.gbapal")]:
        if os.path.exists(gen):
            os.remove(gen)  # generated: make rebuilds them from the files above


def cmd_apply():
    import ow_maker
    recipes, results = ow_maker.build_all()
    ow_maker.write_outputs(recipes, results)
    ow_maker.install(recipes, results, {n for n in recipes if artlib.in_game(ow_maker.piece_of(n))})
    apply_battle()
    # back pics: build_back_pics.py makes the drafts; the swap picks drafts or placeholders
    apply_swaps()
    on = [p for p in artlib.load_pieces() if artlib.in_game(p)]
    print("in the game: %s" % (", ".join(on) if on else "nothing from the Art Kit (all placeholders)"))
    print("now: make firered -j6, rr_check.py, smoke tests")
    return 0


# ---------------------------------------------------------------- checks

def problems():
    out = []
    pieces = artlib.load_pieces()
    for pid, v in pieces.items():
        if v.get("stage") not in artlib.STAGES:
            out.append("%s: stage %r is not one of %s" % (pid, v.get("stage"), ", ".join(artlib.STAGES)))
    # every Art Kit piece is in the tracker
    import tomllib
    recipes = tomllib.load(open(os.path.join(artlib.OW_DIR, "recipes.toml"), "rb"))
    import ow_maker
    for n in recipes:
        if ow_maker.piece_of(n) not in pieces:
            out.append("art_kit.toml: no tracker entry for %s (recipe %s)" % (ow_maker.piece_of(n), n))
    for cast in BATTLE:
        if "battle:" + cast not in pieces:
            out.append("art_kit.toml: no tracker entry for battle:%s" % cast)
    # the game matches the approved list
    text = open(os.path.join(REPO, "src/data/object_events/object_event_graphics_info_pointers.h")).read()
    ids = open(os.path.join(REPO, "include/constants/event_objects.h")).read()
    for n, r in recipes.items():
        if n in ("player_m", "player_f"):
            continue
        c = ow_maker.code_names(n, r)
        if r.get("replaces"):
            inside = "// Research Red: Art Kit (%s)," % n in text
        else:
            inside = "#define RR_ART_HAS_%s\n" % c["gfx"] in ids
        want = artlib.in_game("ow:" + n)
        if inside != want:
            out.append("ow:%s is %s the game but %s (run art_gate.py apply)" % (n, "in" if inside else "not in", "not approved" if inside else "approved"))
    for cast, (pic, pal) in battle_lines(open(TRAINERS_H).read()).items():
        if (pic, pal) != battle_paths(cast):
            out.append("battle:%s: trainers.h uses %s, the tracker says %s (run art_gate.py apply)" % (cast, pic, battle_paths(cast)[0]))
    for game, new, keep in PLAYERS:
        want = os.path.join(REPO, new) if artlib.in_game("ow:players") else os.path.join(artlib.PLACEHOLDER_DIR, keep)
        if not filecmp.cmp(os.path.join(REPO, game), want, shallow=False):
            out.append("ow:players: %s doesn't match the tracker (run art_gate.py apply)" % game)
    for slot in ("red", "leaf"):
        for game, keep in BACK:
            want = os.path.join(artlib.DRAFTS_DIR if artlib.in_game("back_pics") else artlib.PLACEHOLDER_DIR, keep % slot)
            if not filecmp.cmp(os.path.join(REPO, game % slot), want, shallow=False):
                out.append("back_pics: %s doesn't match the tracker (run art_gate.py apply)" % (game % slot))
    # the tracker page is current
    if not os.path.exists(TRACKER_MD) or open(TRACKER_MD).read() != tracker_text():
        out.append("docs/ART_TRACKER.md is stale (run art_gate.py tracker)")
    return out


def cmd_check():
    p = problems()
    for line in p:
        print(line)
    if not p:
        n = len(artlib.load_pieces())
        print("art gate: %d pieces tracked, the game matches (approved: %d)" % (
            n, sum(1 for v in artlib.load_pieces().values() if v.get("stage") == "approved")))
    return 1 if p else 0


# ---------------------------------------------------------------- the tracker page

GROUPS = [("ow:", "Overworld sprites (walking around)"), ("battle:", "Battle pictures"),
          ("back_pics", "Back pictures"), ("game:", "Game lane (placed with the old tools; redo in the art pass)")]
BADGE = {"placeholder": "placeholder", "draft": "draft", "hand-finished": "hand-finished", "approved": "**in the game**"}


def tracker_text():
    pieces = artlib.load_pieces()
    counts = {s: sum(1 for v in pieces.values() if v.get("stage") == s) for s in artlib.STAGES}
    lines = ["# Art tracker", "",
             "_Made by `tools/research_red/art/art_gate.py tracker` from `graphics/research_red/art_kit.toml`: "
             "edit that file (or `art_gate.py set`), not this page._", "",
             "Every piece of art that will be replaced, where it shows up, what the game uses now, and how far "
             "its new version is. Stages: placeholder -> draft -> hand-finished -> **approved** (only approved "
             "pieces are in the game; the owner's call). The first pieces to go in are the Pallet Town ones "
             "(the vertical slice, marked P).", "",
             "Totals: " + ", ".join("%d %s" % (counts[s], s) for s in artlib.STAGES), ""]
    for prefix, title in GROUPS:
        rows = [(k, v) for k, v in pieces.items() if k.startswith(prefix)]
        if not rows:
            continue
        lines += ["## " + title, "", "| Piece | What | Where in the game | The game uses now | Stage | P |",
                  "|---|---|---|---|---|---|"]
        for k, v in rows:
            stage = BADGE.get(v.get("stage"), v.get("stage"))
            if v.get("testing") is True and v.get("stage") != "approved":
                stage += ", **in the game for testing**"
            if v.get("verdict"):
                stage += " (owner: %s)" % v["verdict"]
            what = v.get("what", "") + (" _(%s)_" % v["notes"] if v.get("notes") else "")
            lines.append("| `%s` | %s | %s | %s | %s | %s |" % (
                k, what, v.get("where", ""), v.get("game_now", ""), stage, "P" if v.get("slice") == "pallet" else ""))
        lines.append("")
    return "\n".join(lines)


def cmd_tracker():
    text = tracker_text()
    open(TRACKER_MD, "w").write(text)
    try:
        os.makedirs(os.path.dirname(TRACKER_COPY), exist_ok=True)
        open(TRACKER_COPY, "w", newline="\r\n").write(text)
        print("tracker -> docs/ART_TRACKER.md and E:\\Workspace\\romhacks\\research-red\\ART_TRACKER.md")
    except OSError:
        print("tracker -> docs/ART_TRACKER.md")
    return 0


# ---------------------------------------------------------------- editing the tracker

def set_field(pid, field, value):
    """Change one field of one piece in art_kit.toml, keeping its comments and layout."""
    path = artlib.ART_KIT_SETTINGS
    text = open(path).read()
    head = '[pieces.%s]' % json.dumps(pid)
    if head not in text:
        raise SystemExit("no piece %s in art_kit.toml" % pid)
    i = text.index(head) + len(head)
    j = text.find("\n[", i)
    j = len(text) if j < 0 else j + 1
    section = text[i:j]
    line = "%s = %s" % (field, json.dumps(value, ensure_ascii=False))
    if re.search(r"^%s = .*$" % field, section, flags=re.M):
        section = re.sub(r"^%s = .*$" % field, lambda m: line, section, count=1, flags=re.M)
    else:
        section = section.rstrip("\n") + "\n" + line + "\n\n"
    open(path, "w").write(text[:i] + section + text[j:])


def cmd_set(pid, stage, note=None):
    if stage not in artlib.STAGES:
        raise SystemExit("stage must be one of: " + ", ".join(artlib.STAGES))
    set_field(pid, "stage", stage)
    if note:
        set_field(pid, "notes", note)
    cmd_tracker()
    print("%s -> %s%s" % (pid, stage, "  (now run art_gate.py apply, then build and test)" if stage == "approved" else ""))
    return 0


def cmd_test(pid, state):
    """Switch a piece on or off for testing. The stage doesn't change: this is not an approval."""
    if pid not in artlib.load_pieces() or state not in ("on", "off"):
        raise SystemExit("usage: art_gate.py test ID on|off (ID from art_gate.py status)")
    set_field(pid, "testing", state == "on")
    return cmd_apply() or cmd_tracker()


def cmd_verdicts():
    """The viewer's Approve / Needs work answers about a piece -> the tracker's verdict
    (and its note). The stage itself only changes with art_gate.py set."""
    done = os.path.join(ANSWERS, "applied")
    taken = 0
    for path in sorted(glob.glob(os.path.join(ANSWERS, "*_verdict.json"))):
        a = json.load(open(path))
        pid = a.get("piece")
        if pid and pid in artlib.load_pieces():
            label = {"approve": "approves", "needs_work": "needs work"}.get(a.get("verdict"), a.get("verdict"))
            set_field(pid, "verdict", "%s, %s" % (label, a.get("sent", "")[:10]))
            note = "; ".join(([a["text"]] if a.get("text") else []) +
                             ["spot %d,%d: %s" % (sp["x"], sp["y"], sp.get("text", "")) for sp in a.get("spots", [])])
            if note:
                set_field(pid, "notes", note)
            print("%s: %s%s" % (pid, label, (": " + note) if note else ""))
            os.makedirs(done, exist_ok=True)
            shutil.move(path, os.path.join(done, os.path.basename(path)))
            taken += 1
    if taken:
        cmd_tracker()
    else:
        print("no new verdicts")
    return 0


def cmd_status():
    for pid, v in artlib.load_pieces().items():
        print("  %-24s %-14s %s%s" % (pid, v.get("stage"), "IN GAME  " if v.get("stage") == "approved" else "TESTING  " if v.get("testing") is True else "",
                                      v.get("what", "")))
    return 0


def main(argv):
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__)
        return 0
    cmd, rest = argv[0], argv[1:]
    if cmd == "status":
        return cmd_status()
    if cmd == "set" and len(rest) >= 2:
        return cmd_set(*rest[:3])
    if cmd == "test" and len(rest) == 2:
        return cmd_test(*rest)
    if cmd == "apply":
        return cmd_apply()
    if cmd == "tracker":
        return cmd_tracker()
    if cmd == "verdicts":
        return cmd_verdicts()
    if cmd in ("check", "--check"):
        return cmd_check()
    print(__doc__)
    return 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
