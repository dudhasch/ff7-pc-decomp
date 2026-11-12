// src/ff7/src/battle/battle_fx_magic_hitmark.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle visual effect for the Hitmark magic: a particle pool of bone bursts and radial
//   triangle bursts at a ray-sphere impact point, with an impact sprite.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_magic.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
extern "C" {

struct Vec3sPacked;

#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// GLOBAL: FF7 0x008c9710
int g_BattleMagicHitmark_ResourceDef = 76345;
// GLOBAL: FF7 0x008c9718
BattleMagicHitmarkSpriteFileDef g_BattleMagicHitmark_ResourceDefs = {
    g_BattleFxMagicHitmarkResourceNames + 0x8, {0x0, 0x0, 0x0}, 0x0, 0x0
};
// GLOBAL: FF7 0x008c972c
short g_BattleMagicHitmark_ImpactCenterZ = 1700;
// The particle is a triangle with corners (0, -50, 0), (-50, 50) and (50, 50, 0). The second corner
// is one word; TransformVertices3 reads its Z from the zero alignment word after it.
// GLOBAL: FF7 0x008c9730
SVECTOR g_BattleMagicHitmark_ParticleVertex0 = {0, -50, 0, 0};
// GLOBAL: FF7 0x008c9738
int g_BattleMagicHitmark_ParticleVertex1 = 0x0032FFCE;
// GLOBAL: FF7 0x008c9740
BattleMagicHitmarkSpriteDraw g_BattleMagicHitmark_SpriteDrawReq = {{50, 50, 0, 0}, {0, 0, (char)0x80, (char)0x80, (char)0x80, 0x2e, 0, 0x600}};
// clang-format off
// GLOBAL: FF7 0x008c9758
int g_BattleMagicHitmark_BurstScale = 512;
unsigned char g_rawData_008c975c[4] = {0x00, 0x02, 0x00, 0x00};
// clang-format on
unsigned char g_BattleFxMagicHitmarkResourceNames[20] = {0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4a, 0x4f, 0x5f, 0x43, 0x30, 0x31, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00};
// GLOBAL: FF7 0x00bd05d0
int g_BattleMagicHitmark_RayIntersectionResult = 0;
unsigned char g_bssPad_00bd05d4[4] = {0};
// GLOBAL: FF7 0x00bd05d8
SVECTOR g_BattleMagicHitmark_HitPos1 = {0};
// GLOBAL: FF7 0x00bd05e0
int g_BattleMagicHitmark_RayCoeff = 0;
// GLOBAL: FF7 0x00bd05e4
int g_BattleMagicHitmark_RayDistSq = 0;
// GLOBAL: FF7 0x00bd05e8
int g_BattleMagicHitmark_DeltaX = 0;
// GLOBAL: FF7 0x00bd05ec
int g_BattleMagicHitmark_DeltaZ = 0;
// GLOBAL: FF7 0x00bd05f0
int g_BattleMagicHitmark_PrimitivePtr = 0;
unsigned char g_bssPad_00bd05f4[4] = {0};
// GLOBAL: FF7 0x00bd05f8
struct Vec3sPacked g_BattleMagicHitmark_HitPos2 = {0};
// GLOBAL: FF7 0x00bd0600
struct BattleFxMagicHitmarkParticle g_BattleMagicHitmark_ParticlePool[600] = {0};
// GLOBAL: FF7 0x00bd5a60
short g_BattleMagicHitmark_RayDirection[3] = {0};
// GLOBAL: FF7 0x00bd5a68
struct BattleSpecMagmaDrawListHead g_BattleMagicHitmark_OriginCoords = {0};
// GLOBAL: FF7 0x00bd5a6c
short g_BattleMagicHitmark_OriginCoordZ = 0;
// GLOBAL: FF7 0x00bd5a70
struct GfxTableCursor *g_BattleMagicHitmark_LoadedRes = 0;
unsigned char g_bssPad_00bd5a74[4] = {0};
// GLOBAL: FF7 0x00bd5a78
int g_BattleMagicHitmark_TriangleCenter = 0;
unsigned char g_bssPad_00bd5a7c[4] = {0};
// GLOBAL: FF7 0x00bd5a80
struct BattleMagicHitmark_BurstAngle g_BattleMagicHitmark_BurstAngle = {0};
unsigned char g_bssPad_00bd5a84[4] = {0};
unsigned char g_bssPad_00bd5a88[4] = {0};
// GLOBAL: FF7 0x00bd5a8c
int g_BattleMagicHitmark_SoundPlayedFlag = 0;
// clang-format on


/*
 * Loads resources for the Hitmark magic effect when not already loaded.
 */
// FUNCTION: FF7 0x005a01a0
void __cdecl BattleFx_MagicLoadResHitmark(void)
{
    char pathBuf[200];
    GfxResourceRequest requestStorage;
    int kind;
    GfxModelTableBank *gfxCtx;
    GfxResourceRequest *requestPtr;
    int def;

    if (g_pBattle3dScene != 0 && g_BattleMagicHitmark_LoadedRes == 0) {
        kind = 1;
        def = (int)&g_BattleMagicHitmark_ResourceDef;
        gfxCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        requestPtr = &requestStorage;
        Gfx_InitResourceRequest(requestPtr);
        requestPtr->groupTag = 0;
        // STRING: FF7 0x008c9774
        requestPtr->resourcePath = BattleFx_BuildResourcePath("MAGIC/HITMARK", pathBuf);
        Gfx_LoadModelTableIfAbsent(kind, def, requestPtr, &g_BattleMagicHitmark_LoadedRes, gfxCtx);
    }
}

/*
 * Entry point initializing Hitmark magic effect.
 */
// FUNCTION: FF7 0x005a0230
void __cdecl BattleFx_MagicEnterHitmark(void)
{
    g_BattleMagicHitmark_PrimitivePtr = (int)&g_GfxPsxPacketScratch;
    BattleFx_MagicLoadResHitmark();
    BattleFx_MagicBeginHitmark(0, 0);
}

/*
 * Initializes particle structures and spawns bone particle bursts for Hitmark.
 */
// FUNCTION: FF7 0x005a0250
void __cdecl BattleFx_MagicBeginHitmark(int targetMask, int actorIdx)
{
    int i;

    g_BattleMagicHitmark_OriginCoords.originX = 0;
    g_BattleMagicHitmark_OriginCoords.originY = 0xFC18;
    g_BattleMagicHitmark_OriginCoordZ = 0xEE6C;
    g_BattleMagicHitmark_DeltaX = g_BattleMagicHitmark_OriginCoords.originX - g_BattleMagicHitmark_ResourceDefs.impactCenterX;
    g_BattleMagicHitmark_DeltaZ = g_BattleMagicHitmark_OriginCoordZ - g_BattleMagicHitmark_ImpactCenterZ;
    g_BattleMagicHitmark_RayDistSq = g_BattleMagicHitmark_DeltaX * g_BattleMagicHitmark_DeltaX + g_BattleMagicHitmark_DeltaZ * g_BattleMagicHitmark_DeltaZ + -0x895440 >> 12;
    for (i = 0; i < 600; i++) {
        g_BattleMagicHitmark_ParticlePool[i].timer = 0;
    }
    Battle_AllocateResourceSlot(BattleFx_MagicHitmarkTimelineTask);
    Battle_AllocateResourceSlot(BattleFx_MagicHitmarkParticlePoolTask);
    Battle_SpawnBoneParticleBurst(0, 4, 0x13, 0x5DC, 0, 45);
    Battle_SpawnBoneParticleBurst(0, 4, 0x23, 0x5DC, 0, 45);
}

/*
 * Steps and draws all active particles in the Hitmark particle pool.
 */
// FUNCTION: FF7 0x005a033b
void __cdecl BattleFx_MagicHitmarkParticlePoolTask(void)
{
    int activeCount;
    BattleTaskSlot *taskSlot;
    int i;

    taskSlot = BATTLE_CURRENT_TASK_SLOT;
    activeCount = 0;
    for (i = 0; i < 600; i++) {
        if (g_BattleMagicHitmark_ParticlePool[i].timer != 0) {
            BattleFx_MagicHitmarkDrawAndStepParticle(&g_BattleMagicHitmark_ParticlePool[i]);
            activeCount++;
        }
    }
    if (activeCount == 0 && g_BattleSecondaryTaskActiveCount <= 2) {
        taskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates physics and draws a single Hitmark particle.
 */
// FUNCTION: FF7 0x005a03c4
void __cdecl BattleFx_MagicHitmarkDrawAndStepParticle(BattleFxMagicHitmarkParticle *particle)
{
    BattleHitmarkPrimitive *prim;
    int *bits;
    int color;
    PsxMatrix *matrix;
    int *vector;

    /* The data block holds the matrix, then the ordering depth word and the GTE flag word. */
    matrix = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    vector = (int *)(matrix + 1);
    bits = vector + 1;
    RotMatrix((SVECTOR *)&particle->angleX, matrix);
    matrix->t[0] = particle->posX;
    matrix->t[1] = particle->posY;
    matrix->t[2] = particle->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, matrix, matrix);
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    RefreshGlobalTransformState();
    prim = (BattleHitmarkPrimitive *)g_BattleMagicHitmark_PrimitivePtr;
    StoreDeferredDrawArgs(&g_BattleMagicHitmark_ParticleVertex0, &g_BattleMagicHitmark_ParticleVertex1, &g_BattleMagicHitmark_SpriteDrawReq.particleVertex2);
    ApplyDeferredDrawArgs();
    Gfx_PsxStoreClearFlag(bits);
    if ((*bits & 0x60000) == 0) {
        color = (particle->timer << 7) / 15;
        color = color << 0x10 | 0x22000000 | color << 8 | color;
        Gfx_ReadTransformOrderingDepth2(vector);
        prim->tag = 0x4000000;
        prim->color = color;
        Gfx_DrawPsxPrimitive(prim);
    }
    if (1 != 0) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
        particle->posX += particle->velocityX;
        particle->posY += particle->velocityY;
        particle->posZ += particle->velocityZ;
        particle->velocityX = particle->velocityX * 0xC00 /* 0.75 */ >> 12;
        particle->velocityY = particle->velocityY + 200;
        particle->velocityZ = particle->velocityZ * 0xC00 /* 0.75 */ >> 12;
        particle->angleX += particle->rateX;
        particle->angleY += particle->rateY;
        particle->angleZ += particle->rateZ;
        (particle->timer)--;
    }
}

/*
 * Main timeline task updating ray intersection and spawning impact effects.
 */
// FUNCTION: FF7 0x005a05ce
void __cdecl BattleFx_MagicHitmarkTimelineTask(void)
{
    int dist;
    int angle;
    BattleTaskSlot *task;
    BattleFxHitmarkImpactSpriteTask *newTask;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (DEAD_IN_RETAIL != 0) {
        return;
    }
    if (task->frameCounter < 45 && task->frameCounter % 1 == 0) {
        angle = task->frameCounter * 0x500 / 45;
        g_BattleMagicHitmark_RayDirection[0] = SinFixed(angle - 0x300);
        g_BattleMagicHitmark_RayDirection[1] = rand() % 512 - 256;
        g_BattleMagicHitmark_RayDirection[2] = CosFixed(angle - 0x300);
        g_BattleMagicHitmark_RayIntersectionResult = BattleFx_MagicHitmarkIntersectRaySphere(g_BattleMagicHitmark_RayDirection, &g_BattleMagicHitmark_HitPos1.vx, ((short *)&g_BattleMagicHitmark_HitPos2));
        if (g_BattleMagicHitmark_RayIntersectionResult == 0) {
            BattleFx_MagicHitmarkSpawnImpactParticleBurst((Vec3sPacked *)&g_BattleMagicHitmark_HitPos1, g_BattleMagicHitmark_RayDirection);
            BattleFx_MagicHitmarkSpawnImpactParticleBurst((Vec3sPacked *)&g_BattleMagicHitmark_HitPos2, g_BattleMagicHitmark_RayDirection);
            if ((task->frameCounter & 1) == 0) {
                BattleFx_MagicHitmarkDrawRadialTriangleBurst((Vec3sPacked *)&g_BattleMagicHitmark_HitPos1);
                BattleFx_MagicHitmarkDrawRadialTriangleBurst((Vec3sPacked *)&g_BattleMagicHitmark_HitPos2);
            }
            if (g_BattleMagicHitmark_SoundPlayedFlag == 0) {
                Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH2, Battle_ComputeScreenPanFromPosition(&g_BattleMagicHitmark_HitPos1), 0x227);
                g_BattleMagicHitmark_SoundPlayedFlag = 1;
            }
        } else {
            dist = (-3300 - g_BattleMagicHitmark_OriginCoords.originX << 12) / g_BattleMagicHitmark_RayDirection[0];
            g_BattleMagicHitmark_HitPos1.vz = g_BattleMagicHitmark_OriginCoordZ + (g_BattleMagicHitmark_RayDirection[2] * dist >> 12);
            if (g_BattleMagicHitmark_HitPos1.vz > -3500 && g_BattleMagicHitmark_HitPos1.vz < 700) {
                g_BattleMagicHitmark_HitPos1.vx = 0xF31C;
                g_BattleMagicHitmark_HitPos1.vy = g_BattleMagicHitmark_OriginCoords.originY + (g_BattleMagicHitmark_RayDirection[1] * dist >> 12);
                newTask = (BattleFxHitmarkImpactSpriteTask *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicHitmarkImpactSpriteTask);
                *(Int64Pair *)&newTask->posX = *(Int64Pair *)&g_BattleMagicHitmark_HitPos1;
                newTask->scale = 0x4000 /* 4.0 */;
            }
            if (g_BattleMagicHitmark_SoundPlayedFlag == 0) {
                Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition(&g_BattleMagicHitmark_HitPos1), 0x1E7);
            }
        }
    }
    (task->frameCounter)++;
    if (task->frameCounter >= 45) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Renders and advances the impact billboard sprite for Hitmark.
 */
