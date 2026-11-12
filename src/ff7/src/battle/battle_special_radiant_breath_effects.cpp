// src/ff7/src/battle/battle_special_radiant_breath_effects.cpp
/* Enemy Special Attack [106]: Radiant Breath (orig. special/s_bless, ブレス) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Bless spell. It loads its resources, starts the timeline,
//   and runs the intensity ramp, ground burst, sprite, growing and expanding model, and rising
//   effect tasks.

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
// GLOBAL: FF7 0x00899b60
unsigned int g_BattleSpecRadiantBreathResourceDef[125] = {0x001900b1, (unsigned int)&g_BattleSpecRadiantBreathResName_wa, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecRadiantBreathResName_kage_netu1, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)(g_BattleSpecRadiantBreathResName_kage_netu1 + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecRadiantBreathResName_uchi1_uchi2, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)(g_BattleSpecRadiantBreathResName_uchi1_uchi2 + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecRadiantBreathResName_uchi3_uchi4, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)(g_BattleSpecRadiantBreathResName_uchi3_uchi4 + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecRadiantBreathResName_uchi5_uchi6, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)(g_BattleSpecRadiantBreathResName_uchi5_uchi6 + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecRadiantBreathResName_uchi7_uchi8, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)(g_BattleSpecRadiantBreathResName_uchi7_uchi8 + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)&g_BattleSpecRadiantBreathResName_iwa1_iwa2, 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)(g_BattleSpecRadiantBreathResName_iwa1_iwa2 + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)&g_BattleSpecRadiantBreathResName_iwa3_iwa4, 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)(g_BattleSpecRadiantBreathResName_iwa3_iwa4 + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecRadiantBreathResName_domu_hikari, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)(g_BattleSpecRadiantBreathResName_domu_hikari + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecRadiantBreathResName_breath02_1, (unsigned int)&g_BattleSpecRadiantBreathResName_hahen1, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)(g_BattleSpecRadiantBreathResName_hahen1 + 0xc), (unsigned int)(g_BattleSpecRadiantBreathResName_hahen1 + 0x1c), 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecRadiantBreathResName_breath02_2, (unsigned int)&g_BattleSpecRadiantBreathResName_hikari2, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)(g_BattleSpecRadiantBreathResName_hikari2 + 0xc), (unsigned int)(g_BattleSpecRadiantBreathResName_hikari2 + 0x1c), 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecRadiantBreathResName_breath02_3, (unsigned int)&g_BattleSpecRadiantBreathResName_elec2_1, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)(g_BattleSpecRadiantBreathResName_elec2_1 + 0xc), (unsigned int)(g_BattleSpecRadiantBreathResName_elec2_1 + 0x1c), 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecRadiantBreathResName_breath02_4, (unsigned int)&g_BattleSpecRadiantBreathResName_elec2_2, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)(g_BattleSpecRadiantBreathResName_elec2_2 + 0xc), (unsigned int)(g_BattleSpecRadiantBreathResName_elec2_2 + 0x1c), 0x00000000, 0x00000000};
// GLOBAL: FF7 0x00899d58
struct BattleSpecRadiantBreathSpriteRequest1 g_BattleSpecRadiantBreathSpriteRequest1 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
// The animated sprites' resource entries, picked by each slot's animIndex.
// GLOBAL: FF7 0x00899d68
int g_BattleSpecRadiantBreathAnimData1[2] = {0x17, 0x18};
// GLOBAL: FF7 0x00899d70
BattleSpecRadiantBreathTransformData1 g_BattleSpecRadiantBreathTransformData1 = {{0, 0x28, 0, 0, 0x20}, {0, 0, -128, -128, -128, 0x2c}, {0, 0, -128, -128, -128, 0x2c}};
// The frame-table sprites' resource entries, picked by each slot's animIndex.
// GLOBAL: FF7 0x00899da0
int g_BattleSpecRadiantBreathAnimData2[2] = {0x15, 0x16};
// The frame-table sprite's frames, indexed by frameIndex; the table runs on into the bytes after.
// GLOBAL: FF7 0x00899da8
short g_BattleSpecRadiantBreathParam1[2] = {0, 1};
unsigned char g_rawData_00899dac[4] = {0x02, 0x00, 0x03, 0x00};
unsigned char g_rawData_00899db0[48] = {0x04, 0x00, 0x05, 0x00, 0x06, 0x00, 0x07, 0x00, 0x08, 0x00, 0x09, 0x00, 0x0a, 0x00, 0x0b, 0x00, 0x0c, 0x00, 0x0d, 0x00, 0x02, 0x00, 0x03, 0x00, 0x04, 0x00, 0x05, 0x00, 0x06, 0x00, 0x07, 0x00, 0x08, 0x00, 0x09, 0x00, 0x0a, 0x00, 0x0b, 0x00, 0x0c, 0x00, 0x0d, 0x00, 0x0e, 0x00, 0x0f, 0x00};
// GLOBAL: FF7 0x00899de0
struct BattleSpecRadiantBreathTransformData2 g_BattleSpecRadiantBreathTransformData2 = {0, 0, {0}, 0, {0x20, 0, 0, 0}};
// The growing model's resource entries, cycled by frameCounter % 8, on into the entries of
// g_BattleSpecRadiantBreathAnimData4.
// GLOBAL: FF7 0x00899df0
int g_BattleSpecRadiantBreathAnimData3[2] = {3, 4};
// GLOBAL: FF7 0x00899df8
struct BattleSpecRadiantBreathAnimData4 g_BattleSpecRadiantBreathAnimData4 = {{5, 0, 0, 0, 6, 0, 0, 0, 7, 0, 0, 0, 8, 0, 0, 0, 9, 0, 0, 0, 0xa, 0, 0, 0}, 0, 0};
// GLOBAL: FF7 0x00899e18
struct BattleSpecRadiantBreathSpriteRequest2 g_BattleSpecRadiantBreathSpriteRequest2 = {-128, -128, -128, {0x2e, 0, 0, 0, 0}};
// Spark spread steps, one per frame of the growing model.
// GLOBAL: FF7 0x00899e20
short g_BattleSpecRadiantBreathAnimData5[12] = {2, 8, 5, 0, 3, 0xa, 1, 4, 6, 0xb, 7, 9};
// GLOBAL: FF7 0x00899e38
short g_BattleSpecRadiantBreathSparkFade = 16;
// GLOBAL: FF7 0x00899e40
BattleSpecRadiantBreathSpriteRequest3 g_BattleSpecRadiantBreathSpriteRequest3 = {{0, 0, -128, -128, -128, 0x2e}};
unsigned char g_rawData_00899e5c[4] = {0x20, 0x00, 0x00, 0x00};
// The rising spin models' resource entries, picked by each slot's animIndex.
// GLOBAL: FF7 0x00899e60
int g_BattleSpecRadiantBreathAnimData6[2] = {0xb, 0xc};
// GLOBAL: FF7 0x00899e68
struct BattleSpecRadiantBreathAnimData7 g_BattleSpecRadiantBreathAnimData7 = {{0xd, 0, 0, 0, 0xe, 0, 0, 0}, 0, 0};
// GLOBAL: FF7 0x00899e78
BattleSpecRadiantBreathSpriteRequest4 g_BattleSpecRadiantBreathSpriteRequest4 = {-128, -128, -128, {0x2c}, 0, {0}, 0, {0xa8}, 0x80};
unsigned char g_rawData_00899e8c[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00899e90
unsigned char g_BattleSpecRadiantBreathResName_wa[7] = "WA.RSD";
// GLOBAL: FF7 0x00899e98
unsigned char g_BattleSpecRadiantBreathResName_kage_netu1[22] =
    "KAGE.RSD\0\0\0\0"
    "NETU1.RSD";
// GLOBAL: FF7 0x00899eb0
unsigned char g_BattleSpecRadiantBreathResName_uchi1_uchi2[22] =
    "UCHI1.RSD\0\0\0"
    "UCHI2.RSD";
// GLOBAL: FF7 0x00899ec8
unsigned char g_BattleSpecRadiantBreathResName_uchi3_uchi4[22] =
    "UCHI3.RSD\0\0\0"
    "UCHI4.RSD";
// GLOBAL: FF7 0x00899ee0
unsigned char g_BattleSpecRadiantBreathResName_uchi5_uchi6[22] =
    "UCHI5.RSD\0\0\0"
    "UCHI6.RSD";
// GLOBAL: FF7 0x00899ef8
unsigned char g_BattleSpecRadiantBreathResName_uchi7_uchi8[22] =
    "UCHI7.RSD\0\0\0"
    "UCHI8.RSD";
// GLOBAL: FF7 0x00899f10
unsigned char g_BattleSpecRadiantBreathResName_iwa1_iwa2[21] =
    "IWA1.RSD\0\0\0\0"
    "IWA2.RSD";
// GLOBAL: FF7 0x00899f28
unsigned char g_BattleSpecRadiantBreathResName_iwa3_iwa4[21] =
    "IWA3.RSD\0\0\0\0"
    "IWA4.RSD";
// GLOBAL: FF7 0x00899f40
unsigned char g_BattleSpecRadiantBreathResName_domu_hikari[23] =
    "DOMU.RSD\0\0\0\0"
    "DOSOTO.RSD";
// GLOBAL: FF7 0x00899f58
unsigned char g_BattleSpecRadiantBreathResName_breath02_1[13] = "BREATH03.TIM";
// GLOBAL: FF7 0x00899f68
unsigned char g_BattleSpecRadiantBreathResName_hahen1[38] =
    "HAHEN1.SPT\0\0"
    "BREATH03.TIM\0\0\0\0"
    "KEMU1.SPT";
// GLOBAL: FF7 0x00899f90
unsigned char g_BattleSpecRadiantBreathResName_breath02_2[13] = "BREATH03.TIM";
// GLOBAL: FF7 0x00899fa0
unsigned char g_BattleSpecRadiantBreathResName_hikari2[40] =
    "HIKARI1.SPT\0"
    "BREATH02.TIM\0\0\0\0"
    "HIKARI2.SPT";
// GLOBAL: FF7 0x00899fc8
unsigned char g_BattleSpecRadiantBreathResName_breath02_3[13] = "BREATH02.TIM";
// GLOBAL: FF7 0x00899fd8
unsigned char g_BattleSpecRadiantBreathResName_elec2_1[38] =
    "ELEC1.SPT\0\0\0"
    "BREATH02.TIM\0\0\0\0"
    "ELEC2.SPT";
// GLOBAL: FF7 0x0089a000
unsigned char g_BattleSpecRadiantBreathResName_breath02_4[13] = "BREATH02.TIM";
// GLOBAL: FF7 0x0089a010
unsigned char g_BattleSpecRadiantBreathResName_elec2_2[38] =
    "ELEC3.SPT\0\0\0"
    "BREATH02.TIM\0\0\0\0"
    "ELEC4.SPT";
// GLOBAL: FF7 0x00b4fc68
int g_BattleSpecRadiantBreathGroundFade = 0;
// GLOBAL: FF7 0x00b4fc6c
int g_BattleSpecRadiantBreathDrawHandle = 0;
// GLOBAL: FF7 0x00b4fc70
short g_BattleSpecRadiantBreathTargetMask = 0;
unsigned char g_bssPad_00b4fc74[4] = {0};
// GLOBAL: FF7 0x00b4fc78
int g_BattleSpecRadiantBreathSpinScaleX = 0;
// GLOBAL: FF7 0x00b4fc7c
int g_BattleSpecRadiantBreathSpinScaleY = 0;
// GLOBAL: FF7 0x00b4fc80
int g_BattleSpecRadiantBreathSpinScaleZ = 0;
unsigned char g_bssPad_00b4fc84[4] = {0};
// GLOBAL: FF7 0x00b4fc88
int g_BattleSpecRadiantBreathShellScaleX = 0;
// GLOBAL: FF7 0x00b4fc8c
int g_BattleSpecRadiantBreathShellScaleY = 0;
// GLOBAL: FF7 0x00b4fc90
int g_BattleSpecRadiantBreathShellScaleZ = 0;
unsigned char g_bssPad_00b4fc94[4] = {0};
// GLOBAL: FF7 0x00b4fc98
int g_BattleSpecRadiantBreathSparkScaleX = 0;
// GLOBAL: FF7 0x00b4fc9c
int g_BattleSpecRadiantBreathSparkScaleY = 0;
// GLOBAL: FF7 0x00b4fca0
int g_BattleSpecRadiantBreathSparkScaleZ = 0;
unsigned char g_bssPad_00b4fca4[4] = {0};
// GLOBAL: FF7 0x00b4fca8
int g_BattleSpecRadiantBreathBurstScaleX = 0;
// GLOBAL: FF7 0x00b4fcac
int g_BattleSpecRadiantBreathBurstScaleY = 0;
// GLOBAL: FF7 0x00b4fcb0
int g_BattleSpecRadiantBreathBurstScaleZ = 0;
unsigned char g_bssPad_00b4fcb4[4] = {0};
// GLOBAL: FF7 0x00b4fcb8
struct GfxTableCursor *g_BattleSpecRadiantBreathLoadedResTable = 0;
// GLOBAL: FF7 0x00b4fcbc
short g_BattleSpecRadiantBreathIntensity = 0;
// GLOBAL: FF7 0x00b4fcc0
struct BattleSpecRadiantBreathBuffer g_BattleSpecRadiantBreathShellAngles = {0};
// GLOBAL: FF7 0x00b4fcc4
short g_BattleSpecRadiantBreathShellAngleZ = 0;
// clang-format on


/*
 * When the engine context exists and g_BattleSpecRadiantBreathLoadedResTable is empty, builds special/s_bless with a default-
 * duration record and creates resource type 25 using g_BattleSpecRadiantBreathResourceDef.
 */
