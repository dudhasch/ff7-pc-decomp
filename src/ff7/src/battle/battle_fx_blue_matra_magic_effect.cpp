// src/ff7/src/battle/battle_fx_blue_matra_magic_effect.cpp
// Enemy Skill [10]: Matra Magic (Japanese: マトラマジック, dev tag: "blue/matra" / Matra)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The Matra blue magic effect: missile ribbons with trail segments and impact explosions,
//   drawn as PSX-style quads along a 3D missile model's trajectory.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_fx.h"
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
// GLOBAL: FF7 0x008a8f78
int g_BattleBlueMatraMagicResourceDef = 0x00192A39;
char *g_BattleFxBlueMatraMagicEffectResourceNames[1] = {
    // STRING: FF7 0x008a8fe0
    "MATRA_1.TIM"
};
typedef struct {
    char *filename;
    unsigned int descriptor;
} BattleBlueMatraResourceTailEntry;
BattleBlueMatraResourceTailEntry g_BattleFxBlueMatraMagicEffectAssetPrefix[4] = {
    {
        // STRING: FF7 0x008a8fec
        "MATRA.SPT",
        0x00000000
    },
    {
        0,
        0x00fa00b4
    },
    {
        // STRING: FF7 0x008a8ff8
        "matra.rsd",
        0x00000000
    },
    {
        0,
        0x00000000
    }
};
// GLOBAL: FF7 0x008a8fa0
struct BattleBlueMatraMagicModelDrawReq g_BattleBlueMatraMagicModelDrawReq = {0, {0, 0, 0, 0, 0, 0xf0, 0xff, 0xff, 0, 0, 0, 0}, 0};
unsigned char g_rawData_008a8fb8[24] = {0x00, 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008a8fd0
struct BattleBlueMatraMagicSpriteDrawReq g_BattleBlueMatraMagicSpriteDrawReq = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
// GLOBAL: FF7 0x00b7dff8
int g_BattleBlueMatraMagicDrawBufferSlot = 0;
unsigned char g_bssPad_00b7dffc[4] = {0};
// GLOBAL: FF7 0x00b7e000
BattleFxMatraMagicQuadPacket g_BattleBlueMatraMagicBufferPageIndex[2048] = {0};
// GLOBAL: FF7 0x00b90000
BattleFxMatraMagicQuadPacket g_BattleBlueMatraMagicDrawListBuffer0[2048] = {0};
// GLOBAL: FF7 0x00ba2000
int g_BattleBlueMatraMagicDrawListBuffer1 = 0;
// GLOBAL: FF7 0x00ba2004
int g_BattleBlueMatraMagicRibbonCount = 0;
unsigned char g_bssPad_00ba2008[4] = {0};
// GLOBAL: FF7 0x00ba200c
short g_BattleBlueMatraMagicRibbonAngle = 0;
// GLOBAL: FF7 0x00ba2010
short g_BattleBlueMatraMagicRibbonPitch = 0;
// GLOBAL: FF7 0x00ba2014
short g_BattleBlueMatraMagicRibbonRoll = 0;
// GLOBAL: FF7 0x00ba2018
BattleBlueMatraMagicTrajectory g_BattleBlueMatraMagicTrajectoryParams = {0};
unsigned char g_bssPad_00ba2024[4] = {0};
// GLOBAL: FF7 0x00ba2028
struct BattleFxBlueMatraMagicRibbon g_BattleBlueMatraMagicRibbonRecords[28] = {0};
// GLOBAL: FF7 0x00ba37c8
int g_BattleBlueMatraMagicTargetMask = 0;
// GLOBAL: FF7 0x00ba37cc
int g_BattleBlueMatraMagicStateFlag = 0;
// GLOBAL: FF7 0x00ba37d0
struct BattleBlueMatraMagicEmitterRecord g_BattleBlueMatraMagicEmitterRecord = {0};
// clang-format on
// The TU's own view of moved definitions declared with another type.

/*
 * Loads resources for the Matra blue magic effect when not already loaded.
 */
// FUNCTION: FF7 0x0056a800
void __cdecl BattleFx_BlueLoadResMatraMagic(void)
{
    GfxModelTableBank *stageSceneHandle;
    GfxResourceRequest reqBuffer;
    int kind;
    char tmpBuffer[200];
    GfxResourceRequest *pRequest;
    int stepResourceDef;

    if (g_pBattle3dScene != 0 && g_BattleBlueMatraMagicEmitterRecord.resourceHandle == 0) {
        kind = 2;
        stepResourceDef = (int)&g_BattleBlueMatraMagicResourceDef;
        stageSceneHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a9004
        pRequest->resourcePath = BattleFx_BuildResourcePath("blue/matra", tmpBuffer);
        Gfx_LoadModelTableIfAbsent(kind, stepResourceDef, pRequest, &g_BattleBlueMatraMagicEmitterRecord.resourceHandle, stageSceneHandle);
    }
}

/*
 * Entry point for the Matra blue magic effect.
 */
// FUNCTION: FF7 0x0056a890
void __cdecl BattleFx_BlueEnterMatraMagic(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResMatraMagic();
    BattleFx_BlueBeginMatraMagic(targetMask, actorIdx);
}

/*
 * Initializes and begins the Matra blue magic effect.
 */
// FUNCTION: FF7 0x0056a8aa
void __cdecl BattleFx_BlueBeginMatraMagic(int targetMask, int actorIdx)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[48];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; int emitterCount; };
        struct { unsigned char pad_to_24[0x24]; int slotIndex; };
        struct { unsigned char pad_to_28[0x28]; SVECTOR rotX; };
    } frame;
