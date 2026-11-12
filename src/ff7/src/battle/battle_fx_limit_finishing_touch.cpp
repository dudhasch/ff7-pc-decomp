// src/ff7/src/battle/battle_fx_limit_finishing_touch.cpp
// Limit Break [5, 77] (Cloud Lv 3-2): Finishing Touch (Japanese: 画龍点睛, dev tag: "limit2/garyoten" / Garyoten)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Garyoten limit break effect: a ring chain of sprite packets, radial sprite bursts, a
//   falling model, and actor lift and return tasks, all driven from one timeline with lazily
//   acquired scratch and resources.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_limit.h"
#include "ff7/battle_tasks.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/api/graphics.h"
extern "C" {

#pragma intrinsic(memset)

// clang-format off
// GLOBAL: FF7 0x0082bba8
int g_BattleLimitFinishingTouchResourceDef = 96313;
char *g_BattleFxLimitFinishingTouchResourceNames[1] = {
    // STRING: FF7 0x0082bcf0
    "typhoon2.tim"
};
unsigned int g_BattleLimitFinishingTouchResourceDefDataTail[48] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0082bd00
    (unsigned int)"smoke_2.TIM",
    // STRING: FF7 0x0082bd0c
    (unsigned int)"SMOKE_1.SPT",
    0x00000000,
    0x00000000,
    0x006400b1,
    // STRING: FF7 0x0082bd18
    (unsigned int)"FLASH_1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x006400b1,
    // STRING: FF7 0x0082bd24
    (unsigned int)"KIRA_1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x003200b4,
    // STRING: FF7 0x0082bd30
    (unsigned int)"ROCK_1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x003200b4,
    // STRING: FF7 0x0082bd3c
    (unsigned int)"ROCK_2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x003200b4,
    // STRING: FF7 0x0082bd48
    (unsigned int)"ROCK_3.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x003200b1,
    // STRING: FF7 0x0082bd54
    (unsigned int)"ROCK_1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x003200b1,
    // STRING: FF7 0x0082bd60
    (unsigned int)"ROCK_2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x003200b1,
    // STRING: FF7 0x0082bd6c
    (unsigned int)"ROCK_3.RSD",
    0x00000000,
    0x00000000,
    0x00000000
};
// The model table entries of the three falling rocks, and of their fading versions from age 14.
// GLOBAL: FF7 0x0082bc70
int g_BattleLimitFinishingTouchModelReq1[4] = {4, 5, 6, 0};
// GLOBAL: FF7 0x0082bc80
int g_BattleLimitFinishingTouchModelReq2[4] = {7, 8, 9, 0};
// The ring chain's transform (identity rotation; the ring centre goes into t[0] and t[2]) and the
// sprite requests of the radial sprites and of the per-actor bursts.
typedef struct {
    PsxMatrix transform;
    short pad_1e;
    BattleFxSpriteRequest radialSprite;
    BattleFxSpriteRequest burstSprite;
} BattleLimitGaryotenSpritePool;
// GLOBAL: FF7 0x0082bc90
BattleLimitGaryotenSpritePool g_BattleLimitFinishingTouchSpriteReqPool = {
    {{{0x1000, 0, 0}, {0, 0x1000, 0}, {0, 0, 0x1000}}, {0, 0, 0}},
    0,
    {0, 0, 0x50, 0x50, 0x50, 0x2e, 0, 0},
    {0, 0, (char)0x80, (char)0x80, (char)0x80, 0x2e, 0, 0}
};
unsigned char g_rawData_0082bcd0[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x2e, 0x00, 0x00, 0x00, 0x00};
// The scaled model's scale per stage (4.12 fixed point).
// GLOBAL: FF7 0x0082bce0
int g_BattleLimitFinishingTouchScaleTable[4] = {0xE00, 0x2200, 0x1000, 0x500};
// GLOBAL: FF7 0x00af8b28
BattleTaskSlot *g_BattleLimitFinishingTouchCursorTaskSlot = 0;
// GLOBAL: FF7 0x00af8b2c
BattleLimitFinishingTouchRingScratch *g_pBattleLimitFinishingTouchRingScratch = 0;
// The first of the ten actors' burst sprite pools (48 BattleFxFinishingTouchBurstParticle records,
// 0x480 bytes, each); the rest lie in the padding after it.
// GLOBAL: FF7 0x00af8b30
unsigned char g_BattleLimitFinishingTouchParticleBuf[1132] = {0};
unsigned char g_bssPad_00af8f9c[4] = {0};
unsigned char g_bssPad_00af8fa0[10384] = {0};
// The 140 sprites of BattleFx_LimitFinishingTouchRadialSpriteEmitterTask.
// GLOBAL: FF7 0x00afb830
BattleFxFinishingTouchBurstParticle g_BattleLimitFinishingTouchRadialSprites[140] = {0};
// GLOBAL: FF7 0x00afc550
int g_BattleLimitFinishingTouchScratchBlock = 0;
// GLOBAL: FF7 0x00afc554
unsigned char g_BattleLimitFinishingTouchRingScratchReady = 0;
// GLOBAL: FF7 0x00afc558
int g_BattleLimitFinishingTouchDrawHandle = 0;
// GLOBAL: FF7 0x00afc55c
int g_BattleLimitFinishingTouchDrawListHead = 0;
// GLOBAL: FF7 0x00afc560
BattleLimitFinishingTouchPoint g_BattleLimitFinishingTouchAttachPoint = {0};
// The head of the first of ten BattleLimitFinishingTouchPeerGroup records; g_BattleLimitFinishingTouchPeerList
// is its first peer, and the other records follow in the padding.
typedef struct {
    short timelineId;
    short peerCount;
} BattleLimitGaryotenPeerGroupHead;
// GLOBAL: FF7 0x00afc568
BattleLimitGaryotenPeerGroupHead g_BattleLimitFinishingTouchPeerGroupHead = {0};
// GLOBAL: FF7 0x00afc56c
int g_BattleLimitFinishingTouchPeerList = 0;
unsigned char g_bssPad_00afc570[392] = {0};
unsigned char g_bssPad_00afc6f8[40] = {0};
// GLOBAL: FF7 0x00afc720
BattleLimitFinishingTouchPoint g_BattleLimitFinishingTouchRadialBurst = {0};
// GLOBAL: FF7 0x00afc728
int g_BattleLimitFinishingTouchTargetMask = 0;
// GLOBAL: FF7 0x00afc72c
int g_BattleLimitFinishingTouchEventFlags = 0;
// GLOBAL: FF7 0x00afc730
int g_BattleLimitFinishingTouchActorIndex = 0;
// GLOBAL: FF7 0x00afc734
unsigned char g_BattleLimitFinishingTouchScratchBlockReady = 0;
// GLOBAL: FF7 0x00afc738
BattleFxFinishingTouchState *g_pBattleLimitFinishingTouchDrawBlock = 0;
unsigned char g_bssPad_00afc73c[4] = {0};
// GLOBAL: FF7 0x00afc740
BattleLimitGaryotenQuadRow g_BattleLimitFinishingTouchRingQuadPackets[15] = {0};
// GLOBAL: FF7 0x00b01240
short g_BattleLimitFinishingTouchRingPackets[4] = {0};
// The ring records (BattleLimitFinishingTouchRing) are 20 bytes each; g_BattleLimitFinishingTouchRingAngles
// holds the first record's angle triple.
// GLOBAL: FF7 0x00b01248
unsigned char g_BattleLimitFinishingTouchRingAngles[8] = {0};
// The radius of each of the 16 ring records, read at a 20-byte stride; the array runs
// 20 * 15 + 4 = 0x130 bytes, up to g_BattleLimitFinishingTouchLiftState.
// GLOBAL: FF7 0x00b01250
unsigned char g_BattleLimitFinishingTouchRingScales[304] = {0};
// 10-slot actor lift state table (split into head + g_bssPad_00b01388).
// GLOBAL: FF7 0x00b01380
struct BattleLimitFinishingTouchLiftState g_BattleLimitFinishingTouchLiftState = {0};
#define g_BattleLimitFinishingTouchLiftStates (&g_BattleLimitFinishingTouchLiftState)
unsigned char g_bssPad_00b01388[72] = {0};
// 10-slot saved actor rotation table (split into head + g_bssPad_00b013d8).
// GLOBAL: FF7 0x00b013d0
Int64Pair g_BattleLimitFinishingTouchSavedRotations = {0};
#define g_BattleLimitFinishingTouchSavedRotationTable (&g_BattleLimitFinishingTouchSavedRotations)
unsigned char g_bssPad_00b013d8[72] = {0};
// The actors' draw flags saved before the lift.
// GLOBAL: FF7 0x00b01420
unsigned char g_BattleLimitFinishingTouchSavedDrawFlags[12] = {0};
// GLOBAL: FF7 0x00b0142c
int g_BattleLimitFinishingTouchDrawBlockReady = 0;
// GLOBAL: FF7 0x00b01430
struct GfxTableCursor *g_pBattleLimitFinishingTouchTableCursor = 0;
// GLOBAL: FF7 0x00b01434
int g_BattleLimitFinishingTouchPacketBank = 0;
// GLOBAL: FF7 0x00b01438
struct BattleLimitFinishingTouchSwayState g_BattleLimitFinishingTouchSwayState = {0};
// GLOBAL: FF7 0x00b0143c
short g_BattleLimitFinishingTouchSwayAngleZ = 0;
// GLOBAL: FF7 0x00b01440
struct BattleLimitFinishingTouchSwayWalk g_BattleLimitFinishingTouchSwayWalk = {0};
// GLOBAL: FF7 0x00b01444
short g_BattleLimitFinishingTouchSwayWalkZ = 0;
// clang-format on

