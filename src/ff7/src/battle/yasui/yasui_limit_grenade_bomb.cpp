// src/ff7/src/battle/yasui/yasui_limit_grenade_bomb.cpp
// Limit Break [8] (Barret Lv 2-1): Grenade Bomb (Japanese: グレネードボム, dev tag: "LIMIT/GLENADE" / Glenade)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle effects for Barret's Grenade Bomb limit break: a timeline that drives the
//   detonation, with an expanding shockwave, a growing core, tumbling debris and radial smoke
//   puffs.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "lib/codegen.h"
#include "ff7/types/battle_boss.h"
#include "ff7/battle_tasks.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
extern "C" {

struct BattleFxModelDraw;

#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x007ee150
int g_YasuiLimitGrenadeBombResourceHeader = 0x00C800B1;

int g_YasuiLimitGrenadeBombResourceHeaderHeadPtr[1] = {
    (int)((char *)&g_YasuiLimitGrenadeBombResourceNames + 0x0)
};

int g_YasuiLimitGrenadeBombResourceHeaderDataTail[34] = {
    0x0, 0x0, 0x0, 0xc800b1, (int)((char *)&g_YasuiLimitGrenadeBombResourceNames + 0xc), 0x0,
    0x0, 0x0, 0xc800b1, (int)((char *)&g_YasuiLimitGrenadeBombResourceNames + 0x18), 0x0, 0x0,
    0x0, 0x192a39, (int)((char *)&g_YasuiLimitGrenadeBombResourceNames + 0x24), (int)((char *)&g_YasuiLimitGrenadeBombResourceNames + 0x34), 0x0, 0x0,
    0x192a39, (int)((char *)&g_YasuiLimitGrenadeBombResourceNames + 0x40), (int)((char *)&g_YasuiLimitGrenadeBombResourceNames + 0x50), 0x0, 0x0, 0xc800b1,
    (int)((char *)&g_YasuiLimitGrenadeBombResourceNames + 0x5c), 0x0, 0x0, 0x0, 0xc800b1, (int)((char *)&g_YasuiLimitGrenadeBombResourceNames + 0x6c),
    0x0, 0x0, 0x0, 0x0
};

// GLOBAL: FF7 0x007ee1e0
BattleLimitGrenadeShellReq g_YasuiLimitGrenadeBombExpandingShellModelDrawReq = {0, {0x88, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}, 0, {0x88, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}, 0, {0xa8, 0, 0, 0, 0, 0}, 0};

unsigned char g_rawData_007ee20c[4] = {0x20, 0x00, 0x00, 0x00};

// GLOBAL: FF7 0x007ee210
struct YasuiLimitGrenadeBombExpandingScaleMatrix g_YasuiLimitGrenadeBombExpandingScaleMatrix = {{0, 0x40, 0, 0, 0, 0x40, 0, 0, 0, 0x40, 0, 0, 0, 0, 0, 0}, 0, {0xa8, 0, 0, 0, 0, 0}, 0};

unsigned char g_rawData_007ee22c[4] = {0x20, 0x00, 0x00, 0x00};

// GLOBAL: FF7 0x007ee230
struct YasuiLimitGrenadeBombGrowingScaleMatrix g_YasuiLimitGrenadeBombGrowingScaleMatrix = {0x1000, {0}, 0x1000, {0}, 0x1000, 0, 0, 0, {0}, 0, 0};

// GLOBAL: FF7 0x007ee258
struct YasuiLimitGrenadeBombSpriteDrawReqPool g_YasuiLimitGrenadeBombSpriteDrawReqPool = {{0x2e808080}, 0, {0}, 0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}, 0, {0xa8, 0, 0, 0, 0, 0}, 0};

unsigned char g_rawData_007ee27c[4] = {0x20, 0x00, 0x00, 0x00};

