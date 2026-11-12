// src/ff7/src/battle/battle_fx_special_world_card.cpp
/* Enemy Special Effect [idx 55]: WorldCard (JP: joker2 / ジョーカー2 (Joker 2), asset: "special/joker2") — World (0x1fa, Death Dealer) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect Joker2: loads its resources, runs a timeline with a positional
//   sound and model callbacks, and runs the spinning, oscillating, accelerating, delayed-impact and
//   layered model tasks that draw it.

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
// GLOBAL: FF7 0x0083b5a0
int g_BattleSpecialWorldCardResourceDefs = 0x001900B6;
int g_BattleSpecialWorldCardResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialWorldCardResourceNames + 0x0)
};
int g_BattleSpecialWorldCardResourceDefsDataTail[24] = {
    0x0, 0x0, 0x0, 0x6440b4, (int)((char *)&g_BattleFxSpecialWorldCardResourceNames + 0xc), 0x0,
    0x0, 0x0, 0x6440b1, (int)((char *)&g_BattleFxSpecialWorldCardResourceNames + 0x18), 0x0, 0x0,
    0x0, 0x6440b4, (int)((char *)&g_BattleFxSpecialWorldCardResourceNames + 0x24), 0x0, 0x0, 0x0,
    0x6440b1, (int)((char *)&g_BattleFxSpecialWorldCardResourceNames + 0x30), 0x0, 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x0083b608
struct VECTOR g_BattleWorldCardNegativeZUnitVector = {0, 0, -0x1000, 0};
// GLOBAL: FF7 0x0083b618
struct VECTOR g_BattleSpecialWorldCardAxisAngleMatrix = {0x1000, 0, 0, 0};
// GLOBAL: FF7 0x0083b628
short g_BattleSpecialWorldCardAngleOffsets[4] = {1792, 1536, 1280, 1024};
// GLOBAL: FF7 0x0083b630
short g_BattleSpecialWorldCardFadeLevels[4] = {2048, 3072, 3328, 3584};
unsigned char g_BattleFxSpecialWorldCardResourceNames[60] = {0x43, 0x41, 0x52, 0x44, 0x31, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x00, 0x48, 0x41, 0x52, 0x49, 0x5f, 0x31, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x48, 0x41, 0x52, 0x49, 0x5f, 0x31, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x53, 0x54, 0x4f, 0x50, 0x5f, 0x31, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x53, 0x54, 0x4f, 0x50, 0x5f, 0x32, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00};
// GLOBAL: FF7 0x00b49128
int g_BattleSpecialWorldCardTargetActorIdx = 0;
unsigned char g_bssPad_00b4912c[4] = {0};
// GLOBAL: FF7 0x00b49130
VECTOR g_BattleSpecialWorldCardTransformedVector1 = {0};
// GLOBAL: FF7 0x00b49140
struct Int64Pair g_BattleSpecialWorldCardTargetPos = {0};
// GLOBAL: FF7 0x00b49148
VECTOR g_BattleSpecialWorldCardTransformedVector2 = {0};
// GLOBAL: FF7 0x00b49158
int g_BattleSpecialWorldCardTargetMask = 0;
// GLOBAL: FF7 0x00b4915c
int g_BattleSpecialWorldCardActorIdx = 0;
// GLOBAL: FF7 0x00b49160
struct GfxTableCursor *g_BattleSpecialWorldCardLoadedResTable = 0;
unsigned char g_bssPad_00b49164[4] = {0};
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

// If the battle context exists and the joker2 resource handle is zero, initializes a default-
// duration record, builds the special/joker2 resource path and requests resource type 5 using the
// context field at offset 0x4C. The record is represented by a scalar stack local with subsequent
// offset writes, so its apparent extent does not establish a safe source-level record allocation.
// FUNCTION: FF7 0x004ece70
void __cdecl BattleFx_SpecialLoadResWorldCard(void)
{
    GfxModelTableBank *context;
    GfxResourceRequest requestRecord;
    int kind;
    char pathOut[200];
    GfxResourceRequest *pReq;
    int resultDefEntry;

    if (g_pBattle3dScene != 0 && g_BattleSpecialWorldCardLoadedResTable == 0) {
        kind = 5;
        resultDefEntry = (int)&g_BattleSpecialWorldCardResourceDefs;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestRecord;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x0083b674
        pReq->resourcePath = BattleFx_BuildResourcePath("special/joker2", pathOut);
        Gfx_LoadModelTableIfAbsent(kind, resultDefEntry, pReq, &g_BattleSpecialWorldCardLoadedResTable, context);
    }
}

/*
 * Forwarder: calls BattleFx_SpecialLoadResWorldCard() with no arguments, then BattleFx_SpecialBeginWorldCard(targetMask, actorIdx).
 */
