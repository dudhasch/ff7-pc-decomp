// src/ff7/src/battle/battle_fx_magic_tornado.cpp
/* Magic Effect [49]: Tornado (Japanese: トルネド, dev tag: "MAGIC/TORNADO") */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle visual effect for the Tornado magic: spiral and strip particle pools stepped and
//   drawn in a funnel, plus twin funnel models for each target.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle_magic.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
struct BattleFxTornadoParticleRecord;

#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// GLOBAL: FF7 0x008f3010
int g_BattleMagicTornado_ResourceDefs = 0x001900B1;
int g_BattleMagicTornado_ResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxMagicTornadoResourceNames + 0x0)
};
int g_BattleMagicTornado_ResourceDefsDataTail[14] = {
    0x0, 0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleFxMagicTornadoResourceNames + 0x10), 0x0,
    0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxMagicTornadoResourceNames + 0x20), (int)((char *)&g_BattleFxMagicTornadoResourceNames + 0x30), 0x0,
    0x0, 0x0
};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008f3050
unsigned char g_BattleMagicTornado_QuadReq[8] = {0xe8, 0xff, 0xe8, 0xff, 0x00, 0x00, 0x2f, 0x2f};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008f3058
struct BattleMagicTornado_QuadData g_BattleMagicTornado_QuadData = {-128, -128, -128, {0x2e}};
unsigned char g_rawData_008f305c[4] = {0x2c, 0x00, 0x08, 0x78};
// GLOBAL: FF7 0x008f3060
struct BattleMagicTornado_ModelReq g_BattleMagicTornado_ModelReq = {0, {0}, 0, {8, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}};
unsigned char g_BattleFxMagicTornadoResourceNames[60] =
    "TORNADO1.RSD\0\0\0\0"
    "TORNADO2.RSD\0\0\0\0"
    "TORNAD00.TIM\0\0\0\0"
    "LINE.SPT";
// GLOBAL: FF7 0x00bd5e68
struct BattleFxTornadoParticleRecord g_BattleMagicTornado_SpiralParticleRecords[100] = {0};
// GLOBAL: FF7 0x00bd7c18
BattleFxTornadoControllerTask *g_BattleMagicTornado_SpiralTaskHandle = 0;
// GLOBAL: FF7 0x00bd7c1c
int g_BattleMagicTornado_DrawListHandle = 0;
unsigned char g_bssPad_00bd7c20[4] = {0};
// GLOBAL: FF7 0x00bd7c24
BattleTornadoModelPairRecord *g_BattleMagicTornado_ModelPairTaskHandle = 0;
// GLOBAL: FF7 0x00bd7c28
BattleFxTornadoControllerTask *g_BattleMagicTornado_StripTaskHandle = 0;
// GLOBAL: FF7 0x00bd7c2c
int g_BattleMagicTornado_AlphaFactor = 0;
// GLOBAL: FF7 0x00bd7c30
struct BattleFxTornadoParticleRecord g_BattleMagicTornado_StripParticleRecords[100] = {0};
// GLOBAL: FF7 0x00bd99e0
SVECTOR g_BattleMagicTornado_TargetPosition = {0};
// GLOBAL: FF7 0x00bd99e8
struct BattleMagicTornado_LoadedRes g_BattleMagicTornado_LoadedRes = {0};
unsigned char g_bssPad_00bd99f4[4] = {0};
// clang-format on


/*
 * Loads resources for the Tornado magic effect when not already loaded.
 */
// FUNCTION: FF7 0x005ab420
void __cdecl BattleFx_MagicLoadResTornado(void)
{
    GfxModelTableBank *context;
    GfxResourceRequest requestBuffer;
    int resultType;
    char pathBuffer[200];
    GfxResourceRequest *pReq;
    int resTornadoDefs;

    if (g_pBattle3dScene != 0 && g_BattleMagicTornado_LoadedRes.tableCursor == 0) {
        resultType = 3;
        resTornadoDefs = (int)&g_BattleMagicTornado_ResourceDefs;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestBuffer;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x008f30b4
        pReq->resourcePath = BattleFx_BuildResourcePath("MAGIC/TORNADO", pathBuffer);
        Gfx_LoadModelTableIfAbsent(resultType, resTornadoDefs, pReq, &g_BattleMagicTornado_LoadedRes, context);
    }
}

