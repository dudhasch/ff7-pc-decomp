// src/ff7/src/battle/battle_fx_special_jammer_effect.cpp
/* Enemy Special Attack [113]: Sonic Jammer (orig. SPECIAL/JAMMER) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Jammer': a per-target timeline with a variant sprite, a growing
//   model and a spinning fade sprite task.

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
// GLOBAL: FF7 0x0089dda8
int g_BattleSpecJammerResourceDefs = 0x001900B1;
int g_BattleSpecJammerResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialJammerEffectResourceNames + 0x0)
};
unsigned int g_BattleSpecJammerResourceDefsDataTail[54] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089decc
    (unsigned int)"JAMMA00.TIM",
    // STRING: FF7 0x0089ded8
    (unsigned int)"ONPA1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089dee4
    (unsigned int)"JAMMA00.TIM",
    // STRING: FF7 0x0089def0
    (unsigned int)"ONPA2.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089defc
    (unsigned int)"JAMMA00.TIM",
    // STRING: FF7 0x0089df08
    (unsigned int)"ONPA3.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089df14
    (unsigned int)"JAMMA00.TIM",
    // STRING: FF7 0x0089df20
    (unsigned int)"ONPA4.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089df2c
    (unsigned int)"JAMMA00.TIM",
    // STRING: FF7 0x0089df38
    (unsigned int)"ONPA5.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089df44
    (unsigned int)"JAMMA00.TIM",
    // STRING: FF7 0x0089df50
    (unsigned int)"ONPA6.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089df5c
    (unsigned int)"JAMMA00.TIM",
    // STRING: FF7 0x0089df68
    (unsigned int)"ONPA7.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089df74
    (unsigned int)"JAMMA01.TIM",
    // STRING: FF7 0x0089df80
    (unsigned int)"HIKARI1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089df8c
    (unsigned int)"JAMMA01.TIM",
    // STRING: FF7 0x0089df98
    (unsigned int)"HIKARI2.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089dfa4
    (unsigned int)"JAMMA01.TIM",
    // STRING: FF7 0x0089dfb0
    (unsigned int)"HIKARI3.SPT",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x0089de88
struct BattleLimitStardustRayModelReq2 g_BattleSpecJammerSpriteReq1 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
// GLOBAL: FF7 0x0089de98
struct BattleSpecJammerSpriteReq2 g_BattleSpecJammerSpriteReq2 = {0, {0xa8, 0, 0, 0, 0, 0}, 128};
unsigned char g_rawData_0089dea4[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0089dea8
struct BattleSpecShowerModelParams g_BattleSpecJammerScale16 = {{0, 0xfc, 0, 0, 0, 0, 0, 0}, 0, 0};
// GLOBAL: FF7 0x0089deb8
struct BattleSpecHeatBlastColor g_BattleSpecJammerSpriteReq3 = {-128, -128, -128, {0x2e}};
unsigned char g_BattleFxSpecialJammerEffectResourceNames[8] = {0x48, 0x49, 0x4b, 0x41, 0x52, 0x49, 0x2e, 0x52};
unsigned char g_rawData_0089dec8[4] = {0x53, 0x44, 0x00, 0x00};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b5dd40
int g_BattleSpecJammerDrawListHead = 0;
unsigned char g_bssPad_00b5dd44[4] = {0};
// GLOBAL: FF7 0x00b5dd48
struct GfxTableCursor *g_BattleSpecJammerLoadedResTable = 0;
// clang-format on

/*
 * When the engine context exists and g_BattleSpecJammerLoadedResTable is empty, builds SPECIAL/JAMMER with a default-
 * duration record and creates resource type 11 using g_BattleSpecJammerResourceDefs.
 */
// FUNCTION: FF7 0x00522010
void __cdecl BattleFx_SpecialLoadResJammer(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest currentRecord;
    int kind;
    char outSize[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecJammerLoadedResTable == 0) {
        kind = 0xB;
        resourceDefsWord = (int)&g_BattleSpecJammerResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0089dfbc
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/JAMMER", outSize);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecJammerLoadedResTable, curOwnerCtx);
    }
}

/*
 * Forwards both arguments unchanged to BattleFx_SpecialBeginJammer.
 */
// FUNCTION: FF7 0x005220a0
void __cdecl BattleFx_SpecialEnterJammer(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginJammer(targetMask, actorIdx);
}

