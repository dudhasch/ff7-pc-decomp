// src/ff7/src/battle/battle_fx_blue_aqualung_effect.cpp
// Enemy Skill [13]: Aqualung (Japanese: アクアブレス Aqua Breath, dev tag: "blue/aqua" / Aqua)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The Aqualung blue magic effect: expanding water bubble sprites around the target
//   centroid, with water sounds on the timeline.

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
// GLOBAL: FF7 0x008abde0
int g_BattleBlueAqualungResourceDefs = 0x00192A39;
int g_BattleBlueAqualungTimNamePtr[1] = {
    (int)&g_BattleBlueAqualungResourceNames
};
int g_BattleBlueAqualungSptDefRow[4] = {
    (int)&g_BattleBlueAqualungResourceNames[0xc], 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x008abdf8
unsigned char g_BattleBlueAqualungSpriteReq[12] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x2e};
unsigned char g_BattleBlueAqualungResourceNames[24] =
    "AQUA.TIM\0\0\0\0"
    "AQUA.SPT";
// clang-format on

// clang-format off
// GLOBAL: FF7 0x00ba7458
BattleFxAqualungRing g_BattleBlueAqualungParticleCount = {0};
unsigned char g_bssPad_00ba7474[4] = {0};
unsigned char g_bssPad_00ba7478[3528] = {0};
// GLOBAL: FF7 0x00ba8240
int g_BattleBlueAqualungTargetMask = 0;
// GLOBAL: FF7 0x00ba8244
struct GfxTableCursor *g_BattleBlueAqualungLoadedResTable = 0;
// clang-format on

/* Loads Aqualung blue magic resources. */
// FUNCTION: FF7 0x00573720
void __cdecl BattleFx_BlueLoadResAqualung(void)
{
    GfxModelTableBank *newSceneField;
    GfxResourceRequest srcRequestBuf;
    int kind;
    char resPathBuf[200];
    GfxResourceRequest *pRequest;
    int defsStackPointer;

    if (g_pBattle3dScene != 0 && g_BattleBlueAqualungLoadedResTable == 0) {
        kind = 1;
        defsStackPointer = (int)&g_BattleBlueAqualungResourceDefs;
        newSceneField = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &srcRequestBuf;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008abe20
        pRequest->resourcePath = BattleFx_BuildResourcePath("blue/aqua", resPathBuf);
        Gfx_LoadModelTableIfAbsent(kind, defsStackPointer, pRequest, &g_BattleBlueAqualungLoadedResTable, newSceneField);
    }
}

// Entry point for the Aqualung blue magic effect. It loads the effect's resources and then starts
// the effect for the target mask and caster.
// FUNCTION: FF7 0x005737b0
void __cdecl BattleFx_BlueEnterAqualung(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResAqualung();
    BattleFx_BlueBeginAqualung(targetMask, actorIdx);
}

/* Initializes setup task for Aqualung. */
// FUNCTION: FF7 0x005737ca
void __cdecl BattleFx_BlueBeginAqualung(int targetMask, int actorIdx)
{
    Battle_AllocateResourceSlot(BattleFx_BlueAqualungSetupTask);
    g_BattleBlueAqualungTargetMask = targetMask;
    g_BattleBlueAqualungParticleCount.nextIndex = 0;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x9D);
}

// Sets up the Aqualung target centroid and spawns its timeline task. The setup slot is marked
// finished, the new timeline task receives the centroid of the target mask, and its range and base
// values are set. Its step values are derived from the range, and its particleScale field is set to a fixed
// constant.
// FUNCTION: FF7 0x0057380a
void __cdecl BattleFx_BlueAqualungSetupTask(void)
{
    BattleFxBlueAqualungSlot *task;

    task = (BattleFxBlueAqualungSlot *)BATTLE_CURRENT_TASK_SLOT;
    task->state = 0xFFFF;
    task = (BattleFxBlueAqualungSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueAqualungTimelineTask);
    Battle_GetActorMaskCentroid(g_BattleBlueAqualungTargetMask, (short *)((int)task + 4));
    task->range = 0xFA00;
    task->baseValue = 0xA00;
    task->baseTimes2 = task->baseValue << 1;
    task->step = -(task->range >> 1);
    task->stepTimes3 = task->step * 2 + task->step;
    task->step += task->step;
    task->particleScale = 0xBB3;
}

