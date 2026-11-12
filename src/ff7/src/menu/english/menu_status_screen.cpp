// src/ff7/src/menu/english/menu_status_screen.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The status menu screen: its setup and per-frame input, the character portraits,
//   elemental resistance page, equipment and materia panels, stat rows, experience panel and name
//   grid, in both scales.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/enums/battle.h"
#include "ff7/types/menu.h"
#include "lib/enums/file.h"
#include "lib/enums/sound.h"
#include <string.h>
#include <stdlib.h>
#include "ff7/api/menu.h"
#include "lib/api/file.h"
#include "lib/api/sound.h"
#include "ff7/savemap.h"
#include "ff7/enums/menu.h"
#include "lib/api/graphics.h"
#include "ff7/enums/main.h"
extern "C" {

struct EnglishWindowFrame;

#pragma intrinsic(abs, memcpy, memset)

// clang-format off
// Text: "Fire" "Ice" "Lightning" "Earth" "Poison" "Gravity" "Water" "Wind" "Holy"
// GLOBAL: FF7 0x00920540
unsigned char g_MenuStatusElementNames[96] = {
    0x26, 0x49, 0x52, 0x45, 0xff, 0, 0, 0, 0, 0, 0x29, 0x43,
    0x45, 0xff, 0, 0, 0, 0, 0, 0, 0x2c, 0x49, 0x47, 0x48,
    0x54, 0x4e, 0x49, 0x4e, 0x47, 0xff, 0x25, 0x41, 0x52, 0x54, 0x48, 0xff,
    0, 0, 0, 0, 0x30, 0x4f, 0x49, 0x53, 0x4f, 0x4e, 0xff, 0,
    0, 0, 0x27, 0x52, 0x41, 0x56, 0x49, 0x54, 0x59, 0xff, 0, 0,
    0x37, 0x41, 0x54, 0x45, 0x52, 0xff, 0, 0, 0, 0, 0x37, 0x49,
    0x4e, 0x44, 0xff, 0, 0, 0, 0, 0, 0x28, 0x4f, 0x4c, 0x59,
    0xff
};
// Text: "Death" "Near-death" "Sleep" "Poison" "Sadness" "Fury" "Confusion" "Silence" "Haste" "Slow"
//       "Stop" "Frog" "Small" "Slow-numb" "Petrify" "Regen" "Barrier" "MBarrier" "Reflect" "Shield"
//       "Death-sentence" "Manipulate" "Berserk" "Peerless" "Paralyzed" "Darkness"
// GLOBAL: FF7 0x009205a0
unsigned char g_MenuStatusConditionNames[544] = {
    0x24, 0x45, 0x41, 0x54, 0x48, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2e, 0x45, 0x41, 0x52,
    0x0d, 0x44, 0x45, 0x41, 0x54, 0x48, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x4c, 0x45, 0x45, 0x50, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x4f, 0x49, 0x53, 0x4f, 0x4e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x41, 0x44, 0x4e, 0x45, 0x53, 0x53, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x26, 0x55, 0x52, 0x59, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x23, 0x4f, 0x4e, 0x46, 0x55, 0x53, 0x49, 0x4f, 0x4e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x49, 0x4c, 0x45,
    0x4e, 0x43, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x41, 0x53, 0x54, 0x45, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x4c, 0x4f, 0x57, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x54, 0x4f, 0x50, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x26, 0x52, 0x4f, 0x47, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x33, 0x4d, 0x41, 0x4c, 0x4c, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x4c, 0x4f, 0x57,
    0x0d, 0x4e, 0x55, 0x4d, 0x42, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x45, 0x54, 0x52, 0x49, 0x46, 0x59, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x45, 0x47, 0x45, 0x4e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x41, 0x52, 0x52, 0x49, 0x45, 0x52, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x2d, 0x22, 0x41, 0x52, 0x52, 0x49, 0x45, 0x52, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x32, 0x45, 0x46, 0x4c, 0x45, 0x43, 0x54, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x48, 0x49, 0x45, 0x4c, 0x44, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x45, 0x41, 0x54, 0x48, 0x0d, 0x53, 0x45, 0x4e, 0x54, 0x45, 0x4e,
    0x43, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2d, 0x41, 0x4e, 0x49, 0x50, 0x55, 0x4c, 0x41, 0x54, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x22, 0x45, 0x52, 0x53, 0x45, 0x52, 0x4b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x30, 0x45, 0x45, 0x52, 0x4c, 0x45, 0x53, 0x53, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x41, 0x52, 0x41,
    0x4c, 0x59, 0x5a, 0x45, 0x44, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x41, 0x52, 0x4b, 0x4e, 0x45, 0x53, 0x53,
    0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// The status labels: 27 FF7-encoded strings of 15 bytes, read as row i of the table:
//   [0] "Element", [1] "Effect", [2] "Attack", [3] "Defend", [4] "Halve", [5] "Invalid",
//   [6] "Absorb", [7] "", [8] "Strength", [9] "Dexterity", [10] "Vitality", [11] "Magic",
//   [12] "Spirit", [13] "Luck", [14] "Attack", [15] "Attack%", [16] "Defense", [17] "Defense%",
//   [18] "Magic atk", [19] "Magic def", [20] "Magic def%", [21] "", [22] "Wpn:", [23] "Arm:",
//   [24] "Acc:", [25] "", [26] "Limit level:"
// clang-format off
// GLOBAL: FF7 0x009207c0
unsigned char g_MenuStatusLabelTable[27][15] = {
    {0x25, 0x4c, 0x45, 0x4d, 0x45, 0x4e, 0x54, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x25, 0x46, 0x46, 0x45, 0x43, 0x54, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x21, 0x54, 0x54, 0x41, 0x43, 0x4b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x24, 0x45, 0x46, 0x45, 0x4e, 0x44, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x28, 0x41, 0x4c, 0x56, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x29, 0x4e, 0x56, 0x41, 0x4c, 0x49, 0x44, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x21, 0x42, 0x53, 0x4f, 0x52, 0x42, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x33, 0x54, 0x52, 0x45, 0x4e, 0x47, 0x54, 0x48, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x24, 0x45, 0x58, 0x54, 0x45, 0x52, 0x49, 0x54, 0x59, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x36, 0x49, 0x54, 0x41, 0x4c, 0x49, 0x54, 0x59, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x2d, 0x41, 0x47, 0x49, 0x43, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x33, 0x50, 0x49, 0x52, 0x49, 0x54, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x2c, 0x55, 0x43, 0x4b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x21, 0x54, 0x54, 0x41, 0x43, 0x4b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x21, 0x54, 0x54, 0x41, 0x43, 0x4b, 0x05, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x24, 0x45, 0x46, 0x45, 0x4e, 0x53, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x24, 0x45, 0x46, 0x45, 0x4e, 0x53, 0x45, 0x05, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x2d, 0x41, 0x47, 0x49, 0x43, 0x00, 0x41, 0x54, 0x4b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x2d, 0x41, 0x47, 0x49, 0x43, 0x00, 0x44, 0x45, 0x46, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x2d, 0x41, 0x47, 0x49, 0x43, 0x00, 0x44, 0x45, 0x46, 0x05, 0xff, 0x00, 0x00, 0x00, 0x00},
    {0x37, 0x50, 0x4e, 0x1a, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x21, 0x52, 0x4d, 0x1a, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x21, 0x43, 0x43, 0x1a, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x2c, 0x49, 0x4d, 0x49, 0x54, 0x00, 0x4c, 0x45, 0x56, 0x45, 0x4c, 0x1a, 0xff, 0x00, 0x00},
};
// Screen rectangle, tab widths and tab height for the 640x480 and 320x240 layouts; Menu_StatusScreenInit
// points g_MenuStatusScreenFrame, g_MenuStatusTabWidths and g_MenuStatusTabHeights at one set. The 16
// bytes after the 640x480 heights have no reader.
// GLOBAL: FF7 0x00920958
struct EnglishWindowFrame g_MenuStatusRect640x480 = {0, 0, 640, 480};
// GLOBAL: FF7 0x00920960
struct EnglishWindowFrame g_MenuStatusRect320x240 = {0, 0, 320, 240};
// GLOBAL: FF7 0x00920968
short g_MenuStatusTabs640x480[4] = {100, 173, 253, 320};
// GLOBAL: FF7 0x00920970
short g_MenuStatusTabs320x240[4] = {50, 87, 127, 160};
// GLOBAL: FF7 0x00920978
short g_MenuStatusOffset640x480[4] = {0, 0, 0, 120};
unsigned char g_MenuStatusUnreadFill[16] = {0x00, 0x00, 0x00, 0x00, 0x92, 0x01, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6e, 0x00, 0x78, 0x00};
// GLOBAL: FF7 0x00920990
short g_MenuStatusOffset320x240[4] = {0, 0, 0, 60};
unsigned char g_rawData_00920998[16] = {0x00, 0x00, 0x00, 0x00, 0xc9, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x37, 0x00, 0x3c, 0x00};
// GLOBAL: FF7 0x00dca478
int g_MenuStatusPartyIndex = 0;
// GLOBAL: FF7 0x00dca47c
struct EnglishWindowFrame *g_MenuStatusScreenFrame = 0;
// GLOBAL: FF7 0x00dca480
short *g_MenuStatusTabWidths = 0;
// GLOBAL: FF7 0x00dca484
short *g_MenuStatusTabHeights = 0;
// GLOBAL: FF7 0x00dca488
int g_MenuStatusPage = 0;
unsigned char g_bssPad_00dca48c[4] = {0};
// clang-format on

/*
 * Initializes status menu screen state, selected page, and scaling layouts.
 */
// FUNCTION: FF7 0x00703760
void __cdecl Menu_StatusScreenInit(void)
{
    g_MenuStatusPage = MENU_STATUS_PAGE_STATS;
    g_MenuStatusPartyIndex = g_MenuPartyMemberIndex;
    if (g_MenuIsSmallViewport320x240 != 1) {
        g_MenuStatusScreenFrame = (struct EnglishWindowFrame *)&g_MenuStatusRect640x480;
        g_MenuStatusTabWidths = g_MenuStatusTabs640x480;
        g_MenuStatusTabHeights = g_MenuStatusOffset640x480;
    } else {
        g_MenuStatusScreenFrame = (struct EnglishWindowFrame *)&g_MenuStatusRect320x240;
        g_MenuStatusTabWidths = g_MenuStatusTabs320x240;
        g_MenuStatusTabHeights = g_MenuStatusOffset320x240;
    }
}

/*
 * Draws character portrait for the status block in 2x mode.
 */
// FUNCTION: FF7 0x007037c0
void __cdecl Menu_DrawStatusCharBlockPortrait2x(void)
{
    Menu_DrawCharStatBlock2x(103, 18, g_MenuStatusPartyIndex, 0.1f);
    Menu_DrawCharPortrait2x(13, 9, g_MenuStatusPartyIndex, 0.1f);
}

/*
 * Draws character portrait for the status block in 1x mode.
 */
// FUNCTION: FF7 0x007037f4
void __cdecl Menu_DrawStatusCharBlockPortrait1x(void)
{
    Menu_DrawCharStatBlock1x(52, 9, g_MenuStatusPartyIndex, 0.1f);
    Menu_DrawCharPortrait1x(7, 5, g_MenuStatusPartyIndex, 0.1f);
}

/*
 * Unused element abbreviation helper routine.
 */
// FUNCTION: FF7 0x00703828
void __cdecl Menu_ElementAbbrevStubUnused(int posX, int posY, int which)
{
    int str;
    int unused;
    int bits;
    int i;
    int bit;

    for (unused = i = 0; i < 27; i++) {
        if (i == 19) {
            continue;
        }
        bit = i;
        str = (int)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0xb, 8);
        switch (which) {
        case MENU_STATUS_EFFECT_ROW_ATTACK:
            bits = g_MenuPartyStats[g_MenuStatusPartyIndex].attackStatuses &
                   (1 << bit);
            break;
        case MENU_STATUS_EFFECT_ROW_DEFENSE:
            bits = g_MenuPartyStats[g_MenuStatusPartyIndex].defenseStatuses &
                   (1 << bit);
            break;
        }
    }
}

/*
 * Draws element glyphs and affinity labels for a status row.
 */
// FUNCTION: FF7 0x007038cc
void __cdecl Menu_DrawElementRowGlyphs(int x, int y, int row)
{
    int scratch;
    unsigned char *str;
    int first;
    int set;
    int elementIndex;
    int cx;
    int bit;

    scratch = 0;
    cx = 0;
    elementIndex = 0;
    for (first = elementIndex; elementIndex < 9; elementIndex++) {
        bit = elementIndex;
        str = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0xB, 8);
        switch (row) {
        case MENU_STATUS_ELEMENT_ROW_ATTACK:
            set = g_MenuPartyStats[g_MenuStatusPartyIndex].attackElements & 1 << bit;
            break;
        case MENU_STATUS_ELEMENT_ROW_HALVED:
            set = g_MenuPartyStats[g_MenuStatusPartyIndex].halvedElements & 1 << bit;
            break;
        case MENU_STATUS_ELEMENT_ROW_NULLIFIED:
            set = g_MenuPartyStats[g_MenuStatusPartyIndex].nullifiedElements & 1 << bit;
            break;
        case MENU_STATUS_ELEMENT_ROW_ABSORBED:
            set = g_MenuPartyStats[g_MenuStatusPartyIndex].absorbedElements & 1 << bit;
        }
        if (g_MenuIsSmallViewport320x240 != 1) {
            if (elementIndex == 6) {
                cx = 0;
                y += 30;
            }
            Menu_DrawFontGlyph2x(x + elementIndex * 23 + 0xB1, y + row * 34, set != 0 ? 7 : 0, str[elementIndex * 2 + 1] << 8 | str[elementIndex * 2], 0.1f);
        } else {
            if (elementIndex == 6) {
                cx = 0;
                y += 15;
            }
            Menu_DrawFontGlyph2x(x + elementIndex * 12 + 0x59, y + row * 17, set != 0 ? 7 : 0, str[elementIndex * 2 + 1] << 8 | str[elementIndex * 2], 0.1f);
        }
    }
}

