// src/ff7/src/battle/battle_fx_special_whirlpool.cpp
/* Enemy Special Attack [103]: Whirlpool (orig. SPECIAL/UZUMAKI, 渦巻き) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Uzumaki attack: loads resources, spawns per-target
//   timelines, and drives a whirlpool of spiral and secondary particles along with a growing
//   whirlpool model.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
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
struct BattleFxSpecialWhirlpoolSpiralTrail;

#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// GLOBAL: FF7 0x008b4098
int g_BattleSpecWhirlpoolResourceDefs = 0x001900B1;
int g_BattleSpecWhirlpoolResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialWhirlpoolWhirlpoolResourceNames + 0x0)
};
int g_BattleSpecWhirlpoolResourceDefsDataTail[14] = {
    0x0, 0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleFxSpecialWhirlpoolWhirlpoolResourceNames + 0x10), 0x0,
    0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialWhirlpoolWhirlpoolResourceNames + 0x20), (int)((char *)&g_BattleFxSpecialWhirlpoolWhirlpoolResourceNames + 0x30), 0x0,
    0x0, 0x0
};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008b40d8
unsigned char g_BattleSpecWhirlpoolConfig[8] = {0xe8, 0xff, 0xe8, 0xff, 0x00, 0x00, 0x2f, 0x2f};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008b40e0
struct BattleSpecWhirlpoolColor g_BattleSpecWhirlpoolColor = {-128, -128, -128, {0x2e}};
unsigned char g_rawData_008b40e4[4] = {0x2c, 0x00, 0x08, 0x78};
// GLOBAL: FF7 0x008b40e8
struct BattleSpecWhirlpoolParticleConfig g_BattleSpecWhirlpoolParticleConfig = {0, {0}, 0, {8, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}};
unsigned char g_BattleFxSpecialWhirlpoolWhirlpoolResourceNames[60] =
    "TORNADO1.RSD\0\0\0\0"
    "TORNADO2.RSD\0\0\0\0"
    "TORNAD00.TIM\0\0\0\0"
    "LINE.SPT";
// GLOBAL: FF7 0x00bb8cd0
BattleFxSpecialWhirlpoolSpiralTrail g_BattleSpecWhirlpoolTargetOffsets[100] = {0};
// GLOBAL: FF7 0x00bbaa80
BattleTaskSlot *g_BattleSpecWhirlpoolTrailArrayTask = 0;
// GLOBAL: FF7 0x00bbaa84
int g_BattleSpecWhirlpoolDrawListHandle = 0;
unsigned char g_bssPad_00bbaa88[4] = {0};
// GLOBAL: FF7 0x00bbaa8c
BattleFxWhirlpoolGrowingTask *g_BattleSpecWhirlpoolGrowingModelTask = 0;
// GLOBAL: FF7 0x00bbaa90
BattleTaskSlot *g_BattleSpecWhirlpoolSecondaryArrayTask = 0;
// GLOBAL: FF7 0x00bbaa94
int g_BattleSpecWhirlpoolFadeScale = 0;
// GLOBAL: FF7 0x00bbaa98
BattleFxSpecialWhirlpoolSpiralTrail g_BattleSpecWhirlpoolParticleRecords[100] = {0};
// GLOBAL: FF7 0x00bbc848
SVECTOR g_BattleSpecWhirlpoolCenterPos = {0};
// GLOBAL: FF7 0x00bbc850
struct BattleSpecWhirlpoolSecondaryRecords g_BattleSpecWhirlpoolSecondaryRecords = {0};
unsigned char g_bssPad_00bbc85c[4] = {0};
// clang-format on


/* The first of the 100 spiral trail records is g_BattleSpecWhirlpoolTargetOffsets; the zero-filled pad
 * globals that follow it hold the other 99. */
#define WHIRLPOOL_SPIRAL_TRAILS ((BattleFxSpecialWhirlpoolSpiralTrail *)&g_BattleSpecWhirlpoolTargetOffsets)

/*
 * Loads resources for the Uzumaki special effect.
 */