/*
 * Calls BattleFx_SpecialLoadResJammer and creates a mode-four actor-mask callback task using both arguments and
 * BattleFx_SpecialSpawnJammerPerTarget.
 */
// FUNCTION: FF7 0x005220b5
void __cdecl BattleFx_SpecialBeginJammer(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResJammer();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnJammerPerTarget);
}

/*
 * Allocates a BattleFx_SpecialJammerTimelineTask slot, records the source actor's bone34 position
 * as sourceBonePos and the target's targetBone position as targetBonePos, stores both actors in
 * targetIndex and sourceActor, and plays sound 0x11E panned to the source actor.
 */
// FUNCTION: FF7 0x005220d6
void __cdecl BattleFx_SpecialSpawnJammerPerTarget(int actor, int sourceActor)
{
    BattleFxJammerTimeline *slot;

    slot = (BattleFxJammerTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialJammerTimelineTask);
    Battle_GetBoneWorldPos(sourceActor, g_BattleActorRecords[sourceActor].bone34, (short *)slot->sourceBonePos);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)slot->targetBonePos);
    slot->targetIndex = actor;
    slot->sourceActor = sourceActor;
    Battle_PlaySfxPannedToActor(0x11E, (short)sourceActor, 0);
}

/*
 * Per-target timeline: spawns the spin-fade sprites along the bolt for the first 20 frames, the
 * growing model at frame 0x18, variant sprites around the target until frame 0x24, and ends at 0x32.
 */
// FUNCTION: FF7 0x0052216b
void __cdecl BattleFx_SpecialJammerTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        SVECTOR angleX;
        struct { unsigned char pad_to_8[0x8]; int rotOut; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offsetX; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxMultiJammerTimelineSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_3C[0x3C]; short i; };
        struct { unsigned char pad_to_40[0x40]; short randAngle; };
        struct { unsigned char pad_to_44[0x44]; BattleFxJammerTimelineSlot *slot; };
        struct { unsigned char pad_to_48[0x48]; short loopAngle; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxJammerTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.slot->frameCounter < 0x14) {
        frame.spawnSlot = (BattleFxMultiJammerTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialJammerSpinFadeSpriteTask);
        frame.spawnSlot->posA = frame.slot->posA;
        frame.spawnSlot->posB = frame.slot->posB;
        frame.spawnSlot->posC = frame.slot->posC;
        frame.spawnSlot->velA = (frame.slot->velA - frame.slot->posA) / 0xF;
        frame.spawnSlot->velB = (frame.slot->velB - frame.slot->posB) / 0xF;
        frame.spawnSlot->velC = (frame.slot->velC - frame.slot->posC) / 0xF;
        frame.randAngle = (rand() % 4096);
        frame.offsetX.vx = CosFixed(frame.randAngle) * 0x3C >> 0xC;
        frame.offsetX.vy = SinFixed(frame.randAngle) * 0x3C >> 0xC;
        frame.offsetX.vz = 0;
        frame.angleX.vz = 0;
        frame.angleX.vx = frame.angleX.vz;
        frame.spawnSlot->angle = ArcTangentFixed(frame.slot->velA - frame.slot->posA, frame.slot->velC - frame.slot->posC);
        frame.angleX.vy = frame.spawnSlot->angle;
        RotMatrix(&frame.angleX, &frame.rotMatrix);
        frame.rotMatrix.t[0] = frame.spawnSlot->velA;
        frame.rotMatrix.t[1] = frame.spawnSlot->velB;
        frame.rotMatrix.t[2] = frame.spawnSlot->velC;
        SetRotMatrix(&frame.rotMatrix);
        SetTransMatrix(&frame.rotMatrix);
        RotTransPointToInt16(&frame.offsetX, &frame.spawnSlot->velA, &frame.rotOut);
        frame.spawnSlot->variant = rand() % 7;
    }
    if (frame.slot->frameCounter == 0x18) {
        frame.spawnSlot = (BattleFxMultiJammerTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialJammerGrowingModelTask);
        frame.spawnSlot->posA = frame.slot->velA;
        frame.spawnSlot->posB = frame.slot->velB;
        frame.spawnSlot->posC = frame.slot->velC;
        Battle_PlaySfxPannedToActor(0x189, (short)frame.slot->actorIndex, 0);
    }
    if (frame.slot->frameCounter > 0x18 && frame.slot->frameCounter < 0x24) {
        for (frame.i = 0; frame.i < 2; frame.i++) {
            frame.spawnSlot = (BattleFxMultiJammerTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialJammerVariantSpriteTask);
            Battle_GetBoneWorldPos(frame.slot->actorIndex, g_BattleActorRecords[frame.slot->actorIndex].targetBone, (short *)&frame.spawnSlot->posA);
            frame.spawnSlot->posA += (short)(rand() % 0x4B0 - 0x258);
            frame.spawnSlot->posB += (short)(rand() % 0x4B0 - 0x258);
            frame.spawnSlot->posC += (short)(rand() % 0x4B0 - 0x258);
            frame.spawnSlot->variant = rand() % 3;
            frame.spawnSlot->unusedScale = (rand() % 3 << 0xB) + 0x1000;
            frame.loopAngle = (rand() % 4096);
            frame.offsetX.vx = CosFixed(frame.loopAngle) * 0x3C >> 0xC;
            frame.offsetX.vy = SinFixed(frame.loopAngle) * 0x3C >> 0xC;
            frame.offsetX.vz = 0;
            frame.angleX.vz = 0;
            frame.angleX.vx = frame.angleX.vz;
            frame.angleX.vy = (rand() % 4096);
            RotMatrix(&frame.angleX, &frame.rotMatrix);
            frame.rotMatrix.t[2] = 0;
            frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
            frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
            SetRotMatrix(&frame.rotMatrix);
            SetTransMatrix(&frame.rotMatrix);
            RotTransPointToInt16(&frame.offsetX, &frame.spawnSlot->velA, &frame.rotOut);
        }
    }
    if (frame.slot->frameCounter == 0x1E) {
        Battle_PostActorHitOrHurtMessage(frame.slot->actorIndex);
    }
    (frame.slot->frameCounter)++;
    if (frame.slot->frameCounter == 0x32) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Per-frame draw of the Jammer variant sprite. The sprite is one of the table entries 8 to 0xA,
 * chosen by the slot's variant. A billboard matrix is installed at the slot's position at scale
 * 0x1800, and the frame index is published before the draw is submitted, which happens before the
 * pause check. While the battle is not paused, the velocities are added to posA, posB and posC even
 * when the sprite is null. Only with a nonnull sprite does frameIndex advance, and the slot is
 * retired once frameIndex reaches the frame count in the sprite header.
 */
