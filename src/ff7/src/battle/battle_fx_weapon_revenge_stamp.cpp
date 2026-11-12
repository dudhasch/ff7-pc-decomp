// src/ff7/src/battle/battle_fx_weapon_revenge_stamp.cpp
/* Enemy Special Attack [154]: Emerald Weapon Revenge Stamp (orig. weapon/weapon4) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Attack effect for a weapon attack: loads resources, tumbles the targeted actors, and
//   renders rising rock, stretching column, gradient light strip and bone-emitted particle effects.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_fx.h"
#include "lib/types/graphics.h"
#include "ff7/camera_script.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/codegen.h"
#include <stdlib.h>
extern "C" {

struct BattleFxModelDraw;


typedef struct {
    short x;
    short y;
    short z;
    short pad;
} RevengeStampActorVec;

// clang-format off
// GLOBAL: FF7 0x007d8208
int g_WeaponFxRevengeStampResourceDefs = 0x00192A39;
int g_WeaponFxRevengeStampResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxWeaponRevengeStampAssetPrefix + 0x0)
};
int g_WeaponFxRevengeStampResourceDefsDataTail[44] = {
    (int)((char *)&g_BattleFxWeaponRevengeStampResourceNames + 0xc), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxWeaponRevengeStampResourceNames + 0x1c), (int)((char *)&g_BattleFxWeaponRevengeStampResourceNames + 0x28),
    0x0, 0x0, 0x3200b1, (int)((char *)&g_BattleFxWeaponRevengeStampResourceNames + 0x34), 0x0, 0x0,
    0x0, 0x3200b4, (int)((char *)&g_BattleFxWeaponRevengeStampResourceNames + 0x40), 0x0, 0x0, 0x0,
    0x3200b4, (int)((char *)&g_BattleFxWeaponRevengeStampResourceNames + 0x4c), 0x0, 0x0, 0x0, 0x3200b4,
    (int)((char *)&g_BattleFxWeaponRevengeStampResourceNames + 0x58), 0x0, 0x0, 0x0, 0x3200b1, (int)((char *)&g_BattleFxWeaponRevengeStampResourceNames + 0x64),
    0x0, 0x0, 0x0, 0x3200b1, (int)((char *)&g_BattleFxWeaponRevengeStampResourceNames + 0x70), 0x0,
    0x0, 0x0, 0x3200b1, (int)((char *)&g_BattleFxWeaponRevengeStampResourceNames + 0x7c), 0x0, 0x0,
    0x0, 0x0
};
// Model table indices of the rising rock models and of their fading variants, by modelIndex (the fourth
// entry is unused).
// GLOBAL: FF7 0x007d82c0
int g_WeaponFxRevengeStampModelResIndices1[4] = {3, 4, 5, 0};
// GLOBAL: FF7 0x007d82d0
int g_WeaponFxRevengeStampModelResIndices2[4] = {6, 7, 8, 0};
// GLOBAL: FF7 0x007d82e0
BattleFxSpriteRequest g_WeaponFxRevengeStampBoneParticleSpriteReq = {0, 0, -128, -128, -128, 0x2e, 0, 0};
// The sprite draw requests of the three particle tasks, each filled with its resource table entries before drawing.
// GLOBAL: FF7 0x007d82f0
BattleFxSpriteRequest g_WeaponFxRevengeStampSpriteReq1 = {0, 0, -128, -128, -128, 0x2e, 0, 0};
// GLOBAL: FF7 0x007d8300
BattleFxSpriteRequest g_WeaponFxRevengeStampSpriteReq2 = {0, 0, -128, -128, -128, 0x2e, 0, 0};
unsigned char g_rawData_007d8310[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x2e, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007d8320
int g_WeaponFxRevengeStampScaleDenominators[3] = {400, 850, 500};
// GLOBAL: FF7 0x007d8330
int g_WeaponFxRevengeStampScaleNumerators[3] = {300, 570, 240};
unsigned char g_BattleFxWeaponRevengeStampAssetPrefix[4] = {0x48, 0x4f, 0x55, 0x44};
unsigned char g_BattleFxWeaponRevengeStampResourceNames[136] = {
0x45, 0x4e, 0x5f, 0x31, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x00, 0x00, 0x48, 0x4f, 0x55, 0x44,
0x45, 0x4e, 0x5f, 0x31, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x00, 0x53, 0x4d, 0x4f, 0x4b,
0x45, 0x5f, 0x31, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x53, 0x4d, 0x4f, 0x4b, 0x45, 0x5f, 0x31, 0x2e,
0x53, 0x50, 0x54, 0x00, 0x62, 0x6f, 0x6d, 0x62, 0x5f, 0x31, 0x2e, 0x72, 0x73, 0x64, 0x00, 0x00,
0x52, 0x4f, 0x43, 0x4b, 0x5f, 0x31, 0x2e, 0x72, 0x73, 0x64, 0x00, 0x00, 0x52, 0x4f, 0x43, 0x4b,
0x5f, 0x32, 0x2e, 0x72, 0x73, 0x64, 0x00, 0x00, 0x52, 0x4f, 0x43, 0x4b, 0x5f, 0x33, 0x2e, 0x72,
0x73, 0x64, 0x00, 0x00, 0x52, 0x4f, 0x43, 0x4b, 0x5f, 0x31, 0x2e, 0x72, 0x73, 0x64, 0x00, 0x00,
0x52, 0x4f, 0x43, 0x4b, 0x5f, 0x32, 0x2e, 0x72, 0x73, 0x64, 0x00, 0x00, 0x52, 0x4f, 0x43, 0x4b,
0x5f, 0x33, 0x2e, 0x72, 0x73, 0x64, 0x00, 0x00};
// The four 100-particle buffers (0x960 bytes each) of BattleFxRevengeStampBufferParticle records, indexed by the emitter task's bufferIndex.
// GLOBAL: FF7 0x009f02a0
unsigned char g_WeaponFxRevengeStampParticleBuffers[0x2580] = {0};
// GLOBAL: FF7 0x009f2820
struct Int64Pair g_WeaponFxRevengeStampBonePos = {0};
// GLOBAL: FF7 0x009f2828
short g_WeaponFxRevengeStampCentroidPos[3] = {0};
// GLOBAL: FF7 0x009f2830
int g_WeaponFxRevengeStampTargetMask = 0;
// GLOBAL: FF7 0x009f2834
int g_WeaponFxRevengeStampSourceActor = 0;
// GLOBAL: FF7 0x009f2838
int g_WeaponFxRevengeStampTargetCount = 0;
unsigned char g_bssPad_009f283c[4] = {0};
// GLOBAL: FF7 0x009f2840
int g_WeaponFxRevengeStampTargetActorIndices[10] = {0};
// The position and rotation vectors the tumble task saves from each actor record, and the actor's draw flags, indexed by actor.
// GLOBAL: FF7 0x009f2868
RevengeStampActorVec g_WeaponFxRevengeStampSavedActorPos[10] = {0};
// GLOBAL: FF7 0x009f28b8
RevengeStampActorVec g_WeaponFxRevengeStampSavedActorRot[10] = {0};
// GLOBAL: FF7 0x009f2908
unsigned char g_WeaponFxRevengeStampSavedActorFlags[4] = {0};
unsigned char g_bssPad_009f290c[4] = {0};
unsigned char g_bssPad_009f2910[4] = {0};
// GLOBAL: FF7 0x009f2914
struct GfxTableCursor *g_WeaponFxRevengeStampResourceContext = 0;
// clang-format on

/*
 * Loads weapon4 resources into the battle scene context if not already loaded.
 */
// FUNCTION: FF7 0x00444730
void __cdecl BattleFx_WeaponLoadResRevengeStamp(void)
{
    GfxModelTableBank *sceneContext;
    GfxResourceRequest requestBuffer;
    int resultId;
    char pathSize[200];
    GfxResourceRequest *pReq;
    int defsPtr;

    if (g_pBattle3dScene != 0 && g_WeaponFxRevengeStampResourceContext == 0) {
        resultId = 9;
        defsPtr = (int)&g_WeaponFxRevengeStampResourceDefs;
        sceneContext = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestBuffer;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x007d83c8
        (&pReq->resourcePath)[0] = BattleFx_BuildResourcePath("weapon/weapon4", pathSize);
        Gfx_LoadModelTableIfAbsent(resultId, defsPtr, pReq, &g_WeaponFxRevengeStampResourceContext, sceneContext);
    }
}

/*
 * Entry point to initialize and begin Weapon 4 effect.
 */
// FUNCTION: FF7 0x004447c0
void __cdecl BattleFx_WeaponEnterRevengeStamp(int targetMask, int actorIdx)
{
    BattleFx_WeaponLoadResRevengeStamp();
    BattleFx_WeaponBeginRevengeStamp(targetMask, actorIdx);
}

/*
 * Initializes target bitmask, target indices, and starts Weapon 4 timeline task.
 */
