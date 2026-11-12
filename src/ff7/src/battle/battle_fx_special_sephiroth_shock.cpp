// src/ff7/src/battle/battle_fx_special_sephiroth_shock.cpp
/* Enemy Special Effect [idx 25]: SephirothShock (JP: geki2 / 撃2 (Strike 2), asset: "SPECIAL/GEKI2") — Sephiroth Shock (0x3ac, 0x3b0, Safer*Sephiroth), Demon Rush (0x27c, Demon's Gate), Beast Sword (0x25c), Electromag (0x366) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Geki2 attack: it loads resources and spawns a timeline
//   task that drives an expanding burst, animated sprite steps, expanding, black and cycling
//   models, rising spinning models and fading or bouncing sprites.

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
#include "lib/api/sound.h"
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// The two sound-group descriptors referenced by g_BattleSpecSephirothShockSoundTable[0..1]
// at the start of this TU's .data span (0x008A3308..0x008A3328).
// GLOBAL: FF7 0x008a3308
int g_BattleSpecSephirothShockSoundGroup1[4] = {0x173, 0x7c, 0, 0};
// GLOBAL: FF7 0x008a3318
int g_BattleSpecSephirothShockSoundGroup2[4] = {0xfa, 0, 0, 0};
// GLOBAL: FF7 0x008a3328
unsigned int g_BattleSpecSephirothShockSoundTable[3] = {(unsigned int)&g_BattleSpecSephirothShockSoundGroup1, (unsigned int)&g_BattleSpecSephirothShockSoundGroup2, 0x00000000};
// GLOBAL: FF7 0x008a3338
unsigned int g_BattleSpecSephirothShockResourceDefs[100] ={
    0x003240b1,
    (unsigned int)&g_BattleSpecSephirothShockResNameSephirothShockRsd,
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a35b4
    (unsigned int)"DOMU1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a35c0
    (unsigned int)"DOMU2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a35cc
    (unsigned int)"DOMU3.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a35d8
    (unsigned int)"DOMU4.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a35e4
    (unsigned int)"OKURI6.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a35f0
    (unsigned int)"OKURI2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a35fc
    (unsigned int)"OKURI3.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a3608
    (unsigned int)"OKURI5.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a3614
    (unsigned int)"ISI1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a3620
    (unsigned int)"ISI2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a362c
    (unsigned int)"ISI3.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a3638
    (unsigned int)"ISI4.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a3644
    (unsigned int)"BAKU1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001940b1,
    // STRING: FF7 0x008a3650
    (unsigned int)"WA.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    0x00000000,
    // STRING: FF7 0x008a3658
    (unsigned int)"KEMU1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a3664
    (unsigned int)"GEKI00.TIM",
    // STRING: FF7 0x008a3670
    (unsigned int)"HAHEN1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a367c
    (unsigned int)"GEKI00.TIM",
    // STRING: FF7 0x008a3688
    (unsigned int)"HIBANA.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a3694
    (unsigned int)"GEKI00.TIM",
    // STRING: FF7 0x008a36a0
    (unsigned int)"HIKARI1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    0x00000000,
    // STRING: FF7 0x008a36ac
    (unsigned int)"JO_C01.SPT"
};
// GLOBAL: FF7 0x008a34c8
struct BattleSpecSephirothShockSpriteReq1 g_BattleSpecSephirothShockSpriteReq1 = {0, 0, -128, -128, -128, {0x2e, 0, 0}};
// GLOBAL: FF7 0x008a34d8
struct BattleSpecSephirothShockSpriteReq2 g_BattleSpecSephirothShockSpriteReq2 = {0, 0, {0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x008a34e8
struct BattleSpecSephirothShockSpriteReq3 g_BattleSpecSephirothShockSpriteReq3 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0};
// GLOBAL: FF7 0x008a34f8
struct BattleSpecSephirothShockSpriteReq3 g_BattleSpecSephirothShockSpriteReq4 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0};
// GLOBAL: FF7 0x008a3508
unsigned char g_BattleSpecSephirothShockTransformData1[8] = {0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00};
// GLOBAL: FF7 0x008a3510
int g_BattleSpecSephirothShockScale1 = 8192;
// GLOBAL: FF7 0x008a3518
struct BattleSpecSephirothShockSpriteReq5 g_BattleSpecSephirothShockSpriteReq5 = {0, {0x28, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x008a3528
unsigned char g_BattleSpecSephirothShockTransformData2[8] = {0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00};
// GLOBAL: FF7 0x008a3530
int g_BattleSpecSephirothShockScale2 = 8192;
// GLOBAL: FF7 0x008a3538
struct BattleSpecSephirothShockSpriteReq6 g_BattleSpecSephirothShockSpriteReq6 = {0, {0xa8, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x008a3548
unsigned char g_BattleSpecSephirothShockTransformData3[8] = {0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00};
// GLOBAL: FF7 0x008a3550
int g_BattleSpecSephirothShockScale3 = 8192;
// GLOBAL: FF7 0x008a3558
struct BattleSpecSephirothShockSpriteReq5 g_BattleSpecSephirothShockSpriteReq7 = {0, {0x28, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x008a3568
struct BattleSpecSephirothShockSpriteReq5 g_BattleSpecSephirothShockSpriteReq8 = {0, {0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x008a3578
struct BattleSpecSephirothShockSpriteReq9 g_BattleSpecSephirothShockSpriteReq9 = {0, 0, -128, -128, -128, {0x2c}, 0};
// GLOBAL: FF7 0x008a3588
struct BattleSpecSephirothShockSpriteReq10 g_BattleSpecSephirothShockSpriteReq10 = {0, 0, -128, -128, -128, 44, 0};
// GLOBAL: FF7 0x008a3598
struct BattleSpecSephirothShockSpriteReq11 g_BattleSpecSephirothShockSpriteReq11 = {0, {0x28, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x008a35a8
unsigned char g_BattleSpecSephirothShockResNameSephirothShockRsd[10] = "GEKI2.RSD";
// clang-format on


// clang-format off
// GLOBAL: FF7 0x00b65080
int g_BattleSpecSephirothShockPacketCursor = 0;
// GLOBAL: FF7 0x00b65084
short g_BattleSpecSephirothShockTargetMask = 0;
// GLOBAL: FF7 0x00b65088
int g_BattleSpecSephirothShockScaleVector1_X = 0;
// GLOBAL: FF7 0x00b6508c
int g_BattleSpecSephirothShockScaleVector1_Y = 0;
// GLOBAL: FF7 0x00b65090
int g_BattleSpecSephirothShockScaleVector1_Z = 0;
unsigned char g_bssPad_00b65094[4] = {0};
// GLOBAL: FF7 0x00b65098
int g_BattleSpecSephirothShockScaleVector2_X = 0;
// GLOBAL: FF7 0x00b6509c
int g_BattleSpecSephirothShockScaleVector2_Y = 0;
// GLOBAL: FF7 0x00b650a0
int g_BattleSpecSephirothShockScaleVector2_Z = 0;
unsigned char g_bssPad_00b650a4[4] = {0};
// GLOBAL: FF7 0x00b650a8
int g_BattleSpecSephirothShockScaleVector3_X = 0;
// GLOBAL: FF7 0x00b650ac
int g_BattleSpecSephirothShockScaleVector3_Y = 0;
// GLOBAL: FF7 0x00b650b0
int g_BattleSpecSephirothShockScaleVector3_Z = 0;
unsigned char g_bssPad_00b650b4[4] = {0};
// GLOBAL: FF7 0x00b650b8
struct GfxTableCursor *g_BattleSpecSephirothShockLoadedResTable = 0;
unsigned char g_bssPad_00b650bc[4] = {0};
// GLOBAL: FF7 0x00b650c0
int g_BattleSpecSephirothShockRotationVector = 0;
unsigned char g_bssPad_00b650c4[4] = {0};
// clang-format on

/*
 * When the engine context exists and g_BattleSpecSephirothShockLoadedResTable is empty, builds SPECIAL/GEKI2 with a default-
 * duration record, creates resource type 20 using g_BattleSpecSephirothShockResourceDefs, and registers g_BattleSpecSephirothShockSoundTable sound
 * groups.
 */
