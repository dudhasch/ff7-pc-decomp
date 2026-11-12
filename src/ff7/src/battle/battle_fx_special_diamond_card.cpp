// src/ff7/src/battle/battle_fx_special_diamond_card.cpp
/* Enemy Special Effect [idx 49]: DiamondCard (JP: dia1 / ダイア1 (Diamond 1), asset: "SPECIAL/DIA1") — Diamond (0x156, Joker) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect DIA1: loads its resources from a table, runs a timeline of model
//   callbacks and sounds, and runs the spinning, oscillating and accelerating model tasks, an
//   impact timeline with bouncing models, and sprite-drawing tasks.

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
#include "ff7/types/battle_special.h"
extern "C" {

#pragma function(memcpy)
// clang-format off
// GLOBAL: FF7 0x0083b790
int g_BattleSpecialDiamondCardResourceDefs = 0x001900B4;
int g_BattleSpecialDiamondCardResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialDiamondCardResourceNames + 0x0)
};
int g_BattleSpecialDiamondCardResourceDefsDataTail[28] = {
    0x0, 0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleFxSpecialDiamondCardResourceNames + 0xc), 0x0,
    0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialDiamondCardResourceNames + 0x18), (int)((char *)&g_BattleFxSpecialDiamondCardResourceNames + 0x24), 0x0,
    0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialDiamondCardResourceNames + 0x30), (int)((char *)&g_BattleFxSpecialDiamondCardResourceNames + 0x3c), 0x0, 0x0,
    0x6400b4, (int)((char *)&g_BattleFxSpecialDiamondCardResourceNames + 0x48), 0x0, 0x0, 0x0, 0x6400b1,
    (int)((char *)&g_BattleFxSpecialDiamondCardResourceNames + 0x54), 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x0083b808
struct VECTOR g_BattleSpecialDiamondCardNegativeZUnitVector = {0, 0, -0x1000, 0};
// GLOBAL: FF7 0x0083b818
struct BattleFxDiamondCardAxisSprite g_BattleSpecialDiamondCardAxisAngleMatrix = {{0, 0x10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 0};
// GLOBAL: FF7 0x0083b830
struct BattleFxDiamondCardSpriteState g_BattleSpecialDiamondCardColorTable1 = {0x80, 0x80, 0x80, 0x2e, 0, {0}, 0, 0};
// GLOBAL: FF7 0x0083b840
struct BattleFxDiamondCardSpriteColor g_BattleSpecialDiamondCardColorTable2 = {0x68, 0x68, 0x68, 0x2e, 0, {0}};
unsigned char g_BattleFxSpecialDiamondCardResourceNames[96] =
    "CARD1.RSD\0\0\0"
    "CARD1.RSD\0\0\0"
    "HIKARI1.TIM\0"
    "HIKARI1.SPT\0"
    "HIT1.TIM\0\0\0\0"
    "HIT1.SPT\0\0\0\0"
    "KOIN.RSD\0\0\0\0"
    "KOIN.RSD";
// GLOBAL: FF7 0x00b491e8
int g_BattleSpecialDiamondCardFirstTargetIdx = 0;
// GLOBAL: FF7 0x00b491ec
int g_BattleSpecialDiamondCardBillboardCursor = 0;
// GLOBAL: FF7 0x00b491f0
BattleFxDiamondCardBillboard g_BattleSpecialDiamondCardBillboards[200] = {0};
// GLOBAL: FF7 0x00b49830
VECTOR g_BattleSpecialDiamondCardTransformedVector1 = {0};
// GLOBAL: FF7 0x00b49840
struct BattleFxDiamondCardTargetPos g_BattleSpecialDiamondCardTargetPos = {0};
// GLOBAL: FF7 0x00b49848
VECTOR g_BattleSpecialDiamondCardTransformedVector2 = {0};
// GLOBAL: FF7 0x00b49858
int g_BattleSpecialDiamondCardTargetMask = 0;
// GLOBAL: FF7 0x00b4985c
int g_BattleSpecialDiamondCardActorIdx = 0;
// GLOBAL: FF7 0x00b49860
struct GfxTableCursor *g_BattleSpecialDiamondCardLoadedResTable = 0;
unsigned char g_bssPad_00b49864[4] = {0};
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

// When the engine context exists and g_BattleSpecialDiamondCardLoadedResTable is empty, builds SPECIAL/DIA1 with a default-
// duration record and creates resource type six using g_BattleSpecialDiamondCardResourceDefs.
// FUNCTION: FF7 0x004f06f0
void __cdecl BattleFx_SpecialLoadResDiamondCard(void)
{
    GfxModelTableBank *stage;
    GfxResourceRequest requestSlot;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceTable;

    if (g_pBattle3dScene != 0 && g_BattleSpecialDiamondCardLoadedResTable == 0) {
        kind = 6;
        resourceTable = (int)&g_BattleSpecialDiamondCardResourceDefs;
        stage = (GfxModelTableBank *)g_pBattle3dScene->modelTableBank;
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        ((BattleFxDiamondCardResource *)pRequest)->groupTag = 0;
        // STRING: FF7 0x0083b8a8
        ((BattleFxDiamondCardResource *)pRequest)->resourcePath = (int)BattleFx_BuildResourcePath("SPECIAL/DIA1", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceTable, pRequest, &g_BattleSpecialDiamondCardLoadedResTable, stage);
    }
}

/*
 * Forwarder: calls BattleFx_SpecialLoadResDiamondCard() with no arguments, then BattleFx_SpecialBeginDiamondCard(targetMask, actorIdx).
 */
