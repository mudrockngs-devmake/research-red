# Status

The one-page "where are we right now". **Read it at the start of every session;
update it at the end of every session** (and whenever a build goes out).

_Last updated: 2026-10-20 (v0.0.18 released; session wrapped up)_

## Current build
- **v0.0.18** (2026-10-20): v0.0.17 + battle backgrounds per place in cozier colours (#74, `tools/research_red/battle_bg_theme.py`), the test-mode wild battle freeze fix. Installed as `ResearchRed_local.gba`, tagged, pre-release on GitHub. Smoke tests 35/35 (`test-runs/2026-10-20_0952_v0.0.18_*`). **Save: old saves work.**
- **v0.0.17** (2026-10-20, pre-release): the Field notebook look (#73) + item descriptions on pickup, snappier battle text, B to RUN.
- **v0.0.16** (2026-10-20, pre-release on GitHub): the big systems build, see [CHANGELOG.md](CHANGELOG.md).
- **Reports for the testers:** `E:\Workspace\romhacks\research-red\whats-new\v0.0.16\report.html` (33 items, everything new since v0.0.15) and `...\whats-new\v0.0.17\report.html` (the new look). Rebuild with `python3 tools/research_red/rr_whats_new.py <version>`.
- Interface colours: `tools/research_red/ui_theme.py notebook` is applied in the source (`--restore` returns to FireRed's). Mock-ups: `E:\Workspace\romhacks\research-red\mockups\`.
- Last playtest archived: `test-archive/2026-10-18_v0.0.12_portraits-playtest/`.

## Waiting on the owner
- [ ] Playtest **v0.0.18** with the three reports (v0.0.16, v0.0.17, v0.0.18) (and hand it to the v0.0.13 testers). Afterwards: `tools/research_red/rr_archive_test.sh <name>`, file issues, NOTES.md.
- [ ] Rework the placeholder lines added in v0.0.16 (Ranch Pager tutorial, Oak's Pokémon, Rest, the Rules call, the rematches, the lab aide).
- [ ] Log in the next round's NOTES.md (`session-cost` lines since v0.0.12): 2026-10-18/19 build + hub setup $121.56 (2h36m API); `2026-10-19 03:56  $63.85  1h42m API`; `2026-10-20 04:06  $49.62  1h22m API` (dialogue pass, Step 2 menus/battles/evolution); `2026-10-20 07:18  $48.48  0h58m API  2h44m total` (this session: 100 TMs, Ranch Pager, Rest, calls, rematches, v0.0.16-18 releases, as of the wrap-up).
- [ ] Optional: phone emulator / real hardware (#16).

## Next up: v0.0.19
- Plan: [ROADMAP.md](ROADMAP.md) "Next build: v0.0.19" (optional new background art #74 round 2; title screen mock-ups #12, rematch Megas and other Gym leaders #52, Brock watch scene #69, banter cutscene #5/#4, ambient Pokémon #64, Forest exchanges #9, Pokédex Area tab #20, proper case #11, Cruller, art items).
- Test tools (#18): done this session: route planner (`tools/research_red/test/route_plan.py`), memory pokes (`poke`, `T.set_flag`/`T.clear_flag`, `FLAG_RR_NO_ENCOUNTERS`). Still open: wait-until-ready, the engine's battle tests. Then a smaller patch (#17).

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
