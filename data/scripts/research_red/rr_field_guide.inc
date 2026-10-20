@ Research Red: Pokédex > START > FIELD GUIDE (src/rr_field_guide.c)
RR_EventScript_FieldGuide::
	lockall
	callnative RR_ScriptFieldGuide
	waitstate
	releaseall
	end

@ Research Red: Options > Research Rules (src/rr_rules.c, src/option_menu.c)
@ Start > PC (#34): the usual PC menu (Pokemon storage, your item storage,
@ Prof. Oak's PC) from anywhere. Turning it off only animates a real PC.
RR_EventScript_StartMenuPC::
	lockall
	playse SE_PC_ON
	goto EventScript_PCMainMenu
	end

RR_EventScript_ResearchRules::
	lockall
	callnative RR_ScriptResearchRules
	waitstate
	@ #45: a changed rule plays a short call with Elm and Jenny (#13)
	goto_if_eq VAR_RESULT, FALSE, RR_EventScript_ResearchRules_End
	fadescreen FADE_TO_BLACK
	setvar VAR_0x8004, RR_CALL_RULES
	callnative RR_ScriptStartCall
	waitstate
RR_EventScript_ResearchRules_End::
	releaseall
	end

@ Research Red: shown once when continuing a save from an older layout
@ (src/rr_save_version.c, docs/SAVES.md)
RR_EventScript_OldSaveWarning::
	lockall
	msgbox RR_Text_OldSaveWarning, MSGBOX_DEFAULT
	releaseall
	end

@ Research Red (#57): a save from before TM51-100 existed gets them once
@ (src/rr_save_version.c; the grant script is in rr_lab.inc)
RR_EventScript_TopUpTMs::
	lockall
	call RR_EventScript_GiveTM51To100
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Text_TopUpTMs, MSGBOX_DEFAULT
	releaseall
	end

RR_Text_TopUpTMs::
	.string "Oak: {PLAYER}! The League sent more\n"
	.string "TMs for the grant. Fifty of them!\p"
	.string "They're in your TM Case. Ho ho!$"

RR_Text_OldSaveWarning::
	.string "This save is from an older test\n"
	.string "build of Research Red.\p"
	.string "It may still work, but if anything\n"
	.string "looks wrong, start a new game.$"
