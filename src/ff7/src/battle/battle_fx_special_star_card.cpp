// src/ff7/src/battle/battle_fx_special_star_card.cpp
/* Enemy Special Effect [idx 53]: StarCard (JP: club2 / クラブ2 (Club 2), asset: "SPECIAL/CLUB2") — Star (0x1f8, Death Dealer) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the second club attack. It loads the club resources, starts
//   the effect for each target, and runs its timeline, impact step, and the model and sprite tasks
//   that draw the club and its sprites.

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
// GLOBAL: FF7 0x0083b8b8
unsigned int g_BattleSpecialStarCardModelResourceTable[115] = {0x001900b6, (unsigned int)&g_BattleSpecialStarCardResFilenameCard, 0x00000000, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)(g_BattleSpecialStarCardResFilenameCard + 12), (unsigned int)&g_BattleSpecialStarCardResFilenameBimu1, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)(g_BattleSpecialStarCardResFilenameBimu1 + 12), (unsigned int)&g_BattleSpecialStarCardResFilenameBimu2, 0x00000000, 0x00000000, 0x003240b1, (unsigned int)(g_BattleSpecialStarCardResFilenameBimu2 + 12), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialStarCardResFilenameElec, 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)(g_BattleSpecialStarCardResFilenameElec + 12), 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)&g_BattleSpecialStarCardResFilenameIsi2, 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)(g_BattleSpecialStarCardResFilenameIsi2 + 12), 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)&g_BattleSpecialStarCardResFilenameIsi4, 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)(g_BattleSpecialStarCardResFilenameIsi4 + 12), 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)&g_BattleSpecialStarCardResFilenameIsi6, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)(g_BattleSpecialStarCardResFilenameIsi6 + 12), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialStarCardResFilenameSoto1, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)(g_BattleSpecialStarCardResFilenameSoto1 + 12), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialStarCardResFilenameSoto2, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)(g_BattleSpecialStarCardResFilenameSoto2 + 12), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialStarCardResFilenameSoto3, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)(g_BattleSpecialStarCardResFilenameSoto3 + 12), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialStarCardResFilenameSoto4, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)(g_BattleSpecialStarCardResFilenameSoto4 + 12), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialStarCardResFilenameSoto5, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)(g_BattleSpecialStarCardResFilenameSoto5 + 12), 0x00000000, 0x00000000, 0x00000000, 0x00192a39, 0x00000000, (unsigned int)&g_BattleSpecialStarCardResFilenameJoC, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x0083ba88
unsigned char g_BattleSpecialStarCardFallingModelIndices[24] = {0x05, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0083baa0
unsigned char g_BattleSpecialStarCardCyclingModelIndices[40] = {0x0c, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0083bac8
unsigned char g_BattleSpecialStarCardUpAxis[12] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// the vector (0, 0, -4096) the code measures an angle against by address
// GLOBAL: FF7 0x0083bad8
struct VECTOR g_BattleSpecialStarCardNegativeZUnitVector1 = {0, 0, -0x1000, 0};
// GLOBAL: FF7 0x0083bae8
struct VECTOR g_BattleSpecialStarCardForwardAxis = {0, 0, -0x1000, 0};
// GLOBAL: FF7 0x0083baf8
struct VECTOR g_BattleSpecialStarCardAxisAngleMatrix = {0x1000, 0, 0, 0};
// GLOBAL: FF7 0x0083bb08
struct BattleLimitHowlingMoonSpriteRequest g_BattleSpecialStarCardSpriteRequest1 = {0, 0, -128, -128, -128, {0x2e}, 0, {0}};
// GLOBAL: FF7 0x0083bb18
struct BattleLimitHowlingMoonSpriteRequest g_BattleSpecialStarCardSpriteRequest2 = {0, 0, -128, -128, -128, {0x2e}, 0, {0}};
// GLOBAL: FF7 0x0083bb28
struct BattleLimitStardustRayModelReq2 g_BattleSpecialStarCardSpriteRequest3 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
// GLOBAL: FF7 0x0083bb38
unsigned char g_BattleSpecialStarCardResFilenameCard[22] =
    "CARD1.RSD\0\0\0"
    "BIMU1.TIM";
// GLOBAL: FF7 0x0083bb50
unsigned char g_BattleSpecialStarCardResFilenameBimu1[22] =
    "BIMU1.SPT\0\0\0"
    "BIMU2.TIM";
// GLOBAL: FF7 0x0083bb68
unsigned char g_BattleSpecialStarCardResFilenameBimu2[22] =
    "BIMU2.SPT\0\0\0"
    "BAKU1.RSD";
// GLOBAL: FF7 0x0083bb80
unsigned char g_BattleSpecialStarCardResFilenameElec[21] =
    "ELEC1.RSD\0\0\0"
    "ISI1.RSD";
// GLOBAL: FF7 0x0083bb98
unsigned char g_BattleSpecialStarCardResFilenameIsi2[21] =
    "ISI2.RSD\0\0\0\0"
    "ISI3.RSD";
// GLOBAL: FF7 0x0083bbb0
unsigned char g_BattleSpecialStarCardResFilenameIsi4[21] =
    "ISI4.RSD\0\0\0\0"
    "ISI5.RSD";
// GLOBAL: FF7 0x0083bbc8
unsigned char g_BattleSpecialStarCardResFilenameIsi6[23] =
    "ISI6.RSD\0\0\0\0"
    "KUEISA.RSD";
// GLOBAL: FF7 0x0083bbe0
unsigned char g_BattleSpecialStarCardResFilenameSoto1[22] =
    "SOTO1.RSD\0\0\0"
    "SOTO2.RSD";
// GLOBAL: FF7 0x0083bbf8
unsigned char g_BattleSpecialStarCardResFilenameSoto2[22] =
    "SOTO3.RSD\0\0\0"
    "SOTO4.RSD";
// GLOBAL: FF7 0x0083bc10
unsigned char g_BattleSpecialStarCardResFilenameSoto3[22] =
    "SOTO5.RSD\0\0\0"
    "SOTO6.RSD";
// GLOBAL: FF7 0x0083bc28
unsigned char g_BattleSpecialStarCardResFilenameSoto4[22] =
    "SOTO7.RSD\0\0\0"
    "SOTO8.RSD";
// GLOBAL: FF7 0x0083bc40
unsigned char g_BattleSpecialStarCardResFilenameSoto5[23] =
    "SOTO9.RSD\0\0\0"
    "SOTO10.RSD";
// GLOBAL: FF7 0x0083bc58
unsigned char g_BattleSpecialStarCardResFilenameJoC[11] = "JO_C04.SPT";
// GLOBAL: FF7 0x00b49868
int g_BattleSpecialStarCardDrawListHandle = 0;
unsigned char g_bssPad_00b4986c[4] = {0};
// GLOBAL: FF7 0x00b49870
VECTOR g_BattleSpecialStarCardTransformedVector1 = {0};
// GLOBAL: FF7 0x00b49880
struct Int64Pair g_BattleSpecialStarCardTargetPos = {0};
// GLOBAL: FF7 0x00b49888
VECTOR g_BattleSpecialStarCardTransformedVector2 = {0};
// GLOBAL: FF7 0x00b49898
int g_BattleSpecialStarCardTargetMask = 0;
// GLOBAL: FF7 0x00b4989c
int g_BattleSpecialStarCardActorIdx = 0;
// GLOBAL: FF7 0x00b498a0
PsxMatrix g_BattleSpecialStarCardLoadedResTable = {0};
// GLOBAL: FF7 0x00b498c0
struct GfxTableCursor *g_BattleSpecialStarCardFadeStep = 0;
unsigned char g_bssPad_00b498c4[4] = {0};
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

// If the battle context exists and the CLUB2 resource handle is zero, initializes a default-
// duration record, builds the SPECIAL/CLUB2 resource path and requests resource type 23 using the
// context field at offset 0x4C. The record is represented by a scalar stack local with subsequent
// offset writes, so its apparent extent does not establish a safe source-level record allocation.
// FUNCTION: FF7 0x004f1e20
void __cdecl BattleFx_SpecialLoadResStarCard(void)
{
    GfxModelTableBank *context;
    GfxResourceRequest requestRecord;
    int kind;
    char pathOut[200];
    GfxResourceRequest *pReq;
    int resultDefEntry;

    if (g_pBattle3dScene != 0 && g_BattleSpecialStarCardFadeStep == 0) {
        kind = 0x17;
        resultDefEntry = (int)&g_BattleSpecialStarCardModelResourceTable;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestRecord;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x0083bc64
        pReq->resourcePath = BattleFx_BuildResourcePath("SPECIAL/CLUB2", pathOut);
        Gfx_LoadModelTableIfAbsent(kind, resultDefEntry, pReq, &g_BattleSpecialStarCardFadeStep, context);
    }
}

/*
 * Forwarder: calls BattleFx_SpecialLoadResStarCard() with no arguments, then BattleFx_SpecialBeginStarCard(targetMask, actorIdx).
 */
