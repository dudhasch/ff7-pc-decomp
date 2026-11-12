// src/ff7/src/battle/battle_fx_special_emperor_card.cpp
/* Enemy Special Effect [idx 51]: EmperorCard (JP: spade2 / スペード2 (Spade 2), asset: "SPECIAL/SPADE2") — Emperor (0x1f6, Death Dealer) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the second spade attack. It loads the spade resources, starts
//   the effect, and runs the timeline, model, and bone-particle emitter tasks.

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
// GLOBAL: FF7 0x00857be8
int g_BattleSpecialEmperorCardResourceDefs[6] = {1638582, (int)((char *)&g_BattleSpecialEmperorCardResourceNames + 0x4), 0, 0, 0, 0};
// GLOBAL: FF7 0x00857c00
struct VECTOR g_BattleSpecialEmperorCardForwardAxisFixed12 = {0, 0, -0x1000, 0};
// GLOBAL: FF7 0x00857c10
struct VECTOR g_BattleSpecialEmperorCardRightAxisFixed12 = {0x1000, 0, 0, 0};
// g_BattleSpecialEmperorCardAlphaTable is an 8-byte table the code indexes
// (BattleFx_SpecialEmperorCardBoneParticleSpriteTask (0x004f89a9), unbounded); its last four bytes were
// the fill g_rawData_00857c24. The strings after it stay a fill: the TU's pointer words reach them.
// GLOBAL: FF7 0x00857c20
unsigned char g_BattleSpecialEmperorCardAlphaTable[0x8] = {0x00, 0x20, 0x40, 0x60, 0x80, 0xa0, 0xc0, 0xe0};
unsigned char g_BattleSpecialEmperorCardResourceNames[16] = {0x00, 0x00, 0x00, 0x00, 0x63, 0x61, 0x72, 0x64, 0x31, 0x2e, 0x72, 0x73, 0x64, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00b49d70
int g_BattleSpecialEmperorCardTargetActorIdx = 0;
unsigned char g_bssPad_00b49d74[4] = {0};
// GLOBAL: FF7 0x00b49d78
VECTOR g_BattleSpecialEmperorCardTransformedOffsetVector = {0};
// GLOBAL: FF7 0x00b49d88
struct Int64Pair g_BattleSpecialEmperorCardActorBoneWorldPos = {0};
// GLOBAL: FF7 0x00b49d90
VECTOR g_BattleSpecialEmperorCardTransformedForwardVector = {0};
// GLOBAL: FF7 0x00b49da0
int g_BattleSpecialEmperorCardTargetMask = 0;
// GLOBAL: FF7 0x00b49da4
int g_BattleSpecialEmperorCardActorIndex = 0;
// GLOBAL: FF7 0x00b49da8
struct GfxTableCursor *g_BattleSpecialEmperorCardLoadedResTable = 0;
unsigned char g_bssPad_00b49dac[4] = {0};
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

// If the battle context exists and the SPADE2 resource handle is zero, initializes a default-
// duration record, builds the SPECIAL/SPADE2 resource path and requests resource type 1 using the
// context field at offset 0x4C. The record is represented by a scalar stack local with subsequent
// offset writes, so its apparent extent does not establish a safe source-level record allocation.
// FUNCTION: FF7 0x004f7af0
void __cdecl BattleFx_SpecialLoadResEmperorCard(void)
{
    GfxModelTableBank *context;
    GfxResourceRequest requestRecord;
    int kind;
    char pathOut[200];
    GfxResourceRequest *pReq;
    int resultDefEntry;

    if (g_pBattle3dScene != 0 && g_BattleSpecialEmperorCardLoadedResTable == 0) {
        kind = 1;
        resultDefEntry = (int)&g_BattleSpecialEmperorCardResourceDefs;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestRecord;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x00857c38
        pReq->resourcePath = BattleFx_BuildResourcePath("SPECIAL/SPADE2", pathOut);
        Gfx_LoadModelTableIfAbsent(kind, resultDefEntry, pReq, &g_BattleSpecialEmperorCardLoadedResTable, context);
    }
}

// Mechanical forwarder: calls BattleFx_SpecialBeginEmperorCard(targetMask, actorIdx) and returns.
// FUNCTION: FF7 0x004f7b80
void __cdecl BattleFx_SpecialEnterEmperorCard(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginEmperorCard(targetMask, actorIdx);
}

// Allocates and saves a draw-list-selection slot and calls BattleFx_SpecialLoadResEmperorCard. Stores targetMask as the actor
// mask and records its first selected low-ten-bit actor when one exists, retaining the previous
// actor value when none exists; stores actorIdx and allocates BattleFx_SpecialEmperorCardSpawnTimeline.
// FUNCTION: FF7 0x004f7b95
void __cdecl BattleFx_SpecialBeginEmperorCard(int targetMask, int actorIdx)
{
    int bitIdx;
    int selectedCount;

    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    BattleFx_SpecialLoadResEmperorCard();
    g_BattleSpecialEmperorCardTargetMask = targetMask;
    bitIdx = 0;
    for (selectedCount = 0; bitIdx < 0xA; bitIdx++) {
        if ((targetMask & 1 << bitIdx) != 0) {
            selectedCount++;
            if (selectedCount == 1) {
                g_BattleSpecialEmperorCardTargetActorIdx = bitIdx;
            }
        }
    }
    g_BattleSpecialEmperorCardActorIndex = actorIdx;
    Battle_AllocateResourceSlot(BattleFx_SpecialEmperorCardSpawnTimeline);
}

// Retires the current slot, gets the selected actor bone position into a local eight-byte pair and
// copies that pair to the position of a new BattleFx_SpecialEmperorCardTimelineTask slot. Calls BattleFx_SpecialEmperorCardBuildFacingVectors, then allocates
// BattleFx_SpecialEmperorCardDelayedImpactTask with the same pair; no pause guard appears.
// FUNCTION: FF7 0x004f7c2b
void __cdecl BattleFx_SpecialEmperorCardSpawnTimeline(void)
{
    struct {
        int *slot;
        BattleBonePosCopy pos;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = (int *)BATTLE_CURRENT_TASK_SLOT;
    *(short *)frame.slot = 0xffff;
    Battle_GetBoneWorldPos(g_BattleSpecialEmperorCardTargetActorIdx,
                           g_BattleActorRecords[g_BattleSpecialEmperorCardTargetActorIdx].targetBone,
                           (short *)&frame.pos);
    frame.slot = (int *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEmperorCardTimelineTask);
    *(BattleBonePosCopy *)(frame.slot + 1) = frame.pos;
    BattleFx_SpecialEmperorCardBuildFacingVectors();
    frame.slot = (int *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEmperorCardDelayedImpactTask);
    *(BattleBonePosCopy *)(frame.slot + 1) = frame.pos;
}

// Runs an unpaused sequence that creates a positioned callback and positional sound at age 2, a
// vertically moving callback and centered sound at age 24, then at age 72 samples the target
// attachment and creates an axis-angle-oriented moving callback toward its X/Z position with
// positional sound. Uses the normalized distance square root divided by 12.
// Retires at age 90 before incrementing age.
// FUNCTION: FF7 0x004f7ccf
void __cdecl BattleFx_SpecialEmperorCardTimelineTask(void)
{
    struct {
        BattleFxDiamondCardSlot *slot;
        short matrix[16];
        BattleFxEmperorCardRecord *current;
        int vector[3];
        unsigned char reserved0[8];
        int angle;
        int distance;
        unsigned char reserved1[8];
        int axis[3];
        unsigned char reserved2[4];
    } frame;

    frame.current = (BattleFxEmperorCardRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.current->stage == 2) {
        frame.slot = (BattleFxDiamondCardSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEmperorCardSpinningModelTask);
        *(Int64Pair *)((char *)frame.slot + 4) = *(Int64Pair *)&g_BattleSpecialEmperorCardActorBoneWorldPos;
        frame.distance = g_BattleActorRecords[g_BattleSpecialEmperorCardActorIndex].posY - ((short *)&g_BattleSpecialEmperorCardActorBoneWorldPos)[1];
        if (frame.distance < 0) {
            frame.distance *= -1;
        }
        frame.slot->distance = frame.distance;
        frame.slot->velocityY = 0xFED4;
        frame.slot->scale = 0x2000;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(((short *)&g_BattleSpecialEmperorCardActorBoneWorldPos)), 0x1D5);
    }
    if (frame.current->stage == 0x18) {
        frame.slot = (BattleFxDiamondCardSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEmperorCardOscillatingModelTask);
        frame.slot->posY = 0xFEF6;
        frame.slot->velocityY = 0x21;
        frame.slot->angle = 0;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, 0x40, 0x1D6);
    }
    if (frame.current->stage == 0x48) {
        Battle_GetBoneWorldPos(g_BattleSpecialEmperorCardTargetActorIdx, g_BattleActorRecords[g_BattleSpecialEmperorCardTargetActorIdx].targetBone, &frame.current->posX);
        frame.vector[0] = frame.current->posX - g_BattleCameraPosition.cameraPosX;
        frame.vector[1] = 0;
        frame.vector[2] = frame.current->posZ - g_BattleCameraPosZ;
        frame.distance = NormalizeVectorFixed(&frame.vector[0], &frame.vector[0]);
        frame.distance = SqrtToInt(frame.distance) / 0xC;
        frame.angle = ComputeAngleBetweenVectors(&g_BattleSpecialEmperorCardForwardAxisFixed12, (VECTOR *)(&frame.vector[0]), (VECTOR *)(&frame.axis[0]));
        Gte_BuildAxisAngleMatrix(frame.angle, (PsxMatrix *)(&frame.matrix[0]), (VECTOR *)(&frame.axis[0]));
        TransformVectorToInt32((PsxMatrix *)(&frame.matrix[0]), &g_BattleSpecialEmperorCardForwardAxisFixed12, &frame.vector[0]);
        if (frame.axis[1] < 0) {
            frame.angle *= -1;
        }
        frame.slot = (BattleFxDiamondCardSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEmperorCardAcceleratingModelTask);
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
    if (frame.current->stage >= 0x5A) {
        frame.current->ownerSlot = 0xFFFF;
    }
    (frame.current->stage)++;
}

// Draws a uniformly scaled SPADE2 model using the stored actor heading, an age-driven axis
// rotation and a rotated vertical offset; the angle advances through age 8 then remains a quarter
// turn. Ages at least 18 retire before checking pause. From unpaused age 8 onward, adds vertical
// velocity to Y and increases that velocity by its sixth, then increments age.
// FUNCTION: FF7 0x004f7fa0
void __cdecl BattleFx_SpecialEmperorCardSpinningModelTask(void)
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
        struct { unsigned char pad_to_68[0x68]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.spinRecord = (BattleFxDiamondCardSpinRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.spinRecord->step <= 8) {
        frame.axisAngle = frame.spinRecord->step << 7;
    } else {
        frame.axisAngle = 0x400;
    }
    frame.actorRotX.vx = 0;
    frame.actorRotX.vy = ((short *)&g_BattleSpecialEmperorCardActorBoneWorldPos)[3];
    frame.actorRotX.vz = 0;
    RotMatrixXYZ(&frame.actorRotX, &frame.modelMatrix);
    Gte_BuildAxisAngleMatrix(-frame.axisAngle, &frame.axisMatrix, &g_BattleSpecialEmperorCardRightAxisFixed12);
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
    frame.scale.vz = frame.spinRecord->scale;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.modelMatrix, &frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialEmperorCardLoadedResTable);
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

// Draws the SPADE2 resource model at fixed X/Z and slot Y minus 40 with a quarter-turn X rotation
// and configurable Y rotation. Retires at age 48 before checking pause. Unpaused ages below 8 move
// Y with damped velocity clamped to at least 1; ages 8 through 39 set Y to a sine oscillation and
// rotate during ages 22 through 33; later ages move Y with velocity increased by a quarter.
// FUNCTION: FF7 0x004f81f5
void __cdecl BattleFx_SpecialEmperorCardOscillatingModelTask(void)
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
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialEmperorCardLoadedResTable);
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

// Draws a uniformly scaled SPADE2 model at the slot position with configured Y rotation and half-
// turn Z rotation, enabling flag 8 and reducing intensity by 16 per age from age 12. Retires at
// age 20 before checking pause. Unpaused age 10 triggers a target operation selected by screen pan
// above 64 and positional sound; unpaused calls advance X/Z and increase their velocities by a
// sixteenth before advancing age.
// FUNCTION: FF7 0x004f8430
void __cdecl BattleFx_SpecialEmperorCardAcceleratingModelTask(void)
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
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialEmperorCardLoadedResTable);
    frame.modelDraw->flags = 0;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0x80;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    if (frame.accelRecord->timer >= 0xC) {
        frame.modelDraw->flags = 8;
        frame.modelDraw->fadeLevel = 0x80 - (frame.accelRecord->timer - 0xC << 4);
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
            BattleFx_HitMarkSpawnAtActorBone(g_BattleSpecialEmperorCardTargetActorIdx, 0x118);
        } else {
            BattleFx_HitMarkSpawnAtActorBone(g_BattleSpecialEmperorCardTargetActorIdx, 0x18);
        }
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, frame.screenPan, 0x1D8);
    }
    frame.accelRecord->posA += frame.accelRecord->velocityA;
    frame.accelRecord->posB += frame.accelRecord->velocityB;
    frame.accelRecord->velocityA += (short)(frame.accelRecord->velocityA >> 4);
    frame.accelRecord->velocityB += (short)(frame.accelRecord->velocityB >> 4);
    (frame.accelRecord->timer)++;
}

// Gets the alternate bone position of the g_BattleSpecialEmperorCardActorIndex actor into
// g_BattleSpecialEmperorCardActorBoneWorldPos, lowers Y by 224 and stores the actor's yaw in its fourth
// short. Builds a Y rotation from that yaw and transforms g_BattleSpecialEmperorCardForwardAxisFixed12
// and g_BattleSpecialEmperorCardRightAxisFixed12 into g_BattleSpecialEmperorCardTransformedForwardVector and
// g_BattleSpecialEmperorCardTransformedOffsetVector.
// FUNCTION: FF7 0x004f867c
void __cdecl BattleFx_SpecialEmperorCardBuildFacingVectors(void)
{
    struct {
        int matrix[8];
        short rot[4];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    Battle_GetBoneWorldPos(g_BattleSpecialEmperorCardActorIndex,
                           g_BattleActorRecords[g_BattleSpecialEmperorCardActorIndex].altBone,
                           ((short *)&g_BattleSpecialEmperorCardActorBoneWorldPos));
    ((short *)&g_BattleSpecialEmperorCardActorBoneWorldPos)[1] -= 0xe0;
    ((short *)&g_BattleSpecialEmperorCardActorBoneWorldPos)[3] = g_BattleActorRecords[g_BattleSpecialEmperorCardActorIndex].yaw;
    frame.rot[0] = 0;
    frame.rot[1] = ((short *)&g_BattleSpecialEmperorCardActorBoneWorldPos)[3];
    frame.rot[2] = 0;
    RotMatrixXYZ((void *)frame.rot, (void *)frame.matrix);
    TransformVectorToInt32((PsxMatrix *)frame.matrix, &g_BattleSpecialEmperorCardForwardAxisFixed12, &g_BattleSpecialEmperorCardTransformedForwardVector);
    TransformVectorToInt32((PsxMatrix *)frame.matrix, &g_BattleSpecialEmperorCardRightAxisFixed12, &g_BattleSpecialEmperorCardTransformedOffsetVector);
}

// Delayed impact task, the same shape as BattleFx_SpecialClubCardDelayedImpactTask: it returns while
// g_BattleIsPaused is nonzero, and at frame counter 0x5A calls
// BattleFx_SpecialEmperorCardSpawnBoneParticleEmitterWithSound and frees its slot. Increments the frame
// counter each frame.
// FUNCTION: FF7 0x004f8729
void __cdecl BattleFx_SpecialEmperorCardDelayedImpactTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0x5A) {
        BattleFx_SpecialEmperorCardSpawnBoneParticleEmitterWithSound(g_BattleSpecialEmperorCardTargetActorIdx, g_BattleSpecialEmperorCardActorIndex);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

// Calls BattleFx_SpecialEmperorCardSpawnBoneParticleEmitter with both arguments, then dispatches sound opcode 0x20 with pan computed from
// g_BattleSpecialEmperorCardTargetMask and sound 8.
// FUNCTION: FF7 0x004f878d
void __cdecl BattleFx_SpecialEmperorCardSpawnBoneParticleEmitterWithSound(int targetActorIdx, int actorIdx)
{
    BattleFx_SpecialEmperorCardSpawnBoneParticleEmitter(targetActorIdx, actorIdx);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(g_BattleSpecialEmperorCardTargetMask), 8);
}

// Allocates a BattleFx_SpecialEmperorCardBoneParticleEmitterTask slot for targetActorIdx, stored as
// targetActor. It clears boneIndex, sets cellRow to 0x40, sets cellWord to 0x7947, and sets spriteFlags to 0x3A.
// FUNCTION: FF7 0x004f87be
void __cdecl BattleFx_SpecialEmperorCardSpawnBoneParticleEmitter(int targetActorIdx, int actorIdx)
{
    BattleHeartCardBoneEmitter *slot;

    slot = (BattleHeartCardBoneEmitter *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEmperorCardBoneParticleEmitterTask);
    slot->targetActor = targetActorIdx;
    slot->boneIndex = 0;
    slot->cellRow = 0x40;
    slot->cellWord = 0x7947;
    slot->spriteFlags = 0x3A;
}

// When unpaused emits two BattleFx_SpecialEmperorCardBoneParticleSpriteTask slots at actor bones selected by advancing the parent bone
// index by random one through four modulo the actor bone-count short, without a zero-divisor
// guard. Jitters XYZ by -250 through 249, copies three control fields and assigns scale 0x1000
// through 0x1FFF; advances the parent counter and at thirty or above allocates its actor typed
// record and retires.
// FUNCTION: FF7 0x004f880e
void __cdecl BattleFx_SpecialEmperorCardBoneParticleEmitterTask(void)
{
    BattleFxHeartBoneEmitter *particle;
    int i;
    int unusedWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxHeartCardBoneEmitter *pEmitter;

    pEmitter = (BattleFxHeartCardBoneEmitter *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused == 0) {
        for (i = 0; i < 2; i++) {
            pEmitter->boneIndex = (pEmitter->boneIndex + (rand() & 3) + 1) % g_BattleActorRecords[pEmitter->actorIndex].boneCount;
            particle = (BattleFxHeartBoneEmitter *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEmperorCardBoneParticleSpriteTask);
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
// sprite-quad request using counter-indexed g_BattleSpecialEmperorCardAlphaTable data and the slot's
// cell fields, and submits with draw-list argument zero before checking pause. When unpaused
// advances and retires at eight when cellY is 0x80, otherwise nine.
// FUNCTION: FF7 0x004f89a9
void __cdecl BattleFx_SpecialEmperorCardBoneParticleSpriteTask(void)
{
    BattleFxSpriteQuadRequest *request;
    BattleFxEmperorCardBoneParticleSprite *task;

    task = (BattleFxEmperorCardBoneParticleSprite *)BATTLE_CURRENT_TASK_SLOT;
    request = (BattleFxSpriteQuadRequest *)Gfx_GetPsxScratchpadPtr(0);
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->scale, 0xFFFFFE0C);
    request->offsetY = 0xFFF0;
    request->offsetX = 0xFFF0;
    request->cellX = *(char *)((char *)&g_BattleSpecialEmperorCardAlphaTable + task->alphaIndex);
    request->cellY = task->cellY;
    request->widthUnits = 0x1F;
    request->heightUnits = 0x1F;
    request->flags = task->flags;
    request->cellWord = task->cellWord;
    *(int *)((char *)request + 8) = 0x2E808080;
    request->quadState = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)request, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
    if (g_BattleIsPaused == 0) {
        (task->alphaIndex)++;
        if (task->alphaIndex >= ((unsigned char)task->cellY != 0x80) + 8) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
