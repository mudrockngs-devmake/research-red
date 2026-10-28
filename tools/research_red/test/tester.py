#!/usr/bin/env python3
"""Reusable GBA playthrough tester (the engine behind smoke.py).

Knows how to play a pokefirered / pokeemerald-style decomp ROM headless and
judge the result, but nothing about any particular hack's story. A hack's
test file (Research Red's is smoke.py) describes its scenes as functions
returning rr_run script lines and hands them to main() in a Suite.

What it checks for every scenario:
  - crashes (the blue error screen) and a final black screen
  - scenes that never reach their end: play_until(...) fails at its limit
  - facts read from game memory: check(...) / expect(...)
  - "stuck": the screen froze for 20 seconds while buttons were pressed
and writes REPORT.md plus a contact sheet (grid of screenshots) per scenario.

Files that make up the tester (copy them all to reuse it, see README.md):
  rr_run.c + setup_mgba.sh   the headless emulator runner
  tester.py                  this file
  game_memory.py             names (flags, vars, maps) -> memory addresses
  contact_sheet.py           screenshot grids (needs ../rrpng.py)

Saved states: a script line "save @name" / "load @name" uses a state file
shared between scenarios of one run. The Suite's `first` scenario runs on its
own before the rest (in parallel), so it can save the starting point.
"""
import concurrent.futures
import dataclasses
import datetime
import os
import re
import shutil
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
sys.path.insert(0, os.path.dirname(HERE))
from contact_sheet import read_png, write_sheet  # noqa: E402
from game_memory import GameMemory  # noqa: E402

G = None              # GameMemory for the ROM under test, set by main()
PROGRESS_VAR = None   # the hack's story-progress variable, if it has one


@dataclasses.dataclass
class Suite:
    title: str                 # report heading, e.g. "Research Red v0.0.14 (abc123)"
    repo: str                  # decomp checkout (for the headers)
    rom: str                   # built .gba; the .elf next to it is used too
    scenarios: dict            # name -> (function returning script lines, description)
    out_dir: str               # where this run's report and screenshots go
    runner: str                # path to the built rr_run
    first: str = None          # scenario to run alone first (saves states)
    progress_var: str = None   # e.g. "VAR_0x40FE": enables scene_end / expect(story=)
    map_names: list = dataclasses.field(default_factory=list)  # MAP_* the report can name
    game_define: str = "FIRERED"
    first_timeout: int = 600   # seconds of real time per scenario
    timeout: int = 1200
    setup_hint: str = "build rr_run with setup_mgba.sh"
    after_run: object = None   # called with out_dir once the report is written (e.g. compare.py)
    # Checks run on EVERY scenario (the hack's own, e.g. Research Red's dialogue
    # and leftover checks): extra script lines at the start / end of each
    # script (functions of the GameMemory), and judges called afterwards with
    # (runner output, scenario folder, script text) returning (fails, warns).
    always_start: list = dataclasses.field(default_factory=list)
    always_end: list = dataclasses.field(default_factory=list)
    judges: list = dataclasses.field(default_factory=list)
    # A playthrough: play the scenarios one after another (each starts from a
    # state the one before saved), stop at the first FAIL, and keep the states
    # (.ss, loadable in mGBA) in keep_states, so a later run can start from any
    # leg ("--from NAME" loads the states found there).
    sequential: bool = False
    keep_states: str = None


# --- script building blocks ----------------------------------------------
def step(direction, tiles=1):
    """Walk `tiles` tiles."""
    return [f"hold {direction} 16", "wait 4"] * tiles


def advance(cycles, key="A", every=60, prefix="s"):
    """Advance text for a while, taking a screenshot every cycle."""
    lines = []
    for i in range(1, cycles + 1):
        lines += [f"mash {key} {every}", f"shot {prefix}{i:03d}"]
    return lines


def start_menu_item(downs):
    """Open the Start menu and pick the entry `downs` below the top."""
    return ["press START", "wait 20"] + ["press DOWN", "wait 6"] * downs + ["press A", "wait 40"]


def play_until(label, cond, max_frames, key="A", every=60):
    """Tap key until cond holds (FAIL if it takes more than max_frames),
    taking a screenshot every `every` frames."""
    return [f"autoshot {every} s", f"until {label} {max_frames} {key} {cond}", "autoshot 0"]


def ready(label, max_frames=900):
    """Press nothing until the player can walk again (after a door, a warp or
    a fade), instead of a fixed wait; FAIL if it takes more than max_frames.
    The short wait first lets a warp that is about to start lock the controls."""
    return ["wait 8", f"until {label} {max_frames} - {G.field_free()}"]


def progress_is(value):
    return G.var_is(PROGRESS_VAR, value)


def scene_end(step):
    """The scene has set story progress to `step` and handed control back."""
    return f"{progress_is(step)} && {G.field_free()}"


def set_flag(flag):
    """Set a game flag directly in memory (test setup, e.g. FLAG_RR_NO_ENCOUNTERS)."""
    addr, bit = G.flag_addr(flag)
    return [f"poke {addr} |{bit}"]


