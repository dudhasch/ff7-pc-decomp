// src/ff7/src/menu/btlmenu/english/battle_hud_lists.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Draws the battle HUD: the party HP and MP gauges, status labels and party overview, and
//   the command, item, magic, limit and summon lists at both resolutions. It also holds the Cait
//   Sith and Tifa reel panels and the slot mini-game drawing and penalty logic.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include "ff7/api/menu.h"
#include "lib/api/file.h"
#include "ff7/types/menu.h"
#include "lib/enums/file.h"
#include "ff7/enums/menu.h"
#include <string.h>
#include <stdlib.h>
#include "lib/api/sound.h"
#include "ff7/savemap.h"
#include "lib/api/graphics.h"
#include "ff7/enums/main.h"
#include "ff7/enums/battle.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
extern "C" {

#pragma intrinsic(abs, memcpy, memset)
// clang-format off
// Text (32 x 10-byte status names, starting in g_BtlStatusNameTable):
//   [2] "Sleep" [3] "Poison" [4] "Sadness" [5] "Fury" [6] "Confusion" [7] "Silence"
//   [8] "Haste" [9] "Slow" [10] "Stop" [14] "Petrify" [15] "Regen" [18] "Reflect"
//   [19] "Dual" [20] "Shield" [23] "Berserk" [24] "Peerless" [25] "Paralyze" [26] "Darkness"
//   (remaining entries are empty 0xFF strings).
// GLOBAL: FF7 0x0091e828
int g_BtlStatusNameTable = 255;
unsigned char g_rawData_0091e830[312] = {
0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x4c, 0x45, 0x45,
0x50, 0xff, 0x00, 0x00, 0x00, 0x00, 0x30, 0x4f, 0x49, 0x53, 0x4f, 0x4e, 0xff, 0x00, 0x00, 0x00,
0x33, 0x41, 0x44, 0x4e, 0x45, 0x53, 0x53, 0xff, 0x00, 0x00, 0x26, 0x55, 0x52, 0x59, 0xff, 0x00,
0x00, 0x00, 0x00, 0x00, 0x23, 0x4f, 0x4e, 0x46, 0x55, 0x53, 0x49, 0x4f, 0x4e, 0xff, 0x33, 0x49,
0x4c, 0x45, 0x4e, 0x43, 0x45, 0xff, 0x00, 0x00, 0x28, 0x41, 0x53, 0x54, 0x45, 0xff, 0x00, 0x00,
0x00, 0x00, 0x33, 0x4c, 0x4f, 0x57, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x54, 0x4f, 0x50,
0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x30, 0x45, 0x54, 0x52, 0x49, 0x46, 0x59, 0xff, 0x00, 0x00, 0x32, 0x45,
0x47, 0x45, 0x4e, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x45, 0x46, 0x4c,
0x45, 0x43, 0x54, 0xff, 0x00, 0x00, 0x24, 0x55, 0x41, 0x4c, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
0x33, 0x48, 0x49, 0x45, 0x4c, 0x44, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x45,
0x52, 0x53, 0x45, 0x52, 0x4b, 0xff, 0x00, 0x00, 0x30, 0x45, 0x45, 0x52, 0x4c, 0x45, 0x53, 0x53,
0xff, 0x00, 0x30, 0x41, 0x52, 0x41, 0x4c, 0x59, 0x5a, 0x45, 0xff, 0x00, 0x24, 0x41, 0x52, 0x4b,
0x4e, 0x45, 0x53, 0x53, 0xff, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0091e968
short g_BtlGaugePulseStep = 4;
// The party-status blink colours: Battle_DrawPartyStatusOverview1x/2x take ((unsigned int *)g_BtlGaugeStatusColors)[(t >> 1) &
// 7] (the eight words run on into g_BtlLimitMenuTables) or words 0 and 2.
// GLOBAL: FF7 0x0091e970
unsigned int g_BtlGaugeStatusColors[4] = {0x80000080, 0x80800080, 0x80800000, 0x80804000};
// GLOBAL: FF7 0x0091e980
BtlLimitMenuTables g_BtlLimitMenuTables = {{0, 0x80, 0x80, 0x80, 0, 0x80, 0x40, 0x80, 0, 0x80, 0, 0x80, 0x80, 0x80, 0}, {0x80, 0x32, 0x52, 0x72}, {0x00, 0x40, 0x48, 0x50}, {0x58, 0x19, 0x29, 0x39}, {0}};
// The battle gil prompt's texts (Battle_DrawGilAmountPrompt2x/1x), FF7-encoded and ended by 0xFF; as byte
// arrays, because a string literal would end at the first space byte (0x00).
// Text: "How much will you raise?"
// GLOBAL: FF7 0x0091e9a0
unsigned char g_BtlSquareGilPromptText[25] = {0x28, 0x4f, 0x57, 0x00, 0x4d, 0x55, 0x43, 0x48, 0x00, 0x57, 0x49, 0x4c, 0x4c, 0x00, 0x59, 0x4f, 0x55, 0x00, 0x52, 0x41, 0x49, 0x53, 0x45, 0x1f, 0xff};
// Text: "Gil"
// GLOBAL: FF7 0x0091e9bc
unsigned char g_BtlSquareGilText[4] = {0x27, 0x49, 0x4c, 0xff};
// Text: "After"
// GLOBAL: FF7 0x0091e9c0
unsigned char g_BtlSquareAfterText[6] = {0x21, 0x46, 0x54, 0x45, 0x52, 0xff};
// Text: "Gil on hand"
// GLOBAL: FF7 0x0091e9c8
unsigned char g_BtlSquareGilOnHandText[12] = {0x27, 0x49, 0x4c, 0x00, 0x4f, 0x4e, 0x00, 0x48, 0x41, 0x4e, 0x44, 0xff};
// The Cait Sith slot reels (Menu_DrawCaitReelPanel1x/2x and the stop logic): g_BtlSquareData1 the symbol of each of the 16
// stops of the three reels, g_BtlSquareData2 the atlas (u, v) of each symbol per reel at 1x (+0) and 2x (+0x60), and
// g_BtlSquareData3 the reel speeds.
// GLOBAL: FF7 0x0091e9d8
unsigned char g_BtlSquareData1[48] = {
    0x02, 0x05, 0x04, 0x03, 0x01, 0x02, 0x04, 0x00, 0x01, 0x03, 0x05, 0x02, 0x04, 0x00, 0x05, 0x01,
    0x01, 0x02, 0x04, 0x00, 0x02, 0x01, 0x03, 0x05, 0x04, 0x00, 0x02, 0x01, 0x05, 0x04, 0x02, 0x03,
    0x02, 0x04, 0x03, 0x02, 0x01, 0x04, 0x00, 0x05, 0x02, 0x01, 0x03, 0x02, 0x04, 0x01, 0x02, 0x05
};
// GLOBAL: FF7 0x0091ea08
unsigned char g_BtlSquareData2[0xc0] = {
    0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x70, 0x00, 0x20, 0x00, 0x70, 0x00, 0x40, 0x00, 0x70, 0x00,
    0x00, 0x00, 0xa0, 0x00, 0x20, 0x00, 0xa0, 0x00, 0x40, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x20, 0x00, 0x40, 0x00, 0x00, 0x00, 0x70, 0x00, 0x20, 0x00, 0x70, 0x00, 0x40, 0x00, 0x70, 0x00,
    0x00, 0x00, 0xa0, 0x00, 0x20, 0x00, 0xa0, 0x00, 0x40, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x00, 0x40, 0x00, 0x00, 0x00, 0x70, 0x00, 0x20, 0x00, 0x70, 0x00, 0x40, 0x00, 0x70, 0x00,
    0x00, 0x00, 0xa0, 0x00, 0x20, 0x00, 0xa0, 0x00, 0x40, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x40, 0x00, 0x60, 0x00, 0x80, 0x00, 0x60, 0x00,
    0x00, 0x00, 0xc0, 0x00, 0x40, 0x00, 0xc0, 0x00, 0x80, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x40, 0x00, 0x60, 0x00, 0x80, 0x00, 0x60, 0x00,
    0x00, 0x00, 0xc0, 0x00, 0x40, 0x00, 0xc0, 0x00, 0x80, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x40, 0x00, 0x60, 0x00, 0x80, 0x00, 0x60, 0x00,
    0x00, 0x00, 0xc0, 0x00, 0x40, 0x00, 0xc0, 0x00, 0x80, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00
};
// GLOBAL: FF7 0x0091eac8
unsigned char g_BtlSquareData3[8] = {
    0x02, 0x04, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00
};
// Battle Square slot-reel handicap table (0x00..0xB9) followed at +0xBA by 31 FF7-encoded strings:
//   "Keep goin'?", "Of course!     No way!", "Current Battle Points", "Slot start!", "GREAT!!",
//   "Earned Battle Points", "Then, go for it!", "Magic Materia is broken.", "Summon Materia is broken.",
//   "Support Materia is broken.", "Independent Materia is broken.", "Command Materia is broken.",
//   "All Materia is broken.", "Accessory is broken.", "Item command is sealed.", "Armor is broken.",
//   "Weapon is broken.", "1/2 speed.", "1/2 accuracy.", "Minimum", "Poison", "Toad",
//   "Time x30 damage.", "Down 5 levels.", "Down 10 levels.", "1/2 HP.", "1/2 MP.", "1/2 HP&MP.",
//   "Zero MP.", "Yesss! No handicapp!", "HP restored."
// GLOBAL: FF7 0x0091ead0
unsigned char g_BtlSquareHandicapTexts[1112] = {0x01, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x02, 0x02, 0x02, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x02, 0x02, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x02, 0x02, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x02, 0x00, 0x02, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x09, 0x08, 0x09, 0x09, 0x08, 0x00, 0x0d, 0x0a, 0x0b, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0c, 0x2b, 0x45, 0x45, 0x50, 0x00, 0x47, 0x4f, 0x49, 0x4e, 0x07, 0x1f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x2f, 0x46, 0x00, 0x43, 0x4f, 0x55, 0x52, 0x53, 0x45, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2e, 0x4f, 0x00, 0x57, 0x41, 0x59, 0x01, 0xff, 0x00, 0x23, 0x55, 0x52, 0x52, 0x45, 0x4e, 0x54, 0x00, 0x22, 0x41, 0x54, 0x54, 0x4c, 0x45, 0x00, 0x30, 0x4f, 0x49, 0x4e, 0x54, 0x53, 0xff, 0x33, 0x4c, 0x4f, 0x54, 0x00, 0x53, 0x54, 0x41, 0x52, 0x54, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x27, 0x32, 0x25, 0x21, 0x34, 0x01, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x41, 0x52, 0x4e, 0x45, 0x44, 0x00, 0x22, 0x41, 0x54, 0x54, 0x4c, 0x45, 0x00, 0x30, 0x4f, 0x49, 0x4e, 0x54, 0x53, 0xff, 0x00, 0x34, 0x48, 0x45, 0x4e, 0x0c, 0x00, 0x47, 0x4f, 0x00, 0x46, 0x4f, 0x52, 0x00, 0x49, 0x54, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2d, 0x41, 0x47, 0x49, 0x43, 0x00, 0x2d, 0x41, 0x54, 0x45, 0x52, 0x49, 0x41, 0x00, 0x49, 0x53, 0x00, 0x42, 0x52, 0x4f, 0x4b, 0x45, 0x4e, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x55, 0x4d, 0x4d, 0x4f, 0x4e, 0x00, 0x2d, 0x41, 0x54, 0x45, 0x52, 0x49, 0x41, 0x00, 0x49, 0x53, 0x00, 0x42, 0x52, 0x4f, 0x4b, 0x45, 0x4e, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x55, 0x50, 0x50, 0x4f, 0x52, 0x54, 0x00, 0x2d, 0x41, 0x54, 0x45, 0x52, 0x49, 0x41, 0x00, 0x49, 0x53, 0x00, 0x42, 0x52, 0x4f, 0x4b, 0x45, 0x4e, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x4e, 0x44, 0x45, 0x50, 0x45, 0x4e, 0x44, 0x45, 0x4e, 0x54, 0x00, 0x2d, 0x41, 0x54, 0x45, 0x52, 0x49, 0x41, 0x00, 0x49, 0x53, 0x00, 0x42, 0x52, 0x4f, 0x4b, 0x45, 0x4e, 0x0e, 0xff, 0x00, 0x23, 0x4f, 0x4d, 0x4d, 0x41, 0x4e, 0x44, 0x00, 0x2d, 0x41, 0x54, 0x45, 0x52, 0x49, 0x41, 0x00, 0x49, 0x53, 0x00, 0x42, 0x52, 0x4f, 0x4b, 0x45, 0x4e, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x4c, 0x4c, 0x00, 0x2d, 0x41, 0x54, 0x45, 0x52, 0x49, 0x41, 0x00, 0x49, 0x53, 0x00, 0x42, 0x52, 0x4f, 0x4b, 0x45, 0x4e, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x43, 0x43, 0x45, 0x53, 0x53, 0x4f, 0x52, 0x59, 0x00, 0x49, 0x53, 0x00, 0x42, 0x52, 0x4f, 0x4b, 0x45, 0x4e, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x54, 0x45, 0x4d, 0x00, 0x43, 0x4f, 0x4d, 0x4d, 0x41, 0x4e, 0x44, 0x00, 0x49, 0x53, 0x00, 0x53, 0x45, 0x41, 0x4c, 0x45, 0x44, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x52, 0x4d, 0x4f, 0x52, 0x00, 0x49, 0x53, 0x00, 0x42, 0x52, 0x4f, 0x4b, 0x45, 0x4e, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x37, 0x45, 0x41, 0x50, 0x4f, 0x4e, 0x00, 0x49, 0x53, 0x00, 0x42, 0x52, 0x4f, 0x4b, 0x45, 0x4e, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x0f, 0x12, 0x00, 0x53, 0x50, 0x45, 0x45, 0x44, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x0f, 0x12, 0x00, 0x41, 0x43, 0x43, 0x55, 0x52, 0x41, 0x43, 0x59, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2d, 0x49, 0x4e, 0x49, 0x4d, 0x55, 0x4d, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x4f, 0x49, 0x53, 0x4f, 0x4e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x4f, 0x41, 0x44, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x49, 0x4d, 0x45, 0x00, 0x58, 0x13, 0x10, 0x00, 0x44, 0x41, 0x4d, 0x41, 0x47, 0x45, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x4f, 0x57, 0x4e, 0x00, 0x15, 0x00, 0x4c, 0x45, 0x56, 0x45, 0x4c, 0x53, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x4f, 0x57, 0x4e, 0x00, 0x11, 0x10, 0x00, 0x4c, 0x45, 0x56, 0x45, 0x4c, 0x53, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x0f, 0x12, 0x00, 0x28, 0x30, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x0f, 0x12, 0x00, 0x2d, 0x30, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x0f, 0x12, 0x00, 0x28, 0x30, 0x06, 0x2d, 0x30, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3a, 0x45, 0x52, 0x4f, 0x00, 0x2d, 0x30, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0x45, 0x53, 0x53, 0x53, 0x01, 0x00, 0x2e, 0x4f, 0x00, 0x48, 0x41, 0x4e, 0x44, 0x49, 0x43, 0x41, 0x50, 0x50, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x30, 0x00, 0x52, 0x45, 0x53, 0x54, 0x4f, 0x52, 0x45, 0x44, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// Three encoded strings, 0x22 bytes each, 0x00 for space and 0xFF to end: "Worried about the other members?",
// "Yeah, a little", "Not really". Battle_DrawWorriedChoiceLines1x/2x draw them by row.
// GLOBAL: FF7 0x0091ef28
unsigned char g_BtlSquareHandicapTbl[3][34] = {
    {0x37, 0x4f, 0x52, 0x52, 0x49, 0x45, 0x44, 0x00, 0x41, 0x42, 0x4f, 0x55, 0x54, 0x00, 0x54, 0x48, 0x45, 0x00, 0x4f, 0x54, 0x48, 0x45, 0x52, 0x00, 0x4d, 0x45, 0x4d, 0x42, 0x45, 0x52, 0x53, 0x1f, 0xff, 0x00},
    {0x39, 0x45, 0x41, 0x48, 0x0c, 0x00, 0x41, 0x00, 0x4c, 0x49, 0x54, 0x54, 0x4c, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x2e, 0x4f, 0x54, 0x00, 0x52, 0x45, 0x41, 0x4c, 0x4c, 0x59, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};
// GLOBAL: FF7 0x00dc3900
struct CaitReelSymbolState g_CaitReelSymbolState = {0};
unsigned char g_bssPad_00dc3904[4] = {0};
// GLOBAL: FF7 0x00dc3908
int g_BtlGaugeHpHigh[10] = {0};
// GLOBAL: FF7 0x00dc3930
unsigned char g_BtlGaugeHpDir[16] = {0};
unsigned char g_bssPad_00dc3940[256] = {0};
// GLOBAL: FF7 0x00dc3a40
short g_BtlGaugeUnusedSlotWordsA[10] = {0};
unsigned char g_bssPad_00dc3a54[4] = {0};
// GLOBAL: FF7 0x00dc3a58
unsigned char g_TifaReelEntryIndex[12] = {0};
unsigned char g_bssPad_00dc3a64[4] = {0};
// GLOBAL: FF7 0x00dc3a68
int g_BtlGaugeMpLow[10] = {0};
// GLOBAL: FF7 0x00dc3a90
unsigned char g_BtlGaugeMpDir[16] = {0};
// GLOBAL: FF7 0x00dc3aa0
int g_BtlGaugeHpValues[10] = {0};
// GLOBAL: FF7 0x00dc3ac8
float g_CaitReelPanelVSize = 0;
// GLOBAL: FF7 0x00dc3acc
float g_CaitReelPanelUSize = 0;
// GLOBAL: FF7 0x00dc3ad0
int g_BtlGaugeMpValues[10] = {0};
// GLOBAL: FF7 0x00dc3af8
BtlGaugeReelState g_BtlGaugeReelState = {0};
unsigned char g_bssPad_00dc3afc[4] = {0};
// GLOBAL: FF7 0x00dc3b00
int g_BtlGaugeAnimStates[22] = {0};
// GLOBAL: FF7 0x00dc3b58
short g_BtlGaugeUnusedSlotWordsB[10] = {0};
// GLOBAL: FF7 0x00dc3b6c
unsigned char g_BtlReelSpeedIndex = 0;
// GLOBAL: FF7 0x00dc3b70
unsigned char g_BtlReelCountdown = 0;
unsigned char g_bssPad_00dc3b74[4] = {0};
// GLOBAL: FF7 0x00dc3b78
int g_BtlGaugeMpHigh[10] = {0};
// GLOBAL: FF7 0x00dc3ba0
char g_BtlHandicapFlags = 0;
// GLOBAL: FF7 0x00dc3ba4
float g_CaitReelPanelU = 0;
// GLOBAL: FF7 0x00dc3ba8
float g_CaitReelPanelV = 0;
// GLOBAL: FF7 0x00dc3bac
unsigned char g_TifaReelCount = 0;
// GLOBAL: FF7 0x00dc3bb0
int g_BtlGaugeHpLow[10] = {0};
// GLOBAL: FF7 0x00dc3bd8
unsigned char g_CaitReelActive = 0;
// GLOBAL: FF7 0x00dc3bdc
int g_BtlSelectedSpellAllCount = 0;
// GLOBAL: FF7 0x00dc3be0
int g_BtlSelectedSpellQuadraLinks = 0;
// GLOBAL: FF7 0x00dc3be4
short g_PartyGaugeAnimPos = 0;
// GLOBAL: FF7 0x00dc3be8
short g_BtlGaugeSeeded = 0;
unsigned char g_bssPad_00dc3bec[4] = {0};
// GLOBAL: FF7 0x00dc3bf0
int g_BattleHudStatusIconCycle = 0;
#define g_BattleHudStatusIconCycles (&g_BattleHudStatusIconCycle)
// GLOBAL: FF7 0x00dc3bf4
int g_BattleHudStatusIconCycle1 = 0;
// GLOBAL: FF7 0x00dc3bf8
int g_BattleHudStatusIconCycle2 = 0;
unsigned char g_bssPad_00dc3bfc[4] = {0};
// GLOBAL: FF7 0x00dc3c00
short g_BtlReelPositions[12] = {0};
// GLOBAL: FF7 0x00dc3c18
unsigned char g_BtlGaugePartySlots[12] = {0};
// GLOBAL: FF7 0x00dc3c24
unsigned char g_BtlReelStopIndex = 0;
// GLOBAL: FF7 0x00dc3c28
unsigned char g_BtlSlotSubState = 0;
unsigned char g_bssPad_00dc3c2c[4] = {0};
// GLOBAL: FF7 0x00dc3c30
short g_TifaReelScrollSteps[10] = {0};
unsigned char g_bssPad_00dc3c44[4] = {0};
// GLOBAL: FF7 0x00dc3c48
unsigned char g_TifaReelStoppedFlags[12] = {0};
// clang-format on

// This TU passes the render object as a pointer; lib/api/graphics.h declares the parameter as int.

/*
 * Find the party slot holding character id 0 and return its HP scaled to 0xFFFF.
 * Zero if that character is not in the party.
 */
// FUNCTION: FF7 0x006dc910
int __cdecl Menu_GetCharZeroHpRatio(void)
{
    int i;
    int ratio;

    for (i = 0; i < 3; i++) {
        if (gSavemap.party[i] == PARTY_CHAR_CLOUD) {
            ratio = g_MenuPartyStats[i].hp * 65535 /
                    g_MenuPartyStats[i].maxHp;
            return ratio;
        }
    }
    return 0;
}

/*
 * Draws one party slot's HP and MP gauges. The MenuGauge on the stack is filled twice in place,
 * in the original's order, not the struct's: high is written before max and direction. The
 * gauges show the slot's hp, maxHp, mp and maxMp from g_MenuPartyStats.
 */
// FUNCTION: FF7 0x006dc976
void __cdecl Battle_DrawPartyMemberHpMpGauges2x(short slot)
{
    MenuGauge bar;
    struct MenuPartyStats *stats;

    stats = g_MenuPartyStats;
    bar.x = 288;
    bar.y = (short)(slot * 32 + 364);
    bar.width = 120;
    bar.height = 2;
    bar.low = (short)(g_BtlGaugeHpLow[slot] >> 8);
    bar.high = (short)(g_BtlGaugeHpHigh[slot] >> 8);
    bar.max = stats[slot].maxHp;
    bar.direction = g_BtlGaugeHpDir[slot];
    bar.colour = 0x800080ff;
    Menu_DrawResourceGauge2x((MenuGauge *)&bar, 0.6f);
    bar.x = 414;
    bar.y = (short)(slot * 32 + 364);
    bar.width = 60;
    bar.height = 2;
    bar.low = (short)(g_BtlGaugeMpLow[slot] >> 8);
    bar.high = (short)(g_BtlGaugeMpHigh[slot] >> 8);
    bar.max = stats[slot].maxMp;
    bar.direction = g_BtlGaugeMpDir[slot];
    bar.colour = 0x8000ff80;
    Menu_DrawResourceGauge2x((MenuGauge *)&bar, 0.6f);
}

/*
 * Battle_DrawPartyMemberHpMpGauges2x's 1x twin: every position and size halved, and
 * Menu_DrawResourceGauge1x for the drawing.
 */
// FUNCTION: FF7 0x006dca90
void __cdecl Battle_DrawPartyMemberHpMpGauges1x(short slot)
{
    MenuGauge bar;
    struct MenuPartyStats *stats;

    stats = g_MenuPartyStats;
    bar.x = 144;
    bar.y = (short)(slot * 16 + 182);
    bar.width = 60;
    bar.height = 1;
    bar.low = (short)(g_BtlGaugeHpLow[slot] >> 8);
    bar.high = (short)(g_BtlGaugeHpHigh[slot] >> 8);
    bar.max = stats[slot].maxHp;
    bar.direction = g_BtlGaugeHpDir[slot];
    bar.colour = 0x800080ff;
    Menu_DrawResourceGauge1x((MenuGauge *)&bar, 0.6f);
    bar.x = 207;
    bar.y = (short)(slot * 16 + 182);
    bar.width = 30;
    bar.height = 1;
    bar.low = (short)(g_BtlGaugeMpLow[slot] >> 8);
    bar.high = (short)(g_BtlGaugeMpHigh[slot] >> 8);
    bar.max = stats[slot].maxMp;
    bar.direction = g_BtlGaugeMpDir[slot];
    bar.colour = 0x8000ff80;
    Menu_DrawResourceGauge1x((MenuGauge *)&bar, 0.6f);
}

/*
 * Per frame, steps each of the three party members' displayed HP
 * (g_BtlGaugeHpValues[i]) and MP (g_BtlGaugeMpValues[i])
 * toward the current values in g_MenuPartyStats (hp and mp, shifted left 8) by max/0xF0 per
 * step, and fills the bar
 * descriptors
 * g_BtlGaugeHpLow/g_BtlGaugeHpHigh/g_BtlGaugeHpDir
 * (HP) and
 * g_BtlGaugeMpLow/g_BtlGaugeMpHigh/g_BtlGaugeMpDir
 * (MP) with low, high and direction 2/1/0; the first call seeds the displayed
 * values (gated by g_BtlGaugeSeeded), the pulse g_PartyGaugeAnimPos
 * bounces by g_BtlGaugePulseStep between 0 and 0xB0, and slots whose
 * savemap character id gSavemap.chars[i].id is 0xFF are skipped.
 */
// FUNCTION: FF7 0x006dcbaa
void __cdecl Battle_StepPartyHpMpGaugeDisplay(void)
{
    int t;
    struct MenuPartyStats *stats;
    short member;

    stats = g_MenuPartyStats;
    if (g_BtlGaugeSeeded == 0) {
        for (member = 0; member < 3; member++) {
            g_BtlGaugeHpValues[member] = stats[member].hp << 8;
            g_BtlGaugeMpValues[member] = stats[member].mp << 8;
        }
        g_BtlGaugeSeeded = 1;
    }
    g_PartyGaugeAnimPos += g_BtlGaugePulseStep;
    if (g_PartyGaugeAnimPos > 176) {
        g_BtlGaugePulseStep = 0 - g_BtlGaugePulseStep;
    }
    if (g_PartyGaugeAnimPos < 0) {
        g_PartyGaugeAnimPos = 0;
        g_BtlGaugePulseStep = 0 - g_BtlGaugePulseStep;
    }
    for (member = 0; member < 3; member++) {
        if (gSavemap.chars[member].id == PARTY_CHAR_NONE) {
            continue;
        }
        t = stats[member].hp << 8;
        if (g_BtlGaugeHpValues[member] > t) {
            g_BtlGaugeHpValues[member] -= (stats[member].maxHp << 8) / 240;
            if (g_BtlGaugeHpValues[member] < t) {
                g_BtlGaugeHpValues[member] = stats[member].hp << 8;
            }
            g_BtlGaugeHpLow[member] = t;
            g_BtlGaugeHpHigh[member] = g_BtlGaugeHpValues[member];
            g_BtlGaugeHpDir[member] = 2;
        } else if (g_BtlGaugeHpValues[member] < t) {
            g_BtlGaugeHpValues[member] += (stats[member].maxHp << 8) / 240;
            if (g_BtlGaugeHpValues[member] > t) {
                g_BtlGaugeHpValues[member] = stats[member].hp << 8;
            }
            g_BtlGaugeHpLow[member] = g_BtlGaugeHpValues[member];
            g_BtlGaugeHpHigh[member] = t;
            g_BtlGaugeHpDir[member] = 1;
        } else {
            g_BtlGaugeHpLow[member] = t;
            g_BtlGaugeHpHigh[member] = t;
            g_BtlGaugeHpDir[member] = 0;
        }
        t = stats[member].mp << 8;
        if (g_BtlGaugeMpValues[member] > t) {
            g_BtlGaugeMpValues[member] -= (stats[member].maxMp << 8) / 240;
            if (g_BtlGaugeMpValues[member] < t) {
                g_BtlGaugeMpValues[member] = stats[member].mp << 8;
            }
            g_BtlGaugeMpLow[member] = t;
            g_BtlGaugeMpHigh[member] = g_BtlGaugeMpValues[member];
            g_BtlGaugeMpDir[member] = 2;
        } else if (g_BtlGaugeMpValues[member] < t) {
            g_BtlGaugeMpValues[member] += (stats[member].maxMp << 8) / 240;
            if (g_BtlGaugeMpValues[member] > t) {
                g_BtlGaugeMpValues[member] = stats[member].mp << 8;
            }
            g_BtlGaugeMpLow[member] = g_BtlGaugeMpValues[member];
            g_BtlGaugeMpHigh[member] = t;
            g_BtlGaugeMpDir[member] = 1;
        } else {
            g_BtlGaugeMpHigh[member] = t;
            g_BtlGaugeMpLow[member] = t;
            g_BtlGaugeMpDir[member] = 0;
        }
    }
}

/*
 * Draw one status name at x 0xB0 for the 640x480 layout. The names are ten bytes
 * apart in the table at g_BtlStatusNameTable.
 */
// FUNCTION: FF7 0x006dcff8
void __cdecl Battle_DrawStatusNameLabel2x(int y, int statusBitIndex)
{
    Menu_DrawFixedPitchString2x(176, y, (unsigned char *)(statusBitIndex * 0xA + (int)&g_BtlStatusNameTable), 3, 0);
}

/*
 * The 320x240 twin of Battle_DrawStatusNameLabel2x, at x 0x58.
 */
// FUNCTION: FF7 0x006dd01e
void __cdecl Battle_DrawStatusNameLabel1x(int y, int statusBitIndex)
{
    Menu_DrawFixedPitchString1x(88, y, (unsigned char *)(statusBitIndex * 0xA + (int)&g_BtlStatusNameTable), 3, 0);
}

/*
 * Draws the double-resolution battle party overview headings, then occupied rows
 * at 32-pixel pitch unless one of six specified panels is active and button 0x80
 * is not held. Destructively masks status words, cycles their selected bits and
 * calls the per-row status helper.
 */
// FUNCTION: FF7 0x006dd041
void Battle_DrawPartyStatusOverview2x(void)
{
    // Offsets include the unused locals visible in the original 0x3c-byte frame.
    struct {
        int pen;
        unsigned int colour;
        int unused30[2];
        short flag;
        short align2a;
        int unused28;
        short letter;
        short align22;
        int unused1c[2];
        short i;
        short align16;
        int unused08[4];
        struct MenuPartyStats *stats;
    } v; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    v.stats = g_MenuPartyStats;
    if (*(unsigned short *)&g_BattleAtbFrozen != 0) {
        Menu_DrawAtlasQuad2x(558, 338, 0x60, 8, 24, 5, 1, 0, 0, 0.6f);
    } else {
        Menu_DrawAtlasQuad2x(558, 338, 0x70, 0, 24, 5, 1, 0, 0, 0.6f);
    }
    Menu_DrawAtlasQuad2x(28, 338, 0x80, 0x10, 26, 5, 1, 0, 0, 0.6f);
    Menu_DrawAtlasQuad2x(288, 338, 0x50, 0, 16, 5, 1, 0, 0, 0.6f);
    Menu_DrawAtlasQuad2x(420, 338, 0x60, 0, 16, 5, 1, 0, 0, 0.6f);
    Menu_DrawAtlasQuad2x(480, 338, 0x38, 0, 24, 5, 1, 0, 0, 0.6f);
    Menu_DrawAtlasQuad2x(186, 338, 0x38, 8, 38, 5, 1, 0, 0, 0.6f);
    if (((short)Battle_GetWindowState(BATTLE_MENU_ITEM_LIST) != 2 && (short)Battle_GetWindowState(BATTLE_MENU_MAGIC_LIST) != 2 &&
         (short)Battle_GetWindowState(BATTLE_MENU_ENEMY_SKILL_LIST) != 2 && (short)Battle_GetWindowState(BATTLE_MENU_SUMMON_LIST) != 2 &&
         (short)Battle_GetWindowState(BATTLE_MENU_TIFA_SLOTS) != 2 && (short)Battle_GetWindowState(BATTLE_MENU_CAIT_SITH_SLOTS) != 2) ||
        Input_GetHeld(INPUT_SQUARE) != 0) {
        for (v.i = 0; v.i < 3; v.i++) {
            if (gSavemap.party[v.i] == PARTY_CHAR_NONE) {
                continue;
            }
            g_BattlePartyStatusIconMasks[v.i] &= ~BATTLE_STATUS_HUD_NON_CYCLING_MASK;
            if (((g_BtlMenuFrameCounter & 0xf) == 0 && g_BattlePartyStatusIconMasks[v.i] != 0) ||
                ((((unsigned int *)&g_BattlePartyStatusIconMasks)[v.i] &
                  (1 << g_BattleHudStatusIconCycles[v.i])) == 0 &&
                 g_BattlePartyStatusIconMasks[v.i] != 0)) {
                do {
                    g_BattleHudStatusIconCycles[v.i]++;
                    g_BattleHudStatusIconCycles[v.i] %= 27;
                } while ((((unsigned int *)&g_BattlePartyStatusIconMasks)[v.i] &
                          (1 << g_BattleHudStatusIconCycles[v.i])) == 0);
            }
            v.flag = (g_BattleScene.actors[v.i].statusFlags & BATTLE_SCENE_ACTOR_KO) == 0;
            Battle_DrawPartyMemberHpMpGauges2x(v.i);
            if (g_MenuPartyStats[v.i].hp == 7777) {
                v.pen = 14;
                for (v.letter = 0; v.letter < 13; v.letter++) {
                    if (gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[v.i]]].name[v.letter] == 0xff) {
                        break;
                    }
                    v.pen = Menu_DrawFontGlyph2x(v.pen, v.i * 0x20 + 0x15c,
                                            ((g_BtlMenuFrameCounter >> 3) - v.letter) & 7,
                                            gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[v.i]]].name[v.letter], 0.601f);
                }
            } else {
                if ((short)Battle_GetWindowState(BATTLE_MENU_COMMAND_MENU) == 2 && g_BtlActiveActor == v.i && (short)Battle_GetWindowState(BATTLE_MENU_LIMIT_LIST) != 2) {
                    Menu_DrawEncodedString2x(28, v.i * 0x20 + 0x15c,
                                      (unsigned char *)&gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[v.i]]].name,
                                      ((g_BtlMenuFrameCounter >> 3) & 1) ? 0 : 7, 0.601f);
                } else {
                    Menu_DrawEncodedString2x(28, v.i * 0x20 + 0x15c,
                                      (unsigned char *)&gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[v.i]]].name,
                                      ((v.flag ? -1 : 0) & 5U) + 2, 0.601f);
                }
            }
            if ((unsigned short)v.stats[v.i].atbGauge == 0xffff) {
                if ((short)Battle_GetWindowState(BATTLE_MENU_COMMAND_MENU) == 2 && g_BtlActiveActor == v.i) {
                    Menu_DrawGaugeBar2x(560, v.i * 32 + 0x160,
                                        (unsigned short)v.stats[v.i].atbGauge / 1024, 12,
                                        (g_PartyGaugeAnimPos << 16) | ((g_PartyGaugeAnimPos / 2) << 8), 0.6f);
                } else {
                    Menu_DrawGaugeBar2x(560, v.i * 32 + 0x160,
                                        (unsigned short)v.stats[v.i].atbGauge / 1024, 12,
                                        0x80808020, 0.6f);
                }
            } else {
                v.colour = 0x80108040;
                Menu_DrawGaugeBar2x(560, v.i * 32 + 0x160,
                                    (unsigned short)v.stats[v.i].atbGauge / 1024, 12,
                                    v.colour, 0.6f);
            }
            Menu_DrawAtlasQuad2x(554, v.i * 32 + 0x15e, 0x10, 0, 40, 10, 1, 0, 0, 0.6f);
            if (g_BattleScene.readyActorMask & (1 << v.i)) {
                v.colour = g_BtlGaugeStatusColors[(g_BtlMenuFrameCounter >> 1) & 7];
            } else if (g_BattlePartyStatusIconMasks[v.i] & BATTLE_STATUS_SADNESS) {
                v.colour = g_BtlGaugeStatusColors[0];
            } else if (g_BattlePartyStatusIconMasks[v.i] & BATTLE_STATUS_FURY) {
                v.colour = g_BtlGaugeStatusColors[2];
            } else {
                v.colour = 0x80802050;
            }
            Menu_DrawGaugeBar2x(482, v.i * 32 + 0x160,
                                v.stats[v.i].limitGauge / 1024, 12, v.colour, 0.6f);
            Menu_DrawAtlasQuad2x(476, v.i * 32 + 0x15e, 0x10, 0, 40, 10, 1, 0, 0, 0.6f);
            if ((short)Battle_GetWindowState(BATTLE_MENU_COMMAND_MENU) != 2 || Input_GetHeld(INPUT_SQUARE) != 0) {
                Menu_DrawGaugeBar2x(196, v.i * 32 + 0x15e,
                                    (unsigned short)v.stats[v.i].barrierGauge >> 2, 8, 0x80800000, 0.6f);
                Menu_DrawGaugeBar2x(196, v.i * 32 + 0x168,
                                    (unsigned short)v.stats[v.i].mBarrierGauge >> 2, 8, 0x80804000, 0.6f);
                Menu_DrawAtlasQuad2x(190, v.i * 32 + 0x15c, 0xa0, 0x10, 38, 12, 1, 0, 0, 0.6f);
            }
            if (v.stats[g_BtlActiveActor].commandColumns < 2 ||
                (short)Battle_GetWindowState(BATTLE_MENU_COMMAND_MENU) != 2 || Input_GetHeld(INPUT_SQUARE) != 0) {
                if (v.flag != 0) {
                    Menu_DrawDecimalNumber2x(288, v.i * 32 + 0x15c, g_BtlGaugeHpValues[v.i] >> 8, 4,
                                      ((g_BtlGaugeHpValues[v.i] >> 8) >
                                       v.stats[v.i].maxHp / 4) +
                                          6,
                                      0.6f);
                } else {
                    Menu_DrawDecimalNumber2x(288, v.i * 32 + 0x15c, g_BtlGaugeHpValues[v.i] >> 8, 4, 2, 0.6f);
                }
                Menu_DrawAtlasQuad2x(344, v.i * 32 + 0x15c, 0xd8, 0, 4, 8, 1, 0, 0, 0.6f);
            }
            if (v.stats[g_BtlActiveActor].commandColumns < 3 ||
                (short)Battle_GetWindowState(BATTLE_MENU_COMMAND_MENU) != 2 || Input_GetHeld(INPUT_SQUARE) != 0) {
                Menu_DrawDecimalNumber2x(352, v.i * 0x20 + 0x15c,
                                  v.stats[v.i].maxHp, 4,
                                  ((v.flag ? -1 : 0) & 5U) + 2, 0.6f);
            }
            if (v.flag != 0) {
                Menu_DrawDecimalNumber2x(416, v.i * 32 + 0x15c, g_BtlGaugeMpValues[v.i] >> 8, 4,
                                  ((g_BtlGaugeMpValues[v.i] >> 8) >
                                   v.stats[v.i].maxMp / 4) +
                                      6,
                                  0.6f);
            } else {
                Menu_DrawDecimalNumber2x(416, v.i * 32 + 0x15c, g_BtlGaugeMpValues[v.i] >> 8, 4, 2, 0.6f);
            }
        }
    }
}

