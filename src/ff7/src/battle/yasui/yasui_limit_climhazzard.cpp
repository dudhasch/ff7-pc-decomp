// src/ff7/src/battle/yasui/yasui_limit_climhazzard.cpp
// Limit Break [3] (Cloud Lv 2-2): Climhazzard (Japanese: クライムハザード, dev tag: "LIMIT/HAZARD" / Hazard)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Blade Beam (Hazard) limit break effect: a sword-tip beam trail built from shaded quad
//   slices that fade out, with an animated flash sprite and a screen flash.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_boss.h"
#include "ff7/battle_tasks.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/api/sound.h"
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
extern "C" {

// clang-format off
unsigned char g_YasuiLimitClimhazzardSfxGroups[32] = {
    0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// The sound groups SfxRegisterSoundGroups registers: the two records above, then a null entry; 16 bytes.
// GLOBAL: FF7 0x007ed550
unsigned int g_YasuiLimitClimhazzardSoundGroups[4] = {(unsigned int)g_YasuiLimitClimhazzardSfxGroups, (unsigned int)&g_YasuiLimitClimhazzardSfxGroups[16], 0, 0};
// The resource record Gfx_LoadModelTableIfAbsent takes {kind 0x192A39, tim, spt, 0, 0}; its names are its literals.
// GLOBAL: FF7 0x007ed560
unsigned int g_YasuiLimitClimhazzardResourceDef[5] = {0x00192a39, (unsigned int)"CLIMB.TIM", (unsigned int)"HAZARD.SPT", 0, 0};
// GLOBAL: FF7 0x007ed574
int g_YasuiLimitClimhazzardTrailColor = 0x008000FF;
// The two endpoints RotTransPoint transforms (SVECTOR[2]; the first is all zero, so VC5 emits it only inside the pair).
// GLOBAL: FF7 0x007ed578
struct YasuiLimitClimhazzardEndpoints g_YasuiLimitClimhazzardEndpoints = {{0}, 0, {0x18, 0xfc, 0, 0}};
// The sprite descriptor {frame table, field 4, CVECTOR 0x2E808080, frame counter (short)}; 16 bytes.
// GLOBAL: FF7 0x007ed588
struct YasuiLimitClimhazzardSpriteDrawReq g_YasuiLimitClimhazzardSpriteDrawReq = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
// GLOBAL: FF7 0x00a41a48
char g_YasuiLimitClimhazzardTimelineInitFlag = 0;
// GLOBAL: FF7 0x00a41a4c
char g_YasuiLimitClimhazzardTrailInitFlag = 0;
// GLOBAL: FF7 0x00a41a50
int g_YasuiLimitClimhazzardDrawListHead = 0;
// GLOBAL: FF7 0x00a41a54
int g_YasuiLimitClimhazzardTargetMask = 0;
// GLOBAL: FF7 0x00a41a58
unsigned char g_YasuiLimitClimhazzardTrailPoint1[0x8] = {0};

// GLOBAL: FF7 0x00a41a60
unsigned char g_YasuiLimitClimhazzardTrailPoint2[8] = {0};

unsigned char g_bssPad_00a41a68[944] = {0};
// GLOBAL: FF7 0x00a41e18
int g_YasuiLimitClimhazzardSourceActor = 0;
// GLOBAL: FF7 0x00a41e1c
YasuiLimitClimhazzardTrail *g_YasuiLimitClimhazzardTrailScratch = 0;
// GLOBAL: FF7 0x00a41e20
YasuiLimitClimhazzardTimelineScratch *g_YasuiLimitClimhazzardTimelineScratch = 0;
// GLOBAL: FF7 0x00a41e24
struct GfxTableCursor *g_YasuiLimitClimhazzardResourceContext = 0;
// clang-format on

#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
#pragma intrinsic(abs)


/*
 * When the battle scene exists and g_YasuiLimitClimhazzardResourceContext is zero,
 * initializes a default-duration resource record, builds LIMIT/HAZARD path,
 * calls Gfx_LoadModelTableIfAbsent to load Blade Beam resources, and registers sound groups.
 */
// FUNCTION: FF7 0x00466990
void __cdecl BattleFx_LimitLoadResClimhazzard(void)
{
    char pathScratch[200];
    GfxModelTableBank *context;
    GfxResourceRequest requestRecord;
    int kind;
    GfxResourceRequest *pReq;
    int resourceRows;

    if (g_pBattle3dScene != 0 && g_YasuiLimitClimhazzardResourceContext == 0) {
        kind = 1;
        resourceRows = (int)&g_YasuiLimitClimhazzardResourceDef;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestRecord;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x007ed5b0
        pReq->resourcePath = BattleFx_BuildResourcePath("LIMIT/HAZARD", pathScratch);
        Gfx_LoadModelTableIfAbsent(kind, resourceRows, pReq, &g_YasuiLimitClimhazzardResourceContext, context);
        SfxRegisterSoundGroups((struct SoundGroup **)&g_YasuiLimitClimhazzardSoundGroups);
    }
}

/*
 * Loads Hazard resources, initializes the draw list head, and begins the effect.
 */
// FUNCTION: FF7 0x00466a31
int __cdecl BattleFx_LimitEnterClimhazzard(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResClimhazzard();
    g_YasuiLimitClimhazzardDrawListHead = (int)&g_GfxPsxPacketScratch;
    BattleFx_LimitBeginClimhazzard(targetMask, actorIdx);
    return 0;
}

/*
 * Stores target mask and source actor and allocates the main Blade Beam timeline task.
 */
// FUNCTION: FF7 0x00466a57
void __cdecl BattleFx_LimitBeginClimhazzard(int targetMask, int actorIdx)
{
    g_YasuiLimitClimhazzardTargetMask = targetMask;
    g_YasuiLimitClimhazzardSourceActor = actorIdx;
    Battle_AllocateResourceSlot(BattleFx_LimitClimhazzardTimelineTask);
}

/*
 * Main timeline task driving screen flash, audio, animated sprite, and beam trail emission.
 */
// FUNCTION: FF7 0x00466a7a
void __cdecl BattleFx_LimitClimhazzardTimelineTask(void)
{
    BattleTaskSlot *taskPtr;
    int i;
    BattleFxLimitClimhazzardTimeline2 *pRecord;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if ((*(unsigned char *)&g_YasuiLimitClimhazzardTimelineInitFlag & 1) == 0) {
        g_YasuiLimitClimhazzardTimelineInitFlag |= 1;
        g_YasuiLimitClimhazzardTimelineScratch = (YasuiLimitClimhazzardTimelineScratch *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->frameCounter == 0x24) {
        BattleFx_StartScreenFlashRamp(1);
    }
    if (taskPtr->frameCounter == 0x25) {
        pRecord = (BattleFxLimitClimhazzardTimeline2 *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitClimhazzardAnimatedSpriteTask);
        Battle_ComposeMatrixRelativeToCamera((short *)&g_BattleCameraTransform, (void *)g_BattleActorTransformRecords[g_YasuiLimitClimhazzardSourceActor].auxNode, (PsxMatrix *)g_YasuiLimitClimhazzardTimelineScratch->matrix);
        g_YasuiLimitClimhazzardTimelineScratch->vx = 0;
        g_YasuiLimitClimhazzardTimelineScratch->vy = 0;
        g_YasuiLimitClimhazzardTimelineScratch->vz = 0xFE0C;
        TransformVectorToInt16((PsxMatrix *)g_YasuiLimitClimhazzardTimelineScratch->matrix, (SVECTOR *)g_YasuiLimitClimhazzardTimelineScratch, (short *)g_YasuiLimitClimhazzardTimelineScratch);
        pRecord->posX = g_YasuiLimitClimhazzardTimelineScratch->vx + g_YasuiLimitClimhazzardTimelineScratch->transX;
        pRecord->posY = g_YasuiLimitClimhazzardTimelineScratch->vy + g_YasuiLimitClimhazzardTimelineScratch->transY;
        pRecord->posZ = g_YasuiLimitClimhazzardTimelineScratch->vz + g_YasuiLimitClimhazzardTimelineScratch->transZ;
        pRecord->pushLength = 0xFC18;
        BattleFx_FadeOutScreenFlashRamp(5);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&pRecord->posX), 0x12);
    }
    if (taskPtr->frameCounter == 0x38) {
        pRecord = (BattleFxLimitClimhazzardTimeline2 *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitClimhazzardBeamTrailEmitterTask);
        pRecord->ownerSlot = 0;
    }
    if (taskPtr->frameCounter == 0x3B) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[g_YasuiLimitClimhazzardSourceActor].posX), 0x1B);
    }
    if (taskPtr->frameCounter == 0x4F) {
        for (i = 0; i < 0xA; i++) {
            if ((g_YasuiLimitClimhazzardTargetMask & 1 << i) != 0) {
                Battle_PostActorHitOrHurtMessage(i);
            }
        }
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
        Sfx_ReleaseCachedBuffer(0x12, 0xF);
        Sfx_ReleaseCachedBuffer(0x1B, 0xF);
    }
    (taskPtr->frameCounter)++;
}

