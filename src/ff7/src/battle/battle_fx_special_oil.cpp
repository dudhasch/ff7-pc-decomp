// src/ff7/src/battle/battle_fx_special_oil.cpp
/* Enemy Special Effect [idx 15]: Oil (JP: oil / オイル (Oil), asset: "SPECIAL/OIL") — Oil (0x1a4, Motor Ball / Bahba Velamyu) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Oil attack: it loads resources and spawns a per-target
//   timeline that drives variant sprites, an aimed model and a target-oriented model.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_3d.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x008a5130
unsigned int g_BattleSpecialOilResourceDefs[60] = {0x001900b1, (unsigned int)"mizu.rsd", 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)"1.rsd", 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)"2.rsd", 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)"3.rsd", 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)"4.rsd", 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)"5.rsd", 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)"6.rsd", 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)"7.rsd", 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)"8.rsd", 0x00000000, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"oil02.tim", (unsigned int)"oil1.spt", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"oil03.tim", (unsigned int)"baku1.spt", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"oil03.tim", (unsigned int)"hit1.spt", 0x00000000, 0x00000000};
// clang-format off
// GLOBAL: FF7 0x008a5220
struct BattleSpecialOilColor g_BattleSpecialOilColor = {0, 0, -128, -128, -128, {0x2c}, 0};
// clang-format on
// GLOBAL: FF7 0x008a5230
struct BattleSpecialOilSpriteIndices g_BattleSpecialOilSpriteIndices = {{0xa, 0xb}, 0};
unsigned char g_rawData_008a523c[4] = {0x80, 0x00, 0x00, 0x00};
unsigned char g_rawData_008a5240[8] = {0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a5248
struct BattleSpecialOilScale g_BattleSpecialOilScale = {0x2000, 0, 0x2000, {0}};
// GLOBAL: FF7 0x008a5258
SVECTOR g_BattleSpecialOilRotation = {-1024, 0, 0, 0};
// GLOBAL: FF7 0x008a5260
short g_BattleSpecialOilCounter = 1;
// GLOBAL: FF7 0x008a5268
struct BattleSpecialOilModelReq g_BattleSpecialOilModelReq = {0, {8, 0, 0, 0, 0, 0}, 0};
unsigned char g_rawData_008a5274[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a5278
int g_BattleSpecialOilModelIndices[8] = {1, 2, 3, 4, 5, 6, 7, 8};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b6e0ac
int g_BattleSpecialOilDrawListHandle = 0;
// GLOBAL: FF7 0x00b6e0b0
struct GfxTableCursor *g_BattleSpecialOilLoadedRes = 0;
// GLOBAL: FF7 0x00b6e0b4
short g_BattleSpecialOilTargetMask = 0;
// clang-format on

/*
 * Loads resources for the Oil special effect when not already loaded.
 */
// FUNCTION: FF7 0x005493f0
void __cdecl BattleFx_SpecialLoadResOil(void)
{
    GfxModelTableBank *newSceneField;
    GfxResourceRequest requestSlot;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecialOilLoadedRes == 0) {
        kind = 0xC;
        resourceDefsWord = (int)&g_BattleSpecialOilResourceDefs;
        newSceneField = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a532c
        pRequest->resourcePath = (char *)BattleFx_BuildResourcePath("SPECIAL/OIL", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecialOilLoadedRes, newSceneField);
    }
}

/*
 * Entry point for the Oil special effect, dispatching to begin.
 */
// FUNCTION: FF7 0x00549480
void __cdecl BattleFx_SpecialEnterOil(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginOil(targetMask, actorIdx);
}

/*
 * Initializes and begins the Oil special effect for target actors.
 */
// FUNCTION: FF7 0x00549495
void __cdecl BattleFx_SpecialBeginOil(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResOil();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnOilPerTarget);
    Battle_PlaySfxPannedToActor(0xD5, (short)actorIdx, 0);
}

/*
 * Spawns Oil effect instances for each targeted actor.
 */
// FUNCTION: FF7 0x005494ca
void __cdecl BattleFx_SpecialSpawnOilPerTarget(int actor, int sourceIndex)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxOilTimelineSlot *slotPtr;

    slotPtr = (BattleFxOilTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialOilTimelineTask);
    Battle_GetBoneWorldPos(sourceIndex, g_BattleActorRecords[sourceIndex].targetBone, &slotPtr->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &slotPtr->endX);
    slotPtr->actorIndex = actor;
}

/*
 * Timeline task controlling state progression for the Oil special effect.
 */
