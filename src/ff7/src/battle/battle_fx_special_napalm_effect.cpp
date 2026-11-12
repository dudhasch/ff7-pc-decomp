// src/ff7/src/battle/battle_fx_special_napalm_effect.cpp
/* Enemy Special Attack [107]: Napalm (orig. SPECIAL/NAPARM, ナパーム) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Naparm': loads its resources, runs a per-target timeline with
//   vector interpolation helpers, and drives missile, arcing trail, bouncing, growing, impact and
//   fading sprite tasks.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
// clang-format off

typedef struct BattleFxResourceSlot28 {
    unsigned char pad[0x1c];
    int marker;
} BattleFxResourceSlot28;
#define g_BattleSpecNapalmRsdNames1Marker12 (((BattleFxResourceSlot12 *)&g_BattleSpecNapalmRsdNames1)->marker)
#define g_BattleSpecNapalmRsdNames2Marker12 (((BattleFxResourceSlot12 *)&g_BattleSpecNapalmRsdNames2)->marker)
#define g_BattleSpecNapalmRsdNames3Marker12 (((BattleFxResourceSlot12 *)&g_BattleSpecNapalmRsdNames3)->marker)
#define g_BattleSpecNapalmSptTimNames1Marker12 (((BattleFxResourceSlot12 *)&g_BattleSpecNapalmSptTimNames1)->marker)
#define g_BattleSpecNapalmSptTimNames2Marker12 (((BattleFxResourceSlot12 *)&g_BattleSpecNapalmSptTimNames2)->marker)
#define g_BattleSpecNapalmSptTimNames3Marker12 (((BattleFxResourceSlot12 *)&g_BattleSpecNapalmSptTimNames3)->marker)
#define g_BattleSpecNapalmSptTimNames1Marker28 (((BattleFxResourceSlot28 *)&g_BattleSpecNapalmSptTimNames1)->marker)
#define g_BattleSpecNapalmSptTimNames2Marker28 (((BattleFxResourceSlot28 *)&g_BattleSpecNapalmSptTimNames2)->marker)
#define g_BattleSpecNapalmSptTimNames3Marker28 (((BattleFxResourceSlot28 *)&g_BattleSpecNapalmSptTimNames3)->marker)
// GLOBAL: FF7 0x0089d4d0
unsigned int g_BattleSpecNapalmResourceDefs[65] = {0x001900b1, (unsigned int)&g_BattleSpecNapalmRsdNames1, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecNapalmRsdNames1Marker12, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecNapalmRsdNames2, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecNapalmRsdNames2Marker12, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecNapalmRsdNames3, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecNapalmRsdNames3Marker12, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecNapalmRsdNameWa, 0x00000000, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecNapalmTimName02a, (unsigned int)&g_BattleSpecNapalmSptTimNames1, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecNapalmSptTimNames1Marker12, (unsigned int)&g_BattleSpecNapalmSptTimNames1Marker28, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecNapalmTimName02b, (unsigned int)&g_BattleSpecNapalmSptTimNames2, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecNapalmSptTimNames2Marker12, (unsigned int)&g_BattleSpecNapalmSptTimNames2Marker28, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecNapalmTimName01, (unsigned int)&g_BattleSpecNapalmSptTimNames3, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecNapalmSptTimNames3Marker12, (unsigned int)&g_BattleSpecNapalmSptTimNames3Marker28, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x0089d5d8
struct BattleFxSpriteRequest g_BattleSpecNapalmSpriteReq1 = {0, 0, -128, -128, -128, 0x2e, 0, 0};
// g_BattleSpecNapalmSpriteOffsetTable is indexed by the code as an array. The span runs to the next
// named definition of the TU; the TU's pieces inside 0x89d5e8..0x89d5f0 are aliases.
// GLOBAL: FF7 0x0089d5e8
unsigned char g_BattleSpecNapalmSpriteOffsetTable[8] = {0x08, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0089d5f0
struct BattleSpecNapalmSpriteReq2 g_BattleSpecNapalmSpriteReq2 = {{0, 0x18, 0, 0x20, 0, 0, 0, 0}, 0, 0};
// GLOBAL: FF7 0x0089d600
struct BattleSpecNapalmSpriteReq3 g_BattleSpecNapalmSpriteReq3 = {64, 64, 64, {0x2e}, 0, {0}};
// GLOBAL: FF7 0x0089d608
struct BattleSpecNapalmScale g_BattleSpecNapalmScale = {{0, 0x10, 0, 0, 0, 0x10, 0, 0, 0, 0x10, 0, 0, 0, 0, 0, 0}, 0};
unsigned char g_rawData_0089d61c[4] = {0x80, 0x00, 0x00, 0x00};
unsigned char g_rawData_0089d620[8] = {0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0089d628
struct BattleSpecNapalmSpriteReq4 g_BattleSpecNapalmSpriteReq4 = {-1024, 0, 0, {0}, 0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}, 0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}, 0, 0};
// GLOBAL: FF7 0x0089d658
struct BattleSpecNapalmModelReq g_BattleSpecNapalmModelReq = {-128, -128, -128, 44, 0, {0}, 0, {0x88, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}, 0, 0, {0}, 128};
unsigned char g_rawData_0089d67c[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0089d680
unsigned char g_BattleSpecNapalmRsdNames1[24] =
    "MISA.RSD\0\0\0\0"
    "SINDOM2.RSD";
// GLOBAL: FF7 0x0089d698
unsigned char g_BattleSpecNapalmRsdNames2[23] =
    "SINDOM1.RSD\0"
    "BAKU11.RSD";
// GLOBAL: FF7 0x0089d6b0
unsigned char g_BattleSpecNapalmRsdNames3[24] =
    "HIKARI.RSD\0\0"
    "SENKOU1.RSD";
// GLOBAL: FF7 0x0089d6c8
unsigned char g_BattleSpecNapalmRsdNameWa[7] = "WA.RSD";
// GLOBAL: FF7 0x0089d6d0
unsigned char g_BattleSpecNapalmTimName02a[13] = "NAPALM02.TIM";
// GLOBAL: FF7 0x0089d6e0
unsigned char g_BattleSpecNapalmSptTimNames1[39] =
    "HUNSYA.SPT\0\0"
    "NAPALM03.TIM\0\0\0\0"
    "KEMU11.SPT";
// GLOBAL: FF7 0x0089d708
unsigned char g_BattleSpecNapalmTimName02b[13] = "NAPALM02.TIM";
// GLOBAL: FF7 0x0089d718
unsigned char g_BattleSpecNapalmSptTimNames2[38] =
    "KEMU2.SPT\0\0\0"
    "NAPALM02.TIM\0\0\0\0"
    "KEMU3.SPT";
// GLOBAL: FF7 0x0089d740
unsigned char g_BattleSpecNapalmTimName01[13] = "NAPALM01.TIM";
// GLOBAL: FF7 0x0089d750
unsigned char g_BattleSpecNapalmSptTimNames3[39] =
    "ENMAKU.SPT\0\0"
    "NAPALM02.TIM\0\0\0\0"
    "HAHEN1.SPT";
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b5b0c8
int g_BattleSpecNapalmDrawListHead = 0;
// GLOBAL: FF7 0x00b5b0cc
int g_BattleSpecNapalmBlendStep = 0;
// GLOBAL: FF7 0x00b5b0d0
short g_BattleSpecNapalmModelCounter = 0;
// GLOBAL: FF7 0x00b5b0d4
struct GfxTableCursor *g_BattleSpecNapalmLoadedResTable = 0;
// GLOBAL: FF7 0x00b5b0d8
int g_BattleSpecNapalmRotationMatrix = 0;
unsigned char g_bssPad_00b5b0dc[4] = {0};
unsigned char g_bssPad_00b5b0e0[24] = {0};
// clang-format on

/*
 * When the engine context exists and g_BattleSpecNapalmLoadedResTable is empty, builds SPECIAL/NAPARM with a default-
 * duration record and creates resource type 13 using g_BattleSpecNapalmResourceDefs.
 */
