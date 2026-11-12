// src/ff7/src/battle/battle_fx_magic_death.cpp
/* Magic Effect [24]: Death (Japanese: デス, dev tag: "MAGIC/DETH" / Deth) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Deth magic effect: places the effect, runs linear and curved Gouraud quad strip trails
//   that follow an actor attachment point, and scatters and gathers spiral sprites around the
//   target.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_magic.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include "ff7/camera_script.h"
#include <string.h>
#include <stdlib.h>
extern "C" {

// clang-format off
// GLOBAL: FF7 0x008a8af8
int g_BattleMagicDeathResourceDef = 0x00192A39;
int g_BattleMagicDeathResourceDefHeadPtr[1] = {
    (int)&g_BattleFxMagicDeathResourceNames[0]
};
int g_BattleMagicDeathResourceDefDataTail[4] = {
    (int)&g_BattleFxMagicDeathResourceNames[0xc], 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x008a8b10
BattleFxMagicDeathSpriteDrawReq g_BattleMagicDeathSpriteDrawReq1 = {{0, 0x32, -470}, {0}, {0, 0, -4096}, {0, 0, 0x30, 0x30, 0x30, 0x2e, 0, 0}};
// GLOBAL: FF7 0x008a8b38
BattleFxSpriteRequest g_BattleMagicDeathSpriteColor = {0, 0, 0x30, 0x30, 0x30, 0x2e, 0, 0};
// GLOBAL: FF7 0x008a8b48
BattleFxSpriteRequest g_BattleMagicDeathSpriteDrawReq2 = {0, 0, 0x30, 0x30, 0x30, 0x2e, 0, 0};
unsigned char g_BattleFxMagicDeathResourceNames[24] =
    "SMOKE.TIM\0\0\0"
    "SMOKE_1.SPT";
// GLOBAL: FF7 0x00b73fd8
int g_BattleMagicDeathDeltaX = 0;
// GLOBAL: FF7 0x00b73fdc
int g_BattleMagicDeathDeltaY = 0;
// GLOBAL: FF7 0x00b73fe0
int g_BattleMagicDeathDeltaZ = 0;
unsigned char g_bssPad_00b73fe4[4] = {0};
// GLOBAL: FF7 0x00b73fe8
BattleMagicDeathSmokeRing g_BattleMagicDeathWorkingBlob1 = {0};
unsigned char g_bssPad_00b73ffc[4] = {0};
unsigned char g_bssPad_00b74000[1136] = {0};
// GLOBAL: FF7 0x00b74470
int g_BattleMagicDeathTargetMask = 0;
unsigned char g_bssPad_00b74474[4] = {0};
// GLOBAL: FF7 0x00b74478
struct BattleFxDeathTrailEdge g_BattleMagicDeathTrailWorkingBlob1[17] = {0};
unsigned char g_bssPad_00b74588[1776] = {0};
// GLOBAL: FF7 0x00b74c78
int g_BattleMagicDeathOriginActor = 0;
unsigned char g_bssPad_00b74c7c[4] = {0};
// GLOBAL: FF7 0x00b74c80
struct BattleMagicDeathTrailPoint g_BattleMagicDeathTrailSubdivisionPoints[9] = {0};
unsigned char g_bssPad_00b74cc8[56] = {0};
// GLOBAL: FF7 0x00b74d00
int g_BattleMagicDeathStepFlag = 0;
// GLOBAL: FF7 0x00b74d04
int g_BattleMagicDeathStateParam1 = 0;
// GLOBAL: FF7 0x00b74d08
int g_BattleMagicDeathStateParam2 = 0;
unsigned char g_bssPad_00b74d0c[4] = {0};
// GLOBAL: FF7 0x00b74d10
struct BattleFxDeathTrailStep g_BattleMagicDeathTrailData[16] = {0};
// GLOBAL: FF7 0x00b74f10
struct BattleMagicDeathTarget g_BattleMagicDeathTargetPos = {0};
// GLOBAL: FF7 0x00b74f18
short g_BattleMagicDeathCentroidX = 0;
// GLOBAL: FF7 0x00b74f1c
short g_BattleMagicDeathCentroidZ = 0;
// GLOBAL: FF7 0x00b74f20
struct GfxTableCursor *g_BattleMagicDeathLoadedRes = 0;
unsigned char g_bssPad_00b74f24[4] = {0};
// GLOBAL: FF7 0x00b74f28
short g_BattleMagicDeathAttachPos[3] = {0};
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)



/*
 * Loads resources for the Deth magic effect when not already loaded.
 */
// FUNCTION: FF7 0x00561b90
void __cdecl BattleFx_MagicLoadResDeath(void)
{
    GfxModelTableBank *sceneBase;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int stepResourceDef;

    if (g_pBattle3dScene != 0 && g_BattleMagicDeathLoadedRes == 0) {
        kind = 1;
        stepResourceDef = (int)&g_BattleMagicDeathResourceDef;
        sceneBase = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a8b70
        pRequest->resourcePath = BattleFx_BuildResourcePath("magic/deth", pathText);
        Gfx_LoadModelTableIfAbsent(kind, stepResourceDef, pRequest, &g_BattleMagicDeathLoadedRes, sceneBase);
    }
}

/*
 * Entry point for the Deth magic effect.
 */
// FUNCTION: FF7 0x00561c20
int __cdecl BattleFx_MagicEnterDeath(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResDeath();
    BattleFx_MagicBeginDeath(targetMask, actorIdx);
    return 0;
}

// Stores the allocated BattleFx_SelectDrawListBuffer slot address in g_BattleFxDrawListBufferTaskSlotPtr, allocates a
// slot for BattleFx_MagicDeathPlaceEffectTask, stores both arguments in g_BattleMagicDeathTargetMask and g_BattleMagicDeathOriginActor, and clears
// g_BattleMagicDeathWorkingBlob1.
// FUNCTION: FF7 0x00561c3c
void __cdecl BattleFx_MagicBeginDeath(int targetMask, int actorIdx)
{
    g_BattleFxDrawListBufferTaskSlotPtr = (unsigned char *)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    Battle_AllocateResourceSlot(BattleFx_MagicDeathPlaceEffectTask);
    g_BattleMagicDeathTargetMask = targetMask;
    g_BattleMagicDeathOriginActor = actorIdx;
    g_BattleMagicDeathWorkingBlob1.writePos = 0;
}

// Places the death-magic effect. It counts the actors in g_BattleMagicDeathTargetMask and keeps the
// lowest-indexed one as targetActor, then retires the current slot (ownerSlot = BATTLE_TASK_FREE). With
// exactly one target and an origin actor whose effectCategory is not 0x14, it allocates a
// BattleFx_MagicDeathTimelineTask slot at the target's altBone position, with flag 0 and the
// target's yaw as its angle. Otherwise it allocates one at the origin's altBone with flag 1, aims
// its delta at the centroid of the target mask, and stores the angle between that delta and the
// reference vector in g_BattleMagicDeathSpriteDrawReq1. spawnIndex comes from the actor's radius plus
// 0xE0 or 0x1C0, clamped to 0x76C.
// FUNCTION: FF7 0x00561c87
void __cdecl BattleFx_MagicDeathPlaceEffectTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxDeathPlaceEffectSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_24[0x24]; SVECTOR offsetX; };
        struct { unsigned char pad_to_2C[0x2C]; int targetActor; };
        struct { unsigned char pad_to_30[0x30]; int targetCount; };
        struct { unsigned char pad_to_34[0x34]; int actorIndex; };
        struct { unsigned char pad_to_38[0x38]; int spawnDistance; };
        struct { unsigned char pad_to_3C[0x3C]; SVECTOR angleX; };
        struct { unsigned char pad_to_44[0x44]; VECTOR outVec; };
    } frame;