// FUNCTION: FF7 0x004ecf00
void __cdecl BattleFx_SpecialEnterWorldCard(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResWorldCard();
    BattleFx_SpecialBeginWorldCard(targetMask, actorIdx);
}

/*
 * Allocates and saves a draw-list-selection slot, stores targetMask as the actor mask, and records its
 * first selected low-ten-bit actor when one exists, leaving the previous actor value when none
 * exists. Stores actorIdx and allocates BattleFx_SpecialWorldCardSpawnTimeline.
 */
// FUNCTION: FF7 0x004ecf1a
void __cdecl BattleFx_SpecialBeginWorldCard(int targetMask, int actorIdx)
{
    int bitIdx;
    int count;

    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    g_BattleSpecialWorldCardTargetMask = targetMask;
    bitIdx = 0;
    for (count = 0; bitIdx < 0xA; bitIdx++) {
        if ((targetMask & 1 << bitIdx) != 0) {
            count++;
            if (count == 1) {
                g_BattleSpecialWorldCardTargetActorIdx = bitIdx;
            }
        }
    }
    g_BattleSpecialWorldCardActorIdx = actorIdx;
    Battle_AllocateResourceSlot(BattleFx_SpecialWorldCardSpawnTimeline);
}

/*
 * Retires the current slot, obtains the selected actor bone position into a local eight-byte pair
 * and copies that pair to +4 of a new BattleFx_SpecialWorldCardTimelineTask slot. Calls BattleFx_SpecialWorldCardCaptureTargetAnchor, then allocates
 * BattleFx_SpecialWorldCardDelayedImpactTask with the same eight-byte pair; no pause guard appears.
 */
