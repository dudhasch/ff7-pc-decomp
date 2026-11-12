// src/ff7/src/battle/battle_fx_magic_mbarrier.cpp
/* Magic Effect [16]: MBarrier (Japanese: マバリア, dev tag: "MAGIC/MBARIA" / Mbaria) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle visual effect for the MBarrier magic: loads its resource package, then per
//   target spawns an expanding, fading scaled model on the target actor.

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

struct BattleFxModelDraw;

#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x008fc4f8
int g_BattleMagicMBarrier_ResourceDefs[6] = {1638577, (int)&g_BattleMagicMBarrier_Filename, 0, 0, 0, 0};
// GLOBAL: FF7 0x008fc510
struct BattleMagicMBarrier_ModelReq g_BattleMagicMBarrier_ModelReq = {0, {0x88, 0, 0, 0, 0, 0}, 0};
unsigned char g_rawData_008fc51c[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008fc520
unsigned char g_BattleMagicMBarrier_Filename[12] = "BARI_M.RSD";
// GLOBAL: FF7 0x00be0480
int g_BattleMagicMBarrier_ChainTail = 0;
// GLOBAL: FF7 0x00be0484
int g_BattleMagicMBarrier_Scale = 0;
// GLOBAL: FF7 0x00be0488
int g_BattleMagicMBarrier_BlendFactor = 0;
// GLOBAL: FF7 0x00be048c
struct GfxTableCursor *g_BattleMagicMBarrier_ResourceHandle = 0;
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/* Loads the MBarrier magic resource package if not already loaded. */
// FUNCTION: FF7 0x005b3e40
void __cdecl BattleFx_MagicLoadResMBarrier(void)
{
    GfxModelTableBank *baseOffset;
    GfxResourceRequest requestRecord;
    int kind;
    char pathEnd[200];
    GfxResourceRequest *request;
    int resultDefCatalog;

    if (g_pBattle3dScene != 0 && g_BattleMagicMBarrier_ResourceHandle == 0) {
        kind = 1;
        resultDefCatalog = (int)&g_BattleMagicMBarrier_ResourceDefs;
        baseOffset = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestRecord;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x008fc52c
        request->resourcePath = (char *)BattleFx_BuildResourcePath("MAGIC/MBARIA", pathEnd);
        Gfx_LoadModelTableIfAbsent(kind, resultDefCatalog, request, &g_BattleMagicMBarrier_ResourceHandle, baseOffset);
    }
}

/* Entry point for the MBarrier magic effect. */
// FUNCTION: FF7 0x005b3ed0
void __cdecl BattleFx_MagicEnterMBarrier(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResMBarrier();
    BattleFx_MagicBeginMBarrier(targetMask, actorIdx);
}

/* Initializes MBarrier effect parameters and spawns per-target callback. */
// FUNCTION: FF7 0x005b3eea
void __cdecl BattleFx_MagicBeginMBarrier(int targetMask, int actorIdx)
{
    g_BattleMagicMBarrier_Scale = 0x2000;
    g_BattleMagicMBarrier_BlendFactor = 0;
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 0, BattleFx_MagicSpawnMBarrierPerTarget);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, 0x40, 0x43);
}

/* Spawns the timeline task for MBarrier on a target actor. */
// FUNCTION: FF7 0x005b3f28
void __cdecl BattleFx_MagicSpawnMBarrierPerTarget(int actor)
{
    BattleFxMBarrierSlot *effectSlot;

    effectSlot = (BattleFxMBarrierSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicMBarrierTimelineTask);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &effectSlot->posX);
    effectSlot->posX -= (short)(SinFixed(g_BattleActorRecords[actor].yaw) * g_BattleActorRecords[actor].radius >> 0xC);
    effectSlot->posZ -= (short)(CosFixed(g_BattleActorRecords[actor].yaw) * g_BattleActorRecords[actor].radius >> 0xC);
    *(Int64Pair *)&effectSlot->rot = *(Int64Pair *)&g_BattleActorRecords[actor].rotX;
    effectSlot->actorIndex = actor;
}

/* Manages the timeline and spawns scaled model tasks for MBarrier. */
// FUNCTION: FF7 0x005b4017
void __cdecl BattleFx_MagicMBarrierTimelineTask(void)
{
    BattleFxMBarrierSlot *task;
    BattleFxMBarrierSlot *child;

    task = (BattleFxMBarrierSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->step == 0) {
        child = (BattleFxMBarrierSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicMBarrierScaledModelTask);
        *(Int64Pair *)&child->posX = *(Int64Pair *)&task->posX;
        *(Int64Pair *)&child->rot = *(Int64Pair *)&task->rot;
    }
    if (task->step == 4) {
        child = (BattleFxMBarrierSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicMBarrierScaledModelTask);
        *(Int64Pair *)&child->posX = *(Int64Pair *)&task->posX;
        *(Int64Pair *)&child->rot = *(Int64Pair *)&task->rot;
    }
    if (task->step == 8) {
        child = (BattleFxMBarrierSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicMBarrierScaledModelTask);
        *(Int64Pair *)&child->posX = *(Int64Pair *)&task->posX;
        *(Int64Pair *)&child->rot = *(Int64Pair *)&task->rot;
    }
    if (task->step == 0x10) {
        Battle_PostActorHitOrHurtMessage(task->actorIndex);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->step)++;
}

/* Renders and updates the expanding/fading MBarrier model. */
// FUNCTION: FF7 0x005b415c
void __cdecl BattleFx_MagicMBarrierScaledModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[56];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; int blend; };
        struct { unsigned char pad_to_24[0x24]; BattleFxMBarrierSlot *slot; };
        struct { unsigned char pad_to_28[0x28]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxMBarrierSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleMagicMBarrier_ModelReq.tableField) = Gfx_ModelTableGetRenderObject(0, g_BattleMagicMBarrier_ResourceHandle);
    frame.scaleX.vz = (((frame.slot->step * 0x3BFF) / 16) + 0x400) * g_BattleMagicMBarrier_Scale >> 0xC;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    if (frame.slot->step < 8) {
        frame.blend = 0;
    } else {
        frame.blend = (frame.slot->step - 8 << 0xC) / 8;
    }
    frame.blend += (0x1000 - frame.blend) * g_BattleMagicMBarrier_BlendFactor >> 0xC;
    RotMatrix(&frame.slot->rot, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.slot->posX;
    frame.rotMatrix.t[1] = frame.slot->posY;
    frame.rotMatrix.t[2] = frame.slot->posZ;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleMagicMBarrier_ModelReq.alpha = *(short *)&frame.blend;
    g_BattleMagicMBarrier_ChainTail = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicMBarrier_ModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicMBarrier_ChainTail);
    if (g_BattleIsPaused == 0) {
        (frame.slot->step)++;
        if (frame.slot->step >= 0x10) {
            frame.slot->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
