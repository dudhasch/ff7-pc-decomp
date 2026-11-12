// src/ff7/src/battle/battle_fx_special_rocket_launcher.cpp
/* Enemy Special Effect [idx 19]: RocketLauncher (JP: misairu / ミサイル (Missile), asset: "SPECIAL/MISAIRU") — Rocket Launcher (0x123, Proto Machinegun / Palmer), Fire Roulette (0x2d8..0x2da) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Misairu attack: loads its resources, spawns per-target
//   missile launches, and drives the missile trajectory, smoke and explosion sprites and the guided
//   missile model flight.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_3d.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x008a7810
int g_BattleSpecialRocketLauncherResourceDefs = 0x001900B4;
char *g_BattleSpecialRocketLauncherRsdNames[1] = {
    // STRING: FF7 0x008a78b0
    "MISAIRU.RSD"
};
unsigned int g_BattleSpecialRocketLauncherSpriteResourceDefs[18] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a78bc
    (unsigned int)"MISSIL00.TIM",
    // STRING: FF7 0x008a78cc
    (unsigned int)"HUNSYA.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a78d8
    (unsigned int)"MISSIL00.TIM",
    // STRING: FF7 0x008a78e8
    (unsigned int)"KEMU1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a78f4
    (unsigned int)"MISSIL02.TIM",
    // STRING: FF7 0x008a7904
    (unsigned int)"BAKU1.SPT",
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x008a7860
BattleFxSpriteRequest g_BattleSpecialRocketLauncherSpriteReq1 = {0, 0, 0x40, 0x40, 0x40, 0x2c, 0, 0};
// GLOBAL: FF7 0x008a7870
struct BattleSpecialRocketLauncherSpriteReq2 g_BattleSpecialRocketLauncherSpriteReq2 = {{0, 8, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0}, 0, {0x80, 0, 0, 0, 0, 0, 0x80, 0, 0x20, 0, 0, 0}, 0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}, 0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b6f0dc
int g_BattleSpecialRocketLauncherDrawListHandle = 0;
// GLOBAL: FF7 0x00b6f0e0
struct GfxTableCursor *g_BattleSpecialRocketLauncherLoadedRes = 0;
unsigned char g_bssPad_00b6f0e4[4] = {0};
// clang-format on

/*
 * Loads and registers resources for the Misairu special effect.
 */
// FUNCTION: FF7 0x00555760
void __cdecl BattleFx_SpecialLoadResRocketLauncher(void)
{
    GfxModelTableBank *newSceneLink;
    GfxResourceRequest resultPending;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *reqPtrPtr;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecialRocketLauncherLoadedRes == 0) {
        kind = 4;
        resourceDefsWord = (int)&g_BattleSpecialRocketLauncherResourceDefs;
        newSceneLink = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        reqPtrPtr = &resultPending;
        Gfx_InitResourceRequest(reqPtrPtr);
        reqPtrPtr->groupTag = 0;
        // STRING: FF7 0x008a7910
        reqPtrPtr->resourcePath = BattleFx_BuildResourcePath("SPECIAL/MISAIRU", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, reqPtrPtr, &g_BattleSpecialRocketLauncherLoadedRes, newSceneLink);
    }
}

/*
 * Entry point thunk for the Misairu special effect.
 */
// FUNCTION: FF7 0x005557f0
void __cdecl BattleFx_SpecialEnterRocketLauncher(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResRocketLauncher();
    g_BattleSpecialRocketLauncherDrawListHandle = (int)&g_GfxPsxPacketScratch;
    BattleFx_SpecialBeginRocketLauncher(targetMask, actorIdx);
}

/*
 * Initializes resources and starts target tasks for Misairu.
 */
// FUNCTION: FF7 0x00555814
void __cdecl BattleFx_SpecialBeginRocketLauncher(int targetMask, int actorIdx)
{
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnRocketLauncherPerTarget);
    Battle_PlaySfxPannedToActor(0x82, (short)actorIdx, 0x40);
}

/*
 * Spawns per-target missile launch tasks for Misairu.
 */
// FUNCTION: FF7 0x00555844
void __cdecl BattleFx_SpecialSpawnRocketLauncherPerTarget(int actor, int attackerId)
{
    BattleFxRocketLauncherTimeline *taskPtr;

    taskPtr = (BattleFxRocketLauncherTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialRocketLauncherTimelineTask);
    Battle_GetBoneWorldPos(attackerId, g_BattleActorRecords[attackerId].bone34, &taskPtr->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &taskPtr->endX);
    taskPtr->actorIndex = actor;
}

/*
 * Main timeline task controlling missile trajectory and explosion timing.
 */
