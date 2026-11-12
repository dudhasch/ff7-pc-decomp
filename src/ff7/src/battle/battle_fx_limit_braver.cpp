// src/ff7/src/battle/battle_fx_limit_braver.cpp
// Limit Break [0] (Cloud Lv 1-1): Braver (Japanese: ブレイバー, dev tag: "LIMIT/BLAVER" / Blaver)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Blaver limit-break effect: a leap attack with its per-target timeline, a slash flash on
//   the actor, a descending blade trail and a ground impact shockwave.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_limit.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle_3d.h"
#include <stdlib.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/api/sound.h"
#include "lib/codegen.h"
extern "C" {

struct GfxPsxSpriteQuadState;

#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x00bda468
int g_BattleLimitBraver_DrawListHandle = 0;
unsigned char g_bssPad_00bda46c[4] = {0};
// g_BattleLimitBraver_RibbonPoints.pointWord2, g_BattleLimitBraver_RibbonPoints.pointWord6 are
// indexed by the code as fields of records. The span runs to the next named definition of the TU;
// the TU's pieces inside 0xbda470..0xbda570 are aliases.
// GLOBAL: FF7 0x00bda470
struct BattleLimitBraver_RibbonPoints g_BattleLimitBraver_RibbonPoints = {0};
// GLOBAL: FF7 0x00bda570
int g_BattleLimitBraver_LoadedRes = 0;
// GLOBAL: FF7 0x00bda574
struct GfxTableCursor *g_BattleLimitBraver_TargetPosition = 0;
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008f34b8
int g_BattleLimitBraver_ResourceDefs = 76345;
int g_BattleLimitBraver_ResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxLimitBraverResourceNames + 0x0)
};
int g_BattleLimitBraver_ResourceDefsDataTail[14] = {
    (int)((char *)&g_BattleFxLimitBraverResourceNames + 0x10), 0x0, 0x0, 0x12a39, (int)((char *)&g_BattleFxLimitBraverResourceNames + 0x1c), (int)((char *)&g_BattleFxLimitBraverResourceNames + 0x2c),
    0x0, 0x0, 0x13839, (int)((char *)&g_BattleFxLimitBraverResourceNames + 0x38), 0x0, 0x0,
    0x0, 0x0
};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008f34f8
struct BattleLimitBraver_SpriteReq1 g_BattleLimitBraver_SpriteReq1 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}, 0};
unsigned char g_rawData_008f350c[4] = {0x70, 0xfe, 0x00, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008f3510
struct BattleLimitBraver_TrailColors g_BattleLimitBraver_TrailColors = {{0, 0x40, 0, 0, 0, 0, 0, 0, 0, 0x40, 0, 0, 0, 0, 0, 0, 0, 0x40}, 0, {0}};
// GLOBAL: FF7 0x008f3538
unsigned char g_BattleLimitBraver_SpriteReq2[8] = {0x00, 0x00, 0x00, 0x00, 0xe0, 0xfc, 0x00, 0x00};
unsigned char g_rawData_008f3540[16] = {0x00, 0x00, 0x0c, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0xfe, 0xe0, 0xfc, 0x00, 0x00};
// The 16-byte sprite record the code fills and submits by number.
// GLOBAL: FF7 0x008f3550
unsigned char g_BattleLimitBraver_BillboardReq[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x2e, 0x00, 0x00, 0x00, 0x00};
unsigned char g_BattleFxLimitBraverResourceNames[72] =
    "BLAVER00.TIM\0\0\0\0"
    "BLAVER.SPT\0\0"
    "BLAVER01.TIM\0\0\0\0"
    "SHINING.SPT\0"
    "BLAVER01.TIM";
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/*
 * Loads resources for the Blaver limit effect when not already loaded.
 */
// FUNCTION: FF7 0x005aec40
void __cdecl BattleFx_LimitLoadResBraver(void)
{
    GfxModelTableBank *stage;
    GfxResourceRequest requestSlot;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceTable;

    if (g_pBattle3dScene != 0 && g_BattleLimitBraver_TargetPosition == 0) {
        kind = 3;
        resourceTable = (int)&g_BattleLimitBraver_ResourceDefs;
        stage = (GfxModelTableBank *)g_pBattle3dScene->modelTableBank;
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        ((BattleFxDiamondCardResource *)pRequest)->groupTag = 0;
        // STRING: FF7 0x008f35a8
        pRequest->resourcePath = BattleFx_BuildResourcePath("LIMIT/BLAVER", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceTable, pRequest, &g_BattleLimitBraver_TargetPosition, stage);
    }
}

/*
 * Entry point registering per-target callback for Blaver.
 */
// FUNCTION: FF7 0x005aecd0
int __cdecl Battle_LimitFx00_Braver(int targetMask, int actorIndex)
{
    g_BattleLimitBraver_LoadedRes = 0;
    g_BattleLimitBraver_DrawListHandle = (int)&g_GfxPsxPacketScratch;
    BattleFx_LimitLoadResBraver();
    BattleFx_LimitRegisterBraverPerTargetCallback(targetMask, actorIndex);
    return 0;
}

/*
 * Registers per-target callback for Blaver.
 */
// FUNCTION: FF7 0x005aed00
void __cdecl BattleFx_LimitRegisterBraverPerTargetCallback(int targetMask, int actorIndex)
{
    Battle_CreateBitPatternCallbackTask(targetMask, actorIndex, 0, BattleFx_LimitSpawnBraverPerTarget);
}

/*
 * Spawns timeline task for a single Blaver target.
 */
// FUNCTION: FF7 0x005aed1c
void __cdecl BattleFx_LimitSpawnBraverPerTarget(int actor, int value)
{
    BattleFxBigShotTask *task;

    task = (BattleFxBigShotTask *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitBraverTimelineTask);
    task->targetIndex = actor;
    task->sourceActor = value;
}

/*
 * Drives the main timeline and leap attack sequence for Blaver.
 */
// FUNCTION: FF7 0x005aed55
void __cdecl BattleFx_LimitBraverTimelineTask(void)
{
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxBraverTimeline *task;
    BattleLimitBraverTaskSlot *child;

    task = (BattleFxBraverTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->phase == 0) {
        SfxLoadIntoBuffer(0x1B, 0);
    }
    if (task->phase == 0xA) {
        child = (BattleLimitBraverTaskSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitBraverActorMatrixSpriteTask);
        child->radius = task->radius;
    }
    if (task->phase == 0x11) {
        child = (BattleLimitBraverTaskSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitBraverTrailRibbonTask);
        child->radius = task->radius;
    }
    if (task->phase == 0x1A) {
        child = (BattleLimitBraverTaskSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitBraverBillboardSpriteTask);
        Battle_GetBoneWorldPos(task->actorSlot, g_BattleActorRecords[task->actorSlot].targetBone, (short *)((int)child + 8));
        child->negRadius = -g_BattleActorRecords[task->actorSlot].radius;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition((char *)child + 8), 0x1B);
        Battle_PostActorHitOrHurtMessage(task->actorSlot);
        BattleFx_StartScreenFlashRamp(1);
    }
    if (task->phase == 0x1B) {
        BattleFx_FadeOutScreenFlashRamp(1);
    }
    if (task->phase == 0x1B) {
        task->ownerSlot = BATTLE_TASK_FREE;
        Sfx_ReleaseCachedBuffer(0x1B, 0xF);
    }
    (task->phase)++;
}