// FUNCTION: FF7 0x005a0839
void __cdecl BattleFx_MagicHitmarkImpactSpriteTask(void)
{
    BattleFxHitmarkImpactSpriteTask *task;

    task = (BattleFxHitmarkImpactSpriteTask *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicHitmark_SpriteDrawReq.sprite.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleMagicHitmark_LoadedRes);
    g_BattleMagicHitmark_SpriteDrawReq.sprite.drawObj = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->scale, task->scale * -0x20 >> 12);
    g_BattleMagicHitmark_SpriteDrawReq.sprite.frame = (task->frame + 4) | 0x8000;
    g_BattleMagicHitmark_PrimitivePtr = BattleFx_SubmitSpriteDraw(&g_BattleMagicHitmark_SpriteDrawReq.sprite, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicHitmark_PrimitivePtr);
    if (1 != 0) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
        task->posY = task->posY;
        (task->frame)++;
        if (task->frame >= 4) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws radial triangle burst around impact point.
 */
// FUNCTION: FF7 0x005a0919
void __cdecl BattleFx_MagicHitmarkDrawRadialTriangleBurst(Vec3sPacked *origin)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[124];
        PsxMatrix basisMatrix;
        struct { unsigned char pad_to_20[0x20]; int radius; };
        struct { unsigned char pad_to_24[0x24]; int sector; };
        struct { unsigned char pad_to_28[0x28]; SVECTOR verts[8]; };
        struct { unsigned char pad_to_68[0x68]; SVECTOR dir; };
        struct { unsigned char pad_to_70[0x70]; BattleHitmarkPrimitive *primitive; };
        struct { unsigned char pad_to_74[0x74]; int depthAux; };
        struct { unsigned char pad_to_78[0x78]; int depth; };
    } frame;