unsigned char g_YasuiLimitGrenadeBombResourceNames[120] =
    "GLENADE.RSD\0"
    "CBOMBER.RSD\0"
    "IMPULSE.RSD\0"
    "GRENAD01.TIM\0\0\0\0"
    "HAHEN.SPT\0\0\0"
    "GRENAD00.TIM\0\0\0\0"
    "SMOG.SPT\0\0\0\0"
    "SHOUGEKI.RSD\0\0\0\0"
    "NETSU.RSD";

// GLOBAL: FF7 0x00a545e8
int g_YasuiLimitGrenadeBombExpandingShellScale = 0;

// GLOBAL: FF7 0x00a545ec
int g_YasuiLimitGrenadeBombDrawHandle = 0;

// GLOBAL: FF7 0x00a545f0
int g_YasuiLimitGrenadeBombTargetMask = 0;

unsigned char g_bssPad_00a545f4[4] = {0};

unsigned char g_bssPad_00a545f8[8] = {0};

// GLOBAL: FF7 0x00a54600
struct GfxTableCursor *g_YasuiLimitGrenadeBombLoadedResTable = 0;

unsigned char g_bssPad_00a54604[4] = {0};

// GLOBAL: FF7 0x00a54608
short g_YasuiLimitGrenadeBombBlackBlastTransformMatrix = 0;

unsigned char g_bssPad_00a5460c[4] = {0};

// GLOBAL: FF7 0x00a54610
short g_YasuiLimitGrenadeBombBlackBlastScaleY = 0;

unsigned char g_bssPad_00a54614[4] = {0};

// GLOBAL: FF7 0x00a54618
struct YasuiLimitGrenadeBombBlackBlastMatrixData g_YasuiLimitGrenadeBombBlackBlastMatrixData = {0};
// clang-format on


/*
 * Loads resource files for Barret's Grenade Bomb limit break effect.
 */
// FUNCTION: FF7 0x00471fd0
void __cdecl BattleFx_LimitLoadResGrenadeBomb(void)
{
    char pathScratch[200];
    GfxModelTableBank *context;
    GfxResourceRequest requestRecord;
    int kind;
    GfxResourceRequest *pReq;
    int resourceRows;

    if (g_pBattle3dScene != 0 && g_YasuiLimitGrenadeBombLoadedResTable == 0) {
        kind = 7;
        resourceRows = (int)&g_YasuiLimitGrenadeBombResourceHeader;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestRecord;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x007ee2f8
        pReq->resourcePath = BattleFx_BuildResourcePath("LIMIT/GLENADE", pathScratch);
        Gfx_LoadModelTableIfAbsent(kind, resourceRows, pReq, &g_YasuiLimitGrenadeBombLoadedResTable, context);
    }
}

/*
 * Initializes and begins the Grenade Bomb limit effect for the given targets.
 */
// FUNCTION: FF7 0x00472060
int __cdecl BattleFx_LimitEnterGrenadeBomb(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResGrenadeBomb();
    BattleFx_LimitBeginGrenadeBomb(targetMask, actorIdx);
    return 0;
}

/*
 * Computes centroid and begins Grenade Bomb timeline for the targeted actors.
 */
// FUNCTION: FF7 0x0047207c
void __cdecl BattleFx_LimitBeginGrenadeBomb(int targetMask, int actorIdx)
{
    BattleFxLimitGrenadeBombTimeline *slot;

    slot = (BattleFxLimitGrenadeBombTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitGrenadeBombTimelineTask);
    Battle_GetActorMaskCentroid(g_YasuiLimitGrenadeBombTargetMask = targetMask, &slot->posX);
    slot->posY -= 0xA;
}

/*
 * Main timeline task driving Grenade Bomb visual phases and shell detonation.
 */
