// src/ff7/src/battle/battle_fx_hitmark_effect.cpp
// Contents: Hit-mark battle effect: loads its resources, spawns the script and sprite tasks that
//   step and render the hit marks, and places them at a point or at an actor's bone.

// clang-format off
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/enums/battle.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_fx.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include "ff7/battle.h"
#include "lib/crt.h"
extern "C" {

// clang-format on

// clang-format off
// GLOBAL: FF7 0x007c2ae0
unsigned int g_BattleFxHitMarkSpriteRecordTable[40] ={
    0x00192a39,
    0x00000000,
    (unsigned int)&g_BattleFxHitmarkSpriteName_JO_C01,
    0x00000000,
    0x00000000,
    0x00192a39,
    0x00000000,
    // STRING: FF7 0x007c31e4
    (unsigned int)"JO_C02.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    0x00000000,
    // STRING: FF7 0x007c31f0
    (unsigned int)"JO_C03.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    0x00000000,
    // STRING: FF7 0x007c31fc
    (unsigned int)"JO_C04.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    0x00000000,
    // STRING: FF7 0x007c3208
    (unsigned int)"JO_C05.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    0x00000000,
    // STRING: FF7 0x007c3214
    (unsigned int)"JO_C06.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    0x00000000,
    // STRING: FF7 0x007c3220
    (unsigned int)"JO_C07.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    0x00000000,
    // STRING: FF7 0x007c322c
    (unsigned int)"JO_C08.SPT",
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x007c2b80
int g_BattleFxHitmarkObjectFieldIndexByKind[5] = {0, 1, 2, 3, 4};
// GLOBAL: FF7 0x007c2b94
int g_BattleFxHitmarkType8ObjectFieldIndex = 5;
unsigned char g_rawData_007c2b98[8] = {0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007c2ba0
unsigned char g_BattleFxHitmarkNestedScriptIndex0[7] = {0x00, 0x06, 0x00, 0x00, 0x20, 0x19, 0xff};
// GLOBAL: FF7 0x007c2ba8
unsigned char g_BattleFxHitmarkNestedScriptIndex1[13] = {0x00, 0x06, 0x10, 0x10, 0x20, 0x19, 0x02, 0x06, 0xf0, 0xf0, 0x18, 0x19, 0xff};
// GLOBAL: FF7 0x007c2bb8
unsigned char g_BattleFxHitmarkNestedScriptIndex2[19] = {0x00, 0x06, 0x10, 0x10, 0x20, 0x19, 0x02, 0x06, 0x00, 0x00, 0x18, 0x19, 0x04, 0x06, 0xf0, 0xf0, 0x10, 0x19, 0xff};
// GLOBAL: FF7 0x007c2bd0
unsigned char g_BattleFxHitmarkNestedScriptIndex3[7] = {0x00, 0x02, 0x00, 0x00, 0x40, 0x17, 0xff};
// GLOBAL: FF7 0x007c2bd8
unsigned char g_BattleFxHitmarkNestedScriptIndex4[7] = {0x00, 0x02, 0x00, 0x00, 0x60, 0x17, 0xff};
// GLOBAL: FF7 0x007c2be0
unsigned char g_BattleFxHitmarkNestedScriptIndex5[25] = {0x00, 0x02, 0xe0, 0x10, 0x20, 0x17, 0x02, 0x02, 0x20, 0x00, 0x20, 0x17, 0x04, 0x02, 0x00, 0xe0, 0x20, 0x17, 0x06, 0x02, 0x00, 0x00, 0x20, 0x17, 0xff};
// GLOBAL: FF7 0x007c2c00
unsigned char g_BattleFxHitmarkNestedScriptIndex6[19] = {0x00, 0x02, 0xe0, 0x10, 0x40, 0x17, 0x02, 0x02, 0x20, 0x00, 0x40, 0x17, 0x04, 0x02, 0x00, 0xf0, 0x40, 0x17, 0xff};
// GLOBAL: FF7 0x007c2c18
unsigned char g_BattleFxHitmarkNestedScriptIndex7[19] = {0x00, 0x02, 0xd0, 0x18, 0x60, 0x17, 0x02, 0x02, 0x30, 0x00, 0x60, 0x17, 0x04, 0x02, 0x00, 0xe8, 0x60, 0x17, 0xff};
// GLOBAL: FF7 0x007c2c30
unsigned char g_BattleFxHitmarkNestedScriptIndex8[7] = {0x00, 0x07, 0x00, 0x00, 0x20, 0x15, 0xff};
// GLOBAL: FF7 0x007c2c38
unsigned char g_BattleFxHitmarkNestedScriptIndex9[13] = {0x00, 0x07, 0x10, 0x10, 0x20, 0x15, 0x02, 0x07, 0xf0, 0xf0, 0x18, 0x15, 0xff};
// GLOBAL: FF7 0x007c2c48
unsigned char g_BattleFxHitmarkNestedScriptIndex10[19] = {0x00, 0x07, 0x10, 0x10, 0x20, 0x15, 0x02, 0x07, 0xf0, 0xf0, 0x18, 0x15, 0x04, 0x07, 0xd0, 0xd0, 0x10, 0x15, 0xff};
// GLOBAL: FF7 0x007c2c60
unsigned char g_BattleFxHitmarkNestedScriptIndex11[13] = {0x00, 0x07, 0x10, 0x10, 0x20, 0x15, 0x00, 0x07, 0xf0, 0xf0, 0x18, 0x15, 0xff};
// GLOBAL: FF7 0x007c2c70
unsigned char g_BattleFxHitmarkNestedScriptIndex12[19] = {0x00, 0x07, 0x10, 0x10, 0x20, 0x15, 0x00, 0x07, 0xf0, 0xf0, 0x18, 0x15, 0x00, 0x07, 0xd0, 0xd0, 0x10, 0x15, 0xff};
// GLOBAL: FF7 0x007c2c88
unsigned char g_BattleFxHitmarkNestedScriptIndex13[7] = {0x00, 0x09, 0x00, 0x00, 0x20, 0x19, 0xff};
// GLOBAL: FF7 0x007c2c90
unsigned char g_BattleFxHitmarkNestedScriptIndex14[13] = {0x00, 0x09, 0x00, 0xf0, 0x20, 0x19, 0x02, 0x09, 0x00, 0x10, 0x18, 0x19, 0xff};
// GLOBAL: FF7 0x007c2ca0
unsigned char g_BattleFxHitmarkNestedScriptIndex15[19] = {0x00, 0x09, 0x00, 0xf0, 0x20, 0x19, 0x02, 0x09, 0x08, 0x08, 0x18, 0x19, 0x04, 0x09, 0x10, 0x20, 0x10, 0x19, 0xff};
// GLOBAL: FF7 0x007c2cb8
unsigned char g_BattleFxHitmarkNestedScriptIndex16[19] = {0x00, 0x09, 0x00, 0x00, 0x20, 0x19, 0x00, 0x09, 0x00, 0xe0, 0x18, 0x19, 0x00, 0x09, 0x00, 0x20, 0x18, 0x19, 0xff};
// GLOBAL: FF7 0x007c2cd0
signed char * g_BattleFxHitmarkNestedScriptTable[17] = {(signed char *)&g_BattleFxHitmarkNestedScriptIndex0, (signed char *)&g_BattleFxHitmarkNestedScriptIndex1, (signed char *)&g_BattleFxHitmarkNestedScriptIndex2, (signed char *)&g_BattleFxHitmarkNestedScriptIndex3, (signed char *)&g_BattleFxHitmarkNestedScriptIndex4, (signed char *)&g_BattleFxHitmarkNestedScriptIndex5, (signed char *)&g_BattleFxHitmarkNestedScriptIndex6, (signed char *)&g_BattleFxHitmarkNestedScriptIndex7, (signed char *)&g_BattleFxHitmarkNestedScriptIndex8, (signed char *)&g_BattleFxHitmarkNestedScriptIndex9, (signed char *)&g_BattleFxHitmarkNestedScriptIndex10, (signed char *)&g_BattleFxHitmarkNestedScriptIndex11, (signed char *)&g_BattleFxHitmarkNestedScriptIndex12, (signed char *)&g_BattleFxHitmarkNestedScriptIndex13, (signed char *)&g_BattleFxHitmarkNestedScriptIndex14, (signed char *)&g_BattleFxHitmarkNestedScriptIndex15, (signed char *)&g_BattleFxHitmarkNestedScriptIndex16};
// GLOBAL: FF7 0x007c2d14
int g_BattleFxHitmarkScriptIndex0 = (int)0xFF00FE00;
// GLOBAL: FF7 0x007c2d18
unsigned char g_BattleFxHitmarkScriptIndex1[7] = {0x00, 0xfe, 0x00, 0x06, 0xfe, 0x03, 0xff};
// GLOBAL: FF7 0x007c2d20
unsigned char g_BattleFxHitmarkScriptIndex2[7] = {0x00, 0xfe, 0x00, 0x06, 0xfe, 0x06, 0xff};
// GLOBAL: FF7 0x007c2d28
int g_BattleFxHitmarkScriptIndex3 = (int)0xFF01FE00;
// GLOBAL: FF7 0x007c2d30
unsigned char g_BattleFxHitmarkScriptIndex4[7] = {0x00, 0xfe, 0x01, 0x08, 0xfe, 0x03, 0xff};
// GLOBAL: FF7 0x007c2d38
unsigned char g_BattleFxHitmarkScriptIndex5[7] = {0x00, 0xfe, 0x01, 0x08, 0xfe, 0x06, 0xff};
// GLOBAL: FF7 0x007c2d40
int g_BattleFxHitmarkScriptIndex6 = (int)0xFF02FE00;
// GLOBAL: FF7 0x007c2d48
unsigned char g_BattleFxHitmarkScriptIndex7[7] = {0x00, 0xfe, 0x02, 0x0a, 0xfe, 0x04, 0xff};
// GLOBAL: FF7 0x007c2d50
unsigned char g_BattleFxHitmarkScriptIndex8[7] = {0x00, 0xfe, 0x02, 0x0a, 0xfe, 0x07, 0xff};
// GLOBAL: FF7 0x007c2d58
int g_BattleFxHitmarkScriptIndex9 = (int)0xFF08FE00;
// GLOBAL: FF7 0x007c2d60
unsigned char g_BattleFxHitmarkScriptIndex10[7] = {0x00, 0xfe, 0x08, 0x06, 0xfe, 0x03, 0xff};
// GLOBAL: FF7 0x007c2d68
unsigned char g_BattleFxHitmarkScriptIndex11[7] = {0x00, 0xfe, 0x08, 0x06, 0xfe, 0x06, 0xff};
// GLOBAL: FF7 0x007c2d70
int g_BattleFxHitmarkScriptIndex12 = (int)0xFF09FE00;
// GLOBAL: FF7 0x007c2d78
unsigned char g_BattleFxHitmarkScriptIndex13[7] = {0x00, 0xfe, 0x09, 0x08, 0xfe, 0x03, 0xff};
// GLOBAL: FF7 0x007c2d80
unsigned char g_BattleFxHitmarkScriptIndex14[7] = {0x00, 0xfe, 0x09, 0x08, 0xfe, 0x06, 0xff};
// GLOBAL: FF7 0x007c2d88
int g_BattleFxHitmarkScriptIndex15 = (int)0xFF0AFE00;
// GLOBAL: FF7 0x007c2d90
unsigned char g_BattleFxHitmarkScriptIndex16[7] = {0x00, 0xfe, 0x0a, 0x0a, 0xfe, 0x04, 0xff};
// GLOBAL: FF7 0x007c2d98
unsigned char g_BattleFxHitmarkScriptIndex17[7] = {0x00, 0xfe, 0x0a, 0x0a, 0xfe, 0x07, 0xff};
// GLOBAL: FF7 0x007c2da0
int g_BattleFxHitmarkScriptIndex18 = (int)0xFF0BFE00;
// GLOBAL: FF7 0x007c2da8
unsigned char g_BattleFxHitmarkScriptIndex19[7] = {0x00, 0xfe, 0x0b, 0x06, 0xfe, 0x03, 0xff};
// GLOBAL: FF7 0x007c2db0
unsigned char g_BattleFxHitmarkScriptIndex20[7] = {0x00, 0xfe, 0x0b, 0x06, 0xfe, 0x06, 0xff};
// GLOBAL: FF7 0x007c2db8
int g_BattleFxHitmarkScriptIndex21 = (int)0xFF0CFE00;
// GLOBAL: FF7 0x007c2dc0
unsigned char g_BattleFxHitmarkScriptIndex22[7] = {0x00, 0xfe, 0x0c, 0x06, 0xfe, 0x04, 0xff};
// GLOBAL: FF7 0x007c2dc8
unsigned char g_BattleFxHitmarkScriptIndex23[7] = {0x00, 0xfe, 0x0c, 0x06, 0xfe, 0x07, 0xff};
// GLOBAL: FF7 0x007c2dd0
int g_BattleFxHitmarkScriptIndex24 = (int)0xFF0DFE00;
// GLOBAL: FF7 0x007c2dd8
unsigned char g_BattleFxHitmarkScriptIndex25[7] = {0x00, 0xfe, 0x0d, 0x06, 0xfe, 0x03, 0xff};
// GLOBAL: FF7 0x007c2de0
unsigned char g_BattleFxHitmarkScriptIndex26[7] = {0x00, 0xfe, 0x0d, 0x06, 0xfe, 0x06, 0xff};
// GLOBAL: FF7 0x007c2de8
int g_BattleFxHitmarkScriptIndex27 = (int)0xFF0EFE00;
// GLOBAL: FF7 0x007c2df0
unsigned char g_BattleFxHitmarkScriptIndex28[7] = {0x00, 0xfe, 0x0e, 0x08, 0xfe, 0x03, 0xff};
// GLOBAL: FF7 0x007c2df8
unsigned char g_BattleFxHitmarkScriptIndex29[7] = {0x00, 0xfe, 0x0e, 0x08, 0xfe, 0x06, 0xff};
// GLOBAL: FF7 0x007c2e00
int g_BattleFxHitmarkScriptIndex30 = (int)0xFF0FFE00;
// GLOBAL: FF7 0x007c2e08
unsigned char g_BattleFxHitmarkScriptIndex31[7] = {0x00, 0xfe, 0x0f, 0x0a, 0xfe, 0x04, 0xff};
// GLOBAL: FF7 0x007c2e10
unsigned char g_BattleFxHitmarkScriptIndex32[7] = {0x00, 0xfe, 0x0f, 0x0a, 0xfe, 0x07, 0xff};
// GLOBAL: FF7 0x007c2e18
int g_BattleFxHitmarkScriptIndex33 = (int)0xFF10FE00;
// GLOBAL: FF7 0x007c2e20
unsigned char g_BattleFxHitmarkScriptIndex34[7] = {0x00, 0xfe, 0x10, 0x06, 0xfe, 0x04, 0xff};
// GLOBAL: FF7 0x007c2e28
unsigned char g_BattleFxHitmarkScriptIndex35[7] = {0x00, 0xfe, 0x10, 0x06, 0xfe, 0x07, 0xff};
// GLOBAL: FF7 0x007c2e30
unsigned char g_BattleFxHitmarkScriptIndex36[7] = {0x00, 0x01, 0x00, 0x00, 0x20, 0x18, 0xff};
// GLOBAL: FF7 0x007c2e38
unsigned char g_BattleFxHitmarkScriptIndex37[7] = {0x00, 0x01, 0x00, 0x00, 0x40, 0x18, 0xff};
// GLOBAL: FF7 0x007c2e40
unsigned char g_BattleFxHitmarkScriptIndex38[7] = {0x00, 0x01, 0x00, 0x00, 0x60, 0x18, 0xff};
// GLOBAL: FF7 0x007c2e48
unsigned char g_BattleFxHitmarkScriptIndex39[25] = {0x00, 0x01, 0xe0, 0x10, 0x20, 0x18, 0x02, 0x01, 0x20, 0x00, 0x20, 0x18, 0x04, 0x01, 0x00, 0xe0, 0x20, 0x18, 0x06, 0x01, 0x00, 0x00, 0x20, 0x18, 0xff};
// GLOBAL: FF7 0x007c2e68
unsigned char g_BattleFxHitmarkScriptIndex40[19] = {0x00, 0x01, 0xe0, 0x10, 0x40, 0x18, 0x02, 0x01, 0x20, 0x00, 0x40, 0x18, 0x04, 0x01, 0x00, 0xf0, 0x40, 0x18, 0xff};
// GLOBAL: FF7 0x007c2e80
unsigned char g_BattleFxHitmarkScriptIndex41[19] = {0x00, 0x01, 0xe0, 0x10, 0x60, 0x18, 0x02, 0x01, 0x20, 0x00, 0x60, 0x18, 0x04, 0x01, 0x00, 0xf0, 0x60, 0x18, 0xff};
// GLOBAL: FF7 0x007c2e98
unsigned char g_BattleFxHitmarkScriptIndex42[7] = {0x00, 0x02, 0x00, 0x00, 0x20, 0x17, 0xff};
// GLOBAL: FF7 0x007c2ea0
int g_BattleFxHitmarkScriptIndex43 = (int)0xFF03FE00;
// GLOBAL: FF7 0x007c2ea4
int g_BattleFxHitmarkScriptIndex44 = (int)0xFF04FE00;
// GLOBAL: FF7 0x007c2ea8
unsigned char g_BattleFxHitmarkScriptIndex45[25] = {0x00, 0x02, 0xe0, 0x10, 0x20, 0x17, 0x02, 0x02, 0x20, 0x00, 0x20, 0x17, 0x04, 0x02, 0x00, 0xe0, 0x20, 0x17, 0x06, 0x02, 0x00, 0x00, 0x20, 0x17, 0xff};
// GLOBAL: FF7 0x007c2ec8
unsigned char g_BattleFxHitmarkScriptIndex46[19] = {0x00, 0x02, 0xe0, 0x10, 0x40, 0x17, 0x02, 0x02, 0x20, 0x00, 0x40, 0x17, 0x04, 0x02, 0x00, 0xf0, 0x40, 0x17, 0xff};
// GLOBAL: FF7 0x007c2ee0
unsigned char g_BattleFxHitmarkScriptIndex47[19] = {0x00, 0x02, 0xd0, 0x18, 0x60, 0x17, 0x02, 0x02, 0x30, 0x00, 0x60, 0x17, 0x04, 0x02, 0x00, 0xe8, 0x60, 0x17, 0xff};
// GLOBAL: FF7 0x007c2ef8
unsigned char g_BattleFxHitmarkScriptIndex48[7] = {0x00, 0x03, 0x00, 0x00, 0x20, 0x16, 0xff};
// GLOBAL: FF7 0x007c2f00
unsigned char g_BattleFxHitmarkScriptIndex49[7] = {0x00, 0x03, 0x00, 0x00, 0x40, 0x16, 0xff};
// GLOBAL: FF7 0x007c2f08
unsigned char g_BattleFxHitmarkScriptIndex50[7] = {0x00, 0x03, 0x00, 0x00, 0x60, 0x16, 0xff};
// GLOBAL: FF7 0x007c2f10
unsigned char g_BattleFxHitmarkScriptIndex51[25] = {0x00, 0x03, 0xe0, 0x10, 0x20, 0x16, 0x02, 0x03, 0x20, 0x00, 0x20, 0x16, 0x04, 0x03, 0x00, 0xe0, 0x20, 0x16, 0x06, 0x03, 0x00, 0x00, 0x20, 0x16, 0xff};
// GLOBAL: FF7 0x007c2f30
unsigned char g_BattleFxHitmarkScriptIndex52[19] = {0x00, 0x03, 0xe0, 0x10, 0x40, 0x16, 0x02, 0x03, 0x20, 0x00, 0x40, 0x16, 0x04, 0x03, 0x00, 0xf0, 0x40, 0x16, 0xff};
// GLOBAL: FF7 0x007c2f48
unsigned char g_BattleFxHitmarkScriptIndex53[19] = {0x00, 0x03, 0xe0, 0x10, 0x60, 0x16, 0x02, 0x03, 0x20, 0x00, 0x60, 0x16, 0x04, 0x03, 0x00, 0xf0, 0x60, 0x16, 0xff};
// GLOBAL: FF7 0x007c2f60
unsigned char g_BattleFxHitmarkScriptIndex54[7] = {0x00, 0x04, 0x00, 0x00, 0x20, 0x14, 0xff};
// GLOBAL: FF7 0x007c2f68
unsigned char g_BattleFxHitmarkScriptIndex55[7] = {0x00, 0x04, 0x00, 0x00, 0x40, 0x14, 0xff};
// GLOBAL: FF7 0x007c2f70
unsigned char g_BattleFxHitmarkScriptIndex56[7] = {0x00, 0x04, 0x00, 0x00, 0x60, 0x14, 0xff};
// GLOBAL: FF7 0x007c2f78
unsigned char g_BattleFxHitmarkScriptIndex57[25] = {0x00, 0x04, 0xe0, 0x10, 0x20, 0x14, 0x02, 0x04, 0x20, 0x00, 0x20, 0x14, 0x04, 0x04, 0x00, 0xe0, 0x20, 0x14, 0x06, 0x04, 0x00, 0x00, 0x20, 0x14, 0xff};
// GLOBAL: FF7 0x007c2f98
unsigned char g_BattleFxHitmarkScriptIndex58[19] = {0x00, 0x04, 0xe0, 0x10, 0x40, 0x14, 0x02, 0x04, 0x20, 0x00, 0x40, 0x14, 0x04, 0x04, 0x00, 0xf0, 0x40, 0x14, 0xff};
// GLOBAL: FF7 0x007c2fb0
unsigned char g_BattleFxHitmarkScriptIndex59[19] = {0x00, 0x04, 0xe0, 0x10, 0x60, 0x14, 0x02, 0x04, 0x20, 0x00, 0x60, 0x14, 0x04, 0x04, 0x00, 0xf0, 0x60, 0x14, 0xff};
// GLOBAL: FF7 0x007c2fc8
unsigned char g_BattleFxHitmarkScriptIndex60[7] = {0x00, 0x05, 0x00, 0x00, 0x20, 0x13, 0xff};
// GLOBAL: FF7 0x007c2fd0
unsigned char g_BattleFxHitmarkScriptIndex61[7] = {0x00, 0x05, 0x00, 0x00, 0x40, 0x13, 0xff};
// GLOBAL: FF7 0x007c2fd8
unsigned char g_BattleFxHitmarkScriptIndex62[7] = {0x00, 0x05, 0x00, 0x00, 0x60, 0x13, 0xff};
// GLOBAL: FF7 0x007c2fe0
unsigned char g_BattleFxHitmarkScriptIndex63[25] = {0x00, 0x05, 0xe0, 0x10, 0x20, 0x13, 0x02, 0x05, 0x20, 0x00, 0x20, 0x13, 0x04, 0x05, 0x00, 0xe0, 0x20, 0x13, 0x06, 0x05, 0x00, 0x00, 0x20, 0x13, 0xff};
// GLOBAL: FF7 0x007c3000
unsigned char g_BattleFxHitmarkScriptIndex64[19] = {0x00, 0x05, 0xe0, 0x10, 0x40, 0x13, 0x02, 0x05, 0x20, 0x00, 0x40, 0x13, 0x04, 0x05, 0x00, 0xf0, 0x40, 0x13, 0xff};
// GLOBAL: FF7 0x007c3018
unsigned char g_BattleFxHitmarkScriptIndex65[19] = {0x00, 0x05, 0xe0, 0x10, 0x60, 0x13, 0x02, 0x05, 0x20, 0x00, 0x60, 0x13, 0x04, 0x05, 0x00, 0xf0, 0x60, 0x13, 0xff};
// GLOBAL: FF7 0x007c3030
unsigned char g_BattleFxHitmarkScriptIndex66[7] = {0x00, 0x08, 0x00, 0x00, 0x20, 0x1a, 0xff};
// GLOBAL: FF7 0x007c3038
unsigned char g_BattleFxHitmarkScriptIndex67[7] = {0x00, 0x08, 0x00, 0x00, 0x40, 0x1a, 0xff};
// GLOBAL: FF7 0x007c3040
unsigned char g_BattleFxHitmarkScriptIndex68[7] = {0x00, 0x08, 0x00, 0x00, 0x60, 0x1a, 0xff};
// GLOBAL: FF7 0x007c3048
unsigned char g_BattleFxHitmarkScriptIndex69[25] = {0x00, 0x08, 0xe0, 0x10, 0x20, 0x1a, 0x02, 0x08, 0x20, 0x00, 0x20, 0x1a, 0x04, 0x08, 0x00, 0xe0, 0x20, 0x1a, 0x06, 0x08, 0x00, 0x00, 0x20, 0x1a, 0xff};
// GLOBAL: FF7 0x007c3068
unsigned char g_BattleFxHitmarkScriptIndex70[19] = {0x00, 0x08, 0xe0, 0x10, 0x40, 0x1a, 0x02, 0x08, 0x20, 0x00, 0x40, 0x1a, 0x04, 0x08, 0x00, 0xf0, 0x40, 0x1a, 0xff};
unsigned char g_BattleFxHitmarkEffectDataPool[24] = {0x00, 0x08, 0xe0, 0x10, 0x60, 0x1a, 0x02, 0x08, 0x20, 0x00, 0x60, 0x1a, 0x04, 0x08, 0x00, 0xf0, 0x60, 0x1a, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007c3098
unsigned int g_BattleFxHitmarkScriptTable[71] = {(unsigned int)&g_BattleFxHitmarkScriptIndex0, (unsigned int)((char *)&g_BattleFxHitmarkScriptIndex0 + 4), (unsigned int)&g_BattleFxHitmarkScriptIndex2, (unsigned int)&g_BattleFxHitmarkScriptIndex3, (unsigned int)&g_BattleFxHitmarkScriptIndex4, (unsigned int)&g_BattleFxHitmarkScriptIndex5, (unsigned int)&g_BattleFxHitmarkScriptIndex6, (unsigned int)&g_BattleFxHitmarkScriptIndex7, (unsigned int)&g_BattleFxHitmarkScriptIndex8, (unsigned int)&g_BattleFxHitmarkScriptIndex9, (unsigned int)&g_BattleFxHitmarkScriptIndex10, (unsigned int)&g_BattleFxHitmarkScriptIndex11, (unsigned int)&g_BattleFxHitmarkScriptIndex12, (unsigned int)&g_BattleFxHitmarkScriptIndex13, (unsigned int)&g_BattleFxHitmarkScriptIndex14, (unsigned int)&g_BattleFxHitmarkScriptIndex15, (unsigned int)&g_BattleFxHitmarkScriptIndex16, (unsigned int)&g_BattleFxHitmarkScriptIndex17, (unsigned int)&g_BattleFxHitmarkScriptIndex18, (unsigned int)&g_BattleFxHitmarkScriptIndex19, (unsigned int)&g_BattleFxHitmarkScriptIndex20, (unsigned int)&g_BattleFxHitmarkScriptIndex21, (unsigned int)&g_BattleFxHitmarkScriptIndex22, (unsigned int)&g_BattleFxHitmarkScriptIndex23, (unsigned int)&g_BattleFxHitmarkScriptIndex24, (unsigned int)&g_BattleFxHitmarkScriptIndex25, (unsigned int)&g_BattleFxHitmarkScriptIndex26, (unsigned int)&g_BattleFxHitmarkScriptIndex27, (unsigned int)&g_BattleFxHitmarkScriptIndex28, (unsigned int)&g_BattleFxHitmarkScriptIndex29, (unsigned int)&g_BattleFxHitmarkScriptIndex30, (unsigned int)&g_BattleFxHitmarkScriptIndex31, (unsigned int)&g_BattleFxHitmarkScriptIndex32, (unsigned int)&g_BattleFxHitmarkScriptIndex33, (unsigned int)&g_BattleFxHitmarkScriptIndex34, (unsigned int)&g_BattleFxHitmarkScriptIndex35, (unsigned int)&g_BattleFxHitmarkScriptIndex36, (unsigned int)&g_BattleFxHitmarkScriptIndex37, (unsigned int)&g_BattleFxHitmarkScriptIndex38, (unsigned int)&g_BattleFxHitmarkScriptIndex39, (unsigned int)&g_BattleFxHitmarkScriptIndex40, (unsigned int)&g_BattleFxHitmarkScriptIndex41, (unsigned int)&g_BattleFxHitmarkScriptIndex42, (unsigned int)&g_BattleFxHitmarkScriptIndex43, (unsigned int)((char *)&g_BattleFxHitmarkScriptIndex43 + 4), (unsigned int)((char *)&g_BattleFxHitmarkScriptIndex43 + 8), (unsigned int)&g_BattleFxHitmarkScriptIndex46, (unsigned int)&g_BattleFxHitmarkScriptIndex47, (unsigned int)&g_BattleFxHitmarkScriptIndex48, (unsigned int)&g_BattleFxHitmarkScriptIndex49, (unsigned int)&g_BattleFxHitmarkScriptIndex50, (unsigned int)&g_BattleFxHitmarkScriptIndex51, (unsigned int)&g_BattleFxHitmarkScriptIndex52, (unsigned int)&g_BattleFxHitmarkScriptIndex53, (unsigned int)&g_BattleFxHitmarkScriptIndex54, (unsigned int)&g_BattleFxHitmarkScriptIndex55, (unsigned int)&g_BattleFxHitmarkScriptIndex56, (unsigned int)&g_BattleFxHitmarkScriptIndex57, (unsigned int)&g_BattleFxHitmarkScriptIndex58, (unsigned int)&g_BattleFxHitmarkScriptIndex59, (unsigned int)&g_BattleFxHitmarkScriptIndex60, (unsigned int)&g_BattleFxHitmarkScriptIndex61, (unsigned int)&g_BattleFxHitmarkScriptIndex62, (unsigned int)&g_BattleFxHitmarkScriptIndex63, (unsigned int)&g_BattleFxHitmarkScriptIndex64, (unsigned int)&g_BattleFxHitmarkScriptIndex65, (unsigned int)&g_BattleFxHitmarkScriptIndex66, (unsigned int)&g_BattleFxHitmarkScriptIndex67, (unsigned int)&g_BattleFxHitmarkScriptIndex68, (unsigned int)&g_BattleFxHitmarkScriptIndex69, (unsigned int)&g_BattleFxHitmarkScriptIndex70};
int g_BattleFxHitmarkScriptTableHeadPtr[1] = {
    (int)((char *)&g_BattleFxHitmarkEffectDataPool + 0x0)
};
// GLOBAL: FF7 0x007c31b8
unsigned char g_BattleFxSpriteTransformMatrixM00[8] = {0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007c31c0
unsigned char g_BattleFxSpriteTransformMatrixM11[8] = {0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007c31c8
unsigned char g_BattleFxSpriteTransformMatrixM22[14] = {0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007c31d8
unsigned char g_BattleFxHitmarkSpriteName_JO_C01[11] = "JO_C01.SPT";
// clang-format on


// clang-format off
// GLOBAL: FF7 0x009aeae8
struct GfxTableCursor *g_BattleFxHitmarkResource = 0;
// clang-format on

/*
 * Loads the EFFECT/HITMARK model table into g_BattleFxHitmarkResource when the battle scene exists and
 * the table is not loaded yet.
 */
// FUNCTION: FF7 0x00438610
void __cdecl BattleFx_EffectLoadResHitMark(void)
{
    GfxModelTableBank *sceneContext;
    GfxResourceRequest record;
    int resultId;
    char pathOut[200];
    GfxResourceRequest *request;
    int descriptor;

    if (g_pBattle3dScene != 0 && g_BattleFxHitmarkResource == 0) {
        resultId = 8;
        descriptor = (int)&g_BattleFxHitMarkSpriteRecordTable;
        sceneContext = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &record;
        Gfx_InitResourceRequest(request);
        request->groupTag = 1;
        // STRING: FF7 0x007c3238
        (request->resourcePath) = BattleFx_BuildResourcePath("EFFECT/HITMARK", pathOut);
        Gfx_LoadModelTableIfAbsent(resultId, descriptor, request, &g_BattleFxHitmarkResource, sceneContext);
    }
}

// Allocates a secondary HitMark step-script slot and fills it from the arguments. Its flags
// get scriptSelector & 0xFF00, and its scriptCursor gets the g_BattleFxHitmarkScriptTable entry
// selected by the low byte. The eight bytes at pPosition are copied to its position, param3 is
// its scale and param4 its pushLength.
// FUNCTION: FF7 0x004386a0
void __cdecl BattleFx_HitMarkSpawnScript(void *pPosition, int scriptSelector, int scale, int pushLength)
{
    BattleFxHitMarkScriptSlot *task;

    task = (BattleFxHitMarkScriptSlot *)(&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_HitMarkStepScriptSlot)]);
    task->flags = scriptSelector & 0xFF00;
    task->scriptCursor = ((signed char **)&g_BattleFxHitmarkScriptTable)[scriptSelector & 0xFF];
    *(Int64Pair *)task->position = *(Int64Pair *)pPosition;
    task->scale = scale;
    task->pushLength = pushLength;
}

/*
 * Steps the hitmark script execution slot. Parses script bytecodes, handles nested scripts,
 * spawns sprites via BattleFx_HitMarkSpawnSprite when frame counters match, and retires completed slots.
 */
// FUNCTION: FF7 0x0043870e
void __cdecl BattleFx_HitMarkStepScriptSlot(void)
{
    int keepGoing;
    signed char entDelay;
    signed char *nestedPtr;
    signed char *cursorPtr;
    BattleFxHitMarkScriptSlot *taskSlotPtr;
    signed char nestedDelay;
    int v;
    taskSlotPtr = (BattleFxHitMarkScriptSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    if (g_BattleIsPaused != 0)
        return;
    cursorPtr = taskSlotPtr->scriptCursor;
    keepGoing = 0;
    while ((entDelay = *cursorPtr++) != -1) {
        if (*cursorPtr != -2) {
            if (entDelay == taskSlotPtr->frame) {
                BattleFx_HitMarkSpawnSprite((char *)cursorPtr, taskSlotPtr->position, taskSlotPtr);
            } else if (entDelay > taskSlotPtr->frame) {
                keepGoing = 1;
            }
            cursorPtr += 5;
        } else {
            nestedPtr = g_BattleFxHitmarkNestedScriptTable[cursorPtr[1]];
            cursorPtr += 2;
            while ((nestedDelay = *nestedPtr++) != -1) {
                if (entDelay + nestedDelay == taskSlotPtr->frame) {
                    BattleFx_HitMarkSpawnSprite((char *)nestedPtr, taskSlotPtr->position, taskSlotPtr);
                } else if (entDelay + nestedDelay > taskSlotPtr->frame) {
                    keepGoing = 1;
                }
                nestedPtr += 5;
            }
        }
    }
    if (keepGoing == 0)
        taskSlotPtr->flags = BATTLE_TASK_FREE;
    (taskSlotPtr->frame)++;
}

/*
 * Allocates a secondary BattleFx_HitMarkSpriteTask slot with a copy of the parent's position, combines
 * the first signed script byte minus one with the parent's flags, and derives the X/Y offsets and scale
 * from the following script bytes scaled by the parent's scale.
 */
// FUNCTION: FF7 0x0043886e
void __cdecl BattleFx_HitMarkSpawnSprite(char *pScriptPayload, void *pPosition, BattleFxHitMarkScriptSlot *pParentTask)
{
    BattleFxHitMarkSpriteSlot *sprite;
    int scaledBytes;
    int clamped;

    sprite = (BattleFxHitMarkSpriteSlot *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_HitMarkSpriteTask)];
    *(Int64Pair *)sprite->position = *(Int64Pair *)pPosition;
    sprite->flags = *pScriptPayload - 1 | pParentTask->flags;
    pScriptPayload++;
    if ((pParentTask->flags & 0x100) != 0) {
        sprite->offsetX = -(*pScriptPayload << 3) * pParentTask->scale >> 0xC;
        pScriptPayload++;
    } else {
        sprite->offsetX = (*pScriptPayload << 3) * pParentTask->scale >> 0xC;
        pScriptPayload++;
    }
    sprite->offsetY = (*pScriptPayload << 3) * pParentTask->scale >> 0xC;
    pScriptPayload++;
    sprite->pushLength = pParentTask->pushLength;
    scaledBytes = (*pScriptPayload << 8) * pParentTask->scale >> 0xC;
    pScriptPayload++;
    if (scaledBytes <= 0x7FFF) {
        clamped = scaledBytes;
    } else {
        clamped = 0x7FFF;
    }
    sprite->scale = clamped;
    sprite->clutWord = *pScriptPayload << 6;
    pScriptPayload++;
}

/*
 * Renders an active hitmark sprite quad in view space with transform and color attributes.
 * Advances sprite animation frames while unpaused and retires upon reaching end frame.
 */
// FUNCTION: FF7 0x004389b6
void __cdecl BattleFx_HitMarkSpriteTask(void)
{
    Vec3sPacked *mtx;
    BattleFxHitMarkSpriteSlot *taskPtr;
    int markKind;
    BattleFxHitMarkSprite *spriteQuad;

    taskPtr = (BattleFxHitMarkSpriteSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    spriteQuad = (BattleFxHitMarkSprite *)Gfx_GetPsxScratchpadPtr(0);
    spriteQuad->colorCode = 0x2C808080;
    spriteQuad->frame = (short)(taskPtr->frame | 0x8000);
    spriteQuad->clutWord = taskPtr->clutWord;
    (*(short *)&g_BattleFxSpriteTransformMatrixM22) = taskPtr->scale;
    (*(short *)&g_BattleFxSpriteTransformMatrixM11) = (*(short *)&g_BattleFxSpriteTransformMatrixM22);
    (*(short *)&g_BattleFxSpriteTransformMatrixM00) = (*(short *)&g_BattleFxSpriteTransformMatrixM11);
    markKind = taskPtr->flags & 0xFF;
    if (markKind != 8) {
        spriteQuad->hitmarkObject = Gfx_ModelTableGetLoadedRecord(g_BattleFxHitmarkObjectFieldIndexByKind[markKind], g_BattleFxHitmarkResource);
        spriteQuad->sceneObject = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
        mtx = (Vec3sPacked *)&g_BattleFxSpriteTransformMatrixM00;
    } else {
        spriteQuad->hitmarkObject = Gfx_ModelTableGetLoadedRecord(g_BattleFxHitmarkType8ObjectFieldIndex, g_BattleFxHitmarkResource);
        spriteQuad->sceneObject = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
        mtx = (Vec3sPacked *)((int)spriteQuad + 0x10);
        *((PsxMatrix *)((int)mtx)) = *((PsxMatrix *)&g_BattleFxSpriteTransformMatrixM00);
        RotateMatrixZ(0x200, mtx);
    }
    if ((taskPtr->flags & 0x100) != 0) {
        mtx->vx = -mtx->vx;
        mtx->vy = -mtx->vy;
        mtx->vz = -mtx->vz;
    }
    BattleFx_MakeViewSpacePointPushed(taskPtr->position, taskPtr->pushLength, mtx);
    *(int *)((char *)mtx + 0x12) += taskPtr->offsetX;
    *(int *)((char *)mtx + 0x16) += taskPtr->offsetY;
    SetRotMatrix((void *)mtx);
    SetTransMatrix(mtx);
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(spriteQuad, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0 && spriteQuad->hitmarkObject != 0) {
        (taskPtr->frame)++;
        if (taskPtr->frame >= *(int *)((char *)*(int *)spriteQuad->hitmarkObject + 4)) {
            taskPtr->flags = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Queries world position for an actor bone and spawns a hitmark script at that bone.
 */
// FUNCTION: FF7 0x00438be4
void __cdecl BattleFx_HitMarkSpawnScriptAtActorBone(int actorIdx, int scriptSelector, int scale)
{
    int val[2];

    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].targetBone, (short *)val);
    BattleFx_HitMarkSpawnScript(val, scriptSelector, scale, -g_BattleActorRecords[actorIdx].radius);
}

/*
 * Ensures hitmark resources are loaded and spawns a hitmark script at the actor bone with default scale (0x1000).
 */
// FUNCTION: FF7 0x00438c36
void __cdecl BattleFx_HitMarkSpawnAtActorBone(int actorIdx, int scriptSelector)
{
    BattleFx_EffectLoadResHitMark();
    BattleFx_HitMarkSpawnScriptAtActorBone(actorIdx, scriptSelector, 0x1000);
}

/*
 * Ensures hitmark resources are loaded and spawns a hitmark script at the given point with default scale (0x1000).
 */
// FUNCTION: FF7 0x00438c55
void __cdecl BattleFx_HitMarkSpawnAtPoint(void *pPosition, int scriptSelector, int zOffset)
{
    BattleFx_EffectLoadResHitMark();
    BattleFx_HitMarkSpawnScript(pPosition, scriptSelector, 0x1000, zOffset);
}
} /* extern "C" */
