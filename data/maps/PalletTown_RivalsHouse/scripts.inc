.equ RECEIVED_TOWN_MAP, VAR_TEMP_1

PalletTown_RivalsHouse_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PalletTown_RivalsHouse_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, PalletTown_RivalsHouse_OnFrame
	.byte 0

PalletTown_RivalsHouse_OnTransition::
	call_if_lt VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 2, PalletTown_RivalsHouse_EventScript_MoveDaisyToTable
	call_if_ge VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 2, PalletTown_RivalsHouse_EventScript_AlreadyReceivedTownMap
	end

@ Research Red: the Daisy scene plays once, right after the lab (VAR_0x40FE == 3)
PalletTown_RivalsHouse_OnFrame::
	map_script_2 VAR_0x40FE, 3, RR_Daisy_EventScript_Scene
	.2byte 0

PalletTown_RivalsHouse_EventScript_MoveDaisyToTable::
	setobjectmovementtype LOCALID_DAISY, MOVEMENT_TYPE_FACE_RIGHT
	setobjectxyperm LOCALID_DAISY, 5, 4
	return

PalletTown_RivalsHouse_EventScript_AlreadyReceivedTownMap::
	setvar RECEIVED_TOWN_MAP, TRUE
	return

PalletTown_RivalsHouse_EventScript_Daisy::
	lock
	faceplayer
	famechecker FAMECHECKER_DAISY, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	goto_if_set FLAG_SYS_GAME_CLEAR, PalletTown_RivalsHouse_EventScript_GroomMon
	goto_if_eq RECEIVED_TOWN_MAP, TRUE, PalletTown_RivalsHouse_EventScript_PleaseGiveMonsRest
	goto_if_eq VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 2, PalletTown_RivalsHouse_EventScript_ExplainTownMap
	goto_if_eq VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 1, PalletTown_RivalsHouse_EventScript_GiveTownMap
	goto_if_ge VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB, 1, PalletTown_RivalsHouse_EventScript_HeardBattledRival
	msgbox PalletTown_RivalsHouse_Text_HiBrothersAtLab
	closemessage
	applymovement LOCALID_DAISY, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

PalletTown_RivalsHouse_EventScript_HeardBattledRival::
	msgbox PalletTown_RivalsHouse_Text_HeardYouBattledRival
	release
	end

PalletTown_RivalsHouse_EventScript_GroomMon::
	goto_if_questlog EventScript_ReleaseEnd
	special QuestLog_CutRecording
	goto_if_lt VAR_MASSAGE_COOLDOWN_STEP_COUNTER, 500, PalletTown_RivalsHouse_EventScript_RateMonFriendship
	msgbox PalletTown_RivalsHouse_Text_LikeMeToGroomMon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, PalletTown_RivalsHouse_EventScript_DeclineGrooming
	msgbox PalletTown_RivalsHouse_Text_GroomWhichOne
	special ChoosePartyMon
	waitstate
	lock
	faceplayer
	goto_if_ge VAR_0x8004, PARTY_SIZE, PalletTown_RivalsHouse_EventScript_DeclineGrooming
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_EGG, PalletTown_RivalsHouse_EventScript_CantGroomEgg
	msgbox PalletTown_RivalsHouse_Text_LookingNiceInNoTime
	closemessage
	fadescreen FADE_TO_BLACK
	playfanfare MUS_HEAL
	waitfanfare
	special DaisyMassageServices
	fadescreen FADE_FROM_BLACK
	special BufferMonNickname
	msgbox PalletTown_RivalsHouse_Text_ThereYouGoAllDone
	release
	end

PalletTown_RivalsHouse_EventScript_CantGroomEgg::
	msgbox PalletTown_RivalsHouse_Text_CantGroomAnEgg
	release
	end

PalletTown_RivalsHouse_EventScript_DeclineGrooming::
	msgbox PalletTown_RivalsHouse_Text_DontNeedAnyGrooming
	release
	end

