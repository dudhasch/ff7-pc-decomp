// src/ff7/src/battle/battle_status_effect_handlers.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Handles status effects on battle actors: element affinity masks and immunities, status
//   timers, tick rates and slip HP, and the events each status change raises.

// clang-format off
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "ff7/enums/battle.h"
#include "ff7/battle.h"
#include "lib/codegen.h"
#include "lib/crt.h"
#include "ff7/api/menu.h"
#include "ff7/savemap.h"
#include "ff7/types/menu.h"
extern "C" {

// How long each timed status lasts, by timer slot (enum BattleStatusTimer), in ticks of the actor's
// status clock (Battle_RefreshActorTickRates): Stop 30, Paralysis 20, Death Sentence 60, Slow-numb 30,
// Barrier, MBarrier, Regen and Dual Drain 127, Poison 10, Sleep 100, Shield and Peerless 64.
// GLOBAL: FF7 0x007b74f0
extern const unsigned char g_BattleStatusInitialDurations[16] = {0x1e, 0x14, 0x3c, 0x1e, 0x7f, 0x7f, 0x0a, 0x64, 0x7f, 0x7f, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00};
// The status bit each timer slot runs for (0x0A Stop, 0x19 Paralysis, 0x15 Death Sentence, ...),
// 0xFF for the four unused slots.
// GLOBAL: FF7 0x007b7500
const unsigned char g_BattleStatusTimerSlots[16] = {0x0a, 0x19, 0x15, 0x0d, 0x10, 0x11, 0x03, 0x02, 0x0f, 0x1b, 0x14, 0x18, 0xff, 0xff, 0xff, 0xff};
// The status-transition callbacks (g_BattleStatusTransitionCallbacks), indexed by status bit; each is called
// with (actor, status, set).
typedef void(__cdecl *BattleStatusCallback)(int, int, int);
// GLOBAL: FF7 0x007c2980
BattleStatusCallback g_BattleStatusTransitionCallbacks[16] = {
    (BattleStatusCallback)Battle_RemoveActorAndAwardSpoils,
    (BattleStatusCallback)Battle_OnStatusDeathCleared,
    (BattleStatusCallback)Battle_OnStatusSpeedChanged,
    (BattleStatusCallback)Battle_OnStatusCommandSetChanged,
    (BattleStatusCallback)Battle_OnStatusIncapacitateSet,
    (BattleStatusCallback)Battle_OnStatusIncapacitateCleared,
    (BattleStatusCallback)Battle_RefreshSlipHpRate,
    (BattleStatusCallback)Battle_OnStatusReflectSet,
    (BattleStatusCallback)Battle_OnStatusAutoControlSet,
    (BattleStatusCallback)Battle_ResumeActorAfterStatus,
    (BattleStatusCallback)Battle_OnStatusAutoControlCleared,
    (BattleStatusCallback)Battle_OnStatusCapturedChanged,
    (BattleStatusCallback)Battle_OnStatusManipulateChanged,
    (BattleStatusCallback)Battle_StartStatusTimer,
    (BattleStatusCallback)Battle_StopStatusTimer,
    (BattleStatusCallback)Battle_OnStatusReflectCleared,
};
// Per status bit, the g_BattleStatusTransitionCallbacks entry run when the bit turns on (this
// table) or off (the next one); 0xFF runs nothing.
// GLOBAL: FF7 0x007c29c0
unsigned char g_BattleStatusSetCallbackIndices[32] = {0x00, 0xff, 0x04, 0x0d, 0xff, 0xff, 0x08, 0x03, 0x02, 0x02, 0x04, 0x03, 0xff, 0x0d, 0x04, 0x06, 0x0d, 0x0d, 0x07, 0xff, 0x0d, 0x0d, 0x0c, 0x08, 0x0d, 0x04, 0xff, 0x06, 0xff, 0xff, 0xff, 0x0b};
// GLOBAL: FF7 0x007c29e0
unsigned char g_BattleStatusClearCallbackIndices[32] = {0x01, 0xff, 0x05, 0x0e, 0xff, 0xff, 0x0a, 0x03, 0x02, 0x02, 0x05, 0x03, 0xff, 0x0e, 0x05, 0x06, 0x0e, 0x0e, 0x0f, 0xff, 0x0e, 0x0e, 0x0c, 0x0a, 0x0e, 0x05, 0xff, 0x06, 0xff, 0xff, 0xff, 0x0b};
// clang-format on

/*
 * How an actor reacts to an element or a status, for the AI: the first affinity code 0..7 whose
 * slot holds one of the given elements or statuses, or 3 (no affinity) when none does. The codes
 * are the bits of BattleElementReaction: 0 instant death, 2 weak, 4 half, 5 nullify, 6 absorb,
 * 7 full recovery. The table is built as for an Item (no Shield, Resist or Death-force bits).
 */
// FUNCTION: FF7 0x00433510
int __cdecl Battle_FindElementAffinityIndex(int actor, int elementMask, int statusMask)
{
    struct {
        int index;
        BattleAffinityTable affinityTable;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    Battle_BuildElementAffinityTable(elementMask, statusMask, actor, &frame.affinityTable, 0, 0);
    for (frame.index = 0; frame.index < 8; frame.index++) {
        if ((frame.affinityTable.entries[frame.index] & elementMask) != 0) {
            break;
        }
        if ((frame.affinityTable.entries[frame.index + 8] & statusMask) != 0) {
            break;
        }
    }
    if (frame.index == 8) {
        frame.index = 3;
    }
    return frame.index;
}

/* Element bit 4, Poison, in the kernel's element order (well established). */
#define BATTLE_ELEMENT_POISON 0x10

/*
 * Builds an actor's affinity table against one attack: for each affinity code 0..7 (see
 * BattleElementReaction) the attack's elements (entries 0..7) and statuses (entries 8..15) that
 * the actor reacts to that way. The named members are code 4 half (elementBits), 5 nullify
 * (statusBits) and 6 absorb (elementFlags) for elements, and code 5 for statuses (immunityBits):
 * status immunity is the nullify of a status.
 *
 * A party member brings the halve, nullify and absorb words of its battle stat block; an enemy
 * its eight (element or status, code) pairs from the scene, ids 32 and up meaning statuses. Then,
 * for both:
 * its status immunities (Battle_BuildStatusImmunityMask); against anything but an Item, Shield
 * absorbs Fire to Holy (0x1FF) and nullifies Restorative to Shout (0x7E00); the combatant's own
 * absorb and nullify words. Nullifying the Poison element and immunity to the Poison status
 * imply each other.
 *
 * Finally every entry keeps only the attack's elements or statuses, and status immunity counts
 * only when it covers the attack's whole status change. Slot 3 gets only the common part.
 */
// FUNCTION: FF7 0x00433580
void __cdecl Battle_BuildElementAffinityTable(int elementMask, int statusMask, int actor, BattleAffinityTable *outAffinityTable, int isCommandNotType4, int statusFlag1)
{
    int idx;
    int category;
    BattleEnemyRecord *enemyRec;
    BattlePartyStatWords *pPartyStat;
    int elementId;
    int wordIdx;

    for (idx = 0; idx < 8; idx++) {
        outAffinityTable->elementEntries[idx] = 0;
        outAffinityTable->statusEntries[idx] = 0;
    }
    if (actor < 3) {
        pPartyStat = (BattlePartyStatWords *)((int)g_MenuPartyStats[actor].statBlock);
        outAffinityTable->elementBits = pPartyStat->elementBits;
        outAffinityTable->statusBits = pPartyStat->statusBits;
        outAffinityTable->elementFlags = pPartyStat->elementFlags;
    } else if (actor >= 4) {
        enemyRec = &g_BattleState.enemyRecords[g_BattleScene.enemies[actor - 4].enemyIndex];
        for (idx = 0; (unsigned int)idx < (unsigned int)8; idx++) {
            elementId = enemyRec->elementIds[idx];
            category = enemyRec->elementCategories[idx];
            if (elementId != 0xFF) {
                wordIdx = elementId / 32;
                outAffinityTable->entries[category + wordIdx * 8] |= 1 << ((elementId) % 32);
            }
        }
    }
    outAffinityTable->immunityBits |= Battle_BuildStatusImmunityMask(actor, isCommandNotType4, statusFlag1);
    // Shield: absorb Fire..Holy, nullify Restorative..Shout.
    if (isCommandNotType4 != 0 && (g_aCombatants[actor].status & BATTLE_STATUS_SHIELD) != 0) {
        outAffinityTable->elementFlags |= 0x1FF;
        outAffinityTable->statusBits |= 0x7E00;
    }
    outAffinityTable->elementFlags |= g_aCombatants[actor].elementMask;
    outAffinityTable->statusBits |= g_aCombatants[actor].statusBits;
    // statusBits is the element-nullify word here: 0x10 is the Poison element, not Sadness.
    if ((outAffinityTable->statusBits & BATTLE_ELEMENT_POISON) != 0 || (outAffinityTable->immunityBits & BATTLE_STATUS_POISON) != 0) {
        outAffinityTable->statusBits |= BATTLE_ELEMENT_POISON;
        outAffinityTable->immunityBits |= BATTLE_STATUS_POISON;
    }
    for (idx = 0; idx < 8; idx++) {
        outAffinityTable->elementEntries[idx] &= elementMask;
        outAffinityTable->statusEntries[idx] &= statusMask;
        if (idx == 5 && outAffinityTable->statusEntries[idx] != statusMask) {
            outAffinityTable->statusEntries[idx] = 0;
        }
    }
}

/*
 * An AI query: which of the actors in actorMask react to these elements or statuses, and how. Each
 * combatant's elementAffinity gets its code (3, none, for everyone else), and the AI target mask
 * (lastActorMask) becomes the actors with a reaction. Despite the name nothing is applied.
 */
// FUNCTION: FF7 0x004337f3
void __cdecl Battle_ApplyStatusToActorMask(int actorMask, int elementMask, int statusMask)
{
    int resultMask;
    int actorIndex;
    int affinity;

    resultMask = 0;
    for (actorIndex = 0; actorIndex < 10; actorIndex++) {
        g_aCombatants[actorIndex].elementAffinity = 3;
        if ((actorMask & 1 << actorIndex) != 0) {
            affinity = Battle_FindElementAffinityIndex(actorIndex, elementMask, statusMask);
            if (affinity != 3) {
                g_aCombatants[actorIndex].elementAffinity = affinity;
                resultMask |= 1 << actorIndex;
            }
        }
    }
    gBattleAiGlobals.lastActorMask = resultMask;
}

/*
 * The hit's elemental reaction (elemReaction): one bit per affinity code the target has for the
 * attack's elements or statuses (see BattleElementReaction). Shield and Resist count unless the
 * command is Item (4); a revive (Death in the cure mask) may pass Death immunity.
 *
 * Status immunity nullifies only a status-only attack: an attack with power still does its damage
 * to a target immune to its status. An attack without BATTLE_SPECIAL_NOT_IGNORE_IMMUNITY ignores
 * every affinity.
 *
 * The packed frame retains the output array layout.
 */
// FUNCTION: FF7 0x00433887
void __cdecl Battle_ResolveElementAffinity(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[88];
        int commandNotFour;
        struct { unsigned char pad_to_4[0x4]; int statusMask; };
        struct { unsigned char pad_to_8[0x8]; int statusFlag1; };
        struct { unsigned char pad_to_C[0xC]; int entryIndex; };
        struct { unsigned char pad_to_10[0x10]; int reactionBits; };
        struct { unsigned char pad_to_14[0x14]; int elementMask; };
        struct { unsigned char pad_to_18[0x18]; BattleAffinityTable affinityTable; };
    } frame;
#pragma pack(pop)

    *(short *)&frame.reactionBits = 0;
    frame.statusMask = 0;
    frame.elementMask = g_pBattleCommandCtx->elementMask;
    for (frame.entryIndex = 0; frame.entryIndex < 3; frame.entryIndex++) {
        frame.statusMask |= (&g_pBattleCommandCtx->statusMaskA)[frame.entryIndex];
    }
    frame.commandNotFour = g_pBattleCommandCtx->commandIndex != 4;
    frame.statusFlag1 = (g_pBattleCommandCtx->statusMaskB & BATTLE_STATUS_DEATH) != 0;
    Battle_BuildElementAffinityTable(frame.elementMask, frame.statusMask, g_pBattleCommandCtx->targetSlot, &frame.affinityTable, frame.commandNotFour, frame.statusFlag1);
    if (g_pBattleCommandCtx->power != 0 && frame.affinityTable.immunityBits != 0) {
        frame.affinityTable.immunityBits = 0;
    }
    for (frame.entryIndex = 0; frame.entryIndex < 16; frame.entryIndex++) {
        if (frame.affinityTable.entries[frame.entryIndex] != 0) {
            *(short *)&frame.reactionBits |= (short)(1 << (frame.entryIndex % 8));
        }
    }
    g_pBattleCommandCtx->elemReaction = frame.reactionBits & 0xFFFF;
    if ((g_pBattleCommandCtx->specialFlags & BATTLE_SPECIAL_NOT_IGNORE_IMMUNITY) == 0) {
        g_pBattleCommandCtx->elemReaction = 0;
    }
}

/*
 * How fast an actor's time runs: its ATB gauge and its two status clocks. Called whenever a status
 * that matters changes (Haste, Slow, Stop, Sleep, Petrify, Paralysis, Death, revive, capture).
 *
 * The clock rate (speedRate) starts at the battle speed (g_BattleState.atbSpeedValue), the second
 * clock (tickRate) at 34: both are doubled for a normal actor and doubled again under Haste, so
 * Slow runs at half the normal rate and Haste at twice it. Death and Stop freeze both.
 * Battle_AccumulateTimeSlices adds them up, slice by slice, for the actor's status timers and slip
 * HP, so Haste and Slow also speed up and slow down its Regen, Barrier, Poison and other timers.
 *
 * The gauge fill (atbTick) is the clock rate times the actor's speed over the party's average
 * speed (atbBaseDivisor, average + 50): a party member's speed is its Dexterity, with its stat
 * percentage, plus 50; an enemy's is its bare Dexterity. Sleep, Petrify and Paralysis stop the
 * gauge but not the clocks, so their own timers still run out.
 *
 * A captured actor (status bit 31) counts as stopped or paralysed according to the capture mode
 * (g_BattleState.statusChangeMode); that is only for this computation, the status word is not
 * written. Stores the three values as shorts in the actor's BattleActorRuntime, without a
 * zero-divisor or actor-bound guard.
 */
// FUNCTION: FF7 0x004339c2
void __cdecl Battle_RefreshActorTickRates(int actor)
{
    struct {
        int modifier;
        int increment;
        int speed;
        int status;
        int tick;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.status = g_aCombatants[actor].status;
    frame.speed = 0;
    frame.tick = 0;
    frame.increment = 0;
    if ((frame.status & 0x80000000) != 0) {
        switch (g_BattleState.statusChangeMode) {
        case BATTLE_CAPTURE_STOP:
        case BATTLE_CAPTURE_STOP_POSE:
            frame.status |= BATTLE_STATUS_STOP;
            break;
        case BATTLE_CAPTURE_PARALYZE_SLIP:
            frame.status |= BATTLE_STATUS_PARALYSIS;
            break;
        }
    }
    if ((frame.status & (BATTLE_STATUS_DEATH | BATTLE_STATUS_STOP)) == 0) {
        frame.speed = g_BattleState.atbSpeedValue;
        frame.increment = 34;
        if ((frame.status & BATTLE_STATUS_SLOW) == 0) {
            frame.speed *= 2;
            frame.increment *= 2;
            if ((frame.status & BATTLE_STATUS_HASTE) != 0) {
                frame.speed *= 2;
                frame.increment *= 2;
            }
        }
    }
    if (actor < 3) {
        frame.modifier = Battle_ApplyStatPercentModifier(actor, g_aCombatants[actor].dexterity, 5);
        frame.tick = frame.speed * (frame.modifier + 50) / g_BattleState.atbBaseDivisor;
    } else {
        frame.tick = frame.speed * g_aCombatants[actor].dexterity / g_BattleState.atbBaseDivisor;
    }
    if ((frame.status & (BATTLE_STATUS_SLEEP | BATTLE_STATUS_PETRIFY | BATTLE_STATUS_PARALYSIS)) != 0) {
        frame.tick = 0;
    }
    g_BattleState.actors[actor].speedRate = frame.speed;
    g_BattleState.actors[actor].atbTick = frame.tick;
    g_BattleState.actors[actor].tickRate = frame.increment;
}

/*
 * Starts the timer of a timed status (status is the bit number) that just turned on, at its full
 * duration from g_BattleStatusInitialDurations; an untimed status does nothing. The timer slots
 * run on past the four bytes the statusTimers member declares.
 */
// FUNCTION: FF7 0x00433b3e
void __cdecl Battle_StartStatusTimer(int actor, int status, int set)
{
    int val;

    val = Battle_FindStatusTimerSlot(status);
    if (val >= 0) {
        g_BattleState.actors[actor].statusTimers[val] = g_BattleStatusInitialDurations[val];
    }
}

/*
 * The timer slots whose status shows a visual effect on the actor: Stop, Paralysis, Slow-numb,
 * Sleep, Regen, Shield and Peerless (bits of enum BattleStatusTimer).
 */
#define BATTLE_TIMER_SLOTS_WITH_EFFECT 0xD8B

/*
 * Stops the timer of a timed status that just turned off, and for Stop, Paralysis, Slow-numb,
 * Sleep, Regen, Shield and Peerless also ends the effect shown on the actor.
 */
// FUNCTION: FF7 0x00433b74
void __cdecl Battle_StopStatusTimer(int actor, int status, int set)
{
    int val;

    val = Battle_FindStatusTimerSlot(status);
    if (val >= 0) {
        g_BattleState.actors[actor].statusTimers[val] = 0;
        if ((1 << val & BATTLE_TIMER_SLOTS_WITH_EFFECT) != 0) {
            Battle_StopActorStatusEffect(actor);
        }
    }
}

/*
 * The status handler for Haste and Slow (and part of most others): recomputes the actor's ATB and
 * clock rates.
 */
// FUNCTION: FF7 0x00433bc1
void __cdecl Battle_OnStatusSpeedChanged(int actor, int status, int set)
{
    Battle_RefreshActorTickRates(actor);
}

/* Battle messages 0x54 ("Received {NUM} gil.") and 0x52 ("Received {ITEM}."). */
#define BATTLE_MSG_RECEIVED_GIL 0x54
#define BATTLE_MSG_RECEIVED_ITEM 0x52

/*
 * The status handler for Death turning on: the actor is KO'd and leaves the fight.
 *
 * An enemy can no longer be targeted and its AI stops. A party member's Limit gauge empties (well
 * established: a KO resets the Limit gauge), a member with timing flag 8 loses it and AI control
 * and plays animation 3, its max HP is restored from its party record, and an enemy it was
 * manipulating is released. For everyone: HP 0, the ATB stops, slip HP, status timers and stat percentages are
 * cleared, its pending drops are released, its queued actions are dropped and it leaves the turn
 * order.
 *
 * Spoils the actor still carries (spoilA gil, spoilB an item, 0xFFFF none) are handed over now,
 * with a "Received" message each, the item to whoever is acting; an actor in excludedActorMask
 * gives none.
 */
// FUNCTION: FF7 0x00433bd2
void __cdecl Battle_RemoveActorAndAwardSpoils(int actor, int status, int set)
{
    int manipSlot;
    unsigned short money;
    int clearIdx;
    unsigned short dropItem;

    if (actor >= 3) {
        g_aCombatants[actor].flags &= ~(BATTLE_COMBATANT_TARGETABLE | BATTLE_COMBATANT_AI_CONTROLLED);
    }
    if (actor < 3) {
        g_BattleState.partyStats[actor].limitGauge = 0;
        if ((g_BattleState.actors[actor].timingFlags & 8) != 0) {
            g_BattleState.actors[actor].timingFlags &= 0xF7;
            g_aCombatants[actor].flags &= ~BATTLE_COMBATANT_AI_CONTROLLED;
            Battle_PushAnimStepWithHitRecord(actor, 3, 0, 0, 0, 0, 0);
        }
        g_aCombatants[actor].maxHp = (unsigned int)(*(unsigned short *)&g_BattleState.partyStats[actor].maxHp);
        Battle_PostEvent(BATTLE_EVENT_QUEUE_ACTION, actor, BATTLE_ACTION_EVENT_COPY_PARTY_STAT_WORDS, 0);
    }
    if (actor < 3) {
        manipSlot = g_BattleState.partyStats[actor].manipulatedSlot;
        if (manipSlot >= 4) {
            g_aCombatants[manipSlot].status &= ~BATTLE_STATUS_MANIPULATE;
        }
    }
    g_aCombatants[actor].curHp = 0;
    Battle_OnStatusSpeedChanged(actor, status, set);
    g_BattleState.actors[actor].slipRate = 0;
    ((BattleActorResumeMarker *)&g_BattleState.actorResumeMarker)[actor].marker = 0xFF;
    Battle_ReleasePendingDrops((short)actor);
    for (clearIdx = 0; clearIdx < 16; clearIdx++) {
        g_BattleState.actors[actor].statusTimers[clearIdx] = 0;
    }
    for (clearIdx = 0; clearIdx < 8; clearIdx++) {
        g_BattleState.actors[actor].statPercent[clearIdx] = 0;
    }
    if (((g_BattleState.excludedActorMask) & 1 << actor) == 0) {
        if (*(unsigned short *)&g_aCombatants[actor].spoilA != 0) {
            money = g_aCombatants[actor].spoilA;
            g_aCombatants[actor].spoilA = 0;
            gSavemap.gil += money;
            Battle_PostBattleMessageFormatted(0xA, BATTLE_MSG_RECEIVED_GIL, 1, &money);
        }
        if (*(unsigned short *)&g_aCombatants[actor].spoilB != 0xFFFF) {
            dropItem = g_aCombatants[actor].spoilB;
            g_aCombatants[actor].spoilB = 0xFFFF;
            Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, g_pBattleCommandCtx->attackerSlot, BATTLE_ATB_EVENT_ADD_ITEM_DROP, dropItem);
            Battle_PostBattleMessageFormatted(0xA, BATTLE_MSG_RECEIVED_ITEM, 1, &dropItem);
        }
    }
    Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, actor, BATTLE_ATB_EVENT_REMOVE_FROM_TURN_ORDER, 0);
    Battle_StopActorStatusEffect(actor);
    Battle_InvalidateQueuedActionsForActor(actor, 1);
}

