// src/ff7/src/battle/battle_fx_limit_boost_jump.cpp
// Limit Break [28] (Cid Lv 1-1): Boost Jump (Japanese: ブーストジャンプ, dev tag: "LIMIT3/B_JUMP" / BJump)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Boost Jump (B_JUMP) limit break effect: it loads the effect resources, then runs the
//   entry point, per-target spawning, a master timeline, and the sprite and moving-sprite tasks for
//   the jump and its impact.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/types/battle_limit.h"
#include "ff7/battle_tasks.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/api/graphics.h"
#include "ff7/types/battle_3d.h"
extern "C" {

#pragma intrinsic(memset)

// clang-format off
// GLOBAL: FF7 0x00809130
int g_BattleLimitBoostJumpResourceDef = 0x00192A39;
char *g_BattleFxLimitBoostJumpResourceNames[1] = {
    // STRING: FF7 0x00809178
    "BOOST00.TIM"
};
unsigned int g_BattleFxLimitBoostJumpResourceNames2[8] = {
    // STRING: FF7 0x00809184
    (unsigned int)"H_JUMP.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x00809190
    (unsigned int)"BOOST01.TIM",
    // STRING: FF7 0x0080919c
    (unsigned int)"HJ_HIT.SPT",
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x00809158
BattleFxSpriteRequest g_BattleLimitBoostJumpSpriteReqPool[2] = {
    {0, 0, 0x80, 0x80, 0x80, 0x2e, 0, 0},
    {0, 0, 0x80, 0x80, 0x80, 0x2e, 0, 0}
};
// clang-format on

#pragma intrinsic(memcpy)
#pragma function(memcpy)

// clang-format off
// GLOBAL: FF7 0x00af0430
struct GfxTableCursor *g_BattleLimitBoostJumpLoadedResTable = 0;
unsigned char g_bssPad_00af0434[4] = {0};
// clang-format on

/*
 * Loads Boost Jump (B_JUMP) limit effect resources (LIMIT3/B_JUMP).
 */
// FUNCTION: FF7 0x004b2a50
void __cdecl BattleFx_LimitLoadResBoostJump(void)
{
    GfxModelTableBank *ownerHandle;
    GfxResourceRequest recordBuf;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int stepResourceDef;

    if (g_pBattle3dScene != 0 && g_BattleLimitBoostJumpLoadedResTable == 0) {
        kind = 2;
        stepResourceDef = (int)&g_BattleLimitBoostJumpResourceDef;
        ownerHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &recordBuf;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008091a8
        pRequest->resourcePath = BattleFx_BuildResourcePath("LIMIT3/B_JUMP", pathText);
        Gfx_LoadModelTableIfAbsent(kind, stepResourceDef, pRequest, &g_BattleLimitBoostJumpLoadedResTable, ownerHandle);
    }
}

/*
 * Entry point for Boost Jump limit break effect.
 */
// FUNCTION: FF7 0x004b2ae0
int __cdecl BattleFx_LimitEnterBoostJump(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResBoostJump();
    BattleFx_LimitRegisterBoostJumpPerTargetCallback(targetMask, actorIdx);
    return 0;
}

/*
 * Registers per-target callback task for Boost Jump.
 */
// FUNCTION: FF7 0x004b2afc
void __cdecl BattleFx_LimitRegisterBoostJumpPerTargetCallback(int targetMask, int sourceActor)
{
    Battle_CreateBitPatternCallbackTask(targetMask, sourceActor, 4, (void *)BattleFx_LimitSpawnBoostJumpPerTarget);
}

/*
 * Spawns Boost Jump effect timeline task for a target.
 */
// FUNCTION: FF7 0x004b2b18
void __cdecl BattleFx_LimitSpawnBoostJumpPerTarget(int actor, int sourceActor)
{
    BattleFxBoostJumpTask *slot;

    slot = (BattleFxBoostJumpTask *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitBoostJumpTimelineTask);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)((int)slot + 0xC));
    slot->targetMask = actor;
    slot->sourceActor = sourceActor;
}

