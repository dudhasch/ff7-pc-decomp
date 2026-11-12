// src/ff7/src/battle/battle_fx_magic_demi2.cpp
/* Magic Effect [43]: Demi2 (Japanese: グラビラ Gravira, dev tag: "MAGIC/GRAV2" / Grav2) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle visual effect for the Grav2 magic: orbiting trail sprites, contracting quad
//   sprites, rotating ring sprites and animated billboard sprites around each target.

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


#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x008f2ec0
int g_BattleMagicDemi2_ResourceDefs = 0x00192A38;
char *g_BattleFxMagicDemi2ResourceNames[1] = {
    // STRING: FF7 0x008f2f78
    "GRAVIR03.TIM"
};
unsigned int g_BattleFxMagicDemi2ResourceNames2[24] = {
    // STRING: FF7 0x008f2f88
    (unsigned int)"GRAV_2.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008f2f94
    (unsigned int)"GRAVIR00.TIM",
    // STRING: FF7 0x008f2fa4
    (unsigned int)"ELEC_4.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008f2fb0
    (unsigned int)"GRAVIR01.TIM",
    // STRING: FF7 0x008f2fc0
    (unsigned int)"ELEC_4.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008f2fcc
    (unsigned int)"GRAVIR02.TIM",
    // STRING: FF7 0x008f2fdc
    (unsigned int)"ELEC_4.SPT",
    0x00000000,
    0x00000000,
    0x00192a38,
    // STRING: FF7 0x008f2fe8
    (unsigned int)"GRAVIR03.TIM",
    // STRING: FF7 0x008f2ff8
    (unsigned int)"GRAV_2.SPT",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x00bd5e60
int g_BattleMagicDemi2_DrawListHandle = 0;
// GLOBAL: FF7 0x00bd5e64
struct GfxTableCursor *g_BattleMagicDemi2_LoadedRes = 0;
// clang-format on

// clang-format off
#undef g_BattleMagicDemi2_SpriteReq
// GLOBAL: FF7 0x008f2f28
struct BattleFxSpriteRequest g_BattleMagicDemi2_SpriteReq = {0, 0, -128, -128, -128, 0x2e, 0, 0};
// GLOBAL: FF7 0x008f2f38
unsigned char g_BattleMagicDemi2_QuadReq1[8] = {0xc0, 0xff, 0xc0, 0xff, 0x00, 0x40, 0x7f, 0x7f};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008f2f40
struct BattleMagicRemove_SpriteData g_BattleMagicDemi2_SpriteData1 = {-128, -128, -128, {0x2e, 0x4f, 0, 0xc8, 0x78}, 0, {0}};
// GLOBAL: FF7 0x008f2f50
unsigned char g_BattleMagicDemi2_QuadReq2[8] = {0xf0, 0xff, 0xf0, 0xff, 0x80, 0x40, 0x1f, 0x1f};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008f2f58
struct BattleMagicDemi2_SpriteData2 g_BattleMagicDemi2_SpriteData2 = {-128, -128, -128, {0x2e, 0x2f, 0, 0x48, 0x79}, 0, {0}, 0, 0};
// GLOBAL: FF7 0x008f2f70
struct BattleMagicDemi2_SpriteData3 g_BattleMagicDemi2_SpriteData3 = {-128, -128, -128, {0x2e}, 0, {0}};
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/*
 * Loads resources for the Grav2 magic effect when not already loaded.
 */
// FUNCTION: FF7 0x005aa910
void __cdecl BattleFx_MagicLoadResDemi2(void)
{
    GfxModelTableBank *context;
    GfxResourceRequest requestBuffer;
    int resultType;
    char pathBuffer[200];
    GfxResourceRequest *request;
    int defsGravTbl;

    if (g_pBattle3dScene != 0 && g_BattleMagicDemi2_LoadedRes == 0) {
        resultType = 5;
        defsGravTbl = (int)&g_BattleMagicDemi2_ResourceDefs;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestBuffer;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x008f3004
        request->resourcePath = BattleFx_BuildResourcePath("MAGIC/GRAV2", pathBuffer);
        Gfx_LoadModelTableIfAbsent(resultType, defsGravTbl, request, &g_BattleMagicDemi2_LoadedRes, context);
    }
}

/*
 * Main entry point initializing render state and per-target effects for Grav2.
 */
// FUNCTION: FF7 0x005aa9a0
void __cdecl BattleFx_MagicEnterDemi2(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResDemi2();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 8, BattleFx_MagicSpawnDemi2PerTarget);
}

/*
 * Spawns timeline task for a single Grav2 target.
 */
// FUNCTION: FF7 0x005aa9c1
void __cdecl BattleFx_MagicSpawnDemi2PerTarget(int actor, int param)
{
    BattleFxOrbitCloudTask *effect;

    effect = (BattleFxOrbitCloudTask *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDemi2TimelineTask);
    effect->targetIndex = actor;
    effect->param = param;
}

