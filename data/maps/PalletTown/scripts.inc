.equ SIGN_LADY_READY, VAR_TEMP_2

PalletTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PalletTown_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, PalletTown_OnFrame
	.byte 0

PalletTown_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_PALLET_TOWN
	call PalletTown_EventScript_SetupGrassHookActors
	call_if_set FLAG_PALLET_LADY_NOT_BLOCKING_SIGN, PalletTown_EventScript_TryReadySignLady
	call_if_eq VAR_MAP_SCENE_PALLET_TOWN_SIGN_LADY, 0, PalletTown_EventScript_SetSignLadyPos
	call_if_eq VAR_MAP_SCENE_PALLET_TOWN_SIGN_LADY, 1, PalletTown_EventScript_SetSignLadyDone
	end

PalletTown_EventScript_TryReadySignLady::
	goto_if_unset FLAG_OPENED_START_MENU, EventScript_Return
	goto_if_ge VAR_MAP_SCENE_PALLET_TOWN_SIGN_LADY, 1, EventScript_Return
	setvar VAR_MAP_SCENE_PALLET_TOWN_SIGN_LADY, 1
	return

PalletTown_EventScript_SetSignLadyDone::
	setvar VAR_MAP_SCENE_PALLET_TOWN_SIGN_LADY, 2
	return

PalletTown_EventScript_SetSignLadyPos::
	goto_if_set FLAG_PALLET_LADY_NOT_BLOCKING_SIGN, PalletTown_EventScript_MoveSignLadyToRouteEntrance
	setobjectxyperm LOCALID_PALLET_SIGN_LADY, 5, 15
	setobjectmovementtype LOCALID_PALLET_SIGN_LADY, MOVEMENT_TYPE_FACE_UP
	return

PalletTown_EventScript_MoveSignLadyToRouteEntrance::
	setobjectxyperm LOCALID_PALLET_SIGN_LADY, 12, 2
	setobjectmovementtype LOCALID_PALLET_SIGN_LADY, MOVEMENT_TYPE_FACE_DOWN
	setvar SIGN_LADY_READY, TRUE
	return

PalletTown_OnFrame::
	map_script_2 VAR_0x40FE, 1, PalletTown_EventScript_GrassHook
	map_script_2 VAR_MAP_SCENE_PALLET_TOWN_OAK, 2, PalletTown_EventScript_OakRatingScene
	.2byte 0

@ Oak approaches player after exiting their house post Elite Four to check if they should receieve the National Dex
PalletTown_EventScript_OakRatingScene::
	lockall
	textcolor NPC_TEXT_COLOR_NEUTRAL
	setobjectxyperm LOCALID_PALLET_PROF_OAK, 14, 14
	setobjectmovementtype LOCALID_PALLET_PROF_OAK, MOVEMENT_TYPE_FACE_LEFT
	addobject LOCALID_PALLET_PROF_OAK
	applymovement LOCALID_PALLET_PROF_OAK, PalletTown_Movement_OakWalkToPlayersDoor
	waitmovement 0
	goto_if_unset FLAG_WORLD_MAP_ONE_ISLAND, PalletTown_EventScript_NotBeenToOneIslandYet
	msgbox PalletTown_Text_OakLetMeSeePokedex
	setvar VAR_0x8004, 0
	specialvar VAR_RESULT, GetPokedexCount
	copyvar VAR_0x8008, VAR_0x8005
	copyvar VAR_0x8009, VAR_0x8006
	buffernumberstring STR_VAR_1, VAR_0x8008
	buffernumberstring STR_VAR_2, VAR_0x8009
	goto_if_lt VAR_0x8009, 60, PalletTown_EventScript_NotEnoughMonsForNationalDex
	msgbox PalletTown_Text_CaughtXImpressiveFollowMe
	closemessage
	playbgm MUS_FOLLOW_ME, 0
	applymovement LOCALID_PLAYER, PalletTown_Movement_PlayerWalkToLabFromHouse
	applymovement LOCALID_PALLET_PROF_OAK, PalletTown_Movement_OakWalkToLabFromHouse
	waitmovement 0
	opendoor 16, 13
	waitdooranim
	applymovement LOCALID_PALLET_PROF_OAK, PalletTown_Movement_OakEnterLab
	applymovement LOCALID_PLAYER, PalletTown_Movement_PlayerEnterLab
	waitmovement 0
	closedoor 16, 13
	waitdooranim
	setvar VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB, 7
	setvar VAR_MAP_SCENE_PALLET_TOWN_OAK, 3
	setflag FLAG_HIDE_OAK_IN_PALLET_TOWN
	warp MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB, 6, 12
	waitstate
	releaseall
	end

PalletTown_EventScript_EndOakRatingScene::
	closemessage
	applymovement LOCALID_PALLET_PROF_OAK, PalletTown_Movement_OakExit
	waitmovement 0
	removeobject LOCALID_PALLET_PROF_OAK
	setvar VAR_MAP_SCENE_PALLET_TOWN_OAK, 3
	releaseall
	end

PalletTown_EventScript_NotEnoughMonsForNationalDex::
	msgbox PalletTown_Text_CaughtXPuttingInHonestEffort
	goto PalletTown_EventScript_EndOakRatingScene
	end

