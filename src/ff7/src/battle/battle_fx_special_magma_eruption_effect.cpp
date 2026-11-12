// src/ff7/src/battle/battle_fx_special_magma_eruption_effect.cpp
/* Enemy Special Attack [118]: Magma Eruption (orig. SPECIAL/MAGMA2) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Magma2': a white flash driven by the shared intensity, plus a
//   particle sprite pool whose records are claimed and drawn each frame.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x008a2478
int g_BattleSpecMagmaEruptionResourceDefs = 0x00192A39;
int g_BattleSpecMagmaEruptionTimNamePtr[1] = {
    (int)&g_BattleSpecMagmaEruptionResourceNames
};
// GLOBAL: FF7 0x008a2480
int g_BattleSpecMagmaEruptionResourceDefRows[6] = {
    (int)&g_BattleSpecMagmaEruptionResourceNames[0xc], 0x0, 0x0, 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x008a2498
struct BattleSpecHeatBlastColor g_BattleSpecMagmaEruptionSpriteReq = {-128, -128, -128, {0x2e}};
unsigned char g_BattleSpecMagmaEruptionResourceNames[24] =
    "CHIKA00.TIM\0"
    "MAGMA.SPT";
// GLOBAL: FF7 0x00b632f8
int g_BattleSpecMagmaEruptionActorBonePos = 0;
// GLOBAL: FF7 0x00b632fc
short g_BattleSpecMagmaEruptionActorBonePosZ = 0;
// The 300 particle records (BattleSpecMagmaParticleRecord, 0x18 bytes each) begin at
// g_BattleSpecMagmaEruptionParticlePool; g_BattleSpecMagmaEruptionParticlePos and the pads below are the rest of
// that span and keep their own symbols, so the records are read through a cast of the first.
// GLOBAL: FF7 0x00b63300
unsigned char g_BattleSpecMagmaEruptionParticlePool[8] = {0};
// GLOBAL: FF7 0x00b63308
int g_BattleSpecMagmaEruptionParticlePos = 0;
unsigned char g_bssPad_00b6330c[4] = {0};
unsigned char g_bssPad_00b63310[7184] = {0};
// GLOBAL: FF7 0x00b64f20
int g_BattleSpecMagmaEruptionPacketCursor = 0;
// GLOBAL: FF7 0x00b64f24
short g_BattleSpecMagmaEruptionTargetMask = 0;
// GLOBAL: FF7 0x00b64f28
struct GfxTableCursor *g_BattleSpecMagmaEruptionLoadedResTable = 0;
// GLOBAL: FF7 0x00b64f2c
short g_BattleSpecMagmaEruptionFlashIntensity = 0;
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/*
 * When the engine context exists and g_BattleSpecMagmaEruptionLoadedResTable is empty, builds SPECIAL/MAGMA2 with a default-
 * duration record and creates resource type 1 using g_BattleSpecMagmaEruptionResourceDefs.
 */
// FUNCTION: FF7 0x00533920
void __cdecl BattleFx_SpecialLoadResMagmaEruption(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecMagmaEruptionLoadedResTable == 0) {
        kind = 1;
        resourceDefsWord = (int)&g_BattleSpecMagmaEruptionResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a24b8
        pRequest->resourcePath = (char *)BattleFx_BuildResourcePath("SPECIAL/MAGMA2", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecMagmaEruptionLoadedResTable, curOwnerCtx);
    }
}

/*
 * Mechanical forwarder: passes both arguments straight to BattleFx_SpecialBeginMagmaEruption and does nothing else.
 */
// FUNCTION: FF7 0x005339b0
void __cdecl BattleFx_SpecialEnterMagmaEruption(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginMagmaEruption(targetMask, actorIdx);
}

/*
 * Stores targetMask, loads SPECIAL/MAGMA2, plays the effect's sound at the actor, frees all 300
 * particle records, captures the world position of the actor's bone33 into
 * g_BattleSpecMagmaEruptionActorBonePos, and starts BattleFx_SpecialMagmaEruptionTimelineTask.
 */
// FUNCTION: FF7 0x005339c5
void __cdecl BattleFx_SpecialBeginMagmaEruption(int targetMask, int actorIdx)
{
    short recordIndex;
    int padWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    g_BattleSpecMagmaEruptionTargetMask = targetMask;
    BattleFx_SpecialLoadResMagmaEruption();
    Battle_PlaySfxPannedToActor(0x165, (short)actorIdx, 0x40);
    for (recordIndex = 0; recordIndex < 300; recordIndex++) {
        ((BattleSpecMagmaParticleRecord *)&g_BattleSpecMagmaEruptionParticlePool)[recordIndex].state = 0;
    }
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].bone33, (short *)&g_BattleSpecMagmaEruptionActorBonePos);
    Battle_AllocateResourceSlot(BattleFx_SpecialMagmaEruptionTimelineTask);
}

/*
 * Draws a white flash from shared intensity before checking pause. When unpaused allocates
 * BattleFx_SpecialMagmaEruptionParticleSpritesTask at counter zero, adds eight to intensity below sixteen and subtracts eight above
 * sixty-four, emits typed records for selected low-ten-bit actors at sixty and alternates shared Y
 * offset by +64/-64. Advances and retires exactly at eighty.
 */
