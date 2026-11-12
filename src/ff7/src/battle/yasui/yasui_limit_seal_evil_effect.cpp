// src/ff7/src/battle/yasui/yasui_limit_seal_evil_effect.cpp
// Limit Break [15] (Aeris Lv 1-2): Seal Evil (Japanese: 邪気封印, dev tag: "LIMIT/JAKI" / Jaki)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle effects for the Jaki limit break: a particle pool of billboard and oriented
//   sprite bursts around each target, and a master timeline that drives the screen flashes and
//   sounds.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_boss.h"
#include "ff7/battle_tasks.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include <string.h>
#include <stdlib.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
extern "C" {

struct BattleOrthoMatrix;
// clang-format off
// Two resource records Gfx_LoadModelTableIfAbsent takes: {kind 0x192A39, tim, spt, 0, 0}.
// GLOBAL: FF7 0x007ee308
unsigned int g_YasuiLimitSealEvilResourceDef[10] = {0x00192a39, (unsigned int)"JAKI.TIM", (unsigned int)"KOUKASEN.SPT", 0, 0,
    0x00192a39, (unsigned int)"JAKI.TIM", (unsigned int)"RYUUSHI.SPT", 0, 0};
// Two sprite draw records {table group pointer, field 4, CVECTOR 0x2E808080, 0}; 16 bytes each.
// GLOBAL: FF7 0x007ee330
struct YasuiLimitSealEvilSpriteRequest1 g_YasuiLimitSealEvilSpriteRequest1 = {0, 0, -128, -128, -128, {0x2e}, 0, {0}};
// GLOBAL: FF7 0x007ee340
struct YasuiLimitSealEvilSpriteRequest2 g_YasuiLimitSealEvilSpriteRequest2 = {0, 0, -128, -128, -128, {0x2e, 0, 0, 0, 0}};
// The rotation MATRIX ConcatMatrixPreservingTranslation takes {short m[3][3], pad, long t[3]}; 32 bytes.
// GLOBAL: FF7 0x007ee350
short g_YasuiLimitSealEvilBillboardTransform[16] = {4096, 0, 0, 0, 0, -4096, 0, 4096, 0, 0, 0, 0, 0, 0, 0, 0};
// clang-format on

#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
#pragma intrinsic(abs)


// clang-format off
// clang-format on

// clang-format off
// GLOBAL: FF7 0x00a54628
void (__cdecl *g_YasuiLimitSealEvilParticleCallbacks[0xAA])(int) = {0};
// GLOBAL: FF7 0x00a548d0
int g_YasuiLimitSealEvilSpriteDrawSlot = 0;
// GLOBAL: FF7 0x00a548d4
int g_YasuiLimitSealEvilTargetMask = 0;
// GLOBAL: FF7 0x00a548d8
int g_YasuiLimitSealEvilBillboardCount = 0;
unsigned char g_bssPad_00a548dc[4] = {0};
// GLOBAL: FF7 0x00a548e0
PsxMatrix g_YasuiLimitSealEvilParticleMatrixPool[150] = {0};
unsigned char g_bssPad_00a55a74[4] = {0};
// GLOBAL: FF7 0x00a55a78
struct YasuiLimitSealEvilParticle g_YasuiLimitSealEvilParticlePool[170] = {0};
// GLOBAL: FF7 0x00a56d10
int g_YasuiLimitSealEvilOrientedCount = 0;
// GLOBAL: FF7 0x00a56d14
int g_YasuiLimitSealEvilMatrixPoolIdx = 0;
// GLOBAL: FF7 0x00a56d18
struct GfxTableCursor *g_YasuiLimitSealEvilLoadedResTable = 0;
unsigned char g_bssPad_00a56d1c[4] = {0};
// clang-format on

/*
 * Loads the "LIMIT/JAKI" resource package for the Jaki limit break.
 */
// FUNCTION: FF7 0x00472ec0
void __cdecl BattleFx_LimitLoadResSealEvil(void)
{
    char pathScratch[200];
    GfxModelTableBank *context;
    GfxResourceRequest requestRecord;
    int kind;
    GfxResourceRequest *pReq;
    int resourceRows;

    if (g_pBattle3dScene != 0 && g_YasuiLimitSealEvilLoadedResTable == 0) {
        kind = 2;
        resourceRows = (int)&g_YasuiLimitSealEvilResourceDef;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestRecord;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x007ee3a4
        pReq->resourcePath = BattleFx_BuildResourcePath("LIMIT/JAKI", pathScratch);
        Gfx_LoadModelTableIfAbsent(kind, resourceRows, pReq, &g_YasuiLimitSealEvilLoadedResTable, context);
    }
}

/*
 * Entry point for Jaki limit effect; loads resources and begins effect.
 */
// FUNCTION: FF7 0x00472f50
int __cdecl BattleFx_LimitEnterSealEvil(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResSealEvil();
    BattleFx_LimitBeginSealEvil(targetMask, actorIdx);
    return 0;
}

/*
 * Initializes particle pool and spawns per-target timeline and master timeline tasks.
 */
// FUNCTION: FF7 0x00472f6c
void __cdecl BattleFx_LimitBeginSealEvil(int targetMask, int actorIdx)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int q;

    g_YasuiLimitSealEvilTargetMask = targetMask;
    q = Battle_PopCountActorMask(g_YasuiLimitSealEvilTargetMask);
    g_YasuiLimitSealEvilOrientedCount = 0xE / q;
    g_YasuiLimitSealEvilBillboardCount = g_YasuiLimitSealEvilOrientedCount;
    BattleFx_LimitSealEvilClearParticleTable();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 0, BattleFx_LimitSpawnSealEvilPerTarget);
    Battle_AllocateResourceSlot(BattleFx_LimitSealEvilParticlePoolTask);
    Battle_AllocateResourceSlot(BattleFx_LimitSealEvilTimelineTask);
}

