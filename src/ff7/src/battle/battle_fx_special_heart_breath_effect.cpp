// src/ff7/src/battle/battle_fx_special_heart_breath_effect.cpp
/* Enemy Special Attack [112]: Heart Breath (orig. SPECIAL/PINK) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Pink': a per-target timeline and particle emitter with a pool of
//   particle records and sprites, plus growing models drawn each frame.

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

struct BattleFxHeartBreathParticleEmitter;
struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x0089d8a8
int g_BattleSpecHeartBreathResourceDefs = 0x001900B1;
int g_BattleSpecHeartBreathResourceDefsHeadPtr[1] = {
    (int)&g_BattleFxSpecialHeartBreathEffectResourceNames[0]
};
// GLOBAL: FF7 0x0089d8b0
BattleSpecHeartBreathResourceModelRows g_BattleSpecHeartBreathResourceModelRows = {{
    0x0, 0x0, 0x0, 0x1900b1, (int)&g_BattleFxSpecialHeartBreathEffectResourceNames[0xc], 0x0,
    0x0, 0x0, 0x192a39, (int)&g_BattleFxSpecialHeartBreathEffectResourceNames[0x18], (int)&g_BattleFxSpecialHeartBreathEffectResourceNames[0x24], 0x0,
    0x0, 0x192a39, (int)&g_BattleFxSpecialHeartBreathEffectResourceNames[0x30], (int)&g_BattleFxSpecialHeartBreathEffectResourceNames[0x3c], 0x0, 0x0,
    0x192a39, (int)&g_BattleFxSpecialHeartBreathEffectResourceNames[0x48], (int)&g_BattleFxSpecialHeartBreathEffectResourceNames[0x54], 0x0, 0x0, 0x192a39,
    (int)&g_BattleFxSpecialHeartBreathEffectResourceNames[0x60], (int)&g_BattleFxSpecialHeartBreathEffectResourceNames[0x6c], 0x0, 0x0, 0x0, 0x0,
    0x2c808080, 0x0}, 0x0, 0x0
};
// GLOBAL: FF7 0x0089d938
struct BattleSpecHeartBreathSpriteReq1 g_BattleSpecHeartBreathSpriteReq1 = {-128, -128, -128, {0x2e, 0, 0, 0, 0}};
// GLOBAL: FF7 0x0089d940
int g_BattleSpecHeartBreathModelScale = 6656;
// GLOBAL: FF7 0x0089d944
int g_BattleSpecHeartBreathModelStep = 4096;
// GLOBAL: FF7 0x0089d948
struct BattleSpecHeartBreathSpriteReq2 g_BattleSpecHeartBreathSpriteReq2 = {0x1a00, {0}, 0, {8, 0, 0, 0, 0, 0}, 128};
unsigned char g_rawData_0089d95c[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0089d960
struct BattleFxSpriteRequest g_BattleSpecHeartBreathParticleSpriteReq = {0, 0, -128, -128, -128, 0x2c, 0, 0};
// GLOBAL: FF7 0x0089d970
struct BattleSpecHeartBreathSpriteReq4 g_BattleSpecHeartBreathSpriteReq4 = {0, {0xa8, 0, 0, 0, 0, 0}, 128};
unsigned char g_rawData_0089d97c[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0089d980
int g_BattleSpecHeartBreathMatrixReq = 64512;
unsigned char g_BattleFxSpecialHeartBreathEffectResourceNames[120] =
    "SUNA2.RSD\0\0\0"
    "HIKARI.RSD\0\0"
    "PINK00.TIM\0\0"
    "MARU.SPT\0\0\0\0"
    "TAMA.TIM\0\0\0\0"
    "TAMA.SPT\0\0\0\0"
    "PINK00.TIM\0\0"
    "HEART.SPT\0\0\0"
    "BAKU1.TIM\0\0\0"
    "BAKU1.SPT";
// GLOBAL: FF7 0x00b5c3e0
int g_BattleSpecHeartBreathDrawListHead = 0;
// GLOBAL: FF7 0x00b5c3e4
short g_BattleSpecHeartBreathTargetMask = 0;
// GLOBAL: FF7 0x00b5c3e8
int g_BattleSpecHeartBreathPoolActiveFlag = 0;
// GLOBAL: FF7 0x00b5c3ec
int g_BattleSpecHeartBreathEmitterState = 0;
// GLOBAL: FF7 0x00b5c3f0
short g_BattleSpecHeartBreathParticleLoopIndex = 0;
unsigned char g_bssPad_00b5c3f4[4] = {0};
// GLOBAL: FF7 0x00b5c3f8
struct BattleFxHeartBreathParticleSpriteSlot g_BattleSpecHeartBreathParticleSlots[1198] = {0};
unsigned char g_bssPad_00b5d6b0[8] = {0};
// The first two of the 200 particle records; the code walks all of them, into the bytes that follow.
// GLOBAL: FF7 0x00b5d6b8
BattleSpecHeartBreathParticleRecord g_BattleSpecHeartBreathParticleRecords[2] = {0};
unsigned char g_bssPad_00b5d6c8[1584] = {0};
// GLOBAL: FF7 0x00b5dcf8
struct GfxTableCursor *g_BattleSpecHeartBreathLoadedResTable = 0;
// GLOBAL: FF7 0x00b5dcfc
short g_BattleSpecHeartBreathParticleActiveFlag = 0;
unsigned char g_bssPad_00b5dd00[24] = {0};
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/* Loads the SPECIAL/PINK resources once the battle scene exists. */
// FUNCTION: FF7 0x00520290
void __cdecl BattleFx_SpecialLoadResHeartBreath(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest currentRecord;
    int kind;
    char outSize[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecHeartBreathLoadedResTable == 0) {
        kind = 6;
        resourceDefsWord = (int)&g_BattleSpecHeartBreathResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0089da00
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/PINK", outSize);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecHeartBreathLoadedResTable, curOwnerCtx);
    }
}

