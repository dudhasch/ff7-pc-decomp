// src/ff7/src/battle/battle_fx_special_joker_card.cpp
/* Enemy Special Effect [idx 50 (unused TU)]: JokerCard (JP: joker1 / ジョーカー1 (Joker 1), asset: "special/joker1") — Joker (0x157, Joker; slot 50 in dispatch table points to Battle_EffectAttachStatusOverlayModel) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect Joker1: loads its resources, runs a timeline with a positional
//   sound and model callbacks, and runs the spinning, oscillating and accelerating model tasks plus
//   a begin-impact task and its per-frame impact handler.

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
// GLOBAL: FF7 0x0083b688
int g_BattleSpecialJokerCardResourceDefs = 0x001900B4;
int g_BattleSpecialJokerCardResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleSpecialJokerCardAxisAngleMatrix + 0x10)
};
int g_BattleSpecialJokerCardResourceDefsDataTail[8] = {
    0x0, 0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleSpecialJokerCardAxisAngleMatrix + 0x1c), 0x0,
    0x0, 0x0
};
// GLOBAL: FF7 0x0083b6b0
struct VECTOR g_BattleSpecialJokerCardNegativeZUnitVector = {0, 0, -0x1000, 0};
// GLOBAL: FF7 0x0083b6c0
unsigned char g_BattleSpecialJokerCardAxisAngleMatrix[40] = {0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x41, 0x52, 0x44, 0x31, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x00, 0x43, 0x41, 0x52, 0x44, 0x31, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00b49168
int g_BattleSpecialJokerCardDrawListHandle = 0;
unsigned char g_bssPad_00b4916c[4] = {0};
// GLOBAL: FF7 0x00b49170
VECTOR g_BattleSpecialJokerCardTransformedVector1 = {0};
// GLOBAL: FF7 0x00b49180
struct Int64Pair g_BattleSpecialJokerCardTargetPos = {0};
// GLOBAL: FF7 0x00b49188
VECTOR g_BattleSpecialJokerCardTransformedVector2 = {0};
// GLOBAL: FF7 0x00b49198
int g_BattleSpecialJokerCardTargetMask = 0;
// GLOBAL: FF7 0x00b4919c
int g_BattleSpecialJokerCardActorIdx = 0;
// GLOBAL: FF7 0x00b491a0
struct GfxTableCursor *g_BattleSpecialJokerCardLoadedResTable = 0;
unsigned char g_bssPad_00b491a4[4] = {0};
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

// If the battle context exists and the joker1 resource handle is zero, initializes a default-
// duration record, builds the special/joker1 resource path and requests resource type 2 using the
// context field at offset 0x4C. The record is represented by a scalar stack local with subsequent
// offset writes, so its apparent extent does not establish a safe source-level record allocation.
// FUNCTION: FF7 0x004ee870
void __cdecl BattleFx_SpecialLoadResJokerCard(void)
{
    GfxModelTableBank *context;
    GfxResourceRequest requestRecord;
    int kind;
    char pathOut[200];
    GfxResourceRequest *pReq;
    int resultDefEntry;

    if (g_pBattle3dScene != 0 && g_BattleSpecialJokerCardLoadedResTable == 0) {
        kind = 2;
        resultDefEntry = (int)&g_BattleSpecialJokerCardResourceDefs;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestRecord;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x0083b6e8
        pReq->resourcePath = BattleFx_BuildResourcePath("special/joker1", pathOut);
        Gfx_LoadModelTableIfAbsent(kind, resultDefEntry, pReq, &g_BattleSpecialJokerCardLoadedResTable, context);
    }
}

/*
 * Calls BattleFx_SpecialLoadResJokerCard, forwards both arguments to BattleFx_SpecialBeginJokerCard, and returns zero.
 */
// FUNCTION: FF7 0x004ee900
int __cdecl BattleFx_SpecialEnterJokerCard(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResJokerCard();
    BattleFx_SpecialBeginJokerCard(targetMask, actorIdx);
    return 0;
}

