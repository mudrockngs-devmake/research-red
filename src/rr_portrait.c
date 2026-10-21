// Research Red: character portraits above the field text box.
//
// When a field message starts printing, its first word is checked for a
// speaker prefix ("Elm:", "Ivy:", "Mimey:", ... or the player's name followed
// by ':'). If one matches, that speaker's 64x64 portrait is shown above the
// box, with their name on a small plate between the portrait and the box.
// The portrait bobs gently (like the animatic) and stays up for the rest of the
// scene: it only changes when someone else speaks, and leaves on a narration
// line, when the text box is cleared away (closemessage, release), when the
// scene's script ends, when a battle starts, or for a choice menu that needs
// the space (RR_ScriptPortraitHide, e.g. the starter pick).
//
// Speakers, portraits, name colors and outfit flags come from
// graphics/research_red/portraits.txt (tools/research_red/build_portraits.py
// turns it into src/data/research_red/portraits.h).
#include "global.h"
#include "event_data.h"
#include "field_message_box.h"
#include "malloc.h"
#include "menu.h"
#include "rr_portrait.h"
#include "script.h"
#include "sprite.h"
#include "string_util.h"
#include "text.h"
#include "trig.h"
#include "window.h"
#include "palette.h"
#include "util.h"
#include "event_object_movement.h"
#include "constants/flags.h"

#define TAG_RR_PORTRAIT   0x5A10
#define TAG_RR_NAMEPLATE  0x5A11
#define PORTRAIT_X        40   // centre; left edge lines up with the box at x 8
#define PORTRAIT_Y        70   // centre; bottom 2 rows tuck behind the name plate, so the
                               // 2-pixel bob never opens a gap above it (#22)
#define PLATE_X           40   // 64x32 sprite, the plate is its top 16 rows
#define PLATE_Y           116  // so the plate fills y 100-115, sitting on the box's top edge (#22)
#define BOB_PERIOD        108  // frames per bob (1.8 s, as in the animatic)
#define PORTRAIT_GRACE    10   // frames the box may stay closed mid-script before the portrait leaves

enum
{
    RR_NAME_ORANGE,
    RR_NAME_GREEN,
    RR_NAME_BLUE,
    RR_NAME_BROWN,
    RR_NAME_PINK,
    RR_NAME_PURPLE,
    RR_NAME_TEAL,
    RR_NAME_RED,
    RR_NAME_GRAY,
};

// Name colors (the animatic's speaker colors)
static const u16 sNameColors[] =
{
    [RR_NAME_ORANGE] = RGB(24, 13, 2),
    [RR_NAME_GREEN]  = RGB(5, 15, 9),
    [RR_NAME_BLUE]   = RGB(5, 10, 21),
    [RR_NAME_BROWN]  = RGB(15, 9, 5),
    [RR_NAME_PINK]   = RGB(24, 8, 13),
    [RR_NAME_PURPLE] = RGB(15, 9, 21),
    [RR_NAME_TEAL]   = RGB(3, 17, 16),
    [RR_NAME_RED]    = RGB(24, 7, 5),
    [RR_NAME_GRAY]   = RGB(11, 11, 13),
};

struct RRSpeaker
{
    const u8 *name;
    const u32 *gfx;
    const u16 *pal;
    u8 color;
    const u32 *altGfx; // outfit shown while altFlag is set
    const u16 *altPal;
    u16 altFlag;
};

#include "data/research_red/portraits.h"

#define SPEAKER_NONE    0xFF
#define SPEAKER_PLAYER  0xFE

static const struct OamData sOam_Portrait =
{
    .shape = SPRITE_SHAPE(64x64),
    .size = SPRITE_SIZE(64x64),
    .priority = 0,
};

