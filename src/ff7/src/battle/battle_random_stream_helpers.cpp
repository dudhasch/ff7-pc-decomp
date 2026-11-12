// src/ff7/src/battle/battle_random_stream_helpers.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle random number helpers: a cursor advances through the battle RNG streams to yield
//   random bytes, bounded values and 16-bit values. A helper also spreads scene flag bits across
//   the stream cursor bytes.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include <stdlib.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/codegen.h"
extern "C" {

#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00c06738
int g_BattleRngStreamIndex = 0;
unsigned char g_bssPad_00c0673c[4] = {0};
// GLOBAL: FF7 0x00c06740
unsigned char g_BattleRngStreamCursors[8] = {0};
// GLOBAL: FF7 0x00c06748
int g_BattleRngCallCount = 0;
unsigned char g_bssPad_00c0674c[4] = {0};
// clang-format on

/*
 * Increments the counter g_BattleRngStreamIndex and masks it to three bits, so it cycles 0 through 7.
 */
// FUNCTION: FF7 0x005c8b80
void __cdecl Battle_AdvanceActionSequenceCounter(void)
{
    g_BattleRngStreamIndex++;
    g_BattleRngStreamIndex &= 7;
}

/*
 * Uses g_BattleRngStreamIndex to select an unsigned byte cursor in g_BattleRngStreamCursors, reads
 * the corresponding byte from g_BattleKernelData.randomTable, increments that cursor byte, and
 * returns the fetched byte.
 */
// FUNCTION: FF7 0x005c8ba1
unsigned char __cdecl Battle_NextRandomByte(void)
{
    unsigned char val;

    val = g_BattleKernelData.randomTable[g_BattleRngStreamCursors[g_BattleRngStreamIndex]];
    g_BattleRngStreamCursors[g_BattleRngStreamIndex]++;
    return (char)val;
}

/*
 * Returns Battle_NextRandomByte()*limit shifted right by eight.
 */
// FUNCTION: FF7 0x005c8bdc
int __cdecl Battle_RandBelow(int limit)
{
    return Battle_NextRandomByte() * limit >> 8;
}

/*
 * Reads a random byte as the low result byte, postincrements g_BattleRngCallCount and advances the
 * stream index when its prior low three bits are nonzero, reads a second random byte as the
 * high result byte and returns their combination.
 */
// FUNCTION: FF7 0x005c8bf2
unsigned short __cdecl Battle_NextRandom16(void)
{
    int lowByte;
    int highByte;

    *(unsigned char *)&lowByte = Battle_NextRandomByte();
    if ((g_BattleRngCallCount++ & 7) != 0) {
        Battle_AdvanceActionSequenceCounter();
    }
    *(unsigned char *)&highByte = Battle_NextRandomByte();
    return (highByte & 0xFF) << 8 | (lowByte & 0xFF);
}

/*
 * Spreads the low bits of sceneFlags across the eight stream cursors, LSB first: each byte gets
 * the low 8 bits of the running value before it is shifted right by one. Afterwards it zeroes the
 * counter g_BattleRngStreamIndex.
 */
// FUNCTION: FF7 0x005c8c42
void __cdecl Battle_UnpackSceneFlagsToBytes(int sceneFlags)
{
    int index;

    for (index = 0; index < 8; index++) {
        g_BattleRngStreamCursors[index] = sceneFlags;
        sceneFlags >>= 1;
    }
    g_BattleRngStreamIndex = 0;
}
} /* extern "C" */