/*
 * Clears the active particle callback table.
 */
// FUNCTION: FF7 0x00472fe0
void __cdecl BattleFx_LimitSealEvilClearParticleTable(void)
{
    int i;

    for (i = 0; i < 0xAA; i++) {
        g_YasuiLimitSealEvilParticleCallbacks[i] = 0;
    }
}

/*
 * Per-frame task iterating active particle callbacks and updating their states.
 */
// FUNCTION: FF7 0x00473013
void __cdecl BattleFx_LimitSealEvilParticlePoolTask(void)
{
    int activeCount;
    BattleTaskSlot *task;
    int particleIdx;

    task = BATTLE_CURRENT_TASK_SLOT;
    activeCount = 0;
    for (particleIdx = 0; particleIdx < 0xAA; particleIdx++) {
        if (g_YasuiLimitSealEvilParticleCallbacks[particleIdx] != 0) {
            g_YasuiLimitSealEvilParticleCallbacks[particleIdx](particleIdx * 0x1C + (int)&g_YasuiLimitSealEvilParticlePool);
            if (g_YasuiLimitSealEvilParticlePool[particleIdx].state == -1) {
                g_YasuiLimitSealEvilParticleCallbacks[particleIdx] = 0;
            } else {
                activeCount++;
            }
        }
    }
    if (activeCount == 0 && g_BattleSecondaryTaskActiveCount <= 1) {
        task->ownerSlot = -1;
    }
}

/*
 * Spawns a timeline task at the target actor bone position.
 */
// FUNCTION: FF7 0x004730be
void __cdecl BattleFx_LimitSpawnSealEvilPerTarget(int actor)
{
    BattleFxLimitSealEvilTargetSlot *slot;

    slot = (BattleFxLimitSealEvilTargetSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitSealEvilPerTargetTimelineTask);
    slot->actorIndex = actor;
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &slot->posX);
}

/*
 * Controls per-target particle burst emission timing.
 */