#pragma pack(pop)

    frame.dir.vx = origin->vx - g_BattleMagicHitmark_ResourceDefs.impactCenterX;
    frame.dir.vy = 0;
    frame.dir.vz = origin->vz - g_BattleMagicHitmark_ImpactCenterZ;
    Gte_BuildLookBasisFromDirection(&frame.dir.vx, &frame.basisMatrix.m[0][0]);
    frame.basisMatrix.t[0] = origin->vx;
    frame.basisMatrix.t[1] = origin->vy;
    frame.basisMatrix.t[2] = origin->vz;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.basisMatrix, &frame.basisMatrix);
    SetRotMatrix(&frame.basisMatrix);
    SetTransMatrix(&frame.basisMatrix);
    RefreshGlobalTransformState();
    for (frame.sector = 0; frame.sector < 8; frame.sector++) {
        frame.radius = rand() % 500 + 1000;
        frame.verts[frame.sector].vx = SinFixed((frame.sector << 12) / 8) * frame.radius >> 12;
        frame.verts[frame.sector].vy = CosFixed((frame.sector << 12) / 8) * frame.radius >> 11;
        frame.verts[frame.sector].vz = 0;
    }
    frame.primitive = (BattleHitmarkPrimitive *)g_BattleMagicHitmark_PrimitivePtr;
    for (frame.sector = 0; frame.sector < 8; frame.sector++) {
        frame.primitive->tag = 0x6000000;
        Gfx_TransformTriangleReturnDepth(&frame.verts[frame.sector], &frame.verts[(frame.sector + 1) & 7], (SVECTOR *)&g_BattleMagicHitmark_TriangleCenter, &frame.depth, &frame.depthAux);
        frame.primitive->color = 0x32000000;
        frame.primitive->color1 = 0x30000000;
        frame.primitive->color2 = 0x30808080;
        Gfx_DrawPsxPrimitive(frame.primitive);
    }
    g_BattleMagicHitmark_PrimitivePtr = (int)frame.primitive;
}