// FUNCTION: FF7 0x0051e190
void __cdecl BattleFx_SpecialLoadResNapalm(void)
{
    GfxModelTableBank *workSceneCtx;
    GfxResourceRequest reqBuffer;
    int kind;
    char outSize[200];
    GfxResourceRequest *pRequest;
    int resDefBlock;

    if (g_pBattle3dScene != 0 && g_BattleSpecNapalmLoadedResTable == 0) {
        kind = 0xD;
        resDefBlock = (int)&g_BattleSpecNapalmResourceDefs;
        workSceneCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0089d778
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/NAPARM", outSize);
        Gfx_LoadModelTableIfAbsent(kind, resDefBlock, pRequest, &g_BattleSpecNapalmLoadedResTable, workSceneCtx);
    }
}

/*
 * Loads SPECIAL/NAPARM, resets the model counter and blend step, blends the polygon parts of model groups 1 and 2
 * when both exist, and registers the per-target callback.
 */
// FUNCTION: FF7 0x0051e220
void __cdecl BattleFx_SpecialEnterNapalm(int targetMask, int actorIdx)
{
    int srcParts, dstParts;

    BattleFx_SpecialLoadResNapalm();
    g_BattleSpecNapalmModelCounter = 8;
    g_BattleSpecNapalmBlendStep = 0;
    dstParts = Gfx_ModelTableGetRenderObject(2, g_BattleSpecNapalmLoadedResTable);
    srcParts = Gfx_ModelTableGetRenderObject(1, g_BattleSpecNapalmLoadedResTable);
    if (dstParts != 0 && srcParts != 0) {
        Gfx_ForEachPolygonSetPartPair((DxGroupPairCallback)BattleFx_SpecialNapalmBlendPartVectorsCallback, *(int *)((char *)srcParts + 4), *(int *)((char *)dstParts + 4));
    }
    BattleFx_SpecialRegisterNapalmPerTargetCallback(targetMask, actorIdx);
}