#pragma intrinsic(memcpy)
#pragma function(memcpy)

/*
 * Loads the limit2/garyoten model table (resource type ten) once, when a 3D scene exists and the
 * table cursor is still empty.
 */
// FUNCTION: FF7 0x004be8a0
void __cdecl BattleFx_LimitLoadResFinishingTouch(void)
{
    GfxModelTableBank *drawDev;
    GfxResourceRequest recordBuf;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int tempResDef;

    if (g_pBattle3dScene != 0 && g_pBattleLimitFinishingTouchTableCursor == 0) {
        kind = 0xA;
        tempResDef = (int)&g_BattleLimitFinishingTouchResourceDef;
        drawDev = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &recordBuf;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0082bd78
        pRequest->resourcePath = BattleFx_BuildResourcePath("limit2/garyoten", pathText);
        Gfx_LoadModelTableIfAbsent(kind, tempResDef, pRequest, &g_pBattleLimitFinishingTouchTableCursor, drawDev);
    }
}

/*
 * Calls BattleFx_LimitLoadResFinishingTouch, then BattleFx_LimitBeginFinishingTouch with both arguments, and returns zero.
 */
// FUNCTION: FF7 0x004be930
int __cdecl BattleFx_LimitEnterFinishingTouch(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResFinishingTouch();
    BattleFx_LimitBeginFinishingTouch(targetMask, actorIdx);
    return 0;
}

/*
 * Calls BattleFx_LimitBeginFinishingTouchAlt with both arguments and returns zero.
 */
// FUNCTION: FF7 0x004be94c
int __cdecl BattleFx_LimitEnterFinishingTouchAlt(int targetMask, int actorIdx)
{
    BattleFx_LimitBeginFinishingTouchAlt(targetMask, actorIdx);
    return 0;
}

/*
 * Allocates secondary BattleFx_LimitFinishingTouchSelectDrawCursorTask and primary
 * BattleFx_LimitFinishingTouchMaskGatedExpiryTask slots, sets the expiry task's actorMask to
 * GARYOTEN_EVENT_RELEASE_FIRST, allocates BattleFx_LimitFinishingTouchSpawnTimeline, stores both
 * arguments, and clears g_BattleLimitFinishingTouchEventFlags.
 */
// FUNCTION: FF7 0x004be963
void __cdecl BattleFx_LimitBeginFinishingTouch(int targetMask, int actorIdx)
{
    BattleFxFinishingTouchMaskTask *task;

    g_BattleLimitFinishingTouchCursorTaskSlot = &g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_LimitFinishingTouchSelectDrawCursorTask)];
    task = (BattleFxFinishingTouchMaskTask *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitFinishingTouchMaskGatedExpiryTask);
    task->actorMask = GARYOTEN_EVENT_RELEASE_FIRST;
    Battle_AllocateResourceSlot(BattleFx_LimitFinishingTouchSpawnTimeline);
    g_BattleLimitFinishingTouchTargetMask = targetMask;
    g_BattleLimitFinishingTouchActorIndex = actorIdx;
    g_BattleLimitFinishingTouchEventFlags = 0;
}

/*
 * Frees its own slot, saves the source actor's target bone position and radius as the attach
 * point, starts BattleFx_LimitFinishingTouchTimelineTask at the targets' centroid, then for all ten
 * actors saves the position, rotation and draw flags and clears the actor's peer group. No pause
 * guard appears.
 */
// FUNCTION: FF7 0x004be9d5
void __cdecl BattleFx_LimitFinishingTouchSpawnTimeline(void)
{
    int idx;
    int q;
    int r;
    BattleLimitFinishingTouchTimeline *task;

    task = (BattleLimitFinishingTouchTimeline *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    Battle_GetBoneWorldPos(g_BattleLimitFinishingTouchActorIndex, g_BattleActorRecords[g_BattleLimitFinishingTouchActorIndex].targetBone, &g_BattleLimitFinishingTouchAttachPoint.posX);
    g_BattleLimitFinishingTouchAttachPoint.radius = g_BattleActorRecords[g_BattleLimitFinishingTouchActorIndex].radius;
    task = (BattleLimitFinishingTouchTimeline *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_LimitFinishingTouchTimelineTask)];
    Battle_GetActorMaskCentroid(g_BattleLimitFinishingTouchTargetMask, &task->targetX);
    for (idx = 0; idx < 10; idx++) {
        *(Int64Pair *)&g_BattleLimitFinishingTouchLiftStates[idx] = *(Int64Pair *)&g_BattleActorRecords[idx].posX;
        g_BattleLimitFinishingTouchSavedRotationTable[idx] = *(Int64Pair *)&g_BattleActorRecords[idx].rotX;
        g_BattleLimitFinishingTouchSavedDrawFlags[idx] = g_BattleActorRecords[idx].drawFlags;
        ((BattleLimitFinishingTouchPeerGroup *)&g_BattleLimitFinishingTouchPeerGroupHead)[idx].timelineId = 0xFFFF;
        ((BattleLimitFinishingTouchPeerGroup *)&g_BattleLimitFinishingTouchPeerGroupHead)[idx].peerCount = 0;
    }
}

/*
 * Returns immediately when paused or GARYOTEN_EVENT_HOLD is set. Otherwise schedules model,
 * ring, actor-motion and sound events at explicit ages, groups targets sharing X/Z, and caches
 * peer motion state. At 95 creates typed records and sets GARYOTEN_EVENT_HOLD, so the age-110/120
 * events wait for BattleFx_LimitBeginFinishingTouchAlt to clear it. At 120 or cancellation retires and releases
 * sound 0x182; advances age.
 */
// FUNCTION: FF7 0x004beb0b
void __cdecl BattleFx_LimitFinishingTouchTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        int peerCount;
        struct { unsigned char pad_to_4[0x4]; BattleLimitFinishingTouchTimeline *timeline; };
        struct { unsigned char pad_to_8[0x8]; BattleFxLimitGaryotenSlot *garyotenSlot; };
        struct { unsigned char pad_to_C[0xC]; int peerIndex; };
        struct { unsigned char pad_to_10[0x10]; int otherIndex; };
        struct { unsigned char pad_to_14[0x14]; int actorIndex; };
        struct { unsigned char pad_to_18[0x18]; short attachX; };
        struct { unsigned char pad_to_1A[0x1A]; short attachY; };
        struct { unsigned char pad_to_1C[0x1C]; short attachZ; };
        struct { unsigned char pad_to_20[0x20]; BattleFxFinishingTouchRing *ringRecord; };
        struct { unsigned char pad_to_24[0x24]; int peerList[10]; };
    } frame;