// FUNCTION: FF7 0x005558ba
void __cdecl BattleFx_SpecialRocketLauncherTimelineTask(void)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxRocketLauncherTimeline *taskPtr;
    int s; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int t; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int y; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int a; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int b; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int c; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short j6;
    BattleFxRocketLauncher *pChild;
    int f; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int g; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int h; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int j; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int k; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    taskPtr = (BattleFxRocketLauncherTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->phase == 0) {
        pChild = (BattleFxRocketLauncher *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialRocketLauncherTask);
        pChild->posX = taskPtr->startX;
        pChild->posY = taskPtr->startY;
        pChild->posZ = taskPtr->startZ;
        pChild->angle = ArcTangentFixed(taskPtr->endX - taskPtr->startX, taskPtr->endZ - taskPtr->startZ);
        pChild->velocityX = (taskPtr->endX - taskPtr->startX) / 0xF;
        pChild->velocityY = (taskPtr->endY - taskPtr->startY) / 0xF;
        pChild->velocityZ = (taskPtr->endZ - taskPtr->startZ) / 0xF;
        pChild->arcVelocityY = 0x32;
    }
    if (taskPtr->phase == 0x14) {
        Battle_PostActorHitOrHurtMessage(taskPtr->actorIndex);
    }
    if (taskPtr->phase > 0x14 && taskPtr->phase < 0x18) {
        for (j6 = 0; j6 < 2; j6++) {
            pChild = (BattleFxRocketLauncher *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialRocketLauncherAnimatedSpriteTask);
            Battle_GetBoneWorldPos(taskPtr->actorIndex, g_BattleActorRecords[taskPtr->actorIndex].targetBone, &pChild->posX);
            pChild->posX += (short)(rand() % 0x3E8 - 0x1F4);
            pChild->posY += (short)(rand() % 0x3E8 - 0x1F4);
            pChild->posZ += (short)(rand() % 0x3E8 - 0x1F4);
        }
    }
    (taskPtr->phase)++;
    if (taskPtr->phase == 0x28) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animates missile smoke trail and explosion sprites for Misairu.
 */
// FUNCTION: FF7 0x00555b0f
void __cdecl BattleFx_SpecialRocketLauncherAnimatedSpriteTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialRocketLauncherSpriteReq1.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(3, g_BattleSpecialRocketLauncherLoadedRes);
    g_BattleSpecialRocketLauncherSpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(3, g_BattleSpecialRocketLauncherLoadedRes);
    BattleFx_InstallBillboardMatrix((short *)(task->data), 0x2000, 0);
    g_BattleSpecialRocketLauncherSpriteReq1.frame = task->frameCounter;
    g_BattleSpecialRocketLauncherDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialRocketLauncherSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialRocketLauncherDrawListHandle);
    if (g_BattleIsPaused == 0 && g_BattleSpecialRocketLauncherSpriteReq1.spriteGroup != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= g_BattleSpecialRocketLauncherSpriteReq1.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Animates guided missile 3D model flight and impact orientation.
 */
// FUNCTION: FF7 0x00555be0
void __cdecl BattleFx_SpecialRocketLauncherTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[132];
        int transDepth;
        struct { unsigned char pad_to_4[0x4]; SVECTOR angleX; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR pointX; };
        struct { unsigned char pad_to_14[0x14]; BattleFxRocketLauncherDrawListHandle *drawPacket; };
        struct { unsigned char pad_to_18[0x18]; int vertexDepth; };
        struct { unsigned char pad_to_1C[0x1C]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_40[0x40]; short stepIndex; };
        struct { unsigned char pad_to_44[0x44]; int screenPoint; };
        struct { unsigned char pad_to_48[0x48]; short stepX; };
        struct { unsigned char pad_to_4C[0x4C]; short stepZ; };
        struct { unsigned char pad_to_50[0x50]; BattleFxRocketLauncherSlot *missileSlot; };
        struct { unsigned char pad_to_58[0x58]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_7C[0x7C]; SVECTOR scratchX; };
    } frame;