// FUNCTION: FF7 0x00539b10
void __cdecl BattleFx_SpecialLoadResSephirothShock(void)
{
    GfxModelTableBank *stage;
    GfxResourceRequest currentRecord;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecSephirothShockLoadedResTable == 0) {
        kind = 0x14;
        resourceDefsWord = (int)&g_BattleSpecSephirothShockResourceDefs;
        stage = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a36b8
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/GEKI2", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecSephirothShockLoadedResTable, stage);
        SfxRegisterSoundGroups((struct SoundGroup **)&g_BattleSpecSephirothShockSoundTable);
    }
}

/*
 * Mechanical entry wrapper: calls BattleFx_SpecialLoadResSephirothShock with no arguments, then BattleFx_SpecialSephirothShockSpawnTimeline with both
 * arguments unchanged.
 */
// FUNCTION: FF7 0x00539bb1
void __cdecl BattleFx_SpecialEnterSephirothShock(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResSephirothShock();
    BattleFx_SpecialSephirothShockSpawnTimeline(targetMask, actorIdx);
}

/*
 * Stores actorMask, allocates BattleFx_SpecialSephirothShockTimelineTask with the actorIndex actor bone
 * selected at g_BattleActorRecords[0].targetBone and actorMask actor-mask centroid, plays sound
 * group (0x40, 0), clears the global color triple, and starts a 0x1000 ramp with duration argument
 * 16.
 */