/*
 * Interpolates one vertex: dst + (src - dst) * factor, where factor is the whole number of 30-tick steps
 * in g_BattleSpecNapalmBlendStep (integer division), so it jumps rather than fades smoothly.
 */
// FUNCTION: FF7 0x0051e2a0
void __cdecl BattleFx_SpecialNapalmBlendPartVectorsCallback(int unused, BattleFxHellBomberEndVertex *srcVec, BattleFxSlowBlendVertex *dstVec, BattleFxHellBomberEndVertex *outVec)
{
    float factor;
    int wholeSteps;

    wholeSteps = g_BattleSpecNapalmBlendStep / 30;
    factor = wholeSteps;
    outVec->posX = (srcVec->posX - dstVec->x) * factor + dstVec->x;
    outVec->posY = (srcVec->posY - dstVec->y) * factor + dstVec->y;
    outVec->posZ = (srcVec->posZ - dstVec->z) * factor + dstVec->z;
}

/* Runs BattleFx_SpecialSpawnNapalmPerTarget for each actor in the target mask. */
// FUNCTION: FF7 0x0051e30d
void __cdecl BattleFx_SpecialRegisterNapalmPerTargetCallback(int targetMask, int sourceActor)
{
    Battle_CreateBitPatternCallbackTask(targetMask, sourceActor, 4, BattleFx_SpecialSpawnNapalmPerTarget);
}

/*
 * Starts a timeline task from the source actor's bone34 position to the target actor's bone position,
 * remembers the target actor, and plays the launch sound.
 */
// FUNCTION: FF7 0x0051e329
void __cdecl BattleFx_SpecialSpawnNapalmPerTarget(int actor, int sourceActor)
{
    BattleFxTrajectory *slot;

    slot = (BattleFxTrajectory *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialNapalmTimelineTask);
    Battle_GetBoneWorldPos(sourceActor, g_BattleActorRecords[sourceActor].bone34, &slot->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &slot->endX);
    slot->actorIndex = actor;
    Battle_PlaySfxPannedToActor(0x82, (short)sourceActor, 0x40);
}