// FUNCTION: FF7 0x00511230
void __cdecl BattleFx_SpecialLoadResRadiantBreath(void)
{
    GfxModelTableBank *stage;
    GfxResourceRequest requestSlot;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resultDefArr;

    if (g_pBattle3dScene != 0 && g_BattleSpecRadiantBreathLoadedResTable == 0) {
        kind = 0x19;
        resultDefArr = (int)&g_BattleSpecRadiantBreathResourceDef;
        stage = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0089a038
        pRequest->resourcePath = (char *)BattleFx_BuildResourcePath("special/s_bless", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resultDefArr, pRequest, &g_BattleSpecRadiantBreathLoadedResTable, stage);
    }
}

/* Forwards targetIdx and actorIdx to BattleFx_SpecialBeginRadiantBreath. */
// FUNCTION: FF7 0x005112c0
void __cdecl BattleFx_SpecialEnterRadiantBreath(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginRadiantBreath(targetMask, actorIdx);
}

/*
 * Loads special/s_bless, stores the target mask, allocates BattleFx_SpecialRadiantBreathTimelineTask with
 * the actorIdx actor's bone34 position and the target mask's centroid, clears the global colour
 * triple, starts a 0x1000 ramp with duration argument 16, and plays sound 0x173.
 */
// FUNCTION: FF7 0x005112d5
void __cdecl BattleFx_SpecialBeginRadiantBreath(int targetMask, int actorIdx)
{
    int taskRec;

    BattleFx_SpecialLoadResRadiantBreath();
    g_BattleSpecRadiantBreathTargetMask = targetMask;
    taskRec = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialRadiantBreathTimelineTask);
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].bone34, (short *)(taskRec + 4));
    Battle_GetActorMaskCentroid(targetMask, (short *)(taskRec + 0xC));
    g_BattleFxTimelineState.channel2 = 0;
    g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
    g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    Battle_StartFixedPointRamp(0x1000, 0x10);
    Battle_PlaySfxPannedToActor(0x173, -1, 0x40);
}