/*
 * Allocates and saves a draw-list-selection slot, stores targetMask as the actor mask, and records its
 * first selected low-ten-bit actor when one exists, leaving the previous actor value when none
 * exists. Stores actorIdx and allocates BattleFx_SpecialJokerCardSpawnTimeline.
 */
// FUNCTION: FF7 0x004ee91c
void __cdecl BattleFx_SpecialBeginJokerCard(int targetMask, int actorIdx)
{
    int bitIdx;
    int count;

    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    g_BattleSpecialJokerCardTargetMask = targetMask;
    bitIdx = 0;
    for (count = 0; bitIdx < 0xA; bitIdx++) {
        if ((targetMask & 1 << bitIdx) != 0) {
            count++;
            if (count == 1) {
                g_BattleSpecialJokerCardDrawListHandle = bitIdx;
            }
        }
    }
    g_BattleSpecialJokerCardActorIdx = actorIdx;
    Battle_AllocateResourceSlot(BattleFx_SpecialJokerCardSpawnTimeline);
}

/*
 * Retires the current slot, obtains the selected actor bone position into a local eight-byte pair
 * and copies that pair to +4 of a new BattleFx_SpecialJokerCardTimelineTask slot. Calls BattleFx_SpecialJokerCardBuildFacingVectors, then allocates
 * BattleFx_SpecialJokerCardDelayedImpactTask with the same eight-byte pair; no pause guard appears.
 */
// FUNCTION: FF7 0x004ee9ad
void __cdecl BattleFx_SpecialJokerCardSpawnTimeline(void)
{
    struct {
        int *slot;
        BattleBonePosCopy pos;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = (int *)BATTLE_CURRENT_TASK_SLOT;
    *(short *)frame.slot = 0xffff;
    Battle_GetBoneWorldPos(g_BattleSpecialJokerCardDrawListHandle,
                           g_BattleActorRecords[g_BattleSpecialJokerCardDrawListHandle].targetBone,
                           (short *)&frame.pos);
    frame.slot = (int *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialJokerCardTimelineTask);
    *(BattleBonePosCopy *)(frame.slot + 1) = frame.pos;
    BattleFx_SpecialJokerCardBuildFacingVectors();
    frame.slot = (int *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialJokerCardDelayedImpactTask);
    *(BattleBonePosCopy *)(frame.slot + 1) = frame.pos;
}

/*
 * Runs an unpaused sequence that creates a positioned callback and positional sound at age 2, a
 * vertically moving callback and centered sound at age 24, then at age 72 samples the target
 * attachment and creates an axis-angle-oriented moving callback toward its X/Z position with
 * positional sound. Uses the normalized distance square root divided by 12.
 * Retires at age 90 before incrementing age.
 */
// FUNCTION: FF7 0x004eea51
void __cdecl BattleFx_SpecialJokerCardTimelineTask(void)
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
        frame.slot = (BattleFxDiamondCardSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialJokerCardSpinningModelTask);
        *(Int64Pair *)((char *)frame.slot + 4) = *(Int64Pair *)(&g_BattleSpecialJokerCardTargetPos);
        frame.distance = g_BattleActorRecords[g_BattleSpecialJokerCardActorIdx].posY - ((short *)&g_BattleSpecialJokerCardTargetPos)[1];
        if (frame.distance < 0) {
            frame.distance *= -1;
        }
        frame.slot->distance = frame.distance;
        frame.slot->velocityY = 0xFED4;
        frame.slot->scale = 0x2000;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleSpecialJokerCardTargetPos), 0x1D5);
    }
    if (frame.current->step == 0x18) {
        frame.slot = (BattleFxDiamondCardSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialJokerCardOscillatingModelTask);
        frame.slot->posY = 0xFEF6;
        frame.slot->velocityY = 0x21;
        frame.slot->angle = 0;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, 0x40, 0x1D6);
    }
    if (frame.current->step == 0x48) {
        Battle_GetBoneWorldPos(g_BattleSpecialJokerCardDrawListHandle, g_BattleActorRecords[g_BattleSpecialJokerCardDrawListHandle].targetBone, (short *)((char *)frame.current + 4));
        frame.vector[0] = frame.current->posX - g_BattleCameraPosition.cameraPosX;
        frame.vector[1] = 0;
        frame.vector[2] = frame.current->posZ - g_BattleCameraPosZ;
        frame.distance = NormalizeVectorFixed(&frame.vector[0], &frame.vector[0]);
        frame.distance = SqrtToInt(frame.distance) / 0xC;
        frame.angle = ComputeAngleBetweenVectors(&g_BattleSpecialJokerCardNegativeZUnitVector, (VECTOR *)(&frame.vector[0]), (VECTOR *)(&frame.axis[0]));
        Gte_BuildAxisAngleMatrix(frame.angle, (PsxMatrix *)(&frame.matrix[0]), (VECTOR *)(&frame.axis[0]));
        TransformVectorToInt32((PsxMatrix *)(&frame.matrix[0]), &g_BattleSpecialJokerCardNegativeZUnitVector, &frame.vector[0]);
        if (frame.axis[1] < 0) {
            frame.angle *= -1;
        }
        frame.slot = (BattleFxDiamondCardSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialJokerCardAcceleratingModelTask);
        frame.slot->posX = g_BattleCameraPosition.cameraPosX;
        frame.slot->posY = frame.current->posY;
        frame.slot->posZ = g_BattleCameraPosZ;
        frame.slot->offsetX = frame.distance * frame.vector[0] >> 0xC;
        frame.slot->velocityY = 0;
        frame.slot->offsetZ = frame.distance * frame.vector[2] >> 0xC;
        frame.slot->angle = frame.angle;
        frame.slot->scale = 0x2666;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition((char *)frame.current + 4), 0x1D7);
    }
    if (frame.current->step >= 0x5A) {
        frame.current->ownerSlot = 0xFFFF;
    }
    (frame.current->step)++;
}

