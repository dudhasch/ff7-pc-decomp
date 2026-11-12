// src/ff7/src/battle/battle_fx_limit_stardust_ray.cpp
// Limit Break [38] (Red XIII Lv 3-2): Stardust Ray (Japanese: スターダストレイ, dev tag: "limit3/s_d" / SD)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: SD limit break effect: a draw-list selector, a timeline with a moving model, textured
//   quads, a growing model, spoke and sprite particle pools, and sprite bursts.

// clang-format off
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "lib/codegen.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <stdlib.h>
#include <string.h>
#include "lib/api/sound.h"
#include "ff7/types/battle_limit.h"
extern "C" {

// clang-format on

#pragma intrinsic(memset)
#pragma function(memcpy)
// clang-format off
// GLOBAL: FF7 0x0082cc08
int g_BattleLimitStardustRayResourceDef = 79929;
int g_BattleLimitStardustRayResourceDefHeadPtr[1] = {
    (int)((char *)&g_BattleFxLimitStardustRayResourceNames + 0x0)
};
int g_BattleLimitStardustRayResourceDefDataTail[34] = {
    0x0, 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxLimitStardustRayResourceNames + 0xc), (int)((char *)&g_BattleFxLimitStardustRayResourceNames + 0x18),
    0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxLimitStardustRayResourceNames + 0x24), (int)((char *)&g_BattleFxLimitStardustRayResourceNames + 0x30), 0x0,
    0x0, 0x192a39, (int)((char *)&g_BattleFxLimitStardustRayResourceNames + 0x3c), (int)((char *)&g_BattleFxLimitStardustRayResourceNames + 0x48), 0x0, 0x0,
    0x1900b1, (int)((char *)&g_BattleFxLimitStardustRayResourceNames + 0x54), 0x0, 0x0, 0x0, 0x3200b1,
    (int)((char *)&g_BattleFxLimitStardustRayResourceNames + 0x60), 0x0, 0x0, 0x0, 0x192a39, 0x0,
    (int)((char *)&g_BattleFxLimitStardustRayResourceNames + 0x68), 0x0, 0x0, 0x0
};
// The reference velocity BattleFx_LimitStardustRayDrawMovingModelTask rotates: one unit up (-Y).
// GLOBAL: FF7 0x0082cc98
VECTOR g_BattleLimitStardustRayModelReq1 = {0, -0x1000, 0, 0};
// GLOBAL: FF7 0x0082cca8
int g_BattleLimitStardustRayRandomTable[256] = {
    26601, 18277, 16324, 73, 20121, 8909, 19573, 21415,
    6178, 7249, 3147, 8156, 22087, 4860, 11615, 23453,
    29673, 28710, 21642, 18484, 1149, 24688, 10419, 26839,
    7726, 8318, 18137, 5061, 10961, 22899, 27359, 31014,
    23437, 18734, 22810, 15264, 11078, 12870, 21660, 30393,
    7118, 20422, 11839, 20207, 6602, 18600, 14448, 26521,
    13158, 24339, 11201, 23164, 3512, 22356, 18476, 3372,
    17653, 21150, 16168, 41, 9046, 6530, 12910, 29621,
    23240, 4652, 21384, 32374, 11606, 24163, 28702, 29136,
    12375, 23870, 30464, 10240, 5719, 3239, 16883, 6137,
    11206, 1681, 12347, 19199, 29283, 5625, 24044, 4100,
    1894, 8094, 1009, 2245, 16496, 23167, 19673, 27812,
    11059, 5145, 10853, 7492, 23521, 31837, 30674, 26854,
    13909, 13683, 29926, 5767, 7684, 9009, 26045, 6069,
    14498, 13914, 25425, 31797, 12180, 12182, 10442, 854,
    7192, 5685, 27019, 26387, 2614, 24228, 31737, 15596,
    15974, 31915, 25865, 2177, 10750, 23916, 4240, 23539,
    14946, 1308, 16970, 8698, 28394, 19582, 2983, 28102,
    24977, 31268, 21081, 27799, 11875, 13157, 13215, 21787,
    26021, 285, 20558, 28811, 29889, 25639, 19876, 6532,
    12792, 13723, 5836, 19622, 30406, 20168, 11057, 9966,
    29972, 13552, 1411, 28116, 13088, 17606, 26506, 32036,
    23085, 6568, 5805, 13980, 17386, 11420, 21825, 20298,
    29346, 26380, 9363, 5796, 24617, 9538, 7092, 4964,
    20354, 25761, 24327, 4650, 3409, 6569, 1163, 463,
    19204, 27048, 28905, 14730, 191, 17473, 31037, 1221,
    523, 6302, 21924, 4285, 12607, 8821, 12426, 6108,
    32170, 2748, 24240, 26326, 6278, 22571, 3072, 6021,
    3228, 27510, 26897, 7557, 9398, 839, 9592, 143,
    19657, 1021, 17108, 24726, 23007, 32551, 3224, 8100,
    25798, 14014, 16535, 19825, 9082, 6182, 13393, 12488,
    27987, 27363, 767, 2714, 21999, 12059, 4192, 28383
};
// GLOBAL: FF7 0x0082d0a8
BattleFxSpriteRequest g_BattleLimitStardustRaySpriteReq2 = {0, 0, -128, -128, -128, 0x2e, 0, 0};
// GLOBAL: FF7 0x0082d0b8
BattleFxSpriteRequest g_BattleLimitStardustRayParticleReq3 = {0, 0, -128, -128, -128, 0x2e, 0, 0};
unsigned char g_rawData_0082d0c8[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x2e, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0082d0d8
BattleFxSpriteRequest g_BattleLimitStardustRayParticleReq4 = {0, 0, -128, -128, -128, 0x2e, 0, 0};
unsigned char g_rawData_0082d0e8[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x2e, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0082d0f8
BattleFxSpriteRequest g_BattleLimitStardustRayParticleReq5 = {0, 0, -128, -128, -128, 0x2e, 0, 0};
unsigned char g_BattleFxLimitStardustRayResourceNames[116] =
    "STER1.TIM\0\0\0"
    "HOSI.TIM\0\0\0\0"
    "HOSI.SPT\0\0\0\0"
    "HOSI1.TIM\0\0\0"
    "HOSI1.SPT\0\0\0"
    "SNOW.TIM\0\0\0\0"
    "SNOW.SPT\0\0\0\0"
    "KOU1.rsd\0\0\0\0"
    "WA.rsd\0\0"
    "jo_c04.spt";
// GLOBAL: FF7 0x00b0c738
int g_BattleLimitStardustRayRandomIndex = 0;
// GLOBAL: FF7 0x00b0c73c
BattleTaskSlot *g_BattleLimitStardustRayDrawListTask = 0;
// GLOBAL: FF7 0x00b0c740
struct BattleFxLimitStardustRayParticle g_BattleLimitStardustRaySpokePool[180] = {0};
// The pool of the sprite particles, read by BattleFx_LimitStardustRaySpriteParticlePool120Task.
// GLOBAL: FF7 0x00b0d820
struct BattleFxLimitStardustRayParticle g_BattleLimitStardustRayParticlePool1[120] = {0};
// The pool of the second sprite particles, read by BattleFx_LimitStardustRaySpriteEntry2ParticlePool120Task.
// GLOBAL: FF7 0x00b0e360
struct BattleFxLimitStardustRayParticle g_BattleLimitStardustRayParticlePool2[120] = {0};
// The pool of the textured quads, read by BattleFx_LimitStardustRayTexturedQuadPool30Task.
// GLOBAL: FF7 0x00b0eea0
struct BattleFxLimitStardustRayQuadRecord g_BattleLimitStardustRayQuadPool[30] = {0};
// GLOBAL: FF7 0x00b0f170
int g_BattleLimitStardustRayDrawBuffer1 = 0;
unsigned char g_bssPad_00b0f174[4] = {0};
unsigned char g_bssPad_00b0f178[81912] = {0};
// GLOBAL: FF7 0x00b23170
int g_BattleLimitStardustRayDrawBuffer2 = 0;
unsigned char g_bssPad_00b23174[4] = {0};
unsigned char g_bssPad_00b23178[81912] = {0};
// GLOBAL: FF7 0x00b37170
int g_BattleLimitStardustRayDrawListHead = 0;
// GLOBAL: FF7 0x00b37174
int g_BattleLimitStardustRayTargetActorMask = 0;
// GLOBAL: FF7 0x00b37178
struct GfxTableCursor *g_BattleLimitStardustRayModelTable = 0;
unsigned char g_bssPad_00b3717c[4] = {0};
// clang-format on

#pragma data_seg(".data$u4c_orbit")
#pragma bss_seg(".data$u4c_orbit")
#pragma data_seg(".bss$u4c_orbit_slot")
#pragma bss_seg(".bss$u4c_orbit_slot")
#pragma data_seg(".bss$u4c_orbit_flag")
#pragma bss_seg(".bss$u4c_orbit_flag")
#pragma data_seg()
#pragma bss_seg()

/*
 * If the battle scene exists and the SD model table is not loaded yet, loads limit3/s_d into
 * g_BattleLimitStardustRayModelTable as model table kind 7, from the g_BattleLimitStardustRayResourceDef list.
 */
// FUNCTION: FF7 0x004c8e90
void __cdecl BattleFx_LimitLoadResStardustRay(void)
{
    GfxModelTableBank *drawDev;
    GfxResourceRequest recordBuf;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleLimitStardustRayModelTable == 0) {
        kind = 7;
        resourceDefsWord = (int)&g_BattleLimitStardustRayResourceDef;
        drawDev = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &recordBuf;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0082d17c
        pRequest->resourcePath = BattleFx_BuildResourcePath("limit3/s_d", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleLimitStardustRayModelTable, drawDev);
    }
}

/*
 * Calls BattleFx_LimitLoadResStardustRay, then BattleFx_LimitBeginStardustRay with both arguments, and returns zero.
 */
// FUNCTION: FF7 0x004c8f20
int __cdecl BattleFx_LimitEnterStardustRay(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResStardustRay();
    BattleFx_LimitBeginStardustRay(targetMask, actorIdx);
    return 0;
}

/*
 * Starts the draw-list selector task (kept in g_BattleLimitStardustRayDrawListTask) and the spawn task, and
 * keeps targetMask in g_BattleLimitStardustRayTargetActorMask; sourceActor is unused.
 */
// FUNCTION: FF7 0x004c8f3c
void __cdecl BattleFx_LimitBeginStardustRay(int targetMask, int actorIdx)
{
    g_BattleLimitStardustRayDrawListTask = BATTLE_NEW_TASK_SLOT(BattleFx_LimitStardustRaySelectDrawListBufferTask);
    Battle_AllocateResourceSlot(BattleFx_LimitStardustRaySpawnTimeline);
    g_BattleLimitStardustRayTargetActorMask = targetMask;
}

/*
 * Points g_BattleLimitStardustRayDrawListHead at the draw buffer of this frame's parity and, once at most one
 * secondary task is left, frees its own task slot (ownerSlot and frameCounter both -1).
 */
// FUNCTION: FF7 0x004c8f74
void __cdecl BattleFx_LimitStardustRaySelectDrawListBufferTask(void)
{
    int *rec;

    if (g_BattleDrawFrameParity == 0) {
        rec = &g_BattleLimitStardustRayDrawBuffer1;
    } else {
        rec = &g_BattleLimitStardustRayDrawBuffer2;
    }
    g_BattleLimitStardustRayDrawListHead = (int)rec;
    if (g_BattleSecondaryTaskActiveCount <= 1) {
        *(int *)g_BattleLimitStardustRayDrawListTask = 0xFFFFFFFF;
    }
}

/*
 * Retires the current slot, allocates BattleFx_LimitStardustRayTimelineTask at the shared actor-mask centroid and sets posY
 * -4320, jitterRadius 0x900 and sequenceIndex zero. Clears the active (or state) word of every record in the four
 * pools, of 180, 120, 120 and thirty records, seeds g_BattleLimitStardustRayRandomIndex with rand() % 256 and
 * plays sound 0x128 panned from the position.
 */
// FUNCTION: FF7 0x004c8fb5
void __cdecl BattleFx_LimitStardustRaySpawnTimeline(void)
{
    int i;
    BattleFxStardustRaySpawnRecord *task;

    task = (BattleFxStardustRaySpawnRecord *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    task = (BattleFxStardustRaySpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitStardustRayTimelineTask);
    Battle_GetActorMaskCentroid(g_BattleLimitStardustRayTargetActorMask, &task->posX);
    task->posY = 0xEF20;
    task->jitterRadius = 2304;
    task->sequenceIndex = 0;
    for (i = 0; i < 180; i++) {
        g_BattleLimitStardustRaySpokePool[i].active = 0;
    }
    for (i = 0; i < 120; i++) {
        g_BattleLimitStardustRayParticlePool1[i].active = 0;
    }
    for (i = 0; i < 120; i++) {
        g_BattleLimitStardustRayParticlePool2[i].active = 0;
    }
    for (i = 0; i < 30; i++) {
        g_BattleLimitStardustRayQuadPool[i].state = 0;
    }
    g_BattleLimitStardustRayRandomIndex = rand() % 256;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition(&task->pos), 0x128);
}

/*
 * When unpaused, schedules the seeded effect at eight, three particle systems and sound at
 * thirty-one, a view-space model at fifty-four and randomized ground models on odd counts forty
 * through sixty. Ramps shared intensity and view flag bits, dispatches sequential actor ring/bone
 * callbacks every fourth count from thirty-eight unless the table entry is 0xFF, then from eighty
 * clears flash/intensity, retires and stops sound before advancing.
 */
// FUNCTION: FF7 0x004c9104
void __cdecl BattleFx_LimitStardustRayTimelineTask(void)
{
    unsigned short level;
    BattleFxPoint3s *ringRec;
    int targetActor;
    int extent;
    BattleFxLimitStardustRayTimeline *selfPtr;
    BattleFxLimitStardustRayChildTask *child;
    int span;

    selfPtr = (BattleFxLimitStardustRayTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (selfPtr->phase == 0x1F) {
        child = (BattleFxLimitStardustRayChildTask *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitStardustRaySpokeParticlePool180Task);
        child->pos = selfPtr->pos;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH2, Battle_ComputeScreenPanFromPosition(&selfPtr->pos), 0x265);
    }
    if (selfPtr->phase == 0x1F) {
        child = (BattleFxLimitStardustRayChildTask *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitStardustRaySpriteParticlePool120Task);
        child->pos = selfPtr->pos;
    }
    if (selfPtr->phase == 0x1F) {
        child = (BattleFxLimitStardustRayChildTask *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitStardustRaySpriteEntry2ParticlePool120Task);
        child->pos = selfPtr->pos;
    }
    if (selfPtr->phase == 8) {
        child = (BattleFxLimitStardustRayChildTask *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitStardustRayTexturedQuadPool30Task);
        child->pos = selfPtr->pos;
    }
    if (selfPtr->phase == 0x36) {
        child = (BattleFxLimitStardustRayChildTask *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitStardustRayDrawMovingModelTask);
        child->posX = 170;
        child->posY = 0xFFB0;
        child->rotZ = 0xFB2E;
        child->radius = 655;
        child->scale = 0x10;
    }
    if (selfPtr->phase >= 40 && selfPtr->phase <= 60 && (selfPtr->phase & 1) != 0) {
        extent = selfPtr->spreadRadius;
        span = extent << 1;
        child = (BattleFxLimitStardustRayChildTask *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitStardustRayDrawGrowingModelTask);
        child->posX = selfPtr->posX + rand() % span - extent;
        child->posY = 0;
        child->posZ = selfPtr->posZ + rand() % span - extent;
        extent = 0x1174;
        span = rand() % 2560 + 2048;
        child->radius = 0;
        child->scale = (extent * span >> 12) / 6;
    }
    if (selfPtr->phase < 8) {
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        level = selfPtr->phase << 9;
    } else if (selfPtr->phase >= 72) {
        g_BattleStageHideFlags &= 0xF8;
        level = 0x1000 - (selfPtr->phase - 0x48 << 9);
    } else {
        g_BattleStageHideFlags |= 7;
        level = 0x1000;
    }
    if (selfPtr->phase >= 38 && selfPtr->phase % 4 == 0) {
        targetActor = g_BattleHitTargetActors[selfPtr->sequenceIndex];
        if (targetActor != 0xFF) {
            ringRec = (BattleFxPoint3s *)Battle_AllocSceneMessage(0xA);
            ringRec->targetIndex = targetActor;
            ringRec->sequenceIndex = selfPtr->sequenceIndex;
            (selfPtr->sequenceIndex)++;
            child = (BattleFxLimitStardustRayChildTask *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitStardustRaySpawnSpriteBurstTask);
            Battle_GetBoneWorldPos(targetActor, g_BattleActorRecords[targetActor].targetBone, &child->posX);
            child->radius = g_BattleActorRecords[targetActor].radius;
        }
    }
    if (selfPtr->phase >= 80) {
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        level = 0;
        selfPtr->ownerSlot = BATTLE_TASK_FREE;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition(&selfPtr->pos), 0);
    }
    g_BattleFxTimelineState.startValue = level;
    (selfPtr->phase)++;
}