/*
 * The status handler for Death turning off: the actor is revived. One that comes back with 0 HP
 * gets its max HP. An enemy is targetable and AI-driven again, the scene actor gets back the state
 * it started the battle in, and the ATB runs again. A status the KO did not strip (one the actor
 * cannot lose, see Battle_DispatchStatusTransitions) takes effect again: Death Sentence restarts
 * its countdown (status bit 0x15), Berserk hands the actor back to auto control.
 */
// FUNCTION: FF7 0x00433e8d
void __cdecl Battle_OnStatusDeathCleared(int actor, int status, int set)
{
    if (g_aCombatants[actor].curHp == 0) {
        g_aCombatants[actor].curHp = g_aCombatants[actor].maxHp;
    }
    if (actor >= 3) {
        g_aCombatants[actor].flags |= (BATTLE_COMBATANT_TARGETABLE | BATTLE_COMBATANT_AI_CONTROLLED);
    }
    g_aCombatants[actor].flags &= ~BATTLE_COMBATANT_DEATH_HANDLED;
    g_BattleScene.actors[actor].state = g_BattleState.actors[actor].sceneState;
    Battle_OnStatusSpeedChanged(actor, status, set);
    if ((g_aCombatants[actor].status & BATTLE_STATUS_DEATH_SENTENCE) != 0) {
        Battle_StartStatusTimer(actor, 0x15, 1);
    }
    if ((g_aCombatants[actor].status & BATTLE_STATUS_BERSERK) != 0) {
        Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, actor, BATTLE_ATB_EVENT_ENTER_AUTO_CONTROL, 0);
    }
    g_BattleState.deathClearedMask &= (short)~(1 << actor);
}

