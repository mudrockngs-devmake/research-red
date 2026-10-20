# Scene: rr_lab

Research Red: Oak's lab: starter picker, chain, battles, grant (PalletTown_ProfessorOaksLab, triggered from map)

### RR_Lab_EventScript_Scene

> Oak: Welcome, welcome!
> Mind the cables. And the aides.
>
> Mostly the cables.

> Oak: Now. Why I asked the four of
> you to Pallet Town.
>
> Three Kanto starters on top.
> Three from Johto below them,
> courtesy of Elm.

> Elm: Hand-delivered! Well. Mailed.
> With a very nice note.

> Jenny: It was a very nice note.
> I proofread it.

> Oak: While you stay in Pallet, I
> want each of you to study one.
>
> Observe it. Compare notes.
> Specifically, notes on forms.

> Ivy: Regional forms?
> In starters?

> Oak: Pokémon change with where
> they live. You two proved that
> in the Orange Islands.

> {PLAYER}: Mostly Ivy proved it.
> I carried the snacks.

> Ivy: He carried the snacks and
> the field notes.
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
> study. {PLAYER}, my most reliable
> student. You choose first.

> Elm: “Reliable” is code
> for “slow.”

> Ivy: “Reliable” is code for
> “never set the lab on fire.”

> Elm: That was one Bunsen burner!

### RR_Lab_EventScript_ChooseLoop

> Oak: Kanto on top, Johto below.
> Which one will you study?

*[dynmultichoice 10]*

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

> Jenny: In my professional opinion,
> “quietly” is not a setting he has.

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

> Jenny: Professionally?
> Absolutely.

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

### RR_Lab_EventScript_AfterBattle

> Elm: Again! Best of three!
> Best of five!

> Jenny: Best of zero.
> We're in a lab, dear.

### RR_Lab_EventScript_OakSteps

> Oak: Hmm. If you're going to do
> this unorthodox thing, do it
> properly.

### RR_Lab_EventScript_OakAsk

> Oak: {PLAYER}. Ivy. The two of
> you, against me. Two on two.
>
> {STR_VAR_1} and {STR_VAR_2} have
> been restless anyway.

> Battle Prof. Oak?

> Ivy: Oh, I like this.

### RR_Lab_EventScript_OakPair0

### RR_Lab_EventScript_OakBattle0

*[trainerbattle_no_intro Pkmn Prof Oak (Squirtle, Chikorita)]*

### RR_Lab_EventScript_OakPair1

### RR_Lab_EventScript_OakBattle1

*[trainerbattle_no_intro Pkmn Prof Oak (Bulbasaur, Cyndaquil)]*

### RR_Lab_EventScript_OakPair2

### RR_Lab_EventScript_OakBattle2

*[trainerbattle_no_intro Pkmn Prof Oak (Charmander, Totodile)]*

### RR_Lab_EventScript_OakPair3

### RR_Lab_EventScript_OakBattle3

*[trainerbattle_no_intro Pkmn Prof Oak (Totodile, Bulbasaur)]*

### RR_Lab_EventScript_OakPair4

### RR_Lab_EventScript_OakBattle4

*[trainerbattle_no_intro Pkmn Prof Oak (Chikorita, Charmander)]*

### RR_Lab_EventScript_OakPair5

### RR_Lab_EventScript_OakBattle5

*[trainerbattle_no_intro Pkmn Prof Oak (Cyndaquil, Squirtle, starter Elm/Jenny can have from the lab chain; Elm names his catches after, desserts)]*

### RR_Lab_EventScript_OakResult

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

### RR_Lab_EventScript_Grant

> Oak: Now, the boring part.
> Paperwork!
>
> The League Research Fund approved
> your fieldwork in full!

> {PLAYER} received the
> Research Grant supplies!

> Every Poké Ball, Master Balls too,
> medicine, held items, key items,
> and every TM and HM went in!

> {PLAYER} received a Pokédex
> from Prof. Oak!

> {PLAYER}: That is… a lot
> of Poké Balls.

> Ivy: We're doing research,
> sweetie. Not opening a store.

> Jenny: I will need receipts.
> For every ball.

> Elm: Can I get a grant?

> Oak: You got one, Elm.
> It's the same grant.

> Oak: Go and see Daisy. She has
> Town Maps. And patience.
>
> Mostly patience.

> Elm: Meet you at Daisy's!
> We'll make the bet official!

> Ivy: Daisy's house next, sweetie.
> Lead the way.

*[rr_ivy_follow LOCALID_LAB_IVY]*

*[setvar VAR_0x40FE ]*

### RR_Lab_EventScript_GiveGrant

*[additem ITEM_POKE_BALL]*

*[additem ITEM_GREAT_BALL]*

*[additem ITEM_ULTRA_BALL]*

*[additem ITEM_PREMIER_BALL]*

*[additem ITEM_HEAL_BALL]*

*[additem ITEM_NET_BALL]*

*[additem ITEM_NEST_BALL]*

*[additem ITEM_DIVE_BALL]*

*[additem ITEM_DUSK_BALL]*

*[additem ITEM_TIMER_BALL]*

*[additem ITEM_QUICK_BALL]*

*[additem ITEM_REPEAT_BALL]*

*[additem ITEM_LUXURY_BALL]*

*[additem ITEM_MASTER_BALL]*

*[additem ITEM_FAST_BALL]*

*[additem ITEM_LEVEL_BALL]*

*[additem ITEM_LURE_BALL]*

*[additem ITEM_HEAVY_BALL]*

*[additem ITEM_LOVE_BALL]*

*[additem ITEM_FRIEND_BALL]*

*[additem ITEM_MOON_BALL]*

*[additem ITEM_SPORT_BALL]*

*[additem ITEM_DREAM_BALL]*

