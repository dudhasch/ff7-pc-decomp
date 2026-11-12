// src/ff7/src/battle/battle_fx_limit_bloodfest.cpp
// Limit Break [52] (Yuffie Lv 2-2): Bloodfest (Japanese: 血祭, dev tag: "LIMIT2/MATSURI" / Matsuri)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Matsuri limit effect: loads its resources, stores the target centroid, and runs a
//   timeline that drives a battle sprite animation on an effect slot.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_limit.h"
#include "lib/codegen.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/api/graphics.h"
extern "C" {

#pragma intrinsic(memset)

// clang-format off
unsigned char g_rawData_00806508[16] = {0x59, 0x01, 0x00, 0x00, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00806518
int g_BattleLimitBloodfestSoundGroup[2] = {8414472, 0};
// GLOBAL: FF7 0x00806520
int g_BattleLimitBloodfestResourceDef = 0x00192A39;
char *g_BattleFxLimitBloodfestResourceNames[1] = {
    // STRING: FF7 0x00806574
    "CHI.TIM"
};
unsigned int g_BattleFxLimitBloodfestAssetPrefix[4] = {
    // STRING: FF7 0x0080657c
    (unsigned int)"CHI_1.SPT",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x00806538
struct BattleLimitGreasedLightningSpriteRequest g_BattleLimitBloodfestSpriteRequest = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
// GLOBAL: FF7 0x00806548
struct BattleLimitBloodfestHitFrames1 g_BattleLimitBloodfestHitFrames1 = {{0x14, 0}, 24};
// clang-format off
unsigned char g_rawData_0080654c[4] = {0x1f, 0x00, 0x27, 0x00};
// clang-format on
unsigned char g_rawData_00806550[16] = {0x2f, 0x00, 0x39, 0x00, 0x40, 0x00, 0x50, 0x00, 0x58, 0x00, 0x60, 0x00, 0x71, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00806560
short g_BattleLimitBloodfestHitFrames2[2] = {22, 27};
// clang-format off
unsigned char g_rawData_00806564[4] = {0x24, 0x00, 0x2b, 0x00};
unsigned char g_rawData_00806568[12] = {0x36, 0x00, 0x3c, 0x00, 0x47, 0x00, 0x55, 0x00, 0x5d, 0x00, 0x69, 0x00};
// clang-format on
// GLOBAL: FF7 0x00aef7c0
short g_BattleLimitBloodfestActorIdx = 0;
// GLOBAL: FF7 0x00aef7c4
short g_BattleLimitBloodfestTargetActorIdx = 0;
// GLOBAL: FF7 0x00aef7c8
int g_BattleLimitBloodfestCentroidX = 0;
unsigned char g_bssPad_00aef7cc[4] = {0};
unsigned char g_bssPad_00aef7d0[4] = {0};
// GLOBAL: FF7 0x00aef7d4
short g_BattleLimitBloodfestTargetMask = 0;
// GLOBAL: FF7 0x00aef7d8
short g_BattleLimitBloodfestTimer = 0;
unsigned char g_bssPad_00aef7dc[4] = {0};
// GLOBAL: FF7 0x00aef7e0
struct BattleLimitBloodfestPosDeltaX g_BattleLimitBloodfestPosDeltaX = {0};
// GLOBAL: FF7 0x00aef7e4
short g_BattleLimitBloodfestPosDeltaZ = 0;
// GLOBAL: FF7 0x00aef7e8
struct BattleLimitHighwindLoadedResTable g_BattleLimitBloodfestComputedPosX = {0};
// GLOBAL: FF7 0x00aef7ec
short g_BattleLimitBloodfestComputedPosZ = 0;
// GLOBAL: FF7 0x00aef7f0
short g_BattleLimitBloodfestHitIndex = 0;
// GLOBAL: FF7 0x00aef7f4
struct GfxTableCursor *g_BattleLimitBloodfestLoadedResTable = 0;
// clang-format on

#pragma intrinsic(memcpy)
#pragma function(memcpy)

// When scene pointer is nonzero and g_BattleLimitBloodfestLoadedResTable is zero, initializes local
// resource request with default duration and the LIMIT2/MATSURI path, then calls
// Gfx_LoadModelTableIfAbsent with g_BattleLimitBloodfestResourceDef, caching the table in
// g_BattleLimitBloodfestLoadedResTable under the scene's model table bank, and registers
// g_BattleLimitBloodfestSoundGroup. Otherwise does nothing.
// FUNCTION: FF7 0x004ae240
void __cdecl BattleFx_LimitLoadResBloodfest(void)
{
    GfxModelTableBank *ownerHandle;
    GfxResourceRequest recordBuf;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int stepResourceDef;

    if (g_pBattle3dScene != 0 && g_BattleLimitBloodfestLoadedResTable == 0) {
        kind = 1;
        stepResourceDef = (int)&g_BattleLimitBloodfestResourceDef;
        ownerHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &recordBuf;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x00806588
        pRequest->resourcePath = (char *)BattleFx_BuildResourcePath("LIMIT2/MATSURI", pathText);
        Gfx_LoadModelTableIfAbsent(kind, stepResourceDef, pRequest, &g_BattleLimitBloodfestLoadedResTable, ownerHandle);
        SfxRegisterSoundGroups((struct SoundGroup **)&g_BattleLimitBloodfestSoundGroup);
    }
}

// Loads Matsuri resources, clears the position-delta and computed-position state and
// g_BattleLimitBloodfestTimer/g_BattleLimitBloodfestHitIndex, calls BattleFx_LimitBeginBloodfest, and returns zero.
// FUNCTION: FF7 0x004ae2e1
int __cdecl BattleFx_LimitEnterBloodfest(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResBloodfest();
    g_BattleLimitBloodfestComputedPosZ = 0;
    g_BattleLimitBloodfestComputedPosX.posY = g_BattleLimitBloodfestComputedPosZ;
    (g_BattleLimitBloodfestComputedPosX.posX) = g_BattleLimitBloodfestComputedPosX.posY;
    g_BattleLimitBloodfestPosDeltaZ = (g_BattleLimitBloodfestComputedPosX.posX);
    g_BattleLimitBloodfestPosDeltaX.posY = g_BattleLimitBloodfestPosDeltaZ;
    (g_BattleLimitBloodfestPosDeltaX.deltaX) = g_BattleLimitBloodfestPosDeltaX.posY;
    g_BattleLimitBloodfestTimer = 0;
    g_BattleLimitBloodfestHitIndex = g_BattleLimitBloodfestTimer;
    BattleFx_LimitBeginBloodfest(targetMask, actorIdx);
    return 0;
}

// Stores the target mask and its centroid, allocates BattleFx_LimitBloodfestTimelineTask with the
// attacking actor index, clears the timeline state's channels, sets its red to 0x40, then starts
// the fixed-point ramp (0x1000, 0xA).
// FUNCTION: FF7 0x004ae35f
void __cdecl BattleFx_LimitBeginBloodfest(int targetMask, int actorIdx)
{
    struct BattleTaskSlot *slot;

    g_BattleLimitBloodfestTargetMask = targetMask;
    Battle_GetActorMaskCentroid(targetMask, (short *)&g_BattleLimitBloodfestCentroidX);
    slot = BATTLE_NEW_TASK_SLOT(BattleFx_LimitBloodfestTimelineTask);
    ((BattleFxLimitBloodfestTaskSlot *)slot)->actorIndex = actorIdx;
    g_BattleFxTimelineState.channel1 = 0;
    g_BattleFxTimelineState.channel2 = g_BattleFxTimelineState.channel1;
    g_BattleFxTimelineState.red = 0x40;
    Battle_StartFixedPointRamp(0x1000, 10);
}

// Returns while paused. While the hit index is below ten and the task's step reaches that hit's
// frame in g_BattleLimitBloodfestHitFrames1, picks the hit's target actor and a random heading, and
// places the attacker at the target's position offset by that heading at the target's radius plus
// 1500, facing along the heading. Sets the countdown from the paired frame tables and starts a
// trail task. When the countdown reaches zero it allocates a ring record for the hit, starts
// BattleFx_LimitBloodfestSpriteAnimationTask at the target bone, plays sound group zero panned from
// the attacker's position and advances the hit index. At step 130 it ramps the effect out, frees
// the slot and releases sounds 0x159 and 0x142.
// FUNCTION: FF7 0x004ae3d2
void __cdecl BattleFx_LimitBloodfestTimelineTask(void)
{
    short heading;
    short distance;
    BattleFxLimitBloodfestTaskSlot *taskPtr;
    int s;
    int u;
    BattleFxBloodfestRecord *impactPtr;
    int child;

    taskPtr = (BattleFxLimitBloodfestTaskSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (g_BattleLimitBloodfestHitIndex < 10 && taskPtr->step == ((short *)&g_BattleLimitBloodfestHitFrames1)[g_BattleLimitBloodfestHitIndex]) {
        g_BattleLimitBloodfestActorIdx = taskPtr->actorIndex;
        g_BattleLimitBloodfestTargetActorIdx = g_BattleHitTargetActors[g_BattleLimitBloodfestHitIndex];
        (g_BattleLimitBloodfestPosDeltaX.deltaX) = g_BattleActorRecords[g_BattleLimitBloodfestTargetActorIdx].posX;
        g_BattleLimitBloodfestPosDeltaX.posY = g_BattleActorRecords[g_BattleLimitBloodfestTargetActorIdx].posY;
        g_BattleLimitBloodfestPosDeltaZ = g_BattleActorRecords[g_BattleLimitBloodfestTargetActorIdx].posZ;
        heading = rand() % 0x1000;
        distance = g_BattleActorRecords[g_BattleLimitBloodfestTargetActorIdx].radius + 1500;
        (g_BattleLimitBloodfestPosDeltaX.deltaX) += (short)(SinFixed(heading) * distance >> 12);
        (g_BattleLimitBloodfestComputedPosX.posX) = (g_BattleLimitBloodfestPosDeltaX.deltaX);
        g_BattleLimitBloodfestComputedPosX.posY = 0;
        g_BattleLimitBloodfestPosDeltaZ += (short)(CosFixed(heading) * distance >> 12);
        g_BattleLimitBloodfestComputedPosZ = g_BattleLimitBloodfestPosDeltaZ;
        g_BattleActorRecords[taskPtr->actorIndex].posX = (g_BattleLimitBloodfestComputedPosX.posX);
        g_BattleActorRecords[taskPtr->actorIndex].posY = g_BattleLimitBloodfestComputedPosX.posY;
        g_BattleActorRecords[taskPtr->actorIndex].posZ = g_BattleLimitBloodfestComputedPosZ;
        g_BattleActorRecords[taskPtr->actorIndex].rotX = 0;
        g_BattleActorRecords[taskPtr->actorIndex].yaw = heading;
        g_BattleActorRecords[taskPtr->actorIndex].rotZ = 0;
        g_BattleLimitBloodfestTimer = g_BattleLimitBloodfestHitFrames2[g_BattleLimitBloodfestHitIndex] - ((short *)&g_BattleLimitBloodfestHitFrames1)[g_BattleLimitBloodfestHitIndex];
        Battle_CreateEffectTrailTask(g_BattleLimitBloodfestActorIdx, g_BattleActorRecords[g_BattleLimitBloodfestActorIdx].primaryBone, 0xC8, 0x208020, 4, (&g_BattleLimitBloodfestHitFrames1.frameShort2)[g_BattleLimitBloodfestHitIndex] - ((short *)&g_BattleLimitBloodfestHitFrames1)[g_BattleLimitBloodfestHitIndex]);
    }
    if (g_BattleLimitBloodfestTimer > 0) {
        g_BattleLimitBloodfestTimer--;
        if (g_BattleLimitBloodfestTimer == 0) {
            impactPtr = (BattleFxBloodfestRecord *)Battle_AllocSceneMessage(0xA);
            impactPtr->targetActorIndex = g_BattleLimitBloodfestTargetActorIdx;
            impactPtr->hitIndex = g_BattleLimitBloodfestHitIndex;
            child = (int)BATTLE_NEW_TASK_SLOT(BattleFx_LimitBloodfestSpriteAnimationTask);
            Battle_GetBoneWorldPos(g_BattleLimitBloodfestTargetActorIdx, g_BattleActorRecords[g_BattleLimitBloodfestTargetActorIdx].targetBone, (short *)(child + 4));
            SfxPlaySoundGroup(Battle_ComputeScreenPanFromPosition((short *)&g_BattleLimitBloodfestComputedPosX), 0);
            g_BattleLimitBloodfestHitIndex++;
        }
    }
    (taskPtr->step)++;
    if (taskPtr->step == 130) {
        Battle_StartFixedPointRamp(0, 10);
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
        Sfx_ReleaseCachedBuffer(0x159, 0xF);
        Sfx_ReleaseCachedBuffer(0x142, 0xF);
    }
}

// One frame of the Matsuri sprite effect, like BattleFx_LimitDoomOfTheLivingAnimatedSpriteTask without
// position integration: takes sprite handle and resource from entry 0 of
// g_BattleLimitBloodfestLoadedResTable, builds the billboard at scale 0x2000, sets the request's
// frame from the slot's frameCounter and submits the draw. While unpaused and the sprite is
// resolved the frame advances until it reaches the animation length, which frees the slot.
// FUNCTION: FF7 0x004ae76e
void __cdecl BattleFx_LimitBloodfestSpriteAnimationTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitBloodfestSpriteRequest.spriteHandle = Gfx_ModelTableGetLoadedRecord(0, g_BattleLimitBloodfestLoadedResTable);
    g_BattleLimitBloodfestSpriteRequest.res = Gfx_ModelTableGetSpriteObject(0, g_BattleLimitBloodfestLoadedResTable);
    BattleFx_InstallBillboardMatrix(task->data, 0x2000 /* 2.0 */, 0);
    g_BattleLimitBloodfestSpriteRequest.frame = task->frameCounter;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleLimitBloodfestSpriteRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0 && g_BattleLimitBloodfestSpriteRequest.spriteHandle != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= *(int *)((char *)*(int *)g_BattleLimitBloodfestSpriteRequest.spriteHandle + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