PalletTown_EventScript_NotBeenToOneIslandYet::
	msgbox PalletTown_Text_OakYouEnjoyingTraveling
	goto PalletTown_EventScript_EndOakRatingScene
	end

PalletTown_Movement_OakWalkToPlayersDoor::
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

PalletTown_Movement_OakExit::
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
	walk_right
	walk_right
	walk_right
	step_end

PalletTown_Movement_OakWalkToLabFromHouse::
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
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_up
	step_end

PalletTown_Movement_PlayerWalkToLabFromHouse::
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
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

PalletTown_EventScript_OakTriggerLeft::
	lockall
	setvar VAR_TEMP_1, 0
	goto PalletTown_EventScript_OakTrigger
	end

PalletTown_EventScript_OakTriggerRight::
	lockall
	setvar VAR_TEMP_1, 1
	goto PalletTown_EventScript_OakTrigger
	end

PalletTown_EventScript_OakTrigger::
	famechecker FAMECHECKER_OAK, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	textcolor NPC_TEXT_COLOR_NEUTRAL
	delay 30
	playbgm MUS_OAK, 0
	message PalletTown_Text_OakDontGoOut
	waitmessage
	delay 85
	closemessage
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_PLAYER, Common_Movement_ExclamationMark
	waitmovement 0
	delay 30
	addobject LOCALID_PALLET_PROF_OAK
	call_if_eq VAR_TEMP_1, 0, PalletTown_EventScript_OakEnterLeft
	call_if_eq VAR_TEMP_1, 1, PalletTown_EventScript_OakEnterRight
	delay 30
	msgbox PalletTown_Text_OakGrassUnsafeNeedMon
	closemessage
	delay 30
	call_if_eq VAR_TEMP_1, 0, PalletTown_EventScript_OakLeadPlayerToLabLeft
	call_if_eq VAR_TEMP_1, 1, PalletTown_EventScript_OakLeadPlayerToLabRight
	opendoor 16, 13
	waitdooranim
	applymovement LOCALID_PALLET_PROF_OAK, PalletTown_Movement_OakEnterLab
	applymovement LOCALID_PLAYER, PalletTown_Movement_PlayerEnterLab
	waitmovement 0
	closedoor 16, 13
	waitdooranim
	setvar VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB, 1
	clearflag FLAG_HIDE_OAK_IN_HIS_LAB
	setvar VAR_MAP_SCENE_PALLET_TOWN_OAK, 1
	setflag FLAG_HIDE_OAK_IN_PALLET_TOWN
	setflag FLAG_DONT_TRANSITION_MUSIC
	warp MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB, 6, 12
	waitstate
	releaseall
	end

PalletTown_EventScript_OakEnterLeft::
	applymovement LOCALID_PALLET_PROF_OAK, PalletTown_Movement_OakEnterLeft
	waitmovement 0
	return

PalletTown_EventScript_OakEnterRight::
	applymovement LOCALID_PALLET_PROF_OAK, PalletTown_Movement_OakEnterRight
	waitmovement 0
	return

PalletTown_EventScript_OakLeadPlayerToLabLeft::
	applymovement LOCALID_PALLET_PROF_OAK, PalletTown_Movement_OakWalkToLabLeft
	applymovement LOCALID_PLAYER, PalletTown_Movement_PlayerWalkToLabLeft
	waitmovement 0
	return

PalletTown_EventScript_OakLeadPlayerToLabRight::
	applymovement LOCALID_PALLET_PROF_OAK, PalletTown_Movement_OakWalkToLabRight
	applymovement LOCALID_PLAYER, PalletTown_Movement_PlayerWalkToLabRight
	waitmovement 0
	return

PalletTown_Movement_OakEnterLeft::
	walk_up
	walk_up
	walk_right
	walk_up
	walk_up
	walk_right
	walk_up
	walk_up
	step_end

PalletTown_Movement_OakEnterRight::
	walk_right
	walk_up
	walk_up
	walk_right
	walk_up
	walk_up
	walk_right
	walk_up
	walk_up
	step_end

	.macro walk_to_lab
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	.endm

PalletTown_Movement_OakWalkToLabLeft::
	walk_down
	walk_to_lab
	walk_right
	walk_in_place_faster_up
	step_end

PalletTown_Movement_OakWalkToLabRight::
	walk_down
	walk_left
	walk_to_lab
	walk_right
	walk_in_place_faster_up
	step_end

PalletTown_Movement_OakEnterLab::
	walk_up
	set_invisible
	step_end

PalletTown_Movement_PlayerWalkToLabLeft::
	walk_down
	walk_down
	walk_to_lab
	step_end

PalletTown_Movement_PlayerWalkToLabRight::
	walk_down
	walk_down
	walk_left
	walk_to_lab
	step_end

PalletTown_Movement_PlayerEnterLab::
	walk_right
	walk_up
	set_invisible
	step_end

