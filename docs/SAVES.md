# Saves

## The policy
- **Until v0.1 (the first public demo milestone):** saves may break between test builds. Every changelog entry says which (`Save: old saves work` or `Save: start a new game`).
- **From v0.1 on:** a public release never breaks saves silently. If a change would, the save version goes up, the changelog says so in bold, and the release notes say it first.
- Save states (emulator snapshots) are always tied to the exact build. Use them for testing, not to carry progress across builds.

## The save version
New games store `RR_SAVE_VERSION` (`include/rr_save_version.h`) in the save. When a save from a different version is continued, the game says once, back on the map:

> This save is from an older test build of Research Red. It may still work, but if anything looks wrong, start a new game.

Then it marks the save as current so the message doesn't repeat.

**Bump `RR_SAVE_VERSION`** (and add a row below) when a change:
- changes the size or order of anything in the save blocks (`include/global.h`: SaveBlock1/2/3), e.g. a bigger bag pocket
- reuses a flag or var that an old save could already have set to something else
- changes story progress values (`VAR_0x40FE` stages) in a way old saves can't follow

Adding new flags/vars from the free list, new scenes, text, art, encounters or trainers does **not** need a bump.

## Space left in the save
| Block | Free | Notes |
|---|---|---|
| SaveBlock1 | 8 bytes | 15864 of 15872 (test/save.c checks the size: `make check`) |
| SaveBlock3 | its `unused[]` padding | shrink the padding when adding a field (as `rrSaveVersion` did) |

## Versions
| Save version | Since | What changed |
|---|---|---|
| 0 | before v0.0.14 | no version stored (these saves get the message once) |
| 1 | v0.0.14 | version added; layout as of v0.0.10 (bigger Items pocket, follower data in SaveBlock3) |
| 2 | v0.0.19 | pocket sizes rebalanced in the same space: PC item storage 64, Items pocket 30, Berry Pouch 19 (#96); new games only |
| 3 | v0.0.19 | first-time item descriptions: SaveBlock3 gains the "seen this item" flags (110 bytes, taken from its padding; same total size); new games only |
| 4 | v0.0.19 | grant rework: supplies come from Lab Supplies (a free shop on the PC), so the pockets go back to FireRed's sizes (PC item storage 30, Items pocket 42, Berry Pouch 43); and the unused link / e-Reader / Mystery Gift data is freed (`include/config/save.h` `FREE_*`), see "Room left" below; new games only |

## Room left (measured 2026-10-21, v0.0.19)
`include/config/save.h` frees the parts of the save Research Red never uses:
Mystery Event scripts, Mystery Gift, union room chat, link battle records, the
Enigma Berry, Battle Tower e-Reader trainers, Pokémon Jump records and record
mixing's hall records. Trainer Tower and Match Call (VS Seeker rematches) stay
for now.

| Save block | Before | After | Free |
|---|---|---|---|
| SaveBlock1 | 15872 of 15872 | 13732 of 15872 | 2140 bytes |
| SaveBlock2 | 3968 of 3968 | 2732 of 3968 | 1236 bytes |
| SaveBlock3 (Research Red's own) | 950 used | unchanged | 674 bytes spare |

Permanent RAM (EWRAM): 250156 of 262144 bytes (11.7 KB free; the move
relearner's TM names moved into the screen's own allocation, -1.4 KB). New
screens should allocate while they're open rather than keep permanent buffers.
Measure with `GameMemory.const("sizeof(struct SaveBlock1)")` (tools/research_red/test)
and `arm-none-eabi-size -A pokefirered.elf` (`.ewram.sbss`).