/*
 * Draws the low-resolution battle party overview headings, then occupied rows at
 * 16-pixel pitch unless one of six specified panels is active and button 0x80 is
 * not held. Destructively masks status words, cycles their selected bits and calls
 * the per-row status helper.
 */
// FUNCTION: FF7 0x006dda14
void Battle_DrawPartyStatusOverview1x(void)
{
    // Offsets include the unused locals visible in the original 0x3c-byte frame.
    struct {
        int pen;
        unsigned int colour;
        int unused30[2];
        short flag;
        short align2a;
        int unused28;
        short letter;
        short align22;
        int unused1c[2];
        short i;
        short align16;
        int unused08[4];
        struct MenuPartyStats *stats;
    } v; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    v.stats = g_MenuPartyStats;
    if (*(unsigned short *)&g_BattleAtbFrozen != 0) {
        Menu_DrawAtlasQuad1x(279, 169, 96, 8, 24, 5, 1, 0, 0, 0.6f);
    } else {
        Menu_DrawAtlasQuad1x(279, 169, 112, 0, 24, 5, 1, 0, 0, 0.6f);
    }
    Menu_DrawAtlasQuad1x(14, 169, 128, 16, 26, 5, 1, 0, 0, 0.6f);
    Menu_DrawAtlasQuad1x(144, 169, 80, 0, 16, 5, 1, 0, 0, 0.6f);
    Menu_DrawAtlasQuad1x(210, 169, 96, 0, 16, 5, 1, 0, 0, 0.6f);
    Menu_DrawAtlasQuad1x(240, 169, 56, 0, 24, 5, 1, 0, 0, 0.6f);
    Menu_DrawAtlasQuad1x(93, 169, 56, 8, 38, 5, 1, 0, 0, 0.6f);
    if (((short)Battle_GetWindowState(BATTLE_MENU_ITEM_LIST) != 2 && (short)Battle_GetWindowState(BATTLE_MENU_MAGIC_LIST) != 2 &&
         (short)Battle_GetWindowState(BATTLE_MENU_ENEMY_SKILL_LIST) != 2 && (short)Battle_GetWindowState(BATTLE_MENU_SUMMON_LIST) != 2 &&
         (short)Battle_GetWindowState(BATTLE_MENU_TIFA_SLOTS) != 2 && (short)Battle_GetWindowState(BATTLE_MENU_CAIT_SITH_SLOTS) != 2) ||
        Input_GetHeld(INPUT_SQUARE) != 0) {
        for (v.i = 0; v.i < 3; v.i++) {
            if (gSavemap.party[v.i] == PARTY_CHAR_NONE) {
                continue;
            }
            g_BattlePartyStatusIconMasks[v.i] &= ~BATTLE_STATUS_HUD_NON_CYCLING_MASK;
            if (((g_BtlMenuFrameCounter & 0xf) == 0 && g_BattlePartyStatusIconMasks[v.i] != 0) ||
                ((((unsigned int *)&g_BattlePartyStatusIconMasks)[v.i] &
                  (1 << g_BattleHudStatusIconCycles[v.i])) == 0 &&
                 g_BattlePartyStatusIconMasks[v.i] != 0)) {
                do {
                    g_BattleHudStatusIconCycles[v.i]++;
                    g_BattleHudStatusIconCycles[v.i] %= 27;
                } while ((((unsigned int *)&g_BattlePartyStatusIconMasks)[v.i] &
                          (1 << g_BattleHudStatusIconCycles[v.i])) == 0);
            }
            v.flag = (g_BattleScene.actors[v.i].statusFlags & BATTLE_SCENE_ACTOR_KO) == 0;
            Battle_DrawPartyMemberHpMpGauges1x(v.i);
            if (g_MenuPartyStats[v.i].hp == 7777) {
                v.pen = 14;
                for (v.letter = 0; v.letter < 13; v.letter++) {
                    if (gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[v.i]]].name[v.letter] == 0xff) {
                        break;
                    }
                    v.pen = Menu_DrawFontGlyph1x(v.pen, v.i * 0x10 + 0xae,
                                            ((g_BtlMenuFrameCounter >> 3) - v.letter) & 7,
                                            gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[v.i]]].name[v.letter], 0.601f);
                }
            } else {
                if ((short)Battle_GetWindowState(BATTLE_MENU_COMMAND_MENU) == 2 && g_BtlActiveActor == v.i && (short)Battle_GetWindowState(BATTLE_MENU_LIMIT_LIST) != 2) {
                    Menu_DrawEncodedString1x(14, v.i * 0x10 + 0xae,
                                         (unsigned char *)&gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[v.i]]].name,
                                         ((g_BtlMenuFrameCounter >> 3) & 1) ? 0 : 7, 0.601f);
                } else {
                    Menu_DrawEncodedString1x(14, v.i * 0x10 + 0xae,
                                         (unsigned char *)&gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[v.i]]].name,
                                         ((v.flag ? -1 : 0) & 5U) + 2, 0.601f);
                }
            }
            if ((unsigned short)v.stats[v.i].atbGauge == 0xffff) {
                if ((short)Battle_GetWindowState(BATTLE_MENU_COMMAND_MENU) == 2 && g_BtlActiveActor == v.i) {
                    Menu_DrawGaugeBar1x(280, v.i * 16 + 0xb0,
                                        (unsigned short)v.stats[v.i].atbGauge / 2048, 6,
                                        (g_PartyGaugeAnimPos << 16) | ((g_PartyGaugeAnimPos / 2) << 8), 0.6f);
                } else {
                    Menu_DrawGaugeBar1x(280, v.i * 16 + 0xb0,
                                        (unsigned short)v.stats[v.i].atbGauge / 2048, 6,
                                        0x80808020, 0.6f);
                }
            } else {
                v.colour = 0x80108040;
                Menu_DrawGaugeBar1x(280, v.i * 16 + 0xb0,
                                    (unsigned short)v.stats[v.i].atbGauge / 2048, 6,
                                    v.colour, 0.6f);
            }
            Menu_DrawAtlasQuad1x(277, v.i * 16 + 0xaf, 16, 0, 40, 10, 1, 0, 0, 0.6f);
            if (g_BattleScene.readyActorMask & (1 << v.i)) {
                v.colour = g_BtlGaugeStatusColors[(g_BtlMenuFrameCounter >> 1) & 7];
            } else if (g_BattlePartyStatusIconMasks[v.i] & BATTLE_STATUS_SADNESS) {
                v.colour = g_BtlGaugeStatusColors[0];
            } else if (g_BattlePartyStatusIconMasks[v.i] & BATTLE_STATUS_FURY) {
                v.colour = g_BtlGaugeStatusColors[2];
            } else {
                v.colour = 0x80802050;
            }
            Menu_DrawGaugeBar1x(241, v.i * 16 + 0xb0,
                                v.stats[v.i].limitGauge / 2048, 6, v.colour, 0.6f);
            Menu_DrawAtlasQuad1x(238, v.i * 16 + 0xaf, 16, 0, 40, 10, 1, 0, 0, 0.6f);
            if ((short)Battle_GetWindowState(BATTLE_MENU_COMMAND_MENU) != 2 || Input_GetHeld(INPUT_SQUARE) != 0) {
                Menu_DrawGaugeBar1x(98, v.i * 16 + 0xaf,
                                    (unsigned short)v.stats[v.i].barrierGauge >> 3, 4, 0x80800000, 0.6f);
                Menu_DrawGaugeBar1x(98, v.i * 16 + 0xb4,
                                    (unsigned short)v.stats[v.i].mBarrierGauge >> 3, 4, 0x80804000, 0.6f);
                Menu_DrawAtlasQuad1x(95, v.i * 16 + 0xae, 160, 16, 38, 12, 1, 0, 0, 0.6f);
            }
            if (v.stats[g_BtlActiveActor].commandColumns < 2 ||
                (short)Battle_GetWindowState(BATTLE_MENU_COMMAND_MENU) != 2 || Input_GetHeld(INPUT_SQUARE) != 0) {
                if (v.flag != 0) {
                    Menu_DrawDecimalNumber1x(144, v.i * 16 + 0xae, g_BtlGaugeHpValues[v.i] >> 8, 4,
                                        ((g_BtlGaugeHpValues[v.i] >> 8) >
                                         v.stats[v.i].maxHp / 4) +
                                            6,
                                        0.6f);
                } else {
                    Menu_DrawDecimalNumber1x(144, v.i * 16 + 0xae, g_BtlGaugeHpValues[v.i] >> 8, 4, 2, 0.6f);
                }
                Menu_DrawAtlasQuad1x(172, v.i * 16 + 0xae, 216, 0, 4, 8, 1, 0, 0, 0.6f);
            }
            if (v.stats[g_BtlActiveActor].commandColumns < 3 ||
                (short)Battle_GetWindowState(BATTLE_MENU_COMMAND_MENU) != 2 || Input_GetHeld(INPUT_SQUARE) != 0) {
                Menu_DrawDecimalNumber1x(176, v.i * 0x10 + 0xae,
                                    v.stats[v.i].maxHp, 4,
                                    ((v.flag ? -1 : 0) & 5U) + 2, 0.6f);
            }
            if (v.flag != 0) {
                Menu_DrawDecimalNumber1x(208, v.i * 16 + 0xae, g_BtlGaugeMpValues[v.i] >> 8, 4,
                                    ((g_BtlGaugeMpValues[v.i] >> 8) >
                                     v.stats[v.i].maxMp / 4) +
                                        6,
                                    0.6f);
            } else {
                Menu_DrawDecimalNumber1x(208, v.i * 16 + 0xae, g_BtlGaugeMpValues[v.i] >> 8, 4, 2, 0.6f);
            }
        }
    }
}

/*
 * Draws the selected actor command table as horizontal groups of four vertical
 * entries, unless any of six menu-state flags equals 2. Skips 0xFF IDs and chooses
 * color 0 or 7 from entry flag bit 2.
 */