// FUNCTION: FF7 0x004447da
void __cdecl BattleFx_WeaponBeginRevengeStamp(int targetMask, int actorIdx)
{
    int actorIndex;

    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    Battle_AllocateResourceSlot(BattleFx_RevengeStampSpawnTimeline);
    g_WeaponFxRevengeStampTargetMask = targetMask;
    actorIndex = 0;
    for (g_WeaponFxRevengeStampTargetCount = 0; actorIndex < 10; actorIndex++) {
        if ((targetMask & 1 << actorIndex) != 0) {
            g_WeaponFxRevengeStampTargetActorIndices[g_WeaponFxRevengeStampTargetCount] = actorIndex;
            g_WeaponFxRevengeStampTargetCount++;
        }
    }
    g_WeaponFxRevengeStampSourceActor = actorIdx;
    g_BattleScriptEffectFlag = 1;
}

/*
 * Calculates centroid of targets and initializes Weapon 4 main timeline task.
 */
// FUNCTION: FF7 0x0044487b
void __cdecl BattleFx_RevengeStampSpawnTimeline(void)
{
    BattleFxYuffieWeaponTimeline2 *task;

    task = (BattleFxYuffieWeaponTimeline2 *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    task = (BattleFxYuffieWeaponTimeline2 *)BATTLE_NEW_TASK_SLOT(BattleFx_RevengeStampTimelineTask);
    task->subCounter = 0;
    Battle_GetActorMaskCentroid(g_WeaponFxRevengeStampTargetMask, (short *)&g_WeaponFxRevengeStampCentroidPos);
}

/*
 * Coordinates sequence of animations, particle bursts, screen shakes, and sound effects for Weapon 4.
 */
// FUNCTION: FF7 0x004448d6
void __cdecl BattleFx_RevengeStampTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[44];
        BattleFxRevengeStampPieceRecord *pieceRecord;
        struct { unsigned char pad_to_4[0x4]; BattleFxRevengeStampTimeline *timeline; };
        struct { unsigned char pad_to_8[0x8]; BattleFxRevengeStampParticle *particle; };
        struct { unsigned char pad_to_C[0xC]; int randomSpeed; };
        struct { unsigned char pad_to_10[0x10]; int tmpRatio; };
        struct { unsigned char pad_to_14[0x14]; int tmpSpeed; };
        struct { unsigned char pad_to_18[0x18]; int stepLen; };
        struct { unsigned char pad_to_1C[0x1C]; int travelled; };
        struct { unsigned char pad_to_20[0x20]; int particleIndex; };
        struct { unsigned char pad_to_24[0x24]; short posX; };
        struct { unsigned char pad_to_26[0x26]; short posY; };
        struct { unsigned char pad_to_28[0x28]; short posZ; };
    } frame;
#pragma pack(pop)

    Battle_GetBoneWorldPos(g_WeaponFxRevengeStampSourceActor, g_BattleActorRecords[g_WeaponFxRevengeStampSourceActor].targetBone, ((short *)&g_WeaponFxRevengeStampBonePos));
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.timeline = (BattleFxRevengeStampTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.timeline->timer == 0) {
        frame.particle = (BattleFxRevengeStampParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_RevengeStampSideBurstParticleTask);
        frame.particle->bufferIndex = frame.timeline->counter;
        (frame.timeline->counter)++;
        frame.particle = (BattleFxRevengeStampParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_RevengeStampBone21And23ParticleTask);
        frame.particle->bufferIndex = frame.timeline->counter;
        (frame.timeline->counter)++;
        frame.particle = (BattleFxRevengeStampParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_RevengeStampBone8And10ParticleTask);
        frame.particle->bufferIndex = frame.timeline->counter;
        (frame.timeline->counter)++;
    }
    if (frame.timeline->timer == 92) {
        frame.posX = ((short *)&g_WeaponFxRevengeStampBonePos)[0] - 0xBB8;
        frame.posY = ((short *)&g_WeaponFxRevengeStampBonePos)[1] + 0xC8;
        frame.posZ = ((short *)&g_WeaponFxRevengeStampBonePos)[2] + 0xC8;
        frame.particleIndex = 0;
        for (frame.travelled = 0; frame.travelled < 6000; frame.particleIndex++) {
            frame.particle = (BattleFxRevengeStampParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_RevengeStampGrayGradientStripTask);
            frame.stepLen = rand() % 500 + 0x190 & 0xFFFFFFFE;
            if (frame.travelled + frame.stepLen > 6000) {
                frame.stepLen = 6000 - frame.travelled + 2;
            }
            frame.travelled += frame.stepLen;
            frame.particle->speed = *(short *)&frame.stepLen;
            frame.particle->posX = frame.posX + (frame.stepLen >> 1);
            frame.particle->posY = frame.posY;
            frame.particle->posZ = frame.posZ;
            frame.posX += *(short *)&frame.stepLen;
            frame.particle->lifetime = (rand() % 8) + 20;
            frame.particle->stepSize = frame.particle->speed / (30 - frame.particle->lifetime);
        }
        frame.posX = ((short *)&g_WeaponFxRevengeStampBonePos)[0] - 0xAF0;
        frame.posY = ((short *)&g_WeaponFxRevengeStampBonePos)[1] + 0x12C;
        frame.posZ = ((short *)&g_WeaponFxRevengeStampBonePos)[2] + 0x12C;
        frame.particleIndex = 0;
        for (frame.travelled = 0; frame.travelled < 5600; frame.particleIndex++) {
            frame.particle = (BattleFxRevengeStampParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_RevengeStampBlueGradientStripTask);
            frame.stepLen = rand() % 300 + 0xC8 & 0xFFFFFFFE;
            if (frame.travelled + frame.stepLen > 5600) {
                frame.stepLen = 5600 - frame.travelled + 2;
            }
            frame.travelled += frame.stepLen;
            frame.particle->speed = *(short *)&frame.stepLen;
            frame.particle->posX = frame.posX + (frame.stepLen >> 1);
            frame.particle->posY = frame.posY;
            frame.particle->posZ = frame.posZ;
            frame.posX += *(short *)&frame.stepLen;
            frame.particle->lifetime = rand() % 6 + 18;
            frame.particle->stepSize = frame.particle->speed / (26 - frame.particle->lifetime);
        }
        frame.posX = ((short *)&g_WeaponFxRevengeStampBonePos)[0] - 0xA28;
        frame.posY = ((short *)&g_WeaponFxRevengeStampBonePos)[1] + 0x12C;
        frame.posZ = ((short *)&g_WeaponFxRevengeStampBonePos)[2] + 0x64;
        frame.particleIndex = 0;
        for (frame.travelled = 0; frame.travelled < 5200; frame.particleIndex++) {
            frame.particle = (BattleFxRevengeStampParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_RevengeStampGreenGradientStripTask);
            frame.stepLen = rand() % 220 + 0xB4 & 0xFFFFFFFE;
            if (frame.travelled + frame.stepLen > 5200) {
                frame.stepLen = 5200 - frame.travelled + 2;
            }
            frame.travelled += frame.stepLen;
            frame.particle->speed = *(short *)&frame.stepLen;
            frame.particle->posX = frame.posX + (frame.stepLen >> 1);
            frame.particle->posY = frame.posY;
            frame.particle->posZ = frame.posZ;
            frame.posX += *(short *)&frame.stepLen;
            frame.particle->lifetime = rand() % 6 + 18;
            frame.particle->stepSize = frame.particle->speed / (26 - frame.particle->lifetime);
            frame.tmpSpeed = rand() % 160 + 60;
            if (((short *)&g_WeaponFxRevengeStampBonePos)[0] > frame.posX) {
                frame.particle->centerVelocity = *(short *)&frame.tmpSpeed;
            } else {
                frame.particle->centerVelocity = -frame.tmpSpeed;
            }
        }
    }
    if (frame.timeline->timer == 126) {
        frame.travelled = 0x2A1A;
        for (frame.particleIndex = 0; frame.particleIndex < g_WeaponFxRevengeStampTargetCount; frame.particleIndex++) {
            frame.particle = (BattleFxRevengeStampParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_RevengeStampStretchingModelTask);
            Battle_GetBoneWorldPos(frame.particleIndex, g_BattleActorRecords[frame.particleIndex].targetBone, &frame.particle->posX);
            frame.stepLen = g_BattleActorRecords[frame.particleIndex].radius * 3;
            frame.particle->posX += (short)(rand() % (frame.stepLen << 1) - frame.stepLen);
            frame.particle->posY = 0;
            frame.particle->posZ += (short)(rand() % (frame.stepLen << 1) - frame.stepLen);
            frame.particle->angleY = (rand() % 4096);
            frame.stepLen = rand() % 20 + 10;
            if ((frame.particle->angleY & 1) != 0) {
                frame.particle->angleYStep = *(short *)&frame.stepLen;
            } else {
                frame.particle->angleYStep = -frame.stepLen;
            }
            frame.stepLen = rand() % 1536 + 3072;
            frame.tmpSpeed = frame.travelled * frame.stepLen >> 12;
            frame.particle->speed = *(short *)&frame.tmpSpeed;
            frame.particle->lifetime = 0;
            frame.particle->stepSize = frame.tmpSpeed / 2;
            frame.particle->bufferIndex = rand() % 6;
        }
        frame.tmpRatio = 6 - frame.particleIndex;
        for (frame.particleIndex = 0; frame.particleIndex < frame.tmpRatio; frame.particleIndex++) {
            frame.particle = (BattleFxRevengeStampParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_RevengeStampStretchingModelTask);
            *(Int64Pair *)((char *)frame.particle + 4) = *(Int64Pair *)&g_WeaponFxRevengeStampBonePos;
            frame.particle->posX += (short)(rand() % 5500 - 2750);
            frame.particle->posY = 0;
            frame.particle->posZ += (short)(rand() % 3000 - 1500);
            frame.particle->angleY = (rand() % 4096);
            frame.stepLen = rand() % 20 + 10;
            if ((frame.particle->angleY & 1) != 0) {
                frame.particle->angleYStep = *(short *)&frame.stepLen;
            } else {
                frame.particle->angleYStep = -frame.stepLen;
            }
            frame.stepLen = rand() % 1536 + 3072;
            frame.tmpSpeed = frame.travelled * frame.stepLen >> 12;
            frame.particle->speed = *(short *)&frame.tmpSpeed;
            frame.particle->lifetime = 0;
            frame.particle->stepSize = frame.tmpSpeed / 2;
            frame.particle->bufferIndex = rand() % 6;
        }
    }
    if (frame.timeline->timer >= 128 && frame.timeline->timer <= 132) {
        frame.travelled = 0xBB8;
        frame.stepLen = frame.travelled << 1;
        for (frame.particleIndex = 0; frame.particleIndex < 10; frame.particleIndex++) {
            frame.tmpSpeed = rand() % 3;
            frame.pieceRecord = (BattleFxRevengeStampPieceRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_RevengeStampRisingSpinningModelTask);
            *(Int64Pair *)((char *)frame.pieceRecord + 4) = *(Int64Pair *)&g_WeaponFxRevengeStampBonePos;
            frame.pieceRecord->posX += (short)(rand() % frame.stepLen - frame.travelled);
            frame.pieceRecord->posY = -(rand() % (frame.travelled >> 2) + 450);
            frame.pieceRecord->posZ += (short)(rand() % frame.stepLen - frame.travelled);
            frame.pieceRecord->velocityY = -(rand() % 350 + 190);
            frame.pieceRecord->accelerationY = (rand() % 32) + 18;
            frame.pieceRecord->startAngle = (rand() % 4096);
            frame.pieceRecord->angle = frame.pieceRecord->startAngle;
            frame.pieceRecord->prevAngle = frame.pieceRecord->angle;
            frame.pieceRecord->angleStep = rand() % 0xB4 + 0x60;
            frame.pieceRecord->spinStep = rand() % 0xC8 + 0x4E;
            frame.tmpRatio = (g_WeaponFxRevengeStampScaleNumerators[frame.tmpSpeed] << 12) / g_WeaponFxRevengeStampScaleDenominators[frame.tmpSpeed];
            frame.randomSpeed = rand() % 3072 + 1280;
            frame.pieceRecord->scale = frame.tmpRatio * frame.randomSpeed >> 12;
            frame.pieceRecord->modelIndex = *(short *)&frame.tmpSpeed;
            frame.pieceRecord->lifetimeOrActor = rand() % 10 + 12;
            frame.pieceRecord->fadeStartFrame = frame.pieceRecord->lifetimeOrActor - 6;
        }
    }
    if (frame.timeline->timer == 128) {
        for (frame.particleIndex = 0; frame.particleIndex < g_WeaponFxRevengeStampTargetCount; frame.particleIndex++) {
            frame.travelled = g_WeaponFxRevengeStampTargetActorIndices[frame.particleIndex];
            frame.pieceRecord = (BattleFxRevengeStampPieceRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_RevengeStampActorTumbleTask);
            frame.pieceRecord->lifetimeOrActor = *(short *)&frame.travelled;
        }
    }
    if (frame.timeline->timer == 124) {
        BattleFx_StartScreenFlashRamp(1);
    }
    if (frame.timeline->timer == 125) {
        BattleFx_FadeOutScreenFlashRamp(4);
    }
    if (frame.timeline->timer == 1) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(((short *)&g_WeaponFxRevengeStampBonePos)), 0x18C);
    }
    if (frame.timeline->timer == 92) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(((short *)&g_WeaponFxRevengeStampBonePos)), 0x130);
    }
    if (frame.timeline->timer == 126) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_WeaponFxRevengeStampCentroidPos), 0x176);
    }
    if (frame.timeline->timer >= 160) {
        frame.timeline->ownerSlot = BATTLE_TASK_FREE;
        g_BattleScriptEffectFlag = 0;
    }
    (frame.timeline->timer)++;
}

