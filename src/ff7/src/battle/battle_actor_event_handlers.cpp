// src/ff7/src/battle/battle_actor_event_handlers.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Event hooks the battle engine calls to reset or change an actor's turn, menu,
//   auto-control and ATB state, consume inventory items, expire status timers and post battle
//   events.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "ff7/enums/battle.h"
#include "lib/codegen.h"
#include "ff7/battle.h"
#include "lib/crt.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/api/token.h"
#include "ff7/savemap.h"
#include "ff7/types/menu.h"
#include "ff7/enums/main.h"
extern "C" {

// GLOBAL: FF7 0x007c2a00
unsigned int g_BattleEventQueue0Handlers[17] = {(unsigned int)&Battle_EventResetActorAtb, (unsigned int)&Battle_ResetLimitReadyState, (unsigned int)&Battle_EventRemoveActorFromTurnOrder, (unsigned int)&Battle_EventAddPendingItemDrop, (unsigned int)&Battle_EventReleaseActorCommandMenu, (unsigned int)&Battle_EventCancelActorTurn, (unsigned int)&Battle_EventReopenActorCommandMenu, (unsigned int)&Battle_EventSetAtbFreezeFlag, (unsigned int)&Battle_EventEnterAutoControl, (unsigned int)&Battle_EventLeaveAutoControl, (unsigned int)&Battle_EventSetActorMenuSuppressed, (unsigned int)&Battle_ConsumeRandomInventoryItem, (unsigned int)&Battle_EventSetActorDirtyFlags, (unsigned int)&Battle_EventRebuildActorAbilityLists, (unsigned int)&Battle_EventClearEnemySlotMenuMasks, (unsigned int)&Battle_ReopenPendingCommandMenus, (unsigned int)&Battle_ConsumeInventoryItemById};
// GLOBAL: FF7 0x007c2a48
unsigned int g_BattleEventQueue1Handlers[3] = {(unsigned int)&Battle_EventQueue1NoOp0, (unsigned int)&Battle_EventShowLimitBreakBanner, (unsigned int)&Battle_EventQueueItemStolen};
// GLOBAL: FF7 0x007c2a58
unsigned int g_BattleEventQueue2Handlers[25] = {(unsigned int)&Battle_StatusTimerExpireGeneric, (unsigned int)&Battle_StatusTimerExpireGeneric, (unsigned int)&Battle_StatusTimerExpireDeathSentence, (unsigned int)&Battle_StatusTimerExpireSlowNumb, (unsigned int)&Battle_StatusTimerExpireGeneric, (unsigned int)&Battle_StatusTimerExpireGeneric, (unsigned int)&Battle_StatusTimerExpirePoison, (unsigned int)&Battle_StatusTimerExpireGeneric, (unsigned int)&Battle_StatusTimerExpireGeneric, (unsigned int)&Battle_StatusTimerExpireGeneric, (unsigned int)&Battle_StatusTimerExpireGeneric, (unsigned int)&Battle_StatusTimerExpireGeneric, (unsigned int)&Battle_StatusTimerExpireNoOp, (unsigned int)&Battle_StatusTimerExpireNoOp, (unsigned int)&Battle_StatusTimerExpireNoOp, (unsigned int)&Battle_StatusTimerExpireNoOp, (unsigned int)&Battle_EventQueue2NoOp16, (unsigned int)&Battle_ClearLimitGauge, (unsigned int)&Battle_EventQueueRebuildAbilityLists, (unsigned int)&Battle_RemovePartyActorsFromBattle, (unsigned int)&Battle_EventPostItemStolenMessage, (unsigned int)&Battle_EventQueueSetDirtyFlags, (unsigned int)&Battle_SetActorTimingFlag40, (unsigned int)&Battle_EventPushHitAnimStep, (unsigned int)&Battle_CopyPartyStatWords};
unsigned char g_rawData_007c2abc[4] = {0xc6, 0x50, 0x43, 0x00};
// clang-format off
// GLOBAL: FF7 0x007c2ac0
unsigned int g_BattleEventCallbackFamilyBases[3] = {(unsigned int)&g_BattleEventQueue0Handlers, (unsigned int)&g_BattleEventQueue1Handlers, (unsigned int)&g_BattleEventQueue2Handlers};
// GLOBAL: FF7 0x009ae280
BattleEventEntry g_BattleEventQueueBuffers[3][128] = {0};
// GLOBAL: FF7 0x009ae880
int g_BattleEventQueueReadIndices[3] = {0};
unsigned char g_bssPad_009ae88c[4] = {0};
// GLOBAL: FF7 0x009ae890
int g_BattleEventQueueWriteIndices[3] = {0};
unsigned char g_bssPad_009ae89c[4] = {0};
// clang-format on


#define g_aBattlePartyStat (g_BattleState.partyStats)

/*
 * Resolves the commanding actor, clears its ATB gauge and clears its bit in
 * actorCommandIssuedMask.
 */
// FUNCTION: FF7 0x00434690
void __cdecl Battle_EventResetActorAtb(int actor, int payload)
{
    actor = Battle_ResolveCommandingActor(actor);
    g_BattleState.actors[actor].atbGauge = 0;
    g_BattleState.actorCommandIssuedMask &= (short)~(1 << actor);
}

/*
 * Forwards the actor to Battle_ReleaseCommandMenu.
 */
// FUNCTION: FF7 0x004346ce
void __cdecl Battle_EventReleaseActorCommandMenu(int actor)
{
    Battle_ReleaseCommandMenu(actor);
}

/*
 * For a party actor (below 3), calls Battle_ClearLimitReady, clears the actor's limit gauge in
 * g_MenuPartyStats and its bit in g_BattleScene.readyActorMask. The comparison does not reject
 * negative indices.
 */
// FUNCTION: FF7 0x004346df
void __cdecl Battle_ResetLimitReadyState(int actor, int payload)
{
    if (actor < 3) {
        Battle_ClearLimitReady(actor);
        g_MenuPartyStats[actor].limitGauge = 0;
        g_BattleScene.readyActorMask &= (short)~(1 << actor);
    }
}

/*
 * Releases the actor's command menu; when its actorCommandPendingMask bit is set, requests the
 * menu again if its ATB gauge reads 0xFFFF, otherwise clears the pending bit.
 */
// FUNCTION: FF7 0x00434723
void __cdecl Battle_EventReopenActorCommandMenu(int actor)
{
    Battle_ReleaseCommandMenu(actor);
    if ((g_BattleState.actorCommandPendingMask & 1 << actor) != 0) {
        if (*(unsigned short *)&g_BattleState.actors[actor].atbGauge == 0xFFFF) {
            Battle_RequestCommandMenu(actor);
        } else {
            g_BattleState.actorCommandPendingMask &= (short)~(1 << actor);
        }
    }
}

/*
 * Calls Battle_EventResetActorAtb, Battle_ResetLimitReadyState, and Battle_ReleaseCommandMenu,
 * then clears the actor's bits in actorCommandPendingMask and actorAutoControlMask. The first two
 * take only the actor; the original call pushes the event payload as well.
 */
// FUNCTION: FF7 0x0043478f
void __cdecl Battle_EventRemoveActorFromTurnOrder(int actor, int payload)
{
    Battle_EventResetActorAtb(actor, payload);
    Battle_ResetLimitReadyState(actor, payload);
    Battle_ReleaseCommandMenu(actor);
    g_BattleState.actorCommandPendingMask &= (short)~(1 << actor);
    g_BattleState.actorAutoControlMask &= (short)~(1 << actor);
}

/*
 * Ignores the actor and records itemId as a pending item drop of slot 0xA (no actor) through
 * Battle_ClaimPendingDropRecord.
 */
// FUNCTION: FF7 0x004347f6
void __cdecl Battle_EventAddPendingItemDrop(int actor, int itemId)
{
    Battle_ClaimPendingDropRecord(0xA, (short)itemId);
}

/*
 * Calls Battle_EventResetActorAtb and Battle_ReleaseCommandMenu, then clears the actor's bit in
 * actorCommandPendingMask.
 */
// FUNCTION: FF7 0x0043480a
void __cdecl Battle_EventCancelActorTurn(int actor, int payload)
{
    Battle_EventResetActorAtb(actor, payload);
    Battle_ReleaseCommandMenu(actor);
    g_BattleState.actorCommandPendingMask &= (short)~(1 << actor);
}

/*
 * Ignores the actor and forwards value to Battle_GetSetAtbFreezeFlag, so a negative value leaves
 * g_BattleAtbFreezeFlag alone and anything else stores it; the returned value is discarded.
 */
// FUNCTION: FF7 0x00434848
void __cdecl Battle_EventSetAtbFreezeFlag(int actor, int value)
{
    Battle_GetSetAtbFreezeFlag(value);
}

/*
 * Releases the actor's command menu and sets its bit in actorAutoControlMask; if its
 * actorCommandPendingMask bit is set, posts its base command with two zero arguments.
 */
// FUNCTION: FF7 0x00434859
void __cdecl Battle_EventEnterAutoControl(int actor)
{
    Battle_ReleaseCommandMenu(actor);
    g_BattleState.actorAutoControlMask |= (short)(1 << actor);
    if ((g_BattleState.actorCommandPendingMask & 1 << actor) != 0) {
        Battle_PostCommand((short)actor, Battle_GetActorBaseCommandId(actor), 0, 0);
    }
}

/*
 * Clears the actor's bit in actorAutoControlMask.
 */
// FUNCTION: FF7 0x004348bc
void __cdecl Battle_EventLeaveAutoControl(int actor)
{
    g_BattleState.actorAutoControlMask &= (short)~(1 << actor);
}

/*
 * When suppress is nonzero removes the actor from turn order and sets its actorMenuSuppressMask
 * bit; otherwise clears that bit and, if its actorCommandPendingMask bit is set, releases its menu
 * and posts command -1.
 */
// FUNCTION: FF7 0x004348de
void __cdecl Battle_EventSetActorMenuSuppressed(int actor, int suppress)
{
    if (suppress != 0) {
        Battle_EventRemoveActorFromTurnOrder(actor, suppress);
        g_BattleState.actorMenuSuppressMask |= (short)(1 << actor);
    } else {
        g_BattleState.actorMenuSuppressMask &= (short)~(1 << actor);
        if ((g_BattleState.actorCommandPendingMask & 1 << actor) != 0) {
            Battle_ReleaseCommandMenu(actor);
            Battle_PostCommand((short)actor, -1, 0, 0);
        }
    }
}

/*
 * Counts valid unblocked entries among 320 inventory slots, selects one using the shared unsigned
 * random word modulo that count, consumes the selected slot, and posts category-one event two for
 * arg1 only when consumption returns something other than 0xFFFF.
 */
// FUNCTION: FF7 0x00434969
void __cdecl Battle_ConsumeRandomInventoryItem(int actor)
{
    int itemCnt;
    int slotIndex;
    int pick;
    unsigned short consumed;

    itemCnt = 0;
    consumed = 0xFFFF;
    for (slotIndex = 0; slotIndex < 320; slotIndex++) {
        if (g_BattleItemListBlock.slots[slotIndex].itemId != 0xFFFF && (g_BattleItemListBlock.slots[slotIndex].flags & 1) == 0) {
            itemCnt++;
        }
    }
    if (itemCnt != 0) {
        pick = (g_BattleState.turnRandomWord) % itemCnt;
        for (slotIndex = 0; slotIndex < 320; slotIndex++) {
            if (g_BattleItemListBlock.slots[slotIndex].itemId != 0xFFFF && (g_BattleItemListBlock.slots[slotIndex].flags & 1) == 0) {
                pick--;
                if (pick < 0) {
                    consumed = Battle_ConsumeInventorySlot(slotIndex);
                    break;
                }
            }
        }
        if (consumed != 0xFFFF) {
            Battle_PostEvent(BATTLE_EVENT_QUEUE_TURN, actor, BATTLE_TURN_EVENT_ITEM_STOLEN, consumed);
        }
    }
}

/*
 * For a nonzero inventory count, decrements the count and returns its item id; when the count
 * becomes zero empties the slot (item 0xFFFF, flags 0xA). Every consumption then sets the block's
 * inventoryChanged byte to 0xFF. Returns -1 for an empty slot.
 */
// FUNCTION: FF7 0x00434a80
short __cdecl Battle_ConsumeInventorySlot(int slot)
{
    short val;

    val = 0xFFFF;
    if (g_BattleItemListBlock.slots[slot].count != 0) {
        (*(char *)&g_BattleItemListBlock.slots[slot].count)--;
        val = g_BattleItemListBlock.slots[slot].itemId;
        if (g_BattleItemListBlock.slots[slot].count == 0) {
            g_BattleItemListBlock.slots[slot].itemId = 0xFFFF;
            g_BattleItemListBlock.slots[slot].flags = 0xA;
        }
        g_BattleItemListBlock.inventoryChanged = 0xFF;
    }
    return (short)val;
}

/*
 * ORs dirtyMask into the actor's dirtyFlags.
 */
// FUNCTION: FF7 0x00434b04
void __cdecl Battle_EventSetActorDirtyFlags(int actor, int dirtyMask)
{
    g_BattleState.actors[actor].dirtyFlags |= dirtyMask;
}

/*
 * Calls Battle_InitActorAbilityRecord(actor, payload), then ORs 9 (bits 0 and 3) into the actor's
 * dirtyFlags.
 */
// FUNCTION: FF7 0x00434b24
void __cdecl Battle_EventRebuildActorAbilityLists(int actor, int payload)
{
    Battle_InitActorAbilityRecord(actor, payload);
    g_BattleState.actors[actor].dirtyFlags |= 9;
}

/*
 * Masks actorAutoControlMask, actorMenuSuppressMask, actorCommandPendingMask and
 * actorCommandIssuedMask with 0xFC0F, which clears the bits of actors 4 to 9.
 */
// FUNCTION: FF7 0x00434b53
void __cdecl Battle_EventClearEnemySlotMenuMasks(void)
{
    g_BattleState.actorAutoControlMask &= 0xFC0F;
    g_BattleState.actorMenuSuppressMask &= 0xFC0F;
    g_BattleState.actorCommandPendingMask &= 0xFC0F;
    g_BattleState.actorCommandIssuedMask &= 0xFC0F;
}

/*
 * For each party actor 0..2 not under auto control whose actorCommandIssuedMask bit is set,
 * clears that bit and sets its actorCommandPendingMask bit.
 */
// FUNCTION: FF7 0x00434b9e
void __cdecl Battle_ReopenPendingCommandMenus(void)
{
    int i;

    for (i = 0; i < 3; i++) {
        if ((*(unsigned short *)&g_BattleState.actorAutoControlMask & 1 << i) == 0 && (g_BattleState.actorCommandIssuedMask & 1 << i) != 0) {
            g_BattleState.actorCommandIssuedMask &= (short)~(1 << i);
            g_BattleState.actorCommandPendingMask |= (short)(1 << i);
        }
    }
}

/*
 * Finds the first matching item ID among 0x140 inventory records, consumes it only when its flag
 * byte AND 9 is zero, and returns after that match regardless of consumption.
 */
// FUNCTION: FF7 0x00434c2a
void __cdecl Battle_ConsumeInventoryItemById(int actor, int itemId)
{
    int i;

    for (i = 0; i < 320; i++) {
        if (g_BattleItemListBlock.slots[i].itemId == itemId) {
            if ((g_BattleItemListBlock.slots[i].flags & 9) == 0) {
                Battle_ConsumeInventorySlot(i);
            }
            return;
        }
    }
}

/*
 * Empty hook; performs no work.
 */
// FUNCTION: FF7 0x00434c86
void __cdecl Battle_EventQueue1NoOp0(void)
{
}

/*
 * Queues kernel battle message 0x26 with the actor's unit index as its word parameter, stores
 * the queue result + 0x100 in messageWindowRequest, and sets limitBanner to 0xF.
 */
// FUNCTION: FF7 0x00434c8b
void __cdecl Battle_EventShowLimitBreakBanner(int actor)
{
    char *msgText;
    short unitWord;
    int queueRes;

    unitWord = g_BattleScene.actors[actor].unitIndex;
    msgText = Kernel_GetBattleMessage(0x26);
    queueRes = Battle_QueueAiMessageText(msgText, (unsigned short *)&unitWord);
    (g_BattleState.messageWindowRequest) = queueRes + 0x100;
    (g_BattleState.limitBanner) = 15;
}

/*
 * Posts event (2, actor, 0x14, itemId) and stores itemId in the combatant's spoilB.
 */
// FUNCTION: FF7 0x00434ce0
void __cdecl Battle_EventQueueItemStolen(int actor, int itemId)
{
    Battle_PostEvent(BATTLE_EVENT_QUEUE_ACTION, actor, BATTLE_ACTION_EVENT_ITEM_STOLEN_MESSAGE, itemId);
    g_aCombatants[actor].spoilB = itemId;
}

/*
 * Reads a bit index from g_BattleStatusTimerSlots[timerSlot] and clears that bit in the
 * combatant's status.
 */
// FUNCTION: FF7 0x00434d0a
void __cdecl Battle_StatusTimerExpireGeneric(int actor, int timerSlot)
{
    int val;

    val = 1 << *(unsigned char *)((char *)&g_BattleStatusTimerSlots + timerSlot);
    g_aCombatants[actor].status &= ~val;
}

/*
 * Calls Battle_EnqueueActionDirect(actor, 3, 2, 54, 0), queueing the fixed action 0x36 for that
 * actor.
 */
// FUNCTION: FF7 0x00434d46
void __cdecl Battle_StatusTimerExpireDeathSentence(int actor)
{
    Battle_EnqueueActionDirect(actor, 3, 2, 54, 0);
}

/*
 * If the combatant has Slow-numb, replaces it with Petrify.
 */
// FUNCTION: FF7 0x00434d5f
void __cdecl Battle_StatusTimerExpireSlowNumb(int actor)
{
    if ((g_aCombatants[actor].status & BATTLE_STATUS_SLOW_NUMB) != 0) {
        g_aCombatants[actor].status &= ~BATTLE_STATUS_SLOW_NUMB;
        g_aCombatants[actor].status |= BATTLE_STATUS_PETRIFY;
    }
}

/*
 * Genuine original group 434DB0..4351BD.
 * If the combatant is poisoned (status bit 3), sets its poisonTimer to 0xA and enqueues action
 * (actor, 3, 0x23, 0, 0).
 */
// FUNCTION: FF7 0x00434db0
void __cdecl Battle_StatusTimerExpirePoison(int actor)
{
    if ((g_aCombatants[actor].status & BATTLE_STATUS_POISON) != 0) {
        g_BattleState.actors[actor].poisonTimer = 10;
        Battle_EnqueueActionDirect(actor, 3, 0x23, 0, 0);
    }
}

/*
 * Empty status-timer hook; performs no work.
 */
// FUNCTION: FF7 0x00434de9
void __cdecl Battle_StatusTimerExpireNoOp(void)
{
}

/*
 * Empty event-queue hook; performs no work.
 */
// FUNCTION: FF7 0x00434dee
void __cdecl Battle_EventQueue2NoOp16(void)
{
}

/*
 * For a party actor (below 3), clears the limit words of its g_aBattlePartyStat record and posts
 * event (0, actor, 1, 0).
 */
// FUNCTION: FF7 0x00434df3
void __cdecl Battle_ClearLimitGauge(int actor)
{
    if (actor < 3) {
        g_aBattlePartyStat[actor].loadLimitGauge = 0;
        g_aBattlePartyStat[actor].limitGauge = 0;
        Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, actor, BATTLE_ATB_EVENT_RESET_LIMIT_READY, 0);
    }
}