#pragma pack(pop)

    frame.actorIndex = 0;
    for (frame.targetCount = 0; frame.actorIndex < 10; frame.actorIndex++) {
        if ((g_BattleMagicDeathTargetMask & 1 << frame.actorIndex) != 0) {
            frame.targetCount++;
            if (frame.targetCount == 1) {
                frame.targetActor = frame.actorIndex;
            }
        }
    }
    frame.spawnSlot = (BattleFxDeathPlaceEffectSpawnSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.spawnSlot->ownerSlot = BATTLE_TASK_FREE;
    if (frame.targetCount == 1 && g_BattleActorRecords[g_BattleMagicDeathOriginActor].effectCategory != 0x14) {
        frame.spawnSlot = (BattleFxDeathPlaceEffectSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDeathTimelineTask);
        Battle_GetBoneWorldPos(frame.targetActor, g_BattleActorRecords[frame.targetActor].altBone, &frame.spawnSlot->posX);
        frame.spawnDistance = g_BattleActorRecords[frame.targetActor].radius + 224;
        if (frame.spawnDistance > 1900) {
            frame.spawnDistance = 1900;
        }
        frame.spawnSlot->angleBetweenVectors = g_BattleActorRecords[frame.targetActor].yaw;
        frame.spawnSlot->spawnIndex = *(short *)&frame.spawnDistance;
        frame.spawnSlot->stage = 0;
        frame.spawnSlot->flag = 0;
    } else {
        frame.spawnSlot = (BattleFxDeathPlaceEffectSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDeathTimelineTask);
        Battle_GetBoneWorldPos(g_BattleMagicDeathOriginActor, g_BattleActorRecords[g_BattleMagicDeathOriginActor].altBone, &frame.spawnSlot->posX);
        frame.spawnDistance = g_BattleActorRecords[g_BattleMagicDeathOriginActor].radius + 448;
        if (frame.spawnDistance > 1900) {
            frame.spawnDistance = 1900;
        }
        frame.spawnSlot->spawnIndex = *(short *)&frame.spawnDistance;
        frame.spawnSlot->stage = 0;
        frame.spawnSlot->flag = 1;
        Battle_GetActorMaskCentroid(g_BattleMagicDeathTargetMask, &g_BattleMagicDeathCentroidX);
        g_BattleMagicDeathDeltaX = g_BattleMagicDeathCentroidX - frame.spawnSlot->posX;
        g_BattleMagicDeathDeltaY = 0;
        g_BattleMagicDeathDeltaZ = g_BattleMagicDeathCentroidZ - frame.spawnSlot->posZ;
        NormalizeVectorFixed(&g_BattleMagicDeathDeltaX, &g_BattleMagicDeathDeltaX);
        frame.spawnSlot->angleBetweenVectors = ComputeAngleBetweenVectors(&g_BattleMagicDeathSpriteDrawReq1.reference, (VECTOR *)&g_BattleMagicDeathDeltaX, &frame.outVec);
        if (frame.outVec.vy < 0) {
            frame.spawnSlot->angleBetweenVectors = -frame.spawnSlot->angleBetweenVectors;
        }
    }
    frame.angleX.vx = 0;
    frame.angleX.vy = frame.spawnSlot->angleBetweenVectors;
    frame.angleX.vz = 0;
    RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
    frame.offsetX.vx = 0;
    frame.offsetX.vy = 0;
    frame.offsetX.vz = -frame.spawnDistance;
    TransformVectorToInt16(&frame.rotMatrix, &frame.offsetX, &frame.offsetX.vx);
    frame.spawnSlot->posX += frame.offsetX.vx;
    frame.spawnSlot->posY += (short)(frame.offsetX.vy + 300);
    frame.spawnSlot->posZ += frame.offsetX.vz;
    if (frame.spawnSlot->posY > 0) {
        frame.spawnSlot->posY = 0;
    }
    *(Int64Pair *)(&g_BattleMagicDeathTargetPos) = *(Int64Pair *)&frame.spawnSlot->posX;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition(&frame.spawnSlot->posX), 0x1A5);
}

// Per-frame timeline for the death-magic effect, skipped while g_BattleIsPaused is set. At step
// 0x1E it spawns a strip task (flag picks which one) and copies the base position and the angle
// into it. Steps 1 to 0x10 spawn a gather task and fill four entries of the 0x90-entry sprite ring
// in g_BattleMagicDeathWorkingBlob1, with random offsets around baseX and baseZ and Y set 0xA5A
// below baseY, wrapping the ring index at 0x90. Steps 6 to 0x1A spawn five spiral sprite tasks with
// random speed, angle and lifetime fields. Steps 0x15 to 0x5F spawn a scatter task and advance
// stage.
// FUNCTION: FF7 0x00561faf
void __cdecl BattleFx_MagicDeathTimelineTask(void)
{
    short ramp;
    int maxScale, i, writePos, rnd;
    BattleFxDeathState *innerTask;
    BattleFxDeathSpriteSlot *pSprite;

    if (g_BattleIsPaused != 0) {
        return;
    }
    innerTask = (BattleFxDeathState *)BATTLE_CURRENT_TASK_SLOT;
    if (innerTask->step == 30) {
        if (innerTask->flag != 0) {
            pSprite = (BattleFxDeathSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDeathCurvedStripTimelineTask);
        } else {
            pSprite = (BattleFxDeathSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDeathLinearStripTimelineTask);
        }
        *(Int64Pair *)&pSprite->posX = *(Int64Pair *)&innerTask->baseX;
        pSprite->angle = innerTask->angle;
    }
    if (innerTask->step >= 1 && innerTask->step <= 16) {
        writePos = g_BattleMagicDeathWorkingBlob1.writePos;
        pSprite = (BattleFxDeathSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDeathGatherSpritesToTarget);
        pSprite->ringStartIndex = writePos;
        maxScale = 0x12AA;
        for (i = 0; i < 4; i++) {
            rnd = rand() % 4096 + 2048;
            g_BattleMagicDeathWorkingBlob1.points[writePos].x = innerTask->baseX + rand() % 1800 + -900;
            g_BattleMagicDeathWorkingBlob1.points[writePos].y = innerTask->baseY - 2650;
            g_BattleMagicDeathWorkingBlob1.points[writePos].z = innerTask->baseZ + rand() % 1800 + -900;
            g_BattleMagicDeathWorkingBlob1.points[writePos].scale = maxScale * rnd >> 12;
            writePos++;
            if (writePos >= 144) {
                writePos = 0;
            }
        }
        g_BattleMagicDeathWorkingBlob1.writePos = writePos;
    }
    if (innerTask->step >= 6 && innerTask->step <= 26) {
        maxScale = 0x12AA;
        for (i = 0; i < 5; i++) {
            rnd = rand() % 2048 + 1792;
            pSprite = (BattleFxDeathSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDeathSpiralSpriteTask);
            *(Int64Pair *)&pSprite->posX = *(Int64Pair *)&innerTask->baseX;
            pSprite->posY -= 2550;
            pSprite->angle = (g_BattleMagicDeathWorkingBlob1.writePos + i & 0xFFF) << 6;
            pSprite->angleStep = rand() % 0x1C + 0x20;
            pSprite->scale = maxScale * rnd >> 12;
            pSprite->radialOffset = rand() % 30 + 28;
            pSprite->yStep = rand() % 60 + 220;
            pSprite->radialStep = rand() % 15 + 20;
        }
    }
    if (innerTask->step >= 21 && innerTask->step <= 95) {
        writePos = g_BattleMagicDeathWorkingBlob1.writePos;
        pSprite = (BattleFxDeathSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDeathScatterSpritesFromTarget);
        pSprite->ringStartIndex = writePos;
        maxScale = 0x12AA;
        (innerTask->stage)++;
        if (innerTask->stage >= 5) {
            innerTask->stage = 5;
        }
        for (i = 0; i < innerTask->stage; i++) {
            rnd = rand() % 3072 + 1792;
            g_BattleMagicDeathWorkingBlob1.points[writePos].x = innerTask->baseX + rand() % 1100 + -550;
            g_BattleMagicDeathWorkingBlob1.points[writePos].y = innerTask->baseY;
            g_BattleMagicDeathWorkingBlob1.points[writePos].z = innerTask->baseZ + rand() % 1100 + -550;
            g_BattleMagicDeathWorkingBlob1.points[writePos].scale = maxScale * rnd >> 12;
            writePos++;
            if (writePos >= 144) {
                writePos = 0;
            }
        }
        g_BattleMagicDeathWorkingBlob1.writePos = writePos;
    }
    if (innerTask->step < 10) {
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        ramp = innerTask->step * 204;
    } else if (innerTask->step >= 95) {
        ramp = 0x800 - (innerTask->step - 0x5F << 8);
        if (ramp < 0) {
            ramp = 0;
        }
    } else {
        ramp = 0x800;
    }
    if (innerTask->step >= 103) {
        ramp = 0;
        innerTask->ownerSlot = BATTLE_TASK_FREE;
    }
    g_BattleFxTimelineState.startValue = ramp;
    (innerTask->step)++;
}

