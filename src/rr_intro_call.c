// Research Red: the new-game intro, a Discord-style video call (animatic 00).
//
// Replaces FireRed's controls guide, Pikachu intro and Oak speech (those are
// left in src/oak_speech.c, unused). Layout, from the animatic:
//   header   red "live" dot, call title, round avatars that pop in as people
//            join (Slowpoke, Elm, Jenny, Ivy), signal bars
//   left     Oak's video (Slowpoke shows up in it)
//   right    the player's video (appears once Oak "remembers" who you are)
// Whoever is talking gets a gold border/ring. At the end everyone leaves except
// Slowpoke, who never hangs up.
//
// The call is a list of steps (sCallScript); dialogue lines name their speaker
// with the usual "Name:" prefix. Graphics: tools/research_red/build_call_ui.py.
//
// Later calls (#13) reuse the same screen: each is a struct CallDef in sCalls
// (title, who is on the left "host" video, who is already in the call, and its
// steps), started from a script with
//     setvar VAR_0x8004, RR_CALL_RULES   @ include/constants/rr_calls.h
//     callnative RR_ScriptStartCall      @ after fadescreen FADE_TO_BLACK
//     waitstate
// and they return to the map. The intro (RR_CALL_INTRO) goes on to a new game.
#include "global.h"
#include "bg.h"
#include "event_data.h"
#include "gpu_regs.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "naming_screen.h"
#include "overworld.h"
#include "palette.h"
#include "random.h"
#include "rr_intro_call.h"
#include "scanline_effect.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "window.h"
#include "script.h"
#include "constants/rr_calls.h"
#include "constants/songs.h"

// ---------------------------------------------------------------------------
// Graphics
// ---------------------------------------------------------------------------
static const u16 sCallBg_Pal[] = INCBIN_U16("graphics/research_red/intro_call/bg.gbapal");
static const u32 sCallBg_Tiles[] = INCBIN_U32("graphics/research_red/intro_call/bg.4bpp");
static const u16 sCallBg_Tilemap[] = INCBIN_U16("graphics/research_red/intro_call/bg.bin");
static const u32 sCircle_Slowpoke_Gfx[] = INCBIN_U32("graphics/research_red/intro_call/circle_slowpoke.4bpp");
static const u16 sCircle_Slowpoke_Pal[] = INCBIN_U16("graphics/research_red/intro_call/circle_slowpoke.gbapal");
static const u32 sCircle_Elm_Gfx[] = INCBIN_U32("graphics/research_red/intro_call/circle_elm.4bpp");
static const u16 sCircle_Elm_Pal[] = INCBIN_U16("graphics/research_red/intro_call/circle_elm.gbapal");
static const u32 sCircle_Jenny_Gfx[] = INCBIN_U32("graphics/research_red/intro_call/circle_jenny.4bpp");
static const u16 sCircle_Jenny_Pal[] = INCBIN_U16("graphics/research_red/intro_call/circle_jenny.gbapal");
static const u32 sCircle_Ivy_Gfx[] = INCBIN_U32("graphics/research_red/intro_call/circle_ivy.4bpp");
static const u16 sCircle_Ivy_Pal[] = INCBIN_U16("graphics/research_red/intro_call/circle_ivy.gbapal");
static const u32 sRing_Gfx[] = INCBIN_U32("graphics/research_red/intro_call/ring.4bpp");
static const u32 sVideo_Oak_Gfx[] = INCBIN_U32("graphics/research_red/cast/oak/portrait.4bpp");
static const u16 sVideo_Oak_Pal[] = INCBIN_U16("graphics/research_red/cast/oak/portrait.gbapal");
static const u32 sVideo_Elm_Gfx[] = INCBIN_U32("graphics/research_red/cast/elm/portrait.4bpp");
static const u16 sVideo_Elm_Pal[] = INCBIN_U16("graphics/research_red/cast/elm/portrait.gbapal");
static const u32 sVideo_Slowpoke_Gfx[] = INCBIN_U32("graphics/research_red/pokemon/slowpoke/portrait.4bpp");
static const u16 sVideo_Slowpoke_Pal[] = INCBIN_U16("graphics/research_red/pokemon/slowpoke/portrait.gbapal");
static const u32 sVideo_PlayerM_Gfx[] = INCBIN_U32("graphics/research_red/cast/player_m/portrait.4bpp");
static const u16 sVideo_PlayerM_Pal[] = INCBIN_U16("graphics/research_red/cast/player_m/portrait.gbapal");
static const u32 sVideo_PlayerF_Gfx[] = INCBIN_U32("graphics/research_red/cast/player_f/portrait.4bpp");
static const u16 sVideo_PlayerF_Pal[] = INCBIN_U16("graphics/research_red/cast/player_f/portrait.gbapal");

static const u16 sRingPal_Idle[16] = {0, RGB(31, 31, 31)};
static const u16 sRingPal_Talking[16] = {0, RGB(31, 25, 11)};

#define BG_PAL_SLOT        1
#define PAL_LIVE_DOT       5
#define PAL_SIGNAL_BAR(i)  (6 + (i))
#define PAL_BORDER_HOST     10
#define PAL_BORDER_PLAYER  11
#define COLOR_BORDER       RGB(7, 7, 11)
#define COLOR_TALKING      RGB(31, 25, 11)
#define COLOR_BAR_ON       RGB(31, 31, 31)
#define COLOR_BAR_OFF      RGB(11, 11, 12)

enum
{
    VIDEO_HOST,     // left: whoever hosts the call (Oak in the intro)
    VIDEO_SLOWPOKE,
    VIDEO_PLAYER,
    NUM_VIDEOS,
};

enum
{
    CIRCLE_SLOWPOKE,
    CIRCLE_ELM,
    CIRCLE_JENNY,
    CIRCLE_IVY,
    NUM_CIRCLES,
};

#define TAG_VIDEO(i)   (0x5B00 + (i))
#define TAG_CIRCLE(i)  (0x5B10 + (i))
#define TAG_RING       0x5B20
#define TAG_RING_IDLE  0x5B21
#define TAG_RING_TALK  0x5B22

static const s16 sVideoPos[NUM_VIDEOS][2] = {{46, 72}, {90, 78}, {180, 72}};
#define CIRCLE_X(i)    (112 + (i) * 26)
#define CIRCLE_Y       17

