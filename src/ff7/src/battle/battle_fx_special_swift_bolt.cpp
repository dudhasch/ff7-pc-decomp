// src/ff7/src/battle/battle_fx_special_swift_bolt.cpp
/* Enemy Special Effect [idx 13]: SwiftBolt (JP: sander / サンダー (Thunder), asset: "special/sander") — Swift Bolt (0x365, Dyne), Thunder Kiss (0x1e2, Sonic Speed), Scissor Spark (0x1c0), Support Beam (0x393) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Sander attack: it loads resources and spawns a per-target
//   timeline that drives ramping, animated, growing, radial-spark and spinning sprite and model
//   tasks, plus a bolt emitter that draws bolt segments and a bouncing sprite task.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct GfxRotationAngles2;
struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x008a3bf0
unsigned int g_BattleSpecSwiftBoltResourceDefs[65] = {
    0x001900b1,
    (unsigned int)&g_BattleSpecSwiftBoltResName_geki1,
    0x00000000,
    0x00000000,
    0x00000000,
    0x001940b1,
    // STRING: FF7 0x008a3da4
    (unsigned int)"BAKU11.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a3db0
    (unsigned int)"THUNDE00.TIM",
    // STRING: FF7 0x008a3dc0
    (unsigned int)"00.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a3dc8
    (unsigned int)"THUNDE01.TIM",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a3dd8
    (unsigned int)"THUNDE02.TIM",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a3de8
    (unsigned int)"THUNDE02.TIM",
    // STRING: FF7 0x008a3df8
    (unsigned int)"HAHEN.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a3e04
    (unsigned int)"THUNDE02.TIM",
    // STRING: FF7 0x008a3e14
    (unsigned int)"HIT11.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a3e20
    (unsigned int)"THUNDE02.TIM",
    // STRING: FF7 0x008a3e30
    (unsigned int)"HIT12.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a3e3c
    (unsigned int)"THUNDE03.TIM",
    // STRING: FF7 0x008a3e4c
    (unsigned int)"ELEC1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a3e58
    (unsigned int)"THUNDE03.TIM",
    // STRING: FF7 0x008a3e68
    (unsigned int)"ELEC2.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a3e74
    (unsigned int)"THUNDE02.TIM",
    // STRING: FF7 0x008a3e84
    (unsigned int)"HIBANA.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a3e90
    (unsigned int)"THUNDE03.TIM",
    // STRING: FF7 0x008a3ea0
    (unsigned int)"KEMU11.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    0x00000000,
    // STRING: FF7 0x008a3eac
    (unsigned int)"jo_c01.spt"};
// clang-format off
// GLOBAL: FF7 0x008a3cf8
struct BattleSpecSwiftBoltSpriteReq1 g_BattleSpecSwiftBoltSpriteReq1 = {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0};
// clang-format on
// GLOBAL: FF7 0x008a3d08
unsigned char g_BattleSpecSwiftBoltAnimFrameIndices[16] = {0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00};
// clang-format off
// GLOBAL: FF7 0x008a3d18
struct BattleSpecSwiftBoltSpriteReq2 g_BattleSpecSwiftBoltSpriteReq2 = {0, 0, -128, -128, -128, {0x2e}, 0};
// GLOBAL: FF7 0x008a3d28
struct BattleSpecSwiftBoltSpriteReq3 g_BattleSpecSwiftBoltSpriteReq3 = {0, 0, -128, -128, -128, 44, 0};
// GLOBAL: FF7 0x008a3d38
struct BattleSpecSwiftBoltSpriteReq4 g_BattleSpecSwiftBoltSpriteReq4 = {0, 0, -128, -128, -128, {0x2c, 0, 0}};
// GLOBAL: FF7 0x008a3d48
struct BattleSpecSwiftBoltSpriteReq5 g_BattleSpecSwiftBoltSpriteReq5 = {0, 0, -128, -128, -128, {0x2e}, 0};
// clang-format on
// GLOBAL: FF7 0x008a3d58
unsigned char g_BattleSpecSwiftBoltOffsets1[6] = {0x00, 0x00, 0x00, 0xfc, 0x00, 0x00};
// GLOBAL: FF7 0x008a3d60
struct GfxRotationAngles2 g_BattleSpecSwiftBoltOffsets2 = {1024, -1024, 0};
// clang-format off
// GLOBAL: FF7 0x008a3d68
struct BattleSpecHellBomberModelReq3 g_BattleSpecSwiftBoltModelReq1 = {0, {0x28, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x008a3d78
struct BattleSpecHellBomberModelReq3 g_BattleSpecSwiftBoltModelReq2 = {0, {0x28, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}};
// clang-format on
unsigned char g_BattleSpecSwiftBoltUnusedSpriteReq[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x2c, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a3d98
unsigned char g_BattleSpecSwiftBoltResName_geki1[10] = "GEKI1.RSD";
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b68970
int g_BattleSpecSwiftBoltTargetBonePosXY = 0;
// GLOBAL: FF7 0x00b68974
short g_BattleSpecSwiftBoltTargetBonePosZ = 0;
// GLOBAL: FF7 0x00b68978
int g_BattleSpecSwiftBoltDrawListHead = 0;
unsigned char g_bssPad_00b6897c[4] = {0};
// GLOBAL: FF7 0x00b68980
int g_BattleSpecSwiftBoltModelScaleX = 0;
// GLOBAL: FF7 0x00b68984
int g_BattleSpecSwiftBoltModelScaleY = 0;
// GLOBAL: FF7 0x00b68988
int g_BattleSpecSwiftBoltModelScaleZ = 0;
unsigned char g_bssPad_00b6898c[4] = {0};
// GLOBAL: FF7 0x00b68990
struct GfxTableCursor *g_BattleSpecSwiftBoltModelTable = 0;
unsigned char g_bssPad_00b68994[4] = {0};
// clang-format on

/*
 * Loads the Sander resources (special/sander) into g_BattleSpecSwiftBoltModelTable, once, when the battle scene exists
 * and the table is still empty. Resource type 13 is built from g_BattleSpecSwiftBoltResourceDefs.
 */
