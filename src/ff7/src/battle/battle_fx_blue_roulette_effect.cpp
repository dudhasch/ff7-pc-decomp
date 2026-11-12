// src/ff7/src/battle/battle_fx_blue_roulette_effect.cpp
// Enemy Skill [20..21]: Death Sentence & Roulette (Japanese: 死の宣告 / 死のルーレット, dev tag: "blue/ruretto" / Ruretto)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The Ruretto blue magic effect: loads its resources, records the first targeted slot,
//   then runs its begin, timeline and selection tasks.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "lib/codegen.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_fx.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/enums/graphics.h"
extern "C" {

struct GfxPsxSpriteQuadState;

// clang-format off
// GLOBAL: FF7 0x008a8c18
int g_BattleBlueRouletteResourceDef = 79932;
char *g_BattleFxBlueRouletteEffectResourceNames[1] = {
    // STRING: FF7 0x008a8d00
    "YUBI.TIM"
};
unsigned int g_BattleBlueRouletteResourceDefDataTail[8] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x00013838,
    // STRING: FF7 0x008a8d0c
    (unsigned int)"YUBI.TIM",
    0x00000000,
    0x00000000,
    0x00000000
};
// The selection task reads one entry past the end, which is g_BattleBlueRouletteStopDelay.
// GLOBAL: FF7 0x008a8c40
int g_BattleBlueRouletteStepDelays[47] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 4, 4, 6};
// GLOBAL: FF7 0x008a8cfc
int g_BattleBlueRouletteStopDelay = 6;
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b74f98
int g_BattleBlueRouletteSpinTimer = 0;
// GLOBAL: FF7 0x00b74f9c
int g_BattleBlueRouletteSelectedActor = 0;
// GLOBAL: FF7 0x00b74fa0
int g_BattleBlueRouletteTargetIndex = 0;
// GLOBAL: FF7 0x00b74fa4
int g_BattleBlueRouletteTargetCount = 0;
// GLOBAL: FF7 0x00b74fa8
int g_BattleBlueRouletteStepIndex = 0;
// GLOBAL: FF7 0x00b74fac
int g_BattleBlueRouletteVariantMode = 0;
// Actor indices of the targets the selection cursor cycles through.
// GLOBAL: FF7 0x00b74fb0
int g_BattleBlueRouletteValidTargets[10] = {0};
// GLOBAL: FF7 0x00b74fd8
int g_BattleBlueRouletteCursorIndex = 0;
// GLOBAL: FF7 0x00b74fdc
int g_BattleBlueRouletteTargetMask = 0;
// GLOBAL: FF7 0x00b74fe0
int g_BattleBlueRouletteSpinState = 0;
// GLOBAL: FF7 0x00b74fe4
struct GfxTableCursor *g_BattleBlueRouletteLoadedRes = 0;
// clang-format on

/*
 * Loads resources for the Ruretto blue magic effect when not already loaded.
 */
// FUNCTION: FF7 0x00565ec0
void __cdecl BattleFx_BlueLoadResRoulette(void)
{
    GfxModelTableBank *stageSceneHandle;
    GfxResourceRequest reqHandle;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int defBlock;

    if (g_pBattle3dScene != 0 && g_BattleBlueRouletteLoadedRes == 0) {
        kind = 2;
        defBlock = (int)&g_BattleBlueRouletteResourceDef;
        stageSceneHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqHandle;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a8d18
        pRequest->resourcePath = (char *)BattleFx_BuildResourcePath("blue/ruretto", pathText);
        Gfx_LoadModelTableIfAbsent(kind, defBlock, pRequest, &g_BattleBlueRouletteLoadedRes, stageSceneHandle);
    }
}

// Sets g_BattleBlueRouletteVariantMode to 0, loads Ruretto resources, calls BattleFx_BlueBeginRoulette with both arguments, and
// returns zero.
// FUNCTION: FF7 0x00565f50
int __cdecl BattleFx_BlueEnterRoulette(int targetMask, int actorIdx)
{
    g_BattleBlueRouletteVariantMode = 0;
    BattleFx_BlueLoadResRoulette();
    BattleFx_BlueBeginRoulette(targetMask, actorIdx);
    return 0;
}

/*
 * Initializes and begins the Ruretto blue magic effect.
 */
// FUNCTION: FF7 0x00565f76
int __cdecl BattleFx_BlueEnterRouletteAlt(int targetMask, int actorIdx)
{
    g_BattleBlueRouletteVariantMode = 1;
    BattleFx_BlueLoadResRoulette();
    BattleFx_BlueBeginRoulette(targetMask, actorIdx);
    return 0;
}

