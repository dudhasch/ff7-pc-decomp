// src/ff7/src/battle/battle_fx_magic_ice2.cpp
/* Magic Effect [31]: Ice2 (Japanese: ブリザラ Blizzara, dev tag: "MAGIC/BRIZAR2" / Brizar2) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle effect for the Blizzara (Brizar2) magic: loads its resource package, spawns
//   per-target timeline tasks, and animates shrinking-and-spinning and growing-holding-fading ice
//   models.

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
// GLOBAL: FF7 0x008fd900
int g_BattleMagicIce2_ResourceDefs[6] = {1638577, (int)&g_BattleMagicIce2_Filename, 0, 0, 0, 0};
// GLOBAL: FF7 0x008fd918
struct BattleMagicIce2_ModelReq g_BattleMagicIce2_ModelReq = {0, {8, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}, 0, {8, 0, 0, 0, 0, 0}, 128};
unsigned char g_rawData_008fd934[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008fd938
unsigned char g_BattleMagicIce2_Filename[12] = "BRIZAR.RSD";
// GLOBAL: FF7 0x00be0d80
int g_BattleMagicIce2_ChainTail = 0;
// GLOBAL: FF7 0x00be0d84
struct GfxTableCursor *g_BattleMagicIce2_ResourceHandle = 0;
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)



/* Loads the Blizzara (Brizar2) magic resource package if not already loaded. */
// FUNCTION: FF7 0x005b5e10
void __cdecl BattleFx_MagicLoadResIce2(void)
{
    GfxModelTableBank *baseOffset;
    GfxResourceRequest requestRecord;
    int kind;
    char pathEnd[200];
    GfxResourceRequest *request;
    int resultDefCatalog;

    if (g_pBattle3dScene != 0 && g_BattleMagicIce2_ResourceHandle == 0) {
        kind = 1;
        resultDefCatalog = (int)&g_BattleMagicIce2_ResourceDefs;
        baseOffset = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestRecord;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x008fd944
        request->resourcePath = BattleFx_BuildResourcePath("MAGIC/BRIZAR2", pathEnd);
        Gfx_LoadModelTableIfAbsent(kind, resultDefCatalog, request, &g_BattleMagicIce2_ResourceHandle, baseOffset);
    }
}

/* Entry point for the Blizzara (Brizar2) magic effect. */
// FUNCTION: FF7 0x005b5ea0
void __cdecl BattleFx_MagicEnterIce2(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResIce2();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 0, BattleFx_MagicSpawnIce2PerTarget);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(g_BattleEffectScratch.targetMask), 0x1D);
}

/* Spawns the timeline task for Blizzara (Brizar2) per target actor. */
// FUNCTION: FF7 0x005b5ede
void __cdecl BattleFx_MagicSpawnIce2PerTarget(int actor)
{
    BattleFxMagicIce2Timeline *effectSlot;

    effectSlot = (BattleFxMagicIce2Timeline *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicIce2TimelineTask);
    effectSlot->actorIndex = actor;
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &effectSlot->pos.vx);
}

/* Timeline of Blizzara: starts the spinning ice models, then the grow-hold-fade models, and the hit at the end. */
// FUNCTION: FF7 0x005b5f30
void __cdecl BattleFx_MagicIce2TimelineTask(void)
{
    BattleFxMagicIce2Timeline *task;
    BattleFxIce2TimelineSlot *child;

    task = (BattleFxMagicIce2Timeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->timer == 0) {
        child = (BattleFxIce2TimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicIce2ShrinkingSpinModelTask);
        child->pos = task->pos;
        child->velocityZ = 0;
        child->velocityY = 0;
        child->velocityX = 0;
        child = (BattleFxIce2TimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicIce2ShrinkingSpinModelTask);
        child->pos = task->pos;
        child->velocityY = 512;
        child->velocityX = 512;
        child->velocityZ = 0xFE00;
        child = (BattleFxIce2TimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicIce2ShrinkingSpinModelTask);
        child->pos = task->pos;
        child->velocityY = 0xFE00;
        child->velocityX = 0xFE00;
        child->velocityZ = 512;
    }
    if (task->timer == 15) {
        child = (BattleFxIce2TimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicIce2GrowHoldFadeModelTask);
        child->holdFrames = 4;
        child->pos = task->pos;
        child->velocityZ = 0;
        child->velocityY = 0;
        child->velocityX = 0;
    }
    if (task->timer == 17) {
        child = (BattleFxIce2TimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicIce2GrowHoldFadeModelTask);
        child->holdFrames = 2;
        child->pos = task->pos;
        child->velocityY = 512;
        child->velocityX = 512;
        child->velocityZ = 0xFE00;
    }
    if (task->timer == 19) {
        child = (BattleFxIce2TimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicIce2GrowHoldFadeModelTask);
        child->holdFrames = 0;
        child->pos = task->pos;
        child->velocityY = 0xFE00;
        child->velocityX = 0xFE00;
        child->velocityZ = 512;
    }
    if (task->timer == 25) {
        Battle_PostActorHitOrHurtMessage(task->actorIndex);
    }
    if (task->timer == 25) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->timer)++;
}