/* Timeline of the Naparm effect: smoke sprites, the arcing trail, the missile, the impact models and the bouncing sparks. */
// FUNCTION: FF7 0x0051e3b3
void __cdecl BattleFx_SpecialNapalmTimelineTask(void)
{
        int p, q;
        BattleFxTrajectory *taskPtr;
        int s;
    short j6;
        BattleFxSpecialNapalmSlot *pChild;
        int v, w, x, y, z, a, b, c;
    short frameAngle;
        int e, f, g, h, j, k, m;

    taskPtr = (BattleFxTrajectory *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->step < 10) {
        for (j6 = 0; j6 < 4; j6++) {
            pChild = (BattleFxSpecialNapalmSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialNapalmVariantSpriteTask);
            pChild->posX = taskPtr->startX;
            pChild->posY = taskPtr->startY;
            pChild->posZ = taskPtr->startZ;
            frameAngle = rand() % 0x1000;
            pChild->velocityX = CosFixed(frameAngle) * 0x50 >> 12;
            pChild->velocityZ = SinFixed(frameAngle) * 0x50 >> 12;
            pChild->velocityY = -rand() % 80;
            pChild->variant = 0;
        }
    }
    if (taskPtr->step == 5) {
        frameAngle = ArcTangentFixed(taskPtr->endX - taskPtr->startX, taskPtr->endZ - taskPtr->startZ);
        for (j6 = 0; j6 < 2; j6++) {
            pChild = (BattleFxSpecialNapalmSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialNapalmArcingTrailEmitterTask);
            pChild->posX = taskPtr->startX;
            pChild->posY = taskPtr->startY;
            pChild->posZ = taskPtr->startZ;
            pChild->velocityX = SinFixed(frameAngle + j6 * 600 + -300) * 0x96 >> 12;
            pChild->velocityZ = CosFixed(frameAngle + j6 * 600 + -300) * 0x96 >> 12;
            pChild->velocityY = -rand() % 50 - 50;
        }
    }
    if (taskPtr->step == 10) {
        pChild = (BattleFxSpecialNapalmSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialNapalmMissileTask);
        pChild->posX = taskPtr->startX;
        pChild->posY = taskPtr->startY;
        pChild->posZ = taskPtr->startZ;
        pChild->velocityX = (taskPtr->endX - taskPtr->startX) / 10;
        pChild->velocityY = (taskPtr->endY - taskPtr->startY) / 10;
        pChild->velocityZ = (taskPtr->endZ - taskPtr->startZ) / 10;
        pChild->angle = ArcTangentFixed(taskPtr->endX - taskPtr->startX, taskPtr->endZ - taskPtr->startZ);
    }
    if (taskPtr->step == 40) {
        Battle_PostActorHitOrHurtMessage(taskPtr->actorIndex);
    }
    if (taskPtr->step == 20) {
        pChild = (BattleFxSpecialNapalmSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialNapalmImpactModelsTask);
        pChild->posX = taskPtr->endX;
        pChild->posY = 0;
        pChild->posZ = taskPtr->endZ;
        pChild = (BattleFxSpecialNapalmSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialNapalmGrowingModelTask);
        pChild->posX = taskPtr->endX;
        pChild->posY = 0xFED4;
        pChild->posZ = taskPtr->endZ;
        for (j6 = 0; j6 < 20; j6++) {
            pChild = (BattleFxSpecialNapalmSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialNapalmBouncingSpriteTask);
            pChild->posX = taskPtr->endX;
            pChild->posZ = taskPtr->endZ;
            pChild->posY = 0;
            BattleFx_MakeRandomConeVector(0xFFFFFF06, 100, &pChild->velocityX);
            pChild->variant = rand() % 7;
        }
        Battle_PlaySfxPannedToActor(0x176, (short)taskPtr->actorIndex, 0x40);
    }
    (taskPtr->step)++;
    if (taskPtr->step == 55) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the smoke sprite of the task's variant as a billboard (the variant picks both its scale and its sprite
 * group); the variant-0 sprite also moves by its velocity. The task ends when its frame reaches the group's frame count.
 */
// FUNCTION: FF7 0x0051e83d
void __cdecl BattleFx_SpecialNapalmVariantSpriteTask(void)
{
    BattleFxHellBomberVariantSprite *task;

    task = (BattleFxHellBomberVariantSprite *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix(&task->posX, (short)((short *)&g_BattleSpecNapalmSpriteReq2)[task->variant], 0);
    g_BattleSpecNapalmSpriteReq1.frame = task->frameIndex;
    g_BattleSpecNapalmSpriteReq1.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(((int *)&g_BattleSpecNapalmSpriteOffsetTable)[task->variant], g_BattleSpecNapalmLoadedResTable);
    g_BattleSpecNapalmSpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(((int *)&g_BattleSpecNapalmSpriteOffsetTable)[task->variant], g_BattleSpecNapalmLoadedResTable);
    g_BattleSpecNapalmDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecNapalmSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecNapalmDrawListHead);
    if (g_BattleIsPaused == 0) {
        if (task->variant == 0) {
            task->posX += task->velocityX;
            task->posY += task->velocityY;
            task->posZ += task->velocityZ;
        }
        if (g_BattleSpecNapalmSpriteReq1.spriteGroup != 0) {
            (task->frameIndex)++;
            if (task->frameIndex == g_BattleSpecNapalmSpriteReq1.spriteGroup->header->frameCount) {
                task->ownerSlot = BATTLE_TASK_FREE;
            }
        }
    }
}