/*
 * Strip timeline task controlling linear particle trail generation for Deth.
 */
// FUNCTION: FF7 0x005624a5
void __cdecl BattleFx_MagicDeathLinearStripTimelineTask(void)
{
    BattleFxDeathRingRecord *ringEnt;
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int r;
    int s; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int t; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int y; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int a; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int i8;
    int c; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxMagicDeathStrip *taskSlot;
    int e; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    taskSlot = (BattleFxMagicDeathStrip *)BATTLE_CURRENT_TASK_SLOT;
    if (taskSlot->step == 1) {
        ringEnt = (BattleFxDeathRingRecord *)Battle_AllocSceneMessage(1);
        *(Int64Pair *)&ringEnt->posX = *(Int64Pair *)&taskSlot->posX;
        ringEnt->angle = taskSlot->angle + 0x800 & 0xFFF;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH2, Battle_ComputeScreenPanFromPosition(&taskSlot->posX), 0x1A4);
    } else if (taskSlot->step == 55) {
        Battle_AllocSceneMessage(2);
    }
    if (taskSlot->step > 23 && taskSlot->step < 40) {
        if (taskSlot->step < 31) {
            BattleFx_MagicDeathUpdateDrawLinearStripTrail(1);
        } else {
            BattleFx_MagicDeathUpdateDrawLinearStripTrail(0);
        }
    } else if (taskSlot->step == 1) {
        g_BattleMagicDeathStepFlag = 0;
        g_BattleMagicDeathStateParam2 = 0;
        g_BattleMagicDeathStateParam1 = 0;
        for (i8 = 0; i8 < 16; i8++) {
            g_BattleMagicDeathTrailData[i8].active = 0;
        }
    }
    if (taskSlot->step == 24) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH2, Battle_ComputeScreenPanFromPosition(&taskSlot->posX), 0x1A3);
    }
    if (taskSlot->step == 40) {
        i8 = 0;
        for (r = 0; i8 < 10; i8++) {
            if ((g_BattleMagicDeathTargetMask & 1 << i8) != 0) {
                Battle_PostActorHitOrHurtMessage(i8);
                if ((g_BattleTimelineTargetMask & 1 << i8) != 0) {
                    r++;
                }
            }
        }
        if (r != 0) {
            Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH2, Battle_ComputeScreenPanFromPosition(&taskSlot->posX), 0x1A8);
        }
    }
    if (taskSlot->step >= 65) {
        taskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    (taskSlot->step)++;
}

/*
 * Updates and renders the linear quad strip trail for Deth.
 */
// FUNCTION: FF7 0x005626b4
void __cdecl BattleFx_MagicDeathUpdateDrawLinearStripTrail(int updateFlag)
{
    int attachPos[2];
    int spritePosition[2];
    int stepIdx;
    BattleFxDeathStripTrail *stripTrailPtr;
    int oldIndex;
    int length;
    int padWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int numSegments;
    int stepNum;

    stripTrailPtr = (BattleFxDeathStripTrail *)Gfx_GetPsxScratchpadPtr(0);
    BattleFx_MagicDeathTransformAttachPoint(3, 6, g_BattleMagicDeathAttachPos, (short *)attachPos);
    BattleFx_MagicDeathTransformAttachPoint(3, 6, g_BattleMagicDeathSpriteDrawReq1.attachPos, (short *)spritePosition);
    if (updateFlag != 0) {
        stepIdx = g_BattleMagicDeathStepFlag;
        oldIndex = g_BattleMagicDeathStateParam2;
        length = g_BattleMagicDeathStateParam1;
        *(Int64Pair *)&g_BattleMagicDeathTrailData[stepIdx].startPos = *(Int64Pair *)(attachPos);
        *(Int64Pair *)&g_BattleMagicDeathTrailData[stepIdx].endPos = *(Int64Pair *)(spritePosition);
        g_BattleMagicDeathTrailData[stepIdx].active = 1;
        stepIdx++;
        length++;
        if (length >= 8) {
            length = 8;
        }
        if (stepIdx >= 8) {
            stepIdx = 0;
        }
        if (stepIdx == oldIndex) {
            g_BattleMagicDeathTrailData[oldIndex].active = 0;
            oldIndex++;
            length--;
            if (length < 0) {
                length = 0;
            }
            if (oldIndex >= 8) {
                oldIndex = 0;
            }
        }
        g_BattleMagicDeathStepFlag = stepIdx;
        g_BattleMagicDeathStateParam2 = oldIndex;
        g_BattleMagicDeathStateParam1 = length;
    } else if (g_BattleMagicDeathStateParam2 != g_BattleMagicDeathStepFlag) {
        oldIndex = g_BattleMagicDeathStateParam2;
        g_BattleMagicDeathTrailData[oldIndex].active = 0;
        oldIndex++;
        if (oldIndex >= 8) {
            oldIndex = 0;
        }
        g_BattleMagicDeathStateParam2 = oldIndex;
    }
    SetRotMatrix(g_BattleCameraTransform.matrix);
    SetTransMatrix(g_BattleCameraTransform.matrix);
    RefreshGlobalTransformState();
    SetDrawColorRGB(0x68, 0x28, 0x28);
    stripTrailPtr->colRemaining = 16;
    stripTrailPtr->shadeStartColor = 0x10;
    length = g_BattleMagicDeathStateParam2;
    numSegments = g_BattleMagicDeathStateParam1;
    BattleFx_MagicDeathProjectStripEdgePoints(stripTrailPtr, &g_BattleMagicDeathTrailData[length].startPos, &g_BattleMagicDeathTrailData[length].endPos);
    stepIdx = length;
    for (stepNum = 1;; stepIdx++, stepNum++) {
        if (stepIdx >= 8) {
            stepIdx = 0;
        }
        oldIndex = stepIdx + 1;
        if (oldIndex >= 8) {
            oldIndex = 0;
        }
        if (g_BattleMagicDeathTrailData[oldIndex].active == 0) {
            return;
        }
        stripTrailPtr->prevScreenPoint[0] = stripTrailPtr->screenPoint[0];
        stripTrailPtr->prevScreenPoint[1] = stripTrailPtr->screenPoint[1];
        stripTrailPtr->prevNdcZ[0] = stripTrailPtr->ndcZ[0];
        stripTrailPtr->prevNdcZ[1] = stripTrailPtr->ndcZ[1];
        stripTrailPtr->prevNdcW[0] = stripTrailPtr->ndcW[0];
        stripTrailPtr->prevNdcW[1] = stripTrailPtr->ndcW[1];
        stripTrailPtr->prevDepth = stripTrailPtr->depth;
        stripTrailPtr->rColor = stripTrailPtr->colRemaining;
        BattleFx_MagicDeathProjectStripEdgePoints(stripTrailPtr, &g_BattleMagicDeathTrailData[oldIndex].startPos, &g_BattleMagicDeathTrailData[oldIndex].endPos);
        BattleFx_MagicDeathDrawPsxGouraudQuad(stripTrailPtr, stepNum, numSegments);
    }
}

