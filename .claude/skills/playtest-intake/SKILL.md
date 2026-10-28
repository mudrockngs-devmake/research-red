---
name: playtest-intake
description: Take in a finished playtest round: archive the emulator folders, turn the owner's (and testers') notes and screenshots into issues, fill in NOTES.md. Use when the owner says they played, finished a playtest, has playtest notes or screenshots, or asks to archive the test.
---

# Playtest intake

Full flow: docs/PLAYTESTING.md. Nothing in `_emu` is ever deleted: archiving moves it.

1. **Look first**: list `/mnt/e/Workspace/romhacks/_emu/` (screenshots, notes, saves, states) and read
   the notes files and the owner's message. Look at every screenshot.
2. **Archive**: `tools/research_red/rr_archive_test.sh <short-name>` (moves all six `_emu` folders into
   `test-archive/<date>_<version>_<name>/` and copies the tested ROM).
3. **Notes to issues**: use the `owner-notes` skill (read back numbered, file after the OK). Bugs get
   `bug` / `crash` + `playtest`, with the screenshot or save state named; ideas `feature` / `story`.
   Check existing issues first.
4. **Reproduce what you can**: a bug with a clear path gets a smoke scenario that shows it (it fails now,
   passes after the fix). A crash: try `monkey.py --seed ...` near that spot, or a save state.
5. **NOTES.md** in the archive folder: issue numbers, the session costs from STATUS's "not yet logged" list
   (then clear that list), anything the owner said about the build as a whole.
6. **STATUS**: last playtest archived, what's waiting, next build plan. Commit and push.
