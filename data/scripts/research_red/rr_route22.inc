@ Research Red: Route 22, Elm and Jenny's 6v6 double (animatic 06)
@ Started by Route22's EarlyRival triggers (x 33, entering from Viridian) once
@ the lab is done (VAR_MAP_SCENE_ROUTE22 == 1). Elm and Jenny use their lab
@ starters (the chain from VAR_RR_STARTER) plus their dessert-named catches.
@ Their mid-battle banter is in src/data/research_red/trainer_slides.h.
@ Win or lose the story goes on: story progress becomes 6, which sets up the
@ lunch in Viridian City (rr_lunch.inc).

RR_Route22_EventScript_ElmJenny::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playbgm MUS_ENCOUNTER_RIVAL, 0
	addobject LOCALID_ROUTE22_RIVAL
	addobject LOCALID_ROUTE22_JENNY
	@ #83: Jenny (24,4) trails Elm (25,4) whichever row you came in on
	call_if_ne VAR_TEMP_1, 2, RR_Route22_EventScript_PairApproach
	call_if_eq VAR_TEMP_1, 2, RR_Route22_EventScript_PairApproachBottom
	delay 6
	msgbox RR_Route22_Text_Elm_FoundYou, MSGBOX_DEFAULT
	msgbox RR_Route22_Text_P_Sights, MSGBOX_DEFAULT
	msgbox RR_Route22_Text_Jenny_Desserts, MSGBOX_DEFAULT
	msgbox RR_Route22_Text_Elm_PastryStarters, MSGBOX_DEFAULT
	msgbox RR_Route22_Text_Jenny_NameTags, MSGBOX_DEFAULT
	msgbox RR_Route22_Text_Elm_SixOnSix, MSGBOX_DEFAULT
	msgbox RR_Route22_Text_Ivy_Ready, MSGBOX_DEFAULT
	msgbox RR_Route22_Text_P_Always, MSGBOX_DEFAULT
	closemessage
	@ A loss continues the story instead of whiting out
	setflag FLAG_RR_NO_WHITEOUT
	switch VAR_RR_STARTER
	case 0, RR_Route22_EventScript_BattleAfterBulbasaur
	case 1, RR_Route22_EventScript_BattleAfterCharmander
	case 2, RR_Route22_EventScript_BattleAfterSquirtle
	case 3, RR_Route22_EventScript_BattleAfterChikorita
	case 4, RR_Route22_EventScript_BattleAfterCyndaquil
	case 5, RR_Route22_EventScript_BattleAfterTotodile

@ Player picked Bulbasaur: Elm Cyndaquil, Jenny Totodile
RR_Route22_EventScript_BattleAfterBulbasaur::
	trainerbattle_two_trainers TRAINER_RR_ELM_R22_CYNDAQUIL, RR_Route22_Text_Elm_Defeat, TRAINER_RR_JENNY_R22_TOTODILE, RR_Route22_Text_Jenny_Defeat
	goto RR_Route22_EventScript_AfterBattle

@ Player picked Charmander: Elm Totodile, Jenny Chikorita
RR_Route22_EventScript_BattleAfterCharmander::
	trainerbattle_two_trainers TRAINER_RR_ELM_R22_TOTODILE, RR_Route22_Text_Elm_Defeat, TRAINER_RR_JENNY_R22_CHIKORITA, RR_Route22_Text_Jenny_Defeat
	goto RR_Route22_EventScript_AfterBattle

@ Player picked Squirtle: Elm Chikorita, Jenny Cyndaquil
RR_Route22_EventScript_BattleAfterSquirtle::
	trainerbattle_two_trainers TRAINER_RR_ELM_R22_CHIKORITA, RR_Route22_Text_Elm_Defeat, TRAINER_RR_JENNY_R22_CYNDAQUIL, RR_Route22_Text_Jenny_Defeat
	goto RR_Route22_EventScript_AfterBattle

@ Player picked Chikorita: Elm Charmander, Jenny Squirtle
RR_Route22_EventScript_BattleAfterChikorita::
	trainerbattle_two_trainers TRAINER_RR_ELM_R22_CHARMANDER, RR_Route22_Text_Elm_Defeat, TRAINER_RR_JENNY_R22_SQUIRTLE, RR_Route22_Text_Jenny_Defeat
	goto RR_Route22_EventScript_AfterBattle

