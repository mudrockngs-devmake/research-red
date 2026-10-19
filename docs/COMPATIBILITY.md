# Compatibility

Where each build has been tried. Update the table when you test somewhere new
(or ask Claude to). What to check on a new emulator or device:
- the game boots and the intro call plays
- **saving**: save in game, close the emulator, reopen, Continue works (the game uses 128 KB flash)
- **the clock**: day/night tint and night encounters follow the time (the game reads a real-time clock; some emulators need it turned on)
- doubles battles run at full speed (no slowdown with four Pokémon on screen)
- sound plays normally

| Build | Emulator / device | Boots | Saves | Clock | Speed | Notes |
|---|---|---|---|---|---|---|
| v0.0.13 | mGBA 0.10.5 (Windows) | yes | (save states used) | yes | fine | the owner's main setup |
| v0.0.13 | mGBA 0.10.5 headless (smoke tests) | yes | yes | yes | n/a | in-game save and Continue checked |
| | Android (e.g. Pizza Boy GBA, John GBA) | | | | | not tried yet |
| | iOS (Delta) | | | | | not tried yet |
| | Real GBA via flash cart | | | | | not tried yet; flash carts need the right save type set |

## Known differences between emulators
- **VBA / VBA-M** are inaccurate with expansion-based hacks; mGBA is recommended.
- Some phone emulators don't provide the real-time clock by default: day/night and night-only Pokémon won't change.