// Draws one Gouraud-shaded quad for one segment of segmentCount steps along the trail.
// Gfx_ShadePsxVertexColor(shadeStartRgb, 0x1000 / segmentCount * segment, colRemainingRgb) interpolates
// the colour pair as a 4.12 fraction. The quad takes its corners from the first two screen points and
// the first two previous screen points, coloured colRemaining on the left pair and rColor on the right,
// with blend factors from the first two ndcZ/ndcW and prevNdcZ/prevNdcW entries. The tag top byte is
// set to 0x08 and the primitive code byte (packet byte 7) to 0x3A, then
// Gfx_DrawPsxPrimitive draws it and the packet is left in g_BattleFxActiveDrawListPtr. An averaged
// depth from prevDepth and depth is computed into a local and never used; unreferenced padding
// preserves the retail VC5 /Od stack frame size.
// FUNCTION: FF7 0x005629c2
void __cdecl BattleFx_MagicDeathDrawPsxGouraudQuad(BattleFxDeathStripTrail *trail, int segment, int segmentCount)
{
    int colRemaining;
    int rColor;
    BattleFxPsxGouraudQuadPacket *innerPrim;
    int tBlend;
    int t; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int avgDepth;

    tBlend = 0x1000 / segmentCount * segment;
    Gfx_ShadePsxVertexColor(&trail->shadeStartRgb, tBlend, &trail->colRemainingRgb);
    colRemaining = trail->colRemaining;
    rColor = trail->rColor;
    innerPrim = (BattleFxPsxGouraudQuadPacket *)Gfx_GetPsxPacketScratchPtr();
    avgDepth = trail->prevDepth + trail->depth >> 1 >> 2;
    innerPrim->ndcX0 = trail->screenPoint[0];
    innerPrim->ndcY0 = trail->screenPoint[1];
    innerPrim->prevNdcX0 = trail->prevScreenPoint[0];
    innerPrim->prevNdcY0 = trail->prevScreenPoint[1];
    innerPrim->color0 = colRemaining;
    innerPrim->color1 = colRemaining;
    innerPrim->color2 = rColor;
    innerPrim->color3 = rColor;
    innerPrim->tag = innerPrim->tag & 0xFFFFFF | 0x8000000;
    *(char *)((char *)innerPrim + 7) = 0x3A;
    Gfx_SetBlendFactorQuad(*(float *)&trail->ndcZ[0], *(float *)&trail->ndcW[0], *(float *)&trail->ndcZ[1], *(float *)&trail->ndcW[1], *(float *)&trail->prevNdcZ[0], *(float *)&trail->prevNdcW[0], *(float *)&trail->prevNdcZ[1], *(float *)&trail->prevNdcW[1]);
    Gfx_DrawPsxPrimitive(innerPrim);
    g_BattleFxActiveDrawListPtr = (int)innerPrim;
}

// Projects pointA and pointB into the trail's screen points 0 and 1. The first projection's result
// is stored in depth. Each point's last projected NDC Z and W are captured: pointA into ndcZ[0] and
// ndcW[0], pointB into ndcZ[1] and ndcW[1].
// FUNCTION: FF7 0x00562b15
void __cdecl BattleFx_MagicDeathProjectStripEdgePoints(BattleFxDeathStripTrail *trail, short *pointA, short *pointB)
{
    trail->depth = ProjectPointHardware(pointA, (short *)&trail->screenPoint[0], &trail->projectFlag1, &trail->projectFlag2);
    *(float *)&trail->ndcZ[0] = GetLastProjectedNdcZ();
    *(float *)&trail->ndcW[0] = GetLastProjectedNdcW();
    ProjectPointHardware(pointB, (short *)&trail->screenPoint[1], &trail->projectFlag1, &trail->projectFlag2);
    *(float *)&trail->ndcZ[1] = GetLastProjectedNdcZ();
    *(float *)&trail->ndcW[1] = GetLastProjectedNdcW();
}

// Gets the actor attachment matrix for actorId/attachIndex, subtracts the shared transform translation into
// a short triple and transforms that triple by the transposed shared matrix. Concatenates the
// transpose and attachment matrix preserving translation, transforms inPos into outPos and adds the
// transformed translation difference to its three short coordinates.
// FUNCTION: FF7 0x00562b9a
void __cdecl BattleFx_MagicDeathTransformAttachPoint(int actorId, int attachIndex, short *inPos, short *outPos)
{
    struct {
        PsxMatrix combined;
        char pad0[2];
        short direction[4];
        PsxMatrix transpose;
        char pad1[2];
        PsxMatrix attach;
        char pad2[2];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    Battle3d_GetActorAttachMatrix(actorId, attachIndex, (PsxMatrix *)&frame.attach);
    frame.direction[0] = frame.attach.t[0] - (g_BattleCameraTransform.translation[0]);
    frame.direction[1] = frame.attach.t[1] - (g_BattleCameraTransform.translation[1]);
    frame.direction[2] = frame.attach.t[2] - (g_BattleCameraTransform.translation[2]);
    TransposeMatrix(g_BattleCameraTransform.matrix, &frame.transpose);
    TransformVectorToInt16(&frame.transpose, frame.direction, frame.direction);
    ConcatMatrixPreservingTranslation(&frame.transpose, &frame.attach, &frame.combined);
    TransformVectorToInt16(&frame.combined, inPos, outPos);
    outPos[0] += frame.direction[0];
    outPos[1] += frame.direction[1];
    outPos[2] += frame.direction[2];
}

/*
 * Strip timeline task controlling curved particle trail generation for Deth.
 */
// FUNCTION: FF7 0x00562c60
void __cdecl BattleFx_MagicDeathCurvedStripTimelineTask(void)
{
    BattleFxDeathRingRecord *ringEnt;
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxMagicDeathStrip *rawTaskSlot;
    int t; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int y; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int a; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int i8;
    int c; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int d; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    rawTaskSlot = (BattleFxMagicDeathStrip *)BATTLE_CURRENT_TASK_SLOT;
    if (rawTaskSlot->step == 1) {
        ringEnt = (BattleFxDeathRingRecord *)Battle_AllocSceneMessage(1);
        *(Int64Pair *)&ringEnt->posX = *(Int64Pair *)&rawTaskSlot->posX;
        ringEnt->angle = rawTaskSlot->angle;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH2, Battle_ComputeScreenPanFromPosition(&rawTaskSlot->posX), 0x1A4);
    } else if (rawTaskSlot->step == 55) {
        Battle_AllocSceneMessage(2);
    }
    if (rawTaskSlot->step > 24 && rawTaskSlot->step < 44) {
        if (rawTaskSlot->step < 31) {
            BattleFx_MagicDeathUpdateDrawCurvedStripTrail(1);
        } else {
            BattleFx_MagicDeathUpdateDrawCurvedStripTrail(0);
        }
    } else if (rawTaskSlot->step == 1) {
        g_BattleMagicDeathStepFlag = 0;
        g_BattleMagicDeathStateParam2 = 0;
        g_BattleMagicDeathStateParam1 = 0;
        for (i8 = 0; i8 < 16; i8++) {
            g_BattleMagicDeathTrailData[i8].active = 0;
        }
    }
    if (rawTaskSlot->step == 24) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH2, Battle_ComputeScreenPanFromPosition(&rawTaskSlot->posX), 0x1A3);
    }
    if (rawTaskSlot->step == 40) {
        for (i8 = 0; i8 < 10; i8++) {
            if ((g_BattleMagicDeathTargetMask & 1 << i8) != 0) {
                Battle_PostActorHitOrHurtMessage(i8);
            }
        }
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH2, Battle_ComputeScreenPanFromPosition(&rawTaskSlot->posX), 0x1A8);
    }
    if (rawTaskSlot->step >= 65) {
        rawTaskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    (rawTaskSlot->step)++;
}

