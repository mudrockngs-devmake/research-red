@ ===================================================================
@ Research Red: Pewter, after Brock (animatic 12 "finale", moved to the
@ Pokemon Center: town scenes and rematches happen just outside Pokemon
@ Centers, #10; Nurse Joy doesn't allow battles indoors)
@ Story step 7 + badge 1: Elm and Jenny wait below-left of the Pewter
@ Pokemon Center door (15-16, 27). Talking to them: they spot the badge, 6v6 double, then
@ they head off to challenge Brock themselves. Sets VAR_0x40FE to 8.
@ They're friends travelling the same road: they'll turn up again.
@ ===================================================================

@ PewterCity OnTransition: Elm and Jenny only while it's pending
RR_Pewter_OnTransitionCity::
	call_if_ne VAR_0x40FE, 7, RR_Pewter_EventScript_HideElmJenny
	call_if_lt VAR_MAP_SCENE_PEWTER_CITY, 1, RR_Pewter_EventScript_HideElmJenny
	return

RR_Pewter_EventScript_HideElmJenny::
	setflag FLAG_TEMP_13
	setflag FLAG_TEMP_14
	return

RR_Pewter_EventScript_Rematch::
	lockall
	faceplayer
	playse SE_PIN
	applymovement LOCALID_PEWTER_ELM, Common_Movement_ExclamationMark
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Pewter_Text_Elm_Badge, MSGBOX_DEFAULT
	msgbox RR_Pewter_Text_Elm_SixOnSix, MSGBOX_DEFAULT
	msgbox RR_Pewter_Text_Jenny_Protocol, MSGBOX_DEFAULT
	msgbox RR_Pewter_Text_P_WarmUp, MSGBOX_DEFAULT
	msgbox RR_Pewter_Text_Jenny_Warm, MSGBOX_DEFAULT
	closemessage
	@ A loss continues the scene instead of whiting out
	setflag FLAG_RR_NO_WHITEOUT
	switch VAR_RR_STARTER
	case 0, RR_Pewter_EventScript_BattleAfterBulbasaur
	case 1, RR_Pewter_EventScript_BattleAfterCharmander
	case 2, RR_Pewter_EventScript_BattleAfterSquirtle
	case 3, RR_Pewter_EventScript_BattleAfterChikorita
	case 4, RR_Pewter_EventScript_BattleAfterCyndaquil
	case 5, RR_Pewter_EventScript_BattleAfterTotodile

@ Same pairings as the lab and Route 22 (Elm and Jenny keep their lab picks)
RR_Pewter_EventScript_BattleAfterBulbasaur::
	trainerbattle_two_trainers TRAINER_RR_ELM_PEWTER_CYNDAQUIL, RR_Pewter_Text_Elm_Defeat, TRAINER_RR_JENNY_PEWTER_TOTODILE, RR_Pewter_Text_Jenny_Defeat
	goto RR_Pewter_EventScript_AfterBattle

RR_Pewter_EventScript_BattleAfterCharmander::
	trainerbattle_two_trainers TRAINER_RR_ELM_PEWTER_TOTODILE, RR_Pewter_Text_Elm_Defeat, TRAINER_RR_JENNY_PEWTER_CHIKORITA, RR_Pewter_Text_Jenny_Defeat
	goto RR_Pewter_EventScript_AfterBattle

RR_Pewter_EventScript_BattleAfterSquirtle::
	trainerbattle_two_trainers TRAINER_RR_ELM_PEWTER_CHIKORITA, RR_Pewter_Text_Elm_Defeat, TRAINER_RR_JENNY_PEWTER_CYNDAQUIL, RR_Pewter_Text_Jenny_Defeat
	goto RR_Pewter_EventScript_AfterBattle

RR_Pewter_EventScript_BattleAfterChikorita::
	trainerbattle_two_trainers TRAINER_RR_ELM_PEWTER_CHARMANDER, RR_Pewter_Text_Elm_Defeat, TRAINER_RR_JENNY_PEWTER_SQUIRTLE, RR_Pewter_Text_Jenny_Defeat
	goto RR_Pewter_EventScript_AfterBattle

RR_Pewter_EventScript_BattleAfterCyndaquil::
	trainerbattle_two_trainers TRAINER_RR_ELM_PEWTER_SQUIRTLE, RR_Pewter_Text_Elm_Defeat, TRAINER_RR_JENNY_PEWTER_BULBASAUR, RR_Pewter_Text_Jenny_Defeat
	goto RR_Pewter_EventScript_AfterBattle

RR_Pewter_EventScript_BattleAfterTotodile::
	trainerbattle_two_trainers TRAINER_RR_ELM_PEWTER_BULBASAUR, RR_Pewter_Text_Elm_Defeat, TRAINER_RR_JENNY_PEWTER_CHARMANDER, RR_Pewter_Text_Jenny_Defeat
	goto RR_Pewter_EventScript_AfterBattle

RR_Pewter_EventScript_AfterBattle::
	clearflag FLAG_RR_NO_WHITEOUT
	special HealPlayerParty
	specialvar VAR_RESULT, GetBattleOutcome
	textcolor NPC_TEXT_COLOR_NEUTRAL
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, RR_Pewter_EventScript_Won
	msgbox RR_Pewter_Text_Elm_WeWon, MSGBOX_DEFAULT
	msgbox RR_Pewter_Text_Jenny_HealHere, MSGBOX_DEFAULT
	goto RR_Pewter_EventScript_ToTheGym

RR_Pewter_EventScript_Won::
	msgbox RR_Pewter_Text_Elm_GoodBattle, MSGBOX_DEFAULT
	msgbox RR_Pewter_Text_P_KnowYouDo, MSGBOX_DEFAULT

RR_Pewter_EventScript_ToTheGym::
	msgbox RR_Pewter_Text_Jenny_Brock, MSGBOX_DEFAULT
	msgbox RR_Pewter_Text_Ivy_Stew, MSGBOX_DEFAULT
	msgbox RR_Pewter_Text_Jenny_Motivated, MSGBOX_DEFAULT
	msgbox RR_Pewter_Text_Elm_CatchUp, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_PEWTER_ELM
	removeobject LOCALID_PEWTER_JENNY
	msgbox RR_Pewter_Text_HeadForGym, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_FROM_BLACK
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Pewter_Text_Ivy_OneBadge, MSGBOX_DEFAULT
	msgbox RR_Pewter_Text_P_SevenToGo, MSGBOX_DEFAULT
	msgbox RR_Pewter_Text_Ivy_BigDinner, MSGBOX_DEFAULT
	msgbox RR_Pewter_Text_P_BigDinner, MSGBOX_DEFAULT
	closemessage
	setvar VAR_0x40FE, 8
	releaseall
	end

RR_Pewter_Text_Elm_Badge::
    .string "Elm: {PLAYER}! We saw the whole\n"
    .string "thing! Onix went DOWN!$"

RR_Pewter_Text_Elm_SixOnSix::
    .string "Elm: {PLAYER}! Ivy! Badge or no\n"
    .string "badge, the race isn't over!\p"
    .string "Six on six! Right here!$"

RR_Pewter_Text_Jenny_Protocol::
    .string "Jenny: Out here, please. Nurse Joy\n"
    .string "gave us a look last time.\p"
    .string "By protocol, we battle before\n"
    .string "Brock. To warm up.$"

RR_Pewter_Text_P_WarmUp::
    .string "{PLAYER}: Your warm-up is us?$"

RR_Pewter_Text_Jenny_Warm::
    .string "Jenny: You are very warm people.$"

RR_Pewter_Text_Elm_Defeat::
    .string "Elm: One to nothing. Fine. FINE.\n"
    .string "It's a long road.$"

RR_Pewter_Text_Jenny_Defeat::
    .string "Jenny: He means\n"
    .string "“congratulations.”$"

RR_Pewter_Text_Elm_GoodBattle::
    .string "Elm: …You're really good at this,\n"
    .string "{PLAYER}. I'm proud of you.$"

RR_Pewter_Text_P_KnowYouDo::
    .string "{PLAYER}: I know you are.\n"
    .string "You always are.$"

RR_Pewter_Text_Elm_WeWon::
    .string "Elm: We won! On a sidewalk!\n"
    .string "It still counts!$"

RR_Pewter_Text_Jenny_HealHere::
    .string "Jenny: Conveniently, the healing\n"
    .string "is right through that door.$"

RR_Pewter_Text_Jenny_Brock::
    .string "Jenny: We're going to challenge\n"
    .string "Brock now. Wish us luck.$"

RR_Pewter_Text_Ivy_Stew::
    .string "Ivy: Good luck, Jenny.\n"
    .string "He made stew.$"

RR_Pewter_Text_Jenny_Motivated::
    .string "Jenny: …I am suddenly very\n"
    .string "motivated.$"

RR_Pewter_Text_Elm_CatchUp::
    .string "Elm: See you down the road, you\n"
    .string "two! We always catch up!$"

RR_Pewter_Text_HeadForGym::
    .string "Elm and Jenny head for the Gym,\n"
    .string "arguing happily about strategy.$"

RR_Pewter_Text_Ivy_OneBadge::
    .string "Ivy: One badge down, sweetie.$"

RR_Pewter_Text_P_SevenToGo::
    .string "{PLAYER}: Seven to go.\n"
    .string "And one bet.$"

RR_Pewter_Text_Ivy_BigDinner::
    .string "Ivy: And one very big dinner.$"

RR_Pewter_Text_P_BigDinner::
    .string "{PLAYER}: And one very big dinner.$"