// FUNCTION: FF7 0x004f0780
void __cdecl BattleFx_SpecialEnterDiamondCard(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResDiamondCard();
    BattleFx_SpecialBeginDiamondCard(targetMask, actorIdx);
}

/*
 * Allocates and saves a draw-list-selection slot, stores targetMask as the actor mask and records its
 * first selected low-ten-bit actor when one exists, retaining the previous actor value when none
 * exists. Stores actorIdx and allocates BattleFx_SpecialDiamondCardSpawnTimeline.
 */
// FUNCTION: FF7 0x004f079a
void __cdecl BattleFx_SpecialBeginDiamondCard(int targetMask, int actorIdx)
{
    int bitIdx;
    int count;

    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    g_BattleSpecialDiamondCardTargetMask = targetMask;
    bitIdx = 0;
    for (count = 0; bitIdx < 0xA; bitIdx++) {
        if ((targetMask & 1 << bitIdx) != 0) {
            count++;
            if (count == 1) {
                g_BattleSpecialDiamondCardFirstTargetIdx = bitIdx;
            }
        }
    }
    g_BattleSpecialDiamondCardActorIdx = actorIdx;
    Battle_AllocateResourceSlot(BattleFx_SpecialDiamondCardSpawnTimeline);
}

/*
 * Retires the current slot, reads the first target actor's bone position and gives it to a new
 * BattleFx_SpecialDiamondCardTimelineTask slot. Calls BattleFx_SpecialDiamondCardBuildFacingVectors, then allocates
 * BattleFx_SpecialDiamondCardDelayedImpactTask with the same position; no pause guard appears.
 */
// FUNCTION: FF7 0x004f082b
void __cdecl BattleFx_SpecialDiamondCardSpawnTimeline(void)
{
    struct {
        BattleTaskSlot *slot;
        BattleBonePosCopy pos;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = BATTLE_CURRENT_TASK_SLOT;
    frame.slot->ownerSlot = BATTLE_TASK_FREE;
    Battle_GetBoneWorldPos(g_BattleSpecialDiamondCardFirstTargetIdx,
                           g_BattleActorRecords[g_BattleSpecialDiamondCardFirstTargetIdx].targetBone,
                           (short *)&frame.pos);
    frame.slot = BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDiamondCardTimelineTask);
    *(BattleBonePosCopy *)frame.slot->data = frame.pos;
    BattleFx_SpecialDiamondCardBuildFacingVectors();
    frame.slot = BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDiamondCardDelayedImpactTask);
    *(BattleBonePosCopy *)frame.slot->data = frame.pos;
}

/*
 * Runs an unpaused sequence that creates a positioned callback and positional sound at age 2, a
 * vertically moving callback and centered sound at age 24, then at age 72 samples the target
 * attachment and creates an axis-angle-oriented moving callback toward its X/Z position with
 * positional sound. Uses the normalized distance square root divided by 12.
 * Retires at age 90 before incrementing age.
 */
