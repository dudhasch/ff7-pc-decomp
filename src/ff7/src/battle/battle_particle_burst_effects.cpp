// src/ff7/src/battle/battle_particle_burst_effects.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Particle burst effects: it ensures the particle resource exists, spawns bone-attached
//   bursts, and runs their per-frame tasks that draw tinted quads and bouncing or attached models.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "ff7/battle_tasks.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include "ff7/battle.h"
#include "lib/crt.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
extern "C" {

struct GfxPsxSpriteQuadState;
struct BattleFxModelDraw;

// clang-format off
// GLOBAL: FF7 0x007c32d0
unsigned int g_BattleFxParticleBurstAssetRecords[10] ={
    0x001900b1,
    (unsigned int)&g_BattleFxParticleBurstJoTamRsdLeaf,
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x007c3384
    (unsigned int)"JO_FIR.RSD",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x007c32f8
unsigned char g_BattleFxParticleBurstModel0DrawRequest[16] = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007c3308
unsigned char g_BattleFxParticleQuadVertex0[8] = {0x18, 0xfc, 0xfe, 0xff, 0x18, 0xfc, 0x00, 0x00};
// GLOBAL: FF7 0x007c3310
struct SVECTOR g_BattleFxParticleQuadVertex1 = {1000, -2, -1000, 0};
// GLOBAL: FF7 0x007c3318
struct SVECTOR g_BattleFxParticleQuadVertex2 = {-1000, -2, 1000, 0};
// GLOBAL: FF7 0x007c3320
struct SVECTOR g_BattleFxParticleQuadVertex3 = {1000, -2, 1000, 0};
// GLOBAL: FF7 0x007c3328
struct BattleFxParticleQuadTransformMatrix g_BattleFxParticleQuadTransformMatrix = {{0, 0x20, 0, 0, 0, 0, 0, 0, 0, 0x20, 0, 0, 0, 0, 0, 0, 0, 0x20}, 0, {0}, 0};
// GLOBAL: FF7 0x007c3348
struct BattleFxParticleAttachmentOffsetMatrix g_BattleFxParticleAttachmentOffsetMatrix = {{0, 0x10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x10, 0, 0, 0, 0xf0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0};
// GLOBAL: FF7 0x007c3368
unsigned char g_BattleFxParticleBurstModel1DrawRequest[16] = {0x00, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007c3378
unsigned char g_BattleFxParticleBurstJoTamRsdLeaf[11] = "JO_TAM.RSD";
// clang-format on


typedef struct BattleParticleRotation {
    short pitch;
    short yaw;
} BattleParticleRotation;

// clang-format off
// GLOBAL: FF7 0x009aeaf0
int g_BattleParticleDrawCursor = 0;
// GLOBAL: FF7 0x009aeaf4
struct GfxTableCursor *g_BattleParticleResource = 0;
// GLOBAL: FF7 0x009aeaf8
BattleParticleRotation g_BattleFxParticleQuadRotation = {0};
unsigned char g_bssPad_009aeafc[4] = {0};
// clang-format on

/*
 * When the battle scene exists and g_BattleParticleResource is zero, initializes a default-duration resource
 * record with type zero, builds MAGIC/MGUN into the local path buffer, and calls
 * Gfx_LoadModelTableIfAbsent with resource ID 2, g_BattleFxParticleBurstAssetRecords metadata, the global output slot and
 * scene context at 0x4C. Otherwise does nothing.
 */
// FUNCTION: FF7 0x004390f0
void __cdecl Battle_EnsureParticleResource(void)
{
    GfxModelTableBank *sceneContext;
    GfxResourceRequest requestRecord;
    int resultId;
    char pathBuffer[200];
    GfxResourceRequest *request;
    int assetRecords;

    if (g_pBattle3dScene != 0 && g_BattleParticleResource == 0) {
        resultId = 2;
        assetRecords = (int)&g_BattleFxParticleBurstAssetRecords;
        sceneContext = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestRecord;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x007c3390
        (request->resourcePath) = BattleFx_BuildResourcePath("MAGIC/MGUN", pathBuffer);
        Gfx_LoadModelTableIfAbsent(resultId, assetRecords, request, &g_BattleParticleResource, sceneContext);
    }
}

/*
 * Allocates a secondary 0x20-byte resource slot for Battle_ParticleBurstQuadTask, copies eight bytes from pPos to
 * slot+8, and sets its first word to 1.
 */
// FUNCTION: FF7 0x00439180
void __cdecl Battle_SpawnParticleBurstDarkQuad(Int64Pair *pPos)
{
    int task;

    task = (int)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(Battle_ParticleBurstQuadTask)];
    *(Int64Pair *)((char *)task + 8) = *pPos;
    *(short *)task = 1;
}

/*
 * Draws one tinted PSX quad for the effect record at g_BattleTaskSlotBuffer.secondarySlots +
 * g_BattleSecondaryTaskActiveSlotIdx * 32 and retires it. A random y rotation (rand() & 0x3FF) goes
 * into the matrix at g_BattleFxParticleQuadTransformMatrix with the record's +8 and +0xC as the
 * translation, composed with g_BattleCameraTransform and installed; opcode 0xB is set to one on
 * Battle3d_GetSceneRenderObjectHigh(1, g_pBattle3dScene); the four corners at
 * g_BattleFxParticleQuadVertex0 to g_BattleFxParticleQuadVertex3 are projected through
 * Gfx_TransformQuadReturnDepth into the primitive at g_BattleParticleDrawCursor, which gets code
 * 0x09, colour 0x2E808080 (0x2E202020 when the record's state at +0 is non-zero) and fixed texture
 * and CLUT words, and is submitted when the projection helper returns a positive value. The
 * record's state is 0xFFFF afterwards either way.
 */
// FUNCTION: FF7 0x004391bc
void __cdecl Battle_ParticleBurstQuadTask(void)
{
    int depth;
    int mtx[8];
    int screenY;
    BattleParticleBurstQuad *taskPtr;
    BattleParticleQuad *prim;
    int projA;

    taskPtr = (BattleParticleBurstQuad *)(&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx]);
    g_BattleFxParticleQuadRotation.yaw = rand() & 0x3FF;
    RotMatrix((SVECTOR *)&g_BattleFxParticleQuadRotation, &g_BattleFxParticleQuadTransformMatrix);
    g_BattleFxParticleQuadTransformMatrix.transX = taskPtr->translationX;
    g_BattleFxParticleQuadTransformMatrix.transZ = taskPtr->translationZ;
    MultiplyMatrix((void *)g_BattleCameraTransform.matrix, (int *)&g_BattleFxParticleQuadTransformMatrix, mtx);
    SetRotMatrix(mtx);
    SetTransMatrix(mtx);
    RefreshGlobalTransformState();
    Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)Battle3d_GetSceneRenderObjectHigh(1, g_pBattle3dScene));
    prim = (BattleParticleQuad *)g_BattleParticleDrawCursor;
    depth = Gfx_TransformQuadReturnDepth(&g_BattleFxParticleQuadVertex0, &g_BattleFxParticleQuadVertex1, &g_BattleFxParticleQuadVertex2, &g_BattleFxParticleQuadVertex3, &projA, &screenY);
    if (depth > 0) {
        prim->tag = 0x9000000;
        if (taskPtr->status == 0) {
            prim->colorCode = 0x2E808080;
        } else {
            prim->colorCode = 0x2E202020;
        }
        prim->clut = 0x78C7;
        prim->tpage = 0x3A;
        prim->uv0 = 0xC000;
        prim->uv1 = 0xC03F;
        prim->uv2 = 0xFF00;
        prim->uv3 = 0xFF3F;
        Gfx_DrawPsxPrimitive(prim);
    }
    taskPtr->status = 0xFFFF;
}

