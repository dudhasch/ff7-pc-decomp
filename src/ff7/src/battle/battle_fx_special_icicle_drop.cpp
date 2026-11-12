// src/ff7/src/battle/battle_fx_special_icicle_drop.cpp
/* Enemy Special Effect [idx 78]: IcicleDrop (JP: turara / つらら (Icicle), asset: "special/turara") — Icicle Drop (0x2a9, Icicle), Cave-in (0x27a, Demon's Gate); also called by Battle_LimitFxThunk71 */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Turara attack: it loads resources, then runs per-target
//   timeline tasks that drive falling, rotating and drifting sprites, falling and expanding models,
//   and an idle ramp-down toggle, with a matrix helper for the scaled, rotated models.

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
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// GLOBAL: FF7 0x008a3000
int g_BattleSpecIcicleDropResourceDefs = 0x001900B1;
int g_BattleSpecIcicleDropResourceDefsHeadPtr[1] = {
    (int)&g_BattleFxSpecialIcicleDropResourceNames[0]
};
// GLOBAL: FF7 0x008a3008
BattleSpecIcicleDropResourceDefRows g_BattleSpecIcicleDropResourceDefRows = {{
    0x0, 0x0, 0x0, 0x1900b1, (int)&g_BattleFxSpecialIcicleDropResourceNames[0xc], 0x0,
    0x0, 0x0, 0x1900b1, (int)&g_BattleFxSpecialIcicleDropResourceNames[0x18], 0x0, 0x0,
    0x0, 0x1900b1, (int)&g_BattleFxSpecialIcicleDropResourceNames[0x24], 0x0, 0x0, 0x0,
    0x192a39, (int)&g_BattleFxSpecialIcicleDropResourceNames[0x2c], (int)&g_BattleFxSpecialIcicleDropResourceNames[0x3c], 0x0, 0x0, 0x192a39,
    (int)&g_BattleFxSpecialIcicleDropResourceNames[0x48], (int)&g_BattleFxSpecialIcicleDropResourceNames[0x58], 0x0, 0x0, 0x192a39, (int)&g_BattleFxSpecialIcicleDropResourceNames[0x64],
    (int)&g_BattleFxSpecialIcicleDropResourceNames[0x74], 0x0, 0x0, 0x0}, 0x0, 0x0
};
// GLOBAL: FF7 0x008a3098
struct BattleSpecHeartBreathSpriteReq1 g_BattleSpecIcicleDropSpriteReq1 = {-128, -128, -128, {0x2c, 0, 0, 0, 0}};
// The falling rotating sprites' resource entries, picked by each slot's modelIndex.
// GLOBAL: FF7 0x008a30a0
int g_BattleSpecIcicleDropModelIndices[2] = {5, 6};
// GLOBAL: FF7 0x008a30a8
BattleFxSpriteRequest g_BattleSpecIcicleDropSpriteReq2 = {0, 0, -128, -128, -128, 0x2c};
// GLOBAL: FF7 0x008a30b8
BattleSpecIcicleDropSpriteReq3 g_BattleSpecIcicleDropSpriteReq3 = {{0x1000, 0x1000, 0x1000}, {0, 0x28, 0, 0x80, 0x20}, {0, 1, 2}, 0, 0xa8};
unsigned char g_rawData_008a30f4[4] = {0x20, 0x00, 0x00, 0x00};
unsigned char g_BattleFxSpecialIcicleDropResourceNames[128] =
    "TURARA1.RSD\0"
    "TURARA2.RSD\0"
    "TURARA3.RSD\0"
    "WA.RSD\0\0"
    "TSURAR03.TIM\0\0\0\0"
    "BAKU1.SPT\0\0\0"
    "TSURAR01.TIM\0\0\0\0"
    "CRY1.SPT\0\0\0\0"
    "TSURAR01.TIM\0\0\0\0"
    "CRY2.SPT";
// clang-format on

