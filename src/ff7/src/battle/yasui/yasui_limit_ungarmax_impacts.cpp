// src/ff7/src/battle/yasui/yasui_limit_ungarmax_impacts.cpp
// Limit Break [12] (Barret Lv 3-2): Ungarmax (Japanese: アンガーマックス Angermax, dev tag: "LIMIT/A_MAX" / AMax)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Angermax limit break effect: impact billboards, animated sprites and fading point
//   particles on each target, plus two gun barrel flash models attached to the actor.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_boss.h"
#include "ff7/battle_tasks.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include <string.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include <stdlib.h>
extern "C" {

struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x007ed3a0
int g_YasuiLimitUngarmaxResourceDef = 0x00C800B1;
int g_YasuiLimitUngarmaxResourceDefHeadPtr[1] = {
    (int)((char *)&g_YasuiLimitUngarmaxImpactsResourceNames + 0x20)
};
int g_YasuiLimitUngarmaxResourceDefDataTail[18] = {
    0x0, 0x0, 0x0, 0xc800b1, (int)((char *)&g_YasuiLimitUngarmaxImpactsResourceNames + 0x2c), 0x0,
    0x0, 0x0, 0x1900b1, (int)((char *)&g_YasuiLimitUngarmaxImpactsResourceNames + 0x38), 0x0, 0x0,
    0x0, 0x192a39, (int)((char *)&g_YasuiLimitUngarmaxImpactsResourceNames + 0x44), (int)((char *)&g_YasuiLimitUngarmaxImpactsResourceNames + 0x50), 0x0, 0x0
};
// GLOBAL: FF7 0x007ed3f0
struct YasuiLimitUngarmaxDrawReqPool g_YasuiLimitUngarmaxDrawReqPool = {0, {0xa8, 0, 0, 0, 0, 0, 0, 0, 0x20, 0, 0, 0}, 0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
unsigned char g_rawData_007ed410[80] = {
    0xff, 0x80, 0x40, 0x62, 0x00, 0x00, 0x00, 0x00, 0x18, 0xfc, 0x00, 0x00, 0x18, 0xfc, 0x00, 0x00, 0xe8, 0x03, 0x00, 0x00, 0x18, 0xfc, 0x00, 0x00,
    0x18, 0xfc, 0x00, 0x00, 0xe8, 0x03, 0x00, 0x00, 0xe8, 0x03, 0x00, 0x00, 0xe8, 0x03, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x6a, 0xff, 0x00, 0x00
};
// GLOBAL: FF7 0x00a41a08
struct PsxMatrix *g_YasuiLimitUngarmaxScratchMatrix = 0;
// GLOBAL: FF7 0x00a41a0c
unsigned char g_YasuiLimitUngarmaxImpactInitFlag = 0;
// GLOBAL: FF7 0x00a41a10
int g_YasuiLimitUngarmaxDrawListHead = 0;
// GLOBAL: FF7 0x00a41a14
int g_YasuiLimitUngarmaxSourceActor = 0;
// GLOBAL: FF7 0x00a41a18
YasuiLimitUngarmaxProjectScratch *g_YasuiLimitUngarmaxParticleScratch = 0;
// GLOBAL: FF7 0x00a41a1c
int g_YasuiLimitUngarmaxTargetActor = 0;
// GLOBAL: FF7 0x00a41a20
YasuiLimitUngarmaxImpactScratch *g_YasuiLimitUngarmaxBonePosScratch = 0;
// GLOBAL: FF7 0x00a41a24
unsigned char g_YasuiLimitUngarmaxModelInitFlag = 0;
unsigned char g_bssPad_00a41a28[8] = {0};
// GLOBAL: FF7 0x00a41a30
unsigned char g_YasuiLimitUngarmaxParticleInitFlag = 0;
// GLOBAL: FF7 0x00a41a34
struct GfxTableCursor *g_YasuiLimitUngarmaxResourceContext = 0;
unsigned char g_bssPad_00a41a38[8] = {0};
// GLOBAL: FF7 0x00a41a40
int g_YasuiLimitUngarmaxCurrentTargetActor = 0;
unsigned char g_bssPad_00a41a44[4] = {0};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x007ed460
int g_YasuiLimitUngarmaxAttachTransformMatrix1 = 12288;
unsigned char g_rawData_007ed468[24] = {0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0xd0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6a, 0xff, 0xff, 0xff, 0x00, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x007ed480
unsigned char g_YasuiLimitUngarmaxAttachTransformMatrix2[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00};
unsigned char g_rawData_007ed488[24] = {0x00, 0x04, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0xfd, 0xff, 0xff, 0x00, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x007ed4a0
struct YasuiLimitUngarmaxAttachModelDrawReqPool g_YasuiLimitUngarmaxAttachModelDrawReqPool = {0, {0x88, 0, 0, 0, 0, 0, 0, 0, 0x20, 0, 0, 0}, 0};
unsigned char g_rawData_007ed4b4[4] = {0x88, 0x00, 0x00, 0x00};
unsigned char g_rawData_007ed4b8[8] = {0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x007ed4c0
short g_YasuiLimitUngarmaxTimelineDelays[2] = {8, 10};
unsigned char g_rawData_007ed4c4[4] = {0x0c, 0x00, 0x0e, 0x00};
unsigned char g_YasuiLimitUngarmaxImpactsResourceNames[92] = {
    0x10, 0x00, 0x12, 0x00, 0x16, 0x00, 0x18, 0x00, 0x1a, 0x00, 0x1c, 0x00, 0x1e, 0x00, 0x20, 0x00, 0x23, 0x00, 0x25, 0x00, 0x27, 0x00, 0x29, 0x00,
    0x2b, 0x00, 0x2d, 0x00, 0x2e, 0x00, 0x00, 0x00, 0x41, 0x5f, 0x4d, 0x41, 0x58, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x00, 0x4f, 0x49, 0x54, 0x41,
    0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x00, 0x00, 0x4a, 0x4f, 0x5f, 0x46, 0x49, 0x52, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x41, 0x5f, 0x4d, 0x41,
    0x58, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x00, 0x41, 0x5f, 0x4d, 0x41, 0x58, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00
};
// clang-format on

#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
#pragma intrinsic(abs)


/*
 * When the battle scene exists and g_YasuiLimitUngarmaxResourceContext is zero,
 * initializes a default-duration resource record, builds LIMIT/A_MAX path,
 * and calls Gfx_LoadModelTableIfAbsent to load Angermax resources.
 */
// FUNCTION: FF7 0x00465f70
void __cdecl BattleFx_LimitLoadResUngarmax(void)
{
    char pathScratch[200];
    GfxModelTableBank *context;
    GfxResourceRequest requestRecord;
    int kind;
    GfxResourceRequest *pReq;
    int resourceRows;

    if (g_pBattle3dScene != 0 && g_YasuiLimitUngarmaxResourceContext == 0) {
        kind = 4;
        resourceRows = (int)&g_YasuiLimitUngarmaxResourceDef;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestRecord;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x007ed524
        pReq->resourcePath = (char *)BattleFx_BuildResourcePath("LIMIT/A_MAX", pathScratch);
        Gfx_LoadModelTableIfAbsent(kind, resourceRows, pReq, &g_YasuiLimitUngarmaxResourceContext, context);
    }
}

/*
 * Loads Angermax resources, initializes the draw list head, and begins the effect.
 */
// FUNCTION: FF7 0x00466000
int __cdecl BattleFx_LimitEnterUngarmax(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResUngarmax();
    g_YasuiLimitUngarmaxDrawListHead = (int)&g_GfxPsxPacketScratch;
    BattleFx_LimitBeginUngarmax(targetMask, actorIdx);
    return 0;
}

/*
 * Stores source and target actors and allocates the main Angermax timeline task.
 */
// FUNCTION: FF7 0x00466026
void __cdecl BattleFx_LimitBeginUngarmax(int targetMask, int actorIdx)
{
    struct BattleTaskSlot *task;

    g_YasuiLimitUngarmaxSourceActor = targetMask;
    g_YasuiLimitUngarmaxTargetActor = actorIdx;
    task = BATTLE_NEW_TASK_SLOT(BattleFx_LimitUngarmaxTimelineTask);
    task->ownerSlot = 0;
}

/*
 * Main timeline task driving Angermax impacts and audio across target actors.
 */
// FUNCTION: FF7 0x00466063
void __cdecl BattleFx_LimitUngarmaxTimelineTask(void)
{
    BattleFxUngarmaxHitRingRecord *ring;
    BattleFxUngarmaxTimelineRecord *taskPtr;
    int unusedSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    taskPtr = (BattleFxUngarmaxTimelineRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->step == 8) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[g_YasuiLimitUngarmaxTargetActor].posX), 0x111);
    }
    if (taskPtr->step == g_YasuiLimitUngarmaxTimelineDelays[taskPtr->sequenceIndex]) {
        g_YasuiLimitUngarmaxCurrentTargetActor = g_BattleHitTargetActors[taskPtr->sequenceIndex];
        if (g_YasuiLimitUngarmaxCurrentTargetActor != 0xFF) {
            Battle_AllocateResourceSlot(BattleFx_LimitUngarmaxActorAttachedModelsTask);
            BattleFx_LimitUngarmaxSpawnImpactEffects(g_YasuiLimitUngarmaxCurrentTargetActor);
            ring = (BattleFxUngarmaxHitRingRecord *)Battle_AllocSceneMessage(0xA);
            ring->targetActor = *(short *)&g_YasuiLimitUngarmaxCurrentTargetActor;
            ring->sequenceIndex = taskPtr->sequenceIndex;
            taskPtr->sequenceIndex++;
            Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH2, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[g_YasuiLimitUngarmaxCurrentTargetActor].posX), 0xF);
        } else {
            Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH1, 0x40, 0);
            taskPtr->sequenceIndex = 0xFFFF;
        }
    }
    taskPtr->step++;
}