/*
 * Updates and renders the curved quad strip trail for Deth.
 */
// FUNCTION: FF7 0x00562e32
void __cdecl BattleFx_MagicDeathUpdateDrawCurvedStripTrail(int updateFlag)
{
    struct {
        int total, step, shade, previous_color, next;
        BattleFxDeathStripTrail *scratch;
        int current, increment, point, count;
        union {
            short components[4];
            int words[2];
        } end;
        int color;
        union {
            short components[4];
            int words[2];
        } start;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.scratch = (BattleFxDeathStripTrail *)Gfx_GetPsxScratchpadPtr(0);
    BattleFx_MagicDeathTransformAttachPoint(3, 6, g_BattleMagicDeathAttachPos, frame.start.components);
    frame.scratch->normalA.vx = frame.start.components[0] - g_BattleMagicDeathTargetPos.targetX;
    frame.scratch->normalA.vy = 0;
    frame.scratch->normalA.vz = frame.start.components[2] - g_BattleMagicDeathTargetPos.targetZ;
    NormalizeVectorFixed(&frame.scratch->normalA.vx, &frame.scratch->normalA.vx);
    frame.start.components[0] = g_BattleMagicDeathTargetPos.targetX + (frame.scratch->normalA.vx * 0x12C >> 12);
    frame.start.components[2] = g_BattleMagicDeathTargetPos.targetZ + (frame.scratch->normalA.vz * 0x12C >> 12);
    frame.end.components[0] = g_BattleMagicDeathTargetPos.targetX + (frame.scratch->normalA.vx * 0x1130 >> 12);
    frame.end.components[2] = g_BattleMagicDeathTargetPos.targetZ + (frame.scratch->normalA.vz * 0x1130 >> 12);
    frame.end.components[1] = g_BattleMagicDeathTargetPos.targetHeight - 0x2BC;
    frame.start.components[1] = frame.end.components[1];
    if (updateFlag != 0) {
        if (g_BattleIsPaused == 0) {
            frame.current = g_BattleMagicDeathStepFlag;
            frame.next = g_BattleMagicDeathStateParam2;
            frame.previous_color = g_BattleMagicDeathStateParam1;
            *(Int64Pair *)&g_BattleMagicDeathTrailData[frame.current].startPos = *(Int64Pair *)(&frame.start);
            *(Int64Pair *)&g_BattleMagicDeathTrailData[frame.current].endPos = *(Int64Pair *)(&frame.end);
            g_BattleMagicDeathTrailData[frame.current].active = 1;
            g_BattleMagicDeathTrailData[frame.current].normal = frame.scratch->normalA;
            frame.current++;
            frame.previous_color++;
            if (frame.previous_color >= 8) {
                frame.previous_color = 8;
            }
            if (frame.current >= 8) {
                frame.current = 0;
            }
            if (frame.current == frame.next) {
                g_BattleMagicDeathTrailData[frame.next].active = 0;
                frame.next++;
                frame.previous_color--;
                if (frame.previous_color < 0) {
                    frame.previous_color = 0;
                }
                if (frame.next >= 8) {
                    frame.next = 0;
                }
            }
            g_BattleMagicDeathStepFlag = frame.current;
            g_BattleMagicDeathStateParam2 = frame.next;
            g_BattleMagicDeathStateParam1 = frame.previous_color;
        }
    } else if (g_BattleMagicDeathStateParam2 != g_BattleMagicDeathStepFlag) {
        frame.next = g_BattleMagicDeathStateParam2;
        g_BattleMagicDeathTrailData[frame.next].active = 0;
        frame.next++;
        if (frame.next >= 8) {
            frame.next = 0;
        }
        g_BattleMagicDeathStateParam2 = frame.next;
    }
    frame.scratch->colRemaining = 16;
    frame.scratch->shadeStartColor = 0x10;
    SetRotMatrix(g_BattleCameraTransform.matrix);
    SetTransMatrix(g_BattleCameraTransform.matrix);
    RefreshGlobalTransformState();
    BattleFx_MagicDeathSubdivideProjectTrailSegment(frame.scratch, &g_BattleMagicDeathTrailData[g_BattleMagicDeathStateParam2].startPos, &g_BattleMagicDeathTrailData[g_BattleMagicDeathStateParam2].endPos);
    frame.current = g_BattleMagicDeathStateParam2;
    frame.total = g_BattleMagicDeathStateParam1;
    for (frame.step = 1;; frame.current++, frame.step++) {
        if (frame.current >= 8) {
            frame.current = 0;
        }
        frame.next = frame.current + 1;
        if (frame.next >= 8) {
            frame.next = 0;
        }
        if (g_BattleMagicDeathTrailData[frame.next].active == 0) {
            return;
        }
        *(Int64Pair *)&g_BattleMagicDeathTrailWorkingBlob1[0].startX = *(Int64Pair *)&g_BattleMagicDeathTrailData[frame.current].startPos;
        *(Int64Pair *)&frame.scratch->posAX = *(Int64Pair *)&g_BattleMagicDeathTrailWorkingBlob1[0].startX;
        *(Int64Pair *)&g_BattleMagicDeathTrailWorkingBlob1[0].endX = *(Int64Pair *)&g_BattleMagicDeathTrailData[frame.current].endPos;
        *(Int64Pair *)&frame.scratch->posBX = *(Int64Pair *)&g_BattleMagicDeathTrailWorkingBlob1[0].endX;
        frame.scratch->normalA.vx = frame.scratch->posAX - g_BattleMagicDeathTargetPos.targetX;
        frame.scratch->normalA.vy = 0;
        frame.scratch->normalA.vz = frame.scratch->posAY - g_BattleMagicDeathTargetPos.targetZ;
        frame.scratch->distance = NormalizeVectorFixed(&frame.scratch->normalA.vx, &frame.scratch->normalA.vx);
        frame.scratch->distance = SqrtToInt(frame.scratch->distance);
        frame.scratch->normalA.vx = frame.scratch->posBX - g_BattleMagicDeathTargetPos.targetX;
        frame.scratch->normalA.vy = 0;
        frame.scratch->normalA.vz = frame.scratch->posBY - g_BattleMagicDeathTargetPos.targetZ;
        frame.scratch->segmentLength = NormalizeVectorFixed(&frame.scratch->normalA.vx, &frame.scratch->normalA.vx);
        frame.scratch->segmentLength = SqrtToInt(frame.scratch->segmentLength);
        frame.scratch->normalA.vx = g_BattleMagicDeathTrailData[frame.next].startPos - g_BattleMagicDeathTargetPos.targetX;
        frame.scratch->normalA.vy = 0;
        frame.scratch->normalA.vz = g_BattleMagicDeathTrailData[frame.next].posA - g_BattleMagicDeathTargetPos.targetZ;
        frame.scratch->savedDistance = NormalizeVectorFixed(&frame.scratch->normalA.vx, &frame.scratch->normalA.vx);
        frame.scratch->savedDistance = SqrtToInt(frame.scratch->savedDistance);
        frame.scratch->normalA.vx = g_BattleMagicDeathTrailData[frame.next].endPos - g_BattleMagicDeathTargetPos.targetX;
        frame.scratch->normalA.vy = 0;
        frame.scratch->normalA.vz = g_BattleMagicDeathTrailData[frame.next].posB - g_BattleMagicDeathTargetPos.targetZ;
        frame.scratch->savedSegmentLength = NormalizeVectorFixed(&frame.scratch->normalA.vx, &frame.scratch->normalA.vx);
        frame.scratch->savedSegmentLength = SqrtToInt(frame.scratch->savedSegmentLength);
        frame.scratch->normalA = g_BattleMagicDeathTrailData[frame.current].normal;
        frame.scratch->normalB = g_BattleMagicDeathTrailData[frame.next].normal;
        frame.color = ComputeAngleBetweenVectors(&frame.scratch->normalA, &frame.scratch->normalB, &frame.scratch->cross);
        frame.count = frame.color >> 7;
        frame.increment = 1;
        if (frame.count > 1) {
            frame.scratch->distance = (frame.scratch->distance - frame.scratch->savedDistance) / frame.count;
            frame.scratch->segmentLength = (frame.scratch->segmentLength - frame.scratch->savedSegmentLength) / frame.count;
            frame.scratch->savedDistance = frame.scratch->distance;
            frame.scratch->savedSegmentLength = frame.scratch->segmentLength;
            frame.scratch->posAX -= g_BattleMagicDeathTargetPos.targetX;
            frame.scratch->posAY -= g_BattleMagicDeathTargetPos.targetZ;
            frame.scratch->posBX -= g_BattleMagicDeathTargetPos.targetX;
            frame.scratch->posBY -= g_BattleMagicDeathTargetPos.targetZ;
            frame.scratch->stripBHeight = frame.scratch->posAHeight;
            frame.scratch->stripAHeight = frame.scratch->stripBHeight;
            for (frame.point = 1; frame.point < frame.count; frame.point++) {
                frame.scratch->stripAX = frame.scratch->posAX - (frame.scratch->distance * frame.scratch->normalA.vx >> 12);
                frame.scratch->stripAY = frame.scratch->posAY - (frame.scratch->distance * frame.scratch->normalA.vz >> 12);
                frame.scratch->stripBX = frame.scratch->posBX - (frame.scratch->segmentLength * frame.scratch->normalA.vx >> 12);
                frame.scratch->stripBY = frame.scratch->posBY - (frame.scratch->segmentLength * frame.scratch->normalA.vz >> 12);
                frame.scratch->distance += frame.scratch->savedDistance;
                frame.scratch->segmentLength += frame.scratch->savedSegmentLength;
                Gte_BuildAxisAngleMatrix(frame.point << 7, &frame.scratch->rotMatrix, &frame.scratch->cross);
                TransformVectorToInt16(&frame.scratch->rotMatrix, &frame.scratch->stripAX, &frame.scratch->stripAX);
                TransformVectorToInt16(&frame.scratch->rotMatrix, &frame.scratch->stripBX, &frame.scratch->stripBX);
                g_BattleMagicDeathTrailWorkingBlob1[frame.increment].startX = frame.scratch->stripAX + g_BattleMagicDeathTargetPos.targetX;
                g_BattleMagicDeathTrailWorkingBlob1[frame.increment].startY = frame.scratch->stripAY + g_BattleMagicDeathTargetPos.targetZ;
                g_BattleMagicDeathTrailWorkingBlob1[frame.increment].endX = frame.scratch->stripBX + g_BattleMagicDeathTargetPos.targetX;
                g_BattleMagicDeathTrailWorkingBlob1[frame.increment].endY = frame.scratch->stripBY + g_BattleMagicDeathTargetPos.targetZ;
                g_BattleMagicDeathTrailWorkingBlob1[frame.increment].endHeight = frame.scratch->stripAHeight;
                g_BattleMagicDeathTrailWorkingBlob1[frame.increment].startHeight = g_BattleMagicDeathTrailWorkingBlob1[frame.increment].endHeight;
                frame.increment++;
            }
            frame.count--;
        } else {
            frame.count = 0;
        }
        *(Int64Pair *)&g_BattleMagicDeathTrailWorkingBlob1[frame.increment].startX = *(Int64Pair *)&g_BattleMagicDeathTrailData[frame.next].startPos;
        *(Int64Pair *)&g_BattleMagicDeathTrailWorkingBlob1[frame.increment].endX = *(Int64Pair *)&g_BattleMagicDeathTrailData[frame.next].endPos;
        frame.count += 2;
        g_BattleMagicDeathTrailData[frame.current].startPos += (short)(frame.scratch->normalA.vx * 0x320 >> 12);
        g_BattleMagicDeathTrailData[frame.current].posA += (short)(frame.scratch->normalA.vz * 0x320 >> 12);
        g_BattleMagicDeathTrailData[frame.current].endPos += (short)(frame.scratch->normalA.vx * 0x640 >> 12);
        g_BattleMagicDeathTrailData[frame.current].posB += (short)(frame.scratch->normalA.vz * 0x640 >> 12);
        frame.scratch->rColor = frame.scratch->colRemaining;
        frame.shade = 0x1000 / frame.total * frame.step;
        SetDrawColorRGB(0x78, 0x1A, 0x1A);
        Gfx_ShadePsxVertexColor(&frame.scratch->shadeStartRgb, frame.shade, &frame.scratch->colRemainingRgb);
        SetRotMatrix(g_BattleCameraTransform.matrix);
        SetTransMatrix(g_BattleCameraTransform.matrix);
        RefreshGlobalTransformState();
        frame.increment = 0x1000 / frame.count;
        frame.color = frame.scratch->rColor;
        for (frame.point = 1; frame.point < frame.count; frame.point++) {
            for (frame.shade = 0; frame.shade < 9; frame.shade++) {
                frame.scratch->prevScreenPoint[frame.shade] = frame.scratch->screenPoint[frame.shade];
                frame.scratch->prevPointDepth[frame.shade] = frame.scratch->pointDepth[frame.shade];
                frame.scratch->prevNdcZ[frame.shade] = frame.scratch->ndcZ[frame.shade];
                frame.scratch->prevNdcW[frame.shade] = frame.scratch->ndcW[frame.shade];
            }
            BattleFx_MagicDeathSubdivideProjectTrailSegment(frame.scratch, &g_BattleMagicDeathTrailWorkingBlob1[frame.point].startX, &g_BattleMagicDeathTrailWorkingBlob1[frame.point].endX);
            frame.previous_color = frame.color;
            frame.shade = frame.increment * frame.point;
            SetDrawColorRGB(frame.scratch->colRemainingRgb.r, frame.scratch->colRemainingRgb.g, frame.scratch->colRemainingRgb.b);
            Gfx_ShadePsxVertexColor(&frame.scratch->rColorRgb, frame.shade, (CVECTOR *)&frame.color);
            BattleFx_MagicDeathDrawPsxGouraudQuadStrip(frame.scratch, frame.color, frame.previous_color);
        }
    }
}

// Draws an eight-segment trail strip with one render state block from Gfx_GetPsxPacketScratchPtr, obtained
// once for the whole strip. Each segment joins points segment and segment + 1 from screenPoint and
// prevScreenPoint, coloured colorA at one end and colorB at the other. The tag top byte is set to
// 0x08 and the primitive code byte (packet byte 7) to 0x3A. The per-point blend factors come from
// ndcZ, ndcW, prevNdcZ and prevNdcW, and each segment is drawn with Gfx_DrawPsxPrimitive. An
// averaged depth from pointDepth and prevPointDepth (sum >> 2 >> 2) is computed each iteration and
// never used. The block is left in g_BattleFxActiveDrawListPtr.
// FUNCTION: FF7 0x00563a27
void __cdecl BattleFx_MagicDeathDrawPsxGouraudQuadStrip(BattleFxDeathStripTrail *trail, int colorA, int colorB)
{
    BattleFxPsxGouraudStripPacket *primPtr;
    int segment, nextIndex, avgDepth;

    primPtr = (BattleFxPsxGouraudStripPacket *)Gfx_GetPsxPacketScratchPtr();
    for (segment = 0; segment < 8; segment++) {
        nextIndex = segment + 1;
        avgDepth = trail->pointDepth[segment] + trail->pointDepth[nextIndex] + trail->prevPointDepth[segment] + trail->prevPointDepth[nextIndex];
        avgDepth = avgDepth >> 2 >> 2;
        primPtr->nextCoordA = trail->screenPoint[nextIndex];
        primPtr->coordA = trail->screenPoint[segment];
        primPtr->nextCoordB = trail->prevScreenPoint[nextIndex];
        primPtr->coordB = trail->prevScreenPoint[segment];
        primPtr->color0 = colorA;
        primPtr->color1 = colorA;
        primPtr->color2 = colorB;
        primPtr->color3 = colorB;
        primPtr->tag = primPtr->tag & 0xFFFFFF | 0x8000000;
        *(char *)((char *)primPtr + 7) = 0x3A;
        Gfx_SetBlendFactorQuad(*(float *)&trail->ndcZ[nextIndex], *(float *)&trail->ndcW[nextIndex], *(float *)&trail->ndcZ[segment], *(float *)&trail->ndcW[segment], *(float *)&trail->prevNdcZ[nextIndex], *(float *)&trail->prevNdcW[nextIndex], *(float *)&trail->prevNdcZ[segment], *(float *)&trail->prevNdcW[segment]);
        Gfx_DrawPsxPrimitive(primPtr);
    }
    g_BattleFxActiveDrawListPtr = (int)primPtr;
}

// Copies eight bytes from each endpoint into the first and ninth of nine stride-eight shared
// points. It fills the intermediate posX and posZ values with arithmetic-shift midpoints in a fixed
// order, then sets all nine height values to the first endpoint's height, including overwriting the
// last endpoint's. Each point is projected into the trail's screenPoint and pointDepth arrays, with
// projectFlag1 and projectFlag2 as the projection output arguments, and its last projected NDC Z and
// W go to ndcZ and ndcW. The padding shorts are not initialised by the midpoint writes.
// FUNCTION: FF7 0x00563bb4
void __cdecl BattleFx_MagicDeathSubdivideProjectTrailSegment(BattleFxDeathStripTrail *trail, short *endA, short *endB)
{
    int idx, q, r;
    *(Int64Pair *)&g_BattleMagicDeathTrailSubdivisionPoints[0] = *(Int64Pair *)endA;
    *(Int64Pair *)&g_BattleMagicDeathTrailSubdivisionPoints[8] = *(Int64Pair *)endB;
    g_BattleMagicDeathTrailSubdivisionPoints[4].posX = (g_BattleMagicDeathTrailSubdivisionPoints[0].posX + g_BattleMagicDeathTrailSubdivisionPoints[8].posX) >> 1;
    g_BattleMagicDeathTrailSubdivisionPoints[4].posZ = (g_BattleMagicDeathTrailSubdivisionPoints[0].posZ + g_BattleMagicDeathTrailSubdivisionPoints[8].posZ) >> 1;
    g_BattleMagicDeathTrailSubdivisionPoints[2].posX = (g_BattleMagicDeathTrailSubdivisionPoints[0].posX + g_BattleMagicDeathTrailSubdivisionPoints[4].posX) >> 1;
    g_BattleMagicDeathTrailSubdivisionPoints[2].posZ = (g_BattleMagicDeathTrailSubdivisionPoints[0].posZ + g_BattleMagicDeathTrailSubdivisionPoints[4].posZ) >> 1;
    g_BattleMagicDeathTrailSubdivisionPoints[6].posX = (g_BattleMagicDeathTrailSubdivisionPoints[4].posX + g_BattleMagicDeathTrailSubdivisionPoints[8].posX) >> 1;
    g_BattleMagicDeathTrailSubdivisionPoints[6].posZ = (g_BattleMagicDeathTrailSubdivisionPoints[4].posZ + g_BattleMagicDeathTrailSubdivisionPoints[8].posZ) >> 1;
    g_BattleMagicDeathTrailSubdivisionPoints[1].posX = (g_BattleMagicDeathTrailSubdivisionPoints[0].posX + g_BattleMagicDeathTrailSubdivisionPoints[2].posX) >> 1;
    g_BattleMagicDeathTrailSubdivisionPoints[1].posZ = (g_BattleMagicDeathTrailSubdivisionPoints[0].posZ + g_BattleMagicDeathTrailSubdivisionPoints[2].posZ) >> 1;
    g_BattleMagicDeathTrailSubdivisionPoints[3].posX = (g_BattleMagicDeathTrailSubdivisionPoints[2].posX + g_BattleMagicDeathTrailSubdivisionPoints[4].posX) >> 1;
    g_BattleMagicDeathTrailSubdivisionPoints[3].posZ = (g_BattleMagicDeathTrailSubdivisionPoints[2].posZ + g_BattleMagicDeathTrailSubdivisionPoints[4].posZ) >> 1;
    g_BattleMagicDeathTrailSubdivisionPoints[5].posX = (g_BattleMagicDeathTrailSubdivisionPoints[4].posX + g_BattleMagicDeathTrailSubdivisionPoints[6].posX) >> 1;
    g_BattleMagicDeathTrailSubdivisionPoints[5].posZ = (g_BattleMagicDeathTrailSubdivisionPoints[4].posZ + g_BattleMagicDeathTrailSubdivisionPoints[6].posZ) >> 1;
    g_BattleMagicDeathTrailSubdivisionPoints[7].posX = (g_BattleMagicDeathTrailSubdivisionPoints[6].posX + g_BattleMagicDeathTrailSubdivisionPoints[8].posX) >> 1;
    g_BattleMagicDeathTrailSubdivisionPoints[7].posZ = (g_BattleMagicDeathTrailSubdivisionPoints[6].posZ + g_BattleMagicDeathTrailSubdivisionPoints[8].posZ) >> 1;
    for (idx = 0; idx < 9; idx++) {
        g_BattleMagicDeathTrailSubdivisionPoints[idx].height = endA[1];
        trail->pointDepth[idx] = ProjectPointHardware(&g_BattleMagicDeathTrailSubdivisionPoints[idx], (short *)&trail->screenPoint[idx], &trail->projectFlag1, &trail->projectFlag2);
        *(float *)&trail->ndcZ[idx] = GetLastProjectedNdcZ();
        *(float *)&trail->ndcW[idx] = GetLastProjectedNdcW();
    }
}

/*
 * Draws and steps outward-scattering spirit sprites from the Death spell target.
 */
// FUNCTION: FF7 0x00563dc5
void __cdecl BattleFx_MagicDeathScatterSpritesFromTarget(void)
{
    int i6;
    int readPos;
    int r;
    VECTOR auxDirX;
    int matrix;
    BattleFxBerserkFadeSprite *pFx;

    pFx = (BattleFxBerserkFadeSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicDeathSpriteDrawReq1.sprite.frame = pFx->frame;
    readPos = pFx->ringIndex;
    g_BattleMagicDeathSpriteDrawReq1.sprite.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleMagicDeathLoadedRes);
    g_BattleMagicDeathSpriteDrawReq1.sprite.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleMagicDeathLoadedRes);
    for (i6 = 0; i6 < 4; i6++) {
        if (g_BattleIsPaused == 0) {
            auxDirX.vx = g_BattleMagicDeathWorkingBlob1.points[readPos].x - g_BattleMagicDeathTargetPos.targetX;
            auxDirX.vy = g_BattleMagicDeathWorkingBlob1.points[readPos].y - g_BattleMagicDeathTargetPos.targetHeight;
            auxDirX.vz = g_BattleMagicDeathWorkingBlob1.points[readPos].z - g_BattleMagicDeathTargetPos.targetZ;
            r = NormalizeVectorFixed(&auxDirX, &auxDirX);
            r = SqrtToInt(r) >> 5;
            g_BattleMagicDeathWorkingBlob1.points[readPos].x += (short)(r * auxDirX.vx >> 12);
            g_BattleMagicDeathWorkingBlob1.points[readPos].y += (short)(r * auxDirX.vy >> 12);
            g_BattleMagicDeathWorkingBlob1.points[readPos].z += (short)(r * auxDirX.vz >> 12);
        }
        matrix = (int)BattleFx_InstallBillboardMatrix(&g_BattleMagicDeathWorkingBlob1.points[readPos].x, g_BattleMagicDeathWorkingBlob1.points[readPos].scale, 0);
        if (REG_ALLOC_HINT) {
        }
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleMagicDeathSpriteDrawReq1.sprite, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
        readPos++;
        if (readPos >= 144) {
            readPos = 0;
        }
    }
    if (pFx->frame >= 11) {
        pFx->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (pFx->frame)++;
}

/*
 * Task animating an individual spiral sprite particle for Deth.
 */
// FUNCTION: FF7 0x00563fbd
void __cdecl BattleFx_MagicDeathSpiralSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[60];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxDeathSpiralSprite *spiralSprite; };
        struct { unsigned char pad_to_24[0x24]; SVECTOR posX; };
        struct { unsigned char pad_to_2C[0x2C]; int billboardResult; };
        struct { unsigned char pad_to_30[0x30]; SVECTOR angleX; };
        struct { unsigned char pad_to_38[0x38]; char colorValue; };
    } frame;
