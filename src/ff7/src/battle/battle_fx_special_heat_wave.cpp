// src/ff7/src/battle/battle_fx_special_heat_wave.cpp
/* Enemy Special Effect [idx 18]: HeatWave (JP: netu / 熱 / 熱風 (Heat Wave), asset: "SPECIAL/NETU") — Unused in retail scene.bin (loads netsu03.tim / netu1.rsd) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Netu attack: it loads resources and runs a timeline that
//   drives a white flash pulse, expanding and spinning model draws, and animated and rising sprite
//   tasks.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
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
// GLOBAL: FF7 0x008a37e8
unsigned int g_BattleSpecHeatWaveResourceDefs[100] ={
    0x001900b1,
    (unsigned int)&g_BattleSpecHeatWaveResName_wa,
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    (unsigned int)&g_BattleSpecHeatWaveResName_netu1,
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a3a44
    (unsigned int)"uchi1.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a3a50
    (unsigned int)"uchi2.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a3a5c
    (unsigned int)"uchi3.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a3a68
    (unsigned int)"uchi4.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a3a74
    (unsigned int)"uchi5.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a3a80
    (unsigned int)"uchi6.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a3a8c
    (unsigned int)"uchi7.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a3a98
    (unsigned int)"uchi8.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a3aa4
    (unsigned int)"uchi9.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a3ab0
    (unsigned int)"uchi10.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b4,
    // STRING: FF7 0x008a3abc
    (unsigned int)"isi1.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b4,
    // STRING: FF7 0x008a3ac8
    (unsigned int)"isi2.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b4,
    // STRING: FF7 0x008a3ad4
    (unsigned int)"isi3.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b4,
    // STRING: FF7 0x008a3ae0
    (unsigned int)"isi4.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a3aec
    (unsigned int)"netsu03.tim",
    // STRING: FF7 0x008a3af8
    (unsigned int)"hahen1.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    0x00000000,
    // STRING: FF7 0x008a3b04
    (unsigned int)"kemu1.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a3b10
    (unsigned int)"netsu03.tim",
    // STRING: FF7 0x008a3b1c
    (unsigned int)"hikari.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a3b28
    (unsigned int)"netsu03.tim",
    // STRING: FF7 0x008a3b34
    (unsigned int)"kage.spt"
};
// GLOBAL: FF7 0x008a3978
struct BattleSpecHeatWaveModelReq1 g_BattleSpecHeatWaveModelReq1 = {0, 0, {0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x008a3988
unsigned char g_BattleSpecHeatWaveModelIndices1[40] = {0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a39b0
struct BattleSpecHeatWaveSpriteReq1 g_BattleSpecHeatWaveSpriteReq1 = {0, 0, -128, -128, -128, {0x2e, 0, 0}};
// GLOBAL: FF7 0x008a39c0
struct BattleSpecHeatWaveSpriteReq2 g_BattleSpecHeatWaveSpriteReq2 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
// GLOBAL: FF7 0x008a39d0
unsigned char g_BattleSpecHeatWaveScaleVector[16] = {0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a39e0
struct BattleSpecHeatWaveModelReq2 g_BattleSpecHeatWaveModelReq2 = {0, {0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x008a39f0
unsigned char g_BattleSpecHeatWaveModelIndices2[16] = {0x0c, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a3a00
struct BattleSpecHeatWaveSpriteReq3 g_BattleSpecHeatWaveSpriteReq3 = {0, 0, -128, -128, -128, {0x2c}, 0};
// GLOBAL: FF7 0x008a3a10
struct BattleSpecHeatWaveModelReq2 g_BattleSpecHeatWaveModelReq3 = {0, {0x28, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x008a3a20
struct BattleSpecHeatWaveSpriteReq1 g_BattleSpecHeatWaveSpriteReq4 = {0, 0, -128, -128, -128, {0x2e, 0, 0}};
// GLOBAL: FF7 0x008a3a30
unsigned char g_BattleSpecHeatWaveResName_wa[7] = "wa.rsd";
// GLOBAL: FF7 0x008a3a38
unsigned char g_BattleSpecHeatWaveResName_netu1[10] = "netu1.rsd";
// clang-format on


// clang-format off
// GLOBAL: FF7 0x00b650d8
int g_BattleSpecHeatWaveDrawHandle = 0;
// GLOBAL: FF7 0x00b650dc
short g_BattleSpecHeatWaveTargetMask = 0;
// GLOBAL: FF7 0x00b650e0
int g_BattleSpecHeatWaveScaleX = 0;
// GLOBAL: FF7 0x00b650e4
int g_BattleSpecHeatWaveScaleY = 0;
// GLOBAL: FF7 0x00b650e8
int g_BattleSpecHeatWaveScaleZ = 0;
unsigned char g_bssPad_00b650ec[4] = {0};
// GLOBAL: FF7 0x00b650f0
short g_BattleSpecHeatWaveFlashStep = 0;
// GLOBAL: FF7 0x00b650f4
short g_BattleSpecHeatWaveFlashTimer = 0;
// GLOBAL: FF7 0x00b650f8
struct GfxTableCursor *g_BattleSpecHeatWaveLoadedResTable = 0;
unsigned char g_bssPad_00b650fc[4] = {0};
// clang-format on

/*
 * Resets the flash timer and step, and loads the SPECIAL/NETU model table when it is not loaded yet.
 */
