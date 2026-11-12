// src/ff7/src/battle/battle_fx_magic_mini.cpp
/* Magic Effect [9]: Mini (Japanese: ミニマム Minimum, dev tag: "MAGIC/MINIMAM" / Minimam) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Mini (Minimam) magic effect: a timeline that generates particles and animates billboard
//   sprites for each target.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_magic.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

#define MINI_SPRITE_REQ (&g_BattleMagicMiniSpriteReq)
#define MINI_PARTICLE_POOL ((BattleMagicMiniParticlePool *)&g_BattleMagicMiniParticleIndex)


// clang-format off
// GLOBAL: FF7 0x008abf00
int g_BattleMagicMiniResourceDefs = 0x00192A39;
char *g_BattleFxMagicMiniParticlesResourceNames[1] = {
    // STRING: FF7 0x008abf28
    "MINI_1.TIM"
};
unsigned int g_BattleFxMagicMiniParticlesAssetPrefix[4] = {
    // STRING: FF7 0x008abf34
    (unsigned int)"MINI_1.SPT",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x008abf18
struct BattleFxSpriteRequest g_BattleMagicMiniSpriteReq = {0, 0, -128, -128, -128, 0x2e, 0, 0};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x00ba9068
unsigned char g_BattleMagicMiniParticleIndex[20] = {0};
unsigned char g_bssPad_00ba907c[4] = {0};
unsigned char g_bssPad_00ba9080[3536] = {0};
// GLOBAL: FF7 0x00ba9e50
struct GfxTableCursor *g_BattleMagicMiniLoadedResTable = 0;
unsigned char g_bssPad_00ba9e54[4] = {0};
unsigned char g_bssPad_00ba9e58[8] = {0};
// clang-format on

/* Loads and registers resources for the Mini (Minimam) magic effect. */
// FUNCTION: FF7 0x00574f90
void __cdecl BattleFx_MagicLoadResMini(void)
{
    GfxModelTableBank *sceneBase;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleMagicMiniLoadedResTable == 0) {
        kind = 1;
        resourceDefsWord = (int)&g_BattleMagicMiniResourceDefs;
        sceneBase = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008abf40
        pRequest->resourcePath = BattleFx_BuildResourcePath("magic/minimam", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleMagicMiniLoadedResTable, sceneBase);
    }
}

/* Entry point for the Mini (Minimam) magic effect. */
// FUNCTION: FF7 0x00575020
void __cdecl BattleFx_MagicEnterMini(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResMini();
    BattleFx_MagicBeginMini(targetMask, actorIdx);
}

/* Initializes Mini (Minimam) effect tasks and dispatches audio. */
// FUNCTION: FF7 0x0057503a
void __cdecl BattleFx_MagicBeginMini(int targetMask, int actorIdx)
{
    g_BattleFxDrawListBufferTaskSlotPtr = (unsigned char *)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 1, BattleFx_MagicSpawnMiniPerTarget);
    MINI_PARTICLE_POOL->cursor = 0;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x134);
}

/* Spawns per-target Mini (Minimam) effect timeline tasks. */
// FUNCTION: FF7 0x00575099
void __cdecl BattleFx_MagicSpawnMiniPerTarget(int actor)
{
    BattleFxMiniTarget *minimamTarget;
    int depth;

    minimamTarget = (BattleFxMiniTarget *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicMiniTimelineTask);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)((int)minimamTarget + 4));
    depth = g_BattleActorRecords[actor].radius + 0xE0;
    if (depth > 0x4F2) {
        depth = 0x4F2;
    }
    minimamTarget->depth = depth;
    minimamTarget->depthX2 = minimamTarget->depth << 1;
    minimamTarget->offset = -(minimamTarget->extent >> 1);
    minimamTarget->offsetX4 = minimamTarget->offset << 2;
    minimamTarget->scale = (depth << 0xC) / 0x21C;
    minimamTarget->targetIndex = actor;
}

/* Timeline task controlling particle generation and animation for Mini (Minimam). */
// FUNCTION: FF7 0x00575168
void __cdecl BattleFx_MagicMiniTimelineTask(void)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int spawnIndex;
    int poolIndex;
    int scaleJitter;
    BattleFxMiniRecord *task;
    struct BattleTaskSlot *pNewTask;

    if (g_BattleIsPaused != 0) {
        return;
    }
    task = (BattleFxMiniRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (task->frame > 1 && task->frame < 0x1C) {
        poolIndex = MINI_PARTICLE_POOL->cursor;
        pNewTask = BATTLE_NEW_TASK_SLOT(BattleFx_MagicMiniAnimatedSpritesTask);
        ((BattleFxBerserkFadeSprite *)pNewTask)->ringIndex = poolIndex;
        for (spawnIndex = 0; spawnIndex < 3; spawnIndex++) {
            scaleJitter = rand() % 0x1000 + 0x800;
            MINI_PARTICLE_POOL->particles[poolIndex].posX = task->baseX + rand() % task->jitterRange - task->jitterOffset;
            MINI_PARTICLE_POOL->particles[poolIndex].posY = task->baseY - rand() % task->jitterRangeY + task->jitterOffsetY;
            MINI_PARTICLE_POOL->particles[poolIndex].posZ = task->baseZ + rand() % task->jitterRange - task->jitterOffset;
            MINI_PARTICLE_POOL->particles[poolIndex].scale = task->scale * scaleJitter >> 0xC;
            poolIndex++;
            if (poolIndex >= 0x1BC) {
                poolIndex = 0;
            }
        }
        MINI_PARTICLE_POOL->cursor = poolIndex;
    }
    if (task->frame == 0x1C) {
        Battle_PostActorHitOrHurtMessage(task->recordType);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition((char *)task + 4), 0xB1);
    }
    if (task->frame >= 0x23) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frame)++;
}

/* Animates and submits billboard sprites for Mini (Minimam). */
// FUNCTION: FF7 0x00575341
void __cdecl BattleFx_MagicMiniAnimatedSpritesTask(void)
{
    int spriteIndex, readIdx;
    BattleFxBerserkFadeSprite *task;

    task = (BattleFxBerserkFadeSprite *)BATTLE_CURRENT_TASK_SLOT;
    MINI_SPRITE_REQ->frame = task->frame;
    readIdx = task->ringIndex;
    MINI_SPRITE_REQ->spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleMagicMiniLoadedResTable);
    MINI_SPRITE_REQ->drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleMagicMiniLoadedResTable);
    for (spriteIndex = 0; spriteIndex < 3; spriteIndex++) {
        BattleFx_InstallBillboardMatrix(&MINI_PARTICLE_POOL->particles[readIdx].posX, MINI_PARTICLE_POOL->particles[readIdx].scale, -(MINI_PARTICLE_POOL->particles[readIdx].scale >> 2));
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&MINI_SPRITE_REQ->spriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
        readIdx++;
        if (readIdx >= 0x1BC) {
            readIdx = 0;
        }
    }
    if (task->frame >= 7) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (task->frame)++;
}
} /* extern "C" */
