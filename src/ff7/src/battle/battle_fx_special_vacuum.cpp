// src/ff7/src/battle/battle_fx_special_vacuum.cpp
/* Enemy Special Effect [idx 27]: Vacuum (JP: bakyu / バキューム (Vacuum), asset: "SPECIAL/BAKYU") — Vacuum (0x1e3, Bloatfloat) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Bakyu attack: loads resources, spawns per-target effects,
//   and drives decelerating trail segments from a segment record pool, along with deceleration
//   sprites and a jittering model.

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

struct BattleFxVacuumSegment;
struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x008a8928
int g_BattleSpecVacuumResourceDef = 0x001900B1;
char *g_BattleSpecVacuumModelNames[1] = {
    // STRING: FF7 0x008a8980
    "BAKYUU1.RSD"
};
unsigned int g_BattleSpecVacuumSpriteDefs[8] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a898c
    (unsigned int)"TAMA.TIM",
    // STRING: FF7 0x008a8998
    (unsigned int)"TAMA.SPT",
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x008a8950
BattleFxSpriteRequest g_BattleSpecVacuumSpriteReq = {0, 0, (char)0x80, (char)0x80, (char)0x80, 0x2e, 0, 0};
// GLOBAL: FF7 0x008a8960
struct BattleSpecVacuumModelReq g_BattleSpecVacuumModelReq = {0x7000, 0x7000, 0x7000, {0}, {0, 8, 0, 128, 0x20, 0}};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b72fa0
struct BattleSpecVacuumSegmentRecords g_BattleSpecVacuumSegmentRecords = {0};
unsigned char g_bssPad_00b73f60[8] = {0};
// GLOBAL: FF7 0x00b73f68
short g_BattleSpecVacuumAngle = 0;
// GLOBAL: FF7 0x00b73f6c
int g_BattleSpecVacuumPacketCursor = 0;
// GLOBAL: FF7 0x00b73f70
struct BattleSpecVacuumRotVector g_BattleSpecVacuumRotVector = {0};
// GLOBAL: FF7 0x00b73f74
short g_BattleSpecVacuumBasePitch = 0;
// GLOBAL: FF7 0x00b73f78
short g_BattleSpecVacuumModelStep = 0;
unsigned char g_bssPad_00b73f7c[4] = {0};
// GLOBAL: FF7 0x00b73f80
struct BattleFxTimeDamageWarpPoint g_BattleSpecVacuumPointOffset = {0};
// GLOBAL: FF7 0x00b73f84
short g_BattleSpecVacuumDistance = 0;
// GLOBAL: FF7 0x00b73f88
struct GfxTableCursor *g_BattleSpecVacuumLoadedRes = 0;
// clang-format on

/*
 * Loads the Bakyu model table once.
 */
// FUNCTION: FF7 0x0055eae0
void __cdecl BattleFx_SpecialLoadResVacuum(void)
{
    GfxModelTableBank *ownerHandle;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecVacuumLoadedRes == 0) {
        kind = 2;
        resourceDefsWord = (int)&g_BattleSpecVacuumResourceDef;
        ownerHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a89a4
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/BAKYU", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecVacuumLoadedRes, ownerHandle);
    }
}

/*
 * Loads the Bakyu resources, resets the packet cursor and the model step, and starts the effect.
 */
// FUNCTION: FF7 0x0055eb70
void __cdecl BattleFx_SpecialEnterVacuum(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResVacuum();
    g_BattleSpecVacuumPacketCursor = (int)&g_GfxPsxPacketScratch;
    g_BattleSpecVacuumModelStep = 0;
    BattleFx_SpecialBeginVacuum(targetMask, actorIdx);
}

/*
 * Starts the per-target spawner task, clears the segment pool, plays the sound and ramps the fixed-point effect value.
 */