/*
 * The actor can act again after a status ended. Once it is free of Sleep, Confusion, Stop, Petrify,
 * Berserk and Paralysis its command menu opens again. Once it is free of the four that freeze it
 * (Sleep, Stop, Petrify, Paralysis), a command it had chosen before (actorCommandIssuedMask, its
 * resume marker) is queued after all.
 */
// FUNCTION: FF7 0x00433f8a
void __cdecl Battle_ResumeActorAfterStatus(int actor, int status, int set)
{
    if ((g_aCombatants[actor].status & (BATTLE_STATUS_SLEEP | BATTLE_STATUS_CONFUSION | BATTLE_STATUS_STOP | BATTLE_STATUS_PETRIFY | BATTLE_STATUS_BERSERK | BATTLE_STATUS_PARALYSIS)) == 0) {
        Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, actor, BATTLE_ATB_EVENT_REOPEN_COMMAND_MENU, 0);
    }
    if ((g_aCombatants[actor].status & (BATTLE_STATUS_SLEEP | BATTLE_STATUS_STOP | BATTLE_STATUS_PETRIFY | BATTLE_STATUS_PARALYSIS)) == 0 && (g_BattleState.actorCommandIssuedMask & 1 << actor) != 0 && ((BattleActorResumeMarker *)&g_BattleState.actorResumeMarker)[actor].marker != 0xFF) {
        Battle_EnqueueAction(&((BattleActorResumeMarker *)&g_BattleState.actorResumeMarker)[actor]);
        ((BattleActorResumeMarker *)&g_BattleState.actorResumeMarker)[actor].marker = 0xFF;
    }
}

