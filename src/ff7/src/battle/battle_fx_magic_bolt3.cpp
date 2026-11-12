// src/ff7/src/battle/battle_fx_magic_bolt3.cpp
/* Magic Effect [35]: Bolt3 (Japanese: サンダガ Thundaga, dev tag: "MAGIC/THUNDER3" / Thunder3) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle effect for the Bolt 3 (Thunder3) magic: loads its resources, plays its sound,
//   and runs a timeline that spawns flash sprites, bouncing rock models, billboard lightning
//   sprites, and growing models.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_magic.h"
#include "lib/types/graphics.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;

#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x008fdba8
int g_BattleMagicBolt3_ResourceDefs = 0x00192A39;
int g_BattleMagicBolt3_ResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxMagicBolt3ResourceNames + 0x0)
};
int g_BattleMagicBolt3_ResourceDefsDataTail[28] = {
    (int)((char *)&g_BattleFxMagicBolt3ResourceNames + 0x10), 0x0, 0x0, 0x1900b4, (int)((char *)&g_BattleFxMagicBolt3ResourceNames + 0x1c), 0x0,
    0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleFxMagicBolt3ResourceNames + 0x28), 0x0, 0x0,
    0x0, 0x192a39, (int)((char *)&g_BattleFxMagicBolt3ResourceNames2 + 0x4), (int)((char *)&g_BattleFxMagicBolt3ResourceNames2 + 0x14), 0x0, 0x0,
    0x6401b1, (int)((char *)&g_BattleFxMagicBolt3ResourceNames2 + 0x20), (int)((char *)&g_BattleFxMagicBolt3ResourceNames3 + 0x8), 0x0, 0x0, 0x1900b1,
    (int)((char *)&g_BattleFxMagicBolt3ResourceNames3 + 0x18), 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x008fdc20
short g_BattleMagicBolt3_InitialRotation = 9472;
// GLOBAL: FF7 0x008fdc28
struct BattleMagicBolt3_ModelReq1 g_BattleMagicBolt3_ModelReq1 = {{0, 0x25, 0, 0, 0, 0, 0, 0, 0, 0x25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 128, {0}, 0};
unsigned char g_rawData_008fdc4c[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008fdc50
struct BattleMagicBolt3_ModelReq2 g_BattleMagicBolt3_ModelReq2 = {0, {0x88, 0, 0, 0, 0, 0}, 0};
unsigned char g_rawData_008fdc5c[4] = {0x40, 0x00, 0x00, 0x00};
#undef g_BattleMagicBolt3_ModelReq3
// GLOBAL: FF7 0x008fdc60
struct BattleMagicBolt3_ModelReq3 g_BattleMagicBolt3_ModelReq3 = {{0, 4, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 4}, 0, 0, 0, {0}, 0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}, 0, {0x28, 0, 0, 0}, 0, 128, 0, {0}, 0, {8, 0, 0, 0, 0, 0}, 0};
unsigned char g_rawData_008fdcac[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008fdcb0
struct BattleMagicRemove_QuadColors g_BattleMagicBolt3_SpriteReq = {0x1000, {0}, -0x1000, {0}, 0x1000, {0}, 0, 0, 0, {0}};
unsigned char g_BattleFxMagicBolt3ResourceNames[44] = {
    0x54, 0x48, 0x55, 0x4e, 0x44, 0x47, 0x30, 0x32, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x00, 0x00, 0x4b, 0x41, 0x4d, 0x49, 0x5f, 0x37, 0x2e, 0x53,
    0x50, 0x54, 0x00, 0x00, 0x53, 0x54, 0x4f, 0x4e, 0x45, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x00, 0x53, 0x54, 0x4f, 0x4e
};
unsigned char g_rawData_008fdcfc[4] = {0x45, 0x2e, 0x52, 0x53};
unsigned char g_BattleFxMagicBolt3ResourceNames2[40] = {
    0x44, 0x00, 0x00, 0x00, 0x54, 0x48, 0x55, 0x4e, 0x44, 0x47, 0x30, 0x33, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x00, 0x00, 0x53, 0x50, 0x41, 0x52,
    0x4b, 0x5f, 0x34, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x54, 0x48, 0x55, 0x4e, 0x44, 0x47, 0x5f, 0x32
};
unsigned char g_BattleFxMagicBolt3ResourceNames3[36] = {
    0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x00, 0x00, 0x54, 0x48, 0x55, 0x4e, 0x44, 0x47, 0x30, 0x31, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x00, 0x00,
    0x54, 0x48, 0x55, 0x4e, 0x44, 0x5f, 0x33, 0x2e, 0x52, 0x53, 0x44, 0x00
};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00be0da4
int g_BattleMagicBolt3_ChainTail = 0;
// GLOBAL: FF7 0x00be0da8
struct GfxTableCursor *g_BattleMagicBolt3_ResourceHandle = 0;
// clang-format on

/* Loads the Bolt 3 (Thunder3) magic resource package if not already loaded. */
// FUNCTION: FF7 0x005b7730
void __cdecl BattleFx_MagicLoadResBolt3(void)
{
    GfxModelTableBank *rendererField;
    GfxResourceRequest requestRecord;
    int typeId;
    char pathBuffer[200];
    GfxResourceRequest *request;
    int resDescList;

    if (g_pBattle3dScene != 0 && g_BattleMagicBolt3_ResourceHandle == 0) {
        typeId = 6;
        resDescList = (int)&g_BattleMagicBolt3_ResourceDefs;
        rendererField = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestRecord;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x008fdd4c
        request->resourcePath = BattleFx_BuildResourcePath("MAGIC/THUNDER3", pathBuffer);
        Gfx_LoadModelTableIfAbsent(typeId, resDescList, request, &g_BattleMagicBolt3_ResourceHandle, rendererField);
    }
}

