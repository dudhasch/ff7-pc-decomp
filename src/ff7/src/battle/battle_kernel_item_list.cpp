// src/ff7/src/battle/battle_kernel_item_list.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Builds the battle item list by expanding the packed inventory into fixed-size entries.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "ff7/battle.h"
#include "ff7/gte.h"
#include "ff7/savemap.h"
#include "ff7/types/battle.h"
#include "lib/codegen.h"
#include "lib/api/graphics.h"
#include "lib/crt.h"
#include "ff7/types/battle_actors.h"
#include "ff7/types/menu.h"
extern "C" {


/*
 * Expands all 320 packed inventory shorts into six-byte entries: sentinel preserved with zero
 * quantity/type and mask 0xb; others decode nine-bit ID/seven-bit quantity and category table
 * mask/type. Tracks last nonsentinel index rather than occupied count and sets half-rounded-up
 * list limit minimum three; sixth byte untouched.
 */
// FUNCTION: FF7 0x005d1520
void __cdecl Kernel_BuildBattleItemList(void)
{
    int count;
    int i;
    int limit;
    int mask;
    BattleInventoryItemRecord *pt;
    int num;
    int id;
    int cur;

    count = 0;
    for (i = 0; i < 0x140; i++) {
        pt = (BattleInventoryItemRecord *)(i * 6 + (char *)g_BattleItemListBlock.slots);
        id = gSavemap.items[i];
        num = 0;
        cur = 0;
        mask = 0xB;
        if (id != 0xFFFF) {
            num = (id >> 9) & 0x7F;
            id &= 0x1FF;
            if (id < 0x80) {
                mask = g_MenuKernelItemBuffer[id].equipRestrictions & 0xB;
                cur = g_MenuKernelItemBuffer[id].targetFlags;
            } else if (id < 0x100) {
                mask = g_KernelWeaponDataArray[(id - 0x80)].flagWord & 0xB;
                cur = *(unsigned char *)((int)(*(unsigned char(*)[5632]) & g_KernelWeaponDataArray) + (id - 0x80) * 0x2C);
            } else if (id < 0x120) {
                mask = g_KernelArmorDataArray[(id - 0x100)].flagWord & 0xB;
                cur = 3;
            } else if (id < 0x140) {
                mask = g_KernelAccessoryDataArray[(id - 0x120)].restrictions & 0xB;
                cur = 3;
            }
            count = i + 1;
        }
        pt->itemId = id;
        pt->quantity = num;
        pt->current = cur;
        pt->mask = mask;
    }
    limit = (count + 1) / 2;
    if (limit < 3) {
        limit = 3;
    }
    g_BattleItemListBlock.itemListLimit = limit;
}
} /* extern "C" */
