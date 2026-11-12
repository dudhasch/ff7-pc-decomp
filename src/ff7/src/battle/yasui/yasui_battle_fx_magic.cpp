// src/ff7/src/battle/yasui/yasui_battle_fx_magic.cpp
/* Magic Effect [18]: Wall (Japanese: ウォール, dev tag: "MAGIC/WALL") */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle effects for the Wall magic spell: it loads the Wall resources, then places a
//   shrinking barrier model over each target, in full-length and short variants with scale and
//   alpha envelopes.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_boss.h"
#include "ff7/battle_tasks.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
extern "C" {

struct BattleFxModelDraw;
// clang-format off
// The resource record Gfx_LoadModelTableIfAbsent takes {kind 0x1900B1, name, 0, 0, 0}; its name is its literal.
// GLOBAL: FF7 0x007e3200
unsigned int g_YasuiMagicWallResourceDef[5] = {0x001900b1, (unsigned int)"WOLL.RSD", 0, 0, 0};
// GLOBAL: FF7 0x007e3218
unsigned char g_YasuiMagicWallModelDrawReq[28] = {
    0x00, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};
unsigned char g_rawData_007e3234[4] = {0x20, 0x00, 0x00, 0x00};
// clang-format on

#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
#pragma intrinsic(abs)


// clang-format off
// GLOBAL: FF7 0x00a38a94
int g_YasuiMagicWallScale = 0;
// GLOBAL: FF7 0x00a38a98
int g_YasuiMagicWallDrawOrder = 0;
// GLOBAL: FF7 0x00a38a9c
int g_YasuiMagicWallBlendFactor = 0;
// GLOBAL: FF7 0x00a38aa0
struct GfxTableCursor *g_YasuiMagicWallResourceContext = 0;
unsigned char g_bssPad_00a38aa4[4] = {0};
// clang-format on

/*
 * Loads Wall magic effect resources (WOLL.RSD, MAGIC/WALL) into g_YasuiMagicWallResourceContext
 * if not already loaded.
 */
// FUNCTION: FF7 0x00457410
void __cdecl BattleFx_MagicLoadResWall(void)
{
    char pathScratch[200];
    GfxModelTableBank *context;
    GfxResourceRequest requestRecord;
    int kind;
    GfxResourceRequest *pReq;
    int resourceRows;

    if (g_pBattle3dScene != 0 && g_YasuiMagicWallResourceContext == 0) {
        kind = 1;
        resourceRows = (int)&g_YasuiMagicWallResourceDef;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestRecord;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x007e3244
        pReq->resourcePath = BattleFx_BuildResourcePath("MAGIC/WALL", pathScratch);
        Gfx_LoadModelTableIfAbsent(kind, resourceRows, pReq, &g_YasuiMagicWallResourceContext, context);
    }
}

/*
 * Entry point for the Wall magic effect: loads resources and begins the effect.
 */
// FUNCTION: FF7 0x004574a0
void __cdecl BattleFx_MagicEnterWall(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResWall();
    BattleFx_MagicBeginWall(targetMask, actorIdx);
}

/*
 * Initializes Wall scale and blend factor, schedules per-target wall tasks,
 * and plays the Wall sound effect.
 */
// FUNCTION: FF7 0x004574ba
void __cdecl BattleFx_MagicBeginWall(int targetMask, int actorIdx)
{
    g_YasuiMagicWallScale = 0x1800 /* 1.5 */;
    g_YasuiMagicWallBlendFactor = 0x800;
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 0, BattleFx_MagicSpawnWallPerTarget);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, 0x40, 0x44);
}

/*
 * Spawns a Wall timeline task for an individual target actor.
 * Computes bone world position offset by actor facing direction.
 */
// FUNCTION: FF7 0x004574f8
void __cdecl BattleFx_MagicSpawnWallPerTarget(int actor)
{
    BattleActorWallState *actorState;
    BattleFxMagicWallTask *task;

    task = (BattleFxMagicWallTask *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicWallTimelineTask);
    actorState = (BattleActorWallState *)&g_BattleActorRecords[actor].modelId;
    Battle_GetBoneWorldPos(actor, actorState->boneIndex, &task->x);
    task->x -= (short)(SinFixed(actorState->angle) * actorState->radius >> 12);
    task->z -= (short)(CosFixed(actorState->angle) * actorState->radius >> 12);
    *(Int64Pair *)((char *)task + 0xC) = *(Int64Pair *)((char *)actorState + 0x15E);
    task->targetActor = actor;
}

/*
 * Updates Wall timeline: spawns full shrinking model at frame 0,
 * short shrinking models at frames 4 and 8, and at frame 16 triggers
 * status record allocation and terminates.
 */