// Spawns a bone particle burst. Points g_BattleParticleDrawCursor at g_GfxPsxPacketScratch, ensures
// the particle resources, and allocates a Battle_ParticleBurstTask secondary slot. The slot
// receives burstType, actorIndex, boneIndex, zOffset, delay and duration.
// FUNCTION: FF7 0x0043930d
void __cdecl Battle_SpawnBoneParticleBurst(int burstType, int actorIdx, int boneIdx, int zOffset, int delay, int duration)
{
    BattleBoneParticleBurst *task;

    g_BattleParticleDrawCursor = (int)&g_GfxPsxPacketScratch;
    Battle_EnsureParticleResource();
    task = (BattleBoneParticleBurst *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(Battle_ParticleBurstTask)];
    task->burstType = burstType;
    task->actorIndex = actorIdx;
    task->boneIndex = boneIdx;
    task->zOffset = zOffset;
    task->delay = delay;
    task->duration = duration;
}

/*
 * Returns while paused. For the current secondary particle emitter waits until its signed counter
 * reaches the delay, then emits while elapsed is below the duration with mask 0x80 removed. When
 * g_BattleDrawFrameParity is zero schedules Battle_ParticleBurstAttachedModelTask and copies actor/effect parameters; unless duration
 * mask 0x80 is set also schedules Battle_ParticleBurstBouncingModelTask at a bone or auxiliary-node position, generates
 * three random velocity shorts, rotates them by actor rotation and saves yaw, then emits a world
 * sprite. At elapsed duration retires the emitter; increments its counter on every unpaused
 * invocation including wait, suppression and retirement.
 */
