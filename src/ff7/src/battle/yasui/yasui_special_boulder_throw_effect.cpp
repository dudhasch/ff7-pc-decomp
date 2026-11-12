// src/ff7/src/battle/yasui/yasui_special_boulder_throw_effect.cpp
/* Enemy Special Attack [146]: Boulder Throw (orig. SPECIAL/TOSEKI, 投石) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle effect for the Toseki special effect: a rock mesh flies in, then shatters into
//   displaced fragments drawn from PSX geometry, with hit sprites and a sound cue.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
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

struct GfxPsxShortVertexSlot;
struct GfxPsxSpriteQuadState;

#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
#pragma intrinsic(abs)


// clang-format off
// GLOBAL: FF7 0x007e2f10
unsigned char g_BoulderThrow_MeshPayload[440] = {0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc8, 0xfe, 0xe5, 0xff, 0x00, 0x00, 0x00, 0x00, 0x4b, 0xff, 0x1f, 0xff, 0x00, 0x00, 0xd4, 0x00, 0x91, 0xff, 0xbb, 0xff, 0x00, 0x00, 0x83, 0x00, 0x44, 0xff, 0x91, 0x00, 0x00, 0x00, 0x7d, 0xff, 0x91, 0xff, 0xb4, 0x00, 0x00, 0x00, 0x2c, 0xff, 0x91, 0xff, 0xbb, 0xff, 0x00, 0x00, 0x83, 0x00, 0x3d, 0x00, 0x4c, 0xff, 0x00, 0x00, 0xd4, 0x00, 0x6f, 0x00, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2e, 0x00, 0xd1, 0x00, 0x00, 0x00, 0x2c, 0xff, 0x6f, 0x00, 0x45, 0x00, 0x00, 0x00, 0x7d, 0xff, 0x6f, 0x00, 0x4c, 0xff, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x01, 0xfb, 0xff, 0x00, 0x00, 0x14, 0x00, 0x2c, 0x00, 0x58, 0x00, 0x48, 0x00, 0x50, 0x00, 0x80, 0x00, 0x3d, 0x7e, 0x08, 0x78, 0x41, 0x37, 0x7e, 0x4d, 0x58, 0x00, 0x40, 0x00, 0x48, 0x00, 0x80, 0x00, 0x3d, 0x7e, 0x08, 0x78, 0x00, 0x38, 0x41, 0x37, 0x58, 0x00, 0x38, 0x00, 0x40, 0x00, 0x80, 0x00, 0x3e, 0x7e, 0x08, 0x78, 0x68, 0x50, 0x1b, 0x3e, 0x58, 0x00, 0x30, 0x00, 0x38, 0x00, 0x80, 0x00, 0x4b, 0x00, 0x08, 0x78, 0x42, 0x37, 0x00, 0x18, 0x58, 0x00, 0x50, 0x00, 0x30, 0x00, 0x80, 0x00, 0x4b, 0x00, 0x08, 0x78, 0x7e, 0x38, 0x42, 0x37, 0x28, 0x00, 0x08, 0x00, 0x50, 0x00, 0x80, 0x00, 0x20, 0x67, 0x08, 0x78, 0x6f, 0x5a, 0x49, 0x7e, 0x28, 0x00, 0x50, 0x00, 0x48, 0x00, 0x80, 0x00, 0x69, 0x02, 0x08, 0x78, 0x7e, 0x4d, 0x41, 0x37, 0x20, 0x00, 0x28, 0x00, 0x48, 0x00, 0x80, 0x00, 0x42, 0x4c, 0x08, 0x78, 0x00, 0x3b, 0x19, 0x7e, 0x20, 0x00, 0x48, 0x00, 0x40, 0x00, 0x80, 0x00, 0x42, 0x4c, 0x08, 0x78, 0x19, 0x7e, 0x69, 0x6d, 0x18, 0x00, 0x20, 0x00, 0x40, 0x00, 0x80, 0x00, 0x7e, 0x24, 0x08, 0x78, 0x42, 0x4c, 0x69, 0x6d, 0x18, 0x00, 0x40, 0x00, 0x38, 0x00, 0x80, 0x00, 0x48, 0x00, 0x08, 0x78, 0x1b, 0x3e, 0x68, 0x50, 0x10, 0x00, 0x18, 0x00, 0x38, 0x00, 0x80, 0x00, 0x7e, 0x17, 0x08, 0x78, 0x48, 0x00, 0x68, 0x50, 0x10, 0x00, 0x38, 0x00, 0x30, 0x00, 0x80, 0x00, 0x08, 0x58, 0x08, 0x78, 0x00, 0x18, 0x42, 0x37, 0x08, 0x00, 0x10, 0x00, 0x30, 0x00, 0x80, 0x00, 0x51, 0x7e, 0x08, 0x78, 0x08, 0x58, 0x42, 0x37, 0x08, 0x00, 0x30, 0x00, 0x50, 0x00, 0x80, 0x00, 0x51, 0x7e, 0x08, 0x78, 0x42, 0x37, 0x7e, 0x38, 0x00, 0x00, 0x08, 0x00, 0x28, 0x00, 0x80, 0x00, 0x46, 0x2c, 0x08, 0x78, 0x6f, 0x5a, 0x20, 0x67, 0x00, 0x00, 0x28, 0x00, 0x20, 0x00, 0x80, 0x00, 0x3d, 0x00, 0x08, 0x78, 0x00, 0x3b, 0x42, 0x4c, 0x00, 0x00, 0x20, 0x00, 0x18, 0x00, 0x80, 0x00, 0x3d, 0x00, 0x08, 0x78, 0x42, 0x4c, 0x7e, 0x24, 0x00, 0x00, 0x18, 0x00, 0x10, 0x00, 0x80, 0x00, 0x46, 0x2c, 0x08, 0x78, 0x41, 0x00, 0x7e, 0x1f, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x80, 0x00, 0x46, 0x2c, 0x08, 0x78, 0x7e, 0x1f, 0x6f, 0x5a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007e30c8
unsigned int g_BoulderThrowResourceModelRows[25] = {0x00197c3c, (unsigned int)"ISI.TIM", 0x00000000, 0x00000000, 0x00000000, 0x00197c39, (unsigned int)"ISI.TIM", 0x00000000, 0x00000000, 0x00000000, 0x0019783c, (unsigned int)"ISI.TIM", 0x00000000, 0x00000000, 0x00000000, 0x00197839, (unsigned int)"ISI.TIM", 0x00000000, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"HIT1.TIM", (unsigned int)"HIT1.SPT", 0x00000000, 0x00000000};
// GLOBAL: FF7 0x007e3130
struct BoulderThrow_SpriteResourcePointer g_BoulderThrowSpriteResourcePointer = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
// GLOBAL: FF7 0x007e3140
BattleFxPsxDisplacedMeshDesc g_BoulderThrowMeshDrawDescriptor = {(int)&g_BoulderThrow_MeshPayload, 0, {0}, 0x80, 0x20};
// GLOBAL: FF7 0x007e3150
unsigned char g_BoulderThrowMeshScaleFixed12[16] = {0x00, 0x0a, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x00a38930
struct BoulderThrow_DisplacementVelocities g_BoulderThrow_DisplacementVelocities = {0};
unsigned char g_bssPad_00a38938[152] = {0};
// GLOBAL: FF7 0x00a389d0
struct YasuiBoulderThrowDisplacement g_BoulderThrow_DisplacementPositions = {0};
#define g_BoulderThrow_DisplacementPosArray (&g_BoulderThrow_DisplacementPositions)
#define g_BoulderThrow_DisplacementVelArray (&g_BoulderThrow_DisplacementVelocities)
unsigned char g_bssPad_00a389d8[152] = {0};
// GLOBAL: FF7 0x00a38a70
int g_BoulderThrow_MeshDrawHandle = 0;
// GLOBAL: FF7 0x00a38a74
struct GfxTableCursor *g_BoulderThrow_LoadedResTable = 0;
// GLOBAL: FF7 0x00a38a78
short g_BoulderThrow_PitchAngle = 0;
unsigned char g_bssPad_00a38a7c[4] = {0};
// GLOBAL: FF7 0x00a38a80
short g_BoulderThrow_YawAngle = 0;
unsigned char g_bssPad_00a38a84[4] = {0};
// clang-format on

/*
 * Initializes and requests loading the SPECIAL/TOSEKI battle effect resource if not already loaded.
 */