/*
 * Animates actor knockback/tumbling physics and restores actor positions upon completion.
 */
// FUNCTION: FF7 0x004453dc
void __cdecl BattleFx_RevengeStampActorTumbleTask(void)
{
    RevengeStampActorVec rotated;
    RevengeStampActorVec newPos;
    int padWord[3]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    RevengeStampActorVec offset;
    int actorIndex;
    PsxMatrix matrix;
    BattleFxRevengeStampTumble *pTumble;

    if (g_BattleIsPaused != 0) {
        return;
    }
    pTumble = (BattleFxRevengeStampTumble *)BATTLE_CURRENT_TASK_SLOT;
    actorIndex = pTumble->actorIdx;
    if (pTumble->frame == 0) {
        g_WeaponFxRevengeStampSavedActorPos[actorIndex] = *(RevengeStampActorVec *)&g_BattleActorRecords[actorIndex].posX;
        g_WeaponFxRevengeStampSavedActorRot[actorIndex] = *(RevengeStampActorVec *)&g_BattleActorRecords[actorIndex].rotX;
        g_WeaponFxRevengeStampSavedActorFlags[actorIndex] = g_BattleActorRecords[actorIndex].drawFlags;
        g_BattleActorRecords[actorIndex].drawFlags |= 0x11;
        Battle_GetActorBoneYRange((short)actorIndex, &pTumble->boneMinY, &offset.x);
        Battle_GetBoneWorldPos(actorIndex, g_BattleActorRecords[actorIndex].targetBone, &pTumble->posX);
        pTumble->rotVelX = (short)(rand() % 180 + 168);
        pTumble->rotVelZ = (short)(rand() % 208 + 150);
        pTumble->velocityY = (short)-(rand() % 200 + 550);
        pTumble->accelY = (short)(rand() % 14 + 34);
        pTumble->currentY = g_WeaponFxRevengeStampSavedActorPos[actorIndex].y;
        (pTumble->frame)++;
    } else {
        pTumble->currentY += pTumble->velocityY;
        pTumble->velocityY += pTumble->accelY;
        pTumble->accelY += (short)(pTumble->accelY >> 4);
        g_BattleActorRecords[actorIndex].rotX += pTumble->rotVelX;
        g_BattleActorRecords[actorIndex].rotZ += pTumble->rotVelZ;
        RotMatrix((SVECTOR *)(((char *)&g_BattleActorRecords[actorIndex].rotX)), &matrix);
        offset.x = (short)(pTumble->posX - g_WeaponFxRevengeStampSavedActorPos[actorIndex].x);
        offset.y = (short)(pTumble->posY - g_WeaponFxRevengeStampSavedActorPos[actorIndex].y);
        offset.z = (short)(pTumble->posZ - g_WeaponFxRevengeStampSavedActorPos[actorIndex].z);
        TransformVectorToInt16(&matrix, &offset, &rotated);
        newPos.x = (short)(g_WeaponFxRevengeStampSavedActorPos[actorIndex].x + offset.x - rotated.x);
        newPos.y = (short)(g_WeaponFxRevengeStampSavedActorPos[actorIndex].y + offset.y - rotated.y + pTumble->currentY - pTumble->posY);
        newPos.z = (short)(g_WeaponFxRevengeStampSavedActorPos[actorIndex].z + offset.z - rotated.z);
        if (pTumble->currentY < g_WeaponFxRevengeStampSavedActorPos[actorIndex].y && pTumble->rotVelX != 0) {
            *(RevengeStampActorVec *)&g_BattleActorRecords[actorIndex].posX = newPos;
        } else if (pTumble->rotVelX != 0 && pTumble->rotVelZ != 0) {
            *(RevengeStampActorVec *)&g_BattleActorRecords[actorIndex].posX = g_WeaponFxRevengeStampSavedActorPos[actorIndex];
            g_BattleActorRecords[actorIndex].posY = pTumble->posY;
            g_BattleActorRecords[actorIndex].rotX = 0x800 /* 180 deg */;
            g_BattleActorRecords[actorIndex].yaw = (short)((pTumble->rotVelX + pTumble->rotVelZ) << 2);
            g_BattleActorRecords[actorIndex].rotZ = 0;
            pTumble->rotVelZ = 0;
            pTumble->rotVelX = 0;
            Battle_PostActorHitOrHurtMessage(actorIndex);
        }
        if (pTumble->frame >= 30) {
            *(RevengeStampActorVec *)&g_BattleActorRecords[actorIndex].posX = g_WeaponFxRevengeStampSavedActorPos[actorIndex];
            *(RevengeStampActorVec *)&g_BattleActorRecords[actorIndex].rotX = g_WeaponFxRevengeStampSavedActorRot[actorIndex];
            if ((g_WeaponFxRevengeStampSavedActorFlags[actorIndex] & 0x10) == 0) {
                g_BattleActorRecords[actorIndex].drawFlags &= 0xEF;
            }
            if ((g_WeaponFxRevengeStampSavedActorFlags[actorIndex] & 1) == 0) {
                g_BattleActorRecords[actorIndex].drawFlags &= 0xFE;
            }
            pTumble->ownerSlot = BATTLE_TASK_FREE;
        }
        (pTumble->frame)++;
    }
}