static const struct OamData sOam_NamePlate =
{
    .shape = SPRITE_SHAPE(64x32),
    .size = SPRITE_SIZE(64x32),
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

static const struct SpriteTemplate sSpriteTemplate_NamePlate =
{
    .tileTag = TAG_RR_NAMEPLATE,
    .paletteTag = TAG_RR_NAMEPLATE,
    .oam = &sOam_NamePlate,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

// Sprite ids + 1 (EWRAM must start zeroed, so 0 means "none")
static EWRAM_DATA u8 sPortraitIdPlus1 = 0;
static EWRAM_DATA u8 sPlateIdPlus1 = 0;
static EWRAM_DATA u8 sShownSpeaker = 0;   // speaker index + 1
static EWRAM_DATA bool8 sShownAlt = FALSE;
// The text box is on screen: set when a message starts, cleared when the
// dialogue window is wiped. (The engine's own "box hidden" flag turns on as
// soon as the text has finished printing, while the box still waits for a
// button: trusting it made the portrait vanish on every last page, #76.)
static EWRAM_DATA bool8 sBoxOpen = FALSE;

void RR_PortraitBoxClosed(void)
{
    sBoxOpen = FALSE;
}

// Load a palette into slots 12-15. Slots 0-11 belong to overworld people and
// are rewritten by the overworld code (on maps with reflections, such as the
// lab, FireRed only reserves 0-7, so a normal load could land in 8-11).
u8 RR_LoadInterfaceSpritePalette(const struct SpritePalette *palette)
{
    u8 oldReserved = gReservedSpritePaletteCount;
    u8 slot;

    if (gReservedSpritePaletteCount < OBJ_PALSLOT_COUNT)
        gReservedSpritePaletteCount = OBJ_PALSLOT_COUNT;
    slot = LoadSpritePalette(palette);
    gReservedSpritePaletteCount = oldReserved;
    // Also write the colors to the screen now: the buffered copy can reach it a
    // couple of frames late, flashing the new picture in the old colors
    if (slot != 0xFF)
        CpuCopy16(palette->data, (u16 *)OBJ_PLTT + slot * 16, PLTT_SIZE_4BPP);
    return slot;
}

static bool32 IsSpriteOurs(u8 idPlus1, u16 tileTag)
{
    struct Sprite *sprite;

    if (idPlus1 == 0)
        return FALSE;
    sprite = &gSprites[idPlus1 - 1];
    return sprite->inUse && sprite->template != NULL && sprite->template->tileTag == tileTag;
}

void RR_PortraitHide(void)
{
    if (IsSpriteOurs(sPortraitIdPlus1, TAG_RR_PORTRAIT))
        DestroySprite(&gSprites[sPortraitIdPlus1 - 1]);
    if (IsSpriteOurs(sPlateIdPlus1, TAG_RR_NAMEPLATE))
        DestroySprite(&gSprites[sPlateIdPlus1 - 1]);
    FreeSpriteTilesByTag(TAG_RR_PORTRAIT);
    FreeSpritePaletteByTag(TAG_RR_PORTRAIT);
    FreeSpriteTilesByTag(TAG_RR_NAMEPLATE);
    FreeSpritePaletteByTag(TAG_RR_NAMEPLATE);
    sPortraitIdPlus1 = 0;
    sPlateIdPlus1 = 0;
    sShownSpeaker = 0;
}

// Bob gently; leave once the box is cleared away. Outside a script that's at
// once; inside one (camera pans, people walking off, #3) after a short grace,
// so a closemessage straight into the next line doesn't make it flicker.
static void SpriteCB_Portrait(struct Sprite *sprite)
{
    if (!sBoxOpen)
    {
        if (!RR_IsScriptActive() || ++sprite->data[2] > PORTRAIT_GRACE)
        {
            RR_PortraitHide();
            return;
        }
    }
    else
    {
        sprite->data[2] = 0;
    }
    // A new portrait stays hidden for its first two frames: the overworld
    // copies new sprite colors to the screen a frame late, so showing it at
    // once flashes it in the previous speaker's colors
    if (sprite->data[1] < 2)
    {
        if (++sprite->data[1] == 2)
        {
            sprite->invisible = FALSE;
            if (IsSpriteOurs(sPlateIdPlus1, TAG_RR_NAMEPLATE))
                gSprites[sPlateIdPlus1 - 1].invisible = FALSE;
        }
    }
    sprite->data[0] = (sprite->data[0] + 1) % BOB_PERIOD;
    sprite->y2 = (Cos(sprite->data[0] * 256 / BOB_PERIOD, 2) - 2) / 2; // 0 .. -2
}

static u32 GetSpeaker(const u8 *text)
{
    u32 i, len;

    for (i = 0; i < ARRAY_COUNT(sSpeakers); i++)
    {
        len = StringLength(sSpeakers[i].name);
        if (StringCompareN(text, sSpeakers[i].name, len) == 0 && text[len] == 0xF0) // 0xF0 = ':' in charmap.txt
            return i;
    }
    len = StringLength(gSaveBlock2Ptr->playerName);
    if (len != 0 && StringCompareN(text, gSaveBlock2Ptr->playerName, len) == 0 && text[len] == 0xF0)
        return SPEAKER_PLAYER;
    return SPEAKER_NONE;
}

static void SetPlatePixel(u8 *tiles, u32 x, u32 y, u32 color)
{
    u8 *byte = &tiles[((y / 8) * 8 + x / 8) * TILE_SIZE_4BPP + (y % 8) * 4 + (x % 8) / 2];

    if (x & 1)
        *byte = (*byte & 0x0F) | (color << 4);
    else
        *byte = (*byte & 0xF0) | color;
}

// Draws the name into a hidden 8x2-tile window (never shown on screen), adds a
// rounded border, and copies it into the plate sprite's tiles.
static bool32 LoadNamePlate(const u8 *name, u32 color)
{
    static const u8 sTextColors[3] = {1, 2, 3}; // background, text, shadow
    struct WindowTemplate template = {.bg = 0, .tilemapLeft = 0, .tilemapTop = 0, .width = 8, .height = 2, .paletteNum = 0, .baseBlock = 0};
    struct SpriteSheet sheet;
    struct SpritePalette palette;
    u16 pal[16] = {0};
    u8 *tiles;
    u32 windowId, font, width, x, y;
    u16 fg = sNameColors[color];

    pal[1] = RGB(31, 31, 30);
    pal[2] = fg;
    pal[3] = RGB((GET_R(fg) + 62) / 3, (GET_G(fg) + 62) / 3, (GET_B(fg) + 62) / 3); // light shadow
    pal[4] = RGB(7, 7, 11);

    windowId = AddWindow(&template);
    if (windowId == WINDOW_NONE)
        return FALSE;
    tiles = AllocZeroed(64 * 32 / 2);
    if (tiles == NULL)
    {
        RemoveWindow(windowId);
        return FALSE;
    }
    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
    font = FONT_NORMAL;
    width = GetStringWidth(font, name, 0);
    if (width > 58)
    {
        font = FONT_SMALL;
        width = GetStringWidth(font, name, 0);
    }
    AddTextPrinterParameterized3(windowId, font, (64 - width) / 2, 1, sTextColors, TEXT_SKIP_DRAW, name);
    memcpy(tiles, gWindows[windowId].tileData, 64 * 16 / 2);
    RemoveWindow(windowId);

    for (x = 0; x < 64; x++)
    {
        SetPlatePixel(tiles, x, 0, 4);
        SetPlatePixel(tiles, x, 15, 4);
    }
    for (y = 0; y < 16; y++)
    {
        SetPlatePixel(tiles, 0, y, 4);
        SetPlatePixel(tiles, 63, y, 4);
    }
    SetPlatePixel(tiles, 0, 0, 0);
    SetPlatePixel(tiles, 63, 0, 0);
    SetPlatePixel(tiles, 0, 15, 0);
    SetPlatePixel(tiles, 63, 15, 0);

    sheet.data = tiles;
    sheet.size = 64 * 32 / 2;
    sheet.tag = TAG_RR_NAMEPLATE;
    LoadSpriteSheet(&sheet); // copies to VRAM straight away
    Free(tiles);
    if (GetSpriteTileStartByTag(TAG_RR_NAMEPLATE) == TAG_NONE)
        return FALSE;
    palette.data = pal;
    palette.tag = TAG_RR_NAMEPLATE;
    if (RR_LoadInterfaceSpritePalette(&palette) == 0xFF)
        return FALSE;
    sPlateIdPlus1 = CreateSprite(&sSpriteTemplate_NamePlate, PLATE_X, PLATE_Y, 0) + 1;
    if (sPlateIdPlus1 - 1 == MAX_SPRITES)
    {
        sPlateIdPlus1 = 0;
        return FALSE;
    }
    gSprites[sPlateIdPlus1 - 1].invisible = TRUE; // shown with the portrait
    return TRUE;
}

static bool32 LoadPortrait(const u32 *gfx, const u16 *pal)
{
    struct SpriteSheet sheet = {.data = gfx, .size = 64 * 64 / 2, .tag = TAG_RR_PORTRAIT};
    struct SpritePalette palette = {.data = pal, .tag = TAG_RR_PORTRAIT};

    LoadSpriteSheet(&sheet);
    if (GetSpriteTileStartByTag(TAG_RR_PORTRAIT) == TAG_NONE)
        return FALSE;
    if (RR_LoadInterfaceSpritePalette(&palette) == 0xFF)
        return FALSE;
    sPortraitIdPlus1 = CreateSprite(&sSpriteTemplate_Portrait, PORTRAIT_X, PORTRAIT_Y, 1) + 1; // behind the plate
    if (sPortraitIdPlus1 - 1 == MAX_SPRITES)
    {
        sPortraitIdPlus1 = 0;
        return FALSE;
    }
    gSprites[sPortraitIdPlus1 - 1].invisible = TRUE; // see SpriteCB_Portrait
    return TRUE;
}

// Called with the expanded message text (gStringVar4) as a field message starts.
void RR_PortraitOnMessage(const u8 *text)
{
    u32 speaker = GetSpeaker(text);
    const struct RRSpeaker *s;
    bool32 alt = FALSE;

    sBoxOpen = TRUE;

    if (speaker < ARRAY_COUNT(sSpeakers))
        alt = sSpeakers[speaker].altFlag != 0 && FlagGet(sSpeakers[speaker].altFlag);

    // Same speaker still on screen: keep the portrait (and its bob) going
    if (speaker != SPEAKER_NONE && sShownSpeaker == (u8)(speaker + 1) && sShownAlt == alt
     && IsSpriteOurs(sPortraitIdPlus1, TAG_RR_PORTRAIT))
        return;

    RR_PortraitHide();
    if (speaker == SPEAKER_NONE)
        return;

    // A busy scene may have no sprite VRAM or palette slot left: skip it
    if (speaker == SPEAKER_PLAYER)
    {
        bool32 male = gSaveBlock2Ptr->playerGender == MALE;
        if (!LoadPortrait(male ? sPortraitGfx_cast_player_m : sPortraitGfx_cast_player_f,
                          male ? sPortraitPal_cast_player_m : sPortraitPal_cast_player_f)
         || !LoadNamePlate(gSaveBlock2Ptr->playerName, RR_NAME_BLUE))
        {
            RR_PortraitHide();
            return;
        }
    }
    else
    {
        s = &sSpeakers[speaker];
        if (!LoadPortrait(alt ? s->altGfx : s->gfx, alt ? s->altPal : s->pal)
         || !LoadNamePlate(s->name, s->color))
        {
            RR_PortraitHide();
            return;
        }
    }
    sShownSpeaker = speaker + 1;
    sShownAlt = alt;
}

// Script: callnative RR_ScriptPortraitHide. Takes the portrait down while the
// box stays up, e.g. before a choice menu that sits where the portrait is.
void RR_ScriptPortraitHide(struct ScriptContext *ctx)
{
    RR_PortraitHide();
}

// ---------------------------------------------------------------------------
// Standalone portraits for other screens (the Pokédex banter, #5): no name
// plate, no box tracking. Up to two at once, each with its own tile/palette tag.
// ---------------------------------------------------------------------------
#define TAG_RR_STANDALONE 0x5A12   // + 0 or 1

static EWRAM_DATA const u16 *sStandalonePal[2] = {NULL};

static void SpriteCB_StandalonePortrait(struct Sprite *sprite)
{
    sprite->data[0] = (sprite->data[0] + 1) % BOB_PERIOD;
    sprite->y2 = (Cos(sprite->data[0] * 256 / BOB_PERIOD, 2) - 2) / 2;
}

// (sprites keep a pointer to their template, so these must not be on the stack)
static const struct SpriteTemplate sSpriteTemplate_StandalonePortrait[2] =
{
    {
        .tileTag = TAG_RR_STANDALONE,
        .paletteTag = TAG_RR_STANDALONE,
        .oam = &sOam_Portrait,
        .anims = gDummySpriteAnimTable,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCB_StandalonePortrait,
    },
    {
        .tileTag = TAG_RR_STANDALONE + 1,
        .paletteTag = TAG_RR_STANDALONE + 1,
        .oam = &sOam_Portrait,
        .anims = gDummySpriteAnimTable,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCB_StandalonePortrait,
    },
};

// Who speaks a line: RR_LINE_PLAYER, RR_LINE_IVY, RR_LINE_OTHER (someone else
// with a portrait) or RR_LINE_NARRATION
u32 RR_GetLineSpeaker(const u8 *text)
{
    u32 speaker = GetSpeaker(text);
    static const u8 sIvy[] = _("Ivy");

    if (speaker == SPEAKER_PLAYER)
        return RR_LINE_PLAYER;
    if (speaker == SPEAKER_NONE)
        return RR_LINE_NARRATION;
    if (StringCompare(sSpeakers[speaker].name, sIvy) == 0)
        return RR_LINE_IVY;
    return RR_LINE_OTHER;
}

// slot 0 or 1; who: RR_LINE_PLAYER or RR_LINE_IVY. Returns the sprite id, or
// MAX_SPRITES if there was no room (the scene just plays without it).
u8 RR_CreateStandalonePortrait(u32 slot, u32 who, s16 x, s16 y, bool32 hFlip)
{
    static const u8 sIvy[] = _("Ivy");
    struct SpriteSheet sheet = {.size = 64 * 64 / 2, .tag = TAG_RR_STANDALONE + slot};
    struct SpritePalette palette = {.tag = TAG_RR_STANDALONE + slot};
    u32 i;
    u8 spriteId;

    if (who == RR_LINE_PLAYER)
    {
        bool32 male = gSaveBlock2Ptr->playerGender == MALE;
        sheet.data = male ? sPortraitGfx_cast_player_m : sPortraitGfx_cast_player_f;
        palette.data = male ? sPortraitPal_cast_player_m : sPortraitPal_cast_player_f;
    }
    else
    {
        for (i = 0; i < ARRAY_COUNT(sSpeakers); i++)
        {
            if (StringCompare(sSpeakers[i].name, sIvy) == 0)
                break;
        }
        if (i == ARRAY_COUNT(sSpeakers))
            return MAX_SPRITES;
        if (sSpeakers[i].altFlag != 0 && FlagGet(sSpeakers[i].altFlag))
        {
            sheet.data = sSpeakers[i].altGfx;
            palette.data = sSpeakers[i].altPal;
        }
        else
        {
            sheet.data = sSpeakers[i].gfx;
            palette.data = sSpeakers[i].pal;
        }
    }
    LoadSpriteSheet(&sheet);
    if (GetSpriteTileStartByTag(sheet.tag) == TAG_NONE)
        return MAX_SPRITES;
    if (LoadSpritePalette(&palette) == 0xFF)
    {
        FreeSpriteTilesByTag(sheet.tag);
        return MAX_SPRITES;
    }
    spriteId = CreateSprite(&sSpriteTemplate_StandalonePortrait[slot], x, y, 1);
    if (spriteId == MAX_SPRITES)
    {
        FreeSpriteTilesByTag(sheet.tag);
        FreeSpritePaletteByTag(sheet.tag);
        return MAX_SPRITES;
    }
    gSprites[spriteId].hFlip = hFlip;
    sStandalonePal[slot] = palette.data;
    return spriteId;
}

// Dim the one who isn't speaking, so it's clear who's talking
void RR_SetStandalonePortraitDim(u32 slot, bool32 dim)
{
    u8 palSlot = IndexOfSpritePaletteTag(TAG_RR_STANDALONE + slot);

    if (palSlot == 0xFF || sStandalonePal[slot] == NULL)
        return;
    LoadPalette(sStandalonePal[slot], OBJ_PLTT_ID(palSlot), PLTT_SIZE_4BPP);
    if (dim)
        BlendPalette(OBJ_PLTT_ID(palSlot), 16, 7, RGB(2, 2, 6));
}

void RR_DestroyStandalonePortrait(u32 slot, u8 spriteId)
{
    if (spriteId < MAX_SPRITES && gSprites[spriteId].inUse && gSprites[spriteId].template->tileTag == TAG_RR_STANDALONE + slot)
        DestroySprite(&gSprites[spriteId]);
    FreeSpriteTilesByTag(TAG_RR_STANDALONE + slot);
    FreeSpritePaletteByTag(TAG_RR_STANDALONE + slot);
    sStandalonePal[slot] = NULL;
}
