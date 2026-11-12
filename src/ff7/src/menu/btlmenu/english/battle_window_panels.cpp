// src/ff7/src/menu/btlmenu/english/battle_window_panels.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Draws the battle menu: its windows, selection and target cursors, message and help
//   windows, and the command panels, at both 1x and 2x resolutions. It also posts the chosen
//   commands and draws the enemy action list and the controlled-enemy text.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/menu.h"
#include "lib/codegen.h"
#include "lib/types/graphics.h"
#include "ff7/enums/main.h"
#include "ff7/enums/menu.h"
#include "ff7/enums/battle.h"
#include "lib/enums/file.h"
#include "lib/enums/graphics.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "lib/enums/sound.h"
#include <string.h>
#include "ff7/battle.h"
#include <stdlib.h>
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/file.h"
#include "lib/api/graphics.h"
#include "lib/api/token.h"
#include "ff7/api/main.h"
#include "lib/api/sound.h"
#include "ff7/savemap.h"
extern "C" {

struct GfxEngineContext;
struct EnglishWindowFrame;


#pragma intrinsic(abs, memcpy, memset)
// clang-format off
// The object's first bytes, 8-aligned after callback.cpp's literal "btl_win_l.tim".
unsigned char g_BtlMenuWindowTemplatePool[1200] = {
    0x28, 0x00, 0x00, 0x00, 0x00, 0xe8, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x08, 0xe8, 0x18, 0x01, 0x00, 0x00, 0x00, 0x00, 0x10, 0xe0, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x18, 0xe0, 0x18, 0x01,
    0x10, 0x00, 0x02, 0x00, 0x00, 0xe0, 0x08, 0x00, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0x18, 0x00, 0x00, 0x00, 0x10, 0x00,
    0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0x28, 0x00, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0x38, 0x00, 0x00, 0x00, 0x10, 0x00,
    0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0x48, 0x00, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0x58, 0x00, 0x00, 0x00, 0x10, 0x00,
    0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0x68, 0x00, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0x78, 0x00, 0x00, 0x00, 0x10, 0x00,
    0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0x88, 0x00, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0x98, 0x00, 0x00, 0x00, 0x10, 0x00,
    0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0xa8, 0x00, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0xb8, 0x00, 0x00, 0x00, 0x10, 0x00,
    0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0xc8, 0x00, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0xd8, 0x00, 0x00, 0x00, 0x10, 0x00,
    0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0xe8, 0x00, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0xf8, 0x00, 0x00, 0x00, 0x10, 0x00,
    0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0x08, 0x01, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0x08, 0x00, 0x10, 0x00, 0x10, 0x00,
    0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0x18, 0x00, 0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0x28, 0x00, 0x10, 0x00, 0x10, 0x00,
    0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0x38, 0x00, 0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0x48, 0x00, 0x10, 0x00, 0x10, 0x00,
    0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0x58, 0x00, 0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0x68, 0x00, 0x10, 0x00, 0x10, 0x00,
    0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0x78, 0x00, 0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0x88, 0x00, 0x10, 0x00, 0x10, 0x00,
    0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0x98, 0x00, 0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0xa8, 0x00, 0x10, 0x00, 0x10, 0x00,
    0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0xb8, 0x00, 0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0xc8, 0x00, 0x10, 0x00, 0x10, 0x00,
    0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0xd8, 0x00, 0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0xe8, 0x00, 0x10, 0x00, 0x10, 0x00,
    0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0xf8, 0x00, 0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0x08, 0x01, 0x10, 0x00, 0x10, 0x00,
    0x08, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x18, 0xf0, 0x18, 0x01, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x14, 0x00, 0x00, 0x00, 0x00, 0xe8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xe8, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0xe0, 0x00, 0x00,
    0x10, 0x00, 0x00, 0x00, 0x18, 0xe0, 0x78, 0x00, 0x10, 0x00, 0x02, 0x00, 0x00, 0xe0, 0x08, 0x00, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00,
    0x00, 0xe0, 0x18, 0x00, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0x28, 0x00, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00,
    0x00, 0xe0, 0x38, 0x00, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0x48, 0x00, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00,
    0x00, 0xe0, 0x58, 0x00, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0x68, 0x00, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00,
    0x10, 0xe8, 0x08, 0x00, 0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0x18, 0x00, 0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00,
    0x10, 0xe8, 0x28, 0x00, 0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0x38, 0x00, 0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00,
    0x10, 0xe8, 0x48, 0x00, 0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0x58, 0x00, 0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00,
    0x10, 0xe8, 0x68, 0x00, 0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x18, 0xf8, 0x78, 0x00,
    0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x00, 0xe8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xe8, 0x38, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x10, 0xe0, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x18, 0xe0, 0x38, 0x01, 0x10, 0x00, 0x02, 0x00, 0x00, 0xe0, 0x08, 0x00,
    0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0x18, 0x00, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0x28, 0x00,
    0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0x38, 0x00, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0x48, 0x00,
    0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0x58, 0x00, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0x68, 0x00,
    0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0x78, 0x00, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0x88, 0x00,
    0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0x98, 0x00, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0xa8, 0x00,
    0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0xb8, 0x00, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0xc8, 0x00,
    0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0xd8, 0x00, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0xe8, 0x00,
    0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0xf8, 0x00, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0x08, 0x01,
    0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0x08, 0x00, 0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0x18, 0x00,
    0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0x28, 0x00, 0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0x38, 0x00,
    0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0x48, 0x00, 0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0x58, 0x00,
    0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0x68, 0x00, 0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0x78, 0x00,
    0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0x88, 0x00, 0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0x98, 0x00,
    0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0xa8, 0x00, 0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0xb8, 0x00,
    0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0xc8, 0x00, 0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0xd8, 0x00,
    0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0xe8, 0x00, 0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0xf8, 0x00,
    0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0x08, 0x01, 0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00,
    0x08, 0x00, 0x00, 0x00, 0x18, 0xf0, 0x38, 0x01, 0x08, 0x00, 0x02, 0x00, 0x00, 0xe0, 0x18, 0x01, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00,
    0x00, 0xe0, 0x28, 0x01, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00, 0x10, 0xe8, 0x18, 0x01, 0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0x00,
    0x10, 0xe8, 0x28, 0x01, 0x10, 0x00, 0x10, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// clang-format on
// The battle's windows at 2x scale, the table Battle_SelectMenuUiTablesForScalingMode points
// g_BattleMenuData at when GetUiScalingMode() is 2: per box its rectangle, a sub-rectangle
// count and the sub-rectangles, and the unused pointer (BattleUiBox.unused94). The 1x table after it is 1,392 bytes,
// nine records and 24 more, so it is not one record array and stays as ints.
// clang-format off
// GLOBAL: FF7 0x0091c340
BattleUiBox g_BattleWindowTable2x[29] = {
    {0, 0x14c, 0x280, 0x70, 0, 0, 0, 0, 2,
     {{0, 0, 0x112, 0x70}, {0x112, 0, 0x280, 0x70}},
     {0}, (int)g_BtlMenuWindowDataBuffer},
    {0x90, 0x154, 0x6e, 0x70, 0, 0, 0, 0, 1,
     {{0, 0, 0x70, 0x70}},
     {0}, (int)(g_BtlMenuWindowDataBuffer + 0x4)},
    {0x30, 0x154, 0x70, 0x30, 0, 0, 0, 0, 1,
     {{0, 0, 0x70, 0x30}},
     {0}, (int)(g_BtlMenuWindowDataBuffer + 0x14)},
    {0x30, 0x154, 0x70, 0x30, 0, 0, 0, 0, 1,
     {{0, 0, 0x70, 0x30}},
     {0}, (int)(g_BtlMenuWindowDataBuffer + 0x14)},
    {0, 0x15c, 0x280, 0x70, 0, 0, 0, 0, 2,
     {{0, 0, 0x1f6, 0x70}, {0x1f6, 0, 0x280, 0x70}},
     {0}, (int)(g_BtlMenuWindowDataBuffer + 0x1c)},
    {0, 0x15c, 0x280, 0x70, 0, 0, 0, 0, 1,
     {{0, 0, 0x280, 0x70}},
     {0}, (int)(g_BtlMenuWindowDataBuffer + 0x18)},
    {0, 0x15c, 0x280, 0x70, 0, 0, 0, 0, 2,
     {{0, 0, 0x1f6, 0x70}, {0x1f6, 0, 0x280, 0x70}},
     {0}, (int)(g_BtlMenuWindowDataBuffer + 0x1c)},
    {0, 0x15c, 0x280, 0x70, 0, 0, 0, 0, 2,
     {{0, 0, 0x1f6, 0x70}, {0x1f6, 0, 0x280, 0x70}},
     {0}, (int)(g_BtlMenuWindowDataBuffer + 0x1c)},
    {0, 0x14c, 0x280, 0x78, 0, 0, 0, 0, 0,
     {{0}},
     {0}, (int)g_BtlMenuWindowDataBuffer},
    {0, 0x15c, 0x280, 0x78, 0, 0, 0, 0, 1,
     {{0, 0, 0x280, 0x78}},
     {0}, (int)g_BtlMenuWindowDataBuffer},
    {0, 0x14c, 0x280, 0x78, 0, 0, 0, 0, 0,
     {{0}},
     {0}, (int)g_BtlMenuWindowDataBuffer},
    {0, 0, 0, 0, 0, 0, 0, 0, 0,
     {{0}},
     {0}, (int)g_BtlMenuWindowDataBuffer},
    {0, 0, 0, 0, 0, 0, 0, 0, 0,
     {{0}},
     {0}, (int)g_BtlMenuWindowDataBuffer},
    {0, 0, 0, 0, 0, 0, 0, 0, 0,
     {{0}},
     {0}, (int)g_BtlMenuWindowDataBuffer},
    {0, 0, 0, 0, 0, 0, 0, 0, 0,
     {{0}},
     {0}, (int)g_BtlMenuWindowDataBuffer},
    {0, 0, 0, 0, 0, 0, 0, 0, 0,
     {{0}},
     {0}, (int)g_BtlMenuWindowDataBuffer},
    {0, 0, 0, 0, 0, 0, 0, 0, 0,
     {{0}},
     {0}, (int)g_BtlMenuWindowDataBuffer},
    {0, 0, 0, 0, 0, 0, 0, 0, 0,
     {{0}},
     {0}, (int)g_BtlMenuWindowDataBuffer},
    {0, 0, 0, 0, 0, 0, 0, 0, 0,
     {{0}},
     {0}, (int)g_BtlMenuWindowDataBuffer},
    {0x64, 0x154, 0x1b8, 0x70, 0, 0, 0, 0, 1,
     {{0, 0, 0xf0, 0x70}},
     {0}, (int)(g_BtlMenuWindowDataBuffer + 0x3c)},
    {0x64, 0x15c, 0x148, 0x74, 0, 0, 0, 0, 1,
     {{0, 0, 0x148, 0x74}},
     {0}, (int)(g_BtlMenuWindowDataBuffer + 0x30)},
    {0xa0, 0x164, 0x17c, 0x70, 0, 0, 0x20, 0x70, 1,
     {{0, 0, 0x17c, 0x70}},
     {0}, (int)(g_BtlMenuWindowDataBuffer + 0x34)},
    {0xc0, 0x10, 0x100, 0x30, 0, 0, 0, 0, 1,
     {{0, 0, 0x100, 0x30}},
     {0}, (int)(g_BtlMenuWindowTemplatePool + 0x1d0)},
    {0, 0x11c, 0x280, 0x30, 0, 0, 0, 0, 1,
     {{0, 0, 0x280, 0x30}},
     {0}, (int)(g_BtlMenuWindowTemplatePool + 0x2b0)},
    {0xa0, 0x15c, 0x110, 0x70, 0, 0, 0, 0, 1,
     {{0, 0, 0x110, 0x70}},
     {0}, (int)(g_BtlMenuWindowDataBuffer + 0x28)},
    {0x20, 0x10, 0x240, 0x30, 0, 0, 0, 0, 1,
     {{0, 0, 0x240, 0x30}},
     {0}, (int)g_BtlMenuWindowTemplatePool},
    {0, 0x15c, 0x280, 0x70, 0, 0, 0, 0, 1,
     {{0, 0, 0x280, 0x70}},
     {0}, (int)g_BtlMenuWindowTemplatePool},
    {0, 0x15c, 0x280, 0x70, 0, 0, 0, 0, 1,
     {{0, 0, 0x280, 0x70}},
     {0}, (int)g_BtlMenuWindowTemplatePool},
    {0, 0x15c, 0x280, 0x70, 0, 0, 0, 0, 1,
     {{0, 0, 0x280, 0x70}},
     {0}, (int)g_BtlMenuWindowTemplatePool}
};
// GLOBAL: FF7 0x0091d478
int g_BattleWindowTable1x[348] = {
    0xa60000, 0x380140, 0x0, 0x0, 0x2, 0x890000,
    0x890038, 0x1400000, 0x38, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, (int)(g_BtlMenuWindowDataBuffer + 0x0), 0xaa0048, 0x380037, 0x0, 0x0,
    0x1, 0x380000, 0x38, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, (int)(g_BtlMenuWindowDataBuffer + 0x4), 0xaa0018, 0x180038,
    0x0, 0x0, 0x1, 0x380000, 0x18, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, (int)(g_BtlMenuWindowDataBuffer + 0x14),
    0xaa0018, 0x180038, 0x0, 0x0, 0x1, 0x380000,
    0x18, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, (int)(g_BtlMenuWindowDataBuffer + 0x14), 0xae0000, 0x380140, 0x0, 0x0,
    0x2, 0xfb0000, 0xfb0038, 0x1400000, 0x38, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, (int)(g_BtlMenuWindowDataBuffer + 0x1c), 0xae0000, 0x380140,
    0x0, 0x0, 0x1, 0x1400000, 0x38, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, (int)(g_BtlMenuWindowDataBuffer + 0x18),
    0xae0000, 0x380140, 0x0, 0x0, 0x2, 0xfb0000,
    0xfb0038, 0x1400000, 0x38, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, (int)(g_BtlMenuWindowDataBuffer + 0x1c), 0xae0000, 0x380140, 0x0, 0x0,
    0x2, 0xfb0000, 0xfb0038, 0x1400000, 0x38, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, (int)(g_BtlMenuWindowDataBuffer + 0x1c), 0xa60000, 0x3c0140,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, (int)(g_BtlMenuWindowDataBuffer + 0x0),
    0xae0000, 0x3c0140, 0x0, 0x0, 0x1, 0x1400000
};
int g_BtlMenuPanelDispatchData[754] = {
    0x3c, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, (int)(g_BtlMenuWindowDataBuffer + 0x0), 0xa60000, 0x3c0140, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, (int)(g_BtlMenuWindowDataBuffer + 0x0), 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, (int)(g_BtlMenuWindowDataBuffer + 0x0),
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, (int)(g_BtlMenuWindowDataBuffer + 0x0), 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, (int)(g_BtlMenuWindowDataBuffer + 0x0), 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, (int)(g_BtlMenuWindowDataBuffer + 0x0),
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, (int)(g_BtlMenuWindowDataBuffer + 0x0), 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, (int)(g_BtlMenuWindowDataBuffer + 0x0), 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, (int)(g_BtlMenuWindowDataBuffer + 0x0),
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, (int)(g_BtlMenuWindowDataBuffer + 0x0), 0xaa0028, 0x3800dc, 0x0, 0x0,
    0x1, 0x780000, 0x38, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, (int)(g_BtlMenuWindowDataBuffer + 0x3c), 0xae0032, 0x3a00a4,
    0x0, 0x0, 0x1, 0xa40000, 0x3a, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, (int)(g_BtlMenuWindowDataBuffer + 0x30),
    0xb20050, 0x3800be, 0x0, 0x380010, 0x1, 0xbe0000,
    0x38, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, (int)(g_BtlMenuWindowDataBuffer + 0x34), 0x80060, 0x180080, 0x0, 0x0,
    0x1, 0x800000, 0x18, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, (int)(g_BtlMenuWindowTemplatePool + 0x1d0), 0x8e0000, 0x180140,
    0x0, 0x0, 0x1, 0x1400000, 0x18, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, (int)(g_BtlMenuWindowTemplatePool + 0x2b0),
    0xae0050, 0x380088, 0x0, 0x0, 0x1, 0x880000,
    0x38, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, (int)(g_BtlMenuWindowDataBuffer + 0x28), 0x80010, 0x180120, 0x0, 0x0,
    0x1, 0x1200000, 0x18, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, (int)(g_BtlMenuWindowTemplatePool + 0x0), 0xae0000, 0x380140,
    0x0, 0x0, 0x1, 0x1400000, 0x38, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, (int)(g_BtlMenuWindowTemplatePool + 0x0),
    0xae0000, 0x380140, 0x0, 0x0, 0x1, 0x1400000,
    0x38, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, (int)(g_BtlMenuWindowTemplatePool + 0x0), 0xae0000, 0x380140, 0x0, 0x0,
    0x1, 0x1400000, 0x38, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, (int)(g_BtlMenuWindowTemplatePool + 0x0)
};
// GLOBAL: FF7 0x0091e5b0
float g_BtlMenuWindowSubFrameDepth[34] = {0.6f, 0.5f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.0f, 0.4f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.4f, 0.3f, 0.0f, 0.0f, 0.4f, 0.0f, 0.4f, 0.4f, 0.4f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
// The battle windows' handlers, one per window id (0x20): Battle_OpenWindow calls
// g_BtlMenuWindowOpenHandlers[id]() when a window opens, the window frame calls
// g_BtlMenuWindowActiveHandlers[id]() while it is open, and no code reads
// g_BtlMenuWindowUnusedHandlers, whose functions (each one just before the window's open
// handler) only return.
typedef void (*BattleUiHandlerFn)(void);
// GLOBAL: FF7 0x0091e638
BattleUiHandlerFn g_BtlMenuWindowOpenHandlers[32] = {
    Battle_WindowOpenSlot0NoOp, Battle_WindowOpenCommandMenuResizeResetCursor, Battle_WindowOpenSlot2NoOp, Battle_WindowOpenSlot3NoOp,
    Battle_WindowOpenEnemySkillListResetCursor, Battle_WindowOpenItemPanelResetCursor, Battle_WindowOpenMagicPanelResetCursor, Battle_WindowOpenSummonListResetCursor,
    Battle_WindowOpenNoOp, Battle_WindowOpenWorriedChoice, Battle_WindowOpenNoOp, Battle_WindowOpenSeedGilAmount,
    Battle_WindowOpenSlot12And21NoOp, Battle_WindowOpenNoOp, Battle_WindowOpenNoOp, Battle_WindowOpenLimitListResetCursor,
    Battle_WindowOpenNoOp, Battle_WindowOpenNoOp, Battle_WindowOpenNoOp, Battle_ResetCommandMenuCursors,
    Battle_WindowOpenSeedGilAmount, Battle_WindowOpenSlot12And21NoOp, Battle_WindowOpenNoOp, Battle_WindowOpenNoOp,
    Battle_WindowOpenLimitListResetCursor, Battle_WindowOpenNoOp, Battle_WindowOpenCaitReelWindow, Battle_WindowOpenTifaReelWindow,
    Battle_WindowOpenSlotScreen, Battle_WindowOpenNoOp, Battle_WindowOpenNoOp, Battle_WindowOpenNoOp
};
// GLOBAL: FF7 0x0091e6b8
BattleUiHandlerFn g_BtlMenuWindowActiveHandlers[32] = {
    Battle_WindowFrameSlot0OpenCommandMenuForActor, Battle_WindowFrameCommandMenu, Battle_WindowFrameSlot2ConfirmOrCancel, Battle_WindowFrameSlot3ConfirmOrCancel,
    Battle_WindowFrameEnemySkillList, Battle_WindowFrameItemPanel, Battle_WindowFrameMagicList, Battle_WindowFrameSummonList,
    Battle_WindowFrameNoOp, Battle_WindowFrameWorriedChoice, Battle_WindowFrameNoOp, Battle_WindowFrameGilAmountPrompt,
    Battle_WindowFrameSlot12And21NoOp, Battle_WindowFrameNoOp, Battle_WindowFrameNoOp, Battle_WindowFrameLimitList,
    Battle_WindowFrameNoOp, Battle_WindowFrameNoOp, Battle_WindowFrameNoOp, Battle_WindowFrameControlledEnemyActionList,
    Battle_WindowFrameGilAmountPrompt, Battle_WindowFrameSlot12And21NoOp, Battle_WindowFrameNoOp, Battle_WindowFrameNoOp,
    Battle_WindowFrameLimitList, Battle_WindowFrameNoOp, Battle_WindowFrameCaitReelWindow, Battle_WindowFrameTifaReelWindow,
    Battle_WindowFrameSlotScreen, Battle_WindowFrameNoOp, Battle_WindowFrameNoOp, Battle_WindowFrameNoOp
};
// GLOBAL: FF7 0x0091e738
BattleUiHandlerFn g_BtlMenuWindowUnusedHandlers[32] = {
    Battle_WindowUnusedSlot0NoOp, Battle_WindowUnusedSlot1NoOp, Battle_WindowUnusedSlot2NoOp, Battle_WindowUnusedSlot3NoOp,
    Battle_WindowUnusedSlot4NoOp, Battle_WindowUnusedSlot5NoOp, Battle_WindowUnusedSlot6NoOp, Battle_WindowUnusedSlot7NoOp,
    Battle_WindowUnusedNoOp, Battle_WindowUnusedNoOp, Battle_WindowUnusedNoOp, Battle_WindowUnusedSlot11And20NoOp,
    Battle_WindowUnusedSlot12And21NoOp, Battle_WindowUnusedNoOp, Battle_WindowUnusedNoOp, Battle_WindowUnusedSlot15And24NoOp,
    Battle_WindowUnusedNoOp, Battle_WindowUnusedNoOp, Battle_WindowUnusedNoOp, Battle_WindowUnusedSlot19NoOp,
    Battle_WindowUnusedSlot11And20NoOp, Battle_WindowUnusedSlot12And21NoOp, Battle_WindowUnusedNoOp, Battle_WindowUnusedNoOp,
    Battle_WindowUnusedSlot15And24NoOp, Battle_WindowUnusedNoOp, Battle_WindowUnusedSlot26NoOp, Battle_WindowUnusedSlot27NoOp,
    Battle_WindowUnusedSlot28NoOp, Battle_WindowUnusedNoOp, Battle_WindowUnusedNoOp, Battle_WindowUnusedNoOp
};
// clang-format on
// GLOBAL: FF7 0x0091e7b8
int g_BattleSelectionCursorState = (-1);
// GLOBAL: FF7 0x0091e7c0
unsigned short g_BtlMenuGilDigitDivisors[5] = {10000, 1000, 100, 10, 1};
// clang-format off
unsigned char g_rawData_0091e7d0[8] = {0x00, 0x01, 0x02, 0x03, 0x05, 0xff, 0x00, 0x00};
// GLOBAL: FF7 0x0091e7d8
unsigned char g_BtlMenuEmptyText[8] = {0xff, 0, 0, 0, 0, 0, 0, 0};
// GLOBAL: FF7 0x0091e7e0
int g_BtlMenuWindowColors[8] = {
    0x53554130, 0xff45, (int)(g_BtlMenuWindowDataBuffer + 0x4), (int)(g_BtlMenuWindowDataBuffer + 0x8), (int)(g_BtlMenuWindowDataBuffer + 0xc), (int)(g_BtlMenuWindowDataBuffer + 0x10),
    0xda0078, 0x18a013a
};
// GLOBAL: FF7 0x0091e800
short g_BtlMenuCommandWidths1x[2] = {0x3c, 0x6d};
unsigned char g_rawData_0091e804[4] = {0x9d, 0x00, 0xc5, 0x00};
// clang-format on
// The two four-byte height tables Battle_SelectMenuUiTablesForScalingMode picks (scaling mode 2:
// g_BtlMenuLimitListHeights2x, otherwise g_BtlMenuLimitListHeights1x), as it picks the scaling tables.
// GLOBAL: FF7 0x0091e808
unsigned char g_BtlMenuLimitListHeights2x[4] = {0x38, 0x58, 0x78, 0x00};
// GLOBAL: FF7 0x0091e80c
unsigned char g_BtlMenuLimitListHeights1x[4] = {0x1c, 0x2c, 0x3c, 0x00};
// GLOBAL: FF7 0x0091e810
int g_BattleCountdownDivisors[4] = {600, 60, 10, 1};
// clang-format off
unsigned char g_rawData_0091e820[8] = {0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00};
// clang-format on
// GLOBAL: FF7 0x00dc1f58
unsigned char g_BtlMenuSelectionSingleTarget = 0;
unsigned char g_bssPad_00dc1f5c[4] = {0};
unsigned char g_bssPad_00dc1f60[264] = {0};
// GLOBAL: FF7 0x00dc2068
unsigned char g_BattleWindowStates[32] = {0};
// GLOBAL: FF7 0x00dc2088
short g_BattleSelectedUseCondition = 0;
// GLOBAL: FF7 0x00dc208c
unsigned char *g_BattleTextBuffer = 0;
// GLOBAL: FF7 0x00dc2090
short *g_BattleScalingTable = 0;
// GLOBAL: FF7 0x00dc2094
int g_BtlMenuCursorX = 0;
// GLOBAL: FF7 0x00dc2098
int g_BtlMenuCursorY = 0;
// GLOBAL: FF7 0x00dc209c
unsigned char g_BtlMenuSlotScreenState = BATTLE_SLOT_SCREEN_CHOOSE;
// GLOBAL: FF7 0x00dc20a0
unsigned char g_BtlMenuWindowSets[4] = {0};
// GLOBAL: FF7 0x00dc20a4
unsigned char g_BtlMenuSlotRecords[4] = {0};
unsigned char g_bssPad_00dc20a8[48] = {0};
// The battle menu keeps one 0x700-byte record per party actor; the definitions below are the first
// actor's fields, and actor n's copy of a field is n * 0x700 bytes on. Its cursor blocks are
// MenuCursor blocks of fourteen dwords. This one is the item list's: the item menu reads it as
// `(MenuCursor *)&g_BtlMenuSlotTableA[actor * 0x700]` and the hand cursor reads its row and scrollTop.
// GLOBAL: FF7 0x00dc20d8
unsigned char g_BtlMenuSlotTableA[56] = {0};
// GLOBAL: FF7 0x00dc2110
unsigned char g_BtlMenuSlotTableB[4] = {0};
// GLOBAL: FF7 0x00dc2114
unsigned char g_BtlMenuSlotTableC[4] = {0};
unsigned char g_bssPad_00dc2118[12] = {0};
// GLOBAL: FF7 0x00dc2124
unsigned char g_BtlMenuWindowRecordsB[4] = {0};
unsigned char g_bssPad_00dc2128[32] = {0};
// GLOBAL: FF7 0x00dc2148
unsigned char g_BtlMenuWindowRecordsC[4] = {0};
// GLOBAL: FF7 0x00dc214c
unsigned char g_BtlMenuSlotTableD[4] = {0};
unsigned char g_bssPad_00dc2150[12] = {0};
// GLOBAL: FF7 0x00dc215c
unsigned char g_BtlMenuWindowRecordsD[4] = {0};
unsigned char g_bssPad_00dc2160[32] = {0};
// GLOBAL: FF7 0x00dc2180
unsigned char g_BtlMenuWindows[4] = {0};
// GLOBAL: FF7 0x00dc2184
unsigned char g_BtlMenuCursors[4] = {0};
// GLOBAL: FF7 0x00dc2188
unsigned char g_BtlMenuEnemySkillColCount[4] = {0};
// GLOBAL: FF7 0x00dc218c
unsigned char g_BtlMenuEnemySkillRowCount[4] = {0};
// GLOBAL: FF7 0x00dc2190
unsigned char g_BtlMenuEnemySkillUnused4[4] = {0};
// GLOBAL: FF7 0x00dc2194
unsigned char g_BtlMenuWindowRecords[4] = {0};
// GLOBAL: FF7 0x00dc2198
unsigned char g_BtlMenuEnemySkillRowStep[4] = {0};
// GLOBAL: FF7 0x00dc219c
unsigned char g_BtlMenuEnemySkillTotalRows[4] = {0};
// GLOBAL: FF7 0x00dc21a0
unsigned char g_BtlMenuEnemySkillUnused8[4] = {0};
// GLOBAL: FF7 0x00dc21a4
unsigned char g_BtlMenuEnemySkillScrollAnim[4] = {0};
// GLOBAL: FF7 0x00dc21a8
unsigned char g_BtlMenuEnemySkillColWrapMode[4] = {0};
// GLOBAL: FF7 0x00dc21ac
unsigned char g_BtlMenuEnemySkillRowWrapMode[4] = {0};
// GLOBAL: FF7 0x00dc21b0
unsigned char g_BtlMenuEnemySkillScrollState[4] = {0};
// GLOBAL: FF7 0x00dc21b4
unsigned char g_BtlMenuEnemySkillPageScroll[4] = {0};
// Two more MenuCursor blocks of the per-actor record: the limit list's, which the limit sub-menu hands to
// MenuCursor_Update as `&g_BtlMenuLimitCursor[actor * 0x700]`, and the gil prompt's, whose row places the
// hand cursor.
// GLOBAL: FF7 0x00dc21b8
unsigned char g_BtlMenuLimitCursor[56] = {0};
// GLOBAL: FF7 0x00dc21f0
int g_BtlMenuGilCursor[14] = {0};
// GLOBAL: FF7 0x00dc2228
MenuCursor g_BtlMenuSlotScreenCursor = {0};
// GLOBAL: FF7 0x00dc2260
MenuCursor g_BtlMenuWorriedChoiceCursor = {0};
unsigned char g_bssPad_00dc2298[1232] = {0};
// GLOBAL: FF7 0x00dc2768
int g_BtlMenuControlledActionCursor = 0;
// GLOBAL: FF7 0x00dc276c
int g_BtlMenuControlledActionRow = 0;
// GLOBAL: FF7 0x00dc2770
int g_BtlMenuControlledActionColCount = 0;
// GLOBAL: FF7 0x00dc2774
int g_BtlMenuControlledActionRowCount = 0;
// GLOBAL: FF7 0x00dc2778
int g_BtlMenuControlledActionUnused4 = 0;
// GLOBAL: FF7 0x00dc277c
int g_BtlMenuControlledActionScrollTop = 0;
// GLOBAL: FF7 0x00dc2780
int g_BtlMenuControlledActionRowStep = 0;
// GLOBAL: FF7 0x00dc2784
int g_BtlMenuControlledActionTotalRows = 0;
// GLOBAL: FF7 0x00dc2788
int g_BtlMenuControlledActionUnused8 = 0;
// GLOBAL: FF7 0x00dc278c
int g_BtlMenuControlledActionScrollAnim = 0;
// GLOBAL: FF7 0x00dc2790
int g_BtlMenuControlledActionColWrapMode = 0;
// GLOBAL: FF7 0x00dc2794
int g_BtlMenuControlledActionRowWrapMode = 0;
// GLOBAL: FF7 0x00dc2798
int g_BtlMenuControlledActionScrollState = 0;
// GLOBAL: FF7 0x00dc279c
int g_BtlMenuControlledActionPageScroll = 0;
unsigned char g_bssPad_00dc27a0[3588] = {0};
// GLOBAL: FF7 0x00dc35a4
short g_BtlMenuLimitRowCount = 0;
// GLOBAL: FF7 0x00dc35a8
unsigned char g_BtlMenuControlledActionStarted = 0;
// GLOBAL: FF7 0x00dc35ac
int g_BtlMenuSelectionConfirmed = 0;
// GLOBAL: FF7 0x00dc35b0
short g_BtlMenuSelectionTargetMask = 0;
// GLOBAL: FF7 0x00dc35b4
int *g_pBtlMenuActiveWindow = 0;
unsigned char g_bssPad_00dc35b8[4] = {0};
// GLOBAL: FF7 0x00dc35bc
unsigned char g_BtlMenuSelectionTargetGroup = 0;
// GLOBAL: FF7 0x00dc35c0
unsigned char g_BtlMenuSelectionTargetMode = 0;
unsigned char g_bssPad_00dc35c4[4] = {0};
unsigned char g_bssPad_00dc35c8[104] = {0};
// GLOBAL: FF7 0x00dc3630
BattleUiBox *g_BattleMenuData = 0;
// GLOBAL: FF7 0x00dc3634
short g_BtlMenuResetShortA = 0;
// GLOBAL: FF7 0x00dc3638
short g_BtlMenuSelectionIndex = 0;
// GLOBAL: FF7 0x00dc363c
unsigned int g_BtlMenuGilAmount = 0;
// GLOBAL: FF7 0x00dc3640
unsigned char g_BtlMenuGilAmountText[16] = {0};
// GLOBAL: FF7 0x00dc3650
unsigned char g_BtlMenuSlotSeed = 0;
// GLOBAL: FF7 0x00dc3654
unsigned char g_BtlMenuDrawWindowIndex = 0;
unsigned char g_bssPad_00dc3658[128] = {0};
// GLOBAL: FF7 0x00dc36d8
short g_BtlMenuTargetCursorState = 0;
unsigned char g_bssPad_00dc36dc[4] = {0};
unsigned char g_bssPad_00dc36e0[384] = {0};
// GLOBAL: FF7 0x00dc3860
unsigned char g_BtlMenuSlotIndex = 0;
// GLOBAL: FF7 0x00dc3864
unsigned char g_BtlMenuResetByteA = 0;
// GLOBAL: FF7 0x00dc3868
short g_BtlMenuLimitCursorHidden = 0;
// GLOBAL: FF7 0x00dc386c
unsigned char *g_pBtlMenuScalingTable = 0;
unsigned char g_BtlMenuWindowDataBuffer[64] = {0};
// GLOBAL: FF7 0x00dc38b0
unsigned char g_BtlMenuResetByteB = 0;
// GLOBAL: FF7 0x00dc38b4
unsigned char g_BtlMenuResetByteC = 0;
// GLOBAL: FF7 0x00dc38b8
unsigned char g_BtlMenuResetByteD = 0;
// GLOBAL: FF7 0x00dc38bc
unsigned char g_BtlMenuMessageWindowRequest = 0;
// GLOBAL: FF7 0x00dc38c0
short g_BtlMenuMessageWindowTextId = 0;
// GLOBAL: FF7 0x00dc38c4
int *g_pBtlMenuUiTextures = 0;
// GLOBAL: FF7 0x00dc38c8
int g_BtlMenuWorriedChoiceOpen = 0;
// GLOBAL: FF7 0x00dc38cc
unsigned char g_BtlMenuSelectedFlags = 0;
// GLOBAL: FF7 0x00dc38d0
short g_BtlPointedTargetInvalid = 0;
// GLOBAL: FF7 0x00dc38d4
short g_BtlMenuPanelRefreshFlag = 0;
// GLOBAL: FF7 0x00dc38d8
int g_BtlMenuResetWordA = 0;
// GLOBAL: FF7 0x00dc38dc
int g_BtlMenuResetWordB = 0;
// GLOBAL: FF7 0x00dc38e0
short g_BattleTargetCursorActive = 0;
// GLOBAL: FF7 0x00dc38e4
short g_BtlMenuPanelClearShort = 0;
// GLOBAL: FF7 0x00dc38e8
int g_BtlMenuMessageWindowAtRequest = 0;
// GLOBAL: FF7 0x00dc38ec
int g_BtlMenuMessageWindowAtX = 0;
// GLOBAL: FF7 0x00dc38f0
int g_BtlMenuMessageWindowAtY = 0;
// GLOBAL: FF7 0x00dc38f4
short g_BtlMenuSelectionActionId = 0;
// GLOBAL: FF7 0x00dc38f8
unsigned char g_BtlMenuLastSceneByte = 0;
unsigned char g_bssPad_00dc38fc[4] = {0};
// clang-format on



/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006cf5c0
void __cdecl Battle_UiMenuHookNoOp(void)
{
    /* The original body is empty. */
}

/*
 * Optionally draws a black translucent rectangle over screen rows 332 through 480,
 * then derives the battle cursor position from the active menu, its window
 * coordinates and selected row/column. Returns for unsupported menus, inactive
 * required window states or the explicit blocking flags.
 */
// FUNCTION: FF7 0x006cf5c5
void __cdecl Battle_DrawMenuCursorAndDimOverlay2x(int *rows)
{
    /* Block-scoped extern retained from retail declaration scope. */
    extern char g_BattleScreenRampActive;
    struct {
        GfxTlVertex *vtx11, *vtx10, *vtx9, *vtx8, *vtx7, *vtx6, *vtx5, *vtx4;
        float dv, du, texV, texU;
        GfxTlVertex *vtx3, *vtx2, *vtx1, *vtx0;
        union {
            unsigned int value;
            struct {
                unsigned char blue, green, red, alpha;
            } channels;
        } colour;
    } v; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if (g_BattleScreenRampActive != 0 && Menu_IsTextSubsystemReady() != 0) {
        v.colour.channels.red = v.colour.channels.green = v.colour.channels.blue = 0;
        v.colour.channels.alpha = *(unsigned char *)&g_BattleRainActiveCount;
        if (AcquireBatchNode(1, g_MenuTextBoxQuadGraphicsObject) != 0) {
            v.vtx0 = (GfxTlVertex *)(g_MenuTextBoxQuadGraphicsObject->vertexCursor);
            v.vtx0->x = 0.0f;
            v.vtx0->y = 332.0f;
            v.vtx0->z = 0.0f;
            v.vtx0->rhw = 1.0f;
            v.vtx0->colorUnsigned = v.colour.value;
            v.vtx0->specularUnsigned = 0xff000000;
            v.vtx1 = (GfxTlVertex *)(g_MenuTextBoxQuadGraphicsObject->vertexCursor + 0x20);
            v.vtx1->x = 0.0f;
            v.vtx1->y = 480.0f;
            v.vtx1->z = 0.0f;
            v.vtx1->rhw = 1.0f;
            v.vtx1->colorUnsigned = v.colour.value;
            v.vtx1->specularUnsigned = 0xff000000;
            v.vtx2 = (GfxTlVertex *)(g_MenuTextBoxQuadGraphicsObject->vertexCursor + 0x40);
            v.vtx2->x = 640.0f;
            v.vtx2->y = 332.0f;
            v.vtx2->z = 0.0f;
            v.vtx2->rhw = 1.0f;
            v.vtx2->colorUnsigned = v.colour.value;
            v.vtx2->specularUnsigned = 0xff000000;
            v.vtx3 = (GfxTlVertex *)(g_MenuTextBoxQuadGraphicsObject->vertexCursor + 0x60);
            v.vtx3->x = 640.0f;
            v.vtx3->y = 480.0f;
            v.vtx3->z = 0.0f;
            v.vtx3->rhw = 1.0f;
            v.vtx3->colorUnsigned = v.colour.value;
            v.vtx3->specularUnsigned = 0xff000000;
        }
    }
    switch (g_BattleMenuState) {
    case BATTLE_MENU_SELECT_TARGET:
        return;
    case BATTLE_MENU_COMMAND_MENU:
        if (g_BattleWindowStates[BATTLE_WINDOW_COMMAND_MENU] != BATTLE_WINDOW_STATE_OPEN)
            return;
        g_BattleMenuCursorX = g_BattleMenuData[BATTLE_WINDOW_COMMAND_MENU].x +
                          ((BattleMenuWindowSet *)g_BtlMenuWindowSets)[g_BtlActiveActor].cursorIndex * 0x58 +
                          (((BattleMenuWindowSet *)g_BtlMenuWindowSets)[g_BtlActiveActor].cursorIndex ? 16 : 0) - 0x24;
        g_BattleMenuCursorY = g_BattleMenuData[BATTLE_WINDOW_COMMAND_MENU].y +
                          *(int *)(((unsigned char *)&g_BtlMenuSlotRecords) + g_BtlActiveActor * 0x700) * 0x18 + 14;
        break;
    case BATTLE_MENU_LIMIT_LIST:
        if (g_BattleWindowStates[BATTLE_WINDOW_LIMIT_LIST] != BATTLE_WINDOW_STATE_OPEN || g_BtlMenuLimitCursorHidden != 0)
            return;
        g_BattleMenuCursorX = g_BattleMenuData[BATTLE_WINDOW_LIMIT_LIST].x - 36;
        g_BattleMenuCursorY = g_BattleMenuData[BATTLE_WINDOW_LIMIT_LIST].y +
                          ((BattleMenuCursor *)(&g_BtlMenuLimitCursor[4]))[g_BtlActivePartyMember].row * 30 + 22;
        break;
    case BATTLE_MENU_CHANGE_CONFIRM:
    case BATTLE_MENU_DEFEND_CONFIRM:
        g_BattleMenuCursorX = g_BattleMenuData[g_BattleMenuState].x - 36;
        g_BattleMenuCursorY = g_BattleMenuData[g_BattleMenuState].y + 16;
        break;
    case BATTLE_MENU_ITEM_LIST:
        if (g_BattleWindowStates[BATTLE_WINDOW_ITEM_LIST] != BATTLE_WINDOW_STATE_OPEN)
            return;
        g_BattleMenuCursorX = 40;
        g_BattleMenuCursorY = ((BtlMenuActorListCursorStride *)(&g_BtlMenuSlotTableA[0x4]))[g_BtlActiveActor].row * 32 + 0x16c;
        break;
    case BATTLE_MENU_MAGIC_LIST:
        if (g_BattleWindowStates[BATTLE_WINDOW_MAGIC_LIST] != BATTLE_WINDOW_STATE_OPEN)
            return;
        g_BattleMenuCursorX = ((BtlMenuActorFieldStride *)g_BtlMenuSlotTableB)[g_BtlActiveActor].value * 0x9c - 6;
        g_BattleMenuCursorY = ((BtlMenuActorFieldStride *)g_BtlMenuSlotTableC)[g_BtlActiveActor].value * 32 + 0x16c;
        break;
    case BATTLE_MENU_SUMMON_LIST:
        if (g_BattleWindowStates[BATTLE_WINDOW_SUMMON_LIST] != BATTLE_WINDOW_STATE_OPEN)
            return;
        g_BattleMenuCursorX = 60;
        g_BattleMenuCursorY = ((BtlMenuActorFieldStride *)g_BtlMenuSlotTableD)[g_BtlActiveActor].value * 32 + 0x16c;
        break;
    case BATTLE_MENU_ENEMY_SKILL_LIST:
        if (g_BattleWindowStates[BATTLE_WINDOW_ENEMY_SKILL_LIST] != BATTLE_WINDOW_STATE_OPEN)
            return;
        g_BattleMenuCursorX = ((BattleMenuWindow *)g_BtlMenuWindows)[g_BtlActiveActor].entryIndex ? 240 : 0;
        g_BattleMenuCursorY = ((BattleMenuCursor *)g_BtlMenuCursors)[g_BtlActiveActor].row * 32 + 0x16c;
        break;
    case BATTLE_MENU_GIL_AMOUNT:
        if (g_BattleWindowStates[BATTLE_WINDOW_GIL_AMOUNT] != BATTLE_WINDOW_STATE_OPEN)
            return;
        g_BattleMenuCursorX = g_BattleMenuData[BATTLE_WINDOW_GIL_AMOUNT].x - 36;
        g_BattleMenuCursorY = g_BattleMenuData[BATTLE_WINDOW_GIL_AMOUNT].y +
                          ((BtlMenuActorFieldStride *)(&g_BtlMenuGilCursor[1]))[g_BtlActiveActor].value * 0x18 + 14;
        break;
    case BATTLE_MENU_MANIP_ACTION_LIST:
        if (g_BattleWindowStates[BATTLE_WINDOW_MANIP_ACTION_LIST] != BATTLE_WINDOW_STATE_OPEN)
            return;
        g_BattleMenuCursorX = g_BattleMenuData[BATTLE_WINDOW_MANIP_ACTION_LIST].x - 36;
        g_BattleMenuCursorY = g_BattleMenuData[BATTLE_WINDOW_MANIP_ACTION_LIST].y + g_BtlMenuControlledActionRow * 32 + 16;
        break;
    case BATTLE_MENU_ARENA_SLOT:
        if (g_BattleWindowStates[BATTLE_WINDOW_ARENA_SLOT] != BATTLE_WINDOW_STATE_OPEN)
            return;
        if (g_BtlMenuSlotScreenState == BATTLE_SLOT_SCREEN_CHOOSE) {
            g_BattleMenuCursorX = g_BattleMenuData[BATTLE_WINDOW_ARENA_SLOT].x + g_BtlMenuSlotScreenCursor.col * 144 + 40;
            g_BattleMenuCursorY = g_BattleMenuData[BATTLE_WINDOW_ARENA_SLOT].y + 62;
        } else {
            return;
        }
        break;
    case BATTLE_MENU_ARENA_CONTINUE_CHOICE:
        if (g_BattleWindowStates[BATTLE_WINDOW_ARENA_CONTINUE_CHOICE] != BATTLE_WINDOW_STATE_OPEN)
            return;
        g_BattleMenuCursorX = g_BattleMenuData[BATTLE_WINDOW_ARENA_CONTINUE_CHOICE].x + g_BtlMenuWorriedChoiceCursor.col * (Battle_MeasureWorriedSecondChoice2x() + 64) + 20;
        g_BattleMenuCursorY = g_BattleMenuData[BATTLE_WINDOW_ARENA_CONTINUE_CHOICE].y + 62;
        break;
    default:
        return;
    }
    if (Input_GetHeld(INPUT_SQUARE) == 0 && Menu_IsTextSubsystemReady() != 0) {
        if (AcquireBatchNode(1, g_MenuWinBBlend4GraphicsObject) != 0) {
            v.texU = 0.75f;
            v.texV = 0.0625f;
            v.du = 0.1875f;
            v.dv = 0.1015625f;
            v.vtx4 = (GfxTlVertex *)(g_MenuWinBBlend4GraphicsObject->vertexCursor);
            v.vtx4->x = (float)g_BattleMenuCursorX;
            v.vtx4->y = (float)g_BattleMenuCursorY;
            v.vtx4->z = 0.009f;
            v.vtx4->rhw = 1.0f;
            v.vtx4->colorUnsigned = 0x80ffffff;
            v.vtx4->specularUnsigned = 0xff000000;
            v.vtx4->u = v.texU;
            v.vtx4->v = v.texV;
            v.vtx5 = (GfxTlVertex *)(g_MenuWinBBlend4GraphicsObject->vertexCursor + 0x20);
            v.vtx5->x = (float)g_BattleMenuCursorX;
            v.vtx5->y = (float)g_BattleMenuCursorY + 26.0f;
            v.vtx5->z = 0.009f;
            v.vtx5->rhw = 1.0f;
            v.vtx5->colorUnsigned = 0x80ffffff;
            v.vtx5->specularUnsigned = 0xff000000;
            v.vtx5->u = v.texU;
            v.vtx5->v = v.texV + v.dv;
            v.vtx6 = (GfxTlVertex *)(g_MenuWinBBlend4GraphicsObject->vertexCursor + 0x40);
            v.vtx6->x = (float)g_BattleMenuCursorX + 48.0f;
            v.vtx6->y = (float)g_BattleMenuCursorY;
            v.vtx6->z = 0.009f;
            v.vtx6->rhw = 1.0f;
            v.vtx6->colorUnsigned = 0x80ffffff;
            v.vtx6->specularUnsigned = 0xff000000;
            v.vtx6->u = v.texU + v.du;
            v.vtx6->v = v.texV;
            v.vtx7 = (GfxTlVertex *)(g_MenuWinBBlend4GraphicsObject->vertexCursor + 0x60);
            v.vtx7->x = (float)g_BattleMenuCursorX + 48.0f;
            v.vtx7->y = (float)g_BattleMenuCursorY + 26.0f;
            v.vtx7->z = 0.009f;
            v.vtx7->rhw = 1.0f;
            v.vtx7->colorUnsigned = 0x80ffffff;
            v.vtx7->specularUnsigned = 0xff000000;
            v.vtx7->u = v.texU + v.du;
            v.vtx7->v = v.texV + v.dv;
            **(unsigned char **)&g_MenuWinBBlend4GraphicsObject->colorIndex = 1;
            g_MenuWinBBlend4GraphicsObject->paletteIndex = 1;
        }
        if (AcquireBatchNode(1, g_MenuWinABlend0GraphicsObject) != 0) {
            v.texU = 0.1875f;
            v.texV = 0.59375f;
            v.dv = 0.03125f;
            v.vtx8 = (GfxTlVertex *)(g_MenuWinABlend0GraphicsObject->vertexCursor);
            v.vtx8->x = (float)g_BattleMenuCursorX;
            v.vtx8->y = (float)g_BattleMenuCursorY + 24.0f;
            v.vtx8->z = 0.009f;
            v.vtx8->rhw = 1.0f;
            v.vtx8->colorUnsigned = 0x80ffffff;
            v.vtx8->specularUnsigned = 0xff000000;
            v.vtx8->u = v.texU;
            v.vtx8->v = v.texV;
            v.vtx9 = (GfxTlVertex *)(g_MenuWinABlend0GraphicsObject->vertexCursor + 0x20);
            v.vtx9->x = (float)g_BattleMenuCursorX;
            v.vtx9->y = (float)g_BattleMenuCursorY + 24.0f + 8.0f;
            v.vtx9->z = 0.009f;
            v.vtx9->rhw = 1.0f;
            v.vtx9->colorUnsigned = 0x80ffffff;
            v.vtx9->specularUnsigned = 0xff000000;
            v.vtx9->u = v.texU;
            v.vtx9->v = v.texV + v.dv;
            v.vtx10 = (GfxTlVertex *)(g_MenuWinABlend0GraphicsObject->vertexCursor + 0x40);
            v.vtx10->x = (float)g_BattleMenuCursorX + 48.0f;
            v.vtx10->y = (float)g_BattleMenuCursorY + 24.0f;
            v.vtx10->z = 0.009f;
            v.vtx10->rhw = 1.0f;
            v.vtx10->colorUnsigned = 0x80ffffff;
            v.vtx10->specularUnsigned = 0xff000000;
            v.vtx10->u = v.texU + v.du;
            v.vtx10->v = v.texV;
            v.vtx11 = (GfxTlVertex *)(g_MenuWinABlend0GraphicsObject->vertexCursor + 0x60);
            v.vtx11->x = (float)g_BattleMenuCursorX + 48.0f;
            v.vtx11->y = (float)g_BattleMenuCursorY + 24.0f + 8.0f;
            v.vtx11->z = 0.009f;
            v.vtx11->rhw = 1.0f;
            v.vtx11->colorUnsigned = 0x80ffffff;
            v.vtx11->specularUnsigned = 0xff000000;
            v.vtx11->u = v.texU + v.du;
            v.vtx11->v = v.texV + v.dv;
            **(unsigned char **)&g_MenuWinABlend0GraphicsObject->colorIndex = 1;
            g_MenuWinABlend0GraphicsObject->texturePage = 1;
        }
    }
}

/*
 * Optionally draws a black translucent rectangle over the bottom 74 rows of the
 * 320 by 240 viewport, then derives the battle cursor position from the active
 * menu, window coordinates and selected row/column. Returns for unsupported menus,
 * inactive required window states or explicit blocking flags.
 */
// FUNCTION: FF7 0x006d0022
void __cdecl Battle_DrawMenuCursorAndDimOverlay1x(int *rows)
{
    /* Block-scoped extern retained from retail declaration scope. */
    extern char g_BattleScreenRampActive;
    struct {
        GfxTlVertex *vtx11, *vtx10, *vtx9, *vtx8, *vtx7, *vtx6, *vtx5, *vtx4;
        float dv, du, texV, texU;
        GfxTlVertex *vtx3, *vtx2, *vtx1, *vtx0;
        int top;
        union {
            unsigned int value;
            struct {
                unsigned char blue, green, red, alpha;
            } channels;
        } colour;
    } v; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if (g_BattleScreenRampActive != 0 && Menu_IsTextSubsystemReady() != 0) {
        v.top = g_MenuViewportY + 166;
        v.colour.channels.red = v.colour.channels.green = v.colour.channels.blue = 0;
        v.colour.channels.alpha = *(unsigned char *)&g_BattleRainActiveCount;
        if (AcquireBatchNode(1, g_MenuTextBoxQuadGraphicsObject) != 0) {
            v.vtx0 = (GfxTlVertex *)(g_MenuTextBoxQuadGraphicsObject->vertexCursor);
            v.vtx0->x = (float)g_MenuViewportX + 0.0f;
            v.vtx0->y = (float)v.top;
            v.vtx0->z = 0.0f;
            v.vtx0->rhw = 1.0f;
            v.vtx0->colorUnsigned = v.colour.value;
            v.vtx0->specularUnsigned = 0xff000000;
            v.vtx1 = (GfxTlVertex *)(g_MenuTextBoxQuadGraphicsObject->vertexCursor + 0x20);
            v.vtx1->x = (float)g_MenuViewportX + 0.0f;
            v.vtx1->y = (float)g_MenuViewportY + 240.0f;
            v.vtx1->z = 0.0f;
            v.vtx1->rhw = 1.0f;
            v.vtx1->colorUnsigned = v.colour.value;
            v.vtx1->specularUnsigned = 0xff000000;
            v.vtx2 = (GfxTlVertex *)(g_MenuTextBoxQuadGraphicsObject->vertexCursor + 0x40);
            v.vtx2->x = (float)g_MenuViewportX + 320.0f;
            v.vtx2->y = (float)v.top;
            v.vtx2->z = 0.0f;
            v.vtx2->rhw = 1.0f;
            v.vtx2->colorUnsigned = v.colour.value;
            v.vtx2->specularUnsigned = 0xff000000;
            v.vtx3 = (GfxTlVertex *)(g_MenuTextBoxQuadGraphicsObject->vertexCursor + 0x60);
            v.vtx3->x = (float)g_MenuViewportX + 320.0f;
            v.vtx3->y = (float)g_MenuViewportY + 240.0f;
            v.vtx3->z = 0.0f;
            v.vtx3->rhw = 1.0f;
            v.vtx3->colorUnsigned = v.colour.value;
            v.vtx3->specularUnsigned = 0xff000000;
        }
    }
    switch (g_BattleMenuState) {
    case BATTLE_MENU_SELECT_TARGET:
        return;
    case BATTLE_MENU_COMMAND_MENU:
        if (g_BattleWindowStates[BATTLE_WINDOW_COMMAND_MENU] != BATTLE_WINDOW_STATE_OPEN)
            return;
        g_BattleMenuCursorX = g_BattleMenuData[BATTLE_WINDOW_COMMAND_MENU].x +
                          ((BattleMenuWindowSet *)g_BtlMenuWindowSets)[g_BtlActiveActor].cursorIndex * 0x2c +
                          (((BattleMenuWindowSet *)g_BtlMenuWindowSets)[g_BtlActiveActor].cursorIndex ? 8 : 0) - 0x12;
        g_BattleMenuCursorY = g_BattleMenuData[BATTLE_WINDOW_COMMAND_MENU].y +
                          *(int *)(((unsigned char *)&g_BtlMenuSlotRecords) + g_BtlActiveActor * 0x700) * 12 + 7;
        break;
    case BATTLE_MENU_LIMIT_LIST:
        if (g_BattleWindowStates[BATTLE_WINDOW_LIMIT_LIST] != BATTLE_WINDOW_STATE_OPEN || g_BtlMenuLimitCursorHidden != 0)
            return;
        g_BattleMenuCursorX = g_BattleMenuData[BATTLE_WINDOW_LIMIT_LIST].x - 18;
        g_BattleMenuCursorY = g_BattleMenuData[BATTLE_WINDOW_LIMIT_LIST].y +
                          ((BattleMenuCursor *)(&g_BtlMenuLimitCursor[4]))[g_BtlActivePartyMember].row * 15 + 11;
        break;
    case BATTLE_MENU_CHANGE_CONFIRM:
    case BATTLE_MENU_DEFEND_CONFIRM:
        g_BattleMenuCursorX = g_BattleMenuData[g_BattleMenuState].x - 18;
        g_BattleMenuCursorY = g_BattleMenuData[g_BattleMenuState].y + 8;
        break;
    case BATTLE_MENU_ITEM_LIST:
        if (g_BattleWindowStates[BATTLE_WINDOW_ITEM_LIST] != BATTLE_WINDOW_STATE_OPEN)
            return;
        g_BattleMenuCursorX = 20;
        g_BattleMenuCursorY = ((BtlMenuActorListCursorStride *)(&g_BtlMenuSlotTableA[0x4]))[g_BtlActiveActor].row * 16 + 0xb6;
        break;
    case BATTLE_MENU_MAGIC_LIST:
        if (g_BattleWindowStates[BATTLE_WINDOW_MAGIC_LIST] != BATTLE_WINDOW_STATE_OPEN)
            return;
        g_BattleMenuCursorX = ((BtlMenuActorFieldStride *)g_BtlMenuSlotTableB)[g_BtlActiveActor].value * 0x4e - 3;
        g_BattleMenuCursorY = ((BtlMenuActorFieldStride *)g_BtlMenuSlotTableC)[g_BtlActiveActor].value * 16 + 0xb6;
        break;
    case BATTLE_MENU_SUMMON_LIST:
        if (g_BattleWindowStates[BATTLE_WINDOW_SUMMON_LIST] != BATTLE_WINDOW_STATE_OPEN)
            return;
        g_BattleMenuCursorX = 30;
        g_BattleMenuCursorY = ((BtlMenuActorFieldStride *)g_BtlMenuSlotTableD)[g_BtlActiveActor].value * 16 + 0xb6;
        break;
    case BATTLE_MENU_ENEMY_SKILL_LIST:
        if (g_BattleWindowStates[BATTLE_WINDOW_ENEMY_SKILL_LIST] != BATTLE_WINDOW_STATE_OPEN)
            return;
        g_BattleMenuCursorX = ((BattleMenuWindow *)g_BtlMenuWindows)[g_BtlActiveActor].entryIndex ? 120 : 0;
        g_BattleMenuCursorY = ((BattleMenuCursor *)g_BtlMenuCursors)[g_BtlActiveActor].row * 16 + 0xb6;
        break;
    case BATTLE_MENU_GIL_AMOUNT:
        if (g_BattleWindowStates[BATTLE_WINDOW_GIL_AMOUNT] != BATTLE_WINDOW_STATE_OPEN)
            return;
        g_BattleMenuCursorX = g_BattleMenuData[BATTLE_WINDOW_GIL_AMOUNT].x - 18;
        g_BattleMenuCursorY = g_BattleMenuData[BATTLE_WINDOW_GIL_AMOUNT].y +
                          ((BtlMenuActorFieldStride *)(&g_BtlMenuGilCursor[1]))[g_BtlActiveActor].value * 16 + 8;
        break;
    case BATTLE_MENU_MANIP_ACTION_LIST:
        if (g_BattleWindowStates[BATTLE_WINDOW_MANIP_ACTION_LIST] != BATTLE_WINDOW_STATE_OPEN)
            return;
        g_BattleMenuCursorX = g_BattleMenuData[BATTLE_WINDOW_MANIP_ACTION_LIST].x - 18;
        g_BattleMenuCursorY = g_BattleMenuData[BATTLE_WINDOW_MANIP_ACTION_LIST].y + g_BtlMenuControlledActionRow * 16 + 8;
        break;
    case BATTLE_MENU_ARENA_SLOT:
        if (g_BattleWindowStates[BATTLE_WINDOW_ARENA_SLOT] != BATTLE_WINDOW_STATE_OPEN)
            return;
        if (g_BtlMenuSlotScreenState == BATTLE_SLOT_SCREEN_CHOOSE) {
            g_BattleMenuCursorX = g_BattleMenuData[BATTLE_WINDOW_ARENA_SLOT].x + g_BtlMenuSlotScreenCursor.col * 72 + 20;
            g_BattleMenuCursorY = g_BattleMenuData[BATTLE_WINDOW_ARENA_SLOT].y + 31;
        } else {
            return;
        }
        break;
    case BATTLE_MENU_ARENA_CONTINUE_CHOICE:
        if (g_BattleWindowStates[BATTLE_WINDOW_ARENA_CONTINUE_CHOICE] != BATTLE_WINDOW_STATE_OPEN)
            return;
        g_BattleMenuCursorX = g_BattleMenuData[BATTLE_WINDOW_ARENA_CONTINUE_CHOICE].x + g_BtlMenuWorriedChoiceCursor.col * (Battle_MeasureWorriedSecondChoice1x() + 32) + 10;
        g_BattleMenuCursorY = g_BattleMenuData[BATTLE_WINDOW_ARENA_CONTINUE_CHOICE].y + 31;
        break;
    default:
        return;
    }
    if (Input_GetHeld(INPUT_SQUARE) == 0 && Menu_IsTextSubsystemReady() != 0) {
        g_BattleMenuCursorX += (short)g_MenuViewportX;
        g_BattleMenuCursorY += (short)g_MenuViewportY;
        if (AcquireBatchNode(1, g_MenuWinBlend4GraphicsObject) != 0) {
            v.texU = 0.875f;
            v.texV = 0.03125f;
            v.du = 0.09375f;
            v.dv = 0.05078125f;
            v.vtx4 = (GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor);
            v.vtx4->x = (float)g_BattleMenuCursorX;
            v.vtx4->y = (float)g_BattleMenuCursorY;
            v.vtx4->z = 0.009f;
            v.vtx4->rhw = 1.0f;
            v.vtx4->colorUnsigned = 0x80ffffff;
            v.vtx4->specularUnsigned = 0xff000000;
            v.vtx4->u = v.texU;
            v.vtx4->v = v.texV;
            v.vtx5 = (GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 0x20);
            v.vtx5->x = (float)g_BattleMenuCursorX;
            v.vtx5->y = (float)g_BattleMenuCursorY + 13.0f;
            v.vtx5->z = 0.009f;
            v.vtx5->rhw = 1.0f;
            v.vtx5->colorUnsigned = 0x80ffffff;
            v.vtx5->specularUnsigned = 0xff000000;
            v.vtx5->u = v.texU;
            v.vtx5->v = v.texV + v.dv;
            v.vtx6 = (GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 0x40);
            v.vtx6->x = (float)g_BattleMenuCursorX + 24.0f;
            v.vtx6->y = (float)g_BattleMenuCursorY;
            v.vtx6->z = 0.009f;
            v.vtx6->rhw = 1.0f;
            v.vtx6->colorUnsigned = 0x80ffffff;
            v.vtx6->specularUnsigned = 0xff000000;
            v.vtx6->u = v.texU + v.du;
            v.vtx6->v = v.texV;
            v.vtx7 = (GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 0x60);
            v.vtx7->x = (float)g_BattleMenuCursorX + 24.0f;
            v.vtx7->y = (float)g_BattleMenuCursorY + 13.0f;
            v.vtx7->z = 0.009f;
            v.vtx7->rhw = 1.0f;
            v.vtx7->colorUnsigned = 0x80ffffff;
            v.vtx7->specularUnsigned = 0xff000000;
            v.vtx7->u = v.texU + v.du;
            v.vtx7->v = v.texV + v.dv;
            *g_MenuWinBlend4GraphicsObject->colorBytePtr = 1;
            g_MenuWinBlend4GraphicsObject->texturePage = 1;
        }
        if (AcquireBatchNode(1, g_MenuWinBlend0GraphicsObject) != 0) {
            v.texU = 0.1015625f;
            v.texV = 0.296875f;
            v.dv = 0.015625f;
            v.vtx8 = (GfxTlVertex *)(g_MenuWinBlend0GraphicsObject->vertexCursor);
            v.vtx8->x = (float)g_BattleMenuCursorX;
            v.vtx8->y = (float)g_BattleMenuCursorY + 12.0f;
            v.vtx8->z = 0.009f;
            v.vtx8->rhw = 1.0f;
            v.vtx8->colorUnsigned = 0x80ffffff;
            v.vtx8->specularUnsigned = 0xff000000;
            v.vtx8->u = v.texU;
            v.vtx8->v = v.texV;
            v.vtx9 = (GfxTlVertex *)(g_MenuWinBlend0GraphicsObject->vertexCursor + 0x20);
            v.vtx9->x = (float)g_BattleMenuCursorX;
            v.vtx9->y = (float)g_BattleMenuCursorY + 12.0f + 4.0f;
            v.vtx9->z = 0.009f;
            v.vtx9->rhw = 1.0f;
            v.vtx9->colorUnsigned = 0x80ffffff;
            v.vtx9->specularUnsigned = 0xff000000;
            v.vtx9->u = v.texU;
            v.vtx9->v = v.texV + v.dv;
            v.vtx10 = (GfxTlVertex *)(g_MenuWinBlend0GraphicsObject->vertexCursor + 0x40);
            v.vtx10->x = (float)g_BattleMenuCursorX + 24.0f;
            v.vtx10->y = (float)g_BattleMenuCursorY + 12.0f;
            v.vtx10->z = 0.009f;
            v.vtx10->rhw = 1.0f;
            v.vtx10->colorUnsigned = 0x80ffffff;
            v.vtx10->specularUnsigned = 0xff000000;
            v.vtx10->u = v.texU + v.du;
            v.vtx10->v = v.texV;
            v.vtx11 = (GfxTlVertex *)(g_MenuWinBlend0GraphicsObject->vertexCursor + 0x60);
            v.vtx11->x = (float)g_BattleMenuCursorX + 24.0f;
            v.vtx11->y = (float)g_BattleMenuCursorY + 12.0f + 4.0f;
            v.vtx11->z = 0.009f;
            v.vtx11->rhw = 1.0f;
            v.vtx11->colorUnsigned = 0x80ffffff;
            v.vtx11->specularUnsigned = 0xff000000;
            v.vtx11->u = v.texU + v.du;
            v.vtx11->v = v.texV + v.dv;
            **(unsigned char **)&g_MenuWinBlend0GraphicsObject->colorIndex = 1;
            g_MenuWinBlend0GraphicsObject->texturePage = 1;
        }
    }
}

/*
 * Empty. A per-screen hook that ignores its argument.
 */
// FUNCTION: FF7 0x006d0ade
void __cdecl Battle_BeginWindowTextDrawNoOp(int what)
{
    /* The original body is empty. */
}

/*
 * Empty. A per-screen hook that ignores its argument.
 */
// FUNCTION: FF7 0x006d0ae3
void __cdecl Battle_EndWindowTextDrawNoOp(int what)
{
    /* The original body is empty. */
}

/*
 * Returns window index's state byte from g_BattleWindowStates, widened to an unsigned short.
 */
// FUNCTION: FF7 0x006d0ae8
unsigned short __cdecl Battle_GetWindowState(short index)
{
    return g_BattleWindowStates[index];
}

/*
 * Opens battle window `id` (a BattleWindowId: an index into g_BattleMenuData and the window tables).
 * It sets the window's state to BATTLE_WINDOW_STATE_OPENING, moves the box to (x, y)
 * unless either is -1, which keeps the stored position, stores half its width and height and
 * starts the visible size at 2x2, so the box grows open from its centre. Then it calls the
 * window's open handler from g_BtlMenuWindowOpenHandlers.
 */
// FUNCTION: FF7 0x006d0af9
void __cdecl Battle_OpenWindow(short x, short y, short id)
{
    g_BattleWindowStates[id] = BATTLE_WINDOW_STATE_OPENING;
    if (x != -1 && y != -1) {
        g_BattleMenuData[id].x = x;
        g_BattleMenuData[id].y = y;
    }
    g_BattleMenuData[id].halfWidth = (short)(g_BattleMenuData[id].width / 2);
    g_BattleMenuData[id].halfHeight = (short)(g_BattleMenuData[id].height / 2);
    g_BattleMenuData[id].clipWidth = 2;
    g_BattleMenuData[id].clipHeight = 2;
    g_BtlMenuWindowOpenHandlers[id]();
}

/*
 * Sets menu window `index`'s state g_BattleWindowStates[index] to BATTLE_WINDOW_STATE_CLOSING.
 */
// FUNCTION: FF7 0x006d0be1
void __cdecl Battle_SetWindowStateClosing(short index)
{
    g_BattleWindowStates[index] = BATTLE_WINDOW_STATE_CLOSING;
}

/*
 * Resets the menu window fade: g_BattleSelectionCursorState to 0 and the two fade words
 * g_BtlMenuCursorX/98 to -256.
 */
// FUNCTION: FF7 0x006d0bf1
void __cdecl Battle_ResetSelectionCursorPosition(void)
{
    g_BattleSelectionCursorState = 0;
    g_BtlMenuCursorX = 0xFFFFFF00;
    g_BtlMenuCursorY = 0xFFFFFF00;
}

/*
 * When text is ready, independently acquires and writes a half-alpha white
 * 48-pixel-wide cursor quad and a 48-by-8 shadow quad at Y plus 24.
 * g_BtlMenuTargetCursorState selects the cursor atlas region, height 26 or 32, batch and
 * palette one or seven; the shadow atlas X differs for zero versus nonzero mode
 * and always uses palette one.
 */
// FUNCTION: FF7 0x006d0c14
void __cdecl Battle_DrawSelectionCursorQuads2x(void *unused, short posX, short posY)
{
    float shadowUBase;
    GfxRenderObject *batchNode;
    float vBase;
    int unusedSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    float uSpan;
    float vRowSpan;
    GfxTlVertex *cursorTopLeftPtr;
    GfxTlVertex *iterCursorVert;
    GfxTlVertex *shadowVert1;
    GfxTlVertex *shadowTopRemaining;
    GfxTlVertex *shadowVert2;
    int cellPx;
    float inUStart;
    float shadowV;
    int depth;
    int paletteIndex;
    int depthShadow;
    int padWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    GfxTlVertex *cursorVertex1;
    GfxTlVertex *cursorVertex3;
    GfxTlVertex *pShadowVertAddr;

    if (g_BtlMenuTargetCursorState != 0) {
        if (g_BtlMenuTargetCursorState == 2) {
            inUStart = 0.3125f;
            vBase = 0.125f;
            paletteIndex = 7;
            batchNode = g_MenuWinABlend4GraphicsObject;
            vRowSpan = 0.125f;
            cellPx = 32;
            shadowUBase = 0.1875f;
            shadowV = 0.59375f;
            depth = FLOAT_0_01_BITS;
            depthShadow = 0x3C257A78;
        } else {
            inUStart = 0.75f;
            vBase = 0.0625f;
            paletteIndex = 1;
            batchNode = g_MenuWinBBlend4GraphicsObject;
            vRowSpan = 0.1015625f;
            cellPx = 26;
            shadowUBase = 0.1875f;
            shadowV = 0.59375f;
            depthShadow = 0x3C257A78;
            depth = depthShadow;
        }
    } else {
        inUStart = 0.5625f;
        vBase = 0.0625f;
        paletteIndex = 1;
        batchNode = g_MenuWinBBlend4GraphicsObject;
        vRowSpan = 0.1015625f;
        cellPx = 26;
        shadowUBase = 0.0f;
        shadowV = 0.59375f;
        depthShadow = 0x3C257A78;
        depth = depthShadow;
    }
    if (Menu_IsTextSubsystemReady() != 0) {
        if (AcquireBatchNode(1, batchNode) != 0) {
            uSpan = 0.1875f;
            cursorTopLeftPtr = (GfxTlVertex *)batchNode->vertexCursor;
            cursorTopLeftPtr->x = (float)posX;
            cursorTopLeftPtr->y = (float)posY;
            cursorTopLeftPtr->zBits = depth;
            cursorTopLeftPtr->rhwBits = FLOAT_ONE_BITS;
            cursorTopLeftPtr->color = 0x80FFFFFF;
            cursorTopLeftPtr->specular = 0xFF000000;
            cursorTopLeftPtr->u = inUStart;
            cursorTopLeftPtr->v = vBase;
            cursorVertex1 = (GfxTlVertex *)(batchNode->vertexCursor + 0x20);
            cursorVertex1->x = (float)posX;
            cursorVertex1->y = (float)posY + cellPx;
            cursorVertex1->zBits = depth;
            cursorVertex1->rhwBits = FLOAT_ONE_BITS;
            cursorVertex1->color = 0x80FFFFFF;
            cursorVertex1->specular = 0xFF000000;
            cursorVertex1->u = inUStart;
            cursorVertex1->v = vBase + vRowSpan;
            iterCursorVert = (GfxTlVertex *)(batchNode->vertexCursor + 0x40);
            iterCursorVert->x = (float)posX + 48.0f;
            iterCursorVert->y = (float)posY;
            iterCursorVert->zBits = depth;
            iterCursorVert->rhwBits = FLOAT_ONE_BITS;
            iterCursorVert->color = 0x80FFFFFF;
            iterCursorVert->specular = 0xFF000000;
            iterCursorVert->u = inUStart + uSpan;
            iterCursorVert->v = vBase;
            cursorVertex3 = (GfxTlVertex *)(batchNode->vertexCursor + 0x60);
            cursorVertex3->x = (float)posX + 48.0f;
            cursorVertex3->y = (float)posY + cellPx;
            cursorVertex3->zBits = depth;
            cursorVertex3->rhwBits = FLOAT_ONE_BITS;
            cursorVertex3->color = 0x80FFFFFF;
            cursorVertex3->specular = 0xFF000000;
            cursorVertex3->u = inUStart + uSpan;
            cursorVertex3->v = vBase + vRowSpan;
            *(char *)batchNode->colorIndex = paletteIndex;
            batchNode->paletteIndex = paletteIndex;
        }
        if (AcquireBatchNode(1, g_MenuWinABlend0GraphicsObject) != 0) {
            vRowSpan = 0.03125f;
            shadowTopRemaining = (GfxTlVertex *)(g_MenuWinABlend0GraphicsObject->vertexCursor);
            shadowTopRemaining->x = (float)posX;
            shadowTopRemaining->y = (float)posY + 24.0f;
            shadowTopRemaining->zBits = depthShadow;
            shadowTopRemaining->rhwBits = FLOAT_ONE_BITS;
            shadowTopRemaining->color = 0x80FFFFFF;
            shadowTopRemaining->specular = 0xFF000000;
            shadowTopRemaining->u = shadowUBase;
            shadowTopRemaining->v = shadowV;
            shadowVert1 = (GfxTlVertex *)(g_MenuWinABlend0GraphicsObject->vertexCursor + 0x20);
            shadowVert1->x = (float)posX;
            shadowVert1->y = (float)posY + 24.0f + 8.0f;
            shadowVert1->zBits = depthShadow;
            shadowVert1->rhwBits = FLOAT_ONE_BITS;
            shadowVert1->color = 0x80FFFFFF;
            shadowVert1->specular = 0xFF000000;
            shadowVert1->u = shadowUBase;
            shadowVert1->v = shadowV + vRowSpan;
            shadowVert2 = (GfxTlVertex *)(g_MenuWinABlend0GraphicsObject->vertexCursor + 0x40);
            shadowVert2->x = (float)posX + 48.0f;
            shadowVert2->y = (float)posY + 24.0f;
            shadowVert2->zBits = depthShadow;
            shadowVert2->rhwBits = FLOAT_ONE_BITS;
            shadowVert2->color = 0x80FFFFFF;
            shadowVert2->specular = 0xFF000000;
            shadowVert2->u = shadowUBase + uSpan;
            shadowVert2->v = shadowV;
            pShadowVertAddr = (GfxTlVertex *)(g_MenuWinABlend0GraphicsObject->vertexCursor + 0x60);
            pShadowVertAddr->x = (float)posX + 48.0f;
            pShadowVertAddr->y = (float)posY + 24.0f + 8.0f;
            pShadowVertAddr->zBits = depthShadow;
            pShadowVertAddr->rhwBits = FLOAT_ONE_BITS;
            pShadowVertAddr->color = 0x80FFFFFF;
            pShadowVertAddr->specular = 0xFF000000;
            pShadowVertAddr->u = shadowUBase + uSpan;
            pShadowVertAddr->v = shadowV + vRowSpan;
            *(char *)g_MenuWinABlend0GraphicsObject->colorIndex = 1;
            g_MenuWinABlend0GraphicsObject->texturePage = 1;
        }
    }
}

/*
 * When text is ready, independently emits the current UI cursor and its shadow at
 * supplied short coordinates. Cursor mode g_BtlMenuTargetCursorState chooses atlas origin,
 * 13- or 16-pixel height, palette selector 1 or 7 and retained depth values; all
 * cursor widths are 24.
 */
// FUNCTION: FF7 0x006d1127
void __cdecl Battle_DrawSelectionCursorQuads1x(void *unused, short posX, short posY)
{
    float shadowUBase;
    float subBaseV;
    int unusedSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    float inUStart;
    float vPitch;
    int depthShadow;
    GfxTlVertex *cursorTopLeftPtr;
    GfxTlVertex *iterCursorVert;
    GfxTlVertex *shadowVert1;
    GfxTlVertex *shadowTopRemaining;
    GfxTlVertex *pShadowVertAddr;
    int stageCursorHeight;
    float shadowV;
    int depth;
    float uSpan;
    int paletteIndex;
    int padWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    GfxTlVertex *cursorVertex1;
    GfxTlVertex *cursorVertex3;
    GfxTlVertex *shadowVert2;

    if (g_BtlMenuTargetCursorState != 0) {
        if (g_BtlMenuTargetCursorState == 2) {
            inUStart = 0.15625f;
            subBaseV = 0.0625f;
            paletteIndex = 7;
            vPitch = 0.0625f;
            stageCursorHeight = 16;
            shadowUBase = 0.1015625f;
            shadowV = 0.296875f;
            depth = FLOAT_0_01_BITS;
            depthShadow = 0x3C257A78;
        } else {
            inUStart = 0.875f;
            subBaseV = 0.03125f;
            paletteIndex = 1;
            vPitch = 0.05078125f;
            stageCursorHeight = 13;
            shadowUBase = 0.1015625f;
            shadowV = 0.296875f;
            depthShadow = 0x3C257A78;
            depth = depthShadow;
        }
    } else {
        inUStart = 0.78125f;
        subBaseV = 0.03125f;
        paletteIndex = 1;
        vPitch = 0.05078125f;
        stageCursorHeight = 13;
        shadowUBase = 0.0f;
        shadowV = 0.296875f;
        depthShadow = 0x3C257A78;
        depth = depthShadow;
    }
    if (Menu_IsTextSubsystemReady() != 0) {
        if (AcquireBatchNode(1, g_MenuWinBlend4GraphicsObject) != 0) {
            uSpan = 0.09375f;
            cursorTopLeftPtr = (GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor);
            cursorTopLeftPtr->x = (float)posX;
            cursorTopLeftPtr->y = (float)posY;
            cursorTopLeftPtr->zBits = depth;
            cursorTopLeftPtr->rhwBits = FLOAT_ONE_BITS;
            cursorTopLeftPtr->color = 0x80FFFFFF;
            cursorTopLeftPtr->specular = 0xFF000000;
            cursorTopLeftPtr->u = inUStart;
            cursorTopLeftPtr->v = subBaseV;
            cursorVertex1 = (GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 0x20);
            cursorVertex1->x = (float)posX;
            cursorVertex1->y = (float)posY + stageCursorHeight;
            cursorVertex1->zBits = depth;
            cursorVertex1->rhwBits = FLOAT_ONE_BITS;
            cursorVertex1->color = 0x80FFFFFF;
            cursorVertex1->specular = 0xFF000000;
            cursorVertex1->u = inUStart;
            cursorVertex1->v = subBaseV + vPitch;
            iterCursorVert = (GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 0x40);
            iterCursorVert->x = (float)posX + 24.0f;
            iterCursorVert->y = (float)posY;
            iterCursorVert->zBits = depth;
            iterCursorVert->rhwBits = FLOAT_ONE_BITS;
            iterCursorVert->color = 0x80FFFFFF;
            iterCursorVert->specular = 0xFF000000;
            iterCursorVert->u = inUStart + uSpan;
            iterCursorVert->v = subBaseV;
            cursorVertex3 = (GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 0x60);
            cursorVertex3->x = (float)posX + 24.0f;
            cursorVertex3->y = (float)posY + stageCursorHeight;
            cursorVertex3->zBits = depth;
            cursorVertex3->rhwBits = FLOAT_ONE_BITS;
            cursorVertex3->color = 0x80FFFFFF;
            cursorVertex3->specular = 0xFF000000;
            cursorVertex3->u = inUStart + uSpan;
            cursorVertex3->v = subBaseV + vPitch;
            *g_MenuWinBlend4GraphicsObject->colorBytePtr = paletteIndex;
            g_MenuWinBlend4GraphicsObject->texturePage = paletteIndex;
        }
        if (AcquireBatchNode(1, g_MenuWinBlend0GraphicsObject) != 0) {
            vPitch = 0.015625f;
            shadowTopRemaining = (GfxTlVertex *)(g_MenuWinBlend0GraphicsObject->vertexCursor);
            shadowTopRemaining->x = (float)posX;
            shadowTopRemaining->y = (float)posY + 12.0f;
            shadowTopRemaining->zBits = depthShadow;
            shadowTopRemaining->rhwBits = FLOAT_ONE_BITS;
            shadowTopRemaining->color = 0x80FFFFFF;
            shadowTopRemaining->specular = 0xFF000000;
            shadowTopRemaining->u = shadowUBase;
            shadowTopRemaining->v = shadowV;
            shadowVert1 = (GfxTlVertex *)(g_MenuWinBlend0GraphicsObject->vertexCursor + 0x20);
            shadowVert1->x = (float)posX;
            shadowVert1->y = (float)posY + 12.0f + 4.0f;
            shadowVert1->zBits = depthShadow;
            shadowVert1->rhwBits = FLOAT_ONE_BITS;
            shadowVert1->color = 0x80FFFFFF;
            shadowVert1->specular = 0xFF000000;
            shadowVert1->u = shadowUBase;
            shadowVert1->v = shadowV + vPitch;
            shadowVert2 = (GfxTlVertex *)(g_MenuWinBlend0GraphicsObject->vertexCursor + 0x40);
            shadowVert2->x = (float)posX + 24.0f;
            shadowVert2->y = (float)posY + 12.0f;
            shadowVert2->zBits = depthShadow;
            shadowVert2->rhwBits = FLOAT_ONE_BITS;
            shadowVert2->color = 0x80FFFFFF;
            shadowVert2->specular = 0xFF000000;
            shadowVert2->u = shadowUBase + uSpan;
            shadowVert2->v = shadowV;
            pShadowVertAddr = (GfxTlVertex *)(g_MenuWinBlend0GraphicsObject->vertexCursor + 0x60);
            pShadowVertAddr->x = (float)posX + 24.0f;
            pShadowVertAddr->y = (float)posY + 12.0f + 4.0f;
            pShadowVertAddr->zBits = depthShadow;
            pShadowVertAddr->rhwBits = FLOAT_ONE_BITS;
            pShadowVertAddr->color = 0x80FFFFFF;
            pShadowVertAddr->specular = 0xFF000000;
            pShadowVertAddr->u = shadowUBase + uSpan;
            pShadowVertAddr->v = shadowV + vPitch;
            *(char *)g_MenuWinBlend0GraphicsObject->colorIndex = 1;
            g_MenuWinBlend0GraphicsObject->texturePage = 1;
        }
    }
}

/*
 * Empty. The original reserves eight bytes of locals it never reads, hence the unused array.
 */
// FUNCTION: FF7 0x006d162e
void __cdecl Battle_SelectionCursorOverlayNoOp(void *what, short x, short y)
{
    int unused[2];
}

/*
 * Empty. A window-draw hook that ignores its arguments.
 */
// FUNCTION: FF7 0x006d1638
void __cdecl Battle_EraseSelectionCursorNoOp(void *what, short x, short y)
{
    /* The original body is empty. */
}

/*
 * Derives an even-aligned battle-menu position and its draw mode from the selected
 * record, with fixed override position/mode for the two global flags. Draws
 * through two helpers only within the retained viewport.
 */
// FUNCTION: FF7 0x006d163d
void __cdecl Battle_PositionAndDrawTargetSelectionCursor2x(void *cursorCtx, short actorIndex)
{
    int q;
    short rect[4] = {0, 0, 0, 0};
    short posX;
    int j;
    int ao;
    int i;
    short tempCursorY;
    int k;
    int l;

    if (g_BattleActorRecords[actorIndex].facing == 0) {
        g_BtlMenuTargetCursorState = 1;
        posX = g_BattleActorScreenPositions[actorIndex].prevX - 48;
    } else {
        g_BtlMenuTargetCursorState = 0;
        posX = g_BattleActorScreenPositions[actorIndex].prevX;
    }
    tempCursorY = g_BattleActorScreenPositions[actorIndex].prevY;
    posX &= 0xfffe;
    tempCursorY &= 0xfffe;

    if (g_BattleScriptEffectFlag != 0 || g_BtlPointedTargetInvalid != 0) {
        posX = 64;
        tempCursorY = 268;
        if ((g_BtlMenuFrameCounter & 8) != 0) {
            g_BtlMenuTargetCursorState = 2;
        } else {
            g_BtlMenuTargetCursorState = 1;
        }
    }

    if (g_BattleSelectionCursorState == 0) {
        if (posX >= 0 && posX < 592 && tempCursorY >= 32 && tempCursorY < 332) {
            Battle_DrawSelectionCursorQuads2x(cursorCtx, posX, tempCursorY);
        }
        if (posX >= 0 && posX < 592 && tempCursorY >= 32 && tempCursorY < 332) {
            Battle_SelectionCursorOverlayNoOp(cursorCtx, posX, tempCursorY);
        }
        g_BtlMenuCursorX = posX;
        g_BtlMenuCursorY = tempCursorY;
        g_BattleSelectionCursorState = 1;
    } else {
        if (posX >= 0 && posX < 592 && tempCursorY >= 32 && tempCursorY < 332) {
            Battle_DrawSelectionCursorQuads2x(cursorCtx, posX, tempCursorY);
        }
        if (posX >= 0 && posX < 592 && tempCursorY >= 32 && tempCursorY < 332) {
            Battle_SelectionCursorOverlayNoOp(cursorCtx, posX, tempCursorY);
        }
        if (g_BtlMenuCursorX >= 0 && g_BtlMenuCursorX < 592 &&
            g_BtlMenuCursorY >= 32 && g_BtlMenuCursorY < 332) {
            Battle_EraseSelectionCursorNoOp(cursorCtx, g_BtlMenuCursorX, g_BtlMenuCursorY);
        }
        g_BtlMenuCursorX = posX;
        g_BtlMenuCursorY = tempCursorY;
    }
}

/*
 * Derives an even-aligned battle-menu position and draw mode from the selected
 * record, with a base-relative override for the two global flags. Draws through
 * two helpers inside the base- relative viewport.
 */
// FUNCTION: FF7 0x006d1894
void __cdecl Battle_PositionAndDrawTargetSelectionCursor1x(void *cursorCtx, short actorIndex)
{
    int q;
    short rect[4] = {0, 0, 0, 0};
    short posX;
    int j;
    int ao;
    int i;
    short tempCursorY;
    int k;
    int l;
    int baseX;
    int baseY;

    baseX = g_MenuViewportX;
    baseY = g_MenuViewportY;
    if (g_BattleActorRecords[actorIndex].facing == 0) {
        g_BtlMenuTargetCursorState = 1;
        posX = g_BattleActorScreenPositions[actorIndex].prevX - 24;
    } else {
        g_BtlMenuTargetCursorState = 0;
        posX = g_BattleActorScreenPositions[actorIndex].prevX;
    }
    tempCursorY = g_BattleActorScreenPositions[actorIndex].prevY;
    posX &= 0xfffe;
    tempCursorY &= 0xfffe;

    if (g_BattleScriptEffectFlag != 0 || g_BtlPointedTargetInvalid != 0) {
        posX = g_MenuViewportX + 32;
        tempCursorY = g_MenuViewportY + 134;
        if ((g_BtlMenuFrameCounter & 8) != 0) {
            g_BtlMenuTargetCursorState = 2;
        } else {
            g_BtlMenuTargetCursorState = 1;
        }
    }

    if (g_BattleSelectionCursorState == 0) {
        if (posX >= baseX && posX < baseX + 0x128 &&
            tempCursorY >= baseY + 0x10 && tempCursorY < baseY + 0xa6) {
            Battle_DrawSelectionCursorQuads1x(cursorCtx, posX, tempCursorY);
        }
        if (posX >= baseX && posX < baseX + 0x128 &&
            tempCursorY >= baseY + 0x10 && tempCursorY < baseY + 0xa6) {
            Battle_SelectionCursorOverlayNoOp(cursorCtx, posX, tempCursorY);
        }
        g_BtlMenuCursorX = posX;
        g_BtlMenuCursorY = tempCursorY;
        g_BattleSelectionCursorState = 1;
    } else {
        if (posX >= baseX && posX < baseX + 0x128 &&
            tempCursorY >= baseY + 0x10 && tempCursorY < baseY + 0xa6) {
            Battle_DrawSelectionCursorQuads1x(cursorCtx, posX, tempCursorY);
        }
        if (posX >= baseX && posX < baseX + 0x128 &&
            tempCursorY >= baseY + 0x10 && tempCursorY < baseY + 0xa6) {
            Battle_SelectionCursorOverlayNoOp(cursorCtx, posX, tempCursorY);
        }
        if (g_BtlMenuCursorX >= baseX && g_BtlMenuCursorX < baseX + 0x128 &&
            g_BtlMenuCursorY >= baseY + 0x10 && g_BtlMenuCursorY < baseY + 0xa6) {
            Battle_EraseSelectionCursorNoOp(cursorCtx, g_BtlMenuCursorX, g_BtlMenuCursorY);
        }
        g_BtlMenuCursorX = posX;
        g_BtlMenuCursorY = tempCursorY;
    }
}

/*
 * Forwards to Battle_PositionAndDrawTargetSelectionCursor2x(a, b).
 */
// FUNCTION: FF7 0x006d1b5c
void __cdecl Battle_DrawSelectionCursorForTarget2x(void *cursorCtx, short actorIndex)
{
    Battle_PositionAndDrawTargetSelectionCursor2x(cursorCtx, actorIndex);
}

/*
 * Forwards to Battle_PositionAndDrawTargetSelectionCursor1x(a, b).
 */
// FUNCTION: FF7 0x006d1b72
void __cdecl Battle_DrawSelectionCursorForTarget1x(void *cursorCtx, short actorIndex)
{
    Battle_PositionAndDrawTargetSelectionCursor1x(cursorCtx, actorIndex);
}

/*
 * When g_BattleMenuState is zero, invokes the placement hook at fixed coordinates.
 * Otherwise invokes it at the mouse coordinates only when
 * g_BattleTargetCursorActive is nonzero and the pointer lies within the
 * configured viewport bounds.
 */
// FUNCTION: FF7 0x006d1b88
void __cdecl Battle_DrawTargetCursorWhenMenuStateZero2x(void *what)
{
    if (g_BattleMenuState == BATTLE_MENU_SELECT_TARGET) {
        Battle_DrawSelectionCursorForTarget2x(what, g_BtlPointedTarget);
    } else if (g_BattleTargetCursorActive != 0 && g_BtlMenuCursorX >= 0 &&
               g_BtlMenuCursorX < 592 && g_BtlMenuCursorY >= 32 &&
               g_BtlMenuCursorY < 332) {
        Battle_EraseSelectionCursorNoOp(what, g_BtlMenuCursorX, g_BtlMenuCursorY);
    }
}

/*
 * The same gate as Battle_DrawTargetCursorWhenMenuStateZero2x with a smaller box -- 0..0x128 by
 * 0x10..0xA6 -- and a different fixed-placement callee.
 */
// FUNCTION: FF7 0x006d1c00
void __cdecl Battle_DrawTargetCursorWhenMenuStateZero1x(void *what)
{
    if (g_BattleMenuState == BATTLE_MENU_SELECT_TARGET) {
        Battle_DrawSelectionCursorForTarget1x(what, g_BtlPointedTarget);
    } else if (g_BattleTargetCursorActive != 0 && g_BtlMenuCursorX >= 0 &&
               g_BtlMenuCursorX < 296 && g_BtlMenuCursorY >= 16 &&
               g_BtlMenuCursorY < 166) {
        Battle_EraseSelectionCursorNoOp(what, g_BtlMenuCursorX, g_BtlMenuCursorY);
    }
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006d1c78
void __cdecl Battle_UiFrameHookNoOp(void)
{
    /* The original body is empty. */
}

/*
 * Clears g_BtlMenuPanelClearShort.
 */
// FUNCTION: FF7 0x006d1c7d
void __cdecl BtlMenu_ClearPanelShortFlag(void)
{
    g_BtlMenuPanelClearShort = 0;
}

/*
 * Length of an FF7-encoded string: counts bytes up to the 0xFF terminator.
 */
// FUNCTION: FF7 0x006d1c8b
int __cdecl Text_GetEncodedStringLength(unsigned char *text)
{
    int n;

    n = 0;
    while (*text != 0xFF) {
        text++;
        n++;
    }
    return n;
}

// Each expansion owns a distinct pointer local in the original /Od frame.
// Keep the assignments in this order, including the repeated batch loads.
#define WINDOW_SOLID_VERTEX(batch, index, xpos, ypos, depth, colour) \
    {                                                                \
        GfxTlVertex *vertex;                                  \
        vertex = (batch)->vertices + (index);                        \
        vertex->x = (xpos);                                          \
        vertex->y = (ypos);                                          \
        vertex->z = (depth);                                         \
        vertex->rhw = 1.0f;                                          \
        vertex->colorUnsigned = (colour);                                  \
        vertex->specularUnsigned = 0xFF000000;                               \
    }

/* Steps a batch to its next quad: vertexStride bytes on. */
#define WINDOW_NEXT_QUAD(batch) ((batch)->vertices = (GfxTlVertex *)((char *)(batch)->vertices + (batch)->cursorStep))

#define WINDOW_TEXTURED_VERTEX(batch, index, xpos, ypos, depth, colour, ucoord, vcoord) \
    {                                                                                   \
        GfxTlVertex *vertex;                                                     \
        vertex = (batch)->vertices + (index);                                           \
        vertex->x = (xpos);                                                             \
        vertex->y = (ypos);                                                             \
        vertex->z = (depth);                                                            \
        vertex->rhw = 1.0f;                                                             \
        vertex->colorUnsigned = (colour);                                                     \
        vertex->specularUnsigned = 0xFF000000;                                                  \
        vertex->u = (ucoord);                                                           \
        vertex->v = (vcoord);                                                           \
    }

/*
 * Draw one battle text window: resolve popup/help/message text, measure and centre
 * the glyphs, then submit glyphs, the frame, emphasis and background. The 1x/2x
 * paths preserve their different atlas handling and depth writes.
 */
// FUNCTION: FF7 0x006d1cc0
void __cdecl Menu_DrawBattleWindow(int unused1, short windowIndex, int op, int param, int unused5)
{
    int unusedL_g;
    short advance_k;
    int unusedK_g;
    float dv_j;
    float du_j;
    int unusedJ_g;
    int unusedI_h;
    float v_e;
    int windowY_m;
    float u_e;
    int windowX_n;
    int unusedH_h;
    float depth_a;
    short penY_g;
    int unusedG_h;
    int unusedF_h;
    short charCode_l;
    short penX_h;
    short windowHeight_a;
    short windowWidth_f;
    unsigned char emphasis_d;
    short charIndex_o;
    unsigned char *savedText_i;
    GfxRenderObject *glyphBatch;
    short quadWidth_o;
    int unusedE_i;
    int unusedD_i;
    int unusedC_m;
    short clippedU_d;
    int originY_l;
    short atlasY_f;
    short textLimit_i;
    char unusedText_p[256];
    int originX_p;
    short atlasX_g;
    unsigned char *text_o;
    int unusedB_m;
    int unusedA_n;
    short palette_o;
    float glyphWidth_e;

    originX_p = g_MenuViewportX;
    originY_l = g_MenuViewportY;
    palette_o = 7;
    emphasis_d = 0;
    windowX_n = g_BattleMenuData[windowIndex].x;
    windowY_m = g_BattleMenuData[windowIndex].y;
    switch (windowIndex) {
    case BATTLE_WINDOW_HELP:
        text_o = g_BattleTextBuffer;
        textLimit_i = 26;
        break;
    case BATTLE_WINDOW_ACTION_NAME:
        windowIndex = BATTLE_WINDOW_MESSAGE;
        windowX_n = g_BattleMenuData[windowIndex].x;
        windowY_m = g_BattleMenuData[windowIndex].y;
        switch (op & 0xFF) {
        case BATTLE_EFFECT_CATEGORY_NONE:
        case BATTLE_EFFECT_CATEGORY_MAGIC:
            textLimit_i = 4;
            text_o = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_MAGIC, param & 0xFFFF, 8);
            break;
        case BATTLE_EFFECT_CATEGORY_SUMMON:
            textLimit_i = 8;
            text_o = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_SUMMON_ATTACK, param & 0xFFFF, 8);
            break;
        case BATTLE_EFFECT_CATEGORY_GENERAL:
            textLimit_i = 8;
            text_o = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM, param & 0xFFFF, 8);
            break;
        case BATTLE_EFFECT_CATEGORY_LIMIT:
            textLimit_i = 9;
            text_o = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_LIMIT, param & 0xFFFF, 8);
            break;
        case BATTLE_EFFECT_CATEGORY_ENEMY_SKILL:
            textLimit_i = 9;
            text_o = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ENEMY_SKILL, param & 0xFFFF, 8);
            break;
        case BATTLE_EFFECT_CATEGORY_SPECIAL:
            textLimit_i = 9;
            text_o = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ENEMY_ATTACK, param & 0xFFFF, 8);
            break;
        case BATTLE_EFFECT_CATEGORY_COIN:
            Battle_FormatWindowGilAmountText(param & 0xFFFF);
            text_o = g_BtlMenuGilAmountText;
            break;
        case BATTLE_EFFECT_CATEGORY_THROW:
            text_o = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM, param & 0xFFFF, 8);
            break;
        default:
            textLimit_i = 4;
            text_o = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_MAGIC, 0, 8);
        }
        break;
    case BATTLE_WINDOW_MESSAGE:
        text_o = g_BattleTextBuffer;
        textLimit_i = 26;
    }
    if (g_MenuIsSmallViewport320x240 != 1) {
        int count;
        count = 0;
        savedText_i = text_o;
        penX_h = 0;
        for (charIndex_o = 0; charIndex_o < 256; charIndex_o++) {
            if (*text_o == TEXT_END) {
                break;
            }
            if (*text_o == TEXT_FONT_PAGE_1) {
                text_o++;
                penX_h += (short)(g_pTextSpacing[*text_o + 0xE7] >> 5 << 1);
                penX_h += (short)((g_pTextSpacing[*text_o + 0xE7] & 0x1F) << 1);
                count++;
            } else {
                if (*text_o == TEXT_FONT_PAGE_2) {
                    text_o++;
                    penX_h += (short)(g_pTextSpacing[*text_o + 0x1B9] >> 5 << 1);
                    penX_h += (short)((g_pTextSpacing[*text_o + 0x1B9] & 0x1F) << 1);
                    count++;
                } else {
                    if (*text_o == TEXT_FONT_PAGE_3) {
                        text_o++;
                        penX_h += (short)(g_pTextSpacing[*text_o + 0x2A0] >> 5 << 1);
                        penX_h += (short)((g_pTextSpacing[*text_o + 0x2A0] & 0x1F) << 1);
                        count++;
                    } else {
                        if (*text_o == TEXT_BATTLE_EMPHASIS) {
                            emphasis_d = 1;
                            text_o++;
                        } else {
                            penX_h += (short)(g_pTextSpacing[*text_o] >> 5 << 1);
                            penX_h += (short)((g_pTextSpacing[*text_o] & 0x1F) << 1);
                            count++;
                        }
                    }
                }
            }
            text_o++;
        }
        text_o = savedText_i;
        penX_h = g_BattleMenuData[windowIndex].width / 2 - penX_h / 2;
        penY_g = 12;
        for (charIndex_o = 0; charIndex_o < 256; charIndex_o++) {
            if (*text_o == TEXT_END) {
                break;
            }
            switch (*text_o) {
            case TEXT_FONT_PAGE_1:
                text_o++;
                atlasX_g = *text_o % 21 * 12;
                atlasY_f = *text_o / 21 * 12 + 132;
                penX_h += (short)(g_pTextSpacing[*text_o + 0xE7] >> 5 << 1);
                advance_k = (g_pTextSpacing[*text_o + 0xE7] & 0x1F) << 1;
                break;
            case TEXT_FONT_PAGE_2:
                text_o++;
                atlasX_g = *text_o % 21 * 12;
                atlasY_f = *text_o / 21 * 12;
                penX_h += (short)(g_pTextSpacing[*text_o + 0x1B9] >> 5 << 1);
                advance_k = (g_pTextSpacing[*text_o + 0x1B9] & 0x1F) << 1;
                break;
            case TEXT_FONT_PAGE_3:
                text_o++;
                atlasX_g = *text_o % 21 * 12;
                atlasY_f = *text_o / 21 * 12 + 132;
                penX_h += (short)(g_pTextSpacing[*text_o + 0x2A0] >> 5 << 1);
                advance_k = (g_pTextSpacing[*text_o + 0x2A0] & 0x1F) << 1;
                break;
            case TEXT_BATTLE_EMPHASIS:
                text_o += 2;
                continue;
            default:
                charCode_l = *text_o;
                if (charCode_l == 0xD2 || charCode_l == 0xD3) {
                    charCode_l -= 0x4E;
                }
                atlasX_g = charCode_l % 21 * 24;
                atlasY_f = charCode_l / 21 * 24;
                if (atlasX_g > 240) {
                    clippedU_d = atlasX_g - 0x100;
                    glyphWidth_e = 24.0f;
                    quadWidth_o = 24;
                    glyphBatch = g_pBattle3dScene->glyphOverflow;
                } else {
                    if (atlasX_g == 240) {
                        clippedU_d = atlasX_g;
                        glyphWidth_e = 16.0f;
                        quadWidth_o = 16;
                    } else {
                        clippedU_d = atlasX_g;
                        glyphWidth_e = 24.0f;
                        quadWidth_o = 24;
                    }
                    glyphBatch = g_pBattle3dScene->glyph2x;
                }
                penX_h += (short)(g_pTextSpacing[*text_o] >> 5 << 1);
                advance_k = (g_pTextSpacing[*text_o] & 0x1F) << 1;
            }
            if (Menu_IsTextSubsystemReady() != 0 && g_BattleIsPaused == 0) {
                if (AcquireBatchNode(1, glyphBatch) != 0) {
                    u_e = (float)clippedU_d / 256.0f;
                    v_e = (float)atlasY_f / 256.0f;
                    du_j = glyphWidth_e / 256.0f;
                    dv_j = 0.09375f;
                    WINDOW_TEXTURED_VERTEX(glyphBatch, 0, (float)windowX_n + penX_h, (float)windowY_m + penY_g, 0.0f, 0x80FFFFFF, u_e, v_e);
                    WINDOW_TEXTURED_VERTEX(glyphBatch, 1, (float)windowX_n + penX_h, (float)windowY_m + penY_g + 24.0f, 0.0f, 0x80FFFFFF, u_e, v_e + dv_j);
                    WINDOW_TEXTURED_VERTEX(glyphBatch, 2, (float)windowX_n + penX_h + quadWidth_o, (float)windowY_m + penY_g, 0.0f, 0x80FFFFFF, u_e + du_j, v_e);
                    WINDOW_TEXTURED_VERTEX(glyphBatch, 3, (float)windowX_n + penX_h + quadWidth_o, (float)windowY_m + penY_g + 24.0f, 0.0f, 0x80FFFFFF, u_e + du_j, v_e + dv_j);
                    *glyphBatch->palette = (unsigned char)palette_o << 1;
                    glyphBatch->paletteIndex = palette_o << 1;
                }
            }
            text_o++;
            penX_h += advance_k;
        }
    } else {
        int count;
        count = 0;
        savedText_i = text_o;
        penX_h = 0;
        for (charIndex_o = 0; charIndex_o < 256; charIndex_o++) {
            if (*text_o == TEXT_END) {
                break;
            }
            if (*text_o == TEXT_FONT_PAGE_1) {
                text_o++;
                penX_h += (short)(g_pTextSpacing[*text_o + 0xE7] >> 5);
                penX_h += (short)(g_pTextSpacing[*text_o + 0xE7] & 0x1F);
                count++;
            } else {
                if (*text_o == TEXT_FONT_PAGE_2) {
                    text_o++;
                    penX_h += (short)(g_pTextSpacing[*text_o + 0x1B9] >> 5);
                    penX_h += (short)(g_pTextSpacing[*text_o + 0x1B9] & 0x1F);
                    count++;
                } else {
                    if (*text_o == TEXT_FONT_PAGE_3) {
                        text_o++;
                        penX_h += (short)(g_pTextSpacing[*text_o + 0x2A0] >> 5);
                        penX_h += (short)(g_pTextSpacing[*text_o + 0x2A0] & 0x1F);
                        count++;
                    } else {
                        if (*text_o == TEXT_BATTLE_EMPHASIS) {
                            emphasis_d = 1;
                            text_o++;
                        } else {
                            penX_h += (short)(g_pTextSpacing[*text_o] >> 5);
                            penX_h += (short)(g_pTextSpacing[*text_o] & 0x1F);
                            count++;
                        }
                    }
                }
            }
            text_o++;
        }
        text_o = savedText_i;
        penX_h = g_BattleMenuData[windowIndex].width / 2 - penX_h / 2;
        penY_g = 6;
        for (charIndex_o = 0; charIndex_o < 256; charIndex_o++) {
            if (*text_o == TEXT_END) {
                break;
            }
            switch (*text_o) {
            case TEXT_FONT_PAGE_1:
                text_o++;
                atlasX_g = *text_o % 21 * 12;
                atlasY_f = *text_o / 21 * 12 + 132;
                penX_h += (short)(g_pTextSpacing[*text_o + 0xE7] >> 5);
                advance_k = g_pTextSpacing[*text_o + 0xE7] & 0x1F;
                break;
            case TEXT_FONT_PAGE_2:
                text_o++;
                atlasX_g = *text_o % 21 * 12;
                atlasY_f = *text_o / 21 * 12;
                penX_h += (short)(g_pTextSpacing[*text_o + 0x1B9] >> 5);
                advance_k = g_pTextSpacing[*text_o + 0x1B9] & 0x1F;
                break;
            case TEXT_FONT_PAGE_3:
                text_o++;
                atlasX_g = *text_o % 21 * 12;
                atlasY_f = *text_o / 21 * 12 + 132;
                penX_h += (short)(g_pTextSpacing[*text_o + 0x2A0] >> 5);
                advance_k = g_pTextSpacing[*text_o + 0x2A0] & 0x1F;
                break;
            case TEXT_BATTLE_EMPHASIS:
                text_o += 2;
                continue;
            default:
                atlasX_g = *text_o % 21 * 12;
                atlasY_f = *text_o / 21 * 12;
                penX_h += (short)(g_pTextSpacing[*text_o] >> 5);
                advance_k = g_pTextSpacing[*text_o] & 0x1F;
            }
            if (Menu_IsTextSubsystemReady() != 0 && g_BattleIsPaused == 0) {
                if (AcquireBatchNode(1, g_pBattle3dScene->glyph2x) != 0) {
                    u_e = (float)atlasX_g * 0.00390625f;
                    v_e = (float)atlasY_f * 0.00390625f;
                    dv_j = 0.046875f;
                    du_j = dv_j;
                    WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->glyph2x, 0, (float)windowX_n + penX_h + originX_p, (float)windowY_m + penY_g + originY_l, 0.0f, 0x80FFFFFF, u_e, v_e);
                    WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->glyph2x, 1, (float)windowX_n + penX_h + originX_p, (float)windowY_m + penY_g + 12.0f + originY_l, 0.0f, 0x80FFFFFF, u_e, v_e + dv_j);
                    WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->glyph2x, 2, (float)windowX_n + penX_h + 12.0f + originX_p, (float)windowY_m + penY_g + originY_l, 0.0f, 0x80FFFFFF, u_e + du_j, v_e);
                    WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->glyph2x, 3, (float)windowX_n + penX_h + 12.0f + originX_p, (float)windowY_m + penY_g + 12.0f + originY_l, 0.0f, 0x80FFFFFF, u_e + du_j, v_e + dv_j);
                    *g_pBattle3dScene->glyph2x->palette = (unsigned char)palette_o << 1;
                    g_pBattle3dScene->glyph2x->paletteIndex = palette_o << 1;
                }
            }
            text_o++;
            penX_h += advance_k;
        }
    }
    windowWidth_f = g_BattleMenuData[windowIndex].width;
    windowHeight_a = g_BattleMenuData[windowIndex].height;
    depth_a = 0.0101f;
    if (g_MenuIsSmallViewport320x240 != 1) {
        if (Menu_IsTextSubsystemReady() != 0 && g_BattleIsPaused == 0) {
            if (AcquireBatchNode(8, g_pBattle3dScene->frame2x) != 0) {
                dv_j = 0.03125f;
                du_j = dv_j;
                u_e = 0.0f;
                v_e = 0.8125f;
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 0, (float)windowX_n, (float)windowY_m, depth_a, 0x80FFFFFF, u_e, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 1, (float)windowX_n, (float)windowY_m + 8.0f, depth_a, 0x80FFFFFF, u_e, v_e + dv_j);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 2, (float)windowX_n + 8.0f, (float)windowY_m, depth_a, 0x80FFFFFF, u_e + du_j, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 3, (float)windowX_n + 8.0f, (float)windowY_m + 8.0f, depth_a, 0x80FFFFFF, u_e + du_j, v_e + dv_j);
                *g_pBattle3dScene->frame2x->palette = 0;
                g_pBattle3dScene->frame2x->paletteIndex = 0;
                WINDOW_NEXT_QUAD(g_pBattle3dScene->frame2x);
                g_pBattle3dScene->frame2x->palette++;
                u_e = 0.09375f;
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 0, (float)windowX_n + windowWidth_f - 8.0f, (float)windowY_m, depth_a, 0x80FFFFFF, u_e, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 1, (float)windowX_n + windowWidth_f - 8.0f, (float)windowY_m + 8.0f, depth_a, 0x80FFFFFF, u_e, v_e + dv_j);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 2, (float)windowX_n + windowWidth_f - 8.0f + 8.0f, (float)windowY_m, depth_a, 0x80FFFFFF, u_e + du_j, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 3, (float)windowX_n + windowWidth_f - 8.0f + 8.0f, (float)windowY_m + 8.0f, depth_a, 0x80FFFFFF, u_e + du_j, v_e + dv_j);
                *g_pBattle3dScene->frame2x->palette = 0;
                g_pBattle3dScene->frame2x->paletteIndex = 0;
                WINDOW_NEXT_QUAD(g_pBattle3dScene->frame2x);
                g_pBattle3dScene->frame2x->palette++;
                u_e = 0.125f;
                v_e = 0.78125f;
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 0, (float)windowX_n, (float)windowY_m + windowHeight_a - 8.0f, depth_a, 0x80FFFFFF, u_e, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 1, (float)windowX_n, (float)windowY_m + windowHeight_a - 8.0f + 8.0f, depth_a, 0x80FFFFFF, u_e, v_e + dv_j);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 2, (float)windowX_n + 8.0f, (float)windowY_m + windowHeight_a - 8.0f, depth_a, 0x80FFFFFF, u_e + du_j, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 3, (float)windowX_n + 8.0f, (float)windowY_m + windowHeight_a - 8.0f + 8.0f, depth_a, 0x80FFFFFF, u_e + du_j, v_e + dv_j);
                *g_pBattle3dScene->frame2x->palette = 0;
                g_pBattle3dScene->frame2x->paletteIndex = 0;
                WINDOW_NEXT_QUAD(g_pBattle3dScene->frame2x);
                g_pBattle3dScene->frame2x->palette++;
                u_e = 0.21875f;
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 0, (float)windowX_n + windowWidth_f - 8.0f, (float)windowY_m + windowHeight_a - 8.0f, depth_a, 0x80FFFFFF, u_e, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 1, (float)windowX_n + windowWidth_f - 8.0f, (float)windowY_m + windowHeight_a - 8.0f + 8.0f, depth_a, 0x80FFFFFF, u_e, v_e + dv_j);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 2, (float)windowX_n + windowWidth_f - 8.0f + 8.0f, (float)windowY_m + windowHeight_a - 8.0f, depth_a, 0x80FFFFFF, u_e + du_j, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 3, (float)windowX_n + windowWidth_f - 8.0f + 8.0f, (float)windowY_m + windowHeight_a - 8.0f + 8.0f, depth_a, 0x80FFFFFF, u_e + du_j, v_e + dv_j);
                *g_pBattle3dScene->frame2x->palette = 0;
                g_pBattle3dScene->frame2x->paletteIndex = 0;
                WINDOW_NEXT_QUAD(g_pBattle3dScene->frame2x);
                g_pBattle3dScene->frame2x->palette++;
                u_e = 0.0f;
                v_e = 0.75f;
                du_j = 0.0625f;
                dv_j = 0.03125f;
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 0, (float)windowX_n + 8.0f, (float)windowY_m, depth_a, 0x80FFFFFF, u_e, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 1, (float)windowX_n + 8.0f, (float)windowY_m + 8.0f, depth_a, 0x80FFFFFF, u_e, v_e + dv_j);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 2, (float)windowX_n + 8.0f + windowWidth_f - 16.0f, (float)windowY_m, depth_a, 0x80FFFFFF, u_e + du_j, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 3, (float)windowX_n + 8.0f + windowWidth_f - 16.0f, (float)windowY_m + 8.0f, depth_a, 0x80FFFFFF, u_e + du_j, v_e + dv_j);
                *g_pBattle3dScene->frame2x->palette = 0;
                g_pBattle3dScene->frame2x->paletteIndex = 0;
                WINDOW_NEXT_QUAD(g_pBattle3dScene->frame2x);
                g_pBattle3dScene->frame2x->palette++;
                u_e = 0.125f;
                v_e = 0.84375f;
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 0, (float)windowX_n + 8.0f, (float)windowY_m + windowHeight_a - 8.0f, depth_a, 0x80FFFFFF, u_e, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 1, (float)windowX_n + 8.0f, (float)windowY_m + windowHeight_a - 8.0f + 8.0f, depth_a, 0x80FFFFFF, u_e, v_e + dv_j);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 2, (float)windowX_n + 8.0f + windowWidth_f - 16.0f, (float)windowY_m + windowHeight_a - 8.0f, depth_a, 0x80FFFFFF, u_e + du_j, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 3, (float)windowX_n + 8.0f + windowWidth_f - 16.0f, (float)windowY_m + windowHeight_a - 8.0f + 8.0f, depth_a, 0x80FFFFFF, u_e + du_j, v_e + dv_j);
                *g_pBattle3dScene->frame2x->palette = 0;
                g_pBattle3dScene->frame2x->paletteIndex = 0;
                WINDOW_NEXT_QUAD(g_pBattle3dScene->frame2x);
                g_pBattle3dScene->frame2x->palette++;
                u_e = 0.21875f;
                v_e = 0.875f;
                du_j = 0.03125f;
                dv_j = 0.0625f;
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 0, (float)windowX_n + windowWidth_f - 8.0f, (float)windowY_m + 8.0f, depth_a, 0x80FFFFFF, u_e, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 1, (float)windowX_n + windowWidth_f - 8.0f, (float)windowY_m + 8.0f + windowHeight_a - 16.0f, depth_a, 0x80FFFFFF, u_e, v_e + dv_j);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 2, (float)windowX_n + windowWidth_f - 8.0f + 8.0f, (float)windowY_m + 8.0f, depth_a, 0x80FFFFFF, u_e + du_j, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 3, (float)windowX_n + windowWidth_f - 8.0f + 8.0f, (float)windowY_m + 8.0f + windowHeight_a - 16.0f, depth_a, 0x80FFFFFF, u_e + du_j, v_e + dv_j);
                *g_pBattle3dScene->frame2x->palette = 0;
                g_pBattle3dScene->frame2x->paletteIndex = 0;
                WINDOW_NEXT_QUAD(g_pBattle3dScene->frame2x);
                g_pBattle3dScene->frame2x->palette++;
                u_e = 0.0f;
                v_e = 0.875f;
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 0, (float)windowX_n, (float)windowY_m + 8.0f, depth_a, 0x80FFFFFF, u_e, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 1, (float)windowX_n, (float)windowY_m + 8.0f + windowHeight_a - 16.0f, depth_a, 0x80FFFFFF, u_e, v_e + dv_j);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 2, (float)windowX_n + 8.0f, (float)windowY_m + 8.0f, depth_a, 0x80FFFFFF, u_e + du_j, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame2x, 3, (float)windowX_n + 8.0f, (float)windowY_m + 8.0f + windowHeight_a - 16.0f, depth_a, 0x80FFFFFF, u_e + du_j, v_e + dv_j);
                *g_pBattle3dScene->frame2x->palette = 0;
                g_pBattle3dScene->frame2x->paletteIndex = 0;
            }
        }
        windowWidth_f = g_BattleMenuData[windowIndex].width - 12;
        windowHeight_a = g_BattleMenuData[windowIndex].height - 12;
        if (emphasis_d != 0) {
            if (Menu_IsTextSubsystemReady() != 0 && g_BattleIsPaused == 0) {
                if (AcquireBatchNode(1, g_pBattle3dScene->emphasis) != 0) {
                    WINDOW_SOLID_VERTEX(g_pBattle3dScene->emphasis, 0, (float)windowX_n + 6.0f, (float)windowY_m + 6.0f, depth_a, 0x90700000);
                    WINDOW_SOLID_VERTEX(g_pBattle3dScene->emphasis, 1, (float)windowX_n + 6.0f, (float)windowY_m + 6.0f + g_BattleMenuData[windowIndex].height - 12.0f, depth_a, 0x90700000);
                    WINDOW_SOLID_VERTEX(g_pBattle3dScene->emphasis, 2, (float)windowX_n + 6.0f + g_BattleMenuData[windowIndex].width - 12.0f, (float)windowY_m + 6.0f, depth_a, 0x90700000);
                    WINDOW_SOLID_VERTEX(g_pBattle3dScene->emphasis, 3, (float)windowX_n + 6.0f + g_BattleMenuData[windowIndex].width - 12.0f, (float)windowY_m + 6.0f + g_BattleMenuData[windowIndex].height - 12.0f, depth_a, 0x90700000);
                }
            }
        }
        if (Menu_IsTextSubsystemReady() != 0 && g_BattleIsPaused == 0) {
            if (AcquireBatchNode(1, g_pBattle3dScene->background) != 0) {
                WINDOW_SOLID_VERTEX(g_pBattle3dScene->background, 0, (float)windowX_n + 6.0f, (float)windowY_m + 6.0f, depth_a, *(int *)&g_MenuCornerColorTL);
                WINDOW_SOLID_VERTEX(g_pBattle3dScene->background, 1, (float)windowX_n + 6.0f, (float)windowY_m + windowHeight_a + 6.0f, depth_a, *(int *)&g_MenuCornerColorTR);
                WINDOW_SOLID_VERTEX(g_pBattle3dScene->background, 2, (float)windowX_n + windowWidth_f + 6.0f, (float)windowY_m + 6.0f, depth_a, *(int *)&g_MenuCornerColorBL);
                WINDOW_SOLID_VERTEX(g_pBattle3dScene->background, 3, (float)windowX_n + windowWidth_f + 6.0f, (float)windowY_m + windowHeight_a + 6.0f, depth_a, *(int *)&g_MenuCornerColorBR);
            }
        }
    } else {
        if (Menu_IsTextSubsystemReady() != 0 && g_BattleIsPaused == 0) {
            if (AcquireBatchNode(8, g_pBattle3dScene->frame1x) != 0) {
                dv_j = 0.015625f;
                du_j = dv_j;
                u_e = 0.0f;
                v_e = 0.90625f;
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 0, (float)windowX_n + originX_p, (float)windowY_m + originY_l, depth_a, 0x80FFFFFF, u_e, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 1, (float)windowX_n + originX_p, (float)windowY_m + 4.0f + originY_l, depth_a, 0x80FFFFFF, u_e, v_e + dv_j);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 2, (float)windowX_n + 4.0f + originX_p, (float)windowY_m + originY_l, depth_a, 0x80FFFFFF, u_e + du_j, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 3, (float)windowX_n + 4.0f + originX_p, (float)windowY_m + 4.0f + originY_l, depth_a, 0x80FFFFFF, u_e + du_j, v_e + dv_j);
                *g_pBattle3dScene->frame1x->palette = 0;
                g_pBattle3dScene->frame1x->paletteIndex = 0;
                WINDOW_NEXT_QUAD(g_pBattle3dScene->frame1x);
                g_pBattle3dScene->frame1x->palette++;
                u_e = 0.046875f;
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 0, (float)windowX_n + windowWidth_f - 4.0f + originX_p, (float)windowY_m + originY_l, depth_a, 0x80FFFFFF, u_e, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 1, (float)windowX_n + windowWidth_f - 4.0f + originX_p, (float)windowY_m + 4.0f + originY_l, depth_a, 0x80FFFFFF, u_e, v_e + dv_j);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 2, (float)windowX_n + windowWidth_f - 4.0f + 4.0f + originX_p, (float)windowY_m + originY_l, depth_a, 0x80FFFFFF, u_e + du_j, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 3, (float)windowX_n + windowWidth_f - 4.0f + 4.0f + originX_p, (float)windowY_m + 4.0f + originY_l, depth_a, 0x80FFFFFF, u_e + du_j, v_e + dv_j);
                *g_pBattle3dScene->frame1x->palette = 0;
                g_pBattle3dScene->frame1x->paletteIndex = 0;
                WINDOW_NEXT_QUAD(g_pBattle3dScene->frame1x);
                g_pBattle3dScene->frame1x->palette++;
                u_e = 0.0625f;
                v_e = 0.890625f;
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 0, (float)windowX_n + originX_p, (float)windowY_m + windowHeight_a - 4.0f + originY_l, depth_a, 0x80FFFFFF, u_e, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 1, (float)windowX_n + originX_p, (float)windowY_m + windowHeight_a - 4.0f + 4.0f + originY_l, depth_a, 0x80FFFFFF, u_e, v_e + dv_j);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 2, (float)windowX_n + 4.0f + originX_p, (float)windowY_m + windowHeight_a - 4.0f + originY_l, depth_a, 0x80FFFFFF, u_e + du_j, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 3, (float)windowX_n + 4.0f + originX_p, (float)windowY_m + windowHeight_a - 4.0f + 4.0f + originY_l, depth_a, 0x80FFFFFF, u_e + du_j, v_e + dv_j);
                *g_pBattle3dScene->frame1x->palette = 0;
                g_pBattle3dScene->frame1x->paletteIndex = 0;
                WINDOW_NEXT_QUAD(g_pBattle3dScene->frame1x);
                g_pBattle3dScene->frame1x->palette++;
                u_e = 0.109375f;
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 0, (float)windowX_n + windowWidth_f - 4.0f + originX_p, (float)windowY_m + windowHeight_a - 4.0f + originY_l, depth_a, 0x80FFFFFF, u_e, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 1, (float)windowX_n + windowWidth_f - 4.0f + originX_p, (float)windowY_m + windowHeight_a - 4.0f + 4.0f + originY_l, depth_a, 0x80FFFFFF, u_e, v_e + dv_j);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 2, (float)windowX_n + windowWidth_f - 4.0f + 4.0f + originX_p, (float)windowY_m + windowHeight_a - 4.0f + originY_l, depth_a, 0x80FFFFFF, u_e + du_j, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 3, (float)windowX_n + windowWidth_f - 4.0f + 4.0f + originX_p, (float)windowY_m + windowHeight_a - 4.0f + 4.0f + originY_l, depth_a, 0x80FFFFFF, u_e + du_j, v_e + dv_j);
                *g_pBattle3dScene->frame1x->palette = 0;
                g_pBattle3dScene->frame1x->paletteIndex = 0;
                WINDOW_NEXT_QUAD(g_pBattle3dScene->frame1x);
                g_pBattle3dScene->frame1x->palette++;
                u_e = 0.0f;
                v_e = 0.875f;
                du_j = 0.03125f;
                dv_j = 0.015625f;
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 0, (float)windowX_n + 4.0f + originX_p, (float)windowY_m + originY_l, depth_a, 0x80FFFFFF, u_e, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 1, (float)windowX_n + 4.0f + originX_p, (float)windowY_m + 4.0f + originY_l, depth_a, 0x80FFFFFF, u_e, v_e + dv_j);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 2, (float)windowX_n + 4.0f + windowWidth_f - 8.0f + originX_p, (float)windowY_m + originY_l, depth_a, 0x80FFFFFF, u_e + du_j, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 3, (float)windowX_n + 4.0f + windowWidth_f - 8.0f + originX_p, (float)windowY_m + 4.0f + originY_l, depth_a, 0x80FFFFFF, u_e + du_j, v_e + dv_j);
                *g_pBattle3dScene->frame1x->palette = 0;
                g_pBattle3dScene->frame1x->paletteIndex = 0;
                WINDOW_NEXT_QUAD(g_pBattle3dScene->frame1x);
                g_pBattle3dScene->frame1x->palette++;
                u_e = 0.0625f;
                v_e = 0.921875f;
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 0, (float)windowX_n + 4.0f + originX_p, (float)windowY_m + windowHeight_a - 4.0f + originY_l, depth_a, 0x80FFFFFF, u_e, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 1, (float)windowX_n + 4.0f + originX_p, (float)windowY_m + windowHeight_a - 4.0f + 4.0f + originY_l, depth_a, 0x80FFFFFF, u_e, v_e + dv_j);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 2, (float)windowX_n + 4.0f + windowWidth_f - 8.0f + originX_p, (float)windowY_m + windowHeight_a - 4.0f + originY_l, depth_a, 0x80FFFFFF, u_e + du_j, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 3, (float)windowX_n + 4.0f + windowWidth_f - 8.0f + originX_p, (float)windowY_m + windowHeight_a - 4.0f + 4.0f + originY_l, depth_a, 0x80FFFFFF, u_e + du_j, v_e + dv_j);
                *g_pBattle3dScene->frame1x->palette = 0;
                g_pBattle3dScene->frame1x->paletteIndex = 0;
                WINDOW_NEXT_QUAD(g_pBattle3dScene->frame1x);
                g_pBattle3dScene->frame1x->palette++;
                u_e = 0.109375f;
                v_e = 0.9375f;
                du_j = 0.015625f;
                dv_j = 0.03125f;
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 0, (float)windowX_n + windowWidth_f - 4.0f + originX_p, (float)windowY_m + 4.0f + originY_l, depth_a, 0x80FFFFFF, u_e, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 1, (float)windowX_n + windowWidth_f - 4.0f + originX_p, (float)windowY_m + 4.0f + windowHeight_a - 8.0f + originY_l, depth_a, 0x80FFFFFF, u_e, v_e + dv_j);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 2, (float)windowX_n + windowWidth_f - 4.0f + 4.0f + originX_p, (float)windowY_m + 4.0f + originY_l, depth_a, 0x80FFFFFF, u_e + du_j, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 3, (float)windowX_n + windowWidth_f - 4.0f + 4.0f + originX_p, (float)windowY_m + 4.0f + windowHeight_a - 8.0f + originY_l, depth_a, 0x80FFFFFF, u_e + du_j, v_e + dv_j);
                *g_pBattle3dScene->frame1x->palette = 0;
                g_pBattle3dScene->frame1x->paletteIndex = 0;
                WINDOW_NEXT_QUAD(g_pBattle3dScene->frame1x);
                g_pBattle3dScene->frame1x->palette++;
                u_e = 0.0f;
                v_e = 0.921875f;
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 0, (float)windowX_n + originX_p, (float)windowY_m + 4.0f + originY_l, depth_a, 0x80FFFFFF, u_e, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 1, (float)windowX_n + originX_p, (float)windowY_m + 4.0f + windowHeight_a - 8.0f + originY_l, depth_a, 0x80FFFFFF, u_e, v_e + dv_j);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 2, (float)windowX_n + 4.0f + originX_p, (float)windowY_m + 4.0f + originY_l, depth_a, 0x80FFFFFF, u_e + du_j, v_e);
                WINDOW_TEXTURED_VERTEX(g_pBattle3dScene->frame1x, 3, (float)windowX_n + 4.0f + originX_p, (float)windowY_m + 4.0f + windowHeight_a - 8.0f + originY_l, depth_a, 0x80FFFFFF, u_e + du_j, v_e + dv_j);
                *g_pBattle3dScene->frame1x->palette = 0;
                g_pBattle3dScene->frame1x->paletteIndex = 0;
            }
        }
        windowWidth_f = g_BattleMenuData[windowIndex].width - 6;
        windowHeight_a = g_BattleMenuData[windowIndex].height - 6;
        if (emphasis_d != 0) {
            if (Menu_IsTextSubsystemReady() != 0 && g_BattleIsPaused == 0) {
                if (AcquireBatchNode(1, g_pBattle3dScene->emphasis) != 0) {
                    WINDOW_SOLID_VERTEX(g_pBattle3dScene->emphasis, 0, (float)windowX_n + 3.0f + originX_p, (float)windowY_m + 3.0f + originY_l, depth_a, 0x90700000);
                    WINDOW_SOLID_VERTEX(g_pBattle3dScene->emphasis, 1, (float)windowX_n + 3.0f + originX_p, (float)windowY_m + 3.0f + g_BattleMenuData[windowIndex].height - 6.0f + originY_l, depth_a, 0x90700000);
                    WINDOW_SOLID_VERTEX(g_pBattle3dScene->emphasis, 2, (float)windowX_n + 3.0f + g_BattleMenuData[windowIndex].width - 6.0f + originX_p, (float)windowY_m + 3.0f + originY_l, depth_a, 0x90700000);
                    WINDOW_SOLID_VERTEX(g_pBattle3dScene->emphasis, 3, (float)windowX_n + 3.0f + g_BattleMenuData[windowIndex].width - 6.0f + originX_p, (float)windowY_m + 3.0f + g_BattleMenuData[windowIndex].height - 6.0f + originY_l, depth_a, 0x90700000);
                }
            }
        }
        if (Menu_IsTextSubsystemReady() != 0 && g_BattleIsPaused == 0) {
            if (AcquireBatchNode(1, g_pBattle3dScene->background) != 0) {
                WINDOW_SOLID_VERTEX(g_pBattle3dScene->background, 0, (float)windowX_n + 3.0f + originX_p, (float)windowY_m + 3.0f + originY_l, depth_a, *(int *)&g_MenuCornerColorTL);
                WINDOW_SOLID_VERTEX(g_pBattle3dScene->background, 1, (float)windowX_n + 3.0f + originX_p, (float)windowY_m + windowHeight_a + 3.0f + originY_l, depth_a, *(int *)&g_MenuCornerColorTR);
                WINDOW_SOLID_VERTEX(g_pBattle3dScene->background, 2, (float)windowX_n + windowWidth_f + 3.0f + originX_p, (float)windowY_m + 3.0f + originY_l, depth_a, *(int *)&g_MenuCornerColorBL);
                WINDOW_SOLID_VERTEX(g_pBattle3dScene->background, 3, (float)windowX_n + windowWidth_f + 3.0f + originX_p, (float)windowY_m + windowHeight_a + 3.0f + originY_l, depth_a, *(int *)&g_MenuCornerColorBR);
            }
        }
    }
}

