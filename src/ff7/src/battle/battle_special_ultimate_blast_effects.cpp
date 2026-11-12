// src/ff7/src/battle/battle_special_ultimate_blast_effects.cpp
/* Enemy Special Attack [117]: Ultimate Blast / Gunge Lance (orig. SPECIAL/SAIKYO, 最強) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Saikyo spell. It loads its resources, starts the
//   timeline, and runs the screen flash and the growing, drifting, expanding, and rising model and
//   sprite tasks.

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
// GLOBAL: FF7 0x00899360
unsigned int g_BattleSpecUltimateBlastResourceDef[126] = {0x001900b1, (unsigned int)&g_BattleSpecUltimateBlastResName_wa, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecUltimateBlastResName_kage_netu1, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecUltimateBlastResName_kage_netu1[12], 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecUltimateBlastResName_uchi1_uchi2, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecUltimateBlastResName_uchi1_uchi2[12], 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecUltimateBlastResName_uchi3_uchi4, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecUltimateBlastResName_uchi3_uchi4[12], 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecUltimateBlastResName_uchi5_uchi6, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecUltimateBlastResName_uchi5_uchi6[12], 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecUltimateBlastResName_uchi7_uchi8, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecUltimateBlastResName_uchi7_uchi8[12], 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)&g_BattleSpecUltimateBlastResName_iwa1_iwa2, 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)&g_BattleSpecUltimateBlastResName_iwa1_iwa2[12], 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)&g_BattleSpecUltimateBlastResName_iwa3_iwa4, 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)&g_BattleSpecUltimateBlastResName_iwa3_iwa4[12], 0x00000000, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecUltimateBlastResName_breath03_1, (unsigned int)&g_BattleSpecUltimateBlastResName_hikari1_1, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecUltimateBlastResName_hikari1_1[12], (unsigned int)&g_BattleSpecUltimateBlastResName_hikari1_1[28], 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecUltimateBlastResName_breath03_2, (unsigned int)&g_BattleSpecUltimateBlastResName_kemu1_1, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecUltimateBlastResName_kemu1_1[12], (unsigned int)&g_BattleSpecUltimateBlastResName_kemu1_1[28], 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecUltimateBlastResName_breath02_1, (unsigned int)&g_BattleSpecUltimateBlastResName_onpa2, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecUltimateBlastResName_onpa2[12], (unsigned int)&g_BattleSpecUltimateBlastResName_onpa2[28], 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecUltimateBlastResName_breath02_2, (unsigned int)&g_BattleSpecUltimateBlastResName_onpa4, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecUltimateBlastResName_onpa4[12], (unsigned int)&g_BattleSpecUltimateBlastResName_onpa4[28], 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecUltimateBlastResName_breath02_3, (unsigned int)&g_BattleSpecUltimateBlastResName_onpa6, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecUltimateBlastResName_onpa6[12], (unsigned int)&g_BattleSpecUltimateBlastResName_onpa6[28], 0x00000000, 0x00000000, 0x00000000};
// The drifting sprites' request.
// GLOBAL: FF7 0x00899558
BattleFxSpriteRequest g_BattleSpecUltimateBlastSpriteRequest1 = {0, 0, -128, -128, -128, 0x2e};
// GLOBAL: FF7 0x00899568
int g_BattleSpecUltimateBlastAnimData1[8] = {0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18};
// GLOBAL: FF7 0x00899588
struct BattleSpecialFallingRocksModelReq1 g_BattleSpecUltimateBlastAnimData2 = {0, 0, {0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x00899598
int g_BattleSpecUltimateBlastAnimData3[2] = {3, 4};
// GLOBAL: FF7 0x008995a0
struct BattleSpecUltimateBlastAnimData4 g_BattleSpecUltimateBlastAnimData4 = {{5, 0, 0, 0, 6, 0, 0, 0, 7, 0, 0, 0, 8, 0, 0, 0, 9, 0, 0, 0, 0xa, 0, 0, 0}, 0, 0};
// GLOBAL: FF7 0x008995c0
struct BattleSpecUltimateBlastSpriteRequest2 g_BattleSpecUltimateBlastSpriteRequest2 = {-128, -128, -128, {0x2e}};
// GLOBAL: FF7 0x008995c8
BattleSpecUltimateBlastTransformData1 g_BattleSpecUltimateBlastTransformData1 = {0, 0, {0x80, 0x80, 0x80, 0x2e}};
unsigned char g_rawData_008995e4[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008995e8
int g_BattleSpecUltimateBlastAnimData5[2] = {0xb, 0xc};
// GLOBAL: FF7 0x008995f0
struct BattleSpecUltimateBlastAnimData6 g_BattleSpecUltimateBlastAnimData6 = {{0xd, 0, 0, 0, 0xe, 0, 0, 0}, 0, 0};
// GLOBAL: FF7 0x00899600
BattleSpecUltimateBlastSpriteRequest3 g_BattleSpecUltimateBlastSpriteRequest3 = {-128, -128, -128, {0x2c}, 0, {0}, 0, {0xa8}, 0x80};
unsigned char g_rawData_00899614[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00899618
unsigned char g_BattleSpecUltimateBlastResName_wa[7] = "wa.rsd";
// GLOBAL: FF7 0x00899620
unsigned char g_BattleSpecUltimateBlastResName_kage_netu1[22] =
    "kage.rsd\0\0\0\0"
    "netu1.rsd";
// GLOBAL: FF7 0x00899638
unsigned char g_BattleSpecUltimateBlastResName_uchi1_uchi2[22] =
    "uchi1.rsd\0\0\0"
    "uchi2.rsd";
// GLOBAL: FF7 0x00899650
unsigned char g_BattleSpecUltimateBlastResName_uchi3_uchi4[22] =
    "uchi3.rsd\0\0\0"
    "uchi4.rsd";
// GLOBAL: FF7 0x00899668
unsigned char g_BattleSpecUltimateBlastResName_uchi5_uchi6[22] =
    "uchi5.rsd\0\0\0"
    "uchi6.rsd";
// GLOBAL: FF7 0x00899680
unsigned char g_BattleSpecUltimateBlastResName_uchi7_uchi8[22] =
    "uchi7.rsd\0\0\0"
    "uchi8.rsd";
// GLOBAL: FF7 0x00899698
unsigned char g_BattleSpecUltimateBlastResName_iwa1_iwa2[21] =
    "iwa1.rsd\0\0\0\0"
    "iwa2.rsd";
// GLOBAL: FF7 0x008996b0
unsigned char g_BattleSpecUltimateBlastResName_iwa3_iwa4[21] =
    "iwa3.rsd\0\0\0\0"
    "iwa4.rsd";
// GLOBAL: FF7 0x008996c8
unsigned char g_BattleSpecUltimateBlastResName_breath03_1[13] = "breath03.tim";
// GLOBAL: FF7 0x008996d8
unsigned char g_BattleSpecUltimateBlastResName_hikari1_1[39] =
    "hikari1.spt\0"
    "breath03.tim\0\0\0\0"
    "hahen1.spt";
// GLOBAL: FF7 0x00899700
unsigned char g_BattleSpecUltimateBlastResName_breath03_2[13] = "breath03.tim";
// GLOBAL: FF7 0x00899710
unsigned char g_BattleSpecUltimateBlastResName_kemu1_1[38] =
    "kemu1.spt\0\0\0"
    "breath02.tim\0\0\0\0"
    "onpa1.spt";
// GLOBAL: FF7 0x00899738
unsigned char g_BattleSpecUltimateBlastResName_breath02_1[13] = "breath02.tim";
// GLOBAL: FF7 0x00899748
unsigned char g_BattleSpecUltimateBlastResName_onpa2[38] =
    "onpa2.spt\0\0\0"
    "breath02.tim\0\0\0\0"
    "onpa3.spt";
// GLOBAL: FF7 0x00899770
unsigned char g_BattleSpecUltimateBlastResName_breath02_2[13] = "breath02.tim";
// GLOBAL: FF7 0x00899780
unsigned char g_BattleSpecUltimateBlastResName_onpa4[38] =
    "onpa4.spt\0\0\0"
    "breath02.tim\0\0\0\0"
    "onpa5.spt";
// GLOBAL: FF7 0x008997a8
unsigned char g_BattleSpecUltimateBlastResName_breath02_3[13] = "breath02.tim";
// GLOBAL: FF7 0x008997b8
unsigned char g_BattleSpecUltimateBlastResName_onpa6[38] =
    "onpa6.spt\0\0\0"
    "breath02.tim\0\0\0\0"
    "onpa7.spt";
// GLOBAL: FF7 0x00b4fbc8
int g_BattleSpecUltimateBlastDrawListHandle = 0;
// GLOBAL: FF7 0x00b4fbcc
short g_BattleSpecUltimateBlastTargetMask = 0;
// GLOBAL: FF7 0x00b4fbd0
int g_BattleSpecUltimateBlastSpinScaleX = 0;
// GLOBAL: FF7 0x00b4fbd4
int g_BattleSpecUltimateBlastSpinScaleY = 0;
// GLOBAL: FF7 0x00b4fbd8
int g_BattleSpecUltimateBlastSpinScaleZ = 0;
unsigned char g_bssPad_00b4fbdc[4] = {0};
// GLOBAL: FF7 0x00b4fbe0
int g_BattleSpecUltimateBlastScaleX = 0;
// GLOBAL: FF7 0x00b4fbe4
int g_BattleSpecUltimateBlastScaleY = 0;
// GLOBAL: FF7 0x00b4fbe8
int g_BattleSpecUltimateBlastScaleZ = 0;
unsigned char g_bssPad_00b4fbec[4] = {0};
// GLOBAL: FF7 0x00b4fbf0
int g_BattleSpecUltimateBlastVelX = 0;
// GLOBAL: FF7 0x00b4fbf4
int g_BattleSpecUltimateBlastVelY = 0;
// GLOBAL: FF7 0x00b4fbf8
int g_BattleSpecUltimateBlastVelZ = 0;
unsigned char g_bssPad_00b4fbfc[4] = {0};
// GLOBAL: FF7 0x00b4fc00
short g_BattleSpecUltimateBlastAngle = 0;
// GLOBAL: FF7 0x00b4fc04
short g_BattleSpecUltimateBlastStep = 0;
// GLOBAL: FF7 0x00b4fc08
struct GfxTableCursor *g_BattleSpecUltimateBlastLoadedResTable = 0;
unsigned char g_bssPad_00b4fc0c[4] = {0};
// GLOBAL: FF7 0x00b4fc10
struct BattleSpecUltimateBlastBuffer g_BattleSpecUltimateBlastBuffer = {0};
// GLOBAL: FF7 0x00b4fc14
short g_BattleSpecUltimateBlastActiveCount = 0;
// clang-format on


/*
 * Clears the shared flash intensity and sets its duration to sixteen. With a stage object
 * present and the resource not yet loaded, it builds a request for SPECIAL/SAIKYO and loads
 * g_BattleSpecUltimateBlastResourceDef through Gfx_LoadModelTableIfAbsent.
 */