// FUNCTION: FF7 0x0053daa0
void __cdecl BattleFx_SpecialLoadResSwiftBolt(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest currentRecord;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecSwiftBoltModelTable == 0) {
        kind = 0xD;
        resourceDefsWord = (int)&g_BattleSpecSwiftBoltResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a3eb8
        pRequest->resourcePath = BattleFx_BuildResourcePath("special/sander", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecSwiftBoltModelTable, curOwnerCtx);
    }
}

/* Entry point for the Sander special; forwards both indices to BattleFx_SpecialBeginSwiftBolt. */
// FUNCTION: FF7 0x0053db30
void __cdecl BattleFx_SpecialEnterSwiftBolt(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginSwiftBolt(targetMask, actorIdx);
}

/*
 * Starts the Sander effect: allocates BattleFx_SpecialSwiftBoltRampDownWhenIdleTask, loads the resources, creates a
 * BattleFx_SpecialSpawnSwiftBoltPerTarget callback for each actor in the target mask, plays the caster's sound,
 * clears the timeline colour channels and starts a 0xC00 ramp over ten steps.
 */
// FUNCTION: FF7 0x0053db45
void __cdecl BattleFx_SpecialBeginSwiftBolt(int targetMask, int actorIdx)
{
    Battle_AllocateResourceSlot(BattleFx_SpecialSwiftBoltRampDownWhenIdleTask);
    BattleFx_SpecialLoadResSwiftBolt();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnSwiftBoltPerTarget);
    Battle_PlaySfxPannedToActor(0x119, (short)actorIdx, 0);
    g_BattleFxTimelineState.channel2 = 0;
    g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
    g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    Battle_StartFixedPointRamp(0xC00, 10);
}

/*
 * Draws nothing: waits until g_BattleSecondaryTaskActiveCount drops to 1 or less, then ramps the fixed-point
 * value back to zero over 10 steps and frees its own slot.
 */
// FUNCTION: FF7 0x0053dbb3
void __cdecl BattleFx_SpecialSwiftBoltRampDownWhenIdleTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleSecondaryTaskActiveCount <= 1) {
        Battle_StartFixedPointRamp(0, 10);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Per-target spawner: allocates a BattleFx_SpecialSwiftBoltTimelineTask slot, stores the world position of the
 * source's bone34 in g_BattleSpecSwiftBoltTargetBonePosXY (X, Y) and g_BattleSpecSwiftBoltTargetBonePosZ,
 * the world position of the actor's targetBone in the slot's posX/posY/posZ, and the actor index.
 */
// FUNCTION: FF7 0x0053dbed
void __cdecl BattleFx_SpecialSpawnSwiftBoltPerTarget(int actor, int sourceIndex)
{
    BattleFxAtomicRayTimelineSlot *taskPtr;

    taskPtr = (BattleFxAtomicRayTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSwiftBoltTimelineTask);
    Battle_GetBoneWorldPos(sourceIndex, g_BattleActorRecords[sourceIndex].bone34, (short *)&g_BattleSpecSwiftBoltTargetBonePosXY);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &taskPtr->posX);
    taskPtr->actorIndex = actor;
}