// FUNCTION: FF7 0x00539bcb
void __cdecl BattleFx_SpecialSephirothShockSpawnTimeline(int actorMask, int actorIndex)
{
    int taskSlot;

    g_BattleSpecSephirothShockTargetMask = actorMask;
    taskSlot = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSephirothShockTimelineTask);
    Battle_GetBoneWorldPos(actorIndex, g_BattleActorRecords[actorIndex].targetBone, (short *)(taskSlot + 4));
    Battle_GetActorMaskCentroid(actorMask, (short *)(taskSlot + 0xC));
    SfxPlaySoundGroup(0x40, 0);
    g_BattleFxTimelineState.channel2 = 0;
    g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
    g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    Battle_StartFixedPointRamp(0x1000, 16);
}

/*
 * Master timeline task that sequences expanding shells, rising models, and bursts for Geki2.
 */
// FUNCTION: FF7 0x00539c68
void __cdecl BattleFx_SpecialSephirothShockTimelineTask(void)
{
    short heading;
    BattleTaskSlot *pCur;
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxSpecialSephirothShockSlot *s;
    int t; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int y; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short tempRadius;
    short mainTargetIdx;
    short n5;
    int d; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int e; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int f; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int g; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int h; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int j; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    pCur = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (pCur->frameCounter < 5 && pCur->frameCounter % 2 == 0) {
        s = (BattleFxSpecialSephirothShockSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSephirothShockExpandingBlackModelTask);
        s->velocityX = 0;
        s->velocityY = 0xFCE0;
        s->velocityZ = 0;
    }
    if (pCur->frameCounter == 5) {
        s = (BattleFxSpecialSephirothShockSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSephirothShockDrawExpandingBurstTask);
        s->velocityX = 0;
        s->velocityY = 0;
        s->velocityZ = 0;
        s = (BattleFxSpecialSephirothShockSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSephirothShockExpandingCyclingModelTask);
        s->velocityX = 0;
        s->velocityY = 0;
        s->velocityZ = 0;
    }
    if (pCur->frameCounter >= 5 && pCur->frameCounter < 20 && pCur->frameCounter % 5 == 0) {
        s = (BattleFxSpecialSephirothShockSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSephirothShockExpandingModelTask);
        s->velocityX = 0;
        s->velocityY = 0;
        s->velocityZ = 0;
    }
    if (pCur->frameCounter >= 5 && pCur->frameCounter < 20) {
        tempRadius = pCur->frameCounter * 360;
        s = (BattleFxSpecialSephirothShockSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSephirothShockRisingSpinningModelTask);
        heading = rand() % 0x1000;
        s->velocityX = CosFixed(heading) * tempRadius >> 12;
        s->velocityZ = SinFixed(heading) * tempRadius >> 12;
        s->velocityY = 0;
        s->variant = rand() % 4;
        s->angle = rand() % 0x1000;
        s = (BattleFxSpecialSephirothShockSlot *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialSephirothShockRisingFadeSpriteTask)];
        for (mainTargetIdx = 0; mainTargetIdx < 2; mainTargetIdx++) {
            heading = rand() % 0x1000;
            s->velocityX = CosFixed(heading) * tempRadius >> 12;
            s->velocityZ = SinFixed(heading) * tempRadius >> 12;
            s->velocityY = 0;
            s->variant = rand() % 6;
        }
        for (mainTargetIdx = 0; mainTargetIdx < 4; mainTargetIdx++) {
            s = (BattleFxSpecialSephirothShockSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSephirothShockAnimatedSpriteEntry15Task);
            heading = rand() % 0x1000;
            s->velocityX = CosFixed(heading) * tempRadius >> 12;
            s->velocityZ = SinFixed(heading) * tempRadius >> 12;
            s->velocityY = 0;
        }
        g_BattleCameraFocalPoint.focalPointY += (short)(32 - (pCur->frameCounter % 2 << 6));
    }
    if (pCur->frameCounter == 25) {
        for (mainTargetIdx = 0; mainTargetIdx < 0xA; mainTargetIdx++) {
            if ((g_BattleSpecSephirothShockTargetMask & 1 << mainTargetIdx) != 0) {
                s = (BattleFxSpecialSephirothShockSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSephirothShockGrowingSpinningModelTask);
                Battle_GetBoneWorldPos(mainTargetIdx, g_BattleActorRecords[mainTargetIdx].targetBone, (short *)((int)s + 4));
                s->velocityY = 0;
                for (n5 = 0; n5 < 8; n5++) {
                    s = (BattleFxSpecialSephirothShockSlot *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialSephirothShockBouncingSpriteTask)];
                    Battle_GetBoneWorldPos(mainTargetIdx, g_BattleActorRecords[mainTargetIdx].targetBone, (short *)((int)s + 4));
                    BattleFx_MakeRandomConeVector(0xFFFFFF38, 100, (void *)((int)s + 0xC));
                    s->variant = rand() % 6;
                }
            }
        }
        Battle_PlaySfxPannedToActor(0xFA, -1, 0x40);
    }
    if (pCur->frameCounter > 25 && pCur->frameCounter < 33) {
        for (mainTargetIdx = 0; mainTargetIdx < 0xA; mainTargetIdx++) {
            if ((g_BattleSpecSephirothShockTargetMask & 1 << mainTargetIdx) != 0) {
                s = (BattleFxSpecialSephirothShockSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSephirothShockAnimatedSpriteEntry19Task);
                Battle_GetBoneWorldPos(mainTargetIdx, g_BattleActorRecords[mainTargetIdx].targetBone, (short *)((int)s + 4));
                s->velocityX += (short)(rand() % 1200 - 600);
                s->velocityY += (short)(-rand() % 500);
                s->velocityZ += (short)(rand() % 1200 - 600);
            }
        }
    }
    if (pCur->frameCounter > 33 && pCur->frameCounter < 43) {
        for (mainTargetIdx = 0; mainTargetIdx < 0xA; mainTargetIdx++) {
            if ((g_BattleSpecSephirothShockTargetMask & 1 << mainTargetIdx) != 0) {
                s = (BattleFxSpecialSephirothShockSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSephirothShockAnimatedSpriteEntry15Task);
                Battle_GetBoneWorldPos(mainTargetIdx, g_BattleActorRecords[mainTargetIdx].targetBone, (short *)((int)s + 4));
                s->velocityX += (short)(rand() % 1200 - 600);
                s->velocityY += (short)(rand() % 1200 - 600);
                s->velocityZ += (short)(rand() % 1200 - 600);
            }
        }
    }
    if (pCur->frameCounter == 35) {
        for (mainTargetIdx = 0; mainTargetIdx < 0xA; mainTargetIdx++) {
            if ((g_BattleSpecSephirothShockTargetMask & 1 << mainTargetIdx) != 0) {
                Battle_PostActorHitOrHurtMessage(mainTargetIdx);
            }
        }
        Battle_StartFixedPointRamp(0, 16);
    }
    (pCur->frameCounter)++;
    if (pCur->frameCounter == 55) {
        pCur->ownerSlot = BATTLE_TASK_FREE;
        Sfx_ReleaseCachedBuffer(0x173, 0xF);
        Sfx_ReleaseCachedBuffer(0x7C, 0xF);
        Sfx_ReleaseCachedBuffer(0xFA, 0xF);
    }
}