/*
 * Spawns billboard model, animated sprite, and 15 directional spark particles on target actor bone.
 */
// FUNCTION: FF7 0x004661a9
void __cdecl BattleFx_LimitUngarmaxSpawnImpactEffects(int targetActor)
{
    int actorRadius;
    YasuiLimitUngarmaxImpactSlot *child;
    int sparkIdx;
    int velocity;
    int pitch;

    if ((g_YasuiLimitUngarmaxImpactInitFlag & 1) == 0) {
        g_YasuiLimitUngarmaxImpactInitFlag |= 1;
        g_YasuiLimitUngarmaxBonePosScratch = (YasuiLimitUngarmaxImpactScratch *)Gfx_GetPsxScratchpadPtr(0);
    }
    Battle_GetBoneWorldPos(targetActor, g_BattleActorRecords[targetActor].targetBone, &g_YasuiLimitUngarmaxBonePosScratch->pos.vx);
    actorRadius = g_BattleActorRecords[targetActor].radius;
    g_YasuiLimitUngarmaxBonePosScratch->pos.vx += (short)(rand() % (actorRadius / 2 * 2) - actorRadius / 2);
    g_YasuiLimitUngarmaxBonePosScratch->pos.vy += (short)(rand() % (actorRadius / 2 * 2) - actorRadius / 2);
    g_YasuiLimitUngarmaxBonePosScratch->pos.vz += (short)(rand() % (actorRadius / 2 * 2) - actorRadius / 2);
    child = (YasuiLimitUngarmaxImpactSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitUngarmaxBillboardModelTask);
    child->pos = g_YasuiLimitUngarmaxBonePosScratch->pos;
    child->pushLength = -actorRadius;
    child = (YasuiLimitUngarmaxImpactSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitUngarmaxAnimatedSpriteTask);
    child->pos = g_YasuiLimitUngarmaxBonePosScratch->pos;
    child->pushLength = -actorRadius;
    for (sparkIdx = 0; sparkIdx < 15; sparkIdx++) {
        child = (YasuiLimitUngarmaxImpactSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitUngarmaxFadingPointParticleTask);
        child->pos = g_YasuiLimitUngarmaxBonePosScratch->pos;
        child->color.r = 0xFF;
        child->color.g = rand() & 0xFF;
        child->color.b = 0;
        child->color.cd = 0x62;
        g_YasuiLimitUngarmaxBonePosScratch->dir.vx = rand() & 0xFFF;
        g_YasuiLimitUngarmaxBonePosScratch->dir.vy = g_BattleActorRecords[g_YasuiLimitUngarmaxTargetActor].yaw - 1024;
        if (DEAD_IN_RETAIL != 0) {
            pitch = 0x400 /* 90 deg */;
        } else {
            pitch = rand() % 0x100 + 0x300;
        }
        g_YasuiLimitUngarmaxBonePosScratch->dir.vz = pitch;
        RotMatrix(&g_YasuiLimitUngarmaxBonePosScratch->dir, &g_YasuiLimitUngarmaxBonePosScratch->matrix);
        if (DEAD_IN_RETAIL != 0) {
            velocity = 500;
        } else {
            velocity = rand() % 200 + 300;
        }
        g_YasuiLimitUngarmaxBonePosScratch->dir.vx = velocity;
        g_YasuiLimitUngarmaxBonePosScratch->dir.vy = 0;
        g_YasuiLimitUngarmaxBonePosScratch->dir.vz = 0;
        TransformVectorToInt16(&g_YasuiLimitUngarmaxBonePosScratch->matrix, &g_YasuiLimitUngarmaxBonePosScratch->dir, &child->velocityX);
    }
}

