// src/ff7/src/battle/battle_fx_magic_toad.cpp
/* Magic Effect [10]: Toad (Japanese: トード, dev tag: "MAGIC/TODO" / Todo) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Death (Todo) magic effect: a timeline that spawns and animates floating skull particle
//   sprites on each target.

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

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

#define TOAD_SPRITE_REQ (&g_BattleMagicToadSpriteReq)


// clang-format off
// GLOBAL: FF7 0x008abeb0
int g_BattleMagicToadResourceDefs = 0x00192A39;
char *g_BattleFxMagicToadSkullsResourceNames[1] = {
    // STRING: FF7 0x008abed8
    "todo_1.tim"
};
unsigned int g_BattleFxMagicToadSkullsAssetPrefix[4] = {
    // STRING: FF7 0x008abee4
    (unsigned int)"todo_3.spt",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x00ba8278
int g_BattleMagicToadParticleCount = 0;
unsigned char g_bssPad_00ba827c[4] = {0};
// 444-entry (0xDE0-byte) Death skull billboard ring spanning 0x00ba8280..0x00ba9060; the named
// object covers the first 0x10 bytes, and later entries occupy g_bssPad_00ba8290.
// GLOBAL: FF7 0x00ba8280
struct BattleMagicToadBillboardPoints g_BattleMagicToadBillboardPoints = {0};
#define g_BattleMagicToadBillboardPointJitter (*(int *)((char *)&g_BattleMagicToadBillboardPoints + 0x4))
unsigned char g_bssPad_00ba8290[3536] = {0};
// GLOBAL: FF7 0x00ba9060
struct GfxTableCursor *g_BattleMagicToadLoadedResTable = 0;
unsigned char g_bssPad_00ba9064[4] = {0};
// GLOBAL: FF7 0x008abec8
struct BattleFxSpriteRequest g_BattleMagicToadSpriteReq = {0, 0, -128, -128, -128, 0x2e, 0, 0};
// clang-format on

/* Loads Death (Todo) magic resources. */
// FUNCTION: FF7 0x00574b00
void __cdecl BattleFx_MagicLoadResToad(void)
{
    GfxModelTableBank *sceneBase;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleMagicToadLoadedResTable == 0) {
        kind = 1;
        resourceDefsWord = (int)&g_BattleMagicToadResourceDefs;
        sceneBase = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008abef0
        pRequest->resourcePath = BattleFx_BuildResourcePath("MAGIC/TODO", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleMagicToadLoadedResTable, sceneBase);
    }
}

// Entry point for the Death (Todo) magic effect. It forwards targetMask and param unchanged to
// BattleFx_MagicBeginToad.
// FUNCTION: FF7 0x00574b90
void __cdecl BattleFx_MagicEnterToad(int targetMask, int actorIdx)
{
    BattleFx_MagicBeginToad(targetMask, actorIdx);
}

// Starts the Death (Todo) effect for targetMask. It allocates the effect's task slot into
// g_BattleFxDrawListBufferTaskSlotPtr, loads the Todo resources, and queues
// BattleFx_MagicSpawnToadPerTarget through a bit-pattern callback task over targetMask, passing
// param. It then resets g_BattleMagicToadParticleCount and plays sfx 0x134 with the pan computed
// from targetMask.
// FUNCTION: FF7 0x00574ba5
void __cdecl BattleFx_MagicBeginToad(int targetMask, int actorIdx)
{
    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    BattleFx_MagicLoadResToad();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 1, BattleFx_MagicSpawnToadPerTarget);
    g_BattleMagicToadParticleCount = 0;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x134);
}

// Spawns the timeline task for one target of Death (Todo). It places the task at the target's bone
// world position, stored in the slot, and sets depth to the actor radius plus 0xE0, capped at 0x4F2.
// The depth derives the rate fields: jitterRange is depth times two, riseRange is the negated posY times two,
// and scale is depth << 12 divided by 0x1C2. The target index is stored last.
// FUNCTION: FF7 0x00574c09
void __cdecl BattleFx_MagicSpawnToadPerTarget(int actor)
{
    BattleFxToadTimelineSlot *timelineSlot;
    int depth;

    timelineSlot = (BattleFxToadTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicToadTimelineTask);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)((char *)timelineSlot + 4));
    depth = g_BattleActorRecords[actor].radius + 0xE0;
    if (depth > 0x4F2) {
        depth = 0x4F2;
    }
    timelineSlot->depth = depth;
    timelineSlot->jitterRange = timelineSlot->depth << 1;
    timelineSlot->riseRange = -(timelineSlot->posY << 1);
    timelineSlot->scale = (depth << 0xC) / 0x1C2;
    timelineSlot->targetIndex = actor;
}

