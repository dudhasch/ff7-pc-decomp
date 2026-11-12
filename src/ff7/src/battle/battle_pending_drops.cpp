// src/ff7/src/battle/battle_pending_drops.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Manages the battle's pending item drop table: claims, clears and releases drop records
//   per actor, then moves them into the drop ring buffer and collects them into party inventory.
//   Also picks a random throwable weapon from inventory.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/enums/battle.h"
#include "ff7/types/battle_actors.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "lib/codegen.h"
#include "ff7/battle.h"
#include "lib/crt.h"
#include "ff7/api/menu.h"
#include "ff7/types/menu.h"
extern "C" {

// clang-format off
// clang-format on


// clang-format off
// GLOBAL: FF7 0x009ae218
int g_BattleDropRingTail = 0;
// GLOBAL: FF7 0x009ae21c
int g_BattleDropRingHead = 0;
// GLOBAL: FF7 0x009ae220
struct BattlePendingDropRecord g_BattlePendingDropRecords[16] = {0};
// GLOBAL: FF7 0x009ae260
unsigned char g_BattleDropRing[32] = {0};
// clang-format on

/*
 * Resets the 16-entry pending item drop table by initializing both actor and item
 * ID fields of each record to 0xFFFF.
 */
// FUNCTION: FF7 0x00433020
void __cdecl Battle_ResetPendingDrops(void)
{
    int i;

    for (i = 0; i < 16; i++) {
        g_BattlePendingDropRecords[i].actor = 0xFFFF;
        g_BattlePendingDropRecords[i].itemId = 0xFFFF;
    }
}

/*
 * Searches the pending drop table for the record matching (actor, item) and
 * clears it by setting both fields to 0xFFFF.
 */
// FUNCTION: FF7 0x0043305c
void __cdecl Battle_ClearPendingDropRecord(short actor, short item)
{
    int i;

    for (i = 0; i < 16; i++) {
        if (g_BattlePendingDropRecords[i].actor == actor && g_BattlePendingDropRecords[i].itemId == item) {
            g_BattlePendingDropRecords[i].actor = 0xFFFF;
            g_BattlePendingDropRecords[i].itemId = 0xFFFF;
            return;
        }
    }
}

/*
 * Allocates the first free slot in the pending drop table and records the given
 * (actor, item) pair.
 */
// FUNCTION: FF7 0x004330c0
void __cdecl Battle_ClaimPendingDropRecord(short actor, short item)
{
    int i;

    for (i = 0; i < 16; i++) {
        if (g_BattlePendingDropRecords[i].itemId == -1) {
            g_BattlePendingDropRecords[i].actor = actor;
            g_BattlePendingDropRecords[i].itemId = item;
            return;
        }
    }
}

/*
 * Transfers all active pending drop records for the given actor into the drop ring
 * buffer and frees their slots.
 */
// FUNCTION: FF7 0x00433112
void __cdecl Battle_ReleasePendingDrops(short actor)
{
    int idx;
    short item;

    for (idx = 0; idx < 16; idx++) {
        item = g_BattlePendingDropRecords[idx].itemId;
        if (item != -1 && g_BattlePendingDropRecords[idx].actor == actor) {
            ((short *)&g_BattleDropRing)[g_BattleDropRingHead] = item;
            g_BattleDropRingHead++;
            g_BattleDropRingHead &= 0xF;
            g_BattlePendingDropRecords[idx].actor = 0xFFFF;
            g_BattlePendingDropRecords[idx].itemId = 0xFFFF;
        }
    }
}

/*
 * Drains the circular drop ring buffer and adds collected items to party inventory,
 * incrementing quantity if already present or inserting into an empty inventory slot.
 */
// FUNCTION: FF7 0x004331ac
void __cdecl Battle_CollectDropsToInventory(void)
{
    short itemId;
    int invSlot;
    BattleInventoryItemRecord *pRecord;
    int rowCount;
    int emptySlot;
    BattleDropEntry *pNewEntry;

    while (g_BattleDropRingHead != g_BattleDropRingTail) {
        itemId = ((short *)&g_BattleDropRing)[g_BattleDropRingTail];
        g_BattleDropRingTail++;
        emptySlot = 0xFFFFFFFF;
        if (itemId < 0x180) {
            rowCount = 0;
            for (invSlot = 0; invSlot < 320; invSlot++) {
                pRecord = (BattleInventoryItemRecord *)(invSlot * 6 + (char *)g_BattleItemListBlock.slots);
                if (pRecord->itemId == itemId) {
                    if (pRecord->quantity < 99) {
                        (*(char *)&pRecord->quantity)++;
                    }
                    break;
                }
                if (pRecord->itemId == 0xFFFF && emptySlot == -1) {
                    emptySlot = invSlot;
                }
            }
            if (invSlot == 0x140 && emptySlot != -1) {
                pNewEntry = (BattleDropEntry *)(emptySlot * 6 + (char *)g_BattleItemListBlock.slots);
                pNewEntry->itemId = itemId;
                pNewEntry->count = 1;
                pNewEntry->dropType = 3;
                if (itemId < 0x80) {
                    pNewEntry->dropType = *(char *)&g_MenuKernelItemBuffer[itemId].targetFlags;
                    pNewEntry->dropFlags = *(char *)&g_MenuKernelItemBuffer[itemId].equipRestrictions;
                } else if (itemId < 0x100) {
                    pNewEntry->dropType = g_KernelWeaponDataArray[(itemId - 0x80)].typeByte;
                    pNewEntry->dropFlags = *(char *)&g_KernelWeaponDataArray[(itemId - 0x80)].flagWord;
                } else if (itemId < 0x120) {
                    pNewEntry->dropFlags = *(char *)&g_KernelArmorDataArray[(itemId - 0x100)].flagWord;
                } else if (itemId < 0x140) {
                    pNewEntry->dropFlags = *(char *)((char *)&g_KernelAccessoryDataArray[0].restrictions + (itemId - 0x120 << 4));
                }
                pNewEntry->dropFlags &= 0xB;
                rowCount = (emptySlot + 1) / 2;
            }
            g_BattleItemListBlock.inventoryChanged = 0xFF;
            if (g_BattleItemListBlock.itemRowCount < rowCount) {
                g_BattleItemListBlock.itemRowCount = rowCount;
            }
        }
        g_BattleDropRingTail &= 0xF;
    }
}

/*
 * Collects throwable items from inventory, selects one at random, and if it is a
 * weapon, posts event 0x10 and returns its item ID.
 */
// FUNCTION: FF7 0x004333c4
short __cdecl Battle_PickRandomThrowableWeapon(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[668];
        int pickIndex;
        struct { unsigned char pad_to_4[0x4]; int pickedItem; };
        struct { unsigned char pad_to_8[0x8]; int entryItemId; };
        struct { unsigned char pad_to_C[0xC]; int entryFlags; };
        struct { unsigned char pad_to_10[0x10]; int slotIndex; };
        struct { unsigned char pad_to_14[0x14]; int throwableCount; };
        struct { unsigned char pad_to_18[0x18]; short throwableIds[320]; };
        struct { unsigned char pad_to_298[0x298]; short result; };
    } frame;
#pragma pack(pop)

    frame.result = 0xFFFF;
    frame.throwableCount = 0;
    for (frame.slotIndex = 0; frame.slotIndex < 320; frame.slotIndex++) {
        frame.entryItemId = *(unsigned short *)((char *)g_BattleItemListBlock.slots + frame.slotIndex * 6);
        frame.entryFlags = *(unsigned char *)((char *)&g_BattleItemListBlock.slots[0].flags + frame.slotIndex * 6);
        if (frame.entryItemId != 0xFFFF && (frame.entryFlags & 9) == 0) {
            frame.throwableIds[frame.throwableCount] = *(short *)&frame.entryItemId;
            frame.throwableCount++;
        }
    }
    if (frame.throwableCount != 0) {
        frame.pickIndex = Battle_RandBelow(frame.throwableCount) & 0xFF;
        *(short *)&frame.pickedItem = frame.throwableIds[frame.pickIndex];
        if ((frame.pickedItem & 0xFFFF) >= 128 && (frame.pickedItem & 0xFFFF) < 256) {
            Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, 0, BATTLE_ATB_EVENT_CONSUME_ITEM, frame.pickedItem & 0xFFFF);
            frame.result = *(short *)&frame.pickedItem;
        }
    }
    return (short)frame.result;
}
} /* extern "C" */