/*
 * Draws and expands the central burst sprite cluster for the Geki2 special effect.
 */
// FUNCTION: FF7 0x0053a418
void __cdecl BattleFx_SpecialSephirothShockDrawExpandingBurstTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        SVECTOR rotAngleX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; int burstRes[4]; };
        struct { unsigned char pad_to_38[0x38]; BattleFxOilTargetModelSlot *burstSlot; };
        struct { unsigned char pad_to_3C[0x3C]; PsxMatrix drawMatrix; };
    } frame;
#pragma pack(pop)

    frame.burstSlot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecSephirothShockSpriteReq1.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(18, g_BattleSpecSephirothShockLoadedResTable);
    g_BattleSpecSephirothShockSpriteReq1.handle = Gfx_ModelTableGetSpriteObject(18, g_BattleSpecSephirothShockLoadedResTable);
    frame.burstRes[0] = 1;
    frame.burstRes[1] = 2;
    frame.burstRes[2] = 3;
    frame.burstRes[3] = 4;
    frame.rotAngleX.vz = 0;
    frame.rotAngleX.vx = frame.rotAngleX.vz;
    frame.rotAngleX.vy = 0;
    RotMatrixXYZ(&frame.rotAngleX, &frame.rotMatrix);
    g_BattleSpecSephirothShockScaleVector3_Z = frame.burstSlot->frameCounter * 640;
    g_BattleSpecSephirothShockScaleVector3_Y = g_BattleSpecSephirothShockScaleVector3_Z;
    g_BattleSpecSephirothShockScaleVector3_X = g_BattleSpecSephirothShockScaleVector3_Y;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecSephirothShockScaleVector3_X);
    frame.rotMatrix.t[0] = frame.burstSlot->posX;
    frame.rotMatrix.t[1] = frame.burstSlot->posY;
    frame.rotMatrix.t[2] = frame.burstSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    (g_BattleSpecSephirothShockSpriteReq2.frameResource) = Gfx_ModelTableGetRenderObject(frame.burstRes[(frame.burstSlot->frameCounter) % 4], g_BattleSpecSephirothShockLoadedResTable);
    g_BattleSpecSephirothShockSpriteReq2.size = 40;
    g_BattleSpecSephirothShockSpriteReq2.alphaWord = 0x80 - (frame.burstSlot->frameCounter << 3);
    g_BattleSpecSephirothShockPacketCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecSephirothShockSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecSephirothShockPacketCursor);
    (g_BattleSpecSephirothShockSpriteReq2.frameResource) = Gfx_ModelTableGetRenderObject(frame.burstRes[(frame.burstSlot->frameCounter + 1) % 4], g_BattleSpecSephirothShockLoadedResTable);
    g_BattleSpecSephirothShockSpriteReq2.alphaWord = 0x40 - (frame.burstSlot->frameCounter << 2);
    g_BattleSpecSephirothShockPacketCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecSephirothShockSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecSephirothShockPacketCursor);
    BattleFx_InstallBillboardMatrix((void *)((int)frame.burstSlot + 4), 0x2000 /* 2.0 */, 0);
    g_BattleSpecSephirothShockSpriteReq1.colorB = 0x40 - (frame.burstSlot->frameCounter << 2);
    g_BattleSpecSephirothShockSpriteReq1.colorG = g_BattleSpecSephirothShockSpriteReq1.colorB;
    g_BattleSpecSephirothShockSpriteReq1.colorR = g_BattleSpecSephirothShockSpriteReq1.colorG;
    g_BattleSpecSephirothShockPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecSephirothShockSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecSephirothShockPacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.burstSlot->frameCounter)++;
    if (frame.burstSlot->frameCounter == 15) {
        frame.burstSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Per-frame step for the slot at (g_BattleActiveTaskSlotIdx << 5) +
 * &g_BattleHitLivewireEmitterRecords: sprite pointer from index 0xF of table
 * g_BattleSpecSephirothShockLoadedResTable into g_BattleSpecSephirothShockSpriteReq3, second field from
 * Battle3d_GetSceneRenderObject(1, g_pBattle3dScene), billboard matrix from the slot position at scale 0x2000, frame
 * counter copied into g_BattleSpecSephirothShockSpriteReq3.taskWord, submitted with the cursor in
 * g_BattleSpecSephirothShockPacketCursor. When unpaused with a nonnull sprite it advances the frame counter and
 * frees the slot at the animation frame count.
 */
// FUNCTION: FF7 0x0053a69c
void __cdecl BattleFx_SpecialSephirothShockAnimatedSpriteEntry15Task(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecSephirothShockSpriteReq3.spriteEntry) = Gfx_ModelTableGetLoadedRecord(15, g_BattleSpecSephirothShockLoadedResTable);
    g_BattleSpecSephirothShockSpriteReq3.sceneObject = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), 0x2000 /* 2.0 */, 0);
    g_BattleSpecSephirothShockSpriteReq3.taskWord = task->frameCounter;
    g_BattleSpecSephirothShockPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecSephirothShockSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecSephirothShockPacketCursor);
    if (g_BattleIsPaused == 0 && (g_BattleSpecSephirothShockSpriteReq3.spriteEntry) != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= *(int *)((char *)*(int *)(g_BattleSpecSephirothShockSpriteReq3.spriteEntry) + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * The same shape as BattleFx_SpecialSephirothShockAnimatedSpriteEntry15Task one entry along: index 0x13 of
 * table g_BattleSpecSephirothShockLoadedResTable into the request at g_BattleSpecSephirothShockSpriteReq4, second
 * field from Battle3d_GetSceneRenderObject(3, g_pBattle3dScene), scale 0x2000, frame field
 * g_BattleSpecSephirothShockSpriteReq4.taskWord, cursor g_BattleSpecSephirothShockPacketCursor, and the slot is freed
 * at the animation frame count. Drawing precedes the pause check; the counter
 * advances only when unpaused with a nonnull sprite.
 */
// FUNCTION: FF7 0x0053a76d
void __cdecl BattleFx_SpecialSephirothShockAnimatedSpriteEntry19Task(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecSephirothShockSpriteReq4.spriteEntry) = Gfx_ModelTableGetLoadedRecord(19, g_BattleSpecSephirothShockLoadedResTable);
    g_BattleSpecSephirothShockSpriteReq4.sceneObject = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), 0x2000 /* 2.0 */, 0);
    g_BattleSpecSephirothShockSpriteReq4.taskWord = task->frameCounter;
    g_BattleSpecSephirothShockPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecSephirothShockSpriteReq4, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecSephirothShockPacketCursor);
    if (g_BattleIsPaused == 0 && (g_BattleSpecSephirothShockSpriteReq4.spriteEntry) != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= *(int *)((char *)*(int *)(g_BattleSpecSephirothShockSpriteReq4.spriteEntry) + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Scales and submits the primary expanding shockwave model for Geki2.
 */
// FUNCTION: FF7 0x0053a83e
void __cdecl BattleFx_SpecialSephirothShockExpandingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotAngleX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_30[0x30]; BattleFxHellBomberGrowSlot *growSlot; };
        struct { unsigned char pad_to_34[0x34]; PsxMatrix drawMatrix; };
    } frame;
