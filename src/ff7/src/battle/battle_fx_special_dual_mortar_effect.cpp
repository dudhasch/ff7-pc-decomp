// src/ff7/src/battle/battle_fx_special_dual_mortar_effect.cpp
/* Enemy Special Attack [88]: Dual Mortar (orig. SPECIAL/HAKUGEK1, 迫撃1) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Hakugek1': a per-target timeline between both actor bones, with
//   animated and falling debris sprites, an expanding model and expanding shells.

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
// GLOBAL: FF7 0x0089e150
int g_BattleSpecDualMortarResourceDefs = 0x001940B1;
int g_BattleSpecDualMortarResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialDualMortarEffectResourceNames + 0x4)
};
int g_BattleSpecDualMortarResourceDefsDataTail[38] = {
    0x0, 0x0, 0x0, 0x1940b1, (int)((char *)&g_BattleFxSpecialDualMortarEffectResourceNames + 0xc), 0x0,
    0x0, 0x0, 0x1940b1, (int)((char *)&g_BattleFxSpecialDualMortarEffectResourceNames + 0x18), 0x0, 0x0,
    0x0, 0x1940b1, (int)((char *)&g_BattleFxSpecialDualMortarEffectResourceNames + 0x24), 0x0, 0x0, 0x0,
    0x1940b1, (int)((char *)&g_BattleFxSpecialDualMortarEffectResourceNames + 0x2c), 0x0, 0x0, 0x0, 0x192a39,
    (int)((char *)&g_BattleFxSpecialDualMortarEffectResourceNames + 0x38), (int)((char *)&g_BattleFxSpecialDualMortarEffectResourceNames + 0x44), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialDualMortarEffectResourceNames + 0x50),
    (int)((char *)&g_BattleFxSpecialDualMortarEffectResourceNames + 0x5c), 0x0, 0x0, 0x196a39, (int)((char *)&g_BattleFxSpecialDualMortarEffectResourceNames + 0x68), (int)((char *)&g_BattleFxSpecialDualMortarEffectResourceNames + 0x74),
    0x0, 0x0
};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x0089e1f0
struct BattleSpecDualMortarSpriteReq1 g_BattleSpecDualMortarSpriteReq1 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}, 0, 0};
// GLOBAL: FF7 0x0089e208
struct BattleSpecDualMortarSpriteReq2 g_BattleSpecDualMortarSpriteReq2 = {-128, -128, -128, {0x2e}, 0, {0}, 0, {0xa8, 0, 0, 0, 0, 0}, 128};
unsigned char g_rawData_0089e21c[4] = {0x20, 0x00, 0x00, 0x00};
// clang-format on

// clang-format off
// The 16-byte model-draw request the shells task fills (model handle, flags, alpha) and submits.
typedef struct {
    int modelHandle;
    int flags;
    unsigned char pad_08[0x2];
    short alpha;
    int stateFlags;
} DualMortarShellDrawRequest;
// GLOBAL: FF7 0x0089e220
DualMortarShellDrawRequest g_BattleSpecDualMortarSpriteReq3 = {0, 0, {0}, 0x80, 0x20};
// GLOBAL: FF7 0x0089e230
short g_BattleSpecDualMortarShellScales[2] = {1536, 2560};
unsigned char g_rawData_0089e234[4] = {0x00, 0x0d, 0x00, 0x0f};
unsigned char g_BattleFxSpecialDualMortarEffectResourceNames[128] = {
    0x00, 0x10, 0x00, 0x00, 0x42, 0x4f, 0x4d, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x43, 0x4f, 0x56, 0x45, 0x52, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x00,
    0x57, 0x41, 0x4b, 0x4b, 0x41, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x00, 0x49, 0x4d, 0x50, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x49, 0x4d, 0x50, 0x41,
    0x43, 0x54, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x47, 0x45, 0x4b, 0x49, 0x30, 0x31, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x4b, 0x41, 0x4b, 0x45,
    0x52, 0x41, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x47, 0x45, 0x4b, 0x49, 0x30, 0x31, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x42, 0x41, 0x4c, 0x4c,
    0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x00, 0x47, 0x45, 0x4b, 0x49, 0x30, 0x30, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x44, 0x4f, 0x4b, 0x4b,
    0x41, 0x4e, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00
};
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b5dd54
int g_BattleSpecDualMortarDrawListHead = 0;
// GLOBAL: FF7 0x00b5dd58
struct GfxTableCursor *g_BattleSpecDualMortarLoadedResTable = 0;
// clang-format on

/*
 * When the engine context exists and g_BattleSpecDualMortarLoadedResTable is empty, builds SPECIAL/HAKUGEK1 with a
 * default-duration record and creates resource type 8 using g_BattleSpecDualMortarResourceDefs.
 */
