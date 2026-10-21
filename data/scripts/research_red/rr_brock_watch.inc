@ Research Red (#69): watching Elm and Jenny take on Brock, a scripted scene
@ (not a real battle). Spec: docs/scenes/pewter_brock_watch.md.
@ After the Pewter rematch Elm asks if you want to watch (rr_pewter.inc):
@ Yes warps you into the Gym, No leaves it waiting; either way it plays when
@ you enter the Gym with VAR_RR_BROCK_WATCH at 1. Brock sends two at a time
@ in three rounds; the Pokémon on the field are overworld sprites whose
@ species are set per round (RR_ScriptBrockWatchRound, src/research_red.c).

@ The scene's people and Pokémon stay hidden unless it is playing
RR_BrockWatch_OnTransition::
	setflag FLAG_TEMP_11
	setflag FLAG_TEMP_12
	setflag FLAG_TEMP_13
	setflag FLAG_TEMP_14
	setflag FLAG_TEMP_15
	setflag FLAG_TEMP_16
	setflag FLAG_TEMP_17
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
	msgbox RR_BrockWatch_Text_Brock_Welcome, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_Elm_Ready, MSGBOX_DEFAULT
	closemessage
	@ --- round 1: Geodude and Sandshrew ---
	setvar VAR_0x8004, 1
	call RR_BrockWatch_EventScript_SendOut
	msgbox RR_BrockWatch_Text_Elm_Lecture, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_Brock_Lectured, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_Jenny_Hello, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_Ivy_Eggs, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_P_Eggs, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_Brock_Eggs, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_P_Four, MSGBOX_DEFAULT
	closemessage
	call RR_BrockWatch_EventScript_Clash
	msgbox RR_BrockWatch_Text_SuperEffective, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_Jenny_Evidence, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_Elm_MyObservation, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_Jenny_CaseClosed, MSGBOX_DEFAULT
	closemessage
	call RR_BrockWatch_EventScript_BrockFaints
	@ --- round 2: Kabuto and Omanyte ---
	setvar VAR_0x8004, 2
	call RR_BrockWatch_EventScript_SendOut
	msgbox RR_BrockWatch_Text_Elm_Fossils, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_Jenny_Command, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_Ivy_Ask, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_P_Ask, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_Elm_WhereDidYou, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_Brock_Museum, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_Jenny_Command2, MSGBOX_DEFAULT
	closemessage
	call RR_BrockWatch_EventScript_Clash
	call RR_BrockWatch_EventScript_BrockFaints
	@ --- round 3: Rhyhorn and Onix ---
	setvar VAR_0x8004, 3
	call RR_BrockWatch_EventScript_SendOut
	playmoncry SPECIES_ONIX, CRY_MODE_NORMAL
	waitmoncry
	msgbox RR_BrockWatch_Text_Elm_Bigger, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_Jenny_Command3, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_Elm_ForScience, MSGBOX_DEFAULT
	closemessage
	call RR_BrockWatch_EventScript_Clash
	msgbox RR_BrockWatch_Text_Bind, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_P_Composure, MSGBOX_DEFAULT
	closemessage
	call RR_BrockWatch_EventScript_Clash
	call RR_BrockWatch_EventScript_BrockFaints
	@ --- the badge ---
	playfanfare MUS_OBTAIN_BADGE
	msgbox RR_BrockWatch_Text_Brock_Badge, MSGBOX_DEFAULT
	waitfanfare
	msgbox RR_BrockWatch_Text_Elm_Allergies, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_Brock_Ivy, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_Ivy_Thanks, MSGBOX_DEFAULT
	closemessage
	removeobject LOCALID_PEWTER_GYM_MON_2
	removeobject LOCALID_PEWTER_GYM_MON_3
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

@ Both sides send out their pair (round in VAR_0x8004)
RR_BrockWatch_EventScript_SendOut::
	callnative RR_ScriptBrockWatchRound
	playse SE_BALL_OPEN
	addobject LOCALID_PEWTER_GYM_MON_0
	addobject LOCALID_PEWTER_GYM_MON_1
	delay 20
	playse SE_BALL_OPEN
	addobject LOCALID_PEWTER_GYM_MON_2
	addobject LOCALID_PEWTER_GYM_MON_3
	delay 20
	return

@ A trade of blows: everyone lunges, hits land
RR_BrockWatch_EventScript_Clash::
	applymovement LOCALID_PEWTER_GYM_MON_2, RR_BrockWatch_Movement_LungeUp
	applymovement LOCALID_PEWTER_GYM_MON_3, RR_BrockWatch_Movement_LungeUp
	waitmovement LOCALID_PEWTER_GYM_MON_3
	playse SE_M_DOUBLE_SLAP
	applymovement LOCALID_PEWTER_GYM_MON_0, RR_BrockWatch_Movement_Shake
	applymovement LOCALID_PEWTER_GYM_MON_1, RR_BrockWatch_Movement_Shake
	waitmovement LOCALID_PEWTER_GYM_MON_1
	applymovement LOCALID_PEWTER_GYM_MON_0, RR_BrockWatch_Movement_LungeDown
	applymovement LOCALID_PEWTER_GYM_MON_1, RR_BrockWatch_Movement_LungeDown
	waitmovement LOCALID_PEWTER_GYM_MON_1
	playse SE_M_ROCK_THROW
	applymovement LOCALID_PEWTER_GYM_MON_2, RR_BrockWatch_Movement_Shake
	applymovement LOCALID_PEWTER_GYM_MON_3, RR_BrockWatch_Movement_Shake
	waitmovement LOCALID_PEWTER_GYM_MON_3
	return

@ Brock's pair faints and returns; Elm's and Jenny's go back for the next round
RR_BrockWatch_EventScript_BrockFaints::
	playse SE_FAINT
	applymovement LOCALID_PEWTER_GYM_MON_0, RR_BrockWatch_Movement_Faint
	applymovement LOCALID_PEWTER_GYM_MON_1, RR_BrockWatch_Movement_Faint
	waitmovement LOCALID_PEWTER_GYM_MON_1
	removeobject LOCALID_PEWTER_GYM_MON_0
	removeobject LOCALID_PEWTER_GYM_MON_1
	removeobject LOCALID_PEWTER_GYM_MON_2
	removeobject LOCALID_PEWTER_GYM_MON_3
	delay 20
	return

RR_BrockWatch_Movement_FaceRight:
	face_right
	step_end

RR_BrockWatch_Movement_Up3:
	walk_up
	walk_up
	walk_up
	step_end

RR_BrockWatch_Movement_LungeUp:
	jump_in_place_up
	step_end

RR_BrockWatch_Movement_LungeDown:
	jump_in_place_down
	step_end

RR_BrockWatch_Movement_Shake:
	walk_in_place_fast_left
	walk_in_place_fast_right
	step_end

RR_BrockWatch_Movement_Faint:
	face_down
	delay_8
	set_invisible
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

RR_BrockWatch_Text_Brock_Welcome:
	.string "Brock: Elm and Jenny. The Pewter\n"
	.string "Gym accepts your challenge.$"

RR_BrockWatch_Text_Elm_Ready:
	.string "Elm: For science! And for a badge!\n"
	.string "Mostly the badge!$"

RR_BrockWatch_Text_Elm_Lecture:
	.string "Elm: Did you know Onix used to\n"
	.string "evolve only by trading? Now you\l"
	.string "just-$"

RR_BrockWatch_Text_Brock_Lectured:
	.string "Brock: …Are you lecturing me in my\n"
	.string "own Gym?$"

RR_BrockWatch_Text_Jenny_Hello:
	.string "Jenny: He lectures everyone, dear.\n"
	.string "It's how he says hello.$"

RR_BrockWatch_Text_Ivy_Eggs:
	.string "Ivy: How long till he mentions\n"
	.string "Eggs? Ten seconds.$"

RR_BrockWatch_Text_P_Eggs:
	.string "{PLAYER}: Five.$"

RR_BrockWatch_Text_Brock_Eggs:
	.string "Brock: Funny, breeding Geodude\n"
	.string "taught me… the Eggs are so-$"

RR_BrockWatch_Text_P_Four:
	.string "{PLAYER}: Four. I win, sweetie.$"

RR_BrockWatch_Text_SuperEffective:
	.string "It's super effective!$"

RR_BrockWatch_Text_Jenny_Evidence:
	.string "Jenny: The evidence suggests Rock\n"
	.string "types don't care for that.$"

RR_BrockWatch_Text_Elm_MyObservation:
	.string "Elm: That was MY observation!$"

RR_BrockWatch_Text_Jenny_CaseClosed:
	.string "Jenny: Case closed, dear.$"

RR_BrockWatch_Text_Elm_Fossils:
	.string "Elm: LIVING FOSSILS! In a GYM!\n"
	.string "Jenny, I think I'm crying!$"

RR_BrockWatch_Text_Jenny_Command:
	.string "Jenny: Dear. Command.$"

RR_BrockWatch_Text_Ivy_Ask:
	.string "Ivy: Bet he asks Brock where he\n"
	.string "got them. Mid-battle.$"

RR_BrockWatch_Text_P_Ask:
	.string "{PLAYER}: That's not a bet. That's\n"
	.string "a certainty.$"

RR_BrockWatch_Text_Elm_WhereDidYou:
	.string "Elm: Brock! Where did you get\n"
	.string "them? Which museum? Which dig?$"

RR_BrockWatch_Text_Brock_Museum:
	.string "Brock: The Museum. After the\n"
	.string "battle. Please.$"

RR_BrockWatch_Text_Jenny_Command2:
	.string "Jenny: Dear. Command.$"

RR_BrockWatch_Text_Elm_Bigger:
	.string "Elm: It's so much… bigger in\n"
	.string "person.$"

RR_BrockWatch_Text_Jenny_Command3:
	.string "Jenny: Dear. Command.$"

RR_BrockWatch_Text_Elm_ForScience:
	.string "Elm: Right! For science!$"

RR_BrockWatch_Text_Bind:
	.string "Onix wraps around Cruller.\n"
	.string "Cruller doesn't budge.$"

RR_BrockWatch_Text_P_Composure:
	.string "{PLAYER}: (Cruller has the\n"
	.string "composure of someone who's done\l"
	.string "a lot of paperwork.)$"

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
