// src/ff7/src/battle/battle_fx_special_liquid_poison.cpp
/* Enemy Special Effect [idx 60]: LiquidPoison (JP: dokueki / 毒液 (Poison Liquid), asset: "SPECIAL/DOKUEKI") — Liquid poison (0x17f, Castanets), Acid Rain (0x28e), Toxic Barf (0x2b6), Gas (0x36d) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Dokueki attack: it loads resources and spawns a
//   per-target timeline that drives variant and drifting sprites, plus a stream spawner that emits
//   stream particles.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include "lib/enums/sound.h"
#include <stdlib.h>
#include "lib/api/sound.h"
#include "lib/codegen.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x008a4fc0
int g_BattleSpecialLiquidPoisonResourceDefs = 0x00192A39;
char *g_BattleFxSpecialLiquidPoisonResourceNames[1] = {
    // STRING: FF7 0x008a5020
    "DOKU1.TIM"
};
unsigned int g_BattleFxSpecialLiquidPoisonResourceNames2[14] = {
    // STRING: FF7 0x008a502c
    (unsigned int)"DOKU1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a5038
    (unsigned int)"BAKU1.TIM",
    // STRING: FF7 0x008a5044
    (unsigned int)"BAKU1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    0x00000000,
    // STRING: FF7 0x008a5050
    (unsigned int)"JO_C03.SPT",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x008a5000
struct BattleSpecialLiquidPoisonColor1 g_BattleSpecialLiquidPoisonSpriteReq1 = {0, 0, -128, -128, -128, {0x2c}, 0, {0}};
#undef g_BattleSpecialLiquidPoisonColor2
// GLOBAL: FF7 0x008a5010
struct BattleSpecialLiquidPoisonColor2 g_BattleSpecialLiquidPoisonColor2 = {0, 0, {0x60, 0x60, 0x60, 0x2c}, 0, {0}};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b6e09c
int g_BattleSpecialLiquidPoisonDrawListHandle = 0;
// GLOBAL: FF7 0x00b6e0a0
struct GfxTableCursor *g_BattleSpecialLiquidPoisonLoadedRes = 0;
// clang-format on

/*
 * Loads resources for the Dokueki special effect when not already loaded.
 */
// FUNCTION: FF7 0x005482a0
void __cdecl BattleFx_SpecialLoadResLiquidPoison(void)
{
    GfxModelTableBank *unitOwner;
    GfxResourceRequest requestSlot;
    int resCategory;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecialLiquidPoisonLoadedRes == 0) {
        resCategory = 3;
        resourceDefsWord = (int)&g_BattleSpecialLiquidPoisonResourceDefs;
        unitOwner = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a505c
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/DOKUEKI", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(resCategory, resourceDefsWord, pRequest, &g_BattleSpecialLiquidPoisonLoadedRes, unitOwner);
    }
}

/*
 * Entry point for the Dokueki special effect, dispatching to begin.
 */
// FUNCTION: FF7 0x00548330
void __cdecl BattleFx_SpecialEnterLiquidPoison(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResLiquidPoison();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnLiquidPoisonPerTarget);
    SoundDispatchCommand(SOUND_SFX_PLAY_CH1, 0x40, 0xB2, 0, 0, 0, 0, 0, 0);
}

/*
 * Spawns Dokueki effect instances for each targeted actor.
 */
// FUNCTION: FF7 0x0054836e
void __cdecl BattleFx_SpecialSpawnLiquidPoisonPerTarget(int actor, int actorId)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxLiquidPoisonTimeline *taskRecord;

    taskRecord = (BattleFxLiquidPoisonTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialLiquidPoisonTimelineTask);
    Battle_GetBoneWorldPos(actorId, g_BattleActorRecords[actorId].bone33, &taskRecord->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &taskRecord->endX);
    taskRecord->actorIndex = actor;
}

/*
 * Timeline task controlling state progression for the Dokueki special effect.
 */