// Drives the Death (Todo) timeline and its skull particles, and returns at once while the game is
// paused. While frameCount is from 2 to 0x1B, each unpaused frame allocates a
// BattleFx_MagicToadAnimatedSpritesTask slot and writes four particles into the point ring. Each
// particle gets a jittered base position, a random offset bounded by riseRange, and a scale. The
// ring wraps at 0x1BC, and its index is saved in g_BattleMagicToadParticleCount. At frameCount 12
// it allocates a typed record for recordType, and from frameCount 40 it retires the slot. The
// unreferenced padding preserves the retail VC5 /Od stack frame size.
// FUNCTION: FF7 0x00574cc7
void __cdecl BattleFx_MagicToadTimelineTask(void)
{
    int idx;
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int ringIndex;
    int scaleJitter;
    BattleFxToadTimeline *task;
    struct BattleTaskSlot *pNewTask;

    if (g_BattleIsPaused != 0) {
        return;
    }
    task = (BattleFxToadTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (task->frameCount > 1 && task->frameCount < 0x1C) {
        ringIndex = g_BattleMagicToadParticleCount;
        pNewTask = BATTLE_NEW_TASK_SLOT(BattleFx_MagicToadAnimatedSpritesTask);
        ((BattleFxBerserkFadeSprite *)pNewTask)->ringIndex = ringIndex;
        for (idx = 0; idx < 4; idx++) {
            scaleJitter = (rand() % 4096) + 0x800;
            *(short *)((char *)((int *)&g_BattleMagicToadBillboardPoints) + ringIndex * 8) = task->baseX + rand() % task->jitterRange - task->jitterOffset;
            *(short *)((char *)&g_BattleMagicToadBillboardPoints.pointRise + ringIndex * 8) = -(rand() % task->riseRange);
            *(short *)((char *)&g_BattleMagicToadBillboardPointJitter + ringIndex * 8) = task->baseY + rand() % task->jitterRange - task->jitterOffset;
            *(short *)((char *)&g_BattleMagicToadBillboardPoints.scale + ringIndex * 8) = task->scale * scaleJitter >> 0xC;
            ringIndex++;
            if (ringIndex >= 0x1BC) {
                ringIndex = 0;
            }
        }
        g_BattleMagicToadParticleCount = ringIndex;
    }
    if (task->frameCount == 0xC) {
        Battle_PostActorHitOrHurtMessage(task->recordType);
    }
    if (task->frameCount >= 0x28) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCount)++;
}

// Draws the Death (Todo) skull particles from the point ring, four per call, starting at the index
// in the task's ringIndex field and wrapping at 0x1BC. Each particle takes its scale from the matching
// entry and is submitted with the sprite resource at field 0 of g_BattleMagicToadLoadedResTable.
// The slot retires once the frame reaches 11, even while the game is paused, and the frame advances
// only while the game is unpaused.
// FUNCTION: FF7 0x00574e71
void __cdecl BattleFx_MagicToadAnimatedSpritesTask(void)
{
    int idx, ringIndex;
    BattleFxBerserkFadeSprite *task;

    task = (BattleFxBerserkFadeSprite *)BATTLE_CURRENT_TASK_SLOT;
    TOAD_SPRITE_REQ->spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleMagicToadLoadedResTable);
    TOAD_SPRITE_REQ->drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleMagicToadLoadedResTable);
    TOAD_SPRITE_REQ->frame = task->frame;
    ringIndex = task->ringIndex;
    for (idx = 0; idx < 4; idx++) {
        BattleFx_InstallBillboardMatrix((void *)(ringIndex * 8 + (int)&g_BattleMagicToadBillboardPoints), (short)((BattleMagicToadSpriteEntry *)&g_BattleMagicToadBillboardPoints.scale)[ringIndex].scale, -(((BattleMagicToadSpriteEntry *)&g_BattleMagicToadBillboardPoints.scale)[ringIndex].scale >> 2));
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&TOAD_SPRITE_REQ->spriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
        ringIndex++;
        if (ringIndex >= 0x1BC) {
            ringIndex = 0;
        }
    }
    if (task->frame >= 0xB) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (task->frame)++;
}
} /* extern "C" */