/*
 * Timeline of one target: spawns the bolt emitter, the radial sparks, the spinning model, the fading sprites and
 * the animated sprites at fixed frames, plays the hit sound, and frees its slot at frame 0x3C.
 */
// FUNCTION: FF7 0x0053dc61
void __cdecl BattleFx_SpecialSwiftBoltTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; int depth; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offsetX; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxSwiftBoltTimelineSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_3C[0x3C]; short spawnIndex; };
        struct { unsigned char pad_to_40[0x40]; short phase; };
        struct { unsigned char pad_to_44[0x44]; BattleFxAtomicRayTimelineSlot *timelineSlot; };
    } frame;
#pragma pack(pop)

    frame.timelineSlot = (BattleFxAtomicRayTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timelineSlot->frameCounter == 0) {
        frame.spawnSlot = (BattleFxSwiftBoltTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSwiftBoltBoltEmitterTask);
        frame.spawnSlot->posX = ((BattleSpecSwiftBoltTargetIdx *)&g_BattleSpecSwiftBoltTargetBonePosXY)->targetX;
        frame.spawnSlot->posY = 0;
        frame.spawnSlot->posZ = g_BattleSpecSwiftBoltTargetBonePosZ;
        frame.spawnSlot->velocityX = (frame.timelineSlot->posX - ((BattleSpecSwiftBoltTargetIdx *)&g_BattleSpecSwiftBoltTargetBonePosXY)->targetX) / 0xA;
        frame.spawnSlot->velocityZ = (frame.timelineSlot->posZ - g_BattleSpecSwiftBoltTargetBonePosZ) / 10;
        frame.spawnSlot->velocityY = 0;
    }
    if (frame.timelineSlot->frameCounter == 10) {
        for (frame.spawnIndex = 0; frame.spawnIndex < 12; frame.spawnIndex++) {
            frame.spawnSlot = (BattleFxSwiftBoltTimelineSpawnSlot *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialSwiftBoltRadialSparkSpriteTask)];
            frame.spawnSlot->posX = frame.timelineSlot->posX;
            frame.spawnSlot->posY = 0;
            frame.spawnSlot->posZ = frame.timelineSlot->posZ;
            frame.phase = (-rand() % 1024);
            frame.spawnSlot->phase = frame.phase;
            frame.offsetX.vx = CosFixed(frame.phase) * 0xA0 >> 12;
            frame.offsetX.vy = SinFixed(frame.phase) * 0xA0 >> 12;
            frame.offsetX.vz = 0;
            frame.rotX.vz = 0;
            frame.rotX.vx = frame.rotX.vz;
            frame.rotX.vy = frame.spawnIndex << 8;
            frame.spawnSlot->variant = frame.rotX.vy;
            RotMatrix(&frame.rotX, &frame.rotMatrix);
            frame.rotMatrix.t[2] = 0;
            frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
            frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
            SetRotMatrix(&frame.rotMatrix);
            SetTransMatrix(&frame.rotMatrix);
            RotTransPointToInt16(&frame.offsetX, &frame.spawnSlot->velocityX, &frame.depth);
        }
    }
    if (frame.timelineSlot->frameCounter >= 10 && frame.timelineSlot->frameCounter < 22 && (frame.timelineSlot->frameCounter % 4) == 0) {
        frame.phase = frame.timelineSlot->frameCounter - 0xA >> 2;
        frame.spawnSlot = (BattleFxSwiftBoltTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSwiftBoltGrowingSpinningModelTask);
        frame.spawnSlot->posX = frame.timelineSlot->posX + (SinFixed(frame.phase * 1365) * 0x258 >> 12);
        frame.spawnSlot->posY = 0;
        frame.spawnSlot->posZ = frame.timelineSlot->posZ + (CosFixed(frame.phase * 1365) * 0x258 >> 12);
    }
    if (frame.timelineSlot->frameCounter > 10 && frame.timelineSlot->frameCounter <= 13) {
        for (frame.spawnIndex = 0; frame.spawnIndex < 2; frame.spawnIndex++) {
            frame.spawnSlot = (BattleFxSwiftBoltTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSwiftBoltGrowingFadeSpriteTask);
            frame.spawnSlot->posX = frame.timelineSlot->posX + rand() % 1600 + -800;
            frame.spawnSlot->posY = -rand() % 500;
            frame.spawnSlot->posZ = frame.timelineSlot->posZ + rand() % 1600 + -800;
        }
        Battle_PlaySfxPannedToActor(0x7C, (short)frame.timelineSlot->actorIndex, 0);
    }
    if (frame.timelineSlot->frameCounter > 26 && frame.timelineSlot->frameCounter < 34) {
        frame.spawnSlot = (BattleFxSwiftBoltTimelineSpawnSlot *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialSwiftBoltAnimatedSpriteTask)];
        frame.spawnSlot->posX = frame.timelineSlot->posX + rand() % 1200 + -600;
        frame.spawnSlot->posY = frame.timelineSlot->posY + -rand() % 500 + -200;
        frame.spawnSlot->posZ = frame.timelineSlot->posZ + rand() % 1200 + -600;
        frame.spawnSlot->variant = (rand() % 2) + 2;
        frame.spawnSlot = (BattleFxSwiftBoltTimelineSpawnSlot *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialSwiftBoltAnimatedSpriteTask)];
        frame.spawnSlot->posX = frame.timelineSlot->posX + rand() % 1200 + -600;
        frame.spawnSlot->posY = frame.timelineSlot->posY + -rand() % 500 + -200;
        frame.spawnSlot->posZ = frame.timelineSlot->posZ + rand() % 1200 + -600;
        frame.spawnSlot->variant = (rand() % 2);
    }
    if (frame.timelineSlot->frameCounter == 30) {
        Battle_PostActorHitOrHurtMessage(frame.timelineSlot->actorIndex);
    }
    (frame.timelineSlot->frameCounter)++;
    if (frame.timelineSlot->frameCounter == 60) {
        frame.timelineSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animated sprite in the secondary slot array. Billboards the slot's position at scale 0x2000, fills
 * g_BattleSpecSwiftBoltSpriteReq1 from the g_BattleSpecSwiftBoltModelTable entry that g_BattleSpecSwiftBoltAnimFrameIndices
 * selects with animIndex, and submits it to g_BattleOverlayDrawListHead. When unpaused it advances frameIndex and
 * frees the slot at the sprite's frame count; the resource is not checked for null.
 */
// FUNCTION: FF7 0x0053e1a1
void __cdecl BattleFx_SpecialSwiftBoltAnimatedSpriteTask(void)
{
    BattleFxSwiftBoltAnimatedSpriteSlot *task;

    task = (BattleFxSwiftBoltAnimatedSpriteSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    BattleFx_InstallBillboardMatrix(&task->posX, 0x2000 /* 2.0 */, 0);
    g_BattleSpecSwiftBoltSpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(((int *)&g_BattleSpecSwiftBoltAnimFrameIndices)[task->animIndex], g_BattleSpecSwiftBoltModelTable);
    (g_BattleSpecSwiftBoltSpriteReq1.spriteResource) = Gfx_ModelTableGetLoadedRecord(((int *)&g_BattleSpecSwiftBoltAnimFrameIndices)[task->animIndex], g_BattleSpecSwiftBoltModelTable);
    g_BattleSpecSwiftBoltSpriteReq1.frame = task->frameIndex;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecSwiftBoltSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frameIndex)++;
        if (task->frameIndex == *(int *)((char *)*(int *)(g_BattleSpecSwiftBoltSpriteReq1.spriteResource) + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Grows and fades one sprite: scale starts at 0x2000 and gains 0xC0 per frame, the drawn frame index is
 * clamped to 7, the colour bytes of g_BattleSpecSwiftBoltSpriteReq2 are 0x60 - (frame << 2), and the sprite is
 * entry 0xB of g_BattleSpecSwiftBoltModelTable. Submits to g_BattleSpecSwiftBoltDrawListHead; unless the battle is
 * paused it frees the slot at frame 0x18.
 */
// FUNCTION: FF7 0x0053e282
void __cdecl BattleFx_SpecialSwiftBoltGrowingFadeSpriteTask(void)
{
    BattleFxSwiftBoltGrowingFadeSpriteSlot *taskPtr;
    unsigned short spriteFrame;
    int cap;

    taskPtr = (BattleFxSwiftBoltGrowingFadeSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (taskPtr->frameCounter < 8) {
        cap = taskPtr->frameCounter;
    } else {
        cap = 7;
    }
    spriteFrame = cap;
    BattleFx_InstallBillboardMatrix(&taskPtr->posX, taskPtr->frameCounter * 0xC0 + 0x2000, 0);
    g_BattleSpecSwiftBoltSpriteReq2.frame = spriteFrame;
    g_BattleSpecSwiftBoltSpriteReq2.frameByte10 = 0x60 - (taskPtr->frameCounter << 2);
    g_BattleSpecSwiftBoltSpriteReq2.colorG = g_BattleSpecSwiftBoltSpriteReq2.frameByte10;
    g_BattleSpecSwiftBoltSpriteReq2.colorR = g_BattleSpecSwiftBoltSpriteReq2.colorG;
    g_BattleSpecSwiftBoltSpriteReq2.drawObj = Gfx_ModelTableGetSpriteObject(11, g_BattleSpecSwiftBoltModelTable);
    (g_BattleSpecSwiftBoltSpriteReq2.spriteField) = Gfx_ModelTableGetLoadedRecord(11, g_BattleSpecSwiftBoltModelTable);
    g_BattleSpecSwiftBoltDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecSwiftBoltSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecSwiftBoltDrawListHead);
    if (g_BattleIsPaused == 0) {
        (taskPtr->frameCounter)++;
        if (taskPtr->frameCounter == 24) {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/* Spark sprite that flies along its velocity for 8 frames, fading, and is oriented by the slot's angles. */
// FUNCTION: FF7 0x0053e3a3
void __cdecl BattleFx_SpecialSwiftBoltRadialSparkSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix scaleMatrix; };
        struct { unsigned char pad_to_48[0x48]; short axisIndex; };
        struct { unsigned char pad_to_4C[0x4C]; int depth; };
        struct { unsigned char pad_to_50[0x50]; BattleFxSwiftBoltRadialSparkSpriteSlot *sparkSlot; };
    } frame;
#pragma pack(pop)

    frame.sparkSlot = (BattleFxSwiftBoltRadialSparkSpriteSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.scaleMatrix.m[2][frame.axisIndex] = 0;
        frame.scaleMatrix.m[1][frame.axisIndex] = 0;
        frame.scaleMatrix.m[0][frame.axisIndex] = 0;
    }
    frame.rotX.vz = frame.sparkSlot->angleZ + 0x400;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.sparkSlot->angleY;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint(&frame.sparkSlot->posX, frame.scaleMatrix.t, &frame.depth);
    frame.scaleMatrix.m[2][2] = 0x6000 /* 6.0 */;
    frame.scaleMatrix.m[1][1] = frame.scaleMatrix.m[2][2];
    frame.scaleMatrix.m[0][0] = frame.scaleMatrix.m[1][1];
    ConcatMatrixIntoFirst(&frame.scaleMatrix, &g_BattleCameraTransform);
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.scaleMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.scaleMatrix);
    SetTransMatrix(&frame.scaleMatrix);
    g_BattleSpecSwiftBoltSpriteReq4.colorB = 0x80 - (frame.sparkSlot->frameCounter << 4);
    g_BattleSpecSwiftBoltSpriteReq4.colorG = g_BattleSpecSwiftBoltSpriteReq4.colorB;
    g_BattleSpecSwiftBoltSpriteReq4.colorR = g_BattleSpecSwiftBoltSpriteReq4.colorG;
    g_BattleSpecSwiftBoltSpriteReq4.drawObj = Gfx_ModelTableGetSpriteObject(10, g_BattleSpecSwiftBoltModelTable);
    (g_BattleSpecSwiftBoltSpriteReq4.spriteField) = Gfx_ModelTableGetLoadedRecord(10, g_BattleSpecSwiftBoltModelTable);
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecSwiftBoltSpriteReq4, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.sparkSlot->posX += frame.sparkSlot->velocityX;
    frame.sparkSlot->posY += frame.sparkSlot->velocityY;
    frame.sparkSlot->posZ += frame.sparkSlot->velocityZ;
    (frame.sparkSlot->frameCounter)++;
    if (frame.sparkSlot->frameCounter == 8) {
        frame.sparkSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Model that spins about Y and grows (scale from g_BattleSpecSwiftBoltModelScaleX/Y/Z) for 30 frames while fading. */
// FUNCTION: FF7 0x0053e5a1
void __cdecl BattleFx_SpecialSwiftBoltGrowingSpinningModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[80];
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxOilTargetModelSlot *sanderSlot; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.sanderSlot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.sanderSlot->frameCounter << 6;
    frame.rotX.vz = 0;
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    g_BattleSpecSwiftBoltModelScaleZ = (frame.sanderSlot->frameCounter << 8) + 0x3000;
    g_BattleSpecSwiftBoltModelScaleX = g_BattleSpecSwiftBoltModelScaleZ;
    g_BattleSpecSwiftBoltModelScaleY = (frame.sanderSlot->frameCounter << 8) + 0x2000;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecSwiftBoltModelScaleX);
    frame.rotMatrix.t[0] = frame.sanderSlot->posX;
    frame.rotMatrix.t[1] = frame.sanderSlot->posY;
    frame.rotMatrix.t[2] = frame.sanderSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecSwiftBoltModelReq2.alpha = 0x80 - (frame.sanderSlot->frameCounter << 2);
    (g_BattleSpecSwiftBoltModelReq2.modelField) = Gfx_ModelTableGetRenderObject(1, g_BattleSpecSwiftBoltModelTable);
    g_BattleSpecSwiftBoltDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecSwiftBoltModelReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecSwiftBoltDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.sanderSlot->frameCounter)++;
    if (frame.sanderSlot->frameCounter == 30) {
        frame.sanderSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Bolt emitter: when unpaused emits BattleFx_SpecialSwiftBoltDrawBoltSegmentTask with parent X/Z in child velocity fields and variant counter
 * modulo four plus three, then integrates parent X/Z. On even counters emits BattleFx_SpecialSwiftBoltBouncingSpriteTask there
 * with random radial velocity magnitude 100, negative Y velocity 100 through 299 and lifetime four
 * through nineteen. Advances the parent counter and retires exactly at fourteen.
 */
// FUNCTION: FF7 0x0053e6fc
void __cdecl BattleFx_SpecialSwiftBoltBoltEmitterTask(void)
{
    BattleFxSwiftBoltBoltEmitterRecord *taskPtr;
    short angle;
    BattleFxSwiftBoltBoltSegment *segmentPtr;

    taskPtr = (BattleFxSwiftBoltBoltEmitterRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    segmentPtr = (BattleFxSwiftBoltBoltSegment *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSwiftBoltDrawBoltSegmentTask);
    segmentPtr->cosOffset = taskPtr->posA;
    segmentPtr->sinOffset = taskPtr->posB;
    segmentPtr->startOffset = 0;
    segmentPtr->stepCount = taskPtr->emitIndex % 4 + 3;
    taskPtr->posA += taskPtr->stepA;
    taskPtr->posB += taskPtr->stepB;
    if (taskPtr->emitIndex % 2 == 0) {
        segmentPtr = (BattleFxSwiftBoltBoltSegment *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSwiftBoltBouncingSpriteTask);
        segmentPtr->posA = taskPtr->posA;
        segmentPtr->posB = taskPtr->posB;
        segmentPtr->posY = 0;
        angle = rand() % 0x1000;
        segmentPtr->cosOffset = CosFixed(angle) * 0x64 >> 12;
        segmentPtr->sinOffset = SinFixed(angle) * 0x64 >> 12;
        segmentPtr->startOffset = -rand() % 200 - 100;
        segmentPtr->stepCount = rand() % 16 + 4;
    }
    (taskPtr->emitIndex)++;
    if (taskPtr->emitIndex == 14) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Sprite that spins and bounces on the ground plane (Y = 0) for 15 frames. */
// FUNCTION: FF7 0x0053e8b1
void __cdecl BattleFx_SpecialSwiftBoltBouncingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int frameValue;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; short frameIndex; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix scaleMatrix; };
        struct { unsigned char pad_to_50[0x50]; short axis; };
        struct { unsigned char pad_to_54[0x54]; int screenDepth; };
        struct { unsigned char pad_to_58[0x58]; BattleFxSwiftBoltBouncingSpriteSlot *bounceSlot; };
    } frame;
#pragma pack(pop)

    frame.bounceSlot = (BattleFxSwiftBoltBouncingSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    for (frame.axis = 0; frame.axis < 3; frame.axis++) {
        frame.scaleMatrix.m[2][frame.axis] = 0;
        frame.scaleMatrix.m[1][frame.axis] = 0;
        frame.scaleMatrix.m[0][frame.axis] = 0;
    }
    if (frame.bounceSlot->frameCounter < 7) {
        frame.frameValue = 8;
    } else {
        frame.frameValue = 15 - frame.bounceSlot->frameCounter;
    }
    frame.frameIndex = *(short *)&frame.frameValue;
    frame.rotX.vy = 0;
    frame.rotX.vx = frame.rotX.vy;
    frame.rotX.vz = frame.bounceSlot->spinAngle;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint(&frame.bounceSlot->posX, frame.scaleMatrix.t, &frame.screenDepth);
    frame.scaleMatrix.m[2][2] = frame.bounceSlot->variant < 4 ? 0x1400 : 0x2000;
    frame.scaleMatrix.m[1][1] = frame.scaleMatrix.m[2][2];
    frame.scaleMatrix.m[0][0] = frame.scaleMatrix.m[1][1];
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.scaleMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.scaleMatrix);
    SetTransMatrix(&frame.scaleMatrix);
    g_BattleSpecSwiftBoltSpriteReq3.frame = frame.bounceSlot->variant;
    g_BattleSpecSwiftBoltSpriteReq3.code = (frame.frameIndex - 8 != 0 ? 2 : 0) + 0x2C;
    g_BattleSpecSwiftBoltSpriteReq3.frameByte10 = frame.frameIndex << 4;
    g_BattleSpecSwiftBoltSpriteReq3.colorG = g_BattleSpecSwiftBoltSpriteReq3.frameByte10;
    g_BattleSpecSwiftBoltSpriteReq3.colorR = g_BattleSpecSwiftBoltSpriteReq3.colorG;
    g_BattleSpecSwiftBoltSpriteReq3.drawObj = Gfx_ModelTableGetSpriteObject(5, g_BattleSpecSwiftBoltModelTable);
    (g_BattleSpecSwiftBoltSpriteReq3.spriteField) = Gfx_ModelTableGetLoadedRecord(5, g_BattleSpecSwiftBoltModelTable);
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecSwiftBoltSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.bounceSlot->posX += frame.bounceSlot->velocityX;
    frame.bounceSlot->posY += frame.bounceSlot->velocityY;
    frame.bounceSlot->posZ += frame.bounceSlot->velocityZ;
    if (frame.bounceSlot->posY > 0) {
        frame.bounceSlot->posY = 0;
        frame.bounceSlot->velocityY *= -1;
    }
    frame.bounceSlot->velocityY += 20;
    frame.bounceSlot->spinAngle += 0x200 /* 45 deg */;
    (frame.bounceSlot->frameCounter)++;
    if (frame.bounceSlot->frameCounter == 15) {
        frame.bounceSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws one bolt segment from the target bone position toward its own position as a stretched sprite, plus a
 * flat fading model at its position, for 4 frames.
 */
// FUNCTION: FF7 0x0053eb44
void __cdecl BattleFx_SpecialSwiftBoltDrawBoltSegmentTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[128];
        PsxMatrix boltMatrix;
        struct { unsigned char pad_to_20[0x20]; SVECTOR dirX; };
        struct { unsigned char pad_to_28[0x28]; BattleFxSwiftBoltDrawBoltSegmentSlot *boltSlot; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix lookBasis; };
        struct { unsigned char pad_to_4C[0x4C]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_6C[0x6C]; short distance; };
        struct { unsigned char pad_to_70[0x70]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.boltSlot = (BattleFxSwiftBoltDrawBoltSegmentSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.dirX.vx = frame.boltSlot->posX - ((BattleSpecSwiftBoltTargetIdx *)&g_BattleSpecSwiftBoltTargetBonePosXY)->targetX;
    frame.dirX.vy = frame.boltSlot->posY - ((BattleSpecSwiftBoltTargetIdx *)&g_BattleSpecSwiftBoltTargetBonePosXY)->targetY;
    frame.dirX.vz = frame.boltSlot->posZ - g_BattleSpecSwiftBoltTargetBonePosZ;
    frame.distance = SqrtToInt(frame.dirX.vx * frame.dirX.vx + frame.dirX.vy * frame.dirX.vy + frame.dirX.vz * frame.dirX.vz);
    Gte_BuildLookBasisFromDirection(&frame.dirX.vx, frame.lookBasis.m[0]);
    RotMatrixXYZ(&g_BattleSpecSwiftBoltOffsets1, &frame.boltMatrix);
    ConcatMatrixPreservingTranslation(&frame.lookBasis, &frame.boltMatrix, &frame.boltMatrix);
    frame.scaleX.vx = (frame.distance << 12) / 2000;
    frame.scaleX.vz = 0x2000 /* 2.0 */;
    frame.scaleX.vy = frame.scaleX.vz;
    ScaleMatrix(&frame.boltMatrix, &frame.scaleX);
    frame.boltMatrix.t[0] = ((BattleSpecSwiftBoltTargetIdx *)&g_BattleSpecSwiftBoltTargetBonePosXY)->targetX;
    frame.boltMatrix.t[1] = ((BattleSpecSwiftBoltTargetIdx *)&g_BattleSpecSwiftBoltTargetBonePosXY)->targetY;
    frame.boltMatrix.t[2] = g_BattleSpecSwiftBoltTargetBonePosZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.boltMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecSwiftBoltSpriteReq5.colorB = 0x80 - (frame.boltSlot->frameCounter << 5);
    g_BattleSpecSwiftBoltSpriteReq5.colorG = g_BattleSpecSwiftBoltSpriteReq5.colorB;
    g_BattleSpecSwiftBoltSpriteReq5.colorR = g_BattleSpecSwiftBoltSpriteReq5.colorG;
    g_BattleSpecSwiftBoltSpriteReq5.frame = frame.boltSlot->spriteFrame;
    g_BattleSpecSwiftBoltSpriteReq5.drawObj = Gfx_ModelTableGetSpriteObject(2, g_BattleSpecSwiftBoltModelTable);
    (g_BattleSpecSwiftBoltSpriteReq5.resource) = Gfx_ModelTableGetLoadedRecord(2, g_BattleSpecSwiftBoltModelTable);
    if ((g_BattleSpecSwiftBoltSpriteReq5.resource) != 0) {
        ((BattleFxMagicDemi3MultiDrawReq *)(g_BattleSpecSwiftBoltSpriteReq5.resource))->drawObj0 = Gfx_ModelTableGetSpriteObject(2, g_BattleSpecSwiftBoltModelTable);
        ((BattleFxMagicDemi3MultiDrawReq *)(g_BattleSpecSwiftBoltSpriteReq5.resource))->drawObj1 = Gfx_ModelTableGetSpriteObject(3, g_BattleSpecSwiftBoltModelTable);
        ((BattleFxMagicDemi3MultiDrawReq *)(g_BattleSpecSwiftBoltSpriteReq5.resource))->drawObj2 = Gfx_ModelTableGetSpriteObject(4, g_BattleSpecSwiftBoltModelTable);
    }
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecSwiftBoltSpriteReq5, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    frame.dirX.vx = frame.boltSlot->posX - ((BattleSpecSwiftBoltTargetIdx *)&g_BattleSpecSwiftBoltTargetBonePosXY)->targetX;
    frame.dirX.vy = 0;
    frame.dirX.vz = frame.boltSlot->posZ - g_BattleSpecSwiftBoltTargetBonePosZ;
    Gte_BuildLookBasisFromDirection(&frame.dirX.vx, frame.lookBasis.m[0]);
    RotMatrix((SVECTOR *)&g_BattleSpecSwiftBoltOffsets2, &frame.boltMatrix);
    ConcatMatrixPreservingTranslation(&frame.lookBasis, &frame.boltMatrix, &frame.boltMatrix);
    frame.scaleX.vz = 0x1000 - (frame.boltSlot->frameCounter << 9);
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.boltMatrix, &frame.scaleX);
    frame.boltMatrix.t[0] = frame.boltSlot->posX;
    frame.boltMatrix.t[1] = 0;
    frame.boltMatrix.t[2] = frame.boltSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.boltMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecSwiftBoltModelReq1.alpha = 0x60 - frame.boltSlot->frameCounter * 24;
    (g_BattleSpecSwiftBoltModelReq1.modelField) = Gfx_ModelTableGetRenderObject(0, g_BattleSpecSwiftBoltModelTable);
    g_BattleSpecSwiftBoltDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecSwiftBoltModelReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecSwiftBoltDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.boltSlot->frameCounter)++;
    if (frame.boltSlot->frameCounter == 4) {
        frame.boltSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
