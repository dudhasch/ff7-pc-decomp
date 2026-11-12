// src/ff7/src/battle/battle_fx_special_sun_card.cpp
/* Enemy Special Effect [idx 54]: SunCard (JP: dia2 / ダイア2 (Diamond 2), asset: "SPECIAL/DIA2") — Sun (0x1f9, Death Dealer) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect DIA2: loads its resources, runs a timeline with a positional
//   sound and model callbacks, and runs the spinning, oscillating and accelerating model tasks plus
//   a target-hit model with sound.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/api/graphics.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle_special.h"
extern "C" {
#pragma function(memcpy)
// clang-format off
// GLOBAL: FF7 0x0083b6f8
int g_BattleSpecialSunCardResourceDefs = 0x001900B6;
int g_BattleSpecialSunCardResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleSpecialSunCardAxisAngleMatrix + 0x10)
};
int g_BattleSpecialSunCardResourceDefsDataTail[14] = {
    0x0, 0x0, 0x0, 0x1900b4, (int)((char *)&g_BattleSpecialSunCardAxisAngleMatrix + 0x1c), 0x0,
    0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleSpecialSunCardAxisAngleMatrix + 0x28), 0x0, 0x0,
    0x0, 0x0
};
// GLOBAL: FF7 0x0083b738
struct VECTOR g_BattleSpecialSunCardNegativeZUnitVector = {0, 0, -0x1000, 0};
// GLOBAL: FF7 0x0083b748
unsigned char g_BattleSpecialSunCardAxisAngleMatrix[52] = {0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x41, 0x52, 0x44, 0x31, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x00, 0x53, 0x41, 0x49, 0x4c, 0x45, 0x53, 0x53, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x53, 0x41, 0x49, 0x4c, 0x45, 0x53, 0x53, 0x2e, 0x52, 0x53, 0x44, 0x00};
// GLOBAL: FF7 0x00b491a8
int g_BattleSpecialSunCardDrawListHandle = 0;
unsigned char g_bssPad_00b491ac[4] = {0};
// GLOBAL: FF7 0x00b491b0
VECTOR g_BattleSpecialSunCardTransformedVector1 = {0};
// GLOBAL: FF7 0x00b491c0
struct Int64Pair g_BattleSpecialSunCardTargetPos = {0};
// GLOBAL: FF7 0x00b491c8
VECTOR g_BattleSpecialSunCardTransformedVector2 = {0};
// GLOBAL: FF7 0x00b491d8
int g_BattleSpecialSunCardTargetMask = 0;
// GLOBAL: FF7 0x00b491dc
int g_BattleSpecialSunCardActorIdx = 0;
// GLOBAL: FF7 0x00b491e0
struct GfxTableCursor *g_BattleSpecialSunCardLoadedResTable = 0;
unsigned char g_bssPad_00b491e4[4] = {0};
// clang-format on

#pragma data_seg(".data$u4c_orbit")
#pragma bss_seg(".data$u4c_orbit")
#pragma data_seg(".bss$u4c_orbit_slot")
#pragma bss_seg(".bss$u4c_orbit_slot")
#pragma data_seg(".bss$u4c_orbit_flag")
#pragma bss_seg(".bss$u4c_orbit_flag")
#pragma data_seg()
#pragma bss_seg()

#pragma intrinsic(memset)
#pragma function(memset)

// If the battle context exists and the DIA2 resource handle is zero, initializes a default-
// duration record, builds the SPECIAL/DIA2 resource path and requests resource type 3 using the
// context field at offset 0x4C. The record is represented by a scalar stack local with subsequent
// offset writes, so its apparent extent does not establish a safe source-level record allocation.
// FUNCTION: FF7 0x004ef6a0
void __cdecl BattleFx_SpecialLoadResSunCard(void)
{
    GfxModelTableBank *context;
    GfxResourceRequest requestRecord;
    int kind;
    char pathOut[200];
    GfxResourceRequest *pReq;
    int resultDefEntry;

    if (g_pBattle3dScene != 0 && g_BattleSpecialSunCardLoadedResTable == 0) {
        kind = 3;
        resultDefEntry = (int)&g_BattleSpecialSunCardResourceDefs;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestRecord;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x0083b77c
        pReq->resourcePath = BattleFx_BuildResourcePath("SPECIAL/DIA2", pathOut);
        Gfx_LoadModelTableIfAbsent(kind, resultDefEntry, pReq, &g_BattleSpecialSunCardLoadedResTable, context);
    }
}

/*
 * Forwarder: calls BattleFx_SpecialLoadResSunCard() with no arguments, then BattleFx_SpecialBeginSunCard(targetMask, actorIdx).
 */