PalletTown_EventScript_SignLady::
	lock
	goto_if_eq VAR_MAP_SCENE_PALLET_TOWN_SIGN_LADY, 2, PalletTown_EventScript_SignLadyDone
	goto_if_eq VAR_MAP_SCENE_PALLET_TOWN_SIGN_LADY, 1, PalletTown_EventScript_SignLadyJustShowedSign
	goto_if_eq SIGN_LADY_READY, TRUE, PalletTown_EventScript_SignLadyStartShowSign
	goto_if_set FLAG_TEMP_2, PalletTown_EventScript_SignLadyGoReadSign
	msgbox PalletTown_Text_HmmIsThatRight
	applymovement LOCALID_PALLET_SIGN_LADY, Common_Movement_FacePlayer
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_PALLET_SIGN_LADY, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_PALLET_SIGN_LADY, Common_Movement_Delay48
	waitmovement 0
	msgbox PalletTown_Text_OhLookLook
	closemessage
	call_if_eq VAR_FACING, DIR_EAST, PalletTown_EventScript_SignLadyMoveOutOfWayRight
	call_if_ne VAR_FACING, DIR_EAST, PalletTown_EventScript_SignLadyMoveOutOfWayLeft
	copyobjectxytoperm LOCALID_PALLET_SIGN_LADY
	setflag FLAG_TEMP_2
	release
	end

PalletTown_EventScript_SignLadyMoveOutOfWayRight::
	applymovement LOCALID_PALLET_SIGN_LADY, PalletTown_Movement_SignLadyMoveOutOfWayRight
	waitmovement 0
	return

PalletTown_EventScript_SignLadyMoveOutOfWayLeft::
	applymovement LOCALID_PALLET_SIGN_LADY, PalletTown_Movement_SignLadyMoveOutOfWayLeft
	waitmovement 0
	return

PalletTown_EventScript_SignLadyDone::
	applymovement LOCALID_PALLET_SIGN_LADY, Common_Movement_FacePlayer
	waitmovement 0
	msgbox PalletTown_Text_RaisingMonsToo
	release
	end

PalletTown_EventScript_SignLadyGoReadSign::
	applymovement LOCALID_PALLET_SIGN_LADY, Common_Movement_FacePlayer
	waitmovement 0
	msgbox PalletTown_Text_ReadItReadIt
	release
	end

PalletTown_EventScript_SignLadyJustShowedSign::
	applymovement LOCALID_PALLET_SIGN_LADY, Common_Movement_FacePlayer
	waitmovement 0
	msgbox PalletTown_Text_SignsAreUsefulArentThey
	release
	end

PalletTown_Movement_SignLadyMoveOutOfWayRight::
	walk_right
	walk_in_place_faster_left
	step_end

PalletTown_Movement_SignLadyMoveOutOfWayLeft::
	walk_left
	walk_in_place_faster_right
	step_end

PalletTown_EventScript_FatMan::
	msgbox PalletTown_Text_CanStoreItemsAndMonsInPC, MSGBOX_NPC
	end

PalletTown_EventScript_OaksLabSign::
	lockall
	famechecker FAMECHECKER_OAK, 0
	msgbox PalletTown_Text_OakPokemonResearchLab
	releaseall
	end

PalletTown_EventScript_PlayersHouseSign::
	msgbox PalletTown_Text_PlayersHouse, MSGBOX_SIGN
	end

PalletTown_EventScript_RivalsHouseSign::
	msgbox PalletTown_Text_RivalsHouse, MSGBOX_SIGN
	end

PalletTown_EventScript_TownSign::
	msgbox PalletTown_Text_TownSign, MSGBOX_SIGN
	end

PalletTown_EventScript_TrainerTips::
	lockall
	msgbox PalletTown_Text_PressStartToOpenMenu
	setvar VAR_MAP_SCENE_PALLET_TOWN_SIGN_LADY, 1
	releaseall
	end

PalletTown_EventScript_SignLadyTrigger::
	lockall
	applymovement LOCALID_PALLET_SIGN_LADY, Common_Movement_WalkInPlaceFasterRight
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	call PalletTown_EventScript_SignLadyShowSign
	releaseall
	end

PalletTown_EventScript_SignLadyShowSign::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_Text_LookCopiedTrainerTipsSign
	closemessage
	delay 20
	textcolor NPC_TEXT_COLOR_NEUTRAL
	setflag FLAG_OPENED_START_MENU
	setvar VAR_MAP_SCENE_PALLET_TOWN_SIGN_LADY, 1
	setvar SIGN_LADY_READY, FALSE
	special SetWalkingIntoSignVars
	special DisableMsgBoxWalkaway
	signmsg
	msgbox PalletTown_Text_PressStartToOpenMenuCopy
	normalmsg
	return

PalletTown_EventScript_SignLadyStartShowSign::
	applymovement LOCALID_PALLET_SIGN_LADY, Common_Movement_FacePlayer
	waitmovement 0
	call PalletTown_EventScript_SignLadyShowSign
	release
	end

@ ---------------------------------------------------------------
@ Research Red: grass hook cutscene (VAR_0x40FE 1 -> 2)
@ ---------------------------------------------------------------
PalletTown_EventScript_SetupGrassHookActors::
	setflag FLAG_TEMP_4
	call_if_ne VAR_0x40FE, 1, PalletTown_EventScript_HideElmJenny
	return

