# Scene: rr_lab

Research Red: Oak's lab: starter picker, chain, battles, grant (PalletTown_ProfessorOaksLab, triggered from map)

### RR_Lab_EventScript_Scene

> Oak: Welcome, welcome!
> Mind the cables. And the aides.
>
> Mostly the cables.

> {PLAYER}: No, I'm sure the cables
> are across the floor on purpose.
>
> Trip-based security.
> Very modern.

> Oak: Now. Why I asked the four of
> you to Pallet Town.
>
> Three Kanto starters on top.
> Three from Johto below them,
> courtesy of Elm.

> Elm: Hand-delivered! Well. Sent
> by PC transfer. With a card!

> Jenny: It was a very nice card.
> I proofread it.

> Oak: It's on my fridge.

> Oak: While you stay in Pallet, I
> want each of you to study one.
>
> Observe it. Compare notes.
> All of it goes in the paper.

> Ivy: Regional forms?
> In starters?

> Oak: Pokémon change with where
> they live. You two proved that
> in the Orange Islands.

> {PLAYER}: Mostly Ivy proved it.
> I carried the snacks.

> Ivy: And the field notes,
> sweetie.
>
> The notes were sticky.

> {PLAYER}: The data was delicious.

> Oak: Old stories say the Johto
> three looked very different…
> in other lands.
>
> Other eras, even. Old scrolls.
> Snow where no snow should be.

> Elm: Other eras?
> Like… old-timey?

> Jenny: In my professional opinion,
> a scroll is not evidence.

> Oak: There are even whispers of
> Pokémon that change for a moment.
>
> Brilliantly. Mid-battle.
>
> …But that is a story for
> another lab.

> Oak: So! Choose a partner to
> study. {PLAYER}, you choose first.
>
> Elm went first last time.
> He chose a Bunsen burner.

> Elm: It was a very good Bunsen
> burner!

> Ivy: Is that why the east wing
> is new?

> Elm: That was one Bunsen burner!

### RR_Lab_EventScript_ChooseLoop

> Oak: Kanto on top, Johto below.
> Which one will you study?

*[dynmultichoice 1]*

### RR_Lab_EventScript_PickBulbasaur

> Oak: {STR_VAR_1}!
> Will you study this one?

*[givemon SPECIES_BULBASAUR]*

### RR_Lab_EventScript_PickCharmander

> Oak: {STR_VAR_1}!
> Will you study this one?

*[givemon SPECIES_CHARMANDER]*

### RR_Lab_EventScript_PickSquirtle

> Oak: {STR_VAR_1}!
> Will you study this one?

*[givemon SPECIES_SQUIRTLE]*

### RR_Lab_EventScript_PickChikorita

> Oak: {STR_VAR_1}!
> Will you study this one?

*[givemon SPECIES_CHIKORITA]*

### RR_Lab_EventScript_PickCyndaquil

> Oak: {STR_VAR_1}!
> Will you study this one?

*[givemon SPECIES_CYNDAQUIL]*

### RR_Lab_EventScript_PickTotodile

> Oak: {STR_VAR_1}!
> Will you study this one?

*[givemon SPECIES_TOTODILE]*

### RR_Lab_EventScript_Received

> {PLAYER} received the {STR_VAR_1}
> from Prof. Oak!

### RR_Lab_EventScript_ChainPicks

### RR_Lab_EventScript_ChainBulbasaur

*[givemon SPECIES_CHARMANDER]*

### RR_Lab_EventScript_ChainCharmander

*[givemon SPECIES_SQUIRTLE]*

### RR_Lab_EventScript_ChainSquirtle

*[givemon SPECIES_BULBASAUR]*

### RR_Lab_EventScript_ChainChikorita

*[givemon SPECIES_CYNDAQUIL]*

### RR_Lab_EventScript_ChainCyndaquil

*[givemon SPECIES_TOTODILE]*

### RR_Lab_EventScript_ChainTotodile

*[givemon SPECIES_CHIKORITA]*

### RR_Lab_EventScript_IvyLine