#pragma pack(pop)

    g_BattleBlueMatraMagicDrawBufferSlot = (int)BATTLE_NEW_TASK_SLOT(BattleFx_BlueMatraMagicSelectDrawListBuffer);
    for (frame.slotIndex = 0; frame.slotIndex < 0x1C; frame.slotIndex++) {
        g_BattleBlueMatraMagicRibbonRecords[frame.slotIndex].state = BATTLE_BLUE_MATRA_MAGIC_RIBBON_FREE;
    }
    frame.slotIndex = 0;
    for (frame.emitterCount = 0; frame.slotIndex < 0xA; frame.slotIndex++) {
        if ((targetMask & 1 << frame.slotIndex) != 0) {
            g_BattleBlueMatraMagicEmitterRecord.emitters[frame.emitterCount].targetHandle = frame.slotIndex;
            frame.emitterCount++;
        }
    }
    g_BattleBlueMatraMagicEmitterRecord.targetCount = frame.emitterCount;
    g_BattleBlueMatraMagicEmitterRecord.actorIndex = actorIdx;
    g_BattleBlueMatraMagicRibbonCount = 0;
    g_BattleBlueMatraMagicRibbonAngle = 0x400;
    g_BattleBlueMatraMagicRibbonPitch = 0;
    g_BattleBlueMatraMagicRibbonRoll = 0;
    g_BattleBlueMatraMagicStateFlag = BATTLE_BLUE_MATRA_MAGIC_SPAWN_SHUFFLE;
    frame.rotX.vx = 0;
    frame.rotX.vy = g_BattleActorRecords[actorIdx].yaw;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    TransformVectorToInt32(&frame.rotMatrix, &g_BattleBlueMatraMagicModelDrawReq.matraModelDrawVector, &g_BattleBlueMatraMagicModelDrawReq.matraModelDrawVector);
    Battle_AllocateResourceSlot(BattleFx_BlueMatraMagicSetupTask);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH1, Battle_ComputePanForEntity(targetMask), 0x82);
}

/*
 * Selects the active draw list buffer slot for Matra ribbon rendering.
 */
// FUNCTION: FF7 0x0056a9fa
void __cdecl BattleFx_BlueMatraMagicSelectDrawListBuffer(void)
{
    BattleFxMatraMagicQuadPacket *rec;

    if (g_BattleDrawFrameParity == 0) {
        rec = (BattleFxMatraMagicQuadPacket *)&g_BattleBlueMatraMagicBufferPageIndex;
    } else {
        rec = (BattleFxMatraMagicQuadPacket *)&g_BattleBlueMatraMagicDrawListBuffer0;
    }
    g_BattleBlueMatraMagicDrawListBuffer1 = (int)rec;
    if (g_BattleSecondaryTaskActiveCount <= 1) {
        *(int *)g_BattleBlueMatraMagicDrawBufferSlot = 0xFFFFFFFF;
    }
}

/*
 * Setup task initializing Matra emitter state and ribbon parameters.
 */
// FUNCTION: FF7 0x0056aa3b
void __cdecl BattleFx_BlueMatraMagicSetupTask(void)
{
    int rad, i, actorId;
    BattleFxDiceTask *selfPtr;

    selfPtr = (BattleFxDiceTask *)BATTLE_CURRENT_TASK_SLOT;
    selfPtr->ownerSlot = BATTLE_TASK_FREE;
    for (i = 0; i < g_BattleBlueMatraMagicEmitterRecord.targetCount; i++) {
        actorId = g_BattleBlueMatraMagicEmitterRecord.emitters[i].targetHandle;
        Battle_GetBoneWorldPos(actorId, g_BattleActorRecords[actorId].targetBone, &g_BattleBlueMatraMagicEmitterRecord.emitters[i].posX);
        rad = g_BattleActorRecords[actorId].radius + 0x8C;
        if (rad > 0x4F2) {
            rad = 0x4F2;
        }
        g_BattleBlueMatraMagicEmitterRecord.emitters[i].spread = rad;
        g_BattleBlueMatraMagicEmitterRecord.emitters[i].hitCount = 0;
    }
    selfPtr = (BattleFxDiceTask *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueMatraMagicSpawnRibbonsTask);
    selfPtr->spawnCursor = 0;
    Battle_GetBoneWorldPos(g_BattleBlueMatraMagicEmitterRecord.actorIndex, g_BattleActorRecords[g_BattleBlueMatraMagicEmitterRecord.actorIndex].targetBone, &g_BattleBlueMatraMagicTrajectoryParams.x);
    g_BattleBlueMatraMagicTrajectoryParams.radius = g_BattleActorRecords[g_BattleBlueMatraMagicEmitterRecord.actorIndex].radius;
    g_BattleBlueMatraMagicTrajectoryParams.peakRadius = g_BattleBlueMatraMagicTrajectoryParams.radius + 0x1F4;
    if (g_BattleBlueMatraMagicTrajectoryParams.peakRadius > 0x76C) {
        g_BattleBlueMatraMagicTrajectoryParams.peakRadius = 0x76C;
    }
}