/*
 * Drives the main timeline and sprite generation for Grav2.
 */
// FUNCTION: FF7 0x005aa9fa
void __cdecl BattleFx_MagicDemi2TimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[16];
        BattleFxDemi2Slot *particle;
        struct { unsigned char pad_to_4[0x4]; BattleFxMagicDemi2Timeline *timeline; };
        struct { unsigned char pad_to_8[0x8]; int boneX; };
        struct { unsigned char pad_to_A[0xA]; short boneY; };
        struct { unsigned char pad_to_C[0xC]; int boneZ; };
    } frame;
#pragma pack(pop)

    frame.timeline = (BattleFxMagicDemi2Timeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timeline->step == 0) {
        frame.particle = (BattleFxDemi2Slot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDemi2TrailEmitterTask);
        Battle_GetBoneWorldPos(frame.timeline->targetIndex, g_BattleActorRecords[frame.timeline->targetIndex].targetBone, (short *)&frame.particle->posX);
        Battle_GetBoneWorldPos(frame.timeline->actorIndex, g_BattleActorRecords[frame.timeline->actorIndex].targetBone, (short *)&frame.boneX);
        frame.particle->velocityX = (*(short *)&frame.boneX - frame.particle->posX) / 8;
        frame.particle->velocityY = (frame.boneY - frame.particle->posY) / 8;
        frame.particle->velocityZ = (*(short *)&frame.boneZ - frame.particle->posZ) / 8;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&frame.particle->posX), 0x50);
    }
    if (frame.timeline->step == 8) {
        frame.particle = (BattleFxDemi2Slot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDemi2ShrinkingQuadSpriteTask);
        Battle_GetBoneWorldPos(frame.timeline->actorIndex, g_BattleActorRecords[frame.timeline->actorIndex].targetBone, (short *)&frame.particle->posX);
        frame.particle->pushLength = -g_BattleActorRecords[frame.timeline->actorIndex].radius;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&frame.particle->posX), 0x4B);
    }
    if (frame.timeline->step == 8) {
        Battle_GetBoneWorldPos(frame.timeline->actorIndex, g_BattleActorRecords[frame.timeline->actorIndex].targetBone, (short *)&frame.boneX);
        frame.particle = (BattleFxDemi2Slot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDemi2SpriteRingTask);
        frame.particle->pos = *(Int64Pair *)(&frame.boneX);
        frame.particle->pushLength = -g_BattleActorRecords[frame.timeline->actorIndex].radius;
        frame.particle->velocityZ = 0;
        frame.particle->velocityY = 0;
        frame.particle->velocityX = 0;
        frame.particle = (BattleFxDemi2Slot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDemi2SpriteRingTask);
        frame.particle->pos = *(Int64Pair *)(&frame.boneX);
        frame.particle->pushLength = -g_BattleActorRecords[frame.timeline->actorIndex].radius;
        frame.particle->velocityX = 0;
        frame.particle->velocityY = 0x400;
        frame.particle->velocityZ = 0;
    }
    if (frame.timeline->step == 8) {
        frame.particle = (BattleFxDemi2Slot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDemi2AnimatedSpriteTask);
        Battle_GetBoneWorldPos(frame.timeline->actorIndex, g_BattleActorRecords[frame.timeline->actorIndex].targetBone, (short *)&frame.particle->posX);
        frame.particle->pushLength = -g_BattleActorRecords[frame.timeline->actorIndex].radius;
    }
    if (frame.timeline->step == 8) {
        Battle_PostActorHitOrHurtMessage(frame.timeline->actorIndex);
    }
    if (frame.timeline->step >= 8) {
        frame.timeline->ownerSlot = BATTLE_TASK_FREE;
    }
    frame.timeline->step++;
}

/*
 * Spawns and manages orbiting trail sprite particles for Grav2.
 */
