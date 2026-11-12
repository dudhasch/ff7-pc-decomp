// src/ff7/src/battle/yasui/yasui_limit_planet_protector.cpp
// Limit Break [18] (Aeris Lv 3-1): Planet Protector (Japanese: 星の守護, dev tag: "LIMIT/S_GUARD" / SGuard)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Planet Protector (S_GUARD) limit break effect: resource loading, a spiral and radial
//   particle pool drawn as fading sprites, and per-target timeline tasks with a delayed sound.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_boss.h"
#include "ff7/battle_tasks.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
extern "C" {

// clang-format off
// The resource record Gfx_LoadModelTableIfAbsent takes {kind 0x192A39, tim, spt, 0, 0}; its names are its literals.
// GLOBAL: FF7 0x007e3540
unsigned int g_YasuiLimitPlanetProtectorResourceDef[5] = {0x00192a39, (unsigned int)"S_GUARD.TIM", (unsigned int)"STARS.SPT", 0, 0};
// Two sprite draw records BattleFx_SubmitSpriteDraw takes {table group pointer, field 4, CVECTOR, 0}; 16 bytes each.
// GLOBAL: FF7 0x007e3558
struct YasuiLimitPlanetProtectorSpriteDrawReq1 g_YasuiLimitPlanetProtectorSpriteDrawReq1 = {0, 0, -128, -128, -128, {0x2e, 0, 0, 0, 0}};
// GLOBAL: FF7 0x007e3568
struct YasuiLimitPlanetProtectorSpriteDrawReq2 g_YasuiLimitPlanetProtectorSpriteDrawReq2 = {0, 0, 0x2e808080, {0}};
// clang-format on

#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
#pragma intrinsic(abs)

#define g_BattleLimitPlanetProtectorDampedColorChannel1 (*(char *)((char *)&g_YasuiLimitPlanetProtectorSpriteDrawReq2 + 9))
#define g_BattleLimitPlanetProtectorDampedColorChannel2 (*(char *)((char *)&g_YasuiLimitPlanetProtectorSpriteDrawReq2 + 10))


// clang-format off
// GLOBAL: FF7 0x00a38b68
int g_YasuiLimitPlanetProtectorState[2] = {0};
// GLOBAL: FF7 0x00a38b70
unsigned char g_YasuiLimitPlanetProtectorParticlePool[2388] = {0};
unsigned char g_bssPad_00a394c4[4] = {0};
unsigned char g_bssPad_00a394c8[8] = {0};
// GLOBAL: FF7 0x00a394d0
struct GfxTableCursor *g_YasuiLimitPlanetProtectorResourceContext = 0;
unsigned char g_bssPad_00a394d4[4] = {0};
// clang-format on

/*
 * Loads Planet Protector limit effect resources (LIMIT/S_GUARD) into
 * g_YasuiLimitPlanetProtectorResourceContext if not already loaded.
 */
// FUNCTION: FF7 0x0045a2c0
void __cdecl BattleFx_LimitLoadResPlanetProtector(void)
{
    GfxModelTableBank *bits;
    GfxResourceRequest requestStorage;
    int kind;
    char pathBuffer[200];
    GfxResourceRequest *requestPtr;
    int def;

    if (g_pBattle3dScene != 0 && g_YasuiLimitPlanetProtectorResourceContext == 0) {
        kind = 1;
        def = (int)&g_YasuiLimitPlanetProtectorResourceDef;
        bits = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        requestPtr = &requestStorage;
        Gfx_InitResourceRequest(requestPtr);
        requestPtr->groupTag = 0;
        // STRING: FF7 0x007e3590
        requestPtr->resourcePath = BattleFx_BuildResourcePath("LIMIT/S_GUARD", pathBuffer);
        Gfx_LoadModelTableIfAbsent(kind, def, requestPtr, &g_YasuiLimitPlanetProtectorResourceContext, bits);
    }
}

/*
 * Entry point for Planet Protector (S_GUARD) limit break effect:
 * loads resources and initiates the effect.
 */
// FUNCTION: FF7 0x0045a350
int __cdecl BattleFx_LimitEnterPlanetProtector(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResPlanetProtector();
    BattleFx_LimitBeginPlanetProtector(targetMask, actorIdx);
    return 0;
}

/*
 * Initializes Planet Protector effect: clears particle pool, allocates fading
 * sprite pool task, creates per-target spawn callbacks, and schedules delayed sound.
 */
// FUNCTION: FF7 0x0045a36c
void __cdecl BattleFx_LimitBeginPlanetProtector(int targetMask, int actorIdx)
{
    BattleFx_LimitPlanetProtectorClearParticleRecords();
    Battle_AllocateResourceSlot(BattleFx_LimitPlanetProtectorFadingSpritePoolTask);
    Battle_CreateBitPatternCallbackTask(g_YasuiLimitPlanetProtectorState[1] = targetMask, actorIdx, 0, BattleFx_LimitSpawnPlanetProtectorPerTarget);
    Battle_AllocateResourceSlot(BattleFx_LimitPlanetProtectorDelayedSoundTask);
}

/*
 * Iterates through active particles in the pool, applies scaling and alpha decay,
 * and submits sprite draw requests until all particles expire.
 */
// FUNCTION: FF7 0x0045a3b3
void __cdecl BattleFx_LimitPlanetProtectorFadingSpritePoolTask(void)
{
    int activeCount;
    BattleFxLimitSealEvilDelay *scaleBlock;
    BattleTaskSlot *taskPointer;
    int matrixAddress;
    int spriteIdx;
    BattleFxSourcePoint *angleBlock;
    int mtxTrans;
    BattleFxLimitPlanetProtectorSprite *pPoolSprite;

    taskPointer = BATTLE_CURRENT_TASK_SLOT;
    (g_YasuiLimitPlanetProtectorSpriteDrawReq1.animBlock) = Gfx_ModelTableGetLoadedRecord(0, g_YasuiLimitPlanetProtectorResourceContext);
    g_YasuiLimitPlanetProtectorSpriteDrawReq1.battleLimitSGuardSpriteResource = Gfx_ModelTableGetSpriteObject(0, g_YasuiLimitPlanetProtectorResourceContext);
    matrixAddress = Gfx_GetPsxScratchpadPtr(0);
    mtxTrans = matrixAddress + 0x1E;
    angleBlock = (BattleFxSourcePoint *)(mtxTrans + 4);
    scaleBlock = (BattleFxLimitSealEvilDelay *)((int)angleBlock + 8);
    pPoolSprite = (BattleFxLimitPlanetProtectorSprite *)&g_YasuiLimitPlanetProtectorParticlePool;
    activeCount = 0;
    angleBlock->y = 0;
    angleBlock->x = 0;
    for (spriteIdx = 0; spriteIdx < 0x96; spriteIdx++) {
        if (pPoolSprite->fadeLevel > 0) {
            SetRotMatrix(&g_BattleCameraTransform);
            SetTransMatrix(&g_BattleCameraTransform);
            RotTransPoint((void *)((char *)pPoolSprite + 4), (void *)(matrixAddress + 0x12), (void *)mtxTrans);
            angleBlock->z = pPoolSprite->rotZ;
            RotMatrix((SVECTOR *)angleBlock, (void *)matrixAddress);
            scaleBlock->nextValue = pPoolSprite->baseLevel * ((pPoolSprite->fadeLevel << 0xC) / 8) >> 0xC;
            scaleBlock->value = scaleBlock->nextValue;
            scaleBlock->prevValue = scaleBlock->value;
            ScaleMatrix((void *)matrixAddress, scaleBlock);
            SetRotMatrix((void *)matrixAddress);
            SetTransMatrix((void *)matrixAddress);
            g_YasuiLimitPlanetProtectorSpriteDrawReq1.battleLimitSGuardSpriteFadeAlpha = (pPoolSprite->fadeLevel << 7) / 8;
            g_YasuiLimitPlanetProtectorSpriteDrawReq1.battleLimitSGuardSpriteFadeCopy = g_YasuiLimitPlanetProtectorSpriteDrawReq1.battleLimitSGuardSpriteFadeAlpha;
            g_YasuiLimitPlanetProtectorSpriteDrawReq1.colorRed = g_YasuiLimitPlanetProtectorSpriteDrawReq1.battleLimitSGuardSpriteFadeCopy;
            g_YasuiLimitPlanetProtectorState[0] = BattleFx_SubmitSpriteDraw(&g_YasuiLimitPlanetProtectorSpriteDrawReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitPlanetProtectorState[0]);
            pPoolSprite->fadeLevel -= 2;
            activeCount++;
        }
        pPoolSprite++;
    }
    if (activeCount == 0 && g_BattleSecondaryTaskActiveCount <= 2) {
        taskPointer->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Resets the 150-entry particle pool by zeroing the lifetime header word of each record.
 */
// FUNCTION: FF7 0x0045a5b7
void __cdecl BattleFx_LimitPlanetProtectorClearParticleRecords(void)
{
    int i;

    for (i = 0; i < 0x96; i++) {
        ((YasuiLimitPlanetProtectorParticle *)&g_YasuiLimitPlanetProtectorParticlePool)[i].state = 0;
    }
}

/*
 * Dispatches delayed Planet Protector sound effect with entity panning at frame 32.
 */
// FUNCTION: FF7 0x0045a5eb
void __cdecl BattleFx_LimitPlanetProtectorDelayedSoundTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0x20) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(g_YasuiLimitPlanetProtectorState[1]), 0x7D);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/*
 * Allocates timeline task for an individual target actor and queries target bone world position.
 */
// FUNCTION: FF7 0x0045a655
void __cdecl BattleFx_LimitSpawnPlanetProtectorPerTarget(int actor)
{
    BattleTaskSlot *slot;

    slot = BATTLE_NEW_TASK_SLOT(BattleFx_LimitPlanetProtectorTimelineTask);
    slot->ownerSlot = actor;
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)slot->data);
}

