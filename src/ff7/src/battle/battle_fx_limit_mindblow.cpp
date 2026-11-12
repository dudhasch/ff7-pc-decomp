// src/ff7/src/battle/battle_fx_limit_mindblow.cpp
// Limit Break [9] (Barret Lv 1-2): Mindblow (Japanese: マインドブレイク Mind Break, dev tag: "LIMIT/MIND" / Mind)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Mind limit effect: spawns a per-target timeline that coordinates spinning model pairs,
//   drifting fading sprites and damped particles around the target.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_limit.h"
#include "ff7/battle_tasks.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/api/graphics.h"
extern "C" {

#pragma intrinsic(memset)
/* b3ddata.cpp defines this storage through a byte view; this TU reads it as the ten actor records,
 * whose transform matrix starts at auxNode. */

// clang-format off
// GLOBAL: FF7 0x00806130
int g_BattleLimitMindblowResourceKind = 0x00C800B4;
char *g_BattleFxLimitMindblowResourceNames[1] = {
    // STRING: FF7 0x008061d0
    "MIND.RSD"
};
unsigned int g_BattleLimitMindblowResourceKindDataTail[14] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x00c800b1,
    // STRING: FF7 0x008061dc
    (unsigned int)"MIND.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008061e8
    (unsigned int)"M_BREAK.TIM",
    // STRING: FF7 0x008061f4
    (unsigned int)"M_BRAKE.SPT",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x00806170
int g_BattleLimitMindblowMatrix = 4096;
unsigned char g_rawData_00806178[24] = {0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4c, 0xff, 0xff, 0xff, 0x00, 0x00};
// clang-format on
/* The first twelve bytes of the BattleFxModelDraw request the spinning-pair task submits for both
 * models; g_rawData_0080619c after it holds the request's state flags. */
typedef struct {
    int modelGroup;
    int flags;
    short uvScroll;
    short fadeLevel;
} BattleLimitMindModelRequest;

/* The second spinning model's scale vector (1.125 on each axis), then the sprite group and draw
 * object of the drifting-sprite request, whose colour and frame begin g_BattleLimitMindblowSpriteRequest2. */
typedef struct {
    int secondModelScale[3];
    unsigned char pad_0C[0x4];
    int driftSpriteGroup;
    int driftDrawObj;
} BattleLimitMindSpriteRequest1;

/* The drifting-sprite request's colour, primitive code and frame, then the sprite group and draw
 * object of the damped-sprite request, whose colour and frame are g_BattleLimitMindblowSpriteRequest3. */
typedef struct {
    char driftColorR;
    char driftColorG;
    char driftColorB;
    unsigned char driftCode;
    short driftFrame;
    unsigned char pad_06[0x2];
    int dampedSpriteGroup;
    int dampedDrawObj;
} BattleLimitMindSpriteRequest2;

/* The damped-sprite request's colour, primitive code and frame. */
typedef struct {
    char dampedColorR;
    char dampedColorG;
    char dampedColorB;
    unsigned char dampedCode;
    short dampedFrame;
    unsigned char pad_06[0x2];
} BattleLimitMindSpriteRequest3;

// clang-format off
// GLOBAL: FF7 0x00806190
BattleLimitMindModelRequest g_BattleLimitMindblowModelRequest = {0, 0, 0, 0x80};
unsigned char g_rawData_0080619c[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008061a0
BattleLimitMindSpriteRequest1 g_BattleLimitMindblowSpriteRequest1 = {{0x1200, 0x1200, 0x1200}, {0}, 0, 0};
// GLOBAL: FF7 0x008061b8
BattleLimitMindSpriteRequest2 g_BattleLimitMindblowSpriteRequest2 = {(char)0x80, (char)0x80, (char)0x80, 0x2e, 0, {0}, 0, 0};
// GLOBAL: FF7 0x008061c8
BattleLimitMindSpriteRequest3 g_BattleLimitMindblowSpriteRequest3 = {(char)0x80, (char)0x80, (char)0x80, 0x2e, 0, {0}};
// clang-format on

#pragma intrinsic(memcpy)
#pragma function(memcpy)

// clang-format off
// GLOBAL: FF7 0x00aef0e8
int g_BattleLimitMindblowDrawHandle = 0;
unsigned char g_bssPad_00aef0ec[4] = {0};
unsigned char g_bssPad_00aef0f0[1600] = {0};
// GLOBAL: FF7 0x00aef730
struct GfxTableCursor *g_BattleLimitMindblowLoadedResTable = 0;
unsigned char g_bssPad_00aef734[4] = {0};
// GLOBAL: FF7 0x00aef738
int g_BattleLimitMindblowModel1Rot = 0;
// GLOBAL: FF7 0x00aef73c
short g_BattleLimitMindblowModel1RotZ = 0;
// GLOBAL: FF7 0x00aef740
int g_BattleLimitMindblowModel2Rot = 0;
// GLOBAL: FF7 0x00aef744
short g_BattleLimitMindblowModel2RotZ = 0;
// clang-format on

/*
 * Loads the "MIND" resource package for the Mind limit break effect.
 */
// FUNCTION: FF7 0x004ac3f0
void __cdecl BattleFx_LimitLoadResMindblow(void)
{
    GfxModelTableBank *sceneField;
    GfxResourceRequest requestRecord;
    int kind;
    char pathBuffer[200];
    GfxResourceRequest *pRecordPtr;
    int descriptor;

    if (g_pBattle3dScene != 0 && g_BattleLimitMindblowLoadedResTable == 0) {
        kind = 3;
        descriptor = (int)&g_BattleLimitMindblowResourceKind;
        sceneField = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRecordPtr = &requestRecord;
        Gfx_InitResourceRequest(pRecordPtr);
        pRecordPtr->groupTag = 0;
        // STRING: FF7 0x00806200
        pRecordPtr->resourcePath = (char *)BattleFx_BuildResourcePath("LIMIT/MIND", pathBuffer);
        Gfx_LoadModelTableIfAbsent(kind, descriptor, pRecordPtr, &g_BattleLimitMindblowLoadedResTable, sceneField);
    }
}

/*
 * Entry point for Mind limit effect; loads resources and registers callback.
 */
// FUNCTION: FF7 0x004ac480
int __cdecl BattleFx_LimitEnterMindblow(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResMindblow();
    BattleFx_LimitRegisterMindblowPerTargetCallback(targetMask, actorIdx);
    return 0;
}

/*
 * Registers bit-pattern callback to spawn Mind effects per target.
 */
// FUNCTION: FF7 0x004ac49c
void __cdecl BattleFx_LimitRegisterMindblowPerTargetCallback(int targetMask, int actorIdx)
{
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 0, BattleFx_LimitSpawnMindblowPerTarget);
}

/*
 * Allocates timeline task for a target and stores target and actor indices.
 */
// FUNCTION: FF7 0x004ac4b8
void __cdecl BattleFx_LimitSpawnMindblowPerTarget(int actor, int actorIdx)
{
    BattleFxMindTask *slot;

    slot = (BattleFxMindTask *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitMindblowTimelineTask);
    slot->targetMask = actor;
    slot->actorIndex = actorIdx;
}

/*
 * Master timeline task coordinating spinning models, drifting sprites, and particles.
 */
// FUNCTION: FF7 0x004ac4f1
void __cdecl BattleFx_LimitMindblowTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[124];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix driftMatrix; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix cameraMatrix; };
        struct { unsigned char pad_to_48[0x48]; int pointX; };
        struct { unsigned char pad_to_4C[0x4C]; int pointY; };
        struct { unsigned char pad_to_50[0x50]; int pointZ; };
        struct { unsigned char pad_to_58[0x58]; BattleFxLimitMindblowParticle *particle; };
        struct { unsigned char pad_to_5C[0x5C]; int spawnIndex; };
        struct { unsigned char pad_to_64[0x64]; SVECTOR offsetX; };
        struct { unsigned char pad_to_6C[0x6C]; int depth; };
        struct { unsigned char pad_to_70[0x70]; BattleFxLimitMindblowTaskSlot *mindSlot; };
        struct { unsigned char pad_to_78[0x78]; int step; };
    } frame;
