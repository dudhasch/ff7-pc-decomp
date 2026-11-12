// src/ff7/src/battle/battle_fx_limit_fury_brand.cpp
// Limit Break [17] (Aeris Lv 2-2): Fury Brand (Japanese: 怒りの烙印, dev tag: "LIMIT/RAKUIN" / Rakuin)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Rakuin limit effect: loads its resources and spawns per-target timelines that run
//   fading sprites and models, a growing fade model, and a sound timeline that plays limit sounds
//   at set frames.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "lib/codegen.h"
#include "ff7/types/battle_limit.h"
#include "ff7/battle_tasks.h"
#include "lib/types/graphics.h"
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
unsigned char g_rawData_007fbb78[32] = {
    0x3a, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x74, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// GLOBAL: FF7 0x007fbb98
int g_BattleLimitFuryBrandSoundGroup[4] = {8371064, 8371080, 0, 0};
// GLOBAL: FF7 0x007fbba8
int g_BattleLimitFuryBrandResourceDef = 0x00C800B1;
int g_BattleLimitFuryBrandResourceDefHeadPtr[1] = {
    (int)((char *)&g_BattleFxLimitFuryBrandResourceNames + 0x0)
};
int g_BattleLimitFuryBrandResourceDefDataTail[24] = {
    0x0, 0x0, 0x0, 0xc800b1, (int)((char *)&g_BattleFxLimitFuryBrandResourceNames + 0xc), 0x0,
    0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxLimitFuryBrandResourceNames + 0x18), (int)((char *)&g_BattleFxLimitFuryBrandResourceNames + 0x24), 0x0,
    0x0, 0x192a39, (int)((char *)&g_BattleFxLimitFuryBrandResourceNames + 0x30), (int)((char *)&g_BattleFxLimitFuryBrandResourceNames + 0x3c), 0x0, 0x0,
    0x192a39, (int)((char *)&g_BattleFxLimitFuryBrandResourceNames + 0x4c), (int)((char *)&g_BattleFxLimitFuryBrandResourceNames + 0x58), 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x007fbc10
struct BattleLimitFuryBrandSpriteReqA g_BattleLimitFuryBrandSpriteReqA = {0, 0, 0x2e808080, 0, {0}};
// GLOBAL: FF7 0x007fbc20
unsigned char g_BattleLimitFuryBrandScaleVector[16] = {0x00, 0x30, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007fbc30
struct BattleLimitFuryBrandSpriteReqB g_BattleLimitFuryBrandSpriteReqB = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
// GLOBAL: FF7 0x007fbc40
struct BattleLimitFuryBrandModelReqA g_BattleLimitFuryBrandModelReqA = {0, {0x88, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x007fbc50
struct BattleLimitFuryBrandModelReqA g_BattleLimitFuryBrandModelReqB = {0, {0x88, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// clang-format off
unsigned char g_BattleFxLimitFuryBrandResourceNames[104] =
    "IKARI1.RSD\0\0"
    "IKARI2.RSD\0\0"
    "PLAZMA.TIM\0\0"
    "THUNDER.SPT\0"
    "PLAZMA.TIM\0\0"
    "DENGEKI1.SPT\0\0\0\0"
    "PLAZMA.TIM\0\0"
    "DENGEKI2.SPT";
// clang-format on
// GLOBAL: FF7 0x00aaf960
int g_BattleLimitFuryBrandDrawHandle = 0;
// GLOBAL: FF7 0x00aaf964
struct GfxTableCursor *g_BattleLimitFuryBrandLoadedResTable = 0;
// GLOBAL: FF7 0x00aaf968
short g_BattleLimitFuryBrandScaleMatrix = 0;
unsigned char g_bssPad_00aaf96c[4] = {0};
// GLOBAL: FF7 0x00aaf970
short g_BattleLimitFuryBrandScaleValue = 0;
unsigned char g_bssPad_00aaf974[4] = {0};
// GLOBAL: FF7 0x00aaf978
struct BattleLimitFuryBrandScaleDelta g_BattleLimitFuryBrandScaleDelta = {0};
// clang-format on

#pragma intrinsic(memcpy)
#pragma function(memcpy)

// Loads the LIMIT/RAKUIN model table when the battle scene exists and the table is not loaded yet, then
// registers the Rakuin sound groups.
// FUNCTION: FF7 0x00496b50
void __cdecl BattleFx_LimitLoadResFuryBrand(void)
{
    GfxModelTableBank *newSceneField;
    GfxResourceRequest requestSlot;
    int kind;
    char pathText[200];
    GfxResourceRequest *resourcePtrPtr;
    int stepResourceDef;

    if (g_pBattle3dScene != 0 && g_BattleLimitFuryBrandLoadedResTable == 0) {
        kind = 5;
        stepResourceDef = (int)&g_BattleLimitFuryBrandResourceDef;
        newSceneField = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        resourcePtrPtr = &requestSlot;
        Gfx_InitResourceRequest(resourcePtrPtr);
        resourcePtrPtr->groupTag = 0;
        // STRING: FF7 0x007fbcc8
        resourcePtrPtr->resourcePath = BattleFx_BuildResourcePath("LIMIT/RAKUIN", pathText);
        Gfx_LoadModelTableIfAbsent(kind, stepResourceDef, resourcePtrPtr, &g_BattleLimitFuryBrandLoadedResTable, newSceneField);
        SfxRegisterSoundGroups((struct SoundGroup **)&g_BattleLimitFuryBrandSoundGroup);
    }
}

// Loads the Rakuin resources, then starts the effect on the target mask.
// FUNCTION: FF7 0x00496bf1
int __cdecl BattleFx_LimitEnterFuryBrand(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResFuryBrand();
    BattleFx_LimitBeginFuryBrand(targetMask, actorIdx);
    return 0;
}

// Starts a Rakuin timeline for each target and a sound timeline slot placed at the centroid of the
// targets.
// FUNCTION: FF7 0x00496c0d
void __cdecl BattleFx_LimitBeginFuryBrand(int targetMask, int actorIdx)
{
    BattleFxLimitFuryBrandSoundTimeline *slot;

    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 0, BattleFx_LimitSpawnFuryBrandPerTarget);
    slot = (BattleFxLimitFuryBrandSoundTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitFuryBrandSoundTimelineTask);
    Battle_GetActorMaskCentroid(targetMask, &slot->posX);
}

// Allocates the Rakuin timeline slot for one target: its actor, the world position of the actor's
// target bone, and the negated radius of the actor.
// FUNCTION: FF7 0x00496c5b
void __cdecl BattleFx_LimitSpawnFuryBrandPerTarget(int actor)
{
    BattleFxFuryBrandTask *slot;

    slot = (BattleFxFuryBrandTask *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitFuryBrandTimelineTask);
    slot->targetActor = actor;
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &slot->posX);
    slot->negatedRadius = -g_BattleActorRecords[actor].radius;
}

// Timeline of one Rakuin strike. At phases 55, 57 and 59 it starts a fading sprite at the centre with
// Z rotation -256, +256 and 0 and a screen flash, faded out at 56, 58 and 60. Phases 65 to 69 each
// emit three animated sprites at random angles around the centre, rising at random speed. Phase 75
// starts the fading-models spawner and the growing fade model. Phase 85 allocates the target's record and
// retires the record type. Does nothing while paused.
// FUNCTION: FF7 0x00496cc5
void __cdecl BattleFx_LimitFuryBrandTimelineTask(void)
{
    int angle;
    BattleFxFuryBrandSlot *task;
    BattleFxLimitFuryBrandPiece *pPiece;
    int index;

    task = (BattleFxFuryBrandSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->phase == 0x37) {
        pPiece = (BattleFxLimitFuryBrandPiece *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitFuryBrandFadingSpriteTask);
        pPiece->ownerSlot = 0;
        pPiece->posX = task->centerX;
        pPiece->posY = 0;
        pPiece->posZ = task->centerZ;
        pPiece->rotX = 0;
        pPiece->velocityY = 0;
        pPiece->rotZ = 0xFF00 /* 5737.5 deg */;
        pPiece->pushLength = task->negatedRadius;
        BattleFx_StartScreenFlashRamp(1);
    }
    if (task->phase == 0x38) {
        BattleFx_FadeOutScreenFlashRamp(1);
    }
    if (task->phase == 0x39) {
        pPiece = (BattleFxLimitFuryBrandPiece *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitFuryBrandFadingSpriteTask);
        pPiece->ownerSlot = 0;
        pPiece->posX = task->centerX;
        pPiece->posY = 0;
        pPiece->posZ = task->centerZ;
        pPiece->rotX = 0;
        pPiece->velocityY = 0;
        pPiece->rotZ = 0x100 /* 22.5 deg */;
        pPiece->pushLength = task->negatedRadius;
        BattleFx_StartScreenFlashRamp(1);
    }
    if (task->phase == 0x3A) {
        BattleFx_FadeOutScreenFlashRamp(1);
    }
    if (task->phase == 0x3B) {
        pPiece = (BattleFxLimitFuryBrandPiece *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitFuryBrandFadingSpriteTask);
        pPiece->ownerSlot = 1;
        pPiece->posX = task->centerX;
        pPiece->posY = 0;
        pPiece->posZ = task->centerZ;
        pPiece->rotX = 0;
        pPiece->velocityY = 0;
        pPiece->rotZ = 0;
        pPiece->pushLength = task->negatedRadius;
        BattleFx_StartScreenFlashRamp(1);
    }
    if (task->phase == 0x3C) {
        BattleFx_FadeOutScreenFlashRamp(1);
    }
    if (task->phase >= 65 && task->phase < 70) {
        for (index = 0; index < 3; index++) {
            pPiece = (BattleFxLimitFuryBrandPiece *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitFuryBrandAnimatedSpriteTask);
            pPiece->ownerSlot = rand() & 1;
            angle = rand() & 0xFFF;
            pPiece->posX = task->centerX + (SinFixed(angle) * 0x190 >> 12);
            pPiece->posY = -(rand() % 1000);
            pPiece->posZ = task->centerZ + (CosFixed(angle) * 0x190 >> 12);
            pPiece->velocityY = -(rand() % 50 + 50);
            pPiece->pushLength = 0;
        }
    }
    if (task->phase == 0x4B) {
        pPiece = (BattleFxLimitFuryBrandPiece *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitFuryBrandSpawnFadingModelsTask);
        pPiece->pos = task->pos;
    }
    if (task->phase == 0x4B) {
        pPiece = (BattleFxLimitFuryBrandPiece *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitFuryBrandGrowingFadeModelTask);
        pPiece->posX = task->centerX;
        pPiece->posZ = task->centerZ;
    }
    if (task->phase == 0x55) {
        Battle_PostActorHitOrHurtMessage(task->recordType);
    }
    if (task->phase == 0x55) {
        task->recordType = 0xFFFF;
    }
    (task->phase)++;
}

// Draws the fading Rakuin sprite: its colour holds for five frames, then fades from 128 to 0 over
// five more, after which the task retires. The sprite is rotated by the task's rotation, scaled by
// g_BattleLimitFuryBrandScaleVector and placed at the task's position pushed by pushLength. Only the
// frame counter is pause-gated.
// FUNCTION: FF7 0x00497086
void __cdecl BattleFx_LimitFuryBrandFadingSpriteTask(void)
{
    int age;
    BattleFuryBrandSpriteRecord *task;
    Vec3sPacked *matrix;

    task = (BattleFuryBrandSpriteRecord *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitFuryBrandSpriteReqA.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(2, g_BattleLimitFuryBrandLoadedResTable);
    g_BattleLimitFuryBrandSpriteReqA.spriteData = Gfx_ModelTableGetSpriteObject(2, g_BattleLimitFuryBrandLoadedResTable);
    matrix = (Vec3sPacked *)Gfx_GetPsxScratchpadPtr(0);
    age = task->frame;
    if (age < 5) {
        g_BattleLimitFuryBrandSpriteReqA.colorCommand = 0x2E808080;
    } else {
        age -= 5;
        if (age < 5) {
            g_BattleLimitFuryBrandSpriteReqA.colorB = age * -0x80 / 5 + 0x80;
            g_BattleLimitFuryBrandSpriteReqA.colorG = g_BattleLimitFuryBrandSpriteReqA.colorB;
            g_BattleLimitFuryBrandSpriteReqA.colorR = g_BattleLimitFuryBrandSpriteReqA.colorG;
            g_BattleLimitFuryBrandSpriteReqA.command = 0x2E;
        } else {
            task->drawFrameAndMode = 0xFFFF;
            return;
        }
    }
    RotMatrix((SVECTOR *)&task->rotX, (void *)matrix);
    ScaleMatrix(matrix, (int *)&g_BattleLimitFuryBrandScaleVector);
    BattleFx_MakeViewSpacePointPushed(&task->posX, task->pushLength, matrix);
    SetRotMatrix((void *)matrix);
    SetTransMatrix(matrix);
    g_BattleLimitFuryBrandSpriteReqA.frame = task->drawFrameAndMode;
    g_BattleLimitFuryBrandDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleLimitFuryBrandSpriteReqA, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitFuryBrandDrawHandle);
    if (g_BattleIsPaused == 0) {
        (task->frame)++;
    }
}

// Draws one animated Rakuin sprite as a billboard at its position: the table row is 3 or 4 by
// ownerSlot, and the frame is frameIndex. While unpaused the sprite rises by velocityY, its frame advances,
// and the task retires when the frame reaches the sprite group's frame count.
// FUNCTION: FF7 0x004971ef
void __cdecl BattleFx_LimitFuryBrandAnimatedSpriteTask(void)
{
    BattleFxLimitFuryBrandSpriteState *task;

    task = (BattleFxLimitFuryBrandSpriteState *)BATTLE_CURRENT_TASK_SLOT;
    if (task->ownerSlot == 0) {
        g_BattleLimitFuryBrandSpriteReqB.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(3, g_BattleLimitFuryBrandLoadedResTable);
        g_BattleLimitFuryBrandSpriteReqB.spriteData = Gfx_ModelTableGetSpriteObject(3, g_BattleLimitFuryBrandLoadedResTable);
    } else {
        g_BattleLimitFuryBrandSpriteReqB.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(4, g_BattleLimitFuryBrandLoadedResTable);
        g_BattleLimitFuryBrandSpriteReqB.spriteData = Gfx_ModelTableGetSpriteObject(4, g_BattleLimitFuryBrandLoadedResTable);
    }
    g_BattleLimitFuryBrandSpriteReqB.frame = task->frameIndex;
    BattleFx_InstallBillboardMatrix(&task->posX, 0x1800 /* 1.5 */, task->pushLength);
    g_BattleLimitFuryBrandDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleLimitFuryBrandSpriteReqB.spriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitFuryBrandDrawHandle);
    if (g_BattleIsPaused == 0) {
        task->posY += task->velocityY;
        if (g_BattleLimitFuryBrandSpriteReqB.spriteGroup != 0) {
            (task->frameIndex)++;
            if (task->frameIndex >= g_BattleLimitFuryBrandSpriteReqB.spriteGroup->header->frameCount) {
                task->ownerSlot = BATTLE_TASK_FREE;
            }
        }
    }
}

// Spawns two fading models per frame for 15 frames, on a circle of angle ((frame * 2 + i) << 11) / 30:
// each child copies the position, rises by -(((0x1000 - CosFixed(angle)) * 0x1F4) >> 12) and takes
// (SinFixed(angle) * 0x300) >> 12 as its scale. Does nothing while paused.
// FUNCTION: FF7 0x00497310
void __cdecl BattleFx_LimitFuryBrandSpawnFadingModelsTask(void)
{
    int angle;
    BattleFxRubyTentaclesRadialEmitter *pTaskSlot;
    int i;
    BattleFxFuryBrandFadingSlot *child;

    pTaskSlot = (BattleFxRubyTentaclesRadialEmitter *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    for (i = 0; i < 2; i++) {
        child = (BattleFxFuryBrandFadingSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitFuryBrandFadingModelTask);
        angle = ((pTaskSlot->frame * 2 + i) << 11) / 30;
        child->posX = pTaskSlot->posX;
        child->posY = -((4096 - CosFixed(angle)) * 0x1F4 >> 12);
        child->posZ = pTaskSlot->posZ;
        child->scale = SinFixed(angle) * 0x300 /* 0.1875 */ >> 12;
    }
    (pTaskSlot->frame)++;
    if (pTaskSlot->frame >= 15) {
        pTaskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Draws a fading model at its position, scaled by the task's scale, with the fade level growing as
// frameIndex << 12 over 8. While unpaused it advances the frame and retires at frame 8.
// FUNCTION: FF7 0x00497417
void __cdecl BattleFx_LimitFuryBrandFadingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[40];
        struct {
            unsigned char pad[18];
            int values[3];
        } zero;
        short localMatrix;
        struct { unsigned char pad_to_2[0x2]; short matR2C1; };
        struct { unsigned char pad_to_4[0x4]; short matR2C0; };
        struct { unsigned char pad_to_6[0x6]; short matR1C2; };
        struct { unsigned char pad_to_8[0x8]; short diagY; };
        struct { unsigned char pad_to_A[0xA]; short matR1C0; };
        struct { unsigned char pad_to_C[0xC]; short matR0C2; };
        struct { unsigned char pad_to_E[0xE]; short matR0C1; };
        struct { unsigned char pad_to_10[0x10]; short diagX; };
        struct { unsigned char pad_to_12[0x12]; int transX; };
        struct { unsigned char pad_to_16[0x16]; int transY; };
        struct { unsigned char pad_to_1A[0x1A]; int transZ; };
        struct { unsigned char pad_to_20[0x20]; int dataBlock; };
        struct { unsigned char pad_to_24[0x24]; BattleFxLimitFuryBrandFadeState *fadeState; };
    } frame;
#pragma pack(pop)

    frame.fadeState = (BattleFxLimitFuryBrandFadeState *)BATTLE_CURRENT_TASK_SLOT;
    frame.localMatrix = 0;
    frame.matR2C1 = 0;
    frame.matR2C0 = 0;
    frame.matR1C2 = 0;
    frame.diagY = 0;
    frame.matR1C0 = 0;
    frame.matR0C2 = 0;
    frame.matR0C1 = 0;
    frame.diagX = 0;
    memset(frame.zero.values, 0, 12);
    g_BattleLimitFuryBrandModelReqA.modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleLimitFuryBrandLoadedResTable);
    frame.dataBlock = Gfx_GetPsxScratchpadPtr(0);
    g_BattleLimitFuryBrandModelReqA.fadeLevel = (frame.fadeState->frameIndex << 12) / 8;
    frame.diagX = frame.fadeState->scale;
    frame.diagY = frame.diagX;
    frame.localMatrix = frame.diagY;
    frame.transX = frame.fadeState->posX;
    frame.transY = frame.fadeState->posY;
    frame.transZ = frame.fadeState->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.localMatrix, (void *)frame.dataBlock);
    SetRotMatrix((void *)frame.dataBlock);
    SetTransMatrix((void *)frame.dataBlock);
    SetDrawColorRGB(0, 0, 0);
    g_BattleLimitFuryBrandDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitFuryBrandModelReqA.modelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitFuryBrandDrawHandle);
    if (g_BattleIsPaused == 0) {
        (frame.fadeState->frameIndex)++;
        if (frame.fadeState->frameIndex >= 8) {
            frame.fadeState->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

// Draws the growing fade model: the fade level stays 0 for the first five frames and then ramps up over
// ten, the scale grows with the frame over 15 frames, and the task retires at frame 15. The frame
// advances only when unpaused.
// FUNCTION: FF7 0x0049757a
void __cdecl BattleFx_LimitFuryBrandGrowingFadeModelTask(void)
{
    int age;
    BattleFxFuryBrandGrowFadeModel *task;
    int matrix;

    task = (BattleFxFuryBrandGrowFadeModel *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitFuryBrandModelReqB.modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleLimitFuryBrandLoadedResTable);
    matrix = Gfx_GetPsxScratchpadPtr(0);
    age = task->frame;
    if (age < 5) {
        g_BattleLimitFuryBrandModelReqB.fadeLevel = 0;
    } else {
        age -= 5;
        if (age < 10) {
            g_BattleLimitFuryBrandModelReqB.fadeLevel = (age << 12) / 10;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    (g_BattleLimitFuryBrandScaleDelta.scale) = (task->frame << 12) / 15;
    g_BattleLimitFuryBrandScaleValue = (g_BattleLimitFuryBrandScaleDelta.scale);
    g_BattleLimitFuryBrandScaleMatrix = g_BattleLimitFuryBrandScaleValue;
    g_BattleLimitFuryBrandScaleDelta.x = task->scaleDeltaX;
    g_BattleLimitFuryBrandScaleDelta.y = task->scaleDeltaY;
    MultiplyMatrix(&g_BattleCameraTransform, &g_BattleLimitFuryBrandScaleMatrix, (void *)matrix);
    SetRotMatrix((void *)matrix);
    SetTransMatrix((void *)matrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleLimitFuryBrandDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitFuryBrandModelReqB.modelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitFuryBrandDrawHandle);
    if (g_BattleIsPaused == 0) {
        (task->frame)++;
    }
}

// Plays the Rakuin sounds while unpaused: sound 0x13A at frame 55, and sound 0x174 at frame 75, where it
// also retires the slot and releases both cached sounds from group 0xF.
// FUNCTION: FF7 0x004976d0
void __cdecl BattleFx_LimitFuryBrandSoundTimelineTask(void)
{
    BattleFxLimitFuryBrandSoundTimeline *task;

    task = (BattleFxLimitFuryBrandSoundTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 55) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&task->posX), 0x13A);
    } else if (task->frameCounter == 75) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&task->posX), 0x174);
        task->ownerSlot = BATTLE_TASK_FREE;
        Sfx_ReleaseCachedBuffer(0x13A, 0xF);
        Sfx_ReleaseCachedBuffer(0x174, 0xF);
    }
    (task->frameCounter)++;
}
} /* extern "C" */