// FUNCTION: FF7 0x004f08cf
void __cdecl BattleFx_SpecialDiamondCardTimelineTask(void)
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
        frame.slot = (BattleFxDiamondCardSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDiamondCardSpinningModelTask);
        *(Int64Pair *)((char *)frame.slot + 4) = *(Int64Pair *)&g_BattleSpecialDiamondCardTargetPos;
        frame.distance = g_BattleActorRecords[g_BattleSpecialDiamondCardActorIdx].posY - g_BattleSpecialDiamondCardTargetPos.posY;
        if (frame.distance < 0) {
            frame.distance *= -1;
        }
        frame.slot->distance = frame.distance;
        frame.slot->velocityY = 0xFED4;
        frame.slot->scale = 0x2000;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleSpecialDiamondCardTargetPos), 0x1D5);
    }
    if (frame.current->step == 0x18) {
        frame.slot = (BattleFxDiamondCardSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDiamondCardOscillatingModelTask);
        frame.slot->posY = 0xFEF6;
        frame.slot->velocityY = 0x21;
        frame.slot->angle = 0;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, 0x40, 0x1D6);
    }
    if (frame.current->step == 0x48) {
        Battle_GetBoneWorldPos(g_BattleSpecialDiamondCardFirstTargetIdx, g_BattleActorRecords[g_BattleSpecialDiamondCardFirstTargetIdx].targetBone, &frame.current->posX);
        frame.vector[0] = frame.current->posX - g_BattleCameraPosition.cameraPosX;
        frame.vector[1] = 0;
        frame.vector[2] = frame.current->posZ - g_BattleCameraPosZ;
        frame.distance = NormalizeVectorFixed(&frame.vector[0], &frame.vector[0]);
        frame.distance = SqrtToInt(frame.distance) / 0xC;
        frame.angle = ComputeAngleBetweenVectors(&g_BattleSpecialDiamondCardNegativeZUnitVector, (VECTOR *)(&frame.vector[0]), (VECTOR *)(&frame.axis[0]));
        Gte_BuildAxisAngleMatrix(frame.angle, (PsxMatrix *)(&frame.matrix[0]), (VECTOR *)(&frame.axis[0]));
        TransformVectorToInt32((PsxMatrix *)(&frame.matrix[0]), &g_BattleSpecialDiamondCardNegativeZUnitVector, &frame.vector[0]);
        if (frame.axis[1] < 0) {
            frame.angle *= -1;
        }
        frame.slot = (BattleFxDiamondCardSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDiamondCardAcceleratingModelTask);
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

/*
 * Draws a uniformly scaled model from resource g_BattleSpecialDiamondCardLoadedResTable using the stored
 * actor heading, an age-driven axis rotation and a rotated vertical offset; the angle advances
 * through age 8 then remains a quarter turn. Ages at least 18 retire before checking pause. From
 * unpaused age 8 onward, adds vertical velocity to Y and increases that velocity by its sixth, then
 * increments age.
 */
// FUNCTION: FF7 0x004f0ba0
void __cdecl BattleFx_SpecialDiamondCardSpinningModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        BattleFxDiamondCardSpinRecord *spinRecord;
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

    frame.spinRecord = (BattleFxDiamondCardSpinRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.spinRecord->step <= 8) {
        frame.axisAngle = frame.spinRecord->step << 7;
    } else {
        frame.axisAngle = 0x400;
    }
    frame.targetRotX.vx = 0;
    frame.targetRotX.vy = g_BattleSpecialDiamondCardTargetPos.yaw;
    frame.targetRotX.vz = 0;
    RotMatrixXYZ(&frame.targetRotX, &frame.modelMatrix);
    Gte_BuildAxisAngleMatrix(-frame.axisAngle, &frame.axisMatrix, (VECTOR *)&g_BattleSpecialDiamondCardAxisAngleMatrix);
    ConcatMatrixInPlace(&frame.modelMatrix, &frame.axisMatrix);
    frame.rotOffsetX.vx = 0;
    frame.rotOffsetX.vy = frame.spinRecord->offsetDistance;
    frame.rotOffsetX.vz = 0;
    TransformVectorToInt16(&frame.axisMatrix, &frame.rotOffsetX, &frame.rotOffsetX.vx);
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0x800;
    RotMatrixXYZ(&frame.rotX, &frame.modelMatrix);
    ConcatMatrixInPlace(&frame.axisMatrix, &frame.modelMatrix);
    frame.modelMatrix.t[0] = frame.spinRecord->posX + frame.rotOffsetX.vx;
    frame.modelMatrix.t[1] = frame.spinRecord->posY + frame.rotOffsetX.vy;
    frame.modelMatrix.t[2] = frame.spinRecord->posZ + frame.rotOffsetX.vz;
    frame.matrixScale.vz = frame.spinRecord->scale;
    frame.matrixScale.vy = frame.matrixScale.vz;
    frame.matrixScale.vx = frame.matrixScale.vy;
    ScaleMatrix(&frame.modelMatrix, &frame.matrixScale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    frame.drawRecord = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.drawRecord->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialDiamondCardLoadedResTable);
    frame.drawRecord->flags = 0;
    frame.drawRecord->uvScroll = 0;
    frame.drawRecord->fadeLevel = 0x80;
    frame.drawRecord->stateFlags = 0x20;
    frame.drawRecord->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.drawRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
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

/*
 * Draws the resource g_BattleSpecialDiamondCardLoadedResTable model at fixed X/Z and slot Y minus 40 with a quarter-turn X
 * rotation and configurable Y rotation. Retires at age 48 before checking pause. Unpaused ages
 * below 8 move Y with damped velocity clamped to at least 1; ages 8 through 39 set Y to a sine
 * oscillation and rotate during ages 22 through 33; later ages move Y with velocity increased by a
 * quarter.
 */
// FUNCTION: FF7 0x004f0df5
void __cdecl BattleFx_SpecialDiamondCardOscillatingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        BattleFxOscillatingModel *oscModel;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_18[0x18]; int sineAngle; };
        struct { unsigned char pad_to_1C[0x1C]; PsxMatrix angleMatrix; };
        struct { unsigned char pad_to_3C[0x3C]; PsxMatrix modelMatrix; };
        struct { unsigned char pad_to_5C[0x5C]; SVECTOR angleRotX; };
        struct { unsigned char pad_to_64[0x64]; BattleFxModelDraw *drawRecord; };
    } frame;
