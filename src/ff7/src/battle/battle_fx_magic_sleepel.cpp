// src/ff7/src/battle/battle_fx_magic_sleepel.cpp
/* Magic Effect [11]: Sleepel (Japanese: スリプル, dev tag: "MAGIC/SLIPL" / Slipl) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle visual effect for the Slipl magic: each target gets a spinning, growing and
//   fading model, plus drifting quad particles around it.

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

struct BattleFxModelDraw;

#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x008b3f30
int g_BattleMagicSleepelResourceDefs[6] = {1638577, (int)&g_BattleMagicSleepelFilename, 0, 0, 0, 0};
// GLOBAL: FF7 0x008b3f48
unsigned char g_BattleMagicSleepelSpriteDrawReq[8] = {0xf0, 0xff, 0xe0, 0xff, 0x00, 0x00, 0x1f, 0x1f};
// GLOBAL: FF7 0x008b3f50
unsigned char g_BattleMagicSleepelModelDrawReq[16] = {0x80, 0x80, 0x80, 0x2e, 0x3b, 0x00, 0xc7, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// The MATRIX SetRotMatrix and SetTransMatrix take by number; its m[2][2] is 0x1000.
// GLOBAL: FF7 0x008b3f60
unsigned char g_BattleMagicSleepelTransformMatrix[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008b3f80
unsigned char g_BattleMagicSleepelFilename[12] = "slipl.rsd";
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00bb218c
struct GfxTableCursor *g_BattleMagicSleepelLoadedResTable = 0;

/*
 * Loads resources for the Slipl magic effect.
 */
// FUNCTION: FF7 0x00588d10
void __cdecl BattleFx_MagicLoadResSleepel(void)
{
    char pathOut[200];
    GfxModelTableBank *param;
    GfxResourceRequest requestStorage;
    int kind;
    GfxResourceRequest *requestPtr;
    int resultDefCollection;

    if (g_pBattle3dScene != 0 && g_BattleMagicSleepelLoadedResTable == 0) {
        kind = 1;
        resultDefCollection = (int)&g_BattleMagicSleepelResourceDefs;
        param = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        requestPtr = &requestStorage;
        Gfx_InitResourceRequest(requestPtr);
        requestPtr->groupTag = 0;
        // STRING: FF7 0x008b3f8c
        requestPtr->resourcePath = (char *)BattleFx_BuildResourcePath("MAGIC/SLIPL", pathOut);
        Gfx_LoadModelTableIfAbsent(kind, resultDefCollection, requestPtr, &g_BattleMagicSleepelLoadedResTable, param);
    }
}

/*
 * Entry point for the Slipl magic effect.
 */
// FUNCTION: FF7 0x00588da0
void __cdecl BattleFx_MagicEnterSleepel(int targetMask, int actorIdx)
{
    BattleFx_MagicBeginSleepel(targetMask, actorIdx);
}

/*
 * Initializes Slipl resources and spawns per-target tasks.
 */
// FUNCTION: FF7 0x00588db5
void __cdecl BattleFx_MagicBeginSleepel(int targetMask, int actorIdx)
{
    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    BattleFx_MagicLoadResSleepel();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 1, BattleFx_MagicSpawnSleepelPerTarget);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x49);
}

/*
 * Spawns spinning model and particle tasks for a Slipl target.
 */