#pragma pack(pop)

    frame.growSlot = (BattleFxHellBomberGrowSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecSephirothShockSpriteReq5.modelGroup) = Gfx_ModelTableGetRenderObject(0, g_BattleSpecSephirothShockLoadedResTable);
    frame.rotAngleX.vz = 0;
    frame.rotAngleX.vy = frame.rotAngleX.vz;
    frame.rotAngleX.vx = frame.rotAngleX.vy;
    RotMatrix(&frame.rotAngleX, &frame.rotMatrix);
    g_BattleSpecSephirothShockScale1 = frame.growSlot->growStep * 0x600;
    (*(int *)&g_BattleSpecSephirothShockTransformData1) = g_BattleSpecSephirothShockScale1;
    ScaleMatrix(&frame.rotMatrix, (int *)&g_BattleSpecSephirothShockTransformData1);
    frame.rotMatrix.t[0] = frame.growSlot->posX;
    frame.rotMatrix.t[1] = frame.growSlot->posY;
    frame.rotMatrix.t[2] = frame.growSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    g_BattleSpecSephirothShockSpriteReq5.alphaWord = 0x80 - (frame.growSlot->growStep << 3);
    g_BattleSpecSephirothShockPacketCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecSephirothShockSpriteReq5, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecSephirothShockPacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.growSlot->growStep)++;
    if (frame.growSlot->growStep == 15) {
        frame.growSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Scales and submits the dark inner expanding shell model for Geki2.
 */
// FUNCTION: FF7 0x0053a981
void __cdecl BattleFx_SpecialSephirothShockExpandingBlackModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotAngleX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_30[0x30]; BattleFxHellBomberGrowSlot *growSlot; };
        struct { unsigned char pad_to_34[0x34]; PsxMatrix drawMatrix; };
    } frame;