/*
 * Spawns a burst of impact particles and allocates impact sprite task.
 */
// FUNCTION: FF7 0x005a0ac0
void __cdecl BattleFx_MagicHitmarkSpawnImpactParticleBurst(Vec3sPacked *origin, void *scale)
{
    BattleFxHitmarkImpactSpriteTask *taskPtr;
    BattleFxMagicHitmarkParticle *particle;
    int i;
    int matrix[8];

    for (i = 0; i < 20; i++) {
        particle = BattleFx_MagicHitmarkClaimParticleRecord();
        particle->posX = origin->vx;
        particle->posY = origin->vy;
        particle->posZ = origin->vz;
        g_BattleMagicHitmark_BurstAngle.angleY = rand() % 0x700 - 0x380;
        g_BattleMagicHitmark_BurstAngle.angleX = rand() % 0x700 - 0x380;
        RotMatrix((SVECTOR *)&g_BattleMagicHitmark_BurstAngle, matrix);
        ScaleMatrix(matrix, &g_BattleMagicHitmark_BurstScale);
        TransformVectorToInt16(matrix, scale, &particle->velocityX);
        particle->angleZ = 0;
        particle->angleY = 0;
        particle->angleX = 0;
        particle->rateX = (rand() & 0x3FF) - 512;
        particle->rateY = (rand() & 0x3FF) - 512;
        particle->rateZ = (rand() & 0x3FF) - 512;
    }
    taskPtr = (BattleFxHitmarkImpactSpriteTask *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicHitmarkImpactSpriteTask);
    *(Int64Pair *)&taskPtr->posX = *(Int64Pair *)origin;
    taskPtr->scale = 0x1000 /* 1.0 */;
}