/*
 * Updates and renders slash impact flash anchored to actor for Blaver.
 */
// FUNCTION: FF7 0x005aeef5
void __cdecl BattleFx_LimitBraverActorMatrixSpriteTask(void)
{
    BattleFxBraverActorSprite *taskPtr;
    int tmp;

    taskPtr = (BattleFxBraverActorSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitBraver_SpriteReq1.resource = Gfx_ModelTableGetSpriteObject(1, g_BattleLimitBraver_TargetPosition);
    (g_BattleLimitBraver_SpriteReq1.handle) = Gfx_ModelTableGetLoadedRecord(1, g_BattleLimitBraver_TargetPosition);
    if ((g_BattleLimitBraver_SpriteReq1.handle) != 0) {
        g_BattleLimitBraver_SpriteReq1.frameIndex = taskPtr->phase >> 8;
        if (*(unsigned short *)&g_BattleLimitBraver_SpriteReq1.frameIndex >= ((BattleFxSpriteGroup *)g_BattleLimitBraver_SpriteReq1.handle)->header->frameCount) {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    } else {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
        return;
    }
    SetRotMatrix((void *)((taskPtr->matrixIndex << 6) + (char *)g_BattleActorTransformRecords[0].auxNode));
    SetTransMatrix((void *)((taskPtr->matrixIndex << 6) + (char *)g_BattleActorTransformRecords[0].auxNode));
    RotTransPoint(&g_BattleLimitBraver_SpriteReq1.vector, (int *)&g_BattleLimitBraver_TrailColors.vector, &tmp);
    SetRotMatrix((int *)&g_BattleLimitBraver_TrailColors);
    SetTransMatrix((int *)&g_BattleLimitBraver_TrailColors);
    g_BattleLimitBraver_DrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleLimitBraver_SpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitBraver_DrawListHandle);
    if (g_BattleIsPaused == 0) {
        taskPtr->phase += 0x100;
    }
}

/*
 * Updates and renders descending blade trail ribbon for Blaver.
 */
// FUNCTION: FF7 0x005af02d
void __cdecl BattleFx_LimitBraverTrailRibbonTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        BattleFxBraverTrailPacket *trailPacket;
        struct { unsigned char pad_to_4[0x4]; int quadDepth; };
        struct { unsigned char pad_to_C[0xC]; int composedMatrix; };
        struct { unsigned char pad_to_1E[0x1E]; short pointWord0; };
        struct { unsigned char pad_to_22[0x22]; short pointWord1; };
        struct { unsigned char pad_to_26[0x26]; short pointWord2; };
        struct { unsigned char pad_to_2C[0x2C]; int prevIndex; };
        struct { unsigned char pad_to_34[0x34]; int curIndex; };
        struct { unsigned char pad_to_38[0x38]; short screenCoord; };
        struct { unsigned char pad_to_3C[0x3C]; short pointWord5; };
        struct { unsigned char pad_to_40[0x40]; short pointWord6; };
        struct { unsigned char pad_to_4C[0x4C]; int clipFlags; };
        struct { unsigned char pad_to_50[0x50]; BattleBraverTrailRibbonRecord *ribbonRecord; };
        struct { unsigned char pad_to_58[0x58]; int quadAux; };
    } frame;
