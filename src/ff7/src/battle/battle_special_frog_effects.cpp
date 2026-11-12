// src/ff7/src/battle/battle_special_frog_effects.cpp
/* Enemy Special Attack [126]: Frog Jab / Frog Song (orig. SPECIAL/FROG) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Frog spell. It loads its resources, registers per-target
//   callbacks, and runs the timeline, growing and spiral model tasks, and the animated and
//   two-phase sprite tasks.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x0089aac0
int g_BattleSpecFrogResourceDef = 0x001900B1;
int g_BattleSpecFrogResourceDefHeadPtr[1] = {
    (int)((char *)&g_BattleSpecialFrogEffectsResourceNames + 0x0)
};
// GLOBAL: FF7 0x0089aac8
int g_BattleSpecFrogSpriteDef[30] = {
    0x0, 0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleSpecialFrogEffectsResourceNames + 0xc), 0x0,
    0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleSpecialFrogEffectsResourceNames + 0x18), 0x0, 0x0,
    0x0, 0x192a39, (int)((char *)&g_BattleSpecialFrogEffectsResourceNames + 0x24), (int)((char *)&g_BattleSpecialFrogEffectsResourceNames + 0x30), 0x0, 0x0,
    0x192a39, (int)((char *)&g_BattleSpecialFrogEffectsResourceNames + 0x3c), (int)((char *)&g_BattleSpecialFrogEffectsResourceNames + 0x48), 0x0, 0x0, 0x192a39,
    (int)((char *)&g_BattleSpecialFrogEffectsResourceNames + 0x54), (int)((char *)&g_BattleSpecialFrogEffectsResourceNames + 0x60), 0x0, 0x0, 0x0, 0x0
};
// bare addresses: one object with the retail bytes.
// GLOBAL: FF7 0x0089ab40
unsigned char g_BattleSpecFrogColorData[8] = {0x60, 0x60, 0x60, 0x2c, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0089ab48
struct BattleSpecFrogSpriteRequest g_BattleSpecFrogSpriteRequest = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}, 0, {0xa8, 0, 0, 0, 0, 0}, 128};
unsigned char g_rawData_0089ab64[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0089ab68
struct BattleSpecFrogTransformData g_BattleSpecFrogTransformData = {{0, 0xfc, 0, 0, 0, 0, 0, 0}, 0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
unsigned char g_BattleSpecialFrogEffectsResourceNames[108] =
    "SARCHI.RSD\0\0"
    "SARCHI1.RSD\0"
    "HIKARI.RSD\0\0"
    "HIT1.TIM\0\0\0\0"
    "HIT11.SPT\0\0\0"
    "BAKU1.TIM\0\0\0"
    "BAKU11.SPT\0\0"
    "TODO_1.TIM\0\0"
    "TODO_1.SPT";
// GLOBAL: FF7 0x00b5a7d8
PsxMatrix g_BattleSpecFrogState = {0};
// GLOBAL: FF7 0x00b5a7f8
struct BattleSpecFrogBuffer g_BattleSpecFrogBuffer = {0};
// GLOBAL: FF7 0x00b5a7fc
short g_BattleSpecFrogTargetIdx = 0;
unsigned char g_bssPad_00b5a800[16] = {0};
// GLOBAL: FF7 0x00b5a810
int g_BattleSpecFrogTaskPtr = 0;
unsigned char g_bssPad_00b5a814[4] = {0};
unsigned char g_bssPad_00b5a818[8] = {0};
// GLOBAL: FF7 0x00b5a820
short g_BattleSpecFrogActiveCount = 0;
// GLOBAL: FF7 0x00b5a824
struct GfxTableCursor *g_BattleSpecFrogLoadedResTable = 0;
unsigned char g_bssPad_00b5a828[24] = {0};
// clang-format on
// The TU's own view of moved definitions declared with another type.

/*
 * BattleFx_SpecialLoadResFrog -- 144 bytes
 * When the engine context exists and g_BattleSpecFrogLoadedResTable is empty, builds SPECIAL/FROG with a default-
 * duration record and creates resource type 6 using g_BattleSpecFrogResourceDef.
 */