#pragma pack(pop)

    frame.growSlot = (BattleFxHellBomberGrowSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecSephirothShockSpriteReq6.modelGroup) = Gfx_ModelTableGetRenderObject(14, g_BattleSpecSephirothShockLoadedResTable);
    frame.rotAngleX.vz = 0;
    frame.rotAngleX.vy = frame.rotAngleX.vz;
    frame.rotAngleX.vx = frame.rotAngleX.vy;
    RotMatrix(&frame.rotAngleX, &frame.rotMatrix);
    g_BattleSpecSephirothShockScale2 = frame.growSlot->growStep * 0xC80;
    (*(int *)&g_BattleSpecSephirothShockTransformData2) = g_BattleSpecSephirothShockScale2;
    ScaleMatrix(&frame.rotMatrix, (int *)&g_BattleSpecSephirothShockTransformData2);
    frame.rotMatrix.t[0] = frame.growSlot->posX;
    frame.rotMatrix.t[1] = frame.growSlot->posY;
    frame.rotMatrix.t[2] = frame.growSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecSephirothShockSpriteReq6.scaleWord = frame.growSlot->growStep * 409;
    g_BattleSpecSephirothShockPacketCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecSephirothShockSpriteReq6, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecSephirothShockPacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.growSlot->growStep)++;
    if (frame.growSlot->growStep == 10) {
        frame.growSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Alternates between paired resource models while expanding the cycling Geki2 shockwave.
 */
// FUNCTION: FF7 0x0053aacd
void __cdecl BattleFx_SpecialSephirothShockExpandingCyclingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[104];
        struct { unsigned char pad_to_4[0x4]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; int cycleRes0[4]; };
        struct { unsigned char pad_to_3C[0x3C]; BattleFxOilTargetModelSlot *cycleSlot; };
        struct { unsigned char pad_to_40[0x40]; PsxMatrix drawMatrix; };
    } frame;
#pragma pack(pop)

    frame.cycleSlot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.cycleRes0[0] = 5;
    frame.cycleRes0[1] = 6;
    frame.cycleRes0[2] = 7;
    frame.cycleRes0[3] = 8;
    RotMatrix((SVECTOR *)&g_BattleSpecSephirothShockRotationVector, &frame.rotMatrix);
    g_BattleSpecSephirothShockScale3 = frame.cycleSlot->frameCounter << 9;
    (*(int *)&g_BattleSpecSephirothShockTransformData3) = g_BattleSpecSephirothShockScale3;
    ScaleMatrix(&frame.rotMatrix, (int *)&g_BattleSpecSephirothShockTransformData3);
    frame.rotMatrix.t[0] = frame.cycleSlot->posX;
    frame.rotMatrix.t[1] = frame.cycleSlot->posY;
    frame.rotMatrix.t[2] = frame.cycleSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    g_BattleSpecSephirothShockSpriteReq7.alphaWord = 0x80 - frame.cycleSlot->frameCounter * 6;
    (g_BattleSpecSephirothShockSpriteReq7.modelGroup) = Gfx_ModelTableGetRenderObject(frame.cycleRes0[((frame.cycleSlot->frameCounter) % 4)], g_BattleSpecSephirothShockLoadedResTable);
    g_BattleSpecSephirothShockPacketCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecSephirothShockSpriteReq7, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecSephirothShockPacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.cycleSlot->frameCounter)++;
    if (frame.cycleSlot->frameCounter == 15) {
        frame.cycleSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates vertical rise and spin angles and submits the Geki2 vortex model.
 */
// FUNCTION: FF7 0x0053ac28
void __cdecl BattleFx_SpecialSephirothShockRisingSpinningModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        SVECTOR rotAngleX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; int risingRes0[4]; };
        struct { unsigned char pad_to_38[0x38]; BattleFxSephirothShockRisingSpinningModelSlot *risingSlot; };
        struct { unsigned char pad_to_3C[0x3C]; PsxMatrix drawMatrix; };
    } frame;
