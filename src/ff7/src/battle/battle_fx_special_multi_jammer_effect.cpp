// src/ff7/src/battle/battle_fx_special_multi_jammer_effect.cpp
/* Enemy Special Attack [137]: Multi Sonic Jammer (orig. SPECIAL/JAMMER2) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Jammer2': loads its resources, dispatches a start command, and
//   runs a timeline with a variant sprite, a growing model and spinning fade sprite tasks.

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

struct GfxRotationAngles2;
struct BattleFxModelDraw;
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// GLOBAL: FF7 0x0089da10
unsigned int g_BattleSpecMultiJammerResourceDefs[95] = {
    0x001900b1,
    // STRING: FF7 0x0089dbe8
    (unsigned int)"HIKARI.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001940b1,
    // STRING: FF7 0x0089dbf4
    (unsigned int)"NIZI.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089dc00
    (unsigned int)"JAMMA00.TIM",
    // STRING: FF7 0x0089dc0c
    (unsigned int)"ONPA1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089dc18
    (unsigned int)"JAMMA00.TIM",
    // STRING: FF7 0x0089dc24
    (unsigned int)"ONPA2.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089dc30
    (unsigned int)"JAMMA00.TIM",
    // STRING: FF7 0x0089dc3c
    (unsigned int)"ONPA3.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089dc48
    (unsigned int)"JAMMA00.TIM",
    // STRING: FF7 0x0089dc54
    (unsigned int)"ONPA4.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089dc60
    (unsigned int)"JAMMA00.TIM",
    // STRING: FF7 0x0089dc6c
    (unsigned int)"ONPA5.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089dc78
    (unsigned int)"JAMMA00.TIM",
    // STRING: FF7 0x0089dc84
    (unsigned int)"ONPA6.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089dc90
    (unsigned int)"JAMMA00.TIM",
    // STRING: FF7 0x0089dc9c
    (unsigned int)"ONPA7.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089dca8
    (unsigned int)"JAMMA01.TIM",
    // STRING: FF7 0x0089dcb4
    (unsigned int)"HIKARI1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089dcc0
    (unsigned int)"JAMMA01.TIM",
    // STRING: FF7 0x0089dccc
    (unsigned int)"HIKARI2.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089dcd8
    (unsigned int)"JAMMA01.TIM",
    // STRING: FF7 0x0089dce4
    (unsigned int)"HIKARI3.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089dcf0
    (unsigned int)"JAMMA03.TIM",
    // STRING: FF7 0x0089dcfc
    (unsigned int)"BAKU1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089dd08
    (unsigned int)"JAMMA03.TIM",
    // STRING: FF7 0x0089dd14
    (unsigned int)"BAKU2.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089dd20
    (unsigned int)"JAMMA03.TIM",
    // STRING: FF7 0x0089dd2c
    (unsigned int)"BAKU3.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089dd38
    (unsigned int)"JAMMA03.TIM",
    // STRING: FF7 0x0089dd44
    (unsigned int)"BAKU4.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089dd50
    (unsigned int)"JAMMA03.TIM",
    // STRING: FF7 0x0089dd5c
    (unsigned int)"BAKU5.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089dd68
    (unsigned int)"JAMMA03.TIM",
    // STRING: FF7 0x0089dd74
    (unsigned int)"BAKU6.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089dd80
    (unsigned int)"JAMMA03.TIM",
    // STRING: FF7 0x0089dd8c
    (unsigned int)"BAKU7.SPT", 0x00000000, 0x00000000
};
// GLOBAL: FF7 0x0089db90
struct BattleLimitStardustRayModelReq2 g_BattleSpecMultiJammerSpriteReq1 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
// GLOBAL: FF7 0x0089dba0
struct BattleSpecHeatWaveModelReq2 g_BattleSpecMultiJammerSpriteReq2 = {0, {0xa8, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x0089dbb0
struct GfxRotationAngles2 g_BattleSpecMultiJammerOffset6 = {-1024, 0, 0};
// GLOBAL: FF7 0x0089dbb8
struct BattleSpecMultiJammerScale12 g_BattleSpecMultiJammerScale12 = {{0, 0x18, 0, 0}, 0x1800, {0, 0x18, 0, 0}};
// GLOBAL: FF7 0x0089dbc8
struct BattleSpecHeatWaveModelReq2 g_BattleSpecMultiJammerSpriteReq3 = {0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// 16 bytes, not 14: the trailing zero bytes lie between this object and the literal pool, and VC5
// pads neither $SG nor the pool start, so they can only be this object's tail.
// GLOBAL: FF7 0x0089dbd8
struct BattleSpecMultiJammerSpriteReq4 g_BattleSpecMultiJammerSpriteReq4 = {0, 0, -128, -128, -128, {0x2e, 0, 0, 0, 0}};
// clang-format on


// clang-format off
// GLOBAL: FF7 0x00b5dd18
int g_BattleSpecMultiJammerDrawHandle = 0;
// GLOBAL: FF7 0x00b5dd1c
int g_BattleSpecMultiJammerTargetMask = 0;
// GLOBAL: FF7 0x00b5dd20
struct GfxTableCursor *g_BattleSpecMultiJammerLoadedResTable = 0;
// GLOBAL: FF7 0x00b5dd24
short g_BattleSpecMultiJammerSpinStep = 0;
unsigned char g_bssPad_00b5dd28[24] = {0};
// clang-format on

/*
 * When the engine context exists and g_BattleSpecMultiJammerLoadedResTable is empty, builds SPECIAL/JAMMER2 with a default-
 * duration record and creates resource type 19 using g_BattleSpecMultiJammerResourceDefs.
 */
