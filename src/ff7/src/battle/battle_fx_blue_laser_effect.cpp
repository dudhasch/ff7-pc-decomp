// src/ff7/src/battle/battle_fx_blue_laser_effect.cpp
// Enemy Skill [9]: Laser (Japanese: レーザー, dev tag: "blue/reazer" / Reazer)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The Reazer blue magic effect: laser ribbons toward targets with trails and impact
//   sprites, drawn as PSX-style quads with its own draw-list buffer.

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

struct BattleFxVector3;
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)



// clang-format off
// GLOBAL: FF7 0x008a9010
int g_BattleBlueLaserResourceDef = 0x00192A39;
char *g_BattleFxBlueLaserEffectResourceNames[1] = {
    // STRING: FF7 0x008a9048
    "REAZER.TIM"
};
unsigned int g_BattleFxBlueLaserEffectAssetPrefix[4] = {
    // STRING: FF7 0x008a9054
    (unsigned int)"REAZER.SPT",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x008a9028
unsigned char g_BattleBlueLaserSpriteDrawReq[32] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0x80, 0x80, 0x2e, 0x00, 0x00, 0x00, 0x00
};
// GLOBAL: FF7 0x00ba3870
struct BattleFxBlueLaserState g_BattleBlueLaserState = {0};
unsigned char g_bssPad_00ba49f8[64] = {0};
// GLOBAL: FF7 0x00ba4a38
int g_BattleBlueLaserTargetMask = 0;
// GLOBAL: FF7 0x00ba4a3c
struct GfxTableCursor *g_BattleBlueLaserLoadedRes = 0;
// clang-format on
// The TU's own view of moved definitions declared with another type.

/*
 * Loads resources for the Reazer blue magic effect when not already loaded.
 */
// FUNCTION: FF7 0x0056c1a0
void __cdecl BattleFx_BlueLoadResLaser(void)
{
    GfxModelTableBank *sceneLinkRef;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathBufferSlot[200];
    GfxResourceRequest *pRequest;
    int defEntryRef;

    if (g_pBattle3dScene != 0 && g_BattleBlueLaserLoadedRes == 0) {
        kind = 1;
        defEntryRef = (int)&g_BattleBlueLaserResourceDef;
        sceneLinkRef = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a9060
        pRequest->resourcePath = BattleFx_BuildResourcePath("blue/reazer", pathBufferSlot);
        Gfx_LoadModelTableIfAbsent(kind, defEntryRef, pRequest, &g_BattleBlueLaserLoadedRes, sceneLinkRef);
    }
}

/*
 * Entry point for the Reazer blue magic effect.
 */
// FUNCTION: FF7 0x0056c230
void __cdecl BattleFx_BlueEnterLaser(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResLaser();
    BattleFx_BlueBeginLaser(targetMask, actorIdx);
}

/*
 * Initializes and begins the Reazer blue magic effect.
 */
// FUNCTION: FF7 0x0056c24a
void __cdecl BattleFx_BlueBeginLaser(int targetMask, int actorIdx)
{
    int idx;
    int count;

    g_BattleFxDrawListBufferTaskSlotPtr = (unsigned char *)BATTLE_NEW_TASK_SLOT(BattleFx_LaserSelectDrawListBuffer);
    idx = 0;
    for (count = 0; idx < 10; idx++) {
        if ((targetMask & 1 << idx) != 0) {
            count++;
            if (count == 1) {
                g_BattleBlueLaserState.actorIndex = idx;
            }
        }
    }
    g_BattleBlueLaserTargetMask = actorIdx;
    g_BattleBlueLaserState.spawnStep = 0;
    g_BattleBlueLaserState.ribbonCount = 0;
    g_BattleBlueLaserState.ribbonPhase = 0;
    Battle_AllocateResourceSlot(BattleFx_BlueLaserSetupTask);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH1, Battle_ComputePanForEntity(targetMask), 0x62);
}

/*
 * Setup task configuring Reazer laser parameters and draw list buffers.
 */
