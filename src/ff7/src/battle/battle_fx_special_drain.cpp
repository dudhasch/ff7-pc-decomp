// src/ff7/src/battle/battle_fx_special_drain.cpp
/* Enemy Special Effect [idx 23]: Drain (JP: kyuu / 吸血 (q_kets, Blood Suck/Drain), asset: "SPECIAL/KYUU") — Drain (0x14d, Eligor), Dance (0x264, 0x265), Eyesight (0x277, Demon's Gate); also GeneralDispatchTable[41] */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Kyuu attack: loads its resources, spawns per-target
//   absorption tasks, and drives sphere formation and energy absorption through spark, spin model,
//   spiral, orbiting and rising sprite tasks.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle_3d.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x008a7540
int g_BattleSpecialDrainResourceDefs = 0x001900B1;
char *g_BattleFxSpecialDrainResourceNames[1] = {
    // STRING: FF7 0x008a7610
    "kyuu.rsd"
};
unsigned int g_BattleSpecialDrainResourceDefsDataTail[24] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a761c
    (unsigned int)"q_kets02.tim",
    // STRING: FF7 0x008a762c
    (unsigned int)"kira1.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a7638
    (unsigned int)"q_kets02.tim",
    // STRING: FF7 0x008a7648
    (unsigned int)"kira2.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a7654
    (unsigned int)"q_kets03.tim",
    // STRING: FF7 0x008a7664
    (unsigned int)"titama.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a7670
    (unsigned int)"q_kets01.tim",
    // STRING: FF7 0x008a7680
    (unsigned int)"hit1.spt",
    0x00000000,
    0x00000000,
    0x00000000
};
#undef g_BattleSpecialDrainSpriteReq1
// GLOBAL: FF7 0x008a75a8
struct BattleSpecialDrainSpriteReq1 g_BattleSpecialDrainSpriteReq1 = {{0, 0, -128, -128, -128, 0x2c, 0, 0}, {0, 0, -128, -128, -128, 0x2c, 0, 0}};
#undef g_BattleSpecialDrainSpriteReq2
// GLOBAL: FF7 0x008a75c8
struct BattleSpecialDrainSpriteReq2 g_BattleSpecialDrainSpriteReq2 = {{1, 2}, {0, 0, -128, -128, -128, 0x2c, 0, 0}, {0, 0, -128, -128, -128, 0x2c, 0, 0}, {0, 0, -128, -128, -128, 0x2c, 0, 0}};
// GLOBAL: FF7 0x008a7600
int g_BattleSpecialDrainScale1 = 12288;
unsigned char g_rawData_008a7604[4] = {0x00, 0x30, 0x00, 0x00};
unsigned char g_rawData_008a7608[8] = {0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b6f0c4
int g_BattleSpecialDrainDrawListHandle = 0;
// GLOBAL: FF7 0x00b6f0c8
short g_BattleSpecialDrainState = 0;
// GLOBAL: FF7 0x00b6f0cc
struct GfxTableCursor *g_BattleSpecialDrainLoadedRes = 0;
// clang-format on

/*
 * Loads and registers resources for the Kyuu special effect.
 */
// FUNCTION: FF7 0x00554010
void __cdecl BattleFx_SpecialLoadResDrain(void)
{
    GfxModelTableBank *newSceneLink;
    GfxResourceRequest resultPending;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *reqPtrPtr;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecialDrainLoadedRes == 0) {
        kind = 5;
        resourceDefsWord = (int)&g_BattleSpecialDrainResourceDefs;
        newSceneLink = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        reqPtrPtr = &resultPending;
        Gfx_InitResourceRequest(reqPtrPtr);
        reqPtrPtr->groupTag = 0;
        // STRING: FF7 0x008a768c
        reqPtrPtr->resourcePath = BattleFx_BuildResourcePath("SPECIAL/KYUU", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, reqPtrPtr, &g_BattleSpecialDrainLoadedRes, newSceneLink);
    }
}

/*
 * Entry point thunk for the Kyuu special effect.
 */