// FUNCTION: FF7 0x004720cf
void __cdecl BattleFx_LimitGrenadeBombTimelineTask(void)
{
    BattleFxLimitGrenadeBombTimeline *taskPtr;
    int i;
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxLimitGrenadeBombChild *child;

    taskPtr = (BattleFxLimitGrenadeBombTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->frameCounter == 0x28) {
        BattleFx_StartScreenFlashRamp(1);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&taskPtr->pos), 0xFA);
    }
    if (taskPtr->frameCounter == 0x29) {
        BattleFx_FadeOutScreenFlashRamp(3);
    }
    if (taskPtr->frameCounter == 0x28) {
        child = (BattleFxLimitGrenadeBombChild *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitGrenadeBombBlackBlastModelTask);
        child->pos = taskPtr->pos;
    }
    if (taskPtr->frameCounter == 0x2B) {
        child = (BattleFxLimitGrenadeBombChild *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitGrenadeBombExpandingShellModelTask);
        child->pos = taskPtr->pos;
        child->rotX = 0;
        child->rotY = 0;
        child->rotZ = 0;
    }
    if (taskPtr->frameCounter == 0x2B) {
        child = (BattleFxLimitGrenadeBombChild *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitGrenadeBombGrowingModelTask);
        child->pos = taskPtr->pos;
    }
    if (taskPtr->frameCounter == 0x2B) {
        child = (BattleFxLimitGrenadeBombChild *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitGrenadeBombTumblingSpriteEmitterTask);
        child->pos = taskPtr->pos;
    }
    if (taskPtr->frameCounter == 0x2B) {
        child = (BattleFxLimitGrenadeBombChild *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitGrenadeBombRadialSpriteEmitterTask);
        child->pos = taskPtr->pos;
    }
    if (taskPtr->frameCounter == 0x41) {
        for (i = 0; i < 0xA; i++) {
            if ((g_YasuiLimitGrenadeBombTargetMask & 1 << i) != 0) {
                Battle_PostActorHitOrHurtMessage(i);
                g_BattleActorStates[i].motionState = 3;
            }
        }
    }
    if (taskPtr->frameCounter == 0x41) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
    (taskPtr->frameCounter)++;
}

/*
 * Renders expanding shockwave shell model during Grenade Bomb detonation.
 */
