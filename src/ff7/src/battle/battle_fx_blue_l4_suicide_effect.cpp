// src/ff7/src/battle/battle_fx_blue_l4_suicide_effect.cpp
// Enemy Skill [1]: L4 Suicide (Japanese: レベル4自爆, dev tag: "blue/lv4jibak" / Lv4Jibaku)
// Contents: The Lv4 Jibaku blue magic effect: per-target explosion sprites and expanding rings,
//   drawn as PSX-style Gouraud and flat triangles and quads.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_fx.h"
#include "ff7/camera_script.h"
#include "ff7/gte.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)



// clang-format off
// GLOBAL: FF7 0x008a8ee8
int g_BattleBlueL4SuicideResourceDef = 79929;
char *g_BattleFxBlueL4SuicideEffectResourceNames[1] = {
    // STRING: FF7 0x008a8f40
    "lv4ji_1.tim"
};
unsigned int g_BattleBlueL4SuicideResourceDefDataTail[8] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a8f4c
    (unsigned int)"lv4ji_1.tim",
    // STRING: FF7 0x008a8f58
    (unsigned int)"lv4jiba.spt",
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x008a8f10
int g_BattleBlueL4SuicideRefAxis[3] = {0, 0, -0x1000};
unsigned char g_rawData_008a8f20[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008a8f30
struct BattleFxSpriteRequest g_BattleBlueL4SuicideSpriteDrawReq = {0, 0, -128, -128, -128, 0x2e, 0, 0};
// GLOBAL: FF7 0x00b7bef8
int g_BattleBlueL4SuicideTargetCount = 0;
unsigned char g_bssPad_00b7befc[4] = {0};
// GLOBAL: FF7 0x00b7bf00
struct BattleFxBlueJibakuState g_BattleBlueL4SuicideStateBlob = {0};
// GLOBAL: FF7 0x00b7bf08
unsigned char g_BattleBlueL4SuicideRingRecords[984] = {0};
unsigned char g_bssPad_00b7c2e0[7440] = {0};
// GLOBAL: FF7 0x00b7dff0
struct GfxTableCursor *g_BattleBlueL4SuicideLoadedRes = 0;
unsigned char g_bssPad_00b7dff4[4] = {0};
// clang-format on

/*
 * Loads resources for the Lv4Jibaku blue magic effect when not already loaded.
 */
// FUNCTION: FF7 0x005696a0
void __cdecl BattleFx_BlueLoadResL4Suicide(void)
{
    GfxModelTableBank *sceneBase;
    GfxResourceRequest reqJob;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *extraPtr;
    int resInfoDef;

    if (g_pBattle3dScene != 0 && g_BattleBlueL4SuicideLoadedRes == 0) {
        kind = 2;
        resInfoDef = (int)&g_BattleBlueL4SuicideResourceDef;
        sceneBase = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        extraPtr = &reqJob;
        Gfx_InitResourceRequest(extraPtr);
        extraPtr->groupTag = 0;
        // STRING: FF7 0x008a8f64
        extraPtr->resourcePath = BattleFx_BuildResourcePath("blue/lv4jibak", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resInfoDef, extraPtr, &g_BattleBlueL4SuicideLoadedRes, sceneBase);
    }
}

/*
 * Entry point for the Lv4Jibaku blue magic effect.
 */
// FUNCTION: FF7 0x00569730
void __cdecl BattleFx_BlueEnterL4Suicide(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResL4Suicide();
    BattleFx_BlueBeginL4Suicide(targetMask, actorIdx);
}

/*
 * Initializes and begins the Lv4Jibaku blue magic effect.
 */
// FUNCTION: FF7 0x0056974a
void __cdecl BattleFx_BlueBeginL4Suicide(int targetMask, int actorIdx)
{
    g_BattleFxDrawListBufferTaskSlotPtr = (unsigned char *)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 1, (void *)BattleFx_BlueSpawnL4SuicidePerTarget);
    g_BattleBlueL4SuicideStateBlob.started = 0;
    g_BattleBlueL4SuicideTargetCount = 0;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x116);
}