// Stores targetMask, records the first set bit among its low ten bits into g_BattleBlueRouletteSelectedActor when one exists,
// and allocates BattleFx_BlueRouletteBeginTask.
// FUNCTION: FF7 0x00565f9c
void __cdecl BattleFx_BlueBeginRoulette(int targetMask, int casterIdx)
{
    int actorIdx, setCount;

    g_BattleBlueRouletteTargetMask = targetMask;
    actorIdx = 0;
    for (setCount = 0; actorIdx < 0xA; actorIdx++) {
        if ((targetMask & 1 << actorIdx) != 0) {
            setCount++;
            if (setCount == 1) {
                g_BattleBlueRouletteSelectedActor = actorIdx;
            }
        }
    }
    Battle_AllocateResourceSlot(BattleFx_BlueRouletteBeginTask);
}

// Starts the Rurretto blue magic effect for the selected actor. It clears the active task slot's
// owner, then spawns either the selection task (variant mode) or the timeline task, and writes the
// actor's alt bone world position and yaw into the new slot. In variant mode it collects the target
// mask bits that are not set in incapacitatedMask into the valid target list, picks a random starting target,
// and loads the first step delay.
// FUNCTION: FF7 0x00566007
void __cdecl BattleFx_BlueRouletteBeginTask(void)
{
    int bitIdx, validCount, targetMask;
    BattleFxRouletteSlot *selfPtr;

    selfPtr = (BattleFxRouletteSlot *)BATTLE_CURRENT_TASK_SLOT;
    selfPtr->ownerSlot = BATTLE_TASK_FREE;
    if (g_BattleBlueRouletteVariantMode != 0) {
        selfPtr = (BattleFxRouletteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueRouletteRunSelectionTask);
        Battle_GetBoneWorldPos(g_BattleBlueRouletteSelectedActor, g_BattleActorRecords[g_BattleBlueRouletteSelectedActor].altBone, (short *)&selfPtr->pos);
        selfPtr->actorYaw = g_BattleActorRecords[g_BattleBlueRouletteSelectedActor].yaw;
        targetMask = g_BattleScene.targetMaskBits & ~(g_BattleScene.incapacitatedMask);
        g_BattleBlueRouletteCursorIndex = targetMask;
        bitIdx = 0;
        for (validCount = 0; bitIdx < 0xA; bitIdx++) {
            if ((targetMask & 1 << bitIdx) != 0) {
                g_BattleBlueRouletteValidTargets[validCount] = bitIdx;
                validCount++;
            }
        }
        g_BattleBlueRouletteTargetCount = validCount;
        g_BattleBlueRouletteTargetIndex = rand() % validCount;
        g_BattleBlueRouletteStepIndex = 0;
        g_BattleBlueRouletteSpinTimer = g_BattleBlueRouletteStepDelays[g_BattleBlueRouletteStepIndex];
        g_BattleBlueRouletteSpinState = 0;
    } else {
        selfPtr = (BattleFxRouletteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueRouletteTimelineTask);
        Battle_GetBoneWorldPos(g_BattleBlueRouletteSelectedActor, g_BattleActorRecords[g_BattleBlueRouletteSelectedActor].altBone, (short *)&selfPtr->pos);
        selfPtr->actorYaw = g_BattleActorRecords[g_BattleBlueRouletteSelectedActor].yaw;
    }
}

// Per-frame task of the Ruretto effect. On frame 1 it copies its position and yaw into a new
// ring-buffer record (Battle_AllocSceneMessage(1)); on frame 0x11 it plays the sound at the position;
// on frame 0x20 it allocates a second ring-buffer record; it frees its slot at frame 0x32. While
// g_BattleIsPaused is clear it calls Battle_PostActorHitOrHurtMessage(g_BattleBlueRouletteSelectedActor) on frame
// 0x24 and advances the frame counter.
// FUNCTION: FF7 0x005661a0
void __cdecl BattleFx_BlueRouletteTimelineTask(void)
{
    BattleFxRouletteRingRecord *record;
    BattleFxRouletteSlot *task;

    task = (BattleFxRouletteSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (task->frameCount == 1) {
        record = (BattleFxRouletteRingRecord *)Battle_AllocSceneMessage(1);
        record->pos = task->pos;
        record->yaw = task->actorYaw;
    } else if (task->frameCount == 0x11) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&task->pos), 0x1A4);
    } else if (task->frameCount == 0x20) {
        Battle_AllocSceneMessage(2);
    }
    if (task->frameCount >= 0x32) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCount == 0x24) {
        Battle_PostActorHitOrHurtMessage(g_BattleBlueRouletteSelectedActor);
    }
    task->frameCount++;
}

