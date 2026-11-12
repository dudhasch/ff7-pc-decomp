// src/ff7/src/battle/battle_fx_special_falling_rocks.cpp
/* Enemy Special Effect [idx 36]: FallingRocks (JP: raku / 落石 (Rockfall), asset: "SPECIAL/RAKU") — Falling Rocks (0x279, Demon's Gate); also called by Battle_LimitFxThunk70 */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Raku attack: it loads resources and spawns a timeline
//   that drives a falling rock model, bouncing sprite particles spawned on impact, splitting and
//   fragment rock models, and a camera shake on impact.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle_3d.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// GLOBAL: FF7 0x008a6658
unsigned int g_BattleSpecialFallingRocksResourceDefs[70] ={
    0x001900b4,
    (unsigned int)&g_BattleSpecialFallingRocksResFilenameIsia,
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b4,
    // STRING: FF7 0x008a6834
    (unsigned int)"isib.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b4,
    // STRING: FF7 0x008a6840
    (unsigned int)"isic.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a684c
    (unsigned int)"isid.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b4,
    // STRING: FF7 0x008a6858
    (unsigned int)"isie.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b4,
    // STRING: FF7 0x008a6864
    (unsigned int)"isif.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b4,
    // STRING: FF7 0x008a6870
    (unsigned int)"isig.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a687c
    (unsigned int)"isid.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a6888
    (unsigned int)"isie.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a6894
    (unsigned int)"isif.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a68a0
    (unsigned int)"isig.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a68ac
    (unsigned int)"rakuse01.tim",
    // STRING: FF7 0x008a68bc
    (unsigned int)"hit1.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a68c8
    (unsigned int)"rakuse00.tim",
    // STRING: FF7 0x008a68d8
    (unsigned int)"kage.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a68e4
    (unsigned int)"rakuse00.tim",
    // STRING: FF7 0x008a68f4
    (unsigned int)"hahen.spt"
};
// GLOBAL: FF7 0x008a6770
struct BattleFxSpriteRequest g_BattleSpecialFallingRocksDebrisSpriteReq = {0, 0, -128, -128, -128, 0x2c, 0, 0};
// GLOBAL: FF7 0x008a6780
struct BattleFxSpriteRequest g_BattleSpecialFallingRocksHitSpriteReq = {0, 0, -128, -128, -128, 0x2c, 0, 0};
// GLOBAL: FF7 0x008a6790
VECTOR g_BattleSpecialFallingRocksFragmentScale = {0x1800, 0x1800, 0x1800, 0};
// GLOBAL: FF7 0x008a67a0
struct BattleSpecialFallingRocksModelReq1 g_BattleSpecialFallingRocksFragmentModelReq = {0, 0, {0}, 128, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x008a67b0
int g_BattleSpecialFallingRocksFragmentModelIndices[8] = {3, 4, 5, 6, 7, 8, 9, 10};
// GLOBAL: FF7 0x008a67d0
struct BattleFxSpriteRequest g_BattleSpecialFallingRocksShadowSpriteReq = {0, 0, -128, -128, -128, 0x2c, 0, 0};
// GLOBAL: FF7 0x008a67e0
VECTOR g_BattleSpecialFallingRocksSplitScale = {0x1800, 0x1800, 0x1800, 0};
// GLOBAL: FF7 0x008a67f0
struct BattleSpecialFallingRocksModelReq1 g_BattleSpecialFallingRocksSplitModelReq = {0, 0, {0}, 128, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x008a6800
int g_BattleSpecialFallingRocksSplitModelIndices[2] = {1, 2};
// GLOBAL: FF7 0x008a6808
VECTOR g_BattleSpecialFallingRocksFallingScale = {0x1800, 0x1800, 0x1800, 0};
// GLOBAL: FF7 0x008a6818
struct BattleSpecialFallingRocksModelReq1 g_BattleSpecialFallingRocksFallingModelReq = {0, 0, {0}, 128, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x008a6828
unsigned char g_BattleSpecialFallingRocksResFilenameIsia[9] = "isia.rsd";
// clang-format on


// clang-format off
unsigned char g_bssPad_00b6efc0[4] = {0};
// GLOBAL: FF7 0x00b6efc4
int g_BattleSpecialFallingRocksDrawListHandle = 0;
// GLOBAL: FF7 0x00b6efc8
unsigned char g_BattleSpecialFallingRocksRotMatrix[0x4] = {0};
unsigned char g_bssPad_00b6efcc[4] = {0};
unsigned char g_bssPad_00b6efd0[8] = {0};
// GLOBAL: FF7 0x00b6efd8
unsigned char g_BattleSpecialFallingRocksMatrix[0x10] = {0};
// GLOBAL: FF7 0x00b6efe8
int g_BattleSpecialFallingRocksModelOffset = 0;
// GLOBAL: FF7 0x00b6efec
struct GfxTableCursor *g_BattleSpecialFallingRocksLoadedRes = 0;
unsigned char g_bssPad_00b6eff0[8] = {0};
// clang-format on

/*
 * Loads resources for the Raku special effect when not already loaded.
 */
// FUNCTION: FF7 0x0054d6f0
void __cdecl BattleFx_SpecialLoadResFallingRocks(void)
{
    GfxModelTableBank *newSceneField;
    GfxResourceRequest requestSlot;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecialFallingRocksLoadedRes == 0) {
        kind = 0xE;
        resourceDefsWord = (int)&g_BattleSpecialFallingRocksResourceDefs;
        newSceneField = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a6900
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/RAKU", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecialFallingRocksLoadedRes, newSceneField);
    }
}

/*
 * Entry point for the Raku special effect, dispatching to begin.
 */
// FUNCTION: FF7 0x0054d780
void __cdecl BattleFx_SpecialEnterFallingRocks(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginFallingRocks(targetMask, actorIdx);
}

/*
 * Initializes and begins the Raku special effect for target actors.
 */
// FUNCTION: FF7 0x0054d795
void __cdecl BattleFx_SpecialBeginFallingRocks(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResFallingRocks();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnFallingRocksPerTarget);
    Battle_PlaySfxPannedToActor(0xD6, (short)actorIdx, 0x40);
}

/*
 * Spawns Raku effect instances for each targeted actor.
 */
// FUNCTION: FF7 0x0054d7ca
void __cdecl BattleFx_SpecialSpawnFallingRocksPerTarget(int actor, int sourceIndex)
{
    BattleFxFallingRocksTimeline *slotPtr;

    slotPtr = (BattleFxFallingRocksTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFallingRocksTimelineTask);
    Battle_GetBoneWorldPos(sourceIndex, g_BattleActorRecords[sourceIndex].bone30, &slotPtr->sourcePos.vx);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &slotPtr->targetPos.vx);
    slotPtr->actorIndex = actor;
}

