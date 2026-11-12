// src/ff7/src/battle/battle_fx_special_cold_breath.cpp
/* Enemy Special Effect [idx 8]: ColdBreath (JP: cold1 / コールド1 (Cold 1), asset: "SPECIAL/COLD1") — Cold Breath (0x1bd, Crawler) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Cold1 attack: it loads resources and spawns a per-target
//   timeline that drives an animated sprite, a particle field with its own record claim and draw, a
//   spinning stretched model and a rising growing model.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle_3d.h"
#include "ff7/camera_script.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxColdBreathParticle;
struct BattleFxModelDraw;

// clang-format off
// GLOBAL: FF7 0x008a65a8
int g_BattleSpecialColdBreathResourceDefs = 0x001900B1;
char *g_BattleFxSpecialColdBreathResourceNames[1] = {
    // STRING: FF7 0x008a6618
    "HUBUKI1.RSD"
};
unsigned int g_BattleSpecialColdBreathResourceDefsDataTail[14] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a6624
    (unsigned int)"KOURI.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a6630
    (unsigned int)"BAKU1.TIM",
    // STRING: FF7 0x008a663c
    (unsigned int)"BAKU1.SPT",
    0x00000000,
    0x00000000,
    0x00000000
};
#undef g_BattleSpecialColdBreathModelReq
// GLOBAL: FF7 0x008a65e8
struct BattleSpecialColdBreathModelReq g_BattleSpecialColdBreathModelReq = {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}, 0, {0x28, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}, 0, {0xa8, 0, 0, 0, 0, 0}, 128};
unsigned char g_rawData_008a6614[4] = {0x20, 0x00, 0x00, 0x00};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/* The particle field is 200 records of 8 bytes; the declared object holds the first two and the rest is the storage after it. */

// clang-format off
// GLOBAL: FF7 0x00b6e950
short g_BattleSpecialColdBreathTargetMask = 0;
// GLOBAL: FF7 0x00b6e954
short g_BattleSpecialColdBreathActorIdx = 0;
// GLOBAL: FF7 0x00b6e958
int g_BattleSpecialColdBreathCentroidVec = 0;
unsigned char g_bssPad_00b6e95c[4] = {0};
// GLOBAL: FF7 0x00b6e960
int g_BattleSpecialColdBreathCentroidZ = 0;
// GLOBAL: FF7 0x00b6e964
int g_BattleSpecialColdBreathScale1 = 0;
// GLOBAL: FF7 0x00b6e968
int g_BattleSpecialColdBreathScale2 = 0;
unsigned char g_bssPad_00b6e96c[4] = {0};
// GLOBAL: FF7 0x00b6e970
short g_BattleSpecialColdBreathCounter = 0;
unsigned char g_bssPad_00b6e974[4] = {0};
// GLOBAL: FF7 0x00b6e978
struct BattleSpecialColdBreathParticleState g_BattleSpecialColdBreathParticleState[2] = {0};
unsigned char g_bssPad_00b6e988[1584] = {0};
// GLOBAL: FF7 0x00b6efb8
struct GfxTableCursor *g_BattleSpecialColdBreathLoadedRes = 0;
unsigned char g_bssPad_00b6efbc[4] = {0};
// clang-format on

/*
 * Loads resources for the Cold1 special effect when not already loaded.
 */
// FUNCTION: FF7 0x0054c8e0
void __cdecl BattleFx_SpecialLoadResColdBreath(void)
{
    GfxModelTableBank *newSceneField;
    GfxResourceRequest requestSlot;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecialColdBreathLoadedRes == 0) {
        kind = 3;
        resourceDefsWord = (int)&g_BattleSpecialColdBreathResourceDefs;
        newSceneField = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a6648
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/COLD1", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecialColdBreathLoadedRes, newSceneField);
    }
}

/*
 * Entry point for the Cold1 special effect, dispatching to begin.
 */
