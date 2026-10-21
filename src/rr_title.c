// Research Red (#12): the screens around the start of the game.
//
//  - The creator splash, once per power-on, before FireRed's opening: a
//    little Mareep walks in, its wool gives a spark, and "A fan project by
//    MudrockNGS" fades in. A or Start skips it.
//  - The title screen (replaces FireRed's Charizard screen): one picture
//    drawn by tools/research_red/build_title.py (the logo and Slowpoke in a
//    lab coat on notebook paper), a blinking "Press Start" and the version.
//    The buttons do what FireRed's did: A/Start to the main menu, B+Select+Up
//    to clear the save, B+Select+Left to reset the clock, and after a while
//    it goes back to the opening.
//  - On a new game, before the intro call: a Slowking presents the fan
//    project notice, a spoiler-free welcome to the story and the demo
//    notice. A skips each page.
#include "global.h"
#include "bg.h"
#include "clear_save_data_screen.h"
#include "event_data.h"
#include "gpu_regs.h"
#include "intro.h"
#include "m4a.h"
#include "main.h"
#include "main_menu.h"
#include "malloc.h"
#include "menu.h"
#include "palette.h"
#include "reset_rtc_screen.h"
#include "rr_intro_call.h"
#include "rr_title.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "trainer_pokemon_sprites.h"
#include "window.h"
#include "constants/songs.h"

#define PAPER_COLOR  RGB(31, 30, 27)   // #f8f0d8, the notebook paper
#define TAG_MON_PIC  0x7E10
#define SKIP_KEYS    (A_BUTTON | B_BUTTON | START_BUTTON)

enum { WIN_MAIN, WIN_SMALL };

static const u16 sTitlePal[] = INCBIN_U16("graphics/research_red/title/title.gbapal");
static const u8 sTitleTiles[] = INCBIN_U8("graphics/research_red/title/title.8bpp");

// Text: palette 15. 1 = brown ink, 2 = its soft shadow
static const u16 sTextPal[16] = {
    [0] = PAPER_COLOR, [1] = RGB(11, 8, 6), [2] = RGB(27, 25, 21), [3] = PAPER_COLOR,
};
static const u8 sTextColors[] = {TEXT_COLOR_TRANSPARENT, 1, 2};

static const struct BgTemplate sBgTemplates[] =
{
    // text
    {.bg = 0, .charBaseIndex = 3, .mapBaseIndex = 30, .screenSize = 0, .paletteMode = 0, .priority = 0, .baseTile = 0},
    // the title picture, 256 colours
    {.bg = 3, .charBaseIndex = 0, .mapBaseIndex = 31, .screenSize = 0, .paletteMode = 1, .priority = 3, .baseTile = 0},
};

static EWRAM_DATA struct
{
    MainCallback next;
    u16 *map;
    u16 spriteId;
    u16 timer;
    u8 state;
    u8 page;
    u8 blend;
    bool8 splashShown;
} sRR = {0};

static void VBlankCB_RR(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void CB2_RR(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    RunTextPrinters();
    UpdatePaletteFade();
}

// A clean screen: paper backdrop, the text layer and its windows
static void SetUpScreen(const struct WindowTemplate *windows)
{
    SetVBlankCallback(NULL);
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
    SetGpuReg(REG_OFFSET_BLDY, 0);
    DmaFill16(3, 0, (void *)VRAM, VRAM_SIZE);
    DmaFill32(3, 0, (void *)OAM, OAM_SIZE);
    DmaFill16(3, 0, (void *)PLTT, PLTT_SIZE);
    ResetTasks();
    ResetSpriteData();
    ResetAllPicSprites();
    FreeAllSpritePalettes();
    ResetPaletteFade();
    ResetBgsAndClearDma3BusyFlags(FALSE);
    InitBgsFromTemplates(0, sBgTemplates, ARRAY_COUNT(sBgTemplates));
    InitWindows(windows);
    DeactivateAllTextPrinters();
    LoadPalette(sTextPal, BG_PLTT_ID(15), sizeof(sTextPal));
    gPlttBufferUnfaded[0] = gPlttBufferFaded[0] = PAPER_COLOR;
    sRR.spriteId = SPRITE_NONE;
    sRR.timer = 0;
    sRR.state = 0;
    sRR.page = 0;
    ShowBg(0);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0 | DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON | DISPCNT_BG0_ON);
    BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
    BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
    SetVBlankCallback(VBlankCB_RR);
    SetMainCallback2(CB2_RR);
}