/*
 * Master timeline task that spawns ground bursts, expanding shells, and sparks for the Bless breath effect.
 */
// FUNCTION: FF7 0x0051137c
void __cdecl BattleFx_SpecialRadiantBreathTimelineTask(void)
{
    BattleTaskSlot *pCurrent;
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int s; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int t; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int y; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxRadiantBreathGroundSlot *burstPtr;
    short actorIndex8;
    BattleFxRadiantBreathTimelineState *tl;
    int d; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int e; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int f; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int g; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int h; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    pCurrent = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (pCurrent->frameCounter >= 0 && pCurrent->frameCounter < 8) {
        tl = (BattleFxRadiantBreathTimelineState *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialRadiantBreathFrameTableSpriteTask)];
        tl->offsetX = rand() % 0x1770 - 0xBB8;
        tl->offsetY = 0;
        tl->offsetZ = rand() % 0x1770 - 0xBB8;
        tl->variant = rand() % 2;
        burstPtr = (BattleFxRadiantBreathGroundSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialRadiantBreathGroundBurstTask);
        burstPtr->offsetX = tl->offsetX;
        burstPtr->offsetY = 0;
        burstPtr->offsetZ = tl->offsetZ;
    }
    if (pCurrent->frameCounter == 0x14) {
        tl = (BattleFxRadiantBreathTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialRadiantBreathGrowingModelSparksTask);
        tl->offsetX = 0;
        tl->offsetY = 0xFB50;
        tl->offsetZ = 0;
        tl = (BattleFxRadiantBreathTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialRadiantBreathExpandingShellsTask);
        tl->offsetX = 0;
        tl->offsetY = 0xFB50;
        tl->offsetZ = 0;
        Battle_PlaySfxPannedToActor(0x164, -1, 0x40);
    }
    if (pCurrent->frameCounter == 0x20) {
        Battle_AllocateResourceSlot(BattleFx_SpecialRadiantBreathIntensityRampTask);
    }
    if (pCurrent->frameCounter == 0x30) {
        Battle_StartFixedPointRamp(0, 0x10);
    }
    if (pCurrent->frameCounter == 0x3C) {
        for (actorIndex8 = 0; actorIndex8 < 0xA; actorIndex8++) {
            if ((g_BattleSpecRadiantBreathTargetMask & 1 << actorIndex8) != 0) {
                Battle_PostActorHitOrHurtMessage(actorIndex8);
            }
        }
    }
    (pCurrent->frameCounter)++;
    if (pCurrent->frameCounter == 0x46) {
        pCurrent->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Per-frame step that draws nothing and only drives g_BattleSpecRadiantBreathIntensity. While not paused it
 * adds 0xF for frames below 0x10 and subtracts 0xF for frames above 0x10 (frame 0x10 itself
 * changes nothing), advances the frame counter, and retires the slot at frame 0x20: sixteen
 * rises, a hold and fifteen falls.
 */
// FUNCTION: FF7 0x005115aa
void __cdecl BattleFx_SpecialRadiantBreathIntensityRampTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter < 0x10) {
        g_BattleSpecRadiantBreathIntensity += 0xF;
    }
    if (task->frameCounter > 0x10) {
        g_BattleSpecRadiantBreathIntensity -= 0xF;
    }
    (task->frameCounter)++;
    if (task->frameCounter == 0x20) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Scales and submits a ground-burst model column during the Bless breath effect.
 */
