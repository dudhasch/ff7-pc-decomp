// src/ff7/src/battle/battle_fx_special_searchlight.cpp
/* Enemy Special Effect [idx 17]: Searchlight (JP: sarchi / サーチ (Search), asset: "SPECIAL/SARCHI") — Search (0x328, XCannon) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Sarchi attack: loads resources, spawns per-target
//   timelines, and drives a sweeping searchlight beam model with trailing particle sprites.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle_3d.h"
#include <string.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x008a8860
int g_BattleSpecialSearchlightResourceDefs = 0x001900B1;
char *g_BattleSpecialSearchlightResourceDefModelName[1] = {
    // STRING: FF7 0x008a88d0
    "sarchi.rsd"
};
unsigned int g_BattleSpecialSearchlightResourceDefEntries[18] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a88dc
    (unsigned int)"sarchi2.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a88e8
    (unsigned int)"hit1.tim",
    // STRING: FF7 0x008a88f4
    (unsigned int)"hit11.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a8900
    (unsigned int)"hit1.tim",
    // STRING: FF7 0x008a890c
    (unsigned int)"hit12.spt",
    0x00000000,
    0x00000000
};
#undef g_BattleSpecialSearchlightSpriteData
// GLOBAL: FF7 0x008a88b0
unsigned char g_BattleSpecialSearchlightSpriteData[28] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x2c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};
unsigned char g_BattleSpecialSearchlightSpriteDataTail[4] = {0x20, 0x00, 0x00, 0x00};
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b72f60
PsxMatrix g_BattleSpecialSearchlightBasisMatrix = {0};
// GLOBAL: FF7 0x00b72f80
short g_BattleSpecialSearchlightDeltaVector[3] = {0};
// GLOBAL: FF7 0x00b72f88
int g_BattleSpecialSearchlightDrawListHandle = 0;
unsigned char g_bssPad_00b72f8c[4] = {0};
unsigned char g_bssPad_00b72f90[8] = {0};
// GLOBAL: FF7 0x00b72f98
struct GfxTableCursor *g_BattleSpecialSearchlightLoadedRes = 0;
// GLOBAL: FF7 0x00b72f9c
short g_BattleSpecialSearchlightStep = 0;
// clang-format on

/*
 * Loads resources for the Sarchi special effect when not already loaded.
 */
// FUNCTION: FF7 0x0055e2e0
void __cdecl BattleFx_SpecialLoadResSearchlight(void)
{
    GfxModelTableBank *ownerHandle;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecialSearchlightLoadedRes == 0) {
        kind = 4;
        resourceDefsWord = (int)&g_BattleSpecialSearchlightResourceDefs;
        ownerHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a8918
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/SARCHI", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecialSearchlightLoadedRes, ownerHandle);
    }
}

/*
 * Main entry point for the Sarchi special effect.
 */
// FUNCTION: FF7 0x0055e370
void __cdecl BattleFx_SpecialEnterSearchlight(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResSearchlight();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnSearchlightPerTarget);
    g_BattleFxTimelineState.channel2 = 0;
    g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
    g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    Battle_StartFixedPointRamp(0x400, 0xA);
    Battle_PlaySfxPannedToActor(0x62, (short)actorIdx, 0x40);
}

/*
 * Spawns timeline task for a single Sarchi target.
 */