/*
 * The status handler for Confusion and Berserk turning on: the actor acts on its own.
 */
// FUNCTION: FF7 0x00434018
void __cdecl Battle_OnStatusAutoControlSet(int actor)
{
    Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, actor, BATTLE_ATB_EVENT_ENTER_AUTO_CONTROL, 0);
}

/*
 * The status handler for Confusion or Berserk turning off: once neither is left, the player gets
 * the actor back (Battle_ResumeActorAfterStatus).
 *
 * The mask is the immediate 0x00800040, Confusion | Berserk. The tree spells it as the address of
 * g_BattleSummonOdinGVertexDeltasData1, the global that happens to sit at 0x00800040.
 */
// FUNCTION: FF7 0x0043402f
void __cdecl Battle_OnStatusAutoControlCleared(int actor, int status, int set)
{
    if ((g_aCombatants[actor].status & (int)&g_BattleSummonOdinGVertexDeltasData1) == 0) {
        Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, actor, BATTLE_ATB_EVENT_LEAVE_AUTO_CONTROL, 0);
        Battle_ResumeActorAfterStatus(actor, status, set);
    }
}

/* The status bit number of Petrify (BATTLE_STATUS_PETRIFY is 1 << 14). */
#define BATTLE_STATUS_BIT_PETRIFY 0xE