/*
 * Main frame update and input handling for the status menu screen.
 */
// FUNCTION: FF7 0x00703abd
void __cdecl Menu_StatusScreenFrame(unsigned int frameCounter)
{
    int actor;

    actor = g_CharIdToSaveSlot[gSavemap.party[g_MenuStatusPartyIndex]];
    if (g_MenuIsSmallViewport320x240 != 1) {
        Menu_DrawCommandWindow2x(g_MenuModuleId);
        Menu_DrawStatusCharBlockPortrait2x();
        switch (g_MenuStatusPage) {
        case MENU_STATUS_PAGE_STATS:
            Menu_DrawStatusEntryNameGrid2x(297, 137);
            Menu_DrawStatusEquipmentPanel2x(0x11B, 0x139);
            Menu_DrawStatusStatRows(60, 120);
            Menu_DrawStatusExpPanel2x(frameCounter);
            break;
        case MENU_STATUS_PAGE_ELEMENTS:
        case MENU_STATUS_PAGE_STATUSES:
            Menu_DrawElementStatusPage2x(63, 154);
            Menu_DrawStatusExpPanel2x(frameCounter);
            break;
        }
        Menu_DrawWindowFrame2x(g_MenuStatusScreenFrame, 0.2f);
    } else {
        Menu_DrawCommandWindow1x(g_MenuModuleId);
        Menu_DrawStatusCharBlockPortrait1x();
        switch (g_MenuStatusPage) {
        case MENU_STATUS_PAGE_STATS:
            Menu_DrawStatusEntryNameGrid1x(149, 69);
            Menu_DrawStatusEquipmentPanel1x(0x8E, 0x9D);
            Menu_DrawStatusStatRows1x(30, 60);
            Menu_DrawStatusExpPanel1x(frameCounter);
            break;
        case MENU_STATUS_PAGE_ELEMENTS:
        case MENU_STATUS_PAGE_STATUSES:
            Menu_DrawElementStatusPage1x(32, 77);
            Menu_DrawStatusExpPanel1x(frameCounter);
            break;
        }
        Menu_DrawWindowFrame1x(g_MenuStatusScreenFrame, 0.2f);
    }
    if (Menu_GetPhase() == 0) {
        switch (g_MenuStatusPage) {
        case MENU_STATUS_PAGE_STATS:
            if (Input_GetPressed(INPUT_CIRCLE) != 0) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                g_MenuStatusPage = MENU_STATUS_PAGE_ELEMENTS;
            } else if (Input_GetPressed(INPUT_CROSS) != 0) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                Menu_SetPhase(5, 0);
                Menu_SetScreen(0);
            }
            break;
        case MENU_STATUS_PAGE_ELEMENTS:
            if (Input_GetPressed(INPUT_CIRCLE) != 0) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                g_MenuStatusPage = MENU_STATUS_PAGE_STATUSES;
            } else if (Input_GetPressed(INPUT_CROSS) != 0) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                Menu_SetPhase(5, 0);
                Menu_SetScreen(0);
            }
            break;
        case MENU_STATUS_PAGE_STATUSES:
            if (Input_GetPressed(INPUT_CIRCLE) != 0) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                g_MenuStatusPage = MENU_STATUS_PAGE_STATS;
            } else if (Input_GetPressed(INPUT_CROSS) != 0) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                Menu_SetPhase(5, 0);
                Menu_SetScreen(0);
            }
            break;
        }
        if (BtlMenu_GetInputRepeat(INPUT_L1) != 0) {
            SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
            do {
                g_MenuStatusPartyIndex--;
                if (g_MenuStatusPartyIndex < 0) {
                    g_MenuStatusPartyIndex = 2;
                }
            } while (gSavemap.party[g_MenuStatusPartyIndex] == PARTY_CHAR_NONE);
        } else if (BtlMenu_GetInputRepeat(INPUT_R1) != 0) {
            SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
            do {
                g_MenuStatusPartyIndex++;
                if (g_MenuStatusPartyIndex > 2) {
                    g_MenuStatusPartyIndex = 0;
                }
            } while (gSavemap.party[g_MenuStatusPartyIndex] == PARTY_CHAR_NONE);
        }
    }
}