/*
 * Spawns Lv4Jibaku effect tasks for each target actor.
 */
// FUNCTION: FF7 0x005697b2
void __cdecl BattleFx_BlueSpawnL4SuicidePerTarget(int actor, int sourceActor)
{
    int segment;
    BattleFxBlueJibakuRing *recordPtr;
    int value;
    BattleFxBlueJibakuTarget *pParticle;
    int segmentCount;
    int randomVal;
    BattleFxBlueL4SuicideTask *timeline;
    int length;

    timeline = (BattleFxBlueL4SuicideTask *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueL4SuicideTimelineTask);
    pParticle = (BattleFxBlueJibakuTarget *)Gfx_GetPsxScratchpadPtr(0);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)&timeline->pos);
    value = g_BattleActorRecords[actor].radius + 0xE0;
    if (value > 0x4F2) {
        value = 0x4F2;
    }
    timeline->spreadRadius = value;
    timeline->scaleRatio = (value << 0xC) / 0x12C;
    timeline->ringIndex = (short)g_BattleBlueL4SuicideTargetCount;
    timeline->nextFrame = 0;
    timeline->phase = 0;
    timeline->actorIndex = actor;
    recordPtr = (BattleFxBlueJibakuRing *)(g_BattleBlueL4SuicideTargetCount * sizeof(BattleFxBlueJibakuRing) + (char *)&g_BattleBlueL4SuicideRingRecords);
    g_BattleBlueL4SuicideTargetCount++;
    if (g_BattleBlueL4SuicideStateBlob.started == 0) {
        Battle_GetBoneWorldPos(sourceActor, g_BattleActorRecords[sourceActor].targetBone, (short *)&g_BattleBlueL4SuicideStateBlob);
        g_BattleBlueL4SuicideStateBlob.posY = 0;
        g_BattleBlueL4SuicideStateBlob.started = 1;
    }
    randomVal = rand();
    recordPtr->offset.vx = timeline->pos.vx - g_BattleBlueL4SuicideStateBlob.posX;
    recordPtr->offset.vy = 0;
    recordPtr->offset.vz = timeline->pos.vz - g_BattleBlueL4SuicideStateBlob.posZ;
    if ((randomVal & 1) != 0) {
        recordPtr->perp.vx = -recordPtr->offset.vz;
        recordPtr->perp.vz = recordPtr->offset.vx;
    } else {
        recordPtr->perp.vx = recordPtr->offset.vz;
        recordPtr->perp.vz = -recordPtr->offset.vx;
    }
    recordPtr->perp.vy = 0;
    length = NormalizeVectorFixed(&recordPtr->offset, &recordPtr->offset);
    NormalizeVectorFixed(&recordPtr->perp, &recordPtr->perp);
    length = SqrtToInt(length);
    pParticle->speed = randomVal % 0x8C + 0xC8;
    segment = value / 3;
    pParticle->amplitude = segment + randomVal % segment;
    pParticle->phaseStep = 0x1000 / (randomVal % 0xC + 0x10);
    pParticle->phase = pParticle->phaseStep;
    pParticle->accum = *(SVECTOR *)&g_BattleBlueL4SuicideStateBlob;
    pParticle->goal = timeline->pos;
    pParticle->vel.vx = recordPtr->offset.vx * pParticle->speed >> 0xC;
    pParticle->vel.vy = 0;
    pParticle->vel.vz = recordPtr->offset.vz * pParticle->speed >> 0xC;
    pParticle->swing = recordPtr->perp;
    segmentCount = length / pParticle->speed + 1;
    if (segmentCount > 0x20) {
        segmentCount = 0x20;
    }
    recordPtr->segmentCount = segmentCount;
    segmentCount++;
    recordPtr->segments[0].pos = pParticle->accum;
    for (segment = 1; segment < segmentCount; segment++) {
        pParticle->accum.vx += (short)pParticle->vel.vx;
        pParticle->accum.vz += (short)pParticle->vel.vz;
        length = SinFixed(pParticle->phase) * pParticle->amplitude >> 0xC;
        pParticle->pos.vx = pParticle->accum.vx + (pParticle->swing.vx * length >> 0xC);
        pParticle->pos.vy = 0;
        pParticle->pos.vz = pParticle->accum.vz + (pParticle->swing.vz * length >> 0xC);
        recordPtr->segments[segment].pos = pParticle->pos;
        pParticle->rel.vx = pParticle->pos.vx - recordPtr->segments[segment - 1].pos.vx;
        pParticle->rel.vy = 0;
        pParticle->rel.vz = pParticle->pos.vz - recordPtr->segments[segment - 1].pos.vz;
        NormalizeVectorFixed(&pParticle->rel, &pParticle->rel);
        recordPtr->segments[segment].angle = ComputeAngleBetweenVectors((VECTOR *)g_BattleBlueL4SuicideRefAxis, &pParticle->rel, &pParticle->axis);
        recordPtr->segments[segment].axis = pParticle->axis;
        pParticle->phase += pParticle->phaseStep;
    }
    recordPtr->segments[0].axis = recordPtr->segments[1].axis;
    recordPtr->segments[0].angle = recordPtr->segments[1].angle;
}

