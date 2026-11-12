// src/ff7/src/battle/battle_fx_summon_bahamut.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Bahamut summon effect: loads its resources, runs its camera script, and composes the
//   timeline transform. Its tasks fade and grow billboard sprites and models, rotate and expand
//   model pairs, draw streak lines and an extending beam, and handle the actor descent.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_summon.h"
#include "lib/types/graphics.h"
#include "ff7/enums/battle.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <stdlib.h>
#include <string.h>
#include "lib/api/sound.h"
#include "lib/codegen.h"
extern "C" {

#pragma intrinsic(memset)

// clang-format off
// GLOBAL: FF7 0x007fbcd8
unsigned char g_BattleSummonBahamutSoundGroup1[16] = {0x7c, 0x02, 0x00, 0x00, 0x7d, 0x02, 0x00, 0x00, 0x7e, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007fbce8
unsigned char g_BattleSummonBahamutSoundGroup2[16] = {0x7f, 0x02, 0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0x81, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007fbcf8
unsigned char g_BattleSummonBahamutSoundGroup3[16] = {0x82, 0x02, 0x00, 0x00, 0x83, 0x02, 0x00, 0x00, 0x84, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007fbd08
unsigned char g_BattleSummonBahamutSoundGroup4[16] = {0x85, 0x02, 0x00, 0x00, 0x86, 0x02, 0x00, 0x00, 0x87, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007fbd18
unsigned int g_BattleSummonBahamutSoundGroupPointers[5] = {(unsigned int)&g_BattleSummonBahamutSoundGroup1, (unsigned int)&g_BattleSummonBahamutSoundGroup2, (unsigned int)&g_BattleSummonBahamutSoundGroup3, (unsigned int)&g_BattleSummonBahamutSoundGroup4, 0x00000000};
// GLOBAL: FF7 0x007fbd30
unsigned char g_BattleSummonBahamutPolygonVertexBlob[2312] = {
0x2a, 0xf9, 0x01, 0x00, 0x2a, 0xf9, 0x00, 0x00, 0x2a, 0xf9, 0x01, 0x00, 0x24, 0xfa, 0x00, 0x00,
0x24, 0xfa, 0x01, 0x00, 0x24, 0xfa, 0x00, 0x00, 0x24, 0xfa, 0x01, 0x00, 0x2a, 0xf9, 0x00, 0x00,
0x2a, 0xf9, 0x01, 0x00, 0x1e, 0xfb, 0x00, 0x00, 0x24, 0xfa, 0x01, 0x00, 0x1e, 0xfb, 0x00, 0x00,
0x2a, 0xf9, 0x01, 0x00, 0x18, 0xfc, 0x00, 0x00, 0x24, 0xfa, 0x01, 0x00, 0x18, 0xfc, 0x00, 0x00,
0x2a, 0xf9, 0x01, 0x00, 0x12, 0xfd, 0x00, 0x00, 0x24, 0xfa, 0xf2, 0xff, 0x12, 0xfd, 0x00, 0x00,
0x2a, 0xf9, 0x01, 0x00, 0x0c, 0xfe, 0x00, 0x00, 0x24, 0xfa, 0xbe, 0xff, 0x0c, 0xfe, 0x00, 0x00,
0x2a, 0xf9, 0x01, 0x00, 0x06, 0xff, 0x00, 0x00, 0x24, 0xfa, 0x8b, 0xff, 0x06, 0xff, 0x00, 0x00,
0x2a, 0xf9, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0xfa, 0x76, 0xff, 0x00, 0x00, 0x00, 0x00,
0x2a, 0xf9, 0x01, 0x00, 0xfa, 0x00, 0x00, 0x00, 0x24, 0xfa, 0x8b, 0xff, 0xfa, 0x00, 0x00, 0x00,
0x2a, 0xf9, 0x01, 0x00, 0xf4, 0x01, 0x00, 0x00, 0x24, 0xfa, 0xbe, 0xff, 0xf4, 0x01, 0x00, 0x00,
0x2a, 0xf9, 0x01, 0x00, 0xe8, 0x03, 0x00, 0x00, 0x2a, 0xf9, 0x01, 0x00, 0xe2, 0x04, 0x00, 0x00,
0x24, 0xfa, 0x01, 0x00, 0xe2, 0x04, 0x00, 0x00, 0x24, 0xfa, 0x01, 0x00, 0xe8, 0x03, 0x00, 0x00,
0x2a, 0xf9, 0x01, 0x00, 0xdc, 0x05, 0x00, 0x00, 0x24, 0xfa, 0x01, 0x00, 0xdc, 0x05, 0x00, 0x00,
0x2a, 0xf9, 0x01, 0x00, 0xee, 0x02, 0x00, 0x00, 0x24, 0xfa, 0xf2, 0xff, 0xee, 0x02, 0x00, 0x00,
0x2a, 0xf9, 0x01, 0x00, 0xd6, 0x06, 0x00, 0x00, 0x24, 0xfa, 0x01, 0x00, 0xd6, 0x06, 0x00, 0x00,
0x2a, 0xf9, 0x01, 0x00, 0x30, 0xf8, 0x00, 0x00, 0x24, 0xfa, 0x01, 0x00, 0x30, 0xf8, 0x00, 0x00,
0x2a, 0xf9, 0x01, 0x00, 0xd0, 0x07, 0x00, 0x00, 0x24, 0xfa, 0x01, 0x00, 0xd0, 0x07, 0x00, 0x00,
0x1e, 0xfb, 0x01, 0x00, 0x2a, 0xf9, 0x00, 0x00, 0x1e, 0xfb, 0x01, 0x00, 0x30, 0xf8, 0x00, 0x00,
0x18, 0xfc, 0x01, 0x00, 0x30, 0xf8, 0x00, 0x00, 0x18, 0xfc, 0x01, 0x00, 0x2a, 0xf9, 0x00, 0x00,
0x1e, 0xfb, 0x01, 0x00, 0x24, 0xfa, 0x00, 0x00, 0x18, 0xfc, 0x01, 0x00, 0x24, 0xfa, 0x00, 0x00,
0x1e, 0xfb, 0x01, 0x00, 0x1e, 0xfb, 0x00, 0x00, 0x18, 0xfc, 0xcb, 0xff, 0x1e, 0xfb, 0x00, 0x00,
0x1e, 0xfb, 0xcb, 0xff, 0x18, 0xfc, 0x00, 0x00, 0x18, 0xfc, 0x12, 0xff, 0x18, 0xfc, 0x00, 0x00,
0x1e, 0xfb, 0x48, 0xff, 0x12, 0xfd, 0x00, 0x00, 0x18, 0xfc, 0x11, 0xfe, 0x12, 0xfd, 0x00, 0x00,
0x1e, 0xfb, 0xb1, 0xfe, 0x0c, 0xfe, 0x00, 0x00, 0x18, 0xfc, 0x14, 0xfd, 0x0c, 0xfe, 0x00, 0x00,
0x1e, 0xfb, 0x3d, 0xfe, 0x06, 0xff, 0x00, 0x00, 0x18, 0xfc, 0x5b, 0xfc, 0x06, 0xff, 0x00, 0x00,
0x1e, 0xfb, 0x11, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x18, 0xfc, 0x18, 0xfc, 0x00, 0x00, 0x00, 0x00,
0x1e, 0xfb, 0x3d, 0xfe, 0xfa, 0x00, 0x00, 0x00, 0x18, 0xfc, 0x5b, 0xfc, 0xfa, 0x00, 0x00, 0x00,
0x1e, 0xfb, 0xb1, 0xfe, 0xf4, 0x01, 0x00, 0x00, 0x18, 0xfc, 0x14, 0xfd, 0xf4, 0x01, 0x00, 0x00,
0x1e, 0xfb, 0x01, 0x00, 0xe2, 0x04, 0x00, 0x00, 0x1e, 0xfb, 0xcb, 0xff, 0xe8, 0x03, 0x00, 0x00,
0x18, 0xfc, 0x12, 0xff, 0xe8, 0x03, 0x00, 0x00, 0x18, 0xfc, 0xcb, 0xff, 0xe2, 0x04, 0x00, 0x00,
0x1e, 0xfb, 0x01, 0x00, 0xdc, 0x05, 0x00, 0x00, 0x18, 0xfc, 0x01, 0x00, 0xdc, 0x05, 0x00, 0x00,
0x1e, 0xfb, 0x48, 0xff, 0xee, 0x02, 0x00, 0x00, 0x18, 0xfc, 0x11, 0xfe, 0xee, 0x02, 0x00, 0x00,
0x1e, 0xfb, 0x01, 0x00, 0xd6, 0x06, 0x00, 0x00, 0x18, 0xfc, 0x01, 0x00, 0xd6, 0x06, 0x00, 0x00,
0x1e, 0xfb, 0x01, 0x00, 0xd0, 0x07, 0x00, 0x00, 0x18, 0xfc, 0x01, 0x00, 0xd0, 0x07, 0x00, 0x00,
0x12, 0xfd, 0xf2, 0xff, 0xdc, 0x05, 0x00, 0x00, 0x12, 0xfd, 0x48, 0xff, 0xe2, 0x04, 0x00, 0x00,
0x0c, 0xfe, 0xb1, 0xfe, 0xe2, 0x04, 0x00, 0x00, 0x0c, 0xfe, 0xbe, 0xff, 0xdc, 0x05, 0x00, 0x00,
0x12, 0xfd, 0x01, 0x00, 0xd6, 0x06, 0x00, 0x00, 0x0c, 0xfe, 0x01, 0x00, 0xd6, 0x06, 0x00, 0x00,
0x12, 0xfd, 0x3d, 0xfb, 0xf4, 0x01, 0x00, 0x00, 0x12, 0xfd, 0x46, 0xfa, 0xfa, 0x00, 0x00, 0x00,
0x0c, 0xfe, 0x67, 0xf8, 0xfa, 0x00, 0x00, 0x00, 0x0c, 0xfe, 0x91, 0xf9, 0xf4, 0x01, 0x00, 0x00,
0x12, 0xfd, 0x9c, 0xfc, 0xee, 0x02, 0x00, 0x00, 0x0c, 0xfe, 0x3d, 0xfb, 0xee, 0x02, 0x00, 0x00,
0x12, 0xfd, 0x3d, 0xfb, 0x0c, 0xfe, 0x00, 0x00, 0x12, 0xfd, 0x9c, 0xfc, 0x12, 0xfd, 0x00, 0x00,
0x0c, 0xfe, 0x3d, 0xfb, 0x12, 0xfd, 0x00, 0x00, 0x0c, 0xfe, 0x91, 0xf9, 0x0c, 0xfe, 0x00, 0x00,
0x12, 0xfd, 0x46, 0xfa, 0x06, 0xff, 0x00, 0x00, 0x0c, 0xfe, 0x67, 0xf8, 0x06, 0xff, 0x00, 0x00,
0x12, 0xfd, 0xf2, 0xff, 0x24, 0xfa, 0x00, 0x00, 0x12, 0xfd, 0x01, 0x00, 0x2a, 0xf9, 0x00, 0x00,
0x0c, 0xfe, 0x01, 0x00, 0x2a, 0xf9, 0x00, 0x00, 0x0c, 0xfe, 0xbe, 0xff, 0x24, 0xfa, 0x00, 0x00,
0x12, 0xfd, 0x48, 0xff, 0x1e, 0xfb, 0x00, 0x00, 0x0c, 0xfe, 0xb1, 0xfe, 0x1e, 0xfb, 0x00, 0x00,
0x12, 0xfd, 0x11, 0xfe, 0xe8, 0x03, 0x00, 0x00, 0x0c, 0xfe, 0x14, 0xfd, 0xe8, 0x03, 0x00, 0x00,
0x12, 0xfd, 0xed, 0xf9, 0x00, 0x00, 0x00, 0x00, 0x0c, 0xfe, 0xfc, 0xf7, 0x00, 0x00, 0x00, 0x00,
0x12, 0xfd, 0x11, 0xfe, 0x18, 0xfc, 0x00, 0x00, 0x0c, 0xfe, 0x14, 0xfd, 0x18, 0xfc, 0x00, 0x00,
0x12, 0xfd, 0x01, 0x00, 0x30, 0xf8, 0x00, 0x00, 0x0c, 0xfe, 0x01, 0x00, 0x30, 0xf8, 0x00, 0x00,
0x12, 0xfd, 0x01, 0x00, 0xd0, 0x07, 0x00, 0x00, 0x0c, 0xfe, 0x01, 0x00, 0xd0, 0x07, 0x00, 0x00,
0x06, 0xff, 0x01, 0x00, 0x2a, 0xf9, 0x00, 0x00, 0x06, 0xff, 0x01, 0x00, 0x30, 0xf8, 0x00, 0x00,
0x00, 0x00, 0x01, 0x00, 0x30, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x2a, 0xf9, 0x00, 0x00,
0x06, 0xff, 0x8b, 0xff, 0x24, 0xfa, 0x00, 0x00, 0x00, 0x00, 0x76, 0xff, 0x24, 0xfa, 0x00, 0x00,
0x06, 0xff, 0x3d, 0xfe, 0x1e, 0xfb, 0x00, 0x00, 0x00, 0x00, 0x11, 0xfe, 0x1e, 0xfb, 0x00, 0x00,
0x06, 0xff, 0x5b, 0xfc, 0x18, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x18, 0xfc, 0x18, 0xfc, 0x00, 0x00,
0x06, 0xff, 0x46, 0xfa, 0x12, 0xfd, 0x00, 0x00, 0x00, 0x00, 0xed, 0xf9, 0x12, 0xfd, 0x00, 0x00,
0x06, 0xff, 0x67, 0xf8, 0x0c, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xf7, 0x0c, 0xfe, 0x00, 0x00,
0x06, 0xff, 0x1b, 0xf7, 0x06, 0xff, 0x00, 0x00, 0x00, 0x00, 0xa5, 0xf6, 0x06, 0xff, 0x00, 0x00,
0x06, 0xff, 0xa5, 0xf6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xf6, 0x00, 0x00, 0x00, 0x00,
0x06, 0xff, 0x1b, 0xf7, 0xfa, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa5, 0xf6, 0xfa, 0x00, 0x00, 0x00,
0x06, 0xff, 0x67, 0xf8, 0xf4, 0x01, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xf7, 0xf4, 0x01, 0x00, 0x00,
0x06, 0xff, 0x3d, 0xfe, 0xe2, 0x04, 0x00, 0x00, 0x06, 0xff, 0x5b, 0xfc, 0xe8, 0x03, 0x00, 0x00,
0x00, 0x00, 0x18, 0xfc, 0xe8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x11, 0xfe, 0xe2, 0x04, 0x00, 0x00,
0x06, 0xff, 0x8b, 0xff, 0xdc, 0x05, 0x00, 0x00, 0x00, 0x00, 0x76, 0xff, 0xdc, 0x05, 0x00, 0x00,
0x06, 0xff, 0x46, 0xfa, 0xee, 0x02, 0x00, 0x00, 0x00, 0x00, 0xed, 0xf9, 0xee, 0x02, 0x00, 0x00,
0x06, 0xff, 0x01, 0x00, 0xd6, 0x06, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0xd6, 0x06, 0x00, 0x00,
0x06, 0xff, 0x01, 0x00, 0xd0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0xd0, 0x07, 0x00, 0x00,
0xfa, 0x00, 0x8b, 0xff, 0xdc, 0x05, 0x00, 0x00, 0xfa, 0x00, 0x3d, 0xfe, 0xe2, 0x04, 0x00, 0x00,
0xf4, 0x01, 0xb1, 0xfe, 0xe2, 0x04, 0x00, 0x00, 0xf4, 0x01, 0xbe, 0xff, 0xdc, 0x05, 0x00, 0x00,
0xfa, 0x00, 0x01, 0x00, 0xd6, 0x06, 0x00, 0x00, 0xf4, 0x01, 0x01, 0x00, 0xd6, 0x06, 0x00, 0x00,
0xfa, 0x00, 0x67, 0xf8, 0xf4, 0x01, 0x00, 0x00, 0xfa, 0x00, 0x1b, 0xf7, 0xfa, 0x00, 0x00, 0x00,
0xf4, 0x01, 0x67, 0xf8, 0xfa, 0x00, 0x00, 0x00, 0xf4, 0x01, 0x91, 0xf9, 0xf4, 0x01, 0x00, 0x00,
0xfa, 0x00, 0x46, 0xfa, 0xee, 0x02, 0x00, 0x00, 0xf4, 0x01, 0x3d, 0xfb, 0xee, 0x02, 0x00, 0x00,
0xfa, 0x00, 0x67, 0xf8, 0x0c, 0xfe, 0x00, 0x00, 0xfa, 0x00, 0x46, 0xfa, 0x12, 0xfd, 0x00, 0x00,
0xf4, 0x01, 0x3d, 0xfb, 0x12, 0xfd, 0x00, 0x00, 0xf4, 0x01, 0x91, 0xf9, 0x0c, 0xfe, 0x00, 0x00,
0xfa, 0x00, 0x1b, 0xf7, 0x06, 0xff, 0x00, 0x00, 0xf4, 0x01, 0x67, 0xf8, 0x06, 0xff, 0x00, 0x00,
0xfa, 0x00, 0x8b, 0xff, 0x24, 0xfa, 0x00, 0x00, 0xfa, 0x00, 0x01, 0x00, 0x2a, 0xf9, 0x00, 0x00,
0xf4, 0x01, 0x01, 0x00, 0x2a, 0xf9, 0x00, 0x00, 0xf4, 0x01, 0xbe, 0xff, 0x24, 0xfa, 0x00, 0x00,
0xfa, 0x00, 0x3d, 0xfe, 0x1e, 0xfb, 0x00, 0x00, 0xf4, 0x01, 0xb1, 0xfe, 0x1e, 0xfb, 0x00, 0x00,
0xfa, 0x00, 0x5b, 0xfc, 0xe8, 0x03, 0x00, 0x00, 0xf4, 0x01, 0x14, 0xfd, 0xe8, 0x03, 0x00, 0x00,
0xfa, 0x00, 0xa5, 0xf6, 0x00, 0x00, 0x00, 0x00, 0xf4, 0x01, 0xfc, 0xf7, 0x00, 0x00, 0x00, 0x00,
0xfa, 0x00, 0x5b, 0xfc, 0x18, 0xfc, 0x00, 0x00, 0xf4, 0x01, 0x14, 0xfd, 0x18, 0xfc, 0x00, 0x00,
0xfa, 0x00, 0x01, 0x00, 0x30, 0xf8, 0x00, 0x00, 0xf4, 0x01, 0x01, 0x00, 0x30, 0xf8, 0x00, 0x00,
0xfa, 0x00, 0x01, 0x00, 0xd0, 0x07, 0x00, 0x00, 0xf4, 0x01, 0x01, 0x00, 0xd0, 0x07, 0x00, 0x00,
0xee, 0x02, 0x01, 0x00, 0x2a, 0xf9, 0x00, 0x00, 0xee, 0x02, 0x01, 0x00, 0x30, 0xf8, 0x00, 0x00,
0xe8, 0x03, 0x01, 0x00, 0x30, 0xf8, 0x00, 0x00, 0xe8, 0x03, 0x01, 0x00, 0x2a, 0xf9, 0x00, 0x00,
0xee, 0x02, 0xf2, 0xff, 0x24, 0xfa, 0x00, 0x00, 0xe8, 0x03, 0x01, 0x00, 0x24, 0xfa, 0x00, 0x00,
0xee, 0x02, 0x48, 0xff, 0x1e, 0xfb, 0x00, 0x00, 0xe8, 0x03, 0xcb, 0xff, 0x1e, 0xfb, 0x00, 0x00,
0xee, 0x02, 0x11, 0xfe, 0x18, 0xfc, 0x00, 0x00, 0xe8, 0x03, 0x12, 0xff, 0x18, 0xfc, 0x00, 0x00,
0xee, 0x02, 0x9c, 0xfc, 0x12, 0xfd, 0x00, 0x00, 0xe8, 0x03, 0x11, 0xfe, 0x12, 0xfd, 0x00, 0x00,
0xee, 0x02, 0x3d, 0xfb, 0x0c, 0xfe, 0x00, 0x00, 0xe8, 0x03, 0x14, 0xfd, 0x0c, 0xfe, 0x00, 0x00,
0xee, 0x02, 0x46, 0xfa, 0x06, 0xff, 0x00, 0x00, 0xe8, 0x03, 0x5b, 0xfc, 0x06, 0xff, 0x00, 0x00,
0xee, 0x02, 0xed, 0xf9, 0x00, 0x00, 0x00, 0x00, 0xe8, 0x03, 0x18, 0xfc, 0x00, 0x00, 0x00, 0x00,
0xee, 0x02, 0x46, 0xfa, 0xfa, 0x00, 0x00, 0x00, 0xe8, 0x03, 0x5b, 0xfc, 0xfa, 0x00, 0x00, 0x00,
0xee, 0x02, 0x3d, 0xfb, 0xf4, 0x01, 0x00, 0x00, 0xe8, 0x03, 0x14, 0xfd, 0xf4, 0x01, 0x00, 0x00,
0xee, 0x02, 0x48, 0xff, 0xe2, 0x04, 0x00, 0x00, 0xee, 0x02, 0x11, 0xfe, 0xe8, 0x03, 0x00, 0x00,
0xe8, 0x03, 0x12, 0xff, 0xe8, 0x03, 0x00, 0x00, 0xe8, 0x03, 0xcb, 0xff, 0xe2, 0x04, 0x00, 0x00,
0xee, 0x02, 0xf2, 0xff, 0xdc, 0x05, 0x00, 0x00, 0xe8, 0x03, 0x01, 0x00, 0xdc, 0x05, 0x00, 0x00,
0xee, 0x02, 0x9c, 0xfc, 0xee, 0x02, 0x00, 0x00, 0xe8, 0x03, 0x11, 0xfe, 0xee, 0x02, 0x00, 0x00,
0xee, 0x02, 0x01, 0x00, 0xd6, 0x06, 0x00, 0x00, 0xe8, 0x03, 0x01, 0x00, 0xd6, 0x06, 0x00, 0x00,
0xee, 0x02, 0x01, 0x00, 0xd0, 0x07, 0x00, 0x00, 0xe8, 0x03, 0x01, 0x00, 0xd0, 0x07, 0x00, 0x00,
0xd6, 0x06, 0x01, 0x00, 0x2a, 0xf9, 0x00, 0x00, 0xd6, 0x06, 0x01, 0x00, 0x30, 0xf8, 0x00, 0x00,
0xd0, 0x07, 0x01, 0x00, 0x30, 0xf8, 0x00, 0x00, 0xd0, 0x07, 0x01, 0x00, 0x2a, 0xf9, 0x00, 0x00,
0xd6, 0x06, 0x01, 0x00, 0x24, 0xfa, 0x00, 0x00, 0xd0, 0x07, 0x01, 0x00, 0x24, 0xfa, 0x00, 0x00,
0xd6, 0x06, 0x01, 0x00, 0x1e, 0xfb, 0x00, 0x00, 0xd0, 0x07, 0x01, 0x00, 0x1e, 0xfb, 0x00, 0x00,
0xd6, 0x06, 0x01, 0x00, 0x18, 0xfc, 0x00, 0x00, 0xd0, 0x07, 0x01, 0x00, 0x18, 0xfc, 0x00, 0x00,
0xd6, 0x06, 0x01, 0x00, 0x12, 0xfd, 0x00, 0x00, 0xd0, 0x07, 0x01, 0x00, 0x12, 0xfd, 0x00, 0x00,
0xd6, 0x06, 0x01, 0x00, 0x0c, 0xfe, 0x00, 0x00, 0xd0, 0x07, 0x01, 0x00, 0x0c, 0xfe, 0x00, 0x00,
0xd6, 0x06, 0x01, 0x00, 0x06, 0xff, 0x00, 0x00, 0xd0, 0x07, 0x01, 0x00, 0x06, 0xff, 0x00, 0x00,
0xd6, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd0, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
0xd6, 0x06, 0x01, 0x00, 0xfa, 0x00, 0x00, 0x00, 0xd0, 0x07, 0x01, 0x00, 0xfa, 0x00, 0x00, 0x00,
0xd6, 0x06, 0x01, 0x00, 0xf4, 0x01, 0x00, 0x00, 0xd0, 0x07, 0x01, 0x00, 0xf4, 0x01, 0x00, 0x00,
0xd6, 0x06, 0x01, 0x00, 0xe2, 0x04, 0x00, 0x00, 0xd6, 0x06, 0x01, 0x00, 0xe8, 0x03, 0x00, 0x00,
0xd0, 0x07, 0x01, 0x00, 0xe8, 0x03, 0x00, 0x00, 0xd0, 0x07, 0x01, 0x00, 0xe2, 0x04, 0x00, 0x00,
0xd6, 0x06, 0x01, 0x00, 0xdc, 0x05, 0x00, 0x00, 0xd0, 0x07, 0x01, 0x00, 0xdc, 0x05, 0x00, 0x00,
0xd6, 0x06, 0x01, 0x00, 0xee, 0x02, 0x00, 0x00, 0xd0, 0x07, 0x01, 0x00, 0xee, 0x02, 0x00, 0x00,
0xd6, 0x06, 0x01, 0x00, 0xd6, 0x06, 0x00, 0x00, 0xd0, 0x07, 0x01, 0x00, 0xd6, 0x06, 0x00, 0x00,
0xd6, 0x06, 0x01, 0x00, 0xd0, 0x07, 0x00, 0x00, 0xd0, 0x07, 0x01, 0x00, 0xd0, 0x07, 0x00, 0x00,
0xe2, 0x04, 0x01, 0x00, 0xdc, 0x05, 0x00, 0x00, 0xe2, 0x04, 0x01, 0x00, 0xe2, 0x04, 0x00, 0x00,
0xdc, 0x05, 0x01, 0x00, 0xe2, 0x04, 0x00, 0x00, 0xdc, 0x05, 0x01, 0x00, 0xdc, 0x05, 0x00, 0x00,
0xe2, 0x04, 0x01, 0x00, 0xd6, 0x06, 0x00, 0x00, 0xdc, 0x05, 0x01, 0x00, 0xd6, 0x06, 0x00, 0x00,
0xe2, 0x04, 0xb1, 0xfe, 0xf4, 0x01, 0x00, 0x00, 0xe2, 0x04, 0x3d, 0xfe, 0xfa, 0x00, 0x00, 0x00,
0xdc, 0x05, 0x8b, 0xff, 0xfa, 0x00, 0x00, 0x00, 0xdc, 0x05, 0xbe, 0xff, 0xf4, 0x01, 0x00, 0x00,
0xe2, 0x04, 0x48, 0xff, 0xee, 0x02, 0x00, 0x00, 0xdc, 0x05, 0xf2, 0xff, 0xee, 0x02, 0x00, 0x00,
0xe2, 0x04, 0xb1, 0xfe, 0x0c, 0xfe, 0x00, 0x00, 0xe2, 0x04, 0x48, 0xff, 0x12, 0xfd, 0x00, 0x00,
0xdc, 0x05, 0xf2, 0xff, 0x12, 0xfd, 0x00, 0x00, 0xdc, 0x05, 0xbe, 0xff, 0x0c, 0xfe, 0x00, 0x00,
0xe2, 0x04, 0x3d, 0xfe, 0x06, 0xff, 0x00, 0x00, 0xdc, 0x05, 0x8b, 0xff, 0x06, 0xff, 0x00, 0x00,
0xe2, 0x04, 0x01, 0x00, 0x24, 0xfa, 0x00, 0x00, 0xe2, 0x04, 0x01, 0x00, 0x2a, 0xf9, 0x00, 0x00,
0xdc, 0x05, 0x01, 0x00, 0x2a, 0xf9, 0x00, 0x00, 0xdc, 0x05, 0x01, 0x00, 0x24, 0xfa, 0x00, 0x00,
0xe2, 0x04, 0x01, 0x00, 0x1e, 0xfb, 0x00, 0x00, 0xdc, 0x05, 0x01, 0x00, 0x1e, 0xfb, 0x00, 0x00,
0xe2, 0x04, 0xcb, 0xff, 0xe8, 0x03, 0x00, 0x00, 0xdc, 0x05, 0x01, 0x00, 0xe8, 0x03, 0x00, 0x00,
0xe2, 0x04, 0x11, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xdc, 0x05, 0x76, 0xff, 0x00, 0x00, 0x00, 0x00,
0xe2, 0x04, 0xcb, 0xff, 0x18, 0xfc, 0x00, 0x00, 0xdc, 0x05, 0x01, 0x00, 0x18, 0xfc, 0x00, 0x00,
0xe2, 0x04, 0x01, 0x00, 0x30, 0xf8, 0x00, 0x00, 0xdc, 0x05, 0x01, 0x00, 0x30, 0xf8, 0x00, 0x00,
0xe2, 0x04, 0x01, 0x00, 0xd0, 0x07, 0x00, 0x00, 0xdc, 0x05, 0x01, 0x00, 0xd0, 0x07, 0x00, 0x00,
0x30, 0xf8, 0x01, 0x00, 0xd6, 0x06, 0x00, 0x00, 0x30, 0xf8, 0x01, 0x00, 0xdc, 0x05, 0x00, 0x00,
0x30, 0xf8, 0x01, 0x00, 0xee, 0x02, 0x00, 0x00, 0x30, 0xf8, 0x01, 0x00, 0xf4, 0x01, 0x00, 0x00,
0x30, 0xf8, 0x01, 0x00, 0xe2, 0x04, 0x00, 0x00, 0x30, 0xf8, 0x01, 0x00, 0xe8, 0x03, 0x00, 0x00,
0x30, 0xf8, 0x01, 0x00, 0xfa, 0x00, 0x00, 0x00, 0x30, 0xf8, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
0x30, 0xf8, 0x01, 0x00, 0x06, 0xff, 0x00, 0x00, 0x30, 0xf8, 0x01, 0x00, 0x0c, 0xfe, 0x00, 0x00,
0x30, 0xf8, 0x01, 0x00, 0x12, 0xfd, 0x00, 0x00, 0x30, 0xf8, 0x01, 0x00, 0x18, 0xfc, 0x00, 0x00,
0x30, 0xf8, 0x01, 0x00, 0x1e, 0xfb, 0x00, 0x00, 0x30, 0xf8, 0x01, 0x00, 0x24, 0xfa, 0x00, 0x00,
0x30, 0xf8, 0x01, 0x00, 0x2a, 0xf9, 0x00, 0x00, 0x30, 0xf8, 0x01, 0x00, 0x30, 0xf8, 0x00, 0x00,
0x30, 0xf8, 0x01, 0x00, 0xd0, 0x07, 0x00, 0x00};
// GLOBAL: FF7 0x007fc638
unsigned int g_BattleSummonBahamutResourceDef[140] = {0x00c82a39, (unsigned int)"flash_1.tim", (unsigned int)"flash_1.spt", 0x00000000, 0x00000000, 0x00c82a39, (unsigned int)"flash_2.tim", (unsigned int)"flash_2.spt", 0x00000000, 0x00000000, 0x00c800b1, (unsigned int)"sky_a.rsd", 0x00000000, 0x00000000, 0x00000000, 0x00c800b1, (unsigned int)"wind_1.rsd", 0x00000000, 0x00000000, 0x00000000, 0x00c800b1, (unsigned int)"wind_2.rsd", 0x00000000, 0x00000000, 0x00000000, 0x00c800b1, (unsigned int)"circle_1.rsd", 0x00000000, 0x00000000, 0x00000000, 0x00c800b1, (unsigned int)"bress_1.rsd", 0x00000000, 0x00000000, 0x00000000, 0x00c800b1, (unsigned int)"bress_2.rsd", 0x00000000, 0x00000000, 0x00000000, 0x00c800b1, (unsigned int)"ring_1.rsd", 0x00000000, 0x00000000, 0x00000000, 0x00c800b1, (unsigned int)"exp_1.rsd", 0x00000000, 0x00000000, 0x00000000, 0x00c82a39, (unsigned int)"bomb_1.tim", (unsigned int)"bomb_1.spt", 0x00000000, 0x00000000, 0x00c82a39, (unsigned int)"bomb_2.tim", (unsigned int)"bomb_2.spt", 0x00000000, 0x00000000, 0x00c82a39, (unsigned int)"bomb_3.tim", (unsigned int)"bomb_3.spt", 0x00000000, 0x00000000, 0x00c82a39, (unsigned int)"bomb_4.tim", (unsigned int)"bomb_4.spt", 0x00000000, 0x00000000, 0x00c82a39, (unsigned int)"bomb_5.tim", (unsigned int)"bomb_5.spt", 0x00000000, 0x00000000, 0x00c82a39, (unsigned int)"bomb_6.tim", (unsigned int)"bomb_6.spt", 0x00000000, 0x00000000, 0x00c82a39, (unsigned int)"bomb_7.tim", (unsigned int)"bomb_7.spt", 0x00000000, 0x00000000, 0x00c82a39, (unsigned int)"bomb_8.tim", (unsigned int)"bomb_8.spt", 0x00000000, 0x00000000, 0x00c82a39, (unsigned int)"bomb_9.tim", (unsigned int)"bomb_9.spt", 0x00000000, 0x00000000, 0x00c82a39, (unsigned int)"bomb_10.tim", (unsigned int)"bomb_10.spt", 0x00000000, 0x00000000, 0x00c82a39, (unsigned int)"bomb_11.tim", (unsigned int)"bomb_11.spt", 0x00000000, 0x00000000, 0x00c82a39, (unsigned int)"bomb_12.tim", (unsigned int)"bomb_12.spt", 0x00000000, 0x00000000, 0x00c82a39, (unsigned int)"bomb_13.tim", (unsigned int)"bomb_13.spt", 0x00000000, 0x00000000, 0x00c82a39, (unsigned int)"bomb_14.tim", (unsigned int)"bomb_14.spt", 0x00000000, 0x00000000, 0x00c82a39, (unsigned int)"bomb_15.tim", (unsigned int)"bomb_15.spt", 0x00000000, 0x00000000, 0x00c82a39, (unsigned int)"bomb_16.tim", (unsigned int)"bomb_16.spt", 0x00000000, 0x00000000, 0x00c800b1, (unsigned int)"ring_2.rsd", 0x00000000, 0x00000000, 0x00000000, 0x00c800b1, (unsigned int)"light_1.rsd", 0x00000000, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x007fc868
struct BattleFxBillboardVector g_BattleSummonBahamutModelOffset = {0, -0x222e, 1700};
// GLOBAL: FF7 0x007fc870
unsigned char g_BattleSummonBahamutCameraScript1[36] = {0x41, 0x00, 0x78, 0xec, 0x18, 0xfc, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0x18, 0xfc, 0x00, 0x00, 0x42, 0x00, 0x09, 0x00, 0x78, 0xec, 0x30, 0xf8, 0x00, 0x00, 0x82, 0x00, 0x09, 0x00, 0x00, 0x00, 0x78, 0xec, 0x00, 0x00};
unsigned char g_rawData_007fc894[4] = {0x41, 0x00, 0xa8, 0xe4};
unsigned char g_rawData_007fc898[264] = {
    0x80, 0xc1, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0xd4, 0xb3, 0x00, 0x00, 0x42, 0x00, 0x4b, 0x00,
    0xd8, 0xdc, 0x50, 0xc9, 0x00, 0x00, 0x80, 0x00, 0x4b, 0x00, 0x80, 0x00, 0x0a, 0x00, 0x40, 0x00,
    0x0a, 0x00, 0x08, 0x00, 0x00, 0x01, 0x81, 0x00, 0x00, 0x00, 0xe0, 0xb1, 0xd4, 0xfe, 0x41, 0x00,
    0x00, 0x00, 0x78, 0xec, 0x38, 0xff, 0x82, 0x00, 0x2d, 0x00, 0x00, 0x00, 0x78, 0xec, 0x70, 0xfe,
    0x42, 0x00, 0x2d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9c, 0xff, 0x82, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0x6c, 0xee, 0x00, 0x00, 0x43, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x90, 0xe8, 0x88, 0x13, 0x82, 0x00,
    0x37, 0x00, 0x00, 0x00, 0xa0, 0xdd, 0x00, 0x00, 0x43, 0x00, 0x37, 0x00, 0x00, 0x00, 0xd8, 0xdc,
    0xd0, 0x07, 0x08, 0x00, 0x00, 0x02, 0x41, 0x00, 0xd0, 0x07, 0x68, 0xc5, 0x30, 0xf8, 0x81, 0x00,
    0x00, 0x00, 0x3c, 0xf6, 0x88, 0x13, 0x43, 0x00, 0x23, 0x00, 0x48, 0xf4, 0xcc, 0xde, 0xd0, 0x07,
    0x82, 0x00, 0x23, 0x00, 0x00, 0x00, 0xd8, 0xdc, 0xdc, 0x05, 0x41, 0x00, 0x48, 0xf4, 0xcc, 0xde,
    0xd0, 0x07, 0x81, 0x00, 0x00, 0x00, 0xd8, 0xdc, 0xdc, 0x05, 0x40, 0x00, 0x32, 0x00, 0x80, 0x00,
    0x32, 0x00, 0x42, 0x00, 0x0a, 0x00, 0xf0, 0xd8, 0x60, 0xf0, 0x10, 0x27, 0x82, 0x00, 0x0a, 0x00,
    0x00, 0x00, 0x54, 0xf2, 0x70, 0x17, 0x41, 0x00, 0x00, 0x00, 0xe8, 0x03, 0xb0, 0x36, 0x81, 0x00,
    0x00, 0x00, 0xf8, 0xf8, 0x10, 0x27, 0x40, 0x00, 0x0a, 0x00, 0x80, 0x00, 0x0a, 0x00, 0x41, 0x00,
    0x18, 0xfc, 0x30, 0xf8, 0x18, 0xfc, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x42, 0x00,
    0x0f, 0x00, 0x08, 0xd5, 0xf0, 0xd8, 0x78, 0xec, 0x80, 0x00, 0x0f, 0x00, 0x40, 0x00, 0x1e, 0x00,
    0x80, 0x00, 0x1e, 0x00, 0xff, 0xff, 0x00, 0x00
};
// clang-format on
// GLOBAL: FF7 0x007fc9a0
unsigned char g_BattleSummonBahamutCameraScript2[36] = {0x41, 0x00, 0x78, 0xec, 0x18, 0xfc, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0x18, 0xfc, 0x00, 0x00, 0x42, 0x00, 0x09, 0x00, 0x78, 0xec, 0x30, 0xf8, 0x00, 0x00, 0x82, 0x00, 0x09, 0x00, 0x00, 0x00, 0x78, 0xec, 0x00, 0x00};
// clang-format off
unsigned char g_rawData_007fc9c4[4] = {0x41, 0x00, 0x28, 0x23};
unsigned char g_rawData_007fc9c8[272] = {
    0x68, 0xc5, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0xd4, 0xb3, 0x00, 0x00, 0x40, 0x00, 0x4b, 0x00, 0x80, 0x00, 0x4b, 0x00, 0x80, 0x00, 0x0a, 0x00,
    0x40, 0x00, 0x0a, 0x00, 0x08, 0x00, 0x00, 0x01, 0x81, 0x00, 0x00, 0x00, 0xe0, 0xb1, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0xc0, 0xe0, 0x88, 0x13,
    0x82, 0x00, 0x32, 0x00, 0x00, 0x00, 0x60, 0xf0, 0xe8, 0x03, 0x42, 0x00, 0x32, 0x00, 0x60, 0xf0, 0x60, 0xf0, 0x88, 0x13, 0x82, 0x00, 0x0a, 0x00,
    0x00, 0x00, 0x60, 0xf0, 0xe8, 0x03, 0x43, 0x00, 0x0a, 0x00, 0x48, 0xf4, 0x54, 0xf2, 0xa0, 0x0f, 0x82, 0x00, 0x37, 0x00, 0x00, 0x00, 0xa0, 0xdd,
    0xe8, 0x03, 0x43, 0x00, 0x37, 0x00, 0x30, 0xf8, 0xd8, 0xdc, 0xfc, 0x08, 0x08, 0x00, 0x00, 0x02, 0x41, 0x00, 0x60, 0xf0, 0x90, 0xe8, 0x98, 0x3a,
    0x81, 0x00, 0x00, 0x00, 0xb4, 0xe2, 0x88, 0x13, 0x42, 0x00, 0x23, 0x00, 0xa0, 0x0f, 0x90, 0xe8, 0x98, 0x3a, 0x80, 0x00, 0x23, 0x00, 0x41, 0x00,
    0xdc, 0x05, 0xcc, 0xde, 0xdc, 0x05, 0x81, 0x00, 0x00, 0x00, 0xcc, 0xde, 0xdc, 0x05, 0x42, 0x00, 0x32, 0x00, 0xb8, 0x0b, 0xc0, 0xe0, 0x88, 0x13,
    0x80, 0x00, 0x32, 0x00, 0x42, 0x00, 0x0a, 0x00, 0xa0, 0x0f, 0x60, 0xf0, 0x10, 0x27, 0x82, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x54, 0xf2, 0x70, 0x17,
    0x41, 0x00, 0xa0, 0x0f, 0x60, 0xf0, 0x10, 0x27, 0x81, 0x00, 0x00, 0x00, 0x54, 0xf2, 0x70, 0x17, 0x42, 0x00, 0x0a, 0x00, 0x94, 0x11, 0x30, 0xf8,
    0x18, 0xfc, 0x82, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x41, 0x00, 0x94, 0x11, 0x30, 0xf8, 0x18, 0xfc, 0x81, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x10, 0x27, 0x42, 0x00, 0x0f, 0x00, 0x88, 0x13, 0x90, 0xe8, 0x90, 0xe8, 0x80, 0x00, 0x0f, 0x00, 0x40, 0x00, 0x1e, 0x00, 0x80, 0x00,
    0x1e, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00
};
// clang-format on

// GLOBAL: FF7 0x007fcad8
BattleFxBahamutDrawPair g_BattleSummonBahamutModelDrawRequest1 = {{0, 0x88, 0, 0, 0x20, 0}, 0, 0xa8, 0, 0};
// clang-format off
unsigned char g_rawData_007fcaf4[4] = {0x20, 0x00, 0x00, 0x00};
// clang-format on

// GLOBAL: FF7 0x007fcaf8
BattleFxBahamutDrawHead g_BattleSummonBahamutModelDrawRequest2 = {0, 0x88, 0, 0, 0x20, 0};
// GLOBAL: FF7 0x007fcb08
BattleFxBahamutDrawHead g_BattleSummonBahamutModelDrawRequest3 = {0, 0x88, 0, 0, 0x20, 0};
// GLOBAL: FF7 0x007fcb18
struct BattleFxBahamutFade g_BattleSummonBahamutSpriteDrawRequest1 = {0, 0, -128, -128, -128, 0x2e, 0};
// GLOBAL: FF7 0x007fcb28
BattleFxBahamutDrawHead g_BattleSummonBahamutModelDrawRequest4 = {0, 0xa8, 0, 0, 0x20, 0};
// GLOBAL: FF7 0x007fcb38
BattleFxBahamutDrawHead g_BattleSummonBahamutModelDrawRequest5 = {0, 0x88, 0, 0, 0x20, 0};
// GLOBAL: FF7 0x007fcb48
BattleFxBahamutRingDraw g_BattleSummonBahamutModelDrawRequest6 = {{0, 0x88, 0, 0, 0x20, 0}, {{0x1c00, 0, 0, 0, 0x1c00, 0, 0, 0, 0x1c00}, 0, 0, 0}, 0, {0x1c00, 0x1c00, 0x1c00}, 0};
// GLOBAL: FF7 0x007fcb88
struct BattleFxModelDraw g_BattleSummonBahamutModelDrawRequest7 = {0, 136, 0, 0, 32, 0, 0x1000, 0, {0, 0x10}, 0, {0, 0, 0, 0, 0, 0x10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 136, {0}, 0, {0x20, 0, 0, 0}, 0, {0x88, 0, 0, 0, 0, 0}, 0};

// clang-format off
unsigned char g_rawData_007fcbd4[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007fcbd8
int g_BattleSummonBahamutBeamTransformMatrix[8] = {0, 0, 0, 0, 0, -543948800, 131137535, 0};
// GLOBAL: FF7 0x007fcbf8
int g_BattleSummonBahamutTransformMatrix2[2] = {4096, 0};
// GLOBAL: FF7 0x007fcc00
unsigned char g_BattleSummonBahamutBeamVectorBlob[24] = {0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// GLOBAL: FF7 0x007fcc18
unsigned char g_BattleSummonBahamutBeamVector[16] = {0x50, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x007fcc28
unsigned char g_BattleSummonBahamutBaseDistance1[8] = {0x00, 0x07, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x007fcc30
int g_BattleSummonBahamutBaseDistance2[2] = {1792, 0};
unsigned char g_rawData_007fcc38[8] = {0x00, 0x00, 0xdc, 0x00, 0xa8, 0xfd, 0x00, 0x00};
// GLOBAL: FF7 0x007fcc40
short g_BattleSummonBahamutDefaultPosition[4] = {0, -27500, 0, 0};
// GLOBAL: FF7 0x00aaf988
PsxMatrix g_BattleSummonBahamutTransformMatrix = {0};
// GLOBAL: FF7 0x00aaf9a8
unsigned char g_BattleSummonBahamutDescentInitFlag = 0;
unsigned char g_bssPad_00aaf9ac[4] = {0};
// GLOBAL: FF7 0x00aaf9b0
PsxMatrix g_BattleSummonBahamutComposedMatrix = {0};
// Camera-script operand table: negative operand -k reads element k of the shorts based at
// g_BattleSummonBahamutCameraScriptTable + 6 across the contiguous .bss span.
// GLOBAL: FF7 0x00aaf9d0
unsigned char g_BattleSummonBahamutCameraScriptTable[8] = {0};

unsigned char g_bssPad_00aaf9d8[20] = {0};
// GLOBAL: FF7 0x00aaf9ec
unsigned char g_BattleSummonBahamutBeamInitFlag = 0;
// GLOBAL: FF7 0x00aaf9f0
CameraScriptTrack g_BattleSummonBahamutCameraFocus = {0};
// GLOBAL: FF7 0x00aafa1c
unsigned char g_BattleSummonBahamutCameraFlag1 = 0;
// GLOBAL: FF7 0x00aafa20
unsigned char g_BattleSummonBahamutCameraFlag2 = 0;
// GLOBAL: FF7 0x00aafa24
int g_BattleSummonBahamutCameraParam1 = 0;
// GLOBAL: FF7 0x00aafa28
unsigned char g_BattleSummonBahamutCameraFlag3 = 0;
// GLOBAL: FF7 0x00aafa2c
int g_BattleSummonBahamutDrawHandle = 0;
// GLOBAL: FF7 0x00aafa30
BattleFxBahamutExpandState *g_BattleSummonBahamutSpinningScratch = 0;
// GLOBAL: FF7 0x00aafa34
BattleSummonTaskRecord *g_BattleSummonBahamutBeamScratch = 0;
// GLOBAL: FF7 0x00aafa38
int g_BattleSummonBahamutBlendTimer = 0;
// GLOBAL: FF7 0x00aafa3c
CameraScriptTrack *g_pBattleSummonBahamutCameraTrack = 0;
// GLOBAL: FF7 0x00aafa40
struct Vec3sPacked *g_BattleSummonBahamutTempVector = 0;
// GLOBAL: FF7 0x00aafa44
CameraScriptTrack *g_pBattleSummonBahamutCameraOtherTrack = 0;
// GLOBAL: FF7 0x00aafa48
struct BattleFxBahamutFade *g_pBattleSummonBahamutFade = 0;
// GLOBAL: FF7 0x00aafa4c
BattleFxVertex *g_BattleSummonBahamutScratchVector = 0;
// GLOBAL: FF7 0x00aafa50
int g_BattleSummonBahamutTargetMask = 0;
unsigned char g_bssPad_00aafa54[4] = {0};
// Aliased as g_BattleSummonBahamutCameraEye below.
// GLOBAL: FF7 0x00aafa58
unsigned char CameraEye_00497a37[45] = {0};
#ifndef g_BattleSummonBahamutCameraEye
#define g_BattleSummonBahamutCameraEye CameraEye_00497a37
#endif
// GLOBAL: FF7 0x00aafa88
struct GfxTableCursor *g_BattleSummonBahamutLoadedResTable = 0;
unsigned char g_bssPad_00aafa8c[4] = {0};
// GLOBAL: FF7 0x00aafa90
short g_BattleSummonBahamutRotation[2] = {0};
unsigned char g_bssPad_00aafa94[4] = {0};
// GLOBAL: FF7 0x00aafa98
short g_BattleSummonBahamutUnused[2] = {0};
unsigned char g_bssPad_00aafa9c[4] = {0};
// GLOBAL: FF7 0x00aafaa0
short g_BattleSummonBahamutTimer = 0;
unsigned char g_bssPad_00aafaa4[4] = {0};
// GLOBAL: FF7 0x00aafaa8
BattleFxBahamutBillboardWork g_BattleSummonBahamutBillboardWorkingBlob = {0};
// GLOBAL: FF7 0x00aafab8
struct GfxRotationAngles2 g_BattleSummonBahamutScaleVector = {0};
// GLOBAL: FF7 0x00aafac0
short g_BattleSummonBahamutCentroid[2] = {0};
unsigned char g_bssPad_00aafac4[4] = {0};
// GLOBAL: FF7 0x00aafac8
int g_BattleSummonBahamutTargetActorIdx = 0;
unsigned char g_bssPad_00aafacc[4] = {0};
// clang-format on

#pragma intrinsic(memcpy)
#pragma function(memcpy)

// When the scene pointer is nonzero and g_BattleSummonBahamutLoadedResTable is zero, initializes a
// local resource record with default duration, clears its first field and builds the SUMMON/VAHAMUT
// path into its field eight. Calls Gfx_LoadModelTableIfAbsent with type 0x1C, descriptor
// g_BattleSummonBahamutResourceDef, the local record, cache g_BattleSummonBahamutLoadedResTable and
// scene field 0x4C, then registers g_BattleSummonBahamutSoundGroupPointers sound groups within the
// same guard. Otherwise does nothing. Resource helpers determine allocation and failure behavior;
// sound registration has no separate success test.
// FUNCTION: FF7 0x00497790
void __cdecl BattleFx_SummonLoadResBahamut(void)
{
    GfxModelTableBank *sceneField;
    GfxResourceRequest record;
    int typeId;
    char pathBuffer[200];
    GfxResourceRequest *request;
    int descriptor;

    if (g_pBattle3dScene != 0 && g_BattleSummonBahamutLoadedResTable == 0) {
        typeId = 0x1C;
        descriptor = (int)&g_BattleSummonBahamutResourceDef;
        sceneField = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &record;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x007fce74
        request->resourcePath = (char *)BattleFx_BuildResourcePath("SUMMON/VAHAMUT", pathBuffer);
        Gfx_LoadModelTableIfAbsent(typeId, descriptor, request, &g_BattleSummonBahamutLoadedResTable, sceneField);
        SfxRegisterSoundGroups((struct SoundGroup **)&g_BattleSummonBahamutSoundGroupPointers);
    }
}

// Linear interpolation of a float vector toward a table entry. The blend factor is
// g_BattleSummonBahamutBlendTimer / 40.0f; the target is the three shorts at
// g_BattleSummonBahamutPolygonVertexBlob + index * 8. Writes three floats at out, each
// start[i] + (target[i] - start[i]) * factor.
// FUNCTION: FF7 0x00497831
void __cdecl BattleFx_SummonBahamutBlendPolygonSetVertices(int index, BattleFxBahamutBlendVertex *start, BattleFxHellBomberEndVertex *out)
{
    Vec3sPacked *targetSlot;
    float factor;
    int destX;
    int destY;
    int destZ;

    factor = g_BattleSummonBahamutBlendTimer / 4e+01f;
    targetSlot = (Vec3sPacked *)(index * 8 + (char *)&g_BattleSummonBahamutPolygonVertexBlob);
    destX = targetSlot->vx;
    out->posX = (destX - start->startX) * factor + start->startX;
    destY = targetSlot->vy;
    out->posY = (destY - start->startY) * factor + start->startY;
    destZ = targetSlot->vz;
    out->posZ = (destZ - start->startZ) * factor + start->startZ;
}

// Stores render state, loads Bahamut resources, visits polygon parts of model field 2 with
// BattleFx_SummonBahamutBlendPolygonSetVertices after clearing g_BattleSummonBahamutBlendTimer when the field exists, then calls BattleFx_SummonBeginBahamut and
// returns zero.
// FUNCTION: FF7 0x004978b9
int __cdecl BattleFx_SummonEnterBahamut(int targetMask, int actorIdx)
{
    int val;

    g_BattleSummonBahamutDrawHandle = Gfx_GetPsxPacketScratchPtr();
    BattleFx_SummonLoadResBahamut();
    val = Gfx_ModelTableGetRenderObject(2, g_BattleSummonBahamutLoadedResTable);
    if (val != 0) {
        g_BattleSummonBahamutBlendTimer = 0;
        Gfx_ForEachPolygonSetPart((DxGroupPartCallback)BattleFx_SummonBahamutBlendPolygonSetVertices, *(int *)((char *)val + 4));
    }
    BattleFx_SummonBeginBahamut(targetMask, actorIdx);
    return 0;
}

// Stores the actor mask and obtains its XZ bounding-box midpoint; when its Z is below actorIdx actor Z
// stores Z plus 10000 as the translation Z of g_BattleSummonBahamutTransformMatrix and sets rotation 0x800, otherwise stores Z minus
// 10000. Builds rotation afterward, allocates two supporting tasks, selects one of two camera
// scripts using rand mask 0x100 and sets mask eight in g_BattleStageHideFlags. Native output+4 supplies
// local p through the original adjacent stack slots.
// FUNCTION: FF7 0x00497919
void __cdecl BattleFx_SummonBeginBahamut(int targetMask, int actorIdx)
{
    SVECTOR centerX;

    Battle_GetActorMaskCentroid(g_BattleSummonBahamutCameraParam1 = targetMask, &centerX.vx);
    if (centerX.vz < g_BattleActorRecords[actorIdx].posZ) {
        g_BattleSummonBahamutTransformMatrix.t[2] = centerX.vz + 0x2710;
        g_BattleSummonBahamutRotation[1] = 0x800 /* 180 deg */;
    } else {
        g_BattleSummonBahamutTransformMatrix.t[2] = centerX.vz - 0x2710;
    }
    RotMatrix((SVECTOR *)&g_BattleSummonBahamutRotation, &g_BattleSummonBahamutTransformMatrix);
    Battle_AllocateResourceSlot(BattleFx_SummonBahamutActorDescentTask);
    Battle_AllocateResourceSlot(BattleFx_SummonBahamutTimelineTask);
    if ((rand() & 0x100) != 0) {
        BattleFx_BahamutSpawnCameraScript(&g_BattleSummonBahamutCameraScript1, &g_BattleSummonBahamutTransformMatrix, actorIdx);
    } else {
        BattleFx_BahamutSpawnCameraScript(&g_BattleSummonBahamutCameraScript2, &g_BattleSummonBahamutTransformMatrix, actorIdx);
    }
    g_BattleStageHideFlags |= 8;
}

// Allocates a 0x20-byte resource slot for BattleFx_BahamutRunCameraScript and stores dataA and
// dataB as dwords and mode as a word in it.
// FUNCTION: FF7 0x004979f7
void __cdecl BattleFx_BahamutSpawnCameraScript(void *dataA, void *dataB, int mode)
{
    BattleFxBahamutCameraScriptSlot *slot;

    slot = (BattleFxBahamutCameraScriptSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BahamutRunCameraScript);
    slot->dataA = (int)dataA;
    slot->dataB = (int)dataB;
    slot->mode = mode;
}

// Acquires scratch vectors and interprets eye/focus camera tracks while unpaused, initializing
// only durations and actor selectors once. Reads commands until both durations are nonzero;
// SUMMON_CAMERA_OP_EYE selects eye, and the opcode without 0xC0 is the BattleSummonCameraMode: hold,
// direct position, linear target, distance-preserving target, accelerated target, eased target,
// actor/bone attachment, detach, g_BattleCameraScriptValue or output hold mode nine. Attachment selector zero uses task actor and changes no position offset; detach
// only clears selector. Invalid opcode retires/returns. Distance-target prediction uses other
// linear track remaining duration; accelerated motion updates velocity before position, distance
// motion advances base/radius and normalizes toward the other track, eased motion increments a
// fixed-point fraction before applying easing. Updates eye then focus and decrements both
// durations. Output still runs while paused. With task matrix, unattached positions transform
// fully; attached offsets rotate then add bone/actor world position. Eye installs/restores task
// matrix but focus uses retained installed state, including prior state when eye mode is nine.
// Without task matrix copies positions directly, ignoring attachments. Mode nine preserves that
// output.
// FUNCTION: FF7 0x00497a37
void __cdecl BattleFx_BahamutRunCameraScript(void)
{
    /* VC5 hash order fixes the slots: p=easing, q=wide vector, r=task,
       s=transform flags, t=track index, u/w=scratch vectors. v and x retain
       the original unused frame space (v is the easing-loop index in the
       nine-command dialect). The opcode i follows that space. */
    CameraScriptTask *taskPtr;
    int *wideVec;
    int easedVal;
    int trackIndex;
    int bits;
    int v;
    CameraScriptVector *vecA;
    CameraScriptVector *vecB;
    int x[66];
    short opcode;

    taskPtr = (CameraScriptTask *)((char *)BATTLE_CURRENT_TASK_SLOT);
    vecA = (CameraScriptVector *)Gfx_GetPsxScratchpadPtr(0);
    vecB = vecA + 1;
    wideVec = (int *)(vecB + 1);
    if (g_BattleIsPaused == 0) {
        if (taskPtr->initialized == 0) {
            (*(CameraScriptTrack *)&g_BattleSummonBahamutCameraEye).frames = 0;
            g_BattleSummonBahamutCameraFocus.frames = 0;
            (*(CameraScriptTrack *)&g_BattleSummonBahamutCameraEye).actor = -1;
            g_BattleSummonBahamutCameraFocus.actor = -1;
            taskPtr->initialized = 1;
        }
        while ((*(CameraScriptTrack *)&g_BattleSummonBahamutCameraEye).frames == 0 || g_BattleSummonBahamutCameraFocus.frames == 0) {
            opcode = *taskPtr->cursor++;
            if (opcode & SUMMON_CAMERA_OP_EYE) {
                g_pBattleSummonBahamutCameraTrack = ((CameraScriptTrack *)&g_BattleSummonBahamutCameraEye);
                g_pBattleSummonBahamutCameraOtherTrack = &g_BattleSummonBahamutCameraFocus;
            } else {
                g_pBattleSummonBahamutCameraTrack = &g_BattleSummonBahamutCameraFocus;
                g_pBattleSummonBahamutCameraOtherTrack = ((CameraScriptTrack *)&g_BattleSummonBahamutCameraEye);
            }
            g_pBattleSummonBahamutCameraTrack->mode = opcode & ~0xc0;
            switch (g_pBattleSummonBahamutCameraTrack->mode) {
            case SUMMON_CAMERA_WAIT:
                g_pBattleSummonBahamutCameraTrack->frames = BattleFx_BahamutReadCameraScriptOperand(&taskPtr->cursor);
                break;
            case SUMMON_CAMERA_SET_POSITION:
                g_pBattleSummonBahamutCameraTrack->position.x = *taskPtr->cursor++;
                g_pBattleSummonBahamutCameraTrack->position.y = *taskPtr->cursor++;
                g_pBattleSummonBahamutCameraTrack->position.z = *taskPtr->cursor++;
                break;
            case SUMMON_CAMERA_MOVE_LINEAR:
                g_pBattleSummonBahamutCameraTrack->frames = BattleFx_BahamutReadCameraScriptOperand(&taskPtr->cursor);
                g_pBattleSummonBahamutCameraTrack->velocity.x = (*taskPtr->cursor++ - g_pBattleSummonBahamutCameraTrack->position.x) / g_pBattleSummonBahamutCameraTrack->frames;
                g_pBattleSummonBahamutCameraTrack->velocity.y = (*taskPtr->cursor++ - g_pBattleSummonBahamutCameraTrack->position.y) / g_pBattleSummonBahamutCameraTrack->frames;
                g_pBattleSummonBahamutCameraTrack->velocity.z = (*taskPtr->cursor++ - g_pBattleSummonBahamutCameraTrack->position.z) / g_pBattleSummonBahamutCameraTrack->frames;
                break;
            case SUMMON_CAMERA_MOVE_ORBIT:
                vecA->x = g_pBattleSummonBahamutCameraOtherTrack->position.x - g_pBattleSummonBahamutCameraTrack->position.x;
                vecA->y = g_pBattleSummonBahamutCameraOtherTrack->position.y - g_pBattleSummonBahamutCameraTrack->position.y;
                vecA->z = g_pBattleSummonBahamutCameraOtherTrack->position.z - g_pBattleSummonBahamutCameraTrack->position.z;
                g_pBattleSummonBahamutCameraTrack->distance = SqrtToInt(vecA->x * vecA->x + vecA->y * vecA->y + vecA->z * vecA->z);
                g_pBattleSummonBahamutCameraTrack->base = g_pBattleSummonBahamutCameraTrack->position;
                g_pBattleSummonBahamutCameraTrack->frames = BattleFx_BahamutReadCameraScriptOperand(&taskPtr->cursor);
                vecA->x = *taskPtr->cursor++;
                vecA->y = *taskPtr->cursor++;
                vecA->z = *taskPtr->cursor++;
                g_pBattleSummonBahamutCameraTrack->velocity.x = (vecA->x - g_pBattleSummonBahamutCameraTrack->position.x) / g_pBattleSummonBahamutCameraTrack->frames;
                g_pBattleSummonBahamutCameraTrack->velocity.y = (vecA->y - g_pBattleSummonBahamutCameraTrack->position.y) / g_pBattleSummonBahamutCameraTrack->frames;
                g_pBattleSummonBahamutCameraTrack->velocity.z = (vecA->z - g_pBattleSummonBahamutCameraTrack->position.z) / g_pBattleSummonBahamutCameraTrack->frames;
                switch (g_pBattleSummonBahamutCameraOtherTrack->mode) {
                case SUMMON_CAMERA_MOVE_LINEAR:
                    vecB->x = g_pBattleSummonBahamutCameraOtherTrack->position.x + g_pBattleSummonBahamutCameraOtherTrack->velocity.x * g_pBattleSummonBahamutCameraOtherTrack->frames;
                    vecB->y = g_pBattleSummonBahamutCameraOtherTrack->position.y + g_pBattleSummonBahamutCameraOtherTrack->velocity.y * g_pBattleSummonBahamutCameraOtherTrack->frames;
                    vecB->z = g_pBattleSummonBahamutCameraOtherTrack->position.z + g_pBattleSummonBahamutCameraOtherTrack->velocity.z * g_pBattleSummonBahamutCameraOtherTrack->frames;
                    break;
                default:
                    vecB->x = g_pBattleSummonBahamutCameraOtherTrack->position.x;
                    vecB->y = g_pBattleSummonBahamutCameraOtherTrack->position.y;
                    vecB->z = g_pBattleSummonBahamutCameraOtherTrack->position.z;
                    break;
                }
                vecA->x -= vecB->x;
                vecA->y -= vecB->y;
                vecA->z -= vecB->z;
                g_pBattleSummonBahamutCameraTrack->delta = (SqrtToInt(vecA->x * vecA->x + vecA->y * vecA->y + vecA->z * vecA->z) - g_pBattleSummonBahamutCameraTrack->distance) / g_pBattleSummonBahamutCameraTrack->frames;
                break;
            case SUMMON_CAMERA_MOVE_ACCEL:
                g_pBattleSummonBahamutCameraTrack->frames = BattleFx_BahamutReadCameraScriptOperand(&taskPtr->cursor);
                vecA->x = *taskPtr->cursor++ - g_pBattleSummonBahamutCameraTrack->position.x;
                vecA->y = *taskPtr->cursor++ - g_pBattleSummonBahamutCameraTrack->position.y;
                vecA->z = *taskPtr->cursor++ - g_pBattleSummonBahamutCameraTrack->position.z;
                g_pBattleSummonBahamutCameraTrack->base.x = ((vecA->x - g_pBattleSummonBahamutCameraTrack->velocity.x * g_pBattleSummonBahamutCameraTrack->frames) * 2) / (g_pBattleSummonBahamutCameraTrack->frames * g_pBattleSummonBahamutCameraTrack->frames);
                g_pBattleSummonBahamutCameraTrack->base.y = ((vecA->y - g_pBattleSummonBahamutCameraTrack->velocity.y * g_pBattleSummonBahamutCameraTrack->frames) * 2) / (g_pBattleSummonBahamutCameraTrack->frames * g_pBattleSummonBahamutCameraTrack->frames);
                g_pBattleSummonBahamutCameraTrack->base.z = ((vecA->z - g_pBattleSummonBahamutCameraTrack->velocity.z * g_pBattleSummonBahamutCameraTrack->frames) * 2) / (g_pBattleSummonBahamutCameraTrack->frames * g_pBattleSummonBahamutCameraTrack->frames);
                break;
            case SUMMON_CAMERA_MOVE_EASED:
                g_pBattleSummonBahamutCameraTrack->frames = BattleFx_BahamutReadCameraScriptOperand(&taskPtr->cursor);
                g_pBattleSummonBahamutCameraTrack->easing = *taskPtr->cursor++;
                g_pBattleSummonBahamutCameraTrack->distance = 0;
                g_pBattleSummonBahamutCameraTrack->delta = 0x1000 / g_pBattleSummonBahamutCameraTrack->frames;
                g_pBattleSummonBahamutCameraTrack->base.x = g_pBattleSummonBahamutCameraTrack->position.x;
                g_pBattleSummonBahamutCameraTrack->base.y = g_pBattleSummonBahamutCameraTrack->position.y;
                g_pBattleSummonBahamutCameraTrack->base.z = g_pBattleSummonBahamutCameraTrack->position.z;
                g_pBattleSummonBahamutCameraTrack->velocity.x = *taskPtr->cursor++ - g_pBattleSummonBahamutCameraTrack->position.x;
                g_pBattleSummonBahamutCameraTrack->velocity.y = *taskPtr->cursor++ - g_pBattleSummonBahamutCameraTrack->position.y;
                g_pBattleSummonBahamutCameraTrack->velocity.z = *taskPtr->cursor++ - g_pBattleSummonBahamutCameraTrack->position.z;
                break;
            case SUMMON_CAMERA_ATTACH_BONE:
                if ((g_pBattleSummonBahamutCameraTrack->actor = *taskPtr->cursor++) == 0)
                    g_pBattleSummonBahamutCameraTrack->actor = taskPtr->actor;
                g_pBattleSummonBahamutCameraTrack->bone = *taskPtr->cursor++;
                break;
            case SUMMON_CAMERA_DETACH:
                g_pBattleSummonBahamutCameraTrack->actor = -1;
                break;
            case SUMMON_CAMERA_WAIT_NO_WRITEBACK:
                g_pBattleSummonBahamutCameraTrack->frames = BattleFx_BahamutReadCameraScriptOperand(&taskPtr->cursor);
                break;
            case SUMMON_CAMERA_SET_SCRIPT_VALUE:
                g_BattleCameraScriptValue = *taskPtr->cursor++;
                break;
            default:
                taskPtr->active = -1;
                return;
            }
        }
        g_pBattleSummonBahamutCameraTrack = ((CameraScriptTrack *)&g_BattleSummonBahamutCameraEye);
        g_pBattleSummonBahamutCameraOtherTrack = &g_BattleSummonBahamutCameraFocus;
        for (trackIndex = 0; trackIndex < 2; trackIndex++) {
            switch (g_pBattleSummonBahamutCameraTrack->mode) {
            case SUMMON_CAMERA_WAIT:
                break;
            case SUMMON_CAMERA_MOVE_ACCEL:
                g_pBattleSummonBahamutCameraTrack->velocity.x += g_pBattleSummonBahamutCameraTrack->base.x;
                g_pBattleSummonBahamutCameraTrack->velocity.y += g_pBattleSummonBahamutCameraTrack->base.y;
                g_pBattleSummonBahamutCameraTrack->velocity.z += g_pBattleSummonBahamutCameraTrack->base.z;
            case SUMMON_CAMERA_MOVE_LINEAR:
                g_pBattleSummonBahamutCameraTrack->position.x += g_pBattleSummonBahamutCameraTrack->velocity.x;
                g_pBattleSummonBahamutCameraTrack->position.y += g_pBattleSummonBahamutCameraTrack->velocity.y;
                g_pBattleSummonBahamutCameraTrack->position.z += g_pBattleSummonBahamutCameraTrack->velocity.z;
                break;
            case SUMMON_CAMERA_MOVE_ORBIT:
                g_pBattleSummonBahamutCameraTrack->base.x += g_pBattleSummonBahamutCameraTrack->velocity.x;
                g_pBattleSummonBahamutCameraTrack->base.y += g_pBattleSummonBahamutCameraTrack->velocity.y;
                g_pBattleSummonBahamutCameraTrack->base.z += g_pBattleSummonBahamutCameraTrack->velocity.z;
                g_pBattleSummonBahamutCameraTrack->distance += g_pBattleSummonBahamutCameraTrack->delta;
                wideVec[0] = g_pBattleSummonBahamutCameraOtherTrack->position.x - g_pBattleSummonBahamutCameraTrack->base.x;
                wideVec[1] = g_pBattleSummonBahamutCameraOtherTrack->position.y - g_pBattleSummonBahamutCameraTrack->base.y;
                wideVec[2] = g_pBattleSummonBahamutCameraOtherTrack->position.z - g_pBattleSummonBahamutCameraTrack->base.z;
                VectorNormalToSVector(wideVec, (short *)vecA);
                g_pBattleSummonBahamutCameraTrack->position.x = g_pBattleSummonBahamutCameraOtherTrack->position.x - ((vecA->x * g_pBattleSummonBahamutCameraTrack->distance) >> 12);
                g_pBattleSummonBahamutCameraTrack->position.y = g_pBattleSummonBahamutCameraOtherTrack->position.y - ((vecA->y * g_pBattleSummonBahamutCameraTrack->distance) >> 12);
                g_pBattleSummonBahamutCameraTrack->position.z = g_pBattleSummonBahamutCameraOtherTrack->position.z - ((vecA->z * g_pBattleSummonBahamutCameraTrack->distance) >> 12);
                break;
            case SUMMON_CAMERA_MOVE_EASED:
                g_pBattleSummonBahamutCameraTrack->distance += g_pBattleSummonBahamutCameraTrack->delta;
                easedVal = g_pBattleSummonBahamutCameraTrack->distance;
                easedVal = BattleFx_BahamutApplyCameraEasing(easedVal, g_pBattleSummonBahamutCameraTrack->easing);
                g_pBattleSummonBahamutCameraTrack->position.x = g_pBattleSummonBahamutCameraTrack->base.x + ((g_pBattleSummonBahamutCameraTrack->velocity.x * easedVal) >> 12);
                g_pBattleSummonBahamutCameraTrack->position.y = g_pBattleSummonBahamutCameraTrack->base.y + ((g_pBattleSummonBahamutCameraTrack->velocity.y * easedVal) >> 12);
                g_pBattleSummonBahamutCameraTrack->position.z = g_pBattleSummonBahamutCameraTrack->base.z + ((g_pBattleSummonBahamutCameraTrack->velocity.z * easedVal) >> 12);
                break;
            }
            g_pBattleSummonBahamutCameraTrack->frames--;
            g_pBattleSummonBahamutCameraTrack = &g_BattleSummonBahamutCameraFocus;
            g_pBattleSummonBahamutCameraOtherTrack = ((CameraScriptTrack *)&g_BattleSummonBahamutCameraEye);
        }
    }
    if (taskPtr->matrix) {
        if ((*(CameraScriptTrack *)&g_BattleSummonBahamutCameraEye).mode != 9) {
            SetRotMatrix((void *)taskPtr->matrix);
            SetTransMatrix((void *)taskPtr->matrix);
            if ((*(CameraScriptTrack *)&g_BattleSummonBahamutCameraEye).actor == -1) {
                RotTransPoint(&((CameraScriptTrack *)&g_BattleSummonBahamutCameraEye)->position, wideVec, &bits);
                g_BattleCameraPosition.cameraPosX = wideVec[0];
                g_BattleCameraPosition.cameraPosY = wideVec[1];
                g_BattleCameraPosZ = wideVec[2];
            } else {
                RotateSVectorByInstalledMatrix(&((CameraScriptTrack *)&g_BattleSummonBahamutCameraEye)->position, wideVec);
                if ((*(CameraScriptTrack *)&g_BattleSummonBahamutCameraEye).bone != -1) {
                    Battle_GetBoneWorldPos((*(CameraScriptTrack *)&g_BattleSummonBahamutCameraEye).actor, (*(CameraScriptTrack *)&g_BattleSummonBahamutCameraEye).bone, (short *)vecA);
                } else {
                    vecA->x = g_BattleActorRecords[(*(CameraScriptTrack *)&g_BattleSummonBahamutCameraEye).actor].posX;
                    vecA->y = g_BattleActorRecords[(*(CameraScriptTrack *)&g_BattleSummonBahamutCameraEye).actor].posY;
                    vecA->z = g_BattleActorRecords[(*(CameraScriptTrack *)&g_BattleSummonBahamutCameraEye).actor].posZ;
                }
                g_BattleCameraPosition.cameraPosX = wideVec[0] + vecA->x;
                g_BattleCameraPosition.cameraPosY = wideVec[1] + vecA->y;
                g_BattleCameraPosZ = wideVec[2] + vecA->z;
                SetRotMatrix((void *)taskPtr->matrix);
                SetTransMatrix((void *)taskPtr->matrix);
            }
        }
        if (g_BattleSummonBahamutCameraFocus.mode != 9) {
            if (g_BattleSummonBahamutCameraFocus.actor == -1) {
                RotTransPoint(&g_BattleSummonBahamutCameraFocus.position, wideVec, &bits);
                g_BattleCameraFocalPoint.focalPointX = wideVec[0];
                g_BattleCameraFocalPoint.focalPointY = wideVec[1];
                g_BattleCameraFocusZ = wideVec[2];
            } else {
                RotateSVectorByInstalledMatrix(&g_BattleSummonBahamutCameraFocus.position, wideVec);
                if (g_BattleSummonBahamutCameraFocus.bone != -1) {
                    Battle_GetBoneWorldPos(g_BattleSummonBahamutCameraFocus.actor, g_BattleSummonBahamutCameraFocus.bone, (short *)vecA);
                } else {
                    vecA->x = g_BattleActorRecords[g_BattleSummonBahamutCameraFocus.actor].posX;
                    vecA->y = g_BattleActorRecords[g_BattleSummonBahamutCameraFocus.actor].posY;
                    vecA->z = g_BattleActorRecords[g_BattleSummonBahamutCameraFocus.actor].posZ;
                }
                g_BattleCameraFocalPoint.focalPointX = wideVec[0] + vecA->x;
                g_BattleCameraFocalPoint.focalPointY = wideVec[1] + vecA->y;
                g_BattleCameraFocusZ = wideVec[2] + vecA->z;
            }
        }
    } else {
        if ((*(CameraScriptTrack *)&g_BattleSummonBahamutCameraEye).mode != 9) {
            *(int *)&g_BattleCameraPosition = *(int *)&((CameraScriptTrack *)&g_BattleSummonBahamutCameraEye)->position;
            *(int *)&g_BattleCameraPosZ = *(int *)&((CameraScriptTrack *)&g_BattleSummonBahamutCameraEye)->position.z;
        }
        if (g_BattleSummonBahamutCameraFocus.mode != 9) {
            *(int *)&g_BattleCameraFocalPoint = *(int *)&g_BattleSummonBahamutCameraFocus.position;
            *(int *)&g_BattleCameraFocusZ = *(int *)&g_BattleSummonBahamutCameraFocus.position.z;
        }
    }
}

// Same token reader as BattleFx_NeoBahamutReadCameraScriptOperand with a different pool: reads a short from the cursor at cursor,
// advances it by 2, and if the token is negative returns the short at
// `((short *)((char *)&g_BattleSummonBahamutCameraScriptTable + 6))[-token]` instead.
// FUNCTION: FF7 0x0049897d
short __cdecl BattleFx_BahamutReadCameraScriptOperand(short **cursor)
{
    int token;

    token = **cursor;
    *cursor += 1;
    if (token < 0) {
        token = ((short *)((char *)&g_BattleSummonBahamutCameraScriptTable + 0x6))[-token];
    }
    return (short)token;
}

// Fixed-point easing helper: phase is a 0..0x1000 phase, packed packs a repeat count in bits 8+ and a
// blend weight minus one in the low byte. Applies SinFixed(phase / 4) count times (through 0x1000 -
// phase when packed is negative, negating packed first) and returns the result blended with the
// original phase by ((packed & 0xFF) + 1)/256. Pure function; touches no globals.
// FUNCTION: FF7 0x004989b7
int __cdecl BattleFx_BahamutApplyCameraEasing(int phase, int packed)
{
    int idx;
    int repeats;
    int original;

    original = phase;
    if (packed > 0) {
        repeats = packed >> 8;
        for (idx = 0; idx < repeats; idx++) {
            phase = SinFixed(phase / 4);
        }
    } else {
        packed = -packed;
        repeats = packed >> 8;
        phase = 0x1000 - phase;
        for (idx = 0; idx < repeats; idx++) {
            phase = SinFixed(phase / 4);
        }
        phase = 0x1000 - phase;
    }
    repeats = (packed & 0xFF) + 1;
    phase = (phase * repeats + original * (0x100 - repeats)) >> 8;
    return phase;
}

// Acquires retained scratch once and composes g_BattleCameraTransform with
// g_BattleSummonBahamutTransformMatrix into g_BattleSummonBahamutComposedMatrix before returning
// while paused. Unpaused count zero clears screen RGB and starts ramp 4096 over five; nine sets
// flag bits seven. At ten allocates BattleFx_SummonBahamutSpawnFadingBillboardSpritesTask/00499540
// and dispatches sound stage zero; at 85 allocates BattleFx_SummonBahamutRotatingModelPairTask; at
// 145 dispatches sound stage one. At 155 clears low three flag bits, starts ramp 2048 over 55 and
// clears RGB. At 250 allocates BattleFx_SummonBahamutSpawnStreakLinesTask; at 255 allocates
// BattleFx_SummonBahamutBlackGrowingModelTask, dispatches stage two and releases buffers 0x27C
// through 0x27E. At 285 allocates BattleFx_SummonBahamutExtendingBeamTask and clears its rotation.
// At 315 allocates BattleFx_SummonBahamutSpawnAnimatedSpritesTask, calls
// BattleFx_SummonBahamutSpawnShakeEnvelope(100,0,15,30), dispatches stage three and releases 0x27F
// through 0x281. Counts 315/317/319/321/323 emit three related records: random-position/rotation
// BattleFx_SummonBahamutSpinningExpandingModelTask with size 256 through 383,
// BattleFx_SummonBahamutExpandingModelTask copying eight-byte position/rotation groups with size
// 1024 through 1535, and BattleFx_SummonBahamutBillboardGrowingModelTask copying position with
// field 0x14 4096. At 330 allocates typed records for selected ten actor bits. At 360 and later
// starts zero ramp over five, clears flags, retires and releases 0x282 through 0x287; count still
// advances afterward.
// FUNCTION: FF7 0x00498a96
void __cdecl BattleFx_SummonBahamutTimelineTask(void)
{
    BattleFxBahamutTimelineRecord *expanding;
    int stage;
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleTaskSlot *taskSlot;
    int scratchPtr;
    int actorIndex;
    int blockPointer;
    BattleFxBahamutSpawnRecord *pSpawn;

    taskSlot = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonBahamutCameraFlag1 & 1) == 0) {
        g_BattleSummonBahamutCameraFlag1 |= 1;
        g_BattleSummonBahamutTargetMask = Gfx_GetPsxScratchpadPtr(0);
    }
    MultiplyMatrix((void *)g_BattleCameraTransform.matrix, &g_BattleSummonBahamutTransformMatrix, &g_BattleSummonBahamutComposedMatrix);
    if (g_BattleIsPaused != 0) {
        return;
    }
    blockPointer = Gfx_GetPsxScratchpadPtr(0);
    scratchPtr = blockPointer + 8;
    stage = taskSlot->frameCounter;
    if (stage < 10) {
        if (stage == 0) {
            g_BattleFxTimelineState.channel2 = 0;
            g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
            g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
            Battle_StartFixedPointRamp(0x1000, 5);
        }
        if (stage == 9) {
            g_BattleStageHideFlags |= 7;
        }
    } else {
        stage -= 10;
        if (stage < 45) {
            if (stage == 0) {
                pSpawn = (BattleFxBahamutSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonBahamutSpawnFadingBillboardSpritesTask);
                pSpawn = (BattleFxBahamutSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonBahamutCounterRotatingModelPairTask);
            }
            if (stage == 0) {
                Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 0);
            }
        } else {
            stage -= 45;
            if (stage < 30) { /* empty first arm: lib/codegen.h */
            } else {
                stage -= 30;
                if (stage < 10) {
                    if (stage == 0) {
                        pSpawn = (BattleFxBahamutSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonBahamutRotatingModelPairTask);
                    }
                } else {
                    stage -= 10;
                    if (stage < 60) {
                        if (stage == 50) {
                            Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 1);
                        }
                    } else {
                        stage -= 60;
                        if (stage < 55) {
                            if (stage == 0) {
                                g_BattleStageHideFlags &= 0xF8;
                                Battle_StartFixedPointRamp(0x800, 55);
                                g_BattleFxTimelineState.red = 0;
                                g_BattleFxTimelineState.channel1 = 0;
                                g_BattleFxTimelineState.channel2 = 0;
                            }
                        } else {
                            stage -= 55;
                            if (stage < 35) { /* empty first arm: lib/codegen.h */
                            } else {
                                stage -= 35;
                                if (stage < 40) {
                                    if (stage == 10) {
                                        pSpawn = (BattleFxBahamutSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonBahamutBlackGrowingModelTask);
                                    }
                                    if (stage == 5) {
                                        pSpawn = (BattleFxBahamutSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonBahamutSpawnStreakLinesTask);
                                    }
                                    if (stage == 10) {
                                        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 2);
                                        Sfx_ReleaseCachedBuffer(0x27C, 0);
                                        Sfx_ReleaseCachedBuffer(0x27D, 0);
                                        Sfx_ReleaseCachedBuffer(0x27E, 0);
                                    }
                                } else {
                                    stage -= 40;
                                    if (stage < 5) {
                                        if (stage == 0) {
                                            pSpawn = (BattleFxBahamutSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonBahamutExtendingBeamTask);
                                            pSpawn->angleZ = 0;
                                            pSpawn->angleY = 0;
                                            pSpawn->angleX = 0;
                                        }
                                    } else {
                                        stage -= 5;
                                        if (stage < 15) { /* empty first arm: lib/codegen.h */
                                        } else {
                                            stage -= 15;
                                            if (stage < 10) { /* empty first arm: lib/codegen.h */
                                            } else {
                                                stage -= 10;
                                                if (stage < 15) {
                                                    if (stage == 0) {
                                                        pSpawn = (BattleFxBahamutSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonBahamutSpawnAnimatedSpritesTask);
                                                        BattleFx_SummonBahamutSpawnShakeEnvelope(0x64, 0, 15, 0x1E);
                                                    }
                                                    if (stage < 10 && stage % 2 == 0) {
                                                        pSpawn = (BattleFxBahamutSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonBahamutSpinningExpandingModelTask);
                                                        pSpawn->spawnX = rand() % 6000 - 3000;
                                                        pSpawn->spawnY = rand() % 5000 - 5000;
                                                        pSpawn->spawnZ = rand() % 6000 + 7000;
                                                        pSpawn->angleX = rand() & 0xFFF;
                                                        pSpawn->angleY = rand() & 0xFFF;
                                                        pSpawn->angleZ = rand() & 0xFFF;
                                                        pSpawn->targetValue = rand() % 128 + 256;
                                                        pSpawn->curValue = pSpawn->targetValue;
                                                        expanding = (BattleFxBahamutTimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonBahamutExpandingModelTask);
                                                        *(Int64Pair *)((char *)expanding + 4) = *(Int64Pair *)((char *)pSpawn + 4);
                                                        *(Int64Pair *)((char *)expanding + 0xC) = *(Int64Pair *)((char *)pSpawn + 0xC);
                                                        expanding->targetValue = rand() % 512 + 1024;
                                                        expanding->curValue = expanding->targetValue;
                                                        expanding = (BattleFxBahamutTimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonBahamutBillboardGrowingModelTask);
                                                        *(Int64Pair *)((char *)expanding + 4) = *(Int64Pair *)((char *)pSpawn + 4);
                                                        expanding->curValue = 0x1000;
                                                    }
                                                    if (stage == 0) {
                                                        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 3);
                                                        Sfx_ReleaseCachedBuffer(0x27F, 1);
                                                        Sfx_ReleaseCachedBuffer(0x280, 1);
                                                        Sfx_ReleaseCachedBuffer(0x281, 1);
                                                    }
                                                } else {
                                                    stage -= 15;
                                                    if (stage < 30) {
                                                        if (stage == 0) {
                                                            for (actorIndex = 0; actorIndex < 10; actorIndex++) {
                                                                if ((g_BattleSummonBahamutCameraParam1 & 1 << actorIndex) != 0) {
                                                                    Battle_PostActorHitOrHurtMessage(actorIndex);
                                                                }
                                                            }
                                                        }
                                                    } else {
                                                        Battle_StartFixedPointRamp(0, 5);
                                                        g_BattleStageHideFlags = 0;
                                                        taskSlot->ownerSlot = BATTLE_TASK_FREE;
                                                        Sfx_ReleaseCachedBuffer(0x282, 2);
                                                        Sfx_ReleaseCachedBuffer(0x283, 2);
                                                        Sfx_ReleaseCachedBuffer(0x284, 2);
                                                        Sfx_ReleaseCachedBuffer(0x285, 3);
                                                        Sfx_ReleaseCachedBuffer(0x286, 3);
                                                        Sfx_ReleaseCachedBuffer(0x287, 3);
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
    (taskSlot->frameCounter)++;
}

// Allocates a resource slot for BattleFx_SummonBahamutShakeEnvelopeTask and stores its four arguments as words at
// +4/+6/+8/+0xA.
// FUNCTION: FF7 0x004990af
void __cdecl BattleFx_SummonBahamutSpawnShakeEnvelope(int amplitude, int decay, int duration, int frequency)
{
    BattleFxBahamutShakeEnvelope *slot;

    slot = (BattleFxBahamutShakeEnvelope *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonBahamutShakeEnvelopeTask);
    slot->amplitude = amplitude;
    slot->decay = decay;
    slot->duration = duration;
    slot->frequency = frequency;
}

// Returns while paused. Uses signed task amplitude and three saved durations to ramp amplitude up
// with a 4096 fraction, hold it, then ramp down. Writes positive value to
// g_BattleCameraTransform.posY on odd count and negative value on even count, then advances count.
// When all three phases are exhausted retires, clears g_BattleCameraTransform.posY and returns
// without advancing.
// FUNCTION: FF7 0x004990fe
void __cdecl BattleFx_SummonBahamutShakeEnvelopeTask(void)
{
    struct {
        int ratio;
        BattleFxJitterEnvelope *p;
        int frame;
        int value;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.p = (BattleFxJitterEnvelope *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    local.frame = local.p->frameCount;
    if (local.frame < local.p->riseFrames) {
        local.ratio = (local.frame << 12) / local.p->riseFrames;
        local.value = local.p->amplitude * local.ratio >> 12;
    } else {
        local.frame -= local.p->riseFrames;
        if (local.frame < local.p->holdFrames) {
            local.value = local.p->amplitude;
        } else {
            local.frame -= local.p->holdFrames;
            if (local.frame < local.p->fallFrames) {
                local.ratio = (local.frame << 12) / local.p->fallFrames;
                local.value = local.p->amplitude * (0x1000 - local.ratio) >> 12;
            } else {
                local.p->ownerSlot = 0xFFFF;
                g_BattleCameraTransform.posY = 0;
                return;
            }
        }
    }
    g_BattleCameraTransform.posY = (local.p->frameCount & 1) ? local.value : -local.value;
    (local.p->frameCount)++;
}

// When unpaused acquires data block zero and installs g_BattleSummonBahamutTransformMatrix, then allocates two
// BattleFx_SummonBahamutFadingBillboardSpriteTask slots from points with random X/Z -1000 through 999 and Y -20000 transformed
// through that matrix. Copies the low shorts of the resulting int coordinates and assigns random
// scale 0x1000 through 0x1FFF; advances the emitter counter and retires at forty-one or above.
// FUNCTION: FF7 0x00499227
void __cdecl BattleFx_SummonBahamutSpawnFadingBillboardSpritesTask(void)
{
    int spriteIndex;
    int flagOut;
    BattleFxBahamutFadingSpriteTask *taskPtr;
    BattleFxPosition3s *worldPosition;
    Vec3sPacked *pPoint;
    BattleFxBahamutAnimatedSpriteRecord *pSprite;

    taskPtr = (BattleFxBahamutFadingSpriteTask *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    pPoint = (Vec3sPacked *)Gfx_GetPsxScratchpadPtr(0);
    worldPosition = (BattleFxPosition3s *)((int)pPoint + 8);
    flagOut = (int)worldPosition + 0x10;
    SetRotMatrix(&g_BattleSummonBahamutTransformMatrix);
    SetTransMatrix(&g_BattleSummonBahamutTransformMatrix);
    for (spriteIndex = 0; spriteIndex < 2; spriteIndex++) {
        pSprite = (BattleFxBahamutAnimatedSpriteRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonBahamutFadingBillboardSpriteTask);
        pPoint->vx = rand() % 2000 - 1000;
        pPoint->vy = 0xB1E0;
        pPoint->vz = rand() % 2000 - 1000;
        RotTransPoint(pPoint, (int *)worldPosition, (void *)flagOut);
        pSprite->posX = worldPosition->posX;
        pSprite->posY = worldPosition->posY;
        pSprite->posZ = worldPosition->posZ;
        pSprite->scale = rand() % 0x1000 + 0x1000;
    }
    (taskPtr->step)++;
    if (taskPtr->step >= 41) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Acquires data block zero, raises equal RGB bytes from zero over four counts and fades them from
// 128 over four; later retires before drawing/pause. Selects Bahamut resource fields zero when
// first task short is zero or one otherwise, sets command 0x2E and variant short zero. Installs a
// billboard at task XYZ using signed field 0x14 and final argument zero, then submits into
// g_BattleCurrentOrderingTable plus 0x70 with shift twelve, chaining
// g_BattleSummonBahamutDrawHandle. Only counter advancement is pause-gated.
// FUNCTION: FF7 0x00499388
void __cdecl BattleFx_SummonBahamutFadingBillboardSpriteTask(void)
{
    int stage;
    BattleFxBahamutFadingSpriteTask *sprite;

    sprite = (BattleFxBahamutFadingSpriteTask *)BATTLE_CURRENT_TASK_SLOT;
    g_pBattleSummonBahamutFade = (BattleFxBahamutFade *)Gfx_GetPsxScratchpadPtr(0);
    stage = sprite->step;
    if (stage < 4) {
        g_pBattleSummonBahamutFade->fadeCurrent = (stage << 7) / 4;
        g_pBattleSummonBahamutFade->fadePrev = g_pBattleSummonBahamutFade->fadeCurrent;
        g_pBattleSummonBahamutFade->fadeOld = g_pBattleSummonBahamutFade->fadePrev;
    } else {
        stage -= 4;
        if (stage < 4) {
            g_pBattleSummonBahamutFade->fadeCurrent = stage * -128 / 4 + 128;
            g_pBattleSummonBahamutFade->fadePrev = g_pBattleSummonBahamutFade->fadeCurrent;
            g_pBattleSummonBahamutFade->fadeOld = g_pBattleSummonBahamutFade->fadePrev;
        } else {
            sprite->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    if (sprite->ownerSlot == 0) {
        g_pBattleSummonBahamutFade->resourceId = Gfx_ModelTableGetLoadedRecord(0, g_BattleSummonBahamutLoadedResTable);
        g_pBattleSummonBahamutFade->altResourceId = Gfx_ModelTableGetSpriteObject(0, g_BattleSummonBahamutLoadedResTable);
    } else {
        g_pBattleSummonBahamutFade->resourceId = Gfx_ModelTableGetLoadedRecord(1, g_BattleSummonBahamutLoadedResTable);
        g_pBattleSummonBahamutFade->altResourceId = Gfx_ModelTableGetSpriteObject(1, g_BattleSummonBahamutLoadedResTable);
    }
    g_pBattleSummonBahamutFade->command = 0x2E;
    g_pBattleSummonBahamutFade->frameIndex = 0;
    BattleFx_InstallBillboardMatrix((char *)sprite + 4, (short)sprite->scale, 0);
    g_BattleSummonBahamutDrawHandle = BattleFx_SubmitSpriteDraw(g_pBattleSummonBahamutFade, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSummonBahamutDrawHandle);
    if (g_BattleIsPaused == 0) {
        (sprite->step)++;
    }
}

// Original 739-byte native interval acquires data block zero and partitions two thirty-byte
// matrices plus scale vector. The fade level of g_BattleSummonBahamutModelDrawRequest1 holds
// 4096 for ten counts, falls over twenty-five, holds zero fifty and rises over sixty; later retires
// before drawing/pause. First matrix translation is (0,-20100,0), rotation uses
// g_BattleSummonBahamutScaleVector with Y count times sixteen and uniform scale 8192. Composes
// g_BattleSummonBahamutTransformMatrix then g_BattleCameraTransform, installs it, sets descriptor
// field four 0xA0, refreshes Bahamut resource field two and submits into destination plus 0x406C
// with shift zero. Second pass changes Y translation to -20000 and rotation Y to count times -16,
// rebuilds/scales and composes g_BattleSummonBahamutComposedMatrix, installs, sets descriptor field
// four 0xA8 and submits into destination plus 0x70 with shift twelve. Both chain
// g_BattleSummonBahamutDrawHandle. While unpaused counts 35 through 64 set
// g_BattleSummonBahamutBlendTimer to count minus 34 and, when resource pointer is nonzero, call
// Gfx_ForEachPolygonSetPart with callback BattleFx_SummonBahamutBlendPolygonSetVertices
// (0x00497831) and its field four; then advance count.
// FUNCTION: FF7 0x00499540
void __cdecl BattleFx_SummonBahamutCounterRotatingModelPairTask(void)
{
    int padSlot; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxFixedScaleVec *scaling;
    int stage;
    PsxMatrix *composed;
    BattleFxBahamutMatrixPairBlock *rotMatrix;
    int unusedSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleTaskSlot *taskPtr;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    rotMatrix = (BattleFxBahamutMatrixPairBlock *)Gfx_GetPsxScratchpadPtr(0);
    composed = &rotMatrix->second;
    scaling = (BattleFxFixedScaleVec *)(composed + 1);
    /* VC5 consumes one register choice without emitting code. */
    if (REG_ALLOC_HINT) {
    }
    /* VC5 consumes one register choice without emitting code. */
    if (REG_ALLOC_HINT) {
    }
    stage = taskPtr->frameCounter;
    if (stage < 10) {
        g_BattleSummonBahamutModelDrawRequest1.head.fadeLevel = 4096;
    } else {
        stage -= 10;
        if (stage < 25) {
            g_BattleSummonBahamutModelDrawRequest1.head.fadeLevel = stage * -4096 / 25 + 4096;
        } else {
            stage -= 25;
            if (stage < 50) {
                g_BattleSummonBahamutModelDrawRequest1.head.fadeLevel = 0;
            } else {
                stage -= 50;
                if (stage < 60) {
                    g_BattleSummonBahamutModelDrawRequest1.head.fadeLevel = (stage << 12) / 60;
                } else {
                    taskPtr->ownerSlot = BATTLE_TASK_FREE;
                    return;
                }
            }
        }
    }
    rotMatrix->first.t[0] = 0;
    rotMatrix->first.t[1] = 0xFFFFB17C;
    rotMatrix->first.t[2] = 0;
    g_BattleSummonBahamutScaleVector.angleY = taskPtr->frameCounter << 4;
    RotMatrix((SVECTOR *)&g_BattleSummonBahamutScaleVector, (void *)rotMatrix);
    scaling->scaleZ = 0x2000 /* 2.0 */;
    scaling->scaleY = 0x2000 /* 2.0 */;
    scaling->scaleX = 0x2000 /* 2.0 */;
    ScaleMatrix(rotMatrix, scaling);
    MultiplyMatrix(&g_BattleSummonBahamutTransformMatrix, rotMatrix, composed);
    MultiplyMatrix((void *)g_BattleCameraTransform.matrix, composed, composed);
    SetRotMatrix(composed);
    SetTransMatrix(composed);
    g_BattleSummonBahamutModelDrawRequest1.head.flags = 0xA0;
    g_BattleSummonBahamutModelDrawRequest1.head.modelGroup = Gfx_ModelTableGetRenderObject(2, g_BattleSummonBahamutLoadedResTable);
    g_BattleSummonBahamutDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonBahamutModelDrawRequest1, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST, 0, g_BattleSummonBahamutDrawHandle);
    rotMatrix->first.t[1] = 0xFFFFB1E0;
    g_BattleSummonBahamutScaleVector.angleY = taskPtr->frameCounter * -0x10;
    RotMatrix((SVECTOR *)&g_BattleSummonBahamutScaleVector, (void *)rotMatrix);
    scaling->scaleZ = 0x2000 /* 2.0 */;
    scaling->scaleY = 0x2000 /* 2.0 */;
    scaling->scaleX = 0x2000 /* 2.0 */;
    ScaleMatrix(rotMatrix, scaling);
    MultiplyMatrix(&g_BattleSummonBahamutComposedMatrix, rotMatrix, composed);
    SetRotMatrix(composed);
    SetTransMatrix(composed);
    g_BattleSummonBahamutModelDrawRequest1.head.flags = 0xA8;
    g_BattleSummonBahamutDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonBahamutModelDrawRequest1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSummonBahamutDrawHandle);
    if (g_BattleIsPaused == 0) {
        stage = taskPtr->frameCounter;
        if (stage < 35) { /* empty first arm: lib/codegen.h */
        } else {
            stage -= 35;
            if (stage < 30) {
                g_BattleSummonBahamutBlendTimer = stage + 1;
                if (g_BattleSummonBahamutModelDrawRequest1.head.modelGroup != 0) {
                    Gfx_ForEachPolygonSetPart((DxGroupPartCallback)BattleFx_SummonBahamutBlendPolygonSetVertices, *(int *)((char *)g_BattleSummonBahamutModelDrawRequest1.head.modelGroup + 4));
                }
            }
        }
        (taskPtr->frameCounter)++;
    }
}

// Per-frame model draw of the billboard growth, driven by the slot's frame count. Frames 0 to 3
// grow the scale as frame * 0x1600 / 4 + 0x200 with scaleFraction zero. Frames 4 to 7 hold the
// scale at 0x1800 and ramp scaleFraction as (frame - 4) << 12 / 4. From frame 8 on the slot is
// retired (ownerSlot = BATTLE_TASK_FREE) and the function returns. The scale is multiplied by the slot's
// scale (>> 12) and installed as the billboard matrix at slot+4. The model, row 0x1B of
// g_BattleSummonBahamutLoadedResTable, is drawn black into g_BattleCurrentOrderingTable+0x4084 at
// shift 0. The frame count advances only while the battle is not paused.
// FUNCTION: FF7 0x00499823
void __cdecl BattleFx_SummonBahamutBillboardGrowingModelTask(void)
{
    int stage;
    int scale;
    BattleFxBahamutBillboardGrowingTask *task;

    task = (BattleFxBahamutBillboardGrowingTask *)BATTLE_CURRENT_TASK_SLOT;
    stage = task->frameCount;
    if (stage < 4) {
        scale = stage * 0x1600 / 4 + 0x200;
        g_BattleSummonBahamutModelDrawRequest1.fadeLevel2 = 0;
    } else {
        stage -= 4;
        if (stage < 4) {
            scale = 0x1800 /* 1.5 */;
            g_BattleSummonBahamutModelDrawRequest1.fadeLevel2 = (stage << 12) / 4;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    scale = scale * task->scale >> 12;
    BattleFx_SummonBahamutInstallBillboardMatrix((BattleFxBillboardVector *)((int)task + 4), scale, 0);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSummonBahamutModelDrawRequest1.modelGroup2 = Gfx_ModelTableGetRenderObject(27, g_BattleSummonBahamutLoadedResTable);
    g_BattleSummonBahamutDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonBahamutModelDrawRequest1.modelGroup2, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_BattleSummonBahamutDrawHandle);
    if (g_BattleIsPaused == 0) {
        (task->frameCount)++;
    }
}

// Writes the short value into g_BattleSummonBahamutBillboardWorkingBlob.value, copies it on to
// g_BattleSummonBahamutTimer and g_BattleSummonBahamutUnused, loads the matrix at
// g_BattleSummonBahamutComposedMatrix (SetRotMatrix/SetTransMatrix) and
// RotTransPoint(vector, &g_BattleSummonBahamutBillboardWorkingBlob.x, &p) transforms vector into
// the three ints x, y and z of the blob. When distance is nonzero it normalizes that vector
// (NormalizeVectorFixed) and adds unit * distance >> 12 to each component. Then loads the matrix
// at g_BattleSummonBahamutUnused and returns its address.
// FUNCTION: FF7 0x0049993f
int __cdecl BattleFx_SummonBahamutInstallBillboardMatrix(BattleFxBillboardVector *vector, short value, int distance)
{
    int result;
    VECTOR normal;

    g_BattleSummonBahamutBillboardWorkingBlob.value = value;
    g_BattleSummonBahamutTimer = g_BattleSummonBahamutBillboardWorkingBlob.value;
    g_BattleSummonBahamutUnused[0] = g_BattleSummonBahamutTimer;
    SetRotMatrix(&g_BattleSummonBahamutComposedMatrix);
    SetTransMatrix(&g_BattleSummonBahamutComposedMatrix);
    RotTransPoint((void *)vector, &g_BattleSummonBahamutBillboardWorkingBlob.x, &result);
    if (distance != 0) {
        NormalizeVectorFixed(&g_BattleSummonBahamutBillboardWorkingBlob.x, &normal);
        g_BattleSummonBahamutBillboardWorkingBlob.x += normal.vx * distance >> 12;
        g_BattleSummonBahamutBillboardWorkingBlob.y += normal.vy * distance >> 12;
        g_BattleSummonBahamutBillboardWorkingBlob.z += normal.vz * distance >> 12;
    }
    SetRotMatrix(&g_BattleSummonBahamutUnused);
    SetTransMatrix(&g_BattleSummonBahamutUnused);
    return (int)&g_BattleSummonBahamutUnused;
}

// Acquires data block zero, clears descriptor term for ten counts and ramps it toward 4096 over
// five; later retires before draw/pause. Builds rotation through BattleFx_SummonBahamutBuildEulerRotationMatrix from saved task
// angles and XYZ translation, scales uniformly by task field 0x14, then composes g_BattleSummonBahamutTransformMatrix
// followed by g_BattleCameraTransform. Installs, sets black colour, refreshes Bahamut resource field 0x1A and
// submits into g_BattleCurrentOrderingTable plus 0x70 with shift twelve, chaining g_BattleSummonBahamutDrawHandle. While unpaused
// adds saved growth 0x16 to scale, damps growth by 0xF00 shifted twelve and advances count.
// FUNCTION: FF7 0x00499a1a
void __cdecl BattleFx_SummonBahamutExpandingModelTask(void)
{
    int stage;
    BattleFxBahamutExpandingModelTask *scalePtr;
    BattleFxBahamutExpandingModel *task;
    BattleFxBahamutMatrixScaleBlock *matrix;

    task = (BattleFxBahamutExpandingModel *)BATTLE_CURRENT_TASK_SLOT;
    matrix = (BattleFxBahamutMatrixScaleBlock *)Gfx_GetPsxScratchpadPtr(0);
    scalePtr = &matrix->scale;
    stage = task->frame;
    if (stage < 10) {
        g_BattleSummonBahamutModelDrawRequest2.fadeLevel = 0;
    } else {
        stage -= 10;
        if (stage < 5) {
            g_BattleSummonBahamutModelDrawRequest2.fadeLevel = (stage << 12) / 5;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    scalePtr->scale = task->scale;
    scalePtr->prevScale = scalePtr->scale;
    scalePtr->prevPrevScale = scalePtr->prevScale;
    BattleFx_SummonBahamutBuildEulerRotationMatrix((BattleFxBahamutEulerRotation *)((int)task + 0xC), matrix);
    matrix->matrix.t[0] = task->posX;
    matrix->matrix.t[1] = task->posY;
    matrix->matrix.t[2] = task->posZ;
    ScaleMatrix(matrix, scalePtr);
    MultiplyMatrix(&g_BattleSummonBahamutTransformMatrix, matrix, matrix);
    MultiplyMatrix((void *)g_BattleCameraTransform.matrix, matrix, matrix);
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSummonBahamutModelDrawRequest2.modelGroup = Gfx_ModelTableGetRenderObject(26, g_BattleSummonBahamutLoadedResTable);
    g_BattleSummonBahamutDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonBahamutModelDrawRequest2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSummonBahamutDrawHandle);
    if (g_BattleIsPaused == 0) {
        task->scale += task->scaleStep;
        task->scaleStep = task->scaleStep * 0xF00 /* 0.9375 */ >> 12;
        (task->frame)++;
    }
}

// Builds a rotation matrix in `matrix` using only the source Y rotation initially, then applies source X rotation
// and source Z rotation in that order.
// FUNCTION: FF7 0x00499bdb
void __cdecl BattleFx_SummonBahamutBuildEulerRotationMatrix(BattleFxBahamutEulerRotation *rotation, void *matrix)
{
    short yawVector[3];

    yawVector[0] = 0;
    yawVector[1] = rotation->rotY;
    yawVector[2] = 0;
    RotMatrix((SVECTOR *)yawVector, (int *)matrix);
    RotateMatrixX(rotation->rotX, matrix);
    RotateMatrixZ(rotation->rotZ, matrix);
}

// Acquires retained scratch once using a flag at g_BattleSummonBahamutCameraEye plus 0x2C. First fifteen counts
// set orange RGB and lower descriptor term from 4096; next ten set black RGB and ramp term toward
// 4096; later retire before drawing/pause. Builds saved rotation through
// BattleFx_SummonBahamutBuildEulerRotationMatrix and task XYZ translation, scales uniformly by
// field 0x14 and composes g_BattleSummonBahamutComposedMatrix. Installs, refreshes Bahamut resource
// field nine and submits into g_BattleCurrentOrderingTable plus 0x70 with shift twelve, chaining
// g_BattleSummonBahamutDrawHandle. While unpaused adds growth 0x16 to scale, damps growth by 0xF00
// shifted twelve, increases task Y angle by 0x40 and advances count.
// FUNCTION: FF7 0x00499c33
void __cdecl BattleFx_SummonBahamutSpinningExpandingModelTask(void)
{
    int stage;
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxSpinningModelTask *task;

    task = (BattleFxSpinningModelTask *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonBahamutCameraEye[0x2C] & 1) == 0) {
        *(char *)((char *)&g_BattleSummonBahamutCameraEye + 0x2C) |= 1;
        g_BattleSummonBahamutSpinningScratch = (BattleFxBahamutExpandState *)Gfx_GetPsxScratchpadPtr(0);
    }
    stage = task->frameCount;
    if (stage < 15) {
        SetDrawColorRGB(0xFF, 0x80, 0);
        g_BattleSummonBahamutModelDrawRequest3.fadeLevel = stage * -4096 / 15 + 4096;
    } else {
        stage -= 15;
        if (stage < 10) {
            SetDrawColorRGB(0, 0, 0);
            g_BattleSummonBahamutModelDrawRequest3.fadeLevel = (stage << 12) / 10;
        } else {
            task->step = 0xFFFF;
            return;
        }
    }
    BattleFx_SummonBahamutBuildEulerRotationMatrix((BattleFxBahamutEulerRotation *)((int)task + 0xC), (char *)g_BattleSummonBahamutSpinningScratch + 0x18);
    *(int *)((char *)g_BattleSummonBahamutSpinningScratch + 0x2A) = task->posX;
    *(int *)((char *)g_BattleSummonBahamutSpinningScratch + 0x2E) = task->posY;
    *(int *)((char *)g_BattleSummonBahamutSpinningScratch + 0x32) = task->posZ;
    g_BattleSummonBahamutSpinningScratch->expandAmount = task->expandAmount;
    g_BattleSummonBahamutSpinningScratch->curValue = g_BattleSummonBahamutSpinningScratch->expandAmount;
    g_BattleSummonBahamutSpinningScratch->prevValue = g_BattleSummonBahamutSpinningScratch->curValue;
    ScaleMatrix((void *)((int)g_BattleSummonBahamutSpinningScratch + 0x18), (void *)((int)g_BattleSummonBahamutSpinningScratch + 8));
    MultiplyMatrix(&g_BattleSummonBahamutComposedMatrix, (void *)((int)g_BattleSummonBahamutSpinningScratch + 0x18), (void *)((int)g_BattleSummonBahamutSpinningScratch + 0x18));
    SetRotMatrix((void *)((int)g_BattleSummonBahamutSpinningScratch + 0x18));
    SetTransMatrix((void *)((int)g_BattleSummonBahamutSpinningScratch + 0x18));
    g_BattleSummonBahamutModelDrawRequest3.modelGroup = Gfx_ModelTableGetRenderObject(9, g_BattleSummonBahamutLoadedResTable);
    g_BattleSummonBahamutDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonBahamutModelDrawRequest3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSummonBahamutDrawHandle);
    if (g_BattleIsPaused == 0) {
        task->expandAmount += task->expandSpeed;
        task->expandSpeed = task->expandSpeed * 0xF00 /* 0.9375 */ >> 12;
        task->spinAngle += 0x40;
        (task->frameCount)++;
    }
}

// Returns while paused, acquires scratch vectors and installs g_BattleSummonBahamutTransformMatrix. Local emission term is
// count shifted nine AND 0xFF plus 0x200, yielding two BattleFx_SummonBahamutAnimatedSpriteTask records for ordinary counts
// and discarding remainder. Picks random point X -4000 through 3999, Y -4000 through -1 and Z 5000
// through 14999, transforms it and copies low shorts of each wide output component into record
// XYZ. Assigns size 0x3000 through 0x67FF. Advances count and retires at fifteen.
// FUNCTION: FF7 0x00499e6d
void __cdecl BattleFx_SummonBahamutSpawnAnimatedSpritesTask(void)
{
    int emitCount;
    int flagsAddr;
    BattleFxBurstSpawnerTask *pTaskSlot;
    BattleFxPosition3s *outPos;
    BattleFxBahamutAnimatedSpriteHeader *hdr;
    BattleFxBahamutAnimatedSpriteRecord *pSprite;

    pTaskSlot = (BattleFxBurstSpawnerTask *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    hdr = (BattleFxBahamutAnimatedSpriteHeader *)Gfx_GetPsxScratchpadPtr(0);
    outPos = (BattleFxPosition3s *)((int)hdr + 8);
    flagsAddr = (int)outPos + 0x10;
    SetRotMatrix(&g_BattleSummonBahamutTransformMatrix);
    SetTransMatrix(&g_BattleSummonBahamutTransformMatrix);
    emitCount = (pTaskSlot->spawnIndex << 9 & 0xFF) + 512;
    while (emitCount >= 256) {
        emitCount -= 256;
        pSprite = (BattleFxBahamutAnimatedSpriteRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonBahamutAnimatedSpriteTask);
        hdr->offsetX = rand() % 8000 - 4000;
        hdr->offsetY = rand() % 4000 - 4000;
        hdr->offsetZ = rand() % 10000 + 5000;
        RotTransPoint(hdr, (int *)outPos, (void *)flagsAddr);
        pSprite->posX = outPos->posX;
        pSprite->posY = outPos->posY;
        pSprite->posZ = outPos->posZ;
        pSprite->scale = rand() % 0x3800 + 0x3000;
    }
    (pTaskSlot->spawnIndex)++;
    if (pTaskSlot->spawnIndex >= 15) {
        pTaskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Per-frame sprite draw of the billboard effect in the current slot. It installs the billboard
// matrix for the slot's position with the short scale at depth -0x100, and loads row frameIndex +
// 0xA of g_BattleSummonBahamutLoadedResTable into the sprite request's resource and scene fields.
// It submits the sprite into g_BattleSummonBahamutDrawHandle. Unless the battle is paused, the
// frame advances, and the slot is retired (ownerSlot = BATTLE_TASK_FREE) once frameIndex reaches 0x10.
// FUNCTION: FF7 0x00499fef
void __cdecl BattleFx_SummonBahamutAnimatedSpriteTask(void)
{
    BattleFxAnimatedSpriteSlot *task;

    task = (BattleFxAnimatedSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix((char *)task + 4, (short)task->scale, 0xFFFFFF00);
    g_BattleSummonBahamutSpriteDrawRequest1.resourceId = Gfx_ModelTableGetLoadedRecord(task->frameIndex + 0xA, g_BattleSummonBahamutLoadedResTable);
    g_BattleSummonBahamutSpriteDrawRequest1.altResourceId = Gfx_ModelTableGetSpriteObject(task->frameIndex + 0xA, g_BattleSummonBahamutLoadedResTable);
    g_BattleSummonBahamutDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleSummonBahamutSpriteDrawRequest1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSummonBahamutDrawHandle);
    if (g_BattleIsPaused == 0) {
        (task->frameIndex)++;
        if (task->frameIndex >= 16) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

// Grows scale toward 0x180 over twenty-five counters, holds it for five and grows it toward 0x1000
// during a final five-counter request ramp, then retires before drawing even while paused.
// Installs g_BattleSummonBahamutModelOffset through BattleFx_SummonBahamutInstallBillboardMatrix, selects model field five and submits a black draw;
// advances only when unpaused.
// FUNCTION: FF7 0x0049a0b9
void __cdecl BattleFx_SummonBahamutBlackGrowingModelTask(void)
{
    int scale;
    int stage;
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    stage = task->frameCounter;
    if (stage < 25) {
        scale = stage * 0x180 / 25;
        g_BattleSummonBahamutModelDrawRequest4.fadeLevel = 0;
    } else {
        stage -= 25;
        if (stage < 5) {
            scale = 0x180;
            g_BattleSummonBahamutModelDrawRequest4.fadeLevel = 0;
        } else {
            stage -= 5;
            if (stage < 5) {
                scale = stage * 0xE80 / 5 + 0x180;
                g_BattleSummonBahamutModelDrawRequest4.fadeLevel = (stage << 12) / 5;
            } else {
                task->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    BattleFx_SummonBahamutInstallBillboardMatrix(&g_BattleSummonBahamutModelOffset, (short)scale, 0);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSummonBahamutModelDrawRequest4.modelGroup = Gfx_ModelTableGetRenderObject(5, g_BattleSummonBahamutLoadedResTable);
    g_BattleSummonBahamutDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonBahamutModelDrawRequest4, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_BattleSummonBahamutDrawHandle);
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
    }
}

// Returns while paused. Local emission term is count shifted ten AND 0xFF plus 0x400, yielding four
// BattleFx_SummonBahamutStreakLineTask records for ordinary counts and discarding remainder. Sets
// random low-twelve-bit X angle, Y angle 0xFC00 and random Z angle -0x400 through 0x3FF. Writes
// field four from rand remainder with divisor -500 plus thousand, yielding 1000 through 1499 for
// nonnegative rand, and field six to its negative divided by ten. Leaves other record fields
// unchanged here. Advances count and retires at thirty.
// FUNCTION: FF7 0x0049a1e9
void __cdecl BattleFx_SummonBahamutSpawnStreakLinesTask(void)
{
    BattleFxBurstSpawnerTask *spawnerPtr;
    int rollZ;
    BattleFxStreakLineSlot *streak;
    int emitAmount;
    int startValue;

    spawnerPtr = (BattleFxBurstSpawnerTask *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    emitAmount = (spawnerPtr->spawnIndex << 0xA & 0xFF) + 1024;
    while (emitAmount >= 256) {
        streak = (BattleFxStreakLineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonBahamutStreakLineTask);
        streak->angle = rand() & 0xFFF;
        streak->angleY = 0xFC00 /* 5670 deg */;
        if (DEAD_IN_RETAIL != 0) {
            rollZ = 0x400 /* 90 deg */;
        } else {
            rollZ = (rand() % 0x800) - 0x400;
        }
        streak->angleZ = rollZ;
        if (DEAD_IN_RETAIL != 0) {
            startValue = 0x1F4;
        } else {
            startValue = rand() % -500 + 1000;
        }
        streak->startTime = startValue;
        streak->timeStep = -streak->startTime / 10;
        emitAmount -= 256;
    }
    (spawnerPtr->spawnIndex)++;
    if (spawnerPtr->spawnIndex >= 30) {
        spawnerPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Acquires retained scratch once and while unpaused advances task field four by field six before
// drawing. Builds saved rotation with translation from g_BattleSummonBahamutModelOffset/86c,
// composes g_BattleSummonBahamutComposedMatrix and installs/refreshes transform state. Eases count
// shifted twelve divided by ten with parameter 0x1FF, forms two X-axis points at field four and
// field four plus remaining fraction times 500 and calls Gfx_TransformTriangleReturnDepthAlt with
// the second point twice. When flags 0x60000 are clear writes primitive words
// 0x04000000,0x5200FFFF,0x02008080 through g_BattleSummonBahamutDrawHandle and submits. While
// unpaused advances count and retires at ten.
// FUNCTION: FF7 0x0049a323
void __cdecl BattleFx_SummonBahamutStreakLineTask(void)
{
    BattleFxBahamutStreakLine *pStreak;
    int easedFraction;
    BattleFxGreatGospelRenderStateBlock *packet;

    pStreak = (BattleFxBahamutStreakLine *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonBahamutCameraFlag3 & 1) == 0) {
        g_BattleSummonBahamutCameraFlag3 |= 1;
        g_BattleSummonBahamutScratchVector = (BattleFxVertex *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_BattleIsPaused == 0) {
        pStreak->posX += pStreak->velocityX;
    }
    RotMatrix((SVECTOR *)((char *)pStreak + 0xC), (void *)((int)g_BattleSummonBahamutScratchVector + 0x10));
    *(int *)((char *)g_BattleSummonBahamutScratchVector + 0x22) = g_BattleSummonBahamutModelOffset.posX;
    *(int *)((char *)g_BattleSummonBahamutScratchVector + 0x26) = g_BattleSummonBahamutModelOffset.posY;
    *(int *)((char *)g_BattleSummonBahamutScratchVector + 0x2A) = (g_BattleSummonBahamutModelOffset.posZ);
    MultiplyMatrix(&g_BattleSummonBahamutComposedMatrix, (void *)((int)g_BattleSummonBahamutScratchVector + 0x10), (void *)((int)g_BattleSummonBahamutScratchVector + 0x10));
    SetRotMatrix((void *)((int)g_BattleSummonBahamutScratchVector + 0x10));
    SetTransMatrix((void *)((int)g_BattleSummonBahamutScratchVector + 0x10));
    RefreshGlobalTransformState();
    packet = (BattleFxGreatGospelRenderStateBlock *)g_BattleSummonBahamutDrawHandle;
    easedFraction = (pStreak->frameCount << 12) / 10;
    easedFraction = BattleFx_BahamutApplyCameraEasing(easedFraction, 0x1FF);
    g_BattleSummonBahamutScratchVector->x = pStreak->posX;
    g_BattleSummonBahamutScratchVector->baseY = 0;
    g_BattleSummonBahamutScratchVector->baseZ = 0;
    g_BattleSummonBahamutScratchVector->endX = pStreak->posX + ((4096 - easedFraction) * 0x1F4 >> 12);
    g_BattleSummonBahamutScratchVector->z = 0;
    g_BattleSummonBahamutScratchVector->y = 0;
    g_BattleSummonBahamutScratchVector->depth = Gfx_TransformTriangleReturnDepthAlt(g_BattleSummonBahamutScratchVector, (SVECTOR *)((int)g_BattleSummonBahamutScratchVector + 8), (SVECTOR *)((int)g_BattleSummonBahamutScratchVector + 8), (char *)g_BattleSummonBahamutScratchVector + 0x34, (char *)g_BattleSummonBahamutScratchVector + 0x38);
    if ((g_BattleSummonBahamutScratchVector->flags & 0x60000) == 0) {
        packet->tag = 0x4000000;
        packet->color0 = 0x5200FFFF;
        packet->color1 = 0x2008080;
        Gfx_DrawPsxPrimitive(packet);
    }
    if (g_BattleIsPaused == 0) {
        (pStreak->frameCount)++;
        if (pStreak->frameCount >= 10) {
            pStreak->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

// Acquires data block zero and computes count fraction over 55. Sets retained scale Y from
// (g_BattleActorRecords[3].posY plus 0x6B6C) shifted twelve divided by 2000 with only an upper
// clamp 0x7FFF. Builds rotation with Y count shifted seven and translation from global XYZ with Y
// plus 2000, scales and composes g_BattleCameraTransform. Installs, sets black colour, writes
// descriptor fraction and draws Bahamut field three. Concatenates
// g_BattleSummonBahamutComposedMatrix with g_BattleSummonBahamutModelDrawRequest6 + 0x10 preserving
// translation into scratch, installs only rotation and draws field four; its descriptor alternates
// between fraction and fraction plus remaining fraction shifted eight/right twelve. Toggles
// g_BattleSummonBahamutTargetActorIdx even while paused. Unpaused advances count/retire at 55, then
// still changes task Y by saved velocity and rotates g_BattleSummonBahamutModelDrawRequest6 + 0x10
// about Y by -160.
// FUNCTION: FF7 0x0049a55b
void __cdecl BattleFx_SummonBahamutRotatingModelPairTask(void)
{
    int heightScale;
    BattleFxBahamutRotatingPair *pair;
    int padSlot; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int fraction;
    int clampedScale;
    struct BattleFxShardTransformBatch *matrix;

    pair = (BattleFxBahamutRotatingPair *)BATTLE_CURRENT_TASK_SLOT;
    matrix = (struct BattleFxShardTransformBatch *)Gfx_GetPsxScratchpadPtr(0);
    fraction = (pair->frameIndex << 12) / 55;
    heightScale = (g_BattleActorRecords[3].posY + 0x6B6C << 12) / 2000;
    if (heightScale <= 0x7FFF) {
        clampedScale = heightScale;
    } else {
        clampedScale = 0x7FFF;
    }
    g_BattleSummonBahamutModelDrawRequest6.scale.scaleY = clampedScale;
    g_BattleSummonBahamutCentroid[1] = pair->frameIndex << 7;
    RotMatrix((SVECTOR *)&g_BattleSummonBahamutCentroid, (void *)matrix);
    ScaleMatrix(matrix, &g_BattleSummonBahamutModelDrawRequest6.scale);
    matrix->posX = g_BattleActorRecords[3].posX;
    matrix->posY = g_BattleActorRecords[3].posY + 2000;
    matrix->posZ = g_BattleActorRecords[3].posZ;
    MultiplyMatrix((void *)g_BattleCameraTransform.matrix, matrix, matrix);
    SetRotMatrix((void *)matrix);
    SetTransMatrix(matrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSummonBahamutModelDrawRequest5.fadeLevel = fraction;
    g_BattleSummonBahamutModelDrawRequest5.modelGroup = Gfx_ModelTableGetRenderObject(3, g_BattleSummonBahamutLoadedResTable);
    g_BattleSummonBahamutDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonBahamutModelDrawRequest5, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSummonBahamutDrawHandle);
    ConcatMatrixPreservingTranslation(&g_BattleSummonBahamutComposedMatrix, &g_BattleSummonBahamutModelDrawRequest6.matrix, matrix);
    SetRotMatrix((void *)matrix);
    if (g_BattleSummonBahamutTargetActorIdx != 0) {
        g_BattleSummonBahamutModelDrawRequest6.head.fadeLevel = fraction + (fraction * 0 >> 12);
    } else {
        g_BattleSummonBahamutModelDrawRequest6.head.fadeLevel = fraction + (4096 - fraction << 8 >> 12);
    }
    g_BattleSummonBahamutTargetActorIdx ^= 1;
    g_BattleSummonBahamutModelDrawRequest6.head.modelGroup = Gfx_ModelTableGetRenderObject(4, g_BattleSummonBahamutLoadedResTable);
    g_BattleSummonBahamutDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonBahamutModelDrawRequest6, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSummonBahamutDrawHandle);
    if (g_BattleIsPaused == 0) {
        (pair->frameIndex)++;
        if (pair->frameIndex >= 55) {
            pair->ownerSlot = BATTLE_TASK_FREE;
        }
        pair->angle += pair->angleStep;
        RotateMatrixY(0xFFFFFF60, &g_BattleSummonBahamutModelDrawRequest6.matrix);
    }
}

// Acquires retained scratch and composes g_BattleSummonBahamutComposedMatrix with g_BattleSummonBahamutBeamTransformMatrix before count-zero
// initialization of the latter to X rotation 0x630. First thirty counts grow travel/length toward
// 15000, clear both descriptor terms and while unpaused emit BattleFx_SummonBahamutBeamEasedScaleModelTask every five counts with
// retained matrix pointer and travel short. Next five hold travel 15000, shrink length and raise
// descriptor terms; on entry while unpaused emit BattleFx_SummonBahamutBillboardGrowingModelTask at transformed (0,0,-15000) plus
// composed translation with size 8192. Later retire before drawing/pause. Set retained Z
// translation to thousand minus travel, compose/scale first pass but install rotation from
// g_BattleSummonBahamutTransformMatrix2 and translation from composed scratch, then draw Bahamut field six. Second pass
// uses task rotation and local Z -travel, composes it, sets scale Z from length shifted twelve
// divided by 1800 with only upper clamp 0x7FFF, scales/installs and draws field seven. Both chain
// g_BattleSummonBahamutDrawHandle at destination plus 0x70, shift twelve. Unpaused increments count and Z angle 0x80.
// FUNCTION: FF7 0x0049a7ab
void __cdecl BattleFx_SummonBahamutExtendingBeamTask(void)
{
    int fraction;
    int scaleRaw;
    BattleFxBahamutExtendingBeamTask *beam;
    int stage;
    int travel;
    int clampedScale;
    int len;
    BattleFxExtendingBeamSlot *child;

    beam = (BattleFxBahamutExtendingBeamTask *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonBahamutBeamInitFlag & 1) == 0) {
        g_BattleSummonBahamutBeamInitFlag |= 1;
        g_BattleSummonBahamutBeamScratch = (BattleSummonTaskRecord *)Gfx_GetPsxScratchpadPtr(0);
    }
    MultiplyMatrix(&g_BattleSummonBahamutComposedMatrix, g_BattleSummonBahamutBeamTransformMatrix, (void *)((int)g_BattleSummonBahamutBeamScratch + 8));
    if (beam->step == 0) {
        g_BattleSummonBahamutBeamScratch->offsetX = 0x630;
        g_BattleSummonBahamutBeamScratch->offsetY = 0;
        g_BattleSummonBahamutBeamScratch->offsetZ = 0;
        RotMatrix((SVECTOR *)g_BattleSummonBahamutBeamScratch, g_BattleSummonBahamutBeamTransformMatrix);
    }
    stage = beam->step;
    if (stage < 30) {
        fraction = (stage << 12) / 30;
        len = fraction * 0x3A98 >> 12;
        travel = len;
        g_BattleSummonBahamutModelDrawRequest7.extendAmount = 0;
        g_BattleSummonBahamutModelDrawRequest7.prevExtendAmount = g_BattleSummonBahamutModelDrawRequest7.extendAmount;
        if (g_BattleIsPaused == 0 && stage % 5 == 0) {
            child = (BattleFxExtendingBeamSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonBahamutBeamEasedScaleModelTask);
            child->transformMatrix = &g_BattleSummonBahamutBeamTransformMatrix;
            child->posZ = travel;
        }
    } else {
        stage -= 30;
        if (stage < 5) {
            fraction = (stage << 12) / 5;
            travel = 0x3A98;
            len = (fraction * -0x3A98 >> 12) + 15000;
            g_BattleSummonBahamutModelDrawRequest7.extendAmount = fraction;
            g_BattleSummonBahamutModelDrawRequest7.prevExtendAmount = g_BattleSummonBahamutModelDrawRequest7.extendAmount;
            if (g_BattleIsPaused == 0 && stage == 0) {
                child = (BattleFxExtendingBeamSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonBahamutBillboardGrowingModelTask);
                g_BattleSummonBahamutBeamScratch->offsetX = 0;
                g_BattleSummonBahamutBeamScratch->offsetY = 0;
                g_BattleSummonBahamutBeamScratch->offsetZ = 0xC568;
                TransformVectorToInt16((void *)((int)g_BattleSummonBahamutBeamScratch + 8), (void *)g_BattleSummonBahamutBeamScratch, (void *)g_BattleSummonBahamutBeamScratch);
                child->posX = g_BattleSummonBahamutBeamScratch->offsetX + *(int *)((char *)g_BattleSummonBahamutBeamScratch + 0x1A);
                child->posY = g_BattleSummonBahamutBeamScratch->offsetY + *(int *)((char *)g_BattleSummonBahamutBeamScratch + 0x1E);
                child->posZ = g_BattleSummonBahamutBeamScratch->offsetZ + *(int *)((char *)g_BattleSummonBahamutBeamScratch + 0x22);
                child->scale = 0x2000 /* 2.0 */;
            }
        } else {
            beam->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    (*(int *)((char *)&g_BattleSummonBahamutBeamVectorBlob + 0x12)) = -travel + 0x3E8;
    MultiplyMatrix((void *)((int)g_BattleSummonBahamutBeamScratch + 8), g_BattleSummonBahamutTransformMatrix2, (void *)((int)g_BattleSummonBahamutBeamScratch + 0x26));
    ScaleMatrix((void *)((int)g_BattleSummonBahamutBeamScratch + 0x26), &g_BattleSummonBahamutBeamVector);
    SetRotMatrix(g_BattleSummonBahamutTransformMatrix2);
    SetTransMatrix((void *)((int)g_BattleSummonBahamutBeamScratch + 0x26));
    SetDrawColorRGB(0, 0, 0);
    g_BattleSummonBahamutModelDrawRequest7.auxHandle = Gfx_ModelTableGetRenderObject(6, g_BattleSummonBahamutLoadedResTable);
    g_BattleSummonBahamutDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)(((char *)&g_BattleSummonBahamutModelDrawRequest7 + 0x30)), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSummonBahamutDrawHandle);
    RotMatrix((SVECTOR *)((char *)beam + 0xC), (void *)((int)g_BattleSummonBahamutBeamScratch + 0x26));
    g_BattleSummonBahamutBeamScratch->transX = 0;
    g_BattleSummonBahamutBeamScratch->transY = 0;
    g_BattleSummonBahamutBeamScratch->transZ = -travel;
    MultiplyMatrix((void *)((int)g_BattleSummonBahamutBeamScratch + 8), (void *)((int)g_BattleSummonBahamutBeamScratch + 0x26), (void *)((int)g_BattleSummonBahamutBeamScratch + 0x26));
    scaleRaw = (len << 12) / 1800;
    if (scaleRaw < 0x7FFF) {
        clampedScale = scaleRaw;
    } else {
        clampedScale = 0x7FFF;
    }
    g_BattleSummonBahamutBaseDistance2[0] = clampedScale;
    ScaleMatrix((void *)((int)g_BattleSummonBahamutBeamScratch + 0x26), &g_BattleSummonBahamutBaseDistance1);
    SetRotMatrix((void *)((int)g_BattleSummonBahamutBeamScratch + 0x26));
    SetTransMatrix((void *)((int)g_BattleSummonBahamutBeamScratch + 0x26));
    g_BattleSummonBahamutModelDrawRequest7.extraHandle = Gfx_ModelTableGetRenderObject(7, g_BattleSummonBahamutLoadedResTable);
    g_BattleSummonBahamutDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)(((char *)&g_BattleSummonBahamutModelDrawRequest7 + 0x40)), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSummonBahamutDrawHandle);
    if (g_BattleIsPaused == 0) {
        (beam->step)++;
        beam->length += 128;
    }
}

// Acquires retained scratch once, clears descriptor term for five counts and ramps it toward 4096
// over five; later retires before drawing/pause. Composes g_BattleSummonBahamutComposedMatrix with
// the saved task matrix pointer at 0x18, changes g_BattleSummonBahamutModelDrawRequest7 + 0x2A to
// negative saved travel short and composes g_BattleSummonBahamutModelDrawRequest7.partHandle
// afterward. Eases count fraction over ten with parameter 0x3FF and scales uniformly by term times
// 0x1200 shifted twelve. Installs, sets black colour, refreshes Bahamut resource field eight and
// submits into g_BattleCurrentOrderingTable plus 0x70 with shift twelve, chaining
// g_BattleSummonBahamutDrawHandle. Only counter advancement is pause-gated.
// FUNCTION: FF7 0x0049ac0a
void __cdecl BattleFx_SummonBahamutBeamEasedScaleModelTask(void)
{
    int stage;
    int eased;
    BattleFxEasedBeamSlot *taskPtr;

    taskPtr = (BattleFxEasedBeamSlot *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonBahamutCameraFlag2 & 1) == 0) {
        g_BattleSummonBahamutCameraFlag2 |= 1;
        (*(int *)&g_BattleSummonBahamutCameraScriptTable) = Gfx_GetPsxScratchpadPtr(0);
    }
    stage = taskPtr->frameIndex;
    if (stage < 5) {
        g_BattleSummonBahamutModelDrawRequest7.fadeLevel = 0;
    } else {
        stage -= 5;
        if (stage < 5) {
            g_BattleSummonBahamutModelDrawRequest7.fadeLevel = (stage << 12) / 5;
        } else {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    MultiplyMatrix(&g_BattleSummonBahamutComposedMatrix, taskPtr->transformMatrix, (void *)((*(int *)&g_BattleSummonBahamutCameraScriptTable) + 0x18));
    (*(int *)((char *)&g_BattleSummonBahamutModelDrawRequest7 + 0x2a)) = -taskPtr->travel;
    MultiplyMatrix((void *)((*(int *)&g_BattleSummonBahamutCameraScriptTable) + 0x18), (char *)&g_BattleSummonBahamutModelDrawRequest7 + 0x10, (void *)((*(int *)&g_BattleSummonBahamutCameraScriptTable) + 0x18));
    eased = (taskPtr->frameIndex << 12) / 10;
    eased = BattleFx_BahamutApplyCameraEasing(eased, 0x3FF);
    ((BattleFxBahamutCameraDelay *)(*(int *)&g_BattleSummonBahamutCameraScriptTable))->newSample = eased * 0x1200 /* 1.125 */ >> 12;
    ((BattleFxBahamutCameraDelay *)(*(int *)&g_BattleSummonBahamutCameraScriptTable))->midSample = ((BattleFxBahamutCameraDelay *)(*(int *)&g_BattleSummonBahamutCameraScriptTable))->newSample;
    ((BattleFxBahamutCameraDelay *)(*(int *)&g_BattleSummonBahamutCameraScriptTable))->oldSample = ((BattleFxBahamutCameraDelay *)(*(int *)&g_BattleSummonBahamutCameraScriptTable))->midSample;
    ScaleMatrix((void *)((*(int *)&g_BattleSummonBahamutCameraScriptTable) + 0x18), (void *)(*(int *)&g_BattleSummonBahamutCameraScriptTable));
    SetRotMatrix((void *)((*(int *)&g_BattleSummonBahamutCameraScriptTable) + 0x18));
    SetTransMatrix((void *)((*(int *)&g_BattleSummonBahamutCameraScriptTable) + 0x18));
    SetDrawColorRGB(0, 0, 0);
    g_BattleSummonBahamutModelDrawRequest7.modelGroup = Gfx_ModelTableGetRenderObject(8, g_BattleSummonBahamutLoadedResTable);
    g_BattleSummonBahamutDrawHandle = BattleFx_SubmitModelDraw(&g_BattleSummonBahamutModelDrawRequest7, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSummonBahamutDrawHandle);
    if (g_BattleIsPaused == 0) {
        (taskPtr->frameIndex)++;
    }
}

// Acquires retained scratch before returning while paused. Holds retained
// g_BattleSummonBahamutDefaultPosition point for 84 counts. Next sixty on entry allocates ring
// record one at the rotated point plus g_BattleSummonBahamutTransformMatrix translation, with angle
// g_BattleSummonBahamutRotation + 2 plus 0x800, and sets actor field six 0xC00; eases point Y from
// -27500 toward -4000 using parameter 0x280. Next 65 ease Y from -4000 toward -7000 using parameter
// 0xFFFFFE01. Next 105 reset Y to short 0xE4A8 on entry and at elapsed 104 allocate ring record two
// and retire; later values still reach the common update. Always rotates retained point through
// g_BattleSummonBahamutTransformMatrix, adds translation and writes XYZ shorts at actor 3's record
// (g_BattleActorRecords[3]) plus 0x166, then increments count even after retirement.
// FUNCTION: FF7 0x0049adec
void __cdecl BattleFx_SummonBahamutActorDescentTask(void)
{
    int stage;
    BattleTaskSlot *taskPtr;
    BattleActorFadeDescentRecord *pActor;
    BattleFxLightRigSlot *lightRig;
    int easedFraction;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    pActor = (BattleActorFadeDescentRecord *)&g_BattleActorRecords[3].modelId;
    if ((g_BattleSummonBahamutDescentInitFlag & 1) == 0) {
        g_BattleSummonBahamutDescentInitFlag |= 1;
        g_BattleSummonBahamutTempVector = (Vec3sPacked *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    stage = taskPtr->frameCounter;
    if (stage < 84) { /* empty first arm: lib/codegen.h */
    } else {
        stage -= 84;
        if (stage < 60) {
            if (stage == 0) {
                lightRig = (BattleFxLightRigSlot *)Battle_AllocSceneMessage(1);
                TransformVectorToInt16((void *)&g_BattleSummonBahamutTransformMatrix, &g_BattleSummonBahamutDefaultPosition, (void *)g_BattleSummonBahamutTempVector);
                lightRig->posX = g_BattleSummonBahamutTempVector->vx + g_BattleSummonBahamutTransformMatrix.t[0];
                lightRig->posY = g_BattleSummonBahamutTempVector->vy + g_BattleSummonBahamutTransformMatrix.t[1];
                lightRig->posZ = g_BattleSummonBahamutTempVector->vz + g_BattleSummonBahamutTransformMatrix.t[2];
                lightRig->rotationY = g_BattleSummonBahamutRotation[1] + 0x800;
                pActor->fadeStep = 3072;
            }
            easedFraction = (stage << 12) / 60;
            easedFraction = BattleFx_BahamutApplyCameraEasing(easedFraction, 0x280);
            g_BattleSummonBahamutDefaultPosition[1] = (easedFraction * 0x5BCC >> 12) - 27500;
        } else {
            stage -= 60;
            if (stage < 65) {
                easedFraction = (stage << 12) / 65;
                easedFraction = BattleFx_BahamutApplyCameraEasing(easedFraction, 0xFFFFFE01);
                g_BattleSummonBahamutDefaultPosition[1] = (easedFraction * -0xBB8 >> 12) - 4000;
            } else {
                stage -= 65;
                if (stage < 105) {
                    if (stage == 0) {
                        g_BattleSummonBahamutDefaultPosition[1] = -7000;
                    }
                    if (stage == 104) {
                        Battle_AllocSceneMessage(2);
                        taskPtr->ownerSlot = BATTLE_TASK_FREE;
                    }
                }
            }
        }
    }
    TransformVectorToInt16((void *)&g_BattleSummonBahamutTransformMatrix, &g_BattleSummonBahamutDefaultPosition, (void *)g_BattleSummonBahamutTempVector);
    pActor->posA = g_BattleSummonBahamutTempVector->vx + g_BattleSummonBahamutTransformMatrix.t[0];
    pActor->posB = g_BattleSummonBahamutTempVector->vy + g_BattleSummonBahamutTransformMatrix.t[1];
    pActor->posC = g_BattleSummonBahamutTempVector->vz + g_BattleSummonBahamutTransformMatrix.t[2];
    (taskPtr->frameCounter)++;
}
} /* extern "C" */
