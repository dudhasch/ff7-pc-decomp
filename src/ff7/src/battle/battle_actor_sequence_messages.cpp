// src/ff7/src/battle/battle_actor_sequence_messages.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle actor helpers: decides which side an actor is on, posts formatted battle
//   messages and sequence steps, pushes animation steps with hit records, and finds attack indices.
//   It also applies percentage stat modifiers and invalidates queued actions for an actor.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/battle.h"
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
 * Returns the parity of three tests: actor below four, bit 0x40 set in the combatant's status,
 * and bit 0x400000 set in its status.
 */
// FUNCTION: FF7 0x005c7c50
int __cdecl Battle_GetActorSide(int actor)
{
    int bits;
    int count;

    count = 0;
    bits = g_aCombatants[actor].status;
    if (actor < 4) {
        count++;
    }
    if ((bits & 0x40) != 0) {
        count++;
    }
    if ((bits & 0x400000) != 0) {
        count++;
    }
    return count & 1;
}

/*
 * signature: inferred from parameter accesses
 * Scans four matching drop/steal type entries, scales low-six chance or forces 256, accepts random
 * low-six value <=chance and returns first item short. Chance zero can succeed on random zero;
 * absent minus one.
 */
// FUNCTION: FF7 0x005c7cae
short __cdecl Battle_RollEnemyDropOrSteal(void *enemy, int kind, int forceFlag, int scale)
{
    struct {
        int chance;
        int entryIndex;
        short result;
        short pad;
        BattleEnemyRecord *data;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.result = -1;
    frame.data = (BattleEnemyRecord *)enemy;
    for (frame.entryIndex = 0; frame.entryIndex < 4; frame.entryIndex++) {
        if ((frame.data->dropStealRates[frame.entryIndex] & ~0x3f) == kind) {
            frame.chance = frame.data->dropStealRates[frame.entryIndex] & 0x3f;
            if (forceFlag != 0) {
                frame.chance = 0x100;
            } else {
                frame.chance = frame.chance * scale / 256;
            }
            if ((Battle_NextRandomByte() & 0x3f) <= frame.chance) {
                frame.result = frame.data->dropStealItems[frame.entryIndex];
                break;
            }
        }
    }
    return frame.result;
}

/*
 * Expands message messageId, queues it with queueArg, and passes the returned index plus 0x100 to
 * Battle_PushSequenceStep(actor, 2, mode, ...).
 */
// FUNCTION: FF7 0x005c7d59
void __cdecl Battle_PostBattleMessageFormatted(int actor, int messageId, int mode, void *queueArg)
{
    int index;

    index = Battle_QueueAiMessageText(Kernel_GetBattleMessageExpanded(messageId), (unsigned short *)queueArg);
    Battle_PushSequenceStep(actor, 2, mode, index + 0x100);
}

/*
 * One-line wrapper: calls Battle_PushSequenceStep(10, 2, 1, payload), queueing sequence step kind
 * 0xA with payload as its payload.
 */
// FUNCTION: FF7 0x005c7d99
void __cdecl Battle_PostBattleMessage(int payload)
{
    Battle_PushSequenceStep(10, 2, 1, payload);
}

/*
 * Calls Battle_PushSequenceStep(actor, 5, 0, 0).
 */
// FUNCTION: FF7 0x005c7db0
void __cdecl Battle_StopActorStatusEffect(int actor)
{
    Battle_PushSequenceStep(actor, 5, 0, 0);
}

/*
 * Compares the combatant's curHp with its maxHp shifted right by two, both unsigned; returns whether the former is less than or equal.
 */
// FUNCTION: FF7 0x005c7dc7
int __cdecl Battle_IsActorNearDeath(int actorSlot)
{
    return *(unsigned int *)&g_aCombatants[actorSlot].curHp <=
           (*(unsigned int *)&g_aCombatants[actorSlot].maxHp >> 2);
}

/*
 * Allocates sequence and hit records, writes narrowed parameters/fixed state and sentinel fields,
 * then pushes empty hit record.
 */
// FUNCTION: FF7 0x005c7dea
void __cdecl Battle_PushAnimStepWithHitRecord(int actorIndex, int animId, int stepFlags, int param, int power, int amount, int combatant)
{
    BattleSeqStep *seqStep;
    BattleHitRecord *hitRecord;

    seqStep = (BattleSeqStep *)Battle_AllocSequenceStep();
    hitRecord = (BattleHitRecord *)Battle_AllocHitRecord();
    seqStep->actorIndex = actorIndex;
    seqStep->actionType = 1;
    seqStep->animId = animId;
    seqStep->stepFlags = stepFlags;
    seqStep->effectId = param;
    seqStep->power = power;
    seqStep->clearedFlag = 0;
    seqStep->targetIndex = 0xFFFF;
    hitRecord->targetActor = actorIndex;
    hitRecord->sourceActor = actorIndex;
    hitRecord->animId = 0x33;
    hitRecord->flags = amount;
    hitRecord->status = combatant;
    Battle_PushEmptyHitRecord();
}

/*
 * Searches the first 32 entries of g_BattleState.attackEntries[0x380] for attackId and returns the
 * first matching index, or 32 when absent.
 */
// FUNCTION: FF7 0x005c7e7a
int __cdecl Battle_FindSceneAttackIndex(int attackId)
{
    int index;

    for (index = 0; index < 32; index++) {
        if (g_BattleState.attackIds[index] == attackId) {
            break;
        }
    }
    return index;
}

/*
 * Returns baseValue adjusted by a signed percentage: baseValue + baseValue *
 * g_BattleState.actors[actorIndex].statPercent[field] / 0x64.
 */
// FUNCTION: FF7 0x005c7eb3
int __cdecl Battle_ApplyStatPercentModifier(int actorIndex, int baseValue, int field)
{
    int result;

    result = baseValue + baseValue * g_BattleState.actors[actorIndex].statPercent[field] / 100;
    return result;
}

/*
 * For eight selected mask bits adds delta to signed-byte stat modifier and clamps minus 100..100
 * before storing.
 */
// FUNCTION: FF7 0x005c7ee5
void __cdecl Battle_AddStatPercentModifiers(int actorIndex, int delta, int mask)
{
    int idx;
    int value;

    for (idx = 0; idx < 8; idx++) {
        if ((mask & 1 << idx) != 0) {
            value = g_BattleState.actors[actorIndex].statPercent[idx] + delta;
            if (value > 100) {
                value = 0x64;
            }
            if (value < -100) {
                value = 0xFFFFFF9C;
            }
            g_BattleState.actors[actorIndex].statPercent[idx] = value;
        }
    }
}

/*
 * Returns one if g_BattleState.layoutType is between zero and two inclusive; otherwise returns
 * zero.
 */
// FUNCTION: FF7 0x005c7f60
int __cdecl Battle_IsPartyFirstLayout(void)
{
    int result;
    int layout;

    result = 0;
    layout = g_BattleState.layoutType;
    if (layout >= 0) {
        if (layout > 2) { /* empty first arm: lib/codegen.h */
        } else {
            result = 1;
        }
    }
    return result;
}

/*
 * Scans 64 stride-eight entries; matching signed byte actorIndex with unsigned level neither 0xff
 * nor below threshold has matching byte changed to minus one.
 */
// FUNCTION: FF7 0x005c7f94
void __cdecl Battle_InvalidateQueuedActionsForActor(int actorIndex, int threshold)
{
    int index;

    for (index = 0; index < 64; index++) {
        if (g_BattleState.queuedActions[index].actorIndex == actorIndex &&
            g_BattleState.queuedActions[index].level != 0xFF &&
            g_BattleState.queuedActions[index].level >= threshold) {
            g_BattleState.queuedActions[index].actorIndex = -1;
        }
    }
}
} /* extern "C" */
