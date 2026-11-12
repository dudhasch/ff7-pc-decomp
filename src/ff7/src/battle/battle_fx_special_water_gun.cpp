// src/ff7/src/battle/battle_fx_special_water_gun.cpp
/* Enemy Special Effect [idx 6]: WaterGun (JP: mizu / 水 (Water), asset: "SPECIAL/MIZU") — Water Gun (0x147, Sahagin), Stop Web (0x209, Stinger), Seed Shot (0x22d) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Mizu attack: loads its resources, spawns per-target water
//   effects, and drives the splash timeline, spray sprites and expanding water column models.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <stdlib.h>
#include <string.h>

#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "ff7/battle_tasks.h"
#include "ff7/camera_script.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "lib/codegen.h"
#include "lib/api/graphics.h"
#include "lib/types/graphics.h"
extern "C" {

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

struct BattleFxModelDraw;


// clang-format off
// GLOBAL: FF7 0x008a6a78
int g_BattleSpecialWaterGunResourceDefs = 0x001900B1;
int g_BattleSpecialWaterGunResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialWaterGunResourceNames + 0x0)
};
int g_BattleSpecialWaterGunResourceDefsDataTail[14] = {
    0x0, 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialWaterGunResourceNames + 0xc), (int)((char *)&g_BattleFxSpecialWaterGunResourceNames + 0x18),
    0x0, 0x0, 0x192a39, 0x0, (int)((char *)&g_BattleFxSpecialWaterGunResourceNames + 0x24), 0x0,
    0x0, 0x0
};
// GLOBAL: FF7 0x008a6ab8
struct BattleLimitHowlingMoonSpriteRequest g_BattleSpecialWaterGunSpriteReq = {0, 0, -128, -128, -128, {0x2c}, 0, {0}};
// GLOBAL: FF7 0x008a6ac8
unsigned char g_BattleSpecialWaterGunModelReq[8] = {0x00, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00};
unsigned char g_rawData_008a6ad0[8] = {0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a6ad8
int g_BattleSpecialWaterGunRotXYZ = 64512;
unsigned char g_BattleFxSpecialWaterGunResourceNames[48] =
    "MIZU.RSD\0\0\0\0"
    "MIZU01.TIM\0\0"
    "BAKU1.SPT\0\0\0"
    "JO_C04.SPT";
// clang-format on

// clang-format off
// GLOBAL: FF7 0x00b6f028
short g_BattleSpecialWaterGunScaleStep = 0;
// GLOBAL: FF7 0x00b6f02c
int g_BattleSpecialWaterGunDrawListHandle = 0;
// GLOBAL: FF7 0x00b6f030
int g_BattleSpecialWaterGunScale = 0;
// GLOBAL: FF7 0x00b6f034
int g_BattleSpecialWaterGunModelScaleZ = 0;
// GLOBAL: FF7 0x00b6f038
int g_BattleSpecialWaterGunAlphaFactor = 0;
unsigned char g_bssPad_00b6f03c[4] = {0};
// GLOBAL: FF7 0x00b6f040
struct GfxTableCursor *g_BattleSpecialWaterGunLoadedRes = 0;
unsigned char g_bssPad_00b6f044[4] = {0};
unsigned char g_bssPad_00b6f048[16] = {0};
// clang-format on

/*
 * Loads and registers resources for the Mizu special effect.
 */
// FUNCTION: FF7 0x0054f290
void __cdecl BattleFx_SpecialLoadResWaterGun(void)
{
    GfxModelTableBank *stageSceneHandle;
    GfxResourceRequest requestObj;
    int resourceCount;
    char pathWorkBuffer[200];
    GfxResourceRequest *reqPtrPtr;
    int resourceTable;

    if (g_pBattle3dScene != 0 && g_BattleSpecialWaterGunLoadedRes == 0) {
        resourceCount = 3;
        resourceTable = (int)&g_BattleSpecialWaterGunResourceDefs;
        stageSceneHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        reqPtrPtr = &requestObj;
        Gfx_InitResourceRequest(reqPtrPtr);
        reqPtrPtr->groupTag = 0;
        // STRING: FF7 0x008a6b10
        reqPtrPtr->resourcePath = BattleFx_BuildResourcePath("SPECIAL/MIZU", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(resourceCount, resourceTable, reqPtrPtr, &g_BattleSpecialWaterGunLoadedRes, stageSceneHandle);
    }
}

/*
 * Entry point thunk for the Mizu special effect.
 */
// FUNCTION: FF7 0x0054f320
void __cdecl BattleFx_SpecialEnterWaterGun(int targetMask, int actorIdx)
{
    g_BattleSpecialWaterGunScaleStep = 1;
    g_BattleSpecialWaterGunScale = 0x2000;
    g_BattleSpecialWaterGunModelScaleZ = 0;
    g_BattleSpecialWaterGunAlphaFactor = 0x2000;
    BattleFx_SpecialBeginWaterGun(targetMask, actorIdx);
}

/*
 * Initializes resources and starts target tasks for Mizu.
 */
// FUNCTION: FF7 0x0054f35c
void __cdecl BattleFx_SpecialBeginWaterGun(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResWaterGun();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnWaterGunPerTarget);
    Battle_PlaySfxPannedToActor(0xEB, (short)actorIdx, 0);
}