#pragma pack(pop)

    if ((g_BattleLimitFinishingTouchEventFlags & GARYOTEN_EVENT_HOLD) != 0) {
        return;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.timeline = (BattleLimitFinishingTouchTimeline *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    if (frame.timeline->phase == 0) {
        SfxLoadIntoBuffer(0x182, 0);
    }
    if (frame.timeline->phase == 0x18) {
        frame.actorIndex = 4915;
        frame.garyotenSlot = (BattleFxLimitGaryotenSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitFinishingTouchGrowShrinkModelTask);
        *(Int64Pair *)&frame.garyotenSlot->growShrink.posX = *(Int64Pair *)&g_BattleLimitFinishingTouchAttachPoint;
        frame.garyotenSlot->growShrink.growth = 0;
        frame.garyotenSlot->growShrink.growthRate = 0x12C;
        frame.garyotenSlot->growShrink.scaleRate = frame.actorIndex / 5;
        frame.garyotenSlot->growShrink.scale = frame.garyotenSlot->growShrink.scaleRate;
        frame.garyotenSlot->growShrink.scaleDecay = frame.actorIndex / 6;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&frame.timeline->targetX), 0x130);
    }
    if (frame.timeline->phase == 0x1E) {
        frame.garyotenSlot = (BattleFxLimitGaryotenSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitFinishingTouchScaledModelTask);
        frame.attachX = 0;
        frame.attachY = 0;
        frame.attachZ = 0xFDD5 /* -555 */;
        BattleFx_LimitFinishingTouchTransformAttachPointToEffectSpace(g_BattleLimitFinishingTouchActorIndex, -1, &frame.attachX, &frame.garyotenSlot->scaledModel.posX);
        frame.garyotenSlot->scaledModel.baseScale = 0x1CCC;
    }
    if (frame.timeline->phase == 0x25) {
        frame.ringRecord = (BattleFxFinishingTouchRing *)(&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_LimitFinishingTouchRingChainTask)]);
        frame.attachX = 0;
        frame.attachY = 0;
        frame.attachZ = 0;
        BattleFx_LimitFinishingTouchTransformAttachPointToEffectSpace(g_BattleLimitFinishingTouchActorIndex, -1, &frame.attachX, &g_BattleLimitFinishingTouchAttachPoint.posX);
        *(Int64Pair *)&frame.ringRecord->centerX = *(Int64Pair *)&g_BattleLimitFinishingTouchAttachPoint;
        frame.ringRecord->velocityX = (frame.timeline->targetX - g_BattleLimitFinishingTouchAttachPoint.posX) / 10 << 1;
        frame.ringRecord->velocityZ = (frame.timeline->targetZ - g_BattleLimitFinishingTouchAttachPoint.posZ) / 10 << 1;
    }
    if (frame.timeline->phase == 0x35) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, 0x40, 0x182);
    } else if (frame.timeline->phase == 0x69 || (g_BattleLimitFinishingTouchEventFlags & GARYOTEN_EVENT_CANCEL) != 0) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, 0x40, 0);
    }
    if (frame.timeline->phase == 0x25) {
        frame.garyotenSlot = (BattleFxLimitGaryotenSlot *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_LimitFinishingTouchRadialSpriteEmitterTask)];
        *(int *)&g_BattleLimitFinishingTouchRadialBurst.posX = *(int *)&g_BattleLimitFinishingTouchAttachPoint.posX;
        *(int *)&g_BattleLimitFinishingTouchRadialBurst.posZ = *(int *)&g_BattleLimitFinishingTouchAttachPoint.posZ;
        g_BattleLimitFinishingTouchRadialBurst.radius = 600;
    }
    if (frame.timeline->phase == 0x3C) {
        for (frame.actorIndex = 0; frame.actorIndex < 10; frame.actorIndex++) {
            if ((g_BattleLimitFinishingTouchTargetMask & 1 << frame.actorIndex) != 0) {
                frame.peerCount = 0;
                for (frame.otherIndex = 0; frame.otherIndex < 10; frame.otherIndex++) {
                    if (frame.actorIndex == frame.otherIndex) {
                        continue;
                    }
                    if (g_BattleActorRecords[frame.actorIndex].posX != g_BattleActorRecords[frame.otherIndex].posX) {
                        continue;
                    }
                    if (g_BattleActorRecords[frame.actorIndex].posZ != g_BattleActorRecords[frame.otherIndex].posZ) {
                        continue;
                    }
                    frame.peerList[frame.peerCount] = frame.otherIndex;
                    frame.peerCount++;
                }
                frame.garyotenSlot = (BattleFxLimitGaryotenSlot *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_LimitFinishingTouchActorLiftReturnTask)];
                frame.garyotenSlot->lift.actorIndex = *(short *)&frame.actorIndex;
                frame.garyotenSlot->lift.returnBlocked = g_BattleActiveTargetMask & 1 << frame.actorIndex;
                for (frame.otherIndex = 0; frame.otherIndex < frame.peerCount; frame.otherIndex++) {
                    frame.peerIndex = frame.peerList[frame.otherIndex];
                    if (((BattleLimitFinishingTouchPeerGroup *)&g_BattleLimitFinishingTouchPeerGroupHead)[frame.peerIndex].timelineId < 0) {
                        continue;
                    }
                    frame.garyotenSlot->lift.stepLimit = ((BattleLimitFinishingTouchPeerGroup *)&g_BattleLimitFinishingTouchPeerGroupHead)[frame.peerIndex].timelineId;
                    break;
                }
                if (frame.otherIndex >= frame.peerCount) {
                    frame.garyotenSlot->lift.liftVelocity = -(rand() % 30 + 40);
                    frame.garyotenSlot->lift.yawVelocity = 0;
                    frame.garyotenSlot->lift.yawAccel = rand() % 0x22 + 0x1C;
                    if ((frame.garyotenSlot->lift.liftVelocity & 1) != 0) {
                        frame.garyotenSlot->lift.yawAccel = -frame.garyotenSlot->lift.yawAccel;
                    }
                    frame.garyotenSlot->lift.stepLimit = rand() % 7 + 1;
                    frame.garyotenSlot->lift.stepCount = 0;
                    Battle_GetBoneWorldPos(frame.actorIndex, g_BattleActorRecords[frame.actorIndex].altBone, &frame.attachX);
                    frame.garyotenSlot->lift.baseY = frame.attachY;
                    ((BattleLimitFinishingTouchPeerGroup *)&g_BattleLimitFinishingTouchPeerGroupHead)[frame.actorIndex].timelineId = frame.garyotenSlot->lift.stepLimit;
                    ((BattleLimitFinishingTouchPeerGroup *)&g_BattleLimitFinishingTouchPeerGroupHead)[frame.actorIndex].peerCount = *(short *)&frame.peerCount;
                    for (frame.peerIndex = 0; frame.peerIndex < frame.peerCount; frame.peerIndex++) {
                        ((BattleLimitFinishingTouchPeerList *)&g_BattleLimitFinishingTouchPeerList)[frame.actorIndex].peers[frame.peerIndex] = frame.peerList[frame.peerIndex];
                    }
                }
            }
        }
    }
    if (frame.timeline->phase == 0x5F) {
        for (frame.actorIndex = 0; frame.actorIndex < 10; frame.actorIndex++) {
            if ((g_BattleLimitFinishingTouchTargetMask & 1 << frame.actorIndex) != 0) {
                Battle_PostActorHitOrHurtMessage(frame.actorIndex);
            }
        }
        g_BattleLimitFinishingTouchEventFlags = GARYOTEN_EVENT_HOLD | GARYOTEN_EVENT_RELEASE_FIRST;
    }
    if (frame.timeline->phase == 0x6E && (g_BattleLimitFinishingTouchEventFlags & GARYOTEN_EVENT_CANCEL) == 0) {
        for (frame.actorIndex = 0; frame.actorIndex < 10; frame.actorIndex++) {
            if ((g_BattleLimitFinishingTouchTargetMask & 1 << frame.actorIndex) != 0) {
                Battle_PostActorHitOrHurtMessage(frame.actorIndex);
            }
        }
    }
    if (frame.timeline->phase >= 120 || (g_BattleLimitFinishingTouchEventFlags & GARYOTEN_EVENT_CANCEL) != 0) {
        g_BattleLimitFinishingTouchEventFlags |= GARYOTEN_EVENT_DONE | GARYOTEN_EVENT_RELEASE_SECOND | GARYOTEN_EVENT_RELEASE_FIRST;
        frame.timeline->ownerSlot = BATTLE_TASK_FREE;
        Sfx_ReleaseCachedBuffer(0x182, 0xF);
    }
    (frame.timeline->phase)++;
}

/*
 * Initializes ring geometry at unpaused age zero, computes a four-phase radius/length profile and
 * renders its camera-composed matrix with fade-in/out. Final-phase retirement returns before
 * drawing only when unpaused. Publishes task position and a radius local that is uninitialized on
 * paused calls, and at unpaused ages 32 through 54 emits falling-model tasks while the secondary
 * count permits.
 */
