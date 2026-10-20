# Status

The one-page "where are we right now". **Read it at the start of every session;
update it at the end of every session** (and whenever a build goes out).

_Last updated: 2026-10-20_

## Current build
- GitHub front page (2026-10-19): Research Red README with fan-project disclaimer, description, topics, wiki/projects off, upstream CI disabled, release notes carry the disclaimer. Credits say "story and character designs: MudrockNGS". No AI note in the README for now (owner's choice).
- **v0.0.15** (2026-10-19): installed as `ResearchRed_local.gba` (copy in `builds/`), tagged and pushed (the Action makes the GitHub pre-release with the BPS). Smoke tests: 15/15 passing (new: pokedex, daisy, pewter, demo_end, gerald, pc).
- Changes since the last playtest: see [CHANGELOG.md](CHANGELOG.md) (v0.0.13, v0.0.14 and v0.0.15 entries).
- Last playtest archived: `test-archive/2026-10-18_v0.0.12_portraits-playtest/`.
- Releases: [v0.0.13](https://github.com/mudrockngs-devmake/research-red/releases/tag/v0.0.13), [v0.0.14](https://github.com/mudrockngs-devmake/research-red/releases/tag/v0.0.14) (pre-releases with the BPS patch).
- Latest smoke test: `test-runs/2026-10-20_0044_*` (16/16, new scenario `digletts`).
- **master is ahead of v0.0.15** with v0.0.16 work (not released or installed yet; the installed ROM is still v0.0.15): see CHANGELOG "v0.0.16 (in progress)" and ROADMAP "Next build: v0.0.16".

## Waiting on the owner
- [ ] (Skipped by the owner: v0.0.15 playtest; they'll test v0.0.16 long-term instead.) Old note: playtest **v0.0.15** (the v0.0.13-15 features; v0.0.15 is the big polish build, see its changelog). Testers played v0.0.13: the owner folds their notes in, marked "(tester, .13)"; check those against v0.0.14 before filing (portrait-color/portrait-in-battle notes may already be fixed). Checklist: [PLAYTESTING.md](PLAYTESTING.md) and issue #2.
- [ ] Log in the next round's NOTES.md: `/cost` for the v0.0.13-14 build + GitHub/hub setup session (2026-10-18/19): $121.56, 2h36m API time, 7,494 lines added.
- [ ] Log in the next round's NOTES.md: the 2026-10-19 session (tester upgrades, ideas filed, v0.0.15 build), logged automatically: `2026-10-19 03:56  $62.88  1h42m API  12h18m total` (the same long session, 2026-10-19/20, as of the wrap-up; `session-cost --all-days`).
- [ ] Optional: try the patch on a phone emulator / real hardware (issue #16, [COMPATIBILITY.md](COMPATIBILITY.md)).

## Next up: build v0.0.16
- The whole plan is in [ROADMAP.md](ROADMAP.md) "Next build: v0.0.16" (Step 1 dialogue pass, Step 2 systems and content, Step 3 release). Every decision is on its GitHub issue.
- **Step 1 progress (2026-10-20):** done and approved by the owner: intro call, breakfast, tall grass, lab, Daisy's house, Route 1 (sing-along + catch reactions for the new tables), Route 22, Viridian lunch, Pewter. The old "Mimey's dishes" bet is gone everywhere (new stakes: dinner at Delia's). Couples' names: [characters/NAMES.md](characters/NAMES.md). Still to do in Step 1: Ivy's follower chat, Pokédex banter, then the lines that come with Step 2 features (Rules in Options, Cruller at Daisy's, Brock watch scene, tutorials).
- Owner wants: one question at a time; show mock-ups first for the title screen (#12), the interface colours (#73) and the TM split.
- Step 3 includes the new what's-new report with screenshots (#75).
- After v0.0.16: test-tool upgrades (#18), smaller patch (#17).

## Half-finished / open threads
- No code half-done: master builds, rr_check passes, smoke tests 16/16. v0.0.16 work so far is committed (tables, caps, blockers, line updates).
- New reference and planning docs (2026-10-20): `docs/characters/` (OAK, IVY, PLAYER, ELM, JENNY, DELIA, DAISY, MIMEY_MUFFIN_SLOWPOKE, REWORK), `docs/reference/` (ANIME_AND_GEN2, KANTO_ANIME + kanto_anime.csv via `tools/research_red/build_reference.py`, VOICES_EARLY_ANIME), `docs/scenes/pewter_brock_watch.md`. Copies on E: in `reference/`.
- The League gate block (Route 22) has no automatic test; check by hand (#2).
- 13 rows of `docs/reference/kanto_anime.csv` are marked "memory" (to verify). More of the reference (Johto, movies, #66) can be added a few pages per session from the Internet Archive.
- Still needs the owner's eyes in a playtest: #20 (Pokédex tab glitches), #14 (Route 22 tiles), #37 (Ivy and ledges), #41 (Totodile evolution).

## How to resume in a new session
1. Open Ubuntu (or double-click `E:\Workspace\Claude Hub.bat`): the Claude Hub menu appears. Work on a project > research-red > Start a fresh conversation (normal; Continue only to resume the same task). (By hand: `cd ~/romhacks/research-red && claude --continue`.)
2. Claude reads `CLAUDE.md` (rules) and this file (state), then checks `git log -5` and open issues (`gh issue list`).
3. If the test runner is missing (`~/romhacks/_tools/rr_run`), run `tools/research_red/test/setup_mgba.sh`.
