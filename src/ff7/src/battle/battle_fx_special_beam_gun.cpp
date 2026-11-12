// src/ff7/src/battle/battle_fx_special_beam_gun.cpp
/* Enemy Special Effect [idx 22]: BeamGun (JP: mgbeam / マシンガンビーム (Machine Gun Beam), asset: "SPECIAL/MGBEAM") — Beam Gun (0x116, Grunt / Special Combatant) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Mgbeam attack: loads resources, registers a per-target
//   spawn callback, and drives beam spawning with energy flash sprites, one-shot blast sprites and
//   a drifting beam model.

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
// GLOBAL: FF7 0x008a85e0
int g_BattleSpecialBeamGunResourceDefs = 0x001900B1;
int g_BattleSpecialBeamGunResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialBeamGunResourceNames + 0x0)
};
int g_BattleSpecialBeamGunResourceDefsDataTail[14] = {
    0x0, 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialBeamGunResourceNames + 0xc), (int)((char *)&g_BattleFxSpecialBeamGunResourceNames + 0x18),
    0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialBeamGunResourceNames + 0x24), (int)((char *)&g_BattleFxSpecialBeamGunResourceNames + 0x30), 0x0,
    0x0, 0x0
};
// GLOBAL: FF7 0x008a8620
struct BattleSpecialBeamGunSpriteData1 g_BattleSpecialBeamGunSpriteData1 = {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}};
// GLOBAL: FF7 0x008a8630
int g_BattleSpecialBeamGunRotationVector1 = (int)0xFC000000;
#undef g_BattleSpecialBeamGunScaleData1
// GLOBAL: FF7 0x008a8638
struct BattleSpecialBeamGunScaleData1 g_BattleSpecialBeamGunScaleData1 = {{0, 0x20, 0, 0, 0, 0x20, 0, 0, 0, 0x20, 0, 0, 0, 0, 0, 0}, 0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}};
// GLOBAL: FF7 0x008a8658
struct BattleSpecialBeamGunScaleData2 g_BattleSpecialBeamGunScaleData2 = {{0, 5, 0, 0, 0, 5, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0}, 0};
unsigned char g_rawData_008a866c[4] = {0x88, 0x00, 0x00, 0x00};
unsigned char g_rawData_008a8670[8] = {0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a8678
int g_BattleSpecialBeamGunRotationVector2 = 64512;
unsigned char g_BattleFxSpecialBeamGunResourceNames[60] =
    "GAN1.RSD\0\0\0\0"
    "HIT1.TIM\0\0\0\0"
    "HIT1.SPT\0\0\0\0"
    "HASSYA.TIM\0\0"
    "HASSYA.SPT";
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// clang-format on


// clang-format off
// GLOBAL: FF7 0x00b72f10
struct BattleSpecialBeamGunBasisMatrix g_BattleSpecialBeamGunBasisMatrix = {0};
unsigned char g_bssPad_00b72f30[4] = {0};
// GLOBAL: FF7 0x00b72f34
int g_BattleSpecialBeamGunDrawListHandle = 0;
// GLOBAL: FF7 0x00b72f38
struct GfxTableCursor *g_BattleSpecialBeamGunLoadedRes = 0;
// clang-format on

/*
 * Loads resources for the Mgbeam special effect when not already loaded.
 */
// FUNCTION: FF7 0x0055c940
void __cdecl BattleFx_SpecialLoadResBeamGun(void)
{
    GfxModelTableBank *value;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecialBeamGunLoadedRes == 0) {
        kind = 3;
        resourceDefsWord = (int)&g_BattleSpecialBeamGunResourceDefs;
        value = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a86bc
        pRequest->resourcePath = (char *)BattleFx_BuildResourcePath("SPECIAL/MGBEAM", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecialBeamGunLoadedRes, value);
    }
}

/*
 * Main entry point for the Mgbeam special effect.
 */
// FUNCTION: FF7 0x0055c9d0
void __cdecl BattleFx_SpecialEnterBeamGun(int targetMask, int actorIdx)
{
    BattleFx_SpecialRegisterBeamGunPerTargetCallback(targetMask, actorIdx);
}

/*
 * Registers per-target spawn callback for Mgbeam.
 */
// FUNCTION: FF7 0x0055c9e5
void __cdecl BattleFx_SpecialRegisterBeamGunPerTargetCallback(int targetMask, int actorIndex)
{
    BattleFx_SpecialLoadResBeamGun();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIndex, 4, BattleFx_SpecialSpawnBeamGunPerTarget);
    Battle_PlaySfxPannedToActor(0x65, (short)actorIndex, 0);
}

/*
 * Spawns timeline task for a single Mgbeam target.
 */
