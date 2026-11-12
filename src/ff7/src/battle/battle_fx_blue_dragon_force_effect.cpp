// src/ff7/src/battle/battle_fx_blue_dragon_force_effect.cpp
// Enemy Skill [6]: Dragon Force (Japanese: ドラゴンフォース, dev tag: "blue/dragonn" / Dragonn)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The Dragon Force blue magic effect: a dragon model that spins and scales around the
//   target, with segments shrinking toward it and growing away.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_fx.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x008aae58
int g_BattleBlueDragonForceResourceDefs = 0x00192A39;
char *g_BattleFxBlueDragonForceEffectResourceNames[1] = {
    // STRING: FF7 0x008aaf10
    "DRAGON.TIM"
};
unsigned int g_BattleFxBlueDragonForceEffectResourceNames2[32] = {
    // STRING: FF7 0x008aaf1c
    (unsigned int)"DRAGON.SPT",
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008aaf28
    (unsigned int)"DRAGON_1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008aaf38
    (unsigned int)"DRAGON_2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008aaf48
    (unsigned int)"DRAGON_3.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008aaf58
    (unsigned int)"DRAGON_4.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x004b00b1,
    // STRING: FF7 0x008aaf68
    (unsigned int)"LIGHT.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x2e808080,
    0x00000000
};
// g_BattleBlueDragonForceShrinkModelIndices is indexed by the code as an array. The span runs to the
// next named definition of the TU; the TU's pieces inside 0x8aaee0..0x8aaef0 are aliases.
// GLOBAL: FF7 0x008aaee0
unsigned char g_BattleBlueDragonForceShrinkModelIndices[16] = {0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00};
// g_BattleBlueDragonForceGrowModelIndices is indexed by the code as an array. The span runs to the next
// named definition of the TU; the TU's pieces inside 0x8aaef0..0x8aaf00 are aliases.
// GLOBAL: FF7 0x008aaef0
unsigned char g_BattleBlueDragonForceGrowModelIndices[16] = {0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008aaf00
short g_BattleBlueDragonForceTriggerFrames[2] = {1, 5};
unsigned char g_rawData_008aaf04[4] = {0x09, 0x00, 0x0d, 0x00};
unsigned char g_rawData_008aaf08[8] = {0x12, 0x00, 0x16, 0x00, 0x1a, 0x00, 0x1e, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x00ba5174
int g_BattleBlueDragonForceTargetIndex = 0;
// GLOBAL: FF7 0x00ba5178
struct GfxTableCursor *g_BattleBlueDragonForceLoadedResTable = 0;
// clang-format on

/* Loads Dragon Force blue magic resources. */
// FUNCTION: FF7 0x00570850
void __cdecl BattleFx_BlueLoadResDragonForce(void)
{
    GfxModelTableBank *sceneSource;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathSlotBuf[200];
    GfxResourceRequest *pRequest;
    int defsGroup;

    if (g_pBattle3dScene != 0 && g_BattleBlueDragonForceLoadedResTable == 0) {
        kind = 6;
        defsGroup = (int)&g_BattleBlueDragonForceResourceDefs;
        sceneSource = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008aaf74
        pRequest->resourcePath = BattleFx_BuildResourcePath("blue/dragonn", pathSlotBuf);
        Gfx_LoadModelTableIfAbsent(kind, defsGroup, pRequest, &g_BattleBlueDragonForceLoadedResTable, sceneSource);
    }
}

/* Entry point for Dragon Force blue magic effect. */
// FUNCTION: FF7 0x005708e0
void __cdecl BattleFx_BlueEnterDragonForce(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResDragonForce();
    BattleFx_BlueBeginDragonForce(targetMask, actorIdx);
}

/* Initializes setup task for Dragon Force. */
// FUNCTION: FF7 0x005708fa
void __cdecl BattleFx_BlueBeginDragonForce(int targetMask, int actorIdx)
{
    int idx, count;

    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    idx = 0;
    for (count = 0; idx < 10; idx++) {
        if ((targetMask & 1 << idx) != 0) {
            count++;
            if (count == 1) {
                g_BattleBlueDragonForceTargetIndex = idx;
            }
        }
    }
    Battle_AllocateResourceSlot(BattleFx_BlueDragonForceSetupTask);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x130);
}

/* Sets up target position and spawns rotating model and timeline tasks. */
// FUNCTION: FF7 0x00570996
void __cdecl BattleFx_BlueDragonForceSetupTask(void)
{
    BattleFxDragonForceSpin *spinPtr;
    BattleFxDragonForceSetupTask *task;

    task = (BattleFxDragonForceSetupTask *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    task = (BattleFxDragonForceSetupTask *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueDragonForceTimelineTask);
    Battle_GetBoneWorldPos(g_BattleBlueDragonForceTargetIndex, g_BattleActorRecords[g_BattleBlueDragonForceTargetIndex].targetBone, (short *)((int)task + 4));
    task->angle = 0;
    task->angleStep = 0xFEE0;
    task->scale = 0x1000 /* 1.0 */;
    task->scaleStep = task->scale / 12;
    task->growScale = 0;
    task->growScaleStep = task->scale / 12;
    task->triggerIndex = 0;
    spinPtr = (BattleFxDragonForceSpin *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueDragonForceSpinScaledModelTask);
    *(Int64Pair *)((char *)spinPtr + 4) = *(Int64Pair *)((char *)task + 4);
    spinPtr->angle = 0;
    spinPtr->lifetime = 32;
    spinPtr->rate = 0x1333;
    spinPtr->scaleGrowStep = spinPtr->rate / 12;
    spinPtr->scale = 0;
    spinPtr->scaleShrinkStep = spinPtr->rate / 12;
}

/* Animates spinning and scaling dragon model around target. */
// FUNCTION: FF7 0x00570ad6
void __cdecl BattleFx_BlueDragonForceSpinScaledModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxDragonForceSpinScaledModelSlot *slot; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_30[0x30]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxDragonForceSpinScaledModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = frame.slot->angle;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.scaleX.vz = frame.slot->scale;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    BattleFx_MakeViewSpacePointPushed((void *)((int)frame.slot + 4), -(frame.slot->scale >> 2), (Vec3sPacked *)&frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(5, g_BattleBlueDragonForceLoadedResTable);
    frame.modelDraw->flags = 8;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 128;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.slot->frameCounter >= 52) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.slot->angle += frame.slot->angleStep;
    if (frame.slot->frameCounter < 12) {
        frame.slot->scale += frame.slot->scaleGrowStep;
    } else if (frame.slot->frameCounter >= 40) {
        frame.slot->scale -= frame.slot->scaleShrinkStep;
    } else {
        frame.slot->scale = frame.slot->holdScale;
    }
    (frame.slot->frameCounter)++;
}

/* Drives timeline and spawns shrinking and growing dragon model tasks. */
// FUNCTION: FF7 0x00570c89
void __cdecl BattleFx_BlueDragonForceTimelineTask(void)
{
    BattleFxDragonForceTimeline *pChild;
    BattleFxDragonForceRecord *task;

    task = (BattleFxDragonForceRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frame == g_BattleBlueDragonForceTriggerFrames[task->triggerIndex] && task->triggerIndex < 8) {
        if (task->triggerIndex < 4) {
            pChild = (BattleFxDragonForceTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueDragonForceShrinkingModelTask);
            *(Int64Pair *)((char *)pChild + 4) = task->pos;
            pChild->scale = 0;
            pChild->angleStep = task->angleStep;
            pChild->posX = task->posX;
            pChild->posY = task->posY;
            pChild->triggerIndex = task->triggerIndex;
        } else {
            pChild = (BattleFxDragonForceTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueDragonForceGrowingModelTask);
            *(Int64Pair *)((char *)pChild + 4) = task->pos;
            pChild->scale = 0x200 /* 0.125 */;
            pChild->angleStep = task->angleStep;
            pChild->posX = task->growScale;
            pChild->posY = task->growScaleStep;
            pChild->triggerIndex = task->triggerIndex - 4;
        }
        (task->triggerIndex)++;
    }
    if (task->frame == 55) {
        Battle_PostActorHitOrHurtMessage(g_BattleBlueDragonForceTargetIndex);
    }
    if (task->frame >= 60) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frame)++;
}

