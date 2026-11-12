// src/ff7/src/battle/yasui/vahamut0.cpp
/* Summon [14]: Bahamut ZERO (Japanese: バハムート零式, orig. vahamut0.cpp) */
// Original: C:\FF7\src\Battle\yasui\vahamut0.cpp
// Contents: Bahamut Zero summon effect: resource loading, a camera script on two tracks, a master
//   timeline that runs a table of sub-tasks, and drawing of sprite trails, sparks, glow, sky dome
//   and energy tube.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "lib/api/mem.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle_boss.h"
#include "ff7/camera_script.h"
#include "lib/codegen.h"
#include <string.h>
#include <stdlib.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "lib/api/file.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/enums/graphics.h"
#include <stdio.h>
extern "C" {

struct GfxRotationAngles2;
struct GfxPsxSpriteQuadState;
struct BattleFxModelDraw;

#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x007f6088
unsigned char g_BattleSummonBahamutZeroSfxGroup0[16] = {0xa2, 0x02, 0x00, 0x00, 0xa3, 0x02, 0x00, 0x00, 0xa4, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007f6098
unsigned char g_BattleSummonBahamutZeroSfxGroup1[16] = {0xa5, 0x02, 0x00, 0x00, 0xa6, 0x02, 0x00, 0x00, 0xa7, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007f60a8
unsigned char g_BattleSummonBahamutZeroSfxGroup2[16] = {0xa8, 0x02, 0x00, 0x00, 0xa9, 0x02, 0x00, 0x00, 0xaa, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007f60b8
unsigned char g_BattleSummonBahamutZeroSfxGroup3[16] = {0xab, 0x02, 0x00, 0x00, 0xac, 0x02, 0x00, 0x00, 0xad, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007f60c8
unsigned char g_BattleSummonBahamutZeroSfxGroup4[16] = {0xae, 0x02, 0x00, 0x00, 0xaf, 0x02, 0x00, 0x00, 0xb0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007f60d8
unsigned int g_BahamutZeroSoundGroupPointers[6] = {(unsigned int)&g_BattleSummonBahamutZeroSfxGroup0, (unsigned int)&g_BattleSummonBahamutZeroSfxGroup1, (unsigned int)&g_BattleSummonBahamutZeroSfxGroup2, (unsigned int)&g_BattleSummonBahamutZeroSfxGroup3, (unsigned int)&g_BattleSummonBahamutZeroSfxGroup4, 0x00000000};
unsigned char g_BattleVahamut0DataPool[4] = {0x23, 0x20, 0x00, 0x00};

// The frame limit of the ground-spark slots: BattleFx_BzDrawGroundSpark reads its low byte as the
// object's base operand. Retail holds 0x10 here; without a definition the rebuild left 0
// (padding before g_rawData_007f60f8). PSX: VAHAMUT0.BIN and STING.BIN both
// start an object here (lead only).
// GLOBAL: FF7 0x007f60f4
int g_BattleSummonBahamutZeroSparkFrameLimit = 16;
unsigned char g_rawData_007f60f8[384] = {
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xe0, 0xff, 0x00, 0x00, 0x00, 0x00, 0xac, 0x00, 0xf0, 0x3f, 0x40, 0x40, 0x40, 0x40,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xe0, 0xff, 0x40, 0x00, 0x00, 0x00, 0xac, 0x00, 0xf0, 0x3f, 0x40, 0x40, 0x40, 0x40,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xe0, 0xff, 0x80, 0x00, 0x00, 0x00, 0xac, 0x00, 0xf0, 0x3f, 0x40, 0x40, 0x40, 0x40,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0xe0, 0xff, 0xe0, 0xff, 0xc0, 0x00, 0x00, 0x00, 0xac, 0x00, 0xf0, 0x3f, 0x40, 0x3f, 0x40, 0x40,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xe0, 0xff, 0x00, 0x00, 0x40, 0x00, 0xac, 0x00, 0xf0, 0x3f, 0x40, 0x40, 0x40, 0x40,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xe0, 0xff, 0x40, 0x00, 0x40, 0x00, 0xac, 0x00, 0xf0, 0x3f, 0x40, 0x40, 0x40, 0x40,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xe0, 0xff, 0x80, 0x00, 0x40, 0x00, 0xac, 0x00, 0xf0, 0x3f, 0x40, 0x40, 0x40, 0x40,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0xe0, 0xff, 0xe0, 0xff, 0xc0, 0x00, 0x40, 0x00, 0xac, 0x00, 0xf0, 0x3f, 0x40, 0x3f, 0x40, 0x40,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xe0, 0xff, 0x00, 0x00, 0x80, 0x00, 0xac, 0x00, 0xf0, 0x3f, 0x40, 0x40, 0x40, 0x40,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xe0, 0xff, 0x40, 0x00, 0x80, 0x00, 0xac, 0x00, 0xf0, 0x3f, 0x40, 0x40, 0x40, 0x40,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xe0, 0xff, 0x80, 0x00, 0x80, 0x00, 0xac, 0x00, 0xf0, 0x3f, 0x40, 0x40, 0x40, 0x40,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0xe0, 0xff, 0xe0, 0xff, 0xc0, 0x00, 0x80, 0x00, 0xac, 0x00, 0xf0, 0x3f, 0x40, 0x3f, 0x40, 0x40,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0xe0, 0xff, 0xe0, 0xff, 0x00, 0x00, 0xc0, 0x00, 0xac, 0x00, 0xf0, 0x3f, 0x40, 0x40, 0x40, 0x3d,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0xe0, 0xff, 0xe0, 0xff, 0x40, 0x00, 0xc0, 0x00, 0xac, 0x00, 0xf0, 0x3f, 0x40, 0x40, 0x40, 0x3d,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0xe0, 0xff, 0xe0, 0xff, 0x80, 0x00, 0xc0, 0x00, 0xac, 0x00, 0xf0, 0x3f, 0x40, 0x40, 0x40, 0x3d,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0xe0, 0xff, 0xe0, 0xff, 0xc0, 0x00, 0xc0, 0x00, 0xac, 0x00, 0xf0, 0x3f, 0x40, 0x3f, 0x40, 0x3d
};

// GLOBAL: FF7 0x007f6278
unsigned int g_BattleSummonBahamutZeroResourceTable[169] = {0x00192a39, (unsigned int)"LENS02.TIM", (unsigned int)"LENS_1.SPT", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"LENS01.TIM", (unsigned int)"LENS_2.SPT", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"LENS02.TIM", (unsigned int)"LENS_3.SPT", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"LENS02.TIM", (unsigned int)"LENS_4.SPT", 0x00000000, 0x00000000, 0x00c800b4, (unsigned int)"EARTH_1.RSD", 0x00000000, 0x00000000, 0x00000000, 0x00c800b1, (unsigned int)"EARTH_2.RSD", 0x00000000, 0x00000000, 0x00000000, 0x00c840b1, (unsigned int)"SUN_1.RSD", 0x00000000, 0x00000000, 0x00000000, 0x00c840b1, (unsigned int)"SUN_2.RSD", 0x00000000, 0x00000000, 0x00000000, 0x00c840b1, (unsigned int)"BRESS_1.RSD", 0x00000000, 0x00000000, 0x00000000, 0x00c840b1, (unsigned int)"BRESS_2.RSD", 0x00000000, 0x00000000, 0x00000000, 0x00c840b1, (unsigned int)"BRESS_3.RSD", 0x00000000, 0x00000000, 0x00000000, 0x00c840b1, (unsigned int)"BRESS_4.RSD", 0x00000000, 0x00000000, 0x00000000, 0x00c840b1, (unsigned int)"BRESS_5.RSD", 0x00000000, 0x00000000, 0x00000000, 0x00c840b1, (unsigned int)"BRESS_6.RSD", 0x00000000, 0x00000000, 0x00000000, 0x00c840b1, (unsigned int)"BRESS_7.RSD", 0x00000000, 0x00000000, 0x00000000, 0x00c840b1, (unsigned int)"BRESS_8.RSD", 0x00000000, 0x00000000, 0x00000000, 0x0bb840b1, (unsigned int)"TERA_1.RSD", 0x00000000, 0x00000000, 0x00000000, 0x0fa000b1, (unsigned int)"STER.RSD", 0x00000000, 0x00000000, 0x00000000, 0x00cd00b1, (unsigned int)"KIRA_1.RSD", 0x00000000, 0x00000000, 0x00000000, 0xc35000b1, (unsigned int)"K01.RSD", 0x00000000, 0x00000000, 0x00000000, 0x00c840b1, (unsigned int)"SONIC_2.RSD", 0x00000000, 0x00000000, 0x00000000, 0x00c840b1, (unsigned int)"LINE_1.RSD", 0x00000000, 0x00000000, 0x00000000, 0x00c840b1, (unsigned int)"TERA_2.RSD", 0x00000000, 0x00000000, 0x00000000, 0x0019f83c, (unsigned int)"SKY_A.TIM", 0x00000000, 0x00000000, 0x00000000, 0x0019f839, (unsigned int)"SKY_A.TIM", 0x00000000, 0x00000000, 0x00000000, 0x0019783c, (unsigned int)"SKY_B.TIM", 0x00000000, 0x00000000, 0x00000000, 0x00197839, (unsigned int)"SKY_B.TIM", 0x00000000, 0x00000000, 0x00000000, 0x0019783c, (unsigned int)"BOMB_1.TIM", 0x00000000, 0x00000000, 0x00000000, 0x00197839, (unsigned int)"BOMB_1.TIM", 0x00000000, 0x00000000, 0x00000000, 0x00197839, (unsigned int)"COSMO_2.TIM", 0x00000000, 0x00000000, 0x00000000, 0x00197839, (unsigned int)"NEBULA_1.TIM", 0x00000000, 0x00000000, 0x00000000, 0x00197839, (unsigned int)"NEBULA_2.TIM", 0x00000000, 0x00000000, 0x00000000, 0x00197839, (unsigned int)"NEBULA_3.TIM", 0x00000000, 0x00000000, 0x00000000, 0x00197839, (unsigned int)"EXP2.TIM", 0x00000000, 0x00000000};
// Retail .data has SUMMON/VAHAMUT3 (BattleFx_SummonLoadResBahamutZero) and then the source path of
// BattleFx_BzBuildEnergyTube's GameHeapAlloc right after the table's names (0x7F6BF8, 0x7F6C08), ahead of
// BattleFx_BzMasterTimeline's literals, while BzBuildEnergyTube's code is near the end of the TU: a literal
// returned by a static __inline function is a parse-time unit at the function's definition, and the call
// expands to the plain push under /Ob1.
static __inline char *BattleFx_BahamutZeroResourceDir(void)
{
    // STRING: FF7 0x007f6bf8
    return "SUMMON/VAHAMUT3";
}
static __inline char *BattleFx_BahamutZeroSourceFile(void)
{
    // STRING: FF7 0x007f6c08
    return "C:\\FF7\\src\\Battle\\yasui\\vahamut0.cpp";
}
// GLOBAL: FF7 0x007f6520
unsigned char g_BahamutZeroCameraTimelineScript[418] = {0x08, 0x00, 0x00, 0x01, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x38, 0xff, 0x30, 0xf8, 0x38, 0xff, 0x80, 0x00, 0x22, 0x00, 0x45, 0x00, 0x22, 0x00, 0x20, 0xfd, 0x48, 0xf4, 0xd0, 0x8a, 0x48, 0xf4, 0x08, 0x00, 0x00, 0x02, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x9c, 0xff, 0x30, 0xf8, 0xce, 0xff, 0x80, 0x00, 0x14, 0x00, 0x45, 0x00, 0x14, 0x00, 0x80, 0x02, 0x18, 0xfc, 0xd0, 0x8a, 0x0c, 0xfe, 0x45, 0x00, 0x5a, 0x00, 0x01, 0xfe, 0x1e, 0xfb, 0x5c, 0xc7, 0x72, 0xed, 0x85, 0x00, 0x5a, 0x00, 0x01, 0xfe, 0x12, 0xfd, 0x36, 0xf7, 0x00, 0x00, 0x45, 0x00, 0x5a, 0x00, 0xff, 0x01, 0x24, 0xfa, 0xd8, 0xdc, 0xd8, 0xdc, 0x85, 0x00, 0x5a, 0x00, 0xff, 0x01, 0x24, 0xfa, 0x6c, 0xee, 0x00, 0x00, 0x80, 0x00, 0x1e, 0x00, 0x40, 0x00, 0x1e, 0x00, 0x45, 0x00, 0x50, 0x00, 0xff, 0x01, 0x00, 0x00, 0x30, 0xc6, 0x68, 0xc5, 0x86, 0x00, 0x03, 0x00, 0xff, 0xff, 0x82, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x28, 0x00, 0x87, 0x00, 0x81, 0x00, 0xc0, 0xf9, 0x1a, 0xd2, 0xfc, 0xd6, 0x41, 0x00, 0x40, 0xed, 0xfa, 0x00, 0x3c, 0xdd, 0x80, 0x00, 0x14, 0x00, 0x40, 0x00, 0x14, 0x00, 0x81, 0x00, 0x40, 0x06, 0x1a, 0xd2, 0xfc, 0xd6, 0x41, 0x00, 0xc0, 0x12, 0xfa, 0x00, 0x3c, 0xdd, 0x80, 0x00, 0x1e, 0x00, 0x40, 0x00, 0x1e, 0x00, 0x81, 0x00, 0x00, 0x00, 0xf0, 0xd8, 0xf0, 0xd8, 0x41, 0x00, 0x00, 0x00, 0xf0, 0xd8, 0xf0, 0xd8, 0x80, 0x00, 0x2d, 0x00, 0x42, 0x00, 0x2d, 0x00, 0x00, 0x00, 0xf0, 0xd8, 0x78, 0xec, 0x81, 0x00, 0x00, 0x00, 0x4a, 0xca, 0xbc, 0xda, 0x41, 0x00, 0x40, 0xfc, 0x4a, 0xca, 0x1c, 0xe4, 0x80, 0x00, 0x44, 0x00, 0x40, 0x00, 0x3f, 0x00, 0x45, 0x00, 0x0a, 0x00, 0x01, 0xfe, 0xc0, 0xe0, 0xba, 0xc8, 0xdc, 0xdd, 0x82, 0x00, 0x05, 0x00, 0x80, 0xf3, 0x6a, 0xcd, 0xdc, 0xdd, 0x80, 0x00, 0x11, 0x00, 0x40, 0x00, 0x11, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x30, 0xf8, 0x80, 0xc1, 0xb0, 0xb9, 0x80, 0x00, 0x37, 0x00, 0x42, 0x00, 0x37, 0x00, 0x18, 0xfc, 0xf0, 0xd8, 0x38, 0xcd, 0x81, 0x00, 0x00, 0x00, 0xe0, 0xb1, 0x70, 0xfe, 0x41, 0x00, 0x00, 0x00, 0x80, 0xc1, 0xd4, 0xfe, 0x80, 0x00, 0x46, 0x00, 0x40, 0x00, 0x46, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x78, 0xec, 0x90, 0xe8, 0x78, 0xec, 0x80, 0x00, 0x0a, 0x00, 0x42, 0x00, 0x0a, 0x00, 0xe0, 0xb1, 0xe0, 0xb1, 0x68, 0xc5, 0x80, 0x00, 0x5a, 0x00, 0x42, 0x00, 0x5a, 0x00, 0xe0, 0xb1, 0xe0, 0xb1, 0x68, 0xc5, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x68, 0xc5, 0xf0, 0xd8, 0x78, 0xec, 0x80, 0x00, 0x14, 0x00, 0x40, 0x00, 0x14, 0x00, 0xff, 0xff};
// GLOBAL: FF7 0x007f66c8
BattleSummonBahamutZeroSlot04And05Data g_BattleSummonBahamutZeroSlot04And05 = {0, {0x80, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}, 0, {0x88, 0, 0, 0, 0, 0}, 0};
unsigned char g_rawData_007f66e4[4] = {0x20, 0x00, 0x00, 0x00};

// GLOBAL: FF7 0x007f66e8
unsigned char g_BattleSummonBahamutZeroSlot04Scale[16] = {0x00, 0x30, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007f66f8
unsigned char g_BattleSummonBahamutZeroSlot05Scale[16] = {0x00, 0x30, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// GLOBAL: FF7 0x007f6708
unsigned char g_BattleSummonBahamutZeroSlot04Params[8] = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00};
unsigned char g_rawData_007f6710[8] = {0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007f6718
int g_BattleSummonBahamutZeroSlot11LocalMatrix = 2048;
unsigned char g_rawData_007f6720[24] = {0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x98, 0x3a, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007f6738
struct VECTOR g_BattleSummonBahamutZeroSlot11Scale = {3584, 3584, 3584, 0};

// GLOBAL: FF7 0x007f6748
BattleFxBzSpriteParams g_BattleSummonBahamutZeroSpriteParams = {{0}, 0, {0}, 0, {0}, 0x01000100, 0x64808080, {0x3a, 0}, 0x7888};
// GLOBAL: FF7 0x007f6760
BattleFxBzSpriteParams g_BattleSummonBahamutZeroTrailSprites[3] = {{{0}, 0, {0}, 0, {0}, 0x00600060, 0x66808080, {0xbb, 0}, 0x3fb0}, {{0}, 0, {0}, 0, {0}, 0x00800080, 0x66808080, {0xbb, 0}, 0x3f70}, {{0}, 0, {0}, 0, {0}, 0x00400040, 0x66808080, {0xbc, 0}, 0x3f30}};
// GLOBAL: FF7 0x007f67a0
BattleFxBzTrailOrigin g_BattleSummonBahamutZeroTrailOrigins[3] = {{200, 900}, {800, 500}, {500, 100}};
// GLOBAL: FF7 0x007f67b0
BattleSummonBahamutZeroSlot06And07Data g_BattleSummonBahamutZeroSlot06And07 = {0, {0xa8, 0, 0, 0, 0, 0, 0, 0, 0x20, 0, 0, 0}, 0};
unsigned char g_rawData_007f67c4[4] = {0xa8, 0x00, 0x00, 0x00};
unsigned char g_rawData_007f67c8[8] = {0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// clang-format on

// clang-format off
/* The Bahamut ZERO glow sprites: the table index and radial offset of each of the five glows, then
 * the head of the glow sprite draw request each glow fills in turn. */
typedef struct {
    BattleSummonBahamutZeroGlowOffset offsets[6];
    int spriteFrameTable;
    int spriteValue;
} BattleSummonBahamutZeroGlowTable;
// GLOBAL: FF7 0x007f67d0
BattleSummonBahamutZeroGlowTable g_BattleSummonBahamutZeroGlowOffsets = {{{1, 2000}, {2, 1500}, {4, 1000}, {3, 500}, {4, 200}, {0}}, 0, 0};
// clang-format on
// GLOBAL: FF7 0x007f67f0
struct CVECTOR g_BattleSummonBahamutZeroGlowShadedColor[2] = {{0x80, 0x80, 0x80, 0x2e}, {0}};
// GLOBAL: FF7 0x007f67f8
struct BattleSummonBahamutZeroGlowBaseColor g_BattleSummonBahamutZeroGlowBaseColor = {{0x80, 0x80, 0x80, 0x2e}, 0};
// clang-format off
// GLOBAL: FF7 0x007f6800
BattleSummonBahamutZeroSlot08Data g_BattleSummonBahamutZeroSlot08 = {{0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 3, 0, 0, 0}, 0};
unsigned char g_rawData_007f6814[4] = {0xa8, 0x00, 0x00, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x007f6818
BattleSummonBahamutZeroSlot09Data g_BattleSummonBahamutZeroSlot09 = {{0, 0, 0, 0, 0x20, 0, 0, 0}, 0, {0xa8, 0, 0, 0, 0, 0}, 0};
unsigned char g_rawData_007f682c[4] = {0x20, 0x00, 0x00, 0x00};
// clang-format on

// GLOBAL: FF7 0x007f6830
struct BattleSummonBahamutZeroSlot0AParams g_BattleSummonBahamutZeroSlot0AParams = {{0, 0, 0xf0, 0xd8, 0xf0, 0xd8, 0, 0}, 0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}, 0, {0}, 0, {0}, 0, {0}};
unsigned char g_rawData_007f685c[4] = {0x00, 0x00, 0x96, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x007f6860
BattleFxBzPillarQuadSlot g_BattleSummonBahamutZeroSlot0BDraw = {{0, 0, 0x12, 0xfd, 0xff, 0xff, 0, 0}, 0};
unsigned char g_rawData_007f686c[4] = {0xa8, 0x00, 0x00, 0x00};
// clang-format on

unsigned char g_rawData_007f6870[8] = {0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007f6878
int g_BattleSummonBahamutZeroSlot0BAttachOffset[4] = {9830400, 64786, 0, 0};
// GLOBAL: FF7 0x007f6888
short g_BattleFxBzEmitTilePrimVertex1[4] = {(-50), 400, 0, 0};
// GLOBAL: FF7 0x007f6890
struct SVECTOR g_BattleFxBzEmitTilePrimVertex2 = {50, 400, 0, 0};
// GLOBAL: FF7 0x007f6898
struct BattleLimitFuryBrandModelReqA g_BattleFxBzDrawRequest0C = {0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x007f68a8
struct BattleLimitFuryBrandModelReqA g_BattleFxBzDrawRequest0D = {0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x007f68b8
int g_BattleFxBzDrawRequest10[4] = {0, 0xa8, 0, 0x20};
// GLOBAL: FF7 0x007f68c8
unsigned char g_BattleSummonBahamutZeroQuadSize[4] = {0x00, 0x02, 0x00, 0x08};
unsigned char g_rawData_007f68cc[4] = {0x00, 0x02, 0x00, 0x00};
// clang-format on

// GLOBAL: FF7 0x007f68d0
BattleSummonBahamutZeroSlot0ERandomData g_BattleSummonBahamutZeroSlot0ERandom = {0, {0x88, 0, 0, 0, 0, 0}, 0};
unsigned char g_rawData_007f68dc[4] = {0x20, 0x00, 0x00, 0x00};
// clang-format on

// GLOBAL: FF7 0x007f68e0
struct GfxRotationAngles2 g_BattleSummonBahamutZeroSlot0EAngles[2] = {{-512, 2048, 0}, {0}};
unsigned char g_rawData_007f68ec[4] = {0xa8, 0x00, 0x00, 0x00};
// clang-format on
#define g_BattleBahamutZeroSlot0FDrawHandle (((BattleFxBzCoreQuadSlot *)&g_BattleSummonBahamutZeroSlot0EAngles)->bahamutZeroSlot0FDrawHandle)

unsigned char g_rawData_007f68f0[8] = {0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007f68f8
struct SVECTOR g_BattleSummonBahamutZeroSlot0FAngles = {-512, 2048, 0, 0};
// GLOBAL: FF7 0x007f6900
struct BattleSummonBahamutZeroScreenPrim g_BattleSummonBahamutZeroScreenPrim = {{0, 0, 0, 3, 0, 0, 0, 0x60}, 0, 0};
// GLOBAL: FF7 0x007f690c
BattleSummonBahamutZeroScreenPrimSize g_BattleSummonBahamutZeroScreenSize = {640, 332};
// GLOBAL: FF7 0x007f6910
BattleSummonBahamutZeroSkyLmdPtrs g_BattleSummonBahamutZeroSkyLmdPtrs = {0, {0xa8, 0, 0, 0, 0, 0, 0, 0, 0x20, 0, 0, 0}, 0};
unsigned char g_rawData_007f6924[4] = {0xa8, 0x00, 0x00, 0x00};
// clang-format on

unsigned char g_rawData_007f6928[8] = {0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007f6930
int g_BattleFxBzDrawRequest12[4] = {0, 0x88, 0, 0x20};
// GLOBAL: FF7 0x007f6940
BattleSummonBahamutZeroRingModelData g_BattleSummonBahamutZeroRingModel = {0, {0xa8, 0, 0, 0, 0, 0, 0, 0, 0x20, 0, 0, 0}, 0, {0}, 0, {0}, 0, {0}};
unsigned char g_rawData_007f6964[4] = {0x00, 0x00, 0xb0, 0xfd};
unsigned char g_rawData_007f6968[8] = {0xff, 0xff, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x007f6970
BattleFxFlareSpriteSlot g_BattleFxBahamutZeroSprite = {{0x80, 0x80, 0x80, 0x3e, 0x80, 0x80, 0x80, 0}, 0, {0xa8, 0, 0, 0, 0, 0}, 0};
unsigned char g_rawData_007f6984[4] = {0x20, 0x00, 0x00, 0x00};
// clang-format on

// GLOBAL: FF7 0x007f6988
int g_BattleFxBzDrawRequest13[4] = {0, 0x88, 0, 0x20};
// GLOBAL: FF7 0x007f6998
int g_BattleSummonBahamutZeroBeamTable[12] = {
    0x0, 0xa8, 0x0, 0x20, 0x0, 0xa8,
    0x0, 0x20, (int)((char *)&g_BattleVahamut0DataPool + 0x0), 0x2e808080, 0x0, 0x0
};
// GLOBAL: FF7 0x007f69c8
BattleFxBzFlareQuadSlot g_BattleSummonBahamutZeroSlot10And16Draw = {0, {0xa8, 0, 0, 0, 0, 0, 0, 0, 0x20, 0, 0, 0}, 0};
unsigned char g_rawData_007f69dc[4] = {0xa8, 0x00, 0x00, 0x00};
// clang-format on

unsigned char g_rawData_007f69e0[8] = {0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007f69e8
short g_BattleSummonBahamutZeroSlot1016Angles[4] = {28672, 0, 0, 0};
// clang-format off
// GLOBAL: FF7 0x007f69f0
BattleFxBzFlarePointSlot g_BattleSummonBahamutZeroSlot10And16Points = {{0, 0x40, 0, 0, 0, 0, 0, 0, 0, 0x70}, 0, {0}};
// GLOBAL: FF7 0x007f6a08
int g_BattleSummonBahamutZeroSlot10And16Rotation[2] = {512, 0};
unsigned char g_rawData_007f6a10[24] = {0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00a85460
int g_BattleSummonBahamutZeroSlot10RingBlock = 0;
unsigned char g_bssPad_00a85464[4] = {0};
// GLOBAL: FF7 0x00a85468
struct BattleSummonBahamutZeroLightRig g_BattleSummonBahamutZeroLightRig = {0};
// GLOBAL: FF7 0x00a85488
unsigned char g_BattleFxBzDrawSlot0FInit = 0;
unsigned char g_bssPad_00a8548c[4] = {0};
// GLOBAL: FF7 0x00a85490
void (__cdecl *g_BattleSummonBahamutZeroSubTasks[500])(int) = {0};
// GLOBAL: FF7 0x00a85c60
BattleFxBzModelBlock *g_BattleSummonBahamutZeroSlot0FState = 0;
// GLOBAL: FF7 0x00a85c64
int g_BattleSummonBahamutZeroSlot10And16Block = 0;
// GLOBAL: FF7 0x00a85c68
struct BattleFxBahamutZeroDrawState *g_BattleSummonBahamutZeroSlot12State = 0;
// GLOBAL: FF7 0x00a85c6c
unsigned char g_BattleFxBzDrawSlot0CTo10Init = 0;
// GLOBAL: FF7 0x00a85c70
int g_BattleSummonBahamutZeroSpawnPhase = 0;
unsigned char g_bssPad_00a85c74[4] = {0};
// GLOBAL: FF7 0x00a85c78
int g_BattleSummonBahamutZeroSlot0BMatrix = 0;
unsigned char g_bssPad_00a85c7c[4] = {0};
// GLOBAL: FF7 0x00a85c80
short g_BattleSummonBahamutZeroSlot0BMatrixM11 = 0;
unsigned char g_bssPad_00a85c84[4] = {0};
// GLOBAL: FF7 0x00a85c88
struct BattleSummonBahamutZeroSlot0BState g_BattleSummonBahamutZeroSlot0BState = {0};
// GLOBAL: FF7 0x00a85c98
struct BattleFxBahamutZeroTrail *g_BattleSummonBahamutZeroTrail = 0;
unsigned char g_bssPad_00a85c9c[4] = {0};
// GLOBAL: FF7 0x00a85ca0
int g_BattleSummonBahamutZeroAimMatrix = 0;
unsigned char g_bssPad_00a85ca4[4] = {0};
// GLOBAL: FF7 0x00a85ca8
short g_BattleSummonBahamutZeroAimMatrixM11 = 0;
unsigned char g_bssPad_00a85cac[4] = {0};
// GLOBAL: FF7 0x00a85cb0
struct BattleSummonBahamutZeroAimMatrixTail g_BattleSummonBahamutZeroAimMatrixTail = {0};
unsigned char g_bssPad_00a85cc0[20] = {0};
// GLOBAL: FF7 0x00a85cd4
int g_BattleSummonBahamutZeroInnerMorphVerts = 0;
// GLOBAL: FF7 0x00a85cd8
unsigned char g_BattleSummonBahamutZeroSubTaskRecords[15998] = {0};
unsigned char g_bssPad_00a89b58[117120] = {0};
// GLOBAL: FF7 0x00aa64d8
BattleFxBzModelBlock *g_BattleSummonBahamutZeroSlot13State = 0;
// GLOBAL: FF7 0x00aa64dc
unsigned char g_BattleFxBzEmitGroundSparkPrimInit = 0;
// GLOBAL: FF7 0x00aa64e0
BattleSummonBahamutZeroEffectScaleState g_BattleSummonBahamutZeroScaleState = {0};
// GLOBAL: FF7 0x00aa64e4
short g_BattleSummonBahamutZeroLightRigAngleZ = 0;
// GLOBAL: FF7 0x00aa64e8
unsigned char g_BattleFxBzRunLightRigInit = 0;
// GLOBAL: FF7 0x00aa64ec
unsigned char g_BattleFxBzDrawSlot11Init = 0;
// GLOBAL: FF7 0x00aa64f0
unsigned char g_BattleFxBzDrawSlot10RingInit = 0;
// GLOBAL: FF7 0x00aa64f4
unsigned char g_BattleFxBzDrawSlot04And05Init = 0;
// GLOBAL: FF7 0x00aa64f8
unsigned char g_BattleFxBzDrawSlot0AInit = 0;
// GLOBAL: FF7 0x00aa64fc
short *g_BattleSummonBahamutZeroMorphedOuter = 0;
// GLOBAL: FF7 0x00aa6500
int g_BattleSummonBahamutZeroSpawnCount = 0;
// GLOBAL: FF7 0x00aa6504
BattleFxBahamutZeroLivewire *g_BattleSummonBahamutZeroSparkState = 0;
// GLOBAL: FF7 0x00aa6508
BattleFxBzModelBlock *g_BattleSummonBahamutZeroSlot0405State = 0;
unsigned char g_bssPad_00aa650c[4] = {0};
// GLOBAL: FF7 0x00aa6510
struct BattleSummonBahamutZeroCameraState g_BattleSummonBahamutZeroCameraState = {0};
unsigned char g_bssPad_00aa6514[4] = {0};
unsigned char g_bssPad_00aa6518[4] = {0};
// GLOBAL: FF7 0x00aa651c
struct BattleSummonBahamutZeroAttachState g_BattleSummonBahamutZeroAttachState = {0};
// GLOBAL: FF7 0x00aa6520
int g_BattleSummonBahamutZeroCameraOffset = 0;
// GLOBAL: FF7 0x00aa6524
int g_BattleSummonBahamutZeroCameraOffsetZ = 0;
unsigned char g_bssPad_00aa6528[20] = {0};
// GLOBAL: FF7 0x00aa653c
short *g_BattleSummonBahamutZeroMorphedInner = 0;
// GLOBAL: FF7 0x00aa6540
int g_BattleSummonBahamutZeroRingDebrisBlock = 0;
// GLOBAL: FF7 0x00aa6544
BattleFxBzSpinBlock *g_BattleSummonBahamutZeroTaskRecord = 0;
// GLOBAL: FF7 0x00aa6548
unsigned char g_BattleFxDrawMorphedLmdOuterInit = 0;
unsigned char g_bssPad_00aa654c[4] = {0};
// GLOBAL: FF7 0x00aa6550
int g_BattleSummonBahamutZeroSlot0CTo10ScaleMatrix = 0;
unsigned char g_bssPad_00aa6554[4] = {0};
// GLOBAL: FF7 0x00aa6558
short g_BattleSummonBahamutZeroSlot0CTo10ScaleM11 = 0;
unsigned char g_bssPad_00aa655c[4] = {0};
// GLOBAL: FF7 0x00aa6560
BattleFxBzBeamScaleTail g_BattleSummonBahamutZeroSlot0CTo10ScaleTail = {0};
// GLOBAL: FF7 0x00aa6570
int g_BattleSummonBahamutZeroTargetMask = 0;
// GLOBAL: FF7 0x00aa6574
unsigned char g_BattleFxBzDrawSlot14Init = 0;
// GLOBAL: FF7 0x00aa6578
struct BattleFxBzGroundLivewireScratch *g_BattleSummonBahamutZeroGroundSpark = 0;
// GLOBAL: FF7 0x00aa657c
BattleFxBzModelBlock *g_BattleSummonBahamutZeroSlot0EState = 0;
// GLOBAL: FF7 0x00aa6580
PsxMatrix *g_BattleSummonBahamutZeroGlowState = 0;
// GLOBAL: FF7 0x00aa6584
unsigned char g_BattleFxBzDrawSlot08Init = 0;
// GLOBAL: FF7 0x00aa6588
struct BattleLimitHighwindLoadedResTable g_BattleSummonBahamutZeroLightRigPos = {0};
// GLOBAL: FF7 0x00aa658c
short g_BattleSummonBahamutZeroSpinAngle = 0;
// GLOBAL: FF7 0x00aa6590
struct BattleSummonBahamutZeroMatrixPair g_BattleSummonBahamutZeroMatrixPair = {0};
// GLOBAL: FF7 0x00aa65d8
char g_BattleSummonBahamutZeroSavedDrawFlags[10] = {0};
unsigned char g_bssPad_00aa65e4[4] = {0};
// GLOBAL: FF7 0x00aa65e8
int g_BattleSummonBahamutZeroSlot15ScaleMatrix = 0;
unsigned char g_bssPad_00aa65ec[4] = {0};
// GLOBAL: FF7 0x00aa65f0
short g_BattleSummonBahamutZeroSlot15ScaleM11 = 0;
unsigned char g_bssPad_00aa65f4[4] = {0};
// GLOBAL: FF7 0x00aa65f8
struct BattleSummonBahamutZeroSlot15ScaleTail g_BattleSummonBahamutZeroSlot15ScaleTail = {0};
// GLOBAL: FF7 0x00aa6608
unsigned char g_BattleFxBzDrawSlot10And16Init = 0;
// GLOBAL: FF7 0x00aa660c
BattleFxCameraOrbitState *g_pBattleBahamutZeroCameraTrack = 0;
// GLOBAL: FF7 0x00aa6610
struct BattleBahamutZeroMatrixAAngles g_BattleBahamutZeroMatrixAAngles = {0};
// GLOBAL: FF7 0x00aa6614
short g_BattleBahamutZeroMatrixAAngleZ = 0;
// GLOBAL: FF7 0x00aa6618
struct BattleBahamutZeroMatrixBAngles g_BattleBahamutZeroMatrixBAngles = {0};
// GLOBAL: FF7 0x00aa661c
short g_BattleBahamutZeroMatrixBAngleZ = 0;
// GLOBAL: FF7 0x00aa6620
BattleFxBzRigBlock *g_pBattleBahamutZeroLightRigState = 0;
// GLOBAL: FF7 0x00aa6624
unsigned char g_BattleFxBzDrawSlot15Init = 0;
// GLOBAL: FF7 0x00aa6628
BattleFxCameraOrbitState *g_pBattleBahamutZeroCameraOther = 0;
// GLOBAL: FF7 0x00aa662c
BattleFxBzModelBlock *g_pBattleBahamutZeroSlot08State = 0;
// GLOBAL: FF7 0x00aa6630
PsxMatrix *g_pBattleBahamutZeroSlot0AMatrix = 0;
// GLOBAL: FF7 0x00aa6634
unsigned char g_BattleFxBzDrawSkyDomeTaskInit = 0;
// GLOBAL: FF7 0x00aa6638
unsigned char g_BattleFxDrawMorphedLmdInnerInit = 0;
// GLOBAL: FF7 0x00aa663c
unsigned char g_BattleFxBzMasterTimelineInit = 0;
// GLOBAL: FF7 0x00aa6640
unsigned char g_BattleFxBzDrawSpriteTrailInit = 0;
// GLOBAL: FF7 0x00aa6644
BattleFxBzModelBlock *g_pBattleBahamutZeroSlot14State = 0;
// GLOBAL: FF7 0x00aa6648
int g_BattleSummonBahamutZeroOuterMorphVerts = 0;
// GLOBAL: FF7 0x00aa664c
unsigned char g_BattleFxBzDrawSlot12Init = 0;
// GLOBAL: FF7 0x00aa6650
PsxMatrix *g_pBattleBahamutZeroSlot15Matrix = 0;
unsigned char g_bssPad_00aa6654[4] = {0};
unsigned char g_bssPad_00aa6658[12] = {0};
// GLOBAL: FF7 0x00aa6664
int g_BattleSummonBahamutZeroTrailDistSq = 0;
// GLOBAL: FF7 0x00aa6668
int g_BattleSummonBahamutZeroTimelineBlock = 0;
// GLOBAL: FF7 0x00aa666c
struct BattleFxBzSubTask *g_pBattleBahamutZeroSubTasks = 0;
// GLOBAL: FF7 0x00aa6670
struct BattleFxBahamutZeroState *g_pBattleBahamutZeroSkyDomeState = 0;
unsigned char g_bssPad_00aa6674[4] = {0};
// GLOBAL: FF7 0x00aa6678
struct BattleSummonBahamutZeroCameraState g_BattleBahamutZeroCameraEyeState = {0};
unsigned char g_bssPad_00aa667c[4] = {0};
unsigned char g_bssPad_00aa6680[4] = {0};
// GLOBAL: FF7 0x00aa6684
struct BattleBahamutZeroBoneTarget g_BattleBahamutZeroBoneTarget = {0};
// GLOBAL: FF7 0x00aa6688
int g_BattleSummonBahamutZeroEyeOffset = 0;
// GLOBAL: FF7 0x00aa668c
int g_BattleSummonBahamutZeroEyeOffsetZ = 0;
unsigned char g_bssPad_00aa6690[20] = {0};
// GLOBAL: FF7 0x00aa66a4
unsigned char g_BattleFxBzSpawnRingDebrisInit = 0;
// GLOBAL: FF7 0x00aa66a8
unsigned char g_BattleFxBzDrawSlot0EInit = 0;
// GLOBAL: FF7 0x00aa66ac
int g_BattleBahamutZeroSparkAngle = 0;
// GLOBAL: FF7 0x00aa66b0
unsigned char g_BattleFxBzDrawSlot13Init = 0;
// GLOBAL: FF7 0x00aa66b4
BattleBahamutZeroState *g_pBattleBahamutZeroSlot0CTo10State = 0;
// GLOBAL: FF7 0x00aa66b8
PsxMatrix *g_pBattleBahamutZeroSlot11Matrix = 0;
unsigned char g_bssPad_00aa66bc[4] = {0};
// GLOBAL: FF7 0x00aa66c0
struct BattleBahamutZeroComposedMatrix g_BattleBahamutZeroComposedMatrix = {0};
// GLOBAL: FF7 0x00aa66e0
unsigned char g_BattleFxBzDrawEnergyTubeTaskInit = 0;
// GLOBAL: FF7 0x00aa66e4
unsigned char g_BattleFxBzDrawGlowSpriteInit = 0;
// GLOBAL: FF7 0x00aa66e8
unsigned char g_BattleFxBzDrawSparkSwarmInit = 0;
// GLOBAL: FF7 0x00aa66ec
void *g_pBattleBahamutZeroSkyALmd = 0;
// GLOBAL: FF7 0x00aa66f0
void *g_pBattleBahamutZeroSkyDomeLmd = 0;
// GLOBAL: FF7 0x00aa66f4
void *g_pBattleBahamutZeroSkyBPly = 0;
// GLOBAL: FF7 0x00aa66f8
void *g_pBattleBahamutZeroSkyDPly = 0;
// GLOBAL: FF7 0x00aa66fc
struct GfxTableCursor *g_pBattleBahamutZeroTableCursor = 0;
// GLOBAL: FF7 0x00aa6700
int g_BattleSummonBahamutZeroSlot06AimOrigin = 0;
unsigned char g_bssPad_00aa6704[4] = {0};
// GLOBAL: FF7 0x00aa6708
int g_BattleSummonBahamutZeroSlot07AimOrigin = 0;
unsigned char g_bssPad_00aa670c[4] = {0};
// GLOBAL: FF7 0x00aa6710
int g_BattleSummonBahamutZeroGlowAimOrigin = 0;
unsigned char g_bssPad_00aa6714[4] = {0};
// GLOBAL: FF7 0x00aa6718
int g_BattleSummonBahamutZeroSlot0CTo10ZeroAngles = 0;
unsigned char g_bssPad_00aa671c[4] = {0};
// GLOBAL: FF7 0x00aa6720
BattleBahamutZeroTube *g_pBattleBahamutZeroEnergyTube = 0;
unsigned char g_bssPad_00aa6724[4] = {0};
// GLOBAL: FF7 0x00aa6728
unsigned char g_BattleSummonBahamutZeroSlot10SpriteAimOrigin[4] = {0};
// clang-format on


/*
 * When the battle scene exists and
 * g_pBattleBahamutZeroTableCursor is zero, initializes a default-duration resource record, writes
 * type zero, builds SUMMON/VAHAMUT3 into the local path buffer and calls Gfx_LoadModelTableIfAbsent
 * with resource ID 0x22, g_BattleSummonBahamutZeroResourceTable metadata, the global output slot
 * and scene context at 0x4C. Then registers g_BahamutZeroSoundGroupPointers sound groups inside the
 * same guard regardless of creation result. Otherwise does nothing.
 */
// FUNCTION: FF7 0x00483520
void __cdecl BattleFx_SummonLoadResBahamutZero(void)
{
    GfxModelTableBank *sceneContext;
    GfxResourceRequest record;
    int id;
    char pathOut[200];
    GfxResourceRequest *request;
    int infoPtr;

    if (((int)g_pBattle3dScene) != 0 && g_pBattleBahamutZeroTableCursor == 0) {
        id = 0x22;
        infoPtr = (int)&g_BattleSummonBahamutZeroResourceTable;
        sceneContext = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &record;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        request->resourcePath = BattleFx_BuildResourcePath(BattleFx_BahamutZeroResourceDir(), pathOut);
        Gfx_LoadModelTableIfAbsent(id, infoPtr, request, &g_pBattleBahamutZeroTableCursor, sceneContext);
        SfxRegisterSoundGroups((struct SoundGroup **)&g_BahamutZeroSoundGroupPointers);
    }
}

/*
 * Clears retained Bahamut Zero counters/control values
 * and three rotation shorts, then zeroes eight thirty-byte blocks at
 * g_BattleSummonBahamutZeroLightRig, g_BattleBahamutZeroComposedMatrix,
 * g_BattleSummonBahamutZeroMatrixPair.matrixB, g_BattleSummonBahamutZeroMatrixPair.matrixA,
 * g_BattleSummonBahamutZeroAimMatrix, g_BattleSummonBahamutZeroSlot15ScaleMatrix,
 * g_BattleSummonBahamutZeroSlot0CTo10ScaleMatrix and g_BattleSummonBahamutZeroSlot0BMatrix. Sets matching
 * retained translation/diagonal fields for two blocks to -40, -552 and 4096; writes vector
 * (-2240,-5000,360), two triples of scale shorts 0x400 and three matrix diagonal shorts 0x200.
 * Calls BattleFx_SummonLoadResBahamutZero then BattleFx_BzBeginEffect with both arguments and
 * returns zero.
 */
// FUNCTION: FF7 0x004835c1
int __cdecl BattleFx_SummonEnterBahamutZero(int targetMask, int actorIdx)
{
    g_BattleSummonBahamutZeroSpawnPhase = 0;
    g_BattleSummonBahamutZeroSpawnCount = 0;
    g_BattleBahamutZeroSparkAngle = 0;
    g_BattleSummonBahamutZeroLightRigAngleZ = 0;
    g_BattleSummonBahamutZeroScaleState.effectScaleCurrent = g_BattleSummonBahamutZeroLightRigAngleZ;
    g_BattleSummonBahamutZeroScaleState.effectScale = g_BattleSummonBahamutZeroScaleState.effectScaleCurrent;
    memset(&g_BattleSummonBahamutZeroLightRig, 0, 30);
    memset(&g_BattleBahamutZeroComposedMatrix, 0, 30);
    memset(&g_BattleSummonBahamutZeroMatrixPair.matrixB, 0, 30);
    g_BattleSummonBahamutZeroMatrixPair.matrixB.t[0] = -40;
    g_BattleSummonBahamutZeroMatrixPair.matrixB.t[1] = -552;
    g_BattleSummonBahamutZeroMatrixPair.matrixB.t[2] = 0x1000;
    memset(&g_BattleSummonBahamutZeroMatrixPair.matrixA, 0, 30);
    g_BattleSummonBahamutZeroMatrixPair.matrixA.t[0] = -40;
    g_BattleSummonBahamutZeroMatrixPair.matrixA.t[1] = -552;
    g_BattleSummonBahamutZeroMatrixPair.matrixA.t[2] = 0x1000;
    memset(&g_BattleSummonBahamutZeroAimMatrix, 0, 30);
    (g_BattleSummonBahamutZeroLightRigPos.posX) = -2240;
    g_BattleSummonBahamutZeroLightRigPos.posY = -5000;
    g_BattleSummonBahamutZeroSpinAngle = 360;
    memset(&g_BattleSummonBahamutZeroSlot15ScaleMatrix, 0, 30);
    g_BattleBahamutZeroMatrixBAngleZ = 0x400 /* 90 deg */;
    g_BattleBahamutZeroMatrixBAngles.bahamutZeroMatrixBStateWord = g_BattleBahamutZeroMatrixBAngleZ;
    (g_BattleBahamutZeroMatrixBAngles.matrixBState) = g_BattleBahamutZeroMatrixBAngles.bahamutZeroMatrixBStateWord;
    g_BattleBahamutZeroMatrixAAngleZ = 0x400 /* 90 deg */;
    g_BattleBahamutZeroMatrixAAngles.bahamutZeroMatrixAStateWord = g_BattleBahamutZeroMatrixAAngleZ;
    (g_BattleBahamutZeroMatrixAAngles.matrixAState) = g_BattleBahamutZeroMatrixAAngles.bahamutZeroMatrixAStateWord;
    memset(&g_BattleSummonBahamutZeroSlot0CTo10ScaleMatrix, 0, 30);
    (g_BattleSummonBahamutZeroSlot0CTo10ScaleTail.initialDepth) = 0x200;
    g_BattleSummonBahamutZeroSlot0CTo10ScaleM11 = (g_BattleSummonBahamutZeroSlot0CTo10ScaleTail.initialDepth);
    *(short *)&g_BattleSummonBahamutZeroSlot0CTo10ScaleMatrix = g_BattleSummonBahamutZeroSlot0CTo10ScaleM11;
    memset(&g_BattleSummonBahamutZeroSlot0BMatrix, 0, 30);
    BattleFx_SummonLoadResBahamutZero();
    BattleFx_BzBeginEffect(targetMask, actorIdx);
    return 0;
}

/*
 * Stores the actor mask and obtains its XZ bounding-box midpoint, sets rotation 0x800 only when
 * its Z is below actorIdx's Z, builds the shared transform and saves Z. Allocates the Bz master
 * timeline and light rig, consumes rand although both branches select the same camera script, then
 * sets mask eight in g_BattleStageHideFlags. The midpoint's Z is written to centerPos.vz.
 */
// FUNCTION: FF7 0x00483762
void __cdecl BattleFx_BzBeginEffect(int targetMask, int actorIdx)
{
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    SVECTOR centerPos;

    Battle_GetActorMaskCentroid(g_BattleSummonBahamutZeroTargetMask = targetMask, &centerPos.vx);
    if (centerPos.vz < g_BattleActorRecords[actorIdx].posZ) {
        g_BattleSummonBahamutZeroScaleState.effectScaleCurrent = 0x800 /* 0.5 */;
    }
    RotMatrix((SVECTOR *)&g_BattleSummonBahamutZeroScaleState, (PsxMatrix *)&g_BattleSummonBahamutZeroLightRig);
    g_BattleSummonBahamutZeroLightRig.bahamutZeroLightRigOffsetZ = centerPos.vz;
    Battle_AllocateResourceSlot(BattleFx_BzMasterTimeline);
    Battle_AllocateResourceSlot(BattleFx_BzRunLightRig);
    if ((rand() & 0x100) != 0) {
        BattleFx_BzSpawnCameraScript(&g_BahamutZeroCameraTimelineScript, &g_BattleSummonBahamutZeroLightRig, actorIdx);
    } else {
        BattleFx_BzSpawnCameraScript(&g_BahamutZeroCameraTimelineScript, &g_BattleSummonBahamutZeroLightRig, actorIdx);
    }
    g_BattleStageHideFlags |= 8;
}

// Queues a camera script for BattleFx_BzRunCameraScript in a new 0x20-byte resource slot. It stores
// eyeTrack and focusTrack as dwords and duration as a word.
// FUNCTION: FF7 0x00483826
void __cdecl BattleFx_BzSpawnCameraScript(void *eyeTrack, void *focusTrack, int duration)
{
    BattleFxBzCameraSlot *slot;

    slot = (BattleFxBzCameraSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BzRunCameraScript);
    slot->eyeTrack = (int)eyeTrack;
    slot->focusTrack = (int)focusTrack;
    slot->duration = duration;
}

/*
 * Interprets the Bahamut Zero camera script using two
 * retained tracks. First unpaused call clears durations, marks both actor selectors minus one and
 * sets task count one. While either duration is zero consumes short commands with bit 0x40
 * selecting eye: duration, absolute XYZ, linear velocity, distance-following, acceleration, eased
 * interpolation, attachment, selector-only detachment, suppression duration and global control
 * write. Attachment uses task actor when script actor is zero and subtracts the local attachment
 * point; detachment only sets selector minus one without adding it back. Unknown commands retire
 * and return. Distance setup predicts a linear other track using the current track duration.
 * Updates eye before focus, with acceleration falling into linear motion, normalized distance
 * following and BattleFx_BzEaseFixed interpolation, decrementing both durations. Publication
 * remains active while paused. With a task matrix, unattached eye transforms position; attached
 * eye rotates the offset without translation and adds actor bone/world position, then reinstalls
 * the task matrix. Attached focus instead computes a local attachment, adds its offset and
 * transforms; unattached focus relies on the installed matrix, including prior state if eye is
 * suppressed. Without a task matrix copies nonsuppressed positions directly.
 */
// FUNCTION: FF7 0x00483866
void __cdecl BattleFx_BzRunCameraScript(void)
{
    BattleFxBahamutZeroCameraScript *scriptPtr;
    int extraVec;
    int padField[46]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int padHole[6]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int unusedPad[14]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    Vec3sPacked *offsetsPtr;
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short cmd;
    BattleCameraScriptVec *vecAddr;
    Vec3sPacked *pBlock;
    int fraction;
    int i;
    int rotated;

    scriptPtr = (BattleFxBahamutZeroCameraScript *)BATTLE_CURRENT_TASK_SLOT;
    pBlock = (Vec3sPacked *)Gfx_GetPsxScratchpadPtr(0);
    offsetsPtr = (Vec3sPacked *)((int)pBlock + 8);
    vecAddr = (BattleCameraScriptVec *)((int)offsetsPtr + 8);
    extraVec = (int)vecAddr + 0x10;
    if (g_BattleIsPaused == 0) {
        if (scriptPtr->started == 0) {
            g_BattleBahamutZeroCameraEyeState.waitFlag = 0;
            g_BattleSummonBahamutZeroCameraState.waitFlag = 0;
            (g_BattleBahamutZeroBoneTarget.boneIndex) = 0xFFFF;
            (g_BattleSummonBahamutZeroAttachState.actorSelector) = 0xFFFF;
            scriptPtr->started = 1;
        }
        while (g_BattleBahamutZeroCameraEyeState.waitFlag == 0 || g_BattleSummonBahamutZeroCameraState.waitFlag == 0) {
            cmd = *scriptPtr->scriptPos;
            scriptPtr->scriptPos++;
            if ((cmd & 0x40) != 0) {
                g_pBattleBahamutZeroCameraTrack = (struct BattleFxCameraOrbitState *)&g_BattleBahamutZeroCameraEyeState;
                g_pBattleBahamutZeroCameraOther = (struct BattleFxCameraOrbitState *)&g_BattleSummonBahamutZeroCameraState;
            } else {
                g_pBattleBahamutZeroCameraTrack = (struct BattleFxCameraOrbitState *)&g_BattleSummonBahamutZeroCameraState;
                g_pBattleBahamutZeroCameraOther = (struct BattleFxCameraOrbitState *)&g_BattleBahamutZeroCameraEyeState;
            }
            g_pBattleBahamutZeroCameraTrack->mode = cmd & ~0xC0;
            switch (g_pBattleBahamutZeroCameraTrack->mode) {
            case SUMMON_CAMERA_WAIT:
                g_pBattleBahamutZeroCameraTrack->stepCount = BattleFx_BzFetchScriptCount(&scriptPtr->scriptPos);
                break;
            case SUMMON_CAMERA_SET_POSITION:
                g_pBattleBahamutZeroCameraTrack->posX = *scriptPtr->scriptPos;
                scriptPtr->scriptPos++;
                g_pBattleBahamutZeroCameraTrack->posY = *scriptPtr->scriptPos;
                scriptPtr->scriptPos++;
                g_pBattleBahamutZeroCameraTrack->posZ = *scriptPtr->scriptPos;
                scriptPtr->scriptPos++;
                break;
            case SUMMON_CAMERA_MOVE_LINEAR:
                g_pBattleBahamutZeroCameraTrack->stepCount = BattleFx_BzFetchScriptCount(&scriptPtr->scriptPos);
                g_pBattleBahamutZeroCameraTrack->stepX = (*scriptPtr->scriptPos - g_pBattleBahamutZeroCameraTrack->posX) / g_pBattleBahamutZeroCameraTrack->stepCount;
                scriptPtr->scriptPos++;
                g_pBattleBahamutZeroCameraTrack->stepY = (*scriptPtr->scriptPos - g_pBattleBahamutZeroCameraTrack->posY) / g_pBattleBahamutZeroCameraTrack->stepCount;
                scriptPtr->scriptPos++;
                g_pBattleBahamutZeroCameraTrack->stepZ = (*scriptPtr->scriptPos - g_pBattleBahamutZeroCameraTrack->posZ) / g_pBattleBahamutZeroCameraTrack->stepCount;
                scriptPtr->scriptPos++;
                break;
            case SUMMON_CAMERA_MOVE_ORBIT:
                pBlock->vx = g_pBattleBahamutZeroCameraOther->posX - g_pBattleBahamutZeroCameraTrack->posX;
                pBlock->vy = g_pBattleBahamutZeroCameraOther->posY - g_pBattleBahamutZeroCameraTrack->posY;
                pBlock->vz = g_pBattleBahamutZeroCameraOther->posZ - g_pBattleBahamutZeroCameraTrack->posZ;
                g_pBattleBahamutZeroCameraTrack->progress = SqrtToInt(pBlock->vx * pBlock->vx + pBlock->vy * pBlock->vy + pBlock->vz * pBlock->vz);
                g_pBattleBahamutZeroCameraTrack->delta = g_pBattleBahamutZeroCameraTrack->pos;
                g_pBattleBahamutZeroCameraTrack->stepCount = BattleFx_BzFetchScriptCount(&scriptPtr->scriptPos);
                pBlock->vx = *scriptPtr->scriptPos;
                scriptPtr->scriptPos++;
                pBlock->vy = *scriptPtr->scriptPos;
                scriptPtr->scriptPos++;
                pBlock->vz = *scriptPtr->scriptPos;
                scriptPtr->scriptPos++;
                g_pBattleBahamutZeroCameraTrack->stepX = (pBlock->vx - g_pBattleBahamutZeroCameraTrack->posX) / g_pBattleBahamutZeroCameraTrack->stepCount;
                g_pBattleBahamutZeroCameraTrack->stepY = (pBlock->vy - g_pBattleBahamutZeroCameraTrack->posY) / g_pBattleBahamutZeroCameraTrack->stepCount;
                g_pBattleBahamutZeroCameraTrack->stepZ = (pBlock->vz - g_pBattleBahamutZeroCameraTrack->posZ) / g_pBattleBahamutZeroCameraTrack->stepCount;
                switch (g_pBattleBahamutZeroCameraOther->mode) {
                case SUMMON_CAMERA_MOVE_LINEAR:
                    offsetsPtr->vx = g_pBattleBahamutZeroCameraOther->posX + g_pBattleBahamutZeroCameraOther->stepX * g_pBattleBahamutZeroCameraTrack->stepCount;
                    offsetsPtr->vy = g_pBattleBahamutZeroCameraOther->posY + g_pBattleBahamutZeroCameraOther->stepY * g_pBattleBahamutZeroCameraTrack->stepCount;
                    offsetsPtr->vz = g_pBattleBahamutZeroCameraOther->posZ + g_pBattleBahamutZeroCameraOther->stepZ * g_pBattleBahamutZeroCameraTrack->stepCount;
                    break;
                default:
                    offsetsPtr->vx = g_pBattleBahamutZeroCameraOther->posX;
                    offsetsPtr->vy = g_pBattleBahamutZeroCameraOther->posY;
                    offsetsPtr->vz = g_pBattleBahamutZeroCameraOther->posZ;
                }
                pBlock->vx -= offsetsPtr->vx;
                pBlock->vy -= offsetsPtr->vy;
                pBlock->vz -= offsetsPtr->vz;
                g_pBattleBahamutZeroCameraTrack->stepSize = (SqrtToInt(pBlock->vx * pBlock->vx + pBlock->vy * pBlock->vy + pBlock->vz * pBlock->vz) - g_pBattleBahamutZeroCameraTrack->progress) / g_pBattleBahamutZeroCameraTrack->stepCount;
                break;
            case SUMMON_CAMERA_MOVE_ACCEL:
                g_pBattleBahamutZeroCameraTrack->stepCount = BattleFx_BzFetchScriptCount(&scriptPtr->scriptPos);
                pBlock->vx = *scriptPtr->scriptPos - g_pBattleBahamutZeroCameraTrack->posX;
                scriptPtr->scriptPos++;
                pBlock->vy = *scriptPtr->scriptPos - g_pBattleBahamutZeroCameraTrack->posY;
                scriptPtr->scriptPos++;
                pBlock->vz = *scriptPtr->scriptPos - g_pBattleBahamutZeroCameraTrack->posZ;
                scriptPtr->scriptPos++;
                g_pBattleBahamutZeroCameraTrack->deltaX = (pBlock->vx - g_pBattleBahamutZeroCameraTrack->stepX * g_pBattleBahamutZeroCameraTrack->stepCount << 1) / (g_pBattleBahamutZeroCameraTrack->stepCount * g_pBattleBahamutZeroCameraTrack->stepCount);
                g_pBattleBahamutZeroCameraTrack->deltaY = (pBlock->vy - g_pBattleBahamutZeroCameraTrack->stepY * g_pBattleBahamutZeroCameraTrack->stepCount << 1) / (g_pBattleBahamutZeroCameraTrack->stepCount * g_pBattleBahamutZeroCameraTrack->stepCount);
                g_pBattleBahamutZeroCameraTrack->deltaZ = (pBlock->vz - g_pBattleBahamutZeroCameraTrack->stepZ * g_pBattleBahamutZeroCameraTrack->stepCount << 1) / (g_pBattleBahamutZeroCameraTrack->stepCount * g_pBattleBahamutZeroCameraTrack->stepCount);
                break;
            case SUMMON_CAMERA_MOVE_EASED:
                g_pBattleBahamutZeroCameraTrack->stepCount = BattleFx_BzFetchScriptCount(&scriptPtr->scriptPos);
                g_pBattleBahamutZeroCameraTrack[1].mode = *scriptPtr->scriptPos;
                scriptPtr->scriptPos++;
                g_pBattleBahamutZeroCameraTrack->progress = 0;
                g_pBattleBahamutZeroCameraTrack->stepSize = 4096 / g_pBattleBahamutZeroCameraTrack->stepCount;
                g_pBattleBahamutZeroCameraTrack->deltaX = g_pBattleBahamutZeroCameraTrack->posX;
                g_pBattleBahamutZeroCameraTrack->deltaY = g_pBattleBahamutZeroCameraTrack->posY;
                g_pBattleBahamutZeroCameraTrack->deltaZ = g_pBattleBahamutZeroCameraTrack->posZ;
                g_pBattleBahamutZeroCameraTrack->stepX = *scriptPtr->scriptPos - g_pBattleBahamutZeroCameraTrack->posX;
                scriptPtr->scriptPos++;
                g_pBattleBahamutZeroCameraTrack->stepY = *scriptPtr->scriptPos - g_pBattleBahamutZeroCameraTrack->posY;
                scriptPtr->scriptPos++;
                g_pBattleBahamutZeroCameraTrack->stepZ = *scriptPtr->scriptPos - g_pBattleBahamutZeroCameraTrack->posZ;
                scriptPtr->scriptPos++;
                break;
            case SUMMON_CAMERA_ATTACH_BONE:
                if ((g_pBattleBahamutZeroCameraTrack->attachPoint = *scriptPtr->scriptPos++) == 0) {
                    g_pBattleBahamutZeroCameraTrack->attachPoint = scriptPtr->attachPoint;
                }
                g_pBattleBahamutZeroCameraTrack->attachIndex = *scriptPtr->scriptPos;
                scriptPtr->scriptPos++;
                BattleFx_BzAttachPointToLocal(g_pBattleBahamutZeroCameraTrack->attachPoint, g_pBattleBahamutZeroCameraTrack->attachIndex, scriptPtr->rotMatrix, pBlock);
                g_pBattleBahamutZeroCameraTrack->posX -= pBlock->vx;
                g_pBattleBahamutZeroCameraTrack->posY -= pBlock->vy;
                g_pBattleBahamutZeroCameraTrack->posZ -= pBlock->vz;
                break;
            case SUMMON_CAMERA_DETACH:
                g_pBattleBahamutZeroCameraTrack->attachPoint = 0xFFFF;
                break;
            case SUMMON_CAMERA_WAIT_NO_WRITEBACK:
                g_pBattleBahamutZeroCameraTrack->stepCount = BattleFx_BzFetchScriptCount(&scriptPtr->scriptPos);
                break;
            case SUMMON_CAMERA_SET_SCRIPT_VALUE:
                g_BattleCameraScriptValue = *scriptPtr->scriptPos;
                scriptPtr->scriptPos++;
                break;
            default:
                scriptPtr->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
        g_pBattleBahamutZeroCameraTrack = (struct BattleFxCameraOrbitState *)&g_BattleBahamutZeroCameraEyeState;
        g_pBattleBahamutZeroCameraOther = (struct BattleFxCameraOrbitState *)&g_BattleSummonBahamutZeroCameraState;
        for (i = 0; i < 2; i++) {
            switch (g_pBattleBahamutZeroCameraTrack->mode) {
            case SUMMON_CAMERA_WAIT:
                break;
            case SUMMON_CAMERA_MOVE_ACCEL:
                g_pBattleBahamutZeroCameraTrack->stepX += g_pBattleBahamutZeroCameraTrack->deltaX;
                g_pBattleBahamutZeroCameraTrack->stepY += g_pBattleBahamutZeroCameraTrack->deltaY;
                g_pBattleBahamutZeroCameraTrack->stepZ += g_pBattleBahamutZeroCameraTrack->deltaZ;
            case SUMMON_CAMERA_MOVE_LINEAR:
                g_pBattleBahamutZeroCameraTrack->posX += g_pBattleBahamutZeroCameraTrack->stepX;
                g_pBattleBahamutZeroCameraTrack->posY += g_pBattleBahamutZeroCameraTrack->stepY;
                g_pBattleBahamutZeroCameraTrack->posZ += g_pBattleBahamutZeroCameraTrack->stepZ;
                break;
            case SUMMON_CAMERA_MOVE_ORBIT:
                g_pBattleBahamutZeroCameraTrack->deltaX += g_pBattleBahamutZeroCameraTrack->stepX;
                g_pBattleBahamutZeroCameraTrack->deltaY += g_pBattleBahamutZeroCameraTrack->stepY;
                g_pBattleBahamutZeroCameraTrack->deltaZ += g_pBattleBahamutZeroCameraTrack->stepZ;
                g_pBattleBahamutZeroCameraTrack->progress += g_pBattleBahamutZeroCameraTrack->stepSize;
                ((BattleFxCameraVec *)vecAddr)->x = g_pBattleBahamutZeroCameraOther->posX - g_pBattleBahamutZeroCameraTrack->deltaX;
                ((BattleFxCameraVec *)vecAddr)->y = g_pBattleBahamutZeroCameraOther->posY - g_pBattleBahamutZeroCameraTrack->deltaY;
                ((BattleFxCameraVec *)vecAddr)->z = g_pBattleBahamutZeroCameraOther->posZ - g_pBattleBahamutZeroCameraTrack->deltaZ;
                VectorNormalToSVector((int *)vecAddr, (short *)pBlock);
                g_pBattleBahamutZeroCameraTrack->posX = g_pBattleBahamutZeroCameraOther->posX - (pBlock->vx * g_pBattleBahamutZeroCameraTrack->progress >> 12);
                g_pBattleBahamutZeroCameraTrack->posY = g_pBattleBahamutZeroCameraOther->posY - (pBlock->vy * g_pBattleBahamutZeroCameraTrack->progress >> 12);
                g_pBattleBahamutZeroCameraTrack->posZ = g_pBattleBahamutZeroCameraOther->posZ - (pBlock->vz * g_pBattleBahamutZeroCameraTrack->progress >> 12);
                break;
            case SUMMON_CAMERA_MOVE_EASED:
                g_pBattleBahamutZeroCameraTrack->progress += g_pBattleBahamutZeroCameraTrack->stepSize;
                fraction = g_pBattleBahamutZeroCameraTrack->progress;
                fraction = BattleFx_BzEaseFixed(fraction, g_pBattleBahamutZeroCameraTrack[1].mode);
                g_pBattleBahamutZeroCameraTrack->posX = g_pBattleBahamutZeroCameraTrack->deltaX + (g_pBattleBahamutZeroCameraTrack->stepX * fraction >> 12);
                g_pBattleBahamutZeroCameraTrack->posY = g_pBattleBahamutZeroCameraTrack->deltaY + (g_pBattleBahamutZeroCameraTrack->stepY * fraction >> 12);
                g_pBattleBahamutZeroCameraTrack->posZ = g_pBattleBahamutZeroCameraTrack->deltaZ + (g_pBattleBahamutZeroCameraTrack->stepZ * fraction >> 12);
            }
            (g_pBattleBahamutZeroCameraTrack->stepCount)--;
            g_pBattleBahamutZeroCameraTrack = (struct BattleFxCameraOrbitState *)&g_BattleSummonBahamutZeroCameraState;
            g_pBattleBahamutZeroCameraOther = (struct BattleFxCameraOrbitState *)&g_BattleBahamutZeroCameraEyeState;
        }
    }
    if (scriptPtr->rotMatrix != 0) {
        if ((g_BattleBahamutZeroCameraEyeState.cameraState) != SUMMON_CAMERA_WAIT_NO_WRITEBACK) {
            SetRotMatrix(scriptPtr->rotMatrix);
            SetTransMatrix(scriptPtr->rotMatrix);
            if ((g_BattleBahamutZeroBoneTarget.boneIndex) == -1) {
                RotTransPoint(&g_BattleSummonBahamutZeroEyeOffset, (int *)vecAddr, &rotated);
                g_BattleCameraPosition.cameraPosX = vecAddr->x;
                g_BattleCameraPosition.cameraPosY = vecAddr->y;
                g_BattleCameraPosZ = vecAddr->z;
            } else {
                RotateSVectorByInstalledMatrix(&g_BattleSummonBahamutZeroEyeOffset, vecAddr);
                if (g_BattleBahamutZeroBoneTarget.boneIdx != -1) {
                    Battle_GetBoneWorldPos((g_BattleBahamutZeroBoneTarget.boneIndex), g_BattleBahamutZeroBoneTarget.boneIdx, (short *)pBlock);
                } else {
                    pBlock->vx = g_BattleActorRecords[(g_BattleBahamutZeroBoneTarget.boneIndex)].posX;
                    pBlock->vy = g_BattleActorRecords[(g_BattleBahamutZeroBoneTarget.boneIndex)].posY;
                    pBlock->vz = g_BattleActorRecords[(g_BattleBahamutZeroBoneTarget.boneIndex)].posZ;
                }
                g_BattleCameraPosition.cameraPosX = ((BattleFxCameraVec *)vecAddr)->x + pBlock->vx;
                g_BattleCameraPosition.cameraPosY = ((BattleFxCameraVec *)vecAddr)->y + pBlock->vy;
                g_BattleCameraPosZ = ((BattleFxCameraVec *)vecAddr)->z + pBlock->vz;
                SetRotMatrix(scriptPtr->rotMatrix);
                SetTransMatrix(scriptPtr->rotMatrix);
            }
        }
        if ((g_BattleSummonBahamutZeroCameraState.cameraState) != SUMMON_CAMERA_WAIT_NO_WRITEBACK) {
            if ((g_BattleSummonBahamutZeroAttachState.actorSelector) == -1) {
                RotTransPoint(&g_BattleSummonBahamutZeroCameraOffset, (int *)vecAddr, &rotated);
                g_BattleCameraFocalPoint.focalPointX = vecAddr->x;
                g_BattleCameraFocalPoint.focalPointY = vecAddr->y;
                g_BattleCameraFocusZ = vecAddr->z;
            } else {
                BattleFx_BzAttachPointToLocal((g_BattleSummonBahamutZeroAttachState.actorSelector), g_BattleSummonBahamutZeroAttachState.boneIndex, scriptPtr->rotMatrix, pBlock);
                pBlock->vx += ((BattleSummonBahamutZeroCameraOffset *)&g_BattleSummonBahamutZeroCameraOffset)->offsetStepX;
                pBlock->vy += (((BattleSummonBahamutZeroCameraOffset *)&g_BattleSummonBahamutZeroCameraOffset)->offsetStepY);
                pBlock->vz += *(short *)&g_BattleSummonBahamutZeroCameraOffsetZ;
                SetRotMatrix(scriptPtr->rotMatrix);
                SetTransMatrix(scriptPtr->rotMatrix);
                RotTransPoint((void *)pBlock, (int *)vecAddr, &rotated);
                g_BattleCameraFocalPoint.focalPointX = vecAddr->x;
                g_BattleCameraFocalPoint.focalPointY = vecAddr->y;
                g_BattleCameraFocusZ = vecAddr->z;
            }
        }
    } else {
        if ((g_BattleBahamutZeroCameraEyeState.cameraState) != SUMMON_CAMERA_WAIT_NO_WRITEBACK) {
            *(int *)&g_BattleCameraPosition = g_BattleSummonBahamutZeroEyeOffset;
            *(int *)&g_BattleCameraPosZ = g_BattleSummonBahamutZeroEyeOffsetZ;
        }
        if ((g_BattleSummonBahamutZeroCameraState.cameraState) != SUMMON_CAMERA_WAIT_NO_WRITEBACK) {
            *(int *)&g_BattleCameraFocalPoint = g_BattleSummonBahamutZeroCameraOffset;
            *(int *)&g_BattleCameraFocusZ = g_BattleSummonBahamutZeroCameraOffsetZ;
        }
    }
}

/*
 * Fetches one 16-bit word from the script whose cursor is
 * the pointer at `index`, advancing that cursor by 2. A non-negative word is returned as is; a negative
 * word -n is used as an index and the result becomes the short at
 * ((short *)&g_BattleSummonBahamutZeroAimMatrixTail.bahamutZeroScriptCountBase)[n], so negative
 * literals in the script are indirections into that table. Returns the resulting short.
 */
// FUNCTION: FF7 0x00484825
short __cdecl BattleFx_BzFetchScriptCount(short **index)
{
    int val;

    val = **index;
    (*index)++;
    if (val < 0) {
        val = (&g_BattleSummonBahamutZeroAimMatrixTail.bahamutZeroScriptCountBase)[-val];
    }
    return (short)val;
}

/*
 * Eases a 12-bit fixed-point `value` by `easeMode`: the high byte of |easeMode| is a repeat count, each
 * pass replacing `value` with SinFixed(value / 4) (for negative `easeMode` the value is mirrored around
 * 0x1000 before and after the passes); easeMode == 0 returns `value` unchanged. The low byte of `easeMode` plus
 * one then blends the eased value with the original in 256ths, and the blend is returned.
 */
// FUNCTION: FF7 0x0048485f
int __cdecl BattleFx_BzEaseFixed(int value, int easeMode)
{
    int passIdx;
    int v0;
    int passCount;

    v0 = value;
    if (easeMode > 0) {
        passCount = easeMode >> 8;
        for (passIdx = 0; passIdx < passCount; passIdx++) {
            value = SinFixed(value / 4);
        }
    } else if (easeMode < 0) {
        easeMode = -easeMode;
        passCount = easeMode >> 8;
        value = 0x1000 - value;
        for (passIdx = 0; passIdx < passCount; passIdx++) {
            value = SinFixed(value / 4);
        }
        value = 0x1000 - value;
    } else {
        return value;
    }
    passCount = (easeMode & 0xFF) + 1;
    value = (value * passCount + v0 * (0x100 - passCount)) >> 8;
    return value;
}

// Writes the position of one battle actor point into localOffset, expressed in the local frame of
// matrix. When boneIndex is not -1 the point is the bone's world position from
// Battle_GetBoneWorldPos; otherwise it is the actor's posX, posY and posZ. The matrix translation
// is subtracted from it, and the difference is transposed with TransposeMatrix and rotated through
// that transpose with TransformVectorToInt16, which overwrites localOffset. Seven unreferenced
// padding locals ahead of the transposed matrix preserve VC5 /Od stack layout.
// FUNCTION: FF7 0x0048494b
void __cdecl BattleFx_BzAttachPointToLocal(int actorIndex, int boneIndex, BattleFxActorMatrix *matrix, Vec3sPacked *localOffset)
{
    int transposed[8];

    if (boneIndex != -1) {
        Battle_GetBoneWorldPos(actorIndex, boneIndex, (short *)localOffset);
    } else {
        localOffset->vx = g_BattleActorRecords[actorIndex].posX;
        localOffset->vy = g_BattleActorRecords[actorIndex].posY;
        localOffset->vz = g_BattleActorRecords[actorIndex].posZ;
    }
    localOffset->vx -= matrix->translationX;
    localOffset->vy -= matrix->translationY;
    localOffset->vz -= matrix->translationZ;
    TransposeMatrix(matrix, transposed);
    TransformVectorToInt16(transposed, localOffset, localOffset);
}
#pragma intrinsic(memcpy)
#define BattleFx_BzEmitTilePrimVertex0 (*(short(*)[4])((char *)&g_BattleSummonBahamutZeroSlot0BAttachOffset + 0x8))

// clang-format off
#define g_BattleBahamutZeroSlot0CTo10StateWord (((BattleSummonBahamutZeroSlot0CTo10Angle *)&g_BattleSummonBahamutZeroQuadSize)->bahamutZeroSlot0CTo10StateWord)
// clang-format on

/*
 * Master timeline task driving Bahamut ZERO's space descent, beam charge, and Tera Flare impact.
 */
// FUNCTION: FF7 0x00484a16
void __cdecl BattleFx_BzMasterTimeline(void)
{
    struct {
        unsigned char unused_prefix[200];
        int file_size;
        char path[200];
        BattleFxBzTimelineState *task;
        int unused_word;
        BattleFxBzRingRecord *ring;
        int index;
        BattleFxBzMasterSlot *slot;
        int time;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = (BattleFxBzMasterSlot *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleFxBzMasterTimelineInit & 1) == 0) {
        g_BattleFxBzMasterTimelineInit |= 1;
        g_BattleSummonBahamutZeroTimelineBlock = Gfx_GetPsxScratchpadPtr(0);
    }
    MultiplyMatrix(&g_BattleCameraTransform, &g_BattleSummonBahamutZeroLightRig, &g_BattleBahamutZeroComposedMatrix);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.time = frame.slot->time;
    if (frame.time < 5) {
        if (frame.slot->time == 0) {
            // STRING: FF7 0x007f6c30
            sprintf(frame.path, "%s/SKY_A.LMD", g_BattleDataDirectoryPathBuffer);
            g_pBattleBahamutZeroSkyALmd = (void *)LoadDiskFile(&frame.file_size, (void *)frame.path);
            // STRING: FF7 0x007f6c40
            sprintf(frame.path, "%s/SKY_C.LMD", g_BattleDataDirectoryPathBuffer);
            g_pBattleBahamutZeroSkyDomeLmd = (void *)LoadDiskFile(&frame.file_size, (void *)frame.path);
            // STRING: FF7 0x007f6c50
            sprintf(frame.path, "%s/SKY_B.PLY", g_BattleDataDirectoryPathBuffer);
            g_pBattleBahamutZeroSkyBPly = (void *)LoadDiskFile(&frame.file_size, (void *)frame.path);
            // STRING: FF7 0x007f6c60
            sprintf(frame.path, "%s/SKY_D.PLY", g_BattleDataDirectoryPathBuffer);
            g_pBattleBahamutZeroSkyDPly = (void *)LoadDiskFile(&frame.file_size, (void *)frame.path);
        }
    } else {
        frame.time -= 5;
        if (frame.time < 30) {
            if (frame.time == 0) {
                frame.ring = (BattleFxBzRingRecord *)Battle_AllocSceneMessage(7);
                frame.ring->param = 7;
            }
            if (frame.time == 0) {
                BattleFx_StartScreenFlashRamp(25);
            }
            if (frame.time == 29) {
                g_BattleStageHideFlags |= 7;
                for (frame.index = 4; frame.index < 10; frame.index++) {
                    g_BattleSummonBahamutZeroSavedDrawFlags[frame.index] = g_BattleActorRecords[frame.index].drawFlags;
                    g_BattleActorRecords[frame.index].drawFlags |= 2;
                    g_BattleActorRecords[frame.index].drawFlags |= 4;
                }
            }
            if (frame.time == 0) {
                Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 0);
            }
        } else {
            frame.time -= 30;
            if (frame.time < 20) {
                if (frame.time == 5) {
                    BattleFx_FadeOutScreenFlashRamp(15);
                }
                if (frame.time == 0) {
                    frame.task = (BattleFxBzTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_BzDrawSlot04And05);
                    frame.task->rot04Z = 0;
                    frame.task->rot04Y = 0;
                    frame.task->rot04X = 0;
                    frame.task->rot05Z = 0;
                    frame.task->rot05Y = 0;
                    frame.task->rot05X = 0;
                    frame.task = (BattleFxBzTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_BzDrawSlot11);
                    frame.task = (BattleFxBzTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_BzDrawSpriteTrail);
                }
            } else {
                frame.time -= 20;
                if (frame.time < 180) {
                    BattleFx_BzSpinWorldMatrix(frame.time);
                } else {
                    frame.time -= 180;
                    if (frame.time < 30) {
                        if (frame.time == 0) {
                            frame.task = (BattleFxBzTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_BzSpawnSlot06And07);
                        }
                        if (frame.time == 5) {
                            Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 1);
                        }
                    } else {
                        frame.time -= 30;
                        if (frame.time < 15) { /* empty first arm: lib/codegen.h */
                        } else {
                            frame.time -= 15;
                            if (frame.time < 10) { /* empty first arm: lib/codegen.h */
                            } else {
                                frame.time -= 10;
                                if (frame.time < 15) { /* empty first arm: lib/codegen.h */
                                } else {
                                    frame.time -= 15;
                                    if (frame.time < 20) { /* empty first arm: lib/codegen.h */
                                    } else {
                                        frame.time -= 20;
                                        if (frame.time < 20) { /* empty first arm: lib/codegen.h */
                                        } else {
                                            frame.time -= 20;
                                            if (frame.time < 20) { /* empty first arm: lib/codegen.h */
                                            } else {
                                                frame.time -= 20;
                                                if (frame.time < 30) { /* empty first arm: lib/codegen.h */
                                                } else {
                                                    frame.time -= 30;
                                                    if (frame.time < 30) {
                                                        if (frame.time == 0) {
                                                            frame.task = (BattleFxBzTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_BzSpawnRingDebris);
                                                            frame.task = (BattleFxBzTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_BzDrawSlot09);
                                                        }
                                                    } else {
                                                        frame.time -= 30;
                                                        if (frame.time < 15) { /* empty first arm: lib/codegen.h */
                                                        } else {
                                                            frame.time -= 15;
                                                            if (frame.time < 90) {
                                                                if (frame.time == 0) {
                                                                    frame.task = (BattleFxBzTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_BzSpawnSlot0AAnd0B);
                                                                    frame.task = (BattleFxBzTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_BzDrawSlot0B);
                                                                    frame.task = (BattleFxBzTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_BzSpawnSparkSwarm);
                                                                }
                                                                if (frame.time == 75) {
                                                                    frame.task = (BattleFxBzTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_BzDrawSlot0CTo10);
                                                                }
                                                                if (frame.time == 0) {
                                                                    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 2);
                                                                    Sfx_ReleaseCachedBuffer(0x2A2, 0);
                                                                    Sfx_ReleaseCachedBuffer(0x2A3, 0);
                                                                    Sfx_ReleaseCachedBuffer(0x2A4, 0);
                                                                }
                                                            } else {
                                                                frame.time -= 90;
                                                                if (frame.time < 55) {
                                                                    if (frame.time == 15) {
                                                                        frame.task = (BattleFxBzTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_BzDrawSlot0E);
                                                                    }
                                                                    if (frame.time == 15) {
                                                                        frame.task = (BattleFxBzTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_BzDrawSlot0F);
                                                                    }
                                                                    if (frame.time == 35) {
                                                                        frame.ring = (BattleFxBzRingRecord *)Battle_AllocSceneMessage(7);
                                                                        frame.ring->param = 8;
                                                                    }
                                                                    if (frame.time == 46) {
                                                                        BattleFx_StartScreenFlashRamp(6);
                                                                    }
                                                                } else {
                                                                    frame.time -= 55;
                                                                    if (frame.time < 70) {
                                                                        if (frame.time == 0) {
                                                                            frame.ring = (BattleFxBzRingRecord *)Battle_AllocSceneMessage(7);
                                                                            frame.ring->param = 9;
                                                                        }
                                                                        if (frame.time == 0) {
                                                                            BattleFx_FadeOutScreenFlashRamp(6);
                                                                        }
                                                                        if (frame.time == 0) {
                                                                            frame.task = (BattleFxBzTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_BzDrawSkyDomeTask);
                                                                            frame.task = (BattleFxBzTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_BzEmitScreenPrim);
                                                                            frame.task = (BattleFxBzTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_BzDrawSlot12);
                                                                        }
                                                                        if (frame.time == 25) {
                                                                            frame.task = (BattleFxBzTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_BzDrawSlot10Ring);
                                                                        }
                                                                        if (frame.time == 57) {
                                                                            frame.ring = (BattleFxBzRingRecord *)Battle_AllocSceneMessage(7);
                                                                            frame.ring->param = 0xA;
                                                                        }
                                                                        if (frame.time == 10) {
                                                                            Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 3);
                                                                            Sfx_ReleaseCachedBuffer(0x2A5, 1);
                                                                            Sfx_ReleaseCachedBuffer(0x2A6, 1);
                                                                            Sfx_ReleaseCachedBuffer(0x2A7, 1);
                                                                        }
                                                                    } else {
                                                                        frame.time -= 70;
                                                                        if (frame.time < 10) {
                                                                            if (frame.time == 0) {
                                                                                g_BattleStageHideFlags &= 0xF8;
                                                                                g_BattleFxTimelineState.channel2 = 0;
                                                                                g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
                                                                                g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
                                                                                g_BattleFxTimelineState.startValue = 0;
                                                                                Battle_StartFixedPointRamp(0x1000, 10);
                                                                            }
                                                                            if (frame.time == 0) {
                                                                                for (frame.index = 4; frame.index < 10; frame.index++) {
                                                                                    g_BattleActorRecords[frame.index].drawFlags = g_BattleSummonBahamutZeroSavedDrawFlags[frame.index];
                                                                                }
                                                                            }
                                                                            if (frame.time == 9) {
                                                                                g_BattleStageHideFlags |= 7;
                                                                            }
                                                                        } else {
                                                                            frame.time -= 10;
                                                                            if (frame.time < 90) {
                                                                                if (frame.time == 0) {
                                                                                    g_pBattleBahamutZeroSubTasks = (struct BattleFxBzSubTask *)&g_BattleSummonBahamutZeroSubTaskRecords;
                                                                                    BattleFx_BzResetSubTaskTable();
                                                                                    frame.task = (BattleFxBzTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_BzDrawSlot10And16);
                                                                                    frame.task = (BattleFxBzTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_BzSpawnGroundSparks);
                                                                                    Battle_AllocateResourceSlot(BattleFx_BzRunSubTasks);
                                                                                }
                                                                                if (frame.time == 1) {
                                                                                    frame.task = (BattleFxBzTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_BzDrawSlot10Sprite);
                                                                                }
                                                                                if (frame.time == 1) {
                                                                                    frame.task = (BattleFxBzTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_BzDrawSlot13);
                                                                                }
                                                                                if (frame.time == 1) {
                                                                                    frame.task = (BattleFxBzTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_BzDrawEnergyTubeTask);
                                                                                }
                                                                                if (frame.time == 1) {
                                                                                    frame.task = (BattleFxBzTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_BzSpawnSlot14Swarm);
                                                                                }
                                                                                if (frame.time == 1) {
                                                                                    frame.task = (BattleFxBzTimelineState *)BATTLE_NEW_TASK_SLOT(BattleFx_BzSpawnDust);
                                                                                }
                                                                                if (frame.time == 60) {
                                                                                    BattleFx_StartScreenFlashRamp(15);
                                                                                }
                                                                                if (frame.time == 0) {
                                                                                    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 4);
                                                                                    Sfx_ReleaseCachedBuffer(0x2A8, 2);
                                                                                    Sfx_ReleaseCachedBuffer(0x2A9, 2);
                                                                                    Sfx_ReleaseCachedBuffer(0x2AA, 2);
                                                                                }
                                                                            } else {
                                                                                BattleFx_FadeOutScreenFlashRamp(15);
                                                                                g_BattleStageHideFlags &= 0xF8;
                                                                                g_BattleFxTimelineState.startValue = 0;
                                                                                for (frame.index = 0; frame.index < 10; frame.index++) {
                                                                                    if ((g_BattleSummonBahamutZeroTargetMask & 1 << frame.index) != 0) {
                                                                                        Battle_PostActorHitOrHurtMessage(frame.index);
                                                                                    }
                                                                                }
                                                                                g_BattleStageHideFlags = 0;
                                                                                frame.slot->ownerSlot = 0xFFFF;
                                                                                if (g_pBattleBahamutZeroEnergyTube != 0) {
                                                                                    // STRING: FF7 0x007f6c70
                                                                                    GameHeapFree(g_pBattleBahamutZeroEnergyTube, "C:\\FF7\\src\\Battle\\yasui\\vahamut0.cpp", 3104);
                                                                                    g_pBattleBahamutZeroEnergyTube = 0;
                                                                                }
                                                                                if (g_pBattleBahamutZeroSkyALmd != 0) {
                                                                                    // STRING: FF7 0x007f6c98
                                                                                    GameHeapFree(g_pBattleBahamutZeroSkyALmd, "C:\\FF7\\src\\Battle\\yasui\\vahamut0.cpp", 3110);
                                                                                    g_pBattleBahamutZeroSkyALmd = 0;
                                                                                }
                                                                                if (g_pBattleBahamutZeroSkyDomeLmd != 0) {
                                                                                    // STRING: FF7 0x007f6cc0
                                                                                    GameHeapFree(g_pBattleBahamutZeroSkyDomeLmd, "C:\\FF7\\src\\Battle\\yasui\\vahamut0.cpp", 3115);
                                                                                    g_pBattleBahamutZeroSkyDomeLmd = 0;
                                                                                }
                                                                                if (g_pBattleBahamutZeroSkyBPly != 0) {
                                                                                    // STRING: FF7 0x007f6ce8
                                                                                    GameHeapFree(g_pBattleBahamutZeroSkyBPly, "C:\\FF7\\src\\Battle\\yasui\\vahamut0.cpp", 3120);
                                                                                    g_pBattleBahamutZeroSkyBPly = 0;
                                                                                }
                                                                                if (g_pBattleBahamutZeroSkyDPly != 0) {
                                                                                    // STRING: FF7 0x007f6d10
                                                                                    GameHeapFree(g_pBattleBahamutZeroSkyDPly, "C:\\FF7\\src\\Battle\\yasui\\vahamut0.cpp", 3125);
                                                                                    g_pBattleBahamutZeroSkyDPly = 0;
                                                                                }
                                                                                Sfx_ReleaseCachedBuffer(0x2AB, 3);
                                                                                Sfx_ReleaseCachedBuffer(0x2AC, 3);
                                                                                Sfx_ReleaseCachedBuffer(0x2AD, 3);
                                                                                Sfx_ReleaseCachedBuffer(0x2AE, 4);
                                                                                Sfx_ReleaseCachedBuffer(0x2AF, 4);
                                                                                Sfx_ReleaseCachedBuffer(0x2B0, 4);
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    (frame.slot->time)++;
}

/*
 * Clears the 500-entry Bahamut ZERO sub-task table at g_BattleSummonBahamutZeroSubTasks.
 */
// FUNCTION: FF7 0x004854c1
void __cdecl BattleFx_BzResetSubTaskTable(void)
{
    int i;

    for (i = 0; i < 500; i++) {
        g_BattleSummonBahamutZeroSubTasks[i] = 0;
    }
}

/*
 * Ticks all active entries in Bahamut ZERO's 500-slot auxiliary particle/debris subtask pool.
 */
// FUNCTION: FF7 0x004854f4
void __cdecl BattleFx_BzRunSubTasks(void)
{
    struct {
        int subTaskIndex;
        BattleTaskSlot *slot;
        int active;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = BATTLE_CURRENT_TASK_SLOT;
    frame.active = 0;
    for (frame.subTaskIndex = 0; frame.subTaskIndex < 500; frame.subTaskIndex++) {
        if (g_BattleSummonBahamutZeroSubTasks[frame.subTaskIndex] != 0) {
            g_BattleSummonBahamutZeroSubTasks[frame.subTaskIndex](((int)g_pBattleBahamutZeroSubTasks) + (frame.subTaskIndex << 5));
            if (g_pBattleBahamutZeroSubTasks[frame.subTaskIndex].state == -1) {
                g_BattleSummonBahamutZeroSubTasks[frame.subTaskIndex] = 0;
            } else {
                frame.active++;
            }
        }
    }
    if (frame.active == 0 && g_BattleSecondaryTaskActiveCount <= 1) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animates and draws Bahamut ZERO's paired charge-core models in resource slots 4 and 5.
 */
// FUNCTION: FF7 0x004855a5
void __cdecl BattleFx_BzDrawSlot04And05(void)
{
    int frameNo;
    BattleFxBzSlotRecord *task;
    int ratio;

    task = (BattleFxBzSlotRecord *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleFxBzDrawSlot04And05Init & 1) == 0) {
        g_BattleFxBzDrawSlot04And05Init |= 1;
        g_BattleSummonBahamutZeroSlot0405State = (BattleFxBzModelBlock *)Gfx_GetPsxScratchpadPtr(0);
    }
    (g_BattleSummonBahamutZeroSlot04And05.bahamutZeroSlot04ModelDword) = Gfx_ModelTableGetRenderObject(4, g_pBattleBahamutZeroTableCursor);
    g_BattleSummonBahamutZeroSlot04And05.bahamutZeroSlot05ModelDword = Gfx_ModelTableGetRenderObject(5, g_pBattleBahamutZeroTableCursor);
    frameNo = task->frameCount;
    if (frameNo < 110) {
        g_BattleSummonBahamutZeroSlot04And05.bahamutZeroCurrentWordA = 0;
        g_BattleSummonBahamutZeroSlot04And05.bahamutZeroCachedWordA = g_BattleSummonBahamutZeroSlot04And05.bahamutZeroCurrentWordA;
    } else {
        frameNo -= 110;
        if (frameNo < 90) {
            g_BattleSummonBahamutZeroSlot04And05.bahamutZeroCurrentWordA = 0x1000;
            g_BattleSummonBahamutZeroSlot04And05.bahamutZeroCachedWordA = g_BattleSummonBahamutZeroSlot04And05.bahamutZeroCurrentWordA;
        } else {
            frameNo -= 90;
            if (frameNo < 55) {
                ratio = (frameNo << 12) / 55;
                g_BattleSummonBahamutZeroSlot04And05.bahamutZeroCurrentWordA = 0x1000 - ratio;
                g_BattleSummonBahamutZeroSlot04And05.bahamutZeroCachedWordA = g_BattleSummonBahamutZeroSlot04And05.bahamutZeroCurrentWordA;
            } else {
                frameNo -= 55;
                if (frameNo < 295) {
                    g_BattleSummonBahamutZeroSlot04And05.bahamutZeroCurrentWordA = 0;
                    g_BattleSummonBahamutZeroSlot04And05.bahamutZeroCachedWordA = g_BattleSummonBahamutZeroSlot04And05.bahamutZeroCurrentWordA;
                } else {
                    task->ownerSlot = BATTLE_TASK_FREE;
                    return;
                }
            }
        }
    }
    RotMatrix((SVECTOR *)((int)task + 4), &g_BattleSummonBahamutZeroSlot0405State->matrix);
    g_BattleSummonBahamutZeroSlot0405State->matrix.t[2] = 0;
    g_BattleSummonBahamutZeroSlot0405State->matrix.t[1] = 0;
    g_BattleSummonBahamutZeroSlot0405State->matrix.t[0] = 0;
    ScaleMatrix(&g_BattleSummonBahamutZeroSlot0405State->matrix, &g_BattleSummonBahamutZeroSlot04Scale);
    MultiplyMatrix(&g_BattleBahamutZeroComposedMatrix, &g_BattleSummonBahamutZeroSlot0405State->matrix, &g_BattleSummonBahamutZeroSlot0405State->matrix);
    SetRotMatrix(&g_BattleSummonBahamutZeroSlot0405State->matrix);
    SetTransMatrix(&g_BattleSummonBahamutZeroSlot0405State->matrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonBahamutZeroSlot04And05, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST_3, 0, g_BattleOverlayDrawListHead);
    RotMatrix((SVECTOR *)((int)task + 0xC), &g_BattleSummonBahamutZeroSlot0405State->matrix);
    g_BattleSummonBahamutZeroSlot0405State->matrix.t[2] = 0;
    g_BattleSummonBahamutZeroSlot0405State->matrix.t[1] = 0;
    g_BattleSummonBahamutZeroSlot0405State->matrix.t[0] = 0;
    ScaleMatrix(&g_BattleSummonBahamutZeroSlot0405State->matrix, &g_BattleSummonBahamutZeroSlot05Scale);
    MultiplyMatrix(&g_BattleBahamutZeroComposedMatrix, &g_BattleSummonBahamutZeroSlot0405State->matrix, &g_BattleSummonBahamutZeroSlot0405State->matrix);
    SetRotMatrix(&g_BattleSummonBahamutZeroSlot0405State->matrix);
    SetTransMatrix(&g_BattleSummonBahamutZeroSlot0405State->matrix);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonBahamutZeroSlot04And05.bahamutZeroSlot05ModelDword, g_BattleCurrentOrderingTable + 0x405C, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frameCount)++;
        task->rotY = task->rotY;
        task->countdown += 0xFFFC;
    }
}

/*
 * Scales and submits Bahamut ZERO's orbital flare model in resource slot 17.
 */
// FUNCTION: FF7 0x004858aa
void __cdecl BattleFx_BzDrawSlot11(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleFxBzDrawSlot11Init & 1) == 0) {
        g_BattleFxBzDrawSlot11Init |= 1;
        g_pBattleBahamutZeroSlot11Matrix = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    }
    (*(int *)&g_BattleSummonBahamutZeroSlot04Params) = Gfx_ModelTableGetRenderObject(17, g_pBattleBahamutZeroTableCursor);
    ConcatMatrixPreservingTranslation(&g_BattleBahamutZeroComposedMatrix, &g_BattleSummonBahamutZeroSlot11LocalMatrix, g_pBattleBahamutZeroSlot11Matrix);
    ScaleMatrixL(g_pBattleBahamutZeroSlot11Matrix, &g_BattleSummonBahamutZeroSlot11Scale);
    SetRotMatrix(g_pBattleBahamutZeroSlot11Matrix);
    SetTransMatrix(&g_BattleSummonBahamutZeroSlot11LocalMatrix);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonBahamutZeroSlot04Params, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= 550) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws the multi-sprite exhaust/beam trail across Bahamut ZERO resource slots 30..32.
 */
// FUNCTION: FF7 0x004859aa
void __cdecl BattleFx_BzDrawSpriteTrail(void)
{
    struct {
        int resources[3];
        int at32;
        int at28;
        int at24;
        int at20;
        int at16;
        int at12;
        BattleTaskSlot *at8;
        int at4;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.at8 = BATTLE_CURRENT_TASK_SLOT;
    frame.resources[0] = 0x1E;
    frame.resources[1] = 0x1F;
    frame.resources[2] = 0x20;
    if ((g_BattleFxBzDrawSpriteTrailInit & 1) == 0) {
        g_BattleFxBzDrawSpriteTrailInit |= 1;
        g_BattleSummonBahamutZeroTrail = (struct BattleFxBahamutZeroTrail *)(Gfx_GetPsxScratchpadPtr(0));
    }
    g_BattleSummonBahamutZeroTrail->m00 = 0x1000;
    g_BattleSummonBahamutZeroTrail->m01 = 0;
    g_BattleSummonBahamutZeroTrail->deltaY = 0;
    g_BattleSummonBahamutZeroTrail->m10 = 0;
    g_BattleSummonBahamutZeroTrail->halfHeight = g_BattleBahamutZeroComposedMatrix.halfHeight;
    g_BattleSummonBahamutZeroTrail->negHalfWidth = -g_BattleBahamutZeroComposedMatrix.halfWidth;
    g_BattleSummonBahamutZeroTrail->m20 = 0;
    g_BattleSummonBahamutZeroTrail->halfWidth = g_BattleBahamutZeroComposedMatrix.halfWidth;
    g_BattleSummonBahamutZeroTrail->deltaX = g_BattleBahamutZeroComposedMatrix.halfHeight;
    ConcatMatrixInPlace(&g_BattleBahamutZeroComposedMatrix, &g_BattleSummonBahamutZeroTrail->m00);
    frame.at16 = ArcTangentFixed(g_BattleSummonBahamutZeroTrail->deltaX, g_BattleSummonBahamutZeroTrail->deltaY);
    frame.at12 = frame.at16 << 11 >> 12;
    g_BattleSummonBahamutZeroTrail->startX = g_BattleBahamutZeroComposedMatrix.startX;
    g_BattleSummonBahamutZeroTrail->startY = 0;
    g_BattleSummonBahamutZeroTrail->depth = g_BattleBahamutZeroComposedMatrix.depth;
    g_BattleSummonBahamutZeroTrailDistSq = NormalizeVectorFixed(&g_BattleSummonBahamutZeroTrail->startX, &g_BattleSummonBahamutZeroTrail->startX);
    frame.at4 = SqrtToInt(g_BattleSummonBahamutZeroTrailDistSq);
    frame.at16 = ArcTangentFixed(g_BattleBahamutZeroComposedMatrix.halfWidth, frame.at4);
    frame.at32 = frame.at16 << 11 >> 12;
    frame.at20 = frame.at12 & 0xFF;
    frame.at24 = frame.at32 & 0xFF;
    g_BattleSummonBahamutZeroSpriteParams.x = -frame.at20;
    g_BattleSummonBahamutZeroSpriteParams.y = -frame.at24;
    Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(29, g_pBattleBahamutZeroTableCursor)));
    g_BattleOverlayDrawListHead = (int)BattleFx_BzEmitSprite(&g_BattleSummonBahamutZeroSpriteParams, 0.9999f, g_BattleOverlayDrawListHead);
    g_BattleSummonBahamutZeroSpriteParams.x += 256;
    g_BattleOverlayDrawListHead = (int)BattleFx_BzEmitSprite(&g_BattleSummonBahamutZeroSpriteParams, 0.9999f, g_BattleOverlayDrawListHead);
    g_BattleSummonBahamutZeroSpriteParams.x += 256;
    g_BattleOverlayDrawListHead = (int)BattleFx_BzEmitSprite(&g_BattleSummonBahamutZeroSpriteParams, 0.9999f, g_BattleOverlayDrawListHead);
    g_BattleSummonBahamutZeroSpriteParams.y += 256;
    g_BattleOverlayDrawListHead = (int)BattleFx_BzEmitSprite(&g_BattleSummonBahamutZeroSpriteParams, 0.9999f, g_BattleOverlayDrawListHead);
    g_BattleSummonBahamutZeroSpriteParams.x -= 256;
    g_BattleOverlayDrawListHead = (int)BattleFx_BzEmitSprite(&g_BattleSummonBahamutZeroSpriteParams, 0.9999f, g_BattleOverlayDrawListHead);
    g_BattleSummonBahamutZeroSpriteParams.x -= 256;
    g_BattleOverlayDrawListHead = (int)BattleFx_BzEmitSprite(&g_BattleSummonBahamutZeroSpriteParams, 0.9999f, g_BattleOverlayDrawListHead);
    for (frame.at28 = 0; frame.at28 < 3; frame.at28++) {
        frame.at20 = g_BattleSummonBahamutZeroTrailOrigins[frame.at28].u - frame.at12;
        frame.at20 = (frame.at20 % 768 + 0x300) % 768 - 0x80;
        frame.at24 = g_BattleSummonBahamutZeroTrailOrigins[frame.at28].v - frame.at32;
        frame.at24 = (frame.at24 % 768 + 0x300) % 768 - 0x80;
        frame.at20 <<= 1;
        frame.at24 <<= 1;
        g_BattleSummonBahamutZeroTrailSprites[frame.at28].x = *(short *)&frame.at20;
        g_BattleSummonBahamutZeroTrailSprites[frame.at28].y = *(short *)&frame.at24;
        Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(frame.resources[frame.at28], g_pBattleBahamutZeroTableCursor)));
        g_BattleOverlayDrawListHead = (int)BattleFx_BzEmitSprite(&g_BattleSummonBahamutZeroTrailSprites[frame.at28], 0.999f, g_BattleOverlayDrawListHead);
    }
    if (g_BattleIsPaused == 0) {
        (frame.at8->frameCounter)++;
        if (frame.at8->frameCounter >= 550) {
            frame.at8->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Builds and submits a textured PSX sprite packet with explicit blend factor for Bahamut ZERO.
 */
// FUNCTION: FF7 0x00485dda
void *__cdecl BattleFx_BzEmitSprite(BattleFxBzSpriteParams *spriteParams, float blendFactor, int value)
{
    BattleFxSpritePrim *primitive;

    primitive = (BattleFxSpritePrim *)Gfx_GetPsxPacketScratchPtr();
    primitive->packetTag = 0x4000000;
    primitive->colorWord = spriteParams->colorCmd;
    primitive->packedXY = spriteParams->packedXY;
    primitive->u = spriteParams->u;
    primitive->v = spriteParams->v;
    primitive->clut = spriteParams->clut;
    primitive->packedSize = spriteParams->packedSize;
    /* Retail caller prototype (float, float) -> void preserved for /Od argument promotion on `Gfx_SetBlendFactorPair`. */
    ((void(__cdecl *)(float, float))Gfx_SetBlendFactorPair)(blendFactor, 1.0f);
    primitive->code |= 2;
    Gfx_DrawPsxPrimitive(primitive);
    return (void *)primitive;
}

/*
 * Spawns the staggered Bahamut ZERO beam-lance model tasks for resource slots 6 and 7.
 */
// FUNCTION: FF7 0x00485e6a
void __cdecl BattleFx_BzSpawnSlot06And07(void)
{
    BattleTaskSlot *task;
    int spawned;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0) {
        spawned = (int)BATTLE_NEW_TASK_SLOT(BattleFx_BzDrawSlot06);
        g_BattleSummonBahamutZeroSpawnCount++;
    }
    if (task->frameCounter == 6) {
        spawned = (int)BATTLE_NEW_TASK_SLOT(BattleFx_BzDrawSlot07);
        g_BattleSummonBahamutZeroSpawnCount++;
    }
    if (task->frameCounter == 23) {
        spawned = (int)BATTLE_NEW_TASK_SLOT(BattleFx_BzDrawGlowSprite);
        g_BattleSummonBahamutZeroSpawnCount++;
    }
    if (g_BattleSummonBahamutZeroSpawnCount >= 3) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/*
 * Eases position along the beam axis and draws Bahamut ZERO's inner beam model (slot 6).
 */
// FUNCTION: FF7 0x00485f5b
void __cdecl BattleFx_BzDrawSlot06(void)
{
    int frameNo;
    BattleTaskSlot *task;
    PsxMatrix *aimMatrix;
    int easedValue;

    task = BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSummonBahamutZeroSlot06And07.bahamutZeroSlot06ModelDword) = Gfx_ModelTableGetRenderObject(6, g_pBattleBahamutZeroTableCursor);
    aimMatrix = (PsxMatrix *)BattleFx_BzAimMatrixAlongDelta(&g_BattleSummonBahamutZeroSlot06AimOrigin, 0x2800, 0);
    frameNo = task->frameCounter;
    if (frameNo < 30) {
        easedValue = (frameNo << 12) / 30;
        easedValue = BattleFx_BzEaseFixed(easedValue, 0x180);
        aimMatrix->t[0] = aimMatrix->t[0] + (easedValue * -500 >> 12) + -0x190;
        aimMatrix->t[1] = aimMatrix->t[1] + (easedValue * -700 >> 12) + -0x1F4;
    } else {
        frameNo -= 30;
        if (frameNo < 80) {
            aimMatrix->t[0] -= 0x384;
            aimMatrix->t[1] -= 0x4B0;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    SetRotMatrix(aimMatrix);
    SetTransMatrix(aimMatrix);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonBahamutZeroSlot06And07, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST_3, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
    }
}

/*
 * Builds an aimed PSX transform matrix aligned with the projected delta from srcVec.
 */
// FUNCTION: FF7 0x004860b2
int __cdecl BattleFx_BzAimMatrixAlongDelta(void *srcVec, short stateValue, int scale)
{
    int rotated;
    VECTOR normalized;

    (g_BattleSummonBahamutZeroAimMatrixTail.stateValue) = stateValue;
    g_BattleSummonBahamutZeroAimMatrixM11 = (g_BattleSummonBahamutZeroAimMatrixTail.stateValue);
    *(short *)&g_BattleSummonBahamutZeroAimMatrix = g_BattleSummonBahamutZeroAimMatrixM11;
    SetRotMatrix(&g_BattleBahamutZeroComposedMatrix);
    SetTransMatrix(&g_BattleBahamutZeroComposedMatrix);
    RotTransPoint(srcVec, &g_BattleSummonBahamutZeroAimMatrixTail.aimVector, &rotated);
    if (scale != 0) {
        NormalizeVectorFixed(&g_BattleSummonBahamutZeroAimMatrixTail.aimVector, &normalized);
        g_BattleSummonBahamutZeroAimMatrixTail.aimAccumulator += normalized.vz * scale >> 12;
    }
    SetRotMatrix(&g_BattleSummonBahamutZeroAimMatrix);
    SetTransMatrix(&g_BattleSummonBahamutZeroAimMatrix);
    return (int)&g_BattleSummonBahamutZeroAimMatrix;
}

/*
 * Eases position along the beam axis and draws Bahamut ZERO's outer beam model (slot 7).
 */
// FUNCTION: FF7 0x0048615f
void __cdecl BattleFx_BzDrawSlot07(void)
{
    int frameNo;
    BattleTaskSlot *task;
    PsxMatrix *aimMatrix;
    int easedValue;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSummonBahamutZeroSlot06And07.bahamutZeroSlot07ModelDword = Gfx_ModelTableGetRenderObject(7, g_pBattleBahamutZeroTableCursor);
    aimMatrix = (PsxMatrix *)BattleFx_BzAimMatrixAlongDelta(&g_BattleSummonBahamutZeroSlot07AimOrigin, 0x3000, 0);
    frameNo = task->frameCounter;
    if (frameNo < 60) {
        easedValue = (frameNo << 12) / 60;
        easedValue = BattleFx_BzEaseFixed(easedValue, 0x180);
        aimMatrix->t[0] = aimMatrix->t[0] + (easedValue * -1250 >> 12) + -0x4B0;
        aimMatrix->t[1] = aimMatrix->t[1] + (easedValue * -1500 >> 12) + -0xBB8;
    } else {
        frameNo -= 60;
        if (frameNo < 44) {
            aimMatrix->t[0] -= 0x992;
            aimMatrix->t[1] -= 0x1194;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    SetRotMatrix(aimMatrix);
    SetTransMatrix(aimMatrix);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonBahamutZeroSlot06And07.bahamutZeroSlot07ModelDword, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST_3, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
    }
}

/*
 * Ramps intensity and draws the muzzle/impact glow billboard sprite for Bahamut ZERO.
 */
// FUNCTION: FF7 0x004862b6
void __cdecl BattleFx_BzDrawGlowSprite(void)
{
    int sinValue;
    int fade;
    int age;
    BattleTaskSlot *task;
    int spriteIdx;
    int cosValue;
    struct BattleFxBzDrawGlowSpriteW *baseMatrixPtr;
    int bytes;

    task = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleFxBzDrawGlowSpriteInit & 1) == 0) {
        g_BattleFxBzDrawGlowSpriteInit |= 1;
        g_BattleSummonBahamutZeroGlowState = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    }
    age = task->frameCounter;
    if (age < 3) {
        bytes = (age << 12) / 3;
        fade = 0;
    } else {
        age -= 3;
        if (age < 22) {
            bytes = 0x1000;
            fade = (age << 12) / 22;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    SetDrawColorRGB(0, 0, 0);
    Gfx_ShadePsxVertexColor((CVECTOR *)&g_BattleSummonBahamutZeroGlowBaseColor, fade, g_BattleSummonBahamutZeroGlowShadedColor);
    baseMatrixPtr = (struct BattleFxBzDrawGlowSpriteW *)BattleFx_BzAimMatrixAlongDelta(&g_BattleSummonBahamutZeroGlowAimOrigin, 0x2000, 0xFFFFEC78);
    g_BattleSummonBahamutZeroGlowState->t[2] = baseMatrixPtr->baseZ;
    sinValue = SinFixed(0x280);
    cosValue = CosFixed(0x280);
    for (spriteIdx = 0; spriteIdx < 5; spriteIdx++) {
        g_BattleSummonBahamutZeroGlowState->t[0] = baseMatrixPtr->baseX + ((g_BattleSummonBahamutZeroGlowOffsets.offsets[spriteIdx].glowOffset * sinValue >> 12) * bytes >> 12) + -0x6A4;
        g_BattleSummonBahamutZeroGlowState->t[1] = baseMatrixPtr->baseY + ((g_BattleSummonBahamutZeroGlowOffsets.offsets[spriteIdx].glowOffset * cosValue >> 12) * bytes >> 12) + -0x1194;
        SetTransMatrix(g_BattleSummonBahamutZeroGlowState);
        g_BattleSummonBahamutZeroGlowOffsets.spriteFrameTable = Gfx_ModelTableGetLoadedRecord((&g_BattleSummonBahamutZeroGlowBaseColor.bahamutZeroGlowTableBase)[g_BattleSummonBahamutZeroGlowOffsets.offsets[spriteIdx].glowIndex], g_pBattleBahamutZeroTableCursor);
        g_BattleSummonBahamutZeroGlowOffsets.spriteValue = Gfx_ModelTableGetSpriteObject((&g_BattleSummonBahamutZeroGlowBaseColor.bahamutZeroGlowTableBase)[g_BattleSummonBahamutZeroGlowOffsets.offsets[spriteIdx].glowIndex], g_pBattleBahamutZeroTableCursor);
        g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSummonBahamutZeroGlowOffsets.spriteFrameTable, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_BattleOverlayDrawListHead);
    }
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
    }
}

/*
 * Spawns outward-flying ground ring debris subtasks during Bahamut ZERO's Tera Flare impact.
 */
// FUNCTION: FF7 0x004864eb
void __cdecl BattleFx_BzSpawnRingDebris(void)
{
    struct {
        int randomRadius;
        BattleFxRingDebrisRecord *debrisRecord;
        int spawnBudget;
        BattleTaskSlot *ringDebrisTask;
        int debrisRadius;
        int debrisAngle;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.ringDebrisTask = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleFxBzSpawnRingDebrisInit & 1) == 0) {
        g_BattleFxBzSpawnRingDebrisInit |= 1;
        g_BattleSummonBahamutZeroRingDebrisBlock = Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spawnBudget = (frame.ringDebrisTask->frameCounter << 0xB & 0xFF) + 0x800;
    while (frame.spawnBudget >= 256) {
        frame.debrisRecord = (BattleFxRingDebrisRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_BzDrawSlot08);
        frame.debrisRecord->rotX = 0;
        frame.debrisRecord->rotY = 0;
        frame.debrisAngle = rand() & 0xFFF;
        frame.debrisRecord->angle = frame.debrisAngle;
        if (DEAD_IN_RETAIL != 0) {
            frame.randomRadius = 2000;
        } else {
            frame.randomRadius = rand() % 600 + 1400;
        }
        frame.debrisRadius = frame.randomRadius;
        frame.debrisRecord->offsetX = frame.debrisRadius * SinFixed(frame.debrisAngle) >> 12;
        frame.debrisRecord->offsetZ = -frame.debrisRadius * CosFixed(frame.debrisAngle) >> 12;
        frame.debrisRecord->deltaZ = 0;
        frame.debrisRecord->startX = 0;
        frame.debrisRecord->startY = 0xD8F0;
        frame.debrisRecord->startZ = -10000;
        frame.spawnBudget -= 0x100;
    }
    (frame.ringDebrisTask->frameCounter)++;
    if (frame.ringDebrisTask->frameCounter >= 35) {
        frame.ringDebrisTask->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Scales and draws Bahamut ZERO's shock-column model in resource slot 8.
 */
// FUNCTION: FF7 0x00486670
void __cdecl BattleFx_BzDrawSlot08(void)
{
    BattleFxBzSlot08Record *taskPtr;
    int unusedSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int progress;
    int travelScale;

    taskPtr = (BattleFxBzSlot08Record *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleFxBzDrawSlot08Init & 1) == 0) {
        g_BattleFxBzDrawSlot08Init |= 1;
        g_pBattleBahamutZeroSlot08State = (BattleFxBzModelBlock *)Gfx_GetPsxScratchpadPtr(0);
    }
    g_BattleSummonBahamutZeroSlot08.bahamutZeroSlot08ModelDword = Gfx_ModelTableGetRenderObject(8, g_pBattleBahamutZeroTableCursor);
    progress = (taskPtr->frame << 12) / 10;
    travelScale = 0x1000 - BattleFx_BzEaseFixed(progress, 0) + 0x2FF;
    RotMatrix((SVECTOR *)((int)taskPtr + 0x14), &g_pBattleBahamutZeroSlot08State->matrix);
    g_pBattleBahamutZeroSlot08State->matrix.t[0] = taskPtr->startX + (taskPtr->deltaX * travelScale >> 12);
    g_pBattleBahamutZeroSlot08State->matrix.t[1] = taskPtr->startY + (taskPtr->deltaY * travelScale >> 12);
    g_pBattleBahamutZeroSlot08State->matrix.t[2] = taskPtr->startZ + (taskPtr->deltaZ * travelScale >> 12);
    g_pBattleBahamutZeroSlot08State->scaleZ = 0x400 /* 0.25 */;
    g_pBattleBahamutZeroSlot08State->scaleX = 0x400 /* 0.25 */;
    g_pBattleBahamutZeroSlot08State->scaleY = (0x1000 - progress) * 0x5000 /* 5.0 */ >> 12;
    ScaleMatrix(&g_pBattleBahamutZeroSlot08State->matrix, g_pBattleBahamutZeroSlot08State);
    MultiplyMatrix(&g_BattleBahamutZeroComposedMatrix, &g_pBattleBahamutZeroSlot08State->matrix, &g_pBattleBahamutZeroSlot08State->matrix);
    SetRotMatrix(&g_pBattleBahamutZeroSlot08State->matrix);
    SetTransMatrix(&g_pBattleBahamutZeroSlot08State->matrix);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonBahamutZeroSlot08.bahamutZeroSlot08ModelDword, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (taskPtr->frame)++;
        if (taskPtr->frame >= 10) {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Animates expansion and submits Bahamut ZERO's ground-crater ring model in resource slot 9.
 */
// FUNCTION: FF7 0x0048685c
void __cdecl BattleFx_BzDrawSlot09(void)
{
    int scaledAmount;
    int frameNo;
    BattleTaskSlot *task;
    int ratio;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSummonBahamutZeroSlot09.bahamutZeroSlot09ModelDword = Gfx_ModelTableGetRenderObject(9, g_pBattleBahamutZeroTableCursor);
    frameNo = task->frameCounter;
    if (frameNo < 10) {
        g_BattleSummonBahamutZeroSlot09.bahamutZeroSlot09Word = 0;
    } else {
        frameNo -= 10;
        if (frameNo < 35) {
            ratio = (frameNo << 12) / 35;
            g_BattleSummonBahamutZeroSlot09.bahamutZeroSlot09Word = ratio;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    ratio = (task->frameCounter << 12) / 45;
    ratio = BattleFx_BzEaseFixed(ratio, 0);
    scaledAmount = ratio * 0xA00 /* 0.625 */ >> 12;
    BattleFx_BzAimMatrixAlongDelta(&g_BattleSummonBahamutZeroSlot0AParams, (short)scaledAmount, 0);
    SetDrawColorRGB(0, 0, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonBahamutZeroSlot09.bahamutZeroSlot09ModelDword, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
    }
}

/*
 * Periodically spawns Bahamut ZERO shockwave ring tasks for resource slots 10 and 11.
 */
// FUNCTION: FF7 0x0048697c
void __cdecl BattleFx_BzSpawnSlot0AAnd0B(void)
{
    BattleTaskSlot *task;
    int ratio;
    int spawned;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    ratio = (task->frameCounter << 12) / 65;
    g_BattleSummonBahamutZeroSpawnPhase = g_BattleSummonBahamutZeroSpawnPhase + (ratio * 0x7C >> 12) + 4;
    while (g_BattleSummonBahamutZeroSpawnPhase >= 256) {
        spawned = (int)BATTLE_NEW_TASK_SLOT(BattleFx_BzDrawSlot0A);
        g_BattleSummonBahamutZeroSpawnPhase -= 0x100;
    }
    (task->frameCounter)++;
    if (task->frameCounter >= 65) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Scales and submits Bahamut ZERO's inner shockwave ring model in resource slot 10.
 */
// FUNCTION: FF7 0x00486a35
void __cdecl BattleFx_BzDrawSlot0A(void)
{
    int frameCount;
    BattleTaskSlot *task;
    int ratio;

    task = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleFxBzDrawSlot0AInit & 1) == 0) {
        g_BattleFxBzDrawSlot0AInit |= 1;
        g_pBattleBahamutZeroSlot0AMatrix = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    }
    g_BattleSummonBahamutZeroSlot0AParams.bahamutZeroSlot0ADrawHandle = Gfx_ModelTableGetRenderObject(10, g_pBattleBahamutZeroTableCursor);
    frameCount = task->frameCounter;
    if (frameCount < 5) {
        ratio = (frameCount << 12) / 5;
        g_BattleSummonBahamutZeroSlot0AParams.bahamutZeroSlot0AScaleFactor = 0x1000 - ratio;
    } else {
        frameCount -= 5;
        if (frameCount < 5) {
            g_BattleSummonBahamutZeroSlot0AParams.bahamutZeroSlot0AScaleFactor = 0;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    ratio = (task->frameCounter << 12) / 10;
    ratio = BattleFx_BzEaseFixed(ratio, 0xFFFFFD01);
    g_BattleSummonBahamutZeroSlot0AParams.bahamutZeroSlot0ARotationAngle2 = (ratio * -4096 >> 12) + 0x1000;
    g_BattleSummonBahamutZeroSlot0AParams.bahamutZeroSlot0ARotationAngle1 = g_BattleSummonBahamutZeroSlot0AParams.bahamutZeroSlot0ARotationAngle2;
    g_BattleSummonBahamutZeroSlot0AParams.bahamutZeroSlot0ARotationAngle0 = g_BattleSummonBahamutZeroSlot0AParams.bahamutZeroSlot0ARotationAngle1;
    Battle3d_GetActorAttachMatrix(3, 6, (PsxMatrix *)&g_BattleActorRecords[3].boneNodes5[0x34]);
    MultiplyMatrix(&g_BattleActorRecords[3].boneNodes5[0x34], &g_BattleSummonBahamutZeroSlot0AParams.bahamutZeroSlot0ARotationAngle0, g_pBattleBahamutZeroSlot0AMatrix);
    SetRotMatrix(g_pBattleBahamutZeroSlot0AMatrix);
    SetTransMatrix(g_pBattleBahamutZeroSlot0AMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonBahamutZeroSlot0AParams.bahamutZeroSlot0ADrawHandle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
    }
}

/*
 * Rotates, eases, and draws Bahamut ZERO's outer shockwave ring model in resource slot 11.
 */
// FUNCTION: FF7 0x00486bdf
void __cdecl BattleFx_BzDrawSlot0B(void)
{
    int angle;
    int frameCount;
    BattleTaskSlot *task;
    int ratio;
    int outVec;
    int stepLength[4];

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSummonBahamutZeroSlot0BDraw.bahamutZeroSlot0BDrawHandle = Gfx_ModelTableGetRenderObject(11, g_pBattleBahamutZeroTableCursor);
    frameCount = task->frameCounter;
    if (frameCount < 75) {
        ratio = (frameCount << 12) / 75;
        ratio = BattleFx_BzEaseFixed(ratio, 0);
        angle = ratio << 10 >> 12;
    } else {
        frameCount -= 75;
        if (frameCount < 8) {
            ratio = ((frameCount << 12) / 8);
            ratio = BattleFx_BzEaseFixed(ratio, 0x4FF);
            angle = (ratio * 0x1C00 /* 1.75 */ >> 12) + 0x400;
        } else {
            frameCount -= 8;
            if (frameCount < 2) {
                ratio = ((frameCount << 12) / 2);
                ratio = BattleFx_BzEaseFixed(ratio, 0x1FF);
                angle = (ratio * -6656 >> 12) + 0x2000;
            } else {
                frameCount -= 2;
                if (frameCount < 5) {
                    angle = 0x600 /* 135 deg */;
                } else {
                    task->ownerSlot = BATTLE_TASK_FREE;
                    return;
                }
            }
        }
    }
    (g_BattleSummonBahamutZeroSlot0BState.matrixM22) = angle;
    g_BattleSummonBahamutZeroSlot0BMatrixM11 = (g_BattleSummonBahamutZeroSlot0BState.matrixM22);
    *(short *)&g_BattleSummonBahamutZeroSlot0BMatrix = g_BattleSummonBahamutZeroSlot0BMatrixM11;
    Battle3d_GetActorAttachMatrix(3, 6, (PsxMatrix *)&g_BattleActorRecords[3].boneNodes5[0x34]);
    SetRotMatrix(&g_BattleActorRecords[3].boneNodes5[0x34]);
    SetTransMatrix(&g_BattleActorRecords[3].boneNodes5[0x34]);
    RotTransPoint(&g_BattleSummonBahamutZeroSlot0BAttachOffset, (int *)&g_BattleSummonBahamutZeroSlot0BState.bahamutZeroSlot0BPosition0, &outVec);
    NormalizeVectorFixed((int *)&g_BattleSummonBahamutZeroSlot0BState.bahamutZeroSlot0BPosition0, stepLength);
    g_BattleSummonBahamutZeroSlot0BState.bahamutZeroSlot0BPosition0 += (stepLength[0] * 0) >> 12;
    g_BattleSummonBahamutZeroSlot0BState.bahamutZeroSlot0BPosition1 += (stepLength[0] * 0) >> 12;
    g_BattleSummonBahamutZeroSlot0BState.bahamutZeroSlot0BPosition2 += (stepLength[0] * 0) >> 12;
    SetRotMatrix(&g_BattleSummonBahamutZeroSlot0BMatrix);
    SetTransMatrix(&g_BattleSummonBahamutZeroSlot0BMatrix);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonBahamutZeroSlot0BDraw.bahamutZeroSlot0BDrawHandle, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
    }
}

/*
 * Spawns batches of Tera Flare spark particles per frame while the swarm emitter is active.
 */
// FUNCTION: FF7 0x00486e18
void __cdecl BattleFx_BzSpawnSparkSwarm(void)
{
    struct {
        int randomParamT;
        int randomParamS;
        BattleFxLivewireParticle *sparkParticle;
        int spawnBudget;
        BattleFxLivewireSwarm *sparkSwarmTask;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.sparkSwarmTask = (BattleFxLivewireSwarm *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spawnBudget = (frame.sparkSwarmTask->frame << 8 & 0xFF) + 0x100;
    while (frame.spawnBudget >= 256) {
        frame.sparkParticle = (BattleFxLivewireParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_BzDrawSparkSwarm);
        frame.sparkParticle->spawnFrame = frame.sparkSwarmTask->frame;
        frame.sparkParticle->posX = 0;
        frame.sparkParticle->posY = 150;
        frame.sparkParticle->posZ = 0xFD12;
        frame.sparkParticle->rotationY = 0;
        frame.sparkParticle->rotationX = 0;
        g_BattleBahamutZeroSparkAngle = g_BattleBahamutZeroSparkAngle + (rand() & 0x7FF) + 0x400;
        frame.sparkParticle->rotationZ = *(short *)&g_BattleBahamutZeroSparkAngle;
        if (DEAD_IN_RETAIL != 0) {
            frame.randomParamS = 0x40;
        } else {
            frame.randomParamS = rand() % 32 + 32;
        }
        frame.sparkParticle->paramS = frame.randomParamS;
        if ((rand() & 0x800) != 0) {
            frame.sparkParticle->paramS = -frame.sparkParticle->paramS;
        }
        if (DEAD_IN_RETAIL != 0) {
            frame.randomParamT = 0x1000;
        } else {
            frame.randomParamT = rand() % 2560 + 1536;
        }
        frame.sparkParticle->paramT = frame.randomParamT;
        frame.spawnBudget -= 0x100;
    }
    (frame.sparkSwarmTask->frame)++;
    if (frame.sparkSwarmTask->frame >= 63) {
        frame.sparkSwarmTask->step = 0xFFFF;
    }
}

/*
 * Updates trajectory and draws one spark particle in Bahamut ZERO's Tera Flare swarm.
 */
// FUNCTION: FF7 0x00486fa0
void __cdecl BattleFx_BzDrawSparkSwarm(void)
{
    int frameIndex;
    int fraction;
    BattleFxLivewireSwarmRecord *recordPtr;
    int depth;
    BattleFxBzLivewireSwarmPrim *prim;

    recordPtr = (BattleFxLivewireSwarmRecord *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleFxBzDrawSparkSwarmInit & 1) == 0) {
        g_BattleFxBzDrawSparkSwarmInit |= 1;
        g_BattleSummonBahamutZeroSparkState = (BattleFxBahamutZeroLivewire *)Gfx_GetPsxScratchpadPtr(0);
    }
    frameIndex = recordPtr->frame;
    if (frameIndex < 3) {
        fraction = (frameIndex << 12) / 3;
        g_BattleSummonBahamutZeroSparkState->scale = recordPtr->scale * fraction >> 12;
    } else {
        frameIndex -= 3;
        if (frameIndex < 6) {
            g_BattleSummonBahamutZeroSparkState->scale = recordPtr->scale;
        } else {
            frameIndex -= 6;
            if (frameIndex < 3) {
                fraction = (frameIndex << 12) / 3;
                g_BattleSummonBahamutZeroSparkState->scale = recordPtr->scale * (0x1000 - fraction) >> 12;
            } else {
                recordPtr->step = 0xFFFF;
                return;
            }
        }
    }
    Battle3d_GetActorAttachMatrix(3, 6, (PsxMatrix *)&g_BattleActorRecords[3].boneNodes5[0x34]);
    SetRotMatrix(&g_BattleActorRecords[3].boneNodes5[0x34]);
    SetTransMatrix(&g_BattleActorRecords[3].boneNodes5[0x34]);
    g_BattleSummonBahamutZeroSparkState->depth = RotTransPers((SVECTOR *)((int)recordPtr + 4), &g_BattleSummonBahamutZeroSparkState->screenX, &g_BattleSummonBahamutZeroSparkState->projectedA, &g_BattleSummonBahamutZeroSparkState->projectedB);
    RotMatrix((SVECTOR *)((int)recordPtr + 0xC), &g_BattleSummonBahamutZeroSparkState->matrix);
    GetGeomOffset(&g_BattleSummonBahamutZeroSparkState->geomOffsetX, &g_BattleSummonBahamutZeroSparkState->geomOffsetY);
    g_BattleSummonBahamutZeroSparkState->matrix.t[0] = g_BattleSummonBahamutZeroSparkState->screenX - g_BattleSummonBahamutZeroSparkState->geomOffsetX;
    g_BattleSummonBahamutZeroSparkState->matrix.t[1] = g_BattleSummonBahamutZeroSparkState->screenY - g_BattleSummonBahamutZeroSparkState->geomOffsetY;
    g_BattleSummonBahamutZeroSparkState->matrix.t[2] = GetGeomScreen();
    g_BattleSummonBahamutZeroSparkState->scaleZ = 0x1000 /* 1.0 */;
    g_BattleSummonBahamutZeroSparkState->scaleY = 0x1000 /* 1.0 */;
    ScaleMatrix(&g_BattleSummonBahamutZeroSparkState->matrix, g_BattleSummonBahamutZeroSparkState);
    SetRotMatrix(&g_BattleSummonBahamutZeroSparkState->matrix);
    SetTransMatrix(&g_BattleSummonBahamutZeroSparkState->matrix);
    RefreshGlobalTransformState();
    prim = (BattleFxBzLivewireSwarmPrim *)BattleFx_BzEmitTilePrim();
    prim->color = 0x32808080;
    prim->vertexDeltas = (int)&g_BattleSummonOdinGVertexDeltasData2;
    prim->deltaData = (int)&g_BattleSummonOdinGVertexDeltasData2;
    depth = g_BattleSummonBahamutZeroSparkState->depth >> 2;
    if (depth > 0) {
        Gfx_DrawPsxPrimitive(prim);
    }
    if (g_BattleIsPaused == 0) {
        (recordPtr->frame)++;
        recordPtr->phase += recordPtr->phaseStep;
    }
}

/*
 * The three constants are 8 bytes apart and hold
 * (0,0,0,0) (-50,400,0,0) (50,400,0,0)
 * as shorts -- three SVECTORs, a triangle. `junk` is the
 * dword at [ebp-0xC] that nothing reads; its spelling, and
 * `block`, are what put the other three in their slots.
 */
// FUNCTION: FF7 0x0048723a
void *__cdecl BattleFx_BzEmitTilePrim(void)
{
    int outX;
    int junk;
    int outY;
    int *block;

    block = (int *)Gfx_GetPsxPacketScratchPtr();
    Gfx_TransformTriangleReturnDepthAlt((void *)BattleFx_BzEmitTilePrimVertex0, (SVECTOR *)g_BattleFxBzEmitTilePrimVertex1, (SVECTOR *)&g_BattleFxBzEmitTilePrimVertex2, &outX, &outY);
    *block = 0x6000000;
    return block;
}

/*
 * Animates and submits the composite Tera Flare blast-dome models in resource slots 12, 13, and 16.
 */
// FUNCTION: FF7 0x00487277
void __cdecl BattleFx_BzDrawSlot0CTo10(void)
{
    int frameCount;
    BattleTaskSlot *task;
    int scaledProgress;
    int ratio;

    task = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleFxBzDrawSlot0CTo10Init & 1) == 0) {
        g_BattleFxBzDrawSlot0CTo10Init |= 1;
        g_pBattleBahamutZeroSlot0CTo10State = (BattleBahamutZeroState *)Gfx_GetPsxScratchpadPtr(0);
    }
    (g_BattleFxBzDrawRequest0C.modelGroup) = Gfx_ModelTableGetRenderObject(12, g_pBattleBahamutZeroTableCursor);
    (g_BattleFxBzDrawRequest0D.modelGroup) = Gfx_ModelTableGetRenderObject(13, g_pBattleBahamutZeroTableCursor);
    g_BattleFxBzDrawRequest10[0] = Gfx_ModelTableGetRenderObject(16, g_pBattleBahamutZeroTableCursor);
    frameCount = task->frameCounter;
    if (frameCount < 30) {
        ratio = (frameCount << 12) / 30;
        scaledProgress = ratio * 0x23B5 >> 12;
    } else {
        frameCount -= 30;
        if (frameCount < 40) {
            scaledProgress = 0x23B5;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    g_BattleFxBzDrawRequest0D.fadeLevel = ((task->frameCounter & 1) != 0 ? -128 : 0) + 128;
    g_BattleFxBzDrawRequest0C.fadeLevel = g_BattleFxBzDrawRequest0D.fadeLevel;
    SetDrawColorRGB(0, 0, 0);
    if (task->frameCounter < 15) {
        RotMatrix((SVECTOR *)&g_BattleSummonBahamutZeroSlot0CTo10ZeroAngles, &g_pBattleBahamutZeroSlot0CTo10State->matrix);
        g_pBattleBahamutZeroSlot0CTo10State->matrix.t[0] = 0;
        g_pBattleBahamutZeroSlot0CTo10State->matrix.t[1] = 0xFFFFFFCE;
        g_pBattleBahamutZeroSlot0CTo10State->matrix.t[2] = 0xFFFFFC4A;
        g_pBattleBahamutZeroSlot0CTo10State->offsetY = 0x900;
        g_pBattleBahamutZeroSlot0CTo10State->offsetX = 0x900;
        g_pBattleBahamutZeroSlot0CTo10State->progress = (((scaledProgress << 12) / 5103 << 12) / 512) * 0xC00 /* 0.75 */ >> 12;
        ScaleMatrix(&g_pBattleBahamutZeroSlot0CTo10State->matrix, &g_pBattleBahamutZeroSlot0CTo10State->offsetX);
        Battle3d_GetActorAttachMatrix(3, 6, (PsxMatrix *)&g_BattleActorRecords[3].boneNodes5[0x34]);
        MultiplyMatrix(&g_BattleActorRecords[3].boneNodes5[0x34], &g_pBattleBahamutZeroSlot0CTo10State->matrix, &g_pBattleBahamutZeroSlot0CTo10State->drawMatrix);
        SetRotMatrix(&g_pBattleBahamutZeroSlot0CTo10State->drawMatrix);
        SetTransMatrix(&g_pBattleBahamutZeroSlot0CTo10State->drawMatrix);
        g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleFxBzDrawRequest0C, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    } else {
        RotMatrix((SVECTOR *)&g_BattleSummonBahamutZeroQuadSize, &g_pBattleBahamutZeroSlot0CTo10State->matrix);
        g_pBattleBahamutZeroSlot0CTo10State->matrix.t[0] = 0;
        g_pBattleBahamutZeroSlot0CTo10State->matrix.t[1] = 0xFFFFD8F0;
        g_pBattleBahamutZeroSlot0CTo10State->matrix.t[2] = 0xFFFFD8F0;
        g_pBattleBahamutZeroSlot0CTo10State->offsetY = 0x180;
        g_pBattleBahamutZeroSlot0CTo10State->offsetX = 0x180;
        g_pBattleBahamutZeroSlot0CTo10State->progress = (scaledProgress << 12) / 5103;
        ScaleMatrix(&g_pBattleBahamutZeroSlot0CTo10State->matrix, &g_pBattleBahamutZeroSlot0CTo10State->offsetX);
        MultiplyMatrix(&g_BattleBahamutZeroComposedMatrix, &g_pBattleBahamutZeroSlot0CTo10State->matrix, &g_pBattleBahamutZeroSlot0CTo10State->drawMatrix);
        SetRotMatrix(&g_pBattleBahamutZeroSlot0CTo10State->drawMatrix);
        SetTransMatrix(&g_pBattleBahamutZeroSlot0CTo10State->drawMatrix);
        g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleFxBzDrawRequest0C, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
        if (task->frameCounter < 30) {
            RotMatrix((SVECTOR *)&g_BattleSummonBahamutZeroQuadSize, &g_pBattleBahamutZeroSlot0CTo10State->matrix);
            g_BattleSummonBahamutZeroSlot0CTo10ScaleTail.bahamutZeroSlot0CTo10Depth = -(scaledProgress - 0x27D);
            MultiplyMatrix(&g_pBattleBahamutZeroSlot0CTo10State->matrix, &g_BattleSummonBahamutZeroSlot0CTo10ScaleMatrix, &g_pBattleBahamutZeroSlot0CTo10State->drawMatrix);
            MultiplyMatrix(&g_BattleBahamutZeroComposedMatrix, &g_pBattleBahamutZeroSlot0CTo10State->drawMatrix, &g_pBattleBahamutZeroSlot0CTo10State->drawMatrix);
            SetRotMatrix(&g_pBattleBahamutZeroSlot0CTo10State->drawMatrix);
            SetTransMatrix(&g_pBattleBahamutZeroSlot0CTo10State->drawMatrix);
            g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleFxBzDrawRequest0D, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
        } else {
            frameCount = task->frameCounter - 30;
            ratio = (frameCount << 12) / 40;
            ratio = BattleFx_BzEaseFixed(ratio, 0x2FF);
            RotMatrix((SVECTOR *)&g_BattleSummonBahamutZeroSlot0CTo10ZeroAngles, &g_pBattleBahamutZeroSlot0CTo10State->drawMatrix);
            g_pBattleBahamutZeroSlot0CTo10State->progress = ratio << 5 >> 12;
            g_pBattleBahamutZeroSlot0CTo10State->offsetY = g_pBattleBahamutZeroSlot0CTo10State->progress;
            g_pBattleBahamutZeroSlot0CTo10State->offsetX = g_pBattleBahamutZeroSlot0CTo10State->offsetY;
            ScaleMatrix(&g_pBattleBahamutZeroSlot0CTo10State->drawMatrix, &g_pBattleBahamutZeroSlot0CTo10State->offsetX);
            g_pBattleBahamutZeroSlot0CTo10State->vectorX = (((BattleSummonBahamutZeroSlot0CTo10Angle *)&g_BattleSummonBahamutZeroQuadSize)->angleX);
            g_pBattleBahamutZeroSlot0CTo10State->stateWord = g_BattleBahamutZeroSlot0CTo10StateWord;
            g_pBattleBahamutZeroSlot0CTo10State->timer = 0;
            RotMatrix((SVECTOR *)g_pBattleBahamutZeroSlot0CTo10State, &g_pBattleBahamutZeroSlot0CTo10State->matrix);
            MultiplyMatrix(&g_BattleBahamutZeroComposedMatrix, &g_pBattleBahamutZeroSlot0CTo10State->matrix, &g_pBattleBahamutZeroSlot0CTo10State->matrix);
            g_pBattleBahamutZeroSlot0CTo10State->stateWord = 0;
            g_pBattleBahamutZeroSlot0CTo10State->vectorX = 0;
            g_pBattleBahamutZeroSlot0CTo10State->timer = -scaledProgress + 300;
            SetRotMatrix(&g_pBattleBahamutZeroSlot0CTo10State->matrix);
            SetTransMatrix(&g_pBattleBahamutZeroSlot0CTo10State->matrix);
            RotTransPoint((SVECTOR *)g_pBattleBahamutZeroSlot0CTo10State, g_pBattleBahamutZeroSlot0CTo10State->drawMatrix.t, &g_pBattleBahamutZeroSlot0CTo10State->drawFlag);
            SetRotMatrix(&g_pBattleBahamutZeroSlot0CTo10State->drawMatrix);
            SetTransMatrix(&g_pBattleBahamutZeroSlot0CTo10State->drawMatrix);
            g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)g_BattleFxBzDrawRequest10, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
        }
    }
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
    }
}

/*
 * Draws Bahamut ZERO's flickering blast-pillar model in resource slot 14.
 */
// FUNCTION: FF7 0x00487842
void __cdecl BattleFx_BzDrawSlot0E(void)
{
    int frameCount;
    BattleTaskSlot *task;
    int ratio;

    task = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleFxBzDrawSlot0EInit & 1) == 0) {
        g_BattleFxBzDrawSlot0EInit |= 1;
        g_BattleSummonBahamutZeroSlot0EState = (BattleFxBzModelBlock *)Gfx_GetPsxScratchpadPtr(0);
    }
    (g_BattleSummonBahamutZeroSlot0ERandom.modelGroup) = Gfx_ModelTableGetRenderObject(14, g_pBattleBahamutZeroTableCursor);
    frameCount = task->frameCounter;
    if (frameCount < 5) {
        g_BattleSummonBahamutZeroSlot0ERandom.bahamutZeroSlot0ERandomPhase = 0;
    } else {
        frameCount -= 5;
        if (frameCount < 25) {
            ratio = (frameCount << 12) / 25;
            g_BattleSummonBahamutZeroSlot0ERandom.bahamutZeroSlot0ERandomPhase = ratio;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    ratio = (task->frameCounter << 12) / 30;
    g_BattleSummonBahamutZeroSlot0EState->scaleZ = ratio * 0x1800 /* 1.5 */ >> 12;
    g_BattleSummonBahamutZeroSlot0EState->scaleY = g_BattleSummonBahamutZeroSlot0EState->scaleZ;
    g_BattleSummonBahamutZeroSlot0EState->scaleX = g_BattleSummonBahamutZeroSlot0EState->scaleY;
    RotMatrix((SVECTOR *)g_BattleSummonBahamutZeroSlot0EAngles, &g_BattleSummonBahamutZeroSlot0EState->matrix);
    g_BattleSummonBahamutZeroSlot0EState->matrix.t[0] = 0;
    g_BattleSummonBahamutZeroSlot0EState->matrix.t[1] = 0xFFFFF230;
    g_BattleSummonBahamutZeroSlot0EState->matrix.t[2] = 0xFFFFF230;
    ScaleMatrix(&g_BattleSummonBahamutZeroSlot0EState->matrix, g_BattleSummonBahamutZeroSlot0EState);
    MultiplyMatrix(&g_BattleBahamutZeroComposedMatrix, &g_BattleSummonBahamutZeroSlot0EState->matrix, &g_BattleSummonBahamutZeroSlot0EState->matrix);
    SetRotMatrix(&g_BattleSummonBahamutZeroSlot0EState->matrix);
    SetTransMatrix(&g_BattleSummonBahamutZeroSlot0EState->matrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonBahamutZeroSlot0ERandom, g_BattleCurrentOrderingTable + 0x4054, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
    }
}

/*
 * Stretches along Z and draws Bahamut ZERO's beam-core cylinder model in resource slot 15.
 */
// FUNCTION: FF7 0x00487a2c
void __cdecl BattleFx_BzDrawSlot0F(void)
{
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleTaskSlot *task;
    int ratio;

    task = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleFxBzDrawSlot0FInit & 1) == 0) {
        g_BattleFxBzDrawSlot0FInit |= 1;
        g_BattleSummonBahamutZeroSlot0FState = (BattleFxBzModelBlock *)Gfx_GetPsxScratchpadPtr(0);
    }
    g_BattleBahamutZeroSlot0FDrawHandle = Gfx_ModelTableGetRenderObject(15, g_pBattleBahamutZeroTableCursor);
    ratio = (task->frameCounter << 12) / 40;
    g_BattleSummonBahamutZeroSlot0FState->scaleZ = ratio << 13 >> 12;
    g_BattleSummonBahamutZeroSlot0FState->scaleY = g_BattleSummonBahamutZeroSlot0FState->scaleZ;
    g_BattleSummonBahamutZeroSlot0FState->scaleX = g_BattleSummonBahamutZeroSlot0FState->scaleY;
    RotMatrix(&g_BattleSummonBahamutZeroSlot0FAngles, &g_BattleSummonBahamutZeroSlot0FState->matrix);
    g_BattleSummonBahamutZeroSlot0FState->matrix.t[0] = 0;
    g_BattleSummonBahamutZeroSlot0FState->matrix.t[1] = 0xFFFFF230;
    g_BattleSummonBahamutZeroSlot0FState->matrix.t[2] = 0xFFFFF230;
    ScaleMatrix(&g_BattleSummonBahamutZeroSlot0FState->matrix, g_BattleSummonBahamutZeroSlot0FState);
    MultiplyMatrix(&g_BattleBahamutZeroComposedMatrix, &g_BattleSummonBahamutZeroSlot0FState->matrix, &g_BattleSummonBahamutZeroSlot0FState->matrix);
    SetRotMatrix(&g_BattleSummonBahamutZeroSlot0FState->matrix);
    SetTransMatrix(&g_BattleSummonBahamutZeroSlot0FState->matrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleBahamutZeroSlot0FDrawHandle, g_BattleCurrentOrderingTable + 0x4058, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= 40) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Emits a full-viewport additive flash primitive over the battle screen for Bahamut ZERO.
 */
// FUNCTION: FF7 0x00487bd2
void __cdecl BattleFx_BzEmitScreenPrim(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSummonBahamutZeroScreenPrim.bahamutZeroScreenPrimOriginX = *(short *)&g_BattleViewportOriginX;
    g_BattleSummonBahamutZeroScreenPrim.bahamutZeroScreenPrimOriginY = *(short *)&g_BattleViewportOriginY;
    (g_BattleSummonBahamutZeroScreenSize.bahamutZeroScreenPrimWidth) = *(short *)&g_BattleViewportWidth;
    g_BattleSummonBahamutZeroScreenSize.bahamutZeroScreenPrimHeight = *(short *)&g_BattleSceneViewportHeight;
    Gfx_SetBlendFactorPair(FLOAT_0_9999_BITS, FLOAT_ONE_BITS);
    Gfx_DrawPsxPrimitive(&g_BattleSummonBahamutZeroScreenPrim);
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= 70) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws and morphs Bahamut ZERO's starry space sky-dome LMD meshes across the summon sequence.
 */
// FUNCTION: FF7 0x00487c73
void __cdecl BattleFx_BzDrawSkyDomeTask(void)
{
    int scale;
    int frameCount;
    BattleTaskSlot *task;
    int ratio;
    int blend;

    task = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleFxBzDrawSkyDomeTaskInit & 1) == 0) {
        g_BattleFxBzDrawSkyDomeTaskInit |= 1;
        g_pBattleBahamutZeroSkyDomeState = (struct BattleFxBahamutZeroState *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (task->frameCounter == 0) {
        (g_BattleSummonBahamutZeroSkyLmdPtrs.skyALmdPointer) = (int)g_pBattleBahamutZeroSkyALmd;
        g_BattleSummonBahamutZeroSkyLmdPtrs.bahamutZeroSkyDomeLmdPointer = (int)g_pBattleBahamutZeroSkyDomeLmd;
        g_BattleSummonBahamutZeroOuterMorphVerts = (int)g_pBattleBahamutZeroSkyBPly;
        g_BattleSummonBahamutZeroInnerMorphVerts = (int)g_pBattleBahamutZeroSkyDPly;
    }
    frameCount = task->frameCounter;
    if (frameCount < 40) {
        ratio = (frameCount << 12) / 40;
        g_BattleSummonBahamutZeroMatrixPair.matrixB.t[0] = (ratio * 0x28 >> 12) - 0x28;
        g_BattleSummonBahamutZeroMatrixPair.matrixB.t[1] = (ratio * -0x28 >> 12) - 0x228;
        g_BattleSummonBahamutZeroMatrixPair.matrixB.t[2] = ((ratio * 0) >> 12) + 0x1000;
        g_BattleSummonBahamutZeroMatrixPair.matrixA.t[0] = (ratio * 0x28 >> 12) - 0x28;
        g_BattleSummonBahamutZeroMatrixPair.matrixA.t[1] = (ratio * -0x28 >> 12) - 0x228;
        g_BattleSummonBahamutZeroMatrixPair.matrixA.t[2] = ((ratio * 0) >> 12) + 0x1000;
        blend = 0;
        scale = 0x1000 /* 1.0 */;
    } else {
        frameCount -= 40;
        if (frameCount < 15) {
            if (frameCount == 0) {
                g_BattleSummonBahamutZeroMatrixPair.matrixA.t[0] = 0;
                g_BattleSummonBahamutZeroMatrixPair.matrixB.t[0] = g_BattleSummonBahamutZeroMatrixPair.matrixA.t[0];
                g_BattleSummonBahamutZeroMatrixPair.matrixA.t[1] = 0xFFFFFDB0;
                g_BattleSummonBahamutZeroMatrixPair.matrixB.t[1] = g_BattleSummonBahamutZeroMatrixPair.matrixA.t[1];
                g_BattleSummonBahamutZeroMatrixPair.matrixA.t[2] = 0x1000;
                g_BattleSummonBahamutZeroMatrixPair.matrixB.t[2] = g_BattleSummonBahamutZeroMatrixPair.matrixA.t[2];
            }
            ratio = (frameCount << 12) / 15;
            scale = (ratio * 0x600 /* 0.375 */ >> 12) + 0x1000;
            blend = BattleFx_BzEaseFixed(ratio, 0xFFFFFD10);
            (g_BattleBahamutZeroMatrixBAngles.matrixBState) = (blend << 11 >> 12) + 0x400;
            blend = BattleFx_BzEaseFixed(ratio, 0xFFFFFD10);
            (g_BattleBahamutZeroMatrixAAngles.matrixAState) = (blend * 0x600 /* 0.375 */ >> 12) + 0x400;
            blend = BattleFx_BzEaseFixed(ratio, 0x3FF);
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    g_pBattleBahamutZeroSkyDomeState->skyLayerC = scale;
    g_pBattleBahamutZeroSkyDomeState->skyLayerB = scale;
    g_pBattleBahamutZeroSkyDomeState->skyLayerA = scale;
    RotMatrix((SVECTOR *)&g_BattleBahamutZeroMatrixBAngles, &g_BattleSummonBahamutZeroMatrixPair.matrixB);
    ScaleMatrix(&g_BattleSummonBahamutZeroMatrixPair.matrixB, g_pBattleBahamutZeroSkyDomeState);
    SetRotMatrix(&g_BattleSummonBahamutZeroMatrixPair.matrixB);
    SetTransMatrix(&g_BattleSummonBahamutZeroMatrixPair.matrixB);
    RefreshGlobalTransformState();
    Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(23, g_pBattleBahamutZeroTableCursor)));
    Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(24, g_pBattleBahamutZeroTableCursor)));
    g_BattleOverlayDrawListHead = (int)BattleFx_DrawMorphedLmdOuter((BattleFxBzMorphDesc *)&g_BattleSummonBahamutZeroSkyLmdPtrs, (void *)(g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST), 0, (void *)g_BattleOverlayDrawListHead, (void *)g_BattleSummonBahamutZeroOuterMorphVerts, blend);
    g_pBattleBahamutZeroSkyDomeState->skyLayerC = scale;
    g_pBattleBahamutZeroSkyDomeState->skyLayerB = scale;
    g_pBattleBahamutZeroSkyDomeState->skyLayerA = scale;
    RotMatrix((SVECTOR *)&g_BattleBahamutZeroMatrixAAngles, &g_BattleSummonBahamutZeroMatrixPair.matrixA);
    ScaleMatrix(&g_BattleSummonBahamutZeroMatrixPair.matrixA, g_pBattleBahamutZeroSkyDomeState);
    SetRotMatrix(&g_BattleSummonBahamutZeroMatrixPair.matrixA);
    SetTransMatrix(&g_BattleSummonBahamutZeroMatrixPair.matrixA);
    RefreshGlobalTransformState();
    Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(25, g_pBattleBahamutZeroTableCursor)));
    Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(26, g_pBattleBahamutZeroTableCursor)));
    g_BattleOverlayDrawListHead = (int)BattleFx_DrawMorphedLmdInner((BattleFxBzMorphDesc *)&g_BattleSummonBahamutZeroSkyLmdPtrs.bahamutZeroSkyDomeLmdPointer, (void *)(g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST), 0, (void *)g_BattleOverlayDrawListHead, (void *)g_BattleSummonBahamutZeroInnerMorphVerts, blend);
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
    }
}

/*
 * Interpolates outer LMD mesh vertices toward morphVertexBase by Q12 morphT4096 and submits polygons.
 */
// FUNCTION: FF7 0x00488055
void *__cdecl BattleFx_DrawMorphedLmdOuter(BattleFxBzMorphDesc *desc, void *unusedOt, int unusedZero, void *unusedPrimCursor, void *morphVertexBase, int morphT4096)

{
    struct {
        int avgZ;
        GfxPsxTexturedQuad *pktQuad;
        int pad64;
        GfxPsxTexturedQuad *pktTri;
        int pad5c;
        int pad58;
        int faceIndex;
        int pad50;
        union { /* the face record, read as shorts or as 32-bit words */
            short *cursor;
            int *cursorWords;
        };
        int pad48;
        BattleFxPsxQuadPacket *pktFlatQuad;
        short *plyV3;
        int gteFlag;
        int windingSign;
        BattleFxPsxQuadPacket *pktFlatTri;
        short *plyV2;
        int packedColor;
        short *plyV1;
        short *lmdV3;
        short *plyV0;
        int faceCount;
        short *lmdV2;
        int lmdVertexBase;
        unsigned short tpageWord;
        short pad0e;
        short *lmdV1;
        int pad08;
        short *lmdV0;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if ((g_BattleFxDrawMorphedLmdOuterInit & 1) == 0) {
        g_BattleFxDrawMorphedLmdOuterInit |= 1;
        g_BattleSummonBahamutZeroMorphedOuter = (short *)Gfx_GetPsxScratchpadPtr(0);
    }
    local.lmdVertexBase = desc->lmd + 4;
    local.cursor = (short *)(local.lmdVertexBase + *(int *)desc->lmd);
    local.pktTri = (GfxPsxTexturedQuad *)Gfx_GetPsxPacketScratchPtr();
    local.faceCount = (int)*local.cursor;
    local.cursor = local.cursor + 1;
    local.tpageWord = *local.cursor & 0x19fU | desc->tpage;
    local.cursor = local.cursor + 1;
    local.lmdV0 = (short *)(local.lmdVertexBase + local.cursor[0]);
    local.lmdV1 = (short *)(local.lmdVertexBase + local.cursor[1]);
    local.lmdV2 = (short *)(local.lmdVertexBase + local.cursor[2]);
    local.plyV0 = (short *)((int)morphVertexBase + (int)local.cursor[0]);
    local.plyV1 = (short *)((int)morphVertexBase + (int)local.cursor[1]);
    local.plyV2 = (short *)((int)morphVertexBase + (int)local.cursor[2]);
    for (local.faceIndex = 0; local.faceIndex < local.faceCount; local.faceIndex = local.faceIndex + 1) {
        *g_BattleSummonBahamutZeroMorphedOuter = *local.lmdV0 + (*local.plyV0 * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[1] = local.lmdV0[1] + (local.plyV0[1] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[2] = local.lmdV0[2] + (local.plyV0[2] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[4] = *local.lmdV1 + (*local.plyV1 * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[5] = local.lmdV1[1] + (local.plyV1[1] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[6] = local.lmdV1[2] + (local.plyV1[2] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[8] = *local.lmdV2 + (*local.plyV2 * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[9] = local.lmdV2[1] + (local.plyV2[1] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[10] = local.lmdV2[2] + (local.plyV2[2] * morphT4096 >> 12);
        StoreDeferredDrawArgs(g_BattleSummonBahamutZeroMorphedOuter, g_BattleSummonBahamutZeroMorphedOuter + 4, g_BattleSummonBahamutZeroMorphedOuter + 8);
        ApplyDeferredDrawArgs();
        local.lmdV0 = (short *)(local.lmdVertexBase + local.cursor[8]);
        local.lmdV1 = (short *)(local.lmdVertexBase + local.cursor[9]);
        local.lmdV2 = (short *)(local.lmdVertexBase + local.cursor[10]);
        local.plyV0 = (short *)((int)morphVertexBase + (int)local.cursor[8]);
        local.plyV1 = (short *)((int)morphVertexBase + (int)local.cursor[9]);
        local.plyV2 = (short *)((int)morphVertexBase + (int)local.cursor[10]);
        Gfx_PsxStoreClearFlag(&local.gteFlag);
        if ((local.gteFlag & 0x60000) == 0) {
            if ((desc->flags & BATTLE_FX_BZ_MORPH_DOUBLE_SIDED) != 0) {
                local.windingSign = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&local.windingSign);
            }
            if (0 < (int)local.windingSign) {
                local.pktTri->tag = 0x7000000;
                Gfx_ReadTransformOrderingDepth2(&local.avgZ);
                local.pktTri->uv0Clut = local.cursorWords[2];
                local.pktTri->uv1 = local.cursor[6];
                local.pktTri->uv2 = local.cursor[7];
                local.pktTri->texturePage = local.tpageWord;
                if ((desc->flags & BATTLE_FX_BZ_MORPH_FACE_GREY) != 0) {
                    local.packedColor = (int)local.cursor[3] & 0xff;
                    local.packedColor = local.packedColor << 0x10 | 0x24000000 | local.packedColor << 8 | local.packedColor;
                    if (desc->shade != 0) {
                        Gfx_ShadePsxVertexColor((CVECTOR *)&local.packedColor, (int)desc->shade, &local.pktTri->colorVec);
                        local.pktTri->colorVec.cd = 0x24;
                    } else {
                        local.pktTri->color = local.packedColor;
                    }
                } else {
                    local.pktTri->color = local.packedColor << 0x10 | 0x24000000 | local.packedColor << 8 | local.packedColor;
                }
                if ((desc->flags & BATTLE_FX_BZ_MORPH_SEMI_TRANSPARENT) != 0) {
                    local.pktTri->colorVec.cd |= GFX_PSX_PRIM_CODE_SEMI_TRANSPARENT;
                }
            }
        }
        local.cursor = local.cursor + 8;
    }
    local.faceCount = (int)*local.cursor;
    local.cursor = local.cursor + 2;
    local.pktQuad = local.pktTri;
    local.lmdV0 = (short *)(local.lmdVertexBase + local.cursor[0]);
    local.lmdV1 = (short *)(local.lmdVertexBase + local.cursor[1]);
    local.lmdV2 = (short *)(local.lmdVertexBase + local.cursor[2]);
    local.plyV0 = (short *)((int)morphVertexBase + (int)local.cursor[0]);
    local.plyV1 = (short *)((int)morphVertexBase + (int)local.cursor[1]);
    local.plyV2 = (short *)((int)morphVertexBase + (int)local.cursor[2]);
    for (local.faceIndex = 0; local.faceIndex < local.faceCount; local.faceIndex = local.faceIndex + 1) {
        *g_BattleSummonBahamutZeroMorphedOuter = *local.lmdV0 + (*local.plyV0 * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[1] = local.lmdV0[1] + (local.plyV0[1] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[2] = local.lmdV0[2] + (local.plyV0[2] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[4] = *local.lmdV1 + (*local.plyV1 * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[5] = local.lmdV1[1] + (local.plyV1[1] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[6] = local.lmdV1[2] + (local.plyV1[2] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[8] = *local.lmdV2 + (*local.plyV2 * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[9] = local.lmdV2[1] + (local.plyV2[1] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[10] = local.lmdV2[2] + (local.plyV2[2] * morphT4096 >> 12);
        StoreDeferredDrawArgs(g_BattleSummonBahamutZeroMorphedOuter, g_BattleSummonBahamutZeroMorphedOuter + 4, g_BattleSummonBahamutZeroMorphedOuter + 8);
        ApplyDeferredDrawArgs();
        local.lmdV0 = (short *)(local.lmdVertexBase + local.cursor[10]);
        local.lmdV1 = (short *)(local.lmdVertexBase + local.cursor[0xb]);
        local.lmdV2 = (short *)(local.lmdVertexBase + local.cursor[0xc]);
        local.plyV0 = (short *)((int)morphVertexBase + (int)local.cursor[10]);
        local.plyV1 = (short *)((int)morphVertexBase + (int)local.cursor[0xb]);
        local.plyV2 = (short *)((int)morphVertexBase + (int)local.cursor[0xc]);
        Gfx_PsxStoreClearFlag(&local.gteFlag);
        if ((local.gteFlag & 0x60000) == 0) {
            if ((desc->flags & BATTLE_FX_BZ_MORPH_DOUBLE_SIDED) != 0) {
                local.windingSign = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&local.windingSign);
            }
            if (local.windingSign >= 0) {
                local.lmdV3 = (short *)(local.lmdVertexBase + local.cursor[3]);
                local.plyV3 = (short *)((int)morphVertexBase + (int)local.cursor[3]);
                g_BattleSummonBahamutZeroMorphedOuter[0xc] = *local.lmdV3 + (*local.plyV3 * morphT4096 >> 12);
                g_BattleSummonBahamutZeroMorphedOuter[0xd] = local.lmdV3[1] + (local.plyV3[1] * morphT4096 >> 12);
                g_BattleSummonBahamutZeroMorphedOuter[0xe] = local.lmdV3[2] + (local.plyV3[2] * morphT4096 >> 12);
                SetPendingResourceHandle((struct GfxPsxShortVertexSlot *)(g_BattleSummonBahamutZeroMorphedOuter + 0xc));
                ApplyPendingResourceHandle();
                Gfx_ReadTransformOrderingDepth2(&local.avgZ);
                local.pktQuad->tag = 0x9000000;
                if (local.faceIndex < local.faceCount + -0x16) {
                    local.pktQuad->uv0Clut = local.cursorWords[2];
                    local.pktQuad->uv1 = local.cursor[6];
                    local.pktQuad->uv2 = local.cursor[7];
                    local.pktQuad->uv3 = local.cursor[8];
                }
                local.pktQuad->texturePage = local.tpageWord;
                if ((desc->flags & BATTLE_FX_BZ_MORPH_FACE_GREY) != 0) {
                    local.packedColor = (int)local.cursor[9] & 0xff;
                    local.packedColor = local.packedColor << 0x10 | 0x2c000000 | local.packedColor << 8 | local.packedColor;
                    if (desc->shade != 0) {
                        Gfx_ShadePsxVertexColor((CVECTOR *)&local.packedColor, (int)desc->shade, &local.pktQuad->colorVec);
                        local.pktQuad->colorVec.cd = 0x2c;
                    } else {
                        local.pktQuad->color = local.packedColor;
                    }
                } else {
                    local.packedColor = (int)desc->shade & 0xff;
                    local.pktQuad->color = local.packedColor << 0x10 | 0x2c000000 | local.packedColor << 8 | local.packedColor;
                }
                if ((desc->flags & BATTLE_FX_BZ_MORPH_SEMI_TRANSPARENT) != 0) {
                    local.pktQuad->colorVec.cd |= GFX_PSX_PRIM_CODE_SEMI_TRANSPARENT;
                }
                Gfx_DrawPsxPrimitive(local.pktQuad);
            }
        }
        local.cursor = local.cursor + 10;
    }
    local.faceCount = (int)*local.cursor;
    local.cursor = local.cursor + 2;
    local.pktFlatTri = (BattleFxPsxQuadPacket *)local.pktQuad;
    local.lmdV0 = (short *)(local.lmdVertexBase + local.cursor[0]);
    local.lmdV1 = (short *)(local.lmdVertexBase + local.cursor[1]);
    local.lmdV2 = (short *)(local.lmdVertexBase + local.cursor[2]);
    local.plyV0 = (short *)((int)morphVertexBase + (int)local.cursor[0]);
    local.plyV1 = (short *)((int)morphVertexBase + (int)local.cursor[1]);
    local.plyV2 = (short *)((int)morphVertexBase + (int)local.cursor[2]);
    for (local.faceIndex = 0; local.faceIndex < local.faceCount; local.faceIndex = local.faceIndex + 1) {
        *g_BattleSummonBahamutZeroMorphedOuter = *local.lmdV0 + (*local.plyV0 * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[1] = local.lmdV0[1] + (local.plyV0[1] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[2] = local.lmdV0[2] + (local.plyV0[2] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[4] = *local.lmdV1 + (*local.plyV1 * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[5] = local.lmdV1[1] + (local.plyV1[1] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[6] = local.lmdV1[2] + (local.plyV1[2] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[8] = *local.lmdV2 + (*local.plyV2 * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[9] = local.lmdV2[1] + (local.plyV2[1] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[10] = local.lmdV2[2] + (local.plyV2[2] * morphT4096 >> 12);
        StoreDeferredDrawArgs(g_BattleSummonBahamutZeroMorphedOuter, g_BattleSummonBahamutZeroMorphedOuter + 4, g_BattleSummonBahamutZeroMorphedOuter + 8);
        ApplyDeferredDrawArgs();
        local.lmdV0 = (short *)(local.lmdVertexBase + local.cursor[10]);
        local.lmdV1 = (short *)(local.lmdVertexBase + local.cursor[0xb]);
        local.lmdV2 = (short *)(local.lmdVertexBase + local.cursor[0xc]);
        local.plyV0 = (short *)((int)morphVertexBase + (int)local.cursor[10]);
        local.plyV1 = (short *)((int)morphVertexBase + (int)local.cursor[0xb]);
        local.plyV2 = (short *)((int)morphVertexBase + (int)local.cursor[0xc]);
        Gfx_PsxStoreClearFlag(&local.gteFlag);
        if (((local.gteFlag & 0x60000) == 0) && (Gfx_PsxStoreFrontFacing(&local.windingSign), 0 < (int)local.windingSign)) {
            local.pktFlatTri->tag = 0x6000000;
            Gfx_ReadTransformOrderingDepth2(&local.avgZ);
            local.pktFlatTri->primWord = local.cursorWords[2];
            local.pktFlatTri->color1 = local.cursorWords[3];
            local.pktFlatTri->color2 = local.cursorWords[4];
            if ((desc->flags & BATTLE_FX_BZ_MORPH_SEMI_TRANSPARENT) != 0) {
                local.pktFlatTri->colorVec.cd |= GFX_PSX_PRIM_CODE_SEMI_TRANSPARENT;
            }
        }
        local.cursor = local.cursor + 10;
    }
    local.faceCount = (int)*local.cursor;
    local.cursor = local.cursor + 2;
    local.pktFlatQuad = local.pktFlatTri;
    local.lmdV0 = (short *)(local.lmdVertexBase + local.cursor[0]);
    local.lmdV1 = (short *)(local.lmdVertexBase + local.cursor[1]);
    local.lmdV2 = (short *)(local.lmdVertexBase + local.cursor[2]);
    local.plyV0 = (short *)((int)morphVertexBase + (int)local.cursor[0]);
    local.plyV1 = (short *)((int)morphVertexBase + (int)local.cursor[1]);
    local.plyV2 = (short *)((int)morphVertexBase + (int)local.cursor[2]);
    for (local.faceIndex = 0; local.faceIndex < local.faceCount; local.faceIndex = local.faceIndex + 1) {
        *g_BattleSummonBahamutZeroMorphedOuter = *local.lmdV0 + (*local.plyV0 * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[1] = local.lmdV0[1] + (local.plyV0[1] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[2] = local.lmdV0[2] + (local.plyV0[2] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[4] = *local.lmdV1 + (*local.plyV1 * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[5] = local.lmdV1[1] + (local.plyV1[1] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[6] = local.lmdV1[2] + (local.plyV1[2] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[8] = *local.lmdV2 + (*local.plyV2 * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[9] = local.lmdV2[1] + (local.plyV2[1] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedOuter[10] = local.lmdV2[2] + (local.plyV2[2] * morphT4096 >> 12);
        StoreDeferredDrawArgs(g_BattleSummonBahamutZeroMorphedOuter, g_BattleSummonBahamutZeroMorphedOuter + 4, g_BattleSummonBahamutZeroMorphedOuter + 8);
        ApplyDeferredDrawArgs();
        local.lmdV0 = (short *)(local.lmdVertexBase + local.cursor[0xc]);
        local.lmdV1 = (short *)(local.lmdVertexBase + local.cursor[0xd]);
        local.lmdV2 = (short *)(local.lmdVertexBase + local.cursor[0xe]);
        local.plyV0 = (short *)((int)morphVertexBase + (int)local.cursor[0xc]);
        local.plyV1 = (short *)((int)morphVertexBase + (int)local.cursor[0xd]);
        local.plyV2 = (short *)((int)morphVertexBase + (int)local.cursor[0xe]);
        Gfx_PsxStoreClearFlag(&local.gteFlag);
        if (((local.gteFlag & 0x60000) == 0) &&
            (Gfx_PsxStoreFrontFacing(&local.windingSign), local.windingSign >= 0)) {
            local.lmdV3 = (short *)(local.lmdVertexBase + local.cursor[3]);
            local.plyV3 = (short *)((int)morphVertexBase + (int)local.cursor[3]);
            g_BattleSummonBahamutZeroMorphedOuter[0xc] = *local.lmdV3 + (*local.plyV3 * morphT4096 >> 12);
            g_BattleSummonBahamutZeroMorphedOuter[0xd] = local.lmdV3[1] + (local.plyV3[1] * morphT4096 >> 12);
            g_BattleSummonBahamutZeroMorphedOuter[0xe] = local.lmdV3[2] + (local.plyV3[2] * morphT4096 >> 12);
            SetPendingResourceHandle((struct GfxPsxShortVertexSlot *)(g_BattleSummonBahamutZeroMorphedOuter + 0xc));
            ApplyPendingResourceHandle();
            Gfx_ReadTransformOrderingDepth2(&local.avgZ);
            local.pktFlatQuad->tag = 0x8000000;
            local.pktFlatQuad->primWord = local.cursorWords[2];
            local.pktFlatQuad->color1 = local.cursorWords[3];
            local.pktFlatQuad->color2 = local.cursorWords[4];
            local.pktFlatQuad->color3 = local.cursorWords[5];
            if ((desc->flags & BATTLE_FX_BZ_MORPH_SEMI_TRANSPARENT) != 0) {
                local.pktFlatQuad->colorVec.cd |= GFX_PSX_PRIM_CODE_SEMI_TRANSPARENT;
            }
        }
        local.cursor = local.cursor + 0xc;
    }
    return local.pktFlatQuad;
}

/*
 * Interpolates inner LMD mesh vertices toward morphVertexBase by Q12 morphT4096 and submits polygons.
 */
// FUNCTION: FF7 0x00488ea2
void *__cdecl BattleFx_DrawMorphedLmdInner(BattleFxBzMorphDesc *desc, void *unusedOt, int unusedZero, void *unusedPrimCursor, void *morphVertexBase, int morphT4096)

{
    struct {
        int avgZ;
        GfxPsxTexturedQuad *pktQuad;
        int pad64;
        GfxPsxTexturedQuad *pktTri;
        int pad5c;
        int pad58;
        int faceIndex;
        int pad50;
        union { /* the face record, read as shorts or as 32-bit words */
            short *cursor;
            int *cursorWords;
        };
        int pad48;
        BattleFxPsxQuadPacket *pktFlatQuad;
        short *plyV3;
        int gteFlag;
        int windingSign;
        BattleFxPsxQuadPacket *pktFlatTri;
        short *plyV2;
        int packedColor;
        short *plyV1;
        short *lmdV3;
        short *plyV0;
        int faceCount;
        short *lmdV2;
        int lmdVertexBase;
        unsigned short tpageWord;
        short pad0e;
        short *lmdV1;
        int pad08;
        short *lmdV0;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if ((g_BattleFxDrawMorphedLmdInnerInit & 1) == 0) {
        g_BattleFxDrawMorphedLmdInnerInit |= 1;
        g_BattleSummonBahamutZeroMorphedInner = (short *)Gfx_GetPsxScratchpadPtr(0);
    }
    local.lmdVertexBase = desc->lmd + 4;
    local.cursor = (short *)(local.lmdVertexBase + *(int *)desc->lmd);
    local.pktTri = (GfxPsxTexturedQuad *)Gfx_GetPsxPacketScratchPtr();
    local.faceCount = (int)*local.cursor;
    local.cursor = local.cursor + 1;
    local.tpageWord = *local.cursor & 0x19fU | desc->tpage;
    local.cursor = local.cursor + 1;
    local.lmdV0 = (short *)(local.lmdVertexBase + local.cursor[0]);
    local.lmdV1 = (short *)(local.lmdVertexBase + local.cursor[1]);
    local.lmdV2 = (short *)(local.lmdVertexBase + local.cursor[2]);
    local.plyV0 = (short *)((int)morphVertexBase + (int)local.cursor[0]);
    local.plyV1 = (short *)((int)morphVertexBase + (int)local.cursor[1]);
    local.plyV2 = (short *)((int)morphVertexBase + (int)local.cursor[2]);
    for (local.faceIndex = 0; local.faceIndex < local.faceCount; local.faceIndex = local.faceIndex + 1) {
        *g_BattleSummonBahamutZeroMorphedInner = *local.lmdV0 + (*local.plyV0 * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[1] = local.lmdV0[1] + (local.plyV0[1] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[2] = local.lmdV0[2] + (local.plyV0[2] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[4] = *local.lmdV1 + (*local.plyV1 * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[5] = local.lmdV1[1] + (local.plyV1[1] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[6] = local.lmdV1[2] + (local.plyV1[2] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[8] = *local.lmdV2 + (*local.plyV2 * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[9] = local.lmdV2[1] + (local.plyV2[1] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[10] = local.lmdV2[2] + (local.plyV2[2] * morphT4096 >> 12);
        StoreDeferredDrawArgs(g_BattleSummonBahamutZeroMorphedInner, g_BattleSummonBahamutZeroMorphedInner + 4, g_BattleSummonBahamutZeroMorphedInner + 8);
        ApplyDeferredDrawArgs();
        local.lmdV0 = (short *)(local.lmdVertexBase + local.cursor[8]);
        local.lmdV1 = (short *)(local.lmdVertexBase + local.cursor[9]);
        local.lmdV2 = (short *)(local.lmdVertexBase + local.cursor[10]);
        local.plyV0 = (short *)((int)morphVertexBase + (int)local.cursor[8]);
        local.plyV1 = (short *)((int)morphVertexBase + (int)local.cursor[9]);
        local.plyV2 = (short *)((int)morphVertexBase + (int)local.cursor[10]);
        Gfx_PsxStoreClearFlag(&local.gteFlag);
        if ((local.gteFlag & 0x60000) == 0) {
            if ((desc->flags & BATTLE_FX_BZ_MORPH_DOUBLE_SIDED) != 0) {
                local.windingSign = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&local.windingSign);
            }
            if (0 < (int)local.windingSign) {
                local.pktTri->tag = 0x7000000;
                Gfx_ReadTransformOrderingDepth2(&local.avgZ);
                local.pktTri->uv0Clut = local.cursorWords[2];
                local.pktTri->uv1 = local.cursor[6];
                local.pktTri->uv2 = local.cursor[7];
                local.pktTri->texturePage = local.tpageWord;
                if ((desc->flags & BATTLE_FX_BZ_MORPH_FACE_GREY) != 0) {
                    local.packedColor = (int)local.cursor[3] & 0xff;
                    local.packedColor = local.packedColor << 0x10 | 0x24000000 | local.packedColor << 8 | local.packedColor;
                    if (desc->shade != 0) {
                        Gfx_ShadePsxVertexColor((CVECTOR *)&local.packedColor, (int)desc->shade, &local.pktTri->colorVec);
                        local.pktTri->colorVec.cd = 0x24;
                    } else {
                        local.pktTri->color = local.packedColor;
                    }
                } else {
                    local.pktTri->color = local.packedColor << 0x10 | 0x24000000 | local.packedColor << 8 | local.packedColor;
                }
                if ((desc->flags & BATTLE_FX_BZ_MORPH_SEMI_TRANSPARENT) != 0) {
                    local.pktTri->colorVec.cd |= GFX_PSX_PRIM_CODE_SEMI_TRANSPARENT;
                }
            }
        }
        local.cursor = local.cursor + 8;
    }
    local.faceCount = (int)*local.cursor;
    local.cursor = local.cursor + 2;
    local.pktQuad = local.pktTri;
    local.lmdV0 = (short *)(local.lmdVertexBase + local.cursor[0]);
    local.lmdV1 = (short *)(local.lmdVertexBase + local.cursor[1]);
    local.lmdV2 = (short *)(local.lmdVertexBase + local.cursor[2]);
    local.plyV0 = (short *)((int)morphVertexBase + (int)local.cursor[0]);
    local.plyV1 = (short *)((int)morphVertexBase + (int)local.cursor[1]);
    local.plyV2 = (short *)((int)morphVertexBase + (int)local.cursor[2]);
    for (local.faceIndex = 0; local.faceIndex < local.faceCount; local.faceIndex = local.faceIndex + 1) {
        *g_BattleSummonBahamutZeroMorphedInner = *local.lmdV0 + (*local.plyV0 * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[1] = local.lmdV0[1] + (local.plyV0[1] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[2] = local.lmdV0[2] + (local.plyV0[2] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[4] = *local.lmdV1 + (*local.plyV1 * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[5] = local.lmdV1[1] + (local.plyV1[1] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[6] = local.lmdV1[2] + (local.plyV1[2] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[8] = *local.lmdV2 + (*local.plyV2 * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[9] = local.lmdV2[1] + (local.plyV2[1] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[10] = local.lmdV2[2] + (local.plyV2[2] * morphT4096 >> 12);
        StoreDeferredDrawArgs(g_BattleSummonBahamutZeroMorphedInner, g_BattleSummonBahamutZeroMorphedInner + 4, g_BattleSummonBahamutZeroMorphedInner + 8);
        ApplyDeferredDrawArgs();
        local.lmdV0 = (short *)(local.lmdVertexBase + local.cursor[10]);
        local.lmdV1 = (short *)(local.lmdVertexBase + local.cursor[0xb]);
        local.lmdV2 = (short *)(local.lmdVertexBase + local.cursor[0xc]);
        local.plyV0 = (short *)((int)morphVertexBase + (int)local.cursor[10]);
        local.plyV1 = (short *)((int)morphVertexBase + (int)local.cursor[0xb]);
        local.plyV2 = (short *)((int)morphVertexBase + (int)local.cursor[0xc]);
        Gfx_PsxStoreClearFlag(&local.gteFlag);
        if ((local.gteFlag & 0x60000) == 0) {
            if ((desc->flags & BATTLE_FX_BZ_MORPH_DOUBLE_SIDED) != 0) {
                local.windingSign = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&local.windingSign);
            }
            if (local.windingSign >= 0) {
                local.lmdV3 = (short *)(local.lmdVertexBase + local.cursor[3]);
                local.plyV3 = (short *)((int)morphVertexBase + (int)local.cursor[3]);
                g_BattleSummonBahamutZeroMorphedInner[0xc] = *local.lmdV3 + (*local.plyV3 * morphT4096 >> 12);
                g_BattleSummonBahamutZeroMorphedInner[0xd] = local.lmdV3[1] + (local.plyV3[1] * morphT4096 >> 12);
                g_BattleSummonBahamutZeroMorphedInner[0xe] = local.lmdV3[2] + (local.plyV3[2] * morphT4096 >> 12);
                SetPendingResourceHandle((struct GfxPsxShortVertexSlot *)(g_BattleSummonBahamutZeroMorphedInner + 0xc));
                ApplyPendingResourceHandle();
                Gfx_ReadTransformOrderingDepth2(&local.avgZ);
                local.pktQuad->tag = 0x9000000;
                local.pktQuad->uv0Clut = local.cursorWords[2];
                local.pktQuad->uv1 = local.cursor[6];
                local.pktQuad->uv2 = local.cursor[7];
                local.pktQuad->uv3 = local.cursor[8];
                local.pktQuad->texturePage = local.tpageWord;
                if ((desc->flags & BATTLE_FX_BZ_MORPH_FACE_GREY) != 0) {
                    local.packedColor = (int)local.cursor[9] & 0xff;
                    local.packedColor = local.packedColor << 0x10 | 0x2c000000 | local.packedColor << 8 | local.packedColor;
                    if (desc->shade != 0) {
                        Gfx_ShadePsxVertexColor((CVECTOR *)&local.packedColor, (int)desc->shade, &local.pktQuad->colorVec);
                        local.pktQuad->colorVec.cd = 0x2c;
                    } else {
                        local.pktQuad->color = local.packedColor;
                    }
                } else {
                    local.packedColor = (int)desc->shade & 0xff;
                    local.pktQuad->color = local.packedColor << 0x10 | 0x2c000000 | local.packedColor << 8 | local.packedColor;
                }
                if ((desc->flags & BATTLE_FX_BZ_MORPH_SEMI_TRANSPARENT) != 0) {
                    local.pktQuad->colorVec.cd |= GFX_PSX_PRIM_CODE_SEMI_TRANSPARENT;
                }
                Gfx_DrawPsxPrimitive(local.pktQuad);
            }
        }
        local.cursor = local.cursor + 10;
    }
    local.faceCount = (int)*local.cursor;
    local.cursor = local.cursor + 2;
    local.pktFlatTri = (BattleFxPsxQuadPacket *)local.pktQuad;
    local.lmdV0 = (short *)(local.lmdVertexBase + local.cursor[0]);
    local.lmdV1 = (short *)(local.lmdVertexBase + local.cursor[1]);
    local.lmdV2 = (short *)(local.lmdVertexBase + local.cursor[2]);
    local.plyV0 = (short *)((int)morphVertexBase + (int)local.cursor[0]);
    local.plyV1 = (short *)((int)morphVertexBase + (int)local.cursor[1]);
    local.plyV2 = (short *)((int)morphVertexBase + (int)local.cursor[2]);
    for (local.faceIndex = 0; local.faceIndex < local.faceCount; local.faceIndex = local.faceIndex + 1) {
        *g_BattleSummonBahamutZeroMorphedInner = *local.lmdV0 + (*local.plyV0 * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[1] = local.lmdV0[1] + (local.plyV0[1] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[2] = local.lmdV0[2] + (local.plyV0[2] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[4] = *local.lmdV1 + (*local.plyV1 * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[5] = local.lmdV1[1] + (local.plyV1[1] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[6] = local.lmdV1[2] + (local.plyV1[2] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[8] = *local.lmdV2 + (*local.plyV2 * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[9] = local.lmdV2[1] + (local.plyV2[1] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[10] = local.lmdV2[2] + (local.plyV2[2] * morphT4096 >> 12);
        StoreDeferredDrawArgs(g_BattleSummonBahamutZeroMorphedInner, g_BattleSummonBahamutZeroMorphedInner + 4, g_BattleSummonBahamutZeroMorphedInner + 8);
        ApplyDeferredDrawArgs();
        local.lmdV0 = (short *)(local.lmdVertexBase + local.cursor[10]);
        local.lmdV1 = (short *)(local.lmdVertexBase + local.cursor[0xb]);
        local.lmdV2 = (short *)(local.lmdVertexBase + local.cursor[0xc]);
        local.plyV0 = (short *)((int)morphVertexBase + (int)local.cursor[10]);
        local.plyV1 = (short *)((int)morphVertexBase + (int)local.cursor[0xb]);
        local.plyV2 = (short *)((int)morphVertexBase + (int)local.cursor[0xc]);
        Gfx_PsxStoreClearFlag(&local.gteFlag);
        if (((local.gteFlag & 0x60000) == 0) && (Gfx_PsxStoreFrontFacing(&local.windingSign), 0 < (int)local.windingSign)) {
            local.pktFlatTri->tag = 0x6000000;
            Gfx_ReadTransformOrderingDepth2(&local.avgZ);
            local.pktFlatTri->primWord = local.cursorWords[2];
            local.pktFlatTri->color1 = local.cursorWords[3];
            local.pktFlatTri->color2 = local.cursorWords[4];
            if ((desc->flags & BATTLE_FX_BZ_MORPH_SEMI_TRANSPARENT) != 0) {
                local.pktFlatTri->colorVec.cd |= GFX_PSX_PRIM_CODE_SEMI_TRANSPARENT;
            }
        }
        local.cursor = local.cursor + 10;
    }
    local.faceCount = (int)*local.cursor;
    local.cursor = local.cursor + 2;
    local.pktFlatQuad = local.pktFlatTri;
    local.lmdV0 = (short *)(local.lmdVertexBase + local.cursor[0]);
    local.lmdV1 = (short *)(local.lmdVertexBase + local.cursor[1]);
    local.lmdV2 = (short *)(local.lmdVertexBase + local.cursor[2]);
    local.plyV0 = (short *)((int)morphVertexBase + (int)local.cursor[0]);
    local.plyV1 = (short *)((int)morphVertexBase + (int)local.cursor[1]);
    local.plyV2 = (short *)((int)morphVertexBase + (int)local.cursor[2]);
    for (local.faceIndex = 0; local.faceIndex < local.faceCount; local.faceIndex = local.faceIndex + 1) {
        *g_BattleSummonBahamutZeroMorphedInner = *local.lmdV0 + (*local.plyV0 * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[1] = local.lmdV0[1] + (local.plyV0[1] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[2] = local.lmdV0[2] + (local.plyV0[2] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[4] = *local.lmdV1 + (*local.plyV1 * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[5] = local.lmdV1[1] + (local.plyV1[1] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[6] = local.lmdV1[2] + (local.plyV1[2] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[8] = *local.lmdV2 + (*local.plyV2 * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[9] = local.lmdV2[1] + (local.plyV2[1] * morphT4096 >> 12);
        g_BattleSummonBahamutZeroMorphedInner[10] = local.lmdV2[2] + (local.plyV2[2] * morphT4096 >> 12);
        StoreDeferredDrawArgs(g_BattleSummonBahamutZeroMorphedInner, g_BattleSummonBahamutZeroMorphedInner + 4, g_BattleSummonBahamutZeroMorphedInner + 8);
        ApplyDeferredDrawArgs();
        local.lmdV0 = (short *)(local.lmdVertexBase + local.cursor[0xc]);
        local.lmdV1 = (short *)(local.lmdVertexBase + local.cursor[0xd]);
        local.lmdV2 = (short *)(local.lmdVertexBase + local.cursor[0xe]);
        local.plyV0 = (short *)((int)morphVertexBase + (int)local.cursor[0xc]);
        local.plyV1 = (short *)((int)morphVertexBase + (int)local.cursor[0xd]);
        local.plyV2 = (short *)((int)morphVertexBase + (int)local.cursor[0xe]);
        Gfx_PsxStoreClearFlag(&local.gteFlag);
        if (((local.gteFlag & 0x60000) == 0) &&
            (Gfx_PsxStoreFrontFacing(&local.windingSign), local.windingSign >= 0)) {
            local.lmdV3 = (short *)(local.lmdVertexBase + local.cursor[3]);
            local.plyV3 = (short *)((int)morphVertexBase + (int)local.cursor[3]);
            g_BattleSummonBahamutZeroMorphedInner[0xc] = *local.lmdV3 + (*local.plyV3 * morphT4096 >> 12);
            g_BattleSummonBahamutZeroMorphedInner[0xd] = local.lmdV3[1] + (local.plyV3[1] * morphT4096 >> 12);
            g_BattleSummonBahamutZeroMorphedInner[0xe] = local.lmdV3[2] + (local.plyV3[2] * morphT4096 >> 12);
            SetPendingResourceHandle((struct GfxPsxShortVertexSlot *)(g_BattleSummonBahamutZeroMorphedInner + 0xc));
            ApplyPendingResourceHandle();
            Gfx_ReadTransformOrderingDepth2(&local.avgZ);
            local.pktFlatQuad->tag = 0x8000000;
            local.pktFlatQuad->primWord = local.cursorWords[2];
            local.pktFlatQuad->color1 = local.cursorWords[3];
            local.pktFlatQuad->color2 = local.cursorWords[4];
            local.pktFlatQuad->color3 = local.cursorWords[5];
            if ((desc->flags & BATTLE_FX_BZ_MORPH_SEMI_TRANSPARENT) != 0) {
                local.pktFlatQuad->colorVec.cd |= GFX_PSX_PRIM_CODE_SEMI_TRANSPARENT;
            }
        }
        local.cursor = local.cursor + 0xc;
    }
    return local.pktFlatQuad;
}

/*
 * Transforms and draws Bahamut ZERO's secondary flare shell in resource slot 18.
 */
// FUNCTION: FF7 0x00489cf0
void __cdecl BattleFx_BzDrawSlot12(void)
{
    int frameCount;
    BattleTaskSlot *task;
    int ratio;

    task = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleFxBzDrawSlot12Init & 1) == 0) {
        g_BattleFxBzDrawSlot12Init |= 1;
        g_BattleSummonBahamutZeroSlot12State = (struct BattleFxBahamutZeroDrawState *)(Gfx_GetPsxScratchpadPtr(0));
    }
    g_BattleFxBzDrawRequest12[0] = Gfx_ModelTableGetRenderObject(18, g_pBattleBahamutZeroTableCursor);
    /* VC5 consumes one register choice without emitting code. */
    if (REG_ALLOC_HINT) {
    }
    /* VC5 consumes one register choice without emitting code. */
    if (REG_ALLOC_HINT) {
    }
    frameCount = task->frameCounter;
    if (frameCount < 10) {
        if (g_BattleIsPaused == 0) {
            (task->frameCounter)++;
        }
    } else {
        frameCount -= 10;
        if (frameCount < 5) {
            ratio = (frameCount << 12) / 5;
            g_BattleSummonBahamutZeroSlot12State->scaleZ = ratio << 10 >> 12;
            g_BattleSummonBahamutZeroSlot12State->scaleY = g_BattleSummonBahamutZeroSlot12State->scaleZ;
            g_BattleSummonBahamutZeroSlot12State->scaleX = g_BattleSummonBahamutZeroSlot12State->scaleY;
        } else {
            frameCount -= 5;
            if (frameCount < 5) {
                ratio = (frameCount << 12) / 5;
                g_BattleSummonBahamutZeroSlot12State->scaleZ = 0x1000 - ratio << 10 >> 12;
                g_BattleSummonBahamutZeroSlot12State->scaleY = g_BattleSummonBahamutZeroSlot12State->scaleZ;
                g_BattleSummonBahamutZeroSlot12State->scaleX = g_BattleSummonBahamutZeroSlot12State->scaleY;
            } else {
                task->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
        ratio = (task->frameCounter << 12) / 10;
        g_BattleSummonBahamutZeroSlot12State->rotX = 0;
        g_BattleSummonBahamutZeroSlot12State->rotZ = ratio * 0x600 /* 0.375 */ >> 12;
        g_BattleSummonBahamutZeroSlot12State->rotY = 0;
        RotMatrix((SVECTOR *)g_BattleSummonBahamutZeroSlot12State, &g_BattleSummonBahamutZeroSlot12State->matrix);
        g_BattleSummonBahamutZeroSlot12State->matrix.t[0] = 0;
        g_BattleSummonBahamutZeroSlot12State->matrix.t[1] = 0xFFFFFDB0;
        g_BattleSummonBahamutZeroSlot12State->matrix.t[2] = 0x1000;
        ScaleMatrix(&g_BattleSummonBahamutZeroSlot12State->matrix, &g_BattleSummonBahamutZeroSlot12State->scaleX);
        SetRotMatrix(&g_BattleSummonBahamutZeroSlot12State->matrix);
        SetTransMatrix(&g_BattleSummonBahamutZeroSlot12State->matrix);
        g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)g_BattleFxBzDrawRequest12, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST_1, 0, g_BattleOverlayDrawListHead);
        if (g_BattleIsPaused == 0) {
            (task->frameCounter)++;
        }
    }
}

/*
 * Eases scale and draws the standalone ring pass for Bahamut ZERO resource slot 16.
 */
// FUNCTION: FF7 0x00489f45
void __cdecl BattleFx_BzDrawSlot10Ring(void)
{
    int frameIndex;
    BattleTaskSlot *taskPtr;
    int ease;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleFxBzDrawSlot10RingInit & 1) == 0) {
        g_BattleFxBzDrawSlot10RingInit |= 1;
        g_BattleSummonBahamutZeroSlot10RingBlock = Gfx_GetPsxScratchpadPtr(0);
    }
    (g_BattleSummonBahamutZeroRingModel.modelGroup) = Gfx_ModelTableGetRenderObject(16, g_pBattleBahamutZeroTableCursor);
    frameIndex = taskPtr->frameCounter;
    if (frameIndex < 45) {
        ease = (frameIndex << 12) / 45;
        ease = BattleFx_BzEaseFixed(ease, 0xFFFFFD10);
        g_BattleSummonBahamutZeroRingModel.bahamutZeroRingRadius = (ease * 0x3E0 >> 12) + 16;
        g_BattleSummonBahamutZeroRingModel.bahamutZeroRingRotation1 = g_BattleSummonBahamutZeroRingModel.bahamutZeroRingRadius;
        g_BattleSummonBahamutZeroRingModel.bahamutZeroRingRotation0 = g_BattleSummonBahamutZeroRingModel.bahamutZeroRingRotation1;
    } else {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
        return;
    }
    SetRotMatrix(&g_BattleSummonBahamutZeroRingModel.bahamutZeroRingRotation0);
    SetTransMatrix(&g_BattleSummonBahamutZeroRingModel.bahamutZeroRingRotation0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonBahamutZeroRingModel, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST_1, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (taskPtr->frameCounter)++;
    }
}

/*
 * Battle task that scrolls UVs and submits Bahamut ZERO's procedural energy-tube mesh.
 */
// FUNCTION: FF7 0x0048a074
void __cdecl BattleFx_BzDrawEnergyTubeTask(void)
{
    struct {
        int unused44;
        int buffer;
        int unused36;
        int inner;
        BattleFxBzTubeCell *record;
        int outer;
        int phase;
        int unused16;
        BattleTaskSlot *task;
        int unused8;
        int unused4;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.task = BATTLE_CURRENT_TASK_SLOT;
    frame.buffer = 0;
    if ((g_BattleFxBzDrawEnergyTubeTaskInit & 1) == 0) {
        g_BattleFxBzDrawEnergyTubeTaskInit |= 1;
        g_BattleSummonBahamutZeroMatrixPair.dataBlockPtr = (BattleFxBzTubeDrawBlock *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_BattleIsPaused == 0) {
        if (frame.task->frameCounter == 0) {
            BattleFx_BzBuildEnergyTube();
        }
        if (frame.task->frameCounter == 0) {
            BattleFx_BzImpulseEnergyTube(0x27100);
        }
        BattleFx_BzStepEnergyTube();
    }
    memcpy(g_BattleSummonBahamutZeroMatrixPair.dataBlockPtr, &g_BattleBahamutZeroComposedMatrix, 30);
    frame.phase = (frame.task->frameCounter << 12) / 89;
    g_BattleSummonBahamutZeroMatrixPair.dataBlockPtr->scaleZ = (frame.phase << 13 >> 12) + 0x1000;
    g_BattleSummonBahamutZeroMatrixPair.dataBlockPtr->scaleY = g_BattleSummonBahamutZeroMatrixPair.dataBlockPtr->scaleZ;
    g_BattleSummonBahamutZeroMatrixPair.dataBlockPtr->scaleX = g_BattleSummonBahamutZeroMatrixPair.dataBlockPtr->scaleY;
    ScaleMatrix(g_BattleSummonBahamutZeroMatrixPair.dataBlockPtr, &g_BattleSummonBahamutZeroMatrixPair.dataBlockPtr->scaleX);
    SetRotMatrix(g_BattleSummonBahamutZeroMatrixPair.dataBlockPtr);
    SetTransMatrix(&g_BattleBahamutZeroComposedMatrix);
    RefreshGlobalTransformState();
    Gfx_SetPsxPrimQuadState(15, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(33, g_pBattleBahamutZeroTableCursor)));
    for (frame.outer = 0; frame.outer < 32; frame.outer++) {
        for (frame.inner = 0; frame.inner < 16; frame.inner++) {
            frame.record = &g_pBattleBahamutZeroEnergyTube->cell[frame.outer][frame.inner];
            g_BattleSummonBahamutZeroMatrixPair.dataBlockPtr->depth = Gfx_TransformQuadReturnDepth((void *)frame.record->cornerRowCol, (SVECTOR *)frame.record->cornerNextCol, (SVECTOR *)frame.record->cornerNextRow, (SVECTOR *)frame.record->cornerNextRowCol, &g_BattleSummonBahamutZeroMatrixPair.dataBlockPtr->quadOut, &g_BattleSummonBahamutZeroMatrixPair.dataBlockPtr->quadFlags);
            if ((g_BattleSummonBahamutZeroMatrixPair.dataBlockPtr->quadFlags & 0x60000) == 0) {
                frame.record->packet[frame.buffer].color1 = g_pBattleBahamutZeroEnergyTube->color[frame.outer];
                frame.record->packet[frame.buffer].color0 = frame.record->packet[frame.buffer].color1;
                frame.record->packet[frame.buffer].color3 = g_pBattleBahamutZeroEnergyTube->color[frame.outer + 1];
                frame.record->packet[frame.buffer].color2 = frame.record->packet[frame.buffer].color3;
                frame.record->packet[frame.buffer].tag = (frame.record->packet[frame.buffer].tag & 0xffffff) | 0x0c000000;
                ((char *)&frame.record->packet[frame.buffer].color0)[3] = 0x3c;
                ((char *)&frame.record->packet[frame.buffer].color0)[3] |= 2;
                Gfx_DrawPsxPrimitive(&frame.record->packet[frame.buffer]);
            }
        }
    }
    if (g_BattleIsPaused == 0) {
        (frame.task->frameCounter)++;
        if (frame.task->frameCounter >= 89) {
            frame.task->ownerSlot = -1;
        }
    }
}

/*
 * Allocates and tessellates the procedural cylindrical energy-tube mesh for Bahamut ZERO.
 */
// FUNCTION: FF7 0x0048a378
void __cdecl BattleFx_BzBuildEnergyTube(void)
{
    int row;
    int vCoordinate;
    BattleFxBzTubeCell *cellPtr;
    int angle;
    int column;
    unsigned short uRemaining;
    int vertexIdx;
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    unsigned short uRight;
    int vLimit;
    int rho;
    int vBase;

    g_pBattleBahamutZeroEnergyTube = (struct BattleBahamutZeroTube *)GameHeapAlloc(1, 0x9A0C, BattleFx_BahamutZeroSourceFile(), 2067);
    for (row = 0; row < 32; row++) {
        vBase = row << 2;
        if (vBase + 4 < 128) {
            vLimit = vBase + 4;
        } else {
            vLimit = 0x7F;
        }
        vCoordinate = vLimit;
        for (column = 0; column < 16; column++) {
            cellPtr = &g_pBattleBahamutZeroEnergyTube->cell[row][column];
            cellPtr->cornerRowCol = &g_pBattleBahamutZeroEnergyTube->ring[row][column];
            cellPtr->cornerNextCol = &g_pBattleBahamutZeroEnergyTube->ring[row][(column + 1) % 16];
            cellPtr->cornerNextRow = &g_pBattleBahamutZeroEnergyTube->ring[row + 1][column];
            cellPtr->cornerNextRowCol = &g_pBattleBahamutZeroEnergyTube->ring[row + 1][(column + 1) % 16];
            if (column < 8) {
                uRemaining = column;
                uRight = column + 1;
            } else {
                uRemaining = 16 - column;
                uRight = uRemaining - 1;
            }
            uRemaining = ((uRemaining * 255) / 8);
            uRight = ((uRight * 255) / 8);
            vertexIdx = 0;
            cellPtr->packet[vertexIdx].tag = 0xC000000;
            cellPtr->packet[vertexIdx].color0 = 0x3C808080;
            cellPtr->packet[vertexIdx].color1 = 0x3C808080;
            cellPtr->packet[vertexIdx].color2 = 0x3C808080;
            cellPtr->packet[vertexIdx].color3 = 0x3C808080;
            cellPtr->packet[vertexIdx].u0 = uRemaining;
            cellPtr->packet[vertexIdx].v0 = vBase << 1;
            cellPtr->packet[vertexIdx].u1 = uRight;
            cellPtr->packet[vertexIdx].v1 = vBase << 1;
            cellPtr->packet[vertexIdx].u2 = uRemaining;
            cellPtr->packet[vertexIdx].v2 = vCoordinate << 1;
            cellPtr->packet[vertexIdx].u3 = uRight;
            cellPtr->packet[vertexIdx].v3 = vCoordinate << 1;
            cellPtr->packet[vertexIdx].tpage = 0xAE;
            cellPtr->packet[vertexIdx].clut = 0x3FB0;
        }
    }
    for (row = 0; row < 33; row++) {
        if (row < 32) {
            rho = row << 2 << 6;
        } else {
            rho = 0x1FC0;
        }
        vBase = rho;
        for (column = 0; column < 16; column++) {
            angle = ((column << 12) / 16);
            g_pBattleBahamutZeroEnergyTube->ring[row][column].x = vBase * SinFixed(angle) >> 12;
            g_pBattleBahamutZeroEnergyTube->ring[row][column].y = 0;
            g_pBattleBahamutZeroEnergyTube->ring[row][column].z = -vBase * CosFixed(angle) >> 12;
        }
        g_pBattleBahamutZeroEnergyTube->height[row] = 0;
        g_pBattleBahamutZeroEnergyTube->velocity[row] = 0;
    }
}

// clang-format off
#define g_BattleBahamutZeroSlot14RandomPhase (*(short *)((char *)&g_BattleSummonBahamutZeroBeamTable + 10))
#define g_BattleBahamutZeroSlot15ResourceHandle (*(int *)((char *)&g_BattleSummonBahamutZeroBeamTable + 16))
#define g_BattleBahamutZeroSlot15ScaleValue (*(short *)((char *)&g_BattleSummonBahamutZeroBeamTable + 26))
// clang-format on

/*
 * For thirty-three rows integrates a shared height int by its velocity, caps positive height at
 * zero and copies height shifted ten into sixteen vertex Y shorts. Adds 1500 and a sine-eased
 * contribution scaled by 5000 to velocity, then shades the row color using its resulting height
 * relative to -500; no pause guard appears.
 */
// FUNCTION: FF7 0x0048a73a
void __cdecl BattleFx_BzStepEnergyTube(void)
{
    int ease;
    int shadeScale;
    int column;
    int rowIdx;

    for (rowIdx = 0; rowIdx < 33; rowIdx++) {
        g_pBattleBahamutZeroEnergyTube->height[rowIdx] += g_pBattleBahamutZeroEnergyTube->velocity[rowIdx];
        if (g_pBattleBahamutZeroEnergyTube->height[rowIdx] > 0) {
            g_pBattleBahamutZeroEnergyTube->height[rowIdx] = 0;
        }
        for (column = 0; column < 16; column++) {
            g_pBattleBahamutZeroEnergyTube->ring[rowIdx][column].y = g_pBattleBahamutZeroEnergyTube->height[rowIdx] >> 10;
        }
        g_pBattleBahamutZeroEnergyTube->velocity[rowIdx] += 1500;
        ease = SinFixed((rowIdx << 10) / 32 + 0x400);
        ease = BattleFx_BzEaseFixed(ease, 0xFFFFFC40);
        g_pBattleBahamutZeroEnergyTube->velocity[rowIdx] += ease * 0x1388 >> 12;
        if (-g_pBattleBahamutZeroEnergyTube->ring[rowIdx][0].y < 500) {
            shadeScale = (g_pBattleBahamutZeroEnergyTube->ring[rowIdx][0].y << 12) / 500 + 0x1000;
        } else {
            shadeScale = 0;
        }
        Gfx_ShadePsxVertexColor((CVECTOR *)&g_BattleFxBahamutZeroSprite, shadeScale, (CVECTOR *)&g_pBattleBahamutZeroEnergyTube->color[rowIdx]);
    }
}

/*
 * For indices 0 through 0x20, subtracts SinFixed((index<<10)/0x20+0x400)*amplitude shifted right by 12
 * from g_pBattleBahamutZeroEnergyTube->velocity[index].
 */
// FUNCTION: FF7 0x0048a8eb
void __cdecl BattleFx_BzImpulseEnergyTube(int amplitude)
{
    int i;

    for (i = 0; i < 33; i++) {
        g_pBattleBahamutZeroEnergyTube->velocity[i] -= SinFixed((i << 10) / 32 + 1024) * amplitude >> 12;
    }
}

/*
 * Per-frame handler for the effect slot at
 * g_BattleHitLivewireEmitterRecords + (g_BattleActiveTaskSlotIdx << 5). Stores
 * Gfx_ModelTableGetRenderObject(16, g_pBattleBahamutZeroTableCursor) into the model draw request; for frames below 5
 * writes 0 to its fade level, for frames 5..14 writes ((frame-5) << 12) / 10, and from
 * frame 15 retires the slot (ownerSlot 0xFFFF) and returns. Otherwise it aims the matrix at
 * g_BattleSummonBahamutZeroSlot10SpriteAimOrigin along ((frame << 12) / 15) sign-folded through 20 bits
 * with BattleFx_BzAimMatrixAlongDelta, sets the draw colour to black, submits that request
 * into g_BattleCurrentOrderingTable+0x4084 at shift 0 chaining
 * g_BattleOverlayDrawListHead, and unless g_BattleIsPaused is set advances the slot frameCounter.
 */
// FUNCTION: FF7 0x0048a952
void __cdecl BattleFx_BzDrawSlot10Sprite(void)
{
    int angle;
    BattleTaskSlot *rec;
    int localFrame;
    int ratio;

    rec = BATTLE_CURRENT_TASK_SLOT;
    g_BattleFxBahamutZeroSprite.tableValue = Gfx_ModelTableGetRenderObject(16, g_pBattleBahamutZeroTableCursor);
    localFrame = rec->frameCounter;
    if (localFrame < 5) {
        g_BattleFxBahamutZeroSprite.posX = 0;
    } else {
        localFrame -= 5;
        if (localFrame < 10) {
            ratio = (localFrame << 12) / 10;
            g_BattleFxBahamutZeroSprite.posX = (short)ratio;
        } else {
            rec->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    ratio = (rec->frameCounter << 12) / 15;
    angle = ratio << 12 >> 12;
    BattleFx_BzAimMatrixAlongDelta(&g_BattleSummonBahamutZeroSlot10SpriteAimOrigin, (short)angle, 0);
    SetDrawColorRGB(0, 0, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)(((int)&g_BattleFxBahamutZeroSprite + 8)), g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (rec->frameCounter)++;
    }
}

/*
 * Updates transform state and submits Bahamut ZERO's final afterglow model in resource slot 19.
 */
// FUNCTION: FF7 0x0048aa61
void __cdecl BattleFx_BzDrawSlot13(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[44];
        PsxMatrix vecX;
        struct { unsigned char pad_to_20[0x20]; int progress; };
        struct { unsigned char pad_to_24[0x24]; BattleTaskSlot *task; };
        struct { unsigned char pad_to_28[0x28]; int timer; };
    } frame;
#pragma pack(pop)

    frame.task = BATTLE_CURRENT_TASK_SLOT;
    frame.vecX.m[0][0] = 0;
    frame.vecX.m[0][1] = 0;
    frame.vecX.m[0][2] = 0;
    frame.vecX.m[1][0] = 0;
    frame.vecX.m[1][1] = 0;
    frame.vecX.m[1][2] = 0;
    frame.vecX.m[2][0] = 0;
    frame.vecX.m[2][1] = 0;
    frame.vecX.m[2][2] = 0;
    frame.vecX.t[0] = 0;
    frame.vecX.t[1] = 0;
    frame.vecX.t[2] = 0;
    if ((g_BattleFxBzDrawSlot13Init & 1) == 0) {
        g_BattleFxBzDrawSlot13Init |= 1;
        g_BattleSummonBahamutZeroSlot13State = (BattleFxBzModelBlock *)Gfx_GetPsxScratchpadPtr(0);
    }
    g_BattleFxBzDrawRequest13[0] = Gfx_ModelTableGetRenderObject(19, g_pBattleBahamutZeroTableCursor);
    if (REG_ALLOC_HINT) {
    }
    if (REG_ALLOC_HINT) {
    }
    frame.timer = frame.task->frameCounter;
    if (frame.timer < 89) {
        frame.progress = (frame.timer << 12) / 89;
        frame.progress = BattleFx_BzEaseFixed(frame.progress, 0xFFFFFD80);
        frame.vecX.m[2][2] = frame.progress << 13 >> 12;
        frame.vecX.m[1][1] = frame.vecX.m[2][2];
        frame.vecX.m[0][0] = frame.vecX.m[1][1];
    } else {
        frame.task->ownerSlot = BATTLE_TASK_FREE;
        return;
    }
    MultiplyMatrix(&g_BattleBahamutZeroComposedMatrix, &frame.vecX, &g_BattleSummonBahamutZeroSlot13State->matrix);
    SetRotMatrix(&g_BattleSummonBahamutZeroSlot13State->matrix);
    SetTransMatrix(&g_BattleSummonBahamutZeroSlot13State->matrix);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)g_BattleFxBzDrawRequest13, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    g_BattleSummonBahamutZeroSlot13State->scaleZ = 0xF00 /* 0.9375 */;
    g_BattleSummonBahamutZeroSlot13State->scaleY = 0xF00 /* 0.9375 */;
    g_BattleSummonBahamutZeroSlot13State->scaleX = 0xF00 /* 0.9375 */;
    ScaleMatrix(&g_BattleSummonBahamutZeroSlot13State->matrix, g_BattleSummonBahamutZeroSlot13State);
    SetRotMatrix(&g_BattleSummonBahamutZeroSlot13State->matrix);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)g_BattleFxBzDrawRequest13, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (frame.task->frameCounter)++;
    }
}

// Spawns five task slots that run BattleFx_BzDrawSlot14, unless g_BattleIsPaused is set. Each slot
// gets zeros in posX, posZ, rotationX and rotationZ, a startOffset of i * -0x7D0 - 0x1388, a random phase
// from rand() & 0xFFF, and a scale of a + 0x1000, where a starts at 0x6000 and halves each
// iteration ((a << 11) >> 12). Afterwards the current task slot (at g_BattleHitLivewireEmitterRecords
// + (g_BattleActiveTaskSlotIdx << 5)) is retired by writing 0xFFFF to its first word.
// FUNCTION: FF7 0x0048ac5e
void __cdecl BattleFx_BzSpawnSlot14Swarm(void)
{
    int scale;
    BattleTaskSlot *taskSlot;
    BattleFxBzSwarmParticle *particle;
    int i;

    taskSlot = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    scale = 0x6000 /* 6.0 */;
    for (i = 0; i < 5; i++) {
        particle = (BattleFxBzSwarmParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_BzDrawSlot14);
        particle->posZ = 0;
        particle->posX = 0;
        particle->startOffset = i * -0x7D0 - 0x1388;
        particle->rotationZ = 0;
        particle->rotationX = 0;
        particle->phase = rand() & 0xFFF;
        particle->scale = scale + 0x1000;
        scale = (scale << 11) >> 12;
    }
    taskSlot->ownerSlot = BATTLE_TASK_FREE;
}

/*
 * Acquires retained scratch and refreshes Bahamut Zero resource field 0x14. Below count ten clears
 * descriptor term; counts ten through fourteen ramp it from elapsed shifted twelve divided by
 * five; later retire before drawing/pause. Builds saved task rotation/XYZ translation. Eases total
 * count divided by fifteen with parameter 0xFFFFFD80 and sets X/Z scale to saved task size times
 * that fraction while retaining Y 4096. Scales, multiplies g_BattleBahamutZeroComposedMatrix, installs and submits
 * black into g_BattleCurrentOrderingTable plus 0x70 with shift twelve. Only count advancement is pause-gated.
 */
// FUNCTION: FF7 0x0048ad3c
void __cdecl BattleFx_BzDrawSlot14(void)
{
    int frameCount;
    BattleFxEasedTransformSlot *task;
    int ratio;

    task = (BattleFxEasedTransformSlot *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleFxBzDrawSlot14Init & 1) == 0) {
        g_BattleFxBzDrawSlot14Init |= 1;
        g_pBattleBahamutZeroSlot14State = (BattleFxBzModelBlock *)Gfx_GetPsxScratchpadPtr(0);
    }
    (*(int *)&g_BattleSummonBahamutZeroBeamTable) = Gfx_ModelTableGetRenderObject(20, g_pBattleBahamutZeroTableCursor);
    frameCount = task->frame;
    if (frameCount < 10) {
        g_BattleBahamutZeroSlot14RandomPhase = 0;
    } else {
        frameCount -= 10;
        if (frameCount < 5) {
            ratio = (frameCount << 12) / 5;
            g_BattleBahamutZeroSlot14RandomPhase = ratio;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    RotMatrix((SVECTOR *)((int)task + 0xC), &g_pBattleBahamutZeroSlot14State->matrix);
    g_pBattleBahamutZeroSlot14State->matrix.t[0] = task->posX;
    g_pBattleBahamutZeroSlot14State->matrix.t[1] = task->posY;
    g_pBattleBahamutZeroSlot14State->matrix.t[2] = task->posZ;
    ratio = (task->frame << 12) / 15;
    ratio = BattleFx_BzEaseFixed(ratio, 0xFFFFFD80);
    g_pBattleBahamutZeroSlot14State->scaleZ = task->scale * ratio >> 12;
    g_pBattleBahamutZeroSlot14State->scaleX = g_pBattleBahamutZeroSlot14State->scaleZ;
    g_pBattleBahamutZeroSlot14State->scaleY = 0x1000 /* 1.0 */;
    ScaleMatrix(&g_pBattleBahamutZeroSlot14State->matrix, g_pBattleBahamutZeroSlot14State);
    MultiplyMatrix(&g_BattleBahamutZeroComposedMatrix, &g_pBattleBahamutZeroSlot14State->matrix, &g_pBattleBahamutZeroSlot14State->matrix);
    SetRotMatrix(&g_pBattleBahamutZeroSlot14State->matrix);
    SetTransMatrix(&g_pBattleBahamutZeroSlot14State->matrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonBahamutZeroBeamTable, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frame)++;
    }
}

/*
 * When unpaused derives an emission accumulator from the counter and repeatedly allocates
 * BattleFx_BzDrawSlot15 subtasks while it is at least 0x100. Stores rotationX and rotationY from
 * masked random values, clears rotationZ and posZ and assigns spawnValue a random 800 through 999,
 * subtracting 0x100 each iteration; advances the parent counter and retires at thirty or above.
 */
// FUNCTION: FF7 0x0048af40
void __cdecl BattleFx_BzSpawnDust(void)
{
    int amount;
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int outRatio;
    BattleTaskSlot *taskSlot;
    int spawnValue;
    BattleFxBzParticle *particle;

    taskSlot = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    outRatio = (taskSlot->frameCounter << 12) / 30;
    amount = (outRatio * -0x800 /* 0.5 */ >> 12) + 2560;
    amount = (amount * taskSlot->frameCounter & 0xFF) + amount;
    while (amount >= 256) {
        particle = (BattleFxBzParticle *)(BattleFx_BzAllocSubTask((int)BattleFx_BzDrawSlot15));
        particle->rotationX = -rand() & 0x3FF;
        particle->rotationY = rand() & 0xFFF;
        particle->rotationZ = 0;
        particle->posZ = 0;
        if (DEAD_IN_RETAIL != 0) {
            spawnValue = 0x3E8;
        } else {
            spawnValue = rand() % 200 + 800;
        }
        particle->spawnValue = (short)spawnValue;
        amount -= 256;
    }
    (taskSlot->frameCounter)++;
    if (taskSlot->frameCounter >= 30) {
        taskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Claims the first zero g_BattleSummonBahamutZeroSubTasks entry among 500 by storing value, clears two corresponding
 * g_pBattleBahamutZeroSubTasks record words at stride 0x20, and returns its address; returns zero when full.
 */
// FUNCTION: FF7 0x0048b06a
int __cdecl BattleFx_BzAllocSubTask(int value)
{
    int i;

    for (i = 0; i < 500; i++) {
        if (g_BattleSummonBahamutZeroSubTasks[i] == 0) {
            g_BattleSummonBahamutZeroSubTasks[i] = (void (__cdecl *)(int))value;
            g_pBattleBahamutZeroSubTasks[i].state = 0;
            g_pBattleBahamutZeroSubTasks[i].frame = 0;
            return ((int)g_pBattleBahamutZeroSubTasks) + (i << 5);
        }
    }
    return 0;
}

/*
 * Acquires retained scratch and refreshes Bahamut Zero resource field 0x15 for the supplied task.
 * Below count ten clears descriptor term; counts ten through fourteen ramp from elapsed shifted
 * twelve divided by five; later retire before drawing/pause. Builds task rotation with zero
 * translation and multiplies g_BattleBahamutZeroComposedMatrix. Writes retained
 * g_BattleSummonBahamutZeroSlot15ScaleMatrix matrix X/Y diagonal shorts 4096, Z diagonal as total
 * count shifted twelve divided by fifteen and Z translation from task short eight plus 2000,
 * leaving its other fields retained, then multiplies it into the composed task matrix. Installs and
 * submits black into g_BattleCurrentOrderingTable plus 0x70 with shift twelve. While unpaused
 * advances count and adds task short four to short eight.
 */
// FUNCTION: FF7 0x0048b0df
void __cdecl BattleFx_BzDrawSlot15(YasuiBzSlotState *slotState)
{
    int scale;
    int elapsed;

    if ((g_BattleFxBzDrawSlot15Init & 1) == 0) {
        g_BattleFxBzDrawSlot15Init |= 1;
        g_pBattleBahamutZeroSlot15Matrix = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    }
    g_BattleBahamutZeroSlot15ResourceHandle = Gfx_ModelTableGetRenderObject(21, g_pBattleBahamutZeroTableCursor);
    elapsed = slotState->frame;
    if (elapsed < 10) {
        g_BattleBahamutZeroSlot15ScaleValue = 0;
    } else {
        elapsed -= 10;
        if (elapsed < 5) {
            scale = (elapsed << 12) / 5;
            g_BattleBahamutZeroSlot15ScaleValue = scale;
        } else {
            slotState->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    RotMatrix((SVECTOR *)((char *)slotState + 0xC), g_pBattleBahamutZeroSlot15Matrix);
    g_pBattleBahamutZeroSlot15Matrix->t[2] = 0;
    g_pBattleBahamutZeroSlot15Matrix->t[1] = 0;
    g_pBattleBahamutZeroSlot15Matrix->t[0] = 0;
    MultiplyMatrix(&g_BattleBahamutZeroComposedMatrix, g_pBattleBahamutZeroSlot15Matrix, g_pBattleBahamutZeroSlot15Matrix);
    scale = (slotState->frame << 12) / 15;
    g_BattleSummonBahamutZeroSlot15ScaleM11 = 0x1000 /* 1.0 */;
    *(short *)&g_BattleSummonBahamutZeroSlot15ScaleMatrix = g_BattleSummonBahamutZeroSlot15ScaleM11;
    (g_BattleSummonBahamutZeroSlot15ScaleTail.bahamutZeroScale) = scale << 12 >> 12;
    g_BattleSummonBahamutZeroSlot15ScaleTail.bahamutZeroSlot15ZTranslation = slotState->posZ + 0x7D0;
    MultiplyMatrix(g_pBattleBahamutZeroSlot15Matrix, &g_BattleSummonBahamutZeroSlot15ScaleMatrix, g_pBattleBahamutZeroSlot15Matrix);
    SetRotMatrix(g_pBattleBahamutZeroSlot15Matrix);
    SetTransMatrix(g_pBattleBahamutZeroSlot15Matrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleBahamutZeroSlot15ResourceHandle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (slotState->frame)++;
        slotState->posZ += slotState->speed;
    }
}

/*
 * Returns while paused. From count shifted twelve
 * divided by thirty derives emission term 0xA00 minus fraction times 0x800 and radius fraction
 * times 10000. Adds the low byte of emission term times count to that term, then allocates
 * BattleFx_BzDrawGroundSpark records through BattleFx_BzAllocSubTask while it is at least 0x100,
 * discarding remainder. Each gets a random full-circle angle, X/Z from radius times sine/cosine,
 * Y zero and random size 0x1000 through 0x15FF. Advances count and retires at thirty.
 */
// FUNCTION: FF7 0x0048b2c0
void __cdecl BattleFx_BzSpawnGroundSparks(void)
{
    int angle;
    int ringRadius;
    BattleTaskSlot *task;
    int ratio;
    BattleFxBzParticle *sparkTaskPtr;
    int emission;
    int bytes;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    ratio = (task->frameCounter << 12) / 30;
    emission = (ratio * -0x800 /* 0.5 */ >> 12) + 0xA00;
    emission = (emission * task->frameCounter & 0xFF) + emission;
    ringRadius = ratio * 0x2710 >> 12;
    while (emission >= 256) {
        sparkTaskPtr = (BattleFxBzParticle *)(BattleFx_BzAllocSubTask((int)BattleFx_BzDrawGroundSpark));
        angle = rand() & 0xFFF;
        sparkTaskPtr->spawnValue = ringRadius * SinFixed(angle) >> 12;
        sparkTaskPtr->posY = 0;
        sparkTaskPtr->posZ = ringRadius * CosFixed(angle) >> 12;
        if (DEAD_IN_RETAIL != 0) {
            bytes = 0x1600;
        } else {
            bytes = rand() % 1536 + 4096;
        }
        sparkTaskPtr->scale = bytes;
        emission -= 0x100;
    }
    (task->frameCounter)++;
    if (task->frameCounter >= 30) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Per-frame draw for a ground-spark slot: aims a matrix
 * from the slot's aim vector by its aimAngle via BattleFx_BzAimMatrixAlongDelta, copies the
 * frame index into the model draw request, refreshes the transform state, sets state fields
 * 0xB/4 and 0xB/1 from entries 0x1B and 0x1C of the table at g_pBattleBahamutZeroTableCursor,
 * and chains a primitive emit through g_BattleOverlayDrawListHead (bucket
 * g_BattleCurrentOrderingTable+0x70, shift 0xC). While g_BattleIsPaused is zero it increments
 * the frame index and retires the slot (ownerSlot 0xFFFF) once that reaches the signed byte
 * g_BattleSummonBahamutZeroSparkFrameLimit.
 */
// FUNCTION: FF7 0x0048b411
void __cdecl BattleFx_BzDrawGroundSpark(BattleFxBzGroundLivewireSlot *p)
{
    BattleFx_BzAimMatrixAlongDelta((char *)p + 4, p->aimAngle, 0);
    ((BattleFxModelDrawRequest *)&g_BattleSummonBahamutZeroBeamTable)->frameIndex = p->frameIndex;
    RefreshGlobalTransformState();
    Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(27, g_pBattleBahamutZeroTableCursor)));
    Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(28, g_pBattleBahamutZeroTableCursor)));
    g_BattleOverlayDrawListHead = BattleFx_BzEmitGroundSparkPrim((BattleFxGroundLivewireDescriptor *)(((int)&g_BattleSummonBahamutZeroBeamTable + 32)), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (p->frameIndex)++;
        if (p->frameIndex >= *(char *)&g_BattleSummonBahamutZeroSparkFrameLimit) {
            p->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Acquires retained 0xC0 scratch and render storage.
 * Starts at descriptor resource pointer plus eight, skips the unsigned-short number of sections
 * from descriptor eight using each signed record count times twenty plus four, then reads the
 * selected signed-short count. Clears four scratch Z shorts and for each twenty-byte record
 * unpacks width/height and UV increments from two packed shorts. Builds an XY quad from record
 * position and dimensions shifted five, projects its four corners and, for flags lacking 0x60000,
 * writes header 0x09000000, descriptor colour, record texture shorts and wrapped UV bytes, then
 * submits through the same render pointer. Advances the record by twenty and returns that pointer.
 * Arguments two/three/four and projection return are unused.
 */
// FUNCTION: FF7 0x0048b4d9
int __cdecl BattleFx_BzEmitGroundSparkPrim(BattleFxGroundLivewireDescriptor *descriptor, int unusedA, int unusedB, int unusedC)
{
    short width;
    int padEntry; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short vAdvance;
    short cellH;
    int i;
    int unusedWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxBzGroundLivewireRecord *recordPointer;
    int numberRecords;
    int zDepth;
    int padField; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxGroundLivewirePrim *packet;
    short uStep;

    if ((g_BattleFxBzEmitGroundSparkPrimInit & 1) == 0) {
        g_BattleFxBzEmitGroundSparkPrimInit |= 1;
        g_BattleSummonBahamutZeroGroundSpark = (struct BattleFxBzGroundLivewireScratch *)(Gfx_GetPsxScratchpadPtr(0xC0));
    }
    packet = (BattleFxGroundLivewirePrim *)Gfx_GetPsxPacketScratchPtr();
    recordPointer = (BattleFxBzGroundLivewireRecord *)(descriptor->recordsBase + 8);
    for (i = 0; i < descriptor->recordCount; i++) {
        recordPointer = (BattleFxBzGroundLivewireRecord *)((int)recordPointer + recordPointer->count * 0xA * 2 + 4);
    }
    numberRecords = recordPointer->count;
    recordPointer = (BattleFxBzGroundLivewireRecord *)((int)recordPointer + 4);
    g_BattleSummonBahamutZeroGroundSpark->vertex3Vz = 0;
    g_BattleSummonBahamutZeroGroundSpark->vertex2Vz = 0;
    g_BattleSummonBahamutZeroGroundSpark->vertex1Vz = 0;
    g_BattleSummonBahamutZeroGroundSpark->vertex0Vz = 0;
    for (i = 0; i < numberRecords; i++) {
        width = recordPointer->cellWidth & 0xFF;
        uStep = recordPointer->cellWidth >> 8 & 0xFF;
        cellH = recordPointer->cellHeight & 0xFF;
        vAdvance = recordPointer->cellHeight >> 8 & 0xFF;
        g_BattleSummonBahamutZeroGroundSpark->originX = recordPointer->gridX << 5;
        g_BattleSummonBahamutZeroGroundSpark->curX = g_BattleSummonBahamutZeroGroundSpark->originX;
        g_BattleSummonBahamutZeroGroundSpark->targetX = recordPointer->gridX + width << 5;
        g_BattleSummonBahamutZeroGroundSpark->curTargetX = g_BattleSummonBahamutZeroGroundSpark->targetX;
        g_BattleSummonBahamutZeroGroundSpark->originY = recordPointer->gridY << 5;
        g_BattleSummonBahamutZeroGroundSpark->curY = g_BattleSummonBahamutZeroGroundSpark->originY;
        g_BattleSummonBahamutZeroGroundSpark->targetY = recordPointer->gridY + cellH << 5;
        g_BattleSummonBahamutZeroGroundSpark->curTargetY = g_BattleSummonBahamutZeroGroundSpark->targetY;
        zDepth = Gfx_TransformQuadReturnDepth(g_BattleSummonBahamutZeroGroundSpark, (SVECTOR *)&g_BattleSummonBahamutZeroGroundSpark->curTargetX, (SVECTOR *)&g_BattleSummonBahamutZeroGroundSpark->originX, (SVECTOR *)&g_BattleSummonBahamutZeroGroundSpark->targetX, (int *)&g_BattleSummonBahamutZeroGroundSpark->centerX, &g_BattleSummonBahamutZeroGroundSpark->flags);
        if ((g_BattleSummonBahamutZeroGroundSpark->flags & 0x60000) == 0) {
            packet->tagWord = 0x9000000;
            packet->colorRgb = descriptor->color;
            packet->clut = recordPointer->clut;
            packet->texturePage = recordPointer->tpage;
            packet->uStart = recordPointer->uStart;
            packet->uCoord = packet->uStart;
            packet->uNext = *(unsigned char *)&packet->uCoord + uStep;
            packet->uNextTop = packet->uNext;
            packet->vStart = recordPointer->vStart;
            packet->vCoord = packet->vStart;
            packet->vNext = *(unsigned char *)&packet->vCoord + vAdvance;
            packet->vNextBottom = packet->vNext;
            Gfx_DrawPsxPrimitive(packet);
        }
        recordPointer++;
    }
    return (int)packet;
}

/*
 * Acquires retained scratch and refreshes Bahamut Zero
 * resource fields 0x10 and 0x16. Count zero clears task XYZ. Negative counts enter an explicit
 * division-by-zero expression before assigning retained matrix short 0x7000; nonnegative counts
 * below fifteen linearly reduce that short using count shifted twelve divided by fifteen, and later
 * retire before drawing/pause. Transforms task position through g_BattleBahamutZeroComposedMatrix
 * into retained translation g_BattleSummonBahamutZeroSlot10And16Points.bahamutZeroSlot10And16Point,
 * installs g_BattleSummonBahamutZeroSlot1016Angles rotation/translation and submits resource 0x16.
 * Installs rotation g_BattleSummonBahamutZeroSlot10And16Rotation only and submits resource 0x10,
 * retaining the first translation. Both use g_BattleCurrentOrderingTable plus 0x70 with shift
 * twelve and retained draw colour. Only count advancement is pause-gated; negative-count division
 * and unused fraction in that branch.
 */
// FUNCTION: FF7 0x0048b7c0
void __cdecl BattleFx_BzDrawSlot10And16(void)
{
    int frameCount;
    BattleFxCosmoMemoryExpandingModel *task;
    int ratio;

    task = (BattleFxCosmoMemoryExpandingModel *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleFxBzDrawSlot10And16Init & 1) == 0) {
        g_BattleFxBzDrawSlot10And16Init |= 1;
        g_BattleSummonBahamutZeroSlot10And16Block = Gfx_GetPsxScratchpadPtr(0);
    }
    g_BattleSummonBahamutZeroSlot10And16Draw.bahamutZeroSlot10And16DrawHandle = Gfx_ModelTableGetRenderObject(16, g_pBattleBahamutZeroTableCursor);
    (g_BattleSummonBahamutZeroSlot10And16Draw.modelGroup) = Gfx_ModelTableGetRenderObject(22, g_pBattleBahamutZeroTableCursor);
    if (task->frame == 0) {
        task->posZ = 0;
        task->posY = 0;
        task->posX = 0;
    }
    frameCount = task->frame;
    if (frameCount < 0) {
        ratio = (frameCount << 12) / 0;
        g_BattleSummonBahamutZeroSlot1016Angles[0] = 0x7000 /* 2520 deg */;
    } else {
        frameCount = frameCount;
        if (frameCount < 15) {
            ratio = (frameCount << 12) / 15;
            g_BattleSummonBahamutZeroSlot1016Angles[0] = (ratio * -0x7000 /* 7.0 */ >> 12) + 0x7000;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    SetRotMatrix(&g_BattleBahamutZeroComposedMatrix);
    SetTransMatrix(&g_BattleBahamutZeroComposedMatrix);
    RotTransPoint((char *)task + 4, (int *)&g_BattleSummonBahamutZeroSlot10And16Points.bahamutZeroSlot10And16Point, (void *)(g_BattleSummonBahamutZeroSlot10And16Block + 0x28));
    SetRotMatrix((PsxMatrix *)g_BattleSummonBahamutZeroSlot1016Angles);
    SetTransMatrix(g_BattleSummonBahamutZeroSlot1016Angles);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonBahamutZeroSlot10And16Draw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    SetRotMatrix(&g_BattleSummonBahamutZeroSlot10And16Rotation);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonBahamutZeroSlot10And16Draw.bahamutZeroSlot10And16DrawHandle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frame)++;
    }
}

/*
 * Acquires retained scratch and derives rotation from four
 * successive 45-count ranges of the supplied value. Y ramps across base angles zero, 0x400, 0x800
 * and 0xC00 using easing parameters 0xFFFFFE01, 0x2FF, 0xFFFFFC01 and 0x2FF respectively. The last
 * range has no upper bound. Retains stage-specific X/Z computations and comma-zero expressions, but
 * unconditionally overwrites both X and Z shorts with zero before RotMatrix, so the applied
 * rotation is Y only. Clears matrix translation and left-multiplies this rotation into
 * g_BattleBahamutZeroComposedMatrix in place. No pause or retirement logic appears.
 */
// FUNCTION: FF7 0x0048b98d
void __cdecl BattleFx_BzSpinWorldMatrix(int phase)
{
    int eased;
    int unusedSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    if ((g_BattleSummonBahamutZeroMatrixPair.spinBlockLoaded & 1) == 0) {
        g_BattleSummonBahamutZeroMatrixPair.spinBlockLoaded |= 1;
        g_BattleSummonBahamutZeroTaskRecord = (BattleFxBzSpinBlock *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (phase < 45) {
        eased = (phase << 12) / 45;
        eased = BattleFx_BzEaseFixed(eased, 0xFFFFFE01);
        g_BattleSummonBahamutZeroTaskRecord->offsetX = UNFOLDED(0) >> 12;
        g_BattleSummonBahamutZeroTaskRecord->offsetY = eased << 10 >> 12;
        g_BattleSummonBahamutZeroTaskRecord->offsetZ = UNFOLDED(0) >> 12;
    } else {
        phase -= 0x2D;
        if (phase < 45) {
            eased = (phase << 12) / 45;
            eased = BattleFx_BzEaseFixed(eased, 0x2FF);
            g_BattleSummonBahamutZeroTaskRecord->offsetX = UNFOLDED(0) >> 12;
            g_BattleSummonBahamutZeroTaskRecord->offsetY = (eased << 10 >> 12) + 0x400;
            g_BattleSummonBahamutZeroTaskRecord->offsetZ = eased << 9 >> 12;
        } else {
            phase -= 0x2D;
            if (phase < 45) {
                eased = (phase << 12) / 45;
                eased = BattleFx_BzEaseFixed(eased, 0xFFFFFC01);
                g_BattleSummonBahamutZeroTaskRecord->offsetX = eased * 0x600 /* 0.375 */ >> 12;
                g_BattleSummonBahamutZeroTaskRecord->offsetY = (eased << 10 >> 12) + 0x800;
                g_BattleSummonBahamutZeroTaskRecord->offsetZ = (eased * -0x300 /* 0.1875 */ >> 12) + 0x200;
            } else {
                phase -= 0x2D;
                eased = (phase << 12) / 45;
                eased = BattleFx_BzEaseFixed(eased, 0x2FF);
                g_BattleSummonBahamutZeroTaskRecord->offsetX = (eased * -0x600 /* 0.375 */ >> 12) + 0x600;
                g_BattleSummonBahamutZeroTaskRecord->offsetY = (eased << 10 >> 12) + 0xC00;
                g_BattleSummonBahamutZeroTaskRecord->offsetZ = (eased << 8 >> 12) - 0x100;
            }
        }
    }
    g_BattleSummonBahamutZeroTaskRecord->offsetZ = 0;
    g_BattleSummonBahamutZeroTaskRecord->offsetX = 0;
    RotMatrix((SVECTOR *)g_BattleSummonBahamutZeroTaskRecord, &g_BattleSummonBahamutZeroTaskRecord->matrix);
    g_BattleSummonBahamutZeroTaskRecord->matrix.t[2] = 0;
    g_BattleSummonBahamutZeroTaskRecord->matrix.t[1] = 0;
    g_BattleSummonBahamutZeroTaskRecord->matrix.t[0] = 0;
    MultiplyMatrix(&g_BattleSummonBahamutZeroTaskRecord->matrix, &g_BattleBahamutZeroComposedMatrix, &g_BattleBahamutZeroComposedMatrix);
}

/*
 * Acquires retained scratch before pause and controls actor three at g_BattleActorRecords[3].
 * Unpaused waits 264 counts. Next 25 allocate ring record one on entry with transformed retained
 * position/angle, ramp actor short six from sixteen toward 512 and after elapsed one advance
 * retained position by the rotated (40,-200,-360) vector. Next 55 hold short six 512, change actor
 * angle over a middle twenty-count span toward retained angle minus 0x800, and advance position by
 * that rotated vector scaled with one minus easing of the full 55-count span. Earlier staged
 * computations of s are overwritten by this movement fraction. Next fifty set retained position
 * (0,0xD9EA,0xD6FC) and short six 4096 on entry. Next 120 set RGB bytes white and short six 512 on
 * entry, oscillate short 0x14 with an eight-count sine and set short six 4096 at elapsed 45. Next
 * fifteen clear short 0x14 on entry. Final 35 set position (0,0xD8BE,0xD666), short 0x15E to 0x180
 * and short six 512 on entry; at elapsed 34 allocate ring record two and retire, still reaching the
 * common tail. Every unpaused call transforms retained position through
 * g_BattleSummonBahamutZeroLightRig, adds its retained translation into actor XYZ and increments
 * count. No restoration appears.
 */
// FUNCTION: FF7 0x0048bbfc
void __cdecl BattleFx_BzRunLightRig(void)
{
    int frameCount;
    BattleBzLightRig *rig;
    BattleTaskSlot *task;
    int ratio;
    BattleFxBzLightRig *pRingRecord;
    int subFrame;

    task = BATTLE_CURRENT_TASK_SLOT;
    rig = (BattleBzLightRig *)&g_BattleActorRecords[3].modelId;
    if ((g_BattleFxBzRunLightRigInit & 1) == 0) {
        g_BattleFxBzRunLightRigInit |= 1;
        g_pBattleBahamutZeroLightRigState = (BattleFxBzRigBlock *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frameCount = task->frameCounter;
    if (frameCount < 264) { /* empty first arm: lib/codegen.h */
    } else {
        frameCount -= 264;
        if (frameCount < 25) {
            if (frameCount == 0) {
                pRingRecord = (BattleFxBzLightRig *)Battle_AllocSceneMessage(1);
                TransformVectorToInt16(&g_BattleSummonBahamutZeroLightRig, &g_BattleSummonBahamutZeroLightRigPos, g_pBattleBahamutZeroLightRigState);
                pRingRecord->posX = g_pBattleBahamutZeroLightRigState->x + g_BattleSummonBahamutZeroLightRig.bahamutZeroLightRigOffsetX;
                pRingRecord->posY = g_pBattleBahamutZeroLightRigState->y + g_BattleSummonBahamutZeroLightRig.bahamutZeroLightRigOffsetY;
                pRingRecord->posZ = g_pBattleBahamutZeroLightRigState->z + g_BattleSummonBahamutZeroLightRig.bahamutZeroLightRigOffsetZ;
                pRingRecord->effectScale = g_BattleSummonBahamutZeroScaleState.effectScaleCurrent;
                rig->scale = 0x10;
            }
            g_pBattleBahamutZeroLightRigState->z = 0;
            g_pBattleBahamutZeroLightRigState->x = 0;
            g_pBattleBahamutZeroLightRigState->y = 4096;
            if (frameCount > 1) {
                RotMatrix((SVECTOR *)g_pBattleBahamutZeroLightRigState, &g_pBattleBahamutZeroLightRigState->matrix);
                g_pBattleBahamutZeroLightRigState->x = 40;
                g_pBattleBahamutZeroLightRigState->y = 0xFF38;
                g_pBattleBahamutZeroLightRigState->z = 0xFE98;
                TransformVectorToInt16(&g_pBattleBahamutZeroLightRigState->matrix, g_pBattleBahamutZeroLightRigState, g_pBattleBahamutZeroLightRigState);
                (g_BattleSummonBahamutZeroLightRigPos.posX) += g_pBattleBahamutZeroLightRigState->x;
                g_BattleSummonBahamutZeroLightRigPos.posY += g_pBattleBahamutZeroLightRigState->y;
                g_BattleSummonBahamutZeroSpinAngle += g_pBattleBahamutZeroLightRigState->z;
            }
            ratio = (frameCount << 12) / 25;
            rig->scale = (ratio * 0x1F0 >> 12) + 0x10;
        } else {
            frameCount -= 25;
            if (frameCount < 55) {
                subFrame = frameCount;
                if (frameCount == 0) {
                    rig->scale = 0x200 /* 0.125 */;
                }
                if (frameCount < 15) {
                    ratio = (frameCount << 12) / 15;
                    ratio = BattleFx_BzEaseFixed(ratio, 0xFFFFFE01);
                    ratio = (ratio * -0xC00 /* 0.75 */ >> 12) + 0x1000;
                } else {
                    frameCount -= 15;
                    if (frameCount < 20) {
                        ratio = (frameCount << 12) / 20;
                        rig->yaw = g_BattleSummonBahamutZeroScaleState.effectScaleCurrent + ((g_BattleSummonBahamutZeroScaleState.effectScaleCurrent - 0x800 - g_BattleSummonBahamutZeroScaleState.effectScaleCurrent) * ratio >> 12);
                        ratio = 0x400;
                    } else {
                        frameCount -= 20;
                        ratio = (frameCount << 12) / 20;
                        ratio = BattleFx_BzEaseFixed(ratio, 0x1FF);
                        ratio = (ratio * -0x400 /* 0.25 */ >> 12) + 0x400;
                        rig->yaw = g_BattleSummonBahamutZeroScaleState.effectScaleCurrent - 0x800;
                    }
                }
                g_pBattleBahamutZeroLightRigState->z = 0;
                g_pBattleBahamutZeroLightRigState->x = 0;
                g_pBattleBahamutZeroLightRigState->y = rig->yaw - g_BattleSummonBahamutZeroScaleState.effectScaleCurrent;
                RotMatrix((SVECTOR *)g_pBattleBahamutZeroLightRigState, &g_pBattleBahamutZeroLightRigState->matrix);
                g_pBattleBahamutZeroLightRigState->x = 40;
                g_pBattleBahamutZeroLightRigState->y = 0xFF38;
                g_pBattleBahamutZeroLightRigState->z = 0xFE98;
                TransformVectorToInt16(&g_pBattleBahamutZeroLightRigState->matrix, g_pBattleBahamutZeroLightRigState, g_pBattleBahamutZeroLightRigState);
                ratio = (subFrame << 12) / 55;
                ratio = 0x1000 - BattleFx_BzEaseFixed(ratio, 0x2C0);
                (g_BattleSummonBahamutZeroLightRigPos.posX) += (short)(g_pBattleBahamutZeroLightRigState->x * ratio >> 12);
                g_BattleSummonBahamutZeroLightRigPos.posY += (short)(g_pBattleBahamutZeroLightRigState->y * ratio >> 12);
                g_BattleSummonBahamutZeroSpinAngle += (short)(g_pBattleBahamutZeroLightRigState->z * ratio >> 12);
            } else {
                frameCount -= 55;
                if (frameCount < 50) {
                    if (frameCount == 0) {
                        (g_BattleSummonBahamutZeroLightRigPos.posX) = 0;
                        g_BattleSummonBahamutZeroLightRigPos.posY = 0xD9EA;
                        g_BattleSummonBahamutZeroSpinAngle = 0xD6FC;
                        rig->scale = 0x1000 /* 1.0 */;
                    }
                } else {
                    frameCount -= 50;
                    if (frameCount < 120) {
                        if (frameCount == 0) {
                            rig->colorR = 0xFF;
                            rig->colorG = 0xFF;
                            rig->colorB = 0xFF;
                            rig->scale = 0x200 /* 0.125 */;
                        }
                        ratio = (SinFixed((frameCount << 12) / 8) + 0x1000) / 2;
                        rig->rampPhase = ratio << 10 >> 12;
                        if (frameCount == 45) {
                            rig->scale = 0x1000 /* 1.0 */;
                        }
                    } else {
                        frameCount -= 120;
                        if (frameCount < 15) {
                            if (frameCount == 0) {
                                rig->rampPhase = 0;
                            }
                        } else {
                            frameCount -= 15;
                            if (frameCount < 35) {
                                if (frameCount == 0) {
                                    (g_BattleSummonBahamutZeroLightRigPos.posX) = 0;
                                    g_BattleSummonBahamutZeroLightRigPos.posY = 0xD8BE;
                                    g_BattleSummonBahamutZeroSpinAngle = 0xD666;
                                    rig->rotX = 0x180;
                                    rig->scale = 0x200 /* 0.125 */;
                                }
                                if (frameCount == 34) {
                                    Battle_AllocSceneMessage(2);
                                    task->ownerSlot = BATTLE_TASK_FREE;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    TransformVectorToInt16(&g_BattleSummonBahamutZeroLightRig, &g_BattleSummonBahamutZeroLightRigPos, g_pBattleBahamutZeroLightRigState);
    rig->posX = g_pBattleBahamutZeroLightRigState->x + g_BattleSummonBahamutZeroLightRig.bahamutZeroLightRigOffsetX;
    rig->posY = g_pBattleBahamutZeroLightRigState->y + g_BattleSummonBahamutZeroLightRig.bahamutZeroLightRigOffsetY;
    rig->posZ = g_pBattleBahamutZeroLightRigState->z + g_BattleSummonBahamutZeroLightRig.bahamutZeroLightRigOffsetZ;
    (task->frameCounter)++;
}
} /* extern "C" */