// Cycles a cursor over the valid targets at the speed of g_BattleBlueRouletteStepDelays, drawing it as
// a textured quad at the current target, and starts the timeline task once the cursor stops on the
// selected actor.
// FUNCTION: FF7 0x00566287
void __cdecl BattleFx_BlueRouletteRunSelectionTask(void)
{
    BattleFxRoulettePrimitive *primPtr;
    BattleFxBlueRouletteRunState *runState;
    int actorIdx;
    BattleFxRouletteSlot *timelineTask;
    int padBlock[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxRouletteSlot *task;

    task = (BattleFxRouletteSlot *)BATTLE_CURRENT_TASK_SLOT;
    runState = (BattleFxBlueRouletteRunState *)Gfx_GetPsxScratchpadPtr(0);
    if (g_BattleBlueRouletteSpinState == 0 && g_BattleIsPaused == 0) {
        g_BattleBlueRouletteSpinTimer--;
        if (g_BattleBlueRouletteSpinTimer <= 0) {
            g_BattleBlueRouletteTargetIndex++;
            if (g_BattleBlueRouletteTargetIndex >= g_BattleBlueRouletteTargetCount) {
                g_BattleBlueRouletteTargetIndex = 0;
            }
            g_BattleBlueRouletteStepIndex++;
            if (g_BattleBlueRouletteStepIndex >= 0x30) {
                if (g_BattleBlueRouletteSelectedActor == g_BattleBlueRouletteValidTargets[g_BattleBlueRouletteTargetIndex]) {
                    g_BattleBlueRouletteSpinState++;
                }
                g_BattleBlueRouletteSpinTimer = g_BattleBlueRouletteStopDelay;
            } else {
                g_BattleBlueRouletteSpinTimer = g_BattleBlueRouletteStepDelays[g_BattleBlueRouletteStepIndex];
            }
            actorIdx = g_BattleBlueRouletteValidTargets[g_BattleBlueRouletteTargetIndex];
            Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].targetBone, runState->bonePos);
            Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(runState->bonePos), 0x1E3);
        }
    }
    actorIdx = g_BattleBlueRouletteValidTargets[g_BattleBlueRouletteTargetIndex];
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].targetBone, runState->bonePos);
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    ProjectPointHardware(runState->bonePos, (short *)&runState->corner[0], &runState->projectFlagA, &runState->projectFlagB);
    runState->corner[3] = runState->corner[0];
    runState->corner[2] = runState->corner[3];
    runState->corner[1] = runState->corner[2];
    Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(0, g_BattleBlueRouletteLoadedRes)));
    primPtr = (BattleFxRoulettePrimitive *)Gfx_GetPsxPacketScratchPtr();
    if (actorIdx < 4) {
        runState->corner[0].xy.x += (short)(g_BattleUiScale * -0x18);
        runState->corner[2].xy.x += (short)(g_BattleUiScale * -0x18);
        primPtr->vertex[0].tex.word = 0x78480030;
        primPtr->vertex[1].tex.word = 0x4D0000;
        primPtr->vertex[2].tex.word = 0x2030;
        primPtr->vertex[3].tex.word = 0x2000;
    } else {
        runState->corner[1].xy.x += (short)(g_BattleUiScale * 0x18);
        runState->corner[3].xy.x += (short)(g_BattleUiScale * 0x18);
        primPtr->vertex[0].tex.word = 0x78480000;
        primPtr->vertex[1].tex.word = 0x4D0030;
        primPtr->vertex[2].tex.word = 0x2000;
        primPtr->vertex[3].tex.word = 0x2030;
    }
    runState->corner[2].xy.y += (short)(g_BattleUiScale << 4);
    runState->corner[3].xy.y += (short)(g_BattleUiScale << 4);
    primPtr->vertex[0].xy = runState->corner[0].packed;
    primPtr->vertex[1].xy = runState->corner[1].packed;
    primPtr->vertex[2].xy = runState->corner[2].packed;
    primPtr->vertex[3].xy = runState->corner[3].packed;
    primPtr->rgbc.color = 0x808080;
    primPtr->tag = primPtr->tag & 0xFFFFFF | 0x9000000;
    primPtr->rgbc.code.primCode = PSX_PRIM_POLY_FT4;
    Gfx_SetBlendFactorPair(FLOAT_0_2_BITS, FLOAT_ONE_BITS);
    Gfx_DrawPsxPrimitive(primPtr);
    Gfx_SetPsxPrimQuadState(0xB, 0, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(1, g_BattleBlueRouletteLoadedRes)));
    primPtr->vertex[1].tex.uv.v = 0x21;
    primPtr->vertex[0].tex.uv.v = 0x21;
    primPtr->vertex[3].tex.uv.v = 0x40;
    primPtr->vertex[2].tex.uv.v = 0x40;
    primPtr->rgbc.code.primCode = PSX_PRIM_POLY_FT4_TRANS;
    Gfx_SetBlendFactorPair(FLOAT_0_2_BITS, FLOAT_ONE_BITS);
    Gfx_SetPsxBlendMode(0);
    Gfx_SetPsxCornerAlphas(0x80, 0x80, 0x80, 0x80);
    Gfx_DrawPsxPrimitive(primPtr);
    g_BattleOverlayDrawListHead = (int)primPtr;
    if (g_BattleBlueRouletteSpinState != 0) {
        if (g_BattleBlueRouletteSpinState == 0xA) {
            timelineTask = (BattleFxRouletteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueRouletteTimelineTask);
            timelineTask->pos = task->pos;
            timelineTask->actorYaw = task->actorYaw;
        }
        if (g_BattleBlueRouletteSpinState >= 0x2D) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
        g_BattleBlueRouletteSpinState++;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->frameCount++;
}
} /* extern "C" */