/*
 * Task driving the emission and trajectory setup of Matra missile ribbons.
 */
// FUNCTION: FF7 0x0056ab98
void __cdecl BattleFx_BlueMatraMagicSpawnRibbonsTask(void)
{
    VECTOR sideVector;
    int count;
    int range;
    int emitterIdx;
    BattleFxMatraMagicRibbonSlot *activeSlot;
    VECTOR velX;
    BattleMatraMagicRibbonSlot *ribbonTask;
    int emitterIndex;
    int index;
    int ribbonIndex;
    PsxMatrix rot;

    activeSlot = (BattleFxMatraMagicRibbonSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (g_BattleBlueMatraMagicStateFlag == BATTLE_BLUE_MATRA_MAGIC_SPAWN_SHUFFLE) {
        /* Numeric active target count (0..2). */
        switch (g_BattleBlueMatraMagicEmitterRecord.targetCount) {
        case 0:
            break;
        case 1:
            g_BattleBlueMatraMagicEmitterRecord.order[0] = 0;
            break;
        case 2:
            emitterIndex = rand() % 2;
            g_BattleBlueMatraMagicEmitterRecord.order[0] = emitterIndex;
            g_BattleBlueMatraMagicEmitterRecord.order[1] = emitterIndex + 1 & 1;
            break;
        default:
            for (emitterIndex = 0; emitterIndex < g_BattleBlueMatraMagicEmitterRecord.targetCount; emitterIndex++) {
                g_BattleBlueMatraMagicEmitterRecord.order[emitterIndex] = emitterIndex;
            }
            for (emitterIndex = 0; emitterIndex < g_BattleBlueMatraMagicEmitterRecord.targetCount; emitterIndex++) {
                count = g_BattleBlueMatraMagicEmitterRecord.targetCount - emitterIndex;
                if (count <= 1) {
                    break;
                }
                range = rand() % count + emitterIndex;
                ribbonIndex = g_BattleBlueMatraMagicEmitterRecord.order[emitterIndex];
                g_BattleBlueMatraMagicEmitterRecord.order[emitterIndex] = g_BattleBlueMatraMagicEmitterRecord.order[range];
                g_BattleBlueMatraMagicEmitterRecord.order[range] = ribbonIndex;
            }
        }
        g_BattleBlueMatraMagicStateFlag = BATTLE_BLUE_MATRA_MAGIC_SPAWN_FIRE;
        g_BattleBlueMatraMagicTargetMask = g_BattleBlueMatraMagicStateFlag;
    }
    if (g_BattleBlueMatraMagicStateFlag == BATTLE_BLUE_MATRA_MAGIC_SPAWN_FIRE && g_BattleBlueMatraMagicRibbonRecords[activeSlot->ribbonIndex].state == BATTLE_BLUE_MATRA_MAGIC_RIBBON_FREE) {
        ribbonIndex = activeSlot->ribbonIndex;
        (activeSlot->ribbonIndex)++;
        if (activeSlot->ribbonIndex >= 0x1C) {
            activeSlot->ribbonIndex = 0;
        }
        ribbonTask = (BattleMatraMagicRibbonSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueMatraMagicRibbonTask);
        ribbonTask->ribbonIndex = ribbonIndex;
        emitterIdx = g_BattleBlueMatraMagicEmitterRecord.order[g_BattleBlueMatraMagicTargetMask];
        g_BattleBlueMatraMagicEmitterRecord.emitters[emitterIdx].hitCount++;
        count = g_BattleBlueMatraMagicEmitterRecord.emitters[emitterIdx].spread;
        range = count << 1;
        index = count >> 1;
        index *= index;
        if (g_BattleBlueMatraMagicEmitterRecord.emitters[emitterIdx].hitCount == 1 || g_BattleBlueMatraMagicEmitterRecord.emitters[emitterIdx].hitCount == 4) {
            ribbonTask->isActive = 1;
        } else {
            ribbonTask->isActive = 0;
        }
        g_BattleBlueMatraMagicRibbonRecords[ribbonIndex].state = BATTLE_BLUE_MATRA_MAGIC_RIBBON_LAUNCH;
        *(Int64Pair *)&g_BattleBlueMatraMagicRibbonRecords[ribbonIndex].posX = *(Int64Pair *)&g_BattleBlueMatraMagicTrajectoryParams;
        *(Int64Pair *)&g_BattleBlueMatraMagicRibbonRecords[ribbonIndex].anchorX = *(Int64Pair *)&g_BattleBlueMatraMagicEmitterRecord.emitters[emitterIdx].posX;
        *(Int64Pair *)&g_BattleBlueMatraMagicRibbonRecords[ribbonIndex].targetX = *(Int64Pair *)&g_BattleBlueMatraMagicRibbonRecords[ribbonIndex].anchorX;
        g_BattleBlueMatraMagicRibbonRecords[ribbonIndex].targetX += (short)(rand() % range - count);
        g_BattleBlueMatraMagicRibbonRecords[ribbonIndex].targetY += (short)(rand() % range - count);
        g_BattleBlueMatraMagicRibbonRecords[ribbonIndex].targetZ += (short)(rand() % range - count);
        g_BattleBlueMatraMagicRibbonRecords[ribbonIndex].jitterRadius = count;
        g_BattleBlueMatraMagicRibbonRecords[ribbonIndex].arriveDistSq = index;
        g_BattleBlueMatraMagicRibbonRecords[ribbonIndex].trailHeadIndex = 0;
        g_BattleBlueMatraMagicRibbonRecords[ribbonIndex].trailTailIndex = 0;
        index = rand() % 4096;
        velX.vx = g_BattleBlueMatraMagicRibbonRecords[ribbonIndex].targetX - g_BattleBlueMatraMagicTrajectoryParams.x;
        velX.vy = g_BattleBlueMatraMagicRibbonRecords[ribbonIndex].targetY - g_BattleBlueMatraMagicTrajectoryParams.y;
        velX.vz = g_BattleBlueMatraMagicRibbonRecords[ribbonIndex].targetZ - g_BattleBlueMatraMagicTrajectoryParams.z;
        sideVector.vx = -velX.vy - velX.vz;
        sideVector.vy = velX.vx - velX.vz;
        sideVector.vz = velX.vx + velX.vy;
        NormalizeVectorFixed(&velX, &velX);
        NormalizeVectorFixed(&sideVector, &sideVector);
        Gte_BuildAxisAngleMatrix(index, &rot, &velX);
        TransformVectorToInt32(&rot, &sideVector, &sideVector);
        *(VECTOR *)&g_BattleBlueMatraMagicRibbonRecords[ribbonIndex].aimDirX = velX;
        *(VECTOR *)&g_BattleBlueMatraMagicRibbonRecords[ribbonIndex].velocityX = sideVector;
        for (index = 0; index < 6; index++) {
            g_BattleBlueMatraMagicRibbonRecords[ribbonIndex].trail[index].valid = 0;
        }
        g_BattleBlueMatraMagicTargetMask++;
        if (g_BattleBlueMatraMagicTargetMask >= g_BattleBlueMatraMagicEmitterRecord.targetCount) {
            emitterIndex = 0;
            for (count = 0; emitterIndex < g_BattleBlueMatraMagicEmitterRecord.targetCount; emitterIndex++) {
                if (g_BattleBlueMatraMagicEmitterRecord.emitters[emitterIndex].hitCount >= 8) {
                    count++;
                }
            }
            if (count >= g_BattleBlueMatraMagicEmitterRecord.targetCount) {
                g_BattleBlueMatraMagicStateFlag = BATTLE_BLUE_MATRA_MAGIC_SPAWN_FINISH;
            } else {
                g_BattleBlueMatraMagicStateFlag = BATTLE_BLUE_MATRA_MAGIC_SPAWN_SHUFFLE;
            }
        }
    }
    if (g_BattleBlueMatraMagicStateFlag == BATTLE_BLUE_MATRA_MAGIC_SPAWN_FINISH) {
        if (g_BattleBlueMatraMagicRibbonCount == 0xF) {
            for (emitterIndex = 0; emitterIndex < g_BattleBlueMatraMagicEmitterRecord.targetCount; emitterIndex++) {
                Battle_PostActorHitOrHurtMessage(g_BattleBlueMatraMagicEmitterRecord.emitters[emitterIndex].targetHandle);
            }
        }
        g_BattleBlueMatraMagicRibbonCount++;
    }
    if (g_BattleBlueMatraMagicRibbonCount >= 0x14) {
        activeSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    (activeSlot->frameCounter)++;
}

/*
 * Task updating motion, trail segments, and collisions for an individual Matra ribbon.
 */
// FUNCTION: FF7 0x0056b17c
void __cdecl BattleFx_BlueMatraMagicRibbonTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[148];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; int segmentCount; };
        struct { unsigned char pad_to_24[0x24]; BattleFxMatraMagicAnimSprite *spriteSlot; };
        struct { unsigned char pad_to_28[0x28]; int segmentIndex; };
        struct { unsigned char pad_to_2C[0x2C]; int updated; };
        struct { unsigned char pad_to_30[0x30]; int trailIndex; };
        struct { unsigned char pad_to_34[0x34]; int tailIndex; };
        struct { unsigned char pad_to_38[0x38]; short frameCounter; };
        struct { unsigned char pad_to_3C[0x3C]; int waveOffset; };
        struct { unsigned char pad_to_40[0x40]; BattleFxBlueMatraMagicSegmentWork *ribbonData; };
        struct { unsigned char pad_to_44[0x44]; BattleFxMatraMagicRibbonSlot *ribbonSlot; };
        struct { unsigned char pad_to_48[0x48]; short state; };
        struct { unsigned char pad_to_4C[0x4C]; int tmpAngle; };
        struct { unsigned char pad_to_58[0x58]; short stepX; };
        struct { unsigned char pad_to_5A[0x5A]; short stepY; };
        struct { unsigned char pad_to_5C[0x5C]; short stepZ; };
        struct { unsigned char pad_to_60[0x60]; int pointX; };
        struct { unsigned char pad_to_62[0x62]; short pointY; };
        struct { unsigned char pad_to_64[0x64]; int pointZ; };
        struct { unsigned char pad_to_70[0x70]; BattleFxBlueMatraMagicRibbon *ribbon; };
        struct { unsigned char pad_to_84[0x84]; VECTOR dirX; };
    } frame;