#pragma pack(pop)

    frame.spiralSprite = (BattleFxDeathSpiralSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicDeathSpriteColor.frame = frame.spiralSprite->timer;
    frame.colorValue = 0x30 - (frame.spiralSprite->timer << 2);
    g_BattleMagicDeathSpriteColor.colorB = frame.colorValue;
    g_BattleMagicDeathSpriteColor.colorG = g_BattleMagicDeathSpriteColor.colorB;
    g_BattleMagicDeathSpriteColor.colorR = g_BattleMagicDeathSpriteColor.colorG;
    frame.angleX.vx = 0;
    frame.angleX.vy = frame.spiralSprite->angle;
    frame.angleX.vz = 0;
    RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
    frame.posX.vx = frame.spiralSprite->radialOffset;
    frame.posX.vy = 0;
    frame.posX.vz = 0;
    TransformVectorToInt16(&frame.rotMatrix, &frame.posX, &frame.posX.vx);
    frame.posX.vx += frame.spiralSprite->posX;
    frame.posX.vy += frame.spiralSprite->posY;
    frame.posX.vz += frame.spiralSprite->posZ;
    frame.billboardResult = (int)BattleFx_InstallBillboardMatrix(&frame.posX, (short)frame.spiralSprite->scale, 0);
    if (REG_ALLOC_HINT) {
    }
    g_BattleMagicDeathSpriteColor.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleMagicDeathLoadedRes);
    g_BattleMagicDeathSpriteColor.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleMagicDeathLoadedRes);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleMagicDeathSpriteColor, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.spiralSprite->timer >= 11) {
        frame.spiralSprite->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spiralSprite->posY += frame.spiralSprite->yStep;
    frame.spiralSprite->angle -= frame.spiralSprite->angleStep;
    frame.spiralSprite->scale += (short)(frame.spiralSprite->scale >> 8);
    frame.spiralSprite->radialOffset += frame.spiralSprite->radialStep;
    (frame.spiralSprite->timer)++;
}

