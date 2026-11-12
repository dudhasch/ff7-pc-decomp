// src/ff7/src/battle/battle_fx_blue_trine_effect.cpp
// Enemy Skill [22]: Shadow Flare (Japanese: シャドウフレア, dev tag: "blue/sflea" / Sflea)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The Sflea blue magic effect: a shaded actor and flat color strips, with delayed sprite
//   spawns, grow-and-fade sprites and a frame-table sprite.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "lib/codegen.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_fx.h"
#include "lib/types/graphics.h"
#include "ff7/camera_script.h"
#include <string.h>
#include <stdlib.h>
extern "C" {

struct GfxPsxSpriteQuadState;

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// GLOBAL: FF7 0x008aca58
int g_BattleBlueShadowFlareResourceDefs = 79928;
int g_BattleBlueShadowFlareResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxBlueShadowFlareResourceNames + 0x8)
};
// clang-format on

// clang-format off
int g_BattleBlueShadowFlareResourceDefsDataTail[28] = {
    0x0, 0x0, 0x0, 0x192a38, (int)((char *)&g_BattleFxBlueShadowFlareResourceNames + 0x14), (int)((char *)&g_BattleFxBlueShadowFlareResourceNames + 0x20),
    0x0, 0x0, 0x192a38, (int)((char *)&g_BattleFxBlueShadowFlareResourceNames + 0x2c), (int)((char *)&g_BattleFxBlueShadowFlareResourceNames + 0x38), 0x0,
    0x0, 0x192a38, (int)((char *)&g_BattleFxBlueShadowFlareResourceNames + 0x44), (int)((char *)&g_BattleFxBlueShadowFlareResourceNames + 0x50), 0x0, 0x0,
    0x4b00b0, (int)((char *)&g_BattleFxBlueShadowFlareResourceNames + 0x5c), 0x0, 0x0, 0x0, 0x1900b0,
    (int)((char *)&g_BattleFxBlueShadowFlareResourceNames + 0x68), 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x008acad0
short g_BattleBlueShadowFlareFrameIndexTable[6] = {7, 5, 3, 1, 0, 0};
unsigned char g_rawData_008acadc[4] = {0x01, 0x00, 0x02, 0x00};

// clang-format off
unsigned char g_rawData_008acae0[8] = {0x03, 0x00, 0x04, 0x00, 0x06, 0x00, 0x07, 0x00};
// GLOBAL: FF7 0x008acae8
struct BattleBlueShadowFlareSpriteReq1 g_BattleBlueShadowFlareSpriteReq1 = {0, 0, -128, -128, -128, {0x2e}, 0};
unsigned char g_rawData_008acaf8[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x2e, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008acb08
struct BattleBlueShadowFlareSpriteReq2 g_BattleBlueShadowFlareSpriteReq2 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0};
// GLOBAL: FF7 0x008acb18
struct BattleBlueShadowFlareSpriteReq3 g_BattleBlueShadowFlareSpriteReq3 = {0, 0, -128, -128, -128, {0x2e}, 0};
// GLOBAL: FF7 0x008acb28
unsigned char g_BattleBlueShadowFlareOffsetTableX[4] = {0x00, 0x00, 0xe0, 0xfc};
unsigned char g_rawData_008acb2c[4] = {0x5c, 0xfe, 0xc8, 0x00};
unsigned char g_rawData_008acb30[80] = {
    0xda, 0xfd, 0x36, 0x01, 0x1c, 0x02, 0x42, 0xff, 0x76, 0xfd, 0x28, 0x00, 0x74, 0xff, 0xe6, 0x00, 0x10, 0xff, 0x5e, 0x01, 0x52, 0xfe, 0x76, 0x02,
    0x2e, 0xff, 0x30, 0xfd, 0xbe, 0x00, 0xc2, 0x01, 0xca, 0xfe, 0x6c, 0x02, 0x7a, 0xfe, 0xe6, 0x00, 0xfc, 0xfe, 0x08, 0x02, 0x9e, 0xfd, 0x86, 0x01,
    0x26, 0xfd, 0xfe, 0x01, 0x16, 0xfe, 0x68, 0x01, 0x28, 0x00, 0x7d, 0x00, 0x1f, 0xff, 0x45, 0x01, 0x39, 0xfe, 0xc7, 0xfc, 0x0f, 0x00, 0x2c, 0x01,
    0xfd, 0xfd, 0xbb, 0xfe, 0x55, 0x00, 0x5d, 0xfd
};
unsigned char g_rawData_008acb80[8] = {0x9a, 0x01, 0x0d, 0xfd, 0x11, 0xfe, 0xaf, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008acb88
unsigned char g_BattleBlueShadowFlareOffsetTableY[4] = {0x00, 0x00, 0xdc, 0x05};
unsigned char g_rawData_008acb8c[4] = {0xb8, 0x0b, 0xf4, 0x01};
unsigned char g_rawData_008acb90[80] = {
    0xe8, 0x03, 0x6c, 0x07, 0xa0, 0x0f, 0xe4, 0x0c, 0x1c, 0x0c, 0x14, 0x05, 0xb8, 0x01, 0x34, 0x08, 0xfa, 0x0a, 0x58, 0x02, 0x64, 0x00, 0xfc, 0x08,
    0x8c, 0x0a, 0xe6, 0x00, 0xd0, 0x07, 0xb0, 0x04, 0xd8, 0x0e, 0x20, 0x03, 0xc4, 0x09, 0x96, 0x00, 0x2c, 0x01, 0x4c, 0x04, 0x3a, 0x0c, 0x28, 0x0a,
    0x84, 0x03, 0xaa, 0x05, 0x72, 0x06, 0x60, 0x09, 0x72, 0x06, 0x86, 0x01, 0xb2, 0x0c, 0x66, 0x08, 0x70, 0x03, 0x86, 0x0b, 0x14, 0x05, 0x14, 0x00,
    0x9e, 0x07, 0x38, 0x09, 0x6e, 0x0f, 0x26, 0x02
};
unsigned char g_BattleFxBlueShadowFlareResourceNames[116] = {
    0x3a, 0x07, 0xde, 0x0d, 0xaa, 0x0a, 0x52, 0x03, 0x46, 0x4c, 0x45, 0x41, 0x5f, 0x35, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x46, 0x4c, 0x45, 0x41,
    0x5f, 0x34, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x45, 0x58, 0x50, 0x5f, 0x31, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x46, 0x4c, 0x45, 0x41,
    0x5f, 0x33, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x45, 0x58, 0x50, 0x5f, 0x32, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x46, 0x4c, 0x45, 0x41,
    0x5f, 0x35, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x45, 0x58, 0x50, 0x5f, 0x33, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x66, 0x6c, 0x65, 0x61,
    0x5f, 0x31, 0x2e, 0x72, 0x73, 0x64, 0x00, 0x00, 0x66, 0x6c, 0x65, 0x61, 0x5f, 0x32, 0x2e, 0x72, 0x73, 0x64, 0x00, 0x00
};
// GLOBAL: FF7 0x00bab060
int g_BattleBlueShadowFlareTargetActor = 0;
// GLOBAL: FF7 0x00bab064
struct BattleBlueShadowFlareDrawColor g_BattleBlueShadowFlareDrawColor = {0};
// GLOBAL: FF7 0x00bab068
short g_BattleBlueShadowFlareShadeIntensity = 0;
unsigned char g_bssPad_00bab06c[4] = {0};
// GLOBAL: FF7 0x00bab070
unsigned char g_BattleBlueShadowFlareSavedActorCoord[4] = {0};
// GLOBAL: FF7 0x00bab074
struct BattleBlueShadowFlareSavedActorAlpha g_BattleBlueShadowFlareSavedActorAlpha = {0};
unsigned char g_bssPad_00bab078[71] = {0};
// GLOBAL: FF7 0x00bab0c0
struct GfxTableCursor *g_BattleBlueShadowFlareLoadedResTable = 0;
unsigned char g_bssPad_00bab0c4[4] = {0};
// clang-format on


/* Loads and registers resources for the Sflea blue magic effect. */
// FUNCTION: FF7 0x00576f40
void __cdecl BattleFx_BlueLoadResShadowFlare(void)
{
    GfxModelTableBank *newSceneField;
    GfxResourceRequest requestSlot;
    int kind;
    char resPathBuf[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleBlueShadowFlareLoadedResTable == 0) {
        kind = 6;
        resourceDefsWord = (int)&g_BattleBlueShadowFlareResourceDefs;
        newSceneField = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008acc54
        pRequest->resourcePath = BattleFx_BuildResourcePath("blue/sflea", resPathBuf);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleBlueShadowFlareLoadedResTable, newSceneField);
    }
}

/* Entry point for the Sflea blue magic effect. */
// FUNCTION: FF7 0x00576fd0
void __cdecl BattleFx_BlueEnterShadowFlare(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResShadowFlare();
    BattleFx_BlueBeginShadowFlare(targetMask, actorIdx);
}

/* Initializes Sflea blue magic effect tasks and audio. */
// FUNCTION: FF7 0x00576fea
void __cdecl BattleFx_BlueBeginShadowFlare(int targetMask, int sourceActorIdx)
{
    int actorIdx, targetCount;

    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    actorIdx = 0;
    for (targetCount = 0; actorIdx < 0xA; actorIdx++) {
        if ((targetMask & 1 << actorIdx) != 0) {
            targetCount++;
            if (targetCount == 1) {
                g_BattleBlueShadowFlareTargetActor = actorIdx;
            }
        }
    }
    Battle_AllocateResourceSlot(BattleFx_BlueShadowFlareActorFadeTask);
    g_BattleFxTimelineState.channel2 = 0;
    g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
    g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    g_BattleBlueShadowFlareDrawColor.isActive = 0;
}

/* Fades and shades actor geometry during the Sflea effect. */
// FUNCTION: FF7 0x0057708e
void __cdecl BattleFx_BlueShadowFlareActorFadeTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[24];
        BattleTaskSlot *sfleaSlot;
        struct { unsigned char pad_to_8[0x8]; int tintPhase; };
        struct { unsigned char pad_to_C[0xC]; int actorIndex; };
        struct { unsigned char pad_to_10[0x10]; char red; };
        struct { unsigned char pad_to_11[0x11]; char green; };
        struct { unsigned char pad_to_12[0x12]; char blue; };
        struct { unsigned char pad_to_14[0x14]; short shadeIntensity; };
    } frame;