> Ivy: Then I'll study {STR_VAR_1}.
>
> Type matchups are half of
> research, sweetie. Just so you
> know.

> {PLAYER}: I know. I've been married
> to that fact for years.

### RR_Lab_EventScript_IvyMonJoins

> Ivy's {STR_VAR_1} joined the team!
> You and Ivy battle together.

### RR_Lab_EventScript_ElmLine

> Elm: Then I'll answer with
> {STR_VAR_1}! History repeats!

### RR_Lab_EventScript_JennyLine

> Jenny: {STR_VAR_1} for me. Someone
> has to keep you humble, dear.

### RR_Lab_EventScript_AfterPicks

> Oak: The last time this table held
> starters, a boy from down the
> road overslept.

> Elm: Ash!

> Oak: And my grandson Gary left
> before the dew had dried.
>
> Off to every Gym in Kanto.

> Ivy: They're out there somewhere
> right now, aren't they?

> Oak: Somewhere. Sending me very
> few postcards.

> Elm: …Professor. What if the best
> way to study a Pokémon's
> potential is to push it?

> {PLAYER}: Here it comes.

> Elm: The Gym Challenge!
> Real battles! Real growth!
> Real data!

> Oak: Elm, the point was to
> observe them here. Quietly.

> Jenny: “Quietly” is not a setting
> he has, Professor.

> Elm: Unless {PLAYER} is scared.

> {PLAYER}: I'm a professor. I'm
> scared of everything.
>
> Mostly paperwork.

> {PLAYER}: Ivy? Want in? I could
> use a better strategist.

> Ivy: You mean any strategist.
> Yes, sweetie. I'm in.

> Elm: Jenny? Will you join my team?
> Our team. The team that is us.

> Jenny: I married into this team,
> dear. Of course I will.

> Ivy: Jenny. Side bet:
> we carry them both?

> Jenny: Absolutely. I've seen him
> battle.

> Elm: A test, then! Right here!
> Right now! For science!

### RR_Lab_EventScript_BattleAfterBulbasaur

*[trainerbattle_two_trainers Rival Early Elm (Cyndaquil) + RR Officer Jenny (Totodile)]*

### RR_Lab_EventScript_BattleAfterCharmander

*[trainerbattle_two_trainers Rival Early Elm (Totodile) + RR Officer Jenny (Chikorita)]*

### RR_Lab_EventScript_BattleAfterSquirtle

*[trainerbattle_two_trainers Rival Early Elm (Chikorita) + RR Officer Jenny (Cyndaquil)]*

### RR_Lab_EventScript_BattleAfterChikorita

*[trainerbattle_two_trainers Rival Early Elm (Charmander) + RR Officer Jenny (Squirtle)]*

### RR_Lab_EventScript_BattleAfterCyndaquil

*[trainerbattle_two_trainers Rival Early Elm (Squirtle) + RR Officer Jenny (Bulbasaur)]*

### RR_Lab_EventScript_BattleAfterTotodile

*[trainerbattle_two_trainers Rival Early Elm (Bulbasaur) + RR Officer Jenny (Charmander)]*

### RR_Lab_EventScript_BattleResult

> Elm: Ha! History!
> Write that down, Jenny!

> Jenny: Already written, dear.
> With a diagram.

> Ivy: We let them win, sweetie.
> For morale.

> Elm: Nobody let anybody win!
> That was pure science!

> Elm: We're on a roll, Jenny! Who's
> next? Professor Oak!

### RR_Lab_EventScript_AfterBattle

> Elm: Again! Best of three!
> Best of five!

> Jenny: Best of zero.
> We're in a lab, dear.

> Elm: Fine! Then we battle someone
> else. Professor Oak!

### RR_Lab_EventScript_ElmChallengesOak

> Jenny: By protocol, dear, you ask
> before you challenge a legend.
>
> Jenny: Professor, would you battle
> us? Your two starters, our two.

> Oak: Ho ho! A challenge in my own
> lab? Gladly!
>
> But a professor looks after his
> opponents. Let me heal yours.