PalletTown_EventScript_HideElmJenny::
	setflag FLAG_TEMP_3
	return

PalletTown_EventScript_GrassHook::
	lockall
	applymovement LOCALID_PLAYER, PalletTown_RR_Movement_StepDown
	waitmovement 0
	opendoor 6, 7
	waitdooranim
	addobject LOCALID_PALLET_IVY
	applymovement LOCALID_PALLET_IVY, PalletTown_RR_Movement_StepDown
	waitmovement 0
	closedoor 6, 7
	waitdooranim
	applymovement LOCALID_PLAYER, PalletTown_RR_Movement_FaceUp
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_Text_RR_Ivy_StopByDaisys, MSGBOX_DEFAULT
	closemessage
	delay 20
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_Text_RR_Elm_RouteOne, MSGBOX_DEFAULT
	closemessage
	playse SE_PIN
	applymovement LOCALID_PLAYER, Common_Movement_ExclamationMark
	applymovement LOCALID_PALLET_IVY, Common_Movement_ExclamationMark
	waitmovement 0
	@ Camera pans north to the tall grass
	special SpawnCameraObject
	applymovement LOCALID_CAMERA, PalletTown_RR_Movement_CameraToGrass
	waitmovement 0
	delay 30
	applymovement LOCALID_PALLET_ELM, PalletTown_RR_Movement_ElmSwitchLegs
	waitmovement 0
	delay 20
	applymovement LOCALID_CAMERA, PalletTown_RR_Movement_CameraBack
	waitmovement 0
	special RemoveCameraObject
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_Text_RR_P_FollowTheYelling, MSGBOX_DEFAULT
	closemessage
	@ Walk up to Elm and Jenny, Ivy one step behind
	applymovement LOCALID_PALLET_IVY, PalletTown_RR_Movement_IvyToGrass
	applymovement LOCALID_PLAYER, PalletTown_RR_Movement_PlayerToGrass
	waitmovement 0
	applymovement LOCALID_PALLET_IVY, PalletTown_RR_Movement_FaceRight
	applymovement LOCALID_PALLET_ELM, PalletTown_RR_Movement_FaceDown
	applymovement LOCALID_PALLET_JENNY, PalletTown_RR_Movement_FaceLeft
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_Text_RR_Elm_JustInTime, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_Text_RR_Jenny_Stretching, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_Text_RR_Elm_ReturnFavor, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_Text_RR_Ivy_StraightRoad, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_Text_RR_Elm_Curves, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_Text_RR_Jenny_Support, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_Text_RR_P_WhichPokemon, MSGBOX_DEFAULT
	closemessage
	@ Elm pats his pockets
	applymovement LOCALID_PALLET_ELM, PalletTown_RR_Movement_ElmPatPockets
	waitmovement 0
	delay 30
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_Text_RR_Elm_Transferred, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_Text_RR_P_FightTheGrass, MSGBOX_DEFAULT
	closemessage
	@ Elm steps toward the grass
	applymovement LOCALID_PALLET_ELM, PalletTown_RR_Movement_ElmStepToGrass
	waitmovement 0
	delay 15
	playbgm MUS_OAK, 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_Text_OakDontGoOut, MSGBOX_DEFAULT
	closemessage
	playse SE_PIN
	applymovement LOCALID_PALLET_ELM, Common_Movement_ExclamationMark
	applymovement LOCALID_PLAYER, PalletTown_RR_Movement_FaceDown
	applymovement LOCALID_PALLET_IVY, PalletTown_RR_Movement_FaceDown
	applymovement LOCALID_PALLET_JENNY, PalletTown_RR_Movement_FaceDown
	waitmovement 0
	@ Oak hurries over
	addobject LOCALID_PALLET_PROF_OAK
	applymovement LOCALID_PALLET_PROF_OAK, PalletTown_RR_Movement_OakArrive
	applymovement LOCALID_PALLET_ELM, PalletTown_RR_Movement_ElmBackDown
	waitmovement LOCALID_PALLET_PROF_OAK
	applymovement LOCALID_PLAYER, PalletTown_RR_Movement_FaceRight
	waitmovement 0
	delay 15
	msgbox PalletTown_Text_RR_Oak_WildMons, MSGBOX_DEFAULT
	msgbox PalletTown_Text_RR_Elm_Observing, MSGBOX_DEFAULT
	msgbox PalletTown_Text_RR_Oak_OfficerJenny, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_Text_RR_Jenny_WeAllGetThat, MSGBOX_DEFAULT
	closemessage
	delay 20
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_Text_RR_Oak_Haiku, MSGBOX_DEFAULT
	closemessage
	delay 20
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_Text_RR_Ivy_Poems, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_Text_RR_Oak_ComeToLab, MSGBOX_DEFAULT
	closemessage
	@ Everyone heads to the lab. Jenny trails Elm.
	applymovement LOCALID_PALLET_PROF_OAK, PalletTown_RR_Movement_OakToLab
	applymovement LOCALID_PALLET_IVY, PalletTown_RR_Movement_IvyToLab
	applymovement LOCALID_PLAYER, PalletTown_RR_Movement_PlayerToLab
	applymovement LOCALID_PALLET_ELM, PalletTown_RR_Movement_ElmToLab
	applymovement LOCALID_PALLET_JENNY, PalletTown_RR_Movement_JennyToLab
	waitmovement LOCALID_PALLET_JENNY
	opendoor 16, 13
	waitdooranim
	applymovement LOCALID_PALLET_PROF_OAK, PalletTown_RR_Movement_OakIntoLab
	applymovement LOCALID_PALLET_IVY, PalletTown_RR_Movement_IvyIntoLab
	applymovement LOCALID_PLAYER, PalletTown_RR_Movement_PlayerIntoLab
	applymovement LOCALID_PALLET_ELM, PalletTown_RR_Movement_ElmIntoLab
	applymovement LOCALID_PALLET_JENNY, PalletTown_RR_Movement_JennyIntoLab
	waitmovement LOCALID_PALLET_JENNY
	closedoor 16, 13
	waitdooranim
	setvar VAR_0x40FE, 2
	@ Research Red: 10 = our lab scene (started by a floor trigger in the lab map)
	setvar VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB, 10
	clearflag FLAG_HIDE_OAK_IN_HIS_LAB
	setvar VAR_MAP_SCENE_PALLET_TOWN_OAK, 1
	setflag FLAG_HIDE_OAK_IN_PALLET_TOWN
	setflag FLAG_DONT_TRANSITION_MUSIC
	warp MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB, 6, 12
	waitstate
	releaseall
	end

