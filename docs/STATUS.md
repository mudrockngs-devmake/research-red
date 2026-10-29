# Status

The one-page "where are we right now". **Read it at the start of every session;
update it at the end of every session** (and whenever a build goes out).
Keep it short (about 80 lines): when a run is finished, move its checklist to
[history/](history/) (one file per month) and leave a one-line result here.

_Last updated: 2026-10-29 (v0.0.21 built: Elm & Jenny on the road)_

## Now
- **v0.0.20 playtest done and archived** (`test-archive/2026-10-29_v0.0.20_playtest/`, new issues #133-#148, #106 closed).
- Two lanes: game-lane sessions use the art already in the game; Art Kit sessions improve the tools and drafts
  (#104: Pallet Town first). No Art Kit piece is approved yet (`art_gate.py status`). Exception (owner 2026-10-28,
  #125): v0.0.20 tries the cast's overworld drafts as they are, for testing only (not approved).
- Since v0.0.19 (in master, not in a build yet): tester upgrades (#18, docs/TESTING.md); debug 10's team is Lv 14.
- New test tools (2026-10-28, docs/TESTING.md): checks on every scene (dialogue/portraits and boxes closing early,
  leftovers, battles #128/#107, brightness, wild, script coverage), settings matrix (`matrix.py`), type chart engine
  test (#112), sprite tours (#125), day/night (#108, fails until fixed), wild probes (#106, fail until fixed), and the
  **full playthrough** (`playthrough.py`, power-on to the Pewter rematch, no debug skips): Quick Balls from Lab
  Supplies, one training round with catching, Rare Candies to Lv 14, beats Brock (2026-10-28). The full run from
  power-on can still black out on Route 2 to #106's leftover Squirtle (wild Pokémon reuse Oak's team) when the team
  is weak; after #106 is fixed it should go through. Not yet in it: Brock rematch tiers, Delia, Elm/Jenny rebattles
  (#127, open question on EXP/money).

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
- **v0.0.21 Elm & Jenny on the road** (2026-10-29, spot-check round): installed as `ResearchRed_local.gba`, tagged,
  pre-release on GitHub. #105 scripted lab fights (Elm & Jenny vs Oak, `src/rr_scripted_fight.c`), lab order, #139/#140
  Route 22 all Lv 10 with Cruller, #148 lunch straight after Route 22 (the old man gets up during it), #116, #143,
  debug 15 (Pewter rebattle). Smoke all pass (daynight fails until #108), settings matrix 21/21, playthrough doubles
  14/14; **singles 12/14: the bot loses to Brock's 6-vs-6 in singles** (same as v0.0.20; a balance question for
  v0.0.25). What's-new report: `E:\Workspace\romhacks\research-red\whats-new\v0.0.21\report.html` (debug jump per change).
  **Save: old saves work.**
- Last playtest archived: `test-archive/2026-10-29_v0.0.20_playtest/`.

## Waiting on the owner
- [ ] Balance: the Pewter rematch with Elm and Jenny wins 6% for a
      team at the cap (Lv 14), Brock 94% (docs/BALANCE.md "Simulated fights"). Tune it, or keep it a hard optional fight?
- [ ] Try the viewer (F5: Approve / Needs work, Mark a spot, Animation, In game, History, themes) and the hub's look,
      Today screen, Settings and desktop shortcut; say what feels off.
- [ ] Hub "I'm done for today" (backups were 5 days old on 2026-10-24).
- [ ] The 3 questions at the end of docs/ART_BIBLE.md (outline colour, battle picture proportions, signature colours).
- [ ] Rework draft lines: Oak's grant / Lab Supplies, the lab's Oak rounds (#86, now #105), the aide's PC topic,
      Slowking's pages; and the v0.0.16 placeholders (Ranch Pager tutorial, Oak's Pokémon, Rest, the Rules call, rematches, the aide).
- [ ] Optional: phone emulator / real hardware (#16); workshop the report's playtest form.

## Next up
- **Small themed builds** (owner 2026-10-28): 3-5 related issues per test build, risky systems on their own, each fix
  with a test that fails before and passes after. **Spot-check rounds** for most builds (a debug jump per change,
  the owner plays only those); a **full playthrough** about every third build (owner 2026-10-29, docs/PLAYTESTING.md).
  Milestones on GitHub (plan agreed 2026-10-29):
  - v0.0.21 Elm & Jenny on the road: built 2026-10-29, waiting for the owner's spot check.
  - v0.0.22 day and night (#108; spot check); v0.0.23 Pallet and Daisy's house (#119, #117, #136, #137, #138; then a
    full playthrough); v0.0.24 text and menus (#135, #146, #120, #115, #109, #124); v0.0.25 levels and balance
    (#111, #141, #110, #123, #112); v0.0.26 moves and evolution (#145, #144, #147); v0.0.27 battle pictures (#133,
    #134); v0.0.28 the lab's look (#118, #122, #125).
  - Then the features: scripted battle scenes (#142 vs Brock, on #105's system), #121's video
    call, #113, #114 and the rest of ROADMAP.md.
- Tasks for the owner: #129 credits/permissions list, #130 Oak's aides list.
- Tooling: a smaller patch (#17).

## Watch closely in the v0.0.19 playtest (new or reworked code, most likely to break)
- Lab Supplies: buying, a full Items pocket ("no more room"), leaving back to the PC menu, from Start > PC and a real PC.
- The lab scene after the Elm and Jenny battle, win and loss, Yes and No to both questions; Slowpoke on the table later; Muffin when Daisy visits.
- The dex banter after a catch and with SELECT: B to skip, mashing A or B, Elm/Jenny family species, switching the look (debug 13).
- Battles where you or Ivy speak (Route 22): pictures appearing and leaving.
- The move relearner, the PC item box and the Berry Pouch (pocket sizes changed), saving and loading on a new game.

## Half-finished / open threads
- Nothing half-done. All Art Kit art is drafts (tracker: docs/ART_TRACKER.md).
- In AI-vs-AI test battles the player's side plays almost at random (no partner = no AI settings); harmless so far.
  The lab's watched fights are now scripted (#105); #142 (Elm & Jenny vs Brock) should reuse that system.
- Still needs the owner's eyes: #20 (Pokédex tab glitches), #14 (Route 22 tiles), #37 (Ivy and ledges), #41 (Totodile under the cap; Cyndaquil evolving at 14 is expected).
- The League gate block (Route 22) has no automatic test; check by hand (#2).
- 13 rows of `docs/reference/kanto_anime.csv` are marked "memory" (to verify).

## Session costs not yet logged (log them in the next playtest round's NOTES.md)
- Logged up to 2026-10-27 in the v0.0.19 playtest NOTES.md. The session started 2026-10-27 21:29 ($76.27 at the v0.0.20 intake) is noted in the v0.0.20 NOTES.md; add its final figure at wrap-up.

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