/*
 * Timeline task controlling state progression for the Raku special effect.
 */
// FUNCTION: FF7 0x0054d840
void __cdecl BattleFx_SpecialFallingRocksTimelineTask(void)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxFallingRocksTimeline *pCur;
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int s; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int t; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int y; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int a; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int b; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short i9;
    BattleFxFallingRocksTimelineTask *sprite;
    int e; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int f; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int g; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int h; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int j; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int k; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    pCur = (BattleFxFallingRocksTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (pCur->step == 0) {
        sprite = (BattleFxFallingRocksTimelineTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFallingRocksFallingModelTask);
        Battle_GetBoneWorldPos(pCur->actorIndex, g_BattleActorRecords[pCur->actorIndex].targetBone, &sprite->posX);
        sprite->posY = 0xF830;
        sprite->velocityY = 0x64;
    }
    if (pCur->step == 4) {
        for (i9 = 0; i9 < 5; i9++) {
            sprite = (BattleFxFallingRocksTimelineTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFallingRocksAnimatedSpriteTask);
            Battle_GetBoneWorldPos(pCur->actorIndex, g_BattleActorRecords[pCur->actorIndex].targetBone, &sprite->posX);
            sprite->posX += (short)(rand() % 0x320 - 0x190);
            sprite->posY += (short)(rand() % 0x190 - 0x320);
            sprite->posZ += (short)(rand() % 0x320 - 0x190);
        }
    }
    if (pCur->step == 5 || pCur->step == 0x14) {
        Battle_AllocateResourceSlot(BattleFx_SpecialFallingRocksCameraShakeTask);
    }
    if (pCur->step == 5) {
        Battle_PostActorHitOrHurtMessage(pCur->actorIndex);
    }
    (pCur->step)++;
    if (pCur->step == 0x32) {
        pCur->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task to animate and render sprites for the Raku special effect.
 */
// FUNCTION: FF7 0x0054da23
void __cdecl BattleFx_SpecialFallingRocksAnimatedSpriteTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialFallingRocksHitSpriteReq.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0xB, g_BattleSpecialFallingRocksLoadedRes);
    g_BattleSpecialFallingRocksHitSpriteReq.drawObj = Gfx_ModelTableGetSpriteObject(0xB, g_BattleSpecialFallingRocksLoadedRes);
    BattleFx_InstallBillboardMatrix((short *)task->data, 0x1800, 0);
    g_BattleSpecialFallingRocksHitSpriteReq.frame = task->frameCounter;
    g_BattleSpecialFallingRocksDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialFallingRocksHitSpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialFallingRocksDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (g_BattleSpecialFallingRocksHitSpriteReq.spriteGroup != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= g_BattleSpecialFallingRocksHitSpriteReq.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Task to animate and render falling rock models for Raku.
 */
// FUNCTION: FF7 0x0054daf6
void __cdecl BattleFx_SpecialFallingRocksFallingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[96];
        SVECTOR angleX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleFxFallingRocksBouncingFragmentModelSlot *splitSlot; };
        struct { unsigned char pad_to_2C[0x2C]; short splitIndex; };
        struct { unsigned char pad_to_3C[0x3C]; BattleFxFallingRocksFallingModelSpawnSlot *slot; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxFallingRocksFallingModelSpawnSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialFallingRocksFallingModelReq.modelResource = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialFallingRocksLoadedRes);
    frame.angleX.vz = 0;
    frame.angleX.vy = frame.angleX.vz;
    frame.angleX.vx = frame.angleX.vy;
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialFallingRocksFallingScale);
    frame.rotMatrix.t[0] = frame.slot->startX;
    frame.rotMatrix.t[1] = frame.slot->startY;
    frame.rotMatrix.t[2] = frame.slot->startZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    g_BattleSpecialFallingRocksDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialFallingRocksFallingModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialFallingRocksDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.slot->velocityY += 0x1E;
    frame.slot->startY += frame.slot->velocityY;
    (frame.slot->frameCounter)++;
    if (frame.slot->frameCounter == 5) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
        for (frame.splitIndex = 0; frame.splitIndex < 2; frame.splitIndex++) {
            frame.splitSlot = (BattleFxFallingRocksBouncingFragmentModelSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFallingRocksSplittingSpinModelTask);
            frame.splitSlot->posX = frame.slot->startX;
            frame.splitSlot->posY = frame.slot->startY;
            frame.splitSlot->posZ = frame.slot->startZ;
            frame.splitSlot->velocityX = (frame.splitIndex != 0 ? -100 : 0) + 0x32;
            frame.splitSlot->velocityY = 0xFF74;
            frame.splitSlot->velocityZ = rand() % 0xC8 - 0x64;
            frame.splitSlot->angle = 0;
            frame.splitSlot->angleVelocity = (frame.splitIndex != 0 ? -200 : 0) + 0x64;
            frame.splitSlot->modelIndex = frame.splitIndex;
        }
        BattleFx_SpecialFallingRocksSpawnBouncingSprites(frame.slot, 0xFFFFFF74);
    }
}

