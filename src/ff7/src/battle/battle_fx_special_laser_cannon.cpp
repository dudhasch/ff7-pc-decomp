// src/ff7/src/battle/battle_fx_special_laser_cannon.cpp
/* Enemy Special Effect [idx 4]: LaserCannon (JP: reza1 / レーザー1 (Laser 1), asset: "SPECIAL/REZA1") — Laser Cannon (0x117, Sweeper), Mono Laser (0x14b, Mono Drive), Paralaser (0x169), Laser (0x36c, Jenova*BIRTH) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Reza1 attack: loads resources, spawns per-target
//   timelines, and drives a firing laser beam with stretching ribbon geometry, cycling impact
//   flares and sequential burst sprites.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "lib/types/graphics.h"
#include <stdlib.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/codegen.h"
extern "C" {

struct GfxPsxSpriteQuadState;

#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x008f3200
int g_BattleSpecLaserCannon_ResourceDefs = 0x00192A39;
int g_BattleSpecLaserCannon_ResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialLaserCannonResourceNames + 0x0)
};
int g_BattleSpecLaserCannon_ResourceDefsDataTail[14] = {
    (int)((char *)&g_BattleFxSpecialLaserCannonResourceNames + 0xc), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialLaserCannonResourceNames + 0x18), (int)((char *)&g_BattleFxSpecialLaserCannonResourceNames + 0x24),
    0x0, 0x0, 0x197a39, (int)((char *)&g_BattleFxSpecialLaserCannonResourceNames + 0x30), 0x0, 0x0,
    0x0, 0x0
};
// clang-format on

// clang-format off
// The first 16-byte sprite record the code fills and submits by number.
// GLOBAL: FF7 0x008f3240
struct BattleFxSpriteRequest g_BattleSpecLaserCannon_SpriteReq1 = {0, 0, -128, -128, -128, 0x2e, 0, 0};
// The second 16-byte sprite record the code fills and submits by number.
// GLOBAL: FF7 0x008f3250
struct BattleFxSpriteRequest g_BattleSpecLaserCannon_SpriteReq2 = {0, 0, -128, -128, -128, 0x2c, 0, 0};
unsigned char g_BattleFxSpecialLaserCannonResourceNames[60] =
    "LASER01.TIM\0"
    "MARU.SPT\0\0\0\0"
    "LASER00.TIM\0"
    "HIT1.SPT\0\0\0\0"
    "LASER02.TIM";
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00bd9dd8
int g_BattleSpecLaserCannon_DrawListHandle = 0;
// GLOBAL: FF7 0x00bd9ddc
struct GfxTableCursor *g_BattleSpecLaserCannon_LoadedRes = 0;
unsigned char g_bssPad_00bd9de0[32] = {0};
// clang-format on

/*
 * Loads resources for the Reza1 special effect when not already loaded.
 */
// FUNCTION: FF7 0x005acef0
void __cdecl BattleFx_SpecialLoadResLaserCannon(void)
{
    GfxModelTableBank *stage;
    GfxResourceRequest requestSlot;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceTable;

    if (g_pBattle3dScene != 0 && g_BattleSpecLaserCannon_LoadedRes == 0) {
        kind = 3;
        resourceTable = (int)&g_BattleSpecLaserCannon_ResourceDefs;
        stage = (GfxModelTableBank *)g_pBattle3dScene->modelTableBank;
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        ((BattleFxDiamondCardResource *)pRequest)->groupTag = 0;
        // STRING: FF7 0x008f329c
        ((BattleFxDiamondCardResource *)pRequest)->resourcePath = (int)BattleFx_BuildResourcePath("SPECIAL/REZA1", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceTable, pRequest, &g_BattleSpecLaserCannon_LoadedRes, stage);
    }
}

/*
 * Main entry point initializing render state for Reza1.
 */
// FUNCTION: FF7 0x005acf80
void __cdecl BattleFx_SpecialEnterLaserCannon(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResLaserCannon();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 1, BattleFx_SpecialSpawnLaserCannonPerTarget);
}

/*
 * Spawns timeline task for a single Reza1 target.
 */