// FUNCTION: FF7 0x004f1eb0
void __cdecl BattleFx_SpecialEnterStarCard(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResStarCard();
    BattleFx_SpecialBeginStarCard(targetMask, actorIdx);
}

/*
 * Allocates and saves a draw-list-selection slot, stores targetMask as the actor mask and records its
 * first selected low-ten-bit actor when one exists, retaining the previous actor value when none
 * exists. Stores actorIdx and allocates BattleFx_SpecialStarCardSpawnTimeline.
 */
// FUNCTION: FF7 0x004f1eca
void __cdecl BattleFx_SpecialBeginStarCard(int targetMask, int actorIdx)
{
    int bitIdx;
    int selectedCount;

    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    g_BattleSpecialStarCardTargetMask = targetMask;
    bitIdx = 0;
    for (selectedCount = 0; bitIdx < 0xA; bitIdx++) {
        if ((targetMask & 1 << bitIdx) != 0) {
            selectedCount++;
            if (selectedCount == 1) {
                g_BattleSpecialStarCardDrawListHandle = bitIdx;
            }
        }
    }
    g_BattleSpecialStarCardActorIdx = actorIdx;
    Battle_AllocateResourceSlot(BattleFx_SpecialStarCardSpawnTimeline);
}

/*
 * Retires the current slot, gets the selected actor bone position into a local eight-byte pair and
 * copies that pair to +4 of a new BattleFx_SpecialStarCardTimelineTask slot. Calls BattleFx_SpecialStarCardBuildFacingVectors, then allocates
 * BattleFx_SpecialStarCardStageFadeTask with the same pair; no pause guard appears.
 */
// FUNCTION: FF7 0x004f1f5b
void __cdecl BattleFx_SpecialStarCardSpawnTimeline(void)
{
    struct {
        int *slot;
        BattleBonePosCopy pos;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = (int *)BATTLE_CURRENT_TASK_SLOT;
    *(short *)frame.slot = 0xffff;
    Battle_GetBoneWorldPos(g_BattleSpecialStarCardDrawListHandle,
                           g_BattleActorRecords[g_BattleSpecialStarCardDrawListHandle].targetBone,
                           (short *)&frame.pos);
    frame.slot = (int *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialStarCardTimelineTask);
    *(BattleBonePosCopy *)(frame.slot + 1) = frame.pos;
    BattleFx_SpecialStarCardBuildFacingVectors();
    frame.slot = (int *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialStarCardStageFadeTask);
    *(BattleBonePosCopy *)(frame.slot + 1) = frame.pos;
}

/*
 * Runs an unpaused sequence that creates a positioned callback and positional sound at age 2, a
 * vertically moving callback and centered sound at age 24, then at age 72 samples the target
 * attachment and creates an axis-angle-oriented moving callback toward its X/Z position with
 * positional sound. Uses the normalized distance square root divided by 12.
 * Retires at age 90 before incrementing age.
 */
// FUNCTION: FF7 0x004f1fff
void __cdecl BattleFx_SpecialStarCardTimelineTask(void)
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
        frame.slot = (BattleFxStarCardSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialStarCardSpinningModelTask);
        *(Int64Pair *)((char *)frame.slot + 4) = *(Int64Pair *)(&g_BattleSpecialStarCardTargetPos);
        frame.distance = g_BattleActorRecords[g_BattleSpecialStarCardActorIdx].posY - *(short *)((char *)&g_BattleSpecialStarCardTargetPos + 2);
        if (frame.distance < 0) {
            frame.distance *= -1;
        }
        frame.slot->distance = frame.distance;
        frame.slot->velocityY = 0xFED4;
        frame.slot->scale = 0x2000;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleSpecialStarCardTargetPos), 0x1D5);
    }
    if (frame.current->step == 0x18) {
        frame.slot = (BattleFxStarCardSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialStarCardOscillatingModelTask);
        frame.slot->offsetY = 0xFEF6;
        frame.slot->velocityY = 0x21;
        frame.slot->angle = 0;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, 0x40, 0x1D6);
    }
    if (frame.current->step == 0x48) {
        Battle_GetBoneWorldPos(g_BattleSpecialStarCardDrawListHandle, g_BattleActorRecords[g_BattleSpecialStarCardDrawListHandle].targetBone, (short *)((char *)frame.current + 4));
        frame.vector[0] = frame.current->posX - g_BattleCameraPosition.cameraPosX;
        frame.vector[1] = 0;
        frame.vector[2] = frame.current->posZ - g_BattleCameraPosZ;
        frame.distance = NormalizeVectorFixed(&frame.vector[0], &frame.vector[0]);
        frame.distance = SqrtToInt(frame.distance) / 0xC;
        frame.angle = ComputeAngleBetweenVectors(&g_BattleSpecialStarCardForwardAxis, (VECTOR *)(&frame.vector[0]), (VECTOR *)(&frame.axis[0]));
        Gte_BuildAxisAngleMatrix(frame.angle, (PsxMatrix *)(&frame.matrix[0]), (VECTOR *)(&frame.axis[0]));
        TransformVectorToInt32((PsxMatrix *)(&frame.matrix[0]), &g_BattleSpecialStarCardForwardAxis, &frame.vector[0]);
        if (frame.axis[1] < 0) {
            frame.angle *= -1;
        }
        frame.slot = (BattleFxStarCardSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialStarCardAcceleratingModelTask);
        frame.slot->posX = g_BattleCameraPosition.cameraPosX;
        frame.slot->offsetY = frame.current->posY;
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
 * Draws a uniformly scaled CLUB2 model using the stored actor heading, an age-driven axis rotation
 * and a rotated vertical offset; the angle advances through age 8 then remains a quarter turn.
 * Ages at least 18 retire before checking pause. From unpaused age 8 onward, adds vertical
 * velocity to Y and increases that velocity by its sixth, then increments age.
 */
// FUNCTION: FF7 0x004f22d0
void __cdecl BattleFx_SpecialStarCardSpinningModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        BattleFxStarCardSpinRecord *clubSpinRecord;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR rotOffsetX; };
        struct { unsigned char pad_to_18[0x18]; int axisAngle; };
        struct { unsigned char pad_to_1C[0x1C]; PsxMatrix axisMatrix; };
        struct { unsigned char pad_to_3C[0x3C]; PsxMatrix modelMatrix; };
        struct { unsigned char pad_to_5C[0x5C]; SVECTOR targetRotX; };
        struct { unsigned char pad_to_64[0x64]; BattleFxModelDraw *drawRecord; };
        struct { unsigned char pad_to_68[0x68]; VECTOR matrixScale; };
    } frame;
