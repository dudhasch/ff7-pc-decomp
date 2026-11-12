// src/ff7/src/battle/battle_fx_magic_bolt.cpp
/* Magic Effect [33]: Bolt (Japanese: サンダー Thunder, dev tag: "MAGIC/THUNDER" / Thunder) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle effect for the Bolt (Thunder) magic: loads its resources, registers a per-target
//   callback, and runs a timeline that spawns growing-and-fading models plus billboard sprites.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_magic.h"
#include "lib/types/graphics.h"
#include <stdlib.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;

#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x00be0db8
int g_BattleMagicBolt_ChainTail = 0;
unsigned char g_bssPad_00be0dbc[4] = {0};
// GLOBAL: FF7 0x00be0dc0
struct GfxTableCursor *g_BattleMagicBolt_ResourceHandle = 0;
unsigned char g_bssPad_00be0dc4[4] = {0};
// GLOBAL: FF7 0x00be0dc8
struct BattleMagicBolt_ActorMatrix g_BattleMagicBolt_ActorMatrix = {0};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008fde88
int g_BattleMagicBolt_ResourceDefs = 0x00192A39;
int g_BattleMagicBolt_ResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxMagicBoltResourceNames + 0x0)
};
int g_BattleMagicBolt_ResourceDefsDataTail[14] = {
    (int)((char *)&g_BattleFxMagicBoltResourceNames + 0x10), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxMagicBoltResourceNames + 0x1c), (int)((char *)&g_BattleFxMagicBoltResourceNames + 0x2c),
    0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleFxMagicBoltResourceNames + 0x38), 0x0, 0x0,
    0x0, 0x0
};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008fdec8
struct BattleMagicBolt_ModelReq g_BattleMagicBolt_ModelReq = {0, {8, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}, 0, 0};
// GLOBAL: FF7 0x008fdee0
unsigned char g_BattleMagicBolt_ModelScale[8] = {0x80, 0x80, 0x80, 0x2c, 0x00, 0x00, 0x00, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008fdee8
struct BattleMagicBolt_SpriteReq g_BattleMagicBolt_SpriteReq = {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}};
unsigned char g_BattleFxMagicBoltResourceNames[68] =
    "THUNDE00.TIM\0\0\0\0"
    "KAMI_1.SPT\0\0"
    "THUNDE01.TIM\0\0\0\0"
    "SPARK_1.SPT\0"
    "THUND_3.RSD";
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/* Loads the Bolt (Thunder) magic resource package if not already loaded. */
// FUNCTION: FF7 0x005b8e30
void __cdecl BattleFx_MagicLoadResBolt(void)
{
    GfxModelTableBank *stage;
    GfxResourceRequest requestSlot;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceTable;

    if (g_pBattle3dScene != 0 && g_BattleMagicBolt_ResourceHandle == 0) {
        kind = 3;
        resourceTable = (int)&g_BattleMagicBolt_ResourceDefs;
        stage = (GfxModelTableBank *)g_pBattle3dScene->modelTableBank;
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        ((BattleFxDiamondCardResource *)pRequest)->groupTag = 0;
        // STRING: FF7 0x008fdf3c
        ((BattleFxDiamondCardResource *)pRequest)->resourcePath = (int)BattleFx_BuildResourcePath("MAGIC/THUNDER", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceTable, pRequest, &g_BattleMagicBolt_ResourceHandle, stage);
    }
}

/* Entry point for the Bolt (Thunder) magic effect. */
// FUNCTION: FF7 0x005b8ec0
void __cdecl BattleFx_MagicEnterBolt(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResBolt();
    BattleFx_MagicRegisterBoltPerTargetCallback(targetMask, actorIdx);
}

/* Registers per-target callback task for Bolt (Thunder). */
// FUNCTION: FF7 0x005b8eda
void __cdecl BattleFx_MagicRegisterBoltPerTargetCallback(int targetId, int param)
{
    Battle_CreateBitPatternCallbackTask(targetId, param, 2, BattleFx_MagicSpawnBoltPerTarget);
}

/* Spawns the timeline task for Bolt (Thunder) per target actor. */
// FUNCTION: FF7 0x005b8ef6
void __cdecl BattleFx_MagicSpawnBoltPerTarget(int actor)
{
    BattleFxBoltRecord *record;

    record = (BattleFxBoltRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBoltTimelineTask);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &record->posX);
    record->actorId = actor;
    record->negRadius = -g_BattleActorRecords[actor].radius;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&record->posX), 0xB);
}

