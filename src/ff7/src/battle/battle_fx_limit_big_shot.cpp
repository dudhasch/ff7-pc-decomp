// src/ff7/src/battle/battle_fx_limit_big_shot.cpp
// Limit Break [7] (Barret Lv 1-1): Big Shot (Japanese: ヘビーショット Heavy Shot, dev tag: "LIMIT/HVSHOT" / HvShot)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Hvshot limit-break effect: loads its resources, registers a per-target callback, and
//   runs the timeline that fires projectile shells and muzzle, spin and impact sprites at each
//   target.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_limit.h"
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

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// GLOBAL: FF7 0x008f32b0
int g_BattleLimitBigShot_ResourceDefs = 0x001900B4;
int g_BattleLimitBigShot_ResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxLimitBigShotDataPool + 0x24)
};
int g_BattleLimitBigShot_ResourceDefsDataTail[14] = {
    0x0, 0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleFxLimitBigShotDataPool + 0x2c), 0x0,
    0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxLimitBigShotDataPool + 0x34), (int)((char *)&g_BattleFxLimitBigShotDataPool + 0x40), 0x0,
    0x0, 0x0
};
// GLOBAL: FF7 0x008f32f0
int g_BattleLimitBigShot_Scale = 4096;
unsigned char g_rawData_008f32f8[24] = {0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4c, 0xff, 0xff, 0xff, 0x00, 0x00};
// GLOBAL: FF7 0x008f3310
struct BattleLimitBigShot_ModelReq g_BattleLimitBigShot_ModelReq = {0, 0, {0, 0, 0x80, 0, 0, 0, 0, 0}};
// GLOBAL: FF7 0x008f3320
int g_BattleLimitBigShot_RotStep = 4608;
unsigned char g_rawData_008f3324[4] = {0x00, 0x12, 0x00, 0x00};
unsigned char g_rawData_008f3328[8] = {0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008f3330
int g_BattleLimitBigShot_QuadReq1 = (int)0xFFF0FFF0;
unsigned char g_rawData_008f3334[4] = {0x80, 0x00, 0x20, 0x20};
// clang-format off
// GLOBAL: FF7 0x008f3338
struct BattleLimitBigShot_QuadColor1 g_BattleLimitBigShot_QuadColor1 = {{0x80}, -128, -128, {0x2e}};
unsigned char g_rawData_008f333c[4] = {0x2c, 0x00, 0x08, 0x78};
// GLOBAL: FF7 0x008f3340
unsigned char g_BattleLimitBigShot_SpriteReq1[8] = {0x00, 0x00, 0x00, 0x00, 0xf4, 0xff, 0xf4, 0xff};
unsigned char g_rawData_008f3348[40] = {
    0x80, 0x20, 0x18, 0x18, 0x80, 0x80, 0x80, 0x2e, 0x2c, 0x00, 0x08, 0x78, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0xf8, 0xff, 0x80, 0x38, 0x10, 0x10,
    0x80, 0x80, 0x80, 0x2e, 0x2c, 0x00, 0x08, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// GLOBAL: FF7 0x008f3370
char g_BattleLimitBigShot_QuadReq2[8] = {(char)0xf0, (char)0xff, (char)0xf0, (char)0xff, (char)0xa0, 0x00, 0x20, 0x20};
// clang-format off
// GLOBAL: FF7 0x008f3378
struct BattleLimitBigShot_QuadColor2 g_BattleLimitBigShot_QuadColor2 = {{0x80}, -128, -128, 44};
// clang-format on
unsigned char g_rawData_008f337c[4] = {0x2c, 0x00, 0x48, 0x78};
// GLOBAL: FF7 0x008f3380
unsigned char g_BattleLimitBigShot_SpriteReq2[8] = {0x00, 0x00, 0x00, 0x00, 0xf4, 0xff, 0xf4, 0xff};
unsigned char g_rawData_008f3388[40] = {
    0x98, 0x20, 0x18, 0x18, 0x80, 0x80, 0x80, 0x2c, 0x2c, 0x00, 0x48, 0x78, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0xf8, 0xff, 0x90, 0x38, 0x10, 0x10,
    0x80, 0x80, 0x80, 0x2c, 0x2c, 0x00, 0x48, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008f33b0
char g_BattleLimitBigShot_QuadReq3[8] = {(char)0xf0, (char)0xff, (char)0xf0, (char)0xff, (char)0x80, 0x00, 0x20, 0x20};
// clang-format off
// GLOBAL: FF7 0x008f33b8
struct BattleLimitBigShot_QuadColor1 g_BattleLimitBigShot_QuadColor3 = {{0x80}, -128, -128, {0x2e}};
// clang-format on
unsigned char g_rawData_008f33bc[4] = {0x2c, 0x00, 0x08, 0x78};
// GLOBAL: FF7 0x008f33c0
unsigned char g_BattleLimitBigShot_SpriteReq3[8] = {0x00, 0x00, 0x00, 0x00, 0xf4, 0xff, 0xf4, 0xff};
unsigned char g_BattleFxLimitBigShotDataPool[76] = {
    0x80, 0x20, 0x18, 0x18, 0x80, 0x80, 0x80, 0x2e, 0x2c, 0x00, 0x08, 0x78, 0x00, 0x00, 0x00, 0x00,
    0xf8, 0xff, 0xf8, 0xff, 0x80, 0x38, 0x10, 0x10, 0x80, 0x80, 0x80, 0x2e, 0x2c, 0x00, 0x08, 0x78,
    0x00, 0x00, 0x00, 0x00, 0x48, 0x53, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x48, 0x53, 0x2e, 0x52,
    0x53, 0x44, 0x00, 0x00, 0x48, 0x56, 0x53, 0x48, 0x4f, 0x54, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00,
    0x4c, 0x49, 0x4e, 0x45, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00bd9e00
int g_BattleLimitBigShot_DrawListHandle = 0;
unsigned char g_bssPad_00bd9e04[4] = {0};
// One target-state entry per battle task slot. The span runs to the next named definition of the TU.
// GLOBAL: FF7 0x00bd9e08
struct BattleLimitBigShot_TargetStateBlob g_BattleLimitBigShot_TargetStateBlob = {0};
// GLOBAL: FF7 0x00bda448
struct GfxTableCursor *g_BattleLimitBigShot_LoadedRes = 0;
unsigned char g_bssPad_00bda44c[4] = {0};
// GLOBAL: FF7 0x00bda450
int g_BattleLimitBigShot_State1 = 0;
// GLOBAL: FF7 0x00bda454
short g_BattleLimitBigShot_Timer1 = 0;
// GLOBAL: FF7 0x00bda458
int g_BattleLimitBigShot_State2 = 0;
// GLOBAL: FF7 0x00bda45c
short g_BattleLimitBigShot_Timer2 = 0;
// clang-format on


/*
 * Loads resources for the Hvshot limit effect when not already loaded.
 */
// FUNCTION: FF7 0x005ad5c0
void __cdecl BattleFx_LimitLoadResBigShot(void)
{
    GfxModelTableBank *context;
    GfxResourceRequest requestBuffer;
    int kind;
    char pathBuffer[200];
    GfxResourceRequest *request;
    int defPtr;

    if (g_pBattle3dScene != 0 && g_BattleLimitBigShot_LoadedRes == 0) {
        kind = 3;
        defPtr = (int)&g_BattleLimitBigShot_ResourceDefs;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestBuffer;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x008f3414
        request->resourcePath = BattleFx_BuildResourcePath("LIMIT/HVSHOT", pathBuffer);
        Gfx_LoadModelTableIfAbsent(kind, defPtr, request, &g_BattleLimitBigShot_LoadedRes, context);
    }
}

/*
 * Entry point registering per-target callback for Hvshot.
 */
// FUNCTION: FF7 0x005ad650
int __cdecl Battle_LimitFx07_BigShot(int targetMask, int context)
{
    BattleFx_LimitLoadResBigShot();
    BattleFx_LimitRegisterBigShotPerTargetCallback(targetMask, context);
    return 0;
}

/*
 * Registers per-target callback for Hvshot.
 */
// FUNCTION: FF7 0x005ad66c
void __cdecl BattleFx_LimitRegisterBigShotPerTargetCallback(int targetMask, int context)
{
    Battle_CreateBitPatternCallbackTask(targetMask, context, 0, BattleFx_LimitSpawnBigShotPerTarget);
}

/*
 * Spawns timeline task for a single Hvshot target.
 */
// FUNCTION: FF7 0x005ad688
void __cdecl BattleFx_LimitSpawnBigShotPerTarget(int actor, int value)
{
    BattleFxBigShotTask *task;

    task = (BattleFxBigShotTask *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitBigShotTimelineTask);
    task->targetIndex = actor;
    task->sourceActor = value;
}

/*
 * Drives the main timeline and projectile firing for Hvshot.
 */
// FUNCTION: FF7 0x005ad6c1
void __cdecl BattleFx_LimitBigShotTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        SVECTOR angleX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix actorMatrix; };
        struct { unsigned char pad_to_48[0x48]; int pointX; };
        struct { unsigned char pad_to_4C[0x4C]; int pointY; };
        struct { unsigned char pad_to_50[0x50]; int pointZ; };
        struct { unsigned char pad_to_58[0x58]; BattleFxBigShotParticle *particle; };
        struct { unsigned char pad_to_5C[0x5C]; int i; };
        struct { unsigned char pad_to_60[0x60]; SVECTOR offsetX; };
        struct { unsigned char pad_to_68[0x68]; int pointDepth; };
        struct { unsigned char pad_to_6C[0x6C]; BattleFxBigShotTimelineRecord *timeline; };
        struct { unsigned char pad_to_70[0x70]; int slotIndex; };
        struct { unsigned char pad_to_74[0x74]; int timer; };
    } frame;
#pragma pack(pop)

    frame.timeline = (BattleFxBigShotTimelineRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.timer = frame.timeline->timer;
    if (frame.timer < 0) { /* empty first arm: lib/codegen.h */
    } else {
        frame.timer = frame.timer;
        if (frame.timer < 0x16) {
            if (frame.timer == 0) {
                frame.particle = (BattleFxBigShotParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitBigShotModelPairTask);
                frame.particle->actorSlot = frame.timeline->actorSlot;
                frame.particle->sourceActor = frame.timeline->sourceActor;
                Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[frame.timeline->sourceActor].posX), 0x28);
            }
            if (frame.timer < 0xE) {
                Battle_ComposeMatrixRelativeToCamera((short *)&g_BattleCameraTransform, (void *)((frame.timeline->sourceActor << 6) + (int)g_BattleActorTransformRecords[0].auxNode), &frame.actorMatrix);
                for (frame.i = 0; frame.i < 2; frame.i++) {
                    frame.particle = (BattleFxBigShotParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitBigShotRampHoldSpriteTask);
                    frame.offsetX.vy = 0;
                    frame.offsetX.vx = frame.offsetX.vy;
                    frame.offsetX.vz = -(rand() % 0x3E8 + 0x3E8);
                    frame.angleX.vx = (rand() & 0x3FF) - 0x200;
                    frame.angleX.vy = (rand() & 0x3FF) - 0x200;
                    frame.angleX.vz = 0;
                    RotMatrix(&frame.angleX, &frame.rotMatrix);
                    frame.rotMatrix.t[2] = 0xFFFFFF4C;
                    frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
                    frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
                    MultiplyMatrix(&frame.actorMatrix, &frame.rotMatrix, &frame.rotMatrix);
                    SetRotMatrix(&frame.rotMatrix);
                    SetTransMatrix(&frame.rotMatrix);
                    RotTransPoint(&frame.offsetX, &frame.pointX, &frame.pointDepth);
                    frame.particle->posX = *(short *)&frame.pointX;
                    frame.particle->posY = *(short *)&frame.pointY;
                    frame.particle->posZ = *(short *)&frame.pointZ;
                    frame.particle->velocityX = (frame.rotMatrix.t[0] - frame.pointX) / 8;
                    frame.particle->velocityY = (frame.rotMatrix.t[1] - frame.pointY) / 8;
                    frame.particle->velocityZ = (frame.rotMatrix.t[2] - frame.pointZ) / 8;
                    frame.particle->variant = rand() % 3;
                    frame.particle->scale = 0x1000;
                }
            }
        } else {
            frame.timer -= 0x16;
            if (frame.timer < 7) {
                if (frame.timer == 0) {
                    BattleFx_FadeOutScreenFlashRamp(2);
                    Battle_PostActorHitOrHurtMessage(frame.timeline->actorSlot);
                    for (frame.i = 0; frame.i < 0x32; frame.i++) {
                        frame.slotIndex = (short)Battle_AllocateResourceSlot(BattleFx_LimitBigShotSpinningSpriteTask);
                        frame.particle = (BattleFxBigShotParticle *)&g_BattleTaskSlotBuffer.mainSlots[frame.slotIndex];
                        Battle_GetBoneWorldPos(frame.timeline->actorSlot, g_BattleActorRecords[frame.timeline->actorSlot].targetBone, &frame.particle->posX);
                        frame.offsetX.vy = 0;
                        frame.offsetX.vx = frame.offsetX.vy;
                        frame.offsetX.vz = 0xFFFFFF9C - rand() % 0xC8;
                        frame.angleX.vx = (rand() & 0x7FF) - 0x400;
                        frame.angleX.vy = g_BattleActorRecords[frame.timeline->sourceActor].yaw + (rand() & 0x7FF) + 0x400;
                        frame.angleX.vz = 0;
                        RotMatrix(&frame.angleX, &frame.rotMatrix);
                        TransformVectorToInt16(&frame.rotMatrix, &frame.offsetX, &frame.particle->velocityX);
                        frame.particle->scale = rand() % 0x1800 + 0x800;
                        frame.particle->variant = rand() % 3;
                        g_BattleLimitBigShot_TargetStateBlob.entries[frame.slotIndex].angle = rand() & 0xFFF;
                        g_BattleLimitBigShot_TargetStateBlob.entries[frame.slotIndex].angleX = rand() & 0xFFF;
                        g_BattleLimitBigShot_TargetStateBlob.entries[frame.slotIndex].angleY = rand() & 0xFFF;
                        g_BattleLimitBigShot_TargetStateBlob.entries[frame.slotIndex].radius = rand() % 0x96 + 0x32;
                        g_BattleLimitBigShot_TargetStateBlob.entries[frame.slotIndex].speedX = rand() % 0x96 + 0x32;
                        g_BattleLimitBigShot_TargetStateBlob.entries[frame.slotIndex].speedY = rand() % 0x96 + 0x32;
                    }
                    for (frame.i = 0; frame.i < 0x1E; frame.i++) {
                        frame.particle = (BattleFxBigShotParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitBigShotHoldFadeSpriteTask);
                        Battle_GetBoneWorldPos(frame.timeline->actorSlot, g_BattleActorRecords[frame.timeline->actorSlot].targetBone, &frame.particle->posX);
                        frame.offsetX.vy = 0;
                        frame.offsetX.vx = frame.offsetX.vy;
                        frame.offsetX.vz = 0xFFFFFE0C - rand() % 0x12C;
                        frame.angleX.vx = (rand() & 0x7FF) - 0x400;
                        frame.angleX.vy = g_BattleActorRecords[frame.timeline->sourceActor].yaw + (rand() & 0x7FF) + 0x400;
                        frame.angleX.vz = 0;
                        RotMatrix(&frame.angleX, &frame.rotMatrix);
                        TransformVectorToInt16(&frame.rotMatrix, &frame.offsetX, &frame.particle->velocityX);
                        frame.particle->scale = rand() % 0x1800 + 0x800;
                        frame.particle->variant = rand() % 3;
                    }
                }
            } else {
                frame.timer -= 7;
                if (frame.timer < 0xF) { /* empty first arm: lib/codegen.h */
                } else {
                    frame.timeline->ownerSlot = BATTLE_TASK_FREE;
                }
            }
        }
    }
    (frame.timeline->timer)++;
}