@ Player picked Cyndaquil: Elm Squirtle, Jenny Bulbasaur
RR_Route22_EventScript_BattleAfterCyndaquil::
	trainerbattle_two_trainers TRAINER_RR_ELM_R22_SQUIRTLE, RR_Route22_Text_Elm_Defeat, TRAINER_RR_JENNY_R22_BULBASAUR, RR_Route22_Text_Jenny_Defeat
	goto RR_Route22_EventScript_AfterBattle

@ Player picked Totodile: Elm Bulbasaur, Jenny Charmander
RR_Route22_EventScript_BattleAfterTotodile::
	trainerbattle_two_trainers TRAINER_RR_ELM_R22_BULBASAUR, RR_Route22_Text_Elm_Defeat, TRAINER_RR_JENNY_R22_CHARMANDER, RR_Route22_Text_Jenny_Defeat
	goto RR_Route22_EventScript_AfterBattle

RR_Route22_EventScript_AfterBattle::
	clearflag FLAG_RR_NO_WHITEOUT
	special HealPlayerParty
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, RR_Route22_EventScript_Won
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route22_Text_Elm_WeWon, MSGBOX_DEFAULT
	msgbox RR_Route22_Text_Jenny_Thrilled, MSGBOX_DEFAULT
	goto RR_Route22_EventScript_Goodbye

RR_Route22_EventScript_Won::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route22_Text_Elm_Forever, MSGBOX_DEFAULT
	msgbox RR_Route22_Text_Jenny_GoodGame, MSGBOX_DEFAULT
	msgbox RR_Route22_Text_Elm_GoodGame, MSGBOX_DEFAULT

RR_Route22_EventScript_Goodbye::
	@ Win or lose, Jenny gives you the DexNav: her old spotting-drive logbook
	msgbox RR_Route22_Text_Jenny_BeforeWeGo, MSGBOX_DEFAULT
	msgbox RR_Route22_Text_Jenny_Logbook, MSGBOX_DEFAULT
	msgbox RR_Route22_Text_P_KeptIt, MSGBOX_DEFAULT
	msgbox RR_Route22_Text_Jenny_App, MSGBOX_DEFAULT
	setflag FLAG_RR_DEXNAV_GET
	setflag FLAG_RR_DEXNAV_DETECTOR
	playfanfare MUS_OBTAIN_KEY_ITEM
	message RR_Route22_Text_ReceivedDexNav
	waitmessage
	waitfanfare
	msgbox RR_Route22_Text_Elm_Stickers, MSGBOX_DEFAULT
	msgbox RR_Route22_Text_Jenny_Stickers, MSGBOX_DEFAULT
	msgbox RR_Route22_Text_P_JennyHug, MSGBOX_DEFAULT
	msgbox RR_Route22_Text_Jenny_HowTo, MSGBOX_DEFAULT
	msgbox RR_Route22_Text_Ivy_NextTown, MSGBOX_DEFAULT
	msgbox RR_Route22_Text_Jenny_NextTown, MSGBOX_DEFAULT
	msgbox RR_Route22_Text_Elm_Lunch, MSGBOX_DEFAULT
	closemessage
	delay 10
	playbgm MUS_RIVAL_EXIT, 0
	call_if_ne VAR_TEMP_1, 2, RR_Route22_EventScript_PairExit
	call_if_eq VAR_TEMP_1, 2, RR_Route22_EventScript_PairExitBottom
	fadedefaultbgm
	removeobject LOCALID_ROUTE22_RIVAL
	removeobject LOCALID_ROUTE22_JENNY
	setvar VAR_MAP_SCENE_ROUTE22, 2
	@ Story progress 6: lunch with Elm and Jenny outside the Viridian Pokemon Center
	setvar VAR_0x40FE, 6
	releaseall
	end

@ ===================================================================
@ Research Red: Route 22 text
@ ===================================================================
RR_Route22_Text_Elm_FoundYou::
    .string "Elm: {PLAYER}! Ivy!\n"
    .string "You followed us! Spying, are we?$"

RR_Route22_Text_P_Sights::
    .string "{PLAYER}: We came for the sights.\n"
    .string "You're one of them.$"

RR_Route22_Text_Jenny_Desserts::
    .string "Jenny: We've caught four.\n"
    .string "Elm named them after desserts.$"