// FUNCTION: FF7 0x00519830
void __cdecl BattleFx_SpecialLoadResFrog(void)
{
    GfxModelTableBank *ownerHandle;
    GfxResourceRequest recBlock;
    int kind;
    char pathDstBuf[200];
    GfxResourceRequest *pRequest;
    int resourceDefAddr;

    if (g_pBattle3dScene != 0 && g_BattleSpecFrogLoadedResTable == 0) {
        kind = 6;
        resourceDefAddr = (int)&g_BattleSpecFrogResourceDef;
        ownerHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &recBlock;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0089abec
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/FROG", pathDstBuf);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefAddr, pRequest, &g_BattleSpecFrogLoadedResTable, ownerHandle);
    }
}

/*
 * BattleFx_SpecialEnterFrog -- 35 bytes
 * Entry point for the Frog special effect: loads its resources via BattleFx_SpecialLoadResFrog,
 * zeroes g_BattleSpecFrogActiveCount, then forwards actorMask and sourceIndex to BattleFx_SpecialRegisterFrogPerTargetCallback.
 */
// FUNCTION: FF7 0x005198c0
void __cdecl BattleFx_SpecialEnterFrog(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResFrog();
    g_BattleSpecFrogActiveCount = 0;
    BattleFx_SpecialRegisterFrogPerTargetCallback(targetMask, actorIdx);
}

/*
 * BattleFx_SpecialRegisterFrogPerTargetCallback -- 69 bytes
 * Creates mode-four BattleFx_SpecialSpawnFrogPerTarget actor-mask callbacks, starts a 0x1000 ramp with duration argument
 * ten, and sets the global color triple to (24, 16, 0).
 */
// FUNCTION: FF7 0x005198e3
void __cdecl BattleFx_SpecialRegisterFrogPerTargetCallback(int actorMask, int sourceIndex)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    Battle_CreateBitPatternCallbackTask(actorMask, sourceIndex, 4, BattleFx_SpecialSpawnFrogPerTarget);
    Battle_StartFixedPointRamp(0x1000, 10);
    g_BattleFxTimelineState.red = 0x18;
    g_BattleFxTimelineState.channel1 = 0x10;
    g_BattleFxTimelineState.channel2 = 0;
}

// Allocates a slot for BattleFx_SpecialFrogTimelineTask and fills it. The position at +4 is the
// world position of the byte two past the source actor's secondaryBone, and the position at +0xC is
// the world position of the target actor's targetBone (both from Battle_GetBoneWorldPos). randomRange is
// the target's radius plus 0xC8, and actor is stored in the record. The direction from start
// to end is passed with g_BattleSpecFrogState to Gte_BuildLookBasisFromDirection, then
// Battle_PlaySfxPannedToActor(0x62, sourceIndex, 0x40) is called.
// FUNCTION: FF7 0x00519928
void __cdecl BattleFx_SpecialSpawnFrogPerTarget(int actor, int sourceIndex)
{
    BattleFxFrogSpawnRecord *spawnRecord;
    short direction[3];

    spawnRecord = (BattleFxFrogSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFrogTimelineTask);
    Battle_GetBoneWorldPos(sourceIndex, g_BattleActorRecords[sourceIndex].bone34, (short *)((int)spawnRecord + 4));
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)((int)spawnRecord + 0xC));
    spawnRecord->randomRange = g_BattleActorRecords[actor].radius + 200;
    spawnRecord->targetIndex = actor;
    direction[0] = spawnRecord->endX - spawnRecord->startX;
    direction[1] = spawnRecord->endY - spawnRecord->startY;
    direction[2] = spawnRecord->endZ - spawnRecord->startZ;
    Gte_BuildLookBasisFromDirection(direction, (short *)&g_BattleSpecFrogState);
    Battle_PlaySfxPannedToActor(0x62, sourceIndex, 0x40);
}

/*
 * BattleFx_SpecialFrogTimelineTask -- 1019 bytes
 */
// FUNCTION: FF7 0x00519a1b
void __cdecl BattleFx_SpecialFrogTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; int transformedPos; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offsetX; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxFrogTimelineSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_38[0x38]; short loopIndex; };
        struct { unsigned char pad_to_3C[0x3C]; BattleFxFrogTimeline *ringRecord; };
        struct { unsigned char pad_to_40[0x40]; BattleFxFrogTimelineSlot *timelineSlot; };
        struct { unsigned char pad_to_48[0x48]; short distance; };
    } frame;