// FUNCTION: FF7 0x005540a0
void __cdecl BattleFx_SpecialEnterDrain(int targetMask, int actorIdx)
{
    g_BattleSpecialDrainState = 0;
    BattleFx_SpecialLoadResDrain();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnDrainPerTarget);
    Battle_PlaySfxPannedToActor(0x7E, (short)actorIdx, 0x40);
}

/*
 * Spawns per-target Kyuu absorption tasks.
 */
// FUNCTION: FF7 0x005540db
void __cdecl BattleFx_SpecialSpawnDrainPerTarget(int actor, int attackerId)
{
    BattleFxDrainTimeline *taskPtr;

    taskPtr = (BattleFxDrainTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDrainTimelineTask);
    Battle_GetBoneWorldPos(attackerId, g_BattleActorRecords[attackerId].targetBone, (short *)((int)taskPtr + 4));
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)((int)taskPtr + 0xC));
    taskPtr->targetId = actor;
    taskPtr->attackerId = attackerId;
}

/*
 * Main timeline task controlling sphere formation and energy absorption.
 */
// FUNCTION: FF7 0x0055415c
void __cdecl BattleFx_SpecialDrainTimelineTask(void)
{
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short i8;
    BattleFxDrainTimelineRecord *taskPtr;
    BattleFxDrainTimelineState *spawned;

    taskPtr = (BattleFxDrainTimelineRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->timer == 0) {
        spawned = (BattleFxDrainTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDrainSpinModelTask);
        spawned->posX = taskPtr->startX;
        spawned->posY = taskPtr->startY - 0x190;
        spawned->posZ = taskPtr->startZ;
        spawned->deltaX = (taskPtr->endX - spawned->posX) / 0x14;
        spawned->deltaY = (taskPtr->endY - 0x320 - spawned->posY) / 0x14;
        spawned->deltaZ = (taskPtr->endZ - spawned->posZ) / 0x14;
    }
    if (taskPtr->timer == 0x5A) {
        Battle_PostActorHitOrHurtMessage(taskPtr->effectType);
        Battle_PlaySfxPannedToActor(7, (short)taskPtr->targetIndex, 0x40);
    }
    if (taskPtr->timer > 0x56 && taskPtr->timer < 0x66) {
        for (i8 = 0; i8 < 4; i8++) {
            spawned = (BattleFxDrainTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDrainAnimatedSpriteEntry4Task);
            Battle_GetBoneWorldPos(taskPtr->targetIndex,
                                   rand() % g_BattleActorRecords[taskPtr->targetIndex].boneCount,
                                   (short *)((int)spawned + 4));
            spawned->posX += (short)(rand() % 0x258 - 0x12C);
            spawned->posY += (short)(rand() % 0x258 - 0x12C);
            spawned->posZ += (short)(rand() % 0x258 - 0x12C);
        }
    }
    (taskPtr->timer)++;
    if (taskPtr->timer == 0x6E) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animates convergence spark sprites for Kyuu.
 */
// FUNCTION: FF7 0x0055439c
void __cdecl BattleFx_SpecialDrainAnimatedSpriteEntry4Task(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialDrainSpriteReq1.entry4.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(4, g_BattleSpecialDrainLoadedRes);
    g_BattleSpecialDrainSpriteReq1.entry4.drawObj = Gfx_ModelTableGetSpriteObject(4, g_BattleSpecialDrainLoadedRes);
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), 0x2000, 0);
    g_BattleSpecialDrainSpriteReq1.entry4.frame = task->frameCounter;
    g_BattleSpecialDrainDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialDrainSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialDrainDrawListHandle);
    if (g_BattleIsPaused == 0 && g_BattleSpecialDrainSpriteReq1.entry4.spriteGroup != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= g_BattleSpecialDrainSpriteReq1.entry4.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Animates spinning absorption sphere model for Kyuu.
 */
// FUNCTION: FF7 0x0055446d
void __cdecl BattleFx_SpecialDrainSpinModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleFxDrainSpinModelSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_2C[0x2C]; short spawnIndex; };
        struct { unsigned char pad_to_3C[0x3C]; BattleFxHellBomberRocketSlot *spinSlot; };
        struct { unsigned char pad_to_44[0x44]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_68[0x68]; int modelRecord; };
        struct { unsigned char pad_to_6C[0x6C]; int drawExtra; };
        struct { unsigned char pad_to_70[0x70]; short drawFlags; };
        struct { unsigned char pad_to_72[0x72]; short kyuuState; };
        struct { unsigned char pad_to_74[0x74]; short drawParamA; };
        struct { unsigned char pad_to_76[0x76]; short frameOffset; };
    } frame;
