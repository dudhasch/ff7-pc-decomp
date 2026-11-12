// src/ff7/src/battle/battle_fx_special_energy_ball.cpp
/* Enemy Special Effect [idx 43]: EnergyBall (JP: ho_den / 放電 (Electrical Discharge), asset: "SPECIAL/HO_DEN") — Energy Ball (0x12f, 0x132, Air Buster), Ruby Ray (0x3cb, Ruby Weapon) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the HoDen spell. It loads its target data, spawns a
//   per-target effect, and runs the timeline, the counter-spinning, stretching, beam, and fading
//   model tasks, and the sprite tasks.

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

struct GfxRotationAngles2;
struct BattleFxModelDraw;
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// GLOBAL: FF7 0x008997f0
unsigned int g_BattleSpecEnergyBallResourceDef[110] = {
    0x001900b1,
    (unsigned int)&g_BattleSpecEnergyBallModelName,
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00899a64
    (unsigned int)"HOUDEN1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00899a70
    (unsigned int)"HOUDEN2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00899a7c
    (unsigned int)"HOUDEN3.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00899a88
    (unsigned int)"HOUDEN4.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00899a94
    (unsigned int)"E1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00899a9c
    (unsigned int)"E2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00899aa4
    (unsigned int)"E3.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00899aac
    (unsigned int)"E4.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00899ab4
    (unsigned int)"E5.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00899abc
    (unsigned int)"E6.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00899ac4
    (unsigned int)"E7.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00899acc
    (unsigned int)"E8.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00899ad4
    (unsigned int)"E9.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00899adc
    (unsigned int)"E10.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00899ae4
    (unsigned int)"E11.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00899aec
    (unsigned int)"E12.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00899af4
    (unsigned int)"TAMA1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00899b00
    (unsigned int)"TAMA2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x003200b1,
    // STRING: FF7 0x00899b0c
    (unsigned int)"MOTO.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x00899b18
    (unsigned int)"HO_DEN02.TIM",
    // STRING: FF7 0x00899b28
    (unsigned int)"HIKARI1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x00899b34
    (unsigned int)"HO_DEN03.TIM",
    // STRING: FF7 0x00899b44
    (unsigned int)"BAKU1.SPT"};
// clang-format off
// GLOBAL: FF7 0x008999a8
struct BattleSpecEnergyBallSpriteRequest1 g_BattleSpecEnergyBallSpriteRequest1 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
// GLOBAL: FF7 0x008999b8
struct BattleSpecEnergyBallSpriteRequest2 g_BattleSpecEnergyBallSpriteRequest2 = {0, 0, -128, -128, -128, {0x2e}, 0, {0}};
// GLOBAL: FF7 0x008999c8
struct BattleSpecEnergyBallTransformData1 g_BattleSpecEnergyBallTransformData1 = {0, {0x28, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// clang-format on
// GLOBAL: FF7 0x008999d8
int g_BattleSpecEnergyBallScale1 = 8192;
// GLOBAL: FF7 0x008999dc
int g_BattleSpecEnergyBallScale2 = 4096;
// clang-format off
unsigned char g_rawData_008999e0[8] = {0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008999e8
struct BattleSpecEnergyBallTransformData2 g_BattleSpecEnergyBallTransformData2 = {0, {0x28, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}};
// clang-format on
// GLOBAL: FF7 0x008999f8
struct GfxRotationAngles2 g_BattleSpecEnergyBallVelData1 = {-1024, 0, 0};
// GLOBAL: FF7 0x00899a00
unsigned char g_BattleSpecEnergyBallVelData2[6] = {0x00, 0x20, 0x00, 0x00, 0x00, 0x20};
// clang-format off
unsigned char g_rawData_00899a08[8] = {0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00899a10
struct BattleSpecEnergyBallTransformData3 g_BattleSpecEnergyBallTransformData3 = {0, {0x28, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x00899a20
struct BattleSpecEnergyBallSpriteRequest3 g_BattleSpecEnergyBallSpriteRequest3 = {0, 0, -128, -128, -128, {0x2e}, 0, {0}};
// clang-format on
// GLOBAL: FF7 0x00899a30
struct GfxRotationAngles2 g_BattleSpecEnergyBallVelData3 = {-1024, 0, 0};
// GLOBAL: FF7 0x00899a38
unsigned char g_BattleSpecEnergyBallVelData4[6] = {0x00, 0x30, 0x00, 0x00, 0x00, 0x30};
// clang-format off
unsigned char g_rawData_00899a40[8] = {0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00899a48
struct BattleSpecEnergyBallTransformData4 g_BattleSpecEnergyBallTransformData4 = {0, {0x28, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x00899a58
unsigned char g_BattleSpecEnergyBallModelName[10] = "ELEC1.RSD";
// GLOBAL: FF7 0x00b4fc18
struct BattleSpecEnergyBallStateBuffer g_BattleSpecEnergyBallStateBuffer = {0};
// clang-format on
unsigned char g_bssPad_00b4fc38[4] = {0};
// GLOBAL: FF7 0x00b4fc3c
int g_BattleSpecEnergyBallDrawListHead = 0;
// GLOBAL: FF7 0x00b4fc40
struct BattleLimitHighwindLoadedResTable g_BattleSpecEnergyBallBeamEnd = {0};
// GLOBAL: FF7 0x00b4fc44
short g_BattleSpecEnergyBallStep = 0;
// GLOBAL: FF7 0x00b4fc48
short g_BattleSpecEnergyBallBeamFade = 0;
// GLOBAL: FF7 0x00b4fc4c
short g_BattleSpecEnergyBallStretchFade = 0;
// GLOBAL: FF7 0x00b4fc50
int g_BattleSpecEnergyBallSpinScaleX = 0;
// GLOBAL: FF7 0x00b4fc54
int g_BattleSpecEnergyBallSpinScaleY = 0;
// GLOBAL: FF7 0x00b4fc58
int g_BattleSpecEnergyBallSpinScaleZ = 0;
unsigned char g_bssPad_00b4fc5c[4] = {0};
// GLOBAL: FF7 0x00b4fc60
struct GfxTableCursor *g_BattleSpecEnergyBallResourceTable = 0;
unsigned char g_bssPad_00b4fc64[4] = {0};
// clang-format on


/*
 * When the battle scene exists and the HoDen resource table is not loaded yet, requests SPECIAL/HO_DEN
 * with a default-duration record and loads resource type 22 described by g_BattleSpecEnergyBallResourceDef.
 */