static void PrintCentered(u8 windowId, u8 fontId, const u8 *str, u8 y)
{
    u32 width = GetWindowAttribute(windowId, WINDOW_WIDTH) * 8;
    u32 x = (width - GetStringWidth(fontId, str, 0)) / 2;
    AddTextPrinterParameterized3(windowId, fontId, x, y, sTextColors, TEXT_SKIP_DRAW, str);
}

static void TearDown(void)
{
    if (sRR.spriteId != SPRITE_NONE)
        FreeAndDestroyMonPicSprite(sRR.spriteId);
    sRR.spriteId = SPRITE_NONE;
    TRY_FREE_AND_SET_NULL(sRR.map);
    FreeAllWindowBuffers();
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
}

// --- The creator splash ----------------------------------------------------

static const struct WindowTemplate sSplashWindows[] =
{
    [WIN_MAIN] = {.bg = 0, .tilemapLeft = 2, .tilemapTop = 13, .width = 26, .height = 2, .paletteNum = 15, .baseBlock = 1},
    DUMMY_WIN_TEMPLATE,
};

static const u8 sText_FanProject[] = _("A fan project by MudrockNGS");

#define MAREEP_STOP_X 120

static void Task_Splash(u8 taskId)
{
    struct Sprite *mareep = &gSprites[sRR.spriteId];

    if (sRR.state < 5 && JOY_NEW(SKIP_KEYS))
    {
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        sRR.state = 5;
    }
    sRR.timer++;
    switch (sRR.state)
    {
    case 0: // fade in; Mareep trots in from the right
        if (mareep->x > MAREEP_STOP_X)
        {
            mareep->x--;
            mareep->y2 = ((sRR.timer / 5) % 2) ? -1 : 0;
        }
        else
        {
            mareep->y2 = 0;
            sRR.timer = 0;
            sRR.state++;
        }
        break;
    case 1: // it stops, says hello...
        if (sRR.timer == 20)
            PlayCry_Normal(SPECIES_MAREEP, 0);
        if (sRR.timer == 70)
        {
            sRR.timer = 0;
            sRR.state++;
        }
        break;
    case 2: // ...and its wool sparks
        if (sRR.timer == 1)
            PlaySE(SE_M_THUNDER_WAVE);
        if (sRR.timer % 4 < 2 && sRR.timer < 24)
            BlendPalettes(1 << (16 + mareep->oam.paletteNum), 10, RGB(31, 31, 12));
        else
            BlendPalettes(1 << (16 + mareep->oam.paletteNum), 0, RGB(31, 31, 12));
        if (sRR.timer == 30)
        {
            sRR.timer = 0;
            sRR.blend = 0;
            sRR.state++;
        }
        break;
    case 3: // the credit fades in
        if (sRR.timer % 2 == 0 && sRR.blend < 16)
        {
            sRR.blend++;
            SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(sRR.blend, 16 - sRR.blend));
        }
        if (sRR.blend == 16)
        {
            sRR.timer = 0;
            sRR.state++;
        }
        break;
    case 4:
        if (sRR.timer == 150)
        {
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            sRR.state++;
        }
        break;
    case 5:
        if (!gPaletteFade.active)
        {
            TearDown();
            DestroyTask(taskId);
            SetMainCallback2(sRR.next);
        }
        break;
    }
}

bool32 RR_StartSplashOnce(MainCallback next)
{
    if (sRR.splashShown)
        return FALSE;
    sRR.splashShown = TRUE;
    sRR.next = next;
    SetUpScreen(sSplashWindows);
    FillWindowPixelBuffer(WIN_MAIN, PIXEL_FILL(0));
    PrintCentered(WIN_MAIN, FONT_NORMAL, sText_FanProject, 1);
    PutWindowTilemap(WIN_MAIN);
    CopyWindowToVram(WIN_MAIN, COPYWIN_FULL);
    // the credit starts invisible and is blended in
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BD);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 16));
    sRR.spriteId = CreateMonFrontPicSprite(SPECIES_MAREEP, FALSE, 0, DISPLAY_WIDTH + 40, 72, 0, TAG_MON_PIC);
    gSprites[sRR.spriteId].oam.priority = 0;
    CreateTask(Task_Splash, 0);
    return TRUE;
}

// --- The title screen ------------------------------------------------------

