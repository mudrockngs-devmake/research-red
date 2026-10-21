@ Research Red: Oak's lab: starter picker, chain, battles, grant (PalletTown_ProfessorOaksLab, triggered from map)
@ Moved out of the FireRed map files; the map keeps only its map_script hooks.

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
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_P_TripSecurity, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Oak_WhyHere, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Elm_Mailed, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Jenny_Proofread, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_Fridge, MSGBOX_DEFAULT
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
	msgbox RR_Lab_Text_Elm_GoodBurner, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Ivy_EastWing, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Elm_Bunsen, MSGBOX_DEFAULT
	closemessage
	@ --- choose a starter (works for all three) ---
RR_Lab_EventScript_ChooseLoop::
	@ Six starters: Kanto table (top row) and Johto table (bottom row).
	@ The list shows each Pokemon's picture as you scroll.
	textcolor NPC_TEXT_COLOR_NEUTRAL
	message RR_Lab_Text_ChooseStarter
	waitmessage
	@ The portrait steps aside while you choose, so the list sits on the left
	@ again with its picture box after it (#76; it sat right of the portrait, #21)
	callnative RR_ScriptPortraitHide
	dynmultichoice 1, 0, TRUE, 6, 0, DYN_MULTICHOICE_CB_RR_STARTER, RR_Lab_Text_Name_Bulbasaur, RR_Lab_Text_Name_Charmander, RR_Lab_Text_Name_Squirtle, RR_Lab_Text_Name_Chikorita, RR_Lab_Text_Name_Cyndaquil, RR_Lab_Text_Name_Totodile
	closemessage
	switch VAR_RESULT
	case 0, RR_Lab_EventScript_PickBulbasaur
	case 1, RR_Lab_EventScript_PickCharmander
	case 2, RR_Lab_EventScript_PickSquirtle
	case 3, RR_Lab_EventScript_PickChikorita
	case 4, RR_Lab_EventScript_PickCyndaquil
	case 5, RR_Lab_EventScript_PickTotodile
	goto RR_Lab_EventScript_ChooseLoop

RR_Lab_EventScript_PickBulbasaur::
	bufferspeciesname STR_VAR_1, SPECIES_BULBASAUR
	msgbox RR_Lab_Text_StudyConfirm, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RR_Lab_EventScript_ChooseLoop
	setvar VAR_RR_STARTER, 0
	setvar VAR_STARTER_MON, 0
	removeobject LOCALID_BULBASAUR_BALL
	givemon SPECIES_BULBASAUR, 5
	goto RR_Lab_EventScript_Received

RR_Lab_EventScript_PickCharmander::
	bufferspeciesname STR_VAR_1, SPECIES_CHARMANDER
	msgbox RR_Lab_Text_StudyConfirm, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RR_Lab_EventScript_ChooseLoop
	setvar VAR_RR_STARTER, 1
	setvar VAR_STARTER_MON, 2
	removeobject LOCALID_CHARMANDER_BALL
	givemon SPECIES_CHARMANDER, 5
	goto RR_Lab_EventScript_Received

RR_Lab_EventScript_PickSquirtle::
	bufferspeciesname STR_VAR_1, SPECIES_SQUIRTLE
	msgbox RR_Lab_Text_StudyConfirm, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RR_Lab_EventScript_ChooseLoop
	setvar VAR_RR_STARTER, 2
	setvar VAR_STARTER_MON, 1
	removeobject LOCALID_SQUIRTLE_BALL
	givemon SPECIES_SQUIRTLE, 5
	goto RR_Lab_EventScript_Received

RR_Lab_EventScript_PickChikorita::
	bufferspeciesname STR_VAR_1, SPECIES_CHIKORITA
	msgbox RR_Lab_Text_StudyConfirm, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RR_Lab_EventScript_ChooseLoop
	setvar VAR_RR_STARTER, 3
	setvar VAR_STARTER_MON, 0
	removeobject LOCALID_LAB_CHIKORITA_BALL
	givemon SPECIES_CHIKORITA, 5
	goto RR_Lab_EventScript_Received

RR_Lab_EventScript_PickCyndaquil::
	bufferspeciesname STR_VAR_1, SPECIES_CYNDAQUIL
	msgbox RR_Lab_Text_StudyConfirm, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RR_Lab_EventScript_ChooseLoop
	setvar VAR_RR_STARTER, 4
	setvar VAR_STARTER_MON, 2
	removeobject LOCALID_LAB_CYNDAQUIL_BALL
	givemon SPECIES_CYNDAQUIL, 5
	goto RR_Lab_EventScript_Received

RR_Lab_EventScript_PickTotodile::
	bufferspeciesname STR_VAR_1, SPECIES_TOTODILE
	msgbox RR_Lab_Text_StudyConfirm, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RR_Lab_EventScript_ChooseLoop
	setvar VAR_RR_STARTER, 5
	setvar VAR_STARTER_MON, 1
	removeobject LOCALID_LAB_TOTODILE_BALL
	givemon SPECIES_TOTODILE, 5
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
	@ Ivy: counter to yours from your region (joins your party).
	@ Elm: counter to yours from the other region.
	@ Jenny: counter to Elm's from his region. Oak keeps the last two.
	switch VAR_RR_STARTER
	case 0, RR_Lab_EventScript_ChainBulbasaur
	case 1, RR_Lab_EventScript_ChainCharmander
	case 2, RR_Lab_EventScript_ChainSquirtle
	case 3, RR_Lab_EventScript_ChainChikorita
	case 4, RR_Lab_EventScript_ChainCyndaquil
	case 5, RR_Lab_EventScript_ChainTotodile

@ Bulbasaur: Ivy Charmander, Elm Cyndaquil, Jenny Totodile (Oak: Squirtle, Chikorita)
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

@ Charmander: Ivy Squirtle, Elm Totodile, Jenny Chikorita (Oak: Bulbasaur, Cyndaquil)
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

@ Squirtle: Ivy Bulbasaur, Elm Chikorita, Jenny Cyndaquil (Oak: Charmander, Totodile)
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

@ Chikorita: Ivy Cyndaquil, Elm Charmander, Jenny Squirtle (Oak: Totodile, Bulbasaur)
RR_Lab_EventScript_ChainChikorita::
	bufferspeciesname STR_VAR_1, SPECIES_CYNDAQUIL
	call RR_Lab_EventScript_IvyLine
	removeobject LOCALID_LAB_CYNDAQUIL_BALL
	givemon SPECIES_CYNDAQUIL, 5
	call RR_Lab_EventScript_IvyMonJoins
	bufferspeciesname STR_VAR_1, SPECIES_CHARMANDER
	call RR_Lab_EventScript_ElmLine
	removeobject LOCALID_CHARMANDER_BALL
	bufferspeciesname STR_VAR_1, SPECIES_SQUIRTLE
	call RR_Lab_EventScript_JennyLine
	removeobject LOCALID_SQUIRTLE_BALL
	goto RR_Lab_EventScript_AfterPicks

@ Cyndaquil: Ivy Totodile, Elm Squirtle, Jenny Bulbasaur (Oak: Chikorita, Charmander)
RR_Lab_EventScript_ChainCyndaquil::
	bufferspeciesname STR_VAR_1, SPECIES_TOTODILE
	call RR_Lab_EventScript_IvyLine
	removeobject LOCALID_LAB_TOTODILE_BALL
	givemon SPECIES_TOTODILE, 5
	call RR_Lab_EventScript_IvyMonJoins
	bufferspeciesname STR_VAR_1, SPECIES_SQUIRTLE
	call RR_Lab_EventScript_ElmLine
	removeobject LOCALID_SQUIRTLE_BALL
	bufferspeciesname STR_VAR_1, SPECIES_BULBASAUR
	call RR_Lab_EventScript_JennyLine
	removeobject LOCALID_BULBASAUR_BALL
	goto RR_Lab_EventScript_AfterPicks

@ Totodile: Ivy Chikorita, Elm Bulbasaur, Jenny Charmander (Oak: Cyndaquil, Squirtle)
RR_Lab_EventScript_ChainTotodile::
	bufferspeciesname STR_VAR_1, SPECIES_CHIKORITA
	call RR_Lab_EventScript_IvyLine
	removeobject LOCALID_LAB_CHIKORITA_BALL
	givemon SPECIES_CHIKORITA, 5
	call RR_Lab_EventScript_IvyMonJoins
	bufferspeciesname STR_VAR_1, SPECIES_BULBASAUR
	call RR_Lab_EventScript_ElmLine
	removeobject LOCALID_BULBASAUR_BALL
	bufferspeciesname STR_VAR_1, SPECIES_CHARMANDER
	call RR_Lab_EventScript_JennyLine
	removeobject LOCALID_CHARMANDER_BALL
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
	@ --- first battle: you + Ivy (both in your party) vs Elm + Jenny, a double ---
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playbgm MUS_ENCOUNTER_RIVAL, 0
	msgbox RR_Lab_Text_Elm_TestNow, MSGBOX_DEFAULT
	closemessage
	@ A loss continues the scene (Elm gloats) instead of whiting out
	setflag FLAG_RR_NO_WHITEOUT
	@ Elm and Jenny use their picks from the starter chain
	switch VAR_RR_STARTER
	case 0, RR_Lab_EventScript_BattleAfterBulbasaur
	case 1, RR_Lab_EventScript_BattleAfterCharmander
	case 2, RR_Lab_EventScript_BattleAfterSquirtle
	case 3, RR_Lab_EventScript_BattleAfterChikorita
	case 4, RR_Lab_EventScript_BattleAfterCyndaquil
	case 5, RR_Lab_EventScript_BattleAfterTotodile

@ Player picked Bulbasaur: Elm Cyndaquil, Jenny Totodile
RR_Lab_EventScript_BattleAfterBulbasaur::
	trainerbattle_two_trainers TRAINER_RR_ELM_LAB_CYNDAQUIL, RR_Lab_Text_Elm_Defeat, TRAINER_RR_JENNY_LAB_TOTODILE, RR_Lab_Text_Jenny_Defeat
	goto RR_Lab_EventScript_BattleResult

@ Player picked Charmander: Elm Totodile, Jenny Chikorita
RR_Lab_EventScript_BattleAfterCharmander::
	trainerbattle_two_trainers TRAINER_RR_ELM_LAB_TOTODILE, RR_Lab_Text_Elm_Defeat, TRAINER_RR_JENNY_LAB_CHIKORITA, RR_Lab_Text_Jenny_Defeat
	goto RR_Lab_EventScript_BattleResult

@ Player picked Squirtle: Elm Chikorita, Jenny Cyndaquil
RR_Lab_EventScript_BattleAfterSquirtle::
	trainerbattle_two_trainers TRAINER_RR_ELM_LAB_CHIKORITA, RR_Lab_Text_Elm_Defeat, TRAINER_RR_JENNY_LAB_CYNDAQUIL, RR_Lab_Text_Jenny_Defeat
	goto RR_Lab_EventScript_BattleResult

@ Player picked Chikorita: Elm Charmander, Jenny Squirtle
RR_Lab_EventScript_BattleAfterChikorita::
	trainerbattle_two_trainers TRAINER_RR_ELM_LAB_CHARMANDER, RR_Lab_Text_Elm_Defeat, TRAINER_RR_JENNY_LAB_SQUIRTLE, RR_Lab_Text_Jenny_Defeat
	goto RR_Lab_EventScript_BattleResult

@ Player picked Cyndaquil: Elm Squirtle, Jenny Bulbasaur
RR_Lab_EventScript_BattleAfterCyndaquil::
	trainerbattle_two_trainers TRAINER_RR_ELM_LAB_SQUIRTLE, RR_Lab_Text_Elm_Defeat, TRAINER_RR_JENNY_LAB_BULBASAUR, RR_Lab_Text_Jenny_Defeat
	goto RR_Lab_EventScript_BattleResult

@ Player picked Totodile: Elm Bulbasaur, Jenny Charmander
RR_Lab_EventScript_BattleAfterTotodile::
	trainerbattle_two_trainers TRAINER_RR_ELM_LAB_BULBASAUR, RR_Lab_Text_Elm_Defeat, TRAINER_RR_JENNY_LAB_CHARMANDER, RR_Lab_Text_Jenny_Defeat
	goto RR_Lab_EventScript_BattleResult

RR_Lab_EventScript_BattleResult::
	clearflag FLAG_RR_NO_WHITEOUT
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, RR_Lab_EventScript_AfterBattle
	@ Lost: Elm and Jenny earned the right to face Oak (VAR_TEMP_5 = 1).
	@ Their own lines: "best of five" only makes sense when Elm lost (#47)
	setvar VAR_TEMP_5, 1
	special HealPlayerParty
	fadedefaultbgm
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Elm_Victory, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Jenny_Diagram, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Ivy_Morale, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Elm_PureScience, MSGBOX_DEFAULT
	closemessage
	goto RR_Lab_EventScript_OakSteps

RR_Lab_EventScript_AfterBattle::
	special HealPlayerParty
	fadedefaultbgm
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Elm_BestOfFive, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Jenny_BestOfZero, MSGBOX_DEFAULT
	closemessage
RR_Lab_EventScript_OakSteps::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_Properly, MSGBOX_DEFAULT
	goto_if_eq VAR_TEMP_5, 1, RR_Lab_EventScript_OakVsElm
	@ --- optional double battle vs Oak (his two leftover starters) ---
	switch VAR_RR_STARTER
	case 0, RR_Lab_EventScript_OakPair0
	case 1, RR_Lab_EventScript_OakPair1
	case 2, RR_Lab_EventScript_OakPair2
	case 3, RR_Lab_EventScript_OakPair3
	case 4, RR_Lab_EventScript_OakPair4
	case 5, RR_Lab_EventScript_OakPair5

RR_Lab_EventScript_OakAsk::
	msgbox RR_Lab_Text_Oak_Challenge, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_OakBattleAsk, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RR_Lab_EventScript_OakDeclined
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Ivy_LikeThis, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_RR_NO_WHITEOUT
	switch VAR_RR_STARTER
	case 0, RR_Lab_EventScript_OakBattle0
	case 1, RR_Lab_EventScript_OakBattle1
	case 2, RR_Lab_EventScript_OakBattle2
	case 3, RR_Lab_EventScript_OakBattle3
	case 4, RR_Lab_EventScript_OakBattle4
	case 5, RR_Lab_EventScript_OakBattle5

@ Leftovers when the player picked Bulbasaur
RR_Lab_EventScript_OakPair0::
	bufferspeciesname STR_VAR_1, SPECIES_SQUIRTLE
	bufferspeciesname STR_VAR_2, SPECIES_CHIKORITA
	goto RR_Lab_EventScript_OakAsk

RR_Lab_EventScript_OakBattle0::
	trainerbattle_no_intro TRAINER_RR_OAK_LAB_SQUIRTLE_CHIKORITA, RR_Lab_Text_Oak_Defeat
	goto RR_Lab_EventScript_OakResult

@ Leftovers when the player picked Charmander
RR_Lab_EventScript_OakPair1::
	bufferspeciesname STR_VAR_1, SPECIES_BULBASAUR
	bufferspeciesname STR_VAR_2, SPECIES_CYNDAQUIL
	goto RR_Lab_EventScript_OakAsk

RR_Lab_EventScript_OakBattle1::
	trainerbattle_no_intro TRAINER_RR_OAK_LAB_BULBASAUR_CYNDAQUIL, RR_Lab_Text_Oak_Defeat
	goto RR_Lab_EventScript_OakResult

@ Leftovers when the player picked Squirtle
RR_Lab_EventScript_OakPair2::
	bufferspeciesname STR_VAR_1, SPECIES_CHARMANDER
	bufferspeciesname STR_VAR_2, SPECIES_TOTODILE
	goto RR_Lab_EventScript_OakAsk

RR_Lab_EventScript_OakBattle2::
	trainerbattle_no_intro TRAINER_RR_OAK_LAB_CHARMANDER_TOTODILE, RR_Lab_Text_Oak_Defeat
	goto RR_Lab_EventScript_OakResult

@ Leftovers when the player picked Chikorita
RR_Lab_EventScript_OakPair3::
	bufferspeciesname STR_VAR_1, SPECIES_TOTODILE
	bufferspeciesname STR_VAR_2, SPECIES_BULBASAUR
	goto RR_Lab_EventScript_OakAsk

RR_Lab_EventScript_OakBattle3::
	trainerbattle_no_intro TRAINER_RR_OAK_LAB_TOTODILE_BULBASAUR, RR_Lab_Text_Oak_Defeat
	goto RR_Lab_EventScript_OakResult

@ Leftovers when the player picked Cyndaquil
RR_Lab_EventScript_OakPair4::
	bufferspeciesname STR_VAR_1, SPECIES_CHIKORITA
	bufferspeciesname STR_VAR_2, SPECIES_CHARMANDER
	goto RR_Lab_EventScript_OakAsk

RR_Lab_EventScript_OakBattle4::
	trainerbattle_no_intro TRAINER_RR_OAK_LAB_CHIKORITA_CHARMANDER, RR_Lab_Text_Oak_Defeat
	goto RR_Lab_EventScript_OakResult

@ Leftovers when the player picked Totodile
RR_Lab_EventScript_OakPair5::
	bufferspeciesname STR_VAR_1, SPECIES_CYNDAQUIL
	bufferspeciesname STR_VAR_2, SPECIES_SQUIRTLE
	goto RR_Lab_EventScript_OakAsk

RR_Lab_EventScript_OakBattle5::
	trainerbattle_no_intro TRAINER_RR_OAK_LAB_CYNDAQUIL_SQUIRTLE, RR_Lab_Text_Oak_Defeat
	goto RR_Lab_EventScript_OakResult

RR_Lab_EventScript_OakResult::
	clearflag FLAG_RR_NO_WHITEOUT
	special HealPlayerParty
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, RR_Lab_EventScript_OakBeaten
	@ Loss: Oak's haiku, then Elm offers his own funding
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_CantFund, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Elm_Funding, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_SweetThought, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Oak_Haiku, MSGBOX_DEFAULT
	goto RR_Lab_EventScript_Grant

RR_Lab_EventScript_OakBeaten::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_MoreInYou, MSGBOX_DEFAULT
	goto RR_Lab_EventScript_Grant

RR_Lab_EventScript_OakDeclined::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_P_AfterPaperwork, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_MissedHaiku, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Elm_StillFunding, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_HaveItCovered, MSGBOX_DEFAULT
RR_Lab_EventScript_Grant::
	textcolor NPC_TEXT_COLOR_NEUTRAL
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
	msgbox RR_Lab_Text_Ivy_BuyOutMart, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_CarriedAway, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Jenny_Trust, MSGBOX_DEFAULT
	@ Research Red (#29): the Ranch Pager and the PC (tutorial lines, owner to rework)
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_RanchPager, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Ivy_OnCall, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_Exercise, MSGBOX_DEFAULT
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
	rr_ivy_follow LOCALID_LAB_IVY
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
	additem ITEM_MASTER_BALL, 100
	additem ITEM_FAST_BALL, 100
	additem ITEM_LEVEL_BALL, 100
	additem ITEM_LURE_BALL, 100
	additem ITEM_HEAVY_BALL, 100
	additem ITEM_LOVE_BALL, 100
	additem ITEM_FRIEND_BALL, 100
	additem ITEM_MOON_BALL, 100
	additem ITEM_SPORT_BALL, 100
	additem ITEM_DREAM_BALL, 100
	additem ITEM_BEAST_BALL, 100
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
	@ Research Red: medicine and battle items
	additem ITEM_MAX_POTION, 10
	additem ITEM_ELIXIR, 10
	additem ITEM_MAX_ELIXIR, 5
	additem ITEM_PP_UP, 10
	additem ITEM_X_ATTACK, 10
	additem ITEM_X_DEFENSE, 10
	additem ITEM_X_SP_ATK, 10
	additem ITEM_X_SP_DEF, 10
	additem ITEM_X_SPEED, 10
	additem ITEM_X_ACCURACY, 10
	additem ITEM_DIRE_HIT, 10
	additem ITEM_GUARD_SPEC, 10
	@ held items (one each; Items pocket is 60 slots)
	additem ITEM_LEFTOVERS, 1
	additem ITEM_LIFE_ORB, 1
	additem ITEM_CHOICE_BAND, 1
	additem ITEM_CHOICE_SPECS, 1
	additem ITEM_CHOICE_SCARF, 1
	additem ITEM_FOCUS_SASH, 1
	additem ITEM_ASSAULT_VEST, 1
	additem ITEM_ROCKY_HELMET, 1
	additem ITEM_EVIOLITE, 1
	additem ITEM_EXPERT_BELT, 1
	additem ITEM_LIGHT_CLAY, 1
	additem ITEM_WEAKNESS_POLICY, 1
	additem ITEM_AIR_BALLOON, 1
	additem ITEM_HEAVY_DUTY_BOOTS, 1
	additem ITEM_AMULET_COIN, 1
	additem ITEM_LUCKY_EGG, 1
	additem ITEM_SOOTHE_BELL, 1
	additem ITEM_QUICK_CLAW, 1
	additem ITEM_SCOPE_LENS, 1
	additem ITEM_KINGS_ROCK, 1
	additem ITEM_BLACK_SLUDGE, 1
	additem ITEM_FLAME_ORB, 1
	additem ITEM_TOXIC_ORB, 1
	additem ITEM_SHELL_BELL, 1
	additem ITEM_WIDE_LENS, 1
	additem ITEM_MUSCLE_BAND, 1
	additem ITEM_WISE_GLASSES, 1
	additem ITEM_FOCUS_BAND, 1
	additem ITEM_RED_CARD, 1
	additem ITEM_EJECT_BUTTON, 1
	@ key items (story items like the Town Map stay in their scenes)
	additem ITEM_EXP_SHARE, 1
	additem ITEM_BICYCLE, 1
	additem ITEM_SUPER_ROD, 1 @ the one Fishing Rod (renamed Super Rod)
	additem ITEM_ITEMFINDER, 1
	additem ITEM_VS_SEEKER, 1
	additem ITEM_BERRY_POUCH, 1
	additem ITEM_RANCH_PAGER, 1 @ Research Red (#29)
	setflag FLAG_RR_EXP_SHARE
	@ berries (Berry Pouch above)
	additem ITEM_ORAN_BERRY, 20
	additem ITEM_SITRUS_BERRY, 20
	additem ITEM_LUM_BERRY, 10
	additem ITEM_CHESTO_BERRY, 10
	@ HMs (reusable; field moves need no badge)
	additem ITEM_HM01, 1
	additem ITEM_HM02, 1
	additem ITEM_HM03, 1
	additem ITEM_HM04, 1
	additem ITEM_HM05, 1
	additem ITEM_HM06, 1
	additem ITEM_HM07, 1
	additem ITEM_HM08, 1
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
	@ Research Red (#57): TM51-65 FireRed's move tutors, TM66-100 modern moves
	call RR_EventScript_GiveTM51To100
	return

@ Also used to top up older saves that got the grant before TM51-100 existed
RR_EventScript_GiveTM51To100::
	additem ITEM_TM51, 1
	additem ITEM_TM52, 1
	additem ITEM_TM53, 1
	additem ITEM_TM54, 1
	additem ITEM_TM55, 1
	additem ITEM_TM56, 1
	additem ITEM_TM57, 1
	additem ITEM_TM58, 1
	additem ITEM_TM59, 1
	additem ITEM_TM60, 1
	additem ITEM_TM61, 1
	additem ITEM_TM62, 1
	additem ITEM_TM63, 1
	additem ITEM_TM64, 1
	additem ITEM_TM65, 1
	additem ITEM_TM66, 1
	additem ITEM_TM67, 1
	additem ITEM_TM68, 1
	additem ITEM_TM69, 1
	additem ITEM_TM70, 1
	additem ITEM_TM71, 1
	additem ITEM_TM72, 1
	additem ITEM_TM73, 1
	additem ITEM_TM74, 1
	additem ITEM_TM75, 1
	additem ITEM_TM76, 1
	additem ITEM_TM77, 1
	additem ITEM_TM78, 1
	additem ITEM_TM79, 1
	additem ITEM_TM80, 1
	additem ITEM_TM81, 1
	additem ITEM_TM82, 1
	additem ITEM_TM83, 1
	additem ITEM_TM84, 1
	additem ITEM_TM85, 1
	additem ITEM_TM86, 1
	additem ITEM_TM87, 1
	additem ITEM_TM88, 1
	additem ITEM_TM89, 1
	additem ITEM_TM90, 1
	additem ITEM_TM91, 1
	additem ITEM_TM92, 1
	additem ITEM_TM93, 1
	additem ITEM_TM94, 1
	additem ITEM_TM95, 1
	additem ITEM_TM96, 1
	additem ITEM_TM97, 1
	additem ITEM_TM98, 1
	additem ITEM_TM99, 1
	additem ITEM_TM100, 1
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

RR_Lab_Text_Oak_Welcome::
    .string "Oak: Welcome, welcome!\n"
    .string "Mind the cables. And the aides.\p"
    .string "Mostly the cables.$"

RR_Lab_Text_P_TripSecurity::
    .string "{PLAYER}: No, I'm sure the cables\n"
    .string "are across the floor on purpose.\p"
    .string "Trip-based security.\n"
    .string "Very modern.$"

RR_Lab_Text_Oak_WhyHere::
    .string "Oak: Now. Why I asked the four of\n"
    .string "you to Pallet Town.\p"
    .string "Three Kanto starters on top.\n"
    .string "Three from Johto below them,\l"
    .string "courtesy of Elm.$"

RR_Lab_Text_Elm_Mailed::
    .string "Elm: Hand-delivered! Well. Sent\n"
    .string "by PC transfer. With a card!$"

RR_Lab_Text_Jenny_Proofread::
    .string "Jenny: It was a very nice card.\n"
    .string "I proofread it.$"

RR_Lab_Text_Oak_Fridge::
    .string "Oak: It's on my fridge.$"

RR_Lab_Text_Oak_StudyForms::
    .string "Oak: While you stay in Pallet, I\n"
    .string "want each of you to study one.\p"
    .string "Observe it. Compare notes.\n"
    .string "All of it goes in the paper.$"

RR_Lab_Text_Ivy_InStarters::
    .string "Ivy: Regional forms?\n"
    .string "In starters?$"

RR_Lab_Text_Oak_OrangeIslands::
    .string "Oak: Pokémon change with where\n"
    .string "they live. You two proved that\l"
    .string "in the Orange Islands.$"

RR_Lab_Text_P_Snacks::
    .string "{PLAYER}: Mostly Ivy proved it.\n"
    .string "I carried the snacks.$"

RR_Lab_Text_Ivy_StickyNotes::
    .string "Ivy: And the field notes,\n"
    .string "sweetie.\p"
    .string "The notes were sticky.$"

RR_Lab_Text_P_Delicious::
    .string "{PLAYER}: The data was delicious.$"

RR_Lab_Text_Oak_OtherLands::
    .string "Oak: Old stories say the Johto\n"
    .string "three looked very different…\l"
    .string "in other lands.\p"
    .string "Other eras, even. Old scrolls.\n"
    .string "Snow where no snow should be.$"

RR_Lab_Text_Elm_OldTimey::
    .string "Elm: Other eras?\n"
    .string "Like… old-timey?$"

RR_Lab_Text_Jenny_Scroll::
    .string "Jenny: In my professional opinion,\n"
    .string "a scroll is not evidence.$"

RR_Lab_Text_Oak_MidBattle::
    .string "Oak: There are even whispers of\n"
    .string "Pokémon that change for a moment.\p"
    .string "Brilliantly. Mid-battle.\p"
    .string "…But that is a story for\n"
    .string "another lab.$"

RR_Lab_Text_Oak_ChooseFirst::
    .string "Oak: So! Choose a partner to\n"
    .string "study. {PLAYER}, you choose first.\p"
    .string "Elm went first last time.\n"
    .string "He chose a Bunsen burner.$"

RR_Lab_Text_Elm_GoodBurner::
    .string "Elm: It was a very good Bunsen\n"
    .string "burner!$"

RR_Lab_Text_Ivy_EastWing::
    .string "Ivy: Is that why the east wing\n"
    .string "is new?$"

RR_Lab_Text_Elm_Bunsen::
    .string "Elm: That was one Bunsen burner!$"

RR_Lab_Text_ChooseStarter::
    .string "Oak: Kanto on top, Johto below.\n"
    .string "Which one will you study?$"

RR_Lab_Text_StudyConfirm::
    .string "Oak: {STR_VAR_1}!\n"
    .string "Will you study this one?$"

RR_Lab_Text_Name_Bulbasaur::
    .string "Bulbasaur$"

RR_Lab_Text_Name_Charmander::
    .string "Charmander$"

RR_Lab_Text_Name_Squirtle::
    .string "Squirtle$"

RR_Lab_Text_Name_Chikorita::
    .string "Chikorita$"

RR_Lab_Text_Name_Cyndaquil::
    .string "Cyndaquil$"

RR_Lab_Text_Name_Totodile::
    .string "Totodile$"

RR_Lab_Text_ReceivedStarter::
    .string "{PLAYER} received the {STR_VAR_1}\n"
    .string "from Prof. Oak!$"

RR_Lab_Text_Ivy_Picks::
    .string "Ivy: Then I'll study {STR_VAR_1}.\p"
    .string "Type matchups are half of\n"
    .string "research, sweetie. Just so you\l"
    .string "know.$"

RR_Lab_Text_P_MarriedToIt::
    .string "{PLAYER}: I know. I've been married\n"
    .string "to that fact for years.$"

RR_Lab_Text_Elm_Picks::
    .string "Elm: Then I'll answer with\n"
    .string "{STR_VAR_1}! History repeats!$"

RR_Lab_Text_Jenny_Picks::
    .string "Jenny: {STR_VAR_1} for me. Someone\n"
    .string "has to keep you humble, dear.$"

RR_Lab_Text_Oak_Overslept::
    .string "Oak: The last time this table held\n"
    .string "starters, a boy from down the\l"
    .string "road overslept.$"

RR_Lab_Text_Elm_Ash::
    .string "Elm: Ash!$"

RR_Lab_Text_Oak_Gary::
    .string "Oak: And my grandson Gary left\n"
    .string "before the dew had dried.\p"
    .string "Off to every Gym in Kanto.$"

RR_Lab_Text_Ivy_OutThere::
    .string "Ivy: They're out there somewhere\n"
    .string "right now, aren't they?$"

RR_Lab_Text_Oak_Postcards::
    .string "Oak: Somewhere. Sending me very\n"
    .string "few postcards.$"

RR_Lab_Text_Elm_PushIt::
    .string "Elm: …Professor. What if the best\n"
    .string "way to study a Pokémon's\l"
    .string "potential is to push it?$"

RR_Lab_Text_P_HereItComes::
    .string "{PLAYER}: Here it comes.$"

RR_Lab_Text_Elm_GymChallenge::
    .string "Elm: The Gym Challenge!\n"
    .string "Real battles! Real growth!\l"
    .string "Real data!$"

RR_Lab_Text_Oak_Quietly::
    .string "Oak: Elm, the point was to\n"
    .string "observe them here. Quietly.$"

RR_Lab_Text_Jenny_NotASetting::
    .string "Jenny: “Quietly” is not a setting\n"
    .string "he has, Professor.$"

RR_Lab_Text_Elm_Scared::
    .string "Elm: Unless {PLAYER} is scared.$"

RR_Lab_Text_P_Paperwork::
    .string "{PLAYER}: I'm a professor. I'm\n"
    .string "scared of everything.\p"
    .string "Mostly paperwork.$"

RR_Lab_Text_P_AskIvy::
    .string "{PLAYER}: Ivy? Want in? I could\n"
    .string "use a better strategist.$"

RR_Lab_Text_Ivy_Yes::
    .string "Ivy: You mean any strategist.\n"
    .string "Yes, sweetie. I'm in.$"

RR_Lab_Text_Elm_AskJenny::
    .string "Elm: Jenny? Will you join my team?\n"
    .string "Our team. The team that is us.$"

RR_Lab_Text_Jenny_Yes::
    .string "Jenny: I married into this team,\n"
    .string "dear. Of course I will.$"

RR_Lab_Text_Ivy_SideBet::
    .string "Ivy: Jenny. Side bet:\n"
    .string "we carry them both?$"

RR_Lab_Text_Jenny_Absolutely::
    .string "Jenny: Absolutely. I've seen him\n"
    .string "battle.$"

RR_Lab_Text_Elm_TestNow::
    .string "Elm: A test, then! Right here!\n"
    .string "Right now! For science!$"

RR_Lab_Text_Elm_Defeat::
    .string "Elm: That was the warm-up!\n"
    .string "…It was not the warm-up.$"

RR_Lab_Text_Jenny_Defeat::
    .string "Jenny: Noted.\n"
    .string "For the report.$"

RR_Lab_Text_Elm_Victory::
    .string "Elm: Ha! History!\n"
    .string "Write that down, Jenny!$"

RR_Lab_Text_Jenny_Diagram::
    .string "Jenny: Already written, dear.\n"
    .string "With a diagram.$"

RR_Lab_Text_Ivy_Morale::
    .string "Ivy: We let them win, sweetie.\n"
    .string "For morale.$"

RR_Lab_Text_Elm_PureScience::
    .string "Elm: Nobody let anybody win!\n"
    .string "That was pure science!$"

RR_Lab_Text_Elm_BestOfFive::
    .string "Elm: Again! Best of three!\n"
    .string "Best of five!$"

RR_Lab_Text_Jenny_BestOfZero::
    .string "Jenny: Best of zero.\n"
    .string "We're in a lab, dear.$"

RR_Lab_Text_Oak_Properly::
    .string "Oak: Hmm. If you're going to do\n"
    .string "this unorthodox thing, do it\l"
    .string "properly.$"

RR_Lab_Text_Oak_Challenge::
    .string "Oak: {PLAYER}. Ivy. The two of\n"
    .string "you, against me. Two on two.\p"
    .string "{STR_VAR_1} and {STR_VAR_2} have\n"
    .string "been restless anyway.$"

RR_Lab_Text_OakBattleAsk::
    .string "Battle Prof. Oak?$"

RR_Lab_Text_Ivy_LikeThis::
    .string "Ivy: Oh, I like this.$"

RR_Lab_Text_Oak_Defeat::
    .string "Oak: I may have over-prepared\n"
    .string "the speech and under-prepared\l"
    .string "the Pokémon.$"

RR_Lab_Text_Oak_MoreInYou::
    .string "Oak: Well, well. Perhaps you\n"
    .string "have more in you than my notes\l"
    .string "say.$"

RR_Lab_Text_Oak_Haiku::
    .string "Oak: A student falls down…\n"
    .string "Old man forgot to hold back…\p"
    .string "The grant still goes through.$"

RR_Lab_Text_Elm_Funding::
    .string "Elm: Then I'll fund it! Out of\n"
    .string "my own pocket! …Jenny, how much\l"
    .string "is in my pocket?$"

RR_Lab_Text_P_AfterPaperwork::
    .string "{PLAYER}: After the paperwork,\n"
    .string "Professor. Maybe.$"

RR_Lab_Text_Oak_Wise::
    .string "Oak: Wise. Paperwork first.$"

RR_Lab_Text_Oak_Grant::
    .string "Oak: Now, the boring part.\n"
    .string "Paperwork!\p"
    .string "The League Research Fund approved\n"
    .string "your fieldwork in full!$"

RR_Lab_Text_ReceivedGrant::
    .string "{PLAYER} received the\n"
    .string "Research Grant supplies!$"

RR_Lab_Text_GrantContents::
    .string "Every Poké Ball, Master Balls too,\n"
    .string "medicine, held items, key items,\l"
    .string "and every TM and HM went in!$"

RR_Lab_Text_ReceivedPokedex::
    .string "{PLAYER} received a Pokédex\n"
    .string "from Prof. Oak!$"

RR_Lab_Text_P_LotOfBalls::
    .string "{PLAYER}: That is… a lot\n"
    .string "of Poké Balls.$"

RR_Lab_Text_Ivy_BuyOutMart::
    .string "Ivy: Professor, did you buy out\n"
    .string "the whole Poké Mart?$"

RR_Lab_Text_Oak_CarriedAway::
    .string "Oak: I may have got carried away.\n"
    .string "The clerk sent me a card.$"

RR_Lab_Text_Jenny_Trust::
    .string "Jenny: Master Balls. Plural.\n"
    .string "That's a lot of trust, sir.$"

RR_Lab_Text_Oak_RanchPager::
    .string "Oak: And the best part: a Ranch\n"
    .string "Pager! Page my ranch, and one of\l"
    .string "my Pokémon comes to Fly, Flash,\l"
    .string "Teleport or Dig for you.\p"
    .string "If nobody on your team can Cut or\n"
    .string "Surf, mine will step in too!\p"
    .string "Anything you don't carry waits in\n"
    .string "your PC. Open it from the Start\l"
    .string "menu, anywhere.$"

RR_Lab_Text_Ivy_OnCall::
    .string "Ivy: So your Pidgeot is on call\n"
    .string "now?$"

RR_Lab_Text_Oak_Exercise::
    .string "Oak: Pidgeot, Lapras, Tauros…\n"
    .string "They love the exercise!$"

RR_Lab_Text_Elm_SecondGrant::
    .string "Elm: Can I get a grant?$"

RR_Lab_Text_Oak_SameGrant::
    .string "Oak: You got one, Elm.\n"
    .string "It's the same grant.$"

RR_Lab_Text_Oak_GoSeeDaisy::
    .string "Oak: Go and see Daisy. She has\n"
    .string "Town Maps. And patience.\p"
    .string "Mostly patience.$"

RR_Lab_Text_Elm_MeetAtDaisys::
    .string "Elm: Meet you at Daisy's!\n"
    .string "We'll make the bet official!$"

RR_Lab_Text_Ivy_WaitOutside::
    .string "Ivy: Daisy's house next.\n"
    .string "Lead the way.$"

RR_Lab_Text_IvyMonJoins::
    .string "Ivy's {STR_VAR_1} joined the team!\n"
    .string "You and Ivy battle together.$"

@ ---------------------------------------------------------------
@ Elm and Jenny won, so they face Oak; the player watches (AI vs AI).
@ Placeholder dialogue until the owner's scene spec.
@ ---------------------------------------------------------------
RR_Lab_EventScript_OakVsElm::
	msgbox RR_Lab_Text_Oak_ElmJennyYouWon, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Elm_HoldMyNotebook, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Jenny_Accept, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Ivy_Popcorn, MSGBOX_DEFAULT
	@ Watching is optional (#46): skipping picks a winner at random
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_WatchAsk, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RR_Lab_EventScript_OakVsElmSkip
	closemessage
	switch VAR_RR_STARTER
	case 0, RR_Lab_EventScript_ElmTeam0
	case 1, RR_Lab_EventScript_ElmTeam1
	case 2, RR_Lab_EventScript_ElmTeam2
	case 3, RR_Lab_EventScript_ElmTeam3
	case 4, RR_Lab_EventScript_ElmTeam4
	case 5, RR_Lab_EventScript_ElmTeam5

RR_Lab_EventScript_OakVsElmBattle::
	callnative RR_ScriptBorrowRivalTeam
	setflag FLAG_RR_AI_VS_AI
	setflag FLAG_RR_NO_WHITEOUT
	switch VAR_RR_STARTER
	case 0, RR_Lab_EventScript_OakBattleWatch0
	case 1, RR_Lab_EventScript_OakBattleWatch1
	case 2, RR_Lab_EventScript_OakBattleWatch2
	case 3, RR_Lab_EventScript_OakBattleWatch3
	case 4, RR_Lab_EventScript_OakBattleWatch4
	case 5, RR_Lab_EventScript_OakBattleWatch5

RR_Lab_EventScript_ElmTeam0::
	setvar VAR_0x8004, SPECIES_CYNDAQUIL
	setvar VAR_0x8005, SPECIES_TOTODILE
	goto RR_Lab_EventScript_OakVsElmBattle

RR_Lab_EventScript_OakBattleWatch0::
	trainerbattle_no_intro TRAINER_RR_OAK_LAB_SQUIRTLE_CHIKORITA, RR_Lab_Text_Oak_Defeat
	goto RR_Lab_EventScript_OakVsElmResult

RR_Lab_EventScript_ElmTeam1::
	setvar VAR_0x8004, SPECIES_TOTODILE
	setvar VAR_0x8005, SPECIES_CHIKORITA
	goto RR_Lab_EventScript_OakVsElmBattle

RR_Lab_EventScript_OakBattleWatch1::
	trainerbattle_no_intro TRAINER_RR_OAK_LAB_BULBASAUR_CYNDAQUIL, RR_Lab_Text_Oak_Defeat
	goto RR_Lab_EventScript_OakVsElmResult

RR_Lab_EventScript_ElmTeam2::
	setvar VAR_0x8004, SPECIES_CHIKORITA
	setvar VAR_0x8005, SPECIES_CYNDAQUIL
	goto RR_Lab_EventScript_OakVsElmBattle

RR_Lab_EventScript_OakBattleWatch2::
	trainerbattle_no_intro TRAINER_RR_OAK_LAB_CHARMANDER_TOTODILE, RR_Lab_Text_Oak_Defeat
	goto RR_Lab_EventScript_OakVsElmResult

RR_Lab_EventScript_ElmTeam3::
	setvar VAR_0x8004, SPECIES_CHARMANDER
	setvar VAR_0x8005, SPECIES_SQUIRTLE
	goto RR_Lab_EventScript_OakVsElmBattle

RR_Lab_EventScript_OakBattleWatch3::
	trainerbattle_no_intro TRAINER_RR_OAK_LAB_TOTODILE_BULBASAUR, RR_Lab_Text_Oak_Defeat
	goto RR_Lab_EventScript_OakVsElmResult

RR_Lab_EventScript_ElmTeam4::
	setvar VAR_0x8004, SPECIES_SQUIRTLE
	setvar VAR_0x8005, SPECIES_BULBASAUR
	goto RR_Lab_EventScript_OakVsElmBattle

RR_Lab_EventScript_OakBattleWatch4::
	trainerbattle_no_intro TRAINER_RR_OAK_LAB_CHIKORITA_CHARMANDER, RR_Lab_Text_Oak_Defeat
	goto RR_Lab_EventScript_OakVsElmResult

RR_Lab_EventScript_ElmTeam5::
	setvar VAR_0x8004, SPECIES_BULBASAUR
	setvar VAR_0x8005, SPECIES_CHARMANDER
	goto RR_Lab_EventScript_OakVsElmBattle

RR_Lab_EventScript_OakBattleWatch5::
	trainerbattle_no_intro TRAINER_RR_OAK_LAB_CYNDAQUIL_SQUIRTLE, RR_Lab_Text_Oak_Defeat
	goto RR_Lab_EventScript_OakVsElmResult

RR_Lab_EventScript_OakVsElmResult::
	specialvar VAR_RESULT, GetBattleOutcome
	clearflag FLAG_RR_AI_VS_AI
	clearflag FLAG_RR_NO_WHITEOUT
	callnative RR_ScriptReturnPlayerTeam
	special HealPlayerParty
	textcolor NPC_TEXT_COLOR_NEUTRAL
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, RR_Lab_EventScript_OakLostToElm
RR_Lab_EventScript_OakBeatElm::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_BeatElm, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Elm_Rematch, MSGBOX_DEFAULT
	goto RR_Lab_EventScript_Grant

RR_Lab_EventScript_OakLostToElm::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_ElmWon, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Elm_Science, MSGBOX_DEFAULT
	goto RR_Lab_EventScript_Grant

@ Skipped the Elm and Jenny vs Oak battle: a coin flip decides it
RR_Lab_EventScript_OakVsElmSkip::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_BattleBlur, MSGBOX_DEFAULT
	special HealPlayerParty
	random 2
	goto_if_eq VAR_RESULT, 0, RR_Lab_EventScript_OakLostToElm
	goto RR_Lab_EventScript_OakBeatElm

RR_Lab_Text_WatchAsk::
    .string "Watch Elm and Jenny battle\n"
    .string "Prof. Oak?$"

RR_Lab_Text_BattleBlur::
    .string "The battle is a blur of flashes,\n"
    .string "cries and Elm shouting.$"

@ The lab's Slowpoke (the one from the intro call, still on the line)
RR_Lab_EventScript_Slowpoke::
	lock
	faceplayer
	playmoncry SPECIES_SLOWPOKE, CRY_MODE_NORMAL
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Slowpoke, MSGBOX_DEFAULT
	waitmoncry
	release
	end

RR_Lab_Text_Slowpoke::
    .string "Slowpoke: …Slooow?\p"
    .string "It's still holding the phone from\n"
    .string "this morning's call.$"

@ --- Oak scene variants (placeholder dialogue) ---
RR_Lab_Text_Oak_ElmJennyYouWon::
    .string "Oak: Elm. Jenny. You won, so you\n"
    .string "face me. Two on two.$"

RR_Lab_Text_Elm_HoldMyNotebook::
    .string "Elm: Us? Against you?\n"
    .string "Jenny, hold my notebook.$"

RR_Lab_Text_Jenny_Accept::
    .string "Jenny: Holding it. And accepting.$"

RR_Lab_Text_Ivy_Popcorn::
    .string "Ivy: Sweetie, we get front row\n"
    .string "seats. Somebody find popcorn.$"

RR_Lab_Text_Oak_BeatElm::
    .string "Oak: Ho ho! Not bad. Not bad at\n"
    .string "all. Still mine, though.$"

RR_Lab_Text_Elm_Rematch::
    .string "Elm: Best of thr- No. Good\n"
    .string "battle, Professor. Really.$"

RR_Lab_Text_Oak_ElmWon::
    .string "Oak: Well! Beaten in my own lab.\n"
    .string "Gary will never let me hear it.$"

RR_Lab_Text_Elm_Science::
    .string "Elm: For science! And for\n"
    .string "bragging rights! Mostly that.$"

RR_Lab_Text_Oak_CantFund::
    .string "Oak: Well. I suppose we can't\n"
    .string "really fund it now. Not after a\l"
    .string "showing like that.$"

RR_Lab_Text_Oak_SweetThought::
    .string "Oak: Ho ho! A sweet thought, Elm.\n"
    .string "But it was funded all along.$"

RR_Lab_Text_Oak_MissedHaiku::
    .string "Oak: A battle declined…\n"
    .string "The chance drifts off like a leaf…\p"
    .string "We carry on still.$"

RR_Lab_Text_Elm_StillFunding::
    .string "Elm: But… we still get funding\n"
    .string "for all this, right? Right?$"

RR_Lab_Text_Oak_HaveItCovered::
    .string "Oak: Don't worry, Elm.\n"
    .string "I have it covered.$"