#pragma pack(pop)

    frame.clubSpinRecord = (BattleFxStarCardSpinRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.clubSpinRecord->step <= 8) {
        frame.axisAngle = frame.clubSpinRecord->step << 7;
    } else {
        frame.axisAngle = 0x400;
    }
    frame.targetRotX.vx = 0;
    frame.targetRotX.vy = (((short *)&g_BattleSpecialStarCardTargetPos)[3]);
    frame.targetRotX.vz = 0;
    RotMatrixXYZ(&frame.targetRotX, &frame.modelMatrix);
    Gte_BuildAxisAngleMatrix(-frame.axisAngle, &frame.axisMatrix, &g_BattleSpecialStarCardAxisAngleMatrix);
    ConcatMatrixInPlace(&frame.modelMatrix, &frame.axisMatrix);
    frame.rotOffsetX.vx = 0;
    frame.rotOffsetX.vy = frame.clubSpinRecord->spinRadius;
    frame.rotOffsetX.vz = 0;
    TransformVectorToInt16(&frame.axisMatrix, &frame.rotOffsetX, &frame.rotOffsetX.vx);
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0x800;
    RotMatrixXYZ(&frame.rotX, &frame.modelMatrix);
    ConcatMatrixInPlace(&frame.axisMatrix, &frame.modelMatrix);
    frame.modelMatrix.t[0] = frame.clubSpinRecord->posX + frame.rotOffsetX.vx;
    frame.modelMatrix.t[1] = frame.clubSpinRecord->posY + frame.rotOffsetX.vy;
    frame.modelMatrix.t[2] = frame.clubSpinRecord->posZ + frame.rotOffsetX.vz;
    frame.matrixScale.vz = frame.clubSpinRecord->scale;
    frame.matrixScale.vy = frame.matrixScale.vz;
    frame.matrixScale.vx = frame.matrixScale.vy;
    ScaleMatrix(&frame.modelMatrix, &frame.matrixScale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    frame.drawRecord = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.drawRecord->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialStarCardFadeStep);
    frame.drawRecord->flags = 0;
    frame.drawRecord->uvScroll = 0;
    frame.drawRecord->fadeLevel = 0x80;
    frame.drawRecord->stateFlags = 0x20;
    frame.drawRecord->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.drawRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.clubSpinRecord->step >= 0x12) {
        frame.clubSpinRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.clubSpinRecord->step >= 8) {
        frame.clubSpinRecord->posY += frame.clubSpinRecord->velocityY;
        frame.clubSpinRecord->velocityY += (short)(frame.clubSpinRecord->velocityY / 6);
    }
    (frame.clubSpinRecord->step)++;
}

/*
 * Draws the CLUB2 resource model at fixed X/Z and slot Y minus 40 with a quarter-turn X rotation
 * and configurable Y rotation. Retires at age 48 before checking pause. Unpaused ages below 8 move
 * Y with damped velocity clamped to at least 1; ages 8 through 39 set Y to a sine oscillation and
 * rotate during ages 22 through 33; later ages move Y with velocity increased by a quarter.
 */
// FUNCTION: FF7 0x004f2525
void __cdecl BattleFx_SpecialStarCardOscillatingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        BattleFxOscillatingModel *oscModel;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_18[0x18]; int sinAngle; };
        struct { unsigned char pad_to_1C[0x1C]; PsxMatrix tiltMatrix; };
        struct { unsigned char pad_to_3C[0x3C]; PsxMatrix matrix; };
        struct { unsigned char pad_to_5C[0x5C]; SVECTOR tiltX; };
        struct { unsigned char pad_to_64[0x64]; BattleFxModelDraw *drawModel; };
    } frame;
#pragma pack(pop)

    frame.oscModel = (BattleFxOscillatingModel *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0x400;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.matrix);
    frame.matrix.t[0] = 0;
    frame.matrix.t[1] = frame.oscModel->offset - 0x28;
    frame.matrix.t[2] = 0x380;
    frame.tiltX.vx = 0;
    frame.tiltX.vy = frame.oscModel->angle;
    frame.tiltX.vz = 0;
    RotMatrixXYZ(&frame.tiltX, &frame.tiltMatrix);
    ConcatMatrixInPlace(&frame.tiltMatrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    frame.drawModel = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.drawModel->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialStarCardFadeStep);
    frame.drawModel->flags = 0;
    frame.drawModel->uvScroll = 0;
    frame.drawModel->fadeLevel = 0x80;
    frame.drawModel->stateFlags = 0x20;
    frame.drawModel->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.drawModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.oscModel->phase >= 0x30) {
        frame.oscModel->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.oscModel->phase < 8) {
        frame.oscModel->offset += frame.oscModel->velocity;
        frame.oscModel->velocity -= (short)(frame.oscModel->velocity >> 5);
        if (frame.oscModel->velocity < 2) {
            frame.oscModel->velocity = 1;
        }
    } else if (frame.oscModel->phase < 0x28) {
        frame.sinAngle = frame.oscModel->phase - 8 << 7;
        frame.oscModel->offset = SinFixed(frame.sinAngle) * 0x18 >> 0xC;
        if (frame.oscModel->phase >= 0x16 && frame.oscModel->phase < 0x22) {
            frame.oscModel->angle -= 0xAA;
        }
    } else {
        frame.oscModel->offset += frame.oscModel->velocity;
        frame.oscModel->velocity += (short)(frame.oscModel->velocity >> 2);
    }
    (frame.oscModel->phase)++;
}

/*
 * Draws a uniformly scaled CLUB2 model at the slot position with configured Y rotation and half-
 * turn Z rotation, enabling flag 8 and reducing intensity by 16 per age from age 12. Retires at
 * age 20 before checking pause. Unpaused age 10 triggers a target operation selected by screen pan
 * above 64 and positional sound; all unpaused calls advance X/Z and increase velocities by their
 * sixteenth, then advance age.
 */