// FUNCTION: FF7 0x00523850
void __cdecl BattleFx_SpecialLoadResDualMortar(void)
{
    GfxModelTableBank *stage;
    GfxResourceRequest requestSlot;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceTable;

    if (g_pBattle3dScene != 0 && g_BattleSpecDualMortarLoadedResTable == 0) {
        kind = 8;
        resourceTable = (int)&g_BattleSpecDualMortarResourceDefs;
        stage = (GfxModelTableBank *)g_pBattle3dScene->modelTableBank;
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        ((BattleFxDiamondCardResource *)pRequest)->groupTag = 0;
        // STRING: FF7 0x0089e2b8
        ((BattleFxDiamondCardResource *)pRequest)->resourcePath = (int)BattleFx_BuildResourcePath("SPECIAL/HAKUGEK1", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceTable, pRequest, &g_BattleSpecDualMortarLoadedResTable, stage);
    }
}

/*
 * Forwards both arguments unchanged to BattleFx_SpecialBeginDualMortar; it adds nothing of its own.
 */
// FUNCTION: FF7 0x005238e0
void __cdecl BattleFx_SpecialEnterDualMortar(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginDualMortar(targetMask, actorIdx);
}

/*
 * Calls BattleFx_SpecialLoadResDualMortar and creates a mode-four actor-mask callback task using both arguments and
 * BattleFx_SpecialSpawnDualMortarPerTarget.
 */
// FUNCTION: FF7 0x005238f5
void __cdecl BattleFx_SpecialBeginDualMortar(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResDualMortar();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnDualMortarPerTarget);
}

/*
 * Allocates BattleFx_SpecialDualMortarTimelineTask with the world positions of both actors' target
 * bones, stores both actors in targetIndex and sourceActor, and plays sound 0x14 panned to the
 * source actor.
 */
// FUNCTION: FF7 0x00523916
void __cdecl BattleFx_SpecialSpawnDualMortarPerTarget(int actor, int sourceActor)
{
    BattleFxJammerTimeline *slot;

    slot = (BattleFxJammerTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDualMortarTimelineTask);
    Battle_GetBoneWorldPos(sourceActor, g_BattleActorRecords[sourceActor].targetBone, (short *)slot->sourceBonePos);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)slot->targetBonePos);
    slot->targetIndex = actor;
    slot->sourceActor = sourceActor;
    Battle_PlaySfxPannedToActor(0x14, (short)sourceActor, 0);
}

/*
 * Per-target timeline: spawns the two animated sprites at the source bones on the first frame,
 * the falling debris, expanding shells and expanding models around the target on frame 0x14,
 * releases the target's record on frame 0x23 and ends on frame 0x2D.
 */
// FUNCTION: FF7 0x005239a8
void __cdecl BattleFx_SpecialDualMortarTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[80];
        struct { unsigned char pad_to_C[0xC]; short spawnX; };
        struct { unsigned char pad_to_E[0xE]; short centerY; };
        struct { unsigned char pad_to_10[0x10]; short spawnZ; };
        struct { unsigned char pad_to_34[0x34]; BattleFxDualMortarTimelineSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_38[0x38]; short partIndex; };
        struct { unsigned char pad_to_40[0x40]; short ringIndex; };
        struct { unsigned char pad_to_44[0x44]; short dirAngle; };
        struct { unsigned char pad_to_48[0x48]; BattleFxDualMortarTimelineSlot *timelineSlot; };
    } frame;