// FUNCTION: FF7 0x0054c970
void __cdecl BattleFx_SpecialEnterColdBreath(int targetMask, int actorIdx)
{
    short particleIndex;

    BattleFx_SpecialLoadResColdBreath();
    g_BattleSpecialColdBreathCentroidVec = (int)&g_GfxPsxPacketScratch;
    g_BattleSpecialColdBreathCounter = 0;
    g_BattleSpecialColdBreathCentroidZ = 6656;
    g_BattleSpecialColdBreathScale1 = 0x1000 /* 1.0 */;
    g_BattleSpecialColdBreathScale2 = 0x1A00 /* 1.625 */;
    g_BattleSpecialColdBreathTargetMask = 0;
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnColdBreathPerTarget);
    Battle_PlaySfxPannedToActor(0xB0, (short)actorIdx, 0x40);
    for (particleIndex = 0; particleIndex < 200; particleIndex++) {
        g_BattleSpecialColdBreathParticleState[particleIndex].active = 0;
    }
}

/*
 * Spawns Cold1 effect instances for each targeted actor.
 */
// FUNCTION: FF7 0x0054ca12
void __cdecl BattleFx_SpecialSpawnColdBreathPerTarget(int actor, int sourceIndex)
{
    BattleFxSpecialColdBreathTask *slotPtr;

    slotPtr = (BattleFxSpecialColdBreathTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialColdBreathTimelineTask);
    Battle_GetBoneWorldPos(sourceIndex, g_BattleActorRecords[sourceIndex].bone33, (short *)((char *)slotPtr + 4));
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)((char *)slotPtr + 0xC));
    slotPtr->actorIdx = actor;
}

/*
 * Timeline task controlling state progression for the Cold1 special effect.
 */
// FUNCTION: FF7 0x0054ca88
void __cdecl BattleFx_SpecialColdBreathTimelineTask(void)
{
    short diffY;
    short dx9;
    BattleFxSpecialColdBreathTask *taskPtr;
    short s;
    short j6;
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxColdBreathParticle *spawned;

    taskPtr = (BattleFxSpecialColdBreathTask *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->timer == 0) {
        g_BattleSpecialColdBreathActorIdx = ArcTangentFixed(taskPtr->endX - taskPtr->startX, taskPtr->endY - taskPtr->startY);
        spawned = (BattleFxColdBreathParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialColdBreathParticleFieldTask);
        spawned->posX = taskPtr->startX;
        spawned->baseOffset = taskPtr->baseOffset;
        spawned->posY = taskPtr->startY;
        spawned->velocityX = (taskPtr->endX - taskPtr->startX) / 20;
        spawned->rate = 0;
        spawned->velocityY = (taskPtr->endY - taskPtr->startY) / 20;
        spawned->actorIndex = g_BattleSpecialColdBreathActorIdx;
        spawned = (BattleFxColdBreathParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialColdBreathSpinStretchModelTask);
        spawned->posX = taskPtr->startX;
        spawned->baseOffset = taskPtr->baseOffset;
        spawned->posY = taskPtr->startY;
        spawned->velocityY = 1024;
        spawned->rate = g_BattleSpecialColdBreathActorIdx - 0x400;
        dx9 = taskPtr->endX - taskPtr->startX;
        diffY = taskPtr->endY - taskPtr->startY;
        s = SqrtToInt(dx9 * dx9 + diffY * diffY);
        spawned->scale = s * 0x1500 / 2800;
        spawned->fade = s * 5376 / 2800;
    }
    if (taskPtr->timer == 20) {
        spawned = (BattleFxColdBreathParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialColdBreathRisingGrowingModelTask);
        spawned->velocityX = SinFixed(g_BattleSpecialColdBreathActorIdx + 2048) * 0x1E >> 12;
        spawned->velocityY = CosFixed(g_BattleSpecialColdBreathActorIdx + 2048) * 0x1E >> 12;
        spawned->posX = taskPtr->endX - spawned->velocityX * 10;
        spawned->baseOffset = taskPtr->altBaseOffset + 0x190;
        spawned->posY = taskPtr->endY - spawned->velocityY * 10;
    }
    if (taskPtr->timer == 40) {
        Battle_PostActorHitOrHurtMessage(taskPtr->actorIdx);
        Battle_PlaySfxPannedToActor(0x95, (short)taskPtr->actorIdx, 0x40);
    }
    if (taskPtr->timer > 36 && taskPtr->timer < 44) {
        for (j6 = 0; j6 < 2; j6++) {
            spawned = (BattleFxColdBreathParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialColdBreathAnimatedSpriteTask);
            Battle_GetBoneWorldPos(taskPtr->actorIdx, g_BattleActorRecords[taskPtr->actorIdx].targetBone, (short *)((char *)spawned + 4));
            spawned->posX += (short)(rand() % 1000 - 500);
            spawned->baseOffset += (short)(rand() % 1000 - 500);
            spawned->posY += (short)(rand() % 1000 - 500);
        }
    }
    (taskPtr->timer)++;
    if (taskPtr->timer == 50) {
        taskPtr->step = 0xFFFF;
    }
}