#pragma pack(pop)

    frame.ribbonSlot = (BattleFxMatraMagicRibbonSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.ribbon = &g_BattleBlueMatraMagicRibbonRecords[frame.ribbonSlot->recordIndex];
    frame.ribbonData = (BattleFxBlueMatraMagicSegmentWork *)Gfx_GetPsxScratchpadPtr(0);
    frame.frameCounter = frame.ribbonSlot->frameCounter;
    if (g_BattleIsPaused != 0) {
        frame.state = BATTLE_BLUE_MATRA_MAGIC_RIBBON_PAUSED;
    } else {
        frame.state = *(short *)&frame.ribbon->state;
    }
    frame.updated = 0;
    switch (frame.state) {
    case BATTLE_BLUE_MATRA_MAGIC_RIBBON_LAUNCH:
        frame.ribbon->state = BATTLE_BLUE_MATRA_MAGIC_RIBBON_RISE;
        frame.ribbon->speed = g_BattleBlueMatraMagicTrajectoryParams.peakRadius / 0xA;
        frame.ribbon->waveAmplitude = g_BattleBlueMatraMagicTrajectoryParams.peakRadius + (g_BattleBlueMatraMagicTrajectoryParams.peakRadius >> 1);
    case BATTLE_BLUE_MATRA_MAGIC_RIBBON_RISE:
        frame.stepX = frame.ribbon->speed * frame.ribbon->velocityX >> 0xC;
        frame.stepY = frame.ribbon->speed * frame.ribbon->velocityY >> 0xC;
        frame.stepZ = frame.ribbon->speed * frame.ribbon->velocityZ >> 0xC;
        frame.ribbon->posX += frame.stepX;
        frame.ribbon->posY += frame.stepY;
        frame.ribbon->posZ += frame.stepZ;
        frame.tmpAngle = (frame.frameCounter + 1) * 0xAA;
        frame.waveOffset = SinFixed(frame.tmpAngle) * frame.ribbon->waveAmplitude >> 0xC;
        *(short *)&frame.pointX = frame.ribbon->posX + (frame.waveOffset * -frame.ribbon->aimDirX >> 0xC);
        frame.pointY = frame.ribbon->posY + (frame.waveOffset * -frame.ribbon->aimDirY >> 0xC);
        *(short *)&frame.pointZ = frame.ribbon->posZ + (frame.waveOffset * -frame.ribbon->aimDirZ >> 0xC);
        frame.updated = 1;
        if (frame.frameCounter == 0xB) {
            frame.ribbon->state = BATTLE_BLUE_MATRA_MAGIC_RIBBON_AIM;
        }
        break;
    case BATTLE_BLUE_MATRA_MAGIC_RIBBON_AIM:
        frame.ribbon->state = BATTLE_BLUE_MATRA_MAGIC_RIBBON_HOME;
        frame.ribbon->speed = 0x25B;
        frame.ribbon->speedAccel = rand() % 0xE + 0x10;
        frame.ribbon->waveAmplitudeStep = rand() % 0xC + 0xC;
        frame.ribbon->clearedParam = 0;
        frame.ribbon->deltaX = frame.ribbon->targetX - frame.ribbon->posX;
        frame.ribbon->deltaY = frame.ribbon->targetY - frame.ribbon->posY;
        frame.ribbon->deltaZ = frame.ribbon->targetZ - frame.ribbon->posZ;
        frame.ribbon->vectorLength = NormalizeVectorFixed(&frame.ribbon->deltaX, &frame.ribbon->deltaX);
        frame.tmpAngle = ComputeAngleBetweenVectors((VECTOR *)&frame.ribbon->aimDirX, (VECTOR *)&frame.ribbon->deltaX, &frame.dirX);
        Gte_BuildAxisAngleMatrix(frame.tmpAngle, &frame.rotMatrix, &frame.dirX);
        TransformVectorToInt32(&frame.rotMatrix, &frame.ribbon->velocityX, &frame.ribbon->velocityX);
        frame.ribbon->waveAmplitude = (frame.tmpAngle >> 1) + frame.tmpAngle * 2;
        frame.ribbon->phaseStep = 0x1000 / ((rand() % 8) + 0xA);
        frame.ribbon->phase = frame.ribbon->phaseStep;
        frame.ribbon->phaseStepDecay = (rand() % 8) + 6;
    case BATTLE_BLUE_MATRA_MAGIC_RIBBON_HOME:
        frame.stepX = frame.ribbon->speed * frame.ribbon->deltaX >> 0xC;
        frame.stepY = frame.ribbon->speed * frame.ribbon->deltaY >> 0xC;
        frame.stepZ = frame.ribbon->speed * frame.ribbon->deltaZ >> 0xC;
        frame.ribbon->posX += frame.stepX;
        frame.ribbon->posY += frame.stepY;
        frame.ribbon->posZ += frame.stepZ;
        frame.waveOffset = SinFixed(frame.ribbon->phase) * frame.ribbon->waveAmplitude >> 0xC;
        *(short *)&frame.pointX = frame.ribbon->posX + (frame.waveOffset * frame.ribbon->velocityX >> 0xC);
        frame.pointY = frame.ribbon->posY + (frame.waveOffset * frame.ribbon->velocityY >> 0xC);
        *(short *)&frame.pointZ = frame.ribbon->posZ + (frame.waveOffset * frame.ribbon->velocityZ >> 0xC);
        frame.ribbon->speed += frame.ribbon->speedAccel;
        frame.ribbon->waveAmplitude += frame.ribbon->waveAmplitudeStep;
        frame.ribbon->phase += frame.ribbon->phaseStep;
        frame.ribbon->phaseStep -= frame.ribbon->phaseStepDecay;
        frame.updated = 1;
        frame.dirX.vx = frame.ribbon->targetX - frame.ribbon->posX;
        frame.dirX.vy = frame.ribbon->targetY - frame.ribbon->posY;
        frame.dirX.vz = frame.ribbon->targetZ - frame.ribbon->posZ;
        frame.tmpAngle = NormalizeVectorFixed(&frame.dirX, &frame.dirX);
        if (frame.ribbon->arriveDistSq > frame.tmpAngle || frame.ribbon->vectorLength < frame.tmpAngle) {
            frame.ribbon->state = BATTLE_BLUE_MATRA_MAGIC_RIBBON_IMPACT;
            *(Int64Pair *)&frame.ribbon->anchorX = *(Int64Pair *)(&frame.pointX);
        }
        frame.ribbon->vectorLength = frame.tmpAngle;
        break;
    case BATTLE_BLUE_MATRA_MAGIC_RIBBON_IMPACT:
        frame.ribbon->state = BATTLE_BLUE_MATRA_MAGIC_RIBBON_DRAIN_TRAIL;
        frame.ribbon->speed = frame.frameCounter;
        frame.spriteSlot = (BattleFxMatraMagicAnimSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueMatraMagicAnimatedSpriteTask);
        if (frame.ribbonSlot->flags != 0) {
            Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH2, Battle_ComputeScreenPanFromPosition(&frame.spriteSlot->posX), 0xFA);
        }
        *(Int64Pair *)&frame.spriteSlot->posX = *(Int64Pair *)&frame.ribbon->anchorX;
        frame.spriteSlot->scale = (frame.ribbon->jitterRadius << 0xC) / 0x12C;
    case BATTLE_BLUE_MATRA_MAGIC_RIBBON_DRAIN_TRAIL:
        frame.waveOffset = frame.ribbon->trailTailIndex;
        frame.ribbon->trail[frame.waveOffset].valid = 0;
        frame.waveOffset++;
        if (frame.waveOffset >= 6) {
            frame.waveOffset = 0;
        }
        if (frame.waveOffset == frame.ribbon->trailHeadIndex) {
            frame.ribbon->state = BATTLE_BLUE_MATRA_MAGIC_RIBBON_DONE;
        }
        frame.ribbon->trailTailIndex = frame.waveOffset;
        break;
    case BATTLE_BLUE_MATRA_MAGIC_RIBBON_DONE:
        frame.ribbonSlot->ownerSlot = BATTLE_TASK_FREE;
        frame.ribbon->state = BATTLE_BLUE_MATRA_MAGIC_RIBBON_FREE;
    }
    if (frame.updated != 0) {
        frame.tmpAngle = frame.ribbon->trailHeadIndex;
        frame.waveOffset = frame.ribbon->trailTailIndex;
        *(Int64Pair *)&frame.ribbon->trail[frame.tmpAngle] = *(Int64Pair *)(&frame.pointX);
        frame.ribbon->trail[frame.tmpAngle].valid = 1;
        frame.tmpAngle++;
        if (frame.tmpAngle >= 6) {
            frame.tmpAngle = 0;
        }
        if (frame.tmpAngle == frame.waveOffset) {
            frame.ribbon->trail[frame.waveOffset].valid = 0;
            frame.waveOffset++;
            if (frame.waveOffset >= 6) {
                frame.waveOffset = 0;
            }
        }
        frame.ribbon->trailHeadIndex = frame.tmpAngle;
        frame.ribbon->trailTailIndex = frame.waveOffset;
    }
    frame.tailIndex = frame.ribbon->trailTailIndex;
    *(Int64Pair *)&frame.ribbonData->endX = *(Int64Pair *)&frame.ribbon->trail[frame.tailIndex];
    *(Int64Pair *)&frame.ribbonData->startX = *(Int64Pair *)&frame.ribbonData->endX;
    frame.ribbonData->startX -= 0x64;
    frame.ribbonData->startY -= 0x64;
    frame.ribbonData->startZ -= 0x64;
    frame.ribbonData->colorBottom = 0xC0C0C;
    frame.ribbonData->baseColor = 0xC0C0C;
    BattleFx_BlueMatraMagicProjectRibbonSegmentQuad(frame.ribbonData, 0xC, 0);
    frame.segmentIndex = 0;
    frame.tmpAngle = frame.tailIndex;
    for (frame.segmentCount = 0;; frame.tmpAngle++, frame.segmentIndex++) {
        if (frame.tmpAngle >= 6) {
            frame.tmpAngle = 0;
        }
        frame.waveOffset = frame.tmpAngle + 1;
        if (frame.waveOffset >= 6) {
            frame.waveOffset = 0;
        }
        if (frame.ribbon->trail[frame.waveOffset].valid == 0) {
            break;
        }
        frame.segmentCount++;
    }
    frame.tmpAngle = frame.tailIndex;
    for (frame.segmentIndex = 1;; frame.tmpAngle++, frame.segmentIndex++) {
        if (frame.tmpAngle >= 6) {
            frame.tmpAngle = 0;
        }
        frame.waveOffset = frame.tmpAngle + 1;
        if (frame.waveOffset >= 6) {
            frame.waveOffset = 0;
        }
        if (frame.ribbon->trail[frame.waveOffset].valid == 0) {
            break;
        }
        *(Int64Pair *)&frame.ribbonData->startX = *(Int64Pair *)&frame.ribbon->trail[frame.tmpAngle];
        *(Int64Pair *)&frame.ribbonData->endX = *(Int64Pair *)&frame.ribbon->trail[frame.waveOffset];
        for (frame.trailIndex = 0; frame.trailIndex < 4; frame.trailIndex++) {
            frame.ribbonData->quadVertexPrev[frame.trailIndex] = frame.ribbonData->quadVertex[frame.trailIndex];
            frame.ribbonData->blendFactorA[frame.trailIndex] = frame.ribbonData->screenX[frame.trailIndex];
            frame.ribbonData->blendFactorB[frame.trailIndex] = frame.ribbonData->screenY[frame.trailIndex];
        }
        frame.ribbonData->prevDepth = frame.ribbonData->depth;
        frame.ribbonData->colorStart = frame.ribbonData->colorBottom;
        BattleFx_BlueMatraMagicProjectRibbonSegmentQuad(frame.ribbonData, 0x30, 0);
        BattleFx_BlueMatraMagicDrawPsxGouraudQuads(frame.ribbonData, frame.segmentIndex, frame.segmentCount, 0);
        if (frame.segmentIndex == frame.segmentCount && frame.ribbon->state < BATTLE_BLUE_MATRA_MAGIC_RIBBON_IMPACT) {
            BattleFx_BlueMatraMagicAdvanceDrawModel(frame.ribbonData);
        }
    }
    if (frame.frameCounter >= 0x37) {
        frame.ribbonSlot->ownerSlot = BATTLE_TASK_FREE;
        frame.ribbon->state = BATTLE_BLUE_MATRA_MAGIC_RIBBON_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.ribbonSlot->frameCounter)++;
}