/* What Petrify keeps: every status but Sleep, Poison, Confusion, Regen and Dual Drain. */
#define BATTLE_PETRIFY_KEEPS_STATUS 0xF7FF7FB3

/*
 * The status handler for Sleep, Stop, Petrify and Paralysis turning on: the actor freezes. Its ATB
 * stops, the status's timer starts, its command menu closes and it is no longer manipulated.
 * Petrify also ends Sleep, Poison, Confusion, Regen and Dual Drain.
 */
// FUNCTION: FF7 0x00434070
void __cdecl Battle_OnStatusIncapacitateSet(int actor, int status, int set)
{
    Battle_OnStatusSpeedChanged(actor, status, set);
    Battle_StartStatusTimer(actor, status, set);
    Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, actor, BATTLE_ATB_EVENT_RELEASE_COMMAND_MENU, 0);
    g_aCombatants[actor].status &= ~BATTLE_STATUS_MANIPULATE;
    if (status == BATTLE_STATUS_BIT_PETRIFY) {
        g_aCombatants[actor].status &= BATTLE_PETRIFY_KEEPS_STATUS;
    }
}

/*
 * The status handler for Sleep, Stop, Petrify and Paralysis turning off: the ATB runs again, the
 * timer stops and the actor gets its turn back (Battle_ResumeActorAfterStatus).
 */