// Moves the trail emitter along its arc (gravity adds to velocityY) and spawns a variant-1 smoke sprite at
// each position; the emitter ends after ten frames.
// FUNCTION: FF7 0x0051e980
void __cdecl BattleFx_SpecialNapalmArcingTrailEmitterTask(void)
{
    BattleFxMeteorModelSlot *task;
    BattleFxNapalmTrailSlot *spawned;

    task = (BattleFxMeteorModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posX += task->velocityX;
    task->posY += task->velocityY;
    task->posZ += task->velocityZ;
    task->velocityY += 10;
    spawned = (BattleFxNapalmTrailSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialNapalmVariantSpriteTask);
    spawned->posX = task->posX;
    spawned->posY = task->posY;
    spawned->posZ = task->posZ;
    spawned->variant = 1;
    (task->frameCount)++;
    if (task->frameCount == 10) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Draws the missile model, its billboard and its twelve-sprite exhaust trail, and moves it by its velocity. */
// FUNCTION: FF7 0x0051ea73
void __cdecl BattleFx_SpecialNapalmMissileTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[112];
        struct { unsigned char pad_to_4[0x4]; SVECTOR trailX; };
        struct { unsigned char pad_to_10[0x10]; PsxMatrix baseMatrix; };
        struct { unsigned char pad_to_34[0x34]; short trailIndex; };
        struct { unsigned char pad_to_3C[0x3C]; short stepX; };
        struct { unsigned char pad_to_40[0x40]; short stepZ; };
        struct { unsigned char pad_to_44[0x44]; BattleFxNapalmMissileSlot *missileSlot; };
        struct { unsigned char pad_to_4C[0x4C]; PsxMatrix drawMatrix; };
    } frame;
#pragma pack(pop)

    frame.missileSlot = (BattleFxNapalmMissileSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecNapalmScale.modelRes = Gfx_ModelTableGetRenderObject(0, g_BattleSpecNapalmLoadedResTable);
    g_BattleSpecNapalmSpriteReq4.res8 = Gfx_ModelTableGetLoadedRecord(7, g_BattleSpecNapalmLoadedResTable);
    g_BattleSpecNapalmSpriteReq4.res12 = Gfx_ModelTableGetSpriteObject(7, g_BattleSpecNapalmLoadedResTable);
    g_BattleSpecNapalmSpriteReq4.res24 = Gfx_ModelTableGetLoadedRecord(11, g_BattleSpecNapalmLoadedResTable);
    g_BattleSpecNapalmSpriteReq4.res28 = Gfx_ModelTableGetSpriteObject(11, g_BattleSpecNapalmLoadedResTable);
    (g_BattleSpecNapalmSpriteReq4.rotX) = 0xFC00 /* 5670 deg */;
    g_BattleSpecNapalmSpriteReq4.rotY = 0xFC00 /* 5670 deg */;
    g_BattleSpecNapalmSpriteReq4.rotZ = 0;
    RotMatrixXYZ((short *)&g_BattleSpecNapalmSpriteReq4, &frame.baseMatrix);
    RotateMatrixY(frame.missileSlot->angle, &frame.baseMatrix);
    ScaleMatrix(&frame.baseMatrix, (int *)&g_BattleSpecNapalmScale);
    frame.baseMatrix.t[0] = frame.missileSlot->posX;
    frame.baseMatrix.t[1] = frame.missileSlot->posY;
    frame.baseMatrix.t[2] = frame.missileSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.baseMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecNapalmDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecNapalmScale.modelRes, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecNapalmDrawListHead);
    BattleFx_InstallBillboardMatrix(&frame.missileSlot->posX, 0x2000 /* 2.0 */, 0);
    g_BattleSpecNapalmSpriteReq4.firstFrame = frame.missileSlot->frameCounter % 3;
    g_BattleSpecNapalmDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecNapalmSpriteReq4.res8, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecNapalmDrawListHead);
    frame.trailX.vx = frame.missileSlot->posX;
    frame.trailX.vy = frame.missileSlot->posY;
    frame.trailX.vz = frame.missileSlot->posZ;
    frame.stepX = -SinFixed(frame.missileSlot->angle) * ((frame.missileSlot->frameCounter % 2) * 80 + 80) >> 12;
    frame.stepZ = -CosFixed(frame.missileSlot->angle) * ((frame.missileSlot->frameCounter % 2) * 80 + 80) >> 12;
    for (frame.trailIndex = 0; frame.trailIndex < 12; frame.trailIndex++) {
        BattleFx_InstallBillboardMatrix(&frame.trailX, 0x2000 /* 2.0 */, 0);
        g_BattleSpecNapalmSpriteReq4.secondFrame = frame.trailIndex >> 1;
        g_BattleSpecNapalmDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecNapalmSpriteReq4.res24, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecNapalmDrawListHead);
        frame.trailX.vx += frame.stepX;
        frame.trailX.vz += frame.stepZ;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.missileSlot->posX += frame.missileSlot->velocityX;
    frame.missileSlot->posY += frame.missileSlot->velocityY;
    frame.missileSlot->posZ += frame.missileSlot->velocityZ;
    (frame.missileSlot->frameCounter)++;
    if (frame.missileSlot->frameCounter == 10) {
        frame.missileSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Draws one bouncing spark sprite that falls back to the ground and rebounds until its frame counter runs out. */
// FUNCTION: FF7 0x0051ed97
void __cdecl BattleFx_SpecialNapalmBouncingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int bounceStep;
        struct { unsigned char pad_to_4[0x4]; SVECTOR angleX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; short phase; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix scaleMatrix; };
        struct { unsigned char pad_to_50[0x50]; short axisIndex; };
        struct { unsigned char pad_to_54[0x54]; int depth; };
        struct { unsigned char pad_to_58[0x58]; BattleFxHellBomberBouncingSpriteSlot *bounceSlot; };
    } frame;