// GLOBAL: FF7 0x00b64ff8
int g_BattleSpecIcicleDropPacketCursor = 0;
unsigned char g_bssPad_00b64ffc[4] = {0};
unsigned char g_bssPad_00b65000[32] = {0};
// clang-format off
// GLOBAL: FF7 0x00b65020
PsxMatrix g_BattleSpecIcicleDropLightMatrix = {{{0}}, {0}};
// clang-format on
// GLOBAL: FF7 0x00b65040
struct GfxTableCursor *g_BattleSpecIcicleDropLoadedResTable = 0;
unsigned char g_bssPad_00b65044[4] = {0};
unsigned char g_bssPad_00b65048[8] = {0};
// GLOBAL: FF7 0x00b65050
short g_BattleSpecIcicleDropTransformCoord1 = 0;
unsigned char g_bssPad_00b65054[4] = {0};
// GLOBAL: FF7 0x00b65058
short g_BattleSpecIcicleDropTransformCoord2 = 0;
unsigned char g_bssPad_00b6505c[4] = {0};
// GLOBAL: FF7 0x00b65060
struct BattleSpecIcicleDropTransformCoord3 g_BattleSpecIcicleDropTransformCoord3 = {0};
unsigned char g_bssPad_00b65070[4] = {0};
// clang-format on


/*
 * When the engine context exists and g_BattleSpecIcicleDropLoadedResTable is empty, builds special/turara with a default-
 * duration record and creates resource type 7 using g_BattleSpecIcicleDropResourceDefs.
 */
// FUNCTION: FF7 0x00538480
void __cdecl BattleFx_SpecialLoadResIcicleDrop(void)
{
    GfxModelTableBank *stage;
    GfxResourceRequest currentRecord;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecIcicleDropLoadedResTable == 0) {
        kind = 7;
        resourceDefsWord = (int)&g_BattleSpecIcicleDropResourceDefs;
        stage = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a3178
        pRequest->resourcePath = BattleFx_BuildResourcePath("special/turara", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecIcicleDropLoadedResTable, stage);
    }
}

/*
 * Calls BattleFx_SpecialLoadResIcicleDrop, then BattleFx_SpecialBeginIcicleDrop with both arguments unchanged.
 */
// FUNCTION: FF7 0x00538510
void __cdecl BattleFx_SpecialEnterIcicleDrop(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResIcicleDrop();
    BattleFx_SpecialBeginIcicleDrop(targetMask, actorIdx);
}

/*
 * Allocates BattleFx_SpecialIcicleDropRampDownWhenIdleTask, creates mode-four BattleFx_SpecialSpawnIcicleDropPerTarget actor-mask callbacks, dispatches (0x16E,
 * narrowed targetIndex, 0x40), sets the global color triple to (0, 0, 0x40), and starts a 0x1000 ramp
 * with duration argument ten.
 */
// FUNCTION: FF7 0x0053852a
void __cdecl BattleFx_SpecialBeginIcicleDrop(int targetMask, int actorIdx)
{
    Battle_AllocateResourceSlot(BattleFx_SpecialIcicleDropRampDownWhenIdleTask);
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnIcicleDropPerTarget);
    Battle_PlaySfxPannedToActor(0x16E, (short)actorIdx, 0x40);
    g_BattleFxTimelineState.channel1 = 0;
    g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    g_BattleFxTimelineState.channel2 = 0x40;
    Battle_StartFixedPointRamp(0x1000, 10);
}

/*
 * Two-state toggle step for the current task slot: each frame it flips the slot's toggle between 0 and 1 with an
 * XOR. When g_BattleSecondaryTaskActiveCount has fallen to 1 or below it calls Battle_StartFixedPointRamp(0, 10) and
 * frees the slot.
 */
// FUNCTION: FF7 0x0053858e
void __cdecl BattleFx_SpecialIcicleDropRampDownWhenIdleTask(void)
{
    BattleFxIcicleDropRamp *task;

    task = (BattleFxIcicleDropRamp *)BATTLE_CURRENT_TASK_SLOT;
    task->toggle ^= 1;
    if (g_BattleSecondaryTaskActiveCount <= 1) {
        Battle_StartFixedPointRamp(0, 10);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Allocates a slot for BattleFx_SpecialIcicleDropTimelineTask, writes the source actor's targetBone position and the
 * target actor's targetBone position into the slot, then stores actor in its actorIndex.
 */
// FUNCTION: FF7 0x005385da
void __cdecl BattleFx_SpecialSpawnIcicleDropPerTarget(int actor, int sourceIndex)
{
    BattleFxIcicleDropRecord *taskSlot;

    taskSlot = (BattleFxIcicleDropRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialIcicleDropTimelineTask);
    Battle_GetBoneWorldPos(sourceIndex, g_BattleActorRecords[sourceIndex].targetBone, (short *)((int)taskSlot + 4));
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)((int)taskSlot + 0xC));
    taskSlot->actorIndex = actor;
}

/*
 * Timeline task that drops icicle models and shatter sprites onto the target for Turara.
 */
