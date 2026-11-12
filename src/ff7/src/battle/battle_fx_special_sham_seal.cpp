// src/ff7/src/battle/battle_fx_special_sham_seal.cpp
/* Enemy Special Effect [idx 56]: ShamSeal (JP: kama2 / 鎌2 (Sickle 2), asset: "SPECIAL/KAMA2") — Sham Seal (0x2cf, Wind Wing) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Kama2 sickle attack: loads its resources, spawns
//   per-target slashes, and drives sickle motion with slash wave, impact spark and orbiting sickle
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
#include "ff7/types/battle_3d.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x008a7250
int g_BattleSpecialShamSealResourceDefs = 0x001900B4;
int g_BattleSpecialShamSealResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialShamSealResourceNames + 0x8)
};
// GLOBAL: FF7 0x008a7258
int g_BattleSpecialShamSealDataBlob[36] = {
    0x0, 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialShamSealResourceNames + 0x14), (int)((char *)&g_BattleFxSpecialShamSealResourceNames + 0x20),
    0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialShamSealResourceNames + 0x2c), (int)((char *)&g_BattleFxSpecialShamSealResourceNames + 0x38), 0x0,
    0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialShamSealResourceNames + 0x44), (int)((char *)&g_BattleFxSpecialShamSealResourceNames + 0x50), 0x0, 0x0,
    0x192a39, (int)((char *)&g_BattleFxSpecialShamSealResourceNames + 0x5c), (int)((char *)&g_BattleFxSpecialShamSealResourceNames + 0x68), 0x0, 0x0, 0x192a39,
    (int)((char *)&g_BattleFxSpecialShamSealResourceNames + 0x74), (int)((char *)&g_BattleFxSpecialShamSealResourceNames + 0x80), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialShamSealResourceNames + 0x8c),
    (int)((char *)&g_BattleFxSpecialShamSealResourceNames + 0x98), 0x0, 0x0, 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x008a72e8
unsigned char g_BattleSpecialShamSealColor1[8] = {0x80, 0x80, 0x80, 0x2c, 0x00, 0x00, 0x00, 0x00};
#undef g_BattleSpecialShamSealSpriteReq1
// GLOBAL: FF7 0x008a72f0
struct BattleFxSpriteRequest g_BattleSpecialShamSealSpriteReq1 = {0, 0, 64, 64, 64, 0x2c, 0, 0};
// GLOBAL: FF7 0x008a7300
unsigned char g_BattleSpecialShamSealScale1[8] = {0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00};
unsigned char g_BattleFxSpecialShamSealResourceNames[164] = {
0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4b, 0x41, 0x4d, 0x41, 0x32, 0x2e, 0x52, 0x53,
0x44, 0x00, 0x00, 0x00, 0x4b, 0x41, 0x4d, 0x41, 0x30, 0x33, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00,
0x48, 0x41, 0x53, 0x53, 0x59, 0x41, 0x31, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x4b, 0x41, 0x4d, 0x41,
0x30, 0x30, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x48, 0x49, 0x54, 0x31, 0x31, 0x2e, 0x53, 0x50,
0x54, 0x00, 0x00, 0x00, 0x4b, 0x41, 0x4d, 0x41, 0x30, 0x30, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00,
0x48, 0x49, 0x54, 0x31, 0x32, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x4b, 0x41, 0x4d, 0x41,
0x30, 0x30, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x48, 0x49, 0x54, 0x32, 0x31, 0x2e, 0x53, 0x50,
0x54, 0x00, 0x00, 0x00, 0x4b, 0x41, 0x4d, 0x41, 0x30, 0x30, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00,
0x48, 0x49, 0x54, 0x32, 0x32, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x4b, 0x41, 0x4d, 0x41,
0x30, 0x31, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x48, 0x49, 0x54, 0x33, 0x31, 0x2e, 0x53, 0x50,
0x54, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00b6f0b0
int g_BattleSpecialShamSealDrawListHandle = 0;
// GLOBAL: FF7 0x00b6f0b4
struct GfxTableCursor *g_BattleSpecialShamSealLoadedRes = 0;
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// clang-format on


/*
 * Loads and registers resources for the Kama2 special effect.
 */
// FUNCTION: FF7 0x00552cf0
void __cdecl BattleFx_SpecialLoadResShamSeal(void)
{
    GfxModelTableBank *stageSceneHandle;
    GfxResourceRequest requestObj;
    int resourceCount;
    char pathWorkBuffer[200];
    GfxResourceRequest *reqPtrPtr;
    int resourceTable;

    if (g_pBattle3dScene != 0 && g_BattleSpecialShamSealLoadedRes == 0) {
        resourceCount = 7;
        resourceTable = (int)&g_BattleSpecialShamSealResourceDefs;
        stageSceneHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        reqPtrPtr = &requestObj;
        Gfx_InitResourceRequest(reqPtrPtr);
        reqPtrPtr->groupTag = 0;
        // STRING: FF7 0x008a73ac
        reqPtrPtr->resourcePath = BattleFx_BuildResourcePath("SPECIAL/KAMA2", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(resourceCount, resourceTable, reqPtrPtr, &g_BattleSpecialShamSealLoadedRes, stageSceneHandle);
    }
}

/*
 * Entry point thunk for the Kama2 special effect.
 */
// FUNCTION: FF7 0x00552d80
void __cdecl BattleFx_SpecialEnterShamSeal(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResShamSeal();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnShamSealPerTarget);
    Battle_PlaySfxPannedToActor(0x83, (short)actorIdx, 0x40);
}

