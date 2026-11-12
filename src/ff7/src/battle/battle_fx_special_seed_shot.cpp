// src/ff7/src/battle/battle_fx_special_seed_shot.cpp
/* Enemy Special Effect [idx 7]: SeedShot (JP: tane / 種 (Seed), asset: "SPECIAL/TANE") — Seed Shot (0x1df, Search Crown) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Tane attack: loads resources, spawns per-target
//   timelines, and drives scattered impact sprites and bouncing seed and fruit models.

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

struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x008a86d0
int g_BattleSpecialSeedShotResourceDefs = 0x001900B4;
char *g_BattleFxSpecialSeedShotResourceNames[1] = {
    // STRING: FF7 0x008a8750
    "tane1.rsd"
};
unsigned int g_BattleSpecialSeedShotResourceDefsDataTail[14] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b4,
    // STRING: FF7 0x008a875c
    (unsigned int)"tane2.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a8768
    (unsigned int)"hit1.tim",
    // STRING: FF7 0x008a8774
    (unsigned int)"hit1.spt",
    0x00000000,
    0x00000000,
    0x00000000
};
#undef g_BattleSpecialSeedShotSpriteData
// GLOBAL: FF7 0x008a8710
struct BattleFxSpriteRequest g_BattleSpecialSeedShotSpriteData = {0, 0, -128, -128, -128, 0x2c, 0, 0};
// GLOBAL: FF7 0x008a8720
struct BattleSpecialSeedShotModelConfig g_BattleSpecialSeedShotModelConfig = {{0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0}, 0, {0}, 0, {0x20, 0}, 0, 0};
unsigned char g_rawData_008a8748[8] = {0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b72f3c
int g_BattleSpecialSeedShotDrawListHandle = 0;
// GLOBAL: FF7 0x00b72f40
struct GfxTableCursor *g_BattleSpecialSeedShotLoadedRes = 0;
unsigned char g_bssPad_00b72f44[4] = {0};
// clang-format on

/*
 * Loads resources for the Tane special effect when not already loaded.
 */
// FUNCTION: FF7 0x0055d240
void __cdecl BattleFx_SpecialLoadResSeedShot(void)
{
    GfxModelTableBank *ownerHandle;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecialSeedShotLoadedRes == 0) {
        kind = 3;
        resourceDefsWord = (int)&g_BattleSpecialSeedShotResourceDefs;
        ownerHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a8780
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/TANE", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecialSeedShotLoadedRes, ownerHandle);
    }
}

/*
 * Main entry point for the Tane special effect.
 */
// FUNCTION: FF7 0x0055d2d0
void __cdecl BattleFx_SpecialEnterSeedShot(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResSeedShot();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnSeedShotPerTarget);
    Battle_PlaySfxPannedToActor(0x64, (short)actorIdx, 0x40);
}

/*
 * Spawns timeline task for a single Tane target.
 */
// FUNCTION: FF7 0x0055d302
void __cdecl BattleFx_SpecialSpawnSeedShotPerTarget(int actor, int casterIndex)
{
    BattleFxSeedShotTimelineSlot *taskRecord;

    taskRecord = (BattleFxSeedShotTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSeedShotTimelineTask);
    Battle_GetBoneWorldPos(casterIndex, g_BattleActorRecords[casterIndex].bone33, &taskRecord->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &taskRecord->endX);
    taskRecord->actorIndex = actor;
}

/*
 * Drives the main timeline and bouncing seed spawning for Tane.
 */
// FUNCTION: FF7 0x0055d378
void __cdecl BattleFx_SpecialSeedShotTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        SVECTOR angleA;
        struct { unsigned char pad_to_8[0x8]; int projDepth; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxSeedShotTimelineSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_34[0x34]; short seedIndex; };
        struct { unsigned char pad_to_38[0x38]; short stepX; };
        struct { unsigned char pad_to_3A[0x3A]; short stepZ; };
        struct { unsigned char pad_to_3C[0x3C]; short stepY; };
        struct { unsigned char pad_to_40[0x40]; BattleFxSeedShotTimelineSlot *timelineSlot; };
        struct { unsigned char pad_to_48[0x48]; SVECTOR scatterX; };
    } frame;