*[additem ITEM_BEAST_BALL]*

*[additem ITEM_SUPER_POTION]*

*[additem ITEM_HYPER_POTION]*

*[additem ITEM_FULL_RESTORE]*

*[additem ITEM_REVIVE]*

*[additem ITEM_MAX_REVIVE]*

*[additem ITEM_FULL_HEAL]*

*[additem ITEM_ETHER]*

*[additem ITEM_MAX_ETHER]*

*[additem ITEM_MAX_REPEL]*

*[additem ITEM_ESCAPE_ROPE]*

*[additem ITEM_MAX_POTION]*

*[additem ITEM_ELIXIR]*

*[additem ITEM_MAX_ELIXIR]*

*[additem ITEM_PP_UP]*

*[additem ITEM_X_ATTACK]*

*[additem ITEM_X_DEFENSE]*

*[additem ITEM_X_SP_ATK]*

*[additem ITEM_X_SP_DEF]*

*[additem ITEM_X_SPEED]*

*[additem ITEM_X_ACCURACY]*

*[additem ITEM_DIRE_HIT]*

*[additem ITEM_GUARD_SPEC]*

*[additem ITEM_LEFTOVERS]*

*[additem ITEM_LIFE_ORB]*

*[additem ITEM_CHOICE_BAND]*

*[additem ITEM_CHOICE_SPECS]*

*[additem ITEM_CHOICE_SCARF]*

*[additem ITEM_FOCUS_SASH]*

*[additem ITEM_ASSAULT_VEST]*

*[additem ITEM_ROCKY_HELMET]*

*[additem ITEM_EVIOLITE]*

*[additem ITEM_EXPERT_BELT]*

*[additem ITEM_LIGHT_CLAY]*

*[additem ITEM_WEAKNESS_POLICY]*

*[additem ITEM_AIR_BALLOON]*

*[additem ITEM_HEAVY_DUTY_BOOTS]*

*[additem ITEM_AMULET_COIN]*

*[additem ITEM_LUCKY_EGG]*

*[additem ITEM_SOOTHE_BELL]*

*[additem ITEM_QUICK_CLAW]*

*[additem ITEM_SCOPE_LENS]*

*[additem ITEM_KINGS_ROCK]*

*[additem ITEM_BLACK_SLUDGE]*

*[additem ITEM_FLAME_ORB]*

*[additem ITEM_TOXIC_ORB]*

*[additem ITEM_SHELL_BELL]*

*[additem ITEM_WIDE_LENS]*

*[additem ITEM_MUSCLE_BAND]*

*[additem ITEM_WISE_GLASSES]*

*[additem ITEM_FOCUS_BAND]*

*[additem ITEM_RED_CARD]*

*[additem ITEM_EJECT_BUTTON]*

*[additem ITEM_EXP_SHARE]*

*[additem ITEM_BICYCLE]*

*[additem ITEM_SUPER_ROD]*

*[additem ITEM_ITEMFINDER]*

*[additem ITEM_VS_SEEKER]*

*[additem ITEM_BERRY_POUCH]*

*[additem ITEM_ORAN_BERRY]*

*[additem ITEM_SITRUS_BERRY]*

*[additem ITEM_LUM_BERRY]*

*[additem ITEM_CHESTO_BERRY]*

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

### RR_Lab_EventScript_OakVsElm

> Oak: Elm. Jenny. You won, so you
> face me. Two on two.

> Elm: Us? Against you?
> Jenny, hold my notebook.

> Jenny: Professionally speaking,
> I accept.

> Ivy: Sweetie, we get front row
> seats. Somebody find popcorn.

> Watch Elm and Jenny battle
> Prof. Oak?

### RR_Lab_EventScript_OakVsElmBattle

### RR_Lab_EventScript_ElmTeam0

### RR_Lab_EventScript_OakBattleWatch0

*[trainerbattle_no_intro Pkmn Prof Oak (Squirtle, Chikorita)]*

### RR_Lab_EventScript_ElmTeam1

### RR_Lab_EventScript_OakBattleWatch1

*[trainerbattle_no_intro Pkmn Prof Oak (Bulbasaur, Cyndaquil)]*

### RR_Lab_EventScript_ElmTeam2

### RR_Lab_EventScript_OakBattleWatch2

*[trainerbattle_no_intro Pkmn Prof Oak (Charmander, Totodile)]*

### RR_Lab_EventScript_ElmTeam3

### RR_Lab_EventScript_OakBattleWatch3

*[trainerbattle_no_intro Pkmn Prof Oak (Totodile, Bulbasaur)]*

### RR_Lab_EventScript_ElmTeam4

### RR_Lab_EventScript_OakBattleWatch4

*[trainerbattle_no_intro Pkmn Prof Oak (Chikorita, Charmander)]*

### RR_Lab_EventScript_ElmTeam5

### RR_Lab_EventScript_OakBattleWatch5

*[trainerbattle_no_intro Pkmn Prof Oak (Cyndaquil, Squirtle, starter Elm/Jenny can have from the lab chain; Elm names his catches after, desserts)]*

### RR_Lab_EventScript_OakVsElmResult

### RR_Lab_EventScript_OakBeatElm

> Oak: Ho ho! Not bad. Not bad at
> all. Still mine, though.

> Elm: That was the warm-up!
> …It was not the warm-up.

### RR_Lab_EventScript_OakLostToElm

> Oak: Well! Beaten in my own lab.
> Gary will never let me hear it.

> Elm: For science! And for
> bragging rights! Mostly that.

### RR_Lab_EventScript_OakVsElmSkip

> The battle is a blur of flashes,
> cries and Elm shouting.

### RR_Lab_EventScript_Slowpoke

> Slowpoke: …Slooow?
>
> It's still holding the phone from
> this morning's call.
