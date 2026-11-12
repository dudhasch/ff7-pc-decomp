// src/ff7/src/battle/battle_fx_magic_esuna.cpp
/* Magic Effect [5]: Esuna (Japanese: エスナ, dev tag: "MAGIC/ESNA" / Esna) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle visual effect for the Esna magic: a per-target particle emitter throws radial
//   sprite particles and plays a sound effect.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_magic.h"
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
// GLOBAL: FF7 0x008c9818
int g_BattleMagicEsuna_ResourceDef = 0x00192A39;
char *g_BattleFxMagicEsunaResourceNames[1] = {
    // STRING: FF7 0x008c9850
    "ESNA.TIM"
};
unsigned int g_BattleFxMagicEsunaAssetPrefix[4] = {
    // STRING: FF7 0x008c985c
    (unsigned int)"LINE.SPT",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x008c9830
int g_BattleMagicEsuna_DrawArg1 = (int)0xFFD0FFD0;
unsigned char g_rawData_008c9834[4] = {0x00, 0x00, 0x5f, 0x5f};
// GLOBAL: FF7 0x008c9838
struct BattleMagicEsuna_SpriteDrawReq g_BattleMagicEsuna_SpriteDrawReq = {-128, -128, -128, {0x2e, 0xac, 0, 0xf0, 0x3f}, 0};
unsigned char g_rawData_008c9848[8] = {0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00bd5ac0
int g_BattleMagicEsuna_ParticleCount = 0;
// GLOBAL: FF7 0x00bd5ac4
int g_BattleMagicEsuna_DrawHandle = 0;
// GLOBAL: FF7 0x00bd5ac8
int g_BattleMagicEsuna_EffectState = 0;
// GLOBAL: FF7 0x00bd5acc
struct GfxTableCursor *g_pBattleMagicEsunaTableCursor = 0;
// GLOBAL: FF7 0x00bd5ad0
struct Int64Pair g_BattleMagicEsuna_AnglePair = {0};
// clang-format on

/*
 * Loads resources for the Esna magic effect when not already loaded.
 */
// FUNCTION: FF7 0x005a1850
void __cdecl BattleFx_MagicLoadResEsuna(void)
{
    char pathBuf[200];
    GfxResourceRequest requestStorage;
    int kind;
    GfxModelTableBank *gfxCtx;
    GfxResourceRequest *requestPtr;
    int def;

    if (g_pBattle3dScene != 0 && g_pBattleMagicEsunaTableCursor == 0) {
        kind = 1;
        def = (int)&g_BattleMagicEsuna_ResourceDef;
        gfxCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        requestPtr = &requestStorage;
        Gfx_InitResourceRequest(requestPtr);
        requestPtr->groupTag = 0;
        // STRING: FF7 0x008c9868
        requestPtr->resourcePath = (char *)BattleFx_BuildResourcePath("MAGIC/ESNA", pathBuf);
        Gfx_LoadModelTableIfAbsent(kind, def, requestPtr, &g_pBattleMagicEsunaTableCursor, gfxCtx);
    }
}

/*
 * Entry point for Esna magic effect initializing execution.
 */
// FUNCTION: FF7 0x005a18e0
void __cdecl BattleFx_MagicEnterEsuna(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResEsuna();
    BattleFx_MagicBeginEsuna(targetMask, actorIdx);
}

/*
 * Iterates targets and begins Esna effect execution.
 */
// FUNCTION: FF7 0x005a18fa
void __cdecl BattleFx_MagicBeginEsuna(int targetMask, int actorIdx)
{
    g_BattleMagicEsuna_EffectState = Battle_PopCountActorMask(targetMask);
    g_BattleMagicEsuna_ParticleCount = (g_BattleMagicEsuna_EffectState - 1 != 0 ? 0xFFFFFFFD : 0) + 6;
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, g_BattleMagicEsuna_EffectState <= 3 ? 0 : 0xA, BattleFx_MagicSpawnEsunaPerTarget);
    if (g_BattleMagicEsuna_EffectState <= 3) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x8B);
    }
}

/*
 * Spawns Esna particle emitter task and plays sound effect for target.
 */
// FUNCTION: FF7 0x005a1977
void __cdecl BattleFx_MagicSpawnEsunaPerTarget(int actor)
{
    BattleFxResistTask *task;

    task = (BattleFxResistTask *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicEsunaParticleEmitterTask);
    task->targetIndex = actor;
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)((int)task + 8));
    task->posY = 0;
    if (g_BattleMagicEsuna_EffectState > 3) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition((char *)task + 8), 0x8B);
    }
}