// FUNCTION: FF7 0x004ecfab
void __cdecl BattleFx_SpecialWorldCardSpawnTimeline(void)
{
    struct {
        int *slot;
        BattleBonePosCopy pos;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = (int *)BATTLE_CURRENT_TASK_SLOT;
    *(short *)frame.slot = 0xffff;
    Battle_GetBoneWorldPos(g_BattleSpecialWorldCardTargetActorIdx,
                           g_BattleActorRecords[g_BattleSpecialWorldCardTargetActorIdx].targetBone,
                           (short *)&frame.pos);
    frame.slot = (int *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWorldCardTimelineTask);
    *(BattleBonePosCopy *)(frame.slot + 1) = frame.pos;
    BattleFx_SpecialWorldCardCaptureTargetAnchor();
    frame.slot = (int *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWorldCardDelayedImpactTask);
    *(BattleBonePosCopy *)(frame.slot + 1) = frame.pos;
}

/*
 * Runs an unpaused sequence that creates a positioned callback and positional sound at age 2, a
 * vertically moving callback and centered sound at age 24, then at age 72 samples the target
 * attachment and creates an axis-angle-oriented moving callback toward its X/Z position with
 * positional sound. Uses the normalized distance square root divided by 12.
 * Retires at age 90 before incrementing age.
 */
// FUNCTION: FF7 0x004ed04f
void __cdecl BattleFx_SpecialWorldCardTimelineTask(void)
{
    struct {
        BattleFxDiamondCardSlot *slot;
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
        frame.slot = (BattleFxDiamondCardSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWorldCardSpinningModelTask);
        *(Int64Pair *)((char *)frame.slot + 4) = *(Int64Pair *)&g_BattleSpecialWorldCardTargetPos;
        frame.distance = g_BattleActorRecords[g_BattleSpecialWorldCardActorIdx].posY - ((short *)&g_BattleSpecialWorldCardTargetPos)[1];
        if (frame.distance < 0) {
            frame.distance *= -1;
        }
        frame.slot->distance = frame.distance;
        frame.slot->velocityY = 0xFED4;
        frame.slot->scale = 0x2000 /* 2.0 */;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(((short *)&g_BattleSpecialWorldCardTargetPos)), 0x1D5);
    }
    if (frame.current->step == 24) {
        frame.slot = (BattleFxDiamondCardSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWorldCardOscillatingModelTask);
        frame.slot->posY = 0xFEF6;
        frame.slot->velocityY = 33;
        frame.slot->angle = 0;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, 0x40, 0x1D6);
    }
    if (frame.current->step == 72) {
        Battle_GetBoneWorldPos(g_BattleSpecialWorldCardTargetActorIdx, g_BattleActorRecords[g_BattleSpecialWorldCardTargetActorIdx].targetBone, &frame.current->posX);
        frame.vector[0] = frame.current->posX - g_BattleCameraPosition.cameraPosX;
        frame.vector[1] = 0;
        frame.vector[2] = frame.current->posZ - g_BattleCameraPosZ;
        frame.distance = NormalizeVectorFixed(&frame.vector[0], &frame.vector[0]);
        frame.distance = SqrtToInt(frame.distance) / 12;
        frame.angle = ComputeAngleBetweenVectors(&g_BattleWorldCardNegativeZUnitVector, (VECTOR *)(&frame.vector[0]), (VECTOR *)(&frame.axis[0]));
        Gte_BuildAxisAngleMatrix(frame.angle, (PsxMatrix *)(&frame.matrix[0]), (VECTOR *)(&frame.axis[0]));
        TransformVectorToInt32((PsxMatrix *)(&frame.matrix[0]), &g_BattleWorldCardNegativeZUnitVector, &frame.vector[0]);
        if (frame.axis[1] < 0) {
            frame.angle *= -1;
        }
        frame.slot = (BattleFxDiamondCardSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWorldCardAcceleratingModelTask);
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
 * Draws a uniformly scaled model using actor heading, an age-driven axis rotation and a rotated
 * vertical offset; the angle advances through age 8 then remains a quarter turn. Ages at least 18
 * retire before checking pause. From unpaused age 8 onward, adds vertical velocity to Y and
 * increases that velocity by its sixth, then increments age.
 */
// FUNCTION: FF7 0x004ed320
void __cdecl BattleFx_SpecialWorldCardSpinningModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        BattleFxSunCardSpinModel *spinSlot;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offsetX; };
        struct { unsigned char pad_to_18[0x18]; int spinAngle; };
        struct { unsigned char pad_to_1C[0x1C]; PsxMatrix axisMatrix; };
        struct { unsigned char pad_to_3C[0x3C]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_5C[0x5C]; SVECTOR headingX; };
        struct { unsigned char pad_to_64[0x64]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_68[0x68]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.spinSlot = (BattleFxSunCardSpinModel *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.spinSlot->frameCount <= 8) {
        frame.spinAngle = frame.spinSlot->frameCount << 7;
    } else {
        frame.spinAngle = 0x400 /* 90 deg */;
    }
    frame.headingX.vx = 0;
    frame.headingX.vy = ((short *)&g_BattleSpecialWorldCardTargetPos)[3];
    frame.headingX.vz = 0;
    RotMatrixXYZ(&frame.headingX, &frame.drawMatrix);
    Gte_BuildAxisAngleMatrix(-frame.spinAngle, &frame.axisMatrix, &g_BattleSpecialWorldCardAxisAngleMatrix);
    ConcatMatrixInPlace(&frame.drawMatrix, &frame.axisMatrix);
    frame.offsetX.vx = 0;
    frame.offsetX.vy = frame.spinSlot->offsetY;
    frame.offsetX.vz = 0;
    TransformVectorToInt16(&frame.axisMatrix, &frame.offsetX, &frame.offsetX.vx);
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0x800 /* 180 deg */;
    RotMatrixXYZ(&frame.rotX, &frame.drawMatrix);
    ConcatMatrixInPlace(&frame.axisMatrix, &frame.drawMatrix);
    frame.drawMatrix.t[0] = frame.spinSlot->posX + frame.offsetX.vx;
    frame.drawMatrix.t[1] = frame.spinSlot->posY + frame.offsetX.vy;
    frame.drawMatrix.t[2] = frame.spinSlot->posZ + frame.offsetX.vz;
    frame.scale.vz = frame.spinSlot->modelId;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.drawMatrix, &frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.drawMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialWorldCardLoadedResTable);
    frame.modelDraw->flags = 0;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 128;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.spinSlot->frameCount >= 18) {
        frame.spinSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.spinSlot->frameCount >= 8) {
        frame.spinSlot->posY += frame.spinSlot->velocityY;
        frame.spinSlot->velocityY += (short)(frame.spinSlot->velocityY / 6);
    }
    (frame.spinSlot->frameCount)++;
}

/*
 * Draws the resource model at fixed X/Z and slot Y minus 40 with a quarter-turn X rotation and
 * configurable Y rotation. Retires at age 48 before checking pause. Unpaused ages below 8 move Y
 * with damped velocity clamped to at least 1; ages 8 through 39 set Y to a sine oscillation and
 * rotate during ages 22 through 33; later ages move Y with velocity increased by a quarter.
 */