// FUNCTION: FF7 0x004559b0
void __cdecl BattleFx_SpecialLoadResBoulderThrow(void)
{
    char pathScratch[200];
    GfxModelTableBank *context;
    GfxResourceRequest requestRecord;
    int kind;
    GfxResourceRequest *pReq;
    int resourceRows;

    if (g_pBattle3dScene != 0 && g_BoulderThrow_LoadedResTable == 0) {
        kind = 5;
        resourceRows = (int)&g_BoulderThrowResourceModelRows;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestRecord;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x007e3198
        pReq->resourcePath = BattleFx_BuildResourcePath("SPECIAL/TOSEKI", pathScratch);
        Gfx_LoadModelTableIfAbsent(kind, resourceRows, pReq, &g_BoulderThrow_LoadedResTable, context);
    }
}

/*
 * Entry point for the Toseki special effect: loads resources and starts the effect task.
 */
// FUNCTION: FF7 0x00455a40
void __cdecl BattleFx_SpecialEnterBoulderThrow(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResBoulderThrow();
    BattleFx_SpecialBeginBoulderThrow(targetMask, actorIdx);
}

/*
 * Creates bit-pattern task for Toseki, dispatches sound event 0x163, and initializes displacement vectors.
 */
// FUNCTION: FF7 0x00455a5a
void __cdecl BattleFx_SpecialBeginBoulderThrow(int targetMask, int actorIdx)
{
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, (void *)BattleFx_SpecialSpawnBoulderThrowPerTarget);
    Battle_PlaySfxPannedToActor(0x163, (short)actorIdx, 0);
    BattleFx_SpecialBoulderThrowInitDisplacementVectors();
}