// FUNCTION: FF7 0x004f2760
void __cdecl BattleFx_SpecialStarCardAcceleratingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        BattleFxSpecialAcceleratingSlot *accelRecord;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_18[0x18]; int screenPan; };
        struct { unsigned char pad_to_3C[0x3C]; PsxMatrix matrix; };
        struct { unsigned char pad_to_64[0x64]; BattleFxModelDraw *drawModel; };
        struct { unsigned char pad_to_68[0x68]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.accelRecord = (BattleFxSpecialAcceleratingSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.accelRecord->angleY;
    frame.rotX.vz = 0x800;
    RotMatrixXYZ(&frame.rotX, &frame.matrix);
    frame.matrix.t[0] = frame.accelRecord->posA;
    frame.matrix.t[1] = frame.accelRecord->posY;
    frame.matrix.t[2] = frame.accelRecord->posB;
    frame.scaleX.vz = frame.accelRecord->scale;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.matrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    frame.drawModel = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.drawModel->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialStarCardFadeStep);
    frame.drawModel->flags = 0;
    frame.drawModel->uvScroll = 0;
    frame.drawModel->fadeLevel = 0x80;
    frame.drawModel->stateFlags = 0x20;
    frame.drawModel->texturePageBits = 0;
    if (frame.accelRecord->timer >= 0xC) {
        frame.drawModel->flags = 8;
        frame.drawModel->fadeLevel = 0x80 - (frame.accelRecord->timer - 0xC << 4);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.drawModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.accelRecord->timer >= 0x14) {
        frame.accelRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.accelRecord->timer == 0xA) {
        frame.screenPan = Battle_ComputeScreenPanFromPosition((char *)frame.accelRecord + 4);
        if (frame.screenPan > 0x40) {
            BattleFx_HitMarkSpawnAtActorBone(g_BattleSpecialStarCardDrawListHandle, 0x118);
        } else {
            BattleFx_HitMarkSpawnAtActorBone(g_BattleSpecialStarCardDrawListHandle, 0x18);
        }
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, frame.screenPan, 0x1D8);
    }
    frame.accelRecord->posA += frame.accelRecord->velocityA;
    frame.accelRecord->posB += frame.accelRecord->velocityB;
    frame.accelRecord->velocityA += (short)(frame.accelRecord->velocityA >> 4);
    frame.accelRecord->velocityB += (short)(frame.accelRecord->velocityB >> 4);
    (frame.accelRecord->timer)++;
}

/*
 * Gets the g_BattleSpecialStarCardActorIdx actor bone selected by its +0xBE11AD byte into
 * g_BattleSpecialStarCardTargetPos, lowers Y by 224 and stores actor facing in its fourth short.
 * Builds a Y rotation from that facing and transforms fixed vectors at
 * g_BattleSpecialStarCardForwardAxis (0x83BAE8) and g_BattleSpecialStarCardAxisAngleMatrix (0x83BAF8)
 * into g_BattleSpecialStarCardTransformedVector2 (0xB49888) and g_BattleSpecialStarCardTransformedVector1
 * (0xB49870).
 */