#pragma pack(pop)

    if (g_BattleBlueShadowFlareDrawColor.isActive != 0) {
        SetDrawColorRGB((g_BattleBlueShadowFlareDrawColor.colorR), g_BattleBlueShadowFlareDrawColor.green, g_BattleBlueShadowFlareDrawColor.blue);
        frame.blue = 0;
        frame.green = frame.blue;
        frame.red = frame.green;
        Gfx_ShadePsxVertexColor((CVECTOR *)&frame.red, g_BattleBlueShadowFlareShadeIntensity, (CVECTOR *)&frame.red);
        g_BattleBlueShadowFlareDrawColor.isActive = 1;
        /* Retail caller prototype (char, char, char) -> void preserved for /Od argument promotion on `BattleFx_BlueShadowFlareDrawPsxFlatColorStrips`. */
        ((void(__cdecl *)(char, char, char))BattleFx_BlueShadowFlareDrawPsxFlatColorStrips)((char)frame.red, (char)frame.green, (char)frame.blue);
        g_BattleStageHideFlags |= 7;
    } else {
        g_BattleStageHideFlags &= 0xF8;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.sfleaSlot = BATTLE_CURRENT_TASK_SLOT;
    if (frame.sfleaSlot->frameCounter == 0) {
        for (frame.actorIndex = 0; frame.actorIndex < 0xA; frame.actorIndex++) {
            ((BattleBlueShadowFlareSavedActorCoordEntry *)(g_BattleBlueShadowFlareSavedActorCoord))[frame.actorIndex].tintPhase = g_BattleActorRecords[frame.actorIndex].tintPhase;
            ((BattleBlueShadowFlareSavedActorCoordEntry *)(g_BattleBlueShadowFlareSavedActorCoord))[frame.actorIndex].savedTintPulse = g_BattleActorRecords[frame.actorIndex].tintPulse;
            *(char *)(((char *)&g_BattleBlueShadowFlareSavedActorAlpha) + frame.actorIndex * 8) = g_BattleActorRecords[frame.actorIndex].tintR;
            *(char *)(&g_BattleBlueShadowFlareSavedActorAlpha.savedTintG + frame.actorIndex * 8) = g_BattleActorRecords[frame.actorIndex].tintG;
            *(char *)(&g_BattleBlueShadowFlareSavedActorAlpha.savedTintB + frame.actorIndex * 8) = g_BattleActorRecords[frame.actorIndex].tintB;
            g_BattleActorRecords[frame.actorIndex].drawFlags |= 1;
            g_BattleActorRecords[frame.actorIndex].tintPhase = 0;
            g_BattleActorRecords[frame.actorIndex].tintB = 0;
            g_BattleActorRecords[frame.actorIndex].tintG = 0;
            g_BattleActorRecords[frame.actorIndex].tintR = 0;
        }
    }
    if (frame.sfleaSlot->frameCounter < 8) {
        frame.tintPhase = frame.sfleaSlot->frameCounter << 9;
        for (frame.actorIndex = 0; frame.actorIndex < 0xA; frame.actorIndex++) {
            g_BattleActorRecords[frame.actorIndex].tintPhase = *(short *)&frame.tintPhase;
        }
    } else if (frame.sfleaSlot->frameCounter >= 0x55) {
        frame.tintPhase = 0x1000 - (frame.sfleaSlot->frameCounter - 0x55 << 9);
        if (frame.tintPhase < 0) {
            frame.tintPhase = 0;
        }
        for (frame.actorIndex = 0; frame.actorIndex < 0xA; frame.actorIndex++) {
            g_BattleActorRecords[frame.actorIndex].tintPhase = *(short *)&frame.tintPhase;
        }
    }
    if (frame.sfleaSlot->frameCounter < 8) {
        frame.shadeIntensity = frame.sfleaSlot->frameCounter << 9;
        if (frame.shadeIntensity > 0x1000) {
            frame.shadeIntensity = 0x1000;
        }
    } else if (frame.sfleaSlot->frameCounter >= 0x55) {
        frame.shadeIntensity = 0x1000 - (frame.sfleaSlot->frameCounter - 0x55 << 9);
        if (frame.shadeIntensity < 0) {
            frame.shadeIntensity = 0;
        }
    } else {
        frame.shadeIntensity = 0x1000;
    }
    if (frame.sfleaSlot->frameCounter == 8) {
        Battle_AllocateResourceSlot(BattleFx_BlueShadowFlareSetupTask);
    }
    if (frame.sfleaSlot->frameCounter >= 0x5D) {
        for (frame.actorIndex = 0; frame.actorIndex < 0xA; frame.actorIndex++) {
            g_BattleActorRecords[frame.actorIndex].drawFlags &= 0xFE;
            g_BattleActorRecords[frame.actorIndex].tintPhase = ((BattleBlueShadowFlareSavedActorCoordEntry *)(g_BattleBlueShadowFlareSavedActorCoord))[frame.actorIndex].tintPhase;
            g_BattleActorRecords[frame.actorIndex].tintPulse = *(char *)&((BattleBlueShadowFlareSavedActorCoordEntry *)(g_BattleBlueShadowFlareSavedActorCoord))[frame.actorIndex].savedTintPulse;
            g_BattleActorRecords[frame.actorIndex].tintR = *(char *)(((char *)&g_BattleBlueShadowFlareSavedActorAlpha) + frame.actorIndex * 8);
            g_BattleActorRecords[frame.actorIndex].tintG = *(char *)(&g_BattleBlueShadowFlareSavedActorAlpha.savedTintG + frame.actorIndex * 8);
            g_BattleActorRecords[frame.actorIndex].tintB = *(char *)(&g_BattleBlueShadowFlareSavedActorAlpha.savedTintB + frame.actorIndex * 8);
        }
        frame.shadeIntensity = 0;
        frame.sfleaSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    g_BattleFxTimelineState.startValue = frame.shadeIntensity;
    (frame.sfleaSlot->frameCounter)++;
}

/* Draws flat-shaded color strips for the Sflea effect. */
// FUNCTION: FF7 0x0057747e
void __cdecl BattleFx_BlueShadowFlareDrawPsxFlatColorStrips(int red, int green, int blue)
{
    int rgbWord, xLeft;
    BattleFxFlatColorStripState *renderState;
    int x1, i, stripCount, step;

    renderState = (BattleFxFlatColorStripState *)Gfx_GetPsxPacketScratchPtr();
    stripCount = 0x1E;
    xLeft = 0;
    x1 = 0x13F;
    step = 0x60000;
    rgbWord = red & 0xFF | (green & 0xFF) << 8 | (blue & 0xFF) << 0x10;
    for (i = 0; i < stripCount; i++) {
        renderState->xy0 = xLeft;
        renderState->xy1 = x1;
        xLeft += step;
        x1 += step;
        renderState->xLeft = xLeft;
        renderState->x1 = x1;
        renderState->color = rgbWord;
        renderState->tag = renderState->tag & 0xFFFFFF | 0x5000000;
        ((BattleFxShadowFlareStripRecord *)renderState)->polyCommand = 0x28;
        Gfx_SetBlendFactorPair(FLOAT_0_99999_BITS, FLOAT_ONE_BITS);
        *(short *)&renderState->xy0 <<= 1;
        ((BattleFxShadowFlareStripRecord *)renderState)->vertex2Y <<= 1;
        *(short *)&renderState->x1 <<= 1;
        ((BattleFxShadowFlareStripRecord *)renderState)->vertex1Y <<= 1;
        *(short *)&renderState->xLeft <<= 1;
        ((BattleFxShadowFlareStripRecord *)renderState)->vertex0Y <<= 1;
        *(short *)&renderState->xy1 <<= 1;
        ((BattleFxShadowFlareStripRecord *)renderState)->vertex3Y <<= 1;
        Gfx_DrawPsxPrimitive(renderState);
    }
    g_BattleFxActiveDrawListPtr = (int)renderState;
}

/* Sets up actor position and initial tasks for Sflea. */
// FUNCTION: FF7 0x005775ff
void __cdecl BattleFx_BlueShadowFlareSetupTask(void)
{
    int spawnSlot;
    BattleFxBlueShadowFlareState *task;

    task = (BattleFxBlueShadowFlareState *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    task = (BattleFxBlueShadowFlareState *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueShadowFlareTimelineTask);
    Battle_GetBoneWorldPos(g_BattleBlueShadowFlareTargetActor, g_BattleActorRecords[g_BattleBlueShadowFlareTargetActor].targetBone, (short *)((int)task + 4));
    task->offsetIndex = 0;
    task->range = g_BattleActorRecords[g_BattleBlueShadowFlareTargetActor].radius + 0x200;
    if (task->range > 0x76C) {
        task->range = 0x76C;
    }
    task->targetY = -task->range;
    spawnSlot = (int)BATTLE_NEW_TASK_SLOT(BattleFx_BlueShadowFlareDelayedSpawnTask);
    *(Int64Pair *)((char *)spawnSlot + 4) = *(Int64Pair *)((char *)task + 4);
    ((BattleFxShadowFlareDelayedSpawnSlot *)spawnSlot)->progress = task->range << 1;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition((char *)task + 4), 0x174);
}

/* Spawns delayed animated sprite tasks for Sflea. */
// FUNCTION: FF7 0x00577714
void __cdecl BattleFx_BlueShadowFlareDelayedSpawnTask(void)
{
    BattleShadowFlareDelayedSlot *pFadeQuad;
    BattleFxShadowFlareDelayedSpawnSlot *task;
    BattleShadowFlareDelayedSpawnRecord *spriteRecord;

    if (g_BattleIsPaused != 0) {
        return;
    }
    task = (BattleFxShadowFlareDelayedSpawnSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (task->timer == 0x2C) {
        pFadeQuad = (BattleShadowFlareDelayedSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueShadowFlareGrowFadeQuadTask);
        *(Int64Pair *)((char *)pFadeQuad + 4) = task->pos;
        pFadeQuad->posY = 0;
        pFadeQuad->scale = 0;
        pFadeQuad->progressFrac = (task->progress << 0xC) / 0x120 / 0x18;
        spriteRecord = (BattleShadowFlareDelayedSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueShadowFlareGrowFadeAnimatedSpriteTask);
        *(Int64Pair *)((char *)spriteRecord + 4) = *(Int64Pair *)((char *)pFadeQuad + 4);
        spriteRecord->posY = -(task->progress >> 1);
        spriteRecord->frameIndex = 0;
        spriteRecord->depth = task->progress >> 3;
        spriteRecord->angle = 0;
        spriteRecord->spinRate = (task->progress << 0xC) / 0x180 / 0x18;
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->timer)++;
}

/* Animates growing and fading billboard sprite for Sflea. */
// FUNCTION: FF7 0x0057784f
void __cdecl BattleFx_BlueShadowFlareGrowFadeAnimatedSpriteTask(void)
{
    unsigned char level;
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int s; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxShadowFlareSpriteRecord *taskSlot;
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int y; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int a; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int b; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int c; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int d; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int e; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int f; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    taskSlot = (BattleFxShadowFlareSpriteRecord *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix((void *)((int)taskSlot + 4), (short)taskSlot->angle, -taskSlot->depth);
    level = 0x80;
    if (taskSlot->frame >= 0x16) {
        level -= (char)(taskSlot->frame - 0x16 << 4);
    }
    g_BattleBlueShadowFlareSpriteReq1.sfleaGrowFadeByte10 = level;
    g_BattleBlueShadowFlareSpriteReq1.sfleaGrowFadeByte9 = g_BattleBlueShadowFlareSpriteReq1.sfleaGrowFadeByte10;
    g_BattleBlueShadowFlareSpriteReq1.sfleaGrowFadeByte8 = g_BattleBlueShadowFlareSpriteReq1.sfleaGrowFadeByte9;
    g_BattleBlueShadowFlareSpriteReq1.sfleaGrowFrameIndex = taskSlot->frameIndex;
    (g_BattleBlueShadowFlareSpriteReq1.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(2, g_BattleBlueShadowFlareLoadedResTable);
    g_BattleBlueShadowFlareSpriteReq1.sfleaGrowSpriteHandle = Gfx_ModelTableGetSpriteObject(2, g_BattleBlueShadowFlareLoadedResTable);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleBlueShadowFlareSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (taskSlot->frame >= 0x1E) {
        taskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    taskSlot->angle += taskSlot->spinRate;
    if (taskSlot->angle < 0) {
        taskSlot->angle = 0x7000;
    }
    if (taskSlot->frame > 4) {
        (taskSlot->frameIndex)++;
    }
    if (taskSlot->frameIndex > 0xF) {
        taskSlot->frameIndex = 0xF;
    }
    (taskSlot->frame)++;
}

/* Animates growing and fading quad sprite for Sflea. */
// FUNCTION: FF7 0x005779bd
void __cdecl BattleFx_BlueShadowFlareGrowFadeQuadTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[112];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; int tmpQuadOut; };
        struct { unsigned char pad_to_C[0xC]; BattleFxShadowFlareGrowFadeQuadSlot *quadSlot; };
        struct { unsigned char pad_to_10[0x10]; SVECTOR corner3X; };
        struct { unsigned char pad_to_18[0x18]; SVECTOR corner2X; };
        struct { unsigned char pad_to_20[0x20]; SVECTOR corner1X; };
        struct { unsigned char pad_to_28[0x28]; BattleFxShadowFlareFadeQuadRenderStateBlock *renderBlock; };
        struct { unsigned char pad_to_2C[0x2C]; SVECTOR corner0X; };
        struct { unsigned char pad_to_34[0x34]; PsxMatrix matrix; };
        struct { unsigned char pad_to_54[0x54]; int depth; };
        struct { unsigned char pad_to_58[0x58]; int tmpPrimOut; };
        struct { unsigned char pad_to_5C[0x5C]; VECTOR scaleX; };
        struct { unsigned char pad_to_6C[0x6C]; int fadeColor; };
    } frame;
#pragma pack(pop)

    frame.quadSlot = (BattleFxShadowFlareGrowFadeQuadSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0x400;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.matrix);
    frame.matrix.t[0] = frame.quadSlot->posX;
    frame.matrix.t[1] = frame.quadSlot->posY;
    frame.matrix.t[2] = frame.quadSlot->posZ;
    frame.scaleX.vz = frame.quadSlot->scale;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.matrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    frame.renderBlock = (BattleFxShadowFlareFadeQuadRenderStateBlock *)Gfx_GetPsxPacketScratchPtr();
    Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(0, g_BattleBlueShadowFlareLoadedResTable)));
    frame.corner0X.vx = 0xFEE0;
    frame.corner0X.vy = 0xFEE0;
    frame.corner0X.vz = 0;
    frame.corner1X.vx = 0x120;
    frame.corner1X.vy = 0xFEE0;
    frame.corner1X.vz = 0;
    frame.corner2X.vx = 0xFEE0;
    frame.corner2X.vy = 0x120;
    frame.corner2X.vz = 0;
    frame.corner3X.vx = 0x120;
    frame.corner3X.vy = 0x120;
    frame.corner3X.vz = 0;
    frame.fadeColor = 0x80;
    if (frame.quadSlot->frameCounter >= 0x16) {
        frame.fadeColor -= frame.quadSlot->frameCounter - 0x16 << 4;
    }
    frame.fadeColor = frame.fadeColor | frame.fadeColor << 8 | frame.fadeColor << 0x10;
    frame.depth = Gfx_TransformQuadReturnDepth(&frame.corner0X, &frame.corner1X, &frame.corner2X, &frame.corner3X, &frame.tmpPrimOut, &frame.tmpQuadOut);
    frame.renderBlock->tag = frame.renderBlock->tag & 0xFFFFFF | 0x9000000;
    *(char *)((char *)frame.renderBlock + 7) = 0x2C;
    frame.renderBlock->polyCommand = frame.fadeColor | 0x2E000000;
    frame.renderBlock->tpage = 0x4F;
    frame.renderBlock->clut = 0x7908;
    frame.renderBlock->uvWord0 = 0x8000;
    frame.renderBlock->uvWord1 = 0x8080;
    frame.renderBlock->uvWord2 = 0xFF00;
    frame.renderBlock->uvWord3 = 0xFF80;
    Gfx_DrawPsxPrimitive(frame.renderBlock);
    g_BattleFxActiveDrawListPtr = (int)frame.renderBlock;
    if (frame.quadSlot->frameCounter >= 0x1E) {
        frame.quadSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.quadSlot->scale += frame.quadSlot->scaleStep;
    if (frame.quadSlot->scale < 0) {
        frame.quadSlot->scale = 0x7000;
    }
    (frame.quadSlot->frameCounter)++;
}

/* Main timeline task controlling Sflea animation progression. */
// FUNCTION: FF7 0x00577c0e
void __cdecl BattleFx_BlueShadowFlareTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[80];
        PsxMatrix matrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxBlueShadowFlareRecord *sfleaRecord; };
        struct { unsigned char pad_to_24[0x24]; BattleFxPositionedSpriteSlot *spriteSlot; };
        struct { unsigned char pad_to_28[0x28]; int posX; };
        struct { unsigned char pad_to_2A[0x2A]; short posY; };
        struct { unsigned char pad_to_2C[0x2C]; int posZ; };
        struct { unsigned char pad_to_30[0x30]; int tmpRange; };
        struct { unsigned char pad_to_34[0x34]; int tmpSpread; };
        struct { unsigned char pad_to_38[0x38]; int spawnIndex; };
        struct { unsigned char pad_to_3C[0x3C]; BattleFxShadowFlareLivewireSeed *sparkSeed; };
        struct { unsigned char pad_to_40[0x40]; SVECTOR rotX; };
        struct { unsigned char pad_to_48[0x48]; int ringRecord; };
        struct { unsigned char pad_to_4C[0x4C]; short shadeIntensity; };
    } frame;
