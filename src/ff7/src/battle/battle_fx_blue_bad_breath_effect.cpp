// src/ff7/src/battle/battle_fx_blue_bad_breath_effect.cpp
// Enemy Skill [11]: Bad Breath (Japanese: 臭い息 Kusai Iki, dev tag: "blue/kusai" / Kusai)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The Bad Breath blue magic effect: expanding breath particle sprites emitted from the
//   target centroid, with sound effects on the timeline.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_fx.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x008aafd0
int g_BattleBlueBadBreathResourceDefs = 0x00192A39;
char *g_BattleFxBlueBadBreathEffectResourceNames[1] = {
    // STRING: FF7 0x008aaff8
    "kusai_1.TIM"
};
unsigned int g_BattleFxBlueBadBreathEffectAssetPrefix[4] = {
    // STRING: FF7 0x008ab004
    (unsigned int)"kusai.SPT",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x008aafe8
struct BattleFxSpriteRequest g_BattleBlueBadBreathSpriteReq = {0, 0, 32, 32, 32, 0x2e, 0, 0};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x00ba5180
struct BattleFxBadBreathParticleRing g_BattleBlueBadBreathParticleCount = {0};
// g_BattleBlueBadBreathParticlePool is indexed by the code as an array. The span runs to the next named
// definition of the TU; the TU's pieces inside 0xba5190..0xba5f68 are aliases.
// GLOBAL: FF7 0x00ba5190
unsigned char g_BattleBlueBadBreathParticlePool[3544] = {0};
// GLOBAL: FF7 0x00ba5f68
int g_BattleBlueBadBreathTargetMask = 0;
// GLOBAL: FF7 0x00ba5f6c
struct GfxTableCursor *g_BattleBlueBadBreathLoadedResTable = 0;
// clang-format on

/* Loads Bad Breath blue magic resources. */
// FUNCTION: FF7 0x00571940
void __cdecl BattleFx_BlueLoadResBadBreath(void)
{
    GfxModelTableBank *sceneUserValue;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleBlueBadBreathLoadedResTable == 0) {
        kind = 1;
        resourceDefsWord = (int)&g_BattleBlueBadBreathResourceDefs;
        sceneUserValue = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008ab010
        pRequest->resourcePath = BattleFx_BuildResourcePath("blue/kusai", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleBlueBadBreathLoadedResTable, sceneUserValue);
    }
}

/* Entry point for Bad Breath blue magic effect. */
// FUNCTION: FF7 0x005719d0
void __cdecl BattleFx_BlueEnterBadBreath(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResBadBreath();
    BattleFx_BlueBeginBadBreath(targetMask, actorIdx);
}

/* Initializes setup task for Bad Breath. */
// FUNCTION: FF7 0x005719ea
void __cdecl BattleFx_BlueBeginBadBreath(int targetMask, int actorIdx)
{
    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    Battle_AllocateResourceSlot(BattleFx_BlueBadBreathSetupTask);
    g_BattleBlueBadBreathTargetMask = targetMask;
    (g_BattleBlueBadBreathParticleCount.ringIndex) = 0;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0xA2);
}

/* Sets up target centroid and spawns timeline task for Bad Breath. */
// FUNCTION: FF7 0x00571a48
void __cdecl BattleFx_BlueBadBreathSetupTask(void)
{
    BattleFxBlueAqualungSlot *task;

    task = (BattleFxBlueAqualungSlot *)BATTLE_CURRENT_TASK_SLOT;
    task->state = 0xFFFF;
    task = (BattleFxBlueAqualungSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueBadBreathTimelineTask);
    Battle_GetActorMaskCentroid(g_BattleBlueBadBreathTargetMask, (short *)((char *)task + 4));
    task->range = 0xFAC0;
    task->baseValue = 0x8C0;
    task->baseTimes2 = task->baseValue << 1;
    task->step = -(task->range >> 1);
    task->stepTimes3 = task->step * 2 + task->step;
    task->step += task->step;
    task->particleScale = 0x27D2;
}