/*
 * Task to animate and render sprites for the Cold1 special effect.
 */
// FUNCTION: FF7 0x0054ce7b
void __cdecl BattleFx_SpecialColdBreathAnimatedSpriteTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialColdBreathModelReq.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(2, g_BattleSpecialColdBreathLoadedRes);
    g_BattleSpecialColdBreathModelReq.drawObj = Gfx_ModelTableGetSpriteObject(2, g_BattleSpecialColdBreathLoadedRes);
    BattleFx_InstallBillboardMatrix((char *)task + 4, 0x1000 /* 1.0 */, 0);
    g_BattleSpecialColdBreathModelReq.frame = task->frameCounter;
    g_BattleSpecialColdBreathCentroidVec = BattleFx_SubmitSpriteDraw(&g_BattleSpecialColdBreathModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialColdBreathCentroidVec);
    if (g_BattleIsPaused == 0 && g_BattleSpecialColdBreathModelReq.spriteGroup != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= g_BattleSpecialColdBreathModelReq.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Task managing the particle field animation for Cold1.
 */
// FUNCTION: FF7 0x0054cf4c
void __cdecl BattleFx_SpecialColdBreathParticleFieldTask(void)
{
    BattleFxColdBreathParticle *taskPtr;
    short i, alive;
    BattleSpecialColdBreathParticleState *claimed;

    taskPtr = (BattleFxColdBreathParticle *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_SpecialColdBreathDrawParticleField((BattleFxColdBreathParticle *)taskPtr);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->fade != 0) {
        taskPtr->fade += 8;
    }
    if (taskPtr->frame > 0 && taskPtr->frame < 20) {
        for (i = 0; i < 8; i++) {
            claimed = (BattleSpecialColdBreathParticleState *)BattleFx_SpecialColdBreathClaimParticleRecord();
            if (claimed != 0) {
                claimed->radius = (rand() % 512);
                claimed->angle = rand() & 0xFFF;
            }
        }
    }
    i = 0;
    for (alive = 0; i < 200; i++) {
        if (g_BattleSpecialColdBreathParticleState[i].active != 0) {
            alive++;
            g_BattleSpecialColdBreathParticleState[i].angle -= 0x80;
            (g_BattleSpecialColdBreathParticleState[i].timer)++;
            if (g_BattleSpecialColdBreathParticleState[i].timer == 25) {
                g_BattleSpecialColdBreathParticleState[i].active = 0;
            }
        }
    }
    (taskPtr->frame)++;
    if (taskPtr->frame > 20 && alive == 0) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Allocates a free particle entry for the Cold1 particle field.
 */
// FUNCTION: FF7 0x0054d0dd
int __cdecl BattleFx_SpecialColdBreathClaimParticleRecord(void)
{
    short i;

    for (i = 0; i < 200; i++) {
        if (g_BattleSpecialColdBreathParticleState[i].active == 0) {
            g_BattleSpecialColdBreathParticleState[i].active = 1;
            g_BattleSpecialColdBreathParticleState[i].timer = 0;
            return (int)&g_BattleSpecialColdBreathParticleState[i];
        }
    }
    return 0;
}

/*
 * Renders the particle field geometry for Cold1.
 */
// FUNCTION: FF7 0x0054d142
void __cdecl BattleFx_SpecialColdBreathDrawParticleField(BattleFxColdBreathParticle *particle)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[80];
        int shade;
        struct { unsigned char pad_to_4[0x4]; SVECTOR angleX; };
        struct { unsigned char pad_to_C[0xC]; int transformOutB; };
        struct { unsigned char pad_to_10[0x10]; SVECTOR vertX; };
        struct { unsigned char pad_to_18[0x18]; short activeCount; };
        struct { unsigned char pad_to_1C[0x1C]; BattleFxColdBreathParticleFieldCentroidVec *primitive; };
        struct { unsigned char pad_to_20[0x20]; int depth; };
        struct { unsigned char pad_to_24[0x24]; short colorLevel; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_48[0x48]; short particleIndex; };
        struct { unsigned char pad_to_4C[0x4C]; int transformOutA; };
    } frame;
#pragma pack(pop)

    frame.angleX.vz = 0;
    frame.angleX.vx = frame.angleX.vz;
    frame.angleX.vy = particle->actorIndex;
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    frame.particleIndex = 0;
    for (frame.activeCount = 0; frame.particleIndex < 200; frame.particleIndex++) {
        if (g_BattleSpecialColdBreathParticleState[frame.particleIndex].active != 0) {
            frame.activeCount++;
            frame.vertX.vx = CosFixed(g_BattleSpecialColdBreathParticleState[frame.particleIndex].angle) * g_BattleSpecialColdBreathParticleState[frame.particleIndex].radius * g_BattleSpecialColdBreathParticleState[frame.particleIndex].timer / 20 << 8 >> 20;
            frame.vertX.vy = SinFixed(g_BattleSpecialColdBreathParticleState[frame.particleIndex].angle) * g_BattleSpecialColdBreathParticleState[frame.particleIndex].radius * g_BattleSpecialColdBreathParticleState[frame.particleIndex].timer / 20 << 8 >> 20;
            frame.vertX.vz = 0;
            TransformVectorToInt16(&frame.rotMatrix, &frame.vertX, &frame.vertX.vx);
            frame.vertX.vx += (short)(particle->posX + particle->velocityX * g_BattleSpecialColdBreathParticleState[frame.particleIndex].timer);
            frame.vertX.vy += (short)(particle->baseOffset + particle->rate * g_BattleSpecialColdBreathParticleState[frame.particleIndex].timer);
            frame.vertX.vz += (short)(particle->posY + particle->velocityY * g_BattleSpecialColdBreathParticleState[frame.particleIndex].timer);
            SetRotMatrix(&g_BattleCameraTransform);
            SetTransMatrix(&g_BattleCameraTransform);
            RefreshGlobalTransformState();
            frame.primitive = (BattleFxColdBreathParticleFieldCentroidVec *)g_BattleSpecialColdBreathCentroidVec;
            frame.primitive->packetWord = frame.primitive->packetWord & 0xFFFFFF | 0x2000000;
            frame.primitive->polyCode = 0x68;
            frame.primitive->polyCode = 0x6A;
            frame.depth = Gfx_TransformVertexSlot0ReturnDepth(&frame.vertX, &frame.transformOutA, &frame.transformOutB);
            if (g_BattleSpecialColdBreathParticleState[frame.particleIndex].timer < 17) {
                frame.shade = 8;
            } else {
                frame.shade = 0x19 - g_BattleSpecialColdBreathParticleState[frame.particleIndex].timer;
            }
            frame.colorLevel = *(short *)&frame.shade;
            frame.primitive->colorR = frame.colorLevel << 7 >> 3;
            frame.primitive->colorG = frame.colorLevel << 7 >> 3;
            frame.primitive->colorB = frame.colorLevel << 7 >> 3;
            Gfx_DrawPsxPrimitive(frame.primitive);
        }
    }
}