// FUNCTION: FF7 0x004ef730
void __cdecl BattleFx_SpecialEnterSunCard(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResSunCard();
    BattleFx_SpecialBeginSunCard(targetMask, actorIdx);
}

/*
 * Allocates and saves a draw-list-selection slot, stores targetMask as the actor mask and records its
 * first selected low-ten-bit actor when one exists, retaining the previous actor value when none
 * exists. Stores actorIdx and allocates BattleFx_SpecialSunCardSpawnTimeline.
 */
// FUNCTION: FF7 0x004ef74a
void __cdecl BattleFx_SpecialBeginSunCard(int targetMask, int actorIdx)
{
    int bitIdx;
    int count;

    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    g_BattleSpecialSunCardTargetMask = targetMask;
    bitIdx = 0;
    for (count = 0; bitIdx < 0xA; bitIdx++) {
        if ((targetMask & 1 << bitIdx) != 0) {
            count++;
            if (count == 1) {
                g_BattleSpecialSunCardDrawListHandle = bitIdx;
            }
        }
    }
    g_BattleSpecialSunCardActorIdx = actorIdx;
    Battle_AllocateResourceSlot(BattleFx_SpecialSunCardSpawnTimeline);
}

/*
 * Retires the current slot, gets the selected actor's bone position into a local pair and copies
 * that pair into the payload of a new BattleFx_SpecialSunCardTimelineTask slot. Calls
 * BattleFx_SpecialSunCardBuildFacingVectors, then allocates BattleFx_SpecialSunCardDelayedImpactTask with
 * the same pair; no pause guard appears.
 */
// FUNCTION: FF7 0x004ef7db
void __cdecl BattleFx_SpecialSunCardSpawnTimeline(void)
{
    struct {
        BattleTaskSlot *slot;
        BattleBonePosCopy pos;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = BATTLE_CURRENT_TASK_SLOT;
    frame.slot->ownerSlot = BATTLE_TASK_FREE;
    Battle_GetBoneWorldPos(g_BattleSpecialSunCardDrawListHandle,
                           g_BattleActorRecords[g_BattleSpecialSunCardDrawListHandle].targetBone,
                           (short *)&frame.pos);
    frame.slot = BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSunCardTimelineTask);
    *(BattleBonePosCopy *)frame.slot->data = frame.pos;
    BattleFx_SpecialSunCardBuildFacingVectors();
    frame.slot = BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSunCardDelayedImpactTask);
    *(BattleBonePosCopy *)frame.slot->data = frame.pos;
}

/*
 * Runs an unpaused sequence that creates a positioned callback and positional sound at age 2, a
 * vertically moving callback and centered sound at age 24, then at age 72 samples the target
 * attachment and creates an axis-angle-oriented moving callback toward its X/Z position with
 * positional sound. Uses the normalized distance square root divided by 12.
 * Retires at age 90 before incrementing age.
 */
