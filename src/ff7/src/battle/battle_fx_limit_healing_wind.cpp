// src/ff7/src/battle/battle_fx_limit_healing_wind.cpp
// Limit Break [14] (Aeris Lv 1-1): Healing Wind (Japanese: 癒しの風, dev tag: "LIMIT/IYASHI" / Iyashi / Iyash)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Iyashi party-wide healing effect: sets up the camera and tasks, then drives an aura
//   model, spiral and animated healing sprites, and a pooled particle system with trail ribbons.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_limit.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle_3d.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/codegen.h"
extern "C" {

struct BattleFxLimitHealingWindParticle;
struct BattleFxModelDraw;

#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x008f35b8
int g_BattleLimitHealingWind_ResourceDefs = 0x001900B1;
char *g_BattleFxLimitHealingWindResourceNames[1] = {
    // STRING: FF7 0x008f3634
    "AURA.RSD"
};
unsigned int g_BattleLimitHealingWind_ResourceDefsDataTail[14] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008f3640
    (unsigned int)"IYASHI00.TIM",
    // STRING: FF7 0x008f3650
    (unsigned int)"IYASHI.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008f365c
    (unsigned int)"IYASHI01.TIM",
    // STRING: FF7 0x008f366c
    (unsigned int)"IYASHI2.SPT",
    0x00000000,
    0x00000000,
    0x00000000
};
#undef g_BattleLimitHealingWind_SpriteReq
// GLOBAL: FF7 0x008f35f8
struct BattleLimitHealingWind_SpriteReq g_BattleLimitHealingWind_SpriteReq = {{0, 0, 0, 0, 0x6a, 0xff, 0, 0}, 0, {8, 0, 0, 0, 0, 0, 0, 0, 0x20, 0, 0, 0}, 0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
unsigned char g_rawData_008f3620[20] = {0xf0, 0xff, 0xe0, 0xff, 0x00, 0xa0, 0x1f, 0x1f, 0x80, 0x80, 0x80, 0x2e, 0x3a, 0x00, 0x07, 0x7a, 0x00, 0x00, 0x00, 0x00};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00bda578
int g_BattleLimitHealingWind_LoadedRes = 0;
// GLOBAL: FF7 0x00bda57c
int g_BattleLimitHealingWind_DrawListHandle = 0;
// GLOBAL: FF7 0x00bda580
int g_BattleLimitHealingWind_ActorCenterZ = 0;
// GLOBAL: FF7 0x00bda584
int g_BattleLimitHealingWind_ActorHeight = 0;
#undef g_BattleLimitHealingWind_ParticleRecords
// GLOBAL: FF7 0x00bda588
struct BattleFxHealingWindParticleRecord g_BattleLimitHealingWind_ParticleRecords[100] = {0};
// GLOBAL: FF7 0x00bdc978
struct BattleLimitHealingWind_TaskHandle g_BattleLimitHealingWind_TaskHandle = {0};
unsigned char g_bssPad_00bdc98c[4] = {0};
// clang-format on
#define g_BattleLimitHealingWindTaskRotVector (((short(*)[3])((char *)&g_BattleLimitHealingWind_TaskHandle + 0x8))[0])

/*
 * Loads resources for the Iyashi limit effect when not already loaded.
 */
// FUNCTION: FF7 0x005af350
void __cdecl BattleFx_LimitLoadResHealingWind(void)
{
    GfxModelTableBank *context;
    GfxResourceRequest requestRecord;
    int resultType;
    char pathState[200];
    GfxResourceRequest *request;
    int descriptor;

    if (g_pBattle3dScene != 0 && g_BattleLimitHealingWind_TaskHandle.cursor == 0) {
        resultType = 3;
        descriptor = (int)&g_BattleLimitHealingWind_ResourceDefs;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestRecord;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x008f3678
        request->resourcePath = BattleFx_BuildResourcePath("LIMIT/IYASHI", pathState);
        Gfx_LoadModelTableIfAbsent(resultType, descriptor, request, &g_BattleLimitHealingWind_TaskHandle.cursor, context);
    }
}

/*
 * Entry point spawning the party-wide Iyashi effect.
 */
// FUNCTION: FF7 0x005af3e0
int __cdecl Battle_LimitFx14_HealingWind(int targetMask, int actorIndex)
{
    BattleFx_LimitLoadResHealingWind();
    g_BattleLimitHealingWind_DrawListHandle = (int)&g_GfxPsxPacketScratch;
    BattleFx_LimitBeginHealingWind(targetMask, actorIndex);
    return 0;
}

/*
 * Initializes tasks and camera positioning for Iyashi.
 */
// FUNCTION: FF7 0x005af406
void __cdecl BattleFx_LimitBeginHealingWind(int targetMask, int actorIndex)
{
    int idx;
    int task;

    task = (int)BATTLE_NEW_TASK_SLOT(BattleFx_LimitHealingWindTimelineTask);
    g_BattleLimitHealingWind_ActorCenterZ = targetMask;
    g_BattleLimitHealingWind_ActorHeight = actorIndex;
    g_BattleLimitHealingWind_LoadedRes = (int)&g_BattleActorRecords[actorIndex].modelId;
    (g_BattleLimitHealingWind_TaskHandle.posXAddr) = (int)&g_BattleActorRecords[actorIndex].posX;
    for (idx = 0; idx < 100; idx++) {
        g_BattleLimitHealingWind_ParticleRecords[idx].active = 0;
    }
}

/*
 * Drives the main timeline and healing particle emission for Iyashi.
 */
// FUNCTION: FF7 0x005af48e
void __cdecl BattleFx_LimitHealingWindTimelineTask(void)
{
    int padSlot[9]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int timelineStep;
    int range;
    int index;
    int base;
    BattleFxLimitHealingWindParticle *particle;
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int stackHole[8]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxHealingWindTimeline *pSpawn;
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    timelineStep = task->frameCounter;
    if (timelineStep == 0) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[g_BattleLimitHealingWind_ActorHeight].posX), 0x22);
    }
    if (timelineStep == 11) {
        pSpawn = (BattleFxHealingWindTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitHealingWindMidpointModelTask);
    }
    if (timelineStep == 11) {
        Battle_AllocateResourceSlot(BattleFx_LimitHealingWindParticleControllerTask);
    }
    if (timelineStep == 52) {
        for (index = 0; index < 10; index++) {
            if ((g_BattleLimitHealingWind_ActorCenterZ & 1 << index) != 0) {
                pSpawn = (BattleFxHealingWindTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitHealingWindSpawnSpiralSpritesTask);
                pSpawn->index = index;
                pSpawn->radius = g_BattleActorRecords[index].radius;
                pSpawn->offsetY = 0xFC18;
                pSpawn->offsetZ = 0;
                pSpawn->angle = rand() & 0xFFF;
                pSpawn->angleStep = 0x200 /* 45 deg */;
            }
        }
    }
    if (timelineStep < 11) { /* empty first arm: lib/codegen.h */
    } else {
        timelineStep -= 11;
        if (timelineStep < 80 && timelineStep % 4 == 0) {
            particle = (BattleFxLimitHealingWindParticle *)BattleFx_LimitHealingWindClaimParticleRecord();
            if (particle != 0) {
                base = timelineStep * 25 + 0x3E8;
                range = timelineStep * 6 + 500;
                particle->velocityX = rand() % base + base;
                particle->velocityY = -(rand() % range) - range;
                particle->velocityZ = 0;
                particle->angle = rand() & 0xFFF;
                particle->angleStep = (rand() & 0x3F) + 0x80;
                particle->vectorIndex = 0;
                particle->age = 0;
            }
        }
    }
    if (task->frameCounter == 91) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/*
 * Animates and renders glowing aura model at actor midpoint for Iyashi.
 */