// FUNCTION: FF7 0x0055eb9d
void __cdecl BattleFx_SpecialBeginVacuum(int targetMask, int actorIdx)
{
    short i;

    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnVacuumPerTarget);
    for (i = 0; i < 200; i++) {
        g_BattleSpecVacuumSegmentRecords.segments[i].active = 0;
    }
    Battle_PlaySfxPannedToActor(0x61, (short)actorIdx, 0x40);
    g_BattleFxTimelineState.channel2 = 0;
    g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
    g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    Battle_StartFixedPointRamp(0xC00, 8);
}

/*
 * Creates the timeline task for one target: it starts at the caster's bone and aims at the target's bone.
 */
// FUNCTION: FF7 0x0055ec2a
void __cdecl BattleFx_SpecialSpawnVacuumPerTarget(int actor, int casterIndex)
{
    BattleFxVacuumProjectile *taskRecord;

    taskRecord = (BattleFxVacuumProjectile *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialVacuumTimelineTask);
    Battle_GetBoneWorldPos(casterIndex, g_BattleActorRecords[casterIndex].bone33, (short *)&taskRecord->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)&taskRecord->targetX);
    taskRecord->actorIndex = actor;
}

/*
 * Timeline task: draws the live segments, spawns the jitter model at step 0, then the decelerating
 * sprites and segment spawners until step 0x18, and frees itself when the segments have faded.
 */