/*
 * Draws a uniformly scaled joker1 model using the stored actor heading, an age-driven axis
 * rotation and a rotated vertical offset; the angle advances through age 8 then remains a quarter
 * turn. Ages at least 18 retire before checking pause. From unpaused age 8 onward, adds vertical
 * velocity to Y and increases that velocity by its sixth, then increments age.
 */
// FUNCTION: FF7 0x004eed22
void __cdecl BattleFx_SpecialJokerCardSpinningModelTask(void)
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
        frame.axisAngle = 0x400;
    }
    frame.headingX.vx = 0;
    frame.headingX.vy = (((short *)&g_BattleSpecialJokerCardTargetPos)[3]);
    frame.headingX.vz = 0;
    RotMatrixXYZ(&frame.headingX, &frame.drawMatrix);
    Gte_BuildAxisAngleMatrix(-frame.axisAngle, &frame.axisMatrix, (VECTOR *)&g_BattleSpecialJokerCardAxisAngleMatrix);
    ConcatMatrixInPlace(&frame.drawMatrix, &frame.axisMatrix);
    frame.offsetX.vx = 0;
    frame.offsetX.vy = frame.slot->offsetY;
    frame.offsetX.vz = 0;
    TransformVectorToInt16(&frame.axisMatrix, &frame.offsetX, &frame.offsetX.vx);
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0x800;
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
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialJokerCardLoadedResTable);
    frame.modelDraw->flags = 0;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0x80;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.slot->frameCount >= 0x12) {
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
 * Draws the joker1 resource model at fixed X/Z and slot Y minus 40 with a quarter-turn X rotation
 * and configurable Y rotation. Retires at age 48 before checking pause. Unpaused ages below 8 move
 * Y with damped velocity clamped to at least 1; ages 8 through 39 set Y to a sine oscillation and
 * rotate during ages 22 through 33; later ages move Y with velocity increased by a quarter.
 */
// FUNCTION: FF7 0x004eef77
void __cdecl BattleFx_SpecialJokerCardOscillatingModelTask(void)
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
    frame.rotX.vx = 0x400;
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
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialJokerCardLoadedResTable);
    frame.modelDraw->flags = 0;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0x80;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.slot->phase >= 0x30) {
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
    } else if (frame.slot->phase < 0x28) {
        frame.sinAngle = frame.slot->phase - 8 << 7;
        frame.slot->offset = SinFixed(frame.sinAngle) * 0x18 >> 0xC;
        if (frame.slot->phase >= 0x16 && frame.slot->phase < 0x22) {
            frame.slot->angle -= 0xAA;
        }
    } else {
        frame.slot->offset += frame.slot->velocity;
        frame.slot->velocity += (short)(frame.slot->velocity >> 2);
    }
    (frame.slot->phase)++;
}