// FUNCTION: FF7 0x005af6c5
void __cdecl BattleFx_LimitHealingWindMidpointModelTask(void)
{
    VECTOR sxFirst;
    int projDepth;
    int boneMatrixA[8];
    int billboardScale;
    int age;
    int matrix[8];
    int mtxB[8];
    VECTOR secondX;
    BattleFxHealingWindMidpoint *midpoint;

    midpoint = (BattleFxHealingWindMidpoint *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitHealingWind_SpriteReq.modelDrawRequest = Gfx_ModelTableGetRenderObject(0, (GfxTableCursor *)g_BattleLimitHealingWind_TaskHandle.cursor);
    Battle3d_GetActorAttachMatrix(g_BattleLimitHealingWind_ActorHeight, 12, (PsxMatrix *)boneMatrixA);
    Battle_ComposeMatrixRelativeToCamera((short *)&g_BattleCameraTransform, boneMatrixA, (PsxMatrix *)matrix);
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    RotTransPoint((int *)&g_BattleLimitHealingWind_SpriteReq, &sxFirst.vx, &projDepth);
    Battle3d_GetActorAttachMatrix(g_BattleLimitHealingWind_ActorHeight, 17, (PsxMatrix *)mtxB);
    Battle_ComposeMatrixRelativeToCamera((short *)&g_BattleCameraTransform, mtxB, (PsxMatrix *)matrix);
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    RotTransPoint((int *)&g_BattleLimitHealingWind_SpriteReq, &secondX.vx, &projDepth);
    midpoint->midX = (sxFirst.vx + secondX.vx) / 2;
    midpoint->midY = (sxFirst.vy + secondX.vy) / 2;
    midpoint->midZ = (sxFirst.vz + secondX.vz) / 2;
    age = midpoint->frameIndex;
    if (age < 10) {
        billboardScale = (age << 11) / 10;
    } else {
        age -= 10;
        if (age < 30) {
            billboardScale = 0x800 /* 0.5 */;
        } else {
            age -= 30;
            if (age < 10) {
                billboardScale = age * -0x800 / 10 + 0x800;
            } else {
                midpoint->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    BattleFx_InstallBillboardMatrix((void *)((int)midpoint + 8), (short)billboardScale, 0);
    g_BattleLimitHealingWind_DrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitHealingWind_SpriteReq.modelDrawRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitHealingWind_DrawListHandle);
    if (g_BattleIsPaused == 0) {
        (midpoint->frameIndex)++;
    }
}

/*
 * Spawns spiral ascending particles around target combatants for Iyashi.
 */
// FUNCTION: FF7 0x005af8aa
void __cdecl BattleFx_LimitHealingWindSpawnSpiralSpritesTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[52];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleHealingWindSpiralSpriteRecord *spawnedSprite; };
        struct { unsigned char pad_to_28[0x28]; BattleFxHealingWindSpiralSprite *spiralRecord; };
        struct { unsigned char pad_to_2C[0x2C]; int posX; };
        struct { unsigned char pad_to_2E[0x2E]; short posY; };
        struct { unsigned char pad_to_30[0x30]; int posZ; };
    } frame;
#pragma pack(pop)

    frame.spiralRecord = (BattleFxHealingWindSpiralSprite *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.spiralRecord->step % 1 == 0) {
        g_BattleLimitHealingWindTaskRotVector[1] = frame.spiralRecord->angle;
        RotMatrix((SVECTOR *)&g_BattleLimitHealingWindTaskRotVector, &frame.rotMatrix);
        TransformVectorToInt16(&frame.rotMatrix, (char *)frame.spiralRecord + 8, &frame.posX);
        *(short *)&frame.posX += g_BattleActorRecords[frame.spiralRecord->actorSlot].posX;
        frame.posY += g_BattleActorRecords[frame.spiralRecord->actorSlot].posY;
        *(short *)&frame.posZ += g_BattleActorRecords[frame.spiralRecord->actorSlot].posZ;
        frame.spawnedSprite = (BattleHealingWindSpiralSpriteRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitHealingWindAnimatedSpriteTask);
        *(Int64Pair *)((char *)frame.spawnedSprite + 8) = *(Int64Pair *)(&frame.posX);
        if ((frame.spiralRecord->step & 1) != 0) {
            frame.spawnedSprite->spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, (GfxTableCursor *)g_BattleLimitHealingWind_TaskHandle.cursor);
            frame.spawnedSprite->drawObj = Gfx_ModelTableGetSpriteObject(1, (GfxTableCursor *)g_BattleLimitHealingWind_TaskHandle.cursor);
        } else {
            frame.spawnedSprite->spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(2, (GfxTableCursor *)g_BattleLimitHealingWind_TaskHandle.cursor);
            frame.spawnedSprite->drawObj = Gfx_ModelTableGetSpriteObject(2, (GfxTableCursor *)g_BattleLimitHealingWind_TaskHandle.cursor);
        }
        frame.spawnedSprite->scale = 0x1000 /* 1.0 */;
    }
    frame.spiralRecord->angle = frame.spiralRecord->angle + frame.spiralRecord->angleStep & 0xFFF;
    frame.spiralRecord->offsetY -= -0x21;
    (frame.spiralRecord->step)++;
    if (frame.spiralRecord->step >= 30) {
        Battle_PostActorHitOrHurtMessage(frame.spiralRecord->actorSlot);
        frame.spiralRecord->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates and renders animated healing light sprites for Iyashi.
 */
// FUNCTION: FF7 0x005afa7f
void __cdecl BattleFx_LimitHealingWindAnimatedSpriteTask(void)
{
    BattleFxLimitHealingWindSprite *task;

    task = (BattleFxLimitHealingWindSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitHealingWind_SpriteReq.valueA = task->reqValueA;
    g_BattleLimitHealingWind_SpriteReq.valueB = task->reqValueB;
    g_BattleLimitHealingWind_SpriteReq.frameIndex = task->frameIndex;
    BattleFx_InstallBillboardMatrix((void *)((int)task + 8), (short)task->scale, 0);
    g_BattleLimitHealingWind_DrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleLimitHealingWind_SpriteReq.valueA, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitHealingWind_DrawListHandle);
    if (g_BattleIsPaused == 0 && g_BattleLimitHealingWind_SpriteReq.valueA != 0) {
        (task->frameIndex)++;
        if (task->frameIndex >= ((BattleFxSpriteGroup *)g_BattleLimitHealingWind_SpriteReq.valueA)->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Manages and steps particle pool records for Iyashi.
 */
// FUNCTION: FF7 0x005afb3d
void __cdecl BattleFx_LimitHealingWindParticleControllerTask(void)
{
    int taskSlot;
    int i;
    int activeCnt;

    taskSlot = (int)BATTLE_CURRENT_TASK_SLOT;
    activeCnt = 0;
    for (i = 0; i < 100; i++) {
        if (g_BattleLimitHealingWind_ParticleRecords[i].active != 0) {
            BattleFx_LimitHealingWindStepParticle((BattleFxLimitHealingWindParticle *)(i * 0x5C + (char *)&g_BattleLimitHealingWind_ParticleRecords));
            activeCnt++;
        }
    }
    if (activeCnt == 0) {
        *(short *)taskSlot = 0xFFFF;
    }
}

/*
 * Updates position and velocity for an active Iyashi particle.
 */
// FUNCTION: FF7 0x005afbb7
void __cdecl BattleFx_LimitHealingWindStepParticle(BattleFxLimitHealingWindParticle *particle)
{
    int direction[11];

    if (g_BattleIsPaused == 0) {
        g_BattleLimitHealingWind_TaskHandle.angle = particle->angle;
        RotMatrix((SVECTOR *)&g_BattleLimitHealingWind_TaskHandle.stepRotVectorX, direction);
        TransformVectorToInt16(direction, (char *)particle + 0xC, (char *)particle + particle->vectorIndex * 8 + 0x1C);
        *(short *)((char *)particle + particle->vectorIndex * 8 + 0x1C) += ((SVECTOR *)(g_BattleLimitHealingWind_TaskHandle.posXAddr))->vx;
        *(short *)((char *)particle + particle->vectorIndex * 8 + 0x1E) += ((SVECTOR *)(g_BattleLimitHealingWind_TaskHandle.posXAddr))->vy;
        *(short *)((char *)particle + particle->vectorIndex * 8 + 0x20) += ((SVECTOR *)(g_BattleLimitHealingWind_TaskHandle.posXAddr))->vz;
        particle->vectorIndex = particle->vectorIndex + 1 & 7;
        particle->angle = particle->angle + particle->angleStep & 0xFFF;
        particle->velocityX -= (short)(particle->velocityX >> 2);
        particle->velocityY -= (short)(particle->velocityY >> 3);
        (particle->age)++;
        if (particle->age > 19) {
            particle->active = 0;
            return;
        }
    }
    BattleFx_LimitHealingWindDrawParticleRibbon(particle);
}

/*
 * Renders connected particle trail ribbons for Iyashi.
 */
// FUNCTION: FF7 0x005afd23
void __cdecl BattleFx_LimitHealingWindDrawParticleRibbon(BattleFxLimitHealingWindParticle *particle)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[68];
        int quadDepth;
        struct { unsigned char pad_to_4[0x4]; int ringIndex; };
        struct { unsigned char pad_to_8[0x8]; SVECTOR endX; };
        struct { unsigned char pad_to_10[0x10]; SVECTOR endX2; };
        struct { unsigned char pad_to_18[0x18]; SVECTOR startX; };
        struct { unsigned char pad_to_20[0x20]; int segmentIndex; };
        struct { unsigned char pad_to_24[0x24]; SVECTOR startX2; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxPsxPolygonPrim *ribbonPrim; };
        struct { unsigned char pad_to_30[0x30]; int segmentCount; };
        struct { unsigned char pad_to_34[0x34]; int clipFlags; };
        struct { unsigned char pad_to_38[0x38]; int colorWord; };
        struct { unsigned char pad_to_3C[0x3C]; int widthOffset; };
        struct { unsigned char pad_to_40[0x40]; int quadDepthOut; };
    } frame;
#pragma pack(pop)

    frame.colorWord = 0x3A000000;
    if (particle->age <= 8) {
        frame.segmentCount = particle->age;
    } else if (particle->age <= 12) {
        frame.segmentCount = 8;
    } else if (particle->age <= 20) {
        frame.segmentCount = 20 - particle->age;
    } else {
        return;
    }
    if (frame.segmentCount < 2) {
        return;
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    frame.widthOffset = frame.segmentCount * 20;
    frame.ringIndex = particle->vectorIndex - frame.segmentCount & 7;
    for (frame.segmentIndex = 1; frame.segmentIndex < frame.segmentCount; frame.segmentIndex++) {
        frame.startX.vx = *(short *)((char *)particle + frame.ringIndex * 8 + 0x1C);
        frame.startX2.vx = frame.startX.vx;
        frame.startX2.vy = *(short *)((char *)particle + frame.ringIndex * 8 + 0x1E) - frame.widthOffset;
        frame.startX.vy = *(short *)((char *)particle + frame.ringIndex * 8 + 0x1E) + frame.widthOffset;
        frame.startX.vz = *(short *)((char *)particle + frame.ringIndex * 8 + 0x20);
        frame.startX2.vz = frame.startX.vz;
        frame.ringIndex = frame.ringIndex + 1 & 7;
        frame.widthOffset -= 0x14;
        frame.endX.vx = *(short *)((char *)particle + frame.ringIndex * 8 + 0x1C);
        frame.endX2.vx = frame.endX.vx;
        frame.endX2.vy = *(short *)((char *)particle + frame.ringIndex * 8 + 0x1E) - frame.widthOffset;
        frame.endX.vy = *(short *)((char *)particle + frame.ringIndex * 8 + 0x1E) + frame.widthOffset;
        frame.endX.vz = *(short *)((char *)particle + frame.ringIndex * 8 + 0x20);
        frame.endX2.vz = frame.endX.vz;
        frame.ribbonPrim = (BattleFxPsxPolygonPrim *)g_BattleLimitHealingWind_DrawListHandle;
        frame.quadDepth = Gfx_TransformQuadReturnDepth(&frame.startX2, &frame.startX, &frame.endX2, &frame.endX, &frame.quadDepthOut, &frame.clipFlags);
        if (frame.quadDepth > 0) {
            frame.ribbonPrim->tag = 0x8000000;
            frame.ribbonPrim->color1 = frame.colorWord;
            frame.ribbonPrim->colorCmd = frame.colorWord;
            frame.colorWord += 0x101010;
            frame.ribbonPrim->color3 = frame.colorWord;
            frame.ribbonPrim->color2 = frame.colorWord;
            Gfx_DrawPsxPrimitive(frame.ribbonPrim);
        }
    }
}

/*
 * Allocates an available particle record slot for Iyashi.
 */
// FUNCTION: FF7 0x005aff1b
int __cdecl BattleFx_LimitHealingWindClaimParticleRecord(void)
{
    int i;

    for (i = 0; i < 100; i++) {
        if (g_BattleLimitHealingWind_ParticleRecords[i].active == 0) {
            g_BattleLimitHealingWind_ParticleRecords[i].active = 1;
            return i * 0x5C + (int)&g_BattleLimitHealingWind_ParticleRecords;
        }
    }
    return 0;
}
} /* extern "C" */
