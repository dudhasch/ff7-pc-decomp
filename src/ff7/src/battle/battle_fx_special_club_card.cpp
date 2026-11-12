// src/ff7/src/battle/battle_fx_special_club_card.cpp
/* Enemy Special Effect [idx 48]: ClubCard (JP: club1 / クラブ1 (Club 1), asset: "SPECIAL/club1") — Club (0x155, Joker) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the first club attack. It loads its resources, starts the
//   effect, and runs the timeline, delayed impact, pulsing model, spinning, oscillating, and
//   accelerating model tasks.

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
extern "C" {

// clang-format off
// GLOBAL: FF7 0x0083bc78
unsigned int g_BattleSpecialClubCardModelResourceTable[15] = {0x001900b4, (unsigned int)&g_BattleFxClubCardCard1ModelNames, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)(g_BattleFxClubCardCard1ModelNames + 12), 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)&g_BattleFxClubCardQueikModelName, 0x00000000, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x0083bcb8
struct VECTOR g_BattleSpecialClubCardForwardAxis = {0, 0, -0x1000, 0};
// GLOBAL: FF7 0x0083bcc8
struct VECTOR g_BattleFxClubCardRightAxisFixed12 = {0x1000, 0, 0, 0};
// GLOBAL: FF7 0x0083bcd8
unsigned char g_BattleFxClubCardCard1ModelNames[22] =
    "CARD1.RSD\0\0\0"
    "CARD1.RSD";
// GLOBAL: FF7 0x0083bcf0
unsigned char g_BattleFxClubCardQueikModelName[10] = "QUEIK.RSD";
// GLOBAL: FF7 0x00b498c8
int g_BattleFxClubCardTargetActorIdx = 0;
unsigned char g_bssPad_00b498cc[4] = {0};
// GLOBAL: FF7 0x00b498d0
VECTOR g_BattleFxClubCardTransformedOffsetVector = {0};
// clang-format on
typedef struct {
    short x;
    short y;
    short z;
    short yaw;
} BattleClubCardBonePos;
// clang-format off
// GLOBAL: FF7 0x00b498e0
BattleClubCardBonePos g_BattleFxClubCardActorBoneWorldPos = {0};
// GLOBAL: FF7 0x00b498e8
VECTOR g_BattleFxClubCardTransformedForwardVector = {0};
// GLOBAL: FF7 0x00b498f8
int g_BattleFxClubCardTargetMask = 0;
// GLOBAL: FF7 0x00b498fc
int g_BattleFxClubCardActorIndex = 0;
// GLOBAL: FF7 0x00b49900
struct GfxTableCursor *g_BattleFxClubCardLoadedResTable = 0;
unsigned char g_bssPad_00b49904[4] = {0};
// clang-format on
#pragma function(memcpy)
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

// If the battle context exists and the club1 resource handle is zero, initializes a default-
// duration record, builds the SPECIAL/club1 resource path and requests resource type 3 using the
// context field at offset 0x4C. The record is represented by a scalar stack local with subsequent
// offset writes, so its apparent extent does not establish a safe source-level record allocation.
// FUNCTION: FF7 0x004f4370
void __cdecl BattleFx_SpecialLoadResClubCard(void)
{
    GfxModelTableBank *context;
    GfxResourceRequest requestRecord;
    int kind;
    char pathOut[200];
    GfxResourceRequest *pReq;
    int resultDefEntry;

    if (g_pBattle3dScene != 0 && g_BattleFxClubCardLoadedResTable == 0) {
        kind = 3;
        resultDefEntry = (int)&g_BattleSpecialClubCardModelResourceTable;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestRecord;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x0083bcfc
        pReq->resourcePath = (char *)BattleFx_BuildResourcePath("SPECIAL/club1", pathOut);
        Gfx_LoadModelTableIfAbsent(kind, resultDefEntry, pReq, &g_BattleFxClubCardLoadedResTable, context);
    }
}

// Forwarder: calls BattleFx_SpecialLoadResClubCard() with no arguments, then BattleFx_SpecialBeginClubCard(targetMask, actorIdx).
// FUNCTION: FF7 0x004f4400
void __cdecl BattleFx_SpecialEnterClubCard(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResClubCard();
    BattleFx_SpecialBeginClubCard(targetMask, actorIdx);
}

// Allocates and saves a draw-list-selection slot, stores targetMask as the actor mask and records its
// first selected low-ten-bit actor when one exists, retaining the previous actor value when none
// exists. Stores actorIdx and allocates BattleFx_SpecialClubCardSpawnTimeline.
// FUNCTION: FF7 0x004f441a
void __cdecl BattleFx_SpecialBeginClubCard(int targetMask, int actorIdx)
{
    int bitIdx;
    int selectedCount;

    g_BattleFxDrawListBufferTaskSlotPtr = (unsigned char *)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    g_BattleFxClubCardTargetMask = targetMask;
    bitIdx = 0;
    for (selectedCount = 0; bitIdx < 0xA; bitIdx++) {
        if ((targetMask & 1 << bitIdx) != 0) {
            selectedCount++;
            if (selectedCount == 1) {
                g_BattleFxClubCardTargetActorIdx = bitIdx;
            }
        }
    }
    g_BattleFxClubCardActorIndex = actorIdx;
    Battle_AllocateResourceSlot(BattleFx_SpecialClubCardSpawnTimeline);
}

// Retires the current slot, gets the selected actor bone position into a local eight-byte pair and
// copies that pair to +4 of a new BattleFx_SpecialClubCardTimelineTask slot. Calls BattleFx_SpecialClubCardBuildFacingVectors, then allocates
// BattleFx_SpecialClubCardDelayedImpactTask with the same pair; no pause guard appears.
// FUNCTION: FF7 0x004f44ab
void __cdecl BattleFx_SpecialClubCardSpawnTimeline(void)
{
    struct {
        int *slot;
        BattleBonePosCopy pos;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = (int *)BATTLE_CURRENT_TASK_SLOT;
    *(short *)frame.slot = 0xffff;
    Battle_GetBoneWorldPos(g_BattleFxClubCardTargetActorIdx,
                           g_BattleActorRecords[g_BattleFxClubCardTargetActorIdx].targetBone,
                           (short *)&frame.pos);
    frame.slot = (int *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialClubCardTimelineTask);
    *(BattleBonePosCopy *)(frame.slot + 1) = frame.pos;
    BattleFx_SpecialClubCardBuildFacingVectors();
    frame.slot = (int *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialClubCardDelayedImpactTask);
    *(BattleBonePosCopy *)(frame.slot + 1) = frame.pos;
}

// Runs an unpaused sequence that creates a positioned callback and positional sound at age 2, a
// vertically moving callback and centered sound at age 24, then at age 72 samples the target
// attachment and creates an axis-angle-oriented moving callback toward its X/Z position with
// positional sound. Uses the normalized distance square root divided by 12.
// Retires at age 90 before incrementing age.
// FUNCTION: FF7 0x004f454f
void __cdecl BattleFx_SpecialClubCardTimelineTask(void)
{
    struct {
        BattleFxStarCardSlot *slot;
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
        frame.slot = (BattleFxStarCardSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialClubCardSpinningModelTask);
        *(Int64Pair *)((char *)frame.slot + 4) = *(Int64Pair *)&g_BattleFxClubCardActorBoneWorldPos;
        frame.distance = g_BattleActorRecords[g_BattleFxClubCardActorIndex].posY - g_BattleFxClubCardActorBoneWorldPos.y;
        if (frame.distance < 0) {
            frame.distance *= -1;
        }
        frame.slot->distance = frame.distance;
        frame.slot->velocityY = 0xFED4;
        frame.slot->scale = 0x2000 /* 2.0 */;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleFxClubCardActorBoneWorldPos), 0x1D5);
    }
    if (frame.current->step == 24) {
        frame.slot = (BattleFxStarCardSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialClubCardOscillatingModelTask);
        frame.slot->offsetY = 0xFEF6;
        frame.slot->velocityY = 33;
        frame.slot->angle = 0;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, 0x40, 0x1D6);
    }
    if (frame.current->step == 72) {
        Battle_GetBoneWorldPos(g_BattleFxClubCardTargetActorIdx, g_BattleActorRecords[g_BattleFxClubCardTargetActorIdx].targetBone, (short *)((int)frame.current + 4));
        frame.vector[0] = frame.current->posX - g_BattleCameraPosition.cameraPosX;
        frame.vector[1] = 0;
        frame.vector[2] = frame.current->posZ - g_BattleCameraPosZ;
        frame.distance = NormalizeVectorFixed(&frame.vector[0], &frame.vector[0]);
        frame.distance = SqrtToInt(frame.distance) / 12;
        frame.angle = ComputeAngleBetweenVectors(&g_BattleSpecialClubCardForwardAxis, (VECTOR *)(&frame.vector[0]), (VECTOR *)(&frame.axis[0]));
        Gte_BuildAxisAngleMatrix(frame.angle, (PsxMatrix *)(&frame.matrix[0]), (VECTOR *)(&frame.axis[0]));
        TransformVectorToInt32((PsxMatrix *)(&frame.matrix[0]), &g_BattleSpecialClubCardForwardAxis, &frame.vector[0]);
        if (frame.axis[1] < 0) {
            frame.angle *= -1;
        }
        frame.slot = (BattleFxStarCardSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialClubCardAcceleratingModelTask);
        frame.slot->posX = g_BattleCameraPosition.cameraPosX;
        frame.slot->offsetY = frame.current->posY;
        frame.slot->posZ = g_BattleCameraPosZ;
        frame.slot->offsetX = frame.distance * frame.vector[0] >> 12;
        frame.slot->velocityY = 0;
        frame.slot->offsetZ = frame.distance * frame.vector[2] >> 12;
        frame.slot->angle = frame.angle;
        frame.slot->scale = 0x2666;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition((char *)frame.current + 4), 0x1D7);
    }
    if (frame.current->step >= 90) {
        frame.current->ownerSlot = 0xFFFF;
    }
    (frame.current->step)++;
}