// FUNCTION: FF7 0x0043937f
void __cdecl Battle_ParticleBurstTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[52];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxParticleBurst *spawnSlot; };
        struct { unsigned char pad_to_24[0x24]; SVECTOR velX; };
        struct { unsigned char pad_to_2C[0x2C]; BattleParticleBurst *emitter; };
        struct { unsigned char pad_to_30[0x30]; int elapsed; };
    } frame;
#pragma pack(pop)

    frame.emitter = (BattleParticleBurst *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.elapsed = frame.emitter->tick;
    if (frame.elapsed < frame.emitter->delay) { /* empty first arm: lib/codegen.h */
    } else {
        frame.elapsed -= frame.emitter->delay;
        if (frame.elapsed < (frame.emitter->duration & ~0x80)) {
            if (g_BattleDrawFrameParity == 0) {
                frame.spawnSlot = (BattleFxParticleBurst *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(Battle_ParticleBurstAttachedModelTask)];
                frame.spawnSlot->actorId = frame.emitter->actorIndex;
                frame.spawnSlot->burstType = frame.emitter->burstType;
                frame.spawnSlot->attachIndex = frame.emitter->attachBoneIndex;
                frame.spawnSlot->offsetZ = frame.emitter->zOffset;
                if ((frame.emitter->duration & 0x80) == 0) {
                    frame.spawnSlot = (BattleFxParticleBurst *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(Battle_ParticleBurstBouncingModelTask)];
                    if (frame.emitter->attachBoneIndex != -1) {
                        Battle_GetBoneWorldPos(frame.emitter->actorIndex, frame.emitter->attachBoneIndex, (short *)((int)frame.spawnSlot + 8));
                    } else {
                        Battle_WorldPointToCameraLocalDup((struct BattleCameraWorldPointScratch *)((frame.emitter->actorIndex << 6) + (int)g_BattleActorTransformRecords[0].auxNode), (BattleCameraLocalPoint2 *)((int)frame.spawnSlot + 8));
                    }
                    frame.velX.vx = 0xFFFFFFC4 - (rand() & 0xF);
                    frame.velX.vy = (rand() & 0x1F) - 0x96;
                    frame.velX.vz = (rand() & 0xF) + 0x14;
                    RotMatrix((SVECTOR *)&g_BattleActorRecords[frame.emitter->actorIndex].rotX, &frame.rotMatrix);
                    TransformVectorToInt16(&frame.rotMatrix, &frame.velX, (char *)frame.spawnSlot + 0x10);
                    frame.spawnSlot->actorId = frame.emitter->actorIndex;
                    frame.spawnSlot->ownerSlot = 0;
                    frame.spawnSlot->yaw = g_BattleActorRecords[frame.emitter->actorIndex].yaw;
                    Battle_SpawnWorldSpriteTask((Vec3sPacked *)((int)frame.spawnSlot + 8), 0x400, 0x800);
                }
            }
        } else {
            frame.emitter->endFlag = 0xFFFF;
        }
    }
    (frame.emitter->tick)++;
}