// FUNCTION: FF7 0x0056c307
void __cdecl BattleFx_BlueLaserSetupTask(void)
{
    int rad;
    BattleFxLaserSetupSlot *task;

    task = (BattleFxLaserSetupSlot *)BATTLE_CURRENT_TASK_SLOT;
    task->status = BATTLE_TASK_FREE;
    task = (BattleFxLaserSetupSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueLaserSpawnRibbonsTask);
    Battle_GetBoneWorldPos(g_BattleBlueLaserState.actorIndex, g_BattleActorRecords[g_BattleBlueLaserState.actorIndex].targetBone, &task->bonePos.x);
    rad = g_BattleActorRecords[g_BattleBlueLaserState.actorIndex].radius + 0xAA;
    if (rad > 1266) {
        rad = 0x4F2;
    }
    task->jitterRadius = rad;
    task->stepIndex = 0;
    Battle_GetBoneWorldPos(g_BattleBlueLaserTargetMask, g_BattleActorRecords[g_BattleBlueLaserTargetMask].targetBone, &g_BattleBlueLaserState.target.x);
    g_BattleBlueLaserState.range = g_BattleActorRecords[g_BattleBlueLaserTargetMask].radius + 300;
    if (g_BattleBlueLaserState.range > 1900) {
        g_BattleBlueLaserState.range = 1900;
    }
}

/*
 * Task driving the emission of laser ribbons toward targets for Reazer.
 */
// FUNCTION: FF7 0x0056c40e
void __cdecl BattleFx_BlueLaserSpawnRibbonsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[148];
        int rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxLaserSpawnSlot *reazerSlot; };
        struct { unsigned char pad_to_24[0x24]; short spawnFrameA; };
        struct { unsigned char pad_to_26[0x26]; short spawnFrameB; };
        struct { unsigned char pad_to_28[0x28]; short spawnFrameC; };
        struct { unsigned char pad_to_2A[0x2A]; short spawnFrameD; };
        struct { unsigned char pad_to_2C[0x2C]; short spawnFrameE; };
        struct { unsigned char pad_to_2E[0x2E]; short spawnFrameF; };
        struct { unsigned char pad_to_30[0x30]; short spawnFrameG; };
        struct { unsigned char pad_to_32[0x32]; short spawnFrameH; };
        struct { unsigned char pad_to_34[0x34]; short spawnFrameI; };
        struct { unsigned char pad_to_36[0x36]; short spawnFrameJ; };
        struct { unsigned char pad_to_38[0x38]; short spawnFrameK; };
        struct { unsigned char pad_to_3A[0x3A]; short spawnFrameL; };
        struct { unsigned char pad_to_3C[0x3C]; int tmpSpread; };
        struct { unsigned char pad_to_40[0x40]; int ribbonIndex; };
        struct { unsigned char pad_to_44[0x44]; int jitterRange; };
        struct { unsigned char pad_to_48[0x48]; int jitterRadius; };
        struct { unsigned char pad_to_4C[0x4C]; int tableIndex; };
        struct { unsigned char pad_to_50[0x50]; BattleFxLaserRibbonTaskSlot *ribbonSlot; };
        struct { unsigned char pad_to_74[0x74]; int vecX; };
        struct { unsigned char pad_to_78[0x78]; int vecY; };
        struct { unsigned char pad_to_7C[0x7C]; int vecZ; };
        struct {
            unsigned char pad[128];
            int value;
        } at20;
        struct { unsigned char pad_to_84[0x84]; int dirX; };
        struct { unsigned char pad_to_88[0x88]; int dirY; };
        struct { unsigned char pad_to_8C[0x8C]; int dirZ; };
        struct {
            unsigned char pad[144];
            int value;
        } at4;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
