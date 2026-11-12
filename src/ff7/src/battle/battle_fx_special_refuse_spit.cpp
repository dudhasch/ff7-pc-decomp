// src/ff7/src/battle/battle_fx_special_refuse_spit.cpp
/* Enemy Special Effect [idx 3]: RefuseSpit (JP: tan / 痰 (Spit/Phlegm), asset: "special/tan") — Refuse (0x17d, Brain Pod) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Tan attack: loads resources, spawns per-target timelines
//   that spit projectiles, and drives spit impact sprites and drifting droplet sprites.

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

// clang-format off
// GLOBAL: FF7 0x008a8550
int g_BattleSpecialRefuseSpitResourceDefs = 0x00192A39;
int g_BattleFxSpecialRefuseSpitResourceNames[1] = {
    (int)((char *)&g_BattleFxSpecialRefuseSpitResourceNames3 + 0x4)
};
unsigned int g_BattleFxSpecialRefuseSpitResourceNames2[8] = {
    // STRING: FF7 0x008a85b0
    (unsigned int)"TAN2.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a85bc
    (unsigned int)"TAN00.TIM",
    // STRING: FF7 0x008a85c8
    (unsigned int)"HIT1.SPT",
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x00b72f04
int g_BattleSpecialRefuseSpitDrawListHandle = 0;
// GLOBAL: FF7 0x00b72f08
struct GfxTableCursor *g_BattleSpecialRefuseSpitLoadedRes = 0;
unsigned char g_bssPad_00b72f0c[4] = {0};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008a8578
struct BattleSpecialRefuseSpitSpriteData g_BattleSpecialRefuseSpitSpriteData = {{0, 0, 64, 64, 64, 0x2c, 0, 0}, 0, 0};
unsigned char g_rawData_008a8590[8] = {0x30, 0x30, 0x30, 0x2e, 0x00, 0x00, 0x00, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008a8598
short g_BattleSpecialRefuseSpitBillboardScales[2] = {4096, 6144};
unsigned char g_rawData_008a859c[4] = {0x00, 0x14, 0x00, 0x11};
unsigned char g_BattleFxSpecialRefuseSpitResourceNames3[16] = {0x80, 0x09, 0x00, 0x06, 0x54, 0x41, 0x4e, 0x30, 0x30, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x00};
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

/* The sprite requests in g_BattleSpecialRefuseSpitSpriteData, which the data layout keeps as 24 bytes. */
#define g_RefuseSpitSprites (*&g_BattleSpecialRefuseSpitSpriteData)


/*
 * Loads resources for the Tan special effect when not already loaded.
 */
// FUNCTION: FF7 0x0055c3b0
void __cdecl BattleFx_SpecialLoadResRefuseSpit(void)
{
    GfxModelTableBank *value;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecialRefuseSpitLoadedRes == 0) {
        kind = 2;
        resourceDefsWord = (int)&g_BattleSpecialRefuseSpitResourceDefs;
        value = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a85d4
        pRequest->resourcePath = BattleFx_BuildResourcePath("special/tan", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecialRefuseSpitLoadedRes, value);
    }
}

/*
 * Main entry point for the Tan special effect.
 */
// FUNCTION: FF7 0x0055c440
void __cdecl BattleFx_SpecialEnterRefuseSpit(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResRefuseSpit();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnRefuseSpitPerTarget);
    Battle_PlaySfxPannedToActor(0x66, (short)actorIdx, 0x40);
}

/*
 * Spawns timeline task for a single Tan target.
 */
// FUNCTION: FF7 0x0055c472
void __cdecl BattleFx_SpecialSpawnRefuseSpitPerTarget(int actor, int actorIndex)
{
    BattleFxRefuseSpitPath *path;
    short targetPosition[3];

    path = (BattleFxRefuseSpitPath *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialRefuseSpitTimelineTask);
    Battle_GetBoneWorldPos(actorIndex, g_BattleActorRecords[actorIndex].bone33, &path->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, targetPosition);
    path->velocityX = (targetPosition[0] - path->startX) / 12;
    path->velocityY = (targetPosition[1] - path->startY) / 12;
    path->velocityZ = (targetPosition[2] - path->startZ) / 12;
    path->targetIndex = actor;
}

