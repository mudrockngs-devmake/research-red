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

## Interface look (owner, 2026-10-30)
One light tan paper colour and dark brown ink replace the whites on every box and menu: dialogue, battle, Start menu, party, summary, Pokédex tabs, naming, shops, Bag, TMs, Options, Save. The Pokédex entry page is one step lighter (cream). Blues stay as the contrast colour. Options values are blue with a brown edge; DexNav's black bar and red headings are brown. Night tints the world only: boxes, menus, portraits and name plates keep their daytime colours. Left alone on purpose: type badges, HP bars, Pokémon, the map, PC wallpapers, the Trainer Card (its colour is its rank). Name plates are tan with curved corners, and long names get a longer plate. Source: `tools/research_red/ui_theme.py`.

## 1. Level caps and EXP
**In the game** (#41, owner 2026-10-19/20; src/caps.c)
- Hard caps by **how many** badges you have (not which ones): 0 badges Lv 14, 1: 21, 2: 24, 3: 29, 4: 43, 5: 43, 6: 47, 7: 50, 8 badges before the Elite Four: 63, after: none.
- At the cap: no EXP and no message about it (a team message only if someone benched really gains), Rare Candies stop. Below it: **catch-up EXP** (more the further below).
- Optional: Options > Research Rules > Level caps off (no cap, no catch-up bonus). Overlevelling is the player's choice; Elm jokes about it when setting up the bet.
- Caps off, then back on (checked 2026-10-30, smoke `cap_above`): a Pokémon above the cap keeps its level and gains no EXP or Rare Candy levels until the cap passes it; the others still level up to the cap.
- Day Care and caps (read in the code 2026-10-30, src/daycare.c): steps keep counting while a Pokémon is there; levels shown and given stop at the current cap, worked out when you collect, so a new badge or caps off releases the saved steps at once. Collecting at the cap loses the extra steps. To look at later (owner 2026-10-30).
- Rematches against family and friends give 1.5x EXP and no money (#53). Checked 2026-10-29 (#110): that boost is only on the Oak and Daisy, Delia and Brock rematches, not the lab fights; the lab's extra EXP is catch-up EXP (double while 5+ levels under the cap), kept as it is for now (owner 2026-10-29).

**Planned**
- **Opening fights give normal EXP** (you and Ivy vs Elm and Jenny, Oak's first fight); a double battle giving a fair amount is fine. The big boost ("a very large amount of EXP", an easy place to grind to the cap) is **only for the replayable Oak and Daisy fights** after Route 22 (owner 2026-10-28, #110).
- Double battles give EXP faster in general: after #110, look again at wild levels and the caps (owner 2026-10-28).
- No early EXP from Oak fights in the opening: Oak and Daisy's scaling fights only start after Route 22 (owner 2026-10-28, #105).
- Route 22: Elm and Jenny's Pokémon are all Lv 10 (owner 2026-10-29, #140). A player who grinds can reach Lv 14 before it; no separate cap for Route 22 for now (owner 2026-10-28).

## 2. Battle format
**In the game** (Research Rules, src/rr_rules.c; first shown at Daisy's, then in Options)
- **Forced doubles**: you and Ivy fight together, so trainer battles are doubles (a trainer with one Pokémon fights 2-vs-1). Research Rules can switch trainers to singles, and wild Pokémon to doubles.
- Battle style shift / set; **Heal after battles** on/off (#54).
- Held items and Berries come back after battle (#58).
- **Since v0.0.20:**
  - Research Rules battle style (owner 2026-10-28, #128): **doubles** makes every trainer battle a double; **singles** goes back to the game's own choice for each battle (FireRed's built-in doubles stay doubles). Curated fights keep their **6-vs-6 teams** either way (gym leaders, Elm and Jenny, Oak and Daisy, Delia, rivals from Route 22 on); with singles they are 6-vs-6 single battles.
  - Paired opponents (Elm and Jenny, Oak and Daisy) are **one team of six under one AI** (owner 2026-10-28, #107). The partner is there to see and to talk, like Ivy on your side; in singles they still stand there.
    - Done as **one ordinary trainer** (Elm's / Oak's entry, the partner's Pokémon taking turns in the party so in doubles they lead together) with **one picture of the pair**, the same in singles and doubles (owner 2026-10-28: head to thighs, the partner a step behind; `tools/research_red/build_pair_art.py`). Like FireRed's own Twins: no special engine code; the Research Rules decide singles or doubles.
    - Their battle lines work like FireRed's: the pair picture slides in on their side and the line starts with who's talking ("Jenny: ..."); no portraits for the other side (owner 2026-10-28). You and Ivy keep your portraits over your Pokémon (#95).
    - Their teams: on Route 22 and in Pewter Jenny brings her Growlithe Cruller and Macaron sits out (owner 2026-10-29, #139).
  - Their names in battle text: "Elm&Jenny" and "Oak&Daisy" (10 letters at most; owner 2026-10-28).
    - Your side matches (owner 2026-10-28): you in front and Ivy a step behind, head to thighs, by default; Debug > Scripts > 14 switches back to the #7 picture (you and Ivy turned to each other) for comparing.
    - The 2-down / 4-down lines count the whole team, and the last-Pokémon line plays when they have nobody left to send in (in doubles: when their last two are out).

**Planned**
- Effectiveness in battle: the engine's own markers, not our colours (#112); maybe colours later as their own tested change.

## 3. Progression and gates
**Planned** (owner 2026-10-28, #52)
- Pallet: you can't leave for Route 1 before Daisy's house (the bet). Stepping onto Route 1 early: Elm shouts from Daisy's window, you step back, Ivy nudges you to be good sports (owner 2026-10-28, #119).
- Diglett's Cave route stays as it is in the demo (owner 2026-10-28, #130).
- Viridian lunch (owner 2026-10-29, #148): straight after the Route 22 fight, as part of the same scene (a fade to the picnic), so there is no gap and nothing to block; the old man's lunch gate goes. The lunch stays one standalone scene, so an invite you can play any time before Viridian Forest stays an easy switch.
- The gyms stay in FireRed's order. The demo's blocks (Diglett's Cave, Pewter's east exit, Gerald at sea, the League gate) become real story gates in the full game, written into the story instead of developer notes. The middle stretch (Erika, Koga, Sabrina) can keep some freedom: check FireRed's real gating there.
- **No overworld scaling**: wild Pokémon and route trainers keep their levels (no Crystal Clear-style respec). Only gym leaders adapt (section 5).

## 4. The lab, Oak, and Elm and Jenny's early fights
**In the game** (v0.0.23, #105)
- You and Ivy fight Elm and Jenny (double, Lv 5).
- **You win:** Oak asks right away if you want to battle his two leftover starters (Lv 5). After that (Yes or No) Elm wants his turn: watching Elm and Jenny vs Oak, and the gag after it, is optional (owner 2026-10-29).
- **You lose:** Elm and Jenny gloat; you can watch them take on Oak (Elm and Jenny Lv 7 vs his starters Lv 5, then Lv 9 for the gag: the levels a player reaches in the same fights, owner 2026-10-30), then the gag where Oak's Lv 50 Pidgeot and Tauros flatten them (Lv 12), each optional. Then Oak offers you his fight.
- Oak says his takes-me-back line after you win or lose, which leads into the options (owner 2026-10-30, draft wording). Oak is fired up in the fights, no "go easy"; Ivy points it out at the start.
- Tracy (#158, owner 2026-10-30): the systems aide, a quiet nod to Tracey Sketchit, named only "Tracy", using an existing sprite. The Poké Ball change is the first entry in Tracy's menu. The original scientist stays with his own sprite and conversations.
- Starter table (#118) and Slowpoke's bed (#122, once the story reaches the lab): made with the existing map tools.
- **Start menu** (#124, owner 2026-10-30): Pokédex, Pokémon, Field Guide, DexNav, PC, Bag, player name, Save, Option, Debug (test builds), Exit. Wider so Field Guide fits; nine rows show, longer lists scroll with arrows and wrap at the ends.
- Saying No to Oak's fight: it waits until Route 22 (talk to Oak in the lab); one fight only (FLAG_RR_OAK_FIRST_FIGHT). After Route 22 Oak and Daisy are the rematch.
- Healing before the lab fights is quiet: Oak's lines say he heals, no fanfare (owner 2026-10-29). You turn to face Oak when he asks (#150); his Poké Balls leave the table when his starters battle (#151). Before everyone leaves, Oak points you to his aide for the project, the grant and the gadgets (#152).
- **The watched fights are scripted** (owner 2026-10-29, #105; `src/rr_scripted_fight.c`): a real battle screen that plays out the same way every time, with lines between turns, **seen from Elm and Jenny's side**: their Pokémon on the near side with their pair picture, Oak as the opponent, his picture sliding in when he talks; Elm's, Jenny's and Ivy's lines show their portraits. Each turn's moves come from a table; nothing misses or lands a critical hit and nobody switches (like FireRed's Poké Dude battles). The text moves on by itself. It opens with "Elm and Jenny challenge Prof. Oak!" and gives no prize money. Fight 1 lasts about four turns (Oak's side only uses plain hits); the gag is instant (Pidgeot and Tauros hit straight away).

**Planned** (owner 2026-10-28, #105)
- **Watched fights become scripted cutscenes** with fun dialogue, not AI battles: Elm and Jenny vs the gym leaders next (#142), on the same scripted-fight system as the lab.
- After Route 22, Oak and Daisy become the optional scaling EXP fights (section 5). The first one is **Lv 10**; the scaling per badge is decided after Brock, probably between Brock's and Misty's caps (owner 2026-10-28, #105).
- The lab's Slowpoke is Oak's newest addition (introduced in the video call), never battles and stays a Slowpoke (#121): Nidoran♂ takes its place on Oak's rematch team, a Nidoking from 4 badges (owner 2026-10-28); later it sleeps in a little bed on the table (#122).
- Idea (#126): Ivy asks if you'd recommend her starter; then Elm and Jenny pick so nobody has the same Pokémon.
- All six starters are 50% female (#123). The lunch follows straight after Route 22 (#148), so the Viridian old man's block is gone by the time you can walk north.

## 5. Rematches
**In the game** (v0.0.16-19)
- Brock's rematch with a tier menu (1 badge: Lv 21) and a post-game tier (#52); Oak + Daisy in the lab, growing with every badge (#53); Delia's double at home (#70); Elm and Jenny's rematch outside the Pewter Pokémon Center (#10).
- Rematch teams build on each character's existing team; separate tiers for 7 and 8 badges; a post-game tier with regional forms and exchange-program visitors (Mimey and Muffin stay home); Megas in the post-game tier (owner 2026-10-20, #52).

**Planned** (owner 2026-10-28, #52)
- Each gym leader has **a preset team per badge count**, from their own gym up to 8 badges, plus a post-Elite Four team. Brock: his gym team (0 badges), then 1 to 8, then post-game. Misty from 2 badges up, and so on.
- **Erika, Koga and Sabrina**: their tier follows how many badges you have, so their order doesn't matter.
- **Giovanni**: his gym fight, a way to refight him (depends on the story), and a post-game team.
- **Rebattle levels** (owner 2026-10-30): Oak + Daisy halfway between the previous and current cap, rounded down (Lv 10 before Brock, then 17, 22, 26, 36, 43, 45, 48, 56), a place to train; Delia at the current cap (14, 21, 24, 29, 43, 43, 47, 50, 63), whole team; both whole teams at one level (owner 2026-10-30: optional fights, not barriers). Elm and Jenny's Pewter fight and rebattles are Lv 14, the same as you at the cap, since they haven't beaten Brock either (owner 2026-10-30, replaces cap + 1). Their later levels are planned story fight by story fight, by how often you meet them, like Lv 10 at Route 22. Teams evolve at their stages; movesets planned later. Post-game levels stay as they are for now.
- **Only gym leaders get a tier menu.** Elm and Jenny, Delia, Oak and Daisy grow with badges with no tier choice (owner 2026-10-28).
- Pokédex (owner 2026-10-30): the expansion's Forms and Evolution pages stay. Forms leaves out Mega and Hisuian entries; Evolution keeps Hisuian rows blotted out (??? with a ? icon, "Unlocks later", can't be opened), a nod to unlocking that information later (`RR_IsDexSpoilerSpecies`). Alolan, Galarian and Gigantamax still show.
- Elm and Jenny wait by the Pewter Pokémon Center whenever you're in Pewter after their first fight there, for a rebattle any time (owner 2026-10-29, #143); when the story moves to the next town, they move with it. In the game since v0.0.21; since v0.0.22 they stand left of the Center facing down.
- Pewter Gym, your first visit (owner 2026-10-30): Elm and Jenny stand by the door until you've beaten Brock; his nine brothers and sisters stand behind him (FireRed kid sprites); Liam says he's cooking for them, Brock's talk and the stew come from the animatic with a portrait.
- Dex banter never mentions Hisui or Hisuian forms, or Mega Evolution (owner 2026-10-30): Hisuian talk would spoil the starters' secret forms, Typhlosion above all, and the player learns about Mega Evolution later.
- Oak's lab talk (owner 2026-10-30): Oak now raises regional forms himself before Ivy's question, the "snow where no snow should be" line is gone, and the old scrolls became old sketches and ancient field notes from a professor Oak can't name (a nod to Laventon: he fumbles it as Lavender, Lavington, and nobody can correct him; Hisui itself is never named). These lines override the animatic.
- Why 2 vs 1 is allowed (owner 2026-10-30): Oak called ahead: don't go easy on them, but the League is interested in their work, so he told every Gym Leader to keep things reasonably fair. Brock says so before his fight, and the player jokes about it.
- Brock's gift (owner 2026-10-30): a Lv 12 Alolan Geodude he bred from Ivy's notes, instead of TM39, which is already in the TM Case from the lab. A one-off from his breeding; regional forms as a wider system stay a late-game payoff.
- The Brock fight is a scripted fight on the battle screen (#142, owner 2026-10-30): Brock's six, two at a time; Elm and Jenny keep their starters out and clear a pair each turn, with the scene's jokes between rounds. Its badge and Gym-door scenes stay on the map.
- After Brock, Elm asks if you want to watch them fight Brock (#142). Saying no: next time you're in Pewter they're at the Center, say once that it was a great fight and they learned a lot, then just offer rebattles. Their rebattle team after Brock: all Lv 14 (owner 2026-10-30, #143).
- **Elm and Jenny's rematches** are replayable and give **normal EXP and normal prize money**, like any trainer (owner 2026-10-28, #127; this replaces the earlier "no EXP, no money"). They shouldn't be the place to earn money: that comes from gym leader rebattles and the rest of the game. Still open: an item each time.
- **The rematch menu**: pick any tier from the leader's original team up to your current badge count (like Crystal Clear).

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
**In the game** (#56, #27): Rest (at home with Delia, or with Nurse Joy) moves the clock to night or morning; wild tables change at night.

**Light** (#108, owner 2026-10-28 and 2026-10-30): only two light levels. Day is fully bright (6 AM to 8 PM); night (8 PM to 6 AM) is a lighter blue tint than upstream, option B of the samples, so you can still find your way. The change is instant, no fade.

## 10. Pokédex and banter
**In the game** (#5, #4, #20, #77): a banter cutscene over the new Pokédex entry after a catch (portraits by default, full-body behind debug 13), SELECT replays it; the Area tab; the 151 in order, then Johto by national number.
