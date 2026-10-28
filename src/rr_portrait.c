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
#include "battle.h"
#include "battle_interface.h"
#include "constants/flags.h"
#include "constants/songs.h"
#include "sound.h"

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
    const u32 *fullGfx; // the full-body look (64x64 battle.png), NULL for Pokémon
    const u16 *fullPal;
    const u32 *altFullGfx;
    const u16 *altFullPal;
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
// Standalone portraits for other screens (the Pokédex banter, #5; battle
// lines, #95): no name plate, no box tracking. Up to three at once, each with
// its own tile/palette tag. They bob, can slide in and out, and hop when they
// speak.
//
// The banter look (#5, #95): portraits (head and shoulders, the default) or
// the full-body pictures (cast/*/battle.png, 64x64) while
// FLAG_RR_BANTER_FULL_BODY is set (Debug > Scripts > 13 switches it). Pokémon
// speakers always use their portrait.
// ---------------------------------------------------------------------------
#define TAG_RR_STANDALONE 0x5A12   // + 0, 1 or 2
#define TAG_RR_EMOTE      0x5A15
#define TAG_RR_STAMP      0x5A16
#define STANDALONE_SLOTS  3
#define SLIDE_SPEED       8     // pixels per frame
#define HOP_FRAMES        12
#define HOP_HEIGHT        6

// sprite data
#define sBobPhase data[0]
#define sTargetX  data[3]
#define sHopTimer data[4]

static EWRAM_DATA const u16 *sStandalonePal[STANDALONE_SLOTS] = {NULL};

bool32 RR_BanterFullBody(void)
{
    return FlagGet(FLAG_RR_BANTER_FULL_BODY);
}

static void SpriteCB_StandalonePortrait(struct Sprite *sprite)
{
    s32 hop = 0;

    sprite->sBobPhase = (sprite->sBobPhase + 1) % BOB_PERIOD;
    if (sprite->x < sprite->sTargetX)
        sprite->x = min(sprite->x + SLIDE_SPEED, sprite->sTargetX);
    else if (sprite->x > sprite->sTargetX)
        sprite->x = max(sprite->x - SLIDE_SPEED, sprite->sTargetX);
    if (sprite->sHopTimer > 0)
    {
        sprite->sHopTimer--;
        hop = Sin(sprite->sHopTimer * 128 / HOP_FRAMES, HOP_HEIGHT); // up and back down
    }
    sprite->y2 = (Cos(sprite->sBobPhase * 256 / BOB_PERIOD, 2) - 2) / 2 - hop;
}

// (sprites keep a pointer to their template, so these must not be on the stack)
// slot 2 is Elm's or Jenny's call face (32x32), see RR_CreateStandaloneCallFace
static const struct OamData sOam_CallFace =
{
    .shape = SPRITE_SHAPE(32x32),
    .size = SPRITE_SIZE(32x32),
    .priority = 0,
};

#define STANDALONE_TEMPLATE(n, _oam)                    \
    {                                                   \
        .tileTag = TAG_RR_STANDALONE + (n),             \
        .paletteTag = TAG_RR_STANDALONE + (n),          \
        .oam = _oam,                                    \
        .anims = gDummySpriteAnimTable,                 \
        .affineAnims = gDummySpriteAffineAnimTable,     \
        .callback = SpriteCB_StandalonePortrait,        \
    }