// Draws a uniformly scaled club1 model using the stored actor heading, an age-driven axis rotation
// and a rotated vertical offset; the angle advances through age 8 then remains a quarter turn.
// Ages at least 18 retire before checking pause. From unpaused age 8 onward, adds vertical
// velocity to Y and increases that velocity by its sixth, then increments age.
// FUNCTION: FF7 0x004f4820
void __cdecl BattleFx_SpecialClubCardSpinningModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        BattleFxDiamondCardSpinRecord *spinRecord;
        struct { unsigned char pad_to_4[0x4]; SVECTOR secondRotX; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offsetX; };
        struct { unsigned char pad_to_18[0x18]; int spinAngle; };
        struct { unsigned char pad_to_1C[0x1C]; PsxMatrix axisMatrix; };
        struct { unsigned char pad_to_3C[0x3C]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_5C[0x5C]; SVECTOR rotX; };
        struct { unsigned char pad_to_64[0x64]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_68[0x68]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.spinRecord = (BattleFxDiamondCardSpinRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.spinRecord->step <= 8) {
        frame.spinAngle = frame.spinRecord->step << 7;
    } else {
        frame.spinAngle = 0x400 /* 90 deg */;
    }
    frame.rotX.vx = 0;
    frame.rotX.vy = (g_BattleFxClubCardActorBoneWorldPos.yaw);
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    Gte_BuildAxisAngleMatrix(-frame.spinAngle, &frame.axisMatrix, &g_BattleFxClubCardRightAxisFixed12);
    ConcatMatrixInPlace(&frame.rotMatrix, &frame.axisMatrix);
    frame.offsetX.vx = 0;
    frame.offsetX.vy = frame.spinRecord->offsetDistance;
    frame.offsetX.vz = 0;
    TransformVectorToInt16(&frame.axisMatrix, &frame.offsetX, &frame.offsetX.vx);
    frame.secondRotX.vx = 0;
    frame.secondRotX.vy = 0;
    frame.secondRotX.vz = 0x800 /* 180 deg */;
    RotMatrixXYZ(&frame.secondRotX, &frame.rotMatrix);
    ConcatMatrixInPlace(&frame.axisMatrix, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.spinRecord->posX + frame.offsetX.vx;
    frame.rotMatrix.t[1] = frame.spinRecord->posY + frame.offsetX.vy;
    frame.rotMatrix.t[2] = frame.spinRecord->posZ + frame.offsetX.vz;
    frame.scale.vz = frame.spinRecord->scale;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleFxClubCardLoadedResTable);
    frame.modelDraw->flags = 0;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 128;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.spinRecord->step >= 18) {
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

// Draws the club1 resource model at fixed X/Z and slot Y minus 40 with a quarter-turn X rotation
// and configurable Y rotation. Retires at age 48 before checking pause. Unpaused ages below 8 move
// Y with damped velocity clamped to at least 1; ages 8 through 39 set Y to a sine oscillation and
// rotate during ages 22 through 33; later ages move Y with velocity increased by a quarter.
// FUNCTION: FF7 0x004f4a75
void __cdecl BattleFx_SpecialClubCardOscillatingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        BattleFxOscillatingModel *oscState;
        struct { unsigned char pad_to_4[0x4]; SVECTOR fixedRotX; };
        struct { unsigned char pad_to_18[0x18]; int sineAngle; };
        struct { unsigned char pad_to_1C[0x1C]; PsxMatrix yawMatrix; };
        struct { unsigned char pad_to_3C[0x3C]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_5C[0x5C]; SVECTOR rotX; };
        struct { unsigned char pad_to_64[0x64]; BattleFxModelDraw *modelDraw; };
    } frame;