// FUNCTION: FF7 0x004bf111
void __cdecl BattleFx_LimitFinishingTouchRingChainTask(void)
{
    int size;
    int t;
    int d; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int cnt;
    int outRatio;
    int len;
    PsxMatrix matrix;
    BattleFxFallingModelSlot *newTask;
    int ringTilt;
    int fadeLevel;
    BattleFxFinishingTouchRing *taskSlot;
    int phaseAge;
    int dirAngle;
    int ringSpacing;

    taskSlot = (BattleFxFinishingTouchRing *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    if ((g_BattleLimitFinishingTouchScratchBlockReady & 1) == 0) {
        g_BattleLimitFinishingTouchScratchBlockReady |= 1;
        g_BattleLimitFinishingTouchScratchBlock = Gfx_GetPsxScratchpadPtr(0);
    }
    if (taskSlot->timer == 0 && g_BattleIsPaused == 0) {
        BattleFx_LimitFinishingTouchInitRingPackets();
    }
    if (taskSlot->timer < 10) {
        fadeLevel = (taskSlot->timer << 7) / 10;
    } else if (taskSlot->timer >= 54) {
        fadeLevel = 128 - (taskSlot->timer - 54 << 7) / 12;
    } else {
        fadeLevel = 128;
    }
    if (g_BattleIsPaused == 0) {
        phaseAge = taskSlot->timer;
        if (phaseAge < 17) {
            outRatio = (phaseAge << 12) / 17;
            ringSpacing = (outRatio * 0x316 >> 12) + 10;
            size = (outRatio * 0x1F4 >> 12) + 600;
            len = (outRatio * 0xC8 >> 12) + 50;
            ringTilt = 0xE00;
        } else {
            phaseAge -= 17;
            if (phaseAge < 10) {
                taskSlot->centerX += taskSlot->velocityX;
                taskSlot->centerZ += taskSlot->velocityZ;
                taskSlot->velocityX -= (short)(taskSlot->velocityX >> 3);
                taskSlot->velocityZ -= (short)(taskSlot->velocityZ >> 3);
                outRatio = (phaseAge << 12) / 10;
                ringSpacing = (outRatio * 0x708 >> 12) + 800;
                size = (outRatio * 0x3E8 >> 12) + 1100;
                len = (outRatio * 0xFA >> 12) + 250;
                ringTilt = (outRatio * (-0x500) >> 12) + 0xE00;
            } else {
                phaseAge -= 10;
                if (phaseAge < 22) {
                    outRatio = (phaseAge << 12) / 22;
                    ringSpacing = (outRatio * 0x1900 /* 1.5625 */ >> 12) + 2600;
                    size = (outRatio * 0x7D0 >> 12) + 2100;
                    len = (outRatio * 0x4B0 >> 12) + 500;
                    ringTilt = (outRatio * (-0x3E0) >> 12) + 0x900;
                } else {
                    phaseAge -= 22;
                    if (phaseAge < 17) {
                        outRatio = (phaseAge << 12) / 17;
                        ringSpacing = (outRatio * (-4000) >> 12) + 9000;
                        size = (outRatio * (-2300) >> 12) + 4100;
                        len = (outRatio * (-800) >> 12) + 1700;
                        ringTilt = (outRatio << 10 >> 12) + 0x520;
                    } else {
                        taskSlot->ownerSlot = BATTLE_TASK_FREE;
                        return;
                    }
                }
            }
        }
        BattleFx_LimitFinishingTouchUpdateRingChain(ringSpacing, size, len, ringTilt);
    }
    g_BattleLimitFinishingTouchSpriteReqPool.transform.t[0] = taskSlot->centerX;
    g_BattleLimitFinishingTouchSpriteReqPool.transform.t[2] = taskSlot->centerZ;
    MultiplyMatrix((void *)g_BattleCameraTransform.matrix, &g_BattleLimitFinishingTouchSpriteReqPool.transform, &matrix);
    SetRotMatrix(&matrix);
    SetTransMatrix(&matrix);
    RefreshGlobalTransformState();
    BattleFx_LimitFinishingTouchDrawRingChain(taskSlot->timer << 7, fadeLevel);
    *(Int64Pair *)&g_BattleLimitFinishingTouchRadialBurst = *(Int64Pair *)&taskSlot->centerX;
    g_BattleLimitFinishingTouchRadialBurst.radius = size;
    if (g_BattleIsPaused == 0) {
        if (taskSlot->timer >= 32 && taskSlot->timer <= 54) {
            for (cnt = 0; cnt < 3; cnt++) {
                if (g_BattleSecondaryResourceCount > 50) {
                    break;
                }
                newTask = (BattleFxFallingModelSlot *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_LimitFinishingTouchFallingModelTask)];
                dirAngle = ((rand()) % 4096);
                outRatio = (size >> 1) + rand() % (size >> 1);
                newTask->posX = taskSlot->centerX + (CosFixed(dirAngle) * size >> 12);
                newTask->fallPos = taskSlot->centerHeight;
                newTask->posZ = taskSlot->centerZ - (SinFixed(dirAngle) * size >> 12);
                newTask->fallVelocity = -(rand() % 108 + 38);
                newTask->angleZ = 0;
                newTask->angleX = 0;
                newTask->angleSpeedX = rand() % 230 + 64;
                newTask->angleSpeedZ = rand() % 180 + 48;
                newTask->scale = rand() % 0x580 + 0x260;
                newTask->modelIndex = newTask->scale % 3;
            }
        }
        (taskSlot->timer)++;
    }
}

/*
 * Renders the selected model with stored X/Z angles, uniform scale and camera transform. From age
 * fourteen sets descriptor bit eight and decreases gray intensity without clamping; draws and
 * retires at twenty before checking pause. Unpaused calls update Y and its accelerating velocity,
 * both angles and age.
 */