static const struct SpriteTemplate sSpriteTemplate_StandalonePortrait[STANDALONE_SLOTS] =
{
    STANDALONE_TEMPLATE(0, &sOam_Portrait),
    STANDALONE_TEMPLATE(1, &sOam_Portrait),
    STANDALONE_TEMPLATE(2, &sOam_CallFace),
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

static u32 FindSpeaker(const u8 *name)
{
    u32 i;

    for (i = 0; i < ARRAY_COUNT(sSpeakers); i++)
    {
        if (StringCompare(sSpeakers[i].name, name) == 0)
            return i;
    }
    return SPEAKER_NONE;
}

// The picture for a speaker (an index into sSpeakers, or SPEAKER_PLAYER) in
// the current banter look and outfit
static bool32 GetSpeakerPicture(u32 speaker, const u32 **gfx, const u16 **pal)
{
    bool32 full = RR_BanterFullBody();

    if (speaker == SPEAKER_PLAYER)
    {
        bool32 male = gSaveBlock2Ptr->playerGender == MALE;
        if (full)
        {
            *gfx = male ? sFullBodyGfx_cast_player_m : sFullBodyGfx_cast_player_f;
            *pal = male ? sFullBodyPal_cast_player_m : sFullBodyPal_cast_player_f;
        }
        else
        {
            *gfx = male ? sPortraitGfx_cast_player_m : sPortraitGfx_cast_player_f;
            *pal = male ? sPortraitPal_cast_player_m : sPortraitPal_cast_player_f;
        }
        return TRUE;
    }
    if (speaker >= ARRAY_COUNT(sSpeakers))
        return FALSE;
    if (sSpeakers[speaker].altFlag != 0 && FlagGet(sSpeakers[speaker].altFlag))
    {
        *gfx = (full && sSpeakers[speaker].altFullGfx != NULL) ? sSpeakers[speaker].altFullGfx : sSpeakers[speaker].altGfx;
        *pal = (full && sSpeakers[speaker].altFullGfx != NULL) ? sSpeakers[speaker].altFullPal : sSpeakers[speaker].altPal;
    }
    else
    {
        *gfx = (full && sSpeakers[speaker].fullGfx != NULL) ? sSpeakers[speaker].fullGfx : sSpeakers[speaker].gfx;
        *pal = (full && sSpeakers[speaker].fullGfx != NULL) ? sSpeakers[speaker].fullPal : sSpeakers[speaker].pal;
    }
    return TRUE;
}

static u8 CreateStandalone(u32 slot, const u32 *gfx, const u16 *pal, u32 size, s16 x, s16 y, bool32 hFlip)
{
    struct SpriteSheet sheet = {.data = gfx, .size = size, .tag = TAG_RR_STANDALONE + slot};
    struct SpritePalette palette = {.data = pal, .tag = TAG_RR_STANDALONE + slot};
    u8 spriteId;

    // no free sprite tiles or colour slot (a busy screen, especially a
    // battle): the line just plays without the picture
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
    gSprites[spriteId].sTargetX = x;
    sStandalonePal[slot] = pal;
    return spriteId;
}

// slot 0-2; who: RR_LINE_PLAYER or RR_LINE_IVY. Returns the sprite id, or
// MAX_SPRITES if there was no room (the scene just plays without it).
u8 RR_CreateStandalonePortrait(u32 slot, u32 who, s16 x, s16 y, bool32 hFlip)
{
    static const u8 sIvy[] = _("Ivy");
    const u32 *gfx;
    const u16 *pal;

    if (!GetSpeakerPicture(who == RR_LINE_PLAYER ? SPEAKER_PLAYER : FindSpeaker(sIvy), &gfx, &pal))
        return MAX_SPRITES;
    if (slot >= 2)
        return MAX_SPRITES;
    return CreateStandalone(slot, gfx, pal, 64 * 64 / 2, x, y, hFlip);
}

// Elm's and Jenny's small video-call faces (32x32, cast/*/call.png): they
// "call in" for their family's banter lines
static const u32 sCallGfx_Elm[] = INCBIN_U32("graphics/research_red/cast/elm/call.4bpp");
static const u16 sCallPal_Elm[] = INCBIN_U16("graphics/research_red/cast/elm/call.gbapal");
static const u32 sCallGfx_Jenny[] = INCBIN_U32("graphics/research_red/cast/jenny/call.4bpp");
static const u16 sCallPal_Jenny[] = INCBIN_U16("graphics/research_red/cast/jenny/call.gbapal");

// always slot 2
u8 RR_CreateStandaloneCallFace(bool32 elm, s16 x, s16 y)
{
    return CreateStandalone(2, elm ? sCallGfx_Elm : sCallGfx_Jenny, elm ? sCallPal_Elm : sCallPal_Jenny,
                            32 * 32 / 2, x, y, FALSE);
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

static bool32 IsStandalone(u32 slot, u8 spriteId)
{
    return spriteId < MAX_SPRITES && gSprites[spriteId].inUse
        && gSprites[spriteId].template == &sSpriteTemplate_StandalonePortrait[slot];
}

// Start off-screen at fromX and slide to where it was created
void RR_StandalonePortraitSlideIn(u32 slot, u8 spriteId, s16 fromX)
{
    if (!IsStandalone(slot, spriteId))
        return;
    gSprites[spriteId].sTargetX = gSprites[spriteId].x;
    gSprites[spriteId].x = fromX;
}

void RR_StandalonePortraitSlideTo(u32 slot, u8 spriteId, s16 x)
{
    if (IsStandalone(slot, spriteId))
        gSprites[spriteId].sTargetX = x;
}

bool32 RR_StandalonePortraitSliding(u32 slot, u8 spriteId)
{
    return IsStandalone(slot, spriteId) && gSprites[spriteId].x != gSprites[spriteId].sTargetX;
}

// A little hop, when their line starts
void RR_StandalonePortraitHop(u32 slot, u8 spriteId)
{
    if (IsStandalone(slot, spriteId))
        gSprites[spriteId].sHopTimer = HOP_FRAMES;
}

void RR_DestroyStandalonePortrait(u32 slot, u8 spriteId)
{
    if (IsStandalone(slot, spriteId))
        DestroySprite(&gSprites[spriteId]);
    FreeSpriteTilesByTag(TAG_RR_STANDALONE + slot);
    FreeSpritePaletteByTag(TAG_RR_STANDALONE + slot);
    sStandalonePal[slot] = NULL;
}

// ---------------------------------------------------------------------------
// Emote bubbles (#5): ! ? or a music note pops up above a speaker on some
// lines, rises a little and goes. One at a time.
// graphics/research_red/banter/emotes.png (tools/research_red/build_banter_fx.py)
// ---------------------------------------------------------------------------
static const u32 sEmoteGfx[] = INCBIN_U32("graphics/research_red/banter/emotes.4bpp");
static const u16 sEmotePal[] = INCBIN_U16("graphics/research_red/banter/emotes.gbapal");

static const struct OamData sOam_Emote =
{
    .shape = SPRITE_SHAPE(16x16),
    .size = SPRITE_SIZE(16x16),
    .priority = 0,
};

#define EMOTE_FRAMES 50

static void SpriteCB_Emote(struct Sprite *sprite)
{
    if (++sprite->data[0] % 4 == 0 && sprite->data[0] < 24)
        sprite->y2--;
    if (sprite->data[0] >= EMOTE_FRAMES)
        RR_HideEmote();
}

static const struct SpriteTemplate sSpriteTemplate_Emote =
{
    .tileTag = TAG_RR_EMOTE,
    .paletteTag = TAG_RR_EMOTE,
    .oam = &sOam_Emote,
    .anims = gDummySpriteAnimTable,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_Emote,
};

static EWRAM_DATA u8 sEmoteIdPlus1 = 0;

void RR_HideEmote(void)
{
    if (sEmoteIdPlus1 != 0 && gSprites[sEmoteIdPlus1 - 1].inUse
     && gSprites[sEmoteIdPlus1 - 1].template == &sSpriteTemplate_Emote)
        DestroySprite(&gSprites[sEmoteIdPlus1 - 1]);
    sEmoteIdPlus1 = 0;
    FreeSpriteTilesByTag(TAG_RR_EMOTE);
    FreeSpritePaletteByTag(TAG_RR_EMOTE);
}

// Which bubble a line gets: a question (ends in ?), a song (has a music
// note), or an exclamation (two or more !); most lines get none
u32 RR_GetLineEmote(const u8 *text)
{
    const u8 *c, *last = NULL;
    u32 bangs = 0;

    for (c = text; *c != EOS; c++)
    {
        if (*c == CHAR_EXTRA_SYMBOL && c[1] == 0xE9) // EMOJI_NOTE (charmap.txt)
            return RR_EMOTE_NOTE;
        if (*c == CHAR_EXCL_MARK)
            bangs++;
        if (*c != CHAR_SPACE && *c != CHAR_NEWLINE && *c != CHAR_PROMPT_SCROLL && *c != CHAR_PROMPT_CLEAR)
            last = c;
    }
    if (last != NULL && *last == CHAR_QUESTION_MARK)
        return RR_EMOTE_QUESTION;
    if (bangs >= 2)
        return RR_EMOTE_EXCLAIM;
    return RR_EMOTE_NONE;
}

void RR_ShowEmote(u32 emote, s16 x, s16 y)
{
    struct SpriteSheet sheet = {.data = sEmoteGfx, .size = 16 * 48 / 2, .tag = TAG_RR_EMOTE};
    struct SpritePalette palette = {.data = sEmotePal, .tag = TAG_RR_EMOTE};
    u8 spriteId;

    RR_HideEmote();
    if (emote == RR_EMOTE_NONE)
        return;
    LoadSpriteSheet(&sheet);
    if (GetSpriteTileStartByTag(TAG_RR_EMOTE) == TAG_NONE)
        return;
    if (LoadSpritePalette(&palette) == 0xFF)
    {
        FreeSpriteTilesByTag(TAG_RR_EMOTE);
        return;
    }
    spriteId = CreateSprite(&sSpriteTemplate_Emote, x, y, 0);
    if (spriteId == MAX_SPRITES)
    {
        RR_HideEmote();
        return;
    }
    gSprites[spriteId].oam.tileNum += (emote - RR_EMOTE_EXCLAIM) * 4; // frames stacked top to bottom
    sEmoteIdPlus1 = spriteId + 1;
}

// ---------------------------------------------------------------------------
// The dex number stamped on like an ink stamp (#5): it drops onto the page
// from twice its size with a thump, slightly crooked, and stays for the banter
// ---------------------------------------------------------------------------
static const union AffineAnimCmd sAffineAnim_Stamp[] =
{
    AFFINEANIMCMD_FRAME(0x200, 0x200, -6, 0),
    AFFINEANIMCMD_FRAME(-0x20, -0x20, 0, 8),
    AFFINEANIMCMD_FRAME(0, 0, 0, 1),
    AFFINEANIMCMD_END,
};
static const union AffineAnimCmd *const sAffineAnims_Stamp[] = {sAffineAnim_Stamp};

static const struct OamData sOam_Stamp =
{
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .shape = SPRITE_SHAPE(64x32),
    .size = SPRITE_SIZE(64x32),
    .priority = 0,
};

static const struct SpriteTemplate sSpriteTemplate_Stamp =
{
    .tileTag = TAG_RR_STAMP,
    .paletteTag = TAG_RR_STAMP,
    .oam = &sOam_Stamp,
    .anims = gDummySpriteAnimTable,
    .affineAnims = sAffineAnims_Stamp,
    .callback = SpriteCallbackDummy,
};

static EWRAM_DATA u8 sStampIdPlus1 = 0;

void RR_HideDexStamp(void)
{
    if (sStampIdPlus1 != 0 && gSprites[sStampIdPlus1 - 1].inUse
     && gSprites[sStampIdPlus1 - 1].template == &sSpriteTemplate_Stamp)
    {
        FreeOamMatrix(gSprites[sStampIdPlus1 - 1].oam.matrixNum);
        DestroySprite(&gSprites[sStampIdPlus1 - 1]);
    }
    sStampIdPlus1 = 0;
    FreeSpriteTilesByTag(TAG_RR_STAMP);
    FreeSpritePaletteByTag(TAG_RR_STAMP);
}

void RR_ShowDexStamp(u32 dexNum, s16 x, s16 y)
{
    static const u8 sTextColors[3] = {0, 2, 3}; // clear background, ink, faded ink
    static const u8 sStampText[] = _("Nº{STR_VAR_1}");
    struct WindowTemplate template = {.bg = 0, .tilemapLeft = 0, .tilemapTop = 0, .width = 8, .height = 2, .paletteNum = 0, .baseBlock = 0};
    struct SpriteSheet sheet;
    struct SpritePalette palette;
    u16 pal[16] = {0};
    u8 text[16];
    u8 *tiles;
    u32 windowId, width, x0, xx, yy;
    u8 spriteId;

    RR_HideDexStamp();
    pal[2] = RGB(25, 5, 5);   // red ink
    pal[3] = RGB(29, 16, 14); // where the ink is thin
    windowId = AddWindow(&template);
    if (windowId == WINDOW_NONE)
        return;
    tiles = AllocZeroed(64 * 32 / 2);
    if (tiles == NULL)
    {
        RemoveWindow(windowId);
        return;
    }
    ConvertIntToDecimalStringN(gStringVar1, dexNum, STR_CONV_MODE_LEADING_ZEROS, dexNum >= 1000 ? 4 : 3); // Nº001, as in the Pokédex
    StringExpandPlaceholders(text, sStampText);
    FillWindowPixelBuffer(windowId, PIXEL_FILL(0));
    width = GetStringWidth(FONT_NORMAL, text, 0);
    x0 = (64 - width) / 2;
    AddTextPrinterParameterized3(windowId, FONT_NORMAL, x0, 1, sTextColors, TEXT_SKIP_DRAW, text);
    memcpy(tiles, gWindows[windowId].tileData, 64 * 16 / 2);
    RemoveWindow(windowId);
    // a rubber-stamp frame around the number
    for (xx = x0 - 4; xx < x0 + width + 4 && xx < 64; xx++)
    {
        SetPlatePixel(tiles, xx, 0, 2);
        SetPlatePixel(tiles, xx, 15, 2);
    }
    for (yy = 0; yy < 16; yy++)
    {
        SetPlatePixel(tiles, x0 - 4, yy, 2);
        SetPlatePixel(tiles, min(x0 + width + 3, 63), yy, 2);
    }
    sheet.data = tiles;
    sheet.size = 64 * 32 / 2;
    sheet.tag = TAG_RR_STAMP;
    LoadSpriteSheet(&sheet);
    Free(tiles);
    if (GetSpriteTileStartByTag(TAG_RR_STAMP) == TAG_NONE)
        return;
    palette.data = pal;
    palette.tag = TAG_RR_STAMP;
    if (LoadSpritePalette(&palette) == 0xFF)
    {
        FreeSpriteTilesByTag(TAG_RR_STAMP);
        return;
    }
    spriteId = CreateSprite(&sSpriteTemplate_Stamp, x, y + 8, 0); // the plate is the top half of the 64x32
    if (spriteId == MAX_SPRITES)
    {
        RR_HideDexStamp();
        return;
    }
    sStampIdPlus1 = spriteId + 1;
    PlaySE(SE_BALL_BOUNCE_1);
}

// ---------------------------------------------------------------------------
// In battle (#95): when a page of the battle message is spoken by someone with
// a portrait ("Ivy: That's the best kind, Elm."), their portrait stands above
// the message box until the page changes: you and Ivy on the left, over your
// Pokémon; anyone else (Elm, Jenny, Oak, Daisy...) on the right, facing you.
// Pairs who fight as one trainer share one battle picture (#107), so the
// portrait shows which of them is talking. While the other side's portrait is
// up it stands where your HP boxes are, so those hide until it goes (only the
// ones that were showing come back). Called every frame from BattleMainCB2.
// ---------------------------------------------------------------------------
#define BATTLE_PORTRAIT_X       40
#define BATTLE_PORTRAIT_X_OTHER 200
#define BATTLE_PORTRAIT_Y       80   // bottom edge on the message box (y 112)

static EWRAM_DATA u8 sBattleSpeakerIdPlus1 = 0;   // sprite id + 1 (0: none)
static EWRAM_DATA u8 sBattleSpeaker = 0;  // who has the portrait (only read while one is up)
static EWRAM_DATA u8 sHiddenHealthboxes = 0;  // battlers whose HP box the portrait hid (bits)

// Hide (or bring back) your side's HP boxes under the other side's portrait
static void HideOurHealthboxes(bool32 hide)
{
    u32 battler;

    for (battler = 0; battler < gBattlersCount; battler++)
    {
        if (!IsOnPlayerSide(battler))
            continue;
        if (hide && !gSprites[gHealthboxSpriteIds[battler]].invisible)
        {
            SetHealthboxSpriteInvisible(gHealthboxSpriteIds[battler]);
            sHiddenHealthboxes |= 1u << battler;
        }
        else if (!hide && (sHiddenHealthboxes & (1u << battler)))
        {
            SetHealthboxSpriteVisible(gHealthboxSpriteIds[battler]);
        }
    }
    if (!hide)
        sHiddenHealthboxes = 0;
}

static bool32 IsBattlePortraitUp(void)
{
    u32 id = sBattleSpeakerIdPlus1 - 1;
    return sBattleSpeakerIdPlus1 != 0 && gSprites[id].inUse
        && gSprites[id].template == &sSpriteTemplate_StandalonePortrait[0];
}

void RR_UpdateBattleSpeakerPortrait(const u8 *text)
{
    static const u8 sIvy[] = _("Ivy");
    const u8 *cur, *page;
    const u32 *gfx;
    const u16 *pal;
    u8 state;
    u32 who = SPEAKER_NONE;
    bool32 ourSide;

    if (RR_GetWindowPrinterPos(0, &cur, &state) && cur > text) // window 0: the battle message box
    {
        page = cur;
        if (state == RENDER_STATE_CLEAR && page[-1] == CHAR_PROMPT_CLEAR)
            page--; // waiting at a page break: that page is still on screen
        while (page > text && page[-1] != CHAR_PROMPT_CLEAR)
            page--;
        who = GetSpeaker(page);
    }
    if (!IsBattlePortraitUp())
    {
        // gone without us (the battle ended and its sprites were freed): the
        // HP boxes went with it, nothing to bring back
        sBattleSpeaker = SPEAKER_NONE;
        sHiddenHealthboxes = 0;
    }
    if (who == sBattleSpeaker)
        return;
    if (IsBattlePortraitUp())
    {
        RR_DestroyStandalonePortrait(0, sBattleSpeakerIdPlus1 - 1);
        HideOurHealthboxes(FALSE);
    }
    sBattleSpeakerIdPlus1 = 0;
    sBattleSpeaker = SPEAKER_NONE;
    if (who == SPEAKER_NONE || !GetSpeakerPicture(who, &gfx, &pal))
        return;
    ourSide = who == SPEAKER_PLAYER || who == FindSpeaker(sIvy);
    sBattleSpeakerIdPlus1 = CreateStandalone(0, gfx, pal, 64 * 64 / 2,
                                             ourSide ? BATTLE_PORTRAIT_X : BATTLE_PORTRAIT_X_OTHER,
                                             BATTLE_PORTRAIT_Y, !ourSide) + 1;
    if (sBattleSpeakerIdPlus1 - 1 < MAX_SPRITES)
    {
        sBattleSpeaker = who;
        RR_StandalonePortraitHop(0, sBattleSpeakerIdPlus1 - 1);
        if (!ourSide)
            HideOurHealthboxes(TRUE);
    }
    else
        sBattleSpeakerIdPlus1 = 0;
}