/*
 * Renders elemental resistance and affinity page in 2x mode.
 */
// FUNCTION: FF7 0x00703df1
void __cdecl Menu_DrawElementStatusPage2x(int originX, int originY)
{
    int bitIdx;
    int line;
    int xPos;
    int textWidth;
    unsigned int isSet;
    int index;
    unsigned int bit;

    switch (g_MenuStatusPage) {
    case MENU_STATUS_PAGE_ELEMENTS:
        Menu_DrawEncodedString2x(originX - 0x1b, originY - 0x22,
                          g_MenuStatusLabelTable[0], 5, 0.1f);
        Menu_DrawEncodedString2x(originX, originY,
                          g_MenuStatusLabelTable[2], 5,
                          0.1f);
        Menu_DrawEncodedString2x(originX, originY + 0x49,
                          g_MenuStatusLabelTable[4], 5,
                          0.1f);
        Menu_DrawEncodedString2x(originX, originY + 0x92,
                          g_MenuStatusLabelTable[5], 5,
                          0.1f);
        Menu_DrawEncodedString2x(originX, originY + 0xdb,
                          g_MenuStatusLabelTable[6], 5,
                          0.1f);
        for (index = 0; index < 4; index++) {
            xPos = originX + 75;
            line = bitIdx = 0;
            for (; bitIdx < 9; bitIdx++) {
                bit = 1 << bitIdx;
                switch (index) {
                case MENU_STATUS_ELEMENT_ROW_ATTACK:
                    isSet = g_MenuPartyStats[g_MenuStatusPartyIndex].attackElements &
                        bit;
                    break;
                case MENU_STATUS_ELEMENT_ROW_HALVED:
                    isSet = g_MenuPartyStats[g_MenuStatusPartyIndex].halvedElements &
                        bit;
                    break;
                case MENU_STATUS_ELEMENT_ROW_NULLIFIED:
                    isSet = g_MenuPartyStats[g_MenuStatusPartyIndex].nullifiedElements &
                        bit;
                    break;
                case MENU_STATUS_ELEMENT_ROW_ABSORBED:
                    isSet = g_MenuPartyStats[g_MenuStatusPartyIndex].absorbedElements &
                        bit;
                    break;
                }
                textWidth = Menu_MeasureEncodedString2x(g_MenuStatusElementNames + bitIdx * 10);
                if (textWidth + xPos > 583) {
                    line++;
                    xPos = originX + 75;
                }
                Menu_DrawEncodedString2x(xPos, originY + index * 72 + line * 30,
                                  g_MenuStatusElementNames + bitIdx * 10,
                                  isSet ? 7 : 0, 0.101f);
                textWidth = Menu_MeasureEncodedString2x(g_MenuStatusElementNames + bitIdx * 10);
                xPos = xPos + textWidth + 17;
            }
        }
        break;
    case MENU_STATUS_PAGE_STATUSES:
        originX -= 27;
        originY -= 4;
        Menu_DrawEncodedString2x(originX - 0x17, originY - 0x1e,
                          g_MenuStatusLabelTable[1], 5,
                          0.101f);
        Menu_DrawEncodedString2x(originX - 0x11, originY,
                          g_MenuStatusLabelTable[2], 5,
                          0.101f);
        Menu_DrawEncodedString2x(originX - 0x11, originY + 0x9a,
                          g_MenuStatusLabelTable[3], 5,
                          0.101f);
        for (index = 0; index < 2; index++) {
            xPos = originX + 58;
            line = bitIdx = 0;
            for (; bitIdx < 0x1b; bitIdx++) {
                if (bitIdx == 0x13) {
                    continue;
                }
                bit = 1 << bitIdx;
                switch (index) {
                case MENU_STATUS_EFFECT_ROW_ATTACK:
                    isSet = g_MenuPartyStats[g_MenuStatusPartyIndex].attackStatuses & bit;
                    break;
                case MENU_STATUS_EFFECT_ROW_DEFENSE:
                    isSet = g_MenuPartyStats[g_MenuStatusPartyIndex].defenseStatuses & bit;
                    break;
                }
                textWidth = Menu_MeasureEncodedString2x(g_MenuStatusConditionNames + bitIdx * 20);
                if (textWidth + xPos > 583) {
                    line++;
                    xPos = originX + 58;
                }
                Menu_DrawEncodedString2x(xPos, originY + index * 154 + line * 30,
                                  g_MenuStatusConditionNames + bitIdx * 20,
                                  isSet ? 7 : 0, 0.1f);
                textWidth = Menu_MeasureEncodedString2x(g_MenuStatusConditionNames + bitIdx * 20);
                xPos = xPos + textWidth + 10;
            }
        }
        break;
    }
}

