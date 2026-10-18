// Research Red: the Field Guide (Start menu > Guide).
//
// Shows which wild Pokemon live where the player is standing and how often
// they appear, for the current time of day: one page per kind of encounter
// (grass, surf, each fishing rod, rock smash), Left/Right to flip pages.
// It reads the game's own encounter tables (built from
// data/research_red/encounters/), so the numbers are always the real ones.
#include "global.h"
#include "event_data.h"
#include "main.h"
#include "rr_portrait.h"
#include "menu.h"
#include "pokedex.h"
#include "region_map.h"
#include "rtc.h"
#include "rr_field_guide.h"
#include "script.h"
#include "sound.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "wild_encounter.h"
#include "window.h"
#include "constants/songs.h"

#define HEADER_NONE 0xFFFF // as in wild_encounter.c

// FireRed's fixed slot chances (see ChooseWildMonIndex_* in wild_encounter.c)
static const u8 sLandRates[LAND_WILD_COUNT] = {20, 20, 10, 10, 10, 10, 5, 5, 4, 4, 1, 1};
static const u8 sWaterRates[WATER_WILD_COUNT] = {60, 30, 5, 4, 1};
static const u8 sFishingRates[FISH_WILD_COUNT] = {70, 30, 60, 20, 20, 40, 40, 15, 4, 1};

enum
{
    PAGE_GRASS,
    PAGE_SURF,
    PAGE_OLD_ROD,
    PAGE_GOOD_ROD,
    PAGE_SUPER_ROD,
    PAGE_ROCK_SMASH,
    NUM_PAGES,
};

static const u8 *const sPageNames[NUM_PAGES] =
{
    [PAGE_GRASS] = COMPOUND_STRING("Grass"),
    [PAGE_SURF] = COMPOUND_STRING("Surfing"),
    [PAGE_OLD_ROD] = COMPOUND_STRING("Old Rod"),
    [PAGE_GOOD_ROD] = COMPOUND_STRING("Good Rod"),
    [PAGE_SUPER_ROD] = COMPOUND_STRING("Super Rod"),
    [PAGE_ROCK_SMASH] = COMPOUND_STRING("Rock Smash"),
};

static const u8 *const sTimeNames[TIMES_OF_DAY_COUNT] =
{
    [TIME_MORNING] = COMPOUND_STRING("Morning"),
    [TIME_DAY] = COMPOUND_STRING("Day"),
    [TIME_EVENING] = COMPOUND_STRING("Evening"),
    [TIME_NIGHT] = COMPOUND_STRING("Night"),
};

static const u8 sText_NothingHere[] = _("No wild Pokémon live here.");
static const u8 sText_Controls[] = _("Name, levels, chance.   {DPAD_LEFTRIGHT} Page   {B_BUTTON} Close");
static const u8 sText_Percent[] = _("{STR_VAR_1}%");
static const u8 sText_Levels[] = _("{STR_VAR_1}-{STR_VAR_2}");
static const u8 sText_Level[] = _("{STR_VAR_1}");
static const u8 sText_Caught[] = _("{PKMN}");

static const struct WindowTemplate sGuideWindow =
{
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 1,
    .width = 28,
    .height = 18,
    .paletteNum = 15,
    .baseBlock = 0x008,
};

static const u8 sColors_Normal[3] = {TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY};
static const u8 sColors_Title[3] = {TEXT_COLOR_WHITE, TEXT_COLOR_BLUE, TEXT_COLOR_LIGHT_BLUE};

struct GuideRow
{
    u16 species;
    u8 percent;
    u8 minLevel;
    u8 maxLevel;
};

#define MAX_ROWS 12
#define tWindowId data[0]
#define tPage     data[1]