/*
 * Task to animate spinning stretched models for Cold1.
 */
// FUNCTION: FF7 0x0054d3bc
void __cdecl BattleFx_SpecialColdBreathSpinStretchModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_30[0x30]; BattleFxColdBreathSpinStretchModelSlot *slot; };
        struct { unsigned char pad_to_34[0x34]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxColdBreathSpinStretchModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialColdBreathModelReq.spinModel = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialColdBreathLoadedRes);
    frame.slot->spinAngle = -frame.slot->frameCounter << 7;
    RotMatrix((SVECTOR *)&frame.slot->spinAngle, &frame.rotMatrix);
    g_BattleSpecialColdBreathScale1 = frame.slot->scale;
    g_BattleSpecialColdBreathScale2 = 0x1A00 /* 1.625 */;
    g_BattleSpecialColdBreathCentroidZ = g_BattleSpecialColdBreathScale2;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialColdBreathCentroidZ);
    frame.rotMatrix.t[0] = frame.slot->posX;
    frame.rotMatrix.t[1] = frame.slot->posY;
    frame.rotMatrix.t[2] = frame.slot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecialColdBreathModelReq.spinAlpha = g_BattleSpecialColdBreathCounter << 4;
    g_BattleSpecialColdBreathCentroidVec = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialColdBreathModelReq.spinModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialColdBreathCentroidVec);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.slot->frameCounter < 8) {
        g_BattleSpecialColdBreathCounter++;
    }
    if (frame.slot->frameCounter > 25) {
        g_BattleSpecialColdBreathCounter--;
    }
    (frame.slot->frameCounter)++;
    if (frame.slot->frameCounter == 33) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task to animate rising and growing models for Cold1.
 */
