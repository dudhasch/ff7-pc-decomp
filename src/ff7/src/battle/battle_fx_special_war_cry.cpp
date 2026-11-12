// src/ff7/src/battle/battle_fx_special_war_cry.cpp
/* Enemy Special Effect [idx 61]: WarCry (JP: toboe2 / 遠吠え2 (Howl 2), asset: "special/toboe2") — War Cry (0x24d, Screamer / Staniv) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Toboe2 attack: it loads resources and spawns a per-target
//   timeline that drives a drifting fade sprite, a growing oriented model and an expanding model.

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
// GLOBAL: FF7 0x008a4408
int g_BattleSpecWarCryResourceDefs = 0x001940B1;
int g_BattleSpecWarCryResourceDefsHeadPtr[1] = {
    (int)&g_BattleFxSpecialWarCryResourceNames[0]
};
// GLOBAL: FF7 0x008a4410
int g_BattleSpecWarCryResourceModelRows[16] = {
    0x0, 0x0, 0x0, 0x1940b1, (int)&g_BattleFxSpecialWarCryResourceNames[0xc], 0x0,
    0x0, 0x0, 0x192a39, (int)&g_BattleFxSpecialWarCryResourceNames[0x18], (int)&g_BattleFxSpecialWarCryResourceNames[0x24], 0x0,
    0x0, 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x008a4450
BattleSpecWarCrySpriteReq1 g_BattleSpecWarCrySpriteReq1 = {-128, -128, -128, 0x2c};
unsigned char g_rawData_008a445c[4] = {0xa8, 0x00, 0x00, 0x00};
unsigned char g_rawData_008a4460[8] = {0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a4468
BattleSpecWarCrySpriteReq2 g_BattleSpecWarCrySpriteReq2 = {{0x400, 0, 0, 0}, 0, 0xa8, 0, 128};
unsigned char g_rawData_008a447c[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a4480
int g_BattleSpecWarCryMatrixReq = 64512;
unsigned char g_BattleFxSpecialWarCryResourceNames[48] =
    "ONPA_11.RSD\0"
    "HIKARI.RSD\0\0"
    "TOBOE00.TIM\0"
    "ONPA1.SPT";
// GLOBAL: FF7 0x00b6d2b0
int g_BattleSpecWarCryDrawListHead = 0;
// GLOBAL: FF7 0x00b6d2b4
struct GfxTableCursor *g_BattleSpecWarCryLoadedResTable = 0;
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/*
 * When the engine context exists and g_BattleSpecWarCryLoadedResTable is empty, builds special/toboe2 with a default-
 * duration record and creates resource type 3 using g_BattleSpecWarCryResourceDefs.
 */
// FUNCTION: FF7 0x005434e0
void __cdecl BattleFx_SpecialLoadResWarCry(void)
{
    GfxModelTableBank *unitOwner;
    GfxResourceRequest requestSlot;
    int resCategory;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecWarCryLoadedResTable == 0) {
        resCategory = 3;
        resourceDefsWord = (int)&g_BattleSpecWarCryResourceDefs;
        unitOwner = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a44b8
        pRequest->resourcePath = BattleFx_BuildResourcePath("special/toboe2", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(resCategory, resourceDefsWord, pRequest, &g_BattleSpecWarCryLoadedResTable, unitOwner);
    }
}

/*
 * Entry wrapper: loads the Toboe2 resources, then passes targetMask and actorId to
 * BattleFx_SpecialRegisterWarCryPerTargetCallback.
 */
// FUNCTION: FF7 0x00543570
void __cdecl BattleFx_SpecialEnterWarCry(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResWarCry();
    BattleFx_SpecialRegisterWarCryPerTargetCallback(targetMask, actorIdx);
}

/*
 * Creates a task for BattleFx_SpecialSpawnWarCryPerTarget with both arguments and mode 4, then dispatches event (0x60,
 * actorId, 0x40).
 */
// FUNCTION: FF7 0x0054358a
void __cdecl BattleFx_SpecialRegisterWarCryPerTargetCallback(int targetMask, int actorId)
{
    Battle_CreateBitPatternCallbackTask(targetMask, actorId, 4, BattleFx_SpecialSpawnWarCryPerTarget);
    Battle_PlaySfxPannedToActor(0x60, (short)actorId, 0x40);
}

/*
 * Allocates BattleFx_SpecialWarCryTimelineTask with the source actor's bone position as the origin and
 * the target actor's bone position as the target, and records the target actor.
 */
// FUNCTION: FF7 0x005435b7
void __cdecl BattleFx_SpecialSpawnWarCryPerTarget(int actor, int actorId)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxWarCryTimelineSlot *taskRecord;

    taskRecord = (BattleFxWarCryTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWarCryTimelineTask);
    Battle_GetBoneWorldPos(actorId, g_BattleActorRecords[actorId].bone33, &taskRecord->posX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &taskRecord->targetX);
    taskRecord->targetIndex = actor;
}

