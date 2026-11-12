// src/ff7/submarine/sub_bit_count_tables.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Builds the leading-zero and leading-one bit-count lookup tables and a leading sign-bit
//   counter used by the submarine code.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/submarine.h"
#include "ff7/types/submarine.h"
#include <math.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <mmsystem.h>
#include "ff7/gte.h"

extern "C" {

#pragma intrinsic(abs)


#pragma intrinsic(strcpy, strcat)

// The leading-zero count table: 256 ints, read as BitCountTableEntry cells by Bit_CountLeadingSignBits.
struct BitCountTableEntry;

#pragma data_seg(".data$u77_lut")
#pragma bss_seg(".data$u77_lut")
#pragma data_seg()
#pragma bss_seg()

#pragma intrinsic(sqrt)

/*
 * Fills the 256-entry int table g_SubmarineGridBuffer with a count-leading-zeros
 * lookup for a byte: index 0 gets 8, index 1 gets 7, indices 2 and 3 get 6
 * (index 3 is written first and index 2 copied from it), then each doubling
 * run 4-7, 8-15, 16-31, 32-63, 64-127 and 128-255 gets 5, 4, 3, 2, 1 and 0.
 */
// FUNCTION: FF7 0x007a2ca0
void __cdecl Bit_InitLeadingZeroCountTable(void)
{
    int i;

    ((int *)g_SubmarineGridBuffer)[0] = 8;
    ((int *)g_SubmarineGridBuffer)[1] = 7;
    ((int *)g_SubmarineGridBuffer)[3] = 6;
    ((int *)g_SubmarineGridBuffer)[2] = ((int *)g_SubmarineGridBuffer)[3];
    for (i = 4; i < 8; i++) {
        ((int *)g_SubmarineGridBuffer)[i] = 5;
    }
    for (i = 8; i < 0x10; i++) {
        ((int *)g_SubmarineGridBuffer)[i] = 4;
    }
    for (i = 0x10; i < 0x20; i++) {
        ((int *)g_SubmarineGridBuffer)[i] = 3;
    }
    for (i = 0x20; i < 0x40; i++) {
        ((int *)g_SubmarineGridBuffer)[i] = 2;
    }
    for (i = 0x40; i < 0x80; i++) {
        ((int *)g_SubmarineGridBuffer)[i] = 1;
    }
    for (i = 0x80; i < 0x100; i++) {
        ((int *)g_SubmarineGridBuffer)[i] = 0;
    }
}

/*
 * Fills the 256-entry int table at g_SubmarineSceneBuffer with a count-leading-ones
 * lookup for a byte: indices 0-0x7F get 0, 0x80-0xBF get 1, 0xC0-0xDF get 2,
 * 0xE0-0xEF get 3, 0xF0-0xF7 get 4, 0xF8-0xFB get 5, and the last four entries
 * are the separate globals following the table: g_SubmarineLeadingOnesEntry253 (index 0xFD) and
 * g_SubmarineLeadingOnesEntry252 (index 0xFC) get 6, g_SubmarineLeadingOnesEntry254 (0xFE) gets 7 and
 * g_SubmarineLeadingOnesEntry255 (0xFF) gets 8.
 */
// FUNCTION: FF7 0x007a2dc6
void __cdecl Bit_InitLeadingOneCountTable(void)
{
    int i;

    for (i = 0; i < 0x80; i++) {
        ((int *)&g_SubmarineSceneBuffer)[i] = 0;
    }
    for (i = 0x80; i < 0xC0; i++) {
        ((int *)&g_SubmarineSceneBuffer)[i] = 1;
    }
    for (i = 0xC0; i < 0xE0; i++) {
        ((int *)&g_SubmarineSceneBuffer)[i] = 2;
    }
    for (i = 0xE0; i < 0xF0; i++) {
        ((int *)&g_SubmarineSceneBuffer)[i] = 3;
    }
    for (i = 0xF0; i < 0xF8; i++) {
        ((int *)&g_SubmarineSceneBuffer)[i] = 4;
    }
    for (i = 0xF8; i < 0xFC; i++) {
        ((int *)&g_SubmarineSceneBuffer)[i] = 5;
    }
    g_SubmarineLeadingOnesEntry253 = 6;
    g_SubmarineLeadingOnesEntry252 = g_SubmarineLeadingOnesEntry253;
    g_SubmarineLeadingOnesEntry254 = 7;
    g_SubmarineLeadingOnesEntry255 = 8;
}

/*
 * Examines value bytes from most to least significant and sums the byte values
 * stored in the cells of g_SubmarineGridBuffer for positive inputs or g_SubmarineSceneBuffer
 * for zero/negative inputs. Stops after the first table result other than 8,
 * or returns the sum of all four results when each preceding result was 8.
 */
// FUNCTION: FF7 0x007a2ef8
int __cdecl Bit_CountLeadingSignBits(int value)
{
    unsigned char byte;
    int sum;
    unsigned char tableValue;

    sum = 0;
    if (value > 0) {
        byte = value >> 0x18;
        tableValue = g_SubmarineGridBuffer[byte].tableValue;
        sum += tableValue;
        if (tableValue != 8) {
            return sum;
        }
        byte = value << 8 >> 0x18;
        tableValue = g_SubmarineGridBuffer[byte].tableValue;
        sum += tableValue;
        if (tableValue != 8) {
            return sum;
        }
        byte = value << 0x10 >> 0x18;
        tableValue = g_SubmarineGridBuffer[byte].tableValue;
        sum += tableValue;
        if (tableValue != 8) {
            return sum;
        }
        byte = value << 0x18 >> 0x18;
        tableValue = g_SubmarineGridBuffer[byte].tableValue;
        sum += tableValue;
        return sum;
    } else {
        byte = value >> 0x18;
        tableValue = g_SubmarineSceneBuffer[byte].tableValue;
        sum += tableValue;
        if (tableValue != 8) {
            return sum;
        } else {
            byte = value << 8 >> 0x18;
            tableValue = g_SubmarineSceneBuffer[byte].tableValue;
            sum += tableValue;
            if (tableValue != 8) {
                return sum;
            } else {
                byte = value << 0x10 >> 0x18;
                tableValue = g_SubmarineSceneBuffer[byte].tableValue;
                sum += tableValue;
                if (tableValue != 8) {
                    return sum;
                } else {
                    byte = value << 0x18 >> 0x18;
                    tableValue = g_SubmarineSceneBuffer[byte].tableValue;
                    sum += tableValue;
                    return sum;
                }
            }
        }
    }
}
} /* extern "C" */