/*
 * When unpaused emits four BattleFx_LimitStardustRayBillboardSpriteTask slots with coordinate jitter rand modulo twice the parent's
 * radius minus that radius, without a zero-divisor guard, and scale 3200 times a random 0x800
 * through 0x17FF shifted twelve. Retires the parent at counter four or above after emission, then
 * advances the counter.
 */
// FUNCTION: FF7 0x004c9535
void __cdecl BattleFx_LimitStardustRaySpawnSpriteBurstTask(void)
{
    int jitter;
    int idx;
    int width;
    int baseScale;
    BattleFxLimitStardustRayBurstSprite *selfPtr;
    BattleFxStardustRaySpriteBurstRecord *child;
    int rnd;

    if (g_BattleIsPaused != 0) {
        return;
    }
    selfPtr = (BattleFxLimitStardustRayBurstSprite *)BATTLE_CURRENT_TASK_SLOT;
    jitter = selfPtr->radius;
    width = jitter << 1;
    baseScale = 0xC80;
    for (idx = 0; idx < 4; idx++) {
        child = (BattleFxStardustRaySpriteBurstRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitStardustRayBillboardSpriteTask);
        child->posX = selfPtr->posX + rand() % width - jitter;
        child->posY = selfPtr->posY + rand() % width - jitter;
        child->posZ = selfPtr->posZ + rand() % width - jitter;
        rnd = rand() % 4096 + 2048;
        child->scale = baseScale * rnd >> 12;
    }
    if (selfPtr->timer >= 4) {
        selfPtr->ownerSlot = BATTLE_TASK_FREE;
    }
    (selfPtr->timer)++;
}

