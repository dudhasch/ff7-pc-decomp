// src/ff7/src/battle/battle_fx_special_flying_sickle.cpp
/* Enemy Special Effect [idx 34]: FlyingSickle (JP: kama / 鎌 (Sickle), asset: "SPECIAL/KAMA") — Flying Sickle (0x1fc, Flapbeat / Frozen Nail / SOLDIER:3rd) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Kama attack: loads its resources, spawns per-target
//   sickle strikes, and drives sickle motion with slash wave, rising spark and orbiting sickle
//   model tasks.

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
// GLOBAL: FF7 0x008a76a0
int g_BattleSpecialFlyingSickleResourceDefs = 0x001900B4;
int g_BattleSpecialFlyingSickleResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialFlyingSickleResourceNames + 0x8)
};
// GLOBAL: FF7 0x008a76a8
int g_BattleSpecialFlyingSickleDataBlob[36] = {
    0x0, 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialFlyingSickleResourceNames + 0x14), (int)((char *)&g_BattleFxSpecialFlyingSickleResourceNames + 0x20),
    0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialFlyingSickleResourceNames + 0x2c), (int)((char *)&g_BattleFxSpecialFlyingSickleResourceNames + 0x38), 0x0,
    0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialFlyingSickleResourceNames + 0x44), (int)((char *)&g_BattleFxSpecialFlyingSickleResourceNames + 0x50), 0x0, 0x0,
    0x192a39, (int)((char *)&g_BattleFxSpecialFlyingSickleResourceNames + 0x5c), (int)((char *)&g_BattleFxSpecialFlyingSickleResourceNames + 0x68), 0x0, 0x0, 0x192a39,
    (int)((char *)&g_BattleFxSpecialFlyingSickleResourceNames + 0x74), (int)((char *)&g_BattleFxSpecialFlyingSickleResourceNames + 0x80), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialFlyingSickleResourceNames + 0x8c),
    (int)((char *)&g_BattleFxSpecialFlyingSickleResourceNames + 0x98), 0x0, 0x0, 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x008a7738
unsigned char g_BattleSpecialFlyingSickleColor1[8] = {0x80, 0x80, 0x80, 0x2c, 0x00, 0x00, 0x00, 0x00};
#undef g_BattleSpecialFlyingSickleSpriteReq1
// GLOBAL: FF7 0x008a7740
struct BattleFxSpriteRequest g_BattleSpecialFlyingSickleSpriteReq1 = {0, 0, 64, 64, 64, 0x2c, 0, 0};
// GLOBAL: FF7 0x008a7750
unsigned char g_BattleSpecialFlyingSickleScale1[8] = {0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00};
unsigned char g_BattleFxSpecialFlyingSickleResourceNames[164] = {
0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4b, 0x41, 0x4d, 0x41, 0x31, 0x2e, 0x52, 0x53,
0x44, 0x00, 0x00, 0x00, 0x4b, 0x41, 0x4d, 0x41, 0x30, 0x33, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00,
0x48, 0x41, 0x53, 0x53, 0x59, 0x41, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x4b, 0x41, 0x4d, 0x41,
0x30, 0x30, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x48, 0x49, 0x54, 0x31, 0x31, 0x2e, 0x53, 0x50,
0x54, 0x00, 0x00, 0x00, 0x4b, 0x41, 0x4d, 0x41, 0x30, 0x30, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00,
0x48, 0x49, 0x54, 0x31, 0x32, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x4b, 0x41, 0x4d, 0x41,
0x30, 0x30, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x48, 0x49, 0x54, 0x32, 0x31, 0x2e, 0x53, 0x50,
0x54, 0x00, 0x00, 0x00, 0x4b, 0x41, 0x4d, 0x41, 0x30, 0x30, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00,
0x48, 0x49, 0x54, 0x32, 0x32, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x4b, 0x41, 0x4d, 0x41,
0x30, 0x31, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x48, 0x49, 0x54, 0x33, 0x31, 0x2e, 0x53, 0x50,
0x54, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00b6f0d0
int g_BattleSpecialFlyingSickleDrawListHandle = 0;
// GLOBAL: FF7 0x00b6f0d4
struct GfxTableCursor *g_BattleSpecialFlyingSickleLoadedRes = 0;
unsigned char g_bssPad_00b6f0d8[4] = {0};
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// clang-format on


/*
 * Loads and registers resources for the Kama special effect.
 */
