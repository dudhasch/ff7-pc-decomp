// src/ff7/src/battle/battle_fx_special_dark_wave_effect.cpp
/* Enemy Special Attack [104]: Dark Wave (orig. special/yami, 闇) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Yami' (darkness): a per-target timeline with scattered animated
//   sprites, a radial burst sprite and a projectile.

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
// GLOBAL: FF7 0x008a0ce8
unsigned int g_BattleSpecDarkWaveResourceDefs[70] = {0x001900b4, (unsigned int)&g_BattleSpecDarkWaveMisa1Name, 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)((char *)&g_BattleSpecDarkWaveMisa1Name + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)&g_BattleSpecDarkWaveMisa3Name, 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)((char *)&g_BattleSpecDarkWaveMisa3Name + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)&g_BattleSpecDarkWaveMisa5Name, 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)((char *)&g_BattleSpecDarkWaveMisa5Name + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)&g_BattleSpecDarkWaveMisa7Name, 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)((char *)&g_BattleSpecDarkWaveMisa7Name + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)&g_BattleSpecDarkWaveMisa9Name, 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)((char *)&g_BattleSpecDarkWaveMisa9Name + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecDarkWaveTimName1, (unsigned int)((char *)&g_BattleSpecDarkWaveTimName1 + 0xc), 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecDarkWaveTimName2, (unsigned int)((char *)&g_BattleSpecDarkWaveTimName2 + 0xc), 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecDarkWaveTimName3, (unsigned int)((char *)&g_BattleSpecDarkWaveTimName3 + 0xc), 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecDarkWaveTimName4, (unsigned int)((char *)&g_BattleSpecDarkWaveTimName4 + 0xc), 0x00000000, 0x00000000};
// GLOBAL: FF7 0x008a0e00
struct BattleFxSpriteRequest g_BattleSpecDarkWaveSpriteReq1 = {0, 0, 96, 96, 96, 0x2e, 0, 0};
// GLOBAL: FF7 0x008a0e10
struct BattleFxSpriteRequest g_BattleSpecDarkWaveSpriteReq2 = {0, 0, 16, 16, 16, 0x2e, 0, 0};
// GLOBAL: FF7 0x008a0e20
unsigned char g_BattleSpecDarkWaveScaleVector[12] = {0x00, 0x0c, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00};
// GLOBAL: FF7 0x008a0e30
unsigned char g_BattleSpecDarkWaveModelReq[16] = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a0e40
struct BattleFxSpriteRequest g_BattleSpecDarkWaveSpriteReq3 = {0, 0, -128, -128, -128, 0x2c, 0, 0};
// GLOBAL: FF7 0x008a0e50
struct BattleFxSpriteRequest g_BattleSpecDarkWaveSpriteReq4 = {0, 0, -128, -128, -128, 0x2c, 0, 0};

// GLOBAL: FF7 0x008a0e60
unsigned char g_BattleSpecDarkWaveModelIndices[40] = {0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a0e88
unsigned char g_BattleSpecDarkWaveMisa1Name[22] =
    "MISA1.RSD\0\0\0"
    "MISA2.RSD";
// GLOBAL: FF7 0x008a0ea0
unsigned char g_BattleSpecDarkWaveMisa3Name[22] =
    "MISA3.RSD\0\0\0"
    "MISA4.RSD";
// GLOBAL: FF7 0x008a0eb8
unsigned char g_BattleSpecDarkWaveMisa5Name[22] =
    "MISA5.RSD\0\0\0"
    "MISA6.RSD";
// GLOBAL: FF7 0x008a0ed0
unsigned char g_BattleSpecDarkWaveMisa7Name[22] =
    "MISA7.RSD\0\0\0"
    "MISA8.RSD";
// GLOBAL: FF7 0x008a0ee8
unsigned char g_BattleSpecDarkWaveMisa9Name[23] =
    "MISA9.RSD\0\0\0"
    "MISA10.RSD";
// GLOBAL: FF7 0x008a0f00
unsigned char g_BattleSpecDarkWaveTimName1[23] =
    "YAMI00.TIM\0\0"
    "HUNSYA.SPT";
// GLOBAL: FF7 0x008a0f18
unsigned char g_BattleSpecDarkWaveTimName2[22] =
    "YAMI00.TIM\0\0"
    "KEMU1.SPT";
// GLOBAL: FF7 0x008a0f30
unsigned char g_BattleSpecDarkWaveTimName3[22] =
    "YAMI03.TIM\0\0"
    "BAKU1.SPT";
// GLOBAL: FF7 0x008a0f48
unsigned char g_BattleSpecDarkWaveTimName4[21] =
    "YAMI00.TIM\0\0"
    "HIT1.SPT";
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b5eab0
int g_BattleSpecDarkWaveDrawListHead = 0;
// GLOBAL: FF7 0x00b5eab4
struct GfxTableCursor *g_BattleSpecDarkWaveLoadedResTable = 0;
// clang-format on

/*
 * When the engine context exists and g_BattleSpecDarkWaveLoadedResTable is empty, builds special/yami with a default-
 * duration record and creates resource type 14 using g_BattleSpecDarkWaveResourceDefs.
 */