#pragma pack(pop)

    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.sfleaRecord = (BattleFxBlueShadowFlareRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.sfleaRecord->offsetIndex < 0x12) {
        frame.sparkSeed = (BattleFxShadowFlareLivewireSeed *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueShadowFlareMovingLivewireSpriteTask);
        frame.rotX.vx = ((short *)&g_BattleBlueShadowFlareOffsetTableX)[frame.sfleaRecord->offsetIndex];
        frame.rotX.vy = ((short *)&g_BattleBlueShadowFlareOffsetTableY)[frame.sfleaRecord->offsetIndex];
        frame.rotX.vz = 0;
        RotMatrixXYZ(&frame.rotX, &frame.matrix);
        *(short *)&frame.posX = 0x1450;
        frame.posY = 0;
        *(short *)&frame.posZ = 0;
        TransformVectorToInt16(&frame.matrix, &frame.posX, &frame.posX);
        *(short *)&frame.posX += frame.sfleaRecord->targetX;
        frame.posY += frame.sfleaRecord->targetY;
        *(short *)&frame.posZ += frame.sfleaRecord->targetZ;
        *(Int64Pair *)((char *)frame.sparkSeed + 4) = *(Int64Pair *)(&frame.posX);
        frame.sparkSeed->velocityX = (frame.sfleaRecord->targetX - *(short *)&frame.posX) / 8;
        frame.sparkSeed->velocityY = (frame.sfleaRecord->targetY - frame.posY) / 8;
        frame.sparkSeed->velocityZ = (frame.sfleaRecord->targetZ - *(short *)&frame.posZ) / 8;
        frame.sparkSeed->pushLength = -(frame.sfleaRecord->range >> 2);
        frame.sparkSeed->flag = 0;
        (frame.sfleaRecord->offsetIndex)++;
    }
    if (frame.sfleaRecord->frame > 0x18 && frame.sfleaRecord->frame < 0x25) {
        frame.tmpSpread = 0xA28;
        frame.tmpRange = frame.tmpSpread >> 1;
        for (frame.spawnIndex = 0; frame.spawnIndex < 2; frame.spawnIndex++) {
            frame.spriteSlot = (BattleFxPositionedSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueShadowFlareFrameTableSpriteTask);
            frame.spriteSlot->posX = frame.sfleaRecord->targetX + rand() % frame.tmpSpread - frame.tmpRange;
            frame.spriteSlot->posY = frame.sfleaRecord->targetY + rand() % frame.tmpSpread - frame.tmpRange;
            frame.spriteSlot->posZ = frame.sfleaRecord->targetZ + rand() % frame.tmpSpread - frame.tmpRange;
        }
    }
    if (frame.sfleaRecord->offsetIndex < 0x3E && frame.sfleaRecord->frame > 0x2D) {
        for (frame.spawnIndex = 0; frame.spawnIndex < 4; frame.spawnIndex++) {
            frame.tmpRange = frame.sfleaRecord->offsetIndex - 0x12;
            frame.sparkSeed = (BattleFxShadowFlareLivewireSeed *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueShadowFlareMovingLivewireSpriteTask);
            frame.rotX.vx = ((short *)&g_BattleBlueShadowFlareOffsetTableX)[frame.tmpRange];
            frame.rotX.vy = ((short *)&g_BattleBlueShadowFlareOffsetTableY)[frame.tmpRange];
            frame.rotX.vz = 0;
            RotMatrixXYZ(&frame.rotX, &frame.matrix);
            *(short *)&frame.posX = 0x1450;
            frame.posY = 0;
            *(short *)&frame.posZ = 0;
            TransformVectorToInt16(&frame.matrix, &frame.posX, &frame.posX);
            *(short *)&frame.posX += frame.sfleaRecord->targetX;
            frame.posY += frame.sfleaRecord->targetY;
            *(short *)&frame.posZ += frame.sfleaRecord->targetZ;
            *(Int64Pair *)((char *)frame.sparkSeed + 4) = *(Int64Pair *)((char *)frame.sfleaRecord + 4);
            frame.sparkSeed->velocityX = (*(short *)&frame.posX - frame.sfleaRecord->targetX) / 0x15;
            frame.sparkSeed->velocityY = (frame.posY - frame.sfleaRecord->targetY) / 0x15;
            frame.sparkSeed->velocityZ = (*(short *)&frame.posZ - frame.sfleaRecord->targetZ) / 0x15;
            frame.sparkSeed->pushLength = 0;
            frame.sparkSeed->scale = rand() % 0x1C00 + 0x400;
            frame.sparkSeed->flag = 1;
            (frame.sfleaRecord->offsetIndex)++;
        }
        for (frame.spawnIndex = 0; frame.spawnIndex < 4; frame.spawnIndex++) {
            frame.sparkSeed = (BattleFxShadowFlareLivewireSeed *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueShadowFlareMovingLivewireSpriteTask);
            frame.tmpSpread = rand() % 0x1000;
            frame.tmpRange = rand() % 0x1000;
            frame.rotX.vx = *(short *)&frame.tmpSpread;
            frame.rotX.vy = *(short *)&frame.tmpRange;
            frame.rotX.vz = 0;
            RotMatrixXYZ(&frame.rotX, &frame.matrix);
            *(short *)&frame.posX = 0x1450;
            frame.posY = 0;
            *(short *)&frame.posZ = 0;
            TransformVectorToInt16(&frame.matrix, &frame.posX, &frame.posX);
            *(short *)&frame.posX += frame.sfleaRecord->targetX;
            frame.posY += frame.sfleaRecord->targetY;
            *(short *)&frame.posZ += frame.sfleaRecord->targetZ;
            *(Int64Pair *)((char *)frame.sparkSeed + 4) = *(Int64Pair *)((char *)frame.sfleaRecord + 4);
            frame.sparkSeed->velocityX = (*(short *)&frame.posX - frame.sfleaRecord->targetX) / 0x15;
            frame.sparkSeed->velocityY = (frame.posY - frame.sfleaRecord->targetY) / 0x15;
            frame.sparkSeed->velocityZ = (*(short *)&frame.posZ - frame.sfleaRecord->targetZ) / 0x15;
            frame.sparkSeed->pushLength = 0;
            frame.sparkSeed->scale = rand() % 0x1C00 + 0x400;
            frame.sparkSeed->flag = 1;
        }
    }
    if (frame.sfleaRecord->frame == 0x18) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition((char *)frame.sfleaRecord + 4), 0x175);
    } else if (frame.sfleaRecord->frame == 0x2D) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition((char *)frame.sfleaRecord + 4), 0x176);
    }
    if (frame.sfleaRecord->frame < 0xA) {
        g_BattleBlueShadowFlareDrawColor.blue = 0xFF;
        g_BattleBlueShadowFlareDrawColor.green = g_BattleBlueShadowFlareDrawColor.blue;
        (g_BattleBlueShadowFlareDrawColor.colorR) = g_BattleBlueShadowFlareDrawColor.green;
        g_BattleBlueShadowFlareDrawColor.isActive = 1;
        frame.shadeIntensity = frame.sfleaRecord->frame * 0x199;
    } else if (frame.sfleaRecord->frame >= 0x47) {
        frame.shadeIntensity = 0x1000 - (frame.sfleaRecord->frame - 0x47) * 0x2AA;
        if (frame.shadeIntensity < 0) {
            frame.shadeIntensity = 0;
        }
    } else {
        frame.shadeIntensity = 0x1000;
    }
    if (frame.sfleaRecord->frame == 0x41) {
        if ((g_BattleHurtMessageActorMask & 1 << g_BattleBlueShadowFlareTargetActor) != 0) {
            frame.ringRecord = Battle_AllocSceneMessage(6);
        } else {
            frame.ringRecord = Battle_AllocSceneMessage(9);
        }
        *(short *)frame.ringRecord = *(short *)&g_BattleBlueShadowFlareTargetActor;
    }
    if (frame.sfleaRecord->frame >= 0x4D) {
        g_BattleBlueShadowFlareDrawColor.isActive = 0;
        frame.sfleaRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    g_BattleBlueShadowFlareShadeIntensity = frame.shadeIntensity;
    (frame.sfleaRecord->frame)++;
}

