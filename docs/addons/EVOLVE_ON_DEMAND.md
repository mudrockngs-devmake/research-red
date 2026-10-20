# Add-on: evolve on demand (#50)

Like Scarlet/Violet and Legends: Arceus: Pokémon never evolve on their own after
a battle or a Rare Candy. When one could evolve, its party menu shows **EVOLVE**;
the player picks the moment (B in the evolution scene still cancels).

Written for pokefirered-expansion; the same spots exist in pokeemerald-expansion.

## Switch
`P_RR_EVOLVE_ON_DEMAND` in `include/config/pokemon.h` (TRUE = on, FALSE = the
engine's normal behaviour).

## What changes (search for "Research Red (#50)")
| File | Change |
|---|---|
| `src/pokemon.c`, `GetEvolutionTargetSpecies` | In `EVO_MODE_BATTLE_ONLY` (the after-battle level-up check), `EVO_LEVEL` no longer counts. `EVO_LEVEL_BATTLE_ONLY` (Tandemaus) and `EVO_BATTLE_END` methods still evolve after battle, because their conditions only exist during it |
| `src/party_menu.c`, Rare Candy and `PartyMenuTryEvolution` | Skip the automatic evolution |
| `src/party_menu.c`, `SetPartyMonFieldSelectionActions` | Adds `CURSOR_OPTION_RR_EVOLVE` after SUMMARY when `GetEvolutionTargetSpecies(mon, EVO_MODE_NORMAL, ..., CHECK_EVO)` finds a target; field moves are capped so the list stays within 8 entries |
| `src/party_menu.c`, `CursorCB_RR_Evolve` / `CB2_RR_EvolveFromPartyMenu` | Closes the party menu, re-checks, applies `DO_EVO` (removes items the method uses) and starts `BeginEvolutionScene`, returning to the party menu's exit callback |
| `src/data/party_menu.h` | The new cursor option (`"EVOLVE"`) |

## How "ready" works
Nothing is saved. Readiness is checked when the menu opens, with every extra
condition of the evolution (friendship, time of day, held item, known move,
location...), so a night evolution shows EVOLVE at night. Evolution stones work
as before (use the stone).

## Tested
Smoke test `evolve` (a Lv 30 Squirtle: EVOLVE in the menu, the scene, the
Pokédex banter after) and every story battle (nobody evolves afterwards).