/*
 * Animates and renders projectile shell models for Hvshot.
 */
// FUNCTION: FF7 0x005adc6e
void __cdecl BattleFx_LimitBigShotModelPairTask(void)
{
    VECTOR growZ;
    int frameIdx;
    BattleFxBigShotModelPair *selfPtr;
    PsxMatrix matrix;

    selfPtr = (BattleFxBigShotModelPair *)BATTLE_CURRENT_TASK_SLOT;
    frameIdx = selfPtr->frameIndex;
    if (frameIdx < 0x16) {
        growZ.vz = frameIdx * 0xE00 / 0x16 + 0x200;
        growZ.vy = growZ.vz;
        growZ.vx = growZ.vy;
        if (g_BattleIsPaused == 0) {
            g_BattleLimitBigShot_Timer1 += 0x280;
            g_BattleLimitBigShot_Timer2 -= 0x1C0;
        }
        RotMatrix((SVECTOR *)&g_BattleLimitBigShot_State1, &g_BattleLimitBigShot_Scale);
        ScaleMatrix(&g_BattleLimitBigShot_Scale, &growZ);
        MultiplyMatrix((void *)((selfPtr->matrixIndex << 6) + (char *)g_BattleActorTransformRecords[0].auxNode), &g_BattleLimitBigShot_Scale, &matrix);
        SetRotMatrix(&matrix);
        SetTransMatrix(&matrix);
        g_BattleLimitBigShot_ModelReq.mode = 0;
        g_BattleLimitBigShot_ModelReq.modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleLimitBigShot_LoadedRes);
        g_BattleLimitBigShot_DrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitBigShot_ModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitBigShot_DrawListHandle);
        RotMatrix((SVECTOR *)&g_BattleLimitBigShot_State2, &g_BattleLimitBigShot_Scale);
        ScaleMatrix(&g_BattleLimitBigShot_Scale, &growZ);
        MultiplyMatrix((void *)((selfPtr->matrixIndex << 6) + (char *)g_BattleActorTransformRecords[0].auxNode), &g_BattleLimitBigShot_Scale, &matrix);
        ScaleMatrix(&matrix, &g_BattleLimitBigShot_RotStep);
        SetRotMatrix(&matrix);
        g_BattleLimitBigShot_ModelReq.mode = 8;
        g_BattleLimitBigShot_ModelReq.modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleLimitBigShot_LoadedRes);
        g_BattleLimitBigShot_DrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitBigShot_ModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitBigShot_DrawListHandle);
        if (frameIdx == 0x15) {
            BattleFx_StartScreenFlashRamp(1);
            selfPtr->ownerSlot = BATTLE_TASK_FREE;
        }
    } else {
        selfPtr->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused == 0) {
        (selfPtr->frameIndex)++;
    }
}

