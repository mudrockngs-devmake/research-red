# Status

The one-page "where are we right now". **Read it at the start of every session;
update it at the end of every session** (and whenever a build goes out).
Keep it short (about 80 lines): when a run is finished, move its checklist to
[history/](history/) (one file per month) and leave a one-line result here.

_Last updated: 2026-10-28 (working setup run; v0.0.19 still in playtest)_

## Now
- **v0.0.19 is out for the owner's playtest.** Work is in two lanes: game-lane sessions use the art already in the
  game; Art Kit sessions improve the tools and drafts (#104: Pallet Town first). No Art Kit piece is approved yet
  (`art_gate.py status`), so none is in the game (owner, 2026-10-24).
- Since v0.0.19 (in master, not in a build yet): tester upgrades (#18, docs/TESTING.md); debug 10's team is Lv 14.

## Working setup run (2026-10-28, items 1-6 done; owner answers in ROADMAP.md "Working setup")
- [x] 5 short STATUS, history in docs/history/
- [x] 1 glossary docs/GLOSSARY.md
- [x] 2 game design document docs/DESIGN.md
- [x] 3 notes intake skill (.claude/skills/owner-notes)
- [x] 4 project skills: test build, playtest intake, new scene
- [x] 6 rr_check before every commit (git hook)
- [ ] later, in a claude-setup session: 7 session-start hook, 8 project template (glossary, DESIGN, short STATUS +
      history, .claude/skills, .githooks + core.hooksPath, GitHub checks, issue forms), 9 handbook, 10 global owner-notes skill

## Current build
- **v0.0.19** (2026-10-21): installed as `ResearchRed_local.gba`, tagged, pre-release on GitHub. Smoke 61/61.
  What's-new report: `E:\Workspace\romhacks\research-red\whats-new\v0.0.19\report.html`. **Save: start a new game.**
- Last playtest archived: `test-archive/2026-10-20_v0.0.18_playtest/` (issues #76-#95).
- After the playtest: `tools/research_red/rr_archive_test.sh <test-name>`, then the notes become issues.

## Waiting on the owner
- [ ] v0.0.19 playtest with the what's-new report (pick portraits or full-body there; Debug > Scripts > 13 switches).
- [ ] Notes on the lab and the fights (#105 so far), then balance: the Pewter rematch with Elm and Jenny wins 6% for a
      team at the cap (Lv 14), Brock 94% (docs/BALANCE.md "Simulated fights"). Tune it, or keep it a hard optional fight?
- [ ] Try the viewer (F5: Approve / Needs work, Mark a spot, Animation, In game, History, themes) and the hub's look,
      Today screen, Settings and desktop shortcut; say what feels off.
- [ ] Hub "I'm done for today" (backups were 5 days old on 2026-10-24).
- [ ] The 3 questions at the end of docs/ART_BIBLE.md (outline colour, battle picture proportions, signature colours).
- [ ] Rework draft lines: Oak's grant / Lab Supplies, the lab's Oak rounds (#86, now #105), the aide's PC topic,
      Slowking's pages; and the v0.0.16 placeholders (Ranch Pager tutorial, Oak's Pokémon, Rest, the Rules call, rematches, the aide).
- [ ] Optional: phone emulator / real hardware (#16); workshop the report's playtest form.

## Next up
- After the playtest: v0.0.20 from the owner's notes (#105 lab rework, rematch tiers #52, playtest issues).
- Tooling: a smaller patch (#17).

## Watch closely in the v0.0.19 playtest (new or reworked code, most likely to break)
- Lab Supplies: buying, a full Items pocket ("no more room"), leaving back to the PC menu, from Start > PC and a real PC.
- The lab scene after the Elm and Jenny battle, win and loss, Yes and No to both questions; Slowpoke on the table later; Muffin when Daisy visits.
- The dex banter after a catch and with SELECT: B to skip, mashing A or B, Elm/Jenny family species, switching the look (debug 13).
- Battles where you or Ivy speak (Route 22): pictures appearing and leaving.
- The move relearner, the PC item box and the Berry Pouch (pocket sizes changed), saving and loading on a new game.

## Half-finished / open threads
- Nothing half-done. All Art Kit art is drafts (tracker: docs/ART_TRACKER.md).
- In AI-vs-AI battles (test mode, the lab's watched Oak battle) the player's side plays almost at random (no partner =
  no AI settings); harmless so far. The battle sims work around it; #105 turns the watched fights into cutscenes.
- Still needs the owner's eyes: #20 (Pokédex tab glitches), #14 (Route 22 tiles), #37 (Ivy and ledges), #41 (Totodile under the cap).
- The League gate block (Route 22) has no automatic test; check by hand (#2).
- 13 rows of `docs/reference/kanto_anime.csv` are marked "memory" (to verify).

## Session costs not yet logged (log them in the next playtest round's NOTES.md)
- v0.0.19 part 1 $110.87; part 2 $28.04 (2026-10-21). 2026-10-24: Art Kit rounds 1-2 $33.74; two lanes, viewer, hub $9.54.
- 2026-10-27/28: tester upgrades + working setup (add at wrap-up).

## Capacity (measured 2026-10-21)
| Space | Used | Notes |
| --- | --- | --- |
| Cartridge (ROM) | ~24.6 of 32 MB | ~7 MB free |
| Save: SaveBlock1 | 13732 of 15872 bytes | 2140 free |
| Save: SaveBlock2 | 2732 of 3968 | 1236 free |
| Save: SaveBlock3 | 1536 of 1624 | Research Red's own room: 674 bytes of padding + 88 |
| Save: PC boxes | 34144 of 35712 | 1.5 KB spare |
| Flags / vars | 2304 / 256 | ~570 flags and ~90 vars unused; keep FLAGS_VARS.md current |
| RAM (EWRAM, permanent) | 250156 of 262144 bytes | ~11.7 KB free; new screens should allocate while open |

## History
Finished runs, older builds and their checklists: [history/2026-10.md](history/2026-10.md).

## How to resume in a new session
1. Open Ubuntu (or double-click `E:\Workspace\Claude Hub.bat`): Work on a project > research-red > Start a fresh conversation.
2. Claude reads `CLAUDE.md` (rules) and this file (state), then `git log -5` and `gh issue list`.
3. If the test runner is missing (`~/romhacks/_tools/rr_run`), run `tools/research_red/test/setup_mgba.sh`.