/*
 * Queues BATTLE_ATB_EVENT_REBUILD_ABILITY_LISTS (0xD) for the actor with the payload (Battle_PostEvent).
 */
// FUNCTION: FF7 0x00434e2e
void __cdecl Battle_EventQueueRebuildAbilityLists(int actor, int payload)
{
    Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, actor, BATTLE_ATB_EVENT_REBUILD_ABILITY_LISTS, payload);
}

/*
 * For each party actor 0..2 whose bit is set in partyMask, clears its AI actor and target mask
 * bits, invalidates its party, scene and combatant identifiers, clears its status, slip rate and
 * sixteen timer bytes, invalidates matching actor bytes in the 64-entry action queue, refreshes
 * its tick rates, releases its pending drops, and posts event two. The actor argument is unused.
 */
// FUNCTION: FF7 0x00434e47
void __cdecl Battle_RemovePartyActorsFromBattle(int actor, int partyMask)
{
    int byteIndex;
    int partyIdx;
    int memberBit;

    for (partyIdx = 0; partyIdx < 3; partyIdx++) {
        memberBit = 1 << partyIdx;
        if ((partyMask & memberBit) != 0) {
            gBattleAiGlobals.actorMask &= (short)~memberBit;
            *(short *)&gBattleAiGlobals.targetMask &= (short)~memberBit;
            gSavemap.party[partyIdx] = PARTY_CHAR_NONE;
            g_BattleScene.actors[partyIdx].unitIndex = 0xFF;
            g_aCombatants[partyIdx].unitId = 0xFF;
            g_aCombatants[partyIdx].sideSlot = 0xFF;
            g_aCombatants[partyIdx].status = 0;
            g_aCombatants[partyIdx].flags = 0;
            g_BattleState.actors[partyIdx].slipRate = 0;
            for (byteIndex = 0; byteIndex < 0x10; byteIndex++) {
                /* the sixteen timer bytes from statusTimers on, +0x10..+0x1F of the record */
                *((char *)g_BattleState.actors[0].statusTimers + (partyIdx * 0x44 + byteIndex)) = 0;
            }
            for (byteIndex = 0; byteIndex < 0x40; byteIndex++) {
                if (g_BattleState.queuedActions[byteIndex].actorIndex == partyIdx) {
                    g_BattleState.queuedActions[byteIndex].actorIndex = 0xFF;
                }
            }
            Battle_RefreshActorTickRates(partyIdx);
            Battle_ReleasePendingDrops((short)partyIdx);
            Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, partyIdx, BATTLE_ATB_EVENT_REMOVE_FROM_TURN_ORDER, 0);
        }
    }
}

