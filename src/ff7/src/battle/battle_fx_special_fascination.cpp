// src/ff7/src/battle/battle_fx_special_fascination.cpp
/* Enemy Special Effect [idx 37]: Fascination (JP: yuwaku / 誘惑 (Temptation/Fascination), asset: "special/yuwaku") — Fascination (0x37a, Ultima Weapon), Bewildered (0x377, Red Dragon), L2 Confu (0x2d2) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Yuwaku attack: loads resources when needed, runs ramp
//   tasks and per-target timelines, and drives damped particles, rotating projectiles,
//   sine-floating sprites and rising variant sprites.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "lib/types/graphics.h"
#include "ff7/camera_script.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;

// clang-format off
// GLOBAL: FF7 0x008a7d38
unsigned int g_BattleSpecialFascinationResourceDefs[65] = {
    0x001900b1,
    // STRING: FF7 0x008a7ebc
    (unsigned int)"SOTO.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a7ec8
    (unsigned int)"HARTO.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a7ed4
    (unsigned int)"HARTO1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a7ee0
    (unsigned int)"HARTO2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a7eec
    (unsigned int)"HARTO3.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a7ef8
    (unsigned int)"HARTO4.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a7f04
    (unsigned int)"HARTO5.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a7f10
    (unsigned int)"HARTO6.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a7f1c
    (unsigned int)"YUWAKU03.TIM",
    // STRING: FF7 0x008a7f2c
    (unsigned int)"KIRA2.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a7f38
    (unsigned int)"YUWAKU03.TIM",
    // STRING: FF7 0x008a7f48
    (unsigned int)"KIRA3.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a7f54
    (unsigned int)"YUWAKU03.TIM",
    // STRING: FF7 0x008a7f64
    (unsigned int)"KIRA4.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a7f70
    (unsigned int)"YUWAKU03.TIM",
    // STRING: FF7 0x008a7f80
    (unsigned int)"KIRA5.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a7f8c
    (unsigned int)"YUWAKU02.TIM",
    // STRING: FF7 0x008a7f9c
    (unsigned int)"HIT1.SPT"};
// clang-format off
// GLOBAL: FF7 0x008a7e40
struct BattleSpecialFascinationColor1 g_BattleSpecialFascinationColor1 = {0, 0, {0x40, 0x40, 0x40, 0x2c}, 0};
// GLOBAL: FF7 0x008a7e50
struct BattleSpecialFascinationColor2 g_BattleSpecialFascinationColor2 = {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0};
// clang-format on
// GLOBAL: FF7 0x008a7e60
unsigned char g_BattleSpecialFascinationSpriteIndices1[16] = {0x08, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00};
// clang-format off
// GLOBAL: FF7 0x008a7e70
struct BattleSpecialFascinationColor3 g_BattleSpecialFascinationColor3 = {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0};
// clang-format on
// GLOBAL: FF7 0x008a7e80
unsigned char g_BattleSpecialFascinationSpriteIndices2[16] = {0x08, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00};
// clang-format off
// GLOBAL: FF7 0x008a7e90
struct BattleSpecialFascinationModelReq g_BattleSpecialFascinationModelReq = {0, 0, {0, 0, 0, 0, 0x20, 0, 0, 0}};
// clang-format on
// Model table indices, read as an int array by the projectile task.
// GLOBAL: FF7 0x008a7ea0
unsigned char g_BattleSpecialFascinationModelIndices[28] = {0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b6f14c
int g_BattleSpecialFascinationDrawListHandle = 0;
// GLOBAL: FF7 0x00b6f150
short g_BattleSpecialFascinationAngle = 0;
// GLOBAL: FF7 0x00b6f154
struct GfxTableCursor *g_BattleSpecialFascinationLoadedRes = 0;
// GLOBAL: FF7 0x00b6f158
short g_BattleSpecialFascinationSpin = 0;
unsigned char g_bssPad_00b6f15c[4] = {0};
// clang-format on

/*
 * Loads resources for the Yuwaku special effect when not already loaded.
 */
// FUNCTION: FF7 0x005579e0
void __cdecl BattleFx_SpecialLoadResFascination(void)
{
    GfxModelTableBank *newSceneLink;
    GfxResourceRequest resultPending;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *reqPtrPtr;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecialFascinationLoadedRes == 0) {
        kind = 0xD;
        resourceDefsWord = (int)&g_BattleSpecialFascinationResourceDefs;
        newSceneLink = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        reqPtrPtr = &resultPending;
        Gfx_InitResourceRequest(reqPtrPtr);
        reqPtrPtr->groupTag = 0;
        // STRING: FF7 0x008a7fa8
        reqPtrPtr->resourcePath = BattleFx_BuildResourcePath("special/yuwaku", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, reqPtrPtr, &g_BattleSpecialFascinationLoadedRes, newSceneLink);
    }
}