// FUNCTION: FF7 0x004ef87f
void __cdecl BattleFx_SpecialSunCardTimelineTask(void)
{
    struct {
        BattleFxSunCardSlot *slot;
        short matrix[16];
        BattleFxLimitOmnislashCylinderPair *current;
        int vector[3];
        unsigned char reserved0[8];
        int angle;
        int distance;
        unsigned char reserved1[8];
        int axis[3];
        unsigned char reserved2[4];
    } frame;

    frame.current = (BattleFxLimitOmnislashCylinderPair *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.current->step == 2) {
        frame.slot = (BattleFxSunCardSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSunCardSpinningModelTask);
        *(Int64Pair *)&frame.slot->posX = *(Int64Pair *)(&g_BattleSpecialSunCardTargetPos);
        frame.distance = g_BattleActorRecords[g_BattleSpecialSunCardActorIdx].posY - ((short *)&g_BattleSpecialSunCardTargetPos)[1];
        if (frame.distance < 0) {
            frame.distance *= -1;
        }
        frame.slot->distance = frame.distance;
        frame.slot->velocityY = 0xFED4;
        frame.slot->scale = 0x2000 /* 2.0 */;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleSpecialSunCardTargetPos), 0x1D5);
    }
    if (frame.current->step == 24) {
        frame.slot = (BattleFxSunCardSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSunCardOscillatingModelTask);
        frame.slot->posY = 0xFEF6;
        frame.slot->velocityY = 33;
        frame.slot->angle = 0;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, 0x40, 0x1D6);
    }
    if (frame.current->step == 72) {
        Battle_GetBoneWorldPos(g_BattleSpecialSunCardDrawListHandle, g_BattleActorRecords[g_BattleSpecialSunCardDrawListHandle].targetBone, &frame.current->posX);
        frame.vector[0] = frame.current->posX - g_BattleCameraPosition.cameraPosX;
        frame.vector[1] = 0;
        frame.vector[2] = frame.current->posZ - g_BattleCameraPosZ;
        frame.distance = NormalizeVectorFixed(&frame.vector[0], &frame.vector[0]);
        frame.distance = SqrtToInt(frame.distance) / 12;
        frame.angle = ComputeAngleBetweenVectors(&g_BattleSpecialSunCardNegativeZUnitVector, (VECTOR *)(&frame.vector[0]), (VECTOR *)(&frame.axis[0]));
        Gte_BuildAxisAngleMatrix(frame.angle, (PsxMatrix *)(&frame.matrix[0]), (VECTOR *)(&frame.axis[0]));
        TransformVectorToInt32((PsxMatrix *)(&frame.matrix[0]), &g_BattleSpecialSunCardNegativeZUnitVector, &frame.vector[0]);
        if (frame.axis[1] < 0) {
            frame.angle *= -1;
        }
        frame.slot = (BattleFxSunCardSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSunCardAcceleratingModelTask);
        frame.slot->posX = g_BattleCameraPosition.cameraPosX;
        frame.slot->posY = frame.current->posY;
        frame.slot->posZ = g_BattleCameraPosZ;
        frame.slot->offsetX = frame.distance * frame.vector[0] >> 12;
        frame.slot->velocityY = 0;
        frame.slot->offsetZ = frame.distance * frame.vector[2] >> 12;
        frame.slot->angle = frame.angle;
        frame.slot->scale = 0x2666;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&frame.current->posX), 0x1D7);
    }
    if (frame.current->step >= 90) {
        frame.current->ownerSlot = 0xFFFF;
    }
    (frame.current->step)++;
}

/*
 * Draws a uniformly scaled DIA2 model using the stored actor heading, an age-driven axis rotation
 * and a rotated vertical offset; the angle advances through age 8 then remains a quarter turn.
 * Ages at least 18 retire before checking pause. From unpaused age 8 onward, adds vertical
 * velocity to Y and increases that velocity by its sixth, then increments age.
 */