// Drives the Aqualung timeline, water sounds, and bubble emission. It does nothing while the game
// is paused. From frame 2 to frame 0x20, each call emits two bubble particles into a ring of 0xDE
// entries, with positions jittered around the source, a base size from particleScale, and growth and shrink
// rates of one sixth of that size. Frames below 8 set the channel fields for the fade-in, frames
// from 0x28 fade the level out, and frame 0x28 allocates a record for each target bit. From frame
// 0x30 it clears the channel and frees its slot.
// FUNCTION: FF7 0x005738c8
void __cdecl BattleFx_BlueAqualungTimelineTask(void)
{
    int idx;
    short level;
    int ringIndex;
    int baseSize;
    int bytesRand;
    BattleFxAqualungTimeline *task;
    BattleFxAqualungSpriteTask *pNewTask;

    if (g_BattleIsPaused != 0) {
        return;
    }
    task = (BattleFxAqualungTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (task->frame > 1 && task->frame < 0x21) {
        ringIndex = g_BattleBlueAqualungParticleCount.nextIndex;
        pNewTask = (BattleFxAqualungSpriteTask *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueAqualungGrowShrinkSpritesTask);
        pNewTask->firstParticle = ringIndex;
        for (idx = 0; idx < 2; idx++) {
            bytesRand = rand() % 0x1000 + 0x800;
            g_BattleBlueAqualungParticleCount.particles[ringIndex].posX = task->posX + rand() % task->spreadX - task->offsetX;
            g_BattleBlueAqualungParticleCount.particles[ringIndex].posY = task->posY - rand() % task->spreadY + task->offsetY;
            g_BattleBlueAqualungParticleCount.particles[ringIndex].posZ = task->posZ + rand() % task->spreadX - task->offsetX;
            baseSize = task->particleScale * bytesRand >> 0xC;
            g_BattleBlueAqualungParticleCount.particles[ringIndex].baseSize = baseSize;
            g_BattleBlueAqualungParticleCount.particles[ringIndex].size = 0;
            g_BattleBlueAqualungParticleCount.particles[ringIndex].growRate = baseSize / 6;
            g_BattleBlueAqualungParticleCount.particles[ringIndex].shrinkRate = baseSize / 6;
            ringIndex++;
            if (ringIndex >= 0xDE) {
                ringIndex = 0;
            }
        }
        g_BattleBlueAqualungParticleCount.nextIndex = ringIndex;
    }
    if (task->frame < 8) {
        g_BattleFxTimelineState.channel1 = 0;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        g_BattleFxTimelineState.channel2 = 0x30;
        level = task->frame * 0x180;
    } else if (task->frame >= 0x28) {
        level = 0xC00 - (task->frame - 0x28) * 0x180;
        if (level < 0) {
            level = 0;
        }
    } else {
        level = 0xC00;
    }
    if (task->frame == 0x28) {
        for (idx = 0; idx < 0xA; idx++) {
            if ((g_BattleBlueAqualungTargetMask & 1 << idx) != 0) {
                Battle_PostActorHitOrHurtMessage(idx);
            }
        }
    }
    if (task->frame >= 0x30) {
        level = 0;
        g_BattleFxTimelineState.channel2 = level;
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    g_BattleFxTimelineState.startValue = level;
    (task->frame)++;
}

// Animates and submits the expanding water bubble sprites. Each call handles two particles from the
// 0xDE-entry ring, wrapping the index at the end. Before frame 6 a particle's size grows by its
// growth rate, capped at 0x7000. At frame 6 it is reset to its base size, and from frame 7 it
// shrinks by its shrink rate, floored at zero. The slot is freed from frame 0xD, and the frame
// advances only when the game is not paused.
// FUNCTION: FF7 0x00573b8b
void __cdecl BattleFx_BlueAqualungGrowShrinkSpritesTask(void)
{
    int i, slotIdx;
    BattleFxAqualungSpriteTask *pSprite;

    pSprite = (BattleFxAqualungSpriteTask *)BATTLE_CURRENT_TASK_SLOT;
    slotIdx = pSprite->firstParticle;
    ((BattleFxSpriteRequest *)&g_BattleBlueAqualungSpriteReq)->spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleBlueAqualungLoadedResTable);
    ((BattleFxSpriteRequest *)&g_BattleBlueAqualungSpriteReq)->drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleBlueAqualungLoadedResTable);
    for (i = 0; i < 2; i++) {
        BattleFx_InstallBillboardMatrix(&g_BattleBlueAqualungParticleCount.particles[slotIdx].posX, (short)g_BattleBlueAqualungParticleCount.particles[slotIdx].size, -(g_BattleBlueAqualungParticleCount.particles[slotIdx].size >> 2));
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&((BattleFxSpriteRequest *)&g_BattleBlueAqualungSpriteReq)->spriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
        if (g_BattleIsPaused == 0) {
            if (pSprite->frame < 6) {
                g_BattleBlueAqualungParticleCount.particles[slotIdx].size += g_BattleBlueAqualungParticleCount.particles[slotIdx].growRate;
                if (g_BattleBlueAqualungParticleCount.particles[slotIdx].size > 0x7000) {
                    g_BattleBlueAqualungParticleCount.particles[slotIdx].size = 0x7000;
                }
            } else if (pSprite->frame >= 7) {
                g_BattleBlueAqualungParticleCount.particles[slotIdx].size -= g_BattleBlueAqualungParticleCount.particles[slotIdx].shrinkRate;
                if (g_BattleBlueAqualungParticleCount.particles[slotIdx].size < 0) {
                    g_BattleBlueAqualungParticleCount.particles[slotIdx].size = 0;
                }
            } else {
                g_BattleBlueAqualungParticleCount.particles[slotIdx].size = g_BattleBlueAqualungParticleCount.particles[slotIdx].baseSize;
            }
        }
        slotIdx++;
        if (slotIdx >= 0xDE) {
            slotIdx = 0;
        }
    }
    if (pSprite->frame >= 0xD) {
        pSprite->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (pSprite->frame)++;
}
} /* extern "C" */