/* Entry point of the Pink effect: clears its flags, then begins it. */
// FUNCTION: FF7 0x00520320
void __cdecl BattleFx_SpecialEnterHeartBreath(int targetMask, int actorIdx)
{
    g_BattleSpecHeartBreathParticleActiveFlag = 0;
    g_BattleSpecHeartBreathPoolActiveFlag = 0;
    g_BattleSpecHeartBreathEmitterState = 0;
    BattleFx_SpecialBeginHeartBreath(targetMask, actorIdx);
}

/* Starts the timeline from the caster's bone to the targets' centroid, resets the particle records and starts the sprite pool. */
// FUNCTION: FF7 0x00520352
void __cdecl BattleFx_SpecialBeginHeartBreath(int targetMask, int actorIdx)
{
    short i;
    BattleFxSpecialHeartBreathSegment *task;

    g_BattleSpecHeartBreathTargetMask = targetMask;
    BattleFx_SpecialLoadResHeartBreath();
    task = (BattleFxSpecialHeartBreathSegment *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHeartBreathTimelineTask);
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].bone33, &task->start.vx);
    Battle_GetActorMaskCentroid(targetMask, &task->end.vx);
    for (i = 0; i < 200; i++) {
        g_BattleSpecHeartBreathParticleRecords[i].active = 0;
    }
    BattleFx_SpecialHeartBreathInitParticleSpritePool();
    Battle_PlaySfxPannedToActor(0x11F, (short)actorIdx, 0x40);
}