/*
 * Calculates minimum and maximum Y bounds across all bones of the specified actor.
 */
// FUNCTION: FF7 0x004458b4
void __cdecl Battle_GetActorBoneYRange(short actorIndex, short *minY, short *maxY)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[20];
        int boneMaxY;
        struct { unsigned char pad_to_4[0x4]; int boneIndex; };
        struct { unsigned char pad_to_8[0x8]; int boneMinY; };
        struct { unsigned char pad_to_C[0xC]; int boneWorldPos; };
        struct { unsigned char pad_to_E[0xE]; short boneWorldY; };
    } frame;
#pragma pack(pop)

    frame.boneMinY = 0x7FFF;
    frame.boneMaxY = 0xFFFF8000;
    for (frame.boneIndex = 0; frame.boneIndex < g_BattleActorRecords[actorIndex].boneCount; frame.boneIndex++) {
        Battle_GetBoneWorldPos(actorIndex, frame.boneIndex, (short *)&frame.boneWorldPos);
        if (frame.boneWorldY < frame.boneMinY) {
            frame.boneMinY = frame.boneWorldY;
        }
        if (frame.boneWorldY > frame.boneMaxY) {
            frame.boneMaxY = frame.boneWorldY;
        }
    }
    *minY = *(short *)&frame.boneMinY;
    *maxY = *(short *)&frame.boneMaxY;
}

/*
 * Renders rising spinning debris/rock models with fading over time.
 */
// FUNCTION: FF7 0x0044593f
void __cdecl BattleFx_RevengeStampRisingSpinningModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[108];
        BattleFxRevengeStampRisingModel *risingModel;
        struct { unsigned char pad_to_4[0x4]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; SVECTOR rotX; };
        struct { unsigned char pad_to_58[0x58]; BattleFxModelDraw *drawModel; };
        struct { unsigned char pad_to_5C[0x5C]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.risingModel = (BattleFxRevengeStampRisingModel *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = frame.risingModel->angle;
    frame.rotX.vy = 0;
    frame.rotX.vz = frame.risingModel->spinAngle;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.risingModel->posX;
    frame.rotMatrix.t[1] = frame.risingModel->posY;
    frame.rotMatrix.t[2] = frame.risingModel->posZ;
    frame.scaleVec.vz = frame.risingModel->scale;
    frame.scaleVec.vy = frame.scaleVec.vz;
    frame.scaleVec.vx = frame.scaleVec.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleVec);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    frame.drawModel = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.drawModel->modelGroup = Gfx_ModelTableGetRenderObject(g_WeaponFxRevengeStampModelResIndices1[frame.risingModel->modelIndex], (GfxTableCursor *)(g_WeaponFxRevengeStampResourceContext));
    frame.drawModel->flags = 0;
    frame.drawModel->uvScroll = 0;
    frame.drawModel->fadeLevel = 128;
    frame.drawModel->stateFlags = 0x20;
    frame.drawModel->texturePageBits = 0;
    if (frame.risingModel->frame >= frame.risingModel->fadeStartFrame) {
        frame.drawModel->flags |= 8;
        frame.drawModel->modelGroup = Gfx_ModelTableGetRenderObject(g_WeaponFxRevengeStampModelResIndices2[frame.risingModel->modelIndex], (GfxTableCursor *)(g_WeaponFxRevengeStampResourceContext));
        frame.drawModel->fadeLevel = 128 - (frame.risingModel->frame - frame.risingModel->fadeStartFrame) * 21;
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.drawModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.risingModel->frame >= frame.risingModel->lifetimeFrames) {
        frame.risingModel->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.risingModel->posY += frame.risingModel->velocityY;
    frame.risingModel->velocityY += frame.risingModel->accelerationY;
    if (frame.risingModel->velocityY > -50) {
        frame.risingModel->velocityY = 0xFFCE;
    } else {
        frame.risingModel->accelerationY += (short)(frame.risingModel->accelerationY >> 4);
    }
    frame.risingModel->angle += frame.risingModel->angleStep;
    frame.risingModel->spinAngle += frame.risingModel->spinStep;
    (frame.risingModel->frame)++;
}

/*
 * Renders stretching vertical column/shockwave model.
 */
// FUNCTION: FF7 0x00445ba0
void __cdecl BattleFx_RevengeStampStretchingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[68];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxRevengeStampStretchSlot *stretchSlot; };
        struct { unsigned char pad_to_28[0x28]; SVECTOR rotX; };
        struct { unsigned char pad_to_30[0x30]; BattleFxModelDraw *drawModel; };
        struct { unsigned char pad_to_34[0x34]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.stretchSlot = (BattleFxRevengeStampStretchSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.stretchSlot->countdown > 0) {
        if (g_BattleIsPaused == 0) {
            (frame.stretchSlot->countdown)--;
        }
    } else {
        frame.rotX.vx = 0;
        frame.rotX.vy = frame.stretchSlot->rotY;
        frame.rotX.vz = 0;
        RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
        frame.rotMatrix.t[0] = frame.stretchSlot->posX;
        frame.rotMatrix.t[1] = frame.stretchSlot->posY;
        frame.rotMatrix.t[2] = frame.stretchSlot->posZ;
        frame.scaleVec.vz = frame.stretchSlot->scaleXZ;
        frame.scaleVec.vx = frame.scaleVec.vz;
        frame.scaleVec.vy = frame.stretchSlot->height;
        ScaleMatrix(&frame.rotMatrix, &frame.scaleVec);
        MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
        SetRotMatrix(&frame.rotMatrix);
        SetTransMatrix(&frame.rotMatrix);
        frame.drawModel = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
        frame.drawModel->modelGroup = Gfx_ModelTableGetRenderObject(2, (GfxTableCursor *)(g_WeaponFxRevengeStampResourceContext));
        frame.drawModel->flags = 8;
        frame.drawModel->uvScroll = 0;
        frame.drawModel->fadeLevel = 128;
        frame.drawModel->stateFlags = 0x20;
        frame.drawModel->texturePageBits = 0;
        if (frame.stretchSlot->elapsed >= 12) {
            frame.drawModel->fadeLevel = 128 - (frame.stretchSlot->elapsed - 12 << 4);
        }
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.drawModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
        if (frame.stretchSlot->elapsed >= 20) {
            frame.stretchSlot->ownerSlot = BATTLE_TASK_FREE;
        }
        if (g_BattleIsPaused != 0) {
            return;
        }
        frame.stretchSlot->rotY += frame.stretchSlot->rotYStep;
        frame.stretchSlot->height += frame.stretchSlot->heightStep;
        frame.stretchSlot->heightStep -= (short)(frame.stretchSlot->heightStep) / 2;
        if (frame.stretchSlot->height < 0) {
            frame.stretchSlot->height = 0;
        }
        (frame.stretchSlot->elapsed)++;
    }
}

/*
 * Renders expanding green gradient light strip with bilinear quad projection.
 */
// FUNCTION: FF7 0x00445daf
void __cdecl BattleFx_RevengeStampGreenGradientStripTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[180];
        struct { unsigned char pad_to_4[0x4]; GfxClipVec4 vtxSlot2; };
        struct { unsigned char pad_to_14[0x14]; BattleFxGreenGradientStrip *gradStrip; };
        struct { unsigned char pad_to_18[0x18]; GfxClipVec4 vtxSlotB0; };
        struct { unsigned char pad_to_28[0x28]; SVECTOR v5x; };
        struct { unsigned char pad_to_30[0x30]; GfxClipVec4 vtxSlotB1; };
        struct { unsigned char pad_to_40[0x40]; SVECTOR v4x; };
        struct { unsigned char pad_to_48[0x48]; GfxClipVec4 vtxSlotB2; };
        struct { unsigned char pad_to_58[0x58]; int orderingCount; };
        struct { unsigned char pad_to_5C[0x5C]; SVECTOR v3x; };
        struct { unsigned char pad_to_68[0x68]; SVECTOR v2x; };
        struct { unsigned char pad_to_74[0x74]; SVECTOR v1x; };
        struct { unsigned char pad_to_7C[0x7C]; GfxClipVec4 vtxSlot0; };
        struct { unsigned char pad_to_8C[0x8C]; SVECTOR v0x; };
        struct { unsigned char pad_to_94[0x94]; GfxClipVec4 vtxSlot1; };
        struct { unsigned char pad_to_A4[0xA4]; BattleFxPsxPolygonPrim *stripPrimA; };
        struct { unsigned char pad_to_A8[0xA8]; int orderingDepth; };
        struct { unsigned char pad_to_AC[0xAC]; BattleFxPsxPolygonPrim *stripPrimB; };
    } frame;
