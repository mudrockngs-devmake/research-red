#ifndef GUARD_RR_DEX_BANTER_H
#define GUARD_RR_DEX_BANTER_H

// Research Red: Pokédex banter (src/rr_dex_banter.c)
struct ScriptContext;
void RR_DexBanterQueue(u16 species);
bool32 RR_TryStartDexBanter(void);
void RR_ScriptDexBanterNextLine(struct ScriptContext *ctx);
bool32 RR_DexBanterAvailable(void);
void RR_DexBanterBegin(u16 species);
const u8 *RR_DexBanterNext(void);
void RR_DexBanterForget(u16 species);

#endif // GUARD_RR_DEX_BANTER_H
