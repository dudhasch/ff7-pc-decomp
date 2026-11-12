// src/ff7/src/menu/english/menu_savemap_materia_queries.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Savemap helpers for materia: counting equipped materia, checking whether a materia is
//   held or mastered, the master-materia exchange (its three sets complete, traded for the master
//   materia) and removing a mastered materia, along with unaligned 16-bit savemap access and the
//   highway chase's HP scaling of the first five characters. It also holds a menu stub screen
//   whose frame draws a command window and a line of text.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <string.h>
#include "ff7/types/menu.h"
#include "lib/enums/file.h"
#include "ff7/enums/menu.h"
#include <stdlib.h>
#include "lib/api/file.h"
#include "ff7/api/menu.h"
#include "ff7/savemap.h"
extern "C" {

struct EnglishWindowFrame;
#pragma intrinsic(abs, memcpy, memset)

// clang-format off
// GLOBAL: FF7 0x0091f038
int g_MenuStub12_ActiveCursor = (-1);
// The screen's two windows (2x; the same frames as the Config screen's g_MenuConfigFrames2x):
// the title bar and the body, whose y places the cursor.
// GLOBAL: FF7 0x0091f040
EnglishWindowFrame g_MenuStub12_WindowRects[2] = {{0, 0, 640, 51}, {0, 51, 640, 429}};
// The screen's title, which Menu_StubScreen12Frame draws at (16, 11): six glyph bytes and the
// 0xFF end. In the English glyph encoding they read `s`, `a` circumflex, `7`, `A` ring,
// inverted `!`, `a` circumflex, so the shipped title is not English text. The four ints 4, 3, 2, 1 and the 0xFF fill after it
// are not read under this name.
// GLOBAL: FF7 0x0091f050
unsigned char g_MenuStub12_Title[0x28] = {
    0x53, 0x69, 0x17, 0x61, 0xa1, 0x69, 0xff, 0, 4, 0, 0, 0,
    3, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff
};
// The exchange's three sets (Savemap_CheckMateriaSetComplete, Savemap_ExchangeForMasterMateria): the
// magic materia traded for MATERIA_MASTER_MAGIC, the summons for MATERIA_MASTER_SUMMON, the command
// materia for MATERIA_MASTER_COMMAND; then the pair whose possession SAVEMAP_MATERIA_SET_BAHAMUT_ZERO
// checks before it adds MATERIA_BAHAMUT_ZERO (the twelve bytes after the pair are not read under
// this name).
// GLOBAL: FF7 0x0091f078
char g_MasterMagicMateriaIds[0x15] = {49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 64, 65, 68, 69, 70, 71, 72};
// GLOBAL: FF7 0x0091f090
char g_MasterSummonMateriaIds[0x10] = {74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89};
// GLOBAL: FF7 0x0091f0a0
char g_MasterCommandMateriaIds[7] = {36, 37, 39, 40, 41, 42, 43};
// GLOBAL: FF7 0x0091f0a8
char g_BahamutPairMateriaIds[0x10] = {MATERIA_BAHAMUT, MATERIA_NEO_BAHAMUT, 0, 0, 0, 0, 0, 0, 11, 1, (-49), 0, 107, 0, 67, 0};
// The screen's two cursors; g_MenuStub12_ActiveCursor picks the one MenuCursor_Update runs.
// GLOBAL: FF7 0x00dc4218
MenuCursor g_MenuStub12_Cursors[2] = {0};
unsigned char g_bssPad_00dc4288[10304] = {0};
// clang-format on

/*
 * Initializes two one-column, three-row cursors with total row counts of three and
 * nine respectively, enabling wrapping only for the first, then clears
 * g_MenuStub12_ActiveCursor. Its eight local dwords are unused.
 */
// FUNCTION: FF7 0x006fe2b0
void __cdecl Menu_StubScreen12Init(int arg)
{
    int junk[8];

    MenuCursor_Init(&g_MenuStub12_Cursors[0], 0, 0, 1, 3, 0, 0, 1, 3, 0, 0, 0, 1, 0,
                    0);
    MenuCursor_Init(&g_MenuStub12_Cursors[1], 0, 0, 1, 3, 0, 0, 1, 9, 0, 0, 0, 0, 0,
                    0);
    g_MenuStub12_ActiveCursor = 0;
}

/*
 * Screen 12's per-frame tick: draw the command window, the scroll arrow, one line
 * of text and two frames, then run the cursor and leave on cancel. Two one-case
 * switches on g_MenuStub12_ActiveCursor, not two `if`s: each spills the
 * value to its own frame slot and compares that, which only the switch lowering
 * does.
 */
// FUNCTION: FF7 0x006fe316
void __cdecl Menu_StubScreen12Frame(unsigned int frameCounter)
{
    int p;
    int q;
    int r;
    int s;
    int t;
    int x;
    int y;
    int i;
    int z;
    int j;
    int k;
    int n;

    Menu_DrawCommandWindow2x(g_MenuModuleId);
    /* Unused stub menu cursor/state index (0). */
    switch (g_MenuStub12_ActiveCursor) {
    case 0:
        Menu_DrawMoreTextArrow2x(0, g_MenuStub12_WindowRects[1].y + (g_MenuStub12_Cursors[0].row << 6) + 0x20,
                            0.0f);
        break;
    }
    Menu_DrawEncodedString2x(0x10, 0xb, g_MenuStub12_Title, 7, 0.0f);
    for (y = 0; y < 2; y++) {
        Menu_DrawWindowFrame2x(&g_MenuStub12_WindowRects[y], 0.0f);
    }
    MenuCursor_Update(&g_MenuStub12_Cursors[g_MenuStub12_ActiveCursor]);
    /* Unused stub menu cursor/state index (0). */
    switch (g_MenuStub12_ActiveCursor) {
    case 0:
        Input_GetPressed(INPUT_CIRCLE);
        break;
    }
    if (BtlMenu_GetInputRepeat(INPUT_CROSS) != 0) {
        Menu_SetPhase(MENU_PHASE_CLOSE_SCREEN, MENU_SCREEN_MAIN);
        Menu_SetScreen(MENU_SCREEN_MAIN);
    }
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006fe3f9
void Menu_UnusedMateriaQueryNoOp(void)
{
    /* The original is empty. Its whole body is
       `55 8b ec 5d c3` -- 5 bytes at 0x006fe3f9,
       push/mov/pop/ret plus alignment padding.
       The C body is empty, as in retail. */
}

/*
 * How many of a character sixteen materia slots are filled -- eight in the weapon
 * and eight in the armour, -1 being empty. The two loops share the counter and
 * only the first initialises it, which is why the first sets both variables and
 * the second sets one.
 */
// FUNCTION: FF7 0x006fe3fe
int __cdecl Savemap_CountCharEquippedMateria(int charIdx)
{
    int slot;
    int count;

    for (count = slot = 0; slot < 8; slot++) {
        if (gSavemap.chars[charIdx].weaponMateria[slot] !=
            -1) {
            count++;
        }
    }
    for (slot = 0; slot < 8; slot++) {
        if (gSavemap.chars[charIdx].armorMateria[slot] !=
            -1) {
            count++;
        }
    }
    return count;
}

/*
 * Reads a little-endian 16-bit value from two bytes, without needing alignment.
 */
// FUNCTION: FF7 0x006fe483
int __cdecl Savemap_ReadUnalignedU16(unsigned char *p)
{
    return p[0] | (p[1] << 8);
}

/*
 * Writes a 16-bit value as two little-endian bytes; the store twin of Savemap_ReadUnalignedU16.
 */
// FUNCTION: FF7 0x006fe49e
void __cdecl Savemap_WriteUnalignedU16(unsigned char *p, short value)
{
    p[0] = value & 0xff;
    p[1] = value >> 8;
}

/*
 * Scale the first five characters HP by a per-character fraction over 0xFFFF,
 * never below 1. The scaled result is stored back into the character's 16-bit hp.
 */
// FUNCTION: FF7 0x006fe4be
void __cdecl Savemap_ScaleFirstFiveHp(void)
{
    int spare;
    int hp;
    int i;
    int srcPad;

    for (i = 0; i < 5; i++) {
        hp = gSavemap.chars[i].hp *
             Savemap_ReadUnalignedU16((unsigned char *)&gSavemap.highwayHpFractions[i]) / 0xffff;
        if (hp < 1) {
            hp = 1;
        }
        gSavemap.chars[i].hp = hp;
    }
}

/*
 * Clears gSavemap.materiaSetComplete, then checks that every materia of the selected set is held
 * mastered (the 21 magic materia, the 16 summons or the 7 command materia), or for
 * SAVEMAP_MATERIA_SET_BAHAMUT_ZERO that Bahamut and Neo Bahamut are both held. Any failed lookup
 * returns with the flag zero; otherwise sets it to 1. Field request FIELD_REQUEST_CHECK_MATERIA_SET calls it.
 */
// FUNCTION: FF7 0x006fe53a
void __cdecl Savemap_CheckMateriaSetComplete(int setIndex)
{
    int i;

    gSavemap.materiaSetComplete = 0;
    switch (setIndex) {
    case SAVEMAP_MATERIA_SET_MAGIC:
        for (i = 0; i < 0x15; i++) {
            if (Savemap_HasMasteredMateriaId(g_MasterMagicMateriaIds[i]) == 0) {
                return;
            }
        }
        break;
    case SAVEMAP_MATERIA_SET_SUMMON:
        for (i = 0; i < 0x10; i++) {
            if (Savemap_HasMasteredMateriaId(g_MasterSummonMateriaIds[i]) == 0) {
                return;
            }
        }
        break;
    case SAVEMAP_MATERIA_SET_COMMAND:
        for (i = 0; i < 7; i++) {
            if (Savemap_HasMasteredMateriaId(g_MasterCommandMateriaIds[i]) == 0) {
                return;
            }
        }
        break;
    case SAVEMAP_MATERIA_SET_BAHAMUT_ZERO:
        for (i = 0; i < 2; i++) {
            if (Savemap_HasMateriaId(g_BahamutPairMateriaIds[i]) == 0) {
                return;
            }
        }
        break;
    }
    gSavemap.materiaSetComplete = 1;
}

/*
 * Searches weapon then armour slots for each visible character, followed by 200
 * stock entries, for an entry with all 24 upper AP bits set and a low id byte
 * equal to the complete materiaId argument. Returns 1 on first hit or zero if
 * absent.
 */
// FUNCTION: FF7 0x006fe654
int __cdecl Savemap_HasMasteredMateriaId(unsigned int materiaId)
{
    int apbits;
    int charIdx;
    int slotIdx;
    int id;

    for (charIdx = 0; charIdx < 9; charIdx++) {
        if ((gSavemap.phsVisibilityMask & (1 << charIdx)) == 0) {
            continue;
        }
        for (slotIdx = 0; slotIdx < 8; slotIdx++) {
            apbits = (unsigned int)gSavemap.chars[charIdx].weaponMateria[slotIdx] >>
                     8;
            id = (unsigned int)gSavemap.chars[charIdx].weaponMateria[slotIdx] &
                 0xff;
            if (apbits == 0xffffff && id == materiaId) {
                return 1;
            }
        }
        for (slotIdx = 0; slotIdx < 8; slotIdx++) {
            apbits = (unsigned int)gSavemap.chars[charIdx].armorMateria[slotIdx] >>
                     8;
            id = (unsigned int)gSavemap.chars[charIdx].armorMateria[slotIdx] &
                 0xff;
            if (apbits == 0xffffff && id == materiaId) {
                return 1;
            }
        }
    }
    for (slotIdx = 0; slotIdx < 0xc8; slotIdx++) {
        apbits = (unsigned int)gSavemap.materia[slotIdx] >> 8;
        id = gSavemap.materia[slotIdx] & 0xff;
        if (apbits == 0xffffff && id == materiaId) {
            return 1;
        }
    }
    return 0;
}

/*
 * Returns 1 on the first low-byte materia id equal to the complete materiaId
 * argument, searching weapon then armour slots of each visible character and then
 * all 200 stock entries. Otherwise returns zero.
 */
// FUNCTION: FF7 0x006fe7c5
int __cdecl Savemap_HasMateriaId(unsigned int materiaId)
{
    int characterIdx;
    int slot;
    int id;

    for (characterIdx = 0; characterIdx < 9; characterIdx++) {
        if ((gSavemap.phsVisibilityMask & (1 << characterIdx)) == 0) {
            continue;
        }
        for (slot = 0; slot < 8; slot++) {
            id = gSavemap.chars[characterIdx].weaponMateria[slot] & 0xff;
            if (id == materiaId) {
                return 1;
            }
        }
        for (slot = 0; slot < 8; slot++) {
            id = gSavemap.chars[characterIdx].armorMateria[slot] & 0xff;
            if (id == materiaId) {
                return 1;
            }
        }
    }
    for (slot = 0; slot < 0xc8; slot++) {
        id = gSavemap.materia[slot] & 0xff;
        if (id == materiaId) {
            return 1;
        }
    }
    return 0;
}

/*
 * Exchange a set's mastered materia for its master materia: every mastered materia of the
 * magic, summon or command set for its master materia, or (SAVEMAP_MATERIA_SET_BAHAMUT_ZERO,
 * which takes nothing) MATERIA_BAHAMUT_ZERO.
 * Field request FIELD_REQUEST_REPLACE_MATERIA_GROUP calls it. Four dense cases
 * (a jump table).
 */
// FUNCTION: FF7 0x006fe8da
void __cdecl Savemap_ExchangeForMasterMateria(int groupId)
{
    int i;

    switch (groupId) {
    case SAVEMAP_MATERIA_SET_MAGIC:
        for (i = 0; i < 0x15; i++) {
            Savemap_RemoveMasteredMateria(g_MasterMagicMateriaIds[i]);
        }
        Savemap_AddMateria(MATERIA_MASTER_MAGIC);
        break;
    case SAVEMAP_MATERIA_SET_SUMMON:
        for (i = 0; i < 0x10; i++) {
            Savemap_RemoveMasteredMateria(g_MasterSummonMateriaIds[i]);
        }
        Savemap_AddMateria(MATERIA_MASTER_SUMMON);
        break;
    case SAVEMAP_MATERIA_SET_COMMAND:
        for (i = 0; i < 7; i++) {
            Savemap_RemoveMasteredMateria(g_MasterCommandMateriaIds[i]);
        }
        Savemap_AddMateria(MATERIA_MASTER_COMMAND);
        break;
    case SAVEMAP_MATERIA_SET_BAHAMUT_ZERO:
        Savemap_AddMateria(MATERIA_BAHAMUT_ZERO);
        break;
    }
}

/*
 * Find one mastered copy of a materia and take it out, wherever it is: a slot matches when its
 * word is `materiaId | 0xFFFFFF00`, the id with all 24 AP bits set (as
 * Savemap_HasMasteredMateriaId tests). The search order is every visible character's weapon
 * slots, then their armour slots, then the two-hundred-entry stock.
 */
// FUNCTION: FF7 0x006fe9c3
int __cdecl Savemap_RemoveMasteredMateria(unsigned int materiaId)
{
    int characterIdx;
    int slot;

    materiaId |= 0xffffff00;
    for (characterIdx = 0; characterIdx < 9; characterIdx++) {
        if ((gSavemap.phsVisibilityMask & (1 << characterIdx)) == 0) {
            continue;
        }
        for (slot = 0; slot < 8; slot++) {
            if (gSavemap.chars[characterIdx].weaponMateria[slot] == materiaId) {
                gSavemap.chars[characterIdx].weaponMateria[slot] = -1;
                return 0;
            }
        }
        for (slot = 0; slot < 8; slot++) {
            if (gSavemap.chars[characterIdx].armorMateria[slot] == materiaId) {
                gSavemap.chars[characterIdx].armorMateria[slot] = -1;
                return 0;
            }
        }
    }
    for (slot = 0; slot < 0xc8; slot++) {
        if (gSavemap.materia[slot] == materiaId) {
            gSavemap.materia[slot] = -1;
            return 0;
        }
    }
    return 0;
}
} /* extern "C" */