/* Drives the Pink effect: the screen flash, the particle emitter and growing models, and the hits on the targets. */
// FUNCTION: FF7 0x00520406
void __cdecl BattleFx_SpecialHeartBreathTimelineTask(void)
{
    short heading, dy5, xDelta;
    BattleFxSpecialHeartBreathSegment *self8;
    short dist, srcBit;
        int v, w;
        BattleFxHeartBreathTimelineRecord *spawned;

    self8 = (BattleFxSpecialHeartBreathSegment *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(*(char *)&g_BattleSpecHeartBreathEmitterState, 0, *(char *)&g_BattleSpecHeartBreathEmitterState);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (self8->timer < 16) {
        g_BattleSpecHeartBreathEmitterState += 4;
    }
    if (self8->timer > 94) {
        g_BattleSpecHeartBreathEmitterState -= 4;
    }
    if (self8->timer == 0) {
        heading = ArcTangentFixed(self8->end.vx - self8->start.vx, self8->end.vz - self8->start.vz);
        spawned = (BattleFxHeartBreathTimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHeartBreathParticleEmitterTask);
        spawned->posX = self8->start.vx;
        spawned->posHeight = self8->start.vy;
        spawned->posY = self8->start.vz;
        spawned->stepX = (self8->end.vx - self8->start.vx) / 20;
        spawned->angle = 0;
        spawned->stepY = (self8->end.vz - self8->start.vz) / 20;
        spawned->phase = heading;
        spawned->scalePhase = 0;
        spawned = (BattleFxHeartBreathTimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHeartBreathGrowingModelTask);
        spawned->posX = self8->start.vx;
        spawned->posHeight = self8->start.vy;
        spawned->posY = self8->start.vz;
        spawned->stepY = 1024;
        spawned->angle = heading - 0x400;
        xDelta = self8->end.vx - self8->start.vx;
        dy5 = self8->end.vz - self8->start.vz;
        dist = SqrtToInt(xDelta * xDelta + dy5 * dy5);
        spawned->lengthScale = dist * 0x1500 / 2800;
        spawned->scalePhase = 0;
    }
    if (self8->timer == 48) {
        for (srcBit = 0; srcBit < 0xA; srcBit++) {
            if ((g_BattleSpecHeartBreathTargetMask & 1 << srcBit) != 0) {
                spawned = (BattleFxHeartBreathTimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHeartBreathTargetGrowingModelTask);
                Battle_GetBoneWorldPos(srcBit, g_BattleActorRecords[srcBit].targetBone, &spawned->posX);
            }
        }
        Battle_PlaySfxPannedToActor(0x19B, -1, 0x40);
    }
    if (self8->timer == 80) {
        for (srcBit = 0; srcBit < 0xA; srcBit++) {
            if ((g_BattleSpecHeartBreathTargetMask & 1 << srcBit) != 0) {
                Battle_PostActorHitOrHurtMessage(srcBit);
            }
        }
    }
    (self8->timer)++;
    if (self8->timer == 110) {
        g_BattleSpecHeartBreathParticleActiveFlag = 1;
        self8->step = 0xFFFF;
    }
}

/* Spawns particle records for the first frames, spins them and frees the task once all have faded. */
// FUNCTION: FF7 0x00520746
void __cdecl BattleFx_SpecialHeartBreathParticleEmitterTask(void)
{
    BattleFxHeartBreathParticleEmitter *taskPtr;
    short i, alive;
    BattleFxClaimedParticleRecord *claimed;

    taskPtr = (BattleFxHeartBreathParticleEmitter *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_SpecialHeartBreathDrawParticleSprites(taskPtr);
    if (g_BattleIsPaused != 0) {
        return;
    }
    taskPtr->angle += 0xA;
    if (taskPtr->age > 0 && taskPtr->age < 25) {
        for (i = 0; i < 4; i++) {
            claimed = (BattleFxClaimedParticleRecord *)BattleFx_SpecialHeartBreathClaimParticleRecord();
            if (claimed != 0) {
                claimed->radius = rand() % 800;
                claimed->angle = rand() & 0xFFF;
            }
        }
    }
    i = 0;
    for (alive = 0; i < 200; i++) {
        if (g_BattleSpecHeartBreathParticleRecords[i].active != 0) {
            alive++;
            g_BattleSpecHeartBreathParticleRecords[i].angle -= 0x80;
            (g_BattleSpecHeartBreathParticleRecords[i].radiusScale)++;
            if (g_BattleSpecHeartBreathParticleRecords[i].radiusScale == 0x19) {
                g_BattleSpecHeartBreathParticleRecords[i].active = 0;
            }
        }
    }
    (taskPtr->age)++;
    if (taskPtr->age > 25 && alive == 0) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Claims the first free particle record and returns it, or 0 when all 200 are live. */
// FUNCTION: FF7 0x005208c6
int __cdecl BattleFx_SpecialHeartBreathClaimParticleRecord(void)
{
    short i;

    for (i = 0; i < 200; i++) {
        if (g_BattleSpecHeartBreathParticleRecords[i].active == 0) {
            g_BattleSpecHeartBreathParticleRecords[i].active = 1;
            g_BattleSpecHeartBreathParticleRecords[i].radiusScale = 0;
            return (int)&g_BattleSpecHeartBreathParticleRecords[i];
        }
    }
    return 0;
}

/* Draws every live particle record as a sprite, spread out from the emitter. */
// FUNCTION: FF7 0x0052092b
void __cdecl BattleFx_SpecialHeartBreathDrawParticleSprites(BattleFxHeartBreathParticleEmitter *emitterSlot)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        int sizeValue;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; int depthZ; };
        struct { unsigned char pad_to_10[0x10]; SVECTOR posX; };
        struct { unsigned char pad_to_18[0x18]; short activeCount; };
        struct { unsigned char pad_to_20[0x20]; short sizeIndex; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_44[0x44]; short particleIndex; };
    } frame;
#pragma pack(pop)

    g_BattleSpecHeartBreathResourceModelRows.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(2, g_BattleSpecHeartBreathLoadedResTable);
    g_BattleSpecHeartBreathResourceModelRows.drawObj = Gfx_ModelTableGetSpriteObject(2, g_BattleSpecHeartBreathLoadedResTable);
    frame.rotX.vz = 0;
    frame.rotX.vx = frame.rotX.vz;
    frame.rotX.vy = emitterSlot->rotationY;
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    frame.rotMatrix.t[2] = 0;
    frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
    frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
    frame.particleIndex = 0;
    for (frame.activeCount = 0; frame.particleIndex < 200; frame.particleIndex++) {
        if (g_BattleSpecHeartBreathParticleRecords[frame.particleIndex].active != 0) {
            frame.activeCount++;
            frame.posX.vx = CosFixed(g_BattleSpecHeartBreathParticleRecords[frame.particleIndex].angle) * g_BattleSpecHeartBreathParticleRecords[frame.particleIndex].radius * g_BattleSpecHeartBreathParticleRecords[frame.particleIndex].radiusScale / 20 * (emitterSlot->angle + 256) >> 20;
            frame.posX.vy = SinFixed(g_BattleSpecHeartBreathParticleRecords[frame.particleIndex].angle) * g_BattleSpecHeartBreathParticleRecords[frame.particleIndex].radius * g_BattleSpecHeartBreathParticleRecords[frame.particleIndex].radiusScale / 20 * (emitterSlot->angle + 256) >> 20;
            frame.posX.vz = 0;
            SetRotMatrix(&frame.rotMatrix);
            SetTransMatrix(&frame.rotMatrix);
            RotTransPointToInt16(&frame.posX, (short *)&frame.posX, &frame.depthZ);
            frame.posX.vx += (short)(emitterSlot->centerX + emitterSlot->radiusX * g_BattleSpecHeartBreathParticleRecords[frame.particleIndex].radiusScale);
            frame.posX.vy += (short)(emitterSlot->centerY + emitterSlot->radiusY * g_BattleSpecHeartBreathParticleRecords[frame.particleIndex].radiusScale);
            frame.posX.vz += (short)(emitterSlot->centerZ + emitterSlot->radiusZ * g_BattleSpecHeartBreathParticleRecords[frame.particleIndex].radiusScale);
            if (g_BattleSpecHeartBreathParticleRecords[frame.particleIndex].radiusScale < 0x11) {
                frame.sizeValue = 8;
            } else {
                frame.sizeValue = 25 - g_BattleSpecHeartBreathParticleRecords[frame.particleIndex].radiusScale;
            }
            frame.sizeIndex = *(short *)&frame.sizeValue;
            BattleFx_InstallBillboardMatrix(&frame.posX, (g_BattleSpecHeartBreathParticleRecords[frame.particleIndex].radiusScale << 6) + 0x800, 0);
            g_BattleSpecHeartBreathSpriteReq1.blue = frame.sizeIndex << 4;
            g_BattleSpecHeartBreathSpriteReq1.green = g_BattleSpecHeartBreathSpriteReq1.blue;
            (g_BattleSpecHeartBreathSpriteReq1.red) = g_BattleSpecHeartBreathSpriteReq1.green;
            g_BattleSpecHeartBreathDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecHeartBreathResourceModelRows.spriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecHeartBreathDrawListHead);
        }
    }
}