#pragma pack(pop)

    frame.missileSlot = (BattleFxRocketLauncherSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialRocketLauncherSpriteReq2.modelHandle = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialRocketLauncherLoadedRes);
    g_BattleSpecialRocketLauncherSpriteReq2.misairuReq2Table32 = Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecialRocketLauncherLoadedRes);
    g_BattleSpecialRocketLauncherSpriteReq2.firstDrawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecialRocketLauncherLoadedRes);
    g_BattleSpecialRocketLauncherSpriteReq2.misairuReq2Table48 = Gfx_ModelTableGetLoadedRecord(2, g_BattleSpecialRocketLauncherLoadedRes);
    g_BattleSpecialRocketLauncherSpriteReq2.secondDrawObj = Gfx_ModelTableGetSpriteObject(2, g_BattleSpecialRocketLauncherLoadedRes);
    frame.angleX.vx = 0xFC00;
    frame.angleX.vy = 0xFC00;
    frame.angleX.vz = 0;
    RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
    RotateMatrixY(frame.missileSlot->angle, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, (int *)&g_BattleSpecialRocketLauncherSpriteReq2);
    frame.rotMatrix.t[0] = frame.missileSlot->posX;
    frame.rotMatrix.t[1] = frame.missileSlot->posY;
    frame.rotMatrix.t[2] = frame.missileSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialRocketLauncherDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialRocketLauncherSpriteReq2.modelHandle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialRocketLauncherDrawListHandle);
    frame.angleX.vy = frame.missileSlot->angle;
    frame.angleX.vz = 0;
    frame.angleX.vx = frame.angleX.vz;
    frame.pointX.vy = 0;
    frame.pointX.vx = frame.pointX.vy;
    frame.pointX.vz = 0xFF9C;
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.missileSlot->posX;
    frame.rotMatrix.t[1] = frame.missileSlot->posY;
    frame.rotMatrix.t[2] = frame.missileSlot->posZ;
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    RotTransPointToInt16(&frame.pointX, (short *)&frame.pointX, &frame.transDepth);
    BattleFx_InstallBillboardMatrix(&frame.pointX, 0x2000, 0);
    g_BattleSpecialRocketLauncherSpriteReq2.misairuReq2FrameIndex44 = frame.missileSlot->frameCounter % ((BattleFxSpriteGroup *)g_BattleSpecialRocketLauncherSpriteReq2.misairuReq2Table32)->header->frameCount;
    g_BattleSpecialRocketLauncherDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialRocketLauncherSpriteReq2.misairuReq2Table32, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialRocketLauncherDrawListHandle);
    if (frame.missileSlot->frameCounter >= 6) {
        frame.scratchX.vx = frame.pointX.vx;
        frame.scratchX.vy = frame.pointX.vy;
        frame.scratchX.vz = frame.pointX.vz;
        frame.stepX = -SinFixed(frame.missileSlot->angle) * ((frame.missileSlot->frameCounter % 2) * 0x50 + 0x50) >> 0xC;
        frame.stepZ = -CosFixed(frame.missileSlot->angle) * ((frame.missileSlot->frameCounter % 2) * 0x50 + 0x50) >> 0xC;
        for (frame.stepIndex = 0; frame.stepIndex < 0xC; frame.stepIndex++) {
            BattleFx_InstallBillboardMatrix(&frame.scratchX, 0x2000, 0);
            g_BattleSpecialRocketLauncherSpriteReq2.misairuReq2HalfWord60 = frame.stepIndex >> 1;
            g_BattleSpecialRocketLauncherDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialRocketLauncherSpriteReq2.misairuReq2Table48, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialRocketLauncherDrawListHandle);
            frame.scratchX.vx += frame.stepX;
            frame.scratchX.vz += frame.stepZ;
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    for (frame.stepIndex = 0; frame.stepIndex < 0xA; frame.stepIndex++) {
        frame.scratchX.vx = frame.pointX.vx + rand() % 0x190 + -0xC8;
        frame.scratchX.vy = frame.pointX.vy + rand() % 0x190 + -0xC8;
        frame.scratchX.vz = frame.pointX.vz + rand() % 0x190 + -0xC8;
        frame.drawPacket = (BattleFxRocketLauncherDrawListHandle *)g_BattleSpecialRocketLauncherDrawListHandle;
        frame.drawPacket->tag = frame.drawPacket->tag & 0xFFFFFF | 0x2000000;
        frame.drawPacket->colorCmd = 0x68;
        frame.drawPacket->colorCmd = 0x6A;
        frame.vertexDepth = Gfx_TransformVertexSlot0ReturnDepth(&frame.scratchX, &frame.screenPoint, &frame.transDepth);
        if (frame.vertexDepth > 0) {
            frame.drawPacket->colorR = 0x80;
            frame.drawPacket->colorG = 0x40;
            frame.drawPacket->colorB = 0x20;
            Gfx_DrawPsxPrimitive(frame.drawPacket);
        }
    }
    if (frame.missileSlot->frameCounter < 6) {
        frame.missileSlot->posX += (short)(frame.missileSlot->velocityX >> 2);
        frame.missileSlot->posY += (short)((frame.missileSlot->velocityY >> 2) + frame.missileSlot->arcVelocityY);
        frame.missileSlot->posZ += (short)(frame.missileSlot->velocityZ >> 2);
        frame.missileSlot->arcVelocityY -= 5;
    }
    if (frame.missileSlot->frameCounter >= 6) {
        frame.missileSlot->posX += frame.missileSlot->velocityX;
        frame.missileSlot->posY += (short)(frame.missileSlot->velocityY - 0xA);
        frame.missileSlot->posZ += frame.missileSlot->velocityZ;
    }
    (frame.missileSlot->frameCounter)++;
    if (frame.missileSlot->frameCounter == 0x14) {
        frame.missileSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