static const struct WildPokemonInfo *GetPageInfo(u32 page, u32 *firstSlot, u32 *slotCount, const u8 **rates)
{
    u32 headerId = GetCurrentMapWildMonHeaderId();
    // the encounter code expects the current season/time going in
    enum Season season = gLoadedSeason;
    enum TimeOfDay time = GetTimeOfDay();
    const struct WildPokemonInfo *info = NULL;

    if (headerId == HEADER_NONE)
        return NULL;
    switch (page)
    {
    case PAGE_GRASS:
        GetSeasonAndTimeOfDayForEncounters(headerId, WILD_AREA_LAND, &season, &time);
        info = gWildMonHeaders[headerId].encounterTypes[season][time].landMonsInfo;
        *firstSlot = 0, *slotCount = LAND_WILD_COUNT, *rates = sLandRates;
        break;
    case PAGE_SURF:
        GetSeasonAndTimeOfDayForEncounters(headerId, WILD_AREA_WATER, &season, &time);
        info = gWildMonHeaders[headerId].encounterTypes[season][time].waterMonsInfo;
        *firstSlot = 0, *slotCount = WATER_WILD_COUNT, *rates = sWaterRates;
        break;
    case PAGE_ROCK_SMASH:
        GetSeasonAndTimeOfDayForEncounters(headerId, WILD_AREA_ROCKS, &season, &time);
        info = gWildMonHeaders[headerId].encounterTypes[season][time].rockSmashMonsInfo;
        *firstSlot = 0, *slotCount = ROCK_WILD_COUNT, *rates = sWaterRates;
        break;
    default: // the rods share one table: old 0-1, good 2-4, super 5-9
        GetSeasonAndTimeOfDayForEncounters(headerId, WILD_AREA_FISHING, &season, &time);
        info = gWildMonHeaders[headerId].encounterTypes[season][time].fishingMonsInfo;
        *rates = sFishingRates;
        if (page == PAGE_OLD_ROD)
            *firstSlot = 0, *slotCount = 2;
        else if (page == PAGE_GOOD_ROD)
            *firstSlot = 2, *slotCount = 3;
        else
            *firstSlot = 5, *slotCount = 5;
        break;
    }
    return info;
}

static bool32 PageExists(u32 page)
{
    u32 first, count;
    const u8 *rates;
    return GetPageInfo(page, &first, &count, &rates) != NULL;
}

// Same species in several slots become one row; sorted by chance
static u32 BuildRows(u32 page, struct GuideRow *rows)
{
    u32 first, count, i, j, n = 0;
    const u8 *rates;
    const struct WildPokemonInfo *info = GetPageInfo(page, &first, &count, &rates);

    if (info == NULL)
        return 0;
    for (i = first; i < first + count; i++)
    {
        const struct WildPokemon *mon = &info->wildPokemon[i];
        for (j = 0; j < n; j++)
        {
            if (rows[j].species == mon->species)
                break;
        }
        if (j == n)
        {
            rows[n].species = mon->species;
            rows[n].percent = 0;
            rows[n].minLevel = mon->minLevel;
            rows[n].maxLevel = mon->maxLevel;
            n++;
        }
        rows[j].percent += rates[i];
        rows[j].minLevel = min(rows[j].minLevel, mon->minLevel);
        rows[j].maxLevel = max(rows[j].maxLevel, mon->maxLevel);
    }
    for (i = 1; i < n; i++)
    {
        for (j = i; j > 0 && rows[j].percent > rows[j - 1].percent; j--)
        {
            struct GuideRow tmp = rows[j];
            rows[j] = rows[j - 1];
            rows[j - 1] = tmp;
        }
    }
    return n;
}