// FUNCTION: FF7 0x00510230
void __cdecl BattleFx_SpecialLoadResEnergyBall(void)
{
    GfxModelTableBank *stage;
    GfxResourceRequest requestSlot;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resultDefArr;

    if (g_pBattle3dScene != 0 && g_BattleSpecEnergyBallResourceTable == 0) {
        kind = 0x16;
        resultDefArr = (int)&g_BattleSpecEnergyBallResourceDef;
        stage = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x00899b50
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/HO_DEN", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resultDefArr, pRequest, &g_BattleSpecEnergyBallResourceTable, stage);
    }
}

/*
 * Entry point: sets the beam and stretch fade levels to 8, then forwards targetIdx and actorIdx to
 * BattleFx_SpecialBeginEnergyBall.
 */
// FUNCTION: FF7 0x005102c0
void __cdecl BattleFx_SpecialEnterEnergyBall(int targetMask, int actorIdx)
{
    g_BattleSpecEnergyBallStretchFade = 8;
    g_BattleSpecEnergyBallBeamFade = 8;
    BattleFx_SpecialBeginEnergyBall(targetMask, actorIdx);
}

/*
 * Loads the HoDen resources and creates a mode-four actor-mask callback task that runs
 * BattleFx_SpecialSpawnEnergyBallPerTarget for each target.
 */
// FUNCTION: FF7 0x005102e7
void __cdecl BattleFx_SpecialBeginEnergyBall(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResEnergyBall();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnEnergyBallPerTarget);
}