/*
 * Master timeline task for Boost Jump effect sequence.
 */
// FUNCTION: FF7 0x004b2b75
void __cdecl BattleFx_LimitBoostJumpTimelineTask(void)
{
    BattleFxBoostJumpTimeline *taskPtr;
    short i;
    BattleFxSpecialEnergyOrbSlot *emitter;

    taskPtr = (BattleFxBoostJumpTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->step == 0x20) {
        Battle_PlaySfxPannedToActor(0x30, (short)taskPtr->level, 0);
    }
    if (taskPtr->step == 0x3A) {
        for (i = 0; i < 0x10; i++) {
            emitter = (BattleFxSpecialEnergyOrbSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitBoostJumpMovingSpriteTask);
            emitter->posX = taskPtr->posX;
            emitter->posY = 0;
            emitter->posZ = taskPtr->posZ;
            emitter->velocityX = SinFixed(i << 8) * 0xC8 >> 0xC;
            emitter->velocityZ = CosFixed(i << 8) * 0xC8 >> 0xC;
            emitter->velocityY = -rand() % 0xC8;
        }
        BattleFx_HitMarkSpawnScriptAtActorBone(taskPtr->actorIndex, 0x21, 0x1000);
        Battle_PlaySfxPannedToActor(0x15A, (short)taskPtr->level, 0);
    }
    if (taskPtr->step == 0x44) {
        emitter = (BattleFxSpecialEnergyOrbSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitBoostJumpSpriteAnimationTask);
        emitter->posX = taskPtr->posX;
        emitter->posY = taskPtr->posY;
        emitter->posZ = taskPtr->posZ;
        Battle_PostActorHitOrHurtMessage(taskPtr->actorIndex);
        Battle_PlaySfxPannedToActor(0x141, (short)taskPtr->level, 0);
    }
    (taskPtr->step)++;
    if (taskPtr->step == 0x64) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animated sprite task for Boost Jump impact effect.
 */
// FUNCTION: FF7 0x004b2d5c
void __cdecl BattleFx_LimitBoostJumpSpriteAnimationTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitBoostJumpSpriteReqPool[0].spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_BattleLimitBoostJumpLoadedResTable);
    g_BattleLimitBoostJumpSpriteReqPool[0].drawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleLimitBoostJumpLoadedResTable);
    BattleFx_InstallBillboardMatrix(task->data, 0x2000, 0);
    g_BattleLimitBoostJumpSpriteReqPool[0].frame = task->frameCounter;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleLimitBoostJumpSpriteReqPool[0], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0 && g_BattleLimitBoostJumpSpriteReqPool[0].spriteGroup != 0) {
        (task->frameCounter)++;
        if (task->frameCounter == g_BattleLimitBoostJumpSpriteReqPool[0].spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Moving sprite task for Boost Jump projectile effect.
 */
// FUNCTION: FF7 0x004b2e2d
void __cdecl BattleFx_LimitBoostJumpMovingSpriteTask(void)
{
    BattleFxPulseOfLifeSprite *task;

    task = (BattleFxPulseOfLifeSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitBoostJumpSpriteReqPool[1].spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleLimitBoostJumpLoadedResTable);
    g_BattleLimitBoostJumpSpriteReqPool[1].drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleLimitBoostJumpLoadedResTable);
    BattleFx_InstallBillboardMatrix((char *)task + 4, 0x2000, 0);
    g_BattleLimitBoostJumpSpriteReqPool[1].frame = task->frame;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleLimitBoostJumpSpriteReqPool[1], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        task->posX += task->velocityX;
        task->posY += task->velocityY;
        task->posZ += task->velocityZ;
        if (g_BattleLimitBoostJumpSpriteReqPool[1].spriteGroup != 0) {
            (task->frame)++;
            if (task->frame == g_BattleLimitBoostJumpSpriteReqPool[1].spriteGroup->header->frameCount) {
                task->ownerSlot = BATTLE_TASK_FREE;
            }
        }
    }
}
} /* extern "C" */