// FUNCTION: FF7 0x004340f1
void __cdecl Battle_OnStatusIncapacitateCleared(int actor, int status, int set)
{
    Battle_OnStatusSpeedChanged(actor, status, set);
    Battle_StopStatusTimer(actor, status, set);
    Battle_ResumeActorAfterStatus(actor, status, set);
}

/*
 * The status handler for Silence and Frog, on or off: the commands the actor may use change, so
 * its command menu is marked for rebuilding (dirty flags 0xF).
 */
// FUNCTION: FF7 0x00434132
void __cdecl Battle_OnStatusCommandSetChanged(int actor)
{
    Battle_PostEvent(BATTLE_EVENT_QUEUE_ACTION, actor, BATTLE_ACTION_EVENT_SET_DIRTY_FLAGS, 0xF);
}

/*
 * The status handler for Regen and Dual Drain: the actor's slip HP rate, a 32nd of its max HP
 * gained under Regen and lost under Dual Drain (the two cancel out). A capture in the
 * paralyse-and-drain mode drains like Dual Drain. Then the status's timer starts or stops.
 */
// FUNCTION: FF7 0x00434149
void __cdecl Battle_RefreshSlipHpRate(int actor, int status, int set)
{
    int unit;
    int statusFlags;
    int slipRate;

    slipRate = 0;
    unit = (*(unsigned int *)&g_aCombatants[actor].maxHp) >> 5;
    statusFlags = g_aCombatants[actor].status;
    if ((statusFlags & 0x80000000) != 0 && g_BattleState.statusChangeMode == BATTLE_CAPTURE_PARALYZE_SLIP) {
        statusFlags |= BATTLE_STATUS_DUAL_DRAIN;
    }
    if ((statusFlags & BATTLE_STATUS_REGEN) != 0) {
        slipRate += unit;
    }
    if ((statusFlags & BATTLE_STATUS_DUAL_DRAIN) != 0) {
        slipRate -= unit;
    }
    g_BattleState.actors[actor].slipRate = slipRate;
    if (set != 0) {
        Battle_StartStatusTimer(actor, status, set);
    } else {
        Battle_StopStatusTimer(actor, status, set);
    }
}