// ---------------------------------------------------------------------------
// The call script
// ---------------------------------------------------------------------------
enum
{
    STEP_LINE,          // print text (speaker from its "Name:" prefix)
    STEP_POP,           // arg: circle joins the call
    STEP_DROP,          // arg: circle leaves
    STEP_SLOWPOKE,      // Slowpoke appears in Oak's video
    STEP_ELM_ZOOM,      // arg: TRUE = Elm's camera is one eyeball
    STEP_SIGNAL,        // arg: signal bars (1-4)
    STEP_FUZZ,          // arg: frames of static
    STEP_GENDER,        // which student are you? (shows the player's video)
    STEP_NAME,          // and your name was...?
    STEP_HOST_LEAVES,   // the left video drops out
    STEP_PLAYER_LEAVES, // the player's video shrinks away
    STEP_WAIT,          // arg: frames
    STEP_END,
};

struct CallStep
{
    u8 type;
    u8 arg;
    const u8 *text;
};

#define DO(t, a)   {t, a, NULL}

static const struct CallStep sCallScript[] =
{
    DO(STEP_SIGNAL, 2),
    DO(STEP_WAIT, 30),
    // Version stamp for test builds (bump every build)
    {STEP_LINE, 0, (const u8[])_("Research Red\nv0.0.22 test build\p")},
    {STEP_LINE, 0, (const u8[])_("Oak: Hello? Hello!\nIs this thing on?\p")},
    {STEP_LINE, 0, (const u8[])_("Oak: Ah, there you are! Welcome to\nthe world of Pokémon!\p")},
    {STEP_LINE, 0, (const u8[])_("Oak: My name is Oak! People call\nme the Pokémon Prof-\p")},
    {STEP_LINE, 0, (const u8[])_("???: Professor, we know.\nYou taught us.\p")},
    {STEP_LINE, 0, (const u8[])_("Oak: Ho ho! Forty years of opening\nlectures. It slips out.\p")},
    DO(STEP_SLOWPOKE, 0),
    DO(STEP_POP, CIRCLE_SLOWPOKE),
    DO(STEP_WAIT, 40),
    {STEP_LINE, 0, (const u8[])_("Oak: And this is Slowpoke!\nSay hello!\p")},
    {STEP_LINE, 0, (const u8[])_("Slowpoke: …\p")},
    {STEP_LINE, 0, (const u8[])_("???: Is the call frozen?\p")},
    {STEP_LINE, 0, (const u8[])_("Oak: No, that's just Slowpoke.\p")},
    {STEP_LINE, 0, (const u8[])_("Oak: Now, the others should be\njoining us any moment…\p")},
    DO(STEP_ELM_ZOOM, TRUE),
    DO(STEP_POP, CIRCLE_ELM),
    DO(STEP_WAIT, 27),
    {STEP_LINE, 0, (const u8[])_("Elm: Professor! Is that them?\nLet me see!\p")},
    {STEP_LINE, 0, (const u8[])_("Oak: Elm, you're too close to\nthe camera.\p")},
    DO(STEP_ELM_ZOOM, FALSE),
    DO(STEP_WAIT, 30),
    {STEP_LINE, 0, (const u8[])_("Elm: Sorry! Sorry. Hi!!\p")},
    DO(STEP_SIGNAL, 1),
    DO(STEP_FUZZ, 40),
    {STEP_LINE, 0, (const u8[])_("Oak: Hmm, the signal's fuzzy.\nRemind me…\p")},
    {STEP_LINE, 0, (const u8[])_("Oak: Which of my students\nare you?")},
    DO(STEP_GENDER, 0),
    {STEP_LINE, 0, (const u8[])_("Oak: Ah, of course! My old\nstudent! And your name was…?")},
    DO(STEP_NAME, 0),
    {STEP_LINE, 0, (const u8[])_("Oak: {PLAYER}! Right.\nI knew that.\p")},
    DO(STEP_SIGNAL, 4),
    DO(STEP_WAIT, 27),
    {STEP_LINE, 0, (const u8[])_("Elm: He knew. It loads slowly.\nLike Slowpoke.\p")},
    DO(STEP_POP, CIRCLE_JENNY),
    DO(STEP_WAIT, 27),
    {STEP_LINE, 0, (const u8[])_("Jenny: Dear. Chair.\p")},
    {STEP_LINE, 0, (const u8[])_("Oak: Jenny! Elm's Jenny! I'd know\nyou anywhere.\p")},
    {STEP_LINE, 0, (const u8[])_("Jenny: …That's a first, Professor.\p")},
    {STEP_LINE, 0, (const u8[])_("Oak: {PLAYER}, is Ivy with you?\p")},
    DO(STEP_POP, CIRCLE_IVY),
    DO(STEP_WAIT, 27),
    {STEP_LINE, 0, (const u8[])_("Ivy: Hello, Professor!\nWe're stuck at sea.\p")},
    {STEP_LINE, 0, (const u8[])_("Ivy: A Tentacool is sitting on the\nanchor line.\p")},
    {STEP_LINE, 0, (const u8[])_("{PLAYER}: It's been two hours.\nWe've named it Gerald.\p")},
    {STEP_LINE, 0, (const u8[])_("Oak: Ho ho! Well, Delia has your\nroom ready.\p")},
    {STEP_LINE, 0, (const u8[])_("Oak: Now, the plan. One summer,\none joint paper.\p")},
    {STEP_LINE, 0, (const u8[])_("Oak: I'll lead it, and all four of\nyou are authors.\p")},
    {STEP_LINE, 0, (const u8[])_("Elm: Authors! Plural! Professor,\nI have a draft title!\p")},
    {STEP_LINE, 0, (const u8[])_("Jenny: He has eleven.\p")},
    {STEP_LINE, 0, (const u8[])_("Jenny: I'll do the figures.\nI like a good diagram.\p")},
    {STEP_LINE, 0, (const u8[])_("Oak: Splendid! Come by the lab\ntomorrow. I have a surprise.\p")},
    {STEP_LINE, 0, (const u8[])_("Elm: And I'm already in Pallet!\nHome field advantage!\p")},
    {STEP_LINE, 0, (const u8[])_("{PLAYER}: You're staying in Daisy's\nguest room, Elm.\p")},
    {STEP_LINE, 0, (const u8[])_("Elm: …Home-ish field advantage!\p")},
    {STEP_LINE, 0, (const u8[])_("Elm: …It's good you're here,\n{PLAYER}. Really.\p")},
    {STEP_LINE, 0, (const u8[])_("{PLAYER}: Aw, Elm.\nAre you crying?\p")},
    {STEP_LINE, 0, (const u8[])_("Elm: It's allergies!\nDaisy has a Persian!\p")},
    {STEP_LINE, 0, (const u8[])_("Jenny: She has a Rattata, dear.\p")},
    {STEP_LINE, 0, (const u8[])_("Oak: Your very own research\nlegend is about to-\p")},
    {STEP_LINE, 0, (const u8[])_("Elm: -begin!! Sorry. I've always\nwanted to say that.\p")},
    {STEP_LINE, 0, (const u8[])_("Oak: …\p")},
    {STEP_LINE, 0, (const u8[])_("Jenny: I'm taking his tablet away.\p")},
    {STEP_LINE, 0, (const u8[])_("Ivy: Gerald's off! See you in\nPallet, everyone!\p")},
    {STEP_LINE, 0, (const u8[])_("{PLAYER}: Bye, Gerald. We'll\nalways have the anchor.\p")},
    DO(STEP_DROP, CIRCLE_IVY),
    DO(STEP_WAIT, 33),
    DO(STEP_DROP, CIRCLE_JENNY),
    DO(STEP_WAIT, 33),
    DO(STEP_DROP, CIRCLE_ELM),
    DO(STEP_WAIT, 42),
    DO(STEP_HOST_LEAVES, 0),
    DO(STEP_WAIT, 42),
    DO(STEP_PLAYER_LEAVES, 0),
    DO(STEP_WAIT, 90),
    // Everyone's gone. Slowpoke never hung up.
    {STEP_LINE, 0, (const u8[])_("Slowpoke: …Slow?\p")},
    DO(STEP_WAIT, 40),
    DO(STEP_END, 0),
};

