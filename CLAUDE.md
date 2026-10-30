# Research Red — project rules for Claude Code

FireRed hack on pokefirered-expansion. Repo: mudrockngs-devmake/research-red. Owner: MudrockNGS.
Documentation index: `docs/README.md`.

## Start and end of every session
- **Start:** read `docs/STATUS.md` (current build, what's waiting, what's next, anything half-finished), then `git log -5` and `gh issue list`. If `~/romhacks/_tools/rr_run` is missing, run `tools/research_red/test/setup_mgba.sh`.
- **End (or before the owner restarts):** update `docs/STATUS.md`, make sure everything is committed and pushed, and log the session cost (`session-cost`, saved automatically by the status line) in STATUS / the round's NOTES.md.

## Working rules
- Ask one question at a time. Label each task Light / Medium / Heavy before starting.
- Commit straight to master; the "Build Research Red BPS" GitHub Action makes BPS patches (and a GitHub Release when a `v*` tag is pushed). The upstream "CI" workflow is disabled on GitHub (its engine tests failed on every push and showed a red X); leave it off.
- Every build: `make firered -j6`, `python3 tools/research_red/rr_check.py`, smoke tests `python3 tools/research_red/test/smoke.py` (PASS = every scene finished and its memory checks passed; still open the contact sheets to see it looks right), then `tools/research_red/rr_install_rom.sh`. Never commit ROMs. Full flow: `docs/PLAYTESTING.md`, `docs/TESTING.md`.
- Each test build: bump the stamp (first line of `sCallScript` in `src/rr_intro_call.c`), add a `docs/CHANGELOG.md` section ending in a **Save:** line, tag `v0.0.x` and push the tag (`docs/RELEASING.md`).
- Saves: bump `RR_SAVE_VERSION` when a change breaks old saves (`docs/SAVES.md`).
- Game rules: `docs/DESIGN.md` holds every system's decisions (dated, with the issue). When the owner decides something about a system, update it there too.
- Names and terms: `docs/GLOSSARY.md` (the owner voice-types notes; fix mixed-up names against it, add new mix-ups).
- Bugs and ideas live in GitHub Issues (labels: bug, crash, story, text, art, balance, feature, tooling, decision, playtest; milestones per version). Turn the owner's notes into issues with the `owner-notes` skill (read back first, file after the OK).
- After each owner playtest round: `tools/research_red/rr_archive_test.sh <test-name>` (moves everything in the six `_emu` folders into the workspace `test-archive/` and copies the tested ROM, so `_emu` starts empty), then fill in its `NOTES.md` (issue numbers, `/cost`). mGBA screenshots (F12) land in `/mnt/e/Workspace/romhacks/_emu/screenshots/`; testers' notes files from the what's-new report go in `_emu/notes/`.
- Versioning: v0.0.x per test build; milestones v0.1 (Pallet), v0.2 (Viridian/Route 22/lunch), v0.3 (Route 2/Forest), v0.4 -> v1.0 (Pewter/Brock/finale).
- Text: proper case, max 34 characters per line, story dialogue uses `NPC_TEXT_COLOR_NEUTRAL`, speakers start lines with `Name: ` (portraits). Voices: `docs/STYLE_GUIDE.md`.
- Code: clear comments, `RR_` prefix for custom labels/files, `Research Red` comment on every edit to an engine file (`docs/UPSTREAM.md`).
- Keep `docs/FLAGS_VARS.md` (registry) and `CREDITS.md` (with the Permission column) up to date with every change.
- Generated files (encounters, banter, portraits table, player docs): edit the source and rerun the tool; `rr_check.py` fails if they're stale (`docs/DEV_GUIDE.md`). It also fails on ALL-CAPS game text: `tools/research_red/proper_case.py` fixes it (#11).
- The animatic (`docs/animatic/`) is the scene spec; `docs/scenes/*.md` scene specs from the owner override it.
- Showing the owner pictures or scenes: the live viewer pops up in their Windows browser (`tools/research_red/viewer/viewer.py show FILE --title "question"`, or `board MAP_X` for drag-and-drop staging; replies via `viewer.py answers`). Use it whenever a question is about how something looks or where people stand.
- Debug menu stays ON for test builds (Start > Debug > Scripts, named skips 1-12, 15 and 16, 13 switches the banter look, 14 your battle picture). Turn OFF before any public release.

