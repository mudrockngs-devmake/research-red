@ Research Red: First walk up Route 1 (Route1)
@ Moved out of the FireRed map files; the map keeps only its map_script hooks.

.set RR_ROUTE1_ELM_X, 16  @ Elm's column (map.json: Elm 16,3; Jenny 17,3)

@ ===================================================================
@ Research Red: first walk up Route 1 (VAR_0x40FE == 4, after Daisy's house)
@ Elm and Jenny wait at the north end; walking up to them starts the scene.
@ They react to what you caught on the way (src/research_red.c records
@ Route 1 catches), something streaks across the sky (Mewtwo, blurred), then they dash
@ ahead. Sets VAR_0x40FE to 5.
@ ===================================================================
@ Elm and Jenny only stand here while the scene is pending
RR_Route1_OnTransition::
	call_if_ne VAR_0x40FE, 4, RR_Route1_EventScript_HideElmJenny
	end

RR_Route1_EventScript_HideElmJenny::
	setflag FLAG_TEMP_11
	setflag FLAG_TEMP_12
	return

RR_Route1_EventScript_Walk::
	lockall
	@ Elm (16,3) waits where the path from the grass meets the top of the
	@ route, Jenny diagonally behind him (17,2) (#83). Step over to stand right
	@ in front of Elm (#30). Ivy is still the follower here, so she comes along.
	getplayerxy VAR_0x8004, VAR_0x8005
	call RR_Route1_EventScript_LineUpWithElm
	getplayerxy VAR_0x8004, VAR_0x8005
	copyvar VAR_0x8006, VAR_0x8005
	addvar VAR_0x8006, 1
	@ Ivy is already following; she becomes the scene's Ivy on her own tile
	rr_ivy_to_scene LOCALID_ROUTE1_IVY
	call_if_eq VAR_RESULT, FALSE, RR_Route1_EventScript_PlaceIvy
	playse SE_PIN
	applymovement LOCALID_ROUTE1_ELM, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Elm_WeMeetAgain, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Jenny_RightHere, MSGBOX_DEFAULT
	call RR_Route1_EventScript_CatchReactions
	@ #85: everyone walks on together, two by two: Elm and Jenny ahead on
	@ the top row, you and Ivy beside them, singing on the road to Viridian
	@ (song credited in CREDITS.md)
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Elm_WalkWithUs, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_ROUTE1_ELM, RR_Route1_Movement_ElmWalk
	applymovement LOCALID_ROUTE1_JENNY, RR_Route1_Movement_JennyWalk
	applymovement LOCALID_PLAYER, RR_Route1_Movement_PlayerWalk
	@ Ivy walks to her place beside you (14,4) from wherever she stood
	rr_walk_to LOCALID_ROUTE1_IVY, 14, 4, TRUE, 16
	waitmovement LOCALID_ROUTE1_IVY
	waitmovement LOCALID_ROUTE1_JENNY
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Elm_SingStart, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Elm_Sing1, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_P_Sing2, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Elm_Sing3, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_P_Trainer, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Elm_InMyHeart, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ShareALook, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Jenny_Evidence, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Ivy_ThatsWhy, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Jenny_CaseClosed, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Ivy_Sing4, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Jenny_Sing5, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Elm_KnowWords, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Jenny_Everyone, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Everyone_Sing, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Ivy_OffKey, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_P_OwnKey, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Ivy_Married, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Elm_Banter, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Jenny_Singing, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Elm_UpTheHill, MSGBOX_DEFAULT
	closemessage
	@ #85: up onto the path to Viridian, all four facing north
	applymovement LOCALID_ROUTE1_ELM, RR_Route1_Movement_ElmUp
	applymovement LOCALID_ROUTE1_JENNY, RR_Route1_Movement_JennyUp
	applymovement LOCALID_PLAYER, RR_Route1_Movement_PlayerUp
	applymovement LOCALID_ROUTE1_IVY, RR_Route1_Movement_IvyUp
	waitmovement LOCALID_ROUTE1_IVY
	waitmovement LOCALID_PLAYER
	delay 20
	@ --- something streaks across the sky (Mewtwo, blurred) ---
	playse SE_PIN
	applymovement LOCALID_ROUTE1_ELM, Common_Movement_ExclamationMark
	waitmovement 0
	special SpawnCameraObject
	applymovement LOCALID_CAMERA, RR_Route1_Movement_CameraUp
	waitmovement 0
	delay 10
	@ Mewtwo streaks across the sky, a blur (src/rr_flyby.c)
	callnative RR_ScriptMewtwoFlyby
	waitstate
	delay 30
	applymovement LOCALID_CAMERA, RR_Route1_Movement_CameraDown
	waitmovement 0
	special RemoveCameraObject
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Elm_SawIt, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_P_Believe, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Jenny_Believe, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Elm_Tail, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_P_Seventy, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Ivy_Sixty, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Elm_Race, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Jenny_Protocol, MSGBOX_DEFAULT
	closemessage
	@ Elm runs for Viridian, Jenny right behind him
	applymovement LOCALID_ROUTE1_ELM, RR_Route1_Movement_RunNorth
	applymovement LOCALID_ROUTE1_JENNY, RR_Route1_Movement_RunNorthLate
	waitmovement LOCALID_ROUTE1_JENNY
	removeobject LOCALID_ROUTE1_ELM
	removeobject LOCALID_ROUTE1_JENNY
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_DashedAhead, MSGBOX_DEFAULT
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	applymovement LOCALID_ROUTE1_IVY, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Ivy_Quiet, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_P_Wonderful, MSGBOX_DEFAULT
	closemessage
	rr_ivy_follow LOCALID_ROUTE1_IVY
	setvar VAR_0x40FE, 5
	releaseall
	end

RR_Route1_EventScript_LineUpWithElm::
	goto_if_gt VAR_0x8004, RR_ROUTE1_ELM_X, RR_Route1_EventScript_StepLeftToElm
	goto_if_lt VAR_0x8004, RR_ROUTE1_ELM_X, RR_Route1_EventScript_StepRightToElm
	return

RR_Route1_EventScript_StepLeftToElm::
	applymovement LOCALID_PLAYER, Common_Movement_WalkLeft
	waitmovement 0
	subvar VAR_0x8004, 1
	goto RR_Route1_EventScript_LineUpWithElm

RR_Route1_EventScript_StepRightToElm::
	applymovement LOCALID_PLAYER, Common_Movement_WalkRight
	waitmovement 0
	addvar VAR_0x8004, 1
	goto RR_Route1_EventScript_LineUpWithElm

@ No follower (e.g. an old save): put Ivy behind the player
RR_Route1_EventScript_PlaceIvy::
	setobjectxyperm LOCALID_ROUTE1_IVY, VAR_0x8004, VAR_0x8006
	addobject LOCALID_ROUTE1_IVY
	return

@ One tile: the group stands at y 1-2 for the fly-by, and panning past the
@ top of Route 1 into the Viridian connection leaves the camera offset.
RR_Route1_Movement_CameraUp:
	walk_up
	step_end

RR_Route1_Movement_CameraDown:
	walk_down
	step_end

@ #85: the walk. Elm (16,3) -> (12,3) on the top row, Jenny (17,2) steps
@ down behind him -> (13,3); you (16,4) -> (13,4) beside them, Ivy (14,4).
RR_Route1_Movement_ElmWalk:
	walk_left
	walk_left
	walk_left
	walk_left
	face_right
	step_end

RR_Route1_Movement_JennyWalk:
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	face_left
	step_end

RR_Route1_Movement_PlayerWalk:
	delay_8
	walk_left
	walk_left
	walk_left
	face_up
	step_end

@ Up onto the path to Viridian: Elm (12,1), Jenny (13,1), you (12,2), Ivy (13,2)
RR_Route1_Movement_ElmUp:
	walk_up
	walk_up
	step_end

RR_Route1_Movement_JennyUp:
	walk_up
	walk_up
	step_end

RR_Route1_Movement_PlayerUp:
	delay_8
	walk_left
	walk_up
	walk_up
	step_end

RR_Route1_Movement_IvyUp:
	delay_16
	walk_left
	walk_up
	walk_up
	step_end

RR_Route1_Movement_RunNorth:
	walk_fast_up
	set_invisible
	step_end

RR_Route1_Movement_RunNorthLate:
	delay_8
	walk_fast_up
	set_invisible
	step_end

RR_Route1_Text_Elm_WeMeetAgain::
    .string "Elm: Route One! We meet again!\n"
    .string "This time I brought a partner!$"

RR_Route1_Text_Jenny_RightHere::
    .string "Jenny: And a wife.\n"
    .string "I'm right here, dear.$"

RR_Route1_Text_Elm_SingStart::
    .string "Elm starts singing. {PLAYER} joins\n"
    .string "in on the second word.$"

RR_Route1_Text_Elm_Sing1::
    .string "Elm: {EMOJI_NOTE} We're on the road to\n"
    .string "Viridian City! {EMOJI_NOTE}$"

RR_Route1_Text_P_Sing2::
    .string "{PLAYER}: {EMOJI_NOTE} Meet my friends\n"
    .string "along the way! {EMOJI_NOTE}$"

RR_Route1_Text_Elm_Sing3::
    .string "Elm: {EMOJI_NOTE} I'm on the road to be the\n"
    .string "greatest… professor! {EMOJI_NOTE}$"

RR_Route1_Text_P_Trainer::
    .string "{PLAYER}: Trainer, Elm.$"

RR_Route1_Text_Elm_InMyHeart::
    .string "Elm: Professor in my heart!$"

RR_Route1_Text_ShareALook::
    .string "Ivy and Jenny share a look.$"

RR_Route1_Text_Jenny_Evidence::
    .string "Jenny: The evidence suggests…$"

RR_Route1_Text_Ivy_ThatsWhy::
    .string "Ivy: Yep. That's why they're\n"
    .string "friends.$"

RR_Route1_Text_Jenny_CaseClosed::
    .string "Jenny: Case closed.$"

RR_Route1_Text_Ivy_Sing4::
    .string "Ivy: {EMOJI_NOTE} We keep on trying… {EMOJI_NOTE}$"

RR_Route1_Text_Jenny_Sing5::
    .string "Jenny: {EMOJI_NOTE} …and then we try\n"
    .string "some more! {EMOJI_NOTE}$"

RR_Route1_Text_Elm_KnowWords::
    .string "Elm: Jenny! You know the words?!$"

RR_Route1_Text_Jenny_Everyone::
    .string "Jenny: Dear. Everyone knows\n"
    .string "the words.$"

RR_Route1_Text_Everyone_Sing::
    .string "Everyone sings together:\n"
    .string "{EMOJI_NOTE} To find a place worth\l"
    .string "fighting for! {EMOJI_NOTE}$"

RR_Route1_Text_Ivy_OffKey::
    .string "Ivy: You're off-key, sweetie.$"

RR_Route1_Text_P_OwnKey::
    .string "{PLAYER}: I'm in a key of my own.\n"
    .string "It's very exclusive.$"

RR_Route1_Text_Ivy_Married::
    .string "Ivy: Ha! That's why I married you.$"

RR_Route1_Text_Elm_Banter::
    .string "Elm: Jenny, why don't we banter\n"
    .string "like that?$"

RR_Route1_Text_Jenny_Singing::
    .string "Jenny: We do. You just don't hear\n"
    .string "me over the singing.$"

RR_Route1_Text_Elm_SawIt::
    .string "Elm: Did you see that?!\n"
    .string "Something purple! Flying!\p"
    .string "It looked right at me!$"

RR_Route1_Text_P_Believe::
    .string "{PLAYER}: I believe you, Elm.$"

RR_Route1_Text_Jenny_Believe::
    .string "Jenny: I believe you saw\n"
    .string "something, dear.$"

RR_Route1_Text_Elm_Tail::
    .string "Elm: It had a tail!$"

RR_Route1_Text_P_Seventy::
    .string "{PLAYER}: I saw a tail too.\n"
    .string "Mostly. I'm writing it down.$"

RR_Route1_Text_Ivy_Sixty::
    .string "Ivy: Whatever it was, it came\n"
    .string "from Viridian, sweetie. Look.$"

RR_Route1_Text_Elm_Race::
    .string "Elm: It came out of Viridian and\n"
    .string "turned west! Jenny! After it!$"

RR_Route1_Text_Jenny_Protocol::
    .string "Jenny: Dear, by protocol we stay\n"
    .string "together. Dear? …Dear!$"

RR_Route1_Text_DashedAhead::
    .string "Elm and Jenny dashed ahead\n"
    .string "toward Viridian City.$"

RR_Route1_Text_Ivy_Quiet::
    .string "Ivy: And they're off. Will they\n"
    .string "remember lunch, sweetie?$"

RR_Route1_Text_P_Wonderful::
    .string "{PLAYER}: Elm never forgets a meal.\n"
    .string "And Jenny packed it anyway.$"

@ ---------------------------------------------------------------
@ Catch reactions: one Elm/Jenny exchange per catch (up to four),
@ each followed by Elm cheering (and getting a little competitive).
@ Species match data/research_red/encounters/route1.txt and pallet_town.txt
@ (grass, surf and fishing, day and night): catches in Pallet's sea count too.
@ ---------------------------------------------------------------
RR_Route1_EventScript_CatchReactions::
	goto_if_eq VAR_RR_ROUTE1_CATCHES, 0, RR_Route1_EventScript_NoCatches
	setvar VAR_0x8004, 0
RR_Route1_EventScript_ReactLoop::
	callnative RR_ScriptRoute1Reaction
	bufferspeciesname STR_VAR_1, VAR_0x8005
	textcolor NPC_TEXT_COLOR_NEUTRAL
	switch VAR_RESULT
	case 0, RR_Route1_EventScript_ReactDefault
	case 1, RR_Route1_EventScript_ReactSpearow
	case 2, RR_Route1_EventScript_ReactRattata
	case 3, RR_Route1_EventScript_ReactMankey
	case 4, RR_Route1_EventScript_ReactNidoranM
	case 5, RR_Route1_EventScript_ReactPidgey
	case 6, RR_Route1_EventScript_ReactNidoranF
	case 7, RR_Route1_EventScript_ReactOddish
	case 8, RR_Route1_EventScript_ReactSentret
	case 9, RR_Route1_EventScript_ReactDuplicate
	case 10, RR_Route1_EventScript_ReactMeowth
	case 11, RR_Route1_EventScript_ReactEevee
	case 12, RR_Route1_EventScript_ReactHoppip
	case 13, RR_Route1_EventScript_ReactPikachu
	case 14, RR_Route1_EventScript_ReactSandshrew
	case 15, RR_Route1_EventScript_ReactDiglett
	case 16, RR_Route1_EventScript_ReactHoothoot
	case 17, RR_Route1_EventScript_ReactGastly
	case 18, RR_Route1_EventScript_ReactMurkrow
	case 19, RR_Route1_EventScript_ReactTentacool
	case 20, RR_Route1_EventScript_ReactMagikarp
	case 21, RR_Route1_EventScript_ReactHorsea
	case 22, RR_Route1_EventScript_ReactChinchou
	case 23, RR_Route1_EventScript_ReactQwilfish
	case 24, RR_Route1_EventScript_ReactKrabby
	case 25, RR_Route1_EventScript_ReactStaryu
	case 26, RR_Route1_EventScript_ReactShellder
	case 27, RR_Route1_EventScript_ReactRemoraid
	case 28, RR_Route1_EventScript_ReactDratini
RR_Route1_EventScript_ReactNext::
	addvar VAR_0x8004, 1
	textcolor NPC_TEXT_COLOR_NEUTRAL
	switch VAR_0x8004
	case 1, RR_Route1_EventScript_Jealous1
	case 2, RR_Route1_EventScript_Jealous2
	case 3, RR_Route1_EventScript_Jealous3
	case 4, RR_Route1_EventScript_Jealous4
RR_Route1_EventScript_ReactCheck::
	goto_if_ge VAR_0x8004, 4, RR_Route1_EventScript_ReactDone
	goto_if_lt VAR_0x8004, VAR_RR_ROUTE1_CATCHES, RR_Route1_EventScript_ReactLoop
RR_Route1_EventScript_ReactDone::
	closemessage
	return

RR_Route1_EventScript_NoCatches::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Elm_NoCatches, MSGBOX_DEFAULT
	return

RR_Route1_EventScript_ReactDefault::
	msgbox RR_Route1_Text_ReactDefaultA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactDefaultB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactSpearow::
	msgbox RR_Route1_Text_ReactSpearowA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactSpearowB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactRattata::
	msgbox RR_Route1_Text_ReactRattataA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactRattataB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactMankey::
	msgbox RR_Route1_Text_ReactMankeyA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactMankeyB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactNidoranM::
	msgbox RR_Route1_Text_ReactNidoranMA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactNidoranMB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactPidgey::
	msgbox RR_Route1_Text_ReactPidgeyA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactPidgeyB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactNidoranF::
	msgbox RR_Route1_Text_ReactNidoranFA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactNidoranFB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactOddish::
	msgbox RR_Route1_Text_ReactOddishA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactOddishB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactSentret::
	msgbox RR_Route1_Text_ReactSentretA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactSentretB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactDuplicate::
	msgbox RR_Route1_Text_ReactDuplicateA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactDuplicateB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactMeowth::
	msgbox RR_Route1_Text_ReactMeowthA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactMeowthB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactEevee::
	msgbox RR_Route1_Text_ReactEeveeA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactEeveeB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactHoppip::
	msgbox RR_Route1_Text_ReactHoppipA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactHoppipB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactPikachu::
	msgbox RR_Route1_Text_ReactPikachuA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactPikachuB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactSandshrew::
	msgbox RR_Route1_Text_ReactSandshrewA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactSandshrewB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactDiglett::
	msgbox RR_Route1_Text_ReactDiglettA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactDiglettB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactHoothoot::
	msgbox RR_Route1_Text_ReactHoothootA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactHoothootB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactGastly::
	msgbox RR_Route1_Text_ReactGastlyA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactGastlyB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactMurkrow::
	msgbox RR_Route1_Text_ReactMurkrowA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactMurkrowB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactTentacool::
	msgbox RR_Route1_Text_ReactTentacoolA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactTentacoolB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactMagikarp::
	msgbox RR_Route1_Text_ReactMagikarpA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactMagikarpB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactHorsea::
	msgbox RR_Route1_Text_ReactHorseaA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactHorseaB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactChinchou::
	msgbox RR_Route1_Text_ReactChinchouA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactChinchouB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactQwilfish::
	msgbox RR_Route1_Text_ReactQwilfishA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactQwilfishB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactKrabby::
	msgbox RR_Route1_Text_ReactKrabbyA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactKrabbyB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactStaryu::
	msgbox RR_Route1_Text_ReactStaryuA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactStaryuB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactShellder::
	msgbox RR_Route1_Text_ReactShellderA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactShellderB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactRemoraid::
	msgbox RR_Route1_Text_ReactRemoraidA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactRemoraidB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_ReactDratini::
	msgbox RR_Route1_Text_ReactDratiniA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactDratiniB, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactNext

RR_Route1_EventScript_Jealous1::
	msgbox RR_Route1_Text_Jealous1, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactCheck

RR_Route1_EventScript_Jealous2::
	msgbox RR_Route1_Text_Jealous2, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactCheck

RR_Route1_EventScript_Jealous3::
	msgbox RR_Route1_Text_Jealous3, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactCheck

RR_Route1_EventScript_Jealous4::
	msgbox RR_Route1_Text_Jealous4, MSGBOX_DEFAULT
	goto RR_Route1_EventScript_ReactCheck

RR_Route1_Text_Elm_NoCatches::
    .string "Elm: No catches yet? That's fine!\n"
    .string "The best research takes time!$"

RR_Route1_Text_ReactDefaultA::
    .string "Elm: Huh! I didn't know\n"
    .string "{STR_VAR_1} lived here.$"

RR_Route1_Text_ReactDefaultB::
    .string "Jenny: Noted for the field report.$"

RR_Route1_Text_ReactSpearowA::
    .string "Elm: A Spearow? Loud. Angry.\n"
    .string "Bold. I respect it.$"

RR_Route1_Text_ReactSpearowB::
    .string "Jenny: It has the temperament of a\n"
    .string "parking violation.$"

RR_Route1_Text_ReactRattataA::
    .string "Elm: A Rattata! Is it related\n"
    .string "to Muffin? Is it watching me?$"

RR_Route1_Text_ReactRattataB::
    .string "Ivy: In Alola they're black,\n"
    .string "with a little moustache.$"

RR_Route1_Text_ReactMankeyA::
    .string "Elm: Is it supposed to be that\n"
    .string "mad?$"

RR_Route1_Text_ReactMankeyB::
    .string "{PLAYER}: That's its resting face.\n"
    .string "We've all been there.$"

RR_Route1_Text_ReactNidoranMA::
    .string "Elm: A Nidoran! The male one.\n"
    .string "Big horn, even bigger ears!$"

RR_Route1_Text_ReactNidoranMB::
    .string "Jenny: Mind the horn, everyone.\n"
    .string "Protocol.$"

RR_Route1_Text_ReactPidgeyA::
    .string "Elm: Everyone's first Pidgey.\n"
    .string "Adorable.$"

RR_Route1_Text_ReactPidgeyB::
    .string "Jenny: You started with a Pidgey\n"
    .string "too, dear. It flew away.$"

RR_Route1_Text_ReactNidoranFA::
    .string "Elm: A Nidoran! The female one.\n"
    .string "Smaller horn, same spirit!$"

RR_Route1_Text_ReactNidoranFB::
    .string "{PLAYER}: Two species, one name.\n"
    .string "I have a forum thread on it.$"

RR_Route1_Text_ReactOddishA::
    .string "Elm: An Oddish! It's a little\n"
    .string "garden that goes for walks!$"

RR_Route1_Text_ReactOddishB::
    .string "Jenny: Only at night. Like you\n"
    .string "before a deadline, dear.$"

RR_Route1_Text_ReactSentretA::
    .string "Elm: A Sentret! From Johto! It\n"
    .string "followed us here, I bet!$"

RR_Route1_Text_ReactSentretB::
    .string "Jenny: It keeps watch on its\n"
    .string "tail. I respect a lookout.$"

RR_Route1_Text_ReactDuplicateA::
    .string "Elm: Another one? Are you\n"
    .string "building a choir?$"

RR_Route1_Text_ReactDuplicateB::
    .string "Ivy: Two samples beat one, Elm.\n"
    .string "Good science.$"

RR_Route1_Text_ReactMeowthA::
    .string "Elm: A Meowth! Hide your coins!$"

RR_Route1_Text_ReactMeowthB::
    .string "Ivy: In Galar they're iron-clad.\n"
    .string "They'd take your fork too.$"

RR_Route1_Text_ReactEeveeA::
    .string "Elm: An Eevee?! Eight evolutions!\n"
    .string "On Route One!$"

RR_Route1_Text_ReactEeveeB::
    .string "Jenny: Breathe, dear. In through\n"
    .string "the nose.$"

RR_Route1_Text_ReactHoppipA::
    .string "Elm: A Hoppip! Hold on tight, or\n"
    .string "the wind takes it to Johto!$"

RR_Route1_Text_ReactHoppipB::
    .string "Jenny: Noted. I'll file it under\n"
    .string "flight risk.$"

RR_Route1_Text_ReactPikachuA::
    .string "Elm: A Pikachu?! Like Ash's!\n"
    .string "Don't put it in its ball!$"

RR_Route1_Text_ReactPikachuB::
    .string "Jenny: Rubber gloves, dear.\n"
    .string "It sparks when it's happy.$"

RR_Route1_Text_ReactSandshrewA::
    .string "Elm: A Sandshrew! It's a long\n"
    .string "way from the desert!$"

RR_Route1_Text_ReactSandshrewB::
    .string "Ivy: Alola's live up snowy\n"
    .string "mountains. Ice, not sand.$"

RR_Route1_Text_ReactDiglettA::
    .string "Elm: A Diglett! What's under it?\n"
    .string "Nobody knows! Science!$"

RR_Route1_Text_ReactDiglettB::
    .string "Ivy: In Alola it has three\n"
    .string "golden hairs. Lovely ones.$"

RR_Route1_Text_ReactHoothootA::
    .string "Elm: A Hoothoot! It keeps time\n"
    .string "with its head!$"

RR_Route1_Text_ReactHoothootB::
    .string "Jenny: Finally. Someone punctual.$"

RR_Route1_Text_ReactGastlyA::
    .string "Elm: A g-g-ghost! {PLAYER},\n"
    .string "it went right through me!$"

RR_Route1_Text_ReactGastlyB::
    .string "Jenny: It's mostly gas, dear.\n"
    .string "Don't inhale.$"

RR_Route1_Text_ReactMurkrowA::
    .string "Elm: A Murkrow! Is that bad luck?\n"
    .string "Jenny, is that bad luck?$"

RR_Route1_Text_ReactMurkrowB::
    .string "Jenny: Known for petty theft.\n"
    .string "I'll keep an eye on it.$"

RR_Route1_Text_Jealous1::
    .string "Elm: A fine start! I'm not\n"
    .string "counting. Jenny's counting.$"

RR_Route1_Text_Jealous2::
    .string "Elm: Two already? Inspiring!\n"
    .string "Terrifying, but inspiring!$"

RR_Route1_Text_Jealous3::
    .string "Elm: Three! Jenny, we have to\n"
    .string "catch up! For science!$"

RR_Route1_Text_Jealous4::
    .string "Elm: Four?! A whole research\n"
    .string "team! I'm so proud of you!\l"
    .string "…And a little nervous.$"

RR_Route1_Text_Elm_WalkWithUs::
    .string "Elm: Viridian's this way! Walk\n"
    .string "with us! It's a beautiful day!$"

RR_Route1_Text_Elm_UpTheHill::
    .string "Elm: Up we go! Viridian's just\n"
    .string "over the hill!$"

RR_Route1_Text_ReactTentacoolA::
    .string "Elm: A Tentacool! Is it… related\n"
    .string "to Gerald? Should we tell him?$"

RR_Route1_Text_ReactTentacoolB::
    .string "Jenny: In my professional opinion,\n"
    .string "there is only one Gerald.$"

RR_Route1_Text_ReactMagikarpA::
    .string "Elm: A Magikarp! Don't laugh.\n"
    .string "They become Gyarados one day!$"

RR_Route1_Text_ReactMagikarpB::
    .string "Ivy: Patience and a lot of\n"
    .string "splashing. Like a PhD, sweetie.$"

RR_Route1_Text_ReactHorseaA::
    .string "Elm: A Horsea! It squirts ink\n"
    .string "when it's startled!$"

RR_Route1_Text_ReactHorseaB::
    .string "Jenny: Noted. Don't startle it,\n"
    .string "dear. Protocol.$"

RR_Route1_Text_ReactChinchouA::
    .string "Elm: A Chinchou! It lights up\n"
    .string "the whole sea at night!$"

RR_Route1_Text_ReactChinchouB::
    .string "Jenny: Good. A witness that\n"
    .string "brings its own flashlight.$"

RR_Route1_Text_ReactQwilfishA::
    .string "Elm: A Qwilfish! Don't hug it.\n"
    .string "Trust me. Don't hug it.$"

RR_Route1_Text_ReactQwilfishB::
    .string "Jenny: He hugged one. I filed\n"
    .string "the report. Twice.$"

RR_Route1_Text_ReactKrabbyA::
    .string "Elm: A Krabby! Strong pincers!\n"
    .string "Great for research! Ow!$"

RR_Route1_Text_ReactKrabbyB::
    .string "Ivy: Keep it away from the\n"
    .string "sandwiches, sweetie. Again.$"

RR_Route1_Text_ReactStaryuA::
    .string "Elm: A Staryu! Its core glows\n"
    .string "at night like a little star!$"

RR_Route1_Text_ReactStaryuB::
    .string "Jenny: It regrows lost arms.\n"
    .string "Very useful on stakeouts.$"

RR_Route1_Text_ReactShellderA::
    .string "Elm: A Shellder! It sticks its\n"
    .string "tongue out at everyone!$"

RR_Route1_Text_ReactShellderB::
    .string "{PLAYER}: Same, Shellder.\n"
    .string "Same.$"

RR_Route1_Text_ReactRemoraidA::
    .string "Elm: A Remoraid! It's a water\n"
    .string "pistol with fins!$"

RR_Route1_Text_ReactRemoraidB::
    .string "Jenny: Point it elsewhere,\n"
    .string "please. Protocol.$"

RR_Route1_Text_ReactDratiniA::
    .string "Elm: A D-Dratini?! People\n"
    .string "search their whole lives!$"

RR_Route1_Text_ReactDratiniB::
    .string "Ivy: It's perfect. Breathe, Elm.\n"
    .string "Don't cry on it.$"
