#ifndef GUARD_RR_PORTRAIT_H
#define GUARD_RR_PORTRAIT_H

// Research Red: speaker portraits above the field text box (src/rr_portrait.c)
void RR_PortraitOnMessage(const u8 *text);
void RR_PortraitHide(void);
void RR_PortraitBoxClosed(void);
struct ScriptContext;
void RR_ScriptPortraitHide(struct ScriptContext *ctx);
enum { RR_LINE_NARRATION, RR_LINE_PLAYER, RR_LINE_IVY, RR_LINE_OTHER };
u32 RR_GetLineSpeaker(const u8 *text);
u8 RR_CreateStandalonePortrait(u32 slot, u32 who, s16 x, s16 y, bool32 hFlip);
void RR_SetStandalonePortraitDim(u32 slot, bool32 dim);
void RR_DestroyStandalonePortrait(u32 slot, u8 spriteId);
void RR_UpdateBattleSpeakerPortrait(const u8 *text); // #95: each frame in battle
struct SpritePalette;
u8 RR_LoadInterfaceSpritePalette(const struct SpritePalette *palette); // slots 12-15, safe from overworld palettes

#endif // GUARD_RR_PORTRAIT_H
