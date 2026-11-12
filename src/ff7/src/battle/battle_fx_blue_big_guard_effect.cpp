// src/ff7/src/battle/battle_fx_blue_big_guard_effect.cpp
// Enemy Skill [4]: Big Guard (Japanese: マイティガード Mighty Guard, dev tag: "blue/mytyg" / Mytyg)
// Contents: The Mytyg blue magic effect: loads its resources and spawns a per-target timeline of
//   growing, spinning and ascending model and sprite tasks.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_fx.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x008a8e88
int g_BattleBlueBigGuardResourceDef = 0x004B00B1;
int g_BattleBlueBigGuardResourceDefHeadPtr[1] = {
    (int)((char *)&g_BattleFxBlueBigGuardEffectAssetPrefix + 0x0)
};
unsigned int g_BattleBlueBigGuardResourceDefDataTail[8] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x006400b1,
    // STRING: FF7 0x008a8ed0
    (unsigned int)"light_1.rsd",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x008a8eb0
struct BattleBlueBigGuardSpriteDrawReq g_BattleBlueBigGuardSpriteDrawReq = {-0xf0010, 0, {0x40, 0x1f, 0x1f, 0x80, 0x80, 0x80, 0x2e, 0x3a, 0, 7, 0x7b}, 0};
unsigned char g_BattleFxBlueBigGuardEffectAssetPrefix[4] = {0x6d, 0x79, 0x74, 0x79};
unsigned char g_rawData_008a8ec8[8] = {0x67, 0x5f, 0x31, 0x2e, 0x72, 0x73, 0x64, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x00b7bef0
struct GfxTableCursor *g_BattleBlueBigGuardLoadedRes = 0;
unsigned char g_bssPad_00b7bef4[4] = {0};
// clang-format on

/*
 * Loads resources for the Mytyg blue magic effect when not already loaded.
 */
// FUNCTION: FF7 0x00568d30
void __cdecl BattleFx_BlueLoadResBigGuard(void)
{
    GfxModelTableBank *pTableBank;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int defBlock;

    if (g_pBattle3dScene != 0 && g_BattleBlueBigGuardLoadedRes == 0) {
        kind = 2;
        defBlock = (int)&g_BattleBlueBigGuardResourceDef;
        pTableBank = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a8edc
        pRequest->resourcePath = (char *)BattleFx_BuildResourcePath("blue/mytyg", pathText);
        Gfx_LoadModelTableIfAbsent(kind, defBlock, pRequest, &g_BattleBlueBigGuardLoadedRes, pTableBank);
    }
}

/*
 * Entry point for the Mytyg blue magic effect.
 */
// FUNCTION: FF7 0x00568dc0
void __cdecl BattleFx_BlueEnterBigGuard(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResBigGuard();
    BattleFx_BlueBeginBigGuard(targetMask, actorIdx);
}

/*
 * Initializes and begins the Mytyg blue magic effect.
 */
// FUNCTION: FF7 0x00568dda
void __cdecl BattleFx_BlueBeginBigGuard(int targetMask, int actorIdx)
{
    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 3, BattleFx_BlueSpawnBigGuardPerTarget);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x170);
}

/*
 * Spawns Mytyg effect tasks for each target actor.
 */
// FUNCTION: FF7 0x00568e2f
void __cdecl BattleFx_BlueSpawnBigGuardPerTarget(int actor)
{
    int value;
    BattleBigGuardTargetSlot *taskRecord;

    taskRecord = (BattleBigGuardTargetSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueBigGuardTimelineTask);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)taskRecord->position);
    value = g_BattleActorRecords[actor].radius + 0xE0;
    if (value > 1900) {
        value = 0x76C;
    }
    taskRecord->growRate = value;
    taskRecord->targetYaw = g_BattleActorRecords[actor].yaw;
    taskRecord->targetIndex = actor;
}

/*
 * Timeline task coordinating the stages of the Mytyg blue magic effect.
 */