/*
 * Updates and renders initial muzzle flash sprite for Hvshot.
 */
// FUNCTION: FF7 0x005ade78
void __cdecl BattleFx_LimitBigShotRampHoldSpriteTask(void)
{
    int age;
    BattleFxBigShotRampRecord *task;

    task = (BattleFxBigShotRampRecord *)BATTLE_CURRENT_TASK_SLOT;
    age = task->frameCount;
    if (age < 4) {
        *(char *)(&g_BattleLimitBigShot_QuadColor1.colorB + task->quadIndex * 0x14) = (age << 7) / 4;
        *(char *)(&g_BattleLimitBigShot_QuadColor1.colorG + task->quadIndex * 0x14) = *(char *)(&g_BattleLimitBigShot_QuadColor1.colorB + task->quadIndex * 0x14);
        *(char *)(((char *)&g_BattleLimitBigShot_QuadColor1) + task->quadIndex * 0x14) = *(char *)(&g_BattleLimitBigShot_QuadColor1.colorG + task->quadIndex * 0x14);
    } else {
        age -= 4;
        if (age < 4) {
            *(char *)(&g_BattleLimitBigShot_QuadColor1.colorB + task->quadIndex * 0x14) = 0x80;
            *(char *)(&g_BattleLimitBigShot_QuadColor1.colorG + task->quadIndex * 0x14) = 0x80;
            *(char *)(((char *)&g_BattleLimitBigShot_QuadColor1) + task->quadIndex * 0x14) = 0x80;
        } else {
            task->quadIndex = 0xFFFF;
            return;
        }
    }
    *(int *)((char *)&g_BattleLimitBigShot_SpriteReq1 + task->quadIndex * 0x14) = Gfx_ModelTableGetSpriteObject(2, g_BattleLimitBigShot_LoadedRes);
    BattleFx_InstallBillboardMatrix((void *)((char *)task + 8), (short)task->scale, 0xFFFFF830);
    g_BattleLimitBigShot_DrawListHandle = BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)(task->quadIndex * 0x14 + (char *)&g_BattleLimitBigShot_QuadReq1), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitBigShot_DrawListHandle);
    if (g_BattleIsPaused == 0) {
        (task->frameCount)++;
        task->posX += task->velX;
        task->posY += task->velY;
        task->posZ += task->velZ;
    }
}

