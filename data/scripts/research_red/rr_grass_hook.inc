@ Research Red: Grass hook outside the house (PalletTown)
@ Moved out of the FireRed map files; the map keeps only its map_script hooks.

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
	@ The scene's own Ivy walks out of the door, so drop the follower
	destroyfollowernpc
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
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_Text_RR_Jenny_EightOutOfTen, MSGBOX_DEFAULT
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

@ ---------------------------------------------------------------
@ Research Red: grass hook
@ ---------------------------------------------------------------
PalletTown_Text_RR_Ivy_StopByDaisys::
    .string "Ivy: Want to stop by Daisy's?\n"
    .string "Say hi to Elm and Jenny?$"

PalletTown_Text_RR_Elm_RouteOne::
    .string "Elm: Route One!\n"
    .string "Today, you are mine!$"

PalletTown_Text_RR_P_FollowTheYelling::
    .string "{PLAYER}: …Or we follow the\n"
    .string "yelling.$"

PalletTown_Text_RR_Elm_JustInTime::
    .string "Elm: {PLAYER}! Ivy!\n"
    .string "You're just in time for history!$"

PalletTown_Text_RR_Jenny_Stretching::
    .string "Jenny: Morning. He's been\n"
    .string "stretching for twenty minutes.$"

PalletTown_Text_RR_Elm_ReturnFavor::
    .string "Elm: Last night, Route 1 beat me.\n"
    .string "Today, I return the favor.$"

PalletTown_Text_RR_Ivy_StraightRoad::
    .string "Ivy: Elm, you got lost on a\n"
    .string "straight road.$"

PalletTown_Text_RR_Elm_Curves::
    .string "Elm: It curves! Emotionally!$"

PalletTown_Text_RR_Jenny_Support::
    .string "Jenny: I support you, dear.\n"
    .string "Completely.\p"
    .string "I also have a strong professional\n"
    .string "feeling about this.$"

PalletTown_Text_RR_P_WhichPokemon::
    .string "{PLAYER}: Quick question.\n"
    .string "Which Pokémon are you bringing?$"

PalletTown_Text_RR_Elm_Transferred::
    .string "Elm: …I transferred the Johto\n"
    .string "starters to Oak before we left.\p"
    .string "They're for the research trip.\n"
    .string "I have… nothing.$"

PalletTown_Text_RR_P_FightTheGrass::
    .string "{PLAYER}: So the plan is to fight\n"
    .string "the grass. Personally.$"

PalletTown_Text_RR_Jenny_EightOutOfTen::
    .string "Jenny: Eight out of ten.$"

PalletTown_Text_RR_Oak_WildMons::
    .string "Oak: Wild Pokémon live in tall\n"
    .string "grass!\p"
    .string "I've said that for thirty years.\n"
    .string "Nobody listens.$"

PalletTown_Text_RR_Elm_Observing::
    .string "Elm: P-Professor! I was just\n"
    .string "observing it! From here!$"

PalletTown_Text_RR_Oak_OfficerJenny::
    .string "Oak: And Jenny! At last, in\n"
    .string "person! Elm talks about you\l"
    .string "endlessly.$"

PalletTown_Text_RR_Jenny_WeAllGetThat::
    .string "Jenny: He talks about you too,\n"
    .string "sir. With footnotes.$"

PalletTown_Text_RR_Oak_Haiku::
    .string "Oak: Ahem.\p"
    .string "The grass sways in wind…\p"
    .string "A grown man walks in, unarmed…\n"
    .string "Foolish, but so brave.$"

PalletTown_Text_RR_Ivy_Poems::
    .string "Ivy: I love that he still does\n"
    .string "the poems.$"

PalletTown_Text_RR_Oak_ComeToLab::
    .string "Oak: Come! This research trip\n"
    .string "starts at my lab, not in the\l"
    .string "grass.$"


@ ===================================================================
@ Research Red: Oak's lab scene text (used by RR_Lab_EventScript_Scene)
@ ===================================================================