// FUNCTION: FF7 0x0055e3ce
void __cdecl BattleFx_SpecialSpawnSearchlightPerTarget(int actor, int casterIndex)
{
    struct {
        short direction[4];
        BattleFxSearchlightTimelineState *slot;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.slot = (BattleFxSearchlightTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSearchlightTimelineTask);
    Battle_GetBoneWorldPos(casterIndex, g_BattleActorRecords[casterIndex].bone34, &local.slot->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &local.slot->endX);
    local.direction[0] = local.slot->endX - local.slot->startX;
    local.direction[1] = local.slot->endY - local.slot->startY;
    local.direction[2] = local.slot->endZ - local.slot->startZ;
    Gte_BuildLookBasisFromDirection(local.direction, (short *)&g_BattleSpecialSearchlightBasisMatrix);
    local.slot->recordType = actor;
}

/*
 * Drives the main timeline and searchlight beam for Sarchi.
 */
// FUNCTION: FF7 0x0055e493
void __cdecl BattleFx_SpecialSearchlightTimelineTask(void)
{
    int padSlot; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short curLength;
    short i8;
    BattleFxSearchlightSweep *subSweep;
    int t; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int y; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int a; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxSearchlightTimelineState *taskPtr;
    int c; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int d; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int e; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int f; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int g; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    taskPtr = (BattleFxSearchlightTimelineState *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->step == 0) {
        g_BattleSpecialSearchlightDeltaVector[0] = taskPtr->endX - taskPtr->startX;
        g_BattleSpecialSearchlightDeltaVector[1] = taskPtr->endY - taskPtr->startY;
        g_BattleSpecialSearchlightDeltaVector[2] = taskPtr->endZ - taskPtr->startZ;
        curLength = SqrtToInt(g_BattleSpecialSearchlightDeltaVector[0] * g_BattleSpecialSearchlightDeltaVector[0] + g_BattleSpecialSearchlightDeltaVector[1] * g_BattleSpecialSearchlightDeltaVector[1] + g_BattleSpecialSearchlightDeltaVector[2] * g_BattleSpecialSearchlightDeltaVector[2]);
        for (i8 = 0; i8 < 4; i8++) {
            subSweep = (BattleFxSearchlightSweep *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSearchlightSweepModelTask);
            subSweep->startX = taskPtr->startX;
            subSweep->startY = taskPtr->startY;
            subSweep->startZ = taskPtr->startZ;
            subSweep->angle = ArcTangentFixed(taskPtr->endX - taskPtr->startX, taskPtr->endZ - taskPtr->startZ);
            subSweep->scaledLength = curLength * 0x1500 / 0xAF0;
            subSweep->variant = i8;
            subSweep->pathLength = curLength;
        }
    }
    if (taskPtr->step == 0x10) {
        Battle_PostActorHitOrHurtMessage(taskPtr->recordType);
    }
    (taskPtr->step)++;
    if (taskPtr->step == 0x28) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
        Battle_StartFixedPointRamp(0, 0xA);
    }
}

/*
 * Updates and renders sweeping searchlight beam models for Sarchi.
 */
