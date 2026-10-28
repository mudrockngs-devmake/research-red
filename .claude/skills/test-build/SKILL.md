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
5. **Optional, for risky builds**: `python3 tools/research_red/test/monkey.py` (random buttons) and, after
   team or AI changes, `python3 tools/research_red/test/balance.py` (BALANCE.md "Simulated fights").
6. **Docs**: FLAGS_VARS.md, CREDITS.md, DESIGN.md (built items move from Planned to In the game).
7. **Install**: `tools/research_red/rr_install_rom.sh`. **What's new**: `python3 tools/research_red/rr_whats_new.py v0.0.x`.
8. **Commit, push, tag**: `git tag v0.0.x && git push origin v0.0.x` (GitHub makes the patch, the pre-release,
   and runs every smoke scene). Check the GitHub run passes (`gh run list --limit 3`).
9. **STATUS**: the new current build, what to test, the save note.
10. **Hand-off to the owner**: what changed, what to test (the what's-new report path), the save note.
