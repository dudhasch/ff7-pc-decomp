// src/ff7/src/battle/battle_special_missile_salvo_effects.cpp
/* Enemy Special Attack [105]: Missile Salvo (orig. SPECIAL/RENMISA, 連続ミサイル) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Renmisa spell. It loads its resources, registers a
//   per-target callback, and runs the timeline, bouncing sprite, growing and impact model, animated
//   sprite, and projectile tasks.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x0089aee8
int g_BattleSpecMissileSalvoResourceDef = 0x001900B1;
char *g_BattleSpecialMissileSalvoEffectsResourceNames[1] = {
    // STRING: FF7 0x0089b018
    "MISAIRU.RSD"
};
unsigned int g_BattleSpecMissileSalvoResourceDefDataTail[42] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0089b024
    (unsigned int)"WA.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x003200b1,
    // STRING: FF7 0x0089b02c
    (unsigned int)"SINDOM1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0089b038
    (unsigned int)"HIKARI.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089b044
    (unsigned int)"RENZOK00.TIM",
    // STRING: FF7 0x0089b054
    (unsigned int)"HUNSYA.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089b060
    (unsigned int)"RENZOK00.TIM",
    // STRING: FF7 0x0089b070
    (unsigned int)"KEMU1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089b07c
    (unsigned int)"RENZOK00.TIM",
    // STRING: FF7 0x0089b08c
    (unsigned int)"HAHEN1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    0x00000000,
    // STRING: FF7 0x0089b098
    (unsigned int)"JO_A.SPT",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x2c404040,
    0x00000000
};
#undef g_BattleSpecMissileSalvoSpriteRequest1
// GLOBAL: FF7 0x0089af98
struct BattleSpecMissileSalvoSpriteRequest1 g_BattleSpecMissileSalvoSpriteRequest1 = {0, 0, {0x40, 0x40, 0x40, 0x2e}, 0, {0}, 0, 0};
// GLOBAL: FF7 0x0089afb0
struct BattleSpecMissileSalvoSpriteRequest2 g_BattleSpecMissileSalvoSpriteRequest2 = {-128, -128, -128, 44, 0, {0}, 0, {0x88, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}, 0, 0, {0}, 128};
unsigned char g_rawData_0089afd4[4] = {0x20, 0x00, 0x00, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x0089afd8
struct BattleSpecMissileSalvoTransformMatrix g_BattleSpecMissileSalvoTransformMatrix = {{0, 8, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0}, 0, {0x80, 0, 0, 0, 0, 0, 0x80, 0, 0x20, 0, 0, 0}, 0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}, 0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b5a850
int g_BattleSpecMissileSalvoDrawListHead = 0;
// GLOBAL: FF7 0x00b5a854
short g_BattleSpecMissileSalvoImpactCountdown = 0;
unsigned char g_bssPad_00b5a858[4] = {0};
// GLOBAL: FF7 0x00b5a85c
struct GfxTableCursor *g_BattleSpecMissileSalvoLoadedResTable = 0;
// GLOBAL: FF7 0x00b5a860
unsigned char g_BattleSpecMissileSalvoBuffer[4] = {0};
unsigned char g_bssPad_00b5a864[4] = {0};
// clang-format on

/*
 * When the engine context exists and g_BattleSpecMissileSalvoLoadedResTable is empty, builds SPECIAL/RENMISA with a default-
 * duration record and creates resource type eight using g_BattleSpecMissileSalvoResourceDef.
 */
// FUNCTION: FF7 0x0051b250
void __cdecl BattleFx_SpecialLoadResMissileSalvo(void)
{
    GfxModelTableBank *ownerHandle;
    GfxResourceRequest recBlock;
    int kind;
    char pathDstBuf[200];
    GfxResourceRequest *pRequest;
    int resourceDefAddr;

    if (g_pBattle3dScene != 0 && g_BattleSpecMissileSalvoLoadedResTable == 0) {
        kind = 8;
        resourceDefAddr = (int)&g_BattleSpecMissileSalvoResourceDef;
        ownerHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &recBlock;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0089b0a4
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/RENMISA", pathDstBuf);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefAddr, pRequest, &g_BattleSpecMissileSalvoLoadedResTable, ownerHandle);
    }
}