/*
 * The status handler for Reflect turning on: three spare bounces (reflectState).
 * Battle_ResolveHitOnTarget counts them down, and the bounce that finds none left wears Reflect
 * off, so Reflect bounces four spells. Recasting it on a reflecting actor changes no status bit
 * and so does not refill it.
 */
// FUNCTION: FF7 0x0043420d
void __cdecl Battle_OnStatusReflectSet(int actor)
{
    g_BattleState.actors[actor].reflectState = 3;
}

/*
 * The status handler for the capture bit (31), on or off. By the capture mode the attack chose
 * (g_BattleState.statusChangeMode) the captured actor is frozen like Stop, frozen like Paralysis
 * while losing HP (that case falls through into the Stop cases), or frozen in idle pose 19
 * (BATTLE_CAPTURE_STOP_POSE). Mode 2 does nothing.
 */
// FUNCTION: FF7 0x0043421f
void __cdecl Battle_OnStatusCapturedChanged(int actor, int status, int set)
{
    switch (g_BattleState.statusChangeMode) {
    case BATTLE_CAPTURE_PARALYZE_SLIP:
        Battle_RefreshSlipHpRate(actor, status, set);
    case BATTLE_CAPTURE_STOP:
    case BATTLE_CAPTURE_STOP_POSE:
        if (set != 0) {
            Battle_OnStatusIncapacitateSet(actor, status, set);
        } else {
            Battle_OnStatusIncapacitateCleared(actor, status, set);
        }
        break;
    }
    if (g_BattleState.statusChangeMode == BATTLE_CAPTURE_STOP_POSE) {
        g_aCombatants[actor].idleOverride = set ? 19 : 0;
    }
}

/*
 * The status handler for Manipulate, on or off. A manipulated actor's own menu is suppressed (and
 * it stops being the tracked turn actor). When the manipulation ends the party member that held
 * it is free (manipulatedSlot 0) and gets its command menu back, and the effect shown on the
 * actor ends.
 */
// FUNCTION: FF7 0x004342ae
void __cdecl Battle_OnStatusManipulateChanged(int actor, int status, int set)
{
    int i;

    Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, actor, BATTLE_ATB_EVENT_SET_MENU_SUPPRESSED, set);
    if (set != 0) {
        Battle_ClearTrackedTurnActorIf(actor);
    } else {
        for (i = 0; i < 3; i++) {
            if (g_BattleState.partyStats[i].manipulatedSlot == actor) {
                g_BattleState.partyStats[i].manipulatedSlot = 0;
                Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, i, BATTLE_ATB_EVENT_REOPEN_COMMAND_MENU, 0);
            }
        }
        Battle_StopActorStatusEffect(actor);
    }
}

/*
 * The status handler for Reflect turning off: ends the Reflect effect shown on the actor.
 */
// FUNCTION: FF7 0x00434336
void __cdecl Battle_OnStatusReflectCleared(int actor)
{
    Battle_StopActorStatusEffect(actor);
}

/* What a KO keeps: Death, Frog, Small and the capture bit (plus what the actor cannot lose). */
#define BATTLE_KO_KEEPS_STATUS 0x80001801

/*
 * Makes status changes take effect: for every bit of every combatant that turned on or off since
 * the last call, runs its handler (g_BattleStatusSetCallbackIndices or
 * g_BattleStatusClearCallbackIndices into g_BattleStatusTransitionCallbacks). Returns 1 when a
 * handler changed a status in turn, so the caller runs it again.
 *
 * First a KO strips every status but Death, Frog, Small, the capture bit and those in the actor's
 * statusImmunityMask (statuses it cannot lose; probably an accessory's auto-status, unconfirmed). Each handler runs once per actor per call, except the timer starter and stopper
 * (entries 13 and 14, outside the 0xFFFF9FFF dedup mask), which run for every timed status that
 * changed. Empty slots (unitId -1) are skipped.
 */