#pragma pack(pop)

    frame.spawnFrameA = 1;
    frame.spawnFrameB = 1;
    frame.spawnFrameC = 1;
    frame.spawnFrameD = 2;
    frame.spawnFrameE = 3;
    frame.spawnFrameF = 3;
    frame.spawnFrameG = 5;
    frame.spawnFrameH = 6;
    frame.spawnFrameI = 8;
    frame.spawnFrameJ = 9;
    frame.spawnFrameK = 10;
    frame.spawnFrameL = 11;
    frame.reazerSlot = (BattleFxLaserSpawnSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.reazerSlot->stepIndex < 12) {
        frame.jitterRadius = frame.reazerSlot->jitterRadius;
        frame.jitterRange = frame.jitterRadius << 1;
        for (frame.tableIndex = 0; frame.tableIndex < 12; frame.tableIndex++) {
            if (frame.reazerSlot->frameCounter != (&frame.spawnFrameA)[frame.tableIndex]) {
                continue;
            }
            frame.ribbonIndex = frame.reazerSlot->stepIndex;
            frame.tmpSpread = frame.jitterRadius >> 1;
            frame.tmpSpread *= frame.tmpSpread;
            (frame.reazerSlot->stepIndex)++;
            frame.ribbonSlot = (BattleFxLaserRibbonTaskSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueLaserRibbonTask);
            frame.ribbonSlot->ribbonIndex = *(short *)&frame.ribbonIndex;
            g_BattleBlueLaserState.ribbons[frame.ribbonIndex].state = BATTLE_BLUE_LASER_RIBBON_LAUNCH;
            g_BattleBlueLaserState.ribbons[frame.ribbonIndex].pos = g_BattleBlueLaserState.target;
            g_BattleBlueLaserState.ribbons[frame.ribbonIndex].anchor = frame.reazerSlot->bonePos;
            g_BattleBlueLaserState.ribbons[frame.ribbonIndex].target = g_BattleBlueLaserState.ribbons[frame.ribbonIndex].anchor;
            g_BattleBlueLaserState.ribbons[frame.ribbonIndex].target.x += (short)(rand() % frame.jitterRange - frame.jitterRadius);
            g_BattleBlueLaserState.ribbons[frame.ribbonIndex].target.y += (short)(rand() % frame.jitterRange - frame.jitterRadius);
            g_BattleBlueLaserState.ribbons[frame.ribbonIndex].target.z += (short)(rand() % frame.jitterRange - frame.jitterRadius);
            g_BattleBlueLaserState.ribbons[frame.ribbonIndex].jitterRadius = *(short *)&frame.jitterRadius;
            g_BattleBlueLaserState.ribbons[frame.ribbonIndex].arriveDistSq = frame.tmpSpread;
            g_BattleBlueLaserState.ribbons[frame.ribbonIndex].trailHeadIndex = 0;
            g_BattleBlueLaserState.ribbons[frame.ribbonIndex].trailTailIndex = 0;
            frame.tmpSpread = rand() % 4096;
            frame.dirX = g_BattleBlueLaserState.ribbons[frame.ribbonIndex].target.x - g_BattleBlueLaserState.target.x;
            frame.dirY = g_BattleBlueLaserState.ribbons[frame.ribbonIndex].target.y - g_BattleBlueLaserState.target.y;
            frame.dirZ = g_BattleBlueLaserState.ribbons[frame.ribbonIndex].target.z - g_BattleBlueLaserState.target.z;
            frame.vecX = -frame.dirY - frame.dirZ;
            frame.vecY = frame.dirX - frame.dirZ;
            frame.vecZ = frame.dirX + frame.dirY;
            NormalizeVectorFixed(&frame.dirX, &frame.dirX);
            NormalizeVectorFixed(&frame.vecX, &frame.vecX);
            Gte_BuildAxisAngleMatrix(frame.tmpSpread, (PsxMatrix *)&frame.rotMatrix, (VECTOR *)&frame.dirX);
            TransformVectorToInt32((PsxMatrix *)&frame.rotMatrix, &frame.vecX, &frame.vecX);
            *(BattleVec4i *)&g_BattleBlueLaserState.ribbons[frame.ribbonIndex].aimDirX = *(BattleVec4i *)&frame.dirX;
            *(BattleVec4i *)&g_BattleBlueLaserState.ribbons[frame.ribbonIndex].velocityX = *(BattleVec4i *)&frame.vecX;
            for (frame.tmpSpread = 0; frame.tmpSpread < 24; frame.tmpSpread++) {
                g_BattleBlueLaserState.ribbons[frame.ribbonIndex].trail[frame.tmpSpread].w = 0;
            }
        }
    }
    if (g_BattleBlueLaserState.ribbonCount >= 8) {
        if (g_BattleBlueLaserState.spawnStep == 15) {
            Battle_PostActorHitOrHurtMessage(g_BattleBlueLaserState.actorIndex);
        }
        g_BattleBlueLaserState.spawnStep++;
    }
    if (g_BattleBlueLaserState.spawnStep >= 20) {
        frame.reazerSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    (frame.reazerSlot->frameCounter)++;
}

/*
 * Task updating ribbon trajectory and vertex trail segments for Reazer.
 */