/*
 * Timeline task coordinating the explosion and ring expansion for Lv4Jibaku.
 */
// FUNCTION: FF7 0x00569cab
void __cdecl BattleFx_BlueL4SuicideTimelineTask(void)
{
    int p;
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxBlueJibakuRing *ring;
    int spawnIndex;
    int spread;
    BattleFxJibakuShard *pShardData;
    int fullRange;
    int randFactor;
    BattleFxBlueL4SuicideTask *mainTaskSlot;
    BattleFxBlueL4SuicideSprite *nodePtr;

    mainTaskSlot = (BattleFxBlueL4SuicideTask *)BATTLE_CURRENT_TASK_SLOT;
    ring = (BattleFxBlueJibakuRing *)(mainTaskSlot->ringIndex * sizeof(BattleFxBlueJibakuRing) + (char *)&g_BattleBlueL4SuicideRingRecords);
    pShardData = (BattleFxJibakuShard *)Gfx_GetPsxScratchpadPtr(0);
    switch (mainTaskSlot->phase) {
    case BLUE_JIBAKU_PHASE_FADE_IN:
        ring->segmentIndex = 0;
        p = mainTaskSlot->frame * 0x60606;
        if (mainTaskSlot->frame >= 7) {
            mainTaskSlot->phase = BLUE_JIBAKU_PHASE_HOLD;
        }
        break;
    case BLUE_JIBAKU_PHASE_HOLD:
        ring->segmentIndex = 0;
        p = 0x303030;
        if (mainTaskSlot->frame >= 0xB) {
            mainTaskSlot->phase = BLUE_JIBAKU_PHASE_SWEEP;
        }
        break;
    case BLUE_JIBAKU_PHASE_SWEEP:
        ring->segmentIndex = mainTaskSlot->frame - 0xB;
        p = 0x303030;
        if (ring->segmentCount == ring->segmentIndex) {
            mainTaskSlot->phase = BLUE_JIBAKU_PHASE_EXPLODE;
            mainTaskSlot->nextFrame = mainTaskSlot->frame + 4;
            Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&mainTaskSlot->pos), 0xFA);
        }
        break;
    case BLUE_JIBAKU_PHASE_EXPLODE:
        if (g_BattleIsPaused == 0) {
            spread = mainTaskSlot->spreadRadius;
            fullRange = spread << 1;
            for (spawnIndex = 0; spawnIndex < 2; spawnIndex++) {
                nodePtr = (BattleFxBlueL4SuicideSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueL4SuicideAnimatedSpriteTask);
                nodePtr->pos = mainTaskSlot->pos;
                nodePtr->pos.vx += (short)(rand() % fullRange - spread);
                nodePtr->pos.vy += (short)(rand() % fullRange - spread);
                nodePtr->pos.vz += (short)(rand() % fullRange - spread);
                randFactor = rand() % 0x1000 + 0x800;
                nodePtr->scale = mainTaskSlot->scaleRatio * randFactor >> 0xC;
            }
        }
        if (mainTaskSlot->frame >= mainTaskSlot->nextFrame) {
            mainTaskSlot->phase = BLUE_JIBAKU_PHASE_AFTERGLOW;
            mainTaskSlot->nextFrame += 5;
        }
        break;
    case BLUE_JIBAKU_PHASE_AFTERGLOW:
        if (mainTaskSlot->frame >= mainTaskSlot->nextFrame) {
            mainTaskSlot->phase = BLUE_JIBAKU_PHASE_ALLOC_RECORD;
        }
        break;
    case BLUE_JIBAKU_PHASE_ALLOC_RECORD:
        if (g_BattleIsPaused == 0) {
            Battle_PostActorHitOrHurtMessage(mainTaskSlot->actorIndex);
            mainTaskSlot->phase = BLUE_JIBAKU_PHASE_FINISH;
            mainTaskSlot->nextFrame += 8;
        }
        break;
    case BLUE_JIBAKU_PHASE_FINISH:
        if (mainTaskSlot->frame >= mainTaskSlot->nextFrame) {
            mainTaskSlot->ownerSlot = BATTLE_TASK_FREE;
        }
    }
    if (ring->segmentCount != ring->segmentIndex) {
        BattleFx_BlueL4SuicideDrawRingSegments(pShardData, ring, p);
        if (mainTaskSlot->phase == BLUE_JIBAKU_PHASE_SWEEP) {
            BattleFx_BlueL4SuicideDrawPsxGouraudTriangles(pShardData, ring);
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (mainTaskSlot->frame)++;
}

/*
 * Task animating and rendering explosion sprites for Lv4Jibaku.
 */
// FUNCTION: FF7 0x00569fda
void __cdecl BattleFx_BlueL4SuicideAnimatedSpriteTask(void)
{
    BattleFxBlueL4SuicideSprite *task;

    task = (BattleFxBlueL4SuicideSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleBlueL4SuicideSpriteDrawReq.frame = task->frame;
    BattleFx_InstallBillboardMatrix((short *)&task->pos, (short)task->scale, -(task->scale >> 3));
    g_BattleBlueL4SuicideSpriteDrawReq.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_BattleBlueL4SuicideLoadedRes);
    g_BattleBlueL4SuicideSpriteDrawReq.drawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleBlueL4SuicideLoadedRes);
    g_BattleFxActiveDrawListPtr = (int)BattleFx_SubmitSpriteDraw(&g_BattleBlueL4SuicideSpriteDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->frame >= 0xB) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (task->frame)++;
}

