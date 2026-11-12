// src/ff7/src/battle/battle_fx_special_mortar_effect.cpp
/* Enemy Special Attack [89]: Mortar (orig. SPECIAL/HAKUGEK2, 迫撃2) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Hakugek2': a per-target timeline between both actor bones,
//   driving oriented and gravity sprites, a spinning model and expanding shells.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle_3d.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x0089dfd0
int g_BattleSpecMortarResourceDefs = 0x001900B1;
int g_BattleSpecMortarResourceDefModel[1] = {
    (int)((char *)&g_BattleSpecMortarResourceNames + 0x4)
};
int g_BattleSpecMortarResourceDefEntries[38] = {
    0x0, 0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleSpecMortarResourceNames + 0xc), 0x0,
    0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleSpecMortarResourceNames + 0x18), 0x0, 0x0,
    0x0, 0x1900b1, (int)((char *)&g_BattleSpecMortarResourceNames + 0x24), 0x0, 0x0, 0x0,
    0x1900b1, (int)((char *)&g_BattleSpecMortarResourceNames + 0x2c), 0x0, 0x0, 0x0, 0x192a39,
    (int)((char *)&g_BattleSpecMortarResourceNames + 0x38), (int)((char *)&g_BattleSpecMortarResourceNames + 0x44), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleSpecMortarResourceNames + 0x50),
    (int)((char *)&g_BattleSpecMortarResourceNames + 0x5c), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleSpecMortarResourceNames + 0x68), (int)((char *)&g_BattleSpecMortarResourceNames + 0x74),
    0x0, 0x0
};
// GLOBAL: FF7 0x00b5dd4c
int g_BattleSpecMortarDrawListHead = 0;
// GLOBAL: FF7 0x00b5dd50
struct GfxTableCursor *g_BattleSpecMortarLoadedResTable = 0;
// clang-format on

// clang-format off
// GLOBAL: FF7 0x0089e070
struct BattleSpecDualMortarSpriteReq1 g_BattleSpecMortarSpriteReq1 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}, 0, 0};
// GLOBAL: FF7 0x0089e088
struct BattleSpecMortarSpriteReq2 g_BattleSpecMortarSpriteReq2 = {-128, -128, -128, {0x2e}, 0, {0}, 0, {0xa8, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}, 0, 0, {0}, 128};
unsigned char g_BattleSpecMortarSpriteReq2Tail[4] = {0x20, 0x00, 0x00, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x0089e0b0
short g_BattleSpecMortarShellScales[2] = {1536, 2560};
unsigned char g_rawData_0089e0b4[4] = {0x00, 0x0d, 0x00, 0x0f};
unsigned char g_BattleSpecMortarResourceNames[128] = {
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


/*
 * When the engine context exists and g_BattleSpecMortarLoadedResTable is empty, builds SPECIAL/HAKUGEK2 with a
 * default-duration record and creates resource type 8 using g_BattleSpecMortarResourceDefs.
 */
// FUNCTION: FF7 0x00522b00
void __cdecl BattleFx_SpecialLoadResMortar(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest currentRecord;
    int kind;
    char outSize[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecMortarLoadedResTable == 0) {
        kind = 8;
        resourceDefsWord = (int)&g_BattleSpecMortarResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0089e138
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/HAKUGEK2", outSize);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecMortarLoadedResTable, curOwnerCtx);
    }
}

/*
 * Forwards both arguments unchanged to BattleFx_SpecialBeginMortar; it adds nothing of its own.
 */
// FUNCTION: FF7 0x00522b90
void __cdecl BattleFx_SpecialEnterMortar(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginMortar(targetMask, actorIdx);
}

/*
 * Calls BattleFx_SpecialLoadResMortar and creates a mode-four actor-mask callback task using both arguments and
 * BattleFx_SpecialSpawnMortarPerTarget.
 */
// FUNCTION: FF7 0x00522ba5
void __cdecl BattleFx_SpecialBeginMortar(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResMortar();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnMortarPerTarget);
}

/*
 * Starts the timeline task for one target: records the source and target bone positions and both
 * actors, then plays the launch sound at the source actor.
 */
// FUNCTION: FF7 0x00522bc6
void __cdecl BattleFx_SpecialSpawnMortarPerTarget(int actor, int sourceActor)
{
    BattleFxMortarTimeline *slot;

    slot = (BattleFxMortarTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialMortarTimelineTask);
    Battle_GetBoneWorldPos(sourceActor, g_BattleActorRecords[sourceActor].targetBone, &slot->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &slot->endX);
    slot->recordType = actor;
    slot->actorIndex = sourceActor;
    Battle_PlaySfxPannedToActor(0x14, (short)sourceActor, 0);
}

