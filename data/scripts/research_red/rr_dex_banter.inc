@ Research Red: Pokédex banter (src/rr_dex_banter.c). Started by the game
@ after a new species is registered; one message per line so the portrait
@ follows the speaker. Lines: data/research_red/dex_banter.txt
RR_EventScript_DexBanter::
	lockall
	textcolor NPC_TEXT_COLOR_NEUTRAL
RR_DexBanter_EventScript_Next::
	callnative RR_ScriptDexBanterNextLine
	goto_if_eq VAR_RESULT, FALSE, RR_DexBanter_EventScript_End
	msgbox RR_DexBanter_Text_Line, MSGBOX_DEFAULT
	goto RR_DexBanter_EventScript_Next

RR_DexBanter_EventScript_End::
	closemessage
	releaseall
	end

RR_DexBanter_Text_Line::
	.string "{STR_VAR_1}$"