PalletTown_RivalsHouse_EventScript_RateMonFriendship::
	msgbox PalletTown_RivalsHouse_Text_MayISeeFirstMon
	specialvar VAR_RESULT, GetLeadMonFriendship
	switch VAR_RESULT
	case 0, PalletTown_RivalsHouse_EventScript_MonFriendshipLowest
	case 1, PalletTown_RivalsHouse_EventScript_MonFriendshipLower
	case 2, PalletTown_RivalsHouse_EventScript_MonFriendshipLow
	case 3, PalletTown_RivalsHouse_EventScript_MonFriendshipMid
	case 4, PalletTown_RivalsHouse_EventScript_MonFriendshipHigh
	case 5, PalletTown_RivalsHouse_EventScript_MonFriendshipHigher
	case 6, PalletTown_RivalsHouse_EventScript_MonFriendshipHighest
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipLowest::
	msgbox PalletTown_RivalsHouse_Text_WhyWouldMonHateYouSoMuch
	release
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipLower::
	msgbox PalletTown_RivalsHouse_Text_DontLikeWayItGlaresAtYou
	release
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipLow::
	msgbox PalletTown_RivalsHouse_Text_NotFamiliarWithYouYet
	release
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipMid::
	msgbox PalletTown_RivalsHouse_Text_ItsWarmingUpToYou
	release
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipHigh::
	msgbox PalletTown_RivalsHouse_Text_ItsQuiteFriendly
	release
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipHigher::
	msgbox PalletTown_RivalsHouse_Text_ItLooksVeryHappy
	release
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipHighest::
	msgbox PalletTown_RivalsHouse_Text_CouldntLoveYouMore
	release
	end

PalletTown_RivalsHouse_EventScript_PleaseGiveMonsRest::
	msgbox PalletTown_RivalsHouse_Text_PleaseGiveMonsRest
	release
	end

PalletTown_RivalsHouse_EventScript_GiveTownMap::
	msgbox PalletTown_RivalsHouse_Text_ErrandForGrandpaThisWillHelp
	closemessage
	checkitemspace ITEM_TOWN_MAP
	goto_if_eq VAR_RESULT, FALSE, PalletTown_RivalsHouse_EventScript_NoRoomForTownMap
	applymovement LOCALID_DAISY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	removeobject LOCALID_TOWN_MAP
	setvar VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 2
	delay 15
	applymovement LOCALID_DAISY, Common_Movement_FacePlayer
	waitmovement 0
	delay 12
	giveitem_msg PalletTown_RivalsHouse_Text_ReceivedTownMapFromDaisy, ITEM_TOWN_MAP, 1, MUS_OBTAIN_KEY_ITEM
	release
	end

PalletTown_RivalsHouse_EventScript_NoRoomForTownMap::
	msgbox PalletTown_RivalsHouse_Text_DontHaveSpaceForThis
	release
	end

PalletTown_RivalsHouse_EventScript_ExplainTownMap::
	msgbox PalletTown_RivalsHouse_Text_ExplainTownMap
	release
	end

PalletTown_RivalsHouse_EventScript_TownMap::
	msgbox PalletTown_RivalsHouse_Text_ItsBigMapOfKanto, MSGBOX_NPC
	end

PalletTown_RivalsHouse_EventScript_Bookshelf::
	msgbox PalletTown_RivalsHouse_Text_ShelvesCrammedFullOfBooks, MSGBOX_SIGN
	end

PalletTown_RivalsHouse_EventScript_Picture::
	msgbox PalletTown_RivalsHouse_Text_LovelyAndSweetClefairy, MSGBOX_SIGN
	end