/*
 * Main entry point initializing tasks and spawning per-target effects for Tornado.
 */
// FUNCTION: FF7 0x005ab4b0
void __cdecl BattleFx_MagicEnterTornado(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResTornado();
    g_BattleMagicTornado_DrawListHandle = (int)&g_GfxPsxPacketScratch;
    BattleFx_MagicRegisterTornadoPerTargetCallback(targetMask, actorIdx);
}

/*
 * Registers per-target callback for Tornado.
 */
// FUNCTION: FF7 0x005ab4d4
void __cdecl BattleFx_MagicRegisterTornadoPerTargetCallback(int targetMask, int param)
{
    Battle_CreateBitPatternCallbackTask(targetMask, param, 0x5A, BattleFx_MagicSpawnTornadoPerTarget);
}

/*
 * Spawns timeline task for a single Tornado target.
 */
// FUNCTION: FF7 0x005ab4f0
void __cdecl BattleFx_MagicSpawnTornadoPerTarget(int actor, int param)
{
    int idx;
    BattleFxOrbitCloudTask *effect;

    for (idx = 0; idx < 0x64; idx++) {
        g_BattleMagicTornado_SpiralParticleRecords[idx].claimState = 0xFFFF;
    }
    for (idx = 0; idx < 0x64; idx++) {
        g_BattleMagicTornado_StripParticleRecords[idx].claimState = 0xFFFF;
    }
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)&g_BattleMagicTornado_TargetPosition);
    g_BattleMagicTornado_TargetPosition.vy = 0;
    g_BattleMagicTornado_AlphaFactor = 0x1000;
    effect = (BattleFxOrbitCloudTask *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicTornadoTimelineTask);
    effect->targetIndex = (short)actor;
    effect->param = (short)param;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleMagicTornado_TargetPosition), 0x4E);
}

/*
 * Drives the main timeline and particle emission for Tornado.
 */
// FUNCTION: FF7 0x005ab5cd
void __cdecl BattleFx_MagicTornadoTimelineTask(void)
{
    BattleFxTornadoParticleRecord *spiral;
    int padBlock; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int relStep;
    int stackWords[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxSwirlColumnTask *task;
    BattleFxTornadoParticleRecord *strip;
    int padField; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    task = (BattleFxSwirlColumnTask *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->step == 4) {
        g_BattleMagicTornado_SpiralTaskHandle = (BattleFxTornadoControllerTask *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicTornadoSpiralParticleControllerTask);
        g_BattleMagicTornado_SpiralTaskHandle->state = 0;
        g_BattleMagicTornado_StripTaskHandle = (BattleFxTornadoControllerTask *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicTornadoStripParticleControllerTask);
        g_BattleMagicTornado_StripTaskHandle->state = 0;
        g_BattleMagicTornado_ModelPairTaskHandle = (BattleTornadoModelPairRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicTornadoModelPairTask);
        g_BattleMagicTornado_ModelPairTaskHandle->target = g_BattleMagicTornado_TargetPosition;
    }
    if (task->step == 0x22) {
        Battle_PostActorHitOrHurtMessage(task->recordType);
    }
    if (task->step == 0) {
        Battle_StartFixedPointRamp(0x400, 4);
        g_BattleFxTimelineState.red = 0;
        g_BattleFxTimelineState.channel1 = 0;
        g_BattleFxTimelineState.channel2 = 0;
    }
    relStep = task->step - 4;
    if (relStep >= 0) {
        if (relStep < 0x1E) {
            if (relStep % 5 == 0) {
                strip = BattleFx_MagicTornadoClaimStripParticleRecord();
                if (strip != 0) {
                    strip->period = rand() % 0x3E8 + 0x1388;
                    strip->height = 0;
                    strip->angle = rand() & 0xFFF;
                }
            }
            if (relStep % 1 == 0) {
                spiral = BattleFx_MagicTornadoClaimSpiralParticleRecord();
                if (spiral != 0) {
                    spiral->period = rand() % 0x3E8 + 0x1388;
                    spiral->height = 0;
                    spiral->angle = rand() & 0xFFF;
                    spiral->claimState = 0;
                }
            }
        } else {
            relStep -= 0x1E;
            if (relStep < 0xF) {
                g_BattleMagicTornado_AlphaFactor = relStep * -0x1000 / 0xF + 0x1000;
            } else {
                g_BattleMagicTornado_SpiralTaskHandle->state = -1;
                g_BattleMagicTornado_StripTaskHandle->state = -1;
            }
        }
    }
    if (task->step == 0x31) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->step)++;
}