// FUNCTION: FF7 0x0047310f
void __cdecl BattleFx_LimitSealEvilPerTargetTimelineTask(void)
{
    BattleFxLimitSealEvilTargetSlot *task;
    BattleFxSealEvilBurstParticle *child;

    task = (BattleFxLimitSealEvilTargetSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (DEAD_IN_RETAIL != 0) {
        return;
    }
    if (task->step == 0) {
        child = (BattleFxSealEvilBurstParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitSealEvilParticleBurstTask);
        *(Int64Pair *)&child->posX = *(Int64Pair *)&task->posX;
    }
    if (task->step == 0xF) {
        Battle_PostActorHitOrHurtMessage(task->actorIndex);
    }
    if (task->step == 0x23) {
        task->actorIndex = 0xFFFF;
    }
    (task->step)++;
}

/*
 * Emits billboard and oriented sprite particle bursts around the target.
 */
// FUNCTION: FF7 0x004731ad
void __cdecl BattleFx_LimitSealEvilParticleBurstTask(void)
{
    int i;
    BattleFxSealEvilBurstParticle *pBurst;
    Vec3sPacked *dirVec;
    int matrix;
    BattleFxSealEvilBurstParticle *partPtr;

    pBurst = (BattleFxSealEvilBurstParticle *)BATTLE_CURRENT_TASK_SLOT;
    if (DEAD_IN_RETAIL != 0) {
        return;
    }
    dirVec = (Vec3sPacked *)Gfx_GetPsxScratchpadPtr(0);
    matrix = (int)dirVec + 8;
    for (i = 0; i < g_YasuiLimitSealEvilBillboardCount; i++) {
        partPtr = (BattleFxSealEvilBurstParticle *)(BattleFx_LimitSealEvilClaimParticleRecord((int)BattleFx_LimitSealEvilBillboardSpriteParticle));
        dirVec->vx = 0;
        dirVec->vy = rand() & 0xFFF;
        dirVec->vz = -(rand() & 0x7FF);
        RotMatrix((SVECTOR *)dirVec, (void *)matrix);
        dirVec->vx = rand() % 0x1F4 + 0x1F4;
        dirVec->vy = 0;
        dirVec->vz = 0;
        TransformVectorToInt16((void *)matrix, (void *)dirVec, (void *)dirVec);
        partPtr->posX = pBurst->posX + dirVec->vx * 5;
        partPtr->posY = pBurst->posY + dirVec->vy * 5;
        partPtr->posZ = pBurst->posZ + dirVec->vz * 5;
        partPtr->velocityX = -dirVec->vx;
        partPtr->velocityY = -dirVec->vy;
        partPtr->velocityZ = -dirVec->vz;
        partPtr->lifetime = rand() % 0x1000 + 0x1000;
        partPtr->variant = rand() % 3;
    }
    for (i = 0; i < g_YasuiLimitSealEvilOrientedCount; i++) {
        partPtr = (BattleFxSealEvilBurstParticle *)(BattleFx_LimitSealEvilClaimParticleRecord((int)BattleFx_LimitSealEvilOrientedSpriteParticle));
        dirVec->vx = 0;
        dirVec->vy = rand() & 0xFFF;
        dirVec->vz = -(rand() & 0x7FF);
        RotMatrix((SVECTOR *)dirVec, (void *)matrix);
        dirVec->vx = rand() % 0x1F4 + 0x1F4;
        dirVec->vy = 0;
        dirVec->vz = 0;
        TransformVectorToInt16((void *)matrix, (void *)dirVec, (void *)dirVec);
        partPtr->posX = pBurst->posX + dirVec->vx * 5;
        partPtr->posY = pBurst->posY + dirVec->vy * 5;
        partPtr->posZ = pBurst->posZ + dirVec->vz * 5;
        partPtr->velocityX = -dirVec->vx;
        partPtr->velocityY = -dirVec->vy;
        partPtr->velocityZ = -dirVec->vz;
        partPtr->lifetime = rand() % 0x1000 + 0x1000;
        partPtr->variant = rand() % 3;
        Gte_BuildLookBasisFromDirection((short *)dirVec, (short *)matrix);
        ConcatMatrixPreservingTranslation((void *)matrix, &g_YasuiLimitSealEvilBillboardTransform, (void *)(g_YasuiLimitSealEvilMatrixPoolIdx * 0x1E + (char *)&g_YasuiLimitSealEvilParticleMatrixPool));
        partPtr->matrix = g_YasuiLimitSealEvilMatrixPoolIdx * 0x1E + (int)&g_YasuiLimitSealEvilParticleMatrixPool;
        g_YasuiLimitSealEvilMatrixPoolIdx = (g_YasuiLimitSealEvilMatrixPoolIdx + 1) % 0x96;
    }
    (pBurst->frameCount)++;
    if (pBurst->frameCount >= 0xF) {
        pBurst->variant = 0xFFFF;
    }
}

/*
 * Claims a free particle slot from the particle pool and sets its callback.
 */
// FUNCTION: FF7 0x00473528
int __cdecl BattleFx_LimitSealEvilClaimParticleRecord(int callback)
{
    int i;

    for (i = 0; i < 0xAA; i++) {
        if (g_YasuiLimitSealEvilParticleCallbacks[i] == 0) {
            g_YasuiLimitSealEvilParticleCallbacks[i] = (void (__cdecl *)(int))callback;
            g_YasuiLimitSealEvilParticlePool[i].state = 0;
            g_YasuiLimitSealEvilParticlePool[i].frameCount = 0;
            return i * 0x1C + (int)&g_YasuiLimitSealEvilParticlePool;
        }
    }
    return 0;
}

/*
 * Animates a billboard sprite particle with fade out.
 */
// FUNCTION: FF7 0x00473594
void __cdecl BattleFx_LimitSealEvilBillboardSpriteParticle(YasuiSealEvilParticle *particleRecord)
{
    int val;

    (g_YasuiLimitSealEvilSpriteRequest1.spriteResource) = Gfx_ModelTableGetLoadedRecord(1, g_YasuiLimitSealEvilLoadedResTable);
    g_YasuiLimitSealEvilSpriteRequest1.limitJakiBillboardResource = Gfx_ModelTableGetSpriteObject(1, g_YasuiLimitSealEvilLoadedResTable);
    val = particleRecord->frame;
    if (val < 2) {
        g_YasuiLimitSealEvilSpriteRequest1.limitJakiBillboardScale = (val << 7) / 2;
        g_YasuiLimitSealEvilSpriteRequest1.limitJakiBillboardScaleCopy = g_YasuiLimitSealEvilSpriteRequest1.limitJakiBillboardScale;
        g_YasuiLimitSealEvilSpriteRequest1.scaleSnapshot = g_YasuiLimitSealEvilSpriteRequest1.limitJakiBillboardScaleCopy;
    } else {
        val -= 2;
        if (val < 3) {
            g_YasuiLimitSealEvilSpriteRequest1.limitJakiBillboardScale = val * -0x80 / 2 + 0x80;
            g_YasuiLimitSealEvilSpriteRequest1.limitJakiBillboardScaleCopy = g_YasuiLimitSealEvilSpriteRequest1.limitJakiBillboardScale;
            g_YasuiLimitSealEvilSpriteRequest1.scaleSnapshot = g_YasuiLimitSealEvilSpriteRequest1.limitJakiBillboardScaleCopy;
        } else {
            particleRecord->limitJakiBillboardParticleWord = 0xFFFF;
            return;
        }
    }
    BattleFx_InstallBillboardMatrix(&particleRecord->posX, (short)particleRecord->angle, 0);
    g_YasuiLimitSealEvilSpriteRequest1.limitJakiBillboardParticleWord = particleRecord->limitJakiBillboardParticleWord;
    g_YasuiLimitSealEvilSpriteDrawSlot = BattleFx_SubmitSpriteDraw(&g_YasuiLimitSealEvilSpriteRequest1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitSealEvilSpriteDrawSlot);
    if (1 != 0) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
        (particleRecord->frame)++;
        particleRecord->posX += particleRecord->velocityX;
        particleRecord->posY += particleRecord->velocityY;
        particleRecord->posZ += particleRecord->velocityZ;
    }
}