// FUNCTION: FF7 0x004efb50
void __cdecl BattleFx_SpecialSunCardSpinningModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        BattleFxSunCardSpinModel *slot;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offsetX; };
        struct { unsigned char pad_to_18[0x18]; int axisAngle; };
        struct { unsigned char pad_to_1C[0x1C]; PsxMatrix axisMatrix; };
        struct { unsigned char pad_to_3C[0x3C]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_5C[0x5C]; SVECTOR headingX; };
        struct { unsigned char pad_to_64[0x64]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_68[0x68]; VECTOR uniformScale; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxSunCardSpinModel *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.slot->frameCount <= 8) {
        frame.axisAngle = frame.slot->frameCount << 7;
    } else {
        frame.axisAngle = 0x400 /* 90 deg */;
    }
    frame.headingX.vx = 0;
    frame.headingX.vy = ((short *)&g_BattleSpecialSunCardTargetPos)[3];
    frame.headingX.vz = 0;
    RotMatrixXYZ(&frame.headingX, &frame.drawMatrix);
    Gte_BuildAxisAngleMatrix(-frame.axisAngle, &frame.axisMatrix, (VECTOR *)&g_BattleSpecialSunCardAxisAngleMatrix);
    ConcatMatrixInPlace(&frame.drawMatrix, &frame.axisMatrix);
    frame.offsetX.vx = 0;
    frame.offsetX.vy = frame.slot->offsetY;
    frame.offsetX.vz = 0;
    TransformVectorToInt16(&frame.axisMatrix, &frame.offsetX, &frame.offsetX.vx);
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0x800 /* 180 deg */;
    RotMatrixXYZ(&frame.rotX, &frame.drawMatrix);
    ConcatMatrixInPlace(&frame.axisMatrix, &frame.drawMatrix);
    frame.drawMatrix.t[0] = frame.slot->posX + frame.offsetX.vx;
    frame.drawMatrix.t[1] = frame.slot->posY + frame.offsetX.vy;
    frame.drawMatrix.t[2] = frame.slot->posZ + frame.offsetX.vz;
    frame.uniformScale.vz = frame.slot->modelId;
    frame.uniformScale.vy = frame.uniformScale.vz;
    frame.uniformScale.vx = frame.uniformScale.vy;
    ScaleMatrix(&frame.drawMatrix, &frame.uniformScale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.drawMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialSunCardLoadedResTable);
    frame.modelDraw->flags = 0;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 128;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.slot->frameCount >= 18) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.slot->frameCount >= 8) {
        frame.slot->posY += frame.slot->velocityY;
        frame.slot->velocityY += (short)(frame.slot->velocityY / 6);
    }
    (frame.slot->frameCount)++;
}

/*
 * Draws the DIA2 resource model at fixed X/Z and slot Y minus 40 with a quarter-turn X rotation
 * and configurable Y rotation. Retires at age 48 before checking pause. Unpaused ages below 8 move
 * Y with damped velocity clamped to at least 1; ages 8 through 39 set Y to a sine oscillation and
 * rotate during ages 22 through 33; later ages move Y with velocity increased by a quarter.
 */
// FUNCTION: FF7 0x004efda5
void __cdecl BattleFx_SpecialSunCardOscillatingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        BattleFxOscillatingModel *slot;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_18[0x18]; int sinAngle; };
        struct { unsigned char pad_to_1C[0x1C]; PsxMatrix spinMatrix; };
        struct { unsigned char pad_to_3C[0x3C]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_5C[0x5C]; SVECTOR spinX; };
        struct { unsigned char pad_to_64[0x64]; BattleFxModelDraw *modelDraw; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxOscillatingModel *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0x400 /* 90 deg */;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.drawMatrix);
    frame.drawMatrix.t[0] = 0;
    frame.drawMatrix.t[1] = frame.slot->offset - 0x28;
    frame.drawMatrix.t[2] = 0x380;
    frame.spinX.vx = 0;
    frame.spinX.vy = frame.slot->angle;
    frame.spinX.vz = 0;
    RotMatrixXYZ(&frame.spinX, &frame.spinMatrix);
    ConcatMatrixInPlace(&frame.spinMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialSunCardLoadedResTable);
    frame.modelDraw->flags = 0;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 128;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.slot->phase >= 48) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.slot->phase < 8) {
        frame.slot->offset += frame.slot->velocity;
        frame.slot->velocity -= (short)(frame.slot->velocity >> 5);
        if (frame.slot->velocity < 2) {
            frame.slot->velocity = 1;
        }
    } else if (frame.slot->phase < 40) {
        frame.sinAngle = frame.slot->phase - 8 << 7;
        frame.slot->offset = SinFixed(frame.sinAngle) * 0x18 >> 12;
        if (frame.slot->phase >= 22 && frame.slot->phase < 34) {
            frame.slot->angle -= 0xAA;
        }
    } else {
        frame.slot->offset += frame.slot->velocity;
        frame.slot->velocity += (short)(frame.slot->velocity >> 2);
    }
    (frame.slot->phase)++;
}

