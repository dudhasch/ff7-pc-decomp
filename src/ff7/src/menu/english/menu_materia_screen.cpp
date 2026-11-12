// src/ff7/src/menu/english/menu_materia_screen.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The materia menu screen: lists of commands, magic, summons and enemy skills with their
//   costs, the detail and stat panels for a selected materia, the stock grid with its sorting, and
//   the exchange of materia between party slots, in both scales.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/menu.h"
#include "ff7/enums/menu.h"
#include "lib/enums/file.h"
#include "lib/enums/sound.h"
#include <string.h>
#include "ff7/battle.h"
#include <stdlib.h>
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/graphics.h"
#include "lib/api/file.h"
#include "lib/api/sound.h"
#include "ff7/types/battle.h"
#include "lib/api/token.h"
#include "ff7/savemap.h"
#include "ff7/enums/battle.h"
#include "ff7/enums/main.h"
extern "C" {

struct EnglishWindowFrame;

#pragma intrinsic(abs, memcpy, memset)

// clang-format off
// Four 36-byte encoded menu texts at a 0x24 stride. The second is 4 mod 8, where VC5 cannot start a
// global of 8 B or more, so the records are one array.
// Text: "Do you really want to remove this?" "I sure do!" "Wait a sec" "Sephiroth“……”"
// GLOBAL: FF7 0x00920b18
unsigned char g_MenuMateriaHelpPrompt[144] = {0x24, 0x4f, 0x00, 0x59, 0x4f, 0x55, 0x00, 0x52, 0x45, 0x41, 0x4c, 0x4c, 0x59, 0x00, 0x57, 0x41, 0x4e, 0x54, 0x00, 0x54, 0x4f, 0x00, 0x52, 0x45, 0x4d, 0x4f, 0x56, 0x45, 0x00, 0x54, 0x48, 0x49, 0x53, 0x1f, 0xff, 0x00, 0x29, 0x00, 0x53, 0x55, 0x52, 0x45, 0x00, 0x44, 0x4f, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x37, 0x41, 0x49, 0x54, 0x00, 0x41, 0x00, 0x53, 0x45, 0x43, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x45, 0x50, 0x48, 0x49, 0x52, 0x4f, 0x54, 0x48, 0xb2, 0xa9, 0xa9, 0xb3, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// The materia menu's 42 FF7-encoded 20-byte text records (WPN, ARM, ABILITY LIST, EQUIP EFFECT, CHECK, ARRANGE,
// ..., HP<->MP CHANGE): the code indexes them by number and pushes some by name.
// Text: "Wpn." "Arm." "Ability list" "Equip effect" "Check" "Arrange" "AP" "To next level" "MASTER"
//       "Arrange" "Exchange" "Remove all" "Trash" "Dexterity" "Cover" "Magic" "Luck" "MASTER"
//       "Strength" "Vitality" "Magic" "Magic def" "Dexterity" "Luck" "MaxHP" "MaxMP" "MaxHPUP"
//       "MaxMPUP" "EXP.UP" "Gil UP" "Encount Down" "Encount Up" "Meet Chocobos" "Pre-emptive"
//       "Long range attack" "Attack all" "Counter attack" "Attack-All" "Continuous Attack"
//       "HP MP change"
// GLOBAL: FF7 0x00920ba8
unsigned char g_MenuMateriaLabelTable[0x348] = {
    0x37, 0x50, 0x4e, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x21, 0x52, 0x4d, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x21, 0x42, 0x49, 0x4c, 0x49, 0x54, 0x59, 0x00, 0x4c, 0x49, 0x53, 0x54, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x25, 0x51, 0x55, 0x49, 0x50, 0x00, 0x45, 0x46, 0x46, 0x45, 0x43, 0x54, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x23, 0x48, 0x45, 0x43, 0x4b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x21, 0x52, 0x52, 0x41, 0x4e, 0x47, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x21, 0x30, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x34, 0x4f, 0x00, 0x4e, 0x45, 0x58, 0x54, 0x00, 0x4c, 0x45, 0x56, 0x45, 0x4c, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2d, 0x21, 0x33, 0x34, 0x25, 0x32, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x21, 0x52, 0x52, 0x41, 0x4e, 0x47, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x25, 0x58, 0x43, 0x48, 0x41, 0x4e, 0x47, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x32, 0x45, 0x4d, 0x4f, 0x56, 0x45, 0x00, 0x41, 0x4c, 0x4c, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x34, 0x52, 0x41, 0x53, 0x48, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x24, 0x45, 0x58, 0x54, 0x45, 0x52, 0x49, 0x54, 0x59, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x23, 0x4f, 0x56, 0x45, 0x52, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2d, 0x41, 0x47, 0x49, 0x43, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2c, 0x55, 0x43, 0x4b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2d, 0x21, 0x33, 0x34, 0x25, 0x32, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x33, 0x54, 0x52, 0x45, 0x4e, 0x47, 0x54, 0x48, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x36, 0x49, 0x54, 0x41, 0x4c, 0x49, 0x54, 0x59, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2d, 0x41, 0x47, 0x49, 0x43, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2d, 0x41, 0x47, 0x49, 0x43, 0x00, 0x44, 0x45, 0x46, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x24, 0x45, 0x58, 0x54, 0x45, 0x52, 0x49, 0x54, 0x59, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2c, 0x55, 0x43, 0x4b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2d, 0x41, 0x58, 0x28, 0x30, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2d, 0x41, 0x58, 0x2d, 0x30, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2d, 0x41, 0x58, 0x28, 0x30, 0x35, 0x30, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2d, 0x41, 0x58, 0x2d, 0x30, 0x35, 0x30, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x25, 0x38, 0x30, 0x0e, 0x35, 0x30, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x27, 0x49, 0x4c, 0x00, 0x35, 0x30, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x25, 0x4e, 0x43, 0x4f, 0x55, 0x4e, 0x54, 0x00, 0x24, 0x4f, 0x57, 0x4e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x25, 0x4e, 0x43, 0x4f, 0x55, 0x4e, 0x54, 0x00, 0x35, 0x50, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2d, 0x45, 0x45, 0x54, 0x00, 0x23, 0x48, 0x4f, 0x43, 0x4f, 0x42, 0x4f, 0x53, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x30, 0x52, 0x45, 0x0d, 0x45, 0x4d, 0x50, 0x54, 0x49, 0x56, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2c, 0x4f, 0x4e, 0x47, 0x00, 0x52, 0x41, 0x4e, 0x47, 0x45, 0x00, 0x41, 0x54, 0x54, 0x41, 0x43, 0x4b, 0xff, 0x00, 0x00,
    0x21, 0x54, 0x54, 0x41, 0x43, 0x4b, 0x00, 0x41, 0x4c, 0x4c, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x23, 0x4f, 0x55, 0x4e, 0x54, 0x45, 0x52, 0x00, 0x41, 0x54, 0x54, 0x41, 0x43, 0x4b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x21, 0x54, 0x54, 0x41, 0x43, 0x4b, 0x0d, 0x21, 0x4c, 0x4c, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x23, 0x4f, 0x4e, 0x54, 0x49, 0x4e, 0x55, 0x4f, 0x55, 0x53, 0x00, 0x21, 0x54, 0x54, 0x41, 0x43, 0x4b, 0xff, 0x00, 0x00,
    0x28, 0x30, 0x00, 0x2d, 0x30, 0x00, 0x43, 0x48, 0x41, 0x4e, 0x47, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// The 2x and 1x materia window layout tables: 11 EnglishWindowFrame rectangles each.
// GLOBAL: FF7 0x00920ef0
struct MenuMateriaWindow g_MenuMateriaWindowRects2x = {{{0, 0, 640, 146}, {0, 146, 640, 51}, {0, 189, 387, 291}, {380, 189, 260, 291}, {0, 189, 640, 291}, {247, 36, 127, 120}, {93, 26, 373, 109}, {0, 0, 533, 51}, {0, 51, 640, 52}, {380, 103, 260, 377}, {348, 103, 32, 377}}};
// GLOBAL: FF7 0x00920f48
struct MenuMateriaWindow g_MenuMateriaWindowRects1x = {{{0, 0, 320, 73}, {0, 73, 320, 26}, {0, 94, 194, 146}, {190, 94, 130, 146}, {0, 94, 320, 146}, {123, 18, 64, 60}, {47, 13, 187, 55}, {0, 0, 266, 26}, {0, 26, 320, 26}, {190, 51, 130, 189}, {174, 51, 16, 189}}};
// The materia menu state; VC5 pads the next table to 8.
// GLOBAL: FF7 0x00920fa0
int g_MenuMateriaState = -1;
// The second 2x/1x layout pair: a MenuMateriaSubRects each.
// GLOBAL: FF7 0x00920fa8
struct MenuMateriaSubRects g_MenuMateriaSubRects2x = {47, 214, {0}, 120, {47, 343, 418, 120}, {0}, 115, 120};
// GLOBAL: FF7 0x00920fc0
struct MenuMateriaSubRects g_MenuMateriaSubRects1x = {24, 107, {0}, 60, {24, 172, 209, 60}, {0}, 58, 60};
// GLOBAL: FF7 0x00920fd8
unsigned char g_MenuMateriaCursorCoords[8] = {0x64, 0x00, 0xad, 0x00, 0xfd, 0x00, 0x40, 0x01};
// GLOBAL: FF7 0x00920fe0
unsigned char g_MenuMagicCursorCoords[8] = {0x32, 0, 0x57, 0, 0x7f, 0, 0xa0};
// GLOBAL: FF7 0x00dca7f8
BattleCommandSlot *g_MenuMateriaCommandPtr = 0;
unsigned char g_bssPad_00dca7fc[4] = {0};
/* Contiguous 7-short scroll-bar descriptor passed to Menu_DrawScrollBar2x/1x via
 * &g_MenuMateriaScrollBar.visibleRows. */
// GLOBAL: FF7 0x00dca800
MenuScrollBarRows g_MenuMateriaScrollBar = {0};
// GLOBAL: FF7 0x00dca804
MenuScrollBarPos g_MenuMateriaScrollBarPos = {0};
// GLOBAL: FF7 0x00dca808
MenuScrollBarSize g_MenuMateriaScrollBarSize = {0};
// GLOBAL: FF7 0x00dca80c
short g_MenuMateriaScrollBarHeight = 0;
// GLOBAL: FF7 0x00dca810
struct SavemapCharacter *g_MenuMateriaActorEquipPtr = 0;
// GLOBAL: FF7 0x00dca814
int g_MenuMateriaSelectedSlot = 0;
unsigned char g_bssPad_00dca818[27136] = {0};
// GLOBAL: FF7 0x00dd1218
struct MenuMateriaWindow *g_MenuMateriaWindowPtr = 0;
unsigned char g_bssPad_00dd121c[4] = {0};
// GLOBAL: FF7 0x00dd1220
unsigned char g_MenuMateriaBuffer[0x80] = {0};
// GLOBAL: FF7 0x00dd12a0
int g_MenuMateriaScrollTop = 0;
// GLOBAL: FF7 0x00dd12a4
int g_MenuMateriaScrollMax = 0;
// GLOBAL: FF7 0x00dd12a8
void * g_MenuMateriaTextPtr = 0;
// GLOBAL: FF7 0x00dd12ac
MenuMateriaDetail *g_MenuMateriaDetailPtr = 0;
// GLOBAL: FF7 0x00dd12b0
int g_MenuMateriaTargetSlot = 0;
unsigned char g_bssPad_00dd12b4[4] = {0};
// The first of the materia screen's MenuCursor blocks: the code indexes them as an array
// (MenuCursor_Update at g_MenuMateriaState), and the TU's globals inside the array are aliases of
// their members.
// GLOBAL: FF7 0x00dd12b8
struct MenuMateriaCursorBlock g_MenuMateriaCursorBlock = {0};
// The rest of that first MenuCursor (its scrollState), indexed with the blocks.
// GLOBAL: FF7 0x00dd12e8
unsigned char g_MenuMateriaSubCursorBlock[0x8] = {0};
// GLOBAL: FF7 0x00dd12f0
int g_MenuMateriaActiveSlot = 0;
// GLOBAL: FF7 0x00dd12f4
int g_MenuMateriaEquipSlot = 0;
unsigned char g_bssPad_00dd12f8[104] = {0};
// GLOBAL: FF7 0x00dd1360
int g_MenuMateriaPartySlot = 0;
// GLOBAL: FF7 0x00dd1364
int g_MenuMateriaCharIndex = 0;
unsigned char g_bssPad_00dd1368[12] = {0};
// GLOBAL: FF7 0x00dd1374
int g_MenuMateriaItemIndex = 0;
unsigned char g_bssPad_00dd1378[24] = {0};
// GLOBAL: FF7 0x00dd1390
int g_MenuMateriaCommandCount = 0;
unsigned char g_bssPad_00dd1394[4] = {0};
// GLOBAL: FF7 0x00dd1398
int g_MenuMateriaCommandRow = 0;
// GLOBAL: FF7 0x00dd139c
int g_MenuMateriaCommandCol = 0;
unsigned char g_bssPad_00dd13a0[48] = {0};
// GLOBAL: FF7 0x00dd13d0
int g_MenuMateriaMagicCount = 0;
// GLOBAL: FF7 0x00dd13d4
int g_MenuMateriaMagicSelected = 0;
unsigned char g_bssPad_00dd13d8[12] = {0};
// GLOBAL: FF7 0x00dd13e4
unsigned char g_MenuMateriaMagicBuffer[4] = {0};
unsigned char g_bssPad_00dd13e8[12] = {0};
// GLOBAL: FF7 0x00dd13f4
int g_MenuMateriaSummonCount = 0;
unsigned char g_bssPad_00dd13f8[8] = {0};
// GLOBAL: FF7 0x00dd1400
int g_MenuMateriaEnemySkillCount = 0;
unsigned char g_bssPad_00dd1404[4] = {0};
// GLOBAL: FF7 0x00dd1408
int g_MenuMateriaStockCount = 0;
// GLOBAL: FF7 0x00dd140c
int g_MenuMateriaStockSelected = 0;
unsigned char g_bssPad_00dd1410[12] = {0};
// GLOBAL: FF7 0x00dd141c
int g_MenuMateriaStockScroll = 0;
unsigned char g_bssPad_00dd1420[12] = {0};
// GLOBAL: FF7 0x00dd142c
int g_MenuMateriaStockMax = 0;
unsigned char g_bssPad_00dd1430[8] = {0};
// GLOBAL: FF7 0x00dd1438
int g_MenuMateriaDetailFlag = 0;
unsigned char g_bssPad_00dd143c[4] = {0};
// GLOBAL: FF7 0x00dd1440
int g_MenuMateriaDetailAp = 0;
// GLOBAL: FF7 0x00dd1444
int g_MenuMateriaDetailLevel = 0;
unsigned char g_bssPad_00dd1448[12] = {0};
// GLOBAL: FF7 0x00dd1454
int g_MenuMateriaNextLevelAp = 0;
unsigned char g_bssPad_00dd1458[12] = {0};
// GLOBAL: FF7 0x00dd1464
int g_MenuMateriaCurrentAp = 0;
unsigned char g_bssPad_00dd1468[8] = {0};
// GLOBAL: FF7 0x00dd1470
int g_MenuMateriaEquipEffect = 0;
unsigned char g_bssPad_00dd1474[4] = {0};
// GLOBAL: FF7 0x00dd1478
int g_MenuMateriaAbilityCount = 0;
// GLOBAL: FF7 0x00dd147c
int g_MenuMateriaAbilitySelected = 0;
unsigned char g_bssPad_00dd1480[48] = {0};
// GLOBAL: FF7 0x00dd14b0
int g_MenuMateriaApBonus = 0;
// GLOBAL: FF7 0x00dd14b4
int g_MenuMateriaStatModIndex = 0;
unsigned char g_bssPad_00dd14b8[12] = {0};
// GLOBAL: FF7 0x00dd14c4
int g_MenuMateriaModifierCount = 0;
unsigned char g_bssPad_00dd14c8[24] = {0};
// GLOBAL: FF7 0x00dd14e0
int g_MenuMateriaStatDelta0 = 0;
unsigned char g_bssPad_00dd14e4[4] = {0};
// GLOBAL: FF7 0x00dd14e8
int g_MenuMateriaStatDelta1 = 0;
// GLOBAL: FF7 0x00dd14ec
int g_MenuMateriaStatDelta2 = 0;
unsigned char g_bssPad_00dd14f0[48] = {0};
// GLOBAL: FF7 0x00dd1520
int g_MenuMateriaStatDelta3 = 0;
// GLOBAL: FF7 0x00dd1524
int g_MenuMateriaStatDelta4 = 0;
unsigned char g_bssPad_00dd1528[12] = {0};
// GLOBAL: FF7 0x00dd1534
int g_MenuMateriaStatDelta5 = 0;
unsigned char g_bssPad_00dd1538[12] = {0};
// GLOBAL: FF7 0x00dd1544
int g_MenuMateriaStatDelta6 = 0;
unsigned char g_bssPad_00dd1548[8] = {0};
// GLOBAL: FF7 0x00dd1550
int g_MenuMateriaStatDelta7 = 0;
unsigned char g_bssPad_00dd1554[4] = {0};
// GLOBAL: FF7 0x00dd1558
int g_MenuMateriaExchangeSlotCursor = 0;
// GLOBAL: FF7 0x00dd155c
int g_MenuMateriaStatDelta9 = 0;
unsigned char g_bssPad_00dd1560[48] = {0};
// GLOBAL: FF7 0x00dd1590
int g_MenuMateriaSortMode = 0;
// GLOBAL: FF7 0x00dd1594
int g_MenuMateriaSortOrder = 0;
unsigned char g_bssPad_00dd1598[160] = {0};
// GLOBAL: FF7 0x00dd1638
int g_MenuMateriaPartyIndex = 0;
// GLOBAL: FF7 0x00dd163c
int g_MenuMateriaMemberIndex = 0;
// GLOBAL: FF7 0x00dd1640
int g_MenuMateriaExchangeCursor = 0;
// GLOBAL: FF7 0x00dd1644
MenuMateriaSubRects *g_pMenuMateriaSubRects = 0;
// GLOBAL: FF7 0x00dd1648
unsigned char g_MenuMateriaExchangeSlots[0xc] = {0};
// GLOBAL: FF7 0x00dd1654
unsigned char g_MenuMateriaInitializedFlag = 0;
unsigned char g_bssPad_00dd1658[4] = {0};
// GLOBAL: FF7 0x00dd165c
int g_MenuMateriaCursorParam0 = 0;
unsigned char g_bssPad_00dd1660[4] = {0};
// GLOBAL: FF7 0x00dd1664
int g_MenuMateriaCursorParam1 = 0;
// GLOBAL: FF7 0x00dd1668
int g_MenuMateriaExchangeRow = 0;
// GLOBAL: FF7 0x00dd166c
int g_MenuMateriaExchangeScrollTop = 0;
// GLOBAL: FF7 0x00dd1670
int g_MenuMateriaCursorParam4 = 0;
// GLOBAL: FF7 0x00dd1674
int g_MenuMateriaCursorParam5 = 0;
// GLOBAL: FF7 0x00dd1678
int g_MenuMateriaCursorParam6 = 0;
unsigned char g_bssPad_00dd167c[4] = {0};
// clang-format on

/*
 * Empty no-op function in materia details screen.
 */
// FUNCTION: FF7 0x00708670
void __cdecl Menu_MateriaDetailsNoOp(void)
{
    /* The original is empty. Its whole body is
       `55 8b ec 5d c3` -- 5 bytes at 0x00708670,
       push/mov/pop/ret plus alignment padding.
       The C body is empty, as in retail. */
}

/*
 * Look up command, magic, summon, or enemy skill name by mode and index.
 */
// FUNCTION: FF7 0x00708675
unsigned char *__cdecl Menu_LookupEntryNameByMode(int mode, int id)
{
    unsigned char *text;

    /* The materia case before the command case: the bodies sit in that order in the image. */
    switch (mode) {
    case MENU_MATERIA_NAME_MATERIA:
        text = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_MATERIA, id, 0);
        break;
    case MENU_MATERIA_NAME_COMMAND:
        text = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, id, 0);
        break;
    }
    return text;
}

/*
 * Draws the active actor's command list in 2x mode.
 */
// FUNCTION: FF7 0x007086be
void __cdecl Menu_DrawMateriaScreenCommandList2x(void)
{
    short rect[4];
    struct MenuPartyStats *statBlock;
    int row;
    int cmdIdx;
    int groupCnt;
    int bump;

    statBlock = &g_MenuPartyStats[g_MenuMateriaPartyIndex];
    groupCnt = statBlock->commandColumns;
    for (row = 0; row < groupCnt; row++) {
        for (cmdIdx = 0; cmdIdx < 4; cmdIdx++) {
            if (statBlock->commands[cmdIdx + row * 4].commandId != 0xff) {
                bump = (groupCnt > 1 && row > 0) ? 8 : 0;
                Menu_DrawEncodedString2x(g_pMenuMateriaSubRects->listX +
                                      (bump + row * 0x49) + 0xa,
                                  g_pMenuMateriaSubRects->listY + cmdIdx * 0x1a + 0xb,
                                  (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, statBlock->commands[cmdIdx + row * 4].commandId, 8),
                                  7, 0.1f);
            }
        }
    }
    Menu_SetRectShorts(rect, g_pMenuMateriaSubRects->listX, g_pMenuMateriaSubRects->listY,
                  ((short *)g_MenuMateriaTextPtr)[groupCnt - 1], g_pMenuMateriaSubRects->listHeight);
    Menu_DrawWindowFrame2x((EnglishWindowFrame *)rect, 0.101f);
}

/*
 * Draws the active actor's command list in 1x mode.
 */
// FUNCTION: FF7 0x00708817
void __cdecl Menu_DrawMateriaScreenCommandList1x(void)
{
    short rect[4];
    struct MenuPartyStats *statBlock;
    int row;
    int cmdIdx;
    int groupCnt;
    int bump;

    statBlock = &g_MenuPartyStats[g_MenuMateriaPartyIndex];
    groupCnt = statBlock->commandColumns;
    for (row = 0; row < groupCnt; row++) {
        for (cmdIdx = 0; cmdIdx < 4; cmdIdx++) {
            if (statBlock->commands[cmdIdx + row * 4].commandId != 0xff) {
                bump = (groupCnt > 1 && row > 0) ? 8 : 0;
                Menu_DrawEncodedString1x(g_pMenuMateriaSubRects->listX +
                                         (bump + row * 0x25) + 5,
                                     g_pMenuMateriaSubRects->listY + cmdIdx * 0xd + 6,
                                     (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, statBlock->commands[cmdIdx + row * 4].commandId, 8),
                                     7, 0.1f);
            }
        }
    }
    Menu_SetRectShorts(rect, g_pMenuMateriaSubRects->listX, g_pMenuMateriaSubRects->listY,
                  ((short *)g_MenuMateriaTextPtr)[groupCnt - 1], g_pMenuMateriaSubRects->listHeight);
    Menu_DrawWindowFrame1x((EnglishWindowFrame *)rect, 0.101f);
}

/*
 * Draws the active actor's magic list and costs in 2x mode.
 */
// FUNCTION: FF7 0x00708970
void __cdecl Menu_DrawMateriaScreenMagicList2x(unsigned char *attacks)
{
    short costRect[4];
    int usable;
    int firstEntry;
    BattleAbilitySlot *spellListPtr;
    /* Two adjacent loop-control slots. Keeping them in one array is
       required for VC5 to place them between s and the descriptor. */
    short scrollInfo[7];
    int loopRows[2];
    int colIndex;

    Menu_FlushAndSetScissor2x(g_pMenuMateriaSubRects->frame.x,
                              g_pMenuMateriaSubRects->frame.y + 6,
                              g_pMenuMateriaSubRects->frame.width,
                              g_pMenuMateriaSubRects->frame.height - 0xc);
    scrollInfo[0] = 3;
    scrollInfo[1] = 0x12;
    scrollInfo[2] = *(int *)&g_MenuMateriaMagicBuffer[0];
    scrollInfo[3] = g_pMenuMateriaSubRects->frame.x + 0x18d;
    scrollInfo[4] = g_pMenuMateriaSubRects->frame.y + 6;
    scrollInfo[5] = 0x11;
    scrollInfo[6] = 0x6c;
    Menu_DrawScrollBar2x(scrollInfo, 0.21f);
    spellListPtr = g_MenuPartyStats[g_MenuMateriaPartyIndex].spells;
    loopRows[0] = (*(int *)&g_MenuMateriaMagicBuffer[0x1c] != 0) + 3;
    firstEntry = *(int *)&g_MenuMateriaMagicBuffer[0] * 3;
    for (loopRows[1] = 0; loopRows[1] < loopRows[0]; loopRows[1]++) {
        for (colIndex = 0; colIndex < 3; colIndex++) {
            if (spellListPtr[colIndex + loopRows[1] * 3 + firstEntry].attackId != 0xff) {
                Menu_DrawEncodedString2x(
                    g_pMenuMateriaSubRects->frame.x + 0x23 + colIndex * 0x82,
                    g_pMenuMateriaSubRects->frame.y + 0xd + loopRows[1] * 0x22 +
                        *(int *)&g_MenuMateriaMagicBuffer[0x10] * 9,
                    (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_MAGIC, spellListPtr[colIndex + loopRows[1] * 3 + firstEntry].attackId, 8),
                    7, 0.21f);
            }
        }
    }
    for (loopRows[1] = 0; loopRows[1] < loopRows[0]; loopRows[1]++) {
        for (colIndex = 0; colIndex < 3; colIndex++) {
            if (spellListPtr[colIndex + firstEntry + loopRows[1] * 3].attackId != 0xff) {
                if ((spellListPtr[colIndex + firstEntry + loopRows[1] * 3].allCount != 0 ||
                     spellListPtr[colIndex + firstEntry + loopRows[1] * 3].quadraCount != 0) &&
                    (spellListPtr[colIndex + firstEntry + loopRows[1] * 3].targetFlags & 4) != 0 &&
                    (spellListPtr[colIndex + firstEntry + loopRows[1] * 3].targetFlags & 8) != 0) {
                    usable = 1;
                } else {
                    usable = 0;
                }
                if (usable != 0) {
                    Menu_DrawAtlasQuad2x(
                        g_pMenuMateriaSubRects->frame.x + 0x7a + colIndex * 0x82,
                        g_pMenuMateriaSubRects->frame.y + 0x11 + loopRows[1] * 0x22 +
                            *(int *)&g_MenuMateriaMagicBuffer[0x10] * 9,
                        0x80, 8, 8, 8, 2, 0, 0, 0.21f);
                }
            }
        }
    }
    Menu_FlushAndResetScissor2x();
    colIndex = g_pMenuMateriaSubRects->frame.x + g_pMenuMateriaSubRects->frame.width;
    loopRows[1] = g_pMenuMateriaSubRects->frame.y;
    Menu_DrawMateriaActionCostPanel2x(colIndex, loopRows[1],
                                      g_MenuMateriaMagicCount + g_MenuMateriaMagicSelected * 3 +
                                          *(int *)&g_MenuMateriaMagicBuffer[0] * 3,
                                      g_MenuMateriaPartyIndex, BATTLE_ABILITY_LIST_MAGIC);
    Menu_DrawWindowFrame2x(&g_pMenuMateriaSubRects->frame, 0.21f);
    Menu_SetRectShorts(costRect, colIndex, loopRows[1], g_pMenuMateriaSubRects->costWidth,
                  g_pMenuMateriaSubRects->costHeight);
    Menu_DrawWindowFrame2x((EnglishWindowFrame *)costRect, 0.21f);
}

/*
 * Draws MP cost or requirement panel for materia actions in 2x mode.
 */
// FUNCTION: FF7 0x00708cfc
void __cdecl Menu_DrawMateriaActionCostPanel2x(int baseX, int baseY, int entryIndex, int partyIndex, int mode)
{
    BattleAbilitySlot *entry;

    baseX += 0x14;
    Menu_DrawAtlasQuad2x(baseX + 5, baseY, 0, 0x10, 0x28, 0x10,
                         1, 0, 0, 0.0f);
    switch (mode) {
    case BATTLE_ABILITY_LIST_MAGIC:
        entry = g_MenuPartyStats[partyIndex].spells;
        if (entry[entryIndex].attackId != 0xff &&
            entry[entryIndex].allCount != 0) {
            Menu_DrawEncodedString2x(baseX - 8, baseY + 0x54,
                              (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, 0xe, 8), 7, 0.0f);
        }
        break;
    case BATTLE_ABILITY_LIST_SUMMON:
        entry = g_MenuPartyStats[partyIndex].summons;
        break;
    case BATTLE_ABILITY_LIST_ENEMY_SKILL:
        entry = g_MenuPartyStats[partyIndex].enemySkills;
        break;
    }
    if (entry[entryIndex].attackId != 0xff) {
        if (mode != BATTLE_ABILITY_LIST_ENEMY_SKILL) {
            if (entry[entryIndex].quadraCount != 0) {
                Menu_DrawEncodedString2x(baseX - 8, baseY + 0x3c,
                                  (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, 0xf, 8), 7,
                                  0.0f);
                if (entry[entryIndex].quadraCount != 0xff) {
                    Menu_DrawDecimalNumber2x(baseX + 0x20, baseY + 0x40,
                                      entry[entryIndex].quadraCount, 2, 7, 0.0f);
                } else {
                    Menu_DrawAtlasQuad2x(baseX + 0x20, baseY + 0x40,
                                         0x90, 0x60, 0x10, 8, 6, 0, 0,
                                         0.0f);
                }
                Menu_DrawAtlasQuad2x(baseX + 0x3a, baseY + 0x40,
                                     0x78, 8, 8, 8, 7, 0, 0, 0.0f);
            }
            if (entry[entryIndex].allCount != 0) {
                if (entry[entryIndex].allCount != 0xff) {
                    Menu_DrawDecimalNumber2x(baseX + 0x20, baseY + 0x5a,
                                      entry[entryIndex].allCount, 2, 7, 0.0f);
                } else {
                    Menu_DrawAtlasQuad2x(baseX + 0x20, baseY + 0x5a,
                                         0x90, 0x60, 0x10, 8, 6, 0, 0,
                                         0.0f);
                }
                Menu_DrawAtlasQuad2x(baseX + 0x3a, baseY + 0x5a,
                                     0x78, 8, 8, 8, 7, 0, 0, 0.0f);
            }
        }
        Menu_DrawDecimalNumber2x(baseX, baseY + 0x27,
                          entry[entryIndex].mpCost, 3, 7, 0.0f);
    }
    Menu_DrawDecimalNumber2x(
        baseX + 0x2c, baseY + 0x27,
        g_MenuPartyStats[partyIndex].mp,
        3, 7, 0.0f);
    Menu_DrawAtlasQuad2x(baseX + 0x23, baseY + 0x27, 0xd8, 0,
                         4, 8, 1, 0, 0, 0.0f);
}

/*
 * Draws the active actor's magic list and costs in 1x mode.
 */
// FUNCTION: FF7 0x00708fdf
void __cdecl Menu_DrawMateriaScreenMagicList1x(unsigned char *attacks)
{
    short costRect[4];
    int usable;
    int firstEntry;
    BattleAbilitySlot *spellListPtr;
    /* The 1x twin has the same two-slot loop-control local. */
    short scrollInfo[7];
    int loopRows[2];
    int colIndex;

    Menu_FlushAndSetScissor1x(g_pMenuMateriaSubRects->frame.x,
                              g_pMenuMateriaSubRects->frame.y + 3,
                              g_pMenuMateriaSubRects->frame.width,
                              g_pMenuMateriaSubRects->frame.height - 6);
    scrollInfo[0] = 3;
    scrollInfo[1] = 0x12;
    scrollInfo[2] = *(int *)&g_MenuMateriaMagicBuffer[0];
    scrollInfo[3] = g_pMenuMateriaSubRects->frame.x + 0xc7;
    scrollInfo[4] = g_pMenuMateriaSubRects->frame.y + 3;
    scrollInfo[5] = 9;
    scrollInfo[6] = 0x36;
    Menu_DrawScrollBar1x(scrollInfo, 0.21f);
    spellListPtr = g_MenuPartyStats[g_MenuMateriaPartyIndex].spells;
    loopRows[0] = (*(int *)&g_MenuMateriaMagicBuffer[0x1c] != 0) + 3;
    firstEntry = *(int *)&g_MenuMateriaMagicBuffer[0] * 3;
    for (loopRows[1] = 0; loopRows[1] < loopRows[0]; loopRows[1]++) {
        for (colIndex = 0; colIndex < 3; colIndex++) {
            if (spellListPtr[colIndex + loopRows[1] * 3 + firstEntry].attackId != 0xff) {
                Menu_DrawEncodedString1x(
                    g_pMenuMateriaSubRects->frame.x + 0x11 + colIndex * 0x41,
                    g_pMenuMateriaSubRects->frame.y + 7 + loopRows[1] * 0x11 +
                        *(int *)&g_MenuMateriaMagicBuffer[0x10] * 5,
                    (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_MAGIC, spellListPtr[colIndex + loopRows[1] * 3 + firstEntry].attackId, 8),
                    7, 0.21f);
            }
        }
    }
    for (loopRows[1] = 0; loopRows[1] < loopRows[0]; loopRows[1]++) {
        for (colIndex = 0; colIndex < 3; colIndex++) {
            if (spellListPtr[colIndex + firstEntry + loopRows[1] * 3].attackId != 0xff) {
                if ((spellListPtr[colIndex + firstEntry + loopRows[1] * 3].allCount != 0 ||
                     spellListPtr[colIndex + firstEntry + loopRows[1] * 3].quadraCount != 0) &&
                    (spellListPtr[colIndex + firstEntry + loopRows[1] * 3].targetFlags & 4) != 0 &&
                    (spellListPtr[colIndex + firstEntry + loopRows[1] * 3].targetFlags & 8) != 0) {
                    usable = 1;
                } else {
                    usable = 0;
                }
                if (usable != 0) {
                    Menu_DrawAtlasQuad1x(
                        g_pMenuMateriaSubRects->frame.x + 0x3d + colIndex * 0x41,
                        g_pMenuMateriaSubRects->frame.y + 9 + loopRows[1] * 0x11 +
                            *(int *)&g_MenuMateriaMagicBuffer[0x10] * 9,
                        0x80, 8, 8, 8, 2, 0, 0, 0.21f);
                }
            }
        }
    }
    Menu_FlushAndResetScissor1x();
    colIndex = g_pMenuMateriaSubRects->frame.x + g_pMenuMateriaSubRects->frame.width;
    loopRows[1] = g_pMenuMateriaSubRects->frame.y;
    Menu_DrawMateriaActionCostPanel1x(colIndex, loopRows[1],
                                      g_MenuMateriaMagicCount + g_MenuMateriaMagicSelected * 3 +
                                          *(int *)&g_MenuMateriaMagicBuffer[0] * 3,
                                      g_MenuMateriaPartyIndex, BATTLE_ABILITY_LIST_MAGIC);
    Menu_DrawWindowFrame1x(&g_pMenuMateriaSubRects->frame, 0.21f);
    Menu_SetRectShorts(costRect, colIndex, loopRows[1], g_pMenuMateriaSubRects->costWidth,
                  g_pMenuMateriaSubRects->costHeight);
    Menu_DrawWindowFrame1x((EnglishWindowFrame *)costRect, 0.21f);
}

/*
 * Draws MP cost or requirement panel for materia actions in 1x mode.
 */
// FUNCTION: FF7 0x00709365
void __cdecl Menu_DrawMateriaActionCostPanel1x(int baseX, int baseY, int entryIndex, int partyIndex, int mode)
{
    BattleAbilitySlot *entry;

    baseX += 10;
    Menu_DrawAtlasQuad1x(baseX + 3, baseY, 0, 0x10, 0x28, 0x10,
                         1, 0, 0, 0.01f);
    switch (mode) {
    case BATTLE_ABILITY_LIST_MAGIC:
        entry = g_MenuPartyStats[partyIndex].spells;
        if (entry[entryIndex].attackId != 0xff &&
            entry[entryIndex].allCount != 0) {
            Menu_DrawEncodedString1x(baseX - 4, baseY + 0x2a,
                                 (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, 0xe, 8), 7,
                                 0.0f);
        }
        break;
    case BATTLE_ABILITY_LIST_SUMMON:
        entry = g_MenuPartyStats[partyIndex].summons;
        break;
    case BATTLE_ABILITY_LIST_ENEMY_SKILL:
        entry = g_MenuPartyStats[partyIndex].enemySkills;
        break;
    }
    if (entry[entryIndex].attackId != 0xff) {
        if (mode != BATTLE_ABILITY_LIST_ENEMY_SKILL) {
            if (entry[entryIndex].quadraCount != 0) {
                Menu_DrawEncodedString1x(baseX - 4, baseY + 0x1e,
                                     (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, 0xf, 8), 7,
                                     0.0f);
                if (entry[entryIndex].quadraCount != 0xff) {
                    Menu_DrawDecimalNumber1x(baseX + 0x10, baseY + 0x20,
                                        entry[entryIndex].quadraCount, 2, 7,
                                        0.0f);
                } else {
                    Menu_DrawAtlasQuad1x(baseX + 0x10, baseY + 0x20,
                                         0x90, 0x60, 0x10, 8, 6, 0, 0,
                                         0.0f);
                }
                Menu_DrawAtlasQuad1x(baseX + 0x1d, baseY + 0x20,
                                     0x78, 8, 8, 8, 7, 0, 0, 0.0f);
            }
            if (entry[entryIndex].allCount != 0) {
                if (entry[entryIndex].allCount != 0xff) {
                    Menu_DrawDecimalNumber1x(baseX + 0x10, baseY + 0x2d,
                                        entry[entryIndex].allCount, 2, 7,
                                        0.0f);
                } else {
                    Menu_DrawAtlasQuad1x(baseX + 0x10, baseY + 0x2d,
                                         0x90, 0x60, 0x10, 8, 6, 0, 0,
                                         0.0f);
                }
                Menu_DrawAtlasQuad1x(baseX + 0x1d, baseY + 0x2d,
                                     0x78, 8, 8, 8, 7, 0, 0, 0.0f);
            }
        }
        Menu_DrawDecimalNumber1x(baseX, baseY + 0x14,
                            entry[entryIndex].mpCost, 3, 7, 0.0f);
    }
    Menu_DrawDecimalNumber1x(
        baseX + 0x16, baseY + 0x14,
        g_MenuPartyStats[partyIndex].mp,
        3, 7, 0.0f);
    Menu_DrawAtlasQuad1x(baseX + 0x12, baseY + 0x14, 0xd8, 0,
                         4, 8, 1, 0, 0, 0.0f);
}

/*
 * Draws summon materia action list in 2x mode.
 */
// FUNCTION: FF7 0x0070964b
void __cdecl Menu_DrawMateriaScreenSummonList2x(unsigned char *attacks)
{
    int rectX;
    int firstEnt;
    short costRectangle[4];
    int visibleRows;
    int rowIdx;
    short barDesc[7];
    BattleAbilitySlot *summonTable;

    Menu_FlushAndSetScissor2x(g_pMenuMateriaSubRects->frame.x,
                              g_pMenuMateriaSubRects->frame.y + 6,
                              g_pMenuMateriaSubRects->frame.width,
                              g_pMenuMateriaSubRects->frame.height - 0xc);
    barDesc[0] = 3;
    barDesc[1] = 0x10;
    barDesc[2] = g_MenuMateriaStockScroll;
    barDesc[3] = g_pMenuMateriaSubRects->frame.x + 0x18d;
    barDesc[4] = g_pMenuMateriaSubRects->frame.y + 6;
    barDesc[5] = 0x11;
    barDesc[6] = 0x6c;
    Menu_DrawScrollBar2x(barDesc, 0.1f);
    summonTable = g_MenuPartyStats[g_MenuMateriaPartyIndex].summons;
    visibleRows = (g_MenuMateriaDetailFlag != 0) + 3;
    firstEnt = g_MenuMateriaStockScroll;
    for (rowIdx = 0; rowIdx < visibleRows; rowIdx++) {
        if (summonTable[rowIdx + firstEnt].attackId != 0xff) {
            Menu_DrawEncodedString2x(
                g_pMenuMateriaSubRects->frame.x + 0x5d,
                g_pMenuMateriaSubRects->frame.y + 0xd + rowIdx * 0x22 +
                    g_MenuMateriaStockMax * 9,
                (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_SUMMON, summonTable[rowIdx + firstEnt].attackId, 8),
                7, 0.1f);
        }
    }
    Menu_FlushAndResetScissor2x();
    rectX = g_pMenuMateriaSubRects->frame.x + g_pMenuMateriaSubRects->frame.width;
    rowIdx = g_pMenuMateriaSubRects->frame.y;
    Menu_DrawMateriaActionCostPanel2x(rectX, rowIdx, g_MenuMateriaStockCount + g_MenuMateriaStockSelected + g_MenuMateriaStockScroll,
                                      g_MenuMateriaPartyIndex, BATTLE_ABILITY_LIST_SUMMON);
    Menu_DrawWindowFrame2x(&g_pMenuMateriaSubRects->frame, 0.1f);
    Menu_SetRectShorts(costRectangle, rectX, rowIdx, g_pMenuMateriaSubRects->costWidth,
                  g_pMenuMateriaSubRects->costHeight);
    Menu_DrawWindowFrame2x((EnglishWindowFrame *)costRectangle, 0.1f);
}

/*
 * Draws summon materia action list in 1x mode.
 */
// FUNCTION: FF7 0x00709843
void __cdecl Menu_DrawMateriaScreenSummonList1x(unsigned char *attacks)
{
    int rectX;
    int firstEnt;
    short costRectangle[4];
    int visibleRows;
    int rowIdx;
    short barDesc[7];
    BattleAbilitySlot *summonTable;

    Menu_FlushAndSetScissor1x(g_pMenuMateriaSubRects->frame.x,
                              g_pMenuMateriaSubRects->frame.y + 3,
                              g_pMenuMateriaSubRects->frame.width,
                              g_pMenuMateriaSubRects->frame.height - 6);
    barDesc[0] = 3;
    barDesc[1] = 0x10;
    barDesc[2] = g_MenuMateriaStockScroll;
    barDesc[3] = g_pMenuMateriaSubRects->frame.x + 0xc7;
    barDesc[4] = g_pMenuMateriaSubRects->frame.y + 3;
    barDesc[5] = 9;
    barDesc[6] = 0x36;
    Menu_DrawScrollBar1x(barDesc, 0.1f);
    summonTable = g_MenuPartyStats[g_MenuMateriaPartyIndex].summons;
    visibleRows = (g_MenuMateriaDetailFlag != 0) + 3;
    firstEnt = g_MenuMateriaStockScroll;
    for (rowIdx = 0; rowIdx < visibleRows; rowIdx++) {
        if (summonTable[rowIdx + firstEnt].attackId != 0xff) {
            Menu_DrawEncodedString1x(
                g_pMenuMateriaSubRects->frame.x + 0x2f,
                g_pMenuMateriaSubRects->frame.y + 7 + rowIdx * 0x11 +
                    g_MenuMateriaStockMax * 5,
                (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_SUMMON, summonTable[rowIdx + firstEnt].attackId, 8),
                7, 0.1f);
        }
    }
    Menu_FlushAndResetScissor1x();
    rectX = g_pMenuMateriaSubRects->frame.x + g_pMenuMateriaSubRects->frame.width;
    rowIdx = g_pMenuMateriaSubRects->frame.y;
    Menu_DrawMateriaActionCostPanel1x(rectX, rowIdx, g_MenuMateriaStockCount + g_MenuMateriaStockSelected + g_MenuMateriaStockScroll,
                                      g_MenuMateriaPartyIndex, BATTLE_ABILITY_LIST_SUMMON);
    Menu_DrawWindowFrame1x(&g_pMenuMateriaSubRects->frame, 0.1f);
    Menu_SetRectShorts(costRectangle, rectX, rowIdx, g_pMenuMateriaSubRects->costWidth,
                  g_pMenuMateriaSubRects->costHeight);
    Menu_DrawWindowFrame1x((EnglishWindowFrame *)costRectangle, 0.1f);
}

/*
 * Draws enemy skill materia list in 2x mode.
 */
// FUNCTION: FF7 0x00709a3b
void __cdecl Menu_DrawMateriaScreenEnemySkillList2x(unsigned char *attacks)
{
    int firstEnt;
    short costRectangle[4];
    int visibleRows;
    int rowIdx;
    int column;
    BattleAbilitySlot *pSkillTable;
    short scrollBar[7];

    Menu_FlushAndSetScissor2x(g_pMenuMateriaSubRects->frame.x,
                              g_pMenuMateriaSubRects->frame.y + 6,
                              g_pMenuMateriaSubRects->frame.width,
                              g_pMenuMateriaSubRects->frame.height - 0xc);
    scrollBar[0] = 3;
    scrollBar[1] = 0xc;
    scrollBar[2] = g_MenuMateriaNextLevelAp;
    scrollBar[3] = g_pMenuMateriaSubRects->frame.x + 0x18d;
    scrollBar[4] = g_pMenuMateriaSubRects->frame.y + 6;
    scrollBar[5] = 0x11;
    scrollBar[6] = 0x6c;
    Menu_DrawScrollBar2x(scrollBar, 0.1f);
    pSkillTable = g_MenuPartyStats[g_MenuMateriaPartyIndex].enemySkills;
    visibleRows = (g_MenuMateriaEquipEffect != 0) + 3;
    firstEnt = g_MenuMateriaNextLevelAp << 1;
    for (rowIdx = 0; rowIdx < visibleRows; rowIdx++) {
        for (column = 0; column < 2; column++) {
            if (pSkillTable[column + rowIdx * 2 + firstEnt].attackId != 0xff) {
                Menu_DrawEncodedString2x(
                    g_pMenuMateriaSubRects->frame.x + 0x28 + column * 0xc8,
                    g_pMenuMateriaSubRects->frame.y + 0xd + rowIdx * 0x22 +
                        g_MenuMateriaCurrentAp * 9,
                    (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ENEMY_SKILL, pSkillTable[column + rowIdx * 2 + firstEnt].attackId, 8),
                    7, 0.1f);
            }
        }
    }
    Menu_FlushAndResetScissor2x();
    column = g_pMenuMateriaSubRects->frame.x + g_pMenuMateriaSubRects->frame.width;
    rowIdx = g_pMenuMateriaSubRects->frame.y;
    Menu_DrawMateriaActionCostPanel2x(column, rowIdx,
                                      g_MenuMateriaDetailAp + g_MenuMateriaDetailLevel * 2 + g_MenuMateriaNextLevelAp * 2,
                                      g_MenuMateriaPartyIndex, BATTLE_ABILITY_LIST_ENEMY_SKILL);
    Menu_DrawWindowFrame2x(&g_pMenuMateriaSubRects->frame, 0.1f);
    Menu_SetRectShorts(costRectangle, column, rowIdx, g_pMenuMateriaSubRects->costWidth,
                  g_pMenuMateriaSubRects->costHeight);
    Menu_DrawWindowFrame2x((EnglishWindowFrame *)costRectangle, 0.1f);
}

/*
 * Draws enemy skill materia list in 1x mode.
 */
// FUNCTION: FF7 0x00709c7a
void __cdecl Menu_DrawMateriaScreenEnemySkillList1x(unsigned char *attacks)
{
    int firstEnt;
    short costRectangle[4];
    int visibleRows;
    int rowIdx;
    int column;
    BattleAbilitySlot *pSkillTable;
    short scrollBar[7];

    Menu_FlushAndSetScissor1x(g_pMenuMateriaSubRects->frame.x,
                              g_pMenuMateriaSubRects->frame.y + 3,
                              g_pMenuMateriaSubRects->frame.width,
                              g_pMenuMateriaSubRects->frame.height - 6);
    scrollBar[0] = 3;
    scrollBar[1] = 0xc;
    scrollBar[2] = g_MenuMateriaNextLevelAp;
    scrollBar[3] = g_pMenuMateriaSubRects->frame.x + 0xc7;
    scrollBar[4] = g_pMenuMateriaSubRects->frame.y + 3;
    scrollBar[5] = 9;
    scrollBar[6] = 0x36;
    Menu_DrawScrollBar1x(scrollBar, 0.1f);
    pSkillTable = g_MenuPartyStats[g_MenuMateriaPartyIndex].enemySkills;
    visibleRows = (g_MenuMateriaEquipEffect != 0) + 3;
    firstEnt = g_MenuMateriaNextLevelAp << 1;
    for (rowIdx = 0; rowIdx < visibleRows; rowIdx++) {
        for (column = 0; column < 2; column++) {
            if (pSkillTable[column + rowIdx * 2 + firstEnt].attackId != 0xff) {
                Menu_DrawEncodedString1x(
                    g_pMenuMateriaSubRects->frame.x + 0x14 + column * 0x64,
                    g_pMenuMateriaSubRects->frame.y + 7 + rowIdx * 0x11 +
                        g_MenuMateriaCurrentAp * 5,
                    (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ENEMY_SKILL, pSkillTable[column + rowIdx * 2 + firstEnt].attackId, 8),
                    7, 0.1f);
            }
        }
    }
    Menu_FlushAndResetScissor1x();
    column = g_pMenuMateriaSubRects->frame.x + g_pMenuMateriaSubRects->frame.width;
    rowIdx = g_pMenuMateriaSubRects->frame.y;
    Menu_DrawMateriaActionCostPanel1x(column, rowIdx,
                                      g_MenuMateriaDetailAp + g_MenuMateriaDetailLevel * 2 + g_MenuMateriaNextLevelAp * 2,
                                      g_MenuMateriaPartyIndex, BATTLE_ABILITY_LIST_ENEMY_SKILL);
    Menu_DrawWindowFrame1x(&g_pMenuMateriaSubRects->frame, 0.1f);
    Menu_SetRectShorts(costRectangle, column, rowIdx, g_pMenuMateriaSubRects->costWidth,
                  g_pMenuMateriaSubRects->costHeight);
    Menu_DrawWindowFrame1x((EnglishWindowFrame *)costRectangle, 0.1f);
}

/*
 * Initializes materia menu screen state, cursors, and layout.
 */
// FUNCTION: FF7 0x00709eb6
void __cdecl Menu_MateriaScreenInit(int arg)
{
    int i;

    g_MenuMateriaState = MATERIA_MENU_SELECT_SLOT;
    g_MenuMateriaPartyIndex = g_MenuPartyMemberIndex;
    MenuCursor_Init((MenuCursor *)&g_MenuMateriaCursorBlock, 0, 0, 1, 2, 0, 0, 1, 2, 0, 0, 0, 1, 0, 0);
    MenuCursor_Init((MenuCursor *)&g_MenuMateriaActiveSlot, 0, 0, 8, 2, 0, 0, 8, 2, 0, 0, 0, 0, 0, 0);
    MenuCursor_Init((MenuCursor *)&g_MenuMateriaPartySlot, 0, 0, 1, 0xa, 0, 0, 1, 0xc8, 0, 0, 0, 0,
                    0, 1);
    if (g_MenuIsSmallViewport320x240 != 1) {
        g_MenuMateriaWindowPtr = (MenuMateriaWindow *)&g_MenuMateriaWindowRects2x;
        g_pMenuMateriaSubRects = (MenuMateriaSubRects *)&g_MenuMateriaSubRects2x;
        g_MenuMateriaTextPtr = g_MenuMateriaCursorCoords;
    } else {
        g_MenuMateriaWindowPtr = (MenuMateriaWindow *)&g_MenuMateriaWindowRects1x;
        g_pMenuMateriaSubRects = (MenuMateriaSubRects *)&g_MenuMateriaSubRects1x;
        g_MenuMateriaTextPtr = g_MenuMagicCursorCoords;
    }
    for (i = 0; i < 3; i++) {
        Savemap_BuildPartyStatsFromRecord(i);
        Battle_LoadCombatantFromSavemap((unsigned char)i);
    }
    Party_RecomputeBattleModifiers();
    g_MenuField = 1;
}

/*
 * Draws selection cursors and highlights on materia screen in 2x mode.
 */
// FUNCTION: FF7 0x00709fd8
void __cdecl Menu_DrawMateriaScreenCursors2x(unsigned int flags)
{
    struct MenuPartyStats *memberStats;
    int labelIndex;
    int cursorX;
    int textId;

    memberStats = &g_MenuPartyStats[g_MenuMateriaPartyIndex];
    switch (g_MenuMateriaState) {
    case MATERIA_MENU_SELECT_CHECK_ARRANGE:
        Menu_DrawMoreTextArrow2x(0xF0, g_MenuMateriaCursorBlock.row * 0x33 + 0x3E, 0.101f);
        break;
    case MATERIA_MENU_SELECT_SLOT:
        Menu_DrawMoreTextArrow2x(g_MenuMateriaActiveSlot * 0x1C + 0x13D, g_MenuMateriaEquipSlot * 0x33 + 0x3E, 0.101f);
        break;
    case MATERIA_MENU_SELECT_STOCK_MATERIA:
        if ((flags & 2) != 0) {
            Menu_DrawMoreTextArrow2x(g_MenuMateriaActiveSlot * 0x1C + 0x13D, g_MenuMateriaEquipSlot * 0x33 + 0x3E, 0.101f);
        }
        Menu_DrawMoreTextArrow2x(0x16A, g_MenuMateriaCharIndex * 0x1A + 0xD6, 0.101f);
        break;
    case MATERIA_MENU_CHECK_COMMANDS:
        if ((flags & 2) != 0) {
            Menu_DrawMoreTextArrow2x(0xF0, g_MenuMateriaCursorBlock.row * 0x33 + 0x3E, 0);
        }
        cursorX = g_pMenuMateriaSubRects->listX + g_MenuMateriaCommandRow * 0x49 + -0x21;
        if (g_MenuMateriaCommandRow != 0) {
            cursorX += 0xD;
        }
        Menu_DrawMoreTextArrow2x(cursorX, g_pMenuMateriaSubRects->listY + g_MenuMateriaCommandCol * 0x1A + 0x11, 0);
        textId = memberStats->commands[g_MenuMateriaCommandCol + g_MenuMateriaCommandRow * 4].commandId;
        if (textId != 0xFF) {
            Menu_DrawEncodedString2x(0x1B, g_MenuMateriaWindowPtr->frames[1].y + 0xD, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, textId, 0), 7, 0.101f);
        }
        break;
    case MATERIA_MENU_CHECK_MAGIC:
        if ((flags & 2) != 0) {
            Menu_DrawMoreTextArrow2x(0xF0, g_MenuMateriaCursorBlock.row * 0x33 + 0x3E, 0.1f);
        }
        Menu_DrawMoreTextArrow2x(g_pMenuMateriaSubRects->frame.x + g_MenuMateriaMagicCount * 0x82 + -5, g_pMenuMateriaSubRects->frame.y + g_MenuMateriaMagicSelected * 0x22 + 0x11, 0.1f);
        textId = memberStats->abilities[g_MenuMateriaMagicCount + g_MenuMateriaMagicSelected * 3 + *(int *)&g_MenuMateriaMagicBuffer[0] * 3].attackId;
        if (textId != 0xFF) {
            Menu_DrawEncodedString2x(0x1B, g_MenuMateriaWindowPtr->frames[1].y + 0xD, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_MAGIC, textId, 0), 7, 0.101f);
        }
        break;
    case MATERIA_MENU_CHECK_SUMMON:
        if ((flags & 2) != 0) {
            Menu_DrawMoreTextArrow2x(0xF0, g_MenuMateriaCursorBlock.row * 0x33 + 0x3E, 0.1f);
        }
        Menu_DrawMoreTextArrow2x(g_pMenuMateriaSubRects->frame.x + 0x32, g_pMenuMateriaSubRects->frame.y + g_MenuMateriaStockSelected * 0x22 + 0x11, 0.1f);
        textId = memberStats->abilities[0x38 + g_MenuMateriaStockSelected + g_MenuMateriaStockScroll].attackId;
        if (textId != 0xFF) {
            Menu_DrawEncodedString2x(0x1B, g_MenuMateriaWindowPtr->frames[1].y + 0xD, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_SUMMON, textId, 0), 7, 0.101f);
        }
        break;
    case MATERIA_MENU_CHECK_ENEMY_SKILL:
        if ((flags & 2) != 0) {
            Menu_DrawMoreTextArrow2x(0xF0, g_MenuMateriaCursorBlock.row * 0x33 + 0x3E, 0.1f);
        }
        Menu_DrawMoreTextArrow2x(g_pMenuMateriaSubRects->frame.x + g_MenuMateriaDetailAp * 0xC8, g_pMenuMateriaSubRects->frame.y + g_MenuMateriaDetailLevel * 0x22 + 0x11, 0.1f);
        textId = memberStats->abilities[0x48 + g_MenuMateriaDetailAp + g_MenuMateriaDetailLevel * 2 + g_MenuMateriaNextLevelAp * 2].attackId;
        if (textId != 0xFF) {
            Menu_DrawEncodedString2x(0x1B, g_MenuMateriaWindowPtr->frames[1].y + 0xD, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ENEMY_SKILL, textId, 0), 7, 0.101f);
        }
        break;
    case MATERIA_MENU_TRASH_SELECT:
        Menu_DrawMoreTextArrow2x(0x16A, g_MenuMateriaStatModIndex * 0x1A + 0xD6, 0.001f);
    case MATERIA_MENU_ARRANGE_MENU:
        if (g_MenuMateriaState == MATERIA_MENU_TRASH_SELECT) {
            if ((flags & 2) != 0) {
                Menu_DrawMoreTextArrow2x(g_MenuMateriaWindowPtr->frames[5].x - 0x1E, g_MenuMateriaWindowPtr->frames[5].y + g_MenuMateriaAbilitySelected * 0x1A + 0xB, 0.001f);
            }
        } else {
            Menu_DrawMoreTextArrow2x(g_MenuMateriaWindowPtr->frames[5].x - 0x1E, g_MenuMateriaWindowPtr->frames[5].y + g_MenuMateriaAbilitySelected * 0x1A + 0xB, 0.001f);
        }
        for (labelIndex = 0; labelIndex < 4; labelIndex++) {
            Menu_DrawEncodedString2x(g_MenuMateriaWindowPtr->frames[5].x + 0xD, g_MenuMateriaWindowPtr->frames[5].y + labelIndex * 0x1A + 9, &g_MenuMateriaLabelTable[(labelIndex + 0xA) * 0x14], 7, 0.01f);
        }
        Menu_DrawWindowFrame2x(&g_MenuMateriaWindowPtr->frames[5], 0.01f);
        break;
    case MATERIA_MENU_TRASH_CONFIRM:
        if ((flags & 2) != 0) {
            Menu_DrawMoreTextArrow2x(0x15B, g_MenuMateriaStatModIndex * 0x1A + 0xD6, 0);
        }
        Menu_DrawMoreTextArrow2x(g_MenuMateriaWindowPtr->frames[6].x + 0x35, g_MenuMateriaWindowPtr->frames[6].y + g_MenuMateriaStatDelta2 * 0x1A + 0x33, 0);
        Menu_DrawEncodedString2x(g_MenuMateriaWindowPtr->frames[6].x + 0xD, g_MenuMateriaWindowPtr->frames[6].y + 0xD, g_MenuMateriaHelpPrompt, 7, 0);
        Menu_DrawEncodedString2x(g_MenuMateriaWindowPtr->frames[6].x + 0x6B, g_MenuMateriaWindowPtr->frames[6].y + 0x2D, g_MenuMateriaHelpPrompt + 0x24, 7, 0);
        Menu_DrawEncodedString2x(g_MenuMateriaWindowPtr->frames[6].x + 0x6B, g_MenuMateriaWindowPtr->frames[6].y + 0x49, g_MenuMateriaHelpPrompt + 0x48, 7, 0);
        Menu_DrawWindowFrame2x(&g_MenuMateriaWindowPtr->frames[6], 0);
    }
}