## Skills (`.claude/skills/`)
- `owner-notes` (notes to issues, read back first), `test-build` (a v0.0.x build start to finish), `playtest-intake` (archive + notes to issues), `new-scene` (scene conventions + its smoke test). Follow them; improve them when a step turns out wrong.

## Where things are
- Story scenes: one file each in `data/scripts/research_red/rr_*.inc`; FireRed maps keep only their hooks. Layout and conventions: `docs/DEV_GUIDE.md`.
- Ivy is an NPC follower (FNPC). Cutscenes: `rr_ivy_to_scene LOCALID_X_IVY` at the start, `rr_ivy_follow LOCALID_X_IVY` at the end (scene Ivy objects need a hide flag). Helpers: `asm/macros/event.inc` (Research Red section), `src/research_red.c`, `data/scripts/research_red/rr_ivy.inc`.
- Story progress var `VAR_0x40FE`: 0 none, 1 breakfast, 2 grass hook, 3 lab, 4 Daisy, 5 Route 1, 6 Route 22 done, 7 lunch done, 8 Pewter rematch done (after Brock).
- Intro video call: `src/rr_intro_call.c` (graphics: `tools/research_red/build_call_ui.py`). Debug skips: `data/scripts/debug.inc`. Art: `graphics/research_red/`.
- Dialogue portraits: `src/rr_portrait.c`; speakers in `graphics/research_red/portraits.txt` -> `tools/research_red/build_portraits.py`. Sprite palettes for interface art go through `RR_LoadInterfaceSpritePalette` (slots 12-15).
- Wild Pokemon: `data/research_red/encounters/*.txt` -> `tools/research_red/build_encounters.py`. Field Guide: `src/rr_field_guide.c`. Research Rules: `src/rr_rules.c`.
- Pokedex banter: `data/research_red/dex_banter.txt` -> `tools/research_red/build_dex_banter.py`; `src/rr_dex_banter.c`. Mid-battle lines: `src/data/research_red/trainer_slides.h`.
- Player docs and balance: `tools/research_red/export_player_docs.py` -> `docs/player/`, `docs/BALANCE.md` (copy on E: in `player-docs/`). Story export: `tools/research_red/export_story.py --sync` -> `docs/story/` (incl. `FULL_SCRIPT.md`).
- **Two lanes** (owner, 2026-10-24): the game lane (mechanics, story, cutscenes, test builds) uses the art already in the game: FireRed sprites, stand-ins, Porymap, small recolours; small map pieces the owner asks for are fine with the existing tools (log them as `game:` pieces). The Art Kit lane makes new art off to the side; **only pieces the owner approved are in the game** (tracker + gate `graphics/research_red/art_kit.toml`, `tools/research_red/art/art_gate.py`, page `docs/ART_TRACKER.md`, rules `docs/ART_BIBLE.md`). Never set a piece to approved without the owner's OK. First art pass: the Pallet Town vertical slice (#104). Keep the two lanes in separate sessions.
- Overworld sprites (cast, Delia, gym leaders, E4, players): recipes in `graphics/research_red/overworld/recipes.toml` -> `tools/research_red/art/ow_maker.py install`; map pieces `tile_kit.py`, new buildings `building.py` (all in `docs/ART_TOOLS.md`). Cast art: `tools/research_red/build_cast_art.py` (hand-drawn files in `cast_handmade/` win, `docs/ARTIST_GUIDE.md`); sync a copy to `E:\Workspace\romhacks\research-red\art\generated\cast\`.
- Plans: `docs/CHECKLIST.md` + GitHub milestones. Story: `docs/STORY_BIBLE.md`. Assets: `docs/ASSETS.md`. Sources/credits rules: `docs/RESOURCES.md`.