/*
 * Renders elemental resistance and affinity page in 1x mode.
 */
// FUNCTION: FF7 0x00704207
void __cdecl Menu_DrawElementStatusPage1x(int originX, int originY)
{
    int bitIdx;
    int line;
    int xPos;
    int textWidth;
    unsigned int isSet;
    int index;
    unsigned int bit;

    switch (g_MenuStatusPage) {
    case MENU_STATUS_PAGE_ELEMENTS:
        Menu_DrawEncodedString1x(originX - 0xe, originY - 0x11,
                             g_MenuStatusLabelTable[0], 5, 0.1f);
        Menu_DrawEncodedString1x(originX, originY,
                             g_MenuStatusLabelTable[2], 5,
                             0.1f);
        Menu_DrawEncodedString1x(originX, originY + 0x25,
                             g_MenuStatusLabelTable[4], 5,
                             0.1f);
        Menu_DrawEncodedString1x(originX, originY + 0x4a,
                             g_MenuStatusLabelTable[5], 5,
                             0.1f);
        Menu_DrawEncodedString1x(originX, originY + 0x6f,
                             g_MenuStatusLabelTable[6], 5,
                             0.1f);
        for (index = 0; index < 4; index++) {
            xPos = originX + 38;
            line = bitIdx = 0;
            for (; bitIdx < 9; bitIdx++) {
                bit = 1 << bitIdx;
                switch (index) {
                case MENU_STATUS_ELEMENT_ROW_ATTACK:
                    isSet = g_MenuPartyStats[g_MenuStatusPartyIndex].attackElements &
                        bit;
                    break;
                case MENU_STATUS_ELEMENT_ROW_HALVED:
                    isSet = g_MenuPartyStats[g_MenuStatusPartyIndex].halvedElements &
                        bit;
                    break;
                case MENU_STATUS_ELEMENT_ROW_NULLIFIED:
                    isSet = g_MenuPartyStats[g_MenuStatusPartyIndex].nullifiedElements &
                        bit;
                    break;
                case MENU_STATUS_ELEMENT_ROW_ABSORBED:
                    isSet = g_MenuPartyStats[g_MenuStatusPartyIndex].absorbedElements &
                        bit;
                    break;
                }
                textWidth = Menu_MeasureEncodedString1x(g_MenuStatusElementNames + bitIdx * 10);
                if (textWidth + xPos > 292) {
                    line++;
                    xPos = originX + 38;
                }
                Menu_DrawEncodedString1x(xPos, originY + index * 36 + line * 15,
                                     g_MenuStatusElementNames + bitIdx * 10,
                                     isSet ? 7 : 0, 0.101f);
                textWidth = Menu_MeasureEncodedString1x(g_MenuStatusElementNames + bitIdx * 10);
                xPos = xPos + textWidth + 9;
            }
        }
        break;
    case MENU_STATUS_PAGE_STATUSES:
        originX -= 14;
        originY -= 2;
        Menu_DrawEncodedString1x(originX - 0xc, originY - 0xf,
                             g_MenuStatusLabelTable[1], 5,
                             0.101f);
        Menu_DrawEncodedString1x(originX - 9, originY,
                             g_MenuStatusLabelTable[2], 5,
                             0.101f);
        Menu_DrawEncodedString1x(originX - 9, originY + 0x4d,
                             g_MenuStatusLabelTable[3], 5,
                             0.101f);
        for (index = 0; index < 2; index++) {
            xPos = originX + 29;
            line = bitIdx = 0;
            for (; bitIdx < 0x1b; bitIdx++) {
                if (bitIdx == 0x13) {
                    continue;
                }
                bit = 1 << bitIdx;
                switch (index) {
                case MENU_STATUS_EFFECT_ROW_ATTACK:
                    isSet = g_MenuPartyStats[g_MenuStatusPartyIndex].attackStatuses & bit;
                    break;
                case MENU_STATUS_EFFECT_ROW_DEFENSE:
                    isSet = g_MenuPartyStats[g_MenuStatusPartyIndex].defenseStatuses & bit;
                    break;
                }
                textWidth = Menu_MeasureEncodedString1x(g_MenuStatusConditionNames + bitIdx * 20);
                if (textWidth + xPos > 292) {
                    line++;
                    xPos = originX + 29;
                }
                Menu_DrawEncodedString1x(xPos, originY + index * 77 + line * 15,
                                     g_MenuStatusConditionNames + bitIdx * 20,
                                     isSet ? 7 : 0, 0.1f);
                textWidth = Menu_MeasureEncodedString1x(g_MenuStatusConditionNames + bitIdx * 20);
                xPos = xPos + textWidth + 5;
            }
        }
        break;
    }
}

