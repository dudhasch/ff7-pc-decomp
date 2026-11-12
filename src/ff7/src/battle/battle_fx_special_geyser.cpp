// src/ff7/src/battle/battle_fx_special_geyser.cpp
/* Enemy Special Attack [81]: Geyser / Underground Water (orig. SPECIAL/CHIKASUI, 地下水) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Chikasui attack: it loads resources and spawns a
//   per-target timeline that drives a particle sprite task, with particles taken from a pool.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
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
// GLOBAL: FF7 0x008a3fa8
int g_BattleSpecGeyserResourceDefs = 0x00192A39;
int g_BattleSpecGeyserResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialGeyserResourceNames + 0x0)
};
// GLOBAL: FF7 0x008a3fb0
int g_BattleSpecGeyserResourceModelRows[6] = {
    (int)((char *)&g_BattleFxSpecialGeyserResourceNames + 0xc), 0x0, 0x0, 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x008a3fc8
struct BattleSpecHeatBlastColor g_BattleSpecGeyserSpriteReq = {-128, -128, -128, {0x2e}};
unsigned char g_BattleFxSpecialGeyserResourceNames[24] =
    "MIZU.TIM\0\0\0\0"
    "MIZU.SPT";
// GLOBAL: FF7 0x00b68ff0
int g_BattleSpecGeyserBonePos = 0;
// GLOBAL: FF7 0x00b68ff4
short g_BattleSpecGeyserBonePosZ = 0;
// The pool of BattleSpecGeyserParticle records starts here; the code reaches all 300 through
// this object. The span runs to the next named definition of the TU.
// GLOBAL: FF7 0x00b68ff8
unsigned char g_BattleSpecGeyserParticlePool[8] = {0};
// Address of the first particle's pos member; the sprite task indexes from here.
// GLOBAL: FF7 0x00b69000
int g_BattleSpecGeyserParticlePos = 0;
unsigned char g_bssPad_00b69004[4] = {0};
unsigned char g_bssPad_00b69008[7184] = {0};
// GLOBAL: FF7 0x00b6ac18
int g_BattleSpecGeyserSpriteDrawHandle = 0;
// GLOBAL: FF7 0x00b6ac1c
struct GfxTableCursor *g_BattleSpecGeyserModelTable = 0;
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

#define GEYSER_PARTICLE_COUNT 0x12C


/*
 * Loads the SPECIAL/CHIKASUI model table into g_BattleSpecGeyserModelTable, once the battle scene
 * exists and the table is not loaded yet.
 */
// FUNCTION: FF7 0x0053fe90
void __cdecl BattleFx_SpecialLoadResGeyser(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest currentRecord;
    int kind;
    char resourceName[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecGeyserModelTable == 0) {
        kind = 1;
        resourceDefsWord = (int)&g_BattleSpecGeyserResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a3fe8
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/CHIKASUI", resourceName);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecGeyserModelTable, curOwnerCtx);
    }
}

/*
 * Entry point of the Chikasui effect: loads the resources, then registers the per-target spawns.
 */
// FUNCTION: FF7 0x0053ff20
void __cdecl BattleFx_SpecialEnterGeyser(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResGeyser();
    BattleFx_SpecialRegisterGeyserPerTargetCallback(targetMask, actorIdx);
}

/*
 * Calls BattleFx_SpecialSpawnGeyserPerTarget for every target in the mask, plays the sound
 * effect panned to the source actor, and frees all 300 particles.
 */
// FUNCTION: FF7 0x0053ff3a
void __cdecl BattleFx_SpecialRegisterGeyserPerTargetCallback(int targetIndex, int sourceIndex)
{
    short i;

    Battle_CreateBitPatternCallbackTask(targetIndex, sourceIndex, 4, BattleFx_SpecialSpawnGeyserPerTarget);
    Battle_PlaySfxPannedToActor(0x10B, (short)sourceIndex, 0x40);
    for (i = 0; i < GEYSER_PARTICLE_COUNT; i++) {
        ((BattleSpecGeyserParticle *)g_BattleSpecGeyserParticlePool)[i].state = 0;
    }
}

/*
 * Starts a timeline task for one target. Stores the source actor's bone34 position in
 * g_BattleSpecGeyserBonePos / g_BattleSpecGeyserBonePosZ (where the particles spawn) and the
 * target's targetBone position and index in the task.
 */
// FUNCTION: FF7 0x0053ff9f
void __cdecl BattleFx_SpecialSpawnGeyserPerTarget(int actor, int sourceIndex)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxGeyserTimeline *taskPtr;

    taskPtr = (BattleFxGeyserTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialGeyserTimelineTask);
    Battle_GetBoneWorldPos(sourceIndex, g_BattleActorRecords[sourceIndex].bone34, (short *)&g_BattleSpecGeyserBonePos);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, taskPtr->targetBonePos);
    taskPtr->actorIndex = actor;
}

/*
 * Timeline of one target: starts the particle task at age 0, allocates the target's typed record
 * at age 60 and ends at age 70. Does nothing while the battle is paused.
 */