// FUNCTION: FF7 0x0056c828
void __cdecl BattleFx_BlueLaserRibbonTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[148];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; int segmentCount; };
        struct { unsigned char pad_to_24[0x24]; BattleFxMatraMagicAnimSprite *animSlot; };
        struct { unsigned char pad_to_28[0x28]; int segmentIndex; };
        struct { unsigned char pad_to_2C[0x2C]; int recordTrail; };
        struct { unsigned char pad_to_30[0x30]; int wordIndex; };
        struct { unsigned char pad_to_34[0x34]; int headIndex; };
        struct { unsigned char pad_to_38[0x38]; short ribbonStep; };
        struct { unsigned char pad_to_3C[0x3C]; int sinOffset; };
        struct { unsigned char pad_to_40[0x40]; BattleFxBlueLaserSegmentWork *ribbonBlock; };
        struct { unsigned char pad_to_44[0x44]; short state; };
        struct { unsigned char pad_to_48[0x48]; int tmpAngle; };
        struct { unsigned char pad_to_54[0x54]; BattleFxLaserRibbonTaskSlot *slot; };
        struct { unsigned char pad_to_58[0x58]; short stepX; };
        struct { unsigned char pad_to_5A[0x5A]; short stepY; };
        struct { unsigned char pad_to_5C[0x5C]; short stepZ; };
        struct { unsigned char pad_to_60[0x60]; int pointX; };
        struct { unsigned char pad_to_62[0x62]; short pointY; };
        struct { unsigned char pad_to_64[0x64]; int pointZ; };
        struct { unsigned char pad_to_70[0x70]; BattleFxBlueLaserRibbon *ribbon; };
        struct { unsigned char pad_to_84[0x84]; VECTOR dirX; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxLaserRibbonTaskSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.ribbon = &g_BattleBlueLaserState.ribbons[frame.slot->ribbonIndex];
    frame.ribbonBlock = (BattleFxBlueLaserSegmentWork *)Gfx_GetPsxScratchpadPtr(0);
    frame.ribbonStep = frame.slot->step;
    if (g_BattleIsPaused != 0) {
        frame.state = BATTLE_BLUE_LASER_RIBBON_PAUSED;
    } else {
        frame.state = *(short *)&frame.ribbon->state;
    }
    frame.recordTrail = 0;
    switch (frame.state) {
    case BATTLE_BLUE_LASER_RIBBON_LAUNCH:
        frame.ribbon->state = BATTLE_BLUE_LASER_RIBBON_RISE;
        frame.ribbon->speed = g_BattleBlueLaserState.range / 8;
        frame.ribbon->waveAmplitude = g_BattleBlueLaserState.range + (g_BattleBlueLaserState.range >> 1);
    case BATTLE_BLUE_LASER_RIBBON_RISE:
        frame.stepX = frame.ribbon->speed * frame.ribbon->velocityX >> 12;
        frame.stepY = frame.ribbon->speed * frame.ribbon->velocityY >> 12;
        frame.stepZ = frame.ribbon->speed * frame.ribbon->velocityZ >> 12;
        frame.ribbon->pos.x += frame.stepX;
        frame.ribbon->pos.y += frame.stepY;
        frame.ribbon->pos.z += frame.stepZ;
        frame.tmpAngle = frame.ribbonStep + 1 << 8;
        frame.sinOffset = SinFixed(frame.tmpAngle) * frame.ribbon->waveAmplitude >> 12;
        *(short *)&frame.pointX = frame.ribbon->pos.x + (frame.sinOffset * -frame.ribbon->aimDirX >> 12);
        frame.pointY = frame.ribbon->pos.y + (frame.sinOffset * -frame.ribbon->aimDirY >> 12);
        *(short *)&frame.pointZ = frame.ribbon->pos.z + (frame.sinOffset * -frame.ribbon->aimDirZ >> 12);
        frame.recordTrail = 1;
        if (frame.ribbonStep == 7) {
            frame.ribbon->state = BATTLE_BLUE_LASER_RIBBON_AIM;
        }
        break;
    case BATTLE_BLUE_LASER_RIBBON_AIM:
        frame.ribbon->state = BATTLE_BLUE_LASER_RIBBON_HOME;
        frame.ribbon->speed = 507;
        frame.ribbon->speedAccel = rand() % 14 + 12;
        frame.ribbon->waveAmplitudeStep = (rand() % 8) + 8;
        frame.ribbon->timer = 0;
        frame.ribbon->deltaX = frame.ribbon->target.x - frame.ribbon->pos.x;
        frame.ribbon->deltaY = frame.ribbon->target.y - frame.ribbon->pos.y;
        frame.ribbon->deltaZ = frame.ribbon->target.z - frame.ribbon->pos.z;
        frame.ribbon->vectorLength = NormalizeVectorFixed(&frame.ribbon->deltaX, &frame.ribbon->deltaX);
        frame.tmpAngle = ComputeAngleBetweenVectors((VECTOR *)&frame.ribbon->aimDirX, (VECTOR *)&frame.ribbon->deltaX, &frame.dirX);
        Gte_BuildAxisAngleMatrix(frame.tmpAngle, &frame.rotMatrix, &frame.dirX);
        TransformVectorToInt32(&frame.rotMatrix, &frame.ribbon->velocityX, &frame.ribbon->velocityX);
        frame.ribbon->waveAmplitude = (frame.tmpAngle >> 1) + frame.tmpAngle * 2 + (rand() % 64);
        frame.ribbon->phaseStep = 4096 / ((rand() % 8) + 10);
        frame.ribbon->phase = frame.ribbon->phaseStep;
        frame.ribbon->phaseStepDecay = (rand() % 8) + 4;
    case BATTLE_BLUE_LASER_RIBBON_HOME:
        frame.stepX = frame.ribbon->speed * frame.ribbon->deltaX >> 12;
        frame.stepY = frame.ribbon->speed * frame.ribbon->deltaY >> 12;
        frame.stepZ = frame.ribbon->speed * frame.ribbon->deltaZ >> 12;
        frame.ribbon->pos.x += frame.stepX;
        frame.ribbon->pos.y += frame.stepY;
        frame.ribbon->pos.z += frame.stepZ;
        frame.sinOffset = SinFixed(frame.ribbon->phase) * frame.ribbon->waveAmplitude >> 12;
        *(short *)&frame.pointX = frame.ribbon->pos.x + (frame.sinOffset * frame.ribbon->velocityX >> 12);
        frame.pointY = frame.ribbon->pos.y + (frame.sinOffset * frame.ribbon->velocityY >> 12);
        *(short *)&frame.pointZ = frame.ribbon->pos.z + (frame.sinOffset * frame.ribbon->velocityZ >> 12);
        frame.ribbon->speed += frame.ribbon->speedAccel;
        frame.ribbon->waveAmplitude += frame.ribbon->waveAmplitudeStep;
        frame.ribbon->phase += frame.ribbon->phaseStep;
        frame.ribbon->phaseStep -= frame.ribbon->phaseStepDecay;
        frame.recordTrail = 1;
        if (frame.ribbon->timer != 0) {
            (frame.ribbon->timer)++;
            if (frame.ribbon->timer >= 12) {
                frame.ribbon->state = BATTLE_BLUE_LASER_RIBBON_IMPACT;
            }
            break;
        }
        frame.dirX.vx = frame.ribbon->target.x - frame.ribbon->pos.x;
        frame.dirX.vy = frame.ribbon->target.y - frame.ribbon->pos.y;
        frame.dirX.vz = frame.ribbon->target.z - frame.ribbon->pos.z;
        frame.tmpAngle = NormalizeVectorFixed(&frame.dirX, &frame.dirX);
        if (frame.ribbon->arriveDistSq > frame.tmpAngle || frame.ribbon->vectorLength < frame.tmpAngle) {
            g_BattleBlueLaserState.ribbonCount++;
            if (g_BattleBlueLaserState.ribbonPhase < 4) {
                if (g_BattleBlueLaserState.actorIndex == g_BattleBlueLaserTargetMask) {
                    frame.sinOffset = (rand() % 4);
                    g_BattleBlueLaserState.ribbonPhase = 0;
                } else {
                    frame.sinOffset = (rand() % 16);
                }
                if (frame.sinOffset == 2) {
                    g_BattleBlueLaserState.ribbonPhase++;
                    frame.ribbon->speedAccel <<= 1;
                    frame.ribbon->timer = 1;
                    break;
                }
            }
            frame.ribbon->state = BATTLE_BLUE_LASER_RIBBON_IMPACT;
            *(Int64Pair *)&frame.ribbon->anchor = *(Int64Pair *)&frame.pointX;
        }
        frame.ribbon->vectorLength = frame.tmpAngle;
        break;
    case BATTLE_BLUE_LASER_RIBBON_IMPACT:
        frame.ribbon->state = BATTLE_BLUE_LASER_RIBBON_DRAIN_TRAIL;
        frame.ribbon->speed = frame.ribbonStep;
        if (frame.ribbon->timer == 0) {
            frame.animSlot = (BattleFxMatraMagicAnimSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueLaserAnimatedSpriteTask);
            *(Int64Pair *)&frame.animSlot->posX = *(Int64Pair *)&frame.ribbon->anchor;
            frame.animSlot->scale = (frame.ribbon->jitterRadius << 12) / 300;
            if (g_BattleBlueLaserState.ribbonCount % 3 == 1) {
                Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH2, Battle_ComputeScreenPanFromPosition(&frame.animSlot->posX), 0xFA);
            }
        }
    case BATTLE_BLUE_LASER_RIBBON_DRAIN_TRAIL:
        frame.sinOffset = frame.ribbon->trailTailIndex;
        frame.ribbon->trail[frame.sinOffset].w = 0;
        frame.sinOffset++;
        if (frame.sinOffset >= 0x18) {
            frame.sinOffset = 0;
        }
        if (frame.sinOffset == frame.ribbon->trailHeadIndex) {
            frame.ribbon->state = BATTLE_BLUE_LASER_RIBBON_DONE;
        }
        frame.ribbon->trailTailIndex = frame.sinOffset;
        break;
    case BATTLE_BLUE_LASER_RIBBON_DONE:
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (frame.recordTrail != 0) {
        frame.tmpAngle = frame.ribbon->trailHeadIndex;
        frame.sinOffset = frame.ribbon->trailTailIndex;
        frame.ribbon->trail[frame.tmpAngle] = *(BattleFxLaserPoint *)&frame.pointX;
        frame.ribbon->trail[frame.tmpAngle].w = 1;
        frame.tmpAngle++;
        if (frame.tmpAngle >= 0x18) {
            frame.tmpAngle = 0;
        }
        if (frame.tmpAngle == frame.sinOffset) {
            frame.ribbon->trail[frame.sinOffset].w = 0;
            frame.sinOffset++;
            if (frame.sinOffset >= 0x18) {
                frame.sinOffset = 0;
            }
        }
        frame.ribbon->trailHeadIndex = frame.tmpAngle;
        frame.ribbon->trailTailIndex = frame.sinOffset;
    }
    frame.headIndex = frame.ribbon->trailTailIndex;
    frame.ribbonBlock->end = frame.ribbon->trail[frame.headIndex];
    frame.ribbonBlock->start = frame.ribbonBlock->end;
    frame.ribbonBlock->start.x -= 100;
    frame.ribbonBlock->start.y -= 100;
    frame.ribbonBlock->start.z -= 100;
    frame.ribbonBlock->colorBottom = 0x10;
    frame.ribbonBlock->baseColor = 0x10;
    BattleFx_BlueLaserProjectRibbonSegmentQuad(frame.ribbonBlock, 12, 0);
    frame.tmpAngle = frame.headIndex;
    for (frame.segmentCount = 0;; frame.tmpAngle++, frame.segmentIndex++) {
        if (frame.tmpAngle >= 0x18) {
            frame.tmpAngle = 0;
        }
        frame.sinOffset = frame.tmpAngle + 1;
        if (frame.sinOffset >= 0x18) {
            frame.sinOffset = 0;
        }
        if (frame.ribbon->trail[frame.sinOffset].w == 0) {
            break;
        }
        frame.segmentCount++;
    }
    frame.tmpAngle = frame.headIndex;
    for (frame.segmentIndex = 1;; frame.tmpAngle++, frame.segmentIndex++) {
        if (frame.tmpAngle >= 0x18) {
            frame.tmpAngle = 0;
        }
        frame.sinOffset = frame.tmpAngle + 1;
        if (frame.sinOffset >= 0x18) {
            frame.sinOffset = 0;
        }
        if (frame.ribbon->trail[frame.sinOffset].w == 0) {
            break;
        }
        frame.ribbonBlock->start = frame.ribbon->trail[frame.tmpAngle];
        frame.ribbonBlock->end = frame.ribbon->trail[frame.sinOffset];
        for (frame.wordIndex = 0; frame.wordIndex < 4; frame.wordIndex++) {
            frame.ribbonBlock->prevProjected[frame.wordIndex] = frame.ribbonBlock->projected[frame.wordIndex];
            frame.ribbonBlock->prevScreenX[frame.wordIndex] = frame.ribbonBlock->screenX[frame.wordIndex];
            frame.ribbonBlock->prevScreenY[frame.wordIndex] = frame.ribbonBlock->screenY[frame.wordIndex];
        }
        frame.ribbonBlock->prevDepth = frame.ribbonBlock->depth;
        frame.ribbonBlock->colorStart = frame.ribbonBlock->colorBottom;
        BattleFx_BlueLaserProjectRibbonSegmentQuad(frame.ribbonBlock, 48, 0);
        BattleFx_BlueLaserDrawPsxGouraudQuads(frame.ribbonBlock, frame.segmentIndex, frame.segmentCount, 0);
        if (frame.segmentIndex == frame.segmentCount) {
            for (frame.wordIndex = 0; frame.wordIndex < 4; frame.wordIndex++) {
                frame.ribbonBlock->prevProjected[frame.wordIndex] = frame.ribbonBlock->projected[frame.wordIndex];
                frame.ribbonBlock->prevScreenX[frame.wordIndex] = frame.ribbonBlock->screenX[frame.wordIndex];
                frame.ribbonBlock->prevScreenY[frame.wordIndex] = frame.ribbonBlock->screenY[frame.wordIndex];
            }
            frame.ribbonBlock->prevDepth = frame.ribbonBlock->depth;
            BattleFx_BlueLaserProjectRibbonSegmentQuad(frame.ribbonBlock, 26, 1);
            BattleFx_BlueLaserDrawPsxGouraudQuads(frame.ribbonBlock, frame.segmentIndex, frame.segmentCount, 1);
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.slot->step)++;
}

/*
 * Task animating and rendering explosion sprites at laser impact points.
 */
// FUNCTION: FF7 0x0056d252
void __cdecl BattleFx_BlueLaserAnimatedSpriteTask(void)
{
    BattleFxMatraMagicAnimSprite *task;

    task = (BattleFxMatraMagicAnimSprite *)BATTLE_CURRENT_TASK_SLOT;
    (((BattleFxVector3 *)&g_BattleBlueLaserSpriteDrawReq)->frame) = task->frame;
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->scale, -(task->scale >> 3));
    (((BattleFxVector3 *)&g_BattleBlueLaserSpriteDrawReq)->limitSparkVectorAt16) = Gfx_ModelTableGetLoadedRecord(0, g_BattleBlueLaserLoadedRes);
    (((BattleFxVector3 *)&g_BattleBlueLaserSpriteDrawReq)->drawObj) = Gfx_ModelTableGetSpriteObject(0, g_BattleBlueLaserLoadedRes);
    g_BattleFxActiveDrawListPtr = (int)BattleFx_SubmitSpriteDraw((char *)&g_BattleBlueLaserSpriteDrawReq + 16, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->frame >= 17) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (task->frame)++;
}