@ The starters get pastry names from here on (#48): Bulbasaur Pistachio,
@ Charmander Toffee, Squirtle Cream Puff, Chikorita Matcha, Cyndaquil
@ Cinnamon, Totodile Biscotti (src/data/trainers.party, R22 and Pewter teams)
RR_Route22_Text_Elm_PastryStarters::
    .string "Elm: And the starters too! Both\n"
    .string "are pastries now. Very official.$"

RR_Route22_Text_Jenny_NameTags::
    .string "Jenny: He made name tags.\n"
    .string "With glitter.$"

RR_Route22_Text_Elm_SixOnSix::
    .string "Elm: Our team is complete!\n"
    .string "Six on six! Right here! Right now!$"

RR_Route22_Text_Ivy_Ready::
    .string "Ivy: Ready, sweetie?$"

RR_Route22_Text_P_Always::
    .string "{PLAYER}: Always. Mostly.\n"
    .string "Let's say yes.$"

RR_Route22_Text_Elm_Defeat::
    .string "Elm: I demand a rematch.\n"
    .string "At every Gym. Forever.$"

RR_Route22_Text_Jenny_Defeat::
    .string "Jenny: Well fought, you two.\n"
    .string "I'll write it up kindly.$"

RR_Route22_Text_Elm_Forever::
    .string "Elm: Fine. FINE.\n"
    .string "It's a long road to eight badges.$"

RR_Route22_Text_Jenny_GoodGame::
    .string "Jenny: He means “good game.”$"

RR_Route22_Text_Elm_GoodGame::
    .string "Elm: …Good game, {PLAYER}.\n"
    .string "Really.$"

RR_Route22_Text_Elm_WeWon::
    .string "Elm: We won?! Jenny!\n"
    .string "Jenny, we WON!$"

RR_Route22_Text_Jenny_Thrilled::
    .string "Jenny: I'm thrilled, dear.\n"
    .string "I'm showing it on the inside.$"

RR_Route22_Text_Jenny_BeforeWeGo::
    .string "Jenny: {PLAYER}. Before we go.$"

RR_Route22_Text_Jenny_Logbook::
    .string "Jenny: Remember our spotting\n"
    .string "drives? I kept the logbook.$"

RR_Route22_Text_P_KeptIt::
    .string "{PLAYER}: You kept the logbook?$"

RR_Route22_Text_Jenny_App::
    .string "Jenny: I turned it into an app.\n"
    .string "It finds Pokémon. Like we used to.$"

RR_Route22_Text_ReceivedDexNav::
    .string "{PLAYER} received the DexNav!$"

RR_Route22_Text_Elm_Stickers::
    .string "Elm: She made me one too! Mine has\n"
    .string "stickers.$"

RR_Route22_Text_Jenny_Stickers::
    .string "Jenny: He added the stickers.$"

RR_Route22_Text_P_JennyHug::
    .string "{PLAYER}: (Jenny doesn't hug.\n"
    .string "This is a Jenny hug.)$"

RR_Route22_Text_Jenny_HowTo::
    .string "Jenny: Start menu, DexNav. Pick a\n"
    .string "Pokémon you've seen nearby.\p"
    .string "Then sneak up on it. Slowly.\n"
    .string "It's procedure.$"

RR_Route22_Text_Ivy_NextTown::
    .string "Ivy: Same time next town, Jenny?$"

RR_Route22_Text_Jenny_NextTown::
    .string "Jenny: Same time next town.$"

RR_Route22_Text_Elm_Lunch::
    .string "Elm: But first, lunch! We'll meet\n"
    .string "you at the Pokémon Center!$"

@ #83: Elm leads, Jenny trails one step behind (rr_move_pair)
RR_Route22_EventScript_PairApproach::
	rr_move_pair LOCALID_ROUTE22_RIVAL, LOCALID_ROUTE22_JENNY, Route22_Movement_RivalApproach
	waitmovement LOCALID_ROUTE22_JENNY
	return

RR_Route22_EventScript_PairApproachBottom::
	rr_move_pair LOCALID_ROUTE22_RIVAL, LOCALID_ROUTE22_JENNY, Route22_Movement_RivalApproachBottom
	applymovement LOCALID_PLAYER, Route22_Movement_PlayerFaceRival
	waitmovement LOCALID_ROUTE22_JENNY
	return

RR_Route22_EventScript_PairExit::
	rr_move_pair LOCALID_ROUTE22_RIVAL, LOCALID_ROUTE22_JENNY, Route22_Movement_EarlyRivalExit
	waitmovement LOCALID_ROUTE22_JENNY
	return

RR_Route22_EventScript_PairExitBottom::
	rr_move_pair LOCALID_ROUTE22_RIVAL, LOCALID_ROUTE22_JENNY, Route22_Movement_EarlyRivalExitBottom
	waitmovement LOCALID_ROUTE22_JENNY
	return