/*
 * Draws equipment slots and equipped gear names in 2x mode.
 */
// FUNCTION: FF7 0x00704612
void __cdecl Menu_DrawStatusEquipmentPanel2x(int baseX, int baseY)
{
    SavemapCharacter *recordBase;
    int saveSlot;

    saveSlot = g_CharIdToSaveSlot[gSavemap.party[g_MenuStatusPartyIndex]];
    recordBase = &gSavemap.chars[0];
    Menu_DrawEncodedString2x(baseX + 0x5D, baseY, Kernel_LookupItemNameText(recordBase[saveSlot].weapon + 0x80), 7, 0.1f);
    Menu_DrawEncodedString2x(baseX + 0x5D, baseY + 0x40, Kernel_LookupItemNameText(recordBase[saveSlot].armor + 0x100), 7, 0.1f);
    if (recordBase[saveSlot].accessory != 0xFF) {
        Menu_DrawEncodedString2x(baseX + 0x5D, baseY + 0x80, Kernel_LookupItemNameText(recordBase[saveSlot].accessory + 0x120), 7, 0.1f);
    }
    Menu_DrawEncodedString2x(baseX + 0x1A, baseY, (unsigned char *)g_MenuStatusLabelTable[23], 5, 0.1f);
    Menu_DrawEncodedString2x(baseX + 0x1A, baseY + 0x40, (unsigned char *)g_MenuStatusLabelTable[24], 5, 0.1f);
    Menu_DrawEncodedString2x(baseX + 0x1A, baseY + 0x80, (unsigned char *)g_MenuStatusLabelTable[25], 5, 0.1f);
    Menu_DrawStatusMateriaSlotRow2x(baseX + 0x4D, baseY + 0x20, g_KernelWeaponDataArray[recordBase[saveSlot].weapon].materiaSlotTable, recordBase[saveSlot].weaponMateria);
    Menu_DrawStatusMateriaSlotRow2x(baseX + 0x4D, baseY + 0x60, g_KernelArmorDataArray[recordBase[saveSlot].armor].materiaSlotBytes, recordBase[saveSlot].armorMateria);
}

/*
 * Draws materia slots and linked pairs for status equipment in 2x mode.
 */
// FUNCTION: FF7 0x007047fb
void __cdecl Menu_DrawStatusMateriaSlotRow2x(int x, int y, unsigned char *slotTypes, int *materiaIds)
{
    short rect[4];
    int palette;
    int slot;

    for (slot = 0; slot < 8; slot++) {
        if (slotTypes[slot] != 0) {
            if ((materiaIds[slot] & 0xff) != 0xff) {
                palette = Menu_GetMateriaTypeColorIndex(materiaIds[slot] & 0xff);
                Menu_DrawAtlasQuad2x(x + slot * 28, y, 0x80,
                                     0x20, 16, 16, palette, 1, 0, 0.1f);
            }
            if (slotTypes[slot] > 4) {
                Menu_DrawAtlasQuad2x(x + slot * 28, y, 0x80,
                                     0x30, 16, 16, 1, 0, 0, 0.101f);
            } else {
                Menu_DrawAtlasQuad2x(x + slot * 28, y, 0x70,
                                     0x20, 16, 16, 1, 0, 0, 0.101f);
            }
        }
    }
    for (slot = 0; slot < 8; slot++) {
        if (slotTypes[slot] != 0 &&
            (slotTypes[slot] == 3 ||
             slotTypes[slot] == 7)) {
            Menu_DrawAtlasQuad2x(x + slot * 28 - 0xc, y, 0x70,
                                 0x30, 16, 16, 1, 0, 0, 0.1001f);
        }
    }
    rect[0] = x - 3;
    rect[1] = y;
    rect[2] = 0xe2;
    rect[3] = 0x18;
    Menu_DrawEtchedBox2x((EnglishWindowFrame *)rect, 0.101f);
}

/*
 * Draws equipment slots and equipped gear names in 1x mode.
 */