@ ---------------------------------------------------------------
@ Research Red: grass hook movements
@ ---------------------------------------------------------------
PalletTown_RR_Movement_StepDown:
	walk_down
	step_end

PalletTown_RR_Movement_FaceUp:
	face_up
	step_end

PalletTown_RR_Movement_FaceDown:
	face_down
	step_end

PalletTown_RR_Movement_FaceLeft:
	face_left
	step_end

PalletTown_RR_Movement_FaceRight:
	face_right
	step_end

PalletTown_RR_Movement_CameraToGrass:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

PalletTown_RR_Movement_CameraBack:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

PalletTown_RR_Movement_ElmSwitchLegs:
	face_left
	delay_16
	face_right
	delay_16
	face_left
	delay_16
	face_up
	step_end

PalletTown_RR_Movement_PlayerToGrass:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_up
	step_end

PalletTown_RR_Movement_IvyToGrass:
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

PalletTown_RR_Movement_ElmPatPockets:
	face_left
	delay_8
	face_right
	delay_8
	face_left
	delay_8
	face_right
	delay_16
	face_down
	step_end

PalletTown_RR_Movement_ElmStepToGrass:
	face_up
	delay_8
	walk_up
	step_end

PalletTown_RR_Movement_ElmBackDown:
	delay_16
	delay_16
	walk_down
	face_down
	step_end

PalletTown_RR_Movement_OakArrive:
	walk_up
	walk_up
	walk_right
	walk_up
	walk_up
	walk_right
	walk_right
	walk_up
	walk_up
	face_left
	step_end

PalletTown_RR_Movement_OakToLab:
	walk_down
	walk_down
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	face_up
	step_end

PalletTown_RR_Movement_IvyToLab:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

PalletTown_RR_Movement_PlayerToLab:
	delay_16
	delay_16
	delay_16
	delay_16
	walk_down
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	step_end

PalletTown_RR_Movement_ElmToLab:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_down
	walk_down
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	step_end

PalletTown_RR_Movement_JennyToLab:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_left
	walk_down
	walk_down
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	step_end

PalletTown_RR_Movement_OakIntoLab:
	walk_up
	set_invisible
	step_end

PalletTown_RR_Movement_IvyIntoLab:
	delay_16
	walk_right
	walk_up
	set_invisible
	step_end

PalletTown_RR_Movement_PlayerIntoLab:
	delay_16
	delay_16
	walk_right
	walk_right
	walk_up
	set_invisible
	step_end

PalletTown_RR_Movement_ElmIntoLab:
	delay_16
	delay_16
	delay_16
	walk_right
	walk_right
	walk_right
	walk_up
	set_invisible
	step_end

PalletTown_RR_Movement_JennyIntoLab:
	delay_16
	delay_16
	delay_16
	delay_16
	walk_right
	walk_right
	walk_right
	walk_right
	walk_up
	set_invisible
	step_end


