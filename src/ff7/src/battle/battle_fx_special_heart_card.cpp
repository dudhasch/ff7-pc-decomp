// src/ff7/src/battle/battle_fx_special_heart_card.cpp
/* Enemy Special Effect [idx 47]: HeartCard (JP: heart1 / ハート1 (Heart 1), asset: "special/heart1") — Heart (0x154, Joker) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the first heart attack. It loads the heart resources, starts
//   the effect with a bone-particle emitter that spawns sprite billboards, and runs the timeline
//   and model tasks.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <stdlib.h>
#include <string.h>
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/api/graphics.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle_actors.h"
extern "C" {

#pragma function(memcpy)
// clang-format off
// GLOBAL: FF7 0x00857b68
int g_BattleSpecialHeartCardResourceDefs = 0x001900B4;
int g_BattleSpecialHeartCardResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleSpecialHeartCardResourceNames + 0x4)
};
int g_BattleSpecialHeartCardResourceDefsDataTail[8] = {
    0x0, 0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleSpecialHeartCardResourceNames + 0x10), 0x0,
    0x0, 0x0
};
// GLOBAL: FF7 0x00857b90
struct VECTOR g_BattleSpecialHeartCardForwardAxisFixed12 = {0, 0, -0x1000, 0};
// GLOBAL: FF7 0x00857ba0
struct VECTOR g_BattleSpecialHeartCardRightAxisFixed12 = {0x1000, 0, 0, 0};
// g_BattleSpecialHeartCardAlphaTable is an 8-byte table the code indexes
// (BattleFx_SpecialHeartCardBoneParticleSpriteTask (0x004f79bf), unbounded); its last four bytes were
// the fill g_rawData_00857bb4. The strings after it stay a fill: the TU's pointer words reach them.
// GLOBAL: FF7 0x00857bb0
unsigned char g_BattleSpecialHeartCardAlphaTable[0x8] = {0x00, 0x20, 0x40, 0x60, 0x80, 0xa0, 0xc0, 0xe0};
unsigned char g_BattleSpecialHeartCardResourceNames[28] = {0x00, 0x00, 0x00, 0x00, 0x43, 0x41, 0x52, 0x44, 0x31, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x00, 0x43, 0x41, 0x52, 0x44, 0x31, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00b49d30
int g_BattleSpecialHeartCardTargetActorIdx = 0;
unsigned char g_bssPad_00b49d34[4] = {0};
// GLOBAL: FF7 0x00b49d38
VECTOR g_BattleSpecialHeartCardTransformedOffsetVector = {0};
// GLOBAL: FF7 0x00b49d48
BattleSpecialHeartCardBonePos g_BattleSpecialHeartCardActorBoneWorldPos = {0};
// GLOBAL: FF7 0x00b49d50
VECTOR g_BattleSpecialHeartCardTransformedForwardVector = {0};
// GLOBAL: FF7 0x00b49d60
int g_BattleSpecialHeartCardTargetMask = 0;
// GLOBAL: FF7 0x00b49d64
int g_BattleSpecialHeartCardActorIndex = 0;
// GLOBAL: FF7 0x00b49d68
struct GfxTableCursor *g_BattleSpecialHeartCardLoadedResTable = 0;
unsigned char g_bssPad_00b49d6c[4] = {0};
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

// If the battle context exists and the heart1 resource handle is zero, initializes a default-
// duration record, builds the special/heart1 resource path and requests resource type 2 using the
// context field at offset 0x4C. The record is represented by a scalar stack local with subsequent
// offset writes, so its apparent extent does not establish a safe source-level record allocation.
// FUNCTION: FF7 0x004f6af0
void __cdecl BattleFx_SpecialLoadResHeartCard(void)
{
    GfxModelTableBank *context;
    GfxResourceRequest requestRecord;
    int kind;
    char pathOut[200];
    GfxResourceRequest *pReq;
    int resultDefEntry;

    if (g_pBattle3dScene != 0 && g_BattleSpecialHeartCardLoadedResTable == 0) {
        kind = 2;
        resultDefEntry = (int)&g_BattleSpecialHeartCardResourceDefs;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestRecord;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x00857bd4
        pReq->resourcePath = BattleFx_BuildResourcePath("special/heart1", pathOut);
        Gfx_LoadModelTableIfAbsent(kind, resultDefEntry, pReq, &g_BattleSpecialHeartCardLoadedResTable, context);
    }
}

// Mechanical two-call forwarder: calls BattleFx_SpecialLoadResHeartCard with no arguments, then BattleFx_SpecialBeginHeartCard(targetMask,
// actorIdx).
// FUNCTION: FF7 0x004f6b80
void __cdecl BattleFx_SpecialEnterHeartCard(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResHeartCard();
    BattleFx_SpecialBeginHeartCard(targetMask, actorIdx);
}

// Allocates and saves a draw-list-selection slot, stores targetMask as the actor mask and records its
// first selected low-ten-bit actor when one exists, retaining the previous actor value when none
// exists. Stores actorIdx and allocates BattleFx_SpecialHeartCardSpawnTimeline.
// FUNCTION: FF7 0x004f6b9a
void __cdecl BattleFx_SpecialBeginHeartCard(int targetMask, int actorIdx)
{
    int bitIdx;
    int selectedCount;

    g_BattleFxDrawListBufferTaskSlotPtr = (unsigned char *)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    g_BattleSpecialHeartCardTargetMask = targetMask;
    bitIdx = 0;
    for (selectedCount = 0; bitIdx < 0xA; bitIdx++) {
        if ((targetMask & 1 << bitIdx) != 0) {
            selectedCount++;
            if (selectedCount == 1) {
                g_BattleSpecialHeartCardTargetActorIdx = bitIdx;
            }
        }
    }
    g_BattleSpecialHeartCardActorIndex = actorIdx;
    Battle_AllocateResourceSlot(BattleFx_SpecialHeartCardSpawnTimeline);
}

// Retires the current slot, gets the selected actor bone position into a local eight-byte pair and
// copies that pair to +4 of a new BattleFx_SpecialHeartCardTimelineTask slot. Calls BattleFx_SpecialHeartCardBuildFacingVectors, then allocates
// BattleFx_SpecialHeartCardDelayedImpactTask with the same pair; no pause guard appears.
// FUNCTION: FF7 0x004f6c2b
void __cdecl BattleFx_SpecialHeartCardSpawnTimeline(void)
{
    struct {
        BattleTaskSlot *slot;
        BattleBonePosCopy pos;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = BATTLE_CURRENT_TASK_SLOT;
    frame.slot->ownerSlot = 0xffff;
    Battle_GetBoneWorldPos(g_BattleSpecialHeartCardTargetActorIdx,
                           g_BattleActorRecords[g_BattleSpecialHeartCardTargetActorIdx].targetBone,
                           (short *)&frame.pos);
    frame.slot = BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHeartCardTimelineTask);
    *(BattleBonePosCopy *)frame.slot->data = frame.pos;
    BattleFx_SpecialHeartCardBuildFacingVectors();
    frame.slot = BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHeartCardDelayedImpactTask);
    *(BattleBonePosCopy *)frame.slot->data = frame.pos;
}

// Runs an unpaused sequence that creates a positioned callback and positional sound at age 2, a
// vertically moving callback and centered sound at age 24, then at age 72 samples the target
// attachment and creates an axis-angle-oriented moving callback toward its X/Z position with
// positional sound. Uses the normalized distance square root divided by 12.
// Retires at age 90 before incrementing age.
// FUNCTION: FF7 0x004f6ccf
void __cdecl BattleFx_SpecialHeartCardTimelineTask(void)
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
        frame.slot = (BattleFxSunCardSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHeartCardSpinningModelTask);
        *(Int64Pair *)&frame.slot->posX = *(Int64Pair *)&g_BattleSpecialHeartCardActorBoneWorldPos;
        frame.distance = g_BattleActorRecords[g_BattleSpecialHeartCardActorIndex].posY - g_BattleSpecialHeartCardActorBoneWorldPos.y;
        if (frame.distance < 0) {
            frame.distance *= -1;
        }
        frame.slot->distance = frame.distance;
        frame.slot->velocityY = 0xFED4;
        frame.slot->scale = 0x2000;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleSpecialHeartCardActorBoneWorldPos), 0x1D5);
    }
    if (frame.current->step == 0x18) {
        frame.slot = (BattleFxSunCardSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHeartCardOscillatingModelTask);
        frame.slot->posY = 0xFEF6;
        frame.slot->velocityY = 0x21;
        frame.slot->angle = 0;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, 0x40, 0x1D6);
    }
    if (frame.current->step == 0x48) {
        Battle_GetBoneWorldPos(g_BattleSpecialHeartCardTargetActorIdx, g_BattleActorRecords[g_BattleSpecialHeartCardTargetActorIdx].targetBone, &frame.current->posX);
        frame.vector[0] = frame.current->posX - g_BattleCameraPosition.cameraPosX;
        frame.vector[1] = 0;
        frame.vector[2] = frame.current->posZ - g_BattleCameraPosZ;
        frame.distance = NormalizeVectorFixed(&frame.vector[0], &frame.vector[0]);
        frame.distance = SqrtToInt(frame.distance) / 0xC;
        frame.angle = ComputeAngleBetweenVectors(&g_BattleSpecialHeartCardForwardAxisFixed12, (VECTOR *)(&frame.vector[0]), (VECTOR *)(&frame.axis[0]));
        Gte_BuildAxisAngleMatrix(frame.angle, (PsxMatrix *)(&frame.matrix[0]), (VECTOR *)(&frame.axis[0]));
        TransformVectorToInt32((PsxMatrix *)(&frame.matrix[0]), &g_BattleSpecialHeartCardForwardAxisFixed12, &frame.vector[0]);
        if (frame.axis[1] < 0) {
            frame.angle *= -1;
        }
        frame.slot = (BattleFxSunCardSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHeartCardAcceleratingModelTask);
        frame.slot->posX = g_BattleCameraPosition.cameraPosX;
        frame.slot->posY = frame.current->posY;
        frame.slot->posZ = g_BattleCameraPosZ;
        frame.slot->offsetX = frame.distance * frame.vector[0] >> 0xC;
        frame.slot->velocityY = 0;
        frame.slot->offsetZ = frame.distance * frame.vector[2] >> 0xC;
        frame.slot->angle = frame.angle;
        frame.slot->scale = 0x2666;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&frame.current->posX), 0x1D7);
    }
    if (frame.current->step >= 0x5A) {
        frame.current->ownerSlot = 0xFFFF;
    }
    (frame.current->step)++;
}

// Draws a uniformly scaled heart1 model using the stored actor heading, an age-driven axis
// rotation and a rotated vertical offset; the angle advances through age 8 then remains a quarter
// turn. Ages at least 18 retire before checking pause. From unpaused age 8 onward, adds vertical
// velocity to Y and increases that velocity by its sixth, then increments age.
// FUNCTION: FF7 0x004f6fa0
void __cdecl BattleFx_SpecialHeartCardSpinningModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        BattleFxDiamondCardSpinRecord *spinRecord;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offsetX; };
        struct { unsigned char pad_to_18[0x18]; int axisAngle; };
        struct { unsigned char pad_to_1C[0x1C]; PsxMatrix axisMatrix; };
        struct { unsigned char pad_to_3C[0x3C]; PsxMatrix modelMatrix; };
        struct { unsigned char pad_to_5C[0x5C]; SVECTOR actorRotX; };
        struct { unsigned char pad_to_64[0x64]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_68[0x68]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.spinRecord = (BattleFxDiamondCardSpinRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.spinRecord->step <= 8) {
        frame.axisAngle = frame.spinRecord->step << 7;
    } else {
        frame.axisAngle = 0x400;
    }
    frame.actorRotX.vx = 0;
    frame.actorRotX.vy = g_BattleSpecialHeartCardActorBoneWorldPos.heading;
    frame.actorRotX.vz = 0;
    RotMatrixXYZ(&frame.actorRotX, &frame.modelMatrix);
    Gte_BuildAxisAngleMatrix(-frame.axisAngle, &frame.axisMatrix, &g_BattleSpecialHeartCardRightAxisFixed12);
    ConcatMatrixInPlace(&frame.modelMatrix, &frame.axisMatrix);
    frame.offsetX.vx = 0;
    frame.offsetX.vy = frame.spinRecord->offsetDistance;
    frame.offsetX.vz = 0;
    TransformVectorToInt16(&frame.axisMatrix, &frame.offsetX, &frame.offsetX.vx);
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0x800;
    RotMatrixXYZ(&frame.rotX, &frame.modelMatrix);
    ConcatMatrixInPlace(&frame.axisMatrix, &frame.modelMatrix);
    frame.modelMatrix.t[0] = frame.spinRecord->posX + frame.offsetX.vx;
    frame.modelMatrix.t[1] = frame.spinRecord->posY + frame.offsetX.vy;
    frame.modelMatrix.t[2] = frame.spinRecord->posZ + frame.offsetX.vz;
    frame.scaleX.vz = frame.spinRecord->scale;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.modelMatrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialHeartCardLoadedResTable);
    frame.modelDraw->flags = 0;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0x80;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.spinRecord->step >= 0x12) {
        frame.spinRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.spinRecord->step >= 8) {
        frame.spinRecord->posY += frame.spinRecord->velocityY;
        frame.spinRecord->velocityY += (short)(frame.spinRecord->velocityY / 6);
    }
    (frame.spinRecord->step)++;
}

// Draws the heart1 resource model at fixed X/Z and slot Y minus 40 with a quarter-turn X rotation
// and configurable Y rotation. Retires at age 48 before checking pause. Unpaused ages below 8 move
// Y with damped velocity clamped to at least 1; ages 8 through 39 set Y to a sine oscillation and
// rotate during ages 22 through 33; later ages move Y with velocity increased by a quarter.
// FUNCTION: FF7 0x004f71f5
void __cdecl BattleFx_SpecialHeartCardOscillatingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        BattleFxOscillatingModel *oscRecord;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_18[0x18]; int sineAngle; };
        struct { unsigned char pad_to_1C[0x1C]; PsxMatrix spinMatrix; };
        struct { unsigned char pad_to_3C[0x3C]; PsxMatrix modelMatrix; };
        struct { unsigned char pad_to_5C[0x5C]; SVECTOR spinX; };
        struct { unsigned char pad_to_64[0x64]; BattleFxModelDraw *modelDraw; };
    } frame;
#pragma pack(pop)

    frame.oscRecord = (BattleFxOscillatingModel *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0x400;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.modelMatrix);
    frame.modelMatrix.t[0] = 0;
    frame.modelMatrix.t[1] = frame.oscRecord->offset - 0x28;
    frame.modelMatrix.t[2] = 0x380;
    frame.spinX.vx = 0;
    frame.spinX.vy = frame.oscRecord->angle;
    frame.spinX.vz = 0;
    RotMatrixXYZ(&frame.spinX, &frame.spinMatrix);
    ConcatMatrixInPlace(&frame.spinMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialHeartCardLoadedResTable);
    frame.modelDraw->flags = 0;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0x80;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.oscRecord->phase >= 0x30) {
        frame.oscRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.oscRecord->phase < 8) {
        frame.oscRecord->offset += frame.oscRecord->velocity;
        frame.oscRecord->velocity -= (short)(frame.oscRecord->velocity >> 5);
        if (frame.oscRecord->velocity < 2) {
            frame.oscRecord->velocity = 1;
        }
    } else if (frame.oscRecord->phase < 0x28) {
        frame.sineAngle = frame.oscRecord->phase - 8 << 7;
        frame.oscRecord->offset = SinFixed(frame.sineAngle) * 0x18 >> 0xC;
        if (frame.oscRecord->phase >= 0x16 && frame.oscRecord->phase < 0x22) {
            frame.oscRecord->angle -= 0xAA;
        }
    } else {
        frame.oscRecord->offset += frame.oscRecord->velocity;
        frame.oscRecord->velocity += (short)(frame.oscRecord->velocity >> 2);
    }
    (frame.oscRecord->phase)++;
}

// Draws a uniformly scaled heart1 model at the slot position with configured Y rotation and half-
// turn Z rotation; from age 12 switches to model 1, enables flag 8 and reduces intensity by 16 per
// age. Retires at age 20 before checking pause. Unpaused age 10 triggers a target operation
// selected by screen pan above 64 and positional sound; unpaused calls advance X/Z and increase
// their velocities by a sixteenth before advancing age.
// FUNCTION: FF7 0x004f7430
void __cdecl BattleFx_SpecialHeartCardAcceleratingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        BattleFxSpecialAcceleratingSlot *accelRecord;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_18[0x18]; int screenPan; };
        struct { unsigned char pad_to_3C[0x3C]; PsxMatrix modelMatrix; };
        struct { unsigned char pad_to_64[0x64]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_68[0x68]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.accelRecord = (BattleFxSpecialAcceleratingSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.accelRecord->angleY;
    frame.rotX.vz = 0x800;
    RotMatrixXYZ(&frame.rotX, &frame.modelMatrix);
    frame.modelMatrix.t[0] = frame.accelRecord->posA;
    frame.modelMatrix.t[1] = frame.accelRecord->posY;
    frame.modelMatrix.t[2] = frame.accelRecord->posB;
    frame.scale.vz = frame.accelRecord->scale;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.modelMatrix, &frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialHeartCardLoadedResTable);
    frame.modelDraw->flags = 0;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0x80;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    if (frame.accelRecord->timer >= 0xC) {
        frame.modelDraw->flags = 8;
        frame.modelDraw->fadeLevel = 0x80 - (frame.accelRecord->timer - 0xC << 4);
        frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleSpecialHeartCardLoadedResTable);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.accelRecord->timer >= 0x14) {
        frame.accelRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.accelRecord->timer == 0xA) {
        frame.screenPan = Battle_ComputeScreenPanFromPosition(&frame.accelRecord->posA);
        if (frame.screenPan > 0x40) {
            BattleFx_HitMarkSpawnAtActorBone(g_BattleSpecialHeartCardTargetActorIdx, 0x118);
        } else {
            BattleFx_HitMarkSpawnAtActorBone(g_BattleSpecialHeartCardTargetActorIdx, 0x18);
        }
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, frame.screenPan, 0x1D8);
    }
    frame.accelRecord->posA += frame.accelRecord->velocityA;
    frame.accelRecord->posB += frame.accelRecord->velocityB;
    frame.accelRecord->velocityA += (short)(frame.accelRecord->velocityA >> 4);
    frame.accelRecord->velocityB += (short)(frame.accelRecord->velocityB >> 4);
    (frame.accelRecord->timer)++;
}

// Gets the g_BattleSpecialHeartCardActorIndex actor bone selected by its +0xBE11AD byte into
// g_BattleSpecialHeartCardActorBoneWorldPos, lowers Y by 224 and stores actor facing at +6. Builds a Y
// rotation from that facing and transforms fixed vectors at g_BattleSpecialHeartCardForwardAxisFixed12
// (0x857B90) and g_BattleSpecialHeartCardRightAxisFixed12 (0x857BA0) into
// g_BattleSpecialHeartCardTransformedForwardVector (0xB49D50) and
// g_BattleSpecialHeartCardTransformedOffsetVector (0xB49D38).
// FUNCTION: FF7 0x004f7692
void __cdecl BattleFx_SpecialHeartCardBuildFacingVectors(void)
{
    struct {
        int matrix[8];
        short rot[4];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    Battle_GetBoneWorldPos(g_BattleSpecialHeartCardActorIndex,
                           g_BattleActorRecords[g_BattleSpecialHeartCardActorIndex].altBone,
                           &g_BattleSpecialHeartCardActorBoneWorldPos.x);
    g_BattleSpecialHeartCardActorBoneWorldPos.y -= 0xe0;
    g_BattleSpecialHeartCardActorBoneWorldPos.heading = g_BattleActorRecords[g_BattleSpecialHeartCardActorIndex].yaw;
    frame.rot[0] = 0;
    frame.rot[1] = g_BattleSpecialHeartCardActorBoneWorldPos.heading;
    frame.rot[2] = 0;
    RotMatrixXYZ((void *)frame.rot, (void *)frame.matrix);
    TransformVectorToInt32((PsxMatrix *)frame.matrix, &g_BattleSpecialHeartCardForwardAxisFixed12, &g_BattleSpecialHeartCardTransformedForwardVector);
    TransformVectorToInt32((PsxMatrix *)frame.matrix, &g_BattleSpecialHeartCardRightAxisFixed12, &g_BattleSpecialHeartCardTransformedOffsetVector);
}

// One frame of a battle effect step, the same shape as BattleFx_SpecialClubCardDelayedImpactTask: on the slot at &g_BattleHitLivewireEmitterRecords
// + (g_BattleActiveTaskSlotIdx << 5) it returns while g_BattleIsPaused is nonzero, and at frame counter 0x5A calls
// BattleFx_SpecialHeartCardSpawnBoneParticleEmitterWithSound(g_BattleSpecialHeartCardTargetActorIdx, g_BattleSpecialHeartCardActorIndex) and retires the slot by writing 0xFFFF to slot+0.
// Increments the counter at slot+2 each frame.
// FUNCTION: FF7 0x004f773f
void __cdecl BattleFx_SpecialHeartCardDelayedImpactTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0x5A) {
        BattleFx_SpecialHeartCardSpawnBoneParticleEmitterWithSound(g_BattleSpecialHeartCardTargetActorIdx, g_BattleSpecialHeartCardActorIndex);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

// Calls BattleFx_SpecialHeartCardSpawnBoneParticleEmitter with both arguments, then dispatches sound opcode 0x20 with pan computed from
// g_BattleSpecialHeartCardTargetMask and sound 6.
// FUNCTION: FF7 0x004f77a3
void __cdecl BattleFx_SpecialHeartCardSpawnBoneParticleEmitterWithSound(int targetActorIdx, int actorIdx)
{
    BattleFx_SpecialHeartCardSpawnBoneParticleEmitter(targetActorIdx, actorIdx);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(g_BattleSpecialHeartCardTargetMask), 6);
}

// Allocates a BattleFx_SpecialHeartCardBoneParticleEmitterTask slot for targetActorIdx, stored as
// targetActor. It clears boneIndex and cellRow, sets cellWord to 0x7807, and sets spriteFlags to 0x3A.
// FUNCTION: FF7 0x004f77d4
void __cdecl BattleFx_SpecialHeartCardSpawnBoneParticleEmitter(int targetActorIdx, int actorIdx)
{
    BattleFxHeartCardBoneEmitter *slot;

    slot = (BattleFxHeartCardBoneEmitter *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHeartCardBoneParticleEmitterTask);
    slot->actorIndex = targetActorIdx;
    slot->boneIndex = 0;
    slot->cellRow = 0;
    slot->cellWord = 0x7807;
    slot->spriteFlags = 0x3A;
}

// When unpaused emits two BattleFx_SpecialHeartCardBoneParticleSpriteTask slots at actor bones selected by advancing the parent bone
// index by random one through four modulo the actor bone-count short, without a zero-divisor
// guard. Jitters XYZ by -250 through 249, copies three control fields and assigns scale 0x1000
// through 0x1FFF; advances the parent counter and at thirty or above allocates its actor typed
// record and retires.
// FUNCTION: FF7 0x004f7824
void __cdecl BattleFx_SpecialHeartCardBoneParticleEmitterTask(void)
{
    BattleFxHeartBoneEmitter *particle;
    int i;
    int unusedWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxHeartCardBoneEmitter *pEmitter;

    pEmitter = (BattleFxHeartCardBoneEmitter *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused == 0) {
        for (i = 0; i < 2; i++) {
            pEmitter->boneIndex = (pEmitter->boneIndex + (rand() & 3) + 1) % g_BattleActorRecords[pEmitter->actorIndex].boneCount;
            particle = (BattleFxHeartBoneEmitter *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHeartCardBoneParticleSpriteTask);
            Battle_GetBoneWorldPos(pEmitter->actorIndex, pEmitter->boneIndex, &particle->posX);
            particle->posX += (short)(rand() % 0x1F4 - 0xFA);
            particle->posY += (short)(rand() % 0x1F4 - 0xFA);
            particle->posZ += (short)(rand() % 0x1F4 - 0xFA);
            particle->cellRow = pEmitter->cellRow;
            particle->cellWord = pEmitter->cellWord;
            particle->scale = (rand() & 0xFFF) + 0x1000;
            particle->spriteFlags = pEmitter->spriteFlags;
        }
        (pEmitter->timer)++;
        if (pEmitter->timer >= 0x1E) {
            Battle_PostActorHitOrHurtMessage(pEmitter->actorIndex);
            pEmitter->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

// Acquires data block zero, installs the slot billboard with its scale and offset -500, fills a
// sprite-quad request using counter-indexed g_BattleSpecialHeartCardAlphaTable (0x857BB0) data and
// slot controls, and submits while chaining g_BattleFxActiveDrawListPtr before checking pause. When
// unpaused advances and retires at eight when slot byte +0xD is 0x80, otherwise nine.
// FUNCTION: FF7 0x004f79bf
void __cdecl BattleFx_SpecialHeartCardBoneParticleSpriteTask(void)
{
    BattleFxSpriteQuadRequest *request;
    BattleFxHeartCardBoneParticleSprite *task;

    task = (BattleFxHeartCardBoneParticleSprite *)BATTLE_CURRENT_TASK_SLOT;
    request = (BattleFxSpriteQuadRequest *)Gfx_GetPsxScratchpadPtr(0);
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->scale, 0xFFFFFE0C);
    request->offsetY = 0xFFF0;
    request->offsetX = 0xFFF0;
    request->cellX = (char)g_BattleSpecialHeartCardAlphaTable[task->alphaIndex];
    request->cellY = task->cellRow;
    request->widthUnits = 0x1F;
    request->heightUnits = 0x1F;
    request->flags = task->flags;
    request->cellWord = task->cellWord;
    *(int *)&request->colorR = 0x2E808080;
    request->quadState = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteQuadDraw(request, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (g_BattleIsPaused == 0) {
        (task->alphaIndex)++;
        if (task->alphaIndex >= (*(unsigned char *)&task->cellRow != 0x80) + 8) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