#pragma pack(pop)

    frame.timelineSlot = (BattleFxDualMortarTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timelineSlot->frameCounter == 0) {
        frame.dirAngle = ArcTangentFixed(frame.timelineSlot->centerX - frame.timelineSlot->startX, frame.timelineSlot->centerZ - frame.timelineSlot->startZ);
        frame.spawnSlot = (BattleFxDualMortarTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDualMortarAnimatedSpriteTask);
        Battle_GetBoneWorldPos(frame.timelineSlot->boneOwnerIndex, g_BattleActorRecords[frame.timelineSlot->boneOwnerIndex].bone36, (short *)&frame.spawnSlot->posX);
        frame.spawnSlot->rotOrVelX = 0;
        frame.spawnSlot->yaw = frame.dirAngle + 0x400;
        frame.spawnSlot->spinSpeed = 0xFE00 /* 5715 deg */;
        frame.spawnSlot = (BattleFxDualMortarTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDualMortarAnimatedSpriteTask);
        Battle_GetBoneWorldPos(frame.timelineSlot->boneOwnerIndex, g_BattleActorRecords[frame.timelineSlot->boneOwnerIndex].bone37, (short *)&frame.spawnSlot->posX);
        frame.spawnSlot->rotOrVelX = 0;
        frame.spawnSlot->yaw = frame.dirAngle + 0x400;
        frame.spawnSlot->spinSpeed = 0xFE00 /* 5715 deg */;
    }
    if (frame.timelineSlot->frameCounter == 20) {
        for (frame.ringIndex = 0; frame.ringIndex < 2; frame.ringIndex++) {
            frame.spawnX = frame.timelineSlot->centerX + (CosFixed((frame.ringIndex << 11) + 512) * 0x320 >> 12);
            frame.centerY = frame.timelineSlot->centerY;
            frame.spawnZ = frame.timelineSlot->centerZ + (SinFixed((frame.ringIndex << 11) + 512) * 0x320 >> 12);
            for (frame.partIndex = 0; frame.partIndex < 24; frame.partIndex++) {
                frame.spawnSlot = (BattleFxDualMortarTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDualMortarFallingDebrisSpriteTask);
                frame.spawnSlot->posX = frame.spawnX + rand() % 400 + -200;
                frame.spawnSlot->posZ = frame.spawnZ + rand() % 400 + -200;
                frame.spawnSlot->posY = 0;
                BattleFx_MakeRandomConeVector(0xFFFFFED4, 50, &frame.spawnSlot->rotOrVelX);
                frame.spawnSlot->variant = rand() % 9;
            }
            frame.spawnSlot = (BattleFxDualMortarTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDualMortarExpandingShellsTask);
            frame.spawnSlot->posX = frame.spawnX;
            frame.spawnSlot->posY = 0;
            frame.spawnSlot->posZ = frame.spawnZ;
            for (frame.partIndex = 0; frame.partIndex < 5; frame.partIndex++) {
                frame.spawnSlot = (BattleFxDualMortarTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDualMortarExpandingModelTask);
                frame.spawnSlot->posX = frame.spawnX;
                frame.spawnSlot->posY = frame.centerY;
                frame.spawnSlot->posZ = frame.spawnZ;
                frame.spawnSlot->posY = -(4 - frame.partIndex) * 80;
                frame.spawnSlot->scale = g_BattleSpecDualMortarShellScales[frame.partIndex];
                frame.spawnSlot->yaw = -(4 - frame.partIndex) * 0xA;
                frame.spawnSlot->spinAngle = (rand() % 4096);
            }
        }
        Battle_PlaySfxPannedToActor(0xFA, (short)frame.timelineSlot->actorIndex, 0);
    }
    if (frame.timelineSlot->frameCounter == 35) {
        Battle_PostActorHitOrHurtMessage(frame.timelineSlot->actorIndex);
    }
    (frame.timelineSlot->frameCounter)++;
    if (frame.timelineSlot->frameCounter == 45) {
        frame.timelineSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws one animated sprite at its slot position, turned by its rotation vector, and frees the
 * slot when the sprite resource's duration has elapsed.
 */
// FUNCTION: FF7 0x00523d85
void __cdecl BattleFx_SpecialDualMortarAnimatedSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; PsxMatrix spriteMatrix; };
        struct { unsigned char pad_to_40[0x40]; short axisIndex; };
        struct { unsigned char pad_to_44[0x44]; int projDepth; };
        struct { unsigned char pad_to_48[0x48]; BattleFxDualMortarTimelineSpawnSlot *spriteSlot; };
    } frame;
