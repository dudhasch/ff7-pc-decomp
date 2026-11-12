// src/ff7/src/menu/btlmenu/english/text_char_tables.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Builds the ASCII-to-game-text encoding tables from the character order table and its
//   reverse, and provides the rebuild and encode routines that use them.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <string.h>
#include <stdlib.h>
#include "ff7/types/menu.h"
#include "ff7/api/menu.h"
#include "lib/api/graphics.h"
extern "C" {

#pragma intrinsic(abs, memcpy, memset)

// clang-format off
// g_TextReverseCharCodeTable maps a character code to its position in g_TextCharCodeTable.
// GLOBAL: FF7 0x00dc3d18
unsigned char g_TextReverseCharCodeTable[32] = {0};
// GLOBAL: FF7 0x00dc3d38
char g_TextCharCodeCount = 0;
unsigned char g_bssPad_00dc3d3c[4] = {0};
unsigned char g_bssPad_00dc3d40[216] = {0};
// g_TextCharCodeTable holds the game-text code for each of the 225 positions in the character order.
// GLOBAL: FF7 0x00dc3e18
int g_TextCharCodeTable[225] = {0};
unsigned char g_bssPad_00dc419c[4] = {0};
unsigned char g_bssPad_00dc41a0[120] = {0};
// clang-format on

/*
 * Fills the 225-entry character order table g_TextCharCodeTable: positions 0..0x5F hold the ASCII
 * codes 0x20..0x7F, followed by the accented and special characters, each mapped to its FF7 glyph
 * code (0x20 where there is none).
 */
// FUNCTION: FF7 0x006fdca0
void __cdecl Text_BuildCharCodeTable(void)
{
    int i;

    for (i = 0; i < 96; i++) {
        g_TextCharCodeTable[i] = i + 0x20;
    }
    g_TextCharCodeTable[96] = 0xC4;
    g_TextCharCodeTable[97] = 0xC5;
    g_TextCharCodeTable[98] = 0xC7;
    g_TextCharCodeTable[99] = 0xC9;
    g_TextCharCodeTable[100] = 0xD1;
    g_TextCharCodeTable[101] = 0xD6;
    g_TextCharCodeTable[102] = 0xDC;
    g_TextCharCodeTable[103] = 0xE1;
    g_TextCharCodeTable[104] = 0xE0;
    g_TextCharCodeTable[105] = 0xE2;
    g_TextCharCodeTable[106] = 0xE4;
    g_TextCharCodeTable[107] = 0xE3;
    g_TextCharCodeTable[108] = 0xE5;
    g_TextCharCodeTable[109] = 0xE7;
    g_TextCharCodeTable[110] = 0xE9;
    g_TextCharCodeTable[111] = 0xE8;
    g_TextCharCodeTable[112] = 0xEA;
    g_TextCharCodeTable[113] = 0xEB;
    g_TextCharCodeTable[114] = 0xED;
    g_TextCharCodeTable[115] = 0xEC;
    g_TextCharCodeTable[116] = 0xEE;
    g_TextCharCodeTable[117] = 0xEF;
    g_TextCharCodeTable[118] = 0xF1;
    g_TextCharCodeTable[119] = 0xF3;
    g_TextCharCodeTable[120] = 0xF2;
    g_TextCharCodeTable[121] = 0xF4;
    g_TextCharCodeTable[122] = 0xF6;
    g_TextCharCodeTable[123] = 0xF5;
    g_TextCharCodeTable[124] = 0xFA;
    g_TextCharCodeTable[125] = 0xF9;
    g_TextCharCodeTable[126] = 0xFB;
    g_TextCharCodeTable[127] = 0xFC;
    g_TextCharCodeTable[128] = 0x20;
    g_TextCharCodeTable[129] = 0xB0;
    g_TextCharCodeTable[130] = 0xA2;
    g_TextCharCodeTable[131] = 0xA3;
    g_TextCharCodeTable[132] = 0x20;
    g_TextCharCodeTable[133] = 0x20;
    g_TextCharCodeTable[134] = 0x20;
    g_TextCharCodeTable[135] = 0xDF;
    g_TextCharCodeTable[136] = 0x20;
    g_TextCharCodeTable[137] = 0x20;
    g_TextCharCodeTable[138] = 0x20;
    g_TextCharCodeTable[139] = 0xB4;
    g_TextCharCodeTable[140] = 0xA8;
    g_TextCharCodeTable[141] = 0x20;
    g_TextCharCodeTable[142] = 0xC6;
    g_TextCharCodeTable[143] = 0xD8;
    g_TextCharCodeTable[144] = 0x20;
    g_TextCharCodeTable[145] = 0xB1;
    g_TextCharCodeTable[146] = 0x20;
    g_TextCharCodeTable[147] = 0x20;
    g_TextCharCodeTable[148] = 0xA5;
    g_TextCharCodeTable[149] = 0xB5;
    g_TextCharCodeTable[150] = 0x20;
    g_TextCharCodeTable[151] = 0x20;
    g_TextCharCodeTable[152] = 0x20;
    g_TextCharCodeTable[153] = 0x20;
    g_TextCharCodeTable[154] = 0x20;
    g_TextCharCodeTable[155] = 0xAA;
    g_TextCharCodeTable[156] = 0xBA;
    g_TextCharCodeTable[157] = 0x20;
    g_TextCharCodeTable[158] = 0xE6;
    g_TextCharCodeTable[159] = 0xF8;
    g_TextCharCodeTable[160] = 0xBF;
    g_TextCharCodeTable[161] = 0xA1;
    g_TextCharCodeTable[162] = 0xAC;
    g_TextCharCodeTable[163] = 0x20;
    g_TextCharCodeTable[164] = 0x83;
    g_TextCharCodeTable[165] = 0x20;
    g_TextCharCodeTable[166] = 0x20;
    g_TextCharCodeTable[167] = 0xAB;
    g_TextCharCodeTable[168] = 0xBB;
    g_TextCharCodeTable[169] = 0x85;
    g_TextCharCodeTable[170] = 0x20;
    g_TextCharCodeTable[171] = 0xC0;
    g_TextCharCodeTable[172] = 0xC3;
    g_TextCharCodeTable[173] = 0xD5;
    g_TextCharCodeTable[174] = 0x8C;
    g_TextCharCodeTable[175] = 0x9C;
    g_TextCharCodeTable[176] = 0x96;
    g_TextCharCodeTable[177] = 0x97;
    g_TextCharCodeTable[178] = 0x93;
    g_TextCharCodeTable[179] = 0x94;
    g_TextCharCodeTable[180] = 0x91;
    g_TextCharCodeTable[181] = 0x92;
    g_TextCharCodeTable[182] = 0xF7;
    g_TextCharCodeTable[183] = 0x20;
    g_TextCharCodeTable[184] = 0xFF;
    g_TextCharCodeTable[185] = 0x9F;
    g_TextCharCodeTable[186] = 0x20;
    g_TextCharCodeTable[187] = 0x20;
    g_TextCharCodeTable[188] = 0x8B;
    g_TextCharCodeTable[189] = 0x9B;
    g_TextCharCodeTable[190] = 0x20;
    g_TextCharCodeTable[191] = 0x20;
    g_TextCharCodeTable[192] = 0x20;
    g_TextCharCodeTable[193] = 0x95;
    g_TextCharCodeTable[194] = 0x82;
    g_TextCharCodeTable[195] = 0x84;
    g_TextCharCodeTable[196] = 0x89;
    g_TextCharCodeTable[197] = 0xC5;
    g_TextCharCodeTable[198] = 0xCA;
    g_TextCharCodeTable[199] = 0xC1;
    g_TextCharCodeTable[200] = 0xCB;
    g_TextCharCodeTable[201] = 0xC8;
    g_TextCharCodeTable[202] = 0xCD;
    g_TextCharCodeTable[203] = 0xCE;
    g_TextCharCodeTable[204] = 0xCF;
    g_TextCharCodeTable[205] = 0xCC;
    g_TextCharCodeTable[206] = 0xD3;
    g_TextCharCodeTable[207] = 0xD4;
    g_TextCharCodeTable[208] = 0x20;
    g_TextCharCodeTable[209] = 0xD2;
    g_TextCharCodeTable[210] = 0xDA;
    g_TextCharCodeTable[211] = 0xDB;
    g_TextCharCodeTable[212] = 0xD9;
    g_TextCharCodeTable[213] = 0x20;
    g_TextCharCodeTable[214] = 0x88;
    g_TextCharCodeTable[215] = 0x98;
    g_TextCharCodeTable[216] = 0x20;
    g_TextCharCodeTable[217] = 0x20;
    g_TextCharCodeTable[218] = 0x20;
    g_TextCharCodeTable[219] = 0x20;
    g_TextCharCodeTable[220] = 0xB8;
    g_TextCharCodeTable[221] = 0x20;
    g_TextCharCodeTable[222] = 0x20;
    g_TextCharCodeTable[223] = 0x20;
    g_TextCharCodeTable[224] = 0x20;
}

/*
 * Builds the inverse of g_TextCharCodeTable into g_TextReverseCharCodeTable and resets
 * g_TextCharCodeCount.
 */
// FUNCTION: FF7 0x006fe1dc
void __cdecl Text_BuildCharCodeReverseTable(void)
{
    int i;

    for (i = 0; i < 225; i++) {
        g_TextReverseCharCodeTable[g_TextCharCodeTable[i]] = i;
    }
    g_TextCharCodeCount = 0;
}

/*
 * Rebuilds g_TextCharCodeTable and its inverse.
 */
// FUNCTION: FF7 0x006fe21b
void __cdecl Text_RebuildAsciiEncodeTables(void)
{
    Text_BuildCharCodeTable();
    Text_BuildCharCodeReverseTable();
}

/*
 * Encodes an ASCII string into FF7 text through g_TextReverseCharCodeTable, ending the output with
 * 0xFF.
 */
// FUNCTION: FF7 0x006fe22a
void __cdecl Text_EncodeAsciiWithReverseTable(unsigned char *dst, char *src)
{
    while (*src != 0) {
        *dst = g_TextReverseCharCodeTable[(unsigned char)*src];
        dst++;
        src++;
    }
    *dst = 0xFF;
}

/*
 * Rebuilds the encoding tables and then encodes an ASCII string into FF7 text,
 * ending with 0xFF.
 */
// FUNCTION: FF7 0x006fe265
void __cdecl Text_EncodeAsciiStringRebuildingTables(unsigned char *dst, char *src)
{
    Text_BuildCharCodeTable();
    Text_BuildCharCodeReverseTable();
    while (*src != 0) {
        *dst = g_TextReverseCharCodeTable[(unsigned char)*src];
        dst++;
        src++;
    }
    *dst = 0xFF;
}
} /* extern "C" */
