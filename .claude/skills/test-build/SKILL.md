---
name: test-build
description: Make a Research Red test build for the owner (v0.0.x): stamp, changelog, build, checks, smoke tests, install, what's-new report, tag. Use when the owner asks for a new build, a test build, "put it in the game", "let me play it", or a run ends with "build it".
---

# Test build (v0.0.x)

Full rules: docs/RELEASING.md, docs/PLAYTESTING.md, docs/TESTING.md. Stop and ask before
the install and the tag if the owner hasn't already OK'd them for this build.

1. **Stamp**: bump the version on the first line of `sCallScript` in `src/rr_intro_call.c`.
2. **Changelog**: a `## v0.0.x` section in `docs/CHANGELOG.md`, written for players, ending in a
   **Save:** line (`old saves work` / `start a new game`). Breaks old saves? bump `RR_SAVE_VERSION` (docs/SAVES.md).
3. **Build and check** (low priority): `nice make firered -j6`, `python3 tools/research_red/rr_check.py` (0 problems).
4. **Smoke tests**: `python3 tools/research_red/test/smoke.py`. All PASS (old WARNs are listed in STATUS).
   Read the "Pictures compared" list at the end of REPORT.md: every changed scene should be one this build
   meant to change (open `compare.html`). Open the contact sheets of the scenes this build touched.
5. **Play the checklist yourself** (owner 2026-10-29): every item the testers will check, the way they would (same
   debug jumps and choices; `cl_*` scenes in smoke.py), looking at the screenshots. Fix what you find (rerun),
   correct the what-to-check list if it's wrong, and write your answers (ticked or not, what you saw) as
   `ResearchRed_v0.0.x - Claude's answers.txt` next to the handheld checklist, so the owner can compare.
6. **Optional, for risky builds**: `python3 tools/research_red/test/monkey.py` (random buttons) and, after
   team or AI changes, `python3 tools/research_red/test/balance.py` (BALANCE.md "Simulated fights").
7. **Docs**: FLAGS_VARS.md, CREDITS.md, DESIGN.md (built items move from Planned to In the game).
6b. **Debug jumps** (spot-check builds, docs/PLAYTESTING.md "Two kinds of round"): one Start > Debug jump per change,
   right before its scene with a fitting team and story state; the what's-new checklist's "try" lines name them.
   Also run `python3 tools/research_red/test/playthrough.py` and `matrix.py` (the bot's full run replaces the owner's).
8. **Install**: `tools/research_red/rr_install_rom.sh`. **What's new**: `python3 tools/research_red/rr_whats_new.py v0.0.x`.
   **Handheld copy** (owner 2026-10-29, for now): copy the ROM to `/mnt/f/ROM-DATABASE/ROMs/gba/RomhackDev/ResearchRed_v0.0.x.gba` with `ResearchRed_v0.0.x - what to check.txt` (plain text: how to start, a debug jump per change with [ ] boxes, known issues). Never overwrite other files there.
9. **Commit, push, tag**: `git tag v0.0.x && git push origin v0.0.x` (GitHub makes the patch, the pre-release,
   and runs every smoke scene). Check the GitHub run passes (`gh run list --limit 3`).
10. **STATUS**: the new current build, what to test, the save note.
11. **Hand-off to the owner**: what changed, what to test (the what's-new report path), the save note, and the
   kind of round: spot check (debug jumps) or full playthrough (about every third build).