/* Drives Bad Breath particle emission timeline and sound effects. */
// FUNCTION: FF7 0x00571b06
void __cdecl BattleFx_BlueBadBreathTimelineTask(void)
{
    int idx;
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int ringIndex;
    int baseSize;
    int bytesRand;
    BattleFxBadBreathRecord *task;
    struct BattleTaskSlot *pNewTask;

    if (g_BattleIsPaused != 0) {
        return;
    }
    task = (BattleFxBadBreathRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (task->frame > 1 && task->frame < 33) {
        ringIndex = (g_BattleBlueBadBreathParticleCount.ringIndex);
        pNewTask = BATTLE_NEW_TASK_SLOT(BattleFx_BlueBadBreathAnimatedSpritesTask);
        ((BattleFxBerserkFadeSprite *)pNewTask)->ringIndex = ringIndex;
        for (idx = 0; idx < 4; idx++) {
            bytesRand = rand() % 4096 + 2048;
            *(short *)((char *)&(((BattleFxBadBreathParticleRing *)&(g_BattleBlueBadBreathParticleCount.ringIndex))->posX) + (ringIndex << 4)) = task->baseX + rand() % task->jitterRange - task->jitterOffset;
            *(short *)((char *)&(((BattleBlueBadBreathParticleCountObject *)&(g_BattleBlueBadBreathParticleCount.ringIndex))->kusaiParticlePosY) + (ringIndex << 4)) = task->baseY - rand() % task->jitterRangeY + task->jitterOffsetY;
            *(short *)((char *)&(((BattleFxBadBreathParticleRing *)&(g_BattleBlueBadBreathParticleCount.ringIndex))->posZ) + (ringIndex << 4)) = task->baseZ + rand() % task->jitterRange - task->jitterOffset;
            baseSize = task->scale * bytesRand >> 12;
            ((BattleFxBlueBadBreathParticle *)&g_BattleBlueBadBreathParticlePool)[ringIndex].timeline = baseSize;
            ringIndex++;
            if (ringIndex >= 222) {
                ringIndex = 0;
            }
        }
        (g_BattleBlueBadBreathParticleCount.ringIndex) = ringIndex;
    }
    if (task->frame == 40) {
        for (idx = 0; idx < 10; idx++) {
            if ((g_BattleBlueBadBreathTargetMask & 1 << idx) != 0) {
                Battle_PostActorHitOrHurtMessage(idx);
            }
        }
    }
    if (task->frame >= 48) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frame)++;
}

/* Animates and submits expanding breath particle sprites. */
// FUNCTION: FF7 0x00571cfb
void __cdecl BattleFx_BlueBadBreathAnimatedSpritesTask(void)
{
    int i, slotIdx;
    BattleFxBerserkFadeSprite *pSprite;

    pSprite = (BattleFxBerserkFadeSprite *)BATTLE_CURRENT_TASK_SLOT;
    (((BattleFxSpriteRequest *)&(g_BattleBlueBadBreathSpriteReq.spriteGroup))->frame) = pSprite->frame;
    slotIdx = pSprite->ringIndex;
    (g_BattleBlueBadBreathSpriteReq.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleBlueBadBreathLoadedResTable);
    (((BattleFxSpriteRequest *)&(g_BattleBlueBadBreathSpriteReq.spriteGroup))->drawObj) = Gfx_ModelTableGetSpriteObject(0, g_BattleBlueBadBreathLoadedResTable);
    for (i = 0; i < 4; i++) {
        BattleFx_InstallBillboardMatrix((void *)((slotIdx << 4) + (int)&(((BattleFxBadBreathParticleRing *)&(g_BattleBlueBadBreathParticleCount.ringIndex))->posX)), (short)((BattleFxBlueBadBreathParticle *)&g_BattleBlueBadBreathParticlePool)[slotIdx].timeline, -(((BattleFxBlueBadBreathParticle *)&g_BattleBlueBadBreathParticlePool)[slotIdx].timeline >> 2));
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&(g_BattleBlueBadBreathSpriteReq.spriteGroup), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
        slotIdx++;
        if (slotIdx >= 222) {
            slotIdx = 0;
        }
    }
    if (pSprite->frame >= 11) {
        pSprite->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (pSprite->frame)++;
}
} /* extern "C" */