/*
 * Single-frame billboard model draw for Angermax impact effect.
 */
// FUNCTION: FF7 0x004664a9
void __cdecl BattleFx_LimitUngarmaxBillboardModelTask(void)
{
    YasuiLimitUngarmaxImpactSlot *task;

    task = (YasuiLimitUngarmaxImpactSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_YasuiLimitUngarmaxDrawReqPool.modelRes = Gfx_ModelTableGetRenderObject(0, g_YasuiLimitUngarmaxResourceContext);
    BattleFx_InstallBillboardMatrix(&task->pos.vx, 0x800 /* 0.5 */, task->pushLength);
    g_YasuiLimitUngarmaxDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiLimitUngarmaxDrawReqPool, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitUngarmaxDrawListHead);
    if (g_BattleIsPaused == 0) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animated sprite task for Angermax impact explosion effect.
 */
// FUNCTION: FF7 0x0046652d
void __cdecl BattleFx_LimitUngarmaxAnimatedSpriteTask(void)
{
    YasuiLimitUngarmaxImpactSlot *task;

    task = (YasuiLimitUngarmaxImpactSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_YasuiLimitUngarmaxDrawReqPool.spriteDrawReqObject = Gfx_ModelTableGetLoadedRecord(3, g_YasuiLimitUngarmaxResourceContext);
    g_YasuiLimitUngarmaxDrawReqPool.drawObj = Gfx_ModelTableGetSpriteObject(3, g_YasuiLimitUngarmaxResourceContext);
    BattleFx_InstallBillboardMatrix(&task->pos.vx, 0x2000 /* 2.0 */, task->pushLength);
    g_YasuiLimitUngarmaxDrawReqPool.spriteDrawReqFrame = task->frameCount;
    g_YasuiLimitUngarmaxDrawListHead = BattleFx_SubmitSpriteDraw(&g_YasuiLimitUngarmaxDrawReqPool.spriteDrawReqObject, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitUngarmaxDrawListHead);
    if (g_BattleIsPaused == 0 && g_YasuiLimitUngarmaxDrawReqPool.spriteDrawReqObject != 0) {
        task->frameCount++;
        if (task->frameCount >= *(int *)((char *)*(int *)g_YasuiLimitUngarmaxDrawReqPool.spriteDrawReqObject + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Renders and steps a fading point particle with velocity damping and gravity.
 */
// FUNCTION: FF7 0x00466604
void __cdecl BattleFx_LimitUngarmaxFadingPointParticleTask(void)
{
    YasuiLimitUngarmaxImpactSlot *task;
    BattleFxLimitUngarmaxPointPrim *renderBlock;

    task = (YasuiLimitUngarmaxImpactSlot *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_YasuiLimitUngarmaxParticleInitFlag & 1) == 0) {
        g_YasuiLimitUngarmaxParticleInitFlag |= 1;
        g_YasuiLimitUngarmaxParticleScratch = (YasuiLimitUngarmaxProjectScratch *)Gfx_GetPsxScratchpadPtr(0);
    }
    renderBlock = (BattleFxLimitUngarmaxPointPrim *)Gfx_GetPsxPacketScratchPtr();
    renderBlock->packetTag = (renderBlock->packetTag & 0xFFFFFF) | 0x3000000;
    renderBlock->color.cd = 0x60;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    g_YasuiLimitUngarmaxParticleScratch->depth = ProjectPointHardware(&task->pos, renderBlock->xy, &g_YasuiLimitUngarmaxParticleScratch->flagA, &g_YasuiLimitUngarmaxParticleScratch->flagB);
    if ((g_YasuiLimitUngarmaxParticleScratch->flagB & 0x60000) == 0) {
        renderBlock->packetTag = 0x3000000;
        SetDrawColorRGB(0, 0, 0);
        Gfx_ShadePsxVertexColor(&task->color, (task->frameCount << 12) / 5, &renderBlock->color);
        renderBlock->size = 0x20002;
        /* Retail caller prototype (float, float) -> void preserved for /Od argument promotion on `Gfx_SetBlendFactorPair`. */
        ((void(__cdecl *)(float, float))Gfx_SetBlendFactorPair)(GetLastProjectedNdcZ(), GetLastProjectedNdcW());
        Gfx_DrawPsxPrimitive(renderBlock);
    }
    if (g_BattleIsPaused == 0) {
        task->frameCount++;
        if (task->frameCount >= 5) {
            task->ownerSlot = BATTLE_TASK_FREE;
        } else {
            task->pos.vx += task->velocityX;
            task->pos.vy += task->velocityY;
            task->pos.vz += task->velocityZ;
            task->velocityX = task->velocityX * 0xE00 /* 0.875 */ >> 12;
            task->velocityY = (task->velocityY * 0xE00 /* 0.875 */ >> 12) + 10;
            task->velocityZ = task->velocityZ * 0xE00 /* 0.875 */ >> 12;
        }
    }
}

/*
 * Submits two attached gun barrel flash models transformed by the actor attachment matrix.
 */
// FUNCTION: FF7 0x00466807
void __cdecl BattleFx_LimitUngarmaxActorAttachedModelsTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if ((g_YasuiLimitUngarmaxModelInitFlag & 1) == 0) {
        g_YasuiLimitUngarmaxModelInitFlag |= 1;
        g_YasuiLimitUngarmaxScratchMatrix = (struct PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    }
    g_YasuiLimitUngarmaxAttachModelDrawReqPool.modelRes = Gfx_ModelTableGetRenderObject(2, g_YasuiLimitUngarmaxResourceContext);
    g_YasuiLimitUngarmaxAttachModelDrawReqPool.req2 = Gfx_ModelTableGetRenderObject(1, g_YasuiLimitUngarmaxResourceContext);
    Battle3d_GetActorAttachMatrix(g_YasuiLimitUngarmaxTargetActor, -1, (PsxMatrix *)g_BattleActorTransformRecords[g_YasuiLimitUngarmaxTargetActor].auxNode);
    MultiplyMatrix(g_BattleActorTransformRecords[g_YasuiLimitUngarmaxTargetActor].auxNode, &g_YasuiLimitUngarmaxAttachTransformMatrix1, g_YasuiLimitUngarmaxScratchMatrix);
    SetRotMatrix(g_YasuiLimitUngarmaxScratchMatrix);
    SetTransMatrix(g_YasuiLimitUngarmaxScratchMatrix);
    g_YasuiLimitUngarmaxDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiLimitUngarmaxAttachModelDrawReqPool, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitUngarmaxDrawListHead);
    Battle3d_GetActorAttachMatrix(g_YasuiLimitUngarmaxTargetActor, -1, (PsxMatrix *)g_BattleActorTransformRecords[g_YasuiLimitUngarmaxTargetActor].auxNode);
    MultiplyMatrix(g_BattleActorTransformRecords[g_YasuiLimitUngarmaxTargetActor].auxNode, (int *)&g_YasuiLimitUngarmaxAttachTransformMatrix2, g_YasuiLimitUngarmaxScratchMatrix);
    SetRotMatrix(g_YasuiLimitUngarmaxScratchMatrix);
    SetTransMatrix(g_YasuiLimitUngarmaxScratchMatrix);
    g_YasuiLimitUngarmaxDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiLimitUngarmaxAttachModelDrawReqPool.req2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitUngarmaxDrawListHead);
    task->ownerSlot = BATTLE_TASK_FREE;
}
} /* extern "C" */