// FUNCTION: FF7 0x00588e0c
void __cdecl BattleFx_MagicSpawnSleepelPerTarget(int actor)
{
    BattleFxSleepelSpawnRecord *pSpawnRecord;
    int padSlot[3]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int scratch;
    BattleFxSleepelSpinModel *model;
    int steps;
    int spinRate;
    int sizeTotal;
    int duration;

    model = (BattleFxSleepelSpinModel *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicSleepelSpinGrowFadeModelTask);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)&model->posX);
    model->posY -= (short)(model->posY >> 1);
    model->orbitAngle = 0;
    model->spinAngle = 0;
    model->diameter = g_BattleActorRecords[actor].radius << 1;
    if (model->diameter > 0x76C) {
        model->diameter = 0x76C;
    }
    scratch = model->diameter / 0x26;
    if (scratch <= 0) {
        steps = 1;
    } else {
        steps = scratch;
    }
    model->steps = steps;
    scratch = -(model->posY * 3 / 0x37);
    if (scratch < 0x16) {
        spinRate = 0x16;
    } else {
        spinRate = scratch;
    }
    model->riseSpeed = spinRate;
    model->scale = 0x400;
    scratch = 0xBC;
    if (scratch < 0x20) {
        duration = 0x20;
    } else {
        duration = scratch;
    }
    model->scaleStep = duration;
    model->actorSlot = actor;
    pSpawnRecord = (BattleFxSleepelSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicSleepelSpawnQuadParticlesTask);
    *(Int64Pair *)((char *)pSpawnRecord + 4) = *(Int64Pair *)((char *)model + 4);
    pSpawnRecord->radius = g_BattleActorRecords[actor].radius;
    pSpawnRecord->diameter = pSpawnRecord->radius << 1;
    pSpawnRecord->sizeTotal = g_BattleActorRecords[actor].scaleX + g_BattleActorRecords[actor].scaleY;
    if (pSpawnRecord->sizeTotal < 0x1000) {
        sizeTotal = 0x1000;
    } else {
        sizeTotal = pSpawnRecord->sizeTotal;
    }
    pSpawnRecord->sizeTotal = sizeTotal;
}

/*
 * Updates and renders spinning, expanding, and fading model for Slipl.
 */
// FUNCTION: FF7 0x00589005
void __cdecl BattleFx_MagicSleepelSpinGrowFadeModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[112];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxSleepelSpinRecord *spinRecord; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offsetX; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix workMatrix; };
        struct { unsigned char pad_to_34[0x34]; PsxMatrix rotMatrixA; };
        struct { unsigned char pad_to_54[0x54]; SVECTOR tiltX; };
        struct { unsigned char pad_to_5C[0x5C]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_60[0x60]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.spinRecord = (BattleFxSleepelSpinRecord *)BATTLE_CURRENT_TASK_SLOT;
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.rotX.vx = 0x800;
    frame.rotX.vy = frame.spinRecord->spinAngleY + 0x400;
    frame.rotX.vz = 0;
    RotMatrix(&frame.rotX, &frame.rotMatrixA);
    frame.tiltX.vx = 0;
    frame.tiltX.vy = frame.spinRecord->orbitAngleY;
    frame.tiltX.vz = 0;
    RotMatrix(&frame.tiltX, &frame.workMatrix);
    frame.offsetX.vx = frame.spinRecord->orbitRadius;
    frame.offsetX.vy = 0;
    frame.offsetX.vz = 0;
    ApplyMatrixToSVectorInt32(&frame.workMatrix, &frame.offsetX, &frame.rotMatrixA.t[0]);
    frame.rotMatrixA.t[0] += frame.spinRecord->posX;
    frame.rotMatrixA.t[1] += frame.spinRecord->posY;
    frame.rotMatrixA.t[2] += frame.spinRecord->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrixA, &frame.workMatrix);
    frame.scaleVec.vz = frame.spinRecord->scale;
    frame.scaleVec.vy = frame.scaleVec.vz;
    frame.scaleVec.vx = frame.scaleVec.vy;
    ScaleMatrix(&frame.workMatrix, &frame.scaleVec);
    SetRotMatrix(&frame.workMatrix);
    SetTransMatrix(&frame.workMatrix);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleMagicSleepelLoadedResTable);
    frame.modelDraw->flags = 8;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0x80;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    if (frame.spinRecord->phase < 6) {
        frame.modelDraw->fadeLevel -= (short)((6 - frame.spinRecord->phase) * 0x15);
    } else if (frame.spinRecord->phase >= 0x26) {
        frame.modelDraw->fadeLevel -= (short)((frame.spinRecord->phase - 0x26) * 7);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.spinRecord->phase >= 0x37) {
        frame.spinRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.spinRecord->phase == 0x26) {
        Battle_PostActorHitOrHurtMessage(frame.spinRecord->actorSlot);
    }
    frame.spinRecord->spinAngleY -= 0x100;
    if (frame.spinRecord->phase >= 0x20) {
        frame.spinRecord->spinAngleY += (short)((frame.spinRecord->phase - 0x20) * 0xB);
    }
    frame.spinRecord->orbitAngleY -= 0x100;
    frame.spinRecord->orbitRadius -= frame.spinRecord->orbitRadiusStep;
    if (frame.spinRecord->orbitRadius <= 0) {
        frame.spinRecord->orbitRadius = 0;
    }
    frame.spinRecord->posY -= frame.spinRecord->riseSpeed;
    if (frame.spinRecord->phase >= 0x26) {
        frame.spinRecord->posY -= frame.spinRecord->riseSpeed;
    }
    frame.spinRecord->scale += frame.spinRecord->scaleStep;
    if (frame.spinRecord->phase >= 0x26) {
        frame.spinRecord->scale += (short)(frame.spinRecord->scaleStep >> 1);
    }
    (frame.spinRecord->phase)++;
}

