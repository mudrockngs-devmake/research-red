PalletTown_PlayersHouse_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PalletTown_PlayersHouse_1F_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, PalletTown_PlayersHouse_1F_OnFrame
	.byte 0

@ Ivy is gone once breakfast has been seen
PalletTown_PlayersHouse_1F_OnTransition::
	call_if_ge VAR_0x40FE, 1, PalletTown_PlayersHouse_1F_EventScript_HideIvy
	end

PalletTown_PlayersHouse_1F_EventScript_HideIvy::
	setflag FLAG_TEMP_1
	return

PalletTown_PlayersHouse_1F_OnFrame::
	map_script_2 VAR_0x40FE, 0, PalletTown_PlayersHouse_1F_EventScript_Breakfast
	.2byte 0

@ ---------------------------------------------------------------
@ Research Red: breakfast cutscene (VAR_0x40FE 0 -> 1)
@ ---------------------------------------------------------------
PalletTown_PlayersHouse_1F_EventScript_Breakfast::
	lockall
	applymovement OBJ_EVENT_ID_PLAYER, PalletTown_PlayersHouse_1F_Movement_PlayerToTable
	waitmovement 0
	applymovement LOCALID_MOM, PalletTown_PlayersHouse_1F_Movement_FaceDown
	waitmovement 0
	delay 15
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox PalletTown_PlayersHouse_1F_Text_Delia_GoodMorning, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, PalletTown_PlayersHouse_1F_Movement_FaceLeft
	applymovement LOCALID_PALLET_HOUSE_IVY, PalletTown_PlayersHouse_1F_Movement_FaceRight
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_P_Ferry, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox PalletTown_PlayersHouse_1F_Text_Ivy_Gerald, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_P_Respect, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox PalletTown_PlayersHouse_1F_Text_Ivy_ThrewUp, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_P_Sacrifice, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, PalletTown_PlayersHouse_1F_Movement_FaceUp
	waitmovement 0
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox PalletTown_PlayersHouse_1F_Text_Delia_Pancakes, MSGBOX_DEFAULT
	msgbox PalletTown_PlayersHouse_1F_Text_Ivy_Dangerous, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, PalletTown_PlayersHouse_1F_Movement_FaceLeft
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_P_Authorities, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, PalletTown_PlayersHouse_1F_Movement_FaceUp
	applymovement LOCALID_PALLET_HOUSE_IVY, PalletTown_PlayersHouse_1F_Movement_FaceUp
	waitmovement 0
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox PalletTown_PlayersHouse_1F_Text_Delia_OakCalled, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PALLET_HOUSE_IVY, PalletTown_PlayersHouse_1F_Movement_FaceRight
	waitmovement 0
	msgbox PalletTown_PlayersHouse_1F_Text_Ivy_CertainSomeone, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_P_LoudPassion, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox PalletTown_PlayersHouse_1F_Text_Delia_SixAM, MSGBOX_DEFAULT
	msgbox PalletTown_PlayersHouse_1F_Text_Ivy_Competition, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_P_PeakBeforeLunch, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox PalletTown_PlayersHouse_1F_Text_Delia_EatUp, MSGBOX_DEFAULT
	closemessage
	@ Mimey sweeps past
	applymovement LOCALID_PALLET_HOUSE_MIMEY, PalletTown_PlayersHouse_1F_Movement_MimeySweep
	delay 20
	applymovement OBJ_EVENT_ID_PLAYER, PalletTown_PlayersHouse_1F_Movement_FaceDown
	applymovement LOCALID_PALLET_HOUSE_IVY, PalletTown_PlayersHouse_1F_Movement_FaceDown
	waitmovement LOCALID_PALLET_HOUSE_MIMEY
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_P_PayingRent, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, PalletTown_PlayersHouse_1F_Movement_FaceUp
	waitmovement 0
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox PalletTown_PlayersHouse_1F_Text_Delia_Family, MSGBOX_DEFAULT
	closemessage
	@ Ivy elbows you
	applymovement LOCALID_PALLET_HOUSE_IVY, PalletTown_PlayersHouse_1F_Movement_IvyElbow
	waitmovement 0
	msgbox PalletTown_PlayersHouse_1F_Text_Ivy_BeNice, MSGBOX_DEFAULT
	closemessage
	delay 30
	msgbox PalletTown_PlayersHouse_1F_Text_Delia_Supervised, MSGBOX_DEFAULT
	closemessage
	@ Mimey waves off the credit with a little bow
	applymovement LOCALID_PALLET_HOUSE_MIMEY, PalletTown_PlayersHouse_1F_Movement_MimeyBow
	waitmovement 0
	textcolor NPC_TEXT_COLOR_MON
	msgbox PalletTown_PlayersHouse_1F_Text_Mimey_MimeMime, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, PalletTown_PlayersHouse_1F_Movement_FaceLeft
	applymovement LOCALID_PALLET_HOUSE_IVY, PalletTown_PlayersHouse_1F_Movement_FaceRight
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_P_WritingThisDown, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox PalletTown_PlayersHouse_1F_Text_Ivy_PublishLater, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, PalletTown_PlayersHouse_1F_Movement_FaceUp
	waitmovement 0
	msgbox PalletTown_PlayersHouse_1F_Text_Delia_DoorOpen, MSGBOX_DEFAULT
	closemessage
	@ Ivy heads out first (she reappears behind you outside)
	applymovement LOCALID_PALLET_HOUSE_IVY, PalletTown_PlayersHouse_1F_Movement_IvyExit
	waitmovement 0
	playse SE_EXIT
	removeobject LOCALID_PALLET_HOUSE_IVY
	applymovement LOCALID_MOM, PalletTown_PlayersHouse_1F_Movement_FaceLeft
	waitmovement 0
	setvar VAR_0x40FE, 1
	releaseall
	end