/*
 * Initializes randomized displacement vectors and clears displacement position offsets for 20 mesh pieces.
 */
// FUNCTION: FF7 0x00455a8f
void __cdecl BattleFx_SpecialBoulderThrowInitDisplacementVectors(void)
{
    EffectVectorInitLocals l;

    for (l.index = 0; l.index < 20; l.index++) {
        g_BoulderThrow_DisplacementPosArray[l.index].offsetZ = 0;
        g_BoulderThrow_DisplacementPosArray[l.index].offsetY = 0;
        g_BoulderThrow_DisplacementPosArray[l.index].offset = 0;
        l.vector[0] = 0;
        l.vector[1] = 0;
        l.vector[2] = 0x258;
        l.angles[0] = rand();
        l.angles[1] = rand();
        l.angles[2] = 0;
        RotMatrix((SVECTOR *)l.angles, l.matrix);
        TransformVectorToInt16(l.matrix, l.vector, &g_BoulderThrow_DisplacementVelArray[l.index]);
    }
}

/*
 * Spawns a Toseki timeline task slot for the given target actor and source actor.
 */
// FUNCTION: FF7 0x00455b3f
void __cdecl BattleFx_SpecialSpawnBoulderThrowPerTarget(int actor, int sourceActor)
{
    BattleFxBoulderThrowTask *taskSlot;

    taskSlot = (BattleFxBoulderThrowTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBoulderThrowTimelineTask);
    Battle_GetBoneWorldPos(sourceActor, g_BattleActorRecords[sourceActor].bone3a, taskSlot->startPos);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].altBone, taskSlot->endPos);
    taskSlot->sourceActor = sourceActor;
    taskSlot->targetActor = actor;
}

/*
 * Timeline task coordinating the flying rock mesh and subsequent hit particles/sound event.
 */