// FUNCTION: FF7 0x0055eca0
void __cdecl BattleFx_SpecialVacuumTimelineTask(void)
{
    BattleFxVacuumProjectile *pCurrent;
    short sum, spriteIndex;
    /* The unused local keeps the frame layout of the original. */
    int s;
    BattleFxVacuumChildSlot *child;
    int zOut;

    pCurrent = (BattleFxVacuumProjectile *)BATTLE_CURRENT_TASK_SLOT;
    sum = BattleFx_SpecialVacuumDrawLiveSegmentRecords();
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (pCurrent->step == 0) {
        g_BattleSpecVacuumRotVector.yaw = ArcTangentFixed(pCurrent->targetX - pCurrent->startX, pCurrent->targetZ - pCurrent->startZ);
        g_BattleSpecVacuumBasePitch = 0;
        (g_BattleSpecVacuumRotVector.pitch) = g_BattleSpecVacuumBasePitch;
        RotMatrix((SVECTOR *)&g_BattleSpecVacuumRotVector, &g_BattleSpecVacuumSegmentRecords.rotMatrixA);
        g_BattleSpecVacuumSegmentRecords.historyC = 0;
        g_BattleSpecVacuumSegmentRecords.historyB = g_BattleSpecVacuumSegmentRecords.historyC;
        g_BattleSpecVacuumSegmentRecords.historyA = g_BattleSpecVacuumSegmentRecords.historyB;
        SetRotMatrix(&g_BattleSpecVacuumSegmentRecords.rotMatrixA);
        SetTransMatrix(&g_BattleSpecVacuumSegmentRecords.rotMatrixA);
        (g_BattleSpecVacuumPointOffset.offsetX) = 0;
        g_BattleSpecVacuumPointOffset.offsetY = 0;
        g_BattleSpecVacuumDistance = 2400;
        RotTransPointToInt16((short *)&g_BattleSpecVacuumPointOffset, (short *)&g_BattleSpecVacuumPointOffset, &zOut);
        child = (BattleFxVacuumChildSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialVacuumJitterModelTask);
        child->posX = pCurrent->startX + (g_BattleSpecVacuumPointOffset.offsetX);
        child->posY = pCurrent->posY + g_BattleSpecVacuumPointOffset.offsetY;
        child->posZ = pCurrent->startZ + g_BattleSpecVacuumDistance;
        child->yaw = g_BattleSpecVacuumRotVector.yaw + 0x800;
        child->roll = 0;
        child->pitch = 0;
    }
    if (pCurrent->step > 0 && pCurrent->step < 24) {
        child = (BattleFxVacuumChildSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialVacuumDecelSegmentSpawnerTask);
        g_BattleSpecVacuumAngle = rand() % 0x1000;
        child->velocityX = CosFixed(g_BattleSpecVacuumAngle) * 0xBB8 >> 12;
        child->velocityZ = SinFixed(g_BattleSpecVacuumAngle) * 0xBB8 >> 12;
        child->velocityY = 5000;
        child->posX = pCurrent->startX;
        child->posY = pCurrent->posY;
        child->posZ = pCurrent->startZ;
        child->yaw = g_BattleSpecVacuumRotVector.yaw;
        child->roll = 0;
        child->pitch = 0;
        for (spriteIndex = 0; spriteIndex < 2; spriteIndex++) {
            child = (BattleFxVacuumChildSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialVacuumDecelSpriteTask);
            g_BattleSpecVacuumAngle = rand() % 0x1000;
            child->velocityX = CosFixed(g_BattleSpecVacuumAngle) * 0xBB8 >> 12;
            child->velocityZ = SinFixed(g_BattleSpecVacuumAngle) * 0xBB8 >> 12;
            child->velocityY = 5000;
            child->posX = pCurrent->startX;
            child->posY = pCurrent->posY;
            child->posZ = pCurrent->startZ;
            child->yaw = g_BattleSpecVacuumRotVector.yaw;
            child->roll = 0;
            child->pitch = 0;
        }
    }
    if (pCurrent->step == 20) {
        BattleFx_HitMarkSpawnScriptAtActorBone(pCurrent->actorIndex, 0x31, 0x4000 /* 4.0 */);
        Battle_PostActorHitOrHurtMessage(pCurrent->actorIndex);
    }
    (pCurrent->step)++;
    if (pCurrent->step >= 30 && sum == 0) {
        Battle_StartFixedPointRamp(0, 10);
        pCurrent->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws every live segment of the pool and returns how many were live.
 */
// FUNCTION: FF7 0x0055f071
int __cdecl BattleFx_SpecialVacuumDrawLiveSegmentRecords(void)
{
    int i, liveCount;

    liveCount = 0;
    for (i = 0; i < 200; i++) {
        if (g_BattleSpecVacuumSegmentRecords.segments[i].active != 0) {
            BattleFx_SpecialVacuumDrawSegmentRecord(&g_BattleSpecVacuumSegmentRecords.segments[i]);
            liveCount++;
        }
    }
    return liveCount;
}

/*
 * Draws one segment as a shaded quad that widens with its segmentIndex, then ages it; at index 8 it is freed.
 */
// FUNCTION: FF7 0x0055f0d0
void __cdecl BattleFx_SpecialVacuumDrawSegmentRecord(BattleFxVacuumSegment *segment)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        int quadDepth;
        struct { unsigned char pad_to_8[0x8]; SVECTOR pointCX; };
        struct { unsigned char pad_to_10[0x10]; SVECTOR pointBX; };
        struct { unsigned char pad_to_18[0x18]; SVECTOR pointDX; };
        struct { unsigned char pad_to_20[0x20]; SVECTOR pointAX; };
        struct { unsigned char pad_to_28[0x28]; BattleFxVacuumSegmentRecordPacketCursor *packetCursor; };
        struct { unsigned char pad_to_2C[0x2C]; int transformOutA; };
        struct { unsigned char pad_to_30[0x30]; int colorWord; };
        struct { unsigned char pad_to_34[0x34]; short spreadA; };
        struct { unsigned char pad_to_38[0x38]; short spreadB; };
        struct { unsigned char pad_to_3C[0x3C]; int transformOutB; };
    } frame;
#pragma pack(pop)

    frame.colorWord = 0x3A000000;
    frame.spreadA = (segment->segmentIndex + 1) * 30 + 30;
    frame.spreadB = segment->segmentIndex * 30 + 30;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    frame.pointAX.vx = segment->startX;
    frame.pointAX.vy = segment->centerA - frame.spreadA;
    frame.pointAX.vz = segment->startY;
    frame.pointDX.vx = segment->endX;
    frame.pointDX.vy = segment->centerB - frame.spreadB;
    frame.pointDX.vz = segment->endY;
    frame.pointBX.vx = segment->startX;
    frame.pointBX.vy = segment->centerA + frame.spreadA;
    frame.pointBX.vz = segment->startY;
    frame.pointCX.vx = segment->endX;
    frame.pointCX.vy = segment->centerB + frame.spreadB;
    frame.pointCX.vz = segment->endY;
    frame.packetCursor = (BattleFxVacuumSegmentRecordPacketCursor *)g_BattleSpecVacuumPacketCursor;
    frame.quadDepth = Gfx_TransformQuadReturnDepth(&frame.pointAX, &frame.pointBX, &frame.pointDX, &frame.pointCX, &frame.transformOutB, &frame.transformOutA);
    if (frame.quadDepth > 0) {
        frame.colorWord += (8 - segment->segmentIndex) * 1052688;
        frame.packetCursor->tag = 0x8000000;
        frame.packetCursor->color2 = frame.colorWord - 0x101010;
        frame.packetCursor->color0 = frame.packetCursor->color2;
        frame.packetCursor->color3 = frame.colorWord;
        frame.packetCursor->color1 = frame.colorWord;
        Gfx_DrawPsxPrimitive(frame.packetCursor);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (segment->segmentIndex)++;
    if (segment->segmentIndex == 8) {
        segment->active = 0;
    }
}

/*
 * Child task: draws a sprite at the projected position and decays its velocity until frame 10.
 */
// FUNCTION: FF7 0x0055f284
void __cdecl BattleFx_SpecialVacuumDecelSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[48];
        int pointDepth;
        struct { unsigned char pad_to_4[0x4]; SVECTOR velocityX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxVacuumChildSlot *spriteSlot; };
    } frame;