/*
 * Loads the resources, points g_BattleSpecMissileSalvoDrawListHead at the packet scratch area, sets
 * g_BattleSpecMissileSalvoImpactCountdown to 0x10, then registers the per-target callback.
 */
// FUNCTION: FF7 0x0051b2e0
void __cdecl BattleFx_SpecialEnterMissileSalvo(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResMissileSalvo();
    g_BattleSpecMissileSalvoDrawListHead = (int)&g_GfxPsxPacketScratch;
    g_BattleSpecMissileSalvoImpactCountdown = 16;
    BattleFx_SpecialRegisterMissileSalvoPerTargetCallback(targetMask, actorIdx);
}

/*
 * Creates a mode-four actor-mask callback task using both arguments and BattleFx_SpecialSpawnMissileSalvoPerTarget.
 */
// FUNCTION: FF7 0x0051b30d
void __cdecl BattleFx_SpecialRegisterMissileSalvoPerTargetCallback(int actorMask, int sourceIndex)
{
    Battle_CreateBitPatternCallbackTask(actorMask, sourceIndex, 4, BattleFx_SpecialSpawnMissileSalvoPerTarget);
}

// Allocates a BattleFx_SpecialMissileSalvoTimelineTask slot. Its start position is the world position of
// the source actor's bone34, and its end position is the world position of the target actor's
// targetBone (both via Battle_GetBoneWorldPos). The record then stores actor and sourceIndex.
// FUNCTION: FF7 0x0051b329
void __cdecl BattleFx_SpecialSpawnMissileSalvoPerTarget(int actor, int sourceIndex)
{
    BattleFxMissileSalvoTrack *effectSlot;

    effectSlot = (BattleFxMissileSalvoTrack *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialMissileSalvoTimelineTask);
    Battle_GetBoneWorldPos(sourceIndex, g_BattleActorRecords[sourceIndex].bone34, (short *)&effectSlot->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)&effectSlot->endX);
    effectSlot->targetIndex = actor;
    effectSlot->sourceIndex = sourceIndex;
}

/*
 * Per-target timeline: launches projectiles in the first frames, then at frame 0x14 the impact model, growing model and 20 bouncing sprites, and finishes at frame 0x3C.
 */