/*
 * Draws selection cursors and highlights on materia screen in 1x mode.
 */
// FUNCTION: FF7 0x0070a60a
void __cdecl Menu_DrawMateriaScreenCursors1x(unsigned int flags)
{
    struct MenuPartyStats *memberStats;
    int labelIndex;
    int cursorX;
    int textId;

    memberStats = &g_MenuPartyStats[g_MenuMateriaPartyIndex];
    switch (g_MenuMateriaState) {
    case MATERIA_MENU_SELECT_CHECK_ARRANGE:
        Menu_DrawMoreTextArrow1x(0x78, g_MenuMateriaCursorBlock.row * 0x1A + 0x1F, 0.101f);
        break;
    case MATERIA_MENU_SELECT_SLOT:
        Menu_DrawMoreTextArrow1x(g_MenuMateriaActiveSlot * 0xE + 0x9D, g_MenuMateriaEquipSlot * 0x1A + 0x1F, 0.101f);
        break;
    case MATERIA_MENU_SELECT_STOCK_MATERIA:
        if ((flags & 2) != 0) {
            Menu_DrawMoreTextArrow1x(g_MenuMateriaActiveSlot * 0xE + 0x9D, g_MenuMateriaEquipSlot * 0x1A + 0x1F, 0.101f);
        }
        Menu_DrawMoreTextArrow1x(0xB5, g_MenuMateriaCharIndex * 0xD + 0x6B, 0.101f);
        break;
    case MATERIA_MENU_CHECK_COMMANDS:
        if ((flags & 2) != 0) {
            Menu_DrawMoreTextArrow1x(0x78, g_MenuMateriaCursorBlock.row * 0x1A + 0x1F, 0);
        }
        cursorX = g_pMenuMateriaSubRects->listX + g_MenuMateriaCommandRow * 0x25 + -0x11;
        if (g_MenuMateriaCommandRow != 0) {
            cursorX += 7;
        }
        Menu_DrawMoreTextArrow1x(cursorX, g_pMenuMateriaSubRects->listY + g_MenuMateriaCommandCol * 0xD + 9, 0.01f);
        textId = memberStats->commands[g_MenuMateriaCommandCol + g_MenuMateriaCommandRow * 4].commandId;
        if (textId != 0xFF) {
            Menu_DrawEncodedString1x(0xE, g_MenuMateriaWindowPtr->frames[1].y + 7, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, textId, 0), 7, 0.101f);
        }
        break;
    case MATERIA_MENU_CHECK_MAGIC:
        if ((flags & 2) != 0) {
            Menu_DrawMoreTextArrow1x(0x78, g_MenuMateriaCursorBlock.row * 0x1A + 0x1F, 0.1f);
        }
        Menu_DrawMoreTextArrow1x(g_pMenuMateriaSubRects->frame.x + g_MenuMateriaMagicCount * 0x41 + -3, g_pMenuMateriaSubRects->frame.y + g_MenuMateriaMagicSelected * 0x11 + 9, 0.1f);
        textId = memberStats->abilities[g_MenuMateriaMagicCount + g_MenuMateriaMagicSelected * 3 + *(int *)&g_MenuMateriaMagicBuffer[0] * 3].attackId;
        if (textId != 0xFF) {
            Menu_DrawEncodedString1x(0xE, g_MenuMateriaWindowPtr->frames[1].y + 7, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_MAGIC, textId, 0), 7, 0.101f);
        }
        break;
    case MATERIA_MENU_CHECK_SUMMON:
        if ((flags & 2) != 0) {
            Menu_DrawMoreTextArrow1x(0x78, g_MenuMateriaCursorBlock.row * 0x1A + 0x1F, 0.1f);
        }
        Menu_DrawMoreTextArrow1x(g_pMenuMateriaSubRects->frame.x + 0x19, g_pMenuMateriaSubRects->frame.y + g_MenuMateriaStockSelected * 0x11 + 9, 0.1f);
        textId = memberStats->abilities[0x38 + g_MenuMateriaStockSelected + g_MenuMateriaStockScroll].attackId;
        if (textId != 0xFF) {
            Menu_DrawEncodedString1x(0xE, g_MenuMateriaWindowPtr->frames[1].y + 7, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_SUMMON, textId, 0), 7, 0.101f);
        }
        break;
    case MATERIA_MENU_CHECK_ENEMY_SKILL:
        if ((flags & 2) != 0) {
            Menu_DrawMoreTextArrow1x(0x78, g_MenuMateriaCursorBlock.row * 0x1A + 0x1F, 0.1f);
        }
        Menu_DrawMoreTextArrow1x(g_pMenuMateriaSubRects->frame.x + g_MenuMateriaDetailAp * 0x64, g_pMenuMateriaSubRects->frame.y + g_MenuMateriaDetailLevel * 0x11 + 9, 0.1f);
        textId = memberStats->abilities[0x48 + g_MenuMateriaDetailAp + g_MenuMateriaDetailLevel * 2 + g_MenuMateriaNextLevelAp * 2].attackId;
        if (textId != 0xFF) {
            Menu_DrawEncodedString1x(0xE, g_MenuMateriaWindowPtr->frames[1].y + 7, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ENEMY_SKILL, textId, 0), 7, 0.101f);
        }
        break;
    case MATERIA_MENU_TRASH_SELECT:
        Menu_DrawMoreTextArrow1x(0xB5, g_MenuMateriaStatModIndex * 0xD + 0x6B, 0.001f);
    case MATERIA_MENU_ARRANGE_MENU:
        if (g_MenuMateriaState == MATERIA_MENU_TRASH_SELECT) {
            if ((flags & 2) != 0) {
                Menu_DrawMoreTextArrow1x(g_MenuMateriaWindowPtr->frames[5].x - 0xF, g_MenuMateriaWindowPtr->frames[5].y + g_MenuMateriaAbilitySelected * 0xD + 6, 0.001f);
            }
        } else {
            Menu_DrawMoreTextArrow1x(g_MenuMateriaWindowPtr->frames[5].x - 0xF, g_MenuMateriaWindowPtr->frames[5].y + g_MenuMateriaAbilitySelected * 0xD + 6, 0.001f);
        }
        for (labelIndex = 0; labelIndex < 4; labelIndex++) {
            Menu_DrawEncodedString1x(g_MenuMateriaWindowPtr->frames[5].x + 7, g_MenuMateriaWindowPtr->frames[5].y + labelIndex * 0xD + 5, &g_MenuMateriaLabelTable[(labelIndex + 0xA) * 0x14], 7, 0.01f);
        }
        Menu_DrawWindowFrame1x(&g_MenuMateriaWindowPtr->frames[5], 0.01f);
        break;
    case MATERIA_MENU_TRASH_CONFIRM:
        if ((flags & 2) != 0) {
            Menu_DrawMoreTextArrow1x(0xAE, g_MenuMateriaStatModIndex * 0xD + 0x6B, 0);
        }
        Menu_DrawMoreTextArrow1x(g_MenuMateriaWindowPtr->frames[6].x + 0x1B, g_MenuMateriaWindowPtr->frames[6].y + g_MenuMateriaStatDelta2 * 0xD + 0x1A, 0);
        Menu_DrawEncodedString1x(g_MenuMateriaWindowPtr->frames[6].x + 7, g_MenuMateriaWindowPtr->frames[6].y + 7, g_MenuMateriaHelpPrompt, 7, 0);
        Menu_DrawEncodedString1x(g_MenuMateriaWindowPtr->frames[6].x + 0x36, g_MenuMateriaWindowPtr->frames[6].y + 0x17, g_MenuMateriaHelpPrompt + 0x24, 7, 0);
        Menu_DrawEncodedString1x(g_MenuMateriaWindowPtr->frames[6].x + 0x36, g_MenuMateriaWindowPtr->frames[6].y + 0x25, g_MenuMateriaHelpPrompt + 0x48, 7, 0);
        Menu_DrawWindowFrame1x(&g_MenuMateriaWindowPtr->frames[6], 0);
    }
}