// FUNCTION: FF7 0x0058adb0
void __cdecl BattleFx_SpecialLoadResWhirlpool(void)
{
    GfxModelTableBank *sceneContext;
    GfxResourceRequest record;
    int id;
    char pathBuffer[200];
    GfxResourceRequest *request;
    int defPtr;

    if (g_pBattle3dScene != 0 && (g_BattleSpecWhirlpoolSecondaryRecords.tableCursor) == 0) {
        id = 3;
        defPtr = (int)&g_BattleSpecWhirlpoolResourceDefs;
        sceneContext = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &record;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x008b413c
        request->resourcePath = BattleFx_BuildResourcePath("SPECIAL/UZUMAKI", pathBuffer);
        Gfx_LoadModelTableIfAbsent(id, defPtr, request, &g_BattleSpecWhirlpoolSecondaryRecords, sceneContext);
    }
}

/*
 * Entry point for the Uzumaki special effect.
 */
// FUNCTION: FF7 0x0058ae40
void __cdecl BattleFx_SpecialEnterWhirlpool(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResWhirlpool();
    g_BattleSpecWhirlpoolDrawListHandle = (int)&g_GfxPsxPacketScratch;
    BattleFx_SpecialRegisterWhirlpoolPerTargetCallback(targetMask, actorIdx);
}

/*
 * Registers bit-pattern callback for Uzumaki targets.
 */
// FUNCTION: FF7 0x0058ae64
void __cdecl BattleFx_SpecialRegisterWhirlpoolPerTargetCallback(int actorIndex, int secondIndex)
{
    Battle_CreateBitPatternCallbackTask(actorIndex, secondIndex, 0x5A, BattleFx_SpecialSpawnWhirlpoolPerTarget);
}

/*
 * Spawns timeline task for a single Uzumaki target.
 */
// FUNCTION: FF7 0x0058ae80
void __cdecl BattleFx_SpecialSpawnWhirlpoolPerTarget(int actor, int secondIndex)
{
    int i;
    BattleFxWhirlpoolTimeline *task;

    for (i = 0; i < 0x64; i++) {
        WHIRLPOOL_SPIRAL_TRAILS[i].step = 0xFFFF;
    }
    for (i = 0; i < 0x64; i++) {
        g_BattleSpecWhirlpoolParticleRecords[i].step = 0xFFFF;
    }
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &g_BattleSpecWhirlpoolCenterPos.vx);
    g_BattleSpecWhirlpoolCenterPos.vy = 0;
    g_BattleSpecWhirlpoolFadeScale = 0x1000;
    task = (BattleFxWhirlpoolTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWhirlpoolTimelineTask);
    task->actorIndex = actor;
    task->secondIndex = secondIndex;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleSpecWhirlpoolCenterPos), 0x4E);
}

/*
 * Main timeline task controlling whirlpool spiral trails and model for Uzumaki.
 */