/*
 * Updates and renders spinning projectile flare sprites for Hvshot.
 */
// FUNCTION: FF7 0x005ae014
void __cdecl BattleFx_LimitBigShotSpinningSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[60];
        PsxMatrix spriteMatrix;
        struct { unsigned char pad_to_20[0x20]; int screenDepth; };
        struct { unsigned char pad_to_24[0x24]; BattleFxBigShotSpriteRecord *sprite; };
        struct { unsigned char pad_to_28[0x28]; int fadeStep; };
        struct { unsigned char pad_to_2C[0x2C]; VECTOR scaleVector; };
    } frame;
#pragma pack(pop)

    frame.sprite = (BattleFxBigShotSpriteRecord *)BATTLE_CURRENT_TASK_SLOT;
    frame.fadeStep = frame.sprite->frameCount;
    if (frame.fadeStep < 0xE) {
        *(char *)(&g_BattleLimitBigShot_QuadColor2.colorB + frame.sprite->spriteIndex * 0x14) = 0x80;
        *(char *)(&g_BattleLimitBigShot_QuadColor2.colorG + frame.sprite->spriteIndex * 0x14) = 0x80;
        *(char *)(((char *)&g_BattleLimitBigShot_QuadColor2) + frame.sprite->spriteIndex * 0x14) = 0x80;
        *(char *)(&g_BattleLimitBigShot_QuadColor2.code + frame.sprite->spriteIndex * 0x14) = 0x2C;
    } else {
        frame.fadeStep -= 0xE;
        if (frame.fadeStep < 8) {
            *(char *)(&g_BattleLimitBigShot_QuadColor2.colorB + frame.sprite->spriteIndex * 0x14) = ((frame.fadeStep * -0x80) / 8) + 0x80;
            *(char *)(&g_BattleLimitBigShot_QuadColor2.colorG + frame.sprite->spriteIndex * 0x14) = *(char *)(&g_BattleLimitBigShot_QuadColor2.colorB + frame.sprite->spriteIndex * 0x14);
            *(char *)(((char *)&g_BattleLimitBigShot_QuadColor2) + frame.sprite->spriteIndex * 0x14) = *(char *)(&g_BattleLimitBigShot_QuadColor2.colorG + frame.sprite->spriteIndex * 0x14);
            *(char *)(&g_BattleLimitBigShot_QuadColor2.code + frame.sprite->spriteIndex * 0x14) = 0x2E;
        } else {
            frame.sprite->spriteIndex = 0xFFFF;
            return;
        }
    }
    RotMatrix((SVECTOR *)&g_BattleLimitBigShot_TargetStateBlob.entries[g_BattleActiveTaskSlotIdx], &frame.spriteMatrix);
    frame.scaleVector.vz = frame.sprite->scale;
    frame.scaleVector.vy = frame.scaleVector.vz;
    frame.scaleVector.vx = frame.scaleVector.vy;
    ScaleMatrix(&frame.spriteMatrix, &frame.scaleVector);
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((char *)frame.sprite + 8, frame.spriteMatrix.t, &frame.screenDepth);
    SetRotMatrix(&frame.spriteMatrix);
    SetTransMatrix(&frame.spriteMatrix);
    *(int *)((char *)&g_BattleLimitBigShot_SpriteReq2 + frame.sprite->spriteIndex * 0x14) = Gfx_ModelTableGetSpriteObject(2, g_BattleLimitBigShot_LoadedRes);
    g_BattleLimitBigShot_DrawListHandle = BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)(frame.sprite->spriteIndex * 0x14 + g_BattleLimitBigShot_QuadReq2), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitBigShot_DrawListHandle);
    if (g_BattleIsPaused == 0) {
        (frame.sprite->frameCount)++;
        frame.sprite->posX += frame.sprite->velocityX;
        frame.sprite->posY += frame.sprite->velocityY;
        frame.sprite->angle += frame.sprite->spinRate;
        frame.sprite->velocityY += 0xA;
        g_BattleLimitBigShot_TargetStateBlob.entries[g_BattleActiveTaskSlotIdx].angle += g_BattleLimitBigShot_TargetStateBlob.entries[g_BattleActiveTaskSlotIdx].radius;
        g_BattleLimitBigShot_TargetStateBlob.entries[g_BattleActiveTaskSlotIdx].angleX += g_BattleLimitBigShot_TargetStateBlob.entries[g_BattleActiveTaskSlotIdx].speedX;
        g_BattleLimitBigShot_TargetStateBlob.entries[g_BattleActiveTaskSlotIdx].angleY += g_BattleLimitBigShot_TargetStateBlob.entries[g_BattleActiveTaskSlotIdx].speedY;
        if (frame.sprite->posY > 0) {
            frame.sprite->spriteIndex = 0xFFFF;
        }
    }
}