/* Entry point for the Bolt 3 (Thunder3) magic effect. */
// FUNCTION: FF7 0x005b77c0
void __cdecl BattleFx_MagicEnterBolt3(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResBolt3();
    g_BattleMagicBolt3_ChainTail = Gfx_GetPsxPacketScratchPtr();
    BattleFx_MagicBeginBolt3(targetMask, actorIdx);
}

/* Initializes parameters and dispatches sound for Bolt 3 (Thunder3). */
// FUNCTION: FF7 0x005b77e4
void __cdecl BattleFx_MagicBeginBolt3(int targetMask, int actorIdx)
{
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 0, BattleFx_MagicSpawnBolt3PerTarget);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, 0x40, 0x46);
}

/* Spawns the timeline task for Bolt 3 (Thunder3) per target actor. */
// FUNCTION: FF7 0x005b780e
void __cdecl BattleFx_MagicSpawnBolt3PerTarget(int actor, int param)
{
    BattleFxBolt3MultiRecord *record;

    record = (BattleFxBolt3MultiRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBolt3TimelineTask);
    record->actorId = actor;
    record->param = param;
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)((char *)record + 8));
}

/* Manages the timeline and spawns models and sprites for Bolt 3 (Thunder3). */
// FUNCTION: FF7 0x005b786b
void __cdecl BattleFx_MagicBolt3TimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[28];
        BattleFxBolt3Particle *particle;
        struct { unsigned char pad_to_4[0x4]; BattleFxBolt3Slot *spawnSlot; };
        struct { unsigned char pad_to_8[0x8]; int i; };
        struct { unsigned char pad_to_C[0xC]; BattleFxBolt3Slot *timelineSlot; };
        struct { unsigned char pad_to_10[0x10]; SVECTOR spritePosX; };
        struct { unsigned char pad_to_18[0x18]; int phase; };
    } frame;
