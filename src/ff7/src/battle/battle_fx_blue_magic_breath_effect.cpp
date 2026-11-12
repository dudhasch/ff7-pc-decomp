// src/ff7/src/battle/battle_fx_blue_magic_breath_effect.cpp
// Enemy Skill [15]: Magic Breath (Japanese: マジカルブレス Magical Breath, dev tag: "blue/magikal" / Magikal)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The Magic Breath blue magic effect: a breath projectile sprite that fades as it travels
//   from the source bone to the target, with breath sounds on the timeline.

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
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
#define g_BattleMagicBreathResourceHandle (*(int *)((char *)&g_BattleBlueMagicBreathResourceTable + 16))
#define g_BattleMagicBreathResourceAltHandle (*(int *)((char *)&g_BattleBlueMagicBreathResourceTable + 20))
// clang-format on


// clang-format off
// GLOBAL: FF7 0x008abe30
int g_BattleBlueMagicBreathResourceDefs = 0x00192A39;
int g_BattleBlueMagicBreathResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxBlueMagicBreathEffectResourceNames + 0x0)
};
// GLOBAL: FF7 0x008abe38
int g_BattleBlueMagicBreathResourceTable[6] = {
    (int)((char *)&g_BattleFxBlueMagicBreathEffectResourceNames + 0xc), 0x0, 0x0, 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x008abe50
struct BattleBlueMagicBreathSpriteColor g_BattleBlueMagicBreathSpriteColor = {-128, -128, -128, {0x2e}};
unsigned char g_BattleFxBlueMagicBreathEffectResourceNames[24] =
    "MAGIKAL.TIM\0"
    "MAGIKAL.SPT";
// GLOBAL: FF7 0x00ba8248
int g_BattleBlueMagicBreathTargetMask = 0;
// GLOBAL: FF7 0x00ba824c
int g_BattleBlueMagicBreathCasterIndex = 0;
// GLOBAL: FF7 0x00ba8250
int g_BattleBlueMagicBreathSourcePos = 0;
// GLOBAL: FF7 0x00ba8254
int g_BattleBlueMagicBreathSourcePosZ = 0;
// GLOBAL: FF7 0x00ba8258
struct GfxTableCursor *g_BattleBlueMagicBreathLoadedResTable = 0;
unsigned char g_bssPad_00ba825c[4] = {0};
// clang-format on

/* Loads Magic Breath blue magic resources. */
// FUNCTION: FF7 0x00573d80
void __cdecl BattleFx_BlueLoadResMagicBreath(void)
{
    GfxModelTableBank *sceneOwnVal;
    GfxResourceRequest reqLinkRec;
    int kind;
    char resPathBuf[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleBlueMagicBreathLoadedResTable == 0) {
        kind = 1;
        resourceDefsWord = (int)&g_BattleBlueMagicBreathResourceDefs;
        sceneOwnVal = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqLinkRec;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008abe70
        pRequest->resourcePath = (char *)BattleFx_BuildResourcePath("blue/magikal", resPathBuf);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleBlueMagicBreathLoadedResTable, sceneOwnVal);
    }
}

// Entry point for the Magic Breath blue magic effect. It loads the effect's resources and then
// starts the effect for the target mask and caster.
// FUNCTION: FF7 0x00573e10
void __cdecl BattleFx_BlueEnterMagicBreath(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResMagicBreath();
    BattleFx_BlueBeginMagicBreath(targetMask, actorIdx);
}

/* Initializes setup task for Magic Breath. */
// FUNCTION: FF7 0x00573e2a
void __cdecl BattleFx_BlueBeginMagicBreath(int targetMask, int actorIdx)
{
    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    Battle_AllocateResourceSlot(BattleFx_BlueMagicBreathSetupTask);
    g_BattleBlueMagicBreathTargetMask = targetMask;
    g_BattleBlueMagicBreathCasterIndex = actorIdx;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x9C);
}