#pragma pack(pop)

    frame.spinSlot = (BattleFxHellBomberRocketSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.modelRecord = 0;
    frame.drawExtra = 8;
    frame.drawFlags = 0;
    frame.kyuuState = 0;
    frame.drawParamA = 0x20;
    frame.frameOffset = 0;
    frame.modelRecord = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialDrainLoadedRes);
    frame.rotX.vz = 0;
    frame.rotX.vx = frame.rotX.vz;
    frame.rotX.vy = frame.spinSlot->frameCounter << 7;
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialDrainScale1);
    frame.rotMatrix.t[0] = frame.spinSlot->posX;
    frame.rotMatrix.t[1] = frame.spinSlot->posY;
    frame.rotMatrix.t[2] = frame.spinSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    frame.kyuuState = g_BattleSpecialDrainState;
    frame.frameOffset = frame.spinSlot->frameCounter % 5 * 3 << 6;
    g_BattleSpecialDrainDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&frame.modelRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialDrainDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.spinSlot->frameCounter < 0x14) {
        frame.spinSlot->posX += frame.spinSlot->velocityX;
        frame.spinSlot->posY += frame.spinSlot->velocityY;
        frame.spinSlot->posZ += frame.spinSlot->velocityZ;
        g_BattleSpecialDrainState += 6;
    }
    if (frame.spinSlot->frameCounter >= 0x14 && frame.spinSlot->frameCounter < 0x32) {
        for (frame.spawnIndex = 0; frame.spawnIndex < 1; frame.spawnIndex++) {
            frame.spawnSlot = (BattleFxDrainSpinModelSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDrainSpiralSpriteTask);
            frame.spawnSlot->posX = frame.spinSlot->posX;
            frame.spawnSlot->posY = frame.spinSlot->posY;
            frame.spawnSlot->posZ = frame.spinSlot->posZ;
            frame.spawnSlot->angle = rand() & 0xFFF;
            frame.spawnSlot->weightA = 0x3E8;
            frame.spawnSlot->weightB = 0x3E8;
        }
    }
    if (frame.spinSlot->frameCounter >= 0x26 && frame.spinSlot->frameCounter < 0x2C && (frame.spinSlot->frameCounter % 2) == 0) {
        frame.spawnSlot = (BattleFxDrainSpinModelSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDrainOrbitingSpriteTask);
        frame.spawnSlot->posX = frame.spinSlot->posX;
        frame.spawnSlot->posY = frame.spinSlot->posY - 0x1F4;
        frame.spawnSlot->posZ = frame.spinSlot->posZ;
        frame.spawnSlot->velocityX = -frame.spinSlot->velocityX << 1;
        frame.spawnSlot->velocityY = -frame.spinSlot->velocityY << 1;
        frame.spawnSlot->velocityZ = -frame.spinSlot->velocityZ << 1;
        frame.spawnSlot->scalePhase = 0x100;
        frame.spawnSlot->weightB = 0;
        frame.spawnSlot->weightA = 0;
        frame.spawnSlot->scaleDivisor = ((frame.spinSlot->frameCounter - 0x26) / 2) + 5;
    }
    if (frame.spinSlot->frameCounter > 0x32) {
        g_BattleSpecialDrainState -= 6;
        if (g_BattleSpecialDrainState == 0) {
            frame.spinSlot->ownerSlot = BATTLE_TASK_FREE;
        }
    }
    (frame.spinSlot->frameCounter)++;
}

