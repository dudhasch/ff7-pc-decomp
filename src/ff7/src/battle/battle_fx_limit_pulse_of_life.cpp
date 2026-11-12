// src/ff7/src/battle/battle_fx_limit_pulse_of_life.cpp
// Limit Break [19] (Aeris Lv 3-2): Pulse of Life (Japanese: 生命の鼓動, dev tag: "LIMIT/KODOU" / Kodou / Kodo)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Kodou (Howling Moon) limit effect: loads its resource package and runs a master
//   timeline that pulses a model around the actor and emits rising spinning sprites, with a sound
//   task panned by actor location.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle_limit.h"
#include "ff7/types/battle_3d.h"
#include "ff7/battle_tasks.h"
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

// clang-format off
// GLOBAL: FF7 0x00806000
int g_BattleLimitPulseOfLifeResourceKind = 0x006400B1;
char *g_BattleFxLimitPulseOfLifeResourceNames[1] = {
    // STRING: FF7 0x00806098
    "GL_LIGHT.RSD"
};
unsigned int g_BattleLimitPulseOfLifeResourceKindDataTail[24] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008060a8
    (unsigned int)"KODOU.TIM",
    // STRING: FF7 0x008060b4
    (unsigned int)"KODOU.SPT",
    0x00000000,
    0x00000000,
    0x00642a39,
    // STRING: FF7 0x008060c0
    (unsigned int)"HL_LIGHT.TIM",
    // STRING: FF7 0x008060d0
    (unsigned int)"HL_LIGHT.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008060e0
    (unsigned int)"STARDUST.TIM",
    // STRING: FF7 0x008060f0
    (unsigned int)"STARDUST.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x00806100
    (unsigned int)"STARDST2.TIM",
    // STRING: FF7 0x00806110
    (unsigned int)"STARDST2.SPT",
    0x00000000,
    0x00000000,
    0x00000000
};
// clang-format on
/* The vertex colour and primitive code of a sprite request, written as one word or per channel. */
typedef union {
    int word;
    struct {
        char r;
        char g;
        char b;
        unsigned char code;
    } rgb;
} BattleLimitKodouSpriteColor;

/* The pulsing model's draw request, then the first two words of the rising sprite's
 * BattleFxSpriteRequest; that request's colour, frame and UVs are the first eight bytes of
 * g_BattleLimitPulseOfLifeSpriteRequest2, which follows directly. */
typedef struct {
    BattleFxModelDrawHead pulseModel;
    int risingSpriteGroup;
    int risingSpriteDrawObj;
} BattleLimitKodouSpriteRequest1;

/* The rest of the rising sprite's request, then the request of the burst sprites. */
typedef struct {
    BattleLimitKodouSpriteColor risingSpriteColor;
    unsigned short risingSpriteFrame;
    unsigned short risingSpriteUv;
    BattleFxSpriteRequest burstSprite;
} BattleLimitKodouSpriteRequest2;

// GLOBAL: FF7 0x00806068
BattleLimitKodouSpriteRequest1 g_BattleLimitPulseOfLifeSpriteRequest1 = {{0, 0xA8, 0, 0, 0x20, 0}, 0, 0};
// GLOBAL: FF7 0x00806080
BattleLimitKodouSpriteRequest2 g_BattleLimitPulseOfLifeSpriteRequest2 = {{0x2E808080}, 0, 0, {0, 0, -128, -128, -128, 0x2e, 0, 0}};

#pragma intrinsic(memcpy)
#pragma function(memcpy)