/*
 * Per-target timeline: for the first 10 frames it launches a growing oriented model from the source
 * towards the target, at frame 20 an expanding model on the target, and on frames 20 to 27 two
 * drifting fade sprites in random directions. At frame 40 it queues the target index and frees the slot.
 */
// FUNCTION: FF7 0x0054362f
void __cdecl BattleFx_SpecialWarCryTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        struct { unsigned char pad_to_8[0x8]; int rotTransOut; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR pointX; };
        struct { unsigned char pad_to_14[0x14]; BattleFxWeaponEmeraldBeamSprite *spawnSlot; };
        struct { unsigned char pad_to_18[0x18]; short *ringRecord; };
        struct { unsigned char pad_to_1C[0x1C]; short spawnIndex; };
        struct { unsigned char pad_to_20[0x20]; SVECTOR angleX; };
        struct { unsigned char pad_to_28[0x28]; BattleFxWarCryTimelineSlot *taskSlot; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix rotMatrix; };
    } frame;
#pragma pack(pop)

    frame.taskSlot = (BattleFxWarCryTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.taskSlot->frameCounter < 10 && (frame.taskSlot->frameCounter % 1) == 0) {
        frame.spawnSlot = (BattleFxWeaponEmeraldBeamSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWarCryGrowingOrientedModelTask);
        frame.spawnSlot->posX = frame.taskSlot->posX;
        frame.spawnSlot->posY = frame.taskSlot->posY;
        frame.spawnSlot->posZ = frame.taskSlot->posZ;
        frame.spawnSlot->velocityX = (frame.taskSlot->targetX - frame.taskSlot->posX) / 20;
        frame.spawnSlot->velocityY = (frame.taskSlot->targetY - frame.taskSlot->posY) / 20;
        frame.spawnSlot->velocityZ = (frame.taskSlot->targetZ - frame.taskSlot->posZ) / 20;
        frame.spawnSlot->scale = 0x800 /* 0.5 */;
    }
    if (frame.taskSlot->frameCounter == 20) {
        frame.spawnSlot = (BattleFxWeaponEmeraldBeamSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWarCryExpandingModelTask);
        frame.spawnSlot->posX = frame.taskSlot->targetX;
        frame.spawnSlot->posY = frame.taskSlot->targetY;
        frame.spawnSlot->posZ = frame.taskSlot->targetZ;
    }
    if (frame.taskSlot->frameCounter >= 20 && frame.taskSlot->frameCounter < 28) {
        for (frame.spawnIndex = 0; frame.spawnIndex < 2; frame.spawnIndex++) {
            frame.spawnSlot = (BattleFxWeaponEmeraldBeamSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWarCryDriftingFadeSpriteTask);
            frame.spawnSlot->posX = frame.taskSlot->targetX;
            frame.spawnSlot->posY = frame.taskSlot->targetY;
            frame.spawnSlot->posZ = frame.taskSlot->targetZ;
            frame.pointX.vx = 0;
            frame.pointX.vy = 0;
            frame.pointX.vz = 160;
            frame.angleX.vx = (rand() % 4096);
            frame.angleX.vy = (rand() % 4096);
            frame.angleX.vz = 0;
            RotMatrix(&frame.angleX, &frame.rotMatrix);
            frame.rotMatrix.t[2] = 0;
            frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
            frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
            SetRotMatrix(&frame.rotMatrix);
            SetTransMatrix(&frame.rotMatrix);
            RotTransPointToInt16(&frame.pointX, &frame.spawnSlot->velocityX, &frame.rotTransOut);
            frame.spawnSlot->scale = ((rand() % 4) << 10) + 0x1000;
        }
    }
    (frame.taskSlot->frameCounter)++;
    if (frame.taskSlot->frameCounter == 40) {
        frame.ringRecord = (short *)Battle_AllocSceneMessage(4);
        *frame.ringRecord = frame.taskSlot->targetIndex;
        frame.taskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Per-frame sprite step for the active main effect slot, which drifts and fades. It installs a
// billboard matrix at the slot's scale and sets the sprite request's three colour bytes to 0x80
// minus frame times 0xA. It takes the palette and handle from entry 2 of
// g_BattleSpecWarCryLoadedResTable and submits the sprite at shift 0xC with the cursor in
// g_BattleSpecWarCryDrawListHead. Unless the battle is paused, it adds the velocity to the position
// and retires the slot when frame reaches 12.
// FUNCTION: FF7 0x00543908
void __cdecl BattleFx_SpecialWarCryDriftingFadeSpriteTask(void)
{
    BattleFxBillboardSpriteSlot *task;

    task = (BattleFxBillboardSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->scale, 0);
    g_BattleSpecWarCrySpriteReq1.blue = 0x80 - task->frame * 10;
    g_BattleSpecWarCrySpriteReq1.green = g_BattleSpecWarCrySpriteReq1.blue;
    g_BattleSpecWarCrySpriteReq1.red = g_BattleSpecWarCrySpriteReq1.green;
    g_BattleSpecWarCryResourceModelRows[15] = Gfx_ModelTableGetSpriteObject(2, g_BattleSpecWarCryLoadedResTable);
    g_BattleSpecWarCryResourceModelRows[14] = Gfx_ModelTableGetLoadedRecord(2, g_BattleSpecWarCryLoadedResTable);
    g_BattleSpecWarCryDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecWarCryResourceModelRows[14], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecWarCryDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posX += task->velocityX;
    task->posY += task->velocityY;
    task->posZ += task->velocityZ;
    (task->frame)++;
    if (task->frame == 12) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws a model turned along the slot's velocity and scaled by its growing scale, then moves it by
 * the velocity and grows it by 0x190 a frame; the slot is freed at frame 20.
 */
// FUNCTION: FF7 0x00543a2b
void __cdecl BattleFx_SpecialWarCryGrowingOrientedModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[116];
        PsxMatrix orientMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxNapalmMissileSlot *taskSlot; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_44[0x44]; PsxMatrix lookBasis; };
        struct { unsigned char pad_to_64[0x64]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.taskSlot = (BattleFxNapalmMissileSlot *)BATTLE_CURRENT_TASK_SLOT;
    Gte_BuildLookBasisFromDirection(&frame.taskSlot->velocityX, frame.lookBasis.m[0]);
    RotMatrix(&g_BattleSpecWarCrySpriteReq2.angles, &frame.orientMatrix);
    ConcatMatrixPreservingTranslation(&frame.lookBasis, &frame.orientMatrix, &frame.orientMatrix);
    frame.scaleX.vz = frame.taskSlot->angle;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.orientMatrix, &frame.scaleX);
    frame.orientMatrix.t[0] = frame.taskSlot->posX;
    frame.orientMatrix.t[1] = frame.taskSlot->posY;
    frame.orientMatrix.t[2] = frame.taskSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.orientMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecWarCrySpriteReq1.modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecWarCryLoadedResTable);
    g_BattleSpecWarCryDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecWarCrySpriteReq1.modelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecWarCryDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.taskSlot->posX += frame.taskSlot->velocityX;
    frame.taskSlot->posY += frame.taskSlot->velocityY;
    frame.taskSlot->posZ += frame.taskSlot->velocityZ;
    frame.taskSlot->angle += 0x190;
    (frame.taskSlot->frameCounter)++;
    if (frame.taskSlot->frameCounter == 20) {
        frame.taskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws a model at the slot's position, scaled up by 0x258 a frame from 0x800 and faded by the
 * frame counter; the slot is freed at frame 15.
 */
// FUNCTION: FF7 0x00543bbe
void __cdecl BattleFx_SpecialWarCryExpandingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        int rotTransResult;
        struct { unsigned char pad_to_4[0x4]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix scaleMatrix; };
        struct { unsigned char pad_to_44[0x44]; short axisIndex; };
        struct { unsigned char pad_to_48[0x48]; BattleTaskSlot *taskSlot; };
    } frame;