// FUNCTION: FF7 0x0050f110
void __cdecl BattleFx_SpecialLoadResUltimateBlast(void)
{
    GfxModelTableBank *stage;
    GfxResourceRequest requestSlot;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resultDefArr;

    g_BattleSpecUltimateBlastStep = 0;
    g_BattleSpecUltimateBlastAngle = 0x10;
    if (g_pBattle3dScene != 0 && g_BattleSpecUltimateBlastLoadedResTable == 0) {
        kind = 0x19;
        resultDefArr = (int)&g_BattleSpecUltimateBlastResourceDef;
        stage = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008997e0
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/SAIKYO", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resultDefArr, pRequest, &g_BattleSpecUltimateBlastLoadedResTable, stage);
    }
}

/*
 * A thunk: forwards targetIdx and actorIdx to BattleFx_SpecialBeginUltimateBlast.
 */
// FUNCTION: FF7 0x0050f1b2
void __cdecl BattleFx_SpecialEnterUltimateBlast(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginUltimateBlast(targetMask, actorIdx);
}

/*
 * Loads the resources, stores targetIdx and starts BattleFx_SpecialUltimateBlastTimelineTask with the
 * position of the actorIdx actor's bone34 and the centroid of the targetIdx actors, clears the
 * global color triple and starts a 0x1000 ramp with duration 16.
 */