#undef WINDOW_TEXTURED_VERTEX
#undef WINDOW_SOLID_VERTEX

/*
 * Formats five divisor-table positions from value into g_BtlMenuGilAmountText,
 * suppressing leading zero positions entirely. Once a nonzero quotient appears,
 * emits subsequent zeroes as glyph 0x10 and nonzero quotients as quotient+0x10.
 */
// FUNCTION: FF7 0x006d70f1
void __cdecl Battle_FormatWindowGilAmountText(int value)
{
    unsigned char blanking;
    unsigned char out;
    int i;
    int num;

    out = 0;
    blanking = 1;
    for (i = 0; i < 5; i++) {
        num = value / g_BtlMenuGilDigitDivisors[i];
        if (num == 0) {
            if (blanking == 0) {
                g_BtlMenuGilAmountText[out] = 16;
                out++;
            }
        } else {
            blanking = 0;
            g_BtlMenuGilAmountText[out] = (unsigned char)(num + 16);
            out++;
        }
        value = value % g_BtlMenuGilDigitDivisors[i];
    }
    g_BtlMenuGilAmountText[out] = 16;
    out++;
    for (i = 0; i < 5; i++) {
        g_BtlMenuGilAmountText[out] = (&g_MenuMainLabels.gilText)[i];
        out++;
    }
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006d71f5
void __cdecl Battle_PartyMemberAvailableHookNoOp(unsigned short actor)
{
    /* The original body is empty. */
}

/*
 * Request a battle message window at (x, y): the pending flag, then the two
 * coordinates, all three sixteen bits wide.
 */
// FUNCTION: FF7 0x006d71fa
void __cdecl Battle_RequestMessageWindowAt(short x, short y)
{
    g_BtlMenuMessageWindowAtRequest = 1;
    g_BtlMenuMessageWindowAtX = x;
    g_BtlMenuMessageWindowAtY = y;
}

/*
 * Sets the message shown in the battle's message window (window 0x19): `textId` is an entry of
 * the kernel's battle message text (KERNEL_TEXT_BATTLE_MESSAGE), which Menu_DrawBattleWindowSet
 * looks up and draws every frame while the request flag is set. -1 clears the flag and so hides
 * the window. It holds one message, so a new id replaces the last. Battle code shows the queued
 * display text, message 0x36 and the limit banner text with it, and clears it when a text ends,
 * when a scene round loads and at the end of battle.
 */
// FUNCTION: FF7 0x006d721c
void __cdecl Battle_SetMessageWindowRequest(short textId)
{
    if (textId == -1) {
        g_BtlMenuMessageWindowRequest = 0;
    } else {
        g_BtlMenuMessageWindowRequest = 1;
        g_BtlMenuMessageWindowTextId = textId;
    }
}

/*
 * Draws requested battle-message and contextual help windows. With flag 0x40,
 * calls Battle_DrawTargetedEnemyMarker for target-mask bits four through eleven.
 */
// FUNCTION: FF7 0x006d7245
void __cdecl Menu_DrawBattleWindowSet(int layer)
{
    // Preserve the original unused frame area; switch adds its own final slot.
    struct {
        BattleAbilitySlot *entry;
        unsigned char *limit;
        int unused12c;
        BattleInventorySlot *items;
        int unused124;
        int i;
        unsigned char unused[0x118];
        struct MenuPartyStats *stats;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if (gSavemap.tutSub & 0x40) {
        for (frame.i = 4; frame.i < 12; frame.i++) {
            if ((unsigned short)g_BattleScene.targetExtraMask & (1 << frame.i)) {
                Battle_DrawTargetedEnemyMarker(frame.i);
            }
        }
    }
    if (g_BtlMenuMessageWindowRequest != 0) {
        g_BattleTextBuffer = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, (unsigned short)g_BtlMenuMessageWindowTextId, 8);
        Menu_DrawBattleWindow(layer, BATTLE_WINDOW_MESSAGE, 0, 0, 0);
    } else if (g_BtlMenuMessageWindowAtRequest != 0) {
        /* Retail caller prototype (int, short, unsigned char, short, int) -> void preserved for /Od argument promotion on `Menu_DrawBattleWindow`. */
        ((void(__cdecl *)(int, short, unsigned char, short, int))Menu_DrawBattleWindow)(
            layer, BATTLE_WINDOW_ACTION_NAME, g_BtlMenuMessageWindowAtX, g_BtlMenuMessageWindowAtY, 0);
        g_BtlMenuMessageWindowAtRequest = 0;
    }
    if (gSavemap.options & 0x4000) {
        g_BattleTextBuffer = (unsigned char *)&g_BtlMenuEmptyText;
        switch (g_BattleMenuState) {
        case BATTLE_MENU_COMMAND_MENU:
            if (g_BattleWindowStates[BATTLE_WINDOW_COMMAND_MENU] != BATTLE_WINDOW_STATE_OPEN)
                break;
            frame.stats = &g_MenuPartyStats[0];
            g_BattleTextBuffer = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND,
                                                             (frame.stats + g_BtlActiveActor)->commands[((BattleMenuWindowSet *)g_BtlMenuWindowSets)[g_BtlActiveActor].cursorIndex * 4 + *(int *)(((unsigned char *)&g_BtlMenuSlotRecords) + g_BtlActiveActor * 0x700)].commandId,
                                                             0);
            break;
        case BATTLE_MENU_CHANGE_CONFIRM:
            if (g_BattleWindowStates[BATTLE_WINDOW_CHANGE] != BATTLE_WINDOW_STATE_OPEN)
                break;
            g_BattleTextBuffer = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, BATTLE_COMMAND_CHANGE, 0);
            break;
        case BATTLE_MENU_DEFEND_CONFIRM:
            if (g_BattleWindowStates[BATTLE_WINDOW_DEFEND] != BATTLE_WINDOW_STATE_OPEN)
                break;
            g_BattleTextBuffer = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, BATTLE_COMMAND_DEFEND, 0);
            break;
        case BATTLE_MENU_ITEM_LIST:
            if (g_BattleWindowStates[BATTLE_WINDOW_ITEM_LIST] != BATTLE_WINDOW_STATE_OPEN)
                break;
            frame.items = g_BattleItemListBlock.slots;
            if (frame.items[((BtlMenuActorListCursorStride *)(&g_BtlMenuSlotTableA[0x4]))[g_BtlActiveActor].row + ((BtlMenuActorListCursorStride *)(&g_BtlMenuSlotTableA[0x4]))[g_BtlActiveActor].scrollTop].id != 0xffff) {
                g_BattleTextBuffer = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM,
                                                                 frame.items[((BtlMenuActorListCursorStride *)(&g_BtlMenuSlotTableA[0x4]))[g_BtlActiveActor].row + ((BtlMenuActorListCursorStride *)(&g_BtlMenuSlotTableA[0x4]))[g_BtlActiveActor].scrollTop].id,
                                                                 0);
            }
            break;
        case BATTLE_MENU_MAGIC_LIST:
            if (g_BattleWindowStates[BATTLE_WINDOW_MAGIC_LIST] != BATTLE_WINDOW_STATE_OPEN)
                break;
            frame.entry = g_MenuPartyStats[g_BtlActiveActor].spells;
            if (frame.entry[((BtlMenuActorFieldStride *)g_BtlMenuSlotTableB)[g_BtlActiveActor].value +
                         ((BtlMenuActorFieldStride *)g_BtlMenuSlotTableC)[g_BtlActiveActor].value * 3 +
                         ((BtlMenuActorFieldStride *)g_BtlMenuWindowRecordsB)[g_BtlActiveActor].value * 3].attackId != 0xff) {
                g_BattleTextBuffer = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_MAGIC,
                                                                 frame.entry[((BtlMenuActorFieldStride *)g_BtlMenuSlotTableB)[g_BtlActiveActor].value +
                                                                          ((BtlMenuActorFieldStride *)g_BtlMenuSlotTableC)[g_BtlActiveActor].value * 3 +
                                                                          ((BtlMenuActorFieldStride *)g_BtlMenuWindowRecordsB)[g_BtlActiveActor].value * 3].attackId,
                                                                 0);
            }
            break;
        case BATTLE_MENU_SUMMON_LIST:
            if (g_BattleWindowStates[BATTLE_WINDOW_SUMMON_LIST] != BATTLE_WINDOW_STATE_OPEN)
                break;
            frame.entry = g_MenuPartyStats[g_BtlActiveActor].summons;
            if (frame.entry[((BattleMenuWindow *)g_BtlMenuWindowRecordsC)[g_BtlActiveActor].entryIndex +
                         ((BtlMenuActorFieldStride *)g_BtlMenuSlotTableD)[g_BtlActiveActor].value +
                         ((BtlMenuActorFieldStride *)g_BtlMenuWindowRecordsD)[g_BtlActiveActor].value].attackId != 0xff) {
                g_BattleTextBuffer = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_SUMMON,
                                                                 frame.entry[((BattleMenuWindow *)g_BtlMenuWindowRecordsC)[g_BtlActiveActor].entryIndex +
                                                                          ((BtlMenuActorFieldStride *)g_BtlMenuSlotTableD)[g_BtlActiveActor].value +
                                                                          ((BtlMenuActorFieldStride *)g_BtlMenuWindowRecordsD)[g_BtlActiveActor].value].attackId,
                                                                 0);
            }
            break;
        case BATTLE_MENU_ENEMY_SKILL_LIST:
            if (g_BattleWindowStates[BATTLE_WINDOW_ENEMY_SKILL_LIST] != BATTLE_WINDOW_STATE_OPEN)
                break;
            frame.entry = g_MenuPartyStats[g_BtlActiveActor].enemySkills;
            if (frame.entry[((BattleMenuWindow *)g_BtlMenuWindows)[g_BtlActiveActor].entryIndex +
                         ((BattleMenuCursor *)g_BtlMenuCursors)[g_BtlActiveActor].row * 2 +
                         ((BtlMenuActorFieldStride *)g_BtlMenuWindowRecords)[g_BtlActiveActor].value * 2].attackId != 0xff) {
                g_BattleTextBuffer = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ENEMY_SKILL,
                                                                 frame.entry[((BattleMenuWindow *)g_BtlMenuWindows)[g_BtlActiveActor].entryIndex +
                                                                          ((BattleMenuCursor *)g_BtlMenuCursors)[g_BtlActiveActor].row * 2 +
                                                                          ((BtlMenuActorFieldStride *)g_BtlMenuWindowRecords)[g_BtlActiveActor].value * 2].attackId,
                                                                 0);
            }
            break;
        case BATTLE_MENU_LIMIT_LIST:
            if (g_BattleWindowStates[BATTLE_WINDOW_LIMIT_LIST] != BATTLE_WINDOW_STATE_OPEN)
                break;
            frame.limit = g_MenuPartyStats[g_BtlActivePartyMember].limitAttacks;
            g_BattleTextBuffer = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_LIMIT,
                                                             frame.limit[((BattleMenuCursor *)(&g_BtlMenuLimitCursor[4]))[g_BtlActivePartyMember].row], 0);
            break;
        case BATTLE_MENU_GIL_AMOUNT:
            if (g_BattleWindowStates[BATTLE_WINDOW_GIL_AMOUNT] != BATTLE_WINDOW_STATE_OPEN)
                break;
            frame.stats = &g_MenuPartyStats[0];
            g_BattleTextBuffer = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND,
                                                             (frame.stats + g_BtlActiveActor)->commands[((BattleMenuWindowSet *)g_BtlMenuWindowSets)[g_BtlActiveActor].cursorIndex * 4 + *(int *)(((unsigned char *)&g_BtlMenuSlotRecords) + g_BtlActiveActor * 0x700)].commandId,
                                                             0);
            break;
        case BATTLE_MENU_SELECT_TARGET:
            if (g_BtlPointedTargetInvalid == 0 && g_BattleScriptEffectFlag == 0) {
                if (g_BtlTargetSelectMode & BTL_TARGET_MULTIPLE) {
                    if (g_BtlPointedTarget > 2) {
                        g_BattleTextBuffer = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 6, 8);
                    } else if (g_BtlPointedTarget < 3) {
                        g_BattleTextBuffer = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 5, 8);
                    }
                } else {
                    if (g_BtlPointedTarget > 2) {
                        g_BattleTextBuffer = (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ENEMY_NAME, g_BtlPointedTarget - 4, 8);
                    } else if (g_BtlPointedTarget < 3) {
                        g_BattleTextBuffer = (unsigned char *)&gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[g_BtlPointedTarget]]].name;
                    }
                }
            }
            break;
        }
        Menu_DrawBattleWindow(layer, BATTLE_WINDOW_HELP, 0, 0, 0);
    }
}

