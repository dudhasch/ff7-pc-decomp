// src/ff7/src/battle/battle_fx_blue_l5_death_effect.cpp
// Enemy Skill [19]: L5 Death (Japanese: レベル5デス, dev tag: "blue/lv5deth" / Lv5Deth)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The Level 5 Death blue magic effect: a screen intensity ramp, plus per-target rotating
//   models and fading sprites.

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
// GLOBAL: FF7 0x008aa4e0
int g_BattleBlueL5DeathResourceDefs = 0x00192A39;
int g_BattleBlueL5DeathResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxBlueL5DeathEffectResourceNames + 0x0)
};
// GLOBAL: FF7 0x008aa4e8
int g_BattleBlueL5DeathResourceTable[10] = {
    (int)((char *)&g_BattleFxBlueL5DeathEffectResourceNames + 0xc), 0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleFxBlueL5DeathEffectResourceNames + 0x18), 0x0,
    0x0, 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x008aa510
struct BattleBlueL5DeathSpriteColor g_BattleBlueL5DeathSpriteColor = {-128, -128, -128, {0x2e}, 0, {0}};
unsigned char g_BattleFxBlueL5DeathEffectResourceNames[36] =
    "LV5DETH.TIM\0"
    "LV5DETH.SPT\0"
    "LIGHT_1.RSD";
// GLOBAL: FF7 0x00ba4a48
int g_BattleBlueL5DeathActiveCount = 0;
// GLOBAL: FF7 0x00ba4a4c
struct GfxTableCursor *g_BattleBlueL5DeathLoadedResTable = 0;
// clang-format on

/* Loads Level 5 Death blue magic resources. */
// FUNCTION: FF7 0x0056ecf0
void __cdecl BattleFx_BlueLoadResL5Death(void)
{
    GfxModelTableBank *ownerHandle;
    GfxResourceRequest reqBuffer;
    int kind;
    char nameWork[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleBlueL5DeathLoadedResTable == 0) {
        kind = 2;
        resourceDefsWord = (int)&g_BattleBlueL5DeathResourceDefs;
        ownerHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008aa53c
        pRequest->resourcePath = BattleFx_BuildResourcePath("blue/lv5deth", nameWork);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleBlueL5DeathLoadedResTable, ownerHandle);
    }
}

/* Entry point for Level 5 Death blue magic effect. */
// FUNCTION: FF7 0x0056ed80
void __cdecl BattleFx_BlueEnterL5Death(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResL5Death();
    BattleFx_BlueBeginL5Death(targetMask, actorIdx);
}

/* Initializes screen intensity task and per-target tasks for Level 5 Death. */
// FUNCTION: FF7 0x0056ed9a
void __cdecl BattleFx_BlueBeginL5Death(int targetMask, int actorIdx)
{
    int bitIdx;
    int targetCount;
    struct BattleTaskSlot *taskPointer;

    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    taskPointer = BATTLE_NEW_TASK_SLOT(BattleFx_BlueL5DeathScreenIntensityTask);
    ((BattleFxScreenIntensityTask *)taskPointer)->startStep = 0;
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 2, BattleFx_BlueSpawnL5DeathPerTarget);
    bitIdx = 0;
    for (targetCount = 0; bitIdx < 0xA; bitIdx++) {
        if ((targetMask & 1 << bitIdx) != 0) {
            targetCount++;
        }
    }
    g_BattleBlueL5DeathActiveCount = targetCount;
}

