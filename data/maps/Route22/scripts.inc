Route22_MapScripts::
	.byte 0

Route22_EventScript_EarlyRivalTriggerTop::
	lockall
	setvar VAR_TEMP_1, 0
	goto Route22_EventScript_EarlyRival
	end

Route22_EventScript_EarlyRivalTriggerMid::
	lockall
	setvar VAR_TEMP_1, 1
	setobjectxyperm LOCALID_ROUTE22_RIVAL, 25, 5
	goto Route22_EventScript_EarlyRival
	end

Route22_EventScript_EarlyRivalTriggerBottom::
	lockall
	setvar VAR_TEMP_1, 2
	setobjectxyperm LOCALID_ROUTE22_RIVAL, 25, 5
	goto Route22_EventScript_EarlyRival
	end

@ Research Red: Elm and Jenny's first road battle. Elm uses the rival object;
@ Jenny walks in beside him. Team is FireRed's placeholder until the trainer
@ data update. Winning (or losing) wakes Viridian's old man.
Route22_EventScript_EarlyRival::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playbgm MUS_ENCOUNTER_RIVAL, 0
	addobject LOCALID_ROUTE22_RIVAL
	addobject LOCALID_ROUTE22_JENNY
	applymovement LOCALID_ROUTE22_JENNY, Route22_Movement_RivalApproach
	call_if_eq VAR_TEMP_1, 0, Route22_EventScript_EarlyRivalApproach
	call_if_eq VAR_TEMP_1, 1, Route22_EventScript_EarlyRivalApproach
	call_if_eq VAR_TEMP_1, 2, Route22_EventScript_EarlyRivalApproachBottom
	waitmovement LOCALID_ROUTE22_JENNY
	delay 6
	msgbox RR_Route22_Text_Elm_FoundYou, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route22_Text_P_Sights, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route22_Text_Jenny_Desserts, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route22_Text_Elm_Rematch, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route22_Text_Ivy_Ready, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route22_Text_P_Always, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	call_if_eq VAR_STARTER_MON, 2, Route22_EventScript_EarlyRivalSquirtle
	call_if_eq VAR_STARTER_MON, 1, Route22_EventScript_EarlyRivalBulbasaur
	call_if_eq VAR_STARTER_MON, 0, Route22_EventScript_EarlyRivalCharmander
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route22_Text_Elm_Forever, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route22_Text_Jenny_GoodGame, MSGBOX_DEFAULT
	msgbox RR_Route22_Text_Ivy_NextTown, MSGBOX_DEFAULT
	msgbox RR_Route22_Text_Jenny_NextTown, MSGBOX_DEFAULT
	closemessage
	delay 10
	playbgm MUS_RIVAL_EXIT, 0
	applymovement LOCALID_ROUTE22_JENNY, Route22_Movement_EarlyRivalExit
	call_if_eq VAR_TEMP_1, 0, Route22_EventScript_EarlyRivalExit
	call_if_eq VAR_TEMP_1, 1, Route22_EventScript_EarlyRivalExit
	call_if_eq VAR_TEMP_1, 2, Route22_EventScript_EarlyRivalExitBottom
	waitmovement LOCALID_ROUTE22_JENNY
	fadedefaultbgm
	removeobject LOCALID_ROUTE22_RIVAL
	removeobject LOCALID_ROUTE22_JENNY
	setvar VAR_MAP_SCENE_ROUTE22, 2
	setvar VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN, 1
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route22_Text_OldManAwake, MSGBOX_DEFAULT
	releaseall
	end

Route22_EventScript_EarlyRivalApproach::
	applymovement LOCALID_ROUTE22_RIVAL, Route22_Movement_RivalApproach
	waitmovement 0
	return

Route22_EventScript_EarlyRivalApproachBottom::
	applymovement LOCALID_ROUTE22_RIVAL, Route22_Movement_RivalApproachBottom
	applymovement LOCALID_PLAYER, Route22_Movement_PlayerFaceRival
	waitmovement 0
	return

Route22_EventScript_EarlyRivalSquirtle::
	trainerbattle_earlyrival TRAINER_RIVAL_ROUTE22_EARLY_SQUIRTLE, 0, RR_Route22_Text_Elm_Defeat, Text_RivalVictory
	return

Route22_EventScript_EarlyRivalBulbasaur::
	trainerbattle_earlyrival TRAINER_RIVAL_ROUTE22_EARLY_BULBASAUR, 0, RR_Route22_Text_Elm_Defeat, Text_RivalVictory
	return

Route22_EventScript_EarlyRivalCharmander::
	trainerbattle_earlyrival TRAINER_RIVAL_ROUTE22_EARLY_CHARMANDER, 0, RR_Route22_Text_Elm_Defeat, Text_RivalVictory
	return

Route22_EventScript_EarlyRivalExit::
	applymovement LOCALID_ROUTE22_RIVAL, Route22_Movement_EarlyRivalExit
	waitmovement 0
	return