// FUNCTION: FF7 0x0055ca17
void __cdecl BattleFx_SpecialSpawnBeamGunPerTarget(int actor, int actorIndex)
{
    BattleFxBeamGunPath *pBeamPath;
    short direction[3];

    pBeamPath = (BattleFxBeamGunPath *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBeamGunTimelineTask);
    Battle_GetBoneWorldPos(actorIndex, g_BattleActorRecords[actorIndex].primaryBone, &pBeamPath->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &pBeamPath->endX);
    pBeamPath->targetIndex = (short)actor;
    direction[0] = (pBeamPath->endX - pBeamPath->startX) / 8;
    direction[1] = (pBeamPath->endY - pBeamPath->startY) / 8;
    direction[2] = (pBeamPath->endZ - pBeamPath->startZ) / 8;
    Gte_BuildLookBasisFromDirection(direction, (short *)&g_BattleSpecialBeamGunBasisMatrix);
}

/*
 * Drives the main timeline and beam spawning for Mgbeam.
 */
// FUNCTION: FF7 0x0055caf7
void __cdecl BattleFx_SpecialBeamGunTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        struct { unsigned char pad_to_8[0x8]; int projDepth; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offsetX; };
        struct { unsigned char pad_to_34[0x34]; BattleFxBeamGunTimelineSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_3C[0x3C]; short driftIndex; };
        struct { unsigned char pad_to_40[0x40]; short stepX; };
        struct { unsigned char pad_to_42[0x42]; short stepY; };
        struct { unsigned char pad_to_44[0x44]; short stepZ; };
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
        frame.offsetX.vz = 500;
        g_BattleSpecialBeamGunBasisMatrix.vectorX = frame.timelineSlot->startX;
        g_BattleSpecialBeamGunBasisMatrix.vectorY = frame.timelineSlot->startY;
        g_BattleSpecialBeamGunBasisMatrix.vectorZ = frame.timelineSlot->startZ;
        SetRotMatrix(&g_BattleSpecialBeamGunBasisMatrix);
        SetTransMatrix(&g_BattleSpecialBeamGunBasisMatrix);
        RotTransPointToInt16(&frame.offsetX, &frame.timelineSlot->startX, &frame.projDepth);
        frame.spawnSlot = (BattleFxBeamGunTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBeamGunAnimatedSpriteTask);
        frame.spawnSlot->posX = frame.timelineSlot->startX;
        frame.spawnSlot->posY = frame.timelineSlot->startY;
        frame.spawnSlot->posZ = frame.timelineSlot->startZ;
    }
    if (frame.timelineSlot->frameCounter < 15) {
        g_BattleSpecialBeamGunBasisMatrix.vectorX = frame.timelineSlot->startX;
        g_BattleSpecialBeamGunBasisMatrix.vectorY = frame.timelineSlot->startY;
        g_BattleSpecialBeamGunBasisMatrix.vectorZ = frame.timelineSlot->startZ;
        SetRotMatrix(&g_BattleSpecialBeamGunBasisMatrix);
        SetTransMatrix(&g_BattleSpecialBeamGunBasisMatrix);
        frame.stepX = (frame.timelineSlot->endX - frame.timelineSlot->startX) / 8;
        frame.stepY = (frame.timelineSlot->endY - frame.timelineSlot->startY) / 8;
        frame.stepZ = (frame.timelineSlot->endZ - frame.timelineSlot->startZ) / 8;
        for (frame.driftIndex = 0; frame.driftIndex < 3; frame.driftIndex++) {
            frame.spawnSlot = (BattleFxBeamGunTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBeamGunDriftModelTask);
            frame.driftX.vx = CosFixed((frame.timelineSlot->frameCounter << 9) + frame.driftIndex * 1365) >> 5;
            frame.driftX.vy = SinFixed((frame.timelineSlot->frameCounter << 9) + frame.driftIndex * 1365) >> 5;
            frame.driftX.vz = rand() % 100 - 50;
            RotTransPointToInt16(&frame.driftX, &frame.spawnSlot->posX, &frame.projDepth);
            frame.spawnSlot->velocityX = frame.stepX;
            frame.spawnSlot->velocityY = frame.stepY;
            frame.spawnSlot->velocityZ = frame.stepZ;
        }
    }
    if (frame.timelineSlot->frameCounter > 8 && frame.timelineSlot->frameCounter < 28) {
        frame.spawnSlot = (BattleFxBeamGunTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBeamGunOneShotSpriteTask);
        Battle_GetBoneWorldPos(frame.timelineSlot->actorIndex, g_BattleActorRecords[g_BattleSpecialActorId].targetBone, &frame.spawnSlot->posX);
        frame.spawnSlot->posX += (short)(rand() % 800 - 400);
        frame.spawnSlot->posY += (short)(rand() % 800 - 400);
        frame.spawnSlot->posZ += (short)(rand() % 800 - 400);
        frame.spawnSlot->unusedWord = 0x3000;
        frame.spawnSlot->negRadius = -g_BattleActorRecords[g_BattleSpecialActorId].radius;
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
 * Updates and renders animated energy flash sprites for Mgbeam.
 */
// FUNCTION: FF7 0x0055ce9e
void __cdecl BattleFx_SpecialBeamGunAnimatedSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[36];
        PsxMatrix spriteMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxOilTargetModelSlot *spriteSlot; };
    } frame;