// FUNCTION: FF7 0x006de3db
void __cdecl Battle_DrawActorCommandGrid2x(unsigned char *unused)
{
    struct MenuPartyStats *p;
    int palette;
    unsigned char *pName;
    int subEntryIndex;
    int t;
    int tempTextWidth;
    int group8;
    int w;
    int xPos8;

    p = g_MenuPartyStats;
    if ((short)Battle_GetWindowState(BATTLE_MENU_ITEM_LIST) != 2 && (short)Battle_GetWindowState(BATTLE_MENU_MAGIC_LIST) != 2 &&
        (short)Battle_GetWindowState(BATTLE_MENU_ENEMY_SKILL_LIST) != 2 && (short)Battle_GetWindowState(BATTLE_MENU_SUMMON_LIST) != 2 &&
        (short)Battle_GetWindowState(BATTLE_MENU_TIFA_SLOTS) != 2 && (short)Battle_GetWindowState(BATTLE_MENU_CAIT_SITH_SLOTS) != 2) {
        for (group8 = 0; group8 < p[g_BtlActiveActor].commandColumns; group8++) {
            for (w = 0; w < 4; w++) {
                xPos8 = group8 * 88 + (group8 != 0 ? 16 : 0) + 156;
                subEntryIndex = group8 * 4 + w;
                if (p[g_BtlActiveActor].commands[subEntryIndex].commandId != 0xFF) {
                    palette = (p[g_BtlActiveActor].commands[subEntryIndex].menuFlags & BATTLE_MENU_SLOT_GREYED) ? 0 : 7;
                    if (p[g_BtlActiveActor].commands[subEntryIndex].commandId == 0x14) {
                        pName = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, p[g_BtlActiveActor].commands[subEntryIndex].commandId, 8);
                        for (t = 0; t < 32; t++) {
                            if (pName[t] == 0xFF) {
                                break;
                            }
                            xPos8 = Menu_DrawFontGlyph2x(xPos8, w * 0x18 + 0x15E,
                                                ((g_BtlMenuFrameCounter >> 1) - t) & 7, pName[t], 0.501f);
                        }
                    } else if (p[g_BtlActiveActor].commands[subEntryIndex].commandId == 7) {
                        if (g_BtlMenuFrameCounter & 0x40) {
                            Menu_DrawEncodedString2x(group8 * 0x58 + (group8 != 0 ? 0x10 : 0) + 0x9C, w * 0x18 + 0x15E,
                                              (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, p[g_BtlActiveActor].commands[subEntryIndex].commandId, 8),
                                              palette, 0.501f);
                        } else {
                            Menu_DrawEncodedString2x(group8 * 0x58 + (group8 != 0 ? 0x10 : 0) + 0x9C, w * 0x18 + 0x15E,
                                              (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, 8, 8), palette, 0.501f);
                        }
                    } else {
                        Menu_DrawEncodedString2x(group8 * 0x58 + (group8 != 0 ? 0x10 : 0) + 0x9C, w * 0x18 + 0x15E,
                                          (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, p[g_BtlActiveActor].commands[subEntryIndex].commandId, 8), palette,
                                          0.501f);
                        tempTextWidth = Menu_MeasureEncodedString2x(
                            (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, p[g_BtlActiveActor].commands[subEntryIndex].commandId, 8));
                    }
                    if (p[g_BtlActiveActor].commands[subEntryIndex].menuFlags & BATTLE_MENU_SLOT_MULTI_ARROW) {
                        Menu_DrawAtlasQuad2x(group8 * 88 + (group8 != 0 ? 0x10 : 0) + tempTextWidth + 0x98,
                                             w * 24 + 0x162, 0x80, 8, 8, 8, 2, 0, 0, 0.501f);
                    }
                }
            }
        }
    }
}

/*
 * Draws the active actor command grid at 1x only when six subpanels are not in
 * state two. Skips empty commands, chooses enabled color, animates glyph colors
 * for command 0x14 and alternates command seven/eight by phase.
 */
// FUNCTION: FF7 0x006de7f4
void __cdecl Battle_DrawActorCommandGrid1x(unsigned char *unused)
{
    struct MenuPartyStats *p;
    int palette;
    unsigned char *pName;
    int subEntryIndex;
    int t;
    int tempTextWidth;
    int group8;
    int w;
    int xPos8;

    p = g_MenuPartyStats;
    if ((short)Battle_GetWindowState(BATTLE_MENU_ITEM_LIST) != 2 && (short)Battle_GetWindowState(BATTLE_MENU_MAGIC_LIST) != 2 &&
        (short)Battle_GetWindowState(BATTLE_MENU_ENEMY_SKILL_LIST) != 2 && (short)Battle_GetWindowState(BATTLE_MENU_SUMMON_LIST) != 2 &&
        (short)Battle_GetWindowState(BATTLE_MENU_TIFA_SLOTS) != 2 && (short)Battle_GetWindowState(BATTLE_MENU_CAIT_SITH_SLOTS) != 2) {
        for (group8 = 0; group8 < p[g_BtlActiveActor].commandColumns; group8++) {
            for (w = 0; w < 4; w++) {
                xPos8 = group8 * 44 + (group8 != 0 ? 8 : 0) + 78;
                subEntryIndex = group8 * 4 + w;
                if (p[g_BtlActiveActor].commands[subEntryIndex].commandId != 0xFF) {
                    palette = (p[g_BtlActiveActor].commands[subEntryIndex].menuFlags & BATTLE_MENU_SLOT_GREYED) ? 0 : 7;
                    if (p[g_BtlActiveActor].commands[subEntryIndex].commandId == 0x14) {
                        pName = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, p[g_BtlActiveActor].commands[subEntryIndex].commandId, 8);
                        for (t = 0; t < 32; t++) {
                            if (pName[t] == 0xFF) {
                                break;
                            }
                            xPos8 = Menu_DrawFontGlyph1x(xPos8, w * 0xC + 0xAF,
                                                ((g_BtlMenuFrameCounter >> 1) - t) & 7, pName[t], 0.501f);
                        }
                    } else if (p[g_BtlActiveActor].commands[subEntryIndex].commandId == 7) {
                        if (g_BtlMenuFrameCounter & 0x40) {
                            Menu_DrawEncodedString1x(group8 * 0x2C + (group8 != 0 ? 8 : 0) + 0x4E,
                                                 w * 0xC + 0xAF,
                                                 (unsigned char *)Kernel_LookupUiText(
                                                     KERNEL_TEXT_COMMAND, p[g_BtlActiveActor].commands[subEntryIndex].commandId, 8),
                                                 palette, 0.501f);
                        } else {
                            Menu_DrawEncodedString1x(group8 * 0x2C + (group8 != 0 ? 8 : 0) + 0x4E,
                                                 w * 0xC + 0xAF, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, 8, 8), palette,
                                                 0.501f);
                        }
                    } else {
                        Menu_DrawEncodedString1x(
                            group8 * 0x2C + (group8 != 0 ? 8 : 0) + 0x4E, w * 0xC + 0xAF,
                            (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, p[g_BtlActiveActor].commands[subEntryIndex].commandId, 8), palette, 0.501f);
                        tempTextWidth = Menu_MeasureEncodedString1x(
                            (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, p[g_BtlActiveActor].commands[subEntryIndex].commandId, 8));
                    }
                    if (p[g_BtlActiveActor].commands[subEntryIndex].menuFlags & BATTLE_MENU_SLOT_MULTI_ARROW) {
                        Menu_DrawAtlasQuad1x(group8 * 44 + (group8 != 0 ? 8 : 0) + tempTextWidth + 0x4C,
                                             w * 12 + 0xB1, 128, 8, 8, 8, 2, 0, 0, 0.501f);
                    }
                }
            }
        }
    }
}

/*
 * Draws the 2x battle item list, adding its scrollbar when more than three rows
 * exist. Selects three visible rows or four while scrolling, brackets panel
 * drawing with its two helpers and draws nonempty item icons/counts first then
 * names.
 */
// FUNCTION: FF7 0x006debfe
void Menu_DrawBattleItemList2x(void)
{
    short scrollOfs;
    int *state;
    int color;
    short track[7];
    unsigned char *entry;
    int y;
    short listPosition;
    short newVisibleRows;
    int index;
    short mode;

    entry = g_BattleItemListBlock.itemSlots;
    state = (int *)&g_BtlMenuSlotTableA[g_BtlActiveActor * 0x700];
    if (g_BattleItemListBlock.itemListLimit > 3) {
        track[0] = 3;
        track[1] = g_BattleItemListBlock.itemListLimit << 1;
        track[2] = (short)state[5];
        track[3] = 0x266;
        track[4] = 0x162;
        track[5] = 0x14;
        track[6] = 0x64;
        Menu_DrawScrollBar2x(track, 0.401f);
    }
    scrollOfs = (short)(state[9] << 3);
    mode = (short)state[5];
    if (scrollOfs != 0) {
        newVisibleRows = 4;
    } else {
        newVisibleRows = 3;
    }
    Battle_BeginWindowTextDrawNoOp(BATTLE_MENU_ITEM_LIST);
    for (listPosition = 0; listPosition < newVisibleRows; listPosition++) {
        index = mode + listPosition;
        if (g_BtlSelectedCommand == 3 || g_BtlSelectedCommand == 10) {
            color = (entry[index * 6 + 4] & BATTLE_MENU_ITEM_NO_BATTLE_USE) ? 0 : 7;
        } else {
            color = (entry[index * 6 + 4] & BATTLE_MENU_ITEM_NO_THROW) ? 0 : 7;
        }
        if (*(unsigned short *)&entry[index * 6] != MENU_ITEM_SLOT_EMPTY) {
            Menu_DrawItemTypeIcon2x(94, scrollOfs + listPosition * 0x20 + 0x164,
                                    *(unsigned short *)&entry[index * 6], 0, 0.401f);
            DrawIconGlyph16_2x(366, scrollOfs + listPosition * 32 + 0x16c, 0xd5,
                            color, 0.401f);
            Menu_DrawDecimalNumber2x(378, scrollOfs + listPosition * 32 + 0x16c,
                              entry[index * 6 + 2], 2, color, 0.401f);
        }
    }
    for (listPosition = 0; listPosition < newVisibleRows; listPosition++) {
        index = mode + listPosition;
        if (g_BtlSelectedCommand == 3 || g_BtlSelectedCommand == 10) {
            color = (entry[index * 6 + 4] & BATTLE_MENU_ITEM_NO_BATTLE_USE) ? 0 : 7;
        } else {
            color = (entry[index * 6 + 4] & BATTLE_MENU_ITEM_NO_THROW) ? 0 : 7;
        }
        if (*(unsigned short *)&entry[index * 6] != MENU_ITEM_SLOT_EMPTY) {
            Menu_DrawEncodedString2x(
                130, scrollOfs + listPosition * 0x20 + 0x168,
                (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM,
                                    *(unsigned short *)&entry[index * 6], 8),
                color, 0.401f);
        }
    }
    Battle_EndWindowTextDrawNoOp(BATTLE_MENU_ITEM_LIST);
}

/*
 * Draw one 16x16 status icon, picked from `code` by a chain of ranges. The chain
 * is nine bands over `code - 0x80` and the tile numbers are not in order -- 1, 3,
 * 2, 5, 4, 9, 6, 7, 8 -- so it is a lookup written as comparisons and not
 * arithmetic.
 */
// FUNCTION: FF7 0x006deef0
void __cdecl Menu_DrawItemTypeIcon2x(int screenX, int screenY, int code,
                                     int unused, float z)
{
    int glyph;
    int u;
    int v;
    int tile;

    if (code < 0x80) {
        tile = 0;
    } else if (code < 0x100) {
        glyph = code - 0x80;
        if (glyph < 0x10) {
            tile = 0x1;
        } else if (glyph < 0x20) {
            tile = 0x3;
        } else if (glyph < 0x30) {
            tile = 0x2;
        } else if (glyph < 0x3e) {
            tile = 0x5;
        } else if (glyph < 0x49) {
            tile = 0x4;
        } else if (glyph < 0x57) {
            tile = 0x9;
        } else if (glyph < 0x65) {
            tile = 0x6;
        } else if (glyph < 0x72) {
            tile = 0x7;
        } else {
            tile = 8;
        }
    } else if (code < 0x120) {
        tile = 0xa;
    } else {
        tile = 0xb;
    }
    u = (tile & 1) * 16 + 0x60;
    v = tile / 2 * 16 + 0x70;
    Menu_DrawAtlasQuad2x(screenX, screenY, u, v, 16, 16, 1, 0, 0, z);
}

/*
 * Draws the 1x battle item list, adding its scrollbar when more than three rows
 * exist. Selects three visible rows or four while scrolling, brackets panel
 * drawing with its two helpers and draws nonempty item icons/counts first then
 * names.
 */
// FUNCTION: FF7 0x006df007
void Menu_DrawBattleItemList1x(void)
{
    short scrollOfs;
    int *state;
    int color;
    short track[7];
    unsigned char *entry;
    int y;
    short listPosition;
    short newVisibleRows;
    int index;
    short mode;

    entry = g_BattleItemListBlock.itemSlots;
    state = (int *)&g_BtlMenuSlotTableA[g_BtlActiveActor * 0x700];
    if (g_BattleItemListBlock.itemListLimit > 3) {
        track[0] = 3;
        track[1] = g_BattleItemListBlock.itemListLimit << 1;
        track[2] = (short)state[5];
        track[3] = 0x133;
        track[4] = 0xb1;
        track[5] = 0xa;
        track[6] = 0x32;
        Menu_DrawScrollBar1x(track, 0.401f);
    }
    scrollOfs = (short)(state[9] << 2);
    mode = (short)state[5];
    if (scrollOfs != 0) {
        newVisibleRows = 4;
    } else {
        newVisibleRows = 3;
    }
    Battle_BeginWindowTextDrawNoOp(BATTLE_MENU_ITEM_LIST);
    for (listPosition = 0; listPosition < newVisibleRows; listPosition++) {
        index = mode + listPosition;
        if (g_BtlSelectedCommand == 3 || g_BtlSelectedCommand == 10) {
            color = (entry[index * 6 + 4] & BATTLE_MENU_ITEM_NO_BATTLE_USE) ? 0 : 7;
        } else {
            color = (entry[index * 6 + 4] & BATTLE_MENU_ITEM_NO_THROW) ? 0 : 7;
        }
        if (*(unsigned short *)&entry[index * 6] != MENU_ITEM_SLOT_EMPTY) {
            Menu_DrawItemTypeIcon1x(47, scrollOfs + listPosition * 0x10 + 0xb2,
                                    *(unsigned short *)&entry[index * 6], 0, 0.401f);
            DrawIconGlyph16_1x(183, scrollOfs + listPosition * 16 + 0xb6, 0xd5,
                               color, 0.401f);
            Menu_DrawDecimalNumber1x(189, scrollOfs + listPosition * 16 + 0xb6,
                                entry[index * 6 + 2], 2, color, 0.401f);
        }
    }
    for (listPosition = 0; listPosition < newVisibleRows; listPosition++) {
        index = mode + listPosition;
        if (g_BtlSelectedCommand == 3 || g_BtlSelectedCommand == 10) {
            color = (entry[index * 6 + 4] & BATTLE_MENU_ITEM_NO_BATTLE_USE) ? 0 : 7;
        } else {
            color = (entry[index * 6 + 4] & BATTLE_MENU_ITEM_NO_THROW) ? 0 : 7;
        }
        if (*(unsigned short *)&entry[index * 6] != MENU_ITEM_SLOT_EMPTY) {
            Menu_DrawEncodedString1x(
                65, scrollOfs + listPosition * 0x10 + 0xb4,
                (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM,
                                    *(unsigned short *)&entry[index * 6], 8),
                color, 0.401f);
        }
    }
    Battle_EndWindowTextDrawNoOp(BATTLE_MENU_ITEM_LIST);
}

/*
 * Menu_DrawItemTypeIcon2x's 1x twin. Same bands, same tiles, same atlas coordinates -- only
 * Menu_DrawAtlasQuad1x differs.
 */
// FUNCTION: FF7 0x006df2f6
void __cdecl Menu_DrawItemTypeIcon1x(int screenX, int screenY, int code,
                                     int unused, float z)
{
    int glyph;
    int u;
    int v;
    int tile;

    if (code < 0x80) {
        tile = 0;
    } else if (code < 0x100) {
        glyph = code - 0x80;
        if (glyph < 0x10) {
            tile = 0x1;
        } else if (glyph < 0x20) {
            tile = 0x3;
        } else if (glyph < 0x30) {
            tile = 0x2;
        } else if (glyph < 0x3e) {
            tile = 0x5;
        } else if (glyph < 0x49) {
            tile = 0x4;
        } else if (glyph < 0x57) {
            tile = 0x9;
        } else if (glyph < 0x65) {
            tile = 0x6;
        } else if (glyph < 0x72) {
            tile = 0x7;
        } else {
            tile = 8;
        }
    } else if (code < 0x120) {
        tile = 0xa;
    } else {
        tile = 0xb;
    }
    u = (tile & 1) * 16 + 0x60;
    v = tile / 2 * 16 + 0x70;
    Menu_DrawAtlasQuad1x(screenX, screenY, u, v, 16, 16, 1, 0, 0, z);
}

/*
 * Returns for an empty party slot, otherwise draws the selected actor limit names
 * and three fixed/character-dependent atlas headers at 2x. When text rendering is
 * ready and batch acquisition succeeds, fills a four-vertex translucent background
 * with lower edge selected from the limit-count height table.
 */
// FUNCTION: FF7 0x006df40d
void __cdecl Menu_DrawBattleLimitList2x(void)
{
    int padSlot[4]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    unsigned char *limitList;
    short limitSlot;
    GfxTlVertex *bottomLeft;
    GfxTlVertex *vertA;
    GfxTlVertex *pTopRight;
    GfxTlVertex *bottomRight;

    if (gSavemap.party[g_BtlActivePartyMember] == PARTY_CHAR_NONE) {
        return;
    }
    limitList = g_MenuPartyStats[g_BtlActivePartyMember].limitAttacks;
    for (limitSlot = 0; limitSlot < g_BtlMenuLimitRowCount; limitSlot++) {
        Menu_DrawEncodedString2x(176, limitSlot * 0x1E + 0x170, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_LIMIT, limitList[limitSlot], 8), 7, 0.301f);
    }
    Menu_DrawAtlasQuad2x(170, 356, 0x38, 0, 24, 6, 1, 0, 0, 0.301f);
    Menu_DrawAtlasQuad2x(228, 356, 0xC8, 0x18, 26, 6, 1, 0, 0, 0.301f);
    Menu_DrawAtlasQuad2x(280, 352, g_BtlLimitMenuTables.limitLevelAtlasTable[gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[g_BtlActivePartyMember]]].limitLevel], 0x10, 8, 8, 1, 0, 0, 0.301f);
    if (Menu_IsTextSubsystemReady() != 0) {
        if (AcquireBatchNode(1, g_MenuBattleLimitGraphicsObject) != 0) {
            vertA = (GfxTlVertex *)(g_MenuBattleLimitGraphicsObject->vertexCursor);
            vertA->xBits = FLOAT_166_BITS;
            vertA->yBits = FLOAT_354_BITS;
            vertA->zBits = FLOAT_0_401_BITS;
            vertA->rhwBits = FLOAT_ONE_BITS;
            vertA->color = 0x90900000;
            vertA->specular = 0xFF000000;
            bottomLeft = (GfxTlVertex *)(g_MenuBattleLimitGraphicsObject->vertexCursor + 0x20);
            bottomLeft->xBits = FLOAT_166_BITS;
            bottomLeft->y = (float)g_BtlLimitMenuTables.limitRowYTableLarge[g_BtlMenuLimitRowCount] + 354.0f - 9.0f;
            bottomLeft->zBits = FLOAT_0_401_BITS;
            bottomLeft->rhwBits = FLOAT_ONE_BITS;
            bottomLeft->color = 0x90900000;
            bottomLeft->specular = 0xFF000000;
            pTopRight = (GfxTlVertex *)(g_MenuBattleLimitGraphicsObject->vertexCursor + 0x40);
            pTopRight->xBits = FLOAT_426_BITS;
            pTopRight->yBits = FLOAT_354_BITS;
            pTopRight->zBits = FLOAT_0_401_BITS;
            pTopRight->rhwBits = FLOAT_ONE_BITS;
            pTopRight->color = 0x90900000;
            pTopRight->specular = 0xFF000000;
            bottomRight = (GfxTlVertex *)(g_MenuBattleLimitGraphicsObject->vertexCursor + 0x60);
            bottomRight->xBits = FLOAT_426_BITS;
            bottomRight->y = (float)g_BtlLimitMenuTables.limitRowYTableLarge[g_BtlMenuLimitRowCount] + 354.0f - 9.0f;
            bottomRight->zBits = FLOAT_0_401_BITS;
            bottomRight->rhwBits = FLOAT_ONE_BITS;
            bottomRight->color = 0x90900000;
            bottomRight->specular = 0xFF000000;
        }
    }
}

/*
 * Returns for an empty party slot, otherwise draws the selected actor limit names
 * and three fixed/character-dependent atlas headers at 1x. When text rendering is
 * ready and batch acquisition succeeds, fills a base-offset translucent background
 * with lower edge selected from this variant height table.
 */
// FUNCTION: FF7 0x006df6ca
void __cdecl Menu_DrawBattleLimitList1x(void)
{
    int padSlot[4]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    unsigned char *limitList;
    short limitSlot;
    GfxTlVertex *bottomLeft;
    GfxTlVertex *vertA;
    GfxTlVertex *pTopRight;
    GfxTlVertex *bottomRight;

    if (gSavemap.party[g_BtlActivePartyMember] == PARTY_CHAR_NONE) {
        return;
    }
    limitList = g_MenuPartyStats[g_BtlActivePartyMember].limitAttacks;
    for (limitSlot = 0; limitSlot < g_BtlMenuLimitRowCount; limitSlot++) {
        Menu_DrawEncodedString1x(88, limitSlot * 0xF + 0xB8, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_LIMIT, limitList[limitSlot], 8), 7, 0.301f);
    }
    Menu_DrawAtlasQuad1x(85, 178, 56, 0, 24, 6, 1, 0, 0, 0.301f);
    Menu_DrawAtlasQuad1x(114, 178, 200, 24, 26, 6, 1, 0, 0, 0.301f);
    Menu_DrawAtlasQuad1x(140, 176, g_BtlLimitMenuTables.limitLevelAtlasTable[gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[g_BtlActivePartyMember]]].limitLevel], 16, 8, 8, 1, 0, 0, 0.301f);
    if (Menu_IsTextSubsystemReady() != 0) {
        if (AcquireBatchNode(1, g_MenuBattleLimitGraphicsObject) != 0) {
            vertA = (GfxTlVertex *)(g_MenuBattleLimitGraphicsObject->vertexCursor);
            vertA->x = (float)g_MenuViewportX + 83.0f;
            vertA->y = (float)g_MenuViewportY + 177.0f;
            vertA->zBits = FLOAT_0_401_BITS;
            vertA->rhwBits = FLOAT_ONE_BITS;
            vertA->color = 0x90900000;
            vertA->specular = 0xFF000000;
            bottomLeft = (GfxTlVertex *)(g_MenuBattleLimitGraphicsObject->vertexCursor + 0x20);
            bottomLeft->x = (float)g_MenuViewportX + 83.0f;
            bottomLeft->y = (float)g_BtlLimitMenuTables.limitRowYTableSmall[g_BtlMenuLimitRowCount] + 177.0f - 6.0f + g_MenuViewportY;
            bottomLeft->zBits = FLOAT_0_401_BITS;
            bottomLeft->rhwBits = FLOAT_ONE_BITS;
            bottomLeft->color = 0x90900000;
            bottomLeft->specular = 0xFF000000;
            pTopRight = (GfxTlVertex *)(g_MenuBattleLimitGraphicsObject->vertexCursor + 0x40);
            pTopRight->x = (float)g_MenuViewportX + 213.0f;
            pTopRight->y = (float)g_MenuViewportY + 177.0f;
            pTopRight->zBits = FLOAT_0_401_BITS;
            pTopRight->rhwBits = FLOAT_ONE_BITS;
            pTopRight->color = 0x90900000;
            pTopRight->specular = 0xFF000000;
            bottomRight = (GfxTlVertex *)(g_MenuBattleLimitGraphicsObject->vertexCursor + 0x60);
            bottomRight->x = (float)g_MenuViewportX + 213.0f;
            bottomRight->y = (float)g_BtlLimitMenuTables.limitRowYTableSmall[g_BtlMenuLimitRowCount] + 177.0f - 6.0f + g_MenuViewportY;
            bottomRight->zBits = FLOAT_0_401_BITS;
            bottomRight->rhwBits = FLOAT_ONE_BITS;
            bottomRight->color = 0x90900000;
            bottomRight->specular = 0xFF000000;
        }
    }
}

/*
 * Draws the 2x three-column ability list with scrollbar, three rows or four while
 * scrolling, and disabled colors from entry flag two. A second pass adds markers
 * for flag 0x10, then draws selected-ability details and closes the panel draw
 * bracket.
 */
// FUNCTION: FF7 0x006df9ba
void Menu_DrawBattleMagicList2x(void)
{
    struct MenuPartyStats *p;
    short yoff;
    int blk;
    int choice;
    BattleAbilitySlot *entry;
    int *win;
    short limit;
    short track[7];
    short item;
    int lines;
    short top;

    p = g_MenuPartyStats;
    entry = g_MenuPartyStats[g_BtlActiveActor].spells;
    win = (int *)&g_BtlMenuSlotTableB[g_BtlActiveActor * 0x700];
    track[0] = 3;
    track[1] = 0x12;
    track[2] = (short)win[5];
    track[3] = 0x1dc;
    track[4] = 0x162;
    track[5] = 0x14;
    track[6] = 0x64;
    Menu_DrawScrollBar2x(track, 0.401f);
    yoff = (short)(win[9] << 3);
    top = (short)(win[5] * 3);
    choice = win[0] + win[1] * 3 + win[5] * 3;
    if (yoff != 0) {
        limit = 4;
    } else {
        limit = 3;
    }
    Battle_BeginWindowTextDrawNoOp(BATTLE_MENU_MAGIC_LIST);
    for (item = 0; item < limit; item++) {
        for (lines = 0; lines < 3; lines++) {
            if (entry[lines + top + item * 3].attackId != 0xff) {
                Menu_DrawEncodedString2x(
                    lines * 0x9c + 0x2a,
                    yoff + item * 0x20 + 0x168,
                    (unsigned char *)Kernel_LookupUiText(
                        KERNEL_TEXT_MAGIC, entry[lines + top + item * 3].attackId, 8),
                    (entry[lines + top + item * 3].menuFlags & BATTLE_MENU_SLOT_GREYED) ? 0 : 7,
                    0.401f);
            }
        }
    }
    for (item = 0; item < limit; item++) {
        for (lines = 0; lines < 3; lines++) {
            if (entry[lines + top + item * 3].attackId != 0xff &&
                (entry[lines + top + item * 3].menuFlags & BATTLE_MENU_SLOT_MULTI_ARROW) != 0) {
                Menu_DrawAtlasQuad2x(lines * 156 + 0x92,
                                     yoff + item * 32 + 0x16c,
                                     0x80, 8, 8, 8, 2, 0, 0, 0.401f);
            }
        }
    }
    Menu_DrawBattleAbilityCostPanel2x(528, 348, BATTLE_ABILITY_LIST_MAGIC);
    Battle_EndWindowTextDrawNoOp(BATTLE_MENU_MAGIC_LIST);
}

