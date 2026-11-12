// src/ff7/src/battle/battle_fx_special_self_destruct.cpp
/* Enemy Special Effect [idx 28]: SelfDestruct (JP: jibaku1 / 自爆1 (Self-Destruct 1), asset: "SPECIAL/JIBAKU1") — Self-destruct (0x1ed, Cobrah), Suicide (0x184, Vargid Police), Suicide Yang (0x240), Destroyed (0x197, 0x19a) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Jibaku self-destruct attack: loads its resources, spawns
//   per-target explosions, and drives the explosion sequence and expanding blast sprites.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_3d.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x008a7200
int g_BattleSpecialSelfDestructResourceDefs = 0x00192A39;
char *g_BattleSpecialSelfDestructTimName[1] = {
    // STRING: FF7 0x008a7228
    "ZIBAKU1.TIM"
};
unsigned int g_BattleSpecialSelfDestructSptEntry[4] = {
    // STRING: FF7 0x008a7234
    (unsigned int)"ZIBAKU1.SPT",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x00b6f0a8
int g_BattleSpecialSelfDestructDrawListHandle = 0;
// GLOBAL: FF7 0x00b6f0ac
struct GfxTableCursor *g_BattleSpecialSelfDestructLoadedRes = 0;
// clang-format on

// GLOBAL: FF7 0x008a7218
BattleFxSpriteRequest g_BattleSpecialSelfDestructSpriteReq1 = {0, 0, (char)0x80, (char)0x80, (char)0x80, 0x2c, 0, 0};

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/*
 * Loads and registers resources for the Jibaku1 special effect.
 */
// FUNCTION: FF7 0x005529f0
void __cdecl BattleFx_SpecialLoadResSelfDestruct(void)
{
    GfxModelTableBank *stageSceneHandle;
    GfxResourceRequest requestObj;
    int resourceCount;
    char pathWorkBuffer[200];
    GfxResourceRequest *reqPtrPtr;
    int resourceTable;

    if (g_pBattle3dScene != 0 && g_BattleSpecialSelfDestructLoadedRes == 0) {
        resourceCount = 1;
        resourceTable = (int)&g_BattleSpecialSelfDestructResourceDefs;
        stageSceneHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        reqPtrPtr = &requestObj;
        Gfx_InitResourceRequest(reqPtrPtr);
        reqPtrPtr->groupTag = 0;
        // STRING: FF7 0x008a7240
        reqPtrPtr->resourcePath = (char *)BattleFx_BuildResourcePath("SPECIAL/JIBAKU1", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(resourceCount, resourceTable, reqPtrPtr, &g_BattleSpecialSelfDestructLoadedRes, stageSceneHandle);
    }
}

/*
 * Entry point thunk for the Jibaku1 special effect.
 */
// FUNCTION: FF7 0x00552a80
void __cdecl BattleFx_SpecialEnterSelfDestruct(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResSelfDestruct();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnSelfDestructPerTarget);
    Battle_PlaySfxPannedToActor(0x7C, (short)actorIdx, 0);
}

/*
 * Spawns per-target Jibaku1 explosion tasks.
 */
// FUNCTION: FF7 0x00552ab2
void __cdecl BattleFx_SpecialSpawnSelfDestructPerTarget(int actor, int casterIndex)
{
    BattleFxSelfDestructTimeline *effect;

    effect = (BattleFxSelfDestructTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSelfDestructTimelineTask);
    Battle_GetBoneWorldPos(casterIndex, g_BattleActorRecords[casterIndex].targetBone, (short *)&effect->posX);
    effect->actorIndex = actor;
}

/*
 * Main timeline task controlling self-destruct explosion sequence.
 */
// FUNCTION: FF7 0x00552b04
void __cdecl BattleFx_SpecialSelfDestructTimelineTask(void)
{
    BattleFxSelfDestructTimeline *mainSelf;
    int idx;
    BattleFxPositionedSpriteSlot *spawned;
    int div;
    int mat;

    mainSelf = (BattleFxSelfDestructTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (mainSelf->step >= 0 && mainSelf->step < 0xA) {
        spawned = (BattleFxPositionedSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSelfDestructAnimatedSpriteTask);
        spawned->posX = mainSelf->posX + rand() % 0x3E8 - 0x1F4;
        spawned->posY = mainSelf->posY + rand() % 0x3E8 - 0x1F4;
        spawned->posZ = mainSelf->posZ + rand() % 0x3E8 - 0x1F4;
    }
    if (mainSelf->step == 0) {
        Battle_PostActorHitOrHurtMessage(mainSelf->actorIndex);
    }
    (mainSelf->step)++;
    if (mainSelf->step == 0x14) {
        mainSelf->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animates expanding blast sprites for Jibaku1.
 */
// FUNCTION: FF7 0x00552c14
void __cdecl BattleFx_SpecialSelfDestructAnimatedSpriteTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialSelfDestructSpriteReq1.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecialSelfDestructLoadedRes);
    g_BattleSpecialSelfDestructSpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecialSelfDestructLoadedRes);
    BattleFx_InstallBillboardMatrix((short *)task->data, 0x2000, 0);
    g_BattleSpecialSelfDestructSpriteReq1.frame = task->frameCounter;
    g_BattleSpecialSelfDestructDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialSelfDestructSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSelfDestructDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (g_BattleSpecialSelfDestructSpriteReq1.spriteGroup != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= g_BattleSpecialSelfDestructSpriteReq1.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
