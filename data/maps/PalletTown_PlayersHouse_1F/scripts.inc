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