// #45: after changing a Research Rule (Options > Research Rules). The same
// lines for every rule for now. Elm hosts; Jenny and Ivy are in the call.
static const struct CallStep sRulesCallScript[] =
{
    DO(STEP_WAIT, 20),
    {STEP_LINE, 0, (const u8[])_("Elm: Hello? Oh! {PLAYER}! Ivy!\nHi, hi!\p")},
    {STEP_LINE, 0, (const u8[])_("Ivy: Hi, Elm. We changed one of\nthe Research Rules.\p")},
    {STEP_LINE, 0, (const u8[])_("Elm: A rule change! Bold! Daring!\nIt's practically evolution!\p")},
    {STEP_LINE, 0, (const u8[])_("Jenny: Noted, dated and initialed.\nIn my professional opinion, legal.\p")},
    {STEP_LINE, 0, (const u8[])_("Jenny: The bet still stands, dear.\nLosers cook dinner at Delia's.\p")},
    {STEP_LINE, 0, (const u8[])_("Elm: Dessert included!\nBye-bye!\p")},
    DO(STEP_DROP, CIRCLE_JENNY),
    DO(STEP_WAIT, 30),
    DO(STEP_HOST_LEAVES, 0),
    DO(STEP_WAIT, 50),
    DO(STEP_END, 0),
};

static const u8 sText_CallTitle[] = _("Oak Lab - live");
static const u8 sText_RulesCallTitle[] = _("Rules call - live");
static const u8 sText_Elm[] = _("Elm");
static const u8 sText_Oak[] = _("Oak");
static const u8 sText_Unknown[] = _("???");
static const u8 sText_Empty[] = _("");
static const u8 sText_Male[] = _("Professor (M)");
static const u8 sText_Female[] = _("Professor (F)");
static const u8 sText_NewName[] = _("New name");
static const u8 sText_RivalName[] = _("Elm");
static const u8 sNameChoice_Maple[] = _("Maple");
static const u8 sNameChoice_Cedar[] = _("Cedar");
static const u8 sNameChoice_Sequoia[] = _("Sequoia"); // was Sumac (#94)
static const u8 *const sNameChoices[] = {sNameChoice_Maple, sNameChoice_Cedar, sNameChoice_Sequoia};

// Speakers that light up a video or avatar
enum
{
    TALK_NONE,
    TALK_OAK,
    TALK_PLAYER,
    TALK_SLOWPOKE,
    TALK_ELM,
    TALK_JENNY,
    TALK_IVY,
};

static const u8 sPrefix_Oak[] = _("Oak:");
static const u8 sPrefix_Slowpoke[] = _("Slowpoke:");
static const u8 sPrefix_Elm[] = _("Elm:");
static const u8 sPrefix_Jenny[] = _("Jenny:");
static const u8 sPrefix_Ivy[] = _("Ivy:");
static const u8 sPrefix_Unknown[] = _("???:");

// One call (#13): what is on screen when it starts, and its steps
struct CallDef
{
    const struct CallStep *script;
    const u8 *title;
    const u8 *hostName;     // label under the left video
    const u32 *hostGfx;     // 64x64 portrait
    const u16 *hostPal;
    u8 hostTalker;          // TALK_*: whose lines light up the left video
    u8 circles;             // avatars already in the call (bit per CIRCLE_*)
    bool8 fromField;        // TRUE: everyone is already in; back to the map after
};

static const struct CallDef sCalls[RR_CALL_COUNT] =
{
    [RR_CALL_INTRO] = {sCallScript, sText_CallTitle, sText_Oak, sVideo_Oak_Gfx, sVideo_Oak_Pal, TALK_OAK, 0, FALSE},
    [RR_CALL_RULES] = {sRulesCallScript, sText_RulesCallTitle, sText_Elm, sVideo_Elm_Gfx, sVideo_Elm_Pal, TALK_ELM,
                       (1 << CIRCLE_JENNY) | (1 << CIRCLE_IVY), TRUE},
};

static const struct { const u8 *prefix; u8 talker; } sTalkers[] =
{
    {sPrefix_Oak, TALK_OAK},
    {sPrefix_Slowpoke, TALK_SLOWPOKE},
    {sPrefix_Elm, TALK_ELM},
    {sPrefix_Jenny, TALK_JENNY},
    {sPrefix_Ivy, TALK_IVY},
    {sPrefix_Unknown, TALK_PLAYER},
};