/*
 * Dispatches the selected battle-menu panel to its 2x renderer, including
 * item/limit/party HUD and slot panels. The Change and Defend windows draw their command
 * names; Change is drawn in colour 0 (greyed) in the side, pincer and locked-row layouts, where
 * it cannot be chosen.
 */
// FUNCTION: FF7 0x006d797c
void __cdecl Battle_DrawWindowPanelContent2x(int *slot, short arg)
{
    int color;

    switch (arg) {
    case BATTLE_WINDOW_PARTY_STATUS:
        Battle_DrawPartyStatusOverview2x();
        break;
    case BATTLE_WINDOW_COMMAND_MENU:
        Battle_DrawActorCommandGrid2x(&g_BtlMenuWindowSets[g_BtlActiveActor * 0x700]);
        break;
    case BATTLE_WINDOW_ITEM_LIST:
        Menu_DrawBattleItemList2x();
        break;
    case BATTLE_WINDOW_MAGIC_LIST:
        Menu_DrawBattleMagicList2x();
        break;
    case BATTLE_WINDOW_ENEMY_SKILL_LIST:
        Menu_DrawBattleEnemySkillList2x();
        break;
    case BATTLE_WINDOW_SUMMON_LIST:
        Menu_DrawBattleSummonList2x();
        break;
    case BATTLE_WINDOW_CHANGE:
        switch (g_BattleScene.layout) {
        case BATTLE_LAYOUT_SIDE_ATTACK_1:
        case BATTLE_LAYOUT_PINCER_ATTACK:
        case BATTLE_LAYOUT_SIDE_ATTACK_2:
        case BATTLE_LAYOUT_SIDE_ATTACK_3:
        case BATTLE_LAYOUT_SIDE_ATTACK_4:
        case BATTLE_LAYOUT_FRONT_ROW_LOCKED:
            color = 0;
            break;
        default:
            color = 7;
            break;
        }
        Menu_DrawEncodedString2x(
            g_BattleMenuData[arg].x + g_BattleMenuData[arg].sub[0].left + 0x10,
            g_BattleMenuData[arg].y + g_BattleMenuData[arg].sub[0].top + 0xe,
            (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, BATTLE_COMMAND_CHANGE, 8), color, 0.401f);
        break;
    case BATTLE_WINDOW_DEFEND:
        Menu_DrawEncodedString2x(
            g_BattleMenuData[arg].x + g_BattleMenuData[arg].sub[0].left + 0x12,
            g_BattleMenuData[arg].y + g_BattleMenuData[arg].sub[0].top + 0xe,
            (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, BATTLE_COMMAND_DEFEND, 8), 7, 0.401f);
        break;
    case BATTLE_WINDOW_LIMIT_LIST:
        Menu_DrawBattleLimitList2x();
        break;
    case BATTLE_WINDOW_GIL_AMOUNT:
        Battle_DrawGilAmountPrompt2x();
        break;
    case BATTLE_WINDOW_PARTY_HUD:
        Battle_DrawPartyHudRows2x();
        break;
    case BATTLE_WINDOW_MANIP_ACTION_LIST:
        Battle_DrawControlledEnemyActionList2x();
        break;
    case BATTLE_WINDOW_CAIT_SITH_SLOTS:
        Menu_DrawCaitReelPanel2x();
        break;
    case BATTLE_WINDOW_TIFA_SLOTS:
        Menu_DrawTifaReelPanel2x();
        break;
    case BATTLE_WINDOW_ARENA_SLOT:
        Mini_DrawSlotScreen();
        break;
    case BATTLE_WINDOW_ARENA_CONTINUE_CHOICE:
        Battle_DrawWorriedChoiceLines2x();
        break;
    }
    Battle_FlushMenuBatches2x();
}