// FUNCTION: FF7 0x004ed575
void __cdecl BattleFx_SpecialWorldCardOscillatingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        BattleFxOscillatingModel *modelSlot;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_18[0x18]; int sinAngle; };
        struct { unsigned char pad_to_1C[0x1C]; PsxMatrix yRotMatrix; };
        struct { unsigned char pad_to_3C[0x3C]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_5C[0x5C]; SVECTOR yRotX; };
        struct { unsigned char pad_to_64[0x64]; BattleFxModelDraw *modelDraw; };
    } frame;
#pragma pack(pop)

    frame.modelSlot = (BattleFxOscillatingModel *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0x400 /* 90 deg */;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.drawMatrix);
    frame.drawMatrix.t[0] = 0;
    frame.drawMatrix.t[1] = frame.modelSlot->offset - 0x28;
    frame.drawMatrix.t[2] = 0x380;
    frame.yRotX.vx = 0;
    frame.yRotX.vy = frame.modelSlot->angle;
    frame.yRotX.vz = 0;
    RotMatrixXYZ(&frame.yRotX, &frame.yRotMatrix);
    ConcatMatrixInPlace(&frame.yRotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialWorldCardLoadedResTable);
    frame.modelDraw->flags = 0;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 128;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.modelSlot->phase >= 48) {
        frame.modelSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.modelSlot->phase < 8) {
        frame.modelSlot->offset += frame.modelSlot->velocity;
        frame.modelSlot->velocity -= (short)(frame.modelSlot->velocity >> 5);
        if (frame.modelSlot->velocity < 2) {
            frame.modelSlot->velocity = 1;
        }
    } else if (frame.modelSlot->phase < 40) {
        frame.sinAngle = frame.modelSlot->phase - 8 << 7;
        frame.modelSlot->offset = SinFixed(frame.sinAngle) * 0x18 >> 12;
        if (frame.modelSlot->phase >= 22 && frame.modelSlot->phase < 34) {
            frame.modelSlot->angle -= 0xAA;
        }
    } else {
        frame.modelSlot->offset += frame.modelSlot->velocity;
        frame.modelSlot->velocity += (short)(frame.modelSlot->velocity >> 2);
    }
    (frame.modelSlot->phase)++;
}

/*
 * Draws a uniformly scaled model at the slot position with configured Y rotation and half-turn Z
 * rotation, enabling flag 8 and reducing intensity by 16 per age from age 12. Retires at age 20
 * before checking pause. Unpaused age 10 triggers a target operation selected by screen pan above
 * 64 and positional sound; all unpaused calls advance X/Z and increase velocities by their
 * sixteenth, then advance age.
 */
// FUNCTION: FF7 0x004ed7b0
void __cdecl BattleFx_SpecialWorldCardAcceleratingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        BattleFxSpecialAcceleratingSlot *accelSlot;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_18[0x18]; int screenPan; };
        struct { unsigned char pad_to_3C[0x3C]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_64[0x64]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_68[0x68]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.accelSlot = (BattleFxSpecialAcceleratingSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.accelSlot->angleY;
    frame.rotX.vz = 0x800 /* 180 deg */;
    RotMatrixXYZ(&frame.rotX, &frame.drawMatrix);
    frame.drawMatrix.t[0] = frame.accelSlot->posA;
    frame.drawMatrix.t[1] = frame.accelSlot->posY;
    frame.drawMatrix.t[2] = frame.accelSlot->posB;
    frame.scale.vz = frame.accelSlot->scale;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.drawMatrix, &frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.drawMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialWorldCardLoadedResTable);
    frame.modelDraw->flags = 0;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 128;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    if (frame.accelSlot->timer >= 12) {
        frame.modelDraw->flags = 8;
        frame.modelDraw->fadeLevel = 128 - (frame.accelSlot->timer - 12 << 4);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.accelSlot->timer >= 20) {
        frame.accelSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.accelSlot->timer == 10) {
        frame.screenPan = Battle_ComputeScreenPanFromPosition(&frame.accelSlot->posA);
        if (frame.screenPan > 64) {
            BattleFx_HitMarkSpawnAtActorBone(g_BattleSpecialWorldCardTargetActorIdx, 0x118);
        } else {
            BattleFx_HitMarkSpawnAtActorBone(g_BattleSpecialWorldCardTargetActorIdx, 0x18);
        }
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, frame.screenPan, 0x1D8);
    }
    frame.accelSlot->posA += frame.accelSlot->velocityA;
    frame.accelSlot->posB += frame.accelSlot->velocityB;
    frame.accelSlot->velocityA += (short)(frame.accelSlot->velocityA >> 4);
    frame.accelSlot->velocityB += (short)(frame.accelSlot->velocityB >> 4);
    (frame.accelSlot->timer)++;
}