// FUNCTION: FF7 0x004bf647
void __cdecl BattleFx_LimitFinishingTouchFallingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; SVECTOR angleX; };
        struct { unsigned char pad_to_28[0x28]; BattleFxFallingModelSlot *fallSlot; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxModelDraw *drawReq; };
        struct { unsigned char pad_to_30[0x30]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.fallSlot = (BattleFxFallingModelSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    frame.angleX.vx = frame.fallSlot->angleX;
    frame.angleX.vy = 0;
    frame.angleX.vz = frame.fallSlot->angleZ;
    RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.fallSlot->posX;
    frame.rotMatrix.t[1] = frame.fallSlot->fallPos;
    frame.rotMatrix.t[2] = frame.fallSlot->posZ;
    frame.scaleX.vz = frame.fallSlot->scale;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    MultiplyMatrix((void *)g_BattleCameraTransform.matrix, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    frame.drawReq = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.drawReq->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleLimitFinishingTouchModelReq1[frame.fallSlot->modelIndex], g_pBattleLimitFinishingTouchTableCursor);
    frame.drawReq->flags = 0;
    frame.drawReq->uvScroll = 0;
    frame.drawReq->fadeLevel = 128;
    frame.drawReq->stateFlags = 0x20;
    frame.drawReq->texturePageBits = 0;
    if (frame.fallSlot->timer >= 14) {
        frame.drawReq->flags |= 8;
        frame.drawReq->fadeLevel = 128 - (frame.fallSlot->timer - 14) * 21;
        frame.drawReq->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleLimitFinishingTouchModelReq2[frame.fallSlot->modelIndex], g_pBattleLimitFinishingTouchTableCursor);
    }
    g_BattleLimitFinishingTouchDrawHandle = BattleFx_SubmitModelDraw(frame.drawReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitFinishingTouchDrawHandle);
    if (frame.fallSlot->timer >= 20) {
        frame.fallSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.fallSlot->fallPos += frame.fallSlot->fallVelocity;
    frame.fallSlot->fallVelocity += (short)(frame.fallSlot->fallVelocity >> 3);
    frame.fallSlot->angleX += frame.fallSlot->angleSpeedX;
    frame.fallSlot->angleZ += frame.fallSlot->angleSpeedZ;
    (frame.fallSlot->timer)++;
}

/*
 * Initializes two 40-byte sprite packets for every cell in a fifteen-by-sixteen grid, with
 * mirrored horizontal texture coordinates, vertical texture gradients, fixed opcode/color and
 * texture fields. Initializes sixteen twenty-byte ring records at zero X/Z, a small Y ramp and
 * radius 600. Runs without a pause check.
 */
// FUNCTION: FF7 0x004bf867
void __cdecl BattleFx_LimitFinishingTouchInitRingPackets(void)
{
    unsigned char vStart;
    int fraction;
    int row;
    int r;
    int packetIdx;
    int column;
    unsigned short clutVal;
    unsigned char uRemaining;
    int cellIndex;
    unsigned char uRight;
    BattleFxFinishingTouchRingPacket *packet;
    unsigned char vB;

    clutVal = 0x3FF0;
    for (row = 0; row < 15; row++) {
        cellIndex = row;
        fraction = (cellIndex << 12) / 16;
        vStart = 0xFE - (fraction * 0xFE >> 12);
        fraction = ((cellIndex + 1) << 12) / 16;
        vB = 0xFE - (fraction * 0xFE >> 12);
        for (column = 0; column < 16; column++) {
            cellIndex = column % 8;
            if (cellIndex < 4) {
                uRemaining = cellIndex << 4;
                uRight = uRemaining + 15;
            } else {
                cellIndex = 8 - cellIndex;
                uRemaining = (cellIndex << 4) - 1;
                uRight = uRemaining - 15;
            }
            for (packetIdx = 0; packetIdx < 2; packetIdx++) {
                packet = &((BattleLimitGaryotenQuadRow *)g_BattleLimitFinishingTouchRingQuadPackets)[row][column][packetIdx];
                packet->tag = 0x9000000;
                packet->command = 0x2E808080;
                packet->u2 = uRemaining;
                packet->u0 = uRemaining;
                packet->u3 = uRight;
                packet->u1 = uRight;
                packet->v1 = vB;
                packet->v0 = vB;
                packet->v3 = vStart;
                packet->v2 = vStart;
                packet->clut = clutVal;
                packet->tpage = 0xAC;
            }
        }
    }
    for (row = 0; row < 16; row++) {
        ((BattleLimitFinishingTouchRing *)g_BattleLimitFinishingTouchRingPackets)[row].posX = 0;
        ((BattleLimitFinishingTouchRing *)g_BattleLimitFinishingTouchRingPackets)[row].height = row * 0xA / 0xF;
        ((BattleLimitFinishingTouchRing *)g_BattleLimitFinishingTouchRingPackets)[row].posZ = 0;
        ((BattleLimitFinishingTouchRingRadius *)g_BattleLimitFinishingTouchRingScales)[row].radius = 600;
    }
}

/*
 * Lazily acquires scratch, generates sixteen-point circles for each of sixteen ring records and
 * alternates two vertex buffers, subtracting 64 from the phase between rings. Projects every
 * connecting quad, drawing only when projection flags exclude 0x60000, with replicated grayLevel
 * gray color and the selected resource state. Alternates the shared packet bank after drawing. Runs
 * while paused.
 */
// FUNCTION: FF7 0x004bfa7e
void __cdecl BattleFx_LimitFinishingTouchDrawRingChain(int phase, int grayLevel)
{
    int idx;
    int scale;
    int packedColor;
    int pointIndex;
    BattleFxFinishingTouchRingPacket *packet;
    int bank;

    if ((*(unsigned char *)&g_BattleLimitFinishingTouchDrawBlockReady & 1) == 0) {
        *(char *)&g_BattleLimitFinishingTouchDrawBlockReady |= 1;
        g_pBattleLimitFinishingTouchDrawBlock = (BattleFxFinishingTouchState *)Gfx_GetPsxScratchpadPtr(0);
    }
    packedColor = grayLevel | grayLevel << 8 | grayLevel << 0x10 | 0x2E000000;
    *(Int64Pair *)g_pBattleLimitFinishingTouchDrawBlock = *(Int64Pair *)g_BattleLimitFinishingTouchRingPackets;
    scale = ((BattleLimitFinishingTouchRingRadius *)g_BattleLimitFinishingTouchRingScales)->radius;
    bank = 0;
    for (idx = 0; idx < 16; idx++) {
        g_pBattleLimitFinishingTouchDrawBlock->ringPoints[bank][idx].vx = g_pBattleLimitFinishingTouchDrawBlock->posX + (scale * SinFixed(((idx << 12) / 16) + phase) >> 12);
        g_pBattleLimitFinishingTouchDrawBlock->ringPoints[bank][idx].vy = g_pBattleLimitFinishingTouchDrawBlock->posY;
        g_pBattleLimitFinishingTouchDrawBlock->ringPoints[bank][idx].vz = g_pBattleLimitFinishingTouchDrawBlock->posZ + (scale * CosFixed(((idx << 12) / 16) + phase) >> 12);
    }
    Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(0, g_pBattleLimitFinishingTouchTableCursor)));
    for (idx = 1; idx < 16; idx++) {
        *(Int64Pair *)g_pBattleLimitFinishingTouchDrawBlock = *(Int64Pair *)&((BattleLimitFinishingTouchRing *)g_BattleLimitFinishingTouchRingPackets)[idx].posX;
        scale = ((BattleLimitFinishingTouchRingRadius *)g_BattleLimitFinishingTouchRingScales)[idx].radius;
        bank ^= 1;
        phase -= 0x40;
        for (pointIndex = 0; pointIndex < 16; pointIndex++) {
            g_pBattleLimitFinishingTouchDrawBlock->ringPoints[bank][pointIndex].vx = g_pBattleLimitFinishingTouchDrawBlock->posX + (scale * SinFixed(((pointIndex << 12) / 16) + phase) >> 12);
            g_pBattleLimitFinishingTouchDrawBlock->ringPoints[bank][pointIndex].vy = g_pBattleLimitFinishingTouchDrawBlock->posY;
            g_pBattleLimitFinishingTouchDrawBlock->ringPoints[bank][pointIndex].vz = g_pBattleLimitFinishingTouchDrawBlock->posZ + (scale * CosFixed(((pointIndex << 12) / 16) + phase) >> 12);
        }
        for (pointIndex = 0; pointIndex < 16; pointIndex++) {
            packet = &((BattleLimitGaryotenQuadRow *)g_BattleLimitFinishingTouchRingQuadPackets)[idx - 1][pointIndex][g_BattleLimitFinishingTouchPacketBank];
            g_pBattleLimitFinishingTouchDrawBlock->quadDepth = Gfx_TransformQuadReturnDepth(&g_pBattleLimitFinishingTouchDrawBlock->ringPoints[bank][pointIndex], &g_pBattleLimitFinishingTouchDrawBlock->ringPoints[bank][(pointIndex + 1) % 16], &g_pBattleLimitFinishingTouchDrawBlock->ringPoints[bank ^ 1][pointIndex], &g_pBattleLimitFinishingTouchDrawBlock->ringPoints[bank ^ 1][(pointIndex + 1) % 16], &g_pBattleLimitFinishingTouchDrawBlock->unused114, &g_pBattleLimitFinishingTouchDrawBlock->flags);
            if ((g_pBattleLimitFinishingTouchDrawBlock->flags & 0x60000) == 0) {
                packet->command = packedColor;
                Gfx_DrawPsxPrimitive(packet);
            }
        }
    }
    g_BattleLimitFinishingTouchPacketBank ^= 1;
}

/*
 * Updates sixteen ring records in reverse order, propagating three random-jitter angle shorts from
 * their predecessors and generating a new random-walk seed for ring zero. Builds a small-angle
 * rotation, transforms an xScale-scaled X offset into ring position, derives Y from height and
 * computes radius from radius/falloff through nested sine weighting. Runs without a pause check.
 */
// FUNCTION: FF7 0x004bfe3c
void __cdecl BattleFx_LimitFinishingTouchUpdateRingChain(int height, int radius, int xScale, int falloff)
{
    int ring;
    int factor;
    int iteration;
    int ratio;

    if ((g_BattleLimitFinishingTouchRingScratchReady & 1) == 0) {
        g_BattleLimitFinishingTouchRingScratchReady |= 1;
        g_pBattleLimitFinishingTouchRingScratch = (BattleLimitFinishingTouchRingScratch *)Gfx_GetPsxScratchpadPtr(0);
    }
    for (ring = 0xF; ring >= 0; ring--) {
        ratio = (ring << 12) / 15;
        if (ring > 0) {
            ((BattleLimitFinishingTouchRingAngles *)g_BattleLimitFinishingTouchRingAngles)[ring].angleX = ((BattleLimitFinishingTouchRingAngles *)g_BattleLimitFinishingTouchRingAngles)[ring - 1].angleX + (rand() % 0x20) + -0x10;
            ((BattleLimitFinishingTouchRingAngles *)g_BattleLimitFinishingTouchRingAngles)[ring].angleY = ((BattleLimitFinishingTouchRingAngles *)g_BattleLimitFinishingTouchRingAngles)[ring - 1].angleY + (rand() % 0x20) + -0x10;
            ((BattleLimitFinishingTouchRingAngles *)g_BattleLimitFinishingTouchRingAngles)[ring].angleZ = ((BattleLimitFinishingTouchRingAngles *)g_BattleLimitFinishingTouchRingAngles)[ring - 1].angleZ + (rand() % 0x20) + -0x10;
        } else {
            ((BattleLimitFinishingTouchRingAngles *)g_BattleLimitFinishingTouchRingAngles)[ring].angleX = (g_BattleLimitFinishingTouchSwayState.ringPhase);
            ((BattleLimitFinishingTouchRingAngles *)g_BattleLimitFinishingTouchRingAngles)[ring].angleY = g_BattleLimitFinishingTouchSwayState.angle;
            ((BattleLimitFinishingTouchRingAngles *)g_BattleLimitFinishingTouchRingAngles)[ring].angleZ = g_BattleLimitFinishingTouchSwayAngleZ;
            (g_BattleLimitFinishingTouchSwayWalk.randomWalk) += (short)(rand() % 256 - 128);
            g_BattleLimitFinishingTouchSwayWalk.angle += (short)(rand() % 0x100 - 0x80);
            g_BattleLimitFinishingTouchSwayWalkZ += (short)(rand() % 256 - 128);
            (g_BattleLimitFinishingTouchSwayState.ringPhase) += (short)(SinFixed((g_BattleLimitFinishingTouchSwayWalk.randomWalk)) << 9 >> 12);
            g_BattleLimitFinishingTouchSwayState.angle += (short)(SinFixed(g_BattleLimitFinishingTouchSwayWalk.angle) << 9 >> 12);
            g_BattleLimitFinishingTouchSwayAngleZ += (short)(SinFixed(g_BattleLimitFinishingTouchSwayWalkZ) << 9 >> 12);
        }
        g_pBattleLimitFinishingTouchRingScratch->vector.vx = SinFixed(((BattleLimitFinishingTouchRingAngles *)g_BattleLimitFinishingTouchRingAngles)[ring].angleX) << 9 >> 12;
        g_pBattleLimitFinishingTouchRingScratch->vector.vy = SinFixed(((BattleLimitFinishingTouchRingAngles *)g_BattleLimitFinishingTouchRingAngles)[ring].angleY) << 9 >> 12;
        g_pBattleLimitFinishingTouchRingScratch->vector.vz = SinFixed(((BattleLimitFinishingTouchRingAngles *)g_BattleLimitFinishingTouchRingAngles)[ring].angleZ) << 9 >> 12;
        RotMatrix(&g_pBattleLimitFinishingTouchRingScratch->vector, &g_pBattleLimitFinishingTouchRingScratch->rotation);
        g_pBattleLimitFinishingTouchRingScratch->vector.vx = xScale * ((ratio * 0xE00 /* 0.875 */ >> 12) + 512) >> 12;
        g_pBattleLimitFinishingTouchRingScratch->vector.vy = 0;
        g_pBattleLimitFinishingTouchRingScratch->vector.vz = 0;
        TransformVectorToInt16(&g_pBattleLimitFinishingTouchRingScratch->rotation, &g_pBattleLimitFinishingTouchRingScratch->vector, &g_pBattleLimitFinishingTouchRingScratch->vector.vx);
        ((BattleLimitFinishingTouchRing *)g_BattleLimitFinishingTouchRingPackets)[ring].posX = g_pBattleLimitFinishingTouchRingScratch->vector.vx;
        ((BattleLimitFinishingTouchRing *)g_BattleLimitFinishingTouchRingPackets)[ring].height = -(height * ratio >> 12) + (g_pBattleLimitFinishingTouchRingScratch->vector.vy >> 4);
        ((BattleLimitFinishingTouchRing *)g_BattleLimitFinishingTouchRingPackets)[ring].posZ = g_pBattleLimitFinishingTouchRingScratch->vector.vz;
        factor = SinFixed(ratio << 11 >> 12);
        for (iteration = 0; iteration < 1; iteration++) {
            factor = SinFixed(factor / 4);
        }
        ((BattleLimitFinishingTouchRingRadius *)g_BattleLimitFinishingTouchRingScales)[ring].radius = radius * (4096 - ((4096 - falloff) * factor >> 12)) >> 12;
    }
}