#pragma pack(pop)

    frame.bounceSlot = (BattleFxHellBomberBouncingSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecNapalmSpriteReq4.res40 = Gfx_ModelTableGetLoadedRecord(12, g_BattleSpecNapalmLoadedResTable);
    g_BattleSpecNapalmSpriteReq4.res44 = Gfx_ModelTableGetSpriteObject(12, g_BattleSpecNapalmLoadedResTable);
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.scaleMatrix.m[2][frame.axisIndex] = 0;
        frame.scaleMatrix.m[1][frame.axisIndex] = 0;
        frame.scaleMatrix.m[0][frame.axisIndex] = 0;
    }
    if (frame.bounceSlot->frameCounter < 22) {
        frame.bounceStep = 8;
    } else {
        frame.bounceStep = 30 - frame.bounceSlot->frameCounter;
    }
    frame.phase = *(short *)&frame.bounceStep;
    frame.angleX.vy = 0;
    frame.angleX.vx = frame.angleX.vy;
    frame.angleX.vz = frame.bounceSlot->angle;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((SVECTOR *)&frame.bounceSlot->posX, frame.scaleMatrix.t, &frame.depth);
    frame.scaleMatrix.m[2][2] = 0x2000 /* 2.0 */;
    frame.scaleMatrix.m[1][1] = frame.scaleMatrix.m[2][2];
    frame.scaleMatrix.m[0][0] = frame.scaleMatrix.m[1][1];
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.scaleMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.scaleMatrix);
    SetTransMatrix(&frame.scaleMatrix);
    g_BattleSpecNapalmModelReq.frame = frame.bounceSlot->variant;
    g_BattleSpecNapalmModelReq.code = (frame.phase - 8 != 0 ? 2 : 0) + 0x2C;
    g_BattleSpecNapalmModelReq.blue = frame.phase << 4;
    g_BattleSpecNapalmModelReq.green = g_BattleSpecNapalmModelReq.blue;
    (g_BattleSpecNapalmModelReq.red) = g_BattleSpecNapalmModelReq.green;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecNapalmSpriteReq4.res40, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.bounceSlot->posX += frame.bounceSlot->velocityX;
    frame.bounceSlot->posY += frame.bounceSlot->velocityY;
    frame.bounceSlot->posZ += frame.bounceSlot->velocityZ;
    if (frame.bounceSlot->posY > 0) {
        frame.bounceSlot->posY = 0;
        frame.bounceSlot->velocityY *= -1;
    }
    frame.bounceSlot->velocityY += 20;
    frame.bounceSlot->angle += 0x200 /* 45 deg */;
    (frame.bounceSlot->frameCounter)++;
    if (frame.bounceSlot->frameCounter == 30) {
        frame.bounceSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Draws the growing impact model, which scales up by a fixed step each frame. */
// FUNCTION: FF7 0x0051f011
void __cdecl BattleFx_SpecialNapalmGrowingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[88];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_24[0x24]; BattleFxHellBomberGrowSlot *growSlot; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_48[0x48]; VECTOR scaleVector; };
    } frame;