/*
 * Spawns bouncing sprite particles upon rock impact for Raku.
 */
// FUNCTION: FF7 0x0054dd0a
void __cdecl BattleFx_SpecialFallingRocksSpawnBouncingSprites(BattleFxFallingRocksFallingModelSpawnSlot *origin, short value)
{
    short angle, magnitude;
    BattleFxFallingRocksBouncer *pBouncer;
    short i;

    for (i = 0; i < 6; i++) {
        pBouncer = (BattleFxFallingRocksBouncer *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFallingRocksBouncingSpriteTask);
        pBouncer->startX = origin->startX;
        pBouncer->startY = origin->startY;
        pBouncer->startZ = origin->startZ;
        magnitude = rand() % 100 + 100;
        angle = rand() % 0x1000;
        pBouncer->velocityX = (CosFixed(angle) * magnitude) >> 0xC;
        pBouncer->velocityZ = (SinFixed(angle) * magnitude) >> 0xC;
        pBouncer->velocityY = value;
        pBouncer->randomIndex = rand() % 16;
    }
}

/*
 * Task to animate bouncing sprite particles for the Raku effect.
 */
// FUNCTION: FF7 0x0054de0a
void __cdecl BattleFx_SpecialFallingRocksBouncingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[12];
        short angleX;
        struct { unsigned char pad_to_2[0x2]; short angleY; };
        struct { unsigned char pad_to_4[0x4]; short angleZ; };
        struct { unsigned char pad_to_8[0x8]; BattleFxFallingRocksBouncingSpriteSlot *slot; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxFallingRocksBouncingSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialFallingRocksDebrisSpriteReq.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0xD, g_BattleSpecialFallingRocksLoadedRes);
    g_BattleSpecialFallingRocksDebrisSpriteReq.drawObj = Gfx_ModelTableGetSpriteObject(0xD, g_BattleSpecialFallingRocksLoadedRes);
    frame.angleY = 0;
    frame.angleX = frame.angleY;
    frame.angleZ = frame.slot->frameCounter << 8;
    BattleFx_SpecialFallingRocksInstallScaledSpriteMatrix(&frame.slot->posX, &frame.angleX, 0x1800);
    g_BattleSpecialFallingRocksDebrisSpriteReq.frame = frame.slot->color;
    g_BattleSpecialFallingRocksDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialFallingRocksDebrisSpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialFallingRocksDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.slot->velocityY += 0x28;
    frame.slot->posX += frame.slot->velocityX;
    frame.slot->posY += frame.slot->velocityY;
    frame.slot->posZ += frame.slot->velocityZ;
    if (frame.slot->posY > 0) {
        frame.slot->posY = -frame.slot->posY;
        frame.slot->velocityY = -frame.slot->velocityY;
    }
    (frame.slot->frameCounter)++;
    if (frame.slot->frameCounter == 0xF) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Installs scaled sprite transformation matrices for bouncing particles.
 */