// ---------------------------------------------------------------------------
// Windows (BG0) and BGs
// ---------------------------------------------------------------------------
enum
{
    WIN_TEXTBOX, // from InitStandardTextBoxWindows
    WIN_TITLE,
    WIN_LABEL_HOST,
    WIN_LABEL_PLAYER,
};

static const struct WindowTemplate sWindow_Title = {.bg = 0, .tilemapLeft = 2, .tilemapTop = 1, .width = 11, .height = 2, .paletteNum = 15, .baseBlock = 1};
static const struct WindowTemplate sWindow_LabelOak = {.bg = 0, .tilemapLeft = 1, .tilemapTop = 12, .width = 8, .height = 2, .paletteNum = 15, .baseBlock = 23};
static const struct WindowTemplate sWindow_LabelPlayer = {.bg = 0, .tilemapLeft = 16, .tilemapTop = 12, .width = 9, .height = 2, .paletteNum = 15, .baseBlock = 39};
static const struct WindowTemplate sWindow_Gender = {.bg = 0, .tilemapLeft = 17, .tilemapTop = 6, .width = 11, .height = 4, .paletteNum = 15, .baseBlock = 57};
static const struct WindowTemplate sWindow_Names = {.bg = 0, .tilemapLeft = 19, .tilemapTop = 4, .width = 9, .height = 8, .paletteNum = 15, .baseBlock = 101};

static const struct BgTemplate sBgTemplates[] =
{
    {.bg = 0, .charBaseIndex = 2, .mapBaseIndex = 31, .screenSize = 0, .paletteMode = 0, .priority = 0, .baseTile = 0},
    {.bg = 1, .charBaseIndex = 0, .mapBaseIndex = 30, .screenSize = 0, .paletteMode = 0, .priority = 3, .baseTile = 0},
};

static const u8 sLabelColors[3] = {0, 1, 2}; // transparent, white, dark gray

// ---------------------------------------------------------------------------
// Sprites
// ---------------------------------------------------------------------------
// Normal (not double-size) affine: double-size videos would reach up into the
// header and use up the scanline's sprite time, cutting off the avatars there.
static const struct OamData sOam_Video =
{
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .mosaic = TRUE,
    .shape = SPRITE_SHAPE(64x64),
    .size = SPRITE_SIZE(64x64),
    .priority = 1,
};

static const struct OamData sOam_Circle =
{
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .mosaic = TRUE,
    .shape = SPRITE_SHAPE(32x32),
    .size = SPRITE_SIZE(32x32),
    .priority = 1,
};

static const struct OamData sOam_Ring =
{
    .shape = SPRITE_SHAPE(32x32),
    .size = SPRITE_SIZE(32x32),
    .priority = 1,
};

enum
{
    AFFINE_SHOWN,
    AFFINE_POP,
    AFFINE_DROP,
    AFFINE_SHRINK,
};

