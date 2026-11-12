// src/ff7/src/battle/battle_fx_magic_demi.cpp
/* Magic Effect [42]: Demi (Japanese: グラビデ Gravity, dev tag: "MAGIC/GRAV1" / Grav1) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle effect for the Demi (Grav1) magic: loads its resource package, spawns per-target
//   tasks, and animates the travelling projectile with its trail and impact sprites.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_magic.h"
#include "lib/types/graphics.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/codegen.h"
extern "C" {

#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x008fd7d0
int g_BattleMagicDemi_ResourceDefs = 0x00192A38;
int g_BattleMagicDemi_ResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxMagicDemiResourceNames + 0x0)
};
// GLOBAL: FF7 0x008fd7d8
int g_BattleMagicDemi_ResourceTable[26] = {
    (int)((char *)&g_BattleFxMagicDemiResourceNames + 0x10), 0x0, 0x0, 0x192a38, (int)((char *)&g_BattleFxMagicDemiResourceNames + 0x1c), (int)((char *)&g_BattleFxMagicDemiResourceNames + 0x2c),
    0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxMagicDemiResourceNames + 0x38), (int)((char *)&g_BattleFxMagicDemiResourceNames + 0x48), 0x0,
    0x0, 0x192a39, (int)((char *)&g_BattleFxMagicDemiResourceNames + 0x54), (int)((char *)&g_BattleFxMagicDemiResourceNames + 0x64), 0x0, 0x0,
    0x192a39, (int)((char *)&g_BattleFxMagicDemiResourceNames + 0x70), (int)((char *)&g_BattleFxMagicDemiResourceNames + 0x80), 0x0, 0x0, 0x0,
    0x0, 0x0
};
// GLOBAL: FF7 0x008fd840
BattleMagicDemiSpriteColors g_BattleMagicDemi_SpriteColors = {{0x80, 0x80, 0x80, 0x2e}, 0, 0};
// GLOBAL: FF7 0x008fd848
BattleMagicDemi_SpriteReq g_BattleMagicDemi_SpriteReq = {
    0, 0, {0x80, 0x80, 0x80, 0x2c, 0x01, 0x00, 0x00, 0x00}, 0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0, 0}
};
unsigned char g_BattleFxMagicDemiResourceNames[140] =
    "GRAVID03.TIM\0\0\0\0"
    "GRAV_1.SPT\0\0"
    "GRAVID03.TIM\0\0\0\0"
    "GRAV_A.SPT\0\0"
    "GRAVID00.TIM\0\0\0\0"
    "ELEC_3.SPT\0\0"
    "GRAVID01.TIM\0\0\0\0"
    "ELEC_3.SPT\0\0"
    "GRAVID02.TIM\0\0\0\0"
    "ELEC_3.SPT";
// GLOBAL: FF7 0x00be0d78
int g_BattleMagicDemi_ChainTail = 0;
// GLOBAL: FF7 0x00be0d7c
struct GfxTableCursor *g_BattleMagicDemi_ResourceHandle = 0;
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

/* Indices into g_BattleMagicDemi_ResourceTable of the trail sprite record's table field and word. */
enum { GRAV1_TRAIL_TABLE_FIELD = 24, GRAV1_TRAIL_TABLE_WORD = 25 };


/* Loads the Demi (Grav1) magic resource package if not already loaded. */
// FUNCTION: FF7 0x005b5720
void __cdecl BattleFx_MagicLoadResDemi(void)
{
    GfxModelTableBank *baseOffset;
    GfxResourceRequest requestRecord;
    int kind;
    char pathEnd[200];
    GfxResourceRequest *request;
    int resultDefCatalog;

    if (g_pBattle3dScene != 0 && g_BattleMagicDemi_ResourceHandle == 0) {
        kind = 5;
        resultDefCatalog = (int)&g_BattleMagicDemi_ResourceDefs;
        baseOffset = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestRecord;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x008fd8f4
        request->resourcePath = (char *)BattleFx_BuildResourcePath("MAGIC/GRAV1", pathEnd);
        Gfx_LoadModelTableIfAbsent(kind, resultDefCatalog, request, &g_BattleMagicDemi_ResourceHandle, baseOffset);
    }
}

/* Entry point for the Demi (Grav1) magic effect. */
// FUNCTION: FF7 0x005b57b0
void __cdecl BattleFx_MagicEnterDemi(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResDemi();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 8, BattleFx_MagicSpawnDemiPerTarget);
}

/* Spawns per-target callback task for Demi (Grav1). */
// FUNCTION: FF7 0x005b57d1
void __cdecl BattleFx_MagicSpawnDemiPerTarget(int actor, int sourceActor)
{
    BattleFxDemiTravelSlot *travelSlot;

    travelSlot = (BattleFxDemiTravelSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDemiTravelEmitterTask);
    Battle_GetBoneWorldPos(sourceActor, g_BattleActorRecords[sourceActor].targetBone, &travelSlot->targetX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &travelSlot->velocityX);
    travelSlot->velocityX = (travelSlot->velocityX - travelSlot->targetX) / 24;
    travelSlot->velocityY = (travelSlot->velocityY - travelSlot->targetY) / 24;
    travelSlot->velocityZ = (travelSlot->velocityZ - travelSlot->targetZ) / 24;
    travelSlot->targetActor = actor;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[sourceActor].posX), 0x50);
}