// FUNCTION: FF7 0x00455bc0
void __cdecl BattleFx_SpecialBoulderThrowTimelineTask(void)
{
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[68];
        struct { short angles[3]; unsigned char pad_06[6]; short vector[3]; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix matrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxBoulderThrowSegment *segment; };
        struct { unsigned char pad_to_3C[0x3C]; short particleIndex; };
        struct { unsigned char pad_to_40[0x40]; BattleFxBoulderThrowTimeline *timeline; };
    } frame;

    frame.timeline = (BattleFxBoulderThrowTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timeline->timer == 0) {
        frame.segment = (BattleFxBoulderThrowSegment *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBoulderThrowShatteringMeshTask);
        frame.segment->posX = frame.timeline->startX;
        frame.segment->posY = frame.timeline->startY;
        frame.segment->posZ = frame.timeline->startZ;
        frame.segment->stepX = (frame.timeline->endX - frame.timeline->startX) / 20;
        frame.segment->stepY = (frame.timeline->endY - frame.timeline->startY) / 20;
        frame.segment->stepZ = (frame.timeline->endZ - frame.timeline->startZ) / 20;
        frame.segment = (BattleFxBoulderThrowSegment *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBoulderThrowDampedSpriteParticleTask);
        frame.segment->posX = frame.timeline->startX;
        frame.segment->posY = frame.timeline->startY;
        frame.segment->posZ = frame.timeline->startZ;
        frame.segment->stepX = 0;
        frame.segment->stepY = 0xFFB0;
        frame.segment->stepZ = 0;
    }
    if (frame.timeline->timer == 20) {
        for (frame.particleIndex = 0; frame.particleIndex < 10; frame.particleIndex++) {
            frame.segment = (BattleFxBoulderThrowSegment *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBoulderThrowDampedSpriteParticleTask);
            frame.segment->posX = frame.timeline->endX;
            frame.segment->posY = frame.timeline->endY;
            frame.segment->posZ = frame.timeline->endZ;
            frame.vector[0] = 0;
            frame.vector[1] = 0;
            frame.vector[2] = 0x190;
            frame.angles[0] = rand();
            frame.angles[1] = rand();
            frame.angles[2] = 0;
            RotMatrix((SVECTOR *)frame.angles, &frame.matrix);
            TransformVectorToInt16(&frame.matrix, frame.vector, &frame.segment->stepX);
        }
        Battle_PostActorHitOrHurtMessage(frame.timeline->recordType);
        Battle_PlaySfxPannedToActor(0x14, (short)frame.timeline->recordType, 0);
    }
    (frame.timeline->timer)++;
    if (frame.timeline->timer == 30) {
        frame.timeline->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Billboard sprite particle task with velocity damping and position integration.
 */
// FUNCTION: FF7 0x00455e16
void __cdecl BattleFx_SpecialBoulderThrowDampedSpriteParticleTask(void)
{
    BattleFxEnergyOrbDriftSprite *taskSlot;

    taskSlot = (BattleFxEnergyOrbDriftSprite *)BATTLE_CURRENT_TASK_SLOT;
    (g_BoulderThrowSpriteResourcePointer.resource) = Gfx_ModelTableGetLoadedRecord(4, g_BoulderThrow_LoadedResTable);
    g_BoulderThrowSpriteResourcePointer.objectPointer = Gfx_ModelTableGetSpriteObject(4, g_BoulderThrow_LoadedResTable);
    BattleFx_InstallBillboardMatrix(&taskSlot->posX, 0x2000 /* 2.0 */, 0);
    g_BoulderThrowSpriteResourcePointer.frameIndex = taskSlot->frameIndex;
    g_BoulderThrow_MeshDrawHandle = BattleFx_SubmitSpriteDraw(&g_BoulderThrowSpriteResourcePointer, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BoulderThrow_MeshDrawHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    taskSlot->velocityX = taskSlot->velocityX * 192 / 256;
    taskSlot->velocityY = taskSlot->velocityY * 192 / 256;
    taskSlot->velocityZ = taskSlot->velocityZ * 192 / 256;
    taskSlot->posX += taskSlot->velocityX;
    taskSlot->posY += taskSlot->velocityY;
    taskSlot->posZ += taskSlot->velocityZ;
    if ((g_BoulderThrowSpriteResourcePointer.resource) != 0) {
        (taskSlot->frameIndex)++;
        if (taskSlot->frameIndex == *(int *)((char *)*(int *)(g_BoulderThrowSpriteResourcePointer.resource) + 4)) {
            taskSlot->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Animates rock mesh trajectory, then renders shattered/displaced fragments after impact.
 */
// FUNCTION: FF7 0x00455f8b
void __cdecl BattleFx_SpecialBoulderThrowShatteringMeshTask(void)
{
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[108];
        PsxMatrix yawMatrix;
        struct { unsigned char pad_to_20[0x20]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_44[0x44]; short fragmentTimer; };
        struct { unsigned char pad_to_48[0x48]; BattleFxBoulderThrowShardState *shard; };
        struct { unsigned char pad_to_4C[0x4C]; PsxMatrix viewMatrix; };
    } frame;

    frame.shard = (BattleFxBoulderThrowShardState *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.shard->timer < 20) {
        g_BoulderThrow_PitchAngle = -frame.shard->timer << 7;
        RotMatrix((SVECTOR *)&g_BoulderThrow_PitchAngle, &frame.rotMatrix);
        ScaleMatrix(&frame.rotMatrix, (int *)&g_BoulderThrowMeshScaleFixed12);
        frame.rotMatrix.t[0] = frame.shard->posX;
        frame.rotMatrix.t[1] = frame.shard->posY - (SinFixed(frame.shard->timer * 0x66) * 0x7D0 >> 12);
        frame.rotMatrix.t[2] = frame.shard->posZ;
        MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.viewMatrix);
        SetRotMatrix(&frame.viewMatrix);
        SetTransMatrix(&frame.viewMatrix);
        RefreshGlobalTransformState();
        Gfx_SetPsxPrimQuadState(9, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(0, g_BoulderThrow_LoadedResTable)));
        Gfx_SetPsxPrimQuadState(9, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(1, g_BoulderThrow_LoadedResTable)));
        Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(2, g_BoulderThrow_LoadedResTable)));
        Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(3, g_BoulderThrow_LoadedResTable)));
        g_BoulderThrow_MeshDrawHandle = BattleFx_SpecialBoulderThrowDrawPsxMesh((BattleFxPsxMeshDescriptor *)&g_BoulderThrowMeshDrawDescriptor, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BoulderThrow_MeshDrawHandle);
    } else {
        frame.fragmentTimer = frame.shard->timer - 20;
        g_BoulderThrowMeshDrawDescriptor.flags = 8;
        g_BoulderThrowMeshDrawDescriptor.shade = 0x80 - (frame.fragmentTimer << 4);
        RotMatrix((SVECTOR *)&g_BoulderThrow_PitchAngle, &frame.rotMatrix);
        ScaleMatrix(&frame.rotMatrix, (int *)&g_BoulderThrowMeshScaleFixed12);
        frame.rotMatrix.t[0] = frame.shard->posX;
        frame.rotMatrix.t[1] = frame.shard->posY;
        frame.rotMatrix.t[2] = frame.shard->posZ;
        MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.viewMatrix);
        g_BoulderThrow_YawAngle = frame.fragmentTimer << 8;
        RotMatrix((SVECTOR *)&g_BoulderThrow_YawAngle, &frame.yawMatrix);
        Gfx_SetPsxPrimQuadState(9, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(0, g_BoulderThrow_LoadedResTable)));
        Gfx_SetPsxPrimQuadState(9, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(1, g_BoulderThrow_LoadedResTable)));
        Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(2, g_BoulderThrow_LoadedResTable)));
        Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(3, g_BoulderThrow_LoadedResTable)));
        g_BoulderThrow_MeshDrawHandle = BattleFx_SpecialBoulderThrowDrawPsxDisplacedMesh(&g_BoulderThrowMeshDrawDescriptor, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, (BattleFxPsxPrimitive *)g_BoulderThrow_MeshDrawHandle, (struct Vec3sPacked *)&g_BoulderThrow_DisplacementPositions, &frame.viewMatrix, (struct BattleFxOffsetTriMeshDesc *)&frame.yawMatrix);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.shard->timer < 20) {
        frame.shard->posX += frame.shard->velocityX;
        frame.shard->posY += frame.shard->velocityY;
        frame.shard->posZ += frame.shard->velocityZ;
    } else {
        BattleFx_SpecialBoulderThrowAdvanceMeshDisplacements();
    }
    (frame.shard->timer)++;
    if (frame.shard->timer == 28) {
        frame.shard->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws displaced PSX mesh triangle streams with per-fragment centroid offset.
 */
// FUNCTION: FF7 0x004562ca
int __cdecl BattleFx_SpecialBoulderThrowDrawPsxDisplacedMesh(BattleFxPsxDisplacedMeshDesc *pDescriptor, int reserved, int spare, BattleFxPsxPrimitive *spareC, Vec3sPacked *pDisplacements, PsxMatrix *pRotMatrix, struct BattleFxOffsetTriMeshDesc *pTransMatrix)
{
    int bodyPtr;
    unsigned short renderFlags;
    Vec3sPacked *vertexPointerB;
    int padWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short colour;
    int capFlags[2];
    BattleFxPsxTexturedQuad *quadPointer;
    int j;
    void *matrixOut;
    int padLocal; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int bytes;
    int padBlock; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    Vec3sPacked *vertA;
    Vec3sPacked *vertC;
    int total;
    BattleFxPsxPrimitive *primitive;
    YasuiBoulderThrowTriOffsets *offsetsPtr;
    BattleFxPsxPrimitive *renderStatus;

    offsetsPtr = (YasuiBoulderThrowTriOffsets *)Gfx_GetPsxScratchpadPtr(0xC0);
    matrixOut = (char *)offsetsPtr + 0x28;
    bodyPtr = pDescriptor->meshData + 4;
    quadPointer = (BattleFxPsxTexturedQuad *)(bodyPtr + *(int *)pDescriptor->meshData);
    renderStatus = (BattleFxPsxPrimitive *)Gfx_GetPsxPacketScratchPtr();
    total = quadPointer->flags;
    quadPointer = (BattleFxPsxTexturedQuad *)((char *)quadPointer + 2);
    renderFlags = quadPointer->flags & 0x19F | pDescriptor->polyFlags;
    quadPointer = (BattleFxPsxTexturedQuad *)((char *)quadPointer + 2);
    vertA = (Vec3sPacked *)(bodyPtr + quadPointer->flags);
    vertexPointerB = (Vec3sPacked *)(bodyPtr + quadPointer->vertex1);
    vertC = (Vec3sPacked *)(bodyPtr + quadPointer->vertex2);
    for (j = 0; j < total; j++) {
        offsetsPtr->centerX = (vertA->vx + vertexPointerB->vx + vertC->vx) / 3;
        offsetsPtr->centerY = (vertA->vy + vertexPointerB->vy + vertC->vy) / 3;
        offsetsPtr->centerZ = (vertA->vz + vertexPointerB->vz + vertC->vz) / 3;
        offsetsPtr->rel0X = vertA->vx - offsetsPtr->centerX;
        offsetsPtr->rel0Y = vertA->vy - offsetsPtr->centerY;
        offsetsPtr->rel0Z = vertA->vz - offsetsPtr->centerZ;
        offsetsPtr->rel1X = vertexPointerB->vx - offsetsPtr->centerX;
        offsetsPtr->rel1Y = vertexPointerB->vy - offsetsPtr->centerY;
        offsetsPtr->rel1Z = vertexPointerB->vz - offsetsPtr->centerZ;
        offsetsPtr->rel2X = vertC->vx - offsetsPtr->centerX;
        offsetsPtr->rel2Y = vertC->vy - offsetsPtr->centerY;
        offsetsPtr->rel2Z = vertC->vz - offsetsPtr->centerZ;
        pTransMatrix->transX = offsetsPtr->centerX + pDisplacements->vx;
        pTransMatrix->transY = offsetsPtr->centerY + pDisplacements->vy;
        pTransMatrix->transZ = offsetsPtr->centerZ + pDisplacements->vz;
        pDisplacements = (Vec3sPacked *)((char *)pDisplacements + 8);
        MultiplyMatrix(pRotMatrix, pTransMatrix, matrixOut);
        SetRotMatrix(matrixOut);
        SetTransMatrix(matrixOut);
        RefreshGlobalTransformState();
        StoreDeferredDrawArgs(offsetsPtr, (char *)offsetsPtr + 8, (char *)offsetsPtr + 0x10);
        ApplyDeferredDrawArgs();
        vertA = (Vec3sPacked *)(bodyPtr + quadPointer->offsetC);
        vertexPointerB = (Vec3sPacked *)(bodyPtr + quadPointer->offsetD);
        vertC = (Vec3sPacked *)(bodyPtr + quadPointer[1].flags);
        Gfx_PsxStoreClearFlag(capFlags);
        if ((capFlags[0] & 0x60000) == 0) {
            renderStatus->tag = 0x7000000;
            Gfx_ReadTransformOrderingDepth2(&bytes);
            renderStatus->colorWord = quadPointer->colorWord;
            renderStatus->uvWord0 = quadPointer->uvWord0;
            renderStatus->uvWord1 = quadPointer->uvWord1;
            renderStatus->renderFlags = renderFlags;
            if ((pDescriptor->flags & 0x80) != 0) {
                colour = quadPointer->vertex3 & 0xFF;
                renderStatus->colorCommand = colour << 0x10 | 0x24000000 | colour << 8 | colour;
            } else {
                colour = pDescriptor->shade;
                renderStatus->colorCommand = colour << 0x10 | 0x24000000 | colour << 8 | colour;
            }
            renderStatus->gpuCode |= (char)((pDescriptor->flags & 8) != 0 ? 2 : 0);
            Gfx_DrawPsxPrimitive(renderStatus);
        }
        quadPointer = (BattleFxPsxTexturedQuad *)((char *)quadPointer + 0x10);
    }
    quadPointer = (BattleFxPsxTexturedQuad *)((char *)quadPointer + 4);
    total = quadPointer->flags;
    quadPointer = (BattleFxPsxTexturedQuad *)((char *)quadPointer + 4);
    primitive = (BattleFxPsxPrimitive *)renderStatus;
    vertA = (Vec3sPacked *)(bodyPtr + quadPointer->flags);
    vertexPointerB = (Vec3sPacked *)(bodyPtr + quadPointer->vertex1);
    vertC = (Vec3sPacked *)(bodyPtr + quadPointer->vertex2);
    for (j = 0; j < total; j++) {
        offsetsPtr->centerX = (vertA->vx + vertexPointerB->vx + vertC->vx) / 3;
        offsetsPtr->centerY = (vertA->vy + vertexPointerB->vy + vertC->vy) / 3;
        offsetsPtr->centerZ = (vertA->vz + vertexPointerB->vz + vertC->vz) / 3;
        offsetsPtr->rel0X = vertA->vx - offsetsPtr->centerX;
        offsetsPtr->rel0Y = vertA->vy - offsetsPtr->centerY;
        offsetsPtr->rel0Z = vertA->vz - offsetsPtr->centerZ;
        offsetsPtr->rel1X = vertexPointerB->vx - offsetsPtr->centerX;
        offsetsPtr->rel1Y = vertexPointerB->vy - offsetsPtr->centerY;
        offsetsPtr->rel1Z = vertexPointerB->vz - offsetsPtr->centerZ;
        offsetsPtr->rel2X = vertC->vx - offsetsPtr->centerX;
        offsetsPtr->rel2Y = vertC->vy - offsetsPtr->centerY;
        offsetsPtr->rel2Z = vertC->vz - offsetsPtr->centerZ;
        pTransMatrix->transX = offsetsPtr->centerX + pDisplacements->vx;
        pTransMatrix->transY = offsetsPtr->centerY + pDisplacements->vy;
        pTransMatrix->transZ = offsetsPtr->centerZ + pDisplacements->vz;
        pDisplacements = (Vec3sPacked *)((char *)pDisplacements + 8);
        MultiplyMatrix(pRotMatrix, pTransMatrix, matrixOut);
        SetRotMatrix(matrixOut);
        SetTransMatrix(matrixOut);
        RefreshGlobalTransformState();
        StoreDeferredDrawArgs(offsetsPtr, (char *)offsetsPtr + 8, (char *)offsetsPtr + 0x10);
        ApplyDeferredDrawArgs();
        vertA = (Vec3sPacked *)(bodyPtr + quadPointer[1].flags);
        vertexPointerB = (Vec3sPacked *)(bodyPtr + quadPointer[1].vertex1);
        vertC = (Vec3sPacked *)(bodyPtr + quadPointer[1].vertex2);
        Gfx_PsxStoreClearFlag(capFlags);
        if ((capFlags[0] & 0x60000) == 0) {
            primitive->tag = 0x6000000;
            Gfx_ReadTransformOrderingDepth2(&bytes);
            Gfx_ShadePsxTriVertexColors((CVECTOR *)((char *)quadPointer + 8), (CVECTOR *)((char *)quadPointer + 0xC), (CVECTOR *)((char *)quadPointer + 0x10), pDescriptor->shade, (CVECTOR *)((int)primitive + 4), (CVECTOR *)((int)primitive + 0xC), (CVECTOR *)((int)primitive + 0x14));
            primitive->gpuCode = ((pDescriptor->flags & 8) != 0 ? 2 : 0) + 0x30;
            Gfx_DrawPsxPrimitive(primitive);
        }
        quadPointer++;
    }
    return (int)primitive;
}

/*
 * Draws textured and Gouraud triangle/quad streams from PSX geometry data under installed transform.
 */
// FUNCTION: FF7 0x004569b0
int __cdecl BattleFx_SpecialBoulderThrowDrawPsxMesh(BattleFxPsxMeshDescriptor *pDescriptor, int orderingTable, int depthBias, int listHead)
{
    int padSlot; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    unsigned short texturePage;
    int tableBase;
    BattleFxBoulderThrowPrimitive *pGouraudTri;
    short brightness;
    int facing;
    BattleFxBoulderThrowPrimitive *flatQuadPtr;
    int padLocal; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxBoulderThrowPrimitive *texTri;
    int orderDepth;
    BattleFxBoulderThrowPrimitive *texQuad;
    int va;
    int vb;
    int vertC;
    int padSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int primCnt;
    int gteBits;
    YasuiBoulderThrowMeshStream *streamPtr;
    int faceIndex;

    tableBase = pDescriptor->table + 4;
    streamPtr = (YasuiBoulderThrowMeshStream *)(tableBase + *(int *)pDescriptor->table);
    texTri = (BattleFxBoulderThrowPrimitive *)Gfx_GetPsxPacketScratchPtr();
    primCnt = streamPtr->vertexWord;
    streamPtr = (YasuiBoulderThrowMeshStream *)((char *)streamPtr + 2);
    texturePage = streamPtr->vertexWord & 0x19F | pDescriptor->pageBits;
    streamPtr = (YasuiBoulderThrowMeshStream *)((char *)streamPtr + 2);
    va = tableBase + streamPtr->vertexWord;
    vb = tableBase + streamPtr->vertexOffsetB;
    vertC = tableBase + streamPtr->vertexOffsetC;
    for (faceIndex = 0; faceIndex < primCnt; faceIndex++) {
        StoreDeferredDrawArgs((void *)va, (void *)vb, (void *)vertC);
        ApplyDeferredDrawArgs();
        va = tableBase + streamPtr->vertexOffsetA;
        vb = tableBase + streamPtr->colorVertexB;
        vertC = tableBase + streamPtr->sharedVertexOffset;
        Gfx_PsxStoreClearFlag(&gteBits);
        if ((gteBits & 0x60000) == 0) {
            Gfx_PsxStoreFrontFacing(&facing);
            if (facing > 0) {
                texTri->tag = 0x7000000;
                Gfx_ReadTransformOrderingDepth2(&orderDepth);
                texTri->linkWord = streamPtr->primWord;
                texTri->uvWordA = streamPtr->clutWord;
                texTri->uvWordB = streamPtr->tpageWord;
                texTri->tpage = texturePage;
                if ((pDescriptor->flags & 0x80) != 0) {
                    brightness = streamPtr->resourceOffset & 0xFF;
                    texTri->colorCommand = brightness << 0x10 | 0x24000000 | brightness << 8 | brightness;
                } else {
                    texTri->colorCommand = 0x24808080;
                }
                Gfx_DrawPsxPrimitive(texTri);
            }
        }
        streamPtr = (YasuiBoulderThrowMeshStream *)((char *)streamPtr + 0x10);
    }
    primCnt = streamPtr->vertexWord;
    streamPtr = (YasuiBoulderThrowMeshStream *)((char *)streamPtr + 4);
    texQuad = texTri;
    va = tableBase + streamPtr->vertexWord;
    vb = tableBase + streamPtr->vertexOffsetB;
    vertC = tableBase + streamPtr->vertexOffsetC;
    for (faceIndex = 0; faceIndex < primCnt; faceIndex++) {
        StoreDeferredDrawArgs((void *)va, (void *)vb, (void *)vertC);
        ApplyDeferredDrawArgs();
        va = tableBase + streamPtr->sharedVertexOffset;
        vb = tableBase + streamPtr->vertexOffsetB2;
        vertC = tableBase + streamPtr->sharedVertexOffset2;
        Gfx_PsxStoreClearFlag(&gteBits);
        if ((gteBits & 0x60000) == 0) {
            Gfx_PsxStoreFrontFacing(&facing);
            if (facing >= 0) {
                SetPendingResourceHandle((GfxPsxShortVertexSlot *)(tableBase + streamPtr->resourceOffset));
                ApplyPendingResourceHandle();
                Gfx_ReadTransformOrderingDepth2(&orderDepth);
                texQuad->tag = 0x9000000;
                texQuad->linkWord = streamPtr->primWord;
                texQuad->uvWordA = streamPtr->clutWord;
                texQuad->uvWordB = streamPtr->tpageWord;
                texQuad->vertexOffset = streamPtr->vertexOffsetA;
                texQuad->tpage = texturePage;
                if ((pDescriptor->flags & 0x80) != 0) {
                    brightness = streamPtr->colorVertexB & 0xFF;
                    texQuad->colorCommand = brightness << 0x10 | 0x2C000000 | brightness << 8 | brightness;
                } else {
                    texQuad->colorCommand = 0x2C808080;
                }
                Gfx_DrawPsxPrimitive(texQuad);
            }
        }
        streamPtr = (YasuiBoulderThrowMeshStream *)((char *)streamPtr + 0x14);
    }
    primCnt = streamPtr->vertexWord;
    streamPtr = (YasuiBoulderThrowMeshStream *)((char *)streamPtr + 4);
    pGouraudTri = texQuad;
    va = tableBase + streamPtr->vertexWord;
    vb = tableBase + streamPtr->vertexOffsetB;
    vertC = tableBase + streamPtr->vertexOffsetC;
    for (faceIndex = 0; faceIndex < primCnt; faceIndex++) {
        StoreDeferredDrawArgs((void *)va, (void *)vb, (void *)vertC);
        ApplyDeferredDrawArgs();
        va = tableBase + streamPtr->sharedVertexOffset;
        vb = tableBase + streamPtr->vertexOffsetB2;
        vertC = tableBase + streamPtr->sharedVertexOffset2;
        Gfx_PsxStoreClearFlag(&gteBits);
        if ((gteBits & 0x60000) == 0) {
            Gfx_PsxStoreFrontFacing(&facing);
            if (facing >= 0) {
                pGouraudTri->tag = 0x6000000;
                Gfx_ReadTransformOrderingDepth2(&orderDepth);
                pGouraudTri->colorCommand = streamPtr->primWord;
                pGouraudTri->linkWord = ((BattleFxBoulderThrowLinkRecord *)streamPtr)->linkWord;
                ((BattleFxBoulderThrowPacket *)pGouraudTri)->linkWord = ((BattleFxBoulderThrowLinkRecord *)streamPtr)->primWord;
                Gfx_DrawPsxPrimitive(pGouraudTri);
            }
        }
        streamPtr = (YasuiBoulderThrowMeshStream *)((char *)streamPtr + 0x14);
    }
    primCnt = streamPtr->vertexWord;
    streamPtr = (YasuiBoulderThrowMeshStream *)((char *)streamPtr + 4);
    flatQuadPtr = pGouraudTri;
    va = tableBase + streamPtr->vertexWord;
    vb = tableBase + streamPtr->vertexOffsetB;
    vertC = tableBase + streamPtr->vertexOffsetC;
    for (faceIndex = 0; faceIndex < primCnt; faceIndex++) {
        StoreDeferredDrawArgs((void *)va, (void *)vb, (void *)vertC);
        ApplyDeferredDrawArgs();
        va = tableBase + streamPtr->sharedVertexOffset2;
        vb = tableBase + streamPtr->quadOffset;
        vertC = tableBase + streamPtr->vertCOffset;
        Gfx_PsxStoreClearFlag(&gteBits);
        if ((gteBits & 0x60000) == 0) {
            Gfx_PsxStoreFrontFacing(&facing);
            if (facing >= 0) {
                SetPendingResourceHandle((GfxPsxShortVertexSlot *)(tableBase + streamPtr->resourceOffset));
                ApplyPendingResourceHandle();
                Gfx_ReadTransformOrderingDepth2(&orderDepth);
                flatQuadPtr->tag = 0x8000000;
                flatQuadPtr->colorCommand = streamPtr->primWord;
                flatQuadPtr->linkWord = ((BattleFxBoulderThrowLinkRecord *)streamPtr)->linkWord;
                ((BattleFxBoulderThrowPacket *)flatQuadPtr)->linkWord = ((BattleFxBoulderThrowLinkRecord *)streamPtr)->primWord;
                ((BattleFxBoulderThrowPacket *)flatQuadPtr)->color3 = ((BattleFxBoulderThrowLinkRecord *)streamPtr)->color3;
                Gfx_DrawPsxPrimitive(flatQuadPtr);
            }
        }
        streamPtr = (YasuiBoulderThrowMeshStream *)((char *)streamPtr + 0x18);
    }
    return (int)flatQuadPtr;
}

/*
 * Integrates displacement velocity vectors into displacement positions across all 20 mesh pieces.
 */
// FUNCTION: FF7 0x00456fa7
void __cdecl BattleFx_SpecialBoulderThrowAdvanceMeshDisplacements(void)
{
    short i;

    for (i = 0; i < 20; i++) {
        g_BoulderThrow_DisplacementPosArray[i].offset += g_BoulderThrow_DisplacementVelArray[i].velocityX;
        g_BoulderThrow_DisplacementPosArray[i].offsetY += g_BoulderThrow_DisplacementVelArray[i].velocityY;
        g_BoulderThrow_DisplacementPosArray[i].offsetZ += g_BoulderThrow_DisplacementVelArray[i].velocityZ;
    }
}
} /* extern "C" */
