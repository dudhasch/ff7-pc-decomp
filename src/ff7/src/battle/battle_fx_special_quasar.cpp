// src/ff7/src/battle/battle_fx_special_quasar.cpp
/* Enemy Special Effect [idx 32]: Quasar (JP: kueisa / クェーサー (Quasar, quasar03.tim), asset: "SPECIAL/KUEISA") — Stardust March (0x353, Christopher) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Kueisa attack: it loads resources and runs a timeline
//   that spawns beam components from pulsing sprites, then drives beam-aligned sprites, a beam
//   core, drifting and growing fade sprites, and spinning and expanding models.

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
// clang-format off
// GLOBAL: FF7 0x008a49f8
unsigned int g_BattleSpecialQuasarResourceDefs[128] = {0x001900b1, (unsigned int)"kueisa.rsd", 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)"soto1.rsd", 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)"soto2.rsd", 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)"soto3.rsd", 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)"soto4.rsd", 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)"soto5.rsd", 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)"soto6.rsd", 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)"soto7.rsd", 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)"soto8.rsd", 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)"soto9.rsd", 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)"soto10.rsd", 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)"elec1.rsd", 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)"isi1.rsd", 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)"isi2.rsd", 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)"isi3.rsd", 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)"isi4.rsd", 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)"isi5.rsd", 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)"isi6.rsd", 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)"baku1.rsd", 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)"soko1.rsd", 0x00000000, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"quasar03.tim", (unsigned int)"hikari1.spt", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"quasar03.tim", (unsigned int)"wa.spt", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"quasar03.tim", (unsigned int)"maru1.spt", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"quasar03.tim", (unsigned int)"bimu1.spt", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"quasar03.tim", (unsigned int)"bimu2.spt", 0x00000000, 0x00000000, 0x00192a39, 0x00000000, (unsigned int)"jo_c01.spt"};
// GLOBAL: FF7 0x008a4bf8
struct BattleSpecialQuasarMatrix g_BattleSpecialQuasarMatrix = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x80, 0x80, 0x80, 0x2c, 0, 0, 0, 0}, 0, 0};
// GLOBAL: FF7 0x008a4c18
struct BattleSpecialQuasarColor1 g_BattleSpecialQuasarColor1 = {-128, -128, -128, {0x2c}, 0, {0}};
// GLOBAL: FF7 0x008a4c20
int g_BattleSpecialQuasarAlignedScaleX = 4096;
// GLOBAL: FF7 0x008a4c24
int g_BattleSpecialQuasarAlignedScaleY = 4096;
// GLOBAL: FF7 0x008a4c28
int g_BattleSpecialQuasarAlignedScaleZ = 4096;
// GLOBAL: FF7 0x008a4c30
short g_BattleSpecialQuasarModelIndices1[8] = {1, 2, 3, 4, 5, 6, 7, 8};
unsigned char g_rawData_008a4c40[4] = {0x09, 0x00, 0x0a, 0x00};
unsigned char g_rawData_008a4c44[4] = {0x0b, 0x00, 0x0c, 0x00};
unsigned char g_rawData_008a4c48[16] = {0x0d, 0x00, 0x0e, 0x00, 0x0f, 0x00, 0x10, 0x00, 0x0d, 0x00, 0x0e, 0x00, 0x0f, 0x00, 0x10, 0x00};
// GLOBAL: FF7 0x008a4c58
short g_BattleSpecialQuasarModelIndices2[8] = {1, 2, 3, 4, 5, 6, 7, 8};
unsigned char g_rawData_008a4c68[8] = {0x05, 0x00, 0x06, 0x00, 0x07, 0x00, 0x08, 0x00};
// GLOBAL: FF7 0x008a4c70
BattleSpecialQuasarModelIndexTable g_BattleSpecialQuasarModelIndicesTable = {{g_BattleSpecialQuasarModelIndices1, g_BattleSpecialQuasarModelIndices2}, 0, 0};
#undef g_BattleSpecialQuasarAnimParams
// GLOBAL: FF7 0x008a4c80
struct BattleSpecialQuasarAnimParams g_BattleSpecialQuasarAnimParams = {-128, -128, -128, {0x2c, 0, 0, 0, 0}, 0, {0x28, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}, 0, {0}, 128};
unsigned char g_rawData_008a4ca4[4] = {0x20, 0x00, 0x00, 0x00};
// Model-table indices of the six spinning beam models, picked by the spin task's modelIndex.
// GLOBAL: FF7 0x008a4ca8
int g_BattleSpecialQuasarModelIndices3[6] = {0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11};
// GLOBAL: FF7 0x008a4cc0
struct BattleSpecialQuasarModelReq g_BattleSpecialQuasarModelReq = {0, 0, {0}, 0, {0x20, 0, 0, 0}};
// Model-table indices of the beam core's ten animation frames. The beam core task reads ten
// entries: the last eight are the first bytes of g_BattleSpecialQuasarTable.
// GLOBAL: FF7 0x008a4cd0
int g_BattleSpecialQuasarModelIndices4[2] = {1, 2};
// GLOBAL: FF7 0x008a4cd8
struct BattleSpecialQuasarTable g_BattleSpecialQuasarTable = {{3, 0, 0, 0, 4, 0, 0, 0, 5, 0, 0, 0, 6, 0, 0, 0, 7, 0, 0, 0, 8, 0, 0, 0, 9, 0, 0, 0, 0xa, 0, 0, 0}, 0, 0};
#undef g_BattleSpecialQuasarColor2
// GLOBAL: FF7 0x008a4d00
struct BattleSpecialQuasarColor2 g_BattleSpecialQuasarColor2 = {-128, -128, -128, {0x2c, 0, 0, 0, 0}, 0, 0};
// GLOBAL: FF7 0x008a4d10
struct BattleSpecialQuasarColor1 g_BattleSpecialQuasarColor3 = {-128, -128, -128, {0x2c}, 0, {0}};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// clang-format on


// clang-format off
// clang-format on

// clang-format off
// GLOBAL: FF7 0x00b6d358
PsxMatrix g_BattleSpecialQuasarBeamBasis = {0};
// GLOBAL: FF7 0x00b6d378
BattleSpecialEnergyOrbCentroidPos g_BattleSpecialQuasarCentroidVec = {0};
// GLOBAL: FF7 0x00b6d37c
short g_BattleSpecialQuasarCentroidZ = 0;
unsigned char g_bssPad_00b6d380[4] = {0};
// GLOBAL: FF7 0x00b6d384
int g_BattleSpecialQuasarDrawListHandle = 0;
// GLOBAL: FF7 0x00b6d388
short g_BattleSpecialQuasarTargetMask = 0;
unsigned char g_bssPad_00b6d38c[4] = {0};
// GLOBAL: FF7 0x00b6d390
int g_BattleSpecialQuasarExpandScaleX = 0;
// GLOBAL: FF7 0x00b6d394
int g_BattleSpecialQuasarExpandScaleY = 0;
// GLOBAL: FF7 0x00b6d398
int g_BattleSpecialQuasarExpandScaleZ = 0;
unsigned char g_bssPad_00b6d39c[4] = {0};
// GLOBAL: FF7 0x00b6d3a0
int g_BattleSpecialQuasarSpinScaleX = 0;
// GLOBAL: FF7 0x00b6d3a4
int g_BattleSpecialQuasarSpinScaleY = 0;
// GLOBAL: FF7 0x00b6d3a8
int g_BattleSpecialQuasarSpinScaleZ = 0;
unsigned char g_bssPad_00b6d3ac[4] = {0};
// GLOBAL: FF7 0x00b6d3b0
int g_BattleSpecialQuasarCoreScaleX = 0;
// GLOBAL: FF7 0x00b6d3b4
int g_BattleSpecialQuasarCoreScaleY = 0;
// GLOBAL: FF7 0x00b6d3b8
int g_BattleSpecialQuasarCoreScaleZ = 0;
unsigned char g_bssPad_00b6d3bc[4] = {0};
// GLOBAL: FF7 0x00b6d3c0
short g_BattleSpecialQuasarGlowLevel = 0;
// GLOBAL: FF7 0x00b6d3c4
short g_BattleSpecialQuasarBeamLength = 0;
// GLOBAL: FF7 0x00b6d3c8
short g_BattleSpecialQuasarCoreFade = 0;
// GLOBAL: FF7 0x00b6d3cc
struct GfxTableCursor *g_BattleSpecialQuasarModelTable = 0;
// GLOBAL: FF7 0x00b6d3d0
short g_BattleSpecialQuasarPulseSpriteFrame = 0;
unsigned char g_bssPad_00b6d3d4[4] = {0};
// clang-format on

/*
 * Loads resources for the Kueisa special effect when not already loaded.
 */
