// src/ff7/src/battle/battle_fx_magic_barrier.cpp
/* Magic Effect [15]: Barrier (Japanese: バリア, dev tag: "MAGIC/BARIA" / Baria) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle visual effect for the Barrier magic: a timeline task spawns single or dual
//   growing and fading barrier models on each target actor.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include "ff7/types/battle_magic.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
extern "C" {

struct BattleFxModelDraw;

#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x008fc540
int g_BattleMagicBarrier_ResourceDefs = 0x003200B4;
int g_BattleMagicBarrier_ResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxMagicBarrierResourceNames + 0x0)
};
int g_BattleMagicBarrier_ResourceDefsDataTail[18] = {
    0x0, 0x0, 0x0, 0x3200b4, (int)((char *)&g_BattleFxMagicBarrierResourceNames + 0xc), 0x0,
    0x0, 0x0, 0x3200b1, (int)((char *)&g_BattleFxMagicBarrierResourceNames + 0x18), 0x0, 0x0,
    0x0, 0x3200b1, (int)((char *)&g_BattleFxMagicBarrierResourceNames + 0x24), 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x008fc590
BattleMagicBarrier_ModelReq1 g_BattleMagicBarrier_ModelReq1 = {{0, 0, -500, 0}, 0, 0, {0}, 0};
unsigned char g_rawData_008fc5a4[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008fc5a8
struct PsxSVectorZ32 g_BattleMagicBarrier_OffsetVector = {0, 0, 0xfe0c};
// GLOBAL: FF7 0x008fc5b0
BattleMagicBarrier_ModelReq2 g_BattleMagicBarrier_ModelReq2 = {0, 0, {0}, 0, 0x20};
unsigned char g_BattleFxMagicBarrierResourceNames[48] =
    "BARI_A1.RSD\0"
    "BARI_A2.RSD\0"
    "BARI_A1.RSD\0"
    "BARI_A2.RSD";
// GLOBAL: FF7 0x00be0490
int g_BattleMagicBarrier_ChainTail = 0;
// GLOBAL: FF7 0x00be0494
int g_BattleMagicBarrier_Scale = 0;
// GLOBAL: FF7 0x00be0498
struct GfxTableCursor *g_BattleMagicBarrier_ResourceHandle = 0;
unsigned char g_bssPad_00be049c[4] = {0};
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/* Loads the Barrier magic resource package if not already loaded. */
// FUNCTION: FF7 0x005b42f0
void __cdecl BattleFx_MagicLoadResBarrier(void)
{
    GfxModelTableBank *baseOffset;
    GfxResourceRequest requestRecord;
    int kind;
    char pathEnd[200];
    GfxResourceRequest *request;
    int resultDefCatalog;

    if (g_pBattle3dScene != 0 && g_BattleMagicBarrier_ResourceHandle == 0) {
        kind = 4;
        resultDefCatalog = (int)&g_BattleMagicBarrier_ResourceDefs;
        baseOffset = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestRecord;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x008fc5f0
        request->resourcePath = (char *)BattleFx_BuildResourcePath("MAGIC/BARIA", pathEnd);
        Gfx_LoadModelTableIfAbsent(kind, resultDefCatalog, request, &g_BattleMagicBarrier_ResourceHandle, baseOffset);
    }
}

/* Entry point for the Barrier magic effect. */
// FUNCTION: FF7 0x005b4380
void __cdecl BattleFx_MagicEnterBarrier(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResBarrier();
    BattleFx_MagicBeginBarrier(targetMask, actorIdx);
}

/* Initializes Barrier effect parameters and spawns per-target callback. */
// FUNCTION: FF7 0x005b439a
void __cdecl BattleFx_MagicBeginBarrier(int targetMask, int actorIdx)
{
    g_BattleMagicBarrier_Scale = 0x3000 /* 3.0 */;
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_MagicSpawnBarrierPerTarget);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x5E);
}

/* Spawns the timeline task for Barrier on a target actor. */
// FUNCTION: FF7 0x005b43d9
void __cdecl BattleFx_MagicSpawnBarrierPerTarget(int actor)
{
    BattleFxBarrierSpawnSlot *effectSlot;

    effectSlot = (BattleFxBarrierSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBarrierTimelineTask);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &effectSlot->posX);
    effectSlot->posX -= (short)(SinFixed(g_BattleActorRecords[actor].yaw) *
                                             g_BattleActorRecords[actor].radius >>
                                         12);
    effectSlot->posZ -= (short)(CosFixed(g_BattleActorRecords[actor].yaw) *
                                               g_BattleActorRecords[actor].radius >>
                                           12);
    effectSlot->rot = *(Int64Pair *)&g_BattleActorRecords[actor].rotX;
    effectSlot->actorIndex = actor;
}

