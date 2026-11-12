// src/ff7/src/battle/battle_special_energy_effects.cpp
/* Enemy Special Attack [101]: Energy (orig. SPECIAL/ENERGY) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Energy spell. It loads its resources, spawns a per-target
//   effect, and runs the timeline, bouncing and expanding sprites, spinning and stretching models,
//   beam models, and animated and shrinking sprite tasks.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// GLOBAL: FF7 0x00898e68
unsigned int g_BattleSpecialEnergyResourceDefs[65] = {0x001900b1, (unsigned int)&g_BattleSpecialEnergyModelNames1, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialEnergyModelNames1[12], 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialEnergyModelNames2, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialEnergyModelNames2[12], 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialEnergyModelNames3, 0x00000000, 0x00000000, 0x00000000, 0x003200b1, (unsigned int)&g_BattleSpecialEnergyModelNames3[12], 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialEnergyModelNames4, 0x00000000, 0x00000000, 0x00000000, 0x003200b1, (unsigned int)&g_BattleSpecialEnergyModelNames5, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialEnergyModelNames5[12], 0x00000000, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecialEnergyTextureName1, (unsigned int)&g_BattleSpecialEnergySpriteName1, 0x00000000, 0x00000000, 0x00192a39, 0x00000000, (unsigned int)&g_BattleSpecialEnergyTextureSpriteNames1, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecialEnergyTextureSpriteNames1[12], (unsigned int)&g_BattleSpecialEnergyTextureSpriteNames1[28], 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecialEnergyTextureName2, (unsigned int)&g_BattleSpecialEnergySpriteName2, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x00898f70
BattleSpecialEnergySpriteConfig1 g_BattleSpecialEnergySpriteConfig1 = {0, 0, {0x40, 0x40, 0x40, 0x2e}};
// GLOBAL: FF7 0x00898f88
BattleSpecialEnergyModelDescriptors g_BattleSpecialEnergyModelDescriptors = {-128, -128, -128, 0x2c, 0, {0}, 0, {0x88}, 0, {0x20}, 0, 0, {0}, 0x80, {0x20}, 0, 0, {0x80, 0x80, 0x80, 0x2e}};
// GLOBAL: FF7 0x00898fc8
BattleSpecialEnergySpriteRequest1 g_BattleSpecialEnergySpriteRequest1 = {-128, -128, -128, {0x2e}, 0, {0}, 0, {0xa8}, 0x80};
unsigned char g_rawData_00898fdc[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00898fe0
int g_BattleSpecialEnergyScaleFixed12_1 = 64512;
// GLOBAL: FF7 0x00898fe8
struct BattleSpecialEnergyWaveSpriteConfig2 g_BattleSpecialEnergySpriteConfig2 = {{0, 0x20, 0, 0, 0, 0x20, 0, 0, 0, 0x20, 0, 0, 0, 0, 0, 0}, 0, {0x28, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}, 0, 0};
// GLOBAL: FF7 0x00899010
struct BattleSpecialEnergyWaveSpriteRequest2 g_BattleSpecialEnergySpriteRequest2 = {-128, -128, -128, {0x2e}, 0, {0}};
// GLOBAL: FF7 0x00899018
int g_BattleSpecialEnergyScaleFixed12_2 = 64512;
// GLOBAL: FF7 0x00899020
unsigned char g_BattleSpecialEnergyModelNames1[21] =
    "ELEC1.RSD\0\0\0"
    "HADO.RSD";
// GLOBAL: FF7 0x00899038
unsigned char g_BattleSpecialEnergyModelNames2[22] =
    "HADO1.RSD\0\0\0"
    "TAMA1.RSD";
// GLOBAL: FF7 0x00899050
unsigned char g_BattleSpecialEnergyModelNames3[21] =
    "TAMA2.RSD\0\0\0"
    "MOTO.RSD";
// GLOBAL: FF7 0x00899068
unsigned char g_BattleSpecialEnergyModelNames4[7] = "WA.RSD";
// GLOBAL: FF7 0x00899070
unsigned char g_BattleSpecialEnergyModelNames5[23] =
    "SINDOM1.RSD\0"
    "HIKARI.RSD";
// GLOBAL: FF7 0x00899088
unsigned char g_BattleSpecialEnergyTextureName1[13] = "ENERGY00.TIM";
// GLOBAL: FF7 0x00899098
unsigned char g_BattleSpecialEnergySpriteName1[8] = "KOU.SPT";
// GLOBAL: FF7 0x008990a0
unsigned char g_BattleSpecialEnergyTextureSpriteNames1[39] =
    "JO_A.SPT\0\0\0\0"
    "ENERGY00.TIM\0\0\0\0"
    "HAHEN1.SPT";
// GLOBAL: FF7 0x008990c8
unsigned char g_BattleSpecialEnergyTextureName2[13] = "ENERGY03.TIM";
// GLOBAL: FF7 0x008990d8
unsigned char g_BattleSpecialEnergySpriteName2[10] = "BAKU1.SPT";
// GLOBAL: FF7 0x00b4fb38
struct BattleSpecialEnergyWavePosition g_BattleSpecialEnergyPosition = {0};
unsigned char g_bssPad_00b4fb58[4] = {0};
// GLOBAL: FF7 0x00b4fb5c
int g_BattleSpecialEnergyDrawListHead = 0;
// GLOBAL: FF7 0x00b4fb60
struct BattleFxTimeDamageWarpPoint g_BattleSpecialEnergyOffsetVector = {0};
// GLOBAL: FF7 0x00b4fb64
short g_BattleSpecialEnergyAnglePitch = 0;
// GLOBAL: FF7 0x00b4fb68
struct GfxTableCursor *g_BattleSpecialEnergyLoadedResTable = 0;
// GLOBAL: FF7 0x00b4fb6c
short g_BattleSpecialEnergyAngleRoll = 0;
// GLOBAL: FF7 0x00b4fb70
int g_BattleSpecialEnergyTimer = 0;
unsigned char g_bssPad_00b4fb74[4] = {0};
// clang-format on


// When the engine context exists and g_BattleSpecialEnergyLoadedResTable is empty, builds SPECIAL/ENERGY with a default-
// duration record and creates resource type 13 using g_BattleSpecialEnergyResourceDefs.
// FUNCTION: FF7 0x0050cd90
void __cdecl BattleFx_SpecialLoadResEnergy(void)
{
    GfxModelTableBank *stage;
    GfxResourceRequest requestSlot;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resultDefArr;

    if (g_pBattle3dScene != 0 && g_BattleSpecialEnergyLoadedResTable == 0) {
        kind = 0xD;
        resultDefArr = (int)&g_BattleSpecialEnergyResourceDefs;
        stage = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008990e4
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/ENERGY", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resultDefArr, pRequest, &g_BattleSpecialEnergyLoadedResTable, stage);
    }
}

// Mechanical entry point: calls BattleFx_SpecialLoadResEnergy, then forwards targetMask and actorIdx to BattleFx_SpecialBeginEnergy. Nothing
// else.
// FUNCTION: FF7 0x0050ce20
void __cdecl BattleFx_SpecialEnterEnergy(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResEnergy();
    BattleFx_SpecialBeginEnergy(targetMask, actorIdx);
}

// Loads SPECIAL/ENERGY through BattleFx_SpecialLoadResEnergy and creates a mode-four actor-mask callback task using
// both arguments and BattleFx_SpecialSpawnEnergyPerTarget.
// FUNCTION: FF7 0x0050ce3a
void __cdecl BattleFx_SpecialBeginEnergy(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResEnergy();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnEnergyPerTarget);
}

// Spawns the energy timeline task for one target: records the firing actor's primary bone position
// as the start and the target's bone position as the end, builds a look basis from their
// difference into g_BattleSpecialEnergyPosition, stores the target actor, clears the timeline
// colour state, starts a fixed-point ramp and plays the energy sound at the firing actor.
// FUNCTION: FF7 0x0050ce5b
void __cdecl BattleFx_SpecialSpawnEnergyPerTarget(int actor, int actorIdx)
{
    BattleFxEnergyTimeline *timeline;
    int a6;
    short dir[3];

    timeline = (BattleFxEnergyTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyTimelineTask);
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].primaryBone, &timeline->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &timeline->endX);
    dir[0] = timeline->endX - timeline->startX;
    dir[1] = timeline->endY - timeline->startY;
    dir[2] = timeline->endZ - timeline->startZ;
    Gte_BuildLookBasisFromDirection(dir, (short *)&g_BattleSpecialEnergyPosition);
    timeline->targetActorIdx = actor;
    g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2 = 0;
    Battle_StartFixedPointRamp(0xC00, 0xA);
    Battle_PlaySfxPannedToActor(0xFA, actorIdx, 0x40);
}

// Timeline task for the energy special, the controller that spawns its other tasks as the frame
// counter advances. While the battle is paused it does nothing. At frame 0 it spawns a shrinking
// sprite task, a stretching models task and a beam task at the slot's position, and the stretching
// task gets the distance to the target. From frame 7 to 13 it spawns two animated sprites at random
// offsets within about 600 units of the target. At frame 10 it spawns a spin models task and an
// expanding model task.
// FUNCTION: FF7 0x0050cf60
void __cdecl BattleFx_SpecialEnergyTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[48];
        struct { unsigned char pad_to_C[0xC]; BattleFxEnergyTimelineSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_14[0x14]; short i; };
        struct { unsigned char pad_to_18[0x18]; short distance; };
        struct { unsigned char pad_to_1C[0x1C]; BattleFxEnergyBallTimelineSlot *timelineSlot; };
        struct { unsigned char pad_to_20[0x20]; short deltaX; };
        struct { unsigned char pad_to_22[0x22]; short deltaY; };
        struct { unsigned char pad_to_24[0x24]; short deltaZ; };
    } frame;
#pragma pack(pop)

    frame.timelineSlot = (BattleFxEnergyBallTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timelineSlot->frameCounter == 0) {
        frame.spawnSlot = (BattleFxEnergyTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyShrinkingSpriteTask);
        frame.spawnSlot->posX = frame.timelineSlot->posX;
        frame.spawnSlot->posY = frame.timelineSlot->posY;
        frame.spawnSlot->posZ = frame.timelineSlot->posZ;
    }
    if (frame.timelineSlot->frameCounter == 0) {
        frame.spawnSlot = (BattleFxEnergyTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyStretchingModelsTask);
        frame.spawnSlot->posX = frame.timelineSlot->posX;
        frame.spawnSlot->posY = frame.timelineSlot->posY;
        frame.spawnSlot->posZ = frame.timelineSlot->posZ;
        frame.deltaX = frame.timelineSlot->targetX - frame.spawnSlot->posX;
        frame.deltaY = frame.timelineSlot->targetY - frame.spawnSlot->posY;
        frame.deltaZ = frame.timelineSlot->targetZ - frame.spawnSlot->posZ;
        frame.distance = SqrtToInt(frame.deltaX * frame.deltaX + frame.deltaY * frame.deltaY + frame.deltaZ * frame.deltaZ);
        frame.spawnSlot->startValue = frame.distance * 0x1200 / 0xBB8;
        frame.spawnSlot->clearedParam = 0;
        frame.spawnSlot = (BattleFxEnergyTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyBeamModelsTask);
        frame.spawnSlot->posX = frame.timelineSlot->posX;
        frame.spawnSlot->posY = frame.timelineSlot->posY;
        frame.spawnSlot->posZ = frame.timelineSlot->posZ;
        frame.spawnSlot->startValue = frame.distance;
        frame.spawnSlot->clearedParam = 0;
    }
    if (frame.timelineSlot->frameCounter > 6 && frame.timelineSlot->frameCounter < 0xE) {
        for (frame.i = 0; frame.i < 2; frame.i++) {
            frame.spawnSlot = (BattleFxEnergyTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyMainSlotAnimatedSpriteTask);
            frame.spawnSlot->posX = frame.timelineSlot->targetX + rand() % 0x4B0 + -0x258;
            frame.spawnSlot->posY = frame.timelineSlot->targetY + rand() % 0x4B0 + -0x258;
            frame.spawnSlot->posZ = frame.timelineSlot->targetZ + rand() % 0x4B0 + -0x258;
        }
    }
    if (frame.timelineSlot->frameCounter == 0xA) {
        frame.spawnSlot = (BattleFxEnergyTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergySpinModelsTask);
        frame.spawnSlot->posX = frame.timelineSlot->targetX;
        frame.spawnSlot->posY = 0;
        frame.spawnSlot->posZ = frame.timelineSlot->targetZ;
        frame.spawnSlot->variant = 0;
        frame.spawnSlot = (BattleFxEnergyTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyExpandingModelTask);
        frame.spawnSlot->posX = frame.timelineSlot->targetX;
        frame.spawnSlot->posY = 0xFED4;
        frame.spawnSlot->posZ = frame.timelineSlot->targetZ;
        for (frame.i = 0; frame.i < 0x14; frame.i++) {
            frame.spawnSlot = (BattleFxEnergyTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyBouncingSpriteTask);
            frame.spawnSlot->posX = frame.timelineSlot->targetX;
            frame.spawnSlot->posZ = frame.timelineSlot->targetZ;
            frame.spawnSlot->posY = 0;
            BattleFx_MakeRandomConeVector(0xFFFFFF06, 0x64, &frame.spawnSlot->velocityX);
            frame.spawnSlot->variant = rand() % 7;
        }
        Battle_PlaySfxPannedToActor(0x164, (short)frame.timelineSlot->actorIndex, 0x40);
    }
    if (frame.timelineSlot->frameCounter == 0x23) {
        Battle_PostActorHitOrHurtMessage(frame.timelineSlot->actorIndex);
    }
    frame.timelineSlot->frameCounter++;
    if (frame.timelineSlot->frameCounter == 0x2D) {
        frame.timelineSlot->ownerSlot = BATTLE_TASK_FREE;
        Battle_StartFixedPointRamp(0, 0xA);
    }
}

// Per-frame task for the bouncing sprite of the energy special. The roll is 8 for the first 22
// frames and then falls as 30 minus the frame counter. It is stored in
// g_BattleSpecialEnergyAngleRoll and feeds the model descriptors. The task builds a rotation from
// that roll and the slot's angle, transforms the slot's position, and submits one sprite draw.
// While the battle is not paused, the slot's position advances by its velocity each frame.
// FUNCTION: FF7 0x0050d372
void __cdecl BattleFx_SpecialEnergyBouncingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[88];
        int rollValue;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix matrixRow2; };
        struct { unsigned char pad_to_4C[0x4C]; short axisIndex; };
        struct { unsigned char pad_to_50[0x50]; int transDepth; };
        struct { unsigned char pad_to_54[0x54]; BattleFxEnergyWaveBouncingSpriteSlot *bouncingSlot; };
    } frame;
#pragma pack(pop)

    frame.bouncingSlot = (BattleFxEnergyWaveBouncingSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialEnergySpriteConfig1.bounceSpriteGroup = Gfx_ModelTableGetLoadedRecord(0xB, g_BattleSpecialEnergyLoadedResTable);
    g_BattleSpecialEnergySpriteConfig1.bounceDrawObj = Gfx_ModelTableGetSpriteObject(0xB, g_BattleSpecialEnergyLoadedResTable);
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.matrixRow2.m[2][frame.axisIndex] = 0;
        frame.matrixRow2.m[1][frame.axisIndex] = 0;
        frame.matrixRow2.m[0][frame.axisIndex] = 0;
    }
    if (frame.bouncingSlot->frameCounter < 0x16) {
        frame.rollValue = 8;
    } else {
        frame.rollValue = 0x1E - frame.bouncingSlot->frameCounter;
    }
    g_BattleSpecialEnergyAngleRoll = *(short *)&frame.rollValue;
    frame.rotX.vy = 0;
    frame.rotX.vx = frame.rotX.vy;
    frame.rotX.vz = frame.bouncingSlot->angle;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((SVECTOR *)&frame.bouncingSlot->posX, frame.matrixRow2.t, &frame.transDepth);
    frame.matrixRow2.m[2][2] = 0x2000;
    frame.matrixRow2.m[1][1] = frame.matrixRow2.m[2][2];
    frame.matrixRow2.m[0][0] = frame.matrixRow2.m[1][1];
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.matrixRow2, &frame.rotMatrix);
    SetRotMatrix(&frame.matrixRow2);
    SetTransMatrix(&frame.matrixRow2);
    g_BattleSpecialEnergyModelDescriptors.frameIndex = frame.bouncingSlot->modelDesc;
    g_BattleSpecialEnergyModelDescriptors.primCode = (g_BattleSpecialEnergyAngleRoll - 8 != 0 ? 2 : 0) + 0x2C;
    g_BattleSpecialEnergyModelDescriptors.blue = g_BattleSpecialEnergyAngleRoll << 4;
    g_BattleSpecialEnergyModelDescriptors.green = g_BattleSpecialEnergyModelDescriptors.blue;
    g_BattleSpecialEnergyModelDescriptors.red = g_BattleSpecialEnergyModelDescriptors.green;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecialEnergySpriteConfig1.bounceSpriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.bouncingSlot->posX += frame.bouncingSlot->velocityX;
    frame.bouncingSlot->posY += frame.bouncingSlot->velocityY;
    frame.bouncingSlot->posZ += frame.bouncingSlot->velocityZ;
    if (frame.bouncingSlot->posY > 0) {
        frame.bouncingSlot->posY = 0;
        frame.bouncingSlot->velocityY *= -1;
    }
    frame.bouncingSlot->velocityY += 0x14;
    frame.bouncingSlot->angle += 0x200;
    frame.bouncingSlot->frameCounter++;
    if (frame.bouncingSlot->frameCounter == 0x1E) {
        frame.bouncingSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Per-frame task for the expanding model of the energy special. Its uniform scale is the step
// counter times 0x5DC, and the model is drawn with the camera transform applied. The model
// descriptor's step word is set from the step counter shifted left by eight. While the battle is
// not paused, the step advances each frame, and at step 15 the slot is freed.
// FUNCTION: FF7 0x0050d5f5
void __cdecl BattleFx_SpecialEnergyExpandingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxCosmoMemoryTriSpin *expandTask; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_44[0x44]; VECTOR scaleZ; };
    } frame;
#pragma pack(pop)

    frame.expandTask = (BattleFxCosmoMemoryTriSpin *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialEnergyModelDescriptors.expandModelGroup = Gfx_ModelTableGetRenderObject(6, g_BattleSpecialEnergyLoadedResTable);
    RotMatrixXYZ(&g_BattleSpecialEnergyTimer, &frame.rotMatrix);
    frame.scaleZ.vz = frame.expandTask->step * 0x5DC;
    frame.scaleZ.vy = frame.scaleZ.vz;
    frame.scaleZ.vx = frame.scaleZ.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleZ);
    frame.rotMatrix.t[0] = frame.expandTask->posX;
    frame.rotMatrix.t[1] = frame.expandTask->posY;
    frame.rotMatrix.t[2] = frame.expandTask->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialEnergyModelDescriptors.expandFadeLevel = frame.expandTask->step << 8;
    g_BattleSpecialEnergyDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialEnergyModelDescriptors.expandModelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.expandTask->step++;
    if (frame.expandTask->step == 0xF) {
        frame.expandTask->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Per-frame task for the spin models of the energy special. Each pass builds a rotation and scale
// matrix from the slot's frame counter, places the models at the slot's centre, and draws them with
// the camera transform applied. The first model's descriptor rotation comes from a fixed countdown
// of 16 shifted left by three, and the second's from 0x1000 minus the countdown shifted left by
// nine. The second pass spins the opposite way and uses a different scale.
// FUNCTION: FF7 0x0050d726
void __cdecl BattleFx_SpecialEnergySpinModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[144];
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_18[0x18]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_38[0x38]; short countdown; };
        struct { unsigned char pad_to_3C[0x3C]; BattleFxEnergyWaveSpinSlot *spawnSlot; };
        struct { unsigned char pad_to_40[0x40]; short i; };
        struct { unsigned char pad_to_4C[0x4C]; BattleFxEnergyWaveSpinModelsSlot *spinSlot; };
        struct { unsigned char pad_to_50[0x50]; short spawnRadius; };
        struct { unsigned char pad_to_54[0x54]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_74[0x74]; short spawnAngle; };
        struct { unsigned char pad_to_80[0x80]; VECTOR scaleZ; };
    } frame;
#pragma pack(pop)

    frame.spinSlot = (BattleFxEnergyWaveSpinModelsSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialEnergyModelDescriptors.spinModelGroup = Gfx_ModelTableGetRenderObject(7, g_BattleSpecialEnergyLoadedResTable);
    frame.countdown = 0x10;
    frame.rotX.vy = frame.spinSlot->frameCounter << 6;
    frame.rotX.vz = 0;
    frame.rotX.vx = frame.rotX.vz;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.scaleZ.vz = frame.spinSlot->frameCounter * 0x12C + 0x1000;
    frame.scaleZ.vy = frame.scaleZ.vz;
    frame.scaleZ.vx = frame.scaleZ.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleZ);
    frame.rotMatrix.t[0] = frame.spinSlot->centerX;
    frame.rotMatrix.t[1] = frame.spinSlot->centerY;
    frame.rotMatrix.t[2] = frame.spinSlot->centerZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialEnergyModelDescriptors.spinDrawFlags = 0x28;
    g_BattleSpecialEnergyModelDescriptors.spinFadeLevel = frame.countdown << 3;
    g_BattleSpecialEnergyModelDescriptors.spinModelGroup = Gfx_ModelTableGetRenderObject(7, g_BattleSpecialEnergyLoadedResTable);
    g_BattleSpecialEnergyDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialEnergyModelDescriptors.spinModelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyDrawListHead);
    g_BattleSpecialEnergyModelDescriptors.spinDrawFlags = 0xA8;
    g_BattleSpecialEnergyModelDescriptors.spinModelGroup = Gfx_ModelTableGetRenderObject(8, g_BattleSpecialEnergyLoadedResTable);
    g_BattleSpecialEnergyModelDescriptors.spinFadeLevel = 0x1000 - (frame.countdown << 9);
    g_BattleSpecialEnergyDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialEnergyModelDescriptors.spinModelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyDrawListHead);
    frame.rotX.vy = -frame.spinSlot->frameCounter << 6;
    frame.rotX.vz = 0;
    frame.rotX.vx = frame.rotX.vz;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.scaleZ.vz = frame.spinSlot->frameCounter * 0x118 + 0x800;
    frame.scaleZ.vy = frame.scaleZ.vz;
    frame.scaleZ.vx = frame.scaleZ.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleZ);
    frame.rotMatrix.t[0] = frame.spinSlot->centerX;
    frame.rotMatrix.t[1] = frame.spinSlot->centerY;
    frame.rotMatrix.t[2] = frame.spinSlot->centerZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecialEnergyModelDescriptors.spinDrawFlags = 0x28;
    g_BattleSpecialEnergyModelDescriptors.spinFadeLevel = frame.countdown << 3;
    g_BattleSpecialEnergyModelDescriptors.spinModelGroup = Gfx_ModelTableGetRenderObject(7, g_BattleSpecialEnergyLoadedResTable);
    g_BattleSpecialEnergyDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialEnergyModelDescriptors.spinModelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.spinSlot->frameCounter < 0x16) {
        frame.spawnRadius = frame.spinSlot->frameCounter * 0x50;
        for (frame.i = 0; frame.i < 6; frame.i++) {
            frame.spawnAngle = (rand() % 4096);
            frame.spawnSlot = (BattleFxEnergyWaveSpinSlot *)(&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialEnergyAnimatedSpriteTask)]);
            frame.spawnSlot->posX = frame.spinSlot->centerX + (CosFixed(frame.spawnAngle) * frame.spawnRadius >> 0xC);
            frame.spawnSlot->posZ = frame.spinSlot->centerZ + (SinFixed(frame.spawnAngle) * frame.spawnRadius >> 0xC);
            frame.spawnSlot->posY = 0xFF6A;
            frame.spawnSlot->spriteVariant = 2;
        }
    }
    g_BattleCameraFocalPoint.focalPointY += (short)(((frame.spinSlot->frameCounter % 2) << 4) - 8);
    if (frame.spinSlot->frameCounter > 0xE) {
        frame.countdown--;
    }
    frame.spinSlot->frameCounter++;
    if (frame.spinSlot->frameCounter == 0x1E) {
        frame.spinSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Per-frame task of the animated sprites the spin models task spawns in the secondary slots. It
// loads sprite group 0xA and the scene's sprite object, installs a billboard matrix at the slot's
// position at scale 0x2800, submits the sprite with the frame counter as its frame, and while the
// battle is not paused advances the frame until it reaches the sprite group's frame count, which
// frees the slot.
// FUNCTION: FF7 0x0050db2e
void __cdecl BattleFx_SpecialEnergyAnimatedSpriteTask(void)
{
    BattleFxEnergySpriteSlot *task;

    task = (BattleFxEnergySpriteSlot *)(&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx]);
    g_BattleSpecialEnergySpriteConfig1.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0xA, g_BattleSpecialEnergyLoadedResTable);
    g_BattleSpecialEnergySpriteConfig1.drawObj = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    BattleFx_InstallBillboardMatrix(&task->posX, 0x2800, 0);
    g_BattleSpecialEnergySpriteConfig1.frame = task->frameCounter;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecialEnergySpriteConfig1.spriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0 && g_BattleSpecialEnergySpriteConfig1.spriteGroup != 0) {
        task->frameCounter++;
        if (task->frameCounter >= g_BattleSpecialEnergySpriteConfig1.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

// Per-frame task of the animated sprites the timeline task spawns in the main slots. It loads
// sprite group 0xB, installs a billboard matrix at the slot's position at scale 0x2000, submits the
// sprite with the frame counter as its frame, and while the battle is not paused advances the
// frame until it reaches the sprite group's frame count, which frees the slot.
// FUNCTION: FF7 0x0050dbff
void __cdecl BattleFx_SpecialEnergyMainSlotAnimatedSpriteTask(void)
{
    BattleFxEnergySpriteSlot *task;

    task = (BattleFxEnergySpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialEnergyModelDescriptors.animSpriteGroup = Gfx_ModelTableGetLoadedRecord(0xB, g_BattleSpecialEnergyLoadedResTable);
    g_BattleSpecialEnergyModelDescriptors.animDrawObj = Gfx_ModelTableGetSpriteObject(0xB, g_BattleSpecialEnergyLoadedResTable);
    BattleFx_InstallBillboardMatrix(&task->posX, 0x2000, 0);
    g_BattleSpecialEnergyModelDescriptors.animFrame = task->frameCounter;
    g_BattleSpecialEnergyDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecialEnergyModelDescriptors.animSpriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyDrawListHead);
    if (g_BattleIsPaused == 0 && g_BattleSpecialEnergyModelDescriptors.animSpriteGroup != 0) {
        task->frameCounter++;
        if (task->frameCounter >= *(int *)((char *)*(int *)g_BattleSpecialEnergyModelDescriptors.animSpriteGroup + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

// Selects sprite field nine, shrinks billboard scale by 600 times the counter from 0x3000,
// alternates request frames by counter modulo two and shades RGB at 128 below six then by (ten
// minus counter) times 32. Draws before checking pause; when unpaused advances and retires exactly
// at ten.
// FUNCTION: FF7 0x0050dcd0
void __cdecl BattleFx_SpecialEnergyShrinkingSpriteTask(void)
{
    BattleFxEnergySpriteSlot *taskPtr;
    int fadeStep;
    short shadeByte;

    taskPtr = (BattleFxEnergySpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialEnergyModelDescriptors.shrinkSpriteGroup = Gfx_ModelTableGetLoadedRecord(9, g_BattleSpecialEnergyLoadedResTable);
    g_BattleSpecialEnergyModelDescriptors.shrinkDrawObj = Gfx_ModelTableGetSpriteObject(9, g_BattleSpecialEnergyLoadedResTable);
    if (taskPtr->frameCounter < 6) {
        fadeStep = 4;
    } else {
        fadeStep = 0xA - taskPtr->frameCounter;
    }
    shadeByte = fadeStep;
    BattleFx_InstallBillboardMatrix(&taskPtr->posX, 0x3000 - taskPtr->frameCounter * 0x258, 0);
    g_BattleSpecialEnergySpriteRequest1.frameIndex = taskPtr->frameCounter % 2;
    g_BattleSpecialEnergySpriteRequest1.blue = shadeByte << 5;
    g_BattleSpecialEnergySpriteRequest1.green = g_BattleSpecialEnergySpriteRequest1.blue;
    g_BattleSpecialEnergySpriteRequest1.red = g_BattleSpecialEnergySpriteRequest1.green;
    g_BattleSpecialEnergyDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecialEnergyModelDescriptors.shrinkSpriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyDrawListHead);
    if (g_BattleIsPaused == 0) {
        taskPtr->frameCounter++;
        if (taskPtr->frameCounter == 0xA) {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

// Per-frame task for the special energy stretching effect. It builds the rotation from
// g_BattleSpecialEnergyScaleFixed12_1 and the translation from g_BattleSpecialEnergyPosition. It
// scales X and Z by 0x2000 minus frameCounter << 9, and Y by stretchScaleY times frameCounter divided by 6.
// It multiplies the result with g_BattleCameraTransform and submits model draws from resource table
// fields 1 and 2. Only when not paused (the draws above still run) does the roll angle drop by 2
// after frame 6, the frame counter advances, and the slot is freed at frame 10 to release the
// slot.
// FUNCTION: FF7 0x0050ddfe
void __cdecl BattleFx_SpecialEnergyStretchingModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxEnergyStretchingModelsSlot *stretchSlot; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_44[0x44]; VECTOR scaleZ; };
    } frame;
#pragma pack(pop)

    frame.stretchSlot = (BattleFxEnergyStretchingModelsSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialEnergyAngleRoll = 8;
    RotMatrix((SVECTOR *)&g_BattleSpecialEnergyScaleFixed12_1, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialEnergyPosition, &frame.rotMatrix, &frame.rotMatrix);
    frame.scaleZ.vz = 0x2000 - (frame.stretchSlot->frameCounter << 9);
    frame.scaleZ.vx = frame.scaleZ.vz;
    frame.scaleZ.vy = frame.stretchSlot->stretchScaleY * frame.stretchSlot->frameCounter / 6;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleZ);
    frame.rotMatrix.t[0] = frame.stretchSlot->posX;
    frame.rotMatrix.t[1] = frame.stretchSlot->posY;
    frame.rotMatrix.t[2] = frame.stretchSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialEnergySpriteRequest1.fadeLevel = 0x1000 - (g_BattleSpecialEnergyAngleRoll << 8);
    g_BattleSpecialEnergySpriteRequest1.modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleSpecialEnergyLoadedResTable);
    g_BattleSpecialEnergyDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialEnergySpriteRequest1.modelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyDrawListHead);
    g_BattleSpecialEnergySpriteRequest1.modelGroup = Gfx_ModelTableGetRenderObject(2, g_BattleSpecialEnergyLoadedResTable);
    g_BattleSpecialEnergyDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialEnergySpriteRequest1.modelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.stretchSlot->frameCounter > 6) {
        g_BattleSpecialEnergyAngleRoll -= 2;
    }
    frame.stretchSlot->frameCounter++;
    if (frame.stretchSlot->frameCounter == 0xA) {
        frame.stretchSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Per-frame task of the energy beam: raises the beam models' pitch with the frame counter, builds
// the beam's matrix from the slot's position and draws the three beam models and a sprite.
// FUNCTION: FF7 0x0050dfc5
void __cdecl BattleFx_SpecialEnergyBeamModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        int transDepth;
        struct { unsigned char pad_to_4[0x4]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_24[0x24]; BattleFxEnergyBeamModelsSlot *beamSlot; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.beamSlot = (BattleFxEnergyBeamModelsSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialEnergySpriteConfig2.config2SpriteField = Gfx_ModelTableGetLoadedRecord(9, g_BattleSpecialEnergyLoadedResTable);
    g_BattleSpecialEnergySpriteConfig2.config2SpriteAlt = Gfx_ModelTableGetSpriteObject(9, g_BattleSpecialEnergyLoadedResTable);
    g_BattleSpecialEnergyAngleRoll = 8;
    g_BattleSpecialEnergyOffsetVector.offsetX = 0;
    g_BattleSpecialEnergyOffsetVector.offsetY = 0;
    g_BattleSpecialEnergyAnglePitch = frame.beamSlot->pitchRate * frame.beamSlot->frameCounter / 6;
    g_BattleSpecialEnergyPosition.x = frame.beamSlot->posX;
    g_BattleSpecialEnergyPosition.y = frame.beamSlot->posY;
    g_BattleSpecialEnergyPosition.z = frame.beamSlot->posZ;
    SetRotMatrix(&g_BattleSpecialEnergyPosition);
    SetTransMatrix(&g_BattleSpecialEnergyPosition);
    RotTransPointToInt16((short *)&g_BattleSpecialEnergyOffsetVector, (short *)&g_BattleSpecialEnergyOffsetVector, &frame.transDepth);
    RotMatrix((SVECTOR *)&g_BattleSpecialEnergyScaleFixed12_2, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialEnergyPosition, &frame.rotMatrix, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, (int *)&g_BattleSpecialEnergySpriteConfig2);
    frame.rotMatrix.t[0] = g_BattleSpecialEnergyOffsetVector.offsetX;
    frame.rotMatrix.t[1] = g_BattleSpecialEnergyOffsetVector.offsetY;
    frame.rotMatrix.t[2] = g_BattleSpecialEnergyAnglePitch;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecialEnergySpriteConfig2.config2RollAngle = g_BattleSpecialEnergyAngleRoll << 4;
    g_BattleSpecialEnergySpriteConfig2.config2ModelField = Gfx_ModelTableGetRenderObject(3, g_BattleSpecialEnergyLoadedResTable);
    g_BattleSpecialEnergyDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialEnergySpriteConfig2.config2ModelField, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyDrawListHead);
    g_BattleSpecialEnergySpriteConfig2.config2ModelField = Gfx_ModelTableGetRenderObject(4, g_BattleSpecialEnergyLoadedResTable);
    g_BattleSpecialEnergyDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialEnergySpriteConfig2.config2ModelField, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyDrawListHead);
    g_BattleSpecialEnergySpriteConfig2.config2ModelField = Gfx_ModelTableGetRenderObject(5, g_BattleSpecialEnergyLoadedResTable);
    g_BattleSpecialEnergyDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialEnergySpriteConfig2.config2ModelField, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyDrawListHead);
    BattleFx_InstallBillboardMatrix((void *)&g_BattleSpecialEnergyOffsetVector, 0x3000, 0);
    g_BattleSpecialEnergySpriteRequest2.request2Byte2 = g_BattleSpecialEnergyAngleRoll * 0x14;
    g_BattleSpecialEnergySpriteRequest2.request2Byte1 = g_BattleSpecialEnergySpriteRequest2.request2Byte2;
    g_BattleSpecialEnergySpriteRequest2.red = g_BattleSpecialEnergySpriteRequest2.request2Byte1;
    g_BattleSpecialEnergySpriteRequest2.request2FrameParity = (frame.beamSlot->frameCounter % 2);
    g_BattleSpecialEnergyDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecialEnergySpriteConfig2.config2SpriteField, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.beamSlot->frameCounter > 2) {
        g_BattleSpecialEnergyAngleRoll--;
    }
    frame.beamSlot->frameCounter++;
    if (frame.beamSlot->frameCounter == 0xA) {
        frame.beamSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