/*
 * Spawns per-target sickle slash tasks for Kama2.
 */
// FUNCTION: FF7 0x00552db5
void __cdecl BattleFx_SpecialSpawnShamSealPerTarget(int actor, int casterIndex)
{
    int effect;

    effect = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialShamSealTimelineTask);
    Battle_GetBoneWorldPos(casterIndex, g_BattleActorRecords[casterIndex].primaryBone, (short *)(effect + 4));
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)(effect + 0xC));
    ((BattleFxFlyingSickleTimelineSlot *)effect)->actorIndex = actor;
}

/*
 * Main timeline task controlling sickle motion and slash effects for Kama2.
 */
// FUNCTION: FF7 0x00552e2b
void __cdecl BattleFx_SpecialShamSealTimelineTask(void)
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
        for (frame.spawnIndex = 0; frame.spawnIndex < 6; frame.spawnIndex++) {
            frame.spawnSlot = (BattleFxFlyingSickleTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialShamSealAnimatedSpriteTask);
            frame.spawnSlot->posX = frame.timelineSlot->fromX + rand() % 0x320 + -0x190;
            frame.spawnSlot->posY = frame.timelineSlot->fromY + rand() % 0x320 + -0x190;
            frame.spawnSlot->posZ = frame.timelineSlot->fromZ + rand() % 0x320 + -0x190;
            frame.spawnSlot = (BattleFxFlyingSickleTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialShamSealOrbitModelTask);
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
            frame.spawnSlot->spinRate = ((frame.spawnIndex % 2) != 0 ? -2 : 0) + 1;
        }
    }
    if (frame.timelineSlot->frameCounter == 0x11) {
        BattleFx_HitMarkSpawnScriptAtActorBone(g_BattleSpecialActorId, 9, 0x1800);
        Battle_PostActorHitOrHurtMessage(frame.timelineSlot->actorIndex);
    }
    if (frame.timelineSlot->frameCounter > 0xD && frame.timelineSlot->frameCounter < 0x11) {
        for (frame.spawnIndex = 0; frame.spawnIndex < 2; frame.spawnIndex++) {
            frame.spawnSlot = (BattleFxFlyingSickleTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialShamSealVariantSpriteTask);
            Battle_GetBoneWorldPos(frame.timelineSlot->actorIndex, g_BattleActorRecords[g_BattleSpecialActorId].targetBone, (short *)((char *)frame.spawnSlot + 4));
            frame.spawnSlot->posX += (short)(rand() % 0x3E8 - 0x1F4);
            frame.spawnSlot->posY += (short)(rand() % 0x3E8 - 0x1F4);
            frame.spawnSlot->posZ += (short)(rand() % 0x3E8 - 0x1F4);
            frame.spawnSlot->spinRate = rand() % 5;
        }
    }
    (frame.timelineSlot->frameCounter)++;
    if (frame.timelineSlot->frameCounter == 0x28) {
        frame.timelineSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Per-frame draw of the slot at (g_BattleActiveTaskSlotIdx << 5) + &g_BattleHitLivewireEmitterRecords: installs a billboard matrix
// from slot+4 at scale 0x3000, publishes the frame counter (slot+2) to the short at 0x008A72EC,
// fills the draw record at 0x008A72E0/0x008A72E4 with row {2,3,4,5,6}[slot+0x18] of table
// g_BattleSpecialShamSealLoadedRes via Gfx_ModelTableGetLoadedRecord / Gfx_ModelTableGetSpriteObject, and chains it through
// g_BattleSpecialShamSealDrawListHandle with BattleFx_SubmitSpriteDraw into bucket g_BattleCurrentOrderingTable+0x70 at shift 0xC. While
// g_BattleIsPaused is zero and the record is non-null it increments slot+2 and retires the slot
// (0xFFFF at +0) once it reaches the frame count at *(*record + 4).

/*
 * Animates trailing slash wave sprites for Kama2.
 */
// FUNCTION: FF7 0x005531d7
void __cdecl BattleFx_SpecialShamSealVariantSpriteTask(void)
{
    int tableIds[5];
    BattleFxShamSealVariantTask *variant;

    variant = (BattleFxShamSealVariantTask *)BATTLE_CURRENT_TASK_SLOT;
    tableIds[0] = 2;
    tableIds[1] = 3;
    tableIds[2] = 4;
    tableIds[3] = 5;
    tableIds[4] = 6;
    BattleFx_InstallBillboardMatrix((void *)((char *)variant + 4), 0x3000, 0);
    *(short *)((char *)&g_BattleSpecialShamSealColor1 + 0x4) = variant->frame;
    ((BattleSpecShamSealDataBlob *)&g_BattleSpecialShamSealDataBlob)->frameTable = Gfx_ModelTableGetLoadedRecord(tableIds[variant->tableIndex], g_BattleSpecialShamSealLoadedRes);
    ((BattleSpecShamSealDataBlob *)&g_BattleSpecialShamSealDataBlob)->tableField = Gfx_ModelTableGetSpriteObject(tableIds[variant->tableIndex], g_BattleSpecialShamSealLoadedRes);
    g_BattleSpecialShamSealDrawListHandle = BattleFx_SubmitSpriteDraw((char *)&g_BattleSpecialShamSealDataBlob + 0x88, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialShamSealDrawListHandle);
    if (g_BattleIsPaused == 0) {
        if (((BattleSpecShamSealDataBlob *)&g_BattleSpecialShamSealDataBlob)->frameTable != 0) {
            (variant->frame)++;
            if (variant->frame >= ((BattleFxSpriteGroup *)((BattleSpecShamSealDataBlob *)&g_BattleSpecialShamSealDataBlob)->frameTable)->header->frameCount) {
                variant->ownerSlot = BATTLE_TASK_FREE;
            }
        }
    }
}

/*
 * Animates impact spark sprites for Kama2.
 */
// FUNCTION: FF7 0x005532e0
void __cdecl BattleFx_SpecialShamSealAnimatedSpriteTask(void)
{
    BattleFxSatelliteBeamBlackPuff *task;

    task = (BattleFxSatelliteBeamBlackPuff *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecialShamSealSpriteReq1.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecialShamSealLoadedRes);
    g_BattleSpecialShamSealSpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecialShamSealLoadedRes);
    BattleFx_InstallBillboardMatrix((void *)((char *)task + 4), 0x1600, 0);
    g_BattleSpecialShamSealSpriteReq1.frame = task->frame;
    g_BattleSpecialShamSealDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialShamSealSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialShamSealDrawListHandle);
    if (g_BattleIsPaused == 0) {
        task->posY -= 0x32;
        (task->frame)++;
        if (task->frame >= g_BattleSpecialShamSealSpriteReq1.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Animates orbiting sickle 3D models for Kama2.
 */
// FUNCTION: FF7 0x005533ba
void __cdecl BattleFx_SpecialShamSealOrbitModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        int projDepth;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR orbitX; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix matrix; };
        struct { unsigned char pad_to_40[0x40]; BattleFxFlyingSickleOrbitModelSlot *orbitSlot; };
        struct { unsigned char pad_to_44[0x44]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_68[0x68]; int modelRecord; };
        struct { unsigned char pad_to_6C[0x6C]; int drawExtra; };
        struct { unsigned char pad_to_70[0x70]; short drawFlags; };
        struct { unsigned char pad_to_72[0x72]; short drawParamA; };
        struct { unsigned char pad_to_74[0x74]; short drawParamB; };
        struct { unsigned char pad_to_76[0x76]; short drawParamC; };
    } frame;
