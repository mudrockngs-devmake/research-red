# Versions and releases

## Version numbers
- **v0.0.x**: test builds, one per playtest round.
- **v0.1** Pallet · **v0.2** Viridian, Route 22, lunch · **v0.3** Route 2, Viridian Forest · **v0.4 → v1.0** Pewter, Brock, finale (the demo).
- The version is stamped in the game: the first line of the intro call, `sCallScript` in `src/rr_intro_call.c`. `rr_install_rom.sh` reads it from there.

## Every test build
1. Bump the stamp in `src/rr_intro_call.c`.
2. Add a `## v0.0.x` section to [CHANGELOG.md](CHANGELOG.md). Players read it, so write it for them. End it with a **Save:** line: `Save: old saves work` or `Save: start a new game` (see [SAVES.md](SAVES.md)).
3. Build, `rr_check.py`, smoke tests ([TESTING.md](TESTING.md)), install, commit, push.
4. Tag it and push the tag:
   ```
   git tag -a v0.0.14 -m "Research Red v0.0.14"
   git push origin v0.0.14
   ```
   The "Build Research Red BPS" GitHub Action builds the patch and publishes a **GitHub Release** (marked pre-release) with the `.bps` file, that version's changelog section (`tools/research_red/release_notes.py`) and patching instructions.
5. Update [STATUS.md](STATUS.md).

Pushes to `master` without a tag still build the patch as a downloadable Action artifact (kept 14 days), like before.

## The patch
- Made against **Pokémon FireRed (USA) Rev 1** (v1.1), SHA1 `dd5945db9b930750cb39d00c84da8571feebf417`. The Action builds that ROM from pret's disassembly and checks the hash, so no ROM is ever stored anywhere.
- Never commit or upload a ROM. Releases carry only the `.bps`.

## Before a public release (v0.1 and later)
- [ ] Debug menu **off**: `DEBUG_OVERWORLD_MENU` in `include/config/debug.h` (the skips in `data/scripts/debug.inc` go with it)
- [ ] Version stamp says the release version (no "test build")
- [ ] Changelog entry written for players, with the Save line in bold if saves break
- [ ] "Known issues" section in the release notes (open `bug` issues on that milestone)
- [ ] [CREDITS.md](../CREDITS.md): every third-party asset has permission confirmed (the Permission column), anything unconfirmed replaced
- [ ] [COMPATIBILITY.md](COMPATIBILITY.md) updated for this build
- [ ] Smoke tests pass; a full manual playthrough of the new content
- [ ] Player docs regenerated (`export_player_docs.py`) and linked from the release
- [ ] Release un-marked as pre-release on GitHub