Route22_EventScript_EarlyRivalExitBottom::
	applymovement LOCALID_ROUTE22_RIVAL, Route22_Movement_EarlyRivalExitBottom
	waitmovement 0
	return

Route22_Movement_UnusedRivalExit:: @ 8
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

Route22_Movement_EarlyRivalExit::
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

Route22_Movement_EarlyRivalExitBottom::
	walk_right
	walk_right
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

Route22_Movement_RivalApproach::
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

Route22_Movement_RivalApproachBottom::
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_down
	step_end

Route22_Movement_PlayerFaceRival::
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_8
	walk_in_place_faster_up
	step_end

Route22_EventScript_LateRivalTriggerTop::
	lockall
	setvar VAR_TEMP_1, 0
	goto Route22_EventScript_LateRival
	end

Route22_EventScript_LateRivalTriggerMid::
	lockall
	setvar VAR_TEMP_1, 1
	setobjectxyperm LOCALID_ROUTE22_RIVAL, 25, 5
	goto Route22_EventScript_LateRival
	end

Route22_EventScript_LateRivalTriggerBottom::
	lockall
	setvar VAR_TEMP_1, 2
	setobjectxyperm LOCALID_ROUTE22_RIVAL, 25, 5
	goto Route22_EventScript_LateRival
	end

Route22_EventScript_LateRival::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playbgm MUS_ENCOUNTER_RIVAL, 0
	addobject LOCALID_ROUTE22_RIVAL
	call_if_eq VAR_TEMP_1, 0, Route22_EventScript_LateRivalApproach
	call_if_eq VAR_TEMP_1, 1, Route22_EventScript_LateRivalApproach
	call_if_eq VAR_TEMP_1, 2, Route22_EventScript_LateRivalApproachBottom
	msgbox Route22_Text_LateRivalIntro
	setvar VAR_LAST_TALKED, LOCALID_ROUTE22_RIVAL
	call_if_eq VAR_STARTER_MON, 2, Route22_EventScript_LateRivalSquirtle
	call_if_eq VAR_STARTER_MON, 1, Route22_EventScript_LateRivalBulbasaur
	call_if_eq VAR_STARTER_MON, 0, Route22_EventScript_LateRivalCharmander
	msgbox Route22_Text_LateRivalPostBattle
	closemessage
	delay 10
	playbgm MUS_RIVAL_EXIT, 0
	applymovement LOCALID_ROUTE22_RIVAL, Route22_Movement_LateRivalExit
	waitmovement 0
	fadedefaultbgm
	removeobject LOCALID_ROUTE22_RIVAL
	setvar VAR_MAP_SCENE_ROUTE22, 4
	releaseall
	end

Route22_EventScript_LateRivalApproach::
	applymovement LOCALID_ROUTE22_RIVAL, Route22_Movement_RivalApproach
	waitmovement 0
	return

Route22_EventScript_LateRivalApproachBottom::
	applymovement LOCALID_ROUTE22_RIVAL, Route22_Movement_RivalApproachBottom
	applymovement LOCALID_PLAYER, Route22_Movement_PlayerFaceRival
	waitmovement 0
	return

Route22_EventScript_LateRivalSquirtle::
	trainerbattle_no_intro TRAINER_RIVAL_ROUTE22_LATE_SQUIRTLE, Route22_Text_LateRivalDefeat
	return

Route22_EventScript_LateRivalBulbasaur::
	trainerbattle_no_intro TRAINER_RIVAL_ROUTE22_LATE_BULBASAUR, Route22_Text_LateRivalDefeat
	return

Route22_EventScript_LateRivalCharmander::
	trainerbattle_no_intro TRAINER_RIVAL_ROUTE22_LATE_CHARMANDER, Route22_Text_LateRivalDefeat
	return

Route22_Movement_LateRivalExit::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

Route22_EventScript_LeagueGateSign::
	msgbox Route22_Text_LeagueGateSign, MSGBOX_SIGN
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
    .string "Jenny: He caught a new partner.\n"
    .string "He named it after a dessert.$"

RR_Route22_Text_Elm_Rematch::
    .string "Elm: Rematch! Right here!\n"
    .string "This time I read a book!$"

RR_Route22_Text_Ivy_Ready::
    .string "Ivy: Ready, sweetie?$"

RR_Route22_Text_P_Always::
    .string "{PLAYER}: Always. Mostly.\n"
    .string "Let's say yes.$"

RR_Route22_Text_Elm_Defeat::
    .string "Elm: Okay! Okay.\n"
    .string "That was the warm-up.$"

RR_Route22_Text_Elm_Forever::
    .string "Elm: I demand a rematch.\n"
    .string "At every Gym. Forever.$"

RR_Route22_Text_Jenny_GoodGame::
    .string "Jenny: He means “good game.”$"

RR_Route22_Text_Ivy_NextTown::
    .string "Ivy: Same time next town, Jenny?$"

RR_Route22_Text_Jenny_NextTown::
    .string "Jenny: Same time next town.$"

RR_Route22_Text_OldManAwake::
    .string "Somewhere in Viridian, an old man\n"
    .string "finally wakes up from his nap.$"