/*
 * Allocates an available spiral particle record slot for Tornado.
 */
// FUNCTION: FF7 0x005ab815
BattleFxTornadoParticleRecord *__cdecl BattleFx_MagicTornadoClaimSpiralParticleRecord(void)
{
    int i;

    for (i = 0; i < 0x64; i++) {
        if (g_BattleMagicTornado_SpiralParticleRecords[i].claimState == -1) {
            g_BattleMagicTornado_SpiralParticleRecords[i].claimState = 0;
            g_BattleMagicTornado_SpiralParticleRecords[i].step = 0;
            g_BattleMagicTornado_SpiralParticleRecords[i].pointIndex = 0;
            return &g_BattleMagicTornado_SpiralParticleRecords[i];
        }
    }
    return 0;
}

/*
 * Allocates an available strip particle record slot for Tornado.
 */
// FUNCTION: FF7 0x005ab885
BattleFxTornadoParticleRecord *__cdecl BattleFx_MagicTornadoClaimStripParticleRecord(void)
{
    int i;

    for (i = 0; i < 0x64; i++) {
        if (g_BattleMagicTornado_StripParticleRecords[i].claimState == -1) {
            g_BattleMagicTornado_StripParticleRecords[i].claimState = 0;
            g_BattleMagicTornado_StripParticleRecords[i].step = 0;
            g_BattleMagicTornado_StripParticleRecords[i].pointIndex = 0;
            return &g_BattleMagicTornado_StripParticleRecords[i];
        }
    }
    return 0;
}

/*
 * Manages and steps active spiral particles for Tornado.
 */
// FUNCTION: FF7 0x005ab8f5
void __cdecl BattleFx_MagicTornadoSpiralParticleControllerTask(void)
{
    BattleFxTornadoControllerTask *taskSlot;
    int i;
    int activeCnt;

    taskSlot = (BattleFxTornadoControllerTask *)BATTLE_CURRENT_TASK_SLOT;
    activeCnt = 0;
    for (i = 0; i < 0x64; i++) {
        if (g_BattleMagicTornado_SpiralParticleRecords[i].claimState != -1) {
            BattleFx_MagicTornadoDrawAndStepSpiralParticle(&g_BattleMagicTornado_SpiralParticleRecords[i]);
            activeCnt++;
        }
    }
    if (taskSlot->state == 1 && activeCnt == 0) {
        taskSlot->state = -1;
    }
}

/*
 * Updates physics and renders a spiral quad particle for Tornado.
 */