#pragma pack(pop)

    frame.timelineSlot = (BattleFxBolt3Slot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timelineSlot->step == 0) {
        Battle_StartFixedPointRamp(0x800, 4);
        g_BattleFxTimelineState.red = 0;
        g_BattleFxTimelineState.channel1 = 0;
        g_BattleFxTimelineState.channel2 = 0;
    }
    frame.phase = frame.timelineSlot->step - 4;
    if (frame.phase == 0) {
        frame.spawnSlot = (BattleFxBolt3Slot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBolt3FlashSpriteTask);
        frame.spawnSlot->baseX = frame.timelineSlot->baseX;
        frame.spawnSlot->baseY = 0;
        frame.spawnSlot->baseZ = frame.timelineSlot->baseZ;
        frame.spawnSlot->pushLength = -g_BattleActorRecords[frame.timelineSlot->battlerIndex].radius;
    }
    if (frame.phase == 4) {
        Battle_PostActorHitOrHurtMessage(frame.timelineSlot->battlerIndex);
    }
    if (frame.phase >= 2 && frame.phase < 3) {
        for (frame.i = 0; frame.i < 10; frame.i++) {
            frame.particle = (BattleFxBolt3Particle *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBolt3BouncingModelTask);
            frame.particle->posX = frame.timelineSlot->baseX;
            frame.particle->posY = 0;
            frame.particle->posZ = frame.timelineSlot->baseZ;
            frame.particle->velocityX = rand() % 200 - 100;
            frame.particle->velocityY = -(rand() % 200 + 200);
            frame.particle->velocityZ = rand() % 200 - 100;
            frame.particle->angleStepX = rand() % 40 - 20;
            frame.particle->angleStepY = rand() % 20 - 10;
            frame.particle->angleStepZ = rand() % 10 - 5;
            frame.particle->scale = rand() % 0x1E00 + 0x200;
            frame.particle->ownerSlot = 0;
        }
    }
    if (frame.phase >= 0 && frame.phase < 16) {
        for (frame.i = 0; frame.i < 3; frame.i++) {
            frame.spritePosX.vx = frame.timelineSlot->baseX + rand() % 1600 + -800;
            frame.spritePosX.vy = 0;
            frame.spritePosX.vz = frame.timelineSlot->baseZ + rand() % 1600 + -800;
            Battle_SpawnWorldSpriteTask((Vec3sPacked *)&frame.spritePosX, 12288, 12288);
        }
    }
    if (frame.phase >= 1 && frame.phase < 16) {
        for (frame.i = 0; frame.i < 1; frame.i++) {
            frame.spawnSlot = (BattleFxBolt3Slot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBolt3BillboardSpriteTask);
            frame.spawnSlot->baseX = frame.timelineSlot->baseX + rand() % 2000 + -1000;
            frame.spawnSlot->baseY = frame.timelineSlot->baseY + rand() % 2000 + -1000;
            frame.spawnSlot->baseZ = frame.timelineSlot->baseZ + rand() % 2000 + -1000;
        }
    }
    if (frame.phase == 2) {
        frame.spawnSlot = (BattleFxBolt3Slot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBolt3GrowFadeModelTask);
        *(Int64Pair *)((char *)frame.spawnSlot + 8) = *(Int64Pair *)((char *)frame.timelineSlot + 8);
        frame.spawnSlot->rotZ = 0;
        frame.spawnSlot->rotY = 0;
        frame.spawnSlot->rotX = 0;
    }
    if (frame.phase == 2) {
        frame.spawnSlot = (BattleFxBolt3Slot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBolt3GrowHoldFadeModelTask);
        frame.spawnSlot->baseX = frame.timelineSlot->baseX;
        frame.spawnSlot->baseY = 0;
        frame.spawnSlot->baseZ = frame.timelineSlot->baseZ;
    }
    if (frame.phase == 0x1E) {
        Battle_StartFixedPointRamp(0, 4);
        frame.timelineSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    (frame.timelineSlot->step)++;
}

/* Updates and renders a flash sprite for Bolt 3 (Thunder3). */
// FUNCTION: FF7 0x005b7c6a
void __cdecl BattleFx_MagicBolt3FlashSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[24];
        int loadedRecord;
        struct { unsigned char pad_to_4[0x4]; int spriteObject; };
        struct { unsigned char pad_to_8[0x8]; char tintR; };
        struct { unsigned char pad_to_9[0x9]; char tintG; };
        struct { unsigned char pad_to_A[0xA]; char tintB; };
        struct { unsigned char pad_to_B[0xB]; char flagsByte; };
        struct { unsigned char pad_to_C[0xC]; short tmpShortA; };
        struct { unsigned char pad_to_E[0xE]; short tmpShortB; };
        struct { unsigned char pad_to_10[0x10]; BattleFxFrogSongNoteTask *flashSlot; };
        struct { unsigned char pad_to_14[0x14]; int frameNumber; };
    } frame;
