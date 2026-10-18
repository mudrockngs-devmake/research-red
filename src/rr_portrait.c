// Research Red: character portraits above the field text box.
//
// When a field message starts printing, its first word is checked for a
// speaker prefix ("Elm:", "Ivy:", "Jenny:", "Oak:", "Delia:", "Daisy:" or the
// player's name followed by ':'). If one matches, that character's 64x64
// portrait (graphics/research_red/cast/<name>/portrait.png) is shown just above
// the box; any other message hides it. The portrait also removes itself as soon
// as the box closes, so scenes need no extra script commands.
//
// Outfits: FLAG_RR_IVY_SWIMSUIT and FLAG_RR_JENNY_UNIFORM switch to the
// alternate portraits (Jenny is in casual clothes by default).
#include "global.h"
#include "event_data.h"
#include "field_message_box.h"
#include "rr_portrait.h"
#include "sprite.h"
#include "string_util.h"
#include "constants/flags.h"

#define TAG_RR_PORTRAIT   0x5A10
#define PORTRAIT_X        40   // centre; the box's left edge is at x 8
#define PORTRAIT_Y        78   // centre; bottom edge sits on the box's top border

static const u32 sPortraitGfx_elm[] = INCBIN_U32("graphics/research_red/cast/elm/portrait.4bpp");
static const u16 sPortraitPal_elm[] = INCBIN_U16("graphics/research_red/cast/elm/portrait.gbapal");
static const u32 sPortraitGfx_ivy[] = INCBIN_U32("graphics/research_red/cast/ivy/portrait.4bpp");
static const u16 sPortraitPal_ivy[] = INCBIN_U16("graphics/research_red/cast/ivy/portrait.gbapal");
static const u32 sPortraitGfx_ivy_swimsuit[] = INCBIN_U32("graphics/research_red/cast/ivy_swimsuit/portrait.4bpp");
static const u16 sPortraitPal_ivy_swimsuit[] = INCBIN_U16("graphics/research_red/cast/ivy_swimsuit/portrait.gbapal");
static const u32 sPortraitGfx_jenny[] = INCBIN_U32("graphics/research_red/cast/jenny/portrait.4bpp");
static const u16 sPortraitPal_jenny[] = INCBIN_U16("graphics/research_red/cast/jenny/portrait.gbapal");
static const u32 sPortraitGfx_jenny_police[] = INCBIN_U32("graphics/research_red/cast/jenny_police/portrait.4bpp");
static const u16 sPortraitPal_jenny_police[] = INCBIN_U16("graphics/research_red/cast/jenny_police/portrait.gbapal");
static const u32 sPortraitGfx_oak[] = INCBIN_U32("graphics/research_red/cast/oak/portrait.4bpp");
static const u16 sPortraitPal_oak[] = INCBIN_U16("graphics/research_red/cast/oak/portrait.gbapal");
static const u32 sPortraitGfx_delia[] = INCBIN_U32("graphics/research_red/cast/delia/portrait.4bpp");
static const u16 sPortraitPal_delia[] = INCBIN_U16("graphics/research_red/cast/delia/portrait.gbapal");
static const u32 sPortraitGfx_daisy[] = INCBIN_U32("graphics/research_red/cast/daisy/portrait.4bpp");
static const u16 sPortraitPal_daisy[] = INCBIN_U16("graphics/research_red/cast/daisy/portrait.gbapal");
static const u32 sPortraitGfx_player_m[] = INCBIN_U32("graphics/research_red/cast/player_m/portrait.4bpp");
static const u16 sPortraitPal_player_m[] = INCBIN_U16("graphics/research_red/cast/player_m/portrait.gbapal");
static const u32 sPortraitGfx_player_f[] = INCBIN_U32("graphics/research_red/cast/player_f/portrait.4bpp");
static const u16 sPortraitPal_player_f[] = INCBIN_U16("graphics/research_red/cast/player_f/portrait.gbapal");

enum
{
    RR_PORTRAIT_NONE,
    RR_PORTRAIT_ELM,
    RR_PORTRAIT_IVY,
    RR_PORTRAIT_IVY_SWIMSUIT,
    RR_PORTRAIT_JENNY,
    RR_PORTRAIT_JENNY_POLICE,
    RR_PORTRAIT_OAK,
    RR_PORTRAIT_DELIA,
    RR_PORTRAIT_DAISY,
    RR_PORTRAIT_PLAYER_M,
    RR_PORTRAIT_PLAYER_F,
};

struct RRPortrait
{
    const u32 *gfx;
    const u16 *pal;
};

static const struct RRPortrait sPortraits[] =
{
    [RR_PORTRAIT_ELM]          = { sPortraitGfx_elm,          sPortraitPal_elm },
    [RR_PORTRAIT_IVY]          = { sPortraitGfx_ivy,          sPortraitPal_ivy },
    [RR_PORTRAIT_IVY_SWIMSUIT] = { sPortraitGfx_ivy_swimsuit, sPortraitPal_ivy_swimsuit },
    [RR_PORTRAIT_JENNY]        = { sPortraitGfx_jenny,        sPortraitPal_jenny },
    [RR_PORTRAIT_JENNY_POLICE] = { sPortraitGfx_jenny_police, sPortraitPal_jenny_police },
    [RR_PORTRAIT_OAK]          = { sPortraitGfx_oak,          sPortraitPal_oak },
    [RR_PORTRAIT_DELIA]        = { sPortraitGfx_delia,        sPortraitPal_delia },
    [RR_PORTRAIT_DAISY]        = { sPortraitGfx_daisy,        sPortraitPal_daisy },
    [RR_PORTRAIT_PLAYER_M]     = { sPortraitGfx_player_m,     sPortraitPal_player_m },
    [RR_PORTRAIT_PLAYER_F]     = { sPortraitGfx_player_f,     sPortraitPal_player_f },
};