#pragma pack(pop)

    frame.mindSlot = (BattleFxLimitMindblowTaskSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.step = frame.mindSlot->step;
    if (frame.step < 0) { /* empty first arm: lib/codegen.h */
    } else {
        frame.step = frame.step;
        if (frame.step < 22) {
            if (frame.step == 0) {
                frame.particle = (BattleFxLimitMindblowParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitMindblowSpinningModelPairTask);
                frame.particle->actorIndex = frame.mindSlot->actorIndex;
                frame.particle->matrixIndex = frame.mindSlot->slotIndex;
                Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[frame.mindSlot->slotIndex].posX), 0x28);
            }
            if (frame.step < 14) {
                Battle_ComposeMatrixRelativeToCamera((short *)&g_BattleCameraTransform, g_BattleActorTransformRecords[frame.mindSlot->slotIndex].auxNode, &frame.cameraMatrix);
                for (frame.spawnIndex = 0; frame.spawnIndex < 2; frame.spawnIndex++) {
                    frame.particle = (BattleFxLimitMindblowParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitMindblowDriftingSpriteTask);
                    frame.offsetX.vy = 0;
                    frame.offsetX.vx = frame.offsetX.vy;
                    frame.offsetX.vz = -(rand() % 1000 + 1000);
                    frame.rotX.vx = (rand() & 0x3FF) - 0x200;
                    frame.rotX.vy = (rand() & 0x3FF) - 0x200;
                    frame.rotX.vz = 0;
                    RotMatrix(&frame.rotX, &frame.driftMatrix);
                    frame.driftMatrix.t[2] = 0xFFFFFF4C;
                    frame.driftMatrix.t[1] = frame.driftMatrix.t[2];
                    frame.driftMatrix.t[0] = frame.driftMatrix.t[1];
                    MultiplyMatrix(&frame.cameraMatrix, &frame.driftMatrix, &frame.driftMatrix);
                    SetRotMatrix(&frame.driftMatrix);
                    SetTransMatrix(&frame.driftMatrix);
                    RotTransPoint(&frame.offsetX, &frame.pointX, &frame.depth);
                    frame.particle->originX = *(short *)&frame.pointX;
                    frame.particle->originY = *(short *)&frame.pointY;
                    frame.particle->originZ = *(short *)&frame.pointZ;
                    frame.particle->stepX = (frame.driftMatrix.t[0] - frame.pointX) / 8;
                    frame.particle->stepY = (frame.driftMatrix.t[1] - frame.pointY) / 8;
                    frame.particle->stepZ = (frame.driftMatrix.t[2] - frame.pointZ) / 8;
                    frame.particle->kind = rand() % 3;
                    frame.particle->scale = 0x1000 /* 1.0 */;
                }
            }
        } else {
            frame.step -= 22;
            if (frame.step < 7) {
                if (frame.step == 0) {
                    BattleFx_FadeOutScreenFlashRamp(2);
                    Battle_PostActorHitOrHurtMessage(frame.mindSlot->actorIndex);
                }
                if (frame.step < 3) {
                    for (frame.spawnIndex = 0; frame.spawnIndex < 20; frame.spawnIndex++) {
                        frame.particle = (BattleFxLimitMindblowParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitMindblowDampedSpriteParticleTask);
                        Battle_GetBoneWorldPos(frame.mindSlot->actorIndex, g_BattleActorRecords[frame.mindSlot->actorIndex].targetBone, &frame.particle->originX);
                        frame.offsetX.vy = 0;
                        frame.offsetX.vx = frame.offsetX.vy;
                        frame.offsetX.vz = 0xFFFFFED4 - rand() % 500;
                        frame.rotX.vx = (rand() & 0x3FF) - 0x200;
                        frame.rotX.vy = g_BattleActorRecords[frame.mindSlot->slotIndex].yaw + (rand() & 0x3FF) + -0x200;
                        frame.rotX.vz = 0;
                        RotMatrix(&frame.rotX, &frame.driftMatrix);
                        TransformVectorToInt16(&frame.driftMatrix, &frame.offsetX, &frame.particle->stepX);
                        frame.particle->scale = rand() % 0x800 + 0x800;
                        frame.particle->kind = rand() % 3;
                    }
                }
            } else {
                frame.step -= 7;
                if (frame.step < 15) { /* empty first arm: lib/codegen.h */
                } else {
                    frame.mindSlot->ownerSlot = BATTLE_TASK_FREE;
                }
            }
        }
    }
    (frame.mindSlot->step)++;
}

