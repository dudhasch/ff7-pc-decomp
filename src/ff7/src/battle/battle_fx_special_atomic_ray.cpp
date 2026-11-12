// src/ff7/src/battle/battle_fx_special_atomic_ray.cpp
/* Enemy Special Effect [idx 40]: AtomicRay (JP: atmic / アトミックレイ (Atomic Ray), asset: "SPECIAL/ATMIC") — Atomic Ray (0x2ff, Wolfmeister) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Atmic attack: it loads resources and spawns a per-target
//   timeline that drives a family of sprite and model tasks, including variant, stretching,
//   orbiting, flickering, frame-table, layered, growing and scaling models.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "lib/types/graphics.h"
#include "ff7/camera_script.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;

// clang-format off
// GLOBAL: FF7 0x008a5fd0
unsigned int g_BattleSpecialAtomicRayResourceDefs[130] = {
    0x001900b1, (unsigned int)"ZIMEN.RSD", 0, 0, 0, 0x001900b1, (unsigned int)"kueisa.RSD", 0, 0, 0, 0x001900b1, (unsigned int)"soto1.RSD", 0, 0, 0, 0x001900b1,
    (unsigned int)"SOTO2.RSD", 0, 0, 0, 0x001900b1, (unsigned int)"SOTO3.RSD", 0, 0, 0, 0x001900b1, (unsigned int)"SOTO4.RSD", 0, 0, 0, 0x001900b1, (unsigned int)"SOTO5.RSD",
    0, 0, 0, 0x001900b1, (unsigned int)"DOMU.RSD", 0, 0, 0, 0x001900b1, (unsigned int)"DOSOTO.RSD", 0, 0, 0, 0x001900b1, (unsigned int)"SOKO.RSD", 0,
    0, 0, 0x001900b1, (unsigned int)"UCHIA.RSD", 0, 0, 0, 0x001900b1, (unsigned int)"UCHIB.RSD", 0, 0, 0, 0x001900b1, (unsigned int)"KEMU.RSD", 0, 0,
    0, 0x00192a39, (unsigned int)"ATOMIC00.TIM", (unsigned int)"A.SPT", 0, 0, 0x00192a39, (unsigned int)"ATOMIC03.TIM", (unsigned int)"ELEC1.SPT", 0, 0, 0x00192a39, (unsigned int)"ATOMIC03.TIM", (unsigned int)"ELEC2.SPT", 0, 0,
    0x00192a39, (unsigned int)"ATOMIC03.TIM", (unsigned int)"HIKARI11.SPT", 0, 0, 0x00192a39, (unsigned int)"ATOMIC03.TIM", (unsigned int)"HIKARI12.SPT", 0, 0, 0x00192a39, (unsigned int)"ATOMIC03.TIM", (unsigned int)"HIKARI2.SPT", 0, 0, 0x00192a39,
    (unsigned int)"ATOMIC00.TIM", (unsigned int)"ELECBI.SPT", 0, 0, 0x00192a39, (unsigned int)"ATOMIC03.TIM", (unsigned int)"MARU.SPT", 0, 0, 0x00192a39, (unsigned int)"ATOMIC01.TIM", (unsigned int)"HIT1.SPT", 0, 0, 0x00192a39, (unsigned int)"ATOMIC01.TIM",
    (unsigned int)"HIT2.SPT", 0, 0, 0x00192a39, (unsigned int)"ATOMIC01.TIM", (unsigned int)"HIT3.SPT", 0, 0, 0x00192a39, (unsigned int)"ATOMIC01.TIM", (unsigned int)"HIT4.SPT", 0, 0, 0x00192a39, (unsigned int)"ATOMIC02.TIM", (unsigned int)"BAKU1.SPT",
    0, 0
};
// clang-format off
// The 16-byte sprite draw record the code fills and submits by number.
// GLOBAL: FF7 0x008a61d8
BattleFxSpriteRequest g_BattleSpecialAtomicRayColor1 = {0, 0, -128, -128, -128, 0x2c, 0, 0};
// GLOBAL: FF7 0x008a61e8
struct BattleSpecialAtomicRayColor2 g_BattleSpecialAtomicRayColor2 = {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}, 0, {0}};
unsigned char g_rawData_008a61fc[4] = {0x01, 0x00, 0x01, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008a6200
struct BattleSpecialAtomicRayMotionTable g_BattleSpecialAtomicRayMotionTable = {{2, 0, 2, 0, 3, 0, 3, 0, 4, 0, 4, 0, 5, 0, 5, 0, 0, 0, 0, 0, 1, 0, 1, 0, 2, 0, 2, 0, 3, 0, 3, 0, 4, 0, 4, 0, 5, 0, 5, 0, 0, 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 0, 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 0, 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 0, 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0}, 0, 0};
// clang-format off
// GLOBAL: FF7 0x008a6260
struct BattleSpecialAtomicRayColor3 g_BattleSpecialAtomicRayColor3 = {-128, -128, -128, {0x2c, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
// GLOBAL: FF7 0x008a6270
struct BattleSpecialAtomicRayAnimConfig g_BattleSpecialAtomicRayAnimConfig = {{0x80, 0x80, 0x80, 0x2c, 0, 0, 0, 0}, 0, 8, {0}, 0, {0x20, 0, 0, 0}, 0, 0};
// GLOBAL: FF7 0x008a6290
struct BattleFxAtomicRayColorRecord g_BattleSpecialAtomicRayColor4 = {{0x80, 0x80, 0x80, 0x2c, 0, 0, 0, 0}, 0, 0};
// GLOBAL: FF7 0x008a62a0
unsigned char g_BattleSpecialAtomicRayColor5[8] = {0x80, 0x80, 0x80, 0x2c, 0x00, 0x00, 0x00, 0x00};
// clang-format on
// GLOBAL: FF7 0x008a62a8
short g_BattleSpecialAtomicRayFrameIndices[28] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
// clang-format off
// GLOBAL: FF7 0x008a62e0
struct BattleSpecialAtomicRayScale g_BattleSpecialAtomicRayScale = {0, {0x28, 0, 0, 0, 0, 0}, 128};
unsigned char g_rawData_008a62ec[4] = {0x20, 0x00, 0x00, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008a62f0
unsigned char g_BattleSpecialAtomicRayScaleConst[16] = {0x00, 0x2c, 0x00, 0x00, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008a6300
struct BattleSpecialAtomicRayModelIndices g_BattleSpecialAtomicRayModelIndices = {{0, 0x12, 0, 0, 0, 0x12, 0, 0, 0, 0x12, 0, 0, 0, 0, 0, 0}, 0, {8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}, 0, {0x88, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}, 0, {1, 0}};
unsigned char g_rawData_008a6334[4] = {0x01, 0x00, 0x01, 0x00};
unsigned char g_rawData_008a6338[64] = {
    0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x01, 0x00, 0x01, 0x00, 0x02, 0x00,
    0x02, 0x00, 0x03, 0x00, 0x03, 0x00, 0x04, 0x00, 0x04, 0x00, 0x01, 0x00, 0x01, 0x00, 0x02, 0x00, 0x02, 0x00, 0x03, 0x00, 0x03, 0x00, 0x04, 0x00,
    0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008a6378
struct BattleSpecialAtomicRayModelReq g_BattleSpecialAtomicRayModelReq = {0, {0x88, 0, 0, 0, 0, 0}, 0};
unsigned char g_rawData_008a6384[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00b6e8d8
int g_BattleSpecialAtomicRayScaleX = 0;
unsigned char g_bssPad_00b6e8dc[4] = {0};
// GLOBAL: FF7 0x00b6e8e0
int g_BattleSpecialAtomicRayScaleY = 0;
// GLOBAL: FF7 0x00b6e8e4
int g_BattleSpecialAtomicRayScaleZ = 0;
// GLOBAL: FF7 0x00b6e8e8
int g_BattleSpecialAtomicRayParamVec = 0;
unsigned char g_bssPad_00b6e8ec[4] = {0};
// GLOBAL: FF7 0x00b6e8f0
int g_BattleSpecialAtomicRayScaleFactor = 0;
// GLOBAL: FF7 0x00b6e8f4
int g_BattleSpecialAtomicRayIntensity = 0;
// GLOBAL: FF7 0x00b6e8f8
struct BattleSpecialAtomicRayBuffer g_BattleSpecialAtomicRayBuffer = {0};
unsigned char g_bssPad_00b6e920[4] = {0};
// GLOBAL: FF7 0x00b6e924
int g_BattleSpecialAtomicRayDrawListHead = 0;
// GLOBAL: FF7 0x00b6e928
struct BattleFxAtomicRayOrigin *g_BattleSpecialAtomicRayState1 = 0;
unsigned char g_bssPad_00b6e92c[4] = {0};
// GLOBAL: FF7 0x00b6e930
int g_BattleSpecialAtomicRayState2 = 0;
// GLOBAL: FF7 0x00b6e934
int g_BattleSpecialAtomicRayState3 = 0;
// GLOBAL: FF7 0x00b6e938
int g_BattleSpecialAtomicRayState4 = 0;
unsigned char g_bssPad_00b6e93c[4] = {0};
// GLOBAL: FF7 0x00b6e940
struct GfxTableCursor *g_BattleSpecialAtomicRayLoadedRes = 0;
// GLOBAL: FF7 0x00b6e944
short g_BattleSpecialAtomicRayStepCount = 0;
// GLOBAL: FF7 0x00b6e948
short g_BattleSpecialAtomicRayModelIndex = 0;
// GLOBAL: FF7 0x00b6e94c
short g_BattleSpecialAtomicRayAngle = 0;
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/*
 * Loads resources for the Atmic special effect when not already loaded.
 */
