// src/ff7/src/battle/battle_fx_special_heat_blast_effect.cpp
/* Enemy Special Attack [102]: Heat Blast (orig. special/heatbu, ヒートブラスト) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Heatbu': a spawned timeline with an animated sprite task.

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
// GLOBAL: FF7 0x008a1178
int g_BattleSpecHeatBlastResourceDefs = 0x00192A39;
int g_BattleSpecHeatBlastResourceDefsTim[1] = {
    (int)((char *)&g_BattleSpecHeatBlastResourceNames + 0x0)
};
// GLOBAL: FF7 0x008a1180
int g_BattleSpecHeatBlastAnimData[6] = {
    (int)((char *)&g_BattleSpecHeatBlastResourceNames + 0xc), 0x0, 0x0, 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x008a1198
struct BattleSpecHeatBlastColor g_BattleSpecHeatBlastColor = {-128, -128, -128, {0x2e}};
unsigned char g_BattleSpecHeatBlastResourceNames[24] =
    "HEAT.TIM\0\0\0\0"
    "MAGIKAL.SPT";
// GLOBAL: FF7 0x00b61ad0
int g_BattleSpecHeatBlastTargetMask = 0;
// GLOBAL: FF7 0x00b61ad4
int g_BattleSpecHeatBlastSourceActor = 0;
// GLOBAL: FF7 0x00b61ad8
int g_BattleSpecHeatBlastActorBonePos = 0;
// GLOBAL: FF7 0x00b61adc
int g_BattleSpecHeatBlastActorBonePosZ = 0;
// GLOBAL: FF7 0x00b61ae0
struct GfxTableCursor *g_BattleSpecHeatBlastLoadedResTable = 0;
unsigned char g_bssPad_00b61ae4[4] = {0};
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// clang-format on


/*
 * When the engine context exists and g_BattleSpecHeatBlastLoadedResTable is empty, builds special/heatbu with a default-
 * duration record and creates resource type 1 using g_BattleSpecHeatBlastResourceDefs.
 */
// FUNCTION: FF7 0x005324a0
void __cdecl BattleFx_SpecialLoadResHeatBlast(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest currentRecord;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecHeatBlastLoadedResTable == 0) {
        kind = 1;
        resourceDefsWord = (int)&g_BattleSpecHeatBlastResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a11b8
        pRequest->resourcePath = (char *)BattleFx_BuildResourcePath("special/heatbu", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecHeatBlastLoadedResTable, curOwnerCtx);
    }
}

/*
 * Loads the Heatbu resources, then begins the effect on the target and source.
 */
// FUNCTION: FF7 0x00532530
void __cdecl BattleFx_SpecialEnterHeatBlast(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResHeatBlast();
    BattleFx_SpecialBeginHeatBlast(targetMask, actorIdx);
}

/*
 * Stores the allocated BattleFx_SelectDrawListBuffer slot address in g_BattleFxDrawListBufferTaskSlotPtr, allocates a
 * slot for BattleFx_SpecialHeatBlastSpawnTimeline, stores both arguments in g_BattleSpecHeatBlastTargetMask and g_BattleSpecHeatBlastSourceActor, and dispatches
 * opcode 0x20 with entity-derived pan and sound 0x165.
 */
// FUNCTION: FF7 0x0053254a
void __cdecl BattleFx_SpecialBeginHeatBlast(int targetMask, int actorIdx)
{
    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    Battle_AllocateResourceSlot(BattleFx_SpecialHeatBlastSpawnTimeline);
    g_BattleSpecHeatBlastTargetMask = targetMask;
    g_BattleSpecHeatBlastSourceActor = actorIdx;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x165);
}