// FUNCTION: FF7 0x007049a6
void __cdecl Menu_DrawStatusEquipmentPanel1x(int baseX, int baseY)
{
    SavemapCharacter *recordBase;
    int saveSlot;

    saveSlot = g_CharIdToSaveSlot[gSavemap.party[g_MenuStatusPartyIndex]];
    recordBase = &gSavemap.chars[0];
    Menu_DrawEncodedString1x(baseX + 0x2F, baseY, Kernel_LookupItemNameText(recordBase[saveSlot].weapon + 0x80), 7, 0.1f);
    Menu_DrawEncodedString1x(baseX + 0x2F, baseY + 0x20, Kernel_LookupItemNameText(recordBase[saveSlot].armor + 0x100), 7, 0.1f);
    if (recordBase[saveSlot].accessory != 0xFF) {
        Menu_DrawEncodedString1x(baseX + 0x2F, baseY + 0x40, Kernel_LookupItemNameText(recordBase[saveSlot].accessory + 0x120), 7, 0.1f);
    }
    Menu_DrawEncodedString1x(baseX + 0xD, baseY, (unsigned char *)g_MenuStatusLabelTable[23], 5, 0.1f);
    Menu_DrawEncodedString1x(baseX + 0xD, baseY + 0x20, (unsigned char *)g_MenuStatusLabelTable[24], 5, 0.1f);
    Menu_DrawEncodedString1x(baseX + 0xD, baseY + 0x40, (unsigned char *)g_MenuStatusLabelTable[25], 5, 0.1f);
    Menu_DrawStatusMateriaSlotRow1x(baseX + 0x27, baseY + 0x10, g_KernelWeaponDataArray[recordBase[saveSlot].weapon].materiaSlotTable, recordBase[saveSlot].weaponMateria);
    Menu_DrawStatusMateriaSlotRow1x(baseX + 0x27, baseY + 0x30, g_KernelArmorDataArray[recordBase[saveSlot].armor].materiaSlotBytes, recordBase[saveSlot].armorMateria);
}

/*
 * Draws materia slots and linked pairs for status equipment in 1x mode.
 */
// FUNCTION: FF7 0x00704b89
void __cdecl Menu_DrawStatusMateriaSlotRow1x(int x, int y, unsigned char *slotTypes, int *materiaIds)
{
    short rect[4];
    int palette;
    int slot;

    for (slot = 0; slot < 8; slot++) {
        if (slotTypes[slot] != 0) {
            if ((materiaIds[slot] & 0xff) != 0xff) {
                palette = Menu_GetMateriaTypeColorIndex(materiaIds[slot] & 0xff);
                Menu_DrawAtlasQuad1x(x + slot * 14, y, 128,
                                     32, 16, 16, palette, 1, 0, 0.1f);
            }
            if (slotTypes[slot] > 4) {
                Menu_DrawAtlasQuad1x(x + slot * 14, y, 128,
                                     48, 16, 16, 1, 0, 0, 0.101f);
            } else {
                Menu_DrawAtlasQuad1x(x + slot * 14, y, 112,
                                     32, 16, 16, 1, 0, 0, 0.101f);
            }
        }
    }
    for (slot = 0; slot < 8; slot++) {
        if (slotTypes[slot] != 0 &&
            (slotTypes[slot] == 3 ||
             slotTypes[slot] == 7)) {
            Menu_DrawAtlasQuad1x(x + slot * 14 - 7, y, 112,
                                 48, 16, 16, 1, 0, 0, 0.1001f);
        }
    }
    rect[0] = x - 2;
    rect[1] = y;
    rect[2] = 0x72;
    rect[3] = 0xc;
    Menu_DrawEtchedBox1x((EnglishWindowFrame *)rect, 0.101f);
}

/*
 * Draws primary and secondary character battle statistics rows in 2x mode.
 */
// FUNCTION: FF7 0x00704d34
void __cdecl Menu_DrawStatusStatRows(int x, int y)
{
    int statField;
    int weaponField;
    int defense;
    int statValue;
    int i;
    int wordValC;
    int armorByte;
    int statWordE;
    int partyMember;

    partyMember = gSavemap.party[g_MenuStatusPartyIndex];
    for (i = 0; i < 6; i++) {
        Menu_DrawEncodedString2x(x, y + i * 0x18, (unsigned char *)g_MenuStatusLabelTable[i + 0xA], 5, 0.1f);
    }
    Menu_DrawDecimalNumber2x(x + 0x8D, y, g_MenuPartyStats[g_MenuStatusPartyIndex].strength, 3, 7, 0.1f);
    Menu_DrawDecimalNumber2x(x + 0x8D, y + 0x18, g_MenuPartyStats[g_MenuStatusPartyIndex].dexterity, 3, 7, 0.1f);
    Menu_DrawDecimalNumber2x(x + 0x8D, y + 0x30, g_MenuPartyStats[g_MenuStatusPartyIndex].vitality, 3, 7, 0.1f);
    Menu_DrawDecimalNumber2x(x + 0x8D, y + 0x48, g_MenuPartyStats[g_MenuStatusPartyIndex].magic, 3, 7, 0.1f);
    Menu_DrawDecimalNumber2x(x + 0x8D, y + 0x60, g_MenuPartyStats[g_MenuStatusPartyIndex].spirit, 3, 7, 0.1f);
    Menu_DrawDecimalNumber2x(x + 0x8D, y + 0x78, g_MenuPartyStats[g_MenuStatusPartyIndex].luck, 3, 7, 0.1f);
    y += 176;
    for (i = 0; i < 7; i++) {
        Menu_DrawEncodedString2x(x, y + i * 0x18, (unsigned char *)g_MenuStatusLabelTable[i + 0x10], 5, 0.1f);
    }
    statValue = g_MenuPartyStats[g_MenuStatusPartyIndex].attack;
    weaponField = g_KernelWeaponDataArray[gSavemap.chars[g_CharIdToSaveSlot[partyMember]].weapon].hitRate;
    statField = g_MenuPartyStats[g_MenuStatusPartyIndex].defense;
    defense = g_MenuPartyStats[g_MenuStatusPartyIndex].dexterity / 4 + g_KernelArmorDataArray[gSavemap.chars[g_CharIdToSaveSlot[partyMember]].armor].dfpo;
    wordValC = g_MenuPartyStats[g_MenuStatusPartyIndex].magicAttack;
    statWordE = g_MenuPartyStats[g_MenuStatusPartyIndex].magicDefense;
    armorByte = g_KernelArmorDataArray[gSavemap.chars[g_CharIdToSaveSlot[partyMember]].armor].magicEvade;
    Menu_DrawDecimalNumber2x(x + 0x8D, y, statValue, 3, 7, 0.1f);
    Menu_DrawDecimalNumber2x(x + 0x8D, y + 0x18, weaponField, 3, 7, 0.1f);
    Menu_DrawDecimalNumber2x(x + 0x8D, y + 0x30, statField, 3, 7, 0.1f);
    Menu_DrawDecimalNumber2x(x + 0x8D, y + 0x48, defense, 3, 7, 0.1f);
    Menu_DrawDecimalNumber2x(x + 0x8D, y + 0x60, wordValC, 3, 7, 0.1f);
    Menu_DrawDecimalNumber2x(x + 0x8D, y + 0x78, statWordE, 3, 7, 0.1f);
    Menu_DrawDecimalNumber2x(x + 0x8D, y + 0x90, armorByte, 3, 7, 0.1f);
}