/*
 * Drives the Hakugek2 timeline: an oriented sprite at the source actor on the first frame, then at
 * frame 0x14 the gravity sprites, expanding shells and spinning models at the target.
 */
// FUNCTION: FF7 0x00522c58
void __cdecl BattleFx_SpecialMortarTimelineTask(void)
{
    short i8;
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int b; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxMortarShellRecord *pShell;
    int t; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int y; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short theta;
    BattleFxMortarTimeline *taskPtr;
    int unusedSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int d; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int e; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int f; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int g; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int h; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int j; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    taskPtr = (BattleFxMortarTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->frame == 0) {
        theta = ArcTangentFixed(taskPtr->endX - taskPtr->startX, taskPtr->endZ - taskPtr->startZ);
        pShell = (BattleFxMortarShellRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialMortarOrientedSpriteTask);
        Battle_GetBoneWorldPos(taskPtr->actorIndex, g_BattleActorRecords[taskPtr->actorIndex].bone33, &pShell->posX);
        pShell->rotOrVelX = 0;
        pShell->spinRate = theta + 0x400;
        pShell->rotOrVelZ = 0xFE00;
    }
    if (taskPtr->frame == 0x14) {
        for (i8 = 0; i8 < 0x18; i8++) {
            pShell = (BattleFxMortarShellRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialMortarGravitySpriteTask);
            pShell->posX = taskPtr->endX + rand() % 0x190 + -0xC8;
            pShell->posZ = taskPtr->endZ + rand() % 0x190 + -0xC8;
            pShell->posY = 0;
            BattleFx_MakeRandomConeVector(0xFFFFFED4, 0x32, &pShell->rotOrVelX);
            pShell->variant = rand() % 9;
        }
        pShell = (BattleFxMortarShellRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialMortarExpandingShellsTask);
        pShell->posX = taskPtr->endX;
        pShell->posY = 0;
        pShell->posZ = taskPtr->endZ;
        for (i8 = 0; i8 < 5; i8++) {
            pShell = (BattleFxMortarShellRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialMortarSpinModelTask);
            pShell->posX = taskPtr->endX;
            pShell->posY = taskPtr->endY;
            pShell->posZ = taskPtr->endZ;
            pShell->posY = -(4 - i8) * 0x50;
            pShell->scale = g_BattleSpecMortarShellScales[i8];
            pShell->spinRate = -(4 - i8) * 0xA;
            pShell->angle = rand() % 0x1000;
        }
        Battle_PlaySfxPannedToActor(0xFA, (short)taskPtr->recordType, 0);
    }
    if (taskPtr->frame == 0x23) {
        Battle_PostActorHitOrHurtMessage(taskPtr->recordType);
    }
    (taskPtr->frame)++;
    if (taskPtr->frame == 0x2D) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the oriented beam sprite at the source actor until its sprite group runs out of frames.
 */
// FUNCTION: FF7 0x00522f57
void __cdecl BattleFx_SpecialMortarOrientedSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; PsxMatrix scaleMatrix; };
        struct { unsigned char pad_to_40[0x40]; short rowIndex; };
        struct { unsigned char pad_to_44[0x44]; int pointDepth; };
        struct { unsigned char pad_to_48[0x48]; BattleFxMortarShellRecord *slot; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxMortarShellRecord *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecMortarSpriteReq1.spriteEntry) = Gfx_ModelTableGetLoadedRecord(7, g_BattleSpecMortarLoadedResTable);
    g_BattleSpecMortarSpriteReq1.res4 = Gfx_ModelTableGetSpriteObject(7, g_BattleSpecMortarLoadedResTable);
    for (frame.rowIndex = 0; frame.rowIndex < 3; frame.rowIndex++) {
        frame.scaleMatrix.m[frame.rowIndex][2] = 0;
        frame.scaleMatrix.m[frame.rowIndex][1] = 0;
        frame.scaleMatrix.m[frame.rowIndex][0] = 0;
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((SVECTOR *)&frame.slot->posX, frame.scaleMatrix.t, &frame.pointDepth);
    frame.scaleMatrix.m[2][2] = 0x3000;
    frame.scaleMatrix.m[1][1] = frame.scaleMatrix.m[2][2];
    frame.scaleMatrix.m[0][0] = frame.scaleMatrix.m[1][1];
    ConcatMatrixIntoFirst(&frame.scaleMatrix, &g_BattleCameraTransform);
    RotMatrix((SVECTOR *)&frame.slot->rotOrVelX, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.scaleMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.scaleMatrix);
    SetTransMatrix(&frame.scaleMatrix);
    g_BattleSpecMortarSpriteReq1.frame = frame.slot->frameCounter;
    g_BattleSpecMortarDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecMortarSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecMortarDrawListHead);
    if (g_BattleIsPaused == 0 && (g_BattleSpecMortarSpriteReq1.spriteEntry) != 0) {
        (frame.slot->frameCounter)++;
        if (frame.slot->frameCounter >= ((BattleFxSpriteGroup *)g_BattleSpecMortarSpriteReq1.spriteEntry)->header->frameCount) {
            frame.slot->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws one debris sprite that falls under gravity and fades out over 0x14 frames.
 */
// FUNCTION: FF7 0x005230ee
void __cdecl BattleFx_SpecialMortarGravitySpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[104];
        int fadeAmount;
        struct { unsigned char pad_to_4[0x4]; SVECTOR angleX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; short fadeLevel; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix scaleMatrix; };
        struct { unsigned char pad_to_50[0x50]; int tableFields[3]; };
        struct { unsigned char pad_to_5C[0x5C]; short axisIndex; };
        struct { unsigned char pad_to_60[0x60]; int pointDepth; };
        struct { unsigned char pad_to_64[0x64]; BattleFxMortarGravitySpriteSlot *slot; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxMortarGravitySpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.tableFields[0] = 6;
    frame.tableFields[1] = 6;
    frame.tableFields[2] = 5;
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.scaleMatrix.m[2][frame.axisIndex] = 0;
        frame.scaleMatrix.m[1][frame.axisIndex] = 0;
        frame.scaleMatrix.m[0][frame.axisIndex] = 0;
    }
    if (frame.slot->frameCounter < 0xC) {
        frame.fadeAmount = 8;
    } else {
        frame.fadeAmount = 0x14 - frame.slot->frameCounter;
    }
    frame.fadeLevel = *(short *)&frame.fadeAmount;
    frame.angleX.vy = 0;
    frame.angleX.vx = frame.angleX.vy;
    frame.angleX.vz = frame.slot->spinAngle;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((SVECTOR *)&frame.slot->posX, frame.scaleMatrix.t, &frame.pointDepth);
    frame.scaleMatrix.m[2][2] = 0x2000;
    frame.scaleMatrix.m[1][1] = frame.scaleMatrix.m[2][2];
    frame.scaleMatrix.m[0][0] = frame.scaleMatrix.m[1][1];
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.scaleMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.scaleMatrix);
    SetTransMatrix(&frame.scaleMatrix);
    g_BattleSpecMortarSpriteReq2.frame = frame.slot->spriteIndex % 3;
    g_BattleSpecMortarSpriteReq1.res16 = Gfx_ModelTableGetLoadedRecord(frame.tableFields[frame.slot->spriteIndex / 3], g_BattleSpecMortarLoadedResTable);
    g_BattleSpecMortarSpriteReq1.res20 = Gfx_ModelTableGetSpriteObject(frame.tableFields[frame.slot->spriteIndex / 3], g_BattleSpecMortarLoadedResTable);
    g_BattleSpecMortarSpriteReq2.blue = frame.fadeLevel << 4;
    g_BattleSpecMortarSpriteReq2.green = g_BattleSpecMortarSpriteReq2.blue;
    (g_BattleSpecMortarSpriteReq2.red) = g_BattleSpecMortarSpriteReq2.green;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecMortarSpriteReq1.res16, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.slot->posX += frame.slot->velocityX;
    frame.slot->posY += frame.slot->velocityY;
    frame.slot->posZ += frame.slot->velocityZ;
    frame.slot->velocityY += 0x14;
    frame.slot->spinAngle += 0x200;
    (frame.slot->frameCounter)++;
    if (frame.slot->frameCounter == 0x14) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws one spinning, tilting model that rises for 0x14 frames.
 */
// FUNCTION: FF7 0x0052336d
void __cdecl BattleFx_SpecialMortarSpinModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        SVECTOR angleX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleFxMortarSpinModelSlot *modelSlot; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix modelMatrix; };
        struct { unsigned char pad_to_4C[0x4C]; VECTOR rollScale; };
    } frame;