// FUNCTION: FF7 0x005acfa1
void __cdecl BattleFx_SpecialSpawnLaserCannonPerTarget(int actor, int secondIndex)
{
    BattleFxLaserCannonTimeline *taskRecord;

    taskRecord = (BattleFxLaserCannonTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialLaserCannonTimelineTask);
    taskRecord->actorIndex = actor;
    taskRecord->secondIndex = secondIndex;
}

/*
 * Drives the main timeline and laser beam firing for Reza1.
 */
// FUNCTION: FF7 0x005acfda
void __cdecl BattleFx_SpecialLaserCannonTimelineTask(void)
{
    int padSlot[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int unusedSpace[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxLaserCannonTimelineRecord *taskPtr;
    short posY;
    short coordZ;
    int stackWords[8]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxLaserCannonChild *pChild;
    int padWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short coordinateX;

    taskPtr = (BattleFxLaserCannonTimelineRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->frame == 3) {
        pChild = (BattleFxLaserCannonChild *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialLaserCannonRibbonTask);
        Battle_GetBoneWorldPos(taskPtr->actorB, g_BattleActorRecords[taskPtr->actorB].bone34, pChild->firstPos);
        Battle_GetBoneWorldPos(taskPtr->actorA, g_BattleActorRecords[taskPtr->actorA].targetBone, pChild->secondPos);
        pChild = (BattleFxLaserCannonChild *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialLaserCannonCyclingSpriteTask);
        pChild->firstPos[0] = coordinateX;
        pChild->firstPos[1] = posY;
        pChild->firstPos[2] = coordZ;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, (int)pChild->secondPos, 0x3A);
    } else if (taskPtr->frame == 7) {
        pChild = (BattleFxLaserCannonChild *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialLaserCannonSequentialSpriteTask);
        Battle_GetBoneWorldPos(taskPtr->actorA, g_BattleActorRecords[taskPtr->actorA].targetBone, pChild->firstPos);
        Battle_PostActorHitOrHurtMessage(taskPtr->actorA);
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
    (taskPtr->frame)++;
}

/*
 * Updates and renders stretching laser ribbon geometry for Reza1.
 */
// FUNCTION: FF7 0x005ad15a
void __cdecl BattleFx_SpecialLaserCannonRibbonTask(void)
{
    struct {
        int resource;
        int depth;
        BattleFxLaserCannonRibbonQuad *primitive;
        int unused0[4];
        float secondZ;
        float secondW;
        int unused1[2];
        float secondBlendZ;
        float secondBlendW;
        short secondXY[2];
        int unused2[2];
        float firstZ;
        float firstW;
        short firstXY[2];
        int scale;
        int unused3[2];
        float firstBlendZ;
        float firstBlendW;
        int flags;
        BattleFxLaserCannonChild *slot;
        int vector[3];
        int unused4;
        int projected;
        int unused5;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = (BattleFxLaserCannonChild *)BATTLE_CURRENT_TASK_SLOT;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    frame.depth = ProjectPointHardware(frame.slot->firstPos, frame.firstXY, &frame.projected, &frame.flags);
    frame.firstZ = GetLastProjectedNdcZ();
    frame.firstBlendZ = frame.firstZ;
    frame.firstW = GetLastProjectedNdcW();
    frame.firstBlendW = frame.firstW;
    frame.depth += ProjectPointHardware(frame.slot->secondPos, frame.secondXY, &frame.projected, &frame.flags);
    frame.secondZ = GetLastProjectedNdcZ();
    frame.secondBlendZ = frame.secondZ;
    frame.secondW = GetLastProjectedNdcW();
    frame.secondBlendW = frame.secondW;
    frame.depth /= 2;
    if (frame.depth > 0) {
        frame.primitive = (BattleFxLaserCannonRibbonQuad *)Gfx_GetPsxPacketScratchPtr();
        frame.primitive->primCode |= 2;
        frame.primitive->command = (frame.primitive->command & 0xFFFFFF) | 0x09000000;
        frame.primitive->primCode = 0x2C;
        frame.vector[0] = frame.secondXY[0] - frame.firstXY[0];
        frame.vector[1] = frame.secondXY[1] - frame.firstXY[1];
        frame.vector[2] = 0;
        NormalizeVectorFixed((VECTOR *)frame.vector, (VECTOR *)frame.vector);
        frame.scale = GetGeomScreen();
        frame.vector[0] = frame.vector[0] * frame.scale / frame.depth >> 6;
        frame.vector[1] = frame.vector[1] * frame.scale / frame.depth >> 6;
        frame.primitive->command = 0x09000000;
        frame.primitive->colorCmd = 0x2E808080;
        frame.primitive->x0 = frame.firstXY[0] + frame.vector[1];
        frame.primitive->y0 = frame.firstXY[1] - frame.vector[0];
        frame.primitive->x1 = frame.firstXY[0] - frame.vector[1];
        frame.primitive->y1 = frame.firstXY[1] + frame.vector[0];
        frame.primitive->x2 = frame.secondXY[0] + frame.vector[1];
        frame.primitive->y2 = frame.secondXY[1] - frame.vector[0];
        frame.primitive->x3 = frame.secondXY[0] - frame.vector[1];
        frame.primitive->y3 = frame.secondXY[1] + frame.vector[0];
        frame.primitive->u1 = 0;
        frame.primitive->u0 = 0;
        frame.primitive->u3 = -1;
        frame.primitive->u2 = -1;
        frame.primitive->v2 = frame.slot->frame << 5;
        frame.primitive->v0 = frame.primitive->v2;
        frame.primitive->v3 = (frame.slot->frame << 5) + 31;
        frame.primitive->v1 = frame.primitive->v3;
        frame.primitive->texPage = 0x2E;
        frame.primitive->clut = 0x7808;
        frame.resource = Gfx_ModelTableGetSpriteObject(2, g_BattleSpecLaserCannon_LoadedRes);
        if (frame.resource != 0)
            Gfx_SetPsxPrimQuadState(11, 1, (GfxPsxSpriteQuadState *)frame.resource);
        Gfx_SetBlendFactorQuad(frame.firstBlendZ, frame.firstBlendW, frame.secondBlendZ, frame.secondBlendW,
                               frame.firstZ, frame.firstW, frame.secondZ, frame.secondW);
        Gfx_DrawPsxPrimitive(frame.primitive);
    }
    if (g_BattleIsPaused == 0) {
        if ((frame.slot->frame)++ == 7)
            frame.slot->ownerSlot = -1;
    }
}

/*
 * Updates and renders animated impact flare sprites for Reza1.
 */
// FUNCTION: FF7 0x005ad428
void __cdecl BattleFx_SpecialLaserCannonCyclingSpriteTask(void)
{
    BattleFxLaserCannonChild *task;

    task = (BattleFxLaserCannonChild *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecLaserCannon_SpriteReq1.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecLaserCannon_LoadedRes);
    g_BattleSpecLaserCannon_SpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecLaserCannon_LoadedRes);
    BattleFx_InstallBillboardMatrix(task->firstPos, 0x1000, 0xFFFFFF38);
    g_BattleSpecLaserCannon_SpriteReq1.frame = task->frame % 3;
    g_BattleSpecLaserCannon_DrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecLaserCannon_SpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecLaserCannon_DrawListHandle);
    if (g_BattleIsPaused == 0) {
        if ((task->frame)++ == 7) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Updates and renders sequential burst sprites for Reza1.
 */
// FUNCTION: FF7 0x005ad4f3
void __cdecl BattleFx_SpecialLaserCannonSequentialSpriteTask(void)
{
    BattleFxLaserCannonChild *task;

    task = (BattleFxLaserCannonChild *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecLaserCannon_SpriteReq2.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecLaserCannon_LoadedRes);
    g_BattleSpecLaserCannon_SpriteReq2.drawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecLaserCannon_LoadedRes);
    BattleFx_InstallBillboardMatrix(task->firstPos, 0x1000, 0xFFFFFF38);
    g_BattleSpecLaserCannon_SpriteReq2.frame = task->frame;
    g_BattleSpecLaserCannon_DrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecLaserCannon_SpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecLaserCannon_DrawListHandle);
    if (g_BattleIsPaused == 0) {
        if ((task->frame)++ == 7) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