// FUNCTION: FF7 0x0054b150
void __cdecl BattleFx_SpecialLoadResAtomicRay(void)
{
    GfxModelTableBank *stage;
    GfxResourceRequest requestSlot;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceTable;

    if (g_pBattle3dScene != 0 && g_BattleSpecialAtomicRayLoadedRes == 0) {
        kind = 0x1A;
        resourceTable = (int)&g_BattleSpecialAtomicRayResourceDefs;
        stage = (GfxModelTableBank *)g_pBattle3dScene->modelTableBank;
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        ((BattleFxDiamondCardResource *)pRequest)->groupTag = 0;
        // STRING: FF7 0x008a6594
        ((BattleFxDiamondCardResource *)pRequest)->resourcePath = (int)BattleFx_BuildResourcePath("SPECIAL/ATMIC", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceTable, pRequest, &g_BattleSpecialAtomicRayLoadedRes, stage);
    }
}

/*
 * Entry point for the Atmic special effect, dispatching to begin.
 */
// FUNCTION: FF7 0x0054b1e0
void __cdecl BattleFx_SpecialEnterAtomicRay(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResAtomicRay();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnAtomicRayPerTarget);
    Battle_PlaySfxPannedToActor(0xCF, (short)actorIdx, 0);
}

/*
 * Spawns Atmic effect instances for each targeted actor.
 */