// FUNCTION: FF7 0x0053bfa0
void __cdecl BattleFx_SpecialLoadResHeatWave(void)
{
    GfxModelTableBank *stage;
    GfxResourceRequest currentRecord;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    g_BattleSpecHeatWaveFlashTimer = 0;
    g_BattleSpecHeatWaveFlashStep = 16;
    if (g_pBattle3dScene != 0 && g_BattleSpecHeatWaveLoadedResTable == 0) {
        kind = 0x14;
        resourceDefsWord = (int)&g_BattleSpecHeatWaveResourceDefs;
        stage = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a3b40
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/NETU", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecHeatWaveLoadedResTable, stage);
    }
}

/*
 * Entry point for the Netu special effect; forwards to BattleFx_SpecialBeginHeatWave.
 */
// FUNCTION: FF7 0x0053c042
void __cdecl BattleFx_SpecialEnterHeatWave(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginHeatWave(targetMask, actorIdx);
}

/*
 * Loads the resources, stores the target mask, and starts the timeline task at the caster's bone34
 * position and the target mask's centroid.
 */
// FUNCTION: FF7 0x0053c057
void __cdecl BattleFx_SpecialBeginHeatWave(int targetMask, int actorIdx)
{
    BattleFxHeatWaveTimelineSlot *taskSlot;

    BattleFx_SpecialLoadResHeatWave();
    g_BattleSpecHeatWaveTargetMask = targetMask;
    taskSlot = (BattleFxHeatWaveTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHeatWaveTimelineTask);
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].bone34, taskSlot->bonePos);
    Battle_GetActorMaskCentroid(targetMask, taskSlot->targetPos);
}

/*
 * Timeline: spawns the expanding and spinning model tasks, the white flash, the fixed-point ramp
 * and the hit records.
 */
