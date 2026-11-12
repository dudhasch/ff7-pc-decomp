// src/ff7/src/battle/battle_fx_special_shady_breath.cpp
/* Enemy Special Effect [idx 77]: ShadyBreath (JP: ayasii / 怪しい (Strange/Shady), asset: "special/ayasii") — Shady Breath (0x18d, Sample:H0512), Funny Breath (0x233, Dorky Face) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Ayasii': a setup task with a timeline and scattered sprites.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
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
// GLOBAL: FF7 0x008a1120
int g_BattleSpecShadyBreathResourceDefs = 0x00192A39;
char *g_BattleFxSpecialShadyBreathResourceNames[1] = {
    // STRING: FF7 0x008a1148
    "AYASII_1.TIM"
};
unsigned int g_BattleFxSpecialShadyBreathAssetPrefix[4] = {
    // STRING: FF7 0x008a1158
    (unsigned int)"AYASII.SPT",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x008a1138
struct BattleFxSpriteRequest g_BattleSpecShadyBreathSpriteReq = {0, 0, 32, 32, 32, 0x2e, 0, 0};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b60ce0
struct BattleFxBadBreathParticleRing g_BattleSpecShadyBreathTargetCoords = {0};
// g_BattleSpecShadyBreathTargetCoords.ringIndex is the ring index. The ring of 222 BattleFxShadyBreathLightPoint
// records starts at its posX; g_BattleSpecShadyBreathLightRecords starts eight bytes into the first point,
// so its BattleFxShadyBreathLightRecord[i].scale is the scale of point i.
// GLOBAL: FF7 0x00b60cf0
unsigned char g_BattleSpecShadyBreathLightRecords[3544] = {0};
// GLOBAL: FF7 0x00b61ac8
int g_BattleSpecShadyBreathTargetMask = 0;
// GLOBAL: FF7 0x00b61acc
struct GfxTableCursor *g_BattleSpecShadyBreathLoadedResTable = 0;
// clang-format on

/*
 * When the engine context exists and g_BattleSpecShadyBreathLoadedResTable is empty, builds special/ayasii with a default-
 * duration record and creates resource type 1 using g_BattleSpecShadyBreathResourceDefs.
 */
// FUNCTION: FF7 0x00531fe0
void __cdecl BattleFx_SpecialLoadResShadyBreath(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest currentRecord;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecShadyBreathLoadedResTable == 0) {
        kind = 1;
        resourceDefsWord = (int)&g_BattleSpecShadyBreathResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a1164
        pRequest->resourcePath = BattleFx_BuildResourcePath("special/ayasii", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecShadyBreathLoadedResTable, curOwnerCtx);
    }
}

/*
 * Loads the Ayasii resources, then starts the effect on the target mask.
 */
// FUNCTION: FF7 0x00532070
void __cdecl BattleFx_SpecialEnterShadyBreath(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResShadyBreath();
    BattleFx_SpecialBeginShadyBreath(targetMask, actorIdx);
}

/*
 * Allocates a slot for BattleFx_SpecialShadyBreathSetupTask, stores targetMask in g_BattleSpecShadyBreathTargetMask, clears g_BattleSpecShadyBreathTargetCoords, and
 * dispatches opcode 0x20 with entity-derived pan and sound 0xA2.
 */
// FUNCTION: FF7 0x0053208a
void __cdecl BattleFx_SpecialBeginShadyBreath(int targetMask, int actorIdx)
{
    Battle_AllocateResourceSlot(BattleFx_SpecialShadyBreathSetupTask);
    g_BattleSpecShadyBreathTargetMask = targetMask;
    g_BattleSpecShadyBreathTargetCoords.ringIndex = 0;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0xA2);
}