/*
 * Posts battle message 0x53 for the actor with itemId as its one 16-bit parameter.
 */
// FUNCTION: FF7 0x00434fa4
void __cdecl Battle_EventPostItemStolenMessage(int actor, int itemId)
{
    unsigned short val;

    val = itemId;
    Battle_PostBattleMessageFormatted(actor, 0x53, 1, &val);
}

/*
 * Queues BATTLE_ATB_EVENT_SET_DIRTY_FLAGS (0xC) for the actor with the payload (Battle_PostEvent).
 */
// FUNCTION: FF7 0x00434fc8
void __cdecl Battle_EventQueueSetDirtyFlags(int actor, int payload)
{
    Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, actor, BATTLE_ATB_EVENT_SET_DIRTY_FLAGS, payload);
}

/*
 * ORs 0x40 into the actor's timingFlags.
 */
// FUNCTION: FF7 0x00434fe1
void __cdecl Battle_SetActorTimingFlag40(int actor)
{
    *(char *)&g_BattleState.actors[actor].timingFlags |= 0x40;
}

/*
 * Reads the effect id (resolvedAttackId) of attack entry attackIndex and passes it, the actor's
 * status, and constants 0x34, 2, 0, and 9 to Battle_PushAnimStepWithHitRecord.
 */
// FUNCTION: FF7 0x00435001
void __cdecl Battle_EventPushHitAnimStep(int actor, int attackIndex)
{
    int val;

    val = g_BattleAttackTable[attackIndex].resolvedAttackId;
    Battle_PushAnimStepWithHitRecord(actor, 0x34, 2, val, 0, 9, g_aCombatants[actor].status);
}

