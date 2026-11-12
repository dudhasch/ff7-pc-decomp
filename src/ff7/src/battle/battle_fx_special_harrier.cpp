// src/ff7/src/battle/battle_fx_special_harrier.cpp
/* Enemy Special Effect [idx 44]: Harrier (JP: harrier / ハリアー (Harrier), asset: "SPECIAL/HARRIER") — Harrier (0x246, Ying/Yang), Harrier Beam (0x2e0, Eagle Gun) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Harrier attack: loads its resources, spawns a rocket
//   flight per target, and drives the flight timeline, explosion and exhaust sprites, and drifting
//   debris models.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x008a6910
int g_BattleSpecialHarrierResourceDefs = 0x001900B1;
int g_BattleSpecialHarrierResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialHarrierResourceNames + 0x0)
};
int g_BattleSpecialHarrierResourceDefsDataTail[28] = {
    0x0, 0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleFxSpecialHarrierResourceNames + 0xc), 0x0,
    0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialHarrierResourceNames + 0x18), (int)((char *)&g_BattleFxSpecialHarrierResourceNames + 0x28), 0x0,
    0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialHarrierResourceNames + 0x34), (int)((char *)&g_BattleFxSpecialHarrierResourceNames + 0x44), 0x0, 0x0,
    0x192a39, 0x0, (int)((char *)&g_BattleFxSpecialHarrierResourceNames + 0x50), 0x0, 0x0, 0x192a39,
    (int)((char *)&g_BattleFxSpecialHarrierResourceNames + 0x5c), (int)((char *)&g_BattleFxSpecialHarrierResourceNames + 0x6c), 0x0, 0x0
};
// GLOBAL: FF7 0x008a6988
struct BattleSpecialWaveCannonModelReqMoto g_BattleSpecialHarrierSpriteReq1 = {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}};
// GLOBAL: FF7 0x008a6998
struct SVECTOR g_BattleSpecialHarrierRotAngle = {0, -1024, 0, 0};
// GLOBAL: FF7 0x008a69a0
int g_BattleSpecialHarrierScale1 = 6144;
unsigned char g_rawData_008a69a4[4] = {0x00, 0x18, 0x00, 0x00};
unsigned char g_rawData_008a69a8[8] = {0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a69b0
struct BattleLimitStardustRayModelReq2 g_BattleSpecialHarrierSpriteReq2 = {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}};
// GLOBAL: FF7 0x008a69c0
short g_BattleSpecialHarrierScaleByVariant[3] = {12288, 8192, 4096};
// GLOBAL: FF7 0x008a69c8
struct BattleSpecFreezeBreathScaleVector3 g_BattleSpecialHarrierScaleVec = {{0, 4, 0, 0, 0, 4, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0}, 0};
unsigned char g_rawData_008a69dc[4] = {0x88, 0x00, 0x00, 0x00};
unsigned char g_rawData_008a69e0[8] = {0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a69e8
int g_BattleSpecialHarrierRotXYZ = 64512;
unsigned char g_BattleFxSpecialHarrierResourceNames[120] =
    "HARIR1.RSD\0\0"
    "HARIR2.RSD\0\0"
    "HARRIE00.TIM\0\0\0\0"
    "BAKU1.SPT\0\0\0"
    "HARRIE01.TIM\0\0\0\0"
    "ELEC1.SPT\0\0\0"
    "JO_C01.SPT\0\0"
    "HARRIE02.TIM\0\0\0\0"
    "HASSYA.SPT";
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b6eff8
struct BattleSpecialHarrierLookMatrix g_BattleSpecialHarrierLookMatrix = {0};
// GLOBAL: FF7 0x00b6f018
short g_BattleSpecialHarrierRandomAngle = 0;
// GLOBAL: FF7 0x00b6f01c
int g_BattleSpecialHarrierDrawListHandle = 0;
// GLOBAL: FF7 0x00b6f020
struct GfxTableCursor *g_BattleSpecialHarrierLoadedRes = 0;
unsigned char g_bssPad_00b6f024[4] = {0};
// clang-format on

/*
 * Loads and registers resources for the Harrier special effect.
 */
// FUNCTION: FF7 0x0054e6d0
void __cdecl BattleFx_SpecialLoadResHarrier(void)
{
    GfxModelTableBank *newSceneField;
    GfxResourceRequest requestSlot;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecialHarrierLoadedRes == 0) {
        kind = 6;
        resourceDefsWord = (int)&g_BattleSpecialHarrierResourceDefs;
        newSceneField = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a6a68
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/HARRIER", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecialHarrierLoadedRes, newSceneField);
    }
}