// FUNCTION: FF7 0x00538650
void __cdecl BattleFx_SpecialIcicleDropTimelineTask(void)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxIcicleDropRecord *q;
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short chunkIndex;
    BattleFxIcicleDropDebrisState *newSlotPtr;
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int y; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int a; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int b; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short activeAngle;
    int d; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int e; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int f; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int g; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int h; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int j; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int k; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int m; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    q = (BattleFxIcicleDropRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (q->frame == 0) {
        activeAngle = rand() % 0x1000;
        for (chunkIndex = 0; chunkIndex < 5; chunkIndex++) {
            newSlotPtr = (BattleFxIcicleDropDebrisState *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialIcicleDropFallingModelTask);
            newSlotPtr->posX = q->baseX + (CosFixed(activeAngle) * 0x1F4 >> 12);
            newSlotPtr->posZ = q->baseZ + (SinFixed(activeAngle) * 0x1F4 >> 12);
            newSlotPtr->posY = 0xFFFFF448 - rand() % 2000;
            newSlotPtr->velocityY = 100;
            newSlotPtr->angle = activeAngle;
            newSlotPtr->spinAngle = 0;
            newSlotPtr->variant = rand() % 3;
            activeAngle += (short)(rand() % 0xC8 + 0x2D0);
        }
    }
    if (q->frame == 10) {
        newSlotPtr = (BattleFxIcicleDropDebrisState *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialIcicleDropExpandingModelTask);
        newSlotPtr->posX = q->baseX;
        newSlotPtr->posZ = q->baseZ;
        newSlotPtr->posY = 0;
    }
    if (q->frame >= 10 && q->frame < 14) {
        for (chunkIndex = 0; chunkIndex < 2; chunkIndex++) {
            activeAngle = rand() % 0x1000;
            newSlotPtr = (BattleFxIcicleDropDebrisState *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialIcicleDropDriftAnimatedSpriteTask);
            newSlotPtr->velocityX = CosFixed(activeAngle) * 0x64 >> 12;
            newSlotPtr->velocityZ = SinFixed(activeAngle) * 0x64 >> 12;
            newSlotPtr->velocityY = -rand() % 100;
            newSlotPtr->posX = q->baseX + newSlotPtr->velocityX * 4;
            newSlotPtr->posZ = q->baseZ + newSlotPtr->velocityZ * 4;
            newSlotPtr->posY = 0;
        }
        for (chunkIndex = 0; chunkIndex < 2; chunkIndex++) {
            activeAngle = rand() % 0x1000;
            newSlotPtr = (BattleFxIcicleDropDebrisState *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialIcicleDropFallingRotatingSpriteTask)];
            newSlotPtr->velocityX = CosFixed(activeAngle) * 0x64 >> 12;
            newSlotPtr->velocityZ = SinFixed(activeAngle) * 0x64 >> 12;
            newSlotPtr->velocityY = 0xFFFFFED4 - rand() % 200;
            newSlotPtr->posX = q->baseX + newSlotPtr->velocityX * 4;
            newSlotPtr->posZ = q->baseZ + newSlotPtr->velocityZ * 4;
            newSlotPtr->posY = 0;
            newSlotPtr->variant = chunkIndex;
        }
    }
    if (q->frame == 30) {
        Battle_PostActorHitOrHurtMessage(q->actorIndex);
    }
    (q->frame)++;
    if (q->frame == 45) {
        q->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates falling and spinning ice-shard sprites spawned when a Turara icicle shatters.
 */
// FUNCTION: FF7 0x00538a12
void __cdecl BattleFx_SpecialIcicleDropFallingRotatingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[16];
        int fadeLevel;
        struct { unsigned char pad_to_4[0x4]; short rotAngleX; };
        struct { unsigned char pad_to_6[0x6]; short tmpZero; };
        struct { unsigned char pad_to_8[0x8]; short spinAngle; };
        struct { unsigned char pad_to_C[0xC]; BattleFxIcicleDropFallingRotatingSpriteSlot *spriteSlot; };
    } frame;