/*
 * Draws the 2x selected-ability detail panel, resolving its entry/index from the
 * magic, summon or enemy-skill list (layout). For present entries draws cost and optional
 * labels/values, using an atlas symbol for 0xFF and suppressing two optional
 * groups for the enemy-skill list.
 */
// FUNCTION: FF7 0x006dfc65
void Menu_DrawBattleAbilityCostPanel2x(int x, int y, int layout)
{
    int p;
    int q;
    BattleAbilitySlot *entry;
    int index;

    Menu_DrawAtlasQuad2x(x + 6, y, 0, 0x10, 40, 16,
                         1, 0, 0, 0.401f);
    switch (layout) {
    case BATTLE_ABILITY_LIST_MAGIC:
        entry = g_MenuPartyStats[g_BtlActiveActor].spells;
        index = *(int *)&g_BtlMenuSlotTableB[g_BtlActiveActor * 0x700] +
                *(int *)&g_BtlMenuSlotTableC[g_BtlActiveActor * 0x700] * 3 +
                *(int *)&g_BtlMenuWindowRecordsB[g_BtlActiveActor * 0x700] * 3;
        if (entry[index].attackId != 0xff && entry[index].allCount != 0) {
            Menu_DrawEncodedString2x(x - 0xa, y + 0x4e,
                              (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, 0xe, 8), 7,
                              0.401f);
        }
        break;
    case BATTLE_ABILITY_LIST_SUMMON:
        entry = g_MenuPartyStats[g_BtlActiveActor].summons;
        index = *(int *)&g_BtlMenuWindowRecordsC[g_BtlActiveActor * 0x700] +
                *(int *)&g_BtlMenuSlotTableD[g_BtlActiveActor * 0x700] +
                *(int *)&g_BtlMenuWindowRecordsD[g_BtlActiveActor * 0x700];
        break;
    case BATTLE_ABILITY_LIST_ENEMY_SKILL:
        entry = g_MenuPartyStats[g_BtlActiveActor].enemySkills;
        index = *(int *)&g_BtlMenuWindows[g_BtlActiveActor * 0x700] +
                *(int *)&g_BtlMenuCursors[g_BtlActiveActor * 0x700] * 2 +
                *(int *)&g_BtlMenuWindowRecords[g_BtlActiveActor * 0x700] * 2;
        break;
    }

    if (entry[index].attackId != 0xff) {
        if (layout != BATTLE_ABILITY_LIST_ENEMY_SKILL) {
            if (entry[index].quadraCount != 0) {
                Menu_DrawEncodedString2x(x - 0xa, y + 0x38,
                                  (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, 0xf, 8), 7,
                                  0.401f);
                if (entry[index].quadraCount != 0xff) {
                    Menu_DrawDecimalNumber2x(x + 0x26, y + 0x3c,
                                      entry[index].quadraCount, 2, 7, 0.401f);
                } else {
                    Menu_DrawAtlasQuad2x(x + 0x26, y + 0x3c,
                                         0x90, 0x60, 16, 8, 6, 0, 0,
                                         0.401f);
                }
                Menu_DrawAtlasQuad2x(x + 0x46, y + 0x3c,
                                     0x78, 8, 8, 8, 7, 0, 0, 0.401f);
            }
            if (entry[index].allCount != 0) {
                if (entry[index].allCount != 0xff) {
                    Menu_DrawDecimalNumber2x(x + 0x26, y + 0x54,
                                      entry[index].allCount, 2, 7, 0.401f);
                } else {
                    Menu_DrawAtlasQuad2x(x + 0x26, y + 0x54,
                                         0x90, 0x60, 16, 8, 6, 0, 0,
                                         0.401f);
                }
                Menu_DrawAtlasQuad2x(x + 0x46, y + 0x54,
                                     0x78, 8, 8, 8, 7, 0, 0, 0.401f);
            }
        }
        Menu_DrawDecimalNumber2x(x, y + 0x24,
                          entry[index].mpCost, 3, 7, 0.401f);
    }
    Menu_DrawDecimalNumber2x(
        x + 0x32, y + 0x24,
        g_MenuPartyStats[g_BtlActiveActor].mp,
        3, 7, 0.401f);
    Menu_DrawAtlasQuad2x(x + 0x2a, y + 0x24, 0xd8, 0, 4, 8,
                         1, 0, 0, 0.401f);
}

/*
 * Draws the 1x three-column ability list with scrollbar, three rows or four while
 * scrolling and disabled colors from entry flag two. A second pass adds flag 0x10
 * markers, then draws selected- ability details and closes the panel drawing
 * bracket.
 */
// FUNCTION: FF7 0x006e0046
void Menu_DrawBattleMagicList1x(void)
{
    struct MenuPartyStats *p;
    short yoff;
    int blk;
    int choice;
    BattleAbilitySlot *entry;
    int *win;
    short limit;
    short track[7];
    short item;
    int lines;
    short top;

    p = g_MenuPartyStats;
    entry = g_MenuPartyStats[g_BtlActiveActor].spells;
    win = (int *)&g_BtlMenuSlotTableB[g_BtlActiveActor * 0x700];
    track[0] = 3;
    track[1] = 0x12;
    track[2] = (short)win[5];
    track[3] = 0xee;
    track[4] = 0xb1;
    track[5] = 0xa;
    track[6] = 0x32;
    Menu_DrawScrollBar1x(track, 0.401f);
    yoff = (short)(win[9] << 2);
    top = (short)(win[5] * 3);
    choice = win[0] + win[1] * 3 + win[5] * 3;
    if (yoff != 0) {
        limit = 4;
    } else {
        limit = 3;
    }
    Battle_BeginWindowTextDrawNoOp(BATTLE_MENU_MAGIC_LIST);
    for (item = 0; item < limit; item++) {
        for (lines = 0; lines < 3; lines++) {
            if (entry[lines + top + item * 3].attackId != 0xff) {
                Menu_DrawEncodedString1x(
                    lines * 0x4e + 0x15,
                    yoff + item * 0x10 + 0xb4,
                    (unsigned char *)Kernel_LookupUiText(
                        KERNEL_TEXT_MAGIC, entry[lines + top + item * 3].attackId, 8),
                    (entry[lines + top + item * 3].menuFlags & BATTLE_MENU_SLOT_GREYED) ? 0 : 7,
                    0.401f);
            }
        }
    }
    for (item = 0; item < limit; item++) {
        for (lines = 0; lines < 3; lines++) {
            if (entry[lines + top + item * 3].attackId != 0xff &&
                (entry[lines + top + item * 3].menuFlags & BATTLE_MENU_SLOT_MULTI_ARROW) != 0) {
                Menu_DrawAtlasQuad1x(lines * 78 + 0x49,
                                     yoff + item * 16 + 0xb6,
                                     128, 8, 8, 8, 2, 0, 0, 0.401f);
            }
        }
    }
    Menu_DrawBattleAbilityCostPanel1x(264, 174, BATTLE_ABILITY_LIST_MAGIC);
    Battle_EndWindowTextDrawNoOp(BATTLE_MENU_MAGIC_LIST);
}

/*
 * Draws selected-ability details using 1x coordinates from the magic, summon or
 * enemy-skill list (layout). Draws optional cost labels/values outside the enemy-skill list and uses
 * an atlas symbol for 0xFF.
 */
// FUNCTION: FF7 0x006e02e8
void Menu_DrawBattleAbilityCostPanel1x(int x, int y, int layout)
{
    int p;
    int q;
    BattleAbilitySlot *entry;
    int index;

    Menu_DrawAtlasQuad1x(x + 3, y, 0, 16, 40, 16,
                         1, 0, 0, 0.401f);
    switch (layout) {
    case BATTLE_ABILITY_LIST_MAGIC:
        entry = g_MenuPartyStats[g_BtlActiveActor].spells;
        index = *(int *)&g_BtlMenuSlotTableB[g_BtlActiveActor * 0x700] +
                *(int *)&g_BtlMenuSlotTableC[g_BtlActiveActor * 0x700] * 3 +
                *(int *)&g_BtlMenuWindowRecordsB[g_BtlActiveActor * 0x700] * 3;
        if (entry[index].attackId != 0xff && entry[index].allCount != 0) {
            Menu_DrawEncodedString1x(x - 5, y + 0x27,
                                 (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, 0xe, 8), 7,
                                 0.401f);
        }
        break;
    case BATTLE_ABILITY_LIST_SUMMON:
        entry = g_MenuPartyStats[g_BtlActiveActor].summons;
        index = *(int *)&g_BtlMenuWindowRecordsC[g_BtlActiveActor * 0x700] +
                *(int *)&g_BtlMenuSlotTableD[g_BtlActiveActor * 0x700] +
                *(int *)&g_BtlMenuWindowRecordsD[g_BtlActiveActor * 0x700];
        break;
    case BATTLE_ABILITY_LIST_ENEMY_SKILL:
        entry = g_MenuPartyStats[g_BtlActiveActor].enemySkills;
        index = *(int *)&g_BtlMenuWindows[g_BtlActiveActor * 0x700] +
                *(int *)&g_BtlMenuCursors[g_BtlActiveActor * 0x700] * 2 +
                *(int *)&g_BtlMenuWindowRecords[g_BtlActiveActor * 0x700] * 2;
        break;
    }

    if (entry[index].attackId != 0xff) {
        if (layout != BATTLE_ABILITY_LIST_ENEMY_SKILL) {
            if (entry[index].quadraCount != 0) {
                Menu_DrawEncodedString1x(x - 5, y + 0x1c,
                                     (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, 0xf, 8), 7,
                                     0.401f);
                if (entry[index].quadraCount != 0xff) {
                    Menu_DrawDecimalNumber2x(x + 0x13, y + 0x1e,
                                      entry[index].quadraCount, 2, 7, 0.401f);
                } else {
                    Menu_DrawAtlasQuad1x(x + 0x13, y + 0x1e,
                                         144, 96, 16, 8, 6, 0, 0,
                                         0.401f);
                }
                Menu_DrawAtlasQuad1x(x + 0x23, y + 0x1e,
                                     120, 8, 8, 8, 7, 0, 0, 0.401f);
            }
            if (entry[index].allCount != 0) {
                if (entry[index].allCount != 0xff) {
                    Menu_DrawDecimalNumber1x(x + 0x13, y + 0x2a,
                                        entry[index].allCount, 2, 7,
                                        0.401f);
                } else {
                    Menu_DrawAtlasQuad1x(x + 0x13, y + 0x2a,
                                         144, 96, 16, 8, 6, 0, 0,
                                         0.401f);
                }
                Menu_DrawAtlasQuad1x(x + 0x23, y + 0x2a,
                                     120, 8, 8, 8, 7, 0, 0, 0.401f);
            }
        }
        Menu_DrawDecimalNumber1x(x, y + 0x12,
                            entry[index].mpCost, 3, 7, 0.401f);
    }
    Menu_DrawDecimalNumber1x(
        x + 0x19, y + 0x12,
        g_MenuPartyStats[g_BtlActiveActor].mp,
        3, 7, 0.401f);
    Menu_DrawAtlasQuad1x(x + 0x15, y + 0x12, 216, 0, 4, 8,
                         1, 0, 0, 0.401f);
}

/*
 * Draws the summon list at 2x: a scroll bar, then three or four rows of summon names out
 * of the active actor's record. `blk` and `lines` are never read; they keep the other
 * locals in the original's stack slots.
 */
// FUNCTION: FF7 0x006e06c9
void __cdecl Menu_DrawBattleSummonList2x(void)
{
    struct MenuPartyStats *p;
    short yoff;
    int blk;
    BattleAbilitySlot *entry;
    int *win;
    short limit;
    short track[7];
    short item;
    int lines;
    short top;

    p = g_MenuPartyStats;
    entry = g_MenuPartyStats[g_BtlActiveActor].summons;
    win = (int *)&g_BtlMenuWindowRecordsC[g_BtlActiveActor * 0x700];
    track[0] = 3;
    track[1] = 0x10;
    track[2] = (short)win[5];
    track[3] = 0x1dc;
    track[4] = 0x162;
    track[5] = 0x14;
    track[6] = 0x64;
    Menu_DrawScrollBar2x(track, 0.401f);
    yoff = (short)(win[9] << 3);
    top = (short)win[5];
    if (yoff != 0) {
        limit = 4;
    } else {
        limit = 3;
    }
    Battle_BeginWindowTextDrawNoOp(BATTLE_MENU_SUMMON_LIST);
    for (item = 0; item < limit; item++) {
        if (entry[top + item].attackId != 0xff) {
            Menu_DrawEncodedString2x(112, yoff + item * 0x20 + 0x168,
                              (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_SUMMON, entry[top + item].attackId, 8),
                              (entry[top + item].menuFlags & BATTLE_MENU_SLOT_GREYED) ? 0 : 7, 0.401f);
        }
    }
    Menu_DrawBattleAbilityCostPanel2x(528, 348, BATTLE_ABILITY_LIST_SUMMON);
    Battle_EndWindowTextDrawNoOp(BATTLE_MENU_SUMMON_LIST);
}

/*
 * Menu_DrawBattleSummonList2x at the 1x scale: half the scroll offset, half the row pitch, half
 * every coordinate, and the 1x drawers. Ten locals for eight live ones: `blk` and
 * `lines` are never read and are there to put the other eight in the slots the
 * image has.
 */
// FUNCTION: FF7 0x006e0842
void __cdecl Menu_DrawBattleSummonList1x(void)
{
    struct MenuPartyStats *p;
    short yoff;
    int blk;
    BattleAbilitySlot *entry;
    int *win;
    short limit;
    short track[7];
    short item;
    int lines;
    short top;

    p = g_MenuPartyStats;
    entry = g_MenuPartyStats[g_BtlActiveActor].summons;
    win = (int *)&g_BtlMenuWindowRecordsC[g_BtlActiveActor * 0x700];
    track[0] = 3;
    track[1] = 0x10;
    track[2] = (short)win[5];
    track[3] = 0xee;
    track[4] = 0xb1;
    track[5] = 0xa;
    track[6] = 0x32;
    Menu_DrawScrollBar1x(track, 0.401f);
    yoff = (short)(win[9] << 2);
    top = (short)win[5];
    if (yoff != 0) {
        limit = 4;
    } else {
        limit = 3;
    }
    Battle_BeginWindowTextDrawNoOp(BATTLE_MENU_SUMMON_LIST);
    for (item = 0; item < limit; item++) {
        if (entry[top + item].attackId != 0xff) {
            Menu_DrawEncodedString1x(56, yoff + item * 0x10 + 0xb4,
                                 (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_SUMMON, entry[top + item].attackId, 8),
                                 (entry[top + item].menuFlags & BATTLE_MENU_SLOT_GREYED) ? 0 : 7, 0.401f);
        }
    }
    Menu_DrawBattleAbilityCostPanel1x(264, 174, BATTLE_ABILITY_LIST_SUMMON);
    Battle_EndWindowTextDrawNoOp(BATTLE_MENU_SUMMON_LIST);
}

/*
 * Draws the 2x two-column ability list with scrollbar and three rows or four while
 * scrolling. Skips empty entries and chooses disabled color from flag two, then
 * draws the selected enemy skill's details and closes the panel drawing bracket.
 */
// FUNCTION: FF7 0x006e09bb
void __cdecl Menu_DrawBattleEnemySkillList2x(void)
{
    struct MenuPartyStats *p;
    short yoff;
    int blk;
    BattleAbilitySlot *entry;
    int *win;
    short limit;
    short track[7];
    short item;
    int lines;
    short top;

    p = g_MenuPartyStats;
    entry = g_MenuPartyStats[g_BtlActiveActor].enemySkills;
    win = (int *)&g_BtlMenuWindows[g_BtlActiveActor * 0x700];
    track[0] = 3;
    track[1] = 0xc;
    track[2] = (short)win[5];
    track[3] = 0x1dc;
    track[4] = 0x162;
    track[5] = 0x14;
    track[6] = 0x64;
    Menu_DrawScrollBar2x(track, 0.401f);
    yoff = (short)(win[9] << 3);
    top = (short)(win[5] << 1);
    if (yoff != 0) {
        limit = 4;
    } else {
        limit = 3;
    }
    Battle_BeginWindowTextDrawNoOp(BATTLE_MENU_ENEMY_SKILL_LIST);
    for (item = 0; item < limit; item++) {
        for (lines = 0; lines < 2; lines++) {
            if (entry[lines + top + item * 2].attackId != 0xff) {
                Menu_DrawEncodedString2x(lines * 0xf0 + 0x30,
                                  yoff + item * 0x20 + 0x168,
                                  (unsigned char *)Kernel_LookupUiText(
                                      KERNEL_TEXT_ENEMY_SKILL, entry[lines + top + item * 2].attackId,
                                      8),
                                  (entry[lines + top + item * 2].menuFlags &
                                   2)
                                      ? 0
                                      : 7,
                                  0.401f);
            }
        }
    }
    Menu_DrawBattleAbilityCostPanel2x(528, 348, BATTLE_ABILITY_LIST_ENEMY_SKILL);
    Battle_EndWindowTextDrawNoOp(BATTLE_MENU_ENEMY_SKILL_LIST);
}

/*
 * Draws the selected character's two-column eight-byte-entry list with a scrollbar
 * and three or four visible rows according to scroll animation. Skips 0xFF
 * entries, looks up UI text through Kernel_LookupUiText(2,id,8), selects text
 * style from entry bit 2 and brackets drawing with
 * Battle_BeginWindowTextDrawNoOp/Battle_EndWindowTextDrawNoOp, retaining the final
 * Menu_DrawBattleAbilityCostPanel1x call.
 */
// FUNCTION: FF7 0x006e0b73
void __cdecl Menu_DrawBattleEnemySkillList1x(void)
{
    struct MenuPartyStats *p;
    short yoff;
    int blk;
    BattleAbilitySlot *entry;
    int *win;
    short limit;
    short track[7];
    short item;
    int lines;
    short top;

    p = g_MenuPartyStats;
    entry = g_MenuPartyStats[g_BtlActiveActor].enemySkills;
    win = (int *)&g_BtlMenuWindows[g_BtlActiveActor * 0x700];
    track[0] = 3;
    track[1] = 0xc;
    track[2] = (short)win[5];
    track[3] = 0xee;
    track[4] = 0xb1;
    track[5] = 10;
    track[6] = 0x32;
    Menu_DrawScrollBar1x(track, 0.401f);
    yoff = (short)(win[9] << 2);
    top = (short)(win[5] << 1);
    if (yoff != 0) {
        limit = 4;
    } else {
        limit = 3;
    }
    Battle_BeginWindowTextDrawNoOp(BATTLE_MENU_ENEMY_SKILL_LIST);
    for (item = 0; item < limit; item++) {
        for (lines = 0; lines < 2; lines++) {
            if (entry[lines + top + item * 2].attackId != 0xff) {
                Menu_DrawEncodedString1x(
                    lines * 0x78 + 0x18, yoff + item * 0x10 + 0xb4,
                    (unsigned char *)Kernel_LookupUiText(
                        KERNEL_TEXT_ENEMY_SKILL, entry[lines + top + item * 2].attackId, 8),
                    (entry[lines + top + item * 2].menuFlags & BATTLE_MENU_SLOT_GREYED) ? 0 : 7,
                    0.401f);
            }
        }
    }
    Menu_DrawBattleAbilityCostPanel1x(264, 174, BATTLE_ABILITY_LIST_ENEMY_SKILL);
    Battle_EndWindowTextDrawNoOp(BATTLE_MENU_ENEMY_SKILL_LIST);
}

/*
 * Draws fixed encoded labels and three currency values: selected amount, remaining
 * amount and total. When text is ready and batch acquisition succeeds, fills a
 * 316-by 104 quad at 106/354 with phase-dependent scrolling UVs, fixed
 * depth/W/colors and texture extents 2.46875/1.625.
 */
// FUNCTION: FF7 0x006e0d28
void __cdecl Battle_DrawGilAmountPrompt2x(void)
{
    int padSlot[4]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int quadLeft;
    float uOffset;
    float vOffset;
    float texW;
    float vSpan;
    GfxTlVertex *vertexB;
    GfxTlVertex *corner0Ptr;
    GfxTlVertex *vertexDPtr;
    GfxTlVertex *vertexC;
    int quadTop;

    Menu_DrawEncodedString2x(120, 360, (unsigned char *)g_BtlSquareGilPromptText, 7, 0.401f);
    Menu_DrawEncodedString2x(144, 386, (unsigned char *)&g_BtlSquareGilText, 5, 0.401f);
    Menu_DrawEncodedString2x(144, 404, (unsigned char *)&g_BtlSquareAfterText, 5, 0.401f);
    Menu_DrawEncodedString2x(144, 434, (unsigned char *)g_BtlSquareGilOnHandText, 5, 0.401f);
    Menu_DrawDecimalNumber2x(264, 390, (int)g_BtlMenuGilAmount, 10, 7, 0.401f);
    Menu_DrawDecimalNumber2x(264, 414, gSavemap.gil - (int)g_BtlMenuGilAmount, 10, 7, 0.401f);
    Menu_DrawDecimalNumber2x(264, 438, gSavemap.gil, 10, 7, 0.401f);
    quadLeft = 106;
    quadTop = 354;
    if (Menu_IsTextSubsystemReady() != 0) {
        if (AcquireBatchNode(1, g_BtlMenuRenderObjZeni) != 0) {
            uOffset = (float)((g_BtlMenuFrameCounter << 1 & 0x3F) + 3) * 0.015625f;
            vOffset = (float)(3 - (g_BtlMenuFrameCounter & 0x1F)) * 0.03125f;
            texW = 2.46875f;
            vSpan = 1.625f;
            corner0Ptr = (GfxTlVertex *)(g_BtlMenuRenderObjZeni->vertexCursor);
            corner0Ptr->x = (float)quadLeft;
            corner0Ptr->y = (float)quadTop;
            corner0Ptr->zBits = FLOAT_0_402_BITS;
            corner0Ptr->rhwBits = FLOAT_ONE_BITS;
            corner0Ptr->color = 0x80FFFFFF;
            corner0Ptr->specular = 0xFF000000;
            corner0Ptr->u = uOffset;
            corner0Ptr->v = vOffset;
            vertexB = (GfxTlVertex *)(g_BtlMenuRenderObjZeni->vertexCursor + 0x20);
            vertexB->x = (float)quadLeft;
            vertexB->y = (float)quadTop + 104.0f;
            vertexB->zBits = FLOAT_0_402_BITS;
            vertexB->rhwBits = FLOAT_ONE_BITS;
            vertexB->color = 0x80FFFFFF;
            vertexB->specular = 0xFF000000;
            vertexB->u = uOffset;
            vertexB->v = vOffset + vSpan;
            vertexC = (GfxTlVertex *)(g_BtlMenuRenderObjZeni->vertexCursor + 0x40);
            vertexC->x = (float)quadLeft + 316.0f;
            vertexC->y = (float)quadTop;
            vertexC->zBits = FLOAT_0_402_BITS;
            vertexC->rhwBits = FLOAT_ONE_BITS;
            vertexC->color = 0x80FFFFFF;
            vertexC->specular = 0xFF000000;
            vertexC->u = uOffset + texW;
            vertexC->v = vOffset;
            vertexDPtr = (GfxTlVertex *)(g_BtlMenuRenderObjZeni->vertexCursor + 0x60);
            vertexDPtr->x = (float)quadLeft + 316.0f;
            vertexDPtr->y = (float)quadTop + 104.0f;
            vertexDPtr->zBits = FLOAT_0_402_BITS;
            vertexDPtr->rhwBits = FLOAT_ONE_BITS;
            vertexDPtr->color = 0x80FFFFFF;
            vertexDPtr->specular = 0xFF000000;
            vertexDPtr->u = uOffset + texW;
            vertexDPtr->v = vOffset + vSpan;
        }
    }
}

/*
 * Draws fixed encoded labels and selected/remaining/total currency values at 1x.
 * When text is ready and batch acquisition succeeds, fills a base-offset 158-by 52
 * quad with phase-dependent scrolling UVs and fixed depth/W/colors.
 */