// FUNCTION: FF7 0x0058af5d
void __cdecl BattleFx_SpecialWhirlpoolTimelineTask(void)
{
    int stage;
    BattleFxSpecialWhirlpoolSpiralTrail *particle;
    BattleFxWhirlpoolTimelineSlot *task;
    BattleFxSpecialWhirlpoolSpiralTrail *pTrail;

    task = (BattleFxWhirlpoolTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->timelineStep == 4) {
        g_BattleSpecWhirlpoolTrailArrayTask = BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWhirlpoolSpiralTrailParticleArrayTask);
        g_BattleSpecWhirlpoolTrailArrayTask->ownerSlot = 0;
        g_BattleSpecWhirlpoolSecondaryArrayTask = BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWhirlpoolSecondaryParticleArrayTask);
        g_BattleSpecWhirlpoolSecondaryArrayTask->ownerSlot = 0;
        g_BattleSpecWhirlpoolGrowingModelTask = (BattleFxWhirlpoolGrowingTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWhirlpoolGrowingModelTask);
        *(Int64Pair *)&g_BattleSpecWhirlpoolGrowingModelTask->posX = *(Int64Pair *)&g_BattleSpecWhirlpoolCenterPos;
    }
    if (task->timelineStep == 0x22) {
        Battle_PostActorHitOrHurtMessage(task->recordType);
    }
    if (task->timelineStep == 0) {
        Battle_StartFixedPointRamp(0x400, 4);
        g_BattleFxTimelineState.red = 0;
        g_BattleFxTimelineState.channel1 = 0;
        g_BattleFxTimelineState.channel2 = 0;
    }
    stage = task->timelineStep - 4;
    if (stage >= 0) {
        if (stage < 0x1E) {
            if (stage % 5 == 0) {
                pTrail = (BattleFxSpecialWhirlpoolSpiralTrail *)BattleFx_SpecialWhirlpoolClaimSecondaryParticleRecord();
                if (pTrail != 0) {
                    pTrail->radius = rand() % 0x3E8 + 0x1388;
                    pTrail->height = 0;
                    pTrail->angle = rand() & 0xFFF;
                }
            }
            if (stage % 1 == 0) {
                particle = (BattleFxSpecialWhirlpoolSpiralTrail *)BattleFx_SpecialWhirlpoolClaimSpiralTrailParticleRecord();
                if (particle != 0) {
                    particle->radius = rand() % 0x3E8 + 0x1388;
                    particle->height = 0;
                    particle->angle = rand() & 0xFFF;
                    particle->step = 0;
                }
            }
        } else {
            stage -= 0x1E;
            if (stage < 0xF) {
                g_BattleSpecWhirlpoolFadeScale = stage * -0x1000 / 0xF + 0x1000;
            } else {
                g_BattleSpecWhirlpoolTrailArrayTask->ownerSlot = BATTLE_TASK_FREE;
                g_BattleSpecWhirlpoolSecondaryArrayTask->ownerSlot = BATTLE_TASK_FREE;
            }
        }
    }
    if (task->timelineStep == 0x31) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->timelineStep)++;
}

/*
 * Allocates an available spiral trail particle record slot for Uzumaki.
 */
// FUNCTION: FF7 0x0058b1a5
int __cdecl BattleFx_SpecialWhirlpoolClaimSpiralTrailParticleRecord(void)
{
    int i;

    for (i = 0; i < 0x64; i++) {
        if (WHIRLPOOL_SPIRAL_TRAILS[i].step == -1) {
            WHIRLPOOL_SPIRAL_TRAILS[i].step = 0;
            WHIRLPOOL_SPIRAL_TRAILS[i].index = 0;
            WHIRLPOOL_SPIRAL_TRAILS[i].outIndex = 0;
            return (int)&WHIRLPOOL_SPIRAL_TRAILS[i];
        }
    }
    return 0;
}

/*
 * Allocates an available secondary particle record slot for Uzumaki.
 */
// FUNCTION: FF7 0x0058b215
int __cdecl BattleFx_SpecialWhirlpoolClaimSecondaryParticleRecord(void)
{
    int i;

    for (i = 0; i < 0x64; i++) {
        if (g_BattleSpecWhirlpoolParticleRecords[i].step == -1) {
            g_BattleSpecWhirlpoolParticleRecords[i].step = 0;
            g_BattleSpecWhirlpoolParticleRecords[i].index = 0;
            g_BattleSpecWhirlpoolParticleRecords[i].outIndex = 0;
            return (int)&g_BattleSpecWhirlpoolParticleRecords[i];
        }
    }
    return 0;
}

/*
 * Updates and renders active spiral trail particles for Uzumaki.
 */