/* Ramps screen intensity up and down during Level 5 Death. */
// FUNCTION: FF7 0x0056ee42
void __cdecl BattleFx_BlueL5DeathScreenIntensityTask(void)
{
    unsigned short intensity;
    BattleFxScreenIntensityTask *task;

    task = (BattleFxScreenIntensityTask *)BATTLE_CURRENT_TASK_SLOT;
    if (task->step < 8) {
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        intensity = task->step * 320;
    } else if (g_BattleBlueL5DeathActiveCount <= 0) {
        if (task->startStep == 0) {
            task->startStep = task->step;
        }
        intensity = 0xA00 - (task->step - task->startStep) * 320;
    } else {
        intensity = 0xA00;
    }
    if (task->step >= 53) {
        intensity = 0;
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    g_BattleFxTimelineState.startValue = intensity;
    if (g_BattleIsPaused != 0) {
        return;
    }
    (task->step)++;
}

/* Spawns fading sprite and rotating model tasks per target. */
// FUNCTION: FF7 0x0056ef29
void __cdecl BattleFx_BlueSpawnL5DeathPerTarget(int actor)
{
    int scale, scaledVal;
    BattleFxL5DeathFadingSprite *fadingSprite;
    BattleFxDethRampSlot *rampRotationTask;

    fadingSprite = (BattleFxL5DeathFadingSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueL5DeathFadingSpriteTask);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)fadingSprite->pos);
    scale = 0x384;
    scaledVal = scale << 12;
    fadingSprite->scale = scaledVal / 500;
    fadingSprite->targetIndex = actor;
    rampRotationTask = (BattleFxDethRampSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueL5DeathRampRotationModelTask);
    *(Int64Pair *)rampRotationTask->pos = *(Int64Pair *)fadingSprite->pos;
    rampRotationTask->scale = scaledVal / 725;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(fadingSprite->pos), 0xAA);
}

/* Renders rotating model with ramped angles for Level 5 Death. */
// FUNCTION: FF7 0x0056effe
void __cdecl BattleFx_BlueL5DeathRampRotationModelTask(void)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxModelDraw *drawBlock;
    int s; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int t; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxDethRampSlot *slotPtr;
    int y; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int a; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int b; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int c; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int d; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int e; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    slotPtr = (BattleFxDethRampSlot *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix(slotPtr->pos, (short)slotPtr->scale, -(slotPtr->scale >> 3));
    drawBlock = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    drawBlock->modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleBlueL5DeathLoadedResTable);
    drawBlock->flags = 0x88;
    drawBlock->uvScroll = 0;
    drawBlock->fadeLevel = 2048;
    drawBlock->stateFlags = 0;
    drawBlock->texturePageBits = 0;
    if (slotPtr->timer < 8) {
        drawBlock->fadeLevel = 4096 - (slotPtr->timer << 8);
    } else if (slotPtr->timer >= 37) {
        drawBlock->fadeLevel += (short)(slotPtr->timer - 37 << 8);
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)drawBlock, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (slotPtr->timer >= 45) {
        slotPtr->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (slotPtr->timer)++;
}

/* Animates and fades sprite effect for Level 5 Death. */
// FUNCTION: FF7 0x0056f143
void __cdecl BattleFx_BlueL5DeathFadingSpriteTask(void)
{
    unsigned char colorByte;
    BattleFxFadingSprite *pSprite;

    pSprite = (BattleFxFadingSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleBlueL5DeathSpriteColor.lv5DethSpriteFrameWord = pSprite->frame & 7;
    if (pSprite->frame < 8) {
        colorByte = pSprite->frame << 4;
    } else if (pSprite->frame >= 37) {
        colorByte = 0x80 - (pSprite->frame - 0x25 << 4);
    } else {
        colorByte = 0x80;
    }
    g_BattleBlueL5DeathSpriteColor.lv5DethSpriteColorByte2 = colorByte;
    g_BattleBlueL5DeathSpriteColor.lv5DethSpriteColorByte1 = g_BattleBlueL5DeathSpriteColor.lv5DethSpriteColorByte2;
    g_BattleBlueL5DeathSpriteColor.lv5DethSpriteColorByte0 = g_BattleBlueL5DeathSpriteColor.lv5DethSpriteColorByte1;
    BattleFx_InstallBillboardMatrix(pSprite->pos, (short)pSprite->height, -(pSprite->height >> 2));
    g_BattleBlueL5DeathResourceTable[8] = Gfx_ModelTableGetLoadedRecord(0, g_BattleBlueL5DeathLoadedResTable);
    g_BattleBlueL5DeathResourceTable[9] = Gfx_ModelTableGetSpriteObject(0, g_BattleBlueL5DeathLoadedResTable);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleBlueL5DeathResourceTable[8], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (pSprite->frame >= 45) {
        g_BattleBlueL5DeathActiveCount--;
        pSprite->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (pSprite->frame == 35) {
        Battle_PostActorHitOrHurtMessage(pSprite->recordType);
    }
    (pSprite->frame)++;
}
} /* extern "C" */