/*
 * Draws primary and secondary character battle statistics rows in 1x mode.
 */
// FUNCTION: FF7 0x00705122
void __cdecl Menu_DrawStatusStatRows1x(int originX, int originY)
{
    int statP;
    int statC;
    int defenseValue;
    int statValue;
    int i;
    int magicDefense;
    int fieldE;
    int bonusStat;
    int characterIndex;

    characterIndex = gSavemap.party[g_MenuStatusPartyIndex];
    for (i = 0; i < 6; i++) {
        Menu_DrawEncodedString1x(originX, originY + i * 0xC, (unsigned char *)g_MenuStatusLabelTable[i + 0xA], 5, 0.1f);
    }
    Menu_DrawDecimalNumber1x(originX + 0x47, originY, g_MenuPartyStats[g_MenuStatusPartyIndex].strength, 3, 7, 0.1f);
    Menu_DrawDecimalNumber1x(originX + 0x47, originY + 0xC, g_MenuPartyStats[g_MenuStatusPartyIndex].dexterity, 3, 7, 0.1f);
    Menu_DrawDecimalNumber1x(originX + 0x47, originY + 0x18, g_MenuPartyStats[g_MenuStatusPartyIndex].vitality, 3, 7, 0.1f);
    Menu_DrawDecimalNumber1x(originX + 0x47, originY + 0x24, g_MenuPartyStats[g_MenuStatusPartyIndex].magic, 3, 7, 0.1f);
    Menu_DrawDecimalNumber1x(originX + 0x47, originY + 0x30, g_MenuPartyStats[g_MenuStatusPartyIndex].spirit, 3, 7, 0.1f);
    Menu_DrawDecimalNumber1x(originX + 0x47, originY + 0x3C, g_MenuPartyStats[g_MenuStatusPartyIndex].luck, 3, 7, 0.1f);
    originY += 88;
    for (i = 0; i < 7; i++) {
        Menu_DrawEncodedString1x(originX, originY + i * 0xC, (unsigned char *)g_MenuStatusLabelTable[i + 0x10], 5, 0.1f);
    }
    statValue = g_MenuPartyStats[g_MenuStatusPartyIndex].attack;
    statP = g_KernelWeaponDataArray[gSavemap.chars[g_CharIdToSaveSlot[characterIndex]].weapon].hitRate;
    statC = g_MenuPartyStats[g_MenuStatusPartyIndex].defense;
    defenseValue = g_MenuPartyStats[g_MenuStatusPartyIndex].dexterity / 4 + g_KernelArmorDataArray[gSavemap.chars[g_CharIdToSaveSlot[characterIndex]].armor].dfpo;
    bonusStat = g_MenuPartyStats[g_MenuStatusPartyIndex].magicAttack;
    fieldE = g_MenuPartyStats[g_MenuStatusPartyIndex].magicDefense;
    magicDefense = g_KernelArmorDataArray[gSavemap.chars[g_CharIdToSaveSlot[characterIndex]].armor].magicEvade;
    Menu_DrawDecimalNumber1x(originX + 0x47, originY, statValue, 3, 7, 0.1f);
    Menu_DrawDecimalNumber1x(originX + 0x47, originY + 0xC, statP, 3, 7, 0.1f);
    Menu_DrawDecimalNumber1x(originX + 0x47, originY + 0x18, statC, 3, 7, 0.1f);
    Menu_DrawDecimalNumber1x(originX + 0x47, originY + 0x24, defenseValue, 3, 7, 0.1f);
    Menu_DrawDecimalNumber1x(originX + 0x47, originY + 0x30, bonusStat, 3, 7, 0.1f);
    Menu_DrawDecimalNumber1x(originX + 0x47, originY + 0x3C, fieldE, 3, 7, 0.1f);
    Menu_DrawDecimalNumber1x(originX + 0x47, originY + 0x48, magicDefense, 3, 7, 0.1f);
}

/*
 * Draws current EXP and next level EXP progress panel in 2x mode.
 */
// FUNCTION: FF7 0x007054ec
void __cdecl Menu_DrawStatusExpPanel2x(int frame)
{
    int slotCopy;
    int color;
    int saveSlot;

    saveSlot = g_CharIdToSaveSlot[gSavemap.party[g_MenuStatusPartyIndex]];
    slotCopy = saveSlot;
    if (gSavemap.chars[slotCopy].limitBar == 0xFF) {
        color = g_MenuColorFormatBitmasks[frame >> 1 & 7];
    } else if ((gSavemap.chars[slotCopy].status & BATTLE_STATUS_SADNESS) != 0) {
        color = (*(int *)&g_MenuColorFormatBitmasks);
    } else if ((gSavemap.chars[slotCopy].status & BATTLE_STATUS_FURY) != 0) {
        color = g_MenuColorFormatMask;
    } else {
        color = 0x80802050;
    }
    Menu_DrawGaugeBar2x(407, 90, gSavemap.chars[slotCopy].limitBar * 116 / 256, 12, color, 0.1f);
    Menu_DrawAtlasQuad2x(403, 90, 0x88, 8, 64, 8, 7, 0, 0, 0.1001f);
    Menu_DrawEncodedString2x(247, 13, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 9, 8), 7, 0.1f);
    Menu_DrawGaugeBar2x(329, 42, *(unsigned char *)&gSavemap.chars[saveSlot].levelProgress << 1, 12, 0x80802020, 0.1f);
    Menu_DrawAtlasQuad2x(328, 42, 0x88, 8, 64, 8, 7, 0, 0, 0.1001f);
    Menu_DrawAtlasQuad2x(458, 18, 0xF8, 8, 8, 8, 7, 0, 0, 0.1f);
    Menu_DrawAtlasQuad2x(522, 68, 0xF8, 8, 8, 8, 7, 0, 0, 0.1f);
    Menu_DrawDecimalNumber2x(337, 16, gSavemap.chars[saveSlot].exp, 10, 7, 0.1f);
    Menu_DrawDecimalNumber2x(400, 66, gSavemap.chars[saveSlot].expToNext, 10, 7, 0.1f);
    Menu_DrawEncodedString2x(247, 62, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x2C, 8), 7, 0.1f);
    Menu_DrawEncodedString2x(249, 90, g_MenuStatusLabelTable[26], 7, 0.1f);
    Menu_DrawDecimalNumber2x(Menu_MeasureEncodedString2x(g_MenuStatusLabelTable[26]) + 0xF9, 94, gSavemap.chars[saveSlot].limitLevel, 1, 7, 0.1f);
}