/*
 * Draws and steps inward-gathering spirit sprites converging on the Death spell target.
 */
// FUNCTION: FF7 0x00564191
void __cdecl BattleFx_MagicDeathGatherSpritesToTarget(void)
{
    int i6;
    int readPos;
    int r;
    VECTOR auxDirX;
    int matrix;
    BattleFxBerserkFadeSprite *pFx;

    pFx = (BattleFxBerserkFadeSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicDeathSpriteDrawReq2.frame = pFx->frame;
    readPos = pFx->ringIndex;
    g_BattleMagicDeathSpriteDrawReq2.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleMagicDeathLoadedRes);
    g_BattleMagicDeathSpriteDrawReq2.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleMagicDeathLoadedRes);
    for (i6 = 0; i6 < 4; i6++) {
        if (g_BattleIsPaused == 0) {
            auxDirX.vx = g_BattleMagicDeathTargetPos.targetX - g_BattleMagicDeathWorkingBlob1.points[readPos].x;
            auxDirX.vy = g_BattleMagicDeathTargetPos.targetHeight - g_BattleMagicDeathWorkingBlob1.points[readPos].y;
            auxDirX.vz = g_BattleMagicDeathTargetPos.targetZ - g_BattleMagicDeathWorkingBlob1.points[readPos].z;
            r = NormalizeVectorFixed(&auxDirX, &auxDirX);
            r = SqrtToInt(r) >> 6;
            g_BattleMagicDeathWorkingBlob1.points[readPos].x += (short)(r * auxDirX.vx >> 12);
            g_BattleMagicDeathWorkingBlob1.points[readPos].y += (short)(r * auxDirX.vy >> 12);
            g_BattleMagicDeathWorkingBlob1.points[readPos].z += (short)(r * auxDirX.vz >> 12);
        }
        matrix = (int)BattleFx_InstallBillboardMatrix(&g_BattleMagicDeathWorkingBlob1.points[readPos].x, g_BattleMagicDeathWorkingBlob1.points[readPos].scale, 0);
        if (REG_ALLOC_HINT) {
        }
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleMagicDeathSpriteDrawReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
        readPos++;
        if (readPos >= 144) {
            readPos = 0;
        }
    }
    if (pFx->frame >= 11) {
        pFx->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (pFx->frame)++;
}
} /* extern "C" */
