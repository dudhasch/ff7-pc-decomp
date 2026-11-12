// src/ff7/src/battle/battle_fx_special_snowstorm.cpp
/* Enemy Special Effect [idx 59]: Snowstorm (JP: hubuki / 吹雪 (Blizzard/Snowstorm), asset: "SPECIAL/HUBUKI") — Snowstorm (0x2a4, Lessaloploth), Cold Breath (0x37b) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Hubuki attack: it loads resources and spawns a per-target
//   timeline that drives animated and fading sprites, a particle field that is updated and drawn,
//   and shrinking and expanding rising models.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "lib/codegen.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "ff7/enums/main.h"
extern "C" {

struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x008a4eb0
int g_BattleSpecialSnowstormResourceDefs = 0x001940B1;
int g_BattleSpecialSnowstormResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialSnowstormResourceNames + 0x0)
};
// GLOBAL: FF7 0x008a4eb8
int g_BattleSpecialSnowstormResourceTable[20] = {
    0x0, 0x0, 0x0, 0x1940b1, (int)((char *)&g_BattleFxSpecialSnowstormResourceNames + 0xc), 0x0,
    0x0, 0x0, 0x196a39, (int)((char *)&g_BattleFxSpecialSnowstormResourceNames + 0x18), (int)((char *)&g_BattleFxSpecialSnowstormResourceNames + 0x24), 0x0,
    0x0, 0x196a39, (int)((char *)&g_BattleFxSpecialSnowstormResourceNames + 0x30), (int)((char *)&g_BattleFxSpecialSnowstormResourceNames + 0x3c), 0x0, 0x0,
    0x0, 0x0
};
// GLOBAL: FF7 0x008a4f08
struct BattleSpecialSnowstormColor1 g_BattleSpecialSnowstormColor1 = {{0x80, 0x80, 0x80, 0x2c}, 0, {0}, 0, 0};
// GLOBAL: FF7 0x008a4f18
struct BattleSpecHeatBlastColor g_BattleSpecialSnowstormColor2 = {-128, -128, -128, {0x2c}};
// GLOBAL: FF7 0x008a4f20
struct BattleSpecialSnowstormParticleConfig g_BattleSpecialSnowstormParticleConfig = {{0, 0, 0, 3}, 0, 0, 0, {0x62}, 0, 0};
// GLOBAL: FF7 0x008a4f2c
struct BattleSpecialSnowstormScaleVector g_BattleSpecialSnowstormScaleVector = {1, 1};
// GLOBAL: FF7 0x008a4f30
int g_BattleSpecialSnowstormScale1 = 6656;
// GLOBAL: FF7 0x008a4f34
int g_BattleSpecialSnowstormScale2 = 4096;
// GLOBAL: FF7 0x008a4f38
struct BattleSpecialSnowstormModelReq g_BattleSpecialSnowstormModelReq = {0x1a00, {0}, 0, {0x28, 0, 0, 0, 0, 0}, 128};
unsigned char g_rawData_008a4f4c[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a4f50
struct BattleSpecialSnowstormAnimParams g_BattleSpecialSnowstormAnimParams = {{0, 0xfc, 0, 0, 0, 0, 0, 0}, 0, {0x88, 0, 0, 0, 0, 0}, 128};
unsigned char g_rawData_008a4f64[4] = {0x20, 0x00, 0x00, 0x00};
unsigned char g_BattleFxSpecialSnowstormResourceNames[72] =
    "HUBUKI1.RSD\0"
    "KOURI4.RSD\0\0"
    "KEMU1.TIM\0\0\0"
    "KEMU1.SPT\0\0\0"
    "BAKU1.TIM\0\0\0"
    "BAKU1.SPT";
// GLOBAL: FF7 0x00b6d3d8
PsxMatrix g_BattleSpecialSnowstormLookBasis = {0};
// GLOBAL: FF7 0x00b6d3f8
int g_BattleSpecialSnowstormFieldHeight = 0;
// GLOBAL: FF7 0x00b6d3fc
short g_BattleSpecialSnowstormAngle = 0;
// GLOBAL: FF7 0x00b6d400
short g_BattleSpecialSnowstormArcTan = 0;
unsigned char g_bssPad_00b6d404[4] = {0};
// GLOBAL: FF7 0x00b6d408
short g_BattleSpecialSnowstormCounter = 0;
// GLOBAL: FF7 0x00b6d40c
int g_BattleSpecialSnowstormRenderState = 0;
// GLOBAL: FF7 0x00b6d410
int g_BattleSpecialSnowstormFieldWidth = 0;
unsigned char g_bssPad_00b6d414[4] = {0};
// The head of the 400 particle records (BattleSpecialSnowstormParticle, 8 bytes each); the records
// after the first are in the filler below. Their x and y wrap at g_BattleSpecialSnowstormFieldWidth and
// g_BattleSpecialSnowstormFieldHeight.
// GLOBAL: FF7 0x00b6d418
struct BattleSpecialSnowstormParticleData g_BattleSpecialSnowstormParticleData = {0};
unsigned char g_bssPad_00b6d420[3192] = {0};
// GLOBAL: FF7 0x00b6e098
struct GfxTableCursor *g_BattleSpecialSnowstormLoadedRes = 0;
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
#define g_BattleSpecialSnowstormSpriteField8 (*(int *)((char *)&g_BattleSpecialSnowstormResourceTable + 72))
#define g_BattleSpecialSnowstormSpriteField4 (*(int *)((char *)&g_BattleSpecialSnowstormResourceTable + 76))
// clang-format on


/*
 * Loads resources for the Hubuki special effect when not already loaded.
 */