@ ===================================================================
@ Research Red: Oak's lab scene
@ Started by floor triggers just inside the lab door when
@ VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB == 10 (set by the grass hook).
@ Kept in this file so the large FireRed lab script stays untouched.
@ Replaces: starter choice, rival battle, Oak's Parcel errand, Pokedex scene.
@ ===================================================================
RR_Lab_EventScript_Scene::
	lockall
	textcolor NPC_TEXT_COLOR_NEUTRAL
	applymovement LOCALID_LAB_IVY, RR_Lab_Movement_Up8
	applymovement LOCALID_OAKS_LAB_RIVAL, RR_Lab_Movement_Up8
	applymovement LOCALID_LAB_JENNY, RR_Lab_Movement_Up6
	applymovement LOCALID_PLAYER, RR_Lab_Movement_Up7
	waitmovement 0
	applymovement LOCALID_OAKS_LAB_PROF_OAK, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	clearflag FLAG_DONT_TRANSITION_MUSIC
	savebgm MUS_DUMMY
	fadedefaultbgm
	@ --- why everyone is here ---
	msgbox RR_Lab_Text_Oak_Welcome, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Oak_WhyHere, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Elm_Mailed, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Jenny_Proofread, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_StudyForms, MSGBOX_DEFAULT
	applymovement LOCALID_LAB_IVY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Ivy_InStarters, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_OrangeIslands, MSGBOX_DEFAULT
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_P_Snacks, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Ivy_StickyNotes, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_P_Delicious, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	applymovement LOCALID_LAB_IVY, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_OtherLands, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Elm_OldTimey, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Jenny_Scroll, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_MidBattle, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Oak_ChooseFirst, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Elm_Reliable, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Ivy_Reliable, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Elm_Bunsen, MSGBOX_DEFAULT
	closemessage
	@ --- choose a starter (works for all three) ---
RR_Lab_EventScript_ChooseLoop::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	showmonpic SPECIES_BULBASAUR, 10, 3
	msgbox RR_Lab_Text_StudyBulbasaur, MSGBOX_YESNO
	hidemonpic
	goto_if_eq VAR_RESULT, YES, RR_Lab_EventScript_PickBulbasaur
	showmonpic SPECIES_SQUIRTLE, 10, 3
	msgbox RR_Lab_Text_StudySquirtle, MSGBOX_YESNO
	hidemonpic
	goto_if_eq VAR_RESULT, YES, RR_Lab_EventScript_PickSquirtle
	showmonpic SPECIES_CHARMANDER, 10, 3
	msgbox RR_Lab_Text_StudyCharmander, MSGBOX_YESNO
	hidemonpic
	goto_if_eq VAR_RESULT, YES, RR_Lab_EventScript_PickCharmander
	goto RR_Lab_EventScript_ChooseLoop

RR_Lab_EventScript_PickBulbasaur::
	setvar VAR_STARTER_MON, 0
	removeobject LOCALID_BULBASAUR_BALL
	givemon SPECIES_BULBASAUR, 5
	bufferspeciesname STR_VAR_1, SPECIES_BULBASAUR
	goto RR_Lab_EventScript_Received

RR_Lab_EventScript_PickSquirtle::
	setvar VAR_STARTER_MON, 1
	removeobject LOCALID_SQUIRTLE_BALL
	givemon SPECIES_SQUIRTLE, 5
	bufferspeciesname STR_VAR_1, SPECIES_SQUIRTLE
	goto RR_Lab_EventScript_Received

RR_Lab_EventScript_PickCharmander::
	setvar VAR_STARTER_MON, 2
	removeobject LOCALID_CHARMANDER_BALL
	givemon SPECIES_CHARMANDER, 5
	bufferspeciesname STR_VAR_1, SPECIES_CHARMANDER
	goto RR_Lab_EventScript_Received

RR_Lab_EventScript_Received::
	setflag FLAG_SYS_POKEMON_GET
	setflag FLAG_PALLET_LADY_NOT_BLOCKING_SIGN
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_OBTAIN_KEY_ITEM
	message RR_Lab_Text_ReceivedStarter
	waitmessage
	waitfanfare
	msgbox Text_GiveNicknameToThisMon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RR_Lab_EventScript_ChainPicks
	setvar VAR_0x8004, 0
	call EventScript_ChangePokemonNickname
RR_Lab_EventScript_ChainPicks::
	closemessage
	@ Ivy takes the Kanto counter, Elm the Johto counter, Jenny counters Elm
	goto_if_eq VAR_STARTER_MON, 0, RR_Lab_EventScript_ChainBulbasaur
	goto_if_eq VAR_STARTER_MON, 1, RR_Lab_EventScript_ChainSquirtle
	goto RR_Lab_EventScript_ChainCharmander

RR_Lab_EventScript_ChainBulbasaur::
	bufferspeciesname STR_VAR_1, SPECIES_CHARMANDER
	call RR_Lab_EventScript_IvyLine
	removeobject LOCALID_CHARMANDER_BALL
	givemon SPECIES_CHARMANDER, 5
	call RR_Lab_EventScript_IvyMonJoins
	bufferspeciesname STR_VAR_1, SPECIES_CYNDAQUIL
	call RR_Lab_EventScript_ElmLine
	removeobject LOCALID_LAB_CYNDAQUIL_BALL
	bufferspeciesname STR_VAR_1, SPECIES_TOTODILE
	call RR_Lab_EventScript_JennyLine
	removeobject LOCALID_LAB_TOTODILE_BALL
	goto RR_Lab_EventScript_AfterPicks

RR_Lab_EventScript_ChainSquirtle::
	bufferspeciesname STR_VAR_1, SPECIES_BULBASAUR
	call RR_Lab_EventScript_IvyLine
	removeobject LOCALID_BULBASAUR_BALL
	givemon SPECIES_BULBASAUR, 5
	call RR_Lab_EventScript_IvyMonJoins
	bufferspeciesname STR_VAR_1, SPECIES_CHIKORITA
	call RR_Lab_EventScript_ElmLine
	removeobject LOCALID_LAB_CHIKORITA_BALL
	bufferspeciesname STR_VAR_1, SPECIES_CYNDAQUIL
	call RR_Lab_EventScript_JennyLine
	removeobject LOCALID_LAB_CYNDAQUIL_BALL
	goto RR_Lab_EventScript_AfterPicks