#pragma pack(pop)

    frame.modelSlot = (BattleFxMortarSpinModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecMortarSpriteReq2.res8 = Gfx_ModelTableGetRenderObject(3, g_BattleSpecMortarLoadedResTable);
    frame.angleX.vz = 0;
    frame.angleX.vx = frame.angleX.vz;
    frame.angleX.vy = frame.modelSlot->tiltAngle;
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    frame.rollScale.vy = 0x1000;
    frame.rollScale.vz = frame.modelSlot->rollAngle;
    frame.rollScale.vx = frame.rollScale.vz;
    ScaleMatrix(&frame.rotMatrix, &frame.rollScale);
    frame.rotMatrix.t[0] = frame.modelSlot->posX;
    frame.rotMatrix.t[1] = frame.modelSlot->posY;
    frame.rotMatrix.t[2] = frame.modelSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    g_BattleSpecMortarSpriteReq2.alpha = frame.modelSlot->frameCounter * 0xC8;
    g_BattleSpecMortarDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecMortarSpriteReq2.res8, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecMortarDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.modelSlot->rollAngle += 0x200;
    frame.modelSlot->tiltAngle += 0x80;
    frame.modelSlot->posY += frame.modelSlot->velocityY;
    (frame.modelSlot->frameCounter)++;
    if (frame.modelSlot->frameCounter == 0x14) {
        frame.modelSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the expanding shell models around the target for 0x14 frames.
 */
// FUNCTION: FF7 0x005234e1
void __cdecl BattleFx_SpecialMortarExpandingShellsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleFxMortarExpandingShellsSlot *shellSlot; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_4C[0x4C]; VECTOR scaleZ; };
    } frame;