// FUNCTION: FF7 0x0051b3aa
void __cdecl BattleFx_SpecialMissileSalvoTimelineTask(void)
{
    int p, q;
    BattleFxMissileSalvoTrack *taskPtr;
    int s, t, u, v, w, x, y, z, a, b, c;
    short currIndex;
    BattleFxMissileSalvoSegment *pChild;
    int f, g, h, j, k;

    taskPtr = (BattleFxMissileSalvoTrack *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->frame <= 5 && taskPtr->frame % 5 == 0) {
        pChild = (BattleFxMissileSalvoSegment *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialMissileSalvoProjectileTask);
        pChild->posX = taskPtr->startX;
        pChild->posY = taskPtr->startY - taskPtr->frame * 30;
        pChild->posZ = taskPtr->startZ;
        pChild->angle = ArcTangentFixed(taskPtr->endX - taskPtr->startX, taskPtr->endZ - taskPtr->startZ);
        pChild->stepX = (taskPtr->endX - taskPtr->startX) / 15;
        pChild->stepY = (taskPtr->endY - taskPtr->startY) / 15;
        pChild->stepZ = (taskPtr->endZ - taskPtr->startZ) / 15;
        pChild->frameCount = 50;
        Battle_PlaySfxPannedToActor(0x82, (short)taskPtr->sourceIndex, 0x40);
    }
    if (taskPtr->frame == 50) {
        Battle_PostActorHitOrHurtMessage(taskPtr->targetIndex);
    }
    if (taskPtr->frame == 20) {
        pChild = (BattleFxMissileSalvoSegment *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialMissileSalvoImpactModelTask);
        pChild->posX = taskPtr->endX;
        pChild->posY = 0;
        pChild->posZ = taskPtr->endZ;
        pChild->variant = 0;
        pChild = (BattleFxMissileSalvoSegment *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialMissileSalvoGrowingModelTask);
        pChild->posX = taskPtr->endX;
        pChild->posY = 0xFED4;
        pChild->posZ = taskPtr->endZ;
        for (currIndex = 0; currIndex < 20; currIndex++) {
            pChild = (BattleFxMissileSalvoSegment *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialMissileSalvoBouncingSpriteTask);
            pChild->posX = taskPtr->endX;
            pChild->posZ = taskPtr->endZ;
            pChild->posY = 0;
            BattleFx_MakeRandomConeVector(0xFFFFFF06, 100, (void *)&pChild->stepX);
            pChild->variant = rand() % 7;
        }
        Battle_PlaySfxPannedToActor(0x176, (short)taskPtr->targetIndex, 0x40);
    }
    (taskPtr->frame)++;
    if (taskPtr->frame == 60) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws one bouncing fragment sprite, moves it under gravity, bounces it at the floor and frees it after 30 frames.
 */
// FUNCTION: FF7 0x0051b67d
void __cdecl BattleFx_SpecialMissileSalvoBouncingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int fadeLevel;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotAngleX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; short fadeLevelCopy; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix matrixBase; };
        struct { unsigned char pad_to_50[0x50]; short axisIndex; };
        struct { unsigned char pad_to_54[0x54]; int transDepth; };
        struct { unsigned char pad_to_58[0x58]; BattleFxMissileSalvoBouncingSpriteSlot *spriteSlot; };
    } frame;