/*
 * Spawns per-target Mizu effect tasks and initializes transform basis.
 */
// FUNCTION: FF7 0x0054f391
void __cdecl BattleFx_SpecialSpawnWaterGunPerTarget(int actor, int casterIndex)
{
    int p;
    BattleFxWaterGunTimelineSlot *effect;

    effect = (BattleFxWaterGunTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWaterGunTimelineTask);
    Battle_GetBoneWorldPos(casterIndex, g_BattleActorRecords[casterIndex].bone33, (short *)&effect->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)&effect->deltaX);
    effect->targetSlot = actor;
}

/*
 * Main timeline task controlling water splash animation and spawn events.
 */
// FUNCTION: FF7 0x0054f409
void __cdecl BattleFx_SpecialWaterGunTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        struct { unsigned char pad_to_2C[0x2C]; BattleFxOilTimelineSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_34[0x34]; short spriteIndex; };
        struct { unsigned char pad_to_38[0x38]; short distance; };
        struct { unsigned char pad_to_3C[0x3C]; BattleFxWaterGunTimelineSlot *timelineSlot; };
        struct { unsigned char pad_to_40[0x40]; short deltaX; };
        struct { unsigned char pad_to_42[0x42]; short deltaY; };
        struct { unsigned char pad_to_44[0x44]; short deltaZ; };
    } frame;
