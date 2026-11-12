// src/ff7/src/battle/battle_fx_magic_fire3.cpp
/* Magic Effect [29]: Fire3 (Japanese: ファイガ Firaga, dev tag: "MAGIC/FAIGA" / Faiga) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Firaga (Faiga) magic effect: an eruption and explosion timeline with spinning, growing
//   and fading flame models, a flash sprite and animated ember sprites.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_magic.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x008affb8
int g_BattleMagicFire3ResourceDefs = 0x00192A39;
char *g_BattleFxMagicFire3ResourceNames[1] = {
    // STRING: FF7 0x008b0028
    "FAIGA_3.TIM"
};
unsigned int g_BattleFxMagicFire3ResourceNames2[18] = {
    // STRING: FF7 0x008b0034
    (unsigned int)"FAIGA_3.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008b0040
    (unsigned int)"FAIGA_4.TIM",
    // STRING: FF7 0x008b004c
    (unsigned int)"FAIGA_1.SPT",
    0x00000000,
    0x00000000,
    0x004b00b1,
    // STRING: FF7 0x008b0058
    (unsigned int)"FAIGA_1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x004b00b1,
    // STRING: FF7 0x008b0064
    (unsigned int)"FAIGA_2.RSD",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x008b0008
struct BattleMagicFire3SpriteReq g_BattleMagicFire3SpriteReq = {0, 0, {0x80, 0x80, 0x80, 0x2e, 0, 0, 0, 0}, 0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x00bb0814
int g_BattleMagicFire3TargetMask = 0;
// GLOBAL: FF7 0x00bb0818
struct GfxTableCursor *g_BattleMagicFire3LoadedResTable = 0;
// clang-format on

/* Loads and registers resources for Firaga (Faiga). */
// FUNCTION: FF7 0x0057e260
void __cdecl BattleFx_MagicLoadResFire3(void)
{
    GfxModelTableBank *holderRef;
    GfxResourceRequest requestStorage8;
    int kind;
    char tempPathOutName[200];
    GfxResourceRequest *pRequest;
    int defItemsPointer;

    if (g_pBattle3dScene != 0 && g_BattleMagicFire3LoadedResTable == 0) {
        kind = 4;
        defItemsPointer = (int)&g_BattleMagicFire3ResourceDefs;
        holderRef = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestStorage8;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008b0070
        pRequest->resourcePath = BattleFx_BuildResourcePath("magic/faiga", tempPathOutName);
        Gfx_LoadModelTableIfAbsent(kind, defItemsPointer, pRequest, &g_BattleMagicFire3LoadedResTable, holderRef);
    }
}

/* Entry point for the Firaga (Faiga) magic effect. */
// FUNCTION: FF7 0x0057e2f0
void __cdecl BattleFx_MagicEnterFire3(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResFire3();
    BattleFx_MagicBeginFire3(targetMask, actorIdx);
}

/* Initializes Firaga (Faiga) effect tasks and audio. */
// FUNCTION: FF7 0x0057e30a
void __cdecl BattleFx_MagicBeginFire3(int targetMask, int actorIdx)
{
    g_BattleFxDrawListBufferTaskSlotPtr = (unsigned char *)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    Battle_AllocateResourceSlot(BattleFx_MagicFire3SetupTask);
    g_BattleMagicFire3TargetMask = targetMask;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x8E);
}