/*
 * Draws the name and AP summary of selected materia in 2x mode.
 */
// FUNCTION: FF7 0x0070ac24
void __cdecl Menu_DrawMateriaScreenSelectedMateriaName2x(void)
{
    g_MenuMateriaTargetSlot = -1;
    switch (g_MenuMateriaState) {
    case MATERIA_MENU_SELECT_SLOT:
        switch (g_MenuMateriaEquipSlot) {
        case EQUIP_WEAPON:
            if (g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].weaponMateria[g_MenuMateriaActiveSlot] != -1) {
                Menu_DrawEncodedString2x(
                    0x10, g_MenuMateriaWindowPtr->frames[1].y + 0xd,
                    Menu_LookupEntryNameByMode(
                        MENU_MATERIA_NAME_MATERIA, g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].weaponMateria[g_MenuMateriaActiveSlot] &
                               0xff),
                    7, 0.2f);
            }
            g_MenuMateriaTargetSlot =
                g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].weaponMateria[g_MenuMateriaActiveSlot] &
                0xff;
            break;
        case EQUIP_ARMOR:
            if (g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].armorMateria[g_MenuMateriaActiveSlot] != -1) {
                Menu_DrawEncodedString2x(
                    0x10, g_MenuMateriaWindowPtr->frames[1].y + 0xd,
                    Menu_LookupEntryNameByMode(
                        MENU_MATERIA_NAME_MATERIA, g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].armorMateria[g_MenuMateriaActiveSlot] &
                               0xff),
                    7, 0.2f);
            }
            g_MenuMateriaTargetSlot =
                g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].armorMateria[g_MenuMateriaActiveSlot] &
                0xff;
            break;
        }
        break;
    case MATERIA_MENU_SELECT_STOCK_MATERIA:
        Menu_EmptyTextHook(0x80);
        if (gSavemap.materia[g_MenuMateriaCharIndex + g_MenuMateriaItemIndex] != -1) {
            Menu_DrawEncodedString2x(
                0x10, g_MenuMateriaWindowPtr->frames[1].y + 0xd,
                Menu_LookupEntryNameByMode(
                    MENU_MATERIA_NAME_MATERIA, gSavemap.materia[g_MenuMateriaCharIndex + g_MenuMateriaItemIndex] &
                           0xff),
                7, 0.2f);
        }
        g_MenuMateriaTargetSlot =
            gSavemap.materia[g_MenuMateriaCharIndex + g_MenuMateriaItemIndex] & 0xff;
        break;
    }
}

/*
 * Draws the name and AP summary of selected materia in 1x mode.
 */