/*
 * Animates a pair of spinning scaled models around the target actor.
 */
// FUNCTION: FF7 0x004ac8e1
void __cdecl BattleFx_LimitMindblowSpinningModelPairTask(void)
{
    VECTOR growZ;
    int frameIdx;
    int padLocal[7]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxMindSpinModelPair *selfPtr;
    PsxMatrix matrix;

    selfPtr = (BattleFxMindSpinModelPair *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitMindblowModelRequest.modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleLimitMindblowLoadedResTable);
    frameIdx = selfPtr->frame;
    if (frameIdx < 22) {
        growZ.vz = frameIdx * 448 / 22 + 64;
        growZ.vy = growZ.vz;
        growZ.vx = growZ.vy;
        if (g_BattleIsPaused == 0) {
            g_BattleLimitMindblowModel1RotZ += 0x280;
            g_BattleLimitMindblowModel2RotZ -= 0x1C0;
        }
        RotMatrix((SVECTOR *)&g_BattleLimitMindblowModel1Rot, &g_BattleLimitMindblowMatrix);
        ScaleMatrix(&g_BattleLimitMindblowMatrix, &growZ);
        MultiplyMatrix(g_BattleActorTransformRecords[selfPtr->matrixIndex].auxNode, &g_BattleLimitMindblowMatrix, &matrix);
        SetRotMatrix(&matrix);
        SetTransMatrix(&matrix);
        g_BattleLimitMindblowModelRequest.flags = 0;
        g_BattleLimitMindblowDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitMindblowModelRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitMindblowDrawHandle);
        RotMatrix((SVECTOR *)&g_BattleLimitMindblowModel2Rot, &g_BattleLimitMindblowMatrix);
        ScaleMatrix(&g_BattleLimitMindblowMatrix, &growZ);
        MultiplyMatrix(g_BattleActorTransformRecords[selfPtr->matrixIndex].auxNode, &g_BattleLimitMindblowMatrix, &matrix);
        ScaleMatrix(&matrix, g_BattleLimitMindblowSpriteRequest1.secondModelScale);
        SetRotMatrix(&matrix);
        g_BattleLimitMindblowModelRequest.flags = 8;
        g_BattleLimitMindblowModelRequest.modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleLimitMindblowLoadedResTable);
        g_BattleLimitMindblowDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitMindblowModelRequest, g_BattleCurrentOrderingTable + 0x60, 0xC, g_BattleLimitMindblowDrawHandle);
        if (frameIdx == 21) {
            BattleFx_StartScreenFlashRamp(1);
            selfPtr->ownerSlot = BATTLE_TASK_FREE;
        }
    } else {
        selfPtr->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused == 0) {
        (selfPtr->frame)++;
    }
}