#pragma pack(pop)

    frame.timelineSlot = (BattleFxSeedShotTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timelineSlot->frameCounter < 0xF) {
        frame.angleA.vy = ArcTangentFixed(frame.timelineSlot->endX - frame.timelineSlot->startX, frame.timelineSlot->endY - frame.timelineSlot->startY);
        frame.angleA.vz = 0;
        frame.angleA.vx = frame.angleA.vz;
        RotMatrix(&frame.angleA, &frame.rotMatrix);
        frame.rotMatrix.t[2] = 0;
        frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
        frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
        SetRotMatrix(&frame.rotMatrix);
        SetTransMatrix(&frame.rotMatrix);
        frame.stepX = (frame.timelineSlot->endX - frame.timelineSlot->startX) / 8;
        frame.stepZ = (frame.timelineSlot->endZ - frame.timelineSlot->startZ) / 8;
        frame.stepY = (frame.timelineSlot->endY - frame.timelineSlot->startY) / 8;
        for (frame.seedIndex = 0; frame.seedIndex < 2; frame.seedIndex++) {
            frame.spawnSlot = (BattleFxSeedShotTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSeedShotBounceModelTask);
            frame.scatterX.vx = rand() % 0x12C - 0x96;
            frame.scatterX.vy = rand() % 0x12C - 0x96;
            frame.scatterX.vz = rand() % 0x64 - 0x32;
            RotTransPointToInt16(&frame.scatterX, (short *)&frame.scatterX, &frame.projDepth);
            frame.spawnSlot->posX = frame.timelineSlot->startX + frame.scatterX.vx;
            frame.spawnSlot->posY = frame.timelineSlot->startZ + frame.scatterX.vy;
            frame.spawnSlot->posZ = frame.timelineSlot->startY + frame.scatterX.vz;
            frame.spawnSlot->velocityX = frame.stepX;
            frame.spawnSlot->velocityY = frame.stepZ;
            frame.spawnSlot->velocityZ = frame.stepY;
            frame.spawnSlot->rotY = frame.angleA.vy;
            frame.spawnSlot->rotX = 0xFC00;
            frame.spawnSlot->modelVariant = (rand() % 2);
        }
    }
    if (frame.timelineSlot->frameCounter == 0xF) {
        Battle_PostActorHitOrHurtMessage(frame.timelineSlot->actorIndex);
    }
    if (frame.timelineSlot->frameCounter > 8 && frame.timelineSlot->frameCounter < 0x1C) {
        frame.spawnSlot = (BattleFxSeedShotTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSeedShotScatteredSpriteTask);
        Battle_GetBoneWorldPos(frame.timelineSlot->actorIndex, g_BattleActorRecords[g_BattleSpecialActorId].targetBone, &frame.spawnSlot->posX);
        frame.spawnSlot->posX += (short)(rand() % 0x320 - 0x190);
        frame.spawnSlot->posY += (short)(rand() % 0x320 - 0x190);
        frame.spawnSlot->posZ += (short)(rand() % 0x320 - 0x190);
        frame.spawnSlot->modelVariant = 0x3000;
        frame.spawnSlot->pushLength = -g_BattleActorRecords[g_BattleSpecialActorId].radius;
    }
    (frame.timelineSlot->frameCounter)++;
    if (frame.timelineSlot->frameCounter == 0x28) {
        frame.timelineSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates and renders scattered impact sprites for Tane.
 */
// FUNCTION: FF7 0x0055d6e0
void __cdecl BattleFx_SpecialSeedShotScatteredSpriteTask(void)
{
    BattleFxSeedShotScatterSpriteTask *task;

    task = (BattleFxSeedShotScatterSpriteTask *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialSeedShotSpriteData.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(2, g_BattleSpecialSeedShotLoadedRes);
    g_BattleSpecialSeedShotSpriteData.drawObj = Gfx_ModelTableGetSpriteObject(2, g_BattleSpecialSeedShotLoadedRes);
    BattleFx_InstallBillboardMatrix(&task->posX, 0x1000, task->pushLength);
    g_BattleSpecialSeedShotSpriteData.frame = task->frame;
    g_BattleSpecialSeedShotDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialSeedShotSpriteData, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSeedShotDrawListHandle);
    if (g_BattleIsPaused == 0 && g_BattleSpecialSeedShotSpriteData.spriteGroup != 0) {
        (task->frame)++;
        if (task->frame >= g_BattleSpecialSeedShotSpriteData.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Updates and renders bouncing seed and fruit models for Tane.
 */
// FUNCTION: FF7 0x0055d7b7
void __cdecl BattleFx_SpecialSeedShotBounceModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[100];
        SVECTOR angleB;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_38[0x38]; BattleFxSeedShotBounceModelSlot *bounceSlot; };
        struct { unsigned char pad_to_3C[0x3C]; short speed; };
        struct { unsigned char pad_to_40[0x40]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_60[0x60]; short randAngle; };
    } frame;
#pragma pack(pop)

    frame.bounceSlot = (BattleFxSeedShotBounceModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialSeedShotModelConfig.firstModel = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialSeedShotLoadedRes);
    g_BattleSpecialSeedShotModelConfig.secondModel = Gfx_ModelTableGetRenderObject(1, g_BattleSpecialSeedShotLoadedRes);
    frame.angleB.vx = frame.bounceSlot->angleB;
    frame.angleB.vy = frame.bounceSlot->angleA;
    frame.angleB.vz = 0;
    RotMatrix(&frame.angleB, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, (int *)&g_BattleSpecialSeedShotModelConfig);
    frame.rotMatrix.t[0] = frame.bounceSlot->posX;
    frame.rotMatrix.t[1] = frame.bounceSlot->posY;
    frame.rotMatrix.t[2] = frame.bounceSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    g_BattleSpecialSeedShotModelConfig.flags = 0;
    g_BattleSpecialSeedShotModelConfig.scale = 0x80;
    if (frame.bounceSlot->modelVariant != 0) {
        g_BattleSpecialSeedShotDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialSeedShotModelConfig.firstModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSeedShotDrawListHandle);
    } else {
        g_BattleSpecialSeedShotDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialSeedShotModelConfig.secondModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSeedShotDrawListHandle);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.bounceSlot->frameCounter == 8) {
        frame.randAngle = rand() & 0xFFF;
        frame.speed = rand() % 6;
        frame.bounceSlot->velocityX = CosFixed(frame.randAngle) * frame.speed >> 8;
        frame.bounceSlot->velocityZ = SinFixed(frame.randAngle) * frame.speed >> 8;
        frame.bounceSlot->velocityY = 0xFF00;
        frame.bounceSlot->angleB = rand() & 0xFFF;
        frame.bounceSlot->angleA = rand() & 0xFFF;
    }
    if (frame.bounceSlot->frameCounter > 8) {
        frame.bounceSlot->angleB += 0x80;
        frame.bounceSlot->velocityY += 0x34;
    }
    frame.bounceSlot->posX += frame.bounceSlot->velocityX;
    frame.bounceSlot->posY += frame.bounceSlot->velocityY;
    frame.bounceSlot->posZ += frame.bounceSlot->velocityZ;
    if (frame.bounceSlot->posY > 0) {
        frame.bounceSlot->velocityY = -(frame.bounceSlot->velocityY >> 1);
        frame.bounceSlot->posY = 0;
    }
    (frame.bounceSlot->frameCounter)++;
    if (frame.bounceSlot->frameCounter == 0x1E) {
        frame.bounceSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
