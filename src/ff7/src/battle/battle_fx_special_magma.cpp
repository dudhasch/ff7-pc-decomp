// src/ff7/src/battle/battle_fx_special_magma.cpp
/* Enemy Special Attack [83]: Magma (orig. SPECIAL/MAGMA) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Magma attack: it loads resources and spawns a per-target
//   timeline that drives a particle sprite task, with particles taken from a shared record pool.

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
// GLOBAL: FF7 0x008a3b50
int g_BattleSpecMagmaResourceDefs = 0x00192A39;
int g_BattleSpecMagmaResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialMagmaResourceNames + 0x0)
};
// GLOBAL: FF7 0x008a3b58
int g_BattleSpecMagmaResourceModelRows[6] = {
    (int)((char *)&g_BattleFxSpecialMagmaResourceNames + 0xc), 0x0, 0x8000, 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x008a3b70
struct BattleSpecHeatBlastColor g_BattleSpecMagmaSpriteReq = {-128, -128, -128, {0x2e}};
unsigned char g_BattleFxSpecialMagmaResourceNames[24] =
    "CHIKA00.TIM\0"
    "MAGMA.SPT";
// The x and y of the source bone's world position; the z follows in g_BattleSpecMagmaOriginZ.
// GLOBAL: FF7 0x00b65100
short g_BattleSpecMagmaOriginXY[2] = {0};
// GLOBAL: FF7 0x00b65104
short g_BattleSpecMagmaOriginZ = 0;
// The pool of 300 BattleFxMagmaParticle records (0x1c20 bytes) starts here and runs through the
// two pads below, up to g_BattleSpecMagmaSpriteDrawList; the code reaches it through this object.
// GLOBAL: FF7 0x00b65108
unsigned char g_BattleSpecMagmaParticlePool[8] = {0};
// The x and y of the first particle's position (pool + 8); the sprite task adds the record stride.
// GLOBAL: FF7 0x00b65110
int g_BattleSpecMagmaParticle0Pos = 0;
unsigned char g_bssPad_00b65114[4] = {0};
unsigned char g_bssPad_00b65118[7184] = {0};
// The packet cursor of the particle sprite draws (BattleFx_SubmitSpriteDraw's result).
// GLOBAL: FF7 0x00b66d28
int g_BattleSpecMagmaSpriteDrawList = 0;
// The loaded SPECIAL/MAGMA resource table; 0 until BattleFx_SpecialLoadResMagma loads it.
// GLOBAL: FF7 0x00b66d2c
struct GfxTableCursor *g_BattleSpecMagmaResourceTable = 0;
// The screen flash intensity, raised and lowered by the timeline task.
typedef struct {
    short level;
    short pad_02;
} BattleSpecMagmaFlash;
// GLOBAL: FF7 0x00b66d30
BattleSpecMagmaFlash g_BattleSpecMagmaFlash = {0};
unsigned char g_bssPad_00b66d34[4] = {0};
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

#define MAGMA_PARTICLE_COUNT 0x12C
#define MAGMA_PARTICLES ((BattleFxMagmaParticle *)&g_BattleSpecMagmaParticlePool)
/* The sprite draw request starts at g_BattleSpecMagmaResourceModelRows[4]: the sprite group and
 * draw object words, then the colour bytes of g_BattleSpecMagmaSpriteReq. */
#define MAGMA_SPRITE_REQ_GROUP 4
#define MAGMA_SPRITE_REQ_DRAW_OBJ 5


/*
 * Loads the SPECIAL/MAGMA resources into g_BattleSpecMagmaResourceTable, if the 3D scene exists
 * and they are not loaded yet.
 */
// FUNCTION: FF7 0x0053ccd0
void __cdecl BattleFx_SpecialLoadResMagma(void)
{
    GfxModelTableBank *stage;
    GfxResourceRequest currentRecord;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecMagmaResourceTable == 0) {
        kind = 1;
        resourceDefsWord = (int)&g_BattleSpecMagmaResourceDefs;
        stage = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a3b90
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/MAGMA", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecMagmaResourceTable, stage);
    }
}

/* Starts the Magma special; forwards to BattleFx_SpecialBeginMagma. */
// FUNCTION: FF7 0x0053cd60
void __cdecl BattleFx_SpecialEnterMagma(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginMagma(targetMask, actorIdx);
}

/*
 * Loads SPECIAL/MAGMA, queues BattleFx_SpecialSpawnMagmaPerTarget for each actor in actorMask,
 * plays the effect sound at targetIndex and frees all 300 particle records.
 */
// FUNCTION: FF7 0x0053cd75
void __cdecl BattleFx_SpecialBeginMagma(int targetMask, int actorIdx)
{
    short i;

    BattleFx_SpecialLoadResMagma();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnMagmaPerTarget);
    Battle_PlaySfxPannedToActor(0x165, (short)actorIdx, 0x40);
    for (i = 0; i < MAGMA_PARTICLE_COUNT; i++) {
        MAGMA_PARTICLES[i].state = 0;
    }
}

/*
 * Starts the timeline task for one target: the world position of sourceIndex's bone goes to
 * g_BattleSpecMagmaOriginXY/Z, that of the target's bone to the task's targetBonePos.
 */