#pragma pack(pop)

    frame.oscState = (BattleFxOscillatingModel *)BATTLE_CURRENT_TASK_SLOT;
    frame.fixedRotX.vx = 0x400 /* 90 deg */;
    frame.fixedRotX.vy = 0;
    frame.fixedRotX.vz = 0;
    RotMatrixXYZ(&frame.fixedRotX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = 0;
    frame.rotMatrix.t[1] = frame.oscState->offset - 0x28;
    frame.rotMatrix.t[2] = 0x380;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.oscState->angle;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.yawMatrix);
    ConcatMatrixInPlace(&frame.yawMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleFxClubCardLoadedResTable);
    frame.modelDraw->flags = 0;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 128;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.oscState->phase >= 48) {
        frame.oscState->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.oscState->phase < 8) {
        frame.oscState->offset += frame.oscState->velocity;
        frame.oscState->velocity -= (short)(frame.oscState->velocity >> 5);
        if (frame.oscState->velocity < 2) {
            frame.oscState->velocity = 1;
        }
    } else if (frame.oscState->phase < 40) {
        frame.sineAngle = frame.oscState->phase - 8 << 7;
        frame.oscState->offset = SinFixed(frame.sineAngle) * 0x18 >> 12;
        if (frame.oscState->phase >= 22 && frame.oscState->phase < 34) {
            frame.oscState->angle -= 0xAA;
        }
    } else {
        frame.oscState->offset += frame.oscState->velocity;
        frame.oscState->velocity += (short)(frame.oscState->velocity >> 2);
    }
    (frame.oscState->phase)++;
}

