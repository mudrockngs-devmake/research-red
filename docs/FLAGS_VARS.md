# Flag / Var Registry

Log every flag and var that Research Red adds or repurposes here. Before you claim a new one, check that it's free with `grep -rn`.

## Custom vars

| Var | Purpose | Values |
|---|---|---|
| `VAR_RR_STARTER` (0x40FD) | Lab pick: 0 Bulbasaur, 1 Charmander, 2 Squirtle, 3 Chikorita, 4 Cyndaquil, 5 Totodile. `VAR_STARTER_MON` is also set by type (grass 0, water 1, fire 2) so FireRed rival scripts still work | 0-5 |
| `VAR_RR_ROUTE1_CATCHES` (0x40F2) | Wild Pokemon caught on Route 1 before the group scene (VAR_0x40FE == 4) | count |
| `VAR_RR_ROUTE1_CATCH_0`, `VAR_RR_ROUTE1_CATCH_1`, `VAR_RR_ROUTE1_CATCH_2`, `VAR_RR_ROUTE1_CATCH_3` (0x40F3-0x40F6) | Species of the first four of those catches, for Elm and Jenny's reactions | species |
| `VAR_RR_DEXNAV_SPECIES` (0x40F7, was `VAR_0x40F7`) | DexNav: the registered species (`DN_VAR_SPECIES`, `include/config/dexnav.h`) | species |
| `VAR_RR_DEXNAV_STEPS` (0x40F8, was `VAR_0x40F8`) | DexNav: step counter for hidden Pokémon (`DN_VAR_STEP_COUNTER`) | steps |
| `VAR_0x40FE` | Overall story progress (used by the debug skips) | 0 none, 1 breakfast, 2 grass, 3 lab, 4 Daisy, 5 Route 1, 6 Route 22 battle done (lunch pending), 7 lunch done (Options > Research Rules works from 4, after Daisy's house), 8 Elm and Jenny's rematch outside the Pewter Pokémon Center done (after Brock) |

## Repurposed vanilla scene vars

| Var | Research Red use |
|---|---|
| `VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB` | Lab scene state. At 10, the floor triggers at (5-7, 11) start `RR_Lab` |
| `VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE` | Daisy scene state |
| `VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN` | The old man blocks the way north (0) until the Viridian lunch sets it to 2 (path open; FireRed's catching tutorial is skipped) |
| `VAR_MAP_SCENE_ROUTE22` | Route 22 Elm & Jenny battle: 1 waiting (set by the lab), 2 done |
| `VAR_MAP_SCENE_PEWTER_CITY` | 0 before Brock (FireRed's Gym guide walks you to the Gym). 1 after Brock: demo blocker (#35) at the east exit (42, 21-23, also for 2 on old saves); the Running Shoes aide (FireRed's value 1 scene) never appears |

## Other flags and vars used by Research Red scenes

| What | Use |
|---|---|
| `FLAG_HIDE_PEWTER_CITY_GYM_GUIDE` | Cleared on entering Pewter after Brock, so the guide stays as the demo blocker (`rr_demo_blockers.inc`) |
| `FLAG_HIDE_PEWTER_CITY_RUNNING_SHOES_GUY` | Set on entering Pewter after Brock: the aide never appears (#62; Running Shoes are given from the start) |
| `FLAG_TEMP_13`, `FLAG_TEMP_14` (Pewter City) | Hide Elm and Jenny unless their rematch is pending (story 7 + after Brock) (`rr_pewter.inc`) |
| `VAR_TEMP_3` (Pallet Town) | Always 0 there: keeps Gerald's water triggers (7-10, 18) active (#36) |

## Custom flags

| Flag | Purpose |
|---|---|
| `FLAG_RR_FORCE_DOUBLES` (0x022, was `FLAG_0x022`) | Every trainer battle is a double (2v1 vs one Pokemon). Set at new game; toggle later in Elm's Research Rules |
| `FLAG_RR_FORCE_DOUBLE_WILD` (0x023, was `FLAG_0x023`) | `B_FLAG_FORCE_DOUBLE_WILD`: wild doubles. Off by default |
| `FLAG_RR_AI_VS_AI` (0x024, was `FLAG_0x024`) | `B_FLAG_AI_VS_AI_BATTLE`: set only while Elm and Jenny fight Oak in the lab |
| `FLAG_RR_IVY_SWIMSUIT` (0x025, was `FLAG_0x025`) | Ivy's swimsuit portrait in dialogue |
| `FLAG_RR_AUTO_HEAL` (0x078, was `FLAG_0x078`) | Research Rules "Heal after battles": the team is healed after every battle that isn't lost (#54). Off by default |
| `FLAG_RR_JENNY_UNIFORM` (0x026, was `FLAG_0x026`) | Jenny's police-uniform portrait (casual is the default) |
| `FLAG_RR_EXP_SHARE` (0x021, was `FLAG_0x021`) | `I_EXP_SHARE_FLAG`: Exp. Share key item on/off. Set by the research grant |
| `FLAG_RR_LEVEL_CAPS_OFF` (0x027, was `FLAG_0x027`) | Research Rules: level caps off (on by default; caps in `src/caps.c`) |
| `FLAG_0x077` (was `FLAG_RR_HARD_LEVEL_CAPS`) | Free again (the soft/hard cap choice was removed in v0.0.16). Old saves may have it set: clear it before reusing |
| `FLAG_RR_NO_ENCOUNTERS` (0x0C3, was `FLAG_0x0C3`) | `OW_FLAG_NO_ENCOUNTER`: no wild encounters while set. Only the smoke tests set it (memory poke); never set in normal play |
| `FLAG_RR_DEXNAV_GET` (0x0C0, was `FLAG_0x0C0`) | DexNav in the Start menu. Jenny gives it after the Route 22 battle, win or lose (debug skips 6+ set it) |
| `FLAG_RR_DEXNAV_SEARCHING` (0x0C1, was `FLAG_0x0C1`) | DexNav: a search is running (`DN_FLAG_SEARCHING`) |
| `FLAG_RR_DEXNAV_DETECTOR` (0x0C2, was `FLAG_0x0C2`) | DexNav: hidden Pokémon can be spotted (`DN_FLAG_DETECTOR_MODE`). Set with the DexNav |
| `FLAG_RR_NO_WHITEOUT` (0x020, was `FLAG_0x020`) | `B_FLAG_NO_WHITEOUT`: set around story battles so a loss continues the script. Cleared after each battle |

## Temp flags used by scenes

| Flag | Map | Use |
|---|---|---|
| `FLAG_TEMP_3` | Oak's Lab | Oak's Ranch Pager Pokémon (Pikachu, Farfetch'd, Abra, Machop), hidden until the lab scene is over (story < 3) (`rr_ranch_pager.inc`) |
| `FLAG_TEMP_4` | Oak's Lab | Daisy by Oak for the rematch, hidden until the Route 22 6v6 is done (story < 6) (`rr_rematch.inc`) |
| `FLAG_TEMP_11`, `FLAG_TEMP_12` | Route 1 | Elm and Jenny at the north end (hidden unless the scene is pending) |
| `FLAG_TEMP_13`-`FLAG_TEMP_15` | Viridian City | Lunch scene Elm, Jenny and Officer Jenny (shown by `addobject` during the scene) |

## Custom trainers

| Trainer | Use |
|---|---|
| `TRAINER_RR_ELM_LAB_<STARTER>` (all six) | Lab 2v2, Elm's pick from the chain |
| `TRAINER_RR_JENNY_LAB_<STARTER>` (all six) | Lab 2v2, Jenny's pick from the chain |
| `TRAINER_RR_OAK_LAB_<A>_<B>` (six pairs) | Optional Oak double, the two leftover starters |
| `TRAINER_RR_ELM_R22_<STARTER>`, `TRAINER_RR_JENNY_R22_<STARTER>` (all six each) | Route 22 6v6 double: starter + two dessert-named catches each |
| `TRAINER_RR_ELM_PEWTER_<STARTER>`, `TRAINER_RR_JENNY_PEWTER_<STARTER>` | Pewter finale double (data ready, scene not built yet) |

## Trainer slides (mid-battle lines)

`src/data/research_red/trainer_slides.h`. Research Red adds two slide types to `include/constants/trainer_slide.h`: `TRAINER_SLIDE_RR_TWO_DOWN` and `TRAINER_SLIDE_RR_FOUR_DOWN` (2 / 4 of the opposing side fainted, played once per battle).
