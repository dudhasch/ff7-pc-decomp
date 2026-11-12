// src/ff7/src/battle/battle_fx_special_siren_song_effect.cpp
/* Enemy Special Attack [116]: Siren Song (orig. SPECIAL/UTAGOE, 歌声) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Utagoe': a setup task with a timeline and a fading sprite.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x008a1070
int g_BattleSpecSirenSongResourceDefs = 0x0001AA39;
int g_BattleSpecSirenSongResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialSirenSongEffectResourceNames + 0x0)
};
// GLOBAL: FF7 0x008a1078
int g_BattleSpecSirenSongAnimData[6] = {
    (int)((char *)&g_BattleFxSpecialSirenSongEffectResourceNames + 0xc), 0x0, 0x0, 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x008a1090
unsigned char g_BattleSpecSirenSongColor[4] = {0x80, 0x80, 0x80, 0x2e};
unsigned char g_BattleFxSpecialSirenSongEffectResourceNames[24] =
    "ONPU1.TIM\0\0\0"
    "ONPU.SPT";
// GLOBAL: FF7 0x00b602b8
int g_BattleSpecSirenSongTargetMask = 0;
// GLOBAL: FF7 0x00b602bc
int g_BattleSpecSirenSongSourceActor = 0;
// GLOBAL: FF7 0x00b602c0
BattleSpecHeatBlastActorBonePos g_BattleSpecSirenSongActorBonePos = {0};
/* The bone's Z, then the pad word the sprites' eight-byte position copy carries along. */
// GLOBAL: FF7 0x00b602c4
short g_BattleSpecSirenSongActorBonePosZ[2] = {0};
// GLOBAL: FF7 0x00b602c8
struct GfxTableCursor *g_BattleSpecSirenSongLoadedResTable = 0;
unsigned char g_bssPad_00b602cc[4] = {0};
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/*
 * When the battle scene exists and g_BattleSpecSirenSongLoadedResTable is empty, loads the SPECIAL/UTAGOE
 * model table (resource kind 1, records g_BattleSpecSirenSongResourceDefs) with a default-duration request.
 */
// FUNCTION: FF7 0x00530d30
void __cdecl BattleFx_SpecialLoadResSirenSong(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest currentRecord;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecSirenSongLoadedResTable == 0) {
        kind = 1;
        resourceDefsWord = (int)&g_BattleSpecSirenSongResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a10b0
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/UTAGOE", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecSirenSongLoadedResTable, curOwnerCtx);
    }
}

/*
 * Stores the allocated BattleFx_SelectDrawListBuffer slot address in g_BattleFxDrawListBufferTaskSlotPtr, allocates a
 * slot for BattleFx_SpecialSirenSongSetupTask, stores both arguments in g_BattleSpecSirenSongTargetMask and g_BattleSpecSirenSongSourceActor, and dispatches
 * opcode 0x20 with entity-derived pan and sound 0x16F.
 */
// FUNCTION: FF7 0x00530dc0
void __cdecl BattleFx_SpecialBeginSirenSong(int targetMask, int actorIdx)
{
    g_BattleFxDrawListBufferTaskSlotPtr = (unsigned char *)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    Battle_AllocateResourceSlot(BattleFx_SpecialSirenSongSetupTask);
    g_BattleSpecSirenSongTargetMask = targetMask;
    g_BattleSpecSirenSongSourceActor = actorIdx;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x16F);
}