> Oak: {STR_VAR_1}! {STR_VAR_2}!
> Show them how it's done!

> Ivy: Sweetie, we get front row
> seats. Somebody find popcorn.

> Elm: We won! We beat Professor
> Oak! In his own lab!
>
> Jenny, call the papers! Call my
> mother! Call everyone!
>
> I'm going to frame this moment.
> I'm going to frame the floor!

> Jenny: Noted, dear. The floor goes
> in the report.

> Oak: Ho ho ho! Marvellous!
> I feel thirty years younger!

> Elm: Then let's go again! Same
> time, same place! Best of three!

> Jenny: Dear. Look at his face.

> Oak: Another round? Oh, I'd love
> to. But not with the starters.
>
> Pidgeot! Tauros! Out you come!

> Elm: His ranch team?! Jenny, those
> are fully evolved!

> Jenny: I'll start the incident
> report now, dear.

> Watch Elm and Jenny's second
> round?

> Oak: Fair's fair. A quick heal
> first. Again!

*[trainerbattle_no_intro Pkmn Prof Oak (Pidgeot, Tauros, only hardens. He asks everyone if they're from Pallet Town. */)]*

### RR_Lab_EventScript_RanchSkip

> Pidgeot's wings, Tauros's hooves,
> and Elm shouting. It's over fast.

### RR_Lab_EventScript_RanchAftermath

> Tauros snorts. Pidgeot's gust
> sends Elm's notes everywhere.

> Elm: I saw my whole thesis flash
> before my eyes. It had typos.

> Oak: Ho ho… oh dear. I got a
> little carried away there.
>
> Pidgeot, Tauros, back to the
> ranch. Thank you, old friends.

> Jenny: In my professional opinion,
> that was very fast, sir.

> Oak: Two upstarts rose up…
> The old Tauros was hungry…
>
> Please stop crying, Elm.

> Elm: It's allergies! Tauros shed
> on me!

> Ivy: Look at him, sweetie. He's
> glowing.

> Oak: Now then! {PLAYER}. Ivy.
> {STR_VAR_1} and {STR_VAR_2} are
> still raring to go.
>
> Fancy a round? No pressure. I'll
> heal your team first.

> Battle Prof. Oak?

> Ivy: Oh, I like this.

> Oak healed your Pokémon.

> Oak: Well. I suppose we can't
> really fund it now. Not after a
> showing like that.

> Elm: Then I'll fund it! Out of
> my own pocket! …Jenny, how much
> is in my pocket?

> Oak: Ho ho! A sweet thought, Elm.
> But it was funded all along.

> Oak: A student falls down…
> Old man forgot to hold back…
>
> The grant still goes through.

### RR_Lab_EventScript_OakBeaten

> Oak: Well, well. Perhaps you
> have more in you than my notes
> say.

### RR_Lab_EventScript_OakDeclined

> {PLAYER}: After the paperwork,
> Professor. Maybe.

> Oak: A battle declined…
> The chance drifts off like a leaf…
>
> We carry on still.

> Elm: But… we still get funding
> for all this, right? Right?

> Oak: Don't worry, Elm.
> I have it covered.

### RR_Lab_EventScript_TableNap

> Oak: {STR_VAR_1} and {STR_VAR_2}
> will stay with me. Someone has to
> keep this old man on his toes!

> The lab's Slowpoke shuffles over
> to the empty table… very slowly.
>
> It climbs up and falls asleep.

> Oak: Ho ho! Well, it is a very
> good table.

### RR_Lab_EventScript_Grant

> Oak: Now, the boring part.
> Paperwork!
>
> The League Research Fund approved
> your fieldwork in full!

> {PLAYER} received the
> Research Grant!

> Key items, every HM, and all 100
> TMs. TMs never run out!

> {PLAYER} received a Pokédex
> from Prof. Oak!

> {PLAYER}: Fieldwork… with no
> Poké Balls?

> Oak: Ho ho! Something better:
> Lab Supplies, on every PC!
>
> Poké Balls, medicine, held items.
> Take whatever your research needs.
>
> The fund pays. You never do!

> Ivy: A free supply cupboard? Oh,
> sweetie, we're never coming home.

> Jenny: Master Balls too, sir? In
> my professional opinion… risky.

> Oak: Not those! The Master Ball
> waits for a special occasion.
>
> As they say, a patient Slowpoke
> catches the biggest fish. Ho ho!

> Oak: And the best part: a Ranch
> Pager! Page my ranch, and one of
> my Pokémon comes to Fly, Flash,
> Teleport or Dig for you.
>
> If nobody on your team can Cut or
> Surf, mine will step in too!
>
> Anything you don't carry waits in
> your PC. Open it from the Start
> menu, anywhere.

> Ivy: So your Pidgeot is on call
> now?

> Oak: Pidgeot, Lapras, Tauros…
> They love the exercise!

> Elm: Can I get a grant?

> Oak: You got one, Elm.
> It's the same grant.

> Oak: Go and see Daisy. She has
> Town Maps. And patience.
>
> Mostly patience.

> Elm: Meet you at Daisy's!
> We'll make the bet official!

> Ivy: Daisy's house next.
> Lead the way.

*[rr_ivy_follow LOCALID_LAB_IVY]*

*[setvar VAR_0x40FE ]*

### RR_Lab_EventScript_GiveGrant

*[additem ITEM_EXP_SHARE]*

*[additem ITEM_BICYCLE]*

*[additem ITEM_SUPER_ROD]*

*[additem ITEM_ITEMFINDER]*

*[additem ITEM_VS_SEEKER]*

*[additem ITEM_BERRY_POUCH]*

*[additem ITEM_RANCH_PAGER]*

*[additem ITEM_HM01]*

*[additem ITEM_HM02]*

*[additem ITEM_HM03]*

*[additem ITEM_HM04]*

*[additem ITEM_HM05]*

*[additem ITEM_HM06]*

*[additem ITEM_HM07]*

*[additem ITEM_HM08]*

*[additem ITEM_TM_CASE]*

*[additem ITEM_TM01]*

*[additem ITEM_TM02]*

*[additem ITEM_TM03]*

*[additem ITEM_TM04]*

*[additem ITEM_TM05]*

*[additem ITEM_TM06]*

*[additem ITEM_TM07]*

*[additem ITEM_TM08]*

*[additem ITEM_TM09]*

*[additem ITEM_TM10]*

*[additem ITEM_TM11]*

*[additem ITEM_TM12]*

*[additem ITEM_TM13]*

*[additem ITEM_TM14]*

*[additem ITEM_TM15]*

*[additem ITEM_TM16]*

*[additem ITEM_TM17]*

*[additem ITEM_TM18]*

*[additem ITEM_TM19]*

*[additem ITEM_TM20]*

*[additem ITEM_TM21]*

*[additem ITEM_TM22]*

*[additem ITEM_TM23]*

*[additem ITEM_TM24]*

*[additem ITEM_TM25]*

*[additem ITEM_TM26]*

*[additem ITEM_TM27]*

*[additem ITEM_TM28]*

*[additem ITEM_TM29]*

*[additem ITEM_TM30]*

*[additem ITEM_TM31]*

*[additem ITEM_TM32]*

*[additem ITEM_TM33]*

*[additem ITEM_TM34]*

*[additem ITEM_TM35]*

*[additem ITEM_TM36]*

*[additem ITEM_TM37]*

*[additem ITEM_TM38]*

*[additem ITEM_TM39]*

*[additem ITEM_TM40]*

*[additem ITEM_TM41]*

*[additem ITEM_TM42]*

*[additem ITEM_TM43]*

*[additem ITEM_TM44]*

*[additem ITEM_TM45]*

*[additem ITEM_TM46]*

*[additem ITEM_TM47]*

*[additem ITEM_TM48]*

*[additem ITEM_TM49]*

*[additem ITEM_TM50]*

### RR_EventScript_GiveTM51To100

*[additem ITEM_TM51]*

*[additem ITEM_TM52]*

*[additem ITEM_TM53]*

*[additem ITEM_TM54]*

*[additem ITEM_TM55]*

*[additem ITEM_TM56]*

*[additem ITEM_TM57]*

*[additem ITEM_TM58]*

*[additem ITEM_TM59]*

*[additem ITEM_TM60]*

*[additem ITEM_TM61]*

*[additem ITEM_TM62]*

*[additem ITEM_TM63]*

*[additem ITEM_TM64]*

*[additem ITEM_TM65]*

*[additem ITEM_TM66]*

*[additem ITEM_TM67]*

*[additem ITEM_TM68]*

*[additem ITEM_TM69]*

*[additem ITEM_TM70]*

*[additem ITEM_TM71]*

*[additem ITEM_TM72]*

*[additem ITEM_TM73]*

*[additem ITEM_TM74]*

*[additem ITEM_TM75]*

*[additem ITEM_TM76]*

*[additem ITEM_TM77]*

*[additem ITEM_TM78]*

*[additem ITEM_TM79]*

*[additem ITEM_TM80]*

*[additem ITEM_TM81]*

*[additem ITEM_TM82]*

*[additem ITEM_TM83]*

*[additem ITEM_TM84]*

*[additem ITEM_TM85]*

*[additem ITEM_TM86]*

*[additem ITEM_TM87]*

*[additem ITEM_TM88]*

*[additem ITEM_TM89]*

*[additem ITEM_TM90]*

*[additem ITEM_TM91]*

*[additem ITEM_TM92]*

*[additem ITEM_TM93]*

*[additem ITEM_TM94]*

*[additem ITEM_TM95]*

*[additem ITEM_TM96]*

*[additem ITEM_TM97]*

*[additem ITEM_TM98]*

*[additem ITEM_TM99]*

*[additem ITEM_TM100]*

### RR_Lab_EventScript_HealElmJenny

> Oak healed Elm and Jenny's
> Pokémon.

### RR_Lab_EventScript_SetupPair

### RR_Lab_EventScript_Pair0

### RR_Lab_EventScript_Pair1

### RR_Lab_EventScript_Pair2

### RR_Lab_EventScript_Pair3

### RR_Lab_EventScript_Pair4

### RR_Lab_EventScript_Pair5

### RR_Lab_EventScript_OakStarterBattle

### RR_Lab_EventScript_OakBattle0

*[trainerbattle_no_intro Pkmn Prof Oak (Squirtle, Chikorita)]*

### RR_Lab_EventScript_OakBattle1

*[trainerbattle_no_intro Pkmn Prof Oak (Bulbasaur, Cyndaquil)]*

### RR_Lab_EventScript_OakBattle2

*[trainerbattle_no_intro Pkmn Prof Oak (Charmander, Totodile)]*

### RR_Lab_EventScript_OakBattle3

*[trainerbattle_no_intro Pkmn Prof Oak (Totodile, Bulbasaur)]*

### RR_Lab_EventScript_OakBattle4

*[trainerbattle_no_intro Pkmn Prof Oak (Chikorita, Charmander)]*

### RR_Lab_EventScript_OakBattle5

*[trainerbattle_no_intro Pkmn Prof Oak (Cyndaquil, Squirtle, starter Elm/Jenny can have from the lab chain; Elm names his catches after, desserts)]*

### RR_Lab_EventScript_StartWatch

### RR_Lab_EventScript_EndWatch

### RR_Lab_EventScript_SlowpokeOnTable

### RR_Lab_EventScript_Slowpoke

> Slowpoke: …Slooow?
>
> It's still holding the phone from
> this morning's call.

### RR_Lab_EventScript_SlowpokeNap

> Slowpoke: …Zzz… …Slow?
>
> It's fast asleep on the starter
> table. Prof. Oak doesn't mind.

### RR_Lab_EventScript_Muffin

> Muffin: Rat! Tat tat!
>
> Daisy's Rattata eyes the napping
> Slowpoke. It wants the table too.
