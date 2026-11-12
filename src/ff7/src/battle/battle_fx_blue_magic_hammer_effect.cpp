// src/ff7/src/battle/battle_fx_blue_magic_hammer_effect.cpp
// Enemy Skill [2]: Magic Hammer (Japanese: マジックハンマー, dev tag: "blue/hanmmer" / Hanmmer)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The Magic Hammer blue magic effect: a hammer model that swings down onto each target,
//   followed by drifting sprites.

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
// GLOBAL: FF7 0x008aaf88
int g_BattleBlueMagicHammerResourceDefs[6] = {3276980, 9088948, 0, 0, 0, 0};
// GLOBAL: FF7 0x008aafa0
BattleFxSpriteQuadRequest g_BattleBlueMagicHammerSpriteQuad = {-16, -16, 0, 0x20, 0x1f, 0x1f, 0x80, 0x80, 0x80, {0x2e}, 0x3c, 0x7dc7, 0};
unsigned char g_rawData_008aafb4[4] = {0x68, 0x61, 0x6e, 0x6d};
unsigned char g_rawData_008aafb8[8] = {0x65, 0x72, 0x2e, 0x72, 0x73, 0x64, 0x00, 0x00};
// GLOBAL: FF7 0x00ba517c
struct GfxTableCursor *g_BattleBlueMagicHammerLoadedResTable = 0;
// clang-format on
/* Loads Magic Hammer blue magic resources. */
// FUNCTION: FF7 0x005711e0
void __cdecl BattleFx_BlueLoadResMagicHammer(void)
{
    GfxModelTableBank *sceneDataOwner;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleBlueMagicHammerLoadedResTable == 0) {
        kind = 1;
        resourceDefsWord = (int)&g_BattleBlueMagicHammerResourceDefs;
        sceneDataOwner = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008aafc0
        pRequest->resourcePath = BattleFx_BuildResourcePath("blue/hanmmer", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleBlueMagicHammerLoadedResTable, sceneDataOwner);
    }
}

/* Entry point for Magic Hammer blue magic effect. */
// FUNCTION: FF7 0x00571270
void __cdecl BattleFx_BlueEnterMagicHammer(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResMagicHammer();
    BattleFx_BlueBeginMagicHammer(targetMask, actorIdx);
}

/* Initializes per-target tasks for Magic Hammer. */
// FUNCTION: FF7 0x0057128a
void __cdecl BattleFx_BlueBeginMagicHammer(int targetMask, int actorIdx)
{
    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 5, BattleFx_BlueSpawnMagicHammerPerTarget);
}

/* Spawns swinging hammer model and drifting sprite tasks per target. */
// FUNCTION: FF7 0x005712c3
void __cdecl BattleFx_BlueSpawnMagicHammerPerTarget(int actor)
{
    int scaledRange, swingRange;
    BattleFxHammerDriftTask *driftTask;
    BattleFxMagicHammerSwing *swingPtr;

    swingPtr = (BattleFxMagicHammerSwing *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueMagicHammerSwingModelTask);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].altBone, (short *)((int)swingPtr + 4));
    swingRange = g_BattleActorRecords[actor].radius + 0x120;
    if (swingRange > 0x76C) {
        swingRange = 0x76C;
    }
    swingPtr->swingAngle = 0;
    swingPtr->swingStep = 0xFF00;
    swingPtr->yaw = g_BattleActorRecords[actor].yaw;
    swingPtr->tiltAngle = 0;
    swingPtr->tiltStep = ((swingRange << 0xC) / 0x2EE) / 8;
    scaledRange = swingRange * 0x2EE;
    swingPtr->swingRadius = scaledRange / 0x28A;
    swingPtr->heightOffset = scaledRange / 0x384;
    swingPtr->targetIndex = actor;
    driftTask = (BattleFxHammerDriftTask *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueMagicHammerSpawnDriftingSpritesTask);
    if (swingRange > 0x4F2) {
        swingRange = 0x4F2;
    }
    *(Int64Pair *)((char *)driftTask + 4) = *(Int64Pair *)((char *)swingPtr + 4);
    driftTask->halfSwingRange = swingRange / 2;
    driftTask->quarterSwingRange = driftTask->halfSwingRange / 2;
    driftTask->spriteScale = (swingRange << 0xC) / 0x15E;
    driftTask->driftSpeed = swingRange * 0x32 / 0x258;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition((char *)swingPtr + 4), 0xA4);
}

