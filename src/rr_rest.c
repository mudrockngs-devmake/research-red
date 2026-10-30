// Research Red: Rest at Pokemon Centers and at home (#56).
//
// Nurse Joy and Delia offer Heal, or Rest: Rest heals the team too and moves
// the clock forward, during the day to 8 PM (night) and at night to 7 AM
// (morning). The clock only ever moves forward, so berries and other timed
// things just see time passing. Night is 8 PM to 6 AM all year
// (OW_TIMES_OF_DAY in include/config/overworld.h).
// Scripts: data/scripts/research_red/rr_rest.inc.
#include "global.h"
#include "event_data.h"
#include "overworld.h"
#include "rtc.h"
#include "script.h"
#include "constants/rtc.h"

#define RR_REST_NIGHT_HOUR   20
#define RR_REST_MORNING_HOUR 7

// VAR_RESULT = TRUE at night
void RR_ScriptIsNight(struct ScriptContext *ctx)
{
    gSpecialVar_Result = GetTimeOfDay() == TIME_NIGHT;
}

// Moves the clock on to night (day) or the next morning (night).
// VAR_RESULT = TRUE if it is now night.
void RR_ScriptRest(struct ScriptContext *ctx)
{
    s32 days, hours;
    bool32 toNight;

    RtcCalcLocalTime();
    days = gLocalTime.days;
    toNight = GetTimeOfDay() != TIME_NIGHT;
    if (toNight)
    {
        hours = RR_REST_NIGHT_HOUR;
    }
    else
    {
        if (gLocalTime.hours >= RR_REST_MORNING_HOUR)
            days++; // late evening: morning is tomorrow
        hours = RR_REST_MORNING_HOUR;
    }
    RtcCalcLocalTimeOffset(days, hours, 0, 0);
    UpdateTimeOfDay();
    gSpecialVar_Result = toNight;
}
