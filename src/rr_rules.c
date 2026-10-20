// Research Red: Elm's Research Rules (animatic 07, the lunch in Viridian).
//
// A small settings window: Up/Down picks a rule, Left/Right/A changes it,
// B (or "Done") closes. First shown at Daisy's house, then from
// Options > Research Rules (the row leaves Options and opens this on the map).
//   Level caps     Lv. N / off         FLAG_RR_LEVEL_CAPS_OFF (src/caps.c); shows the current cap
//   Trainers       doubles / singles   FLAG_RR_FORCE_DOUBLES
//   Wild Pokémon   singles / doubles   FLAG_RR_FORCE_DOUBLE_WILD
//   Battle style   shift / set         the Options setting
//   Heal after     on / off            FLAG_RR_AUTO_HEAL (src/battle_main.c, #54)
#include "global.h"
#include "caps.h"
#include "event_data.h"
#include "event_scripts.h"
#include "field_fadetransition.h"
#include "overworld.h"
#include "main.h"
#include "rr_portrait.h"
#include "menu.h"
#include "rr_rules.h"
#include "script.h"
#include "sound.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "window.h"
#include "constants/songs.h"

enum
{
    RULE_LEVEL_CAPS,
    RULE_TRAINERS,
    RULE_WILD,
    RULE_BATTLE_STYLE,
    RULE_AUTO_HEAL,
    RULE_DONE,
    NUM_RULES,
};

static const u8 *const sRuleNames[NUM_RULES] =
{
    [RULE_LEVEL_CAPS] = COMPOUND_STRING("Level caps"),
    [RULE_TRAINERS] = COMPOUND_STRING("Trainer battles"),
    [RULE_WILD] = COMPOUND_STRING("Wild battles"),
    [RULE_BATTLE_STYLE] = COMPOUND_STRING("Battle style"),
    [RULE_AUTO_HEAL] = COMPOUND_STRING("Heal after battles"),
    [RULE_DONE] = COMPOUND_STRING("Done"),
};

static const u8 *const sRuleHelp[NUM_RULES] =
{
    [RULE_LEVEL_CAPS] = COMPOUND_STRING("No EXP past the cap. Each badge\nraises it. Low levels catch up."),
    [RULE_TRAINERS] = COMPOUND_STRING("Trainer battles as doubles\n(you and Ivy) or singles."),
    [RULE_WILD] = COMPOUND_STRING("Wild Pokémon one or two at a time."),
    [RULE_BATTLE_STYLE] = COMPOUND_STRING("Shift: switch after a faint.\nSet: no free switch."),
    [RULE_AUTO_HEAL] = COMPOUND_STRING("On: your team is fully healed\nafter every battle you win."),
    [RULE_DONE] = COMPOUND_STRING("The bet's rules, for both teams.\nChange them anytime in Options."),
};

static const u8 sText_Title[] = _("Elm's Research Rules: the bet");
static const u8 sText_On[] = _("On");
static const u8 sText_Off[] = _("Off");
static const u8 sText_CapLevel[] = _("Lv. {STR_VAR_2}");
static const u8 sText_Doubles[] = _("Doubles");
static const u8 sText_Singles[] = _("Singles");
static const u8 sText_Shift[] = _("Shift");
static const u8 sText_Set[] = _("Set");
static const u8 sText_Arrows[] = _("{LEFT_ARROW} {STR_VAR_1} {RIGHT_ARROW}");

static const struct WindowTemplate sRulesWindow =
{
    .bg = 0,
    .tilemapLeft = 2,
    .tilemapTop = 1,
    .width = 26,
    .height = 18,
    .paletteNum = 15,
    .baseBlock = 0x008,
};

static const u8 sColors_Normal[3] = {TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY};
static const u8 sColors_Title[3] = {TEXT_COLOR_WHITE, TEXT_COLOR_BLUE, TEXT_COLOR_LIGHT_BLUE};
static const u8 sColors_Value[3] = {TEXT_COLOR_WHITE, TEXT_COLOR_RED, TEXT_COLOR_LIGHT_RED};

#define tWindowId data[0]
#define tCursor   data[1]
#define tStartState data[2] // the rules when the window opened (#45)

static const u8 *GetRuleValue(u32 rule)
{
    switch (rule)
    {
    case RULE_LEVEL_CAPS:
        if (FlagGet(FLAG_RR_LEVEL_CAPS_OFF))
            return sText_Off;
        ConvertIntToDecimalStringN(gStringVar2, GetCurrentLevelCap(), STR_CONV_MODE_LEFT_ALIGN, 3);
        StringExpandPlaceholders(gStringVar3, sText_CapLevel);
        return gStringVar3;
    case RULE_TRAINERS:
        return FlagGet(FLAG_RR_FORCE_DOUBLES) ? sText_Doubles : sText_Singles;
    case RULE_WILD:
        return FlagGet(FLAG_RR_FORCE_DOUBLE_WILD) ? sText_Doubles : sText_Singles;
    case RULE_BATTLE_STYLE:
        return gSaveBlock2Ptr->optionsBattleStyle == OPTIONS_BATTLE_STYLE_SET ? sText_Set : sText_Shift;
    case RULE_AUTO_HEAL:
        return FlagGet(FLAG_RR_AUTO_HEAL) ? sText_On : sText_Off;
    }
    return NULL;
}