#pragma pack(pop)

    frame.spriteSlot = (BattleFxVacuumChildSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecVacuumSpriteReq.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecVacuumLoadedRes);
    g_BattleSpecVacuumSpriteReq.drawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecVacuumLoadedRes);
    frame.velocityX.vx = frame.spriteSlot->velocityX;
    frame.velocityX.vy = frame.spriteSlot->velocityZ;
    frame.velocityX.vz = frame.spriteSlot->velocityY;
    RotMatrix((SVECTOR *)&frame.spriteSlot->pitch, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.spriteSlot->posX;
    frame.rotMatrix.t[1] = frame.spriteSlot->posY;
    frame.rotMatrix.t[2] = frame.spriteSlot->posZ;
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    RotTransPointToInt16(&frame.velocityX, (short *)&frame.velocityX, &frame.pointDepth);
    BattleFx_InstallBillboardMatrix(&frame.velocityX, 0x2000 /* 2.0 */, 0);
    g_BattleSpecVacuumSpriteReq.frame = (frame.spriteSlot->frameCounter % 2);
    g_BattleSpecVacuumPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecVacuumSpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecVacuumPacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spriteSlot->velocityX += (short)((0 - frame.spriteSlot->velocityX) / 4);
    frame.spriteSlot->velocityZ += (short)((0 - frame.spriteSlot->velocityZ) / 4);
    frame.spriteSlot->velocityY -= 300;
    (frame.spriteSlot->frameCounter)++;
    if (frame.spriteSlot->frameCounter == 10) {
        frame.spriteSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Child task: decays its velocity and claims a segment spanning the old and new projected positions each frame, until frame 10.
 */
// FUNCTION: FF7 0x0055f42e
void __cdecl BattleFx_SpecialVacuumDecelSegmentSpawnerTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[68];
        SVECTOR velocityX;
        struct { unsigned char pad_to_8[0x8]; int pointDepth; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxVacuumChildSlot *spawnerSlot; };
        struct { unsigned char pad_to_38[0x38]; BattleFxVacuumSegment *segment; };
        struct { unsigned char pad_to_3C[0x3C]; SVECTOR decayedVelX; };
    } frame;
#pragma pack(pop)

    frame.spawnerSlot = (BattleFxVacuumChildSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.velocityX.vx = frame.spawnerSlot->velocityX;
    frame.velocityX.vy = frame.spawnerSlot->velocityZ;
    frame.velocityX.vz = frame.spawnerSlot->velocityY;
    frame.spawnerSlot->velocityX += (short)((0 - frame.spawnerSlot->velocityX) / 4);
    frame.decayedVelX.vx = frame.spawnerSlot->velocityX;
    frame.spawnerSlot->velocityZ += (short)((0 - frame.spawnerSlot->velocityZ) / 4);
    frame.decayedVelX.vy = frame.spawnerSlot->velocityZ;
    frame.spawnerSlot->velocityY -= 300;
    frame.decayedVelX.vz = frame.spawnerSlot->velocityY;
    RotMatrix((SVECTOR *)&frame.spawnerSlot->pitch, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.spawnerSlot->posX;
    frame.rotMatrix.t[1] = frame.spawnerSlot->posY;
    frame.rotMatrix.t[2] = frame.spawnerSlot->posZ;
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    RotTransPointToInt16(&frame.velocityX, (short *)&frame.velocityX, &frame.pointDepth);
    RotTransPointToInt16(&frame.decayedVelX, (short *)&frame.decayedVelX, &frame.pointDepth);
    frame.segment = BattleFx_SpecialVacuumClaimSegmentRecord();
    if (frame.segment != 0) {
        frame.segment->startX = frame.velocityX.vx;
        frame.segment->centerA = frame.velocityX.vy;
        frame.segment->startY = frame.velocityX.vz;
        frame.segment->endX = frame.decayedVelX.vx;
        frame.segment->centerB = frame.decayedVelX.vy;
        frame.segment->endY = frame.decayedVelX.vz;
        frame.segment->segmentIndex = 0;
    }
    (frame.spawnerSlot->frameCounter)++;
    if (frame.spawnerSlot->frameCounter == 10) {
        frame.spawnerSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Claims a free segment of the pool, or returns 0 when all 200 are live.
 */
// FUNCTION: FF7 0x0055f5ed
BattleFxVacuumSegment *__cdecl BattleFx_SpecialVacuumClaimSegmentRecord(void)
{
    int i;

    for (i = 0; i < 200; i++) {
        if (g_BattleSpecVacuumSegmentRecords.segments[i].active == 0) {
            g_BattleSpecVacuumSegmentRecords.segments[i].active = 1;
            return &g_BattleSpecVacuumSegmentRecords.segments[i];
        }
    }
    return 0;
}

/*
 * Child task: draws the Bakyu model, growing for 6 frames and shrinking after frame 30, and jitters the camera focal point.
 */
// FUNCTION: FF7 0x0055f641
void __cdecl BattleFx_SpecialVacuumJitterModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_30[0x30]; BattleFxVacuumChildSlot *jitterSlot; };
        struct { unsigned char pad_to_34[0x34]; PsxMatrix drawMatrix; };
    } frame;
#pragma pack(pop)

    frame.jitterSlot = (BattleFxVacuumChildSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecVacuumModelReq.draw.modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecVacuumLoadedRes);
    RotMatrix((SVECTOR *)&frame.jitterSlot->pitch, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecVacuumModelReq);
    frame.rotMatrix.t[0] = frame.jitterSlot->posX;
    frame.rotMatrix.t[1] = frame.jitterSlot->posY;
    frame.rotMatrix.t[2] = frame.jitterSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    g_BattleSpecVacuumModelReq.draw.texturePageBits = frame.jitterSlot->frameCounter % 5 * 3 << 6;
    g_BattleSpecVacuumModelReq.draw.fadeLevel = g_BattleSpecVacuumModelStep * 112 / 6;
    g_BattleSpecVacuumPacketCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecVacuumModelReq.draw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecVacuumPacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.jitterSlot->frameCounter < 6) {
        g_BattleSpecVacuumModelStep++;
    }
    if (frame.jitterSlot->frameCounter > 30) {
        g_BattleSpecVacuumModelStep--;
    }
    g_BattleCameraFocalPoint.focalPointY += (short)(((frame.jitterSlot->frameCounter % 2) << 6) - 32);
    (frame.jitterSlot->frameCounter)++;
    if (frame.jitterSlot->frameCounter == 36) {
        frame.jitterSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
