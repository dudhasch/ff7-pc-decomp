// src/ff7/src/battle/yasui/yasui_limit_satellite_beam.cpp
// Limit Break [11] (Barret Lv 3-1): Satellite Beam (Japanese: サテライトビーム, dev tag: "LIMIT/S_BEAM" / SBeam)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle effects for Barret's Satellite Beam limit break: a targeting phase with
//   crosshair lines and reticle models on each enemy, then scattered beam columns and an orbital
//   impact with shockwaves, smoke and debris.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_boss.h"
#include "lib/codegen.h"
#include "ff7/battle_tasks.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "ff7/enums/main.h"
#include "ff7/enums/battle.h"
extern "C" {

struct BattleOrthoMatrix;
struct BattleFxModelDraw;

#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
#pragma intrinsic(abs)

// clang-format off

// GLOBAL: FF7 0x007edf08
unsigned int g_YasuiLimitSatelliteBeamResourceTable[45] = {0x006440b4, (unsigned int)"SITE.RSD", 0x00000000, 0x00000000, 0x00000000, 0x006400b1, (unsigned int)"SBEAM.RSD", 0x00000000, 0x00000000, 0x00000000, 0x006400b1, (unsigned int)"IMPULSE1.RSD", 0x00000000, 0x00000000, 0x00000000, 0x006400b1, (unsigned int)"IMPULSE2.RSD", 0x00000000, 0x00000000, 0x00000000, 0x00196a39, (unsigned int)"S_BEAM00.TIM", (unsigned int)"LOCK_ON.SPT", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"S_BEAM00.TIM", (unsigned int)"ROCKS.SPT", 0x00000000, 0x00000000, 0x006400b1, (unsigned int)"S_BEAM.RSD", 0x00000000, 0x00000000, 0x00000000, 0x006400b1, (unsigned int)"CSBOMBER.RSD", 0x00000000, 0x00000000, 0x00000000, 0x006400b1, (unsigned int)"NEKKOU.RSD", 0x00000000, 0x00000000, 0x00000000};

// GLOBAL: FF7 0x007edfc0
struct YasuiLimitSatelliteBeamModelDrawReq g_YasuiLimitSatelliteBeamModelDrawReq = {0, 160, {0, 0, 0, 0, 0x20, 0, 0, 0}};

// GLOBAL: FF7 0x007edfd0
struct YasuiLimitSatelliteBeamTransformMatrix g_YasuiLimitSatelliteBeamTransformMatrix = {{0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2}, 0, 0, 0};

// GLOBAL: FF7 0x007edff0
struct YasuiLimitSatelliteBeamSpriteDrawReq g_YasuiLimitSatelliteBeamSpriteDrawReq = {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}};

// GLOBAL: FF7 0x007ee000
struct YasuiLimitSatelliteBeamScaleMatrix g_YasuiLimitSatelliteBeamScaleMatrix = {2048, {0}, 2048, {0}, 2048, 0, 0, 0, {0}, 0, 0};

// GLOBAL: FF7 0x007ee024
short g_YasuiLimitSatelliteBeamElevation = 5000;

// GLOBAL: FF7 0x007ee028
struct YasuiLimitSatelliteBeamLineColorParams1 g_YasuiLimitSatelliteBeamLineColorParams1 = {{0, 0, 0, 3, 0xc0, 0, 0, 0x62}, 0, 0};

// GLOBAL: FF7 0x007ee034
struct YasuiLimitSatelliteBeamLineCoordParams1 g_YasuiLimitSatelliteBeamLineCoordParams1 = {640, 332};

unsigned char g_rawData_007ee038[16] = {0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x62, 0x00, 0x00, 0x00, 0x00, 0x80, 0x02, 0x4c, 0x01};

// GLOBAL: FF7 0x007ee048
struct YasuiLimitSatelliteBeamLineColorParams2 g_YasuiLimitSatelliteBeamLineColorParams2 = {0, {0xa8, 0, 0, 0, 0, 0, 0, 0, 0x20, 0, 0, 0}, 0, {0x88, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}, 0, {0x88, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}, 0, 0};

// GLOBAL: FF7 0x007ee080
struct YasuiLimitSatelliteBeamLineCoordParams2 g_YasuiLimitSatelliteBeamLineCoordParams2 = {{0x80}, -128, -128, 44};

// GLOBAL: FF7 0x007ee088
struct YasuiLimitSatelliteBeamMeshDescriptor g_YasuiLimitSatelliteBeamMeshDescriptor = {0, {0x28, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}, 0};

unsigned char g_rawData_007ee09c[4] = {0xa8, 0x00, 0x00, 0x00};

unsigned char g_rawData_007ee0a0[8] = {0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};

// GLOBAL: FF7 0x00a54578
PsxMatrix *g_YasuiLimitSatelliteBeamScratchMatrixPtr = 0;

// GLOBAL: FF7 0x00a5457c
int g_YasuiLimitSatelliteBeamDrawHandle = 0;

// GLOBAL: FF7 0x00a54580
int g_YasuiLimitSatelliteBeamTargetMask = 0;

// GLOBAL: FF7 0x00a54584
int g_YasuiLimitSatelliteBeamTimelineDuration = 0;

// GLOBAL: FF7 0x00a54588
char g_YasuiLimitSatelliteBeamScratchAllocated = 0;

// GLOBAL: FF7 0x00a5458c
int g_YasuiLimitSatelliteBeamYawDirection = 0;

// GLOBAL: FF7 0x00a54590
int g_YasuiLimitSatelliteBeamTargetCentroidX = 0;

// GLOBAL: FF7 0x00a54594
short g_YasuiLimitSatelliteBeamTargetCentroidZ = 0;

// GLOBAL: FF7 0x00a54598
struct GfxTableCursor *g_YasuiLimitSatelliteBeamLoadedResTable = 0;

unsigned char g_bssPad_00a5459c[4] = {0};

// GLOBAL: FF7 0x00a545a0
int g_YasuiLimitSatelliteBeamBlastRotationAngles = 0;

unsigned char g_bssPad_00a545a4[4] = {0};

// GLOBAL: FF7 0x00a545a8
short g_YasuiLimitSatelliteBeamPuffTransformMatrix = 0;

unsigned char g_bssPad_00a545ac[4] = {0};

// GLOBAL: FF7 0x00a545b0
short g_YasuiLimitSatelliteBeamPuffScaleY = 0;

unsigned char g_bssPad_00a545b4[4] = {0};

// GLOBAL: FF7 0x00a545b8
struct YasuiLimitSatelliteBeamPuffMatrixData g_YasuiLimitSatelliteBeamPuffMatrixData = {0};

// GLOBAL: FF7 0x00a545c8
short g_YasuiLimitSatelliteBeamColumnTransformMatrix = 0;