RR_Lab_EventScript_ChainCharmander::
	bufferspeciesname STR_VAR_1, SPECIES_SQUIRTLE
	call RR_Lab_EventScript_IvyLine
	removeobject LOCALID_SQUIRTLE_BALL
	givemon SPECIES_SQUIRTLE, 5
	call RR_Lab_EventScript_IvyMonJoins
	bufferspeciesname STR_VAR_1, SPECIES_TOTODILE
	call RR_Lab_EventScript_ElmLine
	removeobject LOCALID_LAB_TOTODILE_BALL
	bufferspeciesname STR_VAR_1, SPECIES_CHIKORITA
	call RR_Lab_EventScript_JennyLine
	removeobject LOCALID_LAB_CHIKORITA_BALL
	goto RR_Lab_EventScript_AfterPicks

RR_Lab_EventScript_IvyLine::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Ivy_Picks, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_P_MarriedToIt, MSGBOX_DEFAULT
	closemessage
	return

@ Ivy's partner travels in the player's party (you battle as a team)
RR_Lab_EventScript_IvyMonJoins::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_LEVEL_UP
	message RR_Lab_Text_IvyMonJoins
	waitmessage
	waitfanfare
	closemessage
	return

RR_Lab_EventScript_ElmLine::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Elm_Picks, MSGBOX_DEFAULT
	closemessage
	return

RR_Lab_EventScript_JennyLine::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Jenny_Picks, MSGBOX_DEFAULT
	closemessage
	return