// FUNCTION: FF7 0x0050f1c7
void __cdecl BattleFx_SpecialBeginUltimateBlast(int targetMask, int actorIdx)
{
    BattleFxUltimateBlastTimelineStart *taskRec;

    BattleFx_SpecialLoadResUltimateBlast();
    g_BattleSpecUltimateBlastTargetMask = targetMask;
    taskRec = (BattleFxUltimateBlastTimelineStart *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialUltimateBlastTimelineTask);
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].bone34, &taskRec->posX);
    Battle_GetActorMaskCentroid(targetMask, &taskRec->targetX);
    g_BattleFxTimelineState.channel2 = 0;
    g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
    g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    Battle_StartFixedPointRamp(0x1000, 0x10);
}

/*
 * Timeline of the Saikyo special: starts the growing and expanding model tasks, the screen flash
 * and the ramp-down at fixed frames, then allocates the typed records for the targets.
 */
// FUNCTION: FF7 0x0050f25d
void __cdecl BattleFx_SpecialUltimateBlastTimelineTask(void)
{
    int p, q, r, s, t, u, v, w, x, y, z;
    BattleTaskSlot *pCur;
    short actorIndex8;
    BattleFxUltimateBlastTimeline2 *spawn;
    int d, e, f, g, h;

    pCur = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (pCur->frameCounter == 0) {
        spawn = (BattleFxUltimateBlastTimeline2 *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialUltimateBlastGrowingModelSparksTask);
        spawn->posX = 0;
        spawn->initialOffset = 0xFB50;
        spawn->posZ = 0;
        spawn = (BattleFxUltimateBlastTimeline2 *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialUltimateBlastExpandingShellsTask);
        spawn->posX = 0;
        spawn->initialOffset = 0xFB50;
        spawn->posZ = 0;
        Battle_PlaySfxPannedToActor(0x164, -1, 0x40);
    }
    if (pCur->frameCounter == 0xC) {
        Battle_AllocateResourceSlot(BattleFx_SpecialUltimateBlastScreenFlashTask);
    }
    if (pCur->frameCounter == 0x1C) {
        Battle_StartFixedPointRamp(0, 0x10);
    }
    if (pCur->frameCounter == 0x28) {
        for (actorIndex8 = 0; actorIndex8 < 0xA; actorIndex8++) {
            if ((g_BattleSpecUltimateBlastTargetMask & 1 << actorIndex8) != 0) {
                Battle_PostActorHitOrHurtMessage(actorIndex8);
            }
        }
    }
    (pCur->frameCounter)++;
    if (pCur->frameCounter == 0x32) {
        pCur->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws a white flash from shared intensity before checking pause. When unpaused adds fifteen
 * below counter sixteen, subtracts fifteen above sixteen and holds it at exactly sixteen; advances
 * the counter and retires exactly at thirty-two.
 */
// FUNCTION: FF7 0x0050f3bb
void __cdecl BattleFx_SpecialUltimateBlastScreenFlashTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(*(char *)&g_BattleSpecUltimateBlastStep, g_BattleSpecUltimateBlastStep >> 2, 0);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter < 0x10) {
        g_BattleSpecUltimateBlastStep += 0xF;
    }
    if (task->frameCounter > 0x10) {
        g_BattleSpecUltimateBlastStep -= 0xF;
    }
    (task->frameCounter)++;
    if (task->frameCounter == 0x20) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws a growing model with a billboard, and spawns drifting spark sprites during its first
 * twelve frames.
 */
// FUNCTION: FF7 0x0050f461
void __cdecl BattleFx_SpecialUltimateBlastGrowingModelSparksTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[96];
        int projectedDepth;
        struct { unsigned char pad_to_4[0x4]; SVECTOR angleX; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR sparkVecX; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxEnergyWaveSpinSlot *sparkSlot; };
        struct { unsigned char pad_to_38[0x38]; short sparkIndex; };
        struct { unsigned char pad_to_3C[0x3C]; BattleFxOilTargetModelSlot *growingSlot; };
        struct { unsigned char pad_to_40[0x40]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.growingSlot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecUltimateBlastAnimData4.spriteField = Gfx_ModelTableGetLoadedRecord(0xF, g_BattleSpecUltimateBlastLoadedResTable);
    g_BattleSpecUltimateBlastAnimData4.spriteAlt = Gfx_ModelTableGetSpriteObject(0xF, g_BattleSpecUltimateBlastLoadedResTable);
    frame.angleX.vz = 0;
    frame.angleX.vx = frame.angleX.vz;
    frame.angleX.vy = 0;
    RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
    g_BattleSpecUltimateBlastVelZ = frame.growingSlot->frameCounter << 0xA;
    g_BattleSpecUltimateBlastVelY = g_BattleSpecUltimateBlastVelZ;
    g_BattleSpecUltimateBlastVelX = g_BattleSpecUltimateBlastVelY;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecUltimateBlastVelX);
    frame.rotMatrix.t[0] = frame.growingSlot->posX;
    frame.rotMatrix.t[1] = frame.growingSlot->posY;
    frame.rotMatrix.t[2] = frame.growingSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    SetDrawColorRGB(0, 0, 0);
    (g_BattleSpecUltimateBlastAnimData2.modelResource) = Gfx_ModelTableGetRenderObject(2, g_BattleSpecUltimateBlastLoadedResTable);
    g_BattleSpecUltimateBlastAnimData2.flags = 0xA8;
    g_BattleSpecUltimateBlastAnimData2.angle = 0x1000 - (g_BattleSpecUltimateBlastAngle << 7);
    g_BattleSpecUltimateBlastDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecUltimateBlastAnimData2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecUltimateBlastDrawListHandle);
    (g_BattleSpecUltimateBlastAnimData2.modelResource) = Gfx_ModelTableGetRenderObject(g_BattleSpecUltimateBlastAnimData3[(frame.growingSlot->frameCounter % 8)], g_BattleSpecUltimateBlastLoadedResTable);
    g_BattleSpecUltimateBlastAnimData2.flags = 0x28;
    g_BattleSpecUltimateBlastAnimData2.angle = g_BattleSpecUltimateBlastAngle << 3;
    g_BattleSpecUltimateBlastDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecUltimateBlastAnimData2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecUltimateBlastDrawListHandle);
    BattleFx_InstallBillboardMatrix(&frame.growingSlot->posX, 0x2000, 0);
    g_BattleSpecUltimateBlastSpriteRequest2.request2Byte2 = g_BattleSpecUltimateBlastAngle << 3;
    g_BattleSpecUltimateBlastSpriteRequest2.request2Byte1 = g_BattleSpecUltimateBlastSpriteRequest2.request2Byte2;
    (g_BattleSpecUltimateBlastSpriteRequest2.red) = g_BattleSpecUltimateBlastSpriteRequest2.request2Byte1;
    g_BattleSpecUltimateBlastDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecUltimateBlastAnimData4.spriteField, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecUltimateBlastDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.growingSlot->frameCounter < 0xC) {
        for (frame.sparkIndex = 0; frame.sparkIndex < 4; frame.sparkIndex++) {
            frame.sparkSlot = (BattleFxEnergyWaveSpinSlot *)(&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialUltimateBlastDriftingSpriteTask)]);
            frame.sparkSlot->posX = frame.growingSlot->posX;
            frame.sparkSlot->posY = frame.growingSlot->posY;
            frame.sparkSlot->posZ = frame.growingSlot->posZ;
            frame.sparkVecX.vy = 0x190;
            frame.sparkVecX.vz = 0;
            frame.sparkVecX.vx = frame.sparkVecX.vz;
            frame.angleX.vx = (rand() % 4096);
            frame.angleX.vz = (rand() % 4096);
            frame.angleX.vy = 0;
            RotMatrix(&frame.angleX, &frame.rotMatrix);
            frame.rotMatrix.t[2] = 0;
            frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
            frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
            SetRotMatrix(&frame.rotMatrix);
            SetTransMatrix(&frame.rotMatrix);
            RotTransPointToInt16(&frame.sparkVecX, &frame.sparkSlot->velocityX, &frame.projectedDepth);
            frame.sparkSlot->spriteVariant = rand() % 7;
        }
    }
    g_BattleCameraFocalPoint.focalPointY += (short)(0x20 - ((frame.growingSlot->frameCounter % 2) << 6));
    if (frame.growingSlot->frameCounter > 0xE) {
        g_BattleSpecUltimateBlastAngle--;
    }
    (frame.growingSlot->frameCounter)++;
    if (frame.growingSlot->frameCounter == 0x1E) {
        frame.growingSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws a fading, rotating sprite that drifts with its velocity for twelve frames.
 */
// FUNCTION: FF7 0x0050f830
void __cdecl BattleFx_SpecialUltimateBlastDriftingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int fadeLevel;
        struct { unsigned char pad_to_4[0x4]; SVECTOR angleX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; short fadeWord; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix xformCol2; };
        struct { unsigned char pad_to_50[0x50]; short axisIndex; };
        struct { unsigned char pad_to_54[0x54]; int projectedDepth; };
        struct { unsigned char pad_to_58[0x58]; BattleFxUltimateBlastDriftingSpriteSlot *driftSprite; };
    } frame;
