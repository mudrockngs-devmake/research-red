EventScript_AfterWhiteOutHeal::
	lockall
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox Text_FirstShouldRestoreMonsHealth
	call EventScript_PkmnCenterNurse_TakeAndHealPkmn
	call_if_unset FLAG_DEFEATED_BROCK, EventScript_AfterWhiteOutHealMsgPreBrock
	call_if_set FLAG_DEFEATED_BROCK, EventScript_AfterWhiteOutHealMsg
	applymovement VAR_LAST_TALKED, Movement_Bow
	waitmovement 0
	fadedefaultbgm
	releaseall
	end

EventScript_AfterWhiteOutHealMsgPreBrock::
	msgbox Text_MonsHealedShouldBuyPotions
	return

EventScript_AfterWhiteOutHealMsg::
	msgbox Text_MonsHealed
	return

@ Research Red: Delia isn't the player's mom; her own lines, with portrait (#63)
EventScript_AfterWhiteOutMomHeal::
	lockall
	textcolor NPC_TEXT_COLOR_NEUTRAL
	applymovement LOCALID_MOM, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox RR_WhiteOut_Text_Delia_RoughDay
	call EventScript_OutOfCenterPartyHeal
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_WhiteOut_Text_Delia_AllBetter
	fadedefaultbgm
	releaseall
	end

RR_WhiteOut_Text_Delia_RoughDay::
    .string "Delia: Oh my, look at you two!\n"
    .string "Rough day out there?\p"
    .string "Sit, sit. I'll take care of your\n"
    .string "Pokémon. And there's pie.$"

RR_WhiteOut_Text_Delia_AllBetter::
    .string "Delia: There, all better!\n"
    .string "Ash came home like this a lot.\p"
    .string "Take a snack for the road.\n"
    .string "And be careful, you two!$"

EventScript_FieldPoison::
	lockall
	textcolor NPC_TEXT_COLOR_NEUTRAL
	special TryFieldPoisonWhiteOut
	waitstate
	goto_if_eq VAR_RESULT, FLDPSN_WHITEOUT, EventScript_FieldWhiteOut
	goto_if_eq VAR_RESULT, FLDPSN_FRONTIER_WHITEOUT, EventScript_FrontierFieldWhiteOut
	releaseall
	end

EventScript_FieldWhiteOut::
	checkmoney 1
	goto_if_eq VAR_RESULT, FALSE, EventScript_FieldWhiteOutNoMoney
	goto_if_eq VAR_RESULT, TRUE, EventScript_FieldWhiteOutHasMoney
	end

EventScript_FieldWhiteOutNoMoney::
	msgbox Text_WhitedOut
	goto EventScript_FieldWhiteOutFade
	end

EventScript_FieldWhiteOutHasMoney::
	special OverworldWhiteOutGetMoneyLoss
	msgbox Text_WhitedOutLostMoney
	goto EventScript_FieldWhiteOutFade
	end

EventScript_FieldWhiteOutFade::
	special Script_FadeOutMapMusic
	waitstate
	fadescreen FADE_TO_BLACK
	special SetCB2WhiteOut
	waitstate
	end

EventScript_FrontierFieldWhiteOut::
	message Text_WhitedOut
	waitmessage
	waitbuttonpress
	pike_inchallenge
	goto_if_eq VAR_RESULT, TRUE, BattleFrontier_BattlePike_EventScript_Retire
	pyramid_getlocation
	goto_if_eq VAR_RESULT, PYRAMID_LOCATION_FLOOR, BattleFrontier_BattlePyramid_EventScript_WarpToLobbyLost
	goto_if_eq VAR_RESULT, PYRAMID_LOCATION_TOP, BattleFrontier_BattlePyramid_EventScript_WarpToLobbyLost
	special Script_FadeOutMapMusic
	waitstate
	fadescreen FADE_TO_BLACK
	special SetCB2WhiteOut
	waitstate
	end
