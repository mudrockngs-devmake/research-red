#!/usr/bin/env python3
"""Research Red: automated smoke tests.

Plays the ROM headless (rr_run, built by setup_mgba.sh) through each story
scene using the debug skips and saves screenshots. Checks:
  - crashes (the blue error screen) and black screens
  - each scene reaches its end (story step set, player free to walk) before a
    time limit, instead of mashing for a fixed number of frames
  - facts read from game memory afterwards: map, story step, team size
    (game_memory.py turns those names into addresses)
  - "stuck": the screen froze for 20 seconds while buttons were pressed
For each scenario it writes a contact sheet (a grid of screenshots) so a
person can skim the whole scene in seconds.

Usage:
  python3 tools/research_red/test/smoke.py              # all scenarios
  python3 tools/research_red/test/smoke.py lunch banter # just these
  python3 tools/research_red/test/smoke.py --list

Output: <workspace>/test-runs/<date>_<version>_<commit>/
  REPORT.md            pass / fail per scenario
  <scenario>.png       contact sheet
  <scenario>/          every screenshot
Run it after building, before handing a ROM to the owner.
"""
import concurrent.futures
import datetime
import os
import re
import shutil
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(os.path.dirname(os.path.dirname(HERE)))
sys.path.insert(0, HERE)
sys.path.insert(0, os.path.dirname(HERE))
from contact_sheet import read_png, write_sheet  # noqa: E402
from game_memory import GameMemory  # noqa: E402

TOOLS = os.environ.get("RR_TOOLS", os.path.expanduser("~/romhacks/_tools"))
RR_RUN = os.path.join(TOOLS, "rr_run")
WORKSPACE = "/mnt/e/Workspace/romhacks/research-red"

STORY = "VAR_0x40FE"  # story progress, see CLAUDE.md
G = None              # GameMemory for the ROM under test, set in main()

# Debug menu (Start > DEBUG > Scripts): see data/scripts/debug.inc
SCRIPT_LAB, SCRIPT_AFTER_LAB, SCRIPT_ROUTE1, SCRIPT_ROUTE22, SCRIPT_TEAM, \
    SCRIPT_LUNCH, SCRIPT_BANTER, SCRIPT_ROUTE1_ELM, SCRIPT_TEST_MODE = range(1, 10)


# --- script building blocks ----------------------------------------------
def debug_script(n):
    """Start > DEBUG > Scripts > script n."""
    lines = ["press START", "wait 20", "press A", "wait 20"]
    lines += ["press DOWN", "wait 6"] * 5          # Scripts is the 6th entry
    lines += ["press A", "wait 20"]
    lines += ["press DOWN", "wait 6"] * (n - 1)
    lines += ["press A", "wait 240"]
    return lines


def step(direction, tiles=1):
    return [f"hold {direction} 16", "wait 4"] * tiles


def advance(cycles, key="A", every=60, prefix="s"):
    """Advance text for a while, taking a screenshot every cycle."""
    lines = []
    for i in range(1, cycles + 1):
        lines += [f"mash {key} {every}", f"shot {prefix}{i:03d}"]
    return lines


def start_menu_item(downs):
    return ["press START", "wait 20"] + ["press DOWN", "wait 6"] * downs + ["press A", "wait 40"]


def play_until(label, cond, max_frames, key="A", every=60):
    """Tap key until cond holds (FAIL if it takes more than max_frames),
    taking a screenshot every `every` frames."""
    return [f"autoshot {every} s", f"until {label} {max_frames} {key} {cond}", "autoshot 0"]


def scene_end(step):
    """The scene has set story progress to `step` and handed control back."""
    return f"{G.var_is(STORY, step)} && {G.field_free()}"


def check(label, cond):
    return [f"check {label} {cond}"]


def expect(map_name=None, story=None, party=None):
    """Assert where the scene left us."""
    lines = []
    if map_name:
        lines += check(f"map={map_name.removeprefix('MAP_')}", G.on_map(map_name))
    if story is not None:
        lines += check(f"story={story}", G.var_is(STORY, story))
    if party is not None:
        lines += check(f"team>={party}", G.party_count(party, ">="))
    return lines


