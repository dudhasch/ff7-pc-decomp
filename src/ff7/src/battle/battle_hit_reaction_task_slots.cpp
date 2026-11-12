// src/ff7/src/battle/battle_hit_reaction_task_slots.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle hit-reaction and script-timer tasks.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
extern "C" {

#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

/*
 * Spawns a hit reaction stagger task.
 */
// FUNCTION: FF7 0x005be9f0
void __cdecl Battle_SpawnHitReactionTask(int duration, int targetId, int strength)
{
    BattleHitReactionEntry *entry;

    entry = (BattleHitReactionEntry *)BATTLE_NEW_TASK_SLOT(Battle_HitReactionStaggerTask);
    entry->ownerSlot = 0;
    entry->duration = duration;
    entry->strength = strength;
    entry->targetId = targetId;
}

/*
 * Drives periodic hit reaction animation and ring buffer records.
 */
// FUNCTION: FF7 0x005bea3c
void __cdecl Battle_HitReactionStaggerTask(void)
{
    BattleHitStaggerRecord *staggerPtr;
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int record;

    staggerPtr = (BattleHitStaggerRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (staggerPtr->frame >= staggerPtr->startFrame &&
        (staggerPtr->frame - staggerPtr->startFrame) % staggerPtr->interval == 0) {
        while ((staggerPtr->pendingMask & 1 << staggerPtr->bitIndex) == 0) {
            (staggerPtr->bitIndex)++;
        }
        record = Battle_AllocSceneMessage(4);
        *(short *)record = staggerPtr->bitIndex;
        staggerPtr->pendingMask &= ~(1 << staggerPtr->bitIndex);
        if (staggerPtr->pendingMask == 0) {
            staggerPtr->bitIndex = 0xFFFF;
        }
    }
    (staggerPtr->frame)++;
}

/*
 * Spawns a battle script timer task.
 */
// FUNCTION: FF7 0x005beb27
void __cdecl Battle_SpawnScriptTimerTask(int timerId, int timer)
{
    BattleScriptTimerTask *entry;

    entry = (BattleScriptTimerTask *)BATTLE_NEW_TASK_SLOT(Battle_ScriptTimerTask);
    entry->ownerSlot = 0;
    entry->timer = timer;
    entry->timerId = timerId;
}

/*
 * Processes script timing delay and ring buffer events.
 */
// FUNCTION: FF7 0x005beb68
void __cdecl Battle_ScriptTimerTask(void)
{
    BattleScriptTimer *pTimer;
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    Battle3dSceneMessage *message;
    int actor;

    pTimer = (BattleScriptTimer *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (pTimer->timer >= pTimer->startTime &&
        (pTimer->timer - pTimer->startTime) % pTimer->interval == 0) {
        actor = g_BattleHitTargetActors[pTimer->sequenceIndex];
        if (actor != 0xFF) {
            message = (Battle3dSceneMessage *)Battle_AllocSceneMessage(0xA);
            message->actor = actor;
            message->effectOrder = pTimer->sequenceIndex;
            (pTimer->sequenceIndex)++;
            if (g_BattleHitTargetActors[pTimer->sequenceIndex] == 0xFF) {
                pTimer->sequenceIndex = 0xFFFF;
            }
        } else {
            pTimer->sequenceIndex = 0xFFFF;
        }
    }
    (pTimer->timer)++;
}

} /* extern "C" */