// For actor indices below 3, sets the max HP and max MP of g_MenuPartyStats[actor]. When
// fromCombatant is nonzero they come from the combatant record in g_aCombatants; otherwise they
// come from g_aBattlePartyStat.
// FUNCTION: FF7 0x0043503f
void __cdecl Battle_CopyPartyStatWords(int actor, int fromCombatant)
{
    if (actor < 3) {
        if (fromCombatant != 0) {
            g_MenuPartyStats[actor].maxHp = (*(short *)&g_aCombatants[actor].maxHp);
            g_MenuPartyStats[actor].maxMp = (*(short *)&g_aCombatants[actor].maxMp);
        } else {
            g_MenuPartyStats[actor].maxHp = g_aBattlePartyStat[actor].maxHp;
            g_MenuPartyStats[actor].maxMp = g_aBattlePartyStat[actor].maxMp;
        }
    }
}

/*
 * Empty hook; performs no work.
 */
// FUNCTION: FF7 0x004350c6
void __cdecl Battle_EventQueue2NoOp25(void)
{
}

/*
 * Resets the three battle event queues. For each queue p it writes 0xFF into the first byte of all
 * 128 entries of g_BattleEventQueueBuffers[p], then zeroes the queue's read and write indices.
 */
// FUNCTION: FF7 0x004350cb
void __cdecl Battle_ResetEventQueues(void)
{
    int queueIndex;
    int entryIndex;

    for (queueIndex = 0; queueIndex < 3; queueIndex++) {
        for (entryIndex = 0; entryIndex < 128; entryIndex++) {
            g_BattleEventQueueBuffers[queueIndex][entryIndex].actor = 0xFF;
        }
        g_BattleEventQueueReadIndices[queueIndex] = 0;
        g_BattleEventQueueWriteIndices[queueIndex] = 0;
    }
}

