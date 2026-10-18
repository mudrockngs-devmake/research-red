#ifndef GUARD_RR_DEX_BANTER_H
#define GUARD_RR_DEX_BANTER_H

// Research Red: Pokédex banter after a new species (src/rr_dex_banter.c)
struct ScriptContext;
void RR_DexBanterQueue(u16 species);
bool32 RR_TryStartDexBanter(void);
void RR_ScriptDexBanterNextLine(struct ScriptContext *ctx);

#endif // GUARD_RR_DEX_BANTER_H