// FUNCTION: FF7 0x0070ae2b
void __cdecl Menu_DrawMateriaScreenSelectedMateriaName1x(void)
{
    g_MenuMateriaTargetSlot = -1;
    switch (g_MenuMateriaState) {
    case MATERIA_MENU_SELECT_SLOT:
        switch (g_MenuMateriaEquipSlot) {
        case EQUIP_WEAPON:
            if (g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].weaponMateria[g_MenuMateriaActiveSlot] != -1) {
                Menu_DrawEncodedString1x(
                    8, g_MenuMateriaWindowPtr->frames[1].y + 7,
                    Menu_LookupEntryNameByMode(
                        MENU_MATERIA_NAME_MATERIA, g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].weaponMateria[g_MenuMateriaActiveSlot] &
                               0xff),
                    7, 0.2f);
            }
            g_MenuMateriaTargetSlot =
                g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].weaponMateria[g_MenuMateriaActiveSlot] &
                0xff;
            break;
        case EQUIP_ARMOR:
            if (g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].armorMateria[g_MenuMateriaActiveSlot] != -1) {
                Menu_DrawEncodedString1x(
                    8, g_MenuMateriaWindowPtr->frames[1].y + 7,
                    Menu_LookupEntryNameByMode(
                        MENU_MATERIA_NAME_MATERIA, g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].armorMateria[g_MenuMateriaActiveSlot] &
                               0xff),
                    7, 0.2f);
            }
            g_MenuMateriaTargetSlot =
                g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].armorMateria[g_MenuMateriaActiveSlot] &
                0xff;
            break;
        }
        break;
    case MATERIA_MENU_SELECT_STOCK_MATERIA:
        Menu_EmptyTextHook(0x80);
        if (gSavemap.materia[g_MenuMateriaCharIndex + g_MenuMateriaItemIndex] != -1) {
            Menu_DrawEncodedString1x(
                8, g_MenuMateriaWindowPtr->frames[1].y + 7,
                Menu_LookupEntryNameByMode(
                    MENU_MATERIA_NAME_MATERIA, gSavemap.materia[g_MenuMateriaCharIndex + g_MenuMateriaItemIndex] &
                           0xff),
                7, 0.2f);
        }
        g_MenuMateriaTargetSlot =
            gSavemap.materia[g_MenuMateriaCharIndex + g_MenuMateriaItemIndex] & 0xff;
        break;
    }
}

/*
 * Applies stock materia selection and renders details in 2x mode.
 */
// FUNCTION: FF7 0x0070b032
void __cdecl Menu_ApplyAndDrawStockMateriaDetail2x(int cursor, int param)
{
    g_MateriaDescribeModeFlag = 1;
    Kernel_ApplyMateriaToActor(gSavemap.materia[((MenuCursor *)&g_MenuMateriaCursorBlock)[cursor].row + ((MenuCursor *)&g_MenuMateriaCursorBlock)[cursor].scrollTop]);
    Menu_DrawMateriaDetailPanel2x(gSavemap.materia[((MenuCursor *)&g_MenuMateriaCursorBlock)[cursor].row + ((MenuCursor *)&g_MenuMateriaCursorBlock)[cursor].scrollTop], param);
}

/*
 * Draws full materia detail panel with abilities and AP gauge in 2x mode.
 */
// FUNCTION: FF7 0x0070b092
void __cdecl Menu_DrawMateriaDetailPanel2x(unsigned int materiaWord, int scrollOffset)
{
    struct {
        unsigned int skills;
        unsigned int ap;
        int column;
        int row;
        int line;
        unsigned int id;
        short box[4];
        int colour;
        unsigned char *text;
    } panel;
    /* Block-scoped extern retained from retail declaration scope. */
    extern unsigned char g_MateriaDetailPanelKind;

    panel.id = materiaWord & 0xff;
    if (materiaWord != 0xffffffff) {
        panel.text = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 11, 8);
        panel.column = g_KernelMateriaDataArray[panel.id].elementId;
        if (panel.column < 9)
            Menu_DrawEncodedString2x(40, 244, g_MenuStatusElementNames + panel.column * 10, 6, 0.1f);
        Menu_DrawEncodedString2x(40, 214, Kernel_LookupMateriaNameText(panel.id), 7, 0.101f);
        panel.colour = Menu_GetMateriaTypeColorIndex(materiaWord & 0xff);
        Menu_DrawAtlasQuad2x(13, 212, 128, 32, 16, 16, panel.colour, 0, 0, 0.1f);
        if (panel.id != MATERIA_MASTER_COMMAND && panel.id != MATERIA_MASTER_MAGIC && panel.id != MATERIA_MASTER_SUMMON && panel.id != MATERIA_ENEMY_SKILL) {
            for (panel.row = 0; panel.row < g_MenuMateriaDetailPtr->maxLevel; panel.row++) {
                if (panel.row < g_MenuMateriaDetailPtr->level)
                    Menu_DrawAtlasQuad2x(panel.row * 25 + 207, 212, 144, 32, 16, 16, panel.colour, 0, 0, 0.1f);
                else
                    Menu_DrawAtlasQuad2x(panel.row * 25 + 207, 212, 144, 48, 16, 16, panel.colour, 0, 0, 0.1f);
            }
            Menu_DrawDecimalNumber2x(250, 270, g_MenuMateriaDetailPtr->nextLevelAp, 7, 7, 0.1f);
            panel.ap = (materiaWord >> 8) & 0xffffff;
            if (panel.ap != 0xffffff)
                Menu_DrawDecimalNumber2x(238, 244, panel.ap, 8, 7, 0.1f);
            else
                Menu_DrawEncodedString2x(250, 244, &g_MenuMateriaLabelTable[9 * 0x14], 7, 0.1f);
            Menu_DrawEncodedString2x(108, 270, &g_MenuMateriaLabelTable[8 * 0x14], 5, 0.1f);
            Menu_DrawEncodedString2x(187, 300, &g_MenuMateriaLabelTable[3 * 0x14], 5, 0.1f);
            Menu_DrawEncodedString2x(207, 244, &g_MenuMateriaLabelTable[7 * 0x14], 5, 0.1f);
            Menu_DrawEncodedString2x(8, 300, &g_MenuMateriaLabelTable[2 * 0x14], 5, 0.1f);
            Menu_DrawEncodedString2x(187, 300, &g_MenuMateriaLabelTable[3 * 0x14], 5, 0.1f);
            switch (g_MateriaDetailPanelKind) {
            case MATERIA_DETAIL_PANEL_MAGIC:
                for (panel.line = panel.row = 0; panel.row < 5; panel.row++) {
                    if (g_MenuMateriaDetailPtr->entries[panel.row].id != 0xff) {
                        panel.colour = g_MenuMateriaDetailPtr->entries[panel.row].enabled ? 7 : 0;
                        Menu_DrawEncodedString2x(40, panel.line * 26 + 326,
                                          (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_MAGIC, g_MenuMateriaDetailPtr->entries[panel.row].id, 8),
                                          (unsigned char)panel.colour, 0.101f);
                        panel.line++;
                    }
                }
                break;
            case MATERIA_DETAIL_PANEL_SUMMON:
                Menu_DrawEncodedString2x(33, 326, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_SUMMON, g_MenuMateriaDetailPtr->entries[0].id, 8), 7, 0.101f);
                break;
            case MATERIA_DETAIL_PANEL_COMMAND:
            case MATERIA_DETAIL_PANEL_EXTRA_CUT:
                for (panel.line = panel.row = 0; panel.row < 5; panel.row++) {
                    if (g_MenuMateriaDetailPtr->entries[panel.row].id != 0xff) {
                        panel.colour = g_MenuMateriaDetailPtr->entries[panel.row].enabled ? 7 : 0;
                        Menu_DrawEncodedString2x(40, panel.line * 26 + 326,
                                          (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, g_MenuMateriaDetailPtr->entries[panel.row].id, 8),
                                          (unsigned char)panel.colour, 0.101f);
                        panel.line++;
                    }
                }
                break;
            default:
                switch (panel.id) {
                case MATERIA_MP_PLUS:
                    Menu_DrawEncodedString2x(30, 326, &g_MenuMateriaLabelTable[29 * 0x14], 7, 0.0f);
                    Menu_DrawDecimalNumberZeroPad2x(140, 330, g_MenuMateriaDetailPtr->entries[0].enabled, 2, 6, 0.101f);
                    DrawIconGlyph16_2x(127, 330, 0xb3, 7, 0.101f);
                    DrawIconGlyph16_2x(163, 330, 0xd3, 7, 0.101f);
                    break;
                case MATERIA_HP_PLUS:
                    Menu_DrawEncodedString2x(30, 326, &g_MenuMateriaLabelTable[28 * 0x14], 7, 0.1f);
                    Menu_DrawDecimalNumberZeroPad2x(140, 330, g_MenuMateriaDetailPtr->entries[0].enabled, 2, 6, 0.1f);
                    DrawIconGlyph16_2x(127, 330, 0xb3, 7, 0.101f);
                    DrawIconGlyph16_2x(163, 330, 0xd3, 7, 0.101f);
                    break;
                case MATERIA_SPEED_PLUS:
                    Menu_DrawEncodedString2x(30, 326, &g_MenuMateriaLabelTable[15 * 0x14], 7, 0.1f);
                    Menu_DrawDecimalNumberZeroPad2x(143, 330, g_MenuMateriaDetailPtr->entries[0].enabled, 2, 6, 0.1f);
                    DrawIconGlyph16_2x(130, 330, 0xb3, 7, 0.101f);
                    DrawIconGlyph16_2x(166, 330, 0xd3, 7, 0.101f);
                    break;
                case MATERIA_MAGIC_PLUS:
                    Menu_DrawEncodedString2x(30, 326, &g_MenuMateriaLabelTable[17 * 0x14], 7, 0.0f);
                    Menu_DrawDecimalNumberZeroPad2x(113, 328, g_MenuMateriaDetailPtr->entries[0].enabled, 2, 6, 0.1f);
                    DrawIconGlyph16_2x(100, 330, 0xb3, 7, 0.101f);
                    DrawIconGlyph16_2x(137, 330, 0xd3, 7, 0.101f);
                    break;
                case MATERIA_LUCK_PLUS:
                    Menu_DrawEncodedString2x(18, 326, &g_MenuMateriaLabelTable[18 * 0x14], 7, 0.1f);
                    Menu_DrawDecimalNumberZeroPad2x(100, 330, g_MenuMateriaDetailPtr->entries[0].enabled, 2, 6, 0.1f);
                    DrawIconGlyph16_2x(87, 328, 0xb3, 7, 0.101f);
                    DrawIconGlyph16_2x(123, 328, 0xd3, 7, 0.101f);
                    break;
                case MATERIA_COVER:
                    Menu_DrawEncodedString2x(30, 326, &g_MenuMateriaLabelTable[16 * 0x14], 7, 0.1f);
                    Menu_DrawDecimalNumberZeroPad2x(113, 330, g_MenuMateriaDetailPtr->entries[0].enabled, 3, 6, 0.1f);
                    DrawIconGlyph16_2x(102, 330, 0xb3, 7, 0.101f);
                    DrawIconGlyph16_2x(148, 330, 0xd3, 7, 0.101f);
                    break;
                case MATERIA_EXP_PLUS:
                case MATERIA_GIL_PLUS:
                case MATERIA_ENEMY_AWAY:
                case MATERIA_ENEMY_LURE:
                case MATERIA_CHOCOBO_LURE:
                case MATERIA_PRE_EMPTIVE:
                case MATERIA_LONG_RANGE:
                case MATERIA_MEGA_ALL:
                case MATERIA_COUNTER_ATTACK:
                case MATERIA_SLASH_ALL:
                case MATERIA_DOUBLE_CUT:
                    Menu_DrawEncodedString2x(30, 326, g_MenuMateriaLabelTable + (panel.id + 25) * 20, 7, 0.1f);
                    break;
                default:
                    Menu_DrawEncodedString2x(30, 326, Kernel_LookupMateriaNameText(panel.id), 7, 0.1f);
                    break;
                }
                break;
            }
            Menu_DrawMateriaStatModifierRows2x();
        } else {
            switch (panel.id) {
            case MATERIA_ENEMY_SKILL:
                panel.skills = materiaWord >> 8;
                Menu_FlushAndSetScissor2x(40, 306, 300, 156);
                for (panel.row = 0; panel.row < 7; panel.row++) {
                    for (panel.column = 0; panel.column < 2; panel.column++) {
                        if (panel.skills & (1 << ((panel.column + panel.row * 2 + (scrollOffset / 26) * 2) % 24)))
                            Menu_DrawEncodedString2x((panel.column ? 155 : 0) + 48,
                                              panel.row * 26 + 309 - scrollOffset % 26,
                                              (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ENEMY_SKILL, (panel.column + panel.row * 2 + (scrollOffset / 26) * 2) % 24, 8), 7, 0.1f);
                    }
                }
                Menu_FlushAndResetScissor2x();
                panel.box[0] = 40;
                panel.box[1] = 304;
                panel.box[2] = 300;
                panel.box[3] = 161;
                Menu_DrawEtchedBox2x((EnglishWindowFrame *)panel.box, 0.11f);
                for (panel.row = 0; panel.row < 24; panel.row++) {
                    if (panel.skills & (1 << panel.row))
                        Menu_DrawAtlasQuad2x((panel.row % 12) * 25 + 40, (panel.row / 12) * 29 + 246, 144, 32, 16, 16, panel.colour, 0, 0, 0.1f);
                    else
                        Menu_DrawAtlasQuad2x((panel.row % 12) * 25 + 40, (panel.row / 12) * 29 + 246, 144, 48, 16, 16, panel.colour, 0, 0, 0.1f);
                }
                break;
            }
        }
    }
}

/*
 * Draws character stat modifier delta rows for materia in 2x mode.
 */
// FUNCTION: FF7 0x0070ba7e
void __cdecl Menu_DrawMateriaStatModifierRows2x(void)
{
    int colour;
    int drawn;
    int n;

    for (drawn = n = 0; n < 8; n++) {
        if (g_MenuMateriaDetailPtr->statModifier[n] != 0) {
            if (Menu_MateriaScreenValueIsNegative(g_MenuMateriaDetailPtr->statModifier[n]) != 0) {
                DrawIconGlyph16_2x(0x12c, drawn * 0x18 + 0x14a, 0xe1, 7, 0.1f);
                colour = 2;
            } else {
                DrawIconGlyph16_2x(0x12c, drawn * 0x18 + 0x14a, 0xb3, 7, 0.1f);
                colour = 6;
            }
            Menu_DrawDecimalNumberZeroPad2x(
                0x139, drawn * 0x18 + 0x14a,
                Menu_MateriaScreenAbsValue(g_MenuMateriaDetailPtr->statModifier[n]), 2, colour,
                0.1f);
            Menu_DrawEncodedString2x(0xc8, drawn * 0x18 + 0x146,
                              &g_MenuMateriaLabelTable[(n + 0x14) * 0x14], 7, 0.1f);
            if (n > 5) {
                DrawIconGlyph16_2x(0x151, drawn * 0x18 + 0x148, 0xd3, 7, 0.1f);
            }
            drawn++;
        }
    }
}

/*
 * Returns non-zero if stat delta value is negative.
 */
// FUNCTION: FF7 0x0070bbda
int __cdecl Menu_MateriaScreenValueIsNegative(int value)
{
    return value < 0;
}

/*
 * Returns absolute value of integer stat delta.
 */
// FUNCTION: FF7 0x0070bbe8
int __cdecl Menu_MateriaScreenAbsValue(int v)
{
    int result;

    if (v < 0) {
        result = 0 - v;
    } else {
        result = v;
    }
    return result;
}

/*
 * Applies stock materia selection and renders details in 1x mode.
 */
// FUNCTION: FF7 0x0070bc09
void __cdecl Menu_ApplyAndDrawStockMateriaDetail1x(int cursor, int param)
{
    g_MateriaDescribeModeFlag = 1;
    Kernel_ApplyMateriaToActor(gSavemap.materia[((MenuCursor *)&g_MenuMateriaCursorBlock)[cursor].row + ((MenuCursor *)&g_MenuMateriaCursorBlock)[cursor].scrollTop]);
    Menu_DrawMateriaDetailPanel1x(gSavemap.materia[((MenuCursor *)&g_MenuMateriaCursorBlock)[cursor].row + ((MenuCursor *)&g_MenuMateriaCursorBlock)[cursor].scrollTop], param);
}

/*
 * Draws full materia detail panel with abilities and AP gauge in 1x mode.
 */
// FUNCTION: FF7 0x0070bc69
void __cdecl Menu_DrawMateriaDetailPanel1x(unsigned int materiaWord, int scrollOffset)
{
    struct {
        unsigned int skills;
        unsigned int ap;
        int column;
        int row;
        int line;
        unsigned int id;
        short box[4];
        int colour;
        unsigned char *text;
    } v; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    extern unsigned char g_MateriaDetailPanelKind;

    v.id = materiaWord & 0xff;
    if (materiaWord != 0xffffffff) {
        v.text = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 11, 8);
        v.column = g_KernelMateriaDataArray[v.id].elementId;
        if (v.column < 9)
            Menu_DrawEncodedString1x(20, 122, g_MenuStatusElementNames + v.column * 10, 6, 0.1f);
        Menu_DrawEncodedString1x(20, 107, Kernel_LookupMateriaNameText(v.id), 7, 0.101f);
        v.colour = Menu_GetMateriaTypeColorIndex(materiaWord & 0xff);
        Menu_DrawAtlasQuad1x(7, 106, 128, 32, 16, 16, v.colour, 0, 0, 0.1f);
        if (v.id != MATERIA_MASTER_COMMAND && v.id != MATERIA_MASTER_MAGIC && v.id != MATERIA_MASTER_SUMMON && v.id != MATERIA_ENEMY_SKILL) {
            for (v.row = 0; v.row < g_MenuMateriaDetailPtr->maxLevel; v.row++) {
                if (v.row < g_MenuMateriaDetailPtr->level)
                    Menu_DrawAtlasQuad1x(v.row * 13 + 104, 106, 144, 32, 16, 16, v.colour, 0, 0, 0.1f);
                else
                    Menu_DrawAtlasQuad1x(v.row * 13 + 104, 106, 144, 48, 16, 16, v.colour, 0, 0, 0.1f);
            }
            Menu_DrawDecimalNumber1x(125, 135, g_MenuMateriaDetailPtr->nextLevelAp, 7, 7, 0.1f);
            v.ap = (materiaWord >> 8) & 0xffffff;
            if (v.ap != 0xffffff)
                Menu_DrawDecimalNumber1x(119, 122, v.ap, 8, 7, 0.1f);
            else
                Menu_DrawEncodedString1x(125, 122, &g_MenuMateriaLabelTable[9 * 0x14], 7, 0.1f);
            Menu_DrawEncodedString1x(54, 135, &g_MenuMateriaLabelTable[8 * 0x14], 5, 0.1f);
            Menu_DrawEncodedString1x(94, 150, &g_MenuMateriaLabelTable[3 * 0x14], 5, 0.1f);
            Menu_DrawEncodedString1x(104, 122, &g_MenuMateriaLabelTable[7 * 0x14], 5, 0.1f);
            Menu_DrawEncodedString1x(4, 150, &g_MenuMateriaLabelTable[2 * 0x14], 5, 0.1f);
            Menu_DrawEncodedString1x(94, 150, &g_MenuMateriaLabelTable[3 * 0x14], 5, 0.1f);
            switch (g_MateriaDetailPanelKind) {
            case MATERIA_DETAIL_PANEL_MAGIC:
                for (v.line = v.row = 0; v.row < 5; v.row++) {
                    if (g_MenuMateriaDetailPtr->entries[v.row].id != 0xff) {
                        v.colour = g_MenuMateriaDetailPtr->entries[v.row].enabled ? 7 : 0;
                        Menu_DrawEncodedString1x(20, v.line * 13 + 163,
                                             (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_MAGIC, g_MenuMateriaDetailPtr->entries[v.row].id, 8),
                                             (unsigned char)v.colour, 0.101f);
                        v.line++;
                    }
                }
                break;
            case MATERIA_DETAIL_PANEL_SUMMON:
                Menu_DrawEncodedString1x(17, 163, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_SUMMON, g_MenuMateriaDetailPtr->entries[0].id, 8), 7, 0.101f);
                break;
            case MATERIA_DETAIL_PANEL_COMMAND:
            case MATERIA_DETAIL_PANEL_EXTRA_CUT:
                for (v.line = v.row = 0; v.row < 5; v.row++) {
                    if (g_MenuMateriaDetailPtr->entries[v.row].id != 0xff) {
                        v.colour = g_MenuMateriaDetailPtr->entries[v.row].enabled ? 7 : 0;
                        Menu_DrawEncodedString1x(20, v.line * 13 + 163,
                                             (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, g_MenuMateriaDetailPtr->entries[v.row].id, 8),
                                             (unsigned char)v.colour, 0.101f);
                        v.line++;
                    }
                }
                break;
            default:
                switch (v.id) {
                case MATERIA_MP_PLUS:
                    Menu_DrawEncodedString1x(15, 163, &g_MenuMateriaLabelTable[29 * 0x14], 7, 0.0f);
                    Menu_DrawDecimalNumberZeroPad1x(70, 165, g_MenuMateriaDetailPtr->entries[0].enabled, 2, 6, 0.101f);
                    DrawIconGlyph16_1x(64, 165, 0xb3, 7, 0.101f);
                    DrawIconGlyph16_1x(82, 165, 0xd3, 7, 0.101f);
                    break;
                case MATERIA_HP_PLUS:
                    Menu_DrawEncodedString1x(15, 163, &g_MenuMateriaLabelTable[28 * 0x14], 7, 0.1f);
                    Menu_DrawDecimalNumberZeroPad1x(70, 165, g_MenuMateriaDetailPtr->entries[0].enabled, 2, 6, 0.1f);
                    DrawIconGlyph16_1x(64, 165, 0xb3, 7, 0.101f);
                    DrawIconGlyph16_1x(82, 165, 0xd3, 7, 0.101f);
                    break;
                case MATERIA_SPEED_PLUS:
                    Menu_DrawEncodedString1x(15, 163, &g_MenuMateriaLabelTable[15 * 0x14], 7, 0.1f);
                    Menu_DrawDecimalNumberZeroPad1x(72, 165, g_MenuMateriaDetailPtr->entries[0].enabled, 2, 6, 0.1f);
                    DrawIconGlyph16_1x(65, 165, 0xb3, 7, 0.101f);
                    DrawIconGlyph16_1x(83, 165, 0xd3, 7, 0.101f);
                    break;
                case MATERIA_MAGIC_PLUS:
                    Menu_DrawEncodedString1x(15, 163, &g_MenuMateriaLabelTable[17 * 0x14], 7, 0.0f);
                    Menu_DrawDecimalNumberZeroPad1x(57, 164, g_MenuMateriaDetailPtr->entries[0].enabled, 2, 6, 0.1f);
                    DrawIconGlyph16_1x(50, 165, 0xb3, 7, 0.101f);
                    DrawIconGlyph16_1x(69, 165, 0xd3, 7, 0.101f);
                    break;
                case MATERIA_LUCK_PLUS:
                    Menu_DrawEncodedString1x(9, 163, &g_MenuMateriaLabelTable[18 * 0x14], 7, 0.1f);
                    Menu_DrawDecimalNumberZeroPad1x(50, 165, g_MenuMateriaDetailPtr->entries[0].enabled, 2, 6, 0.1f);
                    DrawIconGlyph16_1x(44, 164, 0xb3, 7, 0.101f);
                    DrawIconGlyph16_1x(62, 164, 0xd3, 7, 0.101f);
                    break;
                case MATERIA_COVER:
                    Menu_DrawEncodedString1x(15, 163, &g_MenuMateriaLabelTable[16 * 0x14], 7, 0.1f);
                    Menu_DrawDecimalNumberZeroPad1x(57, 165, g_MenuMateriaDetailPtr->entries[0].enabled, 3, 6, 0.1f);
                    DrawIconGlyph16_1x(51, 165, 0xb3, 7, 0.101f);
                    DrawIconGlyph16_1x(74, 165, 0xd3, 7, 0.101f);
                    break;
                case MATERIA_EXP_PLUS:
                case MATERIA_GIL_PLUS:
                case MATERIA_ENEMY_AWAY:
                case MATERIA_ENEMY_LURE:
                case MATERIA_CHOCOBO_LURE:
                case MATERIA_PRE_EMPTIVE:
                case MATERIA_LONG_RANGE:
                case MATERIA_MEGA_ALL:
                case MATERIA_COUNTER_ATTACK:
                case MATERIA_SLASH_ALL:
                case MATERIA_DOUBLE_CUT:
                    Menu_DrawEncodedString1x(15, 163, g_MenuMateriaLabelTable + (v.id + 25) * 20, 7, 0.1f);
                    break;
                default:
                    Menu_DrawEncodedString1x(15, 163, Kernel_LookupMateriaNameText(v.id), 7, 0.1f);
                    break;
                }
                break;
            }
            Menu_DrawMateriaStatModifierRows1x();
        } else {
            switch (v.id) {
            case MATERIA_ENEMY_SKILL:
                v.skills = materiaWord >> 8;
                Menu_FlushAndSetScissor1x(20, 153, 150, 78);
                for (v.row = 0; v.row < 7; v.row++) {
                    for (v.column = 0; v.column < 2; v.column++) {
                        if (v.skills & (1 << ((v.column + v.row * 2 + (scrollOffset / 13) * 2) % 24)))
                            Menu_DrawEncodedString1x((v.column ? 78 : 0) + 24,
                                                 v.row * 13 + 155 - scrollOffset % 13,
                                                 (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ENEMY_SKILL, (v.column + v.row * 2 + (scrollOffset / 13) * 2) % 24, 8), 7, 0.1f);
                    }
                }
                Menu_FlushAndResetScissor1x();
                v.box[0] = 20;
                v.box[1] = 152;
                v.box[2] = 150;
                v.box[3] = 81;
                Menu_DrawEtchedBox1x((EnglishWindowFrame *)v.box, 0.11f);
                for (v.row = 0; v.row < 24; v.row++) {
                    if (v.skills & (1 << v.row))
                        Menu_DrawAtlasQuad1x((v.row % 12) * 13 + 20, (v.row / 12) * 15 + 123, 144, 32, 16, 16, v.colour, 0, 0, 0.1f);
                    else
                        Menu_DrawAtlasQuad1x((v.row % 12) * 13 + 20, (v.row / 12) * 15 + 123, 144, 48, 16, 16, v.colour, 0, 0, 0.1f);
                }
                break;
            }
        }
    }
}