// FUNCTION: FF7 0x0054b215
void __cdecl BattleFx_SpecialSpawnAtomicRayPerTarget(int actor, int sourceIndex)
{
    BattleFxAtomicRayTimelineSlot *slotPtr;

    slotPtr = (BattleFxAtomicRayTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialAtomicRayTimelineTask);
    Battle_GetBoneWorldPos(sourceIndex, g_BattleActorRecords[sourceIndex].targetBone, &slotPtr->sourceX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &slotPtr->posX);
    slotPtr->actorIndex = actor;
}

/*
 * Timeline task controlling state progression for the Atmic special effect.
 */
// FUNCTION: FF7 0x0054b28b
void __cdecl BattleFx_SpecialAtomicRayTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[40];
        int screenPos;
        struct { unsigned char pad_to_4[0x4]; SVECTOR dirX; };
        struct { unsigned char pad_to_C[0xC]; BattleFxAtomicRayTimelineSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_10[0x10]; short spawnIndex; };
        struct { unsigned char pad_to_14[0x14]; short distance; };
        struct { unsigned char pad_to_18[0x18]; BattleFxAtomicRayTimelineSlot *timelineSlot; };
        struct { unsigned char pad_to_1C[0x1C]; short deltaX; };
        struct { unsigned char pad_to_1E[0x1E]; short deltaY; };
        struct { unsigned char pad_to_20[0x20]; short deltaZ; };
        struct { unsigned char pad_to_24[0x24]; BattleFxAtomicRayTimelineSpawnSlot *tmpSlot; };
    } frame;