// FUNCTION: FF7 0x00540015
void __cdecl BattleFx_SpecialGeyserTimelineTask(void)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxGeyserTimeline *taskPtr;
    int s; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int t; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int child;

    taskPtr = (BattleFxGeyserTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->age == 0) {
        child = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialGeyserParticleSpritesTask);
    }
    if (taskPtr->age == 60) {
        Battle_PostActorHitOrHurtMessage(taskPtr->actorIndex);
    }
    (taskPtr->age)++;
    if (taskPtr->age == 70) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the live water particles, spawns new ones for the first 60 frames, moves them under
 * gravity and fades them after the top of their arc. Ends when the spawning is over and none is left.
 */
// FUNCTION: FF7 0x005400a8
void __cdecl BattleFx_SpecialGeyserParticleSpritesTask(void)
{
    short angle, rad;
    BattleTaskSlot *task;
    short oldVy, index;
    short liveCount;
    BattleSpecGeyserParticle *emitter;

    task = BATTLE_CURRENT_TASK_SLOT;
    ((BattleFxSpriteRequest *)&g_BattleSpecGeyserResourceModelRows[4])->spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecGeyserModelTable);
    ((BattleFxSpriteRequest *)&g_BattleSpecGeyserResourceModelRows[4])->drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecGeyserModelTable);
    for (index = 0; index < GEYSER_PARTICLE_COUNT; index++) {
        if (((BattleSpecGeyserParticle *)g_BattleSpecGeyserParticlePool)[index].state == 1) {
            g_BattleSpecGeyserSpriteReq.blue = 0x80 - (((BattleSpecGeyserParticle *)g_BattleSpecGeyserParticlePool)[index].fade << 3);
            g_BattleSpecGeyserSpriteReq.green = g_BattleSpecGeyserSpriteReq.blue;
            (g_BattleSpecGeyserSpriteReq.red) = g_BattleSpecGeyserSpriteReq.green;
            BattleFx_InstallBillboardMatrix((short *)(index * sizeof(BattleSpecGeyserParticle) + (char *)&g_BattleSpecGeyserParticlePos), ((BattleSpecGeyserParticle *)g_BattleSpecGeyserParticlePool)[index].scale, 0);
            g_BattleSpecGeyserSpriteDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecGeyserResourceModelRows[4], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecGeyserSpriteDrawHandle);
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter < 60) {
        for (index = 0; index < (0x3C - task->frameCounter) / 5 + 1; index++) {
            emitter = (BattleSpecGeyserParticle *)BattleFx_SpecialGeyserClaimParticleRecord();
            if ((unsigned int)(int)emitter > (unsigned int)0) {
                angle = (rand() % 4096);
                rad = rand() % 200;
                emitter->pos.vx = ((BattleSpecGeyserBonePos *)&g_BattleSpecGeyserBonePos)->x + (CosFixed(angle) * rad >> 12);
                emitter->pos.vz = g_BattleSpecGeyserBonePosZ + (SinFixed(angle) * rad >> 12);
                emitter->pos.vy = ((BattleSpecGeyserBonePos *)&g_BattleSpecGeyserBonePos)->y;
                angle = (rand() % 4096);
                rad = rand() % 100;
                emitter->vel.vx = CosFixed(angle) * rad >> 12;
                emitter->vel.vz = SinFixed(angle) * rad >> 12;
                emitter->vel.vy = (SinFixed(task->frameCounter << 10) * 0x14 >> 12) + task->frameCounter * 2 + -350;
                emitter->scale = ((rand() % 4) << 9) + 0x800;
            }
        }
    }
    index = 0;
    liveCount = 0;
    for (emitter = (BattleSpecGeyserParticle *)&g_BattleSpecGeyserParticlePool; index < GEYSER_PARTICLE_COUNT; index++) {
        if (emitter->state == 1) {
            emitter->pos.vx += emitter->vel.vx;
            emitter->pos.vy += emitter->vel.vy;
            emitter->pos.vz += emitter->vel.vz;
            oldVy = emitter->vel.vy;
            emitter->vel.vy += 20;
            if (emitter->fade == 0) {
                if (oldVy < 0 && emitter->vel.vy >= 0) {
                    emitter->fade = 1;
                }
            } else {
                (emitter->fade)++;
                if (emitter->fade == 16) {
                    emitter->state = 0;
                }
            }
            liveCount++;
        }
        emitter++;
    }
    (task->frameCounter)++;
    if (task->frameCounter > 60 && liveCount == 0) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Claims the first free particle (state 0) of the pool: marks it live, clears its fade and returns
 * its address; returns zero when the pool is full.
 */
// FUNCTION: FF7 0x0054048d
int __cdecl BattleFx_SpecialGeyserClaimParticleRecord(void)
{
    short i;

    for (i = 0; i < GEYSER_PARTICLE_COUNT; i++) {
        if (((BattleSpecGeyserParticle *)g_BattleSpecGeyserParticlePool)[i].state == 0) {
            ((BattleSpecGeyserParticle *)g_BattleSpecGeyserParticlePool)[i].state = 1;
            ((BattleSpecGeyserParticle *)g_BattleSpecGeyserParticlePool)[i].fade = 0;
            return (int)&((BattleSpecGeyserParticle *)g_BattleSpecGeyserParticlePool)[i];
        }
    }
    return 0;
}
} /* extern "C" */