static void PrintPage(u32 windowId, u32 page)
{
    struct GuideRow rows[MAX_ROWS];
    u32 n = BuildRows(page, rows);
    u32 i, x, y, width;
    enum TimeOfDay time = GetTimeOfDay(); // the clock's time (tables without a night list use the day one)

    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));

    // "Route 1 - Grass (Night)"
    GetMapName(gStringVar1, gMapHeader.regionMapSectionId, 0);
    StringCopy(gStringVar4, gStringVar1);
    StringAppend(gStringVar4, COMPOUND_STRING(" - "));
    StringAppend(gStringVar4, sPageNames[page]);
    StringAppend(gStringVar4, COMPOUND_STRING(" ("));
    StringAppend(gStringVar4, sTimeNames[time]);
    StringAppend(gStringVar4, COMPOUND_STRING(")"));
    AddTextPrinterParameterized3(windowId, FONT_NORMAL, 4, 1, sColors_Title, TEXT_SKIP_DRAW, gStringVar4);

    if (n == 0)
        AddTextPrinterParameterized3(windowId, FONT_NORMAL, 4, 24, sColors_Normal, TEXT_SKIP_DRAW, sText_NothingHere);

    for (i = 0; i < n; i++)
    {
        x = (i % 2) * 112 + 4;
        y = (i / 2) * 18 + 20;
        // a Poké Ball mark on species already caught
        if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(rows[i].species), FLAG_GET_CAUGHT))
            AddTextPrinterParameterized3(windowId, FONT_SMALL, x, y + 2, sColors_Normal, TEXT_SKIP_DRAW, sText_Caught);
        AddTextPrinterParameterized3(windowId, FONT_NORMAL, x + 8, y, sColors_Normal, TEXT_SKIP_DRAW, GetSpeciesName(rows[i].species));
        ConvertIntToDecimalStringN(gStringVar1, rows[i].minLevel, STR_CONV_MODE_LEFT_ALIGN, 3);
        ConvertIntToDecimalStringN(gStringVar2, rows[i].maxLevel, STR_CONV_MODE_LEFT_ALIGN, 3);
        StringExpandPlaceholders(gStringVar4, rows[i].minLevel == rows[i].maxLevel ? sText_Level : sText_Levels);
        AddTextPrinterParameterized3(windowId, FONT_SMALL, x + 64, y + 3, sColors_Normal, TEXT_SKIP_DRAW, gStringVar4);
        ConvertIntToDecimalStringN(gStringVar1, rows[i].percent, STR_CONV_MODE_LEFT_ALIGN, 3);
        StringExpandPlaceholders(gStringVar4, sText_Percent);
        width = GetStringWidth(FONT_NORMAL, gStringVar4, 0);
        AddTextPrinterParameterized3(windowId, FONT_NORMAL, x + 108 - width, y, sColors_Normal, TEXT_SKIP_DRAW, gStringVar4);
    }
    AddTextPrinterParameterized3(windowId, FONT_SMALL, 4, 130, sColors_Normal, TEXT_SKIP_DRAW, sText_Controls);
    CopyWindowToVram(windowId, COPYWIN_GFX);
}

static void Task_FieldGuide(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    s32 page = tPage;
    s32 dir = 0;

    if (JOY_NEW(A_BUTTON | B_BUTTON | START_BUTTON))
    {
        PlaySE(SE_SELECT);
        ClearStdWindowAndFrameToTransparent(tWindowId, TRUE);
        RemoveWindow(tWindowId);
        ScriptContext_Enable();
        DestroyTask(taskId);
        return;
    }
    if (JOY_NEW(DPAD_RIGHT))
        dir = 1;
    else if (JOY_NEW(DPAD_LEFT))
        dir = -1;
    if (dir == 0)
        return;
    do
    {
        page = (page + dir + NUM_PAGES) % NUM_PAGES;
    } while (page != tPage && !PageExists(page));
    if (page != tPage)
    {
        PlaySE(SE_SELECT);
        tPage = page;
        PrintPage(tWindowId, page);
    }
}

// callnative from RR_EventScript_FieldGuide (followed by waitstate)
void RR_ScriptFieldGuide(struct ScriptContext *ctx)
{
    u8 taskId = CreateTask(Task_FieldGuide, 80);
    s16 *data = gTasks[taskId].data;
    u32 page;

    RR_PortraitHide(); // a full-size window: no speaker portrait on top

    tWindowId = AddWindow(&sGuideWindow);
    for (page = 0; page < NUM_PAGES && !PageExists(page); page++)
        ;
    tPage = page < NUM_PAGES ? page : PAGE_GRASS;
    DrawStdFrameWithCustomTileAndPalette(tWindowId, FALSE, GetStandardFrameBaseTileNum(), 14);
    PutWindowTilemap(tWindowId);
    PrintPage(tWindowId, tPage);
    CopyWindowToVram(tWindowId, COPYWIN_FULL);
}