// Retires the current effect slot with 0xFFFF, then stores the world position of the source actor's
// bone33 in g_BattleSpecHeatBlastActorBonePos. It then allocates a BattleFx_SpecialHeatBlastTimelineTask
// slot whose base position is the centroid of g_BattleSpecHeatBlastTargetMask, with baseY set to
// 0xFA00 (-0x600). jitterCenterXZ is 0x800 and jitterRangeXZ twice that. jitterOffsetY is the
// negated half of baseY, jitterRangeY three times that, and jitterOffsetY is then doubled. scale
// is 0xA3D.
// FUNCTION: FF7 0x005325a7
void __cdecl BattleFx_SpecialHeatBlastSpawnTimeline(void)
{
    BattleFxSirenSongTimeline *task;

    task = (BattleFxSirenSongTimeline *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    Battle_GetBoneWorldPos(g_BattleSpecHeatBlastSourceActor, g_BattleActorRecords[g_BattleSpecHeatBlastSourceActor].bone33, (short *)&g_BattleSpecHeatBlastActorBonePos);
    task = (BattleFxSirenSongTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHeatBlastTimelineTask);
    Battle_GetActorMaskCentroid(g_BattleSpecHeatBlastTargetMask, &task->baseX);
    task->baseY = 0xFA00;
    task->jitterCenterXZ = 0x800;
    task->jitterRangeXZ = task->jitterCenterXZ << 1;
    task->jitterOffsetY = -(task->baseY >> 1);
    task->jitterRangeY = task->jitterOffsetY * 2 + task->jitterOffsetY;
    task->jitterOffsetY += task->jitterOffsetY;
    task->scale = 0xA3D;
}

/*
 * Spawns the Heatbu sprites during the first frames from the bone position towards a jittered
 * point around the target centroid, ramps the screen tint, and frees itself at timer 0x37.
 */
// FUNCTION: FF7 0x0053268d
void __cdecl BattleFx_SpecialHeatBlastTimelineTask(void)
{
    short level;
    int destX;
    int destY;
    int destZ;
    int i;
    int scaleRoll;
    BattleFxSirenSongTimeline *selfPtr;
    BattleFxHeatBlastSpriteRecord *pSpawned;

    if (g_BattleIsPaused != 0) {
        return;
    }
    selfPtr = (BattleFxSirenSongTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (selfPtr->timer > 1 && selfPtr->timer < 0x21) {
        for (i = 0; i < 1; i++) {
            pSpawned = (BattleFxHeatBlastSpriteRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHeatBlastAnimatedSpriteTask);
            pSpawned->pos.pair = *(Int64Pair *)(&g_BattleSpecHeatBlastActorBonePos);
            destX = selfPtr->baseX + rand() % selfPtr->jitterRangeXZ - selfPtr->jitterCenterXZ;
            destY = selfPtr->baseY - rand() % selfPtr->jitterRangeY + selfPtr->jitterOffsetY;
            destZ = selfPtr->baseZ + rand() % selfPtr->jitterRangeXZ - selfPtr->jitterCenterXZ;
            pSpawned->velX = (destX - ((BattleSpecHeatBlastActorBonePos *)&g_BattleSpecHeatBlastActorBonePos)->bonePosX) / 0x11;
            pSpawned->velY = (destY - ((BattleSpecHeatBlastActorBonePos *)&g_BattleSpecHeatBlastActorBonePos)->bonePosY) / 9;
            pSpawned->velZ = (destZ - *(short *)&g_BattleSpecHeatBlastActorBonePosZ) / 0x11;
            if (pSpawned->velY > 0) {
                pSpawned->velY = -pSpawned->velY;
            }
            pSpawned->angle = 0;
            scaleRoll = rand() % 0x1000 + 0x800;
            pSpawned->angleStep = (selfPtr->scale * scaleRoll >> 0xC) / 0xB;
        }
    }
    if (selfPtr->timer < 8) {
        g_BattleFxTimelineState.red = 0x20;
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        level = selfPtr->timer * 0x140;
    } else if (selfPtr->timer >= 0x2F) {
        level = 0xA00 - (selfPtr->timer - 0x2F) * 0x140;
        if (level < 0) {
            level = 0;
        }
    } else {
        level = 0xA00;
    }
    if (selfPtr->timer == 0x2D) {
        for (i = 0; i < 0xA; i++) {
            if ((g_BattleSpecHeatBlastTargetMask & 1 << i) != 0) {
                Battle_PostActorHitOrHurtMessage(i);
            }
        }
    }
    if (selfPtr->timer >= 0x37) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(g_BattleSpecHeatBlastTargetMask), 0);
        level = 0;
        g_BattleFxTimelineState.red = level;
        selfPtr->ownerSlot = BATTLE_TASK_FREE;
    }
    g_BattleFxTimelineState.startValue = level;
    (selfPtr->timer)++;
}

// Per-frame tick for the current task slot. It draws animation entry 0 of
// g_BattleSpecHeatBlastLoadedResTable through g_BattleSpecHeatBlastAnimData into the chain
// g_BattleFxActiveDrawListPtr, billboarding the position with the angle and -(angle >> 2).
// The colour bytes are 0x80 until timer reaches 0xA, then the unsigned char 0x80 - ((timer - 0xA)
// << 4). The slot is retired with 0xFFFF in ownerSlot from timer 0x12, before the pause check.
// Unless g_BattleIsPaused is set, timers under 0x11 move pos.x and pos.z by velX and velZ, timers
// from 7 move pos.y by velY and halve velY from 0x10, timers under 0xB add angleStep to the angle,
// and then timer advances.
// FUNCTION: FF7 0x00532963
void __cdecl BattleFx_SpecialHeatBlastAnimatedSpriteTask(void)
{
    unsigned char color;
    BattleFxHeatBlastSpriteRecord *task;

    task = (BattleFxHeatBlastSpriteRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (task->timer >= 0xA) {
        color = 0x80 - (task->timer - 0xA << 4);
    } else {
        color = 0x80;
    }
    g_BattleSpecHeatBlastColor.blue = color;
    g_BattleSpecHeatBlastColor.green = g_BattleSpecHeatBlastColor.blue;
    (g_BattleSpecHeatBlastColor.red) = g_BattleSpecHeatBlastColor.green;
    BattleFx_InstallBillboardMatrix(&task->pos.xyz.x, (short)task->angle, -(task->angle >> 2));
    g_BattleSpecHeatBlastAnimData[4] = Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecHeatBlastLoadedResTable);
    g_BattleSpecHeatBlastAnimData[5] = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecHeatBlastLoadedResTable);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleSpecHeatBlastAnimData[4], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->timer >= 0x12) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->timer < 0x11) {
        task->pos.xyz.x += task->velX;
        task->pos.xyz.z += task->velZ;
    }
    if (task->timer >= 7) {
        task->pos.xyz.y += task->velY;
        if (task->timer >= 0x10) {
            task->velY >>= 1;
        }
    }
    if (task->timer < 0xB) {
        task->angle += task->angleStep;
    }
    (task->timer)++;
}
} /* extern "C" */