/*
 * Renders Gouraud-shaded triangles forming ring segments for Lv4Jibaku.
 */
// FUNCTION: FF7 0x0056a0a9
void __cdecl BattleFx_BlueL4SuicideDrawPsxGouraudTriangles(BattleFxJibakuShard *shard, BattleFxBlueJibakuRing *ringRecord)
{
    BattleFxPsxPolygonPacket *primitive;
    int i, len, segmentIndex;

    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    primitive = (BattleFxPsxPolygonPacket *)Gfx_GetPsxPacketScratchPtr();
    segmentIndex = ringRecord->segmentIndex;
    for (i = 0; i < 0x28; i++) {
        shard->vel0.vx = rand() % 0x1000 - 0x800;
        shard->vel0.vy = -(rand() % 0x800);
        shard->vel0.vz = rand() % 0x1000 - 0x800;
        shard->vel1.vx = -shard->vel0.vy - shard->vel0.vz;
        shard->vel1.vy = shard->vel0.vx - shard->vel0.vz;
        shard->vel1.vz = shard->vel0.vx + shard->vel0.vy;
        NormalizeVectorFixed(&shard->vel0, &shard->vel0);
        NormalizeVectorFixed(&shard->vel1, &shard->vel1);
        len = rand() % 0xB4 + 0x190;
        shard->vertex[0] = ringRecord->segments[segmentIndex].pos;
        shard->vertex[0].vx += (short)(shard->vel0.vx * len >> 0xC);
        shard->vertex[0].vy += (short)(shard->vel0.vy * len >> 0xC);
        shard->vertex[0].vz += (short)(shard->vel0.vz * len >> 0xC);
        len >>= 3;
        shard->vertex[1] = ringRecord->segments[segmentIndex].pos;
        shard->vertex[1].vx += (short)(shard->vel1.vx * len >> 0xC);
        shard->vertex[1].vy += (short)(shard->vel1.vy * len >> 0xC);
        shard->vertex[1].vz += (short)(shard->vel1.vz * len >> 0xC);
        shard->depth = Gfx_TransformTriangleReturnDepthAlt(&shard->vertex[0], &shard->vertex[1], &ringRecord->segments[segmentIndex].pos, &shard->projected, &shard->clipFlags);
        primitive->colorCmd = 0x2030A;
        primitive->color1 = shard->vel0.vx & 0x1F1F3F;
        primitive->color2 = 0xE0E24;
        primitive->tag = primitive->tag & 0xFFFFFF | 0x6000000;
        *(char *)((char *)primitive + 7) = 0x32;
        Gfx_DrawPsxPrimitive(primitive);
    }
    g_BattleFxActiveDrawListPtr = (int)primitive;
}