/* Draws the growing, spinning model along the beam and drives the flash level. */
// FUNCTION: FF7 0x00520be3
void __cdecl BattleFx_SpecialHeartBreathGrowingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_30[0x30]; BattleFxHeartBreathGrowingModelSlot *slot; };
        struct { unsigned char pad_to_34[0x34]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxHeartBreathGrowingModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecHeartBreathSpriteReq2.res8 = Gfx_ModelTableGetRenderObject(0, g_BattleSpecHeartBreathLoadedResTable);
    frame.slot->rotX = -frame.slot->frameCounter << 7;
    RotMatrix((SVECTOR *)&frame.slot->rotX, &frame.rotMatrix);
    g_BattleSpecHeartBreathModelStep = frame.slot->modelStep;
    (g_BattleSpecHeartBreathSpriteReq2.modelScale) = (frame.slot->phase + 0x100) * 0x1A00 >> 8;
    g_BattleSpecHeartBreathModelScale = (g_BattleSpecHeartBreathSpriteReq2.modelScale);
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecHeartBreathModelScale);
    frame.rotMatrix.t[0] = frame.slot->posX;
    frame.rotMatrix.t[1] = frame.slot->posY;
    frame.rotMatrix.t[2] = frame.slot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecHeartBreathSpriteReq2.alpha = g_BattleSpecHeartBreathPoolActiveFlag << 4;
    g_BattleSpecHeartBreathDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecHeartBreathSpriteReq2.res8, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecHeartBreathDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.slot->phase += 0xA;
    if (frame.slot->frameCounter < 8) {
        g_BattleSpecHeartBreathPoolActiveFlag++;
    }
    if (frame.slot->frameCounter > 37) {
        g_BattleSpecHeartBreathPoolActiveFlag--;
    }
    (frame.slot->frameCounter)++;
    if (frame.slot->frameCounter == 45) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Draws the growing model on a target and sprays particle sprites from it. */
