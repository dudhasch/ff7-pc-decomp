// src/ff7/src/battle/battle_fx_special_autumn_leaves.cpp
/* Enemy Special Effect [idx 35]: AutumnLeaves (JP: konoha / 木の葉 (Autumn Leaves / Leaf Swirl), asset: "SPECIAL/KONOHA") — Autumn Leaves (0x289, Rilfsak), Dance (0x294) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Konoha attack: loads resources, sets the target mask, and
//   drives a timeline that spawns swirling falling leaf models.

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
// GLOBAL: FF7 0x008a82c8
int g_BattleSpecialAutumnLeavesResourceDefs = 0x001900B4;
char *g_BattleFxSpecialAutumnLeavesResourceNames[1] = {
    // STRING: FF7 0x008a8360
    "KONOHA1.RSD"
};
unsigned int g_BattleSpecialAutumnLeavesResourceDefsDataTail[28] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b4,
    // STRING: FF7 0x008a836c
    (unsigned int)"KONOHA2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b4,
    // STRING: FF7 0x008a8378
    (unsigned int)"KONOHA3.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a8384
    (unsigned int)"KONOHA1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a8390
    (unsigned int)"KONOHA2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a839c
    (unsigned int)"KONOHA3.RSD",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x008a8340
struct BattleSpecialAutumnLeavesModelConfig g_BattleSpecialAutumnLeavesModelConfig = {{0, 0x10, 0, 0, 0, 0x10, 0, 0, 0, 0x10, 0, 0, 0, 0, 0, 0}, 0, {0}, 128};
unsigned char g_rawData_008a835c[4] = {0x20, 0x00, 0x00, 0x00};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b72ea4
int g_BattleSpecialAutumnLeavesDrawListHandle = 0;
// GLOBAL: FF7 0x00b72ea8
int g_BattleSpecialAutumnLeavesTargetMask = 0;
// GLOBAL: FF7 0x00b72eac
struct GfxTableCursor *g_BattleSpecialAutumnLeavesLoadedRes = 0;
// GLOBAL: FF7 0x00b72eb0
short g_BattleSpecialAutumnLeavesStep = 0;
unsigned char g_bssPad_00b72eb4[4] = {0};
// clang-format on

/*
 * Loads the Konoha special effect's model table when it is not already loaded.
 */
// FUNCTION: FF7 0x0055b350
void __cdecl BattleFx_SpecialLoadResAutumnLeaves(void)
{
    GfxModelTableBank *value;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecialAutumnLeavesLoadedRes == 0) {
        kind = 6;
        resourceDefsWord = (int)&g_BattleSpecialAutumnLeavesResourceDefs;
        value = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a83a8
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/KONOHA", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecialAutumnLeavesLoadedRes, value);
    }
}

/*
 * Resets the leaf fade step and starts the Konoha special effect.
 */
// FUNCTION: FF7 0x0055b3e0
void __cdecl BattleFx_SpecialEnterAutumnLeaves(int targetMask, int actorIdx)
{
    g_BattleSpecialAutumnLeavesStep = 0;
    BattleFx_SpecialBeginAutumnLeaves(targetMask, actorIdx);
}

/*
 * Loads the resources, records the target mask and starts the timeline task at the acting bone and
 * the targets' centroid.
 */
// FUNCTION: FF7 0x0055b3fe
void __cdecl BattleFx_SpecialBeginAutumnLeaves(int targetMask, int actorIdx)
{
    BattleFxAutumnLeavesTimelineSlot *taskPtr;

    BattleFx_SpecialLoadResAutumnLeaves();
    g_BattleSpecialAutumnLeavesTargetMask = targetMask;
    taskPtr = (BattleFxAutumnLeavesTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialAutumnLeavesTimelineTask);
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].targetBone, &taskPtr->startX);
    Battle_GetActorMaskCentroid(targetMask, &taskPtr->endX);
    Battle_PlaySfxPannedToActor(0x68, (short)actorIdx, 0x40);
}

/*
 * Drives the main timeline and spawns swirling leaves for Konoha.
 */
// FUNCTION: FF7 0x0055b477
void __cdecl BattleFx_SpecialAutumnLeavesTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[88];
        struct { unsigned char pad_to_2C[0x2C]; BattleFxAutumnLeavesLeafModelSlot *leafSlot; };
        struct { unsigned char pad_to_38[0x38]; short i; };
        struct { unsigned char pad_to_3C[0x3C]; short velocityX; };
        struct { unsigned char pad_to_3E[0x3E]; short velocityY; };
        struct { unsigned char pad_to_40[0x40]; short velocityZ; };
        struct { unsigned char pad_to_44[0x44]; BattleFxAutumnLeavesTimelineSlot *timelineSlot; };
    } frame;