// Draws a uniformly scaled club1 model at the slot position with configured Y rotation and half-
// turn Z rotation; from age 12 switches to model 1, enables flag 8 and reduces intensity by 16 per
// age. Retires at age 20 before checking pause. Unpaused age 10 triggers a target operation
// selected by screen pan above 64 and positional sound; unpaused calls advance X/Z and increase
// their velocities by a sixteenth before advancing age.
// FUNCTION: FF7 0x004f4cb0
void __cdecl BattleFx_SpecialClubCardAcceleratingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        BattleFxSpecialAcceleratingSlot *accelState;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_18[0x18]; int screenPan; };
        struct { unsigned char pad_to_3C[0x3C]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_64[0x64]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_68[0x68]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.accelState = (BattleFxSpecialAcceleratingSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.accelState->angleY;
    frame.rotX.vz = 0x800 /* 180 deg */;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.accelState->posA;
    frame.rotMatrix.t[1] = frame.accelState->posY;
    frame.rotMatrix.t[2] = frame.accelState->posB;
    frame.scale.vz = frame.accelState->scale;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleFxClubCardLoadedResTable);
    frame.modelDraw->flags = 0;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 128;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    if (frame.accelState->timer >= 12) {
        frame.modelDraw->flags = 8;
        frame.modelDraw->fadeLevel = 128 - (frame.accelState->timer - 12 << 4);
        frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleFxClubCardLoadedResTable);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.accelState->timer >= 20) {
        frame.accelState->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.accelState->timer == 10) {
        frame.screenPan = Battle_ComputeScreenPanFromPosition((char *)frame.accelState + 4);
        if (frame.screenPan > 64) {
            BattleFx_HitMarkSpawnAtActorBone(g_BattleFxClubCardTargetActorIdx, 0x118);
        } else {
            BattleFx_HitMarkSpawnAtActorBone(g_BattleFxClubCardTargetActorIdx, 0x18);
        }
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, frame.screenPan, 0x1D8);
    }
    frame.accelState->posA += frame.accelState->velocityA;
    frame.accelState->posB += frame.accelState->velocityB;
    frame.accelState->velocityA += (short)(frame.accelState->velocityA >> 4);
    frame.accelState->velocityB += (short)(frame.accelState->velocityB >> 4);
    (frame.accelState->timer)++;
}