/*
 * Gets the alternate bone position of the g_BattleSpecialWorldCardActorIdx actor into
 * g_BattleSpecialWorldCardTargetPos, lowers Y by 224 and stores the actor's yaw in its fourth short.
 * Builds a Y rotation from that yaw and transforms g_BattleWorldCardNegativeZUnitVector and
 * g_BattleSpecialWorldCardAxisAngleMatrix into g_BattleSpecialWorldCardTransformedVector2 and
 * g_BattleSpecialWorldCardTransformedVector1.
 */
// FUNCTION: FF7 0x004ed9fc
void __cdecl BattleFx_SpecialWorldCardCaptureTargetAnchor(void)
{
    struct {
        int matrix[8];
        short rot[4];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    Battle_GetBoneWorldPos(g_BattleSpecialWorldCardActorIdx,
                           g_BattleActorRecords[g_BattleSpecialWorldCardActorIdx].altBone,
                           ((short *)&g_BattleSpecialWorldCardTargetPos));
    ((short *)&g_BattleSpecialWorldCardTargetPos)[1] -= 0xe0;
    ((short *)&g_BattleSpecialWorldCardTargetPos)[3] = g_BattleActorRecords[g_BattleSpecialWorldCardActorIdx].yaw;
    frame.rot[0] = 0;
    frame.rot[1] = ((short *)&g_BattleSpecialWorldCardTargetPos)[3];
    frame.rot[2] = 0;
    RotMatrixXYZ((void *)frame.rot, (void *)frame.matrix);
    TransformVectorToInt32((PsxMatrix *)frame.matrix, &g_BattleWorldCardNegativeZUnitVector, &g_BattleSpecialWorldCardTransformedVector2);
    TransformVectorToInt32((PsxMatrix *)frame.matrix, &g_BattleSpecialWorldCardAxisAngleMatrix, &g_BattleSpecialWorldCardTransformedVector1);
}

/*
 * Delayed impact task: returns while g_BattleIsPaused is set; otherwise on frame 0x5A of its
 * frame counter it calls BattleFx_SpecialWorldCardSpawnModelsWithSound and frees its slot. The counter
 * advances on every unpaused frame.
 */
// FUNCTION: FF7 0x004edaa9
void __cdecl BattleFx_SpecialWorldCardDelayedImpactTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 90) {
        BattleFx_SpecialWorldCardSpawnModelsWithSound(g_BattleSpecialWorldCardTargetActorIdx, g_BattleSpecialWorldCardActorIdx);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/*
 * Calls BattleFx_SpecialWorldCardSpawnModels with both arguments, then dispatches sound opcode 0x20 with pan computed from
 * g_BattleSpecialWorldCardTargetMask and sound 0x57.
 */
// FUNCTION: FF7 0x004edb0d
void __cdecl BattleFx_SpecialWorldCardSpawnModelsWithSound(int actorIndex, int extraArg)
{
    BattleFx_SpecialWorldCardSpawnModels(actorIndex, extraArg);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(g_BattleSpecialWorldCardTargetMask), 0x57);
}

/*
 * Creates five resource callbacks at an actor attachment and heading: a primary callback retains
 * the actor index and capped size, while two pairs receive size offsets of opposite signs and
 * distinct rotation/scale parameters. Size is first capped at 1900 and then at 1266 for derived
 * values, with short truncation between arithmetic stages.
 */