#pragma pack(pop)

    frame.timelineSlot = (BattleFxAutumnLeavesTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timelineSlot->frameCounter == 0) {
        frame.velocityX = (frame.timelineSlot->endX - frame.timelineSlot->startX) / 15;
        frame.velocityY = 0;
        frame.velocityZ = (frame.timelineSlot->endZ - frame.timelineSlot->startZ) / 15;
        for (frame.i = 0; frame.i < 60; frame.i++) {
            frame.leafSlot = (BattleFxAutumnLeavesLeafModelSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialAutumnLeavesLeafModelTask);
            frame.leafSlot->angle = (rand() % 4096);
            frame.leafSlot->modelIndex = rand() % 3;
            frame.leafSlot->spinAngle = (rand() % 4096);
            frame.leafSlot->radius = rand() % (3000 - (SinFixed(frame.i * 25 + 500) * 2500 >> 12));
            frame.leafSlot->posX = frame.timelineSlot->startX;
            frame.leafSlot->posY = -frame.i * 40;
            frame.leafSlot->posZ = frame.timelineSlot->startZ;
            frame.leafSlot->velocityX = frame.velocityX;
            frame.leafSlot->velocityY = frame.velocityY;
            frame.leafSlot->velocityZ = frame.velocityZ;
        }
    }
    if (frame.timelineSlot->frameCounter == 40) {
        for (frame.i = 0; frame.i < 10; frame.i++) {
            if ((g_BattleSpecialAutumnLeavesTargetMask & 1 << frame.i) != 0) {
                Battle_PostActorHitOrHurtMessage(frame.i);
                BattleFx_HitMarkSpawnScriptAtActorBone(frame.i, 0x27, 0x1800 /* 1.5 */);
            }
        }
    }
    if (frame.timelineSlot->frameCounter < 8) {
        g_BattleSpecialAutumnLeavesStep++;
    }
    if (frame.timelineSlot->frameCounter > 42) {
        g_BattleSpecialAutumnLeavesStep--;
    }
    (frame.timelineSlot->frameCounter)++;
    if (frame.timelineSlot->frameCounter == 50) {
        frame.timelineSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates and renders individual falling and swirling leaf models for Konoha.
 */
// FUNCTION: FF7 0x0055b6c7
void __cdecl BattleFx_SpecialAutumnLeavesLeafModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[124];
        int projDepth;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offsetX; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_38[0x38]; int modelIdZero[3]; };
        struct { unsigned char pad_to_44[0x44]; int modelIdThree[3]; };
        struct { unsigned char pad_to_50[0x50]; BattleFxAutumnLeavesLeafModelSlot *leafSlot; };
        struct { unsigned char pad_to_58[0x58]; PsxMatrix worldMatrix; };
    } frame;
#pragma pack(pop)

    frame.leafSlot = (BattleFxAutumnLeavesLeafModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.modelIdZero[0] = 0;
    frame.modelIdZero[1] = 1;
    frame.modelIdZero[2] = 2;
    frame.modelIdThree[0] = 3;
    frame.modelIdThree[1] = 4;
    frame.modelIdThree[2] = 5;
    frame.offsetX.vx = SinFixed(frame.leafSlot->angle) * frame.leafSlot->radius >> 12;
    frame.offsetX.vz = CosFixed(frame.leafSlot->angle) * frame.leafSlot->radius >> 12;
    frame.rotX.vx = frame.leafSlot->spinAngle;
    frame.rotX.vy = frame.leafSlot->spinAngle << 1;
    frame.rotX.vz = 0;
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, (int *)&g_BattleSpecialAutumnLeavesModelConfig);
    frame.rotMatrix.t[0] = frame.leafSlot->posX + frame.offsetX.vx;
    frame.rotMatrix.t[1] = frame.leafSlot->posY;
    frame.rotMatrix.t[2] = frame.leafSlot->posZ + frame.offsetX.vz;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.worldMatrix);
    SetRotMatrix(&frame.worldMatrix);
    SetTransMatrix(&frame.worldMatrix);
    g_BattleSpecialAutumnLeavesModelConfig.konohaModelConfigStepShift26 = g_BattleSpecialAutumnLeavesStep << 4;
    if (g_BattleSpecialAutumnLeavesStep == 8) {
        g_BattleSpecialAutumnLeavesModelConfig.modelHandle = Gfx_ModelTableGetRenderObject(frame.modelIdZero[frame.leafSlot->modelIndex], g_BattleSpecialAutumnLeavesLoadedRes);
    } else {
        g_BattleSpecialAutumnLeavesModelConfig.modelHandle = Gfx_ModelTableGetRenderObject(frame.modelIdThree[frame.leafSlot->modelIndex], g_BattleSpecialAutumnLeavesLoadedRes);
    }
    g_BattleSpecialAutumnLeavesDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialAutumnLeavesModelConfig.modelHandle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialAutumnLeavesDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.leafSlot->frameCounter >= 10 && frame.leafSlot->frameCounter < 25) {
        frame.leafSlot->posX += frame.leafSlot->velocityX;
        frame.leafSlot->posZ += frame.leafSlot->velocityZ;
    }
    if (frame.leafSlot->frameCounter >= 25 && frame.leafSlot->frameCounter < 35) {
        frame.leafSlot->posY += (short)((-1600 - frame.leafSlot->posY) / 8);
        frame.leafSlot->radius -= (short)(frame.leafSlot->radius / 6);
    }
    if (frame.leafSlot->frameCounter == 35) {
        frame.offsetX.vx = 0;
        frame.offsetX.vy = 0;
        frame.offsetX.vz = 400;
        frame.rotX.vx = ((rand()) % 4096);
        frame.rotX.vy = ((rand()) % 4096);
        frame.rotX.vz = 0;
        RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
        frame.rotMatrix.t[2] = 0;
        frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
        frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
        SetRotMatrix(&frame.rotMatrix);
        SetTransMatrix(&frame.rotMatrix);
        RotTransPointToInt16(&frame.offsetX, &frame.leafSlot->velocityX, &frame.projDepth);
    }
    if (frame.leafSlot->frameCounter >= 35) {
        frame.leafSlot->radius += 200;
        frame.leafSlot->posX += frame.leafSlot->velocityX;
        frame.leafSlot->posZ += frame.leafSlot->velocityZ;
        frame.leafSlot->posY += frame.leafSlot->velocityY;
    }
    frame.leafSlot->angle += 0x100 /* 22.5 deg */;
    frame.leafSlot->spinAngle += 0x80;
    (frame.leafSlot->frameCounter)++;
    if (frame.leafSlot->frameCounter == 45) {
        frame.leafSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