/*
 * Obtains the selected actor attachment matrix, subtracts global translation from its position and
 * rotates that difference through the transposed global matrix. Composes the transposed global and
 * attachment matrices, transforms srcVec through the result into outVec and adds the rotated
 * difference to its three shorts.
 */
// FUNCTION: FF7 0x004c021f
void __cdecl BattleFx_LimitFinishingTouchTransformAttachPointToEffectSpace(int actorId, int attachIndex, short *srcVec, short *outVec)
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

    Battle3d_GetActorAttachMatrix(actorId, attachIndex, &frame.attach);
    frame.direction[0] = frame.attach.t[0] - (g_BattleCameraTransform.translation[0]);
    frame.direction[1] = frame.attach.t[1] - (g_BattleCameraTransform.translation[1]);
    frame.direction[2] = frame.attach.t[2] - (g_BattleCameraTransform.translation[2]);
    TransposeMatrix((void *)g_BattleCameraTransform.matrix, &frame.transpose);
    TransformVectorToInt16(&frame.transpose, (SVECTOR *)frame.direction, frame.direction);
    ConcatMatrixPreservingTranslation(&frame.transpose, &frame.attach, &frame.combined);
    TransformVectorToInt16(&frame.combined, (SVECTOR *)srcVec, outVec);
    outVec[0] += frame.direction[0];
    outVec[1] += frame.direction[1];
    outVec[2] += frame.direction[2];
}

/*
 * Draws every active record in the 140-entry sprite pool before checking pause; unpaused calls
 * move X/Z, damp velocities and retire records after their age reaches seven. Unpaused task ages
 * below 65 emit up to nine radial sprites using shared position/radius, task-age speed and
 * randomized size; at 75 retire the task and then advance age. Pool capacity stops emission.
 */
// FUNCTION: FF7 0x004c02e5
void __cdecl BattleFx_LimitFinishingTouchRadialSpriteEmitterTask(void)
{
    int idx;
    int q;
    int theta;
    int sizeBase;
    int freeSlot;
    int dist;
    BattleTaskSlot *task;

    task = (BattleTaskSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    g_BattleLimitFinishingTouchSpriteReqPool.radialSprite.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_pBattleLimitFinishingTouchTableCursor);
    g_BattleLimitFinishingTouchSpriteReqPool.radialSprite.drawObj = Gfx_ModelTableGetSpriteObject(1, g_pBattleLimitFinishingTouchTableCursor);
    for (idx = 0; idx < 140; idx++) {
        if (g_BattleLimitFinishingTouchRadialSprites[idx].active != 0) {
            g_BattleLimitFinishingTouchSpriteReqPool.radialSprite.frame = g_BattleLimitFinishingTouchRadialSprites[idx].age << 1;
            BattleFx_InstallBillboardMatrix(&g_BattleLimitFinishingTouchRadialSprites[idx].posX, (short)g_BattleLimitFinishingTouchRadialSprites[idx].scale, -(g_BattleLimitFinishingTouchRadialSprites[idx].scale >> 3));
            g_BattleLimitFinishingTouchDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleLimitFinishingTouchSpriteReqPool.radialSprite, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitFinishingTouchDrawHandle);
            if (g_BattleIsPaused == 0) {
                g_BattleLimitFinishingTouchRadialSprites[idx].posX += g_BattleLimitFinishingTouchRadialSprites[idx].velocityX;
                g_BattleLimitFinishingTouchRadialSprites[idx].posZ += g_BattleLimitFinishingTouchRadialSprites[idx].velocityZ;
                g_BattleLimitFinishingTouchRadialSprites[idx].velocityX -= (short)(g_BattleLimitFinishingTouchRadialSprites[idx].velocityX >> 3);
                g_BattleLimitFinishingTouchRadialSprites[idx].velocityZ -= (short)(g_BattleLimitFinishingTouchRadialSprites[idx].velocityZ >> 3);
                if (g_BattleLimitFinishingTouchRadialSprites[idx].age >= 7) {
                    g_BattleLimitFinishingTouchRadialSprites[idx].active = 0;
                }
                (g_BattleLimitFinishingTouchRadialSprites[idx].age)++;
            }
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter >= 75) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (task->frameCounter < 65) {
        sizeBase = (g_BattleLimitFinishingTouchRadialBurst.radius / 3 << 12) / 256;
        for (idx = 0; idx < 9; idx++) {
            for (freeSlot = 0; freeSlot < 140 && g_BattleLimitFinishingTouchRadialSprites[freeSlot].active != 0; freeSlot++) {
            }
            if (freeSlot >= 140) {
                break;
            }
            g_BattleLimitFinishingTouchRadialSprites[freeSlot].active = 1;
            g_BattleLimitFinishingTouchRadialSprites[freeSlot].age = 0;
            theta = ((rand()) % 4096);
            dist = g_BattleLimitFinishingTouchRadialBurst.radius + rand() % (g_BattleLimitFinishingTouchRadialBurst.radius >> 2);
            g_BattleLimitFinishingTouchRadialSprites[freeSlot].posX = g_BattleLimitFinishingTouchRadialBurst.posX + (CosFixed(theta) * dist >> 12);
            g_BattleLimitFinishingTouchRadialSprites[freeSlot].posY = 0;
            g_BattleLimitFinishingTouchRadialSprites[freeSlot].posZ = g_BattleLimitFinishingTouchRadialBurst.posZ - (SinFixed(theta) * dist >> 12);
            dist = rand() % 50 + task->frameCounter * 8 + 60;
            g_BattleLimitFinishingTouchRadialSprites[freeSlot].velocityX = CosFixed(theta) * dist >> 12;
            g_BattleLimitFinishingTouchRadialSprites[freeSlot].velocityZ = -(SinFixed(theta) * dist >> 12);
            theta = rand() % 0xA00 + task->frameCounter * 8 + 0x800;
            g_BattleLimitFinishingTouchRadialSprites[freeSlot].scale = sizeBase * theta >> 12;
        }
    }
    (task->frameCounter)++;
}