/*
 * Renders Gouraud-shaded quad polygons for Reazer laser trails.
 */
// FUNCTION: FF7 0x0056d321
void __cdecl BattleFx_BlueLaserDrawPsxGouraudQuads(BattleFxBlueLaserSegmentWork *ribbon, int step, int count, int useCache)
{
    int colorStartVal;
    BattleFxLaserQuadPacket *pQp;
    int packetStorage[9];
    int next;
    int order;
    int shadeTail;
    int cornerIndex;

    if (useCache != 0) {
        colorStartVal = ribbon->colorStart;
        shadeTail = ribbon->colorBottom;
    } else {
        SetDrawColorRGB(0x68, 0x28, 0x28);
        cornerIndex = 4096 / count * step;
        Gfx_ShadePsxVertexColor((CVECTOR *)&ribbon->baseColor, cornerIndex, (CVECTOR *)&ribbon->colorBottom);
        colorStartVal = ribbon->colorBottom;
        shadeTail = ribbon->colorStart;
    }
    pQp = (BattleFxLaserQuadPacket *)packetStorage;
    order = ribbon->prevDepth + ribbon->depth >> 1 >> 2;
    for (cornerIndex = 0; cornerIndex < 4; cornerIndex++) {
        next = cornerIndex + 1 & 3;
        pQp->colorCorner = ribbon->projected[cornerIndex];
        pQp->colorNextCorner = ribbon->projected[next];
        pQp->shadeCorner = ribbon->prevProjected[cornerIndex];
        pQp->shadeNextCorner = ribbon->prevProjected[next];
        pQp->colorStart = colorStartVal;
        pQp->colorStartAlt = colorStartVal;
        pQp->shadeTail = shadeTail;
        pQp->shadeTailAlt = shadeTail;
        pQp->tag = pQp->tag & 0xFFFFFF | 0x8000000;
        *(char *)((char *)pQp + 7) = 0x3A;
        Gfx_SetBlendFactorQuad(*(float *)&ribbon->screenX[cornerIndex], *(float *)&ribbon->screenY[cornerIndex], *(float *)&ribbon->screenX[next], *(float *)&ribbon->screenY[next], *(float *)&ribbon->prevScreenX[cornerIndex], *(float *)&ribbon->prevScreenY[cornerIndex], *(float *)&ribbon->prevScreenX[next], *(float *)&ribbon->prevScreenY[next]);
        Gfx_DrawPsxPrimitive(pQp);
    }
    g_BattleFxActiveDrawListPtr = (int)pQp;
}