/*
 * Draws a uniformly scaled joker1 model at the slot position with configured Y rotation and half-
 * turn Z rotation; from age 12 switches to model 1, enables flag 8 and reduces intensity by 16 per
 * age. Retires at age 20 before checking pause. Unpaused age 10 triggers a target operation
 * selected by screen pan above 64 and positional sound; unpaused calls advance X/Z and increase
 * their velocities by a sixteenth before advancing age.
 */
// FUNCTION: FF7 0x004ef1b2
void __cdecl BattleFx_SpecialJokerCardAcceleratingModelTask(void)
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
    frame.rotX.vz = 0x800;
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
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialJokerCardLoadedResTable);
    frame.modelDraw->flags = 0;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0x80;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    if (frame.slot->timer >= 0xC) {
        frame.modelDraw->flags = 8;
        frame.modelDraw->fadeLevel = 0x80 - (frame.slot->timer - 0xC << 4);
        frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleSpecialJokerCardLoadedResTable);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.slot->timer >= 0x14) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.slot->timer == 0xA) {
        frame.screenPan = Battle_ComputeScreenPanFromPosition((char *)frame.slot + 4);
        if (frame.screenPan > 0x40) {
            BattleFx_HitMarkSpawnAtActorBone(g_BattleSpecialJokerCardDrawListHandle, 0x118);
        } else {
            BattleFx_HitMarkSpawnAtActorBone(g_BattleSpecialJokerCardDrawListHandle, 0x18);
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
 * Gets the g_BattleSpecialJokerCardActorIdx actor bone selected by its +0xBE11AD byte into
 * g_BattleSpecialJokerCardTargetPos, lowers Y by 224 and stores actor facing in the fourth short. Builds a Y
 * rotation from that facing and transforms the two fixed vectors at
 * g_BattleSpecialJokerCardNegativeZUnitVector (0x83B6B0) and g_BattleSpecialJokerCardAxisAngleMatrix
 * (0x83B6C0) into g_BattleSpecialJokerCardTransformedVector2 (0xB49188) and
 * g_BattleSpecialJokerCardTransformedVector1 (0xB49170).
 */
// FUNCTION: FF7 0x004ef414
void __cdecl BattleFx_SpecialJokerCardBuildFacingVectors(void)
{
    struct {
        int matrix[8];
        short rot[4];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    Battle_GetBoneWorldPos(g_BattleSpecialJokerCardActorIdx,
                           g_BattleActorRecords[g_BattleSpecialJokerCardActorIdx].altBone,
                           (short *)&g_BattleSpecialJokerCardTargetPos);
    ((short *)&g_BattleSpecialJokerCardTargetPos)[1] -= 0xe0;
    ((short *)&g_BattleSpecialJokerCardTargetPos)[3] = g_BattleActorRecords[g_BattleSpecialJokerCardActorIdx].yaw;
    frame.rot[0] = 0;
    frame.rot[1] = ((short *)&g_BattleSpecialJokerCardTargetPos)[3];
    frame.rot[2] = 0;
    RotMatrixXYZ((void *)frame.rot, (void *)frame.matrix);
    TransformVectorToInt32((PsxMatrix *)frame.matrix, &g_BattleSpecialJokerCardNegativeZUnitVector, &g_BattleSpecialJokerCardTransformedVector2);
    TransformVectorToInt32((PsxMatrix *)frame.matrix, &g_BattleSpecialJokerCardAxisAngleMatrix, &g_BattleSpecialJokerCardTransformedVector1);
}

/*
 * One frame of a battle effect that only waits, the same body as BattleFx_SpecialWorldCardDelayedImpactTask with a different
 * payload: on exactly frame 0x5A of the counter at +2 it calls BattleFx_SpecialJokerCardSpawnBeginImpactTask(g_BattleSpecialJokerCardDrawListHandle,
 * g_BattleSpecialJokerCardActorIdx) and retires the effect with 0xFFFF at +0. Returns without advancing the counter
 * while g_BattleIsPaused is set.
 */
// FUNCTION: FF7 0x004ef4c1
void __cdecl BattleFx_SpecialJokerCardDelayedImpactTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0x5A) {
        BattleFx_SpecialJokerCardSpawnBeginImpactTask(g_BattleSpecialJokerCardDrawListHandle, g_BattleSpecialJokerCardActorIdx);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/*
 * Allocates a resource slot for BattleFx_SpecialJokerCardBeginImpactTask without further initialization.
 */
// FUNCTION: FF7 0x004ef525
void __cdecl BattleFx_SpecialJokerCardSpawnBeginImpactTask(int drawListHandle, int actorIdx)
{
    Battle_AllocateResourceSlot(BattleFx_SpecialJokerCardBeginImpactTask);
}

/*
 * Retires the current slot, allocates BattleFx_SpecialJokerCardImpactTimelineTask, fills its position
 * from the actor bone selected by g_BattleActorRecords[0].altBone, and copies the actor
 * g_BattleActorRecords[0].yaw word into slot +0xC.
 */
// FUNCTION: FF7 0x004ef537
void __cdecl BattleFx_SpecialJokerCardBeginImpactTask(void)
{
    BattleFxJokerCardImpactTask2 *task;

    task = (BattleFxJokerCardImpactTask2 *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    task = (BattleFxJokerCardImpactTask2 *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialJokerCardImpactTimelineTask);
    Battle_GetBoneWorldPos(g_BattleSpecialJokerCardDrawListHandle, g_BattleActorRecords[g_BattleSpecialJokerCardDrawListHandle].altBone, (short *)((char *)task + 4));
    task->yaw = g_BattleActorRecords[g_BattleSpecialJokerCardDrawListHandle].yaw;
}

/*
 * Per-frame handler for the current effect slot at g_BattleHitLivewireEmitterRecords + g_BattleActiveTaskSlotIdx * 0x20. On frame 1
 * it allocates a ring-buffer record of type 1 and copies the slot's 8 bytes at +4 and short at
 * +0xC into it (+4, +0x10); on frame 0x11 it fires sound opcode 0x20 with pan from
 * Battle_ComputeScreenPanFromPosition(slot+4) and 0x1A4; on frame 0x20 it allocates a type-2 record.
 * Retires the slot (0xFFFF at +0) from frame 0x32; unless g_BattleIsPaused is set it allocates a typed
 * record g_BattleSpecialJokerCardDrawListHandle on frame 0x24 and advances the frame counter at +2.
 */
// FUNCTION: FF7 0x004ef5b8
void __cdecl BattleFx_SpecialJokerCardImpactTimelineTask(void)
{
    int q;
    BattleFxWeaponEmeraldBeamBeam *rec;

    rec = (BattleFxWeaponEmeraldBeamBeam *)BATTLE_CURRENT_TASK_SLOT;
    if (rec->stage == 1) {
        q = Battle_AllocSceneMessage(1);
        *(Int64Pair *)((char *)q + 4) = *(Int64Pair *)((char *)rec + 4);
        *(short *)((char *)q + 0x10) = rec->thicknessScale;
    } else if (rec->stage == 0x11) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition((char *)rec + 4), 0x1A4);
    } else if (rec->stage == 0x20) {
        Battle_AllocSceneMessage(2);
    }
    if (rec->stage >= 0x32) {
        rec->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (rec->stage == 0x24) {
        Battle_PostActorHitOrHurtMessage(g_BattleSpecialJokerCardDrawListHandle);
    }
    (rec->stage)++;
}
} /* extern "C" */