/*
 * Main timeline task driving spiral particle emitter tasks and radial burst particles.
 */
// FUNCTION: FF7 0x0045a6a6
void __cdecl BattleFx_LimitPlanetProtectorTimelineTask(void)
{
    int burstIndex;
    BattleFxLimitPlanetProtectorTimelineRecord *taskPtr;
    int unusedSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxPlanetProtectorParams *paramsPtr;
    int matrixAddr;
    BattleFxPlanetProtectorParticle *particle;

    taskPtr = (BattleFxLimitPlanetProtectorTimelineRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    paramsPtr = (BattleFxPlanetProtectorParams *)Gfx_GetPsxScratchpadPtr(0);
    matrixAddr = (int)paramsPtr + 8;
    if (taskPtr->step == 0) {
        g_BattleFxTimelineState.red = 0x80;
        g_BattleFxTimelineState.channel1 = 0x60;
        g_BattleFxTimelineState.channel2 = 0;
        Battle_StartFixedPointRamp(0x800, 0xA);
    }
    if (taskPtr->step >= 0x19 && taskPtr->step < 0x32 && (taskPtr->step - 0x19) % 5 == 0) {
        particle = (BattleFxPlanetProtectorParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitPlanetProtectorSpiralSpritePoolEmitterTask);
        particle->posA = taskPtr->posA;
        particle->posY = -(rand() % 0x3E8 + 0x1194);
        particle->posB = taskPtr->posB;
        particle->startRadius = 0x1388;
        particle->startAngle = ((taskPtr->step - 0x19) / 5 << 0xC) / 5;
        particle->riseSpeed = 0x226;
        particle->radius = rand() % 0x1000 + 0x800;
        particle->ownerSlot = rand() & 1;
        particle->phase = rand() & 0xFFF;
    }
    if (taskPtr->step == 0x46) {
        for (burstIndex = 0; burstIndex < 0x14; burstIndex++) {
            particle = (BattleFxPlanetProtectorParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitPlanetProtectorDampedSpriteParticleTask);
            particle->ownerSlot = rand() % 5 + 5;
            particle->radius = rand() % 0x1000 + 0x800;
            paramsPtr->offsetX = rand() % 0x800 - 0x400;
            paramsPtr->velocityY = rand() & 0xFFF;
            paramsPtr->offsetZ = 0;
            RotMatrix((SVECTOR *)paramsPtr, (void *)matrixAddr);
            paramsPtr->offsetX = 0;
            paramsPtr->velocityY = -(rand() % 0xC8 + 0x12C);
            paramsPtr->offsetZ = 0;
            TransformVectorToInt16((void *)matrixAddr, (void *)paramsPtr, (char *)particle + 0xC);
            *(Int64Pair *)((char *)particle + 4) = *(Int64Pair *)((char *)taskPtr + 4);
            particle->phase = rand() & 0xFFF;
        }
    }
    if (taskPtr->step == 0x46) {
        Battle_PostActorHitOrHurtMessage(taskPtr->recordType);
    }
    if (taskPtr->step == 0x46) {
        Battle_StartFixedPointRamp(0, 0xA);
        taskPtr->recordType = 0xFFFF;
    }
    (taskPtr->step)++;
}

/*
 * Emits ascending spiral particle pairs with decaying radius and upward motion over 20 frames.
 */
// FUNCTION: FF7 0x0045a998
void __cdecl BattleFx_LimitPlanetProtectorSpiralSpritePoolEmitterTask(void)
{
    BattleFxSpiralEmitter *taskPtr;
    int spiralAngle;
    int dataAddress;
    int pairIndex;
    BattleFxPlanetProtectorSpiralParticle *particle;
    int signedHeading;

    taskPtr = (BattleFxSpiralEmitter *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    dataAddress = Gfx_GetPsxScratchpadPtr(0);
    for (pairIndex = 0; pairIndex < 2; pairIndex++) {
        if (taskPtr->ownerSlot != 0) {
            signedHeading = taskPtr->angle;
        } else {
            signedHeading = -taskPtr->angle;
        }
        spiralAngle = signedHeading;
        particle = (BattleFxPlanetProtectorSpiralParticle *)BattleFx_LimitPlanetProtectorClaimParticleRecord();
        particle->fadeLevel = pairIndex + 7;
        particle->posX = taskPtr->centerX + (taskPtr->radius * SinFixed(spiralAngle) >> 0xC);
        particle->posY = taskPtr->posY;
        particle->posZ = taskPtr->centerZ + (taskPtr->radius * CosFixed(spiralAngle) >> 0xC);
        particle->scale = taskPtr->scale;
        particle->phase = taskPtr->phase;
        if (taskPtr->radius > 0x1F4) {
            taskPtr->radius = taskPtr->radius * 0xD00 >> 0xC;
        }
        taskPtr->angle += (short)(0x21555 / taskPtr->radius);
        taskPtr->posY += taskPtr->riseSpeed;
        if (taskPtr->riseSpeed > 0x1E) {
            taskPtr->riseSpeed = taskPtr->riseSpeed * 0xE00 >> 0xC;
        }
        taskPtr->phase += 0x100;
    }
    (taskPtr->frameCount)++;
    if (taskPtr->frameCount >= 0x14) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Allocates an available particle record from the 150-entry pool whose lifetime is nonpositive.
 */
// FUNCTION: FF7 0x0045ab65
int __cdecl BattleFx_LimitPlanetProtectorClaimParticleRecord(void)
{
    int i;

    for (i = 0; i < 0x96; i++) {
        if (((YasuiLimitPlanetProtectorParticle *)&g_YasuiLimitPlanetProtectorParticlePool)[i].state <= 0) {
            return (int)&g_YasuiLimitPlanetProtectorParticlePool + i * 0x10;
        }
    }
    return 0;
}

/*
 * Renders individual burst sprite particle with velocity damping and alpha fadeout.
 */
// FUNCTION: FF7 0x0045abaa
void __cdecl BattleFx_LimitPlanetProtectorDampedSpriteParticleTask(void)
{
    int elapsed;
    BattleFxLimitSealEvilDelay *scaleBlock;
    BattleFxRotatingSpritePair *pRotPair;
    BattleFxPlanetProtectorDampedSprite *particle;
    int padLocal; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    Vec3sPacked *matrix;

    particle = (BattleFxPlanetProtectorDampedSprite *)BATTLE_CURRENT_TASK_SLOT;
    (g_YasuiLimitPlanetProtectorSpriteDrawReq2.animBlock) = Gfx_ModelTableGetLoadedRecord(0, g_YasuiLimitPlanetProtectorResourceContext);
    g_YasuiLimitPlanetProtectorSpriteDrawReq2.battleLimitSGuardDampedSpriteResource = Gfx_ModelTableGetSpriteObject(0, g_YasuiLimitPlanetProtectorResourceContext);
    pRotPair = (BattleFxRotatingSpritePair *)Gfx_GetPsxScratchpadPtr(0);
    matrix = (Vec3sPacked *)((int)pRotPair + 8);
    scaleBlock = (BattleFxLimitSealEvilDelay *)((int)matrix + 0x1E);
    elapsed = particle->age;
    if (elapsed < particle->delay) {
        g_YasuiLimitPlanetProtectorSpriteDrawReq2.battleLimitSGuardDampedColorWord = 0x2E808080;
    } else {
        elapsed -= particle->delay;
        if (elapsed < 5) {
            g_BattleLimitPlanetProtectorDampedColorChannel2 = elapsed * -0x80 / 5 + 0x80;
            g_BattleLimitPlanetProtectorDampedColorChannel1 = g_BattleLimitPlanetProtectorDampedColorChannel2;
            *(char *)&g_YasuiLimitPlanetProtectorSpriteDrawReq2.battleLimitSGuardDampedColorWord = g_BattleLimitPlanetProtectorDampedColorChannel1;
        } else {
            particle->delay = 0xFFFF;
            return;
        }
    }
    BattleFx_MakeViewSpacePointPushed((void *)((int)particle + 4), 0, matrix);
    pRotPair->rotY = 0;
    pRotPair->rotX = 0;
    pRotPair->angle = particle->angle;
    RotMatrix((SVECTOR *)pRotPair, matrix);
    scaleBlock->nextValue = particle->nextValue;
    scaleBlock->value = scaleBlock->nextValue;
    scaleBlock->prevValue = scaleBlock->value;
    ScaleMatrix(matrix, scaleBlock);
    SetRotMatrix((void *)matrix);
    SetTransMatrix(matrix);
    g_YasuiLimitPlanetProtectorState[0] = BattleFx_SubmitSpriteDraw(&g_YasuiLimitPlanetProtectorSpriteDrawReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitPlanetProtectorState[0]);
    if (g_BattleIsPaused == 0) {
        (particle->age)++;
        particle->posX += particle->velocityX;
        particle->posY += particle->velocityY;
        particle->posZ += particle->velocityZ;
        particle->velocityX = particle->velocityX * 0xE00 >> 0xC;
        particle->velocityY = particle->velocityY * 0xE00 >> 0xC;
        particle->velocityZ = particle->velocityZ * 0xE00 >> 0xC;
    }
    particle->angle += 0x100;
}
} /* extern "C" */
