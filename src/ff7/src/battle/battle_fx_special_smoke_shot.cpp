// src/ff7/src/battle/battle_fx_special_smoke_shot.cpp
/* Enemy Special Effect [idx 64]: SmokeShot (JP: kemu1 / 煙1 (Smoke 1), asset: "SPECIAL/KEMU1") — Smoke Shot (0x119, Sweeper), Escape (0x138), Petrify Smog (0x1e8), Throw (0x361); also GeneralDispatchTable[9] */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for Kemu1: loads its resources on first use, sets up render state
//   on entry, and spawns one projectile task per target. Each projectile flies its trajectory and
//   emits smoke, and expanding smoke plume sprites are drawn by their own task.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "lib/types/graphics.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/codegen.h"
extern "C" {

#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x008f3688
int g_BattleSpecSmokeShot_ResourceDefs = 0x00192A39;
char *g_BattleFxSpecialSmokeShotResourceNames[1] = {
    // STRING: FF7 0x008f36b0
    "THREAT.TIM"
};
unsigned int g_BattleFxSpecialSmokeShotAssetPrefix[4] = {
    // STRING: FF7 0x008f36bc
    (unsigned int)"KEMU1.SPT",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x00bdc990
int g_BattleSpecSmokeShot_DrawListHandle = 0;
// GLOBAL: FF7 0x00bdc994
struct GfxTableCursor *g_BattleSpecSmokeShot_LoadedRes = 0;
// clang-format on

// clang-format off
#undef g_BattleSpecSmokeShot_SpriteReq
// GLOBAL: FF7 0x008f36a0
struct BattleFxSpriteRequest g_BattleSpecSmokeShot_SpriteReq = {0, 0, -128, -128, -128, 0x2c, 0, 0};
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/*
 * Loads resources for the Kemu1 special effect when not already loaded.
 */
// FUNCTION: FF7 0x005aff70
void __cdecl BattleFx_SpecialLoadResSmokeShot(void)
{
    GfxModelTableBank *sceneContext;
    GfxResourceRequest record;
    int id;
    char pathBuffer[200];
    GfxResourceRequest *request;
    int defPtr;

    if (g_pBattle3dScene != 0 && g_BattleSpecSmokeShot_LoadedRes == 0) {
        id = 1;
        defPtr = (int)&g_BattleSpecSmokeShot_ResourceDefs;
        sceneContext = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &record;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x008f36c8
        request->resourcePath = BattleFx_BuildResourcePath("SPECIAL/KEMU1", pathBuffer);
        Gfx_LoadModelTableIfAbsent(id, defPtr, request, &g_BattleSpecSmokeShot_LoadedRes, sceneContext);
    }
}

/*
 * Main entry point initializing render state for Kemu1.
 */
// FUNCTION: FF7 0x005b0000
void __cdecl BattleFx_SpecialEnterSmokeShot(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResSmokeShot();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnSmokeShotPerTarget);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[actorIdx].posX), 0x31);
}

/*
 * Spawns projectile task for a single Kemu1 target.
 */
// FUNCTION: FF7 0x005b0046
void __cdecl BattleFx_SpecialSpawnSmokeShotPerTarget(int actor, int sourceIndex)
{
    short targetPos[3];
    BattleFxSmokeShotProjectile *projectile;

    projectile = (BattleFxSmokeShotProjectile *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSmokeShotProjectileTask);
    Battle_GetBoneWorldPos(sourceIndex, g_BattleActorRecords[sourceIndex].bone33, &projectile->posX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, targetPos);
    projectile->velocityX = (targetPos[0] - projectile->posX) / 16;
    projectile->velocityY = (targetPos[1] - projectile->posY) / 16;
    projectile->velocityZ = (targetPos[2] - projectile->posZ) / 16;
    projectile->actorIndex = (short)actor;
}

/*
 * Drives projectile flight trajectory and smoke spawning for Kemu1.
 */
// FUNCTION: FF7 0x005b0112
void __cdecl BattleFx_SpecialSmokeShotProjectileTask(void)
{
    BattleFxSmokeShotSpriteTask *smoke;
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxSmokeShotProjectile *task;

    task = (BattleFxSmokeShotProjectile *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posX += task->velocityX;
    task->posY += task->velocityY;
    task->posZ += task->velocityZ;
    smoke = (BattleFxSmokeShotSpriteTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSmokeShotAnimatedSpriteTask);
    *(Int64Pair *)&smoke->posX = *(Int64Pair *)&task->posX;
    smoke->scale = 0x2000;
    (task->frame)++;
    if (task->frame >= 0x10) {
        BattleFx_HitMarkSpawnAtActorBone(task->actorIndex, 0x32);
        Battle_PostActorHitOrHurtMessage(task->actorIndex);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates and renders expanding smoke plume sprites for Kemu1.
 */
// FUNCTION: FF7 0x005b01fd
void __cdecl BattleFx_SpecialSmokeShotAnimatedSpriteTask(void)
{
    BattleFxSmokeShotSpriteTask *task;

    task = (BattleFxSmokeShotSpriteTask *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecSmokeShot_SpriteReq.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecSmokeShot_LoadedRes);
    g_BattleSpecSmokeShot_SpriteReq.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecSmokeShot_LoadedRes);
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->scale, 0);
    g_BattleSpecSmokeShot_SpriteReq.frame = task->frame;
    g_BattleSpecSmokeShot_DrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecSmokeShot_SpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecSmokeShot_DrawListHandle);
    if (g_BattleIsPaused == 0 && g_BattleSpecSmokeShot_SpriteReq.spriteGroup != 0) {
        (task->frame)++;
        if (task->frame >= g_BattleSpecSmokeShot_SpriteReq.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