/*
 * Draws current EXP and next level EXP progress panel in 1x mode.
 */
// FUNCTION: FF7 0x0070577a
void __cdecl Menu_DrawStatusExpPanel1x(int frame)
{
    int slotCopy;
    int color;
    int saveSlot;

    saveSlot = g_CharIdToSaveSlot[gSavemap.party[g_MenuStatusPartyIndex]];
    slotCopy = saveSlot;
    if (gSavemap.chars[slotCopy].limitBar == 0xFF) {
        color = g_MenuColorFormatBitmasks[frame >> 1 & 7];
    } else if ((gSavemap.chars[slotCopy].status & BATTLE_STATUS_SADNESS) != 0) {
        color = (*(int *)&g_MenuColorFormatBitmasks);
    } else if ((gSavemap.chars[slotCopy].status & BATTLE_STATUS_FURY) != 0) {
        color = g_MenuColorFormatMask;
    } else {
        color = 0x80802050;
    }
    Menu_DrawGaugeBar1x(204, 45, gSavemap.chars[slotCopy].limitBar * 58 / 256, 6, color, 0.1f);
    Menu_DrawAtlasQuad1x(202, 45, 136, 8, 64, 8, 7, 0, 0, 0.1001f);
    Menu_DrawEncodedString1x(124, 7, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 9, 8), 7, 0.1f);
    Menu_DrawGaugeBar1x(164, 21, *(unsigned char *)&gSavemap.chars[saveSlot].levelProgress, 6, 0x80802020, 0.1f);
    Menu_DrawAtlasQuad1x(164, 21, 136, 8, 64, 8, 7, 0, 0, 0.1001f);
    Menu_DrawAtlasQuad1x(229, 9, 248, 8, 8, 8, 7, 0, 0, 0.1f);
    Menu_DrawAtlasQuad1x(261, 34, 248, 8, 8, 8, 7, 0, 0, 0.1f);
    Menu_DrawDecimalNumber1x(169, 8, gSavemap.chars[saveSlot].exp, 0xA, 7, 0.1f);
    Menu_DrawDecimalNumber1x(200, 33, gSavemap.chars[saveSlot].expToNext, 0xA, 7, 0.1f);
    Menu_DrawEncodedString1x(124, 31, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x2C, 8), 7, 0.1f);
    Menu_DrawEncodedString1x(125, 45, g_MenuStatusLabelTable[26], 7, 0.1f);
    Menu_DrawDecimalNumber1x(Menu_MeasureEncodedString1x(g_MenuStatusLabelTable[26]) + 0x7D, 47, gSavemap.chars[saveSlot].limitLevel, 1, 7, 0.1f);
}

/*
 * Draws grid of character status names and labels in 2x mode.
 */
// FUNCTION: FF7 0x007059fb
void __cdecl Menu_DrawStatusEntryNameGrid2x(int x, int y)
{
    short rect[4];
    struct MenuPartyStats *statBlock;
    int row;
    int cmdSlot;
    int bump;
    int n;

    statBlock = &g_MenuPartyStats[g_MenuStatusPartyIndex];
    n = statBlock->commandColumns;
    for (row = 0; row < n; row++) {
        for (cmdSlot = 0; cmdSlot < 4; cmdSlot++) {
            if (statBlock->commands[cmdSlot + row * 4].commandId != 0xff) {
                bump = (n > 1 && row > 0) ? 8 : 0;
                Menu_DrawEncodedString2x(x + (bump + row * 0x49) + 0xa,
                                  y + cmdSlot * 0x1a + 0xb,
                                  (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, statBlock->commands[cmdSlot + row * 4].commandId, 8),
                                  7, 0.1f);
            }
        }
    }
    Menu_SetRectShorts(rect, x, y, g_MenuStatusTabWidths[n - 1],
                  g_MenuStatusTabHeights[3]);
    Menu_DrawWindowFrame2x((EnglishWindowFrame *)rect, 0.1f);
}

/*
 * Draws grid of character status names and labels in 1x mode.
 */
// FUNCTION: FF7 0x00705b36
void __cdecl Menu_DrawStatusEntryNameGrid1x(int x, int y)
{
    short rect[4];
    struct MenuPartyStats *statBlock;
    int row;
    int cmdSlot;
    int bump;
    int n;

    statBlock = &g_MenuPartyStats[g_MenuStatusPartyIndex];
    n = statBlock->commandColumns;
    for (row = 0; row < n; row++) {
        for (cmdSlot = 0; cmdSlot < 4; cmdSlot++) {
            if (statBlock->commands[cmdSlot + row * 4].commandId != 0xff) {
                bump = (n > 1 && row > 0) ? 8 : 0;
                Menu_DrawEncodedString1x(x + (bump + row * 0x25) + 5,
                                     y + cmdSlot * 0xd + 6,
                                     (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, statBlock->commands[cmdSlot + row * 4].commandId, 8),
                                     7, 0.1f);
            }
        }
    }
    Menu_SetRectShorts(rect, x, y, g_MenuStatusTabWidths[n - 1],
                  g_MenuStatusTabHeights[3]);
    Menu_DrawWindowFrame1x((EnglishWindowFrame *)rect, 0.1f);
}

/*
 * Empty no-op hook for status screen.
 */
// FUNCTION: FF7 0x00705c71
void __cdecl Menu_UnusedStatusScreenNoOp(void)
{
    /* The original is empty. Its whole body is
       `55 8b ec 5d c3` -- 15 bytes at 0x00705c71,
       push/mov/pop/ret plus alignment padding.
       The C body is empty, as in retail. */
}
} /* extern "C" */