def clear_flag(flag):
    addr, bit = G.flag_addr(flag)
    return [f"poke {addr} &~{bit}"]


def not_black(shot):
    """FAIL if screenshot `shot` (taken earlier in the script) is all black,
    e.g. a menu that should show on the map (judged from the picture)."""
    return [f"# expect-not-black {shot}"]


def check(label, cond):
    return [f"check {label} {cond}"]


def expect(map_name=None, story=None, party=None):
    """Assert where the scene left us."""
    lines = []
    if map_name:
        lines += check(f"map={map_name.removeprefix('MAP_')}", G.on_map(map_name))
    if story is not None:
        lines += check(f"story={story}", progress_is(story))
    if party is not None:
        lines += check(f"team>={party}", G.party_count(party, ">="))
    return lines


def prelude():
    """Every script: stuck detection on, except while the player is simply
    standing on the map (nothing animates indoors)."""
    return ["stuck 1200", f"idle {G.field_free()}"]


def epilogue():
    """Every script ends by recording the state, for the report."""
    lines = [f"peek map {G.map_addr()}", f"peek team {G.party_addr()}"]
    if PROGRESS_VAR:
        lines.insert(0, f"peek story {G.var_addr(PROGRESS_VAR)}")
    return lines


# --- running and judging ------------------------------------------------------
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


def describe_state(peeks, map_names):
    """'story 3, PALLET_TOWN, team 1' from the epilogue's peeks."""
    names = {G.map_value(m): m.removeprefix("MAP_") for m in map_names}
    where = names.get(peeks.get("map"), f"map {peeks.get('map')}")
    parts = [where, f"team {peeks.get('team')}"]
    if "story" in peeks:
        parts.insert(0, f"story {peeks['story']}")
    return ", ".join(parts)


def with_states(lines, workdir):
    """'save @name' / 'load @name' -> a state file in this run's workdir."""
    return [re.sub(r"^(save|load) @(\w+)$", lambda m: f"{m[1]} {os.path.join(workdir, m[2])}.ss", ln)
            for ln in lines]


def run(suite, rom, name, lines, workdir, timeout):
    """Play one scenario. Returns (name, status, notes, state)."""
    sdir = os.path.join(suite.out_dir, name)
    shutil.rmtree(sdir, ignore_errors=True)   # a rerun in the same minute shares the folder
    os.makedirs(sdir, exist_ok=True)
    script = os.path.join(workdir, name + ".txt")
    start = [ln for f in suite.always_start for ln in f(G)]
    end = [ln for f in suite.always_end for ln in f(G)]
    open(script, "w").write("\n".join(with_states(prelude() + start + lines + end + epilogue(), workdir)) + "\n")
    try:
        out = subprocess.run([suite.runner, rom, script, sdir], timeout=timeout, check=True,
                             capture_output=True, text=True).stdout
    except subprocess.TimeoutExpired:
        return name, "FAIL", ["timed out"], ""
    except subprocess.CalledProcessError as e:
        return name, "FAIL", [f"runner exited {e.returncode}"], ""
    open(os.path.join(sdir, "runner.log"), "w").write(out)   # peeks, checks, oam/pal dumps
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
            if os.environ.get("RR_SHOW_PEEKS"):    # debugging: RR_SHOW_PEEKS=1 smoke.py ...
                print(f"    {name}: {p[1]} = {p[2]}")
    for judge in suite.judges:
        jf, jw = judge(out, sdir, open(script).read())
        fails += jf
        warns += jw
    states = [(f, classify(os.path.join(sdir, f))) for f in shots]
    for want in re.findall(r"^# expect-not-black (\S+)$", open(script).read(), re.M):
        if dict(states).get(want + ".png") == "black":
            fails.append(f"{want}.png is black (expected a picture)")
    crashes = [f for f, s in states if s == "crash"]
    if crashes:
        fails.insert(0, f"error screen from {crashes[0]}")
    if states[-1][1] == "black":
        warns.append("ends on a black screen")
    status = "FAIL" if fails else "WARN" if warns else "PASS"
    notes = fails + warns + [f"{len(shots)} screenshots"]
    write_sheet(os.path.join(suite.out_dir, name + ".png"), [os.path.join(sdir, f) for f in shots])
    return name, status, notes, describe_state(peeks, suite.map_names)


def write_report(suite, results):
    with open(os.path.join(suite.out_dir, "REPORT.md"), "w", encoding="utf-8") as f:
        f.write(f"# {suite.title}\n\nRun {datetime.datetime.now():%Y-%m-%d %H:%M}. "
                "Open each scenario's contact sheet to skim the scene.\n\n"
                "FAIL: crashed, a check failed, or a scene never finished. "
                "WARN: froze for a while, or ended on a black screen.\n\n"
                "| Scenario | Result | Notes | Ended at | Contact sheet |\n|---|---|---|---|---|\n")
        for name, status, notes, state in results:
            f.write(f"| {name}: {suite.scenarios[name][1]} | {status} | {'<br>'.join(notes)} | {state} "
                    f"| [{name}.png]({name}.png) |\n")