#pragma pack(pop)

    frame.timelineSlot = (BattleFxFrogTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timelineSlot->frameCounter == 0) {
        (g_BattleSpecFrogBuffer.spanX) = frame.timelineSlot->endX - frame.timelineSlot->startX;
        g_BattleSpecFrogBuffer.spanY = frame.timelineSlot->endY - frame.timelineSlot->startY;
        g_BattleSpecFrogTargetIdx = frame.timelineSlot->endZ - frame.timelineSlot->startZ;
        frame.distance = SqrtToInt((g_BattleSpecFrogBuffer.spanX) * (g_BattleSpecFrogBuffer.spanX) + g_BattleSpecFrogBuffer.spanY * g_BattleSpecFrogBuffer.spanY + g_BattleSpecFrogTargetIdx * g_BattleSpecFrogTargetIdx);
        for (frame.loopIndex = 0; frame.loopIndex < 4; frame.loopIndex++) {
            frame.spawnSlot = (BattleFxFrogTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFrogSpiralModelTask);
            frame.spawnSlot->startX = frame.timelineSlot->startX;
            frame.spawnSlot->startY = frame.timelineSlot->startY;
            frame.spawnSlot->startZ = frame.timelineSlot->startZ;
            frame.spawnSlot->angle = ArcTangentFixed(frame.timelineSlot->endX - frame.timelineSlot->startX, frame.timelineSlot->endZ - frame.timelineSlot->startZ);
            frame.spawnSlot->scaleY = frame.distance * 0x1500 / 2800;
            frame.spawnSlot->spiralIndex = frame.loopIndex;
            frame.spawnSlot->distance = frame.distance;
        }
    }
    if (frame.timelineSlot->frameCounter == 35) {
        frame.spawnSlot = (BattleFxFrogTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFrogGrowingModelTask);
        Battle_GetBoneWorldPos(frame.timelineSlot->actorIndex, g_BattleActorRecords[frame.timelineSlot->actorIndex].targetBone, (short *)((int)frame.spawnSlot + 4));
    }
    if (frame.timelineSlot->frameCounter == 45) {
        Battle_PlaySfxPannedToActor(0x134, (short)frame.timelineSlot->actorIndex, 0x40);
    }
    if (frame.timelineSlot->frameCounter >= 45 && frame.timelineSlot->frameCounter < 65) {
        for (frame.loopIndex = 0; frame.loopIndex < 5; frame.loopIndex++) {
            frame.spawnSlot = (BattleFxFrogTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFrogAnimatedSpriteTask);
            Battle_GetBoneWorldPos(frame.timelineSlot->actorIndex, g_BattleActorRecords[frame.timelineSlot->actorIndex].targetBone, (short *)((int)frame.spawnSlot + 4));
            frame.offsetX.vy = rand() % frame.timelineSlot->randomRange;
            frame.offsetX.vz = 0;
            frame.offsetX.vx = frame.offsetX.vz;
            frame.rotX.vx = (rand() % 4096);
            frame.rotX.vz = (rand() % 4096);
            frame.rotX.vy = 0;
            RotMatrix(&frame.rotX, &frame.rotMatrix);
            frame.rotMatrix.t[0] = frame.spawnSlot->startX;
            frame.rotMatrix.t[1] = frame.spawnSlot->startY;
            frame.rotMatrix.t[2] = frame.spawnSlot->startZ;
            SetRotMatrix(&frame.rotMatrix);
            SetTransMatrix(&frame.rotMatrix);
            RotTransPointToInt16(&frame.offsetX, (short *)((int)frame.spawnSlot + 4), &frame.transformedPos);
        }
    }
    if (frame.timelineSlot->frameCounter == 50) {
        if ((g_BattleHurtMessageActorMask & 1 << frame.timelineSlot->actorIndex) != 0) {
            frame.ringRecord = (BattleFxFrogTimeline *)Battle_AllocSceneMessage(6);
        } else if ((g_BattleTimelineTargetMask & 1 << frame.timelineSlot->actorIndex) != 0) {
            frame.ringRecord = (BattleFxFrogTimeline *)Battle_AllocSceneMessage(4);
            frame.ringRecord->effectOrder = 0;
        } else {
            frame.ringRecord = (BattleFxFrogTimeline *)Battle_AllocSceneMessage(4);
        }
        frame.ringRecord->actorIndex = frame.timelineSlot->actorIndex;
    }
    (frame.timelineSlot->frameCounter)++;
    if (frame.timelineSlot->frameCounter == 80) {
        Battle_StartFixedPointRamp(0, 10);
        frame.timelineSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * BattleFx_SpecialFrogAnimatedSpriteTask -- 211 bytes Per-frame tick for the effect slot at
 * (g_BattleActiveTaskSlotIdx << 5) + &g_BattleHitLivewireEmitterRecords. It fills the draw record at
 * g_BattleSpecFrogSpriteRequest from Gfx_ModelTableGetLoadedRecord(5, g_BattleSpecFrogLoadedResTable) and
 * Gfx_ModelTableGetSpriteObject(5, g_BattleSpecFrogLoadedResTable), installs the billboard matrix for
 * the slot transform at p+4 with scale 0x3000, publishes the frame counter to
 * g_BattleSpecFrogSpriteRequest[12], and chains the sprite onto the draw list
 * g_BattleSpecFrogTaskPtr. When unpaused with a nonnull sprite it advances the frame counter and
 * stores 0xFFFF into word 0 when the counter equals the animation frame count at
 * *(*g_BattleSpecFrogSpriteRequest + 4).
 */
// FUNCTION: FF7 0x00519e16
void __cdecl BattleFx_SpecialFrogAnimatedSpriteTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecFrogSpriteRequest.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(5, g_BattleSpecFrogLoadedResTable);
    g_BattleSpecFrogSpriteRequest.reqResource = Gfx_ModelTableGetSpriteObject(5, g_BattleSpecFrogLoadedResTable);
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), 0x3000 /* 3.0 */, 0);
    g_BattleSpecFrogSpriteRequest.reqWord12 = task->frameCounter;
    g_BattleSpecFrogTaskPtr = BattleFx_SubmitSpriteDraw(&g_BattleSpecFrogSpriteRequest.spriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecFrogTaskPtr);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (g_BattleSpecFrogSpriteRequest.spriteGroup != 0) {
        (task->frameCounter)++;
        if (task->frameCounter == g_BattleSpecFrogSpriteRequest.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * BattleFx_SpecialFrogGrowingModelTask -- 378 bytes
 */
// FUNCTION: FF7 0x00519ee9
void __cdecl BattleFx_SpecialFrogGrowingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int projDepth;
        struct { unsigned char pad_to_4[0x4]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix growMatrixA; };
        struct { unsigned char pad_to_48[0x48]; short loopIndex; };
        struct { unsigned char pad_to_50[0x50]; BattleTaskSlot *growingModel; };
    } frame;
#pragma pack(pop)

    frame.growingModel = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecFrogSpriteRequest.modelRecord = Gfx_ModelTableGetRenderObject(2, g_BattleSpecFrogLoadedResTable);
    for (frame.loopIndex = 0; frame.loopIndex < 3; frame.loopIndex++) {
        frame.growMatrixA.m[2][frame.loopIndex] = 0;
        frame.growMatrixA.m[1][frame.loopIndex] = 0;
        frame.growMatrixA.m[0][frame.loopIndex] = 0;
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint(frame.growingModel->data, frame.growMatrixA.t, &frame.projDepth);
    frame.growMatrixA.m[2][2] = frame.growingModel->frameCounter * 1000 + 0x400;
    frame.growMatrixA.m[1][1] = frame.growMatrixA.m[2][2];
    frame.growMatrixA.m[0][0] = frame.growMatrixA.m[1][1];
    RotMatrix((SVECTOR *)&g_BattleSpecFrogTransformData, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.growMatrixA, &frame.rotMatrix);
    SetRotMatrix(&frame.growMatrixA);
    SetTransMatrix(&frame.growMatrixA);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecFrogSpriteRequest.reqWord26 = frame.growingModel->frameCounter << 8;
    g_BattleSpecFrogTaskPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecFrogSpriteRequest.modelRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecFrogTaskPtr);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.growingModel->frameCounter)++;
    if (frame.growingModel->frameCounter == 15) {
        frame.growingModel->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * BattleFx_SpecialFrogSpiralModelTask -- 903 bytes
 */
// FUNCTION: FF7 0x0051a063
void __cdecl BattleFx_SpecialFrogSpiralModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[132];
        int fadeWord;
        struct { unsigned char pad_to_4[0x4]; SVECTOR spiralRotX; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offsetX; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix spiralMatrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxFrogTwoPhaseAnimatedSpriteSlot *phaseSprite; };
        struct { unsigned char pad_to_40[0x40]; int transformedPos; };
        struct { unsigned char pad_to_44[0x44]; SVECTOR spriteRotX; };
        struct { unsigned char pad_to_4C[0x4C]; BattleFxFrogSpiralModelSlot *spiralSlot; };
        struct { unsigned char pad_to_70[0x70]; short spiralSign; };
        struct { unsigned char pad_to_74[0x74]; VECTOR scaleVector; };
    } frame;