// FUNCTION: FF7 0x00511634
void __cdecl BattleFx_SpecialRadiantBreathGroundBurstTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        int burstState;
        struct { unsigned char pad_to_4[0x4]; SVECTOR angleX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix burstMatrix; };
        struct { unsigned char pad_to_30[0x30]; BattleFxOilTargetModelSlot *burstSlot; };
        struct { unsigned char pad_to_34[0x34]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.burstSlot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.burstSlot->frameCounter < 0x14) {
        frame.burstState = 8;
    } else {
        frame.burstState = 0x1C - frame.burstSlot->frameCounter;
    }
    g_BattleSpecRadiantBreathGroundFade = frame.burstState;
    frame.angleX.vz = 0;
    frame.angleX.vx = frame.angleX.vz;
    frame.angleX.vy = frame.burstSlot->frameCounter << 6;
    RotMatrix(&frame.angleX, &frame.burstMatrix);
    g_BattleSpecRadiantBreathBurstScaleZ = frame.burstSlot->frameCounter * 0x258;
    g_BattleSpecRadiantBreathBurstScaleY = g_BattleSpecRadiantBreathBurstScaleZ;
    g_BattleSpecRadiantBreathBurstScaleX = g_BattleSpecRadiantBreathBurstScaleY;
    ScaleMatrix(&frame.burstMatrix, &g_BattleSpecRadiantBreathBurstScaleX);
    frame.burstMatrix.t[0] = frame.burstSlot->posX;
    frame.burstMatrix.t[1] = frame.burstSlot->posY;
    frame.burstMatrix.t[2] = frame.burstSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.burstMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecRadiantBreathTransformData1.groundModel.fadeLevel = g_BattleSpecRadiantBreathGroundFade << 4;
    g_BattleSpecRadiantBreathTransformData1.groundModel.modelGroup = Gfx_ModelTableGetRenderObject(0xF, g_BattleSpecRadiantBreathLoadedResTable);
    g_BattleSpecRadiantBreathDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecRadiantBreathTransformData1.groundModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRadiantBreathDrawHandle);
    g_BattleSpecRadiantBreathTransformData1.groundModel.modelGroup = Gfx_ModelTableGetRenderObject(0x10, g_BattleSpecRadiantBreathLoadedResTable);
    g_BattleSpecRadiantBreathDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecRadiantBreathTransformData1.groundModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRadiantBreathDrawHandle);
    BattleFx_InstallBillboardMatrix((void *)((int)frame.burstSlot + 4), ((frame.burstSlot->frameCounter % 2) << 0xA) + 0x1000, 0);
    g_BattleSpecRadiantBreathTransformData1.groundSprite.drawObj = Gfx_ModelTableGetSpriteObject(0x14, g_BattleSpecRadiantBreathLoadedResTable);
    g_BattleSpecRadiantBreathTransformData1.groundSprite.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0x14, g_BattleSpecRadiantBreathLoadedResTable);
    g_BattleSpecRadiantBreathDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecRadiantBreathTransformData1.groundSprite, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRadiantBreathDrawHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.burstSlot->frameCounter)++;
    if (frame.burstSlot->frameCounter == 0x1C) {
        frame.burstSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Per-frame step for the current secondary-slot record of the bless effect. Installs a billboard
// matrix at the slot position at scale 0x2000. Loads the frame parameter word from
// g_BattleSpecRadiantBreathParam1 indexed by frameIndex, and the two sprite fields from the resource that
// animIndex selects in g_BattleSpecRadiantBreathAnimData2 (via g_BattleSpecRadiantBreathLoadedResTable), so
// animIndex chooses which resource is drawn. Submits the draw into the depth-sorted ordering
// table. While not paused the frame advances, and the slot is retired exactly at frame 0x1C.
// FUNCTION: FF7 0x00511861
void __cdecl BattleFx_SpecialRadiantBreathFrameTableSpriteTask(void)
{
    BattleFxRadiantBreathFrameSprite *task;

    task = (BattleFxRadiantBreathFrameSprite *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), 0x2000, 0);
    g_BattleSpecRadiantBreathTransformData1.frameSprite.frame = g_BattleSpecRadiantBreathParam1[task->frameIndex];
    g_BattleSpecRadiantBreathTransformData1.frameSprite.drawObj = Gfx_ModelTableGetSpriteObject(g_BattleSpecRadiantBreathAnimData2[task->animIndex], g_BattleSpecRadiantBreathLoadedResTable);
    g_BattleSpecRadiantBreathTransformData1.frameSprite.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(g_BattleSpecRadiantBreathAnimData2[task->animIndex], g_BattleSpecRadiantBreathLoadedResTable);
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecRadiantBreathTransformData1.frameSprite, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frameIndex)++;
        if (task->frameIndex == 0x1C) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws a growing Bless pillar model while emitting rising spark sprites around its base.
 */