#pragma pack(pop)

    frame.driftSprite = (BattleFxUltimateBlastDriftingSpriteSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    frame.angleX.vy = 0;
    frame.angleX.vx = 0;
    frame.angleX.vz = frame.driftSprite->frameCounter << 8;
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.xformCol2.m[frame.axisIndex][2] = 0;
        frame.xformCol2.m[frame.axisIndex][1] = 0;
        frame.xformCol2.m[frame.axisIndex][0] = 0;
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint(&frame.driftSprite->posX, frame.xformCol2.t, &frame.projectedDepth);
    frame.xformCol2.m[2][2] = 0x1400;
    frame.xformCol2.m[1][1] = frame.xformCol2.m[2][2];
    frame.xformCol2.m[0][0] = frame.xformCol2.m[1][1];
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.xformCol2, &frame.rotMatrix);
    SetRotMatrix(&frame.xformCol2);
    SetTransMatrix(&frame.xformCol2);
    if (frame.driftSprite->frameCounter < 4) {
        frame.fadeLevel = 8;
    } else {
        frame.fadeLevel = 0xC - frame.driftSprite->frameCounter;
    }
    frame.fadeWord = *(short *)&frame.fadeLevel;
    g_BattleSpecUltimateBlastSpriteRequest1.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(g_BattleSpecUltimateBlastAnimData1[frame.driftSprite->animIndex], g_BattleSpecUltimateBlastLoadedResTable);
    g_BattleSpecUltimateBlastSpriteRequest1.drawObj = Gfx_ModelTableGetSpriteObject(g_BattleSpecUltimateBlastAnimData1[frame.driftSprite->animIndex], g_BattleSpecUltimateBlastLoadedResTable);
    g_BattleSpecUltimateBlastSpriteRequest1.colorB = frame.fadeWord << 4;
    g_BattleSpecUltimateBlastSpriteRequest1.colorG = g_BattleSpecUltimateBlastSpriteRequest1.colorB;
    g_BattleSpecUltimateBlastSpriteRequest1.colorR = g_BattleSpecUltimateBlastSpriteRequest1.colorG;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecUltimateBlastSpriteRequest1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        frame.driftSprite->posX += frame.driftSprite->velocityX;
        frame.driftSprite->posY += frame.driftSprite->velocityY;
        frame.driftSprite->posZ += frame.driftSprite->velocityZ;
        (frame.driftSprite->frameCounter)++;
        if (frame.driftSprite->frameCounter == 0xC) {
            frame.driftSprite->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws two expanding, fading shell models around the task's position.
 */
// FUNCTION: FF7 0x0050fa5b
void __cdecl BattleFx_SpecialUltimateBlastExpandingShellsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[88];
        struct { unsigned char pad_to_4[0x4]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_24[0x24]; BattleFxTailLaserTargetTimelineSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_28[0x28]; short spawnIndex; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxOilTargetModelSlot *shellSlot; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_50[0x50]; short spreadRadius; };
        struct { unsigned char pad_to_54[0x54]; short angle; };
    } frame;