// FUNCTION: FF7 0x004f29ac
void __cdecl BattleFx_SpecialStarCardBuildFacingVectors(void)
{
    struct {
        int matrix[8];
        short rot[4];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    Battle_GetBoneWorldPos(g_BattleSpecialStarCardActorIdx,
                           g_BattleActorRecords[g_BattleSpecialStarCardActorIdx].altBone,
                           ((short *)&g_BattleSpecialStarCardTargetPos));
    ((short *)&g_BattleSpecialStarCardTargetPos)[1] -= 0xe0;
    ((short *)&g_BattleSpecialStarCardTargetPos)[3] = g_BattleActorRecords[g_BattleSpecialStarCardActorIdx].yaw;
    frame.rot[0] = 0;
    frame.rot[1] = ((short *)&g_BattleSpecialStarCardTargetPos)[3];
    frame.rot[2] = 0;
    RotMatrixXYZ((void *)frame.rot, (void *)frame.matrix);
    TransformVectorToInt32((PsxMatrix *)frame.matrix, &g_BattleSpecialStarCardForwardAxis, &g_BattleSpecialStarCardTransformedVector2);
    TransformVectorToInt32((PsxMatrix *)frame.matrix, &g_BattleSpecialStarCardAxisAngleMatrix, &g_BattleSpecialStarCardTransformedVector1);
}

/*
 * One frame of a battle effect that fires
 * BattleFx_SpecialStarCardSpawnImpactTimeline(g_BattleSpecialStarCardDrawListHandle,
 * g_BattleSpecialStarCardActorIdx) on exactly frame 0x5A and drives the 12-bit level
 * g_BattleFxTimelineState.startValue from the frame counter at +2: zero before frame 0x4E, rising
 * 0x155 per frame from 0x4E to 0x59 (zeroing the colour bytes g_BattleFxTimelineState,
 * g_BattleFxTimelineState.channel1 and g_BattleFxTimelineState.channel2 on the way), held at 0x1000
 * through frame 0x8D, falling 0x200 per frame from 0x8E, and forced to zero at or above frame 0x96
 * where the effect also retires with 0xFFFF at +0. Returns immediately while g_BattleIsPaused is
 * set.
 */
// FUNCTION: FF7 0x004f2a59
void __cdecl BattleFx_SpecialStarCardStageFadeTask(void)
{
    unsigned short level;
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0x5A) {
        BattleFx_SpecialStarCardSpawnImpactTimeline(g_BattleSpecialStarCardDrawListHandle, g_BattleSpecialStarCardActorIdx);
    }
    if (task->frameCounter < 0x4E) {
        level = 0;
    } else if (task->frameCounter < 0x5A) {
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        level = (task->frameCounter - 0x4E) * 0x155;
    } else if (task->frameCounter >= 0x8E) {
        level = 0x1000 - (task->frameCounter - 0x8E << 9);
    } else {
        level = 0x1000;
    }
    if (task->frameCounter >= 0x96) {
        level = 0;
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    g_BattleFxTimelineState.startValue = level;
    (task->frameCounter)++;
}

/*
 * Allocates a battle-effect slot (Battle_AllocateResourceSlot(BattleFx_SpecialStarCardImpactTimelineTask)) at
 * g_BattleHitLivewireEmitterRecords, fills its +4 vector with actor attackerIndex's bone
 * g_BattleActorRecords[0].targetBone, stores
 * min(g_BattleActorRecords[attackerIndex].radius + 0x2A0, 0x76C) at +0x1C, writes actor
 * targetIndex's bone g_BattleActorRecords[0].altBone position into the vector at
 * g_BattleSpecialStarCardTargetPos (0xB49880) and lowers its y (0xB49882) by 0x1F8, then plays sound
 * 0x1D9 via Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(slot+4), 0x1D9).
 */
// FUNCTION: FF7 0x004f2b68
void __cdecl BattleFx_SpecialStarCardSpawnImpactTimeline(int attackerIndex, int targetIndex)
{
    int ofsY;
    int padSlot[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxStarCardImpactTimeline *record;

    record = (BattleFxStarCardImpactTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialStarCardImpactTimelineTask);
    Battle_GetBoneWorldPos(attackerIndex, g_BattleActorRecords[attackerIndex].targetBone, &record->bonePos);
    ofsY = g_BattleActorRecords[attackerIndex].radius + 0x2A0;
    if (ofsY > 0x76C) {
        ofsY = 0x76C;
    }
    record->offsetY = (short)ofsY;
    Battle_GetBoneWorldPos(targetIndex, g_BattleActorRecords[targetIndex].altBone, ((short *)&g_BattleSpecialStarCardTargetPos));
    ((short *)&g_BattleSpecialStarCardTargetPos)[1] -= 0x1F8;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition((char *)record + 4), 0x1D9);
}

/*
 * Recomputes a shared axis-angle matrix from the midpoint of the stored position and controller
 * toward the fixed reference even while paused. Unpaused age 0 creates three rotating model
 * callbacks; age 6 creates two groups of five callbacks aimed at randomized target X/Z positions.
 * Ages 10 through 49 emit one randomly positioned callback per call, and odd ages 15 through 39
 * emit a moving callback with duration 12 through 15 and derived rotation/scale rates.
 * Age 55 allocates a typed target record and age 60 retires before incrementing age.
 */
// FUNCTION: FF7 0x004f2c35
void __cdecl BattleFx_SpecialStarCardImpactTimelineTask(void)
{
    int half;
    int idx;
    VECTOR dirX;
    int baseScale;
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int scaleRoll;
    BattleFxStarCardImpact *pImpact;
    BattleFxStarCardImpactParticle *child;
    VECTOR axis;
    int width;

    pImpact = (BattleFxStarCardImpact *)BATTLE_CURRENT_TASK_SLOT;
    dirX.vx = g_BattleCameraPosition.cameraPosX - (((short *)&g_BattleSpecialStarCardTargetPos)[0] + pImpact->posX >> 1);
    dirX.vy = 0;
    dirX.vz = g_BattleCameraPosZ - (((short *)&g_BattleSpecialStarCardTargetPos)[2] + pImpact->posZ >> 1);
    NormalizeVectorFixed(&dirX, &dirX);
    idx = ComputeAngleBetweenVectors(&g_BattleSpecialStarCardNegativeZUnitVector1, &dirX, &axis);
    Gte_BuildAxisAngleMatrix(idx, (PsxMatrix *)&g_BattleSpecialStarCardLoadedResTable, &axis);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (pImpact->step == 0) {
        child = (BattleFxStarCardImpactParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialStarCardPulsingModelTask);
        *(Int64Pair *)((char *)child + 4) = *&g_BattleSpecialStarCardTargetPos;
        child->angle = 0;
        child->velocityY = rand() % 0x48 + 0x40;
        child->velocityX = 0;
        child->size = rand() % 0x3A + 0x2C;
        child->scale = 0x5B6;
    }
    if (pImpact->step == 0) {
        child = (BattleFxStarCardImpactParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialStarCardCyclingModelTask);
        *(Int64Pair *)((char *)child + 4) = *&g_BattleSpecialStarCardTargetPos;
        child->angle = 0xE0;
        child->velocityY = -(rand() % 0x1E + 0x18);
        child->velocityX = 0xFF9C;
        child->size = rand() % 0x14 + 0xC;
        child->scale = 0x560;
    }
    if (pImpact->step == 0) {
        child = (BattleFxStarCardImpactParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialStarCardBlackModelTask);
        *(Int64Pair *)((char *)child + 4) = *&g_BattleSpecialStarCardTargetPos;
        child->angle = 0x12;
        child->velocityY = -(rand() % 0x28 + 0x24);
        child->velocityX = 0x24;
        child->size = rand() % 0x20 + 0x16;
        child->scale = 0x53B;
    }
    if (pImpact->step == 6) {
        half = pImpact->angle;
        width = half << 1;
        for (idx = 0; idx < 5; idx++) {
            child = (BattleFxStarCardImpactParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialStarCardStretchedSprite1Task);
            *(Int64Pair *)((char *)child + 4) = *&g_BattleSpecialStarCardTargetPos;
            *(Int64Pair *)((char *)child + 0xC) = *(Int64Pair *)((char *)pImpact + 4);
            child->targetX += (short)(rand() % width - half);
            child->targetY = 0;
            child->targetZ += (short)(rand() % width - half);
        }
        for (idx = 0; idx < 5; idx++) {
            child = (BattleFxStarCardImpactParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialStarCardStretchedSprite2Task);
            *(Int64Pair *)((char *)child + 4) = *&g_BattleSpecialStarCardTargetPos;
            *(Int64Pair *)((char *)child + 0xC) = *(Int64Pair *)((char *)pImpact + 4);
            child->targetX += (short)(rand() % width - half);
            child->targetY = 0;
            child->targetZ += (short)(rand() % width - half);
        }
    }
    if (pImpact->step >= 0xA && pImpact->step <= 0x31) {
        width = pImpact->angle;
        half = width >> 1;
        baseScale = 0xC80;
        for (idx = 0; idx < 1; idx++) {
            child = (BattleFxStarCardImpactParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialStarCardDrawBillboardSpriteTask);
            *(Int64Pair *)((char *)child + 4) = *(Int64Pair *)((char *)pImpact + 4);
            child->posX += (short)(rand() % width - half);
            child->posY += (short)(rand() % width - half);
            child->posZ += (short)(rand() % width - half);
            scaleRoll = rand() % 0xE00 + 0xA00;
            child->scale = baseScale * scaleRoll >> 0xC;
        }
    }
    if (pImpact->step >= 0xE && pImpact->step <= 0x27 && (pImpact->step & 1) != 0) {
        idx = rand() % 4 + 0xC;
        half = pImpact->angle / 3 << 1;
        width = half << 1;
        child = (BattleFxStarCardImpactParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialStarCardFallingModelTask);
        *(Int64Pair *)((char *)child + 4) = *&g_BattleSpecialStarCardTargetPos;
        child->frameCount = idx;
        *(Int64Pair *)((char *)child + 0xC) = *(Int64Pair *)((char *)pImpact + 4);
        child->targetX += (short)(rand() % width - half);
        child->targetY = 0;
        child->targetZ += (short)(rand() % width - half);
        child->targetX = (child->targetX - child->posX) / idx;
        child->targetY = (child->targetY - child->posY) / idx;
        child->targetZ = (child->targetZ - child->posZ) / idx;
        child->variant = rand() % 6;
        child->velocityX = 0;
        child->angle = 0;
        child->velocityY = rand() % 0x60 + 0x30;
        child->size = child->velocityY >> 1;
        baseScale = rand() % 0xE00 + 0xA00;
        scaleRoll = baseScale * 0x6E4 >> 0xC;
        child->scale = scaleRoll >> 1;
        child->scaleRate = (scaleRoll >> 1) / idx;
    }
    if (pImpact->step == 0x37) {
        Battle_PostActorHitOrHurtMessage(g_BattleSpecialStarCardDrawListHandle);
    }
    if (pImpact->step >= 0x3C) {
        pImpact->ownerSlot = BATTLE_TASK_FREE;
    }
    (pImpact->step)++;
}

/*
 * Draws a table-selected CLUB2 model with X/Z rotation and uniform scale. When age reaches stored
 * duration plus 4 or Y reaches -120, allocates a ground-level replacement callback with randomized
 * rotation rate and enlarged scale, then retires the current slot before checking pause; this
 * replacement can therefore spawn while paused. Unpaused calls advance XYZ with velocity damping
 * by a sixty-fourth, advance angles and scale, increase the scale rate by its sixty-fourth and
 * increment age.
 */
// FUNCTION: FF7 0x004f32fd
void __cdecl BattleFx_SpecialStarCardFallingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[68];
        PsxMatrix matrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxStarCardFallRecord *fallRecord; };
        struct { unsigned char pad_to_24[0x24]; BattleStarCardFallingModel *spawnedModel; };
        struct { unsigned char pad_to_28[0x28]; SVECTOR angleX; };
        struct { unsigned char pad_to_30[0x30]; BattleFxModelDraw *drawModel; };
        struct { unsigned char pad_to_34[0x34]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.fallRecord = (BattleFxStarCardFallRecord *)BATTLE_CURRENT_TASK_SLOT;
    frame.angleX.vx = frame.fallRecord->angle;
    frame.angleX.vy = 0;
    frame.angleX.vz = frame.fallRecord->tilt;
    RotMatrixXYZ(&frame.angleX, &frame.matrix);
    frame.matrix.t[0] = frame.fallRecord->posX;
    frame.matrix.t[1] = frame.fallRecord->posY;
    frame.matrix.t[2] = frame.fallRecord->posZ;
    frame.scaleX.vz = frame.fallRecord->scale;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.matrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    frame.drawModel = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.drawModel->modelGroup = Gfx_ModelTableGetRenderObject(((int *)&g_BattleSpecialStarCardFallingModelIndices)[frame.fallRecord->modelIndex], g_BattleSpecialStarCardFadeStep);
    frame.drawModel->flags = 0;
    frame.drawModel->uvScroll = 0;
    frame.drawModel->fadeLevel = 0x80;
    frame.drawModel->stateFlags = 0x20;
    frame.drawModel->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.drawModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.fallRecord->phase >= frame.fallRecord->endPhase + 4 || frame.fallRecord->posY >= -0x78) {
        frame.spawnedModel = (BattleStarCardFallingModel *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialStarCardSpreadingModelTask);
        *(Int64Pair *)((char *)frame.spawnedModel + 4) = *(Int64Pair *)((char *)frame.fallRecord + 4);
        frame.spawnedModel->posY = 0;
        frame.spawnedModel->angle = frame.fallRecord->angle;
        frame.spawnedModel->angleVelocity = rand() % 0x44 + 0x4E;
        frame.spawnedModel->scaleXZ = frame.fallRecord->scale + (rand() % 512) + 0x100;
        frame.spawnedModel->scaleY = frame.fallRecord->scale << 1;
        frame.fallRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.fallRecord->posX += frame.fallRecord->velocityX;
    frame.fallRecord->posY += frame.fallRecord->velocityY;
    frame.fallRecord->posZ += frame.fallRecord->velocityZ;
    frame.fallRecord->velocityX -= (short)(frame.fallRecord->velocityX >> 6);
    frame.fallRecord->velocityY -= (short)(frame.fallRecord->velocityY >> 6);
    frame.fallRecord->velocityZ -= (short)(frame.fallRecord->velocityZ >> 6);
    frame.fallRecord->angle += frame.fallRecord->angleVelocity;
    frame.fallRecord->tilt += frame.fallRecord->tiltVelocity;
    frame.fallRecord->scale += frame.fallRecord->scaleRate;
    frame.fallRecord->scaleRate += (short)(frame.fallRecord->scaleRate >> 6);
    (frame.fallRecord->phase)++;
}