/*
 * Task animating and rendering explosion sprites for Matra impacts.
 */
// FUNCTION: FF7 0x0056ba62
void __cdecl BattleFx_BlueMatraMagicAnimatedSpriteTask(void)
{
    BattleFxMatraMagicAnimSprite *task;

    task = (BattleFxMatraMagicAnimSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleBlueMatraMagicSpriteDrawReq.matraSpriteDrawFrame = task->frame;
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->scale, -(task->scale >> 3));
    g_BattleBlueMatraMagicSpriteDrawReq.matraSpriteGroup = Gfx_ModelTableGetLoadedRecord(0, g_BattleBlueMatraMagicEmitterRecord.resourceHandle);
    g_BattleBlueMatraMagicSpriteDrawReq.matraSpriteDrawHandle = Gfx_ModelTableGetSpriteObject(0, g_BattleBlueMatraMagicEmitterRecord.resourceHandle);
    g_BattleBlueMatraMagicDrawListBuffer1 = BattleFx_SubmitSpriteDraw(&g_BattleBlueMatraMagicSpriteDrawReq.matraSpriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleBlueMatraMagicDrawListBuffer1);
    if (task->frame >= 0xB) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (task->frame)++;
}

/*
 * Renders Gouraud-shaded quad polygons for Matra ribbon trails.
 */