// FUNCTION: FF7 0x006e1017
void __cdecl Battle_DrawGilAmountPrompt1x(void)
{
    int padSlot[4]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int quadLeft;
    float uOffset;
    float vOffset;
    float texW;
    float vSpan;
    GfxTlVertex *vertexB;
    GfxTlVertex *corner0Ptr;
    GfxTlVertex *vertexDPtr;
    GfxTlVertex *vertexC;
    int quadTop;

    Menu_DrawEncodedString1x(60, 180, (unsigned char *)g_BtlSquareGilPromptText, 7, 0.401f);
    Menu_DrawEncodedString1x(72, 193, (unsigned char *)&g_BtlSquareGilText, 5, 0.401f);
    Menu_DrawEncodedString1x(72, 202, (unsigned char *)&g_BtlSquareAfterText, 5, 0.401f);
    Menu_DrawEncodedString1x(72, 217, (unsigned char *)g_BtlSquareGilOnHandText, 5, 0.401f);
    Menu_DrawDecimalNumber1x(132, 195, (int)g_BtlMenuGilAmount, 0xA, 7, 0.401f);
    Menu_DrawDecimalNumber1x(132, 207, gSavemap.gil - (int)g_BtlMenuGilAmount, 0xA, 7, 0.401f);
    Menu_DrawDecimalNumber1x(132, 219, gSavemap.gil, 0xA, 7, 0.401f);
    quadLeft = g_MenuViewportX + 53;
    quadTop = g_MenuViewportY + 177;
    if (Menu_IsTextSubsystemReady() != 0) {
        if (AcquireBatchNode(1, g_BtlMenuRenderObjZeni) != 0) {
            uOffset = (float)((g_BtlMenuFrameCounter << 1 & 0x3F) + 3) * 0.015625f;
            vOffset = (float)(3 - (g_BtlMenuFrameCounter & 0x1F)) * 0.03125;
            texW = 2.46875f;
            vSpan = 1.625f;
            corner0Ptr = (GfxTlVertex *)(g_BtlMenuRenderObjZeni->vertexCursor);
            corner0Ptr->x = (float)quadLeft;
            corner0Ptr->y = (float)quadTop;
            corner0Ptr->zBits = FLOAT_0_402_BITS;
            corner0Ptr->rhwBits = FLOAT_ONE_BITS;
            corner0Ptr->color = 0x80FFFFFF;
            corner0Ptr->specular = 0xFF000000;
            corner0Ptr->u = uOffset;
            corner0Ptr->v = vOffset;
            vertexB = (GfxTlVertex *)(g_BtlMenuRenderObjZeni->vertexCursor + 0x20);
            vertexB->x = (float)quadLeft;
            vertexB->y = (float)quadTop + 52.0f;
            vertexB->zBits = FLOAT_0_402_BITS;
            vertexB->rhwBits = FLOAT_ONE_BITS;
            vertexB->color = 0x80FFFFFF;
            vertexB->specular = 0xFF000000;
            vertexB->u = uOffset;
            vertexB->v = vOffset + vSpan;
            vertexC = (GfxTlVertex *)(g_BtlMenuRenderObjZeni->vertexCursor + 0x40);
            vertexC->x = (float)quadLeft + 158.0f;
            vertexC->y = (float)quadTop;
            vertexC->zBits = FLOAT_0_402_BITS;
            vertexC->rhwBits = FLOAT_ONE_BITS;
            vertexC->color = 0x80FFFFFF;
            vertexC->specular = 0xFF000000;
            vertexC->u = uOffset + texW;
            vertexC->v = vOffset;
            vertexDPtr = (GfxTlVertex *)(g_BtlMenuRenderObjZeni->vertexCursor + 0x60);
            vertexDPtr->x = (float)quadLeft + 158.0f;
            vertexDPtr->y = (float)quadTop + 52.0f;
            vertexDPtr->zBits = FLOAT_0_402_BITS;
            vertexDPtr->rhwBits = FLOAT_ONE_BITS;
            vertexDPtr->color = 0x80FFFFFF;
            vertexDPtr->specular = 0xFF000000;
            vertexDPtr->u = uOffset + texW;
            vertexDPtr->v = vOffset + vSpan;
        }
    }
}

/*
 * Draws occupied battle party HUD rows at double resolution in two passes: names
 * first, then HP, MP or cycling status labels selected by g_BattleSelectedUseCondition. HP/MP
 * modes draw four-digit current and maximum values, slash quads and 120-by-2
 * blue/green gauges from shifted animation counters.
 */
// FUNCTION: FF7 0x006e1308
void __cdecl Battle_DrawPartyHudRows2x(void)
{
    struct PartyHudFrame2x {
        short flag;
        unsigned short align0;
        MenuGauge bar;
        short i;
        unsigned short align1;
        struct MenuPartyStats *stats;
    } v; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    v.stats = g_MenuPartyStats;
    for (v.i = 0; v.i < 3; v.i++) {
        if (gSavemap.party[v.i] == PARTY_CHAR_NONE) {
            continue;
        }
        v.flag = (g_BattleScene.actors[v.i].statusFlags & BATTLE_SCENE_ACTOR_KO) == 0;
        Menu_DrawEncodedString2x(
            176, v.i * 0x20 + 0x174,
            (unsigned char *)&gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[v.i]]].name,
            ((v.flag ? -1 : 0) & 5U) + 2, 0.301f);
    }
    for (v.i = 0; v.i < 3; v.i++) {
        if (gSavemap.party[v.i] == PARTY_CHAR_NONE) {
            continue;
        }
        v.flag = g_BattleScene.targetMask & (1 << v.i);
        switch (g_BattleSelectedUseCondition) {
        case BATTLE_USE_CONDITION_HP:
            if (v.flag != 0) {
                Menu_DrawDecimalNumber2x(
                    406, v.i * 32 + 0x178,
                    g_BtlGaugeHpValues[v.i] >> 8, 4,
                    ((g_BtlGaugeHpValues[v.i] >> 8) >
                     v.stats[v.i].maxHp / 4) +
                        6,
                    0.301f);
            } else {
                Menu_DrawDecimalNumber2x(406, v.i * 32 + 0x178,
                                  g_BtlGaugeHpValues[v.i] >> 8, 4, 2,
                                  0.301f);
            }
            Menu_DrawAtlasQuad2x(462, v.i * 32 + 0x178, 0xd8, 0, 4, 8,
                                 1, 0, 0, 0.301f);
            Menu_DrawDecimalNumber2x(
                470, v.i * 0x20 + 0x178,
                v.stats[v.i].maxHp, 4,
                (((v.flag != 0) ? -1 : 0) & 5U) + 2, 0.301f);
            v.bar.x = 406;
            v.bar.y = (short)(v.i * 32 + 392);
            v.bar.width = 120;
            v.bar.height = 2;
            v.bar.low = (short)(g_BtlGaugeHpLow[v.i] >> 8);
            v.bar.high = (short)(g_BtlGaugeHpHigh[v.i] >> 8);
            v.bar.max = v.stats[v.i].maxHp;
            v.bar.direction = g_BtlGaugeHpDir[v.i];
            v.bar.colour = 0x800080ff;
            Menu_DrawResourceGauge2x((MenuGauge *)&v.bar, 0.301f);
            break;
        case BATTLE_USE_CONDITION_MP:
            if (v.flag != 0) {
                Menu_DrawDecimalNumber2x(
                    406, v.i * 32 + 0x178,
                    g_BtlGaugeMpValues[v.i] >> 8, 4,
                    ((g_BtlGaugeMpValues[v.i] >> 8) >
                     v.stats[v.i].maxMp / 4) +
                        6,
                    0.301f);
            } else {
                Menu_DrawDecimalNumber2x(406, v.i * 32 + 0x178,
                                  g_BtlGaugeMpValues[v.i] >> 8, 4, 2,
                                  0.301f);
            }
            Menu_DrawAtlasQuad2x(462, v.i * 32 + 0x178, 0xd8, 0, 4, 8,
                                 1, 0, 0, 0.301f);
            Menu_DrawDecimalNumber2x(
                470, v.i * 0x20 + 0x178,
                v.stats[v.i].maxMp, 4,
                (((v.flag != 0) ? -1 : 0) & 5U) + 2, 0.301f);
            v.bar.x = 406;
            v.bar.y = (short)(v.i * 32 + 392);
            v.bar.width = 120;
            v.bar.height = 2;
            v.bar.low = (short)(g_BtlGaugeMpLow[v.i] >> 8);
            v.bar.high = (short)(g_BtlGaugeMpHigh[v.i] >> 8);
            v.bar.max = v.stats[v.i].maxMp;
            v.bar.direction = g_BtlGaugeMpDir[v.i];
            v.bar.colour = 0x8000ff80;
            Menu_DrawResourceGauge2x((MenuGauge *)&v.bar, 0.301f);
            break;
        case BATTLE_USE_CONDITION_STATUS:
            g_BattlePartyStatusIconMasks[v.i] &= ~BATTLE_STATUS_HUD_NON_CYCLING_MASK;
            if ((((g_BtlMenuFrameCounter & 0xf) == 0) &&
                 g_BattlePartyStatusIconMasks[v.i] != 0) ||
                ((((unsigned int *)&g_BattlePartyStatusIconMasks)[v.i] &
                  (1 << g_BattleHudStatusIconCycles[v.i])) == 0 &&
                 g_BattlePartyStatusIconMasks[v.i] != 0)) {
                do {
                    g_BattleHudStatusIconCycles[v.i]++;
                    g_BattleHudStatusIconCycles[v.i] %= 27;
                } while ((((unsigned int *)&g_BattlePartyStatusIconMasks)[v.i] &
                          (1 << g_BattleHudStatusIconCycles[v.i])) == 0);
            }
            if (g_BattlePartyStatusIconMasks[v.i] != 0) {
                Menu_DrawEncodedString2x(
                    364, v.i * 0x20 + 0x174,
                    (unsigned char *)&g_BtlStatusNameTable +
                        g_BattleHudStatusIconCycles[v.i] * 10,
                    3, 0.301f);
            }
            break;
        }
    }
    Menu_DrawAtlasQuad2x(172, 362, 0x80, 0x10, 26, 5, 1, 0, 0,
                         0.301f);
    switch (g_BattleSelectedUseCondition) {
    case BATTLE_USE_CONDITION_HP:
        Menu_DrawAtlasQuad2x(414, 362, 0x50, 0, 16, 5, 1, 0, 0,
                             0.301f);
        break;
    case BATTLE_USE_CONDITION_MP:
        Menu_DrawAtlasQuad2x(414, 362, 0x60, 0, 16, 5, 1, 0, 0,
                             0.301f);
        break;
    case BATTLE_USE_CONDITION_STATUS:
        Menu_DrawAtlasQuad2x(356, 362, 0x80, 0x18, 32, 5, 1, 0, 0,
                             0.301f);
        break;
    }
}

/*
 * Draws occupied battle party HUD rows at low resolution in two passes: names
 * first, then HP, MP or cycling status labels selected by g_BattleSelectedUseCondition. HP/MP
 * modes draw four-digit current/maximum values, slash quads and 60-by-1 blue/green
 * gauges from shifted animation counters.
 */
// FUNCTION: FF7 0x006e193b
void __cdecl Battle_DrawPartyHudRows1x(void)
{
    struct PartyHudFrame1x {
        short flag;
        unsigned short align0;
        MenuGauge bar;
        short i;
        unsigned short align1;
        struct MenuPartyStats *stats;
    } v; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    v.stats = g_MenuPartyStats;
    for (v.i = 0; v.i < 3; v.i++) {
        if (gSavemap.party[v.i] == PARTY_CHAR_NONE) {
            continue;
        }
        v.flag = (g_BattleScene.actors[v.i].statusFlags & BATTLE_SCENE_ACTOR_KO) == 0;
        Menu_DrawEncodedString1x(
            88, v.i * 0x10 + 0xba,
            (unsigned char *)&gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[v.i]]].name,
            ((v.flag ? -1 : 0) & 5U) + 2, 0.301f);
    }
    for (v.i = 0; v.i < 3; v.i++) {
        if (gSavemap.party[v.i] == PARTY_CHAR_NONE) {
            continue;
        }
        v.flag = g_BattleScene.targetMask & (1 << v.i);
        switch (g_BattleSelectedUseCondition) {
        case BATTLE_USE_CONDITION_HP:
            if (v.flag != 0) {
                Menu_DrawDecimalNumber1x(
                    203, v.i * 16 + 0xbc,
                    g_BtlGaugeHpValues[v.i] >> 8, 4,
                    ((g_BtlGaugeHpValues[v.i] >> 8) >
                     v.stats[v.i].maxHp / 4) +
                        6,
                    0.301f);
            } else {
                Menu_DrawDecimalNumber1x(203, v.i * 16 + 0xbc,
                                    g_BtlGaugeHpValues[v.i] >> 8, 4, 2,
                                    0.301f);
            }
            Menu_DrawAtlasQuad1x(231, v.i * 16 + 0xbc, 216, 0, 4, 8, 1,
                                 0, 0, 0.301f);
            Menu_DrawDecimalNumber1x(
                235, v.i * 0x10 + 0xbc,
                v.stats[v.i].maxHp, 4,
                ((v.flag ? -1 : 0) & 5U) + 2, 0.301f);
            v.bar.x = 203;
            v.bar.y = (short)(v.i * 16 + 196);
            v.bar.width = 60;
            v.bar.height = 1;
            v.bar.low = (short)(g_BtlGaugeHpLow[v.i] >> 8);
            v.bar.high = (short)(g_BtlGaugeHpHigh[v.i] >> 8);
            v.bar.max = v.stats[v.i].maxHp;
            v.bar.direction = g_BtlGaugeHpDir[v.i];
            v.bar.colour = 0x800080ff;
            Menu_DrawResourceGauge1x((MenuGauge *)&v.bar, 0.301f);
            break;
        case BATTLE_USE_CONDITION_MP:
            if (v.flag != 0) {
                Menu_DrawDecimalNumber1x(
                    203, v.i * 16 + 0xbc,
                    g_BtlGaugeMpValues[v.i] >> 8, 4,
                    ((g_BtlGaugeMpValues[v.i] >> 8) >
                     v.stats[v.i].maxMp / 4) +
                        6,
                    0.301f);
            } else {
                Menu_DrawDecimalNumber1x(203, v.i * 16 + 0xbc,
                                    g_BtlGaugeMpValues[v.i] >> 8, 4, 2,
                                    0.301f);
            }
            Menu_DrawAtlasQuad1x(231, v.i * 16 + 0xbc, 216, 0, 4, 8, 1,
                                 0, 0, 0.301f);
            Menu_DrawDecimalNumber1x(
                235, v.i * 0x10 + 0xbc,
                v.stats[v.i].maxMp, 4,
                ((v.flag ? -1 : 0) & 5U) + 2, 0.301f);
            v.bar.x = 203;
            v.bar.y = (short)(v.i * 16 + 196);
            v.bar.width = 60;
            v.bar.height = 1;
            v.bar.low = (short)(g_BtlGaugeMpLow[v.i] >> 8);
            v.bar.high = (short)(g_BtlGaugeMpHigh[v.i] >> 8);
            v.bar.max = v.stats[v.i].maxMp;
            v.bar.direction = g_BtlGaugeMpDir[v.i];
            v.bar.colour = 0x8000ff80;
            Menu_DrawResourceGauge1x((MenuGauge *)&v.bar, 0.301f);
            break;
        case BATTLE_USE_CONDITION_STATUS:
            g_BattlePartyStatusIconMasks[v.i] &= ~BATTLE_STATUS_HUD_NON_CYCLING_MASK;
            if ((((g_BtlMenuFrameCounter & 0xf) == 0) &&
                 g_BattlePartyStatusIconMasks[v.i] != 0) ||
                ((((unsigned int *)&g_BattlePartyStatusIconMasks)[v.i] &
                  (1 << g_BattleHudStatusIconCycles[v.i])) == 0 &&
                 g_BattlePartyStatusIconMasks[v.i] != 0)) {
                do {
                    g_BattleHudStatusIconCycles[v.i]++;
                    g_BattleHudStatusIconCycles[v.i] %= 27;
                } while ((((unsigned int *)&g_BattlePartyStatusIconMasks)[v.i] &
                          (1 << g_BattleHudStatusIconCycles[v.i])) == 0);
            }
            if (g_BattlePartyStatusIconMasks[v.i] != 0) {
                Menu_DrawEncodedString1x(
                    182, v.i * 0x10 + 0xba,
                    (unsigned char *)&g_BtlStatusNameTable +
                        g_BattleHudStatusIconCycles[v.i] * 10,
                    3, 0.301f);
            }
            break;
        }
    }
    Menu_DrawAtlasQuad1x(86, 181, 128, 16, 26, 5, 1, 0, 0,
                         0.301f);
    switch (g_BattleSelectedUseCondition) {
    case BATTLE_USE_CONDITION_HP:
        Menu_DrawAtlasQuad1x(207, 181, 80, 0, 16, 5, 1, 0, 0,
                             0.301f);
        break;
    case BATTLE_USE_CONDITION_MP:
        Menu_DrawAtlasQuad1x(207, 181, 96, 0, 16, 5, 1, 0, 0,
                             0.301f);
        break;
    case BATTLE_USE_CONDITION_STATUS:
        Menu_DrawAtlasQuad1x(178, 181, 128, 24, 32, 5, 1, 0, 0,
                             0.301f);
        break;
    }
}

/*
 * Draw three or four rows of looked-up UI text down the right of the screen,
 * starting at y = 8 * g_BtlMenuControlledActionCursor[9]. Draw three or four rows of looked-up UI
 * text down the right of the screen, starting at ypos = 8 * g_BtlMenuControlledActionCursor[9].
 */
// FUNCTION: FF7 0x006e1f64
void Battle_DrawControlledEnemyActionList2x(void)
{
    short ypos;
    unsigned char *tab;
    int *list;
    short i;
    short num;
    int col;
    int pad;
    int junk[4];
    int n;

    tab = &g_BattleItemListBlock.enemyActionBytes[g_BtlEnemyIndex * 0x60];
    list = &g_BtlMenuControlledActionCursor;
    ypos = (short)(list[9] * 8);
    Menu_EmptyTextHook(9);
    if (ypos != 0) {
        num = 4;
    } else {
        num = 3;
    }
    Battle_BeginWindowTextDrawNoOp(BATTLE_MENU_MANIP_ACTION_LIST);
    for (i = 0; i < num; i++) {
        Menu_DrawEncodedString2x(116, ypos + i * 0x20 + 0x160,
                          (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ENEMY_ATTACK, tab[(list[5] + i) * 6], 8),
                          7, 0.501f);
    }
    Battle_EndWindowTextDrawNoOp(BATTLE_MENU_MANIP_ACTION_LIST);
}

/*
 * Battle_DrawControlledEnemyActionList2x's 1x twin: same locals, same frame with the same four
 * holes, and every constant halved -- 4 instead of 8 for the row pitch, 0x10 instead of 0x20 for
 * the step, 0xB0 instead of 0x160 for the top, 0x3A instead of 0x74 for the left edge, and
 * Menu_DrawEncodedString1x.
 */
// FUNCTION: FF7 0x006e2033
void Battle_DrawControlledEnemyActionList1x(void)
{
    short ypos;
    unsigned char *tab;
    int *list;
    short i;
    short num;
    int col;
    int pad;
    int junk[4];
    int n;

    tab = &g_BattleItemListBlock.enemyActionBytes[g_BtlEnemyIndex * 0x60];
    list = &g_BtlMenuControlledActionCursor;
    ypos = (short)(list[9] * 4);
    Menu_EmptyTextHook(9);
    if (ypos != 0) {
        num = 4;
    } else {
        num = 3;
    }
    Battle_BeginWindowTextDrawNoOp(BATTLE_MENU_MANIP_ACTION_LIST);
    for (i = 0; i < num; i++) {
        Menu_DrawEncodedString1x(58, ypos + i * 0x10 + 0xb0,
                             (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ENEMY_ATTACK, tab[(list[5] + i) * 6],
                                                 8),
                             7, 0.501f);
    }
    Battle_EndWindowTextDrawNoOp(BATTLE_MENU_MANIP_ACTION_LIST);
}

/*
 * Menu_CaitReelArm -- PSX func_800E4B88: arm the limit slot reels - clear the
 * three reel-stop flags, stop counter = 0, input delay = 10, reel speed index = 2.
 */
// FUNCTION: FF7 0x006e2102
void __cdecl Menu_CaitReelArm(void)
{
    int i;

    for (i = 0; i < 3; i++) {
        g_BtlGaugePartySlots[i] = 0;
    }
    g_BtlReelStopIndex = 0;
    g_BtlReelCountdown = 10;
    g_BtlReelSpeedIndex = 2;
}

/*
 * Menu_CaitReelState -- PSX func_800E4BCC: 0 = reels not armed, 2 = all three
 * reels stopped, 1 = still spinning.
 */
// FUNCTION: FF7 0x006e2143
int __cdecl Menu_CaitReelState(void)
{
    if (g_BtlGaugePartySlots[0] == 0) {
        return 0;
    }
    if (g_BtlReelStopIndex == 3) {
        return 2;
    }
    return 1;
}

/*
 * Advances and draws three slot reels within clip group 26. Moving reels increment
 * their counters; stopped reels continue to the configured table boundary, with
 * second/third reel alignment nudges and retained first-stop playtime gates
 * controlling matching behavior.
 */
