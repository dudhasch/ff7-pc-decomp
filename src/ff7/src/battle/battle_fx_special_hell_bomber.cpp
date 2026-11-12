// src/ff7/src/battle/battle_fx_special_hell_bomber.cpp
/* Enemy Special Effect [idx 63]: HellBomber (JP: roket / ロケット (Rocket), asset: "SPECIAL/ROKET") — Hell Bomber (0x13d, Hell House) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Roket' (rocket): a rocket task with a per-target timeline,
//   morphing growing models, an expanding model, bouncing and falling sprite emitters, and vertex
//   interpolation.

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
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// GLOBAL: FF7 0x0089f330
unsigned int g_BattleSpecHellBomberResourceDefs[65] = {
    0x001900b4,
    (unsigned int)&g_BattleSpecHellBomberMisaName,
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0089f4e4
    (unsigned int)"SINDOM2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0089f4f0
    (unsigned int)"SINDOM1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x002800b1,
    // STRING: FF7 0x0089f4fc
    (unsigned int)"BAKU11.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0089f508
    (unsigned int)"HIKARI.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0089f514
    (unsigned int)"WA.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089f51c
    (unsigned int)"ROCKET02.TIM",
    // STRING: FF7 0x0089f52c
    (unsigned int)"HUNSYA.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089f538
    (unsigned int)"ROCKET03.TIM",
    // STRING: FF7 0x0089f548
    (unsigned int)"KEMU11.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089f554
    (unsigned int)"ROCKET02.TIM",
    // STRING: FF7 0x0089f564
    (unsigned int)"KEMU2.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089f570
    (unsigned int)"ROCKET02.TIM",
    // STRING: FF7 0x0089f580
    (unsigned int)"KEMU3.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089f58c
    (unsigned int)"ROCKET01.TIM",
    // STRING: FF7 0x0089f59c
    (unsigned int)"ENMAKU.SPT",
    0x00000000,
    0x00000000,
    0x00192a3c,
    // STRING: FF7 0x0089f5a8
    (unsigned int)"ROCKET02.TIM",
    // STRING: FF7 0x0089f5b8
    (unsigned int)"HAHEN1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089f5c4
    (unsigned int)"ROCKET02.TIM",
    // STRING: FF7 0x0089f5d4
    (unsigned int)"HAHEN1.SPT"};
// GLOBAL: FF7 0x0089f438
struct BattleSpecHellBomberSpriteReq1 g_BattleSpecHellBomberSpriteReq1 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0};
// GLOBAL: FF7 0x0089f448
unsigned char g_BattleSpecHellBomberAnimIndices[16] = {0x07, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0089f458
unsigned char g_BattleSpecHellBomberScaleTable[8] = {0x00, 0x18, 0x00, 0x20, 0x00, 0x20, 0x00, 0x30};
// GLOBAL: FF7 0x0089f460
struct BattleSpecHellBomberSpriteReq2 g_BattleSpecHellBomberSpriteReq2 = {0, 0, 64, 64, 64, {0x2e}, 0};
// GLOBAL: FF7 0x0089f470
unsigned char g_BattleSpecHellBomberScaleVector[16] = {0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0089f480
unsigned char g_BattleSpecHellBomberModelReq1[16] = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0089f490
unsigned char g_BattleSpecHellBomberRotAngles[6] = {0x00, 0xfc, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0089f498
struct BattleSpecHellBomberSpriteReq3 g_BattleSpecHellBomberSpriteReq3 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0};
// GLOBAL: FF7 0x0089f4a8
struct BattleSpecHellBomberSpriteReq4 g_BattleSpecHellBomberSpriteReq4 = {0, 0, -128, -128, -128, {0x2c}, 0};
// GLOBAL: FF7 0x0089f4b8
struct BattleSpecHellBomberModelReq3 g_BattleSpecHellBomberModelReq3 = {0, {0x88, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x0089f4c8
struct BattleSpecHellBomberModelReq2 g_BattleSpecHellBomberModelReq2 = {0, 0, {0}, 128, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x0089f4d8
unsigned char g_BattleSpecHellBomberMisaName[9] = "MISA.RSD";
// clang-format on


// clang-format off
// GLOBAL: FF7 0x00b5e40c
int g_BattleSpecHellBomberDrawListHead = 0;
// GLOBAL: FF7 0x00b5e410
int g_BattleSpecHellBomberCounter = 0;
// GLOBAL: FF7 0x00b5e414
short g_BattleSpecHellBomberTargetMask = 0;
// GLOBAL: FF7 0x00b5e418
struct GfxTableCursor *g_BattleSpecHellBomberLoadedResTable = 0;
unsigned char g_bssPad_00b5e41c[4] = {0};
// GLOBAL: FF7 0x00b5e420
int g_BattleSpecHellBomberRotVector = 0;
unsigned char g_bssPad_00b5e424[4] = {0};
unsigned char g_bssPad_00b5e428[4] = {0};
unsigned char g_bssPad_00b5e42c[4] = {0};
unsigned char g_bssPad_00b5e430[4] = {0};
unsigned char g_bssPad_00b5e434[4] = {0};
unsigned char g_bssPad_00b5e438[4] = {0};
unsigned char g_bssPad_00b5e43c[4] = {0};
// clang-format on

/*
 * When the engine context exists and g_BattleSpecHellBomberLoadedResTable is empty, builds SPECIAL/ROKET with a default-
 * duration record and creates resource type 13 using g_BattleSpecHellBomberResourceDefs.
 */