/*
 * Draws one burst sprite: model-table sprite entry six at the task's frame, as a billboard at the
 * task's position and scale, chained into g_BattleLimitStardustRayDrawListHead. Frees the task from frame 3;
 * the frame advances only while unpaused, and the sprite is drawn while paused too.
 */
// FUNCTION: FF7 0x004c965f
void __cdecl BattleFx_LimitStardustRayBillboardSpriteTask(void)
{
    BattleFxStardustRayBillboardSprite *task;

    task = (BattleFxStardustRayBillboardSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitStardustRaySpriteReq2.frame = task->frame;
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->scale, -(task->scale >> 4));
    g_BattleLimitStardustRaySpriteReq2.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(6, g_BattleLimitStardustRayModelTable);
    g_BattleLimitStardustRaySpriteReq2.drawObj = Gfx_ModelTableGetSpriteObject(6, g_BattleLimitStardustRayModelTable);
    g_BattleLimitStardustRayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleLimitStardustRaySpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitStardustRayDrawListHead);
    if (task->frame >= 3) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (task->frame)++;
}

/*
 * Builds a packed-frame Z-rotation matrix at task X/Y and fixed Z 512, transforms a reference
 * velocity, applies uniform scale and draws model entry four through the alternate render-state
 * block with draw selector zero. Drawing and retirement from twenty occur while paused. Unpaused
 * calls move X/Y by stored speed times the transformed vector, increase speed by its signed
 * sixteenth and advance.
 */