// FUNCTION: FF7 0x004722fd
void __cdecl BattleFx_LimitGrenadeBombExpandingShellModelTask(void)
{
    BattleFxGrenadeBombScaleDelay *scaler;
    int frame;
    BattleFxGrenadeBombShellRecord *task;
    struct BattleFxShardTransformBatch *matrix;

    task = (BattleFxGrenadeBombShellRecord *)BATTLE_CURRENT_TASK_SLOT;
    (g_YasuiLimitGrenadeBombExpandingShellModelDrawReq.limitGlenadeShellDrawResource0) = Gfx_ModelTableGetRenderObject(0, g_YasuiLimitGrenadeBombLoadedResTable);
    g_YasuiLimitGrenadeBombExpandingShellModelDrawReq.limitGlenadeShellDrawResourceA = Gfx_ModelTableGetRenderObject(1, g_YasuiLimitGrenadeBombLoadedResTable);
    g_YasuiLimitGrenadeBombExpandingShellModelDrawReq.limitGlenadeShellDrawResourceB = Gfx_ModelTableGetRenderObject(6, g_YasuiLimitGrenadeBombLoadedResTable);
    matrix = (struct BattleFxShardTransformBatch *)Gfx_GetPsxScratchpadPtr(0);
    scaler = (BattleFxGrenadeBombScaleDelay *)((char *)matrix + 0x1E);
    frame = task->frame;
    if (frame < 0xA) {
        g_YasuiLimitGrenadeBombExpandingShellModelDrawReq.limitGlenadeShellDrawWordC = 0;
        g_YasuiLimitGrenadeBombExpandingShellModelDrawReq.limitGlenadeShellDrawWordB = g_YasuiLimitGrenadeBombExpandingShellModelDrawReq.limitGlenadeShellDrawWordC;
        g_YasuiLimitGrenadeBombExpandingShellModelDrawReq.limitGlenadeShellDrawWordA = g_YasuiLimitGrenadeBombExpandingShellModelDrawReq.limitGlenadeShellDrawWordB;
    } else {
        frame -= 0xA;
        if (frame < 0xA) {
            g_YasuiLimitGrenadeBombExpandingShellModelDrawReq.limitGlenadeShellDrawWordC = (frame << 0xC) / 0xA;
            g_YasuiLimitGrenadeBombExpandingShellModelDrawReq.limitGlenadeShellDrawWordB = g_YasuiLimitGrenadeBombExpandingShellModelDrawReq.limitGlenadeShellDrawWordC;
            g_YasuiLimitGrenadeBombExpandingShellModelDrawReq.limitGlenadeShellDrawWordA = g_YasuiLimitGrenadeBombExpandingShellModelDrawReq.limitGlenadeShellDrawWordB;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    RotMatrixXYZ((SVECTOR *)&task->rotX, (void *)matrix);
    matrix->posX = task->posX;
    matrix->posY = task->posY;
    matrix->posZ = task->posZ;
    scaler->scaleTarget = (task->frame << 9) / 0x14;
    scaler->scaleDelayed = scaler->scaleTarget;
    scaler->scale = scaler->scaleDelayed;
    g_YasuiLimitGrenadeBombExpandingShellScale = scaler->scale;
    ScaleMatrix(matrix, scaler);
    MultiplyMatrix(&g_BattleCameraTransform, matrix, matrix);
    SetRotMatrix((void *)matrix);
    SetTransMatrix(matrix);
    SetDrawColorRGB(0, 0, 0);
    g_YasuiLimitGrenadeBombDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiLimitGrenadeBombExpandingShellModelDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitGrenadeBombDrawHandle);
    g_YasuiLimitGrenadeBombDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiLimitGrenadeBombExpandingShellModelDrawReq.limitGlenadeShellDrawResourceA, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitGrenadeBombDrawHandle);
    ScaleMatrix(matrix, (int *)&g_YasuiLimitGrenadeBombExpandingScaleMatrix);
    SetRotMatrix((void *)matrix);
    g_YasuiLimitGrenadeBombDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiLimitGrenadeBombExpandingShellModelDrawReq.limitGlenadeShellDrawResourceB, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitGrenadeBombDrawHandle);
    if (g_BattleIsPaused == 0) {
        (task->frame)++;
        task->spin += 0xFFC0;
    }
}

/*
 * Renders growing explosion core model during Grenade Bomb detonation.
 */
// FUNCTION: FF7 0x0047256c
void __cdecl BattleFx_LimitGrenadeBombGrowingModelTask(void)
{
    int frame;
    BattleFxCosmoMemoryExpandingModel *task;
    PsxMatrix *matrix;

    task = (BattleFxCosmoMemoryExpandingModel *)BATTLE_CURRENT_TASK_SLOT;
    matrix = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    g_YasuiLimitGrenadeBombExpandingScaleMatrix.limitGlenadeExpandScaleResource = Gfx_ModelTableGetRenderObject(2, g_YasuiLimitGrenadeBombLoadedResTable);
    frame = task->frame;
    if (frame < 0xA) {
        g_YasuiLimitGrenadeBombExpandingScaleMatrix.limitGlenadeExpandScaleFactor = 0x800;
    } else {
        frame -= 0xA;
        if (frame < 0xA) {
            g_YasuiLimitGrenadeBombExpandingScaleMatrix.limitGlenadeExpandScaleFactor = (frame << 0xB) / 0xA + 0x800;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    g_YasuiLimitGrenadeBombGrowingScaleMatrix.limitGlenadeGrowScaleProgress = (task->frame << 0xA) / 0x14;
    g_YasuiLimitGrenadeBombGrowingScaleMatrix.limitGlenadeGrowScaleCurrent = g_YasuiLimitGrenadeBombGrowingScaleMatrix.limitGlenadeGrowScaleProgress;
    (g_YasuiLimitGrenadeBombGrowingScaleMatrix.growScaleCopy) = g_YasuiLimitGrenadeBombGrowingScaleMatrix.limitGlenadeGrowScaleCurrent;
    g_YasuiLimitGrenadeBombGrowingScaleMatrix.limitGlenadeGrowPosX = task->posX;
    g_YasuiLimitGrenadeBombGrowingScaleMatrix.limitGlenadeGrowPosY = task->posY;
    g_YasuiLimitGrenadeBombGrowingScaleMatrix.limitGlenadeGrowPosZ = task->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, (short *)&g_YasuiLimitGrenadeBombGrowingScaleMatrix, matrix);
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    g_YasuiLimitGrenadeBombDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiLimitGrenadeBombExpandingScaleMatrix.limitGlenadeExpandScaleResource, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitGrenadeBombDrawHandle);
    if (g_BattleIsPaused == 0) {
        (task->frame)++;
    }
}