// Spawns one HoDen effect for a pair of actors. It allocates a slot running
// BattleFx_SpecialEnergyBallTimelineTask. It fills the start point with Battle_GetBoneWorldPos of
// actorIdx's bone34, and the end point with that of actor's targetBone. It builds the direction
// from start to end in a local short[3] and passes it to Gte_BuildLookBasisFromDirection, which
// writes g_BattleSpecEnergyBallStateBuffer. It stores actor in the timeline and zeroes the colour
// fields channel2, channel1 and red of g_BattleFxTimelineState. It then calls
// Battle_StartFixedPointRamp(0x1000, 20) and Battle_PlaySfxPannedToActor(0x1C8, actorIdx, 0x40).
// FUNCTION: FF7 0x00510308
void __cdecl BattleFx_SpecialSpawnEnergyBallPerTarget(int actor, int actorIdx)
{
    BattleFxEnergyBallTimeline *timeline;
    int a6;
    short direction[3];

    timeline = (BattleFxEnergyBallTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyBallTimelineTask);
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].bone34, &timeline->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &timeline->endX);
    direction[0] = timeline->endX - timeline->startX;
    direction[1] = timeline->endY - timeline->startY;
    direction[2] = timeline->endZ - timeline->startZ;
    Gte_BuildLookBasisFromDirection(direction, (short *)&g_BattleSpecEnergyBallStateBuffer);
    timeline->targetIdx = actor;
    g_BattleFxTimelineState.channel2 = 0;
    g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
    g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    Battle_StartFixedPointRamp(0x1000, 20);
    Battle_PlaySfxPannedToActor(0x1C8, actorIdx, 0x40);
}

/*
 * Runs the HoDen timeline: spawns the growing flash, the counter-spinning models, the fading models,
 * the stretching and beam models and the impact sprites at fixed frames, then frees itself.
 */