/*
 * Animates drifting and fading billboard sprites around the target.
 */
// FUNCTION: FF7 0x004acaea
void __cdecl BattleFx_LimitMindblowDriftingSpriteTask(void)
{
    int age;
    BattleFxLimitMindblowDriftSprite *task;

    task = (BattleFxLimitMindblowDriftSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitMindblowSpriteRequest1.driftSpriteGroup = Gfx_ModelTableGetLoadedRecord(2, g_BattleLimitMindblowLoadedResTable);
    g_BattleLimitMindblowSpriteRequest1.driftDrawObj = Gfx_ModelTableGetSpriteObject(2, g_BattleLimitMindblowLoadedResTable);
    age = task->frame;
    if (age < 4) {
        g_BattleLimitMindblowSpriteRequest2.driftColorB = (age << 7) / 4;
        g_BattleLimitMindblowSpriteRequest2.driftColorG = g_BattleLimitMindblowSpriteRequest2.driftColorB;
        g_BattleLimitMindblowSpriteRequest2.driftColorR = g_BattleLimitMindblowSpriteRequest2.driftColorG;
    } else {
        age -= 4;
        if (age < 4) {
            g_BattleLimitMindblowSpriteRequest2.driftColorB = 0x80;
            g_BattleLimitMindblowSpriteRequest2.driftColorG = g_BattleLimitMindblowSpriteRequest2.driftColorB;
            g_BattleLimitMindblowSpriteRequest2.driftColorR = g_BattleLimitMindblowSpriteRequest2.driftColorG;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->scale, 0);
    g_BattleLimitMindblowSpriteRequest2.driftFrame = task->ownerSlot;
    g_BattleLimitMindblowDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleLimitMindblowSpriteRequest1.driftSpriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitMindblowDrawHandle);
    if (g_BattleIsPaused == 0) {
        (task->frame)++;
        task->posX += task->velocityX;
        task->posY += task->velocityY;
        task->posZ += task->velocityZ;
    }
}

/*
 * Animates damped velocity sprite particles bursting outward from the actor.
 */
// FUNCTION: FF7 0x004acc4f
void __cdecl BattleFx_LimitMindblowDampedSpriteParticleTask(void)
{
    int age;
    BattleFxMindDampedSprite *task;

    task = (BattleFxMindDampedSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitMindblowSpriteRequest2.dampedSpriteGroup = Gfx_ModelTableGetLoadedRecord(2, g_BattleLimitMindblowLoadedResTable);
    g_BattleLimitMindblowSpriteRequest2.dampedDrawObj = Gfx_ModelTableGetSpriteObject(2, g_BattleLimitMindblowLoadedResTable);
    age = task->age;
    if (age < 5) {
        g_BattleLimitMindblowSpriteRequest3.dampedColorB = 0x80;
        g_BattleLimitMindblowSpriteRequest3.dampedColorG = g_BattleLimitMindblowSpriteRequest3.dampedColorB;
        g_BattleLimitMindblowSpriteRequest3.dampedColorR = g_BattleLimitMindblowSpriteRequest3.dampedColorG;
    } else {
        age -= 5;
        if (age < 10) {
            g_BattleLimitMindblowSpriteRequest3.dampedColorB = age * -0x80 / 10 + 0x80;
            g_BattleLimitMindblowSpriteRequest3.dampedColorG = g_BattleLimitMindblowSpriteRequest3.dampedColorB;
            g_BattleLimitMindblowSpriteRequest3.dampedColorR = g_BattleLimitMindblowSpriteRequest3.dampedColorG;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->scale, 0);
    g_BattleLimitMindblowSpriteRequest3.dampedFrame = task->ownerSlot;
    g_BattleLimitMindblowDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleLimitMindblowSpriteRequest2.dampedSpriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitMindblowDrawHandle);
    if (g_BattleIsPaused == 0) {
        (task->age)++;
        task->posX += task->velocityX;
        task->posY += task->velocityY;
        task->posZ += task->velocityZ;
        task->velocityX = task->velocityX * 0xF00 /* 0.9375 */ >> 12;
        task->velocityY = task->velocityY * 0xF00 /* 0.9375 */ >> 12;
        task->velocityZ = task->velocityZ * 0xF00 /* 0.9375 */ >> 12;
        if (task->posY > 0) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