// FUNCTION: FF7 0x004edb3e
void __cdecl BattleFx_SpecialWorldCardSpawnModels(int actorIndex, int extraArg)
{
    int size;
    int scaledBytes;
    BattleFxWorldCardEntry *entry;
    BattleFxWorldCardSpawnRecord *primaryRecord;

    primaryRecord = (BattleFxWorldCardSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWorldCardPrimaryModelTask);
    Battle_GetBoneWorldPos(actorIndex, g_BattleActorRecords[actorIndex].targetBone, &primaryRecord->posX);
    primaryRecord->angle = 0;
    primaryRecord->yaw = g_BattleActorRecords[actorIndex].yaw;
    size = g_BattleActorRecords[actorIndex].radius + 256;
    if (size > 1900) {
        size = 1900;
    }
    primaryRecord->base = size;
    if (size > 1266) {
        size = 1266;
    }
    scaledBytes = size << 12;
    primaryRecord->ramp = 0;
    primaryRecord->spawnCount = scaledBytes / 1000;
    primaryRecord->batchCount = primaryRecord->spawnCount;
    primaryRecord->batchCount = primaryRecord->batchCount / 4;
    primaryRecord->spawnCount = primaryRecord->spawnCount / 10;
    primaryRecord->targetIndex = actorIndex;
    entry = (BattleFxWorldCardEntry *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWorldCardSecondaryModelTask);
    *(Int64Pair *)&entry->posX = *(Int64Pair *)&primaryRecord->posX;
    entry->roll = 0;
    entry->yaw = primaryRecord->yaw;
    entry->offset = size * 200 / 1000;
    entry->position = primaryRecord->base + entry->offset;
    entry->offset = entry->offset / 10;
    entry->spawnTotal = 0;
    entry->spawnCount = scaledBytes / 1120;
    entry->batchCount = entry->spawnCount;
    entry->batchCount = entry->batchCount / 8;
    entry->spawnCount = entry->spawnCount / 10;
    entry = (BattleFxWorldCardEntry *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWorldCardLayeredModelsTask);
    *(Int64Pair *)&entry->posX = *(Int64Pair *)&primaryRecord->posX;
    entry->roll = 0x800 /* 180 deg */;
    entry->yaw = primaryRecord->yaw;
    entry->offset = size * 264 / 1000;
    entry->position = primaryRecord->base + entry->offset;
    entry->offset = entry->offset / 10;
    entry->spawnCount = scaledBytes / 1000;
    entry->spawnTotal = entry->spawnCount;
    entry->batchCount = entry->spawnTotal / 8;
    entry->spawnTotal <<= 1;
    entry->clampLimit = entry->spawnTotal;
    entry->spawnCount = entry->spawnCount / 10;
    entry = (BattleFxWorldCardEntry *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWorldCardSecondaryModelTask);
    *(Int64Pair *)&entry->posX = *(Int64Pair *)&primaryRecord->posX;
    entry->roll = 0;
    entry->yaw = primaryRecord->yaw;
    entry->offset = size * 200 / 1000;
    entry->position = primaryRecord->base - entry->offset;
    entry->offset = entry->offset / 10;
    entry->spawnTotal = 0;
    entry->spawnCount = scaledBytes / 1120;
    entry->batchCount = entry->spawnCount;
    entry->batchCount = entry->batchCount / 8;
    entry->spawnCount = entry->spawnCount / 10;
    entry = (BattleFxWorldCardEntry *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWorldCardLayeredModelsTask);
    *(Int64Pair *)&entry->posX = *(Int64Pair *)&primaryRecord->posX;
    entry->roll = 0x800 /* 180 deg */;
    entry->yaw = primaryRecord->yaw;
    entry->offset = size * 264 / 1000;
    entry->position = primaryRecord->base - entry->offset;
    entry->offset = entry->offset / 10;
    entry->spawnCount = scaledBytes / 1000;
    entry->spawnTotal = entry->spawnCount;
    entry->batchCount = entry->spawnTotal / 8;
    entry->spawnTotal <<= 1;
    entry->clampLimit = entry->spawnTotal;
    entry->spawnCount = entry->spawnCount / 10;
}

/*
 * Draws resource model 3 with flag 0x80 at a heading-rotated negative-Z offset from the slot
 * position and uniform scale. Retires ages at least 35 before checking pause. Unpaused age 25
 * allocates a typed record for the stored actor index; ages below 4 grow scale, and ages at least
 * 25 rotate by 128 and shrink scale before advancing age.
 */
// FUNCTION: FF7 0x004edffe
void __cdecl BattleFx_SpecialWorldCardPrimaryModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[116];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxStopCentralRecord *record; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offsetX; };
        struct { unsigned char pad_to_18[0x18]; PsxMatrix headingMatrix; };
        struct { unsigned char pad_to_38[0x38]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_58[0x58]; SVECTOR headingX; };
        struct { unsigned char pad_to_60[0x60]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_64[0x64]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.record = (BattleFxStopCentralRecord *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.record->yaw;
    frame.rotX.vz = frame.record->angle;
    RotMatrixXYZ(&frame.rotX, &frame.drawMatrix);
    frame.headingX.vx = 0;
    frame.headingX.vy = frame.record->yaw;
    frame.headingX.vz = 0;
    RotMatrixXYZ(&frame.headingX, &frame.headingMatrix);
    frame.offsetX.vx = 0;
    frame.offsetX.vy = 0;
    frame.offsetX.vz = -frame.record->offsetDistance;
    ApplyMatrixToSVectorInt32(&frame.headingMatrix, &frame.offsetX, &frame.drawMatrix.t[0]);
    frame.drawMatrix.t[0] += frame.record->posX;
    frame.drawMatrix.t[1] += frame.record->posY;
    frame.drawMatrix.t[2] += frame.record->posZ;
    frame.scale.vz = frame.record->ramp;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.drawMatrix, &frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.drawMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(3, g_BattleSpecialWorldCardLoadedResTable);
    frame.modelDraw->flags = 0x80;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.record->timer >= 35) {
        frame.record->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.record->timer == 25) {
        Battle_PostActorHitOrHurtMessage(frame.record->allocType);
    }
    if (frame.record->timer < 4) {
        frame.record->ramp += frame.record->rampAccel;
    }
    if (frame.record->timer >= 25) {
        frame.record->angle += 0x80;
        frame.record->ramp -= frame.record->rampDrag;
    }
    (frame.record->timer)++;
}

