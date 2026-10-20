@ Research Red (#56): Heal or Rest, offered by Nurse Joy and by Delia at home.
@ Rest heals the team and moves the clock forward: day -> 8 PM (night),
@ night -> 7 AM (morning). C side: src/rr_rest.c.

@ The menu: VAR_RESULT 0 = Heal, 1 = Rest, anything else = cancel.
@ The Rest label says where the clock will go.
RR_EventScript_HealOrRestMenu::
	callnative RR_ScriptIsNight
	goto_if_eq VAR_RESULT, TRUE, RR_EventScript_HealOrRestMenu_Night
	dynmultichoice 18, 6, FALSE, 3, 0, DYN_MULTICHOICE_CB_NONE, RR_Rest_Text_Heal, RR_Rest_Text_RestTillNight, RR_Rest_Text_NoThanks
	return

RR_EventScript_HealOrRestMenu_Night::
	dynmultichoice 18, 6, FALSE, 3, 0, DYN_MULTICHOICE_CB_NONE, RR_Rest_Text_Heal, RR_Rest_Text_RestTillMorning, RR_Rest_Text_NoThanks
	return

@ After healing: fade out, move the clock, fade back in, say where it went
RR_EventScript_RestPassTime::
	fadescreen FADE_TO_BLACK
	callnative RR_ScriptRest
	copyvar VAR_0x8008, VAR_RESULT
	delay 40
	fadescreen FADE_FROM_BLACK
	textcolor NPC_TEXT_COLOR_NEUTRAL
	goto_if_eq VAR_0x8008, TRUE, RR_EventScript_RestPassTime_Night
	msgbox RR_Rest_Text_Morning, MSGBOX_DEFAULT
	return

RR_EventScript_RestPassTime_Night::
	msgbox RR_Rest_Text_Night, MSGBOX_DEFAULT
	return

@ Nurse Joy: Rest (EventScript_PkmnCenterNurse, data/scripts/pkmn_center_nurse.inc)
RR_EventScript_PkmnCenterNurse_Rest::
	incrementgamestat GAME_STAT_USED_POKECENTER
	message RR_Rest_Text_NurseRest
	waitmessage
	call EventScript_PkmnCenterNurse_TakeAndHealPkmn
	special SetUsedPkmnCenterQuestLogEvent
	call RR_EventScript_RestPassTime
	goto EventScript_PkmnCenterNurse_CheckTrainerTowerAndUnionRoom

@ Delia at home (PalletTown_PlayersHouse_1F_EventScript_MomHeal)
RR_EventScript_DeliaHealOrRest::
	message RR_Rest_Text_DeliaAsk
	waitmessage
	call RR_EventScript_HealOrRestMenu
	closemessage
	switch VAR_RESULT
	case 0, RR_EventScript_DeliaHeal
	case 1, RR_EventScript_DeliaRest
	msgbox RR_Rest_Text_DeliaBye, MSGBOX_DEFAULT
	release
	end

RR_EventScript_DeliaHeal::
	call EventScript_OutOfCenterPartyHeal
	msgbox PalletTown_PlayersHouse_1F_Text_LookingGreatTakeCare
	release
	end

RR_EventScript_DeliaRest::
	call EventScript_OutOfCenterPartyHeal
	call RR_EventScript_RestPassTime
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_LookingGreatTakeCare
	release
	end

RR_Rest_Text_Heal:
	.string "Heal$"

RR_Rest_Text_RestTillNight:
	.string "Rest till night$"

RR_Rest_Text_RestTillMorning:
	.string "Rest till morning$"

RR_Rest_Text_NoThanks:
	.string "No thanks$"

RR_Rest_Text_NurseRest:
	.string "Of course. Rest here as long as\n"
	.string "you like. I'll look after them.$"

RR_Rest_Text_Night:
	.string "{PLAYER} rested until night.$"

RR_Rest_Text_Morning:
	.string "{PLAYER} rested until morning.$"

RR_Rest_Text_DeliaAsk:
	.string "Delia: {PLAYER}! Do you want me to\n"
	.string "fix up your team, or have a rest?$"

RR_Rest_Text_DeliaBye:
	.string "Delia: All right. Take care now!$"