// FUNCTION: FF7 0x0052a550
void __cdecl BattleFx_SpecialLoadResHellBomber(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest currentRecord;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecHellBomberLoadedResTable == 0) {
        kind = 0xD;
        resourceDefsWord = (int)&g_BattleSpecHellBomberResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0089f5e0
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/ROKET", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecHellBomberLoadedResTable, curOwnerCtx);
    }
}

/*
 * Loads SPECIAL/ROKET, sets g_BattleSpecHellBomberTargetMask to eight and g_BattleSpecHellBomberCounter to zero,
 * and when models two and one exist interpolates their vertices with
 * BattleFx_SpecialHellBomberInterpolateVertexCallback. Then registers the per-target callbacks.
 */
// FUNCTION: FF7 0x0052a5e0
void __cdecl BattleFx_SpecialEnterHellBomber(int targetMask, int actorIdx)
{
    int fieldOne, fieldTwo;

    BattleFx_SpecialLoadResHellBomber();
    g_BattleSpecHellBomberTargetMask = 8;
    g_BattleSpecHellBomberCounter = 0;
    fieldTwo = Gfx_ModelTableGetRenderObject(2, g_BattleSpecHellBomberLoadedResTable);
    fieldOne = Gfx_ModelTableGetRenderObject(1, g_BattleSpecHellBomberLoadedResTable);
    if (fieldTwo != 0 && fieldOne != 0) {
        Gfx_ForEachPolygonSetPartPair((DxGroupPairCallback)BattleFx_SpecialHellBomberInterpolateVertexCallback, *(int *)((char *)fieldOne + 4), *(int *)((char *)fieldTwo + 4));
    }
    BattleFx_SpecialRegisterHellBomberPerTargetCallback(targetMask, actorIdx);
}

/*
 * Writes the three floats at outVertex by interpolating from the vector at startVertex towards the vector at
 * endVertex: out[i] = (endVertex[i] - startVertex[i]) * t + startVertex[i]. The weight t is g_BattleSpecHellBomberCounter / 0x1E computed
 * as an integer division and only then converted to float, so it steps in whole units rather than
 * sweeping smoothly. ignored is never read.
 */
// FUNCTION: FF7 0x0052a660
void __cdecl BattleFx_SpecialHellBomberInterpolateVertexCallback(int ignored, BattleFxHellBomberEndVertex *endVertex, BattleFxSlowBlendVertex *startVertex, BattleFxHellBomberEndVertex *outVertex)
{
    float weight;
    int stepIndex;

    stepIndex = g_BattleSpecHellBomberCounter / 30;
    weight = stepIndex;
    outVertex->posX = (endVertex->posX - startVertex->x) * weight + startVertex->x;
    outVertex->posY = (endVertex->posY - startVertex->y) * weight + startVertex->y;
    outVertex->posZ = (endVertex->posZ - startVertex->z) * weight + startVertex->z;
}

/*
 * Creates the per-target spawn callbacks and plays the sound at the target actor.
 */
// FUNCTION: FF7 0x0052a6cd
void __cdecl BattleFx_SpecialRegisterHellBomberPerTargetCallback(int sourceActor, int targetActor)
{
    Battle_CreateBitPatternCallbackTask(sourceActor, targetActor, 4, BattleFx_SpecialSpawnHellBomberPerTarget);
    Battle_PlaySfxPannedToActor(0x82, (short)targetActor, 0x40);
}