/*
 * Entry point thunk for the Harrier special effect.
 */
// FUNCTION: FF7 0x0054e760
void __cdecl BattleFx_SpecialEnterHarrier(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginHarrier(targetMask, actorIdx);
}

/*
 * Initializes resources and starts target tasks for Harrier.
 */
// FUNCTION: FF7 0x0054e775
void __cdecl BattleFx_SpecialBeginHarrier(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResHarrier();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnHarrierPerTarget);
    Battle_PlaySfxPannedToActor(0x65, (short)actorIdx, 0x40);
}

/*
 * Spawns per-target Harrier effect tasks and computes trajectory basis.
 */
// FUNCTION: FF7 0x0054e7a7
void __cdecl BattleFx_SpecialSpawnHarrierPerTarget(int actor, int casterIndex)
{
    short direction[3];
    BattleFxBeamGunPath *timelinePtr;

    timelinePtr = (BattleFxBeamGunPath *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHarrierTimelineTask);
    Battle_GetBoneWorldPos(casterIndex, g_BattleActorRecords[casterIndex].primaryBone, &timelinePtr->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &timelinePtr->endX);
    timelinePtr->targetIndex = actor;
    direction[0] = (timelinePtr->endX - timelinePtr->startX) / 8;
    direction[1] = (timelinePtr->endY - timelinePtr->startY) / 8;
    direction[2] = (timelinePtr->endZ - timelinePtr->startZ) / 8;
    Gte_BuildLookBasisFromDirection(direction, (short *)&g_BattleSpecialHarrierLookMatrix);
}

/*
 * Main timeline task controlling Harrier rocket flight and impact events.
 */
// FUNCTION: FF7 0x0054e887
void __cdecl BattleFx_SpecialHarrierTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        struct { unsigned char pad_to_8[0x8]; int depth; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offsetX; };
        struct { unsigned char pad_to_34[0x34]; BattleFxNapalmTrailSlot *spawnSlot; };
        struct { unsigned char pad_to_3C[0x3C]; short driftIndex; };
        struct { unsigned char pad_to_40[0x40]; short velocityX; };
        struct { unsigned char pad_to_42[0x42]; short velocityY; };
        struct { unsigned char pad_to_44[0x44]; short velocityZ; };
        struct { unsigned char pad_to_48[0x48]; BattleFxOilTimelineSlot *timelineSlot; };
        struct { unsigned char pad_to_4C[0x4C]; SVECTOR driftX; };
    } frame;