#pragma pack(pop)

    frame.ribbonRecord = (BattleBraverTrailRibbonRecord *)BATTLE_CURRENT_TASK_SLOT;
    Battle_ComposeMatrixRelativeToCamera((short *)&g_BattleCameraTransform, (void *)((frame.ribbonRecord->actorMatrixIndex << 6) + (int)g_BattleActorTransformRecords[0].auxNode), (PsxMatrix *)&frame.composedMatrix);
    SetRotMatrix(&frame.composedMatrix);
    SetTransMatrix(&frame.composedMatrix);
    *(short *)((char *)&g_BattleLimitBraver_RibbonPoints + (g_BattleLimitBraver_LoadedRes << 4)) = frame.pointWord0;
    *(short *)((char *)&g_BattleLimitBraver_RibbonPoints.pointWord1 + (g_BattleLimitBraver_LoadedRes << 4)) = frame.pointWord1;
    *(short *)((char *)&g_BattleLimitBraver_RibbonPoints.pointWord2 + (g_BattleLimitBraver_LoadedRes << 4)) = frame.pointWord2;
    RotTransPoint((int *)&g_BattleLimitBraver_SpriteReq2, &frame.screenCoord, &frame.clipFlags);
    *(short *)((char *)&g_BattleLimitBraver_RibbonPoints.pointCoord + (g_BattleLimitBraver_LoadedRes << 4)) = frame.screenCoord;
    *(short *)((char *)&g_BattleLimitBraver_RibbonPoints.pointWord5 + (g_BattleLimitBraver_LoadedRes << 4)) = frame.pointWord5;
    *(short *)((char *)&g_BattleLimitBraver_RibbonPoints.pointWord6 + (g_BattleLimitBraver_LoadedRes << 4)) = frame.pointWord6;
    if (frame.ribbonRecord->frame < 20) {
        if (frame.ribbonRecord->frame >= 1) {
            frame.curIndex = g_BattleLimitBraver_LoadedRes & 0xF;
            frame.prevIndex = g_BattleLimitBraver_LoadedRes - 1 & 0xF;
            frame.trailPacket = (BattleFxBraverTrailPacket *)g_BattleLimitBraver_DrawListHandle;
            SetRotMatrix(&g_BattleCameraTransform);
            SetTransMatrix(&g_BattleCameraTransform);
            RefreshGlobalTransformState();
            Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(2, g_BattleLimitBraver_TargetPosition)));
            frame.quadDepth = Gfx_TransformQuadReturnDepth((void *)((frame.curIndex << 4) + (char *)&g_BattleLimitBraver_RibbonPoints), (SVECTOR *)((frame.curIndex << 4) + (char *)&g_BattleLimitBraver_RibbonPoints.pointCoord), (SVECTOR *)((frame.prevIndex << 4) + (char *)&g_BattleLimitBraver_RibbonPoints), (SVECTOR *)((frame.prevIndex << 4) + (char *)&g_BattleLimitBraver_RibbonPoints.pointCoord), &frame.quadAux, &frame.clipFlags);
            if ((frame.clipFlags & 0x60000) == 0) {
                frame.trailPacket->tag = 0x9000000;
                frame.trailPacket->rgbCode = 0x2E808080;
                frame.trailPacket->uv0 = 0x5080;
                frame.trailPacket->uv1 = 0x50C0;
                frame.trailPacket->uv2 = 0x80;
                frame.trailPacket->uv3 = 0xC0;
                frame.trailPacket->clut = 0x7888;
                frame.trailPacket->texturePage = 0x2D;
                Gfx_DrawPsxPrimitive(frame.trailPacket);
            }
        }
    } else {
        frame.ribbonRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused == 0) {
        g_BattleLimitBraver_LoadedRes = g_BattleLimitBraver_LoadedRes + 1 & 0xF;
        (frame.ribbonRecord->frame)++;
    }
}

