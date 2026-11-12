// src/ff7/src/battle/battle_init_exp_results.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Sets up and updates the experience result rows shown after a battle, awards the reward
//   items to the inventory, and looks up each character's limit attack.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/types/battle.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/codegen.h"
#include "ff7/savemap.h"
#include "ff7/api/menu.h"
#include "ff7/enums/battle.h"
extern "C" {






#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// The party's shown experience (three ints, two of which other TUs read by name as g_BattleArenaRoundIndex and
// g_BattleArenaSavedRoundHp; this TU indexes all three through g_BattleExpShownByPartySlot), then the
// target experience per party slot.
unsigned int g_BattleExpShownByPartySlot[1] = {0};
int g_BattleArenaRoundIndex = 0;
int g_BattleArenaSavedRoundHp = 0;
unsigned char g_bssPad_00c0664c[4] = {0};
unsigned int g_BattleExpTargetByPartySlot[3] = {0};
unsigned char g_bssPad_00c0665c[4] = {0};
// GLOBAL: FF7 0x00c06660
unsigned int g_BattlePartyGrowthStats[3][12] = {0};
unsigned char g_bssPad_00c066f0[48] = {0};
// GLOBAL: FF7 0x00c06720
unsigned char g_BattlePartyLevelUpFlags[3] = {0};
// GLOBAL: FF7 0x00c06724
unsigned char g_BattlePartyInitialLevels[3] = {0};
// clang-format off
// GLOBAL: FF7 0x00c06728
int g_BattleExpPartyMask = 0;
// GLOBAL: FF7 0x00c0672c
int g_BattleExpAnimFrameCount = 0;
// GLOBAL: FF7 0x00c06730
int g_BattleExpCompletedMask = 0;
unsigned char g_bssPad_00c06734[4] = {0};
// clang-format on

/*
 * Starts the experience result rows: stores partyMask and clears the animation frame count and the
 * completion mask. Always returns 0.
 */
// FUNCTION: FF7 0x005c8180
int __cdecl Battle_InitExpResultRows(int partyMask)
{
    g_BattleExpAnimFrameCount = 0;
    g_BattleExpCompletedMask = 0;
    g_BattleExpPartyMask = partyMask;
    return 0;
}

/*
 * Updates three experience-result rows, marking empty party slots complete and locating matching
 * saved character records. Selects the active threshold interval, advances displayed experience
 * after the first invocation using an accumulated fixed-point increment, clamps at a threshold,
 * and marks completion at the final target. Uses saved row values for completed or level-99 cases,
 * otherwise publishes displayed level, experience, remaining amount, and a 61-unit progress byte.
 * An additional mask forces displayed level zero. Increments the invocation count and returns
 * whether the completion mask differs from exactly seven. The loop reads threshold element 11
 * before testing its upper limit; table bounds remain a prerequisite.
 */
// FUNCTION: FF7 0x005c81a3
int __cdecl Battle_UpdateExpResultRows(int forceComplete)
{
    int partyIndex;
    int unused;
    int recordIndex;
    int tierIndex;
    unsigned int lowerExp;
    unsigned int threshold;
    unsigned int increment;
    int id;
    unused = 0;
    if (forceComplete != 0)
        g_BattleExpCompletedMask |= BATTLE_ACTOR_MASK_PARTY;
    for (partyIndex = 0; partyIndex < 3; partyIndex++) {
        id = gSavemap.party[partyIndex];
        if (id == 0xff) {
            g_BattleExpCompletedMask |= 1 << partyIndex;
            continue;
        }
        for (recordIndex = 0; recordIndex < 9; recordIndex++) {
            if (gSavemap.chars[recordIndex].id == id) {
                lowerExp = g_BattlePartyGrowthStats[partyIndex][0];
                for (tierIndex = 1; threshold = g_BattlePartyGrowthStats[partyIndex][tierIndex], tierIndex < 11; tierIndex++) {
                    if (g_BattleExpShownByPartySlot[partyIndex] < threshold)
                        break;
                    lowerExp = threshold;
                }
                if (tierIndex == 11)
                    g_BattleExpShownByPartySlot[partyIndex] = g_BattleExpTargetByPartySlot[partyIndex];
                if (g_BattleExpAnimFrameCount != 0) {
                    if (g_BattleExpTargetByPartySlot[partyIndex] > g_BattleExpShownByPartySlot[partyIndex]) {
                        if (g_BattleExpShownByPartySlot[partyIndex] < threshold) {
                            increment = g_BattlePartyLevelUpFlags[partyIndex];
                            increment += threshold - lowerExp;
                            g_BattleExpShownByPartySlot[partyIndex] += increment >> 8;
                            g_BattlePartyLevelUpFlags[partyIndex] = increment;
                        }
                        if (g_BattleExpShownByPartySlot[partyIndex] >= threshold) {
                            g_BattleExpShownByPartySlot[partyIndex] = threshold;
                            g_BattlePartyLevelUpFlags[partyIndex] = 0;
                        }
                    } else {
                        g_BattleExpCompletedMask |= 1 << partyIndex;
                        g_BattleExpShownByPartySlot[partyIndex] = g_BattleExpTargetByPartySlot[partyIndex];
                    }
                }
                if ((g_BattleExpCompletedMask & (1 << partyIndex)) != 0 || g_BattlePartyInitialLevels[partyIndex] + tierIndex - 1 >= 99) {
                    g_BattleRewardTotals.party[partyIndex].displayedLevel = gSavemap.chars[recordIndex].level;
                    g_BattleRewardTotals.party[partyIndex].displayedExp = gSavemap.chars[recordIndex].exp;
                    g_BattleRewardTotals.party[partyIndex].remainingExp = gSavemap.chars[recordIndex].expToNext;
                    g_BattleRewardTotals.party[partyIndex].expBar = *(unsigned char *)&gSavemap.chars[recordIndex].levelProgress;
                } else {
                    g_BattleRewardTotals.party[partyIndex].displayedLevel = g_BattlePartyInitialLevels[partyIndex] + tierIndex - 1;
                    g_BattleRewardTotals.party[partyIndex].displayedExp = g_BattleExpShownByPartySlot[partyIndex];
                    g_BattleRewardTotals.party[partyIndex].remainingExp = threshold - g_BattleExpShownByPartySlot[partyIndex];
                    g_BattleRewardTotals.party[partyIndex].expBar = 0;
                    if ((threshold - lowerExp) != 0) {
                        g_BattleRewardTotals.party[partyIndex].expBar = (g_BattleExpShownByPartySlot[partyIndex] - lowerExp) * 61 / (threshold - lowerExp);
                    }
                }
                break;
            }
        }
        if ((g_BattleExpPartyMask & (1 << partyIndex)) != 0)
            g_BattleRewardTotals.party[partyIndex].displayedLevel = 0;
    }
    g_BattleExpAnimFrameCount++;
    return g_BattleExpCompletedMask != BATTLE_ACTOR_MASK_PARTY;
}

/*
 * Processes four enabled rewards. Item IDs below 384 merge first matching packed inventory
 * quantity with upper 99 cap or use first empty slot without cap. Higher IDs fill materia empty
 * slots until decremented quantity becomes zero; zero/negative starting quantity can fill all
 * empties. Rewards not cleared.
 */
// FUNCTION: FF7 0x005c84ed
void __cdecl Battle_AddRewardItemsToInventory(void)
{
    int slot;
    short itemId;
    int invIdx;
    int quantity;
    int emptySlot;

    for (slot = 0; slot < 4; slot++) {
        if (g_BattleRewardItems[slot].selected != 0) {
            itemId = g_BattleRewardItems[slot].itemId;
            if (itemId != -1) {
                emptySlot = 0xFFFFFFFF;
                quantity = g_BattleRewardItems[slot].count;
                if (itemId < 0x180) {
                    for (invIdx = 0; invIdx < 320; invIdx++) {
                        if ((gSavemap.items[invIdx] & 0x1FF) == itemId) {
                            quantity += gSavemap.items[invIdx] >> 9;
                            if (quantity > 99) {
                                quantity = 99;
                            }
                            gSavemap.items[invIdx] = itemId | quantity << 9;
                            break;
                        }
                        if (gSavemap.items[invIdx] == 0xFFFF && emptySlot == -1) {
                            emptySlot = invIdx;
                        }
                    }
                    if (invIdx == 320 && emptySlot != -1) {
                        gSavemap.items[emptySlot] = itemId | quantity << 9;
                    }
                } else {
                    for (invIdx = 0; invIdx < 256; invIdx++) {
                        if (gSavemap.materia[invIdx] == -1) {
                            gSavemap.materia[invIdx] = itemId - 0x180;
                            quantity--;
                            if (quantity == 0) {
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

/*
 * For charIndex in zero through eight, reads an unsigned byte from
 * g_BattleKernelData.chars[charIndex].limitAttackIds[limitSlot / 3][limitSlot % 3] and subtracts 0x80; otherwise
 * returns 0x7F. No limitSlot bound is checked.
 */
// FUNCTION: FF7 0x005c8684
int __cdecl Kernel_GetLimitAttackId(int charIndex, int limitSlot)
{
    int attackId;

    attackId = 0x7F;
    if (charIndex >= 0 && charIndex < 9) {
        attackId = g_BattleKernelData.chars[charIndex].limitAttackIds[limitSlot / 3][limitSlot % 3] - 0x80;
    }
    return attackId;
}
} /* extern "C" */