// FUNCTION: FF7 0x00546030
void __cdecl BattleFx_SpecialLoadResQuasar(void)
{
    GfxModelTableBank *unitOwner;
    GfxResourceRequest requestSlot;
    int resCategory;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    g_BattleSpecialQuasarCoreFade = 0;
    g_BattleSpecialQuasarGlowLevel = 0x10;
    if (g_pBattle3dScene != 0 && g_BattleSpecialQuasarModelTable == 0) {
        resCategory = 0x1A;
        resourceDefsWord = (int)&g_BattleSpecialQuasarResourceDefs;
        unitOwner = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a4e9c
        pRequest->resourcePath = (char *)BattleFx_BuildResourcePath("SPECIAL/KUEISA", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(resCategory, resourceDefsWord, pRequest, &g_BattleSpecialQuasarModelTable, unitOwner);
    }
}

/*
 * Entry point for the Kueisa special effect, dispatching to begin.
 */
// FUNCTION: FF7 0x005460d2
void __cdecl BattleFx_SpecialEnterQuasar(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginQuasar(targetMask, actorIdx);
}

/*
 * Initializes and begins the Kueisa special effect for target actors.
 */
// FUNCTION: FF7 0x005460e7
void __cdecl BattleFx_SpecialBeginQuasar(int targetMask, int actorIdx)
{
    int taskRecord;

    BattleFx_SpecialLoadResQuasar();
    g_BattleSpecialQuasarTargetMask = targetMask;
    taskRecord = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialQuasarTimelineTask);
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].bone34, (short *)(taskRecord + 4));
    Battle_GetActorMaskCentroid(targetMask, &g_BattleSpecialQuasarCentroidVec.centroidX);
    Battle_PlaySfxPannedToActor(0x131, (short)actorIdx, 0);
}