static const u8 sPrefix_Elm[]   = _("Elm:");
static const u8 sPrefix_Ivy[]   = _("Ivy:");
static const u8 sPrefix_Jenny[] = _("Jenny:");
static const u8 sPrefix_Oak[]   = _("Oak:");
static const u8 sPrefix_Delia[] = _("Delia:");
static const u8 sPrefix_Daisy[] = _("Daisy:");

static const struct { const u8 *prefix; u8 portrait; } sSpeakers[] =
{
    { sPrefix_Elm,   RR_PORTRAIT_ELM },
    { sPrefix_Ivy,   RR_PORTRAIT_IVY },
    { sPrefix_Jenny, RR_PORTRAIT_JENNY },
    { sPrefix_Oak,   RR_PORTRAIT_OAK },
    { sPrefix_Delia, RR_PORTRAIT_DELIA },
    { sPrefix_Daisy, RR_PORTRAIT_DAISY },
};

static const struct OamData sOam_Portrait =
{
    .shape = SPRITE_SHAPE(64x64),
    .size = SPRITE_SIZE(64x64),
    .priority = 0,
};

static void SpriteCB_Portrait(struct Sprite *sprite);

static const struct SpriteTemplate sSpriteTemplate_Portrait =
{
    .tileTag = TAG_RR_PORTRAIT,
    .paletteTag = TAG_RR_PORTRAIT,
    .oam = &sOam_Portrait,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_Portrait,
};

// Sprite id + 1 (EWRAM must start zeroed, so 0 means "no portrait")
static EWRAM_DATA u8 sSpriteIdPlus1 = 0;
static EWRAM_DATA u8 sShown = RR_PORTRAIT_NONE;
#define sSpriteId (sSpriteIdPlus1 - 1)

static bool32 IsOurSprite(void)
{
    return sSpriteIdPlus1 != 0
        && gSprites[sSpriteId].inUse
        && gSprites[sSpriteId].callback == SpriteCB_Portrait;
}

void RR_PortraitHide(void)
{
    if (IsOurSprite())
    {
        DestroySprite(&gSprites[sSpriteId]);
        FreeSpriteTilesByTag(TAG_RR_PORTRAIT);
        FreeSpritePaletteByTag(TAG_RR_PORTRAIT);
    }
    sSpriteIdPlus1 = 0;
    sShown = RR_PORTRAIT_NONE;
}

// Leave with the text box, whichever way it was closed.
static void SpriteCB_Portrait(struct Sprite *sprite)
{
    if (IsFieldMessageBoxHidden())
        RR_PortraitHide();
}

static u32 GetSpeakerPortrait(const u8 *text)
{
    u32 i, len;

    for (i = 0; i < ARRAY_COUNT(sSpeakers); i++)
    {
        if (StringCompareN(text, sSpeakers[i].prefix, StringLength(sSpeakers[i].prefix)) == 0)
        {
            if (sSpeakers[i].portrait == RR_PORTRAIT_IVY && FlagGet(FLAG_RR_IVY_SWIMSUIT))
                return RR_PORTRAIT_IVY_SWIMSUIT;
            if (sSpeakers[i].portrait == RR_PORTRAIT_JENNY && FlagGet(FLAG_RR_JENNY_UNIFORM))
                return RR_PORTRAIT_JENNY_POLICE;
            return sSpeakers[i].portrait;
        }
    }

    // The player: their name followed by ':'
    len = StringLength(gSaveBlock2Ptr->playerName);
    if (len != 0 && StringCompareN(text, gSaveBlock2Ptr->playerName, len) == 0 && text[len] == 0xF0) // 0xF0 = ':' in charmap.txt
        return gSaveBlock2Ptr->playerGender == MALE ? RR_PORTRAIT_PLAYER_M : RR_PORTRAIT_PLAYER_F;

    return RR_PORTRAIT_NONE;
}

// Called with the expanded message text (gStringVar4) as a field message starts.
void RR_PortraitOnMessage(const u8 *text)
{
    u32 portrait = GetSpeakerPortrait(text);
    struct SpriteSheet sheet;
    struct SpritePalette palette;

    if (portrait == sShown && (portrait == RR_PORTRAIT_NONE || IsOurSprite()))
        return;

    RR_PortraitHide();
    if (portrait == RR_PORTRAIT_NONE)
        return;

    sheet.data = sPortraits[portrait].gfx;
    sheet.size = 64 * 64 / 2;
    sheet.tag = TAG_RR_PORTRAIT;
    palette.data = sPortraits[portrait].pal;
    palette.tag = TAG_RR_PORTRAIT;
    // A busy scene may have no sprite VRAM or palette slot left: skip the portrait
    LoadSpriteSheet(&sheet);
    if (GetSpriteTileStartByTag(TAG_RR_PORTRAIT) == TAG_NONE)
        return;
    if (LoadSpritePalette(&palette) == 0xFF)
    {
        FreeSpriteTilesByTag(TAG_RR_PORTRAIT);
        return;
    }
    sSpriteIdPlus1 = CreateSprite(&sSpriteTemplate_Portrait, PORTRAIT_X, PORTRAIT_Y, 0) + 1;
    if (sSpriteId == MAX_SPRITES)
    {
        sSpriteIdPlus1 = 0;
        FreeSpriteTilesByTag(TAG_RR_PORTRAIT);
        FreeSpritePaletteByTag(TAG_RR_PORTRAIT);
        return;
    }
    sShown = portrait;
}