/*
 * Animates a 3D oriented sprite particle with direction alignment and fade out.
 */
// FUNCTION: FF7 0x004736f0
void __cdecl BattleFx_LimitSealEvilOrientedSpriteParticle(MatrixParticleRecord *particle)
{
    int frameIndex;
    BattleFxLimitSealEvilDelay *scaleBlock;
    BattleOrthoMatrix *orthoMatrix;

    (g_YasuiLimitSealEvilSpriteRequest2.animBlock) = Gfx_ModelTableGetLoadedRecord(0, g_YasuiLimitSealEvilLoadedResTable);
    g_YasuiLimitSealEvilSpriteRequest2.limitJakiOrientedResource = Gfx_ModelTableGetSpriteObject(0, g_YasuiLimitSealEvilLoadedResTable);
    orthoMatrix = (BattleOrthoMatrix *)Gfx_GetPsxScratchpadPtr(0);
    scaleBlock = (BattleFxLimitSealEvilDelay *)((int)orthoMatrix + 0x1E);
    frameIndex = particle->frame;
    if (frameIndex < 2) {
        g_YasuiLimitSealEvilSpriteRequest2.limitJakiOrientedScale = (frameIndex << 7) / 2;
        g_YasuiLimitSealEvilSpriteRequest2.limitJakiOrientedScaleCopy = g_YasuiLimitSealEvilSpriteRequest2.limitJakiOrientedScale;
        g_YasuiLimitSealEvilSpriteRequest2.limitJakiOrientedScaleOut = g_YasuiLimitSealEvilSpriteRequest2.limitJakiOrientedScaleCopy;
    } else {
        frameIndex -= 2;
        if (frameIndex < 3) {
            g_YasuiLimitSealEvilSpriteRequest2.limitJakiOrientedScale = frameIndex * -0x80 / 3 + 0x80;
            g_YasuiLimitSealEvilSpriteRequest2.limitJakiOrientedScaleCopy = g_YasuiLimitSealEvilSpriteRequest2.limitJakiOrientedScale;
            g_YasuiLimitSealEvilSpriteRequest2.limitJakiOrientedScaleOut = g_YasuiLimitSealEvilSpriteRequest2.limitJakiOrientedScaleCopy;
        } else {
            particle->active = 0xFFFF;
            return;
        }
    }
    particle->matrix->t[0] = (int)particle->x;
    particle->matrix->t[1] = (int)particle->y;
    particle->matrix->t[2] = (int)particle->z;
    MultiplyMatrix(&g_BattleCameraTransform, (void *)(((BattleFxSealEvilSpriteParticle *)particle)->matrix), orthoMatrix);
    Gte_OrthonormaliseFromYAxis(orthoMatrix);
    scaleBlock->nextValue = ((BattleFxSealEvilSpriteParticle *)particle)->nextValue;
    scaleBlock->value = scaleBlock->nextValue;
    scaleBlock->prevValue = scaleBlock->value;
    ScaleMatrix(orthoMatrix, scaleBlock);
    SetRotMatrix((void *)orthoMatrix);
    SetTransMatrix(orthoMatrix);
    g_YasuiLimitSealEvilSpriteDrawSlot = BattleFx_SubmitSpriteDraw(&g_YasuiLimitSealEvilSpriteRequest2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitSealEvilSpriteDrawSlot);
    if (1 != 0) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
        (particle->frame)++;
        particle->x += ((BattleFxSealEvilSpriteParticle *)particle)->velocityX;
        particle->y += ((BattleFxSealEvilSpriteParticle *)particle)->velocityY;
        particle->z += ((BattleFxSealEvilSpriteParticle *)particle)->velocityZ;
    }
}

/*
 * Master timeline task coordinating screen flashes and sound effects.
 */
// FUNCTION: FF7 0x004738de
void __cdecl BattleFx_LimitSealEvilTimelineTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(g_YasuiLimitSealEvilTargetMask), 0x241);
    }
    if (task->frameCounter == 0x19) {
        BattleFx_StartScreenFlashRamp(1);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, 0x40, 0x189);
    } else if (task->frameCounter == 0x1A) {
        BattleFx_FadeOutScreenFlashRamp(5);
    } else if (task->frameCounter == 0x1E) {
        BattleFx_StartScreenFlashRamp(1);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, 0x40, 0x189);
    } else if (task->frameCounter == 0x1F) {
        BattleFx_FadeOutScreenFlashRamp(0xA);
    }
    if (task->frameCounter == 0x23) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}
} /* extern "C" */