def prelude():
    """Every script: stuck detection on, except while the player is simply
    standing on the map (nothing animates indoors)."""
    return ["stuck 1200", f"idle {G.field_free()}"]


def epilogue():
    """Every script ends by recording the state, for the report."""
    return [f"peek story {G.var_addr(STORY)}", f"peek map {G.map_addr()}", f"peek team {G.party_addr()}"]


# --- scenarios ------------------------------------------------------------
# Each returns script lines. "@bedroom" is replaced by loading the state saved
# at the end of the intro scenario (a new game standing in the bedroom).
def sc_intro():
    lines = ["wait 240", "press START", "wait 200", "press START", "wait 150"]
    lines += play_until("bedroom", f"{G.on_map('MAP_PALLET_TOWN_PLAYERS_HOUSE_2F')} && {G.field_free()}",
                        12000, every=270)
    lines += ["wait 200", "mash B 120", "wait 60", "shot s999"]
    lines += expect("MAP_PALLET_TOWN_PLAYERS_HOUSE_2F", story=0)
    return lines + ["save @bedroom"]


def sc_breakfast():
    return (["load @bedroom"] + step("RIGHT", 4) + step("UP", 5) + ["hold LEFT 20", "wait 60", "shot s000"]
            + play_until("breakfast", scene_end(1), 6000)
            + expect("MAP_PALLET_TOWN_PLAYERS_HOUSE_1F", story=1))


def sc_lab():
    return (["load @bedroom"] + debug_script(SCRIPT_TEST_MODE) + ["mash B 40"] + debug_script(SCRIPT_LAB)
            + ["shot s000"] + step("UP") + play_until("lab", scene_end(3), 60000)
            + expect(story=3, party=1))


def sc_route1():
    return (["load @bedroom"] + debug_script(SCRIPT_ROUTE1_ELM) + ["shot s000"] + step("DOWN")
            + play_until("route1", scene_end(5), 6000, key="B", every=24)
            + advance(20, key="B", every=24, prefix="t")   # the Mewtwo fly-by
            + expect("MAP_ROUTE1", story=5))


def sc_route22():
    return (["load @bedroom"] + debug_script(SCRIPT_ROUTE22) + debug_script(SCRIPT_TEST_MODE) + ["mash B 40", "shot s000"]
            + step("LEFT") + play_until("route22", scene_end(6), 30000)
            + expect("MAP_ROUTE22", story=6, party=1))


def sc_lunch():
    return (["load @bedroom"] + debug_script(SCRIPT_LUNCH) + ["shot s000"] + step("RIGHT", 3)
            + play_until("lunch", scene_end(7), 10000, key="B", every=50)
            + expect("MAP_VIRIDIAN_CITY", story=7))


def sc_banter():
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB) + debug_script(SCRIPT_BANTER) + ["wait 30"]
            + advance(26, every=50) + expect(story=3, party=1))


def sc_guide():
    return (["load @bedroom"] + debug_script(SCRIPT_ROUTE1) + step("UP", 4) + ["wait 60", "shot s000"]
            # Start menu here: DEBUG, POKEDEX, POKEMON, Guide
            + start_menu_item(3) + ["shot s001", "press RIGHT", "wait 20", "shot s002",
                                    "press B", "wait 30", "shot s003"]
            + expect("MAP_ROUTE1"))


def sc_rules():
    # after the lunch: Start menu DEBUG, POKEDEX, POKEMON, Guide, Rules
    return (sc_lunch() + ["mash B 200", "wait 60"] + start_menu_item(4)
            + ["shot t001", "press DOWN", "wait 10", "press RIGHT", "wait 10", "shot t002",
               "press B", "wait 30", "shot t003"])


# Maps the report can name (anything else shows as a number)
MAP_NAMES = ["MAP_PALLET_TOWN", "MAP_PALLET_TOWN_PLAYERS_HOUSE_1F", "MAP_PALLET_TOWN_PLAYERS_HOUSE_2F",
             "MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB", "MAP_PALLET_TOWN_RIVALS_HOUSE", "MAP_ROUTE1",
             "MAP_VIRIDIAN_CITY", "MAP_ROUTE22"]