// clang-format off
// GLOBAL: FF7 0x00aef0ac
unsigned char g_BattleLimitPulseOfLifeActiveFlag = 0;
// GLOBAL: FF7 0x00aef0b0
unsigned char g_BattleLimitPulseOfLifeStateFlag = 0;
// GLOBAL: FF7 0x00aef0b4
struct BattleFxPulseOfLifeSpriteScratch *g_BattleLimitPulseOfLifeSpriteScratch = 0;
// GLOBAL: FF7 0x00aef0b8
int g_BattleLimitPulseOfLifeDrawListHandle = 0;
// GLOBAL: FF7 0x00aef0bc
struct BattleFxPulseOfLifeTransformScratch *g_BattleLimitPulseOfLifeTransformScratch = 0;
// GLOBAL: FF7 0x00aef0c0
struct GfxTableCursor *g_BattleLimitPulseOfLifeLoadedRes = 0;
unsigned char g_bssPad_00aef0c4[4] = {0};
unsigned char g_bssPad_00aef0c8[4] = {0};
unsigned char g_bssPad_00aef0cc[4] = {0};
unsigned char g_bssPad_00aef0d0[4] = {0};
unsigned char g_bssPad_00aef0d4[4] = {0};
unsigned char g_bssPad_00aef0d8[4] = {0};
unsigned char g_bssPad_00aef0dc[4] = {0};
unsigned char g_bssPad_00aef0e0[4] = {0};
unsigned char g_bssPad_00aef0e4[4] = {0};
// clang-format on

/*
 * Loads the "KODOU" (Howling Moon / Lunatic High) resource package.
 */
// FUNCTION: FF7 0x004ab910
void __cdecl BattleFx_LimitLoadResPulseOfLife(void)
{
    GfxModelTableBank *sceneField;
    GfxResourceRequest requestRecord;
    int kind;
    char pathBuffer[200];
    GfxResourceRequest *pRecordPtr;
    int descriptor;

    if (g_pBattle3dScene != 0 && g_BattleLimitPulseOfLifeLoadedRes == 0) {
        kind = 5;
        descriptor = (int)&g_BattleLimitPulseOfLifeResourceKind;
        sceneField = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRecordPtr = &requestRecord;
        Gfx_InitResourceRequest(pRecordPtr);
        pRecordPtr->groupTag = 0;
        // STRING: FF7 0x00806120
        pRecordPtr->resourcePath = BattleFx_BuildResourcePath("LIMIT/KODOU", pathBuffer);
        Gfx_LoadModelTableIfAbsent(kind, descriptor, pRecordPtr, &g_BattleLimitPulseOfLifeLoadedRes, sceneField);
    }
}

/*
 * Entry point for Kodou limit effect; loads resources and begins effect.
 */
// FUNCTION: FF7 0x004ab9a0
int __cdecl Battle_LimitFx19_PulseOfLife(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResPulseOfLife();
    BattleFx_LimitBeginPulseOfLife(targetMask, actorIdx);
    return 0;
}

/*
 * Spawns a timeline task for each target in the mask and the sound task at the actor's target bone.
 */
// FUNCTION: FF7 0x004ab9bc
void __cdecl BattleFx_LimitBeginPulseOfLife(int targetMask, int actorIdx)
{
    BattleTaskSlot *slot;

    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 0, BattleFx_LimitSpawnPulseOfLifePerTarget);
    slot = BATTLE_NEW_TASK_SLOT(BattleFx_LimitPulseOfLifeSoundTask);
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].targetBone, (short *)slot->data);
}

/*
 * Spawns per-target timeline task at the target actor bone position.
 */
// FUNCTION: FF7 0x004aba1b
void __cdecl BattleFx_LimitSpawnPulseOfLifePerTarget(int actor)
{
    BattleFxPulseOfLifeTask *slot;

    slot = (BattleFxPulseOfLifeTask *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitPulseOfLifeTimelineTask);
    slot->actorIndex = actor;
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &slot->posX);
    slot->negRadius = -g_BattleActorRecords[actor].radius;
}

/*
 * Master timeline task managing pulsating model and rising sprite emitter.
 */