/* Manages the timeline and spawns model tasks for Barrier. */
// FUNCTION: FF7 0x005b44c8
void __cdecl BattleFx_MagicBarrierTimelineTask(void)
{
    BattleFxBarrierTimeline *timeline;
    BattleFxBarrierState *child;

    timeline = (BattleFxBarrierTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (timeline->phase == 0) {
        child = (BattleFxBarrierState *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBarrierGrowFadeModelTask);
        child->phase = timeline->phase;
        child->mode = 0;
        child->rot = timeline->rot;
        child->pos = timeline->pos;
    }
    if (timeline->phase == 2) {
        child = (BattleFxBarrierState *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBarrierGrowFadeModelTask);
        child->phase = timeline->phase;
        child->mode = 1;
        child->rot = timeline->rot;
        child->pos = timeline->pos;
    }
    if (timeline->phase == 4) {
        child = (BattleFxBarrierState *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBarrierGrowFadeModelTask);
        child->phase = timeline->phase;
        child->mode = 3;
        child->rot = timeline->rot;
        child->pos = timeline->pos;
    }
    if (timeline->phase == 6) {
        child = (BattleFxBarrierState *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBarrierGrowFadeModelTask);
        child->phase = timeline->phase;
        child->mode = 2;
        child->rot = timeline->rot;
        child->pos = timeline->pos;
    }
    if (timeline->phase == 1) {
        child = (BattleFxBarrierState *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBarrierDualGrowFadeModelTask);
        child->phase = timeline->phase;
        child->mode = 0;
        child->rot = timeline->rot;
        child->pos = timeline->pos;
    }
    if (timeline->phase == 3) {
        child = (BattleFxBarrierState *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBarrierDualGrowFadeModelTask);
        child->phase = timeline->phase;
        child->mode = 1;
        child->rot = timeline->rot;
        child->pos = timeline->pos;
    }
    if (timeline->phase == 5) {
        child = (BattleFxBarrierState *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBarrierDualGrowFadeModelTask);
        child->phase = timeline->phase;
        child->mode = 3;
        child->rot = timeline->rot;
        child->pos = timeline->pos;
    }
    if (timeline->phase == 7) {
        child = (BattleFxBarrierState *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBarrierDualGrowFadeModelTask);
        child->phase = timeline->phase;
        child->mode = 2;
        child->rot = timeline->rot;
        child->pos = timeline->pos;
    }
    if (timeline->phase == 0x11) {
        Battle_PostActorHitOrHurtMessage(timeline->allocType);
        timeline->ownerSlot = BATTLE_TASK_FREE;
    }
    (timeline->phase)++;
}

/* Updates and renders a growing and fading Barrier model. */
// FUNCTION: FF7 0x005b483a
void __cdecl BattleFx_MagicBarrierGrowFadeModelTask(void)
{
    BattleFxBarrierScaleHistory *pScales;
    BattleFxBarrierWork *work;
    BattleFxMagicBarrierGrowFadeModel *taskSlot;
    int t;
    int bits;
    int fade;

    taskSlot = (BattleFxMagicBarrierGrowFadeModel *)BATTLE_CURRENT_TASK_SLOT;
    work = (BattleFxBarrierWork *)Gfx_GetPsxScratchpadPtr(0);
    pScales = &work->scale;
    t = taskSlot->frameCount + taskSlot->frameOffset + -0x11;
    if (t < 0) {
        pScales->scaleNext = g_BattleMagicBarrier_Scale * 0xC00 /* 0.75 */ >> 12;
        pScales->scaleCur = pScales->scaleNext;
        pScales->scalePrev = pScales->scaleCur;
        fade = 0;
        bits = taskSlot->flags;
        g_BattleMagicBarrier_ModelReq1.modelHandle = Gfx_ModelTableGetRenderObject(0, g_BattleMagicBarrier_ResourceHandle);
    } else {
        if (t >= 8) {
            taskSlot->frameOffset = 0xFFFF;
            return;
        }
        bits = taskSlot->flags | 8;
        pScales->scaleNext = ((t * 0xC00 / 8) + 0xC00) * g_BattleMagicBarrier_Scale >> 12;
        pScales->scaleCur = pScales->scaleNext;
        pScales->scalePrev = pScales->scaleCur;
        fade = (t << 12) / 8;
        g_BattleMagicBarrier_ModelReq1.modelHandle = Gfx_ModelTableGetRenderObject(2, g_BattleMagicBarrier_ResourceHandle);
    }
    SetDrawColorRGB(0, 0, 0);
    RotMatrix(&taskSlot->rot, work);
    ScaleMatrix(work, pScales);
    ApplyMatrixToSVectorInt32(work, &g_BattleMagicBarrier_ModelReq1.offset, &work->matrix.t[0]);
    work->matrix.t[0] += taskSlot->deltaX;
    work->matrix.t[1] += taskSlot->deltaY;
    work->matrix.t[2] += taskSlot->deltaZ;
    MultiplyMatrix(&g_BattleCameraTransform, work, work);
    SetRotMatrix(&work->matrix);
    SetTransMatrix(work);
    g_BattleMagicBarrier_ModelReq1.flags = bits | 0x80;
    g_BattleMagicBarrier_ModelReq1.alpha = fade;
    g_BattleMagicBarrier_ChainTail = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicBarrier_ModelReq1.modelHandle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicBarrier_ChainTail);
    if (g_BattleIsPaused == 0) {
        (taskSlot->frameCount)++;
    }
}