// FUNCTION: FF7 0x0051040d
void __cdecl BattleFx_SpecialEnergyBallTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[48];
        struct { unsigned char pad_to_C[0xC]; BattleFxEnergyBallTimelineSpawnSlot *spawnSlot; };
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
        frame.spawnSlot = (BattleFxEnergyBallTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyBallGrowingFadeSpriteTask);
        frame.spawnSlot->posX = frame.timelineSlot->posX;
        frame.spawnSlot->posY = frame.timelineSlot->posY;
        frame.spawnSlot->posZ = frame.timelineSlot->posZ;
    }
    if (frame.timelineSlot->frameCounter < 12 && (frame.timelineSlot->frameCounter % 4) == 0) {
        frame.spawnSlot = (BattleFxEnergyBallTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyBallCounterSpinModelsTask);
        frame.spawnSlot->posX = frame.timelineSlot->posX;
        frame.spawnSlot->posY = frame.timelineSlot->posY;
        frame.spawnSlot->posZ = frame.timelineSlot->posZ;
    }
    if (frame.timelineSlot->frameCounter >= 20 && frame.timelineSlot->frameCounter < 28) {
        frame.spawnSlot = (BattleFxEnergyBallTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyBallFadingModelTask);
        frame.spawnSlot->posX = frame.timelineSlot->posX;
        frame.spawnSlot->posY = frame.timelineSlot->posY;
        frame.spawnSlot->posZ = frame.timelineSlot->posZ;
        frame.spawnSlot->modelIndex = (rand() % 4);
        frame.spawnSlot->angleX = 0xFC00 /* 5670 deg */;
        frame.spawnSlot->angleZ = 0;
        frame.spawnSlot->angleY = (rand() % 4096);
    }
    if (frame.timelineSlot->frameCounter == 20) {
        frame.spawnSlot = (BattleFxEnergyBallTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyBallStretchingModelTask);
        frame.spawnSlot->posX = frame.timelineSlot->posX;
        frame.spawnSlot->posY = frame.timelineSlot->posY;
        frame.spawnSlot->posZ = frame.timelineSlot->posZ;
        frame.deltaX = frame.timelineSlot->targetX - frame.spawnSlot->posX;
        frame.deltaY = frame.timelineSlot->targetY - frame.spawnSlot->posY;
        frame.deltaZ = frame.timelineSlot->targetZ - frame.spawnSlot->posZ;
        frame.distance = SqrtToInt(frame.deltaX * frame.deltaX + frame.deltaY * frame.deltaY + frame.deltaZ * frame.deltaZ);
        frame.spawnSlot->scale = frame.distance * 0x1200 / 3000;
        frame.spawnSlot->clearedParam = 0;
        frame.spawnSlot = (BattleFxEnergyBallTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyBallBeamModelsTask);
        frame.spawnSlot->posX = frame.timelineSlot->posX;
        frame.spawnSlot->posY = frame.timelineSlot->posY;
        frame.spawnSlot->posZ = frame.timelineSlot->posZ;
        frame.spawnSlot->scale = frame.distance;
        frame.spawnSlot->clearedParam = 0;
    }
    if (frame.timelineSlot->frameCounter > 27 && frame.timelineSlot->frameCounter < 43) {
        for (frame.i = 0; frame.i < 2; frame.i++) {
            frame.spawnSlot = (BattleFxEnergyBallTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyBallAnimatedSpriteTask);
            frame.spawnSlot->posX = frame.timelineSlot->targetX + rand() % 1200 + -600;
            frame.spawnSlot->posY = frame.timelineSlot->targetY + rand() % 1200 + -600;
            frame.spawnSlot->posZ = frame.timelineSlot->targetZ + rand() % 1200 + -600;
        }
    }
    if (frame.timelineSlot->frameCounter == 35) {
        Battle_PostActorHitOrHurtMessage(frame.timelineSlot->actorIndex);
        Battle_PlaySfxPannedToActor(0xFA, (short)frame.timelineSlot->actorIndex, 0x40);
    }
    (frame.timelineSlot->frameCounter)++;
    if (frame.timelineSlot->frameCounter == 45) {
        Battle_StartFixedPointRamp(0, 10);
        frame.timelineSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Per-frame step of an impact sprite: fills g_BattleSpecEnergyBallSpriteRequest1 from resource 0x15 of
 * the HoDen resource table, installs a billboard matrix at the task's position at scale 0x2000,
 * uses the task's frameCounter as the sprite frame and submits the sprite into the depth-sorted
 * ordering table. While the game is not paused the frame advances, and the task frees itself at
 * the sprite's frame count.
 */
// FUNCTION: FF7 0x0051080d
void __cdecl BattleFx_SpecialEnergyBallAnimatedSpriteTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecEnergyBallSpriteRequest1.spriteEntry) = Gfx_ModelTableGetLoadedRecord(21, g_BattleSpecEnergyBallResourceTable);
    g_BattleSpecEnergyBallSpriteRequest1.request1ResField = Gfx_ModelTableGetSpriteObject(21, g_BattleSpecEnergyBallResourceTable);
    BattleFx_InstallBillboardMatrix((short *)task->data, 0x2000 /* 2.0 */, 0);
    g_BattleSpecEnergyBallSpriteRequest1.request1Short = task->frameCounter;
    g_BattleSpecEnergyBallDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecEnergyBallSpriteRequest1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecEnergyBallDrawListHead);
    if (g_BattleIsPaused == 0 && (g_BattleSpecEnergyBallSpriteRequest1.spriteEntry) != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= *(int *)((char *)*(int *)(g_BattleSpecEnergyBallSpriteRequest1.spriteEntry) + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Selects sprite field twenty, grows billboard scale from 0x2000 by 200 times the counter,
 * alternates request frames by counter modulo two and shades RGB at 128 below twenty-eight then by
 * (thirty-six minus counter) times sixteen. Draws before checking pause; when unpaused advances
 * and retires exactly at thirty-six.
 */
// FUNCTION: FF7 0x005108de
void __cdecl BattleFx_SpecialEnergyBallGrowingFadeSpriteTask(void)
{
    BattleTaskSlot *taskPtr;
    int fadeStep;
    short shadeByte;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecEnergyBallSpriteRequest2.spriteResource) = Gfx_ModelTableGetLoadedRecord(20, g_BattleSpecEnergyBallResourceTable);
    g_BattleSpecEnergyBallSpriteRequest2.request2ResField = Gfx_ModelTableGetSpriteObject(20, g_BattleSpecEnergyBallResourceTable);
    if (taskPtr->frameCounter < 28) {
        fadeStep = 8;
    } else {
        fadeStep = 36 - taskPtr->frameCounter;
    }
    shadeByte = fadeStep;
    BattleFx_InstallBillboardMatrix((short *)taskPtr->data, taskPtr->frameCounter * 0xC8 + 0x2000, 0);
    g_BattleSpecEnergyBallSpriteRequest2.request2FrameParity = taskPtr->frameCounter % 2;
    g_BattleSpecEnergyBallSpriteRequest2.request2Byte3 = shadeByte << 4;
    g_BattleSpecEnergyBallSpriteRequest2.request2Byte2 = g_BattleSpecEnergyBallSpriteRequest2.request2Byte3;
    g_BattleSpecEnergyBallSpriteRequest2.request2Byte1 = g_BattleSpecEnergyBallSpriteRequest2.request2Byte2;
    g_BattleSpecEnergyBallDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecEnergyBallSpriteRequest2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecEnergyBallDrawListHead);
    if (g_BattleIsPaused == 0) {
        (taskPtr->frameCounter)++;
        if (taskPtr->frameCounter == 36) {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws two counter-rotating, shrinking models at the task's position for 20 frames.
 */
// FUNCTION: FF7 0x00510a0b
void __cdecl BattleFx_SpecialEnergyBallCounterSpinModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        SVECTOR angleX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix spinMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleFxEnergyBallCounterSpinModelsSlot *spinSlot; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.spinSlot = (BattleFxEnergyBallCounterSpinModelsSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecEnergyBallTransformData1.transform1Value) = Gfx_ModelTableGetRenderObject(0, g_BattleSpecEnergyBallResourceTable);
    frame.angleX.vx = frame.spinSlot->frameCounter << 6;
    frame.angleX.vy = frame.spinSlot->frameCounter << 7;
    frame.angleX.vz = 0;
    RotMatrix(&frame.angleX, &frame.spinMatrix);
    g_BattleSpecEnergyBallSpinScaleZ = 0x2000 - frame.spinSlot->frameCounter * 0x199;
    g_BattleSpecEnergyBallSpinScaleY = g_BattleSpecEnergyBallSpinScaleZ;
    g_BattleSpecEnergyBallSpinScaleX = g_BattleSpecEnergyBallSpinScaleY;
    ScaleMatrix(&frame.spinMatrix, &g_BattleSpecEnergyBallSpinScaleX);
    frame.spinMatrix.t[0] = frame.spinSlot->rotX;
    frame.spinMatrix.t[1] = frame.spinSlot->rotY;
    frame.spinMatrix.t[2] = frame.spinSlot->rotZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.spinMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecEnergyBallTransformData1.transform1ScaledShort = frame.spinSlot->frameCounter * 6;
    g_BattleSpecEnergyBallDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecEnergyBallTransformData1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecEnergyBallDrawListHead);
    frame.angleX.vx = frame.spinSlot->frameCounter << 6;
    frame.angleX.vy = -frame.spinSlot->frameCounter << 7;
    frame.angleX.vz = 0;
    RotMatrix(&frame.angleX, &frame.spinMatrix);
    g_BattleSpecEnergyBallSpinScaleZ = 0x1800 - frame.spinSlot->frameCounter * 0x133;
    g_BattleSpecEnergyBallSpinScaleY = g_BattleSpecEnergyBallSpinScaleZ;
    g_BattleSpecEnergyBallSpinScaleX = g_BattleSpecEnergyBallSpinScaleY;
    ScaleMatrix(&frame.spinMatrix, &g_BattleSpecEnergyBallSpinScaleX);
    frame.spinMatrix.t[0] = frame.spinSlot->rotX;
    frame.spinMatrix.t[1] = frame.spinSlot->rotY;
    frame.spinMatrix.t[2] = frame.spinSlot->rotZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.spinMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecEnergyBallDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecEnergyBallTransformData1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecEnergyBallDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.spinSlot->frameCounter)++;
    if (frame.spinSlot->frameCounter == 20) {
        frame.spinSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the stretching beam model along the target direction, cycling through resources 5 to 11, and
 * fades it out over the last frames.
 */
// FUNCTION: FF7 0x00510c49
void __cdecl BattleFx_SpecialEnergyBallStretchingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[116];
        PsxMatrix stretchMatrix;
        struct { unsigned char pad_to_20[0x20]; int resIdx5; };
        struct { unsigned char pad_to_24[0x24]; int resIdx6; };
        struct { unsigned char pad_to_28[0x28]; int resIdx7[10]; };
        struct { unsigned char pad_to_50[0x50]; BattleFxOilAimedModelSlot *stretchSlot; };
        struct { unsigned char pad_to_54[0x54]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.stretchSlot = (BattleFxOilAimedModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.resIdx5 = 5;
    frame.resIdx6 = 6;
    frame.resIdx7[0] = 7;
    frame.resIdx7[1] = 8;
    frame.resIdx7[2] = 9;
    frame.resIdx7[3] = 10;
    frame.resIdx7[4] = 11;
    frame.resIdx7[5] = 12;
    frame.resIdx7[6] = 13;
    frame.resIdx7[7] = 14;
    frame.resIdx7[8] = 15;
    frame.resIdx7[9] = 16;
    RotMatrix((SVECTOR *)&g_BattleSpecEnergyBallVelData1, &frame.stretchMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecEnergyBallStateBuffer, &frame.stretchMatrix, &frame.stretchMatrix);
    g_BattleSpecEnergyBallScale2 = frame.stretchSlot->scaleRate * frame.stretchSlot->frameCounter / 10;
    ScaleMatrix(&frame.stretchMatrix, &g_BattleSpecEnergyBallScale1);
    frame.stretchMatrix.t[0] = frame.stretchSlot->posX;
    frame.stretchMatrix.t[1] = frame.stretchSlot->posY;
    frame.stretchMatrix.t[2] = frame.stretchSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.stretchMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecEnergyBallTransformData2.transform2IndexWord = g_BattleSpecEnergyBallStretchFade << 4;
    (g_BattleSpecEnergyBallTransformData2.transform2Value) = Gfx_ModelTableGetRenderObject(frame.resIdx7[frame.stretchSlot->frameCounter % 7], g_BattleSpecEnergyBallResourceTable);
    g_BattleSpecEnergyBallDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecEnergyBallTransformData2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecEnergyBallDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.stretchSlot->frameCounter > 8) {
        g_BattleSpecEnergyBallStretchFade--;
    }
    (frame.stretchSlot->frameCounter)++;
    if (frame.stretchSlot->frameCounter == 16) {
        frame.stretchSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the beam models (resources 0x11 to 0x13) and the glow sprite at the beam's moving end point
 * for 16 frames, fading them out after frame 8.
 */
// FUNCTION: FF7 0x00510e08
void __cdecl BattleFx_SpecialEnergyBallBeamModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        int depth;
        struct { unsigned char pad_to_4[0x4]; PsxMatrix beamMatrix; };
        struct { unsigned char pad_to_24[0x24]; BattleFxEnergyBallBeamModelsSlot *beamSlot; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.beamSlot = (BattleFxEnergyBallBeamModelsSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecEnergyBallSpriteRequest3.spriteResource) = Gfx_ModelTableGetLoadedRecord(20, g_BattleSpecEnergyBallResourceTable);
    g_BattleSpecEnergyBallSpriteRequest3.request3ResField = Gfx_ModelTableGetSpriteObject(20, g_BattleSpecEnergyBallResourceTable);
    (g_BattleSpecEnergyBallBeamEnd.posX) = 0;
    g_BattleSpecEnergyBallBeamEnd.posY = 0;
    g_BattleSpecEnergyBallStep = frame.beamSlot->stepRate * frame.beamSlot->frameCounter / 10;
    g_BattleSpecEnergyBallStateBuffer.x = frame.beamSlot->posX;
    g_BattleSpecEnergyBallStateBuffer.y = frame.beamSlot->posY;
    g_BattleSpecEnergyBallStateBuffer.z = frame.beamSlot->posZ;
    SetRotMatrix(&g_BattleSpecEnergyBallStateBuffer);
    SetTransMatrix(&g_BattleSpecEnergyBallStateBuffer);
    RotTransPointToInt16(&g_BattleSpecEnergyBallBeamEnd, &g_BattleSpecEnergyBallBeamEnd.posX, &frame.depth);
    RotMatrix((SVECTOR *)&g_BattleSpecEnergyBallVelData3, &frame.beamMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecEnergyBallStateBuffer, &frame.beamMatrix, &frame.beamMatrix);
    ScaleMatrix(&frame.beamMatrix, (int *)&g_BattleSpecEnergyBallVelData2);
    frame.beamMatrix.t[0] = (g_BattleSpecEnergyBallBeamEnd.posX);
    frame.beamMatrix.t[1] = g_BattleSpecEnergyBallBeamEnd.posY;
    frame.beamMatrix.t[2] = g_BattleSpecEnergyBallStep;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.beamMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecEnergyBallTransformData3.transform3CountWord = g_BattleSpecEnergyBallBeamFade << 4;
    (g_BattleSpecEnergyBallTransformData3.resEntry) = Gfx_ModelTableGetRenderObject(17, g_BattleSpecEnergyBallResourceTable);
    g_BattleSpecEnergyBallDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecEnergyBallTransformData3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecEnergyBallDrawListHead);
    (g_BattleSpecEnergyBallTransformData3.resEntry) = Gfx_ModelTableGetRenderObject(18, g_BattleSpecEnergyBallResourceTable);
    g_BattleSpecEnergyBallDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecEnergyBallTransformData3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecEnergyBallDrawListHead);
    (g_BattleSpecEnergyBallTransformData3.resEntry) = Gfx_ModelTableGetRenderObject(19, g_BattleSpecEnergyBallResourceTable);
    g_BattleSpecEnergyBallDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecEnergyBallTransformData3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecEnergyBallDrawListHead);
    BattleFx_InstallBillboardMatrix(&g_BattleSpecEnergyBallBeamEnd.posX, 0x3000 /* 3.0 */, 0);
    g_BattleSpecEnergyBallSpriteRequest3.request3Byte3 = g_BattleSpecEnergyBallBeamFade * 20;
    g_BattleSpecEnergyBallSpriteRequest3.request3Byte2 = g_BattleSpecEnergyBallSpriteRequest3.request3Byte3;
    g_BattleSpecEnergyBallSpriteRequest3.request3Byte1 = g_BattleSpecEnergyBallSpriteRequest3.request3Byte2;
    g_BattleSpecEnergyBallSpriteRequest3.request3FrameParity = (frame.beamSlot->frameCounter % 2);
    g_BattleSpecEnergyBallDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecEnergyBallSpriteRequest3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecEnergyBallDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.beamSlot->frameCounter > 8) {
        g_BattleSpecEnergyBallBeamFade--;
    }
    (frame.beamSlot->frameCounter)++;
    if (frame.beamSlot->frameCounter == 16) {
        frame.beamSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws one randomly chosen fading model (resources 1 to 4) for four frames, shrinking it each frame.
 */
// FUNCTION: FF7 0x005110e0
void __cdecl BattleFx_SpecialEnergyBallFadingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix fadeMatrix;
        struct { unsigned char pad_to_20[0x20]; int resIdx1[4]; };
        struct { unsigned char pad_to_30[0x30]; BattleFxEnergyBallFadeTask *fadeTask; };
        struct { unsigned char pad_to_34[0x34]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.fadeTask = (BattleFxEnergyBallFadeTask *)BATTLE_CURRENT_TASK_SLOT;
    frame.resIdx1[0] = 1;
    frame.resIdx1[1] = 2;
    frame.resIdx1[2] = 3;
    frame.resIdx1[3] = 4;
    RotMatrixXYZ((SVECTOR *)&frame.fadeTask->angleX, &frame.fadeMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecEnergyBallStateBuffer, &frame.fadeMatrix, &frame.fadeMatrix);
    ScaleMatrix(&frame.fadeMatrix, (int *)&g_BattleSpecEnergyBallVelData4);
    frame.fadeMatrix.t[0] = frame.fadeTask->posX;
    frame.fadeMatrix.t[1] = frame.fadeTask->posY;
    frame.fadeMatrix.t[2] = frame.fadeTask->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.fadeMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecEnergyBallTransformData4.transform4Scale = 0x80 - (frame.fadeTask->step << 5);
    (g_BattleSpecEnergyBallTransformData4.transform4Value) = Gfx_ModelTableGetRenderObject(frame.resIdx1[frame.fadeTask->modelIndex], g_BattleSpecEnergyBallResourceTable);
    g_BattleSpecEnergyBallDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecEnergyBallTransformData4, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecEnergyBallDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.fadeTask->step)++;
    if (frame.fadeTask->step == 4) {
        frame.fadeTask->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