// FUNCTION: FF7 0x0056bb31
void __cdecl BattleFx_BlueMatraMagicDrawPsxGouraudQuads(BattleFxBlueMatraMagicSegmentWork *ribbon, int step, int count, int useCache)
{
    int colorStart, colorBottom;
    BattleFxMatraMagicQuadPacket *packetPtr;
    int index, next, order;

    if (useCache != 0) {
        colorStart = ribbon->colorStart;
        colorBottom = ribbon->colorBottom;
    } else {
        SetDrawColorRGB(0x60, 0x60, 0x60);
        index = 0x1000 / count * step;
        Gfx_ShadePsxVertexColor((CVECTOR *)&ribbon->baseColor, index, (CVECTOR *)&ribbon->colorBottom);
        colorStart = ribbon->colorBottom;
        colorBottom = ribbon->colorStart;
    }
    packetPtr = (BattleFxMatraMagicQuadPacket *)Gfx_GetPsxPacketScratchPtr();
    order = ribbon->prevDepth + ribbon->depth >> 1 >> 2;
    for (index = 0; index < 4; index++) {
        next = index + 1 & 3;
        packetPtr->vertex0 = ribbon->quadVertex[index];
        packetPtr->vertex1 = ribbon->quadVertex[next];
        packetPtr->vertex2 = ribbon->quadVertexPrev[index];
        packetPtr->vertex3 = ribbon->quadVertexPrev[next];
        packetPtr->colorStart = colorStart;
        packetPtr->color1 = colorStart;
        packetPtr->colorBottom = colorBottom;
        packetPtr->color3 = colorBottom;
        packetPtr->tag = packetPtr->tag & 0xFFFFFF | 0x8000000;
        *(char *)((char *)packetPtr + 7) = 0x3A;
        Gfx_SetBlendFactorQuad(*(float *)&ribbon->screenX[index], *(float *)&ribbon->screenY[index], *(float *)&ribbon->screenX[next], *(float *)&ribbon->screenY[next], *(float *)&ribbon->blendFactorA[index], *(float *)&ribbon->blendFactorB[index], *(float *)&ribbon->blendFactorA[next], *(float *)&ribbon->blendFactorB[next]);
        Gfx_DrawPsxPrimitive(packetPtr);
    }
    g_BattleBlueMatraMagicDrawListBuffer1 = (int)packetPtr;
}