#pragma pack(pop)

    frame.risingSlot = (BattleFxSephirothShockRisingSpinningModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.risingRes0[0] = 9;
    frame.risingRes0[1] = 0xA;
    frame.risingRes0[2] = 0xB;
    frame.risingRes0[3] = 0xC;
    frame.rotAngleX.vy = 0;
    frame.rotAngleX.vx = frame.rotAngleX.vy;
    frame.rotAngleX.vz = frame.risingSlot->spinAngle;
    RotMatrixXYZ(&frame.rotAngleX, &frame.rotMatrix);
    g_BattleSpecSephirothShockScaleVector2_Z = 4096;
    g_BattleSpecSephirothShockScaleVector2_Y = g_BattleSpecSephirothShockScaleVector2_Z;
    g_BattleSpecSephirothShockScaleVector2_X = g_BattleSpecSephirothShockScaleVector2_Y;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecSephirothShockScaleVector2_X);
    frame.rotMatrix.t[0] = frame.risingSlot->posX;
    frame.rotMatrix.t[1] = frame.risingSlot->posY;
    frame.rotMatrix.t[2] = frame.risingSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    g_BattleSpecSephirothShockSpriteReq8.alphaWord = 0x80 - (frame.risingSlot->frameCounter << 3);
    (g_BattleSpecSephirothShockSpriteReq8.modelGroup) = Gfx_ModelTableGetRenderObject(frame.risingRes0[frame.risingSlot->tableIndex], g_BattleSpecSephirothShockLoadedResTable);
    g_BattleSpecSephirothShockPacketCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecSephirothShockSpriteReq8, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecSephirothShockPacketCursor);
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
 * The rising, fading step of BattleFx_SpecialColdAirRisingFadeSpriteTask applied to the second slot
 * array, (g_BattleSecondaryTaskActiveSlotIdx << 5) + &g_BattleTaskSlotBuffer.secondarySlots. Reads
 * index 0x10 of table g_BattleSpecSephirothShockLoadedResTable into the request at
 * g_BattleSpecSephirothShockSpriteReq9, takes the request's frame field from the slot, sets the colour bytes
 * g_BattleSpecSephirothShockSpriteReq9.colorR through g_BattleSpecSephirothShockSpriteReq9.colorB to 0x80 -
 * (frame << 3), and submits with the cursor in g_BattleOverlayDrawListHead. When unpaused it
 * subtracts 0x12C from the slot's y position and frees the slot after a fixed 15
 * frames.
 */
// FUNCTION: FF7 0x0053adb8
void __cdecl BattleFx_SpecialSephirothShockRisingFadeSpriteTask(void)
{
    BattleFxSephirothShockRisingFadeSprite *task;

    task = (BattleFxSephirothShockRisingFadeSprite *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    (g_BattleSpecSephirothShockSpriteReq9.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(16, g_BattleSpecSephirothShockLoadedResTable);
    g_BattleSpecSephirothShockSpriteReq9.handle = Gfx_ModelTableGetSpriteObject(16, g_BattleSpecSephirothShockLoadedResTable);
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), 0x2000 /* 2.0 */, 0);
    g_BattleSpecSephirothShockSpriteReq9.taskWord = task->taskWord;
    g_BattleSpecSephirothShockSpriteReq9.colorB = 0x80 - (task->fadeStep << 3);
    g_BattleSpecSephirothShockSpriteReq9.colorG = g_BattleSpecSephirothShockSpriteReq9.colorB;
    g_BattleSpecSephirothShockSpriteReq9.colorR = g_BattleSpecSephirothShockSpriteReq9.colorG;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecSephirothShockSpriteReq9, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posY -= 300;
    (task->fadeStep)++;
    if (task->fadeStep == 15) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Steps ballistic bounce motion and draws a scattered Geki2 debris sprite.
 */
// FUNCTION: FF7 0x0053aeb9
void __cdecl BattleFx_SpecialSephirothShockBouncingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int fadeLevel;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotAngleX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; short fadeCopy; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix transformMatrix; };
        struct { unsigned char pad_to_50[0x50]; short axisIdx; };
        struct { unsigned char pad_to_54[0x54]; int rotDepth; };
        struct { unsigned char pad_to_58[0x58]; BattleFxPoisonGasBouncingSpriteSlot *bounceSlot; };
    } frame;
