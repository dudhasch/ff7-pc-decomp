// src/ff7/src/battle/battle_fx_special_hot_springs.cpp
/* Enemy Special Attack [82]: Hot Springs (orig. SPECIAL/ONSEN, 温泉) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Onsen attack: it loads resources and spawns a per-target
//   timeline that drives a particle sprite task, with particles taken from a shared record pool.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x008a3ba0
int g_BattleSpecHotSpringsResourceDefs = 76345;
int g_BattleSpecHotSpringsResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialHotSpringsResourceNames + 0x0)
};
// GLOBAL: FF7 0x008a3ba8
int g_BattleSpecHotSpringsResourceModelRows[6] = {
    (int)((char *)&g_BattleFxSpecialHotSpringsResourceNames + 0xc), 0x0, 0x0, 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x008a3bc0
struct BattleSpecHeatBlastColor g_BattleSpecHotSpringsSpriteReq = {-128, -128, -128, {0x2e}};
unsigned char g_BattleFxSpecialHotSpringsResourceNames[24] =
    "ONSEN.TIM\0\0\0"
    "ONSEN.SPT";
// The source bone's world position (Battle_GetBoneWorldPos fills x and y here and z in the next
// global).
// GLOBAL: FF7 0x00b66d38
BattleFxHotSpringsBonePos g_BattleSpecHotSpringsSourceBonePosXY = {0};
// GLOBAL: FF7 0x00b66d3c
short g_BattleSpecHotSpringsSourceBonePosZ = 0;
// The pool of BattleFxHotSpringsParticle records starts here; the code reaches all 300 through this
// object. The span runs to the next named definition of the TU.
// GLOBAL: FF7 0x00b66d40
unsigned char g_BattleSpecHotSpringsParticlePool[8] = {0};
// Lies inside the pool: the position of the first particle record.
// GLOBAL: FF7 0x00b66d48
int g_BattleSpecHotSpringsFirstParticlePos = 0;
unsigned char g_bssPad_00b66d4c[4] = {0};
unsigned char g_bssPad_00b66d50[7184] = {0};
// GLOBAL: FF7 0x00b68960
int g_BattleSpecHotSpringsDrawHandle = 0;
// GLOBAL: FF7 0x00b68964
struct GfxTableCursor *g_BattleSpecHotSpringsModelTable = 0;
// The screen flash brightness: its low byte is the flash colour, its low short is ramped by the timeline.
// GLOBAL: FF7 0x00b68968
unsigned char g_BattleSpecHotSpringsFlashLevel[4] = {0};
unsigned char g_bssPad_00b6896c[4] = {0};
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

#define HOT_SPRINGS_PARTICLE_COUNT 0x12C
#define HOT_SPRINGS_PARTICLES ((BattleFxHotSpringsParticle *)&g_BattleSpecHotSpringsParticlePool)


/*
 * Loads the SPECIAL/ONSEN model table into g_BattleSpecHotSpringsModelTable unless it is already loaded
 * or the battle scene does not exist yet.
 */
// FUNCTION: FF7 0x0053d3d0
void __cdecl BattleFx_SpecialLoadResHotSprings(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest currentRecord;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecHotSpringsModelTable == 0) {
        kind = 1;
        resourceDefsWord = (int)&g_BattleSpecHotSpringsResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a3be0
        pRequest->resourcePath = (char *)BattleFx_BuildResourcePath("SPECIAL/ONSEN", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecHotSpringsModelTable, curOwnerCtx);
    }
}

/*
 * Entry point of the Onsen special effect: forwards both indices to BattleFx_SpecialBeginHotSprings.
 */
// FUNCTION: FF7 0x0053d460
void __cdecl BattleFx_SpecialEnterHotSprings(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginHotSprings(targetMask, actorIdx);
}

/*
 * Loads the model table, schedules BattleFx_SpecialSpawnHotSpringsPerTarget for each actor in
 * targetIndex, plays the sound panned to the source actor and frees all 300 particle records.
 */
// FUNCTION: FF7 0x0053d475
void __cdecl BattleFx_SpecialBeginHotSprings(int targetMask, int actorIdx)
{
    short i;

    BattleFx_SpecialLoadResHotSprings();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnHotSpringsPerTarget);
    Battle_PlaySfxPannedToActor(0x10B, (short)actorIdx, 0x40);
    for (i = 0; i < HOT_SPRINGS_PARTICLE_COUNT; i++) {
        HOT_SPRINGS_PARTICLES[i].state = 0;
    }
}

/*
 * Starts a timeline task for one target actor: stores the source actor's bone position in
 * g_BattleSpecHotSpringsSourceBonePosXY/Z, the target's bone position and the target index in the task.
 */
// FUNCTION: FF7 0x0053d4df
void __cdecl BattleFx_SpecialSpawnHotSpringsPerTarget(int actor, int sourceIndex)
{
    BattleFxHotSpringsTimelineSlot *taskPtr;

    taskPtr = (BattleFxHotSpringsTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHotSpringsTimelineTask);
    Battle_GetBoneWorldPos(sourceIndex, g_BattleActorRecords[sourceIndex].bone34, (short *)&g_BattleSpecHotSpringsSourceBonePosXY);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, taskPtr->targetBonePos);
    taskPtr->actorIndex = actor;
}