// FUNCTION: FF7 0x005212b0
void __cdecl BattleFx_SpecialLoadResMultiJammer(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest currentRecord;
    int kind;
    char outSize[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecMultiJammerLoadedResTable == 0) {
        kind = 0x13;
        resourceDefsWord = (int)&g_BattleSpecMultiJammerResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0089dd98
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/JAMMER2", outSize);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecMultiJammerLoadedResTable, curOwnerCtx);
    }
}

/*
 * Forwards both arguments to BattleFx_SpecialBeginMultiJammer.
 */
// FUNCTION: FF7 0x00521340
void __cdecl BattleFx_SpecialEnterMultiJammer(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginMultiJammer(targetMask, actorIdx);
}

/*
 * Stores the target mask, loads SPECIAL/JAMMER2, plays the start sound panned to the source
 * actor, then starts BattleFx_SpecialMultiJammerTimelineTask with the source actor's bone34 world
 * position in posA..posC and the target mask's centroid in velA..velC.
 */
// FUNCTION: FF7 0x00521355
void __cdecl BattleFx_SpecialBeginMultiJammer(int targetMask, int actorIdx)
{
    BattleFxMultiJammerTimelineSlot *slot;

    g_BattleSpecMultiJammerTargetMask = targetMask;
    BattleFx_SpecialLoadResMultiJammer();
    Battle_PlaySfxPannedToActor(0x11E, (short)actorIdx, 0);
    slot = (BattleFxMultiJammerTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialMultiJammerTimelineTask);
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].bone34, &slot->posA);
    Battle_GetActorMaskCentroid(targetMask, &slot->velA);
}

/*
 * Timeline of the Jammer2 effect: spawns spin-fade sprites for 20 frames, the spinning model at
 * frame 30, growing models and variant sprites on the targets from frame 54, and ends at frame 70.
 */