#pragma pack(pop)

    frame.timelineSlot = (BattleFxAtomicRayTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timelineSlot->frameCounter == 0) {
        frame.spawnSlot = (BattleFxAtomicRayTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialAtomicRayScaleUpModelTask);
        frame.spawnSlot->posX = 0;
        frame.spawnSlot->posY = 0;
        frame.spawnSlot->posZ = 0;
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        Battle_StartFixedPointRamp(0x1000, 20);
    }
    if (frame.timelineSlot->frameCounter == 20) {
        frame.spawnSlot = (BattleFxAtomicRayTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialAtomicRayLayeredModelsTask);
        g_BattleSpecialAtomicRayState1 = (BattleFxAtomicRayOrigin *)frame.spawnSlot;
        frame.spawnSlot->posX = 0;
        frame.spawnSlot->posY = 0xFE70;
        frame.spawnSlot->posZ = 0;
        frame.spawnSlot->velocityY = 0xFF74;
    }
    if (frame.timelineSlot->frameCounter >= 10 && frame.timelineSlot->frameCounter < 30 && frame.timelineSlot->frameCounter % 5 == 0) {
        frame.spawnSlot = (BattleFxAtomicRayTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialAtomicRaySpinFadeModelTask);
        frame.spawnSlot->posX = 0;
        frame.spawnSlot->posY = 0;
        frame.spawnSlot->posZ = 0;
    }
    if (frame.timelineSlot->frameCounter >= 30 && frame.timelineSlot->frameCounter < 38) {
        frame.spawnSlot = (BattleFxAtomicRayTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialAtomicRayFrameTableSpriteTask);
        frame.spawnSlot->posX = rand() % 4000 - 2000;
        frame.spawnSlot->posY = 0;
        frame.spawnSlot->posZ = rand() % 4000 - 2000;
        frame.spawnSlot->variantIndex = (rand() % 2);
        frame.tmpSlot = (BattleFxAtomicRayTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialAtomicRayDrawGrowingModelsTask);
        frame.tmpSlot->posX = frame.spawnSlot->posX;
        frame.tmpSlot->posY = 0;
        frame.tmpSlot->posZ = frame.spawnSlot->posZ;
    }
    if (frame.timelineSlot->frameCounter == 30) {
        frame.spawnSlot = (BattleFxAtomicRayTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialAtomicRayFlickerSpriteTask);
        frame.dirX.vx = frame.timelineSlot->posX - g_BattleSpecialAtomicRayState1->savedX;
        frame.dirX.vy = frame.timelineSlot->posY - g_BattleSpecialAtomicRayState1->savedY;
        frame.dirX.vz = frame.timelineSlot->posZ - g_BattleSpecialAtomicRayState1->savedZ;
        Gte_BuildLookBasisFromDirection(&frame.dirX.vx, (short *)&g_BattleSpecialAtomicRayBuffer.matrix);
        g_BattleSpecialAtomicRayBuffer.matrix.t[0] = g_BattleSpecialAtomicRayState1->savedX;
        g_BattleSpecialAtomicRayBuffer.matrix.t[1] = g_BattleSpecialAtomicRayState1->savedY;
        g_BattleSpecialAtomicRayBuffer.matrix.t[2] = g_BattleSpecialAtomicRayState1->savedZ;
        SetRotMatrix(&g_BattleSpecialAtomicRayBuffer.matrix);
        SetTransMatrix(&g_BattleSpecialAtomicRayBuffer.matrix);
        frame.dirX.vx = 0;
        frame.dirX.vy = 0;
        frame.dirX.vz = 600;
        RotTransPointToInt16(&frame.dirX, &frame.spawnSlot->posX, &frame.screenPos);
        frame.spawnSlot->variantIndex = 0;
        frame.tmpSlot = (BattleFxAtomicRayTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialAtomicRayStretchSpriteTask);
        frame.deltaX = frame.timelineSlot->posX - frame.spawnSlot->posX;
        frame.deltaY = frame.timelineSlot->posY - frame.spawnSlot->posY;
        frame.deltaZ = frame.timelineSlot->posZ - frame.spawnSlot->posZ;
        frame.distance = SqrtToInt(frame.deltaX * frame.deltaX + frame.deltaY * frame.deltaY + frame.deltaZ * frame.deltaZ);
        frame.tmpSlot->scale = (frame.distance << 12) / 900;
        frame.tmpSlot->posX = frame.spawnSlot->posX;
        frame.tmpSlot->posY = frame.spawnSlot->posY;
        frame.tmpSlot->posZ = frame.spawnSlot->posZ;
        frame.tmpSlot->velocityX = 0;
        frame.tmpSlot->velocityY = 0xFC00;
        frame.tmpSlot->velocityZ = 1024;
        frame.spawnSlot = (BattleFxAtomicRayTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialAtomicRayFlickerSpriteTask);
        frame.spawnSlot->posX = frame.timelineSlot->posX;
        frame.spawnSlot->posY = frame.timelineSlot->posY;
        frame.spawnSlot->posZ = frame.timelineSlot->posZ;
        frame.spawnSlot->variantIndex = 1;
    }
    if (frame.timelineSlot->frameCounter == 50) {
        for (frame.spawnIndex = 0; frame.spawnIndex < 6; frame.spawnIndex++) {
            frame.spawnSlot = (BattleFxAtomicRayTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialAtomicRayOrbitSpriteTask);
            frame.spawnSlot->posX = frame.timelineSlot->posX;
            frame.spawnSlot->posY = frame.timelineSlot->posY;
            frame.spawnSlot->posZ = frame.timelineSlot->posZ;
            frame.spawnSlot->velocityY = 0;
            frame.spawnSlot->velocityX = 0;
            frame.spawnSlot->velocityZ = frame.spawnIndex * 170 - 512;
            frame.spawnSlot->angle = (rand() % 4096);
            frame.spawnSlot->scale = ((rand() % 4) << 9) + 0x1800;
        }
    }
    if (frame.timelineSlot->frameCounter > 50 && frame.timelineSlot->frameCounter < 80) {
        frame.spawnSlot = (BattleFxAtomicRayTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialAtomicRayVariantSpriteTask);
        Battle_GetBoneWorldPos(frame.timelineSlot->actorIndex, g_BattleActorRecords[frame.timelineSlot->actorIndex].targetBone, &frame.spawnSlot->posX);
        frame.spawnSlot->posX += (short)(rand() % 1000 - 500);
        frame.spawnSlot->posY += (short)(rand() % 1000 - 500);
        frame.spawnSlot->posZ += (short)(rand() % 1000 - 500);
        frame.spawnSlot->variantIndex = (rand() % 4);
    }
    if (frame.timelineSlot->frameCounter > 86 && frame.timelineSlot->frameCounter < 94) {
        frame.spawnSlot = (BattleFxAtomicRayTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialAtomicRayVariantSpriteTask);
        Battle_GetBoneWorldPos(frame.timelineSlot->actorIndex, g_BattleActorRecords[frame.timelineSlot->actorIndex].targetBone, &frame.spawnSlot->posX);
        frame.spawnSlot->posX += (short)(rand() % 1000 - 500);
        frame.spawnSlot->posY += (short)(rand() % 1000 - 500);
        frame.spawnSlot->posZ += (short)(rand() % 1000 - 500);
        frame.spawnSlot->variantIndex = 4;
    }
    if (frame.timelineSlot->frameCounter == 90) {
        Battle_PostActorHitOrHurtMessage(frame.timelineSlot->actorIndex);
        Battle_PlaySfxPannedToActor(0xFA, (short)frame.timelineSlot->actorIndex, 0);
        Battle_StartFixedPointRamp(0, 20);
    }
    (frame.timelineSlot->frameCounter)++;
    if (frame.timelineSlot->frameCounter == 100) {
        frame.timelineSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task to animate and render variant sprites for the Atmic effect.
 */
// FUNCTION: FF7 0x0054b9e3
void __cdecl BattleFx_SpecialAtomicRayVariantSpriteTask(void)
{
    struct {
        int table[5];
        BattleFxAtomicRaySpriteState *slot;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.slot = (BattleFxAtomicRaySpriteState *)BATTLE_CURRENT_TASK_SLOT;
    local.table[0] = 0x15;
    local.table[1] = 0x16;
    local.table[2] = 0x17;
    local.table[3] = 0x18;
    local.table[4] = 0x19;
    BattleFx_InstallBillboardMatrix(
        &local.slot->posX,
        local.slot->variant == 4 ? 0x4000 : 0x2000,
        0);
    g_BattleSpecialAtomicRayColor1.frame = local.slot->frameCounter;
    g_BattleSpecialAtomicRayColor1.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(
        local.table[local.slot->variant], g_BattleSpecialAtomicRayLoadedRes);
    g_BattleSpecialAtomicRayColor1.drawObj = Gfx_ModelTableGetSpriteObject(
        local.table[local.slot->variant], g_BattleSpecialAtomicRayLoadedRes);
    g_BattleSpecialAtomicRayDrawListHead = BattleFx_SubmitSpriteDraw(
        &g_BattleSpecialAtomicRayColor1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC,
        g_BattleSpecialAtomicRayDrawListHead);
    if (g_BattleIsPaused == 0 && g_BattleSpecialAtomicRayColor1.spriteGroup != 0) {
        (local.slot->frameCounter)++;
        if (local.slot->frameCounter ==
            *(int *)(*(int *)g_BattleSpecialAtomicRayColor1.spriteGroup + 4)) {
            local.slot->endMarker = 0xFFFF;
        }
    }
}

/*
 * Task to animate stretching sprites for the Atmic special effect.
 */
// FUNCTION: FF7 0x0054bb02
void __cdecl BattleFx_SpecialAtomicRayStretchSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix localMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxAtomicRayStretchSlot *stretchSlot; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_44[0x44]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.stretchSlot = (BattleFxAtomicRayStretchSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecialAtomicRayColor2.spriteField) = Gfx_ModelTableGetLoadedRecord(19, g_BattleSpecialAtomicRayLoadedRes);
    g_BattleSpecialAtomicRayColor2.drawObj = Gfx_ModelTableGetSpriteObject(19, g_BattleSpecialAtomicRayLoadedRes);
    RotMatrixXYZ((SVECTOR *)&frame.stretchSlot->rotX, &frame.localMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialAtomicRayBuffer.matrix, &frame.localMatrix, &frame.localMatrix);
    frame.scaleX.vy = frame.stretchSlot->lengthScale;
    frame.scaleX.vz = 0x2000 /* 2.0 */;
    frame.scaleX.vx = frame.scaleX.vz;
    ScaleMatrix(&frame.localMatrix, &frame.scaleX);
    frame.localMatrix.t[0] = frame.stretchSlot->posX;
    frame.localMatrix.t[1] = frame.stretchSlot->posY;
    frame.localMatrix.t[2] = frame.stretchSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.localMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    g_BattleSpecialAtomicRayColor2.frame = (&g_BattleSpecialAtomicRayColor2.frameTable)[frame.stretchSlot->frameIndex];
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecialAtomicRayColor2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (frame.stretchSlot->frameIndex)++;
        if (frame.stretchSlot->frameIndex == 48) {
            frame.stretchSlot->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Task to animate orbiting sprites for the Atmic special effect.
 */
// FUNCTION: FF7 0x0054bc4f
void __cdecl BattleFx_SpecialAtomicRayOrbitSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[56];
        int sizeRaw;
        struct { unsigned char pad_to_4[0x4]; int screenPos; };
        struct { unsigned char pad_to_8[0x8]; SVECTOR orbitX; };
        struct { unsigned char pad_to_10[0x10]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_30[0x30]; short spriteSize; };
        struct { unsigned char pad_to_34[0x34]; BattleFxAtomicRayOrbitSpriteSlot *orbitSlot; };
    } frame;
#pragma pack(pop)

    frame.orbitSlot = (BattleFxAtomicRayOrbitSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialAtomicRayMotionTable.spriteRecord = Gfx_ModelTableGetLoadedRecord(20, g_BattleSpecialAtomicRayLoadedRes);
    g_BattleSpecialAtomicRayMotionTable.spriteObject = Gfx_ModelTableGetSpriteObject(20, g_BattleSpecialAtomicRayLoadedRes);
    if (frame.orbitSlot->frameCounter < 22) {
        frame.sizeRaw = 8;
    } else {
        frame.sizeRaw = 30 - frame.orbitSlot->frameCounter;
    }
    frame.spriteSize = *(short *)&frame.sizeRaw;
    frame.orbitX.vx = CosFixed(frame.orbitSlot->angle) * 0x320 >> 12;
    frame.orbitX.vz = SinFixed(frame.orbitSlot->angle) * 0x320 >> 12;
    frame.orbitX.vy = 0;
    RotMatrix((SVECTOR *)&frame.orbitSlot->rotX, &frame.drawMatrix);
    frame.drawMatrix.t[0] = frame.orbitSlot->centerX;
    frame.drawMatrix.t[1] = frame.orbitSlot->centerY;
    frame.drawMatrix.t[2] = frame.orbitSlot->centerZ;
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    RotTransPointToInt16(&frame.orbitX, (short *)&frame.orbitX, &frame.screenPos);
    BattleFx_InstallBillboardMatrix(&frame.orbitX, (short)frame.orbitSlot->scale, 0);
    g_BattleSpecialAtomicRayColor3.blue = frame.spriteSize << 4;
    g_BattleSpecialAtomicRayColor3.green = g_BattleSpecialAtomicRayColor3.blue;
    (g_BattleSpecialAtomicRayColor3.red) = g_BattleSpecialAtomicRayColor3.green;
    g_BattleSpecialAtomicRayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecialAtomicRayMotionTable.spriteRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialAtomicRayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.orbitSlot->angle += 0x100 /* 22.5 deg */;
    (frame.orbitSlot->frameCounter)++;
    if (frame.orbitSlot->frameCounter == 30) {
        frame.orbitSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task to animate flickering sprites for the Atmic special effect.
 */
// FUNCTION: FF7 0x0054be05
void __cdecl BattleFx_SpecialAtomicRayFlickerSpriteTask(void)
{
    BattleFxTailLaserAnimSprite *p;
    int tbl[2];

    p = (BattleFxTailLaserAnimSprite *)BATTLE_CURRENT_TASK_SLOT;
    tbl[0] = 0x11;
    tbl[1] = 0x12;
    BattleFx_InstallBillboardMatrix(&p->posX, (p->variant != 0 ? 0x2000 : 0x1000) + (p->frame % 2 << 10), -800);
    ((BattleFxAtomicRayColorRecord *)&g_BattleSpecialAtomicRayColor3)->spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(tbl[p->variant], g_BattleSpecialAtomicRayLoadedRes);
    ((BattleFxAtomicRayColorRecord *)&g_BattleSpecialAtomicRayColor3)->drawObject = Gfx_ModelTableGetSpriteObject(tbl[p->variant], g_BattleSpecialAtomicRayLoadedRes);
    g_BattleSpecialAtomicRayDrawListHead = BattleFx_SubmitSpriteDraw((char *)&g_BattleSpecialAtomicRayColor3 + 8, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialAtomicRayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (p->frame)++;
        if (p->frame == 48) {
            p->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Task to render growing models for the Atmic special effect.
 */
// FUNCTION: FF7 0x0054bf09
void __cdecl BattleFx_SpecialAtomicRayDrawGrowingModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[80];
        int growScale;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix localMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxOilTargetModelSlot *growSlot; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix drawMatrix; };
    } frame;
#pragma pack(pop)

    frame.growSlot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialAtomicRayAnimConfig.spriteRecord = Gfx_ModelTableGetLoadedRecord(18, g_BattleSpecialAtomicRayLoadedRes);
    g_BattleSpecialAtomicRayAnimConfig.spriteObject = Gfx_ModelTableGetSpriteObject(18, g_BattleSpecialAtomicRayLoadedRes);
    if (frame.growSlot->frameCounter < 20) {
        frame.growScale = 8;
    } else {
        frame.growScale = 0x1C - frame.growSlot->frameCounter;
    }
    g_BattleSpecialAtomicRayScaleX = frame.growScale;
    frame.rotX.vz = 0;
    frame.rotX.vx = frame.rotX.vz;
    frame.rotX.vy = frame.growSlot->frameCounter << 6;
    RotMatrix(&frame.rotX, &frame.localMatrix);
    g_BattleSpecialAtomicRayState4 = frame.growSlot->frameCounter * 400;
    g_BattleSpecialAtomicRayState3 = g_BattleSpecialAtomicRayState4;
    g_BattleSpecialAtomicRayState2 = g_BattleSpecialAtomicRayState3;
    ScaleMatrix(&frame.localMatrix, &g_BattleSpecialAtomicRayState2);
    frame.localMatrix.t[0] = frame.growSlot->posX;
    frame.localMatrix.t[1] = frame.growSlot->posY;
    frame.localMatrix.t[2] = frame.growSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.localMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    g_BattleSpecialAtomicRayAnimConfig.flags = 8;
    g_BattleSpecialAtomicRayAnimConfig.scale = g_BattleSpecialAtomicRayScaleX << 4;
    g_BattleSpecialAtomicRayAnimConfig.modelDraw = Gfx_ModelTableGetRenderObject(7, g_BattleSpecialAtomicRayLoadedRes);
    g_BattleSpecialAtomicRayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialAtomicRayAnimConfig.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialAtomicRayDrawListHead);
    g_BattleSpecialAtomicRayAnimConfig.modelDraw = Gfx_ModelTableGetRenderObject(8, g_BattleSpecialAtomicRayLoadedRes);
    g_BattleSpecialAtomicRayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialAtomicRayAnimConfig.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialAtomicRayDrawListHead);
    g_BattleSpecialAtomicRayAnimConfig.flags = 0x88;
    g_BattleSpecialAtomicRayAnimConfig.scale = 0x1000 - (g_BattleSpecialAtomicRayScaleX << 9);
    g_BattleSpecialAtomicRayAnimConfig.modelDraw = Gfx_ModelTableGetRenderObject(9, g_BattleSpecialAtomicRayLoadedRes);
    g_BattleSpecialAtomicRayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialAtomicRayAnimConfig.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialAtomicRayDrawListHead);
    BattleFx_InstallBillboardMatrix(&frame.growSlot->posX, ((frame.growSlot->frameCounter % 2) << 10) + 0x1000, 0);
    g_BattleSpecialAtomicRayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecialAtomicRayAnimConfig.spriteRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialAtomicRayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.growSlot->frameCounter)++;
    if (frame.growSlot->frameCounter == 28) {
        frame.growSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task to animate sprites driven by frame tables for Atmic.
 */
// FUNCTION: FF7 0x0054c19f
void __cdecl BattleFx_SpecialAtomicRayFrameTableSpriteTask(void)
{
    BattleFxRadiantBreathFrameSprite *p;
    int tbl[2];

    p = (BattleFxRadiantBreathFrameSprite *)BATTLE_CURRENT_TASK_SLOT;
    tbl[0] = 0xE;
    tbl[1] = 0xF;
    BattleFx_InstallBillboardMatrix(&p->posX, 0x1800 /* 1.5 */, 0);
    *(short *)((char *)&g_BattleSpecialAtomicRayColor5 + 0x4) = g_BattleSpecialAtomicRayFrameIndices[p->frameIndex];
    g_BattleSpecialAtomicRayColor4.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(tbl[p->animIndex], g_BattleSpecialAtomicRayLoadedRes);
    g_BattleSpecialAtomicRayColor4.drawObject = Gfx_ModelTableGetSpriteObject(tbl[p->animIndex], g_BattleSpecialAtomicRayLoadedRes);
    g_BattleSpecialAtomicRayDrawListHead = BattleFx_SubmitSpriteDraw((char *)&g_BattleSpecialAtomicRayColor4 + 0x8, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialAtomicRayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (p->frameIndex)++;
        if (p->frameIndex == 28) {
            p->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Task to animate spinning and fading models for Atmic.
 */
// FUNCTION: FF7 0x0054c28a
void __cdecl BattleFx_SpecialAtomicRaySpinFadeModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        SVECTOR angleX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleFxOilTargetModelSlot *slot; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecialAtomicRayScale.modelField) = Gfx_ModelTableGetRenderObject(12, g_BattleSpecialAtomicRayLoadedRes);
    frame.angleX.vz = 0;
    frame.angleX.vx = frame.angleX.vz;
    frame.angleX.vy = frame.slot->frameCounter << 6;
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    (g_BattleSpecialAtomicRayBuffer.scaleFactor) = frame.slot->frameCounter * 0xFA + 0x800;
    g_BattleSpecialAtomicRayScaleFactor = (g_BattleSpecialAtomicRayBuffer.scaleFactor);
    g_BattleSpecialAtomicRayIntensity = 0x4000 - frame.slot->frameCounter * 800;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialAtomicRayScaleFactor);
    frame.rotMatrix.t[0] = frame.slot->posX;
    frame.rotMatrix.t[1] = frame.slot->posY;
    frame.rotMatrix.t[2] = frame.slot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecialAtomicRayScale.component10 = 0x80 - (frame.slot->frameCounter << 2);
    g_BattleSpecialAtomicRayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialAtomicRayScale, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialAtomicRayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.slot->frameCounter)++;
    if (frame.slot->frameCounter == 20) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task to render layered models for the Atmic special effect.
 */
// FUNCTION: FF7 0x0054c3f0
void __cdecl BattleFx_SpecialAtomicRayLayeredModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[100];
        int nextModelIndex;
        struct { unsigned char pad_to_4[0x4]; SVECTOR angleX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; int modelIdxA[5]; };
        struct { unsigned char pad_to_40[0x40]; BattleFxAtomicRayLayeredModelsSlot *slot; };
        struct { unsigned char pad_to_44[0x44]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxAtomicRayLayeredModelsSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.modelIdxA[0] = 2;
    frame.modelIdxA[1] = 3;
    frame.modelIdxA[2] = 4;
    frame.modelIdxA[3] = 5;
    frame.modelIdxA[4] = 6;
    frame.angleX.vz = 0;
    frame.angleX.vy = frame.angleX.vz;
    frame.angleX.vx = frame.angleX.vy;
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialAtomicRayScaleConst);
    frame.rotMatrix.t[0] = frame.slot->posX;
    frame.rotMatrix.t[1] = frame.slot->posY;
    frame.rotMatrix.t[2] = frame.slot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecialAtomicRayModelIndices.indexWord26 = 8 - g_BattleSpecialAtomicRayStepCount << 4;
    g_BattleSpecialAtomicRayModelIndices.firstModelRes = Gfx_ModelTableGetRenderObject(frame.modelIdxA[(&g_BattleSpecialAtomicRayModelIndices.indexWord48)[g_BattleSpecialAtomicRayModelIndex]], g_BattleSpecialAtomicRayLoadedRes);
    g_BattleSpecialAtomicRayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialAtomicRayModelIndices.firstModelRes, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialAtomicRayDrawListHead);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialAtomicRayModelIndices.indexWord42 = g_BattleSpecialAtomicRayStepCount << 9;
    g_BattleSpecialAtomicRayModelIndices.secondModelRes = Gfx_ModelTableGetRenderObject(10, g_BattleSpecialAtomicRayLoadedRes);
    g_BattleSpecialAtomicRayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialAtomicRayModelIndices.secondModelRes, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialAtomicRayDrawListHead);
    g_BattleSpecialAtomicRayModelIndices.secondModelRes = Gfx_ModelTableGetRenderObject(11, g_BattleSpecialAtomicRayLoadedRes);
    g_BattleSpecialAtomicRayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialAtomicRayModelIndices.secondModelRes, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialAtomicRayDrawListHead);
    frame.angleX.vx = 0;
    frame.angleX.vz = frame.slot->frameCounter << 6;
    frame.angleX.vy = frame.angleX.vz;
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, (int *)&g_BattleSpecialAtomicRayModelIndices);
    frame.rotMatrix.t[0] = frame.slot->posX;
    frame.rotMatrix.t[1] = frame.slot->posY;
    frame.rotMatrix.t[2] = frame.slot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecialAtomicRayModelIndices.secondModelRes = Gfx_ModelTableGetRenderObject(1, g_BattleSpecialAtomicRayLoadedRes);
    g_BattleSpecialAtomicRayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialAtomicRayModelIndices.secondModelRes, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialAtomicRayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.slot->velocityY < 0) {
        frame.slot->posY += frame.slot->velocityY;
        frame.slot->velocityY += 10;
    }
    if (frame.slot->velocityY == 0) {
        if (g_BattleSpecialAtomicRayModelIndex == 0x20) {
            frame.nextModelIndex = 1;
        } else {
            frame.nextModelIndex = g_BattleSpecialAtomicRayModelIndex + 1;
        }
        g_BattleSpecialAtomicRayModelIndex = *(short *)&frame.nextModelIndex;
    }
    if (frame.slot->frameCounter > 72) {
        g_BattleSpecialAtomicRayStepCount++;
    }
    frame.slot->stepCount = g_BattleSpecialAtomicRayStepCount;
    (frame.slot->frameCounter)++;
    if (frame.slot->frameCounter == 80) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task to render scaling models for the Atmic special effect.
 */