// FUNCTION: FF7 0x0052f170
void __cdecl BattleFx_SpecialLoadResDarkWave(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest currentRecord;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecDarkWaveLoadedResTable == 0) {
        kind = 0xE;
        resourceDefsWord = (int)&g_BattleSpecDarkWaveResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a0f60
        pRequest->resourcePath = BattleFx_BuildResourcePath("special/yami", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecDarkWaveLoadedResTable, curOwnerCtx);
    }
}

/*
 * Entry point: calls BattleFx_SpecialLoadResDarkWave with no arguments, then forwards its own two
 * arguments to BattleFx_SpecialRegisterDarkWavePerTargetCallback.
 */
// FUNCTION: FF7 0x0052f200
void __cdecl BattleFx_SpecialEnterDarkWave(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResDarkWave();
    BattleFx_SpecialRegisterDarkWavePerTargetCallback(targetMask, actorIdx);
}

/*
 * Creates a task for BattleFx_SpecialSpawnDarkWavePerTarget with both arguments and mode 4, then dispatches event (0x82,
 * sourceActor, 0x40).
 */
// FUNCTION: FF7 0x0052f21a
void __cdecl BattleFx_SpecialRegisterDarkWavePerTargetCallback(int targetActor, int sourceActor)
{
    Battle_CreateBitPatternCallbackTask(targetActor, sourceActor, 4, BattleFx_SpecialSpawnDarkWavePerTarget);
    Battle_PlaySfxPannedToActor(0x82, (short)sourceActor, 0x40);
}

// Spawns a BattleFx_SpecialDarkWaveTimelineTask slot. The source actor's bone34 world position goes to
// the slot's posX/posY/posZ, the target actor's targetBone position goes to its velocityX/velocityY/velocityZ,
// and actor is stored in actorIndex.
// FUNCTION: FF7 0x0052f24a
void __cdecl BattleFx_SpecialSpawnDarkWavePerTarget(int actor, int sourceActor)
{
    BattleFxDarkWaveTimelineSlot *taskSlot;

    taskSlot = (BattleFxDarkWaveTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDarkWaveTimelineTask);
    Battle_GetBoneWorldPos(sourceActor, g_BattleActorRecords[sourceActor].bone34, (short *)((char *)taskSlot + 4));
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)((char *)taskSlot + 0xC));
    taskSlot->actorIndex = actor;
}

/*
 * Timeline task that sequences dark sphere expansion and scattered sprites for the Yami effect.
 */
// FUNCTION: FF7 0x0052f2c0
void __cdecl BattleFx_SpecialDarkWaveTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        struct { unsigned char pad_to_8[0x8]; int rotTransOut; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offsetCos; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix matrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxDarkWaveTimelineSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_3C[0x3C]; short angleStep; };
        struct { unsigned char pad_to_40[0x40]; SVECTOR rotX; };
        struct { unsigned char pad_to_48[0x48]; BattleFxDarkWaveTimelineSlot *timelineSlot; };
        struct { unsigned char pad_to_50[0x50]; short angle; };
    } frame;