/*
 * Coordinates rendering of expanding ring segments for Lv4Jibaku.
 */
// FUNCTION: FF7 0x0056a398
void __cdecl BattleFx_BlueL4SuicideDrawRingSegments(BattleFxJibakuShard *ringState, BattleFxBlueJibakuRing *ringRecord, int color)
{
    int segmentIdx, nextSegment, vertex;

    BattleFx_BlueL4SuicideProjectRingPoints(ringState, &ringRecord->segments[ringRecord->segmentIndex], 0x1A);
    for (segmentIdx = ringRecord->segmentIndex; segmentIdx < ringRecord->segmentCount; segmentIdx++) {
        nextSegment = segmentIdx + 1;
        for (vertex = 0; vertex < 4; vertex++) {
            ringState->prevQuadXY[vertex] = ringState->quadXY[vertex];
            ringState->prevBlendFirst[vertex] = ringState->blendFirst[vertex];
            ringState->prevBlendSecond[vertex] = ringState->blendSecond[vertex];
        }
        ringState->prevDepth = ringState->depth;
        BattleFx_BlueL4SuicideProjectRingPoints(ringState, &ringRecord->segments[nextSegment], 0x1A);
        BattleFx_BlueL4SuicideDrawPsxFlatRingQuads(ringState, &ringRecord->segments[nextSegment], color);
    }
}

/*
 * Renders flat-shaded quad polygons for Lv4Jibaku expanding rings.
 */