/*
 * Updates and renders impact burst fade sprites for Hvshot.
 */
// FUNCTION: FF7 0x005ae30b
void __cdecl BattleFx_LimitBigShotHoldFadeSpriteTask(void)
{
    int age;
    BattleFxBigShotHoldFadeSprite *task;

    task = (BattleFxBigShotHoldFadeSprite *)BATTLE_CURRENT_TASK_SLOT;
    age = task->frame;
    if (age < 0xE) {
        *(char *)(&g_BattleLimitBigShot_QuadColor3.colorB + task->slot * 0x14) = 0x80;
        *(char *)(&g_BattleLimitBigShot_QuadColor3.colorG + task->slot * 0x14) = 0x80;
        *(char *)(((char *)&g_BattleLimitBigShot_QuadColor3) + task->slot * 0x14) = 0x80;
    } else {
        age -= 0xE;
        if (age < 8) {
            *(char *)(&g_BattleLimitBigShot_QuadColor3.colorB + task->slot * 0x14) = (age * -0x80 / 8) + 0x80;
            *(char *)(&g_BattleLimitBigShot_QuadColor3.colorG + task->slot * 0x14) = *(char *)(&g_BattleLimitBigShot_QuadColor3.colorB + task->slot * 0x14);
            *(char *)(((char *)&g_BattleLimitBigShot_QuadColor3) + task->slot * 0x14) = *(char *)(&g_BattleLimitBigShot_QuadColor3.colorG + task->slot * 0x14);
        } else {
            task->slot = 0xFFFF;
            return;
        }
    }
    BattleFx_InstallBillboardMatrix((void *)((char *)task + 8), (short)task->scale, 0);
    *(int *)((char *)&g_BattleLimitBigShot_SpriteReq3 + task->slot * 0x14) = Gfx_ModelTableGetSpriteObject(2, g_BattleLimitBigShot_LoadedRes);
    g_BattleLimitBigShot_DrawListHandle = BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)(task->slot * 0x14 + g_BattleLimitBigShot_QuadReq3), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitBigShot_DrawListHandle);
    if (g_BattleIsPaused == 0) {
        (task->frame)++;
        task->posX += task->velX;
        task->fadeLevel += task->fadeStep;
        task->posY += task->velY;
        if (task->fadeLevel > 0) {
            task->slot = 0xFFFF;
        }
    }
}
} /* extern "C" */
