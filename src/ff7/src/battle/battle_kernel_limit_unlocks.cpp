// src/ff7/src/battle/battle_kernel_limit_unlocks.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Updates the kill and limit-use counters that unlock limit breaks after a battle,
//   capping them at their maximum values.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/savemap.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
extern "C" {

#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/*
 * Adds killDelta to the kill count and useDelta to the use count of the current limit level (both
 * capped at 0xFFFF above only), then sets the usableLimits bits whose kill or use thresholds in
 * the kernel data are now met (0xFFFF means no threshold). The level is the low two bits of
 * limitLevel - 1; level 3 has no use count. Returns the usableLimits bits that changed.
 */
// FUNCTION: FF7 0x005c7090
int __cdecl Kernel_UpdateLimitUnlocks(SavemapCharacter *charRecord, int killDelta, int useDelta)
{
    int killSum;
    int unlockMask;
    int characterId;
    int useCnt;
    int reqCnt;
    int bitValue;
    int killIdx;
    int thresholdIdx;
    unsigned short *thresholdPointer;
    int laneBit;
    int usesRemaining;
    int idx;
    unsigned short previousLimits;

    characterId = charRecord->id;
    idx = charRecord->limitLevel - 1 & 3;
    killSum = charRecord->killCount;
    unlockMask = charRecord->limits;
    previousLimits = unlockMask;
    killSum += killDelta;
    if (killSum > 0xFFFF) {
        killSum = 0xFFFF;
    }
    if (characterId < 9) {
        bitValue = 8;
        for (killIdx = 0; killIdx < 2; killIdx++) {
            if ((unlockMask & bitValue) == 0) {
                reqCnt = g_BattleKernelData.chars[characterId].limitLevelKills[killIdx];
                if (reqCnt != 0xFFFF && reqCnt <= killSum) {
                    unlockMask |= bitValue;
                }
            }
            bitValue <<= 3;
        }
    }
    if (idx < 3) {
        useCnt = charRecord->limitUses[idx];
        useCnt += useDelta;
        if (useCnt > 0xFFFF) {
            useCnt = 0xFFFF;
        }
        if (characterId < 9) {
            thresholdPointer = g_BattleKernelData.chars[characterId].limitUnlockUses[idx];
            laneBit = 2;
            for (thresholdIdx = 0; thresholdIdx < idx; thresholdIdx++) {
                laneBit <<= 3;
            }
            for (thresholdIdx = 0; thresholdIdx < 2; thresholdIdx++) {
                if ((unlockMask & laneBit) == 0) {
                    usesRemaining = thresholdPointer[thresholdIdx];
                    if (usesRemaining != 0xFFFF && usesRemaining <= useCnt) {
                        unlockMask |= laneBit;
                    }
                }
                laneBit <<= 1;
            }
        }
        charRecord->limitUses[idx] = useCnt;
    }
    charRecord->killCount = killSum;
    charRecord->limits = unlockMask;
    return previousLimits ^ unlockMask;
}
} /* extern "C" */