#pragma pack(pop)

    frame.gradStrip = (BattleFxGreenGradientStrip *)BATTLE_CURRENT_TASK_SLOT;
    frame.v4x.vx = frame.gradStrip->center - (frame.gradStrip->width >> 1);
    frame.v2x.vx = frame.v4x.vx;
    frame.v0x.vx = frame.v2x.vx;
    frame.v5x.vx = frame.gradStrip->center + (frame.gradStrip->width >> 1);
    frame.v3x.vx = frame.v5x.vx;
    frame.v1x.vx = frame.v3x.vx;
    frame.v1x.vy = frame.gradStrip->posY;
    frame.v0x.vy = frame.v1x.vy;
    frame.v3x.vy = 0;
    frame.v2x.vy = frame.v3x.vy;
    frame.v5x.vy = frame.gradStrip->posY - 500;
    frame.v4x.vy = frame.v5x.vy;
    frame.v5x.vz = frame.gradStrip->posZ;
    frame.v4x.vz = frame.v5x.vz;
    frame.v3x.vz = frame.v4x.vz;
    frame.v2x.vz = frame.v3x.vz;
    frame.v1x.vz = frame.v2x.vz;
    frame.v0x.vz = frame.v1x.vz;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    frame.stripPrimA = (BattleFxPsxPolygonPrim *)Gfx_GetPsxPacketScratchPtr();
    frame.stripPrimB = (BattleFxPsxPolygonPrim *)((int)frame.stripPrimA + 0x24);
    StoreDeferredDrawArgs(&frame.v0x, &frame.v1x, &frame.v2x);
    ApplyDeferredDrawArgs();
    Gfx_ReadTransformOrderingDepth(&frame.orderingDepth);
    Gfx_ReadVertexSlotVector(0, (GfxHomogeneousVec4 *)&frame.vtxSlot0);
    Gfx_ReadVertexSlotVector(1, (GfxHomogeneousVec4 *)&frame.vtxSlot1);
    Gfx_ReadVertexSlotVector(2, (GfxHomogeneousVec4 *)&frame.vtxSlot2);
    frame.orderingCount = frame.orderingDepth >> 2;
    if (frame.orderingCount > 0) {
        StoreDeferredDrawArgs(&frame.v3x, &frame.v4x, &frame.v5x);
        ApplyDeferredDrawArgs();
        Gfx_ReadVertexSlotVector(0, (GfxHomogeneousVec4 *)&frame.vtxSlotB0);
        Gfx_ReadVertexSlotVector(1, (GfxHomogeneousVec4 *)&frame.vtxSlotB1);
        Gfx_ReadVertexSlotVector(2, (GfxHomogeneousVec4 *)&frame.vtxSlotB2);
        frame.stripPrimA->colorCmd = 0x70A060;
        frame.stripPrimA->color1 = 0x70A060;
        frame.stripPrimA->color2 = 0x305020;
        frame.stripPrimA->color3 = 0x305020;
        frame.stripPrimB->colorCmd = 0;
        frame.stripPrimB->color1 = 0;
        frame.stripPrimB->color2 = 0x70A060;
        frame.stripPrimB->color3 = 0x70A060;
        frame.stripPrimA->tag = frame.stripPrimA->tag & 0xFFFFFF | 0x8000000;
        *(char *)((char *)frame.stripPrimA + 7) = 0x3A;
        Gfx_SetVertexSlotClipRect(0, (GfxHomogeneousVec4 *)&frame.vtxSlot0);
        Gfx_SetVertexSlotClipRect(1, (GfxHomogeneousVec4 *)&frame.vtxSlot1);
        Gfx_SetVertexSlotClipRect(2, (GfxHomogeneousVec4 *)&frame.vtxSlot2);
        Gfx_SetVertexSlotClipRect(3, (GfxHomogeneousVec4 *)&frame.vtxSlotB0);
        Gfx_DrawPsxPrimitive(frame.stripPrimA);
        Gfx_SetVertexSlotClipRect(0, (GfxHomogeneousVec4 *)&frame.vtxSlotB1);
        Gfx_SetVertexSlotClipRect(1, (GfxHomogeneousVec4 *)&frame.vtxSlotB2);
        Gfx_SetVertexSlotClipRect(2, (GfxHomogeneousVec4 *)&frame.vtxSlot0);
        Gfx_SetVertexSlotClipRect(3, (GfxHomogeneousVec4 *)&frame.vtxSlot1);
        frame.stripPrimB->tag = frame.stripPrimB->tag & 0xFFFFFF | 0x8000000;
        *(char *)((char *)frame.stripPrimB + 7) = 0x3A;
        Gfx_DrawPsxPrimitive(frame.stripPrimB);
    }
    g_BattleFxActiveDrawListPtr = (int)frame.stripPrimB;
    if (frame.gradStrip->frameCount >= 26) {
        frame.gradStrip->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.gradStrip->frameCount >= frame.gradStrip->duration) {
        frame.gradStrip->width -= frame.gradStrip->widthRate;
        frame.gradStrip->widthRate += (short)(frame.gradStrip->widthRate >> 5);
        if (frame.gradStrip->width < 2) {
            frame.gradStrip->width = 2;
        }
        frame.gradStrip->center += frame.gradStrip->centerVelocity;
        frame.gradStrip->centerVelocity += (short)(frame.gradStrip->centerVelocity >> 5);
    }
    (frame.gradStrip->frameCount)++;
}

/*
 * Renders expanding blue gradient light strip with bilinear quad projection.
 */
// FUNCTION: FF7 0x004461b0
void __cdecl BattleFx_RevengeStampBlueGradientStripTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[180];
        struct { unsigned char pad_to_4[0x4]; GfxClipVec4 vtxSlot2; };
        struct { unsigned char pad_to_14[0x14]; BattleFxBlueGradientStrip *gradStrip; };
        struct { unsigned char pad_to_18[0x18]; GfxClipVec4 vtxSlotB0; };
        struct { unsigned char pad_to_28[0x28]; SVECTOR v5x; };
        struct { unsigned char pad_to_30[0x30]; GfxClipVec4 vtxSlotB1; };
        struct { unsigned char pad_to_40[0x40]; SVECTOR v4x; };
        struct { unsigned char pad_to_48[0x48]; GfxClipVec4 vtxSlotB2; };
        struct { unsigned char pad_to_58[0x58]; int orderingCount; };
        struct { unsigned char pad_to_5C[0x5C]; SVECTOR v3x; };
        struct { unsigned char pad_to_68[0x68]; SVECTOR v2x; };
        struct { unsigned char pad_to_74[0x74]; SVECTOR v1x; };
        struct { unsigned char pad_to_7C[0x7C]; GfxClipVec4 vtxSlot0; };
        struct { unsigned char pad_to_8C[0x8C]; SVECTOR v0x; };
        struct { unsigned char pad_to_94[0x94]; GfxClipVec4 vtxSlot1; };
        struct { unsigned char pad_to_A4[0xA4]; BattleFxPsxPolygonPrim *stripPrimA; };
        struct { unsigned char pad_to_A8[0xA8]; int orderingDepth; };
        struct { unsigned char pad_to_AC[0xAC]; BattleFxPsxPolygonPrim *stripPrimB; };
    } frame;