/* Renders animated frame-table sprite for Sflea. */
// FUNCTION: FF7 0x005782af
void __cdecl BattleFx_BlueShadowFlareFrameTableSpriteTask(void)
{
    BattleFxShadowFlareSpriteSlot *task;

    task = (BattleFxShadowFlareSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), 0x1800, 0);
    g_BattleBlueShadowFlareSpriteReq2.sfleaFrameTableIndex = g_BattleBlueShadowFlareFrameIndexTable[task->frameIndex];
    g_BattleBlueShadowFlareSpriteReq2.sfleaSpriteGroup = Gfx_ModelTableGetLoadedRecord(1, g_BattleBlueShadowFlareLoadedResTable);
    g_BattleBlueShadowFlareSpriteReq2.sfleaFrameTableHandle = Gfx_ModelTableGetSpriteObject(1, g_BattleBlueShadowFlareLoadedResTable);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleBlueShadowFlareSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->frameIndex >= 0xB) {
        task->status = 0xFFFF;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (task->frameIndex)++;
}

/* Animates moving spark sprite for Sflea. */
// FUNCTION: FF7 0x00578378
void __cdecl BattleFx_BlueShadowFlareMovingLivewireSpriteTask(void)
{
    short alpha, angle, stepLimit;
    BattleFxShadowFlareLivewire *sparkSlot;

    sparkSlot = (BattleFxShadowFlareLivewire *)BATTLE_CURRENT_TASK_SLOT;
    if (sparkSlot->flag != 0) {
        angle = sparkSlot->scale;
        g_BattleBlueShadowFlareSpriteReq3.sfleaSparkFrameWord = 7 - (sparkSlot->step >> 1);
        stepLimit = 0xE;
        alpha = 0x80;
        if (sparkSlot->step >= 8) {
            alpha -= (short)((sparkSlot->step - 8) * 0x15);
        }
        g_BattleBlueShadowFlareSpriteReq3.sfleaSparkByte10 = alpha;
        g_BattleBlueShadowFlareSpriteReq3.sfleaSparkByte9 = g_BattleBlueShadowFlareSpriteReq3.sfleaSparkByte10;
        g_BattleBlueShadowFlareSpriteReq3.sfleaSparkByte8 = g_BattleBlueShadowFlareSpriteReq3.sfleaSparkByte9;
    } else {
        angle = 0x2000 - (sparkSlot->step << 9);
        g_BattleBlueShadowFlareSpriteReq3.sfleaSparkFrameWord = sparkSlot->step;
        stepLimit = 7;
    }
    BattleFx_InstallBillboardMatrix((void *)((int)sparkSlot + 4), (short)angle, sparkSlot->pushLength);
    (g_BattleBlueShadowFlareSpriteReq3.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_BattleBlueShadowFlareLoadedResTable);
    g_BattleBlueShadowFlareSpriteReq3.sfleaSparkSpriteHandle = Gfx_ModelTableGetSpriteObject(1, g_BattleBlueShadowFlareLoadedResTable);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleBlueShadowFlareSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (sparkSlot->step >= stepLimit) {
        sparkSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    sparkSlot->posX += sparkSlot->velocityX;
    sparkSlot->posY += sparkSlot->velocityY;
    sparkSlot->posZ += sparkSlot->velocityZ;
    (sparkSlot->step)++;
}
} /* extern "C" */