/*
 * Drives the main timeline and projectile spitting for Tan.
 */
// FUNCTION: FF7 0x0055c53b
void __cdecl BattleFx_SpecialRefuseSpitTimelineTask(void)
{
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short spriteIndex;
    BattleFxRefuseSpitTimeline *taskPtr;
    BattleFxRefuseSpitTimeline *sprite;

    taskPtr = (BattleFxRefuseSpitTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->timer < 3) {
        for (spriteIndex = 0; spriteIndex < 2; spriteIndex++) {
            sprite = (BattleFxRefuseSpitTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialRefuseSpitDriftSpriteTask);
            sprite->posX = taskPtr->posX - taskPtr->sizeX / 2 * spriteIndex;
            sprite->posY = taskPtr->posY - taskPtr->sizeY / 2 * spriteIndex;
            sprite->posZ = taskPtr->posZ - taskPtr->sizeZ / 2 * spriteIndex;
            sprite->sizeX = taskPtr->sizeX;
            sprite->sizeY = taskPtr->sizeY;
            sprite->sizeZ = taskPtr->sizeZ;
            sprite->scaleIndex = taskPtr->timer * 2 + spriteIndex;
        }
    }
    if (taskPtr->timer == 12) {
        Battle_PostActorHitOrHurtMessage(taskPtr->recordType);
    }
    if (taskPtr->timer > 10 && taskPtr->timer < 20) {
        sprite = (BattleFxRefuseSpitTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialRefuseSpitAnimatedSpriteTask);
        Battle_GetBoneWorldPos(taskPtr->recordType, g_BattleActorRecords[g_BattleSpecialActorId].targetBone, &sprite->posX);
        sprite->posX += (short)(rand() % 800 - 400);
        sprite->posY += (short)(rand() % 800 - 400);
        sprite->posZ += (short)(rand() % 800 - 400);
    }
    (taskPtr->timer)++;
    if (taskPtr->timer == 40) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates and renders animated spit impact sprites for Tan.
 */
// FUNCTION: FF7 0x0055c777
void __cdecl BattleFx_SpecialRefuseSpitAnimatedSpriteTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_RefuseSpitSprites.impact.drawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecialRefuseSpitLoadedRes);
    g_RefuseSpitSprites.impact.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecialRefuseSpitLoadedRes);
    BattleFx_InstallBillboardMatrix((short *)task->data, 0x1C00 /* 1.75 */, 0);
    g_RefuseSpitSprites.impact.frame = task->frameCounter;
    g_BattleSpecialRefuseSpitDrawListHandle = BattleFx_SubmitSpriteDraw(&g_RefuseSpitSprites.impact, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialRefuseSpitDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (task->frameCounter)++;
    if (task->frameCounter == ((BattleRefuseSpitSpriteGroup *)g_RefuseSpitSprites.impact.spriteGroup)->header->frameCount) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates and renders drifting droplet sprites for Tan.
 */
// FUNCTION: FF7 0x0055c841
void __cdecl BattleFx_SpecialRefuseSpitDriftSpriteTask(void)
{
    BattleFxRefuseSpitDriftSprite *task;

    task = (BattleFxRefuseSpitDriftSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_RefuseSpitSprites.dropletDrawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecialRefuseSpitLoadedRes);
    g_RefuseSpitSprites.dropletGroup = Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecialRefuseSpitLoadedRes);
    BattleFx_InstallBillboardMatrix(&task->posX, (short)g_BattleSpecialRefuseSpitBillboardScales[task->scaleIndex], 0);
    g_BattleSpecialRefuseSpitDrawListHandle = BattleFx_SubmitSpriteDraw(&g_RefuseSpitSprites.dropletGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialRefuseSpitDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posX += task->velocityX;
    task->posY += task->velocityY;
    task->posZ += task->velocityZ;
    (task->frameIndex)++;
    if (task->frameIndex == 12) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