@ ===================================================================
@ Research Red: Daisy's house scene (maps, Muffin, Kukui and Burnett, the bet)
@ Elm and Jenny share FLAG_HIDE_TOWN_MAP, so they disappear for good once the
@ Town Map is handed over.
@ ===================================================================
RR_Daisy_EventScript_Scene::
	lockall
	applymovement LOCALID_PLAYER, RR_Daisy_Movement_Up2
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_ElmOnChair, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_MALE
	msgbox RR_Daisy_Text_Elm_Help, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_MuffinSits, MSGBOX_DEFAULT
	applymovement LOCALID_DAISY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox RR_Daisy_Text_Daisy_Muffin, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_MALE
	msgbox RR_Daisy_Text_Elm_PersianEyes, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox RR_Daisy_Text_Jenny_ComeDown, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_DAISY, Common_Movement_FacePlayer
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_P_Maps, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox RR_Daisy_Text_Daisy_TwoMaps, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_MALE
	msgbox RR_Daisy_Text_Elm_Strategy, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox RR_Daisy_Text_Jenny_Toothbrush, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_DAISY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	removeobject LOCALID_TOWN_MAP
	delay 15
	applymovement LOCALID_DAISY, Common_Movement_FacePlayer
	waitmovement 0
	additem ITEM_TOWN_MAP, 1
	setvar VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 2
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_OBTAIN_KEY_ITEM
	message RR_Daisy_Text_ReceivedMap
	waitmessage
	waitfanfare
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox RR_Daisy_Text_Daisy_TwoMoreProfessors, MSGBOX_DEFAULT
	msgbox RR_Daisy_Text_Ivy_KukuiBurnett, MSGBOX_DEFAULT
	msgbox RR_Daisy_Text_Ivy_Vulpix, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_P_MidVulpix, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox RR_Daisy_Text_Ivy_Professional, MSGBOX_DEFAULT
	closemessage
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_MuffinHops, MSGBOX_DEFAULT
	closemessage
	playse SE_PIN
	applymovement LOCALID_RIVALS_HOUSE_ELM, Common_Movement_ExclamationMark
	waitmovement 0
	textcolor NPC_TEXT_COLOR_MALE
	msgbox RR_Daisy_Text_Elm_Choo, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox RR_Daisy_Text_Daisy_Rattata, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_MALE
	msgbox RR_Daisy_Text_Elm_Fluffy, MSGBOX_DEFAULT
	msgbox RR_Daisy_Text_Elm_Terms, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox RR_Daisy_Text_Jenny_SmallStakes, MSGBOX_DEFAULT
	msgbox RR_Daisy_Text_Ivy_Dishes, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_P_Mimey, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox RR_Daisy_Text_Ivy_MimeysDishes, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_MALE
	msgbox RR_Daisy_Text_Elm_Deal, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_LEVEL_UP
	message RR_Daisy_Text_NumbersRegistered
	waitmessage
	waitfanfare
	msgbox RR_Daisy_Text_BetIsOn, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox RR_Daisy_Text_Daisy_Excited, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_MALE
	msgbox RR_Daisy_Text_Elm_SoExcited, MSGBOX_DEFAULT
	msgbox RR_Daisy_Text_Elm_SeeYou, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_RIVALS_HOUSE_ELM, RR_Daisy_Movement_ElmExit
	waitmovement 0
	removeobject LOCALID_RIVALS_HOUSE_ELM
	playse SE_EXIT
	applymovement LOCALID_RIVALS_HOUSE_JENNY, RR_Daisy_Movement_JennyExit
	waitmovement 0
	removeobject LOCALID_RIVALS_HOUSE_JENNY
	playse SE_EXIT
	setvar VAR_0x40FE, 4
	releaseall
	end

RR_Daisy_Movement_Up2:
	walk_up
	walk_up
	step_end

RR_Daisy_Movement_ElmExit:
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

RR_Daisy_Movement_JennyExit:
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

RR_Daisy_Text_ElmOnChair::
    .string "Elm is standing on a chair.\n"
    .string "Jenny is holding it steady.$"

RR_Daisy_Text_Elm_Help::
    .string "Elm: {PLAYER}! Help!\n"
    .string "It's looking at me!$"

RR_Daisy_Text_MuffinSits::
    .string "A small purple Pokémon sits\n"
    .string "calmly on the rug.$"

RR_Daisy_Text_Daisy_Muffin::
    .string "Daisy: It's a Rattata, Elm.\n"
    .string "Her name is Muffin.$"

RR_Daisy_Text_Elm_PersianEyes::
    .string "Elm: Muffin has the eyes\n"
    .string "of a Persian.$"