unsigned char g_bssPad_00a545cc[4] = {0};

// GLOBAL: FF7 0x00a545d0
short g_YasuiLimitSatelliteBeamColumnScaleY = 0;

unsigned char g_bssPad_00a545d4[4] = {0};

// GLOBAL: FF7 0x00a545d8
struct YasuiLimitSatelliteBeamColumnMatrixData g_YasuiLimitSatelliteBeamColumnMatrixData = {0};
// clang-format on

/*
 * Loads resource files for Barret's Satellite Beam limit break effect.
 */
// FUNCTION: FF7 0x0046ff00
void __cdecl BattleFx_LimitLoadResSatelliteBeam(void)
{
    char pathScratch[200];
    GfxModelTableBank *context;
    GfxResourceRequest requestRecord;
    int kind;
    GfxResourceRequest *pReq;
    int resourceRows;

    if (g_pBattle3dScene != 0 && g_YasuiLimitSatelliteBeamLoadedResTable == 0) {
        kind = 9;
        resourceRows = (int)&g_YasuiLimitSatelliteBeamResourceTable;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestRecord;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x007ee140
        pReq->resourcePath = BattleFx_BuildResourcePath("LIMIT/S_BEAM", pathScratch);
        Gfx_LoadModelTableIfAbsent(kind, resourceRows, pReq, &g_YasuiLimitSatelliteBeamLoadedResTable, context);
    }
}

/*
 * Initializes and begins the Satellite Beam limit effect for the given targets.
 */
// FUNCTION: FF7 0x0046ff90
int __cdecl BattleFx_LimitEnterSatelliteBeam(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResSatelliteBeam();
    BattleFx_LimitBeginSatelliteBeam(targetMask, actorIdx);
    return 0;
}

/*
 * Computes centroid and duration, and allocates the main Satellite Beam timeline task.
 */
// FUNCTION: FF7 0x0046ffac
void __cdecl BattleFx_LimitBeginSatelliteBeam(int targetMask, int actorIdx)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int q;

    g_YasuiLimitSatelliteBeamTargetMask = targetMask;
    q = Battle_PopCountActorMask(g_YasuiLimitSatelliteBeamTargetMask);
    g_YasuiLimitSatelliteBeamTimelineDuration = (q - 1) * 5 + 23;
    Battle_GetActorMaskCentroid(targetMask, (short *)&g_YasuiLimitSatelliteBeamTargetCentroidX);
    Battle_AllocateResourceSlot(BattleFx_LimitSatelliteBeamTimelineTask);
    g_YasuiLimitSatelliteBeamYawDirection = g_BattleActorRecords[actorIdx].yaw + 1024 < 2048 ? 0xFFFFFF00 : 0x100;
}

/*
 * Drives the initial target lock-on phase and camera timing for Satellite Beam.
 */