/*
 * Draws CLUB2 model 3 with flag 0x28, Y rotation and separate horizontal/vertical scales, fading
 * intensity by 12 per age. Retires at age 10 before checking pause. Unpaused calls advance the
 * rotation, damp its rate by an eighth, grow horizontal scale by an eighth, shrink vertical scale
 * by an eighth and increment age.
 */
// FUNCTION: FF7 0x004f3608
void __cdecl BattleFx_SpecialStarCardSpreadingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        PsxMatrix matrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxStarCardSpread *spreadRecord; };
        struct { unsigned char pad_to_24[0x24]; SVECTOR rotX; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxModelDraw *drawModel; };
        struct { unsigned char pad_to_30[0x30]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.spreadRecord = (BattleFxStarCardSpread *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.spreadRecord->angle;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.matrix);
    frame.matrix.t[0] = frame.spreadRecord->baseA;
    frame.matrix.t[1] = frame.spreadRecord->baseB;
    frame.matrix.t[2] = frame.spreadRecord->baseC;
    frame.scaleX.vz = frame.spreadRecord->scaleXZ;
    frame.scaleX.vx = frame.scaleX.vz;
    frame.scaleX.vy = frame.spreadRecord->scaleY;
    ScaleMatrix(&frame.matrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    frame.drawModel = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.drawModel->modelGroup = Gfx_ModelTableGetRenderObject(3, g_BattleSpecialStarCardFadeStep);
    frame.drawModel->flags = 0x28;
    frame.drawModel->uvScroll = 0;
    frame.drawModel->fadeLevel = 0x80 - frame.spreadRecord->step * 0xC;
    frame.drawModel->stateFlags = 0x20;
    frame.drawModel->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.drawModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.spreadRecord->step >= 0xA) {
        frame.spreadRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spreadRecord->angle += frame.spreadRecord->angleVelocity;
    frame.spreadRecord->angleVelocity -= (short)(frame.spreadRecord->angleVelocity >> 3);
    frame.spreadRecord->scaleXZ += (short)(frame.spreadRecord->scaleXZ >> 3);
    frame.spreadRecord->scaleY -= (short)(frame.spreadRecord->scaleY >> 3);
    (frame.spreadRecord->step)++;
}

/*
 * Draws an animated sprite aligned from the slot origin toward its stored endpoint and combined
 * with the shared effect matrix. Horizontal scale is fixed at 8192 and vertical scale derives from
 * the square-root distance divided by 1920, capped at 32767. Animation advances through frame 11
 * then cycles frames 12 through 15; from age 36 the byte color fades by 10 per age. Retires at age
 * 48 before checking pause; only age advancement is paused.
 */
// FUNCTION: FF7 0x004f37dd
void __cdecl BattleFx_SpecialStarCardStretchedSprite1Task(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[140];
        PsxMatrix matrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxStarCardStretch *stretchRecord; };
        struct { unsigned char pad_to_24[0x24]; VECTOR dirX; };
        struct { unsigned char pad_to_38[0x38]; int axisAngle; };
        struct { unsigned char pad_to_3C[0x3C]; int distance; };
        struct { unsigned char pad_to_40[0x40]; SVECTOR rotX; };
        struct { unsigned char pad_to_48[0x48]; PsxMatrix axisMatrix; };
        struct { unsigned char pad_to_68[0x68]; VECTOR axis; };
        struct { unsigned char pad_to_78[0x78]; VECTOR scaleX; };
        struct { unsigned char pad_to_88[0x88]; char colorLevel; };
    } frame;