#pragma pack(pop)

    frame.oscModel = (BattleFxOscillatingModel *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0x400;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.modelMatrix);
    frame.modelMatrix.t[0] = 0;
    frame.modelMatrix.t[1] = frame.oscModel->offset - 0x28;
    frame.modelMatrix.t[2] = 0x380;
    frame.angleRotX.vx = 0;
    frame.angleRotX.vy = frame.oscModel->angle;
    frame.angleRotX.vz = 0;
    RotMatrixXYZ(&frame.angleRotX, &frame.angleMatrix);
    ConcatMatrixInPlace(&frame.angleMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    frame.drawRecord = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.drawRecord->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialDiamondCardLoadedResTable);
    frame.drawRecord->flags = 0;
    frame.drawRecord->uvScroll = 0;
    frame.drawRecord->fadeLevel = 0x80;
    frame.drawRecord->stateFlags = 0x20;
    frame.drawRecord->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.drawRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
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
        frame.sineAngle = frame.oscModel->phase - 8 << 7;
        frame.oscModel->offset = SinFixed(frame.sineAngle) * 0x18 >> 0xC;
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
 * Draws a uniformly scaled model from resource g_BattleSpecialDiamondCardLoadedResTable at the slot position with configured Y
 * rotation and half-turn Z rotation; from age 12 switches to model 1, enables flag 8 and reduces
 * intensity by 16 per age. Retires at age 20 before checking pause. Unpaused age 10 triggers a
 * target operation selected by screen pan above 64 and positional sound; unpaused calls advance
 * X/Z and increase their velocities by a sixteenth before advancing age.
 */
// FUNCTION: FF7 0x004f1030
void __cdecl BattleFx_SpecialDiamondCardAcceleratingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        BattleFxSpecialAcceleratingSlot *accelSlot;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_18[0x18]; int screenPan; };
        struct { unsigned char pad_to_3C[0x3C]; PsxMatrix modelMatrix; };
        struct { unsigned char pad_to_64[0x64]; BattleFxModelDraw *drawRecord; };
        struct { unsigned char pad_to_68[0x68]; VECTOR matrixScale; };
    } frame;
