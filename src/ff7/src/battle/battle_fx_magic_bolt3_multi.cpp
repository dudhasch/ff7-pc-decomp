// src/ff7/src/battle/battle_fx_magic_bolt3_multi.cpp
/* Magic Effect [35 multi-target]: Bolt3 Multi-Target (Japanese: サンダガ Thundaga, dev tag: "MAGIC/THUNDER4" / Thunder4) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle effect for the Bolt 4 (Thunder4) magic: loads its resources and runs a timeline
//   that spawns flash sprites, bouncing rock models, flipped and billboard lightning sprites, and a
//   growing-holding-fading model.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_magic.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
extern "C" {

struct BattleFxModelDraw;

#pragma intrinsic(abs)

// clang-format off

// GLOBAL: FF7 0x008fd9a0
unsigned int g_BattleMagicBolt3Multi_ResourceDefs[36] = {0x00192a39, (unsigned int)&g_BattleMagicBolt3Multi_Filename_Thundg02, (unsigned int)&g_BattleMagicBolt3Multi_Filename_Kami7, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)((char *)&g_BattleMagicBolt3Multi_Filename_Kami7 + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleMagicBolt3Multi_Filename_Stone, 0x00000000, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)((char *)&g_BattleMagicBolt3Multi_Filename_Stone + 0xc), (unsigned int)((char *)&g_BattleMagicBolt3Multi_Filename_Stone + 0x1c), 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleMagicBolt3Multi_Filename_Thundg00, (unsigned int)&g_BattleMagicBolt3Multi_Filename_ElecY1, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)((char *)&g_BattleMagicBolt3Multi_Filename_ElecY1 + 0xc), (unsigned int)((char *)&g_BattleMagicBolt3Multi_Filename_ElecY1 + 0x1c), 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleMagicBolt3Multi_Filename_Thund3, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x008fda30
short g_BattleMagicBolt3Multi_InitialAngle = 9472;
// GLOBAL: FF7 0x008fda38
struct BattleMagicThunderModelReq g_BattleMagicBolt3Multi_ModelReq1 = {{0, 0x25, 0, 0, 0, 0, 0, 0, 0, 0x25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 128, {0}, 0};

unsigned char g_rawData_008fda5c[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008fda60
int g_BattleMagicBolt3Multi_ModelReq2[3] = {0, 0x88, 0};
unsigned char g_rawData_008fda6c[4] = {0x40, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008fda70
struct BattleFxMagicBolt3MultiModelRequest g_BattleMagicBolt3Multi_ModelReq3 = {{0, 4, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 0};
// GLOBAL: FF7 0x008fda98
BattleFxThunderSpriteColor g_BattleMagicBolt3Multi_SpriteColors = {-128, -128, -128, 0x2e, 0, {0}};
// GLOBAL: FF7 0x008fdaa0
short g_BattleMagicBolt3Multi_InitialScale = 6144;
// GLOBAL: FF7 0x008fdaa8
unsigned char g_BattleMagicBolt3Multi_ParamTable[24] = {0, 0x14, 0, 0, 0, 0, 0, 0, 0, 0x10, 0, 0, 0, 0, 0xd8, 0xfe, 0xff, 0xff};
// GLOBAL: FF7 0x008fdac0
BattleFxBolt3MultiSpriteReq g_BattleMagicBolt3Multi_SpriteReq1 = {0, 0, 0x80, 0x80, 0x80, 0x2e, 0, {0}, 0, 8, {0}, 0x80};
unsigned char g_rawData_008fdadc[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008fdae0
struct BattleFxBolt3MultiBoltMatrix g_BattleMagicBolt3Multi_SpriteReq2[2] = {{{0, 0x10, 0, 0, 0, 0, 0, 0, 0, 0}, -0x1000, {0}, 0x1000}, {0}};
// GLOBAL: FF7 0x008fdb00
unsigned char g_BattleMagicBolt3Multi_Filename_Thundg02[13] = "THUNDG02.TIM";
// GLOBAL: FF7 0x008fdb10
unsigned char g_BattleMagicBolt3Multi_Filename_Kami7[22] =
    "KAMI_7.SPT\0\0"
    "STONE.RSD";
// GLOBAL: FF7 0x008fdb28
unsigned char g_BattleMagicBolt3Multi_Filename_Stone[40] =
    "STONE.RSD\0\0\0"
    "THUNDG03.TIM\0\0\0\0"
    "SPARK_4.SPT";
// GLOBAL: FF7 0x008fdb50
unsigned char g_BattleMagicBolt3Multi_Filename_Thundg00[13] = "THUNDG00.TIM";
// GLOBAL: FF7 0x008fdb60
unsigned char g_BattleMagicBolt3Multi_Filename_ElecY1[40] =
    "ELEC_Y1.SPT\0"
    "THUNDG01.TIM\0\0\0\0"
    "ELEC_Y2.SPT";
// GLOBAL: FF7 0x008fdb88
unsigned char g_BattleMagicBolt3Multi_Filename_Thund3[12] = "THUND_3.RSD";
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00be0d90
int g_BattleMagicBolt3Multi_ChainTail = 0;
// GLOBAL: FF7 0x00be0d94
int g_BattleMagicBolt3Multi_ActorMask = 0;
// GLOBAL: FF7 0x00be0d98
int g_BattleMagicBolt3Multi_CentroidXY = 0;
// GLOBAL: FF7 0x00be0d9c
short g_BattleMagicBolt3Multi_CentroidZ = 0;
// GLOBAL: FF7 0x00be0da0
struct GfxTableCursor *g_BattleMagicBolt3Multi_ResourceTable = 0;
// clang-format on

/* Loads the Bolt 4 (Thunder4) magic resource package if not already loaded. */
// FUNCTION: FF7 0x005b68a0
void __cdecl BattleFx_MagicLoadResBolt3Multi(void)
{
    GfxModelTableBank *baseOffset;
    GfxResourceRequest requestRecord;
    int kind;
    char pathEnd[200];
    GfxResourceRequest *request;
    int resultDefCatalog;

    if (g_pBattle3dScene != 0 && g_BattleMagicBolt3Multi_ResourceTable == 0) {
        kind = 7;
        resultDefCatalog = (int)&g_BattleMagicBolt3Multi_ResourceDefs;
        baseOffset = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestRecord;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x008fdb94
        request->resourcePath = BattleFx_BuildResourcePath("MAGIC/THUNDER4", pathEnd);
        Gfx_LoadModelTableIfAbsent(kind, resultDefCatalog, request, &g_BattleMagicBolt3Multi_ResourceTable, baseOffset);
    }
}