// FUNCTION: FF7 0x00554e80
void __cdecl BattleFx_SpecialLoadResFlyingSickle(void)
{
    GfxModelTableBank *newSceneLink;
    GfxResourceRequest resultPending;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *reqPtrPtr;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecialFlyingSickleLoadedRes == 0) {
        kind = 7;
        resourceDefsWord = (int)&g_BattleSpecialFlyingSickleResourceDefs;
        newSceneLink = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        reqPtrPtr = &resultPending;
        Gfx_InitResourceRequest(reqPtrPtr);
        reqPtrPtr->groupTag = 0;
        // STRING: FF7 0x008a77fc
        reqPtrPtr->resourcePath = (char *)BattleFx_BuildResourcePath("SPECIAL/KAMA", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, reqPtrPtr, &g_BattleSpecialFlyingSickleLoadedRes, newSceneLink);
    }
}

/*
 * Entry point thunk for the Kama special effect.
 */
// FUNCTION: FF7 0x00554f10
void __cdecl BattleFx_SpecialEnterFlyingSickle(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginFlyingSickle(targetMask, actorIdx);
}

/*
 * Initializes resources and starts target tasks for Kama.
 */
// FUNCTION: FF7 0x00554f25
void __cdecl BattleFx_SpecialBeginFlyingSickle(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResFlyingSickle();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnFlyingSicklePerTarget);
    Battle_PlaySfxPannedToActor(0x83, (short)actorIdx, 0x40);
}

/*
 * Spawns per-target sickle attack tasks for Kama.
 */
// FUNCTION: FF7 0x00554f5a
void __cdecl BattleFx_SpecialSpawnFlyingSicklePerTarget(int actor, int attackerId)
{
    int taskPtr;

    taskPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFlyingSickleTimelineTask);
    Battle_GetBoneWorldPos(attackerId, g_BattleActorRecords[attackerId].targetBone, (short *)(taskPtr + 4));
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)(taskPtr + 0xC));
    ((BattleFxFlyingSickleTimelineSlot *)taskPtr)->actorIndex = actor;
}

/*
 * Main timeline task controlling sickle motion and slash strikes for Kama.
 */
// FUNCTION: FF7 0x00554fd0
void __cdecl BattleFx_SpecialFlyingSickleTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        struct { unsigned char pad_to_2C[0x2C]; BattleFxFlyingSickleTimelineSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_38[0x38]; short spawnIndex; };
        struct { unsigned char pad_to_44[0x44]; BattleFxFlyingSickleTimelineSlot *timelineSlot; };
        struct { unsigned char pad_to_50[0x50]; short deltaX; };
        struct { unsigned char pad_to_52[0x52]; short deltaY; };
        struct { unsigned char pad_to_54[0x54]; short deltaZ; };
    } frame;
