@ Research Red (#69, #142): watching Elm and Jenny take on Brock, a scripted
@ fight on the battle screen. Spec: docs/scenes/pewter_brock_watch.md.
@ After the Pewter rematch Elm asks if you want to watch (rr_pewter.inc):
@ Yes warps you into the Gym, No leaves it waiting; either way it plays when
@ you enter the Gym with VAR_RR_BROCK_WATCH at 1. Brock sends two at a time
@ in three rounds; the lines are in RR_FIGHT_BROCK_WATCH (src/rr_scripted_fight.c).

@ The scene's people and Pokémon stay hidden unless it is playing
RR_BrockWatch_OnTransition::
	setflag FLAG_TEMP_11
	setflag FLAG_TEMP_12
	setflag FLAG_TEMP_13
	return

RR_BrockWatch_OnFrame::
	map_script_2 VAR_RR_BROCK_WATCH, 1, RR_BrockWatch_EventScript_Scene
	.2byte 0

RR_BrockWatch_EventScript_Scene::
	lockall
	@ you and Ivy take the side of the field, Elm and Jenny walk in
	rr_ivy_to_scene LOCALID_PEWTER_GYM_IVY
	rr_walk_to LOCALID_PLAYER, 2, 9, FALSE, 0
	rr_walk_to LOCALID_PEWTER_GYM_IVY, 2, 10, FALSE, 16
	waitmovement LOCALID_PEWTER_GYM_IVY
	waitmovement LOCALID_PLAYER
	applymovement LOCALID_PLAYER, RR_BrockWatch_Movement_FaceRight
	applymovement LOCALID_PEWTER_GYM_IVY, RR_BrockWatch_Movement_FaceRight
	waitmovement 0
	setobjectxy LOCALID_PEWTER_GYM_ELM, 5, 13
	setobjectxy LOCALID_PEWTER_GYM_JENNY, 7, 13
	addobject LOCALID_PEWTER_GYM_ELM
	addobject LOCALID_PEWTER_GYM_JENNY
	applymovement LOCALID_PEWTER_GYM_ELM, RR_BrockWatch_Movement_Up3
	applymovement LOCALID_PEWTER_GYM_JENNY, RR_BrockWatch_Movement_Up3
	waitmovement LOCALID_PEWTER_GYM_JENNY
	textcolor NPC_TEXT_COLOR_NEUTRAL
	closemessage
	@ Elm and Jenny's first picks; the rest of the fight is in rr_scripted_fight.c
	callnative RR_ScriptBrockWatchPair
	setvar VAR_0x8006, RR_FIGHT_BROCK_WATCH
	callnative RR_ScriptStartScriptedFight
	trainerbattle_no_intro TRAINER_RR_BROCK_WATCH, RR_BrockWatch_Text_Brock_Defeat
	callnative RR_ScriptEndScriptedFight
	@ --- the badge ---
	playfanfare MUS_OBTAIN_BADGE
	msgbox RR_BrockWatch_Text_Brock_Badge, MSGBOX_DEFAULT
	waitfanfare
	msgbox RR_BrockWatch_Text_Elm_Allergies, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_Brock_Ivy, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_Ivy_Thanks, MSGBOX_DEFAULT
	closemessage
	@ --- the four of you by the Gym door ---
	fadescreen FADE_TO_BLACK
	setobjectxy LOCALID_PEWTER_GYM_ELM, 5, 12
	setobjectxy LOCALID_PEWTER_GYM_JENNY, 7, 12
	turnobject LOCALID_PEWTER_GYM_ELM, DIR_SOUTH
	turnobject LOCALID_PEWTER_GYM_JENNY, DIR_SOUTH
	setobjectxy LOCALID_PLAYER, 5, 13
	setobjectxy LOCALID_PEWTER_GYM_IVY, 7, 13
	turnobject LOCALID_PLAYER, DIR_NORTH
	turnobject LOCALID_PEWTER_GYM_IVY, DIR_NORTH
	special DrawWholeMapView
	fadescreen FADE_FROM_BLACK
	msgbox RR_BrockWatch_Text_Elm_RaceOn, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_Jenny_Dinner, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_P_Menu, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_Ivy_Sweetie, MSGBOX_DEFAULT
	closemessage
	@ Elm and Jenny head out first; Ivy follows you again
	applymovement LOCALID_PLAYER, RR_BrockWatch_Movement_StepAsideLeft
	applymovement LOCALID_PEWTER_GYM_IVY, RR_BrockWatch_Movement_StepAsideRight
	waitmovement 0
	rr_move_pair LOCALID_PEWTER_GYM_ELM, LOCALID_PEWTER_GYM_JENNY, RR_BrockWatch_Movement_ElmOut
	waitmovement LOCALID_PEWTER_GYM_JENNY
	removeobject LOCALID_PEWTER_GYM_ELM
	removeobject LOCALID_PEWTER_GYM_JENNY
	playse SE_EXIT
	rr_ivy_follow LOCALID_PEWTER_GYM_IVY
	setvar VAR_RR_BROCK_WATCH, 2
	releaseall
	end

RR_BrockWatch_Movement_FaceRight:
	face_right
	step_end

RR_BrockWatch_Movement_Up3:
	walk_up
	walk_up
	walk_up
	step_end

RR_BrockWatch_Movement_StepAsideLeft:
	walk_left
	face_right
	step_end

RR_BrockWatch_Movement_StepAsideRight:
	walk_right
	face_left
	step_end

RR_BrockWatch_Movement_ElmOut:
	walk_down
	walk_down
	set_invisible
	step_end

RR_BrockWatch_Text_Brock_Defeat:
	.string "Brock: I have never lost to a\n"
	.string "lecture before.$"

RR_BrockWatch_Text_Brock_Badge:
	.string "Brock: Fair and square. The\n"
	.string "Boulder Badge is yours.$"

RR_BrockWatch_Text_Elm_Allergies:
	.string "Elm: I'm not crying! It's\n"
	.string "allergies! This Gym is very…\l"
	.string "rocky!$"

RR_BrockWatch_Text_Brock_Ivy:
	.string "Brock: Professor Ivy, your notes\n"
	.string "on regional forms helped my\l"
	.string "breeding more than I can say.$"

RR_BrockWatch_Text_Ivy_Thanks:
	.string "Ivy: Then they did their job.\n"
	.string "Thank you, Brock.$"

RR_BrockWatch_Text_Elm_RaceOn:
	.string "Elm: One badge each! The race\n"
	.string "is ON!$"

RR_BrockWatch_Text_Jenny_Dinner:
	.string "Jenny: And dinner at Delia's is\n"
	.string "still on the line, dear.$"

RR_BrockWatch_Text_P_Menu:
	.string "{PLAYER}: I've already planned\n"
	.string "the menu. For you to cook.$"

RR_BrockWatch_Text_Ivy_Sweetie:
	.string "Ivy: Sweetie.$"