// FUNCTION: FF7 0x004c972e
void __cdecl BattleFx_LimitStardustRayDrawMovingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[80];
        PsxMatrix modelMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxStardustRayMovingSlot *movingSlot; };
        struct { unsigned char pad_to_24[0x24]; VECTOR velX; };
        struct { unsigned char pad_to_34[0x34]; SVECTOR rotX; };
        struct { unsigned char pad_to_3C[0x3C]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_40[0x40]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.movingSlot = (BattleFxStardustRayMovingSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = frame.movingSlot->rotZ;
    RotMatrixXYZ(&frame.rotX, &frame.modelMatrix);
    frame.modelMatrix.t[0] = frame.movingSlot->posX;
    frame.modelMatrix.t[1] = frame.movingSlot->posY;
    frame.modelMatrix.t[2] = 0x200;
    TransformVectorToInt32(&frame.modelMatrix, &g_BattleLimitStardustRayModelReq1, &frame.velX);
    frame.scale.vz = frame.movingSlot->uniformScale;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.modelMatrix, &frame.scale);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    RefreshGlobalTransformState();
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(4, g_BattleLimitStardustRayModelTable);
    frame.modelDraw->flags = 0x80;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    g_BattleLimitStardustRayDrawListHead = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST_1, 0, g_BattleLimitStardustRayDrawListHead);
    if (frame.movingSlot->frame >= 20) {
        frame.movingSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.movingSlot->posX += (short)(frame.movingSlot->speed * frame.velX.vx >> 12);
    frame.movingSlot->posY += (short)(frame.movingSlot->speed * frame.velX.vy >> 12);
    frame.movingSlot->speed += (short)(frame.movingSlot->speed >> 4);
    (frame.movingSlot->frame)++;
}

/*
 * Draws all active entries in the thirty-record table with gray rising over eight counts, holding
 * through fifty-six and falling afterward. Unpaused count zero then fills up to twenty-four free
 * entries with advancing byte-masked table seeds and random angle shorts. Unpaused calls retire
 * from sixty-five and advance. Rendering precedes initialization and continues while paused.
 */
// FUNCTION: FF7 0x004c98e7
void __cdecl BattleFx_LimitStardustRayTexturedQuadPool30Task(void)
{
    int level;
    int idx;
    int colorWord;
    BattleFxLimitStardustRayQuadState *task;

    task = (BattleFxLimitStardustRayQuadState *)BATTLE_CURRENT_TASK_SLOT;
    if (task->index < 8) {
        level = task->index << 4;
    } else if (task->index >= 57) {
        level = 0x80 - (task->index - 0x39 << 4);
    } else {
        level = 0x80;
    }
    colorWord = level << 0x10 | level << 8 | level;
    for (idx = 0; idx < 30; idx++) {
        if (g_BattleLimitStardustRayQuadPool[idx].state != 0) {
            BattleFx_LimitStardustRayDrawTexturedQuad(&g_BattleLimitStardustRayQuadPool[idx].corner, g_BattleLimitStardustRayQuadPool[idx].variant, colorWord);
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->index >= 65) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (task->index == 0) {
        for (idx = 0; idx < 24; idx++) {
            for (colorWord = 0; colorWord < 0x1E && g_BattleLimitStardustRayQuadPool[colorWord].state != 0; colorWord++) {
            }
            if (colorWord >= 0x1E) {
                break;
            }
            g_BattleLimitStardustRayQuadPool[colorWord].state = 1;
            g_BattleLimitStardustRayRandomIndex = g_BattleLimitStardustRayRandomIndex + 1 & 0xFF;
            g_BattleLimitStardustRayQuadPool[colorWord].variant = (short)g_BattleLimitStardustRayRandomIndex;
            g_BattleLimitStardustRayQuadPool[colorWord].corner.srcX = rand() % 600;
            g_BattleLimitStardustRayQuadPool[colorWord].corner.srcY = rand() % 300;
        }
    }
    (task->index)++;
}

/*
 * Builds one PSX-style textured quad in the block returned by Gfx_GetPsxPacketScratchPtr and draws it.
 * Sets state opcode 0xB to Gfx_ModelTableGetSpriteObject(0, g_BattleLimitStardustRayModelTable), then takes the two shorts
 * at cornerPtr as the top-left corner, writing them to the quad's srcX/x0 and srcY/y0 fields with
 * the opposite corner 0x20 further in both axes (spanX/x1 and spanY/y1), so the quad is a
 * fixed 32 by 32. variant chooses between two texture and CLUT word sets -- 0x78C80000, 0x2D001F,
 * 0x1F00, 0x1F when nonzero, and 0x78C80020, 0x2D003F, 0x1F20, 0x3F when zero, the latter shifted
 * 0x20 across in u. Stores commandFlags | 0x2C000000 in command, ORs 2 into its top byte, forces the top byte
 * of the tag word to 9, calls Gfx_SetBlendFactorPair with the floats 0.2 and 1.0, then
 * Gfx_DrawPsxPrimitive, and leaves the block in g_BattleLimitStardustRayDrawListHead.
 */
// FUNCTION: FF7 0x004c9ab2
void __cdecl BattleFx_LimitStardustRayDrawTexturedQuad(BattleFxStardustRayCorner *cornerPtr, int variant, int commandFlags)
{
    BattleFxLimitStardustRayQuad *quad;

    quad = (BattleFxLimitStardustRayQuad *)Gfx_GetPsxPacketScratchPtr();
    Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(0, g_BattleLimitStardustRayModelTable)));
    quad->srcX = cornerPtr->srcX;
    quad->x0 = quad->srcX;
    quad->srcY = cornerPtr->srcY;
    quad->y0 = quad->srcY;
    quad->spanX = quad->x0 + 32;
    quad->x1 = quad->spanX;
    quad->spanY = quad->y0 + 32;
    quad->y1 = quad->spanY;
    if (variant != 0) {
        quad->clutWord = 0x78C80000;
        quad->uvWord = 0x2D001F;
        quad->tpageWord = 0x1F00;
        quad->uvMask = 0x1F;
    } else {
        quad->clutWord = 0x78C80020;
        quad->uvWord = 0x2D003F;
        quad->tpageWord = 0x1F20;
        quad->uvMask = 0x3F;
    }
    quad->command = commandFlags | 0x2C000000;
    quad->commandHigh |= 2;
    quad->tag = quad->tag & 0xFFFFFF | 0x9000000;
    Gfx_SetBlendFactorPair(FLOAT_0_2_BITS, FLOAT_ONE_BITS);
    Gfx_DrawPsxPrimitive(quad);
    g_BattleLimitStardustRayDrawListHead = (int)quad;
}