/*
 * Emits tumbling debris sprite particles during Grenade Bomb explosion.
 */
// FUNCTION: FF7 0x004726c5
void __cdecl BattleFx_LimitGrenadeBombTumblingSpriteEmitterTask(void)
{
    BattleFxLimitGrenadeBombTimeline *taskPtr;
    BattleFxTumblingParams *params;
    int i;
    PsxMatrix *matrix;
    BattleFxGrenadeBombTumbleRecord *pDebris;

    taskPtr = (BattleFxLimitGrenadeBombTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    params = (BattleFxTumblingParams *)Gfx_GetPsxScratchpadPtr(0);
    matrix = (PsxMatrix *)((char *)params + 8);
    for (i = 0; i < 7; i++) {
        pDebris = (BattleFxGrenadeBombTumbleRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitGrenadeBombTumblingSpriteTask);
        pDebris->pos = taskPtr->pos;
        params->angle = 0;
        params->spinSpeed = rand() & 0xFFF;
        params->tiltSpeed = rand() % 0x600 - 0x300;
        RotMatrix((SVECTOR *)params, matrix);
        params->angle = 0;
        params->spinSpeed = -(rand() % 0x1F4 + 0x1F4);
        params->tiltSpeed = 0;
        TransformVectorToInt16(matrix, (void *)params, pDebris->velocity);
        pDebris->angleZ = 0;
        pDebris->angleY = 0;
        pDebris->angleX = 0;
        pDebris->spinX = (rand() & 0xFF) - 0x80;
        pDebris->spinY = (rand() & 0xFF) - 0x80;
        pDebris->spinZ = (rand() & 0xFF) - 0x80;
        pDebris->scale = rand() % 0x1000 + 0x1000;
        pDebris->spriteIndex = rand() % 3;
    }
    (taskPtr->frameCounter)++;
    if (taskPtr->frameCounter > 5) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Simulates and renders an individual tumbling debris sprite particle.
 */
// FUNCTION: FF7 0x0047288a
void __cdecl BattleFx_LimitGrenadeBombTumblingSpriteTask(void)
{
    BattleFxLimitSealEvilDelay *scaleVecPtr;
    int frameIndex;
    BattleFxGrenadeBombSprite *sprite;
    BattleFxEulerAngles16 *pAngles;
    struct BattleFxShardTransformBatch *matrix;

    sprite = (BattleFxGrenadeBombSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_YasuiLimitGrenadeBombGrowingScaleMatrix.limitGlenadeTumbleSpriteResource = Gfx_ModelTableGetLoadedRecord(3, g_YasuiLimitGrenadeBombLoadedResTable);
    g_YasuiLimitGrenadeBombGrowingScaleMatrix.limitGlenadeTumbleSpriteResourceB = Gfx_ModelTableGetSpriteObject(3, g_YasuiLimitGrenadeBombLoadedResTable);
    pAngles = (BattleFxEulerAngles16 *)Gfx_GetPsxScratchpadPtr(0);
    matrix = (struct BattleFxShardTransformBatch *)((int)pAngles + 8);
    scaleVecPtr = (BattleFxLimitSealEvilDelay *)((char *)matrix + 0x1E);
    frameIndex = sprite->frameIndex;
    if (frameIndex < 0xA) {
        g_YasuiLimitGrenadeBombSpriteDrawReqPool.colorWord = 0x2C808080;
    } else {
        frameIndex -= 0xA;
        if (frameIndex < 5) {
            g_YasuiLimitGrenadeBombSpriteDrawReqPool.colorB = frameIndex * -0x80 / 5 + 0x80;
            g_YasuiLimitGrenadeBombSpriteDrawReqPool.colorG = g_YasuiLimitGrenadeBombSpriteDrawReqPool.colorB;
            g_YasuiLimitGrenadeBombSpriteDrawReqPool.colorR = g_YasuiLimitGrenadeBombSpriteDrawReqPool.colorG;
            g_YasuiLimitGrenadeBombSpriteDrawReqPool.command = 0x2E;
        } else {
            sprite->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    pAngles->angleX = sprite->angleX << 4;
    pAngles->angleY = sprite->angleY << 4;
    pAngles->angleZ = sprite->angleZ << 4;
    RotMatrix((SVECTOR *)pAngles, matrix);
    matrix->posX = sprite->posX;
    matrix->posY = sprite->posY;
    matrix->posZ = sprite->posZ;
    scaleVecPtr->nextValue = sprite->nextValue;
    scaleVecPtr->value = scaleVecPtr->nextValue;
    scaleVecPtr->prevValue = scaleVecPtr->value;
    ScaleMatrix(matrix, scaleVecPtr);
    MultiplyMatrix(&g_BattleCameraTransform, matrix, matrix);
    SetRotMatrix((void *)matrix);
    SetTransMatrix(matrix);
    g_YasuiLimitGrenadeBombSpriteDrawReqPool.limitGlenadeTumbleRecordWord = sprite->ownerSlot;
    g_YasuiLimitGrenadeBombDrawHandle = BattleFx_SubmitSpriteDraw(&g_YasuiLimitGrenadeBombGrowingScaleMatrix.limitGlenadeTumbleSpriteResource, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitGrenadeBombDrawHandle);
    if (g_BattleIsPaused == 0) {
        (sprite->frameIndex)++;
        sprite->posX += sprite->velocityX;
        sprite->posY += sprite->velocityY;
        sprite->posZ += sprite->velocityZ;
        sprite->velocityX = sprite->velocityX * 0xF00 >> 0xC;
        sprite->velocityY = (sprite->velocityY * 0xF00 >> 0xC) + 0x32;
        sprite->velocityZ = sprite->velocityZ * 0xF00 >> 0xC;
        *(char *)&sprite->angleX += sprite->spinX;
        *(char *)&sprite->angleY += sprite->spinY;
        *(char *)&sprite->angleZ += sprite->spinZ;
    }
}

/*
 * Emits radial smoke sprite particles expanding from the blast center.
 */
// FUNCTION: FF7 0x00472b3d
void __cdecl BattleFx_LimitGrenadeBombRadialSpriteEmitterTask(void)
{
    BattleFxLimitGrenadeBombTimeline *pEmitterTask;
    int angle;
    int i;
    BattleFxGrenadeBombRadialSpriteTask *smoke;

    pEmitterTask = (BattleFxLimitGrenadeBombTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        smoke = (BattleFxGrenadeBombRadialSpriteTask *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitGrenadeBombRadialSpriteTask);
        smoke->pos = pEmitterTask->pos;
        angle = rand() & 0xFFF;
        smoke->offsetX = SinFixed(angle) * 0x4650 >> 0xC;
        smoke->offsetY = 0;
        smoke->offsetZ = CosFixed(angle) * 0x4650 >> 0xC;
        smoke->scale = rand() % 0x800 + 0x1000;
    }
    (pEmitterTask->frameCounter)++;
    if (pEmitterTask->frameCounter >= 0xF) {
        pEmitterTask->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Simulates and renders an expanding radial smoke puff billboard sprite.
 */
// FUNCTION: FF7 0x00472c4b
void __cdecl BattleFx_LimitGrenadeBombRadialSpriteTask(void)
{
    BattleFxLimitGrenadeBombRadialSprite *puff;
    Vec3sPacked *position;

    puff = (BattleFxLimitGrenadeBombRadialSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_YasuiLimitGrenadeBombSpriteDrawReqPool.limitGlenadeRadialSpriteResource = Gfx_ModelTableGetLoadedRecord(4, g_YasuiLimitGrenadeBombLoadedResTable);
    g_YasuiLimitGrenadeBombSpriteDrawReqPool.limitGlenadeRadialSpriteResourceB = Gfx_ModelTableGetSpriteObject(4, g_YasuiLimitGrenadeBombLoadedResTable);
    position = (Vec3sPacked *)Gfx_GetPsxScratchpadPtr(0);
    position->vx = puff->baseX + (puff->expandX * g_YasuiLimitGrenadeBombExpandingShellScale >> 0xC);
    position->vy = puff->posY;
    position->vz = puff->baseZ + (puff->expandZ * g_YasuiLimitGrenadeBombExpandingShellScale >> 0xC);
    BattleFx_InstallBillboardMatrix((short *)position, (short)puff->angle, 0);
    g_YasuiLimitGrenadeBombSpriteDrawReqPool.limitGlenadeRadialSpriteFrameIndex = puff->frameIndex;
    g_YasuiLimitGrenadeBombDrawHandle = BattleFx_SubmitSpriteDraw(&g_YasuiLimitGrenadeBombSpriteDrawReqPool.limitGlenadeRadialSpriteResource, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitGrenadeBombDrawHandle);
    if (g_BattleIsPaused == 0 && g_YasuiLimitGrenadeBombSpriteDrawReqPool.limitGlenadeRadialSpriteResource != 0) {
        (puff->frameIndex)++;
        if (puff->frameIndex >= *(int *)((char *)*(int *)g_YasuiLimitGrenadeBombSpriteDrawReqPool.limitGlenadeRadialSpriteResource + 4)) {
            puff->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Renders dark blast smoke puff model at the grenade detonation point.
 */
// FUNCTION: FF7 0x00472d7a
void __cdecl BattleFx_LimitGrenadeBombBlackBlastModelTask(void)
{
    BattleFxCosmoMemoryExpandingModel *task;
    PsxMatrix *matrix;

    task = (BattleFxCosmoMemoryExpandingModel *)BATTLE_CURRENT_TASK_SLOT;
    g_YasuiLimitGrenadeBombSpriteDrawReqPool.limitGlenadeBlastModelResource = Gfx_ModelTableGetRenderObject(5, g_YasuiLimitGrenadeBombLoadedResTable);
    matrix = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    g_YasuiLimitGrenadeBombSpriteDrawReqPool.limitGlenadeBlastModelScale = (task->frame << 0xC) / 0xA;
    (g_YasuiLimitGrenadeBombBlackBlastMatrixData.scaleY) = (task->frame << 0xD) / 0xA;
    g_YasuiLimitGrenadeBombBlackBlastScaleY = (g_YasuiLimitGrenadeBombBlackBlastMatrixData.scaleY);
    g_YasuiLimitGrenadeBombBlackBlastTransformMatrix = g_YasuiLimitGrenadeBombBlackBlastScaleY;
    g_YasuiLimitGrenadeBombBlackBlastMatrixData.limitGlenadeBlackBlastPosX = task->posX;
    g_YasuiLimitGrenadeBombBlackBlastMatrixData.limitGlenadeBlackBlastPosY = task->posY;
    g_YasuiLimitGrenadeBombBlackBlastMatrixData.limitGlenadeBlackBlastPosZ = task->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &g_YasuiLimitGrenadeBombBlackBlastTransformMatrix, matrix);
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    SetDrawColorRGB(0, 0, 0);
    g_YasuiLimitGrenadeBombDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiLimitGrenadeBombSpriteDrawReqPool.limitGlenadeBlastModelResource, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitGrenadeBombDrawHandle);
    if (g_BattleIsPaused == 0) {
        (task->frame)++;
        if (task->frame >= 0xA) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