// FUNCTION: FF7 0x00549542
void __cdecl BattleFx_SpecialOilTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        struct { unsigned char pad_to_2C[0x2C]; BattleFxOilTimelineSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_34[0x34]; short i; };
        struct { unsigned char pad_to_38[0x38]; short distance; };
        struct { unsigned char pad_to_3C[0x3C]; BattleFxOilTimelineSlot *timelineSlot; };
        struct { unsigned char pad_to_40[0x40]; short deltaX; };
        struct { unsigned char pad_to_42[0x42]; short deltaY; };
        struct { unsigned char pad_to_44[0x44]; short deltaZ; };
    } frame;
#pragma pack(pop)

    frame.timelineSlot = (BattleFxOilTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timelineSlot->frameCounter == 0) {
        frame.spawnSlot = (BattleFxOilTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialOilAimedModelTask);
        frame.spawnSlot->angle = ArcTangentFixed(frame.timelineSlot->endX - frame.timelineSlot->startX, frame.timelineSlot->endZ - frame.timelineSlot->startZ);
        frame.spawnSlot->posX = frame.timelineSlot->startX;
        frame.spawnSlot->posY = frame.timelineSlot->startY;
        frame.spawnSlot->posZ = frame.timelineSlot->startZ;
        frame.spawnSlot->deltaX = frame.timelineSlot->endX - frame.timelineSlot->startX;
        frame.deltaX = frame.spawnSlot->deltaX;
        frame.spawnSlot->deltaY = frame.timelineSlot->endY - frame.timelineSlot->startY;
        frame.deltaY = frame.spawnSlot->deltaY;
        frame.spawnSlot->deltaZ = frame.timelineSlot->endZ - frame.timelineSlot->startZ;
        frame.deltaZ = frame.spawnSlot->deltaZ;
        frame.distance = SqrtToInt(frame.deltaX * frame.deltaX + frame.deltaY * frame.deltaY + frame.deltaZ * frame.deltaZ);
        frame.spawnSlot->scaleRate = (frame.distance << 12) / 2800;
    }
    if (frame.timelineSlot->frameCounter == 15) {
        frame.spawnSlot = (BattleFxOilTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialOilTargetModelTask);
        frame.spawnSlot->posX = frame.timelineSlot->endX;
        frame.spawnSlot->posY = 0;
        frame.spawnSlot->posZ = frame.timelineSlot->endZ;
        Battle_PlaySfxPannedToActor(0xA, (short)frame.timelineSlot->actorIndex, 0);
    }
    if (frame.timelineSlot->frameCounter == 40) {
        Battle_PostActorHitOrHurtMessage(frame.timelineSlot->actorIndex);
    }
    if (frame.timelineSlot->frameCounter > 4 && frame.timelineSlot->frameCounter < 9) {
        for (frame.i = 0; frame.i < 2; frame.i++) {
            frame.spawnSlot = (BattleFxOilTimelineSpawnSlot *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialOilVariantSpriteTask)];
            Battle_GetBoneWorldPos(frame.timelineSlot->actorIndex, g_BattleActorRecords[frame.timelineSlot->actorIndex].targetBone, &frame.spawnSlot->posX);
            frame.spawnSlot->posX += (short)(rand() % 1200 - 600);
            frame.spawnSlot->posY += (short)(rand() % 1200 - 600);
            frame.spawnSlot->posZ += (short)(rand() % 1200 - 600);
            frame.spawnSlot->isActive = 0;
        }
        frame.spawnSlot = (BattleFxOilTimelineSpawnSlot *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialOilVariantSpriteTask)];
        Battle_GetBoneWorldPos(frame.timelineSlot->actorIndex, g_BattleActorRecords[frame.timelineSlot->actorIndex].targetBone, &frame.spawnSlot->posX);
        frame.spawnSlot->posX += (short)(rand() % 600 - 300);
        frame.spawnSlot->posY += (short)(rand() % 600 - 300);
        frame.spawnSlot->posZ += (short)(rand() % 600 - 300);
        frame.spawnSlot->isActive = 1;
    }
    (frame.timelineSlot->frameCounter)++;
    if (frame.timelineSlot->frameCounter == 50) {
        frame.timelineSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task to animate and render variant sprites for the Oil effect.
 */
// FUNCTION: FF7 0x00549906
void __cdecl BattleFx_SpecialOilVariantSpriteTask(void)
{
    BattleFxOilSpriteSlot *task;
    PsxMatrix *matrix;

    task = (BattleFxOilSpriteSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    matrix = (PsxMatrix *)BattleFx_InstallBillboardMatrix(&task->posX, (task->variant != 0 ? 0x800 : 0) + 0x1800, 0);
    g_BattleSpecialOilColor.variantShort12 = task->frameIndex;
    g_BattleSpecialOilColor.variantByte10 = (task->variant - 4 != 0 ? 0x20 : 0) + 0x40;
    g_BattleSpecialOilColor.variantByte9 = g_BattleSpecialOilColor.variantByte10;
    g_BattleSpecialOilColor.variantByte8 = g_BattleSpecialOilColor.variantByte9;
    (g_BattleSpecialOilColor.spriteResource) = Gfx_ModelTableGetLoadedRecord(g_BattleSpecialOilSpriteIndices.spriteIndices[task->variant], g_BattleSpecialOilLoadedRes);
    g_BattleSpecialOilColor.drawObj = Gfx_ModelTableGetSpriteObject(g_BattleSpecialOilSpriteIndices.spriteIndices[task->variant], g_BattleSpecialOilLoadedRes);
    if (matrix->t[2] > 0) {
        g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecialOilColor, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if ((g_BattleSpecialOilColor.spriteResource) != 0) {
        (task->frameIndex)++;
        if (task->frameIndex >= ((BattleFxSpriteGroup *)g_BattleSpecialOilColor.spriteResource)->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Task to animate and render aimed models for the Oil effect.
 */
// FUNCTION: FF7 0x00549a43
void __cdecl BattleFx_SpecialOilAimedModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[100];
        PsxMatrix localMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxOilAimedModelSlot *oilSlot; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix lookBasis; };
        struct { unsigned char pad_to_44[0x44]; PsxMatrix drawMatrix; };
    } frame;
#pragma pack(pop)

    frame.oilSlot = (BattleFxOilAimedModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialOilSpriteIndices.modelDrawTableField = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialOilLoadedRes);
    Gte_BuildLookBasisFromDirection(&frame.oilSlot->deltaX, (short *)&frame.lookBasis);
    RotMatrixXYZ(&g_BattleSpecialOilRotation, &frame.localMatrix);
    ConcatMatrixPreservingTranslation(&frame.lookBasis, &frame.localMatrix, &frame.localMatrix);
    g_BattleSpecialOilScale.component4 = (frame.oilSlot->scaleRate * g_BattleSpecialOilCounter) / 4;
    ScaleMatrix(&frame.localMatrix, &g_BattleSpecialOilScale);
    frame.localMatrix.t[0] = frame.oilSlot->posX;
    frame.localMatrix.t[1] = frame.oilSlot->posY;
    frame.localMatrix.t[2] = frame.oilSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.localMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialOilDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialOilSpriteIndices.modelDrawTableField, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialOilDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.oilSlot->frameCounter < 4) {
        g_BattleSpecialOilCounter++;
    } else {
        (g_BattleSpecialOilScale.scaleX) -= 0x800 /* 0.5 */;
        g_BattleSpecialOilScale.component8 -= 0x800 /* 0.5 */;
    }
    (frame.oilSlot->frameCounter)++;
    if (frame.oilSlot->frameCounter == 8) {
        frame.oilSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task to render target-oriented models for the Oil effect.
 */
// FUNCTION: FF7 0x00549bd0
void __cdecl BattleFx_SpecialOilTargetModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[116];
        SVECTOR rotX;
        struct { unsigned char pad_to_10[0x10]; PsxMatrix localMatrix; };
        struct { unsigned char pad_to_40[0x40]; BattleFxOilTargetModelSlot *targetSlot; };
        struct { unsigned char pad_to_44[0x44]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_64[0x64]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.targetSlot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0x400 /* 90 deg */;
    frame.rotX.vz = 0;
    frame.rotX.vy = frame.rotX.vz;
    RotMatrix(&frame.rotX, &frame.localMatrix);
    frame.scaleX.vz = 0x2800 /* 2.5 */;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.localMatrix, &frame.scaleX);
    frame.localMatrix.t[0] = frame.targetSlot->posX;
    frame.localMatrix.t[1] = frame.targetSlot->posY;
    frame.localMatrix.t[2] = frame.targetSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.localMatrix, &frame.drawMatrix);
    (g_BattleSpecialOilModelReq.modelResField) = Gfx_ModelTableGetRenderObject(g_BattleSpecialOilModelIndices[frame.targetSlot->frameCounter % 8], g_BattleSpecialOilLoadedRes);
    g_BattleSpecialOilModelReq.stride = g_BattleSpecialOilTargetMask * 12;
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    g_BattleSpecialOilDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialOilModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialOilDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.targetSlot->frameCounter < 8) {
        g_BattleSpecialOilTargetMask++;
    }
    if (frame.targetSlot->frameCounter > 12) {
        g_BattleSpecialOilTargetMask--;
    }
    (frame.targetSlot->frameCounter)++;
    if (frame.targetSlot->frameCounter == 20) {
        frame.targetSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