// FUNCTION: FF7 0x0054df6a
int __cdecl BattleFx_SpecialFallingRocksInstallScaledSpriteMatrix(void *point, short *rotation, short scale)
{
    int depth;
    PsxMatrix matrix = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint(point, matrix.t, &depth);
    matrix.m[0][0] = matrix.m[1][1] = matrix.m[2][2] = scale;
    ConcatMatrixIntoFirst(&matrix, &g_BattleCameraTransform);
    RotMatrix((SVECTOR *)rotation, (int *)&g_BattleSpecialFallingRocksRotMatrix);
    ConcatMatrixIntoFirst(&matrix, &g_BattleSpecialFallingRocksRotMatrix);
    SetRotMatrix(&matrix);
    SetTransMatrix(&matrix);
    *(int *)((char *)&g_BattleSpecialFallingRocksMatrix + 0x2) = matrix.t[0];
    *(int *)((char *)&g_BattleSpecialFallingRocksMatrix + 0x6) = matrix.t[1];
    *(int *)((char *)&g_BattleSpecialFallingRocksMatrix + 0xa) = matrix.t[2];
    return (int)&g_BattleSpecialFallingRocksRotMatrix;
}

/*
 * Task to animate spinning and splitting rock models for Raku.
 */
// FUNCTION: FF7 0x0054e065
void __cdecl BattleFx_SpecialFallingRocksSplittingSpinModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[96];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleFxFallingRocksBouncingFragmentModelSlot *fragmentSlot; };
        struct { unsigned char pad_to_2C[0x2C]; short fragmentIndex; };
        struct { unsigned char pad_to_3C[0x3C]; BattleFxFallingRocksFallingModelSpawnSlot *spawnSlot; };
    } frame;