// FUNCTION: FF7 0x005483e6
void __cdecl BattleFx_SpecialLiquidPoisonTimelineTask(void)
{
    short dz;
    BattleFxLiquidPoisonTimeline *pCur;
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxLiquidPoisonParticleRecord *rawParticle;
    int t; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int y; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short dx9;
    short theta;
    int c; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int d; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int e; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int f; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int g; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    pCur = (BattleFxLiquidPoisonTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (pCur->frame == 0) {
        rawParticle = (BattleFxLiquidPoisonParticleRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialLiquidPoisonStreamSpawnerTask);
        rawParticle->angle = ArcTangentFixed(pCur->endX - pCur->startX, pCur->endZ - pCur->startZ);
        rawParticle->posX = pCur->startX;
        rawParticle->posY = pCur->startY;
        rawParticle->posZ = pCur->startZ;
        dx9 = pCur->endX - rawParticle->posX;
        dz = pCur->endZ - rawParticle->posZ;
        rawParticle->distance = SqrtToInt(dx9 * dx9 + dz * dz) / 24;
    }
    if (pCur->frame == 50) {
        Battle_PostActorHitOrHurtMessage(pCur->actorIndex);
        SoundDispatchCommand(SOUND_SFX_PLAY_CH2, 0x40, 0x95, 0, 0, 0, 0, 0, 0);
    }
    if (pCur->frame > 40 && pCur->frame < 48) {
        theta = (pCur->frame - 0x28 << 9) - 0x400;
        rawParticle = (BattleFxLiquidPoisonParticleRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialLiquidPoisonVariantSpriteTask);
        Battle_GetBoneWorldPos(pCur->actorIndex, g_BattleActorRecords[pCur->actorIndex].targetBone, &rawParticle->posX);
        rawParticle->posX += (short)(CosFixed(theta) * 0x320 >> 12);
        rawParticle->posY += (short)(SinFixed(theta) * 0x320 >> 12);
        rawParticle->active = 1;
        rawParticle = (BattleFxLiquidPoisonParticleRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialLiquidPoisonVariantSpriteTask);
        Battle_GetBoneWorldPos(pCur->actorIndex, g_BattleActorRecords[pCur->actorIndex].targetBone, &rawParticle->posX);
        rawParticle->posX += (short)(rand() % 600 - 300);
        rawParticle->posY += (short)(rand() % 600 - 300);
        rawParticle->posZ += (short)(rand() % 600 - 300);
        rawParticle->active = 0;
    }
    (pCur->frame)++;
    if (pCur->frame == 60) {
        pCur->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task to animate and render variant sprites for the Dokueki effect.
 */
// FUNCTION: FF7 0x005486e2
void __cdecl BattleFx_SpecialLiquidPoisonVariantSpriteTask(void)
{
    struct {
        int table[2];
        BattleFxWaterGunSpriteTask *slot;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = (BattleFxWaterGunSpriteTask *)BATTLE_CURRENT_TASK_SLOT;
    frame.table[0] = 1;
    frame.table[1] = 2;
    BattleFx_InstallBillboardMatrix(&frame.slot->posX, 0x2000 /* 2.0 */, 0);
    g_BattleSpecialLiquidPoisonSpriteReq1.frameSelector = frame.slot->frameIndex;
    g_BattleSpecialLiquidPoisonSpriteReq1.blue = (frame.slot->variant - 4 != 0 ? 0x20 : 0) + 0x40;
    g_BattleSpecialLiquidPoisonSpriteReq1.green = g_BattleSpecialLiquidPoisonSpriteReq1.blue;
    g_BattleSpecialLiquidPoisonSpriteReq1.red = g_BattleSpecialLiquidPoisonSpriteReq1.green;
    g_BattleSpecialLiquidPoisonSpriteReq1.resource = Gfx_ModelTableGetLoadedRecord(frame.table[frame.slot->variant], g_BattleSpecialLiquidPoisonLoadedRes);
    if (frame.slot->variant != 0) {
        g_BattleSpecialLiquidPoisonSpriteReq1.object = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    } else {
        g_BattleSpecialLiquidPoisonSpriteReq1.object = Gfx_ModelTableGetSpriteObject(frame.table[frame.slot->variant], g_BattleSpecialLiquidPoisonLoadedRes);
    }
    g_BattleSpecialLiquidPoisonDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialLiquidPoisonSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialLiquidPoisonDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (g_BattleSpecialLiquidPoisonSpriteReq1.resource != 0) {
        (frame.slot->frameIndex)++;
        if (frame.slot->frameIndex >= *(int *)((char *)*(int *)(g_BattleSpecialLiquidPoisonSpriteReq1.resource) + 4)) {
            frame.slot->ownerSlot = 0xFFFF;
        }
    }
}

/*
 * Task to spawn and manage stream particles for the Dokueki effect.
 */
// FUNCTION: FF7 0x00548829
void __cdecl BattleFx_SpecialLiquidPoisonStreamSpawnerTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[60];
        SVECTOR angleX;
        struct { unsigned char pad_to_8[0x8]; int projected; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR pointX; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxNapalmTrailSlot *driftSlot; };
        struct { unsigned char pad_to_38[0x38]; BattleFxPoisonBreathEmit *streamSlot; };
    } frame;
#pragma pack(pop)

    frame.streamSlot = (BattleFxPoisonBreathEmit *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.driftSlot = (BattleFxNapalmTrailSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialLiquidPoisonDriftingAnimatedSpriteTask);
    frame.driftSlot->posX = frame.streamSlot->posX;
    frame.driftSlot->posY = frame.streamSlot->posY;
    frame.driftSlot->posZ = frame.streamSlot->posZ;
    frame.driftSlot->variant = frame.streamSlot->step >> 1;
    frame.pointX.vx = 0;
    frame.pointX.vy = 0;
    frame.pointX.vz = frame.streamSlot->speed;
    frame.angleX.vx = 0;
    frame.angleX.vy = -SinFixed(frame.streamSlot->step * 0x88) / 16;
    frame.angleX.vz = 0x200 /* 45 deg */;
    RotMatrixZYX(&frame.angleX, &frame.rotMatrix);
    RotateMatrixY(frame.streamSlot->angleY, &frame.rotMatrix);
    frame.rotMatrix.t[2] = 0;
    frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
    frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    RotTransPointToInt16(&frame.pointX, &frame.driftSlot->velocityX, &frame.projected);
    (frame.streamSlot->step)++;
    if (frame.streamSlot->step == 32) {
        frame.streamSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task to animate drifting sprites for the Dokueki special effect.
 */
// FUNCTION: FF7 0x00548980
void __cdecl BattleFx_SpecialLiquidPoisonDriftingAnimatedSpriteTask(void)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int size;
    int limit;
    int poolIdx;
    BattleFxLiquidPoisonDriftingAnimatedSprite *drift;

    drift = (BattleFxLiquidPoisonDriftingAnimatedSprite *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecialLiquidPoisonColor2.resource) = Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecialLiquidPoisonLoadedRes);
    g_BattleSpecialLiquidPoisonColor2.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecialLiquidPoisonLoadedRes);
    poolIdx = drift->unusedParam;
    size = 16384;
    limit = size;
    BattleFx_InstallBillboardMatrix(&drift->posX, 0x1800 /* 1.5 */, 0);
    g_BattleSpecialLiquidPoisonColor2.driftSlot = drift->slotIndex;
    if (g_BattleSpecialLiquidPoisonColor2.resource != 0) {
        g_BattleSpecialLiquidPoisonDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialLiquidPoisonColor2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialLiquidPoisonDrawListHandle);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    drift->posX += drift->velocityX;
    drift->posY += drift->velocityY;
    drift->posZ += drift->velocityZ;
    if (g_BattleSpecialLiquidPoisonColor2.resource != 0) {
        (drift->frame)++;
        if (drift->frame >= *(int *)((char *)*(int *)(g_BattleSpecialLiquidPoisonColor2.resource) + 4)) {
            drift->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