#pragma pack(pop)

    frame.spriteSlot = (BattleFxMissileSalvoBouncingSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecMissileSalvoSpriteRequest1.record = Gfx_ModelTableGetLoadedRecord(6, g_BattleSpecMissileSalvoLoadedResTable);
    g_BattleSpecMissileSalvoSpriteRequest1.drawObj = Gfx_ModelTableGetSpriteObject(6, g_BattleSpecMissileSalvoLoadedResTable);
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.matrixBase.m[2][frame.axisIndex] = 0;
        frame.matrixBase.m[1][frame.axisIndex] = 0;
        frame.matrixBase.m[0][frame.axisIndex] = 0;
    }
    if (frame.spriteSlot->frameCounter < 22) {
        frame.fadeLevel = 8;
    } else {
        frame.fadeLevel = 30 - frame.spriteSlot->frameCounter;
    }
    frame.fadeLevelCopy = *(short *)&frame.fadeLevel;
    frame.rotAngleX.vy = 0;
    frame.rotAngleX.vx = frame.rotAngleX.vy;
    frame.rotAngleX.vz = frame.spriteSlot->angle;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((SVECTOR *)&frame.spriteSlot->posX, frame.matrixBase.t, &frame.transDepth);
    frame.matrixBase.m[2][2] = 0x2000;
    frame.matrixBase.m[1][1] = frame.matrixBase.m[2][2];
    frame.matrixBase.m[0][0] = frame.matrixBase.m[1][1];
    RotMatrix(&frame.rotAngleX, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.matrixBase, &frame.rotMatrix);
    SetRotMatrix(&frame.matrixBase);
    SetTransMatrix(&frame.matrixBase);
    g_BattleSpecMissileSalvoSpriteRequest2.req2Word4 = frame.spriteSlot->variant;
    g_BattleSpecMissileSalvoSpriteRequest2.req2Byte3 = (frame.fadeLevelCopy - 8 != 0 ? 2 : 0) + 0x2C;
    g_BattleSpecMissileSalvoSpriteRequest2.req2Byte2 = frame.fadeLevelCopy << 4;
    g_BattleSpecMissileSalvoSpriteRequest2.req2Byte1 = g_BattleSpecMissileSalvoSpriteRequest2.req2Byte2;
    (g_BattleSpecMissileSalvoSpriteRequest2.red) = g_BattleSpecMissileSalvoSpriteRequest2.req2Byte1;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecMissileSalvoSpriteRequest1.record, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spriteSlot->posX += frame.spriteSlot->velocityX;
    frame.spriteSlot->posY += frame.spriteSlot->velocityY;
    frame.spriteSlot->posZ += frame.spriteSlot->velocityZ;
    if (frame.spriteSlot->posY > 0) {
        frame.spriteSlot->posY = 0;
        frame.spriteSlot->velocityY *= -1;
    }
    frame.spriteSlot->velocityY += 20;
    frame.spriteSlot->angle += 0x200 /* 45 deg */;
    (frame.spriteSlot->frameCounter)++;
    if (frame.spriteSlot->frameCounter == 30) {
        frame.spriteSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the growing model at the impact point, scaled by its growth step, until step 0xF.
 */
// FUNCTION: FF7 0x0051b8f7
void __cdecl BattleFx_SpecialMissileSalvoGrowingModelTask(void)
{
    struct {
        PsxMatrix first;
        char pad0[6];
        BattleFxMissileSalvoGrowingRecord *slot;
        PsxMatrix second;
        char pad1[2];
        int scale[3];
        int unused;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.slot = (BattleFxMissileSalvoGrowingRecord *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecMissileSalvoSpriteRequest2.growingModelRecord = Gfx_ModelTableGetRenderObject(1, g_BattleSpecMissileSalvoLoadedResTable);
    RotMatrixXYZ(&g_BattleSpecMissileSalvoBuffer, &local.first);
    local.scale[2] = local.slot->growStep * 0x5DC;
    local.scale[1] = local.scale[2];
    local.scale[0] = local.scale[1];
    ScaleMatrix(&local.first, (void *)local.scale);
    local.first.t[0] = local.slot->posX;
    local.first.t[1] = local.slot->posY;
    local.first.t[2] = local.slot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &local.first, &local.second);
    SetRotMatrix(&local.second);
    SetTransMatrix(&local.second);
    g_BattleSpecMissileSalvoSpriteRequest2.req2Word18 = local.slot->growStep << 8;
    g_BattleSpecMissileSalvoDrawListHead = BattleFx_SubmitModelDraw(
        (BattleFxModelDraw *)&g_BattleSpecMissileSalvoSpriteRequest2.growingModelRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC,
        g_BattleSpecMissileSalvoDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (local.slot->growStep)++;
    if (local.slot->growStep == 15) {
        local.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the spinning, growing impact models, emits ring sprites for the first 22 frames and shakes the camera focal point.
 */
// FUNCTION: FF7 0x0051ba1a
void __cdecl BattleFx_SpecialMissileSalvoImpactModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[144];
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotAngleX; };
        struct { unsigned char pad_to_18[0x18]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_38[0x38]; BattleFxMissileSalvoImpactSlot *impactSlot; };
        struct { unsigned char pad_to_3C[0x3C]; short spawnIndex; };
        struct { unsigned char pad_to_48[0x48]; BattleFxOilTargetModelSlot *modelSlot; };
        struct { unsigned char pad_to_4C[0x4C]; short spawnRadius; };
        struct { unsigned char pad_to_50[0x50]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_70[0x70]; short spawnAngle; };
        struct { unsigned char pad_to_80[0x80]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.modelSlot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotAngleX.vy = frame.modelSlot->frameCounter << 6;
    frame.rotAngleX.vz = 0;
    frame.rotAngleX.vx = frame.rotAngleX.vz;
    RotMatrixXYZ(&frame.rotAngleX, &frame.rotMatrix);
    frame.scaleVec.vz = frame.modelSlot->frameCounter * 0x12C + 0x1000;
    frame.scaleVec.vy = frame.scaleVec.vz;
    frame.scaleVec.vx = frame.scaleVec.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleVec);
    frame.rotMatrix.t[0] = frame.modelSlot->posX;
    frame.rotMatrix.t[1] = frame.modelSlot->posY;
    frame.rotMatrix.t[2] = frame.modelSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecMissileSalvoSpriteRequest2.req2Int28 = 0x28;
    g_BattleSpecMissileSalvoSpriteRequest2.req2Word34 = g_BattleSpecMissileSalvoImpactCountdown << 3;
    g_BattleSpecMissileSalvoSpriteRequest2.modelRecord = Gfx_ModelTableGetRenderObject(2, g_BattleSpecMissileSalvoLoadedResTable);
    g_BattleSpecMissileSalvoDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecMissileSalvoSpriteRequest2.modelRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecMissileSalvoDrawListHead);
    g_BattleSpecMissileSalvoSpriteRequest2.req2Int28 = 0xA8;
    g_BattleSpecMissileSalvoSpriteRequest2.modelRecord = Gfx_ModelTableGetRenderObject(3, g_BattleSpecMissileSalvoLoadedResTable);
    g_BattleSpecMissileSalvoSpriteRequest2.req2Word34 = 0x1000 - (g_BattleSpecMissileSalvoImpactCountdown << 9);
    g_BattleSpecMissileSalvoDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecMissileSalvoSpriteRequest2.modelRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecMissileSalvoDrawListHead);
    frame.rotAngleX.vy = -frame.modelSlot->frameCounter << 6;
    frame.rotAngleX.vz = 0;
    frame.rotAngleX.vx = frame.rotAngleX.vz;
    RotMatrixXYZ(&frame.rotAngleX, &frame.rotMatrix);
    frame.scaleVec.vz = frame.modelSlot->frameCounter * 0x118 + 0x800;
    frame.scaleVec.vy = frame.scaleVec.vz;
    frame.scaleVec.vx = frame.scaleVec.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleVec);
    frame.rotMatrix.t[0] = frame.modelSlot->posX;
    frame.rotMatrix.t[1] = frame.modelSlot->posY;
    frame.rotMatrix.t[2] = frame.modelSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecMissileSalvoSpriteRequest2.req2Int28 = 0x28;
    g_BattleSpecMissileSalvoSpriteRequest2.req2Word34 = g_BattleSpecMissileSalvoImpactCountdown << 3;
    g_BattleSpecMissileSalvoSpriteRequest2.modelRecord = Gfx_ModelTableGetRenderObject(2, g_BattleSpecMissileSalvoLoadedResTable);
    g_BattleSpecMissileSalvoDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecMissileSalvoSpriteRequest2.modelRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecMissileSalvoDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.modelSlot->frameCounter < 22) {
        frame.spawnRadius = frame.modelSlot->frameCounter * 80;
        for (frame.spawnIndex = 0; frame.spawnIndex < 6; frame.spawnIndex++) {
            frame.spawnAngle = (rand() % 4096);
            frame.impactSlot = (BattleFxMissileSalvoImpactSlot *)(&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialMissileSalvoAnimatedSpriteTask)]);
            frame.impactSlot->posX = frame.modelSlot->posX + (CosFixed(frame.spawnAngle) * frame.spawnRadius >> 12);
            frame.impactSlot->posZ = frame.modelSlot->posZ + (SinFixed(frame.spawnAngle) * frame.spawnRadius >> 12);
            frame.impactSlot->posY = 0xFF6A;
            frame.impactSlot->scale = 2;
        }
    }
    g_BattleCameraFocalPoint.focalPointY += (short)(((frame.modelSlot->frameCounter % 2) << 4) - 8);
    if (frame.modelSlot->frameCounter > 14) {
        g_BattleSpecMissileSalvoImpactCountdown--;
    }
    (frame.modelSlot->frameCounter)++;
    if (frame.modelSlot->frameCounter == 30) {
        frame.modelSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Per-frame tick of a sprite in the secondary task slots (not the primary slots its neighbours use):
 * fills g_BattleSpecMissileSalvoSpriteRequest1 from resource 7 and the scene object, installs the billboard
 * matrix at the slot's position, publishes the frame counter and chains the sprite onto
 * g_BattleOverlayDrawListHead. While unpaused it advances the frame counter and frees the slot
 * once it reaches the sprite's animation frame count.
 */
// FUNCTION: FF7 0x0051be08
void __cdecl BattleFx_SpecialMissileSalvoAnimatedSpriteTask(void)
{
    BattleTaskSlot *task;

    task = (BattleTaskSlot *)(&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx]);
    (g_BattleSpecMissileSalvoSpriteRequest1.spriteResource) = Gfx_ModelTableGetLoadedRecord(7, g_BattleSpecMissileSalvoLoadedResTable);
    g_BattleSpecMissileSalvoSpriteRequest1.req1Object = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    BattleFx_InstallBillboardMatrix((short *)task->data, 0x2800 /* 2.5 */, 0);
    g_BattleSpecMissileSalvoSpriteRequest1.req1Word12 = task->frameCounter;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecMissileSalvoSpriteRequest1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0 && (g_BattleSpecMissileSalvoSpriteRequest1.spriteResource) != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= *(int *)((char *)*(int *)(g_BattleSpecMissileSalvoSpriteRequest1.spriteResource) + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws the projectile model and its billboard sprite and moves it along its step each frame.
 */
// FUNCTION: FF7 0x0051bed9
void __cdecl BattleFx_SpecialMissileSalvoProjectileTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[132];
        int transFlag;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotAngleX; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR baseX; };
        struct { unsigned char pad_to_14[0x14]; BattleFxMissileSalvoProjectileState *projectileState; };
        struct { unsigned char pad_to_18[0x18]; int depth; };
        struct { unsigned char pad_to_1C[0x1C]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_40[0x40]; short loopIndex; };
        struct { unsigned char pad_to_44[0x44]; int screenPos; };
        struct { unsigned char pad_to_48[0x48]; short stepX; };
        struct { unsigned char pad_to_4C[0x4C]; short stepZ; };
        struct { unsigned char pad_to_50[0x50]; BattleFxMissileSalvoProjectileSlot *projectileSlot; };
        struct { unsigned char pad_to_58[0x58]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_7C[0x7C]; SVECTOR pointX; };
    } frame;