#pragma pack(pop)

    frame.bounceSlot = (BattleFxPoisonGasBouncingSpriteSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    (g_BattleSpecSephirothShockSpriteReq10.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(16, g_BattleSpecSephirothShockLoadedResTable);
    g_BattleSpecSephirothShockSpriteReq10.handle = Gfx_ModelTableGetSpriteObject(16, g_BattleSpecSephirothShockLoadedResTable);
    for (frame.axisIdx = 0; frame.axisIdx < 3; frame.axisIdx++) {
        frame.transformMatrix.m[2][frame.axisIdx] = 0;
        frame.transformMatrix.m[1][frame.axisIdx] = 0;
        frame.transformMatrix.m[0][frame.axisIdx] = 0;
    }
    if (frame.bounceSlot->frameCounter < 7) {
        frame.fadeLevel = 8;
    } else {
        frame.fadeLevel = 15 - frame.bounceSlot->frameCounter;
    }
    frame.fadeCopy = *(short *)&frame.fadeLevel;
    frame.rotAngleX.vy = 0;
    frame.rotAngleX.vx = frame.rotAngleX.vy;
    frame.rotAngleX.vz = frame.bounceSlot->phase;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint(&frame.bounceSlot->posX, frame.transformMatrix.t, &frame.rotDepth);
    frame.transformMatrix.m[2][2] = frame.bounceSlot->variant < 4 ? 0x1400 : 0x2000;
    frame.transformMatrix.m[1][1] = frame.transformMatrix.m[2][2];
    frame.transformMatrix.m[0][0] = frame.transformMatrix.m[1][1];
    RotMatrix(&frame.rotAngleX, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.transformMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.transformMatrix);
    SetTransMatrix(&frame.transformMatrix);
    g_BattleSpecSephirothShockSpriteReq10.frame = frame.bounceSlot->variant;
    g_BattleSpecSephirothShockSpriteReq10.code = (frame.fadeCopy - 8 != 0 ? 2 : 0) + 0x2C;
    g_BattleSpecSephirothShockSpriteReq10.colorB = frame.fadeCopy << 4;
    g_BattleSpecSephirothShockSpriteReq10.colorG = g_BattleSpecSephirothShockSpriteReq10.colorB;
    g_BattleSpecSephirothShockSpriteReq10.colorR = g_BattleSpecSephirothShockSpriteReq10.colorG;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecSephirothShockSpriteReq10, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.bounceSlot->posX += frame.bounceSlot->velocityX;
    frame.bounceSlot->posY += frame.bounceSlot->velocityY;
    frame.bounceSlot->posZ += frame.bounceSlot->velocityZ;
    if (frame.bounceSlot->posY > 0) {
        frame.bounceSlot->posY = 0;
        frame.bounceSlot->velocityY *= -1;
    }
    frame.bounceSlot->velocityY += 20;
    frame.bounceSlot->phase += 0x200;
    (frame.bounceSlot->frameCounter)++;
    if (frame.bounceSlot->frameCounter == 15) {
        frame.bounceSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Grows, spins, and submits the ground-ring model for the Geki2 special effect.
 */
// FUNCTION: FF7 0x0053b14d
void __cdecl BattleFx_SpecialSephirothShockGrowingSpinningModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[80];
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxOilTargetModelSlot *growingSlot; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.growingSlot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecSephirothShockSpriteReq11.modelGroup) = Gfx_ModelTableGetRenderObject(13, g_BattleSpecSephirothShockLoadedResTable);
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.growingSlot->frameCounter << 6;
    frame.rotX.vz = 0;
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    g_BattleSpecSephirothShockScaleVector1_Z = (frame.growingSlot->frameCounter << 8) + 12288;
    g_BattleSpecSephirothShockScaleVector1_X = g_BattleSpecSephirothShockScaleVector1_Z;
    g_BattleSpecSephirothShockScaleVector1_Y = (frame.growingSlot->frameCounter << 8) + 8192;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecSephirothShockScaleVector1_X);
    frame.rotMatrix.t[0] = frame.growingSlot->posX;
    frame.rotMatrix.t[1] = frame.growingSlot->posY;
    frame.rotMatrix.t[2] = frame.growingSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecSephirothShockSpriteReq11.alpha = 0x80 - (frame.growingSlot->frameCounter << 2);
    g_BattleSpecSephirothShockPacketCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecSephirothShockSpriteReq11, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecSephirothShockPacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.growingSlot->frameCounter)++;
    if (frame.growingSlot->frameCounter == 15) {
        frame.growingSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