#pragma pack(pop)

    frame.timelineSlot = (BattleFxOilTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timelineSlot->frameCounter == 0) {
        frame.offsetX.vx = 0;
        frame.offsetX.vy = 0;
        frame.offsetX.vz = 200;
        g_BattleSpecialHarrierLookMatrix.transX = frame.timelineSlot->startX;
        g_BattleSpecialHarrierLookMatrix.transY = frame.timelineSlot->startY;
        g_BattleSpecialHarrierLookMatrix.transZ = frame.timelineSlot->startZ;
        SetRotMatrix(&g_BattleSpecialHarrierLookMatrix);
        SetTransMatrix(&g_BattleSpecialHarrierLookMatrix);
        RotTransPointToInt16(&frame.offsetX, &frame.timelineSlot->startX, &frame.depth);
        frame.spawnSlot = (BattleFxNapalmTrailSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHarrierAnimatedSpriteTask);
        frame.spawnSlot->posX = frame.timelineSlot->startX;
        frame.spawnSlot->posY = frame.timelineSlot->startY;
        frame.spawnSlot->posZ = frame.timelineSlot->startZ;
    }
    if (frame.timelineSlot->frameCounter < 15) {
        g_BattleSpecialHarrierLookMatrix.transX = frame.timelineSlot->startX;
        g_BattleSpecialHarrierLookMatrix.transY = frame.timelineSlot->startY;
        g_BattleSpecialHarrierLookMatrix.transZ = frame.timelineSlot->startZ;
        SetRotMatrix(&g_BattleSpecialHarrierLookMatrix);
        SetTransMatrix(&g_BattleSpecialHarrierLookMatrix);
        frame.velocityX = (frame.timelineSlot->endX - frame.timelineSlot->startX) / 8;
        frame.velocityY = (frame.timelineSlot->endY - frame.timelineSlot->startY) / 8;
        frame.velocityZ = (frame.timelineSlot->endZ - frame.timelineSlot->startZ) / 8;
        for (frame.driftIndex = 0; frame.driftIndex < 3; frame.driftIndex++) {
            frame.spawnSlot = (BattleFxNapalmTrailSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHarrierDriftModelsTask);
            frame.driftX.vx = CosFixed((frame.timelineSlot->frameCounter << 9) + frame.driftIndex * 1365) >> 5;
            frame.driftX.vy = SinFixed((frame.timelineSlot->frameCounter << 9) + frame.driftIndex * 1365) >> 5;
            frame.driftX.vz = rand() % 100 - 50;
            RotTransPointToInt16(&frame.driftX, &frame.spawnSlot->posX, &frame.depth);
            frame.spawnSlot->velocityX = frame.velocityX;
            frame.spawnSlot->velocityY = frame.velocityY;
            frame.spawnSlot->velocityZ = frame.velocityZ;
        }
    }
    if (frame.timelineSlot->frameCounter >= 5 && frame.timelineSlot->frameCounter < 15) {
        frame.spawnSlot = (BattleFxNapalmTrailSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHarrierVariantSpriteTask);
        Battle_GetBoneWorldPos(frame.timelineSlot->actorIndex, g_BattleActorRecords[g_BattleSpecialActorId].targetBone, &frame.spawnSlot->posX);
        frame.spawnSlot->posX += (short)(rand() % 800 - 400);
        frame.spawnSlot->posY += (short)(rand() % 800 - 400);
        frame.spawnSlot->posZ += (short)(rand() % 800 - 400);
        frame.spawnSlot->variant = 1;
        frame.spawnSlot = (BattleFxNapalmTrailSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHarrierVariantSpriteTask);
        Battle_GetBoneWorldPos(frame.timelineSlot->actorIndex, g_BattleActorRecords[frame.timelineSlot->actorIndex].targetBone, &frame.spawnSlot->posX);
        frame.spawnSlot->posX += (short)(rand() % 800 - 400);
        frame.spawnSlot->posY += (short)(rand() % 800 - 400);
        frame.spawnSlot->posZ += (short)(rand() % 800 - 400);
        frame.spawnSlot->variant = 2;
    }
    if (frame.timelineSlot->frameCounter > 15 && frame.timelineSlot->frameCounter < 19) {
        for (frame.driftIndex = 0; frame.driftIndex < 2; frame.driftIndex++) {
            frame.spawnSlot = (BattleFxNapalmTrailSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHarrierVariantSpriteTask);
            Battle_GetBoneWorldPos(frame.timelineSlot->actorIndex, g_BattleActorRecords[frame.timelineSlot->actorIndex].targetBone, &frame.spawnSlot->posX);
            frame.spawnSlot->posX += (short)(rand() % 800 - 400);
            frame.spawnSlot->posY += (short)(rand() % 800 - 400);
            frame.spawnSlot->posZ += (short)(rand() % 800 - 400);
            g_BattleSpecialHarrierRandomAngle = (rand() % 4096);
            frame.spawnSlot->velocityX = CosFixed(g_BattleSpecialHarrierRandomAngle) * 0x32 >> 12;
            frame.spawnSlot->velocityZ = SinFixed(g_BattleSpecialHarrierRandomAngle) * 0x32 >> 12;
            frame.spawnSlot->velocityY = -rand() % 100;
            frame.spawnSlot->variant = 0;
        }
    }
    if (frame.timelineSlot->frameCounter == 15) {
        Battle_PostActorHitOrHurtMessage(frame.timelineSlot->actorIndex);
    }
    (frame.timelineSlot->frameCounter)++;
    if (frame.timelineSlot->frameCounter == 40) {
        frame.timelineSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animates and renders explosion and exhaust sprites for Harrier.
 */
// FUNCTION: FF7 0x0054ee3a
void __cdecl BattleFx_SpecialHarrierAnimatedSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[36];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxCosmoMemoryExpandingModel *spriteSlot; };
    } frame;