#pragma pack(pop)

    frame.timelineSlot = (BattleFxFlyingSickleTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timelineSlot->frameCounter == 0) {
        frame.spawnSlot = (BattleFxFlyingSickleTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFlyingSickleRisingAnimatedSpriteTask);
        frame.spawnSlot->posX = frame.timelineSlot->fromX + rand() % 800 + -400;
        frame.spawnSlot->posY = frame.timelineSlot->fromY + rand() % 800 + -400;
        frame.spawnSlot->posZ = frame.timelineSlot->fromZ + rand() % 800 + -400;
        frame.spawnSlot = (BattleFxFlyingSickleTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFlyingSickleOrbitModelTask);
        frame.spawnSlot->posX = (frame.timelineSlot->fromX + frame.timelineSlot->toX) / 2;
        frame.spawnSlot->posY = (frame.timelineSlot->fromY + frame.timelineSlot->toY) / 2;
        frame.spawnSlot->posZ = (frame.timelineSlot->fromZ + frame.timelineSlot->toZ) / 2;
        frame.spawnSlot->angleX = 0;
        frame.spawnSlot->angle = ArcTangentFixed(frame.timelineSlot->toX - frame.timelineSlot->fromX, frame.timelineSlot->toZ - frame.timelineSlot->fromZ);
        frame.spawnSlot->angleZ = (rand() % 1024);
        frame.deltaX = frame.timelineSlot->toX - frame.timelineSlot->fromX;
        frame.deltaY = frame.timelineSlot->toY - frame.timelineSlot->fromY;
        frame.deltaZ = frame.timelineSlot->toZ - frame.timelineSlot->fromZ;
        frame.spawnSlot->distance = SqrtToInt(frame.deltaX * frame.deltaX + frame.deltaY * frame.deltaY + frame.deltaZ * frame.deltaZ) >> 1;
        frame.spawnSlot->orbitAngle = 0;
        frame.spawnSlot->spinRate = 1;
    }
    if (frame.timelineSlot->frameCounter == 17) {
        BattleFx_HitMarkSpawnScriptAtActorBone(g_BattleSpecialActorId, 9, 0x1000 /* 1.0 */);
        Battle_PostActorHitOrHurtMessage(frame.timelineSlot->actorIndex);
    }
    if (frame.timelineSlot->frameCounter > 13 && frame.timelineSlot->frameCounter < 17) {
        for (frame.spawnIndex = 0; frame.spawnIndex < 2; frame.spawnIndex++) {
            frame.spawnSlot = (BattleFxFlyingSickleTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFlyingSickleVariantSpriteTask);
            Battle_GetBoneWorldPos(frame.timelineSlot->actorIndex, g_BattleActorRecords[g_BattleSpecialActorId].targetBone, (short *)((int)frame.spawnSlot + 4));
            frame.spawnSlot->posX += (short)(rand() % 1000 - 500);
            frame.spawnSlot->posY += (short)(rand() % 1000 - 500);
            frame.spawnSlot->posZ += (short)(rand() % 1000 - 500);
            frame.spawnSlot->spinRate = rand() % 5;
        }
    }
    (frame.timelineSlot->frameCounter)++;
    if (frame.timelineSlot->frameCounter == 40) {
        frame.timelineSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Per-frame handler for the effect record at g_BattleHitLivewireEmitterRecords + g_BattleActiveTaskSlotIdx * 0x20. Installs a
// billboard matrix for record+4 at scale 0x3000 and depth 0, copies the frame counter at +2 into
// the short at 0x008A773C, and fills the sprite request at 0x008A7730/0x008A7734 from
// Gfx_ModelTableGetLoadedRecord / Gfx_ModelTableGetSpriteObject with the row index taken from a local table
// {2,3,4,5,6} by the variant short at +0x18 and the handle g_BattleSpecialFlyingSickleLoadedRes, then submits it through
// BattleFx_SubmitSpriteDraw into g_BattleSpecialFlyingSickleDrawListHandle. Unless g_BattleIsPaused is set, and only when the
// request pointer is non-null, it advances the counter and retires the record with 0xFFFF at +0
// once the counter reaches the frame count at [[[0x008A7730]]+4].

/*
 * Animates trailing slash wave sprites for Kama.
 */
// FUNCTION: FF7 0x0055533f
void __cdecl BattleFx_SpecialFlyingSickleVariantSpriteTask(void)
{
    BattleFxFlyingSickleVariantSprite *task;
    int entryIds[5];

    task = (BattleFxFlyingSickleVariantSprite *)BATTLE_CURRENT_TASK_SLOT;
    entryIds[0] = 2;
    entryIds[1] = 3;
    entryIds[2] = 4;
    entryIds[3] = 5;
    entryIds[4] = 6;
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), 0x3000 /* 3.0 */, 0);
    *(short *)((char *)&g_BattleSpecialFlyingSickleColor1 + 0x4) = task->frameIndex;
    ((BattleSpecFlyingSickleDataBlob *)&g_BattleSpecialFlyingSickleDataBlob)->frameTable = Gfx_ModelTableGetLoadedRecord(entryIds[task->variantIndex], g_BattleSpecialFlyingSickleLoadedRes);
    ((BattleSpecFlyingSickleDataBlob *)&g_BattleSpecialFlyingSickleDataBlob)->variantField = Gfx_ModelTableGetSpriteObject(entryIds[task->variantIndex], g_BattleSpecialFlyingSickleLoadedRes);
    g_BattleSpecialFlyingSickleDrawListHandle = BattleFx_SubmitSpriteDraw((void *)(g_BattleSpecialFlyingSickleDataBlob + 0x22), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialFlyingSickleDrawListHandle);
    if (g_BattleIsPaused == 0) {
        if (((BattleSpecFlyingSickleDataBlob *)&g_BattleSpecialFlyingSickleDataBlob)->frameTable != 0) {
            (task->frameIndex)++;
            if (task->frameIndex >= *(int *)((char *)*(int *)((BattleSpecFlyingSickleDataBlob *)&g_BattleSpecialFlyingSickleDataBlob)->frameTable + 4)) {
                task->ownerSlot = BATTLE_TASK_FREE;
            }
        }
    }
}