/*
 * When unpaused, first advances the task delay and sets actor/peer flags on its final step.
 * Afterwards cancellation retires, the shared hold flag returns, and a positive cached actor state
 * drives an accelerating displacement then return motion copied to peers. At age 45 restores
 * actor/peer positions and selected flags, clears actor sprite records, starts a radial callback
 * and sound when enabled, then retires and advances age.
 */
// FUNCTION: FF7 0x004c06d8
void __cdecl BattleFx_LimitFinishingTouchActorLiftReturnTask(void)
{
    int actorIdx;
    int peerIdx;
    int peerActor;
    BattleFxFinishingTouchBurst *pBurst;
    BattleFxFinishingTouchLift *task;

    if (g_BattleIsPaused != 0) {
        return;
    }
    task = (BattleFxFinishingTouchLift *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    actorIdx = task->actorIndex;
    if (task->stepCount < task->stepLimit) {
        (task->stepCount)++;
        if (task->stepCount == task->stepLimit && ((BattleLimitFinishingTouchPeerGroup *)&g_BattleLimitFinishingTouchPeerGroupHead)[actorIdx].timelineId > 0) {
            g_BattleActorRecords[actorIdx].drawFlags |= 0x11;
            for (peerIdx = 0; peerIdx < ((BattleLimitFinishingTouchPeerGroup *)&g_BattleLimitFinishingTouchPeerGroupHead)[actorIdx].peerCount; peerIdx++) {
                peerActor = ((BattleLimitFinishingTouchPeerList *)&g_BattleLimitFinishingTouchPeerList)[actorIdx].peers[peerIdx];
                g_BattleActorRecords[peerActor].drawFlags |= 0x11;
            }
        }
    } else if ((g_BattleLimitFinishingTouchEventFlags & GARYOTEN_EVENT_CANCEL) != 0) {
        task->ownerSlot = BATTLE_TASK_FREE;
    } else {
        if ((g_BattleLimitFinishingTouchEventFlags & GARYOTEN_EVENT_HOLD) != 0) {
            return;
        }
        if (((BattleLimitFinishingTouchPeerGroup *)&g_BattleLimitFinishingTouchPeerGroupHead)[actorIdx].timelineId > 0) {
            if (task->phase < 35 && g_BattleActorRecords[actorIdx].posY > -10000) {
                g_BattleActorRecords[actorIdx].posY += task->liftVelocity;
                task->liftVelocity += (short)(task->liftVelocity / 6);
                g_BattleActorRecords[actorIdx].yaw += task->yawVelocity;
                task->yawVelocity += task->yawAccel;
                task->yawAccel += (short)(task->yawAccel >> 4);
            } else if (task->phase >= 35 && task->returnBlocked == 0) {
                if (task->phase == 0x23) {
                    task->liftVelocity = g_BattleActorRecords[actorIdx].posY;
                    task->liftDecay = task->liftVelocity / 10;
                }
                task->liftVelocity -= task->liftDecay;
                g_BattleActorRecords[actorIdx].posY = task->liftVelocity + task->baseY;
                g_BattleActorRecords[actorIdx].rotZ = 0x800 /* 180 deg */;
                g_BattleActorRecords[actorIdx].yaw += task->yawVelocity;
                task->yawVelocity += task->yawAccel;
                task->yawAccel -= (short)(task->yawAccel >> 3);
            }
            for (peerIdx = 0; peerIdx < ((BattleLimitFinishingTouchPeerGroup *)&g_BattleLimitFinishingTouchPeerGroupHead)[actorIdx].peerCount; peerIdx++) {
                peerActor = ((BattleLimitFinishingTouchPeerList *)&g_BattleLimitFinishingTouchPeerList)[actorIdx].peers[peerIdx];
                *(Int64Pair *)&g_BattleActorRecords[peerActor].posX = *(Int64Pair *)&g_BattleActorRecords[actorIdx].posX;
                *(Int64Pair *)&g_BattleActorRecords[peerActor].rotX = *(Int64Pair *)&g_BattleActorRecords[actorIdx].rotX;
            }
        }
        if (task->phase >= 45) {
            if (task->returnBlocked == 0 && ((BattleLimitFinishingTouchPeerGroup *)&g_BattleLimitFinishingTouchPeerGroupHead)[actorIdx].timelineId > 0) {
                pBurst = (BattleFxFinishingTouchBurst *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitFinishingTouchActorRadialSpriteBurstTask);
                *(Int64Pair *)&pBurst->centerX = *(Int64Pair *)&g_BattleLimitFinishingTouchLiftStates[actorIdx];
                pBurst->centerY = 0;
                pBurst->speedBase = 0x5A;
                pBurst->speedRange = 70;
                pBurst->radiusBase = g_BattleActorRecords[actorIdx].radius / 3 << 1;
                pBurst->radiusRange = pBurst->radiusBase >> 3;
                pBurst->spawnLimit = 2;
                pBurst->spriteCount = 10;
                pBurst->baseScale = pBurst->radiusBase + 0xE0;
                if (pBurst->baseScale > 0x4F2) {
                    pBurst->baseScale = 0x4F2;
                }
                pBurst->stateIndex = actorIdx;
                for (peerIdx = 0; peerIdx < 48; peerIdx++) {
                    ((BattleLimitGaryotenBurstPool *)g_BattleLimitFinishingTouchParticleBuf)[actorIdx][peerIdx].active = 0;
                }
                g_BattleActorRecords[actorIdx].posY = g_BattleLimitFinishingTouchLiftStates[actorIdx].posY;
                *(Int64Pair *)&g_BattleActorRecords[actorIdx].rotX = g_BattleLimitFinishingTouchSavedRotationTable[actorIdx];
                if ((g_BattleLimitFinishingTouchSavedDrawFlags[actorIdx] & 0x10) == 0) {
                    g_BattleActorRecords[actorIdx].drawFlags &= 0xEF;
                }
                if ((g_BattleLimitFinishingTouchSavedDrawFlags[actorIdx] & 1) == 0) {
                    g_BattleActorRecords[actorIdx].drawFlags &= 0xFE;
                }
                for (peerIdx = 0; peerIdx < ((BattleLimitFinishingTouchPeerGroup *)&g_BattleLimitFinishingTouchPeerGroupHead)[actorIdx].peerCount; peerIdx++) {
                    peerActor = ((BattleLimitFinishingTouchPeerList *)&g_BattleLimitFinishingTouchPeerList)[actorIdx].peers[peerIdx];
                    g_BattleActorRecords[peerActor].posY = g_BattleLimitFinishingTouchLiftStates[peerActor].posY;
                    *(Int64Pair *)&g_BattleActorRecords[peerActor].rotX = g_BattleLimitFinishingTouchSavedRotationTable[peerActor];
                    if ((g_BattleLimitFinishingTouchSavedDrawFlags[peerActor] & 0x10) == 0) {
                        g_BattleActorRecords[peerActor].drawFlags &= 0xEF;
                    }
                    if ((g_BattleLimitFinishingTouchSavedDrawFlags[peerActor] & 1) == 0) {
                        g_BattleActorRecords[peerActor].drawFlags &= 0xFE;
                    }
                }
                Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&pBurst->centerX), 0xA6);
            }
            task->ownerSlot = BATTLE_TASK_FREE;
        }
        (task->phase)++;
    }
}

/*
 * Draws the actor-indexed 48-entry sprite pool even while paused; unpaused calls move/damp X/Z and
 * retire sprite records from age fifteen. Before the task emission threshold creates radial
 * sprites using its position, randomized radius/speed and scaled size, stopping when the pool is
 * full. Unpaused task age twenty retires, then age advances.
 */