// FUNCTION: FF7 0x00547460
void __cdecl BattleFx_SpecialLoadResSnowstorm(void)
{
    GfxModelTableBank *unitOwner;
    GfxResourceRequest requestSlot;
    int resCategory;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecialSnowstormLoadedRes == 0) {
        resCategory = 4;
        resourceDefsWord = (int)&g_BattleSpecialSnowstormResourceDefs;
        unitOwner = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a4fb0
        pRequest->resourcePath = (char *)BattleFx_BuildResourcePath("SPECIAL/HUBUKI", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(resCategory, resourceDefsWord, pRequest, &g_BattleSpecialSnowstormLoadedRes, unitOwner);
    }
}

/*
 * Entry point for the Hubuki special effect, dispatching to begin.
 */
// FUNCTION: FF7 0x005474f0
void __cdecl BattleFx_SpecialEnterSnowstorm(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResSnowstorm();
    g_BattleSpecialSnowstormRenderState = Gfx_GetPsxPacketScratchPtr();
    g_BattleSpecialSnowstormAngle = 0;
    g_BattleSpecialSnowstormCounter = 0;
    switch (g_BattleUiScalingMode) {
    case MAIN_GRAPHICS_MODE_320X240:
        g_BattleSpecialSnowstormFieldWidth = 320;
        g_BattleSpecialSnowstormFieldHeight = 160;
        break;
    case MAIN_GRAPHICS_MODE_320X240_CENTERED:
        g_BattleSpecialSnowstormFieldWidth = g_BattleViewportOriginX + 320;
        g_BattleSpecialSnowstormFieldHeight = g_BattleViewportOriginY + 160;
        break;
    case MAIN_GRAPHICS_MODE_640X480:
        g_BattleSpecialSnowstormFieldWidth = 640;
        g_BattleSpecialSnowstormFieldHeight = 320;
        break;
    }
    BattleFx_SpecialBeginSnowstorm(targetMask, actorIdx);
}

/*
 * Initializes and begins the Hubuki special effect for target actors.
 */
// FUNCTION: FF7 0x00547595
void __cdecl BattleFx_SpecialBeginSnowstorm(int targetMask, int actorIdx)
{
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnSnowstormPerTarget);
    g_BattleSpecialSnowstormRenderState = Gfx_GetPsxPacketScratchPtr();
    g_BattleFxTimelineState.channel2 = 0;
    g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
    g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    Battle_StartFixedPointRamp(0x1000, 5);
}

/*
 * Spawns Hubuki effect instances for each targeted actor.
 */
// FUNCTION: FF7 0x005475e7
void __cdecl BattleFx_SpecialSpawnSnowstormPerTarget(int actor, int actorId)
{
    short i;
    BattleFxSnowstormTaskSlot *taskPointer;

    taskPointer = (BattleFxSnowstormTaskSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSnowstormTimelineTask);
    Battle_GetBoneWorldPos(actorId, g_BattleActorRecords[actorId].bone33, &taskPointer->sourcePos);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &taskPointer->targetPos);
    taskPointer->targetIndex = (short)actor;
    for (i = 0; i < 400; i++) {
        ((BattleSpecialSnowstormParticle *)&g_BattleSpecialSnowstormParticleData)[i].x = rand() % g_BattleSpecialSnowstormFieldWidth;
        ((BattleSpecialSnowstormParticle *)&g_BattleSpecialSnowstormParticleData)[i].y = rand() % g_BattleSpecialSnowstormFieldHeight;
    }
}