/*
 * Waits without drawing until age 3, advancing age only when unpaused. Thereafter draws resource
 * model 4 with flag 0x28 at a heading-rotated negative-Z offset and uniform scale, and retires at
 * age 35 before checking pause. Unpaused ages below 11 grow scale; ages at least 25 rotate by 128,
 * shrink scale and reduce the offset distance, then increment age.
 */
// FUNCTION: FF7 0x004ee236
void __cdecl BattleFx_SpecialWorldCardSecondaryModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxStopSideModel *record; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offsetX; };
        struct { unsigned char pad_to_18[0x18]; PsxMatrix headingMatrix; };
        struct { unsigned char pad_to_38[0x38]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_5C[0x5C]; SVECTOR headingX; };
        struct { unsigned char pad_to_64[0x64]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_68[0x68]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.record = (BattleFxStopSideModel *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.record->timer < 3) {
        if (g_BattleIsPaused == 0) {
            (frame.record->timer)++;
        }
    } else {
        frame.rotX.vx = 0;
        frame.rotX.vy = frame.record->yaw;
        frame.rotX.vz = frame.record->spinAngle;
        RotMatrixXYZ(&frame.rotX, &frame.drawMatrix);
        frame.headingX.vx = 0;
        frame.headingX.vy = frame.record->yaw;
        frame.headingX.vz = 0;
        RotMatrixXYZ(&frame.headingX, &frame.headingMatrix);
        frame.offsetX.vx = 0;
        frame.offsetX.vy = 0;
        frame.offsetX.vz = -frame.record->offsetDistance;
        ApplyMatrixToSVectorInt32(&frame.headingMatrix, &frame.offsetX, &frame.drawMatrix.t[0]);
        frame.drawMatrix.t[0] += frame.record->posX;
        frame.drawMatrix.t[1] += frame.record->posY;
        frame.drawMatrix.t[2] += frame.record->posZ;
        frame.scale.vz = frame.record->ramp;
        frame.scale.vy = frame.scale.vz;
        frame.scale.vx = frame.scale.vy;
        ScaleMatrix(&frame.drawMatrix, &frame.scale);
        MultiplyMatrix(&g_BattleCameraTransform, &frame.drawMatrix, &frame.drawMatrix);
        SetRotMatrix(&frame.drawMatrix);
        SetTransMatrix(&frame.drawMatrix);
        frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
        frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(4, g_BattleSpecialWorldCardLoadedResTable);
        frame.modelDraw->flags = 0x28;
        frame.modelDraw->uvScroll = 0;
        frame.modelDraw->fadeLevel = 128;
        frame.modelDraw->stateFlags = 0x20;
        frame.modelDraw->texturePageBits = 0;
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
        if (frame.record->timer >= 35) {
            frame.record->ownerSlot = BATTLE_TASK_FREE;
        }
        if (g_BattleIsPaused != 0) {
            return;
        }
        if (frame.record->timer < 11) {
            frame.record->ramp += frame.record->rampAccel;
        }
        if (frame.record->timer >= 25) {
            frame.record->spinAngle += 0x80;
            frame.record->ramp -= frame.record->rampDrag;
            frame.record->offsetDistance -= frame.record->offsetDistanceStep;
        }
        (frame.record->timer)++;
    }
}

/*
 * Waits until age 15 with age advancing only when unpaused, then draws model 1 with flag 0xA0 at a
 * heading-rotated negative-Z offset. Also draws selected model-2 copies whose table-derived angles
 * fall from 2048 through 4095; each selected copy cumulatively increases the local scale up to the
 * stored cap, uses flag 0xA8 and a table intensity. Retires age 35 before checking pause. Unpaused
 * ages below 23 rotate by 256 and reduce scale; ages at least 25 rotate by 128 and reduce scale
 * and offset distance.
 */