#pragma pack(pop)

    frame.spriteSlot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecialBeamGunSpriteData1.spriteResource) = Gfx_ModelTableGetLoadedRecord(2, g_BattleSpecialBeamGunLoadedRes);
    g_BattleSpecialBeamGunSpriteData1.tableField = Gfx_ModelTableGetSpriteObject(2, g_BattleSpecialBeamGunLoadedRes);
    RotMatrix((SVECTOR *)&g_BattleSpecialBeamGunRotationVector1, &frame.spriteMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialBeamGunBasisMatrix, &frame.spriteMatrix, &frame.spriteMatrix);
    ScaleMatrix(&frame.spriteMatrix, (int *)&g_BattleSpecialBeamGunScaleData1);
    frame.spriteMatrix.t[0] = frame.spriteSlot->posX;
    frame.spriteMatrix.t[1] = frame.spriteSlot->posY;
    frame.spriteMatrix.t[2] = frame.spriteSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.spriteMatrix, &frame.spriteMatrix);
    SetRotMatrix(&frame.spriteMatrix);
    SetTransMatrix(&frame.spriteMatrix);
    if ((g_BattleSpecialBeamGunSpriteData1.spriteResource) != 0) {
        g_BattleSpecialBeamGunSpriteData1.frameIndex = frame.spriteSlot->frameCounter % *(int *)((char *)*(int *)(g_BattleSpecialBeamGunSpriteData1.spriteResource) + 4);
    }
    g_BattleSpecialBeamGunDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialBeamGunSpriteData1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialBeamGunDrawListHandle);
    if (g_BattleIsPaused == 0) {
        if ((g_BattleSpecialBeamGunSpriteData1.spriteResource) != 0) {
            (frame.spriteSlot->frameCounter)++;
            if (frame.spriteSlot->frameCounter >= *(int *)((char *)*(int *)(g_BattleSpecialBeamGunSpriteData1.spriteResource) + 4)) {
                frame.spriteSlot->ownerSlot = BATTLE_TASK_FREE;
            }
        } else {
            frame.spriteSlot->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Updates and renders one-shot blast sprites for Mgbeam.
 */
// FUNCTION: FF7 0x0055cffc
void __cdecl BattleFx_SpecialBeamGunOneShotSpriteTask(void)
{
    BattleFxBeamGunOneShotSprite *task;

    task = (BattleFxBeamGunOneShotSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialBeamGunScaleData1.spriteRecord = Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecialBeamGunLoadedRes);
    g_BattleSpecialBeamGunScaleData1.spriteObject = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecialBeamGunLoadedRes);
    BattleFx_InstallBillboardMatrix(&task->posX, 0x1000 /* 1.0 */, task->pushLength);
    g_BattleSpecialBeamGunScaleData1.frameIndex = task->frameIndex;
    g_BattleSpecialBeamGunDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialBeamGunScaleData1.spriteRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialBeamGunDrawListHandle);
    if (g_BattleIsPaused == 0) {
        if (g_BattleSpecialBeamGunScaleData1.spriteRecord != 0) {
            (task->frameIndex)++;
            if (task->frameIndex >= *(int *)((char *)*(int *)g_BattleSpecialBeamGunScaleData1.spriteRecord + 4)) {
                task->ownerSlot = BATTLE_TASK_FREE;
            }
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Updates and renders drifting beam model along look-basis vector for Mgbeam.
 */
// FUNCTION: FF7 0x0055d0dd
void __cdecl BattleFx_SpecialBeamGunDriftModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix modelMatrix;
        struct { unsigned char pad_to_30[0x30]; BattleFxHellBomberRocketSlot *driftSlot; };
    } frame;
#pragma pack(pop)

    frame.driftSlot = (BattleFxHellBomberRocketSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialBeamGunScaleData2.scale2ModelField = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialBeamGunLoadedRes);
    RotMatrixXYZ(&g_BattleSpecialBeamGunRotationVector2, &frame.modelMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialBeamGunBasisMatrix, &frame.modelMatrix, &frame.modelMatrix);
    ScaleMatrix(&frame.modelMatrix, (int *)&g_BattleSpecialBeamGunScaleData2);
    frame.modelMatrix.t[0] = frame.driftSlot->posX;
    frame.modelMatrix.t[1] = frame.driftSlot->posY;
    frame.modelMatrix.t[2] = frame.driftSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialBeamGunDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialBeamGunScaleData2.scale2ModelField, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialBeamGunDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.driftSlot->posX += frame.driftSlot->velocityX;
    frame.driftSlot->posY += frame.driftSlot->velocityY;
    frame.driftSlot->posZ += frame.driftSlot->velocityZ;
    (frame.driftSlot->frameCounter)++;
    if (frame.driftSlot->frameCounter == 7) {
        frame.driftSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