// FUNCTION: FF7 0x004aba85
void __cdecl BattleFx_LimitPulseOfLifeTimelineTask(void)
{
    BattleFxPulseOfLifeTaskSlot *taskPtr;
    int i;
    BattleFxPulseOfLifeTimelineRecord *child;

    taskPtr = (BattleFxPulseOfLifeTaskSlot *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleLimitPulseOfLifeActiveFlag & 1) == 0) {
        g_BattleLimitPulseOfLifeActiveFlag |= 1;
        g_BattleLimitPulseOfLifeTransformScratch = (struct BattleFxPulseOfLifeTransformScratch *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->step == 56) {
        child = (BattleFxPulseOfLifeTimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitPulseOfLifePulsingModelTask);
        child->posX = taskPtr->posX;
        child->posY = 0;
        child->posZ = taskPtr->posZ;
        child->negRadius = taskPtr->negRadius;
    }
    if (taskPtr->step == 56) {
        child = (BattleFxPulseOfLifeTimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitPulseOfLifeSpawnRisingSpriteTask);
        child->actorIndex = taskPtr->actorIndex;
        child->negRadius = taskPtr->negRadius;
    }
    if (taskPtr->step == 97) {
        for (i = 0; i < 20; i++) {
            child = (BattleFxPulseOfLifeTimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitPulseOfLifeAnimatedSpriteTask);
            *(Int64Pair *)&child->posX = *(Int64Pair *)&taskPtr->posX;
            g_BattleLimitPulseOfLifeTransformScratch->angles.vx = 0;
            g_BattleLimitPulseOfLifeTransformScratch->angles.vy = rand() & 0xFFF;
            g_BattleLimitPulseOfLifeTransformScratch->angles.vz = -(rand() % 0x100 + 0x200);
            RotMatrix(&g_BattleLimitPulseOfLifeTransformScratch->angles, &g_BattleLimitPulseOfLifeTransformScratch->matrix);
            g_BattleLimitPulseOfLifeTransformScratch->angles.vx = rand() % 0xC8 + 0x12C;
            g_BattleLimitPulseOfLifeTransformScratch->angles.vy = 0;
            g_BattleLimitPulseOfLifeTransformScratch->angles.vz = 0;
            TransformVectorToInt16(&g_BattleLimitPulseOfLifeTransformScratch->matrix, &g_BattleLimitPulseOfLifeTransformScratch->angles, &child->velocityX);
        }
        Battle_PostActorHitOrHurtMessage(taskPtr->actorIndex);
        taskPtr->actorIndex = BATTLE_TASK_FREE;
    }
    (taskPtr->step)++;
}

/*
 * Animates pulsing model scaling and orientation around the actor.
 */
