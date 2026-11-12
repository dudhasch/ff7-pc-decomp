// src/ff7/src/battle/battle_fx_special_stare_down.cpp
/* Enemy Special Effect [idx 16]: StareDown (JP: hikari / 光 (Light/Flash), asset: "SPECIAL/HIKARI") — Stare Down (0x248, Jersey), Darkeye (0x1d9, Grangalan), Triclops (0x1da), Stop Eye (0x1db) */
// Contents: Battle special effect for the Hikari attack: loads resources when needed, runs
//   per-target timelines that control target visibility, and drives primary flash sprites and
//   secondary light flare sprites.

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

// clang-format off
// GLOBAL: FF7 0x008a8040
int g_BattleSpecialStareDownResourceDefs = 0x00192A39;
char *g_BattleFxSpecialStareDownResourceNames[1] = {
    // STRING: FF7 0x008a8088
    "HIKARI1.TIM"
};
unsigned int g_BattleFxSpecialStareDownResourceNames2[8] = {
    // STRING: FF7 0x008a8094
    (unsigned int)"HIKARI1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a80a0
    (unsigned int)"HIT1.TIM",
    // STRING: FF7 0x008a80ac
    (unsigned int)"HIT1.SPT",
    0x00000000,
    0x00000000
};
#undef g_BattleSpecialStareDownSpriteData
// GLOBAL: FF7 0x008a8068
struct BattleSpecialStareDownSpriteData g_BattleSpecialStareDownSpriteData = {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}, 0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// clang-format on


// clang-format off
// GLOBAL: FF7 0x00b71878
short g_BattleSpecialStareDownSavedTargetStates[10] = {0};
// GLOBAL: FF7 0x00b7188c
int g_BattleSpecialStareDownDrawListHandle = 0;
// GLOBAL: FF7 0x00b71890
struct GfxTableCursor *g_BattleSpecialStareDownLoadedRes = 0;
unsigned char g_bssPad_00b71894[4] = {0};
// clang-format on

/*
 * Loads resources for the Hikari special effect when not already loaded.
 */
// FUNCTION: FF7 0x00559470
void __cdecl BattleFx_SpecialLoadResStareDown(void)
{
    GfxModelTableBank *newSceneLink;
    GfxResourceRequest resultPending;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *reqPtrPtr;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecialStareDownLoadedRes == 0) {
        kind = 2;
        resourceDefsWord = (int)&g_BattleSpecialStareDownResourceDefs;
        newSceneLink = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        reqPtrPtr = &resultPending;
        Gfx_InitResourceRequest(reqPtrPtr);
        reqPtrPtr->groupTag = 0;
        // STRING: FF7 0x008a80b8
        reqPtrPtr->resourcePath = (char *)BattleFx_BuildResourcePath("SPECIAL/HIKARI", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, reqPtrPtr, &g_BattleSpecialStareDownLoadedRes, newSceneLink);
    }
}

/*
 * Main entry point for the Hikari special effect.
 */
// FUNCTION: FF7 0x00559500
void __cdecl BattleFx_SpecialEnterStareDown(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResStareDown();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnStareDownPerTarget);
    Battle_PlaySfxPannedToActor(0x6B, (short)actorIdx, 0x40);
}

/*
 * Spawns timeline task for a single Hikari target.
 */
// FUNCTION: FF7 0x00559532
void __cdecl BattleFx_SpecialSpawnStareDownPerTarget(int actor, int attackerId)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int taskPtr;

    taskPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialStareDownTimelineTask);
    Battle_GetBoneWorldPos(attackerId, g_BattleActorRecords[attackerId].bone34, (short *)(taskPtr + 4));
    ((BattleFxStareDownTrack *)taskPtr)->actorIndex = actor;
}

/*
 * Drives the main timeline and target visibility for Hikari.
 */
// FUNCTION: FF7 0x00559586
void __cdecl BattleFx_SpecialStareDownTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[60];
        int rotTransOut;
        struct { unsigned char pad_to_4[0x4]; SVECTOR offX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxStareDownParticle *particle; };
        struct { unsigned char pad_to_34[0x34]; short actorIndex; };
        struct { unsigned char pad_to_38[0x38]; BattleFxStareDownTrack *track; };
    } frame;