// FUNCTION: FF7 0x00511942
void __cdecl BattleFx_SpecialRadiantBreathGrowingModelSparksTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[80];
        SVECTOR angleX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix sparkMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleFxRadiantBreathAnimatedSpriteSlot *spriteSlot; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxRadiantBreathGrowingModelSparksSlot *sparksSlot; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.sparksSlot = (BattleFxRadiantBreathGrowingModelSparksSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.angleX.vz = 0;
    frame.angleX.vx = frame.angleX.vz;
    frame.angleX.vy = 0;
    RotMatrixXYZ(&frame.angleX, &frame.sparkMatrix);
    g_BattleSpecRadiantBreathSparkScaleZ = frame.sparksSlot->frameCounter << 0xA;
    g_BattleSpecRadiantBreathSparkScaleY = g_BattleSpecRadiantBreathSparkScaleZ;
    g_BattleSpecRadiantBreathSparkScaleX = g_BattleSpecRadiantBreathSparkScaleY;
    ScaleMatrix(&frame.sparkMatrix, &g_BattleSpecRadiantBreathSparkScaleX);
    frame.sparkMatrix.t[0] = frame.sparksSlot->growthX;
    frame.sparkMatrix.t[1] = frame.sparksSlot->growthY;
    frame.sparkMatrix.t[2] = frame.sparksSlot->growthZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.sparkMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    SetDrawColorRGB(0, 0, 0);
    (g_BattleSpecRadiantBreathTransformData2.resEntry) = Gfx_ModelTableGetRenderObject(2, g_BattleSpecRadiantBreathLoadedResTable);
    g_BattleSpecRadiantBreathTransformData2.flags = 0xA8;
    g_BattleSpecRadiantBreathTransformData2.alpha = 0x1000 - (g_BattleSpecRadiantBreathSparkFade << 7);
    g_BattleSpecRadiantBreathDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecRadiantBreathTransformData2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRadiantBreathDrawHandle);
    (g_BattleSpecRadiantBreathTransformData2.resEntry) = Gfx_ModelTableGetRenderObject(g_BattleSpecRadiantBreathAnimData3[(frame.sparksSlot->frameCounter % 8)], g_BattleSpecRadiantBreathLoadedResTable);
    g_BattleSpecRadiantBreathTransformData2.flags = 0x28;
    g_BattleSpecRadiantBreathTransformData2.alpha = g_BattleSpecRadiantBreathSparkFade << 3;
    g_BattleSpecRadiantBreathDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecRadiantBreathTransformData2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRadiantBreathDrawHandle);
    BattleFx_InstallBillboardMatrix((void *)((int)frame.sparksSlot + 4), 0x2000, 0);
    g_BattleSpecRadiantBreathSpriteRequest2.req2Byte2 = g_BattleSpecRadiantBreathSparkFade << 3;
    g_BattleSpecRadiantBreathSpriteRequest2.req2Byte1 = g_BattleSpecRadiantBreathSpriteRequest2.req2Byte2;
    (g_BattleSpecRadiantBreathSpriteRequest2.red) = g_BattleSpecRadiantBreathSpriteRequest2.req2Byte1;
    g_BattleSpecRadiantBreathAnimData4.drawObj = Gfx_ModelTableGetSpriteObject(0x13, g_BattleSpecRadiantBreathLoadedResTable);
    g_BattleSpecRadiantBreathAnimData4.record24 = Gfx_ModelTableGetLoadedRecord(0x13, g_BattleSpecRadiantBreathLoadedResTable);
    g_BattleSpecRadiantBreathDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecRadiantBreathAnimData4.record24, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRadiantBreathDrawHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.sparksSlot->frameCounter < 0xC) {
        frame.spriteSlot = (BattleFxRadiantBreathAnimatedSpriteSlot *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialRadiantBreathAnimatedSpriteTask)];
        frame.spriteSlot->posX = frame.sparksSlot->growthX;
        frame.spriteSlot->posY = frame.sparksSlot->growthY;
        frame.spriteSlot->posZ = frame.sparksSlot->growthZ;
        frame.spriteSlot->rotOrVelY = 0;
        frame.spriteSlot->rotOrVelX = 0;
        frame.spriteSlot->rotOrVelZ = g_BattleSpecRadiantBreathAnimData5[frame.sparksSlot->frameCounter] * 0x154 + rand() % 0xC8 + -0x64;
        frame.spriteSlot->animIndex = (rand() % 2);
    }
    g_BattleCameraFocalPoint.focalPointY += (short)(0x20 - ((frame.sparksSlot->frameCounter % 2) << 6));
    if (frame.sparksSlot->frameCounter > 0xE) {
        g_BattleSpecRadiantBreathSparkFade--;
    }
    (frame.sparksSlot->frameCounter)++;
    if (frame.sparksSlot->frameCounter == 0x1E) {
        frame.sparksSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Advances and submits one animated spark billboard sprite for the Bless effect.
 */
// FUNCTION: FF7 0x00511c99
void __cdecl BattleFx_SpecialRadiantBreathAnimatedSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix blessMatrix; };
        struct { unsigned char pad_to_48[0x48]; short axisIndex; };
        struct { unsigned char pad_to_4C[0x4C]; int transDepth; };
        struct { unsigned char pad_to_50[0x50]; BattleFxRadiantBreathAnimatedSpriteSlot *blessSlot; };
    } frame;