static const struct WindowTemplate sTitleWindows[] =
{
    [WIN_MAIN]  = {.bg = 0, .tilemapLeft = 10, .tilemapTop = 15, .width = 10, .height = 2, .paletteNum = 15, .baseBlock = 1},
    [WIN_SMALL] = {.bg = 0, .tilemapLeft = 15, .tilemapTop = 18, .width = 15, .height = 2, .paletteNum = 15, .baseBlock = 21},
    DUMMY_WIN_TEMPLATE,
};

static const u8 sText_PressStart[] = _("Press Start");

#define TITLE_TIMEOUT 2700   // frames, as in FireRed: then the opening plays again
#define KEYS_DELSAVE   (B_BUTTON | SELECT_BUTTON | DPAD_UP)
#define KEYS_RESET_RTC (B_BUTTON | SELECT_BUTTON | DPAD_LEFT)

enum { TITLE_RUN, TITLE_CRY, TITLE_LEAVE };

static void Task_Title(u8 taskId)
{
    sRR.timer++;
    switch (sRR.state)
    {
    case TITLE_RUN:
        // "Press Start" blinks
        if (sRR.timer % 32 == 0)
            PutWindowTilemap(WIN_MAIN);
        else if (sRR.timer % 32 == 20)
            ClearWindowTilemap(WIN_MAIN);
        CopyBgTilemapBufferToVram(0);
        if (gPaletteFade.active)
            break;
        if (JOY_HELD(KEYS_DELSAVE) == KEYS_DELSAVE)
        {
            sRR.next = CB2_SaveClearScreen_Init;
            sRR.state = TITLE_LEAVE;
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        }
        else if (JOY_HELD(KEYS_RESET_RTC) == KEYS_RESET_RTC && CanResetRTC() == TRUE)
        {
            sRR.next = CB2_InitResetRtcScreen;
            sRR.state = TITLE_LEAVE;
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        }
        else if (JOY_NEW(A_BUTTON | START_BUTTON))
        {
            PutWindowTilemap(WIN_MAIN);
            CopyBgTilemapBufferToVram(0);
            PlayCry_Normal(SPECIES_SLOWPOKE, 0);
            sRR.timer = 0;
            sRR.state = TITLE_CRY;
        }
        else if (sRR.timer >= TITLE_TIMEOUT)
        {
            sRR.next = CB2_InitCopyrightScreenAfterTitleScreen;
            sRR.state = TITLE_LEAVE;
            FadeOutBGM(4);
            BeginNormalPaletteFade(PALETTES_ALL, 3, 0, 16, RGB_BLACK);
        }
        break;
    case TITLE_CRY:
        // Slowpoke answers, a beat late
        if (sRR.timer == 60)
        {
            sRR.next = CB2_InitMainMenu;
            sRR.state = TITLE_LEAVE;
            FadeOutBGM(4);
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_WHITE);
        }
        break;
    case TITLE_LEAVE:
        if (!gPaletteFade.active)
        {
            m4aMPlayAllStop();
            TearDown();
            DestroyTask(taskId);
            SetMainCallback2(sRR.next);
        }
        break;
    }
}

void CB2_RR_InitTitleScreen(void)
{
    u32 i;
    const u8 *version = RR_GetVersionLine();

    SetUpScreen(sTitleWindows);
    // the picture: 600 tiles in reading order, colours 0-223
    LoadPalette(sTitlePal, BG_PLTT_ID(0), 224 * sizeof(u16));
    LoadBgTiles(3, sTitleTiles, sizeof(sTitleTiles), 0);
    sRR.map = AllocZeroed(BG_SCREEN_SIZE);
    for (i = 0; i < 32 * 20; i++)
        sRR.map[i] = (i % 32 < 30) ? (i / 32) * 30 + (i % 32) : 0;
    LoadBgTilemap(3, sRR.map, BG_SCREEN_SIZE, 0);
    ShowBg(3);
    SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_BG3_ON);

    FillWindowPixelBuffer(WIN_MAIN, PIXEL_FILL(0));
    PrintCentered(WIN_MAIN, FONT_NORMAL, sText_PressStart, 1);
    PutWindowTilemap(WIN_MAIN);
    CopyWindowToVram(WIN_MAIN, COPYWIN_FULL);
    FillWindowPixelBuffer(WIN_SMALL, PIXEL_FILL(0));
    AddTextPrinterParameterized3(WIN_SMALL, FONT_SMALL, 15 * 8 - 2 - GetStringWidth(FONT_SMALL, version, 0), 2,
                                 sTextColors, TEXT_SKIP_DRAW, version);
    PutWindowTilemap(WIN_SMALL);
    CopyWindowToVram(WIN_SMALL, COPYWIN_FULL);
    CreateTask(Task_Title, 0);
    m4aSongNumStart(MUS_TITLE);
}