// Retires the current slot, then allocates a BattleFx_SpecialShadyBreathTimelineTask slot centred on the
// centroid of g_BattleSpecShadyBreathTargetMask, with its height replaced by -1344 (0xFAC0). The points
// scatter by jitterOffset 0x8C0 over jitterRange twice that on X and Z, and on Y from 672 below to
// 1344 above that height. particleScale is 0x27D2.
// FUNCTION: FF7 0x005320ca
void __cdecl BattleFx_SpecialShadyBreathSetupTask(void)
{
    BattleFxShadyBreathTimeline *task;

    task = (BattleFxShadyBreathTimeline *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    task = (BattleFxShadyBreathTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialShadyBreathTimelineTask);
    Battle_GetActorMaskCentroid(g_BattleSpecShadyBreathTargetMask, &task->centerX);
    task->centerY = 0xFAC0;
    task->jitterOffset = 0x8C0;
    task->jitterRange = task->jitterOffset << 1;
    task->jitterOffsetY = -(task->centerY >> 1);
    task->jitterRangeY = task->jitterOffsetY * 2 + task->jitterOffsetY;
    task->jitterOffsetY += task->jitterOffsetY;
    task->particleScale = 0x27D2;
}

/*
 * Counts frames; between frames 2 and 32 it scatters four random light points per frame into the ring
 * of light records and starts a scattered-sprites task, and at frame 40 it allocates a typed record
 * for each target in the mask. Retires at frame 48.
 */
// FUNCTION: FF7 0x00532188
void __cdecl BattleFx_SpecialShadyBreathTimelineTask(void)
{
    int spriteIndex;
    int ringIndex;
    int scaleJitter;
    BattleFxShadyBreathTimeline *task;
    BattleFxBerserkFadeSprite *pNewTask;
    int spriteScale;

    if (g_BattleIsPaused != 0) {
        return;
    }
    task = (BattleFxShadyBreathTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (task->frameCount > 1 && task->frameCount < 33) {
        ringIndex = g_BattleSpecShadyBreathTargetCoords.ringIndex;
        pNewTask = (BattleFxBerserkFadeSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialShadyBreathScatteredSpritesTask);
        pNewTask->ringIndex = ringIndex;
        for (spriteIndex = 0; spriteIndex < 4; spriteIndex++) {
            scaleJitter = rand() % 0x1000 + 0x800;
            ((BattleFxShadyBreathLightPoint *)&g_BattleSpecShadyBreathTargetCoords.posX)[ringIndex].x = task->centerX + rand() % task->jitterRange - task->jitterOffset;
            ((BattleFxShadyBreathLightPoint *)&g_BattleSpecShadyBreathTargetCoords.posX)[ringIndex].y = task->centerY - rand() % task->jitterRangeY + task->jitterOffsetY;
            ((BattleFxShadyBreathLightPoint *)&g_BattleSpecShadyBreathTargetCoords.posX)[ringIndex].z = task->centerZ + rand() % task->jitterRange - task->jitterOffset;
            spriteScale = task->particleScale * scaleJitter >> 12;
            ((BattleFxShadyBreathLightRecord *)&g_BattleSpecShadyBreathLightRecords)[ringIndex].scale = spriteScale;
            ringIndex++;
            if (ringIndex >= 222) {
                ringIndex = 0;
            }
        }
        g_BattleSpecShadyBreathTargetCoords.ringIndex = ringIndex;
    }
    if (task->frameCount == 40) {
        for (spriteIndex = 0; spriteIndex < 10; spriteIndex++) {
            if ((g_BattleSpecShadyBreathTargetMask & 1 << spriteIndex) != 0) {
                Battle_PostActorHitOrHurtMessage(spriteIndex);
            }
        }
    }
    if (task->frameCount >= 48) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCount)++;
}

/*
 * Selects sprite field zero, then draws four consecutive light records from the ring index stored in the
 * task, wrapping the index at 222. Each light's scale sizes the billboard and its negative quarter is the
 * push length, chaining g_BattleFxActiveDrawListPtr. Retires at frame 11 or above, and counts a frame only
 * when unpaused.
 */
// FUNCTION: FF7 0x0053237d
void __cdecl BattleFx_SpecialShadyBreathScatteredSpritesTask(void)
{
    int spriteIndex, ringIndex;
    BattleFxBerserkFadeSprite *task;

    task = (BattleFxBerserkFadeSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecShadyBreathSpriteReq.frame = task->frame;
    ringIndex = task->ringIndex;
    g_BattleSpecShadyBreathSpriteReq.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecShadyBreathLoadedResTable);
    g_BattleSpecShadyBreathSpriteReq.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecShadyBreathLoadedResTable);
    for (spriteIndex = 0; spriteIndex < 4; spriteIndex++) {
        BattleFx_InstallBillboardMatrix(&((BattleFxShadyBreathLightPoint *)&g_BattleSpecShadyBreathTargetCoords.posX)[ringIndex], (short)((BattleFxShadyBreathLightRecord *)&g_BattleSpecShadyBreathLightRecords)[ringIndex].scale, -(((BattleFxShadyBreathLightRecord *)&g_BattleSpecShadyBreathLightRecords)[ringIndex].scale >> 2));
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleSpecShadyBreathSpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
        ringIndex++;
        if (ringIndex >= 222) {
            ringIndex = 0;
        }
    }
    if (task->frame >= 11) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (task->frame)++;
}
} /* extern "C" */