/*
 * Dispatches the selected battle-menu panel to its 1x renderer, including
 * item/limit/party HUD and slot panels. The Change and Defend windows draw their command
 * names; Change is drawn in colour 0 (greyed) in the side, pincer and locked-row layouts, where
 * it cannot be chosen.
 */
// FUNCTION: FF7 0x006d7bbb
void __cdecl Battle_DrawWindowPanelContent1x(int *slot, short arg)
{
    int color;

    switch (arg) {
    case BATTLE_WINDOW_PARTY_STATUS:
        Battle_DrawPartyStatusOverview1x();
        break;
    case BATTLE_WINDOW_COMMAND_MENU:
        Battle_DrawActorCommandGrid1x(&g_BtlMenuWindowSets[g_BtlActiveActor * 0x700]);
        break;
    case BATTLE_WINDOW_ITEM_LIST:
        Menu_DrawBattleItemList1x();
        break;
    case BATTLE_WINDOW_MAGIC_LIST:
        Menu_DrawBattleMagicList1x();
        break;
    case BATTLE_WINDOW_ENEMY_SKILL_LIST:
        Menu_DrawBattleEnemySkillList1x();
        break;
    case BATTLE_WINDOW_SUMMON_LIST:
        Menu_DrawBattleSummonList1x();
        break;
    case BATTLE_WINDOW_CHANGE:
        switch (g_BattleScene.layout) {
        case BATTLE_LAYOUT_SIDE_ATTACK_1:
        case BATTLE_LAYOUT_PINCER_ATTACK:
        case BATTLE_LAYOUT_SIDE_ATTACK_2:
        case BATTLE_LAYOUT_SIDE_ATTACK_3:
        case BATTLE_LAYOUT_SIDE_ATTACK_4:
        case BATTLE_LAYOUT_FRONT_ROW_LOCKED:
            color = 0;
            break;
        default:
            color = 7;
            break;
        }
        Menu_DrawEncodedString1x(
            g_BattleMenuData[arg].x + g_BattleMenuData[arg].sub[0].left + 8,
            g_BattleMenuData[arg].y + g_BattleMenuData[arg].sub[0].top + 7,
            (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, BATTLE_COMMAND_CHANGE, 8), color, 0.401f);
        break;
    case BATTLE_WINDOW_DEFEND:
        Menu_DrawEncodedString1x(
            g_BattleMenuData[arg].x + g_BattleMenuData[arg].sub[0].left + 9,
            g_BattleMenuData[arg].y + g_BattleMenuData[arg].sub[0].top + 7,
            (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_COMMAND, BATTLE_COMMAND_DEFEND, 8), 7, 0.401f);
        break;
    case BATTLE_WINDOW_LIMIT_LIST:
        Menu_DrawBattleLimitList1x();
        break;
    case BATTLE_WINDOW_GIL_AMOUNT:
        Battle_DrawGilAmountPrompt1x();
        break;
    case BATTLE_WINDOW_PARTY_HUD:
        Battle_DrawPartyHudRows1x();
        break;
    case BATTLE_WINDOW_MANIP_ACTION_LIST:
        Battle_DrawControlledEnemyActionList1x();
        break;
    case BATTLE_WINDOW_CAIT_SITH_SLOTS:
        Menu_DrawCaitReelPanel1x();
        break;
    case BATTLE_WINDOW_TIFA_SLOTS:
        Menu_DrawTifaReelPanel1x();
        break;
    case BATTLE_WINDOW_ARENA_SLOT:
        Mini_DrawSlotScreen1x();
        break;
    case BATTLE_WINDOW_ARENA_CONTINUE_CHOICE:
        Battle_DrawWorriedChoiceLines1x();
        break;
    }
    Battle_FlushMenuBatches1x();
}