#pragma pack(pop)

    frame.shellSlot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecUltimateBlastSpriteRequest3.modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecUltimateBlastLoadedResTable);
    (g_BattleSpecUltimateBlastBuffer.rotX) = 0;
    g_BattleSpecUltimateBlastBuffer.scaledShort = frame.shellSlot->frameCounter << 6;
    g_BattleSpecUltimateBlastActiveCount = 0;
    RotMatrix((SVECTOR *)&g_BattleSpecUltimateBlastBuffer, &frame.rotMatrix);
    g_BattleSpecUltimateBlastScaleZ = frame.shellSlot->frameCounter * 0x7D0;
    g_BattleSpecUltimateBlastScaleY = g_BattleSpecUltimateBlastScaleZ;
    g_BattleSpecUltimateBlastScaleX = g_BattleSpecUltimateBlastScaleY;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecUltimateBlastScaleX);
    frame.rotMatrix.t[0] = frame.shellSlot->posX;
    frame.rotMatrix.t[1] = frame.shellSlot->posY;
    frame.rotMatrix.t[2] = frame.shellSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecUltimateBlastSpriteRequest3.fadeLevel = 0x80 - frame.shellSlot->frameCounter * 6;
    g_BattleSpecUltimateBlastSpriteRequest3.modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecUltimateBlastLoadedResTable);
    g_BattleSpecUltimateBlastDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecUltimateBlastSpriteRequest3.modelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecUltimateBlastDrawListHandle);
    RotMatrixXYZ((short *)&g_BattleSpecUltimateBlastBuffer, &frame.rotMatrix);
    g_BattleSpecUltimateBlastScaleZ = frame.shellSlot->frameCounter * 0x320;
    g_BattleSpecUltimateBlastScaleY = g_BattleSpecUltimateBlastScaleZ;
    g_BattleSpecUltimateBlastScaleX = g_BattleSpecUltimateBlastScaleY;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecUltimateBlastScaleX);
    frame.rotMatrix.t[0] = frame.shellSlot->posX;
    frame.rotMatrix.t[1] = 0;
    frame.rotMatrix.t[2] = frame.shellSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecUltimateBlastSpriteRequest3.fadeLevel = 0x80 - frame.shellSlot->frameCounter * 6;
    g_BattleSpecUltimateBlastSpriteRequest3.modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleSpecUltimateBlastLoadedResTable);
    g_BattleSpecUltimateBlastDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecUltimateBlastSpriteRequest3.modelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecUltimateBlastDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spreadRadius = frame.shellSlot->frameCounter * 0xF0;
    frame.spawnSlot = (BattleFxTailLaserTargetTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialUltimateBlastRisingFadeSpriteTask);
    for (frame.spawnIndex = 0; frame.spawnIndex < 2; frame.spawnIndex++) {
        frame.angle = (rand() % 4096);
        frame.spawnSlot->posX = CosFixed(frame.angle) * frame.spreadRadius >> 0xC;
        frame.spawnSlot->posZ = SinFixed(frame.angle) * frame.spreadRadius >> 0xC;
        frame.spawnSlot->posY = 0;
        frame.spawnSlot->variant = (rand() % 16);
    }
    if ((frame.shellSlot->frameCounter % 2) == 0) {
        frame.spawnSlot = (BattleFxTailLaserTargetTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialUltimateBlastRisingSpinModelTask);
        frame.angle = (rand() % 4096);
        frame.spawnSlot->posX = CosFixed(frame.angle) * frame.spreadRadius >> 0xC;
        frame.spawnSlot->posZ = SinFixed(frame.angle) * frame.spreadRadius >> 0xC;
        frame.spawnSlot->posY = 0;
        frame.spawnSlot->variant = (rand() % 4);
        frame.spawnSlot->angle = (rand() % 4096);
    }
    for (frame.spawnIndex = 0; frame.spawnIndex < 3; frame.spawnIndex++) {
        frame.spawnSlot = (BattleFxTailLaserTargetTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialUltimateBlastAnimatedSpriteTask);
        frame.angle = (rand() % 4096);
        frame.spawnSlot->posX = CosFixed(frame.angle) * frame.spreadRadius >> 0xC;
        frame.spawnSlot->posZ = SinFixed(frame.angle) * frame.spreadRadius >> 0xC;
        frame.spawnSlot->posY = 0;
    }
    (frame.shellSlot->frameCounter)++;
    if (frame.shellSlot->frameCounter == 0x14) {
        frame.shellSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animated billboard sprite from resource 0x11 at scale 0x3000. Its frame counter is the sprite
 * frame; it retires when the counter reaches the frame count of the resource header.
 */
// FUNCTION: FF7 0x0050fee2
void __cdecl BattleFx_SpecialUltimateBlastAnimatedSpriteTask(void)
{
    BattleFxUltimateBlastAnimatedSpriteSlot *task;

    task = (BattleFxUltimateBlastAnimatedSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecUltimateBlastTransformData1.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0x11, g_BattleSpecUltimateBlastLoadedResTable);
    g_BattleSpecUltimateBlastTransformData1.drawObj = Gfx_ModelTableGetSpriteObject(0x11, g_BattleSpecUltimateBlastLoadedResTable);
    BattleFx_InstallBillboardMatrix(&task->posX, 0x3000, 0);
    g_BattleSpecUltimateBlastTransformData1.frame = task->frameCounter;
    g_BattleSpecUltimateBlastDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecUltimateBlastTransformData1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecUltimateBlastDrawListHandle);
    if (g_BattleIsPaused == 0 && g_BattleSpecUltimateBlastTransformData1.spriteGroup != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= g_BattleSpecUltimateBlastTransformData1.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Moves a spinning model upward and submits it, retiring it after fifteen frames.
 */
// FUNCTION: FF7 0x0050ffb3
void __cdecl BattleFx_SpecialUltimateBlastRisingSpinModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        SVECTOR angleX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleFxRadiantBreathRisingSpinModelSlot *spinSlot; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.spinSlot = (BattleFxRadiantBreathRisingSpinModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.angleX.vy = 0;
    frame.angleX.vx = frame.angleX.vy;
    frame.angleX.vz = frame.spinSlot->spinAngle;
    RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
    g_BattleSpecUltimateBlastSpinScaleZ = 0x1000;
    g_BattleSpecUltimateBlastSpinScaleY = g_BattleSpecUltimateBlastSpinScaleZ;
    g_BattleSpecUltimateBlastSpinScaleX = g_BattleSpecUltimateBlastSpinScaleY;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecUltimateBlastSpinScaleX);
    frame.rotMatrix.t[0] = frame.spinSlot->posX;
    frame.rotMatrix.t[1] = frame.spinSlot->posY;
    frame.rotMatrix.t[2] = frame.spinSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecUltimateBlastTransformData1.fadeLevel = 0x80 - (frame.spinSlot->frameCounter << 3);
    g_BattleSpecUltimateBlastTransformData1.modelGroup = Gfx_ModelTableGetRenderObject(g_BattleSpecUltimateBlastAnimData5[frame.spinSlot->animIndex], g_BattleSpecUltimateBlastLoadedResTable);
    g_BattleSpecUltimateBlastDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecUltimateBlastTransformData1.modelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecUltimateBlastDrawListHandle);
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
 * Rising, fading billboard sprite from resource 0x10 at scale 0x2000. Its brightness is 0x80 minus
 * eight per frame; while unpaused it moves up by 0x12C per frame and retires at frame 0xF.
 */
// FUNCTION: FF7 0x0051012a
void __cdecl BattleFx_SpecialUltimateBlastRisingFadeSpriteTask(void)
{
    BattleFxHeatWaveRisingFadeSprite *task;

    task = (BattleFxHeatWaveRisingFadeSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecUltimateBlastAnimData6.spriteField = Gfx_ModelTableGetLoadedRecord(0x10, g_BattleSpecUltimateBlastLoadedResTable);
    g_BattleSpecUltimateBlastAnimData6.spriteAlt = Gfx_ModelTableGetSpriteObject(0x10, g_BattleSpecUltimateBlastLoadedResTable);
    BattleFx_InstallBillboardMatrix(&task->posX, 0x2000, 0);
    g_BattleSpecUltimateBlastSpriteRequest3.frameIndex = task->variant;
    g_BattleSpecUltimateBlastSpriteRequest3.blue = 0x80 - (task->fadeStep << 3);
    g_BattleSpecUltimateBlastSpriteRequest3.green = g_BattleSpecUltimateBlastSpriteRequest3.blue;
    g_BattleSpecUltimateBlastSpriteRequest3.red = g_BattleSpecUltimateBlastSpriteRequest3.green;
    g_BattleSpecUltimateBlastDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecUltimateBlastAnimData6.spriteField, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecUltimateBlastDrawListHandle);
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