// FUNCTION: FF7 0x00520d7c
void __cdecl BattleFx_SpecialHeartBreathTargetGrowingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[96];
        int depthZ;
        struct { unsigned char pad_to_4[0x4]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix rotAngles; };
        struct { unsigned char pad_to_48[0x48]; BattleFxRadiantBreathAnimatedSpriteSlot *particleRecord; };
        struct { unsigned char pad_to_4C[0x4C]; short axisIndex; };
        struct { unsigned char pad_to_54[0x54]; BattleFxOilTargetModelSlot *slot; };
        struct { unsigned char pad_to_5C[0x5C]; short spriteAngle; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecHeartBreathSpriteReq4.modelField) = Gfx_ModelTableGetRenderObject(1, g_BattleSpecHeartBreathLoadedResTable);
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.rotAngles.m[2][frame.axisIndex] = 0;
        frame.rotAngles.m[1][frame.axisIndex] = 0;
        frame.rotAngles.m[0][frame.axisIndex] = 0;
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((char *)frame.slot + 4, frame.rotAngles.t, &frame.depthZ);
    frame.rotAngles.m[2][2] = frame.slot->frameCounter * 0xF0 + 0x800;
    frame.rotAngles.m[1][1] = frame.rotAngles.m[2][2];
    frame.rotAngles.m[0][0] = frame.rotAngles.m[1][1];
    RotMatrix((SVECTOR *)&g_BattleSpecHeartBreathMatrixReq, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.rotAngles, &frame.rotMatrix);
    SetRotMatrix(&frame.rotAngles);
    SetTransMatrix(&frame.rotAngles);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecHeartBreathSpriteReq4.alpha = frame.slot->frameCounter * 100;
    g_BattleSpecHeartBreathDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecHeartBreathSpriteReq4, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecHeartBreathDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.slot->frameCounter < 20) {
        frame.particleRecord = (BattleFxRadiantBreathAnimatedSpriteSlot *)(((short)BattleFx_SpecialHeartBreathClaimParticleSpriteSlot() << 5) + (char *)&g_BattleSpecHeartBreathParticleSlots);
        frame.particleRecord->posX = frame.slot->posX;
        frame.particleRecord->posY = -rand() % 100;
        frame.particleRecord->posZ = frame.slot->posZ;
        frame.spriteAngle = ((rand()) % 4096);
        frame.particleRecord->rotOrVelX = SinFixed(frame.spriteAngle) * 0x3C >> 12;
        frame.particleRecord->rotOrVelZ = CosFixed(frame.spriteAngle) * 0x3C >> 12;
        frame.particleRecord->rotOrVelY = 0xFFFFFF6A - rand() % 50;
        frame.particleRecord->animIndex = ((rand()) % 2);
    }
    (frame.slot->frameCounter)++;
    if (frame.slot->frameCounter == 40) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Frees all 150 particle sprite slots and starts the pool task. */
