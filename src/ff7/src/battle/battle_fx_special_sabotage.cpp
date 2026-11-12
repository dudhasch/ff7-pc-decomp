// src/ff7/src/battle/battle_fx_special_sabotage.cpp
/* Enemy Special Attack [90]: Sabotage (orig. SPECIAL/HAKAI, 破壊工作) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Hakai attack: it loads resources, registers a per-target
//   callback that spawns a timeline task drawing an alternating screen flash, and runs a variant
//   sprite task for the current effect slot.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
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
// GLOBAL: FF7 0x008a3280
int g_BattleSpecSabotageResourceDefs = 0x00192A39;
int g_BattleSpecSabotageResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialSabotageResourceNames + 0x0)
};
// GLOBAL: FF7 0x008a3288
int g_BattleSpecSabotageResourceDefRows[10] = {
    (int)((char *)&g_BattleFxSpecialSabotageResourceNames + 0x10), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialSabotageResourceNames + 0x20), (int)((char *)&g_BattleFxSpecialSabotageResourceNames + 0x30),
    0x0, 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x008a32b0
BattleSpecSabotageSpriteReq g_BattleSpecSabotageSpriteReq = {{0x80, 0x80, 0x80, 0x2c}, 0, 0};
unsigned char g_BattleFxSpecialSabotageResourceNames[60] =
    "BIRIBIRI.TIM\0\0\0\0"
    "BIRIBIRI.SPT\0\0\0\0"
    "BIRIBIRI.TIM\0\0\0\0"
    "B_FLIP.SPT";
// clang-format on

/* Indices into g_BattleSpecSabotageResourceDefRows of the sprite record's frame table and variant field. */
enum { HAKAI_DEF_ROW_FRAME_TABLE = 8, HAKAI_DEF_ROW_VARIANT_FIELD = 9 };

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b65078
int g_BattleSpecSabotagePacketCursor = 0;
// GLOBAL: FF7 0x00b6507c
struct GfxTableCursor *g_BattleSpecSabotageLoadedResTable = 0;
// clang-format on

/*
 * When the engine context exists and g_BattleSpecSabotageLoadedResTable is empty, builds SPECIAL/HAKAI with a default-
 * duration record and creates resource type 2 using g_BattleSpecSabotageResourceDefs.
 */
// FUNCTION: FF7 0x00539770
void __cdecl BattleFx_SpecialLoadResSabotage(void)
{
    GfxModelTableBank *stage;
    GfxResourceRequest requestSlot;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceTable;

    if (g_pBattle3dScene != 0 && g_BattleSpecSabotageLoadedResTable == 0) {
        kind = 2;
        resourceTable = (int)&g_BattleSpecSabotageResourceDefs;
        stage = (GfxModelTableBank *)g_pBattle3dScene->modelTableBank;
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        ((BattleFxDiamondCardResource *)pRequest)->groupTag = 0;
        // STRING: FF7 0x008a32f4
        ((BattleFxDiamondCardResource *)pRequest)->resourcePath = (int)BattleFx_BuildResourcePath("SPECIAL/HAKAI", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceTable, pRequest, &g_BattleSpecSabotageLoadedResTable, stage);
    }
}

/*
 * Mechanical entry wrapper: calls BattleFx_SpecialLoadResSabotage with no arguments, then BattleFx_SpecialRegisterSabotagePerTargetCallback with both
 * arguments unchanged.
 */
// FUNCTION: FF7 0x00539800
void __cdecl BattleFx_SpecialEnterSabotage(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResSabotage();
    BattleFx_SpecialRegisterSabotagePerTargetCallback(targetMask, actorIdx);
}

/*
 * Creates a task for BattleFx_SpecialSpawnSabotagePerTarget with both arguments and mode 4, then dispatches event (0x186,
 * targetIndex, 0).
 */
// FUNCTION: FF7 0x0053981a
void __cdecl BattleFx_SpecialRegisterSabotagePerTargetCallback(int actorMask, int targetIndex)
{
    Battle_CreateBitPatternCallbackTask(actorMask, targetIndex, 4, BattleFx_SpecialSpawnSabotagePerTarget);
    Battle_PlaySfxPannedToActor(0x186, (short)targetIndex, 0);
}