// FUNCTION: FF7 0x005ab97b
void __cdecl BattleFx_MagicTornadoDrawAndStepSpiralParticle(BattleFxTornadoParticleRecord *particle)
{
    int idx;
    int padSlot; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int numSteps;
    BattleFxTornadoSpiralParticle *scratch;
    PsxMatrix *rotOut;
    int ringIndex;
    PsxMatrix *billboard;
    int heightDelta;
    SVECTOR *angleVec;
    int stepCap;

    scratch = (BattleFxTornadoSpiralParticle *)Gfx_GetPsxScratchpadPtr(0);
    angleVec = (SVECTOR *)(scratch + 1);
    rotOut = (PsxMatrix *)(angleVec + 1);
    g_BattleMagicTornado_ModelReq.tableField = Gfx_ModelTableGetSpriteObject(2, g_BattleMagicTornado_LoadedRes.tableCursor);
    if (g_BattleIsPaused == 0) {
        for (idx = 0; idx < 3; idx++) {
            particle->angle += (short)(0x25000 / particle->period);
            particle->height -= (short)(0x3A98 / (particle->period - 0x12C));
            if (particle->height >> 3 > -0x6E) {
                heightDelta = particle->height >> 4;
            } else {
                heightDelta = -0x6E;
            }
            particle->period -= (short)(heightDelta + 0x64);
            if (particle->period < 0x190) {
                particle->period = 0x190;
            }
            scratch->period = particle->period;
            scratch->height = particle->height;
            scratch->baseY = 0;
            angleVec->vx = 0;
            angleVec->vz = 0;
            angleVec->vy = particle->angle;
            RotMatrix(angleVec, rotOut);
            TransformVectorToInt16(rotOut, (SVECTOR *)scratch, (short *)scratch);
            particle->points[particle->pointIndex].vx = scratch->period + g_BattleMagicTornado_TargetPosition.vx;
            particle->points[particle->pointIndex].vy = scratch->height;
            particle->points[particle->pointIndex].vz = scratch->baseY + g_BattleMagicTornado_TargetPosition.vz;
            particle->pointIndex = particle->pointIndex + 1 & 7;
        }
    }
    if (particle->step < 5) {
        stepCap = particle->step;
    } else {
        stepCap = 5;
    }
    numSteps = stepCap;
    ringIndex = particle->pointIndex - numSteps & 7;
    for (idx = 1; idx < numSteps; idx++) {
        billboard = (PsxMatrix *)BattleFx_InstallBillboardMatrix((short *)&particle->points[ringIndex], 0x1000, 0);
        if (billboard->t[2] > 0 && billboard->t[2] < 0x7530) {
            g_BattleMagicTornado_QuadData.blue = (idx << 7) / 5 * g_BattleMagicTornado_AlphaFactor >> 0xC;
            g_BattleMagicTornado_QuadData.green = g_BattleMagicTornado_QuadData.blue;
            (g_BattleMagicTornado_QuadData.red) = g_BattleMagicTornado_QuadData.green;
            g_BattleMagicTornado_DrawListHandle = BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)&g_BattleMagicTornado_QuadReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicTornado_DrawListHandle);
        }
        ringIndex = ringIndex + 1 & 7;
    }
    if (g_BattleIsPaused == 0) {
        (particle->step)++;
        if (particle->step > 0x32) {
            particle->claimState = 0xFFFF;
        }
    }
}

/*
 * Manages and steps active strip particles for Tornado.
 */
// FUNCTION: FF7 0x005abc71
void __cdecl BattleFx_MagicTornadoStripParticleControllerTask(void)
{
    BattleFxTornadoControllerTask *taskSlot;
    int i;
    int activeCnt;

    taskSlot = (BattleFxTornadoControllerTask *)BATTLE_CURRENT_TASK_SLOT;
    activeCnt = 0;
    for (i = 0; i < 0x64; i++) {
        if (g_BattleMagicTornado_StripParticleRecords[i].claimState != -1) {
            BattleFx_MagicTornadoDrawAndStepStripParticle(&g_BattleMagicTornado_StripParticleRecords[i]);
            activeCnt++;
        }
    }
    if (taskSlot->state == 1 && activeCnt == 0) {
        taskSlot->state = -1;
    }
    if (g_BattleIsPaused == 0) {
        g_BattleMagicTornado_LoadedRes.counter4++;
    }
}