#pragma pack(pop)

    frame.timelineSlot = (BattleFxWaterGunTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timelineSlot->frameCounter == 0) {
        frame.spawnSlot = (BattleFxOilTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWaterGunStretchModelTask);
        frame.spawnSlot->angle = ArcTangentFixed(frame.timelineSlot->deltaX - frame.timelineSlot->startX, frame.timelineSlot->deltaZ - frame.timelineSlot->startZ);
        frame.spawnSlot->posX = frame.timelineSlot->startX;
        frame.spawnSlot->posY = frame.timelineSlot->startY;
        frame.spawnSlot->posZ = frame.timelineSlot->startZ;
        frame.spawnSlot->deltaX = frame.timelineSlot->deltaX - frame.timelineSlot->startX;
        frame.deltaX = frame.spawnSlot->deltaX;
        frame.spawnSlot->deltaY = frame.timelineSlot->deltaY - frame.timelineSlot->startY;
        frame.deltaY = frame.spawnSlot->deltaY;
        frame.spawnSlot->deltaZ = frame.timelineSlot->deltaZ - frame.timelineSlot->startZ;
        frame.deltaZ = frame.spawnSlot->deltaZ;
        frame.distance = SqrtToInt(frame.deltaX * frame.deltaX + frame.deltaY * frame.deltaY + frame.deltaZ * frame.deltaZ);
        frame.spawnSlot->scaleRate = (frame.distance << 0xC) / 0xAF0;
    }
    if (frame.timelineSlot->frameCounter == 0xA) {
        Battle_PostActorHitOrHurtMessage(frame.timelineSlot->targetSlot);
    }
    if (frame.timelineSlot->frameCounter > 5 && frame.timelineSlot->frameCounter < 0xA) {
        for (frame.spriteIndex = 0; frame.spriteIndex < 2; frame.spriteIndex++) {
            frame.spawnSlot = (BattleFxOilTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWaterGunVariantSpriteTask);
            Battle_GetBoneWorldPos(frame.timelineSlot->targetSlot, g_BattleActorRecords[frame.timelineSlot->targetSlot].targetBone, (short *)&frame.spawnSlot->posX);
            frame.spawnSlot->posX += (short)(rand() % 0x4B0 - 0x258);
            frame.spawnSlot->posY += (short)(rand() % 0x4B0 - 0x258);
            frame.spawnSlot->posZ += (short)(rand() % 0x4B0 - 0x258);
            frame.spawnSlot->isActive = 0;
        }
        frame.spawnSlot = (BattleFxOilTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWaterGunVariantSpriteTask);
        Battle_GetBoneWorldPos(frame.timelineSlot->targetSlot, g_BattleActorRecords[frame.timelineSlot->targetSlot].targetBone, (short *)&frame.spawnSlot->posX);
        frame.spawnSlot->posX += (short)(rand() % 0x258 - 0x12C);
        frame.spawnSlot->posY += (short)(rand() % 0x258 - 0x12C);
        frame.spawnSlot->posZ += (short)(rand() % 0x258 - 0x12C);
        frame.spawnSlot->isActive = 1;
    }
    (frame.timelineSlot->frameCounter)++;
    if (frame.timelineSlot->frameCounter == 0xF) {
        frame.timelineSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animates and renders water spray variant sprites for Mizu.
 */
// FUNCTION: FF7 0x0054f76c
void __cdecl BattleFx_SpecialWaterGunVariantSpriteTask(void)
{
    struct {
        int table[2];
        BattleFxWaterGunSpriteTask *slot;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = (BattleFxWaterGunSpriteTask *)BATTLE_CURRENT_TASK_SLOT;
    frame.table[0] = 1;
    frame.table[1] = 2;
    BattleFx_InstallBillboardMatrix(&frame.slot->posX, (frame.slot->variant != 0 ? 0x800 : 0) + 0x1800, 0);
    g_BattleSpecialWaterGunSpriteReq.frame = frame.slot->frameIndex;
    g_BattleSpecialWaterGunSpriteReq.colorB = 0x60;
    g_BattleSpecialWaterGunSpriteReq.colorG = g_BattleSpecialWaterGunSpriteReq.colorB;
    g_BattleSpecialWaterGunSpriteReq.colorR = g_BattleSpecialWaterGunSpriteReq.colorG;
    (g_BattleSpecialWaterGunSpriteReq.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(frame.table[frame.slot->variant], g_BattleSpecialWaterGunLoadedRes);
    if (frame.slot->variant != 1) {
        g_BattleSpecialWaterGunSpriteReq.drawObj = Gfx_ModelTableGetSpriteObject(frame.table[frame.slot->variant], g_BattleSpecialWaterGunLoadedRes);
    } else {
        g_BattleSpecialWaterGunSpriteReq.drawObj = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    }
    g_BattleSpecialWaterGunDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialWaterGunSpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialWaterGunDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if ((g_BattleSpecialWaterGunSpriteReq.spriteGroup) != 0) {
        (frame.slot->frameIndex)++;
        if (frame.slot->frameIndex >= g_BattleSpecialWaterGunSpriteReq.spriteGroup->header->frameCount) {
            frame.slot->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Animates and renders expanding water column models for Mizu.
 */
// FUNCTION: FF7 0x0054f8b5
void __cdecl BattleFx_SpecialWaterGunStretchModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[100];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxWaterGunStretchModelSlot *stretchSlot; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix lookBasis; };
        struct { unsigned char pad_to_44[0x44]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.stretchSlot = (BattleFxWaterGunStretchModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    (*(int *)&g_BattleSpecialWaterGunModelReq) = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialWaterGunLoadedRes);
    Gte_BuildLookBasisFromDirection(&frame.stretchSlot->dirX, frame.lookBasis.m[0]);
    RotMatrixXYZ(&g_BattleSpecialWaterGunRotXYZ, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&frame.lookBasis, &frame.rotMatrix, &frame.rotMatrix);
    g_BattleSpecialWaterGunModelScaleZ = (frame.stretchSlot->scaleZ * g_BattleSpecialWaterGunScaleStep) / 4;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialWaterGunScale);
    frame.rotMatrix.t[0] = frame.stretchSlot->posX;
    frame.rotMatrix.t[1] = frame.stretchSlot->posY;
    frame.rotMatrix.t[2] = frame.stretchSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialWaterGunDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialWaterGunModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialWaterGunDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.stretchSlot->frameCounter < 4) {
        g_BattleSpecialWaterGunScaleStep++;
    } else {
        g_BattleSpecialWaterGunScale -= 0x800;
        g_BattleSpecialWaterGunAlphaFactor -= 0x800;
    }
    (frame.stretchSlot->frameCounter)++;
    if (frame.stretchSlot->frameCounter == 8) {
        frame.stretchSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