// FUNCTION: FF7 0x00533a52
void __cdecl BattleFx_SpecialMagmaEruptionTimelineTask(void)
{
    short i7;
    BattleFxAvalancheTimelineSlot *pCur;
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int spawned;

    pCur = (BattleFxAvalancheTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(*(char *)&g_BattleSpecMagmaEruptionFlashIntensity, 0, 0);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (pCur->timelineStep == 0) {
        spawned = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialMagmaEruptionParticleSpritesTask);
    }
    if (pCur->timelineStep < 16) {
        g_BattleSpecMagmaEruptionFlashIntensity += 8;
    }
    if (pCur->timelineStep > 64) {
        g_BattleSpecMagmaEruptionFlashIntensity -= 8;
    }
    if (pCur->timelineStep == 60) {
        for (i7 = 0; i7 < 10; i7++) {
            if ((g_BattleSpecMagmaEruptionTargetMask & 1 << i7) != 0) {
                Battle_PostActorHitOrHurtMessage(i7);
            }
        }
    }
    g_BattleCameraFocalPoint.focalPointY += (short)(64 - (pCur->timelineStep % 2 << 7));
    (pCur->timelineStep)++;
    if (pCur->timelineStep == 80) {
        pCur->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Per-frame task of the Magma2 particles: draws every live particle as a sprite that fades with its
 * age, spawns new particles around the actor's bone for the first 60 frames, and moves, ages and
 * retires the live ones. Frees itself once spawning has ended and no particle is live.
 */
// FUNCTION: FF7 0x00533b9e
void __cdecl BattleFx_SpecialMagmaEruptionParticleSpritesTask(void)
{
    short angle, rad;
    BattleTaskSlot *task;
    short oldVy, index;
    short liveCount;
    BattleSpecMagmaParticleRecord *emitter;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecMagmaEruptionResourceDefRows[4] = Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecMagmaEruptionLoadedResTable);
    g_BattleSpecMagmaEruptionResourceDefRows[5] = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecMagmaEruptionLoadedResTable);
    for (index = 0; index < 300; index++) {
        if (((BattleSpecMagmaParticleRecord *)&g_BattleSpecMagmaEruptionParticlePool)[index].state == 1) {
            g_BattleSpecMagmaEruptionSpriteReq.blue = 0x80 - (((BattleSpecMagmaParticleRecord *)&g_BattleSpecMagmaEruptionParticlePool)[index].age << 3);
            g_BattleSpecMagmaEruptionSpriteReq.green = g_BattleSpecMagmaEruptionSpriteReq.blue;
            (g_BattleSpecMagmaEruptionSpriteReq.red) = g_BattleSpecMagmaEruptionSpriteReq.green;
            BattleFx_InstallBillboardMatrix((void *)(index * 0x18 + (int)&g_BattleSpecMagmaEruptionParticlePos), ((BattleSpecMagmaParticleRecord *)&g_BattleSpecMagmaEruptionParticlePool)[index].scale, 0);
            g_BattleSpecMagmaEruptionPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecMagmaEruptionResourceDefRows[4], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecMagmaEruptionPacketCursor);
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter < 60) {
        for (index = 0; index < (0x3C - task->frameCounter) / 5 + 1; index++) {
            emitter = (BattleSpecMagmaParticleRecord *)BattleFx_SpecialMagmaEruptionClaimSpriteRecord();
            if ((unsigned int)(int)emitter > (unsigned int)0) {
                emitter->posX = ((BattleSpecHeatBlastActorBonePos *)&g_BattleSpecMagmaEruptionActorBonePos)->bonePosX + (CosFixed(angle) * rad >> 12);
                emitter->posZ = g_BattleSpecMagmaEruptionActorBonePosZ + (SinFixed(angle) * rad >> 12);
                emitter->posY = ((BattleSpecHeatBlastActorBonePos *)&g_BattleSpecMagmaEruptionActorBonePos)->bonePosY;
                angle = (rand() % 4096);
                rad = rand() % 200;
                emitter->velocityX = CosFixed(angle) * rad >> 12;
                emitter->velocityZ = SinFixed(angle) * rad >> 12;
                emitter->velocityY = (SinFixed(task->frameCounter << 10) * 0x14 >> 12) + task->frameCounter * 2 + -350;
                emitter->scale = 0x1800 - task->frameCounter * 0x1E + ((rand() % 4) << 9);
            }
        }
    }
    index = 0;
    liveCount = 0;
    for (emitter = (BattleSpecMagmaParticleRecord *)&g_BattleSpecMagmaEruptionParticlePool; index < 300; index++) {
        if (emitter->state == 1) {
            emitter->posX += emitter->velocityX;
            emitter->posY += emitter->velocityY;
            emitter->posZ += emitter->velocityZ;
            oldVy = emitter->velocityY;
            emitter->velocityY += 20;
            if (emitter->age == 0) {
                if (oldVy < 0 && emitter->velocityY >= 0) {
                    emitter->age = 1;
                }
            } else {
                (emitter->age)++;
                if (emitter->age == 16) {
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
 * Claims the first free particle record (state 0) of the 300 in g_BattleSpecMagmaEruptionParticlePool by
 * marking it live and clearing its age, and returns its address; returns zero when all are in use.
 */
// FUNCTION: FF7 0x00533f6a
int __cdecl BattleFx_SpecialMagmaEruptionClaimSpriteRecord(void)
{
    short i;

    for (i = 0; i < 300; i++) {
        if (((BattleSpecMagmaParticleRecord *)&g_BattleSpecMagmaEruptionParticlePool)[i].state == 0) {
            ((BattleSpecMagmaParticleRecord *)&g_BattleSpecMagmaEruptionParticlePool)[i].state = 1;
            ((BattleSpecMagmaParticleRecord *)&g_BattleSpecMagmaEruptionParticlePool)[i].age = 0;
            return (int)&((BattleSpecMagmaParticleRecord *)&g_BattleSpecMagmaEruptionParticlePool)[i];
        }
    }
    return 0;
}
} /* extern "C" */