/*
 * Timeline of one target. Every frame it submits a white screen flash whose colour is the low byte
 * of g_BattleSpecHotSpringsFlashLevel, then returns if the battle is paused. Frame 0 starts
 * BattleFx_SpecialHotSpringsParticleSpritesTask. The flash level rises by 4 per step for the first 16
 * steps and falls by 4 per step after step 0x36. At step 0x3C it allocates the typed record for
 * the target, and at step 0x46 it frees itself.
 */
// FUNCTION: FF7 0x0053d553
void __cdecl BattleFx_SpecialHotSpringsTimelineTask(void)
{
    BattleFxHotSpringsTimelineSlot *mainSelf;
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int spawned;

    mainSelf = (BattleFxHotSpringsTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(*(unsigned char *)&g_BattleSpecHotSpringsFlashLevel, *(unsigned char *)&g_BattleSpecHotSpringsFlashLevel, *(unsigned char *)&g_BattleSpecHotSpringsFlashLevel);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (mainSelf->timelineStep == 0) {
        spawned = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHotSpringsParticleSpritesTask);
    }
    if (mainSelf->timelineStep < 16) {
        *(short *)&g_BattleSpecHotSpringsFlashLevel += 4;
    }
    if (mainSelf->timelineStep > 54) {
        *(short *)&g_BattleSpecHotSpringsFlashLevel -= 4;
    }
    if (mainSelf->timelineStep == 60) {
        Battle_PostActorHitOrHurtMessage(mainSelf->actorIndex);
    }
    (mainSelf->timelineStep)++;
    if (mainSelf->timelineStep == 70) {
        mainSelf->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the live particles, emits new ones for the first 60 frames from the source bone position,
 * and advances the physics of the live ones (gravity, then a short fade once they start falling).
 * It frees itself when the emission is over and no particle is live.
 */
// FUNCTION: FF7 0x0053d646
void __cdecl BattleFx_SpecialHotSpringsParticleSpritesTask(void)
{
    short angle, rad;
    BattleTaskSlot *task;
    short oldVy, index;
    short liveCount;
    BattleFxHotSpringsParticle *emitter;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecHotSpringsResourceModelRows[4] = Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecHotSpringsModelTable);
    g_BattleSpecHotSpringsResourceModelRows[5] = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecHotSpringsModelTable);
    for (index = 0; index < HOT_SPRINGS_PARTICLE_COUNT; index++) {
        if (HOT_SPRINGS_PARTICLES[index].state == 1) {
            g_BattleSpecHotSpringsSpriteReq.blue = 0x80 - (HOT_SPRINGS_PARTICLES[index].fade << 3);
            g_BattleSpecHotSpringsSpriteReq.green = g_BattleSpecHotSpringsSpriteReq.blue;
            (g_BattleSpecHotSpringsSpriteReq.red) = g_BattleSpecHotSpringsSpriteReq.green;
            // The position of record index, reached from the first record's position symbol.
            BattleFx_InstallBillboardMatrix((short *)(index * 0x18 + (char *)&g_BattleSpecHotSpringsFirstParticlePos), HOT_SPRINGS_PARTICLES[index].scale, 0);
            g_BattleSpecHotSpringsDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecHotSpringsResourceModelRows[4], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecHotSpringsDrawHandle);
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter < 60) {
        for (index = 0; index < (0x3C - task->frameCounter) / 5 + 1; index++) {
            emitter = (BattleFxHotSpringsParticle *)BattleFx_SpecialHotSpringsClaimParticleRecord();
            if ((unsigned int)(int)emitter > (unsigned int)0) {
                angle = (rand() % 4096);
                rad = rand() % 200;
                emitter->pos.vx = g_BattleSpecHotSpringsSourceBonePosXY.x + (CosFixed(angle) * rad >> 12);
                emitter->pos.vz = g_BattleSpecHotSpringsSourceBonePosZ + (SinFixed(angle) * rad >> 12);
                emitter->pos.vy = g_BattleSpecHotSpringsSourceBonePosXY.y;
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
    for (emitter = HOT_SPRINGS_PARTICLES; index < HOT_SPRINGS_PARTICLE_COUNT; index++) {
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
 * Claims the first free particle record of g_BattleSpecHotSpringsParticlePool, marking it live and
 * clearing its fade, and returns its address; returns zero when the pool is full.
 */
// FUNCTION: FF7 0x0053da2b
int __cdecl BattleFx_SpecialHotSpringsClaimParticleRecord(void)
{
    short i;

    for (i = 0; i < HOT_SPRINGS_PARTICLE_COUNT; i++) {
        if (HOT_SPRINGS_PARTICLES[i].state == 0) {
            HOT_SPRINGS_PARTICLES[i].state = 1;
            HOT_SPRINGS_PARTICLES[i].fade = 0;
            return (int)&HOT_SPRINGS_PARTICLES[i];
        }
    }
    return 0;
}
} /* extern "C" */