/*
 * Draws character stat modifier delta rows for materia in 1x mode.
 */
// FUNCTION: FF7 0x0070c601
void __cdecl Menu_DrawMateriaStatModifierRows1x(void)
{
    int colour;
    int drawn;
    int n;

    for (drawn = n = 0; n < 8; n++) {
        if (g_MenuMateriaDetailPtr->statModifier[n] != 0) {
            if (Menu_MateriaScreenValueIsNegative(g_MenuMateriaDetailPtr->statModifier[n]) != 0) {
                DrawIconGlyph16_1x(0x96, drawn * 0xc + 0xa5, 0xe1, 7, 0.1f);
                colour = 2;
            } else {
                DrawIconGlyph16_1x(0x96, drawn * 0xc + 0xa5, 0xb3, 7, 0.1f);
                colour = 6;
            }
            Menu_DrawDecimalNumberZeroPad1x(
                0x9d, drawn * 0xc + 0xa5,
                Menu_MateriaScreenAbsValue(g_MenuMateriaDetailPtr->statModifier[n]), 2, colour,
                0.1f);
            Menu_DrawEncodedString1x(0x64, drawn * 0xc + 0xa3,
                                 &g_MenuMateriaLabelTable[(n + 0x14) * 0x14], 7, 0.1f);
            if (n > 5) {
                DrawIconGlyph16_1x(0xa9, drawn * 0xc + 0xa4, 0xd3, 7, 0.1f);
            }
            drawn++;
        }
    }
}

/*
 * Draws stock materia inventory grid in 2x mode.
 */
// FUNCTION: FF7 0x0070c75a
void __cdecl Menu_DrawMateriaScreenStockList2x(int listIndex)
{
    int row;
    int typeColour;
    int rowCount;

    Menu_FlushAndSetScissor2x(0, 0xC5, 0x280, 0x115);
    rowCount = (((MenuMateriaSubCursor *)&g_MenuMateriaSubCursorBlock)[listIndex].rowBonus != 0) + 0xA;
    for (row = 0; row < rowCount; row++) {
        if (gSavemap.materia[row + ((MenuCursor *)&g_MenuMateriaCursorBlock)[listIndex].scrollTop] != -1) {
            Menu_DrawEncodedString2x(0x1AB, row * 0x1A + ((MenuCursor *)&g_MenuMateriaCursorBlock)[listIndex].scrollAnim * 6 + 0xD2, Kernel_LookupMateriaNameText(gSavemap.materia[row + ((MenuCursor *)&g_MenuMateriaCursorBlock)[listIndex].scrollTop] & 0xFF), 7, 0.2f);
        }
    }
    g_MenuMateriaScrollBar.visibleRows = 0xA;
    g_MenuMateriaScrollBar.totalRows = 0xC8;
    g_MenuMateriaScrollBarPos.scrollTop = *(short *)&((MenuCursor *)&g_MenuMateriaCursorBlock)[listIndex].scrollTop;
    g_MenuMateriaScrollBarPos.x = 0x26A;
    g_MenuMateriaScrollBarSize.y = 0xC5;
    g_MenuMateriaScrollBarSize.width = 0x11;
    g_MenuMateriaScrollBarHeight = 0x115;
    Menu_DrawScrollBar2x(&g_MenuMateriaScrollBar.visibleRows, 0.2f);
    for (row = 0; row < rowCount; row++) {
        typeColour = Menu_GetMateriaTypeColorIndex(gSavemap.materia[row + ((MenuCursor *)&g_MenuMateriaCursorBlock)[listIndex].scrollTop] & 0xFF);
        if (gSavemap.materia[row + ((MenuCursor *)&g_MenuMateriaCursorBlock)[listIndex].scrollTop] != -1) {
            Menu_DrawAtlasQuad2x(0x193, row * 0x1A + ((MenuCursor *)&g_MenuMateriaCursorBlock)[listIndex].scrollAnim * 6 + 0xD2, 0x80, 0x20, 0x10, 0x10, typeColour, 0, 0, 0.2f);
        }
    }
    Menu_FlushAndResetScissor2x();
}

/*
 * Draws stock materia inventory grid in 1x mode.
 */
// FUNCTION: FF7 0x0070c92b
void __cdecl Menu_DrawMateriaScreenStockList1x(int listIndex)
{
    int row;
    int typeColour;
    int rowCount;

    Menu_FlushAndSetScissor1x(0, 0x63, 0x140, 0x8B);
    rowCount = (((MenuMateriaSubCursor *)&g_MenuMateriaSubCursorBlock)[listIndex].rowBonus != 0) + 0xA;
    for (row = 0; row < rowCount; row++) {
        if (gSavemap.materia[row + ((MenuCursor *)&g_MenuMateriaCursorBlock)[listIndex].scrollTop] != -1) {
            Menu_DrawEncodedString1x(0xD6, row * 0xD + ((MenuCursor *)&g_MenuMateriaCursorBlock)[listIndex].scrollAnim * 3 + 0x69, Kernel_LookupMateriaNameText(gSavemap.materia[row + ((MenuCursor *)&g_MenuMateriaCursorBlock)[listIndex].scrollTop] & 0xFF), 7, 0.2f);
        }
    }
    g_MenuMateriaScrollBar.visibleRows = 0xA;
    g_MenuMateriaScrollBar.totalRows = 0xC8;
    g_MenuMateriaScrollBarPos.scrollTop = *(short *)&((MenuCursor *)&g_MenuMateriaCursorBlock)[listIndex].scrollTop;
    g_MenuMateriaScrollBarPos.x = 0x135;
    g_MenuMateriaScrollBarSize.y = 0x63;
    g_MenuMateriaScrollBarSize.width = 9;
    g_MenuMateriaScrollBarHeight = 0x8B;
    Menu_DrawScrollBar1x(&g_MenuMateriaScrollBar.visibleRows, 0.2f);
    for (row = 0; row < rowCount; row++) {
        typeColour = Menu_GetMateriaTypeColorIndex(gSavemap.materia[row + ((MenuCursor *)&g_MenuMateriaCursorBlock)[listIndex].scrollTop] & 0xFF);
        if (gSavemap.materia[row + ((MenuCursor *)&g_MenuMateriaCursorBlock)[listIndex].scrollTop] != -1) {
            Menu_DrawAtlasQuad1x(0xCA, row * 0xD + ((MenuCursor *)&g_MenuMateriaCursorBlock)[listIndex].scrollAnim * 3 + 0x69, 0x80, 0x20, 0x10, 0x10, typeColour, 0, 0, 0.2f);
        }
    }
    Menu_FlushAndResetScissor1x();
}

/*
 * Draws materia description and attribute sub-panels in 2x mode.
 */
// FUNCTION: FF7 0x0070caf0
void __cdecl Menu_DrawMateriaScreenDetailPanels2x(int scrollOffset)
{
    switch (g_MenuMateriaState) {
    case MATERIA_MENU_SELECT_SLOT:
        Menu_MateriaDetailsNoOp();
        g_MateriaDescribeModeFlag = 1;
        switch (g_MenuMateriaEquipSlot) {
        case EQUIP_WEAPON:
            Kernel_ApplyMateriaToActor(
                g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].weaponMateria[g_MenuMateriaActiveSlot]);
            Menu_DrawMateriaDetailPanel2x(
                g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].weaponMateria[g_MenuMateriaActiveSlot],
                scrollOffset);
            break;
        case EQUIP_ARMOR:
            Kernel_ApplyMateriaToActor(
                g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].armorMateria[g_MenuMateriaActiveSlot]);
            Menu_DrawMateriaDetailPanel2x(
                g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].armorMateria[g_MenuMateriaActiveSlot],
                scrollOffset);
            break;
        }
        break;
    case MATERIA_MENU_SELECT_STOCK_MATERIA:
        Menu_ApplyAndDrawStockMateriaDetail2x(MATERIA_MENU_SELECT_STOCK_MATERIA, scrollOffset);
        Menu_DrawMateriaScreenStockList2x(MATERIA_MENU_SELECT_STOCK_MATERIA);
        break;
    case MATERIA_MENU_SELECT_CHECK_ARRANGE:
        if (g_MenuMateriaCursorBlock.row == 0) {
            Menu_DrawMateriaScreenCommandList2x();
        }
    case MATERIA_MENU_ARRANGE_MENU:
    case MATERIA_MENU_TRASH_SELECT:
    case MATERIA_MENU_TRASH_CONFIRM:
        if (g_MenuMateriaCursorBlock.row != 0) {
            if (g_MenuMateriaState != MATERIA_MENU_SELECT_CHECK_ARRANGE && g_MenuMateriaState != MATERIA_MENU_ARRANGE_MENU) {
                Menu_ApplyAndDrawStockMateriaDetail2x(MATERIA_MENU_TRASH_SELECT, scrollOffset);
            }
            Menu_DrawMateriaScreenStockList2x(MATERIA_MENU_TRASH_SELECT);
        }
        break;
    case MATERIA_MENU_CHECK_COMMANDS:
        Menu_DrawMateriaScreenCommandList2x();
        break;
    default:
        Menu_DrawMateriaScreenCommandList2x();
        switch (g_MenuMateriaCommandPtr[g_MenuMateriaCommandCol + g_MenuMateriaCommandRow * 4].cursorAction) {
        case BATTLE_CURSOR_ACTION_MAGIC:
        case BATTLE_CURSOR_ACTION_W_MAGIC:
            Menu_DrawMateriaScreenMagicList2x(&g_BattleAttackTable[0].accuracy);
            break;
        case BATTLE_CURSOR_ACTION_SUMMON:
        case BATTLE_CURSOR_ACTION_W_SUMMON:
            Menu_DrawMateriaScreenSummonList2x(&g_BattleAttackTable[56].accuracy);
            break;
        case BATTLE_CURSOR_ACTION_ENEMY_SKILL:
            Menu_DrawMateriaScreenEnemySkillList2x(&g_BattleAttackTable[72].accuracy);
            break;
        }
        break;
    }
}

/*
 * Draws materia description and attribute sub-panels in 1x mode.
 */
// FUNCTION: FF7 0x0070cd00
void __cdecl Menu_DrawMateriaScreenDetailPanels1x(int scrollOffset)
{
    switch (g_MenuMateriaState) {
    case MATERIA_MENU_SELECT_SLOT:
        g_MateriaDescribeModeFlag = 1;
        switch (g_MenuMateriaEquipSlot) {
        case EQUIP_WEAPON:
            Kernel_ApplyMateriaToActor(
                g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].weaponMateria[g_MenuMateriaActiveSlot]);
            Menu_DrawMateriaDetailPanel1x(
                g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].weaponMateria[g_MenuMateriaActiveSlot],
                scrollOffset);
            break;
        case EQUIP_ARMOR:
            Kernel_ApplyMateriaToActor(
                g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].armorMateria[g_MenuMateriaActiveSlot]);
            Menu_DrawMateriaDetailPanel1x(
                g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].armorMateria[g_MenuMateriaActiveSlot],
                scrollOffset);
            break;
        }
        break;
    case MATERIA_MENU_SELECT_STOCK_MATERIA:
        Menu_ApplyAndDrawStockMateriaDetail1x(MATERIA_MENU_SELECT_STOCK_MATERIA, scrollOffset);
        Menu_DrawMateriaScreenStockList1x(MATERIA_MENU_SELECT_STOCK_MATERIA);
        break;
    case MATERIA_MENU_SELECT_CHECK_ARRANGE:
        if (g_MenuMateriaCursorBlock.row == 0) {
            Menu_DrawMateriaScreenCommandList1x();
        }
    case MATERIA_MENU_ARRANGE_MENU:
    case MATERIA_MENU_TRASH_SELECT:
    case MATERIA_MENU_TRASH_CONFIRM:
        if (g_MenuMateriaCursorBlock.row != 0) {
            if (g_MenuMateriaState != MATERIA_MENU_SELECT_CHECK_ARRANGE && g_MenuMateriaState != MATERIA_MENU_ARRANGE_MENU) {
                Menu_ApplyAndDrawStockMateriaDetail1x(MATERIA_MENU_TRASH_SELECT, scrollOffset);
            }
            Menu_DrawMateriaScreenStockList1x(MATERIA_MENU_TRASH_SELECT);
        }
        break;
    case MATERIA_MENU_CHECK_COMMANDS:
        Menu_DrawMateriaScreenCommandList1x();
        break;
    default:
        Menu_DrawMateriaScreenCommandList1x();
        switch (g_MenuMateriaCommandPtr[g_MenuMateriaCommandCol + g_MenuMateriaCommandRow * 4].cursorAction) {
        case BATTLE_CURSOR_ACTION_MAGIC:
        case BATTLE_CURSOR_ACTION_W_MAGIC:
            Menu_DrawMateriaScreenMagicList1x(&g_BattleAttackTable[0].accuracy);
            break;
        case BATTLE_CURSOR_ACTION_SUMMON:
        case BATTLE_CURSOR_ACTION_W_SUMMON:
            Menu_DrawMateriaScreenSummonList1x(&g_BattleAttackTable[56].accuracy);
            break;
        case BATTLE_CURSOR_ACTION_ENEMY_SKILL:
            Menu_DrawMateriaScreenEnemySkillList1x(&g_BattleAttackTable[72].accuracy);
            break;
        }
        break;
    }
}

/*
 * Updates materia screen logic, cursors, and user input per frame.
 */