#pragma pack(pop)

    frame.stretchRecord = (BattleFxStarCardStretch *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.matrix);
    frame.matrix.t[0] = frame.stretchRecord->startX;
    frame.matrix.t[1] = frame.stretchRecord->startY;
    frame.matrix.t[2] = frame.stretchRecord->startZ;
    ConcatMatrixInPlace(&g_BattleSpecialStarCardLoadedResTable, &frame.matrix);
    frame.dirX.vx = frame.stretchRecord->endX - frame.stretchRecord->startX;
    frame.dirX.vy = frame.stretchRecord->endY - frame.stretchRecord->startY;
    frame.dirX.vz = frame.stretchRecord->endZ - frame.stretchRecord->startZ;
    frame.distance = NormalizeVectorFixed(&frame.dirX, &frame.dirX);
    frame.distance = SqrtToInt(frame.distance);
    frame.axisAngle = ComputeAngleBetweenVectors((VECTOR *)&g_BattleSpecialStarCardUpAxis, &frame.dirX, &frame.axis);
    Gte_BuildAxisAngleMatrix(frame.axisAngle, &frame.axisMatrix, &frame.axis);
    ConcatMatrixInPlace(&frame.axisMatrix, &frame.matrix);
    frame.scaleX.vz = 0x2000;
    frame.scaleX.vx = frame.scaleX.vz;
    frame.scaleX.vy = (frame.distance << 0xC) / 0x780;
    if (frame.scaleX.vy > 0x7FFF) {
        frame.scaleX.vy = 0x7FFF;
    }
    ScaleMatrix(&frame.matrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    if (frame.stretchRecord->spriteFrame < 0xC) {
        g_BattleSpecialStarCardSpriteRequest1.frame = frame.stretchRecord->spriteFrame;
    } else {
        g_BattleSpecialStarCardSpriteRequest1.frame = ((frame.stretchRecord->spriteFrame - 0xC) % 4) + 0xC;
    }
    if (frame.stretchRecord->spriteFrame >= 0x24) {
        frame.colorLevel = 0x80 - (frame.stretchRecord->spriteFrame - 0x24) * 0xA;
    } else {
        frame.colorLevel = 0x80;
    }
    g_BattleSpecialStarCardSpriteRequest1.colorB = frame.colorLevel;
    g_BattleSpecialStarCardSpriteRequest1.colorG = g_BattleSpecialStarCardSpriteRequest1.colorB;
    g_BattleSpecialStarCardSpriteRequest1.colorR = g_BattleSpecialStarCardSpriteRequest1.colorG;
    (g_BattleSpecialStarCardSpriteRequest1.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecialStarCardFadeStep);
    g_BattleSpecialStarCardSpriteRequest1.drawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecialStarCardFadeStep);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&(g_BattleSpecialStarCardSpriteRequest1.spriteGroup), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.stretchRecord->spriteFrame >= 0x30) {
        frame.stretchRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.stretchRecord->spriteFrame)++;
}

/*
 * Draws resource sprite 2 aligned from the slot origin toward its stored endpoint and combined
 * with the shared effect matrix. Horizontal scale is fixed at 8192 and vertical scale derives from
 * the square-root distance divided by 1920, capped at 32767. Animation advances through frame 3
 * then cycles frames 4 through 7; from age 36 the byte color fades by 10 per age. Retires at age
 * 48 before checking pause; only age advancement is paused.
 */
// FUNCTION: FF7 0x004f3a6e
void __cdecl BattleFx_SpecialStarCardStretchedSprite2Task(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[140];
        PsxMatrix matrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxStarCardStretch *stretchRecord; };
        struct { unsigned char pad_to_24[0x24]; VECTOR dirX; };
        struct { unsigned char pad_to_38[0x38]; int axisAngle; };
        struct { unsigned char pad_to_3C[0x3C]; int distance; };
        struct { unsigned char pad_to_40[0x40]; SVECTOR rotX; };
        struct { unsigned char pad_to_48[0x48]; PsxMatrix axisMatrix; };
        struct { unsigned char pad_to_68[0x68]; VECTOR axis; };
        struct { unsigned char pad_to_78[0x78]; VECTOR scaleX; };
        struct { unsigned char pad_to_88[0x88]; char colorLevel; };
    } frame;
#pragma pack(pop)

    frame.stretchRecord = (BattleFxStarCardStretch *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.matrix);
    frame.matrix.t[0] = frame.stretchRecord->startX;
    frame.matrix.t[1] = frame.stretchRecord->startY;
    frame.matrix.t[2] = frame.stretchRecord->startZ;
    ConcatMatrixInPlace(&g_BattleSpecialStarCardLoadedResTable, &frame.matrix);
    frame.dirX.vx = frame.stretchRecord->endX - frame.stretchRecord->startX;
    frame.dirX.vy = frame.stretchRecord->endY - frame.stretchRecord->startY;
    frame.dirX.vz = frame.stretchRecord->endZ - frame.stretchRecord->startZ;
    frame.distance = NormalizeVectorFixed(&frame.dirX, &frame.dirX);
    frame.distance = SqrtToInt(frame.distance);
    frame.axisAngle = ComputeAngleBetweenVectors((VECTOR *)&g_BattleSpecialStarCardUpAxis, &frame.dirX, &frame.axis);
    Gte_BuildAxisAngleMatrix(frame.axisAngle, &frame.axisMatrix, &frame.axis);
    ConcatMatrixInPlace(&frame.axisMatrix, &frame.matrix);
    frame.scaleX.vz = 0x2000;
    frame.scaleX.vx = frame.scaleX.vz;
    frame.scaleX.vy = (frame.distance << 0xC) / 0x780;
    if (frame.scaleX.vy > 0x7FFF) {
        frame.scaleX.vy = 0x7FFF;
    }
    ScaleMatrix(&frame.matrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    if (frame.stretchRecord->spriteFrame < 4) {
        g_BattleSpecialStarCardSpriteRequest2.frame = frame.stretchRecord->spriteFrame;
    } else {
        g_BattleSpecialStarCardSpriteRequest2.frame = ((frame.stretchRecord->spriteFrame - 4) % 4) + 4;
    }
    if (frame.stretchRecord->spriteFrame >= 0x24) {
        frame.colorLevel = 0x80 - (frame.stretchRecord->spriteFrame - 0x24) * 0xA;
    } else {
        frame.colorLevel = 0x80;
    }
    g_BattleSpecialStarCardSpriteRequest2.colorB = frame.colorLevel;
    g_BattleSpecialStarCardSpriteRequest2.colorG = g_BattleSpecialStarCardSpriteRequest2.colorB;
    g_BattleSpecialStarCardSpriteRequest2.colorR = g_BattleSpecialStarCardSpriteRequest2.colorG;
    (g_BattleSpecialStarCardSpriteRequest2.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(2, g_BattleSpecialStarCardFadeStep);
    g_BattleSpecialStarCardSpriteRequest2.drawObj = Gfx_ModelTableGetSpriteObject(2, g_BattleSpecialStarCardFadeStep);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&(g_BattleSpecialStarCardSpriteRequest2.spriteGroup), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.stretchRecord->spriteFrame >= 0x30) {
        frame.stretchRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.stretchRecord->spriteFrame)++;
}