/*
 * Spawns drifting quad particles around Slipl target.
 */
// FUNCTION: FF7 0x0058932b
void __cdecl BattleFx_MagicSleepelSpawnQuadParticlesTask(void)
{
    BattleSleepelQuadParticle *particle;
    int i;
    BattleFxSleepelQuadParticle *task;

    if (g_BattleIsPaused != 0) {
        return;
    }
    task = (BattleFxSleepelQuadParticle *)BATTLE_CURRENT_TASK_SLOT;
    if (task->timer > 8) {
        for (i = 0; i < 2; i++) {
            particle = (BattleSleepelQuadParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicSleepelQuadParticleTask);
            particle->posX = task->posX + rand() % task->jitterRange - task->jitterOffset;
            particle->posY = task->posY + rand() % task->jitterRange - task->jitterOffset;
            particle->posZ = task->posZ + rand() % task->jitterRange - task->jitterOffset;
            particle->depth = task->depth;
            particle->scaleX = particle->depth;
        }
    }
    if (task->timer > 0x1E) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->timer)++;
}

/*
 * Updates physics and renders floating quad particle for Slipl.
 */
// FUNCTION: FF7 0x00589461
void __cdecl BattleFx_MagicSleepelQuadParticleTask(void)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxSleepelQuadParticle *q;

    q = (BattleFxSleepelQuadParticle *)BATTLE_CURRENT_TASK_SLOT;
    ((BattleSpriteQuadRequest *)&g_BattleMagicSleepelSpriteDrawReq)->fadeLevel = q->timer << 5;
    *(short *)&g_BattleMagicSleepelTransformMatrix = q->jitterRange;
    *(short *)((char *)&g_BattleMagicSleepelTransformMatrix + 8) = q->depth;
    BattleFx_MakeViewSpacePointPushed(&q->posX, -(q->jitterRange >> 4), (Vec3sPacked *)&g_BattleMagicSleepelTransformMatrix);
    SetRotMatrix(&g_BattleMagicSleepelTransformMatrix);
    SetTransMatrix(&g_BattleMagicSleepelTransformMatrix);
    *(int *)((char *)&g_BattleMagicSleepelModelDrawReq + 8) = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)&g_BattleMagicSleepelSpriteDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
    if (g_BattleIsPaused == 0) {
        q->timer += 1;
        if (q->timer >= 7) {
            q->ownerSlot = -1;
        }
    }
}
} /* extern "C" */