/* Animates shrinking dragon model segments towards target. */
// FUNCTION: FF7 0x00570e24
void __cdecl BattleFx_BlueDragonForceShrinkingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxDragonForceGrowingModelSlot *slot; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_30[0x30]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxDragonForceGrowingModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.slot->angle;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
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
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(((int *)&g_BattleBlueDragonForceShrinkModelIndices)[frame.slot->modelIndex], g_BattleBlueDragonForceLoadedResTable);
    frame.modelDraw->flags = 0x88;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    if (frame.slot->frameCounter < 8) {
        frame.modelDraw->fadeLevel = 4096 - (frame.slot->frameCounter << 9);
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.slot->frameCounter >= 12) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.slot->angle += frame.slot->angleStep;
    frame.slot->scale -= frame.slot->scaleStep;
    if (frame.slot->scale < 0) {
        frame.slot->scale = 0;
    }
    (frame.slot->frameCounter)++;
}

/* Animates growing dragon model segments away from target. */
// FUNCTION: FF7 0x00570ffd
void __cdecl BattleFx_BlueDragonForceGrowingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxDragonForceGrowingModelSlot *slot; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_30[0x30]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxDragonForceGrowingModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.slot->angle;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
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
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(((int *)&g_BattleBlueDragonForceGrowModelIndices)[frame.slot->modelIndex], g_BattleBlueDragonForceLoadedResTable);
    frame.modelDraw->flags = 0x88;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    if (frame.slot->frameCounter >= 4) {
        frame.modelDraw->fadeLevel = frame.slot->frameCounter - 4 << 9;
        if (frame.modelDraw->fadeLevel > 4096) {
            frame.modelDraw->fadeLevel = 4096;
        }
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.slot->frameCounter >= 12) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.slot->angle += frame.slot->angleStep;
    frame.slot->scale += frame.slot->scaleStep;
    (frame.slot->frameCounter)++;
}
} /* extern "C" */