// FUNCTION: FF7 0x0070cf0b
void __cdecl Menu_MateriaScreenFrame(unsigned int frameCounter)
{
struct BattleMateriaStatPlusAbility;
    /* Block-scoped extern retained from retail declaration scope. */
    extern struct BattleMateriaStatPlusAbility *g_MateriaCurrentAbilityMask;
    /* Retail call was unprototyped and pushes 32-bit int rather than narrow char/short; local override preserves push width. */
    int __cdecl Savemap_GetPartyWeaponRecord(int);
    /* Unprototyped retail call signature (parameter width differs from callee definition). */
    int __cdecl Savemap_GetPartyArmourRecord(int);
    struct {
        int locked;
        int unused0[2];
        int oldRow;
        int oldColumn;
        unsigned char *slots;
        int i;
        int dy;
        int materia;
        int dx;
        int unused1[4];
    } v; // Stack frame layout: see FRAME UNION in lib/codegen.h.
    /* VC5 consumes a register choice for this empty condition without emitting code. */
    if (0)
        ;
    v.locked = gSavemap.snowUnused & 1;
    g_MenuMateriaDetailPtr = (MenuMateriaDetail *)&g_MenuMateriaBuffer;
    g_MateriaCurrentAbilityMask = (struct BattleMateriaStatPlusAbility *)&g_MenuMateriaBuffer;
    for (v.i = 0; v.i < 128; v.i++)
        g_MenuMateriaBuffer[v.i] = 255;
    g_MenuMateriaCommandPtr = g_MenuPartyStats[g_MenuMateriaPartyIndex].commands;
    g_MenuMateriaMemberIndex = g_CharIdToSaveSlot[gSavemap.party[g_MenuMateriaPartyIndex]];
    g_MenuMateriaActorEquipPtr = &gSavemap.chars[0];
    if (g_MenuIsSmallViewport320x240 != 1) {
        Menu_DrawCommandWindow2x(g_MenuModuleId);
        if (g_MenuMateriaCursorParam0 == 0) {
            Menu_DrawMateriaScreenCursors2x(frameCounter);
            Menu_DrawMateriaScreenSelectedMateriaName2x();
            Menu_DrawMateriaScreenDetailPanels2x(frameCounter);
            Menu_DrawMateriaScreenCharacterPanel2x();
            Menu_EmptyTextHook(10);
            Menu_DrawEncodedString2x(320, 32, Kernel_LookupItemNameText(g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].weapon + 128), 7, 0.2f);
            Menu_DrawEncodedString2x(320, 84, Kernel_LookupItemNameText(g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].armor + 256), 7, 0.2f);
            Menu_DrawEncodedString2x(267, 32, g_MenuMateriaLabelTable, 5, 0.2f);
            Menu_DrawEncodedString2x(267, 84, &g_MenuMateriaLabelTable[1 * 0x14], 5, 0.2f);
            Menu_DrawEncodedString2x(280, 60, &g_MenuMateriaLabelTable[4 * 0x14], 7, 0.2f);
            Menu_DrawEncodedString2x(280, 111, &g_MenuMateriaLabelTable[5 * 0x14], v.locked ? 0 : 7, 0.2f);
            Menu_DrawMateriaSlotRow(357, 59,
                                    g_KernelWeaponDataArray[g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].weapon].materiaSlotTable, g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].weaponMateria);
            Menu_DrawMateriaSlotRow(357, 109,
                                    g_KernelArmorDataArray[g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].armor].materiaSlotBytes, g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].armorMateria);
            Menu_DrawWindowFrame2x(g_MenuMateriaWindowPtr->frames, 0.3f);
            Menu_DrawWindowFrame2x(&g_MenuMateriaWindowPtr->frames[1], 0.4f);
            switch (g_MenuMateriaState) {
            case MATERIA_MENU_SELECT_SLOT:
            case MATERIA_MENU_SELECT_STOCK_MATERIA:
            case MATERIA_MENU_ARRANGE_MENU:
            case MATERIA_MENU_TRASH_SELECT:
            mat_windows_2x:
                Menu_DrawWindowFrame2x(&g_MenuMateriaWindowPtr->frames[2], 0.5f);
                Menu_DrawWindowFrame2x(&g_MenuMateriaWindowPtr->frames[3], 0.6f);
                break;
            default:
                /* goto kept: default jumps back into the slot/stock/arrange/trash-select body laid out before it; no case order does that */
                if (g_MenuMateriaCursorBlock.row != 0)
                    goto mat_windows_2x;
                Menu_DrawWindowFrame2x(&g_MenuMateriaWindowPtr->frames[4], 0.5f);
                break;
            }
        } else
            Menu_DrawMateriaExchangeList2x(frameCounter);
    } else {
        Menu_DrawCommandWindow1x(g_MenuModuleId);
        if (g_MenuMateriaCursorParam0 == 0) {
            Menu_DrawMateriaScreenCursors1x(frameCounter);
            Menu_DrawMateriaScreenSelectedMateriaName1x();
            Menu_DrawMateriaScreenDetailPanels1x(frameCounter);
            Menu_DrawMateriaScreenCharacterPanel1x();
            Menu_EmptyTextHook(10);
            Menu_DrawEncodedString1x(160, 16, Kernel_LookupItemNameText(g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].weapon + 128), 7, 0.2f);
            Menu_DrawEncodedString1x(160, 42, Kernel_LookupItemNameText(g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].armor + 256), 7, 0.2f);
            Menu_DrawEncodedString1x(134, 16, g_MenuMateriaLabelTable, 5, 0.2f);
            Menu_DrawEncodedString1x(134, 42, &g_MenuMateriaLabelTable[1 * 0x14], 5, 0.2f);
            Menu_DrawEncodedString1x(140, 30, &g_MenuMateriaLabelTable[4 * 0x14], 7, 0.2f);
            Menu_DrawEncodedString1x(140, 56, &g_MenuMateriaLabelTable[5 * 0x14], v.locked ? 0 : 7, 0.2f);
            Menu_DrawMateriaSlotRow1x(179, 30,
                                      g_KernelWeaponDataArray[g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].weapon].materiaSlotTable, g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].weaponMateria);
            Menu_DrawMateriaSlotRow1x(179, 55,
                                      g_KernelArmorDataArray[g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].armor].materiaSlotBytes, g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].armorMateria);
            Menu_DrawWindowFrame1x(g_MenuMateriaWindowPtr->frames, 0.3f);
            Menu_DrawWindowFrame1x(&g_MenuMateriaWindowPtr->frames[1], 0.4f);
            switch (g_MenuMateriaState) {
            case MATERIA_MENU_SELECT_SLOT:
            case MATERIA_MENU_SELECT_STOCK_MATERIA:
            case MATERIA_MENU_ARRANGE_MENU:
            case MATERIA_MENU_TRASH_SELECT:
            mat_windows_1x:
                Menu_DrawWindowFrame1x(&g_MenuMateriaWindowPtr->frames[2], 0.5f);
                Menu_DrawWindowFrame1x(&g_MenuMateriaWindowPtr->frames[3], 0.6f);
                break;
            default:
                /* goto kept: default jumps back into the slot/stock/arrange/trash-select body laid out before it; no case order does that */
                if (g_MenuMateriaCursorBlock.row != 0)
                    goto mat_windows_1x;
                Menu_DrawWindowFrame1x(&g_MenuMateriaWindowPtr->frames[4], 0.5f);
                break;
            }
        } else
            Menu_DrawMateriaExchangeList1x(frameCounter);
    }

    if (Menu_GetPhase() == 0 && Menu_IsPopupActive() == 0) {
        switch (g_MenuMateriaState) {
        case MATERIA_MENU_SELECT_CHECK_ARRANGE:
        case MATERIA_MENU_SELECT_SLOT:
        case MATERIA_MENU_ARRANGE_MENU:
            if (BtlMenu_GetInputRepeat(INPUT_L1)) {
                do {
                    g_MenuMateriaPartyIndex--;
                    if (g_MenuMateriaPartyIndex < 0)
                        g_MenuMateriaPartyIndex = 2;
                } while (gSavemap.party[g_MenuMateriaPartyIndex] == PARTY_CHAR_NONE);
            } else if (BtlMenu_GetInputRepeat(INPUT_R1)) {
                do {
                    g_MenuMateriaPartyIndex++;
                    if (g_MenuMateriaPartyIndex > 2)
                        g_MenuMateriaPartyIndex = 0;
                } while (gSavemap.party[g_MenuMateriaPartyIndex] == PARTY_CHAR_NONE);
            }
            break;
        }
        switch (g_MenuMateriaState) {
        case MATERIA_MENU_SELECT_SLOT:
            v.oldColumn = g_MenuMateriaActiveSlot;
            MenuCursor_Update((MenuCursor *)&g_MenuMateriaActiveSlot);
            break;
        case MATERIA_MENU_CHECK_COMMANDS:
            v.oldColumn = g_MenuMateriaCommandRow;
            v.oldRow = g_MenuMateriaCommandCol;
            MenuCursor_Update((MenuCursor *)&g_MenuMateriaCommandRow);
            v.dx = g_MenuMateriaCommandRow - v.oldColumn;
            v.dy = g_MenuMateriaCommandCol - v.oldRow;
            while (1) { /* Constant condition kept for /Od branch/register layout; see lib/codegen.h. */
                if (v.dy == 0 && v.dx == 0 && g_MenuMateriaCommandPtr[g_MenuMateriaCommandCol + g_MenuMateriaCommandRow * 4].menuFlags != 0) {
                    g_MenuMateriaCommandCol = 0;
                    g_MenuMateriaCommandRow = 0;
                }
                if (g_MenuMateriaCommandPtr[g_MenuMateriaCommandCol + g_MenuMateriaCommandRow * 4].menuFlags == 0)
                    break;
                g_MenuMateriaCommandCol += v.dy;
                g_MenuMateriaCommandRow += v.dx;
                g_MenuMateriaCommandCol &= 3;
                g_MenuMateriaCommandRow %= g_MenuPartyStats[g_MenuMateriaPartyIndex].commandColumns;
            }
            break;
        case MATERIA_MENU_EXCHANGE:
            break;
        default:
            MenuCursor_Update((MenuCursor *)&g_MenuMateriaCursorBlock + g_MenuMateriaState);
            break;
        }
        switch (g_MenuMateriaState) {
        case MATERIA_MENU_SELECT_CHECK_ARRANGE:
            if (BtlMenu_GetInputRepeat(INPUT_CROSS) || MenuCursor_ReadPressed(INPUT_RIGHT, MOUSE_DIR_RIGHT)) {
                g_MenuMateriaEquipSlot = g_MenuMateriaCursorBlock.row;
                g_MenuMateriaState = MATERIA_MENU_SELECT_SLOT;
            } else if (Input_GetPressed(INPUT_CIRCLE)) {
                if (g_MenuMateriaCursorBlock.row == 0) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    g_MenuMateriaState = MATERIA_MENU_CHECK_COMMANDS;
                    MenuCursor_Init((MenuCursor *)&g_MenuMateriaCommandRow, 0, 0, g_MenuPartyStats[g_MenuMateriaPartyIndex].commandColumns, 4, 0, 0, g_MenuPartyStats[g_MenuMateriaPartyIndex].commandColumns, 4, 0, 0, 1, 1, 0, 0);
                } else if (v.locked == 0) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    g_MenuMateriaState = MATERIA_MENU_ARRANGE_MENU;
                    Menu_SetFrameClearFlag(1);
                    MenuCursor_Init((MenuCursor *)&g_MenuMateriaAbilityCount, 0, 0, 1, 4, 0, 0, 1, 4, 0, 0, 0, 1, 0, 0);
                    MenuCursor_Init((MenuCursor *)&g_MenuMateriaApBonus, 0, 0, 1, 10, 0, 0, 1, 200, 0, 0, 0, 0, 0, 0);
                } else
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
            }
            break;
        case MATERIA_MENU_SELECT_SLOT:
            if (MenuCursor_ReadPressed(INPUT_LEFT, MOUSE_DIR_LEFT) && v.oldColumn == g_MenuMateriaActiveSlot && v.oldColumn == 0) {
                g_MenuMateriaCursorBlock.row = g_MenuMateriaEquipSlot;
                g_MenuMateriaState = MATERIA_MENU_SELECT_CHECK_ARRANGE;
            } else if (Input_GetPressed(INPUT_CIRCLE)) {
                switch (g_MenuMateriaEquipSlot) {
                case EQUIP_WEAPON:
                    v.slots = (unsigned char *)Savemap_GetPartyWeaponRecord(g_MenuMateriaPartyIndex);
                    break;
                case EQUIP_ARMOR:
                    v.slots = (unsigned char *)Savemap_GetPartyArmourRecord(g_MenuMateriaPartyIndex);
                    break;
                }
                if (v.slots[g_MenuMateriaActiveSlot] != 0 && v.locked == 0) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    g_MenuMateriaState = MATERIA_MENU_SELECT_STOCK_MATERIA;
                } else {
                    if (v.locked && gSavemap.party[g_MenuMateriaPartyIndex] == PARTY_CHAR_SEPHIROTH)
                        Menu_OpenPopupMessage((void *)(g_MenuMateriaHelpPrompt + 0x6c), 7);
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                }
            } else if (Input_GetPressed(INPUT_TRIANGLE)) {
                switch (g_MenuMateriaEquipSlot) {
                case EQUIP_WEAPON:
                    v.slots = (unsigned char *)Savemap_GetPartyWeaponRecord(g_MenuMateriaPartyIndex);
                    if (v.slots[g_MenuMateriaActiveSlot] != 0 && v.locked == 0) {
                        if (g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].weaponMateria[g_MenuMateriaActiveSlot] != -1) {
                            if (Savemap_AddMateria(g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].weaponMateria[g_MenuMateriaActiveSlot]) == -1) {
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_MATERIA_REMOVE);
                                g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].weaponMateria[g_MenuMateriaActiveSlot] = -1;
                                Savemap_BuildPartyStatsFromRecord(g_MenuMateriaPartyIndex);
                                Battle_LoadCombatantFromSavemap(g_MenuMateriaPartyIndex);
                                Party_RecomputeBattleModifiers();
                            } else
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                        } else
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                    } else {
                        if (v.locked && gSavemap.party[g_MenuMateriaPartyIndex] == PARTY_CHAR_SEPHIROTH)
                            Menu_OpenPopupMessage((void *)(g_MenuMateriaHelpPrompt + 0x6c), 7);
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                    }
                    break;
                case EQUIP_ARMOR:
                    v.slots = (unsigned char *)Savemap_GetPartyArmourRecord(g_MenuMateriaPartyIndex);
                    if (v.slots[g_MenuMateriaActiveSlot] != 0 && v.locked == 0) {
                        if (g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].armorMateria[g_MenuMateriaActiveSlot] != -1) {
                            if (Savemap_AddMateria(g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].armorMateria[g_MenuMateriaActiveSlot]) == -1) {
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_MATERIA_REMOVE);
                                g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].armorMateria[g_MenuMateriaActiveSlot] = -1;
                                Savemap_BuildPartyStatsFromRecord(g_MenuMateriaPartyIndex);
                                Battle_LoadCombatantFromSavemap(g_MenuMateriaPartyIndex);
                                Party_RecomputeBattleModifiers();
                            } else
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                        } else
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                    } else {
                        if (v.locked && gSavemap.party[g_MenuMateriaPartyIndex] == PARTY_CHAR_SEPHIROTH)
                            Menu_OpenPopupMessage((void *)(g_MenuMateriaHelpPrompt + 0x6c), 7);
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                    }
                    break;
                }
            } else if (Input_GetPressed(INPUT_CROSS)) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                Menu_SetPhase(5, 0);
                Menu_SetScreen(0);
                Menu_ClearMateriaScreenInitializedFlag();
            } else if (Input_GetPressed(INPUT_SQUARE)) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                Menu_SetPhase(4, 4);
                Menu_SetScreen(4);
                g_MenuPartyMemberIndex = g_MenuMateriaPartyIndex;
                Menu_ClearMateriaScreenInitializedFlag();
            }
            break;
        case MATERIA_MENU_SELECT_STOCK_MATERIA:
            if (g_MenuMateriaCommandCount == 0) {
                if (Input_GetPressed(INPUT_CIRCLE)) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_MATERIA_REMOVE);
                    switch (g_MenuMateriaEquipSlot) {
                    case EQUIP_WEAPON:
                        v.materia = g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].weaponMateria[g_MenuMateriaActiveSlot];
                        g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].weaponMateria[g_MenuMateriaActiveSlot] = gSavemap.materia[g_MenuMateriaItemIndex + g_MenuMateriaCharIndex];
                        gSavemap.materia[g_MenuMateriaItemIndex + g_MenuMateriaCharIndex] = v.materia;
                        g_MenuMateriaState = MATERIA_MENU_SELECT_SLOT;
                        break;
                    case EQUIP_ARMOR:
                        v.materia = g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].armorMateria[g_MenuMateriaActiveSlot];
                        g_MenuMateriaActorEquipPtr[g_MenuMateriaMemberIndex].armorMateria[g_MenuMateriaActiveSlot] = gSavemap.materia[g_MenuMateriaItemIndex + g_MenuMateriaCharIndex];
                        gSavemap.materia[g_MenuMateriaItemIndex + g_MenuMateriaCharIndex] = v.materia;
                        g_MenuMateriaState = MATERIA_MENU_SELECT_SLOT;
                        break;
                    }
                    Savemap_BuildPartyStatsFromRecord(g_MenuMateriaPartyIndex);
                    Battle_LoadCombatantFromSavemap(g_MenuMateriaPartyIndex);
                    Party_RecomputeBattleModifiers();
                } else if (Input_GetPressed(INPUT_CROSS)) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                    g_MenuMateriaState = MATERIA_MENU_SELECT_SLOT;
                }
            }
            break;
        case MATERIA_MENU_CHECK_COMMANDS:
            if (Input_GetPressed(INPUT_CIRCLE)) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                switch (g_MenuMateriaCommandPtr[g_MenuMateriaCommandCol + g_MenuMateriaCommandRow * 4].cursorAction) {
                case BATTLE_CURSOR_ACTION_MAGIC:
                case BATTLE_CURSOR_ACTION_W_MAGIC:
                    Menu_ClearPartySingleTargetAllCounts();
                    g_MenuMateriaState = MATERIA_MENU_CHECK_MAGIC;
                    MenuCursor_Init((MenuCursor *)&g_MenuMateriaMagicCount, 0, 0, 3, 3, 0, 0, 3, 18, 0, 0, 2, 0, 0, 1);
                    break;
                case BATTLE_CURSOR_ACTION_SUMMON:
                case BATTLE_CURSOR_ACTION_W_SUMMON:
                    g_MenuMateriaState = MATERIA_MENU_CHECK_SUMMON;
                    MenuCursor_Init((MenuCursor *)&g_MenuMateriaStockCount, 0, 0, 1, 3, 0, 0, 1, 16, 0, 0, 0, 0, 0, 1);
                    break;
                case BATTLE_CURSOR_ACTION_ENEMY_SKILL:
                    g_MenuMateriaState = MATERIA_MENU_CHECK_ENEMY_SKILL;
                    MenuCursor_Init((MenuCursor *)&g_MenuMateriaDetailAp, 0, 0, 2, 3, 0, 0, 2, 12, 0, 0, 2, 0, 0, 1);
                    break;
                }
            } else if (Input_GetPressed(INPUT_CROSS)) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                g_MenuMateriaState = MATERIA_MENU_SELECT_CHECK_ARRANGE;
            }
            break;
        case MATERIA_MENU_CHECK_MAGIC:
            if (*(int *)((char *)&g_MenuMateriaMagicBuffer + 28) == 0 && Input_GetPressed(INPUT_CROSS)) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                g_MenuMateriaState = MATERIA_MENU_CHECK_COMMANDS;
            }
            break;
        case MATERIA_MENU_CHECK_SUMMON:
            if (g_MenuMateriaDetailFlag == 0 && Input_GetPressed(INPUT_CROSS)) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                g_MenuMateriaState = MATERIA_MENU_CHECK_COMMANDS;
            }
            break;
        case MATERIA_MENU_CHECK_ENEMY_SKILL:
            if (g_MenuMateriaEquipEffect == 0 && Input_GetPressed(INPUT_CROSS)) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                g_MenuMateriaState = MATERIA_MENU_CHECK_COMMANDS;
            }
            break;
        case MATERIA_MENU_ARRANGE_MENU:
            if (Input_GetPressed(INPUT_CIRCLE)) {
                switch (g_MenuMateriaAbilitySelected) {
                case MENU_MATERIA_ARRANGE_SORT:
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    Menu_MateriaSortStock(0);
                    break;
                case MENU_MATERIA_ARRANGE_EXCHANGE:
                    Menu_MateriaExchangeInitPartyList();
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    g_MenuMateriaState = MATERIA_MENU_EXCHANGE;
                    g_MenuMateriaCursorParam0 = 1;
                    break;
                case MENU_MATERIA_ARRANGE_REMOVE_ALL:
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_MATERIA_REMOVE);
                    for (v.i = 0; v.i < 8; v.i++) {
                        v.materia = gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[g_MenuMateriaPartyIndex]]].weaponMateria[v.i];
                        if (v.materia != -1 && v.materia != Savemap_AddMateria(v.materia))
                            gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[g_MenuMateriaPartyIndex]]].weaponMateria[v.i] = -1;
                        v.materia = gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[g_MenuMateriaPartyIndex]]].armorMateria[v.i];
                        if (v.materia != -1 && v.materia != Savemap_AddMateria(v.materia))
                            gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[g_MenuMateriaPartyIndex]]].armorMateria[v.i] = -1;
                    }
                    Savemap_BuildPartyStatsFromRecord(g_MenuMateriaPartyIndex);
                    Battle_LoadCombatantFromSavemap(g_MenuMateriaPartyIndex);
                    Party_RecomputeBattleModifiers();
                    break;
                case MENU_MATERIA_ARRANGE_TRASH:
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    g_MenuMateriaState = MATERIA_MENU_TRASH_SELECT;
                    break;
                }
            } else if (Input_GetPressed(INPUT_CROSS)) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                g_MenuMateriaState = MATERIA_MENU_SELECT_CHECK_ARRANGE;
            }
            break;
        case MATERIA_MENU_TRASH_SELECT:
            if (g_MenuMateriaStatDelta0 == 0) {
                if (Input_GetPressed(INPUT_CIRCLE)) {
                    if (gSavemap.materia[g_MenuMateriaStatModIndex + g_MenuMateriaModifierCount] != -1) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        MenuCursor_Init((MenuCursor *)&g_MenuMateriaStatDelta1, 0, 1, 1, 2, 0, 0, 1, 2, 0, 0, 0, 1, 0, 0);
                        g_MenuMateriaState = MATERIA_MENU_TRASH_CONFIRM;
                    } else
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                } else if (Input_GetPressed(INPUT_CROSS)) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                    g_MenuMateriaState = MATERIA_MENU_ARRANGE_MENU;
                }
            }
            break;
        case MATERIA_MENU_TRASH_CONFIRM:
            if (Input_GetPressed(INPUT_CIRCLE)) {
                if (g_MenuMateriaStatDelta2 == 0) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    gSavemap.materia[g_MenuMateriaStatModIndex + g_MenuMateriaModifierCount] = -1;
                    g_MenuMateriaState = MATERIA_MENU_TRASH_SELECT;
                } else
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                g_MenuMateriaState = MATERIA_MENU_TRASH_SELECT;
            } else if (Input_GetPressed(INPUT_CROSS)) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                g_MenuMateriaState = MATERIA_MENU_TRASH_SELECT;
            }
            break;
        case MATERIA_MENU_EXCHANGE:
            Menu_MateriaExchangeHandleInput();
            break;
        }
    }
}

/*
 * Sorts stock materia inventory according to active sort mode.
 */
// FUNCTION: FF7 0x0070e2aa
void __cdecl Menu_MateriaSortStock(int skip)
{
    int flag;

    flag = skip;
    if (flag != 0) {
        return;
    }
    Menu_MateriaSortIterativeQuicksort((int)gSavemap.materia, 0xC8, (short(__cdecl *)(short, short, int *))Menu_MateriaSortCompareStockEntries,
                                       (void(__cdecl *)(short, short, int *))Menu_MateriaSortSwapStockIndices);
}

/*
 * Quicksort implementation for stock materia entries.
 */
// FUNCTION: FF7 0x0070e2dc
short __cdecl Menu_MateriaSortIterativeQuicksort(int base, unsigned int count, short(__cdecl *cmp)(short, short, int *), void(__cdecl *swap)(short, short, int *))
{
    unsigned int rightStack[64];
    unsigned int stackTop;
    unsigned int lower;
    unsigned int scanRight;
    unsigned int scanLeft;
    unsigned int stackLo[64];

    if (count < 2) {
        return 1;
    }
    stackTop = 1;
    stackLo[0] = 0;
    rightStack[0] = count - 1;
    while (stackTop--) {
        lower = stackLo[stackTop];
        scanLeft = lower + 1;
        scanRight = rightStack[stackTop];
        count = scanRight;
        while (scanLeft < scanRight) {
            while (scanLeft < scanRight && cmp((short)scanLeft, (short)lower, &base) < 1) {
                scanLeft++;
            }
            while (scanLeft <= scanRight && cmp((short)lower, (short)scanRight, &base) < 1) {
                scanRight--;
            }
            if (scanLeft < scanRight) {
                swap((short)scanLeft++, (short)scanRight--, &base);
            }
        }
        if (cmp((short)lower, (short)scanRight, &base) > 0) {
            swap((short)lower, (short)scanRight, &base);
        }
        if (scanRight > lower) {
            scanRight--;
        }
        if (scanRight > lower && count > scanLeft && scanRight - lower < count - scanLeft) {
            Menu_MateriaSortSwapRangeBounds((int *)&scanRight, (int *)&count);
            Menu_MateriaSortSwapRangeBounds((int *)&lower, (int *)&scanLeft);
        }
        if (scanRight > lower) {
            stackLo[stackTop] = lower;
            rightStack[stackTop] = scanRight;
            stackTop++;
        }
        if (count > scanLeft) {
            stackLo[stackTop] = scanLeft;
            rightStack[stackTop] = count;
            stackTop++;
        }
        if (stackTop >= 0x40) {
            return 0;
        }
    }
    return 1;
}

/*
 * Swaps sort boundary bounds during quicksort partitioning.
 */
// FUNCTION: FF7 0x0070e5b8
void __cdecl Menu_MateriaSortSwapRangeBounds(int *first, int *second)
{
    int tmp;

    tmp = *first;
    *first = *second;
    *second = tmp;
}

/*
 * Compares two stock materia entries by sort criteria.
 */
// FUNCTION: FF7 0x0070e5da
void __cdecl Menu_MateriaSortCompareStockEntries(unsigned int indexA, unsigned int indexB, int **list)
{
    unsigned int keyA;
    unsigned int keyB;
    unsigned char colA;
    unsigned char colB;

    if ((*list)[indexA & 0xffff] == -1) {
        colA = 0x80;
        keyA = 0;
    } else {
        colA =
            (unsigned char)Menu_GetMateriaTypeColorIndex((*list)[indexA & 0xffff]);
        keyA = ((unsigned int)(*list)[indexA & 0xffff] >> 8) +
               (((*list)[indexA & 0xffff] & 0xff) << 0x18);
    }
    if ((*list)[indexB & 0xffff] == -1) {
        colB = 0x80;
        keyB = 0;
    } else {
        colB =
            (unsigned char)Menu_GetMateriaTypeColorIndex((*list)[indexB & 0xffff]);
        keyB = ((unsigned int)(*list)[indexB & 0xffff] >> 8) +
               (((*list)[indexB & 0xffff] & 0xff) << 0x18);
    }
    Menu_MateriaSortCompareKeys(colA, colB, keyA, keyB);
}

/*
 * Compares sorting key values for stock materia.
 */
// FUNCTION: FF7 0x0070e6da
int __cdecl Menu_MateriaSortCompareKeys(unsigned char colorA, unsigned char colorB, unsigned int keyA, unsigned int keyB)
{
    if (colorA == colorB) {
        if (keyA == keyB) {
            return 0;
        }
        if (keyA > keyB) {
            return -1;
        }
        return 1;
    }
    if (colorA > colorB) {
        return 1;
    }
    return -1;
}

/*
 * Swaps indexed stock materia table slots.
 */
// FUNCTION: FF7 0x0070e734
void __cdecl Menu_MateriaSortSwapStockIndices(int i, int j, void *table)
{
    Menu_MateriaSortSwapStockEntries((int *)(*(int *)table + (i & 0xFFFF) * 4), (int *)(*(int *)table + (j & 0xFFFF) * 4));
}

/*
 * Swaps two stock materia structure records.
 */
// FUNCTION: FF7 0x0070e764
void __cdecl Menu_MateriaSortSwapStockEntries(int *first, int *second)
{
    int tmp;

    tmp = *first;
    *first = *second;
    *second = tmp;
}

/*
 * Clears secondary spell slot indicators across active party.
 */
// FUNCTION: FF7 0x0070e786
void __cdecl Menu_ClearPartySingleTargetAllCounts(void)
{
    int partyIdx;
    int spellIndex;
    BattleAbilitySlot *base;

    for (partyIdx = 0; partyIdx < 3; partyIdx++) {
        base = g_MenuPartyStats[partyIdx].spells;
        for (spellIndex = 0; spellIndex < 0x38; spellIndex++) {
            if (base[spellIndex].attackId != 0xff && base[spellIndex].allCount != 0 &&
                ((base[spellIndex].targetFlags & 4) == 0 ||
                 (base[spellIndex].targetFlags & 8) == 0)) {
                base[spellIndex].allCount = 0;
            }
        }
    }
}

/*
 * Draws equipment materia slots and links in 2x mode.
 */
// FUNCTION: FF7 0x0070e831
void __cdecl Menu_DrawMateriaSlotRow(int x, int y, unsigned char *slotTypes, int *materiaIds)
{
    short rect[4];
    int tintKind;
    int i;

    for (i = 0; i < 8; i++) {
        if (slotTypes[i] != 0) {
            if ((materiaIds[i] & 0xff) != 0xff) {
                tintKind = Menu_GetMateriaTypeColorIndex(materiaIds[i] & 0xff);
                Menu_DrawAtlasQuad2x(x + i * 0x1c, y, 0x80,
                                     0x20, 0x10, 0x10, tintKind, 1, 0, 0.2f);
            }
            if (slotTypes[i] > 4) {
                Menu_DrawAtlasQuad2x(x + i * 0x1c, y, 0x80,
                                     0x30, 0x10, 0x10, 1, 0, 0, 0.201f);
            } else {
                Menu_DrawAtlasQuad2x(x + i * 0x1c, y, 0x70,
                                     0x20, 0x10, 0x10, 1, 0, 0, 0.201f);
            }
        }
    }
    for (i = 0; i < 8; i++) {
        if (slotTypes[i] != 0 &&
            (slotTypes[i] == 3 ||
             slotTypes[i] == 7)) {
            Menu_DrawAtlasQuad2x(x + i * 0x1c - 0xc, y, 0x70,
                                 0x30, 0x10, 0x10, 1, 0, 0, 0.2001f);
        }
    }
    rect[0] = x - 3;
    rect[1] = y;
    rect[2] = 0xe2;
    rect[3] = 0x18;
    Menu_DrawEtchedBox2x((EnglishWindowFrame *)rect, 0.201f);
}

/*
 * Draws equipment materia slots and links in 1x mode.
 */