/*
 * Allocates a timeline task whose start is the target actor's bone34 position and whose end is
 * the actor's targetBone position, and stores actor in it.
 */
// FUNCTION: FF7 0x0052a6fd
void __cdecl BattleFx_SpecialSpawnHellBomberPerTarget(int actor, int targetActor)
{
    BattleFxTrajectory *taskSlot;

    taskSlot = (BattleFxTrajectory *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHellBomberTimelineTask);
    Battle_GetBoneWorldPos(targetActor, g_BattleActorRecords[targetActor].bone34, &taskSlot->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &taskSlot->endX);
    taskSlot->actorIndex = actor;
}

/*
 * Timeline task of one target: spawns the smoke sprites, the falling sprite emitters, the rocket, and at the impact the growing models, the expanding model and the bouncing sprites.
 */
// FUNCTION: FF7 0x0052a773
void __cdecl BattleFx_SpecialHellBomberTimelineTask(void)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxTrajectory *taskPtr;
    int s; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short j6;
    BattleFxNapalmTrailSlot *pChild;
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int y; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int a; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int b; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int c; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short frameAngle;
    int e; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int f; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int g; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int h; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int j; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int k; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int m; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    taskPtr = (BattleFxTrajectory *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->step < 10) {
        for (j6 = 0; j6 < 4; j6++) {
            pChild = (BattleFxNapalmTrailSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHellBomberVariantSpriteTask);
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
            pChild = (BattleFxNapalmTrailSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHellBomberFallingSpriteEmitterTask);
            pChild->posX = taskPtr->startX;
            pChild->posY = taskPtr->startY;
            pChild->posZ = taskPtr->startZ;
            pChild->velocityX = SinFixed(frameAngle + j6 * 600 + -300) * 0x96 >> 12;
            pChild->velocityZ = CosFixed(frameAngle + j6 * 600 + -300) * 0x96 >> 12;
            pChild->velocityY = -rand() % 200 - 200;
        }
    }
    if (taskPtr->step == 10) {
        pChild = (BattleFxNapalmTrailSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHellBomberRocketTask);
        pChild->posX = taskPtr->startX;
        pChild->posY = taskPtr->startY;
        pChild->posZ = taskPtr->startZ;
        pChild->velocityX = (taskPtr->endX - taskPtr->startX) / 20;
        pChild->velocityY = 0xFED4;
        pChild->velocityZ = (taskPtr->endZ - taskPtr->startZ) / 20;
    }
    if (taskPtr->step == 50) {
        Battle_PostActorHitOrHurtMessage(taskPtr->actorIndex);
    }
    if (taskPtr->step == 30) {
        pChild = (BattleFxNapalmTrailSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHellBomberGrowingMorphModelsTask);
        pChild->posX = taskPtr->endX;
        pChild->posY = 0;
        pChild->posZ = taskPtr->endZ;
        pChild = (BattleFxNapalmTrailSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHellBomberExpandingModelTask);
        pChild->posX = taskPtr->endX;
        pChild->posY = 0xFED4;
        pChild->posZ = taskPtr->endZ;
        for (j6 = 0; j6 < 20; j6++) {
            pChild = (BattleFxNapalmTrailSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHellBomberBouncingSpriteTask);
            pChild->posX = taskPtr->endX;
            pChild->posZ = taskPtr->endZ;
            pChild->posY = 0;
            BattleFx_MakeRandomConeVector(0xFFFFFF06, 100, &pChild->velocityX);
            pChild->variant = rand() % 7;
        }
        Battle_StartFixedPointRamp(0x1000, 10);
        g_BattleFxTimelineState.red = 0x20;
        g_BattleFxTimelineState.channel1 = 0;
        g_BattleFxTimelineState.channel2 = 0;
        Battle_PlaySfxPannedToActor(0x176, (short)taskPtr->actorIndex, 0x40);
    }
    (taskPtr->step)++;
    if (taskPtr->step == 60) {
        Battle_StartFixedPointRamp(0, 10);
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Per-frame tick of one sprite. The variant index selects the billboard scale from
// g_BattleSpecHellBomberScaleTable and the animation entry from g_BattleSpecHellBomberAnimIndices. The sprite
// is fetched from g_BattleSpecHellBomberLoadedResTable into g_BattleSpecHellBomberSpriteReq1 and chained into
// g_BattleSpecHellBomberDrawListHead. Unless g_BattleIsPaused is set, only variants below 1 move, by
// their velocities. With a nonnull sprite the frame index advances, and the slot is freed when it
// equals the animation's frame count.
// FUNCTION: FF7 0x0052abe9
void __cdecl BattleFx_SpecialHellBomberVariantSpriteTask(void)
{
    BattleFxHellBomberVariantSprite *task;

    task = (BattleFxHellBomberVariantSprite *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix(&task->posX, (short)((short *)&g_BattleSpecHellBomberScaleTable)[task->variant], 0);
    g_BattleSpecHellBomberSpriteReq1.req1FrameIndex = task->frameIndex;
    (g_BattleSpecHellBomberSpriteReq1.spriteResource) = Gfx_ModelTableGetLoadedRecord(((int *)&g_BattleSpecHellBomberAnimIndices)[task->variant], g_BattleSpecHellBomberLoadedResTable);
    g_BattleSpecHellBomberSpriteReq1.variantResourceHandle = Gfx_ModelTableGetSpriteObject(((int *)&g_BattleSpecHellBomberAnimIndices)[task->variant], g_BattleSpecHellBomberLoadedResTable);
    g_BattleSpecHellBomberDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecHellBomberSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecHellBomberDrawListHead);
    if (g_BattleIsPaused == 0) {
        if (task->variant < 1) {
            task->posX += task->velocityX;
            task->posY += task->velocityY;
            task->posZ += task->velocityZ;
        }
        if ((g_BattleSpecHellBomberSpriteReq1.spriteResource) != 0) {
            (task->frameIndex)++;
            if (task->frameIndex == *(int *)((char *)*(int *)(g_BattleSpecHellBomberSpriteReq1.spriteResource) + 4)) {
                task->ownerSlot = BATTLE_TASK_FREE;
            }
        }
    }
}

/*
 * When unpaused integrates the position, increases the Y velocity by forty and emits a variant sprite
 * with the resulting position, half the velocities and variant one. Frees itself after ten frames.
 */
// FUNCTION: FF7 0x0052ad2d
void __cdecl BattleFx_SpecialHellBomberFallingSpriteEmitterTask(void)
{
    BattleFxHellBomberFallingSprite *task;
    BattleFxHellBomberFallingSprite *spawned;

    task = (BattleFxHellBomberFallingSprite *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posX += task->velocityX;
    task->posY += task->velocityY;
    task->posZ += task->velocityZ;
    task->velocityY += 40;
    spawned = (BattleFxHellBomberFallingSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHellBomberVariantSpriteTask);
    spawned->posX = task->posX;
    spawned->posY = task->posY;
    spawned->posZ = task->posZ;
    spawned->velocityX = task->velocityX >> 1;
    spawned->velocityY = task->velocityY >> 1;
    spawned->velocityZ = task->velocityZ >> 1;
    spawned->variant = 1;
    (task->frameCount)++;
    if (task->frameCount == 10) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the rocket model and its flame sprite, leaves a smoke sprite behind it each frame and moves it along its arc for 20 frames.
 */
// FUNCTION: FF7 0x0052ae50
void __cdecl BattleFx_SpecialHellBomberRocketTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[128];
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleFxNapalmTrailSlot *sprite; };
        struct { unsigned char pad_to_34[0x34]; BattleFxHellBomberRocketSlot *rocketSlot; };
        struct { unsigned char pad_to_3C[0x3C]; PsxMatrix lookBasis; };
        struct { unsigned char pad_to_5C[0x5C]; PsxMatrix modelMatrix; };
    } frame;
#pragma pack(pop)

    frame.rocketSlot = (BattleFxHellBomberRocketSlot *)BATTLE_CURRENT_TASK_SLOT;
    (*(int *)&g_BattleSpecHellBomberModelReq1) = Gfx_ModelTableGetRenderObject(0, g_BattleSpecHellBomberLoadedResTable);
    (g_BattleSpecHellBomberSpriteReq3.spriteField) = Gfx_ModelTableGetLoadedRecord(6, g_BattleSpecHellBomberLoadedResTable);
    g_BattleSpecHellBomberSpriteReq3.req3ResourceHandle = Gfx_ModelTableGetSpriteObject(6, g_BattleSpecHellBomberLoadedResTable);
    Gte_BuildLookBasisFromDirection((short *)&frame.rocketSlot->velocityX, frame.lookBasis.m[0]);
    RotMatrixXYZ((int *)&g_BattleSpecHellBomberRotAngles, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&frame.lookBasis, &frame.rotMatrix, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, (int *)&g_BattleSpecHellBomberScaleVector);
    frame.rotMatrix.t[0] = frame.rocketSlot->posX;
    frame.rotMatrix.t[1] = frame.rocketSlot->posY;
    frame.rotMatrix.t[2] = frame.rocketSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecHellBomberDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecHellBomberModelReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecHellBomberDrawListHead);
    BattleFx_InstallBillboardMatrix(&frame.rocketSlot->posX, 0x2000 /* 2.0 */, 0);
    g_BattleSpecHellBomberSpriteReq3.req3Word12 = frame.rocketSlot->frameCounter % 3;
    g_BattleSpecHellBomberDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecHellBomberSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecHellBomberDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.sprite = (BattleFxNapalmTrailSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHellBomberVariantSpriteTask);
    frame.sprite->posX = frame.rocketSlot->posX;
    frame.sprite->posY = frame.rocketSlot->posY;
    frame.sprite->posZ = frame.rocketSlot->posZ;
    frame.sprite->variant = 3;
    frame.rocketSlot->posX += frame.rocketSlot->velocityX;
    frame.rocketSlot->posY += frame.rocketSlot->velocityY;
    frame.rocketSlot->posZ += frame.rocketSlot->velocityZ;
    frame.rocketSlot->velocityY += 30;
    (frame.rocketSlot->frameCounter)++;
    if (frame.rocketSlot->frameCounter == 20) {
        frame.rocketSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws one bouncing spark sprite under the camera, picking its stage from its age, and moves it.
 */
// FUNCTION: FF7 0x0052b09d
void __cdecl BattleFx_SpecialHellBomberBouncingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int stageTicks;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; short stage; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix combinedMat; };
        struct { unsigned char pad_to_50[0x50]; short axis; };
        struct { unsigned char pad_to_54[0x54]; int depth; };
        struct { unsigned char pad_to_58[0x58]; BattleFxHellBomberBouncingSpriteSlot *slot; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxHellBomberBouncingSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    for (frame.axis = 0; frame.axis < 3; frame.axis++) {
        frame.combinedMat.m[2][frame.axis] = 0;
        frame.combinedMat.m[1][frame.axis] = 0;
        frame.combinedMat.m[0][frame.axis] = 0;
    }
    if (frame.slot->frameCounter < 22) {
        frame.stageTicks = 8;
    } else {
        frame.stageTicks = 30 - frame.slot->frameCounter;
    }
    frame.stage = *(short *)&frame.stageTicks;
    frame.rotX.vy = 0;
    frame.rotX.vx = frame.rotX.vy;
    frame.rotX.vz = frame.slot->angle;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((SVECTOR *)&frame.slot->posX, frame.combinedMat.t, &frame.depth);
    frame.combinedMat.m[2][2] = 0x2000;
    frame.combinedMat.m[1][1] = frame.combinedMat.m[2][2];
    frame.combinedMat.m[0][0] = frame.combinedMat.m[1][1];
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.combinedMat, &frame.rotMatrix);
    SetRotMatrix(&frame.combinedMat);
    SetTransMatrix(&frame.combinedMat);
    g_BattleSpecHellBomberSpriteReq4.req4Word12 = frame.slot->variant;
    if (frame.stage == 8) {
        (g_BattleSpecHellBomberSpriteReq4.spriteResource) = Gfx_ModelTableGetLoadedRecord(11, g_BattleSpecHellBomberLoadedResTable);
        g_BattleSpecHellBomberSpriteReq4.req4ResourceHandle = Gfx_ModelTableGetSpriteObject(11, g_BattleSpecHellBomberLoadedResTable);
    } else {
        (g_BattleSpecHellBomberSpriteReq4.spriteResource) = Gfx_ModelTableGetLoadedRecord(12, g_BattleSpecHellBomberLoadedResTable);
        g_BattleSpecHellBomberSpriteReq4.req4ResourceHandle = Gfx_ModelTableGetSpriteObject(12, g_BattleSpecHellBomberLoadedResTable);
    }
    g_BattleSpecHellBomberSpriteReq4.req4ChainByte2 = frame.stage << 4;
    g_BattleSpecHellBomberSpriteReq4.req4ChainByte1 = g_BattleSpecHellBomberSpriteReq4.req4ChainByte2;
    g_BattleSpecHellBomberSpriteReq4.req4ChainByte0 = g_BattleSpecHellBomberSpriteReq4.req4ChainByte1;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecHellBomberSpriteReq4, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.slot->posX += frame.slot->velocityX;
    frame.slot->posY += frame.slot->velocityY;
    frame.slot->posZ += frame.slot->velocityZ;
    if (frame.slot->posY > 0) {
        frame.slot->posY = 0;
        frame.slot->velocityY *= -1;
    }
    frame.slot->velocityY += 20;
    frame.slot->angle += 0x200 /* 45 deg */;
    (frame.slot->frameCounter)++;
    if (frame.slot->frameCounter == 30) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the expanding model at the slot position as it grows and fades.
 */