/*
 * Builds a packed-frame matrix from task position and uniform scale, composes with the camera and
 * draws model entry five in black with descriptor flags 0x88 and a count-shifted field from four.
 * Drawing and retirement from twelve occur while paused; unpaused calls add the stored scale step
 * and advance.
 */
// FUNCTION: FF7 0x004c9c02
void __cdecl BattleFx_LimitStardustRayDrawGrowingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        PsxMatrix modelMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxGrowingModelSlot *growingSlot; };
        struct { unsigned char pad_to_24[0x24]; SVECTOR rotX; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_30[0x30]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.growingSlot = (BattleFxGrowingModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.modelMatrix);
    frame.modelMatrix.t[0] = frame.growingSlot->posX;
    frame.modelMatrix.t[1] = frame.growingSlot->posY;
    frame.modelMatrix.t[2] = frame.growingSlot->posZ;
    frame.scale.vz = frame.growingSlot->growth;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.modelMatrix, &frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    RefreshGlobalTransformState();
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(5, g_BattleLimitStardustRayModelTable);
    frame.modelDraw->flags = 0x88;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    if (frame.growingSlot->frame >= 4) {
        frame.modelDraw->fadeLevel = frame.growingSlot->frame - 4 << 9;
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleLimitStardustRayDrawListHead = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitStardustRayDrawListHead);
    if (frame.growingSlot->frame >= 12) {
        frame.growingSlot->slotTag = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.growingSlot->growth += frame.growingSlot->growthStep;
    (frame.growingSlot->frame)++;
}

/*
 * Draws active entries from the 180-record pool using sprite entry three, age-derived gray and
 * billboard size, then emits each auxiliary spoke primitive with the caller retained byte-width
 * color cast. Unpaused calls move XYZ, update vertical velocity/acceleration, deactivate
 * ground-crossing or age-thirty-one entries and advance their ages. Before task count twenty-five
 * fills free records at a rate falling from eight to four with random radial positions/velocities,
 * spoke counts and size; retires the task from fifty-five and advances. Drawing continues while
 * paused.
 */
// FUNCTION: FF7 0x004c9da2
void __cdecl BattleFx_LimitStardustRaySpokeParticlePool180Task(void)
{
    PsxMatrix *mtx;
    int poolIndex;
    int emptyIdx;
    int rate;
    BattleFxLimitSpokes *workBlock;
    int baseSize;
    int rnd;
    int radius;
    BattleFxStardustRaySpokeParticle *emitter;

    emitter = (BattleFxStardustRaySpokeParticle *)BATTLE_CURRENT_TASK_SLOT;
    workBlock = (BattleFxLimitSpokes *)Gfx_GetPsxScratchpadPtr(0);
    g_BattleLimitStardustRayParticleReq3.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(3, g_BattleLimitStardustRayModelTable);
    g_BattleLimitStardustRayParticleReq3.drawObj = Gfx_ModelTableGetSpriteObject(3, g_BattleLimitStardustRayModelTable);
    for (poolIndex = 0; poolIndex < 180; poolIndex++) {
        if (g_BattleLimitStardustRaySpokePool[poolIndex].active != 0) {
            g_BattleLimitStardustRayParticleReq3.colorB = 0x80 - (g_BattleLimitStardustRaySpokePool[poolIndex].age << 2);
            g_BattleLimitStardustRayParticleReq3.colorG = g_BattleLimitStardustRayParticleReq3.colorB;
            g_BattleLimitStardustRayParticleReq3.colorR = g_BattleLimitStardustRayParticleReq3.colorG;
            mtx = (PsxMatrix *)BattleFx_InstallBillboardMatrix(&g_BattleLimitStardustRaySpokePool[poolIndex].posX, (short)g_BattleLimitStardustRaySpokePool[poolIndex].scale, -(g_BattleLimitStardustRaySpokePool[poolIndex].scale >> 3));
            workBlock->depth = mtx->t[2] >> 4;
            g_BattleLimitStardustRayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleLimitStardustRayParticleReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitStardustRayDrawListHead);
            // The original caller passes a byte; the callee masks its dword slot.
            ((void(__cdecl *)(BattleFxLimitSpokes *, void *, int, char))BattleFx_LimitStardustRayDrawSpokes)(workBlock, &g_BattleLimitStardustRaySpokePool[poolIndex].posX, g_BattleLimitStardustRaySpokePool[poolIndex].spokeLength, g_BattleLimitStardustRayParticleReq3.colorR);
            if (g_BattleIsPaused == 0) {
                g_BattleLimitStardustRaySpokePool[poolIndex].posX += g_BattleLimitStardustRaySpokePool[poolIndex].velX;
                g_BattleLimitStardustRaySpokePool[poolIndex].posY += g_BattleLimitStardustRaySpokePool[poolIndex].velY;
                g_BattleLimitStardustRaySpokePool[poolIndex].posZ += g_BattleLimitStardustRaySpokePool[poolIndex].velZ;
                g_BattleLimitStardustRaySpokePool[poolIndex].velY += g_BattleLimitStardustRaySpokePool[poolIndex].accelY;
                if (g_BattleLimitStardustRaySpokePool[poolIndex].velY < 0) {
                    g_BattleLimitStardustRaySpokePool[poolIndex].velY = 0;
                } else {
                    (g_BattleLimitStardustRaySpokePool[poolIndex].accelY)--;
                }
                if (g_BattleLimitStardustRaySpokePool[poolIndex].posY > 0 || g_BattleLimitStardustRaySpokePool[poolIndex].age >= 31) {
                    g_BattleLimitStardustRaySpokePool[poolIndex].active = 0;
                }
                (g_BattleLimitStardustRaySpokePool[poolIndex].age)++;
            }
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (emitter->frameCount >= 55) {
        emitter->ownerSlot = BATTLE_TASK_FREE;
    }
    if (emitter->frameCount < 25) {
        rate = 8 - (emitter->frameCount >> 1);
        if (rate < 4) {
            rate = 4;
        }
        baseSize = 0xF33;
        for (poolIndex = 0; poolIndex < rate; poolIndex++) {
            for (emptyIdx = 0; emptyIdx < 180 && g_BattleLimitStardustRaySpokePool[emptyIdx].active != 0; emptyIdx++) {
            }
            if (emptyIdx >= 180) {
                break;
            }
            g_BattleLimitStardustRaySpokePool[emptyIdx].active = 1;
            g_BattleLimitStardustRaySpokePool[emptyIdx].age = 0;
            rnd = ((rand()) % 4096);
            radius = rand() % 180 + 80;
            g_BattleLimitStardustRaySpokePool[emptyIdx].posX = emitter->posX + (CosFixed(rnd) * radius >> 12);
            g_BattleLimitStardustRaySpokePool[emptyIdx].posY = emitter->posY;
            g_BattleLimitStardustRaySpokePool[emptyIdx].posZ = emitter->posZ + (SinFixed(rnd) * radius >> 12);
            radius = rand() % 80 + 50;
            g_BattleLimitStardustRaySpokePool[emptyIdx].velX = CosFixed(rnd) * radius >> 12;
            g_BattleLimitStardustRaySpokePool[emptyIdx].velY = rand() % 110 + 220;
            g_BattleLimitStardustRaySpokePool[emptyIdx].velZ = SinFixed(rnd) * radius >> 12;
            g_BattleLimitStardustRaySpokePool[emptyIdx].accelY = -(((rand()) % 4) + 4);
            g_BattleLimitStardustRaySpokePool[emptyIdx].spokeLength = rand() % 5 + 3;
            rnd = ((rand()) % 4096) + 2560 - emitter->frameCount * 20;
            g_BattleLimitStardustRaySpokePool[emptyIdx].scale = baseSize * rnd >> 12;
        }
    }
    (emitter->frameCount)++;
}

/*
 * Builds paired random offsets around the supplied point using the shared byte-wrapped
 * random-table index, projects them and draws twice the requested count of spokes from the
 * projected center. Masks the fourth DWORD argument to its low byte and replicates it into
 * opcode-0x52 color (the caller passes a byte). Updates blend factors per spoke and leaves the
 * primitive block in g_BattleLimitStardustRayDrawListHead.
 */
// FUNCTION: FF7 0x004ca2a3
void __cdecl BattleFx_LimitStardustRayDrawSpokes(BattleFxLimitSpokes *spokes, Int64Pair *centerPair, int count, int color)
{
    int screenY;
    int scrX;
    int rgbWord;
    BattleFxStardustRaySpokePrim *pSpokePrim;
    int vertexIdx;
    int jitterX;
    int srcSample;
    int i;
    int jitterZ;
    int offsetY;

    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    spokes->center = *centerPair;
    spokes->vertexEnd[1] = spokes->center;
    vertexIdx = 1;
    for (i = 0; i < count; i++) {
        g_BattleLimitStardustRayRandomIndex = g_BattleLimitStardustRayRandomIndex + 1 & 0xFF;
        srcSample = g_BattleLimitStardustRayRandomTable[g_BattleLimitStardustRayRandomIndex] % 200 - 0x64;
        if (srcSample > 0) {
            jitterX = srcSample + 70;
        } else {
            jitterX = srcSample - 70;
        }
        spokes->spreadX = jitterX;
        g_BattleLimitStardustRayRandomIndex = g_BattleLimitStardustRayRandomIndex + 1 & 0xFF;
        srcSample = g_BattleLimitStardustRayRandomTable[g_BattleLimitStardustRayRandomIndex] % 200 - 0x64;
        if (srcSample > 0) {
            offsetY = srcSample + 0x46;
        } else {
            offsetY = srcSample - 0x46;
        }
        spokes->spreadY = offsetY;
        g_BattleLimitStardustRayRandomIndex = g_BattleLimitStardustRayRandomIndex + 1 & 0xFF;
        srcSample = g_BattleLimitStardustRayRandomTable[g_BattleLimitStardustRayRandomIndex] % 200 - 0x64;
        if (srcSample > 0) {
            jitterZ = srcSample + 70;
        } else {
            jitterZ = srcSample - 70;
        }
        spokes->spreadZ = jitterZ;
        ProjectPointHardware(spokes->vertex[vertexIdx], (short *)&spokes->projected[vertexIdx], &scrX, &screenY);
        spokes->vertex[vertexIdx + 1][0] = spokes->centerX + spokes->spreadX;
        spokes->vertex[vertexIdx + 1][1] = spokes->centerY + spokes->spreadY;
        spokes->vertex[vertexIdx + 1][2] = spokes->centerZ + spokes->spreadZ;
        vertexIdx++;
        ProjectPointHardware(spokes->vertex[vertexIdx], (short *)&spokes->projected[vertexIdx], &scrX, &screenY);
        spokes->vertex[vertexIdx + 1][0] = spokes->centerX - spokes->spreadX;
        spokes->vertex[vertexIdx + 1][1] = spokes->centerY - spokes->spreadY;
        spokes->vertex[vertexIdx + 1][2] = spokes->centerZ - spokes->spreadZ;
        vertexIdx++;
    }
    ProjectPointHardware(spokes->vertex[vertexIdx], (short *)&spokes->projected[vertexIdx], &scrX, &screenY);
    pSpokePrim = (BattleFxStardustRaySpokePrim *)Gfx_GetPsxPacketScratchPtr();
    rgbWord = (color & 0xFF) << 0x10 | (color & 0xFF) << 8 | color & 0xFF | 0x52000000;
    for (vertexIdx = 1; vertexIdx <= count << 1; vertexIdx++) {
        pSpokePrim->startXY = spokes->projectedCenter;
        pSpokePrim->endXY = spokes->projected[vertexIdx + 1];
        pSpokePrim->nextPrim = rgbWord;
        pSpokePrim->endColor = 0;
        pSpokePrim->tag = pSpokePrim->tag & 0xFFFFFF | 0x4000000;
        /* Retail caller prototype (float, float) -> void preserved for /Od argument promotion on `Gfx_SetBlendFactorPair`. */
        ((void(__cdecl *)(float, float))Gfx_SetBlendFactorPair)(GetLastProjectedNdcZ(), GetLastProjectedNdcW());
        Gfx_DrawPsxPrimitive(pSpokePrim);
    }
    g_BattleLimitStardustRayDrawListHead = (int)pSpokePrim;
}

/*
 * Draws active entries from the 120-record pool using sprite entry one, rotated/scaled view-space
 * matrices and gray holding until age sixteen then falling. Unpaused calls move XYZ, update
 * vertical velocity/acceleration and Z rotation, deactivate ground-crossing or age-thirty-one
 * entries and advance ages. Before task count twenty-five fills free records at a rate falling
 * from six to three with randomized radial motion, size and signed rotation step; retires the task
 * from fifty-five and advances. Drawing continues while paused.
 */
// FUNCTION: FF7 0x004ca64a
void __cdecl BattleFx_LimitStardustRaySpriteParticlePool120Task(void)
{
    struct BattleLimitStardustDotTask *scratch;
    int idx;
    int spawnCount;
    int emptyIdx;
    int baseSize;
    int rnd;
    int radius;
    BattleFxStardustRaySpokeParticle *emitter;

    emitter = (BattleFxStardustRaySpokeParticle *)BATTLE_CURRENT_TASK_SLOT;
    scratch = (struct BattleLimitStardustDotTask *)Gfx_GetPsxScratchpadPtr(0);
    scratch->rotX = 0;
    scratch->rotY = 0;
    scratch->rotZ = 0;
    g_BattleLimitStardustRayParticleReq4.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_BattleLimitStardustRayModelTable);
    g_BattleLimitStardustRayParticleReq4.drawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleLimitStardustRayModelTable);
    for (idx = 0; idx < 120; idx++) {
        if (g_BattleLimitStardustRayParticlePool1[idx].active != 0) {
            if (g_BattleLimitStardustRayParticlePool1[idx].age >= 16) {
                g_BattleLimitStardustRayParticleReq4.colorB = 0x80 - (g_BattleLimitStardustRayParticlePool1[idx].age * 8 + -0x80);
                g_BattleLimitStardustRayParticleReq4.colorG = g_BattleLimitStardustRayParticleReq4.colorB;
                g_BattleLimitStardustRayParticleReq4.colorR = g_BattleLimitStardustRayParticleReq4.colorG;
            } else {
                g_BattleLimitStardustRayParticleReq4.colorB = 0x80;
                g_BattleLimitStardustRayParticleReq4.colorG = g_BattleLimitStardustRayParticleReq4.colorB;
                g_BattleLimitStardustRayParticleReq4.colorR = g_BattleLimitStardustRayParticleReq4.colorG;
            }
            scratch->rotZ = g_BattleLimitStardustRayParticlePool1[idx].rotZ;
            RotMatrixXYZ(&scratch->rotX, scratch->matrix);
            scratch->scaleZ = g_BattleLimitStardustRayParticlePool1[idx].scale;
            scratch->scaleY = scratch->scaleZ;
            scratch->scaleX = scratch->scaleY;
            ScaleMatrix(scratch->matrix, &scratch->scaleX);
            BattleFx_MakeViewSpacePointPushed(&g_BattleLimitStardustRayParticlePool1[idx].posX, -(g_BattleLimitStardustRayParticlePool1[idx].scale >> 3), (Vec3sPacked *)scratch->matrix);
            SetRotMatrix(scratch->matrix);
            SetTransMatrix(scratch->matrix);
            g_BattleLimitStardustRayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleLimitStardustRayParticleReq4, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitStardustRayDrawListHead);
            if (g_BattleIsPaused == 0) {
                g_BattleLimitStardustRayParticlePool1[idx].posX += g_BattleLimitStardustRayParticlePool1[idx].velX;
                g_BattleLimitStardustRayParticlePool1[idx].posY += g_BattleLimitStardustRayParticlePool1[idx].velY;
                g_BattleLimitStardustRayParticlePool1[idx].posZ += g_BattleLimitStardustRayParticlePool1[idx].velZ;
                g_BattleLimitStardustRayParticlePool1[idx].velY += g_BattleLimitStardustRayParticlePool1[idx].accelY;
                g_BattleLimitStardustRayParticlePool1[idx].rotZ += g_BattleLimitStardustRayParticlePool1[idx].rotStep;
                if (g_BattleLimitStardustRayParticlePool1[idx].velY < 0) {
                    g_BattleLimitStardustRayParticlePool1[idx].velY = 0;
                } else {
                    (g_BattleLimitStardustRayParticlePool1[idx].accelY)--;
                }
                if (g_BattleLimitStardustRayParticlePool1[idx].posY > 0 || g_BattleLimitStardustRayParticlePool1[idx].age >= 31) {
                    g_BattleLimitStardustRayParticlePool1[idx].active = 0;
                }
                (g_BattleLimitStardustRayParticlePool1[idx].age)++;
            }
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (emitter->frameCount >= 55) {
        emitter->ownerSlot = BATTLE_TASK_FREE;
    }
    if (emitter->frameCount < 25) {
        spawnCount = 6 - (emitter->frameCount >> 1);
        if (spawnCount < 3) {
            spawnCount = 3;
        }
        baseSize = 0x10E0;
        for (idx = 0; idx < spawnCount; idx++) {
            for (emptyIdx = 0; emptyIdx < 120 && g_BattleLimitStardustRayParticlePool1[emptyIdx].active != 0; emptyIdx++) {
            }
            if (emptyIdx >= 120) {
                break;
            }
            g_BattleLimitStardustRayParticlePool1[emptyIdx].active = 1;
            g_BattleLimitStardustRayParticlePool1[emptyIdx].age = 0;
            rnd = ((rand()) % 4096);
            radius = rand() % 180 + 80;
            g_BattleLimitStardustRayParticlePool1[emptyIdx].posX = emitter->posX + (CosFixed(rnd) * radius >> 12);
            g_BattleLimitStardustRayParticlePool1[emptyIdx].posY = emitter->posY + radius;
            g_BattleLimitStardustRayParticlePool1[emptyIdx].posZ = emitter->posZ + (SinFixed(rnd) * radius >> 12);
            g_BattleLimitStardustRayRandomIndex = g_BattleLimitStardustRayRandomIndex + 1 & 0xFF;
            g_BattleLimitStardustRayParticlePool1[emptyIdx].rotZ = (short)g_BattleLimitStardustRayRandomIndex;
            radius = rand() % 85 + 50;
            g_BattleLimitStardustRayParticlePool1[emptyIdx].velX = CosFixed(rnd) * radius >> 12;
            g_BattleLimitStardustRayParticlePool1[emptyIdx].velY = rand() % 130 + 200;
            g_BattleLimitStardustRayParticlePool1[emptyIdx].velZ = SinFixed(rnd) * radius >> 12;
            g_BattleLimitStardustRayParticlePool1[emptyIdx].accelY = -(((rand()) % 4) + 4);
            g_BattleLimitStardustRayParticlePool1[emptyIdx].rotStep = rand() % 0xDC + 0x96;
            if ((g_BattleLimitStardustRayParticlePool1[emptyIdx].rotZ & 1) != 0) {
                g_BattleLimitStardustRayParticlePool1[emptyIdx].rotStep = -g_BattleLimitStardustRayParticlePool1[emptyIdx].rotStep;
            }
            rnd = rand() % 5120 + 2048 - (emitter->frameCount << 2);
            g_BattleLimitStardustRayParticlePool1[emptyIdx].scale = baseSize * rnd >> 12;
        }
    }
    (emitter->frameCount)++;
}

/*
 * Draws active entries from the 120-record pool using sprite entry two, age-derived gray and
 * billboard size. Unpaused calls move XYZ, update vertical velocity/acceleration, deactivate
 * ground-crossing or age-thirty-one entries and advance ages. Before task count twenty-five fills
 * free records at a rate falling from six to three with randomized radial position/velocity and
 * size; retires the task from fifty-five and advances. Drawing continues while paused.
 */
// FUNCTION: FF7 0x004cac85
void __cdecl BattleFx_LimitStardustRaySpriteEntry2ParticlePool120Task(void)
{
    int scratch;
    int idx;
    int spawnCount;
    int emptyIdx;
    int baseSize;
    int rnd;
    int radius;
    BattleFxLimitStardustRayParticleSeed *emitter;

    emitter = (BattleFxLimitStardustRayParticleSeed *)BATTLE_CURRENT_TASK_SLOT;
    scratch = Gfx_GetPsxScratchpadPtr(0);
    g_BattleLimitStardustRayParticleReq5.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(2, g_BattleLimitStardustRayModelTable);
    g_BattleLimitStardustRayParticleReq5.drawObj = Gfx_ModelTableGetSpriteObject(2, g_BattleLimitStardustRayModelTable);
    for (idx = 0; idx < 120; idx++) {
        if (g_BattleLimitStardustRayParticlePool2[idx].active != 0) {
            g_BattleLimitStardustRayParticleReq5.colorB = 0x80 - (g_BattleLimitStardustRayParticlePool2[idx].age << 2);
            g_BattleLimitStardustRayParticleReq5.colorG = g_BattleLimitStardustRayParticleReq5.colorB;
            g_BattleLimitStardustRayParticleReq5.colorR = g_BattleLimitStardustRayParticleReq5.colorG;
            BattleFx_InstallBillboardMatrix(&g_BattleLimitStardustRayParticlePool2[idx].posX, (short)g_BattleLimitStardustRayParticlePool2[idx].scale, -(g_BattleLimitStardustRayParticlePool2[idx].scale >> 3));
            g_BattleLimitStardustRayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleLimitStardustRayParticleReq5, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitStardustRayDrawListHead);
            if (g_BattleIsPaused == 0) {
                g_BattleLimitStardustRayParticlePool2[idx].posX += g_BattleLimitStardustRayParticlePool2[idx].velX;
                g_BattleLimitStardustRayParticlePool2[idx].posY += g_BattleLimitStardustRayParticlePool2[idx].velY;
                g_BattleLimitStardustRayParticlePool2[idx].posZ += g_BattleLimitStardustRayParticlePool2[idx].velZ;
                g_BattleLimitStardustRayParticlePool2[idx].velY += g_BattleLimitStardustRayParticlePool2[idx].accelY;
                if (g_BattleLimitStardustRayParticlePool2[idx].velY < 0) {
                    g_BattleLimitStardustRayParticlePool2[idx].velY = 0;
                } else {
                    (g_BattleLimitStardustRayParticlePool2[idx].accelY)--;
                }
                if (g_BattleLimitStardustRayParticlePool2[idx].posY > 0 || g_BattleLimitStardustRayParticlePool2[idx].age >= 31) {
                    g_BattleLimitStardustRayParticlePool2[idx].active = 0;
                }
                (g_BattleLimitStardustRayParticlePool2[idx].age)++;
            }
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (emitter->index >= 55) {
        emitter->ownerSlot = BATTLE_TASK_FREE;
    }
    if (emitter->index < 25) {
        spawnCount = 6 - (emitter->index >> 1);
        if (spawnCount < 3) {
            spawnCount = 3;
        }
        baseSize = 0x1400;
        for (idx = 0; idx < spawnCount; idx++) {
            for (emptyIdx = 0; emptyIdx < 120 && g_BattleLimitStardustRayParticlePool2[emptyIdx].active != 0; emptyIdx++) {
            }
            if (emptyIdx >= 120) {
                break;
            }
            g_BattleLimitStardustRayParticlePool2[emptyIdx].active = 1;
            g_BattleLimitStardustRayParticlePool2[emptyIdx].age = 0;
            rnd = ((rand()) % 4096);
            radius = rand() % 180 + 80;
            g_BattleLimitStardustRayParticlePool2[emptyIdx].posX = emitter->posX + (CosFixed(rnd) * radius >> 12);
            g_BattleLimitStardustRayParticlePool2[emptyIdx].posY = emitter->posY + radius;
            g_BattleLimitStardustRayParticlePool2[emptyIdx].posZ = emitter->posZ + (SinFixed(rnd) * radius >> 12);
            radius = rand() % 90 + 50;
            g_BattleLimitStardustRayParticlePool2[emptyIdx].velX = CosFixed(rnd) * radius >> 12;
            g_BattleLimitStardustRayParticlePool2[emptyIdx].velY = rand() % 130 + 200;
            g_BattleLimitStardustRayParticlePool2[emptyIdx].velZ = SinFixed(rnd) * radius >> 12;
            g_BattleLimitStardustRayParticlePool2[emptyIdx].accelY = -(((rand()) % 4) + 4);
            rnd = ((rand()) % 4096) + 2048;
            g_BattleLimitStardustRayParticlePool2[emptyIdx].scale = baseSize * rnd >> 12;
        }
    }
    (emitter->index)++;
}
} /* extern "C" */
