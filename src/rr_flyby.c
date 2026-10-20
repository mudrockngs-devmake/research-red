// Research Red: the Route 1 fly-by. Something streaks across the sky, too fast
// to see properly: Mewtwo's sprite, stretched sideways and see-through, with two
// fainter copies trailing behind it for a motion blur. It crosses from the lower
// right to the upper left, like the movie (#30). Used while the camera is
// panned up in RR_Route1_EventScript_Walk (callnative + waitstate).
#include "global.h"
#include "gpu_regs.h"
#include "rr_flyby.h"
#include "rr_portrait.h"
#include "script.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "constants/songs.h"

#define TAG_RR_FLYBY  0x5A20
#define NUM_COPIES    3      // the sprite and two trailing copies
#define FLYBY_START_Y 92     // enters at the right, low
#define FLYBY_SPEED   8      // pixels left per frame
#define FLYBY_RISE    2      // pixels up per frame
#define COPY_GAP      18     // pixels between the copies

static const u32 sFlyby_Gfx[] = INCBIN_U32("graphics/pokemon/mewtwo/anim_front.4bpp");
static const u16 sFlyby_Pal[] = INCBIN_U16("graphics/pokemon/mewtwo/normal.gbapal");

static const struct OamData sOam_Flyby =
{
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_BLEND,
    .shape = SPRITE_SHAPE(64x64),
    .size = SPRITE_SIZE(64x64),
    .priority = 1,
};

// Stretched long and thin: a streak, not a clear look. Copies get thinner.
// Negative x scale mirrors it so it faces left, the way it flies.
static const union AffineAnimCmd sAffine_Streak0[] = { AFFINEANIMCMD_FRAME(-400, 200, 0, 0), AFFINEANIMCMD_END };
static const union AffineAnimCmd sAffine_Streak1[] = { AFFINEANIMCMD_FRAME(-460, 160, 0, 0), AFFINEANIMCMD_END };
static const union AffineAnimCmd sAffine_Streak2[] = { AFFINEANIMCMD_FRAME(-512, 110, 0, 0), AFFINEANIMCMD_END };
static const union AffineAnimCmd *const sAffineAnims_Flyby[] = { sAffine_Streak0, sAffine_Streak1, sAffine_Streak2 };

static const struct SpriteTemplate sSpriteTemplate_Flyby =
{
    .tileTag = TAG_RR_FLYBY,
    .paletteTag = TAG_RR_FLYBY,
    .oam = &sOam_Flyby,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_Flyby,
    .callback = SpriteCallbackDummy,
};

#define tTimer      data[0]
#define tOldBldCnt  data[1]
#define tOldAlpha   data[2]
#define tSpriteId(i) data[3 + (i)]

static void Task_Flyby(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u32 i;
    bool32 onScreen = FALSE;

    tTimer++;
    for (i = 0; i < NUM_COPIES; i++)
    {
        struct Sprite *sprite = &gSprites[tSpriteId(i)];
        sprite->x -= FLYBY_SPEED;
        sprite->y -= FLYBY_RISE;
        sprite->y2 = (tTimer / 3) % 2; // a little wobble
        if (sprite->x > -64 && sprite->y > -64)
            onScreen = TRUE;
    }
    if (onScreen)
        return;

    for (i = 0; i < NUM_COPIES; i++)
        DestroySpriteAndFreeResources(&gSprites[tSpriteId(i)]);
    SetGpuReg(REG_OFFSET_BLDCNT, tOldBldCnt);
    SetGpuReg(REG_OFFSET_BLDALPHA, tOldAlpha);
    DestroyTask(taskId);
    ScriptContext_Enable();
}

void RR_ScriptMewtwoFlyby(struct ScriptContext *ctx)
{
    struct SpriteSheet sheet = {.data = sFlyby_Gfx, .size = 64 * 64 / 2, .tag = TAG_RR_FLYBY};
    struct SpritePalette palette = {.data = sFlyby_Pal, .tag = TAG_RR_FLYBY};
    u8 taskId = CreateTask(Task_Flyby, 80);
    s16 *data = gTasks[taskId].data;
    u32 i;

    LoadSpriteSheet(&sheet);
    RR_LoadInterfaceSpritePalette(&palette);
    for (i = 0; i < NUM_COPIES; i++)
    {
        // the copies trail behind: further right and lower
        u8 spriteId = CreateSprite(&sSpriteTemplate_Flyby, DISPLAY_WIDTH + 64 + i * COPY_GAP,
                                   FLYBY_START_Y + i * COPY_GAP * FLYBY_RISE / FLYBY_SPEED, 2 + i);
        StartSpriteAffineAnim(&gSprites[spriteId], i);
        tSpriteId(i) = spriteId;
    }
    // See-through: the sky shows through the streak
    tOldBldCnt = GetGpuReg(REG_OFFSET_BLDCNT);
    tOldAlpha = GetGpuReg(REG_OFFSET_BLDALPHA);
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_BG_ALL | BLDCNT_TGT2_BD);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(8, 10));
    PlaySE(SE_M_FLY);
}