RR_Lab_EventScript_AfterPicks::
	@ --- Ash, Gary, and Elm's big idea ---
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_Overslept, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Elm_Ash, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Oak_Gary, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Ivy_OutThere, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_Postcards, MSGBOX_DEFAULT
	closemessage
	playse SE_PIN
	applymovement LOCALID_OAKS_LAB_RIVAL, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_OAKS_LAB_RIVAL, Common_Movement_WalkInPlaceFasterLeft
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox RR_Lab_Text_Elm_PushIt, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_P_HereItComes, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Elm_GymChallenge, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Oak_Quietly, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Jenny_NotASetting, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Elm_Scared, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_P_Paperwork, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	msgbox RR_Lab_Text_P_AskIvy, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Ivy_Yes, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Elm_AskJenny, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Jenny_Yes, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Ivy_SideBet, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Jenny_Absolutely, MSGBOX_DEFAULT
	closemessage
	@ --- first battle: Elm (team is FireRed's placeholder for now) ---
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playbgm MUS_ENCOUNTER_RIVAL, 0
	msgbox RR_Lab_Text_Elm_TestNow, MSGBOX_DEFAULT
	closemessage
	goto_if_eq VAR_STARTER_MON, 0, RR_Lab_EventScript_BattleCharmander
	goto_if_eq VAR_STARTER_MON, 1, RR_Lab_EventScript_BattleBulbasaur
	goto RR_Lab_EventScript_BattleSquirtle

RR_Lab_EventScript_BattleCharmander::
	trainerbattle_earlyrival TRAINER_RIVAL_OAKS_LAB_CHARMANDER, RIVAL_BATTLE_TUTORIAL, RR_Lab_Text_Elm_Defeat, RR_Lab_Text_Elm_Victory
	goto RR_Lab_EventScript_AfterBattle

RR_Lab_EventScript_BattleBulbasaur::
	trainerbattle_earlyrival TRAINER_RIVAL_OAKS_LAB_BULBASAUR, RIVAL_BATTLE_TUTORIAL, RR_Lab_Text_Elm_Defeat, RR_Lab_Text_Elm_Victory
	goto RR_Lab_EventScript_AfterBattle

RR_Lab_EventScript_BattleSquirtle::
	trainerbattle_earlyrival TRAINER_RIVAL_OAKS_LAB_SQUIRTLE, RIVAL_BATTLE_TUTORIAL, RR_Lab_Text_Elm_Defeat, RR_Lab_Text_Elm_Victory
	goto RR_Lab_EventScript_AfterBattle

RR_Lab_EventScript_AfterBattle::
	special HealPlayerParty
	fadedefaultbgm
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Elm_BestOfFive, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Jenny_BestOfZero, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_Properly, MSGBOX_DEFAULT
	@ --- research grant ---
	msgbox RR_Lab_Text_Oak_Grant, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	call RR_Lab_EventScript_GiveGrant
	playfanfare MUS_OBTAIN_KEY_ITEM
	message RR_Lab_Text_ReceivedGrant
	waitmessage
	waitfanfare
	msgbox RR_Lab_Text_GrantContents, MSGBOX_DEFAULT
	removeobject LOCALID_POKEDEX_1
	removeobject LOCALID_POKEDEX_2
	playfanfare MUS_OBTAIN_KEY_ITEM
	message RR_Lab_Text_ReceivedPokedex
	waitmessage
	waitfanfare
	setflag FLAG_SYS_POKEDEX_GET
	special SetUnlockedPokedexFlags
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_P_LotOfBalls, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Ivy_NotAStore, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Jenny_Receipts, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Elm_SecondGrant, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Oak_SameGrant, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Oak_GoSeeDaisy, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Elm_MeetAtDaisys, MSGBOX_DEFAULT
	closemessage
	@ --- everyone heads out ---
	applymovement LOCALID_OAKS_LAB_RIVAL, RR_Lab_Movement_Down8
	applymovement LOCALID_LAB_JENNY, RR_Lab_Movement_Down7
	waitmovement 0
	removeobject LOCALID_OAKS_LAB_RIVAL
	removeobject LOCALID_LAB_JENNY
	playse SE_EXIT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Ivy_WaitOutside, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_LAB_IVY, RR_Lab_Movement_Down8
	waitmovement 0
	removeobject LOCALID_LAB_IVY
	playse SE_EXIT
	@ --- story flags: the parcel errand is skipped ---
	setflag FLAG_BEAT_RIVAL_IN_OAKS_LAB
	setvar VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB, 6
	setvar VAR_MAP_SCENE_VIRIDIAN_CITY_MART, 2
	setvar VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 1
	setvar VAR_MAP_SCENE_ROUTE22, 1
	setvar VAR_MAP_SCENE_POKEMON_CENTER_TEALA, 1
	@ Viridian's old man stays asleep until you battle Elm on Route 22
	setvar VAR_0x40FE, 3
	releaseall
	end

@ The grant: 100 of each of 13 Poke Balls (FireRed's ball pocket holds 13 kinds),
@ medicine, repels, TM Case and TM01-TM50.
RR_Lab_EventScript_GiveGrant::
	additem ITEM_POKE_BALL, 100
	additem ITEM_GREAT_BALL, 100
	additem ITEM_ULTRA_BALL, 100
	additem ITEM_PREMIER_BALL, 100
	additem ITEM_HEAL_BALL, 100
	additem ITEM_NET_BALL, 100
	additem ITEM_NEST_BALL, 100
	additem ITEM_DIVE_BALL, 100
	additem ITEM_DUSK_BALL, 100
	additem ITEM_TIMER_BALL, 100
	additem ITEM_QUICK_BALL, 100
	additem ITEM_REPEAT_BALL, 100
	additem ITEM_LUXURY_BALL, 100
	additem ITEM_SUPER_POTION, 30
	additem ITEM_HYPER_POTION, 20
	additem ITEM_FULL_RESTORE, 10
	additem ITEM_REVIVE, 15
	additem ITEM_MAX_REVIVE, 5
	additem ITEM_FULL_HEAL, 20
	additem ITEM_ETHER, 10
	additem ITEM_MAX_ETHER, 5
	additem ITEM_MAX_REPEL, 30
	additem ITEM_ESCAPE_ROPE, 10
	additem ITEM_TM_CASE, 1
	additem ITEM_TM01, 1
	additem ITEM_TM02, 1
	additem ITEM_TM03, 1
	additem ITEM_TM04, 1
	additem ITEM_TM05, 1
	additem ITEM_TM06, 1
	additem ITEM_TM07, 1
	additem ITEM_TM08, 1
	additem ITEM_TM09, 1
	additem ITEM_TM10, 1
	additem ITEM_TM11, 1
	additem ITEM_TM12, 1
	additem ITEM_TM13, 1
	additem ITEM_TM14, 1
	additem ITEM_TM15, 1
	additem ITEM_TM16, 1
	additem ITEM_TM17, 1
	additem ITEM_TM18, 1
	additem ITEM_TM19, 1
	additem ITEM_TM20, 1
	additem ITEM_TM21, 1
	additem ITEM_TM22, 1
	additem ITEM_TM23, 1
	additem ITEM_TM24, 1
	additem ITEM_TM25, 1
	additem ITEM_TM26, 1
	additem ITEM_TM27, 1
	additem ITEM_TM28, 1
	additem ITEM_TM29, 1
	additem ITEM_TM30, 1
	additem ITEM_TM31, 1
	additem ITEM_TM32, 1
	additem ITEM_TM33, 1
	additem ITEM_TM34, 1
	additem ITEM_TM35, 1
	additem ITEM_TM36, 1
	additem ITEM_TM37, 1
	additem ITEM_TM38, 1
	additem ITEM_TM39, 1
	additem ITEM_TM40, 1
	additem ITEM_TM41, 1
	additem ITEM_TM42, 1
	additem ITEM_TM43, 1
	additem ITEM_TM44, 1
	additem ITEM_TM45, 1
	additem ITEM_TM46, 1
	additem ITEM_TM47, 1
	additem ITEM_TM48, 1
	additem ITEM_TM49, 1
	additem ITEM_TM50, 1
	return

RR_Lab_Movement_Up6:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

RR_Lab_Movement_Up7:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

RR_Lab_Movement_Up8:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

RR_Lab_Movement_Down7:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

RR_Lab_Movement_Down8:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end