/*
 * Projects edge vertex coordinates for an individual Reazer ribbon quad segment.
 */
// FUNCTION: FF7 0x0056d4e4
void __cdecl BattleFx_BlueLaserProjectRibbonSegmentQuad(BattleFxBlueLaserSegmentWork *ribbon, int halfSize, int advanceFlag)
{
    int scale;
    BattleFxProjectedPoint *projectedPtr;
    int res, extent;

    if (advanceFlag != 0) {
        scale = SqrtToInt(ribbon->deltaLengthSq) >> 2;
        ribbon->rotation.t[0] += scale * ribbon->deltaX >> 12;
        ribbon->rotation.t[1] += scale * ribbon->deltaY >> 12;
        ribbon->rotation.t[2] += scale * ribbon->deltaZ >> 12;
    } else {
        ribbon->deltaX = ribbon->end.x - ribbon->start.x;
        ribbon->deltaY = ribbon->end.y - ribbon->start.y;
        ribbon->deltaZ = ribbon->end.z - ribbon->start.z;
        ribbon->deltaLengthSq = NormalizeVectorFixed(&ribbon->deltaX, &ribbon->deltaX);
        scale = ComputeAngleBetweenVectors((VECTOR *)&g_BattleBlueLaserSpriteDrawReq, (VECTOR *)&ribbon->deltaX, &ribbon->axis);
        Gte_BuildAxisAngleMatrix(scale, &ribbon->rotation, &ribbon->axis);
        ribbon->rotation.t[0] = ribbon->end.x;
        ribbon->rotation.t[1] = ribbon->end.y;
        ribbon->rotation.t[2] = ribbon->end.z;
    }
    ribbon->corner[0].vx = 0;
    ribbon->corner[0].vy = -halfSize;
    ribbon->corner[0].vz = 0;
    ribbon->corner[1].vx = halfSize;
    ribbon->corner[1].vy = 0;
    ribbon->corner[1].vz = 0;
    ribbon->corner[2].vx = 0;
    ribbon->corner[2].vy = halfSize;
    ribbon->corner[2].vz = 0;
    ribbon->corner[3].vx = -halfSize;
    ribbon->corner[3].vy = 0;
    ribbon->corner[3].vz = 0;
    MultiplyMatrix(&g_BattleCameraTransform, &ribbon->rotation, &ribbon->view);
    SetRotMatrix(&ribbon->view);
    SetTransMatrix(&ribbon->view);
    RefreshGlobalTransformState();
    ribbon->depth = Gfx_ProjectQuadToScreenAlt(&ribbon->corner[0], &ribbon->corner[1], &ribbon->corner[2], &ribbon->corner[3], &ribbon->projected[0], &ribbon->projected[1], &ribbon->projected[2], &ribbon->projected[3], &res, &extent);
    projectedPtr = (BattleFxProjectedPoint *)Gfx_GetProjectedVertexRecord(0);
    ribbon->screenX[0] = projectedPtr->screenX;
    ribbon->screenY[0] = projectedPtr->screenY;
    projectedPtr = (BattleFxProjectedPoint *)Gfx_GetProjectedVertexRecord(1);
    ribbon->screenX[1] = projectedPtr->screenX;
    ribbon->screenY[1] = projectedPtr->screenY;
    projectedPtr = (BattleFxProjectedPoint *)Gfx_GetProjectedVertexRecord(2);
    ribbon->screenX[2] = projectedPtr->screenX;
    ribbon->screenY[2] = projectedPtr->screenY;
    projectedPtr = (BattleFxProjectedPoint *)Gfx_GetProjectedVertexRecord(3);
    ribbon->screenX[3] = projectedPtr->screenX;
    ribbon->screenY[3] = projectedPtr->screenY;
}

/*
 * Selects the active draw list buffer slot for Reazer laser rendering.
 */
// FUNCTION: FF7 0x0056d817
void __cdecl BattleFx_LaserSelectDrawListBuffer(void)
{
    int delayIndex;
    unsigned char *buffer;

    if (g_BattleDrawFrameParity == 0) {
        buffer = g_BattleFxDrawListBuffer0;
    } else {
        buffer = g_BattleFxDrawListBuffer1;
    }
    g_BattleFxActiveDrawListPtr = (int)buffer;
    if (g_BattleSecondaryTaskActiveCount <= 1) {
        *(int *)g_BattleFxDrawListBufferTaskSlotPtr = 0xFFFFFFFF;
    }
    for (delayIndex = 0; delayIndex < 100; delayIndex++) {
    }
}
} /* extern "C" */