@ Delia after breakfast
PalletTown_PlayersHouse_1F_EventScript_Mom::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_RIVAL_IN_OAKS_LAB, PalletTown_PlayersHouse_1F_EventScript_MomHeal
	msgbox PalletTown_PlayersHouse_1F_Text_Delia_AfterBreakfast
	closemessage
	applymovement LOCALID_MOM, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

PalletTown_PlayersHouse_1F_EventScript_MomHeal::
	msgbox PalletTown_PlayersHouse_1F_Text_YouShouldTakeQuickRest
	closemessage
	call EventScript_OutOfCenterPartyHeal
	msgbox PalletTown_PlayersHouse_1F_Text_LookingGreatTakeCare
	release
	end

PalletTown_PlayersHouse_1F_EventScript_Mimey::
	lock
	faceplayer
	textcolor NPC_TEXT_COLOR_MON
	msgbox PalletTown_PlayersHouse_1F_Text_Mimey_Talk
	closemessage
	release
	end

@ Displays special text if interacted with from side or back (which are normally inaccessible)
PalletTown_PlayersHouse_1F_EventScript_TV::
	lockall
	goto_if_eq VAR_FACING, DIR_NORTH, PalletTown_PlayersHouse_1F_EventScript_TVScreen
	msgbox PalletTown_PlayersHouse_1F_Text_OopsWrongSide
	releaseall
	end

PalletTown_PlayersHouse_1F_EventScript_TVScreen::
	checkplayergender
	call_if_eq VAR_RESULT, MALE, PalletTown_PlayersHouse_1F_EventScript_TVScreenMale
	call_if_eq VAR_RESULT, FEMALE, PalletTown_PlayersHouse_1F_EventScript_TVScreenFemale
	releaseall
	end

PalletTown_PlayersHouse_1F_EventScript_TVScreenMale::
	msgbox PalletTown_PlayersHouse_1F_Text_MovieOnTVFourBoysOnRailroad
	return

PalletTown_PlayersHouse_1F_EventScript_TVScreenFemale::
	msgbox PalletTown_PlayersHouse_1F_Text_MovieOnTVGirlOnBrickRoad
	return

@ ---------------------------------------------------------------
@ Movements
@ ---------------------------------------------------------------
PalletTown_PlayersHouse_1F_Movement_PlayerToTable:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	face_up
	step_end

PalletTown_PlayersHouse_1F_Movement_FaceUp:
	face_up
	step_end

PalletTown_PlayersHouse_1F_Movement_FaceDown:
	face_down
	step_end

PalletTown_PlayersHouse_1F_Movement_FaceLeft:
	face_left
	step_end

PalletTown_PlayersHouse_1F_Movement_FaceRight:
	face_right
	step_end

PalletTown_PlayersHouse_1F_Movement_MimeySweep:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	face_right
	step_end

PalletTown_PlayersHouse_1F_Movement_IvyElbow:
	face_right
	walk_in_place_fast_right
	step_end

PalletTown_PlayersHouse_1F_Movement_MimeyBow:
	face_down
	delay_16
	face_right
	step_end

PalletTown_PlayersHouse_1F_Movement_IvyExit:
	walk_down
	walk_left
	walk_down
	step_end