/*
 * Timeline task spawning radial particle sprites for Esna.
 */
// FUNCTION: FF7 0x005a19fa
void __cdecl BattleFx_MagicEsunaParticleEmitterTask(void)
{
    BattleFxSwirlColumnTask *taskPtr;
    int i;
    int buffer;
    BattleWhiteWindFadingSpriteSlot *pSprite;

    taskPtr = (BattleFxSwirlColumnTask *)BATTLE_CURRENT_TASK_SLOT;
    buffer = Gfx_GetPsxScratchpadPtr(0);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->step < 0xA) {
        *(short *)((char *)&g_BattleMagicEsuna_AnglePair + 2) = rand() & 0xFFF;
        for (i = 0; i < g_BattleMagicEsuna_ParticleCount; i++) {
            pSprite = (BattleWhiteWindFadingSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicEsunaParticleSpriteTask);
            pSprite->origin = taskPtr->origin;
            pSprite->scale = rand() % 0x800 + 0x800;
            pSprite->angle = g_BattleMagicEsuna_AnglePair;
            pSprite->radialSpeed = rand() % 0x190 + 0x64 << 4;
            pSprite->spinStep = rand() % 0x64 - 0x32;
            *(short *)((char *)&g_BattleMagicEsuna_AnglePair + 2) += (short)(0x1000 / g_BattleMagicEsuna_ParticleCount);
        }
    } else {
        Battle_PostActorHitOrHurtMessage(taskPtr->recordType);
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
    (taskPtr->step)++;
}

/*
 * Renders and updates a single particle sprite for Esna.
 */
// FUNCTION: FF7 0x005a1b55
void __cdecl BattleFx_MagicEsunaParticleSpriteTask(void)
{
    int localFrame;
    BattleFxEsunaParticleSprite *task;
    int matrix;
    BattleFxWhiteWindFadeState *velBlockPtr;

    task = (BattleFxEsunaParticleSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicEsuna_SpriteDrawReq.tableField = Gfx_ModelTableGetSpriteObject(0, g_pBattleMagicEsunaTableCursor);
    velBlockPtr = (BattleFxWhiteWindFadeState *)Gfx_GetPsxScratchpadPtr(0);
    matrix = (int)velBlockPtr + 8;
    localFrame = task->timer;
    if (localFrame < 0xD) {
        g_BattleMagicEsuna_SpriteDrawReq.colorB = 0x80;
        g_BattleMagicEsuna_SpriteDrawReq.colorG = g_BattleMagicEsuna_SpriteDrawReq.colorB;
        (g_BattleMagicEsuna_SpriteDrawReq.colorR) = g_BattleMagicEsuna_SpriteDrawReq.colorG;
    } else {
        localFrame -= 0xD;
        if (localFrame < 4) {
            g_BattleMagicEsuna_SpriteDrawReq.colorB = (localFrame * -0x80 / 4) + 0x80;
            g_BattleMagicEsuna_SpriteDrawReq.colorG = g_BattleMagicEsuna_SpriteDrawReq.colorB;
            (g_BattleMagicEsuna_SpriteDrawReq.colorR) = g_BattleMagicEsuna_SpriteDrawReq.colorG;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    BattleFx_InstallBillboardMatrix((void *)((int)task + 8), (short)task->scale, 0);
    g_BattleMagicEsuna_DrawHandle = BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)&g_BattleMagicEsuna_DrawArg1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicEsuna_DrawHandle);
    if (g_BattleIsPaused == 0) {
        (task->timer)++;
        velBlockPtr->velocityX = task->velocityX;
        velBlockPtr->velocityY = 0;
        velBlockPtr->velocityZ = 0;
        RotMatrix((SVECTOR *)((int)task + 0x10), (void *)matrix);
        TransformVectorToInt16((void *)matrix, (void *)velBlockPtr, (void *)velBlockPtr);
        task->posX += (short)(velBlockPtr->velocityX >> 4);
        task->posY += (short)(velBlockPtr->velocityY >> 4);
        task->posZ += (short)(velBlockPtr->velocityZ >> 4);
        task->velocityX -= (short)(task->velocityX >> 3);
        task->heightOffset += (short)(-0x400 - task->heightOffset >> 2);
        task->velocity += task->accel;
    }
}
} /* extern "C" */