#pragma pack(pop)

    frame.projectileSlot = (BattleFxMissileSalvoProjectileSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecMissileSalvoTransformMatrix.modelRecord = Gfx_ModelTableGetRenderObject(0, g_BattleSpecMissileSalvoLoadedResTable);
    g_BattleSpecMissileSalvoTransformMatrix.resource = Gfx_ModelTableGetLoadedRecord(4, g_BattleSpecMissileSalvoLoadedResTable);
    g_BattleSpecMissileSalvoTransformMatrix.res36 = Gfx_ModelTableGetSpriteObject(4, g_BattleSpecMissileSalvoLoadedResTable);
    g_BattleSpecMissileSalvoTransformMatrix.spriteRecord = Gfx_ModelTableGetLoadedRecord(5, g_BattleSpecMissileSalvoLoadedResTable);
    g_BattleSpecMissileSalvoTransformMatrix.secondDrawObj = Gfx_ModelTableGetSpriteObject(5, g_BattleSpecMissileSalvoLoadedResTable);
    frame.rotAngleX.vx = 0xFC00 /* 5670 deg */;
    frame.rotAngleX.vy = 0xFC00 /* 5670 deg */;
    frame.rotAngleX.vz = 0;
    RotMatrixXYZ(&frame.rotAngleX, &frame.rotMatrix);
    RotateMatrixY(frame.projectileSlot->angle, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, (int *)&g_BattleSpecMissileSalvoTransformMatrix);
    frame.rotMatrix.t[0] = frame.projectileSlot->posX;
    frame.rotMatrix.t[1] = frame.projectileSlot->posY;
    frame.rotMatrix.t[2] = frame.projectileSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecMissileSalvoDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecMissileSalvoTransformMatrix.modelRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecMissileSalvoDrawListHead);
    frame.rotAngleX.vy = frame.projectileSlot->angle;
    frame.rotAngleX.vz = 0;
    frame.rotAngleX.vx = frame.rotAngleX.vz;
    frame.baseX.vy = 0;
    frame.baseX.vx = frame.baseX.vy;
    frame.baseX.vz = 0xFF9C;
    RotMatrix(&frame.rotAngleX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.projectileSlot->posX;
    frame.rotMatrix.t[1] = frame.projectileSlot->posY;
    frame.rotMatrix.t[2] = frame.projectileSlot->posZ;
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    RotTransPointToInt16(&frame.baseX, (short *)&frame.baseX, &frame.transFlag);
    BattleFx_InstallBillboardMatrix(&frame.baseX, 0x2000 /* 2.0 */, 0);
    g_BattleSpecMissileSalvoTransformMatrix.firstFrame = frame.projectileSlot->frameCounter % *(int *)((char *)*(int *)g_BattleSpecMissileSalvoTransformMatrix.resource + 4);
    g_BattleSpecMissileSalvoDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecMissileSalvoTransformMatrix.resource, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecMissileSalvoDrawListHead);
    if (frame.projectileSlot->frameCounter >= 6) {
        frame.pointX.vx = frame.baseX.vx;
        frame.pointX.vy = frame.baseX.vy;
        frame.pointX.vz = frame.baseX.vz;
        frame.stepX = -SinFixed(frame.projectileSlot->angle) * ((frame.projectileSlot->frameCounter % 2) * 80 + 80) >> 12;
        frame.stepZ = -CosFixed(frame.projectileSlot->angle) * ((frame.projectileSlot->frameCounter % 2) * 80 + 80) >> 12;
        for (frame.loopIndex = 0; frame.loopIndex < 12; frame.loopIndex++) {
            BattleFx_InstallBillboardMatrix(&frame.pointX, 0x2000 /* 2.0 */, 0);
            g_BattleSpecMissileSalvoTransformMatrix.secondFrame = frame.loopIndex >> 1;
            g_BattleSpecMissileSalvoDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecMissileSalvoTransformMatrix.spriteRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecMissileSalvoDrawListHead);
            frame.pointX.vx += frame.stepX;
            frame.pointX.vz += frame.stepZ;
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    for (frame.loopIndex = 0; frame.loopIndex < 10; frame.loopIndex++) {
        frame.pointX.vx = frame.baseX.vx + rand() % 400 + -200;
        frame.pointX.vy = frame.baseX.vy + rand() % 400 + -200;
        frame.pointX.vz = frame.baseX.vz + rand() % 400 + -200;
        frame.projectileState = (BattleFxMissileSalvoProjectileState *)g_BattleSpecMissileSalvoDrawListHead;
        frame.projectileState->commandWord = frame.projectileState->commandWord & 0xFFFFFF | 0x2000000;
        frame.projectileState->primCode = 0x68;
        frame.projectileState->primCode = 0x6A;
        frame.depth = Gfx_TransformVertexSlot0ReturnDepth(&frame.pointX, &frame.screenPos, &frame.transFlag);
        if (frame.depth > 0) {
            frame.projectileState->colorR = 0x80;
            frame.projectileState->colorG = 0x40;
            frame.projectileState->colorB = 0x20;
            Gfx_DrawPsxPrimitive(frame.projectileState);
        }
    }
    if (frame.projectileSlot->frameCounter < 6) {
        frame.projectileSlot->posX += (short)(frame.projectileSlot->velocityX >> 2);
        frame.projectileSlot->posY += (short)((frame.projectileSlot->velocityY >> 2) + frame.projectileSlot->arcVelocity);
        frame.projectileSlot->posZ += (short)(frame.projectileSlot->velocityZ >> 2);
        frame.projectileSlot->arcVelocity -= 5;
    }
    if (frame.projectileSlot->frameCounter >= 6) {
        frame.projectileSlot->posX += frame.projectileSlot->velocityX;
        frame.projectileSlot->posY += (short)(frame.projectileSlot->velocityY - 10);
        frame.projectileSlot->posZ += frame.projectileSlot->velocityZ;
    }
    (frame.projectileSlot->frameCounter)++;
    if (frame.projectileSlot->frameCounter == 20) {
        frame.projectileSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