#pragma pack(pop)

    frame.flashSlot = (BattleFxFrogSongNoteTask *)BATTLE_CURRENT_TASK_SLOT;
    frame.loadedRecord = 0;
    frame.spriteObject = 0;
    frame.tintR = 0x80;
    frame.tintG = 0x80;
    frame.tintB = 0x80;
    frame.flagsByte = 0x2E;
    frame.tmpShortA = 0;
    memset(&frame.tmpShortB, 0, 2);
    frame.loadedRecord = Gfx_ModelTableGetLoadedRecord(0, g_BattleMagicBolt3_ResourceHandle);
    frame.spriteObject = Gfx_ModelTableGetSpriteObject(0, g_BattleMagicBolt3_ResourceHandle);
    frame.frameNumber = frame.flashSlot->frameIndex;
    if (frame.frameNumber < 0) {
        g_BattleMagicBolt3_InitialRotation = 0x100 /* 22.5 deg */;
    } else {
        frame.frameNumber = frame.frameNumber;
        if (frame.frameNumber < 0) {
            g_BattleMagicBolt3_InitialRotation = frame.frameNumber * 0x2400 / 0 + 0x100;
        } else {
            frame.frameNumber = frame.frameNumber;
            if (frame.frameNumber < 1) {
                g_BattleMagicBolt3_InitialRotation = 0x2500 /* 832.5 deg */;
            } else {
                frame.frameNumber--;
                if (frame.frameNumber < 8) {
                    g_BattleMagicBolt3_InitialRotation = ((frame.frameNumber * 0) / 8) + 0x2500;
                    frame.tintB = 0x80 - ((frame.frameNumber << 7) / 8);
                    frame.tintG = frame.tintB;
                    frame.tintR = frame.tintG;
                } else {
                    frame.flashSlot->ownerSlot = BATTLE_TASK_FREE;
                    return;
                }
            }
        }
    }
    BattleFx_MakeViewSpacePointPushed((char *)frame.flashSlot + 8, frame.flashSlot->ringIndex, (Vec3sPacked *)&g_BattleMagicBolt3_InitialRotation);
    SetRotMatrix(&g_BattleMagicBolt3_InitialRotation);
    SetTransMatrix(&g_BattleMagicBolt3_InitialRotation);
    g_BattleMagicBolt3_ChainTail = BattleFx_SubmitSpriteDraw(&frame.loadedRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicBolt3_ChainTail);
    if (g_BattleIsPaused == 0) {
        (frame.flashSlot->frameIndex)++;
    }
}