/*
 * Animates spiraling particle sprites for Kyuu.
 */
// FUNCTION: FF7 0x005547d9
void __cdecl BattleFx_SpecialDrainSpiralSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[12];
        SVECTOR spiralX;
        struct { unsigned char pad_to_8[0x8]; BattleFxDrainSpiralSpriteSlot *spiralSlot; };
    } frame;
#pragma pack(pop)

    frame.spiralSlot = (BattleFxDrainSpiralSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialDrainSpriteReq2.spiral.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(2, g_BattleSpecialDrainLoadedRes);
    g_BattleSpecialDrainSpriteReq2.spiral.drawObj = Gfx_ModelTableGetSpriteObject(2, g_BattleSpecialDrainLoadedRes);
    frame.spiralX.vx = frame.spiralSlot->posX + (SinFixed(frame.spiralSlot->angle) * frame.spiralSlot->radius >> 0xC);
    frame.spiralX.vz = frame.spiralSlot->posZ + (CosFixed(frame.spiralSlot->angle) * frame.spiralSlot->radius >> 0xC);
    frame.spiralX.vy = frame.spiralSlot->posY + frame.spiralSlot->velocityY;
    BattleFx_InstallBillboardMatrix(&frame.spiralX, 0x2000, 0);
    if (g_BattleSpecialDrainSpriteReq2.spiral.spriteGroup != 0) {
        g_BattleSpecialDrainSpriteReq2.spiral.frame = frame.spiralSlot->frameCounter % g_BattleSpecialDrainSpriteReq2.spiral.spriteGroup->header->frameCount;
    }
    g_BattleSpecialDrainDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialDrainSpriteReq2.spiral, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialDrainDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spiralSlot->radius -= (short)(frame.spiralSlot->radius / 0xA);
    frame.spiralSlot->velocityY -= 0x32;
    frame.spiralSlot->angle += 0x80;
    (frame.spiralSlot->frameCounter)++;
    if (frame.spiralSlot->frameCounter == 0x14) {
        frame.spiralSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animates orbiting particle sprites circling the sphere.
 */
// FUNCTION: FF7 0x0055495e
void __cdecl BattleFx_SpecialDrainOrbitingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[20];
        SVECTOR orbitX;
        struct { unsigned char pad_to_8[0x8]; BattleFxRadialLivewireSlot *spawnSlot; };
        struct { unsigned char pad_to_10[0x10]; BattleFxDrainOrbitingSpriteSlot *orbitSlot; };
    } frame;
#pragma pack(pop)

    frame.orbitSlot = (BattleFxDrainOrbitingSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialDrainSpriteReq2.orbit.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(3, g_BattleSpecialDrainLoadedRes);
    g_BattleSpecialDrainSpriteReq2.orbit.drawObj = Gfx_ModelTableGetSpriteObject(3, g_BattleSpecialDrainLoadedRes);
    frame.orbitX.vx = frame.orbitSlot->posX + (SinFixed(frame.orbitSlot->angle) * frame.orbitSlot->radius >> 0xC);
    frame.orbitX.vy = frame.orbitSlot->posY + (CosFixed(frame.orbitSlot->angle) * frame.orbitSlot->radius >> 0xC);
    frame.orbitX.vz = frame.orbitSlot->posZ;
    BattleFx_InstallBillboardMatrix(&frame.orbitX, frame.orbitSlot->scale * 5 / frame.orbitSlot->scaleDivisor, 0);
    if (g_BattleSpecialDrainSpriteReq2.orbit.spriteGroup != 0) {
        g_BattleSpecialDrainSpriteReq2.orbit.frame = frame.orbitSlot->frameCounter % g_BattleSpecialDrainSpriteReq2.orbit.spriteGroup->header->frameCount;
    }
    g_BattleSpecialDrainDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialDrainSpriteReq2.orbit, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialDrainDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.orbitSlot->frameCounter < 0x1E) {
        frame.orbitSlot->scale += 0xC0;
    }
    if (frame.orbitSlot->frameCounter >= 0x1E && frame.orbitSlot->frameCounter < 0x28) {
        frame.orbitSlot->posX += frame.orbitSlot->velocityX;
        frame.orbitSlot->posY += frame.orbitSlot->velocityY;
        frame.orbitSlot->posZ += frame.orbitSlot->velocityZ;
        frame.orbitSlot->pulseAngle += 0xCC;
        frame.orbitSlot->radius = SinFixed(frame.orbitSlot->pulseAngle) * 0x190 >> 0xC;
        frame.orbitSlot->angle += 0x200;
        frame.spawnSlot = (BattleFxRadialLivewireSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDrainAnimatedSpriteEntry1Task);
        frame.spawnSlot->posX = frame.orbitSlot->posX + rand() % 0x320 + -0x190;
        frame.spawnSlot->posY = frame.orbitSlot->posY + rand() % 0x320 + -0x190;
        frame.spawnSlot->posZ = frame.orbitSlot->posZ + rand() % 0x320 + -0x190;
    }
    if (frame.orbitSlot->frameCounter > 0x28) {
        frame.orbitSlot->scale -= 0x100;
        frame.spawnSlot = (BattleFxRadialLivewireSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDrainRisingLoopingSpriteTask);
        frame.spawnSlot->posX = frame.orbitSlot->posX + rand() % 0x258 + -0x12C;
        frame.spawnSlot->posY = frame.orbitSlot->posY + rand() % 0x258 + -0x12C;
        frame.spawnSlot->posZ = frame.orbitSlot->posZ + rand() % 0x258 + -0x12C;
        frame.spawnSlot->scaleOrVariant = rand() & 1;
    }
    (frame.orbitSlot->frameCounter)++;
    if (frame.orbitSlot->frameCounter == 0x3C) {
        frame.orbitSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animates rising and looping particle sprites for Kyuu.
 */
// FUNCTION: FF7 0x00554c9f
void __cdecl BattleFx_SpecialDrainRisingLoopingSpriteTask(void)
{
    BattleFxDrainRisingSprite *task;

    task = (BattleFxDrainRisingSprite *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), 0x2000, 0);
    g_BattleSpecialDrainSpriteReq1.rising.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(g_BattleSpecialDrainSpriteReq2.risingEntry[task->requestIndex], g_BattleSpecialDrainLoadedRes);
    g_BattleSpecialDrainSpriteReq1.rising.drawObj = Gfx_ModelTableGetSpriteObject(g_BattleSpecialDrainSpriteReq2.risingEntry[task->requestIndex], g_BattleSpecialDrainLoadedRes);
    if (g_BattleSpecialDrainSpriteReq1.rising.spriteGroup != 0) {
        g_BattleSpecialDrainSpriteReq1.rising.frame = task->frameIndex % g_BattleSpecialDrainSpriteReq1.rising.spriteGroup->header->frameCount;
    }
    g_BattleSpecialDrainDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialDrainSpriteReq1.rising, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialDrainDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->angle += 0x64;
    (task->frameIndex)++;
    if (task->frameIndex == 0xA) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animates initial burst sprites for Kyuu.
 */
// FUNCTION: FF7 0x00554da1
void __cdecl BattleFx_SpecialDrainAnimatedSpriteEntry1Task(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialDrainSpriteReq2.burst.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecialDrainLoadedRes);
    g_BattleSpecialDrainSpriteReq2.burst.drawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecialDrainLoadedRes);
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), 0x2000, 0);
    g_BattleSpecialDrainSpriteReq2.burst.frame = task->frameCounter;
    g_BattleSpecialDrainDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialDrainSpriteReq2.burst, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialDrainDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (g_BattleSpecialDrainSpriteReq2.burst.spriteGroup != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= g_BattleSpecialDrainSpriteReq2.burst.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