// Gets the g_BattleFxClubCardActorIndex actor bone selected by its +0xBE11AD byte into
// g_BattleFxClubCardActorBoneWorldPos, lowers Y by 224 and stores actor facing at +6. Builds a Y
// rotation from that facing and transforms fixed vectors at g_BattleSpecialClubCardForwardAxis
// (0x83BCB8) and g_BattleFxClubCardRightAxisFixed12 (0x83BCC8) into
// g_BattleFxClubCardTransformedForwardVector (0xB498E8) and g_BattleFxClubCardTransformedOffsetVector
// (0xB498D0).
// FUNCTION: FF7 0x004f4f12
void __cdecl BattleFx_SpecialClubCardBuildFacingVectors(void)
{
    struct {
        int matrix[8];
        short rot[4];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    Battle_GetBoneWorldPos(g_BattleFxClubCardActorIndex,
                           g_BattleActorRecords[g_BattleFxClubCardActorIndex].altBone,
                           &g_BattleFxClubCardActorBoneWorldPos.x);
    g_BattleFxClubCardActorBoneWorldPos.y -= 224;
    g_BattleFxClubCardActorBoneWorldPos.yaw = g_BattleActorRecords[g_BattleFxClubCardActorIndex].yaw;
    frame.rot[0] = 0;
    frame.rot[1] = g_BattleFxClubCardActorBoneWorldPos.yaw;
    frame.rot[2] = 0;
    RotMatrixXYZ((void *)frame.rot, (void *)frame.matrix);
    TransformVectorToInt32((PsxMatrix *)frame.matrix, &g_BattleSpecialClubCardForwardAxis, &g_BattleFxClubCardTransformedForwardVector);
    TransformVectorToInt32((PsxMatrix *)frame.matrix, &g_BattleFxClubCardRightAxisFixed12, &g_BattleFxClubCardTransformedOffsetVector);
}

// One frame of a battle effect step. Takes the 0x20-byte slot at &g_BattleHitLivewireEmitterRecords + (g_BattleActiveTaskSlotIdx <<
// 5), returns doing nothing while g_BattleIsPaused is nonzero, and when the frame counter at slot+2
// equals 0x5A calls BattleFx_SpecialClubCardSpawnPulsingModelWithSound(g_BattleFxClubCardTargetActorIdx, g_BattleFxClubCardActorIndex) and writes 0xFFFF to slot+0 to retire
// the slot. Increments the counter at slot+2 on the way out.
// FUNCTION: FF7 0x004f4fbf
void __cdecl BattleFx_SpecialClubCardDelayedImpactTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 90) {
        BattleFx_SpecialClubCardSpawnPulsingModelWithSound(g_BattleFxClubCardTargetActorIdx, g_BattleFxClubCardActorIndex);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

// Calls BattleFx_SpecialClubCardSpawnPulsingModel with both arguments, then dispatches sound opcode 0x20 with pan computed from
// g_BattleFxClubCardTargetMask and sound 0x47.
// FUNCTION: FF7 0x004f5023
void __cdecl BattleFx_SpecialClubCardSpawnPulsingModelWithSound(int targetActorIdx, int actorIdx)
{
    BattleFx_SpecialClubCardSpawnPulsingModel(targetActorIdx, actorIdx);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(g_BattleFxClubCardTargetMask), 0x47);
}