/*
 * Draws a uniformly scaled DIA2 model at the slot position with configured Y rotation and half-
 * turn Z rotation, enabling flag 8 and reducing intensity by 16 per age from age 12. Retires at
 * age 20 before checking pause. Unpaused age 10 triggers a target operation selected by screen pan
 * above 64 and positional sound; all unpaused calls advance X/Z and increase velocities by their
 * sixteenth, then advance age.
 */
// FUNCTION: FF7 0x004effe0
void __cdecl BattleFx_SpecialSunCardAcceleratingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        BattleFxSpecialAcceleratingSlot *slot;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_18[0x18]; int screenPan; };
        struct { unsigned char pad_to_3C[0x3C]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_64[0x64]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_68[0x68]; VECTOR uniformScale; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxSpecialAcceleratingSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.slot->angleY;
    frame.rotX.vz = 0x800 /* 180 deg */;
    RotMatrixXYZ(&frame.rotX, &frame.drawMatrix);
    frame.drawMatrix.t[0] = frame.slot->posA;
    frame.drawMatrix.t[1] = frame.slot->posY;
    frame.drawMatrix.t[2] = frame.slot->posB;
    frame.uniformScale.vz = frame.slot->scale;
    frame.uniformScale.vy = frame.uniformScale.vz;
    frame.uniformScale.vx = frame.uniformScale.vy;
    ScaleMatrix(&frame.drawMatrix, &frame.uniformScale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.drawMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialSunCardLoadedResTable);
    frame.modelDraw->flags = 0;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 128;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    if (frame.slot->timer >= 12) {
        frame.modelDraw->flags = 8;
        frame.modelDraw->fadeLevel = 128 - (frame.slot->timer - 12 << 4);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.slot->timer >= 20) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.slot->timer == 10) {
        frame.screenPan = Battle_ComputeScreenPanFromPosition(&frame.slot->posA);
        if (frame.screenPan > 64) {
            BattleFx_HitMarkSpawnAtActorBone(g_BattleSpecialSunCardDrawListHandle, 0x118);
        } else {
            BattleFx_HitMarkSpawnAtActorBone(g_BattleSpecialSunCardDrawListHandle, 0x18);
        }
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, frame.screenPan, 0x1D8);
    }
    frame.slot->posA += frame.slot->velocityA;
    frame.slot->posB += frame.slot->velocityB;
    frame.slot->velocityA += (short)(frame.slot->velocityA >> 4);
    frame.slot->velocityB += (short)(frame.slot->velocityB >> 4);
    (frame.slot->timer)++;
}

/*
 * Gets the g_BattleSpecialSunCardActorIdx actor's bone position into g_BattleSpecialSunCardTargetPos,
 * lowers Y by 224 and stores the actor's facing as its fourth short. Builds a Y rotation from that
 * facing and transforms the fixed vectors g_BattleSpecialSunCardNegativeZUnitVector and
 * g_BattleSpecialSunCardAxisAngleMatrix into g_BattleSpecialSunCardTransformedVector2 and
 * g_BattleSpecialSunCardTransformedVector1.
 */