#pragma pack(pop)

    frame.spriteSlot = (BattleFxIcicleDropFallingRotatingSpriteSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    frame.tmpZero = 0;
    frame.rotAngleX = frame.tmpZero;
    frame.spinAngle = frame.spriteSlot->frameCounter << 8;
    BattleFx_SpecialIcicleDropInstallRotatedScaledMatrix(&frame.spriteSlot->posX, &frame.rotAngleX, 0x1800 /* 1.5 */);
    g_BattleSpecIcicleDropResourceDefRows.drawObj = Gfx_ModelTableGetSpriteObject(g_BattleSpecIcicleDropModelIndices[frame.spriteSlot->modelIndex], g_BattleSpecIcicleDropLoadedResTable);
    g_BattleSpecIcicleDropResourceDefRows.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(g_BattleSpecIcicleDropModelIndices[frame.spriteSlot->modelIndex], g_BattleSpecIcicleDropLoadedResTable);
    if (frame.spriteSlot->frameCounter < 12) {
        frame.fadeLevel = 8;
    } else {
        frame.fadeLevel = 20 - frame.spriteSlot->frameCounter;
    }
    g_BattleSpecIcicleDropSpriteReq1.blue = frame.fadeLevel << 4;
    g_BattleSpecIcicleDropSpriteReq1.green = g_BattleSpecIcicleDropSpriteReq1.blue;
    (g_BattleSpecIcicleDropSpriteReq1.red) = g_BattleSpecIcicleDropSpriteReq1.green;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecIcicleDropResourceDefRows.spriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spriteSlot->velocityY += 40;
    frame.spriteSlot->posX += frame.spriteSlot->velocityX;
    frame.spriteSlot->posY += frame.spriteSlot->velocityY;
    frame.spriteSlot->posZ += frame.spriteSlot->velocityZ;
    (frame.spriteSlot->frameCounter)++;
    if (frame.spriteSlot->frameCounter == 20) {
        frame.spriteSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Builds g_BattleSpecIcicleDropLightMatrix. Zeroes its rotation rows, loads g_BattleCameraTransform as the GTE
 * rotation and translation matrix, transforms point with RotTransPoint into the matrix's translation,
 * writes scale into the diagonal, builds the rotation from rotation with RotMatrix and installs the
 * matrix with SetRotMatrix/SetTransMatrix. Returns the matrix.
 */
// FUNCTION: FF7 0x00538b98
int __cdecl BattleFx_SpecialIcicleDropInstallRotatedScaledMatrix(void *point, void *rotation, short scale)
{
    int depth;
    short i;

    for (i = 0; i < 3; i++) {
        g_BattleSpecIcicleDropLightMatrix.m[2][i] = 0;
        g_BattleSpecIcicleDropLightMatrix.m[1][i] = 0;
        g_BattleSpecIcicleDropLightMatrix.m[0][i] = 0;
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint(point, g_BattleSpecIcicleDropLightMatrix.t, &depth);
    g_BattleSpecIcicleDropLightMatrix.m[2][2] = scale;
    g_BattleSpecIcicleDropLightMatrix.m[1][1] = g_BattleSpecIcicleDropLightMatrix.m[2][2];
    g_BattleSpecIcicleDropLightMatrix.m[0][0] = g_BattleSpecIcicleDropLightMatrix.m[1][1];
    RotMatrix(rotation, &g_BattleSpecIcicleDropLightMatrix);
    SetRotMatrix(&g_BattleSpecIcicleDropLightMatrix);
    SetTransMatrix(&g_BattleSpecIcicleDropLightMatrix);
    return (int)&g_BattleSpecIcicleDropLightMatrix;
}

// Per-frame step for a sprite that moves under a stored per-axis velocity. It installs the
// billboard from the slot's position at scale 0x2000, writes the frame index to
// g_BattleSpecIcicleDropSpriteReq2.frame, and fills g_BattleSpecIcicleDropSpriteReq2 from index 4 of
// g_BattleSpecIcicleDropLoadedResTable. The drawObj handle is assigned before the sprite group.
// The sprite is submitted with the cursor in g_BattleSpecIcicleDropPacketCursor. Unless
// g_BattleIsPaused is set, the position adds velocityX, velocityY and velocityZ, frameIndex
// advances, and the slot is freed once frameIndex reaches the
// animation's frame count.
// FUNCTION: FF7 0x00538c6f
void __cdecl BattleFx_SpecialIcicleDropDriftAnimatedSpriteTask(void)
{
    BattleFxEnergyOrbDriftSprite *task;

    task = (BattleFxEnergyOrbDriftSprite *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), 0x2000 /* 2.0 */, 0);
    g_BattleSpecIcicleDropSpriteReq2.frame = task->frameIndex;
    g_BattleSpecIcicleDropSpriteReq2.drawObj = Gfx_ModelTableGetSpriteObject(4, g_BattleSpecIcicleDropLoadedResTable);
    g_BattleSpecIcicleDropSpriteReq2.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(4, g_BattleSpecIcicleDropLoadedResTable);
    g_BattleSpecIcicleDropPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecIcicleDropSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecIcicleDropPacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posX += task->velocityX;
    task->posY += task->velocityY;
    task->posZ += task->velocityZ;
    (task->frameIndex)++;
    if (task->frameIndex >= g_BattleSpecIcicleDropSpriteReq2.spriteGroup->header->frameCount) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Advances a falling 3D icicle model and triggers impact shatter sprites for Turara.
 */
// FUNCTION: FF7 0x00538d77
void __cdecl BattleFx_SpecialIcicleDropFallingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[100];
        int scaleLevel;
        struct { unsigned char pad_to_4[0x4]; SVECTOR spinAngle; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_40[0x40]; BattleFxIcicleDropFallingModelSlot *modelSlot; };
    } frame;
#pragma pack(pop)

    frame.modelSlot = (BattleFxIcicleDropFallingModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.spinAngle.vx = frame.modelSlot->spinAngle;
    frame.spinAngle.vy = frame.modelSlot->angleY;
    frame.spinAngle.vz = 0;
    RotMatrix(&frame.spinAngle, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecIcicleDropSpriteReq3.scale);
    frame.rotMatrix.t[0] = frame.modelSlot->posX;
    frame.rotMatrix.t[1] = frame.modelSlot->posY;
    frame.rotMatrix.t[2] = frame.modelSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    g_BattleSpecIcicleDropSpriteReq3.modelDraw.modelGroup = Gfx_ModelTableGetRenderObject(g_BattleSpecIcicleDropSpriteReq3.modelIds[frame.modelSlot->modelIndex], g_BattleSpecIcicleDropLoadedResTable);
    if (frame.modelSlot->frameCounter < 17) {
        frame.scaleLevel = 8;
    } else {
        frame.scaleLevel = 0x19 - frame.modelSlot->frameCounter;
    }
    g_BattleSpecIcicleDropSpriteReq3.modelDraw.fadeLevel = frame.scaleLevel << 4;
    g_BattleSpecIcicleDropPacketCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecIcicleDropSpriteReq3.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecIcicleDropPacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.modelSlot->posY < 100) {
        frame.modelSlot->velocityY += 50;
        frame.modelSlot->posY += frame.modelSlot->velocityY;
    } else {
        frame.modelSlot->spinAngle += 0x40;
    }
    (frame.modelSlot->frameCounter)++;
    if (frame.modelSlot->frameCounter == 25) {
        frame.modelSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Sets equal shared scale entries to counter times 2000 and copies slot XYZ as translation,
 * composes with g_BattleCameraTransform into local matrix storage and installs it. Draws model field three in
 * black with request short counter times 0x111 before checking pause; when unpaused advances and
 * frees the slot exactly at fifteen.
 */
// FUNCTION: FF7 0x00538f0e
void __cdecl BattleFx_SpecialIcicleDropExpandingModelTask(void)
{
    BattleFxIcicleDropGrowSlot *expanding;
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    PsxMatrix matrix;
    int a; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int b; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    expanding = (BattleFxIcicleDropGrowSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecIcicleDropTransformCoord3.scale) = expanding->growStep * 0x7D0;
    g_BattleSpecIcicleDropTransformCoord2 = (g_BattleSpecIcicleDropTransformCoord3.scale);
    g_BattleSpecIcicleDropTransformCoord1 = g_BattleSpecIcicleDropTransformCoord2;
    g_BattleSpecIcicleDropTransformCoord3.x = expanding->coordX;
    g_BattleSpecIcicleDropTransformCoord3.y = expanding->coordY;
    g_BattleSpecIcicleDropTransformCoord3.z = expanding->coordZ;
    MultiplyMatrix(&g_BattleCameraTransform, &g_BattleSpecIcicleDropTransformCoord1, &matrix);
    SetRotMatrix(&matrix);
    SetTransMatrix(&matrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecIcicleDropSpriteReq3.expandFadeLevel = expanding->growStep * 273;
    g_BattleSpecIcicleDropSpriteReq3.expandModelGroup = Gfx_ModelTableGetRenderObject(3, g_BattleSpecIcicleDropLoadedResTable);
    g_BattleSpecIcicleDropPacketCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecIcicleDropSpriteReq3.expandModelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecIcicleDropPacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (expanding->growStep)++;
    if (expanding->growStep == 15) {
        expanding->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