/*
 * Allocates a slot for BattleFx_SpecialSabotageTimelineTask and stores the actor mask and target index in it.
 */
// FUNCTION: FF7 0x0053984a
void __cdecl BattleFx_SpecialSpawnSabotagePerTarget(int actor, int targetIndex)
{
    BattleFxSabotageTimelineSlot *taskSlot;

    taskSlot = (BattleFxSabotageTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSabotageTimelineTask);
    taskSlot->actor = actor;
    taskSlot->targetIndex = targetIndex;
}

/*
 * Draws a screen flash alternating third argument 32 and 64 before checking pause. When unpaused
 * below counter twenty-two emits BattleFx_SpecialSabotageVariantSpriteTask at a random actor bone with unguarded bone-count
 * modulo, XYZ jitter -200 through 199, scale 0x1800 through 0x27FF and random variant zero or one.
 * Advances and exactly at thirty allocates the actor typed record and retires.
 */
// FUNCTION: FF7 0x00539883
void __cdecl BattleFx_SpecialSabotageTimelineTask(void)
{
    BattleFxSabotageVariantSprite *sprite;
    BattleFxSabotageTimelineSlot *pTimeline;
    unsigned short flash;

    pTimeline = (BattleFxSabotageTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    flash = (pTimeline->timelineStep % 2 << 5) + 0x20;
    g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(0, 0, (char)flash);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (pTimeline->timelineStep < 0x16) {
        sprite = (BattleFxSabotageVariantSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSabotageVariantSpriteTask);
        Battle_GetBoneWorldPos(pTimeline->actor, rand() % g_BattleActorRecords[pTimeline->actor].boneCount, (short *)&sprite->posX);
        sprite->posX += (short)(rand() % 0x190 - 0xC8);
        sprite->posY += (short)(rand() % 0x190 - 0xC8);
        sprite->posZ += (short)(rand() % 0x190 - 0xC8);
        sprite->scale = rand() % 0x1000 + 0x1800;
        sprite->variant = rand() % 2;
    }
    (pTimeline->timelineStep)++;
    if (pTimeline->timelineStep == 0x1E) {
        Battle_PostActorHitOrHurtMessage(pTimeline->actor);
        pTimeline->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Per-frame handler for the variant sprite task: installs a billboard matrix at the task position and
 * scale, looks up the frame table and variant field for the task's variant (0 or 1) in
 * g_BattleSpecSabotageLoadedResTable, and submits the sprite record through BattleFx_SubmitSpriteDraw
 * into the depth-sorted ordering table, threading g_BattleSpecSabotagePacketCursor. When the frame table
 * resolved it advances the frame and frees the task once the frame reaches the animation length.
 */
// FUNCTION: FF7 0x00539a19
void __cdecl BattleFx_SpecialSabotageVariantSpriteTask(void)
{
    BattleFxSabotageVariantSprite *task;
    int entryIds[2];

    task = (BattleFxSabotageVariantSprite *)BATTLE_CURRENT_TASK_SLOT;
    entryIds[0] = 0;
    entryIds[1] = 1;
    BattleFx_InstallBillboardMatrix(&task->posX, task->scale, 0);
    g_BattleSpecSabotageResourceDefRows[HAKAI_DEF_ROW_FRAME_TABLE] = Gfx_ModelTableGetLoadedRecord(entryIds[task->variant], g_BattleSpecSabotageLoadedResTable);
    g_BattleSpecSabotageResourceDefRows[HAKAI_DEF_ROW_VARIANT_FIELD] = Gfx_ModelTableGetSpriteObject(entryIds[task->variant], g_BattleSpecSabotageLoadedResTable);
    g_BattleSpecSabotageSpriteReq.frame = task->frame;
    g_BattleSpecSabotagePacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecSabotageResourceDefRows[HAKAI_DEF_ROW_FRAME_TABLE], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecSabotagePacketCursor);
    if (g_BattleSpecSabotageResourceDefRows[HAKAI_DEF_ROW_FRAME_TABLE] != 0) {
        (task->frame)++;
        if (task->frame >= *(int *)((char *)*(int *)g_BattleSpecSabotageResourceDefRows[HAKAI_DEF_ROW_FRAME_TABLE] + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
