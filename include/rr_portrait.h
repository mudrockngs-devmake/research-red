#ifndef GUARD_RR_PORTRAIT_H
#define GUARD_RR_PORTRAIT_H

// Research Red: speaker portraits above the field text box (src/rr_portrait.c)
void RR_PortraitOnMessage(const u8 *text);
void RR_PortraitHide(void);
void RR_PortraitBoxClosed(void);
struct ScriptContext;
void RR_ScriptPortraitHide(struct ScriptContext *ctx);
struct SpritePalette;
u8 RR_LoadInterfaceSpritePalette(const struct SpritePalette *palette); // slots 12-15, safe from overworld palettes

#endif // GUARD_RR_PORTRAIT_H
