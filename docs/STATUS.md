# Status

The one-page "where are we right now". **Read it at the start of every session;
update it at the end of every session** (and whenever a build goes out).

_Last updated: 2026-10-19_

## Current build
- **v0.0.14**: installed as `ResearchRed_local.gba` (copy in `builds/`). Smoke tests: 9/9 passing.
- Changes since the last playtest: see [CHANGELOG.md](CHANGELOG.md) (v0.0.13 and v0.0.14 entries).
- Last playtest archived: `test-archive/2026-10-18_v0.0.12_portraits-playtest/`.
- Releases: [v0.0.13](https://github.com/mudrockngs-devmake/research-red/releases/tag/v0.0.13), [v0.0.14](https://github.com/mudrockngs-devmake/research-red/releases/tag/v0.0.14) (pre-releases with the BPS patch).
- Latest smoke test: `test-runs/2026-10-19_0014_v0.0.14_*` (9/9).

## Waiting on the owner
- [ ] Playtest v0.0.14 (the v0.0.13 features + the fixes). Checklist: [PLAYTESTING.md](PLAYTESTING.md) and issue #2.
- [ ] Decide when Mega / Z-Moves / Dynamax / Tera unlock (issue #1).
- [ ] Decide whether portraits should hide during camera pans (issue #3).
- [ ] Run `/cost` and log it in the next round's NOTES.md.
- [ ] Optional: try the patch on a phone emulator / real hardware (issue #16, [COMPATIBILITY.md](COMPATIBILITY.md)).

## Next up (after the playtest)
1. Fix whatever the playtest finds (issues labeled `playtest`).
2. Milestone **v0.1 Pallet**: title screen + boot disclaimer (#12), compatibility check (#16).
3. Then the v0.2 / v0.3 issues: lunch picnic art, Research Rules difficulty, Viridian Forest exchanges.
4. Tooling: smaller BPS patch (#17).

## Half-finished / open threads
- None in progress. (If a session ends mid-task, describe it here: what was being done, which files, what's left.)

## How to resume in a new session
1. `cd ~/romhacks/research-red`, start Claude Code (`claude --continue` reopens the last conversation).
2. Claude reads `CLAUDE.md` (rules) and this file (state), then checks `git log -5` and open issues (`gh issue list`).
3. If the test runner is missing (`~/romhacks/_tools/rr_run`), run `tools/research_red/test/setup_mgba.sh`.
