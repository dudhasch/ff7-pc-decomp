// src/ff7/src/battle/battle_fx_magic_bio.cpp
/* Magic Effect [39]: Bio (Japanese: バイオ, dev tag: "MAGIC/BIO") */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle visual effect for the Bio magic: a per-target timeline spawns rising fading
//   sprites and six-frame sprites, with sound effects.

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
// GLOBAL: FF7 0x008f1de8
int g_BattleMagicBio_ResourceDef = 0x00192A39;
char *g_BattleFxMagicBioResourceNames[1] = {
    // STRING: FF7 0x008f1e30
    "BIO.TIM"
};
unsigned int g_BattleFxMagicBioResourceNames2[8] = {
    // STRING: FF7 0x008f1e38
    (unsigned int)"BIO_A.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008f1e44
    (unsigned int)"BIO.TIM",
    // STRING: FF7 0x008f1e4c
    (unsigned int)"BUBBLE_A.SPT",
    0x00000000,
    0x00000000
};
#undef g_BattleMagicBio_SpriteDrawReq
// GLOBAL: FF7 0x008f1e10
struct BattleMagicBio_SpriteDrawReq g_BattleMagicBio_SpriteDrawReq = {0, 0, 0x2c808080, 0, {0}, 0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
#define BIO_DRAW_REQ (&g_BattleMagicBio_SpriteDrawReq)
// clang-format on


// clang-format off
// GLOBAL: FF7 0x00bd5b40
int g_BattleMagicBio_DrawHandle = 0;
// GLOBAL: FF7 0x00bd5b44
struct GfxTableCursor *g_BattleMagicBio_ModelTable = 0;
// clang-format on

/*
 * Loads resources for the Bio magic effect when not already loaded.
 */
// FUNCTION: FF7 0x005a66a0
void __cdecl BattleFx_MagicLoadResBio(void)
{
    GfxModelTableBank *parentHandle;
    GfxResourceRequest requestRecord;
    int kind;
    char pathBuffer[200];
    GfxResourceRequest *pReq;
    int requestTable;

    if (g_pBattle3dScene != 0 && g_BattleMagicBio_ModelTable == 0) {
        kind = 2;
        requestTable = (int)&g_BattleMagicBio_ResourceDef;
        parentHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestRecord;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x008f1e5c
        pReq->resourcePath = BattleFx_BuildResourcePath("MAGIC/BIO", pathBuffer);
        Gfx_LoadModelTableIfAbsent(kind, requestTable, pReq, &g_BattleMagicBio_ModelTable, parentHandle);
    }
}

/*
 * Entry point for Bio magic effect initializing execution.
 */
// FUNCTION: FF7 0x005a6730
void __cdecl BattleFx_MagicEnterBio(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResBio();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 0, BattleFx_MagicSpawnBioPerTarget);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x3F);
}

/*
 * Spawns Bio timeline task for a single target.
 */
// FUNCTION: FF7 0x005a676a
void __cdecl BattleFx_MagicSpawnBioPerTarget(int actor, int param)
{
    BattleFxOrbitCloudTask *effect;

    effect = (BattleFxOrbitCloudTask *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBioTimelineTask);
    effect->targetIndex = actor;
    effect->param = param;
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, effect->bonePos);
}

/*
 * Timeline task managing sprite spawning and sound effects for Bio.
 */
// FUNCTION: FF7 0x005a67c7
void __cdecl BattleFx_MagicBioTimelineTask(void)
{
    BattleFxBioTimelineState *taskPtr;
    BattleFxBioParticle *particle;

    taskPtr = (BattleFxBioTimelineState *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->step >= 0 && taskPtr->step < 15 && taskPtr->step % 4 == 0) {
        particle = (BattleFxBioParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBioRisingFadeSpriteTask);
        particle->posX = taskPtr->spawnX + rand() % 1000 + -500;
        particle->posY = 0;
        particle->posZ = taskPtr->spawnZ + rand() % 1000 + -500;
        particle->scale = rand() % 0x500 + 0x1000;
        particle->timer = rand() % 90 + 60;
    }
    if (taskPtr->step >= 0 && taskPtr->step < 15 && taskPtr->step % 2 == 0) {
        particle = (BattleFxBioParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBioSixFrameSpriteTask);
        particle->posX = taskPtr->spawnX + rand() % 800 + -400;
        particle->posY = taskPtr->spawnY + rand() % 800 + -400;
        particle->posZ = taskPtr->spawnZ + rand() % 800 + -400;
        particle->scale = rand() % 0x900 + 0x700;
    }
    if (taskPtr->step == 15) {
        Battle_PostActorHitOrHurtMessage(taskPtr->recordType);
    }
    if (taskPtr->step == 15) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
    (taskPtr->step)++;
}

/*
 * Renders and animates a rising fading sprite for Bio.
 */
// FUNCTION: FF7 0x005a69e1
void __cdecl BattleFx_MagicBioRisingFadeSpriteTask(void)
{
    int localFrame;
    BattleFxBioRisingFadeSprite *task;

    task = (BattleFxBioRisingFadeSprite *)BATTLE_CURRENT_TASK_SLOT;
    BIO_DRAW_REQ->tableEntry = Gfx_ModelTableGetLoadedRecord(0, g_BattleMagicBio_ModelTable);
    BIO_DRAW_REQ->drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleMagicBio_ModelTable);
    localFrame = task->frame;
    if (localFrame < 10) {
        BIO_DRAW_REQ->color = 0x2C808080;
    } else {
        localFrame -= 10;
        if (localFrame < 10) {
            ((char *)&BIO_DRAW_REQ->color)[3] = 0x2E;
            ((char *)&BIO_DRAW_REQ->color)[2] = localFrame * -0x80 / 0xA + 0x80;
            ((char *)&BIO_DRAW_REQ->color)[1] = ((char *)&BIO_DRAW_REQ->color)[2];
            *(char *)&BIO_DRAW_REQ->color = ((char *)&BIO_DRAW_REQ->color)[1];
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    BIO_DRAW_REQ->frame = task->frame % 7;
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->size, 0);
    g_BattleMagicBio_DrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleMagicBio_SpriteDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicBio_DrawHandle);
    if (g_BattleIsPaused == 0) {
        (task->frame)++;
        task->posY -= task->riseSpeed;
    }
}

/*
 * Renders and steps an animated six-frame sprite for Bio.
 */
// FUNCTION: FF7 0x005a6b1e
void __cdecl BattleFx_MagicBioSixFrameSpriteTask(void)
{
    BattleFxStardustRayBillboardSprite *task;

    task = (BattleFxStardustRayBillboardSprite *)BATTLE_CURRENT_TASK_SLOT;
    BIO_DRAW_REQ->tableB = Gfx_ModelTableGetLoadedRecord(1, g_BattleMagicBio_ModelTable);
    BIO_DRAW_REQ->tableC = Gfx_ModelTableGetSpriteObject(1, g_BattleMagicBio_ModelTable);
    BIO_DRAW_REQ->word = task->frame;
    BattleFx_InstallBillboardMatrix((void *)((int)task + 8), (short)task->scale, 0);
    g_BattleMagicBio_DrawHandle = BattleFx_SubmitSpriteDraw(&BIO_DRAW_REQ->tableB, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicBio_DrawHandle);
    if (g_BattleIsPaused == 0) {
        (task->frame)++;
        if (task->frame >= 6) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