#pragma pack(pop)

    frame.growSlot = (BattleFxHellBomberGrowSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecNapalmModelReq.res8 = Gfx_ModelTableGetRenderObject(6, g_BattleSpecNapalmLoadedResTable);
    RotMatrixXYZ(&g_BattleSpecNapalmRotationMatrix, &frame.rotMatrix);
    frame.scaleVector.vz = frame.growSlot->growStep * 0x5DC;
    frame.scaleVector.vy = frame.scaleVector.vz;
    frame.scaleVector.vx = frame.scaleVector.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleVector);
    frame.rotMatrix.t[0] = frame.growSlot->posX;
    frame.rotMatrix.t[1] = frame.growSlot->posY;
    frame.rotMatrix.t[2] = frame.growSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecNapalmModelReq.alpha = frame.growSlot->growStep << 8;
    g_BattleSpecNapalmDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecNapalmModelReq.res8, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecNapalmDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.growSlot->growStep)++;
    if (frame.growSlot->growStep == 15) {
        frame.growSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Draws the spinning, growing impact models, blends the polygon parts, and spawns the fading sprites around the impact. */
// FUNCTION: FF7 0x0051f142
void __cdecl BattleFx_SpecialNapalmImpactModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[176];
        int blendResB;
        struct { unsigned char pad_to_4[0x4]; int blendResA; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR angleX; };
        struct { unsigned char pad_to_20[0x20]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_40[0x40]; BattleFxNapalmTrailSlot *spawnSlot; };
        struct { unsigned char pad_to_44[0x44]; short spawnIndex; };
        struct { unsigned char pad_to_50[0x50]; BattleFxNapalmImpactModelsSlot *impactSlot; };
        struct { unsigned char pad_to_54[0x54]; short spawnRadius; };
        struct { unsigned char pad_to_58[0x58]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_7C[0x7C]; short spawnAngle; };
        struct { unsigned char pad_to_98[0x98]; VECTOR scaleVector; };
    } frame;