/* Updates and renders a shrinking and spinning Blizzara (Brizar2) model. */
// FUNCTION: FF7 0x005b619e
void __cdecl BattleFx_MagicIce2ShrinkingSpinModelTask(void)
{
    BattleIce2ShrinkState *pScales;
    int localFrame;
    BattleFxIce2SpinSlot *task;
    PsxMatrix *matrix;

    task = (BattleFxIce2SpinSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleMagicIce2_ModelReq.modelEntry) = Gfx_ModelTableGetRenderObject(0, g_BattleMagicIce2_ResourceHandle);
    pScales = (BattleIce2ShrinkState *)Gfx_GetPsxScratchpadPtr(0);
    matrix = &pScales->matrix;
    pScales->nextValue = task->step * -0xC00 / 15 + 0xC00;
    pScales->curValue = pScales->nextValue;
    pScales->prevValue = pScales->curValue;
    localFrame = task->step;
    if (localFrame < 5) {
        g_BattleMagicIce2_ModelReq.spinFadeLevel = (localFrame << 7) / 5;
    } else {
        localFrame -= 5;
        if (localFrame < 10) {
            g_BattleMagicIce2_ModelReq.spinFadeLevel = 128;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    RotMatrix(&task->spin, matrix);
    ScaleMatrix(matrix, pScales);
    matrix->t[0] = task->pos.vx;
    matrix->t[1] = task->pos.vy;
    matrix->t[2] = task->pos.vz;
    MultiplyMatrix(&g_BattleCameraTransform, matrix, matrix);
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    g_BattleMagicIce2_ChainTail = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicIce2_ModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicIce2_ChainTail);
    if (g_BattleIsPaused == 0) {
        (task->step)++;
        task->spin.vx += 0x100 /* 22.5 deg */;
        task->spin.vy += 0x80;
        task->spin.vz += 0x40;
    }
}

/* Updates and renders a growing, holding, and fading Blizzara (Brizar2) model. */
// FUNCTION: FF7 0x005b635b
void __cdecl BattleFx_MagicIce2GrowHoldFadeModelTask(void)
{
    BattleFxIce2GrowHoldFadeState *pScales;
    int localFrame;
    BattleFxIce2GrowTask *task;
    PsxMatrix *matrix;

    task = (BattleFxIce2GrowTask *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicIce2_ModelReq.growModelEntry = Gfx_ModelTableGetRenderObject(0, g_BattleMagicIce2_ResourceHandle);
    pScales = (BattleFxIce2GrowHoldFadeState *)Gfx_GetPsxScratchpadPtr(0);
    matrix = &pScales->matrix;
    localFrame = task->frame;
    if (localFrame < 5) {
        g_BattleMagicIce2_ModelReq.growFadeLevel = 128;
        pScales->growLevel = (localFrame << 11) / 5;
        pScales->holdLevel = pScales->growLevel;
        pScales->fadeLevel = pScales->holdLevel;
    } else {
        localFrame -= 5;
        if (localFrame < task->holdFrames + 5) {
            g_BattleMagicIce2_ModelReq.growFadeLevel = 128;
            pScales->growLevel = 0x800 /* 0.5 */;
            pScales->holdLevel = 0x800 /* 0.5 */;
            pScales->fadeLevel = 0x800 /* 0.5 */;
        } else {
            localFrame -= task->holdFrames + 5;
            if (localFrame < 5) {
                g_BattleMagicIce2_ModelReq.growFadeLevel = localFrame * -128 / 5 + 128;
                pScales->growLevel = 0x800 /* 0.5 */;
                pScales->holdLevel = 0x800 /* 0.5 */;
                pScales->fadeLevel = 0x800 /* 0.5 */;
            } else {
                task->holdFrames = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    RotMatrix(&task->rotation, matrix);
    ScaleMatrix(matrix, pScales);
    matrix->t[0] = task->pos.vx;
    matrix->t[1] = task->pos.vy;
    matrix->t[2] = task->pos.vz;
    MultiplyMatrix(&g_BattleCameraTransform, matrix, matrix);
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    g_BattleMagicIce2_ChainTail = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicIce2_ModelReq.growModelEntry, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicIce2_ChainTail);
    if (g_BattleIsPaused == 0) {
        (task->frame)++;
    }
}
} /* extern "C" */