// Sets up the Magic Breath source and spawns its timeline task. The caster's secondary bone world
// position is stored as the breath source, and the setup slot's owner is cleared. The new timeline
// task receives the centroid of the target mask, and its initial velocity, scale, and derived
// velocity values are set from fixed constants, with spriteScale set to a constant too.
// FUNCTION: FF7 0x00573e87
void __cdecl BattleFx_BlueMagicBreathSetupTask(void)
{
    BattleFxBlueMagikRecord *task;

    task = (BattleFxBlueMagikRecord *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    Battle_GetBoneWorldPos(g_BattleBlueMagicBreathCasterIndex, g_BattleActorRecords[g_BattleBlueMagicBreathCasterIndex].bone33, (short *)&g_BattleBlueMagicBreathSourcePos);
    task = (BattleFxBlueMagikRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueMagicBreathTimelineTask);
    Battle_GetActorMaskCentroid(g_BattleBlueMagicBreathTargetMask, (short *)((int)task + 4));
    task->initialVelocity = -0x600;
    task->scale = 0x800;
    task->scaleTimes2 = task->scale << 1;
    task->velocity = -(task->initialVelocity >> 1);
    task->velocityTimes3 = task->velocity * 2 + task->velocity;
    task->velocity += task->velocity;
    task->spriteScale = 0xA3D;
}

// Drives the Magic Breath timeline, the breath sound opcodes, and the projectile sprite emission.
// It does nothing while the game is paused. From phase 2 to phase 0x20, each call spawns one fading
// sprite task at the source, with its target jittered around the target position and a random
// scale. Phases below 8 set the channel fields for the start, phases from 0x2F fade the level down
// to zero, and in between the level holds at 0x800. At phase 0x2D it allocates a record for each
// target bit. From phase 0x37 it clears the level and frees its slot. The phase advances on each
// call that is not paused.
// FUNCTION: FF7 0x00573f6d
void __cdecl BattleFx_BlueMagicBreathTimelineTask(void)
{
    short level;
    int destX;
    int destY;
    int destZ;
    int loopIdx;
    BattleFxBlueMagicBreathTimeline *selfPtr;
    BattleFxBlueMagicBreathTimeline *child;
    int rnd;

    if (g_BattleIsPaused != 0) {
        return;
    }
    selfPtr = (BattleFxBlueMagicBreathTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (selfPtr->phase > 1 && selfPtr->phase < 0x21) {
        for (loopIdx = 0; loopIdx < 1; loopIdx++) {
            child = (BattleFxBlueMagicBreathTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueMagicBreathFadingSpriteTask);
            *(Int64Pair *)&child->posX = *(Int64Pair *)(&g_BattleBlueMagicBreathSourcePos);
            destX = selfPtr->posX + rand() % selfPtr->jitterRange - selfPtr->jitterBias;
            destY = selfPtr->posY - rand() % selfPtr->yJitterRange + selfPtr->yBias;
            destZ = selfPtr->posZ + rand() % selfPtr->jitterRange - selfPtr->jitterBias;
            child->jitterBias = (destX - ((BattleSpecTailLaserSourceBonePos *)&g_BattleBlueMagicBreathSourcePos)->sourceX) / 0x11;
            child->jitterRange = (destY - ((BattleSpecTailLaserSourceBonePos *)&g_BattleBlueMagicBreathSourcePos)->sourceY) / 9;
            child->yBias = (destZ - *(short *)&g_BattleBlueMagicBreathSourcePosZ) / 0x11;
            if (child->jitterRange > 0) {
                child->jitterRange = -child->jitterRange;
            }
            child->scale = 0;
            rnd = rand() % 0x1000 + 0x800;
            child->scaleStep = (selfPtr->scale * rnd >> 0xC) / 0xB;
        }
    }
    if (selfPtr->phase < 8) {
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        level = selfPtr->phase << 8;
    } else if (selfPtr->phase >= 0x2F) {
        level = 0x800 - (selfPtr->phase - 0x2F << 8);
        if (level < 0) {
            level = 0;
        }
    } else {
        level = 0x800;
    }
    if (selfPtr->phase == 0x2D) {
        for (loopIdx = 0; loopIdx < 0xA; loopIdx++) {
            if ((g_BattleBlueMagicBreathTargetMask & 1 << loopIdx) != 0) {
                Battle_PostActorHitOrHurtMessage(loopIdx);
            }
        }
    }
    if (selfPtr->phase >= 0x37) {
        level = 0;
        selfPtr->ownerSlot = BATTLE_TASK_FREE;
    }
    g_BattleFxTimelineState.startValue = level;
    (selfPtr->phase)++;
}

// Animates one breath projectile sprite and fades it toward the target. Its color byte fades out
// after elapsed frame 0xA, and the sprite is submitted with a billboard matrix built from its
// angle. Its slot is freed at elapsed 0x12. While the game is not paused, x and z move by their
// velocities until elapsed 0x11, y moves from elapsed 7 with its velocity halved from 0x10, the
// angle steps until elapsed 0xB, and elapsed advances each frame.
// FUNCTION: FF7 0x0057421b
void __cdecl BattleFx_BlueMagicBreathFadingSpriteTask(void)
{
    unsigned char colorByte;
    BattleFxBlueMagicBreathSprite *pSprite;

    pSprite = (BattleFxBlueMagicBreathSprite *)BATTLE_CURRENT_TASK_SLOT;
    if (pSprite->elapsed >= 0xA) {
        colorByte = 0x80 - (pSprite->elapsed - 0xA << 4);
    } else {
        colorByte = 0x80;
    }
    g_BattleBlueMagicBreathSpriteColor.magikalSpriteColorByte2 = colorByte;
    g_BattleBlueMagicBreathSpriteColor.magikalSpriteColorByte1 = g_BattleBlueMagicBreathSpriteColor.magikalSpriteColorByte2;
    g_BattleBlueMagicBreathSpriteColor.magikalSpriteColorByte0 = g_BattleBlueMagicBreathSpriteColor.magikalSpriteColorByte1;
    BattleFx_InstallBillboardMatrix((void *)((int)pSprite + 4), (short)pSprite->angle, -(pSprite->angle >> 2));
    g_BattleMagicBreathResourceHandle = Gfx_ModelTableGetLoadedRecord(0, g_BattleBlueMagicBreathLoadedResTable);
    g_BattleMagicBreathResourceAltHandle = Gfx_ModelTableGetSpriteObject(0, g_BattleBlueMagicBreathLoadedResTable);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleMagicBreathResourceHandle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (pSprite->elapsed >= 0x12) {
        pSprite->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (pSprite->elapsed < 0x11) {
        pSprite->posX += pSprite->velocityX;
        pSprite->posZ += pSprite->velocityZ;
    }
    if (pSprite->elapsed >= 7) {
        pSprite->posY += pSprite->velocityY;
        if (pSprite->elapsed >= 0x10) {
            pSprite->velocityY >>= 1;
        }
    }
    if (pSprite->elapsed < 0xB) {
        pSprite->angle += pSprite->angleStep;
    }
    (pSprite->elapsed)++;
}
} /* extern "C" */