/*
 * Main entry point for the Yuwaku special effect.
 */
// FUNCTION: FF7 0x00557a70
void __cdecl BattleFx_SpecialEnterFascination(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResFascination();
    BattleFx_SpecialBeginFascination(targetMask, actorIdx);
}

/*
 * Initializes ramp tasks and spawns target tasks for Yuwaku.
 */
// FUNCTION: FF7 0x00557a8a
void __cdecl BattleFx_SpecialBeginFascination(int targetMask, int actorIdx)
{
    Battle_AllocateResourceSlot(BattleFx_SpecialFascinationWaitRampEndTask);
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnFascinationPerTarget);
    Battle_PlaySfxPannedToActor(0x7D, (short)actorIdx, 0x40);
    g_BattleFxTimelineState.red = 0x40;
    g_BattleFxTimelineState.channel1 = 0x20;
    g_BattleFxTimelineState.channel2 = 0x60;
    Battle_StartFixedPointRamp(0x1000, 10);
}

/*
 * Waits for the fixed-point ramp to complete before retiring.
 */
// FUNCTION: FF7 0x00557ae8
void __cdecl BattleFx_SpecialFascinationWaitRampEndTask(void)
{
    BattleFxIcicleDropRamp *task;

    task = (BattleFxIcicleDropRamp *)BATTLE_CURRENT_TASK_SLOT;
    task->toggle ^= 1;
    if (g_BattleSecondaryTaskActiveCount <= 1) {
        Battle_StartFixedPointRamp(0, 10);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Spawns timeline task for a single Yuwaku target.
 */
// FUNCTION: FF7 0x00557b34
void __cdecl BattleFx_SpecialSpawnFascinationPerTarget(int actor, int attackerId)
{
    int taskPtr;

    taskPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFascinationTimelineTask);
    Battle_GetBoneWorldPos(attackerId, g_BattleActorRecords[attackerId].targetBone, (short *)(taskPtr + 4));
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)(taskPtr + 0xC));
    ((BattleFxDarkWaveTimelineSlot *)taskPtr)->actorIndex = actor;
}

/*
 * Drives the main timeline and sprite/projectile spawning for Yuwaku.
 */
// FUNCTION: FF7 0x00557baa
void __cdecl BattleFx_SpecialFascinationTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        SVECTOR angleX;
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxFascinationTimelineSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_34[0x34]; short spawnIndex; };
        struct { unsigned char pad_to_38[0x38]; SVECTOR dirX; };
        struct { unsigned char pad_to_40[0x40]; BattleFxDarkWaveTimelineSlot *taskSlot; };
    } frame;