#pragma pack(pop)

    frame.spiralSlot = (BattleFxFrogSpiralModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.spiralSign = (frame.spiralSlot->spiralIndex >> 1) * 2 - 1;
    frame.spiralRotX.vx = SinFixed(frame.spiralSlot->frameCounter << 8) / 40 * frame.spiralSign;
    frame.spriteRotX.vx = frame.spiralRotX.vx;
    frame.spiralRotX.vy = (SinFixed(frame.spiralSlot->frameCounter << 7) >> 5) * frame.spiralSign;
    frame.spriteRotX.vy = frame.spiralRotX.vy;
    frame.spiralRotX.vz = 0;
    frame.spriteRotX.vz = frame.spiralRotX.vz;
    frame.spiralRotX.vx -= 0x400 /* 90 deg */;
    RotMatrix(&frame.spiralRotX, &frame.spiralMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecFrogState, &frame.spiralMatrix, &frame.spiralMatrix);
    frame.scaleVector.vy = frame.spiralSlot->scaleY;
    frame.scaleVector.vz = 0x3000 /* 3.0 */;
    frame.scaleVector.vx = frame.scaleVector.vz;
    ScaleMatrix(&frame.spiralMatrix, &frame.scaleVector);
    frame.spiralMatrix.t[0] = frame.spiralSlot->posX;
    frame.spiralMatrix.t[1] = frame.spiralSlot->posY;
    frame.spiralMatrix.t[2] = frame.spiralSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.spiralMatrix, &frame.spiralMatrix);
    SetRotMatrix(&frame.spiralMatrix);
    SetTransMatrix(&frame.spiralMatrix);
    SetDrawColorRGB(0, 0, 0);
    if ((frame.spiralSlot->spiralIndex % 2) == 0) {
        g_BattleSpecFrogTransformData.resource8 = Gfx_ModelTableGetRenderObject(1, g_BattleSpecFrogLoadedResTable);
    } else {
        g_BattleSpecFrogTransformData.resource8 = Gfx_ModelTableGetRenderObject(0, g_BattleSpecFrogLoadedResTable);
    }
    if ((frame.spiralSlot->spiralIndex % 2) == 0) {
        frame.fadeWord = 0x1000 - g_BattleSpecFrogActiveCount * 96;
    } else {
        frame.fadeWord = 0x1000 - (g_BattleSpecFrogActiveCount << 5);
    }
    g_BattleSpecFrogTransformData.alpha = *(short *)&frame.fadeWord;
    g_BattleSpecFrogTaskPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecFrogTransformData.resource8, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecFrogTaskPtr);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if ((frame.spiralSlot->spiralIndex % 2) == 0) {
        frame.phaseSprite = (BattleFxFrogTwoPhaseAnimatedSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFrogTwoPhaseAnimatedSpriteTask);
        frame.phaseSprite->posX = frame.spiralSlot->posX;
        frame.phaseSprite->posY = frame.spiralSlot->posY;
        frame.phaseSprite->posZ = frame.spiralSlot->posZ;
        frame.offsetX.vx = 0;
        frame.offsetX.vy = 0;
        frame.offsetX.vz = frame.spiralSlot->distance;
        RotMatrix(&frame.spriteRotX, &frame.spiralMatrix);
        ConcatMatrixPreservingTranslation(&g_BattleSpecFrogState, &frame.spiralMatrix, &frame.spiralMatrix);
        frame.spiralMatrix.t[0] = frame.phaseSprite->posX;
        frame.spiralMatrix.t[1] = frame.phaseSprite->posY;
        frame.spiralMatrix.t[2] = frame.phaseSprite->posZ;
        SetRotMatrix(&frame.spiralMatrix);
        SetTransMatrix(&frame.spiralMatrix);
        RotTransPointToInt16(&frame.offsetX, (short *)((int)frame.phaseSprite + 4), &frame.transformedPos);
        frame.phaseSprite->rotX = 0;
        frame.phaseSprite->yaw = frame.spiralSlot->yaw;
        frame.phaseSprite->rotZ = 0;
        frame.phaseSprite->phase = 0;
    }
    if (frame.spiralSlot->frameCounter < 8) {
        g_BattleSpecFrogActiveCount++;
    }
    if (frame.spiralSlot->frameCounter > 24) {
        g_BattleSpecFrogActiveCount--;
    }
    (frame.spiralSlot->frameCounter)++;
    if (frame.spiralSlot->frameCounter == 32) {
        frame.spiralSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * BattleFx_SpecialFrogTwoPhaseAnimatedSpriteTask -- 278 bytes
 * Per-frame draw of the slot at (g_BattleActiveTaskSlotIdx << 5) + &g_BattleHitLivewireEmitterRecords: installs a billboard matrix
 * from slot+4 at scale 0x2400, fills the draw record at 0x0089AB38/0x0089AB3C with row
 * {3,4}[slot+0x1A] of table g_BattleSpecFrogLoadedResTable via Gfx_ModelTableGetLoadedRecord / Gfx_ModelTableGetSpriteObject,
 * publishes the frame counter (slot+2) to the short at 0x0089AB44 and chains the draw through
 * g_BattleSpecFrogTaskPtr into bucket g_BattleCurrentOrderingTable+0x70 at shift 0xC. While g_BattleIsPaused is zero and the
 * record is non-null it increments slot+2; when that reaches *(*record + 4) it either switches
 * phase word slot+0x1A from 0 to 1 and restarts the counter, or (phase already 1) retires the slot
 * with 0xFFFF at +0.
 */
// FUNCTION: FF7 0x0051a3ea
void __cdecl BattleFx_SpecialFrogTwoPhaseAnimatedSpriteTask(void)
{
    int rowTable[2];
    BattleFxFrogTwoPhaseSlot *taskPtr;

    taskPtr = (BattleFxFrogTwoPhaseSlot *)BATTLE_CURRENT_TASK_SLOT;
    rowTable[0] = 3;
    rowTable[1] = 4;
    BattleFx_InstallBillboardMatrix((void *)((int)taskPtr + 4), 0x2400 /* 2.25 */, 0);
    ((BattleSpecFrogSpriteDef *)&g_BattleSpecFrogSpriteDef)->frameTable = Gfx_ModelTableGetLoadedRecord(rowTable[taskPtr->phase], g_BattleSpecFrogLoadedResTable);
    ((BattleSpecFrogSpriteDef *)&g_BattleSpecFrogSpriteDef)->drawObj = Gfx_ModelTableGetSpriteObject(rowTable[taskPtr->phase], g_BattleSpecFrogLoadedResTable);
    *(short *)((char *)&g_BattleSpecFrogColorData + 0x4) = taskPtr->frameIndex;
    g_BattleSpecFrogTaskPtr = BattleFx_SubmitSpriteDraw((char *)&g_BattleSpecFrogSpriteDef + 0x70, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecFrogTaskPtr);
    if (g_BattleIsPaused == 0) {
        if (((BattleSpecFrogSpriteDef *)&g_BattleSpecFrogSpriteDef)->frameTable != 0) {
            (taskPtr->frameIndex)++;
            if (taskPtr->frameIndex >= *(int *)(*(int *)((BattleSpecFrogSpriteDef *)&g_BattleSpecFrogSpriteDef)->frameTable + 4)) {
                if (taskPtr->phase == 0) {
                    taskPtr->phase = 1;
                    taskPtr->frameIndex = 0;
                } else {
                    taskPtr->ownerSlot = BATTLE_TASK_FREE;
                }
            }
        }
    }
}
} /* extern "C" */