#pragma pack(pop)

    frame.taskSlot = BATTLE_CURRENT_TASK_SLOT;
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.scaleMatrix.m[2][frame.axisIndex] = 0;
        frame.scaleMatrix.m[1][frame.axisIndex] = 0;
        frame.scaleMatrix.m[0][frame.axisIndex] = 0;
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((SVECTOR *)frame.taskSlot->data, frame.scaleMatrix.t, &frame.rotTransResult);
    frame.scaleMatrix.m[2][2] = frame.taskSlot->frameCounter * 0x258 + 0x800;
    frame.scaleMatrix.m[1][1] = frame.scaleMatrix.m[2][2];
    frame.scaleMatrix.m[0][0] = frame.scaleMatrix.m[1][1];
    RotMatrix((SVECTOR *)&g_BattleSpecWarCryMatrixReq, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.scaleMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.scaleMatrix);
    SetTransMatrix(&frame.scaleMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecWarCrySpriteReq2.fadeLevel = frame.taskSlot->frameCounter << 8;
    g_BattleSpecWarCrySpriteReq2.modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleSpecWarCryLoadedResTable);
    g_BattleSpecWarCryDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecWarCrySpriteReq2.modelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecWarCryDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.taskSlot->frameCounter)++;
    if (frame.taskSlot->frameCounter == 15) {
        frame.taskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