/* Sets up target centroid and spawns fire model tasks for Firaga. */
// FUNCTION: FF7 0x0057e35e
void __cdecl BattleFx_MagicFire3SetupTask(void)
{
    int activeSize, actorIdx, radiusQ12, count, solo;
    BattleFxFire3SetupRecord *timelinePtr;
    BattleFxFire3FlashTask *task;

    actorIdx = 0;
    for (count = 0; actorIdx < 0xA; actorIdx++) {
        if ((g_BattleMagicFire3TargetMask & 1 << actorIdx) != 0) {
            count++;
            if (count == 1) {
                solo = actorIdx;
            }
        }
    }
    task = (BattleFxFire3FlashTask *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    task = (BattleFxFire3FlashTask *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFire3FlashSpriteTask);
    if (count == 1) {
        Battle_GetBoneWorldPos(solo, g_BattleActorRecords[solo].targetBone, (short *)((char *)task + 4));
        activeSize = g_BattleActorRecords[solo].radius + 0x100;
        if (activeSize > 0x76C) {
            activeSize = 0x76C;
        }
    } else {
        Battle_GetActorMaskCentroid(g_BattleMagicFire3TargetMask, (short *)((char *)task + 4));
        task->posY = 0xFB00;
        activeSize = 0x600;
    }
    radiusQ12 = activeSize << 0xC;
    task->flashLevel = 0x5555;
    task->flashStep = task->flashLevel / 8;
    task->flashLevelReset = task->flashLevel;
    task->billboardDistance = activeSize >> 1;
    timelinePtr = (BattleFxFire3SetupRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFire3TimelineTask);
    *(Int64Pair *)((char *)timelinePtr + 4) = *(Int64Pair *)((char *)task + 4);
    timelinePtr->radius = activeSize;
    timelinePtr = (BattleFxFire3SetupRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFire3SpawnAnimatedSpritesTask);
    *(Int64Pair *)((char *)timelinePtr + 4) = *(Int64Pair *)((char *)task + 4);
    timelinePtr->radius = activeSize;
    timelinePtr->angle = 0;
    timelinePtr->angleStep = 0x80;
}

/* Main timeline task controlling Firaga eruption and explosion phases. */
// FUNCTION: FF7 0x0057e52b
void __cdecl BattleFx_MagicFire3TimelineTask(void)
{
    short ramp;
    int i, extent;
    BattleFxFire3TaskSlot *selfPtr;
    BattleFxFire3Timeline *child;

    if (g_BattleIsPaused != 0) {
        return;
    }
    selfPtr = (BattleFxFire3TaskSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (selfPtr->step == 8) {
        extent = selfPtr->radius << 0xC;
        child = (BattleFxFire3Timeline *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFire3SpinGrowFadeModel2Task);
        child->spinAngle = 0;
        child->riseSpeed = 0;
        child->offsetY = 0xFFEC;
        *(Int64Pair *)((char *)child + 4) = *(Int64Pair *)((char *)selfPtr + 4);
        child->spread = extent / 0x2EE >> 1;
        child->step = (extent / 0x2EE) / 0x20;
        child = (BattleFxFire3Timeline *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFire3SpinGrowFadeModel3EarlyFadeTask);
        child->spinAngle = 0;
        child->riseSpeed = 0xFC00;
        child->offsetY = 0xFFDE;
        *(Int64Pair *)((char *)child + 4) = *(Int64Pair *)((char *)selfPtr + 4);
        child->spread = extent / 0x26C >> 1;
        child->step = (extent / 0x26C) / 0x20;
        child = (BattleFxFire3Timeline *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFire3SpinGrowFadeModel3Task);
        child->spinAngle = 0;
        child->riseSpeed = 0xFC00;
        child->offsetY = 0xFFE6;
        *(Int64Pair *)((char *)child + 4) = *(Int64Pair *)((char *)selfPtr + 4);
        child->spread = extent / 0x258 >> 1;
        child->step = (extent / 0x258) / 0x20;
    }
    if (selfPtr->step < 8) {
        g_BattleFxTimelineState.red = 0x20;
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        ramp = selfPtr->step * 0x1C0;
    } else if (selfPtr->step >= 0x26) {
        ramp = 0xE00 - (selfPtr->step - 0x26) * 0x255;
        if (ramp < 0) {
            ramp = 0;
        }
    } else {
        ramp = 0xE00;
    }
    if (selfPtr->step == 0x1D) {
        for (i = 0; i < 0xA; i++) {
            if ((g_BattleMagicFire3TargetMask & 1 << i) != 0) {
                Battle_PostActorHitOrHurtMessage(i);
            }
        }
    }
    if (selfPtr->step >= 0x2E) {
        ramp = 0;
        g_BattleFxTimelineState.red = ramp;
        selfPtr->ownerSlot = BATTLE_TASK_FREE;
    }
    g_BattleFxTimelineState.startValue = ramp;
    (selfPtr->step)++;
}

/* Animates spinning, growing, and fading secondary model for Firaga. */
// FUNCTION: FF7 0x0057e7d6
void __cdecl BattleFx_MagicFire3SpinGrowFadeModel2Task(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxFire3SpinGrowFadeModel2Slot *spinSlot; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix matrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_30[0x30]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.spinSlot = (BattleFxFire3SpinGrowFadeModel2Slot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.spinSlot->angle + frame.spinSlot->angleOffset;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.matrix);
    frame.matrix.t[0] = frame.spinSlot->posX;
    frame.matrix.t[1] = frame.spinSlot->posY;
    frame.matrix.t[2] = frame.spinSlot->posZ;
    frame.scaleX.vz = frame.spinSlot->scale;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.matrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(2, g_BattleMagicFire3LoadedResTable);
    frame.modelDraw->flags = 8;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0x80;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    if (frame.spinSlot->frameCounter < 0xD) {
        frame.modelDraw->fadeLevel -= (short)((0xD - frame.spinSlot->frameCounter) * 9);
    } else if (frame.spinSlot->frameCounter >= 0x14) {
        frame.modelDraw->fadeLevel -= (short)((frame.spinSlot->frameCounter - 0x14) * 0xE);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.spinSlot->frameCounter >= 0x1D) {
        frame.spinSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spinSlot->scale += frame.spinSlot->scaleStep;
    if (frame.spinSlot->scale < 0) {
        frame.spinSlot->scale = 0x7000;
    }
    frame.spinSlot->angle += frame.spinSlot->angleStep;
    (frame.spinSlot->frameCounter)++;
}

/* Animates tertiary fire model variation for Firaga. */
// FUNCTION: FF7 0x0057e9d4
void __cdecl BattleFx_MagicFire3SpinGrowFadeModel3EarlyFadeTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxFire3SpinGrowFadeModel3Slot *spinSlot; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix matrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_30[0x30]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.spinSlot = (BattleFxFire3SpinGrowFadeModel3Slot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.spinSlot->scale + frame.spinSlot->scaleBias;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.matrix);
    frame.matrix.t[0] = frame.spinSlot->posX;
    frame.matrix.t[1] = frame.spinSlot->posY;
    frame.matrix.t[2] = frame.spinSlot->posZ;
    frame.scaleX.vz = frame.spinSlot->angle;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.matrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(3, g_BattleMagicFire3LoadedResTable);
    frame.modelDraw->flags = 8;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0x80;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    if (frame.spinSlot->frameCounter < 0xD) {
        frame.modelDraw->fadeLevel -= (short)((0xD - frame.spinSlot->frameCounter) * 9);
    } else if (frame.spinSlot->frameCounter >= 0x16) {
        frame.modelDraw->fadeLevel -= (short)((frame.spinSlot->frameCounter - 0x16) * 9);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.spinSlot->frameCounter >= 0x23) {
        frame.spinSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spinSlot->angle += frame.spinSlot->angleStep;
    if (frame.spinSlot->angle < 0) {
        frame.spinSlot->angle = 0x7000;
    }
    frame.spinSlot->scale += frame.spinSlot->scaleStep;
    (frame.spinSlot->frameCounter)++;
}

/* Animates spinning and growing flame model for Firaga. */
// FUNCTION: FF7 0x0057ebd2
void __cdecl BattleFx_MagicFire3SpinGrowFadeModel3Task(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxFire3SpinGrowFadeModel3Slot *spinSlot; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix matrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_30[0x30]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.spinSlot = (BattleFxFire3SpinGrowFadeModel3Slot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.spinSlot->scale + frame.spinSlot->scaleBias;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.matrix);
    frame.matrix.t[0] = frame.spinSlot->posX;
    frame.matrix.t[1] = frame.spinSlot->posY;
    frame.matrix.t[2] = frame.spinSlot->posZ;
    frame.scaleX.vz = frame.spinSlot->angle;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.matrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(3, g_BattleMagicFire3LoadedResTable);
    frame.modelDraw->flags = 8;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0x80;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    if (frame.spinSlot->frameCounter < 0xD) {
        frame.modelDraw->fadeLevel -= (short)((0xD - frame.spinSlot->frameCounter) * 9);
    } else if (frame.spinSlot->frameCounter >= 0x17) {
        frame.modelDraw->fadeLevel -= (short)((frame.spinSlot->frameCounter - 0x17) * 9);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.spinSlot->frameCounter >= 0x24) {
        frame.spinSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spinSlot->angle += frame.spinSlot->angleStep;
    if (frame.spinSlot->angle < 0) {
        frame.spinSlot->angle = 0x7000;
    }
    frame.spinSlot->scale += frame.spinSlot->scaleStep;
    (frame.spinSlot->frameCounter)++;
}

/* Animates screen flash sprite for Firaga. */
// FUNCTION: FF7 0x0057edd0
void __cdecl BattleFx_MagicFire3FlashSpriteTask(void)
{
    BattleFxFire3FlashSprite *task;

    task = (BattleFxFire3FlashSprite *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix((char *)task + 4, (short)task->level, -task->billboardDistance);
    g_BattleMagicFire3SpriteReq.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleMagicFire3LoadedResTable);
    g_BattleMagicFire3SpriteReq.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleMagicFire3LoadedResTable);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleMagicFire3SpriteReq.spriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->timer >= 0xC) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    /* Numeric flame timeline tick index (0..11). */
    switch (task->timer) {
    case BATTLE_FIRE3_TICK_SHRINK_0:
    case BATTLE_FIRE3_TICK_SHRINK_1:
        task->level -= task->levelStep;
        break;
    case BATTLE_FIRE3_TICK_FLASH_START_1:
        BattleFx_StartScreenFlashRamp(1);
        task->level -= task->levelStep;
        break;
    case BATTLE_FIRE3_TICK_FLASH_FADE_1:
        BattleFx_FadeOutScreenFlashRamp(2);
        task->level -= task->levelStep;
        break;
    case BATTLE_FIRE3_TICK_RESET_LEVEL:
        task->level = task->levelReset;
        break;
    case BATTLE_FIRE3_TICK_SHRINK_2:
        task->level -= task->levelStep;
        break;
    case BATTLE_FIRE3_TICK_FLASH_START_2:
        BattleFx_StartScreenFlashRamp(1);
        task->level -= task->levelStep;
        break;
    case BATTLE_FIRE3_TICK_FLASH_FADE_2:
        BattleFx_FadeOutScreenFlashRamp(4);
    case BATTLE_FIRE3_TICK_SHRINK_3:
    case BATTLE_FIRE3_TICK_SHRINK_4:
    case BATTLE_FIRE3_TICK_SHRINK_5:
    case BATTLE_FIRE3_TICK_SHRINK_6:
        task->level -= task->levelStep;
    }
    (task->timer)++;
}

/* Spawns animated ember and burst sprites for Firaga. */
// FUNCTION: FF7 0x0057efa7
void __cdecl BattleFx_MagicFire3SpawnAnimatedSpritesTask(void)
{
    int i6, offsetMax, w, scaleFrac, seed;
    BattleFxFire3SpriteRecord *pSprite;
    BattleFxFire33SpinModel *spawner;

    spawner = (BattleFxFire33SpinModel *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (spawner->frameCount >= 3 && spawner->frameCount <= 0x24) {
        offsetMax = spawner->radius;
        scaleFrac = (offsetMax << 0xC) / 0x28A;
        offsetMax += offsetMax >> 1;
        w = offsetMax << 1;
        for (i6 = 0; i6 < 3; i6++) {
            pSprite = (BattleFxFire3SpriteRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFire3AnimatedSpriteTask);
            *(Int64Pair *)((char *)pSprite + 4) = *(Int64Pair *)((char *)spawner + 4);
            pSprite->posX += (short)(rand() % w - offsetMax);
            pSprite->posY += (short)(rand() % w - offsetMax);
            pSprite->posZ += (short)(rand() % w - offsetMax);
            seed = spawner->angle + rand() % 0x1000 + 0x800;
            pSprite->size = seed * scaleFrac >> 0xC;
        }
        spawner->angle += spawner->angleStep;
    }
    if (spawner->frameCount >= 0x2E) {
        spawner->ownerSlot = BATTLE_TASK_FREE;
    }
    (spawner->frameCount)++;
}

/* Animates and renders flame sprite for Firaga. */
// FUNCTION: FF7 0x0057f13b
void __cdecl BattleFx_MagicFire3AnimatedSpriteTask(void)
{
    BattleFire3AnimatedSprite *task;

    task = (BattleFire3AnimatedSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicFire3SpriteReq.frameIndex = task->frameIndex;
    BattleFx_InstallBillboardMatrix((char *)task + 4, (short)task->size, -(task->size >> 2));
    g_BattleMagicFire3SpriteReq.secondSpriteGroup = Gfx_ModelTableGetLoadedRecord(1, g_BattleMagicFire3LoadedResTable);
    g_BattleMagicFire3SpriteReq.secondDrawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleMagicFire3LoadedResTable);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleMagicFire3SpriteReq.secondSpriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->frameIndex >= 7) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (task->frameIndex)++;
}
} /* extern "C" */