// FUNCTION: FF7 0x0052b337
void __cdecl BattleFx_SpecialHellBomberExpandingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[88];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_24[0x24]; BattleFxHellBomberGrowSlot *growSlot; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix modelMatrix; };
        struct { unsigned char pad_to_48[0x48]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.growSlot = (BattleFxHellBomberGrowSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecHellBomberModelReq3.modelField) = Gfx_ModelTableGetRenderObject(5, g_BattleSpecHellBomberLoadedResTable);
    RotMatrixXYZ(&g_BattleSpecHellBomberRotVector, &frame.rotMatrix);
    frame.scaleX.vz = frame.growSlot->growStep * 0x5DC;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    frame.rotMatrix.t[0] = frame.growSlot->posX;
    frame.rotMatrix.t[1] = frame.growSlot->posY;
    frame.rotMatrix.t[2] = frame.growSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecHellBomberModelReq3.alpha = frame.growSlot->growStep << 8;
    g_BattleSpecHellBomberDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecHellBomberModelReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecHellBomberDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.growSlot->growStep)++;
    if (frame.growSlot->growStep == 15) {
        frame.growSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the pair of models that morph into each other while growing.
 */
// FUNCTION: FF7 0x0052b468
void __cdecl BattleFx_SpecialHellBomberGrowingMorphModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[176];
        int partTwo;
        struct { unsigned char pad_to_4[0x4]; int partOne; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR rotX; };
        struct { unsigned char pad_to_20[0x20]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_40[0x40]; BattleFxNapalmTrailSlot *sprite; };
        struct { unsigned char pad_to_44[0x44]; short spriteIndex; };
        struct { unsigned char pad_to_50[0x50]; BattleFxHellBomberGrowingMorphModelsSlot *morphSlot; };
        struct { unsigned char pad_to_54[0x54]; short radius; };
        struct { unsigned char pad_to_58[0x58]; PsxMatrix modelMatrix; };
        struct { unsigned char pad_to_7C[0x7C]; short randAngle; };
        struct { unsigned char pad_to_98[0x98]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.morphSlot = (BattleFxHellBomberGrowingMorphModelsSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vy = frame.morphSlot->frameCounter << 5;
    frame.rotX.vz = 0;
    frame.rotX.vx = frame.rotX.vz;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.scaleX.vz = frame.morphSlot->frameCounter * 0x190 + 0x1000;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    frame.rotMatrix.t[0] = frame.morphSlot->centerX;
    frame.rotMatrix.t[1] = frame.morphSlot->centerY;
    frame.rotMatrix.t[2] = frame.morphSlot->centerZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecHellBomberModelReq2.flags = 0;
    g_BattleSpecHellBomberModelReq2.alpha = g_BattleSpecHellBomberTargetMask << 4;
    (g_BattleSpecHellBomberModelReq2.modelResource) = Gfx_ModelTableGetRenderObject(2, g_BattleSpecHellBomberLoadedResTable);
    g_BattleSpecHellBomberDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecHellBomberModelReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecHellBomberDrawListHead);
    g_BattleSpecHellBomberModelReq2.flags = 0x28;
    (g_BattleSpecHellBomberModelReq2.modelResource) = Gfx_ModelTableGetRenderObject(3, g_BattleSpecHellBomberLoadedResTable);
    g_BattleSpecHellBomberDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecHellBomberModelReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecHellBomberDrawListHead);
    g_BattleSpecHellBomberModelReq2.flags = 0xA8;
    (g_BattleSpecHellBomberModelReq2.modelResource) = Gfx_ModelTableGetRenderObject(4, g_BattleSpecHellBomberLoadedResTable);
    g_BattleSpecHellBomberModelReq2.alpha = 0x1000 - (g_BattleSpecHellBomberTargetMask << 9);
    g_BattleSpecHellBomberDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecHellBomberModelReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecHellBomberDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    g_BattleSpecHellBomberCounter = frame.morphSlot->frameCounter + 1;
    frame.partTwo = Gfx_ModelTableGetRenderObject(2, g_BattleSpecHellBomberLoadedResTable);
    frame.partOne = Gfx_ModelTableGetRenderObject(1, g_BattleSpecHellBomberLoadedResTable);
    if (frame.partTwo != 0 && frame.partOne != 0) {
        Gfx_ForEachPolygonSetPartPair((DxGroupPairCallback)BattleFx_SpecialHellBomberInterpolateVertexCallback, *(int *)((char *)frame.partOne + 4), *(int *)((char *)frame.partTwo + 4));
    }
    if (frame.morphSlot->frameCounter < 22) {
        frame.radius = frame.morphSlot->frameCounter * 80;
        for (frame.spriteIndex = 0; frame.spriteIndex < 4; frame.spriteIndex++) {
            frame.randAngle = (rand() % 4096);
            frame.sprite = (BattleFxNapalmTrailSlot *)(&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialHellBomberFadingAnimatedSpriteTask)]);
            frame.sprite->posX = frame.morphSlot->centerX + (CosFixed(frame.randAngle) * frame.radius >> 12);
            frame.sprite->posZ = frame.morphSlot->centerZ + (SinFixed(frame.randAngle) * frame.radius >> 12);
            frame.sprite->posY = 0xFF6A;
            frame.sprite->variant = 2;
        }
    }
    g_BattleCameraFocalPoint.focalPointY += (short)(((frame.morphSlot->frameCounter % 2) << 4) - 8);
    if (frame.morphSlot->frameCounter > 22) {
        g_BattleSpecHellBomberTargetMask--;
    }
    (frame.morphSlot->frameCounter)++;
    if (frame.morphSlot->frameCounter == 30) {
        frame.morphSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Per-frame tick for a record in a different array, at (g_BattleSecondaryTaskActiveSlotIdx << 5) +
 * &g_BattleTaskSlotBuffer.secondarySlots. Loads animation entry 9 of resource
 * g_BattleSpecHellBomberLoadedResTable into the sprite descriptor g_BattleSpecHellBomberSpriteReq2,
 * billboards at scale 0x1800, copies the frame counter at +2 into the descriptor's +0xC and sets
 * its three colour bytes at +8, +9 and +0xA all to 0x40 - (counter << 2), a fade that drops by 4
 * per frame, then chains the draw into g_BattleOverlayDrawListHead. When unpaused with a
 * nonnull sprite it advances the counter and stores 0xFFFF at +0 at the animation's frame count.
 */
// FUNCTION: FF7 0x0052b813
void __cdecl BattleFx_SpecialHellBomberFadingAnimatedSpriteTask(void)
{
    BattleTaskSlot *task;

    task = (BattleTaskSlot *)(&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx]);
    (g_BattleSpecHellBomberSpriteReq2.spriteResource) = Gfx_ModelTableGetLoadedRecord(9, g_BattleSpecHellBomberLoadedResTable);
    g_BattleSpecHellBomberSpriteReq2.fadeResourceHandle = Gfx_ModelTableGetSpriteObject(9, g_BattleSpecHellBomberLoadedResTable);
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), 0x1800 /* 1.5 */, 0);
    g_BattleSpecHellBomberSpriteReq2.req2Word12 = task->frameCounter;
    g_BattleSpecHellBomberSpriteReq2.req2ChainByte2 = 0x40 - (task->frameCounter << 2);
    g_BattleSpecHellBomberSpriteReq2.req2ChainByte1 = g_BattleSpecHellBomberSpriteReq2.req2ChainByte2;
    g_BattleSpecHellBomberSpriteReq2.req2ChainByte0 = g_BattleSpecHellBomberSpriteReq2.req2ChainByte1;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecHellBomberSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0 && (g_BattleSpecHellBomberSpriteReq2.spriteResource) != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= *(int *)((char *)*(int *)(g_BattleSpecHellBomberSpriteReq2.spriteResource) + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
