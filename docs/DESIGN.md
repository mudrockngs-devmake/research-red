# Game design: the rules of Research Red

How each system works, and why: the owner's decisions in one place. The story lives in
[STORY_BIBLE.md](STORY_BIBLE.md), scenes in [animatic/](animatic/) and [scenes/](scenes/), the work
in GitHub Issues. This page holds the rules those all follow.

How to use it:
- Each section says **In the game** (true today, in master) and **Planned** (decided, not built yet),
  with the date and the issue where it was decided.
- When the owner decides something about a system, update its section here (with the date) and
  link the issue. When it's built, move it from Planned to In the game.
- If the code and this page disagree, one of them is a bug: ask the owner which.

## 1. Level caps and EXP
**In the game** (#41, owner 2026-10-19/20; src/caps.c)
- Hard caps by **how many** badges you have (not which ones): 0 badges Lv 14, 1: 21, 2: 24, 3: 29, 4: 43, 5: 43, 6: 47, 7: 50, 8 badges before the Elite Four: 63, after: none.
- At the cap: no EXP, Rare Candies stop. Below it: **catch-up EXP** (more the further below).
- Optional: Options > Research Rules > Level caps off (no cap, no catch-up bonus). Overlevelling is the player's choice; Elm jokes about it when setting up the bet.
- Rematches against family and friends give 1.5x EXP and no money (#53). **Bug** (#110): it also reaches the opening lab fights.

**Planned**
- **Opening fights give normal EXP** (you and Ivy vs Elm and Jenny, Oak's first fight); a double battle giving a fair amount is fine. The big boost ("a very large amount of EXP", an easy place to grind to the cap) is **only for the replayable Oak and Daisy fights** after Route 22 (owner 2026-10-28, #110).
- Double battles give EXP faster in general: after #110, look again at wild levels and the caps (owner 2026-10-28).
- No early EXP from Oak fights in the opening: Oak and Daisy's scaling fights only start after Route 22 (owner 2026-10-28, #105).
- Watch: a player who grinds reaches Lv 14 before Route 22, where Elm and Jenny are around Lv 8-9. No separate cap for Route 22 for now; keep an eye on it in playtests (owner 2026-10-28).

## 2. Battle format
**In the game** (Research Rules, src/rr_rules.c; first shown at Daisy's, then in Options)
- **Forced doubles**: you and Ivy fight together, so trainer battles are doubles (a trainer with one Pokémon fights 2-vs-1). Research Rules can switch trainers to singles, and wild Pokémon to doubles.
- Battle style shift / set; **Heal after battles** on/off (#54).
- Held items and Berries come back after battle (#58).

**Planned** (owner 2026-10-28, #128)
- Research Rules battle style (owner 2026-10-28, #128): **doubles** makes every trainer battle a double; **singles** goes back to the game's own choice for each battle (FireRed's built-in doubles stay doubles). Curated fights keep their **6-vs-6 teams** either way (gym leaders, Elm and Jenny, Oak and Daisy, Delia, rivals from Route 22 on); with singles they are 6-vs-6 single battles.
- Paired opponents (Elm and Jenny, Oak and Daisy) are **one team of six under one AI** (owner 2026-10-28, #107). The partner is there to see and to talk, like Ivy on your side; in singles they still stand there.
  - Done as **one ordinary trainer** (Elm's / Oak's entry, the partner's Pokémon taking turns in the party so in doubles they lead together) with **one picture of the pair**, the same in singles and doubles (owner 2026-10-28: head to thighs, the partner a step behind; `tools/research_red/build_pair_art.py`). Like FireRed's own Twins: no special engine code; the Research Rules decide singles or doubles.
  - Their battle lines work like FireRed's: the pair picture slides in on their side and the line starts with who's talking ("Jenny: ..."); no portraits for the other side (owner 2026-10-28). You and Ivy keep your portraits over your Pokémon (#95).
  - Their names in battle text: "Elm&Jenny" and "Oak&Daisy" (10 letters at most; owner 2026-10-28).
  - Your side matches (owner 2026-10-28): you in front and Ivy a step behind, head to thighs, by default; Debug > Scripts > 14 switches back to the #7 picture (you and Ivy turned to each other) for comparing.
  - The 2-down / 4-down lines count the whole team, and the last-Pokémon line plays when they have nobody left to send in (in doubles: when their last two are out).
- Two-person teams are **one team of 6 under one AI** (Elm's; Oak's for Oak and Daisy, #107); the partner is there for dialogue and pictures.
- Effectiveness in battle: the engine's own markers, not our colours (#112); maybe colours later as their own tested change.

## 3. Progression and gates
**Planned** (owner 2026-10-28, #52)
- Pallet: you can't leave for Route 1 before Daisy's house (the bet). Stepping onto Route 1 early: Elm shouts from Daisy's window, you step back, Ivy nudges you to be good sports (owner 2026-10-28, #119).
- Diglett's Cave route stays as it is in the demo (owner 2026-10-28, #130).
- The gyms stay in FireRed's order. The demo's blocks (Diglett's Cave, Pewter's east exit, Gerald at sea, the League gate) become real story gates in the full game, written into the story instead of developer notes. The middle stretch (Erika, Koga, Sabrina) can keep some freedom: check FireRed's real gating there.
- **No overworld scaling**: wild Pokémon and route trainers keep their levels (no Crystal Clear-style respec). Only gym leaders adapt (section 5).

## 4. The lab, Oak, and Elm and Jenny's early fights
**In the game** (v0.0.19, #86)
- You and Ivy fight Elm and Jenny (double, Lv 5). Win or lose, Elm and Jenny then fight Oak's leftover starters (watched, a real AI battle), Oak's Lv 50 Pidgeot and Tauros flatten them (optional to watch), then Oak offers you a fight.

**Planned** (owner 2026-10-28, #105)
- **Watched fights become scripted cutscenes** with fun dialogue, not AI battles: Elm and Jenny vs Oak, the gag rematch, and Elm and Jenny vs the gym leaders.
- **You win against Elm and Jenny**: Oak asks right away if you want to battle him (a proper fight).
- **You lose**: no Oak fight then. Optionally watch Elm and Jenny vs Oak (cutscene), then the gag where Oak flattens them (cutscene, optional).
- After the cutscenes, win or lose, Oak offers **one optional fight** for some EXP, available **any time until Route 22** ("if you'd like to fight me with these, any time is fine"). His team: two Lv 5 Pokémon.
- Levels on screen add up: Elm and Jenny fight you at Lv 5, Oak's leftover starters at Lv 6 vs Oak's Lv 5s, the gag at Lv 7 vs Oak's Lv 50s.
- After Route 22, Oak and Daisy become the optional scaling EXP fights (section 5). The first one is **Lv 10**; the scaling per badge is decided after Brock, probably between Brock's and Misty's caps (owner 2026-10-28, #105).
- Watched fights show both Elm and Jenny (#105). The lab's Slowpoke is Oak's newest addition (introduced in the video call), never battles and stays a Slowpoke (#121): Nidoran♂ takes its place on Oak's rematch team, a Nidoking from 4 badges (owner 2026-10-28); later it sleeps in a little bed on the table (#122).
- Idea (#126): Ivy asks if you'd recommend her starter; then Elm and Jenny pick so nobody has the same Pokémon.
- All six starters are 50% female (#123). The Viridian old man makes sure the Route 22 point is always reached first.

## 5. Rematches
**In the game** (v0.0.16-19)
- Brock's rematch with a tier menu (1 badge: Lv 21) and a post-game tier (#52); Oak + Daisy in the lab, growing with every badge (#53); Delia's double at home (#70); Elm and Jenny's rematch outside the Pewter Pokémon Center (#10).
- Rematch teams build on each character's existing team; separate tiers for 7 and 8 badges; a post-game tier with regional forms and exchange-program visitors (Mimey and Muffin stay home); Megas in the post-game tier (owner 2026-10-20, #52).

**Planned** (owner 2026-10-28, #52)
- Each gym leader has **a preset team per badge count**, from their own gym up to 8 badges, plus a post-Elite Four team. Brock: his gym team (0 badges), then 1 to 8, then post-game. Misty from 2 badges up, and so on.
- **Erika, Koga and Sabrina**: their tier follows how many badges you have, so their order doesn't matter.
- **Giovanni**: his gym fight, a way to refight him (depends on the story), and a post-game team.
- **Only gym leaders get a tier menu.** Elm and Jenny, Delia, Oak and Daisy grow with badges with no tier choice (owner 2026-10-28).
- **Elm and Jenny's rematches** are replayable and give **normal EXP and normal prize money**, like any trainer (owner 2026-10-28, #127; this replaces the earlier "no EXP, no money"). They shouldn't be the place to earn money: that comes from gym leader rebattles and the rest of the game. Still open: an item each time.
- **The rematch menu**: pick any tier from the leader's original team up to your current badge count (like Crystal Clear).
- Open: the Pewter rematch with Elm and Jenny wins 6% for a team at the cap (Lv 14) (BALANCE.md "Simulated fights"): tune it, or keep it a hard optional fight?

## 6. Items, TMs and the grant
**In the game** (#57, #90, #96-#100)
- Oak's research grant is TMs/HMs and key items; **Lab Supplies** is a free shop on every PC (no Master Ball); 150 Poké Balls, consumables, 6 battle items; Change a Ball (#97).
- 100 TMs, all from the grant. 19 TMs that repeated a free tutor move were swapped for others (#90; the old moves stay in the relearner's Tutor Moves).

## 7. Evolution and moves
**In the game** (#50, #51, #65, #89)
- Evolve on demand from the party menu; after battle, level-up evolutions wait for the party menu (#50).
- Trade evolutions happen by level (37), no Linking Cord (#65).
- Pre-evolution moves are relearnable at any level (#51); the move relearner keeps the list open after teaching (#89).

## 8. Field moves: the Ranch Pager
**In the game** (#29): Oak's Pokémon do the field moves (Fly with Pidgeot, Dig with Diglett, Flash, Teleport with Abra...), called with the Ranch Pager.

## 9. Time and rest
**Bug** (#108): the light doesn't follow the clock; there should be only two light levels, day and night (owner 2026-10-28).

**In the game** (#56, #27): Rest (at home with Delia, or with Nurse Joy) moves the clock to night or morning; wild tables change at night.

## 10. Pokédex and banter
**In the game** (#5, #4, #20, #77): a banter cutscene over the new Pokédex entry after a catch (portraits by default, full-body behind debug 13), SELECT replays it; the Area tab; the 151 in order, then Johto by national number.