/*
 * Projects edge vertex coordinates for an individual Matra ribbon segment quad.
 */
// FUNCTION: FF7 0x0056bcf4
void __cdecl BattleFx_BlueMatraMagicProjectRibbonSegmentQuad(BattleFxBlueMatraMagicSegmentWork *segment, int halfSize, int advanceFlag)
{
    int scale;
    BattleFxProjectedPoint *projectedPtr;
    int res, extent;

    if (advanceFlag != 0) {
        scale = SqrtToInt(segment->deltaLength) >> 2;
        segment->rotation.t[0] += scale * segment->deltaX >> 0xC;
        segment->rotation.t[1] += scale * segment->deltaY >> 0xC;
        segment->rotation.t[2] += scale * segment->deltaZ >> 0xC;
    } else {
        segment->deltaX = segment->endX - segment->startX;
        segment->deltaY = segment->endY - segment->startY;
        segment->deltaZ = segment->endZ - segment->startZ;
        segment->deltaLength = NormalizeVectorFixed(&segment->deltaX, &segment->deltaX);
        scale = ComputeAngleBetweenVectors((VECTOR *)&g_BattleBlueMatraMagicModelDrawReq.restDirX, (VECTOR *)&segment->deltaX, &segment->axis);
        Gte_BuildAxisAngleMatrix(scale, &segment->rotation, &segment->axis);
        segment->rotation.t[0] = segment->endX;
        segment->rotation.t[1] = segment->endY;
        segment->rotation.t[2] = segment->endZ;
    }
    segment->corner0X = 0;
    segment->corner0Y = -halfSize;
    segment->corner0Z = 0;
    segment->corner1X = halfSize;
    segment->corner1Y = 0;
    segment->corner1Z = 0;
    segment->corner2X = 0;
    segment->corner2Y = halfSize;
    segment->corner2Z = 0;
    segment->corner3X = -halfSize;
    segment->corner3Y = 0;
    segment->corner3Z = 0;
    MultiplyMatrix(&g_BattleCameraTransform, &segment->rotation, &segment->cameraRotation);
    SetRotMatrix(&segment->cameraRotation);
    SetTransMatrix(&segment->cameraRotation);
    RefreshGlobalTransformState();
    segment->depth = Gfx_ProjectQuadToScreenAlt((SVECTOR *)&segment->corner0X, (SVECTOR *)&segment->corner1X, (SVECTOR *)&segment->corner2X, (SVECTOR *)&segment->corner3X, &segment->quadVertex[0], &segment->quadVertex[1], &segment->quadVertex[2], &segment->quadVertex[3], &res, &extent);
    projectedPtr = (BattleFxProjectedPoint *)Gfx_GetProjectedVertexRecord(0);
    segment->screenX[0] = projectedPtr->screenX;
    segment->screenY[0] = projectedPtr->screenY;
    projectedPtr = (BattleFxProjectedPoint *)Gfx_GetProjectedVertexRecord(1);
    segment->screenX[1] = projectedPtr->screenX;
    segment->screenY[1] = projectedPtr->screenY;
    projectedPtr = (BattleFxProjectedPoint *)Gfx_GetProjectedVertexRecord(2);
    segment->screenX[2] = projectedPtr->screenX;
    segment->screenY[2] = projectedPtr->screenY;
    projectedPtr = (BattleFxProjectedPoint *)Gfx_GetProjectedVertexRecord(3);
    segment->screenX[3] = projectedPtr->screenX;
    segment->screenY[3] = projectedPtr->screenY;
}