// FUNCTION: FF7 0x004f022c
void __cdecl BattleFx_SpecialSunCardBuildFacingVectors(void)
{
    struct {
        int matrix[8];
        short rot[4];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    Battle_GetBoneWorldPos(g_BattleSpecialSunCardActorIdx,
                           g_BattleActorRecords[g_BattleSpecialSunCardActorIdx].altBone,
                           (short *)&g_BattleSpecialSunCardTargetPos);
    ((short *)&g_BattleSpecialSunCardTargetPos)[1] -= 0xe0;
    ((short *)&g_BattleSpecialSunCardTargetPos)[3] = g_BattleActorRecords[g_BattleSpecialSunCardActorIdx].yaw;
    frame.rot[0] = 0;
    frame.rot[1] = ((short *)&g_BattleSpecialSunCardTargetPos)[3];
    frame.rot[2] = 0;
    RotMatrixXYZ((void *)frame.rot, (void *)frame.matrix);
    TransformVectorToInt32((PsxMatrix *)frame.matrix, &g_BattleSpecialSunCardNegativeZUnitVector, &g_BattleSpecialSunCardTransformedVector2);
    TransformVectorToInt32((PsxMatrix *)frame.matrix, &g_BattleSpecialSunCardAxisAngleMatrix, &g_BattleSpecialSunCardTransformedVector1);
}

/*
 * One frame of a battle effect that only waits, the same body as
 * BattleFx_SpecialWorldCardDelayedImpactTask with a different payload: on exactly frame 0x5A it calls
 * BattleFx_SpecialSunCardSpawnTargetHitModelWithSound(g_BattleSpecialSunCardDrawListHandle,
 * g_BattleSpecialSunCardActorIdx) and frees the slot. Returns without advancing the counter while
 * g_BattleIsPaused is set.
 */
// FUNCTION: FF7 0x004f02d9
void __cdecl BattleFx_SpecialSunCardDelayedImpactTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 90) {
        BattleFx_SpecialSunCardSpawnTargetHitModelWithSound(g_BattleSpecialSunCardDrawListHandle, g_BattleSpecialSunCardActorIdx);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/*
 * Calls BattleFx_SpecialSunCardSpawnTargetHitModel with both arguments, then dispatches sound opcode 0x20 with pan computed from
 * g_BattleSpecialSunCardTargetMask and sound 0x54.
 */
// FUNCTION: FF7 0x004f033d
void __cdecl BattleFx_SpecialSunCardSpawnTargetHitModelWithSound(int actorIndex, int extraArg)
{
    BattleFx_SpecialSunCardSpawnTargetHitModel(actorIndex, extraArg);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(g_BattleSpecialSunCardTargetMask), 0x54);
}

// Allocates a slot running BattleFx_SpecialSunCardTargetHitModelTask for actorIndex. It stores the
// actor's target bone world position, zeroes the spin angle, and copies the actor's yaw. The
// radius is the actor radius plus 0xE0, capped at 0x76C; offsetStep is that radius divided by 16
// before the radius is capped again at 0x4F2. scaleStep is then (radius << 12) / 0xFA / 8, with
// the scale zero and targetIndex set to actorIndex.
// FUNCTION: FF7 0x004f036e
void __cdecl BattleFx_SpecialSunCardSpawnTargetHitModel(int actorIndex, int extraArg)
{
    int radius;
    BattleFxSunCardTargetHitRecord *hitRecord;

    hitRecord = (BattleFxSunCardTargetHitRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSunCardTargetHitModelTask);
    Battle_GetBoneWorldPos(actorIndex, g_BattleActorRecords[actorIndex].targetBone, &hitRecord->posX);
    hitRecord->spinAngle = 0;
    hitRecord->yaw = g_BattleActorRecords[actorIndex].yaw;
    radius = g_BattleActorRecords[actorIndex].radius + 224;
    if (radius > 1900) {
        radius = 1900;
    }
    hitRecord->radius = radius;
    hitRecord->offsetStep = radius / 16;
    if (radius > 1266) {
        radius = 1266;
    }
    hitRecord->scale = 0;
    hitRecord->scaleStep = (radius << 12) / 250 / 8;
    hitRecord->targetIndex = actorIndex;
}