// FUNCTION: FF7 0x0058b285
void __cdecl BattleFx_SpecialWhirlpoolSpiralTrailParticleArrayTask(void)
{
    BattleTaskSlot *task;
    int particleIndex;
    int liveCount;

    task = BATTLE_CURRENT_TASK_SLOT;
    liveCount = 0;
    for (particleIndex = 0; particleIndex < 0x64; particleIndex++) {
        if (WHIRLPOOL_SPIRAL_TRAILS[particleIndex].step != -1) {
            BattleFx_SpecialWhirlpoolUpdateSpiralTrailParticle(&WHIRLPOOL_SPIRAL_TRAILS[particleIndex]);
            liveCount++;
        }
    }
    if (task->ownerSlot == 1 && liveCount == 0) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates physics and renders spiral particle along whirlpool path for Uzumaki.
 */
// FUNCTION: FF7 0x0058b30b
void __cdecl BattleFx_SpecialWhirlpoolUpdateSpiralTrailParticle(BattleFxSpecialWhirlpoolSpiralTrail *trail)
{
    int idx;
    int numberSegments;
    Vec3sPacked *scratch;
    int cursor;
    BattleFxVortexStripAngle *rotVec;
    PsxMatrix *rotMtx;
    int limit;
    int delta;

    scratch = (Vec3sPacked *)Gfx_GetPsxScratchpadPtr(0);
    rotVec = (BattleFxVortexStripAngle *)((int)scratch + 8);
    rotMtx = (PsxMatrix *)((int)rotVec + 8);
    (g_BattleSpecWhirlpoolParticleConfig.resField) = Gfx_ModelTableGetSpriteObject(2, (GfxTableCursor *)(g_BattleSpecWhirlpoolSecondaryRecords.tableCursor));
    if (g_BattleIsPaused == 0) {
        for (idx = 0; idx < 3; idx++) {
            trail->angle += (short)(0x25000 / trail->radius);
            trail->height -= (short)(0x3A98 / (trail->radius - 0x12C));
            if (trail->height >> 3 > -0x6E) {
                delta = trail->height >> 4;
            } else {
                delta = 0xFFFFFF92;
            }
            trail->radius -= (short)(delta + 0x64);
            if (trail->radius < 0x190) {
                trail->radius = 0x190;
            }
            scratch->vx = trail->radius;
            scratch->vy = trail->height;
            scratch->vz = 0;
            rotVec->angleX = 0;
            rotVec->angleZ = 0;
            rotVec->angle = trail->angle;
            RotMatrix((SVECTOR *)rotVec, rotMtx);
            TransformVectorToInt16(rotMtx, (SVECTOR *)scratch, (short *)scratch);
            trail->points[trail->outIndex].x = scratch->vx + g_BattleSpecWhirlpoolCenterPos.vx;
            trail->points[trail->outIndex].y = scratch->vy;
            trail->points[trail->outIndex].z = scratch->vz + g_BattleSpecWhirlpoolCenterPos.vz;
            trail->outIndex = trail->outIndex + 1 & 7;
        }
    }
    if (trail->index < 5) {
        limit = trail->index;
    } else {
        limit = 5;
    }
    numberSegments = limit;
    cursor = trail->outIndex - numberSegments & 7;
    for (idx = 1; idx < numberSegments; idx++) {
        BattleFx_InstallBillboardMatrix(&trail->points[cursor].x, 0x1000, 0);
        g_BattleSpecWhirlpoolColor.blue = (idx << 7) / 5 * g_BattleSpecWhirlpoolFadeScale >> 0xC;
        g_BattleSpecWhirlpoolColor.green = g_BattleSpecWhirlpoolColor.blue;
        (g_BattleSpecWhirlpoolColor.red) = g_BattleSpecWhirlpoolColor.green;
        g_BattleSpecWhirlpoolDrawListHandle = BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)&g_BattleSpecWhirlpoolConfig, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecWhirlpoolDrawListHandle);
        cursor = cursor + 1 & 7;
    }
    if (g_BattleIsPaused == 0) {
        (trail->index)++;
        if (trail->index > 0x32) {
            trail->step = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Updates and renders active secondary particles for Uzumaki.
 */
// FUNCTION: FF7 0x0058b5e9
void __cdecl BattleFx_SpecialWhirlpoolSecondaryParticleArrayTask(void)
{
    BattleTaskSlot *task;
    int particleIndex;
    int liveCount;

    task = BATTLE_CURRENT_TASK_SLOT;
    liveCount = 0;
    for (particleIndex = 0; particleIndex < 0x64; particleIndex++) {
        if (g_BattleSpecWhirlpoolParticleRecords[particleIndex].step != -1) {
            BattleFx_SpecialWhirlpoolUpdateSecondaryParticle(&g_BattleSpecWhirlpoolParticleRecords[particleIndex]);
            liveCount++;
        }
    }
    if (task->ownerSlot == 1 && liveCount == 0) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused == 0) {
        g_BattleSpecWhirlpoolSecondaryRecords.counter++;
    }
}

/*
 * Updates physics and renders secondary particle for Uzumaki.
 */
// FUNCTION: FF7 0x0058b689
void __cdecl BattleFx_SpecialWhirlpoolUpdateSecondaryParticle(BattleFxSpecialWhirlpoolSpiralTrail *particle)
{
    Vec3sPacked *quadA;
    BattleFxShardQuad *quadBuf;
    Vec3sPacked *pScratchPoint;
    int clipFlags;
    int segments;
    int zDepth;
    int curIndex;
    int drawCnt;
    int decay;
    BattleFxVortexStripAngle *rotVector;
    PsxMatrix *mtxBlock;
    int ignored;
    Vec3sPacked *vertA;
    int color;
    Vec3sPacked *pVertexB;
    BattleFxGouraudQuadPacket *buffer;
    int index;
    int nextSeq;

    color = 0x3A000000;
    pScratchPoint = (Vec3sPacked *)Gfx_GetPsxScratchpadPtr(0);
    rotVector = (BattleFxVortexStripAngle *)((int)pScratchPoint + 8);
    quadA = (Vec3sPacked *)((int)rotVector + 8);
    vertA = (Vec3sPacked *)((int)quadA + 8);
    quadBuf = (BattleFxShardQuad *)((int)vertA + 8);
    pVertexB = (Vec3sPacked *)((int)quadBuf + 8);
    mtxBlock = (PsxMatrix *)((int)pVertexB + 8);
    if (g_BattleIsPaused == 0) {
        for (index = 0; index < 3; index++) {
            particle->angle += (short)(0x25000 / particle->radius);
            particle->height -= (short)(0x3A98 / (particle->radius - 0x12C));
            if (particle->height >> 3 > -0x6E) {
                decay = particle->height >> 4;
            } else {
                decay = 0xFFFFFF92;
            }
            particle->radius -= (short)(decay + 0x64);
            if (particle->radius < 0x190) {
                particle->radius = 0x190;
            }
            pScratchPoint->vx = particle->radius;
            pScratchPoint->vy = particle->height;
            pScratchPoint->vz = 0;
            rotVector->angleX = 0;
            rotVector->angleZ = 0;
            rotVector->angle = particle->angle;
            RotMatrix((SVECTOR *)rotVector, mtxBlock);
            TransformVectorToInt16(mtxBlock, (SVECTOR *)pScratchPoint, (short *)pScratchPoint);
            particle->points[particle->outIndex].x = pScratchPoint->vx + g_BattleSpecWhirlpoolCenterPos.vx;
            particle->points[particle->outIndex].y = pScratchPoint->vy;
            particle->points[particle->outIndex].z = pScratchPoint->vz + g_BattleSpecWhirlpoolCenterPos.vz;
            particle->outIndex = particle->outIndex + 1 & 7;
        }
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    if (particle->index < 8) {
        drawCnt = particle->index;
    } else {
        drawCnt = 8;
    }
    segments = drawCnt;
    curIndex = particle->outIndex - segments & 7;
    for (index = 1; index < segments; index++) {
        buffer = (BattleFxGouraudQuadPacket *)g_BattleSpecWhirlpoolDrawListHandle;
        vertA->vx = particle->points[curIndex].x;
        quadA->vx = vertA->vx;
        quadA->vy = particle->points[curIndex].y - 0x19;
        vertA->vy = particle->points[curIndex].y + 0x19;
        vertA->vz = particle->points[curIndex].z;
        quadA->vz = vertA->vz;
        nextSeq = curIndex + 1 & 7;
        pVertexB->vx = particle->points[nextSeq].x;
        quadBuf->corner0X = pVertexB->vx;
        quadBuf->corner0Y = particle->points[nextSeq].y - 0x19;
        pVertexB->vy = particle->points[nextSeq].y + 0x19;
        pVertexB->vz = particle->points[nextSeq].z;
        quadBuf->depth = pVertexB->vz;
        zDepth = Gfx_TransformQuadReturnDepth(quadA, (SVECTOR *)vertA, (SVECTOR *)quadBuf, (SVECTOR *)pVertexB, &ignored, &clipFlags);
        if ((clipFlags & 0x60000) == 0) {
            buffer->tag = 0x8000000;
            buffer->color1 = color;
            buffer->color0 = color;
            if (index == segments - 1) {
                color = 0x3A000000;
            } else {
                color += g_BattleSpecWhirlpoolFadeScale << 4 >> 0xC << 0x10 | g_BattleSpecWhirlpoolFadeScale << 4 >> 0xC << 8 | g_BattleSpecWhirlpoolFadeScale << 4 >> 0xC;
            }
            buffer->color3 = color;
            buffer->color2 = color;
            Gfx_DrawPsxPrimitive(buffer);
        }
        curIndex = nextSeq;
    }
    if (g_BattleIsPaused == 0) {
        (particle->index)++;
        if (particle->index > 0x32) {
            particle->step = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Updates and renders growing whirlpool 3D model for Uzumaki.
 */
// FUNCTION: FF7 0x0058baba
void __cdecl BattleFx_SpecialWhirlpoolGrowingModelTask(void)
{
    BattleFxWhirlpoolScaleState *pScaleState;
    int stage;
    BattleFxWhirlpoolGrowingTask *task;
    PsxMatrix *matrix;

    task = (BattleFxWhirlpoolGrowingTask *)BATTLE_CURRENT_TASK_SLOT;
    matrix = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    pScaleState = (BattleFxWhirlpoolScaleState *)((char *)matrix + 0x1E);
    stage = task->step;
    if (stage < 0xF) {
        g_BattleSpecWhirlpoolParticleConfig.scale = (stage << 7) / 0xF;
        pScaleState->scaleTo = stage * 0x1500 / 0xF;
        pScaleState->scaleFrom = pScaleState->scaleTo;
        pScaleState->scale = stage * 0 / 0xF + 0x1500;
    } else {
        stage -= 0xF;
        if (stage < 0) {
            g_BattleSpecWhirlpoolParticleConfig.scale = 0x80;
            pScaleState->scaleTo = 0x1500;
            pScaleState->scaleFrom = 0x1500;
            pScaleState->scale = 0x1500;
        } else {
            stage = stage;
            if (stage < 0x1E) {
                g_BattleSpecWhirlpoolParticleConfig.scale = stage * -0x80 / 0x1E + 0x80;
                pScaleState->scaleTo = (stage << 0xC) / 0x1E + 0x1500;
                pScaleState->scaleFrom = pScaleState->scaleTo;
                pScaleState->scale = (stage << 0xC) / 0x1E + 0x1500;
            } else {
                task->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    RotMatrix((SVECTOR *)&g_BattleSpecWhirlpoolSecondaryRecords.rotByte, matrix);
    ScaleMatrix(matrix, pScaleState);
    matrix->t[0] = task->posX;
    matrix->t[1] = 0;
    matrix->t[2] = task->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, matrix, matrix);
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    g_BattleSpecWhirlpoolParticleConfig.modelField = Gfx_ModelTableGetRenderObject(0, (GfxTableCursor *)(g_BattleSpecWhirlpoolSecondaryRecords.tableCursor));
    g_BattleSpecWhirlpoolDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecWhirlpoolParticleConfig.modelField, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecWhirlpoolDrawListHandle);
    g_BattleSpecWhirlpoolParticleConfig.modelField = Gfx_ModelTableGetRenderObject(1, (GfxTableCursor *)(g_BattleSpecWhirlpoolSecondaryRecords.tableCursor));
    g_BattleSpecWhirlpoolDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecWhirlpoolParticleConfig.modelField, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecWhirlpoolDrawListHandle);
    if (g_BattleIsPaused == 0) {
        g_BattleSpecWhirlpoolSecondaryRecords.angle -= -0x80;
        (task->step)++;
    }
}
} /* extern "C" */