// FUNCTION: FF7 0x005225e7
void __cdecl BattleFx_SpecialJammerVariantSpriteTask(void)
{
    struct {
        int table[3];
        BattleFxJammerVariantSprite *slot;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = (BattleFxJammerVariantSprite *)BATTLE_CURRENT_TASK_SLOT;
    frame.table[0] = 8;
    frame.table[1] = 9;
    frame.table[2] = 0xA;
    BattleFx_InstallBillboardMatrix(&frame.slot->posA, 0x1800, 0);
    g_BattleSpecJammerSpriteReq1.frameIndex = frame.slot->frameIndex;
    (g_BattleSpecJammerSpriteReq1.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(frame.table[frame.slot->variant], g_BattleSpecJammerLoadedResTable);
    g_BattleSpecJammerSpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(frame.table[frame.slot->variant], g_BattleSpecJammerLoadedResTable);
    g_BattleSpecJammerDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecJammerSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecJammerDrawListHead);
    if (g_BattleIsPaused == 0) {
        frame.slot->posA += frame.slot->velA;
        frame.slot->posB += frame.slot->velB;
        frame.slot->posC += frame.slot->velC;
        if ((g_BattleSpecJammerSpriteReq1.spriteGroup) != 0) {
            (frame.slot->frameIndex)++;
            if (frame.slot->frameIndex >= g_BattleSpecJammerSpriteReq1.spriteGroup->header->frameCount) {
                frame.slot->ownerSlot = BATTLE_TASK_FREE;
            }
        }
    }
}

/*
 * Draws the growing Jammer model at the slot's position, scaling it up each frame; ends after 0xF frames.
 */
// FUNCTION: FF7 0x00522721
void __cdecl BattleFx_SpecialJammerGrowingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int pointDepth;
        struct { unsigned char pad_to_4[0x4]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix scaleMatrix; };
        struct { unsigned char pad_to_48[0x48]; short axisIndex; };
        struct { unsigned char pad_to_50[0x50]; BattleTaskSlot *modelSlot; };
    } frame;