// FUNCTION: FF7 0x00470031
void __cdecl BattleFx_LimitSatelliteBeamTimelineTask(void)
{
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleTaskSlot *task;
    struct BattleTaskSlot *spawned;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 18) {
        spawned = BATTLE_NEW_TASK_SLOT(BattleFx_LimitSatelliteBeamTargetEmitterTask);
        spawned->ownerSlot = 0;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, 0x40, 0x246);
    }
    if (task->frameCounter == 90) {
        Battle_CreateBitPatternCallbackTask(BATTLE_ACTOR_MASK_ENEMIES, 0, 5, BattleFx_LimitSpawnSatelliteBeamPerTarget);
        spawned = BATTLE_NEW_TASK_SLOT(BattleFx_LimitSatelliteBeamScatteredColumnEmitterTask);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/*
 * Emits per-target targeting reticle tasks for each targeted enemy.
 */
// FUNCTION: FF7 0x004700f7
void __cdecl BattleFx_LimitSatelliteBeamTargetEmitterTask(void)
{
    BattleFxLimitSatelliteBeamEmitter *pEmitter;
    BattleFxLimitSatelliteBeamTimeline *reticleAddress;

    pEmitter = (BattleFxLimitSatelliteBeamEmitter *)BATTLE_CURRENT_TASK_SLOT;
    g_YasuiLimitSatelliteBeamLineColorParams1.limitSBeamLineColorParams1Word8 = *(short *)&g_BattleViewportOriginX;
    g_YasuiLimitSatelliteBeamLineColorParams1.limitSBeamLineColorParams1Word10 = *(short *)&g_BattleViewportOriginY;
    (g_YasuiLimitSatelliteBeamLineCoordParams1.viewportWidth) = *(short *)&g_BattleViewportWidth;
    g_YasuiLimitSatelliteBeamLineCoordParams1.limitSBeamLineCoordViewportHeight = *(short *)&g_BattleSceneViewportHeight;
    Gfx_SetBlendFactorPair(0, FLOAT_ONE_BITS);
    Gfx_DrawPsxPrimitive(&g_YasuiLimitSatelliteBeamLineColorParams1);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (g_YasuiLimitSatelliteBeamTargetMask >= 1 << pEmitter->maskBit && pEmitter->frameCounter % 5 == 0) {
        while ((g_YasuiLimitSatelliteBeamTargetMask & 1 << pEmitter->maskBit) == 0) {
            (pEmitter->maskBit)++;
        }
        reticleAddress = (BattleFxLimitSatelliteBeamTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitSatelliteBeamTargetTimelineTask);
        reticleAddress->value = pEmitter->frameCounter;
        Battle_GetBoneWorldPos(pEmitter->maskBit, g_BattleActorRecords[pEmitter->maskBit].targetBone, &reticleAddress->posX);
        (pEmitter->maskBit)++;
    }
    if (pEmitter->frameCounter >= 65) {
        pEmitter->maskBit = 0xFFFF;
    }
    (pEmitter->frameCounter)++;
}

/*
 * Coordinates crosshair line drawing and quadrant model spawning per target.
 */
// FUNCTION: FF7 0x0047025a
void __cdecl BattleFx_LimitSatelliteBeamTargetTimelineTask(void)
{
    BattleFxLimitSatelliteBeamTimeline *timeline;
    PsxMatrix *dataBlock;
    Vec3sPacked *offsetVec;
    BattleFxLimitSatelliteBeamTimeline *newTask;

    timeline = (BattleFxLimitSatelliteBeamTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    dataBlock = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    offsetVec = (Vec3sPacked *)(dataBlock + 1);
    if (timeline->step == 0) {
        newTask = (BattleFxLimitSatelliteBeamTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitSatelliteBeamCrosshairLineTask);
        newTask->value = timeline->value + timeline->step;
        TransposeMatrix((void *)g_BattleCameraTransform.matrix, dataBlock);
        offsetVec->vx = g_YasuiLimitSatelliteBeamScaleMatrix.targetOffsetX - (g_BattleCameraTransform.translation[0]);
        offsetVec->vy = g_YasuiLimitSatelliteBeamScaleMatrix.targetOffsetY - (g_BattleCameraTransform.translation[1]);
        offsetVec->vz = g_YasuiLimitSatelliteBeamElevation - (g_BattleCameraTransform.translation[2]);
        TransformVectorToInt16(dataBlock, (void *)offsetVec, &newTask->posX);
        *(Int64Pair *)((char *)newTask + 0xC) = *(Int64Pair *)&timeline->posX;
    }
    if (timeline->step == 5) {
        newTask = (BattleFxLimitSatelliteBeamTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitSatelliteBeamQuadrantModelTask);
        newTask->value = timeline->value + timeline->step;
        *(Int64Pair *)&newTask->posX = *(Int64Pair *)&timeline->posX;
    }
    if (timeline->step == 5) {
        newTask = (BattleFxLimitSatelliteBeamTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitSatelliteBeamProjectedSpritePairTask);
        newTask->value = timeline->value + timeline->step;
        *(Int64Pair *)&newTask->posX = *(Int64Pair *)&timeline->posX;
        newTask->startU = rand() % 40 + 80;
        newTask->startV = rand() % 30 + 60;
        timeline->value = BATTLE_TASK_FREE;
    }
    (timeline->step)++;
}

/*
 * Renders converging laser targeting crosshair lines around the target actor.
 */
// FUNCTION: FF7 0x00470438
void __cdecl BattleFx_LimitSatelliteBeamCrosshairLineTask(void)
{
    BattleFxSatelliteBeamProjectedPair *screenPointPtr;
    BattleFxSatelliteBeamLineState *lineState;
    BattleFxBeamCrosshairLine *lineTask;
    int age;
    int *bitsOut;
    int extent;
    Vec3sPacked *targetPoint;
    int clipLeft;
    int bottomY;
    int clipTop;
    int clipRight;
    int *screenDepth;
    int lerpFactor;

    lineTask = (BattleFxBeamCrosshairLine *)BATTLE_CURRENT_TASK_SLOT;
    targetPoint = (Vec3sPacked *)Gfx_GetPsxScratchpadPtr(0);
    screenDepth = (int *)((char *)targetPoint + 8);
    bitsOut = screenDepth + 1;
    screenPointPtr = (BattleFxSatelliteBeamProjectedPair *)(bitsOut + 1);
    age = lineTask->age;
    if (age < 5) {
        lerpFactor = (age << 12) / 5;
        targetPoint->vx = lineTask->targetX * lerpFactor + lineTask->currentX * (4096 - lerpFactor) >> 12;
        lineTask->currentX = targetPoint->vx;
        targetPoint->vy = lineTask->targetY * lerpFactor + lineTask->currentY * (4096 - lerpFactor) >> 12;
        lineTask->currentY = targetPoint->vy;
        targetPoint->vz = lineTask->targetZ * lerpFactor + lineTask->currentZ * (4096 - lerpFactor) >> 12;
        lineTask->currentZ = targetPoint->vz;
        extent = g_BattleViewportWidth;
    } else {
        age -= 5;
        if (age < 5) {
            targetPoint->vx = lineTask->targetX;
            targetPoint->vy = lineTask->targetY;
            targetPoint->vz = lineTask->targetZ;
            extent = g_BattleViewportWidth + (0x10 - g_BattleViewportWidth) * age / 5;
        } else if (lineTask->ownerSlot + lineTask->age < 65) {
            targetPoint->vx = lineTask->targetX;
            targetPoint->vy = lineTask->targetY;
            targetPoint->vz = lineTask->targetZ;
            extent = 0x10;
        } else {
            lineTask->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    SetRotMatrix((void *)g_BattleCameraTransform.matrix);
    SetTransMatrix((void *)g_BattleCameraTransform.matrix);
    ProjectPointHardware(targetPoint, (short *)screenPointPtr, screenDepth, bitsOut);
    if ((*bitsOut & 0x60000) == 0) {
        lineState = (BattleFxSatelliteBeamLineState *)Gfx_GetPsxPacketScratchPtr();
        lineState->commandWord = 0x3000000;
        lineState->colorWord = 0x40FFFFFF;
        lineState->posX = screenPointPtr->projX;
        lineState->x = lineState->posX;
        if (screenPointPtr->projY - extent < g_BattleViewportOriginY) {
            clipTop = g_BattleViewportOriginY;
        } else {
            clipTop = screenPointPtr->projY - extent;
        }
        lineState->y = clipTop;
        if (screenPointPtr->projY + extent > (*(int *)&g_BattleViewportMaxY)) {
            bottomY = (*(int *)&g_BattleViewportMaxY);
        } else {
            bottomY = screenPointPtr->projY + extent;
        }
        lineState->posY = bottomY;
        Gfx_SetBlendFactorPair(0, FLOAT_ONE_BITS);
        Gfx_DrawPsxPrimitive(lineState);
        lineState->commandWord = 0x3000000;
        lineState->colorWord = 0x40FFFFFF;
        lineState->posY = screenPointPtr->projY;
        lineState->y = lineState->posY;
        if (screenPointPtr->projX - extent < g_BattleViewportOriginX) {
            clipLeft = g_BattleViewportOriginX;
        } else {
            clipLeft = screenPointPtr->projX - extent;
        }
        lineState->x = clipLeft;
        if (screenPointPtr->projX + extent > g_BattleViewportMaxX) {
            clipRight = g_BattleViewportMaxX;
        } else {
            clipRight = screenPointPtr->projX + extent;
        }
        lineState->posX = clipRight;
        Gfx_SetBlendFactorPair(0, FLOAT_ONE_BITS);
        Gfx_DrawPsxPrimitive(lineState);
    }
    if (g_BattleIsPaused == 0) {
        (lineTask->age)++;
    }
}

/*
 * Renders rotating quadrant reticle models around the locked-on target.
 */
// FUNCTION: FF7 0x004707cc
void __cdecl BattleFx_LimitSatelliteBeamQuadrantModelTask(void)
{
    int *screenDepth;
    BattleFxSatelliteBeamProjectedPair *pScreenPoint;
    int quadX;
    int *bitsOut;
    BattleTaskSlot *pSlot;
    int frame;
    int *geomOfsY;
    int *centerXAddr;
    int quadY;

    pSlot = BATTLE_CURRENT_TASK_SLOT;
    (g_YasuiLimitSatelliteBeamModelDrawReq.quadrantModelResource) = Gfx_ModelTableGetRenderObject(0, g_YasuiLimitSatelliteBeamLoadedResTable);
    screenDepth = (int *)Gfx_GetPsxScratchpadPtr(0);
    bitsOut = screenDepth + 1;
    pScreenPoint = (BattleFxSatelliteBeamProjectedPair *)(bitsOut + 1);
    centerXAddr = (int *)(pScreenPoint + 1);
    geomOfsY = centerXAddr + 1;
    frame = pSlot->frameCounter;
    if (frame < 5) {
        quadX = frame * -80 / 5 + 80;
        quadY = frame * -60 / 5 + 60;
    } else {
        frame -= 5;
        if (frame < 2) {
            quadX = 0;
            quadY = 0;
        } else {
            frame -= 2;
            if (frame < 6) {
                if ((frame & 1) == 1) {
                    if (g_BattleIsPaused == 0) {
                        (pSlot->frameCounter)++;
                    }
                    return;
                }
                quadX = 0;
                quadY = 0;
            } else if (pSlot->ownerSlot + pSlot->frameCounter < 65) {
                quadX = 0;
                quadY = 0;
            } else {
                pSlot->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    SetRotMatrix((void *)g_BattleCameraTransform.matrix);
    SetTransMatrix((void *)g_BattleCameraTransform.matrix);
    ProjectPointHardware(pSlot->data, (short *)pScreenPoint, screenDepth, bitsOut);
    if ((*bitsOut & 0x60000) == 0) {
        GetGeomOffset(centerXAddr, geomOfsY);
        if (g_BattleUiScalingMode == 0) {
            *centerXAddr = 0xA0;
            *geomOfsY = 0x78;
        } else {
            *centerXAddr = 0x140;
            *geomOfsY = 0xF0;
        }
        pScreenPoint->projX -= *(short *)centerXAddr;
        pScreenPoint->projY -= *(short *)geomOfsY;
        pScreenPoint->projX = pScreenPoint->projX / g_BattleUiScale;
        pScreenPoint->projY = pScreenPoint->projY / g_BattleUiScale;
        g_YasuiLimitSatelliteBeamTransformMatrix.translationX = pScreenPoint->projX + quadX << 3;
        g_YasuiLimitSatelliteBeamTransformMatrix.translationY = pScreenPoint->projY - quadY << 3;
        g_YasuiLimitSatelliteBeamTransformMatrix.translationDepth = GetGeomScreen() << 3;
        g_YasuiLimitSatelliteBeamModelDrawReq.drawFlags = 0xA0;
        SetRotMatrix((PsxMatrix *)&g_YasuiLimitSatelliteBeamTransformMatrix);
        SetTransMatrix((PsxMatrix *)&g_YasuiLimitSatelliteBeamTransformMatrix);
        g_YasuiLimitSatelliteBeamDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiLimitSatelliteBeamModelDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_YasuiLimitSatelliteBeamDrawHandle);
        g_YasuiLimitSatelliteBeamTransformMatrix.translationX = pScreenPoint->projX - quadX << 3;
        g_YasuiLimitSatelliteBeamModelDrawReq.drawFlags |= 1;
        SetTransMatrix((PsxMatrix *)&g_YasuiLimitSatelliteBeamTransformMatrix);
        g_YasuiLimitSatelliteBeamDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiLimitSatelliteBeamModelDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_YasuiLimitSatelliteBeamDrawHandle);
        g_YasuiLimitSatelliteBeamTransformMatrix.translationY = pScreenPoint->projY + quadY << 3;
        g_YasuiLimitSatelliteBeamModelDrawReq.drawFlags |= 2;
        SetRotMatrix((PsxMatrix *)&g_YasuiLimitSatelliteBeamTransformMatrix);
        SetTransMatrix((PsxMatrix *)&g_YasuiLimitSatelliteBeamTransformMatrix);
        g_YasuiLimitSatelliteBeamDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiLimitSatelliteBeamModelDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_YasuiLimitSatelliteBeamDrawHandle);
        g_YasuiLimitSatelliteBeamTransformMatrix.translationX = pScreenPoint->projX + quadX << 3;
        g_YasuiLimitSatelliteBeamModelDrawReq.drawFlags &= 0xFFFFFFFE;
        SetRotMatrix((PsxMatrix *)&g_YasuiLimitSatelliteBeamTransformMatrix);
        SetTransMatrix((PsxMatrix *)&g_YasuiLimitSatelliteBeamTransformMatrix);
        g_YasuiLimitSatelliteBeamDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiLimitSatelliteBeamModelDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_YasuiLimitSatelliteBeamDrawHandle);
    }
    if (g_BattleIsPaused == 0) {
        (pSlot->frameCounter)++;
    }
}

/*
 * Renders paired billboard sprites projected at the targeting point.
 */
// FUNCTION: FF7 0x00470b8e
void __cdecl BattleFx_LimitSatelliteBeamProjectedSpritePairTask(void)
{
    int frameIndex;
    BattleFxSatelliteBeamProjectedPair *screenPosition;
    int *currDepthOut;
    BattleFxSatelliteBeamSpriteRecord *taskPtr;
    int *projFlags;
    int offsetX;
    int offsetY;
    int *geomOfsX;
    int *geomY;

    taskPtr = (BattleFxSatelliteBeamSpriteRecord *)BATTLE_CURRENT_TASK_SLOT;
    (g_YasuiLimitSatelliteBeamSpriteDrawReq.pairedSpriteResource) = Gfx_ModelTableGetLoadedRecord(4, g_YasuiLimitSatelliteBeamLoadedResTable);
    g_YasuiLimitSatelliteBeamSpriteDrawReq.drawObj = Gfx_ModelTableGetSpriteObject(4, g_YasuiLimitSatelliteBeamLoadedResTable);
    screenPosition = (BattleFxSatelliteBeamProjectedPair *)Gfx_GetPsxScratchpadPtr(0);
    currDepthOut = (int *)(screenPosition + 1);
    projFlags = currDepthOut + 1;
    geomOfsX = projFlags + 1;
    geomY = geomOfsX + 1;
    frameIndex = taskPtr->frameCount;
    if (frameIndex < 5) {
        offsetX = taskPtr->startU + (0 - taskPtr->startU) * frameIndex / 5;
        offsetY = taskPtr->startV + (0 - taskPtr->startV) * frameIndex / 5;
        offsetX -= SinFixed((frameIndex << 11) / 5) * 0x32 >> 12;
        g_YasuiLimitSatelliteBeamScaleMatrix.battleLimitSBeamProjectedScale = frameIndex * -0x800 / 5 + 0x1000;
        g_YasuiLimitSatelliteBeamScaleMatrix.limitSBeamScaleMatrixWord8 = g_YasuiLimitSatelliteBeamScaleMatrix.battleLimitSBeamProjectedScale;
        (g_YasuiLimitSatelliteBeamScaleMatrix.word8Copy) = g_YasuiLimitSatelliteBeamScaleMatrix.limitSBeamScaleMatrixWord8;
    } else if (taskPtr->timer + taskPtr->frameCount < 65) {
        offsetX = 0;
        offsetY = 0;
        g_YasuiLimitSatelliteBeamScaleMatrix.battleLimitSBeamProjectedScale = 0x800 /* 0.5 */;
        g_YasuiLimitSatelliteBeamScaleMatrix.limitSBeamScaleMatrixWord8 = g_YasuiLimitSatelliteBeamScaleMatrix.battleLimitSBeamProjectedScale;
        (g_YasuiLimitSatelliteBeamScaleMatrix.word8Copy) = g_YasuiLimitSatelliteBeamScaleMatrix.limitSBeamScaleMatrixWord8;
    } else {
        taskPtr->timer = BATTLE_TASK_FREE;
        return;
    }
    SetRotMatrix((void *)g_BattleCameraTransform.matrix);
    SetTransMatrix((void *)g_BattleCameraTransform.matrix);
    ProjectPointHardware(&taskPtr->posX, (short *)screenPosition, currDepthOut, projFlags);
    if ((*projFlags & 0x60000) == 0) {
        GetGeomOffset(geomOfsX, geomY);
        switch (g_BattleUiScalingMode) {
        case MAIN_GRAPHICS_MODE_320X240:
            *geomOfsX = 0xA0;
            *geomY = 120;
            break;
        default:
            *geomOfsX = 0x140;
            *geomY = 240;
        }
        screenPosition->projX -= *(short *)geomOfsX;
        screenPosition->projY -= *(short *)geomY;
        screenPosition->projX = screenPosition->projX / g_BattleUiScale;
        screenPosition->projY = screenPosition->projY / g_BattleUiScale;
        g_YasuiLimitSatelliteBeamScaleMatrix.projectedPosX = screenPosition->projX + offsetX << 3;
        g_YasuiLimitSatelliteBeamScaleMatrix.projectedPosY = screenPosition->projY + offsetY << 3;
        g_YasuiLimitSatelliteBeamScaleMatrix.projectedScreenScale = GetGeomScreen() << 3;
        SetRotMatrix((PsxMatrix *)&g_YasuiLimitSatelliteBeamScaleMatrix);
        SetTransMatrix(&g_YasuiLimitSatelliteBeamScaleMatrix);
        g_YasuiLimitSatelliteBeamSpriteDrawReq.limitSBeamSpriteDrawReqFlag12 = 0;
        g_YasuiLimitSatelliteBeamDrawHandle = BattleFx_SubmitSpriteDraw(&g_YasuiLimitSatelliteBeamSpriteDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_YasuiLimitSatelliteBeamDrawHandle);
        g_YasuiLimitSatelliteBeamScaleMatrix.projectedPosX = screenPosition->projX - offsetX << 3;
        g_YasuiLimitSatelliteBeamScaleMatrix.projectedPosY = screenPosition->projY - offsetY << 3;
        SetTransMatrix(&g_YasuiLimitSatelliteBeamScaleMatrix);
        g_YasuiLimitSatelliteBeamSpriteDrawReq.limitSBeamSpriteDrawReqFlag12 = 1;
        g_YasuiLimitSatelliteBeamDrawHandle = BattleFx_SubmitSpriteDraw(&g_YasuiLimitSatelliteBeamSpriteDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_YasuiLimitSatelliteBeamDrawHandle);
    }
    if (g_BattleIsPaused == 0) {
        (taskPtr->frameCount)++;
    }
}

/*
 * Emits scattered energy column models and growing beams around targets.
 */
// FUNCTION: FF7 0x00470eef
void __cdecl BattleFx_LimitSatelliteBeamScatteredColumnEmitterTask(void)
{
    BattleFxSatelliteBeamGrowingModel *beamAddr;
    BattleTaskSlot *task;
    BattleFxSatelliteBeamScatteredColumn *pColumn;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter % 3 == 0) {
        pColumn = (BattleFxSatelliteBeamScatteredColumn *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitSatelliteBeamScatteredColumnModelTask);
        pColumn->posX = *(short *)&g_YasuiLimitSatelliteBeamTargetCentroidX + rand() % 6000 + -3000;
        pColumn->posY = 0;
        pColumn->posZ = g_YasuiLimitSatelliteBeamTargetCentroidZ + rand() % 6000 + -3000;
        pColumn->rotX = *(short *)&g_YasuiLimitSatelliteBeamYawDirection;
        pColumn->rotY = 0;
        pColumn->rotZ = 0;
        pColumn->offsetY = 0xFED4;
        beamAddr = (BattleFxSatelliteBeamGrowingModel *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitSatelliteBeamGrowingModelTask);
        *(Int64Pair *)&beamAddr->posX = *(Int64Pair *)&pColumn->posX;
        beamAddr->targetScale = 0x800;
    }
    (task->frameCounter)++;
    if (task->frameCounter >= 33) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Renders vertically scaling laser pillar before orbital impact.
 */
// FUNCTION: FF7 0x00471023
void __cdecl BattleFx_LimitSatelliteBeamGrowingModelTask(void)
{
    BattleFxSatelliteBeamGrowingModel *taskPtr;
    int unusedSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int growth;

    taskPtr = (BattleFxSatelliteBeamGrowingModel *)BATTLE_CURRENT_TASK_SLOT;
    if ((*(unsigned char *)&g_YasuiLimitSatelliteBeamScratchAllocated & 1) == 0) {
        g_YasuiLimitSatelliteBeamScratchAllocated |= 1;
        g_YasuiLimitSatelliteBeamScratchMatrixPtr = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    }
    (g_YasuiLimitSatelliteBeamMeshDescriptor.pillarModelResource) = Gfx_ModelTableGetRenderObject(8, g_YasuiLimitSatelliteBeamLoadedResTable);
    growth = (taskPtr->frame << 12) / 10;
    g_YasuiLimitSatelliteBeamMeshDescriptor.limitSBeamMeshDescriptorWord10 = (growth * -0x80 >> 12) + 0x80;
    growth = (growth * 0xE00 /* 0.875 */ >> 12) + 0x200;
    (g_YasuiLimitSatelliteBeamColumnMatrixData.scaleY) = taskPtr->targetScale * growth >> 12;
    g_YasuiLimitSatelliteBeamColumnScaleY = (g_YasuiLimitSatelliteBeamColumnMatrixData.scaleY);
    g_YasuiLimitSatelliteBeamColumnTransformMatrix = g_YasuiLimitSatelliteBeamColumnScaleY;
    g_YasuiLimitSatelliteBeamColumnMatrixData.limitSBeamColumnMatrixPosX = taskPtr->posX;
    g_YasuiLimitSatelliteBeamColumnMatrixData.limitSBeamColumnMatrixPosY = 0;
    g_YasuiLimitSatelliteBeamColumnMatrixData.limitSBeamColumnMatrixPosZ = taskPtr->posZ;
    MultiplyMatrix((void *)g_BattleCameraTransform.matrix, &g_YasuiLimitSatelliteBeamColumnTransformMatrix, g_YasuiLimitSatelliteBeamScratchMatrixPtr);
    SetRotMatrix(g_YasuiLimitSatelliteBeamScratchMatrixPtr);
    SetTransMatrix(g_YasuiLimitSatelliteBeamScratchMatrixPtr);
    g_YasuiLimitSatelliteBeamDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiLimitSatelliteBeamMeshDescriptor, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST, 0, g_YasuiLimitSatelliteBeamDrawHandle);
    if (g_BattleIsPaused == 0) {
        (taskPtr->frame)++;
        if (taskPtr->frame >= 10) {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Renders randomized vertical energy beam columns striking the battlefield.
 */
// FUNCTION: FF7 0x004711a1
void __cdecl BattleFx_LimitSatelliteBeamScatteredColumnModelTask(void)
{
    BattleFxLimitSatelliteBeamImpact *scaleVecPtr;
    int frameIndex;
    BattleFxSatelliteBeamImpactRecord *task;
    PsxMatrix *matrixAddress;

    task = (BattleFxSatelliteBeamImpactRecord *)BATTLE_CURRENT_TASK_SLOT;
    g_YasuiLimitSatelliteBeamMeshDescriptor.nextModelResource = Gfx_ModelTableGetRenderObject(1, g_YasuiLimitSatelliteBeamLoadedResTable);
    matrixAddress = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    scaleVecPtr = (BattleFxLimitSatelliteBeamImpact *)(matrixAddress + 1);
    frameIndex = task->frameCount;
    if (frameIndex < 5) {
        scaleVecPtr->targetRate = 0x200;
        scaleVecPtr->rateA = 0x200;
        scaleVecPtr->rateB = 0x800;
    } else {
        frameIndex -= 5;
        if (frameIndex < 5) {
            scaleVecPtr->targetRate = frameIndex * -0x200 / 5 + 0x200;
            scaleVecPtr->rateA = scaleVecPtr->targetRate;
            scaleVecPtr->rateB = 0x800;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    RotMatrix((SVECTOR *)&task->rotX, matrixAddress);
    matrixAddress->t[0] = task->posX;
    matrixAddress->t[1] = task->posY;
    matrixAddress->t[2] = task->posZ;
    MultiplyMatrix((void *)g_BattleCameraTransform.matrix, matrixAddress, matrixAddress);
    Gte_OrthonormaliseFromYAxis((BattleOrthoMatrix *)matrixAddress);
    ScaleMatrix(matrixAddress, scaleVecPtr);
    NormalizeVectorFixed(matrixAddress->t, (int *)scaleVecPtr);
    matrixAddress->t[0] -= scaleVecPtr->rateA * task->scatter >> 12;
    matrixAddress->t[1] -= scaleVecPtr->rateB * task->scatter >> 12;
    matrixAddress->t[2] -= scaleVecPtr->targetRate * task->scatter >> 12;
    SetRotMatrix(matrixAddress);
    SetTransMatrix(matrixAddress);
    g_YasuiLimitSatelliteBeamDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiLimitSatelliteBeamMeshDescriptor.nextModelResource, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitSatelliteBeamDrawHandle);
    if (g_BattleIsPaused == 0) {
        (task->frameCount)++;
    }
}

/*
 * Spawns main impact timeline task for an individual target.
 */
// FUNCTION: FF7 0x004713a1
void __cdecl BattleFx_LimitSpawnSatelliteBeamPerTarget(int actor)
{
    BattleFxSatelliteBeamTargetRecord *slot;

    slot = (BattleFxSatelliteBeamTargetRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitSatelliteBeamImpactTimelineTask);
    if (g_YasuiLimitSatelliteBeamTargetMask & (1 << actor)) {
        Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &slot->posX);
        slot->posY = 0;
        slot->radius = g_BattleActorRecords[actor].radius;
        slot->actorIndex = actor;
    } else {
        slot->posX = *(short *)&g_YasuiLimitSatelliteBeamTargetCentroidX + rand() % 6000 - 3000;
        slot->posY = 0;
        slot->posZ = g_YasuiLimitSatelliteBeamTargetCentroidZ + rand() % 6000 - 3000;
        slot->radius = 1000;
        slot->actorIndex = 0xFFFF;
    }
}

/*
 * Coordinates massive orbital strike impact, screen shake, and shockwaves.
 */
// FUNCTION: FF7 0x0047148b
void __cdecl BattleFx_LimitSatelliteBeamImpactTimelineTask(void)
{
    int puffIndex;
    BattleFxProgram1Slot *taskPtr;
    BattleFxBeamImpactParams *params;
    int matrixTail;
    BattleFxLimitSatelliteBeamImpactSlot *spawned;
    PsxMatrix *matrixAddress;

    taskPtr = (BattleFxProgram1Slot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    params = (BattleFxBeamImpactParams *)Gfx_GetPsxScratchpadPtr(0);
    matrixAddress = (PsxMatrix *)((char *)params + 8);
    matrixTail = (int)(matrixAddress + 1);
    if (taskPtr->step == 0) {
        spawned = (BattleFxLimitSatelliteBeamImpactSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitSatelliteBeamImpactColumnModelTask);
        *(Int64Pair *)&spawned->posX = *(Int64Pair *)&taskPtr->posX;
        spawned->posY = 0;
        spawned->yaw = *(short *)&g_YasuiLimitSatelliteBeamYawDirection;
        spawned->rotY = 0;
        spawned->rotZ = 0;
        spawned->driftY = 0xFC18;
    }
    if (taskPtr->step == 1) {
        spawned = (BattleFxLimitSatelliteBeamImpactSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitSatelliteBeamExpandingShellModelTask);
        *(Int64Pair *)&spawned->posX = *(Int64Pair *)&taskPtr->posX;
        spawned->yaw = 0;
        spawned->rotY = 0;
        spawned->rotZ = 0;
        spawned = (BattleFxLimitSatelliteBeamImpactSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitSatelliteBeamBlackPuffEmitterTask);
        *(Int64Pair *)&spawned->posX = *(Int64Pair *)&taskPtr->posX;
        for (puffIndex = 0; puffIndex < 15; puffIndex++) {
            spawned = (BattleFxLimitSatelliteBeamImpactSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitSatelliteBeamDampedSpriteParticleTask);
            *(Int64Pair *)&spawned->posX = *(Int64Pair *)&taskPtr->posX;
            params->timer = 0;
            params->phase = rand() & 0xFFF;
            params->drift = -((rand() & 0x1FF) + 512);
            RotMatrix((SVECTOR *)params, matrixAddress);
            params->timer = rand() % 300 + 500;
            params->phase = 0;
            params->drift = 0;
            TransformVectorToInt16(matrixAddress, (void *)params, &spawned->yaw);
            spawned->driftX = rand() & 0x3F;
            spawned->driftY = rand() & 0x3F;
        }
        spawned = (BattleFxLimitSatelliteBeamImpactSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitSatelliteBeamGrowingModelTask);
        *(Int64Pair *)&spawned->posX = *(Int64Pair *)&taskPtr->posX;
        spawned->driftX = 8192;
        if (taskPtr->recordType != -1) {
            Battle_PostActorHitOrHurtMessage(taskPtr->recordType);
        }
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
    (taskPtr->step)++;
}

/*
 * Renders the primary massive orbital laser beam cylinder on target.
 */
// FUNCTION: FF7 0x00471722
void __cdecl BattleFx_LimitSatelliteBeamImpactColumnModelTask(void)
{
    BattleFxLimitSatelliteBeamImpact *scaleVecPtr;
    int frameIndex;
    BattleFxSatelliteBeamImpactRecord *task;
    PsxMatrix *matrixAddress;

    task = (BattleFxSatelliteBeamImpactRecord *)BATTLE_CURRENT_TASK_SLOT;
    (g_YasuiLimitSatelliteBeamLineColorParams2.beamModelResource) = Gfx_ModelTableGetRenderObject(1, g_YasuiLimitSatelliteBeamLoadedResTable);
    matrixAddress = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    scaleVecPtr = (BattleFxLimitSatelliteBeamImpact *)(matrixAddress + 1);
    frameIndex = task->frameCount;
    if (frameIndex < 10) {
        scaleVecPtr->targetRate = 0x800;
        scaleVecPtr->rateA = 0x800;
        scaleVecPtr->rateB = 0x800;
    } else {
        frameIndex -= 10;
        if (frameIndex < 5) {
            scaleVecPtr->targetRate = frameIndex * -0x800 / 5 + 0x800;
            scaleVecPtr->rateA = scaleVecPtr->targetRate;
            scaleVecPtr->rateB = 0x800;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    RotMatrix((SVECTOR *)&task->rotX, matrixAddress);
    matrixAddress->t[0] = task->posX;
    matrixAddress->t[1] = task->posY;
    matrixAddress->t[2] = task->posZ;
    MultiplyMatrix((void *)g_BattleCameraTransform.matrix, matrixAddress, matrixAddress);
    Gte_OrthonormaliseFromYAxis((BattleOrthoMatrix *)matrixAddress);
    ScaleMatrix(matrixAddress, scaleVecPtr);
    NormalizeVectorFixed(matrixAddress->t, (int *)scaleVecPtr);
    matrixAddress->t[0] -= scaleVecPtr->rateA * task->scatter >> 12;
    matrixAddress->t[1] -= scaleVecPtr->rateB * task->scatter >> 12;
    matrixAddress->t[2] -= scaleVecPtr->targetRate * task->scatter >> 12;
    SetRotMatrix(matrixAddress);
    SetTransMatrix(matrixAddress);
    g_YasuiLimitSatelliteBeamDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiLimitSatelliteBeamLineColorParams2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitSatelliteBeamDrawHandle);
    if (g_BattleIsPaused == 0) {
        (task->frameCount)++;
    }
}

/*
 * Renders expanding explosion shockwave shell models on impact.
 */
// FUNCTION: FF7 0x00471922
void __cdecl BattleFx_LimitSatelliteBeamExpandingShellModelTask(void)
{
    int frameIndex;
    BattleFxLimitSatelliteBeamShell *pScaleRec;
    PsxMatrix *rotMatrix;
    BattleFxCosmoMemoryExpandingModel *taskPtr;
    PsxMatrix *viewMatrix;

    taskPtr = (BattleFxCosmoMemoryExpandingModel *)BATTLE_CURRENT_TASK_SLOT;
    rotMatrix = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    pScaleRec = (BattleFxLimitSatelliteBeamShell *)(rotMatrix + 1);
    viewMatrix = (PsxMatrix *)((char *)pScaleRec + 0x10);
    frameIndex = taskPtr->frame;
    if (frameIndex < 5) {
        g_YasuiLimitSatelliteBeamLineColorParams2.flareScale = 0;
    } else {
        frameIndex -= 5;
        if (frameIndex < 15) {
            g_YasuiLimitSatelliteBeamLineColorParams2.flareScale = (frameIndex << 12) / 15;
        } else {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    pScaleRec->nextSize = (taskPtr->frame << 12) / 20;
    pScaleRec->size = pScaleRec->nextSize;
    pScaleRec->prevSize = pScaleRec->size;
    RotMatrix((SVECTOR *)((int)taskPtr + 0xC), rotMatrix);
    rotMatrix->t[0] = taskPtr->posX;
    rotMatrix->t[1] = taskPtr->posY;
    rotMatrix->t[2] = taskPtr->posZ;
    ScaleMatrix(rotMatrix, pScaleRec);
    MultiplyMatrix((void *)g_BattleCameraTransform.matrix, rotMatrix, viewMatrix);
    SetRotMatrix(viewMatrix);
    SetTransMatrix(viewMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_YasuiLimitSatelliteBeamLineColorParams2.flareModelRes = Gfx_ModelTableGetRenderObject(7, g_YasuiLimitSatelliteBeamLoadedResTable);
    g_YasuiLimitSatelliteBeamDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiLimitSatelliteBeamLineColorParams2.flareModelRes, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitSatelliteBeamDrawHandle);
    g_YasuiLimitSatelliteBeamLineColorParams2.flareModelRes = Gfx_ModelTableGetRenderObject(6, g_YasuiLimitSatelliteBeamLoadedResTable);
    g_YasuiLimitSatelliteBeamDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiLimitSatelliteBeamLineColorParams2.flareModelRes, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitSatelliteBeamDrawHandle);
    RotMatrix((SVECTOR *)&g_YasuiLimitSatelliteBeamBlastRotationAngles, rotMatrix);
    pScaleRec->nextSize = pScaleRec->prevSize << 1;
    pScaleRec->size = pScaleRec->nextSize;
    pScaleRec->prevSize = pScaleRec->size;
    ScaleMatrix(rotMatrix, pScaleRec);
    MultiplyMatrix((void *)g_BattleCameraTransform.matrix, rotMatrix, viewMatrix);
    SetRotMatrix(viewMatrix);
    g_YasuiLimitSatelliteBeamLineColorParams2.flareModelRes = Gfx_ModelTableGetRenderObject(2, g_YasuiLimitSatelliteBeamLoadedResTable);
    g_YasuiLimitSatelliteBeamDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiLimitSatelliteBeamLineColorParams2.flareModelRes, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitSatelliteBeamDrawHandle);
    if (g_BattleIsPaused == 0) {
        (taskPtr->frame)++;
    }
}

/*
 * Emits dark blast smoke puff particles during ground impact.
 */
// FUNCTION: FF7 0x00471b90
void __cdecl BattleFx_LimitSatelliteBeamBlackPuffEmitterTask(void)
{
    BattleFxSatelliteBeamBlackPuff *taskPtr;
    int unusedSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxBlackPuffEmitter *pPuff;

    taskPtr = (BattleFxSatelliteBeamBlackPuff *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    pPuff = (BattleFxBlackPuffEmitter *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitSatelliteBeamBlackPuffModelTask);
    *(Int64Pair *)&pPuff->posX = *(Int64Pair *)&taskPtr->posX;
    pPuff->scaleY = taskPtr->frame * 0xC00 / 13 + 0x400;
    if (taskPtr->frame > 10) {
        pPuff->frameOffset = taskPtr->frame - 0xA;
    }
    taskPtr->posY -= 100;
    (taskPtr->frame)++;
    if (taskPtr->frame >= 13) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Renders dark blast smoke puff model at impact ground coordinates.
 */
// FUNCTION: FF7 0x00471c61
void __cdecl BattleFx_LimitSatelliteBeamBlackPuffModelTask(void)
{
    BattleFxSatelliteBeamPuffRecord *task;
    PsxMatrix *matrixAddress;

    task = (BattleFxSatelliteBeamPuffRecord *)BATTLE_CURRENT_TASK_SLOT;
    g_YasuiLimitSatelliteBeamLineColorParams2.ringModelRes = Gfx_ModelTableGetRenderObject(3, g_YasuiLimitSatelliteBeamLoadedResTable);
    matrixAddress = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    g_YasuiLimitSatelliteBeamLineColorParams2.ringScale = (task->frame << 12) / 3;
    (g_YasuiLimitSatelliteBeamPuffMatrixData.scaleY) = task->scaleY;
    g_YasuiLimitSatelliteBeamPuffScaleY = (g_YasuiLimitSatelliteBeamPuffMatrixData.scaleY);
    g_YasuiLimitSatelliteBeamPuffTransformMatrix = g_YasuiLimitSatelliteBeamPuffScaleY;
    g_YasuiLimitSatelliteBeamPuffMatrixData.limitSBeamPuffMatrixPosX = task->posX;
    g_YasuiLimitSatelliteBeamPuffMatrixData.limitSBeamPuffMatrixPosY = task->posY;
    g_YasuiLimitSatelliteBeamPuffMatrixData.limitSBeamPuffMatrixPosZ = task->posZ;
    MultiplyMatrix((void *)g_BattleCameraTransform.matrix, &g_YasuiLimitSatelliteBeamPuffTransformMatrix, matrixAddress);
    SetRotMatrix(matrixAddress);
    SetTransMatrix(matrixAddress);
    SetDrawColorRGB(0, 0, 0);
    g_YasuiLimitSatelliteBeamDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiLimitSatelliteBeamLineColorParams2.ringModelRes, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitSatelliteBeamDrawHandle);
    if (g_BattleIsPaused == 0) {
        (task->frame)++;
        if (task->frame >= 3) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Renders and simulates motion for damped debris sprite particles.
 */
// FUNCTION: FF7 0x00471d99
void __cdecl BattleFx_LimitSatelliteBeamDampedSpriteParticleTask(void)
{
    BattleFxLimitSatelliteBeamDisplacement *displacePtr;
    BattleFxLimitSatelliteBeamParticle *particle;
    int frame;
    PsxMatrix *matrix;

    particle = (BattleFxLimitSatelliteBeamParticle *)BATTLE_CURRENT_TASK_SLOT;
    g_YasuiLimitSatelliteBeamLineColorParams2.smokeSpriteRecord = Gfx_ModelTableGetLoadedRecord(5, g_YasuiLimitSatelliteBeamLoadedResTable);
    g_YasuiLimitSatelliteBeamLineColorParams2.smokeSpriteObject = Gfx_ModelTableGetSpriteObject(5, g_YasuiLimitSatelliteBeamLoadedResTable);
    matrix = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    displacePtr = (BattleFxLimitSatelliteBeamDisplacement *)(matrix + 1);
    frame = particle->frameCount;
    if (frame < 10) {
        (*(int *)&g_YasuiLimitSatelliteBeamLineCoordParams2) = 0x2C808080;
    } else {
        frame -= 10;
        if (frame < 5) {
            g_YasuiLimitSatelliteBeamLineCoordParams2.limitSBeamLineCoordParams2Byte2 = frame * -0x80 / 5 + 0x80;
            g_YasuiLimitSatelliteBeamLineCoordParams2.limitSBeamLineCoordParams2Byte1 = g_YasuiLimitSatelliteBeamLineCoordParams2.limitSBeamLineCoordParams2Byte2;
            *(char *)&g_YasuiLimitSatelliteBeamLineCoordParams2 = g_YasuiLimitSatelliteBeamLineCoordParams2.limitSBeamLineCoordParams2Byte1;
            g_YasuiLimitSatelliteBeamLineCoordParams2.limitSBeamLineCoordParams2Byte3 = 0x2E;
        } else {
            particle->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    displacePtr->displaceX = particle->frameCount * particle->stepX;
    displacePtr->displaceY = particle->frameCount * particle->stepY;
    displacePtr->displaceZ = 0;
    RotMatrix((SVECTOR *)displacePtr, matrix);
    matrix->t[0] = particle->posX;
    matrix->t[1] = particle->posY;
    matrix->t[2] = particle->posZ;
    MultiplyMatrix((void *)g_BattleCameraTransform.matrix, matrix, matrix);
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    g_YasuiLimitSatelliteBeamDrawHandle = BattleFx_SubmitSpriteDraw(&g_YasuiLimitSatelliteBeamLineColorParams2.smokeSpriteRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitSatelliteBeamDrawHandle);
    if (g_BattleIsPaused == 0) {
        (particle->frameCount)++;
        particle->posX += particle->velocityX;
        particle->posY += particle->velocityY;
        particle->posZ += particle->velocityZ;
        particle->velocityX = particle->velocityX * 0xF00 /* 0.9375 */ >> 12;
        particle->velocityY = (particle->velocityY * 0xF00 /* 0.9375 */ >> 12) + 60;
        particle->velocityZ = particle->velocityZ * 0xF00 /* 0.9375 */ >> 12;
    }
}
} /* extern "C" */