// FUNCTION: FF7 0x0070e9dc
void __cdecl Menu_DrawMateriaSlotRow1x(int x, int y, unsigned char *slotTypes, int *materiaIds)
{
    int hueIdx;
    short rect[4];
    int i;

    for (i = 0; i < 8; i++) {
        if (slotTypes[i] != 0) {
            if ((materiaIds[i] & 0xff) != 0xff) {
                hueIdx = Menu_GetMateriaTypeColorIndex(materiaIds[i] & 0xff);
                Menu_DrawAtlasQuad1x(x + i * 0xe, y, 0x80,
                                     0x20, 0x10, 0x10, hueIdx, 1, 0, 0.2f);
            }
            if (slotTypes[i] > 4) {
                Menu_DrawAtlasQuad1x(x + i * 0xe, y, 0x80,
                                     0x30, 0x10, 0x10, 1, 0, 0, 0.201f);
            } else {
                Menu_DrawAtlasQuad1x(x + i * 0xe, y, 0x70,
                                     0x20, 0x10, 0x10, 1, 0, 0, 0.201f);
            }
        }
    }
    for (i = 0; i < 8; i++) {
        if (slotTypes[i] != 0 &&
            (slotTypes[i] == 3 ||
             slotTypes[i] == 7)) {
            Menu_DrawAtlasQuad1x(x + i * 0xe - 7, y, 0x70,
                                 0x30, 0x10, 0x10, 1, 0, 0, 0.2001f);
        }
    }
    rect[0] = x - 2;
    rect[1] = y;
    rect[2] = 0x71;
    rect[3] = 0xc;
    Menu_DrawEtchedBox1x((EnglishWindowFrame *)rect, 0.201f);
}

/*
 * Draws character stats and equipped materia summary in 2x mode.
 */
// FUNCTION: FF7 0x0070eb87
void __cdecl Menu_DrawMateriaScreenCharacterPanel2x(void)
{
    Menu_DrawCharStatBlock2x(0x67, 0x1E, g_MenuMateriaPartyIndex, 0.2f);
    Menu_DrawCharPortrait2x(0x11, 0x13, g_MenuMateriaPartyIndex, 0.11f);
}

/*
 * Draws character stats and equipped materia summary in 1x mode.
 */
// FUNCTION: FF7 0x0070ebbb
void __cdecl Menu_DrawMateriaScreenCharacterPanel1x(void)
{
    Menu_DrawCharStatBlock1x(0x34, 0xF, g_MenuMateriaPartyIndex, 0.2f);
    Menu_DrawCharPortrait1x(9, 0xA, g_MenuMateriaPartyIndex, 0.11f);
}

/*
 * Initializes party member list for materia exchange screen.
 */
// FUNCTION: FF7 0x0070ebef
void __cdecl Menu_MateriaExchangeInitPartyList(void)
{
    int charIndex;
    int visibleCount;

    for (charIndex = 0; charIndex < 9; charIndex++) {
        g_MenuMateriaExchangeSlots[charIndex] = 0xff;
    }
    for (visibleCount = charIndex = 0; charIndex < 9; charIndex++) {
        if ((gSavemap.phsVisibilityMask & (1 << charIndex)) != 0) {
            g_MenuMateriaExchangeSlots[visibleCount] = (unsigned char)charIndex;
            visibleCount++;
        }
    }
    g_MenuMateriaInitializedFlag = (unsigned char)visibleCount;
    MenuCursor_Init((MenuCursor *)&g_MenuMateriaStatDelta3, 0, 0, 1, 0xa, 0, 0, 1, 0xc8, 0, 0, 0,
                    0, 0, 1);
    MenuCursor_Init((MenuCursor *)&g_MenuMateriaExchangeSlotCursor, 0, 0, 8, 2, 0, 0, 8, 2, 0, 0, 0, 0, 0,
                    0);
    MenuCursor_Init((MenuCursor *)&g_MenuMateriaSortMode, 0, 0, 1, 3, 0, 0, 1, 3, 0, 0, 0, 0, 0,
                    0);
}

/*
 * Draws party materia exchange slots in 2x mode.
 */
// FUNCTION: FF7 0x0070ecf4
void __cdecl Menu_DrawMateriaExchangeList2x(unsigned int flags)
{
    struct {
        int slot;
        int rows;
        int rowY;
        int rowX;
        int rowIdx;
        int remaining;
        int last;
        short rect[4];
        struct SavemapCharacter *record;
        unsigned int colour;
    } v; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    v.last = g_MenuMateriaInitializedFlag > 4 ? 3 : g_MenuMateriaInitializedFlag - 1;
    v.remaining = g_MenuMateriaInitializedFlag - 4;
    v.record = &gSavemap.chars[g_MenuMateriaExchangeSlots[g_MenuMateriaExchangeScrollTop + g_MenuMateriaExchangeRow]];
    if (g_MenuMateriaCursorParam6 != 0) {
        switch (g_MenuMateriaSelectedSlot) {
        case MENU_MATERIA_EXCHANGE_PANE_SLOTS:
            if ((flags & 2) && g_MenuMateriaExchangeCursor - g_MenuMateriaExchangeScrollTop >= 0) {
                Menu_DrawMoreTextArrow2x(g_MenuMateriaScrollMax * 0x1b + 0x45,
                                    g_MenuMateriaScrollTop * 0x1a + 0x87 +
                                        (g_MenuMateriaExchangeCursor - g_MenuMateriaExchangeScrollTop) * 0x5c + g_MenuMateriaCursorParam4 * 0xb,
                                    0.0f);
            }
            break;
        case MENU_MATERIA_EXCHANGE_PANE_ROWS:
            if ((flags & 2) && g_MenuMateriaExchangeCursor - g_MenuMateriaExchangeScrollTop >= 0) {
                Menu_DrawMoreTextArrow2x(0xc, g_MenuMateriaScrollTop * 0x1a + 0x78 + (g_MenuMateriaExchangeCursor - g_MenuMateriaExchangeScrollTop) * 0x5e + g_MenuMateriaCursorParam4 * 0xb, 0.0f);
            }
            break;
        }
    }
    g_MateriaDescribeModeFlag = 1;
    switch (g_MenuMateriaCursorParam1) {
    case MENU_MATERIA_EXCHANGE_PANE_SLOTS:
        Menu_DrawMoreTextArrow2x(g_MenuMateriaExchangeSlotCursor * 0x1c + 0x48,
                            g_MenuMateriaStatDelta9 * 0x1a + 0x8b + g_MenuMateriaExchangeRow * 0x5e, 0.01f);
        Menu_EmptyTextHook(0x80);
        switch (g_MenuMateriaStatDelta9) {
        case EQUIP_WEAPON:
            if (v.record->weaponMateria[g_MenuMateriaExchangeSlotCursor] != -1) {
                Menu_DrawEncodedString2x(0x10, 0x40, Menu_LookupEntryNameByMode(MENU_MATERIA_NAME_MATERIA, v.record->weaponMateria[g_MenuMateriaExchangeSlotCursor] & 0xff), 7, 0.01f);
                v.colour = Menu_DrawMateriaNameAndApStars2x(0x1b, 0xd, v.record->weaponMateria[g_MenuMateriaExchangeSlotCursor]);
            }
            break;
        case EQUIP_ARMOR:
            if (v.record->armorMateria[g_MenuMateriaExchangeSlotCursor] != -1) {
                Menu_DrawEncodedString2x(0x10, 0x40, Menu_LookupEntryNameByMode(MENU_MATERIA_NAME_MATERIA, v.record->armorMateria[g_MenuMateriaExchangeSlotCursor] & 0xff), 7, 0.01f);
                v.colour = Menu_DrawMateriaNameAndApStars2x(0x1b, 0xd, v.record->armorMateria[g_MenuMateriaExchangeSlotCursor]);
            }
            break;
        }
        break;
    case MENU_MATERIA_EXCHANGE_PANE_STOCK:
        Menu_DrawMoreTextArrow2x(g_MenuMateriaWindowPtr->frames[9].x - 0x12,
                            g_MenuMateriaWindowPtr->frames[9].y + 0x11 + g_MenuMateriaStatDelta4 * 0x24, 0.01f);
        if (gSavemap.materia[g_MenuMateriaStatDelta4 + g_MenuMateriaStatDelta5] != -1) {
            Menu_DrawEncodedString2x(0x10, 0x40, Menu_LookupEntryNameByMode(MENU_MATERIA_NAME_MATERIA, gSavemap.materia[g_MenuMateriaStatDelta4 + g_MenuMateriaStatDelta5] & 0xff), 7, 0.01f);
            v.colour = Menu_DrawMateriaNameAndApStars2x(0x1b, 0xd, gSavemap.materia[g_MenuMateriaStatDelta4 + g_MenuMateriaStatDelta5]);
        }
        break;
    case MENU_MATERIA_EXCHANGE_PANE_ROWS:
        Menu_DrawMoreTextArrow2x(0xf, g_MenuMateriaSortOrder * 0x1a + 0x72 + g_MenuMateriaExchangeRow * 0x5e, 0.01f);
        break;
    }
    Menu_DrawMateriaExchangeStockList2x();
    Menu_DrawWindowFrame2x(&g_MenuMateriaWindowPtr->frames[7], 0.1f);
    Menu_DrawWindowFrame2x(&g_MenuMateriaWindowPtr->frames[8], 0.1f);
    Menu_DrawWindowFrame2x(&g_MenuMateriaWindowPtr->frames[9], 0.2f);
    v.rows = (g_MenuMateriaCursorParam5 != MENU_MATERIA_EXCHANGE_SCROLL_NONE) + 4;
    Menu_FlushAndSetScissor2x(0, 0x67, 0x280, 0x179);
    for (v.rowIdx = 0; v.rowIdx < v.rows; v.rowIdx++) {
        v.slot = g_MenuMateriaExchangeScrollTop + v.rowIdx;
        v.rowX = 0;
        v.rowY = v.rowIdx * 0x5e + 0x67 + g_MenuMateriaCursorParam4 * 0xb;
        if (g_MenuMateriaExchangeSlots[v.slot] != 0xff) {
            Menu_DrawMateriaSlotRow(v.rowX + 0x70, v.rowY + 0x20,
                                    g_KernelWeaponDataArray[g_MenuMateriaActorEquipPtr[g_MenuMateriaExchangeSlots[v.slot]].weapon].materiaSlotTable,
                                    g_MenuMateriaActorEquipPtr[g_MenuMateriaExchangeSlots[v.slot]].weaponMateria);
            Menu_DrawMateriaSlotRow(v.rowX + 0x70, v.rowY + 0x3a,
                                    g_KernelArmorDataArray[g_MenuMateriaActorEquipPtr[g_MenuMateriaExchangeSlots[v.slot]].armor].materiaSlotBytes,
                                    g_MenuMateriaActorEquipPtr[g_MenuMateriaExchangeSlots[v.slot]].armorMateria);
            Menu_DrawEncodedString2x(v.rowX + 0x37, v.rowY + 9, gSavemap.chars[g_MenuMateriaExchangeSlots[v.slot]].name, 7, 0.2f);
            Menu_DrawEncodedString2x(v.rowX + 0x37, v.rowY + 0x22, g_MenuMateriaLabelTable, 5, 0.2f);
            Menu_DrawEncodedString2x(v.rowX + 0x37, v.rowY + 0x3c, &g_MenuMateriaLabelTable[1 * 0x14], 5, 0.2f);
        }
        Menu_SetRectShorts(v.rect, v.rowX, v.rowY, 0x167, 0x5f);
        Menu_DrawWindowFrame2x((EnglishWindowFrame *)v.rect, 0.3f);
    }
    Menu_FlushAndResetScissor2x();
    if (v.remaining > 0) {
        g_MenuMateriaScrollBar.visibleRows = 4;
        g_MenuMateriaScrollBar.totalRows = g_MenuMateriaInitializedFlag;
        g_MenuMateriaScrollBarPos.scrollTop = (short)g_MenuMateriaExchangeScrollTop;
        g_MenuMateriaScrollBarPos.x = g_MenuMateriaWindowPtr->frames[10].x + 9;
        g_MenuMateriaScrollBarSize.y = 0x6d;
        g_MenuMateriaScrollBarSize.width = 0x11;
        g_MenuMateriaScrollBarHeight = 0x16d;
        Menu_DrawScrollBar2x(&g_MenuMateriaScrollBar.visibleRows, 0.4f);
    }
    Menu_DrawWindowFrame2x(&g_MenuMateriaWindowPtr->frames[10], 0.4f);
}

/*
 * Draws stock list for materia exchange screen in 2x mode.
 */
// FUNCTION: FF7 0x0070f2e9
void __cdecl Menu_DrawMateriaExchangeStockList2x(void)
{
    int row;
    int typeColor;
    int topY;
    int positionX;
    int visibleRows;

    Menu_FlushAndSetScissor2x(0, 0, 0x280, 0x1E0);
    positionX = 0x1AB;
    topY = 0x74;
    visibleRows = (g_MenuMateriaStatDelta7 != 0) + 0xA;
    for (row = 0; row < visibleRows; row++) {
        if (gSavemap.materia[row + g_MenuMateriaStatDelta5] != -1) {
            Menu_DrawEncodedString2x(positionX, topY + row * 0x24 + g_MenuMateriaStatDelta6 * 9, Kernel_LookupMateriaNameText(gSavemap.materia[row + g_MenuMateriaStatDelta5] & 0xFF), 7, 0.2f);
        }
    }
    g_MenuMateriaScrollBar.visibleRows = 0xA;
    g_MenuMateriaScrollBar.totalRows = 0xC8;
    g_MenuMateriaScrollBarPos.scrollTop = *(short *)&g_MenuMateriaStatDelta5;
    g_MenuMateriaScrollBarPos.x = 0x26A;
    g_MenuMateriaScrollBarSize.y = 0x6D;
    g_MenuMateriaScrollBarSize.width = 0x11;
    g_MenuMateriaScrollBarHeight = 0x16D;
    Menu_DrawScrollBar2x(&g_MenuMateriaScrollBar.visibleRows, 0.2f);
    for (row = 0; row < visibleRows; row++) {
        typeColor = Menu_GetMateriaTypeColorIndex(gSavemap.materia[row + g_MenuMateriaStatDelta5] & 0xFF);
        if (gSavemap.materia[row + g_MenuMateriaStatDelta5] != -1) {
            Menu_DrawAtlasQuad2x(positionX - 0x1B, (topY - 2) + row * 0x24 + g_MenuMateriaStatDelta6 * 9, 0x80, 0x20, 0x10, 0x10, typeColor, 0, 0, 0.2f);
        }
    }
    Menu_FlushAndResetScissor2x();
}

/*
 * Draws materia name, level stars, and AP indicator in 2x mode.
 */
// FUNCTION: FF7 0x0070f491
unsigned int __cdecl Menu_DrawMateriaNameAndApStars2x(int x, int y, unsigned int item)
{
    unsigned int colour;
    int code;
    int n;

    code = item & 0xff;
    Menu_DrawEncodedString2x(x + 0x1b, y, Kernel_LookupMateriaNameText(item & 0xff), 7, 0.01f);
    colour = Menu_GetMateriaTypeColorIndex(item & 0xff);
    Menu_DrawAtlasQuad2x(x, y - 2, 0x80, 0x20, 0x10, 0x10, colour, 0, 0, 0.01f);
    Kernel_ApplyMateriaToActor(item);
    if (code != 0x30 && code != 0x49 && code != 0x5a && code != 0x2c) {
        for (n = 0; n < g_MenuMateriaDetailPtr->maxLevel; n++) {
            if (n < g_MenuMateriaDetailPtr->level) {
                Menu_DrawAtlasQuad2x(n * 0x19 + 0x11b, 0xd, 0x90, 0x20, 0x10, 0x10,
                                     colour, 0, 0, 0.01f);
            } else {
                Menu_DrawAtlasQuad2x(n * 0x19 + 0x11b, 0xd, 0x90, 0x30, 0x10, 0x10,
                                     colour, 0, 0, 0.01f);
            }
        }
    }
    return colour;
}

/*
 * Draws party materia exchange slots in 1x mode.
 */
// FUNCTION: FF7 0x0070f5de
void __cdecl Menu_DrawMateriaExchangeList1x(unsigned int flags)
{
    struct {
        int slot;
        int rows;
        int y;
        int x;
        int i;
        int remaining;
        int last;
        short rect[4];
        struct SavemapCharacter *record;
        unsigned int colour;
    } state;

    state.last = g_MenuMateriaInitializedFlag > 4 ? 3 : g_MenuMateriaInitializedFlag - 1;
    state.remaining = g_MenuMateriaInitializedFlag - 4;
    state.record = &gSavemap.chars[g_MenuMateriaExchangeSlots[g_MenuMateriaExchangeScrollTop + g_MenuMateriaExchangeRow]];
    if (g_MenuMateriaCursorParam6 != 0) {
        switch (g_MenuMateriaSelectedSlot) {
        case MENU_MATERIA_EXCHANGE_PANE_SLOTS:
            if ((flags & 2) && g_MenuMateriaExchangeCursor - g_MenuMateriaExchangeScrollTop >= 0) {
                Menu_DrawMoreTextArrow1x(g_MenuMateriaScrollMax * 0xe + 0x22,
                                    g_MenuMateriaScrollTop * 0xd + 0x43 +
                                        (g_MenuMateriaExchangeCursor - g_MenuMateriaExchangeScrollTop) * 0x2e + g_MenuMateriaCursorParam4 * 5,
                                    0.0f);
            }
            break;
        case MENU_MATERIA_EXCHANGE_PANE_ROWS:
            if ((flags & 2) && g_MenuMateriaExchangeCursor - g_MenuMateriaExchangeScrollTop >= 0) {
                Menu_DrawMoreTextArrow1x(6, g_MenuMateriaScrollTop * 0xd + 0x3c + (g_MenuMateriaExchangeCursor - g_MenuMateriaExchangeScrollTop) * 0x2f + g_MenuMateriaCursorParam4 * 5, 0.0f);
            }
            break;
        }
    }
    g_MateriaDescribeModeFlag = 1;
    switch (g_MenuMateriaCursorParam1) {
    case MENU_MATERIA_EXCHANGE_PANE_SLOTS:
        Menu_DrawMoreTextArrow1x(g_MenuMateriaExchangeSlotCursor * 0xe + 0x24,
                            g_MenuMateriaStatDelta9 * 0xd + 0x46 + g_MenuMateriaExchangeRow * 0x2f, 0.01f);
        Menu_EmptyTextHook(0x80);
        switch (g_MenuMateriaStatDelta9) {
        case EQUIP_WEAPON:
            if (state.record->weaponMateria[g_MenuMateriaExchangeSlotCursor] != -1) {
                Menu_DrawEncodedString1x(8, 0x20, Menu_LookupEntryNameByMode(MENU_MATERIA_NAME_MATERIA, state.record->weaponMateria[g_MenuMateriaExchangeSlotCursor] & 0xff), 7, 0.01f);
                state.colour = Menu_DrawMateriaNameAndApStars1x(0xe, 7, state.record->weaponMateria[g_MenuMateriaExchangeSlotCursor]);
            }
            break;
        case EQUIP_ARMOR:
            if (state.record->armorMateria[g_MenuMateriaExchangeSlotCursor] != -1) {
                Menu_DrawEncodedString1x(8, 0x20, Menu_LookupEntryNameByMode(MENU_MATERIA_NAME_MATERIA, state.record->armorMateria[g_MenuMateriaExchangeSlotCursor] & 0xff), 7, 0.01f);
                state.colour = Menu_DrawMateriaNameAndApStars1x(0xe, 7, state.record->armorMateria[g_MenuMateriaExchangeSlotCursor]);
            }
            break;
        }
        break;
    case MENU_MATERIA_EXCHANGE_PANE_STOCK:
        Menu_DrawMoreTextArrow1x(g_MenuMateriaWindowPtr->frames[9].x - 9,
                            g_MenuMateriaWindowPtr->frames[9].y + 9 + g_MenuMateriaStatDelta4 * 0x12, 0.01f);
        if (gSavemap.materia[g_MenuMateriaStatDelta4 + g_MenuMateriaStatDelta5] != -1) {
            Menu_DrawEncodedString1x(8, 0x20, Menu_LookupEntryNameByMode(MENU_MATERIA_NAME_MATERIA, gSavemap.materia[g_MenuMateriaStatDelta4 + g_MenuMateriaStatDelta5] & 0xff), 7, 0.01f);
            state.colour = Menu_DrawMateriaNameAndApStars1x(0xe, 7, gSavemap.materia[g_MenuMateriaStatDelta4 + g_MenuMateriaStatDelta5]);
        }
        break;
    case MENU_MATERIA_EXCHANGE_PANE_ROWS:
        Menu_DrawMoreTextArrow1x(8, g_MenuMateriaSortOrder * 0xd + 0x39 + g_MenuMateriaExchangeRow * 0x2f, 0.01f);
        break;
    }
    Menu_DrawMateriaExchangeStockList1x();
    Menu_DrawWindowFrame1x(&g_MenuMateriaWindowPtr->frames[7], 0.1f);
    Menu_DrawWindowFrame1x(&g_MenuMateriaWindowPtr->frames[8], 0.1f);
    Menu_DrawWindowFrame1x(&g_MenuMateriaWindowPtr->frames[9], 0.2f);
    state.rows = (g_MenuMateriaCursorParam5 != MENU_MATERIA_EXCHANGE_SCROLL_NONE) + 4;
    Menu_FlushAndSetScissor1x(0, 0x33, 0x140, 0xbd);
    for (state.i = 0; state.i < state.rows; state.i++) {
        state.slot = g_MenuMateriaExchangeScrollTop + state.i;
        state.x = 0;
        state.y = state.i * 0x2f + 0x34 + g_MenuMateriaCursorParam4 * 5;
        if (g_MenuMateriaExchangeSlots[state.slot] != 0xff) {
            Menu_DrawMateriaSlotRow1x(state.x + 0x38, state.y + 0x10,
                                      g_KernelWeaponDataArray[g_MenuMateriaActorEquipPtr[g_MenuMateriaExchangeSlots[state.slot]].weapon].materiaSlotTable,
                                      g_MenuMateriaActorEquipPtr[g_MenuMateriaExchangeSlots[state.slot]].weaponMateria);
            Menu_DrawMateriaSlotRow1x(state.x + 0x38, state.y + 0x1d,
                                      g_KernelArmorDataArray[g_MenuMateriaActorEquipPtr[g_MenuMateriaExchangeSlots[state.slot]].armor].materiaSlotBytes,
                                      g_MenuMateriaActorEquipPtr[g_MenuMateriaExchangeSlots[state.slot]].armorMateria);
            Menu_DrawEncodedString1x(state.x + 0x1c, state.y + 5, gSavemap.chars[g_MenuMateriaExchangeSlots[state.slot]].name, 7, 0.2f);
            Menu_DrawEncodedString1x(state.x + 0x1c, state.y + 0x11, g_MenuMateriaLabelTable, 5, 0.2f);
            Menu_DrawEncodedString1x(state.x + 0x1c, state.y + 0x1e, &g_MenuMateriaLabelTable[1 * 0x14], 5, 0.2f);
        }
        Menu_SetRectShorts(state.rect, state.x, state.y, 0xb4, 0x2f);
        Menu_DrawWindowFrame1x((EnglishWindowFrame *)state.rect, 0.3f);
    }
    Menu_FlushAndResetScissor1x();
    if (state.remaining > 0) {
        g_MenuMateriaScrollBar.visibleRows = 4;
        g_MenuMateriaScrollBar.totalRows = g_MenuMateriaInitializedFlag;
        g_MenuMateriaScrollBarPos.scrollTop = (short)g_MenuMateriaExchangeScrollTop;
        g_MenuMateriaScrollBarPos.x = g_MenuMateriaWindowPtr->frames[10].x + 5;
        g_MenuMateriaScrollBarSize.y = 0x37;
        g_MenuMateriaScrollBarSize.width = 9;
        g_MenuMateriaScrollBarHeight = 0xb7;
        Menu_DrawScrollBar1x(&g_MenuMateriaScrollBar.visibleRows, 0.4f);
    }
    Menu_DrawWindowFrame1x(&g_MenuMateriaWindowPtr->frames[10], 0.4f);
}

/*
 * Draws stock list for materia exchange screen in 1x mode.
 */