#pragma pack(pop)

    frame.accelSlot = (BattleFxSpecialAcceleratingSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.accelSlot->angleY;
    frame.rotX.vz = 0x800;
    RotMatrixXYZ(&frame.rotX, &frame.modelMatrix);
    frame.modelMatrix.t[0] = frame.accelSlot->posA;
    frame.modelMatrix.t[1] = frame.accelSlot->posY;
    frame.modelMatrix.t[2] = frame.accelSlot->posB;
    frame.matrixScale.vz = frame.accelSlot->scale;
    frame.matrixScale.vy = frame.matrixScale.vz;
    frame.matrixScale.vx = frame.matrixScale.vy;
    ScaleMatrix(&frame.modelMatrix, &frame.matrixScale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    frame.drawRecord = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.drawRecord->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialDiamondCardLoadedResTable);
    frame.drawRecord->flags = 0;
    frame.drawRecord->uvScroll = 0;
    frame.drawRecord->fadeLevel = 0x80;
    frame.drawRecord->stateFlags = 0x20;
    frame.drawRecord->texturePageBits = 0;
    if (frame.accelSlot->timer >= 0xC) {
        frame.drawRecord->flags = 8;
        frame.drawRecord->fadeLevel = 0x80 - (frame.accelSlot->timer - 0xC << 4);
        frame.drawRecord->modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleSpecialDiamondCardLoadedResTable);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.drawRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.accelSlot->timer >= 0x14) {
        frame.accelSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.accelSlot->timer == 0xA) {
        frame.screenPan = Battle_ComputeScreenPanFromPosition(&frame.accelSlot->posA);
        if (frame.screenPan > 0x40) {
            BattleFx_HitMarkSpawnAtActorBone(g_BattleSpecialDiamondCardFirstTargetIdx, 0x118);
        } else {
            BattleFx_HitMarkSpawnAtActorBone(g_BattleSpecialDiamondCardFirstTargetIdx, 0x18);
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
 * Reads the alternate bone position of the g_BattleSpecialDiamondCardActorIdx actor into
 * g_BattleSpecialDiamondCardTargetPos, lowers Y by 224 and stores the actor's facing as yaw. Builds a Y
 * rotation from that facing and transforms g_BattleSpecialDiamondCardNegativeZUnitVector and the axis
 * vector at the head of g_BattleSpecialDiamondCardAxisAngleMatrix into g_BattleSpecialDiamondCardTransformedVector2
 * and g_BattleSpecialDiamondCardTransformedVector1.
 */
// FUNCTION: FF7 0x004f1292
void __cdecl BattleFx_SpecialDiamondCardBuildFacingVectors(void)
{
    struct {
        int matrix[8];
        short rot[4];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    Battle_GetBoneWorldPos(g_BattleSpecialDiamondCardActorIdx,
                           g_BattleActorRecords[g_BattleSpecialDiamondCardActorIdx].altBone,
                           &g_BattleSpecialDiamondCardTargetPos.posX);
    g_BattleSpecialDiamondCardTargetPos.posY -= 0xe0;
    g_BattleSpecialDiamondCardTargetPos.yaw = g_BattleActorRecords[g_BattleSpecialDiamondCardActorIdx].yaw;
    frame.rot[0] = 0;
    frame.rot[1] = g_BattleSpecialDiamondCardTargetPos.yaw;
    frame.rot[2] = 0;
    RotMatrixXYZ((void *)frame.rot, (void *)frame.matrix);
    TransformVectorToInt32((PsxMatrix *)frame.matrix, &g_BattleSpecialDiamondCardNegativeZUnitVector, &g_BattleSpecialDiamondCardTransformedVector2);
    TransformVectorToInt32((PsxMatrix *)frame.matrix, &g_BattleSpecialDiamondCardAxisAngleMatrix, &g_BattleSpecialDiamondCardTransformedVector1);
}

/*
 * Waits for the frame counter to reach 0x5A, then spawns the impact timeline and retires the task.
 * Returns without advancing the counter while g_BattleIsPaused is set.
 */
// FUNCTION: FF7 0x004f133f
void __cdecl BattleFx_SpecialDiamondCardDelayedImpactTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0x5A) {
        BattleFx_SpecialDiamondCardSpawnImpactTimeline(g_BattleSpecialDiamondCardFirstTargetIdx, g_BattleSpecialDiamondCardActorIdx);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/*
 * Starts BattleFx_SpecialDiamondCardImpactTimelineTask at the actor's target bone position, with its Y
 * lowered to the alternate bone's Y minus 0x200 and its jitter range set to the actor's radius plus
 * 0x300, clamped to 0x76C. Resets the billboard cursor and plays the impact sound at the position.
 */
// FUNCTION: FF7 0x004f13a3
void __cdecl BattleFx_SpecialDiamondCardSpawnImpactTimeline(int actorIndex, int casterIdx)
{
    BattleFxDiamondCardImpactTimeline *effectPointer;
    int h;
    short positionVec[3];

    effectPointer = (BattleFxDiamondCardImpactTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDiamondCardImpactTimelineTask);
    Battle_GetBoneWorldPos(actorIndex, g_BattleActorRecords[actorIndex].targetBone, &effectPointer->posX);
    Battle_GetBoneWorldPos(actorIndex, g_BattleActorRecords[actorIndex].altBone, positionVec);
    effectPointer->posY = positionVec[1] - 0x200;
    h = g_BattleActorRecords[actorIndex].radius + 0x300;
    if (h > 0x76C) {
        h = 0x76C;
    }
    effectPointer->jitter = h;
    g_BattleSpecialDiamondCardBillboardCursor = 0;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&effectPointer->posX), 0x1DB);
}

/*
 * When unpaused, ages 4 through 10 create six randomly directed bouncing model callbacks per call;
 * ages 1 through 13 independently create a two-point sprite callback, and ages 8 through 19 create
 * a four-point callback using the wrapping 200-record array. The latter random extents divide by
 * twice the stored radius and by negative Y without zero checks. Age 35 allocates a typed target
 * record; age 40 retires before incrementing age.
 */
// FUNCTION: FF7 0x004f1479
void __cdecl BattleFx_SpecialDiamondCardImpactTimelineTask(void)
{
    int idx;
    int cursor;
    int jitterSpan;
    VECTOR dirXPart;
    int baseScale;
    int scaleRoll;
    BattleFxDiamondCardImpact *selfPtr;
    BattleFxDiamondCardImpactSlot *child;
    int invBase;
    int mag;

    if (g_BattleIsPaused != 0) {
        return;
    }
    selfPtr = (BattleFxDiamondCardImpact *)BATTLE_CURRENT_TASK_SLOT;
    if (selfPtr->step >= 4 && selfPtr->step <= 0xA) {
        for (idx = 0; idx < 6; idx++) {
            child = (BattleFxDiamondCardImpactSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDiamondCardBouncingModelTask);
            *(Int64Pair *)((char *)child + 4) = *(Int64Pair *)((char *)selfPtr + 4);
            dirXPart.vx = rand() % 0x1000 - 0x800;
            dirXPart.vy = 0;
            dirXPart.vz = rand() % 0x1000 - 0x800;
            NormalizeVectorFixed(&dirXPart, &dirXPart);
            mag = rand() % 0x50 + 0x3C;
            child->velocityX = mag * dirXPart.vx >> 0xC;
            child->velocityY = -(rand() % 0x3C + 0xA5);
            child->velocityZ = mag * dirXPart.vz >> 0xC;
            child->gravity = rand() % 0x16 + 0x1A;
            cursor = rand() % 6 - 3;
            if (cursor == 0) {
                cursor = 3;
            }
            child->speed = (rand() % 0x48 + 0x54) * cursor;
            child->angleX = child->speed << 1;
            child->lateralSpeed = (rand() % 0x43 + 0x2E) * cursor;
            child->angleZ = child->lateralSpeed;
            child->scaleOrIndex = 0x492;
        }
    }
    if (selfPtr->step >= 1 && selfPtr->step <= 0xD) {
        cursor = g_BattleSpecialDiamondCardBillboardCursor;
        child = (BattleFxDiamondCardImpactSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDiamondCardDrawTwoSpritesTask);
        child->scaleOrIndex = cursor;
        mag = 0x100;
        jitterSpan = mag << 1;
        baseScale = 0x1400;
        for (idx = 0; idx < 2; idx++) {
            g_BattleSpecialDiamondCardBillboards[cursor].posX = selfPtr->baseA + (rand() % jitterSpan - mag);
            g_BattleSpecialDiamondCardBillboards[cursor].posY = selfPtr->baseB + (rand() % jitterSpan - mag);
            g_BattleSpecialDiamondCardBillboards[cursor].posZ = selfPtr->baseC + (rand() % jitterSpan - mag);
            scaleRoll = rand() % 0x1000 + 0xA00;
            g_BattleSpecialDiamondCardBillboards[cursor].scale = baseScale * scaleRoll >> 0xC;
            cursor++;
            if (cursor >= 0xC8) {
                cursor = 0;
            }
        }
        g_BattleSpecialDiamondCardBillboardCursor = cursor;
    }
    if (selfPtr->step >= 8 && selfPtr->step <= 0x13) {
        cursor = g_BattleSpecialDiamondCardBillboardCursor;
        child = (BattleFxDiamondCardImpactSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialDiamondCardDrawFourSpritesTask);
        child->scaleOrIndex = cursor;
        mag = selfPtr->jitter;
        jitterSpan = mag << 1;
        baseScale = 0x1400;
        invBase = -selfPtr->baseB;
        invBase = invBase;
        for (idx = 0; idx < 4; idx++) {
            g_BattleSpecialDiamondCardBillboards[cursor].posX = selfPtr->baseA + (rand() % jitterSpan - mag);
            g_BattleSpecialDiamondCardBillboards[cursor].posY = selfPtr->baseB + rand() % invBase;
            g_BattleSpecialDiamondCardBillboards[cursor].posZ = selfPtr->baseC + (rand() % jitterSpan - mag);
            scaleRoll = rand() % 0xE00 + 0x800;
            g_BattleSpecialDiamondCardBillboards[cursor].scale = baseScale * scaleRoll >> 0xC;
            cursor++;
            if (cursor >= 0xC8) {
                cursor = 0;
            }
        }
        g_BattleSpecialDiamondCardBillboardCursor = cursor;
    }
    if (selfPtr->step == 0x23) {
        Battle_PostActorHitOrHurtMessage(g_BattleSpecialDiamondCardFirstTargetIdx);
    }
    if (selfPtr->step >= 0x28) {
        selfPtr->ownerSlot = BATTLE_TASK_FREE;
    }
    (selfPtr->step)++;
}

/*
 * Draws four consecutive billboard points of the ring, starting at the task's record index and
 * wrapping at 200, each as a sprite from table entry 2 scaled by the point's scale. Retires the
 * task from frame 2 and advances the frame unless g_BattleIsPaused is set.
 */
// FUNCTION: FF7 0x004f1906
void __cdecl BattleFx_SpecialDiamondCardDrawFourSpritesTask(void)
{
    int idx;
    int cursor;
    BattleDiamondCardTwoSpritesTask *task;

    task = (BattleDiamondCardTwoSpritesTask *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialDiamondCardColorTable1.frame = task->frameIndex;
    cursor = task->recordIndex;
    g_BattleSpecialDiamondCardAxisAngleMatrix.frameTable = Gfx_ModelTableGetLoadedRecord(2, g_BattleSpecialDiamondCardLoadedResTable);
    g_BattleSpecialDiamondCardAxisAngleMatrix.spriteData = Gfx_ModelTableGetSpriteObject(2, g_BattleSpecialDiamondCardLoadedResTable);
    for (idx = 0; idx < 4; idx++) {
        BattleFx_InstallBillboardMatrix(&g_BattleSpecialDiamondCardBillboards[cursor].posX, g_BattleSpecialDiamondCardBillboards[cursor].scale, -(g_BattleSpecialDiamondCardBillboards[cursor].scale >> 3));
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw((char *)&g_BattleSpecialDiamondCardAxisAngleMatrix + 0x10, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
        cursor++;
        if (cursor >= 0xC8) {
            cursor = 0;
        }
    }
    if (task->frameIndex >= 2) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (task->frameIndex)++;
}

/*
 * Draws two consecutive billboard points of the ring, starting at the task's record index and
 * wrapping at 200, each as a sprite from table entry 3 scaled by the point's scale. Retires the
 * task from frame 7 and advances the frame unless g_BattleIsPaused is set.
 */
// FUNCTION: FF7 0x004f1a20
void __cdecl BattleFx_SpecialDiamondCardDrawTwoSpritesTask(void)
{
    int ii;
    int c;
    BattleDiamondCardTwoSpritesTask *rec;

    rec = (BattleDiamondCardTwoSpritesTask *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialDiamondCardColorTable2.frame = rec->frameIndex;
    ii = rec->recordIndex;
    g_BattleSpecialDiamondCardColorTable1.frameTable = Gfx_ModelTableGetLoadedRecord(3, g_BattleSpecialDiamondCardLoadedResTable);
    g_BattleSpecialDiamondCardColorTable1.spriteData = Gfx_ModelTableGetSpriteObject(3, g_BattleSpecialDiamondCardLoadedResTable);
    for (c = 0; c < 2; c++) {
        BattleFx_InstallBillboardMatrix(&g_BattleSpecialDiamondCardBillboards[ii].posX, g_BattleSpecialDiamondCardBillboards[ii].scale, -(g_BattleSpecialDiamondCardBillboards[ii].scale >> 3));
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw((char *)&g_BattleSpecialDiamondCardColorTable1 + 0x8, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
        ii++;
        if (ii >= 0xC8) {
            ii = 0;
        }
    }
    if (rec->frameIndex >= 7) {
        rec->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (rec->frameIndex)++;
}

/*
 * Draws resource model 4 at the slot position with X/Z rotation and uniform scale; from age 24
 * switches to model 5 with flag 8 and intensity declining by 21 per age. Retires at age 30 before
 * checking pause. Unpaused calls advance X/Z, integrate Y and damp acceleration while Y is
 * negative, otherwise reduce horizontal velocity and randomize a rebound velocity/acceleration;
 * both rotation angles and age then advance.
 */
// FUNCTION: FF7 0x004f1b3a
void __cdecl BattleFx_SpecialDiamondCardBouncingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        PsxMatrix modelMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxDiamondCardBounceModel *bounceRecord; };
        struct { unsigned char pad_to_24[0x24]; SVECTOR rotX; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxModelDraw *drawRecord; };
        struct { unsigned char pad_to_30[0x30]; VECTOR matrixScale; };
    } frame;
#pragma pack(pop)

    frame.bounceRecord = (BattleFxDiamondCardBounceModel *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = frame.bounceRecord->angleX;
    frame.rotX.vy = 0;
    frame.rotX.vz = frame.bounceRecord->angleZ;
    RotMatrixXYZ(&frame.rotX, &frame.modelMatrix);
    frame.modelMatrix.t[0] = frame.bounceRecord->posX;
    frame.modelMatrix.t[1] = frame.bounceRecord->posY;
    frame.modelMatrix.t[2] = frame.bounceRecord->posZ;
    frame.matrixScale.vz = frame.bounceRecord->scale;
    frame.matrixScale.vy = frame.matrixScale.vz;
    frame.matrixScale.vx = frame.matrixScale.vy;
    ScaleMatrix(&frame.modelMatrix, &frame.matrixScale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    frame.drawRecord = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.drawRecord->modelGroup = Gfx_ModelTableGetRenderObject(4, g_BattleSpecialDiamondCardLoadedResTable);
    frame.drawRecord->flags = 0;
    frame.drawRecord->uvScroll = 0;
    frame.drawRecord->fadeLevel = 0x80;
    frame.drawRecord->stateFlags = 0x20;
    frame.drawRecord->texturePageBits = 0;
    if (frame.bounceRecord->step >= 0x18) {
        frame.drawRecord->flags = 8;
        frame.drawRecord->fadeLevel = 0x80 - (frame.bounceRecord->step - 0x18) * 0x15;
        frame.drawRecord->modelGroup = Gfx_ModelTableGetRenderObject(5, g_BattleSpecialDiamondCardLoadedResTable);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.drawRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.bounceRecord->step >= 0x1E) {
        frame.bounceRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.bounceRecord->posX += frame.bounceRecord->velocityX;
    frame.bounceRecord->posZ += frame.bounceRecord->velocityZ;
    if (frame.bounceRecord->posY < 0) {
        frame.bounceRecord->posY += frame.bounceRecord->velocityY;
        frame.bounceRecord->velocityY += frame.bounceRecord->gravity;
        frame.bounceRecord->gravity -= (short)(frame.bounceRecord->gravity >> 4);
    } else {
        frame.bounceRecord->velocityX = frame.bounceRecord->velocityX / 3 << 1;
        frame.bounceRecord->velocityZ = frame.bounceRecord->velocityZ / 3 << 1;
        frame.bounceRecord->velocityY = -((frame.bounceRecord->velocityY >> 2) + rand() % 0x2D);
        frame.bounceRecord->gravity = (frame.bounceRecord->gravity >> 3) + rand() % 0x19;
        frame.bounceRecord->posY += frame.bounceRecord->velocityY;
    }
    frame.bounceRecord->angleX += frame.bounceRecord->angleSpeed;
    frame.bounceRecord->angleZ += frame.bounceRecord->angleSpeedZ;
    (frame.bounceRecord->step)++;
}
} /* extern "C" */