// FUNCTION: FF7 0x004ee493
void __cdecl BattleFx_SpecialWorldCardLayeredModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[132];
        int clampedScale;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; BattleFxAngleGatedModel *record; };
        struct { unsigned char pad_to_10[0x10]; SVECTOR offsetX; };
        struct { unsigned char pad_to_1C[0x1C]; int copyAngle; };
        struct { unsigned char pad_to_20[0x20]; int copyIndex; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix headingMatrix; };
        struct { unsigned char pad_to_44[0x44]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_68[0x68]; SVECTOR headingX; };
        struct { unsigned char pad_to_70[0x70]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_74[0x74]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.record = (BattleFxAngleGatedModel *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.record->phase < 15) {
        if (g_BattleIsPaused == 0) {
            (frame.record->phase)++;
        }
    } else {
        frame.rotX.vx = 0;
        frame.rotX.vy = frame.record->yaw;
        frame.rotX.vz = frame.record->spinAngle;
        RotMatrixXYZ(&frame.rotX, &frame.drawMatrix);
        frame.headingX.vx = 0;
        frame.headingX.vy = frame.record->yaw;
        frame.headingX.vz = 0;
        RotMatrixXYZ(&frame.headingX, &frame.headingMatrix);
        frame.offsetX.vx = 0;
        frame.offsetX.vy = 0;
        frame.offsetX.vz = -frame.record->sideDistance;
        ApplyMatrixToSVectorInt32(&frame.headingMatrix, &frame.offsetX, &frame.drawMatrix.t[0]);
        frame.drawMatrix.t[0] += frame.record->posX;
        frame.drawMatrix.t[1] += frame.record->posY;
        frame.drawMatrix.t[2] += frame.record->posZ;
        frame.scale.vz = frame.record->scale;
        frame.scale.vy = frame.scale.vz;
        frame.scale.vx = frame.scale.vy;
        ScaleMatrix(&frame.drawMatrix, &frame.scale);
        MultiplyMatrix(&g_BattleCameraTransform, &frame.drawMatrix, &frame.headingMatrix);
        SetRotMatrix(&frame.headingMatrix);
        SetTransMatrix(&frame.headingMatrix);
        frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
        frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleSpecialWorldCardLoadedResTable);
        frame.modelDraw->flags = 0xA0;
        frame.modelDraw->uvScroll = 0;
        frame.modelDraw->fadeLevel = 0;
        frame.modelDraw->stateFlags = 0;
        frame.modelDraw->texturePageBits = 0;
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
        frame.modelDraw->flags = 0xA8;
        frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(2, g_BattleSpecialWorldCardLoadedResTable);
        for (frame.copyIndex = 0; frame.copyIndex < 4; frame.copyIndex++) {
            frame.copyAngle = g_BattleSpecialWorldCardAngleOffsets[frame.copyIndex] + (frame.record->phase - 0xF << 8);
            if (frame.copyAngle < 0x800 || frame.copyAngle >= 0x1000) {
                continue;
            }
            frame.rotX.vz = *(short *)&frame.copyAngle;
            RotMatrixXYZ(&frame.rotX, &frame.drawMatrix);
            frame.copyAngle = frame.scale.vx + frame.record->scaleStep;
            if (frame.copyAngle > frame.record->clampLimit) {
                frame.clampedScale = frame.record->clampLimit;
            } else {
                frame.clampedScale = frame.copyAngle;
            }
            frame.copyAngle = frame.clampedScale;
            frame.scale.vz = frame.copyAngle;
            frame.scale.vy = frame.scale.vz;
            frame.scale.vx = frame.scale.vy;
            ScaleMatrix(&frame.drawMatrix, &frame.scale);
            MultiplyMatrix(&g_BattleCameraTransform, &frame.drawMatrix, &frame.headingMatrix);
            SetRotMatrix(&frame.headingMatrix);
            SetTransMatrix(&frame.headingMatrix);
            SetDrawColorRGB(0, 0, 0);
            frame.modelDraw->fadeLevel = g_BattleSpecialWorldCardFadeLevels[frame.copyIndex];
            g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
        }
        if (frame.record->phase >= 35) {
            frame.record->ownerSlot = BATTLE_TASK_FREE;
        }
        if (g_BattleIsPaused != 0) {
            return;
        }
        if (frame.record->phase < 23) {
            frame.record->spinAngle += 0x100 /* 22.5 deg */;
            frame.record->scale -= frame.record->scaleStep;
        }
        if (frame.record->phase >= 25) {
            frame.record->spinAngle += 0x80;
            frame.record->scale -= frame.record->lateScaleStep;
            frame.record->sideDistance -= frame.record->sideDistanceStep;
        }
        (frame.record->phase)++;
    }
}
} /* extern "C" */