/*
 * One frame of a battle effect drawing the sprite record at g_BattleSpecialStarCardSpriteRequest3. Copies the frame counter
 * at +2 into g_BattleSpecialStarCardSpriteRequest3.frameIndex, installs the billboard matrix from the size at +0x1C with -(size >> 4),
 * fills g_BattleSpecialStarCardSpriteRequest3 from Gfx_ModelTableGetLoadedRecord(0x16, g_BattleSpecialStarCardFadeStep) and g_BattleSpecialStarCardSpriteRequest3.drawObj from
 * Battle3d_GetSceneRenderObject(3, g_pBattle3dScene), and chains the sprite through
 * BattleFx_SubmitSpriteDraw into the list head g_BattleFxActiveDrawListPtr. Retires the effect with 0xFFFF at +0
 * at or above frame 3 and advances the counter unless g_BattleIsPaused is set. Unreferenced padding
 * locals preserve the retail VC5 /Od stack frame size.
 */
// FUNCTION: FF7 0x004f3cff
void __cdecl BattleFx_SpecialStarCardDrawBillboardSpriteTask(void)
{
    int padSlot[3]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxStarCardBillboardSprite *s;

    s = (BattleFxStarCardBillboardSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialStarCardSpriteRequest3.frameIndex = s->age;
    BattleFx_InstallBillboardMatrix((char *)s + 4, (short)s->scale, -(s->scale >> 4));
    (g_BattleSpecialStarCardSpriteRequest3.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0x16, g_BattleSpecialStarCardFadeStep);
    g_BattleSpecialStarCardSpriteRequest3.drawObj = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&(g_BattleSpecialStarCardSpriteRequest3.spriteGroup), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (s->age >= 3) {
        s->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (s->age)++;
}

/*
 * Draws CLUB2 model 11 with flag 0xA8, black color, uniform scale and X/Y rotation. Its intensity
 * decreases from 4096 by 512 per age through age 7, remains zero through age 45, and increases by
 * 512 per age from age 46. Retires at age 54 before checking pause; unpaused calls advance both
 * rotations and age.
 */
// FUNCTION: FF7 0x004f3dd0
void __cdecl BattleFx_SpecialStarCardBlackModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxStarCardBlackModelState *clubState; };
        struct { unsigned char pad_to_24[0x24]; SVECTOR rotX; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_30[0x30]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.clubState = (BattleFxStarCardBlackModelState *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = frame.clubState->accumA;
    frame.rotX.vy = frame.clubState->accumB;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.clubState->posX;
    frame.rotMatrix.t[1] = frame.clubState->posY;
    frame.rotMatrix.t[2] = frame.clubState->posZ;
    frame.scale.vz = frame.clubState->scale;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(0xB, g_BattleSpecialStarCardFadeStep);
    frame.modelDraw->flags = 0xA8;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    if (frame.clubState->timer < 8) {
        frame.modelDraw->fadeLevel = 0x1000 - (frame.clubState->timer << 9);
    } else if (frame.clubState->timer >= 0x2E) {
        frame.modelDraw->fadeLevel = frame.clubState->timer - 0x2E << 9;
    } else {
        frame.modelDraw->fadeLevel = 0;
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.clubState->timer >= 0x36) {
        frame.clubState->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.clubState->accumA += frame.clubState->stepA;
    frame.clubState->accumB += frame.clubState->stepB;
    (frame.clubState->timer)++;
}

/*
 * Draws a model selected by the age modulo 10 table with flag 0x28, uniform scale and X/Z
 * rotation. Intensity rises by 24 per age through age 7, remains 192 through age 45, then falls by
 * 24 per age. Retires at age 54 before checking pause; unpaused calls advance both rotations and
 * age.
 */
// FUNCTION: FF7 0x004f3fb0
void __cdecl BattleFx_SpecialStarCardCyclingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[68];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxStarCardCycle *cycleState; };
        struct { unsigned char pad_to_24[0x24]; int modelIndex; };
        struct { unsigned char pad_to_28[0x28]; SVECTOR rotX; };
        struct { unsigned char pad_to_30[0x30]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_34[0x34]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.cycleState = (BattleFxStarCardCycle *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = frame.cycleState->angle;
    frame.rotX.vy = 0;
    frame.rotX.vz = frame.cycleState->phase;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.cycleState->baseA;
    frame.rotMatrix.t[1] = frame.cycleState->baseB;
    frame.rotMatrix.t[2] = frame.cycleState->baseC;
    frame.scale.vz = frame.cycleState->scale;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    frame.modelIndex = frame.cycleState->step % 0xA;
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(((int *)&g_BattleSpecialStarCardCyclingModelIndices)[frame.modelIndex], g_BattleSpecialStarCardFadeStep);
    frame.modelDraw->flags = 0x28;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    if (frame.cycleState->step < 8) {
        frame.modelDraw->fadeLevel = frame.cycleState->step * 0x18;
    } else if (frame.cycleState->step >= 0x2E) {
        frame.modelDraw->fadeLevel = 0xC0 - (frame.cycleState->step - 0x2E) * 0x18;
    } else {
        frame.modelDraw->fadeLevel = 0xC0;
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.cycleState->step >= 0x36) {
        frame.cycleState->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.cycleState->angle += frame.cycleState->angleVelocity;
    frame.cycleState->phase += frame.cycleState->phaseVelocity;
    (frame.cycleState->step)++;
}

/*
 * Draws CLUB2 model 4 with flag 0x28, uniform scale and X/Z rotation. Intensity follows a six-
 * age pulse: 0, 42, 84, 128, 86, 44. Retires at age 54 before checking pause; unpaused calls
 * advance both rotations and age.
 */
// FUNCTION: FF7 0x004f419f
void __cdecl BattleFx_SpecialStarCardPulsingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[68];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxStarCardPulsingModel *pulseState; };
        struct { unsigned char pad_to_24[0x24]; int pulsePhase; };
        struct { unsigned char pad_to_28[0x28]; SVECTOR rotX; };
        struct { unsigned char pad_to_30[0x30]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_34[0x34]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.pulseState = (BattleFxStarCardPulsingModel *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = frame.pulseState->angle;
    frame.rotX.vy = 0;
    frame.rotX.vz = frame.pulseState->pulseAngle;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.pulseState->posX;
    frame.rotMatrix.t[1] = frame.pulseState->posY;
    frame.rotMatrix.t[2] = frame.pulseState->posZ;
    frame.scale.vz = frame.pulseState->scale;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(4, g_BattleSpecialStarCardFadeStep);
    frame.modelDraw->flags = 0x28;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0x80;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    frame.pulsePhase = frame.pulseState->timer % 6;
    if (frame.pulsePhase < 3) {
        frame.modelDraw->fadeLevel = frame.pulsePhase * 0x2A;
    } else {
        frame.modelDraw->fadeLevel = 0x80 - (frame.pulsePhase - 3) * 0x2A;
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.pulseState->timer >= 0x36) {
        frame.pulseState->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.pulseState->angle += frame.pulseState->angleStep;
    frame.pulseState->pulseAngle += frame.pulseState->pulseAngleStep;
    (frame.pulseState->timer)++;
}
} /* extern "C" */