/*
 * Advances and renders the 3D missile model along the Matra trajectory.
 */
// FUNCTION: FF7 0x0056c02c
void __cdecl BattleFx_BlueMatraMagicAdvanceDrawModel(BattleFxBlueMatraMagicSegmentWork *missile)
{
    VECTOR scaleVec;
    int step;
    BattleFxModelDraw *drawReq;

    step = SqrtToInt(missile->deltaLength) >> 2;
    missile->rotation.t[0] += step * missile->deltaX >> 0xC;
    missile->rotation.t[1] += step * missile->deltaY >> 0xC;
    missile->rotation.t[2] += step * missile->deltaZ >> 0xC;
    scaleVec.vz = g_BattleBlueMatraMagicRibbonAngle;
    scaleVec.vy = scaleVec.vz;
    scaleVec.vx = scaleVec.vy;
    ScaleMatrix(&missile->rotation, &scaleVec);
    MultiplyMatrix(&g_BattleCameraTransform, &missile->rotation, &missile->cameraRotation);
    SetRotMatrix(&missile->cameraRotation);
    SetTransMatrix(&missile->cameraRotation);
    drawReq = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    drawReq->modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleBlueMatraMagicEmitterRecord.resourceHandle);
    drawReq->flags = 0x80;
    drawReq->uvScroll = 0;
    drawReq->fadeLevel = 0;
    drawReq->stateFlags = 0;
    drawReq->texturePageBits = 0;
    g_BattleBlueMatraMagicDrawListBuffer1 = BattleFx_SubmitModelDraw((BattleFxModelDraw *)drawReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleBlueMatraMagicDrawListBuffer1);
}
} /* extern "C" */