#pragma pack(pop)

    frame.spriteSlot = (BattleFxDualMortarTimelineSpawnSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecDualMortarSpriteReq1.spriteEntry) = Gfx_ModelTableGetLoadedRecord(7, g_BattleSpecDualMortarLoadedResTable);
    g_BattleSpecDualMortarSpriteReq1.res4 = Gfx_ModelTableGetSpriteObject(7, g_BattleSpecDualMortarLoadedResTable);
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.spriteMatrix.m[frame.axisIndex][2] = 0;
        frame.spriteMatrix.m[frame.axisIndex][1] = 0;
        frame.spriteMatrix.m[frame.axisIndex][0] = 0;
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((SVECTOR *)&frame.spriteSlot->posX, frame.spriteMatrix.t, &frame.projDepth);
    frame.spriteMatrix.m[2][2] = 0x3000;
    frame.spriteMatrix.m[1][1] = frame.spriteMatrix.m[2][2];
    frame.spriteMatrix.m[0][0] = frame.spriteMatrix.m[1][1];
    ConcatMatrixIntoFirst(&frame.spriteMatrix, &g_BattleCameraTransform);
    RotMatrix((SVECTOR *)&frame.spriteSlot->rotOrVelX, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.spriteMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.spriteMatrix);
    SetTransMatrix(&frame.spriteMatrix);
    g_BattleSpecDualMortarSpriteReq1.frame = frame.spriteSlot->frameCounter;
    g_BattleSpecDualMortarDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecDualMortarSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecDualMortarDrawListHead);
    if (g_BattleIsPaused == 0 && (g_BattleSpecDualMortarSpriteReq1.spriteEntry) != 0) {
        (frame.spriteSlot->frameCounter)++;
        if (frame.spriteSlot->frameCounter >= *(int *)((char *)*(int *)(g_BattleSpecDualMortarSpriteReq1.spriteEntry) + 4)) {
            frame.spriteSlot->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws one debris sprite that falls under gravity, fading by the frame counter, and frees the
 * slot after 20 frames.
 */
// FUNCTION: FF7 0x00523f1c
void __cdecl BattleFx_SpecialDualMortarFallingDebrisSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[104];
        int rampValue;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; short rampShort; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix debrisMatrix; };
        struct { unsigned char pad_to_50[0x50]; int resIdx[3]; };
        struct { unsigned char pad_to_5C[0x5C]; short axisIndex; };
        struct { unsigned char pad_to_60[0x60]; int projDepth; };
        struct { unsigned char pad_to_64[0x64]; BattleFxMortarGravitySpriteSlot *debrisSlot; };
    } frame;