/*
 * Draws resource field zero at the current secondary particle position with its saved yaw,
 * composing the transform with g_BattleCameraTransform and submitting before the pause check. While unpaused
 * advances position by velocity and adds thirty to vertical velocity. On nonnegative Y
 * quarters/inverts the vertical position/velocity, adds three random velocity terms, increments
 * its bounce short and retires only when that short equals two, then emits a world sprite even on
 * that retiring bounce.
 */
// FUNCTION: FF7 0x00439592
void __cdecl Battle_ParticleBurstBouncingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[44];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleParticleBounceSlot *particleSlot; };
    } frame;
#pragma pack(pop)

    frame.particleSlot = (BattleParticleBounceSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    frame.rotX.vz = 0;
    frame.rotX.vx = frame.rotX.vz;
    frame.rotX.vy = frame.particleSlot->yaw;
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.particleSlot->posX;
    frame.rotMatrix.t[1] = frame.particleSlot->posY;
    frame.rotMatrix.t[2] = frame.particleSlot->posZ;
    MultiplyMatrix((void *)g_BattleCameraTransform.matrix, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    *(int *)g_BattleFxParticleBurstModel0DrawRequest = Gfx_ModelTableGetRenderObject(0, g_BattleParticleResource);
    g_BattleParticleDrawCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleFxParticleBurstModel0DrawRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleParticleDrawCursor);
    if (g_BattleIsPaused == 0) {
        frame.particleSlot->posX += frame.particleSlot->velX;
        frame.particleSlot->posY += frame.particleSlot->velY;
        frame.particleSlot->posZ += frame.particleSlot->velZ;
        frame.particleSlot->velY += 0x1E;
        if (frame.particleSlot->posY >= 0) {
            frame.particleSlot->velY = (-frame.particleSlot->velY >> 2) - (rand() & 0x1F);
            frame.particleSlot->posY = -frame.particleSlot->posY >> 2;
            frame.particleSlot->velX = (frame.particleSlot->velX >> 2) + (rand() & 0x1F) + -0x10;
            frame.particleSlot->velZ = (frame.particleSlot->velZ >> 2) + (rand() & 0x1F) + -0x10;
            (frame.particleSlot->bounces)++;
            if (frame.particleSlot->bounces == 2) {
                frame.particleSlot->bounces = 0xFFFF;
            }
            Battle_SpawnWorldSpriteTask((Vec3sPacked *)((int)frame.particleSlot + 8), 0x200, 0x400);
        }
    }
}

/*
 * Retires the current secondary slot when unpaused and its counter is nonzero. Otherwise builds the
 * retained transform from the actor attachment matrix or auxiliary actor node, with a negated saved
 * short in g_BattleFxParticleAttachmentOffsetMatrix.transZ, selects resource field one and submits a model. While unpaused schedules
 * Battle_ParticleBurstQuadTask, converts its composed translation relative to
 * g_BattleCameraTransform.translation/46/4A through the transposed view matrix, initializes the new
 * slot and increments the original counter. Paused calls continue drawing without allocating or
 * advancing.
 */
// FUNCTION: FF7 0x00439774
void __cdecl Battle_ParticleBurstAttachedModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        int attachMatrix;
        struct { unsigned char pad_to_20[0x20]; GfxPsxMatrixTransposed *actorMatrix; };
        struct { unsigned char pad_to_24[0x24]; BattleParticleAttachedModel *quadSlot; };
        struct { unsigned char pad_to_28[0x28]; BattleFxParticleBurst *burstSlot; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix composedMatrix; };
    } frame;