#pragma pack(pop)

    frame.spawnSlot = (BattleFxFallingRocksFallingModelSpawnSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialFallingRocksSplitModelReq.modelResource = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialFallingRocksLoadedRes);
    frame.rotX.vy = 0;
    frame.rotX.vx = frame.rotX.vy;
    frame.rotX.vz = frame.spawnSlot->angle;
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialFallingRocksSplitScale);
    frame.rotMatrix.t[0] = frame.spawnSlot->startX;
    frame.rotMatrix.t[1] = frame.spawnSlot->startY;
    frame.rotMatrix.t[2] = frame.spawnSlot->startZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    g_BattleSpecialFallingRocksSplitModelReq.modelResource = Gfx_ModelTableGetRenderObject(g_BattleSpecialFallingRocksSplitModelIndices[frame.spawnSlot->modelIndex], g_BattleSpecialFallingRocksLoadedRes);
    g_BattleSpecialFallingRocksDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialFallingRocksSplitModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialFallingRocksDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spawnSlot->velocityY += 0xA;
    frame.spawnSlot->startX += frame.spawnSlot->velocityX;
    frame.spawnSlot->startY += frame.spawnSlot->velocityY;
    frame.spawnSlot->startZ += frame.spawnSlot->velocityZ;
    frame.spawnSlot->angle += frame.spawnSlot->angleVelocity;
    (frame.spawnSlot->frameCounter)++;
    if (frame.spawnSlot->frameCounter == 0xA) {
        frame.spawnSlot->ownerSlot = BATTLE_TASK_FREE;
        for (frame.fragmentIndex = 0; frame.fragmentIndex < 2; frame.fragmentIndex++) {
            frame.fragmentSlot = (BattleFxFallingRocksBouncingFragmentModelSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFallingRocksBouncingFragmentModelTask);
            frame.fragmentSlot->posX = frame.spawnSlot->startX;
            frame.fragmentSlot->posY = frame.spawnSlot->startY;
            frame.fragmentSlot->posZ = frame.spawnSlot->startZ;
            frame.fragmentSlot->velocityX = frame.spawnSlot->velocityX + (frame.fragmentIndex != 0 ? -40 : 40);
            frame.fragmentSlot->velocityY = frame.spawnSlot->velocityY;
            frame.fragmentSlot->velocityZ = rand() % 0xC8 - 0x64;
            frame.fragmentSlot->angle = frame.spawnSlot->angle;
            frame.fragmentSlot->angleVelocity = (frame.fragmentIndex != 0 ? -200 : 0) + 0x64;
            frame.fragmentSlot->modelIndex = frame.spawnSlot->modelIndex * 2 + frame.fragmentIndex;
        }
        BattleFx_SpecialFallingRocksSpawnBouncingSprites(frame.spawnSlot, (short)frame.spawnSlot->velocityY);
    }
}

/*
 * Task to animate bouncing rock fragment models for Raku.
 */
// FUNCTION: FF7 0x0054e2fe
void __cdecl BattleFx_SpecialFallingRocksBouncingFragmentModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        int phaseInit;
        struct { unsigned char pad_to_4[0x4]; short rotX; };
        struct { unsigned char pad_to_6[0x6]; short rotY; };
        struct { unsigned char pad_to_8[0x8]; short angle; };
        struct { unsigned char pad_to_C[0xC]; short spriteX; };
        struct { unsigned char pad_to_E[0xE]; short spriteY; };
        struct { unsigned char pad_to_10[0x10]; short spriteZ; };
        struct { unsigned char pad_to_14[0x14]; int spriteRecord; };
        struct { unsigned char pad_to_18[0x18]; short phase; };
        struct { unsigned char pad_to_1C[0x1C]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_44[0x44]; short heightFactor; };
        struct { unsigned char pad_to_54[0x54]; BattleFxFallingRocksBouncingFragmentModelSlot *fragmentSlot; };
    } frame;
