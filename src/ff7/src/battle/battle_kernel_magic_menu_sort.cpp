// src/ff7/src/battle/battle_kernel_magic_menu_sort.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Sorts the magic menu entries into the order chosen by the save options, grouping them
//   through a group map.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/battle.h"
#include "ff7/types/battle.h"
#include "ff7/api/battle.h"
#include "ff7/gte.h"
#include "lib/crt.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/codegen.h"
#include "ff7/savemap.h"
#include "ff7/enums/battle.h"
#include "ff7/types/menu.h"
extern "C" {

// GLOBAL: FF7 0x007b7790
extern const unsigned char g_KernelMagicGroupPermutations[6][4] = {{0x00, 0x01, 0x02, 0x03}, {0x00, 0x02, 0x01, 0x03}, {0x02, 0x00, 0x01, 0x03}, {0x01, 0x00, 0x02, 0x03}, {0x01, 0x02, 0x00, 0x03}, {0x02, 0x01, 0x00, 0x03}};

/*
 * Reorders the party member's 56 magic menu entries (BattleAbilitySlot) using a save-option-selected group mapping.
 * Copies all records, maps each nonempty ID to its last original index, clears only destination
 * IDs, constructs four 24-cell groups for the first 54 IDs, filters absent spells, and emits each
 * nonempty three-cell group while reserving all three destination positions. Empty positions
 * preserve their other bytes. Finally scans 72 entries and zeroes MP for effective IDs 25 and 71,
 * adding 56 for entries beyond the sorted region.
 */
// FUNCTION: FF7 0x005dd700
void __cdecl Kernel_SortMagicMenuOrder(int partySlot)
{
    struct MagicEntry {
        unsigned char attackId, b1, b2, b3, mpCost, b5, b6, b7;
    };
    int idx;
    unsigned char grp[4][24];
    unsigned char live[56];
    int entryIdx;
    int inner;
    struct MagicEntry *src;
    struct MagicEntry prev[56];
    unsigned char index[56];
    unsigned char map;
    int code;
    int laneIndex8;
    unsigned char *cell;
    unsigned char cur;
    unsigned char id;
    int bit;

    src = (struct MagicEntry *)g_MenuPartyStats[partySlot].spells;
    code = (gSavemap.options >> 10) & 7;
    for (entryIdx = 0; entryIdx < 56; entryIdx++) {
        prev[entryIdx] = src[entryIdx];
        if (src[entryIdx].attackId != 0xff) {
            index[src[entryIdx].attackId] = (unsigned char)entryIdx;
        }
        src[entryIdx].attackId = 0xff;
    }
    for (entryIdx = 0; entryIdx < 4; entryIdx++) {
        for (inner = 0; inner < 24; inner++) {
            grp[entryIdx][inner] = 0xff;
        }
    }
    for (entryIdx = 0; entryIdx < 54; entryIdx++) {
        map = g_BattleKernelData.magicOrder[entryIdx];
        grp[((unsigned char(*)[4])((int)&g_KernelMagicGroupPermutations))[code][map >> 5]][map & 0x1f] = (unsigned char)entryIdx;
    }
    for (entryIdx = 0; entryIdx < 56; entryIdx++) {
        live[entryIdx] = 0xff;
    }
    for (entryIdx = 0; entryIdx < 56; entryIdx++) {
        id = prev[entryIdx].attackId;
        if (id != 0xff) {
            live[id] = id;
        }
    }
    for (entryIdx = 0; entryIdx < 4; entryIdx++) {
        for (inner = 0; inner < 24; inner++) {
            cur = grp[entryIdx][inner];
            if (cur != 0xff) {
                grp[entryIdx][inner] = live[cur];
            }
        }
    }
    idx = 0;
    for (entryIdx = 0; entryIdx < 4; entryIdx++) {
        for (inner = 0; inner < 24; inner += 3) {
            cell = &grp[entryIdx][inner];
            if ((cell[0] & cell[1] & cell[2]) != 0xff) {
                for (laneIndex8 = 0; laneIndex8 < 3; laneIndex8++) {
                    if (cell[laneIndex8] != 0xff) {
                        src[idx] = prev[index[cell[laneIndex8]]];
                    }
                    idx++;
                }
            }
        }
    }
    for (entryIdx = 0; entryIdx < 72; entryIdx++) {
        bit = src[entryIdx].attackId;
        if (bit != 0xff) {
            if (entryIdx >= 56) {
                bit += 0x38;
            }
            switch (bit) {
            case KERNEL_MENU_ABILITY_ESCAPE:
            case KERNEL_MENU_ABILITY_SUMMON_0F:
                src[entryIdx].mpCost = 0;
                break;
            }
        }
    }
}
} /* extern "C" */