#pragma pack(pop)

    frame.blessSlot = (BattleFxRadiantBreathAnimatedSpriteSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.blessMatrix.m[2][frame.axisIndex] = 0;
        frame.blessMatrix.m[1][frame.axisIndex] = 0;
        frame.blessMatrix.m[0][frame.axisIndex] = 0;
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((char *)frame.blessSlot + 4, frame.blessMatrix.t, &frame.transDepth);
    frame.blessMatrix.m[2][2] = 0x3000;
    frame.blessMatrix.m[1][1] = frame.blessMatrix.m[2][2];
    frame.blessMatrix.m[0][0] = frame.blessMatrix.m[1][1];
    RotMatrix((SVECTOR *)((int)frame.blessSlot + 0xC), &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.blessMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.blessMatrix);
    SetTransMatrix(&frame.blessMatrix);
    g_BattleSpecRadiantBreathSpriteRequest1.req1Word12 = frame.blessSlot->frameCounter;
    g_BattleSpecRadiantBreathSpriteRequest1.req1Resource = Gfx_ModelTableGetSpriteObject(g_BattleSpecRadiantBreathAnimData1[frame.blessSlot->animIndex], g_BattleSpecRadiantBreathLoadedResTable);
    (g_BattleSpecRadiantBreathSpriteRequest1.spriteEntry) = Gfx_ModelTableGetLoadedRecord(g_BattleSpecRadiantBreathAnimData1[frame.blessSlot->animIndex], g_BattleSpecRadiantBreathLoadedResTable);
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecRadiantBreathSpriteRequest1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.blessSlot->frameCounter)++;
    if (frame.blessSlot->frameCounter == *(int *)((char *)*(int *)(g_BattleSpecRadiantBreathSpriteRequest1.spriteEntry) + 4)) {
        frame.blessSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Rotates, scales, and draws concentric expanding energy shells for the Bless effect.
 */
// FUNCTION: FF7 0x00511e2a
void __cdecl BattleFx_SpecialRadiantBreathExpandingShellsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[88];
        struct { unsigned char pad_to_4[0x4]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_24[0x24]; BattleFxRadiantBreathExpandingShellsSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_28[0x28]; short spawnIndex; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxRadiantBreathExpandingShellsSlot *shellSlot; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix shellMatrix; };
        struct { unsigned char pad_to_50[0x50]; short radius; };
        struct { unsigned char pad_to_54[0x54]; short angle; };
    } frame;