/*
 * Selects the current four-byte event slot in the requested ring; if its first byte is 0xff,
 * stores payload as a word, code and actor as bytes, then advances that ring cursor modulo 128. A
 * busy slot drops the event.
 */
// FUNCTION: FF7 0x00435139
void __cdecl Battle_PostEvent(int queueIndex, int actor, int opcode, int payload)
{
    BattleEventEntry *entry;
    int writeIndex;

    writeIndex = g_BattleEventQueueWriteIndices[queueIndex];
    entry = &g_BattleEventQueueBuffers[queueIndex][writeIndex];
    if (entry->actor == 0xFF) {
        entry->payload = payload;
        entry->opcode = opcode;
        entry->actor = actor;
        g_BattleEventQueueWriteIndices[queueIndex] = Battle_NextEventSlot(writeIndex);
    }
}

/*
 * Advances a queue index: returns (slot + 1) & 0x7F, wrapping in the 128-entry event ring.
 */
// FUNCTION: FF7 0x004351a3
int __cdecl Battle_NextEventSlot(int slot)
{
    slot++;
    slot &= 0x7F;
    return slot;
}

/*
 * Flushes the selected four-byte event queue until the current record begins with 0xFF. Selects
 * the callback table by queueIndex and the callback by the record's opcode byte, passing the
 * actor byte and the payload word. Marks the actor byte 0xFF and advances the saved queue cursor
 * through Battle_NextEventSlot after each callback.
 */
// FUNCTION: FF7 0x004351bd
void __cdecl Battle_FlushEventQueue(int queueIndex)
{
    unsigned char *p;
    int q;
    void(__cdecl * *r)(int, int);
    /* Each pass reloads the read cursor and the record inside the loop test. Written as a
     * `for (;;)` with a break, VC5 emits different code. */
    while ((q = g_BattleEventQueueReadIndices[queueIndex],
            p = (unsigned char *)&g_BattleEventQueueBuffers[queueIndex][q],
            *p != 0xFF)) {
        r =((void(__cdecl ***)(int, int)) & g_BattleEventCallbackFamilyBases)[queueIndex];
        r[p[1]](p[0], *(unsigned short *)(p + 2));
        *p = 0xFF;
        g_BattleEventQueueReadIndices[queueIndex] = Battle_NextEventSlot(q);
    }
}
} /* extern "C" */