// FUNCTION: FF7 0x00520fcd
void __cdecl BattleFx_SpecialHeartBreathInitParticleSpritePool(void)
{
    BattleFxHeartBreathParticleSpriteSlot *cursor;
    short i;

    cursor = g_BattleSpecHeartBreathParticleSlots;
    for (i = 0; i < 150; i++) {
        cursor->timer = 0;
        cursor->status = 0;
        cursor += 8;
    }
    Battle_AllocateResourceSlot(BattleFx_SpecialHeartBreathParticleSpritePoolTask);
}

/* Updates every live particle sprite slot and frees the task once the effect is done. */
// FUNCTION: FF7 0x00521027
void __cdecl BattleFx_SpecialHeartBreathParticleSpritePoolTask(void)
{
    BattleTaskSlot *taskPtr;
    BattleFxHeartBreathParticleSpriteSlot *rec;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    rec = g_BattleSpecHeartBreathParticleSlots;
    for (g_BattleSpecHeartBreathParticleLoopIndex = 0; g_BattleSpecHeartBreathParticleLoopIndex < 150; g_BattleSpecHeartBreathParticleLoopIndex++) {
        if (rec->status == 1) {
            BattleFx_SpecialHeartBreathUpdateParticleSprite();
            if (rec->status == -1) {
                rec->timer = 0;
                rec->status = 0;
            }
        }
        rec += 8;
    }
    if (g_BattleSpecHeartBreathParticleActiveFlag == 1) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Draws one particle sprite, fading it out late in its life, and moves it under gravity. */
// FUNCTION: FF7 0x005210c1
void __cdecl BattleFx_SpecialHeartBreathUpdateParticleSprite(void)
{
    struct {
        int color;
        int table[2];
        BattleFxHeartBreathParticleSprite *slot;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.slot = (BattleFxHeartBreathParticleSprite *)((g_BattleSpecHeartBreathParticleLoopIndex << 5) + (char *)&g_BattleSpecHeartBreathParticleSlots);
    frame.table[0] = 3;
    frame.table[1] = 4;
    BattleFx_InstallBillboardMatrix(&frame.slot->posX, (frame.slot->angle << 11) + 0x800, 0);
    g_BattleSpecHeartBreathParticleSpriteReq.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(frame.table[frame.slot->angle], g_BattleSpecHeartBreathLoadedResTable);
    g_BattleSpecHeartBreathParticleSpriteReq.drawObj = Gfx_ModelTableGetSpriteObject(frame.table[frame.slot->angle], g_BattleSpecHeartBreathLoadedResTable);
    if (frame.slot->timer < 22)
        frame.color = 8;
    else
        frame.color = 0x1E - frame.slot->timer;
    g_BattleSpecHeartBreathParticleSpriteReq.colorB = frame.color << 4;
    g_BattleSpecHeartBreathParticleSpriteReq.colorG = g_BattleSpecHeartBreathParticleSpriteReq.colorB;
    g_BattleSpecHeartBreathParticleSpriteReq.colorR = g_BattleSpecHeartBreathParticleSpriteReq.colorG;
    g_BattleSpecHeartBreathDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecHeartBreathParticleSpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecHeartBreathDrawListHead);
    if (g_BattleIsPaused != 0)
        return;
    frame.slot->posX += frame.slot->velocityX;
    frame.slot->posY += frame.slot->velocityY;
    frame.slot->posZ += frame.slot->velocityZ;
    frame.slot->velocityY += 10;
    (frame.slot->timer)++;
    if (frame.slot->timer == 30)
        frame.slot->status = 0xFFFF;
}

/* Claims the first free particle sprite slot and returns its index; plays a sound and returns 0 when all are in use. */
// FUNCTION: FF7 0x0052123e
short __cdecl BattleFx_SpecialHeartBreathClaimParticleSpriteSlot(void)
{
    BattleFxHeartBreathParticleSpriteSlot *cursor;
    short i;

    cursor = g_BattleSpecHeartBreathParticleSlots;
    for (i = 0; i < 150; i++) {
        if (cursor->status == 0) {
            cursor->status = 1;
            return i;
        }
        cursor += 8;
    }
    i = 0;
    Battle_PlaySfxPannedToActor(6, -1, 0x40);
    return 0;
}
} /* extern "C" */