#pragma pack(pop)

    frame.track = (BattleFxStareDownTrack *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.track->phase == 0) {
        frame.particle = (BattleFxStareDownParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialStareDownAnimatedSpriteEntry0Task);
        frame.particle->angleX = 0;
        frame.particle->angleX = 0;
        frame.particle->angle = ArcTangentFixed(frame.track->targetX - frame.track->posX, frame.track->targetZ - frame.track->posZ);
        frame.offX.vy = 0;
        frame.offX.vx = frame.offX.vy;
        frame.offX.vz = 0x64;
        RotMatrix((SVECTOR *)&frame.particle->angleX, &frame.rotMatrix);
        frame.rotMatrix.t[2] = 0;
        frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
        frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
        SetRotMatrix(&frame.rotMatrix);
        SetTransMatrix(&frame.rotMatrix);
        RotTransPointToInt16(&frame.offX, (short *)&frame.offX, &frame.rotTransOut);
        frame.particle->posX = frame.track->posX + frame.offX.vx;
        frame.particle->posY = frame.track->posY + frame.offX.vy;
        frame.particle->posZ = frame.track->posZ + frame.offX.vz;
    }
    if (frame.track->phase == 2) {
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        g_BattleFxTimelineState.startValue = 0x1000;
        for (frame.actorIndex = 0; frame.actorIndex < 0xA; frame.actorIndex++) {
            g_BattleSpecialStareDownSavedTargetStates[frame.actorIndex] = (short)*(unsigned char *)&g_BattleActorRecords[frame.actorIndex].drawFlags;
            g_BattleActorRecords[frame.actorIndex].drawFlags |= 2;
        }
    }
    if (frame.track->phase == 4) {
        g_BattleFxTimelineState.startValue = 0;
        BattleFx_StartScreenFlashRamp(0xF);
        for (frame.actorIndex = 0; frame.actorIndex < 0xA; frame.actorIndex++) {
            g_BattleActorRecords[frame.actorIndex].drawFlags = *(char *)((char *)&g_BattleSpecialStareDownSavedTargetStates + frame.actorIndex * 2);
        }
    }
    if (frame.track->phase == 0x14) {
        BattleFx_FadeOutScreenFlashRamp(0xF);
    }
    if (frame.track->phase == 0x1E) {
        Battle_PostActorHitOrHurtMessage(frame.track->actorIndex);
        Battle_PlaySfxPannedToActor(0x95, (short)frame.track->actorIndex, 0x40);
    }
    if (frame.track->phase > 0x1E && frame.track->phase < 0x23) {
        for (frame.actorIndex = 0; frame.actorIndex < 2; frame.actorIndex++) {
            frame.particle = (BattleFxStareDownParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialStareDownAnimatedSpriteEntry1Task);
            Battle_GetBoneWorldPos(frame.track->actorIndex, g_BattleActorRecords[frame.track->actorIndex].targetBone, &frame.particle->posX);
            frame.particle->posX += (short)(rand() % 0x3E8 - 0x1F4);
            frame.particle->posY += (short)(rand() % 0x3E8 - 0x1F4);
            frame.particle->posZ += (short)(rand() % 0x3E8 - 0x1F4);
        }
    }
    (frame.track->phase)++;
    if (frame.track->phase == 0x28) {
        frame.track->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates and renders primary flash sprites for Hikari.
 */
// FUNCTION: FF7 0x0055991a
void __cdecl BattleFx_SpecialStareDownAnimatedSpriteEntry0Task(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecialStareDownSpriteData.spriteResource) = Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecialStareDownLoadedRes);
    g_BattleSpecialStareDownSpriteData.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecialStareDownLoadedRes);
    BattleFx_InstallBillboardMatrix((short *)task->data, 0x1800, 0);
    g_BattleSpecialStareDownSpriteData.hikariSpriteArgWord = task->frameCounter;
    g_BattleSpecialStareDownDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialStareDownSpriteData, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialStareDownDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if ((g_BattleSpecialStareDownSpriteData.spriteResource) != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= *(int *)((char *)*(int *)(g_BattleSpecialStareDownSpriteData.spriteResource) + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Updates and renders secondary light flare sprites for Hikari.
 */
// FUNCTION: FF7 0x005599ed
void __cdecl BattleFx_SpecialStareDownAnimatedSpriteEntry1Task(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialStareDownSpriteData.hikariSpriteDataTable16 = Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecialStareDownLoadedRes);
    g_BattleSpecialStareDownSpriteData.secondDrawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecialStareDownLoadedRes);
    BattleFx_InstallBillboardMatrix((short *)task->data, 0x2000, 0);
    g_BattleSpecialStareDownSpriteData.hikariSpriteArgWord28 = task->frameCounter;
    g_BattleSpecialStareDownDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialStareDownSpriteData.hikariSpriteDataTable16, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialStareDownDrawListHandle);
    if (g_BattleIsPaused == 0 && g_BattleSpecialStareDownSpriteData.hikariSpriteDataTable16 != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= *(int *)((char *)*(int *)g_BattleSpecialStareDownSpriteData.hikariSpriteDataTable16 + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