#pragma pack(pop)

    frame.gradStrip = (BattleFxBlueGradientStrip *)BATTLE_CURRENT_TASK_SLOT;
    frame.v4x.vx = frame.gradStrip->centerA - (frame.gradStrip->width >> 1);
    frame.v2x.vx = frame.v4x.vx;
    frame.v0x.vx = frame.v2x.vx;
    frame.v5x.vx = frame.gradStrip->centerA + (frame.gradStrip->width >> 1);
    frame.v3x.vx = frame.v5x.vx;
    frame.v1x.vx = frame.v3x.vx;
    frame.v1x.vy = frame.gradStrip->centerB;
    frame.v0x.vy = frame.v1x.vy;
    frame.v3x.vy = 0;
    frame.v2x.vy = frame.v3x.vy;
    frame.v5x.vy = frame.gradStrip->centerB - 500;
    frame.v4x.vy = frame.v5x.vy;
    frame.v5x.vz = frame.gradStrip->posZ;
    frame.v4x.vz = frame.v5x.vz;
    frame.v3x.vz = frame.v4x.vz;
    frame.v2x.vz = frame.v3x.vz;
    frame.v1x.vz = frame.v2x.vz;
    frame.v0x.vz = frame.v1x.vz;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    frame.stripPrimA = (BattleFxPsxPolygonPrim *)Gfx_GetPsxPacketScratchPtr();
    frame.stripPrimB = (BattleFxPsxPolygonPrim *)((int)frame.stripPrimA + 0x24);
    StoreDeferredDrawArgs(&frame.v0x, &frame.v1x, &frame.v2x);
    ApplyDeferredDrawArgs();
    Gfx_ReadTransformOrderingDepth(&frame.orderingDepth);
    Gfx_ReadVertexSlotVector(0, (GfxHomogeneousVec4 *)&frame.vtxSlot0);
    Gfx_ReadVertexSlotVector(1, (GfxHomogeneousVec4 *)&frame.vtxSlot1);
    Gfx_ReadVertexSlotVector(2, (GfxHomogeneousVec4 *)&frame.vtxSlot2);
    frame.orderingCount = frame.orderingDepth >> 2;
    if (frame.orderingCount > 0) {
        StoreDeferredDrawArgs(&frame.v3x, &frame.v4x, &frame.v5x);
        ApplyDeferredDrawArgs();
        Gfx_ReadVertexSlotVector(0, (GfxHomogeneousVec4 *)&frame.vtxSlotB0);
        Gfx_ReadVertexSlotVector(1, (GfxHomogeneousVec4 *)&frame.vtxSlotB1);
        Gfx_ReadVertexSlotVector(2, (GfxHomogeneousVec4 *)&frame.vtxSlotB2);
        frame.stripPrimA->colorCmd = 0xB07070;
        frame.stripPrimA->color1 = 0xB07070;
        frame.stripPrimA->color2 = 0x603030;
        frame.stripPrimA->color3 = 0x603030;
        frame.stripPrimB->colorCmd = 0;
        frame.stripPrimB->color1 = 0;
        frame.stripPrimB->color2 = 0xB07070;
        frame.stripPrimB->color3 = 0xB07070;
        frame.stripPrimA->tag = frame.stripPrimA->tag & 0xFFFFFF | 0x8000000;
        *(char *)((char *)frame.stripPrimA + 7) = 0x3A;
        Gfx_SetVertexSlotClipRect(0, (GfxHomogeneousVec4 *)&frame.vtxSlot0);
        Gfx_SetVertexSlotClipRect(1, (GfxHomogeneousVec4 *)&frame.vtxSlot1);
        Gfx_SetVertexSlotClipRect(2, (GfxHomogeneousVec4 *)&frame.vtxSlot2);
        Gfx_SetVertexSlotClipRect(3, (GfxHomogeneousVec4 *)&frame.vtxSlotB0);
        Gfx_DrawPsxPrimitive(frame.stripPrimA);
        Gfx_SetVertexSlotClipRect(0, (GfxHomogeneousVec4 *)&frame.vtxSlotB1);
        Gfx_SetVertexSlotClipRect(1, (GfxHomogeneousVec4 *)&frame.vtxSlotB2);
        Gfx_SetVertexSlotClipRect(2, (GfxHomogeneousVec4 *)&frame.vtxSlot0);
        Gfx_SetVertexSlotClipRect(3, (GfxHomogeneousVec4 *)&frame.vtxSlot1);
        frame.stripPrimB->tag = frame.stripPrimB->tag & 0xFFFFFF | 0x8000000;
        *(char *)((char *)frame.stripPrimB + 7) = 0x3A;
        Gfx_DrawPsxPrimitive(frame.stripPrimB);
    }
    g_BattleFxActiveDrawListPtr = (int)frame.stripPrimB;
    if (frame.gradStrip->step >= 26) {
        frame.gradStrip->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.gradStrip->step >= frame.gradStrip->endStep) {
        frame.gradStrip->width -= frame.gradStrip->widthRate;
        frame.gradStrip->widthRate += (short)(frame.gradStrip->widthRate >> 5);
        if (frame.gradStrip->width < 2) {
            frame.gradStrip->width = 2;
        }
    }
    (frame.gradStrip->step)++;
}

/*
 * Renders expanding gray gradient light strip with bilinear quad projection.
 */
// FUNCTION: FF7 0x0044656b
void __cdecl BattleFx_RevengeStampGrayGradientStripTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[180];
        struct { unsigned char pad_to_4[0x4]; GfxClipVec4 vtxSlot2; };
        struct { unsigned char pad_to_14[0x14]; BattleFxGrayGradientStrip *gradStrip; };
        struct { unsigned char pad_to_18[0x18]; GfxClipVec4 vtxSlotB0; };
        struct { unsigned char pad_to_28[0x28]; SVECTOR v5x; };
        struct { unsigned char pad_to_30[0x30]; GfxClipVec4 vtxSlotB1; };
        struct { unsigned char pad_to_40[0x40]; SVECTOR v4x; };
        struct { unsigned char pad_to_48[0x48]; GfxClipVec4 vtxSlotB2; };
        struct { unsigned char pad_to_58[0x58]; int orderingCount; };
        struct { unsigned char pad_to_5C[0x5C]; SVECTOR v3x; };
        struct { unsigned char pad_to_68[0x68]; SVECTOR v2x; };
        struct { unsigned char pad_to_74[0x74]; SVECTOR v1x; };
        struct { unsigned char pad_to_7C[0x7C]; GfxClipVec4 vtxSlot0; };
        struct { unsigned char pad_to_8C[0x8C]; SVECTOR v0x; };
        struct { unsigned char pad_to_94[0x94]; GfxClipVec4 vtxSlot1; };
        struct { unsigned char pad_to_A4[0xA4]; BattleFxPsxPolygonPrim *stripPrimA; };
        struct { unsigned char pad_to_A8[0xA8]; int orderingDepth; };
        struct { unsigned char pad_to_AC[0xAC]; BattleFxPsxPolygonPrim *stripPrimB; };
    } frame;
#pragma pack(pop)

    frame.gradStrip = (BattleFxGrayGradientStrip *)BATTLE_CURRENT_TASK_SLOT;
    frame.v4x.vx = frame.gradStrip->centerX - (frame.gradStrip->width >> 1);
    frame.v2x.vx = frame.v4x.vx;
    frame.v0x.vx = frame.v2x.vx;
    frame.v5x.vx = frame.gradStrip->centerX + (frame.gradStrip->width >> 1);
    frame.v3x.vx = frame.v5x.vx;
    frame.v1x.vx = frame.v3x.vx;
    frame.v1x.vy = frame.gradStrip->posY;
    frame.v0x.vy = frame.v1x.vy;
    frame.v3x.vy = 0;
    frame.v2x.vy = frame.v3x.vy;
    frame.v5x.vy = frame.gradStrip->posY - 400;
    frame.v4x.vy = frame.v5x.vy;
    frame.v5x.vz = frame.gradStrip->posZ;
    frame.v4x.vz = frame.v5x.vz;
    frame.v3x.vz = frame.v4x.vz;
    frame.v2x.vz = frame.v3x.vz;
    frame.v1x.vz = frame.v2x.vz;
    frame.v0x.vz = frame.v1x.vz;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    frame.stripPrimA = (BattleFxPsxPolygonPrim *)Gfx_GetPsxPacketScratchPtr();
    frame.stripPrimB = (BattleFxPsxPolygonPrim *)((int)frame.stripPrimA + 0x24);
    StoreDeferredDrawArgs(&frame.v0x, &frame.v1x, &frame.v2x);
    ApplyDeferredDrawArgs();
    Gfx_ReadTransformOrderingDepth(&frame.orderingDepth);
    Gfx_ReadVertexSlotVector(0, (GfxHomogeneousVec4 *)&frame.vtxSlot0);
    Gfx_ReadVertexSlotVector(1, (GfxHomogeneousVec4 *)&frame.vtxSlot1);
    Gfx_ReadVertexSlotVector(2, (GfxHomogeneousVec4 *)&frame.vtxSlot2);
    frame.orderingCount = frame.orderingDepth >> 2;
    if (frame.orderingCount > 0) {
        StoreDeferredDrawArgs(&frame.v3x, &frame.v4x, &frame.v5x);
        ApplyDeferredDrawArgs();
        Gfx_ReadVertexSlotVector(0, (GfxHomogeneousVec4 *)&frame.vtxSlotB0);
        Gfx_ReadVertexSlotVector(1, (GfxHomogeneousVec4 *)&frame.vtxSlotB1);
        Gfx_ReadVertexSlotVector(2, (GfxHomogeneousVec4 *)&frame.vtxSlotB2);
        frame.stripPrimA->colorCmd = 0xB0B0B0;
        frame.stripPrimA->color1 = 0xB0B0B0;
        frame.stripPrimA->color2 = 0x707070;
        frame.stripPrimA->color3 = 0x707070;
        frame.stripPrimB->colorCmd = 0;
        frame.stripPrimB->color1 = 0;
        frame.stripPrimB->color2 = 0xB0B0B0;
        frame.stripPrimB->color3 = 0xB0B0B0;
        frame.stripPrimA->tag = frame.stripPrimA->tag & 0xFFFFFF | 0x8000000;
        *(char *)((char *)frame.stripPrimA + 7) = 0x3A;
        Gfx_SetVertexSlotClipRect(0, (GfxHomogeneousVec4 *)&frame.vtxSlot0);
        Gfx_SetVertexSlotClipRect(1, (GfxHomogeneousVec4 *)&frame.vtxSlot1);
        Gfx_SetVertexSlotClipRect(2, (GfxHomogeneousVec4 *)&frame.vtxSlot2);
        Gfx_SetVertexSlotClipRect(3, (GfxHomogeneousVec4 *)&frame.vtxSlotB0);
        Gfx_DrawPsxPrimitive(frame.stripPrimA);
        Gfx_SetVertexSlotClipRect(0, (GfxHomogeneousVec4 *)&frame.vtxSlotB1);
        Gfx_SetVertexSlotClipRect(1, (GfxHomogeneousVec4 *)&frame.vtxSlotB2);
        Gfx_SetVertexSlotClipRect(2, (GfxHomogeneousVec4 *)&frame.vtxSlot0);
        Gfx_SetVertexSlotClipRect(3, (GfxHomogeneousVec4 *)&frame.vtxSlot1);
        frame.stripPrimB->tag = frame.stripPrimB->tag & 0xFFFFFF | 0x8000000;
        *(char *)((char *)frame.stripPrimB + 7) = 0x3A;
        Gfx_DrawPsxPrimitive(frame.stripPrimB);
    }
    g_BattleFxActiveDrawListPtr = (int)frame.stripPrimB;
    if (frame.gradStrip->timer >= 30) {
        frame.gradStrip->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.gradStrip->timer >= frame.gradStrip->duration) {
        frame.gradStrip->width -= frame.gradStrip->shrinkStep;
        frame.gradStrip->shrinkStep += (short)(frame.gradStrip->shrinkStep >> 4);
        if (frame.gradStrip->width < 2) {
            frame.gradStrip->width = 2;
        }
    }
    (frame.gradStrip->timer)++;
}

