# Status

The one-page "where are we right now". **Read it at the start of every session;
update it at the end of every session** (and whenever a build goes out).

_Last updated: 2026-10-20 (v0.0.17 released)_

## Current build
- **v0.0.17** (2026-10-20): v0.0.16 + the Field notebook look (#73) + item descriptions on pickup, snappier battle text, B to RUN. Installed as `ResearchRed_local.gba`, tagged, pre-release on GitHub. Smoke tests 33/33 (`test-runs/2026-10-20_0942_v0.0.17_*`). **Save: old saves work.**
- **v0.0.16** (2026-10-20, pre-release on GitHub): the big systems build, see [CHANGELOG.md](CHANGELOG.md).
- **Reports for the testers:** `E:\Workspace\romhacks\research-red\whats-new\v0.0.16\report.html` (33 items, everything new since v0.0.15) and `...\whats-new\v0.0.17\report.html` (the new look). Rebuild with `python3 tools/research_red/rr_whats_new.py <version>`.
- Interface colours: `tools/research_red/ui_theme.py notebook` is applied in the source (`--restore` returns to FireRed's). Mock-ups: `E:\Workspace\romhacks\research-red\mockups\`.
- Last playtest archived: `test-archive/2026-10-18_v0.0.12_portraits-playtest/`.

## Waiting on the owner
- [ ] Playtest **v0.0.17** with both reports (v0.0.16 and v0.0.17) (and hand it to the v0.0.13 testers). Afterwards: `tools/research_red/rr_archive_test.sh <name>`, file issues, NOTES.md.
- [ ] Rework the placeholder lines added in v0.0.16 (Ranch Pager tutorial, Oak's Pokémon, Rest, the Rules call, the rematches, the lab aide).
- [ ] Log in the next round's NOTES.md: the costs since v0.0.12: 2026-10-18/19 build + hub setup $121.56 (2h36m API); 2026-10-19 $62.88/$63.85 (1h42m API); 2026-10-20 sessions: run `session-cost --all-days` (this v0.0.16 session included).
- [ ] Optional: phone emulator / real hardware (#16).

## Next up: v0.0.18
- Plan: [ROADMAP.md](ROADMAP.md) "Next build: v0.0.18" (battle backgrounds per location #74 first, with mock-ups; title screen mock-ups #12, rematch Megas and other Gym leaders #52, Brock watch scene #69, banter cutscene #5/#4, ambient Pokémon #64, Forest exchanges #9, Pokédex Area tab #20, proper case #11, Cruller, art items).
- After that: test-tool upgrades (#18; a route planner from map data was prototyped this session for the Pewter Gym path), smaller patch (#17).

## Half-finished / open threads
- Nothing half-done: everything is committed and pushed.
- Rematch rules (owner, 2026-10-20) are on #52: build on each character's existing team, separate tiers for 7 and 8 badges, a post-game tier with regional forms and exchange-program visitors (Mimey, Muffin stay home), Megas in the post-game tier (to add).
- Still needs the owner's eyes: #20 (Pokédex tab glitches), #14 (Route 22 tiles), #37 (Ivy and ledges), #41 (Totodile evolution under the cap).
- The League gate block (Route 22) has no automatic test; check by hand (#2).
- 13 rows of `docs/reference/kanto_anime.csv` are marked "memory" (to verify).

## How to resume in a new session
1. Open Ubuntu (or double-click `E:\Workspace\Claude Hub.bat`): the Claude Hub menu appears. Work on a project > research-red > Start a fresh conversation (normal; Continue only to resume the same task). (By hand: `cd ~/romhacks/research-red && claude --continue`.)
2. Claude reads `CLAUDE.md` (rules) and this file (state), then checks `git log -5` and open issues (`gh issue list`).
3. If the test runner is missing (`~/romhacks/_tools/rr_run`), run `tools/research_red/test/setup_mgba.sh`.
