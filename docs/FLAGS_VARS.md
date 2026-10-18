# Flag / Var Registry

Log every flag and var that Research Red adds or repurposes here. Before you claim a new one, check that it's free with `grep -rn`.

## Custom vars

| Var | Purpose | Values |
|---|---|---|
| `VAR_RR_STARTER` (0x40FD) | Lab pick: 0 Bulbasaur, 1 Charmander, 2 Squirtle, 3 Chikorita, 4 Cyndaquil, 5 Totodile. `VAR_STARTER_MON` is also set by type (grass 0, water 1, fire 2) so FireRed rival scripts still work | 0-5 |
| `VAR_0x40FE` | Overall story progress (used by the debug skips) | 0 none, 1 breakfast, 2 grass, 3 lab, 4 Daisy, 5 Route 1 |

## Repurposed vanilla scene vars

| Var | Research Red use |
|---|---|
| `VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB` | Lab scene state. At 10, the floor triggers at (5-7, 11) start `RR_Lab` |
| `VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE` | Daisy scene state |
| `VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN` | The tipsy old man blocks the way north until the Route 22 Elm battle |
| `VAR_MAP_SCENE_ROUTE22` | Route 22 Elm battle state |

## Custom flags

| Flag | Purpose |
|---|---|
| `FLAG_RR_EXP_SHARE` (0x021, was `FLAG_0x021`) | `I_EXP_SHARE_FLAG`: Exp. Share key item on/off. Set by the research grant |
| `FLAG_RR_NO_WHITEOUT` (0x020, was `FLAG_0x020`) | `B_FLAG_NO_WHITEOUT`: set around story battles so a loss continues the script. Cleared after each battle |

## Custom trainers

| Trainer | Use |
|---|---|
| `TRAINER_RR_ELM_LAB_<STARTER>` (all six) | Lab 2v2, Elm's pick from the chain |
| `TRAINER_RR_JENNY_LAB_<STARTER>` (all six) | Lab 2v2, Jenny's pick from the chain |
| `TRAINER_RR_OAK_LAB_<A>_<B>` (six pairs) | Optional Oak double, the two leftover starters |