def main(suite, argv):
    """Command line: [scenario ...] [--list] [--from NAME]. Exits 1 if anything FAILs."""
    start_from = None
    if "--from" in argv:
        i = argv.index("--from")
        start_from = argv[i + 1]
        argv = argv[:i] + argv[i + 2:]
    args = [a for a in argv if not a.startswith("--")]
    if "--list" in argv:
        for k, (_, desc) in suite.scenarios.items():
            print(f"{k:10} {desc}")
        return
    if not os.path.exists(suite.runner):
        sys.exit(f"{suite.runner} not found: {suite.setup_hint}")
    elf = suite.rom.removesuffix(".gba") + ".elf"
    if not (os.path.exists(suite.rom) and os.path.exists(elf)):
        sys.exit(f"{suite.rom} (and .elf) not found: build first")
    wanted = args or list(suite.scenarios)
    if start_from:
        wanted = list(suite.scenarios)[list(suite.scenarios).index(start_from):]
    for w in wanted:
        if w not in suite.scenarios:
            sys.exit(f"unknown scenario '{w}' (see --list)")

    try:
        os.nice(10)   # low priority, so the PC stays usable while every core plays a scenario
    except OSError:
        pass
    os.makedirs(suite.out_dir, exist_ok=True)
    workdir = tempfile.mkdtemp(prefix="gba_tester_")
    rom = os.path.join(workdir, "test.gba")   # the build can keep changing while tests run
    shutil.copy(suite.rom, rom)
    shutil.copy(elf, os.path.join(workdir, "test.elf"))
    global G, PROGRESS_VAR
    G = GameMemory(suite.repo, os.path.join(workdir, "test.elf"), suite.game_define)
    PROGRESS_VAR = suite.progress_var
    scripts = {w: suite.scenarios[w][0]() for w in set(wanted) | ({suite.first} if suite.first else set())}

    results = []
    print(f"{suite.title} -> {suite.out_dir}")
    if suite.sequential:
        return main_sequential(suite, rom, wanted, scripts, workdir, start_from)
    rest = [w for w in wanted if w != suite.first]
    if suite.first:
        print(f"  {suite.first} ...", flush=True)
        results.append(run(suite, rom, suite.first, scripts[suite.first], workdir, suite.first_timeout))
        print(f"  {suite.first}: {results[-1][1]} ({'; '.join(results[-1][2])})")
        if suite.first not in wanted:
            results.pop()
    for w in rest:
        for m in re.finditer(r"^load @(\w+)$", "\n".join(scripts[w]), re.M):
            if f"save @{m[1]}" in scripts[w]:   # the scenario saves that state itself
                continue
            if not os.path.exists(os.path.join(workdir, m[1] + ".ss")):
                sys.exit(f"'{w}' needs the state @{m[1]}, which {suite.first or 'nothing'} didn't save")
    with concurrent.futures.ThreadPoolExecutor(max_workers=os.cpu_count() or 4) as pool:
        futures = [pool.submit(run, suite, rom, w, scripts[w], workdir, suite.timeout) for w in rest]
        for f in concurrent.futures.as_completed(futures):
            r = f.result()
            results.append(r)
            print(f"  {r[0]}: {r[1]} ({'; '.join(r[2])})", flush=True)

    order = list(suite.scenarios)
    results.sort(key=lambda r: order.index(r[0]))
    write_report(suite, results)
    shutil.rmtree(workdir, ignore_errors=True)
    if suite.after_run:
        suite.after_run(suite.out_dir)
    failed = [r for r in results if r[1] == "FAIL"]
    print(f"{len(results) - len(failed)}/{len(results)} passed. Report: {os.path.join(suite.out_dir, 'REPORT.md')}")
    sys.exit(1 if failed else 0)


def main_sequential(suite, rom, wanted, scripts, workdir, start_from):
    """Play the legs in order; a FAIL stops the run (later legs need its state)."""
    if start_from and suite.keep_states:
        for f in os.listdir(suite.keep_states):
            if f.endswith(".ss"):
                shutil.copy(os.path.join(suite.keep_states, f), workdir)
    results = []
    for w in wanted:
        print(f"  {w} ...", flush=True)
        r = run(suite, rom, w, scripts[w], workdir, suite.timeout)
        results.append(r)
        print(f"  {w}: {r[1]} ({'; '.join(r[2])})", flush=True)
        if suite.keep_states:
            os.makedirs(suite.keep_states, exist_ok=True)
            for f in os.listdir(workdir):
                if f.endswith(".ss"):
                    shutil.copy(os.path.join(workdir, f), suite.keep_states)
        if r[1] == "FAIL":
            break
    write_report(suite, results)
    shutil.rmtree(workdir, ignore_errors=True)
    if suite.after_run:
        suite.after_run(suite.out_dir)
    failed = [r for r in results if r[1] == "FAIL"]
    print(f"{len(results) - len(failed)}/{len(wanted)} legs passed. Report: {os.path.join(suite.out_dir, 'REPORT.md')}")
    sys.exit(1 if failed else 0)