#pragma pack(pop)

    frame.spriteSlot = (BattleFxCosmoMemoryExpandingModel *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecialHarrierSpriteReq1.resource) = Gfx_ModelTableGetLoadedRecord(5, g_BattleSpecialHarrierLoadedRes);
    g_BattleSpecialHarrierSpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(5, g_BattleSpecialHarrierLoadedRes);
    RotMatrix(&g_BattleSpecialHarrierRotAngle, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialHarrierLookMatrix, &frame.rotMatrix, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialHarrierScale1);
    frame.rotMatrix.t[0] = frame.spriteSlot->posX;
    frame.rotMatrix.t[1] = frame.spriteSlot->posY;
    frame.rotMatrix.t[2] = frame.spriteSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    if ((g_BattleSpecialHarrierSpriteReq1.resource) != 0) {
        g_BattleSpecialHarrierSpriteReq1.frame = frame.spriteSlot->frame % *(int *)((char *)*(int *)(g_BattleSpecialHarrierSpriteReq1.resource) + 4);
    }
    g_BattleSpecialHarrierDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialHarrierSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialHarrierDrawListHandle);
    if (g_BattleIsPaused == 0) {
        (frame.spriteSlot->frame)++;
        if (frame.spriteSlot->frame == 20) {
            frame.spriteSlot->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Animates secondary variant sprites for Harrier.
 */
// FUNCTION: FF7 0x0054ef7d
void __cdecl BattleFx_SpecialHarrierVariantSpriteTask(void)
{
    struct {
        int table[3];
        BattleFxHarrierSprite *slot;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = (BattleFxHarrierSprite *)BATTLE_CURRENT_TASK_SLOT;
    frame.table[0] = 2;
    frame.table[1] = 3;
    frame.table[2] = 4;
    BattleFx_InstallBillboardMatrix(&frame.slot->posX, (short)g_BattleSpecialHarrierScaleByVariant[frame.slot->variant], 0);
    (g_BattleSpecialHarrierSpriteReq2.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(frame.table[frame.slot->variant], g_BattleSpecialHarrierLoadedRes);
    if (frame.slot->variant != 2) {
        g_BattleSpecialHarrierSpriteReq2.drawObj = Gfx_ModelTableGetSpriteObject(frame.table[frame.slot->variant], g_BattleSpecialHarrierLoadedRes);
    } else {
        g_BattleSpecialHarrierSpriteReq2.drawObj = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    }
    g_BattleSpecialHarrierSpriteReq2.frameIndex = frame.slot->frameIndex;
    g_BattleSpecialHarrierDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialHarrierSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialHarrierDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.slot->variant == 0) {
        frame.slot->posX += frame.slot->velocityX;
        frame.slot->posY += frame.slot->velocityY;
        frame.slot->posZ += frame.slot->velocityZ;
    }
    if ((g_BattleSpecialHarrierSpriteReq2.spriteGroup) != 0) {
        (frame.slot->frameIndex)++;
        if (frame.slot->frameIndex >= g_BattleSpecialHarrierSpriteReq2.spriteGroup->header->frameCount) {
            frame.slot->ownerSlot = 0xFFFF;
        }
    }
}

/*
 * Animates drifting debris models for Harrier.
 */
// FUNCTION: FF7 0x0054f0f6
void __cdecl BattleFx_SpecialHarrierDriftModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_30[0x30]; BattleFxHellBomberRocketSlot *driftSlot; };
    } frame;
#pragma pack(pop)

    frame.driftSlot = (BattleFxHellBomberRocketSlot *)BATTLE_CURRENT_TASK_SLOT;
    RotMatrixXYZ(&g_BattleSpecialHarrierRotXYZ, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialHarrierLookMatrix, &frame.rotMatrix, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, (int *)&g_BattleSpecialHarrierScaleVec);
    frame.rotMatrix.t[0] = frame.driftSlot->posX;
    frame.rotMatrix.t[1] = frame.driftSlot->posY;
    frame.rotMatrix.t[2] = frame.driftSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialHarrierScaleVec.modelHandle = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialHarrierLoadedRes);
    g_BattleSpecialHarrierDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialHarrierScaleVec.modelHandle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialHarrierDrawListHandle);
    g_BattleSpecialHarrierScaleVec.modelHandle = Gfx_ModelTableGetRenderObject(1, g_BattleSpecialHarrierLoadedRes);
    g_BattleSpecialHarrierDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialHarrierScaleVec.modelHandle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialHarrierDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.driftSlot->posX += frame.driftSlot->velocityX;
    frame.driftSlot->posY += frame.driftSlot->velocityY;
    frame.driftSlot->posZ += frame.driftSlot->velocityZ;
    (frame.driftSlot->frameCounter)++;
    if (frame.driftSlot->frameCounter == 8) {
        frame.driftSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
