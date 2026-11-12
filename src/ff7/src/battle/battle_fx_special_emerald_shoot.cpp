// src/ff7/src/battle/battle_fx_special_emerald_shoot.cpp
/* Enemy Special Effect [idx 42]: EmeraldShoot (JP: hado / 波動 (Wave), asset: "SPECIAL/HADO") — Emerald Shoot (0x3d8, 0x3db, Emerald Weapon), Wave (0x12b, Special Combatant) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Hado spell. It loads its resources, registers per-target
//   callbacks, and runs the timeline, bouncing and expanding sprites, spinning and stretching
//   models, beam models, and animated and shrinking sprite tasks.

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
#include "lib/enums/graphics.h"
extern "C" {

struct BattleFxModelDraw;
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// GLOBAL: FF7 0x00898bb0
unsigned int g_BattleSpecialEnergyWaveResourceDefs[70] = {0x001900b1, (unsigned int)&g_BattleSpecialEnergyWaveModelNames1, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialEnergyWaveModelNames1[12], 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialEnergyWaveModelNames2, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialEnergyWaveModelNames2[12], 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialEnergyWaveModelNames3, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialEnergyWaveModelNames3[12], 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialEnergyWaveModelNames4, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialEnergyWaveModelNames5, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialEnergyWaveModelNames5[12], 0x00000000, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecialEnergyWaveTextureName1, (unsigned int)&g_BattleSpecialEnergyWaveSpriteName1, 0x00000000, 0x00000000, 0x00192a39, 0x00000000, (unsigned int)&g_BattleSpecialEnergyWaveTextureSpriteNames1, 0x00000000, 0x00000000, 0x00192a3c, (unsigned int)&g_BattleSpecialEnergyWaveTextureSpriteNames1[12], (unsigned int)&g_BattleSpecialEnergyWaveTextureSpriteNames1[28], 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecialEnergyWaveTextureName2, (unsigned int)&g_BattleSpecialEnergyWaveTextureSpriteNames2, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecialEnergyWaveTextureSpriteNames2[12], (unsigned int)&g_BattleSpecialEnergyWaveTextureSpriteNames2[28], 0x00000000, 0x00000000};
// GLOBAL: FF7 0x00898cc8
BattleSpecialEnergyWaveSpriteConfig1 g_BattleSpecialEnergyWaveSpriteConfig1 = {0, 0, {0x40, 0x40, 0x40, 0x2e}};
// GLOBAL: FF7 0x00898ce0
BattleSpecialEnergyWaveModelDescriptors g_BattleSpecialEnergyWaveModelDescriptors = {-128, -128, -128, 0x2c, 0, {0}, 0, {0x88}, 0, {0x20}, 0, 0, {0}, 0x80, {0x20}, 0, 0, {0x80, 0x80, 0x80, 0x2e}};
// GLOBAL: FF7 0x00898d20
BattleSpecialEnergyWaveSpriteRequest1 g_BattleSpecialEnergyWaveSpriteRequest1 = {-128, -128, -128, {0x2e}, 0, {0}, 0, {0xa8}, 0x80};
unsigned char g_rawData_00898d34[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00898d38
int g_BattleSpecialEnergyWaveScaleFixed12_1 = 64512;
// GLOBAL: FF7 0x00898d40
struct BattleSpecialEnergyWaveSpriteConfig2 g_BattleSpecialEnergyWaveSpriteConfig2 = {{0, 0x20, 0, 0, 0, 0x20, 0, 0, 0, 0x20, 0, 0, 0, 0, 0, 0}, 0, {0x28, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}, 0, 0};
// GLOBAL: FF7 0x00898d68
struct BattleSpecialEnergyWaveSpriteRequest2 g_BattleSpecialEnergyWaveSpriteRequest2 = {-128, -128, -128, {0x2e}, 0, {0}};
// GLOBAL: FF7 0x00898d70
int g_BattleSpecialEnergyWaveScaleFixed12_2 = 64512;
// GLOBAL: FF7 0x00898d78
unsigned char g_BattleSpecialEnergyWaveModelNames1[21] =
    "ELEC1.RSD\0\0\0"
    "HADO.RSD";
// GLOBAL: FF7 0x00898d90
unsigned char g_BattleSpecialEnergyWaveModelNames2[22] =
    "HADO1.RSD\0\0\0"
    "TAMA1.RSD";
// GLOBAL: FF7 0x00898da8
unsigned char g_BattleSpecialEnergyWaveModelNames3[21] =
    "TAMA2.RSD\0\0\0"
    "MOTO.RSD";
// GLOBAL: FF7 0x00898dc0
unsigned char g_BattleSpecialEnergyWaveModelNames4[7] = "WA.RSD";
// GLOBAL: FF7 0x00898dc8
unsigned char g_BattleSpecialEnergyWaveModelNames5[23] =
    "SINDOM1.RSD\0"
    "HIKARI.RSD";
// GLOBAL: FF7 0x00898de0
unsigned char g_BattleSpecialEnergyWaveTextureName1[13] = "ENERGY00.TIM";
// GLOBAL: FF7 0x00898df0
unsigned char g_BattleSpecialEnergyWaveSpriteName1[8] = "KOU.SPT";
// GLOBAL: FF7 0x00898df8
unsigned char g_BattleSpecialEnergyWaveTextureSpriteNames1[39] =
    "JO_A.SPT\0\0\0\0"
    "ENERGY00.TIM\0\0\0\0"
    "HAHEN1.SPT";
// GLOBAL: FF7 0x00898e20
unsigned char g_BattleSpecialEnergyWaveTextureName2[13] = "ENERGY00.TIM";
// GLOBAL: FF7 0x00898e30
unsigned char g_BattleSpecialEnergyWaveTextureSpriteNames2[38] =
    "HAHEN1.SPT\0\0"
    "ENERGY02.TIM\0\0\0\0"
    "BAKU1.SPT";
// GLOBAL: FF7 0x00b4faf0
struct BattleSpecialEnergyWavePosition g_BattleSpecialEnergyWavePosition = {0};
unsigned char g_bssPad_00b4fb10[4] = {0};
// GLOBAL: FF7 0x00b4fb14
int g_BattleSpecialEnergyWaveDrawListHandle = 0;
// GLOBAL: FF7 0x00b4fb18
struct BattleFxTimeDamageWarpPoint g_BattleSpecialEnergyWaveOffsetVector = {0};
// GLOBAL: FF7 0x00b4fb1c
short g_BattleSpecialEnergyWaveAnglePitch = 0;
// GLOBAL: FF7 0x00b4fb20
short g_BattleSpecialEnergyWaveAngleYaw = 0;
// GLOBAL: FF7 0x00b4fb24
short g_BattleSpecialEnergyWaveAngleRoll = 0;
// GLOBAL: FF7 0x00b4fb28
short g_BattleSpecialEnergyWaveTimer = 0;
// GLOBAL: FF7 0x00b4fb2c
struct GfxTableCursor *g_BattleSpecialEnergyWaveResourceTable = 0;
// GLOBAL: FF7 0x00b4fb30
int g_BattleSpecialEnergyWaveModelRotation = 0;
unsigned char g_bssPad_00b4fb34[4] = {0};
// clang-format on


// Builds SPECIAL/HADO and loads the Hado resource table (type 14) if it is not loaded yet.
// FUNCTION: FF7 0x0050b850
void __cdecl BattleFx_SpecialLoadResEnergyWave(void)
{
    GfxModelTableBank *stage;
    GfxResourceRequest requestSlot;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resultDefArr;

    if (g_pBattle3dScene != 0 && g_BattleSpecialEnergyWaveResourceTable == 0) {
        kind = 0xE;
        resultDefArr = (int)&g_BattleSpecialEnergyWaveResourceDefs;
        stage = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x00898e58
        pRequest->resourcePath = (char *)BattleFx_BuildResourcePath("SPECIAL/HADO", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resultDefArr, pRequest, &g_BattleSpecialEnergyWaveResourceTable, stage);
    }
}

// Entry point: resets the spin, beam and stretch counters, loads the resources and registers the per-target callback.
// FUNCTION: FF7 0x0050b8e0
void __cdecl BattleFx_SpecialEnterEnergyWave(int targetMask, int actorIdx)
{
    g_BattleSpecialEnergyWaveAngleYaw = 0x10;
    g_BattleSpecialEnergyWaveTimer = 8;
    g_BattleSpecialEnergyWaveAngleRoll = 8;
    BattleFx_SpecialLoadResEnergyWave();
    BattleFx_SpecialRegisterEnergyWavePerTargetCallback(targetMask, actorIdx);
}

// Queues SpawnHadoPerTarget for every actor in the target mask.
// FUNCTION: FF7 0x0050b915
void __cdecl BattleFx_SpecialRegisterEnergyWavePerTargetCallback(int targetMask, int actorIdx)
{
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnEnergyWavePerTarget);
}

// Spawns the timeline task between the caster's bone and the target's bone, builds the beam's look basis from the
// direction between them, clears the fade channels, starts the fixed-point ramp and plays the sound.
// FUNCTION: FF7 0x0050b931
void __cdecl BattleFx_SpecialSpawnEnergyWavePerTarget(int actor, int actorIdx)
{
    BattleFxEnergyWaveTimeline *timeline;
    int idx;
    short dir[3];

    timeline = (BattleFxEnergyWaveTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyWaveTimelineTask);
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].primaryBone, &timeline->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &timeline->endX);
    dir[0] = timeline->endX - timeline->startX;
    dir[1] = timeline->endY - timeline->startY;
    dir[2] = timeline->endZ - timeline->startZ;
    Gte_BuildLookBasisFromDirection(dir, (short *)&g_BattleSpecialEnergyWavePosition);
    timeline->targetMask = actor;
    g_BattleFxTimelineState.channel2 = 0;
    g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
    g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    Battle_StartFixedPointRamp(0xC00, 0xA);
    Battle_PlaySfxPannedToActor(0xFA, (short)actorIdx, 0x40);
}

// Timeline: on frame 0 spawns the shrinking sprite, the stretching models and the beam models (their scale follows the
// start-to-target distance). Frames 7 to 13 spawn two animated sprites near the target per frame; frame 10 spawns the
// spin models, the expanding model and 20 bouncing sprites at the target. Returns at once while the battle is paused.
// FUNCTION: FF7 0x0050ba36
void __cdecl BattleFx_SpecialEnergyWaveTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[48];
        struct { unsigned char pad_to_C[0xC]; BattleFxEnergyWaveTimelineSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_14[0x14]; short i; };
        struct { unsigned char pad_to_18[0x18]; short distance; };
        struct { unsigned char pad_to_1C[0x1C]; BattleFxEnergyWaveTimelineSlot *timelineSlot; };
        struct { unsigned char pad_to_20[0x20]; short deltaX; };
        struct { unsigned char pad_to_22[0x22]; short deltaY; };
        struct { unsigned char pad_to_24[0x24]; short deltaZ; };
    } frame;
#pragma pack(pop)

    frame.timelineSlot = (BattleFxEnergyWaveTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timelineSlot->frameCounter == 0) {
        frame.spawnSlot = (BattleFxEnergyWaveTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyWaveShrinkingSpriteTask);
        frame.spawnSlot->posX = frame.timelineSlot->startX;
        frame.spawnSlot->posY = frame.timelineSlot->startY;
        frame.spawnSlot->posZ = frame.timelineSlot->startZ;
    }
    if (frame.timelineSlot->frameCounter == 0) {
        frame.spawnSlot = (BattleFxEnergyWaveTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyWaveStretchingModelsTask);
        frame.spawnSlot->posX = frame.timelineSlot->startX;
        frame.spawnSlot->posY = frame.timelineSlot->startY;
        frame.spawnSlot->posZ = frame.timelineSlot->startZ;
        frame.deltaX = frame.timelineSlot->targetX - frame.spawnSlot->posX;
        frame.deltaY = frame.timelineSlot->targetY - frame.spawnSlot->posY;
        frame.deltaZ = frame.timelineSlot->targetZ - frame.spawnSlot->posZ;
        frame.distance = SqrtToInt(frame.deltaX * frame.deltaX + frame.deltaY * frame.deltaY + frame.deltaZ * frame.deltaZ);
        frame.spawnSlot->scale = frame.distance * 0x1200 / 0xBB8;
        frame.spawnSlot->clearedParam = 0;
        frame.spawnSlot = (BattleFxEnergyWaveTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyWaveBeamModelsTask);
        frame.spawnSlot->posX = frame.timelineSlot->startX;
        frame.spawnSlot->posY = frame.timelineSlot->startY;
        frame.spawnSlot->posZ = frame.timelineSlot->startZ;
        frame.spawnSlot->scale = frame.distance;
        frame.spawnSlot->clearedParam = 0;
    }
    if (frame.timelineSlot->frameCounter > 6 && frame.timelineSlot->frameCounter < 0xE) {
        for (frame.i = 0; frame.i < 2; frame.i++) {
            frame.spawnSlot = (BattleFxEnergyWaveTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyWaveMainSlotAnimatedSpriteTask);
            frame.spawnSlot->posX = frame.timelineSlot->targetX + rand() % 0x4B0 + -0x258;
            frame.spawnSlot->posY = frame.timelineSlot->targetY + rand() % 0x4B0 + -0x258;
            frame.spawnSlot->posZ = frame.timelineSlot->targetZ + rand() % 0x4B0 + -0x258;
        }
    }
    if (frame.timelineSlot->frameCounter == 0xA) {
        frame.spawnSlot = (BattleFxEnergyWaveTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyWaveSpinModelsTask);
        frame.spawnSlot->posX = frame.timelineSlot->targetX;
        frame.spawnSlot->posY = 0;
        frame.spawnSlot->posZ = frame.timelineSlot->targetZ;
        frame.spawnSlot->variant = 0;
        frame.spawnSlot = (BattleFxEnergyWaveTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyWaveExpandingModelTask);
        frame.spawnSlot->posX = frame.timelineSlot->targetX;
        frame.spawnSlot->posY = 0xFED4;
        frame.spawnSlot->posZ = frame.timelineSlot->targetZ;
        for (frame.i = 0; frame.i < 0x14; frame.i++) {
            frame.spawnSlot = (BattleFxEnergyWaveTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyWaveBouncingSpriteTask);
            frame.spawnSlot->posX = frame.timelineSlot->targetX;
            frame.spawnSlot->posZ = frame.timelineSlot->targetZ;
            frame.spawnSlot->posY = 0;
            BattleFx_MakeRandomConeVector(0xFFFFFF06, 0x64, &frame.spawnSlot->velocityX);
            frame.spawnSlot->variant = rand() % 7;
        }
        Battle_PlaySfxPannedToActor(0x176, (short)frame.timelineSlot->actorIndex, 0x40);
    }
    if (frame.timelineSlot->frameCounter == 0x23) {
        Battle_PostActorHitOrHurtMessage(frame.timelineSlot->actorIndex);
    }
    (frame.timelineSlot->frameCounter)++;
    if (frame.timelineSlot->frameCounter == 0x2D) {
        frame.timelineSlot->ownerSlot = BATTLE_TASK_FREE;
        Battle_StartFixedPointRamp(0, 0xA);
    }
}

// Draws one bouncing sprite. Its brightness level is 8 until frame 0x16, then falls to 0 at frame 0x1E; level 8 uses
// table row 0xB and the others row 0xC. The sprite bounces off the ground (Y = 0) and spins by 0x200 per frame.
// FUNCTION: FF7 0x0050be48
void __cdecl BattleFx_SpecialEnergyWaveBouncingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int levelWide;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; short level; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_50[0x50]; short axisIndex; };
        struct { unsigned char pad_to_54[0x54]; int rotDepth; };
        struct { unsigned char pad_to_58[0x58]; BattleFxEnergyWaveBouncingSpriteSlot *spriteSlot; };
    } frame;
#pragma pack(pop)

    frame.spriteSlot = (BattleFxEnergyWaveBouncingSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.combinedMatrix.m[2][frame.axisIndex] = 0;
        frame.combinedMatrix.m[1][frame.axisIndex] = 0;
        frame.combinedMatrix.m[0][frame.axisIndex] = 0;
    }
    if (frame.spriteSlot->frameCounter < 0x16) {
        frame.levelWide = 8;
    } else {
        frame.levelWide = 0x1E - frame.spriteSlot->frameCounter;
    }
    frame.level = *(short *)&frame.levelWide;
    frame.rotX.vy = 0;
    frame.rotX.vx = frame.rotX.vy;
    frame.rotX.vz = frame.spriteSlot->angle;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint(&frame.spriteSlot->posX, frame.combinedMatrix.t, &frame.rotDepth);
    frame.combinedMatrix.m[2][2] = 0x2000;
    frame.combinedMatrix.m[1][1] = frame.combinedMatrix.m[2][2];
    frame.combinedMatrix.m[0][0] = frame.combinedMatrix.m[1][1];
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.combinedMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecialEnergyWaveModelDescriptors.frameIndex = frame.spriteSlot->modelDesc;
    if (frame.level == 8) {
        g_BattleSpecialEnergyWaveModelDescriptors.primCode = PSX_PRIM_POLY_FT4;
        g_BattleSpecialEnergyWaveSpriteConfig1.bounceSpriteGroup = Gfx_ModelTableGetLoadedRecord(0xB, g_BattleSpecialEnergyWaveResourceTable);
        g_BattleSpecialEnergyWaveSpriteConfig1.bounceDrawObj = Gfx_ModelTableGetSpriteObject(0xB, g_BattleSpecialEnergyWaveResourceTable);
    } else {
        g_BattleSpecialEnergyWaveModelDescriptors.primCode = PSX_PRIM_POLY_FT4_TRANS;
        g_BattleSpecialEnergyWaveSpriteConfig1.bounceSpriteGroup = Gfx_ModelTableGetLoadedRecord(0xC, g_BattleSpecialEnergyWaveResourceTable);
        g_BattleSpecialEnergyWaveSpriteConfig1.bounceDrawObj = Gfx_ModelTableGetSpriteObject(0xC, g_BattleSpecialEnergyWaveResourceTable);
    }
    g_BattleSpecialEnergyWaveModelDescriptors.blue = frame.level << 4;
    g_BattleSpecialEnergyWaveModelDescriptors.green = g_BattleSpecialEnergyWaveModelDescriptors.blue;
    g_BattleSpecialEnergyWaveModelDescriptors.red = g_BattleSpecialEnergyWaveModelDescriptors.green;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecialEnergyWaveSpriteConfig1.bounceSpriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spriteSlot->posX += frame.spriteSlot->velocityX;
    frame.spriteSlot->posY += frame.spriteSlot->velocityY;
    frame.spriteSlot->posZ += frame.spriteSlot->velocityZ;
    if (frame.spriteSlot->posY > 0) {
        frame.spriteSlot->posY = 0;
        frame.spriteSlot->velocityY *= -1;
    }
    frame.spriteSlot->velocityY += 0x14;
    frame.spriteSlot->angle += 0x200;
    (frame.spriteSlot->frameCounter)++;
    if (frame.spriteSlot->frameCounter == 0x1E) {
        frame.spriteSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Draws the expanding model with a black draw color, scaled by 0x5DC per step. Drawing runs even while paused;
// otherwise the step advances and the task frees itself at step 15.
// FUNCTION: FF7 0x0050c0f0
void __cdecl BattleFx_SpecialEnergyWaveExpandingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxCosmoMemoryTriSpin *expandTask; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_44[0x44]; VECTOR scaleFactor; };
    } frame;
#pragma pack(pop)

    frame.expandTask = (BattleFxCosmoMemoryTriSpin *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialEnergyWaveModelDescriptors.expandModelGroup = Gfx_ModelTableGetRenderObject(6, g_BattleSpecialEnergyWaveResourceTable);
    RotMatrixXYZ(&g_BattleSpecialEnergyWaveModelRotation, &frame.rotMatrix);
    frame.scaleFactor.vz = frame.expandTask->step * 0x5DC;
    frame.scaleFactor.vy = frame.scaleFactor.vz;
    frame.scaleFactor.vx = frame.scaleFactor.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleFactor);
    frame.rotMatrix.t[0] = frame.expandTask->posX;
    frame.rotMatrix.t[1] = frame.expandTask->posY;
    frame.rotMatrix.t[2] = frame.expandTask->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialEnergyWaveModelDescriptors.expandFadeLevel = frame.expandTask->step << 8;
    g_BattleSpecialEnergyWaveDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialEnergyWaveModelDescriptors.expandModelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyWaveDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.expandTask->step)++;
    if (frame.expandTask->step == 0xF) {
        frame.expandTask->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Draws the two spin models (the second spins the opposite way at a different scale) and, for the first 0x16 frames,
// spawns six animated sprites on a ring around the center. Shakes the camera focal point and frees itself at frame 0x1E.
// FUNCTION: FF7 0x0050c221
void __cdecl BattleFx_SpecialEnergyWaveSpinModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[140];
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_18[0x18]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_38[0x38]; BattleFxEnergyWaveSpinSlot *spinSprite; };
        struct { unsigned char pad_to_3C[0x3C]; short i; };
        struct { unsigned char pad_to_48[0x48]; BattleFxEnergyWaveSpinModelsSlot *spinSlot; };
        struct { unsigned char pad_to_4C[0x4C]; short radius; };
        struct { unsigned char pad_to_50[0x50]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_70[0x70]; short randAngle; };
        struct { unsigned char pad_to_7C[0x7C]; VECTOR scaleFactor; };
    } frame;
#pragma pack(pop)

    frame.spinSlot = (BattleFxEnergyWaveSpinModelsSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vy = frame.spinSlot->frameCounter << 6;
    frame.rotX.vz = 0;
    frame.rotX.vx = frame.rotX.vz;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.scaleFactor.vz = frame.spinSlot->frameCounter * 0x12C + 0x1000;
    frame.scaleFactor.vy = frame.scaleFactor.vz;
    frame.scaleFactor.vx = frame.scaleFactor.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleFactor);
    frame.rotMatrix.t[0] = frame.spinSlot->centerX;
    frame.rotMatrix.t[1] = frame.spinSlot->centerY;
    frame.rotMatrix.t[2] = frame.spinSlot->centerZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialEnergyWaveModelDescriptors.spinDrawFlags = 0x28;
    g_BattleSpecialEnergyWaveModelDescriptors.spinFadeLevel = g_BattleSpecialEnergyWaveAngleYaw << 3;
    g_BattleSpecialEnergyWaveModelDescriptors.spinModelGroup = Gfx_ModelTableGetRenderObject(7, g_BattleSpecialEnergyWaveResourceTable);
    g_BattleSpecialEnergyWaveDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialEnergyWaveModelDescriptors.spinModelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyWaveDrawListHandle);
    g_BattleSpecialEnergyWaveModelDescriptors.spinDrawFlags = 0xA8;
    g_BattleSpecialEnergyWaveModelDescriptors.spinModelGroup = Gfx_ModelTableGetRenderObject(8, g_BattleSpecialEnergyWaveResourceTable);
    g_BattleSpecialEnergyWaveModelDescriptors.spinFadeLevel = 0x1000 - (g_BattleSpecialEnergyWaveAngleYaw << 9);
    g_BattleSpecialEnergyWaveDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialEnergyWaveModelDescriptors.spinModelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyWaveDrawListHandle);
    frame.rotX.vy = -frame.spinSlot->frameCounter << 6;
    frame.rotX.vz = 0;
    frame.rotX.vx = frame.rotX.vz;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.scaleFactor.vz = frame.spinSlot->frameCounter * 0x118 + 0x800;
    frame.scaleFactor.vy = frame.scaleFactor.vz;
    frame.scaleFactor.vx = frame.scaleFactor.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleFactor);
    frame.rotMatrix.t[0] = frame.spinSlot->centerX;
    frame.rotMatrix.t[1] = frame.spinSlot->centerY;
    frame.rotMatrix.t[2] = frame.spinSlot->centerZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    g_BattleSpecialEnergyWaveModelDescriptors.spinDrawFlags = 0x28;
    g_BattleSpecialEnergyWaveModelDescriptors.spinFadeLevel = g_BattleSpecialEnergyWaveAngleYaw << 3;
    g_BattleSpecialEnergyWaveModelDescriptors.spinModelGroup = Gfx_ModelTableGetRenderObject(7, g_BattleSpecialEnergyWaveResourceTable);
    g_BattleSpecialEnergyWaveDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialEnergyWaveModelDescriptors.spinModelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyWaveDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.spinSlot->frameCounter < 0x16) {
        frame.radius = frame.spinSlot->frameCounter * 0x50;
        for (frame.i = 0; frame.i < 6; frame.i++) {
            frame.randAngle = (rand() % 4096);
            frame.spinSprite = (BattleFxEnergyWaveSpinSlot *)(&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialEnergyWaveAnimatedSpriteTask)]);
            frame.spinSprite->posX = frame.spinSlot->centerX + (CosFixed(frame.randAngle) * frame.radius >> 0xC);
            frame.spinSprite->posZ = frame.spinSlot->centerZ + (SinFixed(frame.randAngle) * frame.radius >> 0xC);
            frame.spinSprite->posY = 0xFF6A;
            frame.spinSprite->spriteVariant = 2;
        }
    }
    g_BattleCameraFocalPoint.focalPointY += (short)(((frame.spinSlot->frameCounter % 2) << 4) - 8);
    if (frame.spinSlot->frameCounter > 0xE) {
        g_BattleSpecialEnergyWaveAngleYaw--;
    }
    (frame.spinSlot->frameCounter)++;
    if (frame.spinSlot->frameCounter == 0x1E) {
        frame.spinSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Animated sprite from table row 0xA (secondary task slot): draws the current frame and frees itself at the last frame.
// FUNCTION: FF7 0x0050c61d
void __cdecl BattleFx_SpecialEnergyWaveAnimatedSpriteTask(void)
{
    BattleFxSewerTsunamiAnimSprite *task;

    task = (BattleFxSewerTsunamiAnimSprite *)(&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx]);
    g_BattleSpecialEnergyWaveSpriteConfig1.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0xA, g_BattleSpecialEnergyWaveResourceTable);
    g_BattleSpecialEnergyWaveSpriteConfig1.drawObj = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    BattleFx_InstallBillboardMatrix(&task->posX, 0x2800, 0);
    g_BattleSpecialEnergyWaveSpriteConfig1.frame = task->frame;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecialEnergyWaveSpriteConfig1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0 && g_BattleSpecialEnergyWaveSpriteConfig1.spriteGroup != 0) {
        (task->frame)++;
        if (task->frame >= g_BattleSpecialEnergyWaveSpriteConfig1.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

// Animated sprite from table row 0xD (main task slot): draws the current frame and frees itself at the last frame.
// FUNCTION: FF7 0x0050c6ee
void __cdecl BattleFx_SpecialEnergyWaveMainSlotAnimatedSpriteTask(void)
{
    BattleFxSewerTsunamiAnimSprite *task;

    task = (BattleFxSewerTsunamiAnimSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialEnergyWaveModelDescriptors.animSpriteGroup = Gfx_ModelTableGetLoadedRecord(0xD, g_BattleSpecialEnergyWaveResourceTable);
    g_BattleSpecialEnergyWaveModelDescriptors.animDrawObj = Gfx_ModelTableGetSpriteObject(0xD, g_BattleSpecialEnergyWaveResourceTable);
    BattleFx_InstallBillboardMatrix(&task->posX, 0x2000, 0);
    g_BattleSpecialEnergyWaveModelDescriptors.animFrame = task->frame;
    g_BattleSpecialEnergyWaveDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialEnergyWaveModelDescriptors.animSpriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyWaveDrawListHandle);
    if (g_BattleIsPaused == 0 && g_BattleSpecialEnergyWaveModelDescriptors.animSpriteGroup != 0) {
        (task->frame)++;
        if (task->frame >= *(int *)((char *)*(int *)g_BattleSpecialEnergyWaveModelDescriptors.animSpriteGroup + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

// Shrinking sprite (table row 8): the billboard scale shrinks by 600 per frame from 0x3000 and the brightness fades over
// the last four frames. Draws before checking pause; frees itself at frame 10.
// FUNCTION: FF7 0x0050c7bf
void __cdecl BattleFx_SpecialEnergyWaveShrinkingSpriteTask(void)
{
    BattleFxSewerTsunamiAnimSprite *taskPtr;
    int fadeStep;
    short shadeByte;

    taskPtr = (BattleFxSewerTsunamiAnimSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialEnergyWaveModelDescriptors.shrinkSpriteGroup = Gfx_ModelTableGetLoadedRecord(8, g_BattleSpecialEnergyWaveResourceTable);
    g_BattleSpecialEnergyWaveModelDescriptors.shrinkDrawObj = Gfx_ModelTableGetSpriteObject(8, g_BattleSpecialEnergyWaveResourceTable);
    if (taskPtr->frame < 6) {
        fadeStep = 4;
    } else {
        fadeStep = 0xA - taskPtr->frame;
    }
    shadeByte = fadeStep;
    BattleFx_InstallBillboardMatrix(&taskPtr->posX, 0x3000 - taskPtr->frame * 0x258, 0);
    g_BattleSpecialEnergyWaveSpriteRequest1.frameIndex = taskPtr->frame % 2;
    g_BattleSpecialEnergyWaveSpriteRequest1.blue = shadeByte << 5;
    g_BattleSpecialEnergyWaveSpriteRequest1.green = g_BattleSpecialEnergyWaveSpriteRequest1.blue;
    g_BattleSpecialEnergyWaveSpriteRequest1.red = g_BattleSpecialEnergyWaveSpriteRequest1.green;
    g_BattleSpecialEnergyWaveDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialEnergyWaveModelDescriptors.shrinkSpriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyWaveDrawListHandle);
    if (g_BattleIsPaused == 0) {
        (taskPtr->frame)++;
        if (taskPtr->frame == 0xA) {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

// Draws the two stretching models at the slot position, scaled down as the frame counter rises and stretched by
// stretchRate. Unless paused, the timer drops by 2 after frame 6 and the task frees itself at frame 10.
// FUNCTION: FF7 0x0050c8ed
void __cdecl BattleFx_SpecialEnergyWaveStretchingModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxEnergyWaveStretchingModelsSlot *stretchSlot; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_44[0x44]; VECTOR scaleFactor; };
    } frame;
#pragma pack(pop)

    frame.stretchSlot = (BattleFxEnergyWaveStretchingModelsSlot *)BATTLE_CURRENT_TASK_SLOT;
    RotMatrix((SVECTOR *)&g_BattleSpecialEnergyWaveScaleFixed12_1, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialEnergyWavePosition, &frame.rotMatrix, &frame.rotMatrix);
    frame.scaleFactor.vz = 0x2000 - (frame.stretchSlot->frameCounter << 9);
    frame.scaleFactor.vx = frame.scaleFactor.vz;
    frame.scaleFactor.vy = frame.stretchSlot->stretchRate * frame.stretchSlot->frameCounter / 6;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleFactor);
    frame.rotMatrix.t[0] = frame.stretchSlot->posX;
    frame.rotMatrix.t[1] = frame.stretchSlot->posY;
    frame.rotMatrix.t[2] = frame.stretchSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialEnergyWaveSpriteRequest1.fadeLevel = 0x1000 - (g_BattleSpecialEnergyWaveTimer << 8);
    g_BattleSpecialEnergyWaveSpriteRequest1.modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleSpecialEnergyWaveResourceTable);
    g_BattleSpecialEnergyWaveDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialEnergyWaveSpriteRequest1.modelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyWaveDrawListHandle);
    g_BattleSpecialEnergyWaveSpriteRequest1.modelGroup = Gfx_ModelTableGetRenderObject(2, g_BattleSpecialEnergyWaveResourceTable);
    g_BattleSpecialEnergyWaveDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialEnergyWaveSpriteRequest1.modelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyWaveDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.stretchSlot->frameCounter > 6) {
        g_BattleSpecialEnergyWaveTimer -= 2;
    }
    (frame.stretchSlot->frameCounter)++;
    if (frame.stretchSlot->frameCounter == 0xA) {
        frame.stretchSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Draws the three beam models and the beam sprite at the slot position with a growing pitch. Unless paused, the roll
// angle drops by 1 after frame 2 and the task frees itself at frame 10.
// FUNCTION: FF7 0x0050caab
void __cdecl BattleFx_SpecialEnergyWaveBeamModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        int transDepth;
        struct { unsigned char pad_to_4[0x4]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_24[0x24]; BattleFxEnergyWaveBeamModelsSlot *beamSlot; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.beamSlot = (BattleFxEnergyWaveBeamModelsSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialEnergyWaveSpriteConfig2.config2SpriteField = Gfx_ModelTableGetLoadedRecord(8, g_BattleSpecialEnergyWaveResourceTable);
    g_BattleSpecialEnergyWaveSpriteConfig2.config2SpriteAlt = Gfx_ModelTableGetSpriteObject(8, g_BattleSpecialEnergyWaveResourceTable);
    (g_BattleSpecialEnergyWaveOffsetVector.offsetX) = 0;
    g_BattleSpecialEnergyWaveOffsetVector.offsetY = 0;
    g_BattleSpecialEnergyWaveAnglePitch = frame.beamSlot->pitchStep * frame.beamSlot->frameCounter / 6;
    g_BattleSpecialEnergyWavePosition.x = frame.beamSlot->posX;
    g_BattleSpecialEnergyWavePosition.y = frame.beamSlot->posY;
    g_BattleSpecialEnergyWavePosition.z = frame.beamSlot->posZ;
    SetRotMatrix(&g_BattleSpecialEnergyWavePosition);
    SetTransMatrix(&g_BattleSpecialEnergyWavePosition);
    RotTransPointToInt16((short *)&g_BattleSpecialEnergyWaveOffsetVector, (short *)&g_BattleSpecialEnergyWaveOffsetVector, &frame.transDepth);
    RotMatrix((SVECTOR *)&g_BattleSpecialEnergyWaveScaleFixed12_2, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialEnergyWavePosition, &frame.rotMatrix, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialEnergyWaveSpriteConfig2);
    frame.rotMatrix.t[0] = (g_BattleSpecialEnergyWaveOffsetVector.offsetX);
    frame.rotMatrix.t[1] = g_BattleSpecialEnergyWaveOffsetVector.offsetY;
    frame.rotMatrix.t[2] = g_BattleSpecialEnergyWaveAnglePitch;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecialEnergyWaveSpriteConfig2.config2RollAngle = g_BattleSpecialEnergyWaveAngleRoll << 4;
    g_BattleSpecialEnergyWaveSpriteConfig2.config2ModelField = Gfx_ModelTableGetRenderObject(3, g_BattleSpecialEnergyWaveResourceTable);
    g_BattleSpecialEnergyWaveDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialEnergyWaveSpriteConfig2.config2ModelField, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyWaveDrawListHandle);
    g_BattleSpecialEnergyWaveSpriteConfig2.config2ModelField = Gfx_ModelTableGetRenderObject(4, g_BattleSpecialEnergyWaveResourceTable);
    g_BattleSpecialEnergyWaveDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialEnergyWaveSpriteConfig2.config2ModelField, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyWaveDrawListHandle);
    g_BattleSpecialEnergyWaveSpriteConfig2.config2ModelField = Gfx_ModelTableGetRenderObject(5, g_BattleSpecialEnergyWaveResourceTable);
    g_BattleSpecialEnergyWaveDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialEnergyWaveSpriteConfig2.config2ModelField, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyWaveDrawListHandle);
    BattleFx_InstallBillboardMatrix((short *)&g_BattleSpecialEnergyWaveOffsetVector, 0x3000, 0);
    g_BattleSpecialEnergyWaveSpriteRequest2.request2Byte2 = g_BattleSpecialEnergyWaveAngleRoll * 0x14;
    g_BattleSpecialEnergyWaveSpriteRequest2.request2Byte1 = g_BattleSpecialEnergyWaveSpriteRequest2.request2Byte2;
    (g_BattleSpecialEnergyWaveSpriteRequest2.red) = g_BattleSpecialEnergyWaveSpriteRequest2.request2Byte1;
    g_BattleSpecialEnergyWaveSpriteRequest2.request2FrameParity = (frame.beamSlot->frameCounter % 2);
    g_BattleSpecialEnergyWaveDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialEnergyWaveSpriteConfig2.config2SpriteField, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyWaveDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.beamSlot->frameCounter > 2) {
        g_BattleSpecialEnergyWaveAngleRoll--;
    }
    (frame.beamSlot->frameCounter)++;
    if (frame.beamSlot->frameCounter == 0xA) {
        frame.beamSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