/* Updates and renders bouncing rock/stone models for Bolt 3 (Thunder3). */
// FUNCTION: FF7 0x005b7e00
void __cdecl BattleFx_MagicBolt3BouncingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[60];
        SVECTOR angleX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleFxBolt3MultiBounceTask *bounceTask; };
        struct { unsigned char pad_to_2C[0x2C]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.bounceTask = (BattleFxBolt3MultiBounceTask *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.bounceTask->ownerSlot < 1) {
        g_BattleMagicBolt3_ModelReq1.flags = 0x80;
        g_BattleMagicBolt3_ModelReq1.scaleFraction = 0;
        g_BattleMagicBolt3_ModelReq1.modelHandle = Gfx_ModelTableGetRenderObject(1, g_BattleMagicBolt3_ResourceHandle);
    } else {
        g_BattleMagicBolt3_ModelReq1.flags = 0x88;
        g_BattleMagicBolt3_ModelReq1.scaleFraction = ((frame.bounceTask->frameCount << 12) / 8);
        g_BattleMagicBolt3_ModelReq1.modelHandle = Gfx_ModelTableGetRenderObject(2, g_BattleMagicBolt3_ResourceHandle);
    }
    if (g_pBattle3dScene != 0) {
        (g_BattleMagicBolt3_ModelReq2.modelRequest) = g_pBattle3dScene->modelRequest;
    } else {
        (g_BattleMagicBolt3_ModelReq2.modelRequest) = 0;
    }
    frame.angleX.vx = frame.bounceTask->angleX << 4;
    frame.angleX.vy = frame.bounceTask->angleY << 4;
    frame.angleX.vz = frame.bounceTask->angleZ << 4;
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.bounceTask->posX;
    frame.rotMatrix.t[1] = frame.bounceTask->posY;
    frame.rotMatrix.t[2] = frame.bounceTask->posZ;
    frame.scaleVec.vz = frame.bounceTask->scale;
    frame.scaleVec.vy = frame.scaleVec.vz;
    frame.scaleVec.vx = frame.scaleVec.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleVec);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleMagicBolt3_ChainTail = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicBolt3_ModelReq1.modelHandle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicBolt3_ChainTail);
    g_BattleMagicBolt3_ModelReq3.transX = frame.bounceTask->posX;
    g_BattleMagicBolt3_ModelReq3.transY = 0;
    g_BattleMagicBolt3_ModelReq3.transZ = frame.bounceTask->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, (int *)&g_BattleMagicBolt3_ModelReq3, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    g_BattleMagicBolt3_ModelReq2.alpha = g_BattleMagicBolt3_ModelReq1.scaleFraction;
    g_BattleMagicBolt3_ChainTail = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicBolt3_ModelReq2, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_2, 1, g_BattleMagicBolt3_ChainTail);
    if (g_BattleIsPaused == 0) {
        if (frame.bounceTask->ownerSlot >= 1) {
            (frame.bounceTask->frameCount)++;
            if (frame.bounceTask->frameCount >= 8) {
                frame.bounceTask->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
        frame.bounceTask->posX += frame.bounceTask->velocityX;
        frame.bounceTask->posY += frame.bounceTask->velocityY;
        frame.bounceTask->posZ += frame.bounceTask->velocityZ;
        frame.bounceTask->velocityY += 30;
        if (frame.bounceTask->posY > 0) {
            frame.bounceTask->posY = -frame.bounceTask->posY;
            frame.bounceTask->velocityY = (-frame.bounceTask->velocityY) / 4;
            frame.bounceTask->velocityX >>= 1;
            frame.bounceTask->velocityZ >>= 1;
            frame.bounceTask->angleStepX >>= 1;
            frame.bounceTask->angleStepY >>= 1;
            frame.bounceTask->angleStepZ >>= 1;
            (frame.bounceTask->ownerSlot)++;
        }
        *(char *)&frame.bounceTask->angleX += *(char *)&frame.bounceTask->angleStepX;
        *(char *)&frame.bounceTask->angleY += *(char *)&frame.bounceTask->angleStepY;
        *(char *)&frame.bounceTask->angleZ += *(char *)&frame.bounceTask->angleStepZ;
    }
}

/* Updates and renders a billboard lightning sprite for Bolt 3 (Thunder3). */
// FUNCTION: FF7 0x005b8175
void __cdecl BattleFx_MagicBolt3BillboardSpriteTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicBolt3_ModelReq3.spriteRecord = Gfx_ModelTableGetLoadedRecord(3, g_BattleMagicBolt3_ResourceHandle);
    g_BattleMagicBolt3_ModelReq3.spriteObject = Gfx_ModelTableGetSpriteObject(3, g_BattleMagicBolt3_ResourceHandle);
    g_BattleMagicBolt3_ModelReq3.spriteFrame = task->frameCounter >> 8;
    BattleFx_InstallBillboardMatrix((char *)task + 8, 0x2000 /* 2.0 */, 0);
    g_BattleMagicBolt3_ChainTail = BattleFx_SubmitSpriteDraw(&g_BattleMagicBolt3_ModelReq3.spriteRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicBolt3_ChainTail);
    if (g_BattleIsPaused == 0) {
        task->frameCounter += 256;
        if (g_BattleMagicBolt3_ModelReq3.spriteRecord != 0 && task->frameCounter >> 8 >= *(int *)((char *)*(int *)g_BattleMagicBolt3_ModelReq3.spriteRecord + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/* Updates and renders a growing and fading model for Bolt 3 (Thunder3). */
// FUNCTION: FF7 0x005b824d
void __cdecl BattleFx_MagicBolt3GrowFadeModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[60];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; int phase; };
        struct { unsigned char pad_to_24[0x24]; BattleFxBolt3GrowModel *growModel; };
        struct { unsigned char pad_to_28[0x28]; int fadeTimer; };
        struct { unsigned char pad_to_2C[0x2C]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.growModel = (BattleFxBolt3GrowModel *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicBolt3_ModelReq3.shellModel = Gfx_ModelTableGetRenderObject(4, g_BattleMagicBolt3_ResourceHandle);
    frame.phase = frame.growModel->timer & 0xF;
    g_BattleMagicBolt3_ModelReq3.shellUvOffset = (frame.phase & 1) << 7 | (frame.phase >> 1 & 3) << 6 << 8;
    g_BattleMagicBolt3_ModelReq3.shellBlendMode = (frame.phase >> 3) | 0x20;
    frame.scaleVec.vz = frame.growModel->timer * 0x2800 / 23;
    frame.scaleVec.vy = frame.scaleVec.vz;
    frame.scaleVec.vx = frame.scaleVec.vy;
    frame.fadeTimer = frame.growModel->timer;
    if (frame.fadeTimer < 15) {
        g_BattleMagicBolt3_ModelReq3.shellAlpha = 0x80;
    } else {
        frame.fadeTimer -= 15;
        if (frame.fadeTimer < 8) {
            g_BattleMagicBolt3_ModelReq3.shellAlpha = ((frame.fadeTimer * -0x80) / 8) + 0x80;
        } else {
            frame.growModel->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    RotMatrix((SVECTOR *)((int)frame.growModel + 0x10), &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.growModel->posX;
    frame.rotMatrix.t[1] = frame.growModel->posY;
    frame.rotMatrix.t[2] = frame.growModel->posZ;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleVec);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    g_BattleMagicBolt3_ChainTail = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicBolt3_ModelReq3.shellModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicBolt3_ChainTail);
    if (g_BattleIsPaused == 0) {
        frame.growModel->rotX = frame.growModel->rotX;
        frame.growModel->rotY = frame.growModel->rotY;
        frame.growModel->rotZ = frame.growModel->rotZ;
        (frame.growModel->timer)++;
    }
}

/* Updates and renders a growing, holding, and fading model for Bolt 3 (Thunder3). */
// FUNCTION: FF7 0x005b840b
void __cdecl BattleFx_MagicBolt3GrowHoldFadeModelTask(void)
{
    int angle;
    int localFrame;
    BattleFxBolt3MultiGrowRecord *selfPtr;
    int matrix[8];

    selfPtr = (BattleFxBolt3MultiGrowRecord *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicBolt3_ModelReq3.coreModel = Gfx_ModelTableGetRenderObject(5, g_BattleMagicBolt3_ResourceHandle);
    localFrame = selfPtr->frame;
    if (localFrame < 8) {
        g_BattleMagicBolt3_ModelReq3.coreAlpha = 0x80;
    } else {
        localFrame -= 8;
        if (localFrame < 8) {
            g_BattleMagicBolt3_ModelReq3.coreAlpha = (localFrame * -0x80 / 8) + 0x80;
        } else {
            selfPtr->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    angle = selfPtr->frame * 0x6000 / 16;
    g_BattleMagicBolt3_SpriteReq.scaleZ = angle;
    (g_BattleMagicBolt3_SpriteReq.scaleX) = g_BattleMagicBolt3_SpriteReq.scaleZ;
    g_BattleMagicBolt3_SpriteReq.scaleY = -angle;
    g_BattleMagicBolt3_SpriteReq.transX = selfPtr->offsetX;
    g_BattleMagicBolt3_SpriteReq.transY = selfPtr->offsetY;
    g_BattleMagicBolt3_SpriteReq.transZ = selfPtr->offsetZ;
    MultiplyMatrix(&g_BattleCameraTransform, (short *)&g_BattleMagicBolt3_SpriteReq, matrix);
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    g_BattleMagicBolt3_ChainTail = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicBolt3_ModelReq3.coreModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicBolt3_ChainTail);
    if (g_BattleIsPaused == 0) {
        (selfPtr->frame)++;
    }
}
} /* extern "C" */