// FUNCTION: FF7 0x004c0d63
void __cdecl BattleFx_LimitFinishingTouchActorRadialSpriteBurstTask(void)
{
    int idx;
    int theta;
    int sizeBase;
    int freeSlot;
    int r;
    BattleFxFinishingTouchBurstParticle *pParticlePool;
    int dist;
    BattleFxFinishingTouchBurst *task;

    task = (BattleFxFinishingTouchBurst *)BATTLE_CURRENT_TASK_SLOT;
    pParticlePool = ((BattleLimitGaryotenBurstPool *)g_BattleLimitFinishingTouchParticleBuf)[task->stateIndex];
    g_BattleLimitFinishingTouchSpriteReqPool.burstSprite.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_pBattleLimitFinishingTouchTableCursor);
    g_BattleLimitFinishingTouchSpriteReqPool.burstSprite.drawObj = Gfx_ModelTableGetSpriteObject(1, g_pBattleLimitFinishingTouchTableCursor);
    for (idx = 0; idx < 48; idx++) {
        if (pParticlePool[idx].active != 0) {
            g_BattleLimitFinishingTouchSpriteReqPool.burstSprite.frame = pParticlePool[idx].age;
            BattleFx_InstallBillboardMatrix(&pParticlePool[idx].posX, (short)pParticlePool[idx].scale, -(pParticlePool[idx].scale >> 3));
            g_BattleLimitFinishingTouchDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleLimitFinishingTouchSpriteReqPool.burstSprite, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitFinishingTouchDrawHandle);
            if (g_BattleIsPaused == 0) {
                pParticlePool[idx].posX += pParticlePool[idx].velocityX;
                pParticlePool[idx].posZ += pParticlePool[idx].velocityZ;
                pParticlePool[idx].velocityX -= (short)(pParticlePool[idx].velocityX >> 3);
                pParticlePool[idx].velocityZ -= (short)(pParticlePool[idx].velocityZ >> 3);
                if (pParticlePool[idx].age >= 15) {
                    pParticlePool[idx].active = 0;
                }
                (pParticlePool[idx].age)++;
            }
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frame >= 20) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (task->frame < task->spawnLimit) {
        sizeBase = (task->baseScale << 12) / 256;
        for (idx = 0; idx < task->spriteCount; idx++) {
            for (freeSlot = 0; freeSlot < 48 && pParticlePool[freeSlot].active != 0; freeSlot++) {
            }
            if (freeSlot >= 48) {
                break;
            }
            pParticlePool[freeSlot].active = 1;
            pParticlePool[freeSlot].age = 0;
            theta = ((rand()) % 4096);
            dist = task->radiusBase + rand() % task->radiusRange;
            pParticlePool[freeSlot].posX = task->centerX + (CosFixed(theta) * dist >> 12);
            pParticlePool[freeSlot].posY = task->centerY;
            pParticlePool[freeSlot].posZ = task->centerZ - (SinFixed(theta) * dist >> 12);
            dist = task->speedBase + rand() % task->speedRange + (task->radiusBase >> 4);
            pParticlePool[freeSlot].velocityX = CosFixed(theta) * dist >> 12;
            pParticlePool[freeSlot].velocityZ = -(SinFixed(theta) * dist >> 12);
            theta = rand() % 0xB00 + 0x600;
            pParticlePool[freeSlot].scale = sizeBase * theta >> 12;
        }
    }
    (task->frame)++;
}

/*
 * Builds a zero-angle uniformly scaled matrix using the task scale times an age-indexed table
 * entry, pushes the task point toward the view and submits resource entry two with descriptor 0x88
 * and zero rotation fields. Drawing and retirement from age three precede the pause return; only
 * unpaused calls advance age.
 */
// FUNCTION: FF7 0x004c1185
void __cdecl BattleFx_LimitFinishingTouchScaledModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        PsxMatrix matrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxFinishingTouchScaledModel *scaledModel; };
        struct { unsigned char pad_to_24[0x24]; SVECTOR rotX; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_30[0x30]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.scaledModel = (BattleFxFinishingTouchScaledModel *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.matrix);
    frame.scaleX.vz = frame.scaledModel->baseScale * g_BattleLimitFinishingTouchScaleTable[frame.scaledModel->stage] >> 12;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.matrix, &frame.scaleX);
    BattleFx_MakeViewSpacePointPushed(&frame.scaledModel->posX, -(frame.scaledModel->baseScale >> 3), (Vec3sPacked *)&frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(2, g_pBattleLimitFinishingTouchTableCursor);
    frame.modelDraw->flags = 0x88;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    g_BattleLimitFinishingTouchDrawHandle = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitFinishingTouchDrawHandle);
    if (frame.scaledModel->stage >= 3) {
        frame.scaledModel->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.scaledModel->stage)++;
}

/*
 * Builds a stored-Z-angle uniformly scaled matrix, resolves an actor-relative offset of minus 555
 * on Z into task position, pushes that point toward the view and submits resource entry three with
 * descriptor 0x88. Drawing and retirement from age twelve precede the pause return. Unpaused calls
 * advance Z rotation, grow scale below five, shrink it from six and advance age.
 */
// FUNCTION: FF7 0x004c12dc
void __cdecl BattleFx_LimitFinishingTouchGrowShrinkModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        PsxMatrix matrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxFinishingTouchGrowShrink *growShrinkRecord; };
        struct { unsigned char pad_to_24[0x24]; SVECTOR rotX; };
        struct { unsigned char pad_to_2C[0x2C]; short offsetX; };
        struct { unsigned char pad_to_2E[0x2E]; short offsetY; };
        struct { unsigned char pad_to_30[0x30]; short offsetZ; };
        struct { unsigned char pad_to_34[0x34]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_38[0x38]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.growShrinkRecord = (BattleFxFinishingTouchGrowShrink *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = frame.growShrinkRecord->growth;
    RotMatrixXYZ(&frame.rotX, &frame.matrix);
    frame.scaleX.vz = frame.growShrinkRecord->scale;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.matrix, &frame.scaleX);
    frame.offsetX = 0;
    frame.offsetY = 0;
    frame.offsetZ = 0xFDD5;
    BattleFx_LimitFinishingTouchTransformAttachPointToEffectSpace(g_BattleLimitFinishingTouchActorIndex, -1, &frame.offsetX, &frame.growShrinkRecord->posX);
    BattleFx_MakeViewSpacePointPushed(&frame.growShrinkRecord->posX, -(frame.growShrinkRecord->scale >> 3), (Vec3sPacked *)&frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(3, g_pBattleLimitFinishingTouchTableCursor);
    frame.modelDraw->flags = 0x88;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    g_BattleLimitFinishingTouchDrawHandle = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitFinishingTouchDrawHandle);
    if (frame.growShrinkRecord->step >= 12) {
        frame.growShrinkRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.growShrinkRecord->growth += frame.growShrinkRecord->growthRate;
    if (frame.growShrinkRecord->step < 5) {
        frame.growShrinkRecord->scale += frame.growShrinkRecord->scaleRate;
    }
    if (frame.growShrinkRecord->step >= 6) {
        frame.growShrinkRecord->scale -= frame.growShrinkRecord->scaleDecay;
    }
    (frame.growShrinkRecord->step)++;
}

/*
 * One frame of an expiry task gated on a mask: it does nothing unless g_BattleLimitFinishingTouchEventFlags
 * and the task's actorMask share a bit. While they do, it counts up the timer and frees the task
 * once the timer passes 5. It draws nothing.
 */
// FUNCTION: FF7 0x004c14ab
void __cdecl BattleFx_LimitFinishingTouchMaskGatedExpiryTask(void)
{
    BattleFxFinishingTouchMaskTask *task;

    task = (BattleFxFinishingTouchMaskTask *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleLimitFinishingTouchEventFlags & task->actorMask) != 0) {
        (task->timer)++;
        if (task->timer > 5) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Points g_BattleLimitFinishingTouchDrawHandle at g_BattleLimitFinishingTouchDrawListHead, plus 0xE000 on odd
 * draw frames, and writes -1 into the task slot g_BattleLimitFinishingTouchCursorTaskSlot once
 * GARYOTEN_EVENT_DONE (the sign bit of g_BattleLimitFinishingTouchEventFlags) is set.
 */
// FUNCTION: FF7 0x004c14fd
void __cdecl BattleFx_LimitFinishingTouchSelectDrawCursorTask(void)
{
    int val;

    if (g_BattleDrawFrameParity == 0) {
        val = g_BattleLimitFinishingTouchDrawListHead;
    } else {
        val = g_BattleLimitFinishingTouchDrawListHead + 0xE000;
    }
    g_BattleLimitFinishingTouchDrawHandle = val;
    if (g_BattleLimitFinishingTouchEventFlags < 0) {
        *(int *)g_BattleLimitFinishingTouchCursorTaskSlot = 0xFFFFFFFF;
    }
}

/*
 * Allocates BattleFx_LimitFinishingTouchMaskGatedExpiryTask with actorMask GARYOTEN_EVENT_RELEASE_SECOND,
 * clears GARYOTEN_EVENT_HOLD, sets GARYOTEN_EVENT_CANCEL when targetMask is the source actor
 * alone, and stores both arguments.
 */
// FUNCTION: FF7 0x004c1544
void __cdecl BattleFx_LimitBeginFinishingTouchAlt(int targetMask, int actorIdx)
{
    BattleFxFinishingTouchMaskTask *task;

    task = (BattleFxFinishingTouchMaskTask *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitFinishingTouchMaskGatedExpiryTask);
    task->actorMask = GARYOTEN_EVENT_RELEASE_SECOND;
    g_BattleLimitFinishingTouchEventFlags &= ~GARYOTEN_EVENT_HOLD;
    if (targetMask == 1 << actorIdx) {
        g_BattleLimitFinishingTouchEventFlags |= GARYOTEN_EVENT_CANCEL;
    }
    g_BattleLimitFinishingTouchTargetMask = targetMask;
    g_BattleLimitFinishingTouchActorIndex = actorIdx;
}
} /* extern "C" */
