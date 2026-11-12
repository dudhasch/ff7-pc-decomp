// src/ff7/src/battle/battle_fx_magic_reflect.cpp
/* Magic Effect [17]: Reflect (Japanese: リフレク, dev tag: "MAGIC/RIFREK" / Rifrek) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle visual effect for the Reflect magic: loads its resource package, then per target
//   spawns an expanding, fading scaled model on the target actor.

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
// GLOBAL: FF7 0x008fc4b0
int g_BattleMagicReflect_ResourceDefs[6] = {1638577, (int)&g_BattleMagicReflect_Filename, 0, 0, 0, 0};
// GLOBAL: FF7 0x008fc4c8
struct BattleMagicMBarrier_ModelReq g_BattleMagicReflect_ModelReq = {0, {0x88, 0, 0, 0, 0, 0}, 0};
unsigned char g_rawData_008fc4d4[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008fc4d8
unsigned char g_BattleMagicReflect_Filename[12] = "RIFREK.RSD";
// GLOBAL: FF7 0x00be0470
int g_BattleMagicReflect_Scale = 0;
// GLOBAL: FF7 0x00be0474
int g_BattleMagicReflect_BlendFactor = 0;
// GLOBAL: FF7 0x00be0478
int g_BattleMagicReflect_ChainTail = 0;
// GLOBAL: FF7 0x00be047c
struct GfxTableCursor *g_BattleMagicReflect_ResourceHandle = 0;
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/* Loads the Reflect magic resource package if not already loaded. */
// FUNCTION: FF7 0x005b3970
void __cdecl BattleFx_MagicLoadResReflect(void)
{
    GfxModelTableBank *baseOffset;
    GfxResourceRequest requestRecord;
    int kind;
    char pathEnd[200];
    GfxResourceRequest *request;
    int resultDefCatalog;

    if (g_pBattle3dScene != 0 && g_BattleMagicReflect_ResourceHandle == 0) {
        kind = 1;
        resultDefCatalog = (int)&g_BattleMagicReflect_ResourceDefs;
        baseOffset = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestRecord;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x008fc4e4
        request->resourcePath = (char *)BattleFx_BuildResourcePath("MAGIC/RIFREK", pathEnd);
        Gfx_LoadModelTableIfAbsent(kind, resultDefCatalog, request, &g_BattleMagicReflect_ResourceHandle, baseOffset);
    }
}

/* Entry point for the Reflect magic effect. */
// FUNCTION: FF7 0x005b3a00
void __cdecl BattleFx_MagicEnterReflect(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResReflect();
    BattleFx_MagicBeginReflect(targetMask, actorIdx);
}

/* Initializes Reflect effect parameters and spawns per-target callback. */
// FUNCTION: FF7 0x005b3a1a
void __cdecl BattleFx_MagicBeginReflect(int targetMask, int actorIdx)
{
    g_BattleMagicReflect_Scale = 0x2000 /* 2.0 */;
    g_BattleMagicReflect_BlendFactor = 0;
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 0, BattleFx_MagicSpawnReflectPerTarget);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, 0x40, 0x48);
}

/* Spawns the timeline task for Reflect on a target actor. */
// FUNCTION: FF7 0x005b3a58
void __cdecl BattleFx_MagicSpawnReflectPerTarget(int actor)
{
    BattleActorWallState *modelStatus;
    BattleFxReflectSpawnSlot *effectSlot;

    effectSlot = (BattleFxReflectSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicReflectTimelineTask);
    modelStatus = (BattleActorWallState *)&g_BattleActorRecords[actor].modelId;
    Battle_GetBoneWorldPos(actor, modelStatus->boneIndex, &effectSlot->posX);
    effectSlot->posX -= (short)(SinFixed(modelStatus->angle) * modelStatus->radius >> 12);
    effectSlot->posZ -= (short)(CosFixed(modelStatus->angle) * modelStatus->radius >> 12);
    effectSlot->rotation = *(Int64Pair *)((char *)modelStatus + 0x15E);
    effectSlot->actorIndex = actor;
}

/* Manages the timeline and spawns scaled model tasks for Reflect. */
// FUNCTION: FF7 0x005b3b2f
void __cdecl BattleFx_MagicReflectTimelineTask(void)
{
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxReflectTimelineRecord *task;
    BattleFxReflectTimeline *child;

    task = (BattleFxReflectTimelineRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->step == 0) {
        child = (BattleFxReflectTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicReflectScaledModelTask);
        child->position = task->position;
        child->rotation = task->rotation;
        child->scale = 0x200 /* 0.125 */;
        child->distance = 352;
    }
    if (task->step == 8) {
        child = (BattleFxReflectTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicReflectScaledModelTask);
        child->position = task->position;
        child->rotation = task->rotation;
        child->scale = 0x200 /* 0.125 */;
        child->distance = 384;
    }
    if (task->step == 16) {
        child = (BattleFxReflectTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicReflectScaledModelTask);
        child->position = task->position;
        child->rotation = task->rotation;
        child->scale = 0x200 /* 0.125 */;
        child->distance = 416;
    }
    if (task->step == 24) {
        Battle_PostActorHitOrHurtMessage(task->recordType);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->step)++;
}

/* Renders and updates the expanding/fading Reflect model. */
// FUNCTION: FF7 0x005b3caa
void __cdecl BattleFx_MagicReflectScaledModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[60];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; int blend; };
        struct { unsigned char pad_to_24[0x24]; BattleFxReflectScaledSlot *slot; };
        struct { unsigned char pad_to_2C[0x2C]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxReflectScaledSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleMagicReflect_ModelReq.tableField) = Gfx_ModelTableGetRenderObject(0, g_BattleMagicReflect_ResourceHandle);
    frame.scaleX.vz = frame.slot->scale * g_BattleMagicReflect_Scale >> 12;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    if (frame.slot->frame < 8) {
        frame.blend = 0;
    } else {
        frame.blend = (frame.slot->frame - 8 << 12) / 8;
    }
    frame.blend += (0x1000 - frame.blend) * g_BattleMagicReflect_BlendFactor >> 12;
    RotMatrix(&frame.slot->rotation, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.slot->posX;
    frame.rotMatrix.t[1] = frame.slot->posY;
    frame.rotMatrix.t[2] = frame.slot->posZ;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleMagicReflect_ModelReq.alpha = *(short *)&frame.blend;
    g_BattleMagicReflect_ChainTail = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicReflect_ModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicReflect_ChainTail);
    if (g_BattleIsPaused == 0) {
        (frame.slot->frame)++;
        if (frame.slot->frame >= 16) {
            frame.slot->slotTag = 0xFFFF;
        } else {
            frame.slot->scale += frame.slot->scaleStep;
        }
    }
}
} /* extern "C" */