/*
 * Claims an unused particle record from the Hitmark pool.
 */
// FUNCTION: FF7 0x005a0c1c
BattleFxMagicHitmarkParticle *__cdecl BattleFx_MagicHitmarkClaimParticleRecord(void)
{
    int i;

    for (i = 0; i < 600; i++) {
        if (g_BattleMagicHitmark_ParticlePool[i].timer == 0) {
            g_BattleMagicHitmark_ParticlePool[i].timer = 15;
            return &g_BattleMagicHitmark_ParticlePool[i];
        }
    }
    return g_BattleMagicHitmark_ParticlePool;
}

/*
 * Computes ray-sphere intersection points for Hitmark impact calculations.
 */
// FUNCTION: FF7 0x005a0c73
int __cdecl BattleFx_MagicHitmarkIntersectRaySphere(short *dir, short *pointA, short *pointB)
{
    struct {
        int discriminant;
        int rootT;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    g_BattleMagicHitmark_RayCoeff = (dir[0] * g_BattleMagicHitmark_DeltaX + dir[2] * g_BattleMagicHitmark_DeltaZ) * 2 >> 12;
    frame.discriminant = g_BattleMagicHitmark_RayCoeff * g_BattleMagicHitmark_RayCoeff - (g_BattleMagicHitmark_RayDistSq << 14);
    if (frame.discriminant < 0) {
        return -1;
    }
    frame.discriminant = SqrtToInt(frame.discriminant);
    frame.rootT = (-g_BattleMagicHitmark_RayCoeff - frame.discriminant) / 2;
    pointA[0] = (g_BattleMagicHitmark_OriginCoords.originX) + (dir[0] * frame.rootT >> 12);
    pointA[1] = g_BattleMagicHitmark_OriginCoords.originY + (dir[1] * frame.rootT >> 12);
    pointA[2] = g_BattleMagicHitmark_OriginCoordZ + (dir[2] * frame.rootT >> 12);
    frame.rootT = (-g_BattleMagicHitmark_RayCoeff + frame.discriminant) / 2;
    pointB[0] = (g_BattleMagicHitmark_OriginCoords.originX) + (dir[0] * frame.rootT >> 12);
    pointB[1] = g_BattleMagicHitmark_OriginCoords.originY + (dir[1] * frame.rootT >> 12);
    pointB[2] = g_BattleMagicHitmark_OriginCoordZ + (dir[2] * frame.rootT >> 12);
    return 0;
}
} /* extern "C" */