#pragma pack(pop)

    frame.timelineSlot = (BattleFxDarkWaveTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timelineSlot->frameCounter == 0) {
        frame.spawnSlot = (BattleFxDarkWaveTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDarkWaveProjectileTask);
        frame.spawnSlot->posX = frame.timelineSlot->posX;
        frame.spawnSlot->posY = frame.timelineSlot->posY;
        frame.spawnSlot->posZ = frame.timelineSlot->posZ;
        frame.spawnSlot->angleY = ArcTangentFixed(frame.timelineSlot->velocityX - frame.timelineSlot->posX, frame.timelineSlot->velocityZ - frame.timelineSlot->posZ);
        frame.spawnSlot->velocityX = (frame.timelineSlot->velocityX - frame.timelineSlot->posX) / 15;
        frame.spawnSlot->velocityY = (frame.timelineSlot->velocityY - frame.timelineSlot->posY) / 15;
        frame.spawnSlot->velocityZ = (frame.timelineSlot->velocityZ - frame.timelineSlot->posZ) / 15;
        frame.spawnSlot->verticalDrift = 20;
    }
    if (frame.timelineSlot->frameCounter == 35) {
        for (frame.angleStep = 0; frame.angleStep < 0x10; frame.angleStep++) {
            frame.spawnSlot = (BattleFxDarkWaveTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDarkWaveRadialBurstSpriteTask);
            frame.spawnSlot->posX = frame.timelineSlot->velocityX;
            frame.spawnSlot->posY = frame.timelineSlot->velocityY;
            frame.spawnSlot->posZ = frame.timelineSlot->velocityZ;
            frame.angle = (rand() % 4096);
            frame.offsetCos.vx = CosFixed(frame.angle) * 0x3C >> 12;
            frame.offsetCos.vy = SinFixed(frame.angle) * 0x3C >> 12;
            frame.offsetCos.vz = 0;
            frame.rotX.vz = 0;
            frame.rotX.vx = frame.rotX.vz;
            frame.rotX.vy = frame.angleStep << 8;
            RotMatrix(&frame.rotX, &frame.matrix);
            frame.matrix.t[2] = 0;
            frame.matrix.t[1] = frame.matrix.t[2];
            frame.matrix.t[0] = frame.matrix.t[1];
            SetRotMatrix(&frame.matrix);
            SetTransMatrix(&frame.matrix);
            RotTransPointToInt16(&frame.offsetCos, (short *)((char *)frame.spawnSlot + 0xC), &frame.rotTransOut);
            frame.spawnSlot->unusedAngle = (rand() % 4096);
        }
        Battle_PostActorHitOrHurtMessage(frame.timelineSlot->actorIndex);
        Battle_PlaySfxPannedToActor(0xFA, (short)frame.timelineSlot->actorIndex, 0x40);
    }
    if (frame.timelineSlot->frameCounter == 33) {
        for (frame.angleStep = 0; frame.angleStep < 3; frame.angleStep++) {
            frame.spawnSlot = (BattleFxDarkWaveTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDarkWaveScatteredAnimatedSpriteTask);
            Battle_GetBoneWorldPos(frame.timelineSlot->actorIndex, g_BattleActorRecords[frame.timelineSlot->actorIndex].targetBone, (short *)((char *)frame.spawnSlot + 4));
            frame.spawnSlot->posX += (short)(rand() % 1000 - 500);
            frame.spawnSlot->posY += (short)(rand() % 1000 - 500);
            frame.spawnSlot->posZ += (short)(rand() % 1000 - 500);
        }
    }
    (frame.timelineSlot->frameCounter)++;
    if (frame.timelineSlot->frameCounter == 45) {
        frame.timelineSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Per-frame tick for the current task slot using animation entry 0xD of g_BattleSpecDarkWaveLoadedResTable in the sprite
 * request g_BattleSpecDarkWaveSpriteReq1, billboarded at scale 0x2000, with the slot's frameCounter copied into the
 * request's frame and the draw chained into g_BattleSpecDarkWaveDrawListHead; the draw object is written before the
 * sprite group. Unless g_BattleIsPaused is set it advances the counter and frees the slot when the counter equals the
 * animation's frame count; there is no null-resource guard. An unreferenced local preserves the
 * retail VC5 /Od stack frame size.
 */
// FUNCTION: FF7 0x0052f65c
void __cdecl BattleFx_SpecialDarkWaveScatteredAnimatedSpriteTask(void)
{
    BattleTaskSlot *p;
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    p = BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix((void *)((char *)p + 4), 0x2000 /* 2.0 */, 0);
    (g_BattleSpecDarkWaveSpriteReq1.frame) = p->frameCounter;
    (g_BattleSpecDarkWaveSpriteReq1.drawObj) = Gfx_ModelTableGetSpriteObject(13, g_BattleSpecDarkWaveLoadedResTable);
    (g_BattleSpecDarkWaveSpriteReq1.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(13, g_BattleSpecDarkWaveLoadedResTable);
    g_BattleSpecDarkWaveDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecDarkWaveSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecDarkWaveDrawListHead);
    if (g_BattleIsPaused == 0) {
        (p->frameCounter)++;
        if (p->frameCounter == g_BattleSpecDarkWaveSpriteReq1.spriteGroup->header->frameCount) {
            p->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

// Per-frame tick for the current task slot. It installs the billboard for the slot's position at
// scale 0x4000 and keeps the returned matrix, submitting the sprite only when its translation z is
// positive. When it does, animation entry 0xC of g_BattleSpecDarkWaveLoadedResTable is
// loaded into g_BattleSpecDarkWaveSpriteReq2 and chained into g_BattleSpecDarkWaveDrawListHead. Unless
// g_BattleIsPaused is set, the position adds its velocities, frameIndex advances, and the slot is
// freed when frameIndex equals the frame count read through the request's sprite group.
// That check runs even when the submission was skipped, and no null-resource guard
// appears.
// FUNCTION: FF7 0x0052f725
void __cdecl BattleFx_SpecialDarkWaveRadialBurstSpriteTask(void)
{
    BattleFxEnergyOrbDriftSprite *burstPtr;
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    PsxMatrix *billboard;

    burstPtr = (BattleFxEnergyOrbDriftSprite *)BATTLE_CURRENT_TASK_SLOT;
    billboard = (PsxMatrix *)BattleFx_InstallBillboardMatrix((void *)((char *)burstPtr + 4), 0x4000 /* 4.0 */, 0);
    (g_BattleSpecDarkWaveSpriteReq2.frame) = burstPtr->frameIndex;
    if (billboard->t[2] > 0) {
        (g_BattleSpecDarkWaveSpriteReq2.drawObj) = Gfx_ModelTableGetSpriteObject(12, g_BattleSpecDarkWaveLoadedResTable);
        (g_BattleSpecDarkWaveSpriteReq2.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(12, g_BattleSpecDarkWaveLoadedResTable);
        g_BattleSpecDarkWaveDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecDarkWaveSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecDarkWaveDrawListHead);
    }
    if (g_BattleIsPaused == 0) {
        burstPtr->posX += burstPtr->velocityX;
        burstPtr->posY += burstPtr->velocityY;
        burstPtr->posZ += burstPtr->velocityZ;
        (burstPtr->frameIndex)++;
        if (burstPtr->frameIndex == g_BattleSpecDarkWaveSpriteReq2.spriteGroup->header->frameCount) {
            burstPtr->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Per-frame handler for a projectile effect (the current task slot). Draws its model with a rotation of
 * (-0x400, -0x400, 0) then the record's angleY, scaled by g_BattleSpecDarkWaveScaleVector,
 * choosing the model by phase (phases 10..19 index g_BattleSpecDarkWaveModelIndices); draws a cycling billboard sprite (entry 0xA) 0x64 in
 * front of it; from frame 0x14 trails twelve sprites of entry 0xB back along the heading,
 * alternately 0x64 and 0xC8 apart. While unpaused it also scatters ten small PSX primitives (code
 * 0x02, colour 0x80/0x40/0x20) within 0xC8 of that point, moves the record by an eighth of its
 * velocity plus a falling term (timer, decremented each frame) for the first 20 frames and by
 * the full velocity minus 10 in y after that, and retires it at frame 0x21. q, r and h are slots
 * the original's frame reserves.
 */
// FUNCTION: FF7 0x0052f837
void __cdecl BattleFx_SpecialDarkWaveProjectileTask(void)
{
    int viewMtx[8];
    int stepPrimBuf[2];
    int q[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int unusedSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short trailPt[4];
    BattleFxDarkWaveProjectile *projectilePtr;
    short trailStepZ;
    short trailStepX;
    short i;
    int sxy;
    short frameIdx;
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short rotation[4];
    short nextHeadPoint[4];
    BattleFxPolyF4Prim *spark;
    int transformRes;
    int matrix[8];
    int depthOut;

    projectilePtr = (BattleFxDarkWaveProjectile *)BATTLE_CURRENT_TASK_SLOT;
    rotation[0] = -0x400 /* 90 deg */;
    rotation[1] = -0x400 /* 90 deg */;
    rotation[2] = 0;
    RotMatrixXYZ(rotation, matrix);
    RotateMatrixY(projectilePtr->angleY, matrix);
    ScaleMatrix(matrix, &g_BattleSpecDarkWaveScaleVector);
    ((PsxMatrix *)matrix)->t[0] = projectilePtr->posX;
    ((PsxMatrix *)matrix)->t[1] = projectilePtr->posY;
    ((PsxMatrix *)matrix)->t[2] = projectilePtr->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, matrix, viewMtx);
    SetRotMatrix((void *)viewMtx);
    SetTransMatrix(viewMtx);
    if (projectilePtr->phase < 10) {
        frameIdx = 0;
    } else if (projectilePtr->phase >= 19) {
        frameIdx = 9;
    } else {
        frameIdx = projectilePtr->phase - 10;
    }
    ((BattleFxModelDrawRequest *)&g_BattleSpecDarkWaveModelReq)->modelHandle = Gfx_ModelTableGetRenderObject(((int *)&g_BattleSpecDarkWaveModelIndices)[frameIdx], g_BattleSpecDarkWaveLoadedResTable);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecDarkWaveDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecDarkWaveModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecDarkWaveDrawListHead);
    rotation[1] = projectilePtr->angleY;
    rotation[0] = rotation[2] = 0;
    nextHeadPoint[0] = nextHeadPoint[1] = 0;
    nextHeadPoint[2] = -0x64;
    RotMatrix((SVECTOR *)rotation, matrix);
    ((PsxMatrix *)matrix)->t[0] = projectilePtr->posX;
    ((PsxMatrix *)matrix)->t[1] = projectilePtr->posY;
    ((PsxMatrix *)matrix)->t[2] = projectilePtr->posZ;
    SetRotMatrix((void *)matrix);
    SetTransMatrix(matrix);
    RotTransPointToInt16(nextHeadPoint, nextHeadPoint, &depthOut);
    BattleFx_InstallBillboardMatrix(nextHeadPoint, 0x2000 /* 2.0 */, 0);
    g_BattleSpecDarkWaveSpriteReq3.drawObj = Gfx_ModelTableGetSpriteObject(10, g_BattleSpecDarkWaveLoadedResTable);
    g_BattleSpecDarkWaveSpriteReq3.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(10, g_BattleSpecDarkWaveLoadedResTable);
    g_BattleSpecDarkWaveSpriteReq3.frame = projectilePtr->phase % g_BattleSpecDarkWaveSpriteReq3.spriteGroup->header->frameCount;
    g_BattleSpecDarkWaveDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecDarkWaveSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecDarkWaveDrawListHead);
    if (projectilePtr->phase >= 20) {
        trailPt[0] = nextHeadPoint[0];
        trailPt[1] = nextHeadPoint[1];
        trailPt[2] = nextHeadPoint[2];
        trailStepX = -SinFixed(projectilePtr->angleY) * (projectilePtr->phase % 2 * 100 + 100) >> 12;
        trailStepZ = -CosFixed(projectilePtr->angleY) * (projectilePtr->phase % 2 * 100 + 100) >> 12;
        for (i = 0; i < 12; i++) {
            BattleFx_InstallBillboardMatrix(trailPt, 0x2000 /* 2.0 */, 0);
            g_BattleSpecDarkWaveSpriteReq4.frame = i >> 1;
            g_BattleSpecDarkWaveSpriteReq3.drawObj = Gfx_ModelTableGetSpriteObject(11, g_BattleSpecDarkWaveLoadedResTable);
            g_BattleSpecDarkWaveSpriteReq3.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(11, g_BattleSpecDarkWaveLoadedResTable);
            g_BattleSpecDarkWaveDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecDarkWaveSpriteReq4, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecDarkWaveDrawListHead);
            trailPt[0] += trailStepX;
            trailPt[2] += trailStepZ;
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    for (i = 0; i < 10; i++) {
        trailPt[0] = nextHeadPoint[0] + rand() % 400 - 200;
        trailPt[1] = nextHeadPoint[1] + rand() % 400 - 200;
        trailPt[2] = nextHeadPoint[2] + rand() % 400 - 200;
        spark = (BattleFxPolyF4Prim *)stepPrimBuf;
        spark->tag = spark->tag & 0xFFFFFF | 0x2000000;
        spark->primCode = 0x68;
        spark->primCode = 0x6A;
        transformRes = Gfx_TransformVertexSlot0ReturnDepth(trailPt, &sxy, &depthOut);
        if (transformRes > 0) {
            spark->colorR = 0x80;
            spark->colorG = 0x40;
            spark->colorB = 0x20;
            Gfx_DrawPsxPrimitive(spark);
        }
    }
    if (projectilePtr->phase < 20) {
        projectilePtr->posX += projectilePtr->velocityX >> 3;
        projectilePtr->posY += (projectilePtr->velocityY >> 3) + projectilePtr->timer;
        projectilePtr->posZ += projectilePtr->velocityZ >> 3;
        projectilePtr->timer -= 1;
    }
    if (projectilePtr->phase >= 20) {
        projectilePtr->posX += projectilePtr->velocityX;
        projectilePtr->posY += projectilePtr->velocityY - 10;
        projectilePtr->posZ += projectilePtr->velocityZ;
    }
    (projectilePtr->phase)++;
    if (projectilePtr->phase == 0x21) {
        projectilePtr->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