// FUNCTION: FF7 0x0055e671
void __cdecl BattleFx_SpecialSearchlightSweepModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[132];
        int spriteFrameWord;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR trailOffsetX; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxSpecialEnergyOrbSlot *trailSlot; };
        struct { unsigned char pad_to_40[0x40]; int trailProjected; };
        struct { unsigned char pad_to_44[0x44]; SVECTOR baseRotX; };
        struct { unsigned char pad_to_4C[0x4C]; BattleFxSearchlightSweepModelSlot *sweepSlot; };
        struct { unsigned char pad_to_70[0x70]; short direction; };
        struct { unsigned char pad_to_74[0x74]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.sweepSlot = (BattleFxSearchlightSweepModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.direction = (frame.sweepSlot->variant >> 1) * 2 - 1;
    frame.rotX.vx = SinFixed(frame.sweepSlot->frameCounter << 8) / 0x28 * frame.direction;
    frame.baseRotX.vx = frame.rotX.vx;
    frame.rotX.vy = (SinFixed(frame.sweepSlot->frameCounter << 7) >> 5) * frame.direction;
    frame.baseRotX.vy = frame.rotX.vy;
    frame.rotX.vz = 0;
    frame.baseRotX.vz = frame.rotX.vz;
    frame.rotX.vx -= 0x400;
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialSearchlightBasisMatrix, &frame.rotMatrix, &frame.rotMatrix);
    frame.scaleVec.vy = frame.sweepSlot->scaledLength;
    frame.scaleVec.vz = ((frame.sweepSlot->variant % 2) != 0 ? 0xA00 : 0) + 0x1400;
    frame.scaleVec.vx = frame.scaleVec.vz;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleVec);
    frame.rotMatrix.t[0] = frame.sweepSlot->posX;
    frame.rotMatrix.t[1] = frame.sweepSlot->posY;
    frame.rotMatrix.t[2] = frame.sweepSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    SetDrawColorRGB(0, 0, 0);
    if ((frame.sweepSlot->variant % 2) == 0) {
        ((BattleFxModelDraw *)(g_BattleSpecialSearchlightSpriteData + 0x10))->modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleSpecialSearchlightLoadedRes);
    } else {
        ((BattleFxModelDraw *)(g_BattleSpecialSearchlightSpriteData + 0x10))->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialSearchlightLoadedRes);
    }
    if ((frame.sweepSlot->variant % 2) == 0) {
        frame.spriteFrameWord = 0x1000 - (g_BattleSpecialSearchlightStep << 5);
    } else {
        frame.spriteFrameWord = 0x1000 - g_BattleSpecialSearchlightStep * 0x18;
    }
    ((BattleFxModelDraw *)(g_BattleSpecialSearchlightSpriteData + 0x10))->fadeLevel = *(short *)&frame.spriteFrameWord;
    g_BattleSpecialSearchlightDrawListHandle = BattleFx_SubmitModelDraw(((BattleFxModelDraw *)(g_BattleSpecialSearchlightSpriteData + 0x10)), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSearchlightDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if ((frame.sweepSlot->variant % 2) == 0) {
        frame.trailSlot = (BattleFxSpecialEnergyOrbSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSearchlightTrailSpriteTask);
        frame.trailSlot->posX = frame.sweepSlot->posX;
        frame.trailSlot->posY = frame.sweepSlot->posY;
        frame.trailSlot->posZ = frame.sweepSlot->posZ;
        frame.trailOffsetX.vx = 0;
        frame.trailOffsetX.vy = 0;
        frame.trailOffsetX.vz = frame.sweepSlot->pathLength;
        RotMatrix(&frame.baseRotX, &frame.rotMatrix);
        ConcatMatrixPreservingTranslation(&g_BattleSpecialSearchlightBasisMatrix, &frame.rotMatrix, &frame.rotMatrix);
        frame.rotMatrix.t[0] = frame.trailSlot->posX;
        frame.rotMatrix.t[1] = frame.trailSlot->posY;
        frame.rotMatrix.t[2] = frame.trailSlot->posZ;
        SetRotMatrix(&frame.rotMatrix);
        SetTransMatrix(&frame.rotMatrix);
        RotTransPointToInt16(&frame.trailOffsetX, &frame.trailSlot->posX, &frame.trailProjected);
        frame.trailSlot->velocityX = 0;
        frame.trailSlot->velocityY = frame.sweepSlot->yaw;
        frame.trailSlot->velocityZ = 0;
    }
    if (frame.sweepSlot->frameCounter < 8) {
        g_BattleSpecialSearchlightStep++;
    }
    if (frame.sweepSlot->frameCounter > 0x18) {
        g_BattleSpecialSearchlightStep--;
    }
    (frame.sweepSlot->frameCounter)++;
    if (frame.sweepSlot->frameCounter == 0x20) {
        frame.sweepSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates and renders trailing searchlight particles for Sarchi.
 */
// FUNCTION: FF7 0x0055ea0a
void __cdecl BattleFx_SpecialSearchlightTrailSpriteTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    ((BattleFxSpriteRequest *)g_BattleSpecialSearchlightSpriteData)->spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(2, g_BattleSpecialSearchlightLoadedRes);
    ((BattleFxSpriteRequest *)g_BattleSpecialSearchlightSpriteData)->drawObj = Gfx_ModelTableGetSpriteObject(2, g_BattleSpecialSearchlightLoadedRes);
    BattleFx_InstallBillboardMatrix((short *)task->data, 0x1800, 0);
    ((BattleFxSpriteRequest *)g_BattleSpecialSearchlightSpriteData)->frame = task->frameCounter;
    g_BattleSpecialSearchlightDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialSearchlightSpriteData, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSearchlightDrawListHandle);
    if (g_BattleIsPaused == 0 && ((BattleFxSpriteRequest *)g_BattleSpecialSearchlightSpriteData)->spriteGroup != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= ((BattleFxSpriteGroup *)((BattleFxSpriteRequest *)g_BattleSpecialSearchlightSpriteData)->spriteGroup)->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
