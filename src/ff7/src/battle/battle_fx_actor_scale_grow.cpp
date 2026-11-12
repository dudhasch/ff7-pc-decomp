// src/ff7/src/battle/battle_fx_actor_scale_grow.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Effect entry that grows an actor's scale through a hold and then a ramp, plus a helper
//   task that holds a timing slot for a set number of frames.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/battle.h"
#include "ff7/battle_tasks.h"
#include "ff7/gte.h"
#include "lib/crt.h"

#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/types/battle.h"

#include "lib/codegen.h"
extern "C" {

/*
 * Starts the grow effect: a 30-frame hold task and a 16-frame scale ramp task (+256 per frame)
 * on the actor named by actorSelector, sets the script wait to 14 frames and plays the centered
 * sound 0x29. Flag 2 is set on the other two fixed actors. unusedArg is ignored; returns 0.
 */
// FUNCTION: FF7 0x005d4240
int __cdecl BattleFx_EnterGrowActorScale(int targetMask, int actorIdx)
{
    int slotIdx;

    slotIdx = (short)Battle_AllocateResourceSlot(BattleFx_TaskHoldFrames);
    *(short *)g_BattleTaskSlotBuffer.mainSlots[slotIdx].data = 0x1E;
    slotIdx = (short)Battle_AllocateSecondaryResourceSlot(Battle3d_ActorScaleRampTask);
    *(short *)&g_BattleTaskSlotBuffer.secondarySlots[slotIdx].data[2] = actorIdx;
    *(short *)g_BattleTaskSlotBuffer.secondarySlots[slotIdx].data = 0x10;
    g_BattleTaskSlotBuffer.secondarySlots[slotIdx].frameCounter = 0x100;
    g_BattleScriptWaitFrames = 0xE;
    Battle_PlaySfxCenteredMulti(0x29);
    if (actorIdx == 0) {
        g_BattleActorRecords[1].stateFlags = g_BattleActorRecords[1].stateFlags | 2;
        g_BattleActorRecords[2].stateFlags = g_BattleActorRecords[2].stateFlags | 2;
    }
    if (actorIdx == 1) {
        g_BattleActorRecords[0].stateFlags = g_BattleActorRecords[0].stateFlags | 2;
        g_BattleActorRecords[2].stateFlags = g_BattleActorRecords[2].stateFlags | 2;
    }
    if (actorIdx == 2) {
        g_BattleActorRecords[0].stateFlags = g_BattleActorRecords[0].stateFlags | 2;
        g_BattleActorRecords[1].stateFlags = g_BattleActorRecords[1].stateFlags | 2;
    }
    return 0;
}

/*
 * Hold task: when its frame counter (the first payload short) reaches zero it frees its slot,
 * then the counter is decremented.
 */
// FUNCTION: FF7 0x005d4331
void __cdecl BattleFx_TaskHoldFrames(void)
{
    if (*(short *)g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].data == 0) {
        g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].ownerSlot = BATTLE_TASK_FREE;
    }
    (*(short *)g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].data)--;
}
} /* extern "C" */