SCENARIOS = {
    "intro": (sc_intro, "New game: the video call, ending in the bedroom"),
    "breakfast": (sc_breakfast, "Downstairs: breakfast with Delia, Ivy and Mimey"),
    "lab": (sc_lab, "Lab scene (test mode: the AI fights)"),
    "route1": (sc_route1, "Route 1: Elm and Jenny, the Mewtwo fly-by"),
    "route22": (sc_route22, "Route 22: the 6v6 double (test mode)"),
    "lunch": (sc_lunch, "Viridian lunch, Officer Jenny, Research Rules"),
    "banter": (sc_banter, "Pokédex banter preview (debug 7)"),
    "guide": (sc_guide, "Start > Guide on Route 1"),
    "rules": (sc_rules, "Start > Rules after the lunch"),
}


# --- checks ---------------------------------------------------------------
def classify(path):
    """'crash' for the blue error screen, 'black' for an all-black frame."""
    w, h, px = read_png(path)
    blue = sum(1 for row in px[::8] for c in row[::8] if c[2] > 200 and c[0] < 40 and c[1] < 40)
    total = len(px[::8]) * len(px[0][::8])
    if blue > total * 0.6:
        return "crash"
    if all(max(c) < 16 for row in px[::4] for c in row[::4]):
        return "black"
    return "ok"


def describe_state(peeks):
    """'story 3, PALLET_TOWN, team 1' from the epilogue's peeks."""
    names = {G.map_value(m): m.removeprefix("MAP_") for m in MAP_NAMES}
    where = names.get(peeks.get("map"), f"map {peeks.get('map')}")
    return f"story {peeks.get('story')}, {where}, team {peeks.get('team')}"


def run(rom, name, lines, workdir, outdir, timeout):
    """Returns (name, status, notes, state)."""
    sdir = os.path.join(outdir, name)
    os.makedirs(sdir, exist_ok=True)
    lines = prelude() + lines + epilogue()
    text = "\n".join(lines).replace("@bedroom", os.path.join(workdir, "bedroom.ss"))
    script = os.path.join(workdir, name + ".txt")
    open(script, "w").write(text + "\n")
    try:
        out = subprocess.run([RR_RUN, rom, script, sdir], timeout=timeout, check=True,
                             capture_output=True, text=True).stdout
    except subprocess.TimeoutExpired:
        return name, "FAIL", ["timed out"], ""
    except subprocess.CalledProcessError as e:
        return name, "FAIL", [f"runner exited {e.returncode}"], ""
    shots = sorted(f for f in os.listdir(sdir) if f.endswith(".png"))
    if not shots:
        return name, "FAIL", ["no screenshots"], ""
    fails, warns, peeks = [], [], {}
    for line in out.splitlines():
        p = line.split()
        if p[0] == "CHECK" and p[2] == "fail":
            fails.append(f"check {p[1]} failed (read {p[3]})")
        elif p[0] == "UNTIL" and p[2] == "timeout":
            fails.append(f"never reached '{p[1]}' within {p[3]} frames (see timeout_{p[1]}.png)")
        elif p[0] == "STUCK":
            warns.append(f"screen frozen 20 s with buttons pressed after {p[3]} "
                         f"(script line {p[1]}, see stuck_{int(p[1]):05d}.png)")
        elif p[0] == "PEEK":
            peeks[p[1]] = int(p[2])
    states = [(f, classify(os.path.join(sdir, f))) for f in shots]
    crashes = [f for f, s in states if s == "crash"]
    if crashes:
        fails.insert(0, f"error screen from {crashes[0]}")
    if states[-1][1] == "black":
        warns.append("ends on a black screen")
    status = "FAIL" if fails else "WARN" if warns else "PASS"
    notes = fails + warns + [f"{len(shots)} screenshots"]
    write_sheet(os.path.join(outdir, name + ".png"), [os.path.join(sdir, f) for f in shots])
    return name, status, notes, describe_state(peeks)