// FUNCTION: FF7 0x005aad5c
void __cdecl BattleFx_MagicDemi2TrailEmitterTask(void)
{
    BattleFxDemi2Slot *taskPtr;
    BattleFxDemi2Slot *sprite;

    taskPtr = (BattleFxDemi2Slot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    taskPtr->posX += taskPtr->velocityX;
    taskPtr->posY += taskPtr->velocityY;
    taskPtr->posZ += taskPtr->velocityZ;
    sprite = (BattleFxDemi2Slot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDemi2TrailSpriteTask);
    sprite->pos = taskPtr->pos;
    taskPtr->frameCount++;
    if (taskPtr->frameCount >= 8) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates and renders trailing particle sprites for Grav2.
 */
// FUNCTION: FF7 0x005aae1c
void __cdecl BattleFx_MagicDemi2TrailSpriteTask(void)
{
    BattleFxDemi2Slot *taskPtr;
    PsxMatrix *billboard;

    taskPtr = (BattleFxDemi2Slot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicDemi2_SpriteReq.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleMagicDemi2_LoadedRes);
    g_BattleMagicDemi2_SpriteReq.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleMagicDemi2_LoadedRes);
    billboard = (PsxMatrix *)BattleFx_InstallBillboardMatrix(&taskPtr->posX, 0xD00, 0);
    if (billboard->t[2] > 0) {
        g_BattleMagicDemi2_SpriteReq.frame = taskPtr->frameCount;
        g_BattleMagicDemi2_DrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleMagicDemi2_SpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicDemi2_DrawListHandle);
    }
    if (g_BattleIsPaused == 0) {
        taskPtr->frameCount++;
        if (taskPtr->frameCount >= 5) {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Updates and renders contracting quad sprites for Grav2.
 */
// FUNCTION: FF7 0x005aaee9
void __cdecl BattleFx_MagicDemi2ShrinkingQuadSpriteTask(void)
{
    int size;
    int phaseFrame;
    BattleFxDemi2Slot *task;
    PsxMatrix *matrix;

    task = (BattleFxDemi2Slot *)BATTLE_CURRENT_TASK_SLOT;
    size = task->frameCount * -0x3000 / 0x28 + 0x3000;
    phaseFrame = task->frameCount;
    if (phaseFrame < 0x14) {
        g_BattleMagicDemi2_SpriteData1.colorB = (phaseFrame << 7) / 0x14;
        g_BattleMagicDemi2_SpriteData1.colorG = g_BattleMagicDemi2_SpriteData1.colorB;
        (g_BattleMagicDemi2_SpriteData1.colorR) = g_BattleMagicDemi2_SpriteData1.colorG;
    } else {
        phaseFrame -= 0x14;
        if (phaseFrame < 0x14) {
            g_BattleMagicDemi2_SpriteData1.colorB = 0x80;
            g_BattleMagicDemi2_SpriteData1.colorG = g_BattleMagicDemi2_SpriteData1.colorB;
            (g_BattleMagicDemi2_SpriteData1.colorR) = g_BattleMagicDemi2_SpriteData1.colorG;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    g_BattleMagicDemi2_SpriteData1.drawObj = Gfx_ModelTableGetSpriteObject(4, g_BattleMagicDemi2_LoadedRes);
    matrix = (PsxMatrix *)BattleFx_InstallBillboardMatrix(&task->posX, (short)size, task->pushLength);
    if (matrix->t[2] > 0) {
        g_BattleMagicDemi2_DrawListHandle = BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)&g_BattleMagicDemi2_QuadReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicDemi2_DrawListHandle);
    }
    if (g_BattleIsPaused == 0) {
        task->frameCount++;
    }
}

/*
 * Updates and renders rotating ring sprites for Grav2.
 */
// FUNCTION: FF7 0x005ab018
void __cdecl BattleFx_MagicDemi2SpriteRingTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[52];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; int spriteIndex; };
        struct { unsigned char pad_to_24[0x24]; BattleFxDemi2RingSlot *ringSlot; };
        struct { unsigned char pad_to_28[0x28]; SVECTOR offsetX; };
        struct { unsigned char pad_to_30[0x30]; int phase; };
    } frame;
#pragma pack(pop)

    frame.ringSlot = (BattleFxDemi2RingSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.phase = frame.ringSlot->step;
    if (frame.phase < 5) {
        g_BattleMagicDemi2_SpriteData2.colorB = (frame.phase << 7) / 5;
        g_BattleMagicDemi2_SpriteData2.colorG = g_BattleMagicDemi2_SpriteData2.colorB;
        (g_BattleMagicDemi2_SpriteData2.colorR) = g_BattleMagicDemi2_SpriteData2.colorG;
    } else {
        frame.phase -= 5;
        if (frame.phase < 0xF) {
            g_BattleMagicDemi2_SpriteData2.colorB = 0x80;
            g_BattleMagicDemi2_SpriteData2.colorG = g_BattleMagicDemi2_SpriteData2.colorB;
            (g_BattleMagicDemi2_SpriteData2.colorR) = g_BattleMagicDemi2_SpriteData2.colorG;
        } else {
            frame.phase -= 0xF;
            if (frame.phase < 5) {
                g_BattleMagicDemi2_SpriteData2.colorB = frame.phase * -0x80 / 5 + 0x80;
                g_BattleMagicDemi2_SpriteData2.colorG = g_BattleMagicDemi2_SpriteData2.colorB;
                (g_BattleMagicDemi2_SpriteData2.colorR) = g_BattleMagicDemi2_SpriteData2.colorG;
            } else {
                frame.ringSlot->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    g_BattleMagicDemi2_SpriteData2.drawObj = Gfx_ModelTableGetSpriteObject(4, g_BattleMagicDemi2_LoadedRes);
    for (frame.spriteIndex = 0; frame.spriteIndex < 0x10; frame.spriteIndex++) {
        frame.offsetX.vx = SinFixed(((frame.spriteIndex << 0xC) / 16)) * 0x1F4 >> 0xC;
        frame.offsetX.vy = CosFixed(((frame.spriteIndex << 0xC) / 16)) * 0x1F4 >> 0xC;
        frame.offsetX.vz = 0;
        RotMatrix(&frame.ringSlot->rot, &frame.rotMatrix);
        TransformVectorToInt16(&frame.rotMatrix, &frame.offsetX, &frame.offsetX.vx);
        frame.offsetX.vx += frame.ringSlot->spinX;
        frame.offsetX.vy += frame.ringSlot->spinY;
        frame.offsetX.vz += frame.ringSlot->spinZ;
        BattleFx_InstallBillboardMatrix(&frame.offsetX, 0x600, frame.ringSlot->pushLength);
        g_BattleMagicDemi2_DrawListHandle = BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)&g_BattleMagicDemi2_QuadReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicDemi2_DrawListHandle);
    }
    if (g_BattleIsPaused == 0) {
        frame.ringSlot->rot.vx += 0x50;
        frame.ringSlot->rot.vy += 0x40;
        frame.ringSlot->rot.vz += 0x50;
        frame.ringSlot->step++;
    }
}

/*
 * Updates animated billboard sprites for Grav2.
 */
// FUNCTION: FF7 0x005ab255
void __cdecl BattleFx_MagicDemi2AnimatedSpriteTask(void)
{
    int phaseFrame;
    BattleFxDemi2Slot *task;

    task = (BattleFxDemi2Slot *)BATTLE_CURRENT_TASK_SLOT;
    phaseFrame = task->frameCount;
    if (phaseFrame < 5) {
        g_BattleMagicDemi2_SpriteData3.colorB = (phaseFrame << 7) / 5;
        g_BattleMagicDemi2_SpriteData3.colorG = g_BattleMagicDemi2_SpriteData3.colorB;
        (g_BattleMagicDemi2_SpriteData3.colorR) = g_BattleMagicDemi2_SpriteData3.colorG;
    } else {
        phaseFrame -= 5;
        if (phaseFrame < 0x23) {
            g_BattleMagicDemi2_SpriteData3.colorB = 0x80;
            g_BattleMagicDemi2_SpriteData3.colorG = g_BattleMagicDemi2_SpriteData3.colorB;
            (g_BattleMagicDemi2_SpriteData3.colorR) = g_BattleMagicDemi2_SpriteData3.colorG;
        } else {
            phaseFrame -= 0x23;
            if (phaseFrame < 5) {
                g_BattleMagicDemi2_SpriteData3.colorB = phaseFrame * -0x80 / 5 + 0x80;
                g_BattleMagicDemi2_SpriteData3.colorG = g_BattleMagicDemi2_SpriteData3.colorB;
                (g_BattleMagicDemi2_SpriteData3.colorR) = g_BattleMagicDemi2_SpriteData3.colorG;
            } else {
                task->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    g_BattleMagicDemi2_SpriteData2.fadingRecord = Gfx_ModelTableGetLoadedRecord(1, g_BattleMagicDemi2_LoadedRes);
    g_BattleMagicDemi2_SpriteData2.fadingObj = Gfx_ModelTableGetSpriteObject(1, g_BattleMagicDemi2_LoadedRes);
    if (g_BattleMagicDemi2_SpriteData2.fadingRecord != 0) {
        ((BattleMagicDemi3SpriteData *)g_BattleMagicDemi2_SpriteData2.fadingRecord)->spriteObject1 = Gfx_ModelTableGetSpriteObject(1, g_BattleMagicDemi2_LoadedRes);
        ((BattleMagicDemi3SpriteData *)g_BattleMagicDemi2_SpriteData2.fadingRecord)->spriteObject2 = Gfx_ModelTableGetSpriteObject(2, g_BattleMagicDemi2_LoadedRes);
        ((BattleMagicDemi3SpriteData *)g_BattleMagicDemi2_SpriteData2.fadingRecord)->spriteObject3 = Gfx_ModelTableGetSpriteObject(3, g_BattleMagicDemi2_LoadedRes);
    }
    g_BattleMagicDemi2_SpriteData3.framePhase = task->frameCount % 0xC;
    BattleFx_InstallBillboardMatrix(&task->posX, 0x2000, task->pushLength);
    g_BattleMagicDemi2_DrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleMagicDemi2_SpriteData2.fadingRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicDemi2_DrawListHandle);
    if (g_BattleIsPaused == 0) {
        task->frameCount++;
    }
}
} /* extern "C" */
