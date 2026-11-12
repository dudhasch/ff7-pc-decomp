// src/ff7/src/battle/battle_fx_special_energy_siphon.cpp
/* Enemy Special Effect [idx 65]: EnergySiphon (JP: kyuu2 / 吸収 (q_shu, Absorption/Siphon), asset: "SPECIAL/KYUU2") — Energy Siphon (0x200, Sneaky Step), Aspil (0x188, Hammer Blaster), Magic Cutter (0x229), Dance (0x263), Eyesight (0x275); also GeneralDispatchTable[42] */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the second Kyuu attack: loads its resources, spawns
//   per-target spheres, and drives sphere collapse and particle convergence through spin model,
//   spiral, orbit, looping and burst sprite tasks.

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
// GLOBAL: FF7 0x008a70b0
int g_BattleSpecialEnergySiphonResourceDefs = 0x001900B1;
char *g_BattleSpecialEnergySiphonModelNames[1] = {
    // STRING: FF7 0x008a7180
    "kyuu.rsd"
};
unsigned int g_BattleSpecialEnergySiphonResourceRecords[24] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a718c
    (unsigned int)"q_shu02.tim",
    // STRING: FF7 0x008a7198
    (unsigned int)"kira1.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a71a4
    (unsigned int)"q_shu02.tim",
    // STRING: FF7 0x008a71b0
    (unsigned int)"kira2.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a71bc
    (unsigned int)"q_shu03.tim",
    // STRING: FF7 0x008a71c8
    (unsigned int)"titama.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a71d4
    (unsigned int)"q_shu01.tim",
    // STRING: FF7 0x008a71e0
    (unsigned int)"hit1.spt",
    0x00000000,
    0x00000000,
    0x00000000
};
// The entry-4 spark request, then the looping glow request.
// GLOBAL: FF7 0x008a7118
BattleSpecialEnergySiphonSpriteRequest g_BattleSpecialEnergySiphonSpriteReq1[2] = {
    {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}},
    {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}}
};
// GLOBAL: FF7 0x008a7138
BattleSpecialEnergySiphonSpriteReq2 g_BattleSpecialEnergySiphonSpriteReq2 = {
    {1, 2},
    {
        {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}},
        {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}},
        {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}}
    }
};
// GLOBAL: FF7 0x008a7170
int g_BattleSpecialEnergySiphonScale1 = 12288;
unsigned char g_BattleSpecialEnergySiphonScaleY[4] = {0x00, 0x30, 0x00, 0x00};
unsigned char g_BattleSpecialEnergySiphonScaleZ[8] = {0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b6f09c
int g_BattleSpecialEnergySiphonDrawListHandle = 0;
// GLOBAL: FF7 0x00b6f0a0
short g_BattleSpecialEnergySiphonState = 0;
// GLOBAL: FF7 0x00b6f0a4
struct GfxTableCursor *g_BattleSpecialEnergySiphonLoadedRes = 0;
// clang-format on

/*
 * Loads and registers resources for the Kyuu2 special effect.
 */
// FUNCTION: FF7 0x00551b80
void __cdecl BattleFx_SpecialLoadResEnergySiphon(void)
{
    GfxModelTableBank *stageSceneHandle;
    GfxResourceRequest requestObj;
    int resourceCount;
    char pathWorkBuffer[200];
    GfxResourceRequest *reqPtrPtr;
    int resourceTable;

    g_BattleSpecialEnergySiphonState = 0;
    if (g_pBattle3dScene != 0 && g_BattleSpecialEnergySiphonLoadedRes == 0) {
        resourceCount = 5;
        resourceTable = (int)&g_BattleSpecialEnergySiphonResourceDefs;
        stageSceneHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        reqPtrPtr = &requestObj;
        Gfx_InitResourceRequest(reqPtrPtr);
        reqPtrPtr->groupTag = 0;
        // STRING: FF7 0x008a71ec
        reqPtrPtr->resourcePath = BattleFx_BuildResourcePath("SPECIAL/KYUU2", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(resourceCount, resourceTable, reqPtrPtr, &g_BattleSpecialEnergySiphonLoadedRes, stageSceneHandle);
    }
}

/*
 * Entry point thunk for the Kyuu2 special effect.
 */
// FUNCTION: FF7 0x00551c19
void __cdecl BattleFx_SpecialEnterEnergySiphon(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResEnergySiphon();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnEnergySiphonPerTarget);
    Battle_PlaySfxPannedToActor(0x7E, (short)actorIdx, 0x40);
}

/*
 * Spawns per-target Kyuu2 timeline and absorption tasks.
 */
// FUNCTION: FF7 0x00551c4b
void __cdecl BattleFx_SpecialSpawnEnergySiphonPerTarget(int actor, int casterIndex)
{
    BattleFxEnergySiphonTimeline *effect;

    effect = (BattleFxEnergySiphonTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergySiphonTimelineTask);
    Battle_GetBoneWorldPos(casterIndex, g_BattleActorRecords[casterIndex].targetBone, &effect->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &effect->endX);
    effect->targetIndex = actor;
    effect->casterIndex = casterIndex;
}

/*
 * Main timeline task controlling sphere collapse and particle convergence.
 */
// FUNCTION: FF7 0x00551ccc
void __cdecl BattleFx_SpecialEnergySiphonTimelineTask(void)
{
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short i8;
    BattleFxEnergySiphonTimeline *taskPtr;
    BattleFxDrainTimelineState *spawned;

    taskPtr = (BattleFxEnergySiphonTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->timer == 0) {
        spawned = (BattleFxDrainTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergySiphonSpinModelTask);
        spawned->posX = taskPtr->startX;
        spawned->posY = taskPtr->startY - 400;
        spawned->posZ = taskPtr->startZ;
        spawned->deltaX = (taskPtr->endX - spawned->posX) / 20;
        spawned->deltaY = (taskPtr->endY - 800 - spawned->posY) / 20;
        spawned->deltaZ = (taskPtr->endZ - spawned->posZ) / 20;
    }
    if (taskPtr->timer == 90) {
        Battle_PostActorHitOrHurtMessage(taskPtr->targetIndex);
        Battle_PlaySfxPannedToActor(0x1F, (short)taskPtr->casterIndex, 0x40);
    }
    if (taskPtr->timer > 86 && taskPtr->timer < 102) {
        for (i8 = 0; i8 < 4; i8++) {
            spawned = (BattleFxDrainTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergySiphonAnimatedSpriteEntry4Task);
            Battle_GetBoneWorldPos(taskPtr->casterIndex,
                                   rand() % g_BattleActorRecords[taskPtr->casterIndex].boneCount,
                                   &spawned->posX);
            spawned->posX += (short)(rand() % 600 - 300);
            spawned->posY += (short)(rand() % 600 - 300);
            spawned->posZ += (short)(rand() % 600 - 300);
        }
    }
    (taskPtr->timer)++;
    if (taskPtr->timer == 110) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animates impact spark sprites for Kyuu2.
 */
// FUNCTION: FF7 0x00551f0c
void __cdecl BattleFx_SpecialEnergySiphonAnimatedSpriteEntry4Task(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialEnergySiphonSpriteReq1[0].spriteResource = Gfx_ModelTableGetLoadedRecord(4, g_BattleSpecialEnergySiphonLoadedRes);
    g_BattleSpecialEnergySiphonSpriteReq1[0].spriteObject = Gfx_ModelTableGetSpriteObject(4, g_BattleSpecialEnergySiphonLoadedRes);
    BattleFx_InstallBillboardMatrix((short *)task->data, 0x2000 /* 2.0 */, 0);
    g_BattleSpecialEnergySiphonSpriteReq1[0].frameIndex = task->frameCounter;
    g_BattleSpecialEnergySiphonDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialEnergySiphonSpriteReq1[0], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergySiphonDrawListHandle);
    if (g_BattleIsPaused == 0 && g_BattleSpecialEnergySiphonSpriteReq1[0].spriteResource != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= ((BattleFxSpriteGroup *)g_BattleSpecialEnergySiphonSpriteReq1[0].spriteResource)->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Animates spinning sphere model converging on target.
 */
// FUNCTION: FF7 0x00551fdd
void __cdecl BattleFx_SpecialEnergySiphonSpinModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        SVECTOR angleX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleFxDrainSpinModelSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_2C[0x2C]; short spawnIndex; };
        struct { unsigned char pad_to_3C[0x3C]; BattleFxHellBomberRocketSlot *slot; };
        struct { unsigned char pad_to_44[0x44]; PsxMatrix modelMatrix; };
        struct { unsigned char pad_to_68[0x68]; int modelPtr; };
        struct { unsigned char pad_to_6C[0x6C]; int modelParamB; };
        struct { unsigned char pad_to_70[0x70]; short modelParamC; };
        struct { unsigned char pad_to_72[0x72]; short modelState; };
        struct { unsigned char pad_to_74[0x74]; short modelParamD; };
        struct { unsigned char pad_to_76[0x76]; short modelFrame; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxHellBomberRocketSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.modelPtr = 0;
    frame.modelParamB = 8;
    frame.modelParamC = 0;
    frame.modelState = 0;
    frame.modelParamD = 0x20;
    frame.modelFrame = 0;
    frame.modelPtr = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialEnergySiphonLoadedRes);
    frame.angleX.vz = 0;
    frame.angleX.vx = frame.angleX.vz;
    frame.angleX.vy = frame.slot->frameCounter << 7;
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialEnergySiphonScale1);
    frame.rotMatrix.t[0] = frame.slot->posX;
    frame.rotMatrix.t[1] = frame.slot->posY;
    frame.rotMatrix.t[2] = frame.slot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    frame.modelState = g_BattleSpecialEnergySiphonState;
    frame.modelFrame = frame.slot->frameCounter % 5 * 3 << 6;
    g_BattleSpecialEnergySiphonDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&frame.modelPtr, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergySiphonDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.slot->frameCounter < 20) {
        frame.slot->posX += frame.slot->velocityX;
        frame.slot->posY += frame.slot->velocityY;
        frame.slot->posZ += frame.slot->velocityZ;
        g_BattleSpecialEnergySiphonState += 6;
    }
    if (frame.slot->frameCounter >= 20 && frame.slot->frameCounter < 50) {
        for (frame.spawnIndex = 0; frame.spawnIndex < 1; frame.spawnIndex++) {
            frame.spawnSlot = (BattleFxDrainSpinModelSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergySiphonSpiralSpriteTask);
            frame.spawnSlot->posX = frame.slot->posX;
            frame.spawnSlot->posY = frame.slot->posY;
            frame.spawnSlot->posZ = frame.slot->posZ;
            frame.spawnSlot->angle = rand() & 0xFFF;
            frame.spawnSlot->weightA = 0x3E8;
            frame.spawnSlot->weightB = 0x3E8;
        }
    }
    if (frame.slot->frameCounter >= 38 && frame.slot->frameCounter < 44 && (frame.slot->frameCounter % 2) == 0) {
        frame.spawnSlot = (BattleFxDrainSpinModelSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergySiphonOrbitSpriteTask);
        frame.spawnSlot->posX = frame.slot->posX;
        frame.spawnSlot->posY = frame.slot->posY - 500;
        frame.spawnSlot->posZ = frame.slot->posZ;
        frame.spawnSlot->velocityX = -frame.slot->velocityX << 1;
        frame.spawnSlot->velocityY = -frame.slot->velocityY << 1;
        frame.spawnSlot->velocityZ = -frame.slot->velocityZ << 1;
        frame.spawnSlot->scalePhase = 0x100 /* 0.0625 */;
        frame.spawnSlot->weightB = 0;
        frame.spawnSlot->weightA = 0;
        frame.spawnSlot->scaleDivisor = ((frame.slot->frameCounter - 0x26) / 2) + 5;
    }
    if (frame.slot->frameCounter > 50) {
        g_BattleSpecialEnergySiphonState -= 6;
        if (g_BattleSpecialEnergySiphonState == 0) {
            frame.slot->ownerSlot = BATTLE_TASK_FREE;
        }
    }
    (frame.slot->frameCounter)++;
}