// FUNCTION: FF7 0x0053cddf
void __cdecl BattleFx_SpecialSpawnMagmaPerTarget(int actor, int sourceIndex)
{
    BattleFxMagmaTimeline *taskPtr;

    taskPtr = (BattleFxMagmaTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialMagmaTimelineTask);
    Battle_GetBoneWorldPos(sourceIndex, g_BattleActorRecords[sourceIndex].bone34, g_BattleSpecMagmaOriginXY);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, taskPtr->targetBonePos);
    taskPtr->actorIndex = actor;
}

/*
 * Per-frame task of the effect: flashes the screen with g_BattleSpecMagmaFlash, which rises by 4
 * per frame for the first 16 frames and falls by 4 after frame 0x40. Starts the particle sprite
 * task on frame 0 and allocates a typed record for the target on frame 0x3C. Every frame it moves
 * the camera focal point's Y by 0x20 and back, alternating, and the task ends at frame 0x50.
 */
// FUNCTION: FF7 0x0053ce53
void __cdecl BattleFx_SpecialMagmaTimelineTask(void)
{
    BattleFxMagmaTimeline *mainSelf;
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int spawned;

    mainSelf = (BattleFxMagmaTimeline *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite((unsigned char)g_BattleSpecMagmaFlash.level, 0, 0);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (mainSelf->timer == 0) {
        spawned = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialMagmaParticleSpritesTask);
    }
    if (mainSelf->timer < 16) {
        g_BattleSpecMagmaFlash.level += 4;
    }
    if (mainSelf->timer > 64) {
        g_BattleSpecMagmaFlash.level -= 4;
    }
    if (mainSelf->timer == 60) {
        Battle_PostActorHitOrHurtMessage(mainSelf->actorIndex);
    }
    g_BattleCameraFocalPoint.focalPointY += 32 - ((mainSelf->timer % 2) << 6);
    (mainSelf->timer)++;
    if (mainSelf->timer == 80) {
        mainSelf->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task that draws, spawns and moves the lava particles. For the first 60 frames it claims new
 * particles around the source bone position; each live particle then moves by its velocity, which
 * gains gravity, and fades out 16 frames after the top of its arc. Ends when none is left.
 */
// FUNCTION: FF7 0x0053cf69
void __cdecl BattleFx_SpecialMagmaParticleSpritesTask(void)
{
    short angle, rad;
    BattleTaskSlot *task;
    short oldVy, index;
    short liveCount;
    BattleFxMagmaParticle *emitter;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecMagmaResourceModelRows[MAGMA_SPRITE_REQ_GROUP] = Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecMagmaResourceTable);
    g_BattleSpecMagmaResourceModelRows[MAGMA_SPRITE_REQ_DRAW_OBJ] = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecMagmaResourceTable);
    for (index = 0; index < MAGMA_PARTICLE_COUNT; index++) {
        if (MAGMA_PARTICLES[index].state == 1) {
            g_BattleSpecMagmaSpriteReq.blue = 0x80 - (MAGMA_PARTICLES[index].fade << 3);
            g_BattleSpecMagmaSpriteReq.green = g_BattleSpecMagmaSpriteReq.blue;
            g_BattleSpecMagmaSpriteReq.red = g_BattleSpecMagmaSpriteReq.green;
            BattleFx_InstallBillboardMatrix((short *)(index * 0x18 + (char *)&g_BattleSpecMagmaParticle0Pos), MAGMA_PARTICLES[index].scale, 0);
            g_BattleSpecMagmaSpriteDrawList = BattleFx_SubmitSpriteDraw(&g_BattleSpecMagmaResourceModelRows[MAGMA_SPRITE_REQ_GROUP], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecMagmaSpriteDrawList);
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter < 60) {
        for (index = 0; index < (0x3C - task->frameCounter) / 5 + 1; index++) {
            emitter = (BattleFxMagmaParticle *)BattleFx_SpecialMagmaClaimParticleRecord();
            if ((unsigned int)(int)emitter > (unsigned int)0) {
                angle = (rand() % 4096);
                rad = rand() % 200;
                emitter->pos.vx = g_BattleSpecMagmaOriginXY[0] + (CosFixed(angle) * rad >> 12);
                emitter->pos.vz = g_BattleSpecMagmaOriginZ + (SinFixed(angle) * rad >> 12);
                emitter->pos.vy = g_BattleSpecMagmaOriginXY[1];
                angle = (rand() % 4096);
                rad = rand() % 100;
                emitter->vel.vx = CosFixed(angle) * rad >> 12;
                emitter->vel.vz = SinFixed(angle) * rad >> 12;
                emitter->vel.vy = (SinFixed(task->frameCounter << 10) * 0x14 >> 12) + task->frameCounter * 2 + -350;
                emitter->scale = 0xC00 - task->frameCounter * 0x1E + ((rand() % 4) << 9);
            }
        }
    }
    index = 0;
    liveCount = 0;
    for (emitter = MAGMA_PARTICLES; index < MAGMA_PARTICLE_COUNT; index++) {
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

/* Claims the first free particle record, marks it live and returns its address; 0 when all are live. */
// FUNCTION: FF7 0x0053d35d
int __cdecl BattleFx_SpecialMagmaClaimParticleRecord(void)
{
    short i;

    for (i = 0; i < MAGMA_PARTICLE_COUNT; i++) {
        if (MAGMA_PARTICLES[i].state == 0) {
            MAGMA_PARTICLES[i].state = 1;
            MAGMA_PARTICLES[i].fade = 0;
            return (int)&MAGMA_PARTICLES[i];
        }
    }
    return 0;
}
} /* extern "C" */