/*
 * Draws DIA2 model 1 at a heading-rotated negative-Z offset with configured Z rotation and uniform
 * scale; from age 12 switches to model 2 with flag 8 and an intensity reduced by 8 per age.
 * Retires at age 28 before checking pause. Unpaused age 20 allocates a typed record for the stored
 * actor; ages below 8 rotate by 512 and grow scale, while ages at least 12 reduce the offset
 * distance, then advance age.
 */
// FUNCTION: FF7 0x004f0460
void __cdecl BattleFx_SpecialSunCardTargetHitModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[116];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxSilenceGrowingModel *dia2Record; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offsetX; };
        struct { unsigned char pad_to_18[0x18]; PsxMatrix headingMatrix; };
        struct { unsigned char pad_to_38[0x38]; PsxMatrix modelMatrix; };
        struct { unsigned char pad_to_58[0x58]; SVECTOR headingX; };
        struct { unsigned char pad_to_60[0x60]; BattleFxModelDraw *drawRecord; };
        struct { unsigned char pad_to_64[0x64]; VECTOR matrixScale; };
    } frame;
#pragma pack(pop)

    frame.dia2Record = (BattleFxSilenceGrowingModel *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.dia2Record->yaw;
    frame.rotX.vz = frame.dia2Record->spinAngle;
    RotMatrixXYZ(&frame.rotX, &frame.modelMatrix);
    frame.headingX.vx = 0;
    frame.headingX.vy = frame.dia2Record->yaw;
    frame.headingX.vz = 0;
    RotMatrixXYZ(&frame.headingX, &frame.headingMatrix);
    frame.offsetX.vx = 0;
    frame.offsetX.vy = 0;
    frame.offsetX.vz = -frame.dia2Record->extent;
    ApplyMatrixToSVectorInt32(&frame.headingMatrix, &frame.offsetX, &frame.modelMatrix.t[0]);
    frame.modelMatrix.t[0] += frame.dia2Record->posX;
    frame.modelMatrix.t[1] += frame.dia2Record->posY;
    frame.modelMatrix.t[2] += frame.dia2Record->posZ;
    frame.matrixScale.vz = frame.dia2Record->scale;
    frame.matrixScale.vy = frame.matrixScale.vz;
    frame.matrixScale.vx = frame.matrixScale.vy;
    ScaleMatrix(&frame.modelMatrix, &frame.matrixScale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    frame.drawRecord = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.drawRecord->modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleSpecialSunCardLoadedResTable);
    frame.drawRecord->flags = 0;
    frame.drawRecord->uvScroll = 0;
    frame.drawRecord->fadeLevel = 128;
    frame.drawRecord->stateFlags = 0x20;
    frame.drawRecord->texturePageBits = 0;
    if (frame.dia2Record->timer >= 12) {
        frame.drawRecord->flags = 8;
        frame.drawRecord->fadeLevel -= (short)(frame.dia2Record->timer * 8 + -96);
        frame.drawRecord->modelGroup = Gfx_ModelTableGetRenderObject(2, g_BattleSpecialSunCardLoadedResTable);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.drawRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.dia2Record->timer >= 28) {
        frame.dia2Record->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.dia2Record->timer == 20) {
        Battle_PostActorHitOrHurtMessage(frame.dia2Record->targetIndex);
    }
    if (frame.dia2Record->timer < 8) {
        frame.dia2Record->spinAngle += 0x200 /* 45 deg */;
        frame.dia2Record->scale += frame.dia2Record->scaleStep;
    } else if (frame.dia2Record->timer >= 12) {
        frame.dia2Record->extent -= frame.dia2Record->extentStep;
    }
    (frame.dia2Record->timer)++;
}
} /* extern "C" */