// FUNCTION: FF7 0x00568ecd
void __cdecl BattleFx_BlueBigGuardTimelineTask(void)
{
    int spriteIndex, halfRange, doubleSpread, ratio, randVal;
    BattleFxBigGuardRecord *mainTaskSlot;
    BattleFxBigGuardParticle *newParticlePtr;

    mainTaskSlot = (BattleFxBigGuardRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (mainTaskSlot->frame >= 1 && mainTaskSlot->frame <= 44 && (mainTaskSlot->frame & 1) != 0) {
        halfRange = mainTaskSlot->growRate;
        doubleSpread = halfRange << 1;
        ratio = (halfRange << 12) / 350;
        for (spriteIndex = 0; spriteIndex < 3; spriteIndex++) {
            newParticlePtr = (BattleFxBigGuardParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueBigGuardAscendingQuadSpriteTask);
            *(Int64Pair *)&newParticlePtr->posX = *(Int64Pair *)mainTaskSlot->position;
            newParticlePtr->posX += (short)(rand() % doubleSpread - halfRange);
            newParticlePtr->posY = -(rand() % halfRange);
            newParticlePtr->posZ += (short)(rand() % doubleSpread - halfRange);
            randVal = rand();
            newParticlePtr->lifetime = randVal % 90 + 50;
            newParticlePtr->scale = randVal % 0x1000 + 0x800;
            newParticlePtr->scale = newParticlePtr->scale * ratio >> 12;
        }
    }
    if (mainTaskSlot->frame >= 2 && mainTaskSlot->frame <= 18 && mainTaskSlot->frame % 6 == 0) {
        newParticlePtr = (BattleFxBigGuardParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueBigGuardSpinGrowModelTask);
        *(Int64Pair *)&newParticlePtr->posX = *(Int64Pair *)mainTaskSlot->position;
        newParticlePtr->delay = 0;
        newParticlePtr->lifetime = rand() % 48 + 100;
        newParticlePtr->speed = newParticlePtr->lifetime * mainTaskSlot->frame;
        newParticlePtr->rotYStep = rand() % 48 + 20;
        newParticlePtr->rotZ = 0;
        newParticlePtr->rotZStep = -(rand() % 32 + 70);
        newParticlePtr->scale = 0;
        newParticlePtr->growStep = (mainTaskSlot->growRate << 12) / 750 / 3;
    }
    if (mainTaskSlot->frame == 26) {
        newParticlePtr = (BattleFxBigGuardParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueBigGuardGrowingFadeModelTask);
        *(Int64Pair *)&newParticlePtr->posX = *(Int64Pair *)mainTaskSlot->position;
        newParticlePtr->delay = mainTaskSlot->delay;
        newParticlePtr->speed = mainTaskSlot->growRate;
        newParticlePtr->scale = 0;
        newParticlePtr->growStep = (mainTaskSlot->growRate << 12) / 700 / 10;
    }
    if (mainTaskSlot->frame == 48) {
        Battle_PostActorHitOrHurtMessage(mainTaskSlot->recordType);
    }
    if (mainTaskSlot->frame >= 55) {
        mainTaskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    (mainTaskSlot->frame)++;
}

/*
 * Task animating a growing and fading 3D model for Mytyg.
 */
// FUNCTION: FF7 0x005691fe
void __cdecl BattleFx_BlueBigGuardGrowingFadeModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxBigGuardGrowingFadeModelSlot *fadeSlot; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offsetX; };
        struct { unsigned char pad_to_14[0x14]; int rotMatrix; };
        struct { unsigned char pad_to_26[0x26]; GfxDirectionVec3 posX; };
        struct { unsigned char pad_to_34[0x34]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_38[0x38]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.fadeSlot = (BattleFxBigGuardGrowingFadeModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.fadeSlot->rotationY;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.offsetX.vx = 0;
    frame.offsetX.vy = 0;
    frame.offsetX.vz = -frame.fadeSlot->offsetDistance;
    ApplyMatrixToSVectorInt32(&frame.rotMatrix, &frame.offsetX, &frame.posX);
    frame.posX.x += frame.fadeSlot->posX;
    frame.posX.y += frame.fadeSlot->posY;
    frame.posX.z += frame.fadeSlot->posZ;
    frame.scaleX.vz = frame.fadeSlot->scale;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleBlueBigGuardLoadedRes);
    frame.modelDraw->flags = 8;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 128;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    if (frame.fadeSlot->frameCounter >= 8) {
        frame.modelDraw->fadeLevel -= (short)(frame.fadeSlot->frameCounter - 8 << 4);
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.fadeSlot->frameCounter >= 16) {
        frame.fadeSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.fadeSlot->scale += frame.fadeSlot->scaleStep;
    (frame.fadeSlot->frameCounter)++;
}

/*
 * Task animating a spinning and growing 3D model for Mytyg.
 */
// FUNCTION: FF7 0x005693e3
void __cdecl BattleFx_BlueBigGuardSpinGrowModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        SVECTOR angle;
        struct { unsigned char pad_to_8[0x8]; BattleFxBigGuardSpinGrowModelSlot *spinSlot; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_30[0x30]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.spinSlot = (BattleFxBigGuardSpinGrowModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.angle.vx = frame.spinSlot->angle;
    frame.angle.vy = frame.spinSlot->growth;
    frame.angle.vz = frame.spinSlot->spinAngle;
    RotMatrixXYZ(&frame.angle, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.spinSlot->posX;
    frame.rotMatrix.t[1] = frame.spinSlot->posY;
    frame.rotMatrix.t[2] = frame.spinSlot->posZ;
    frame.scaleX.vz = frame.spinSlot->growAmount;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleBlueBigGuardLoadedRes);
    frame.modelDraw->flags = 0x88;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    if (frame.spinSlot->frameCounter >= 2) {
        frame.modelDraw->fadeLevel = frame.spinSlot->frameCounter - 2 << 9;
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.spinSlot->frameCounter >= 10) {
        frame.spinSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spinSlot->angle += frame.spinSlot->spinStep;
    frame.spinSlot->growth += frame.spinSlot->growthStep;
    frame.spinSlot->spinAngle += frame.spinSlot->spinAngleStep;
    frame.spinSlot->growAmount += frame.spinSlot->growAmountStep;
    (frame.spinSlot->frameCounter)++;
}

/*
 * Task animating an ascending quad sprite particle for Mytyg.
 */
// FUNCTION: FF7 0x005695cc
void __cdecl BattleFx_BlueBigGuardAscendingQuadSpriteTask(void)
{
    BattleFxBigGuardSprite *task;

    task = (BattleFxBigGuardSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleBlueBigGuardSpriteDrawReq.mytygSpriteAngle = task->step << 5;
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->scale, -(task->scale >> 4));
    g_BattleBlueBigGuardSpriteDrawReq.mytygSpriteSceneObject = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)&g_BattleBlueBigGuardSpriteDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->step >= 7) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->height -= task->ascentSpeed;
    (task->step)++;
}
} /* extern "C" */