/*
 * Emits and simulates particle swarm emanating from actor bones 8 and 10.
 */
// FUNCTION: FF7 0x00446926
void __cdecl BattleFx_RevengeStampBone8And10ParticleTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[108];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxRevengeStampParticle *particle2; };
        struct { unsigned char pad_to_24[0x24]; int spawnCount; };
        struct { unsigned char pad_to_28[0x28]; int randScale; };
        struct { unsigned char pad_to_2C[0x2C]; int sizeBase; };
        struct { unsigned char pad_to_30[0x30]; int freeIndex; };
        struct { unsigned char pad_to_34[0x34]; int particleF10; };
        struct { unsigned char pad_to_38[0x38]; BattleFxRevengeStampBufferParticle *particleBuf; };
        struct { unsigned char pad_to_3C[0x3C]; int particleIndex; };
        struct { unsigned char pad_to_44[0x44]; SVECTOR bone10PosX; };
        struct { unsigned char pad_to_4C[0x4C]; SVECTOR bone8PosX; };
        struct { unsigned char pad_to_54[0x54]; SVECTOR rotX; };
        struct { unsigned char pad_to_5C[0x5C]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.particle2 = (BattleFxRevengeStampParticle *)BATTLE_CURRENT_TASK_SLOT;
    frame.particleBuf = (BattleFxRevengeStampBufferParticle *)(frame.particle2->bufferIndex * 0x960 + (char *)&g_WeaponFxRevengeStampParticleBuffers);
    Battle_GetBoneWorldPos(g_WeaponFxRevengeStampSourceActor, 8, &frame.bone8PosX.vx);
    Battle_GetBoneWorldPos(g_WeaponFxRevengeStampSourceActor, 0xA, &frame.bone10PosX.vx);
    frame.rotX.vx = 0;
    frame.rotX.vy = 0x800 /* 180 deg */;
    frame.rotX.vz = 0;
    g_WeaponFxRevengeStampBoneParticleSpriteReq.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, (GfxTableCursor *)(g_WeaponFxRevengeStampResourceContext));
    g_WeaponFxRevengeStampBoneParticleSpriteReq.drawObj = Gfx_ModelTableGetSpriteObject(0, (GfxTableCursor *)(g_WeaponFxRevengeStampResourceContext));
    for (frame.particleIndex = 0; frame.particleIndex < 100; frame.particleIndex++) {
        if (frame.particleBuf[frame.particleIndex].active != 0) {
            g_WeaponFxRevengeStampBoneParticleSpriteReq.frame = frame.particleBuf[frame.particleIndex].frame;
            frame.particleF10 = frame.particleBuf[frame.particleIndex].depthVelocity;
            if ((frame.particleF10 & 1) != 0) {
                frame.particleBuf[frame.particleIndex].depth = (((short *)&g_WeaponFxRevengeStampBonePos)[2]) + frame.particleF10;
            } else {
                frame.particleBuf[frame.particleIndex].depth = (((short *)&g_WeaponFxRevengeStampBonePos)[2]) - frame.particleF10;
            }
            RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
            frame.scaleVec.vz = frame.particleBuf[frame.particleIndex].scale;
            frame.scaleVec.vy = frame.scaleVec.vz;
            frame.scaleVec.vx = frame.scaleVec.vy;
            ScaleMatrix(&frame.rotMatrix, &frame.scaleVec);
            BattleFx_InstallBillboardMatrix((void *)((char *)frame.particleBuf + frame.particleIndex * 0x18 + 4), (short)frame.particleBuf[frame.particleIndex].scale, -(frame.particleBuf[frame.particleIndex].scale >> 3));
            g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_WeaponFxRevengeStampBoneParticleSpriteReq.spriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
            if (g_BattleIsPaused == 0) {
                frame.particleBuf[frame.particleIndex].position += frame.particleBuf[frame.particleIndex].velocity;
                frame.particleBuf[frame.particleIndex].height += frame.particleBuf[frame.particleIndex].heightVelocity;
                frame.particleBuf[frame.particleIndex].velocity -= (short)(frame.particleBuf[frame.particleIndex].velocity >> 3);
                frame.particleBuf[frame.particleIndex].heightVelocity -= (short)(frame.particleBuf[frame.particleIndex].heightVelocity >> 3);
                if (frame.particleBuf[frame.particleIndex].frame >= 7) {
                    frame.particleBuf[frame.particleIndex].active = 0;
                }
                (frame.particleBuf[frame.particleIndex].frame)++;
            }
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.particle2->frameIndex >= 118) {
        frame.particle2->ownerSlot = BATTLE_TASK_FREE;
    }
    if (frame.particle2->frameIndex >= 32 && frame.particle2->frameIndex <= 111) {
        frame.sizeBase = 0x1900;
        frame.spawnCount = frame.particle2->frameIndex >> 4;
        if (frame.spawnCount == 0) {
            frame.spawnCount = 1;
        }
        if (frame.spawnCount >= 2) {
            frame.spawnCount = 2;
        }
        for (frame.particleIndex = 0; frame.particleIndex < frame.spawnCount; frame.particleIndex++) {
            for (frame.freeIndex = 0; frame.freeIndex < 100 && frame.particleBuf[frame.freeIndex].active != 0; frame.freeIndex++) {
            }
            if (frame.freeIndex >= 100) {
                break;
            }
            frame.particleBuf[frame.freeIndex].active = 1;
            frame.particleBuf[frame.freeIndex].frame = 0;
            frame.particleBuf[frame.freeIndex].position = frame.bone8PosX.vx - (rand() % 1100 - 700);
            frame.particleBuf[frame.freeIndex].height = frame.bone8PosX.vy + rand() % 1400 + -900;
            frame.particleBuf[frame.freeIndex].depth = rand() % 1000 + 700;
            frame.particleBuf[frame.freeIndex].velocity = -(rand() % 55 + 30);
            frame.particleBuf[frame.freeIndex].heightVelocity = rand() % 40 + 28;
            frame.randScale = rand() % 0xB00 + 0xB00;
            frame.particleBuf[frame.freeIndex].scale = frame.sizeBase * frame.randScale >> 12;
        }
        for (frame.particleIndex = 0; frame.particleIndex < frame.spawnCount; frame.particleIndex++) {
            for (frame.freeIndex = 0; frame.freeIndex < 100 && frame.particleBuf[frame.freeIndex].active != 0; frame.freeIndex++) {
            }
            if (frame.freeIndex >= 100) {
                break;
            }
            frame.particleBuf[frame.freeIndex].active = 1;
            frame.particleBuf[frame.freeIndex].frame = 0;
            frame.particleBuf[frame.freeIndex].position = frame.bone10PosX.vx - (rand() % 2100 - 1300) + 700;
            frame.particleBuf[frame.freeIndex].height = frame.bone10PosX.vy + rand() % 1200 + -600;
            frame.particleBuf[frame.freeIndex].depth = rand() % 900 + 700;
            frame.particleBuf[frame.freeIndex].velocity = -(rand() % 105 + 60);
            frame.particleBuf[frame.freeIndex].heightVelocity = rand() % 40 + 28;
            frame.randScale = rand() % 0x800 + 0xA00;
            frame.particleBuf[frame.freeIndex].scale = frame.sizeBase * frame.randScale >> 12;
        }
    }
    (frame.particle2->frameIndex)++;
}

/*
 * Emits and simulates particle swarm emanating from actor bones 21 and 23.
 */