/*
 * Emits interpolated shaded quad trail slices along the transformed sword endpoints over 15 frames.
 */
// FUNCTION: FF7 0x00466cd2
void __cdecl BattleFx_LimitClimhazzardBeamTrailEmitterTask(void)
{
    int i;
    BattleFxLimitClimhazzardTrail *pEmitter;
    BattleFxLimitClimhazzardBeamEmitter *quadTask;
    int ratio;

    pEmitter = (BattleFxLimitClimhazzardTrail *)BATTLE_CURRENT_TASK_SLOT;
    if ((*(unsigned char *)&g_YasuiLimitClimhazzardTrailInitFlag & 1) == 0) {
        g_YasuiLimitClimhazzardTrailInitFlag |= 1;
        g_YasuiLimitClimhazzardTrailScratch = (YasuiLimitClimhazzardTrail *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    Battle_ComposeMatrixRelativeToCamera((short *)&g_BattleCameraTransform, (void *)g_BattleActorTransformRecords[g_YasuiLimitClimhazzardSourceActor].auxNode, (PsxMatrix *)g_YasuiLimitClimhazzardTrailScratch->matrix);
    SetRotMatrix((PsxMatrix *)g_YasuiLimitClimhazzardTrailScratch->matrix);
    SetTransMatrix(g_YasuiLimitClimhazzardTrailScratch->matrix);
    RotTransPoint(&g_YasuiLimitClimhazzardEndpoints, &g_YasuiLimitClimhazzardTrailScratch->point1DeltaX, &g_YasuiLimitClimhazzardTrailScratch->rotateFlag);
    RotTransPoint(&g_YasuiLimitClimhazzardEndpoints.endpoint2, &g_YasuiLimitClimhazzardTrailScratch->point2DeltaX, &g_YasuiLimitClimhazzardTrailScratch->rotateFlag);
    if (pEmitter->pointIndex != 0) {
        *(Int64Pair *)&g_YasuiLimitClimhazzardTrailScratch->point1BaseX = *(Int64Pair *)&((YasuiLimitClimhazzardTrailPoint1 *)(g_YasuiLimitClimhazzardTrailPoint1))[pEmitter->pointIndex - 1].posX;
        *(Int64Pair *)&g_YasuiLimitClimhazzardTrailScratch->point2BaseX = *(Int64Pair *)&((YasuiLimitClimhazzardTrailPoint1 *)(g_YasuiLimitClimhazzardTrailPoint2))[pEmitter->pointIndex - 1].posX;
        for (i = 1; i <= 4; i++) {
            ratio = (i << 0xC) / 4;
            g_YasuiLimitClimhazzardTrailScratch->point1X = g_YasuiLimitClimhazzardTrailScratch->point1DeltaX * ratio + g_YasuiLimitClimhazzardTrailScratch->point1BaseX * (0x1000 - ratio) >> 0xC;
            g_YasuiLimitClimhazzardTrailScratch->point1Y = g_YasuiLimitClimhazzardTrailScratch->point1DeltaY * ratio + g_YasuiLimitClimhazzardTrailScratch->point1BaseY * (0x1000 - ratio) >> 0xC;
            g_YasuiLimitClimhazzardTrailScratch->point1Z = g_YasuiLimitClimhazzardTrailScratch->point1DeltaZ * ratio + g_YasuiLimitClimhazzardTrailScratch->point1BaseZ * (0x1000 - ratio) >> 0xC;
            g_YasuiLimitClimhazzardTrailScratch->point2X = g_YasuiLimitClimhazzardTrailScratch->point2DeltaX * ratio + g_YasuiLimitClimhazzardTrailScratch->point2BaseX * (0x1000 - ratio) >> 0xC;
            g_YasuiLimitClimhazzardTrailScratch->point2Y = g_YasuiLimitClimhazzardTrailScratch->point2DeltaY * ratio + g_YasuiLimitClimhazzardTrailScratch->point2BaseY * (0x1000 - ratio) >> 0xC;
            g_YasuiLimitClimhazzardTrailScratch->point2Z = g_YasuiLimitClimhazzardTrailScratch->point2DeltaZ * ratio + g_YasuiLimitClimhazzardTrailScratch->point2BaseZ * (0x1000 - ratio) >> 0xC;
            BattleFx_LimitClimhazzardOffsetAlongDirection((Vec3sPacked *)&g_YasuiLimitClimhazzardTrailScratch->point1X, (Vec3sPacked *)&g_YasuiLimitClimhazzardTrailScratch->point2X, 0x3E8, (Vec3sPacked *)&g_YasuiLimitClimhazzardTrailScratch->trailOffsetX);
            ((YasuiLimitClimhazzardTrailPoint1 *)(g_YasuiLimitClimhazzardTrailPoint1))[pEmitter->pointIndex].posX = g_YasuiLimitClimhazzardTrailScratch->point1X + g_YasuiLimitClimhazzardTrailScratch->trailOffsetX;
            ((YasuiLimitClimhazzardTrailPoint1 *)(g_YasuiLimitClimhazzardTrailPoint1))[pEmitter->pointIndex].posY = g_YasuiLimitClimhazzardTrailScratch->point1Y + g_YasuiLimitClimhazzardTrailScratch->trailOffsetY;
            ((YasuiLimitClimhazzardTrailPoint1 *)(g_YasuiLimitClimhazzardTrailPoint1))[pEmitter->pointIndex].posZ = g_YasuiLimitClimhazzardTrailScratch->point1Z + g_YasuiLimitClimhazzardTrailScratch->trailOffsetZ;
            ((YasuiLimitClimhazzardTrailPoint1 *)(g_YasuiLimitClimhazzardTrailPoint2))[pEmitter->pointIndex].posX = g_YasuiLimitClimhazzardTrailScratch->point2X + g_YasuiLimitClimhazzardTrailScratch->trailOffsetX * 2;
            ((YasuiLimitClimhazzardTrailPoint1 *)(g_YasuiLimitClimhazzardTrailPoint2))[pEmitter->pointIndex].posY = g_YasuiLimitClimhazzardTrailScratch->point2Y + g_YasuiLimitClimhazzardTrailScratch->trailOffsetY * 2;
            ((YasuiLimitClimhazzardTrailPoint1 *)(g_YasuiLimitClimhazzardTrailPoint2))[pEmitter->pointIndex].posZ = g_YasuiLimitClimhazzardTrailScratch->point2Z + g_YasuiLimitClimhazzardTrailScratch->trailOffsetZ * 2;
            quadTask = (BattleFxLimitClimhazzardBeamEmitter *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitClimhazzardShadedQuadTrailTask);
            quadTask->ownerSlot = i + pEmitter->frameCount * 4 + -1;
            quadTask->frameCounter = 4 - i;
            quadTask->trailPointPtr = (pEmitter->pointIndex << 4) + (int)&g_YasuiLimitClimhazzardTrailPoint1;
            (pEmitter->pointIndex)++;
        }
    } else {
        ((YasuiLimitClimhazzardTrailPoint1 *)(g_YasuiLimitClimhazzardTrailPoint1))[pEmitter->pointIndex].posX = *(short *)&g_YasuiLimitClimhazzardTrailScratch->point1DeltaX;
        ((YasuiLimitClimhazzardTrailPoint1 *)(g_YasuiLimitClimhazzardTrailPoint1))[pEmitter->pointIndex].posY = *(short *)&g_YasuiLimitClimhazzardTrailScratch->point1DeltaY;
        ((YasuiLimitClimhazzardTrailPoint1 *)(g_YasuiLimitClimhazzardTrailPoint1))[pEmitter->pointIndex].posZ = *(short *)&g_YasuiLimitClimhazzardTrailScratch->point1DeltaZ;
        ((YasuiLimitClimhazzardTrailPoint1 *)(g_YasuiLimitClimhazzardTrailPoint2))[pEmitter->pointIndex].posX = *(short *)&g_YasuiLimitClimhazzardTrailScratch->point2DeltaX;
        ((YasuiLimitClimhazzardTrailPoint1 *)(g_YasuiLimitClimhazzardTrailPoint2))[pEmitter->pointIndex].posY = *(short *)&g_YasuiLimitClimhazzardTrailScratch->point2DeltaY;
        ((YasuiLimitClimhazzardTrailPoint1 *)(g_YasuiLimitClimhazzardTrailPoint2))[pEmitter->pointIndex].posZ = *(short *)&g_YasuiLimitClimhazzardTrailScratch->point2DeltaZ;
        (pEmitter->pointIndex)++;
    }
    (pEmitter->frameCount)++;
    if (pEmitter->frameCount >= 0xF) {
        pEmitter->pointIndex = 0xFFFF;
    }
}

/*
 * Vector math helper computing a point offset along the direction from startPt to endPt.
 */
// FUNCTION: FF7 0x0046719f
void __cdecl BattleFx_LimitClimhazzardOffsetAlongDirection(Vec3sPacked *startPt, Vec3sPacked *endPt, int distanceFixed, Vec3sPacked *outPt)
{
    VECTOR outVec;

    outVec.vx = endPt->vx - startPt->vx;
    outVec.vy = endPt->vy - startPt->vy;
    outVec.vz = endPt->vz - startPt->vz;
    NormalizeVectorFixed(&outVec, &outVec);
    outPt->vx = startPt->vx + (outVec.vx * distanceFixed >> 0xC) - endPt->vx;
    outPt->vy = startPt->vy + (outVec.vy * distanceFixed >> 0xC) - endPt->vy;
    outPt->vz = startPt->vz + (outVec.vz * distanceFixed >> 0xC) - endPt->vz;
}

/*
 * Renders one shaded quad trail segment with vertex colors interpolated over 40 frames.
 */
// FUNCTION: FF7 0x00467256
void __cdecl BattleFx_LimitClimhazzardShadedQuadTrailTask(void)
{
    BattleFxClimhazzardDrawNode *node;
    int clipBits;
    BattleFxLimitClimhazzardQuadRecord *quad;
    int padSlot; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int clipA;
    int nextRatio;
    int colorVal;
    int shadeCur;
    int zDepth;

    quad = (BattleFxLimitClimhazzardQuadRecord *)BATTLE_CURRENT_TASK_SLOT;
    nextRatio = (quad->phase + 1 << 0xC) / 0x28;
    shadeCur = (quad->phase << 0xC) / 0x28;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    node = (BattleFxClimhazzardDrawNode *)g_YasuiLimitClimhazzardDrawListHead;
    zDepth = Gfx_TransformQuadReturnDepth((void *)(quad->vertexBase - 0x10), (SVECTOR *)(quad->vertexBase - 8), (SVECTOR *)(quad->vertexBase), (SVECTOR *)(quad->vertexBase + 8), &clipA, &clipBits);
    if ((clipBits & 0x60000) == 0) {
        node->tagWord = 0x8000000;
        Gfx_ShadePsxVertexColor((CVECTOR *)&g_YasuiLimitClimhazzardTrailColor, nextRatio, (CVECTOR *)&colorVal);
        node->color1 = colorVal | 0x3A000000;
        node->color0 = node->color1;
        Gfx_ShadePsxVertexColor((CVECTOR *)&g_YasuiLimitClimhazzardTrailColor, shadeCur, (CVECTOR *)&colorVal);
        node->color3 = colorVal;
        node->color2 = colorVal;
        Gfx_DrawPsxPrimitive(node);
    }
    if (g_BattleIsPaused == 0) {
        quad->phase += 4;
        if (quad->phase >= 0x28) {
            quad->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Animated sprite task for Blade Beam flash / explosion effect.
 */
// FUNCTION: FF7 0x004673aa
void __cdecl BattleFx_LimitClimhazzardAnimatedSpriteTask(void)
{
    BattleFxLimitClimhazzardSpriteTask *task;

    task = (BattleFxLimitClimhazzardSpriteTask *)BATTLE_CURRENT_TASK_SLOT;
    (g_YasuiLimitClimhazzardSpriteDrawReq.animBlock) = Gfx_ModelTableGetLoadedRecord(0, g_YasuiLimitClimhazzardResourceContext);
    g_YasuiLimitClimhazzardSpriteDrawReq.drawObj = Gfx_ModelTableGetSpriteObject(0, g_YasuiLimitClimhazzardResourceContext);
    BattleFx_InstallBillboardMatrix(&task->posX, 0x1800, task->pushLength);
    g_YasuiLimitClimhazzardSpriteDrawReq.frame = task->frame;
    g_YasuiLimitClimhazzardDrawListHead = BattleFx_SubmitSpriteDraw(&g_YasuiLimitClimhazzardSpriteDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitClimhazzardDrawListHead);
    if (g_BattleIsPaused == 0 && (g_YasuiLimitClimhazzardSpriteDrawReq.animBlock) != 0) {
        (task->frame)++;
        if (task->frame >= *(int *)((char *)*(int *)(g_YasuiLimitClimhazzardSpriteDrawReq.animBlock) + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