RR_Daisy_Text_Jenny_ComeDown::
    .string "Jenny: Muffin has the eyes of a\n"
    .string "Rattata, dear. Come down.$"

RR_Daisy_Text_P_Maps::
    .string "{PLAYER}: Daisy? Your grandpa says\n"
    .string "you have Town Maps for us.$"

RR_Daisy_Text_Daisy_TwoMaps::
    .string "Daisy: I have two. Gary took one.\n"
    .string "Ash borrowed the other one.\p"
    .string "I'm still waiting.\n"
    .string "So: one per couple.$"

RR_Daisy_Text_Elm_Strategy::
    .string "Elm: One per couple?!\n"
    .string "But strategy! Divide and conquer!$"

RR_Daisy_Text_Jenny_Toothbrush::
    .string "Jenny: We share a toothbrush cup,\n"
    .string "dear. We can share a map.$"

RR_Daisy_Text_ReceivedMap::
    .string "{PLAYER} received the Town Map!$"

RR_Daisy_Text_Daisy_TwoMoreProfessors::
    .string "Daisy: Grandpa said two more\n"
    .string "professors were supposed to come.\p"
    .string "A couple from Alola?$"

RR_Daisy_Text_Ivy_KukuiBurnett::
    .string "Ivy: Kukui and Burnett.\n"
    .string "They couldn't get away.\p"
    .string "Something about starting\n"
    .string "a league.$"

RR_Daisy_Text_Ivy_Vulpix::
    .string "Ivy: Burnett once showed me a\n"
    .string "Vulpix from Alola's mountains.\p"
    .string "White. Cold to the touch.\n"
    .string "Same species, different home.$"

RR_Daisy_Text_P_MidVulpix::
    .string "{PLAYER}: That's when she married\n"
    .string "me, by the way. Mid-Vulpix story.$"

RR_Daisy_Text_Ivy_Professional::
    .string "Ivy: I married you after the\n"
    .string "story, sweetie.\p"
    .string "I'm a professional.$"

RR_Daisy_Text_MuffinHops::
    .string "Muffin hops onto the chair.\n"
    .string "Elm hops onto the table.$"

RR_Daisy_Text_Elm_Choo::
    .string "Elm: Ah… ah… choo!\n"
    .string "Allergies. Daisy's Persian.$"

RR_Daisy_Text_Daisy_Rattata::
    .string "Daisy: Muffin. Rattata.$"

RR_Daisy_Text_Elm_Fluffy::
    .string "Elm: …A very fluffy Rattata.$"

RR_Daisy_Text_Elm_Terms::
    .string "Elm: Official terms! First team\n"
    .string "to win all eight Gym badges\l"
    .string "takes the bet!$"

RR_Daisy_Text_Jenny_SmallStakes::
    .string "Jenny: Small stakes.\n"
    .string "In my professional opinion.$"

RR_Daisy_Text_Ivy_Dishes::
    .string "Ivy: Losers do the dishes\n"
    .string "for a month.$"

RR_Daisy_Text_P_Mimey::
    .string "{PLAYER}: Mimey already does\n"
    .string "the dishes.$"

RR_Daisy_Text_Ivy_MimeysDishes::
    .string "Ivy: Then the losers do\n"
    .string "Mimey's dishes.$"

RR_Daisy_Text_Elm_Deal::
    .string "Elm: Deal! We swap numbers,\n"
    .string "and it's official!$"

RR_Daisy_Text_NumbersRegistered::
    .string "Elm's and Jenny's numbers were\n"
    .string "saved!$"

RR_Daisy_Text_BetIsOn::
    .string "The bet is on: first team to\n"
    .string "eight Gym badges.$"

RR_Daisy_Text_Daisy_Excited::
    .string "Daisy: Grandpa is going to pretend\n"
    .string "he's not excited about this.$"

RR_Daisy_Text_Elm_SoExcited::
    .string "Elm: He is so excited about this.$"

RR_Daisy_Text_Elm_SeeYou::
    .string "Elm: See you on Route 1!\n"
    .string "Last one to Viridian buys lunch!$"