/*
 * Timeline task controlling state progression for the Hubuki special effect.
 */
// FUNCTION: FF7 0x005476b0
void __cdecl BattleFx_SpecialSnowstormTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[28];
        BattleFxSnowstormTimelineSpawnSlot *spawnSlot;
        struct { unsigned char pad_to_8[0x8]; short i; };
        struct { unsigned char pad_to_C[0xC]; short distance; };
        struct { unsigned char pad_to_10[0x10]; BattleFxEnergyWaveTimelineSlot *timelineSlot; };
        struct { unsigned char pad_to_14[0x14]; SVECTOR deltaX; };
    } frame;
#pragma pack(pop)

    frame.timelineSlot = (BattleFxEnergyWaveTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timelineSlot->frameCounter == 0) {
        frame.spawnSlot = (BattleFxSnowstormTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSnowstormParticleFieldTask);
        g_BattleSpecialSnowstormArcTan = ArcTangentFixed(frame.timelineSlot->targetX - frame.timelineSlot->startX, frame.timelineSlot->targetZ - frame.timelineSlot->startZ);
        frame.spawnSlot = (BattleFxSnowstormTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSnowstormShrinkingOrientedModelTask);
        frame.spawnSlot->posX = frame.timelineSlot->startX;
        frame.spawnSlot->posY = frame.timelineSlot->startY;
        frame.spawnSlot->posZ = frame.timelineSlot->startZ;
        frame.deltaX.vx = frame.timelineSlot->targetX - frame.timelineSlot->startX;
        frame.deltaX.vy = frame.timelineSlot->targetY - frame.timelineSlot->startY;
        frame.deltaX.vz = frame.timelineSlot->targetZ - frame.timelineSlot->startZ;
        Gte_BuildLookBasisFromDirection(&frame.deltaX.vx, (short *)&g_BattleSpecialSnowstormLookBasis);
        frame.distance = SqrtToInt(frame.deltaX.vx * frame.deltaX.vx + frame.deltaX.vy * frame.deltaX.vy + frame.deltaX.vz * frame.deltaX.vz);
        frame.spawnSlot->scale = frame.distance * 0x1500 / 2800;
    }
    if (frame.timelineSlot->frameCounter == 8) {
        frame.spawnSlot = (BattleFxSnowstormTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSnowstormFadeSpriteTask);
        frame.spawnSlot->posX = 0xFF9C;
        frame.spawnSlot->posY = 0xFF9C;
        frame.spawnSlot->posZ = 2000;
        Battle_PlaySfxPannedToActor(0xB0, -1, 0x40);
        g_BattleStageHideFlags |= 7;
    }
    if (frame.timelineSlot->frameCounter >= 15 && frame.timelineSlot->frameCounter < 30 && frame.timelineSlot->frameCounter % 5 == 0) {
        frame.spawnSlot = (BattleFxSnowstormTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSnowstormExpandingRisingModelTask);
        frame.spawnSlot->velocityX = SinFixed(g_BattleSpecialSnowstormArcTan + 2048) * 0x3C >> 12;
        frame.spawnSlot->velocityZ = CosFixed(g_BattleSpecialSnowstormArcTan + 2048) * 0x3C >> 12;
        frame.spawnSlot->posX = frame.timelineSlot->targetX - frame.spawnSlot->velocityX * 10;
        frame.spawnSlot->posY = frame.timelineSlot->targetY + 400;
        frame.spawnSlot->posZ = frame.timelineSlot->targetZ - frame.spawnSlot->velocityZ * 10;
    }
    if (frame.timelineSlot->frameCounter == 45) {
        Battle_PostActorHitOrHurtMessage(frame.timelineSlot->actorIndex);
        Battle_PlaySfxPannedToActor(0x95, (short)frame.timelineSlot->actorIndex, 0x40);
    }
    if (frame.timelineSlot->frameCounter > 41 && frame.timelineSlot->frameCounter < 49) {
        for (frame.i = 0; frame.i < 2; frame.i++) {
            frame.spawnSlot = (BattleFxSnowstormTimelineSpawnSlot *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialSnowstormAnimatedSpriteTask)];
            Battle_GetBoneWorldPos(frame.timelineSlot->actorIndex, g_BattleActorRecords[frame.timelineSlot->actorIndex].targetBone, (short *)((int)frame.spawnSlot + 4));
            frame.spawnSlot->posX += (short)(rand() % 1000 - 500);
            frame.spawnSlot->posY += (short)(rand() % 1000 - 500);
            frame.spawnSlot->posZ += (short)(rand() % 1000 - 500);
        }
    }
    (frame.timelineSlot->frameCounter)++;
    if (frame.timelineSlot->frameCounter == 65) {
        g_BattleStageHideFlags &= 0xF8;
        Battle_StartFixedPointRamp(0, 10);
        frame.timelineSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task to animate and render sprites for the Hubuki special effect.
 */
// FUNCTION: FF7 0x00547ac3
void __cdecl BattleFx_SpecialSnowstormAnimatedSpriteTask(void)
{
    BattleTaskSlot *task;

    task = (BattleTaskSlot *)(&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx]);
    g_BattleSpecialSnowstormSpriteField8 = Gfx_ModelTableGetLoadedRecord(3, g_BattleSpecialSnowstormLoadedRes);
    g_BattleSpecialSnowstormSpriteField4 = Gfx_ModelTableGetSpriteObject(3, g_BattleSpecialSnowstormLoadedRes);
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), 0x1000 /* 1.0 */, 0);
    g_BattleSpecialSnowstormColor1.frameIndex = task->frameCounter;
    g_BattleSpecialSnowstormRenderState = BattleFx_SubmitSpriteDraw(&g_BattleSpecialSnowstormSpriteField8, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSnowstormRenderState);
    if (g_BattleIsPaused == 0 && g_BattleSpecialSnowstormSpriteField8 != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= *(int *)((char *)*(int *)g_BattleSpecialSnowstormSpriteField8 + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Task to animate fading sprites for the Hubuki special effect.
 */
// FUNCTION: FF7 0x00547b94
void __cdecl BattleFx_SpecialSnowstormFadeSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[52];
        short mat00;
        struct { unsigned char pad_to_2[0x2]; short mat01; };
        struct { unsigned char pad_to_4[0x4]; short mat02; };
        struct { unsigned char pad_to_6[0x6]; short mat10; };
        struct { unsigned char pad_to_8[0x8]; short mat11; };
        struct { unsigned char pad_to_A[0xA]; short mat12; };
        struct { unsigned char pad_to_C[0xC]; short mat20; };
        struct { unsigned char pad_to_E[0xE]; short mat21; };
        struct { unsigned char pad_to_10[0x10]; short mat22; };
        struct { unsigned char pad_to_12[0x12]; int transX; };
        struct { unsigned char pad_to_16[0x16]; int transY; };
        struct { unsigned char pad_to_1A[0x1A]; int transZ; };
        struct { unsigned char pad_to_20[0x20]; BattleFxSnowstormFadeSpriteSlot *fadeSlot; };
        struct { unsigned char pad_to_24[0x24]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.fadeSlot = (BattleFxSnowstormFadeSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialSnowstormColor1.spriteRecord = Gfx_ModelTableGetLoadedRecord(2, g_BattleSpecialSnowstormLoadedRes);
    g_BattleSpecialSnowstormColor1.spriteObject = Gfx_ModelTableGetSpriteObject(2, g_BattleSpecialSnowstormLoadedRes);
    frame.mat00 = 0x1000;
    frame.mat01 = 0;
    frame.mat02 = 0;
    frame.mat10 = 0;
    frame.mat11 = 0x1000;
    frame.mat12 = 0;
    frame.mat20 = 0;
    frame.mat21 = 0;
    frame.mat22 = 0x1000;
    memset(&frame.transX, 0, 12);
    frame.scaleX.vz = 0xC00 /* 0.75 */;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.mat00, &frame.scaleX);
    frame.transX = frame.fadeSlot->posX;
    frame.transY = frame.fadeSlot->posY;
    frame.transZ = frame.fadeSlot->posZ;
    SetRotMatrix(&frame.mat00);
    SetTransMatrix(&frame.mat00);
    g_BattleSpecialSnowstormColor2.blue = g_BattleSpecialSnowstormAngle << 4;
    g_BattleSpecialSnowstormColor2.green = g_BattleSpecialSnowstormColor2.blue;
    (g_BattleSpecialSnowstormColor2.red) = g_BattleSpecialSnowstormColor2.green;
    g_BattleSpecialSnowstormRenderState = BattleFx_SubmitSpriteDraw(&g_BattleSpecialSnowstormColor1.spriteRecord, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_0, 1, g_BattleSpecialSnowstormRenderState);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.fadeSlot->posX += 4;
    frame.fadeSlot->posY += 4;
    if (frame.fadeSlot->frameCounter < 8) {
        g_BattleSpecialSnowstormAngle++;
    }
    if (frame.fadeSlot->frameCounter > 37) {
        g_BattleSpecialSnowstormAngle--;
    }
    (frame.fadeSlot->frameCounter)++;
    if (frame.fadeSlot->frameCounter == 45) {
        frame.fadeSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task managing the particle field animation for Hubuki.
 */
// FUNCTION: FF7 0x00547d56
void __cdecl BattleFx_SpecialSnowstormParticleFieldTask(void)
{
    struct {
        int unused;
        short step;
        short pad0;
        short i;
        short pad1;
        BattleTaskSlot *slot;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.slot = BATTLE_CURRENT_TASK_SLOT;
    BattleFx_SpecialSnowstormDrawParticleField(g_BattleSpecialSnowstormCounter);
    if (g_BattleIsPaused != 0) {
        return;
    }
    for (local.i = 0; local.i < 400; local.i++) {
        local.step = local.i % 2 * 2 + 2;
        ((BattleSpecialSnowstormParticle *)&g_BattleSpecialSnowstormParticleData)[local.i].x =
            (((BattleSpecialSnowstormParticle *)&g_BattleSpecialSnowstormParticleData)[local.i].x + local.step) %
            g_BattleSpecialSnowstormFieldWidth;
        ((BattleSpecialSnowstormParticle *)&g_BattleSpecialSnowstormParticleData)[local.i].y =
            (((BattleSpecialSnowstormParticle *)&g_BattleSpecialSnowstormParticleData)[local.i].y + local.step) %
            g_BattleSpecialSnowstormFieldHeight;
    }
    if (local.slot->frameCounter < 8) {
        g_BattleSpecialSnowstormCounter++;
    }
    if (local.slot->frameCounter > 42) {
        g_BattleSpecialSnowstormCounter--;
    }
    (local.slot->frameCounter)++;
    if (local.slot->frameCounter == 50) {
        local.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the active particle field geometry for Hubuki.
 */
// FUNCTION: FF7 0x00547e75
void __cdecl BattleFx_SpecialSnowstormDrawParticleField(short intensity)
{
    short idx;
    int q;
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    q = 0x40;
    for (idx = 0; idx < 400; idx++) {
        g_BattleSpecialSnowstormParticleConfig.phase = ((BattleSpecialSnowstormParticle *)&g_BattleSpecialSnowstormParticleData)[idx].x;
        g_BattleSpecialSnowstormParticleConfig.age = ((BattleSpecialSnowstormParticle *)&g_BattleSpecialSnowstormParticleData)[idx].y;
        g_BattleSpecialSnowstormScaleVector.y = (*(short *)&g_BattleUiScale);
        (g_BattleSpecialSnowstormScaleVector.x) = g_BattleSpecialSnowstormScaleVector.y;
        g_BattleSpecialSnowstormParticleConfig.red = intensity << 7 >> 3;
        g_BattleSpecialSnowstormParticleConfig.green = intensity << 7 >> 3;
        g_BattleSpecialSnowstormParticleConfig.blue = intensity << 7 >> 3;
        Gfx_SetBlendFactorPair(FLOAT_0_2_BITS, FLOAT_ONE_BITS);
        Gfx_DrawPsxPrimitive(&g_BattleSpecialSnowstormParticleConfig);
    }
}

/*
 * Task to render shrinking oriented models for the Hubuki effect.
 */
// FUNCTION: FF7 0x00547f3c
void __cdecl BattleFx_SpecialSnowstormShrinkingOrientedModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix orientMatrix;
        struct { unsigned char pad_to_30[0x30]; BattleFxSnowstormModelSlot *shrinkSlot; };
        struct { unsigned char pad_to_34[0x34]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.shrinkSlot = (BattleFxSnowstormModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialSnowstormModelReq.tableField = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialSnowstormLoadedRes);
    ((short *)&g_BattleSpecialSnowstormAnimParams)[1] = frame.shrinkSlot->frameCounter << 7;
    RotMatrixXYZ(&g_BattleSpecialSnowstormAnimParams, &frame.orientMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialSnowstormLookBasis, &frame.orientMatrix, &frame.orientMatrix);
    g_BattleSpecialSnowstormScale2 = frame.shrinkSlot->scale;
    (g_BattleSpecialSnowstormModelReq.scaleValue) = 0x5DC0 - frame.shrinkSlot->frameCounter * 0x320;
    g_BattleSpecialSnowstormScale1 = (g_BattleSpecialSnowstormModelReq.scaleValue);
    ScaleMatrix(&frame.orientMatrix, &g_BattleSpecialSnowstormScale1);
    frame.orientMatrix.t[0] = frame.shrinkSlot->posX;
    frame.orientMatrix.t[1] = frame.shrinkSlot->posY;
    frame.orientMatrix.t[2] = frame.shrinkSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.orientMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecialSnowstormModelReq.stepScale = frame.shrinkSlot->frameCounter * 6;
    g_BattleSpecialSnowstormRenderState = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialSnowstormModelReq.tableField, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSnowstormRenderState);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.shrinkSlot->frameCounter)++;
    if (frame.shrinkSlot->frameCounter == 30) {
        frame.shrinkSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task to render expanding rising models for the Hubuki effect.
 */
// FUNCTION: FF7 0x0054809a
void __cdecl BattleFx_SpecialSnowstormExpandingRisingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[104];
        int rampValue;
        struct { unsigned char pad_to_4[0x4]; short rampLevel; };
        struct { unsigned char pad_to_8[0x8]; short mat00; };
        struct { unsigned char pad_to_A[0xA]; short mat01; };
        struct { unsigned char pad_to_C[0xC]; short mat02; };
        struct { unsigned char pad_to_E[0xE]; short mat10; };
        struct { unsigned char pad_to_10[0x10]; short mat11; };
        struct { unsigned char pad_to_12[0x12]; short mat12; };
        struct { unsigned char pad_to_14[0x14]; short mat20; };
        struct { unsigned char pad_to_16[0x16]; short mat21; };
        struct { unsigned char pad_to_18[0x18]; short mat22; };
        struct { unsigned char pad_to_1A[0x1A]; int posX; };
        struct { unsigned char pad_to_1E[0x1E]; int posY; };
        struct { unsigned char pad_to_22[0x22]; int posZ; };
        struct { unsigned char pad_to_34[0x34]; BattleFxColdBreathRisingGrowingModelSlot *risingSlot; };
        struct { unsigned char pad_to_38[0x38]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_58[0x58]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.mat00 = 0x1000;
    frame.mat01 = 0;
    frame.mat02 = 0;
    frame.mat10 = 0;
    frame.mat11 = 0x1000;
    frame.mat12 = 0;
    frame.mat20 = 0;
    frame.mat21 = 0;
    frame.mat22 = 0x1000;
    memset(&frame.posX, 0, 12);
    frame.mat00 = 0x1000;
    frame.mat01 = 0;
    frame.mat02 = 0;
    frame.mat10 = 0;
    frame.mat11 = 0x1000;
    frame.mat12 = 0;
    frame.mat20 = 0;
    frame.mat21 = 0;
    frame.mat22 = 0x1000;
    frame.risingSlot = (BattleFxColdBreathRisingGrowingModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialSnowstormAnimParams.modelRecord = Gfx_ModelTableGetRenderObject(1, g_BattleSpecialSnowstormLoadedRes);
    if (frame.risingSlot->frameCounter < 12) {
        frame.rampValue = 8;
    } else {
        frame.rampValue = 0x14 - frame.risingSlot->frameCounter;
    }
    frame.rampLevel = *(short *)&frame.rampValue;
    frame.scaleX.vz = frame.risingSlot->frameCounter * 0x1E0;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.mat00, &frame.scaleX);
    frame.posX = frame.risingSlot->posX;
    frame.posY = frame.risingSlot->posY;
    frame.posZ = frame.risingSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.mat00, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialSnowstormAnimParams.scaleLevel = 0x1000 - (frame.rampLevel << 9);
    g_BattleSpecialSnowstormRenderState = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialSnowstormAnimParams.modelRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSnowstormRenderState);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.risingSlot->posY += 40;
    frame.risingSlot->posX += frame.risingSlot->velocityX;
    frame.risingSlot->posZ += frame.risingSlot->velocityZ;
    (frame.risingSlot->frameCounter)++;
    if (frame.risingSlot->frameCounter == 20) {
        frame.risingSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