static void ToggleFlag(u32 flag)
{
    if (FlagGet(flag))
        FlagClear(flag);
    else
        FlagSet(flag);
}

// Every rule as one bit, to tell whether anything changed (#45)
static u16 GetRulesState(void)
{
    return FlagGet(FLAG_RR_LEVEL_CAPS_OFF)
         | FlagGet(FLAG_RR_FORCE_DOUBLES) << 1
         | FlagGet(FLAG_RR_FORCE_DOUBLE_WILD) << 2
         | (gSaveBlock2Ptr->optionsBattleStyle & 1) << 3
         | FlagGet(FLAG_RR_AUTO_HEAL) << 4;
}

static void ChangeRule(u32 rule)
{
    switch (rule)
    {
    case RULE_LEVEL_CAPS:
        ToggleFlag(FLAG_RR_LEVEL_CAPS_OFF);
        break;
    case RULE_TRAINERS:
        ToggleFlag(FLAG_RR_FORCE_DOUBLES);
        break;
    case RULE_WILD:
        ToggleFlag(FLAG_RR_FORCE_DOUBLE_WILD);
        break;
    case RULE_BATTLE_STYLE:
        gSaveBlock2Ptr->optionsBattleStyle ^= 1;
        break;
    case RULE_AUTO_HEAL:
        ToggleFlag(FLAG_RR_AUTO_HEAL);
        break;
    }
}

static void PrintRules(u32 windowId, u32 cursor)
{
    u32 i, y, width;

    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
    AddTextPrinterParameterized3(windowId, FONT_NORMAL, 4, 1, sColors_Title, TEXT_SKIP_DRAW, sText_Title);
    for (i = 0; i < NUM_RULES; i++)
    {
        y = 17 + i * 13; // up to 7 rows end above the help text at y 112
        if (i == cursor)
            FillWindowPixelRect(windowId, PIXEL_FILL(3), 2, y - 1, 204, 13);
        AddTextPrinterParameterized3(windowId, FONT_NORMAL, 10, y, sColors_Normal, TEXT_SKIP_DRAW, sRuleNames[i]);
        if (i != RULE_DONE)
        {
            StringCopy(gStringVar1, GetRuleValue(i));
            StringExpandPlaceholders(gStringVar4, sText_Arrows);
            width = GetStringWidth(FONT_NORMAL, gStringVar4, 0);
            AddTextPrinterParameterized3(windowId, FONT_NORMAL, 202 - width, y, i == cursor ? sColors_Value : sColors_Normal, TEXT_SKIP_DRAW, gStringVar4);
        }
    }
    AddTextPrinterParameterized3(windowId, FONT_NORMAL, 4, 112, sColors_Normal, TEXT_SKIP_DRAW, sRuleHelp[cursor]);
    CopyWindowToVram(windowId, COPYWIN_GFX);
}

static void Task_Rules(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (JOY_NEW(B_BUTTON) || (JOY_NEW(A_BUTTON) && tCursor == RULE_DONE))
    {
        PlaySE(SE_SELECT);
        ClearStdWindowAndFrameToTransparent(tWindowId, TRUE);
        RemoveWindow(tWindowId);
        gSpecialVar_Result = GetRulesState() != tStartState; // #45: a change plays the call with Elm and Jenny
        ScriptContext_Enable();
        DestroyTask(taskId);
        return;
    }
    if (JOY_NEW(DPAD_UP))
        tCursor = (tCursor + NUM_RULES - 1) % NUM_RULES;
    else if (JOY_NEW(DPAD_DOWN))
        tCursor = (tCursor + 1) % NUM_RULES;
    else if (JOY_NEW(A_BUTTON | DPAD_LEFT | DPAD_RIGHT) && tCursor != RULE_DONE)
        ChangeRule(tCursor);
    else
        return;
    PlaySE(SE_SELECT);
    PrintRules(tWindowId, tCursor);
}

// callnative from a script (followed by waitstate)
void RR_ScriptResearchRules(struct ScriptContext *ctx)
{
    u8 taskId = CreateTask(Task_Rules, 80);
    s16 *data = gTasks[taskId].data;

    RR_PortraitHide(); // a full-size window: no speaker portrait on top

    tWindowId = AddWindow(&sRulesWindow);
    tCursor = 0;
    tStartState = GetRulesState();
    DrawStdFrameWithCustomTileAndPalette(tWindowId, FALSE, GetStandardFrameBaseTileNum(), 14);
    PutWindowTilemap(tWindowId);
    PrintRules(tWindowId, tCursor);
    CopyWindowToVram(tWindowId, COPYWIN_FULL);
}

// Options > Research Rules: only from the Start menu on the map (not the title
// screen's Options), and only once Elm has explained the rules at Daisy's
bool32 RR_CanOpenRulesFromOptions(void)
{
    return gMain.savedCallback == CB2_ReturnToFieldWithOpenMenu && VarGet(VAR_0x40FE) >= 4;
}

void RR_FieldCB_OpenResearchRules(void)
{
    FadeInFromBlack();
    ScriptContext_SetupScript(RR_EventScript_ResearchRules);
}
