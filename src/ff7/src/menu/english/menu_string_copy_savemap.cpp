// src/ff7/src/menu/english/menu_string_copy_savemap.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Limited string copying for the menu, the save preview header, the save checksum, and
//   small savemap record accessors used by the save file code.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/menu.h"
#include <stdlib.h>
#include "lib/crt.h"
#include <math.h>
#include <string.h>
#include "ff7/api/menu.h"
#include "lib/pfile.h"
#include "ff7/savemap.h"
#include "ff7/enums/main.h"

#pragma intrinsic(abs)
#pragma intrinsic(memset)
extern "C" {

#pragma intrinsic(abs)
#pragma intrinsic(cos)
#pragma intrinsic(sin)
#pragma intrinsic(sqrt, atan2)

// clang-format off
// GLOBAL: FF7 0x00dd7740
int g_SavemapRecordsTable = 0;
unsigned char g_bssPad_00dd7744[4] = {0};
unsigned char g_bssPad_00dd7748[1252] = {0};
// GLOBAL: FF7 0x00dd7c2c
int g_Ff7StrCopyLimit = 0;
// clang-format on

/*
 * Copies string up to configured byte limit or terminating 0xFF byte.
 */
// FUNCTION: FF7 0x007228c0
void __cdecl Menu_CopyEncodedTextLimited(int dst, int src)
{
    int i;

    for (i = 0; i < g_Ff7StrCopyLimit; i++) {
        *(char *)dst = *(char *)src;
        if (*(unsigned char *)src == 0xFF) {
            return;
        }
        dst++;
        src++;
    }
}

/*
 * Builds savemap preview header including character levels and statistics.
 */
// FUNCTION: FF7 0x00722913
void __cdecl Savemap_BuildPreviewHeader(void)
{
    int slot;
    int charId;

    for (slot = 0; slot < 3; slot++) {
        gSavemap.descParty[slot] = *(char *)&gSavemap.party[slot];
    }
    Menu_SetEncodedTextCopyLimit(16);
    for (slot = 0; slot < 3; slot++) {
        charId = gSavemap.party[slot];
        if (charId != PARTY_CHAR_NONE) {
            Menu_CopyEncodedTextLimited((int)gSavemap.descName, (int)&gSavemap.chars[charId].name);
            gSavemap.descLevel = *(char *)&gSavemap.chars[charId].level;
            gSavemap.descHp = g_MenuPartyStats[slot].hp;
            gSavemap.descMaxHp = g_MenuPartyStats[slot].maxHp;
            gSavemap.descMp = g_MenuPartyStats[slot].mp;
            gSavemap.descMaxMp = g_MenuPartyStats[slot].maxMp;
            break;
        }
    }
    for (slot = 0; slot < 12; slot++) {
        if (slot > 2 && slot < 6) {
            gSavemap.windowColors[slot + 3] = *(char *)((char *)&g_FieldWindowColours + slot);
        } else if (slot > 5 && slot < 9) {
            gSavemap.descLocation[slot + 29] = *(char *)((char *)&g_FieldWindowColours + slot);
        } else {
            gSavemap.windowColors[slot] = *(char *)((char *)&g_FieldWindowColours + slot);
        }
    }
    gSavemap.descGil = gSavemap.gil;
    gSavemap.descTime = *(int *)&gSavemap.seconds;
    Menu_SetEncodedTextCopyLimit(24);
    Menu_CopyEncodedTextLimited((int)gSavemap.descLocation, (int)gSavemap.location);
}

/*
 * Sets maximum copy length for string copying routines.
 */
// FUNCTION: FF7 0x00722abd
void __cdecl Menu_SetEncodedTextCopyLimit(int limit)
{
    g_Ff7StrCopyLimit = limit;
}

/*
 * Calculates 16-bit CRC checksum over save data buffer.
 */
// FUNCTION: FF7 0x00722aca
int __cdecl Menu_SaveChecksumCrc16(int length, void *data)
{
    unsigned short crc;
    unsigned short i;
    unsigned short bit;

    crc = 0xFFFF;
    for (i = 0; i < (length & 0xFFFF); i++) {
        crc ^= (short)(*(unsigned char *)((char *)data + i) << 8);
        for (bit = 0; bit < 8; bit++) {
            if ((crc & 0x8000) != 0) {
                crc = crc << 1 ^ 0x1021;
            } else {
                crc <<= 1;
            }
        }
    }
    return ~crc & 0xFFFF;
}

/*
 * Stub routine returning zero.
 */
// FUNCTION: FF7 0x00722b8d
short __cdecl Savemap_ReturnZero_1(void)
{
    return 0;
}

/*
 * Returns pointer to indexed save record in table.
 */
// FUNCTION: FF7 0x00722b95
int __cdecl Savemap_RecordAt(int index)
{
    return index * 0x54 + (int)&g_SavemapRecordsTable;
}

/*
 * Stub routine returning zero.
 */
// FUNCTION: FF7 0x00722ba5
short __cdecl Savemap_ReturnZero_2(void)
{
    return 0;
}
} /* extern "C" */
