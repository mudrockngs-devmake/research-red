# Writing style guide

Research Red is warm, quick and slapstick: grown-ups who love each other and
their work, being a little ridiculous about it. Jokes come from character,
not meanness. Every scene should leave the player smiling.

## The cast's voices
How each of them talked in the early anime, with sample lines: [reference/VOICES_EARLY_ANIME.md](reference/VOICES_EARLY_ANIME.md).

| Who | Voice | Tics and habits | Avoid |
|---|---|---|---|
| **The player** (Maple / Cedar / Sumac) | Wry, warm, absurd: ironic mock-disbelief, dry understatement, puns ([sheet](characters/PLAYER.md)) | Undersells themself ("I carried the snacks"), adores Ivy, teases Elm gently | Being cruel; long speeches |
| **Ivy** (the player's wife) | Warm, sharp, confident; the foremost expert on regional forms ([sheet](characters/IVY.md)) | Calls the player **"sweetie"**; corrects the record with a smile; fun facts | Being only the straight man; she gets jokes too |
| **Elm** (rival, Johto professor) | Loud, eager, competitive, secretly sentimental | "For science!", grand declarations, instant excuses ("It's allergies! Daisy has a Persian!"), dessert names for his Pokémon | Being genuinely dumb; he's brilliant and over-excited |
| **Jenny** (Elm's wife, off-duty detective) | Deadpan, loving, procedural ([sheet](characters/JENNY.md)) | **"In my professional opinion"**, "by protocol", calls Elm **"dear"**, mid-sentence reality checks | Being cold; she's on his side, always |
| **Officer Jenny** (her cousin) | Brisk, on duty | Family-reunion jokes (every Jenny looks the same) | |
| **Prof. Oak** | Grandfatherly, theatrical about joy, forgetful about small things | Poems at odd moments, "Ho ho!", Pokémon proverbs ("the early Pidgey catches the Weedle"), even tutorials are him talking to you ([sheet](characters/OAK.md)) | Cruelty, anger, sounding like a rules screen |
| **Delia** | Motherly, feeds everyone | Pancakes, "Ash used to forget breakfast every time" | |
| **Daisy** | Calm, practical, unimpressed by Elm | Corrects "Persian" to "Rattata" | |
| **Mimey** | Mime | "Mime mime!" Bows. Makes pancakes. | Real words |
| **Muffin** (Daisy's Rattata) | Rattata | "Rat! Tat tat!", extremely pleased with herself | |
| **Slowpoke** (the lab's) | Slowpoke | "…Slow?", and it's always a beat late | |

Running gags to keep alive: Gerald the Tentacool, "Muffin is a Rattata", the
bet (first team to eight badges; losers do Mimey's dishes), Jenny's reports,
Elm's allergies, Oak's haiku.

## Text rules (checked by `rr_check.py`)
- **34 characters per line** at most, two lines per box. `\n` for the second line, `\l` to scroll a third, `\p` for a new box.
- **Proper case** everywhere: Elm, Pokémon, Poké Ball. No ALL-CAPS names (FireRed's leftovers are being cleaned up, issue #11).
- Lines a character speaks start with **`Name: `** (`Elm: `, `Ivy: `, `{PLAYER}: `). That's what makes the portrait and name plate appear. Narration has no prefix.
- One idea per box. If a line needs three boxes, it's probably two lines for two characters.
- Story dialogue uses `textcolor NPC_TEXT_COLOR_NEUTRAL`.
- Use `…` (one character), not `...`. Curly quotes “ ” in text, not `"`.

## Where text lives
| Text | File |
|---|---|
| Scenes | `data/scripts/research_red/rr_*.inc` |
| The intro call | `src/rr_intro_call.c` (`sCallScript`) |
| Mid-battle lines | `src/data/research_red/trainer_slides.h` |
| Pokédex banter | `data/research_red/dex_banter.txt` (wraps automatically) |
| Ivy's follower chatter | `data/scripts/research_red/rr_ivy.inc` |

To read it all in story order: `docs/story/FULL_SCRIPT.md` (regenerate with `python3 tools/research_red/export_story.py --sync`; the copy on E: is in `story/`).
