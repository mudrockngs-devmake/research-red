# Research Red — project rules for Claude Code

FireRed hack on pokefirered-expansion. Repo: mudrockngs-devmake/research-red. Owner: MudrockNGS.

## Working rules
- Ask one question at a time. Label each task Light / Medium / Heavy before starting.
- Commit straight to master; the "Build Research Red BPS" GitHub Action makes BPS patches. Ignore the "CI" workflow.
- Build locally first: `make firered -j6`, then `tools/research_red/rr_install_rom.sh` (installs `ResearchRed_local.gba` and keeps a versioned copy in the workspace `builds/`). Never commit ROMs.
- After each owner playtest round: `tools/research_red/rr_archive_test.sh <test-name>` (moves screenshots, copies saves/states and the tested ROM into the workspace `test-archive/`), then fill in its `NOTES.md` from the owner's notes. mGBA screenshots (F12) land in `/mnt/e/Workspace/romhacks/_emu/screenshots/`; read them there.
- Add each build's changes to `docs/CHANGELOG.md` (used for GitHub release notes).
- Versioning: v0.0.x per test build; milestones v0.1 (Pallet), v0.2 (Viridian/Route 22/lunch), v0.3 (Route 2/Forest), v0.4 -> v1.0 (Pewter/Brock/finale). Stamp: `sText_Call_Hello` in `src/oak_speech.c`.
- Text: proper case everywhere (no ALL-CAPS names), max 34 characters per line, story dialogue uses `NPC_TEXT_COLOR_NEUTRAL`.
- Code: clear comments, `RR_` prefix for custom labels/files, keep custom work easy to find for upstream merges.
- Keep `docs/FLAGS_VARS.md` (registry) and `CREDITS.md` up to date with every change.
- The animatic (`docs/animatic/`) is the scene spec; `docs/scenes/*.md` scene specs from the owner override it.
- Debug menu stays ON for test builds (Start > Debug > Scripts 1-5 = skips). Turn OFF before any public release.

## Where things are
- Story scripts: `RR_*` labels. Lab scene lives in `data/maps/PalletTown/scripts.inc` (started by lab floor triggers at (5-7,11) when the lab scene var == 10).
- Ivy is an NPC follower (FNPC). Cutscenes: `rr_ivy_to_scene LOCALID_X_IVY` at the start, `rr_ivy_follow LOCALID_X_IVY` at the end (scene Ivy objects need a hide flag). Helpers: `asm/macros/event.inc` (Research Red section), `src/research_red.c`, `data/scripts/rr_ivy.inc`.
- Story progress var `VAR_0x40FE`: 0 none, 1 breakfast, 2 grass hook, 3 lab, 4 Daisy, 5 Route 1.
- Intro video call: `src/oak_speech.c`. Debug skips: `data/scripts/debug.inc`. Art: `graphics/research_red/`.
- Plans: `docs/CHECKLIST.md`. Story: `docs/STORY_BIBLE.md`. Assets: `docs/ASSETS.md`. Sources/credits rules: `docs/RESOURCES.md`. Chat-to-code workflow: `docs/CHAT_TO_CODE.md`.

## Known bugs (v0.0.7)
1. Lab battle freezes: `RR_Lab` uses `trainerbattle_earlyrival` with `RIVAL_BATTLE_TUTORIAL` while the player has 2 Pokemon.
2. Route 1 camera stays offset: the pan goes past the map top into the Viridian connection.
3. Elm battles use FireRed rival teams and Gary's battle picture.