/*
 * Calls Battle_FlushMenuBatches2x, then sets the viewport using the selected
 * battle box position plus g_MenuViewportX/1060 and its half extents. Uses the box
 * clipHeight except when id is zero, held input 0x80 is clear and any box 5, 6, 7
 * or 4 has state 2; then substitutes 0x14.
 */
// FUNCTION: FF7 0x006d7dfa
void __cdecl Menu_FlushAndClipToBattleWindow2x(int *rows, short id)
{
    int value;
    GfxEngineContext *engine;

    engine = GetEngineContext();
    Battle_FlushMenuBatches2x();
    if (id != 0 || Input_GetHeld(INPUT_SQUARE) != 0) {
        value = g_BattleMenuData[id].clipHeight;
    } else if (g_BattleWindowStates[BATTLE_WINDOW_ITEM_LIST] == BATTLE_WINDOW_STATE_OPEN || g_BattleWindowStates[BATTLE_WINDOW_MAGIC_LIST] == BATTLE_WINDOW_STATE_OPEN ||
               g_BattleWindowStates[BATTLE_WINDOW_SUMMON_LIST] == BATTLE_WINDOW_STATE_OPEN || g_BattleWindowStates[BATTLE_WINDOW_ENEMY_SKILL_LIST] == BATTLE_WINDOW_STATE_OPEN) {
        value = 0x14;
    } else {
        value = g_BattleMenuData[id].clipHeight;
    }
    GfxSetViewportRect(
        g_BattleMenuData[id].halfWidth + (g_MenuViewportX + g_BattleMenuData[id].x),
        g_BattleMenuData[id].halfHeight + (g_MenuViewportY + g_BattleMenuData[id].y),
        g_BattleMenuData[id].clipWidth, value, engine);
}

/*
 * Calls Battle_FlushMenuBatches1x, then sets the viewport using the selected
 * battle box position plus g_MenuViewportX/1060 and its half extents. Uses the box
 * clipHeight except when id is zero, held input 0x80 is clear and any box 5, 6, 7
 * or 4 has state 2; then substitutes 0xA.
 */
// FUNCTION: FF7 0x006d7f26
void __cdecl Menu_FlushAndClipToBattleWindow1x(int *rows, short id)
{
    int value;
    GfxEngineContext *engine;

    engine = GetEngineContext();
    Battle_FlushMenuBatches1x();
    if (id != 0 || Input_GetHeld(INPUT_SQUARE) != 0) {
        value = g_BattleMenuData[id].clipHeight;
    } else if (g_BattleWindowStates[BATTLE_WINDOW_ITEM_LIST] == BATTLE_WINDOW_STATE_OPEN || g_BattleWindowStates[BATTLE_WINDOW_MAGIC_LIST] == BATTLE_WINDOW_STATE_OPEN ||
               g_BattleWindowStates[BATTLE_WINDOW_SUMMON_LIST] == BATTLE_WINDOW_STATE_OPEN || g_BattleWindowStates[BATTLE_WINDOW_ENEMY_SKILL_LIST] == BATTLE_WINDOW_STATE_OPEN) {
        value = 0xa;
    } else {
        value = g_BattleMenuData[id].clipHeight;
    }
    GfxSetViewportRect(
        g_BattleMenuData[id].halfWidth + (g_MenuViewportX + g_BattleMenuData[id].x),
        g_BattleMenuData[id].halfHeight + (g_MenuViewportY + g_BattleMenuData[id].y),
        g_BattleMenuData[id].clipWidth, value, engine);
}

/*
 * Outline every sub-rectangle of one battle box. The sub-rectangles are stored as
 * corners, and Menu_SetRectShorts wants an origin and a size, so each pass subtracts
 * twice.
 */
// FUNCTION: FF7 0x006d8052
void __cdecl Menu_DrawBattleWindowSubRectFrames2x(int *rows, short id)
{
    int scratch[4];
    short rect[4];
    short i;
    short dw;
    short dh;

    for (i = 0; i < g_BattleMenuData[id].subCount; i++) {
        dw = g_BattleMenuData[id].sub[i].right -
             g_BattleMenuData[id].sub[i].left;
        dh = g_BattleMenuData[id].sub[i].bottom -
             g_BattleMenuData[id].sub[i].top;
        Menu_SetRectShorts(rect,
                      g_BattleMenuData[id].x + g_BattleMenuData[id].sub[i].left,
                      g_BattleMenuData[id].y + g_BattleMenuData[id].sub[i].top,
                      dw, dh);
        Menu_DrawWindowFrame2x((EnglishWindowFrame *)rect, g_BtlMenuWindowSubFrameDepth[id]);
    }
}

/*
 * Menu_DrawBattleWindowSubRectFrames2x's twin, differing in one call: Menu_DrawWindowFrame1x instead of
 * Menu_DrawWindowFrame2x. Same table of z values, same arithmetic, same frame.
 */
// FUNCTION: FF7 0x006d819e
void __cdecl Menu_DrawBattleWindowSubRectFrames1x(int *rows, short id)
{
    int scratch[4];
    short rect[4];
    short i;
    short dw;
    short dh;

    for (i = 0; i < g_BattleMenuData[id].subCount; i++) {
        dw = g_BattleMenuData[id].sub[i].right -
             g_BattleMenuData[id].sub[i].left;
        dh = g_BattleMenuData[id].sub[i].bottom -
             g_BattleMenuData[id].sub[i].top;
        Menu_SetRectShorts(rect,
                      g_BattleMenuData[id].x + g_BattleMenuData[id].sub[i].left,
                      g_BattleMenuData[id].y + g_BattleMenuData[id].sub[i].top,
                      dw, dh);
        Menu_DrawWindowFrame1x((EnglishWindowFrame *)rect, g_BtlMenuWindowSubFrameDepth[id]);
    }
}

/*
 * Draws the next window row (five ints): its clip rectangle from the first, its sub-rectangle
 * frames from the third and its panel content from the fifth, then moves on to the next row.
 */
// FUNCTION: FF7 0x006d82ea
void __cdecl Battle_DrawWindowRow2x(int *rows, short arg)
{
    Menu_FlushAndClipToBattleWindow2x(&rows[g_BtlMenuDrawWindowIndex * 5], arg);
    Menu_DrawBattleWindowSubRectFrames2x(&rows[g_BtlMenuDrawWindowIndex * 5 + 2], arg);
    Battle_DrawWindowPanelContent2x(&rows[g_BtlMenuDrawWindowIndex * 5 + 4], arg);
    g_BtlMenuDrawWindowIndex += 1;
}

/*
 * Draws the next window row (five ints): its clip rectangle from the first, its sub-rectangle
 * frames from the third and its panel content from the fifth, then moves on to the next row.
 */
// FUNCTION: FF7 0x006d8359
void __cdecl Battle_DrawWindowRow1x(int *rows, short arg)
{
    Menu_FlushAndClipToBattleWindow1x(&rows[g_BtlMenuDrawWindowIndex * 5], arg);
    Menu_DrawBattleWindowSubRectFrames1x(&rows[g_BtlMenuDrawWindowIndex * 5 + 2], arg);
    Battle_DrawWindowPanelContent1x(&rows[g_BtlMenuDrawWindowIndex * 5 + 4], arg);
    g_BtlMenuDrawWindowIndex += 1;
}

/*
 * Walk all 0x20 battle boxes and hand the open ones to the writers. Box 1 is
 * special: if it is open and the party slot named by g_BtlActiveActor is empty, every
 * open box from 1 up gets Battle_SetWindowStateClosing first.
 */
// FUNCTION: FF7 0x006d83c8
void __cdecl Battle_DrawOpenWindows2x(void)
{
    short i;
    int junk;
    int n;

    if (g_BattleIsPaused != 0) {
        Battle_DrawPauseCaption2x();
    }
    if (g_BattleWindowStates[BATTLE_WINDOW_COMMAND_MENU] != BATTLE_WINDOW_STATE_CLOSED && gSavemap.party[g_BtlActiveActor] == PARTY_CHAR_NONE) {
        for (i = 1; i < 32; i++) {
            if (g_BattleWindowStates[i] != BATTLE_WINDOW_STATE_CLOSED) {
                Battle_SetWindowStateClosing(i);
            }
        }
    }
    g_BtlMenuDrawWindowIndex = 0;
    for (i = 0; i < 32; i++) {
        if (i != 0 && g_BattleWindowStates[i] != BATTLE_WINDOW_STATE_CLOSED && Input_GetHeld(INPUT_SQUARE) != 0) {
            continue;
        }
        if (g_BattleWindowStates[i] != BATTLE_WINDOW_STATE_CLOSED) {
            Battle_DrawWindowRow2x(g_pBtlMenuActiveWindow, i);
        }
    }
    Battle_DrawMenuCursorAndDimOverlay2x(g_pBtlMenuActiveWindow);
}

/*
 * The PAUSE caption, centred over its own frame; it blinks between colours 7 and 0 with bit 0x20
 * of g_BtlMenuFrameCounter.
 */
// FUNCTION: FF7 0x006d84c4
void __cdecl Battle_DrawPauseCaption2x(void)
{
    short rect[4];
    int w;

    w = Menu_MeasureEncodedString2x((unsigned char *)g_BtlMenuWindowColors);
    Menu_DrawEncodedString2x(0x140 - w / 2 + 8, 178, (unsigned char *)g_BtlMenuWindowColors,
                      (g_BtlMenuFrameCounter & 0x20) ? 7 : 0, 0.0f);
    Menu_SetRectShorts(rect, 0x140 - w / 2 - 4, 166, w + 0x14, 48);
    Menu_DrawWindowFrame2x((EnglishWindowFrame *)rect, 0.0f);
}

/*
 * Battle_DrawOpenWindows2x's twin. Three calls differ and nothing else does: the 1x preparer,
 * the 1x writer set, and the 1x flush.
 */
// FUNCTION: FF7 0x006d8550
void __cdecl Battle_DrawOpenWindows1x(void)
{
    short i;
    int junk;
    int n;

    if (g_BattleIsPaused != 0) {
        Battle_DrawPauseCaption1x();
    }
    if (g_BattleWindowStates[BATTLE_WINDOW_COMMAND_MENU] != BATTLE_WINDOW_STATE_CLOSED && gSavemap.party[g_BtlActiveActor] == PARTY_CHAR_NONE) {
        for (i = 1; i < 32; i++) {
            if (g_BattleWindowStates[i] != BATTLE_WINDOW_STATE_CLOSED) {
                Battle_SetWindowStateClosing(i);
            }
        }
    }
    g_BtlMenuDrawWindowIndex = 0;
    for (i = 0; i < 32; i++) {
        if (i != 0 && g_BattleWindowStates[i] != BATTLE_WINDOW_STATE_CLOSED && Input_GetHeld(INPUT_SQUARE) != 0) {
            continue;
        }
        if (g_BattleWindowStates[i] != BATTLE_WINDOW_STATE_CLOSED) {
            Battle_DrawWindowRow1x(g_pBtlMenuActiveWindow, i);
        }
    }
    Battle_DrawMenuCursorAndDimOverlay1x(g_pBtlMenuActiveWindow);
}

/*
 * The PAUSE caption, centred over its own frame; it blinks between colours 7 and 0 with bit 0x20
 * of g_BtlMenuFrameCounter.
 */
// FUNCTION: FF7 0x006d864c
void __cdecl Battle_DrawPauseCaption1x(void)
{
    short rect[4];
    int w;

    w = Menu_MeasureEncodedString1x((unsigned char *)g_BtlMenuWindowColors);
    Menu_DrawEncodedString1x(0xa0 - w / 2 + 4, 89, (unsigned char *)g_BtlMenuWindowColors,
                         (g_BtlMenuFrameCounter & 0x20) ? 7 : 0, 0.0f);
    Menu_SetRectShorts(rect, 0xa0 - w / 2 - 2, 83, w + 0xa, 24);
    Menu_DrawWindowFrame1x((EnglishWindowFrame *)rect, 0.0f);
}

/*
 * Plays confirmation sound, marks input handled and closes active command
 * subpanels, retaining the repeated panel-four close test. Selects the alternate
 * actor when panel 0x13 is active and posts the command with a single-target,
 * full-mask, low-nibble or high-mask target according to targeting mode and the
 * retained eligibility-mask cases.
 */
// FUNCTION: FF7 0x006d86d2
void __cdecl Battle_ConfirmSelectionAndPostCommand(void)
{
    int actor;

    actor = g_BtlActiveActor;
    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
    g_BtlMenuSelectionConfirmed = 1;
    g_BattleMenuState = -1;
    if (g_BattleWindowStates[BATTLE_WINDOW_MANIP_ACTION_LIST] == BATTLE_WINDOW_STATE_OPEN) {
        actor = g_BtlEnemyIndex + 4;
        Battle_SetWindowStateClosing(BATTLE_WINDOW_MANIP_ACTION_LIST);
    }
    if (g_BattleWindowStates[BATTLE_WINDOW_MAGIC_LIST] == BATTLE_WINDOW_STATE_OPEN) {
        Battle_SetWindowStateClosing(BATTLE_WINDOW_MAGIC_LIST);
    }
    if (g_BattleWindowStates[BATTLE_WINDOW_ITEM_LIST] == BATTLE_WINDOW_STATE_OPEN) {
        Battle_SetWindowStateClosing(BATTLE_WINDOW_ITEM_LIST);
    }
    if (g_BattleWindowStates[BATTLE_WINDOW_SUMMON_LIST] == BATTLE_WINDOW_STATE_OPEN) {
        Battle_SetWindowStateClosing(BATTLE_WINDOW_SUMMON_LIST);
    }
    if (g_BattleWindowStates[BATTLE_WINDOW_ENEMY_SKILL_LIST] == BATTLE_WINDOW_STATE_OPEN) {
        Battle_SetWindowStateClosing(BATTLE_WINDOW_ENEMY_SKILL_LIST);
    }
    if (g_BattleWindowStates[BATTLE_WINDOW_ENEMY_SKILL_LIST] == BATTLE_WINDOW_STATE_OPEN) {
        Battle_SetWindowStateClosing(BATTLE_WINDOW_ENEMY_SKILL_LIST);
    }
    if (g_BattleWindowStates[BATTLE_WINDOW_COMMAND_MENU] == BATTLE_WINDOW_STATE_OPEN) {
        Battle_SetWindowStateClosing(BATTLE_WINDOW_COMMAND_MENU);
    }
    if (g_BattleWindowStates[BATTLE_WINDOW_GIL_AMOUNT] == BATTLE_WINDOW_STATE_OPEN) {
        Battle_SetWindowStateClosing(BATTLE_WINDOW_GIL_AMOUNT);
    }

    switch (g_BtlTargetSelectMode) {
    case BTL_TARGET_ONE_ANY:
    case BTL_TARGET_ONE_PARTY:
    case BTL_TARGET_ONE_ENEMY:
        Battle_PostCommand(actor, g_BtlIssuedCommandId, g_BtlIssuedActionId,
                           1 << g_BtlSingleTargetIndex);
        break;
    case BTL_TARGET_ALL:
        Battle_PostCommand(actor, g_BtlIssuedCommandId, g_BtlIssuedActionId,
                           g_BtlTargetAvailableMask);
        break;
    case BTL_TARGET_ALL_PARTY:
        switch (g_BattleScene.layout) {
        case BATTLE_LAYOUT_SIDE_ATTACK_1:
        case BATTLE_LAYOUT_SIDE_ATTACK_2:
        case BATTLE_LAYOUT_SIDE_ATTACK_3:
        case BATTLE_LAYOUT_SIDE_ATTACK_4:
            Battle_PostCommand(
                actor, g_BtlIssuedCommandId, g_BtlIssuedActionId,
                (unsigned short)g_BtlTargetAvailableMask & BATTLE_ACTOR_MASK_ALLIES & g_BattleScene.targetMaskTable[g_BtlTargetGroupIndex]);
            break;
        default:
            Battle_PostCommand(actor, g_BtlIssuedCommandId, g_BtlIssuedActionId,
                               (unsigned short)g_BtlTargetAvailableMask & BATTLE_ACTOR_MASK_ALLIES);
            break;
        }
        break;
    case BTL_TARGET_ALL_ENEMIES:
        switch (g_BattleScene.layout) {
        case BATTLE_LAYOUT_PINCER_ATTACK:
            Battle_PostCommand(
                actor, g_BtlIssuedCommandId, g_BtlIssuedActionId,
                (unsigned short)g_BtlTargetAvailableMask & -0x10 & g_BattleScene.targetMaskTable[g_BtlTargetGroupIndex]);
            break;
        default:
            Battle_PostCommand(actor, g_BtlIssuedCommandId, g_BtlIssuedActionId,
                               (unsigned short)g_BtlTargetAvailableMask & -0x10);
            break;
        }
        break;
    }
}