/*
 * Updates and renders ground impact shockwave billboard for Blaver.
 */
// FUNCTION: FF7 0x005af26c
void __cdecl BattleFx_LimitBraverBillboardSpriteTask(void)
{
    int frameIdx;
    BattleFxBraverBillboardTask *task;

    task = (BattleFxBraverBillboardTask *)BATTLE_CURRENT_TASK_SLOT;
    ((BattleFxBraverBillboardReq *)&g_BattleLimitBraver_BillboardReq)->drawObject = Gfx_ModelTableGetSpriteObject(0, g_BattleLimitBraver_TargetPosition);
    ((BattleFxBraverBillboardReq *)&g_BattleLimitBraver_BillboardReq)->resource = Gfx_ModelTableGetLoadedRecord(0, g_BattleLimitBraver_TargetPosition);
    frameIdx = task->frameAccumulator >> 8;
    if (((BattleFxBraverBillboardReq *)&g_BattleLimitBraver_BillboardReq)->resource != 0 && frameIdx < *(int *)((char *)*(int *)((BattleFxBraverBillboardReq *)&g_BattleLimitBraver_BillboardReq)->resource + 4)) {
        ((BattleFxBraverBillboardReq *)&g_BattleLimitBraver_BillboardReq)->frame = frameIdx;
    } else {
        task->ownerSlot = BATTLE_TASK_FREE;
        return;
    }
    BattleFx_InstallBillboardMatrix((void *)((int)task + 8), 0x1000 /* 1.0 */, task->angle);
    g_BattleLimitBraver_DrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleLimitBraver_BillboardReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitBraver_DrawListHandle);
    if (g_BattleIsPaused == 0) {
        task->frameAccumulator += 256;
    }
}
} /* extern "C" */
