// src/ff7/src/battle/battle_random_source_helpers.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle random source: returns the next random byte and 16-bit value, rolls a 1-to-100
//   percentage, picks a random target from a mask, and counts set bits.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/battle.h"
#include "ff7/gte.h"
#include "lib/crt.h"

#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "lib/codegen.h"
extern "C" {

unsigned short __cdecl Battle_AiVm_Random16(void);

/*
 * Wrapper returning the next pseudo-random byte for battle calculations.
 */
// FUNCTION: FF7 0x005d1910
unsigned char __cdecl Battle_NextRandomByteWrapper(void)
{
    return Battle_NextRandomByte();
}

/*
 * Generates a 16-bit pseudo-random value for battle AI VM opcode 0x81.
 */
// FUNCTION: FF7 0x005d191a
unsigned short __cdecl Battle_AiVm_Random16(void)
{
    return Battle_NextRandom16();
}

/*
 * Returns random 16 times 99 divided 65535 plus one using signed promoted arithmetic.
 * Inclusive endpoints 1 to 100; distribution not assumed uniform.
 */
// FUNCTION: FF7 0x005d1924
int __cdecl Battle_Roll1To100(void)
{
    /* Signed division on the promoted return value: the original divides signed. */
    return Battle_AiVm_Random16() * 99 / 65535 + 1;
}

/*
 * Scales a value down by a random 4.12 fixed-point factor in the range [0xF01, 0x1000].
 */
// FUNCTION: FF7 0x005d1941
int __cdecl BattleFx_JitterScaleDownUnused(int value)
{
    return (value * (Battle_NextRandomByteWrapper() + 0xf01)) >> 12;
}

/*
 * Counts the number of set bits in a 16-bit integer mask.
 */
// FUNCTION: FF7 0x005d1960
int __cdecl Battle_PopCount16(unsigned short mask)
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
 * For non-empty 16-bit mask selects random-byte modulo population ordinal and walks set bits,
 * returning selected bit. Empty returns zero.
 */
// FUNCTION: FF7 0x005d19a4
unsigned short __cdecl Battle_PickRandomTargetFromMask(unsigned short targetMask)
{
    unsigned short selectedBit;
    int ordinal;

    selectedBit = 0;
    ordinal = Battle_PopCount16((short)targetMask);
    if (ordinal != 0) {
        ordinal = (Battle_NextRandomByteWrapper()) % ordinal;
        for (selectedBit = 1; selectedBit != 0; selectedBit <<= 1) {
            if ((targetMask & selectedBit) != 0) {
                ordinal--;
                if (ordinal < 0) {
                    break;
                }
            }
        }
    }
    return (short)selectedBit;
}
} /* extern "C" */
