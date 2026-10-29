@ Research Red: Oak's lab: starter picker, chain, battles, grant (PalletTown_ProfessorOaksLab, triggered from map)
@ Moved out of the FireRed map files; the map keeps only its map_script hooks.

@ The empty starter table, where the lab's Slowpoke naps after the scene
.set RR_LAB_NAP_X, 9
.set RR_LAB_NAP_Y, 4

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
	@ Every starter comes with a Normal move, a status move and its early
	@ same-type move, so all six have a fair first battle (#87)
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
	givemon SPECIES_BULBASAUR, 5, move1=MOVE_TACKLE, move2=MOVE_GROWL, move3=MOVE_VINE_WHIP
	goto RR_Lab_EventScript_Received

RR_Lab_EventScript_PickCharmander::
	bufferspeciesname STR_VAR_1, SPECIES_CHARMANDER
	msgbox RR_Lab_Text_StudyConfirm, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RR_Lab_EventScript_ChooseLoop
	setvar VAR_RR_STARTER, 1
	setvar VAR_STARTER_MON, 2
	removeobject LOCALID_CHARMANDER_BALL
	givemon SPECIES_CHARMANDER, 5, move1=MOVE_SCRATCH, move2=MOVE_GROWL, move3=MOVE_EMBER
	goto RR_Lab_EventScript_Received

RR_Lab_EventScript_PickSquirtle::
	bufferspeciesname STR_VAR_1, SPECIES_SQUIRTLE
	msgbox RR_Lab_Text_StudyConfirm, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RR_Lab_EventScript_ChooseLoop
	setvar VAR_RR_STARTER, 2
	setvar VAR_STARTER_MON, 1
	removeobject LOCALID_SQUIRTLE_BALL
	givemon SPECIES_SQUIRTLE, 5, move1=MOVE_TACKLE, move2=MOVE_TAIL_WHIP, move3=MOVE_WATER_GUN
	goto RR_Lab_EventScript_Received

RR_Lab_EventScript_PickChikorita::
	bufferspeciesname STR_VAR_1, SPECIES_CHIKORITA
	msgbox RR_Lab_Text_StudyConfirm, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RR_Lab_EventScript_ChooseLoop
	setvar VAR_RR_STARTER, 3
	setvar VAR_STARTER_MON, 0
	removeobject LOCALID_LAB_CHIKORITA_BALL
	givemon SPECIES_CHIKORITA, 5, move1=MOVE_TACKLE, move2=MOVE_GROWL, move3=MOVE_RAZOR_LEAF
	goto RR_Lab_EventScript_Received

RR_Lab_EventScript_PickCyndaquil::
	bufferspeciesname STR_VAR_1, SPECIES_CYNDAQUIL
	msgbox RR_Lab_Text_StudyConfirm, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RR_Lab_EventScript_ChooseLoop
	setvar VAR_RR_STARTER, 4
	setvar VAR_STARTER_MON, 2
	removeobject LOCALID_LAB_CYNDAQUIL_BALL
	givemon SPECIES_CYNDAQUIL, 5, move1=MOVE_TACKLE, move2=MOVE_LEER, move3=MOVE_EMBER
	goto RR_Lab_EventScript_Received

RR_Lab_EventScript_PickTotodile::
	bufferspeciesname STR_VAR_1, SPECIES_TOTODILE
	msgbox RR_Lab_Text_StudyConfirm, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RR_Lab_EventScript_ChooseLoop
	setvar VAR_RR_STARTER, 5
	setvar VAR_STARTER_MON, 1
	removeobject LOCALID_LAB_TOTODILE_BALL
	givemon SPECIES_TOTODILE, 5, move1=MOVE_SCRATCH, move2=MOVE_LEER, move3=MOVE_WATER_GUN
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
	givemon SPECIES_CHARMANDER, 5, move1=MOVE_SCRATCH, move2=MOVE_GROWL, move3=MOVE_EMBER
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
	givemon SPECIES_SQUIRTLE, 5, move1=MOVE_TACKLE, move2=MOVE_TAIL_WHIP, move3=MOVE_WATER_GUN
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
	givemon SPECIES_BULBASAUR, 5, move1=MOVE_TACKLE, move2=MOVE_GROWL, move3=MOVE_VINE_WHIP
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
	givemon SPECIES_CYNDAQUIL, 5, move1=MOVE_TACKLE, move2=MOVE_LEER, move3=MOVE_EMBER
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
	givemon SPECIES_TOTODILE, 5, move1=MOVE_SCRATCH, move2=MOVE_LEER, move3=MOVE_WATER_GUN
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
	givemon SPECIES_CHIKORITA, 5, move1=MOVE_TACKLE, move2=MOVE_GROWL, move3=MOVE_RAZOR_LEAF
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
	trainerbattle_no_intro TRAINER_RR_ELM_LAB_CYNDAQUIL, RR_Lab_Text_Elm_Defeat
	goto RR_Lab_EventScript_BattleResult

@ Player picked Charmander: Elm Totodile, Jenny Chikorita
RR_Lab_EventScript_BattleAfterCharmander::
	trainerbattle_no_intro TRAINER_RR_ELM_LAB_TOTODILE, RR_Lab_Text_Elm_Defeat
	goto RR_Lab_EventScript_BattleResult

@ Player picked Squirtle: Elm Chikorita, Jenny Cyndaquil
RR_Lab_EventScript_BattleAfterSquirtle::
	trainerbattle_no_intro TRAINER_RR_ELM_LAB_CHIKORITA, RR_Lab_Text_Elm_Defeat
	goto RR_Lab_EventScript_BattleResult

@ Player picked Chikorita: Elm Charmander, Jenny Squirtle
RR_Lab_EventScript_BattleAfterChikorita::
	trainerbattle_no_intro TRAINER_RR_ELM_LAB_CHARMANDER, RR_Lab_Text_Elm_Defeat
	goto RR_Lab_EventScript_BattleResult

@ Player picked Cyndaquil: Elm Squirtle, Jenny Bulbasaur
RR_Lab_EventScript_BattleAfterCyndaquil::
	trainerbattle_no_intro TRAINER_RR_ELM_LAB_SQUIRTLE, RR_Lab_Text_Elm_Defeat
	goto RR_Lab_EventScript_BattleResult

@ Player picked Totodile: Elm Bulbasaur, Jenny Charmander
RR_Lab_EventScript_BattleAfterTotodile::
	trainerbattle_no_intro TRAINER_RR_ELM_LAB_BULBASAUR, RR_Lab_Text_Elm_Defeat
	goto RR_Lab_EventScript_BattleResult

RR_Lab_EventScript_BattleResult::
	clearflag FLAG_RR_NO_WHITEOUT
	special HealPlayerParty
	fadedefaultbgm
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, RR_Lab_EventScript_AfterBattle
	@ Lost: Elm and Jenny gloat, then take on Oak if you want to watch (#105)
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Elm_Victory, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Jenny_Diagram, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Ivy_Morale, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Elm_PureScience, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Elm_OnARoll, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_WatchOakAsk, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, RR_Lab_EventScript_ElmChallengesOak
	msgbox RR_Lab_Text_Ivy_LetThemHaveIt, MSGBOX_DEFAULT
	goto RR_Lab_EventScript_OakOffer

@ Won (#105, owner 2026-10-28): Oak asks you right away
RR_Lab_EventScript_AfterBattle::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Elm_BestOfFive, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Jenny_BestOfZero, MSGBOX_DEFAULT
	goto RR_Lab_EventScript_OakOffer

@ #105, after a loss and only if you want to watch: Elm and Jenny (Lv 6) take
@ on Oak's two leftover starters (Lv 5) and win; Elm asks for another round
@ and Oak's Lv 50 ranch Pokemon flatten them (Lv 7, optional to watch).
@ Both are scripted fights (src/rr_scripted_fight.c): the same every time.
RR_Lab_EventScript_ElmChallengesOak::
	closemessage
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	applymovement LOCALID_OAKS_LAB_RIVAL, Common_Movement_WalkInPlaceFasterUp
	applymovement LOCALID_LAB_JENNY, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Jenny_Protocol, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Oak_Accepts, MSGBOX_DEFAULT
	call RR_Lab_EventScript_HealElmJenny
	call RR_Lab_EventScript_SetupPair
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_StartersGo, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Ivy_Popcorn, MSGBOX_DEFAULT
	closemessage
	setvar VAR_0x8006, RR_FIGHT_LAB_STARTERS
	call RR_Lab_EventScript_ScriptedFight
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Elm_WeBeatOak, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Jenny_Noted, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Oak_Invigorated, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Elm_AnotherRound, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Jenny_LookAtHim, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Oak_RanchTeam, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Elm_Pidgeot, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Jenny_Incident, MSGBOX_DEFAULT
	@ round two is optional to watch (#46): Oak's ranch Pokemon at Lv 50
	msgbox RR_Lab_Text_WatchAsk, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RR_Lab_EventScript_RanchSkip
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_HealAgain, MSGBOX_DEFAULT
	call RR_Lab_EventScript_HealElmJenny
	closemessage
	setvar VAR_0x8006, RR_FIGHT_LAB_RANCH
	call RR_Lab_EventScript_ScriptedFight
	goto RR_Lab_EventScript_RanchAftermath

RR_Lab_EventScript_RanchSkip::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_BattleBlur, MSGBOX_DEFAULT
RR_Lab_EventScript_RanchAftermath::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_RanchAftermath, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Elm_Thesis, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Oak_CalmsDown, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Jenny_OakFast, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Oak_TaurosHaiku, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Elm_Allergies, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Ivy_OakGlow, MSGBOX_DEFAULT
@ --- your turn, if you like: never forced, healed first. One fight with
@ Oak's two leftover starters (Lv 5); if you say No it waits until Route 22:
@ talk to Oak in the lab (#105, owner 2026-10-28) ---
RR_Lab_EventScript_OakOffer::
	call RR_Lab_EventScript_SetupPair
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_Challenge, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_OakBattleAsk, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RR_Lab_EventScript_OakDeclined
	call RR_Lab_EventScript_OakFirstFight
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, RR_Lab_EventScript_OakBeaten
	@ Loss: Oak's haiku, then Elm offers his own funding
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_CantFund, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Elm_Funding, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Oak_SweetThought, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Oak_Haiku, MSGBOX_DEFAULT
	goto RR_Lab_EventScript_TableNap

RR_Lab_EventScript_OakBeaten::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_MoreInYou, MSGBOX_DEFAULT
	goto RR_Lab_EventScript_TableNap

RR_Lab_EventScript_OakDeclined::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_P_AfterPaperwork, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Oak_AnyTime, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Elm_StillFunding, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_Oak_HaveItCovered, MSGBOX_DEFAULT

@ Oak keeps the two leftover starters (they're in his rematch team); the
@ table is empty, and the lab's Slowpoke claims it for naps from now on
RR_Lab_EventScript_TableNap::
	call RR_Lab_EventScript_SetupPair
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_KeepStarters, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_BULBASAUR_BALL
	removeobject LOCALID_SQUIRTLE_BALL
	removeobject LOCALID_CHARMANDER_BALL
	removeobject LOCALID_LAB_CHIKORITA_BALL
	removeobject LOCALID_LAB_CYNDAQUIL_BALL
	removeobject LOCALID_LAB_TOTODILE_BALL
	call RR_Lab_EventScript_SlowpokeOnTable
	setobjectxy LOCALID_LAB_SLOWPOKE, RR_LAB_NAP_X, RR_LAB_NAP_Y
	turnobject LOCALID_LAB_SLOWPOKE, DIR_SOUTH
	fadescreen FADE_FROM_BLACK
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_SlowpokeTable, MSGBOX_DEFAULT
	playmoncry SPECIES_SLOWPOKE, CRY_MODE_NORMAL
	waitmoncry
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_GoodTable, MSGBOX_DEFAULT
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
	msgbox RR_Lab_Text_P_NoBalls, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_LabSupplies, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Ivy_Cupboard, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Jenny_MasterBalls, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_NotThose, MSGBOX_DEFAULT
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

@ The grant: only reusable things (key items, HMs, every TM). Poké Balls,
@ medicine and held items come free from Lab Supplies on every PC
@ (data/scripts/pc.inc, src/shop.c, gRRLabSupplies in src/research_red.c).
RR_Lab_EventScript_GiveGrant::
	@ key items (story items like the Town Map stay in their scenes)
	additem ITEM_EXP_SHARE, 1
	additem ITEM_BICYCLE, 1
	additem ITEM_SUPER_ROD, 1 @ the one Fishing Rod (renamed Super Rod)
	additem ITEM_ITEMFINDER, 1
	additem ITEM_VS_SEEKER, 1
	additem ITEM_BERRY_POUCH, 1
	additem ITEM_RANCH_PAGER, 1 @ Research Red (#29)
	setflag FLAG_RR_EXP_SHARE
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
    .string "…It was not the warm-up.\p"
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

RR_Lab_Text_Oak_Challenge::
    .string "Oak: Now then! {PLAYER}. Ivy.\n"
    .string "{STR_VAR_1} and {STR_VAR_2} are\l"
    .string "still raring to go.\p"
    .string "Fancy a round? No pressure. I'll\n"
    .string "heal your team first.$"

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
    .string "Research Grant!$"

RR_Lab_Text_GrantContents::
    .string "Key items, every HM, and all 100\n"
    .string "TMs. TMs never run out!$"

RR_Lab_Text_ReceivedPokedex::
    .string "{PLAYER} received a Pokédex\n"
    .string "from Prof. Oak!$"

RR_Lab_Text_P_NoBalls::
    .string "{PLAYER}: Fieldwork… with no\n"
    .string "Poké Balls?$"

RR_Lab_Text_Oak_LabSupplies::
    .string "Oak: Ho ho! Something better:\n"
    .string "Lab Supplies, on every PC!\p"
    .string "Poké Balls, medicine, held items.\n"
    .string "Take whatever your research needs.\p"
    .string "The fund pays. You never do!$"

RR_Lab_Text_Ivy_Cupboard::
    .string "Ivy: A free supply cupboard? Oh,\n"
    .string "sweetie, we're never coming home.$"

RR_Lab_Text_Jenny_MasterBalls::
    .string "Jenny: Master Balls too, sir? In\n"
    .string "my professional opinion… risky.$"

RR_Lab_Text_Oak_NotThose::
    .string "Oak: Not those! The Master Ball\n"
    .string "waits for a special occasion.\p"
    .string "As they say, a patient Slowpoke\n"
    .string "catches the biggest fish. Ho ho!$"

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
@ Oak heals Elm and Jenny's Pokémon (a fanfare and a line)
RR_Lab_EventScript_HealElmJenny::
	playfanfare MUS_HEAL
	message RR_Lab_Text_HealedElmJenny
	waitmessage
	waitfanfare
	return

@ The two starters Oak has left (STR_VAR_1, STR_VAR_2) and Elm and Jenny's
@ picks (VAR_0x8004, VAR_0x8005) for the starter the player chose
RR_Lab_EventScript_SetupPair::
	switch VAR_RR_STARTER
	case 0, RR_Lab_EventScript_Pair0
	case 1, RR_Lab_EventScript_Pair1
	case 2, RR_Lab_EventScript_Pair2
	case 3, RR_Lab_EventScript_Pair3
	case 4, RR_Lab_EventScript_Pair4
	case 5, RR_Lab_EventScript_Pair5
	return

@ Player picked Bulbasaur
RR_Lab_EventScript_Pair0::
	bufferspeciesname STR_VAR_1, SPECIES_SQUIRTLE
	bufferspeciesname STR_VAR_2, SPECIES_CHIKORITA
	setvar VAR_0x8004, SPECIES_CYNDAQUIL
	setvar VAR_0x8005, SPECIES_TOTODILE
	return

@ Player picked Charmander
RR_Lab_EventScript_Pair1::
	bufferspeciesname STR_VAR_1, SPECIES_BULBASAUR
	bufferspeciesname STR_VAR_2, SPECIES_CYNDAQUIL
	setvar VAR_0x8004, SPECIES_TOTODILE
	setvar VAR_0x8005, SPECIES_CHIKORITA
	return

@ Player picked Squirtle
RR_Lab_EventScript_Pair2::
	bufferspeciesname STR_VAR_1, SPECIES_CHARMANDER
	bufferspeciesname STR_VAR_2, SPECIES_TOTODILE
	setvar VAR_0x8004, SPECIES_CHIKORITA
	setvar VAR_0x8005, SPECIES_CYNDAQUIL
	return

@ Player picked Chikorita
RR_Lab_EventScript_Pair3::
	bufferspeciesname STR_VAR_1, SPECIES_TOTODILE
	bufferspeciesname STR_VAR_2, SPECIES_BULBASAUR
	setvar VAR_0x8004, SPECIES_CHARMANDER
	setvar VAR_0x8005, SPECIES_SQUIRTLE
	return

@ Player picked Cyndaquil
RR_Lab_EventScript_Pair4::
	bufferspeciesname STR_VAR_1, SPECIES_CHIKORITA
	bufferspeciesname STR_VAR_2, SPECIES_CHARMANDER
	setvar VAR_0x8004, SPECIES_SQUIRTLE
	setvar VAR_0x8005, SPECIES_BULBASAUR
	return

@ Player picked Totodile
RR_Lab_EventScript_Pair5::
	bufferspeciesname STR_VAR_1, SPECIES_CYNDAQUIL
	bufferspeciesname STR_VAR_2, SPECIES_SQUIRTLE
	setvar VAR_0x8004, SPECIES_BULBASAUR
	setvar VAR_0x8005, SPECIES_CHARMANDER
	return

@ Oak's two leftover starters as a double battle (the player's optional
@ battle, or Elm and Jenny's watched one)
RR_Lab_EventScript_OakStarterBattle::
	switch VAR_RR_STARTER
	case 0, RR_Lab_EventScript_OakBattle0
	case 1, RR_Lab_EventScript_OakBattle1
	case 2, RR_Lab_EventScript_OakBattle2
	case 3, RR_Lab_EventScript_OakBattle3
	case 4, RR_Lab_EventScript_OakBattle4
	case 5, RR_Lab_EventScript_OakBattle5
	return

RR_Lab_EventScript_OakBattle0::
	trainerbattle_no_intro TRAINER_RR_OAK_LAB_SQUIRTLE_CHIKORITA, RR_Lab_Text_Oak_Defeat
	return

RR_Lab_EventScript_OakBattle1::
	trainerbattle_no_intro TRAINER_RR_OAK_LAB_BULBASAUR_CYNDAQUIL, RR_Lab_Text_Oak_Defeat
	return

RR_Lab_EventScript_OakBattle2::
	trainerbattle_no_intro TRAINER_RR_OAK_LAB_CHARMANDER_TOTODILE, RR_Lab_Text_Oak_Defeat
	return

RR_Lab_EventScript_OakBattle3::
	trainerbattle_no_intro TRAINER_RR_OAK_LAB_TOTODILE_BULBASAUR, RR_Lab_Text_Oak_Defeat
	return

RR_Lab_EventScript_OakBattle4::
	trainerbattle_no_intro TRAINER_RR_OAK_LAB_CHIKORITA_CHARMANDER, RR_Lab_Text_Oak_Defeat
	return

RR_Lab_EventScript_OakBattle5::
	trainerbattle_no_intro TRAINER_RR_OAK_LAB_CYNDAQUIL_SQUIRTLE, RR_Lab_Text_Oak_Defeat
	return

@ #105: your one-time fight with Oak's leftover starters (heal first);
@ VAR_RESULT is the outcome. From the lab scene or by talking to Oak later
@ (before Route 22). No whiteout.
RR_Lab_EventScript_OakFirstFight::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Ivy_LikeThis, MSGBOX_DEFAULT
	playfanfare MUS_HEAL
	special HealPlayerParty
	message RR_Lab_Text_HealedYours
	waitmessage
	waitfanfare
	closemessage
	setflag FLAG_RR_OAK_FIRST_FIGHT
	setflag FLAG_RR_NO_WHITEOUT
	call RR_Lab_EventScript_OakStarterBattle
	clearflag FLAG_RR_NO_WHITEOUT
	special HealPlayerParty
	specialvar VAR_RESULT, GetBattleOutcome
	return

@ #105: talking to Oak after the lab scene and before Route 22: his one
@ fight if you haven't had it yet
RR_Lab_EventScript_OakTalkBeforeRoute22::
	goto_if_set FLAG_RR_OAK_FIRST_FIGHT, RR_Lab_EventScript_OakOffToViridian
	call RR_Lab_EventScript_SetupPair
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_StillOffer, MSGBOX_DEFAULT
	msgbox RR_Lab_Text_OakBattleAsk, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RR_Lab_EventScript_OakOffToViridian
	call RR_Lab_EventScript_OakFirstFight
	textcolor NPC_TEXT_COLOR_NEUTRAL
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, RR_Lab_EventScript_OakLaterWon
	msgbox RR_Lab_Text_Oak_Haiku, MSGBOX_DEFAULT
	release
	end

RR_Lab_EventScript_OakLaterWon::
	msgbox RR_Lab_Text_Oak_MoreInYou, MSGBOX_DEFAULT
	release
	end

RR_Lab_EventScript_OakOffToViridian::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Oak_OffYouGo, MSGBOX_DEFAULT
	release
	end

@ #105: a scripted fight (VAR_0x8006 = RR_FIGHT_*): Elm and Jenny's picks
@ against Oak's Pokemon, who stand in for your party until it ends
RR_Lab_EventScript_ScriptedFight::
	call RR_Lab_EventScript_SetupPair
	callnative RR_ScriptStartScriptedFight
	trainerbattle_no_intro TRAINER_RR_ELM_JENNY_WATCH, RR_Lab_Text_ElmJenny_RanchDefeat
	callnative RR_ScriptEndScriptedFight
	special HealPlayerParty
	return

@ After the lab scene the lab's Slowpoke naps on the empty starter table
@ (called from the lab's map script when the story is past the lab, too)
RR_Lab_EventScript_SlowpokeOnTable::
	setobjectxyperm LOCALID_LAB_SLOWPOKE, RR_LAB_NAP_X, RR_LAB_NAP_Y
	setobjectmovementtype LOCALID_LAB_SLOWPOKE, MOVEMENT_TYPE_FACE_DOWN
	return

@ --- #86 rework: Elm and Jenny vs Oak, then the table nap (drafts) ---
RR_Lab_Text_Elm_OnARoll::
    .string "Elm: We're on a roll, Jenny! Who's\n"
    .string "next? Professor Oak!$"

RR_Lab_Text_Elm_SomeoneElse::
    .string "Elm: Fine! Then we battle someone\n"
    .string "else. Professor Oak!$"

RR_Lab_Text_Jenny_Protocol::
    .string "Jenny: By protocol, dear, you ask\n"
    .string "before you challenge a legend.\p"
    .string "Jenny: Professor, would you battle\n"
    .string "us? Your two starters, our two.$"

RR_Lab_Text_Oak_Accepts::
    .string "Oak: Ho ho! A challenge in my own\n"
    .string "lab? Gladly!\p"
    .string "But a professor looks after his\n"
    .string "opponents. Let me heal yours.$"

RR_Lab_Text_HealedElmJenny::
    .string "Oak healed Elm and Jenny's\n"
    .string "Pokémon.$"

RR_Lab_Text_Oak_StartersGo::
    .string "Oak: {STR_VAR_1}! {STR_VAR_2}!\n"
    .string "Show them how it's done!$"

RR_Lab_Text_Elm_WeBeatOak::
    .string "Elm: We won! We beat Professor\n"
    .string "Oak! In his own lab!\p"
    .string "Jenny, call the papers! Call my\n"
    .string "mother! Call everyone!\p"
    .string "I'm going to frame this moment.\n"
    .string "I'm going to frame the floor!$"

RR_Lab_Text_Jenny_Noted::
    .string "Jenny: Noted, dear. The floor goes\n"
    .string "in the report.$"

RR_Lab_Text_Elm_AnotherRound::
    .string "Elm: Then let's go again! Same\n"
    .string "time, same place! Best of three!$"

RR_Lab_Text_Jenny_LookAtHim::
    .string "Jenny: Dear. Look at his face.$"

RR_Lab_Text_Jenny_Incident::
    .string "Jenny: I'll start the incident\n"
    .string "report now, dear.$"

RR_Lab_Text_Oak_HealAgain::
    .string "Oak: Fair's fair. A quick heal\n"
    .string "first. Again!$"

RR_Lab_Text_RanchAftermath::
    .string "Tauros snorts. Pidgeot's gust\n"
    .string "sends Elm's notes everywhere.$"

RR_Lab_Text_Elm_Thesis::
    .string "Elm: I saw my whole thesis flash\n"
    .string "before my eyes. It had typos.$"

RR_Lab_Text_Oak_CalmsDown::
    .string "Oak: Ho ho… oh dear. I got a\n"
    .string "little carried away there.\p"
    .string "Pidgeot, Tauros, back to the\n"
    .string "ranch. Thank you, old friends.$"

RR_Lab_Text_Oak_TaurosHaiku::
    .string "Oak: Two upstarts rose up…\n"
    .string "The old Tauros was hungry…\p"
    .string "Please stop crying, Elm.$"

RR_Lab_Text_Elm_Allergies::
    .string "Elm: It's allergies! Tauros shed\n"
    .string "on me!$"

RR_Lab_Text_HealedYours::
    .string "Oak healed your Pokémon.$"

RR_Lab_Text_Oak_KeepStarters::
    .string "Oak: {STR_VAR_1} and {STR_VAR_2}\n"
    .string "will stay with me. Someone has to\l"
    .string "keep this old man on his toes!$"

RR_Lab_Text_SlowpokeTable::
    .string "The lab's Slowpoke shuffles over\n"
    .string "to the empty table… very slowly.\p"
    .string "It climbs up and falls asleep.$"

RR_Lab_Text_Oak_GoodTable::
    .string "Oak: Ho ho! Well, it is a very\n"
    .string "good table.$"


RR_Lab_Text_WatchAsk::
    .string "Watch Elm and Jenny's second\n"
    .string "round?$"

RR_Lab_Text_BattleBlur::
    .string "Pidgeot's wings, Tauros's hooves,\n"
    .string "and Elm shouting. It's over fast.$"

@ The lab's Slowpoke (the one from the intro call, still on the line)
RR_Lab_EventScript_Slowpoke::
	lock
	goto_if_ge VAR_0x40FE, 3, RR_Lab_EventScript_SlowpokeNap
	faceplayer
	playmoncry SPECIES_SLOWPOKE, CRY_MODE_NORMAL
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Slowpoke, MSGBOX_DEFAULT
	waitmoncry
	release
	end

@ #86: after the lab scene it naps on the empty starter table (it doesn't
@ turn to face you; it's asleep)
RR_Lab_EventScript_SlowpokeNap::
	playmoncry SPECIES_SLOWPOKE, CRY_MODE_WEAK
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_SlowpokeNap, MSGBOX_DEFAULT
	waitmoncry
	release
	end

@ Daisy's Rattata, by her side when she visits for the rematch (#86)
RR_Lab_EventScript_Muffin::
	lock
	faceplayer
	playmoncry SPECIES_RATTATA, CRY_MODE_NORMAL
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lab_Text_Muffin, MSGBOX_DEFAULT
	waitmoncry
	release
	end

RR_Lab_Text_Slowpoke::
    .string "Slowpoke: …Slooow?\p"
    .string "It's still holding the phone from\n"
    .string "this morning's call.$"

RR_Lab_Text_SlowpokeNap::
    .string "Slowpoke: …Zzz… …Slow?\p"
    .string "It's fast asleep on the starter\n"
    .string "table. Prof. Oak doesn't mind.$"

RR_Lab_Text_Muffin::
    .string "Muffin: Rat! Tat tat!\p"
    .string "Daisy's Rattata eyes the napping\n"
    .string "Slowpoke. It wants the table too.$"

@ --- Oak scene variants (placeholder dialogue) ---
RR_Lab_Text_Ivy_Popcorn::
    .string "Ivy: Sweetie, we get front row\n"
    .string "seats. Somebody find popcorn.$"

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

RR_Lab_Text_Oak_RanchTeam::
    .string "Oak: Another round? Oh, I'd love\n"
    .string "to. But not with the starters.\p"
    .string "Pidgeot! Tauros! Out you come!$"

RR_Lab_Text_Elm_Pidgeot::
    .string "Elm: His ranch team?! Jenny, those\n"
    .string "are fully evolved!$"

@ Elm and Jenny's line when Oak's ranch team wins (the scripted fight)
RR_Lab_Text_ElmJenny_RanchDefeat::
    .string "Elm: We were winning! For one\n"
    .string "whole turn!$"

RR_Lab_Text_Jenny_OakFast::
    .string "Jenny: In my professional opinion,\n"
    .string "that was very fast, sir.$"

RR_Lab_Text_Oak_Invigorated::
    .string "Oak: Ho ho ho! Marvellous!\n"
    .string "I feel thirty years younger!$"

RR_Lab_Text_Ivy_OakGlow::
    .string "Ivy: Look at him, sweetie. He's\n"
    .string "glowing.$"

@ --- #105 lab order (drafts for the owner to rework) ---
RR_Lab_Text_WatchOakAsk::
    .string "Watch Elm and Jenny take on\n"
    .string "Prof. Oak?$"

RR_Lab_Text_Ivy_LetThemHaveIt::
    .string "Ivy: Let them have their moment,\n"
    .string "sweetie. We'll hear all about it.$"

RR_Lab_Text_Oak_AnyTime::
    .string "Oak: If you'd like to fight me\n"
    .string "with these, any time is fine.\p"
    .string "Oak: Before you head out past\n"
    .string "Viridian, that is!$"

RR_Lab_Text_Oak_StillOffer::
    .string "Oak: Ah, {PLAYER}! {STR_VAR_1} and\n"
    .string "{STR_VAR_2} are still raring to\l"
    .string "go. Fancy that round now?$"

RR_Lab_Text_Oak_OffYouGo::
    .string "Oak: Off you go! Viridian won't\n"
    .string "research itself.$"