#pragma pack(pop)

    frame.shellSlot = (BattleFxMortarExpandingShellsSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vz = 0;
    frame.rotX.vx = frame.rotX.vz;
    frame.rotX.vy = frame.shellSlot->frameCounter << 6;
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    frame.scaleZ.vz = frame.shellSlot->frameCounter * 0xF0 + 0x600;
    frame.scaleZ.vy = frame.scaleZ.vz;
    frame.scaleZ.vx = frame.scaleZ.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleZ);
    frame.rotMatrix.t[0] = frame.shellSlot->startSize;
    frame.rotMatrix.t[1] = 0;
    frame.rotMatrix.t[2] = frame.shellSlot->endSize;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecMortarSpriteReq2.flags = 0x28;
    g_BattleSpecMortarSpriteReq2.res24 = Gfx_ModelTableGetRenderObject(0, g_BattleSpecMortarLoadedResTable);
    g_BattleSpecMortarSpriteReq2.secondAlpha = 0x80 - frame.shellSlot->frameCounter * 6;
    g_BattleSpecMortarDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecMortarSpriteReq2.res24, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecMortarDrawListHead);
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    frame.scaleZ.vz = frame.shellSlot->frameCounter * 0x140 + 0x800;
    frame.scaleZ.vy = frame.scaleZ.vz;
    frame.scaleZ.vx = frame.scaleZ.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleZ);
    frame.rotMatrix.t[0] = frame.shellSlot->startSize;
    frame.rotMatrix.t[1] = 0;
    frame.rotMatrix.t[2] = frame.shellSlot->endSize;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    g_BattleSpecMortarSpriteReq2.flags = 0x28;
    g_BattleSpecMortarSpriteReq2.res24 = Gfx_ModelTableGetRenderObject(2, g_BattleSpecMortarLoadedResTable);
    g_BattleSpecMortarSpriteReq2.secondAlpha = 0x80 - frame.shellSlot->frameCounter * 6;
    g_BattleSpecMortarDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecMortarSpriteReq2.res24, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecMortarDrawListHead);
    frame.scaleZ.vz = frame.shellSlot->frameCounter * 0xF0 + 0x600;
    frame.scaleZ.vy = frame.scaleZ.vz;
    frame.scaleZ.vx = frame.scaleZ.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleZ);
    frame.rotMatrix.t[0] = frame.shellSlot->startSize;
    frame.rotMatrix.t[1] = 0;
    frame.rotMatrix.t[2] = frame.shellSlot->endSize;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecMortarSpriteReq2.flags = 0xA8;
    g_BattleSpecMortarSpriteReq2.res24 = Gfx_ModelTableGetRenderObject(1, g_BattleSpecMortarLoadedResTable);
    g_BattleSpecMortarSpriteReq2.secondAlpha = frame.shellSlot->frameCounter * 0x30 + 0xC00;
    g_BattleSpecMortarDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecMortarSpriteReq2.res24, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecMortarDrawListHead);
    g_BattleSpecMortarSpriteReq2.flags = 0xA8;
    g_BattleSpecMortarSpriteReq2.res24 = Gfx_ModelTableGetRenderObject(4, g_BattleSpecMortarLoadedResTable);
    g_BattleSpecMortarSpriteReq2.secondAlpha = frame.shellSlot->frameCounter * 0x60;
    g_BattleSpecMortarDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecMortarSpriteReq2.res24, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecMortarDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.shellSlot->frameCounter)++;
    if (frame.shellSlot->frameCounter == 0x14) {
        frame.shellSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