/* Manages the timeline and spawns models and sprites for Bolt (Thunder). */
// FUNCTION: FF7 0x005b8f7d
void __cdecl BattleFx_MagicBoltTimelineTask(void)
{
    BattleFxBoltTimeline *task;
    BattleFxBoltSlot *child;

    task = (BattleFxBoltTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused == 0) {
        if (task->stage == 0) {
            child = (BattleFxBoltSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBoltBillboardSpriteTask);
            *(Int64Pair *)((char *)child + 4) = *(Int64Pair *)((char *)task + 4);
            child->posY = 0;
            child->pushLength = task->negRadius;
            Battle_PostActorHitOrHurtMessage(task->recordType);
        }
        if (task->stage == 0) {
            child = (BattleFxBoltSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBoltGrowFadeModelTask);
            *(Int64Pair *)((char *)child + 4) = *(Int64Pair *)((char *)task + 4);
            child->posY = 0;
            child->offset = 0x1000;
            child->velocity = 512;
            child->pushLength = task->negRadius;
        }
        if (task->stage >= 2) {
            child = (BattleFxBoltSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBoltFlippedBillboardSpriteTask);
            child->posX = task->posX + rand() % 1000 + -500;
            child->posY = task->posY + rand() % 1000 + -500;
            child->posZ = task->posZ + rand() % 1000 + -500;
            child->variant = rand() & 3;
            child->pushLength = task->negRadius;
        }
        if ((task->stage)++ == 0xF) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/* Updates and renders a growing and fading model for Bolt (Thunder). */
// FUNCTION: FF7 0x005b9141
void __cdecl BattleFx_MagicBoltGrowFadeModelTask(void)
{
    int mtx[8];
    BattleFxBoltGrowRecord *task;

    task = (BattleFxBoltGrowRecord *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleMagicBolt_ModelReq.tableField) = Gfx_ModelTableGetRenderObject(2, g_BattleMagicBolt_ResourceHandle);
    if (task->step < 8) {
        g_BattleMagicBolt_ModelReq.alpha = 0x80;
    } else if (task->step < 16) {
        g_BattleMagicBolt_ModelReq.alpha = 0x80 - (task->step - 8 << 4);
    } else {
        task->ownerSlot = BATTLE_TASK_FREE;
        return;
    }
    g_BattleMagicBolt_ActorMatrix.offset = task->offset;
    (g_BattleMagicBolt_ActorMatrix.scaleX) = g_BattleMagicBolt_ActorMatrix.offset;
    g_BattleMagicBolt_ActorMatrix.alpha = -task->offset;
    g_BattleMagicBolt_ActorMatrix.offsetX = task->offsetX;
    g_BattleMagicBolt_ActorMatrix.offsetY = task->offsetY;
    g_BattleMagicBolt_ActorMatrix.offsetZ = task->offsetZ;
    MultiplyMatrix(&g_BattleCameraTransform, (short *)&g_BattleMagicBolt_ActorMatrix, mtx);
    SetRotMatrix(mtx);
    SetTransMatrix(mtx);
    g_BattleMagicBolt_ChainTail = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicBolt_ModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicBolt_ChainTail);
    if (g_BattleIsPaused == 0) {
        (task->step)++;
        task->offset += task->velocity;
    }
}

/* Updates and renders a billboard sprite for Bolt (Thunder). */
// FUNCTION: FF7 0x005b9295
void __cdecl BattleFx_MagicBoltBillboardSpriteTask(void)
{
    BattleFxBoltBillboardSlot *task;

    task = (BattleFxBoltBillboardSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicBolt_ModelReq.resourceHandle = Gfx_ModelTableGetLoadedRecord(0, g_BattleMagicBolt_ResourceHandle);
    g_BattleMagicBolt_ModelReq.altHandle = Gfx_ModelTableGetSpriteObject(0, g_BattleMagicBolt_ResourceHandle);
    BattleFx_InstallBillboardMatrix(&task->posX, 0x2000 /* 2.0 */, task->pushLength);
    *(short *)((char *)&g_BattleMagicBolt_ModelScale + 0x4) = task->frameCount >> 1;
    g_BattleMagicBolt_ChainTail = BattleFx_SubmitSpriteDraw(&g_BattleMagicBolt_ModelReq.resourceHandle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicBolt_ChainTail);
    if (g_BattleIsPaused == 0) {
        if ((task->frameCount)++ == 8) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/* Updates and renders a flipped billboard sprite for Bolt (Thunder). */
// FUNCTION: FF7 0x005b935d
void __cdecl BattleFx_MagicBoltFlippedBillboardSpriteTask(void)
{
    BattleFxBoltSprite *task;
    short *matrix;

    task = (BattleFxBoltSprite *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleMagicBolt_SpriteReq.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_BattleMagicBolt_ResourceHandle);
    g_BattleMagicBolt_SpriteReq.drawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleMagicBolt_ResourceHandle);
    matrix = (short *)BattleFx_InstallBillboardMatrix(
        &task->posX, 0x2000 /* 2.0 */, task->pushLength);
    if ((task->flags & 1) != 0) {
        *matrix = -*matrix;
    }
    if ((task->flags & 2) != 0) {
        matrix[4] = -matrix[4];
    }
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    g_BattleMagicBolt_SpriteReq.frame = task->frameCount;
    g_BattleMagicBolt_ChainTail = BattleFx_SubmitSpriteDraw(
        &(g_BattleMagicBolt_SpriteReq.spriteGroup), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC,
        g_BattleMagicBolt_ChainTail);
    if (g_BattleIsPaused == 0) {
        if ((task->frameCount)++ == 7) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