/*
 * Draw Battle_ConfirmSelectionAndPostCommand twice: once with a second set of six values swapped
 * into the globals it reads, and once with the originals put back. Twelve `short` locals and no
 * other structure.
 */
// FUNCTION: FF7 0x006d8963
void __cdecl Battle_PostSavedThenCurrentCommandSelections(void)
{
    short menuMask;
    short origGroupIdx;
    short swapGroupIndex;
    short altCursorIndex;
    short mainOrigActionId;
    short menuSingle;
    short origCursorIndex;
    short swapActionId;
    short origSingleTarget;
    short menuMode;
    short previousMask;
    short keepMode;

    swapActionId = g_BtlMenuSelectionActionId;
    altCursorIndex = g_BtlMenuSelectionIndex;
    menuMode = g_BtlMenuSelectionTargetMode;
    menuMask = g_BtlMenuSelectionTargetMask;
    menuSingle = g_BtlMenuSelectionSingleTarget;
    swapGroupIndex = g_BtlMenuSelectionTargetGroup;
    mainOrigActionId = g_BtlIssuedActionId;
    origCursorIndex = g_BtlListCursorIndex;
    keepMode = g_BtlTargetSelectMode;
    previousMask = g_BtlTargetAvailableMask;
    origSingleTarget = g_BtlSingleTargetIndex;
    origGroupIdx = g_BtlTargetGroupIndex;
    g_BtlTargetSelectMode = (unsigned char)menuMode;
    g_BtlTargetAvailableMask = menuMask;
    g_BtlSingleTargetIndex = (unsigned char)menuSingle;
    g_BtlIssuedActionId = swapActionId;
    g_BtlListCursorIndex = altCursorIndex;
    g_BtlTargetGroupIndex = (unsigned char)swapGroupIndex;
    Battle_ConfirmSelectionAndPostCommand();
    g_BtlTargetSelectMode = (unsigned char)keepMode;
    g_BtlTargetAvailableMask = previousMask;
    g_BtlSingleTargetIndex = (unsigned char)origSingleTarget;
    g_BtlIssuedActionId = mainOrigActionId;
    g_BtlListCursorIndex = origCursorIndex;
    g_BtlTargetGroupIndex = (unsigned char)origGroupIdx;
    Battle_ConfirmSelectionAndPostCommand();
}

/*
 * Resets the controlled-action list: its MenuCursor (g_BtlMenuControlledActionCursor and the
 * thirteen fields after it) to one column of three rows, wrapping rows, at the first entry; also
 * g_BattleCommandMenuPhase, g_BtlSelectedCommand, g_BtlMenuControlledActionStarted, and the
 * availability mode g_BtlTargetMode to 1.
 */
// FUNCTION: FF7 0x006d8a71
void __cdecl Battle_ResetCommandMenuCursors(void)
{
    g_BattleCommandMenuPhase = 0;
    g_BtlMenuControlledActionCursor = 0;
    g_BtlMenuControlledActionRow = 0;
    g_BtlMenuControlledActionUnused4 = 0;
    g_BtlMenuControlledActionScrollTop = 0;
    g_BtlMenuControlledActionColCount = 1;
    g_BtlMenuControlledActionRowStep = 1;
    g_BtlMenuControlledActionRowCount = 3;
    g_BtlMenuControlledActionTotalRows = 3;
    g_BtlMenuControlledActionColWrapMode = MENU_CURSOR_CLAMP;
    g_BtlMenuControlledActionRowWrapMode = MENU_CURSOR_WRAP;
    g_BtlMenuControlledActionUnused8 = 0;
    g_BtlMenuControlledActionScrollAnim = 0;
    g_BtlMenuControlledActionScrollState = MENU_SCROLL_NONE;
    g_BtlMenuControlledActionPageScroll = 0;
    g_BtlSelectedCommand = BATTLE_CURSOR_ACTION_TARGET;
    g_BtlTargetMode = 1;
    g_BtlMenuControlledActionStarted = 0;
}

/*
 * Runs the cursor over the controlled enemy's action list (BATTLE_MENU_MANIP_ACTION_LIST) and picks
 * one, once. The entry is the cursor's row plus scrollTop; each entry is six bytes laid out like a
 * BattleCommandSlot: the attack id, its BattleCursorAction (only the target cursor is handled), the
 * target flags and the BattleMenuSlotFlag bits (greyed: unavailable).
 */