/* Manages projectile travel and spawns trail sprites for Demi (Grav1). */
// FUNCTION: FF7 0x005b58c9
void __cdecl BattleFx_MagicDemiTravelEmitterTask(void)
{
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxMagicDemiEmitter *task;
    BattleFxDemiEmitterSlot *pTrail;

    task = (BattleFxMagicDemiEmitter *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    pTrail = (BattleFxDemiEmitterSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDemiTrailSpriteTask);
    *(Int64Pair *)&pTrail->posX = *(Int64Pair *)&task->posX;
    pTrail->mode = 2;
    task->posX += task->velocityX;
    task->posY += task->velocityY;
    task->posZ += task->velocityZ;
    pTrail = (BattleFxDemiEmitterSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDemiTrailSpriteTask);
    *(Int64Pair *)&pTrail->posX = *(Int64Pair *)&task->posX;
    pTrail->mode = 1;
    task->posX += task->velocityX;
    task->posY += task->velocityY;
    task->posZ += task->velocityZ;
    pTrail = (BattleFxDemiEmitterSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDemiTrailSpriteTask);
    *(Int64Pair *)&pTrail->posX = *(Int64Pair *)&task->posX;
    task->posX += task->velocityX;
    task->posY += task->velocityY;
    task->posZ += task->velocityZ;
    (task->timer)++;
    if (task->timer >= 8) {
        pTrail = (BattleFxDemiEmitterSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDemiAnimatedSpriteTask);
        *(Int64Pair *)&pTrail->posX = *(Int64Pair *)&task->posX;
        pTrail = (BattleFxDemiEmitterSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDemiExpandingSpriteTask);
        *(Int64Pair *)&pTrail->posX = *(Int64Pair *)&task->posX;
        pTrail->size = 512;
        pTrail->sizeStep = 720;
        Battle_PostActorHitOrHurtMessage(task->actorIndex);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[task->actorIndex].posX), 0x1E);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Updates and renders a trail sprite for Demi (Grav1). */
// FUNCTION: FF7 0x005b5b1e
void __cdecl BattleFx_MagicDemiTrailSpriteTask(void)
{
    BattleFxDemiEmitterSlot *task;
    int matrix;

    task = (BattleFxDemiEmitterSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicDemi_ResourceTable[GRAV1_TRAIL_TABLE_FIELD] = Gfx_ModelTableGetLoadedRecord(0, g_BattleMagicDemi_ResourceHandle);
    g_BattleMagicDemi_ResourceTable[GRAV1_TRAIL_TABLE_WORD] = Gfx_ModelTableGetSpriteObject(0, g_BattleMagicDemi_ResourceHandle);
    g_BattleMagicDemi_SpriteColors.frame = task->mode;
    matrix = (int)BattleFx_InstallBillboardMatrix(&task->posX, 0x800 /* 0.5 */, 0);
    if (*(int *)((char *)matrix + 0x1A) > 0) {
        g_BattleMagicDemi_ChainTail = BattleFx_SubmitSpriteDraw(&g_BattleMagicDemi_ResourceTable[GRAV1_TRAIL_TABLE_FIELD], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicDemi_ChainTail);
    }
    if (g_BattleIsPaused == 0) {
        task->mode += 3;
        if (task->mode >= 5) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/* Updates and renders an expanding impact sprite for Demi (Grav1). */
// FUNCTION: FF7 0x005b5bec
void __cdecl BattleFx_MagicDemiExpandingSpriteTask(void)
{
    BattleFxDemiExpandTask *task;
    int matrix;

    task = (BattleFxDemiExpandTask *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicDemi_SpriteReq.expandFrameTable = Gfx_ModelTableGetLoadedRecord(1, g_BattleMagicDemi_ResourceHandle);
    g_BattleMagicDemi_SpriteReq.expandVariantField = Gfx_ModelTableGetSpriteObject(1, g_BattleMagicDemi_ResourceHandle);
    matrix = (int)BattleFx_InstallBillboardMatrix((void *)((int)task + 4), (short)task->size, -(task->size >> 4));
    if (*(int *)((char *)matrix + 0x1A) > 0) {
        g_BattleMagicDemi_ChainTail = BattleFx_SubmitSpriteDraw(&g_BattleMagicDemi_SpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicDemi_ChainTail);
    }
    if (g_BattleIsPaused == 0) {
        task->size += task->sizeStep;
        task->sizeStep -= 40;
        (task->frame)++;
        if (task->frame >= 36) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/* Updates and renders an animated impact sprite for Demi (Grav1). */
// FUNCTION: FF7 0x005b5ce1
void __cdecl BattleFx_MagicDemiAnimatedSpriteTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicDemi_SpriteReq.animFramePhase = task->frameCounter % 12;
    g_BattleMagicDemi_SpriteReq.animFrameTable = Gfx_ModelTableGetLoadedRecord(2, g_BattleMagicDemi_ResourceHandle);
    g_BattleMagicDemi_SpriteReq.animVariantField = Gfx_ModelTableGetSpriteObject(2, g_BattleMagicDemi_ResourceHandle);
    if (g_BattleMagicDemi_SpriteReq.animFrameTable != 0) {
        ((BattleMagicDemiSpriteReq *)g_BattleMagicDemi_SpriteReq.animFrameTable)->spriteObject2 = Gfx_ModelTableGetSpriteObject(2, g_BattleMagicDemi_ResourceHandle);
        ((BattleMagicDemiSpriteReq *)g_BattleMagicDemi_SpriteReq.animFrameTable)->spriteObject3 = Gfx_ModelTableGetSpriteObject(3, g_BattleMagicDemi_ResourceHandle);
        ((BattleMagicDemiSpriteReq *)g_BattleMagicDemi_SpriteReq.animFrameTable)->spriteObject4 = Gfx_ModelTableGetSpriteObject(4, g_BattleMagicDemi_ResourceHandle);
    }
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), 0x1000 /* 1.0 */, 0xFFFFFF38);
    g_BattleMagicDemi_ChainTail = BattleFx_SubmitSpriteDraw(&g_BattleMagicDemi_SpriteReq.animFrameTable, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicDemi_ChainTail);
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= 36) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