#pragma pack(pop)

    frame.taskSlot = (BattleFxDarkWaveTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.taskSlot->frameCounter == 0) {
        frame.spawnSlot = (BattleFxFascinationTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFascinationTask);
        frame.spawnSlot->posX = frame.taskSlot->posX;
        frame.spawnSlot->posY = frame.taskSlot->posY;
        frame.spawnSlot->posZ = frame.taskSlot->posZ;
        frame.spawnSlot->swayPhase = 0;
        frame.spawnSlot->spinAngleY = 0;
        frame.spawnSlot->scale = 0x1000 /* 1.0 */;
        frame.spawnSlot->angleY = ArcTangentFixed(frame.taskSlot->velocityX - frame.taskSlot->posX, frame.taskSlot->velocityZ - frame.taskSlot->posZ);
        frame.spawnSlot->posY = 0xF9C0;
        frame.spawnSlot->velocityX = (frame.taskSlot->velocityX - frame.taskSlot->posX) / 20;
        frame.spawnSlot->velocityY = 0;
        frame.spawnSlot->velocityZ = (frame.taskSlot->velocityZ - frame.taskSlot->posZ) / 20;
    }
    if (frame.taskSlot->frameCounter == 40) {
        for (frame.spawnIndex = 0; frame.spawnIndex < 16; frame.spawnIndex++) {
            frame.spawnSlot = (BattleFxFascinationTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFascinationDampedSpriteTask);
            Battle_GetBoneWorldPos(frame.taskSlot->actorIndex, g_BattleActorRecords[frame.taskSlot->actorIndex].targetBone, (short *)((int)frame.spawnSlot + 4));
            frame.dirX.vx = 0;
            frame.dirX.vy = 0;
            frame.dirX.vz = 80;
            frame.angleX.vx = frame.spawnIndex << 8;
            frame.angleX.vy = rand();
            frame.angleX.vz = 0;
            RotMatrix(&frame.angleX, &frame.rotMatrix);
            TransformVectorToInt16(&frame.rotMatrix, &frame.dirX, (void *)((int)frame.spawnSlot + 0xC));
            frame.spawnSlot->posX += (short)(frame.spawnSlot->velocityX << 2);
            frame.spawnSlot->posY += (short)(frame.spawnSlot->velocityY << 2);
            frame.spawnSlot->posZ += (short)(frame.spawnSlot->velocityZ << 2);
        }
        Battle_PostActorHitOrHurtMessage(frame.taskSlot->actorIndex);
    }
    if (frame.taskSlot->frameCounter < 8) {
        g_BattleSpecialFascinationSpin += 6;
    }
    if (frame.taskSlot->frameCounter > 52) {
        g_BattleSpecialFascinationSpin -= 6;
    }
    (frame.taskSlot->frameCounter)++;
    if (frame.taskSlot->frameCounter == 60) {
        frame.taskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates and draws damped velocity particles for Yuwaku.
 */
// FUNCTION: FF7 0x00557e58
void __cdecl BattleFx_SpecialFascinationDampedSpriteTask(void)
{
    BattleFxLivewireDotSlot *task;

    task = (BattleFxLivewireDotSlot *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), 0x1000 /* 1.0 */, 0);
    g_BattleSpecialFascinationColor1.yuwakuColor1StepWord = task->step;
    g_BattleSpecialFascinationColor1.drawObj = Gfx_ModelTableGetSpriteObject(12, g_BattleSpecialFascinationLoadedRes);
    (g_BattleSpecialFascinationColor1.animTable) = Gfx_ModelTableGetLoadedRecord(12, g_BattleSpecialFascinationLoadedRes);
    g_BattleSpecialFascinationDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialFascinationColor1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialFascinationDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posX += task->velocityX;
    task->posY += task->velocityY;
    task->posZ += task->velocityZ;
    task->velocityX -= (short)(task->velocityX / 8);
    task->velocityY -= (short)(task->velocityY / 8);
    task->velocityZ -= (short)(task->velocityZ / 8);
    (task->step)++;
    if (task->step == *(int *)((char *)*(int *)(g_BattleSpecialFascinationColor1.animTable) + 4)) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates and draws rotating projectile models for Yuwaku.
 */
// FUNCTION: FF7 0x00557fc5
void __cdecl BattleFx_SpecialFascinationTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        SVECTOR angleX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleFxFascinationSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_2C[0x2C]; short spawnIndex; };
        struct { unsigned char pad_to_3C[0x3C]; BattleFxFascinationSlot *taskSlot; };
        struct { unsigned char pad_to_44[0x44]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_68[0x68]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.taskSlot = (BattleFxFascinationSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.scaleVec.vz = frame.taskSlot->radius * 0x12 >> 3;
    frame.scaleVec.vy = frame.scaleVec.vz;
    frame.scaleVec.vx = frame.scaleVec.vy;
    frame.angleX.vx = 0xFC00 /* 5670 deg */;
    frame.angleX.vy = frame.taskSlot->angle;
    frame.angleX.vz = 0;
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, &frame.scaleVec);
    frame.rotMatrix.t[0] = frame.taskSlot->posX + (SinFixed((frame.taskSlot->phase << 12) / 20) >> 2);
    frame.rotMatrix.t[1] = frame.taskSlot->posY;
    frame.rotMatrix.t[2] = frame.taskSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialFascinationModelReq.flags = 0x88;
    (g_BattleSpecialFascinationModelReq.modelHandle) = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialFascinationLoadedRes);
    g_BattleSpecialFascinationDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialFascinationModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialFascinationDrawListHandle);
    frame.scaleVec.vz = frame.taskSlot->radius;
    frame.scaleVec.vy = frame.scaleVec.vz;
    frame.scaleVec.vx = frame.scaleVec.vy;
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, &frame.scaleVec);
    frame.rotMatrix.t[0] = frame.taskSlot->posX + (SinFixed((frame.taskSlot->phase << 12) / 20) >> 2);
    frame.rotMatrix.t[1] = frame.taskSlot->posY;
    frame.rotMatrix.t[2] = frame.taskSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    g_BattleSpecialFascinationModelReq.flags = 0x80;
    (g_BattleSpecialFascinationModelReq.modelHandle) = Gfx_ModelTableGetRenderObject(((int *)&g_BattleSpecialFascinationModelIndices)[frame.taskSlot->frameCounter % 7], g_BattleSpecialFascinationLoadedRes);
    g_BattleSpecialFascinationDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialFascinationModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialFascinationDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.taskSlot->frameCounter < 20) {
        frame.taskSlot->angle += 0x80;
        (frame.taskSlot->phase)++;
        frame.taskSlot->posX += frame.taskSlot->velocityX;
        frame.taskSlot->posZ += frame.taskSlot->velocityZ;
        for (frame.spawnIndex = 0; frame.spawnIndex < 2; frame.spawnIndex++) {
            frame.spawnSlot = (BattleFxFascinationSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFascinationSineSpriteTask);
            frame.spawnSlot->posX = frame.taskSlot->posX + rand() % 400 + -200;
            frame.spawnSlot->posY = frame.taskSlot->posY + rand() % 400 + -200;
            frame.spawnSlot->posZ = frame.taskSlot->posZ + rand() % 400 + -200;
            frame.spawnSlot->variant = (rand() % 4);
            frame.spawnSlot->sinePhase = rand();
        }
    }
    if (frame.taskSlot->frameCounter >= 20) {
        frame.taskSlot->angle += 0x200 /* 45 deg */;
        frame.taskSlot->posY += 40;
        frame.taskSlot->radius -= 200;
        for (frame.spawnIndex = 0; frame.spawnIndex < 2; frame.spawnIndex++) {
            frame.spawnSlot = (BattleFxFascinationSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFascinationRisingVariantSpriteTask);
            g_BattleSpecialFascinationAngle = (rand() % 4096);
            frame.spawnSlot->posX = frame.taskSlot->posX + (SinFixed(g_BattleSpecialFascinationAngle) * frame.taskSlot->radius / 12 >> 12);
            frame.spawnSlot->posZ = frame.taskSlot->posZ + (CosFixed(g_BattleSpecialFascinationAngle) * frame.taskSlot->radius / 12 >> 12);
            frame.spawnSlot->posY = frame.taskSlot->posY + rand() % 200 + -100;
            frame.spawnSlot->variant = (rand() % 4);
        }
    }
    (frame.taskSlot->frameCounter)++;
    if (frame.taskSlot->frameCounter == 40) {
        frame.taskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates and renders sinusoidal floating sprites for Yuwaku.
 */
// FUNCTION: FF7 0x0055848f
void __cdecl BattleFx_SpecialFascinationSineSpriteTask(void)
{
    BattleFxFascinationSineTask *p;
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    p = (BattleFxFascinationSineTask *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix((void *)((int)p + 4), 0x2000 /* 2.0 */, 0);
    g_BattleSpecialFascinationColor2.drawObj = Gfx_ModelTableGetSpriteObject(((int *)&g_BattleSpecialFascinationSpriteIndices1)[p->spriteIndex], g_BattleSpecialFascinationLoadedRes);
    (g_BattleSpecialFascinationColor2.animTable) = Gfx_ModelTableGetLoadedRecord(((int *)&g_BattleSpecialFascinationSpriteIndices1)[p->spriteIndex], g_BattleSpecialFascinationLoadedRes);
    g_BattleSpecialFascinationColor2.yuwakuSineFrameIndex = p->frame % *(int *)((char *)*(int *)(g_BattleSpecialFascinationColor2.animTable) + 4);
    g_BattleSpecialFascinationDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialFascinationColor2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialFascinationDrawListHandle);
    if (g_BattleIsPaused == 0) {
        (p->phase)++;
        p->sineOffset += (short)(SinFixed(p->phase << 9) * 0x32 >> 12);
        (p->frame)++;
        if (p->frame == *(int *)((char *)*(int *)(g_BattleSpecialFascinationColor2.animTable) + 4) << 1) {
            p->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Updates and renders vertically rising variant sprites for Yuwaku.
 */
// FUNCTION: FF7 0x005585bd
void __cdecl BattleFx_SpecialFascinationRisingVariantSpriteTask(void)
{
    BattleFxFascinationRisingSprite *task;

    task = (BattleFxFascinationRisingSprite *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), 0x2000 /* 2.0 */, 0);
    g_BattleSpecialFascinationColor3.yuwakuRisingFrameWord = task->frameIndex;
    g_BattleSpecialFascinationColor3.drawObj = Gfx_ModelTableGetSpriteObject(((int *)&g_BattleSpecialFascinationSpriteIndices2)[task->requestIndex], g_BattleSpecialFascinationLoadedRes);
    (g_BattleSpecialFascinationColor3.animTable) = Gfx_ModelTableGetLoadedRecord(((int *)&g_BattleSpecialFascinationSpriteIndices2)[task->requestIndex], g_BattleSpecialFascinationLoadedRes);
    g_BattleSpecialFascinationDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialFascinationColor3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialFascinationDrawListHandle);
    if (g_BattleIsPaused == 0) {
        task->angle += 0xB4;
        (task->frameIndex)++;
        if (task->frameIndex == *(int *)((char *)*(int *)(g_BattleSpecialFascinationColor3.animTable) + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