#pragma pack(pop)

    frame.shellSlot = (BattleFxRadiantBreathExpandingShellsSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecRadiantBreathShellAngles.vx) = 0;
    g_BattleSpecRadiantBreathShellAngles.vy = frame.shellSlot->frameCounter << 6;
    g_BattleSpecRadiantBreathShellAngleZ = 0;
    RotMatrix((SVECTOR *)&g_BattleSpecRadiantBreathShellAngles, &frame.rotMatrix);
    g_BattleSpecRadiantBreathShellScaleZ = frame.shellSlot->frameCounter * 0x7D0;
    g_BattleSpecRadiantBreathShellScaleY = g_BattleSpecRadiantBreathShellScaleZ;
    g_BattleSpecRadiantBreathShellScaleX = g_BattleSpecRadiantBreathShellScaleY;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecRadiantBreathShellScaleX);
    frame.rotMatrix.t[0] = frame.shellSlot->posX;
    frame.rotMatrix.t[1] = frame.shellSlot->posY;
    frame.rotMatrix.t[2] = frame.shellSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.shellMatrix);
    SetRotMatrix(&frame.shellMatrix);
    SetTransMatrix(&frame.shellMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecRadiantBreathSpriteRequest4.fadeLevel = 0x80 - frame.shellSlot->frameCounter * 6;
    g_BattleSpecRadiantBreathSpriteRequest4.modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecRadiantBreathLoadedResTable);
    g_BattleSpecRadiantBreathDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecRadiantBreathSpriteRequest4.modelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRadiantBreathDrawHandle);
    RotMatrixXYZ((short *)&g_BattleSpecRadiantBreathShellAngles, &frame.rotMatrix);
    g_BattleSpecRadiantBreathShellScaleZ = frame.shellSlot->frameCounter * 0x320;
    g_BattleSpecRadiantBreathShellScaleY = g_BattleSpecRadiantBreathShellScaleZ;
    g_BattleSpecRadiantBreathShellScaleX = g_BattleSpecRadiantBreathShellScaleY;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecRadiantBreathShellScaleX);
    frame.rotMatrix.t[0] = frame.shellSlot->posX;
    frame.rotMatrix.t[1] = 0;
    frame.rotMatrix.t[2] = frame.shellSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.shellMatrix);
    SetRotMatrix(&frame.shellMatrix);
    SetTransMatrix(&frame.shellMatrix);
    g_BattleSpecRadiantBreathSpriteRequest4.fadeLevel = 0x80 - frame.shellSlot->frameCounter * 6;
    g_BattleSpecRadiantBreathSpriteRequest4.modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleSpecRadiantBreathLoadedResTable);
    g_BattleSpecRadiantBreathDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecRadiantBreathSpriteRequest4.modelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRadiantBreathDrawHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.radius = frame.shellSlot->frameCounter * 0xF0;
    frame.spawnSlot = (BattleFxRadiantBreathExpandingShellsSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialRadiantBreathRisingFadeSpriteTask);
    for (frame.spawnIndex = 0; frame.spawnIndex < 2; frame.spawnIndex++) {
        frame.angle = (rand() % 4096);
        frame.spawnSlot->offsetX = CosFixed(frame.angle) * frame.radius >> 0xC;
        frame.spawnSlot->offsetZ = SinFixed(frame.angle) * frame.radius >> 0xC;
        frame.spawnSlot->offsetY = 0;
        frame.spawnSlot->variant = rand() % 7;
    }
    if ((frame.shellSlot->frameCounter % 2) == 0) {
        frame.spawnSlot = (BattleFxRadiantBreathExpandingShellsSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialRadiantBreathRisingSpinModelTask);
        frame.angle = (rand() % 4096);
        frame.spawnSlot->offsetX = CosFixed(frame.angle) * frame.radius >> 0xC;
        frame.spawnSlot->offsetZ = SinFixed(frame.angle) * frame.radius >> 0xC;
        frame.spawnSlot->offsetY = 0;
        frame.spawnSlot->variant = (rand() % 4);
        frame.spawnSlot->angle = (rand() % 4096);
    }
    for (frame.spawnIndex = 0; frame.spawnIndex < 3; frame.spawnIndex++) {
        frame.spawnSlot = (BattleFxRadiantBreathExpandingShellsSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialRadiantBreathLargeAnimatedSpriteTask);
        frame.angle = (rand() % 4096);
        frame.spawnSlot->offsetX = CosFixed(frame.angle) * frame.radius >> 0xC;
        frame.spawnSlot->offsetZ = SinFixed(frame.angle) * frame.radius >> 0xC;
        frame.spawnSlot->offsetY = 0;
    }
    (frame.shellSlot->frameCounter)++;
    if (frame.shellSlot->frameCounter == 0x14) {
        frame.shellSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Per-frame step for the current task slot. It installs a billboard matrix at the slot position at
 * scale 0x3000, copies the frame counter into the sprite request, fills the request from resource
 * 0x12 of g_BattleSpecRadiantBreathLoadedResTable, and submits it into the depth-sorted ordering table. While
 * not paused the frame advances and the slot retires when the frame equals the animation's frame
 * count (read through the request's sprite entry); unlike its neighbours this one compares for
 * equality and does not test the resource for null.
 */
// FUNCTION: FF7 0x00512297
void __cdecl BattleFx_SpecialRadiantBreathLargeAnimatedSpriteTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), 0x3000, 0);
    g_BattleSpecRadiantBreathSpriteRequest3.sprite.frame = task->frameCounter;
    g_BattleSpecRadiantBreathSpriteRequest3.sprite.drawObj = Gfx_ModelTableGetSpriteObject(0x12, g_BattleSpecRadiantBreathLoadedResTable);
    g_BattleSpecRadiantBreathSpriteRequest3.sprite.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0x12, g_BattleSpecRadiantBreathLoadedResTable);
    g_BattleSpecRadiantBreathDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecRadiantBreathSpriteRequest3.sprite, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRadiantBreathDrawHandle);
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
        if (task->frameCounter == g_BattleSpecRadiantBreathSpriteRequest3.sprite.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Translates upward, spins, and draws a rising shock-ring model for the Bless effect.
 */
// FUNCTION: FF7 0x0051235e
void __cdecl BattleFx_SpecialRadiantBreathRisingSpinModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        SVECTOR angleX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleFxRadiantBreathRisingSpinModelSlot *spinSlot; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix spinMatrix; };
    } frame;