/* Animates swinging hammer model down onto target. */
// FUNCTION: FF7 0x00571468
void __cdecl BattleFx_BlueMagicHammerSwingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[116];
        SVECTOR bodyRotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxMagicHammerSwingModelSlot *slot; };
        struct { unsigned char pad_to_10[0x10]; SVECTOR localX; };
        struct { unsigned char pad_to_18[0x18]; PsxMatrix swingMatrix; };
        struct { unsigned char pad_to_38[0x38]; PsxMatrix bodyMatrix; };
        struct { unsigned char pad_to_58[0x58]; SVECTOR armRotX; };
        struct { unsigned char pad_to_60[0x60]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_64[0x64]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxMagicHammerSwingModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.bodyRotX.vx = 0;
    frame.bodyRotX.vy = 0;
    frame.bodyRotX.vz = frame.slot->swingAngle;
    RotMatrixXYZ(&frame.bodyRotX, &frame.bodyMatrix);
    frame.localX.vx = frame.slot->swingRadius;
    frame.localX.vy = 0;
    frame.localX.vz = 0;
    frame.armRotX.vx = 0;
    frame.armRotX.vy = frame.slot->angle + 0x400;
    frame.armRotX.vz = 0;
    RotMatrixXYZ(&frame.armRotX, &frame.swingMatrix);
    ConcatMatrixPreservingTranslation(&frame.swingMatrix, &frame.bodyMatrix, &frame.bodyMatrix);
    ApplyMatrixToSVectorInt32(&frame.swingMatrix, &frame.localX, &frame.bodyMatrix.t[0]);
    frame.bodyMatrix.t[0] += frame.slot->posX;
    frame.bodyMatrix.t[1] += frame.slot->posY - frame.slot->heightOffset;
    frame.bodyMatrix.t[2] += frame.slot->posZ;
    frame.scaleX.vz = frame.slot->tiltAngle;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.bodyMatrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.bodyMatrix, &frame.bodyMatrix);
    SetRotMatrix(&frame.bodyMatrix);
    SetTransMatrix(&frame.bodyMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleBlueMagicHammerLoadedResTable);
    frame.modelDraw->flags = 0x80;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.slot->frameCounter >= 0x1A) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.slot->frameCounter == 0x10) {
        Battle_PostActorHitOrHurtMessage(frame.slot->actorIndex);
    }
    if (frame.slot->frameCounter >= 0xA && frame.slot->frameCounter < 0xE) {
        frame.slot->swingAngle += frame.slot->swingStep;
    }
    if (frame.slot->frameCounter < 8) {
        frame.slot->tiltAngle += frame.slot->tiltStep;
    }
    (frame.slot->frameCounter)++;
}

/* Spawns drifting particle sprites after hammer impact. */
// FUNCTION: FF7 0x005716b2
void __cdecl BattleFx_BlueMagicHammerSpawnDriftingSpritesTask(void)
{
    VECTOR nx;
    int i;
    int pace;
    int radius;
    BattleFxBlueMagicHammerSpawner *spawner;
    BattleFxMagicHammerDriftSprite *pDriftSprite;

    spawner = (BattleFxBlueMagicHammerSpawner *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (spawner->step >= 0xE && spawner->step <= 0x11) {
        for (i = 0; i < 9; i++) {
            pDriftSprite = (BattleFxMagicHammerDriftSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueMagicHammerDriftingAnimatedSpriteTask);
            nx.vx = rand() % 0x4B0 - 0x258;
            nx.vy = 0;
            nx.vz = rand() % 0x4B0 - 0x258;
            NormalizeVectorFixed(&nx, &nx);
            radius = spawner->center - spawner->halfRange + rand() % (spawner->halfRange << 1);
            pDriftSprite->posX = spawner->baseX + (radius * nx.vx >> 0xC);
            pDriftSprite->posY = spawner->baseY;
            pDriftSprite->posZ = spawner->baseZ + (radius * nx.vz >> 0xC);
            pDriftSprite->scale = spawner->spriteScale;
            pace = spawner->driftSpeed + rand() % 0x20;
            pDriftSprite->velocityX = pace * nx.vx >> 0xC;
            pDriftSprite->velocityZ = pace * nx.vz >> 0xC;
        }
    }
    if (spawner->step >= 0x1A) {
        spawner->ownerSlot = BATTLE_TASK_FREE;
    }
    (spawner->step)++;
}

/* Animates and submits drifting particle quad sprite. */
// FUNCTION: FF7 0x0057185c
void __cdecl BattleFx_BlueMagicHammerDriftingAnimatedSpriteTask(void)
{
    BattleFxMagicHammerDriftSlot *task;

    task = (BattleFxMagicHammerDriftSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleBlueMagicHammerSpriteQuad.quadState = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    g_BattleBlueMagicHammerSpriteQuad.cellX = task->timer << 5;
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), (short)task->scale, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteQuadDraw(&g_BattleBlueMagicHammerSpriteQuad, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->timer >= 7) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posX += task->velocityX;
    task->posY += task->velocityY;
    (task->timer)++;
}
} /* extern "C" */