// FUNCTION: FF7 0x00446f2b
void __cdecl BattleFx_RevengeStampBone21And23ParticleTask(void)
{
    short bone21Pos[4];
    BattleFxRevengeStampBufferParticle *pParticles;
    int i;
    int unusedWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short positionB[4];
    int depthVel;
    BattleFxRevengeStampParticleSlot *emitter;
    int spawnCnt;
    int sizeRollSpan;
    int scaleBase;
    int freeIndex;

    emitter = (BattleFxRevengeStampParticleSlot *)BATTLE_CURRENT_TASK_SLOT;
    pParticles = (BattleFxRevengeStampBufferParticle *)(emitter->bufferIndex * 0x960 + (char *)&g_WeaponFxRevengeStampParticleBuffers);
    Battle_GetBoneWorldPos(g_WeaponFxRevengeStampSourceActor, 0x15, bone21Pos);
    Battle_GetBoneWorldPos(g_WeaponFxRevengeStampSourceActor, 0x17, positionB);
    g_WeaponFxRevengeStampSpriteReq1.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, (GfxTableCursor *)(g_WeaponFxRevengeStampResourceContext));
    g_WeaponFxRevengeStampSpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(0, (GfxTableCursor *)(g_WeaponFxRevengeStampResourceContext));
    for (i = 0; i < 100; i++) {
        if (pParticles[i].active != 0) {
            g_WeaponFxRevengeStampSpriteReq1.frame = pParticles[i].frame;
            depthVel = pParticles[i].depthVelocity;
            if ((depthVel & 1) != 0) {
                pParticles[i].depth = (short)(((short *)&g_WeaponFxRevengeStampBonePos)[2] + depthVel);
            } else {
                pParticles[i].depth = (short)(((short *)&g_WeaponFxRevengeStampBonePos)[2] - depthVel);
            }
            BattleFx_InstallBillboardMatrix(&pParticles[i].position, (short)pParticles[i].scale, -(pParticles[i].scale >> 3));
            g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_WeaponFxRevengeStampSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
            if (g_BattleIsPaused == 0) {
                pParticles[i].position += pParticles[i].velocity;
                pParticles[i].height += pParticles[i].heightVelocity;
                pParticles[i].velocity -= (short)(pParticles[i].velocity >> 3);
                pParticles[i].heightVelocity -= (short)(pParticles[i].heightVelocity >> 3);
                if (pParticles[i].frame >= 7) {
                    pParticles[i].active = 0;
                }
                (pParticles[i].frame)++;
            }
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (emitter->timer >= 118) {
        emitter->ownerSlot = BATTLE_TASK_FREE;
    }
    if (emitter->timer >= 32 && emitter->timer <= 111) {
        scaleBase = 0x1900;
        spawnCnt = emitter->timer >> 4;
        if (spawnCnt == 0) {
            spawnCnt = 1;
        }
        if (spawnCnt >= 2) {
            spawnCnt = 2;
        }
        for (i = 0; i < spawnCnt; i++) {
            for (freeIndex = 0; freeIndex < 100 && pParticles[freeIndex].active != 0; freeIndex++) {
            }
            if (freeIndex >= 100) {
                break;
            }
            pParticles[freeIndex].active = 1;
            pParticles[freeIndex].frame = 0;
            pParticles[freeIndex].position = (short)(bone21Pos[0] + (rand() % 1100 - 700));
            pParticles[freeIndex].height = (short)(bone21Pos[1] + (rand() % 1400 - 900));
            pParticles[freeIndex].depth = (short)(rand() % 1000 + 700);
            pParticles[freeIndex].velocity = (short)(rand() % 55 + 30);
            pParticles[freeIndex].heightVelocity = (short)(rand() % 40 + 28);
            sizeRollSpan = rand() % 0xB00 + 0xB00;
            pParticles[freeIndex].scale = (short)(scaleBase * sizeRollSpan >> 12);
        }
        for (i = 0; i < spawnCnt; i++) {
            for (freeIndex = 0; freeIndex < 100 && pParticles[freeIndex].active != 0; freeIndex++) {
            }
            if (freeIndex >= 100) {
                break;
            }
            pParticles[freeIndex].active = 1;
            pParticles[freeIndex].frame = 0;
            pParticles[freeIndex].position = (short)(positionB[0] + (rand() % 2100 - 2000));
            pParticles[freeIndex].height = (short)(positionB[1] + (rand() % 1200 - 600));
            pParticles[freeIndex].depth = (short)(rand() % 900 + 700);
            pParticles[freeIndex].velocity = (short)(rand() % 105 + 60);
            pParticles[freeIndex].heightVelocity = (short)(rand() % 40 + 28);
            sizeRollSpan = rand() % 0x800 + 0xA00;
            pParticles[freeIndex].scale = (short)(scaleBase * sizeRollSpan >> 12);
        }
    }
    (emitter->timer)++;
}

/*
 * Emits and simulates lateral burst particles dispersing horizontally.
 */
// FUNCTION: FF7 0x004474d5
void __cdecl BattleFx_RevengeStampSideBurstParticleTask(void)
{
    int idx;
    int padSlot; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxRevengeStampBufferParticle *buf;
    int slot;
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxRevengeStampParticleSlot *ep;
    int baseScale;
    int quota;
    int scaleRoll;

    ep = (BattleFxRevengeStampParticleSlot *)BATTLE_CURRENT_TASK_SLOT;
    buf = (BattleFxRevengeStampBufferParticle *)(ep->bufferIndex * 0x960 + (char *)&g_WeaponFxRevengeStampParticleBuffers);
    g_WeaponFxRevengeStampSpriteReq2.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, (GfxTableCursor *)(g_WeaponFxRevengeStampResourceContext));
    g_WeaponFxRevengeStampSpriteReq2.drawObj = Gfx_ModelTableGetSpriteObject(1, (GfxTableCursor *)(g_WeaponFxRevengeStampResourceContext));
    for (idx = 0; idx < 100; idx++) {
        if (buf[idx].active != 0) {
            g_WeaponFxRevengeStampSpriteReq2.frame = buf[idx].frame;
            BattleFx_InstallBillboardMatrix(&buf[idx].position, (short)buf[idx].scale, -(buf[idx].scale >> 3));
            g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_WeaponFxRevengeStampSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
            if (g_BattleIsPaused == 0) {
                buf[idx].position += buf[idx].velocity;
                buf[idx].height += buf[idx].heightVelocity;
                buf[idx].depth += buf[idx].depthVelocity;
                buf[idx].velocity -= (short)(buf[idx].velocity >> 4);
                buf[idx].heightVelocity -= (short)(buf[idx].heightVelocity >> 4);
                buf[idx].depthVelocity -= (short)(buf[idx].depthVelocity >> 4);
                if (buf[idx].frame >= 15) {
                    buf[idx].active = 0;
                }
                (buf[idx].frame)++;
            }
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (ep->timer >= 91) {
        ep->ownerSlot = BATTLE_TASK_FREE;
    }
    if (ep->timer >= 1 && ep->timer <= 76) {
        baseScale = 0x1900;
        quota = ep->timer >> 3;
        if (quota == 0) {
            quota = 1;
        }
        if (quota >= 3) {
            quota = 3;
        }
        for (idx = 0; idx < quota; idx++) {
            for (slot = 0; slot < 100 && buf[slot].active != 0; slot++) {
            }
            if (slot >= 100) {
                break;
            }
            buf[slot].active = 1;
            buf[slot].frame = 0;
            buf[slot].position = (short)(rand() % 2600 + 400);
            buf[slot].height = (short)(((short *)&g_WeaponFxRevengeStampBonePos)[1] - 0x190);
            buf[slot].depth = (short)(((short *)&g_WeaponFxRevengeStampBonePos)[2] + (rand() % 300 - 150));
            buf[slot].velocity = (short)(rand() % 55 + 30);
            buf[slot].heightVelocity = (short)(rand() % 80 + 160);
            buf[slot].depthVelocity = (short)-(rand() % 60 + 20);
            scaleRoll = rand() % 0x800 + 0xB00;
            buf[slot].scale = (short)(baseScale * scaleRoll >> 12);
        }
        for (idx = 0; idx < quota; idx++) {
            for (slot = 0; slot < 100 && buf[slot].active != 0; slot++) {
            }
            if (slot >= 100) {
                break;
            }
            buf[slot].active = 1;
            buf[slot].frame = 0;
            buf[slot].position = (short)-(rand() % 2600 + 400);
            buf[slot].height = (short)(((short *)&g_WeaponFxRevengeStampBonePos)[1] - 0x190);
            buf[slot].depth = (short)(((short *)&g_WeaponFxRevengeStampBonePos)[2] - (rand() % 300 - 150));
            buf[slot].velocity = (short)-(rand() % 55 + 30);
            buf[slot].heightVelocity = (short)(rand() % 80 + 160);
            buf[slot].depthVelocity = (short)(rand() % 60 + 20);
            scaleRoll = rand() % 0x800 + 0xB00;
            buf[slot].scale = (short)(baseScale * scaleRoll >> 12);
        }
    }
    (ep->timer)++;
}
} /* extern "C" */
