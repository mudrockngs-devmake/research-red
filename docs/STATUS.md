# Status

The one-page "where are we right now". **Read it at the start of every session;
update it at the end of every session** (and whenever a build goes out).
Keep it short (about 80 lines): when a run is finished, move its checklist to
[history/](history/) (one file per month) and leave a one-line result here.

_Last updated: 2026-10-31 (v0.0.23 still waiting on the owner's spot check; v0.0.24's small fixes and the moment board tool in master; owner's 2026-10-31 notes waiting for an OK)_

## Now
- **Plan to the demo:** `docs/ROADMAP.md`, "Road to the demo": v0.0.24 to v0.0.29, then v1.0. Waiting on the owner: which art the public demo uses (#167; Claude suggested the stand-ins once approved).
- **v0.0.23 is out** (tag and pre-release on GitHub, tests passed): waiting on the owner's spot check. v0.0.22 round archived (`test-archive/2026-10-30_v0.0.22_v0.0.22-spotcheck/`).
- **In master, not in a build yet:** the lunch Pokémon in two rows of six plus Brock's six on the sides (18 in all); v0.0.24's small fixes #113 #117 #119 #160. Still to do for v0.0.24: #137, #138 (held until the owner's spot check notes). (full wording: docs/history/2026-11.md, trim 2026-11-17)
- Older "Now" notes (v0.0.21 spot check, two lanes, #108, new test tools, full playthrough): history/2026-11.md (trim 2026-11-17).

## Current build
- **v0.0.23** (2026-10-30): the one big build; old saves work. (full wording: docs/history/2026-11.md, trim 2026-11-17)
- v0.0.22 details and the last archived playtest: history/2026-11.md (trim 2026-11-17).

## Waiting on the owner
- [ ] Spot check v0.0.23: `F:\ROM-DATABASE\ROMs\gba\RomhackDev\ResearchRed_v0.0.23 - what to check.txt` (full: history/2026-11.md, trim)
- [ ] Balance: the Pewter rematch with Elm and Jenny wins 8% for a team at the cap (Lv 14), Brock 100% (docs/BALANCE.md "Simulated fights"). (full: history/2026-11.md, trim)
- [ ] Try the viewer (F5: Approve / Needs work, Mark a spot, Animation, In game, History, themes) and the hub's look, Today screen, Settings and desktop shortcut (full: history/2026-11.md, trim)
- [ ] Hub "I'm done for today" (backups were 5 days old on 2026-10-24).
- [ ] The 3 questions at the end of docs/ART_BIBLE.md (outline colour, battle picture proportions, signature colours).
- [ ] Rework draft lines: Oak's grant / Lab Supplies, the lab's Oak rounds (#86, now #105), the aide's PC topic, Slowking's pages (full: history/2026-11.md, trim)
- [ ] Optional: phone emulator / real hardware (#16); workshop the report's playtest form.

## Next up
- Builds: small themed builds, spot-check rounds, a full playthrough about every third build (full rule and finished milestones: history/2026-11.md (trim 2026-11-17)). Milestones on GitHub:
  - **Next: v0.0.24 Pallet and Daisy's house (Gerald #113, the lunch rows from master). A full playthrough round is due after v0.0.23.**
  - Later milestones (shifted one on 2026-10-29): v0.0.25 text and menus, v0.0.26 levels and balance, v0.0.27 moves
    and evolution, v0.0.28 battle pictures, v0.0.29 the lab's look (GitHub milestones).
  - Then the features: scripted battle scenes (#142 vs Brock, on #105's system), #121's video
    call, #113, #114 and the rest of ROADMAP.md.
- Tasks for the owner: #129 credits/permissions list, #130 Oak's aides list.
- Tooling: a smaller patch (#17).

## Asked for, not done yet
- [ ] Ideas from claude-setup's away run 2026-11-16 (owner asked to send them here): `docs/ideas-from-claude-setup-2026-11-16.md`. (full: history/2026-11.md, trim)
- [ ] **Watch mode** (owner 2026-11-02, from claude-setup; Medium): let the tester's runner (`tools/research_red/test/rr_run.c`) optionally open a visible window so the owner can watch Claude play in real time, like mecha64's emulator window. (full: history/2026-11.md, trim)
- [ ] Lessons from mecha64 (owner asked 2026-11-02, noted from a mecha64 chat): three items, including a stray file to check before deleting. (full: history/2026-11.md, trim)
- [ ] Owner notes 2026-10-31, NOT filed yet (read back in chat, waiting for the owner's OK, then `owner-notes`): six notes (sprites, evolving, Master Balls). (full: history/2026-11.md, trim)
- [ ] (#166, v0.0.28) Owner 2026-10-30: review the whole order of Pewter scenes (including the later ones) and say what makes coherent sense without causing problems (full: history/2026-11.md, trim)
- [ ] (#161, v0.0.25) Later (owner 2026-10-30): decide when coloured text should be blue versus other colours (full: history/2026-11.md, trim)
- [ ] (#164, v0.0.26) Day Care and level caps: how it should work, for later (owner 2026-10-30). What it does now is in DESIGN section 1.
- [ ] Visual theming overhaul (owner, 2026-10-30): one darker, warmer base colour replaces all the whites, across the Start menu, attack menu, dialogue, Pokédex, party and summary, without breaking anything. (full: history/2026-11.md, trim)
- [ ] (#165, v0.0.26) Trainer AI and difficulty, not decided (owner 2026-10-30): keep the original game's difficulty; not Radical Red; cozy in look and play, challenge only if wanted, and not for every route trainer. (full: history/2026-11.md, trim)
- [ ] (#163, v0.0.26) Measure Oak and Daisy's first fight (Lv 10) and Delia's (Lv 14) in balance.py, to compare with FireRed's rival (two Pokémon at Lv 9 on Route 22). (full: history/2026-11.md, trim)
- Ticked items: history/2026-11.md (trim 2026-11-17).

## Half-finished / open threads
- Moment boards are a proof of concept to improve over time (owner 2026-10-31) (full: history/2026-11.md, trim)
- Not played since the seating change: reaching lunch by saying yes outside the Center (only the debug jump was run).
- Tool bug: `compare.py`'s automatic "vs v0.0.x" uses much older runs as the "before" for many scenes (26 of 34 flagged on 2026-10-30 were from v0.0.19), so it over-reports. (full: history/2026-11.md, trim)
- Still needs the owner's eyes: #20 (Pokédex tab glitches), #14 (Route 22 tiles), #37 (Ivy and ledges), #41 (Totodile under the cap; Cyndaquil evolving at 14 is expected).
- The League gate block (Route 22) has no automatic test; check by hand (#2).
- 13 rows of `docs/reference/kanto_anime.csv` are marked "memory" (to verify).
- Cost log, AI-vs-AI note and other older thread notes: history/2026-11.md (trim 2026-11-17).

## Moved out (pointers)
- Working setup run (one open item for a claude-setup session): history/2026-11.md (trim 2026-11-17).
- Watch closely in the v0.0.19 playtest: history/2026-11.md (trim 2026-11-17).
- Session costs not yet logged: history/2026-11.md (trim 2026-11-17).
- Capacity table: history/2026-11.md (trim 2026-11-17).

## History
Finished runs, older builds and their checklists: [history/2026-10.md](history/2026-10.md).
STATUS trim 2026-11-17 (old sections and full wording of shortened items): [history/2026-11.md](history/2026-11.md).

## How to resume in a new session
1. Open Ubuntu (or double-click `E:\Workspace\Claude Hub.bat`): Work on a project > research-red > Start a fresh conversation.
2. Claude reads `CLAUDE.md` (rules) and this file (state), then `git log -5` and `gh issue list`.
3. If the test runner is missing (`~/romhacks/_tools/rr_run`), run `tools/research_red/test/setup_mgba.sh`.
