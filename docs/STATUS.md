# Status

The one-page "where are we right now". **Read it at the start of every session;
update it at the end of every session** (and whenever a build goes out).

_Last updated: 2026-10-19_

## Current build
- GitHub front page (2026-10-19): Research Red README with fan-project disclaimer, description, topics, wiki/projects off, upstream CI disabled, release notes carry the disclaimer. Credits say "story and character designs: MudrockNGS". No AI note in the README for now (owner's choice).
- **v0.0.14**: installed as `ResearchRed_local.gba` (copy in `builds/`). Smoke tests: 9/9 passing.
- Changes since the last playtest: see [CHANGELOG.md](CHANGELOG.md) (v0.0.13 and v0.0.14 entries).
- Last playtest archived: `test-archive/2026-10-18_v0.0.12_portraits-playtest/`.
- Releases: [v0.0.13](https://github.com/mudrockngs-devmake/research-red/releases/tag/v0.0.13), [v0.0.14](https://github.com/mudrockngs-devmake/research-red/releases/tag/v0.0.14) (pre-releases with the BPS patch).
- Latest smoke test: `test-runs/2026-10-19_0304_v0.0.14_*` (9/9, first run of the upgraded tester: every scene reached its end, memory checks pass, nothing stuck).

## Waiting on the owner
- [ ] Playtest v0.0.14 (the v0.0.13 features + the fixes). Testers played v0.0.13: the owner folds their notes in, marked "(tester, .13)"; check those against v0.0.14 before filing (portrait-color/portrait-in-battle notes may already be fixed). Checklist: [PLAYTESTING.md](PLAYTESTING.md) and issue #2.
- [ ] Decide when Mega / Z-Moves / Dynamax / Tera unlock (issue #1).
- [ ] Decide whether portraits should hide during camera pans (issue #3).
- [ ] Log in the next round's NOTES.md: `/cost` for the v0.0.13-14 build + GitHub/hub setup session (2026-10-18/19): $121.56, 2h36m API time, 7,494 lines added.
- [ ] Log in the next round's NOTES.md: `/cost` for the tester-upgrades session (2026-10-19, issue #18 parts 1-4); paste it in the next conversation.
- [ ] Optional: try the patch on a phone emulator / real hardware (issue #16, [COMPATIBILITY.md](COMPATIBILITY.md)).

## Next up (after the playtest)
- **New from the owner's notes (2026-10-19): issues #19-#34**, plus additions to #5 (banter as a "dex cutscene" over the entry) and #11 (Start menu casing). Top of the list: **#19 Pokédex tabs crash (blue screen)**; quick wins: #26 (fast text, L/R defaults), #22 (portrait position). Decision for the owner: #25 (when to show opponent types).
- **Tester upgrades (issue #18)**: parts 1-4 done 2026-10-19 (stuck detector, memory checks, wait-until, reusable `tester.py` split from Research Red's `smoke.py`; see [TESTING.md](TESTING.md) and `tools/research_red/test/README.md`). Left: the "later" ideas (before/after screenshot comparison, random-input stress test, AI-vs-AI balance sims, running in the GitHub Action). Tooling only, no version bump.
1. Fix whatever the playtest finds (issues labeled `playtest`).
2. Milestone **v0.1 Pallet**: title screen + boot disclaimer (#12), compatibility check (#16).
3. Then the v0.2 / v0.3 issues: lunch picnic art, Research Rules difficulty, Viridian Forest exchanges.
4. Tooling: smaller BPS patch (#17).

## Half-finished / open threads
- None in progress. (If a session ends mid-task, describe it here: what was being done, which files, what's left.)

## How to resume in a new session
1. Open Ubuntu (or double-click `E:\Workspace\Claude Hub.bat`): the Claude Hub menu appears. Work on a project > research-red > Start a fresh conversation (normal; Continue only to resume the same task). (By hand: `cd ~/romhacks/research-red && claude --continue`.)
2. Claude reads `CLAUDE.md` (rules) and this file (state), then checks `git log -5` and open issues (`gh issue list`).
3. If the test runner is missing (`~/romhacks/_tools/rr_run`), run `tools/research_red/test/setup_mgba.sh`.
