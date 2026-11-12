// src/ff7/src/battle/battle_fx_limit_greased_lightning.cpp
// Limit Break [49] (Yuffie Lv 1-1): Greased Lightning (Japanese: 疾風迅雷, dev tag: "LIMIT2/JINRAI" / Jinrai)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Jinrai limit effect: loads its resources, registers a per-target callback that spawns
//   timelines at chosen actor bone positions, and drives sprite animations.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/types/battle_limit.h"
#include "ff7/battle_tasks.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/api/graphics.h"
extern "C" {

#pragma intrinsic(memset)

// clang-format off
// GLOBAL: FF7 0x00806598
int g_BattleLimitGreasedLightningResourceDef = 0x00192A39;
char *g_BattleFxLimitGreasedLightningResourceNames[1] = {
    // STRING: FF7 0x008065c0
    "JINRAI.TIM"
};
unsigned int g_BattleFxLimitGreasedLightningAssetPrefix[4] = {
    // STRING: FF7 0x008065cc
    (unsigned int)"JINRAI.SPT",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x008065b0
struct BattleLimitGreasedLightningSpriteRequest g_BattleLimitGreasedLightningSpriteRequest = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
// clang-format on

#pragma intrinsic(memcpy)
#pragma function(memcpy)

// clang-format off
// GLOBAL: FF7 0x00aef7f8
struct GfxTableCursor *g_BattleLimitGreasedLightningLoadedResTable = 0;
unsigned char g_bssPad_00aef7fc[4] = {0};
// clang-format on

// When scene pointer is nonzero and g_BattleLimitGreasedLightningLoadedResTable is zero, initializes local
// resource request with default duration and the LIMIT2/JINRAI path, then calls
// Gfx_LoadModelTableIfAbsent with g_BattleLimitGreasedLightningResourceDef, caching the table in
// g_BattleLimitGreasedLightningLoadedResTable under the scene's model table bank. Otherwise does nothing.
// FUNCTION: FF7 0x004ae840
void __cdecl BattleFx_LimitLoadResGreasedLightning(void)
{
    GfxModelTableBank *ownerHandle;
    GfxResourceRequest recordBuf;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int stepResourceDef;

    if (g_pBattle3dScene != 0 && g_BattleLimitGreasedLightningLoadedResTable == 0) {
        kind = 1;
        stepResourceDef = (int)&g_BattleLimitGreasedLightningResourceDef;
        ownerHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &recordBuf;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008065d8
        pRequest->resourcePath = BattleFx_BuildResourcePath("LIMIT2/JINRAI", pathText);
        Gfx_LoadModelTableIfAbsent(kind, stepResourceDef, pRequest, &g_BattleLimitGreasedLightningLoadedResTable, ownerHandle);
    }
}

// Loads the Jinrai limit resources, starts its effect task and returns success.
// Calls BattleFx_LimitLoadResGreasedLightning, then BattleFx_LimitRegisterGreasedLightningPerTargetCallback, and returns zero.
// FUNCTION: FF7 0x004ae8d0
int __cdecl BattleFx_LimitEnterGreasedLightning(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResGreasedLightning();
    BattleFx_LimitRegisterGreasedLightningPerTargetCallback(targetMask, actorIdx);
    return 0;
}

// Calls Battle_CreateBitPatternCallbackTask with both arguments, mode 4, and callback BattleFx_LimitSpawnGreasedLightningPerTarget.
// FUNCTION: FF7 0x004ae8ec
void __cdecl BattleFx_LimitRegisterGreasedLightningPerTargetCallback(int targetMask, int actorIdx)
{
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, (void *)BattleFx_LimitSpawnGreasedLightningPerTarget);
}

// Allocates a BattleFx_LimitGreasedLightningTimelineTask slot and fills its two bone positions with
// Battle_GetBoneWorldPos: the targetBone position of the attacker (actorIdx), then that of the
// target (actor). The slot then stores target and actorIndex.
// FUNCTION: FF7 0x004ae908
void __cdecl BattleFx_LimitSpawnGreasedLightningPerTarget(int actor, int actorIdx)
{
    BattleFxGreasedLightningTask *slot;

    slot = (BattleFxGreasedLightningTask *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitGreasedLightningTimelineTask);
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].targetBone, (short *)((int)slot + 4));
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)((int)slot + 0xC));
    slot->target = actor;
    slot->actorIndex = actorIdx;
}

// Per-frame handler for the Jinrai effect. It does nothing while g_BattleIsPaused is set. On steps 0x16 and 0x37 it allocates a
// BattleFx_LimitGreasedLightningSpriteAnimationTask slot, copies posX, posY and posZ into it, and plays sound
// 0x155 panned to actorIndex. On step 0x1E it plays 0x30 panned to actorIndex, and on step 0x37 it
// allocates the target's typed record. Then it advances step, and at step 0x46 it writes
// 0xFFFF to state to retire the slot.
// FUNCTION: FF7 0x004ae989
void __cdecl BattleFx_LimitGreasedLightningTimelineTask(void)
{
    BattleFxGreasedLightningTask *task;
    BattleFxPositionedSpriteSlot *child;

    task = (BattleFxGreasedLightningTask *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->step == 0x16 || task->step == 0x37) {
        child = (BattleFxPositionedSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitGreasedLightningSpriteAnimationTask);
        child->posX = task->posX;
        child->posY = task->posY;
        child->posZ = task->posZ;
        Battle_PlaySfxPannedToActor(0x155, task->actorIndex, 0);
    }
    if (task->step == 0x1E) {
        Battle_PlaySfxPannedToActor(0x30, task->actorIndex, 0);
    }
    if (task->step == 0x37) {
        Battle_PostActorHitOrHurtMessage(task->target);
    }
    task->step += 1;
    if (task->step == 0x46) {
        task->state = 0xFFFF;
    }
}

// One frame of the Jinrai sprite effect, same as BattleFx_LimitBloodfestSpriteAnimationTask at
// billboard scale 0x3000: takes sprite handle and resource from entry 0 of
// g_BattleLimitGreasedLightningLoadedResTable, sets the request's frame from the slot's frameCounter and
// submits the draw. While unpaused the frame advances until it reaches the animation length, which
// frees the slot.
// FUNCTION: FF7 0x004aea8c
void __cdecl BattleFx_LimitGreasedLightningSpriteAnimationTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitGreasedLightningSpriteRequest.spriteHandle = Gfx_ModelTableGetLoadedRecord(0, g_BattleLimitGreasedLightningLoadedResTable);
    g_BattleLimitGreasedLightningSpriteRequest.res = Gfx_ModelTableGetSpriteObject(0, g_BattleLimitGreasedLightningLoadedResTable);
    BattleFx_InstallBillboardMatrix(task->data, 0x3000, 0);
    g_BattleLimitGreasedLightningSpriteRequest.frame = task->frameCounter;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleLimitGreasedLightningSpriteRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0 && g_BattleLimitGreasedLightningSpriteRequest.spriteHandle != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= *(int *)((char *)*(int *)g_BattleLimitGreasedLightningSpriteRequest.spriteHandle + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