// FUNCTION: FF7 0x006e2170
void Menu_DrawCaitReelPanel2x(void)
{
    struct {
        int drawX;
        int digit;
        int modeOffset;
        int modeWidth;
        int rowIdx;
        int reelIdx;
        int arg4;
        int offset;
        int arg3;
        int drawY;
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    Battle_BeginWindowTextDrawNoOp(BATTLE_MENU_CAIT_SITH_SLOTS);
    Menu_DrawTifaReelFadeBands2x(216, 354, 192, 100);
    for (s.reelIdx = 0; s.reelIdx < 3; s.reelIdx++) {
        if (g_BtlGaugePartySlots[s.reelIdx] == 0) {
            g_BtlReelPositions[s.reelIdx]++;
        } else if ((g_BtlReelPositions[s.reelIdx] & (g_BtlSquareData3[g_BtlReelSpeedIndex] - 1)) != 0) {
            g_BtlReelPositions[s.reelIdx]++;
        } else {
            switch (s.reelIdx) {
            case CAIT_REEL_COL_MIDDLE:
                if (g_CaitReelActive != 0 && (g_CaitReelSymbolState.reelSymbol) != g_CaitReelSymbolState.targetSymbol && (g_BtlGaugeReelState.reelCounter) != 0) {
                    g_BtlReelPositions[s.reelIdx]++;
                    (g_BtlGaugeReelState.reelCounter)--;
                }
                break;
            case CAIT_REEL_COL_RIGHT:
                if (g_CaitReelActive != 0) {
                    if (g_CaitReelSymbolState.targetSymbol != g_CaitReelSymbolState.lastSymbol && g_BtlGaugeReelState.countdown != 0) {
                        g_BtlReelPositions[s.reelIdx]++;
                        g_BtlGaugeReelState.countdown--;
                    }
                } else if ((g_CaitReelSymbolState.reelSymbol) == g_CaitReelSymbolState.targetSymbol && g_CaitReelSymbolState.targetSymbol == g_CaitReelSymbolState.lastSymbol) {
                    g_BtlReelPositions[s.reelIdx]++;
                }
                break;
            }
        }
    }

    switch (g_BtlActivePartyMember) {
    case BATTLE_PARTY_SLOT_0:
        s.modeWidth = 0x140;
        s.modeOffset = 0;
        break;
    case BATTLE_PARTY_SLOT_1:
        s.modeWidth = 0x140;
        s.modeOffset = 0x100;
        break;
    case BATTLE_PARTY_SLOT_2:
        s.modeWidth = 0x180;
        s.modeOffset = 0x100;
        break;
    }
    for (s.reelIdx = 0; s.reelIdx < 3; s.reelIdx++) {
        s.offset = (0x60 / (int)g_BtlSquareData3[g_BtlReelSpeedIndex]) *
                   (g_BtlReelPositions[s.reelIdx] & (g_BtlSquareData3[g_BtlReelSpeedIndex] - 1));
        s.digit = (g_BtlReelPositions[s.reelIdx] >> (g_BtlReelSpeedIndex + 1)) & 0xF;
        for (s.rowIdx = 0; s.rowIdx < 3; s.rowIdx++) {
            s.drawX = s.reelIdx * 64 + 216;
            s.drawY = s.offset + 260 + s.rowIdx * 96;
            s.arg3 = *(unsigned short *)&g_BtlSquareData2[0x60 + (s.reelIdx << 5) +
                                                      g_BtlSquareData1[(s.reelIdx << 4) + ((s.rowIdx - s.digit) & 0xF)] * 4];
            s.arg4 = *(unsigned short *)&g_BtlSquareData2[0x62 + (s.reelIdx << 5) +
                                                      g_BtlSquareData1[(s.reelIdx << 4) + ((s.rowIdx - s.digit) & 0xF)] * 4];
            Menu_DrawCaitReelSymbol2x(s.drawX, s.drawY, s.arg3, s.arg4,
                                      (g_BtlSquareData1[(s.reelIdx << 4) + ((s.rowIdx - s.digit) & 0xF)] != 0) + 1);
        }
    }
    if (g_BtlReelCountdown != 0) {
        g_BtlReelCountdown--;
    }
    if (Input_GetPressed(INPUT_CIRCLE) != 0 && g_BtlReelCountdown == 0 && g_GamePauseLatch == 0) {
        switch (g_BtlReelStopIndex) {
        case CAIT_REEL_COL_MIDDLE:
            if (g_CaitReelActive != 0) {
                (g_BtlGaugeReelState.reelCounter) = 4;
            }
            break;
        case CAIT_REEL_COL_RIGHT:
            if (g_CaitReelActive != 0) {
                g_BtlGaugeReelState.countdown = 4;
            }
            break;
        }
        if (g_BtlReelStopIndex == 0) {
            g_CaitReelActive = 0;
            switch ((g_CaitReelSymbolState.reelSymbol)) {
            case CAIT_REEL_SYMBOL_CAIT_SITH:
                if ((gSavemap.seconds & 0x3F) == 0) {
                    g_CaitReelActive = 1;
                }
                break;
            case CAIT_REEL_SYMBOL_BAR:
            case CAIT_REEL_SYMBOL_STAR:
                if ((gSavemap.seconds & 1) == 0) {
                    g_CaitReelActive = 1;
                }
                break;
            case CAIT_REEL_SYMBOL_HEART:
                if ((gSavemap.seconds & 3) == 0) {
                    g_CaitReelActive = 1;
                }
                break;
            case CAIT_REEL_SYMBOL_CROWN:
            case CAIT_REEL_SYMBOL_MOOGLE:
                g_CaitReelActive = 1;
                break;
            }
        }
        g_BtlGaugePartySlots[g_BtlReelStopIndex] = 1;
        if (g_BtlReelStopIndex < 4) {
            if (g_BtlReelStopIndex == 2) {
                SfxPlayOnChannel(0x40, 0x1E3, 4);
            }
            g_BtlReelStopIndex++;
        }
    }
    for (s.reelIdx = 0; s.reelIdx < 3; s.reelIdx++) {
        s.digit = ((g_BtlReelPositions[s.reelIdx] >> (g_BtlReelSpeedIndex + 1)) & 0xF) +
                  ((g_BtlReelPositions[s.reelIdx] & (g_BtlSquareData3[g_BtlReelSpeedIndex] - 1)) != 0);
        ((unsigned char *)&g_CaitReelSymbolState)[s.reelIdx] = g_BtlSquareData1[(s.reelIdx << 4) + ((1 - s.digit) & 0xF)];
    }
    Battle_EndWindowTextDrawNoOp(BATTLE_MENU_CAIT_SITH_SLOTS);
}

/*
 * Updates and draws the single-scale three-reel battle slot panel between menu
 * clipping helpers, drawing its fade bands first. Advances running reels and
 * stopped reels until the configured alignment; optional second/third-reel nudges
 * try to match symbols, while disabled matching keeps an all-equal third reel
 * moving.
 */
// FUNCTION: FF7 0x006e26b4
void Menu_DrawCaitReelPanel1x(void)
{
    struct {
        GfxTlVertex *vertBR, *vertTR, *vertBL, *vertTL;
        int drawX;
        int digit;
        int modeOffset;
        int modeWidth;
        int rowIdx;
        int reelIdx;
        int arg4;
        int offset;
        int arg3;
        int drawY;
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    Battle_BeginWindowTextDrawNoOp(BATTLE_MENU_CAIT_SITH_SLOTS);
    Menu_DrawTifaReelFadeBands1x(108, 177, 96, 50);
    for (s.reelIdx = 0; s.reelIdx < 3; s.reelIdx++) {
        if (g_BtlGaugePartySlots[s.reelIdx] == 0) {
            g_BtlReelPositions[s.reelIdx]++;
        } else if ((g_BtlReelPositions[s.reelIdx] & (g_BtlSquareData3[g_BtlReelSpeedIndex] - 1)) != 0) {
            g_BtlReelPositions[s.reelIdx]++;
        } else {
            switch (s.reelIdx) {
            case CAIT_REEL_COL_MIDDLE:
                if (g_CaitReelActive != 0 && (g_CaitReelSymbolState.reelSymbol) != g_CaitReelSymbolState.targetSymbol && (g_BtlGaugeReelState.reelCounter) != 0) {
                    g_BtlReelPositions[s.reelIdx]++;
                    (g_BtlGaugeReelState.reelCounter)--;
                }
                break;
            case CAIT_REEL_COL_RIGHT:
                if (g_CaitReelActive != 0) {
                    if (g_CaitReelSymbolState.targetSymbol != g_CaitReelSymbolState.lastSymbol && g_BtlGaugeReelState.countdown != 0) {
                        g_BtlReelPositions[s.reelIdx]++;
                        g_BtlGaugeReelState.countdown--;
                    }
                } else if ((g_CaitReelSymbolState.reelSymbol) == g_CaitReelSymbolState.targetSymbol && g_CaitReelSymbolState.targetSymbol == g_CaitReelSymbolState.lastSymbol) {
                    g_BtlReelPositions[s.reelIdx]++;
                }
                break;
            }
        }
    }

    switch (g_BtlActivePartyMember) {
    case BATTLE_PARTY_SLOT_0:
        s.modeWidth = 0x140;
        s.modeOffset = 0;
        break;
    case BATTLE_PARTY_SLOT_1:
        s.modeWidth = 0x140;
        s.modeOffset = 0x100;
        break;
    case BATTLE_PARTY_SLOT_2:
        s.modeWidth = 0x180;
        s.modeOffset = 0x100;
        break;
    }
    if (Menu_IsTextSubsystemReady() != 0 && AcquireBatchNode(9, g_BtlMenuRenderObjKetcy2) != 0) {
        for (s.reelIdx = 0; s.reelIdx < 3; s.reelIdx++) {
            s.offset = (48 / (int)g_BtlSquareData3[g_BtlReelSpeedIndex]) *
                       (g_BtlReelPositions[s.reelIdx] & (g_BtlSquareData3[g_BtlReelSpeedIndex] - 1));
            s.digit = (g_BtlReelPositions[s.reelIdx] >> (g_BtlReelSpeedIndex + 1)) & 15;
            for (s.rowIdx = 0; s.rowIdx < 3; s.rowIdx++) {
                s.drawX = (s.reelIdx << 5) + 108;
                s.drawY = s.offset + 130 + s.rowIdx * 48;
                s.arg3 = *(unsigned short *)&g_BtlSquareData2[(s.reelIdx << 5) + g_BtlSquareData1[(s.reelIdx << 4) + ((s.rowIdx - s.digit) & 0xf)] * 4];
                s.arg4 = *(unsigned short *)&g_BtlSquareData2[0x2 + (s.reelIdx << 5) + g_BtlSquareData1[(s.reelIdx << 4) + ((s.rowIdx - s.digit) & 0xf)] * 4];
                g_CaitReelPanelU = (float)s.arg3 * 0.0078125f;
                g_CaitReelPanelV = (float)s.arg4 * 0.00390625f;
                g_CaitReelPanelUSize = 0.25f;
                g_CaitReelPanelVSize = 0.1875f;
                s.drawX += g_MenuViewportX;
                s.drawY += g_MenuViewportY;
                s.vertTL = (GfxTlVertex *)(g_BtlMenuRenderObjKetcy2->vertexCursor);
                s.vertTL->x = (float)s.drawX;
                s.vertTL->y = (float)s.drawY;
                s.vertTL->z = 0.401f;
                s.vertTL->rhw = 1.0f;
                s.vertTL->colorUnsigned = 0x80ffffff;
                s.vertTL->specularUnsigned = 0xff000000;
                s.vertTL->u = g_CaitReelPanelU;
                s.vertTL->v = g_CaitReelPanelV;
                s.vertBL = (GfxTlVertex *)(g_BtlMenuRenderObjKetcy2->vertexCursor + 0x20);
                s.vertBL->x = (float)s.drawX;
                s.vertBL->y = (float)s.drawY + 48.0f;
                s.vertBL->z = 0.401f;
                s.vertBL->rhw = 1.0f;
                s.vertBL->colorUnsigned = 0x80ffffff;
                s.vertBL->specularUnsigned = 0xff000000;
                s.vertBL->u = g_CaitReelPanelU;
                s.vertBL->v = g_CaitReelPanelV + g_CaitReelPanelVSize;
                s.vertTR = (GfxTlVertex *)(g_BtlMenuRenderObjKetcy2->vertexCursor + 0x40);
                s.vertTR->x = (float)s.drawX + 32.0f;
                s.vertTR->y = (float)s.drawY;
                s.vertTR->z = 0.401f;
                s.vertTR->rhw = 1.0f;
                s.vertTR->colorUnsigned = 0x80ffffff;
                s.vertTR->specularUnsigned = 0xff000000;
                s.vertTR->u = g_CaitReelPanelU + g_CaitReelPanelUSize;
                s.vertTR->v = g_CaitReelPanelV;
                s.vertBR = (GfxTlVertex *)(g_BtlMenuRenderObjKetcy2->vertexCursor + 0x60);
                s.vertBR->x = (float)s.drawX + 32.0f;
                s.vertBR->y = (float)s.drawY + 48.0f;
                s.vertBR->z = 0.401f;
                s.vertBR->rhw = 1.0f;
                s.vertBR->colorUnsigned = 0x80ffffff;
                s.vertBR->specularUnsigned = 0xff000000;
                s.vertBR->u = g_CaitReelPanelU + g_CaitReelPanelUSize;
                s.vertBR->v = g_CaitReelPanelV + g_CaitReelPanelVSize;
                *g_BtlMenuRenderObjKetcy2->colorBytePtr = (g_BtlSquareData1[(s.reelIdx << 4) + ((s.rowIdx - s.digit) & 0xf)] != 0) + 1;
                g_BtlMenuRenderObjKetcy2->texturePage = (g_BtlSquareData1[(s.reelIdx << 4) + ((s.rowIdx - s.digit) & 0xf)] != 0) + 1;
                g_BtlMenuRenderObjKetcy2->vertexCursor += g_BtlMenuRenderObjKetcy2->cursorStep;
                (g_BtlMenuRenderObjKetcy2->colorBytePtr)++;
            }
        }
    }
    if (g_BtlReelCountdown != 0) {
        g_BtlReelCountdown--;
    }
    if (Input_GetPressed(INPUT_CIRCLE) != 0 && g_BtlReelCountdown == 0 && g_GamePauseLatch == 0) {
        switch (g_BtlReelStopIndex) {
        case CAIT_REEL_COL_MIDDLE:
            if (g_CaitReelActive != 0) {
                (g_BtlGaugeReelState.reelCounter) = 4;
            }
            break;
        case CAIT_REEL_COL_RIGHT:
            if (g_CaitReelActive != 0) {
                g_BtlGaugeReelState.countdown = 4;
            }
            break;
        }
        if (g_BtlReelStopIndex == 0) {
            g_CaitReelActive = 0;
            switch ((g_CaitReelSymbolState.reelSymbol)) {
            case CAIT_REEL_SYMBOL_CAIT_SITH:
                if ((gSavemap.seconds & 0x3F) == 0) {
                    g_CaitReelActive = 1;
                }
                break;
            case CAIT_REEL_SYMBOL_BAR:
            case CAIT_REEL_SYMBOL_STAR:
                if ((gSavemap.seconds & 1) == 0) {
                    g_CaitReelActive = 1;
                }
                break;
            case CAIT_REEL_SYMBOL_HEART:
                if ((gSavemap.seconds & 3) == 0) {
                    g_CaitReelActive = 1;
                }
                break;
            case CAIT_REEL_SYMBOL_CROWN:
            case CAIT_REEL_SYMBOL_MOOGLE:
                g_CaitReelActive = 1;
                break;
            }
        }
        g_BtlGaugePartySlots[g_BtlReelStopIndex] = 1;
        if (g_BtlReelStopIndex < 4) {
            if (g_BtlReelStopIndex == 2) {
                SfxPlayOnChannel(0x40, 0x1E3, 4);
            }
            g_BtlReelStopIndex++;
        }
    }
    for (s.reelIdx = 0; s.reelIdx < 3; s.reelIdx++) {
        s.digit = ((g_BtlReelPositions[s.reelIdx] >> (g_BtlReelSpeedIndex + 1)) & 0xF) +
                  ((g_BtlReelPositions[s.reelIdx] & (g_BtlSquareData3[g_BtlReelSpeedIndex] - 1)) != 0);
        ((unsigned char *)&g_CaitReelSymbolState)[s.reelIdx] = g_BtlSquareData1[(s.reelIdx << 4) + ((1 - s.digit) & 0xF)];
    }
    Battle_EndWindowTextDrawNoOp(BATTLE_MENU_CAIT_SITH_SLOTS);
}

/*
 * On pressed mask 0x20 with both input locks clear, updates the selected phase's
 * byte flags. Phase zero derives g_CaitReelActive from g_CaitReelSymbolState and low
 * bits of savemap seconds; phases 1/2 may set their result byte to 4.
 */
// FUNCTION: FF7 0x006e2e5b
void __cdecl Menu_CaitReelStopOnConfirm(void)
{
    if (Input_GetPressed(INPUT_CIRCLE) != 0 && g_BtlReelCountdown == 0 &&
        g_GamePauseLatch == 0) {
        switch (g_BtlReelStopIndex) {
        case CAIT_REEL_COL_MIDDLE:
            if (g_CaitReelActive != 0) {
                (g_BtlGaugeReelState.reelCounter) = 4;
            }
            break;
        case CAIT_REEL_COL_RIGHT:
            if (g_CaitReelActive != 0) {
                g_BtlGaugeReelState.countdown = 4;
            }
            break;
        }
        if (g_BtlReelStopIndex == 0) {
            g_CaitReelActive = 0;
            switch ((g_CaitReelSymbolState.reelSymbol)) {
            case CAIT_REEL_SYMBOL_CAIT_SITH:
                if ((gSavemap.seconds & 0x3f) == 0) {
                    g_CaitReelActive = 1;
                }
                break;
            case CAIT_REEL_SYMBOL_BAR:
            case CAIT_REEL_SYMBOL_STAR:
                if ((gSavemap.seconds & 1) == 0) {
                    g_CaitReelActive = 1;
                }
                break;
            case CAIT_REEL_SYMBOL_HEART:
                if ((gSavemap.seconds & 3) == 0) {
                    g_CaitReelActive = 1;
                }
                break;
            case CAIT_REEL_SYMBOL_CROWN:
            case CAIT_REEL_SYMBOL_MOOGLE:
                g_CaitReelActive = 1;
                break;
            }
        }
        g_BtlGaugePartySlots[g_BtlReelStopIndex] = 1;
        if (g_BtlReelStopIndex < 4) {
            if (g_BtlReelStopIndex == 2) {
                SfxPlayOnChannel(0x40, 0x1e3, 4);
            }
            g_BtlReelStopIndex++;
        }
    }
}

/*
 * Turn the three reel counters into the three digit tiles the slot screen draws.
 */
// FUNCTION: FF7 0x006e2fa9
void __cdecl Menu_BuildCaitReelResults(void)
{
    int reel;
    int digit;

    for (reel = 0; reel < 3; reel++) {
        digit = (g_BtlReelPositions[reel] >> (g_BtlReelSpeedIndex + 1)) & 0xf;
        g_BattleScene.squareSlots[reel] = g_BtlSquareData1[(reel << 4) + ((1 - digit) & 0xf)];
    }
}

/*
 * Rebuild the list of available limit levels for the current character. The limit
 * level comes out of the savemap and a four-case jump table turns it into how many
 * bits of gSavemap.chars[2].limits to look at: 3, 6, 9 or 0xA.
 */
// FUNCTION: FF7 0x006e300d
void __cdecl Menu_TifaReelArmAndBuildLimitList(void)
{
    int i;
    int limit;

    for (i = 0; i < 7; i++) {
        g_BtlGaugePartySlots[i] = 0;
    }
    g_BtlReelStopIndex = 0;
    g_BtlReelCountdown = 10;
    g_BtlReelSpeedIndex = 2;
    switch (gSavemap.chars[2].limitLevel) {
    case CHAR_LIMIT_LEVEL_1:
        limit = 3;
        break;
    case CHAR_LIMIT_LEVEL_2:
        limit = 6;
        break;
    case CHAR_LIMIT_LEVEL_3:
        limit = 9;
        break;
    case CHAR_LIMIT_LEVEL_4:
        limit = 10;
        break;
    }
    for (g_TifaReelCount = i = 0; i < limit; i++) {
        if ((gSavemap.chars[2].limits & (1 << i)) != 0) {
            g_TifaReelEntryIndex[g_TifaReelCount] = (unsigned char)i;
            g_TifaReelCount++;
        }
    }
}

/*
 * Menu_TifaReelState -- PSX func_800E54EC: 0 = reels not armed, 2 = every armed
 * reel stopped (counter equals armed-reel count), 1 = still spinning.
 */
// FUNCTION: FF7 0x006e3101
int __cdecl Menu_TifaReelState(void)
{
    if (g_BtlGaugePartySlots[0] == 0) {
        return 0;
    }
    if (g_BtlReelStopIndex == g_TifaReelCount) {
        return 2;
    }
    return 1;
}

/*
 * Draws the 2x multi-reel slot panel, selects five atlas tiles per reel from its
 * phase table and advances moving reels or finishes alignment of stopped reels.
 * Decrements the input delay and on eligible confirmation marks the selected reel
 * stopped before checking its index, advances selection and plays a sound after
 * the final reel.
 */
// FUNCTION: FF7 0x006e3135
void __cdecl Menu_DrawTifaReelPanel2x(void)
{
    int p;
    int tileV;
    int reel;
    int q;
    int srcTileIndex;
    int u;
    int panelLeft;

    panelLeft = 320 - (g_TifaReelCount * 64) / 2;
    Battle_BeginWindowTextDrawNoOp(BATTLE_MENU_TIFA_SLOTS);
    Menu_DrawTifaReelFadeBands2x(panelLeft, 354, g_TifaReelCount << 6, 100);
    for (reel = 0; reel < g_TifaReelCount; reel++) {
        for (srcTileIndex = 0; srcTileIndex < 5; srcTileIndex++) {
            switch (g_BtlSquareHandicapTexts[(g_TifaReelEntryIndex[reel] << 4) +
                                 ((srcTileIndex - g_BtlReelPositions[reel] / 4) & 0xf)]) {
            case TIFA_REEL_SYMBOL_YEAH:
                tileV = 0xa0;
                break;
            case TIFA_REEL_SYMBOL_HIT:
                tileV = 0xc0;
                break;
            case TIFA_REEL_SYMBOL_MISS:
                tileV = 0xe0;
                break;
            }
            if (g_BtlGaugePartySlots[reel] != 0) {
                Menu_DrawAtlasQuad2x(
                    panelLeft + reel * 64,
                    srcTileIndex * 32 + 0x144 + (g_BtlReelPositions[reel] & 3) * 8,
                    tileV, 0x40, 32, 16, 0xb, 0, 0, 0.401f);
            } else {
                Menu_DrawAtlasQuad2x(
                    panelLeft + reel * 64,
                    srcTileIndex * 32 + 0x144 + (g_BtlReelPositions[reel] & 7) * 4,
                    tileV, 0x40, 32, 16, 0xb, 0, 0, 0.401f);
            }
        }
    }
    Battle_EndWindowTextDrawNoOp(BATTLE_MENU_TIFA_SLOTS);
    for (reel = 0; reel < g_TifaReelCount; reel++) {
        if (g_BtlGaugePartySlots[reel] == 0) {
            g_BtlReelPositions[reel]++;
        } else if (g_BtlGaugePartySlots[reel] != 0 && (g_BtlReelPositions[reel] & 3) != 0) {
            g_BtlReelPositions[reel]++;
        }
    }
    if (g_BtlReelCountdown != 0) {
        g_BtlReelCountdown--;
    }
    if (Input_GetPressed(INPUT_CIRCLE) != 0 && g_BtlReelCountdown == 0 &&
        g_GamePauseLatch == 0) {
        g_BtlGaugePartySlots[g_BtlReelStopIndex] = 1;
        if (g_BtlReelStopIndex < g_TifaReelCount) {
            g_BtlReelStopIndex++;
            if (g_BtlReelStopIndex == g_TifaReelCount) {
                SfxPlayOnChannel(0x40, 0x1e3, 4);
            }
        }
    }
}

/*
 * Draws the 1x multi-reel slot panel, selects five atlas tiles per reel from its
 * phase table and advances moving reels or finishes alignment of stopped reels.
 * Decrements input delay and on eligible confirmation marks the selected reel
 * stopped before checking its index, advances selection and plays a sound after
 * the final reel.
 */
// FUNCTION: FF7 0x006e33ed
void __cdecl Menu_DrawTifaReelPanel1x(void)
{
    int p;
    int tileV;
    int reel;
    int q;
    int srcTileIndex;
    int u;
    int panelLeft;

    panelLeft = 160 - (g_TifaReelCount * 32) / 2;
    Battle_BeginWindowTextDrawNoOp(BATTLE_MENU_TIFA_SLOTS);
    Menu_DrawTifaReelFadeBands1x(panelLeft, 177, g_TifaReelCount << 5, 50);
    for (reel = 0; reel < g_TifaReelCount; reel++) {
        for (srcTileIndex = 0; srcTileIndex < 5; srcTileIndex++) {
            switch (g_BtlSquareHandicapTexts[(g_TifaReelEntryIndex[reel] << 4) +
                                 ((srcTileIndex - g_BtlReelPositions[reel] / 4) & 0xf)]) {
            case TIFA_REEL_SYMBOL_YEAH:
                tileV = 0xa0;
                break;
            case TIFA_REEL_SYMBOL_HIT:
                tileV = 0xc0;
                break;
            case TIFA_REEL_SYMBOL_MISS:
                tileV = 0xe0;
                break;
            }
            if (g_BtlGaugePartySlots[reel] != 0) {
                Menu_DrawAtlasQuad1x(
                    panelLeft + reel * 32,
                    srcTileIndex * 16 + 0xa2 + (g_BtlReelPositions[reel] & 3) * 4,
                    tileV, 64, 32, 16, 0xb, 0, 0, 0.401f);
            } else {
                Menu_DrawAtlasQuad1x(
                    panelLeft + reel * 32,
                    srcTileIndex * 16 + 0xa2 + (g_BtlReelPositions[reel] & 7) * 2,
                    tileV, 64, 32, 16, 0xb, 0, 0, 0.401f);
            }
        }
    }
    Battle_EndWindowTextDrawNoOp(BATTLE_MENU_TIFA_SLOTS);
    for (reel = 0; reel < g_TifaReelCount; reel++) {
        if (g_BtlGaugePartySlots[reel] == 0) {
            g_BtlReelPositions[reel]++;
        } else if (g_BtlGaugePartySlots[reel] != 0 && (g_BtlReelPositions[reel] & 3) != 0) {
            g_BtlReelPositions[reel]++;
        }
    }
    if (g_BtlReelCountdown != 0) {
        g_BtlReelCountdown--;
    }
    if (Input_GetPressed(INPUT_CIRCLE) != 0 && g_BtlReelCountdown == 0 &&
        g_GamePauseLatch == 0) {
        g_BtlGaugePartySlots[g_BtlReelStopIndex] = 1;
        if (g_BtlReelStopIndex < g_TifaReelCount) {
            g_BtlReelStopIndex++;
            if (g_BtlReelStopIndex == g_TifaReelCount) {
                SfxPlayOnChannel(0x40, 0x1e3, 4);
            }
        }
    }
}

/*
 * On a 0x20 input press when both input-delay and holdoff globals are zero, marks
 * the reel at the current stop counter. If the counter is below the armed-reel
 * count, increments it and plays the confirmation sound when it reaches that
 * count.
 */
// FUNCTION: FF7 0x006e36a5
void __cdecl Menu_TifaReelStopOnConfirm(void)
{
    if (Input_GetPressed(INPUT_CIRCLE) != 0 && g_BtlReelCountdown == 0 &&
        g_GamePauseLatch == 0) {
        g_BtlGaugePartySlots[g_BtlReelStopIndex] = 1;
        if (g_BtlReelStopIndex < g_TifaReelCount) {
            g_BtlReelStopIndex += 1;
            if (g_BtlReelStopIndex == g_TifaReelCount) {
                SfxPlayOnChannel(0x40, 0x1e3, 4);
            }
        }
    }
}

/*
 * One output byte per entry: a row picked by g_TifaReelEntryIndex and a column that counts
 * down from 2 as g_BtlReelPositions grows, four to the step, wrapped into four bits. The
 * row is shifted rather than multiplied, so the source shifts too, and the table has
 * to be flat for the row term to come first in the sum.
 */
// FUNCTION: FF7 0x006e3724
void __cdecl Battle_BuildLimitComboResults(void)
{
    int i;

    for (i = 0; i < g_TifaReelCount; i++) {
        g_BattleScene.turnQueueSlots[i] =
            g_BtlSquareHandicapTexts[(g_TifaReelEntryIndex[i] << 4) +
                         ((2 - g_BtlReelPositions[i] / 4) & 0xf)];
    }
}

/*
 * Mini_SlotResetSubStateAndInputDelay -- PSX func_800E58B0: reset Battle Square
 * handicap-slot sub-state and re-arm the 10-frame input delay.
 */
// FUNCTION: FF7 0x006e378b
void __cdecl Mini_SlotResetSubStateAndInputDelay(void)
{
    /* The original opens `sub esp, 8` and closes `mov esp, ebp` for a body
       that touches nothing but two globals: eight bytes of locals it never
       reads. Two unreferenced ints reproduce that exactly (an int[2] does
       too, so the shape is proven but the declaration is not). */
    int padWord;
    int unusedValue;

    g_BtlSlotSubState = 0;
    g_BtlReelCountdown = 10;
}

/*
 * Three entries out of a caller-supplied table into the working list, or the whole
 * working list cleared when there is no table. The source stride is written `* 7 *
 * 3` and not `* 21`: the original multiplies by 7 and by 3 separately.
 */
// FUNCTION: FF7 0x006e37a3
void __cdecl Mini_SlotLoadReelSymbols(void)
{
    int *base;
    int n;

    base = g_pBtlMenuUiTextures;
    if (base != 0) {
        for (n = 0; n < 3; n++) {
            g_BtlGaugeAnimStates[g_BtlMenuSlotIndex * 3 + n] =
                base[g_BtlMenuSlotSeed * 7 * 3 + n * 7 + g_BtlMenuSlotIndex];
        }
    } else {
        for (n = 0; n < 21; n++) {
            g_BtlGaugeAnimStates[n] = 0;
        }
    }
}

/*
 * Mini_SlotGetCurrentReelStopFlag -- PSX func_800E593C: return the stop flag of
 * the current handicap reel.
 */
// FUNCTION: FF7 0x006e3839
int __cdecl Mini_SlotGetCurrentReelStopFlag(void)
{
    /* Returns int, not unsigned char, so the byte is widened through a full register as in the original. */
    return g_TifaReelStoppedFlags[g_BtlMenuSlotIndex];
}

/*
 * Draws and advances the Battle Square slot screen according to g_BtlMenuSlotScreenState.
 * BATTLE_SLOT_SCREEN_CHOOSE asks "Keep goin'?" and shows the ten-digit battle points total.
 */
// FUNCTION: FF7 0x006e384f
void __cdecl Mini_DrawSlotScreen(void)
{
    int iconY;
    int r[4];
    int reelX;
    int reelIndexStep;
    int packedSum;
    int msgWidth;
    int atlasU;
    int atlasV;
    int iconIdx;
    int y;
    int z;
    int faceIndex;
    int k;
    int m;
    int subPanelLeft;
    int n;

    switch (g_BtlMenuSlotScreenState) {
    case BATTLE_SLOT_SCREEN_CHOOSE:
        gSavemap.slotIndex = g_BtlMenuSlotIndex + 1;
        Menu_DrawEncodedString2x(32, 364, &g_BtlSquareHandicapTexts[0xb8], 7, 0.401f);
        Menu_DrawEncodedString2x(92, 404, &g_BtlSquareHandicapTexts[0xc8], 7, 0.401f);
        packedSum = gSavemap.slotTotalByte0 + (gSavemap.slotTotalByte1 << 8) +
            (gSavemap.slotTotalByte2 << 16) + (gSavemap.slotTotalByte3 << 24);
        Menu_DrawEncodedString2x(352, 372, &g_BtlSquareHandicapTexts[0xe0], 5, 0.401f);
        Menu_DrawDecimalNumber2x(480, 404, packedSum, 10, 7, 0.401f);
        break;
    case BATTLE_SLOT_SCREEN_SPINNING:
        z = g_BtlMenuSlotIndex + 1;
        subPanelLeft = 320 - z * 64 / 2;
        Battle_BeginWindowTextDrawNoOp(BATTLE_MENU_ARENA_SLOT);
        Menu_DrawTifaReelFadeBands2x(subPanelLeft, 354, z * 64, 100);
        for (reelIndexStep = 0; reelIndexStep < z; reelIndexStep++) {
            for (iconIdx = 0; iconIdx < 4; iconIdx++) {
                faceIndex = g_BtlGaugeAnimStates[reelIndexStep * 3 +
                                 abs((iconIdx - 1 - g_TifaReelScrollSteps[reelIndexStep] / 8) % 3)] >>
                    24;
                atlasU = faceIndex % 5;
                atlasV = faceIndex / 5;
                atlasU <<= 6;
                atlasV <<= 6;
                iconY = iconIdx * 64 + (g_TifaReelScrollSteps[reelIndexStep] & 7) * 8 + 244;
                reelX = subPanelLeft + reelIndexStep * 64;
                Mini_DrawSlotReelIcon2x(reelX, iconY, atlasU, atlasV, g_BtlSquareHandicapTexts[0xa0 + faceIndex]);
            }
        }
        Battle_EndWindowTextDrawNoOp(BATTLE_MENU_ARENA_SLOT);
        for (reelIndexStep = 0; reelIndexStep < z; reelIndexStep++) {
            if (g_TifaReelStoppedFlags[reelIndexStep] == 0) {
                g_TifaReelScrollSteps[reelIndexStep] += 1;
            } else if (g_TifaReelStoppedFlags[reelIndexStep] != 0 &&
                       (g_TifaReelScrollSteps[reelIndexStep] & 7) != 0) {
                g_TifaReelScrollSteps[reelIndexStep] += 1;
            }
        }
        if (g_BtlReelCountdown != 0) {
            g_BtlReelCountdown -= 1;
        }
        if (Input_GetPressed(INPUT_CIRCLE) != 0 && g_BtlReelCountdown == 0 && g_GamePauseLatch == 0) {
            g_TifaReelStoppedFlags[g_BtlMenuSlotIndex] = 1;
            SfxPlayOnChannel(0x40, 0x1E3, 4);
        }
        break;
    case BATTLE_SLOT_SCREEN_START_PROMPT:
        msgWidth = Menu_MeasureEncodedString2x(&g_BtlSquareHandicapTexts[0xf6]);
        Menu_DrawEncodedString2x(0x140 - msgWidth / 2, 392, &g_BtlSquareHandicapTexts[0xf6],
                          g_BtlMenuFrameCounter & 7, 0.401f);
        break;
    case BATTLE_SLOT_SCREEN_RESULT:
        Menu_DrawEncodedString2x(274, 404, &g_BtlSquareHandicapTexts[0x138], 7, 0.401f);
        Menu_DrawEncodedString2x(170, 372,
                          &g_BtlSquareHandicapTexts[0x138] +
                              ((g_BtlGaugeAnimStates[g_BtlMenuSlotIndex * 3 +
                                             abs((1 - g_TifaReelScrollSteps[g_BtlMenuSlotIndex] / 8) % 3)] >>
                                24) +
                               1) *
                                  0x20,
                          g_BtlMenuFrameCounter & 7, 0.401f);
        break;
    case BATTLE_SLOT_SCREEN_INTRO:
        Menu_DrawEncodedString2x(0x140 - Menu_MeasureEncodedString2x(&g_BtlSquareHandicapTexts[0x10c]) / 2, 372,
                          &g_BtlSquareHandicapTexts[0x10c], g_BtlMenuFrameCounter & 7, 0.401f);
        break;
    }
}

/*
 * Draws and updates the low-resolution penalty-slot panel according to
 * g_BtlMenuSlotScreenState. BATTLE_SLOT_SCREEN_CHOOSE updates the reel-count byte and displays the
 * four-byte points total; BATTLE_SLOT_SCREEN_SPINNING centers g_BtlMenuSlotIndex+1 reels within clip group 28 and emits
 * four 32-by-32 icon quads per reel from the high-byte outcome table, with per-
 * icon palette selectors and explicit batch pointer advances.
 */
// FUNCTION: FF7 0x006e3c9c
void Mini_DrawSlotScreen1x(void)
{
    struct {
        float *vertBR;
        float *vertTR;
        float *vertBL;
        float *vertTL;
        int left;
        float dv;
        float du;
        int icon;
        float texV;
        int reels;
        float texU;
        int row;
        int sourceY;
        int sourceX;
        int total;
        int width;
        int reel;
        int unused[4];
        int drawX;
        int drawY;
    } v; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    switch (g_BtlMenuSlotScreenState) {
    case BATTLE_SLOT_SCREEN_CHOOSE:
        gSavemap.slotIndex = g_BtlMenuSlotIndex + 1;
        Menu_DrawEncodedString1x(16, 182, &g_BtlSquareHandicapTexts[0xb8], 7, 0.401f);
        Menu_DrawEncodedString1x(46, 202, &g_BtlSquareHandicapTexts[0xc8], 7, 0.401f);
        v.total = gSavemap.slotTotalByte0 + (gSavemap.slotTotalByte1 << 8) +
                  (gSavemap.slotTotalByte2 << 16) + (gSavemap.slotTotalByte3 << 24);
        Menu_DrawEncodedString1x(176, 186, &g_BtlSquareHandicapTexts[0xe0], 5, 0.401f);
        Menu_DrawDecimalNumber1x(240, 202, v.total, 10, 7, 0.401f);
        break;
    case BATTLE_SLOT_SCREEN_SPINNING:
        v.reels = g_BtlMenuSlotIndex + 1;
        v.left = 160 - v.reels * 32 / 2;
        Battle_BeginWindowTextDrawNoOp(BATTLE_MENU_ARENA_SLOT);
        Menu_DrawTifaReelFadeBands1x(v.left, 177, v.reels * 32, 50);
        if (Menu_IsTextSubsystemReady() && AcquireBatchNode(v.reels * 4, g_BtlMenuRenderObjColo)) {
            for (v.reel = 0; v.reel < v.reels; v.reel++) {
                for (v.row = 0; v.row < 4; v.row++) {
                    v.icon = g_BtlGaugeAnimStates[v.reel * 3 +
                                          abs((v.row - 1 - g_TifaReelScrollSteps[v.reel] / 8) % 3)] >>
                             24;
                    v.sourceX = v.icon % 5;
                    v.sourceY = v.icon / 5;
                    v.sourceX <<= 5;
                    v.sourceY <<= 5;
                    v.drawY = v.row * 32 + (g_TifaReelScrollSteps[v.reel] & 7) * 4 + 122;
                    v.drawX = v.left + v.reel * 32;
                    v.texU = (float)v.sourceX * 0.00390625f;
                    v.texV = (float)v.sourceY * 0.00390625f;
                    v.du = v.dv = 0.125f;
                    v.drawX += g_MenuViewportX;
                    v.drawY += g_MenuViewportY;
                    v.vertTL = g_BtlMenuRenderObjColo->vertexFloats;
                    v.vertTL[0] = (float)v.drawX;
                    v.vertTL[1] = (float)v.drawY;
                    v.vertTL[2] = 0.401f;
                    v.vertTL[3] = 1.0f;
                    ((unsigned int *)v.vertTL)[4] = 0x80ffffff;
                    ((unsigned int *)v.vertTL)[5] = 0xff000000;
                    v.vertTL[6] = v.texU;
                    v.vertTL[7] = v.texV;
                    v.vertBL = g_BtlMenuRenderObjColo->vertexFloats + 8;
                    v.vertBL[0] = (float)v.drawX;
                    v.vertBL[1] = (float)v.drawY + 32.0f;
                    v.vertBL[2] = 0.401f;
                    v.vertBL[3] = 1.0f;
                    ((unsigned int *)v.vertBL)[4] = 0x80ffffff;
                    ((unsigned int *)v.vertBL)[5] = 0xff000000;
                    v.vertBL[6] = v.texU;
                    v.vertBL[7] = v.texV + v.dv;
                    v.vertTR = g_BtlMenuRenderObjColo->vertexFloats + 16;
                    v.vertTR[0] = (float)v.drawX + 32.0f;
                    v.vertTR[1] = (float)v.drawY;
                    v.vertTR[2] = 0.401f;
                    v.vertTR[3] = 1.0f;
                    ((unsigned int *)v.vertTR)[4] = 0x80ffffff;
                    ((unsigned int *)v.vertTR)[5] = 0xff000000;
                    v.vertTR[6] = v.texU + v.du;
                    v.vertTR[7] = v.texV;
                    v.vertBR = g_BtlMenuRenderObjColo->vertexFloats + 24;
                    v.vertBR[0] = (float)v.drawX + 32.0f;
                    v.vertBR[1] = (float)v.drawY + 32.0f;
                    v.vertBR[2] = 0.401f;
                    v.vertBR[3] = 1.0f;
                    ((unsigned int *)v.vertBR)[4] = 0x80ffffff;
                    ((unsigned int *)v.vertBR)[5] = 0xff000000;
                    v.vertBR[6] = v.texU + v.du;
                    v.vertBR[7] = v.texV + v.dv;
                    *g_BtlMenuRenderObjColo->colorBytePtr = g_BtlSquareHandicapTexts[0xa0 + v.icon];
                    g_BtlMenuRenderObjColo->texturePage = g_BtlSquareHandicapTexts[0xa0 + v.icon];
                    *(int *)&g_BtlMenuRenderObjColo->vertexFloats += g_BtlMenuRenderObjColo->cursorStep;
                    (*(int *)&g_BtlMenuRenderObjColo->colorBytePtr)++;
                }
            }
        }
        Battle_EndWindowTextDrawNoOp(BATTLE_MENU_ARENA_SLOT);
        for (v.reel = 0; v.reel < v.reels; v.reel++) {
            if (g_TifaReelStoppedFlags[v.reel] == 0) {
                g_TifaReelScrollSteps[v.reel] += 1;
            } else if (g_TifaReelStoppedFlags[v.reel] != 0 && (g_TifaReelScrollSteps[v.reel] & 7) != 0) {
                g_TifaReelScrollSteps[v.reel] += 1;
            }
        }
        if (g_BtlReelCountdown != 0) {
            g_BtlReelCountdown -= 1;
        }
        if (Input_GetPressed(INPUT_CIRCLE) != 0 && g_BtlReelCountdown == 0 && g_GamePauseLatch == 0) {
            g_TifaReelStoppedFlags[g_BtlMenuSlotIndex] = 1;
            SfxPlayOnChannel(0x40, 0x1e3, 4);
        }
        break;
    case BATTLE_SLOT_SCREEN_START_PROMPT:
        v.width = Menu_MeasureEncodedString1x(&g_BtlSquareHandicapTexts[0xf6]);
        Menu_DrawEncodedString1x(0xa0 - v.width / 2, 196, &g_BtlSquareHandicapTexts[0xf6], g_BtlMenuFrameCounter & 7, 0.401f);
        break;
    case BATTLE_SLOT_SCREEN_RESULT:
        Menu_DrawEncodedString1x(137, 202, &g_BtlSquareHandicapTexts[0x138], 7, 0.401f);
        Menu_DrawEncodedString1x(85, 186, &g_BtlSquareHandicapTexts[0x138] + ((g_BtlGaugeAnimStates[g_BtlMenuSlotIndex * 3 + abs((1 - g_TifaReelScrollSteps[g_BtlMenuSlotIndex] / 8) % 3)] >> 24) + 1) * 0x20,
                             g_BtlMenuFrameCounter & 7, 0.401f);
        break;
    case BATTLE_SLOT_SCREEN_INTRO:
        Menu_DrawEncodedString1x(0xa0 - Menu_MeasureEncodedString1x(&g_BtlSquareHandicapTexts[0x10c]) / 2,
                             186, &g_BtlSquareHandicapTexts[0x10c], g_BtlMenuFrameCounter & 7, 0.401f);
        break;
    }
}

/*
 * On the confirm button, when input is not held off, mark the current slot as
 * taken and play the confirm sound.
 */
// FUNCTION: FF7 0x006e4304
void __cdecl Mini_SlotStopReelOnConfirm(void)
{
    if (Input_GetPressed(INPUT_CIRCLE) != 0 && g_BtlReelCountdown == 0 && g_GamePauseLatch == 0) {
        g_TifaReelStoppedFlags[g_BtlMenuSlotIndex] = 1;
        SfxPlayOnChannel(0x40, 0x1E3, 4);
    }
}

/*
 * Applies the selected slot outcome: adds its low 24-bit award to the four-byte
 * points total, selects character/party indices and dispatches its high-byte
 * effect. Effects modify equipment/stat fields, halve retained values, subtract
 * playtime-derived HP or levels, clear MP, or set flags; unsupported effects make
 * no direct change.
 */
// FUNCTION: FF7 0x006e434e
void __cdecl Mini_ApplySlotPenalty(void)
{
    int slot;
    int effect;
    int i;
    int points;
    int id;
    int which;

    which = g_BtlMenuSlotIndex - 1;
    points = gSavemap.slotTotalByte0 + (gSavemap.slotTotalByte1 << 8) +
             (gSavemap.slotTotalByte2 << 16) + (gSavemap.slotTotalByte3 << 24);
    points = points +
             (g_BtlGaugeAnimStates[which * 3 +
                           abs((1 - g_TifaReelScrollSteps[which] / 8) % 3)] &
              0xffffff);
    gSavemap.slotTotalByte0 = points & 0xff;
    gSavemap.slotTotalByte1 = (points >> 8) & 0xff;
    gSavemap.slotTotalByte2 = (points >> 16) & 0xff;
    gSavemap.slotTotalByte3 = (points >> 24) & 0xff;
    effect = g_BtlGaugeAnimStates[which * 3 +
                          abs((1 - g_TifaReelScrollSteps[which] / 8) % 3)] >>
             24;
    id = Party_GetLastFilledSlotCharId();
    slot = Party_GetLastFilledSlotIndex();
    g_BtlHandicapFlags = 0;
    switch (effect) {
    case BATTLE_SQUARE_HANDICAP_BREAK_MAGIC_MATERIA:
    case BATTLE_SQUARE_HANDICAP_BREAK_SUMMON_MATERIA:
    case BATTLE_SQUARE_HANDICAP_BREAK_SUPPORT_MATERIA:
    case BATTLE_SQUARE_HANDICAP_BREAK_INDEPENDENT_MATERIA:
    case BATTLE_SQUARE_HANDICAP_BREAK_COMMAND_MATERIA:
    case BATTLE_SQUARE_HANDICAP_BREAK_ALL_MATERIA:
        Mini_SlotPenaltyClearMatchingMateria(id, effect);
        *(int *)&g_BtlMenuWindowSets[slot * 0x700] = 0;
        ((BtlMenuActorFieldStride *)&g_BtlMenuSlotRecords)[slot].value = 0;
        break;
    case BATTLE_SQUARE_HANDICAP_BREAK_ACCESSORY:
        gSavemap.chars[id].accessory = 0xff;
        break;
    case BATTLE_SQUARE_HANDICAP_HALF_SPEED:
        gSavemap.chars[id].dexterity = gSavemap.chars[id].dexterity / 2;
        break;
    case BATTLE_SQUARE_HANDICAP_TIME_X30_DAMAGE:
        gSavemap.chars[id].hp -=
            Menu_PlaytimeToHoursDigits(gSavemap.seconds) * 30;
        break;
    case BATTLE_SQUARE_HANDICAP_DOWN_5_LEVELS:
        gSavemap.chars[id].level -= 5;
        break;
    case BATTLE_SQUARE_HANDICAP_DOWN_10_LEVELS:
        gSavemap.chars[id].level -= 0xa;
        break;
    case BATTLE_SQUARE_HANDICAP_HALF_HP:
        gSavemap.chars[id].baseHp =
            gSavemap.chars[id].baseHp / 2;
        break;
    case BATTLE_SQUARE_HANDICAP_HALF_MP:
        gSavemap.chars[id].baseMp =
            gSavemap.chars[id].baseMp / 2;
        break;
    case BATTLE_SQUARE_HANDICAP_HALF_HP_MP:
        gSavemap.chars[id].baseHp =
            gSavemap.chars[id].baseHp / 2;
        gSavemap.chars[id].baseMp =
            gSavemap.chars[id].baseMp / 2;
        break;
    case BATTLE_SQUARE_HANDICAP_ZERO_MP:
        gSavemap.chars[id].mp = 0;
        break;
    case BATTLE_SQUARE_HANDICAP_HP_RESTORED:
        g_BtlHandicapFlags |= BATTLE_SQUARE_HANDICAP_FLAG_HP_RESTORED;
        break;
    case BATTLE_SQUARE_HANDICAP_MINIMUM:
        g_BtlHandicapFlags |= BATTLE_SQUARE_HANDICAP_FLAG_MINIMUM;
        break;
    case BATTLE_SQUARE_HANDICAP_POISON:
        g_BtlHandicapFlags |= BATTLE_SQUARE_HANDICAP_FLAG_POISON;
        break;
    case BATTLE_SQUARE_HANDICAP_TOAD:
        g_BtlHandicapFlags |= BATTLE_SQUARE_HANDICAP_FLAG_TOAD;
        break;
    }
    if (gSavemap.chars[id].level > 100 || gSavemap.chars[id].level == 0) {
        gSavemap.chars[id].level = 1;
    }
    if (gSavemap.chars[id].hp > 9999 ||
        gSavemap.chars[id].hp == 0) {
        gSavemap.chars[id].hp = 1;
    }
    Savemap_BuildPartyStatsFromRecord(slot);
    Battle_LoadCombatantFromSavemap((unsigned char)slot);
    for (i = 0; i < g_BtlMenuSlotIndex; i++) {
        effect = g_BtlGaugeAnimStates[i * 3 +
                              abs((1 - g_TifaReelScrollSteps[i] / 8) % 3)] >>
                 24;
        switch (effect) {
        case BATTLE_SQUARE_HANDICAP_SEAL_ITEM_COMMAND:
            g_BtlHandicapFlags |= BATTLE_SQUARE_HANDICAP_FLAG_ITEM_SEALED;
            break;
        case BATTLE_SQUARE_HANDICAP_BREAK_ARMOR:
            g_MenuPartyStats[slot].defense =
                g_MenuPartyStats[slot].defense / 2;
            break;
        case BATTLE_SQUARE_HANDICAP_BREAK_WEAPON:
            g_MenuPartyStats[slot].attack =
                g_MenuPartyStats[slot].attack / 2;
            break;
        case BATTLE_SQUARE_HANDICAP_HALF_ACCURACY:
            g_MenuPartyStats[slot].slotPenaltyBase = g_MenuPartyStats[slot].slotPenaltyBase / 2;
            break;
        }
    }
    g_BtlGaugeHpValues[slot] =
        g_MenuPartyStats[slot].hp << 8;
    g_BtlGaugeMpValues[slot] =
        g_MenuPartyStats[slot].mp << 8;
    *(int *)&g_BtlMenuWindowSets[slot * 0x700] = 0;
    ((BtlMenuActorFieldStride *)&g_BtlMenuSlotRecords)[slot].value = 0;
}

/*
 * Returns the char id of the last filled party slot. BUG kept from 1998: ret stays uninitialized when all three slots are
 * 0xFF.
 */
// FUNCTION: FF7 0x006e4911
int __cdecl Party_GetLastFilledSlotCharId(void)
{
    /* Local names, not declaration order, decide the stack slots. */
    int i;
    int charId;

    for (i = 0; i < 3; i++) {
        if (gSavemap.party[i] != PARTY_CHAR_NONE) {
            charId = gSavemap.party[i];
        }
    }
    // BUG: charId is written only for a filled slot; with all three slots empty this
    // returns whatever was on the stack.
    return charId;
}

/*
 * Returns the index of the last filled party slot, twin of Party_GetLastFilledSlotCharId. Same 1998 bug: ret
 * uninitialized when the party is empty.
 */
// FUNCTION: FF7 0x006e4959
int __cdecl Party_GetLastFilledSlotIndex(void)
{
    /* Local names, not declaration order, decide the stack slots. */
    int i;
    int index;

    for (i = 0; i < 3; i++) {
        if (gSavemap.party[i] != PARTY_CHAR_NONE) {
            index = i;
        }
    }
    // BUG: index is written only for a filled slot; with all three slots empty this
    // returns whatever was on the stack.
    return index;
}

/*
 * Walks eight slot indices, checking the weapon slot then armour slot at each
 * index in the selected 0x84-byte character record. For each nonempty materia
 * word, obtains its type color and clears the word to -1 when
 * Mini_SlotPenaltyMateriaColorMatches(arg, type) accepts.
 */
// FUNCTION: FF7 0x006e4999
void __cdecl Mini_SlotPenaltyClearMatchingMateria(int recordIndex, int arg)
{
    int type;
    int i;

    for (i = 0; i < 8; i++) {
        if (gSavemap.chars[recordIndex].weaponMateria[i] !=
            -1) {
            type = Menu_GetMateriaTypeColorIndex(
                gSavemap.chars[recordIndex].weaponMateria[i]);
            if (Mini_SlotPenaltyMateriaColorMatches(arg, type) != 0) {
                gSavemap.chars[recordIndex].weaponMateria[i] =
                    -1;
            }
        }
        if (gSavemap.chars[recordIndex].armorMateria[i] !=
            -1) {
            type = Menu_GetMateriaTypeColorIndex(
                gSavemap.chars[recordIndex].armorMateria[i]);
            if (Mini_SlotPenaltyMateriaColorMatches(arg, type) != 0) {
                gSavemap.chars[recordIndex].armorMateria[i] =
                    -1;
            }
        }
    }
}

/*
 * Returns 1 when the materia colour index (value) is the one a "<type> Materia is broken."
 * handicap (kind) breaks, or for any value under BATTLE_SQUARE_HANDICAP_BREAK_ALL_MATERIA.
 * Returns zero for all other combinations.
 */
// FUNCTION: FF7 0x006e4a84
int __cdecl Mini_SlotPenaltyMateriaColorMatches(int kind, int value)
{
    int ok;

    ok = 0;
    switch (kind) {
    case BATTLE_SQUARE_HANDICAP_BREAK_MAGIC_MATERIA:
        if (value == MENU_MATERIA_COLOR_MAGIC) {
            ok = 1;
        }
        break;
    case BATTLE_SQUARE_HANDICAP_BREAK_SUMMON_MATERIA:
        if (value == MENU_MATERIA_COLOR_SUMMON) {
            ok = 1;
        }
        break;
    case BATTLE_SQUARE_HANDICAP_BREAK_SUPPORT_MATERIA:
        if (value == MENU_MATERIA_COLOR_SUPPORT) {
            ok = 1;
        }
        break;
    case BATTLE_SQUARE_HANDICAP_BREAK_INDEPENDENT_MATERIA:
        if (value == MENU_MATERIA_COLOR_INDEPENDENT) {
            ok = 1;
        }
        break;
    case BATTLE_SQUARE_HANDICAP_BREAK_COMMAND_MATERIA:
        if (value == MENU_MATERIA_COLOR_COMMAND) {
            ok = 1;
        }
        break;
    case BATTLE_SQUARE_HANDICAP_BREAK_ALL_MATERIA:
        ok = 1;
        break;
    }
    return ok;
}

/*
 * Width of the second choice line ("Yeah, a little") at 2x, used to place the
 * third line after it.
 */
// FUNCTION: FF7 0x006e4b18
int __cdecl Battle_MeasureWorriedSecondChoice2x(void)
{
    return Menu_MeasureEncodedString2x(g_BtlSquareHandicapTbl[1]);
}

/*
 * Width of the second choice line ("Yeah, a little") at 1x, used to place the
 * third line after it.
 */
// FUNCTION: FF7 0x006e4b2a
int __cdecl Battle_MeasureWorriedSecondChoice1x(void)
{
    return Menu_MeasureEncodedString1x(g_BtlSquareHandicapTbl[1]);
}

/*
 * Draws the three choice lines of the g_BtlSquareHandicapTbl table at 2x: the first at
 * (0x20, 0x16C), the second at (0x58, 0x194), the third after the second's width plus 0x98 on the
 * same row.
 */
// FUNCTION: FF7 0x006e4b3c
void __cdecl Battle_DrawWorriedChoiceLines2x(void)
{
    Menu_DrawEncodedString2x(32, 364, g_BtlSquareHandicapTbl[0], 7, 0.401f);
    Menu_DrawEncodedString2x(88, 404, g_BtlSquareHandicapTbl[1], 7, 0.401f);
    Menu_DrawEncodedString2x(Battle_MeasureWorriedSecondChoice2x() + 0x98, 404, g_BtlSquareHandicapTbl[2], 7, 0.401f);
}

/*
 * Draws the three choice lines of the g_BtlSquareHandicapTbl table at 1x: the first at
 * (0x10, 0xB6), the second at (0x2C, 0xCA), the third after the second's width plus 0x4C on the
 * same row.
 */
// FUNCTION: FF7 0x006e4b9b
void __cdecl Battle_DrawWorriedChoiceLines1x(void)
{
    Menu_DrawEncodedString1x(16, 182, g_BtlSquareHandicapTbl[0], 7, 0.401f);
    Menu_DrawEncodedString1x(44, 202, g_BtlSquareHandicapTbl[1], 7, 0.401f);
    Menu_DrawEncodedString1x(Battle_MeasureWorriedSecondChoice1x() + 0x4C, 202, g_BtlSquareHandicapTbl[2], 7, 0.401f);
}

/*
 * When text is ready, draws a half-alpha white 64-by-96 image at posX/posY, with
 * atlas origin atlasX/atlasY divided by 256 and paletteSel as palette selector. For atlasY
 * below 192, uses a single quad from g_BtlMenuRenderObjKetcy2a.
 */
// FUNCTION: FF7 0x006e4bf8
void __cdecl Menu_DrawCaitReelSymbol2x(int posX, int posY, int atlasX, int atlasY, int paletteSel)
{
    float uFrom;
    GfxTlVertex *bottomLeftVertA;
    GfxTlVertex *vertATopRemaining;
    GfxTlVertex *vertexATopRight;
    GfxTlVertex *pBottomRightVertB;
    GfxTlVertex *topRightVertexB;
    GfxTlVertex *vertBBottomLeft;
    GfxTlVertex *pSymbolBottomLeft;
    GfxTlVertex *symbolTopLeft;
    GfxTlVertex *symbolBottomRightPtr;
    GfxTlVertex *symTopRight;
    float textureV;
    float vSize;
    float uExtent;
    GfxTlVertex *vertexBottomRightPtr;
    GfxTlVertex *pVertBTopLeft;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    if (atlasY < 192) {
        if (AcquireBatchNode(1, g_BtlMenuRenderObjKetcy2a) != 0) {
            uFrom = atlasX * 0.00390625f;
            textureV = atlasY * 0.00390625f;
            uExtent = 0.25f;
            vSize = 0.375f;
            vertATopRemaining = (GfxTlVertex *)(g_BtlMenuRenderObjKetcy2a->vertexCursor);
            vertATopRemaining->x = posX;
            vertATopRemaining->y = posY;
            vertATopRemaining->zBits = FLOAT_0_401_BITS;
            vertATopRemaining->rhwBits = FLOAT_ONE_BITS;
            vertATopRemaining->color = 0x80FFFFFF;
            vertATopRemaining->specular = 0xFF000000;
            vertATopRemaining->u = uFrom;
            vertATopRemaining->v = textureV;
            bottomLeftVertA = (GfxTlVertex *)(g_BtlMenuRenderObjKetcy2a->vertexCursor + 0x20);
            bottomLeftVertA->x = posX;
            bottomLeftVertA->y = posY + 96.0f;
            bottomLeftVertA->zBits = FLOAT_0_401_BITS;
            bottomLeftVertA->rhwBits = FLOAT_ONE_BITS;
            bottomLeftVertA->color = 0x80FFFFFF;
            bottomLeftVertA->specular = 0xFF000000;
            bottomLeftVertA->u = uFrom;
            bottomLeftVertA->v = textureV + vSize;
            vertexATopRight = (GfxTlVertex *)(g_BtlMenuRenderObjKetcy2a->vertexCursor + 0x40);
            vertexATopRight->x = posX + 64.0f;
            vertexATopRight->y = posY;
            vertexATopRight->zBits = FLOAT_0_401_BITS;
            vertexATopRight->rhwBits = FLOAT_ONE_BITS;
            vertexATopRight->color = 0x80FFFFFF;
            vertexATopRight->specular = 0xFF000000;
            vertexATopRight->u = uFrom + uExtent;
            vertexATopRight->v = textureV;
            vertexBottomRightPtr = (GfxTlVertex *)(g_BtlMenuRenderObjKetcy2a->vertexCursor + 0x60);
            vertexBottomRightPtr->x = posX + 64.0f;
            vertexBottomRightPtr->y = posY + 96.0f;
            vertexBottomRightPtr->zBits = FLOAT_0_401_BITS;
            vertexBottomRightPtr->rhwBits = FLOAT_ONE_BITS;
            vertexBottomRightPtr->color = 0x80FFFFFF;
            vertexBottomRightPtr->specular = 0xFF000000;
            vertexBottomRightPtr->u = uFrom + uExtent;
            vertexBottomRightPtr->v = textureV + vSize;
            *(char *)g_BtlMenuRenderObjKetcy2a->colorIndex = paletteSel;
            g_BtlMenuRenderObjKetcy2a->paletteIndex = paletteSel;
        }
    } else {
        if (AcquireBatchNode(1, g_BtlMenuRenderObjKetcy2a) != 0) {
            uFrom = atlasX * 0.00390625f;
            textureV = atlasY * 0.00390625f;
            uExtent = 0.25f;
            vSize = 0.25f;
            pVertBTopLeft = (GfxTlVertex *)(g_BtlMenuRenderObjKetcy2a->vertexCursor);
            pVertBTopLeft->x = posX;
            pVertBTopLeft->y = posY;
            pVertBTopLeft->zBits = FLOAT_0_401_BITS;
            pVertBTopLeft->rhwBits = FLOAT_ONE_BITS;
            pVertBTopLeft->color = 0x80FFFFFF;
            pVertBTopLeft->specular = 0xFF000000;
            pVertBTopLeft->u = uFrom;
            pVertBTopLeft->v = textureV;
            vertBBottomLeft = (GfxTlVertex *)(g_BtlMenuRenderObjKetcy2a->vertexCursor + 0x20);
            vertBBottomLeft->x = posX;
            vertBBottomLeft->y = posY + 64.0f;
            vertBBottomLeft->zBits = FLOAT_0_401_BITS;
            vertBBottomLeft->rhwBits = FLOAT_ONE_BITS;
            vertBBottomLeft->color = 0x80FFFFFF;
            vertBBottomLeft->specular = 0xFF000000;
            vertBBottomLeft->u = uFrom;
            vertBBottomLeft->v = textureV + vSize;
            topRightVertexB = (GfxTlVertex *)(g_BtlMenuRenderObjKetcy2a->vertexCursor + 0x40);
            topRightVertexB->x = posX + 64.0f;
            topRightVertexB->y = posY;
            topRightVertexB->zBits = FLOAT_0_401_BITS;
            topRightVertexB->rhwBits = FLOAT_ONE_BITS;
            topRightVertexB->color = 0x80FFFFFF;
            topRightVertexB->specular = 0xFF000000;
            topRightVertexB->u = uFrom + uExtent;
            topRightVertexB->v = textureV;
            pBottomRightVertB = (GfxTlVertex *)(g_BtlMenuRenderObjKetcy2a->vertexCursor + 0x60);
            pBottomRightVertB->x = posX + 64.0f;
            pBottomRightVertB->y = posY + 64.0f;
            pBottomRightVertB->zBits = FLOAT_0_401_BITS;
            pBottomRightVertB->rhwBits = FLOAT_ONE_BITS;
            pBottomRightVertB->color = 0x80FFFFFF;
            pBottomRightVertB->specular = 0xFF000000;
            pBottomRightVertB->u = uFrom + uExtent;
            pBottomRightVertB->v = textureV + vSize;
            *(char *)g_BtlMenuRenderObjKetcy2a->colorIndex = paletteSel;
            g_BtlMenuRenderObjKetcy2a->paletteIndex = paletteSel;
        }
        if (AcquireBatchNode(1, g_BtlMenuRenderObjKetcy2b) != 0) {
            textureV = 0.0f;
            vSize = 1.0f;
            symbolTopLeft = (GfxTlVertex *)(g_BtlMenuRenderObjKetcy2b->vertexCursor);
            symbolTopLeft->x = posX;
            symbolTopLeft->y = posY + 64.0f;
            symbolTopLeft->zBits = FLOAT_0_401_BITS;
            symbolTopLeft->rhwBits = FLOAT_ONE_BITS;
            symbolTopLeft->color = 0x80FFFFFF;
            symbolTopLeft->specular = 0xFF000000;
            symbolTopLeft->u = uFrom;
            symbolTopLeft->v = textureV;
            pSymbolBottomLeft = (GfxTlVertex *)(g_BtlMenuRenderObjKetcy2b->vertexCursor + 0x20);
            pSymbolBottomLeft->x = posX;
            pSymbolBottomLeft->y = posY + 64.0f + 32.0f;
            pSymbolBottomLeft->zBits = FLOAT_0_401_BITS;
            pSymbolBottomLeft->rhwBits = FLOAT_ONE_BITS;
            pSymbolBottomLeft->color = 0x80FFFFFF;
            pSymbolBottomLeft->specular = 0xFF000000;
            pSymbolBottomLeft->u = uFrom;
            pSymbolBottomLeft->v = textureV + vSize;
            symTopRight = (GfxTlVertex *)(g_BtlMenuRenderObjKetcy2b->vertexCursor + 0x40);
            symTopRight->x = posX + 64.0f;
            symTopRight->y = posY + 64.0f;
            symTopRight->zBits = FLOAT_0_401_BITS;
            symTopRight->rhwBits = FLOAT_ONE_BITS;
            symTopRight->color = 0x80FFFFFF;
            symTopRight->specular = 0xFF000000;
            symTopRight->u = uFrom + uExtent;
            symTopRight->v = textureV;
            symbolBottomRightPtr = (GfxTlVertex *)(g_BtlMenuRenderObjKetcy2b->vertexCursor + 0x60);
            symbolBottomRightPtr->x = posX + 64.0f;
            symbolBottomRightPtr->y = posY + 64.0f + 32.0f;
            symbolBottomRightPtr->zBits = FLOAT_0_401_BITS;
            symbolBottomRightPtr->rhwBits = FLOAT_ONE_BITS;
            symbolBottomRightPtr->color = 0x80FFFFFF;
            symbolBottomRightPtr->specular = 0xFF000000;
            symbolBottomRightPtr->u = uFrom + uExtent;
            symbolBottomRightPtr->v = textureV + vSize;
            *(char *)g_BtlMenuRenderObjKetcy2b->colorIndex = paletteSel;
            g_BtlMenuRenderObjKetcy2b->paletteIndex = paletteSel;
        }
    }
}

/*
 * When text is ready, selects one of three atlas batches and writes a half-alpha
 * white 64-by-64 quad at posX/posY with fixed depth and paletteSel as palette selector.
 * If both atlas coordinates are below 256, scales both by 1/256 with UV extents
 * one quarter.
 */
// FUNCTION: FF7 0x006e51c1
void __cdecl Mini_DrawSlotReelIcon2x(int posX, int posY, int atlasX, int atlasY, int paletteSel)
{
    float uOriginF;
    GfxTlVertex *bottomLeftVertA;
    GfxTlVertex *vertATopRemaining;
    GfxTlVertex *vertexATopRight;
    GfxTlVertex *pBottomRightVertB;
    GfxTlVertex *topRightVertexB;
    GfxTlVertex *vertBBottomLeft;
    GfxTlVertex *pSymbolBottomLeft;
    GfxTlVertex *symbolTopLeft;
    GfxTlVertex *symbolBottomRightPtr;
    GfxTlVertex *symTopRight;
    float textureV;
    float vSize;
    float uExtent;
    GfxTlVertex *vertexBottomRightPtr;
    GfxTlVertex *pVertBTopLeft;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    if (atlasX < 256 && atlasY < 256) {
        if (AcquireBatchNode(1, g_BtlMenuRenderObjColoa) != 0) {
            uOriginF = atlasX * 0.00390625f;
            textureV = atlasY * 0.00390625f;
            vSize = 0.25f;
            uExtent = vSize;
            vertATopRemaining = (GfxTlVertex *)(g_BtlMenuRenderObjColoa->vertexCursor);
            vertATopRemaining->x = posX;
            vertATopRemaining->y = posY;
            vertATopRemaining->zBits = FLOAT_0_401_BITS;
            vertATopRemaining->rhwBits = FLOAT_ONE_BITS;
            vertATopRemaining->color = 0x80FFFFFF;
            vertATopRemaining->specular = 0xFF000000;
            vertATopRemaining->u = uOriginF;
            vertATopRemaining->v = textureV;
            bottomLeftVertA = (GfxTlVertex *)(g_BtlMenuRenderObjColoa->vertexCursor + 0x20);
            bottomLeftVertA->x = posX;
            bottomLeftVertA->y = posY + 64.0f;
            bottomLeftVertA->zBits = FLOAT_0_401_BITS;
            bottomLeftVertA->rhwBits = FLOAT_ONE_BITS;
            bottomLeftVertA->color = 0x80FFFFFF;
            bottomLeftVertA->specular = 0xFF000000;
            bottomLeftVertA->u = uOriginF;
            bottomLeftVertA->v = textureV + vSize;
            vertexATopRight = (GfxTlVertex *)(g_BtlMenuRenderObjColoa->vertexCursor + 0x40);
            vertexATopRight->x = posX + 64.0f;
            vertexATopRight->y = posY;
            vertexATopRight->zBits = FLOAT_0_401_BITS;
            vertexATopRight->rhwBits = FLOAT_ONE_BITS;
            vertexATopRight->color = 0x80FFFFFF;
            vertexATopRight->specular = 0xFF000000;
            vertexATopRight->u = uOriginF + uExtent;
            vertexATopRight->v = textureV;
            vertexBottomRightPtr = (GfxTlVertex *)(g_BtlMenuRenderObjColoa->vertexCursor + 0x60);
            vertexBottomRightPtr->x = posX + 64.0f;
            vertexBottomRightPtr->y = posY + 64.0f;
            vertexBottomRightPtr->zBits = FLOAT_0_401_BITS;
            vertexBottomRightPtr->rhwBits = FLOAT_ONE_BITS;
            vertexBottomRightPtr->color = 0x80FFFFFF;
            vertexBottomRightPtr->specular = 0xFF000000;
            vertexBottomRightPtr->u = uOriginF + uExtent;
            vertexBottomRightPtr->v = textureV + vSize;
            *(char *)g_BtlMenuRenderObjColoa->colorIndex = paletteSel;
            g_BtlMenuRenderObjColoa->paletteIndex = paletteSel;
        }
    } else if (atlasX > 255 && atlasY < 256) {
        if (AcquireBatchNode(1, g_BtlMenuRenderObjColob) != 0) {
            atlasX -= 256;
            textureV = atlasY * 0.00390625f;
            uExtent = 1.0f;
            vSize = 0.25f;
            uOriginF = atlasX * 0.015625f;
            pVertBTopLeft = (GfxTlVertex *)(g_BtlMenuRenderObjColob->vertexCursor);
            pVertBTopLeft->x = posX;
            pVertBTopLeft->y = posY;
            pVertBTopLeft->zBits = FLOAT_0_401_BITS;
            pVertBTopLeft->rhwBits = FLOAT_ONE_BITS;
            pVertBTopLeft->color = 0x80FFFFFF;
            pVertBTopLeft->specular = 0xFF000000;
            pVertBTopLeft->u = uOriginF;
            pVertBTopLeft->v = textureV;
            vertBBottomLeft = (GfxTlVertex *)(g_BtlMenuRenderObjColob->vertexCursor + 0x20);
            vertBBottomLeft->x = posX;
            vertBBottomLeft->y = posY + 64.0f;
            vertBBottomLeft->zBits = FLOAT_0_401_BITS;
            vertBBottomLeft->rhwBits = FLOAT_ONE_BITS;
            vertBBottomLeft->color = 0x80FFFFFF;
            vertBBottomLeft->specular = 0xFF000000;
            vertBBottomLeft->u = uOriginF;
            vertBBottomLeft->v = textureV + vSize;
            topRightVertexB = (GfxTlVertex *)(g_BtlMenuRenderObjColob->vertexCursor + 0x40);
            topRightVertexB->x = posX + 64.0f;
            topRightVertexB->y = posY;
            topRightVertexB->zBits = FLOAT_0_401_BITS;
            topRightVertexB->rhwBits = FLOAT_ONE_BITS;
            topRightVertexB->color = 0x80FFFFFF;
            topRightVertexB->specular = 0xFF000000;
            topRightVertexB->u = uOriginF + uExtent;
            topRightVertexB->v = textureV;
            pBottomRightVertB = (GfxTlVertex *)(g_BtlMenuRenderObjColob->vertexCursor + 0x60);
            pBottomRightVertB->x = posX + 64.0f;
            pBottomRightVertB->y = posY + 64.0f;
            pBottomRightVertB->zBits = FLOAT_0_401_BITS;
            pBottomRightVertB->rhwBits = FLOAT_ONE_BITS;
            pBottomRightVertB->color = 0x80FFFFFF;
            pBottomRightVertB->specular = 0xFF000000;
            pBottomRightVertB->u = uOriginF + uExtent;
            pBottomRightVertB->v = textureV + vSize;
            *(char *)g_BtlMenuRenderObjColob->colorIndex = paletteSel;
            g_BtlMenuRenderObjColob->paletteIndex = paletteSel;
        }
    } else if (AcquireBatchNode(1, g_BtlMenuRenderObjColoc) != 0) {
        atlasY -= 256;
        uOriginF = atlasX * 0.00390625f;
        uExtent = 0.25f;
        vSize = 1.0f;
        textureV = atlasY * 0.015625f;
        symbolTopLeft = (GfxTlVertex *)(g_BtlMenuRenderObjColoc->vertexCursor);
        symbolTopLeft->x = posX;
        symbolTopLeft->y = posY;
        symbolTopLeft->zBits = FLOAT_0_401_BITS;
        symbolTopLeft->rhwBits = FLOAT_ONE_BITS;
        symbolTopLeft->color = 0x80FFFFFF;
        symbolTopLeft->specular = 0xFF000000;
        symbolTopLeft->u = uOriginF;
        symbolTopLeft->v = textureV;
        pSymbolBottomLeft = (GfxTlVertex *)(g_BtlMenuRenderObjColoc->vertexCursor + 0x20);
        pSymbolBottomLeft->x = posX;
        pSymbolBottomLeft->y = posY + 64.0f;
        pSymbolBottomLeft->zBits = FLOAT_0_401_BITS;
        pSymbolBottomLeft->rhwBits = FLOAT_ONE_BITS;
        pSymbolBottomLeft->color = 0x80FFFFFF;
        pSymbolBottomLeft->specular = 0xFF000000;
        pSymbolBottomLeft->u = uOriginF;
        pSymbolBottomLeft->v = textureV + vSize;
        symTopRight = (GfxTlVertex *)(g_BtlMenuRenderObjColoc->vertexCursor + 0x40);
        symTopRight->x = posX + 64.0f;
        symTopRight->y = posY;
        symTopRight->zBits = FLOAT_0_401_BITS;
        symTopRight->rhwBits = FLOAT_ONE_BITS;
        symTopRight->color = 0x80FFFFFF;
        symTopRight->specular = 0xFF000000;
        symTopRight->u = uOriginF + uExtent;
        symTopRight->v = textureV;
        symbolBottomRightPtr = (GfxTlVertex *)(g_BtlMenuRenderObjColoc->vertexCursor + 0x60);
        symbolBottomRightPtr->x = posX + 64.0f;
        symbolBottomRightPtr->y = posY + 64.0f;
        symbolBottomRightPtr->zBits = FLOAT_0_401_BITS;
        symbolBottomRightPtr->rhwBits = FLOAT_ONE_BITS;
        symbolBottomRightPtr->color = 0x80FFFFFF;
        symbolBottomRightPtr->specular = 0xFF000000;
        symbolBottomRightPtr->u = uOriginF + uExtent;
        symbolBottomRightPtr->v = textureV + vSize;
        *(char *)g_BtlMenuRenderObjColoc->colorIndex = paletteSel;
        g_BtlMenuRenderObjColoc->paletteIndex = paletteSel;
    }
}

/*
 * Reset the battle menu's per-slot state: two counters, then fifteen ten-entry tables cleared in
 * one loop, then the scalars. The runs of `a = 0; b = a; c = b;` are chained assignments (each store is
 * reloaded before the next), and g_BtlGaugePulseStep is set to 4.
 */
// FUNCTION: FF7 0x006e57ce
void __cdecl Battle_ResetHudGaugeAndReelState(void)
{
    int i;

    g_BtlSelectedSpellAllCount = 0;
    g_BtlSelectedSpellQuadraLinks = 0;
    for (i = 0; i < 10; i++) {
        g_BtlGaugeHpLow[i] = 0;
        g_BtlGaugeMpLow[i] = 0;
        g_BtlGaugeHpHigh[i] = 0;
        g_BtlGaugeMpHigh[i] = 0;
        g_BtlGaugeHpValues[i] = 0;
        g_BtlGaugeMpValues[i] = 0;
        g_BtlGaugeUnusedSlotWordsA[i] = 0;
        g_BtlGaugeUnusedSlotWordsB[i] = 0;
        g_BtlGaugeHpDir[i] = 0;
        g_BtlGaugeMpDir[i] = 0;
        g_BtlReelPositions[i] = 0;
        g_BtlGaugePartySlots[i] = 0;
        g_TifaReelEntryIndex[i] = 0;
        g_TifaReelScrollSteps[i] = 0;
        g_TifaReelStoppedFlags[i] = 0;
    }
    g_PartyGaugeAnimPos = 0;
    g_BtlGaugePulseStep = 4;
    g_BtlGaugeSeeded = 0;
    g_BtlReelCountdown = 0;
    g_BtlReelStopIndex = 0;
    g_BtlReelSpeedIndex = 0;
    g_CaitReelActive = 0;
    g_CaitReelSymbolState.caitReelByte3 = 0;
    g_CaitReelSymbolState.lastSymbol = g_CaitReelSymbolState.caitReelByte3;
    g_CaitReelSymbolState.targetSymbol = g_CaitReelSymbolState.lastSymbol;
    (g_CaitReelSymbolState.reelSymbol) = g_CaitReelSymbolState.targetSymbol;
    g_BtlGaugeReelState.countdown = 0;
    (g_BtlGaugeReelState.reelCounter) = g_BtlGaugeReelState.countdown;
    g_TifaReelCount = 0;
    g_BtlHandicapFlags = 0;
    g_BattleHudStatusIconCycle2 = 0;
    g_BattleHudStatusIconCycle1 = g_BattleHudStatusIconCycle2;
    g_BattleHudStatusIconCycle = g_BattleHudStatusIconCycle1;
    g_BtlSlotSubState = 0;
    g_pBtlMenuUiTextures = 0;
}
} /* extern "C" */
