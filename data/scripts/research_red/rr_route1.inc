@ Research Red: First walk up Route 1 (Route1)
@ Moved out of the FireRed map files; the map keeps only its map_script hooks.

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
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Humming, MSGBOX_DEFAULT
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
	closemessage
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
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_ROUTE1_ELM
	removeobject LOCALID_ROUTE1_JENNY
	fadescreen FADE_FROM_BLACK
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_DashedAhead, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Ivy_Quiet, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_P_Wonderful, MSGBOX_DEFAULT
	closemessage
	rr_ivy_follow LOCALID_ROUTE1_IVY
	setvar VAR_0x40FE, 5
	releaseall
	end

@ No follower (e.g. an old save): put Ivy behind the player
RR_Route1_EventScript_PlaceIvy::
	setobjectxyperm LOCALID_ROUTE1_IVY, VAR_0x8004, VAR_0x8006
	addobject LOCALID_ROUTE1_IVY
	return

@ Only 3 tiles: the scene triggers at y=4, and panning past the top of
@ Route 1 into the Viridian connection left the camera offset.
RR_Route1_Movement_CameraUp:
	walk_up
	walk_up
	walk_up
	step_end

RR_Route1_Movement_CameraDown:
	walk_down
	walk_down
	walk_down
	step_end

RR_Route1_Text_Elm_WeMeetAgain::
    .string "Elm: Route One! We meet again!\n"
    .string "This time I brought a partner!$"

RR_Route1_Text_Jenny_RightHere::
    .string "Jenny: And a wife.\n"
    .string "I'm right here, dear.$"

RR_Route1_Text_Humming::
    .string "Elm starts humming a traveling\n"
    .string "song. Ivy joins in.$"

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
    .string "{PLAYER}: (I believe him about\n"
    .string "seventy percent.)$"

RR_Route1_Text_Ivy_Sixty::
    .string "Ivy: (Sixty. But I love him\n"
    .string "for it.)$"

RR_Route1_Text_Elm_Race::
    .string "Elm: It flew toward Viridian!\n"
    .string "Jenny! After it!$"

RR_Route1_Text_Jenny_Protocol::
    .string "Jenny: Dear, by protocol we stay\n"
    .string "together. Dear? …Dear!$"

RR_Route1_Text_DashedAhead::
    .string "Elm and Jenny dashed ahead\n"
    .string "toward Viridian City.$"

RR_Route1_Text_Ivy_Quiet::
    .string "Ivy: And they're off. Lunch is\n"
    .string "going to be very quiet, sweetie.$"

RR_Route1_Text_P_Wonderful::
    .string "{PLAYER}: Lunch is going to be\n"
    .string "wonderful.$"

@ ---------------------------------------------------------------
@ Catch reactions: one Elm/Jenny exchange per catch (up to four),
@ each followed by Elm getting more jealous. Placeholder lines.
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
	case 8, RR_Route1_EventScript_ReactBellsprout
	case 9, RR_Route1_EventScript_ReactDuplicate
	case 10, RR_Route1_EventScript_ReactMeowth
	case 11, RR_Route1_EventScript_ReactEevee
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

RR_Route1_EventScript_ReactBellsprout::
	msgbox RR_Route1_Text_ReactBellsproutA, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_ReactBellsproutB, MSGBOX_DEFAULT
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
    .string "Elm: Nothing caught yet? Ha!\n"
    .string "There's still time!$"

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
    .string "Elm: Rattata! Classic. Reliable.\n"
    .string "Like you, apparently.$"

RR_Route1_Text_ReactRattataB::
    .string "Jenny: Daisy will want to compare\n"
    .string "tails.$"

RR_Route1_Text_ReactMankeyA::
    .string "Elm: Is it supposed to be that\n"
    .string "mad?$"

RR_Route1_Text_ReactMankeyB::
    .string "Jenny: In my professional opinion,\n"
    .string "it needs anger management.$"

RR_Route1_Text_ReactNidoranMA::
    .string "Elm: A Nidoran! Poisonous AND\n"
    .string "adorable!$"

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
    .string "Elm: The other Nidoran! Is that\n"
    .string "even allowed?$"

RR_Route1_Text_ReactNidoranFB::
    .string "Jenny: Two kinds, one name. Filed\n"
    .string "under complicated.$"

RR_Route1_Text_ReactOddishA::
    .string "Elm: It's a weed. With feet.$"

RR_Route1_Text_ReactOddishB::
    .string "Jenny: Dear, it can hear you.$"

RR_Route1_Text_ReactBellsproutA::
    .string "Elm: Is that a plant or a noodle?$"

RR_Route1_Text_ReactBellsproutB::
    .string "Jenny: In my professional opinion:\n"
    .string "both.$"

RR_Route1_Text_ReactDuplicateA::
    .string "Elm: Another one? Are you\n"
    .string "building a choir?$"

RR_Route1_Text_ReactDuplicateB::
    .string "Ivy: Two samples beat one, Elm.\n"
    .string "Good science.$"

RR_Route1_Text_ReactMeowthA::
    .string "Elm: A Meowth! Hide your coins!$"

RR_Route1_Text_ReactMeowthB::
    .string "Jenny: It's already eyeing your\n"
    .string "wallet, dear.$"

RR_Route1_Text_ReactEeveeA::
    .string "Elm: An Eevee?! On Route One?!$"

RR_Route1_Text_ReactEeveeB::
    .string "Jenny: Breathe, dear. In through\n"
    .string "the nose.$"

RR_Route1_Text_Jealous1::
    .string "Elm: Beginner's luck.$"

RR_Route1_Text_Jealous2::
    .string "Elm: …Two? Two already?$"

RR_Route1_Text_Jealous3::
    .string "Elm: Three. Okay. Three is fine.\n"
    .string "I am fine.$"

RR_Route1_Text_Jealous4::
    .string "Elm: Four?! Jenny, they have\n"
    .string "four!$"