// FUNCTION: FF7 0x006d8b1e
void __cdecl Battle_WindowFrameControlledEnemyActionList(void)
{
    MenuCursor *cursor;
    unsigned char *rec;
    int w;
    int g;
    int x;
    int h;
    int i;
    int pos;

    if (g_BtlMenuControlledActionStarted == 0) {
        SfxPreloadAndPlayOnChannel5(SFX_MENU_CONFIRM);
        g_BtlMenuControlledActionStarted = 1;
    }
    cursor = (MenuCursor *)&g_BtlMenuControlledActionCursor;
    rec = &g_BattleItemListBlock.enemyActionBytes[g_BtlEnemyIndex * 0x60];
    if (g_BattleMenuState == BATTLE_MENU_MANIP_ACTION_LIST && g_BtlMenuSelectionConfirmed == 0) {
        MenuCursor_Update(cursor);
        if (BtlMenu_GetInputRepeat(INPUT_CIRCLE) != 0) {
            g_BtlMenuSelectionConfirmed = 1;
            pos = cursor->row + cursor->scrollTop;
            if ((rec[pos * 6 + 3] & BATTLE_MENU_SLOT_GREYED) == 0) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                g_BtlIssuedCommandId = BATTLE_COMMAND_ENEMY_ATTACK;
                g_BtlIssuedActionId = rec[pos * 6];
                g_BtlTargetAvailableMask = g_BattleScene.targetExtraMask;
                switch (rec[pos * 6 + 1]) {
                case BATTLE_CURSOR_ACTION_TARGET:
                    g_BattleWindowFrameId = BATTLE_MENU_MANIP_ACTION_LIST;
                    g_BattleMenuState = BATTLE_MENU_SELECT_TARGET;
                    g_BtlTargetFlags = rec[pos * 6 + 2];
                    Battle_BeginTargetSelection();
                    break;
                }
            } else {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
            }
        }
    }
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006d8c4d
void __cdecl Battle_WindowUnusedSlot19NoOp(void)
{
    /* The original body is empty. */
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006d8c52
void __cdecl Battle_WindowOpenNoOp(void)
{
    /* The original body is empty. */
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006d8c57
void __cdecl Battle_WindowUnusedNoOp(void)
{
    /* The original body is empty. */
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006d8c5c
void __cdecl Battle_WindowFrameNoOp(void)
{
    /* The original body is empty. */
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006d8c61
void __cdecl Battle_WindowOpenSlot12And21NoOp(void)
{
    /* The original body is empty. */
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006d8c66
void __cdecl Battle_WindowFrameSlot12And21NoOp(void)
{
    /* The original body is empty. */
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006d8c6b
void __cdecl Battle_WindowUnusedSlot12And21NoOp(void)
{
    /* The original body is empty. */
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006d8c70
void __cdecl Battle_WindowOpenSlot0NoOp(void)
{
    /* The original body is empty. */
}

/*
 * Detects newly set party availability bits and plays/dispatches their
 * notification, then updates the remembered low byte and toggles two input-
 * selected flags. Updates menu state, opens the active normal/alternate actor
 * panel when available and closes active subpanels when their actor disappears or
 * changes, including the two special close sounds.
 */
// FUNCTION: FF7 0x006d8c75
void __cdecl Battle_WindowFrameSlot0OpenCommandMenuForActor(void)
{
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short activeActor;
    int padLocal[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short slotIdx;
    short newFlags;

    if (g_BattleScene.readyActorMask != g_BtlMenuLastSceneByte) {
        newFlags = g_BattleScene.readyActorMask ^ g_BtlMenuLastSceneByte;
        newFlags &= *(short *)&g_BattleScene.readyActorMask;
        if (newFlags != 0) {
            SfxPreloadAndPlayOnChannel5(SFX_MENU_CONFIRM);
        }
        for (slotIdx = 0; slotIdx < 3; slotIdx++) {
            if ((newFlags & 1 << slotIdx) != 0) {
                Battle_PartyMemberAvailableHookNoOp((unsigned short)gSavemap.party[slotIdx]);
            }
        }
    }
    g_BtlMenuLastSceneByte = *(char *)&g_BattleScene.readyActorMask;
    if (BtlMenu_GetInputRepeat(INPUT_R2) != 0) {
        *(char *)&gSavemap.tutSub ^= 0x40;
    }
    if (BtlMenu_GetInputRepeat(INPUT_SELECT) != 0) {
        gSavemap.options ^= 0x4000;
    }
    Battle_StepPartyHpMpGaugeDisplay();
    if (g_BattleWindowStates[BATTLE_WINDOW_ARENA_SLOT] == BATTLE_WINDOW_STATE_CLOSED) {
        activeActor = Battle_GetActiveMenuActor();
        if ((activeActor & 0xFF) != 0xFF && g_BattleWindowStates[BATTLE_WINDOW_COMMAND_MENU] == BATTLE_WINDOW_STATE_CLOSED && g_BattleWindowStates[BATTLE_WINDOW_MANIP_ACTION_LIST] == BATTLE_WINDOW_STATE_CLOSED) {
            if ((activeActor & 0xFF) >= 4) {
                g_BtlEnemyIndex = (activeActor & 0xFF) - 4;
                g_BattleMenuState = BATTLE_MENU_MANIP_ACTION_LIST;
                Battle_OpenWindow(-1, -1, BATTLE_WINDOW_MANIP_ACTION_LIST);
            } else {
                g_BtlActiveActor = activeActor & 0xFF;
                g_BattleMenuState = BATTLE_MENU_COMMAND_MENU;
                Battle_OpenWindow(-1, -1, BATTLE_WINDOW_COMMAND_MENU);
            }
        }
        if (g_BattleWindowStates[BATTLE_WINDOW_COMMAND_MENU] != BATTLE_WINDOW_STATE_CLOSED && ((activeActor & 0xFF) == 0xFF || (activeActor & 0xFF) != g_BtlActiveActor)) {
            for (slotIdx = 1; slotIdx < 32; slotIdx++) {
                if (g_BattleWindowStates[slotIdx] != BATTLE_WINDOW_STATE_CLOSED) {
                    Battle_SetWindowStateClosing((short)slotIdx);
                    if (slotIdx == BATTLE_WINDOW_TIFA_SLOTS || slotIdx == BATTLE_WINDOW_CAIT_SITH_SLOTS) {
                        Battle_PlaySfxCentered(SFX_ID_BATTLE_LIMIT_READY);
                    }
                }
            }
            g_BattleMenuState = 0xFFFF;
        }
        if (g_BattleWindowStates[BATTLE_WINDOW_MANIP_ACTION_LIST] != BATTLE_WINDOW_STATE_CLOSED && ((activeActor & 0xFF) == 0xFF || (activeActor & 0xFF) != g_BtlEnemyIndex + 4)) {
            for (slotIdx = 1; slotIdx < 32; slotIdx++) {
                if (g_BattleWindowStates[slotIdx] != BATTLE_WINDOW_STATE_CLOSED) {
                    Battle_SetWindowStateClosing((short)slotIdx);
                }
            }
            g_BattleMenuState = 0xFFFF;
        }
    }
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006d8f1d
void __cdecl Battle_WindowUnusedSlot0NoOp(void)
{
    /* The original body is empty. */
}

/*
 * Seed the gil amount being chosen from the party's gil, capped at 600,000. The
 * amount is unsigned: Battle_WindowFrameGilAmountPrompt divides it with `div`.
 */
// FUNCTION: FF7 0x006d8f22
void __cdecl Battle_WindowOpenSeedGilAmount(void)
{
    g_BtlMenuGilAmount = gSavemap.gil;
    if (g_BtlMenuGilAmount > (unsigned int)0x927C0) {
        g_BtlMenuGilAmount = 0x927C0;
    }
}

/*
 * Drive the gil-amount prompt while the battle menu is in BATTLE_MENU_GIL_AMOUNT and no answer
 * has been given. Circle commits: the menu goes to target selection with the prompt remembered
 * as the state to return to, the target flags select the enemy side, a tenth of the amount is
 * the action id and Battle_BeginTargetSelection runs. Left/Right step the amount by 1,000 and
 * Up/Down by 10,000, clamped to 0..600,000 and the party's gil.
 */
// FUNCTION: FF7 0x006d8f47
void __cdecl Battle_WindowFrameGilAmountPrompt(void)
{
    int entry;

    entry = g_BtlActiveActor * 0x700 + (int)&g_BtlMenuGilCursor;
    if (g_BattleMenuState == BATTLE_MENU_GIL_AMOUNT && g_BtlMenuSelectionConfirmed == 0) {
        if (BtlMenu_GetInputRepeat(INPUT_CIRCLE) != 0) {
            g_BtlMenuSelectionConfirmed = 1;
            g_BattleMenuState = BATTLE_MENU_SELECT_TARGET;
            g_BattleWindowFrameId = BATTLE_MENU_GIL_AMOUNT;
            g_BtlTargetFlags = BTL_TARGETF_SELECTABLE | BTL_TARGETF_START_ON_ENEMIES |
                               BTL_TARGETF_DEFAULT_MULTIPLE | BTL_TARGETF_ONE_SIDE_ONLY;
            g_BtlIssuedActionId = g_BtlMenuGilAmount / 10;
            Battle_BeginTargetSelection();
        } else if (BtlMenu_GetInputRepeat(INPUT_CROSS) != 0) {
            g_BtlMenuSelectionConfirmed = 1;
            g_BattleMenuState = BATTLE_MENU_COMMAND_MENU;
            Battle_SetWindowStateClosing(BATTLE_WINDOW_GIL_AMOUNT);
            SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
        } else {
            if (BtlMenu_GetInputRepeat(INPUT_RIGHT) != 0) {
                g_BtlMenuGilAmount += 1000;
            } else if (BtlMenu_GetInputRepeat(INPUT_LEFT) != 0) {
                g_BtlMenuGilAmount -= 1000;
            } else if (BtlMenu_GetInputRepeat(INPUT_UP) != 0) {
                g_BtlMenuGilAmount += 10000;
            } else if (BtlMenu_GetInputRepeat(INPUT_DOWN) != 0) {
                g_BtlMenuGilAmount -= 10000;
            }
            if ((g_BtlMenuGilAmount & 0x80000000) != 0) {
                g_BtlMenuGilAmount = 0;
            }
            if (g_BtlMenuGilAmount > (unsigned int)0x927C0) {
                g_BtlMenuGilAmount = 0x927C0;
            }
            if (g_BtlMenuGilAmount > (unsigned int)gSavemap.gil) {
                g_BtlMenuGilAmount = gSavemap.gil;
            }
        }
    }
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006d90e3
void __cdecl Battle_WindowUnusedSlot11And20NoOp(void)
{
    /* The original body is empty. */
}

/*
 * Opens the active actor's command menu: sizes window 1 to the actor's command columns
 * (g_BattleScalingTable[columns - 1]) and resets the command MenuCursor to those columns by four
 * rows, wrapping rows. With g_BtlMenuOption zero the cursor also returns to the first command.
 */
// FUNCTION: FF7 0x006d90e8
void __cdecl Battle_WindowOpenCommandMenuResizeResetCursor(void)
{
    short columnCount;
    MenuCursor *slotStatus;

    g_BtlMenuControlledActionStarted = 0;
    Battle_RefreshPendingCommandAvailability(g_BtlActiveActor, 0);
    columnCount = g_MenuPartyStats[g_BtlActiveActor].commandColumns;
    slotStatus = (MenuCursor *)&g_BtlMenuWindowSets[g_BtlActiveActor * 0x700];
    g_BattleMenuData[BATTLE_WINDOW_COMMAND_MENU].width = g_BattleScalingTable[columnCount - 1];
    g_BattleMenuData[BATTLE_WINDOW_COMMAND_MENU].sub[0].right = g_BattleScalingTable[columnCount - 1];
    if (g_BtlMenuOption == 0) {
        slotStatus->col = 0;
        slotStatus->row = 0;
        slotStatus->unused4 = 0;
        slotStatus->scrollTop = 0;
    }
    slotStatus->colCount = columnCount;
    slotStatus->rowCount = 4;
    slotStatus->rowStep = columnCount;
    slotStatus->totalRows = 4;
    slotStatus->colWrapMode = MENU_CURSOR_CLAMP;
    slotStatus->rowWrapMode = MENU_CURSOR_WRAP;
    slotStatus->unused8 = 0;
    slotStatus->scrollAnim = 0;
    slotStatus->scrollState = MENU_SCROLL_NONE;
}

/*
 * Services the active party battle-command menu, invoking
 * Battle_RefreshPendingCommandAvailability and playing its entry sound once. In
 * BATTLE_MENU_COMMAND_MENU with input enabled, updates the cursor and tries at most 20 times to
 * skip empty command slots (id 0xFF) using the observed row/column movement, wrapping columns by
 * four and rows by the configured count. Left opens Change, Right Defend, and Circle runs the
 * chosen command's BattleCursorAction.
 */
// FUNCTION: FF7 0x006d91fa
void __cdecl Battle_WindowFrameCommandMenu(void)
{
    struct {
        short oldColumn, align1a;
        short oldRow, align16;
        int tries;
        short dy, align0e;
        short dx, align0a;
        BattleCommandSlot *commands;
        MenuCursor *cursor;
    } v; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    Battle_RefreshPendingCommandAvailability(g_BtlActiveActor, 0);
    if (g_BtlMenuControlledActionStarted == 0) {
        SfxPreloadAndPlayOnChannel5(SFX_MENU_CONFIRM);
        g_BtlMenuControlledActionStarted = 1;
    }
    v.cursor = (MenuCursor *)&((BattleMenuWindowSet *)g_BtlMenuWindowSets)[g_BtlActiveActor].cursorIndex;
    v.commands = g_MenuPartyStats[g_BtlActiveActor].commands;
    if (g_BattleMenuState == BATTLE_MENU_COMMAND_MENU && g_BtlMenuSelectionConfirmed == 0) {
        v.oldRow = v.cursor->col;
        v.oldColumn = v.cursor->row;
        MenuCursor_Update(v.cursor);
        v.dx = v.cursor->col - v.oldRow;
        v.dy = v.cursor->row - v.oldColumn;
        for (v.tries = 0; v.tries < 20; v.tries++) {
            if (v.commands[v.cursor->col * 4 + v.cursor->row].commandId != 0xff) {
                break;
            }
            v.cursor->row += v.dy;
            v.cursor->col += v.dx;
            v.cursor->row &= 3;
            v.cursor->col %= g_MenuPartyStats[g_BtlActiveActor].commandColumns;
        }
        if (v.cursor->col == v.oldRow && BtlMenu_GetInputRepeat(INPUT_LEFT) != 0) {
            SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
            g_BtlIssuedCommandId = BATTLE_COMMAND_CHANGE;
            g_BattleMenuState = BATTLE_MENU_CHANGE_CONFIRM;
            if (g_MenuIsSmallViewport320x240 != 1) {
                Battle_OpenWindow(g_BattleMenuData[BATTLE_WINDOW_COMMAND_MENU].x - 0x60,
                                  v.cursor->row * 24 + 0x152, BATTLE_WINDOW_CHANGE);
            } else {
                Battle_OpenWindow(g_BattleMenuData[BATTLE_WINDOW_COMMAND_MENU].x - 0x30,
                                  v.cursor->row * 12 + 0xa9, BATTLE_WINDOW_CHANGE);
            }
            g_BtlMenuSelectionConfirmed = 1;
        } else if (v.cursor->col == v.oldRow && BtlMenu_GetInputRepeat(INPUT_RIGHT) != 0) {
            SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
            g_BtlIssuedCommandId = BATTLE_COMMAND_DEFEND;
            g_BattleMenuState = BATTLE_MENU_DEFEND_CONFIRM;
            if (g_MenuIsSmallViewport320x240 != 1) {
                Battle_OpenWindow(g_BattleMenuData[BATTLE_WINDOW_COMMAND_MENU].x +
                                      g_BattleMenuData[BATTLE_WINDOW_COMMAND_MENU].width - 0x10,
                                  v.cursor->row * 24 + 0x152, BATTLE_WINDOW_DEFEND);
            } else {
                Battle_OpenWindow(g_BattleMenuData[BATTLE_WINDOW_COMMAND_MENU].x +
                                      g_BattleMenuData[BATTLE_WINDOW_COMMAND_MENU].width - 8,
                                  v.cursor->row * 12 + 0xa9, BATTLE_WINDOW_DEFEND);
            }
            g_BtlMenuSelectionConfirmed = 1;
        } else if (BtlMenu_GetInputRepeat(INPUT_CIRCLE) != 0) {
            g_BtlTargetMode = 1;
            g_BtlMenuSelectionConfirmed = 1;
            if ((v.commands[v.cursor->col * 4 + v.cursor->row].menuFlags & BATTLE_MENU_SLOT_GREYED) == 0) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                g_BtlSelectedCommand = v.commands[v.cursor->col * 4 + v.cursor->row].cursorAction;
                g_BtlIssuedCommandId = v.commands[v.cursor->col * 4 + v.cursor->row].commandId;
                g_BtlTargetAvailableMask = g_BattleScene.targetExtraMask;
                g_BattleCommandMenuPhase = 0;
                switch (g_BtlSelectedCommand) {
                case BATTLE_CURSOR_ACTION_TARGET:
                    g_BtlTargetMode = 1;
                    g_BattleWindowFrameId = BATTLE_MENU_COMMAND_MENU;
                    g_BattleMenuState = BATTLE_MENU_SELECT_TARGET;
                    g_BtlTargetFlags = v.commands[v.cursor->col * 4 + v.cursor->row].targetFlags;
                    Battle_BeginTargetSelection();
                    break;
                case BATTLE_CURSOR_ACTION_TARGET_VALID_MASK:
                    g_BtlTargetMode = 0;
                    g_BattleWindowFrameId = BATTLE_MENU_COMMAND_MENU;
                    g_BattleMenuState = BATTLE_MENU_SELECT_TARGET;
                    g_BtlTargetFlags = v.commands[v.cursor->col * 4 + v.cursor->row].targetFlags;
                    Battle_BeginTargetSelection();
                    break;
                case BATTLE_CURSOR_ACTION_W_MAGIC:
                    g_BattleCommandMenuPhase = 1;
                case BATTLE_CURSOR_ACTION_MAGIC:
                    g_BattleMenuState = BATTLE_MENU_MAGIC_LIST;
                    Battle_OpenWindow(-1, -1, BATTLE_WINDOW_MAGIC_LIST);
                    break;
                case BATTLE_CURSOR_ACTION_W_ITEM:
                    g_BattleCommandMenuPhase = 1;
                case BATTLE_CURSOR_ACTION_ITEM:
                    g_BattleMenuState = BATTLE_MENU_ITEM_LIST;
                    Battle_OpenWindow(-1, -1, BATTLE_WINDOW_ITEM_LIST);
                    break;
                case BATTLE_CURSOR_ACTION_W_SUMMON:
                    g_BattleCommandMenuPhase = 1;
                case BATTLE_CURSOR_ACTION_SUMMON:
                    g_BattleMenuState = BATTLE_MENU_SUMMON_LIST;
                    g_BtlMenuSelectionConfirmed = 1;
                    Battle_OpenWindow(-1, -1, BATTLE_WINDOW_SUMMON_LIST);
                    break;
                case BATTLE_CURSOR_ACTION_ENEMY_SKILL:
                    g_BattleMenuState = BATTLE_MENU_ENEMY_SKILL_LIST;
                    g_BtlMenuSelectionConfirmed = 1;
                    Battle_OpenWindow(-1, -1, BATTLE_WINDOW_ENEMY_SKILL_LIST);
                    break;
                case BATTLE_CURSOR_ACTION_COIN:
                    if (g_BtlMenuFrameCounter & 0x40) {
                        g_BattleMenuState = BATTLE_MENU_GIL_AMOUNT;
                        g_BtlMenuSelectionConfirmed = 1;
                        Battle_OpenWindow(-1, -1, BATTLE_WINDOW_GIL_AMOUNT);
                    } else {
                        g_BattleMenuState = BATTLE_MENU_ITEM_LIST;
                        g_BtlMenuSelectionConfirmed = 1;
                        g_BtlSelectedCommand = BATTLE_CURSOR_ACTION_THROW;
                        g_BtlIssuedCommandId = BATTLE_COMMAND_THROW;
                        Battle_OpenWindow(-1, -1, BATTLE_WINDOW_ITEM_LIST);
                    }
                    break;
                case BATTLE_CURSOR_ACTION_THROW:
                    g_BattleMenuState = BATTLE_MENU_ITEM_LIST;
                    Battle_OpenWindow(-1, -1, BATTLE_WINDOW_ITEM_LIST);
                    break;
                case BATTLE_CURSOR_ACTION_LIMIT:
                    g_BtlActivePartyMember = g_BtlActiveActor;
                    g_BtlMenuSelectionConfirmed = 1;
                    if (gSavemap.party[g_BtlActiveActor] == PARTY_CHAR_TIFA) {
                        g_BattleMenuState = BATTLE_MENU_TIFA_SLOTS;
                        Battle_OpenWindow(-1, -1, BATTLE_WINDOW_TIFA_SLOTS);
                        Battle_PlaySfxCentered(SFX_ID_BATTLE_CMD_CHANGE);
                    } else if (gSavemap.party[g_BtlActiveActor] == PARTY_CHAR_CAIT_SITH) {
                        if (gSavemap.chars[6].limitLevel == 2) {
                            g_BattleMenuState = BATTLE_MENU_CAIT_SITH_SLOTS;
                            Battle_OpenWindow(-1, -1, BATTLE_WINDOW_CAIT_SITH_SLOTS);
                            Battle_PlaySfxCentered(SFX_ID_BATTLE_CMD_CHANGE);
                        } else {
                            g_BattleMenuState = BATTLE_MENU_LIMIT_LIST;
                            Battle_OpenWindow(-1, -1, BATTLE_WINDOW_LIMIT_LIST);
                        }
                    } else {
                        g_BattleMenuState = BATTLE_MENU_LIMIT_LIST;
                        Battle_OpenWindow(-1, -1, BATTLE_WINDOW_LIMIT_LIST);
                    }
                    break;
                }
            } else {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
            }
        }
    }
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006d9827
void __cdecl Battle_WindowUnusedSlot1NoOp(void)
{
    /* The original body is empty. */
}

/*
 * Resets the active actor's item-list MenuCursor: one column of three visible rows over twice
 * the item list's row count, wrapping into rows. With g_BtlMenuOption zero the cursor also
 * returns to the first item.
 */
// FUNCTION: FF7 0x006d982c
void __cdecl Battle_WindowOpenItemPanelResetCursor(void)
{
    MenuCursor *cursor;

    cursor = (MenuCursor *)&g_BtlMenuSlotTableA[g_BtlActiveActor * 0x700];
    if (g_BtlMenuOption == 0) {
        cursor->col = 0;
        cursor->row = 0;
        cursor->unused4 = 0;
        cursor->scrollTop = 0;
    }
    cursor->colCount = 1;
    cursor->rowCount = 3;
    cursor->rowStep = 1;
    cursor->totalRows = g_BattleItemListBlock.itemRowCount * 2;
    cursor->colWrapMode = MENU_CURSOR_WRAP_INTO_ROWS;
    cursor->rowWrapMode = MENU_CURSOR_CLAMP;
    cursor->unused8 = 0;
    cursor->scrollAnim = 0;
    cursor->scrollState = MENU_SCROLL_NONE;
    cursor->pageScrollEnable = 1;
}

/*
 * For the active item panel, resets input lock on an update flag and otherwise
 * updates its cursor. Confirmation on a settled cursor checks item flags/type and
 * nonempty ID, copies selection/target parameters, resolves use conditions and
 * opens the condition panel when needed, or plays failure sound.
 */
// FUNCTION: FF7 0x006d98e3
void __cdecl Battle_WindowFrameItemPanel(void)
{
    MenuCursor *cursor;
    BattleInventorySlot *list;
    int flag;
    short itemIndex;

    list = g_BattleItemListBlock.slots;
    cursor = (MenuCursor *)&g_BtlMenuSlotTableA[g_BtlActiveActor * 0x700];
    if (g_BattleMenuState == BATTLE_MENU_ITEM_LIST) {
        if (g_BattleItemListBlock.inventoryChanged != 0) {
            g_BattleItemListBlock.inventoryChanged = 0;
            g_BtlMenuSelectionConfirmed = 1;
        }
        if (g_BtlMenuSelectionConfirmed == 0) {
            MenuCursor_Update(cursor);
            if (cursor->scrollState == MENU_SCROLL_NONE) {
                if (BtlMenu_GetInputRepeat(INPUT_CIRCLE) != 0) {
                    g_BtlMenuSelectionConfirmed = 1;
                    itemIndex = cursor->col + cursor->row + cursor->scrollTop;
                    if (g_BtlSelectedCommand == BATTLE_CURSOR_ACTION_ITEM || g_BtlSelectedCommand == BATTLE_CURSOR_ACTION_W_ITEM) {
                        flag = list[itemIndex].flags & BATTLE_MENU_ITEM_NO_BATTLE_USE;
                    } else {
                        flag = list[itemIndex].flags & BATTLE_MENU_ITEM_NO_THROW;
                    }
                    if (flag == 0 && list[itemIndex].id != 0xffff) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        g_BtlIssuedActionId = *(short *)&list[itemIndex].id;
                        g_BtlTargetFlags = list[itemIndex].targetFlags;
                        g_BtlListCursorIndex = itemIndex;
                        Battle_BeginTargetSelection();
                        g_BattleMenuState = BATTLE_MENU_SELECT_TARGET;
                        g_BattleWindowFrameId = BATTLE_MENU_ITEM_LIST;
                        g_BattleSelectedUseCondition =
                            Kernel_GetUseCondition(4, list[itemIndex].id);
                        if (g_BattleSelectedUseCondition != -1) {
                            Battle_OpenWindow(-1, -1, BATTLE_WINDOW_PARTY_HUD);
                        }
                    } else {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                    }
                } else if (BtlMenu_GetInputRepeat(INPUT_CROSS) != 0) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                    g_BtlMenuSelectionConfirmed = 1;
                    g_BattleMenuState = BATTLE_MENU_COMMAND_MENU;
                    Battle_SetWindowStateClosing(BATTLE_WINDOW_ITEM_LIST);
                }
            }
        }
    }
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006d9ad0
void __cdecl Battle_WindowUnusedSlot5NoOp(void)
{
    /* The original body is empty. */
}

/*
 * Battle_WindowOpenItemPanelResetCursor's shape again, on the magic list's MenuCursor at
 * g_BtlMenuSlotTableB: three columns and 18 rows in all. Then refreshes command 1's availability.
 */
// FUNCTION: FF7 0x006d9ad5
void __cdecl Battle_WindowOpenMagicPanelResetCursor(void)
{
    MenuCursor *cursor;

    cursor = (MenuCursor *)&g_BtlMenuSlotTableB[g_BtlActiveActor * 0x700];
    if (g_BtlMenuOption == 0) {
        cursor->col = 0;
        cursor->row = 0;
        cursor->unused4 = 0;
        cursor->scrollTop = 0;
    }
    cursor->colCount = 3;
    cursor->rowCount = 3;
    cursor->rowStep = 3;
    cursor->totalRows = 18;
    cursor->colWrapMode = MENU_CURSOR_WRAP_INTO_ROWS;
    cursor->rowWrapMode = MENU_CURSOR_CLAMP;
    cursor->unused8 = 0;
    cursor->scrollAnim = 0;
    cursor->scrollState = MENU_SCROLL_NONE;
    cursor->pageScrollEnable = 1;
    Battle_RefreshPendingCommandAvailability(g_BtlActiveActor, 1);
}

/*
 * Refreshes the selected character's mode-1 records, then in menu state 6 with no
 * input latch updates its cursor. Confirm computes the three-column entry index,
 * rejects bit-2/255 entries with sound 3, otherwise saves its
 * item/index/auxiliary fields, plays sound 1 and enters the use- condition path
 * through Battle_BeginTargetSelection/Kernel_GetUseCondition, invoking Battle_OpenWindow when
 * allowed.
 */
// FUNCTION: FF7 0x006d9b98
void __cdecl Battle_WindowFrameMagicList(void)
{
    BattleMagicListWindow *window;
    int statBlock;
    BattleAbilitySlot *slots;
    int entIndex;

    Battle_RefreshPendingCommandAvailability(g_BtlActiveActor, 1);
    statBlock = (int)g_MenuPartyStats[0].statBlock;
    slots = g_MenuPartyStats[g_BtlActiveActor].spells;
    window = (BattleMagicListWindow *)(g_BtlActiveActor * 0x700 + (char *)&g_BtlMenuSlotTableB);
    if (g_BattleMenuState == BATTLE_MENU_MAGIC_LIST && g_BtlMenuSelectionConfirmed == 0) {
        MenuCursor_Update((MenuCursor *)window);
        if (window->scrollState == 0) {
            if (BtlMenu_GetInputRepeat(INPUT_CIRCLE) != 0) {
                g_BtlMenuSelectionConfirmed = 1;
                entIndex = window->col + window->row * 3 + window->scrollTop * 3;
                if ((slots[entIndex].menuFlags & BATTLE_MENU_SLOT_GREYED) == 0 && slots[entIndex].attackId != 0xFF) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    g_BtlIssuedActionId = slots[entIndex].attackId;
                    g_BtlTargetFlags = slots[entIndex].targetFlags;
                    g_BtlListCursorIndex = entIndex;
                    if (DEAD_IN_RETAIL != 0) {
                        g_BattleMenuState = BATTLE_MENU_GIL_AMOUNT;
                        g_BattleMenuReturnState = BATTLE_MENU_MAGIC_LIST;
                        Battle_OpenWindow(-1, -1, BATTLE_WINDOW_GIL_AMOUNT);
                        g_BtlSelectedSpellAllCount = slots[entIndex].allCount;
                        g_BtlSelectedSpellQuadraLinks = slots[entIndex].quadraLinks;
                        g_BtlMenuSelectedFlags = slots[entIndex].menuFlags >> 2 & 3;
                    } else {
                        Battle_BeginTargetSelection();
                        g_BattleMenuState = BATTLE_MENU_SELECT_TARGET;
                        g_BattleWindowFrameId = BATTLE_MENU_MAGIC_LIST;
                        g_BattleSelectedUseCondition = Kernel_GetUseCondition(0, slots[entIndex].attackId);
                        if (g_BattleSelectedUseCondition != -1) {
                            Battle_OpenWindow(-1, -1, BATTLE_WINDOW_PARTY_HUD);
                        }
                    }
                } else {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                }
            } else if (BtlMenu_GetInputRepeat(INPUT_CROSS) != 0) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                g_BtlMenuSelectionConfirmed = 1;
                g_BattleMenuState = BATTLE_MENU_COMMAND_MENU;
                Battle_SetWindowStateClosing(BATTLE_WINDOW_MAGIC_LIST);
            }
        }
    }
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006d9db4
void __cdecl Battle_WindowUnusedSlot6NoOp(void)
{
    /* The original body is empty. */
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006d9db9
void __cdecl Battle_WindowOpenSlot2NoOp(void)
{
    /* The original body is empty. */
}

/*
 * Confirm or cancel out of state 2, once. Battle_WindowFrameSlot3ConfirmOrCancel's shape with a
 * veto in front of the confirm arm.
 */
// FUNCTION: FF7 0x006d9dbe
void __cdecl Battle_WindowFrameSlot2ConfirmOrCancel(void)
{
    int allowed;

    switch (g_BattleScene.layout) {
    case BATTLE_LAYOUT_SIDE_ATTACK_1:
    case BATTLE_LAYOUT_PINCER_ATTACK:
    case BATTLE_LAYOUT_SIDE_ATTACK_2:
    case BATTLE_LAYOUT_SIDE_ATTACK_3:
    case BATTLE_LAYOUT_SIDE_ATTACK_4:
    case BATTLE_LAYOUT_FRONT_ROW_LOCKED:
        allowed = 0;
        break;
    default:
        allowed = 1;
        break;
    }
    if (g_BattleMenuState == BATTLE_MENU_CHANGE_CONFIRM && g_BtlMenuSelectionConfirmed == 0) {
        if (BtlMenu_GetInputRepeat(INPUT_CIRCLE) != 0) {
            if (allowed != 0) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                g_BtlMenuSelectionConfirmed = 1;
                g_BattleMenuState = -1;
                Battle_PostCommand(g_BtlActiveActor, g_BtlIssuedCommandId, g_BtlIssuedActionId,
                                   g_BtlTargetAvailableMask);
                Battle_SetWindowStateClosing(BATTLE_WINDOW_COMMAND_MENU);
                Battle_SetWindowStateClosing(BATTLE_WINDOW_CHANGE);
            } else {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
            }
        } else if (BtlMenu_GetInputRepeat(INPUT_CROSS) != 0 ||
                   BtlMenu_GetInputRepeat(INPUT_RIGHT) != 0) {
            SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
            g_BtlMenuSelectionConfirmed = 1;
            g_BattleMenuState = BATTLE_MENU_COMMAND_MENU;
            Battle_SetWindowStateClosing(BATTLE_WINDOW_CHANGE);
        }
    }
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006d9ecd
void __cdecl Battle_WindowUnusedSlot2NoOp(void)
{
    /* The original body is empty. */
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006d9ed2
void __cdecl Battle_WindowOpenSlot3NoOp(void)
{
    /* The original body is empty. */
}

/*
 * Confirm or cancel out of state 3, once. g_BtlMenuSelectionConfirmed is the once-only latch:
 * both arms set it, and the guard at the top refuses to run again until something
 * clears it.
 */
// FUNCTION: FF7 0x006d9ed7
void __cdecl Battle_WindowFrameSlot3ConfirmOrCancel(void)
{
    if (g_BattleMenuState == BATTLE_MENU_DEFEND_CONFIRM && g_BtlMenuSelectionConfirmed == 0) {
        if (BtlMenu_GetInputRepeat(INPUT_CIRCLE) != 0) {
            SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
            g_BtlMenuSelectionConfirmed = 1;
            g_BattleMenuState = -1;
            Battle_PostCommand(g_BtlActiveActor, g_BtlIssuedCommandId, g_BtlIssuedActionId,
                               g_BtlTargetAvailableMask);
            Battle_SetWindowStateClosing(BATTLE_WINDOW_DEFEND);
            Battle_SetWindowStateClosing(BATTLE_WINDOW_COMMAND_MENU);
        } else if (BtlMenu_GetInputRepeat(INPUT_CROSS) != 0 ||
                   BtlMenu_GetInputRepeat(INPUT_LEFT) != 0) {
            SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
            g_BtlMenuSelectionConfirmed = 1;
            g_BattleMenuState = BATTLE_MENU_COMMAND_MENU;
            Battle_SetWindowStateClosing(BATTLE_WINDOW_DEFEND);
        }
    }
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006d9faa
void __cdecl Battle_WindowUnusedSlot3NoOp(void)
{
    /* The original body is empty. */
}

/*
 * The same, on the summon list's MenuCursor at g_BtlMenuWindowRecordsC: one column and 16 rows
 * in all. Then refreshes command 2's availability.
 */
// FUNCTION: FF7 0x006d9faf
void __cdecl Battle_WindowOpenSummonListResetCursor(void)
{
    MenuCursor *cursor;

    cursor = (MenuCursor *)&g_BtlMenuWindowRecordsC[g_BtlActiveActor * 0x700];
    if (g_BtlMenuOption == 0) {
        cursor->col = 0;
        cursor->row = 0;
        cursor->unused4 = 0;
        cursor->scrollTop = 0;
    }
    cursor->colCount = 1;
    cursor->rowCount = 3;
    cursor->rowStep = 1;
    cursor->totalRows = 16;
    cursor->colWrapMode = MENU_CURSOR_WRAP_INTO_ROWS;
    cursor->rowWrapMode = MENU_CURSOR_CLAMP;
    cursor->unused8 = 0;
    cursor->scrollAnim = 0;
    cursor->scrollState = MENU_SCROLL_NONE;
    cursor->pageScrollEnable = 1;
    Battle_RefreshPendingCommandAvailability(g_BtlActiveActor, 2);
}

/*
 * Refreshes the selected character's mode-2 records, then in menu state 7 with no
 * input latch updates its cursor. Confirm computes the entry index, rejects
 * bit-2/255 entries with sound 3, otherwise saves item/index/auxiliary fields,
 * plays sound 1, calls Battle_BeginTargetSelection and selects state 0 with prior state 7.
 */
// FUNCTION: FF7 0x006da072
void __cdecl Battle_WindowFrameSummonList(void)
{
    BattleMagicListWindow *window;
    int statBlock;
    BattleAbilitySlot *slots;
    int entIndex;

    statBlock = (int)g_MenuPartyStats[0].statBlock;
    slots = g_MenuPartyStats[g_BtlActiveActor].summons;
    window = (BattleMagicListWindow *)(g_BtlActiveActor * 0x700 + (char *)&g_BtlMenuWindowRecordsC);
    Battle_RefreshPendingCommandAvailability(g_BtlActiveActor, 2);
    if (g_BattleMenuState == BATTLE_MENU_SUMMON_LIST && g_BtlMenuSelectionConfirmed == 0) {
        MenuCursor_Update((MenuCursor *)window);
        if (window->scrollState == 0) {
            if (BtlMenu_GetInputRepeat(INPUT_CIRCLE) != 0) {
                g_BtlMenuSelectionConfirmed = 1;
                entIndex = window->col + window->row + window->scrollTop;
                if ((slots[entIndex].menuFlags & BATTLE_MENU_SLOT_GREYED) == 0 && slots[entIndex].attackId != 0xFF) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    g_BtlIssuedActionId = slots[entIndex].attackId;
                    g_BtlListCursorIndex = entIndex;
                    g_BtlTargetFlags = slots[entIndex].targetFlags;
                    if (1 != 0) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
                        Battle_BeginTargetSelection();
                        g_BattleMenuState = BATTLE_MENU_SELECT_TARGET;
                        g_BattleWindowFrameId = BATTLE_MENU_SUMMON_LIST;
                    } else {
                        g_BtlSelectedSpellAllCount = slots[entIndex].allCount;
                        g_BtlSelectedSpellQuadraLinks = slots[entIndex].quadraLinks;
                        g_BattleMenuState = BATTLE_MENU_GIL_AMOUNT;
                        g_BattleMenuReturnState = BATTLE_MENU_SUMMON_LIST;
                        Battle_OpenWindow(-1, -1, BATTLE_WINDOW_GIL_AMOUNT);
                    }
                } else {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                }
            } else if (BtlMenu_GetInputRepeat(INPUT_CROSS) != 0) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                g_BtlMenuSelectionConfirmed = 1;
                g_BattleMenuState = BATTLE_MENU_COMMAND_MENU;
                Battle_SetWindowStateClosing(BATTLE_WINDOW_SUMMON_LIST);
            }
        }
    }
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006da237
void __cdecl Battle_WindowUnusedSlot7NoOp(void)
{
    /* The original body is empty. */
}

/*
 * Resets the active actor's Enemy Skill cursor block (a MenuCursor in the actor's 0x700-byte
 * record): two columns of three visible rows over twelve, wrapping across rows. With
 * g_BtlMenuOption zero the cursor also goes back to the first entry.
 */
// FUNCTION: FF7 0x006da23c
void __cdecl Battle_WindowOpenEnemySkillListResetCursor(void)
{
    if (g_BtlMenuOption == 0) {
        ((BattleMenuWindow *)g_BtlMenuWindows)[g_BtlActiveActor].entryIndex = 0;
        ((BattleMenuCursor *)g_BtlMenuCursors)[g_BtlActiveActor].row = 0;
        *(int *)(g_BtlMenuEnemySkillUnused4 + g_BtlActiveActor * 0x700) = 0;
        ((BtlMenuActorFieldStride *)g_BtlMenuWindowRecords)[g_BtlActiveActor].value = 0;
    }
    *(int *)(g_BtlMenuEnemySkillColCount + g_BtlActiveActor * 0x700) = 2;
    *(int *)(g_BtlMenuEnemySkillRowCount + g_BtlActiveActor * 0x700) = 3;
    *(int *)(g_BtlMenuEnemySkillRowStep + g_BtlActiveActor * 0x700) = 2;
    *(int *)(g_BtlMenuEnemySkillTotalRows + g_BtlActiveActor * 0x700) = 0xc;
    *(int *)(g_BtlMenuEnemySkillColWrapMode + g_BtlActiveActor * 0x700) = MENU_CURSOR_WRAP_INTO_ROWS;
    *(int *)(g_BtlMenuEnemySkillRowWrapMode + g_BtlActiveActor * 0x700) = MENU_CURSOR_CLAMP;
    *(int *)(g_BtlMenuEnemySkillUnused8 + g_BtlActiveActor * 0x700) = 0;
    *(int *)(g_BtlMenuEnemySkillScrollAnim + g_BtlActiveActor * 0x700) = 0;
    *(int *)(g_BtlMenuEnemySkillScrollState + g_BtlActiveActor * 0x700) = MENU_SCROLL_NONE;
    *(int *)(g_BtlMenuEnemySkillPageScroll + g_BtlActiveActor * 0x700) = 1;
    Battle_RefreshPendingCommandAvailability(g_BtlActiveActor, 3);
}

/*
 * Runs the Enemy Skill cursor (state 4) over its two-column grid of eight-byte entries and picks
 * one, once. Battle_WindowFrameLimitList's shape with a grid rather than a list: the entry is
 * col + row * 2 + scrollTop * 2.
 */
// FUNCTION: FF7 0x006da3a8
void __cdecl Battle_WindowFrameEnemySkillList(void)
{
    unsigned char *p;
    MenuCursor *cursor;
    BattleAbilitySlot *rec;
    int pos;

    Battle_RefreshPendingCommandAvailability(g_BtlActiveActor, 3);
    p = (unsigned char *)g_MenuPartyStats[0].statBlock;
    rec = g_MenuPartyStats[g_BtlActiveActor].enemySkills;
    cursor = (MenuCursor *)&g_BtlMenuWindows[g_BtlActiveActor * 0x700];
    if (g_BattleMenuState == BATTLE_MENU_ENEMY_SKILL_LIST && g_BtlMenuSelectionConfirmed == 0) {
        MenuCursor_Update(cursor);
        if (cursor->scrollState == MENU_SCROLL_NONE) {
            if (BtlMenu_GetInputRepeat(INPUT_CIRCLE) != 0) {
                g_BtlMenuSelectionConfirmed = 1;
                pos = cursor->col + cursor->row * 2 + cursor->scrollTop * 2;
                if ((rec[pos].menuFlags & 2) == 0 && rec[pos].attackId != 0xff) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    g_BtlIssuedActionId = rec[pos].attackId;
                    g_BtlTargetFlags = rec[pos].targetFlags;
                    g_BtlListCursorIndex = (short)pos;
                    Battle_BeginTargetSelection();
                    g_BattleMenuState = BATTLE_MENU_SELECT_TARGET;
                    g_BattleWindowFrameId = BATTLE_MENU_ENEMY_SKILL_LIST;
                } else {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                }
            } else if (BtlMenu_GetInputRepeat(INPUT_CROSS) != 0) {
                g_BtlMenuSelectionConfirmed = 1;
                g_BattleMenuState = BATTLE_MENU_COMMAND_MENU;
                Battle_SetWindowStateClosing(BATTLE_WINDOW_ENEMY_SKILL_LIST);
            }
        }
    }
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006da515
void __cdecl Battle_WindowUnusedSlot4NoOp(void)
{
    /* The original body is empty. */
}

/*
 * Uses the limit count (byte 6 of the actor's limit record) to index the window-height table at
 * count-1 and set box 24 height/bottom. Stores that count for cursor setup,
 * substituting 1 for cursor limits when the displayed party slot is empty; the
 * height lookup still uses the original count.
 */
// FUNCTION: FF7 0x006da51a
void __cdecl Battle_WindowOpenLimitListResetCursor(void)
{
    MenuCursor *cursor;
    unsigned char *record;
    short n;

    record = g_MenuPartyStats[g_BtlActiveActor].limitAttacks;
    cursor = (MenuCursor *)&g_BtlMenuLimitCursor[g_BtlActiveActor * 0x700];
    n = record[6];
    g_BtlMenuLimitRowCount = n;
    if (gSavemap.party[g_BtlActivePartyMember] == PARTY_CHAR_NONE) {
        g_BtlMenuLimitRowCount = 1;
    }
    n -= 1;
    g_BattleMenuData[BATTLE_WINDOW_LIMIT_LIST].height = g_pBtlMenuScalingTable[n];
    g_BattleMenuData[BATTLE_WINDOW_LIMIT_LIST].sub[0].bottom =
        (short)(g_pBtlMenuScalingTable[n] - 3);
    if (g_BtlMenuOption == 0) {
        cursor->col = 0;
        cursor->row = 0;
        cursor->unused4 = 0;
        cursor->scrollTop = 0;
    }
    cursor->colCount = 1;
    cursor->rowCount = g_BtlMenuLimitRowCount;
    cursor->rowStep = 1;
    cursor->totalRows = g_BtlMenuLimitRowCount;
    cursor->colWrapMode = 0;
    if (g_BtlMenuLimitRowCount == 1) {
        cursor->rowWrapMode = 0;
    } else {
        cursor->rowWrapMode = 1;
    }
    cursor->unused8 = 0;
    cursor->scrollAnim = 0;
    cursor->scrollState = 0;
    cursor->pageScrollEnable = 0;
}

/*
 * Run the cursor over the limit list (state 0x18) and commit or cancel, once.
 * Battle_WindowFrameTifaReelWindow's shape with a cursor in front of it and no reel-state
 * veto.
 */
// FUNCTION: FF7 0x006da67e
void __cdecl Battle_WindowFrameLimitList(void)
{
    MenuCursor *cursor;
    int unused;
    unsigned char *record;

    record = g_MenuPartyStats[g_BtlActivePartyMember].limitAttacks;
    cursor = (MenuCursor *)&g_BtlMenuLimitCursor[g_BtlActivePartyMember * 0x700];
    if (g_BattleMenuState == BATTLE_MENU_LIMIT_LIST && g_BtlMenuSelectionConfirmed == 0) {
        MenuCursor_Update(cursor);
        if (BtlMenu_GetInputRepeat(INPUT_CIRCLE) != 0) {
            SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
            g_BtlTargetFlags = record[cursor->row + 3];
            g_BtlIssuedActionId = record[cursor->row];
            g_BtlListCursorIndex = (short)cursor->row;
            Battle_BeginTargetSelection();
            g_BattleMenuState = BATTLE_MENU_SELECT_TARGET;
            g_BattleWindowFrameId = BATTLE_MENU_LIMIT_LIST;
            g_BtlMenuSelectionConfirmed = 1;
        } else if (BtlMenu_GetInputRepeat(INPUT_CROSS) != 0) {
            SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
            g_BtlMenuSelectionConfirmed = 1;
            g_BattleMenuState = BATTLE_MENU_COMMAND_MENU;
            Battle_SetWindowStateClosing(BATTLE_WINDOW_LIMIT_LIST);
        }
    }
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006da786
void __cdecl Battle_WindowUnusedSlot15And24NoOp(void)
{
    /* The original body is empty. */
}

/*
 * Battle_WindowOpenTifaReelWindow -- PSX func_800E0274: init callback for the Tifa
 * limit reel window (0x1B), one-line delegate to the reel-count/arming
 * initializer.
 */
// FUNCTION: FF7 0x006da78b
void __cdecl Battle_WindowOpenTifaReelWindow(void)
{
    Menu_TifaReelArmAndBuildLimitList();
}

/*
 * Confirm or cancel out of Tifa's slot window (state 0x1B), once. Same shape as
 * Battle_WindowFrameSlot3ConfirmOrCancel one screen over: g_BtlMenuSelectionConfirmed is the
 * once-only latch, the guard is one `&&`, and the two arms are an `if` / `else if` on the two
 * buttons.
 */
// FUNCTION: FF7 0x006da795
void __cdecl Battle_WindowFrameTifaReelWindow(void)
{
    int unused;
    unsigned char *record;

    record = g_MenuPartyStats[g_BtlActivePartyMember].limitAttacks;
    if (g_BattleMenuState == BATTLE_MENU_TIFA_SLOTS && g_BtlMenuSelectionConfirmed == 0) {
        if (BtlMenu_GetInputRepeat(INPUT_CIRCLE) != 0) {
            if (Menu_TifaReelState() == 2) {
                g_BtlMenuSelectionConfirmed = 1;
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                g_BtlTargetFlags = record[3];
                g_BtlIssuedActionId = record[0];
                g_BtlListCursorIndex = 0;
                Battle_BeginTargetSelection();
                Battle_ConfirmSelectionAndPostCommand();
                Battle_BuildLimitComboResults();
                Battle_SetWindowStateClosing(BATTLE_WINDOW_TIFA_SLOTS);
                Battle_SetWindowStateClosing(BATTLE_WINDOW_COMMAND_MENU);
            }
        } else if (BtlMenu_GetInputRepeat(INPUT_CROSS) != 0) {
            if (Menu_TifaReelState() == 0) {
                SfxPlayOnChannel(0x40, 4, 4);
                g_BtlMenuSelectionConfirmed = 1;
                g_BattleMenuState = BATTLE_MENU_COMMAND_MENU;
                Battle_SetWindowStateClosing(BATTLE_WINDOW_TIFA_SLOTS);
            }
        }
    }
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006da889
void __cdecl Battle_WindowUnusedSlot27NoOp(void)
{
    /* The original body is empty. */
}

/*
 * Battle_WindowOpenCaitReelWindow -- PSX func_800E03D0: init callback for the Cait
 * Sith Slots window (0x1A), one-line delegate to the reel-arming function.
 */
// FUNCTION: FF7 0x006da88e
void __cdecl Battle_WindowOpenCaitReelWindow(void)
{
    Menu_CaitReelArm();
}

/*
 * Battle_WindowFrameTifaReelWindow for the Cait Sith slot window (state 0x1A). Three
 * differences beyond the state number and the reel-state call: the latch is set
 * before the state is asked rather than after, the cancel arm plays its sound
 * through SfxPreloadAndPlayOnChannel5 rather than SfxPlayOnChannel, and the tile
 * rebuild is Menu_BuildCaitReelResults rather than Battle_BuildLimitComboResults.
 */
// FUNCTION: FF7 0x006da898
void __cdecl Battle_WindowFrameCaitReelWindow(void)
{
    int unused;
    unsigned char *record;

    record = g_MenuPartyStats[g_BtlActivePartyMember].limitAttacks;
    if (g_BattleMenuState == BATTLE_MENU_CAIT_SITH_SLOTS && g_BtlMenuSelectionConfirmed == 0) {
        if (BtlMenu_GetInputRepeat(INPUT_CIRCLE) != 0) {
            g_BtlMenuSelectionConfirmed = 1;
            if (Menu_CaitReelState() == 2) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                g_BtlTargetFlags = record[3];
                g_BtlIssuedActionId = record[0];
                g_BtlListCursorIndex = 0;
                Battle_BeginTargetSelection();
                Battle_ConfirmSelectionAndPostCommand();
                Menu_BuildCaitReelResults();
                Battle_SetWindowStateClosing(BATTLE_WINDOW_CAIT_SITH_SLOTS);
                Battle_SetWindowStateClosing(BATTLE_WINDOW_COMMAND_MENU);
            }
        } else if (BtlMenu_GetInputRepeat(INPUT_CROSS) != 0) {
            if (Menu_CaitReelState() == 0) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                g_BtlMenuSelectionConfirmed = 1;
                g_BattleMenuState = BATTLE_MENU_COMMAND_MENU;
                Battle_SetWindowStateClosing(BATTLE_WINDOW_CAIT_SITH_SLOTS);
            }
        }
    }
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006da988
void __cdecl Battle_WindowUnusedSlot26NoOp(void)
{
    /* The original body is empty. */
}

/*
 * Initializes the cursor at g_BtlMenuSlotScreenCursor, requests state 3 for each nonzero
 * window-state entry from 1 through 0x1b, sets g_BtlMenuSlotScreenState to 5 and
 * refreshes the working list with Mini_SlotLoadReelSymbols.
 */
// FUNCTION: FF7 0x006da98d
void __cdecl Battle_WindowOpenSlotScreen(void)
{
    MenuCursor *cursor;
    int i;

    cursor = &g_BtlMenuSlotScreenCursor;
    MenuCursor_Init(cursor, 0, 0, 2, 1, 0, 0, 2, 1, 0, 0, 1, 0, 0, 0);
    for (i = 1; i < 28; i++) {
        if (g_BattleWindowStates[i] != BATTLE_WINDOW_STATE_CLOSED) {
            Battle_SetWindowStateClosing(i);
        }
    }
    g_BtlMenuSlotScreenState = BATTLE_SLOT_SCREEN_INTRO;
    Mini_SlotLoadReelSymbols();
}

/*
 * BATTLE_WINDOW_ARENA_SLOT's per-frame tick: a six-way state machine on g_BtlMenuSlotScreenState, every arm
 * gated on the same key. The jump table has six entries and BATTLE_SLOT_SCREEN_DONE is not one of
 * them -- its slot holds the default label, which is what MSVC fills a gap with.
 */
// FUNCTION: FF7 0x006daa08
void __cdecl Battle_WindowFrameSlotScreen(void)
{
    MenuCursor *cursor;

    cursor = &g_BtlMenuSlotScreenCursor;
    if (g_BattleMenuState == BATTLE_MENU_ARENA_SLOT) {
        switch (g_BtlMenuSlotScreenState) {
        case BATTLE_SLOT_SCREEN_CHOOSE:
            MenuCursor_Update(cursor);
            if (Input_GetPressed(INPUT_CIRCLE) != 0) {
                if (g_BtlMenuSlotScreenCursor.col == 0) {
                    Mini_SlotResetSubStateAndInputDelay();
                    g_BtlMenuSlotScreenState = BATTLE_SLOT_SCREEN_START_PROMPT;
                } else {
                    Battle_SetArenaSelectionResult(0);
                }
            }
            break;
        case BATTLE_SLOT_SCREEN_SPINNING:
            if (Input_GetPressed(INPUT_CIRCLE) != 0 && Mini_SlotGetCurrentReelStopFlag() != 0) {
                g_BtlMenuSlotScreenState = BATTLE_SLOT_SCREEN_RESULT;
            }
            break;
        case BATTLE_SLOT_SCREEN_START_PROMPT:
            if (Input_GetPressed(INPUT_CIRCLE) != 0) {
                Battle_PlaySfxCentered(SFX_ID_BATTLE_CMD_CHANGE);
                g_BtlMenuSlotScreenState = BATTLE_SLOT_SCREEN_SPINNING;
            }
            break;
        case BATTLE_SLOT_SCREEN_RESULT:
            if (Input_GetPressed(INPUT_CIRCLE) != 0) {
                Battle_SetArenaSelectionResult(1);
                Battle_SetWindowStateClosing(BATTLE_WINDOW_ARENA_SLOT);
                gSavemap.slotIndex = g_BtlMenuSlotIndex;
                g_BtlMenuSlotIndex++;
                g_BtlMenuSlotScreenState = BATTLE_SLOT_SCREEN_DONE;
            }
            break;
        case BATTLE_SLOT_SCREEN_INTRO:
            if (Input_GetPressed(INPUT_CIRCLE) != 0) {
                g_BtlMenuSlotScreenState = BATTLE_SLOT_SCREEN_CHOOSE;
            }
            break;
        }
    }
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006dab3f
void __cdecl Battle_WindowUnusedSlot28NoOp(void)
{
    /* The original body is empty. */
}

/*
 * The same shape as Battle_WindowOpenSlotScreen with a second cursor, a longer range, entry 9
 * skipped, and a flag raised at the end instead of a call. The skip is an `if`
 * with a `continue` in it: the image has `jne` over a `jmp` back to the increment,
 * which is what a continue compiles to.
 */
// FUNCTION: FF7 0x006dab44
void __cdecl Battle_WindowOpenWorriedChoice(void)
{
    MenuCursor *cursor;
    int i;

    cursor = &g_BtlMenuWorriedChoiceCursor;
    MenuCursor_Init(cursor, 0, 0, 2, 1, 0, 0, 2, 1, 0, 0, 1, 0, 0, 0);
    for (i = 1; i < 32; i++) {
        if (i == 9) {
            continue;
        }
        if (g_BattleWindowStates[i] != BATTLE_WINDOW_STATE_CLOSED) {
            Battle_SetWindowStateClosing(i);
        }
    }
    g_BtlMenuWorriedChoiceOpen = 1;
}

/*
 * While screen 9 is active, updates its cursor. A 0x20 input press passes 1 to
 * Battle_SetArenaSelectionResult for cursor entry zero and 0 otherwise, requests
 * state 3 for window 9, and clears g_BtlMenuWorriedChoiceOpen.
 */
// FUNCTION: FF7 0x006dabc5
void __cdecl Battle_WindowFrameWorriedChoice(void)
{
    MenuCursor *cursor;

    cursor = &g_BtlMenuWorriedChoiceCursor;
    if (g_BattleMenuState == BATTLE_MENU_ARENA_CONTINUE_CHOICE) {
        MenuCursor_Update(cursor);
        if (Input_GetPressed(INPUT_CIRCLE) != 0) {
            if (g_BtlMenuWorriedChoiceCursor.col == 0) {
                Battle_SetArenaSelectionResult(1);
            } else {
                Battle_SetArenaSelectionResult(0);
            }
            Battle_SetWindowStateClosing(BATTLE_WINDOW_ARENA_CONTINUE_CHOICE);
            g_BtlMenuWorriedChoiceOpen = 0;
        }
    }
}

/*
 * Expands the indexed 0x98-byte window record: index zero increases width by
 * original width/45 and height by one, others increase each dimension by one fifth
 * of its original. Marks g_BattleWindowStates[windowIndex] as 2 only when both exceed originals,
 * clamps dimensions to originals and recomputes the centered halfWidth/halfHeight offsets.
 */
// FUNCTION: FF7 0x006dac2d
void __cdecl Menu_BattleWindowOpenStep(short windowIndex)
{
    if (windowIndex == 0) {
        g_BattleMenuData[windowIndex].clipWidth += (short)(g_BattleMenuData[windowIndex].width / 45);
        (g_BattleMenuData[windowIndex].clipHeight)++;
    } else {
        g_BattleMenuData[windowIndex].clipWidth += (short)(g_BattleMenuData[windowIndex].width / 5);
        g_BattleMenuData[windowIndex].clipHeight += (short)(g_BattleMenuData[windowIndex].height / 5);
    }
    if (g_BattleMenuData[windowIndex].clipWidth > g_BattleMenuData[windowIndex].width && g_BattleMenuData[windowIndex].clipHeight > g_BattleMenuData[windowIndex].height) {
        g_BattleWindowStates[windowIndex] = BATTLE_WINDOW_STATE_OPEN;
    }
    if (g_BattleMenuData[windowIndex].clipWidth > g_BattleMenuData[windowIndex].width) {
        g_BattleMenuData[windowIndex].clipWidth = g_BattleMenuData[windowIndex].width;
    }
    if (g_BattleMenuData[windowIndex].clipHeight > g_BattleMenuData[windowIndex].height) {
        g_BattleMenuData[windowIndex].clipHeight = g_BattleMenuData[windowIndex].height;
    }
    g_BattleMenuData[windowIndex].halfWidth = g_BattleMenuData[windowIndex].width / 2 - g_BattleMenuData[windowIndex].clipWidth / 2;
    g_BattleMenuData[windowIndex].halfHeight = g_BattleMenuData[windowIndex].height / 2 - g_BattleMenuData[windowIndex].clipHeight / 2;
}

// Shrinks one battle window by a quarter of its step on each call. scaleX and scaleY each drop by a
// quarter of stepX and stepY in the indexed 0x98-byte record at g_BattleMenuData. The window's
// g_BattleWindowStates entry is cleared when both scales fall below one. Each scale is then clamped
// to at least one, and offsetX and offsetY center the window within the original dimensions (step
// over two minus scale over two).
// FUNCTION: FF7 0x006daef0
void __cdecl Menu_BattleWindowCloseStep(short windowIndex)
{
    g_BattleMenuData[windowIndex].clipWidth -= (short)(g_BattleMenuData[windowIndex].width / 4);
    g_BattleMenuData[windowIndex].clipHeight -= (short)(g_BattleMenuData[windowIndex].height / 4);
    if (g_BattleMenuData[windowIndex].clipWidth < 1 && g_BattleMenuData[windowIndex].clipHeight < 1) {
        g_BattleWindowStates[windowIndex] = BATTLE_WINDOW_STATE_CLOSED;
    }
    if (g_BattleMenuData[windowIndex].clipWidth < 1) {
        g_BattleMenuData[windowIndex].clipWidth = 1;
    }
    if (g_BattleMenuData[windowIndex].clipHeight < 1) {
        g_BattleMenuData[windowIndex].clipHeight = 1;
    }
    g_BattleMenuData[windowIndex].halfWidth = g_BattleMenuData[windowIndex].width / 2 - g_BattleMenuData[windowIndex].clipWidth / 2;
    g_BattleMenuData[windowIndex].halfHeight = g_BattleMenuData[windowIndex].height / 2 - g_BattleMenuData[windowIndex].clipHeight / 2;
}

/*
 * Menu_ClearBattleWindowStates -- PSX func_800E0DF4: zero the 32-entry battle menu
 * window-state array.
 */
// FUNCTION: FF7 0x006db0bc
void __cdecl Menu_ClearBattleWindowStates(void)
{
    short i;

    for (i = 0; i < 32; i++) {
        g_BattleWindowStates[i] = BATTLE_WINDOW_STATE_CLOSED;
    }
}

/*
 * Services the 32 battle windows and the active command confirmation/cancellation.
 * Closes the open windows for an empty active party slot, records a held Square,
 * and handles Triangle (reopen the queued command menu) before stepping opening and closing
 * windows through their animation helpers and running each open window's active handler;
 * an open BATTLE_WINDOW_ARENA_SLOT suppresses the other active handlers.
 */
// FUNCTION: FF7 0x006db0ee
void Battle_ProcessCommandMenuInput(void)
{
    struct {
        int unused;
        BattleInventorySlot *items;
        short i;
        MenuCursor *cursor;
    } v; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if (g_BattleWindowStates[BATTLE_WINDOW_COMMAND_MENU] != BATTLE_WINDOW_STATE_CLOSED && gSavemap.party[g_BtlActiveActor] == PARTY_CHAR_NONE) {
        for (v.i = 1; v.i < 32; v.i++) {
            if (g_BattleWindowStates[v.i] != BATTLE_WINDOW_STATE_CLOSED) {
                Battle_SetWindowStateClosing(v.i);
            }
        }
    }
    v.cursor = (MenuCursor *)(g_BtlMenuSlotTableA + g_BtlActiveActor * 0x700);
    v.items = g_BattleItemListBlock.slots;
    if (Input_GetHeld(INPUT_SQUARE) != 0) {
        g_BtlMenuSelectionConfirmed = 1;
    } else {
        g_BtlMenuSelectionConfirmed = 0;
    }
    if (g_BtlMenuSelectionConfirmed == 0 &&
        (g_BattleWindowStates[BATTLE_WINDOW_COMMAND_MENU] == BATTLE_WINDOW_STATE_OPEN || g_BattleWindowStates[BATTLE_WINDOW_MANIP_ACTION_LIST] == BATTLE_WINDOW_STATE_OPEN) &&
        g_BattleWindowStates[BATTLE_WINDOW_CAIT_SITH_SLOTS] != BATTLE_WINDOW_STATE_OPEN && g_BattleWindowStates[BATTLE_WINDOW_TIFA_SLOTS] != BATTLE_WINDOW_STATE_OPEN &&
        Input_GetPressed(INPUT_TRIANGLE) != 0) {
        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
        Battle_ReopenQueuedCommandMenu();
        g_BtlMenuSelectionConfirmed = 1;
        g_BattleMenuState = -1;
        for (v.i = 1; v.i < 32; v.i++) {
            if (g_BattleWindowStates[v.i] != BATTLE_WINDOW_STATE_CLOSED) {
                Battle_SetWindowStateClosing(v.i);
            }
        }
    }
    for (v.i = 0; v.i < 32; v.i++) {
        switch ((int)g_BattleWindowStates[v.i]) {
        case BATTLE_WINDOW_STATE_OPENING:
            Menu_BattleWindowOpenStep(v.i);
            break;
        case BATTLE_WINDOW_STATE_OPEN:
            if (v.i == BATTLE_WINDOW_ARENA_SLOT) {
                g_BtlMenuWindowActiveHandlers[v.i]();
            } else if (g_BattleWindowStates[BATTLE_WINDOW_ARENA_SLOT] == BATTLE_WINDOW_STATE_CLOSED) {
                g_BtlMenuWindowActiveHandlers[v.i]();
            }
            break;
        case BATTLE_WINDOW_STATE_CLOSING:
            Menu_BattleWindowCloseStep(v.i);
            break;
        case BATTLE_WINDOW_STATE_CLOSED:
            break;
        }
    }
    if (g_BattleMenuState == BATTLE_MENU_SELECT_TARGET) {
        Battle_LoadTargetMaskForMode();
        Battle_UpdateTargetSelection();
        g_BtlPointedTargetInvalid = 0;
        if (((unsigned short)g_BtlTargetAvailableMask & (1 << g_BtlPointedTarget)) == 0) {
            g_BtlPointedTargetInvalid = 1;
        }
        if (g_BattleItemListBlock.inventoryChanged != 0 &&
            (g_BtlSelectedCommand == BATTLE_CURSOR_ACTION_W_ITEM || g_BtlSelectedCommand == BATTLE_CURSOR_ACTION_ITEM || g_BtlSelectedCommand == BATTLE_CURSOR_ACTION_THROW)) {
            g_BattleItemListBlock.inventoryChanged = 0;
            g_BattleMenuState = g_BattleWindowFrameId;
            g_BattleTargetCursorActive = 1;
            g_BtlMenuSelectionConfirmed = 1;
            if (g_BattleWindowStates[BATTLE_WINDOW_PARTY_HUD] != BATTLE_WINDOW_STATE_CLOSED) {
                Battle_SetWindowStateClosing(BATTLE_WINDOW_PARTY_HUD);
            }
        } else if (g_BtlMenuSelectionConfirmed == 0) {
            if (BtlMenu_GetInputRepeat(INPUT_CIRCLE) != 0) {
                if (g_BattleScriptEffectFlag != 0 || g_BtlPointedTargetInvalid != 0) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                    g_BtlMenuSelectionConfirmed = 1;
                } else {
                    if (g_BattleCommandMenuPhase != 0) {
                        if (g_BattleCommandMenuPhase == 2) {
                            if (g_BtlSelectedCommand == BATTLE_CURSOR_ACTION_W_ITEM) {
                                v.items[v.cursor->row + v.cursor->scrollTop].count--;
                                if (v.items[v.cursor->row + v.cursor->scrollTop].count == 0) {
                                    v.items[v.cursor->row + v.cursor->scrollTop].id = 0xffff;
                                }
                            }
                            Battle_PostSavedThenCurrentCommandSelections();
                        } else {
                            if (g_BtlSelectedCommand == BATTLE_CURSOR_ACTION_W_ITEM) {
                                v.items[v.cursor->row + v.cursor->scrollTop].count--;
                                if (v.items[v.cursor->row + v.cursor->scrollTop].count == 0) {
                                    v.items[v.cursor->row + v.cursor->scrollTop].id = 0xffff;
                                }
                            }
                            g_BattleCommandMenuPhase++;
                            g_BtlMenuSelectionActionId = g_BtlIssuedActionId;
                            g_BtlMenuSelectionIndex = g_BtlListCursorIndex;
                            g_BtlMenuSelectionTargetMode = g_BtlTargetSelectMode;
                            g_BtlMenuSelectionTargetMask = g_BtlTargetAvailableMask;
                            g_BtlMenuSelectionSingleTarget = g_BtlSingleTargetIndex;
                            g_BtlMenuSelectionTargetGroup = g_BtlTargetGroupIndex;
                            g_BattleMenuState = g_BattleWindowFrameId;
                            g_BattleTargetCursorActive = 1;
                            g_BtlMenuSelectionConfirmed = 1;
                            if (g_BattleWindowStates[BATTLE_WINDOW_PARTY_HUD] != BATTLE_WINDOW_STATE_CLOSED) {
                                Battle_SetWindowStateClosing(BATTLE_WINDOW_PARTY_HUD);
                            }
                        }
                    } else {
                        if (g_BtlSelectedCommand == BATTLE_CURSOR_ACTION_ITEM || g_BtlSelectedCommand == BATTLE_CURSOR_ACTION_W_ITEM || g_BtlSelectedCommand == BATTLE_CURSOR_ACTION_THROW) {
                            v.items[v.cursor->row + v.cursor->scrollTop].count--;
                            if (v.items[v.cursor->row + v.cursor->scrollTop].count == 0) {
                                v.items[v.cursor->row + v.cursor->scrollTop].id = 0xffff;
                            }
                        }
                        Battle_ConfirmSelectionAndPostCommand();
                    }
                }
            } else if (BtlMenu_GetInputRepeat(INPUT_CROSS) != 0) {
                if (g_BattleCommandMenuPhase == 2 && g_BtlSelectedCommand == BATTLE_CURSOR_ACTION_W_ITEM) {
                    // BUG: cancelling the second W-Item pick refunds the first item by bumping the count of
                    // the slot it came from. The saved item id is put back only when that slot is empty, so
                    // if the inventory was rearranged and the slot now holds another item, that item gains
                    // one instead (the W-Item duplication bug).
                    if (v.items[(unsigned short)g_BtlMenuSelectionIndex].count == 0) {
                        v.items[(unsigned short)g_BtlMenuSelectionIndex].id = g_BtlMenuSelectionActionId;
                    }
                    v.items[(unsigned short)g_BtlMenuSelectionIndex].count++;
                    if (v.items[(unsigned short)g_BtlMenuSelectionIndex].count > 99) {
                        v.items[(unsigned short)g_BtlMenuSelectionIndex].count = 99;
                    }
                }
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                g_BattleMenuState = g_BattleWindowFrameId;
                g_BattleTargetCursorActive = 1;
                g_BtlMenuSelectionConfirmed = 1;
                if (g_BattleWindowStates[BATTLE_WINDOW_PARTY_HUD] != BATTLE_WINDOW_STATE_CLOSED) {
                    Battle_SetWindowStateClosing(BATTLE_WINDOW_PARTY_HUD);
                }
            }
        }
    }
}

/*
 * Reset the screen state: a dozen stores, two calls, and one value taken off the
 * play clock. gSavemapSeconds & 0x7F is what seeds g_BtlMenuSlotSeed, which
 * Mini_SlotLoadReelSymbols multiplies by 21 to pick a row.
 */
// FUNCTION: FF7 0x006db716
void __cdecl Battle_ResetMenuScreenState(void)
{
    int junk;

    g_MenuNonTopLevelScreenFlag = 1;
    g_BtlMenuSlotIndex = 0;
    g_BtlMenuSlotSeed = gSavemap.seconds & 0x7f;
    g_BtlMenuResetByteA = 0;
    Battle_StepPartyHpMpGaugeDisplay();
    g_BtlMenuLastSceneByte = g_BattleScene.readyActorMask;
    g_BtlMenuPanelRefreshFlag = 1;
    g_BattleMenuState = 0xffff;
    Menu_ClearBattleWindowStates();
    if (g_BtlMenuPanelRefreshFlag != 0) {
        g_BtlMenuPanelRefreshFlag = 0;
    }
    g_BattleIsPaused = 0;
    g_GamePauseLatch = 0;
    g_BtlMenuSavedMask = g_BattleScene.readyActorMask;
    g_BtlMenuOption = (gSavemap.options >> 4) & 3;
}

/*
 * When text is ready and five frame-batch quads are acquired, emits a separator
 * plus four decimal digits with palette 12, half-alpha white and depth 0.1.
 * Selects separator UV from the original value low bit before signed-clamping
 * values above 5999; signed-negative values are not clamped.
 */
// FUNCTION: FF7 0x006db7ab
void Battle_DrawCountdownTimer(int ignored, int posX, int y, unsigned int value)
{
    struct {
        GfxTlVertex *vtx15, *vtx14, *vtx13, *vtx12, *vtx11, *vtx10, *vtx9, *vtx8;
        GfxTlVertex *vtx7, *vtx6, *vtx5, *vtx4, *vtx3, *vtx2, *vtx1, *vtx0;
        int digit;
        float drawX;
        float dv, du;
        int index;
        float texV, texU;
        int unused[4];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (Menu_IsTextSubsystemReady() == 0)
        return;
    if (g_MenuIsSmallViewport320x240 != 1) {
        posX <<= 1;
        if (AcquireBatchNode(5, g_pBattle3dScene->frame1x) != 0) {
            frame.drawX = (float)posX + 64.0f;
            frame.texU = (value & 1) ? 0.3125f : 0.25f;
            frame.texV = 0.625f;
            frame.du = 0.0625f;
            frame.dv = 0.1640625f;
            frame.vtx0 = g_pBattle3dScene->frame1x->vertices;
            frame.vtx0->x = frame.drawX;
            frame.vtx0->y = (float)y;
            frame.vtx0->z = 0.1f;
            frame.vtx0->rhw = 1.0f;
            frame.vtx0->colorUnsigned = 0x80ffffff;
            frame.vtx0->specularUnsigned = 0xff000000;
            frame.vtx0->u = frame.texU;
            frame.vtx0->v = frame.texV;
            frame.vtx1 = g_pBattle3dScene->frame1x->vertices + 1;
            frame.vtx1->x = frame.drawX;
            frame.vtx1->y = (float)y + 42.0f;
            frame.vtx1->z = 0.1f;
            frame.vtx1->rhw = 1.0f;
            frame.vtx1->colorUnsigned = 0x80ffffff;
            frame.vtx1->specularUnsigned = 0xff000000;
            frame.vtx1->u = frame.texU;
            frame.vtx1->v = frame.texV + frame.dv;
            frame.vtx2 = g_pBattle3dScene->frame1x->vertices + 2;
            frame.vtx2->x = frame.drawX + 16.0f;
            frame.vtx2->y = (float)y;
            frame.vtx2->z = 0.1f;
            frame.vtx2->rhw = 1.0f;
            frame.vtx2->colorUnsigned = 0x80ffffff;
            frame.vtx2->specularUnsigned = 0xff000000;
            frame.vtx2->u = frame.texU + frame.du;
            frame.vtx2->v = frame.texV;
            frame.vtx3 = g_pBattle3dScene->frame1x->vertices + 3;
            frame.vtx3->x = frame.drawX + 16.0f;
            frame.vtx3->y = (float)y + 42.0f;
            frame.vtx3->z = 0.1f;
            frame.vtx3->rhw = 1.0f;
            frame.vtx3->colorUnsigned = 0x80ffffff;
            frame.vtx3->specularUnsigned = 0xff000000;
            frame.vtx3->u = frame.texU + frame.du;
            frame.vtx3->v = frame.texV + frame.dv;
            *g_pBattle3dScene->frame1x->palette = 12;
            g_pBattle3dScene->frame1x->paletteIndex = 12;
            WINDOW_NEXT_QUAD(g_pBattle3dScene->frame1x);
            g_pBattle3dScene->frame1x->palette++;
            if ((int)value > 5999)
                value = 5999;
            for (frame.index = 0; frame.index < 4; frame.index++) {
                frame.digit = (int)value / g_BattleCountdownDivisors[frame.index];
                frame.texU = (float)(((frame.digit % 5) << 5) + 96) * 0.00390625f;
                frame.texV = frame.digit > 4 ? 0.8125f : 0.625f;
                frame.du = 0.125f;
                frame.vtx4 = g_pBattle3dScene->frame1x->vertices;
                frame.vtx4->x = (float)posX;
                frame.vtx4->y = (float)y;
                frame.vtx4->z = 0.1f;
                frame.vtx4->rhw = 1.0f;
                frame.vtx4->colorUnsigned = 0x80ffffff;
                frame.vtx4->specularUnsigned = 0xff000000;
                frame.vtx4->u = frame.texU;
                frame.vtx4->v = frame.texV;
                frame.vtx5 = g_pBattle3dScene->frame1x->vertices + 1;
                frame.vtx5->x = (float)posX;
                frame.vtx5->y = (float)y + 42.0f;
                frame.vtx5->z = 0.1f;
                frame.vtx5->rhw = 1.0f;
                frame.vtx5->colorUnsigned = 0x80ffffff;
                frame.vtx5->specularUnsigned = 0xff000000;
                frame.vtx5->u = frame.texU;
                frame.vtx5->v = frame.texV + frame.dv;
                frame.vtx6 = g_pBattle3dScene->frame1x->vertices + 2;
                frame.vtx6->x = (float)posX + 32.0f;
                frame.vtx6->y = (float)y;
                frame.vtx6->z = 0.1f;
                frame.vtx6->rhw = 1.0f;
                frame.vtx6->colorUnsigned = 0x80ffffff;
                frame.vtx6->specularUnsigned = 0xff000000;
                frame.vtx6->u = frame.texU + frame.du;
                frame.vtx6->v = frame.texV;
                frame.vtx7 = g_pBattle3dScene->frame1x->vertices + 3;
                frame.vtx7->x = (float)posX + 32.0f;
                frame.vtx7->y = (float)y + 42.0f;
                frame.vtx7->z = 0.1f;
                frame.vtx7->rhw = 1.0f;
                frame.vtx7->colorUnsigned = 0x80ffffff;
                frame.vtx7->specularUnsigned = 0xff000000;
                frame.vtx7->u = frame.texU + frame.du;
                frame.vtx7->v = frame.texV + frame.dv;
                *g_pBattle3dScene->frame1x->palette = 12;
                g_pBattle3dScene->frame1x->paletteIndex = 12;
                WINDOW_NEXT_QUAD(g_pBattle3dScene->frame1x);
                g_pBattle3dScene->frame1x->palette++;
                if (frame.index == 1)
                    posX += 16;
                value = (int)value % g_BattleCountdownDivisors[frame.index];
                posX += 32;
            }
        }
    } else {
        if (AcquireBatchNode(5, g_pBattle3dScene->frame1x) != 0) {
            frame.drawX = (float)posX + 32.0f + g_MenuViewportX;
            y = y - 8 + g_MenuViewportY;
            frame.texU = (value & 1) ? 0.65625f : 0.625f;
            frame.texV = 0.3125f;
            frame.du = 0.03125f;
            frame.dv = 0.08203125f;
            frame.vtx8 = g_pBattle3dScene->frame1x->vertices;
            frame.vtx8->x = frame.drawX;
            frame.vtx8->y = (float)y;
            frame.vtx8->z = 0.1f;
            frame.vtx8->rhw = 1.0f;
            frame.vtx8->colorUnsigned = 0x80ffffff;
            frame.vtx8->specularUnsigned = 0xff000000;
            frame.vtx8->u = frame.texU;
            frame.vtx8->v = frame.texV;
            frame.vtx9 = g_pBattle3dScene->frame1x->vertices + 1;
            frame.vtx9->x = frame.drawX;
            frame.vtx9->y = (float)y + 21.0f;
            frame.vtx9->z = 0.1f;
            frame.vtx9->rhw = 1.0f;
            frame.vtx9->colorUnsigned = 0x80ffffff;
            frame.vtx9->specularUnsigned = 0xff000000;
            frame.vtx9->u = frame.texU;
            frame.vtx9->v = frame.texV + frame.dv;
            frame.vtx10 = g_pBattle3dScene->frame1x->vertices + 2;
            frame.vtx10->x = frame.drawX + 8.0f;
            frame.vtx10->y = (float)y;
            frame.vtx10->z = 0.1f;
            frame.vtx10->rhw = 1.0f;
            frame.vtx10->colorUnsigned = 0x80ffffff;
            frame.vtx10->specularUnsigned = 0xff000000;
            frame.vtx10->u = frame.texU + frame.du;
            frame.vtx10->v = frame.texV;
            frame.vtx11 = g_pBattle3dScene->frame1x->vertices + 3;
            frame.vtx11->x = frame.drawX + 8.0f;
            frame.vtx11->y = (float)y + 21.0f;
            frame.vtx11->z = 0.1f;
            frame.vtx11->rhw = 1.0f;
            frame.vtx11->colorUnsigned = 0x80ffffff;
            frame.vtx11->specularUnsigned = 0xff000000;
            frame.vtx11->u = frame.texU + frame.du;
            frame.vtx11->v = frame.texV + frame.dv;
            *g_pBattle3dScene->frame1x->palette = 12;
            g_pBattle3dScene->frame1x->paletteIndex = 12;
            WINDOW_NEXT_QUAD(g_pBattle3dScene->frame1x);
            g_pBattle3dScene->frame1x->palette++;
            if ((int)value > 5999)
                value = 5999;
            for (frame.index = 0; frame.index < 4; frame.index++) {
                frame.digit = (int)value / g_BattleCountdownDivisors[frame.index];
                frame.texU = (float)(((frame.digit % 5) << 4) + 176) * 0.00390625f;
                frame.texV = frame.digit > 4 ? 0.40625f : 0.3125f;
                frame.du = 0.0625f;
                frame.vtx12 = g_pBattle3dScene->frame1x->vertices;
                frame.vtx12->x = (float)posX + g_MenuViewportX;
                frame.vtx12->y = (float)y;
                frame.vtx12->z = 0.1f;
                frame.vtx12->rhw = 1.0f;
                frame.vtx12->colorUnsigned = 0x80ffffff;
                frame.vtx12->specularUnsigned = 0xff000000;
                frame.vtx12->u = frame.texU;
                frame.vtx12->v = frame.texV;
                frame.vtx13 = g_pBattle3dScene->frame1x->vertices + 1;
                frame.vtx13->x = (float)posX + g_MenuViewportX;
                frame.vtx13->y = (float)y + 21.0f;
                frame.vtx13->z = 0.1f;
                frame.vtx13->rhw = 1.0f;
                frame.vtx13->colorUnsigned = 0x80ffffff;
                frame.vtx13->specularUnsigned = 0xff000000;
                frame.vtx13->u = frame.texU;
                frame.vtx13->v = frame.texV + frame.dv;
                frame.vtx14 = g_pBattle3dScene->frame1x->vertices + 2;
                frame.vtx14->x = (float)posX + 16.0f + g_MenuViewportX;
                frame.vtx14->y = (float)y;
                frame.vtx14->z = 0.1f;
                frame.vtx14->rhw = 1.0f;
                frame.vtx14->colorUnsigned = 0x80ffffff;
                frame.vtx14->specularUnsigned = 0xff000000;
                frame.vtx14->u = frame.texU + frame.du;
                frame.vtx14->v = frame.texV;
                frame.vtx15 = g_pBattle3dScene->frame1x->vertices + 3;
                frame.vtx15->x = (float)posX + 16.0f + g_MenuViewportX;
                frame.vtx15->y = (float)y + 21.0f;
                frame.vtx15->z = 0.1f;
                frame.vtx15->rhw = 1.0f;
                frame.vtx15->colorUnsigned = 0x80ffffff;
                frame.vtx15->specularUnsigned = 0xff000000;
                frame.vtx15->u = frame.texU + frame.du;
                frame.vtx15->v = frame.texV + frame.dv;
                *g_pBattle3dScene->frame1x->palette = 12;
                g_pBattle3dScene->frame1x->paletteIndex = 12;
                WINDOW_NEXT_QUAD(g_pBattle3dScene->frame1x);
                g_pBattle3dScene->frame1x->palette++;
                if (frame.index == 1)
                    posX += 8;
                value = (int)value % g_BattleCountdownDivisors[frame.index];
                posX += 16;
            }
        }
    }
}

/*
 * Flush every battle-menu batch list in both passes, invalidate them all, release
 * the window texture's render object and set the viewport back to the full
 * 640x480. The pass-0 list is longer than the menu's own flush (Menu_FlushAndResetScissor2x) by
 * six battle-side batches.
 */
// FUNCTION: FF7 0x006dc1eb
void __cdecl Battle_FlushMenuBatches2x(void)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    Gfx_SetScenePassCall(0, ctx);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWindowBgGraphicsObject, ctx);
    Gfx_DrawRenderObjectCall(g_MenuFlatRectGraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuFontAGraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuFontBGraphicsObject.batchList, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinABlend4GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinBBlend4GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinCBlend4GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinCBlend4DiffGraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinDBlend4GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_BtlMenuRenderObjKetcy2a, ctx);
    Gfx_EmitPrimitiveBatchList(g_BtlMenuRenderObjKetcy2b, ctx);
    Gfx_EmitPrimitiveBatchList(g_BtlMenuRenderObjZeni, ctx);
    Gfx_EmitPrimitiveBatchList(g_BtlMenuRenderObjColoa, ctx);
    Gfx_EmitPrimitiveBatchList(g_BtlMenuRenderObjColob, ctx);
    Gfx_EmitPrimitiveBatchList(g_BtlMenuRenderObjColoc, ctx);
    Gfx_SetScenePassCall(1, ctx);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuBandGraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuBattleLimitGraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuBlendWindowBgGraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuGaugeBarGraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinABlend0GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuTextBoxQuadGraphicsObject, ctx);
    InvalidateBatchGeneration(g_BtlMenuRenderObjColoa);
    InvalidateBatchGeneration(g_BtlMenuRenderObjColob);
    InvalidateBatchGeneration(g_BtlMenuRenderObjColoc);
    InvalidateBatchGeneration(g_BtlMenuRenderObjKetcy2a);
    InvalidateBatchGeneration(g_BtlMenuRenderObjKetcy2b);
    InvalidateBatchGeneration(g_BtlMenuRenderObjZeni);
    InvalidateBatchGeneration(g_MenuFontAGraphicsObject);
    InvalidateBatchGeneration(g_MenuFontBGraphicsObject.batchList);
    InvalidateBatchGeneration(g_MenuBandGraphicsObject);
    InvalidateBatchGeneration(g_MenuBattleLimitGraphicsObject);
    InvalidateBatchGeneration(g_MenuBlendWindowBgGraphicsObject);
    InvalidateBatchGeneration(g_MenuGaugeBarGraphicsObject);
    InvalidateBatchGeneration(g_MenuWinABlend4GraphicsObject);
    InvalidateBatchGeneration(g_MenuWinBBlend4GraphicsObject);
    InvalidateBatchGeneration(g_MenuWinCBlend4GraphicsObject);
    InvalidateBatchGeneration(g_MenuWinCBlend4DiffGraphicsObject);
    InvalidateBatchGeneration(g_MenuWinDBlend4GraphicsObject);
    InvalidateBatchGeneration(g_MenuWinABlend0GraphicsObject);
    Gfx_EmptyRenderObjectBatch((int)g_MenuFlatRectGraphicsObject);
    InvalidateBatchGeneration(g_MenuWindowBgGraphicsObject);
    InvalidateBatchGeneration(g_MenuTextBoxQuadGraphicsObject);
    GfxSetViewportRect(0, 0, 640, 480, (GfxEngineContext *)ctx);
}

/*
 * The 320x240 twin of Battle_FlushMenuBatches2x: fewer lists per pass, two of them
 * inside the 0x7D4-byte block at g_BtlMenuDoubleBufferedCursorRecord, and the viewport back to the 1x
 * layout's rectangle rather than the full screen.
 */
// FUNCTION: FF7 0x006dc50d
void __cdecl Battle_FlushMenuBatches1x(void)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    Gfx_SetScenePassCall(0, ctx);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWindowBgGraphicsObject, ctx);
    Gfx_DrawRenderObjectCall(g_MenuFlatRectGraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuFontBlend4GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinBlend4GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinCBlend4DiffGraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_BtlMenuRenderObjKetcy2, ctx);
    Gfx_EmitPrimitiveBatchList(g_BtlMenuRenderObjZeni, ctx);
    Gfx_EmitPrimitiveBatchList(g_BtlMenuRenderObjColo, ctx);
    Gfx_SetScenePassCall(1, ctx);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuBandGraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuBattleLimitGraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuBlendWindowBgGraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuGaugeBarGraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinBlend0GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuTextBoxQuadGraphicsObject, ctx);
    InvalidateBatchGeneration(g_BtlMenuRenderObjColo);
    InvalidateBatchGeneration(g_BtlMenuRenderObjKetcy2);
    InvalidateBatchGeneration(g_BtlMenuRenderObjZeni);
    InvalidateBatchGeneration(g_MenuFontBlend4GraphicsObject);
    InvalidateBatchGeneration(g_MenuBandGraphicsObject);
    InvalidateBatchGeneration(g_MenuBattleLimitGraphicsObject);
    InvalidateBatchGeneration(g_MenuBlendWindowBgGraphicsObject);
    InvalidateBatchGeneration(g_MenuGaugeBarGraphicsObject);
    InvalidateBatchGeneration(g_MenuWinBlend4GraphicsObject);
    InvalidateBatchGeneration(g_MenuWinBlend0GraphicsObject);
    InvalidateBatchGeneration(g_MenuWinCBlend4DiffGraphicsObject);
    Gfx_EmptyRenderObjectBatch((int)g_MenuFlatRectGraphicsObject);
    InvalidateBatchGeneration(g_MenuWindowBgGraphicsObject);
    InvalidateBatchGeneration(g_MenuTextBoxQuadGraphicsObject);
    GfxSetViewportRect(g_MenuViewportX, g_MenuViewportY, g_MenuViewportWidth, g_MenuViewportHeight, (GfxEngineContext *)ctx);
}

/*
 * Clear the battle UI's state, one global at a time. Thirty-seven stores and no loop: widths are
 * whatever each global is, and three of them are chained copies rather than repeated literals --
 * g_BtlMenuSlotIndex takes g_BtlMenuSlotSeed after that was cleared, g_BtlMenuSlotScreenState takes
 * g_BtlMenuSlotIndex, and g_BtlMenuCursorX takes g_BtlMenuCursorY.
 */
// FUNCTION: FF7 0x006dc752
void __cdecl Battle_ResetMenuUiState(void)
{
    g_BtlMenuLimitCursorHidden = 0;
    g_BattleSelectedUseCondition = 0;
    g_BtlMenuResetShortA = 0;
    g_BtlMenuResetByteB = 0;
    g_BtlMenuResetByteC = 0;
    g_BtlMenuResetByteD = 0;
    g_BtlMenuMessageWindowRequest = 0;
    g_BtlMenuMessageWindowTextId = 0;
    g_BtlMenuSlotSeed = 0;
    g_BtlMenuSlotIndex = g_BtlMenuSlotSeed;
    g_BtlMenuSlotScreenState = g_BtlMenuSlotIndex;
    g_BtlMenuSelectionConfirmed = 0;
    g_BtlMenuSelectedFlags = 0;
    g_BtlPointedTargetInvalid = 0;
    g_BtlMenuPanelRefreshFlag = 0;
    g_BtlMenuResetWordA = 0;
    g_BtlMenuResetWordB = 0;
    g_BattleSelectionCursorState = -1;
    g_BtlMenuCursorY = 0;
    g_BtlMenuCursorX = g_BtlMenuCursorY;
    g_BattleTargetCursorActive = 0;
    g_BtlMenuTargetCursorState = 0;
    g_BtlMenuPanelClearShort = 0;
    g_BtlMenuMessageWindowAtRequest = 0;
    g_BtlMenuMessageWindowAtX = 0;
    g_BtlMenuMessageWindowAtY = 0;
    g_BtlMenuDrawWindowIndex = 0;
    g_BtlMenuSelectionActionId = 0;
    g_BtlMenuSelectionIndex = 0;
    g_BtlMenuSelectionTargetMode = 0;
    g_BtlMenuSelectionTargetMask = 0;
    g_BtlMenuSelectionSingleTarget = 0;
    g_BtlMenuSelectionTargetGroup = 0;
    g_BtlMenuControlledActionStarted = 0;
    g_BtlMenuResetByteA = 0;
    g_BtlMenuLastSceneByte = 0;
    g_BtlMenuGilAmount = 0;
}

/*
 * Selects three battle UI resource pointers for scaling mode 2 or the alternate
 * layout, calls Battle_ResetHudGaugeAndReelState and Battle_ResetMenuUiState, then
 * clears 0x1500 bytes of g_BtlMenuWindowSets.
 */
// FUNCTION: FF7 0x006dc89a
void Battle_SelectMenuUiTablesForScalingMode(void)
{
    if (GetUiScalingMode() == 2) {
        g_BattleMenuData = g_BattleWindowTable2x;
        g_BattleScalingTable = (short *)&g_BtlMenuWindowColors[6];
        g_pBtlMenuScalingTable = g_BtlMenuLimitListHeights2x;
    } else {
        g_BattleMenuData = (BattleUiBox *)g_BattleWindowTable1x;
        g_BattleScalingTable = g_BtlMenuCommandWidths1x;
        g_pBtlMenuScalingTable = g_BtlMenuLimitListHeights1x;
    }
    Battle_ResetHudGaugeAndReelState();
    Battle_ResetMenuUiState();
    memset(g_BtlMenuWindowSets, 0, 0x1500);
}
} /* extern "C" */