// FUNCTION: FF7 0x0053c0c1
void __cdecl BattleFx_SpecialHeatWaveTimelineTask(void)
{
    BattleTaskSlot *mainSelf;
    short i7;
    BattleFxPositionedSpriteSlot *pParticle;
    int s; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int t; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int y; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int a; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int b; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int c; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int d; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int e; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int f; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int g; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    mainSelf = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (mainSelf->frameCounter == 0) {
        pParticle = (BattleFxPositionedSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHeatWaveDrawExpandingModelsTask);
        pParticle->posX = 0;
        pParticle->posY = 0xFB50;
        pParticle->posZ = 0;
        pParticle = (BattleFxPositionedSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHeatWaveDrawSpinModelsEmitTask);
        pParticle->posX = 0;
        pParticle->posY = 0xFB50;
        pParticle->posZ = 0;
    }
    if (mainSelf->frameCounter == 12) {
        Battle_AllocateResourceSlot(BattleFx_SpecialHeatWaveWhiteFlashPulseTask);
    }
    if (mainSelf->frameCounter == 28) {
        Battle_StartFixedPointRamp(0, 16);
    }
    if (mainSelf->frameCounter == 35) {
        for (i7 = 0; i7 < 10; i7++) {
            if ((g_BattleSpecHeatWaveTargetMask & 1 << i7) != 0) {
                Battle_PostActorHitOrHurtMessage(i7);
            }
        }
    }
    (mainSelf->frameCounter)++;
    if (mainSelf->frameCounter == 50) {
        mainSelf->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the white screen flash, then ramps the flash timer up for sixteen frames and back down,
 * retiring after thirty-two.
 */
// FUNCTION: FF7 0x0053c20e
void __cdecl BattleFx_SpecialHeatWaveWhiteFlashPulseTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(*(char *)&g_BattleSpecHeatWaveFlashTimer, 0, 0);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter < 16) {
        g_BattleSpecHeatWaveFlashTimer += 15;
    }
    if (task->frameCounter > 16) {
        g_BattleSpecHeatWaveFlashTimer -= 15;
    }
    (task->frameCounter)++;
    if (task->frameCounter == 32) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the expanding model set and its sprite while shaking the camera focal point, then fades
 * the flash step out.
 */
// FUNCTION: FF7 0x0053c2b0
void __cdecl BattleFx_SpecialHeatWaveDrawExpandingModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleFxOilTargetModelSlot *expandingSlot; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.expandingSlot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecHeatWaveSpriteReq1.spriteResource) = Gfx_ModelTableGetLoadedRecord(18, g_BattleSpecHeatWaveLoadedResTable);
    g_BattleSpecHeatWaveSpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(18, g_BattleSpecHeatWaveLoadedResTable);
    frame.rotX.vz = 0;
    frame.rotX.vx = frame.rotX.vz;
    frame.rotX.vy = 0;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    g_BattleSpecHeatWaveScaleZ = frame.expandingSlot->frameCounter << 10;
    g_BattleSpecHeatWaveScaleY = g_BattleSpecHeatWaveScaleZ;
    g_BattleSpecHeatWaveScaleX = g_BattleSpecHeatWaveScaleY;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecHeatWaveScaleX);
    frame.rotMatrix.t[0] = frame.expandingSlot->posX;
    frame.rotMatrix.t[1] = frame.expandingSlot->posY;
    frame.rotMatrix.t[2] = frame.expandingSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    (g_BattleSpecHeatWaveModelReq1.modelResource) = Gfx_ModelTableGetRenderObject(1, g_BattleSpecHeatWaveLoadedResTable);
    g_BattleSpecHeatWaveModelReq1.flags = 0xA8;
    g_BattleSpecHeatWaveModelReq1.flashWord10 = 0x1000 - (g_BattleSpecHeatWaveFlashStep << 7);
    g_BattleSpecHeatWaveDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecHeatWaveModelReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecHeatWaveDrawHandle);
    (g_BattleSpecHeatWaveModelReq1.modelResource) = Gfx_ModelTableGetRenderObject(((int *)&g_BattleSpecHeatWaveModelIndices1)[frame.expandingSlot->frameCounter % 10], g_BattleSpecHeatWaveLoadedResTable);
    g_BattleSpecHeatWaveModelReq1.flags = 0x28;
    g_BattleSpecHeatWaveModelReq1.flashWord10 = g_BattleSpecHeatWaveFlashStep << 3;
    g_BattleSpecHeatWaveDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecHeatWaveModelReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecHeatWaveDrawHandle);
    BattleFx_InstallBillboardMatrix(&frame.expandingSlot->posX, 0x2000 /* 2.0 */, 0);
    g_BattleSpecHeatWaveSpriteReq1.colorB = g_BattleSpecHeatWaveFlashStep << 3;
    g_BattleSpecHeatWaveSpriteReq1.colorG = g_BattleSpecHeatWaveSpriteReq1.colorB;
    g_BattleSpecHeatWaveSpriteReq1.colorR = g_BattleSpecHeatWaveSpriteReq1.colorG;
    g_BattleSpecHeatWaveDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecHeatWaveSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecHeatWaveDrawHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    g_BattleCameraFocalPoint.focalPointY += (short)(32 - ((frame.expandingSlot->frameCounter % 2) << 6));
    if (frame.expandingSlot->frameCounter > 14) {
        g_BattleSpecHeatWaveFlashStep--;
    }
    (frame.expandingSlot->frameCounter)++;
    if (frame.expandingSlot->frameCounter == 30) {
        frame.expandingSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the spinning model and sprite, and emits the rising fade sprites, rising spinning model and
 * animated sprites.
 */
// FUNCTION: FF7 0x0053c53f
void __cdecl BattleFx_SpecialHeatWaveDrawSpinModelsEmitTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[112];
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxHeatWaveEmitterTask *spawnSlot; };
        struct { unsigned char pad_to_30[0x30]; short spawnIndex; };
        struct { unsigned char pad_to_34[0x34]; BattleFxOilTargetModelSlot *emitSlot; };
        struct { unsigned char pad_to_38[0x38]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_58[0x58]; short spawnSpeed; };
        struct { unsigned char pad_to_5C[0x5C]; short spawnAngle; };
        struct { unsigned char pad_to_60[0x60]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.emitSlot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecHeatWaveModelReq3.modelResource) = Gfx_ModelTableGetRenderObject(0, g_BattleSpecHeatWaveLoadedResTable);
    (g_BattleSpecHeatWaveSpriteReq4.spriteResource) = Gfx_ModelTableGetLoadedRecord(19, g_BattleSpecHeatWaveLoadedResTable);
    g_BattleSpecHeatWaveSpriteReq4.drawObj = Gfx_ModelTableGetSpriteObject(19, g_BattleSpecHeatWaveLoadedResTable);
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.emitSlot->frameCounter << 6;
    frame.rotX.vz = 0;
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    frame.scaleX.vz = frame.emitSlot->frameCounter * 0x7D0;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    frame.rotMatrix.t[0] = frame.emitSlot->posX;
    frame.rotMatrix.t[1] = frame.emitSlot->posY;
    frame.rotMatrix.t[2] = frame.emitSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecHeatWaveModelReq3.alpha = 0x80 - frame.emitSlot->frameCounter * 6;
    g_BattleSpecHeatWaveDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecHeatWaveModelReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecHeatWaveDrawHandle);
    frame.rotX.vy = 0;
    frame.rotX.vz = frame.rotX.vy;
    frame.rotX.vx = 0x400 /* 90 deg */;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.scaleX.vz = frame.emitSlot->frameCounter * 0x320;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    frame.rotMatrix.t[0] = frame.emitSlot->posX;
    frame.rotMatrix.t[1] = 0;
    frame.rotMatrix.t[2] = frame.emitSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecHeatWaveSpriteReq4.colorB = 0x80 - frame.emitSlot->frameCounter * 6;
    g_BattleSpecHeatWaveSpriteReq4.colorG = g_BattleSpecHeatWaveSpriteReq4.colorB;
    g_BattleSpecHeatWaveSpriteReq4.colorR = g_BattleSpecHeatWaveSpriteReq4.colorG;
    g_BattleSpecHeatWaveDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecHeatWaveSpriteReq4, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecHeatWaveDrawHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spawnSpeed = frame.emitSlot->frameCounter * 240;
    for (frame.spawnIndex = 0; frame.spawnIndex < 2; frame.spawnIndex++) {
        frame.spawnSlot = (BattleFxHeatWaveEmitterTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHeatWaveRisingFadeSpriteTask);
        frame.spawnAngle = (rand() % 4096);
        frame.spawnSlot->velocityX = CosFixed(frame.spawnAngle) * frame.spawnSpeed >> 12;
        frame.spawnSlot->velocityZ = SinFixed(frame.spawnAngle) * frame.spawnSpeed >> 12;
        frame.spawnSlot->velocityY = 0;
        frame.spawnSlot->variant = (rand() % 16);
    }
    frame.spawnSlot = (BattleFxHeatWaveEmitterTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHeatWaveRisingSpinningModelTask);
    frame.spawnAngle = (rand() % 4096);
    frame.spawnSlot->velocityX = CosFixed(frame.spawnAngle) * frame.spawnSpeed >> 12;
    frame.spawnSlot->velocityZ = SinFixed(frame.spawnAngle) * frame.spawnSpeed >> 12;
    frame.spawnSlot->velocityY = 0;
    frame.spawnSlot->variant = (rand() % 4);
    frame.spawnSlot->rotation = (rand() % 4096);
    for (frame.spawnIndex = 0; frame.spawnIndex < 4; frame.spawnIndex++) {
        frame.spawnSlot = (BattleFxHeatWaveEmitterTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHeatWaveAnimatedSpriteTask);
        frame.spawnAngle = (rand() % 4096);
        frame.spawnSlot->velocityX = CosFixed(frame.spawnAngle) * frame.spawnSpeed >> 12;
        frame.spawnSlot->velocityZ = SinFixed(frame.spawnAngle) * frame.spawnSpeed >> 12;
        frame.spawnSlot->velocityY = 0;
    }
    (frame.emitSlot->frameCounter)++;
    if (frame.emitSlot->frameCounter == 20) {
        frame.emitSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws a billboarded animated sprite from table entry 0x11 at the slot's position. When unpaused
 * it advances the frame counter and retires at the sprite's frame count.
 */
// FUNCTION: FF7 0x0053c9a5
void __cdecl BattleFx_SpecialHeatWaveAnimatedSpriteTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecHeatWaveSpriteReq2.spriteResource) = Gfx_ModelTableGetLoadedRecord(17, g_BattleSpecHeatWaveLoadedResTable);
    g_BattleSpecHeatWaveSpriteReq2.drawObj = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    BattleFx_InstallBillboardMatrix((short *)task->data, 0x2000 /* 2.0 */, 0);
    g_BattleSpecHeatWaveSpriteReq2.frame = task->frameCounter;
    g_BattleSpecHeatWaveDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecHeatWaveSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecHeatWaveDrawHandle);
    if (g_BattleIsPaused == 0 && (g_BattleSpecHeatWaveSpriteReq2.spriteResource) != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= *(int *)((char *)*(int *)(g_BattleSpecHeatWaveSpriteReq2.spriteResource) + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws a rising, spinning model that fades out and retires after fifteen frames.
 */
// FUNCTION: FF7 0x0053ca76
void __cdecl BattleFx_SpecialHeatWaveRisingSpinningModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleFxHeatWaveRisingSpinningModelSlot *risingSlot; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.risingSlot = (BattleFxHeatWaveRisingSpinningModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vy = 0;
    frame.rotX.vx = frame.rotX.vy;
    frame.rotX.vz = frame.risingSlot->spinAngle;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, (int *)&g_BattleSpecHeatWaveScaleVector);
    frame.rotMatrix.t[0] = frame.risingSlot->posX;
    frame.rotMatrix.t[1] = frame.risingSlot->posY;
    frame.rotMatrix.t[2] = frame.risingSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecHeatWaveModelReq2.alpha = 0x80 - (frame.risingSlot->frameCounter << 3);
    (g_BattleSpecHeatWaveModelReq2.modelResource) = Gfx_ModelTableGetRenderObject(((int *)&g_BattleSpecHeatWaveModelIndices2)[frame.risingSlot->modelIndex], g_BattleSpecHeatWaveLoadedResTable);
    g_BattleSpecHeatWaveDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecHeatWaveModelReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecHeatWaveDrawHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.risingSlot->posY -= 100;
    frame.risingSlot->spinAngle += 0x100 /* 22.5 deg */;
    (frame.risingSlot->frameCounter)++;
    if (frame.risingSlot->frameCounter == 15) {
        frame.risingSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws a billboarded sprite from table entry 0x10 whose colour fades with fadeStep. When unpaused
 * it lowers posY by 0x12C and retires the slot when fadeStep reaches 15.
 */
// FUNCTION: FF7 0x0053cbcf
void __cdecl BattleFx_SpecialHeatWaveRisingFadeSpriteTask(void)
{
    BattleFxHeatWaveRisingFadeSprite *task;

    task = (BattleFxHeatWaveRisingFadeSprite *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecHeatWaveSpriteReq3.spriteResource) = Gfx_ModelTableGetLoadedRecord(16, g_BattleSpecHeatWaveLoadedResTable);
    g_BattleSpecHeatWaveSpriteReq3.drawObj = Gfx_ModelTableGetSpriteObject(16, g_BattleSpecHeatWaveLoadedResTable);
    BattleFx_InstallBillboardMatrix(&task->posX, 0x2000 /* 2.0 */, 0);
    g_BattleSpecHeatWaveSpriteReq3.frame = task->variant;
    g_BattleSpecHeatWaveSpriteReq3.fadeByte10 = 0x80 - (task->fadeStep << 3);
    g_BattleSpecHeatWaveSpriteReq3.colorG = g_BattleSpecHeatWaveSpriteReq3.fadeByte10;
    g_BattleSpecHeatWaveSpriteReq3.colorR = g_BattleSpecHeatWaveSpriteReq3.colorG;
    g_BattleSpecHeatWaveDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecHeatWaveSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecHeatWaveDrawHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posY -= 300;
    (task->fadeStep)++;
    if (task->fadeStep == 15) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