/*
 * Updates physics and renders a strip quad particle for Tornado.
 */
// FUNCTION: FF7 0x005abd11
void __cdecl BattleFx_MagicTornadoDrawAndStepStripParticle(BattleFxTornadoParticleRecord *stripState)
{
    int projection;
    BattleFxShardQuad *quadBuf;
    BattleFxTornadoStripScratch *pStripBlock;
    SVECTOR *vertexBPtr;
    int nextIdx;
    int stepSegCount;
    int padLocal; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int zDepth;
    int stepCap;
    int dH;
    SVECTOR *rotVector;
    SVECTOR *vertA;
    SVECTOR *xyzA;
    int bits;
    int color;
    BattleTornadoStripPrimitive *primitive;
    int i;
    int slotIdx;
    int padField; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    PsxMatrix *outMat;

    pStripBlock = (BattleFxTornadoStripScratch *)Gfx_GetPsxScratchpadPtr(0);
    rotVector = (SVECTOR *)(pStripBlock + 1);
    xyzA = rotVector + 1;
    vertA = xyzA + 1;
    quadBuf = (BattleFxShardQuad *)(vertA + 1);
    vertexBPtr = (SVECTOR *)quadBuf + 1;
    outMat = (PsxMatrix *)(vertexBPtr + 1);
    color = 0x3A000000;
    if (g_BattleIsPaused == 0) {
        for (i = 0; i < 3; i++) {
            stripState->angle += (short)(0x25000 / stripState->period);
            stripState->height -= (short)(0x3A98 / (stripState->period - 0x12C));
            if (stripState->height >> 3 > -0x6E) {
                dH = stripState->height >> 4;
            } else {
                dH = -0x6E;
            }
            stripState->period -= (short)(dH + 0x64);
            if (stripState->period < 0x190) {
                stripState->period = 0x190;
            }
            pStripBlock->period = stripState->period;
            pStripBlock->posY = stripState->height;
            pStripBlock->baseY = 0;
            rotVector->vx = 0;
            rotVector->vz = 0;
            rotVector->vy = stripState->angle;
            RotMatrix(rotVector, outMat);
            TransformVectorToInt16(outMat, (SVECTOR *)pStripBlock, (short *)pStripBlock);
            stripState->points[stripState->pointIndex].vx = pStripBlock->period + g_BattleMagicTornado_TargetPosition.vx;
            stripState->points[stripState->pointIndex].vy = pStripBlock->posY;
            stripState->points[stripState->pointIndex].vz = pStripBlock->baseY + g_BattleMagicTornado_TargetPosition.vz;
            stripState->pointIndex = stripState->pointIndex + 1 & 7;
        }
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    if (stripState->step < 8) {
        stepCap = stripState->step;
    } else {
        stepCap = 8;
    }
    stepSegCount = stepCap;
    slotIdx = stripState->pointIndex - stepSegCount & 7;
    for (i = 1; i < stepSegCount; i++) {
        primitive = (BattleTornadoStripPrimitive *)g_BattleMagicTornado_DrawListHandle;
        vertA->vx = stripState->points[slotIdx].vx;
        xyzA->vx = vertA->vx;
        xyzA->vy = stripState->points[slotIdx].vy - 0x19;
        vertA->vy = stripState->points[slotIdx].vy + 0x19;
        vertA->vz = stripState->points[slotIdx].vz;
        xyzA->vz = vertA->vz;
        nextIdx = slotIdx + 1 & 7;
        vertexBPtr->vx = stripState->points[nextIdx].vx;
        quadBuf->corner0X = vertexBPtr->vx;
        quadBuf->corner0Y = stripState->points[nextIdx].vy - 0x19;
        vertexBPtr->vy = stripState->points[nextIdx].vy + 0x19;
        vertexBPtr->vz = stripState->points[nextIdx].vz;
        quadBuf->depth = vertexBPtr->vz;
        zDepth = Gfx_TransformQuadReturnDepth(xyzA, vertA, (SVECTOR *)quadBuf, vertexBPtr, &projection, &bits);
        if ((bits & 0x60000) == 0) {
            primitive->primCode = 0x8000000;
            primitive->colorB = color;
            primitive->colorA = color;
            if (i == stepSegCount - 1) {
                color = 0x3A000000;
            } else {
                color += g_BattleMagicTornado_AlphaFactor << 4 >> 0xC << 0x10 | g_BattleMagicTornado_AlphaFactor << 4 >> 0xC << 8 | g_BattleMagicTornado_AlphaFactor << 4 >> 0xC;
            }
            primitive->colorD = color;
            primitive->colorC = color;
            Gfx_DrawPsxPrimitive(primitive);
        }
        slotIdx = nextIdx;
    }
    if (g_BattleIsPaused == 0) {
        (stripState->step)++;
        if (stripState->step > 0x32) {
            stripState->claimState = 0xFFFF;
        }
    }
}

/*
 * Animates and renders twin funnel models for Tornado.
 */
// FUNCTION: FF7 0x005ac142
void __cdecl BattleFx_MagicTornadoModelPairTask(void)
{
    BattleFxTornadoValuePair *scale;
    int frame;
    BattleTornadoModelPairRecord *task;
    BattleFxTornadoModelWork *matrix;

    task = (BattleTornadoModelPairRecord *)BATTLE_CURRENT_TASK_SLOT;
    matrix = (BattleFxTornadoModelWork *)Gfx_GetPsxScratchpadPtr(0);
    scale = &matrix->scale;
    frame = task->frame;
    if (frame < 0xF) {
        g_BattleMagicTornado_ModelReq.alpha = (frame << 7) / 0xF;
        scale->current = frame * 0x1500 / 0xF;
        scale->previous = scale->current;
        scale->baseValue = frame * 0 / 0xF + 0x1500;
    } else {
        frame -= 0xF;
        if (frame < 0) {
            g_BattleMagicTornado_ModelReq.alpha = 0x80;
            scale->current = 0x1500;
            scale->previous = 0x1500;
            scale->baseValue = 0x1500;
        } else {
            frame = frame;
            if (frame < 0x1E) {
                g_BattleMagicTornado_ModelReq.alpha = frame * -0x80 / 0x1E + 0x80;
                scale->current = (frame << 0xC) / 0x1E + 0x1500;
                scale->previous = scale->current;
                scale->baseValue = (frame << 0xC) / 0x1E + 0x1500;
            } else {
                task->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    RotMatrix((SVECTOR *)&g_BattleMagicTornado_LoadedRes.rotByte8, &matrix->matrix);
    ScaleMatrix(matrix, scale);
    matrix->matrix.t[0] = task->target.vx;
    matrix->matrix.t[1] = 0;
    matrix->matrix.t[2] = task->target.vz;
    MultiplyMatrix(&g_BattleCameraTransform, matrix, matrix);
    SetRotMatrix(&matrix->matrix);
    SetTransMatrix(matrix);
    g_BattleMagicTornado_ModelReq.modelHandle = Gfx_ModelTableGetRenderObject(0, g_BattleMagicTornado_LoadedRes.tableCursor);
    g_BattleMagicTornado_DrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicTornado_ModelReq.modelHandle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicTornado_DrawListHandle);
    g_BattleMagicTornado_ModelReq.modelHandle = Gfx_ModelTableGetRenderObject(1, g_BattleMagicTornado_LoadedRes.tableCursor);
    g_BattleMagicTornado_DrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicTornado_ModelReq.modelHandle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicTornado_DrawListHandle);
    if (g_BattleIsPaused == 0) {
        g_BattleMagicTornado_LoadedRes.angle10 -= -0x80;
        (task->frame)++;
    }
}
} /* extern "C" */