// FUNCTION: FF7 0x004abcb0
void __cdecl BattleFx_LimitPulseOfLifePulsingModelTask(void)
{
    int stage;
    BattleFxPulseOfLifePulseTask *task;

    task = (BattleFxPulseOfLifePulseTask *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitPulseOfLifeSpriteRequest1.pulseModel.modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleLimitPulseOfLifeLoadedRes);
    stage = task->step;
    if (stage < 15) {
        if (stage == 0) {
            Battle_StartFixedPointRamp(0x800, 15);
        }
        g_BattleLimitPulseOfLifeSpriteRequest1.pulseModel.fadeLevel = stage * -2048 / 15 + 4096;
    } else {
        stage -= 15;
        if (stage < 25) {
            g_BattleLimitPulseOfLifeSpriteRequest1.pulseModel.fadeLevel = 2048;
        } else {
            stage -= 25;
            if (stage < 15) {
                if (stage == 0) {
                    Battle_StartFixedPointRamp(0, 15);
                }
                g_BattleLimitPulseOfLifeSpriteRequest1.pulseModel.fadeLevel = (stage << 11) / 15 + 2048;
            } else {
                task->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    BattleFx_InstallBillboardMatrix(&task->posX, 0x800 /* 0.5 */, task->pushLength);
    SetDrawColorRGB(0, 0, 0);
    g_BattleLimitPulseOfLifeDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitPulseOfLifeSpriteRequest1.pulseModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitPulseOfLifeDrawListHandle);
    if (g_BattleIsPaused == 0) {
        (task->step)++;
    }
}

/*
 * Emitter task spawning rising spinning sprites.
 */
// FUNCTION: FF7 0x004abdf1
void __cdecl BattleFx_LimitPulseOfLifeSpawnRisingSpriteTask(void)
{
    BattleFxLimitBoneSpawnSlot *taskPtr;
    int unusedSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int i;
    BattleFxLimitPulseOfLifeSpinSprite *pSprite;

    taskPtr = (BattleFxLimitBoneSpawnSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    for (i = 0; i < 1; i++) {
        pSprite = (BattleFxLimitPulseOfLifeSpinSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitPulseOfLifeRisingSpinningSpriteTask);
        pSprite->ownerSlot = 0xA - taskPtr->timer;
        Battle_GetBoneWorldPos(taskPtr->actorIndex, rand() % g_BattleActorRecords[taskPtr->actorIndex].boneCount, &pSprite->posX);
        pSprite->posX += (short)(rand() % 200 - 100);
        pSprite->posY += (short)(rand() % 200 - 1975);
        pSprite->posZ += (short)(rand() % 200 - 100);
        pSprite->tiltAngle = rand() & 0xFFF;
        pSprite->startScale = rand() % 0x400 + 0x400;
        pSprite->pushLength = taskPtr->negRadius;
    }
    (taskPtr->timer)++;
    if (taskPtr->timer >= 10) {
        taskPtr->actorIndex = BATTLE_TASK_FREE;
    }
}

/*
 * Animates rising and spinning billboard sprites with vertical velocity.
 */
// FUNCTION: FF7 0x004abf6a
void __cdecl BattleFx_LimitPulseOfLifeRisingSpinningSpriteTask(void)
{
    int frame;
    BattleFxLimitPulseOfLifeSpinSprite *task;
    int bytes;

    task = (BattleFxLimitPulseOfLifeSpinSprite *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleLimitPulseOfLifeStateFlag & 1) == 0) {
        g_BattleLimitPulseOfLifeStateFlag |= 1;
        g_BattleLimitPulseOfLifeSpriteScratch = (struct BattleFxPulseOfLifeSpriteScratch *)Gfx_GetPsxScratchpadPtr(0);
    }
    g_BattleLimitPulseOfLifeSpriteRequest1.risingSpriteGroup = Gfx_ModelTableGetLoadedRecord(2, g_BattleLimitPulseOfLifeLoadedRes);
    g_BattleLimitPulseOfLifeSpriteRequest1.risingSpriteDrawObj = Gfx_ModelTableGetSpriteObject(2, g_BattleLimitPulseOfLifeLoadedRes);
    frame = task->frame;
    if (frame < 5) {
        g_BattleLimitPulseOfLifeSpriteRequest2.risingSpriteColor.rgb.b = (frame << 7) / 5;
        g_BattleLimitPulseOfLifeSpriteRequest2.risingSpriteColor.rgb.g = g_BattleLimitPulseOfLifeSpriteRequest2.risingSpriteColor.rgb.b;
        g_BattleLimitPulseOfLifeSpriteRequest2.risingSpriteColor.rgb.r = g_BattleLimitPulseOfLifeSpriteRequest2.risingSpriteColor.rgb.g;
        bytes = task->startScale;
    } else {
        frame -= 5;
        if (frame < 20) {
            g_BattleLimitPulseOfLifeSpriteRequest2.risingSpriteColor.word = 0x2E808080;
            bytes = task->startScale;
        } else {
            frame -= 20;
            if (frame < 5) {
                g_BattleLimitPulseOfLifeSpriteRequest2.risingSpriteColor.rgb.b = frame * -0x80 / 5 + 0x80;
                g_BattleLimitPulseOfLifeSpriteRequest2.risingSpriteColor.rgb.g = g_BattleLimitPulseOfLifeSpriteRequest2.risingSpriteColor.rgb.b;
                g_BattleLimitPulseOfLifeSpriteRequest2.risingSpriteColor.rgb.r = g_BattleLimitPulseOfLifeSpriteRequest2.risingSpriteColor.rgb.g;
                bytes = task->startScale + (0 - task->startScale) * frame / 5;
            } else {
                task->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    g_BattleLimitPulseOfLifeSpriteScratch->angles.vx = 0;
    g_BattleLimitPulseOfLifeSpriteScratch->angles.vy = 0;
    g_BattleLimitPulseOfLifeSpriteScratch->angles.vz = task->tiltAngle;
    RotMatrix(&g_BattleLimitPulseOfLifeSpriteScratch->angles, &g_BattleLimitPulseOfLifeSpriteScratch->matrix);
    g_BattleLimitPulseOfLifeSpriteScratch->scale.vz = bytes;
    g_BattleLimitPulseOfLifeSpriteScratch->scale.vy = bytes;
    g_BattleLimitPulseOfLifeSpriteScratch->scale.vx = bytes;
    ScaleMatrix(&g_BattleLimitPulseOfLifeSpriteScratch->matrix, &g_BattleLimitPulseOfLifeSpriteScratch->scale);
    BattleFx_MakeViewSpacePointPushed(&task->posX, task->pushLength, (Vec3sPacked *)&g_BattleLimitPulseOfLifeSpriteScratch->matrix);
    SetRotMatrix(&g_BattleLimitPulseOfLifeSpriteScratch->matrix);
    SetTransMatrix(&g_BattleLimitPulseOfLifeSpriteScratch->matrix);
    g_BattleLimitPulseOfLifeDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleLimitPulseOfLifeSpriteRequest1.risingSpriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitPulseOfLifeDrawListHandle);
    if (g_BattleIsPaused == 0) {
        if (task->frame < 25) {
            task->posY += 75;
            task->tiltAngle += 0x80;
        }
        (task->frame)++;
    }
}

/*
 * Animates billboard sprite with scaling, color fading, and retirement.
 */
// FUNCTION: FF7 0x004ac1e7
void __cdecl BattleFx_LimitPulseOfLifeAnimatedSpriteTask(void)
{
    BattleFxPulseOfLifeSprite *task;

    task = (BattleFxPulseOfLifeSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitPulseOfLifeSpriteRequest2.burstSprite.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_BattleLimitPulseOfLifeLoadedRes);
    g_BattleLimitPulseOfLifeSpriteRequest2.burstSprite.drawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleLimitPulseOfLifeLoadedRes);
    BattleFx_InstallBillboardMatrix(&task->posX, 0x1000 /* 1.0 */, 0xFFFFFF00);
    g_BattleLimitPulseOfLifeSpriteRequest2.burstSprite.frame = task->frame / 2;
    g_BattleLimitPulseOfLifeDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleLimitPulseOfLifeSpriteRequest2.burstSprite, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitPulseOfLifeDrawListHandle);
    if (g_BattleIsPaused == 0) {
        if (g_BattleLimitPulseOfLifeSpriteRequest2.burstSprite.spriteGroup != 0) {
            (task->frame)++;
            if (task->frame / 2 >= g_BattleLimitPulseOfLifeSpriteRequest2.burstSprite.spriteGroup->header->frameCount) {
                task->ownerSlot = BATTLE_TASK_FREE;
            }
        } else {
            task->posX += task->velocityX;
            task->posY += task->velocityY;
            task->posZ += task->velocityZ;
            task->velocityX = task->velocityX * 0xC00 /* 0.75 */ >> 12;
            task->velocityY = (task->velocityY * 0xE00 /* 0.875 */ >> 12) + 30;
            task->velocityZ = task->velocityZ * 0xC00 /* 0.75 */ >> 12;
        }
    }
}

/*
 * Task playing limit sound effects panned by actor location.
 */
// FUNCTION: FF7 0x004ac354
void __cdecl BattleFx_LimitPulseOfLifeSoundTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 5) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(task->data), 0x15);
    } else if (task->frameCounter == 56) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(task->data), 0x11E);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}
} /* extern "C" */