def version_and_commit():
    src = open(os.path.join(REPO, "src/rr_intro_call.c"), encoding="utf-8").read()
    m = re.search(r"v\d+\.\d+\.\d+", src)
    commit = subprocess.run(["git", "-C", REPO, "rev-parse", "--short=9", "HEAD"], capture_output=True, text=True).stdout.strip()
    dirty = subprocess.run(["git", "-C", REPO, "status", "--porcelain"], capture_output=True, text=True).stdout.strip()
    return (m.group(0) if m else "v?"), commit + ("-dirty" if dirty else "")


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    if "--list" in sys.argv:
        for k, (_, desc) in SCENARIOS.items():
            print(f"{k:10} {desc}")
        return
    if not os.path.exists(RR_RUN):
        sys.exit(f"{RR_RUN} not found: run tools/research_red/test/setup_mgba.sh first")
    rom = os.path.join(REPO, "pokefirered.gba")
    if not os.path.exists(rom):
        sys.exit("pokefirered.gba not found: build first (make firered -j6)")
    wanted = args or list(SCENARIOS)
    for w in wanted:
        if w not in SCENARIOS:
            sys.exit(f"unknown scenario '{w}' (see --list)")

    version, commit = version_and_commit()
    base = os.path.join(WORKSPACE, "test-runs") if os.path.isdir(WORKSPACE) else os.path.join(REPO, "build/test-runs")
    outdir = os.path.join(base, f"{datetime.datetime.now():%Y-%m-%d_%H%M}_{version}_{commit}")
    os.makedirs(outdir, exist_ok=True)
    workdir = tempfile.mkdtemp(prefix="rr_smoke_")
    romcopy = os.path.join(workdir, "test.gba")   # the build can keep changing while tests run
    shutil.copy(rom, romcopy)
    elfcopy = os.path.join(workdir, "test.elf")
    shutil.copy(rom.removesuffix(".gba") + ".elf", elfcopy)
    global G
    G = GameMemory(REPO, elfcopy)

    results = []
    print(f"Smoke tests for {version} ({commit}) -> {outdir}")
    # Every other scenario starts from the bedroom state the intro saves
    print("  intro ...", flush=True)
    results.append(run(romcopy, "intro", SCENARIOS["intro"][0](), workdir, outdir, 600))
    print(f"  intro: {results[-1][1]} ({'; '.join(results[-1][2])})")
    rest = [w for w in wanted if w != "intro"]
    if rest and not os.path.exists(os.path.join(workdir, "bedroom.ss")):
        sys.exit("the intro didn't reach the bedroom; can't run the other scenarios")
    with concurrent.futures.ThreadPoolExecutor(max_workers=os.cpu_count() or 4) as pool:
        futures = [pool.submit(run, romcopy, w, SCENARIOS[w][0](), workdir, outdir, 1200) for w in rest]
        for f in concurrent.futures.as_completed(futures):
            r = f.result()
            results.append(r)
            print(f"  {r[0]}: {r[1]} ({'; '.join(r[2])})", flush=True)

    order = list(SCENARIOS)
    results.sort(key=lambda r: order.index(r[0]))
    with open(os.path.join(outdir, "REPORT.md"), "w", encoding="utf-8") as f:
        f.write(f"# Smoke test: {version} ({commit})\n\nRun {datetime.datetime.now():%Y-%m-%d %H:%M}. "
                "Open each scenario's contact sheet to skim the scene.\n\n"
                "FAIL: crashed, a check failed, or a scene never finished. "
                "WARN: froze for a while, or ended on a black screen.\n\n"
                "| Scenario | Result | Notes | Ended at | Contact sheet |\n|---|---|---|---|---|\n")
        for name, status, notes, state in results:
            f.write(f"| {name}: {SCENARIOS[name][1]} | {status} | {'<br>'.join(notes)} | {state} "
                    f"| [{name}.png]({name}.png) |\n")
    shutil.rmtree(workdir, ignore_errors=True)
    failed = [r for r in results if r[1] == "FAIL"]
    print(f"{len(results) - len(failed)}/{len(results)} passed. Report: {os.path.join(outdir, 'REPORT.md')}")
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