// FUNCTION: FF7 0x0054c735
void __cdecl BattleFx_SpecialAtomicRayScaleUpModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        int scaleAmount;
        struct { unsigned char pad_to_4[0x4]; short scaleM00; };
        struct { unsigned char pad_to_6[0x6]; short scaleM01; };
        struct { unsigned char pad_to_8[0x8]; short scaleM02; };
        struct { unsigned char pad_to_A[0xA]; short scaleM10; };
        struct { unsigned char pad_to_C[0xC]; short scaleM11; };
        struct { unsigned char pad_to_E[0xE]; short scaleM12; };
        struct { unsigned char pad_to_10[0x10]; short scaleM20; };
        struct { unsigned char pad_to_12[0x12]; short scaleM21; };
        struct { unsigned char pad_to_14[0x14]; short scaleM22; };
        struct { unsigned char pad_to_16[0x16]; int extraX; };
        struct { unsigned char pad_to_1A[0x1A]; int extraY; };
        struct { unsigned char pad_to_1E[0x1E]; int extraZ; };
        struct { unsigned char pad_to_24[0x24]; BattleFxOilTargetModelSlot *slot; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.scaleM00 = 0x1000 /* 1.0 */;
    frame.scaleM01 = 0;
    frame.scaleM02 = 0;
    frame.scaleM10 = 0;
    frame.scaleM11 = 0x1000 /* 1.0 */;
    frame.scaleM12 = 0;
    frame.scaleM20 = 0;
    frame.scaleM21 = 0;
    frame.scaleM22 = 0x1000 /* 1.0 */;
    memset(&frame.extraX, 0, 12);
    (g_BattleSpecialAtomicRayModelReq.modelField) = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialAtomicRayLoadedRes);
    if (frame.slot->frameCounter < 8) {
        frame.scaleAmount = frame.slot->frameCounter * 0x600;
    } else {
        frame.scaleAmount = 0x3000 /* 3.0 */;
    }
    g_BattleSpecialAtomicRayParamVec = frame.scaleAmount;
    g_BattleSpecialAtomicRayScaleZ = g_BattleSpecialAtomicRayParamVec;
    g_BattleSpecialAtomicRayScaleY = g_BattleSpecialAtomicRayScaleZ;
    ScaleMatrix(&frame.scaleM00, &g_BattleSpecialAtomicRayScaleY);
    frame.extraX = frame.slot->posX;
    frame.extraY = frame.slot->posY;
    frame.extraZ = frame.slot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.scaleM00, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialAtomicRayModelReq.angle = g_BattleSpecialAtomicRayAngle << 9;
    g_BattleSpecialAtomicRayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialAtomicRayModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialAtomicRayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.slot->frameCounter > 92) {
        g_BattleSpecialAtomicRayAngle++;
    }
    (frame.slot->frameCounter)++;
    if (frame.slot->frameCounter == 100) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