#pragma pack(pop)

    frame.orbitSlot = (BattleFxFlyingSickleOrbitModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.modelRecord = 0;
    frame.drawExtra = 0;
    frame.drawFlags = 0;
    frame.drawParamA = 0x80;
    frame.drawParamB = 0x20;
    frame.drawParamC = 0;
    frame.modelRecord = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialShamSealLoadedRes);
    frame.orbitX.vx = SinFixed((frame.orbitSlot->orbitAngle << 0xC) / 0x23 + 0x800) * frame.orbitSlot->radius >> 0xC;
    frame.orbitX.vz = CosFixed((frame.orbitSlot->orbitAngle << 0xC) / 0x23 + 0x800) * frame.orbitSlot->radius >> 0xC;
    frame.orbitX.vy = 0;
    RotMatrix((SVECTOR *)((int)frame.orbitSlot + 0xC), &frame.matrix);
    frame.matrix.t[0] = frame.orbitSlot->posX;
    frame.matrix.t[1] = frame.orbitSlot->posY;
    frame.matrix.t[2] = frame.orbitSlot->posZ;
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    RotTransPointToInt16(&frame.orbitX, (short *)&frame.orbitX, &frame.projDepth);
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.orbitSlot->frameCounter << 0xA;
    frame.rotX.vz = frame.orbitSlot->angleZ;
    RotMatrixZYX(&frame.rotX, &frame.matrix);
    RotateMatrixY(frame.orbitSlot->angleY, &frame.matrix);
    ScaleMatrix(&frame.matrix, &g_BattleSpecialShamSealScale1);
    frame.matrix.t[0] = frame.orbitX.vx;
    frame.matrix.t[1] = frame.orbitX.vy;
    frame.matrix.t[2] = frame.orbitX.vz;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecialShamSealDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&frame.modelRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialShamSealDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.orbitSlot->orbitAngle += frame.orbitSlot->angleStep;
    (frame.orbitSlot->frameCounter)++;
    if (frame.orbitSlot->frameCounter == 0x23) {
        frame.orbitSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