// FUNCTION: FF7 0x005213d1
void __cdecl BattleFx_SpecialMultiJammerTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[80];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; int depthZ; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offsetX; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxMultiJammerTimelineSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_38[0x38]; short repeatIndex; };
        struct { unsigned char pad_to_40[0x40]; short targetSlot; };
        struct { unsigned char pad_to_44[0x44]; short randAngle; };
        struct { unsigned char pad_to_48[0x48]; BattleFxMultiJammerTimelineSlot *slot; };
        struct { unsigned char pad_to_4C[0x4C]; short randAngleB; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxMultiJammerTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.slot->frameCounter < 20) {
        for (frame.targetSlot = 0; frame.targetSlot < 2; frame.targetSlot++) {
            frame.spawnSlot = (BattleFxMultiJammerTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialMultiJammerSpinFadeSpriteTask);
            frame.spawnSlot->posA = frame.slot->posA;
            frame.spawnSlot->posB = frame.slot->posB;
            frame.spawnSlot->posC = frame.slot->posC;
            frame.spawnSlot->velA = (frame.slot->velA - frame.slot->posA) / 15;
            frame.spawnSlot->velB = (frame.slot->velB - frame.slot->posB) / 15;
            frame.spawnSlot->velC = (frame.slot->velC - frame.slot->posC) / 15;
            frame.randAngle = (rand() % 4096);
            frame.offsetX.vx = CosFixed(frame.randAngle) * 0x64 >> 12;
            frame.offsetX.vy = SinFixed(frame.randAngle) * 0x64 >> 12;
            frame.offsetX.vz = 0;
            frame.rotX.vz = 0;
            frame.rotX.vx = frame.rotX.vz;
            frame.spawnSlot->angle = ArcTangentFixed(frame.slot->velA - frame.slot->posA, frame.slot->velC - frame.slot->posC);
            frame.rotX.vy = frame.spawnSlot->angle;
            RotMatrix(&frame.rotX, &frame.rotMatrix);
            frame.rotMatrix.t[0] = frame.spawnSlot->velA;
            frame.rotMatrix.t[1] = frame.spawnSlot->velB;
            frame.rotMatrix.t[2] = frame.spawnSlot->velC;
            SetRotMatrix(&frame.rotMatrix);
            SetTransMatrix(&frame.rotMatrix);
            RotTransPointToInt16(&frame.offsetX, &frame.spawnSlot->velA, &frame.depthZ);
            frame.spawnSlot->variant = rand() % 7;
        }
    }
    if (frame.slot->frameCounter == 30) {
        frame.spawnSlot = (BattleFxMultiJammerTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialMultiJammerSpinGrowingModelTask);
        Battle_StartFixedPointRamp(0x1000, 10);
        g_BattleFxTimelineState.red = 0;
        g_BattleFxTimelineState.channel1 = 0x20;
        g_BattleFxTimelineState.channel2 = 0;
    }
    if (frame.slot->frameCounter == 54) {
        for (frame.targetSlot = 0; frame.targetSlot < 0xA; frame.targetSlot++) {
            if ((g_BattleSpecMultiJammerTargetMask & 1 << frame.targetSlot) != 0) {
                frame.spawnSlot = (BattleFxMultiJammerTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialMultiJammerGrowingModelTask);
                Battle_GetBoneWorldPos(frame.targetSlot, g_BattleActorRecords[frame.targetSlot].targetBone, &frame.spawnSlot->posA);
            }
        }
        Battle_PlaySfxPannedToActor(0x189, -1, 0x40);
    }
    if (frame.slot->frameCounter > 56 && frame.slot->frameCounter < 64) {
        for (frame.targetSlot = 0; frame.targetSlot < 0xA; frame.targetSlot++) {
            if ((g_BattleSpecMultiJammerTargetMask & 1 << frame.targetSlot) != 0) {
                for (frame.repeatIndex = 0; frame.repeatIndex < 2; frame.repeatIndex++) {
                    frame.spawnSlot = (BattleFxMultiJammerTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialMultiJammerVariantSpriteTask);
                    Battle_GetBoneWorldPos(frame.targetSlot, g_BattleActorRecords[frame.targetSlot].targetBone, &frame.spawnSlot->posA);
                    frame.spawnSlot->posA += (short)(rand() % 1200 - 600);
                    frame.spawnSlot->posB += (short)(rand() % 1200 - 600);
                    frame.spawnSlot->posC += (short)(rand() % 1200 - 600);
                    frame.spawnSlot->variant = rand() % 10;
                    frame.spawnSlot->unusedScale = (rand() % 3 << 11) + 0x1000;
                    frame.randAngleB = (rand() % 4096);
                    frame.offsetX.vx = CosFixed(frame.randAngleB) * 0x3C >> 12;
                    frame.offsetX.vy = SinFixed(frame.randAngleB) * 0x3C >> 12;
                    frame.offsetX.vz = 0;
                    frame.rotX.vz = 0;
                    frame.rotX.vx = frame.rotX.vz;
                    frame.rotX.vy = (rand() % 4096);
                    RotMatrix(&frame.rotX, &frame.rotMatrix);
                    frame.rotMatrix.t[2] = 0;
                    frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
                    frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
                    SetRotMatrix(&frame.rotMatrix);
                    SetTransMatrix(&frame.rotMatrix);
                    RotTransPointToInt16(&frame.offsetX, &frame.spawnSlot->velA, &frame.depthZ);
                }
            }
        }
    }
    if (frame.slot->frameCounter == 60) {
        for (frame.targetSlot = 0; frame.targetSlot < 0xA; frame.targetSlot++) {
            if ((g_BattleSpecMultiJammerTargetMask & 1 << frame.targetSlot) != 0) {
                Battle_PostActorHitOrHurtMessage(frame.targetSlot);
            }
        }
    }
    (frame.slot->frameCounter)++;
    if (frame.slot->frameCounter == 70) {
        Battle_StartFixedPointRamp(0, 10);
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Per-frame draw of the Jammer2 variant sprite in the current task slot. The sprite is one of
// the ten table entries 9 to 0x12, chosen by the slot's variant. A billboard matrix is installed from
// the slot's position at scale 0x1800, and the frame index is published before the draw is
// submitted, which happens before the pause check. While g_BattleIsPaused is zero, the velocities
// are added to posA, posB and posC even when the sprite is null. Only with a nonnull sprite does
// frameIndex advance, and the slot is retired with ownerSlot 0xFFFF at or above the frame count in
// the sprite header.
// FUNCTION: FF7 0x00521967
void __cdecl BattleFx_SpecialMultiJammerVariantSpriteTask(void)
{
    struct {
        int table[10];
        BattleFxJammerVariantSprite *slot;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = (BattleFxJammerVariantSprite *)BATTLE_CURRENT_TASK_SLOT;
    frame.table[0] = 9;
    frame.table[1] = 0xA;
    frame.table[2] = 0xB;
    frame.table[3] = 0xC;
    frame.table[4] = 0xD;
    frame.table[5] = 0xE;
    frame.table[6] = 0xF;
    frame.table[7] = 0x10;
    frame.table[8] = 0x11;
    frame.table[9] = 0x12;
    BattleFx_InstallBillboardMatrix(&frame.slot->posA, 0x1800 /* 1.5 */, 0);
    g_BattleSpecMultiJammerSpriteReq1.frameIndex = frame.slot->frameIndex;
    (g_BattleSpecMultiJammerSpriteReq1.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(frame.table[frame.slot->variant], g_BattleSpecMultiJammerLoadedResTable);
    g_BattleSpecMultiJammerSpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(frame.table[frame.slot->variant], g_BattleSpecMultiJammerLoadedResTable);
    g_BattleSpecMultiJammerDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecMultiJammerSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecMultiJammerDrawHandle);
    if (g_BattleIsPaused == 0) {
        frame.slot->posA += frame.slot->velA;
        frame.slot->posB += frame.slot->velB;
        frame.slot->posC += frame.slot->velC;
        if ((g_BattleSpecMultiJammerSpriteReq1.spriteGroup) != 0) {
            (frame.slot->frameIndex)++;
            if (frame.slot->frameIndex >= g_BattleSpecMultiJammerSpriteReq1.spriteGroup->header->frameCount) {
                frame.slot->ownerSlot = 0xFFFF;
            }
        }
    }
}

/*
 * Draws one growing model at the target position for 15 frames, then frees its slot.
 */
// FUNCTION: FF7 0x00521ad2
void __cdecl BattleFx_SpecialMultiJammerGrowingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int depthZ;
        struct { unsigned char pad_to_4[0x4]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix rotAngles; };
        struct { unsigned char pad_to_48[0x48]; short axisIndex; };
        struct { unsigned char pad_to_50[0x50]; BattleTaskSlot *growingModel; };
    } frame;
#pragma pack(pop)

    frame.growingModel = BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecMultiJammerSpriteReq2.modelResource) = Gfx_ModelTableGetRenderObject(0, g_BattleSpecMultiJammerLoadedResTable);
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.rotAngles.m[2][frame.axisIndex] = 0;
        frame.rotAngles.m[1][frame.axisIndex] = 0;
        frame.rotAngles.m[0][frame.axisIndex] = 0;
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((SVECTOR *)frame.growingModel->data, frame.rotAngles.t, &frame.depthZ);
    frame.rotAngles.m[2][2] = frame.growingModel->frameCounter * 0x258 + 0x800;
    frame.rotAngles.m[1][1] = frame.rotAngles.m[2][2];
    frame.rotAngles.m[0][0] = frame.rotAngles.m[1][1];
    RotMatrix((SVECTOR *)&g_BattleSpecMultiJammerOffset6, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.rotAngles, &frame.rotMatrix);
    SetRotMatrix(&frame.rotAngles);
    SetTransMatrix(&frame.rotAngles);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecMultiJammerSpriteReq2.alpha = frame.growingModel->frameCounter << 8;
    g_BattleSpecMultiJammerDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecMultiJammerSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecMultiJammerDrawHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.growingModel->frameCounter)++;
    if (frame.growingModel->frameCounter == 15) {
        frame.growingModel->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the large spinning model for 30 frames, then frees its slot.
 */
// FUNCTION: FF7 0x00521c4c
void __cdecl BattleFx_SpecialMultiJammerSpinGrowingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[88];
        SVECTOR angleX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleTaskSlot *modelSlot; };
    } frame;