#pragma pack(pop)

    frame.modelSlot = BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecJammerSpriteReq2.modelResource) = Gfx_ModelTableGetRenderObject(0, g_BattleSpecJammerLoadedResTable);
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.scaleMatrix.m[2][frame.axisIndex] = 0;
        frame.scaleMatrix.m[1][frame.axisIndex] = 0;
        frame.scaleMatrix.m[0][frame.axisIndex] = 0;
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint(frame.modelSlot->data, frame.scaleMatrix.t, &frame.pointDepth);
    frame.scaleMatrix.m[2][2] = frame.modelSlot->frameCounter * 0x258 + 0x800;
    frame.scaleMatrix.m[1][1] = frame.scaleMatrix.m[2][2];
    frame.scaleMatrix.m[0][0] = frame.scaleMatrix.m[1][1];
    RotMatrix((SVECTOR *)&g_BattleSpecJammerScale16, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.scaleMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.scaleMatrix);
    SetTransMatrix(&frame.scaleMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecJammerSpriteReq2.alpha = frame.modelSlot->frameCounter << 8;
    g_BattleSpecJammerDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecJammerSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecJammerDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.modelSlot->frameCounter)++;
    if (frame.modelSlot->frameCounter == 0xF) {
        frame.modelSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws a spinning, fading Jammer sprite that drifts by its velocity; ends after 0x14 frames.
 */
// FUNCTION: FF7 0x0052289b
void __cdecl BattleFx_SpecialJammerSpinFadeSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        int fadeAmount;
        struct { unsigned char pad_to_4[0x4]; SVECTOR angleX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; short fadeLevel; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix scaleMatrix; };
        struct { unsigned char pad_to_50[0x50]; int tableFields[7]; };
        struct { unsigned char pad_to_6C[0x6C]; short rowIndex; };
        struct { unsigned char pad_to_70[0x70]; int pointDepth; };
        struct { unsigned char pad_to_74[0x74]; BattleFxJammerSpinFadeSpriteSlot *slot; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxJammerSpinFadeSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.tableFields[0] = 1;
    frame.tableFields[1] = 2;
    frame.tableFields[2] = 3;
    frame.tableFields[3] = 4;
    frame.tableFields[4] = 5;
    frame.tableFields[5] = 6;
    frame.tableFields[6] = 7;
    frame.angleX.vy = 0;
    frame.angleX.vx = 0;
    frame.angleX.vz = frame.slot->frameCounter << 8;
    for (frame.rowIndex = 0; frame.rowIndex < 3; frame.rowIndex++) {
        frame.scaleMatrix.m[frame.rowIndex][2] = 0;
        frame.scaleMatrix.m[frame.rowIndex][1] = 0;
        frame.scaleMatrix.m[frame.rowIndex][0] = 0;
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((char *)frame.slot + 4, frame.scaleMatrix.t, &frame.pointDepth);
    frame.scaleMatrix.m[2][2] = 0x1400;
    frame.scaleMatrix.m[1][1] = frame.scaleMatrix.m[2][2];
    frame.scaleMatrix.m[0][0] = frame.scaleMatrix.m[1][1];
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.scaleMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.scaleMatrix);
    SetTransMatrix(&frame.scaleMatrix);
    if (frame.slot->frameCounter < 0xC) {
        frame.fadeAmount = 8;
    } else {
        frame.fadeAmount = 0x14 - frame.slot->frameCounter;
    }
    frame.fadeLevel = *(short *)&frame.fadeAmount;
    g_BattleSpecJammerScale16.res8 = Gfx_ModelTableGetLoadedRecord(frame.tableFields[frame.slot->tableIndex], g_BattleSpecJammerLoadedResTable);
    g_BattleSpecJammerScale16.res12 = Gfx_ModelTableGetSpriteObject(frame.tableFields[frame.slot->tableIndex], g_BattleSpecJammerLoadedResTable);
    g_BattleSpecJammerSpriteReq3.blue = frame.fadeLevel << 4;
    g_BattleSpecJammerSpriteReq3.green = g_BattleSpecJammerSpriteReq3.blue;
    (g_BattleSpecJammerSpriteReq3.red) = g_BattleSpecJammerSpriteReq3.green;
    g_BattleSpecJammerDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecJammerScale16.res8, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecJammerDrawListHead);
    if (g_BattleIsPaused == 0) {
        frame.slot->posA += frame.slot->velA;
        frame.slot->posB += frame.slot->velB;
        frame.slot->posC += frame.slot->velC;
        (frame.slot->frameCounter)++;
        if (frame.slot->frameCounter == 0x14) {
            frame.slot->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
