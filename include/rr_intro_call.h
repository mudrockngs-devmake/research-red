#ifndef GUARD_RR_INTRO_CALL_H
#define GUARD_RR_INTRO_CALL_H

// Research Red: the new-game intro video call (src/rr_intro_call.c)
struct ScriptContext;

void RR_StartIntroCall(void);
void RR_ScriptStartCall(struct ScriptContext *ctx); // #13: later calls from scripts

#endif // GUARD_RR_INTRO_CALL_H