/* Entry point for the Bolt 4 (Thunder4) magic effect. */
// FUNCTION: FF7 0x005b6930
void __cdecl BattleFx_MagicEnterBolt3Multi(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResBolt3Multi();
    BattleFx_MagicBeginBolt3Multi(targetMask, actorIdx);
}

/* Initializes parameters and spawns timeline task for Bolt 4 (Thunder4). */
// FUNCTION: FF7 0x005b694a
void __cdecl BattleFx_MagicBeginBolt3Multi(int targetMask, int actorIdx)
{
    BattleFxBolt3MultiRecord *record;

    if (REG_ALLOC_HINT) {
    }
    g_BattleMagicBolt3Multi_ActorMask = targetMask;
    Battle_GetActorMaskCentroid(targetMask, (short *)&g_BattleMagicBolt3Multi_CentroidXY);
    record = (BattleFxBolt3MultiRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBolt3MultiTimelineTask);
    record->actorId = g_BattleSpecialActorId;
    record->param = actorIdx;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, 0x40, 0x56);
}

/* Manages the timeline and spawns models and sprites for Bolt 4 (Thunder4). */
// FUNCTION: FF7 0x005b69af
void __cdecl BattleFx_MagicBolt3MultiTimelineTask(void)
{
    int padGap[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxBolt3MultiGrowRecord *grow;
    int padWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxBolt3MultiParticle *particle;
    int stackWords[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxBolt3MultiBounceTask *pBounce;
    int stackGap[8]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxFreezeTimeline *timeline;
    int actorIndex;
    int i;

    timeline = (BattleFxFreezeTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (timeline->stage == 0) {
        g_BattleFxTimelineState.red = 0;
        g_BattleFxTimelineState.channel1 = 0;
        g_BattleFxTimelineState.channel2 = 0;
        Battle_StartFixedPointRamp(0x800, 4);
    }
    if (timeline->stage >= 6 && timeline->stage < 26 && (timeline->stage - 6) % 4 == 0) {
        particle = (BattleFxBolt3MultiParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBolt3MultiFlashSpriteTask);
        particle->posX = *(short *)&g_BattleMagicBolt3Multi_CentroidXY + rand() % 3000 * 2 + -3000;
        particle->posY = 0;
        particle->posZ = g_BattleMagicBolt3Multi_CentroidZ + rand() % 1500 * 2 + -1500;
        for (i = 0; i < 3; i++) {
            pBounce = (BattleFxBolt3MultiBounceTask *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBolt3MultiBouncingModelTask);
            *(Int64Pair *)&pBounce->posX = *(Int64Pair *)&particle->posX;
            pBounce->velocityX = rand() % 200 - 100;
            pBounce->velocityY = -(rand() % 200 + 200);
            pBounce->velocityZ = rand() % 200 - 100;
            pBounce->angleStepX = rand() % 10 - 5;
            pBounce->angleStepY = rand() % 20 - 10;
            pBounce->angleStepZ = rand() % 40 - 20;
            pBounce->scale = rand() % 0x2400 + 0x400;
            pBounce->ownerSlot = 0;
        }
        grow = (BattleFxBolt3MultiGrowRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBolt3MultiGrowHoldFadeModelTask);
        *(Int64Pair *)&grow->offsetX = *(Int64Pair *)&particle->posX;
    }
    if (timeline->stage == 40) {
        for (i = 0; i < 10; i++) {
            if ((g_BattleMagicBolt3Multi_ActorMask & 1 << i) != 0) {
                Battle_PostActorHitOrHurtMessage(i);
            }
        }
    }
    if (timeline->stage >= 24 && timeline->stage < 28) {
        for (i = 0; i < 1; i++) {
            for (actorIndex = 0; actorIndex < 10; actorIndex++) {
                if ((g_BattleMagicBolt3Multi_ActorMask & 1 << actorIndex) != 0) {
                    particle = (BattleFxBolt3MultiParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBolt3MultiBillboardSpriteTask);
                    Battle_GetBoneWorldPos(actorIndex, g_BattleActorRecords[actorIndex].targetBone, &particle->posX);
                    particle->posX += (short)(rand() % 2400 - 1200);
                    particle->posY += (short)(rand() % 2400 - 1200);
                    particle->posZ += (short)(rand() % 2400 - 1200);
                }
            }
        }
    }
    if (timeline->stage == 36) {
        Battle_AllocateResourceSlot(BattleFx_MagicBolt3MultiFlippedSpriteTask);
    }
    if (timeline->stage == 68) {
        Battle_StartFixedPointRamp(0, 4);
    }
    if (timeline->stage == 68) {
        timeline->ownerSlot = BATTLE_TASK_FREE;
    }
    (timeline->stage)++;
}

/* Updates and renders a flash sprite for Bolt 4 (Thunder4). */
// FUNCTION: FF7 0x005b6da3
void __cdecl BattleFx_MagicBolt3MultiFlashSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[36];
        int tableWordA;
        struct { unsigned char pad_to_4[0x4]; int tableWordB; };
        struct { unsigned char pad_to_8[0x8]; char colorR; };
        struct { unsigned char pad_to_9[0x9]; char colorG; };
        struct { unsigned char pad_to_A[0xA]; char colorB; };
        struct { unsigned char pad_to_B[0xB]; char colorCode; };
        struct { unsigned char pad_to_C[0xC]; short primShortA; };
        struct { unsigned char pad_to_E[0xE]; short primShortB; };
        struct { unsigned char pad_to_10[0x10]; int spawnIndex; };
        struct { unsigned char pad_to_14[0x14]; BattleBolt3MultiFlashSlot *slot; };
        struct { unsigned char pad_to_18[0x18]; SVECTOR spawnX; };
        struct { unsigned char pad_to_20[0x20]; int tmpFrameIndex; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleBolt3MultiFlashSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.tableWordA = 0;
    frame.tableWordB = 0;
    frame.colorR = 0x80;
    frame.colorG = 0x80;
    frame.colorB = 0x80;
    frame.colorCode = 0x2E;
    frame.primShortA = 0;
    memset(&frame.primShortB, 0, 2);
    frame.tableWordA = Gfx_ModelTableGetLoadedRecord(0, g_BattleMagicBolt3Multi_ResourceTable);
    frame.tableWordB = Gfx_ModelTableGetSpriteObject(0, g_BattleMagicBolt3Multi_ResourceTable);
    frame.tmpFrameIndex = frame.slot->frameIndex;
    if (frame.tmpFrameIndex < 0) {
        g_BattleMagicBolt3Multi_InitialAngle = 0;
    } else {
        frame.tmpFrameIndex = frame.tmpFrameIndex;
        if (frame.tmpFrameIndex < 0) {
            g_BattleMagicBolt3Multi_InitialAngle = frame.tmpFrameIndex * 0x2500 / 0;
        } else {
            frame.tmpFrameIndex = frame.tmpFrameIndex;
            if (frame.tmpFrameIndex < 1) {
                g_BattleMagicBolt3Multi_InitialAngle = 0x2500 /* 832.5 deg */;
            } else {
                frame.tmpFrameIndex--;
                if (frame.tmpFrameIndex < 4) {
                    g_BattleMagicBolt3Multi_InitialAngle = ((frame.tmpFrameIndex * 0) / 4) + 0x2500;
                    frame.colorB = 0x80 - ((frame.tmpFrameIndex << 7) / 4);
                    frame.colorG = frame.colorB;
                    frame.colorR = frame.colorG;
                } else {
                    frame.slot->ownerSlot = BATTLE_TASK_FREE;
                    return;
                }
            }
        }
    }
    BattleFx_MakeViewSpacePointPushed((void *)((char *)frame.slot + 8), 0xFFFFFE0C, (Vec3sPacked *)&g_BattleMagicBolt3Multi_InitialAngle);
    SetRotMatrix(&g_BattleMagicBolt3Multi_InitialAngle);
    SetTransMatrix(&g_BattleMagicBolt3Multi_InitialAngle);
    g_BattleMagicBolt3Multi_ChainTail = BattleFx_SubmitSpriteDraw(&frame.tableWordA, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicBolt3Multi_ChainTail);
    if (g_BattleIsPaused == 0) {
        for (frame.spawnIndex = 0; frame.spawnIndex < 3; frame.spawnIndex++) {
            frame.spawnX.vx = frame.slot->posX + rand() % 1600 + -800;
            frame.spawnX.vy = 0;
            frame.spawnX.vz = frame.slot->posZ + rand() % 1600 + -800;
            Battle_SpawnWorldSpriteTask((Vec3sPacked *)&frame.spawnX, 12288, 12288);
        }
        (frame.slot->frameIndex)++;
    }
}

/* Updates and renders bouncing rock/stone models for Bolt 4 (Thunder4). */
// FUNCTION: FF7 0x005b6fad
void __cdecl BattleFx_MagicBolt3MultiBouncingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[60];
        SVECTOR angleX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleFxBolt3MultiBounceTask *slot; };
        struct { unsigned char pad_to_2C[0x2C]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxBolt3MultiBounceTask *)BATTLE_CURRENT_TASK_SLOT;
    if (g_pBattle3dScene != 0) {
        g_BattleMagicBolt3Multi_ModelReq2[0] = g_pBattle3dScene->modelRequest;
    } else {
        g_BattleMagicBolt3Multi_ModelReq2[0] = 0;
    }
    if (frame.slot->ownerSlot < 1) {
        (g_BattleMagicBolt3Multi_ModelReq1.modelHandle) = Gfx_ModelTableGetRenderObject(1, g_BattleMagicBolt3Multi_ResourceTable);
        (g_BattleMagicBolt3Multi_ModelReq1.flags) = 0x80;
        (g_BattleMagicBolt3Multi_ModelReq1.fadeLevel) = 0;
    } else {
        (g_BattleMagicBolt3Multi_ModelReq1.modelHandle) = Gfx_ModelTableGetRenderObject(2, g_BattleMagicBolt3Multi_ResourceTable);
        (g_BattleMagicBolt3Multi_ModelReq1.flags) = 0x88;
        (g_BattleMagicBolt3Multi_ModelReq1.fadeLevel) = ((frame.slot->frameCount << 12) / 4);
    }
    frame.angleX.vx = frame.slot->angleX << 4;
    frame.angleX.vy = frame.slot->angleY << 4;
    frame.angleX.vz = frame.slot->angleZ << 4;
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.slot->posX;
    frame.rotMatrix.t[1] = frame.slot->posY;
    frame.rotMatrix.t[2] = frame.slot->posZ;
    frame.scaleX.vz = frame.slot->scale;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleMagicBolt3Multi_ChainTail = BattleFx_SubmitModelDraw((BattleFxModelDraw *)(((char *)&g_BattleMagicBolt3Multi_ModelReq1 + 24)), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicBolt3Multi_ChainTail);
    (*(int *)((char *)&g_BattleMagicBolt3Multi_ModelReq3 + 18)) = frame.slot->posX;
    (*(int *)((char *)&g_BattleMagicBolt3Multi_ModelReq3 + 22)) = 0;
    (*(int *)((char *)&g_BattleMagicBolt3Multi_ModelReq3 + 26)) = frame.slot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, (int *)&g_BattleMagicBolt3Multi_ModelReq3, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    (((BattleFxModelDraw *)&g_BattleMagicBolt3Multi_ModelReq2)->fadeLevel) = (g_BattleMagicBolt3Multi_ModelReq1.fadeLevel);
    g_BattleMagicBolt3Multi_ChainTail = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicBolt3Multi_ModelReq2, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_2, 1, g_BattleMagicBolt3Multi_ChainTail);
    if (g_BattleIsPaused == 0) {
        if (frame.slot->ownerSlot >= 1) {
            (frame.slot->frameCount)++;
            if (frame.slot->frameCount >= 4) {
                frame.slot->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
        frame.slot->posX += frame.slot->velocityX;
        frame.slot->posY += frame.slot->velocityY;
        frame.slot->posZ += frame.slot->velocityZ;
        frame.slot->velocityY += 30;
        if (frame.slot->posY > 0) {
            frame.slot->posY = -frame.slot->posY;
            frame.slot->velocityY = (-frame.slot->velocityY) / 4;
            frame.slot->velocityX >>= 1;
            frame.slot->velocityZ >>= 1;
            frame.slot->angleStepX >>= 1;
            frame.slot->angleStepY >>= 1;
            frame.slot->angleStepZ >>= 1;
            (frame.slot->ownerSlot)++;
        }
        *(char *)&frame.slot->angleX += *(char *)&frame.slot->angleStepX;
        *(char *)&frame.slot->angleY += *(char *)&frame.slot->angleStepY;
        *(char *)&frame.slot->angleZ += *(char *)&frame.slot->angleStepZ;
    }
}

/* Updates and renders a flipped billboard sprite for Bolt 4 (Thunder4). */
// FUNCTION: FF7 0x005b7323
void __cdecl BattleFx_MagicBolt3MultiFlippedSpriteTask(void)
{
    int age;
    BattleTaskSlot *task;
    int phase;

    task = BATTLE_CURRENT_TASK_SLOT;
    phase = task->frameCounter & 7;
    if (phase < 4) {
        g_BattleMagicBolt3Multi_InitialScale = 0x1800 /* 1.5 */;
    } else {
        phase -= 4;
        g_BattleMagicBolt3Multi_InitialScale = 0xE800 /* 14.5 */;
    }
    if (phase < 2) {
        (g_BattleMagicBolt3Multi_ModelReq3.tableValueA) = Gfx_ModelTableGetLoadedRecord(4, g_BattleMagicBolt3Multi_ResourceTable);
        (g_BattleMagicBolt3Multi_ModelReq3.tableValueB) = Gfx_ModelTableGetSpriteObject(4, g_BattleMagicBolt3Multi_ResourceTable);
    } else {
        (g_BattleMagicBolt3Multi_ModelReq3.tableValueA) = Gfx_ModelTableGetLoadedRecord(5, g_BattleMagicBolt3Multi_ResourceTable);
        (g_BattleMagicBolt3Multi_ModelReq3.tableValueB) = Gfx_ModelTableGetSpriteObject(5, g_BattleMagicBolt3Multi_ResourceTable);
    }
    g_BattleMagicBolt3Multi_SpriteColors.flip = phase & 1;
    age = task->frameCounter;
    if (age < 4) {
        g_BattleMagicBolt3Multi_SpriteColors.colorB = (age << 7) / 4;
        g_BattleMagicBolt3Multi_SpriteColors.colorG = g_BattleMagicBolt3Multi_SpriteColors.colorB;
        g_BattleMagicBolt3Multi_SpriteColors.colorR = g_BattleMagicBolt3Multi_SpriteColors.colorG;
    } else {
        age -= 4;
        if (age < 15) {
            g_BattleMagicBolt3Multi_SpriteColors.colorB = 0x80;
            g_BattleMagicBolt3Multi_SpriteColors.colorG = g_BattleMagicBolt3Multi_SpriteColors.colorB;
            g_BattleMagicBolt3Multi_SpriteColors.colorR = g_BattleMagicBolt3Multi_SpriteColors.colorG;
        } else {
            age -= 15;
            if (age < 4) {
                g_BattleMagicBolt3Multi_SpriteColors.colorB = (age * -0x80 / 4) + 0x80;
                g_BattleMagicBolt3Multi_SpriteColors.colorG = g_BattleMagicBolt3Multi_SpriteColors.colorB;
                g_BattleMagicBolt3Multi_SpriteColors.colorR = g_BattleMagicBolt3Multi_SpriteColors.colorG;
            } else {
                task->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    (*(int *)((char *)&g_BattleMagicBolt3Multi_ParamTable + 18)) = GetGeomScreen() << 3;
    SetRotMatrix(&g_BattleMagicBolt3Multi_InitialScale);
    SetTransMatrix(&g_BattleMagicBolt3Multi_InitialScale);
    g_BattleMagicBolt3Multi_ChainTail = BattleFx_SubmitSpriteDraw((char *)&g_BattleMagicBolt3Multi_ModelReq3 + 32, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 3, g_BattleMagicBolt3Multi_ChainTail);
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
    }
}

/* Updates and renders a billboard lightning sprite for Bolt 4 (Thunder4). */
// FUNCTION: FF7 0x005b74fa
void __cdecl BattleFx_MagicBolt3MultiBillboardSpriteTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicBolt3Multi_SpriteReq1.drawReq = (BattleFxMagicDemi3MultiDrawReq *)Gfx_ModelTableGetLoadedRecord(3, g_BattleMagicBolt3Multi_ResourceTable);
    g_BattleMagicBolt3Multi_SpriteReq1.resource = Gfx_ModelTableGetSpriteObject(3, g_BattleMagicBolt3Multi_ResourceTable);
    g_BattleMagicBolt3Multi_SpriteReq1.frame = task->frameCounter >> 8;
    BattleFx_InstallBillboardMatrix((short *)(task->data + 4), 0x3000 /* 3.0 */, 0);
    g_BattleMagicBolt3Multi_ChainTail = BattleFx_SubmitSpriteDraw(&g_BattleMagicBolt3Multi_SpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicBolt3Multi_ChainTail);
    if (g_BattleIsPaused == 0) {
        task->frameCounter += 256;
        if (g_BattleMagicBolt3Multi_SpriteReq1.drawReq != 0 && task->frameCounter >> 8 >= g_BattleMagicBolt3Multi_SpriteReq1.drawReq->frameTable->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/* Updates and renders a growing, holding, and fading model for Bolt 4 (Thunder4). */
// FUNCTION: FF7 0x005b75d2
void __cdecl BattleFx_MagicBolt3MultiGrowHoldFadeModelTask(void)
{
    int angle;
    int localFrame;
    BattleFxBolt3MultiGrowRecord *selfPtr;
    int matrix[8];

    selfPtr = (BattleFxBolt3MultiGrowRecord *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicBolt3Multi_SpriteReq1.modelGroup = Gfx_ModelTableGetRenderObject(6, g_BattleMagicBolt3Multi_ResourceTable);
    localFrame = selfPtr->frame;
    if (localFrame < 8) {
        g_BattleMagicBolt3Multi_SpriteReq1.level = 0x80;
    } else {
        localFrame -= 8;
        if (localFrame < 8) {
            g_BattleMagicBolt3Multi_SpriteReq1.level = (localFrame * -0x80 / 8) + 0x80;
        } else {
            selfPtr->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    angle = selfPtr->frame * 0x4800 / 16;
    (g_BattleMagicBolt3Multi_SpriteReq2[0].m21) = angle;
    (*(short *)&g_BattleMagicBolt3Multi_SpriteReq2) = (g_BattleMagicBolt3Multi_SpriteReq2[0].m21);
    (g_BattleMagicBolt3Multi_SpriteReq2[0].m12) = -angle;
    (*(int *)((char *)&g_BattleMagicBolt3Multi_SpriteReq2 + 18)) = selfPtr->offsetX;
    (*(int *)((char *)&g_BattleMagicBolt3Multi_SpriteReq2 + 22)) = selfPtr->offsetY;
    (*(int *)((char *)&g_BattleMagicBolt3Multi_SpriteReq2 + 26)) = selfPtr->offsetZ;
    MultiplyMatrix(&g_BattleCameraTransform, (short *)&g_BattleMagicBolt3Multi_SpriteReq2, matrix);
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    g_BattleMagicBolt3Multi_ChainTail = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicBolt3Multi_SpriteReq1.modelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicBolt3Multi_ChainTail);
    if (g_BattleIsPaused == 0) {
        (selfPtr->frame)++;
    }
}
} /* extern "C" */