// Allocates a slot for BattleFx_SpecialClubCardPulsingModelTask at the actor's target bone position,
// stored from +4. It clears posY and extent, sets radius to the actor's radius plus 0xE0 capped at
// 0x76C, then divides it by eight, and stores targetActorIdx as targetIndex.
// FUNCTION: FF7 0x004f5054
void __cdecl BattleFx_SpecialClubCardSpawnPulsingModel(int targetActorIdx, int actorIdx)
{
    BattleFxClubCardPulsingModel *slot;

    slot = (BattleFxClubCardPulsingModel *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialClubCardPulsingModelTask);
    Battle_GetBoneWorldPos(targetActorIdx, g_BattleActorRecords[targetActorIdx].targetBone, (short *)((int)slot + 4));
    slot->posY = 0;
    slot->extent = 0;
    slot->radius = g_BattleActorRecords[targetActorIdx].radius + 224;
    if (slot->radius > 1900) {
        slot->radius = 1900;
    }
    slot->radius = slot->radius / 8;
    slot->targetIndex = targetActorIdx;
}

// Draws club1 model 2 at the slot position with uniform scale derived from the stored extent times
// 4096 divided by 600. Retires at age 22 before checking pause. Unpaused age 14 allocates a typed
// record for the stored actor; ages below 8 grow the extent, ages 8 through 13 hold it, and ages
// at least 14 shrink it before advancing age.
// FUNCTION: FF7 0x004f5104
void __cdecl BattleFx_SpecialClubCardPulsingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[68];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxGrowShrinkRecord *pulseRecord; };
        struct { unsigned char pad_to_C[0xC]; int scaleValue; };
        struct { unsigned char pad_to_10[0x10]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_30[0x30]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_34[0x34]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.pulseRecord = (BattleFxGrowShrinkRecord *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.pulseRecord->posX;
    frame.rotMatrix.t[1] = frame.pulseRecord->posY;
    frame.rotMatrix.t[2] = frame.pulseRecord->posZ;
    frame.scaleValue = (frame.pulseRecord->value << 12) / 600;
    frame.scale.vz = frame.scaleValue;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(2, g_BattleFxClubCardLoadedResTable);
    frame.modelDraw->flags = 0;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 128;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.pulseRecord->stage >= 22) {
        frame.pulseRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.pulseRecord->stage == 14) {
        Battle_PostActorHitOrHurtMessage(frame.pulseRecord->recordType);
    }
    if (frame.pulseRecord->stage < 8) {
        frame.pulseRecord->value += frame.pulseRecord->step;
    } else if (frame.pulseRecord->stage >= 14) {
        frame.pulseRecord->value -= frame.pulseRecord->step;
    }
    (frame.pulseRecord->stage)++;
}
} /* extern "C" */
