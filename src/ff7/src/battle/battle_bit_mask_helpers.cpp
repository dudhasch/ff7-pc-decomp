// src/ff7/src/battle/battle_bit_mask_helpers.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Small battle helpers: a dword copy of scene data, counting and picking set bits in
//   actor masks, and clamping HP/MP deltas to a maximum value.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
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
 * Copies byteCount / 4 32-bit words from src to dst, ascending, with no overlap check.
 */
// FUNCTION: FF7 0x005c8000
void __cdecl Battle_CopySceneDataBlock(void *dst, void *src, int byteCount)
{
    int index;

    byteCount >>= 2;
    for (index = 0; index < byteCount; index++) {
        ((int *)dst)[index] = ((int *)src)[index];
    }
}

/*
 * Returns the index of the highest set bit of value (0 for both 0 and 1).
 */
// FUNCTION: FF7 0x005c8041
int __cdecl Battle_HighestSetBitIndex(int value)
{
    int index;

    index = 0;
    for (index = 0; 1 != 0; index++) {
        value = (unsigned int)value >> 1;
        if (value == 0) {
            break;
        }
    }
    return index;
}

/*
 * Returns the number of set bits in mask.
 */
// FUNCTION: FF7 0x005c8080
int __cdecl Battle_PopCountMask(unsigned int mask)
{
    int count;

    count = 0;
    while (mask != 0) {
        if ((mask & 1) != 0) {
            count++;
        }
        mask >>= 1;
    }
    return count;
}

/*
 * Picks a random set bit of mask and returns it as a single-bit mask; an empty mask returns zero.
 */
// FUNCTION: FF7 0x005c80b5
int __cdecl Battle_PickRandomBitFromMask(unsigned int mask)
{
    int bitMask = 0;
    int ordinal = Battle_PopCountMask(mask);
    if (ordinal != 0) {
        ordinal = Battle_NextRandomByte() * ordinal >> 8;
        for (bitMask = 1; bitMask != 0; bitMask <<= 1) {
            if ((mask & bitMask) != 0) {
                ordinal--;
                if (ordinal < 0) {
                    break;
                }
            }
        }
    }
    return bitMask;
}

/*
 * Adds delta to value (isAdd) or subtracts it, clamping to maxValue on overflow or above it, or to
 * zero on underflow. The wrap checks compare unsigned.
 */
// FUNCTION: FF7 0x005c8126
int __cdecl Battle_ClampHpMpDelta(int value, int delta, int maxValue, int isAdd)
{
    int original;

    original = value;
    if (isAdd != 0) {
        value += delta;
        if ((unsigned int)value < (unsigned int)original) {
            value = maxValue;
        }
        if ((unsigned int)value > (unsigned int)maxValue) {
            value = maxValue;
        }
    } else {
        value -= delta;
        // BUG: a negative delta, such as an overflowed damage, raises value above original, and
        // this unsigned test reads that as an underflow: HP or MP drops to 0 instead of rising.
        if ((unsigned int)value > (unsigned int)original) {
            value = 0;
        }
    }
    return value;
}
} /* extern "C" */