// FUNCTION: FF7 0x0070fbcd
void __cdecl Menu_DrawMateriaExchangeStockList1x(void)
{
    int row;
    int typeColor;
    int topY;
    int positionX;
    int visibleRows;

    Menu_FlushAndSetScissor1x(0, 0, 0x140, 0xF0);
    positionX = 0xD6;
    topY = 0x3A;
    visibleRows = (g_MenuMateriaStatDelta7 != 0) + 0xA;
    for (row = 0; row < visibleRows; row++) {
        if (gSavemap.materia[row + g_MenuMateriaStatDelta5] != -1) {
            Menu_DrawEncodedString1x(positionX, topY + row * 0x12 + g_MenuMateriaStatDelta6 * 5, Kernel_LookupMateriaNameText(gSavemap.materia[row + g_MenuMateriaStatDelta5] & 0xFF), 7, 0.2f);
        }
    }
    g_MenuMateriaScrollBar.visibleRows = 0xA;
    g_MenuMateriaScrollBar.totalRows = 0xC8;
    g_MenuMateriaScrollBarPos.scrollTop = *(short *)&g_MenuMateriaStatDelta5;
    g_MenuMateriaScrollBarPos.x = 0x135;
    g_MenuMateriaScrollBarSize.y = 0x37;
    g_MenuMateriaScrollBarSize.width = 9;
    g_MenuMateriaScrollBarHeight = 0xB7;
    Menu_DrawScrollBar1x(&g_MenuMateriaScrollBar.visibleRows, 0.2f);
    for (row = 0; row < visibleRows; row++) {
        typeColor = Menu_GetMateriaTypeColorIndex(gSavemap.materia[row + g_MenuMateriaStatDelta5] & 0xFF);
        if (gSavemap.materia[row + g_MenuMateriaStatDelta5] != -1) {
            Menu_DrawAtlasQuad1x(positionX - 0xE, (topY - 2) + row * 0x12 + g_MenuMateriaStatDelta6 * 5, 0x80, 0x20, 0x10, 0x10, typeColor, 0, 0, 0.2f);
        }
    }
    Menu_FlushAndResetScissor1x();
}

/*
 * Draws materia name, level stars, and AP indicator in 1x mode.
 */
// FUNCTION: FF7 0x0070fd75
unsigned int __cdecl Menu_DrawMateriaNameAndApStars1x(int x, int y, unsigned int item)
{
    unsigned int colour;
    int code;
    int n;

    code = item & 0xff;
    Menu_DrawEncodedString1x(x + 0xe, y, Kernel_LookupMateriaNameText(item & 0xff), 7, 0.01f);
    colour = Menu_GetMateriaTypeColorIndex(item & 0xff);
    Menu_DrawAtlasQuad1x(x, y - 1, 0x80, 0x20, 0x10, 0x10, colour, 0, 0, 0.01f);
    Kernel_ApplyMateriaToActor(item);
    if (code != 0x30 && code != 0x49 && code != 0x5a && code != 0x2c) {
        for (n = 0; n < g_MenuMateriaDetailPtr->maxLevel; n++) {
            if (n < g_MenuMateriaDetailPtr->level) {
                Menu_DrawAtlasQuad1x(n * 0xd + 0x8e, 7, 0x90, 0x20, 0x10, 0x10,
                                     colour, 0, 0, 0.01f);
            } else {
                Menu_DrawAtlasQuad1x(n * 0xd + 0x8e, 7, 0x90, 0x30, 0x10, 0x10,
                                     colour, 0, 0, 0.01f);
            }
        }
    }
    return colour;
}

/*
 * Handles cursor movement and materia swapping on exchange screen.
 */
// FUNCTION: FF7 0x0070fec2
void __cdecl Menu_MateriaExchangeHandleInput(void)
{
    /* Stack names preserve the measured VC5 local hash order. */
    int *slotPtrA;
    int *materiaPointerB;
    int temp;
    unsigned char *tableA;
    int member;
    int maxScrollTop;
    int previousSlotCursor;
    unsigned char *tmpSlotTable;
    unsigned char *slotCapsB;
    int oldDelta;
    int bottomRow;

    bottomRow = g_MenuMateriaInitializedFlag > 4 ? 3 : g_MenuMateriaInitializedFlag - 1;
    maxScrollTop = g_MenuMateriaInitializedFlag - 4;
    if (g_MenuMateriaCursorParam5 == MENU_MATERIA_EXCHANGE_SCROLL_NONE) {
        if (Input_GetPressed(INPUT_CROSS) != 0) {
            if (g_MenuMateriaCursorParam6 == 0) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                Menu_SetFrameClearFlag(1);
                g_MenuMateriaCursorParam0 = 0;
                g_MenuMateriaState = MATERIA_MENU_ARRANGE_MENU;
                for (member = 0; member < 3; member++) {
                    Savemap_BuildPartyStatsFromRecord(member);
                    Battle_LoadCombatantFromSavemap((char)member);
                }
                Party_RecomputeBattleModifiers();
            } else {
                g_MenuMateriaCursorParam6 = 0;
                if (g_MenuMateriaCursorParam1 == MENU_MATERIA_EXCHANGE_PANE_STOCK) {
                    g_MenuMateriaCursorParam1 = MENU_MATERIA_EXCHANGE_PANE_SLOTS;
                }
            }
        } else if (Input_GetPressed(INPUT_TRIANGLE) != 0 && g_MenuMateriaCursorParam1 == MENU_MATERIA_EXCHANGE_PANE_SLOTS) {
            switch (g_MenuMateriaStatDelta9) {
            case EQUIP_WEAPON:
                tmpSlotTable = g_KernelWeaponDataArray[gSavemap.chars[g_MenuMateriaExchangeSlots[(g_MenuMateriaExchangeScrollTop + g_MenuMateriaExchangeRow)]].weapon].materiaSlotTable;
                slotPtrA = &gSavemap.chars[g_MenuMateriaExchangeSlots[(g_MenuMateriaExchangeScrollTop + g_MenuMateriaExchangeRow)]].weaponMateria[g_MenuMateriaExchangeSlotCursor];
                if (tmpSlotTable[g_MenuMateriaExchangeSlotCursor] != 0) {
                    if (*slotPtrA != -1) {
                        if (Savemap_AddMateria(*slotPtrA) == -1) {
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_MATERIA_REMOVE);
                            *slotPtrA = 0xFFFFFFFF;
                        } else {
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                        }
                    } else {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                    }
                } else {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                }
                break;
            case EQUIP_ARMOR:
                tmpSlotTable = g_KernelArmorDataArray[gSavemap.chars[g_MenuMateriaExchangeSlots[(g_MenuMateriaExchangeScrollTop + g_MenuMateriaExchangeRow)]].armor].materiaSlotBytes;
                slotPtrA = &gSavemap.chars[g_MenuMateriaExchangeSlots[(g_MenuMateriaExchangeScrollTop + g_MenuMateriaExchangeRow)]].armorMateria[g_MenuMateriaExchangeSlotCursor];
                if (tmpSlotTable[g_MenuMateriaActiveSlot] != 0) {
                    if (*slotPtrA != -1) {
                        if (Savemap_AddMateria(*slotPtrA) == -1) {
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_MATERIA_REMOVE);
                            *slotPtrA = 0xFFFFFFFF;
                        } else {
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                        }
                    } else {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                    }
                } else {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                }
            }
        } else if (Input_GetPressed(INPUT_CIRCLE) != 0) {
            if (g_MenuMateriaCursorParam6 == 0) {
                g_MenuMateriaSelectedSlot = g_MenuMateriaCursorParam1;
                switch (g_MenuMateriaCursorParam1) {
                case MENU_MATERIA_EXCHANGE_PANE_SLOTS:
                    if (g_MenuMateriaStatDelta9 == EQUIP_WEAPON) {
                        tmpSlotTable = g_KernelWeaponDataArray[gSavemap.chars[g_MenuMateriaExchangeSlots[(g_MenuMateriaExchangeScrollTop + g_MenuMateriaExchangeRow)]].weapon].materiaSlotTable;
                    } else {
                        tmpSlotTable = g_KernelArmorDataArray[gSavemap.chars[g_MenuMateriaExchangeSlots[(g_MenuMateriaExchangeScrollTop + g_MenuMateriaExchangeRow)]].armor].materiaSlotBytes;
                    }
                    if (tmpSlotTable[g_MenuMateriaExchangeSlotCursor] != 0) {
                        g_MenuMateriaScrollMax = g_MenuMateriaExchangeSlotCursor;
                        g_MenuMateriaScrollTop = g_MenuMateriaStatDelta9;
                        g_MenuMateriaExchangeCursor = g_MenuMateriaExchangeScrollTop + g_MenuMateriaExchangeRow;
                        g_MenuMateriaCursorParam6 = 1;
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    } else {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                    }
                    break;
                case MENU_MATERIA_EXCHANGE_PANE_ROWS:
                    g_MenuMateriaScrollMax = 0;
                    g_MenuMateriaScrollTop = g_MenuMateriaSortOrder;
                    g_MenuMateriaExchangeCursor = g_MenuMateriaExchangeScrollTop + g_MenuMateriaExchangeRow;
                    g_MenuMateriaCursorParam6 = 1;
                }
            } else {
                switch (g_MenuMateriaSelectedSlot) {
                case MENU_MATERIA_EXCHANGE_PANE_SLOTS:
                    if (g_MenuMateriaCursorParam1 == MENU_MATERIA_EXCHANGE_PANE_SLOTS) {
                        if (g_MenuMateriaStatDelta9 == EQUIP_WEAPON) {
                            tmpSlotTable = g_KernelWeaponDataArray[gSavemap.chars[g_MenuMateriaExchangeSlots[(g_MenuMateriaExchangeScrollTop + g_MenuMateriaExchangeRow)]].weapon].materiaSlotTable;
                        } else {
                            tmpSlotTable = g_KernelArmorDataArray[gSavemap.chars[g_MenuMateriaExchangeSlots[(g_MenuMateriaExchangeScrollTop + g_MenuMateriaExchangeRow)]].armor].materiaSlotBytes;
                        }
                        if (tmpSlotTable[g_MenuMateriaExchangeSlotCursor] == 0) {
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                            break;
                        }
                    }
                    if (g_MenuMateriaScrollTop == EQUIP_WEAPON) {
                        slotPtrA = &gSavemap.chars[g_MenuMateriaExchangeSlots[g_MenuMateriaExchangeCursor]].weaponMateria[g_MenuMateriaScrollMax];
                    } else {
                        slotPtrA = &gSavemap.chars[g_MenuMateriaExchangeSlots[g_MenuMateriaExchangeCursor]].armorMateria[g_MenuMateriaScrollMax];
                    }
                    switch (g_MenuMateriaCursorParam1) {
                    case MENU_MATERIA_EXCHANGE_PANE_SLOTS:
                        if (g_MenuMateriaStatDelta9 == EQUIP_WEAPON) {
                            materiaPointerB = &gSavemap.chars[g_MenuMateriaExchangeSlots[(g_MenuMateriaExchangeScrollTop + g_MenuMateriaExchangeRow)]].weaponMateria[g_MenuMateriaExchangeSlotCursor];
                        } else {
                            materiaPointerB = &gSavemap.chars[g_MenuMateriaExchangeSlots[(g_MenuMateriaExchangeScrollTop + g_MenuMateriaExchangeRow)]].armorMateria[g_MenuMateriaExchangeSlotCursor];
                        }
                        break;
                    case MENU_MATERIA_EXCHANGE_PANE_STOCK:
                        materiaPointerB = &gSavemap.materia[(g_MenuMateriaStatDelta4 + g_MenuMateriaStatDelta5)];
                    }
                    temp = *slotPtrA;
                    *slotPtrA = *materiaPointerB;
                    *materiaPointerB = temp;
                    g_MenuMateriaCursorParam6 = 0;
                    if (g_MenuMateriaCursorParam1 == MENU_MATERIA_EXCHANGE_PANE_STOCK) {
                        g_MenuMateriaCursorParam1 = MENU_MATERIA_EXCHANGE_PANE_SLOTS;
                    }
                    break;
                case MENU_MATERIA_EXCHANGE_PANE_ROWS:
                    switch (g_MenuMateriaScrollTop) {
                    case MENU_MATERIA_EXCHANGE_ROW_ALL:
                        if (g_MenuMateriaSortOrder != MENU_MATERIA_EXCHANGE_ROW_ALL) {
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                            break;
                        }
                        tableA = g_KernelWeaponDataArray[gSavemap.chars[g_MenuMateriaExchangeSlots[g_MenuMateriaExchangeCursor]].weapon].materiaSlotTable;
                        slotPtrA = gSavemap.chars[g_MenuMateriaExchangeSlots[g_MenuMateriaExchangeCursor]].weaponMateria;
                        slotCapsB = g_KernelWeaponDataArray[gSavemap.chars[g_MenuMateriaExchangeSlots[(g_MenuMateriaExchangeScrollTop + g_MenuMateriaExchangeRow)]].weapon].materiaSlotTable;
                        materiaPointerB = gSavemap.chars[g_MenuMateriaExchangeSlots[(g_MenuMateriaExchangeScrollTop + g_MenuMateriaExchangeRow)]].weaponMateria;
                        Menu_SwapEquippedMateriaWhereBothSlotsExist(tableA, slotCapsB, slotPtrA, materiaPointerB);
                        tableA = g_KernelArmorDataArray[gSavemap.chars[g_MenuMateriaExchangeSlots[g_MenuMateriaExchangeCursor]].armor].materiaSlotBytes;
                        slotPtrA = gSavemap.chars[g_MenuMateriaExchangeSlots[g_MenuMateriaExchangeCursor]].armorMateria;
                        slotCapsB = g_KernelArmorDataArray[gSavemap.chars[g_MenuMateriaExchangeSlots[(g_MenuMateriaExchangeScrollTop + g_MenuMateriaExchangeRow)]].armor].materiaSlotBytes;
                        materiaPointerB = gSavemap.chars[g_MenuMateriaExchangeSlots[(g_MenuMateriaExchangeScrollTop + g_MenuMateriaExchangeRow)]].armorMateria;
                        Menu_SwapEquippedMateriaWhereBothSlotsExist(tableA, slotCapsB, slotPtrA, materiaPointerB);
                        g_MenuMateriaCursorParam6 = 0;
                        break;
                    case MENU_MATERIA_EXCHANGE_ROW_WEAPON:
                    case MENU_MATERIA_EXCHANGE_ROW_ARMOR:
                        if (g_MenuMateriaSortOrder == MENU_MATERIA_EXCHANGE_ROW_ALL) {
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                        } else {
                            if (g_MenuMateriaScrollTop == MENU_MATERIA_EXCHANGE_ROW_WEAPON) {
                                tableA = g_KernelWeaponDataArray[gSavemap.chars[g_MenuMateriaExchangeSlots[g_MenuMateriaExchangeCursor]].weapon].materiaSlotTable;
                                slotPtrA = gSavemap.chars[g_MenuMateriaExchangeSlots[g_MenuMateriaExchangeCursor]].weaponMateria;
                            } else {
                                tableA = g_KernelArmorDataArray[gSavemap.chars[g_MenuMateriaExchangeSlots[g_MenuMateriaExchangeCursor]].armor].materiaSlotBytes;
                                slotPtrA = gSavemap.chars[g_MenuMateriaExchangeSlots[g_MenuMateriaExchangeCursor]].armorMateria;
                            }
                            if (g_MenuMateriaSortOrder == MENU_MATERIA_EXCHANGE_ROW_WEAPON) {
                                slotCapsB = g_KernelWeaponDataArray[gSavemap.chars[g_MenuMateriaExchangeSlots[(g_MenuMateriaExchangeScrollTop + g_MenuMateriaExchangeRow)]].weapon].materiaSlotTable;
                                materiaPointerB = gSavemap.chars[g_MenuMateriaExchangeSlots[(g_MenuMateriaExchangeScrollTop + g_MenuMateriaExchangeRow)]].weaponMateria;
                            } else {
                                slotCapsB = g_KernelArmorDataArray[gSavemap.chars[g_MenuMateriaExchangeSlots[(g_MenuMateriaExchangeScrollTop + g_MenuMateriaExchangeRow)]].armor].materiaSlotBytes;
                                materiaPointerB = gSavemap.chars[g_MenuMateriaExchangeSlots[(g_MenuMateriaExchangeScrollTop + g_MenuMateriaExchangeRow)]].armorMateria;
                            }
                            Menu_SwapEquippedMateriaWhereBothSlotsExist(tableA, slotCapsB, slotPtrA, materiaPointerB);
                            g_MenuMateriaCursorParam6 = 0;
                        }
                    }
                }
            }
        } else {
            switch (g_MenuMateriaCursorParam1) {
            case MENU_MATERIA_EXCHANGE_PANE_SLOTS:
                previousSlotCursor = g_MenuMateriaExchangeSlotCursor;
                oldDelta = g_MenuMateriaStatDelta9;
                MenuCursor_Update((MenuCursor *)&g_MenuMateriaExchangeSlotCursor);
                if (MenuCursor_ReadRepeat(INPUT_RIGHT, MOUSE_DIR_RIGHT) != 0 && previousSlotCursor == g_MenuMateriaExchangeSlotCursor) {
                    if (g_MenuMateriaCursorParam6 != 0) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        g_MenuMateriaCursorParam1 = MENU_MATERIA_EXCHANGE_PANE_STOCK;
                    }
                } else if (MenuCursor_ReadRepeat(INPUT_DOWN, MOUSE_DIR_DOWN) != 0 && oldDelta == g_MenuMateriaStatDelta9) {
                    if (g_MenuMateriaExchangeRow < bottomRow) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        g_MenuMateriaExchangeRow++;
                        g_MenuMateriaStatDelta9 = EQUIP_WEAPON;
                    } else if (g_MenuMateriaExchangeRow == 3 && g_MenuMateriaExchangeScrollTop < maxScrollTop) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        g_MenuMateriaCursorParam5 = MENU_MATERIA_EXCHANGE_SCROLL_DOWN;
                        g_MenuMateriaCursorParam4 = 0xFFFFFFFF;
                        g_MenuMateriaStatDelta9 = EQUIP_WEAPON;
                    }
                } else if (MenuCursor_ReadRepeat(INPUT_UP, MOUSE_DIR_UP) != 0 && oldDelta == g_MenuMateriaStatDelta9) {
                    if (g_MenuMateriaExchangeRow != 0) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        g_MenuMateriaExchangeRow--;
                        g_MenuMateriaStatDelta9 = EQUIP_ARMOR;
                    } else if (g_MenuMateriaExchangeScrollTop > 0) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        g_MenuMateriaCursorParam5 = MENU_MATERIA_EXCHANGE_SCROLL_UP;
                        g_MenuMateriaCursorParam4 = 0xFFFFFFF9;
                        g_MenuMateriaExchangeScrollTop--;
                        g_MenuMateriaStatDelta9 = EQUIP_ARMOR;
                    }
                } else if (MenuCursor_ReadRepeat(INPUT_LEFT, MOUSE_DIR_LEFT) != 0 && previousSlotCursor == g_MenuMateriaExchangeSlotCursor && g_MenuMateriaCursorParam6 == 0) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    g_MenuMateriaSortOrder = g_MenuMateriaStatDelta9 + 1;
                    g_MenuMateriaCursorParam1 = MENU_MATERIA_EXCHANGE_PANE_ROWS;
                }
                break;
            case MENU_MATERIA_EXCHANGE_PANE_STOCK:
                MenuCursor_Update((MenuCursor *)&g_MenuMateriaStatDelta3);
                if (MenuCursor_ReadPressed(INPUT_LEFT, MOUSE_DIR_LEFT) != 0 && g_MenuMateriaStatDelta7 == 0) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    g_MenuMateriaCursorParam1 = MENU_MATERIA_EXCHANGE_PANE_SLOTS;
                }
                break;
            case MENU_MATERIA_EXCHANGE_PANE_ROWS:
                previousSlotCursor = g_MenuMateriaSortMode;
                oldDelta = g_MenuMateriaSortOrder;
                MenuCursor_Update((MenuCursor *)&g_MenuMateriaSortMode);
                if (MenuCursor_ReadPressed(INPUT_RIGHT, MOUSE_DIR_RIGHT) != 0 && g_MenuMateriaSortOrder != 0 && g_MenuMateriaCursorParam6 == 0) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    g_MenuMateriaCursorParam1 = MENU_MATERIA_EXCHANGE_PANE_SLOTS;
                    g_MenuMateriaStatDelta9 = g_MenuMateriaSortOrder - 1;
                } else if (MenuCursor_ReadRepeat(INPUT_DOWN, MOUSE_DIR_DOWN) != 0 && oldDelta == g_MenuMateriaSortOrder) {
                    if (g_MenuMateriaExchangeRow < bottomRow) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        g_MenuMateriaExchangeRow++;
                        g_MenuMateriaSortOrder = MENU_MATERIA_EXCHANGE_ROW_ALL;
                    } else if (g_MenuMateriaExchangeRow == 3 && g_MenuMateriaExchangeScrollTop < maxScrollTop) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        g_MenuMateriaCursorParam5 = MENU_MATERIA_EXCHANGE_SCROLL_DOWN;
                        g_MenuMateriaCursorParam4 = 0xFFFFFFFF;
                        g_MenuMateriaSortOrder = MENU_MATERIA_EXCHANGE_ROW_ALL;
                    }
                } else if (MenuCursor_ReadRepeat(INPUT_UP, MOUSE_DIR_UP) != 0 && oldDelta == g_MenuMateriaSortOrder) {
                    if (g_MenuMateriaExchangeRow != 0) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        g_MenuMateriaExchangeRow--;
                        g_MenuMateriaSortOrder = MENU_MATERIA_EXCHANGE_ROW_ARMOR;
                    } else if (g_MenuMateriaExchangeScrollTop > 0) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        g_MenuMateriaCursorParam5 = MENU_MATERIA_EXCHANGE_SCROLL_UP;
                        g_MenuMateriaCursorParam4 = 0xFFFFFFF9;
                        g_MenuMateriaExchangeScrollTop--;
                        g_MenuMateriaSortOrder = MENU_MATERIA_EXCHANGE_ROW_ARMOR;
                    }
                }
            }
        }
    } else {
        switch (g_MenuMateriaCursorParam5) {
        case MENU_MATERIA_EXCHANGE_SCROLL_UP:
            g_MenuMateriaCursorParam4++;
            if (g_MenuMateriaCursorParam4 == 0) {
                g_MenuMateriaCursorParam4 = 0;
                g_MenuMateriaCursorParam5 = g_MenuMateriaCursorParam4;
            }
            break;
        case MENU_MATERIA_EXCHANGE_SCROLL_DOWN:
            g_MenuMateriaCursorParam4--;
            if (g_MenuMateriaCursorParam4 == -8) {
                g_MenuMateriaExchangeScrollTop++;
                g_MenuMateriaCursorParam4 = 0;
                g_MenuMateriaCursorParam5 = g_MenuMateriaCursorParam4;
            }
        }
    }
}

/*
 * Swaps equipped materia between two characters when both slots exist.
 */
// FUNCTION: FF7 0x00710bd0
void __cdecl Menu_SwapEquippedMateriaWhereBothSlotsExist(unsigned char *liveA, unsigned char *liveB,
                                                         int *slotsA, int *slotsB)
{
    int tmp;
    int i;

    for (i = 0; i < 8; i++) {
        if (liveA[i] != 0 && liveB[i] != 0) {
            tmp = slotsA[i];
            slotsA[i] = slotsB[i];
            slotsB[i] = tmp;
        }
    }
}

/*
 * Resets materia screen initialization state flag.
 */
// FUNCTION: FF7 0x00710c3c
void __cdecl Menu_ClearMateriaScreenInitializedFlag(void)
{
    g_MenuField = 0;
}
} /* extern "C" */