/* Updates and renders dual growing and fading Barrier models. */
// FUNCTION: FF7 0x005b4a76
void __cdecl BattleFx_MagicBarrierDualGrowFadeModelTask(void)
{
    struct {
        int alpha;
        int flags;
        PsxMatrix *copy;
        PsxMatrix *matrix;
        BattleFxBarrierDualScale *scale2;
        BattleFxMagicBarrierGrowFadeModel *slot;
        int time;
        BattleFxBarrierDualScale *scale1;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.slot = (BattleFxMagicBarrierGrowFadeModel *)BATTLE_CURRENT_TASK_SLOT;
    frame.matrix = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    frame.copy = frame.matrix + 1;
    frame.scale1 = (BattleFxBarrierDualScale *)(frame.copy + 1);
    frame.scale2 = frame.scale1 + 1;
    frame.time = frame.slot->frameCount + frame.slot->frameOffset - 17;
    if (frame.time < 0) {
        if (frame.slot->frameCount < 6) {
            frame.scale1->scaleNext = (frame.slot->frameCount * 0xc00 / 6) * g_BattleMagicBarrier_Scale >> 12;
            frame.scale1->scaleCur = frame.scale1->scaleNext;
            frame.scale1->scalePrev = frame.scale1->scaleCur;
        } else {
            frame.scale1->scaleNext = g_BattleMagicBarrier_Scale * 0xc00 /* 0.75 */ >> 12;
            frame.scale1->scaleCur = frame.scale1->scaleNext;
            frame.scale1->scalePrev = frame.scale1->scaleCur;
        }
        frame.scale2->scaleNext = g_BattleMagicBarrier_Scale * 0xc00 /* 0.75 */ >> 12;
        frame.scale2->scaleCur = frame.scale2->scaleNext;
        frame.scale2->scalePrev = frame.scale2->scaleCur;
        frame.flags = frame.slot->flags;
        frame.alpha = 0;
        g_BattleMagicBarrier_ModelReq2.modelHandle = Gfx_ModelTableGetRenderObject(1, g_BattleMagicBarrier_ResourceHandle);
    } else {
        if (frame.time >= 8) {
            frame.slot->frameOffset = -1;
            return;
        }
        frame.flags = frame.slot->flags | 8;
        frame.scale2->scaleNext = ((frame.time * 0xc00 / 8) + 0xc00) * g_BattleMagicBarrier_Scale >> 12;
        frame.scale2->scaleCur = frame.scale2->scaleNext;
        frame.scale2->scalePrev = frame.scale2->scaleCur;
        frame.scale1->scaleNext = frame.scale2->scalePrev;
        frame.scale1->scaleCur = frame.scale1->scaleNext;
        frame.scale1->scalePrev = frame.scale1->scaleCur;
        frame.alpha = (frame.time << 12) / 8;
        g_BattleMagicBarrier_ModelReq2.modelHandle = Gfx_ModelTableGetRenderObject(3, g_BattleMagicBarrier_ResourceHandle);
    }
    SetDrawColorRGB(0, 0, 0);
    RotMatrix(&frame.slot->rot, frame.matrix);
    *frame.copy = *frame.matrix;
    ScaleMatrix(frame.matrix, frame.scale1);
    ScaleMatrix(frame.copy, frame.scale2);
    ApplyMatrixToSVectorInt32(frame.copy, (SVECTOR *)&g_BattleMagicBarrier_OffsetVector, &frame.matrix->t[0]);
    frame.matrix->t[0] += frame.slot->deltaX;
    frame.matrix->t[1] += frame.slot->deltaY;
    frame.matrix->t[2] += frame.slot->deltaZ;
    MultiplyMatrix(&g_BattleCameraTransform, frame.matrix, frame.matrix);
    SetRotMatrix(frame.matrix);
    SetTransMatrix(frame.matrix);
    g_BattleMagicBarrier_ModelReq2.flags = frame.flags | 0x80;
    g_BattleMagicBarrier_ModelReq2.alpha = frame.alpha;
    g_BattleMagicBarrier_ChainTail = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicBarrier_ModelReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 12, g_BattleMagicBarrier_ChainTail);
    if (g_BattleIsPaused == 0)
        ++frame.slot->frameCount;
}
} /* extern "C" */