/*
 * Animates rising spark sprites on sickle impact for Kama.
 */
// FUNCTION: FF7 0x00555448
void __cdecl BattleFx_SpecialFlyingSickleRisingAnimatedSpriteTask(void)
{
    BattleFxSatelliteBeamBlackPuff *task;

    task = (BattleFxSatelliteBeamBlackPuff *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecialFlyingSickleSpriteReq1.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecialFlyingSickleLoadedRes);
    g_BattleSpecialFlyingSickleSpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecialFlyingSickleLoadedRes);
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), 0x1600 /* 1.375 */, 0);
    g_BattleSpecialFlyingSickleSpriteReq1.frame = task->frame;
    g_BattleSpecialFlyingSickleDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialFlyingSickleSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialFlyingSickleDrawListHandle);
    if (g_BattleIsPaused == 0) {
        task->posY -= 50;
        if ((g_BattleSpecialFlyingSickleSpriteReq1.spriteGroup) != 0) {
            (task->frame)++;
            if (task->frame >= g_BattleSpecialFlyingSickleSpriteReq1.spriteGroup->header->frameCount) {
                task->ownerSlot = BATTLE_TASK_FREE;
            }
        }
    }
}

/*
 * Animates orbiting sickle models for Kama.
 */
// FUNCTION: FF7 0x0055552b
void __cdecl BattleFx_SpecialFlyingSickleOrbitModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        int transDepth;
        struct { unsigned char pad_to_4[0x4]; SVECTOR angleX; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR orbitX; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_40[0x40]; BattleFxFlyingSickleOrbitModelSlot *kamaSlot; };
        struct { unsigned char pad_to_44[0x44]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_68[0x68]; int modelHandle; };
        struct { unsigned char pad_to_6C[0x6C]; int drawParamA; };
        struct { unsigned char pad_to_70[0x70]; short drawParamB; };
        struct { unsigned char pad_to_72[0x72]; short drawParamC; };
        struct { unsigned char pad_to_74[0x74]; short drawParamD; };
        struct { unsigned char pad_to_76[0x76]; short drawParamE; };
    } frame;
#pragma pack(pop)

    frame.kamaSlot = (BattleFxFlyingSickleOrbitModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.modelHandle = 0;
    frame.drawParamA = 0x80;
    frame.drawParamB = 0;
    frame.drawParamC = 0;
    frame.drawParamD = 0x20;
    frame.drawParamE = 0;
    frame.modelHandle = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialFlyingSickleLoadedRes);
    frame.orbitX.vx = SinFixed((frame.kamaSlot->orbitAngle << 12) / 35 + 2048) * frame.kamaSlot->radius >> 12;
    frame.orbitX.vz = CosFixed((frame.kamaSlot->orbitAngle << 12) / 35 + 2048) * frame.kamaSlot->radius >> 12;
    frame.orbitX.vy = 0;
    RotMatrix((SVECTOR *)((int)frame.kamaSlot + 0xC), &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.kamaSlot->posX;
    frame.rotMatrix.t[1] = frame.kamaSlot->posY;
    frame.rotMatrix.t[2] = frame.kamaSlot->posZ;
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    RotTransPointToInt16(&frame.orbitX, (short *)&frame.orbitX, &frame.transDepth);
    frame.angleX.vx = 0;
    frame.angleX.vy = frame.kamaSlot->frameCounter << 10;
    frame.angleX.vz = frame.kamaSlot->angleZ;
    RotMatrixZYX(&frame.angleX, &frame.rotMatrix);
    RotateMatrixY(frame.kamaSlot->angleY, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialFlyingSickleScale1);
    frame.rotMatrix.t[0] = frame.orbitX.vx;
    frame.rotMatrix.t[1] = frame.orbitX.vy;
    frame.rotMatrix.t[2] = frame.orbitX.vz;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialFlyingSickleDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&frame.modelHandle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialFlyingSickleDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.kamaSlot->orbitAngle += frame.kamaSlot->angleStep;
    (frame.kamaSlot->frameCounter)++;
    if (frame.kamaSlot->frameCounter == 35) {
        frame.kamaSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