// FUNCTION: FF7 0x004575cf
void __cdecl BattleFx_MagicWallTimelineTask(void)
{
    BattleFxMagicWallTimeline *task;
    BattleFxCosmoMemoryTriSpin *childTask;

    task = (BattleFxMagicWallTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frame == 0) {
        childTask = (BattleFxCosmoMemoryTriSpin *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicWallShrinkingModelTask);
        *(Int64Pair *)((char *)childTask + 0xC) = task->actorPos;
        *(Int64Pair *)((char *)childTask + 4) = task->startPos;
    }
    if (task->frame == 4) {
        childTask = (BattleFxCosmoMemoryTriSpin *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicWallShortShrinkingModelTask);
        *(Int64Pair *)((char *)childTask + 0xC) = task->actorPos;
        *(Int64Pair *)((char *)childTask + 4) = task->startPos;
    }
    if (task->frame == 8) {
        childTask = (BattleFxCosmoMemoryTriSpin *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicWallShortShrinkingModelTask);
        *(Int64Pair *)((char *)childTask + 0xC) = task->actorPos;
        *(Int64Pair *)((char *)childTask + 4) = task->startPos;
    }
    if (task->frame == 16) {
        Battle_PostActorHitOrHurtMessage(task->actorIndex);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frame)++;
}

/*
 * Renders the 32-frame shrinking Wall barrier model with scale and alpha envelope.
 */
// FUNCTION: FF7 0x00457714
void __cdecl BattleFx_MagicWallShrinkingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        int fade;
        struct { unsigned char pad_to_4[0x4]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_24[0x24]; int envelope; };
        struct { unsigned char pad_to_28[0x28]; BattleFxCosmoMemoryTriSpin *record; };
        struct { unsigned char pad_to_2C[0x2C]; int step; };
        struct { unsigned char pad_to_30[0x30]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.record = (BattleFxCosmoMemoryTriSpin *)BATTLE_CURRENT_TASK_SLOT;
    *(int *)&g_YasuiMagicWallModelDrawReq = Gfx_ModelTableGetRenderObject(0, g_YasuiMagicWallResourceContext);
    if (frame.record->step < 16) {
        frame.envelope = (frame.record->step * -0x2000) / 16 + 0x3000;
    } else {
        frame.envelope = 0x1000;
    }
    frame.scaleX.vz = frame.envelope * g_YasuiMagicWallScale >> 12;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    frame.step = frame.record->step;
    if (frame.step < 8) {
        frame.fade = 4096 - (frame.step << 12) / 8;
    } else {
        frame.step -= 8;
        if (frame.step < 16) {
            frame.fade = 0;
        } else {
            frame.step -= 16;
            if (frame.step < 8) {
                frame.fade = (frame.step << 12) / 8;
            }
        }
    }
    frame.fade += (4096 - frame.fade) * g_YasuiMagicWallBlendFactor >> 12;
    RotMatrix((SVECTOR *)((int)frame.record + 0xC), &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.record->posX;
    frame.rotMatrix.t[1] = frame.record->posY;
    frame.rotMatrix.t[2] = frame.record->posZ;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    SetDrawColorRGB(0, 0, 0);
    ((BattleFxModelDraw *)&g_YasuiMagicWallModelDrawReq)->fadeLevel = *(short *)&frame.fade;
    g_YasuiMagicWallDrawOrder = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiMagicWallModelDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiMagicWallDrawOrder);
    if (g_BattleIsPaused == 0) {
        (frame.record->step)++;
        if (frame.record->step >= 32) {
            frame.record->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Renders the 16-frame short shrinking Wall barrier model with scale and alpha envelope.
 */
// FUNCTION: FF7 0x004578f0
void __cdecl BattleFx_MagicWallShortShrinkingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[60];
        int fade;
        struct { unsigned char pad_to_4[0x4]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_24[0x24]; BattleFxCosmoMemoryTriSpin *record; };
        struct { unsigned char pad_to_28[0x28]; int step; };
        struct { unsigned char pad_to_2C[0x2C]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.record = (BattleFxCosmoMemoryTriSpin *)BATTLE_CURRENT_TASK_SLOT;
    frame.scaleX.vz = ((frame.record->step * -0x2000) / 16 + 0x3000) * g_YasuiMagicWallScale >> 12;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    frame.step = frame.record->step;
    if (frame.step < 8) {
        frame.fade = 4096 - (frame.step << 12) / 8;
    } else {
        frame.step -= 8;
        if (frame.step < 8) {
            frame.fade = (frame.step << 12) / 8;
        }
    }
    frame.fade += (4096 - frame.fade) * g_YasuiMagicWallBlendFactor >> 12;
    RotMatrix((SVECTOR *)((int)frame.record + 0xC), &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.record->posX;
    frame.rotMatrix.t[1] = frame.record->posY;
    frame.rotMatrix.t[2] = frame.record->posZ;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    SetDrawColorRGB(0, 0, 0);
    ((BattleFxModelDraw *)&g_YasuiMagicWallModelDrawReq)->blendFraction = *(short *)&frame.fade;
    g_YasuiMagicWallDrawOrder = BattleFx_SubmitModelDraw((BattleFxModelDraw *)((char *)&g_YasuiMagicWallModelDrawReq + 16), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiMagicWallDrawOrder);
    if (g_BattleIsPaused == 0) {
        (frame.record->step)++;
        if (frame.record->step >= 16) {
            frame.record->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
