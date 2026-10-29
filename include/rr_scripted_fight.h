#ifndef GUARD_RR_SCRIPTED_FIGHT_H
#define GUARD_RR_SCRIPTED_FIGHT_H

// Research Red: scripted fights you watch (src/rr_scripted_fight.c, #105)
struct ScriptContext;

bool32 RR_IsScriptedFight(void);
void RR_ScriptedFightAdjustParty(struct Pokemon *party, u16 trainerId);
void RR_ScriptedFightChooseMove(enum BattlerId battler);
const u8 *RR_ScriptedFightLineScript(void);
const u8 *RR_ScriptedFightIntroText(void);
void RR_ScriptedFightHideTrainerPic(enum BattlerId battler);

#endif // GUARD_RR_SCRIPTED_FIGHT_H