/*
 * Animates spiraling particle sprites converging into the sphere.
 */
// FUNCTION: FF7 0x00552349
void __cdecl BattleFx_SpecialEnergySiphonSpiralSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[12];
        SVECTOR spriteX;
        struct { unsigned char pad_to_8[0x8]; BattleFxDrainSpiralSpriteSlot *slot; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxDrainSpiralSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialEnergySiphonSpriteReq2.request[0].spriteResource = Gfx_ModelTableGetLoadedRecord(2, g_BattleSpecialEnergySiphonLoadedRes);
    g_BattleSpecialEnergySiphonSpriteReq2.request[0].spriteObject = Gfx_ModelTableGetSpriteObject(2, g_BattleSpecialEnergySiphonLoadedRes);
    frame.spriteX.vx = frame.slot->posX + (SinFixed(frame.slot->angle) * frame.slot->radius >> 12);
    frame.spriteX.vz = frame.slot->posZ + (CosFixed(frame.slot->angle) * frame.slot->radius >> 12);
    frame.spriteX.vy = frame.slot->posY + frame.slot->velocityY;
    BattleFx_InstallBillboardMatrix(&frame.spriteX, 0x2000 /* 2.0 */, 0);
    if (g_BattleSpecialEnergySiphonSpriteReq2.request[0].spriteResource != 0) {
        g_BattleSpecialEnergySiphonSpriteReq2.request[0].frameIndex = frame.slot->frameCounter % ((BattleFxSpriteGroup *)g_BattleSpecialEnergySiphonSpriteReq2.request[0].spriteResource)->header->frameCount;
    }
    g_BattleSpecialEnergySiphonDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialEnergySiphonSpriteReq2.request[0], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergySiphonDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.slot->radius -= (short)(frame.slot->radius / 10);
    frame.slot->velocityY -= 50;
    frame.slot->angle += 0x80;
    (frame.slot->frameCounter)++;
    if (frame.slot->frameCounter == 20) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animates orbiting particle sprites swirling around the target.
 */
// FUNCTION: FF7 0x005524ce
void __cdecl BattleFx_SpecialEnergySiphonOrbitSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[20];
        SVECTOR spriteX;
        struct { unsigned char pad_to_8[0x8]; BattleFxEnergySiphonOrbitSpriteSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_10[0x10]; BattleFxEnergySiphonOrbitSpriteSlot *slot; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxEnergySiphonOrbitSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialEnergySiphonSpriteReq2.request[2].spriteResource = Gfx_ModelTableGetLoadedRecord(3, g_BattleSpecialEnergySiphonLoadedRes);
    g_BattleSpecialEnergySiphonSpriteReq2.request[2].spriteObject = Gfx_ModelTableGetSpriteObject(3, g_BattleSpecialEnergySiphonLoadedRes);
    frame.spriteX.vx = frame.slot->posX + (SinFixed(frame.slot->angle) * frame.slot->radius >> 12);
    frame.spriteX.vy = frame.slot->posY + (CosFixed(frame.slot->angle) * frame.slot->radius >> 12);
    frame.spriteX.vz = frame.slot->posZ;
    BattleFx_InstallBillboardMatrix(&frame.spriteX, frame.slot->phase * 5 / frame.slot->scaleDivisor, 0);
    if (g_BattleSpecialEnergySiphonSpriteReq2.request[2].spriteResource != 0) {
        g_BattleSpecialEnergySiphonSpriteReq2.request[2].frameIndex = frame.slot->frameCounter % ((BattleFxSpriteGroup *)g_BattleSpecialEnergySiphonSpriteReq2.request[2].spriteResource)->header->frameCount;
    }
    g_BattleSpecialEnergySiphonDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialEnergySiphonSpriteReq2.request[2], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergySiphonDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.slot->frameCounter < 30) {
        frame.slot->phase += 0xC0;
    }
    if (frame.slot->frameCounter >= 30 && frame.slot->frameCounter < 40) {
        frame.slot->posX += frame.slot->velocityX;
        frame.slot->posY += frame.slot->velocityY;
        frame.slot->posZ += frame.slot->velocityZ;
        frame.slot->pulseAngle += 0xCC;
        frame.slot->radius = SinFixed(frame.slot->pulseAngle) * 0x190 >> 12;
        frame.slot->angle += 0x200 /* 45 deg */;
        frame.spawnSlot = (BattleFxEnergySiphonOrbitSpriteSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergySiphonAnimatedSpriteEntry1Task);
        frame.spawnSlot->posX = frame.slot->posX + rand() % 800 + -400;
        frame.spawnSlot->posY = frame.slot->posY + rand() % 800 + -400;
        frame.spawnSlot->posZ = frame.slot->posZ + rand() % 800 + -400;
    }
    if (frame.slot->frameCounter > 40) {
        frame.slot->phase -= 0x100;
        frame.spawnSlot = (BattleFxEnergySiphonOrbitSpriteSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergySiphonVariantLoopingSpriteTask);
        frame.spawnSlot->posX = frame.slot->posX + rand() % 600 + -300;
        frame.spawnSlot->posY = frame.slot->posY + rand() % 600 + -300;
        frame.spawnSlot->posZ = frame.slot->posZ + rand() % 600 + -300;
        frame.spawnSlot->variant = rand() & 1;
    }
    (frame.slot->frameCounter)++;
    if (frame.slot->frameCounter == 60) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animates looping glow sprites for Kyuu2.
 */
// FUNCTION: FF7 0x0055280f
void __cdecl BattleFx_SpecialEnergySiphonVariantLoopingSpriteTask(void)
{
    BattleFxEnergySiphonLoopingSprite *task;

    task = (BattleFxEnergySiphonLoopingSprite *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix(&task->posX, 0x2000 /* 2.0 */, 0);
    g_BattleSpecialEnergySiphonSpriteReq1[1].spriteResource = Gfx_ModelTableGetLoadedRecord(g_BattleSpecialEnergySiphonSpriteReq2.variantEntries[task->variant], g_BattleSpecialEnergySiphonLoadedRes);
    g_BattleSpecialEnergySiphonSpriteReq1[1].spriteObject = Gfx_ModelTableGetSpriteObject(g_BattleSpecialEnergySiphonSpriteReq2.variantEntries[task->variant], g_BattleSpecialEnergySiphonLoadedRes);
    if (g_BattleSpecialEnergySiphonSpriteReq1[1].spriteResource != 0) {
        g_BattleSpecialEnergySiphonSpriteReq1[1].frameIndex = task->frame % ((BattleFxSpriteGroup *)g_BattleSpecialEnergySiphonSpriteReq1[1].spriteResource)->header->frameCount;
    }
    g_BattleSpecialEnergySiphonDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialEnergySiphonSpriteReq1[1], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergySiphonDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posY += 100;
    (task->frame)++;
    if (task->frame == 10) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animates entry burst sprites for Kyuu2.
 */
// FUNCTION: FF7 0x00552911
void __cdecl BattleFx_SpecialEnergySiphonAnimatedSpriteEntry1Task(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialEnergySiphonSpriteReq2.request[1].spriteResource = Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecialEnergySiphonLoadedRes);
    g_BattleSpecialEnergySiphonSpriteReq2.request[1].spriteObject = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecialEnergySiphonLoadedRes);
    BattleFx_InstallBillboardMatrix((short *)task->data, 0x2000 /* 2.0 */, 0);
    g_BattleSpecialEnergySiphonSpriteReq2.request[1].frameIndex = task->frameCounter;
    g_BattleSpecialEnergySiphonDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialEnergySiphonSpriteReq2.request[1], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergySiphonDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (g_BattleSpecialEnergySiphonSpriteReq2.request[1].spriteResource != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= ((BattleFxSpriteGroup *)g_BattleSpecialEnergySiphonSpriteReq2.request[1].spriteResource)->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