#pragma pack(pop)

    frame.modelSlot = BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecMultiJammerSpriteReq3.modelResource) = Gfx_ModelTableGetRenderObject(1, g_BattleSpecMultiJammerLoadedResTable);
    g_BattleSpecMultiJammerScale12.value4 = frame.modelSlot->frameCounter * 0x12C + 0x2000;
    frame.angleX.vx = 0x420;
    frame.angleX.vy = frame.modelSlot->frameCounter << 5;
    frame.angleX.vz = 0;
    RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, (int *)&g_BattleSpecMultiJammerScale12);
    frame.rotMatrix.t[0] = 0;
    frame.rotMatrix.t[1] = 0xFFFFFE70;
    frame.rotMatrix.t[2] = 0x1F40;
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecMultiJammerSpriteReq3.alpha = 0x1000 - (g_BattleSpecMultiJammerSpinStep << 9);
    g_BattleSpecMultiJammerDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecMultiJammerSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecMultiJammerDrawHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.modelSlot->frameCounter < 8) {
        g_BattleSpecMultiJammerSpinStep++;
    }
    if (frame.modelSlot->frameCounter > 22) {
        g_BattleSpecMultiJammerSpinStep--;
    }
    (frame.modelSlot->frameCounter)++;
    if (frame.modelSlot->frameCounter == 30) {
        frame.modelSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws one spin-fade sprite that drifts by its velocity and fades out over 20 frames.
 */
// FUNCTION: FF7 0x00521db2
void __cdecl BattleFx_SpecialMultiJammerSpinFadeSpriteTask(void)
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
    frame.tableFields[0] = 2;
    frame.tableFields[1] = 3;
    frame.tableFields[2] = 4;
    frame.tableFields[3] = 5;
    frame.tableFields[4] = 6;
    frame.tableFields[5] = 7;
    frame.tableFields[6] = 8;
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
    RotTransPoint((SVECTOR *)&frame.slot->posA, frame.scaleMatrix.t, &frame.pointDepth);
    frame.scaleMatrix.m[2][2] = 0x1400 /* 1.25 */;
    frame.scaleMatrix.m[1][1] = frame.scaleMatrix.m[2][2];
    frame.scaleMatrix.m[0][0] = frame.scaleMatrix.m[1][1];
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.scaleMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.scaleMatrix);
    SetTransMatrix(&frame.scaleMatrix);
    if (frame.slot->frameCounter < 12) {
        frame.fadeAmount = 8;
    } else {
        frame.fadeAmount = 20 - frame.slot->frameCounter;
    }
    frame.fadeLevel = *(short *)&frame.fadeAmount;
    (g_BattleSpecMultiJammerSpriteReq4.spriteResource) = Gfx_ModelTableGetLoadedRecord(frame.tableFields[frame.slot->tableIndex], g_BattleSpecMultiJammerLoadedResTable);
    g_BattleSpecMultiJammerSpriteReq4.res4 = Gfx_ModelTableGetSpriteObject(frame.tableFields[frame.slot->tableIndex], g_BattleSpecMultiJammerLoadedResTable);
    g_BattleSpecMultiJammerSpriteReq4.colorB = frame.fadeLevel << 4;
    g_BattleSpecMultiJammerSpriteReq4.colorG = g_BattleSpecMultiJammerSpriteReq4.colorB;
    g_BattleSpecMultiJammerSpriteReq4.colorR = g_BattleSpecMultiJammerSpriteReq4.colorG;
    g_BattleSpecMultiJammerDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecMultiJammerSpriteReq4, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecMultiJammerDrawHandle);
    if (g_BattleIsPaused == 0) {
        frame.slot->posA += frame.slot->velA;
        frame.slot->posB += frame.slot->velB;
        frame.slot->posC += frame.slot->velC;
        (frame.slot->frameCounter)++;
        if (frame.slot->frameCounter == 20) {
            frame.slot->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