// Frees the current slot, then stores the world position of the source actor's targetBone in
// g_BattleSpecSirenSongActorBonePos. It allocates a BattleFx_SpecialSirenSongTimelineTask slot whose base is
// the centroid of g_BattleSpecSirenSongTargetMask with baseY replaced by -0x600, and jitterCenterXZ 0xA00.
// jitterRangeXZ is twice jitterCenterXZ, jitterOffsetY is the negated half of baseY, jitterRangeY is
// three times that offset, the offset is then doubled, and scale is 0xA3D.
// FUNCTION: FF7 0x00530e1d
void __cdecl BattleFx_SpecialSirenSongSetupTask(void)
{
    BattleFxSirenSongTimeline *task;

    task = (BattleFxSirenSongTimeline *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    Battle_GetBoneWorldPos(g_BattleSpecSirenSongSourceActor, g_BattleActorRecords[g_BattleSpecSirenSongSourceActor].targetBone,
                           &g_BattleSpecSirenSongActorBonePos.bonePosX);
    task = (BattleFxSirenSongTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSirenSongTimelineTask);
    Battle_GetActorMaskCentroid(g_BattleSpecSirenSongTargetMask, &task->baseX);
    task->baseY = -0x600;
    task->jitterCenterXZ = 0xA00;
    task->jitterRangeXZ = task->jitterCenterXZ * 2;
    task->jitterOffsetY = -(task->baseY >> 1);
    task->jitterRangeY = task->jitterOffsetY * 2 + task->jitterOffsetY;
    task->jitterOffsetY += task->jitterOffsetY;
    task->scale = 0xA3D;
}

/*
 * Spawns two fading sprites per frame between frames 2 and 0x28 and ramps the screen colour with the timer.
 */
// FUNCTION: FF7 0x00530f03
void __cdecl BattleFx_SpecialSirenSongTimelineTask(void)
{
    int diffY;
    int dz;
    int loopDeltaX;
    int destX;
    int destY;
    int destZ;
    int speedRoll;
    int loopIndex;
    BattleFxSirenSongTimeline *timelinePtr;
    BattleFxSirenSongFadeSprite *fadeSprite;
    int lengthSq;
    int padSlot; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short level;

    if (g_BattleIsPaused != 0) {
        return;
    }
    timelinePtr = (BattleFxSirenSongTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (timelinePtr->timer > 1 && timelinePtr->timer < 0x28) {
        for (loopIndex = 0; loopIndex < 2; loopIndex++) {
            fadeSprite = (BattleFxSirenSongFadeSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSirenSongFadingSpriteTask);
            *(Int64Pair *)&fadeSprite->posX = *(Int64Pair *)&g_BattleSpecSirenSongActorBonePos;
            fadeSprite->prevOffset = 0;
            destX = timelinePtr->baseX + rand() % timelinePtr->jitterRangeXZ - timelinePtr->jitterCenterXZ;
            destY = timelinePtr->baseY - rand() % timelinePtr->jitterRangeY + timelinePtr->jitterOffsetY;
            destZ = timelinePtr->baseZ + rand() % timelinePtr->jitterRangeXZ - timelinePtr->jitterCenterXZ;
            loopDeltaX = destX - g_BattleSpecSirenSongActorBonePos.bonePosX;
            diffY = destY - g_BattleSpecSirenSongActorBonePos.bonePosY;
            dz = destZ - g_BattleSpecSirenSongActorBonePosZ[0];
            fadeSprite->velocityX = loopDeltaX / 0x15;
            fadeSprite->velocityY = diffY / 0x15;
            fadeSprite->velocityZ = dz / 0x15;
            fadeSprite->rotation = 0;
            speedRoll = (rand() % 4096) + 0x800;
            fadeSprite->rotationStep = (timelinePtr->scale * speedRoll >> 0xC) / 0xD;
            fadeSprite->amplitude = SqrtToInt(lengthSq) >> 5;
            fadeSprite->angularVelocity = rand() % 0x60 + 0xDC;
            fadeSprite->angle = fadeSprite->angularVelocity;
        }
    }
    if (timelinePtr->timer < 8) {
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        level = timelinePtr->timer << 7;
    } else if (timelinePtr->timer >= 0x34) {
        level = 0x400 - (timelinePtr->timer - 0x34 << 7);
        if (level < 0) {
            level = 0;
        }
    } else {
        level = 0x400;
    }
    if (timelinePtr->timer == 0x35) {
        for (loopIndex = 0; loopIndex < 0xA; loopIndex++) {
            if ((g_BattleSpecSirenSongTargetMask & 1 << loopIndex) != 0) {
                Battle_PostActorHitOrHurtMessage(loopIndex);
            }
        }
    }
    if (timelinePtr->timer >= 0x3C) {
        level = 0;
        timelinePtr->ownerSlot = BATTLE_TASK_FREE;
    }
    g_BattleFxTimelineState.startValue = level;
    (timelinePtr->timer)++;
}

/*
 * Per-frame task of a fading Utagoe sprite: draws it, moves it with a wobble in Y, and frees it after 0x16 frames.
 */
// FUNCTION: FF7 0x005311eb
void __cdecl BattleFx_SpecialSirenSongFadingSpriteTask(void)
{
    unsigned char level;
    int frameWobble;
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int s; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxSirenSongFadeSprite *sprite;

    g_BattleSpecSirenSongAnimData[4] = Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecSirenSongLoadedResTable);
    g_BattleSpecSirenSongAnimData[5] = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecSirenSongLoadedResTable);
    sprite = (BattleFxSirenSongFadeSprite *)BATTLE_CURRENT_TASK_SLOT;
    if (sprite->step >= 0xE) {
        level = 0x80 - (sprite->step - 0xE << 4);
    } else {
        level = 0x80;
    }
    g_BattleSpecSirenSongColor[2] = level;
    g_BattleSpecSirenSongColor[1] = g_BattleSpecSirenSongColor[2];
    g_BattleSpecSirenSongColor[0] = g_BattleSpecSirenSongColor[1];
    BattleFx_InstallBillboardMatrix(&sprite->posX, (short)sprite->rotation, -(sprite->rotation >> 2));
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleSpecSirenSongAnimData[4], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (sprite->step >= 0x16) {
        sprite->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    sprite->posX += sprite->velocityX;
    sprite->posY += sprite->velocityY;
    sprite->posZ += sprite->velocityZ;
    frameWobble = (sprite->amplitude * SinFixed(sprite->angle) >> 0xC) - sprite->prevOffset;
    sprite->posY -= frameWobble;
    sprite->prevOffset = frameWobble;
    sprite->amplitude += (short)(sprite->amplitude >> 4);
    sprite->angle += sprite->angularVelocity;
    if (sprite->step < 0xD) {
        sprite->rotation += sprite->rotationStep;
    }
    (sprite->step)++;
}

/*
 * Entry point of the Utagoe special: forwards both arguments to
 * BattleFx_SpecialBeginSirenSong first, then calls BattleFx_SpecialLoadResSirenSong with none.
 */
// FUNCTION: FF7 0x005313d2
void __cdecl BattleFx_SpecialEnterSirenSong(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginSirenSong(targetMask, actorIdx);
    BattleFx_SpecialLoadResSirenSong();
}
} /* extern "C" */