// FUNCTION: FF7 0x0054d531
void __cdecl BattleFx_SpecialColdBreathRisingGrowingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[100];
        short scaleM00;
        struct { unsigned char pad_to_2[0x2]; short scaleM01; };
        struct { unsigned char pad_to_4[0x4]; short scaleM02; };
        struct { unsigned char pad_to_6[0x6]; short scaleM10; };
        struct { unsigned char pad_to_8[0x8]; short scaleM11; };
        struct { unsigned char pad_to_A[0xA]; short scaleM12; };
        struct { unsigned char pad_to_C[0xC]; short scaleM20; };
        struct { unsigned char pad_to_E[0xE]; short scaleM21; };
        struct { unsigned char pad_to_10[0x10]; short scaleM22; };
        struct { unsigned char pad_to_12[0x12]; int posX; };
        struct { unsigned char pad_to_16[0x16]; int posY; };
        struct { unsigned char pad_to_1A[0x1A]; int posZ; };
        struct { unsigned char pad_to_30[0x30]; BattleFxColdBreathRisingGrowingModelSlot *slot; };
        struct { unsigned char pad_to_34[0x34]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_54[0x54]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxColdBreathRisingGrowingModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.scaleM00 = 0x1000 /* 1.0 */;
    frame.scaleM01 = 0;
    frame.scaleM02 = 0;
    frame.scaleM10 = 0;
    frame.scaleM11 = 0x1000 /* 1.0 */;
    frame.scaleM12 = 0;
    frame.scaleM20 = 0;
    frame.scaleM21 = 0;
    frame.scaleM22 = 0x1000 /* 1.0 */;
    memset(&frame.posX, 0, 12);
    g_BattleSpecialColdBreathModelReq.risingModel = Gfx_ModelTableGetRenderObject(1, g_BattleSpecialColdBreathLoadedRes);
    frame.scaleX.vz = frame.slot->frameCounter * 0x1E0;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.scaleM00, &frame.scaleX);
    frame.posX = frame.slot->posX;
    frame.posY = frame.slot->posY;
    frame.posZ = frame.slot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.scaleM00, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecialColdBreathModelReq.risingAlpha = g_BattleSpecialColdBreathTargetMask << 9;
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialColdBreathCentroidVec = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialColdBreathModelReq.risingModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialColdBreathCentroidVec);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.slot->posY += 40;
    frame.slot->posX += frame.slot->velocityX;
    frame.slot->posZ += frame.slot->velocityZ;
    if (frame.slot->frameCounter > 12) {
        g_BattleSpecialColdBreathTargetMask++;
    }
    (frame.slot->frameCounter)++;
    if (frame.slot->frameCounter == 20) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