// --- The Slowking notices on a new game ------------------------------------

static const struct WindowTemplate sNoticeWindows[] =
{
    // tall enough for the six-line welcome page
    [WIN_MAIN] = {.bg = 0, .tilemapLeft = 1, .tilemapTop = 9, .width = 28, .height = 11, .paletteNum = 15, .baseBlock = 1},
    DUMMY_WIN_TEMPLATE,
};

enum {
    NOTICE_PAGE_FAN_PROJECT,
    NOTICE_PAGE_WELCOME,
    NOTICE_PAGE_DEMO,
};

static const u8 sText_Notice[] = _(
    "Research Red is a free fan project.\n"
    "It is not affiliated with Nintendo,\n"
    "Creatures, Game Freak or The Pokémon\n"
    "Company, and is not for sale.");
// the story setup in a few cozy, spoiler-free lines (owner-approved wording)
static const u8 sText_Welcome[] = _(
    "You and Ivy, two Pokémon professors,\n"
    "set out across Kanto with Elm and\n"
    "Jenny. There's one big research paper\n"
    "to write, and a friendly race to eight\n"
    "badges. Take your time: catch what you\n"
    "love, take notes, and enjoy the road.");
static const u8 sText_DemoA[] = _("This is the ");
static const u8 sText_DemoB[] = _(".\n"
    "This demo goes as far as Pewter City.\n"
    "Thank you so much for playtesting!");

#define NOTICE_PAGE_FRAMES 360

static void PrintNoticePage(void)
{
    FillWindowPixelBuffer(WIN_MAIN, PIXEL_FILL(0));
    if (sRR.page == NOTICE_PAGE_FAN_PROJECT)
    {
        AddTextPrinterParameterized3(WIN_MAIN, FONT_NORMAL, 4, 2, sTextColors, TEXT_SKIP_DRAW, sText_Notice);
    }
    else if (sRR.page == NOTICE_PAGE_WELCOME)
    {
        // the longest line is 218 pixels wide: start at 2 so it fits the 224
        AddTextPrinterParameterized3(WIN_MAIN, FONT_NORMAL, 2, 2, sTextColors, TEXT_SKIP_DRAW, sText_Welcome);
    }
    else
    {
        StringCopy(gStringVar4, sText_DemoA);
        StringAppend(gStringVar4, RR_GetVersionLine());
        StringAppend(gStringVar4, sText_DemoB);
        AddTextPrinterParameterized3(WIN_MAIN, FONT_NORMAL, 4, 2, sTextColors, TEXT_SKIP_DRAW, gStringVar4);
    }
    PutWindowTilemap(WIN_MAIN);
    CopyWindowToVram(WIN_MAIN, COPYWIN_FULL);
    sRR.timer = 0;
}

static void Task_Notice(u8 taskId)
{
    sRR.timer++;
    switch (sRR.state)
    {
    case 0:
        if (sRR.timer == 30)
            PlayCry_Normal(SPECIES_SLOWKING, 0);
        if (gPaletteFade.active || (sRR.timer < NOTICE_PAGE_FRAMES && !JOY_NEW(SKIP_KEYS)))
            break;
        PlaySE(SE_SELECT);
        if (sRR.page != NOTICE_PAGE_DEMO)
        {
            sRR.page++;
            PrintNoticePage();
        }
        else
        {
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            sRR.state++;
        }
        break;
    case 1:
        if (!gPaletteFade.active)
        {
            TearDown();
            DestroyTask(taskId);
            RR_StartIntroCall();
        }
        break;
    }
}

void RR_StartNewGameNotice(void)
{
    SetUpScreen(sNoticeWindows);
    PrintNoticePage();
    sRR.spriteId = CreateMonFrontPicSprite(SPECIES_SLOWKING, FALSE, 0, DISPLAY_WIDTH / 2, 38, 0, TAG_MON_PIC);
    CreateTask(Task_Notice, 0);
}