#pragma pack(pop)

    frame.impactSlot = (BattleFxNapalmImpactModelsSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.angleX.vy = frame.impactSlot->frameCounter << 5;
    frame.angleX.vz = 0;
    frame.angleX.vx = frame.angleX.vz;
    RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
    frame.scaleVector.vz = frame.impactSlot->frameCounter * 0x190 + 0x1000;
    frame.scaleVector.vy = frame.scaleVector.vz;
    frame.scaleVector.vx = frame.scaleVector.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleVector);
    frame.rotMatrix.t[0] = frame.impactSlot->originX;
    frame.rotMatrix.t[1] = frame.impactSlot->originY;
    frame.rotMatrix.t[2] = frame.impactSlot->originZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecNapalmModelReq.flags = 0x28;
    g_BattleSpecNapalmModelReq.secondAlpha = g_BattleSpecNapalmModelCounter << 4;
    g_BattleSpecNapalmModelReq.res24 = Gfx_ModelTableGetRenderObject(2, g_BattleSpecNapalmLoadedResTable);
    g_BattleSpecNapalmDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecNapalmModelReq.res24, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecNapalmDrawListHead);
    g_BattleSpecNapalmModelReq.flags = 0x28;
    g_BattleSpecNapalmModelReq.res24 = Gfx_ModelTableGetRenderObject(3, g_BattleSpecNapalmLoadedResTable);
    g_BattleSpecNapalmDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecNapalmModelReq.res24, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecNapalmDrawListHead);
    g_BattleSpecNapalmModelReq.flags = 0xA8;
    g_BattleSpecNapalmModelReq.res24 = Gfx_ModelTableGetRenderObject(4, g_BattleSpecNapalmLoadedResTable);
    g_BattleSpecNapalmModelReq.secondAlpha = 0x1000 - (g_BattleSpecNapalmModelCounter << 9);
    g_BattleSpecNapalmDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecNapalmModelReq.res24, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecNapalmDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    g_BattleSpecNapalmBlendStep = frame.impactSlot->frameCounter + 1;
    frame.blendResB = Gfx_ModelTableGetRenderObject(2, g_BattleSpecNapalmLoadedResTable);
    frame.blendResA = Gfx_ModelTableGetRenderObject(1, g_BattleSpecNapalmLoadedResTable);
    if (frame.blendResB != 0 && frame.blendResA != 0) {
        Gfx_ForEachPolygonSetPartPair((DxGroupPairCallback)BattleFx_SpecialNapalmBlendPartVectorsCallback, *(int *)((char *)frame.blendResA + 4), *(int *)((char *)frame.blendResB + 4));
    }
    if (frame.impactSlot->frameCounter < 22) {
        frame.spawnRadius = frame.impactSlot->frameCounter * 80;
        for (frame.spawnIndex = 0; frame.spawnIndex < 4; frame.spawnIndex++) {
            frame.spawnAngle = (rand() % 4096);
            frame.spawnSlot = (BattleFxNapalmTrailSlot *)(&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialNapalmFadingSpriteTask)]);
            frame.spawnSlot->posX = frame.impactSlot->originX + (CosFixed(frame.spawnAngle) * frame.spawnRadius >> 12);
            frame.spawnSlot->posZ = frame.impactSlot->originZ + (SinFixed(frame.spawnAngle) * frame.spawnRadius >> 12);
            frame.spawnSlot->posY = 0xFF6A;
            frame.spawnSlot->variant = 2;
        }
    }
    g_BattleCameraFocalPoint.focalPointY += (short)(((frame.impactSlot->frameCounter % 2) << 4) - 8);
    if (frame.impactSlot->frameCounter > 22) {
        g_BattleSpecNapalmModelCounter--;
    }
    (frame.impactSlot->frameCounter)++;
    if (frame.impactSlot->frameCounter == 30) {
        frame.impactSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws a fading sprite (sprite group 0xA) as a billboard at the task's position; its colour dims by four per frame.
 * The task ends when its frame reaches the group's frame count.
 */
// FUNCTION: FF7 0x0051f4ed
void __cdecl BattleFx_SpecialNapalmFadingSpriteTask(void)
{
    BattleTaskSlot *task;

    task = (BattleTaskSlot *)(&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx]);
    g_BattleSpecNapalmSpriteReq2.fadingRecord = Gfx_ModelTableGetLoadedRecord(10, g_BattleSpecNapalmLoadedResTable);
    g_BattleSpecNapalmSpriteReq2.drawObj = Gfx_ModelTableGetSpriteObject(10, g_BattleSpecNapalmLoadedResTable);
    BattleFx_InstallBillboardMatrix((short *)task->data, 0x1800 /* 1.5 */, 0);
    g_BattleSpecNapalmSpriteReq3.frame = task->frameCounter;
    g_BattleSpecNapalmSpriteReq3.blue = 0x40 - (task->frameCounter << 2);
    g_BattleSpecNapalmSpriteReq3.green = g_BattleSpecNapalmSpriteReq3.blue;
    g_BattleSpecNapalmSpriteReq3.red = g_BattleSpecNapalmSpriteReq3.green;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecNapalmSpriteReq2.fadingRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0 && g_BattleSpecNapalmSpriteReq2.fadingRecord != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= *(int *)((char *)*(int *)g_BattleSpecNapalmSpriteReq2.fadingRecord + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