#pragma pack(pop)

    frame.spinSlot = (BattleFxRadiantBreathRisingSpinModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.angleX.vy = 0;
    frame.angleX.vx = frame.angleX.vy;
    frame.angleX.vz = frame.spinSlot->spinAngle;
    RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
    g_BattleSpecRadiantBreathSpinScaleZ = 0x1000;
    g_BattleSpecRadiantBreathSpinScaleY = g_BattleSpecRadiantBreathSpinScaleZ;
    g_BattleSpecRadiantBreathSpinScaleX = g_BattleSpecRadiantBreathSpinScaleY;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecRadiantBreathSpinScaleX);
    frame.rotMatrix.t[0] = frame.spinSlot->posX;
    frame.rotMatrix.t[1] = frame.spinSlot->posY;
    frame.rotMatrix.t[2] = frame.spinSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.spinMatrix);
    SetRotMatrix(&frame.spinMatrix);
    SetTransMatrix(&frame.spinMatrix);
    g_BattleSpecRadiantBreathSpriteRequest3.fadeLevel = 0x80 - (frame.spinSlot->frameCounter << 3);
    g_BattleSpecRadiantBreathSpriteRequest3.modelGroup = Gfx_ModelTableGetRenderObject(g_BattleSpecRadiantBreathAnimData6[frame.spinSlot->animIndex], g_BattleSpecRadiantBreathLoadedResTable);
    g_BattleSpecRadiantBreathDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecRadiantBreathSpriteRequest3.modelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRadiantBreathDrawHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spinSlot->posY -= 0x190;
    frame.spinSlot->spinAngle += 0x100;
    (frame.spinSlot->frameCounter)++;
    if (frame.spinSlot->frameCounter == 0xF) {
        frame.spinSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Per-frame step for a rising, fading sprite, the same shape as
 * BattleFx_SpecialUltimateBlastRisingFadeSpriteTask with different globals. It installs a billboard matrix
 * at the slot position at scale 0x2000, copies the slot's variant into sprite request 4, writes the
 * fade level 0x80 - (fadeStep << 3) into its three colour bytes, fills the sprite record from
 * resource 0x11 of g_BattleSpecRadiantBreathLoadedResTable, and submits it into the depth-sorted ordering
 * table. While not paused it subtracts 0x12C from posY, advances fadeStep, and retires the slot at
 * fadeStep 0xF.
 */
// FUNCTION: FF7 0x005124d5
void __cdecl BattleFx_SpecialRadiantBreathRisingFadeSpriteTask(void)
{
    BattleFxHeatWaveRisingFadeSprite *task;

    task = (BattleFxHeatWaveRisingFadeSprite *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), 0x2000, 0);
    g_BattleSpecRadiantBreathSpriteRequest4.frameIndex = task->variant;
    g_BattleSpecRadiantBreathSpriteRequest4.blue = 0x80 - (task->fadeStep << 3);
    g_BattleSpecRadiantBreathSpriteRequest4.green = g_BattleSpecRadiantBreathSpriteRequest4.blue;
    g_BattleSpecRadiantBreathSpriteRequest4.red = g_BattleSpecRadiantBreathSpriteRequest4.green;
    g_BattleSpecRadiantBreathAnimData7.drawObj = Gfx_ModelTableGetSpriteObject(0x11, g_BattleSpecRadiantBreathLoadedResTable);
    g_BattleSpecRadiantBreathAnimData7.risingFadeRecord = Gfx_ModelTableGetLoadedRecord(0x11, g_BattleSpecRadiantBreathLoadedResTable);
    g_BattleSpecRadiantBreathDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecRadiantBreathAnimData7.risingFadeRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRadiantBreathDrawHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posY -= 0x12C;
    (task->fadeStep)++;
    if (task->fadeStep == 0xF) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