/*
 * Timeline task controlling state progression for the Kueisa special effect.
 */
// FUNCTION: FF7 0x00546163
void __cdecl BattleFx_SpecialQuasarTimelineTask(void)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxQuasarTimeline *pCur;
    short i8;
    BattleFxPositionedSpriteSlot *s;
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
    int h; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int j; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    pCur = (BattleFxQuasarTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (pCur->step == 0) {
        s = (BattleFxPositionedSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialQuasarPulsingSpriteSpawnBeamTask);
        s->posX = pCur->posX;
        s->posY = pCur->posY;
        s->posZ = pCur->posZ;
        g_BattleFxTimelineState.startValue = 0x1000;
    }
    if (pCur->step == 13) {
        BattleFx_StartScreenFlashRamp(2);
    }
    if (pCur->step == 15) {
        BattleFx_FadeOutScreenFlashRamp(15);
    }
    if (pCur->step == 70) {
        for (i8 = 0; i8 < 10; i8++) {
            if ((g_BattleSpecialQuasarTargetMask & 1 << i8) != 0) {
                Battle_PostActorHitOrHurtMessage(i8);
            }
        }
    }
    (pCur->step)++;
    if (pCur->step == 80) {
        Battle_StartFixedPointRamp(0, 20);
        pCur->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task to animate pulsing sprites and spawn beam components for Kueisa.
 */
// FUNCTION: FF7 0x00546298
void __cdecl BattleFx_SpecialQuasarPulsingSpriteSpawnBeamTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[20];
        BattleFxQuasarBeamTask *spawnSlot;
        struct { unsigned char pad_to_4[0x4]; short beamIndex; };
        struct { unsigned char pad_to_8[0x8]; SVECTOR dirX; };
        struct { unsigned char pad_to_10[0x10]; BattleFxOilTargetModelSlot *pulseSlot; };
    } frame;
#pragma pack(pop)

    frame.pulseSlot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialQuasarColor2.spriteRecord = Gfx_ModelTableGetLoadedRecord(20, g_BattleSpecialQuasarModelTable);
    g_BattleSpecialQuasarColor2.spriteObject = Gfx_ModelTableGetSpriteObject(20, g_BattleSpecialQuasarModelTable);
    BattleFx_InstallBillboardMatrix(&frame.pulseSlot->posX, (SinFixed(frame.pulseSlot->frameCounter * 0x300) << 11 >> 12) + 0x3000, 0);
    g_BattleSpecialQuasarColor3.spriteFrame = g_BattleSpecialQuasarPulseSpriteFrame;
    g_BattleSpecialQuasarColor3.blue = g_BattleSpecialQuasarGlowLevel << 3;
    g_BattleSpecialQuasarColor3.green = g_BattleSpecialQuasarColor3.blue;
    g_BattleSpecialQuasarColor3.red = g_BattleSpecialQuasarColor3.green;
    g_BattleSpecialQuasarDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialQuasarColor2.spriteRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialQuasarDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.pulseSlot->frameCounter < 15) {
        frame.pulseSlot->posY -= 100;
    }
    if (frame.pulseSlot->frameCounter == 15) {
        g_BattleSpecialQuasarPulseSpriteFrame++;
        frame.spawnSlot = (BattleFxQuasarBeamTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialQuasarGrowingFadeSpriteTask);
        frame.spawnSlot->posX = frame.pulseSlot->posX;
        frame.spawnSlot->posY = frame.pulseSlot->posY;
        frame.spawnSlot->posZ = frame.pulseSlot->posZ;
        frame.spawnSlot->scale = 0x800 /* 0.5 */;
        frame.spawnSlot = (BattleFxQuasarBeamTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialQuasarBeamCoreTask);
        frame.spawnSlot->posX = frame.pulseSlot->posX;
        frame.spawnSlot->posY = frame.pulseSlot->posY;
        frame.spawnSlot->posZ = frame.pulseSlot->posZ;
        frame.dirX.vx = g_BattleSpecialQuasarCentroidVec.centroidX - frame.pulseSlot->posX;
        frame.dirX.vy = g_BattleSpecialQuasarCentroidVec.centroidY - frame.pulseSlot->posY;
        frame.dirX.vz = g_BattleSpecialQuasarCentroidZ - frame.pulseSlot->posZ;
        Gte_BuildLookBasisFromDirection(&frame.dirX.vx, (short *)&g_BattleSpecialQuasarBeamBasis);
        g_BattleSpecialQuasarBeamLength = SqrtToInt(frame.dirX.vx * frame.dirX.vx + frame.dirX.vy * frame.dirX.vy + frame.dirX.vz * frame.dirX.vz);
        for (frame.beamIndex = 0; frame.beamIndex < 10; frame.beamIndex++) {
            frame.spawnSlot = (BattleFxQuasarBeamTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialQuasarBeamAlignedSpriteTask);
            frame.spawnSlot->posX = frame.pulseSlot->posX;
            frame.spawnSlot->posY = frame.pulseSlot->posY;
            frame.spawnSlot->posZ = frame.pulseSlot->posZ;
            frame.spawnSlot->scale = (g_BattleSpecialQuasarBeamLength << 12) / 2000;
            frame.spawnSlot->rotX = (rand() % 1024) - 0x200;
            frame.spawnSlot->rotY = (rand() % 1024) + 0x200;
            frame.spawnSlot->rotZ = 0x400 /* 90 deg */;
            frame.spawnSlot->timer = (rand() % 8);
            frame.spawnSlot->modelIndex = (frame.beamIndex % 2);
        }
    }
    if (frame.pulseSlot->frameCounter > 15) {
        g_BattleSpecialQuasarGlowLevel--;
    }
    (frame.pulseSlot->frameCounter)++;
    if (frame.pulseSlot->frameCounter == 30) {
        frame.pulseSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task to animate beam-aligned sprites for the Kueisa special effect.
 */
// FUNCTION: FF7 0x0054660e
void __cdecl BattleFx_SpecialQuasarBeamAlignedSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[128];
        int rampValue;
        struct { unsigned char pad_to_34[0x34]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_54[0x54]; short alphaLevel; };
        struct { unsigned char pad_to_58[0x58]; short timerLimit; };
        struct { unsigned char pad_to_5C[0x5C]; BattleFxQuasarBeamAlignedSpriteSlot *alignedSlot; };
        struct { unsigned char pad_to_60[0x60]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.alignedSlot = (BattleFxQuasarBeamAlignedSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.alignedSlot->frameCounter < 47) {
        frame.rampValue = 8;
    } else {
        frame.rampValue = 0x37 - frame.alignedSlot->frameCounter;
    }
    frame.alphaLevel = (short)frame.rampValue;
    RotMatrixXYZ(&frame.alignedSlot->rotation, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialQuasarBeamBasis, &frame.rotMatrix, &frame.rotMatrix);
    g_BattleSpecialQuasarAlignedScaleY = frame.alignedSlot->scaleY;
    g_BattleSpecialQuasarAlignedScaleZ = 0x2000 /* 2.0 */;
    g_BattleSpecialQuasarAlignedScaleX = g_BattleSpecialQuasarAlignedScaleZ;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialQuasarAlignedScaleX);
    frame.rotMatrix.t[0] = frame.alignedSlot->posX;
    frame.rotMatrix.t[1] = frame.alignedSlot->posY;
    frame.rotMatrix.t[2] = frame.alignedSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    if (frame.alignedSlot->modelIndex == 0) {
        g_BattleSpecialQuasarMatrix.spriteRecord = Gfx_ModelTableGetLoadedRecord(23, g_BattleSpecialQuasarModelTable);
        g_BattleSpecialQuasarMatrix.spriteObject = Gfx_ModelTableGetSpriteObject(23, g_BattleSpecialQuasarModelTable);
    } else {
        g_BattleSpecialQuasarMatrix.spriteRecord = Gfx_ModelTableGetLoadedRecord(24, g_BattleSpecialQuasarModelTable);
        g_BattleSpecialQuasarMatrix.spriteObject = Gfx_ModelTableGetSpriteObject(24, g_BattleSpecialQuasarModelTable);
    }
    g_BattleSpecialQuasarColor1.spriteFrame = g_BattleSpecialQuasarModelIndicesTable.modelIndices[frame.alignedSlot->modelIndex][frame.alignedSlot->timer];
    g_BattleSpecialQuasarColor1.blue = frame.alphaLevel << 4;
    g_BattleSpecialQuasarColor1.green = g_BattleSpecialQuasarColor1.blue;
    g_BattleSpecialQuasarColor1.red = g_BattleSpecialQuasarColor1.green;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecialQuasarMatrix.spriteRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.timerLimit = (frame.alignedSlot->modelIndex != 0 ? 0xFFFFFFF8 : 0) + 20;
    (frame.alignedSlot->timer)++;
    if (frame.alignedSlot->timer == frame.timerLimit) {
        frame.alignedSlot->timer -= 4;
    }
    (frame.alignedSlot->frameCounter)++;
    if (frame.alignedSlot->frameCounter == 55) {
        frame.alignedSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task managing core beam geometry and rendering for Kueisa.
 */
// FUNCTION: FF7 0x00546846
void __cdecl BattleFx_SpecialQuasarBeamCoreTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[100];
        int rotPoint;
        struct { unsigned char pad_to_4[0x4]; SVECTOR angleX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxQuasarBeamCoreSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_34[0x34]; SVECTOR pointX; };
        struct { unsigned char pad_to_40[0x40]; BattleFxOilTargetModelSlot *coreSlot; };
        struct { unsigned char pad_to_44[0x44]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.coreSlot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.angleX.vz = 0;
    frame.angleX.vy = frame.angleX.vz;
    frame.angleX.vx = frame.angleX.vy;
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    g_BattleSpecialQuasarCoreScaleZ = 0x1000 /* 1.0 */;
    g_BattleSpecialQuasarCoreScaleY = g_BattleSpecialQuasarCoreScaleZ;
    g_BattleSpecialQuasarCoreScaleX = g_BattleSpecialQuasarCoreScaleY;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialQuasarCoreScaleX);
    frame.rotMatrix.t[0] = frame.coreSlot->posX;
    frame.rotMatrix.t[1] = frame.coreSlot->posY;
    frame.rotMatrix.t[2] = frame.coreSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecialQuasarModelReq.flags = 8;
    g_BattleSpecialQuasarModelReq.fadeLevel = g_BattleSpecialQuasarCoreFade << 4;
    g_BattleSpecialQuasarModelReq.modelGroup = Gfx_ModelTableGetRenderObject(g_BattleSpecialQuasarModelIndices4[frame.coreSlot->frameCounter % 10], g_BattleSpecialQuasarModelTable);
    g_BattleSpecialQuasarDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialQuasarModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialQuasarDrawListHandle);
    frame.angleX.vx = 0;
    frame.angleX.vy = frame.coreSlot->frameCounter << 9;
    frame.angleX.vz = frame.coreSlot->frameCounter << 8;
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    g_BattleSpecialQuasarCoreScaleZ = 0x1000 /* 1.0 */;
    g_BattleSpecialQuasarCoreScaleY = g_BattleSpecialQuasarCoreScaleZ;
    g_BattleSpecialQuasarCoreScaleX = g_BattleSpecialQuasarCoreScaleY;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialQuasarCoreScaleX);
    frame.rotMatrix.t[0] = frame.coreSlot->posX;
    frame.rotMatrix.t[1] = frame.coreSlot->posY;
    frame.rotMatrix.t[2] = frame.coreSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecialQuasarModelReq.modelGroup = Gfx_ModelTableGetRenderObject(11, g_BattleSpecialQuasarModelTable);
    g_BattleSpecialQuasarModelReq.fadeLevel = (((rand() % 64) + 64) * g_BattleSpecialQuasarCoreFade) / 8;
    g_BattleSpecialQuasarDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialQuasarModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialQuasarDrawListHandle);
    frame.angleX.vx = 0;
    frame.angleX.vy = frame.coreSlot->frameCounter << 6;
    frame.angleX.vz = frame.coreSlot->frameCounter << 6;
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    g_BattleSpecialQuasarCoreScaleZ = 0x1000 /* 1.0 */;
    g_BattleSpecialQuasarCoreScaleY = g_BattleSpecialQuasarCoreScaleZ;
    g_BattleSpecialQuasarCoreScaleX = g_BattleSpecialQuasarCoreScaleY;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialQuasarCoreScaleX);
    frame.rotMatrix.t[0] = frame.coreSlot->posX;
    frame.rotMatrix.t[1] = frame.coreSlot->posY;
    frame.rotMatrix.t[2] = frame.coreSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialQuasarModelReq.fadeLevel = 4096 - (g_BattleSpecialQuasarCoreFade << 9);
    g_BattleSpecialQuasarModelReq.flags = 0x80;
    g_BattleSpecialQuasarModelReq.modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialQuasarModelTable);
    g_BattleSpecialQuasarDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialQuasarModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialQuasarDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.coreSlot->frameCounter < 8) {
        g_BattleSpecialQuasarCoreFade++;
    }
    if (frame.coreSlot->frameCounter > 42) {
        g_BattleSpecialQuasarCoreFade--;
    }
    if ((frame.coreSlot->frameCounter % 2) == 0) {
        frame.spawnSlot = (BattleFxQuasarBeamCoreSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialQuasarDriftingFadeSpriteTask);
        frame.spawnSlot->posX = frame.coreSlot->posX;
        frame.spawnSlot->posY = frame.coreSlot->posY;
        frame.spawnSlot->posZ = frame.coreSlot->posZ;
        frame.spawnSlot->scale = (rand() % 2048) + 0x800;
        frame.angleX.vx = (rand() % 1024) - 0x200;
        frame.angleX.vy = (rand() % 1024) - 0x200;
        frame.angleX.vz = 0;
        RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
        ConcatMatrixIntoFirst(&frame.rotMatrix, &g_BattleSpecialQuasarBeamBasis);
        frame.rotMatrix.t[2] = 0;
        frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
        frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
        SetRotMatrix(&frame.rotMatrix);
        SetTransMatrix(&frame.rotMatrix);
        frame.pointX.vx = 0;
        frame.pointX.vy = 0;
        frame.pointX.vz = g_BattleSpecialQuasarBeamLength / 15;
        RotTransPointToInt16(&frame.pointX, &frame.spawnSlot->rotation.vx, &frame.rotPoint);
    }
    if (frame.coreSlot->frameCounter % 3 == 0) {
        frame.spawnSlot = (BattleFxQuasarBeamCoreSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialQuasarBeamSpinModelTask);
        frame.spawnSlot->posX = frame.coreSlot->posX;
        frame.spawnSlot->posY = frame.coreSlot->posY;
        frame.spawnSlot->posZ = frame.coreSlot->posZ;
        frame.spawnSlot->variant = rand() % 6;
        frame.spawnSlot->scale = 0x800 /* 0.5 */;
        frame.angleX.vx = (rand() % 512) - 0x100;
        frame.angleX.vy = (rand() % 512) - 0x100;
        frame.angleX.vz = 0;
        RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
        ConcatMatrixIntoFirst(&frame.rotMatrix, &g_BattleSpecialQuasarBeamBasis);
        frame.rotMatrix.t[2] = 0;
        frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
        frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
        SetRotMatrix(&frame.rotMatrix);
        SetTransMatrix(&frame.rotMatrix);
        frame.pointX.vx = 0;
        frame.pointX.vy = 0;
        frame.pointX.vz = g_BattleSpecialQuasarBeamLength / 15;
        RotTransPointToInt16(&frame.pointX, &frame.spawnSlot->rotation.vx, &frame.rotPoint);
    }
    (frame.coreSlot->frameCounter)++;
    if (frame.coreSlot->frameCounter == 50) {
        frame.coreSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task to animate drifting fade sprites for the Kueisa special effect.
 */
// FUNCTION: FF7 0x00546e86
void __cdecl BattleFx_SpecialQuasarDriftingFadeSpriteTask(void)
{
    BattleFxQuasarDriftTask *taskPtr;
    short fadeLevel;
    int raw;

    taskPtr = (BattleFxQuasarDriftTask *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialQuasarModelIndicesTable.spriteRecord = Gfx_ModelTableGetLoadedRecord(22, g_BattleSpecialQuasarModelTable);
    g_BattleSpecialQuasarModelIndicesTable.spriteObject = Gfx_ModelTableGetSpriteObject(22, g_BattleSpecialQuasarModelTable);
    if (taskPtr->fadeStep < 7) {
        raw = 8;
    } else {
        raw = 0xF - taskPtr->fadeStep;
    }
    fadeLevel = raw;
    BattleFx_InstallBillboardMatrix(&taskPtr->posX, (short)taskPtr->scale, 0);
    g_BattleSpecialQuasarAnimParams.blue = fadeLevel << 4;
    g_BattleSpecialQuasarAnimParams.green = g_BattleSpecialQuasarAnimParams.blue;
    (g_BattleSpecialQuasarAnimParams.red) = g_BattleSpecialQuasarAnimParams.green;
    g_BattleSpecialQuasarDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialQuasarModelIndicesTable.spriteRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialQuasarDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    taskPtr->posX += taskPtr->velX;
    taskPtr->posY += taskPtr->velY;
    taskPtr->posZ += taskPtr->velZ;
    (taskPtr->fadeStep)++;
    if (taskPtr->fadeStep == 15) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task to animate spinning models along the beam for Kueisa.
 */
// FUNCTION: FF7 0x00546fd0
void __cdecl BattleFx_SpecialQuasarBeamSpinModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int rampValue;
        struct { unsigned char pad_to_8[0x8]; SVECTOR angleX; };
        struct { unsigned char pad_to_10[0x10]; short animScale; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxPositionedSpriteSlot *spawnSlot; };
        struct { unsigned char pad_to_38[0x38]; BattleFxQuasarBeamSpinModelSlot *spinModelSlot; };
        struct { unsigned char pad_to_3C[0x3C]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.spinModelSlot = (BattleFxQuasarBeamSpinModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.animScale = 0;
    if (frame.spinModelSlot->frameCounter < 12) {
        frame.rampValue = 8;
    } else {
        frame.rampValue = 0x14 - frame.spinModelSlot->frameCounter;
    }
    frame.animScale = (short)frame.rampValue;
    frame.angleX.vx = 0;
    frame.angleX.vy = frame.spinModelSlot->frameCounter << 9;
    frame.angleX.vz = frame.spinModelSlot->frameCounter << 8;
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    g_BattleSpecialQuasarSpinScaleZ = frame.spinModelSlot->scaleZ;
    g_BattleSpecialQuasarSpinScaleY = g_BattleSpecialQuasarSpinScaleZ;
    g_BattleSpecialQuasarSpinScaleX = g_BattleSpecialQuasarSpinScaleY;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialQuasarSpinScaleX);
    frame.rotMatrix.t[0] = frame.spinModelSlot->posX;
    frame.rotMatrix.t[1] = frame.spinModelSlot->posY;
    frame.rotMatrix.t[2] = frame.spinModelSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecialQuasarAnimParams.spinModelGroup = Gfx_ModelTableGetRenderObject(g_BattleSpecialQuasarModelIndices3[frame.spinModelSlot->modelIndex], g_BattleSpecialQuasarModelTable);
    g_BattleSpecialQuasarAnimParams.spinModelFade = frame.animScale << 4;
    g_BattleSpecialQuasarDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialQuasarAnimParams.spinModelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialQuasarDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spinModelSlot->posX += frame.spinModelSlot->velocityX;
    frame.spinModelSlot->posY += frame.spinModelSlot->velocityY;
    frame.spinModelSlot->posZ += frame.spinModelSlot->velocityZ;
    frame.spinModelSlot->scaleZ += 0x80;
    if (frame.spinModelSlot->posY > -100) {
        frame.spawnSlot = (BattleFxPositionedSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialQuasarExpandingSpinModelTask);
        frame.spawnSlot->posX = frame.spinModelSlot->posX;
        frame.spawnSlot->posY = 0;
        frame.spawnSlot->posZ = frame.spinModelSlot->posZ;
        frame.spinModelSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    (frame.spinModelSlot->frameCounter)++;
    if (frame.spinModelSlot->frameCounter == 20) {
        frame.spinModelSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task to animate expanding spinning models for Kueisa.
 */
// FUNCTION: FF7 0x00547204
void __cdecl BattleFx_SpecialQuasarExpandingSpinModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[80];
        struct { unsigned char pad_to_4[0x4]; SVECTOR angleX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxQuasarSpinTask *spinTask; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.spinTask = (BattleFxQuasarSpinTask *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialQuasarAnimParams.expandModelGroup = Gfx_ModelTableGetRenderObject(18, g_BattleSpecialQuasarModelTable);
    frame.angleX.vx = 0;
    frame.angleX.vy = frame.spinTask->progress << 8;
    frame.angleX.vz = 0;
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    g_BattleSpecialQuasarExpandScaleZ = (frame.spinTask->progress << 8) + 0x1000;
    g_BattleSpecialQuasarExpandScaleY = g_BattleSpecialQuasarExpandScaleZ;
    g_BattleSpecialQuasarExpandScaleX = g_BattleSpecialQuasarExpandScaleY;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialQuasarExpandScaleX);
    frame.rotMatrix.t[0] = frame.spinTask->posX;
    frame.rotMatrix.t[1] = frame.spinTask->posY;
    frame.rotMatrix.t[2] = frame.spinTask->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecialQuasarAnimParams.expandModelFade = 0x80 - frame.spinTask->progress * 6;
    g_BattleSpecialQuasarDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialQuasarAnimParams.expandModelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialQuasarDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.spinTask->progress)++;
    if (frame.spinTask->progress == 0x14) {
        frame.spinTask->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task to animate growing fade sprites for the Kueisa special effect.
 */
// FUNCTION: FF7 0x00547358
void __cdecl BattleFx_SpecialQuasarGrowingFadeSpriteTask(void)
{
    BattleFxQuasarFadeSpriteTask *task;

    task = (BattleFxQuasarFadeSpriteTask *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialQuasarTable.spriteRecord = Gfx_ModelTableGetLoadedRecord(21, g_BattleSpecialQuasarModelTable);
    g_BattleSpecialQuasarTable.spriteObject = Gfx_ModelTableGetSpriteObject(21, g_BattleSpecialQuasarModelTable);
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->scale, 0);
    g_BattleSpecialQuasarColor2.blue = g_BattleSpecialQuasarGlowLevel << 3;
    g_BattleSpecialQuasarColor2.green = g_BattleSpecialQuasarColor2.blue;
    (g_BattleSpecialQuasarColor2.red) = g_BattleSpecialQuasarColor2.green;
    g_BattleSpecialQuasarDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialQuasarTable.spriteRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialQuasarDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->scale += 0x800 /* 0.5 */;
    g_BattleSpecialQuasarGlowLevel--;
    (task->step)++;
    if (task->step == 16) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