// FUNCTION: FF7 0x0056a49c
void __cdecl BattleFx_BlueL4SuicideDrawPsxFlatRingQuads(BattleFxJibakuShard *ringState, void *context, int color)
{
    BattleFxRingQuadPrim *primPtr;
    int corner, nextCorner, radius;

    radius = ringState->prevDepth + ringState->depth >> 1 >> 2;
    if (radius < 0) {
        return;
    }
    primPtr = (BattleFxRingQuadPrim *)Gfx_GetPsxPacketScratchPtr();
    for (corner = 0; corner < 4; corner++) {
        nextCorner = corner + 1 & 3;
        primPtr->xyWord0 = ringState->quadXY[corner];
        primPtr->xyWord1 = ringState->quadXY[nextCorner];
        primPtr->xyWord2 = ringState->prevQuadXY[corner];
        primPtr->xyWord3 = ringState->prevQuadXY[nextCorner];
        primPtr->color = color;
        primPtr->tag = primPtr->tag & 0xFFFFFF | 0x5000000;
        *(char *)((char *)primPtr + 7) = 0x2A;
        Gfx_SetBlendFactorQuad(*(float *)&ringState->blendFirst[corner], *(float *)&ringState->blendSecond[corner], *(float *)&ringState->blendFirst[nextCorner], *(float *)&ringState->blendSecond[nextCorner], *(float *)&ringState->prevBlendFirst[corner], *(float *)&ringState->prevBlendSecond[corner], *(float *)&ringState->prevBlendFirst[nextCorner], *(float *)&ringState->prevBlendSecond[nextCorner]);
        Gfx_DrawPsxPrimitive(primPtr);
    }
}

/*
 * Projects 3D ring vertex coordinates into screen space for Lv4Jibaku.
 */
// FUNCTION: FF7 0x0056a5e4
void __cdecl BattleFx_BlueL4SuicideProjectRingPoints(BattleFxJibakuShard *ring, BattleFxJibakuRingSource *source, int size)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxProjectedPoint *projectedPtr;
    int res;
    int extent;

    Gte_BuildAxisAngleMatrix(source->angle, &ring->ringMatrix, &source->axis);
    ring->ringMatrix.t[0] = source->pos.vx;
    ring->ringMatrix.t[1] = source->pos.vy;
    ring->ringMatrix.t[2] = source->pos.vz;
    ring->vertex[0].vx = 0;
    ring->vertex[0].vy = -size;
    ring->vertex[0].vz = 0;
    ring->vertex[1].vx = size;
    ring->vertex[1].vy = 0;
    ring->vertex[1].vz = 0;
    ring->vertex[2].vx = 0;
    ring->vertex[2].vy = size;
    ring->vertex[2].vz = 0;
    ring->vertex[3].vx = -size;
    ring->vertex[3].vy = 0;
    ring->vertex[3].vz = 0;
    MultiplyMatrix(&g_BattleCameraTransform, &ring->ringMatrix, &ring->cameraMatrix);
    SetRotMatrix(&ring->cameraMatrix);
    SetTransMatrix(&ring->cameraMatrix);
    RefreshGlobalTransformState();
    ring->depth = Gfx_ProjectQuadToScreenAlt(&ring->vertex[0], &ring->vertex[1], &ring->vertex[2], &ring->vertex[3], &ring->quadXY[0], &ring->quadXY[1], &ring->quadXY[2], &ring->quadXY[3], &res, &extent);
    projectedPtr = (BattleFxProjectedPoint *)Gfx_GetProjectedVertexRecord(0);
    ring->blendFirst[0] = projectedPtr->screenX;
    ring->blendSecond[0] = projectedPtr->screenY;
    projectedPtr = (BattleFxProjectedPoint *)Gfx_GetProjectedVertexRecord(1);
    ring->blendFirst[1] = projectedPtr->screenX;
    ring->blendSecond[1] = projectedPtr->screenY;
    projectedPtr = (BattleFxProjectedPoint *)Gfx_GetProjectedVertexRecord(2);
    ring->blendFirst[2] = projectedPtr->screenX;
    ring->blendSecond[2] = projectedPtr->screenY;
    projectedPtr = (BattleFxProjectedPoint *)Gfx_GetProjectedVertexRecord(3);
    ring->blendFirst[3] = projectedPtr->screenX;
    ring->blendSecond[3] = projectedPtr->screenY;
}
} /* extern "C" */