// FUNCTION: FF7 0x00434347
int __cdecl Battle_DispatchStatusTransitions(void)
{
    int result, actorIdx, handled, auxPrevStatus, current, changeMask, bitNo, bitValue, turnedOn;
    unsigned char callbackIdx;
    int callbackMask;
    result = 0;
    for (actorIdx = 0; actorIdx < 10; actorIdx++) {
        if ((g_aCombatants[actorIdx].status & BATTLE_STATUS_DEATH) != 0) {
            g_aCombatants[actorIdx].status &= ~(~(g_BattleState.actors[actorIdx].statusImmunityMask | BATTLE_KO_KEEPS_STATUS));
        }
    }
    for (actorIdx = 0; actorIdx < 10; actorIdx++) {
        current = g_aCombatants[actorIdx].status;
        auxPrevStatus = g_aCombatants[actorIdx].prevStatusFlags;
        handled = 0;
        g_aCombatants[actorIdx].prevStatusFlags = g_aCombatants[actorIdx].status;
        if (g_aCombatants[actorIdx].unitId == -1) {
            continue;
        }
        if (auxPrevStatus != current) {
            bitValue = 1;
            changeMask = auxPrevStatus ^ current;
            for (bitNo = 0; bitNo < 32; bitNo++) {
                if ((bitValue & changeMask) != 0) {
                    turnedOn = (bitValue & current) != 0;
                    if (turnedOn != 0) {
                        callbackIdx = g_BattleStatusSetCallbackIndices[bitNo];
                    } else {
                        callbackIdx = g_BattleStatusClearCallbackIndices[bitNo];
                    }
                    if (callbackIdx != 255) {
                        callbackMask = (1 << callbackIdx) & 0xffff9fff;
                        if ((handled & callbackMask) == 0) {
                            handled |= callbackMask;
                            g_BattleStatusTransitionCallbacks[callbackIdx](actorIdx, bitNo, turnedOn);
                        }
                    }
                }
                bitValue <<= 1;
            }
        }
        if (g_aCombatants[actorIdx].prevStatusFlags != g_aCombatants[actorIdx].status) {
            result = 1;
        }
    }
    return result;
}

/*
 * The timer slot of a status (its bit number), or -1 for a status without a timer. The loop does
 * not break, so a duplicate entry in g_BattleStatusTimerSlots would win on the later index.
 */
// FUNCTION: FF7 0x00434524
int __cdecl Battle_FindStatusTimerSlot(int status)
{
    int foundIdx;
    int index;

    foundIdx = 0xFFFFFFFF;
    for (index = 0; index < 16; index++) {
        if (g_BattleStatusTimerSlots[index] == status) {
            foundIdx = index;
        }
    }
    return foundIdx;
}

/* Resist: immune to every status but Resist itself and the capture bit. */
#define BATTLE_RESIST_IMMUNITY 0x5FFFFFFF
/* Peerless: immune to every status but the capture bit. */
#define BATTLE_PEERLESS_IMMUNITY 0x7FFFFFFF

/*
 * The statuses an attack cannot change on the actor. It starts from the actor's own immunities
 * (an enemy's from the scene, ~BattleEnemyRecord.statusImmunityMask), then:
 *  - timing flag 8 adds Confusion, Frog and Berserk (0x800840);
 *  - against anything but an Item (isCommandNotType4), Resist makes it immune to all statuses but
 *    Resist, and Death-force to Death;
 *  - Peerless makes it immune to everything;
 *  - immunity to Haste or Slow is immunity to both;
 *  - a revive (statusFlag1: the attack cures Death) passes a party member's Death immunity, but not
 *    BATTLE_COMBATANT_IMMUNE_TO_DEATH;
 *  - immunity to Death is also immunity to Death Sentence.
 * An attack without BATTLE_SPECIAL_NOT_IGNORE_IMMUNITY ignores all of it (returns 0).
 */
// FUNCTION: FF7 0x00434568
int __cdecl Battle_BuildStatusImmunityMask(int actor, int isCommandNotType4, int statusFlag1)
{
    int val;

    val = g_BattleState.actors[actor].statusImmunityMask;
    if ((g_BattleState.actors[actor].timingFlags & 8) != 0) {
        val |= 0x800840;
    }
    if (isCommandNotType4 != 0) {
        if ((g_aCombatants[actor].status & BATTLE_STATUS_RESIST) != 0) {
            val |= BATTLE_RESIST_IMMUNITY;
        }
        if ((g_aCombatants[actor].status & BATTLE_STATUS_DEATH_FORCE) != 0) {
            val |= 1;
        }
    }
    if ((g_aCombatants[actor].status & BATTLE_STATUS_PEERLESS) != 0) {
        val |= BATTLE_PEERLESS_IMMUNITY;
    }
    if ((val & 0x300) != 0) {
        val |= 0x300;
    }
    if (actor < 3 && statusFlag1 != 0) {
        val &= 0xFFFFFFFE;
    }
    if ((g_aCombatants[actor].flags & BATTLE_COMBATANT_IMMUNE_TO_DEATH) != 0) {
        val |= 1;
    }
    if ((val & 1) != 0) {
        val |= 0x200000;
    }
    if ((g_pBattleCommandCtx->specialFlags & BATTLE_SPECIAL_NOT_IGNORE_IMMUNITY) == 0) {
        val = 0;
    }
    return val;
}
} /* extern "C" */
