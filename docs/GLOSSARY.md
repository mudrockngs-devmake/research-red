# Glossary

Names and words used in Research Red, so notes, issues and code all mean the same thing.
The owner often voice-types notes: the "heard as" column lists the mix-ups seen so far.
Claude reads notes against this page and fixes the names before filing anything (see the
owner-notes skill). Add a row whenever a new mix-up turns up.

## People
| Name | Who | Heard as |
|---|---|---|
| Player | An adult professor: Maple, Cedar or Sequoia (male or female). Ivy's spouse | |
| Ivy | The player's wife, regional-forms researcher; follows you (FNPC) and fights beside you | Ivey |
| Elm | Johto professor, the player's best-friend rival | Elmo, Helm, Elm's (for "Elm and") |
| Jenny | Elm's wife, New Bark Town's Officer Jenny (off duty); Growlithe Cruller | Jenny's, Genie |
| Officer Jenny | The local police officer (Viridian lunch); a different person from Jenny | |
| Prof. Oak | Mentor, a grandfather to everyone; rematches that grow with your badges | Oaks, oak |
| Daisy | Oak's granddaughter, hosts Elm and Jenny, the bet's referee; Muffin the Rattata | |
| Delia | Ash's mom, host in Pallet; Mimey; optional double battle | Delia's, Dahlia |
| Brock, Misty, Lt. Surge, Erika, Koga, Sabrina, Blaine, Giovanni | The Kanto Gym Leaders, in FireRed's order | Brockwood (Brock would), Erica, blame / Blain (Blaine), Gio |
| Gerald | The Lapras who blocks surfing from Pallet to Cinnabar (demo) | |
| Slowking | Writes the notices at New Game | |

## Pokémon with names
| Name | Who |
|---|---|
| Mimey | Delia's Mr. Mime |
| Muffin | Daisy's Rattata (Elm insists it's a Persian) |
| Cruller | Jenny's Growlithe |
| Matcha, Cinnamon, Toffee, Pistachio, Biscotti, Churro, Macaron, Licorice, Pudding... | Elm and Jenny's team: dessert and drink names (docs/player/TRAINERS.md) |

## Places
| Name | Heard as |
|---|---|
| Pallet Town | Palatown, Pallettown, palette |
| Viridian City, Viridian Forest | Veridian, Verdian |
| Pewter City | Pewder, Peter |
| Cinnabar Island | Cinnebar |
| Route 1, Route 2, Route 22 | root one, root 22 |
| Oak's lab, Daisy's house (Rival's house on the map) | |

## Game words (Research Red)
| Word | Means | Heard as |
|---|---|---|
| badge | Gym badge; the level cap follows how many you have | batch, badges |
| level cap | The highest level your team can reach for your badge count (src/caps.c); optional (Research Rules) | | 
| catch-up EXP | Bonus EXP below the cap | |
| tier | A rematch difficulty: one preset team per badge count (#52) | tear |
| rematch | Fighting a character again (gym leaders, Oak and Daisy, Elm and Jenny, Delia) | |
| forced doubles | You and Ivy fight together: trainer battles are doubles (FLAG_RR_FORCE_DOUBLES) | |
| test mode | Debug 9: the AI fights your battles with a Lv 30 team (FLAG_RR_AI_VS_AI) | |
| debug skip | Start > Debug > Scripts 1-13: jump to a point in the story | |
| cutscene fight | A scripted battle you watch (no real AI); planned for Elm and Jenny's watched fights (#105) | cut scene |
| the bet | Elm and the player's running bet, refereed by Daisy | |
| banter | The Pokédex conversation cutscene after a catch (#5) | |
| Ranch Pager | Calls Oak's Pokémon for field moves (Fly, Dig, Flash...) (#29) | ranch pager, launch pager |
| Lab Supplies | The grant's free shop on every PC (#96) | |
| Field Guide | Notes on Pokémon and places (Start menu) | |
| Research Rules | Options that change the rules (level caps, healing...) (src/rr_rules.c) | |
| FNPC | A follower NPC (Ivy) | |
| story step | `VAR_0x40FE`, how far the story is (CLAUDE.md) | |
| smoke tests, monkey test, battle sims | The automated tests (docs/TESTING.md) | |
| Art Kit | The side lane making new art; only approved pieces go in the game | art kit |

## Outside names
| Name | Means | Heard as |
|---|---|---|
| Claude Code | This assistant in the terminal | cloud code, claw code, Claude code |
| Claude Hub (`hub`) | The owner's menu that opens with Ubuntu | the hub |
| ROM hack | A modified game | ROMAC, rom hack |
| pokefirered-expansion | The engine Research Red is built on | expansion |
| Crystal Clear | Another ROM hack (open world, rematch tiers) | |
| Pokémon Origins | The anime special following the games' story | Origins anime |
| Porymap | The map editor | pory map |
| mGBA | The emulator | m GBA |