#pragma pack(pop)

    frame.burstSlot = (BattleFxParticleBurst *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    if (g_BattleIsPaused == 0 && frame.burstSlot->timer != 0) {
        frame.burstSlot->ownerSlot = BATTLE_TASK_FREE;
    } else {
        g_BattleFxParticleAttachmentOffsetMatrix.transZ = -frame.burstSlot->offsetZ;
        if (frame.burstSlot->attachIndex != -1) {
            frame.actorMatrix = (GfxPsxMatrixTransposed *)&frame.attachMatrix;
            Battle3d_GetActorAttachMatrix(frame.burstSlot->actorId, frame.burstSlot->attachIndex, (PsxMatrix *)frame.actorMatrix);
        } else {
            frame.actorMatrix = (GfxPsxMatrixTransposed *)((frame.burstSlot->actorId << 6) + (int)g_BattleActorTransformRecords[0].auxNode);
        }
        MultiplyMatrix(frame.actorMatrix, (int *)&g_BattleFxParticleAttachmentOffsetMatrix, &frame.composedMatrix);
        SetRotMatrix(&frame.composedMatrix);
        SetTransMatrix(&frame.composedMatrix);
        *(int *)g_BattleFxParticleBurstModel1DrawRequest = Gfx_ModelTableGetRenderObject(1, g_BattleParticleResource);
        g_BattleParticleDrawCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleFxParticleBurstModel1DrawRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleParticleDrawCursor);
        if (g_BattleIsPaused == 0) {
            frame.quadSlot = (BattleParticleAttachedModel *)(&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(Battle_ParticleBurstQuadTask)]);
            frame.quadSlot->relX = frame.composedMatrix.t[0] - (g_BattleCameraTransform.translation[0]);
            frame.quadSlot->relY = frame.composedMatrix.t[1] - (g_BattleCameraTransform.translation[1]);
            frame.quadSlot->relZ = frame.composedMatrix.t[2] - (g_BattleCameraTransform.translation[2]);
            TransposeMatrix((void *)g_BattleCameraTransform.matrix, &frame.composedMatrix);
            TransformVectorToInt16(&frame.composedMatrix, (char *)frame.quadSlot + 8, (char *)frame.quadSlot + 8);
            frame.quadSlot->status = 0;
            (frame.burstSlot->timer)++;
        }
    }
}

/*
 * Forwards arguments to Battle_SpawnBoneParticleBurst, adding indexed byte at
 * g_BattleActorRecords[0].primaryBone and word at g_BattleActorRecords[0].burstOffsetA.
 */
// FUNCTION: FF7 0x00439907
void __cdecl Battle_SpawnBoneParticleBurstA(int burstType, int actorIdx, int delay, int duration)
{
    Battle_SpawnBoneParticleBurst(burstType, actorIdx, g_BattleActorRecords[actorIdx].primaryBone, g_BattleActorRecords[actorIdx].burstOffsetA, delay, duration);
}

/*
 * Forwards arguments to Battle_SpawnBoneParticleBurst, adding indexed byte at
 * g_BattleActorRecords[0].secondaryBone and word at g_BattleActorRecords[0].burstOffsetB.
 */
// FUNCTION: FF7 0x00439946
void __cdecl Battle_SpawnBoneParticleBurstB(int burstType, int actorIdx, int delay, int duration)
{
    Battle_SpawnBoneParticleBurst(burstType, actorIdx, g_BattleActorRecords[actorIdx].secondaryBone, g_BattleActorRecords[actorIdx].burstOffsetB, delay, duration);
}

/*
 * Calls Battle_SpawnBoneParticleBurst twice for actorIdx, once per bone: using
 * g_BattleActorRecords[0].primaryBone/g_BattleActorRecords[0].burstOffsetA and
 * g_BattleActorRecords[0].secondaryBone/g_BattleActorRecords[0].burstOffsetB.
 */
// FUNCTION: FF7 0x00439985
void __cdecl Battle_SpawnBoneParticleBurstBothBones(int burstType, int actorIdx, int delay, int duration)
{
    Battle_SpawnBoneParticleBurst(burstType, actorIdx, g_BattleActorRecords[actorIdx].primaryBone, g_BattleActorRecords[actorIdx].burstOffsetA, delay, duration);
    Battle_SpawnBoneParticleBurst(burstType, actorIdx, g_BattleActorRecords[actorIdx].secondaryBone, g_BattleActorRecords[actorIdx].burstOffsetB, delay, duration);
}
} /* extern "C" */