#pragma pack(pop)

    frame.fragmentSlot = (BattleFxFallingRocksBouncingFragmentModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.fragmentSlot->frameCounter == 0) {
        g_BattleSpecialFallingRocksModelOffset = 0;
    }
    g_BattleSpecialFallingRocksFragmentModelReq.modelResource = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialFallingRocksLoadedRes);
    g_BattleSpecialFallingRocksShadowSpriteReq.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0xC, g_BattleSpecialFallingRocksLoadedRes);
    g_BattleSpecialFallingRocksShadowSpriteReq.drawObj = Gfx_ModelTableGetSpriteObject(0xC, g_BattleSpecialFallingRocksLoadedRes);
    if (frame.fragmentSlot->frameCounter < 0xC) {
        frame.phaseInit = 8;
    } else {
        frame.phaseInit = 0x14 - frame.fragmentSlot->frameCounter;
    }
    frame.phase = *(short *)&frame.phaseInit;
    frame.rotY = 0;
    frame.rotX = frame.rotY;
    frame.angle = frame.fragmentSlot->angle;
    RotMatrix((SVECTOR *)&frame.rotX, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialFallingRocksFragmentScale);
    frame.rotMatrix.t[0] = frame.fragmentSlot->posX;
    frame.rotMatrix.t[1] = frame.fragmentSlot->posY;
    frame.rotMatrix.t[2] = frame.fragmentSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    g_BattleSpecialFallingRocksFragmentModelReq.flags = frame.phase - 8 != 0 ? 8 : 0;
    g_BattleSpecialFallingRocksFragmentModelReq.angle = frame.phase << 4;
    g_BattleSpecialFallingRocksFragmentModelReq.modelResource = Gfx_ModelTableGetRenderObject(g_BattleSpecialFallingRocksFragmentModelIndices[frame.fragmentSlot->modelIndex + g_BattleSpecialFallingRocksModelOffset], g_BattleSpecialFallingRocksLoadedRes);
    g_BattleSpecialFallingRocksDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialFallingRocksFragmentModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialFallingRocksDrawListHandle);
    frame.heightFactor = 0x80 - (-frame.fragmentSlot->posY >> 4);
    if (frame.heightFactor > 0) {
        frame.rotX = 0x400;
        frame.angle = 0;
        frame.rotY = frame.angle;
        frame.spriteX = frame.fragmentSlot->posX;
        frame.spriteZ = frame.fragmentSlot->posZ;
        frame.spriteY = 0;
        frame.spriteRecord = BattleFx_SpecialFallingRocksInstallScaledSpriteMatrix(&frame.spriteX, &frame.rotX, 0x2000);
        if (((PsxMatrix *)frame.spriteRecord)->t[2] > 0) {
            g_BattleSpecialFallingRocksModelOffset = 4;
            g_BattleSpecialFallingRocksShadowSpriteReq.colorB = frame.heightFactor * frame.phase >> 3;
            g_BattleSpecialFallingRocksShadowSpriteReq.colorG = g_BattleSpecialFallingRocksShadowSpriteReq.colorB;
            g_BattleSpecialFallingRocksShadowSpriteReq.colorR = g_BattleSpecialFallingRocksShadowSpriteReq.colorG;
            g_BattleSpecialFallingRocksDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialFallingRocksShadowSpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialFallingRocksDrawListHandle);
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.fragmentSlot->velocityY += 0x50;
    frame.fragmentSlot->posX += frame.fragmentSlot->velocityX;
    frame.fragmentSlot->posY += frame.fragmentSlot->velocityY;
    frame.fragmentSlot->posZ += frame.fragmentSlot->velocityZ;
    if (frame.fragmentSlot->posY > -0x64) {
        frame.fragmentSlot->velocityY = -frame.fragmentSlot->velocityY;
        frame.fragmentSlot->posY = 0xFF9C;
    }
    frame.fragmentSlot->angle += frame.fragmentSlot->angleVelocity;
    if (frame.fragmentSlot->frameCounter > 0xC) {
        frame.phase--;
    }
    (frame.fragmentSlot->frameCounter)++;
    if (frame.fragmentSlot->frameCounter == 0x14) {
        frame.fragmentSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task to perform camera shake effects on rock impact for Raku.
 */
// FUNCTION: FF7 0x0054e637
void __cdecl BattleFx_SpecialFallingRocksCameraShakeTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    g_BattleCameraFocalPoint.focalPointY += (short)((6 - task->frameCounter) * 5 * (1 - (task->frameCounter % 2 << 1)));
    (task->frameCounter)++;
    if (task->frameCounter == 6) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