static const union AffineAnimCmd sAffine_Shown[] =
{
    AFFINEANIMCMD_FRAME(256, 256, 0, 0),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffine_Pop[] =
{
    AFFINEANIMCMD_FRAME(16, 16, 0, 0),
    AFFINEANIMCMD_FRAME(35, 35, 0, 8),  // grow past full size...
    AFFINEANIMCMD_FRAME(-10, -10, 0, 4), // ...and settle
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffine_Drop[] =
{
    AFFINEANIMCMD_FRAME(256, 256, 0, 0),
    AFFINEANIMCMD_FRAME(-25, -25, 0, 10),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffine_Shrink[] =
{
    AFFINEANIMCMD_FRAME(256, 256, 0, 0),
    AFFINEANIMCMD_FRAME(-4, -4, 0, 51),  // down to a dot, slowly
    AFFINEANIMCMD_FRAME(-10, -10, 0, 5),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sAffineAnims[] =
{
    [AFFINE_SHOWN] = sAffine_Shown,
    [AFFINE_POP] = sAffine_Pop,
    [AFFINE_DROP] = sAffine_Drop,
    [AFFINE_SHRINK] = sAffine_Shrink,
};

struct CallResources
{
    const struct CallDef *def;
    u16 step;
    u8 videoSpriteIds[NUM_VIDEOS];
    u8 circleSpriteIds[NUM_CIRCLES];
    u8 ringSpriteIds[NUM_CIRCLES];
    bool8 videoShown[NUM_VIDEOS];
    bool8 playerLeft; // the player has hung up: their label goes blank (#81)
    bool8 circleShown[NUM_CIRCLES];
    bool8 elmZoom;
    bool8 named;
    u8 signal;
    u8 talker;
    u8 fuzz;
    u8 menuWindowId;
    u8 timer;
    u8 state;
};

static EWRAM_DATA struct CallResources *sCall = NULL;

static void Task_Call(u8 taskId);
static void CB2_ReturnFromNamingScreen(void);

static void VBlankCB_Call(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void CB2_Call(void)
{
    RunTasks();
    RunTextPrinters();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

// ---------------------------------------------------------------------------
// Drawing helpers
// ---------------------------------------------------------------------------
static void SetBgColor(u32 index, u16 color)
{
    gPlttBufferUnfaded[BG_PLTT_ID(BG_PAL_SLOT) + index] = color;
    if (!gPaletteFade.active)
        gPlttBufferFaded[BG_PLTT_ID(BG_PAL_SLOT) + index] = color;
}

static void DrawSignal(void)
{
    u32 i;
    for (i = 0; i < 4; i++)
        SetBgColor(PAL_SIGNAL_BAR(i), i < sCall->signal ? COLOR_BAR_ON : COLOR_BAR_OFF);
}

static void PrintLabel(u32 windowId, const u8 *str)
{
    FillWindowPixelBuffer(windowId, PIXEL_FILL(0));
    AddTextPrinterParameterized3(windowId, FONT_SMALL, 2, 2, sLabelColors, 0, str);
    CopyWindowToVram(windowId, COPYWIN_FULL);
}

static void PrintLabels(void)
{
    PrintLabel(WIN_TITLE, sCall->def->title);
    PrintLabel(WIN_LABEL_HOST, sCall->videoShown[VIDEO_HOST] ? sCall->def->hostName : sText_Empty);
    if (sCall->playerLeft)
        PrintLabel(WIN_LABEL_PLAYER, sText_Empty); // left the call: no name under an empty frame (#81)
    else
        PrintLabel(WIN_LABEL_PLAYER, sCall->named ? gSaveBlock2Ptr->playerName : sText_Unknown);
}

// Gold border on whoever is speaking
static void SetTalker(u32 talker)
{
    u32 i;

    sCall->talker = talker;
    SetBgColor(PAL_BORDER_HOST, talker == sCall->def->hostTalker ? COLOR_TALKING : COLOR_BORDER);
    SetBgColor(PAL_BORDER_PLAYER, talker == TALK_PLAYER ? COLOR_TALKING : COLOR_BORDER);
    for (i = 0; i < NUM_CIRCLES; i++)
    {
        bool32 talking = talker == TALK_SLOWPOKE + i;
        gSprites[sCall->ringSpriteIds[i]].oam.paletteNum = IndexOfSpritePaletteTag(talking ? TAG_RING_TALK : TAG_RING_IDLE);
    }
}

static u32 GetTalker(const u8 *text)
{
    u32 i, len;

    for (i = 0; i < ARRAY_COUNT(sTalkers); i++)
    {
        len = StringLength(sTalkers[i].prefix);
        if (StringCompareN(text, sTalkers[i].prefix, len) == 0)
            return sTalkers[i].talker;
    }
    len = StringLength(gSaveBlock2Ptr->playerName);
    if (len != 0 && StringCompareN(text, gSaveBlock2Ptr->playerName, len) == 0 && text[len] == 0xF0) // ':'
        return TALK_PLAYER;
    return TALK_NONE;
}

static void PrintLine(const u8 *text)
{
    StringExpandPlaceholders(gStringVar4, text);
    SetTalker(GetTalker(gStringVar4));
    DrawDialogueFrame(WIN_TEXTBOX, FALSE);
    AddTextPrinterParameterized2(WIN_TEXTBOX, FONT_NORMAL, gStringVar4, GetPlayerTextSpeedDelay(), NULL,
                                 TEXT_COLOR_DARK_GRAY, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY);
    CopyWindowToVram(WIN_TEXTBOX, COPYWIN_FULL);
}

// ---------------------------------------------------------------------------
// Sprites
// ---------------------------------------------------------------------------
static u8 CreateVideoSprite(u32 video, const u32 *gfx, const u16 *pal)
{
    struct SpriteSheet sheet = {.data = gfx, .size = 64 * 64 / 2, .tag = TAG_VIDEO(video)};
    struct SpritePalette palette = {.data = pal, .tag = TAG_VIDEO(video)};
    struct SpriteTemplate template = {
        .tileTag = TAG_VIDEO(video), .paletteTag = TAG_VIDEO(video), .oam = &sOam_Video,
        .anims = gDummySpriteAnimTable, .images = NULL, .affineAnims = sAffineAnims, .callback = SpriteCallbackDummy,
    };
    u8 spriteId;

    FreeSpriteTilesByTag(TAG_VIDEO(video));
    FreeSpritePaletteByTag(TAG_VIDEO(video));
    LoadSpriteSheet(&sheet);
    LoadSpritePalette(&palette);
    spriteId = CreateSprite(&template, sVideoPos[video][0], sVideoPos[video][1], video == VIDEO_SLOWPOKE ? 1 : 2);
    gSprites[spriteId].invisible = TRUE;
    return spriteId;
}

static void CreateCallSprites(void)
{
    static const u32 *const sCircleGfx[NUM_CIRCLES] = {sCircle_Slowpoke_Gfx, sCircle_Elm_Gfx, sCircle_Jenny_Gfx, sCircle_Ivy_Gfx};
    static const u16 *const sCirclePal[NUM_CIRCLES] = {sCircle_Slowpoke_Pal, sCircle_Elm_Pal, sCircle_Jenny_Pal, sCircle_Ivy_Pal};
    struct SpriteSheet ring = {.data = sRing_Gfx, .size = 32 * 32 / 2, .tag = TAG_RING};
    struct SpritePalette ringIdle = {.data = sRingPal_Idle, .tag = TAG_RING_IDLE};
    struct SpritePalette ringTalk = {.data = sRingPal_Talking, .tag = TAG_RING_TALK};
    struct SpriteTemplate ringTemplate = {
        .tileTag = TAG_RING, .paletteTag = TAG_RING_IDLE, .oam = &sOam_Ring,
        .anims = gDummySpriteAnimTable, .images = NULL, .affineAnims = gDummySpriteAffineAnimTable, .callback = SpriteCallbackDummy,
    };
    bool32 male = gSaveBlock2Ptr->playerGender == MALE;
    u32 i;

    sCall->videoSpriteIds[VIDEO_HOST] = CreateVideoSprite(VIDEO_HOST, sCall->def->hostGfx, sCall->def->hostPal);
    sCall->videoSpriteIds[VIDEO_SLOWPOKE] = CreateVideoSprite(VIDEO_SLOWPOKE, sVideo_Slowpoke_Gfx, sVideo_Slowpoke_Pal);
    sCall->videoSpriteIds[VIDEO_PLAYER] = CreateVideoSprite(VIDEO_PLAYER, male ? sVideo_PlayerM_Gfx : sVideo_PlayerF_Gfx,
                                                            male ? sVideo_PlayerM_Pal : sVideo_PlayerF_Pal);
    for (i = 0; i < NUM_VIDEOS; i++)
    {
        if (sCall->videoShown[i])
        {
            gSprites[sCall->videoSpriteIds[i]].invisible = FALSE;
            StartSpriteAffineAnim(&gSprites[sCall->videoSpriteIds[i]], AFFINE_SHOWN);
        }
    }

    LoadSpriteSheet(&ring);
    LoadSpritePalette(&ringIdle);
    LoadSpritePalette(&ringTalk);
    for (i = 0; i < NUM_CIRCLES; i++)
    {
        struct SpriteSheet sheet = {.data = sCircleGfx[i], .size = (i == CIRCLE_ELM ? 2 : 1) * 32 * 32 / 2, .tag = TAG_CIRCLE(i)};
        struct SpritePalette palette = {.data = sCirclePal[i], .tag = TAG_CIRCLE(i)};
        struct SpriteTemplate template = {
            .tileTag = TAG_CIRCLE(i), .paletteTag = TAG_CIRCLE(i), .oam = &sOam_Circle,
            .anims = gDummySpriteAnimTable, .images = NULL, .affineAnims = sAffineAnims, .callback = SpriteCallbackDummy,
        };
        LoadSpriteSheet(&sheet);
        LoadSpritePalette(&palette);
        sCall->circleSpriteIds[i] = CreateSprite(&template, CIRCLE_X(i), CIRCLE_Y, 2);
        sCall->ringSpriteIds[i] = CreateSprite(&ringTemplate, CIRCLE_X(i), CIRCLE_Y, 1);
        gSprites[sCall->circleSpriteIds[i]].invisible = !sCall->circleShown[i];
        gSprites[sCall->ringSpriteIds[i]].invisible = !sCall->circleShown[i];
        if (sCall->circleShown[i])
            StartSpriteAffineAnim(&gSprites[sCall->circleSpriteIds[i]], AFFINE_SHOWN);
    }
    if (sCall->elmZoom)
        gSprites[sCall->circleSpriteIds[CIRCLE_ELM]].oam.tileNum += 16;
}

static void ShowVideo(u32 video)
{
    struct Sprite *sprite = &gSprites[sCall->videoSpriteIds[video]];
    sCall->videoShown[video] = TRUE;
    sprite->invisible = FALSE;
    StartSpriteAffineAnim(sprite, AFFINE_POP);
}

static void SetElmZoom(bool32 zoom)
{
    if (sCall->elmZoom != zoom)
    {
        gSprites[sCall->circleSpriteIds[CIRCLE_ELM]].oam.tileNum += zoom ? 16 : -16;
        sCall->elmZoom = zoom;
    }
}

// Rings follow their avatar once it has finished popping in / until it drops
static void UpdateRings(void)
{
    u32 i;
    // Whoever has left the call: hide them once they've shrunk away
    for (i = 0; i < NUM_VIDEOS; i++)
    {
        struct Sprite *video = &gSprites[sCall->videoSpriteIds[i]];
        if (!sCall->videoShown[i] && video->affineAnimEnded)
            video->invisible = TRUE;
    }
    for (i = 0; i < NUM_CIRCLES; i++)
    {
        struct Sprite *circle = &gSprites[sCall->circleSpriteIds[i]];
        if (!sCall->circleShown[i] && circle->affineAnimEnded)
            circle->invisible = TRUE;
    }
    for (i = 0; i < NUM_CIRCLES; i++)
    {
        struct Sprite *circle = &gSprites[sCall->circleSpriteIds[i]];
        gSprites[sCall->ringSpriteIds[i]].invisible = !sCall->circleShown[i] || circle->invisible || !circle->affineAnimEnded;
    }
}

// ---------------------------------------------------------------------------
// Setup (also used when coming back from the naming screen)
// ---------------------------------------------------------------------------
static bool32 SetUpCallScene(void)
{
    switch (gMain.state)
    {
    case 0:
        SetVBlankCallback(NULL);
        SetHBlankCallback(NULL);
        DmaFill16(3, 0, VRAM, VRAM_SIZE);
        DmaFill32(3, 0, OAM, OAM_SIZE);
        DmaFill16(3, 0, PLTT, PLTT_SIZE);
        ResetPaletteFade();
        ScanlineEffect_Stop();
        ResetSpriteData();
        FreeAllSpritePalettes();
        gReservedSpritePaletteCount = 0; // the call needs 9 sprite palettes
        ResetTasks();
        break;
    case 1:
        SetGpuReg(REG_OFFSET_WIN0H, 0);
        SetGpuReg(REG_OFFSET_WIN0V, 0);
        SetGpuReg(REG_OFFSET_WININ, 0);
        SetGpuReg(REG_OFFSET_WINOUT, 0);
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 0);
        SetGpuReg(REG_OFFSET_MOSAIC, 0);
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sBgTemplates, ARRAY_COUNT(sBgTemplates));
        ChangeBgX(1, 0, BG_COORD_SET);
        ChangeBgY(1, 0, BG_COORD_SET);
        SetBgAttribute(1, BG_ATTR_MOSAIC, 1);
        gSpriteCoordOffsetX = 0;
        gSpriteCoordOffsetY = 0;
        break;
    case 2:
        FreeAllWindowBuffers();
        InitStandardTextBoxWindows();
        InitTextBoxGfxAndPrinters();
        Menu_LoadStdPalAt(BG_PLTT_ID(13));
        AddWindow(&sWindow_Title);
        AddWindow(&sWindow_LabelOak);
        AddWindow(&sWindow_LabelPlayer);
        PutWindowTilemap(WIN_TITLE);
        PutWindowTilemap(WIN_LABEL_HOST);
        PutWindowTilemap(WIN_LABEL_PLAYER);
        break;
    case 3:
        LoadBgTiles(1, sCallBg_Tiles, sizeof(sCallBg_Tiles), 0);
        LoadBgTilemap(1, sCallBg_Tilemap, sizeof(sCallBg_Tilemap), 0);
        LoadPalette(sCallBg_Pal, BG_PLTT_ID(BG_PAL_SLOT), PLTT_SIZE_4BPP);
        LoadPalette(sCallBg_Pal, BG_PLTT_ID(0), PLTT_SIZEOF(1)); // backdrop
        break;
    case 4:
        CreateCallSprites();
        DrawSignal();
        SetTalker(TALK_NONE);
        PrintLabels();
        CopyBgTilemapBufferToVram(0);
        break;
    case 5:
        BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0 | DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
        ShowBg(0);
        ShowBg(1);
        SetVBlankCallback(VBlankCB_Call);
        gTextFlags.canABSpeedUpPrint = TRUE;
        gMain.state = 0;
        return TRUE;
    }
    gMain.state++;
    return FALSE;
}

static void CB2_SetUpCall(void)
{
    if (SetUpCallScene())
    {
        CreateTask(Task_Call, 0);
        SetMainCallback2(CB2_Call);
    }
}

// #12: the second line of the version stamp ("v0.0.19 test build"), for the
// title screen and the demo notice, so the stamp stays the one place to bump
const u8 *RR_GetVersionLine(void)
{
    const u8 *str = NULL;
    u32 i;

    for (i = 0; sCallScript[i].type != STEP_END; i++)
    {
        if (sCallScript[i].type == STEP_LINE)
        {
            str = sCallScript[i].text;
            break;
        }
    }
    gStringVar3[0] = EOS;
    if (str == NULL)
        return gStringVar3;
    while (*str != CHAR_NEWLINE && *str != EOS)
        str++;
    if (*str == CHAR_NEWLINE)
        str++;
    // copy up to the page break
    for (i = 0; str[i] != EOS && str[i] != CHAR_PROMPT_CLEAR && i < 40; i++)
        gStringVar3[i] = str[i];
    gStringVar3[i] = EOS;
    return gStringVar3;
}

void RR_StartIntroCall(void)
{
    sCall = AllocZeroed(sizeof(*sCall));
    sCall->def = &sCalls[RR_CALL_INTRO];
    sCall->signal = 2;
    sCall->videoShown[VIDEO_HOST] = TRUE;
    PlayBGM(MUS_NEW_GAME_INTRO);
    gMain.state = 0;
    SetMainCallback2(CB2_SetUpCall);
}

// #13: a later call from a script. VAR_0x8004 = RR_CALL_*; call it after
// fadescreen FADE_TO_BLACK and follow it with waitstate. Everyone in the call
// is already there; the map comes back (and the script goes on) afterwards.
void RR_ScriptStartCall(struct ScriptContext *ctx)
{
    u32 i;

    sCall = AllocZeroed(sizeof(*sCall));
    sCall->def = &sCalls[gSpecialVar_0x8004 < RR_CALL_COUNT ? gSpecialVar_0x8004 : RR_CALL_RULES];
    sCall->signal = 4;
    sCall->named = TRUE;
    sCall->videoShown[VIDEO_HOST] = TRUE;
    sCall->videoShown[VIDEO_PLAYER] = TRUE;
    for (i = 0; i < NUM_CIRCLES; i++)
        sCall->circleShown[i] = (sCall->def->circles >> i) & 1;
    gMain.state = 0;
    SetMainCallback2(CB2_SetUpCall);
}

static void CB2_ReturnFromNamingScreen(void)
{
    if (SetUpCallScene())
    {
        sCall->named = TRUE;
        PrintLabels();
        CreateTask(Task_Call, 0);
        SetMainCallback2(CB2_Call);
    }
}

// ---------------------------------------------------------------------------
// Menus
// ---------------------------------------------------------------------------
static void ShowMenu(const struct WindowTemplate *template, const u8 *const *options, u32 count)
{
    u32 i;

    sCall->menuWindowId = AddWindow(template);
    DrawStdFrameWithCustomTileAndPalette(sCall->menuWindowId, FALSE, GetStandardFrameBaseTileNum(), 14);
    FillWindowPixelBuffer(sCall->menuWindowId, PIXEL_FILL(1));
    for (i = 0; i < count; i++)
        AddTextPrinterParameterized(sCall->menuWindowId, FONT_NORMAL, options[i], 8, 16 * i + 1, 0, NULL);
    InitMenuNormal(sCall->menuWindowId, FONT_NORMAL, 0, 1, 16, count, 0);
    PutWindowTilemap(sCall->menuWindowId);
    CopyWindowToVram(sCall->menuWindowId, COPYWIN_FULL);
}

static void CloseMenu(void)
{
    ClearStdWindowAndFrameToTransparent(sCall->menuWindowId, TRUE);
    RemoveWindow(sCall->menuWindowId);
}

static void SetName(const u8 *src)
{
    StringCopyN(gSaveBlock2Ptr->playerName, src, PLAYER_NAME_LENGTH);
    gSaveBlock2Ptr->playerName[PLAYER_NAME_LENGTH] = EOS;
}

static void SetRivalName(void)
{
    StringCopyN(gSaveBlock1Ptr->rivalName, sText_RivalName, PLAYER_NAME_LENGTH);
    gSaveBlock1Ptr->rivalName[PLAYER_NAME_LENGTH] = EOS;
}

// ---------------------------------------------------------------------------
// The call
// ---------------------------------------------------------------------------
enum
{
    STATE_NEXT_STEP,
    STATE_WAIT_TEXT,
    STATE_WAIT_TIMER,
    STATE_GENDER_MENU,
    STATE_NAME_MENU,
    STATE_TO_NAMING_SCREEN,
    STATE_FADE_OUT,
};

static void UpdateLiveEffects(void)
{
    // Blinking "live" dot
    SetBgColor(PAL_LIVE_DOT, (gMain.vblankCounter1 >> 4) & 1 ? RGB(28, 7, 6) : RGB(15, 4, 3));
    // Bad signal: shake and pixelate
    if (sCall->fuzz != 0)
    {
        u32 level = sCall->fuzz > 1 ? Random() % 4 : 0;
        sCall->fuzz--;
        gSpriteCoordOffsetX = sCall->fuzz == 0 ? 0 : ((sCall->fuzz & 2) ? 2 : -2);
        ChangeBgX(1, (s16)gSpriteCoordOffsetX * -256, BG_COORD_SET);
        SetGpuReg(REG_OFFSET_MOSAIC, level | (level << 4) | (level << 8) | (level << 12));
    }
    UpdateRings();
}

static void Task_Call(u8 taskId)
{
    const struct CallStep *step;
    s8 input;

    UpdateLiveEffects();
    if (gPaletteFade.active && sCall->state != STATE_FADE_OUT)
        return;

    // Test builds only (the debug menu is on): START skips the new-game call
    // as Maple (male), straight to the bedroom
    if (DEBUG_OVERWORLD_MENU && !sCall->def->fromField && JOY_NEW(START_BUTTON)
     && sCall->state != STATE_FADE_OUT && sCall->state != STATE_TO_NAMING_SCREEN)
    {
        if (sCall->state == STATE_GENDER_MENU || sCall->state == STATE_NAME_MENU)
            CloseMenu();
        gSaveBlock2Ptr->playerGender = MALE;
        SetName(sNameChoices[0]);
        ClearDialogWindowAndFrame(WIN_TEXTBOX, TRUE);
        BeginNormalPaletteFade(PALETTES_ALL, 2, 0, 16, RGB_BLACK);
        sCall->state = STATE_FADE_OUT;
        return;
    }

    switch (sCall->state)
    {
    case STATE_WAIT_TEXT:
        if (!IsTextPrinterActiveOnWindow(WIN_TEXTBOX))
            sCall->state = STATE_NEXT_STEP;
        return;
    case STATE_WAIT_TIMER:
        if (sCall->timer == 0 || --sCall->timer == 0)
            sCall->state = STATE_NEXT_STEP;
        return;
    case STATE_GENDER_MENU:
        input = Menu_ProcessInputNoWrap();
        if (input == 0 || input == 1)
        {
            PlaySE(SE_SELECT);
            gSaveBlock2Ptr->playerGender = input == 0 ? MALE : FEMALE;
            CloseMenu();
            // The player's video connects, with the right picture this time
            DestroySprite(&gSprites[sCall->videoSpriteIds[VIDEO_PLAYER]]);
            sCall->videoSpriteIds[VIDEO_PLAYER] = CreateVideoSprite(VIDEO_PLAYER,
                input == 0 ? sVideo_PlayerM_Gfx : sVideo_PlayerF_Gfx, input == 0 ? sVideo_PlayerM_Pal : sVideo_PlayerF_Pal);
            ShowVideo(VIDEO_PLAYER);
            sCall->timer = 30;
            sCall->state = STATE_WAIT_TIMER;
        }
        return;
    case STATE_NAME_MENU:
        input = Menu_ProcessInput();
        if (input == MENU_NOTHING_CHOSEN || input == MENU_B_PRESSED)
            return;
        PlaySE(SE_SELECT);
        CloseMenu();
        if (input < (s8)ARRAY_COUNT(sNameChoices))
        {
            SetName(sNameChoices[input]);
            sCall->named = TRUE;
            PrintLabels();
            sCall->state = STATE_NEXT_STEP;
        }
        else
        {
            SetName(sNameChoices[0]); // default if the naming screen is left empty
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            sCall->state = STATE_TO_NAMING_SCREEN;
        }
        return;
    case STATE_TO_NAMING_SCREEN:
        sCall->state = STATE_NEXT_STEP;
        DestroyTask(taskId);
        FreeAllWindowBuffers();
        DoNamingScreen(NAMING_SCREEN_PLAYER, gSaveBlock2Ptr->playerName, gSaveBlock2Ptr->playerGender, 0, 0, CB2_ReturnFromNamingScreen);
        return;
    case STATE_FADE_OUT:
        if (!gPaletteFade.active)
        {
            bool32 fromField = sCall->def->fromField;
            if (!fromField)
                SetRivalName();
            FreeAllWindowBuffers();
            Free(sCall);
            sCall = NULL;
            gTextFlags.canABSpeedUpPrint = FALSE;
            DestroyTask(taskId);
            SetMainCallback2(fromField ? CB2_ReturnToFieldContinueScriptPlayMapMusic : CB2_NewGame);
        }
        return;
    }

    // STATE_NEXT_STEP
    step = &sCall->def->script[sCall->step++];
    switch (step->type)
    {
    case STEP_LINE:
        PrintLine(step->text);
        sCall->state = STATE_WAIT_TEXT;
        break;
    case STEP_POP:
        sCall->circleShown[step->arg] = TRUE;
        gSprites[sCall->circleSpriteIds[step->arg]].invisible = FALSE;
        StartSpriteAffineAnim(&gSprites[sCall->circleSpriteIds[step->arg]], AFFINE_POP);
        PlaySE(SE_BALL_TRAY_ENTER);
        break;
    case STEP_DROP:
        sCall->circleShown[step->arg] = FALSE;
        StartSpriteAffineAnim(&gSprites[sCall->circleSpriteIds[step->arg]], AFFINE_DROP);
        PlaySE(SE_BALL_TRAY_EXIT);
        break;
    case STEP_SLOWPOKE:
        ShowVideo(VIDEO_SLOWPOKE);
        break;
    case STEP_ELM_ZOOM:
        SetElmZoom(step->arg);
        break;
    case STEP_SIGNAL:
        sCall->signal = step->arg;
        DrawSignal();
        break;
    case STEP_FUZZ:
        sCall->fuzz = step->arg;
        break;
    case STEP_GENDER:
    {
        static const u8 *const sGenders[] = {sText_Male, sText_Female};
        ShowMenu(&sWindow_Gender, sGenders, ARRAY_COUNT(sGenders));
        sCall->state = STATE_GENDER_MENU;
        break;
    }
    case STEP_NAME:
    {
        static const u8 *const sNames[] = {sNameChoice_Maple, sNameChoice_Cedar, sNameChoice_Sequoia, sText_NewName};
        ShowMenu(&sWindow_Names, sNames, ARRAY_COUNT(sNames));
        sCall->state = STATE_NAME_MENU;
        break;
    }
    case STEP_HOST_LEAVES:
        sCall->videoShown[VIDEO_HOST] = FALSE;
        StartSpriteAffineAnim(&gSprites[sCall->videoSpriteIds[VIDEO_HOST]], AFFINE_DROP);
        PrintLabels();
        PlaySE(SE_BALL_TRAY_EXIT);
        break;
    case STEP_PLAYER_LEAVES:
        sCall->videoShown[VIDEO_PLAYER] = FALSE;
        sCall->playerLeft = TRUE;
        StartSpriteAffineAnim(&gSprites[sCall->videoSpriteIds[VIDEO_PLAYER]], AFFINE_SHRINK);
        PrintLabels();
        PlaySE(SE_WARP_IN);
        FadeOutBGM(4);
        break;
    case STEP_WAIT:
        sCall->timer = step->arg;
        sCall->state = STATE_WAIT_TIMER;
        break;
    case STEP_END:
    default:
        ClearDialogWindowAndFrame(WIN_TEXTBOX, TRUE);
        BeginNormalPaletteFade(PALETTES_ALL, 2, 0, 16, RGB_BLACK);
        sCall->state = STATE_FADE_OUT;
        break;
    }
}