#pragma pack(pop)

    frame.debrisSlot = (BattleFxMortarGravitySpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.resIdx[0] = 6;
    frame.resIdx[1] = 6;
    frame.resIdx[2] = 5;
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.debrisMatrix.m[2][frame.axisIndex] = 0;
        frame.debrisMatrix.m[1][frame.axisIndex] = 0;
        frame.debrisMatrix.m[0][frame.axisIndex] = 0;
    }
    if (frame.debrisSlot->frameCounter < 12) {
        frame.rampValue = 8;
    } else {
        frame.rampValue = 0x14 - frame.debrisSlot->frameCounter;
    }
    frame.rampShort = *(short *)&frame.rampValue;
    frame.rotX.vy = 0;
    frame.rotX.vx = frame.rotX.vy;
    frame.rotX.vz = frame.debrisSlot->spinAngle;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((SVECTOR *)&frame.debrisSlot->posX, frame.debrisMatrix.t, &frame.projDepth);
    frame.debrisMatrix.m[2][2] = 0x2000;
    frame.debrisMatrix.m[1][1] = frame.debrisMatrix.m[2][2];
    frame.debrisMatrix.m[0][0] = frame.debrisMatrix.m[1][1];
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.debrisMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.debrisMatrix);
    SetTransMatrix(&frame.debrisMatrix);
    g_BattleSpecDualMortarSpriteReq2.frame = frame.debrisSlot->spriteIndex % 3;
    g_BattleSpecDualMortarSpriteReq1.res16 = Gfx_ModelTableGetLoadedRecord(frame.resIdx[frame.debrisSlot->spriteIndex / 3], g_BattleSpecDualMortarLoadedResTable);
    g_BattleSpecDualMortarSpriteReq1.res20 = Gfx_ModelTableGetSpriteObject(frame.resIdx[frame.debrisSlot->spriteIndex / 3], g_BattleSpecDualMortarLoadedResTable);
    g_BattleSpecDualMortarSpriteReq2.blue = frame.rampShort << 4;
    g_BattleSpecDualMortarSpriteReq2.green = g_BattleSpecDualMortarSpriteReq2.blue;
    (g_BattleSpecDualMortarSpriteReq2.red) = g_BattleSpecDualMortarSpriteReq2.green;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecDualMortarSpriteReq1.res16, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.debrisSlot->posX += frame.debrisSlot->velocityX;
    frame.debrisSlot->posY += frame.debrisSlot->velocityY;
    frame.debrisSlot->posZ += frame.debrisSlot->velocityZ;
    frame.debrisSlot->velocityY += 20;
    frame.debrisSlot->spinAngle += 0x200 /* 45 deg */;
    (frame.debrisSlot->frameCounter)++;
    if (frame.debrisSlot->frameCounter == 20) {
        frame.debrisSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws one expanding model that spins and tilts, and frees the slot after 20 frames.
 */
// FUNCTION: FF7 0x0052419b
void __cdecl BattleFx_SpecialDualMortarExpandingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleFxDualMortarExpandingModelSlot *modelSlot; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_4C[0x4C]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.modelSlot = (BattleFxDualMortarExpandingModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecDualMortarSpriteReq2.res8 = Gfx_ModelTableGetRenderObject(3, g_BattleSpecDualMortarLoadedResTable);
    frame.rotX.vz = 0;
    frame.rotX.vx = frame.rotX.vz;
    frame.rotX.vy = frame.modelSlot->spinAngle;
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    frame.scaleX.vy = 0x1000 /* 1.0 */;
    frame.scaleX.vz = frame.modelSlot->tiltAngle;
    frame.scaleX.vx = frame.scaleX.vz;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    frame.rotMatrix.t[0] = frame.modelSlot->posX;
    frame.rotMatrix.t[1] = frame.modelSlot->posY;
    frame.rotMatrix.t[2] = frame.modelSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecDualMortarSpriteReq2.alpha = frame.modelSlot->frameCounter * 200;
    g_BattleSpecDualMortarDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecDualMortarSpriteReq2.res8, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecDualMortarDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.modelSlot->tiltAngle += 0x200 /* 45 deg */;
    frame.modelSlot->spinAngle += 0x80;
    frame.modelSlot->posY += frame.modelSlot->velocityY;
    (frame.modelSlot->frameCounter)++;
    if (frame.modelSlot->frameCounter == 20) {
        frame.modelSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws one frame of the expanding shells as four model packets: the first, second and third
 * build a rotation from the timer (angle = timer << 6), scale it, compose it with
 * g_BattleCameraTransform and submit it through BattleFx_SubmitModelDraw with the model handle,
 * flags and alpha of g_BattleSpecDualMortarSpriteReq3; the fourth reuses the last transform. The
 * timer advances while unpaused and the slot is freed at 0x14.
 */
// FUNCTION: FF7 0x0052431d
void __cdecl BattleFx_SpecialDualMortarExpandingShellsTask(void)
{
    int scale[4];
    int composedMat[8];
    short euler[4];
    int rotMat[8];
    BattleFxDualMortarShellRecord *curPtr;

    curPtr = (BattleFxDualMortarShellRecord *)BATTLE_CURRENT_TASK_SLOT;
    euler[2] = 0;
    euler[0] = euler[2];
    euler[1] = curPtr->timer << 6;
    RotMatrix((SVECTOR *)euler, rotMat);
    scale[2] = curPtr->timer * 0xF0 + 0x600;
    scale[1] = scale[2];
    scale[0] = scale[1];
    ScaleMatrix(rotMat, scale);
    *(int *)((char *)rotMat + 0x12) = curPtr->startSize;
    *(int *)((char *)rotMat + 0x16) = 0;
    ((PsxMatrix *)rotMat)->t[2] = curPtr->endSize;
    MultiplyMatrix(&g_BattleCameraTransform, rotMat, composedMat);
    SetRotMatrix(composedMat);
    SetTransMatrix(composedMat);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecDualMortarSpriteReq3.flags = 0x28;
    g_BattleSpecDualMortarSpriteReq3.modelHandle = Gfx_ModelTableGetRenderObject(0, g_BattleSpecDualMortarLoadedResTable);
    g_BattleSpecDualMortarSpriteReq3.alpha = 0x80 - curPtr->timer * 6;
    g_BattleSpecDualMortarDrawListHead = BattleFx_SubmitModelDraw(
        (BattleFxModelDraw *)&g_BattleSpecDualMortarSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecDualMortarDrawListHead);
    RotMatrix((SVECTOR *)euler, rotMat);
    scale[2] = curPtr->timer * 0x140 + 0x800;
    scale[1] = scale[2];
    scale[0] = scale[1];
    ScaleMatrix(rotMat, scale);
    *(int *)((char *)rotMat + 0x12) = curPtr->startSize;
    *(int *)((char *)rotMat + 0x16) = 0;
    ((PsxMatrix *)rotMat)->t[2] = curPtr->endSize;
    MultiplyMatrix(&g_BattleCameraTransform, rotMat, composedMat);
    SetRotMatrix(composedMat);
    SetTransMatrix(composedMat);
    g_BattleSpecDualMortarSpriteReq3.flags = 0x28;
    g_BattleSpecDualMortarSpriteReq3.modelHandle = Gfx_ModelTableGetRenderObject(2, g_BattleSpecDualMortarLoadedResTable);
    g_BattleSpecDualMortarSpriteReq3.alpha = 0x80 - curPtr->timer * 6;
    g_BattleSpecDualMortarDrawListHead = BattleFx_SubmitModelDraw(
        (BattleFxModelDraw *)&g_BattleSpecDualMortarSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecDualMortarDrawListHead);
    RotMatrix((SVECTOR *)euler, rotMat);
    scale[2] = curPtr->timer * 0xF0 + 0x600;
    scale[1] = scale[2];
    scale[0] = scale[1];
    ScaleMatrix(rotMat, scale);
    *(int *)((char *)rotMat + 0x12) = curPtr->startSize;
    *(int *)((char *)rotMat + 0x16) = 0;
    ((PsxMatrix *)rotMat)->t[2] = curPtr->endSize;
    MultiplyMatrix(&g_BattleCameraTransform, rotMat, composedMat);
    SetRotMatrix(composedMat);
    SetTransMatrix(composedMat);
    g_BattleSpecDualMortarSpriteReq3.flags = 0xA8;
    g_BattleSpecDualMortarSpriteReq3.modelHandle = Gfx_ModelTableGetRenderObject(1, g_BattleSpecDualMortarLoadedResTable);
    g_BattleSpecDualMortarSpriteReq3.alpha = curPtr->timer * 48 + 0xC00;
    g_BattleSpecDualMortarDrawListHead = BattleFx_SubmitModelDraw(
        (BattleFxModelDraw *)&g_BattleSpecDualMortarSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecDualMortarDrawListHead);
    g_BattleSpecDualMortarSpriteReq3.flags = 0xA8;
    g_BattleSpecDualMortarSpriteReq3.modelHandle = Gfx_ModelTableGetRenderObject(4, g_BattleSpecDualMortarLoadedResTable);
    g_BattleSpecDualMortarSpriteReq3.alpha = curPtr->timer * 96;
    g_BattleSpecDualMortarDrawListHead = BattleFx_SubmitModelDraw(
        (BattleFxModelDraw *)&g_BattleSpecDualMortarSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecDualMortarDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (curPtr->timer)++;
    if (curPtr->timer == 20) {
        curPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
