// src/ff7/src/battle/battle_fx_summon_neo_bahamut.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Neo Bahamut summon effect: loads its resources, runs an eased camera script, and drives
//   a master timeline. Around it are PSX mesh drawing with fragment matrices, sprite tasks for
//   thunder, dust and balls, and model tasks for rocks, clouds, breath and light lines.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/api/graphics.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle_summon.h"
#include "ff7/enums/battle.h"
#include "lib/codegen.h"
#include "lib/enums/graphics.h"
extern "C" {

#pragma intrinsic(memset)
// clang-format off

// clang-format on

// clang-format off
unsigned char g_BattleFxSummonNeoBahamutSfxGroups[64] = {
    0x88, 0x02, 0x00, 0x00, 0x89, 0x02, 0x00, 0x00, 0x8a, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8b, 0x02, 0x00, 0x00, 0x8c, 0x02, 0x00, 0x00,
    0x8d, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8e, 0x02, 0x00, 0x00, 0x8f, 0x02, 0x00, 0x00, 0x90, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x91, 0x02, 0x00, 0x00, 0x92, 0x02, 0x00, 0x00, 0x93, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// GLOBAL: FF7 0x007f6d78
int g_BattleSummonNeoBahamutSoundTable = 0x007F6D38;
int g_BattleSummonNeoBahamutSoundTableHeadPtr[1] = {
    (int)((char *)&g_BattleFxSummonNeoBahamutSfxGroups + 0x10)
};
int g_BattleSummonNeoBahamutSoundTableDataTail[4] = {
    (int)((char *)&g_BattleFxSummonNeoBahamutSfxGroups + 0x20), (int)((char *)&g_BattleFxSummonNeoBahamutSfxGroups + 0x30), 0x0, 0x0
};
// GLOBAL: FF7 0x007f6d90
unsigned char g_BattleSummonNeoBahamutPolygonVertexBlob[9080] = {0x98, 0x07, 0x00, 0x00, 0x58, 0x01, 0xfc, 0xff, 0xda, 0xf9, 0x00, 0x00, 0x43, 0x00, 0xfc, 0xff, 0x7f, 0xf9, 0x00, 0x00, 0xd1, 0xff, 0xfc, 0xff, 0xe3, 0xff, 0x00, 0x00, 0x06, 0x03, 0xfc, 0xff, 0xd4, 0xfa, 0x00, 0x00, 0x19, 0x05, 0xfc, 0xff, 0x98, 0xfb, 0x00, 0x00, 0x77, 0x06, 0xfc, 0xff, 0x83, 0xfc, 0x00, 0x00, 0x1d, 0x07, 0xfc, 0xff, 0xe1, 0xfd, 0x00, 0x00, 0xb1, 0x06, 0xfc, 0xff, 0x46, 0xff, 0x00, 0x00, 0xe7, 0x05, 0xfc, 0xff, 0x05, 0x01, 0x00, 0x00, 0x69, 0x04, 0xfc, 0xff, 0x62, 0x02, 0x00, 0x00, 0x76, 0x04, 0xfc, 0xff, 0xee, 0x04, 0x00, 0x00, 0xc4, 0x03, 0xfc, 0xff, 0x23, 0x05, 0x00, 0x00, 0xc9, 0x01, 0xfc, 0xff, 0x14, 0x05, 0x00, 0x00, 0x68, 0xff, 0xfc, 0xff, 0x02, 0x06, 0x00, 0x00, 0xec, 0xfd, 0xfc, 0xff, 0x41, 0x06, 0x00, 0x00, 0x97, 0xfc, 0xfc, 0xff, 0xb3, 0x05, 0x00, 0x00, 0xf5, 0xfa, 0xfc, 0xff, 0x73, 0x04, 0x00, 0x00, 0x50, 0xf9, 0xfc, 0xff, 0x5f, 0x03, 0x00, 0x00, 0xc3, 0xf8, 0xfc, 0xff, 0x07, 0x02, 0x00, 0x00, 0x4a, 0xf9, 0xfc, 0xff, 0x09, 0x00, 0x00, 0x00, 0xf8, 0xf8, 0xfc, 0xff, 0x19, 0xfe, 0x00, 0x00, 0xfc, 0xf9, 0xfc, 0xff, 0xd7, 0xfc, 0x00, 0x00, 0x36, 0xfb, 0xfc, 0xff, 0x3e, 0xfb, 0x00, 0x00, 0x31, 0xfc, 0xfc, 0xff, 0x7e, 0xfa, 0x00, 0x00, 0xa2, 0xfe, 0xfc, 0xff, 0xbc, 0xfa, 0x00, 0x00, 0x5e, 0x05, 0xfc, 0xff, 0xc3, 0xf6, 0x00, 0x00, 0xcb, 0x02, 0xfc, 0xff, 0x45, 0xf5, 0x00, 0x00, 0x8f, 0x08, 0xfc, 0xff, 0xf1, 0xf7, 0x00, 0x00, 0x29, 0x0b, 0xfc, 0xff, 0xaf, 0xf9, 0x00, 0x00, 0x98, 0x0c, 0xfc, 0xff, 0xb7, 0xfc, 0x00, 0x00, 0xbf, 0x0b, 0xfc, 0xff, 0x86, 0xff, 0x00, 0x00, 0x5e, 0x0a, 0xfc, 0xff, 0x91, 0x02, 0x00, 0x00, 0x52, 0x08, 0xfc, 0xff, 0x71, 0x04, 0x00, 0x00, 0xf1, 0x07, 0xfc, 0xff, 0x4d, 0x08, 0x00, 0x00, 0x22, 0x06, 0xfc, 0xff, 0x21, 0x09, 0x00, 0x00, 0x29, 0x03, 0xfc, 0xff, 0x58, 0x09, 0x00, 0x00, 0x9f, 0xff, 0xfc, 0xff, 0xba, 0x0a, 0x00, 0x00, 0xc1, 0xfc, 0xfc, 0xff, 0x32, 0x0b, 0x00, 0x00, 0x1d, 0xfa, 0xfc, 0xff, 0x1b, 0x0a, 0x00, 0x00, 0x85, 0xf7, 0xfc, 0xff, 0x1d, 0x08, 0x00, 0x00, 0xaa, 0xf4, 0xfc, 0xff, 0x3d, 0x06, 0x00, 0x00, 0x6f, 0xf3, 0xfc, 0xff, 0x3f, 0x03, 0x00, 0x00, 0x48, 0xf4, 0xfc, 0xff, 0x09, 0x00, 0x00, 0x00, 0xbc, 0xf3, 0xfc, 0xff, 0xb9, 0xfc, 0x00, 0x00, 0xb8, 0xf5, 0xfc, 0xff, 0x46, 0xfa, 0x00, 0x00, 0xb1, 0xf7, 0xfc, 0xff, 0xb4, 0xf7, 0x00, 0x00, 0xd8, 0xf9, 0xfc, 0xff, 0x0d, 0xf6, 0x00, 0x00, 0x8e, 0xfd, 0xfc, 0xff, 0x6a, 0xf6, 0x00, 0x00, 0x32, 0x00, 0xfc, 0xff, 0x6a, 0xf4, 0x00, 0x00, 0x6f, 0x07, 0xfc, 0xff, 0x0b, 0xf3, 0x00, 0x00, 0x32, 0x04, 0xfc, 0xff, 0x2a, 0xf1, 0x00, 0x00, 0x78, 0x0b, 0xfc, 0xff, 0x88, 0xf4, 0x00, 0x00, 0x3e, 0x0f, 0xfc, 0xff, 0x0f, 0xf7, 0x00, 0x00, 0x7c, 0x11, 0xfc, 0xff, 0xce, 0xfb, 0x00, 0x00, 0x39, 0x10, 0xfc, 0xff, 0x00, 0x00, 0x00, 0x00, 0x52, 0x0e, 0xfc, 0xff, 0x32, 0x04, 0x00, 0x00, 0xf9, 0x0b, 0xfc, 0xff, 0x58, 0x06, 0x00, 0x00, 0x78, 0x0b, 0xfc, 0xff, 0x78, 0x0b, 0x00, 0x00, 0x1c, 0x08, 0xfc, 0xff, 0x02, 0x0d, 0x00, 0x00, 0x5a, 0x04, 0xfc, 0xff, 0x48, 0x0d, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0xfa, 0x0e, 0x00, 0x00, 0xce, 0xfb, 0xfc, 0xff, 0xab, 0x0f, 0x00, 0x00, 0xe4, 0xf7, 0xfc, 0xff, 0x0c, 0x0e, 0x00, 0x00, 0x88, 0xf4, 0xfc, 0xff, 0x78, 0x0b, 0x00, 0x00, 0x9a, 0xf0, 0xfc, 0xff, 0xe3, 0x08, 0x00, 0x00, 0xac, 0xee, 0xfc, 0xff, 0x32, 0x04, 0x00, 0x00, 0xc7, 0xef, 0xfc, 0xff, 0x00, 0x00, 0x00, 0x00, 0x09, 0xef, 0xfc, 0xff, 0x7e, 0xfb, 0x00, 0x00, 0xf4, 0xf1, 0xfc, 0xff, 0xe4, 0xf7, 0x00, 0x00, 0x88, 0xf4, 0xfc, 0xff, 0x88, 0xf4, 0x00, 0x00, 0xe4, 0xf7, 0xfc, 0xff, 0xf4, 0xf1, 0x00, 0x00, 0x88, 0xfc, 0xfc, 0xff, 0x68, 0xf2, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0xc7, 0xef, 0x00, 0x00, 0x00, 0xfc, 0x67, 0x0c, 0x67, 0x04, 0x00, 0x00, 0xbc, 0xfd, 0xa7, 0x0e, 0x22, 0x01, 0x00, 0x00, 0x1f, 0xfe, 0xca, 0x0e, 0x2c, 0x02, 0x00, 0x00, 0xf3, 0xfa, 0x83, 0x0c, 0xf9, 0x00, 0x00, 0x00, 0x4e, 0xfb, 0xb2, 0x0d, 0x56, 0x02, 0x00, 0x00, 0xe7, 0xfd, 0x7f, 0x0e, 0xec, 0xff, 0x00, 0x00, 0xa0, 0xfd, 0x62, 0x0f, 0x6a, 0x00, 0x00, 0x00, 0xa1, 0xf8, 0xf6, 0x0c, 0x72, 0xff, 0x00, 0x00, 0x95, 0xfd, 0x5b, 0x0e, 0x32, 0xff, 0x00, 0x00, 0x16, 0xfb, 0xb6, 0x0c, 0x8c, 0xfe, 0x00, 0x00, 0x2c, 0xfe, 0xec, 0x0e, 0x90, 0xfe, 0x00, 0x00, 0x02, 0xfc, 0x41, 0x0c, 0x48, 0xfb, 0x00, 0x00, 0xe4, 0xfe, 0x86, 0x0e, 0x7a, 0xfe, 0x00, 0x00, 0x9b, 0xfe, 0x5b, 0x0f, 0x9f, 0xfd, 0x00, 0x00, 0xf5, 0xfe, 0x63, 0x0c, 0x74, 0xfb, 0x00, 0x00, 0x87, 0xff, 0x8c, 0x0e, 0xa0, 0xfd, 0x00, 0x00, 0x04, 0x00, 0x91, 0x0e, 0x38, 0xfe, 0x00, 0x00, 0x7c, 0x01, 0xb2, 0x0b, 0x80, 0xf9, 0x00, 0x00, 0xc2, 0x00, 0x5e, 0x0e, 0x03, 0xfd, 0x00, 0x00, 0xdb, 0x02, 0xf2, 0x0a, 0x0a, 0xfb, 0x00, 0x00, 0x24, 0x01, 0x6e, 0x0e, 0x4d, 0xfe, 0x00, 0x00, 0x92, 0x02, 0xc9, 0x0e, 0xa8, 0xfd, 0x00, 0x00, 0xe9, 0x04, 0xae, 0x0b, 0x81, 0xfb, 0x00, 0x00, 0xb9, 0x03, 0x26, 0x0e, 0x24, 0xff, 0x00, 0x00, 0x93, 0x05, 0xed, 0x0c, 0xe0, 0xfc, 0x00, 0x00, 0xa6, 0x07, 0x30, 0x0c, 0x0e, 0xfe, 0x00, 0x00, 0xb5, 0x01, 0x4f, 0x0e, 0x30, 0xff, 0x00, 0x00, 0x19, 0x03, 0xf7, 0x0d, 0x12, 0x00, 0x00, 0x00, 0x50, 0x06, 0xcf, 0x0c, 0x93, 0x00, 0x00, 0x00, 0xd1, 0x02, 0xf3, 0x0e, 0xdf, 0x00, 0x00, 0x00, 0x73, 0x06, 0x3a, 0x0c, 0xf6, 0x01, 0x00, 0x00, 0xd6, 0x04, 0x20, 0x0c, 0x0f, 0x03, 0x00, 0x00, 0xd0, 0x01, 0x5e, 0x0e, 0xfc, 0x00, 0x00, 0x00, 0xb0, 0x02, 0x0c, 0x0e, 0xf3, 0x02, 0x00, 0x00, 0xb7, 0x02, 0x97, 0x0c, 0xaf, 0x06, 0x00, 0x00, 0xc2, 0x03, 0x52, 0x0c, 0x74, 0x04, 0x00, 0x00, 0xf8, 0x01, 0x44, 0x0c, 0x61, 0x06, 0x00, 0x00, 0x42, 0x01, 0x92, 0x0e, 0xf0, 0x01, 0x00, 0x00, 0x3f, 0x00, 0x24, 0x0f, 0x0a, 0x03, 0x00, 0x00, 0xd8, 0xff, 0x54, 0x0c, 0x4c, 0x05, 0x00, 0x00, 0xd8, 0xff, 0x01, 0x0f, 0x2a, 0x02, 0x00, 0x00, 0x0a, 0xff, 0xc6, 0x0e, 0xd7, 0x01, 0x00, 0x00, 0xc9, 0xfc, 0x6e, 0x0b, 0x63, 0x05, 0x00, 0x00, 0xcc, 0xfe, 0xc4, 0x0c, 0xdb, 0x04, 0x00, 0x00, 0xc3, 0xff, 0xfe, 0x0a, 0x46, 0xfb, 0x00, 0x00, 0x83, 0x00, 0x9a, 0x0f, 0xcd, 0x01, 0x00, 0x00, 0xc3, 0xfb, 0x2f, 0x0b, 0x8a, 0xfd, 0x00, 0x00, 0x66, 0xfd, 0xa8, 0x0a, 0xb4, 0xfa, 0x00, 0x00, 0x9b, 0xf7, 0xeb, 0x07, 0x24, 0xfb, 0x00, 0x00, 0x53, 0xf2, 0x0d, 0x03, 0xaf, 0xfa, 0x00, 0x00, 0x76, 0xf0, 0xc6, 0x03, 0x33, 0xfc, 0x00, 0x00, 0xdf, 0xf0, 0x04, 0x03, 0xe8, 0xfd, 0x00, 0x00, 0xd1, 0xf3, 0x23, 0x08, 0xfc, 0xff, 0x00, 0x00, 0x41, 0xff, 0xad, 0x05, 0xd6, 0xf1, 0x00, 0x00, 0xf7, 0xfd, 0x06, 0x03, 0xe2, 0xf1, 0x00, 0x00, 0xcc, 0xfc, 0x56, 0x06, 0x95, 0xf5, 0x00, 0x00, 0xcd, 0xfa, 0x90, 0x03, 0x4e, 0xf3, 0x00, 0x00, 0xb7, 0xfa, 0xb4, 0x07, 0x5a, 0xf5, 0x00, 0x00, 0x84, 0xf7, 0x7b, 0x03, 0x23, 0xf4, 0x00, 0x00, 0x82, 0xf6, 0x7b, 0x06, 0xb9, 0xf4, 0x00, 0x00, 0x40, 0xf5, 0x90, 0x03, 0x12, 0xf7, 0x00, 0x00, 0x9e, 0xf1, 0x80, 0x03, 0xaf, 0x01, 0x00, 0x00, 0x9e, 0xf1, 0x86, 0x06, 0xa9, 0x03, 0x00, 0x00, 0x68, 0xf2, 0x0f, 0x03, 0x3a, 0x05, 0x00, 0x00, 0xba, 0xf4, 0x8a, 0x06, 0x13, 0x06, 0x00, 0x00, 0x3d, 0xf4, 0x10, 0x04, 0x4b, 0x09, 0x00, 0x00, 0xf5, 0xf7, 0xa2, 0x06, 0xb3, 0x08, 0x00, 0x00, 0x02, 0xf7, 0x8d, 0x04, 0x0e, 0x0c, 0x00, 0x00, 0xfa, 0xf9, 0xfc, 0x07, 0x75, 0x0a, 0x00, 0x00, 0x34, 0xf9, 0x54, 0x03, 0xfc, 0x0e, 0x00, 0x00, 0x4b, 0xfb, 0xd2, 0x03, 0xa3, 0x0f, 0x00, 0x00, 0xc4, 0xfd, 0x0d, 0x03, 0x80, 0x0e, 0x00, 0x00, 0x00, 0x00, 0xe9, 0x07, 0xb0, 0x09, 0x00, 0x00, 0x2e, 0x02, 0xfa, 0x03, 0x28, 0x0d, 0x00, 0x00, 0x88, 0x04, 0xa8, 0x07, 0x0d, 0x0b, 0x00, 0x00, 0xab, 0x05, 0xf1, 0x03, 0xd2, 0x0c, 0x00, 0x00, 0xd6, 0x05, 0xe5, 0x07, 0x83, 0x0a, 0x00, 0x00, 0xbf, 0x08, 0xb3, 0x03, 0x8c, 0x0c, 0x00, 0x00, 0x1c, 0x09, 0x26, 0x06, 0x5b, 0x0b, 0x00, 0x00, 0xc2, 0x0a, 0x92, 0x03, 0xf1, 0x08, 0x00, 0x00, 0x66, 0x08, 0xe6, 0x07, 0xd6, 0x04, 0x00, 0x00, 0x7c, 0x0c, 0xfd, 0x03, 0xbc, 0x04, 0x00, 0x00, 0xd4, 0x0b, 0xb5, 0x07, 0xca, 0x01, 0x00, 0x00, 0x99, 0x0e, 0x8f, 0x03, 0x7f, 0x01, 0x00, 0x00, 0xae, 0x0e, 0x6a, 0x06, 0x00, 0x00, 0x00, 0x00, 0x87, 0x0e, 0x9a, 0x03, 0x1d, 0xfe, 0x00, 0x00, 0x8c, 0x0b, 0xe3, 0x07, 0xa5, 0xfc, 0x00, 0x00, 0xf5, 0x0d, 0x14, 0x03, 0xfc, 0xf9, 0x00, 0x00, 0xaf, 0x0d, 0x52, 0x04, 0x97, 0xf7, 0x00, 0x00, 0x31, 0x0c, 0x14, 0x03, 0xec, 0xf6, 0x00, 0x00, 0xad, 0x08, 0xe3, 0x07, 0xae, 0xf7, 0x00, 0x00, 0xe6, 0x08, 0x9a, 0x03, 0x5d, 0xf4, 0x00, 0x00, 0x57, 0x07, 0x6a, 0x06, 0x49, 0xf3, 0x00, 0x00, 0xa3, 0x05, 0x9a, 0x03, 0x7a, 0xf2, 0x00, 0x00, 0xe5, 0x02, 0xe5, 0x07, 0x54, 0xf4, 0x00, 0x00, 0x96, 0x01, 0x6b, 0x03, 0x92, 0xf1, 0x00, 0x00, 0x92, 0x05, 0x27, 0x0a, 0xb8, 0x07, 0x00, 0x00, 0xff, 0x04, 0xe3, 0x0a, 0xb6, 0x04, 0x00, 0x00, 0x94, 0x06, 0xe8, 0x09, 0xfd, 0x03, 0x00, 0x00, 0x9a, 0x06, 0x40, 0x0b, 0xda, 0x02, 0x00, 0x00, 0x73, 0x08, 0x1a, 0x0a, 0xa3, 0x01, 0x00, 0x00, 0xca, 0x02, 0x26, 0x0a, 0x32, 0xf6, 0x00, 0x00, 0xef, 0x00, 0xa7, 0x0a, 0xcc, 0xf8, 0x00, 0x00, 0xb6, 0xff, 0xf4, 0x09, 0x3a, 0xf8, 0x00, 0x00, 0xd5, 0xfe, 0xa8, 0x0a, 0xe0, 0xf9, 0x00, 0x00, 0x01, 0xfe, 0xe0, 0x09, 0x8c, 0xf8, 0x00, 0x00, 0xe0, 0x04, 0xf1, 0x09, 0x59, 0xf9, 0x00, 0x00, 0xf9, 0x02, 0x9d, 0x0a, 0x10, 0xf9, 0x00, 0x00, 0xdc, 0x07, 0x23, 0x0a, 0x5c, 0xf8, 0x00, 0x00, 0x19, 0x05, 0x98, 0x0a, 0x62, 0xfa, 0x00, 0x00, 0x41, 0x06, 0xf0, 0x09, 0x9b, 0xfb, 0x00, 0x00, 0xbf, 0x05, 0xa2, 0x0a, 0x8f, 0xfb, 0x00, 0x00, 0x02, 0x07, 0x16, 0x0a, 0xef, 0xfd, 0x00, 0x00, 0xcd, 0x05, 0x5b, 0x0a, 0x5b, 0xfd, 0x00, 0x00, 0xa7, 0x09, 0x01, 0x0a, 0x44, 0x00, 0x00, 0x00, 0x57, 0x07, 0x22, 0x0b, 0xb5, 0xff, 0x00, 0x00, 0xc2, 0x07, 0x68, 0x0b, 0x76, 0x01, 0x00, 0x00, 0xc7, 0x03, 0x1b, 0x0a, 0xc0, 0x07, 0x00, 0x00, 0xdf, 0x03, 0xbc, 0x0b, 0xf2, 0x06, 0x00, 0x00, 0x56, 0x02, 0xf6, 0x09, 0xf9, 0x07, 0x00, 0x00, 0xaa, 0x02, 0x73, 0x0b, 0x56, 0x07, 0x00, 0x00, 0xa6, 0xff, 0x8b, 0x0a, 0x94, 0x06, 0x00, 0x00, 0xed, 0x00, 0xc8, 0x0a, 0x3d, 0x06, 0x00, 0x00, 0x90, 0xfd, 0x2f, 0x0a, 0x08, 0x09, 0x00, 0x00, 0xe1, 0xfe, 0x2e, 0x0b, 0x40, 0x06, 0x00, 0x00, 0x41, 0xfb, 0x94, 0x0a, 0xb1, 0x07, 0x00, 0x00, 0x08, 0xfd, 0x36, 0x0b, 0xc6, 0x06, 0x00, 0x00, 0x39, 0xf8, 0xd1, 0x0a, 0xe6, 0x06, 0x00, 0x00, 0x3f, 0xfb, 0x2b, 0x0b, 0x09, 0x06, 0x00, 0x00, 0x4a, 0xf7, 0x4f, 0x0b, 0x29, 0x04, 0x00, 0x00, 0x6d, 0xfa, 0x50, 0x0b, 0x14, 0x04, 0x00, 0x00, 0xeb, 0xf7, 0x9a, 0x0b, 0xf6, 0x01, 0x00, 0x00, 0x9d, 0xf9, 0x27, 0x0c, 0x36, 0x02, 0x00, 0x00, 0x59, 0xf7, 0x96, 0x0a, 0xee, 0xfe, 0x00, 0x00, 0xce, 0xf8, 0xd4, 0x0b, 0x86, 0x00, 0x00, 0x00, 0x20, 0xf8, 0x87, 0x0a, 0xf3, 0xfd, 0x00, 0x00, 0xf3, 0xf8, 0x80, 0x0b, 0xe3, 0xfe, 0x00, 0x00, 0xe7, 0xfa, 0xe7, 0x0a, 0x31, 0xf9, 0x00, 0x00, 0x56, 0xfb, 0x6b, 0x0a, 0xc5, 0xfb, 0x00, 0x00, 0x75, 0xf9, 0x03, 0x0a, 0x09, 0xfc, 0x00, 0x00, 0x18, 0xfa, 0x17, 0x0b, 0x86, 0xfd, 0x00, 0x00, 0xc2, 0xfb, 0xbe, 0x09, 0xc5, 0xf7, 0x00, 0x00, 0x9e, 0xfc, 0x4d, 0x0a, 0x00, 0xfa, 0x00, 0x00, 0x90, 0xfd, 0x8d, 0x0a, 0x9e, 0xf9, 0x00, 0x00, 0x84, 0xfe, 0x67, 0x08, 0x03, 0x0a, 0x00, 0x00, 0x0b, 0x00, 0xb0, 0x09, 0x2a, 0x09, 0x00, 0x00, 0x50, 0x01, 0xff, 0x08, 0xba, 0x08, 0x00, 0x00, 0x29, 0x01, 0x75, 0x08, 0x25, 0xf5, 0x00, 0x00, 0x2a, 0xfe, 0x04, 0x08, 0x0c, 0xf6, 0x00, 0x00, 0x7a, 0x04, 0xa2, 0x08, 0xca, 0xf5, 0x00, 0x00, 0x30, 0x07, 0x9e, 0x08, 0x72, 0xf7, 0x00, 0x00, 0x57, 0x09, 0x26, 0x08, 0x16, 0xf9, 0x00, 0x00, 0xd8, 0x09, 0x28, 0x08, 0x72, 0xfb, 0x00, 0x00, 0xb9, 0x0a, 0xa3, 0x08, 0xf2, 0xfe, 0x00, 0x00, 0x25, 0x0b, 0x93, 0x08, 0x23, 0x01, 0x00, 0x00, 0xce, 0x08, 0xe1, 0x08, 0x0b, 0x03, 0x00, 0x00, 0x6a, 0x07, 0x8f, 0x08, 0xfa, 0x06, 0x00, 0x00, 0x13, 0x06, 0x9c, 0x08, 0x54, 0x09, 0x00, 0x00, 0x1f, 0x04, 0xea, 0x08, 0x51, 0x09, 0x00, 0x00, 0x9c, 0xfb, 0x25, 0x08, 0xde, 0x0a, 0x00, 0x00, 0x6a, 0xf9, 0x59, 0x09, 0xa2, 0x08, 0x00, 0x00, 0xf0, 0xf6, 0x55, 0x09, 0xbb, 0x06, 0x00, 0x00, 0x51, 0xf5, 0x02, 0x09, 0xd1, 0x03, 0x00, 0x00, 0xcf, 0xf5, 0x82, 0x08, 0xe7, 0x00, 0x00, 0x00, 0xc3, 0xf4, 0x43, 0x08, 0x28, 0xfe, 0x00, 0x00, 0xd2, 0xf8, 0x91, 0x08, 0x5e, 0xf9, 0x00, 0x00, 0x31, 0xf6, 0x28, 0x08, 0x3d, 0xfc, 0x00, 0x00, 0x7b, 0xfa, 0x73, 0x08, 0xdb, 0xf6, 0x00, 0x00, 0x5a, 0xfc, 0x6e, 0x08, 0xd0, 0xf6, 0x00, 0x00, 0x3c, 0x00, 0x5f, 0x10, 0xf1, 0xff, 0x00, 0x00, 0x82, 0x01, 0x2c, 0x01, 0x00, 0x00, 0x08, 0x00, 0x10, 0x00, 0x80, 0x00, 0xc3, 0xad, 0x00, 0x00, 0xc0, 0xb1, 0xbe, 0x7d, 0x18, 0x00, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0xca, 0xa6, 0x00, 0x00, 0xc3, 0xad, 0xbe, 0x7d, 0x20, 0x00, 0x18, 0x00, 0x10, 0x00, 0x80, 0x00, 0xd1, 0x9e, 0x00, 0x00, 0xca, 0xa6, 0xbe, 0x7d, 0x28, 0x00, 0x20, 0x00, 0x10, 0x00, 0x80, 0x00, 0xd6, 0x99, 0x00, 0x00, 0xd1, 0x9e, 0xbe, 0x7d, 0x30, 0x00, 0x28, 0x00, 0x10, 0x00, 0x80, 0x00, 0xd9, 0x8e, 0x00, 0x00, 0xd6, 0x99, 0xbe, 0x7d, 0x38, 0x00, 0x30, 0x00, 0x10, 0x00, 0x80, 0x00, 0xd7, 0x83, 0x00, 0x00, 0xd9, 0x8e, 0xbe, 0x7d, 0x40, 0x00, 0x38, 0x00, 0x10, 0x00, 0x80, 0x00, 0xd4, 0x74, 0x00, 0x00, 0xd7, 0x83, 0xbe, 0x7d, 0x48, 0x00, 0x40, 0x00, 0x10, 0x00, 0x80, 0x00, 0xcf, 0x69, 0x00, 0x00, 0xd4, 0x74, 0xbe, 0x7d, 0x50, 0x00, 0x48, 0x00, 0x10, 0x00, 0x80, 0x00, 0xcf, 0x54, 0x00, 0x00, 0xcf, 0x69, 0xbe, 0x7d, 0x58, 0x00, 0x50, 0x00, 0x10, 0x00, 0x80, 0x00, 0xcd, 0x53, 0x00, 0x00, 0xcf, 0x54, 0xbe, 0x7d, 0x60, 0x00, 0x58, 0x00, 0x10, 0x00, 0x80, 0x00, 0xc5, 0x54, 0x00, 0x00, 0xcd, 0x53, 0xbe, 0x7d, 0x68, 0x00, 0x60, 0x00, 0x10, 0x00, 0x80, 0x00, 0xbd, 0x4d, 0x00, 0x00, 0xc5, 0x54, 0xbe, 0x7d, 0x70, 0x00, 0x68, 0x00, 0x10, 0x00, 0x80, 0x00, 0xb7, 0x4b, 0x00, 0x00, 0xbd, 0x4d, 0xbe, 0x7d, 0x78, 0x00, 0x70, 0x00, 0x10, 0x00, 0x80, 0x00, 0xb2, 0x4f, 0x00, 0x00, 0xb7, 0x4b, 0xbe, 0x7d, 0x80, 0x00, 0x78, 0x00, 0x10, 0x00, 0x80, 0x00, 0xac, 0x58, 0x00, 0x00, 0xb2, 0x4f, 0xbe, 0x7d, 0x88, 0x00, 0x80, 0x00, 0x10, 0x00, 0x80, 0x00, 0xa6, 0x61, 0x00, 0x00, 0xac, 0x58, 0xbe, 0x7d, 0x90, 0x00, 0x88, 0x00, 0x10, 0x00, 0x80, 0x00, 0xa4, 0x6d, 0x00, 0x00, 0xa6, 0x61, 0xbe, 0x7d, 0x98, 0x00, 0x90, 0x00, 0x10, 0x00, 0x80, 0x00, 0xa6, 0x7b, 0x00, 0x00, 0xa4, 0x6d, 0xbe, 0x7d, 0xa0, 0x00, 0x98, 0x00, 0x10, 0x00, 0x80, 0x00, 0xa5, 0x8c, 0x00, 0x00, 0xa6, 0x7b, 0xbe, 0x7d, 0xa8, 0x00, 0xa0, 0x00, 0x10, 0x00, 0x80, 0x00, 0xa9, 0x95, 0x00, 0x00, 0xa5, 0x8c, 0xbe, 0x7d, 0xb0, 0x00, 0xa8, 0x00, 0x10, 0x00, 0x80, 0x00, 0xad, 0xa2, 0x00, 0x00, 0xa9, 0x95, 0xbe, 0x7d, 0xb8, 0x00, 0xb0, 0x00, 0x10, 0x00, 0x80, 0x00, 0xb1, 0xa8, 0x00, 0x00, 0xad, 0xa2, 0xbe, 0x7d, 0xc0, 0x00, 0xb8, 0x00, 0x10, 0x00, 0x80, 0x00, 0xba, 0xa6, 0x00, 0x00, 0xb1, 0xa8, 0xbe, 0x7d, 0x48, 0x02, 0x50, 0x02, 0x58, 0x02, 0x2a, 0x00, 0xb0, 0x5a, 0x00, 0x00, 0xb7, 0x74, 0xb7, 0x6b, 0x60, 0x02, 0x50, 0x02, 0x68, 0x02, 0x3a, 0x00, 0xac, 0x74, 0x00, 0x00, 0xb7, 0x74, 0xad, 0x69, 0x60, 0x02, 0x70, 0x02, 0x78, 0x02, 0x44, 0x00, 0xac, 0x74, 0x00, 0x00, 0xb7, 0x7d, 0xb6, 0x79, 0x80, 0x02, 0x88, 0x02, 0x70, 0x02, 0x2c, 0x00, 0xa4, 0x81, 0x00, 0x00, 0xb6, 0x83, 0xb7, 0x7d, 0x90, 0x02, 0x98, 0x02, 0x88, 0x02, 0x2a, 0x00, 0xac, 0x88, 0x00, 0x00, 0xb8, 0x88, 0xb6, 0x83, 0x98, 0x02, 0xa0, 0x02, 0xa8, 0x02, 0x41, 0x00, 0xb8, 0x88, 0x00, 0x00, 0xb0, 0xa2, 0xbb, 0x88, 0xa0, 0x02, 0xb0, 0x02, 0xa8, 0x02, 0x2a, 0x00, 0xb0, 0xa2, 0x00, 0x00, 0xb9, 0x90, 0xbb, 0x88, 0xb0, 0x02, 0xb8, 0x02, 0xc0, 0x02, 0x40, 0x00, 0xb9, 0x90, 0x00, 0x00, 0xbb, 0xa0, 0xbd, 0x90, 0xb8, 0x02, 0xc8, 0x02, 0xc0, 0x02, 0x3e, 0x00, 0xbb, 0xa0, 0x00, 0x00, 0xbf, 0x8a, 0xbd, 0x90, 0xc8, 0x02, 0xd0, 0x02, 0xd8, 0x02, 0x3a, 0x00, 0xbf, 0x8a, 0x00, 0x00, 0xc4, 0xb1, 0xc2, 0x93, 0xd8, 0x02, 0xe0, 0x02, 0xe8, 0x02, 0x36, 0x00, 0xc2, 0x93, 0x00, 0x00, 0xc9, 0xa4, 0xc2, 0x8a, 0xf0, 0x02, 0xe0, 0x02, 0xf8, 0x02, 0x44, 0x00, 0xc8, 0x90, 0x00, 0x00, 0xc9, 0xa4, 0xd1, 0xa0, 0x00, 0x03, 0x08, 0x03, 0x10, 0x03, 0x31, 0x00, 0xcd, 0x83, 0x00, 0x00, 0xd3, 0x95, 0xda, 0x8c, 0x18, 0x03, 0xf8, 0x02, 0x08, 0x03, 0x3e, 0x00, 0xc5, 0x83, 0x00, 0x00, 0xd1, 0xa0, 0xd3, 0x95, 0x20, 0x03, 0x10, 0x03, 0x28, 0x03, 0x2e, 0x00, 0xca, 0x7b, 0x00, 0x00, 0xda, 0x8c, 0xd6, 0x78, 0x30, 0x03, 0x28, 0x03, 0x38, 0x03, 0x30, 0x00, 0xc9, 0x76, 0x00, 0x00, 0xd6, 0x78, 0xd6, 0x6d, 0x30, 0x03, 0x40, 0x03, 0x48, 0x03, 0x2e, 0x00, 0xc9, 0x76, 0x00, 0x00, 0xd0, 0x63, 0xc5, 0x74, 0x40, 0x03, 0x50, 0x03, 0x48, 0x03, 0x33, 0x00, 0xd0, 0x63, 0x00, 0x00, 0xc8, 0x65, 0xc5, 0x74, 0x58, 0x03, 0x50, 0x03, 0x60, 0x03, 0x30, 0x00, 0xc8, 0x48, 0x00, 0x00, 0xc8, 0x65, 0xcd, 0x58, 0x68, 0x03, 0x70, 0x03, 0x58, 0x03, 0x2a, 0x00, 0xc6, 0x49, 0x00, 0x00, 0xc3, 0x6d, 0xc8, 0x48, 0x78, 0x03, 0x68, 0x03, 0x80, 0x03, 0x2c, 0x00, 0xc0, 0x63, 0x00, 0x00, 0xc6, 0x49, 0xbe, 0x53, 0x80, 0x03, 0x88, 0x03, 0x78, 0x03, 0x2a, 0x00, 0xbe, 0x53, 0x00, 0x00, 0xbe, 0x6b, 0xc0, 0x63, 0x90, 0x03, 0x98, 0x03, 0x48, 0x02, 0x2e, 0x00, 0xbb, 0x6e, 0x00, 0x00, 0xb3, 0x51, 0xb0, 0x5a, 0x90, 0x03, 0x88, 0x03, 0xa0, 0x03, 0x2a, 0x00, 0xbb, 0x6e, 0x00, 0x00, 0xbe, 0x6b, 0xba, 0x56, 0xd0, 0x02, 0xc8, 0x02, 0xa8, 0x03, 0x3a, 0x00, 0xc4, 0xb1, 0x00, 0x00, 0xbf, 0x8a, 0xbd, 0xa2, 0xe0, 0x02, 0xd8, 0x02, 0xd0, 0x02, 0x38, 0x00, 0xc9, 0xa4, 0x00, 0x00, 0xc2, 0x93, 0xc4, 0xb1, 0xf0, 0x02, 0xe8, 0x02, 0xe0, 0x02, 0x42, 0x00, 0xc8, 0x90, 0x00, 0x00, 0xc2, 0x8a, 0xc9, 0xa4, 0x18, 0x03, 0xf0, 0x02, 0xf8, 0x02, 0x33, 0x00, 0xc5, 0x83, 0x00, 0x00, 0xc8, 0x90, 0xd1, 0xa0, 0x00, 0x03, 0x18, 0x03, 0x08, 0x03, 0x38, 0x00, 0xcd, 0x83, 0x00, 0x00, 0xc5, 0x83, 0xd3, 0x95, 0x20, 0x03, 0x00, 0x03, 0x10, 0x03, 0x30, 0x00, 0xca, 0x7b, 0x00, 0x00, 0xcd, 0x83, 0xda, 0x8c, 0x30, 0x03, 0x20, 0x03, 0x28, 0x03, 0x40, 0x00, 0xc9, 0x76, 0x00, 0x00, 0xca, 0x7b, 0xd6, 0x78, 0x40, 0x03, 0x30, 0x03, 0x38, 0x03, 0x30, 0x00, 0xd0, 0x63, 0x00, 0x00, 0xc9, 0x76, 0xd6, 0x6d, 0x50, 0x03, 0x40, 0x03, 0x60, 0x03, 0x30, 0x00, 0xc8, 0x65, 0x00, 0x00, 0xd0, 0x63, 0xcd, 0x58, 0x70, 0x03, 0x50, 0x03, 0x58, 0x03, 0x2d, 0x00, 0xc3, 0x6d, 0x00, 0x00, 0xc8, 0x65, 0xc8, 0x48, 0xb0, 0x03, 0x70, 0x03, 0x68, 0x03, 0x31, 0x00, 0xc1, 0x6e, 0x00, 0x00, 0xc3, 0x6d, 0xc6, 0x49, 0x98, 0x03, 0x90, 0x03, 0xa0, 0x03, 0x2a, 0x00, 0xb3, 0x51, 0x00, 0x00, 0xbb, 0x6e, 0xba, 0x56, 0x58, 0x02, 0x90, 0x03, 0x48, 0x02, 0x31, 0x00, 0xb7, 0x6b, 0x00, 0x00, 0xbb, 0x6e, 0xb0, 0x5a, 0x50, 0x02, 0x48, 0x02, 0x68, 0x02, 0x2d, 0x00, 0xb7, 0x74, 0x00, 0x00, 0xb0, 0x5a, 0xad, 0x69, 0x78, 0x02, 0x50, 0x02, 0x60, 0x02, 0x2a, 0x00, 0xb6, 0x79, 0x00, 0x00, 0xb7, 0x74, 0xac, 0x74, 0x70, 0x02, 0x60, 0x02, 0x80, 0x02, 0x2c, 0x00, 0xb7, 0x7d, 0x00, 0x00, 0xac, 0x74, 0xa4, 0x81, 0xa0, 0x02, 0x98, 0x02, 0xb8, 0x03, 0x2a, 0x00, 0xb0, 0xa2, 0x00, 0x00, 0xb8, 0x88, 0xaf, 0x90, 0xb0, 0x02, 0xa0, 0x02, 0xc0, 0x03, 0x46, 0x00, 0xb9, 0x90, 0x00, 0x00, 0xb0, 0xa2, 0xb5, 0xa6, 0xb8, 0x02, 0xb0, 0x02, 0xc0, 0x03, 0x44, 0x00, 0xbb, 0xa0, 0x00, 0x00, 0xb9, 0x90, 0xb5, 0xa6, 0xc8, 0x02, 0xb8, 0x02, 0xa8, 0x03, 0x3e, 0x00, 0xbf, 0x8a, 0x00, 0x00, 0xbb, 0xa0, 0xbd, 0xa2, 0x88, 0x02, 0x80, 0x02, 0x90, 0x02, 0x42, 0x00, 0xb6, 0x83, 0x00, 0x00, 0xa4, 0x81, 0xac, 0x88, 0x98, 0x02, 0x90, 0x02, 0xb8, 0x03, 0x45, 0x00, 0xb8, 0x88, 0x00, 0x00, 0xac, 0x88, 0xaf, 0x90, 0x78, 0x03, 0xb0, 0x03, 0x68, 0x03, 0x30, 0x00, 0xc0, 0x63, 0x00, 0x00, 0xc1, 0x6e, 0xc6, 0x49, 0x88, 0x03, 0x80, 0x03, 0xa0, 0x03, 0x2e, 0x00, 0xbe, 0x6b, 0x00, 0x00, 0xbe, 0x53, 0xba, 0x56, 0x20, 0x02, 0xc8, 0x03, 0xd0, 0x03, 0x3e, 0x00, 0x8b, 0xbc, 0x00, 0x00, 0xa0, 0xa2, 0x8d, 0xa6, 0x18, 0x02, 0x20, 0x02, 0xd0, 0x03, 0x3b, 0x00, 0x80, 0xa0, 0x00, 0x00, 0x8b, 0xbc, 0x8d, 0xa6, 0xd8, 0x03, 0x18, 0x02, 0xd0, 0x03, 0x43, 0x00, 0x86, 0x9b, 0x00, 0x00, 0x80, 0xa0, 0x8d, 0xa6, 0x18, 0x02, 0xd8, 0x03, 0xe0, 0x03, 0x2a, 0x00, 0x80, 0xa0, 0x00, 0x00, 0x86, 0x9b, 0x87, 0x8e, 0x10, 0x02, 0x18, 0x02, 0xe0, 0x03, 0x2a, 0x00, 0x84, 0x7d, 0x00, 0x00, 0x80, 0xa0, 0x87, 0x8e, 0xe8, 0x03, 0x10, 0x02, 0xe0, 0x03, 0x2a, 0x00, 0x92, 0x7d, 0x00, 0x00, 0x84, 0x7d, 0x87, 0x8e, 0x40, 0x02, 0xf0, 0x03, 0xf8, 0x03, 0x44, 0x00, 0xbf, 0xfd, 0x00, 0x00, 0xbc, 0xee, 0xb7, 0xec, 0x38, 0x02, 0x40, 0x02, 0xf8, 0x03, 0x43, 0x00, 0xb2, 0xe8, 0x00, 0x00, 0xbf, 0xfd, 0xb7, 0xec, 0x00, 0x04, 0x38, 0x02, 0xf8, 0x03, 0x3a, 0x00, 0xb3, 0xce, 0x00, 0x00, 0xb2, 0xe8, 0xb7, 0xec, 0x38, 0x02, 0x00, 0x04, 0x08, 0x04, 0x44, 0x00, 0xb2, 0xe8, 0x00, 0x00, 0xb3, 0xce, 0xac, 0xe1, 0x30, 0x02, 0x38, 0x02, 0x08, 0x04, 0x48, 0x00, 0xa1, 0xec, 0x00, 0x00, 0xb2, 0xe8, 0xac, 0xe1, 0x10, 0x04, 0x30, 0x02, 0x08, 0x04, 0x46, 0x00, 0xac, 0xd0, 0x00, 0x00, 0xa1, 0xec, 0xac, 0xe1, 0x30, 0x02, 0x10, 0x04, 0x18, 0x04, 0x44, 0x00, 0xa1, 0xec, 0x00, 0x00, 0xac, 0xd0, 0xa0, 0xdb, 0x28, 0x02, 0x30, 0x02, 0x18, 0x04, 0x41, 0x00, 0x95, 0xd8, 0x00, 0x00, 0xa1, 0xec, 0xa0, 0xdb, 0x20, 0x04, 0x28, 0x02, 0x18, 0x04, 0x49, 0x00, 0x9c, 0xd6, 0x00, 0x00, 0x95, 0xd8, 0xa0, 0xdb, 0x28, 0x02, 0x20, 0x04, 0x28, 0x04, 0x2e, 0x00, 0x95, 0xd8, 0x00, 0x00, 0x9c, 0xd6, 0x97, 0xc3, 0x20, 0x02, 0x28, 0x02, 0x28, 0x04, 0x3b, 0x00, 0x8b, 0xbc, 0x00, 0x00, 0x95, 0xd8, 0x97, 0xc3, 0xc8, 0x03, 0x20, 0x02, 0x28, 0x04, 0x32, 0x00, 0xa0, 0xa2, 0x00, 0x00, 0x8b, 0xbc, 0x97, 0xc3, 0x10, 0x02, 0xe8, 0x03, 0x30, 0x04, 0x2a, 0x00, 0x84, 0x7d, 0x00, 0x00, 0x92, 0x7d, 0x8b, 0x6e, 0x08, 0x02, 0x10, 0x02, 0x30, 0x04, 0x31, 0x00, 0x80, 0x5a, 0x00, 0x00, 0x84, 0x7d, 0x8b, 0x6e, 0x38, 0x04, 0x08, 0x02, 0x30, 0x04, 0x3a, 0x00, 0x8b, 0x60, 0x00, 0x00, 0x80, 0x5a, 0x8b, 0x6e, 0x08, 0x02, 0x38, 0x04, 0x40, 0x04, 0x2c, 0x00, 0x80, 0x5a, 0x00, 0x00, 0x8b, 0x60, 0x8d, 0x53, 0x00, 0x02, 0x08, 0x02, 0x40, 0x04, 0x2a, 0x00, 0x86, 0x35, 0x00, 0x00, 0x80, 0x5a, 0x8d, 0x53, 0x48, 0x04, 0x00, 0x02, 0x40, 0x04, 0x2a, 0x00, 0x96, 0x4b, 0x00, 0x00, 0x86, 0x35, 0x8d, 0x53, 0x00, 0x02, 0x48, 0x04, 0x50, 0x04, 0x2a, 0x00, 0x86, 0x35, 0x00, 0x00, 0x96, 0x4b, 0x94, 0x31, 0xf8, 0x01, 0x00, 0x02, 0x50, 0x04, 0x2b, 0x00, 0x95, 0x21, 0x00, 0x00, 0x86, 0x35, 0x94, 0x31, 0x58, 0x04, 0xf8, 0x01, 0x50, 0x04, 0x2e, 0x00, 0xa1, 0x37, 0x00, 0x00, 0x95, 0x21, 0x94, 0x31, 0xf8, 0x01, 0x58, 0x04, 0x60, 0x04, 0x2a, 0x00, 0x95, 0x21, 0x00, 0x00, 0xa1, 0x37, 0x9e, 0x1d, 0xf0, 0x01, 0xf8, 0x01, 0x60, 0x04, 0x2a, 0x00, 0xa1, 0x0c, 0x00, 0x00, 0x95, 0x21, 0x9e, 0x1d, 0x68, 0x04, 0xf0, 0x01, 0x60, 0x04, 0x30, 0x00, 0xa9, 0x28, 0x00, 0x00, 0xa1, 0x0c, 0x9e, 0x1d, 0xf0, 0x01, 0x68, 0x04, 0x70, 0x04, 0x2a, 0x00, 0xa1, 0x0c, 0x00, 0x00, 0xa9, 0x28, 0xa6, 0x05, 0xe8, 0x01, 0xf0, 0x01, 0x70, 0x04, 0x36, 0x00, 0xaf, 0x00, 0x00, 0x00, 0xa1, 0x0c, 0xa6, 0x05, 0x78, 0x04, 0xe8, 0x01, 0x70, 0x04, 0x30, 0x00, 0xad, 0x00, 0x00, 0x00, 0xaf, 0x00, 0xa6, 0x05, 0xe8, 0x01, 0x78, 0x04, 0x80, 0x04, 0x32, 0x00, 0xaf, 0x00, 0x00, 0x00, 0xad, 0x00, 0xb7, 0x09, 0xe0, 0x01, 0xe8, 0x01, 0x80, 0x04, 0x31, 0x00, 0xbe, 0x05, 0x00, 0x00, 0xaf, 0x00, 0xb7, 0x09, 0x88, 0x04, 0xe0, 0x01, 0x80, 0x04, 0x32, 0x00, 0xbf, 0x30, 0x00, 0x00, 0xbe, 0x05, 0xb7, 0x09, 0xe0, 0x01, 0x88, 0x04, 0x90, 0x04, 0x2c, 0x00, 0xbe, 0x05, 0x00, 0x00, 0xbf, 0x30, 0xc7, 0x14, 0xd8, 0x01, 0xe0, 0x01, 0x90, 0x04, 0x32, 0x00, 0xce, 0x12, 0x00, 0x00, 0xbe, 0x05, 0xc7, 0x14, 0x98, 0x04, 0xd8, 0x01, 0x90, 0x04, 0x32, 0x00, 0xcf, 0x24, 0x00, 0x00, 0xce, 0x12, 0xc7, 0x14, 0xd8, 0x01, 0x98, 0x04, 0xa0, 0x04, 0x2c, 0x00, 0xce, 0x12, 0x00, 0x00, 0xcf, 0x24, 0xd3, 0x16, 0xd0, 0x01, 0xd8, 0x01, 0xa0, 0x04, 0x30, 0x00, 0xdc, 0x14, 0x00, 0x00, 0xce, 0x12, 0xd3, 0x16, 0xa8, 0x04, 0xd0, 0x01, 0xa0, 0x04, 0x32, 0x00, 0xd4, 0x28, 0x00, 0x00, 0xdc, 0x14, 0xd3, 0x16, 0xd0, 0x01, 0xa8, 0x04, 0xb0, 0x04, 0x2c, 0x00, 0xdc, 0x14, 0x00, 0x00, 0xd4, 0x28, 0xde, 0x18, 0xc8, 0x01, 0xd0, 0x01, 0xb0, 0x04, 0x32, 0x00, 0xe9, 0x21, 0x00, 0x00, 0xdc, 0x14, 0xde, 0x18, 0xb8, 0x04, 0xc8, 0x01, 0xb0, 0x04, 0x33, 0x00, 0xe0, 0x23, 0x00, 0x00, 0xe9, 0x21, 0xde, 0x18, 0xc8, 0x01, 0xb8, 0x04, 0xc0, 0x04, 0x32, 0x00, 0xe9, 0x21, 0x00, 0x00, 0xe0, 0x23, 0xe6, 0x35, 0xc0, 0x01, 0xc8, 0x01, 0xc0, 0x04, 0x32, 0x00, 0xea, 0x49, 0x00, 0x00, 0xe9, 0x21, 0xe6, 0x35, 0xc8, 0x04, 0xc0, 0x01, 0xc0, 0x04, 0x34, 0x00, 0xde, 0x56, 0x00, 0x00, 0xea, 0x49, 0xe6, 0x35, 0xc0, 0x01, 0xc8, 0x04, 0xd0, 0x04, 0x32, 0x00, 0xea, 0x49, 0x00, 0x00, 0xde, 0x56, 0xec, 0x56, 0xb8, 0x01, 0xc0, 0x01, 0xd0, 0x04, 0x33, 0x00, 0xf3, 0x5a, 0x00, 0x00, 0xea, 0x49, 0xec, 0x56, 0xd8, 0x04, 0xb8, 0x01, 0xd0, 0x04, 0x32, 0x00, 0xe9, 0x6e, 0x00, 0x00, 0xf3, 0x5a, 0xec, 0x56, 0xb8, 0x01, 0xd8, 0x04, 0xe0, 0x04, 0x32, 0x00, 0xf3, 0x5a, 0x00, 0x00, 0xe9, 0x6e, 0xf4, 0x70, 0xb0, 0x01, 0xb8, 0x01, 0xe0, 0x04, 0x33, 0x00, 0xfa, 0x7b, 0x00, 0x00, 0xf3, 0x5a, 0xf4, 0x70, 0xe8, 0x04, 0xb0, 0x01, 0xe0, 0x04, 0x33, 0x00, 0xf4, 0x7b, 0x00, 0x00, 0xfa, 0x7b, 0xf4, 0x70, 0xb0, 0x01, 0xe8, 0x04, 0xf0, 0x04, 0x3a, 0x00, 0xfa, 0x7b, 0x00, 0x00, 0xf4, 0x7b, 0xf4, 0x8c, 0xa8, 0x01, 0xb0, 0x01, 0xf0, 0x04, 0x32, 0x00, 0xff, 0x9e, 0x00, 0x00, 0xfa, 0x7b, 0xf4, 0x8c, 0xf8, 0x04, 0xa8, 0x01, 0xf0, 0x04, 0x30, 0x00, 0xe9, 0x97, 0x00, 0x00, 0xff, 0x9e, 0xf4, 0x8c, 0xa8, 0x01, 0xf8, 0x04, 0x00, 0x05, 0x3a, 0x00, 0xff, 0x9e, 0x00, 0x00, 0xe9, 0x97, 0xf2, 0xab, 0xa0, 0x01, 0xa8, 0x01, 0x00, 0x05, 0x38, 0x00, 0xf6, 0xc3, 0x00, 0x00, 0xff, 0x9e, 0xf2, 0xab, 0x08, 0x05, 0xa0, 0x01, 0x00, 0x05, 0x31, 0x00, 0xf0, 0xc0, 0x00, 0x00, 0xf6, 0xc3, 0xf2, 0xab, 0xa0, 0x01, 0x08, 0x05, 0x10, 0x05, 0x48, 0x00, 0xf6, 0xc3, 0x00, 0x00, 0xf0, 0xc0, 0xeb, 0xc5, 0x98, 0x01, 0xa0, 0x01, 0x10, 0x05, 0x42, 0x00, 0xe9, 0xd8, 0x00, 0x00, 0xf6, 0xc3, 0xeb, 0xc5, 0x18, 0x05, 0x98, 0x01, 0x10, 0x05, 0x42, 0x00, 0xde, 0xbe, 0x00, 0x00, 0xe9, 0xd8, 0xeb, 0xc5, 0x98, 0x01, 0x18, 0x05, 0x20, 0x05, 0x3e, 0x00, 0xe9, 0xd8, 0x00, 0x00, 0xde, 0xbe, 0xdf, 0xda, 0x88, 0x01, 0x98, 0x01, 0x20, 0x05, 0x48, 0x00, 0xda, 0xe3, 0x00, 0x00, 0xe9, 0xd8, 0xdf, 0xda, 0x28, 0x05, 0x88, 0x01, 0x20, 0x05, 0x43, 0x00, 0xd9, 0xe1, 0x00, 0x00, 0xda, 0xe3, 0xdf, 0xda, 0x88, 0x01, 0x28, 0x05, 0x30, 0x05, 0x48, 0x00, 0xda, 0xe3, 0x00, 0x00, 0xd9, 0xe1, 0xd3, 0xe8, 0x90, 0x01, 0x88, 0x01, 0x30, 0x05, 0x46, 0x00, 0xce, 0xf2, 0x00, 0x00, 0xda, 0xe3, 0xd3, 0xe8, 0x38, 0x05, 0x90, 0x01, 0x30, 0x05, 0x48, 0x00, 0xc9, 0xda, 0x00, 0x00, 0xce, 0xf2, 0xd3, 0xe8, 0x90, 0x01, 0x38, 0x05, 0x40, 0x05, 0x44, 0x00, 0xce, 0xf2, 0x00, 0x00, 0xc9, 0xda, 0xc4, 0xf0, 0x40, 0x02, 0x90, 0x01, 0x40, 0x05, 0x47, 0x00, 0xbf, 0xfd, 0x00, 0x00, 0xce, 0xf2, 0xc4, 0xf0, 0xf0, 0x03, 0x40, 0x02, 0x40, 0x05, 0x47, 0x00, 0xbc, 0xee, 0x00, 0x00, 0xbf, 0xfd, 0xc4, 0xf0, 0x48, 0x05, 0x60, 0x03, 0x50, 0x05, 0x30, 0x00, 0xd3, 0x3e, 0x00, 0x00, 0xcd, 0x58, 0xd1, 0x56, 0x58, 0x05, 0x48, 0x05, 0x50, 0x05, 0x2e, 0x00, 0xd7, 0x5c, 0x00, 0x00, 0xd3, 0x3e, 0xd1, 0x56, 0x40, 0x03, 0x58, 0x05, 0x50, 0x05, 0x31, 0x00, 0xd0, 0x63, 0x00, 0x00, 0xd7, 0x5c, 0xd1, 0x56, 0x58, 0x05, 0x40, 0x03, 0x60, 0x05, 0x31, 0x00, 0xd7, 0x5c, 0x00, 0x00, 0xd0, 0x63, 0xd7, 0x65, 0x68, 0x05, 0x58, 0x05, 0x60, 0x05, 0x32, 0x00, 0xde, 0x6e, 0x00, 0x00, 0xd7, 0x5c, 0xd7, 0x65, 0x38, 0x03, 0x68, 0x05, 0x60, 0x05, 0x32, 0x00, 0xd6, 0x6d, 0x00, 0x00, 0xde, 0x6e, 0xd7, 0x65, 0x70, 0x05, 0xd0, 0x02, 0x78, 0x05, 0x3b, 0x00, 0xc9, 0xcb, 0x00, 0x00, 0xc4, 0xb1, 0xc2, 0xb6, 0x80, 0x05, 0x70, 0x05, 0x78, 0x05, 0x36, 0x00, 0xbd, 0xba, 0x00, 0x00, 0xc9, 0xcb, 0xc2, 0xb6, 0xa8, 0x03, 0x80, 0x05, 0x78, 0x05, 0x34, 0x00, 0xbd, 0xa2, 0x00, 0x00, 0xbd, 0xba, 0xc2, 0xb6, 0x80, 0x05, 0xa8, 0x03, 0x88, 0x05, 0x34, 0x00, 0xbd, 0xba, 0x00, 0x00, 0xbd, 0xa2, 0xba, 0xad, 0x90, 0x05, 0x80, 0x05, 0x88, 0x05, 0x38, 0x00, 0xb7, 0xb8, 0x00, 0x00, 0xbd, 0xba, 0xba, 0xad, 0xb8, 0x02, 0x90, 0x05, 0x88, 0x05, 0x3f, 0x00, 0xbb, 0xa0, 0x00, 0x00, 0xb7, 0xb8, 0xba, 0xad, 0x98, 0x05, 0xe0, 0x02, 0xa0, 0x05, 0x30, 0x00, 0xd1, 0xb1, 0x00, 0x00, 0xc9, 0xa4, 0xc9, 0xb3, 0x70, 0x05, 0x98, 0x05, 0xa0, 0x05, 0x31, 0x00, 0xc9, 0xcb, 0x00, 0x00, 0xd1, 0xb1, 0xc9, 0xb3, 0xd0, 0x02, 0x70, 0x05, 0xa0, 0x05, 0x32, 0x00, 0xc4, 0xb1, 0x00, 0x00, 0xc9, 0xcb, 0xc9, 0xb3, 0xa8, 0x05, 0xf8, 0x02, 0xb0, 0x05, 0x40, 0x00, 0xdb, 0xb8, 0x00, 0x00, 0xd1, 0xa0, 0xd1, 0xa9, 0x98, 0x05, 0xa8, 0x05, 0xb0, 0x05, 0x3a, 0x00, 0xd1, 0xb1, 0x00, 0x00, 0xdb, 0xb8, 0xd1, 0xa9, 0xe0, 0x02, 0x98, 0x05, 0xb0, 0x05, 0x3b, 0x00, 0xc9, 0xa4, 0x00, 0x00, 0xd1, 0xb1, 0xd1, 0xa9, 0xb8, 0x05, 0x08, 0x03, 0xc0, 0x05, 0x40, 0x00, 0xd5, 0x9e, 0x00, 0x00, 0xd3, 0x95, 0xd3, 0xa0, 0xa8, 0x05, 0xb8, 0x05, 0xc0, 0x05, 0x32, 0x00, 0xdb, 0xb8, 0x00, 0x00, 0xd5, 0x9e, 0xd3, 0xa0, 0xf8, 0x02, 0xa8, 0x05, 0xc0, 0x05, 0x32, 0x00, 0xd1, 0xa0, 0x00, 0x00, 0xdb, 0xb8, 0xd3, 0xa0, 0xc8, 0x05, 0x10, 0x03, 0xd0, 0x05, 0x4d, 0x00, 0xd9, 0x8c, 0x00, 0x00, 0xda, 0x8c, 0xd4, 0x91, 0xb8, 0x05, 0xc8, 0x05, 0xd0, 0x05, 0x30, 0x00, 0xd5, 0x9e, 0x00, 0x00, 0xd9, 0x8c, 0xd4, 0x91, 0x08, 0x03, 0xb8, 0x05, 0xd0, 0x05, 0x32, 0x00, 0xd3, 0x95, 0x00, 0x00, 0xd5, 0x9e, 0xd4, 0x91, 0xd8, 0x05, 0x28, 0x03, 0xe0, 0x05, 0x3e, 0x00, 0xe2, 0x79, 0x00, 0x00, 0xd6, 0x78, 0xd9, 0x7f, 0xc8, 0x05, 0xd8, 0x05, 0xe0, 0x05, 0x3b, 0x00, 0xd9, 0x8c, 0x00, 0x00, 0xe2, 0x79, 0xd9, 0x7f, 0x10, 0x03, 0xc8, 0x05, 0xe0, 0x05, 0x4a, 0x00, 0xda, 0x8c, 0x00, 0x00, 0xd9, 0x8c, 0xd9, 0x7f, 0x68, 0x05, 0x38, 0x03, 0xe8, 0x05, 0x32, 0x00, 0xde, 0x6e, 0x00, 0x00, 0xd6, 0x6d, 0xdb, 0x70, 0xd8, 0x05, 0x68, 0x05, 0xe8, 0x05, 0x32, 0x00, 0xe2, 0x79, 0x00, 0x00, 0xde, 0x6e, 0xdb, 0x70, 0x28, 0x03, 0xd8, 0x05, 0xe8, 0x05, 0x30, 0x00, 0xd6, 0x78, 0x00, 0x00, 0xe2, 0x79, 0xdb, 0x70, 0xf0, 0x05, 0x58, 0x03, 0xf8, 0x05, 0x30, 0x00, 0xcd, 0x3e, 0x00, 0x00, 0xc8, 0x48, 0xcd, 0x46, 0x48, 0x05, 0xf0, 0x05, 0xf8, 0x05, 0x2f, 0x00, 0xd3, 0x3e, 0x00, 0x00, 0xcd, 0x3e, 0xcd, 0x46, 0x60, 0x03, 0x48, 0x05, 0xf8, 0x05, 0x30, 0x00, 0xcd, 0x58, 0x00, 0x00, 0xd3, 0x3e, 0xcd, 0x46, 0x00, 0x06, 0x68, 0x03, 0x08, 0x06, 0x2b, 0x00, 0xc8, 0x3c, 0x00, 0x00, 0xc6, 0x49, 0xc8, 0x42, 0xf0, 0x05, 0x00, 0x06, 0x08, 0x06, 0x30, 0x00, 0xcd, 0x3e, 0x00, 0x00, 0xc8, 0x3c, 0xc8, 0x42, 0x58, 0x03, 0xf0, 0x05, 0x08, 0x06, 0x31, 0x00, 0xc8, 0x48, 0x00, 0x00, 0xcd, 0x3e, 0xc8, 0x42, 0x10, 0x06, 0x80, 0x03, 0x18, 0x06, 0x30, 0x00, 0xbd, 0x48, 0x00, 0x00, 0xbe, 0x53, 0xc2, 0x4b, 0x00, 0x06, 0x10, 0x06, 0x18, 0x06, 0x2c, 0x00, 0xc8, 0x3c, 0x00, 0x00, 0xbd, 0x48, 0xc2, 0x4b, 0x68, 0x03, 0x00, 0x06, 0x18, 0x06, 0x2a, 0x00, 0xc6, 0x49, 0x00, 0x00, 0xc8, 0x3c, 0xc2, 0x4b, 0x20, 0x06, 0xa0, 0x03, 0x28, 0x06, 0x32, 0x00, 0xb6, 0x35, 0x00, 0x00, 0xba, 0x56, 0xbb, 0x4b, 0x10, 0x06, 0x20, 0x06, 0x28, 0x06, 0x30, 0x00, 0xbd, 0x48, 0x00, 0x00, 0xb6, 0x35, 0xbb, 0x4b, 0x80, 0x03, 0x10, 0x06, 0x28, 0x06, 0x30, 0x00, 0xbe, 0x53, 0x00, 0x00, 0xbd, 0x48, 0xbb, 0x4b, 0x30, 0x06, 0x98, 0x03, 0x38, 0x06, 0x2a, 0x00, 0xad, 0x3e, 0x00, 0x00, 0xb3, 0x51, 0xb4, 0x46, 0x20, 0x06, 0x30, 0x06, 0x38, 0x06, 0x2c, 0x00, 0xb6, 0x35, 0x00, 0x00, 0xad, 0x3e, 0xb4, 0x46, 0xa0, 0x03, 0x20, 0x06, 0x38, 0x06, 0x2b, 0x00, 0xba, 0x56, 0x00, 0x00, 0xb6, 0x35, 0xb4, 0x46, 0x40, 0x06, 0x48, 0x02, 0x48, 0x06, 0x2e, 0x00, 0xa2, 0x46, 0x00, 0x00, 0xb0, 0x5a, 0xad, 0x4b, 0x30, 0x06, 0x40, 0x06, 0x48, 0x06, 0x2c, 0x00, 0xad, 0x3e, 0x00, 0x00, 0xa2, 0x46, 0xad, 0x4b, 0x98, 0x03, 0x30, 0x06, 0x48, 0x06, 0x2c, 0x00, 0xb3, 0x51, 0x00, 0x00, 0xad, 0x3e, 0xad, 0x4b, 0x50, 0x06, 0x68, 0x02, 0x58, 0x06, 0x2f, 0x00, 0x9f, 0x5c, 0x00, 0x00, 0xad, 0x69, 0xaa, 0x5c, 0x40, 0x06, 0x50, 0x06, 0x58, 0x06, 0x2b, 0x00, 0xa2, 0x46, 0x00, 0x00, 0x9f, 0x5c, 0xaa, 0x5c, 0x48, 0x02, 0x40, 0x06, 0x58, 0x06, 0x32, 0x00, 0xb0, 0x5a, 0x00, 0x00, 0xa2, 0x46, 0xaa, 0x5c, 0x60, 0x06, 0x60, 0x02, 0x68, 0x06, 0x2c, 0x00, 0xa1, 0x6d, 0x00, 0x00, 0xac, 0x74, 0xa7, 0x6b, 0x50, 0x06, 0x60, 0x06, 0x68, 0x06, 0x2a, 0x00, 0x9f, 0x5c, 0x00, 0x00, 0xa1, 0x6d, 0xa7, 0x6b, 0x68, 0x02, 0x50, 0x06, 0x68, 0x06, 0x37, 0x00, 0xad, 0x69, 0x00, 0x00, 0x9f, 0x5c, 0xa7, 0x6b, 0x70, 0x06, 0x80, 0x02, 0x78, 0x06, 0x2a, 0x00, 0x9f, 0x85, 0x00, 0x00, 0xa4, 0x81, 0xa4, 0x78, 0x60, 0x06, 0x70, 0x06, 0x78, 0x06, 0x30, 0x00, 0xa1, 0x6d, 0x00, 0x00, 0x9f, 0x85, 0xa4, 0x78, 0x60, 0x02, 0x60, 0x06, 0x78, 0x06, 0x2b, 0x00, 0xac, 0x74, 0x00, 0x00, 0xa1, 0x6d, 0xa4, 0x78, 0x80, 0x06, 0x90, 0x02, 0x88, 0x06, 0x36, 0x00, 0xa2, 0x8c, 0x00, 0x00, 0xac, 0x88, 0xa5, 0x85, 0x70, 0x06, 0x80, 0x06, 0x88, 0x06, 0x37, 0x00, 0x9f, 0x85, 0x00, 0x00, 0xa2, 0x8c, 0xa5, 0x85, 0x80, 0x02, 0x70, 0x06, 0x88, 0x06, 0x48, 0x00, 0xa4, 0x81, 0x00, 0x00, 0x9f, 0x85, 0xa5, 0x85, 0x90, 0x06, 0xa0, 0x02, 0x98, 0x06, 0x2a, 0x00, 0xac, 0xb3, 0x00, 0x00, 0xb0, 0xa2, 0xad, 0x9e, 0xa0, 0x06, 0x90, 0x06, 0x98, 0x06, 0x2a, 0x00, 0xa6, 0x9d, 0x00, 0x00, 0xac, 0xb3, 0xad, 0x9e, 0xb8, 0x03, 0xa0, 0x06, 0x98, 0x06, 0x35, 0x00, 0xaf, 0x90, 0x00, 0x00, 0xa6, 0x9d, 0xad, 0x9e, 0xa0, 0x06, 0xb8, 0x03, 0xa8, 0x06, 0x3c, 0x00, 0xa6, 0x9d, 0x00, 0x00, 0xaf, 0x90, 0xa9, 0x90, 0x80, 0x06, 0xa0, 0x06, 0xa8, 0x06, 0x38, 0x00, 0xa2, 0x8c, 0x00, 0x00, 0xa6, 0x9d, 0xa9, 0x90, 0x90, 0x02, 0x80, 0x06, 0xa8, 0x06, 0x40, 0x00, 0xac, 0x88, 0x00, 0x00, 0xa2, 0x8c, 0xa9, 0x90, 0xb0, 0x06, 0xc0, 0x03, 0xb8, 0x06, 0x2e, 0x00, 0xaf, 0xbe, 0x00, 0x00, 0xb5, 0xa6, 0xb2, 0xab, 0x90, 0x06, 0xb0, 0x06, 0xb8, 0x06, 0x37, 0x00, 0xac, 0xb3, 0x00, 0x00, 0xaf, 0xbe, 0xb2, 0xab, 0xa0, 0x02, 0x90, 0x06, 0xb8, 0x06, 0x3e, 0x00, 0xb0, 0xa2, 0x00, 0x00, 0xac, 0xb3, 0xb2, 0xab, 0x90, 0x05, 0xb8, 0x02, 0xc0, 0x06, 0x3d, 0x00, 0xb7, 0xb8, 0x00, 0x00, 0xbb, 0xa0, 0xb6, 0xaf, 0xb0, 0x06, 0x90, 0x05, 0xc0, 0x06, 0x3a, 0x00, 0xaf, 0xbe, 0x00, 0x00, 0xb7, 0xb8, 0xb6, 0xaf, 0xc0, 0x03, 0xb0, 0x06, 0xc0, 0x06, 0x2e, 0x00, 0xb5, 0xa6, 0x00, 0x00, 0xaf, 0xbe, 0xb6, 0xaf, 0x78, 0x04, 0x20, 0x06, 0xc8, 0x06, 0x31, 0x00, 0xad, 0x00, 0x00, 0x00, 0xb6, 0x35, 0xb9, 0x2c, 0x88, 0x04, 0x78, 0x04, 0xc8, 0x06, 0x31, 0x00, 0xbf, 0x30, 0x00, 0x00, 0xad, 0x00, 0xb9, 0x2c, 0xd0, 0x06, 0x88, 0x04, 0xc8, 0x06, 0x32, 0x00, 0xbf, 0x33, 0x00, 0x00, 0xbf, 0x30, 0xb9, 0x2c, 0x10, 0x06, 0xd0, 0x06, 0xc8, 0x06, 0x2a, 0x00, 0xbd, 0x48, 0x00, 0x00, 0xbf, 0x33, 0xb9, 0x2c, 0xd0, 0x06, 0x10, 0x06, 0xd8, 0x06, 0x2f, 0x00, 0xbf, 0x33, 0x00, 0x00, 0xbd, 0x48, 0xc3, 0x37, 0x88, 0x04, 0xd0, 0x06, 0xd8, 0x06, 0x32, 0x00, 0xbf, 0x30, 0x00, 0x00, 0xbf, 0x33, 0xc3, 0x37, 0x98, 0x04, 0x88, 0x04, 0xd8, 0x06, 0x2d, 0x00, 0xcf, 0x24, 0x00, 0x00, 0xbf, 0x30, 0xc3, 0x37, 0x00, 0x06, 0x98, 0x04, 0xd8, 0x06, 0x2c, 0x00, 0xc8, 0x3c, 0x00, 0x00, 0xcf, 0x24, 0xc3, 0x37, 0x38, 0x05, 0x70, 0x05, 0xe0, 0x06, 0x42, 0x00, 0xc9, 0xda, 0x00, 0x00, 0xc9, 0xcb, 0xc3, 0xd2, 0xf0, 0x03, 0x38, 0x05, 0xe0, 0x06, 0x3e, 0x00, 0xbc, 0xee, 0x00, 0x00, 0xc9, 0xda, 0xc3, 0xd2, 0x80, 0x05, 0xf0, 0x03, 0xe0, 0x06, 0x3b, 0x00, 0xbd, 0xba, 0x00, 0x00, 0xbc, 0xee, 0xc3, 0xd2, 0xf0, 0x03, 0x80, 0x05, 0xe8, 0x06, 0x3a, 0x00, 0xbc, 0xee, 0x00, 0x00, 0xbd, 0xba, 0xb8, 0xcb, 0x00, 0x04, 0xf0, 0x03, 0xe8, 0x06, 0x3a, 0x00, 0xb3, 0xce, 0x00, 0x00, 0xbc, 0xee, 0xb8, 0xcb, 0x90, 0x05, 0x00, 0x04, 0xe8, 0x06, 0x3a, 0x00, 0xb7, 0xb8, 0x00, 0x00, 0xb3, 0xce, 0xb8, 0xcb, 0x28, 0x05, 0x98, 0x05, 0xf0, 0x06, 0x36, 0x00, 0xd9, 0xe1, 0x00, 0x00, 0xd1, 0xb1, 0xcf, 0xcd, 0x38, 0x05, 0x28, 0x05, 0xf0, 0x06, 0x3c, 0x00, 0xc9, 0xda, 0x00, 0x00, 0xd9, 0xe1, 0xcf, 0xcd, 0x70, 0x05, 0x38, 0x05, 0xf0, 0x06, 0x40, 0x00, 0xc9, 0xcb, 0x00, 0x00, 0xc9, 0xda, 0xcf, 0xcd, 0x18, 0x05, 0xa8, 0x05, 0xf8, 0x06, 0x46, 0x00, 0xde, 0xbe, 0x00, 0x00, 0xdb, 0xb8, 0xd9, 0xc0, 0x28, 0x05, 0x18, 0x05, 0xf8, 0x06, 0x38, 0x00, 0xd9, 0xe1, 0x00, 0x00, 0xde, 0xbe, 0xd9, 0xc0, 0x98, 0x05, 0x28, 0x05, 0xf8, 0x06, 0x39, 0x00, 0xd1, 0xb1, 0x00, 0x00, 0xd9, 0xe1, 0xd9, 0xc0, 0x08, 0x05, 0xb8, 0x05, 0x00, 0x07, 0x31, 0x00, 0xf0, 0xc0, 0x00, 0x00, 0xd5, 0x9e, 0xe1, 0xb3, 0x18, 0x05, 0x08, 0x05, 0x00, 0x07, 0x38, 0x00, 0xde, 0xbe, 0x00, 0x00, 0xf0, 0xc0, 0xe1, 0xb3, 0xa8, 0x05, 0x18, 0x05, 0x00, 0x07, 0x3c, 0x00, 0xdb, 0xb8, 0x00, 0x00, 0xde, 0xbe, 0xe1, 0xb3, 0xf8, 0x04, 0xc8, 0x05, 0x08, 0x07, 0x34, 0x00, 0xe9, 0x97, 0x00, 0x00, 0xd9, 0x8c, 0xe3, 0xa0, 0x08, 0x05, 0xf8, 0x04, 0x08, 0x07, 0x38, 0x00, 0xf0, 0xc0, 0x00, 0x00, 0xe9, 0x97, 0xe3, 0xa0, 0xb8, 0x05, 0x08, 0x05, 0x08, 0x07, 0x38, 0x00, 0xd5, 0x9e, 0x00, 0x00, 0xf0, 0xc0, 0xe3, 0xa0, 0xe8, 0x04, 0xd8, 0x05, 0x10, 0x07, 0x38, 0x00, 0xf4, 0x7b, 0x00, 0x00, 0xe2, 0x79, 0xe6, 0x85, 0xf8, 0x04, 0xe8, 0x04, 0x10, 0x07, 0x30, 0x00, 0xe9, 0x97, 0x00, 0x00, 0xf4, 0x7b, 0xe6, 0x85, 0xc8, 0x05, 0xf8, 0x04, 0x10, 0x07, 0x31, 0x00, 0xd9, 0x8c, 0x00, 0x00, 0xe9, 0x97, 0xe6, 0x85, 0xd8, 0x04, 0x68, 0x05, 0x18, 0x07, 0x30, 0x00, 0xe9, 0x6e, 0x00, 0x00, 0xde, 0x6e, 0xe8, 0x74, 0xe8, 0x04, 0xd8, 0x04, 0x18, 0x07, 0x30, 0x00, 0xf4, 0x7b, 0x00, 0x00, 0xe9, 0x6e, 0xe8, 0x74, 0xd8, 0x05, 0xe8, 0x04, 0x18, 0x07, 0x30, 0x00, 0xe2, 0x79, 0x00, 0x00, 0xf4, 0x7b, 0xe8, 0x74, 0xc8, 0x04, 0x58, 0x05, 0x20, 0x07, 0x31, 0x00, 0xde, 0x56, 0x00, 0x00, 0xd7, 0x5c, 0xde, 0x63, 0xd8, 0x04, 0xc8, 0x04, 0x20, 0x07, 0x30, 0x00, 0xe9, 0x6e, 0x00, 0x00, 0xde, 0x56, 0xde, 0x63, 0x68, 0x05, 0xd8, 0x04, 0x20, 0x07, 0x30, 0x00, 0xde, 0x6e, 0x00, 0x00, 0xe9, 0x6e, 0xde, 0x63, 0xb8, 0x04, 0x48, 0x05, 0x28, 0x07, 0x30, 0x00, 0xe0, 0x23, 0x00, 0x00, 0xd3, 0x3e, 0xd9, 0x44, 0xc8, 0x04, 0xb8, 0x04, 0x28, 0x07, 0x30, 0x00, 0xde, 0x56, 0x00, 0x00, 0xe0, 0x23, 0xd9, 0x44, 0x58, 0x05, 0xc8, 0x04, 0x28, 0x07, 0x30, 0x00, 0xd7, 0x5c, 0x00, 0x00, 0xde, 0x56, 0xd9, 0x44, 0xa8, 0x04, 0xf0, 0x05, 0x30, 0x07, 0x2e, 0x00, 0xd4, 0x28, 0x00, 0x00, 0xcd, 0x3e, 0xd5, 0x31, 0xb8, 0x04, 0xa8, 0x04, 0x30, 0x07, 0x2f, 0x00, 0xe0, 0x23, 0x00, 0x00, 0xd4, 0x28, 0xd5, 0x31, 0x48, 0x05, 0xb8, 0x04, 0x30, 0x07, 0x2f, 0x00, 0xd3, 0x3e, 0x00, 0x00, 0xe0, 0x23, 0xd5, 0x31, 0x98, 0x04, 0x00, 0x06, 0x38, 0x07, 0x2e, 0x00, 0xcf, 0x24, 0x00, 0x00, 0xc8, 0x3c, 0xcd, 0x31, 0xa8, 0x04, 0x98, 0x04, 0x38, 0x07, 0x2e, 0x00, 0xd4, 0x28, 0x00, 0x00, 0xcf, 0x24, 0xcd, 0x31, 0xf0, 0x05, 0xa8, 0x04, 0x38, 0x07, 0x2e, 0x00, 0xcd, 0x3e, 0x00, 0x00, 0xd4, 0x28, 0xcd, 0x31, 0x68, 0x04, 0x30, 0x06, 0x40, 0x07, 0x2c, 0x00, 0xa9, 0x28, 0x00, 0x00, 0xad, 0x3e, 0xaf, 0x26, 0x78, 0x04, 0x68, 0x04, 0x40, 0x07, 0x2c, 0x00, 0xad, 0x00, 0x00, 0x00, 0xa9, 0x28, 0xaf, 0x26, 0x20, 0x06, 0x78, 0x04, 0x40, 0x07, 0x2d, 0x00, 0xb6, 0x35, 0x00, 0x00, 0xad, 0x00, 0xaf, 0x26, 0x58, 0x04, 0x40, 0x06, 0x48, 0x07, 0x2a, 0x00, 0xa1, 0x37, 0x00, 0x00, 0xa2, 0x46, 0xa6, 0x37, 0x68, 0x04, 0x58, 0x04, 0x48, 0x07, 0x2a, 0x00, 0xa9, 0x28, 0x00, 0x00, 0xa1, 0x37, 0xa6, 0x37, 0x30, 0x06, 0x68, 0x04, 0x48, 0x07, 0x2c, 0x00, 0xad, 0x3e, 0x00, 0x00, 0xa9, 0x28, 0xa6, 0x37, 0x48, 0x04, 0x50, 0x06, 0x50, 0x07, 0x2a, 0x00, 0x96, 0x4b, 0x00, 0x00, 0x9f, 0x5c, 0x9d, 0x46, 0x58, 0x04, 0x48, 0x04, 0x50, 0x07, 0x2a, 0x00, 0xa1, 0x37, 0x00, 0x00, 0x96, 0x4b, 0x9d, 0x46, 0x40, 0x06, 0x58, 0x04, 0x50, 0x07, 0x2b, 0x00, 0xa2, 0x46, 0x00, 0x00, 0xa1, 0x37, 0x9d, 0x46, 0x38, 0x04, 0x60, 0x06, 0x58, 0x07, 0x2a, 0x00, 0x8b, 0x60, 0x00, 0x00, 0xa1, 0x6d, 0x97, 0x5e, 0x48, 0x04, 0x38, 0x04, 0x58, 0x07, 0x2a, 0x00, 0x96, 0x4b, 0x00, 0x00, 0x8b, 0x60, 0x97, 0x5e, 0x50, 0x06, 0x48, 0x04, 0x58, 0x07, 0x2a, 0x00, 0x9f, 0x5c, 0x00, 0x00, 0x96, 0x4b, 0x97, 0x5e, 0xe8, 0x03, 0x70, 0x06, 0x60, 0x07, 0x2a, 0x00, 0x92, 0x7d, 0x00, 0x00, 0x9f, 0x85, 0x9a, 0x76, 0x38, 0x04, 0xe8, 0x03, 0x60, 0x07, 0x2a, 0x00, 0x8b, 0x60, 0x00, 0x00, 0x92, 0x7d, 0x9a, 0x76, 0x60, 0x06, 0x38, 0x04, 0x60, 0x07, 0x3a, 0x00, 0xa1, 0x6d, 0x00, 0x00, 0x8b, 0x60, 0x9a, 0x76, 0xd8, 0x03, 0x80, 0x06, 0x68, 0x07, 0x3a, 0x00, 0x86, 0x9b, 0x00, 0x00, 0xa2, 0x8c, 0x96, 0x8a, 0xe8, 0x03, 0xd8, 0x03, 0x68, 0x07, 0x33, 0x00, 0x92, 0x7d, 0x00, 0x00, 0x86, 0x9b, 0x96, 0x8a, 0x70, 0x06, 0xe8, 0x03, 0x68, 0x07, 0x32, 0x00, 0x9f, 0x85, 0x00, 0x00, 0x92, 0x7d, 0x96, 0x8a, 0x20, 0x04, 0x90, 0x06, 0x70, 0x07, 0x2a, 0x00, 0x9c, 0xd6, 0x00, 0x00, 0xac, 0xb3, 0xa5, 0xb1, 0xc8, 0x03, 0x20, 0x04, 0x70, 0x07, 0x2d, 0x00, 0xa0, 0xa2, 0x00, 0x00, 0x9c, 0xd6, 0xa5, 0xb1, 0xa0, 0x06, 0xc8, 0x03, 0x70, 0x07, 0x31, 0x00, 0xa6, 0x9d, 0x00, 0x00, 0xa0, 0xa2, 0xa5, 0xb1, 0xc8, 0x03, 0xa0, 0x06, 0x78, 0x07, 0x3f, 0x00, 0xa0, 0xa2, 0x00, 0x00, 0xa6, 0x9d, 0x9b, 0x9b, 0xd8, 0x03, 0xc8, 0x03, 0x78, 0x07, 0x40, 0x00, 0x86, 0x9b, 0x00, 0x00, 0xa0, 0xa2, 0x9b, 0x9b, 0x80, 0x06, 0xd8, 0x03, 0x78, 0x07, 0x36, 0x00, 0xa2, 0x8c, 0x00, 0x00, 0x86, 0x9b, 0x9b, 0x9b, 0x10, 0x04, 0xb0, 0x06, 0x80, 0x07, 0x38, 0x00, 0xac, 0xd0, 0x00, 0x00, 0xaf, 0xbe, 0xab, 0xc5, 0x20, 0x04, 0x10, 0x04, 0x80, 0x07, 0x39, 0x00, 0x9c, 0xd6, 0x00, 0x00, 0xac, 0xd0, 0xab, 0xc5, 0x90, 0x06, 0x20, 0x04, 0x80, 0x07, 0x41, 0x00, 0xac, 0xb3, 0x00, 0x00, 0x9c, 0xd6, 0xab, 0xc5, 0x00, 0x04, 0x90, 0x05, 0x88, 0x07, 0x43, 0x00, 0xb3, 0xce, 0x00, 0x00, 0xb7, 0xb8, 0xb2, 0xc5, 0x10, 0x04, 0x00, 0x04, 0x88, 0x07, 0x42, 0x00, 0xac, 0xd0, 0x00, 0x00, 0xb3, 0xce, 0xb2, 0xc5, 0xb0, 0x06, 0x10, 0x04, 0x88, 0x07, 0x40, 0x00, 0xaf, 0xbe, 0x00, 0x00, 0xac, 0xd0, 0xb2, 0xc5, 0x90, 0x05, 0xb0, 0x06, 0x88, 0x07, 0x42, 0x00, 0xb7, 0xb8, 0x00, 0x00, 0xaf, 0xbe, 0xb2, 0xc5, 0xb0, 0x06, 0x90, 0x06, 0x80, 0x07, 0x40, 0x00, 0xaf, 0xbe, 0x00, 0x00, 0xac, 0xb3, 0xab, 0xc5, 0xa0, 0x06, 0x80, 0x06, 0x78, 0x07, 0x3a, 0x00, 0xa6, 0x9d, 0x00, 0x00, 0xa2, 0x8c, 0x9b, 0x9b, 0x90, 0x06, 0xa0, 0x06, 0x70, 0x07, 0x30, 0x00, 0xac, 0xb3, 0x00, 0x00, 0xa6, 0x9d, 0xa5, 0xb1, 0x80, 0x06, 0x70, 0x06, 0x68, 0x07, 0x38, 0x00, 0xa2, 0x8c, 0x00, 0x00, 0x9f, 0x85, 0x96, 0x8a, 0x70, 0x06, 0x60, 0x06, 0x60, 0x07, 0x2b, 0x00, 0x9f, 0x85, 0x00, 0x00, 0xa1, 0x6d, 0x9a, 0x76, 0x60, 0x06, 0x50, 0x06, 0x58, 0x07, 0x2e, 0x00, 0xa1, 0x6d, 0x00, 0x00, 0x9f, 0x5c, 0x97, 0x5e, 0x50, 0x06, 0x40, 0x06, 0x50, 0x07, 0x2a, 0x00, 0x9f, 0x5c, 0x00, 0x00, 0xa2, 0x46, 0x9d, 0x46, 0x40, 0x06, 0x30, 0x06, 0x48, 0x07, 0x2d, 0x00, 0xa2, 0x46, 0x00, 0x00, 0xad, 0x3e, 0xa6, 0x37, 0x30, 0x06, 0x20, 0x06, 0x40, 0x07, 0x2c, 0x00, 0xad, 0x3e, 0x00, 0x00, 0xb6, 0x35, 0xaf, 0x26, 0x00, 0x06, 0xf0, 0x05, 0x38, 0x07, 0x2e, 0x00, 0xc8, 0x3c, 0x00, 0x00, 0xcd, 0x3e, 0xcd, 0x31, 0xf0, 0x05, 0x48, 0x05, 0x30, 0x07, 0x2e, 0x00, 0xcd, 0x3e, 0x00, 0x00, 0xd3, 0x3e, 0xd5, 0x31, 0x48, 0x05, 0x58, 0x05, 0x28, 0x07, 0x30, 0x00, 0xd3, 0x3e, 0x00, 0x00, 0xd7, 0x5c, 0xd9, 0x44, 0x58, 0x05, 0x68, 0x05, 0x20, 0x07, 0x30, 0x00, 0xd7, 0x5c, 0x00, 0x00, 0xde, 0x6e, 0xde, 0x63, 0x68, 0x05, 0xd8, 0x05, 0x18, 0x07, 0x30, 0x00, 0xde, 0x6e, 0x00, 0x00, 0xe2, 0x79, 0xe8, 0x74, 0xd8, 0x05, 0xc8, 0x05, 0x10, 0x07, 0x39, 0x00, 0xe2, 0x79, 0x00, 0x00, 0xd9, 0x8c, 0xe6, 0x85, 0xc8, 0x05, 0xb8, 0x05, 0x08, 0x07, 0x32, 0x00, 0xd9, 0x8c, 0x00, 0x00, 0xd5, 0x9e, 0xe3, 0xa0, 0xb8, 0x05, 0xa8, 0x05, 0x00, 0x07, 0x31, 0x00, 0xd5, 0x9e, 0x00, 0x00, 0xdb, 0xb8, 0xe1, 0xb3, 0xa8, 0x05, 0x98, 0x05, 0xf8, 0x06, 0x42, 0x00, 0xdb, 0xb8, 0x00, 0x00, 0xd1, 0xb1, 0xd9, 0xc0, 0x98, 0x05, 0x70, 0x05, 0xf0, 0x06, 0x37, 0x00, 0xd1, 0xb1, 0x00, 0x00, 0xc9, 0xcb, 0xcf, 0xcd, 0x80, 0x05, 0x90, 0x05, 0xe8, 0x06, 0x3d, 0x00, 0xbd, 0xba, 0x00, 0x00, 0xb7, 0xb8, 0xb8, 0xcb, 0x70, 0x05, 0x80, 0x05, 0xe0, 0x06, 0x3c, 0x00, 0xc9, 0xcb, 0x00, 0x00, 0xbd, 0xba, 0xc3, 0xd2, 0x10, 0x06, 0x00, 0x06, 0xd8, 0x06, 0x2c, 0x00, 0xbd, 0x48, 0x00, 0x00, 0xc8, 0x3c, 0xc3, 0x37, 0x20, 0x06, 0x10, 0x06, 0xc8, 0x06, 0x32, 0x00, 0xb6, 0x35, 0x00, 0x00, 0xbd, 0x48, 0xb9, 0x2c, 0xb8, 0x02, 0xc0, 0x03, 0xc0, 0x06, 0x30, 0x00, 0xbb, 0xa0, 0x00, 0x00, 0xb5, 0xa6, 0xb6, 0xaf, 0xc0, 0x03, 0xa0, 0x02, 0xb8, 0x06, 0x41, 0x00, 0xb5, 0xa6, 0x00, 0x00, 0xb0, 0xa2, 0xb2, 0xab, 0xb8, 0x03, 0x90, 0x02, 0xa8, 0x06, 0x45, 0x00, 0xaf, 0x90, 0x00, 0x00, 0xac, 0x88, 0xa9, 0x90, 0xa0, 0x02, 0xb8, 0x03, 0x98, 0x06, 0x2a, 0x00, 0xb0, 0xa2, 0x00, 0x00, 0xaf, 0x90, 0xad, 0x9e, 0x90, 0x02, 0x80, 0x02, 0x88, 0x06, 0x47, 0x00, 0xac, 0x88, 0x00, 0x00, 0xa4, 0x81, 0xa5, 0x85, 0x80, 0x02, 0x60, 0x02, 0x78, 0x06, 0x2c, 0x00, 0xa4, 0x81, 0x00, 0x00, 0xac, 0x74, 0xa4, 0x78, 0x60, 0x02, 0x68, 0x02, 0x68, 0x06, 0x38, 0x00, 0xac, 0x74, 0x00, 0x00, 0xad, 0x69, 0xa7, 0x6b, 0x68, 0x02, 0x48, 0x02, 0x58, 0x06, 0x2a, 0x00, 0xad, 0x69, 0x00, 0x00, 0xb0, 0x5a, 0xaa, 0x5c, 0x48, 0x02, 0x98, 0x03, 0x48, 0x06, 0x2f, 0x00, 0xb0, 0x5a, 0x00, 0x00, 0xb3, 0x51, 0xad, 0x4b, 0x98, 0x03, 0xa0, 0x03, 0x38, 0x06, 0x2a, 0x00, 0xb3, 0x51, 0x00, 0x00, 0xba, 0x56, 0xb4, 0x46, 0xa0, 0x03, 0x80, 0x03, 0x28, 0x06, 0x2e, 0x00, 0xba, 0x56, 0x00, 0x00, 0xbe, 0x53, 0xbb, 0x4b, 0x80, 0x03, 0x68, 0x03, 0x18, 0x06, 0x2c, 0x00, 0xbe, 0x53, 0x00, 0x00, 0xc6, 0x49, 0xc2, 0x4b, 0x68, 0x03, 0x58, 0x03, 0x08, 0x06, 0x2b, 0x00, 0xc6, 0x49, 0x00, 0x00, 0xc8, 0x48, 0xc8, 0x42, 0x58, 0x03, 0x60, 0x03, 0xf8, 0x05, 0x2f, 0x00, 0xc8, 0x48, 0x00, 0x00, 0xcd, 0x58, 0xcd, 0x46, 0x38, 0x03, 0x28, 0x03, 0xe8, 0x05, 0x30, 0x00, 0xd6, 0x6d, 0x00, 0x00, 0xd6, 0x78, 0xdb, 0x70, 0x28, 0x03, 0x10, 0x03, 0xe0, 0x05, 0x32, 0x00, 0xd6, 0x78, 0x00, 0x00, 0xda, 0x8c, 0xd9, 0x7f, 0x10, 0x03, 0x08, 0x03, 0xd0, 0x05, 0x51, 0x00, 0xda, 0x8c, 0x00, 0x00, 0xd3, 0x95, 0xd4, 0x91, 0x08, 0x03, 0xf8, 0x02, 0xc0, 0x05, 0x40, 0x00, 0xd3, 0x95, 0x00, 0x00, 0xd1, 0xa0, 0xd3, 0xa0, 0xf8, 0x02, 0xe0, 0x02, 0xb0, 0x05, 0x40, 0x00, 0xd1, 0xa0, 0x00, 0x00, 0xc9, 0xa4, 0xd1, 0xa9, 0xe0, 0x02, 0xd0, 0x02, 0xa0, 0x05, 0x31, 0x00, 0xc9, 0xa4, 0x00, 0x00, 0xc4, 0xb1, 0xc9, 0xb3, 0xa8, 0x03, 0xb8, 0x02, 0x88, 0x05, 0x3e, 0x00, 0xbd, 0xa2, 0x00, 0x00, 0xbb, 0xa0, 0xba, 0xad, 0xd0, 0x02, 0xa8, 0x03, 0x78, 0x05, 0x37, 0x00, 0xc4, 0xb1, 0x00, 0x00, 0xbd, 0xa2, 0xc2, 0xb6, 0x40, 0x03, 0x38, 0x03, 0x60, 0x05, 0x31, 0x00, 0xd0, 0x63, 0x00, 0x00, 0xd6, 0x6d, 0xd7, 0x65, 0x60, 0x03, 0x40, 0x03, 0x50, 0x05, 0x31, 0x00, 0xcd, 0x58, 0x00, 0x00, 0xd0, 0x63, 0xd1, 0x56, 0x38, 0x05, 0xf0, 0x03, 0x40, 0x05, 0x45, 0x00, 0xc9, 0xda, 0x00, 0x00, 0xbc, 0xee, 0xc4, 0xf0, 0x28, 0x05, 0x38, 0x05, 0x30, 0x05, 0x48, 0x00, 0xd9, 0xe1, 0x00, 0x00, 0xc9, 0xda, 0xd3, 0xe8, 0x18, 0x05, 0x28, 0x05, 0x20, 0x05, 0x3a, 0x00, 0xde, 0xbe, 0x00, 0x00, 0xd9, 0xe1, 0xdf, 0xda, 0x08, 0x05, 0x18, 0x05, 0x10, 0x05, 0x48, 0x00, 0xf0, 0xc0, 0x00, 0x00, 0xde, 0xbe, 0xeb, 0xc5, 0xf8, 0x04, 0x08, 0x05, 0x00, 0x05, 0x31, 0x00, 0xe9, 0x97, 0x00, 0x00, 0xf0, 0xc0, 0xf2, 0xab, 0xe8, 0x04, 0xf8, 0x04, 0xf0, 0x04, 0x3e, 0x00, 0xf4, 0x7b, 0x00, 0x00, 0xe9, 0x97, 0xf4, 0x8c, 0xd8, 0x04, 0xe8, 0x04, 0xe0, 0x04, 0x32, 0x00, 0xe9, 0x6e, 0x00, 0x00, 0xf4, 0x7b, 0xf4, 0x70, 0xc8, 0x04, 0xd8, 0x04, 0xd0, 0x04, 0x32, 0x00, 0xde, 0x56, 0x00, 0x00, 0xe9, 0x6e, 0xec, 0x56, 0xb8, 0x04, 0xc8, 0x04, 0xc0, 0x04, 0x30, 0x00, 0xe0, 0x23, 0x00, 0x00, 0xde, 0x56, 0xe6, 0x35, 0xa8, 0x04, 0xb8, 0x04, 0xb0, 0x04, 0x2f, 0x00, 0xd4, 0x28, 0x00, 0x00, 0xe0, 0x23, 0xde, 0x18, 0x98, 0x04, 0xa8, 0x04, 0xa0, 0x04, 0x31, 0x00, 0xcf, 0x24, 0x00, 0x00, 0xd4, 0x28, 0xd3, 0x16, 0x88, 0x04, 0x98, 0x04, 0x90, 0x04, 0x2d, 0x00, 0xbf, 0x30, 0x00, 0x00, 0xcf, 0x24, 0xc7, 0x14, 0x78, 0x04, 0x88, 0x04, 0x80, 0x04, 0x32, 0x00, 0xad, 0x00, 0x00, 0x00, 0xbf, 0x30, 0xb7, 0x09, 0x68, 0x04, 0x78, 0x04, 0x70, 0x04, 0x2b, 0x00, 0xa9, 0x28, 0x00, 0x00, 0xad, 0x00, 0xa6, 0x05, 0x58, 0x04, 0x68, 0x04, 0x60, 0x04, 0x2a, 0x00, 0xa1, 0x37, 0x00, 0x00, 0xa9, 0x28, 0x9e, 0x1d, 0x48, 0x04, 0x58, 0x04, 0x50, 0x04, 0x2a, 0x00, 0x96, 0x4b, 0x00, 0x00, 0xa1, 0x37, 0x94, 0x31, 0x38, 0x04, 0x48, 0x04, 0x40, 0x04, 0x2a, 0x00, 0x8b, 0x60, 0x00, 0x00, 0x96, 0x4b, 0x8d, 0x53, 0xe8, 0x03, 0x38, 0x04, 0x30, 0x04, 0x37, 0x00, 0x92, 0x7d, 0x00, 0x00, 0x8b, 0x60, 0x8b, 0x6e, 0x20, 0x04, 0xc8, 0x03, 0x28, 0x04, 0x2a, 0x00, 0x9c, 0xd6, 0x00, 0x00, 0xa0, 0xa2, 0x97, 0xc3, 0x10, 0x04, 0x20, 0x04, 0x18, 0x04, 0x49, 0x00, 0xac, 0xd0, 0x00, 0x00, 0x9c, 0xd6, 0xa0, 0xdb, 0x00, 0x04, 0x10, 0x04, 0x08, 0x04, 0x44, 0x00, 0xb3, 0xce, 0x00, 0x00, 0xac, 0xd0, 0xac, 0xe1, 0xf0, 0x03, 0x00, 0x04, 0xf8, 0x03, 0x3c, 0x00, 0xbc, 0xee, 0x00, 0x00, 0xb3, 0xce, 0xb7, 0xec, 0xd8, 0x03, 0xe8, 0x03, 0xe0, 0x03, 0x2a, 0x00, 0x86, 0x9b, 0x00, 0x00, 0x92, 0x7d, 0x87, 0x8e, 0xc8, 0x03, 0xd8, 0x03, 0xd0, 0x03, 0x42, 0x00, 0xa0, 0xa2, 0x00, 0x00, 0x86, 0x9b, 0x8d, 0xa6, 0x08, 0x00, 0xc0, 0x00, 0x10, 0x00, 0x80, 0x00, 0xc0, 0xb1, 0x00, 0x00, 0xba, 0xa6, 0xbe, 0x7d, 0xc0, 0x02, 0xc8, 0x02, 0x90, 0x07, 0x3e, 0x00, 0xbd, 0x90, 0x00, 0x00, 0xbf, 0x8a, 0xc0, 0x7d, 0xb0, 0x02, 0xc0, 0x02, 0x90, 0x07, 0x3e, 0x00, 0xb9, 0x90, 0x00, 0x00, 0xbd, 0x90, 0xc0, 0x7d, 0xa8, 0x02, 0xb0, 0x02, 0x90, 0x07, 0x2a, 0x00, 0xbb, 0x88, 0x00, 0x00, 0xb9, 0x90, 0xc0, 0x7d, 0x98, 0x02, 0xa8, 0x02, 0x90, 0x07, 0x44, 0x00, 0xb8, 0x88, 0x00, 0x00, 0xbb, 0x88, 0xc0, 0x7d, 0x88, 0x02, 0x98, 0x02, 0x90, 0x07, 0x2a, 0x00, 0xb6, 0x83, 0x00, 0x00, 0xb8, 0x88, 0xc0, 0x7d, 0x70, 0x02, 0x88, 0x02, 0x90, 0x07, 0x2a, 0x00, 0xb7, 0x7d, 0x00, 0x00, 0xb6, 0x83, 0xc0, 0x7d, 0x78, 0x02, 0x70, 0x02, 0x90, 0x07, 0x46, 0x00, 0xb6, 0x79, 0x00, 0x00, 0xb7, 0x7d, 0xc0, 0x7d, 0x50, 0x02, 0x78, 0x02, 0x90, 0x07, 0x2d, 0x00, 0xb7, 0x74, 0x00, 0x00, 0xb6, 0x79, 0xc0, 0x7d, 0x58, 0x02, 0x50, 0x02, 0x90, 0x07, 0x2a, 0x00, 0xb7, 0x6b, 0x00, 0x00, 0xb7, 0x74, 0xc0, 0x7d, 0x90, 0x03, 0x58, 0x02, 0x90, 0x07, 0x30, 0x00, 0xbb, 0x6e, 0x00, 0x00, 0xb7, 0x6b, 0xc0, 0x7d, 0x88, 0x03, 0x90, 0x03, 0x90, 0x07, 0x2a, 0x00, 0xbe, 0x6b, 0x00, 0x00, 0xbb, 0x6e, 0xc0, 0x7d, 0x78, 0x03, 0x88, 0x03, 0x90, 0x07, 0x2a, 0x00, 0xc0, 0x63, 0x00, 0x00, 0xbe, 0x6b, 0xc0, 0x7d, 0xb0, 0x03, 0x78, 0x03, 0x90, 0x07, 0x2d, 0x00, 0xc1, 0x6e, 0x00, 0x00, 0xc0, 0x63, 0xc0, 0x7d, 0x70, 0x03, 0xb0, 0x03, 0x90, 0x07, 0x30, 0x00, 0xc3, 0x6d, 0x00, 0x00, 0xc1, 0x6e, 0xc0, 0x7d, 0x50, 0x03, 0x70, 0x03, 0x90, 0x07, 0x2c, 0x00, 0xc8, 0x65, 0x00, 0x00, 0xc3, 0x6d, 0xc0, 0x7d, 0x48, 0x03, 0x50, 0x03, 0x90, 0x07, 0x37, 0x00, 0xc5, 0x74, 0x00, 0x00, 0xc8, 0x65, 0xc0, 0x7d, 0x30, 0x03, 0x48, 0x03, 0x90, 0x07, 0x2e, 0x00, 0xc9, 0x76, 0x00, 0x00, 0xc5, 0x74, 0xc0, 0x7d, 0x20, 0x03, 0x30, 0x03, 0x90, 0x07, 0x3d, 0x00, 0xca, 0x7b, 0x00, 0x00, 0xc9, 0x76, 0xc0, 0x7d, 0x00, 0x03, 0x20, 0x03, 0x90, 0x07, 0x31, 0x00, 0xcd, 0x83, 0x00, 0x00, 0xca, 0x7b, 0xc0, 0x7d, 0x18, 0x03, 0x00, 0x03, 0x90, 0x07, 0x48, 0x00, 0xc5, 0x83, 0x00, 0x00, 0xcd, 0x83, 0xc0, 0x7d, 0xf0, 0x02, 0x18, 0x03, 0x90, 0x07, 0x33, 0x00, 0xc8, 0x90, 0x00, 0x00, 0xc5, 0x83, 0xc0, 0x7d, 0xe8, 0x02, 0xf0, 0x02, 0x90, 0x07, 0x43, 0x00, 0xc2, 0x8a, 0x00, 0x00, 0xc8, 0x90, 0xc0, 0x7d, 0xd8, 0x02, 0xe8, 0x02, 0x90, 0x07, 0x36, 0x00, 0xc2, 0x93, 0x00, 0x00, 0xc2, 0x8a, 0xc0, 0x7d, 0xc8, 0x02, 0xd8, 0x02, 0x90, 0x07, 0x3a, 0x00, 0xbf, 0x8a, 0x00, 0x00, 0xc2, 0x93, 0xc0, 0x7d, 0x30, 0x00, 0x00, 0x00, 0xc8, 0x00, 0xd0, 0x00, 0x18, 0x00, 0x00, 0x00, 0xd3, 0xc5, 0x00, 0x00, 0xc9, 0xd2, 0xca, 0xa6, 0xc3, 0xad, 0x80, 0x00, 0xd8, 0x00, 0xc8, 0x00, 0x20, 0x00, 0x18, 0x00, 0xde, 0xbc, 0x00, 0x00, 0xd3, 0xc5, 0xd1, 0x9e, 0xca, 0xa6, 0x80, 0x00, 0xe0, 0x00, 0xd8, 0x00, 0x28, 0x00, 0x20, 0x00, 0xe8, 0xaf, 0x00, 0x00, 0xde, 0xbc, 0xd6, 0x99, 0xd1, 0x9e, 0x80, 0x00, 0xe8, 0x00, 0xe0, 0x00, 0x30, 0x00, 0x28, 0x00, 0xed, 0x97, 0x00, 0x00, 0xe8, 0xaf, 0xd9, 0x8e, 0xd6, 0x99, 0x80, 0x00, 0xf0, 0x00, 0xe8, 0x00, 0x38, 0x00, 0x30, 0x00, 0xe9, 0x81, 0x00, 0x00, 0xed, 0x97, 0xd7, 0x83, 0xd9, 0x8e, 0x80, 0x00, 0xf8, 0x00, 0xf0, 0x00, 0x40, 0x00, 0x38, 0x00, 0xe4, 0x67, 0x00, 0x00, 0xe9, 0x81, 0xd4, 0x74, 0xd7, 0x83, 0x80, 0x00, 0x00, 0x01, 0xf8, 0x00, 0x48, 0x00, 0x40, 0x00, 0xdd, 0x58, 0x00, 0x00, 0xe4, 0x67, 0xcf, 0x69, 0xd4, 0x74, 0x80, 0x00, 0x08, 0x01, 0x00, 0x01, 0x50, 0x00, 0x48, 0x00, 0xdc, 0x3b, 0x00, 0x00, 0xdd, 0x58, 0xcf, 0x54, 0xcf, 0x69, 0x80, 0x00, 0x10, 0x01, 0x08, 0x01, 0x58, 0x00, 0x50, 0x00, 0xd5, 0x33, 0x00, 0x00, 0xdc, 0x3b, 0xcd, 0x53, 0xcf, 0x54, 0x80, 0x00, 0x18, 0x01, 0x10, 0x01, 0x60, 0x00, 0x58, 0x00, 0xca, 0x31, 0x00, 0x00, 0xd5, 0x33, 0xc5, 0x54, 0xcd, 0x53, 0x80, 0x00, 0x20, 0x01, 0x18, 0x01, 0x68, 0x00, 0x60, 0x00, 0xbd, 0x26, 0x00, 0x00, 0xca, 0x31, 0xbd, 0x4d, 0xc5, 0x54, 0x80, 0x00, 0x28, 0x01, 0x20, 0x01, 0x70, 0x00, 0x68, 0x00, 0xb3, 0x23, 0x00, 0x00, 0xbd, 0x26, 0xb7, 0x4b, 0xbd, 0x4d, 0x80, 0x00, 0x30, 0x01, 0x28, 0x01, 0x78, 0x00, 0x70, 0x00, 0xa9, 0x2c, 0x00, 0x00, 0xb3, 0x23, 0xb2, 0x4f, 0xb7, 0x4b, 0x80, 0x00, 0x38, 0x01, 0x30, 0x01, 0x80, 0x00, 0x78, 0x00, 0xa0, 0x3c, 0x00, 0x00, 0xa9, 0x2c, 0xac, 0x58, 0xb2, 0x4f, 0x80, 0x00, 0x40, 0x01, 0x38, 0x01, 0x88, 0x00, 0x80, 0x00, 0x96, 0x4b, 0x00, 0x00, 0xa0, 0x3c, 0xa6, 0x61, 0xac, 0x58, 0x80, 0x00, 0x48, 0x01, 0x40, 0x01, 0x90, 0x00, 0x88, 0x00, 0x90, 0x63, 0x00, 0x00, 0x96, 0x4b, 0xa4, 0x6d, 0xa6, 0x61, 0x80, 0x00, 0x50, 0x01, 0x48, 0x01, 0x98, 0x00, 0x90, 0x00, 0x94, 0x7b, 0x00, 0x00, 0x90, 0x63, 0xa6, 0x7b, 0xa4, 0x6d, 0x80, 0x00, 0x58, 0x01, 0x50, 0x01, 0xa0, 0x00, 0x98, 0x00, 0x92, 0x97, 0x00, 0x00, 0x94, 0x7b, 0xa5, 0x8c, 0xa6, 0x7b, 0x80, 0x00, 0x60, 0x01, 0x58, 0x01, 0xa8, 0x00, 0xa0, 0x00, 0x99, 0xa9, 0x00, 0x00, 0x92, 0x97, 0xa9, 0x95, 0xa5, 0x8c, 0x80, 0x00, 0x68, 0x01, 0x60, 0x01, 0xb0, 0x00, 0xa8, 0x00, 0xa1, 0xbe, 0x00, 0x00, 0x99, 0xa9, 0xad, 0xa2, 0xa9, 0x95, 0x80, 0x00, 0x70, 0x01, 0x68, 0x01, 0xb8, 0x00, 0xb0, 0x00, 0xa8, 0xcb, 0x00, 0x00, 0xa1, 0xbe, 0xb1, 0xa8, 0xad, 0xa2, 0x80, 0x00, 0x78, 0x01, 0x70, 0x01, 0xc0, 0x00, 0xb8, 0x00, 0xb6, 0xc9, 0x00, 0x00, 0xa8, 0xcb, 0xba, 0xa6, 0xb1, 0xa8, 0x80, 0x00, 0x80, 0x01, 0x78, 0x01, 0x08, 0x00, 0xc0, 0x00, 0xbf, 0xd8, 0x00, 0x00, 0xb6, 0xc9, 0xc0, 0xb1, 0xba, 0xa6, 0x80, 0x00, 0xd0, 0x00, 0x80, 0x01, 0x00, 0x00, 0x08, 0x00, 0xc9, 0xd2, 0x00, 0x00, 0xbf, 0xd8, 0xc3, 0xad, 0xc0, 0xb1, 0x80, 0x00, 0x88, 0x01, 0x90, 0x01, 0xc8, 0x00, 0xd0, 0x00, 0xda, 0xe3, 0x00, 0x00, 0xce, 0xf2, 0xd3, 0xc5, 0xc9, 0xd2, 0x80, 0x00, 0x98, 0x01, 0x88, 0x01, 0xd8, 0x00, 0xc8, 0x00, 0xe9, 0xd8, 0x00, 0x00, 0xda, 0xe3, 0xde, 0xbc, 0xd3, 0xc5, 0x80, 0x00, 0xa0, 0x01, 0x98, 0x01, 0xe0, 0x00, 0xd8, 0x00, 0xf6, 0xc3, 0x00, 0x00, 0xe9, 0xd8, 0xe8, 0xaf, 0xde, 0xbc, 0x80, 0x00, 0xa8, 0x01, 0xa0, 0x01, 0xe8, 0x00, 0xe0, 0x00, 0xff, 0x9e, 0x00, 0x00, 0xf6, 0xc3, 0xed, 0x97, 0xe8, 0xaf, 0x80, 0x00, 0xb0, 0x01, 0xa8, 0x01, 0xf0, 0x00, 0xe8, 0x00, 0xfa, 0x7b, 0x00, 0x00, 0xff, 0x9e, 0xe9, 0x81, 0xed, 0x97, 0x80, 0x00, 0xb8, 0x01, 0xb0, 0x01, 0xf8, 0x00, 0xf0, 0x00, 0xf3, 0x5a, 0x00, 0x00, 0xfa, 0x7b, 0xe4, 0x67, 0xe9, 0x81, 0x80, 0x00, 0xc0, 0x01, 0xb8, 0x01, 0x00, 0x01, 0xf8, 0x00, 0xea, 0x49, 0x00, 0x00, 0xf3, 0x5a, 0xdd, 0x58, 0xe4, 0x67, 0x80, 0x00, 0xc8, 0x01, 0xc0, 0x01, 0x08, 0x01, 0x00, 0x01, 0xe9, 0x21, 0x00, 0x00, 0xea, 0x49, 0xdc, 0x3b, 0xdd, 0x58, 0x80, 0x00, 0xd0, 0x01, 0xc8, 0x01, 0x10, 0x01, 0x08, 0x01, 0xdc, 0x14, 0x00, 0x00, 0xe9, 0x21, 0xd5, 0x33, 0xdc, 0x3b, 0x80, 0x00, 0xd8, 0x01, 0xd0, 0x01, 0x18, 0x01, 0x10, 0x01, 0xce, 0x12, 0x00, 0x00, 0xdc, 0x14, 0xca, 0x31, 0xd5, 0x33, 0x80, 0x00, 0xe0, 0x01, 0xd8, 0x01, 0x20, 0x01, 0x18, 0x01, 0xbe, 0x05, 0x00, 0x00, 0xce, 0x12, 0xbd, 0x26, 0xca, 0x31, 0x80, 0x00, 0xe8, 0x01, 0xe0, 0x01, 0x28, 0x01, 0x20, 0x01, 0xaf, 0x00, 0x00, 0x00, 0xbe, 0x05, 0xb3, 0x23, 0xbd, 0x26, 0x80, 0x00, 0xf0, 0x01, 0xe8, 0x01, 0x30, 0x01, 0x28, 0x01, 0xa1, 0x0c, 0x00, 0x00, 0xaf, 0x00, 0xa9, 0x2c, 0xb3, 0x23, 0x80, 0x00, 0xf8, 0x01, 0xf0, 0x01, 0x38, 0x01, 0x30, 0x01, 0x95, 0x21, 0x00, 0x00, 0xa1, 0x0c, 0xa0, 0x3c, 0xa9, 0x2c, 0x80, 0x00, 0x00, 0x02, 0xf8, 0x01, 0x40, 0x01, 0x38, 0x01, 0x86, 0x35, 0x00, 0x00, 0x95, 0x21, 0x96, 0x4b, 0xa0, 0x3c, 0x80, 0x00, 0x08, 0x02, 0x00, 0x02, 0x48, 0x01, 0x40, 0x01, 0x80, 0x5a, 0x00, 0x00, 0x86, 0x35, 0x90, 0x63, 0x96, 0x4b, 0x80, 0x00, 0x10, 0x02, 0x08, 0x02, 0x50, 0x01, 0x48, 0x01, 0x84, 0x7d, 0x00, 0x00, 0x80, 0x5a, 0x94, 0x7b, 0x90, 0x63, 0x80, 0x00, 0x18, 0x02, 0x10, 0x02, 0x58, 0x01, 0x50, 0x01, 0x80, 0xa0, 0x00, 0x00, 0x84, 0x7d, 0x92, 0x97, 0x94, 0x7b, 0x80, 0x00, 0x20, 0x02, 0x18, 0x02, 0x60, 0x01, 0x58, 0x01, 0x8b, 0xbc, 0x00, 0x00, 0x80, 0xa0, 0x99, 0xa9, 0x92, 0x97, 0x80, 0x00, 0x28, 0x02, 0x20, 0x02, 0x68, 0x01, 0x60, 0x01, 0x95, 0xd8, 0x00, 0x00, 0x8b, 0xbc, 0xa1, 0xbe, 0x99, 0xa9, 0x80, 0x00, 0x30, 0x02, 0x28, 0x02, 0x70, 0x01, 0x68, 0x01, 0xa1, 0xec, 0x00, 0x00, 0x95, 0xd8, 0xa8, 0xcb, 0xa1, 0xbe, 0x80, 0x00, 0x38, 0x02, 0x30, 0x02, 0x78, 0x01, 0x70, 0x01, 0xb2, 0xe8, 0x00, 0x00, 0xa1, 0xec, 0xb6, 0xc9, 0xa8, 0xcb, 0x80, 0x00, 0x40, 0x02, 0x38, 0x02, 0x80, 0x01, 0x78, 0x01, 0xbf, 0xfd, 0x00, 0x00, 0xb2, 0xe8, 0xbf, 0xd8, 0xb6, 0xc9, 0x80, 0x00, 0x90, 0x01, 0x40, 0x02, 0xd0, 0x00, 0x80, 0x01};
unsigned char g_rawData_007f9108[24] = {0xce, 0xf2, 0x00, 0x00, 0xbf, 0xfd, 0xc9, 0xd2, 0xbf, 0xd8, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007f9120
int g_BattleSummonNeoBahamutResourceDef = 0x00C840B1;
int g_BattleSummonNeoBahamutResourceDefHeadPtr[1] = {
    (int)((char *)&g_BattleFxSummonNeoBahamutAssetPrefix + 0x0)
};
int g_BattleSummonNeoBahamutResourceDefDataTail[98] = {
    0x0, 0x0, 0x0, 0xc840b1, (int)((char *)&g_BattleFxSummonNeoBahamutResourceNames + 0x8), 0x0,
    0x0, 0x0, 0xc840b1, (int)((char *)&g_BattleFxSummonNeoBahamutResourceNames + 0x14), 0x0, 0x0,
    0x0, 0x1f440b1, (int)((char *)&g_BattleFxSummonNeoBahamutResourceNames + 0x20), 0x0, 0x0, 0x0,
    0xdc40b1, (int)((char *)&g_BattleFxSummonNeoBahamutResourceNames + 0x2c), 0x0, 0x0, 0x0, 0xc800b4,
    (int)((char *)&g_BattleFxSummonNeoBahamutResourceNames + 0x38), 0x0, 0x0, 0x0, 0xc800b1, (int)((char *)&g_BattleFxSummonNeoBahamutResourceNames + 0x44),
    0x0, 0x0, 0x0, 0xc800b4, (int)((char *)&g_BattleFxSummonNeoBahamutResourceNames + 0x50), 0x0,
    0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleFxSummonNeoBahamutResourceNames + 0x5c), 0x0, 0x0,
    0x0, 0xc800b4, (int)((char *)&g_BattleFxSummonNeoBahamutResourceNames + 0x68), 0x0, 0x0, 0x0,
    0xc800b1, (int)((char *)&g_BattleFxSummonNeoBahamutResourceNames + 0x74), 0x0, 0x0, 0x0, 0x1f400b4,
    (int)((char *)&g_BattleFxSummonNeoBahamutResourceNames + 0x80), 0x0, 0x0, 0x0, 0x1f400b1, (int)((char *)&g_BattleFxSummonNeoBahamutResourceNames + 0x8c),
    0x0, 0x0, 0x0, 0x1f400b4, (int)((char *)&g_BattleFxSummonNeoBahamutResourceNames + 0x98), 0x0,
    0x0, 0x0, 0x1f440b1, (int)((char *)&g_BattleFxSummonNeoBahamutResourceNames + 0xa4), 0x0, 0x0,
    0x0, 0xc800b1, (int)((char *)&g_BattleFxSummonNeoBahamutResourceNames + 0xb0), 0x0, 0x0, 0x0,
    0x192a38, (int)((char *)&g_BattleFxSummonNeoBahamutResourceNames + 0xbc), (int)((char *)&g_BattleFxSummonNeoBahamutResourceNames + 0xc8), 0x0, 0x0, 0x192a39,
    (int)((char *)&g_BattleFxSummonNeoBahamutResourceNames + 0xd4), (int)((char *)&g_BattleFxSummonNeoBahamutResourceNames + 0xe4), 0x0, 0x0, 0x192a39, 0x0,
    (int)((char *)&g_BattleFxSummonNeoBahamutResourceNames + 0xf0), 0x0, 0x0, 0x192a3c, (int)((char *)&g_BattleFxSummonNeoBahamutResourceNames + 0xfc), (int)((char *)&g_BattleFxSummonNeoBahamutResourceNames + 0x108),
    0x0, 0x0
};
// GLOBAL: FF7 0x007f92b0
BattleFxNeoBahamutResIds g_BattleNeoBahamutPrimaryResourceIds = {{5, 0, 0, 0, 7, 0, 0, 0, 9, 0, 0, 0}, 6, {8, 0, 0, 0, 0xa, 0, 0, 0}};
// GLOBAL: FF7 0x007f92c8
short g_BattleSummonNeoBahamutAnchorPosition[4] = {0, (-1500), (-7500), 0};
// GLOBAL: FF7 0x007f92d0
int g_BattleNeoBahamutField4OffscreenSlot = (-1);
// GLOBAL: FF7 0x007f92d4
int g_BattleNeoBahamutField12OffscreenSlot = (-1);
// GLOBAL: FF7 0x007f92d8
unsigned char g_BattleSummonNeoBahamutCameraScript1[344] = {
    0x89, 0x00, 0x04, 0x00, 0x49, 0x00, 0x04, 0x00, 0x81, 0x00, 0x00, 0x00, 0xe8, 0x03, 0x00, 0x00,
    0x41, 0x00, 0xf0, 0xd8, 0x78, 0xec, 0xf0, 0xd8, 0x80, 0x00, 0x14, 0x00, 0x40, 0x00, 0x14, 0x00,
    0x82, 0x00, 0x19, 0x00, 0x00, 0x00, 0x30, 0xf8, 0x00, 0x00, 0x40, 0x00, 0x19, 0x00, 0x81, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0xc0, 0xe0, 0xd0, 0x8a, 0xf0, 0xd8, 0x80, 0x00,
    0x14, 0x00, 0x40, 0x00, 0x14, 0x00, 0x08, 0x00, 0x60, 0x00, 0x81, 0x00, 0x00, 0x00, 0xf0, 0xd8,
    0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x13, 0x82, 0x00, 0x64, 0x00, 0x00, 0x00,
    0x68, 0xc5, 0x00, 0x00, 0x40, 0x00, 0x64, 0x00, 0x08, 0x00, 0x00, 0x01, 0x81, 0x00, 0x88, 0x13,
    0x40, 0x1f, 0x00, 0x00, 0x82, 0x00, 0x37, 0x00, 0xe8, 0x03, 0x18, 0xfc, 0x30, 0xf8, 0x41, 0x00,
    0x48, 0xf4, 0x48, 0xf4, 0xb8, 0x0b, 0x42, 0x00, 0x37, 0x00, 0x78, 0xec, 0xf0, 0xd8, 0x90, 0xe8,
    0x08, 0x00, 0x00, 0x02, 0x81, 0x00, 0x00, 0x00, 0xd0, 0x07, 0xd0, 0x8a, 0x82, 0x00, 0x5d, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x78, 0xec, 0x41, 0x00, 0x88, 0x13, 0x18, 0xfc, 0x00, 0x00, 0x42, 0x00,
    0x5d, 0x00, 0x68, 0xc5, 0xc0, 0xe0, 0xa8, 0x61, 0x81, 0x00, 0x00, 0x00, 0x24, 0xfa, 0xb4, 0xe2,
    0x41, 0x00, 0x98, 0x3a, 0xac, 0x0d, 0x4c, 0x1d, 0x80, 0x00, 0x46, 0x00, 0x42, 0x00, 0x46, 0x00,
    0xd0, 0x07, 0x24, 0xfa, 0x90, 0xe8, 0x81, 0x00, 0x00, 0x00, 0x24, 0xfa, 0xb4, 0xe2, 0x41, 0x00,
    0xd0, 0x07, 0x24, 0xfa, 0x90, 0xe8, 0x80, 0x00, 0x2a, 0x00, 0x42, 0x00, 0x2a, 0x00, 0x48, 0xf4,
    0x0c, 0xfe, 0x6c, 0xee, 0x82, 0x00, 0x32, 0x00, 0x00, 0x00, 0x0c, 0xfe, 0xc4, 0x09, 0x42, 0x00,
    0x32, 0x00, 0x6c, 0xee, 0xc8, 0x00, 0xd4, 0x30, 0x81, 0x00, 0x00, 0x00, 0xe8, 0x03, 0x00, 0x00,
    0x41, 0x00, 0xd0, 0x8a, 0x30, 0xf8, 0x10, 0x27, 0x80, 0x00, 0x28, 0x00, 0x40, 0x00, 0x28, 0x00,
    0x81, 0x00, 0x00, 0x00, 0xe0, 0xb1, 0x48, 0xf4, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0x00, 0x28, 0x00, 0x40, 0x00, 0x28, 0x00, 0x81, 0x00, 0x00, 0x00, 0x18, 0xfc, 0x00, 0x00,
    0x41, 0x00, 0x38, 0xcd, 0xc0, 0xe0, 0x68, 0xc5, 0x80, 0x00, 0x14, 0x00, 0x40, 0x00, 0x14, 0x00,
    0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// GLOBAL: FF7 0x007f9430
unsigned char g_BattleSummonNeoBahamutCameraScript2[352] = {
    0x89, 0x00, 0x04, 0x00, 0x49, 0x00, 0x04, 0x00, 0x81, 0x00, 0x00, 0x00, 0xe8, 0x03, 0x00, 0x00,
    0x41, 0x00, 0xf0, 0xd8, 0x78, 0xec, 0xf0, 0xd8, 0x80, 0x00, 0x14, 0x00, 0x40, 0x00, 0x14, 0x00,
    0x82, 0x00, 0x19, 0x00, 0x00, 0x00, 0x30, 0xf8, 0x00, 0x00, 0x40, 0x00, 0x19, 0x00, 0x81, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0xc0, 0xe0, 0xd0, 0x8a, 0xf0, 0xd8, 0x80, 0x00,
    0x14, 0x00, 0x40, 0x00, 0x14, 0x00, 0x08, 0x00, 0x60, 0x00, 0x81, 0x00, 0x00, 0x00, 0xf0, 0xd8,
    0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x13, 0x82, 0x00, 0x64, 0x00, 0x00, 0x00,
    0x68, 0xc5, 0x00, 0x00, 0x40, 0x00, 0x64, 0x00, 0x08, 0x00, 0x00, 0x01, 0x81, 0x00, 0x88, 0x13,
    0x40, 0x1f, 0x00, 0x00, 0x82, 0x00, 0x37, 0x00, 0xe8, 0x03, 0x18, 0xfc, 0x30, 0xf8, 0x41, 0x00,
    0x48, 0xf4, 0x48, 0xf4, 0xb8, 0x0b, 0x42, 0x00, 0x37, 0x00, 0x78, 0xec, 0xf0, 0xd8, 0x90, 0xe8,
    0x08, 0x00, 0x00, 0x02, 0x81, 0x00, 0x00, 0x00, 0xd0, 0x07, 0xd0, 0x8a, 0x82, 0x00, 0x5d, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x78, 0xec, 0x41, 0x00, 0x88, 0x13, 0x18, 0xfc, 0x00, 0x00, 0x42, 0x00,
    0x5d, 0x00, 0x68, 0xc5, 0xc0, 0xe0, 0xa8, 0x61, 0x81, 0x00, 0x00, 0x00, 0x24, 0xfa, 0xb4, 0xe2,
    0x41, 0x00, 0x98, 0x3a, 0xac, 0x0d, 0x4c, 0x1d, 0x80, 0x00, 0x46, 0x00, 0x42, 0x00, 0x46, 0x00,
    0xd0, 0x07, 0x24, 0xfa, 0x90, 0xe8, 0x81, 0x00, 0x00, 0x00, 0x24, 0xfa, 0xb4, 0xe2, 0x41, 0x00,
    0xd0, 0x07, 0x24, 0xfa, 0x90, 0xe8, 0x80, 0x00, 0x2a, 0x00, 0x42, 0x00, 0x2a, 0x00, 0x48, 0xf4,
    0x0c, 0xfe, 0x6c, 0xee, 0x82, 0x00, 0x32, 0x00, 0x00, 0x00, 0x0c, 0xfe, 0xc4, 0x09, 0x42, 0x00,
    0x32, 0x00, 0x6c, 0xee, 0xc8, 0x00, 0xd4, 0x30, 0x81, 0x00, 0x00, 0x00, 0xe8, 0x03, 0x00, 0x00,
    0x41, 0x00, 0xd0, 0x8a, 0x30, 0xf8, 0x10, 0x27, 0x80, 0x00, 0x28, 0x00, 0x40, 0x00, 0x28, 0x00,
    0x81, 0x00, 0x00, 0x00, 0xe0, 0xb1, 0x48, 0xf4, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0x00, 0x28, 0x00, 0x40, 0x00, 0x28, 0x00, 0x81, 0x00, 0x00, 0x00, 0x18, 0xfc, 0x00, 0x00,
    0x41, 0x00, 0x38, 0xcd, 0xc0, 0xe0, 0x68, 0xc5, 0x80, 0x00, 0x14, 0x00, 0x40, 0x00, 0x14, 0x00,
    0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// GLOBAL: FF7 0x007f9590
unsigned char g_BattleSummonNeoBahamutStageOffscreenParam[32] = {0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
/* The end of the stage scale matrix that starts eight bytes before it (m[1][1] to m[2][2] and the
   translation), then the offscreen screen rectangle and the stage ground mesh. */
#pragma pack(push, 2)
typedef struct {
    short m[5];
    int t[3];
    short pad_16;
    BattleFxScreenRect rect;
    BattleFxProceduralMesh mesh;
} BattleSummonNeoBahamutStageRequest;
#pragma pack(pop)
// GLOBAL: FF7 0x007f95b0
BattleSummonNeoBahamutStageRequest g_BattleSummonNeoBahamutStageOffscreenRequest = {{0x1000, 0, 0, 0, 0x1000}, {0, 0, 0}, 0, {0x380, 0, 0x80, 0x100}, {(int)&g_BattleSummonNeoBahamutPolygonVertexBlob, 0x80, {0, 0}, 0, 0x20}};

// GLOBAL: FF7 0x007f95e0
struct BattleLimitStardustRayModelReq2 g_NeoBahamutThunderSpriteDrawRequest1 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
// GLOBAL: FF7 0x007f95f0
struct BattleLimitStardustRayModelReq2 g_NeoBahamutThunderSpriteDrawRequest2 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
/* The dust sprite request: animation table, 3D scene object, colour and frame. */
typedef struct {
    int animTable;
    int sceneField;
    int color;
    short frame;
    short uvPacked;
} BattleFxNeoBahamutDustSprite;
// GLOBAL: FF7 0x007f9600
BattleFxNeoBahamutDustSprite g_BattleSummonNeoBahamutDustSpriteDrawRequest = {0, 0, 0x2e808080, 0, 0x200};
// GLOBAL: FF7 0x007f9610
BattleFxNeoBahamutModelReq g_BattleNeoBahamutParticleDrawRequest = {0, 128, {0}, 0, {0x20, 0, 0, 0}};
/* The head of a model draw request: model group, flags, fade level and state flags. */
typedef struct {
    int modelGroup;
    int flags;
    short uvScroll;
    short fadeLevel;
    short stateFlags;
    short texturePageBits;
} BattleSummonNeoBahamutModelHead;
// GLOBAL: FF7 0x007f9620
BattleSummonNeoBahamutModelHead g_BattleSummonNeoBahamutCloud1ModelDrawRequest = {0, 0x28, 0, 0, 0x20, 0};
// GLOBAL: FF7 0x007f9630
unsigned char g_BattleSummonNeoBahamutCloud1Matrix[32] = {0x00, 0x68, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xb1, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
/* The two cloud layers' model draw requests; the second layer's request continues past this object. */
typedef struct {
    BattleSummonNeoBahamutModelHead layer1;
    int layer2ModelGroup;
    int layer2Flags;
    short layer2UvScroll;
    short layer2FadeLevel;
} BattleSummonNeoBahamutCloud2Draw;
// GLOBAL: FF7 0x007f9650
BattleSummonNeoBahamutCloud2Draw g_BattleSummonNeoBahamutCloud2ModelDrawRequest = {{0, 0x80, 0, 0, 0x20, 0}, 0, 0x88, 0, 0};

unsigned char g_rawData_007f966c[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007f9670
unsigned char g_BattleSummonNeoBahamutCloud2Prim[8] = {0x00, 0x00, 0x00, 0x03, 0x00, 0x40, 0x80, 0x60};
// GLOBAL: FF7 0x007f9678
struct BattleFxCloud2VertexData g_BattleSummonNeoBahamutCloud2VertexData = {0xa0, 0, 320, 332, 0};

unsigned char g_rawData_007f9684[4] = {0xa8, 0x00, 0x00, 0x00};
unsigned char g_rawData_007f9688[24] = {0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007f96a0
struct BattleFxSkyMaskVertexData g_BattleSummonNeoBahamutSkyMaskVertexData1 = {3840, {0, 0, 0, 0, 0, 0, 0, 0}, -2000, {0}, 3840};

unsigned char g_rawData_007f96b0[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8e, 0xfe, 0xff, 0xff, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007f96c0
struct BattleFxNeoBahamutSkyMaskVertex g_BattleSummonNeoBahamutSkyMaskVertexData2 = {3840, {0, 0, 0, 0, 0, 0, 0, 0}, -2000, {0}, 3840};

// GLOBAL: FF7 0x007f96d0
struct BattleSummonNeoBahamutSkyMask g_BattleSummonNeoBahamutSkyMaskWorkingBlob = {{0, 0, 0, 0, 0, 0, 0x8e, 0xfe, 0xff, 0xff, 0, 3, 0, 0, 0, 0, 1, 0xf, 0, 0, 0, 0, 0, 0, 0, 0, 0x2f, 0xf8, 0, 0, 1, 0xf, 0, 0, 0, 0, 0, 0, 0x7f, 0xfe, 0xff, 0xff, 0x64, 3, 0, 0, 0, 0, 2, 0xf, 0, 0, 0, 0, 0, 0, 0, 0, 0x2e, 0xf8, 0, 0, 2, 0xf, 0, 0, 0, 0, 0, 0, 0x84, 0xfe, 0xff, 0xff, 0, 3, 0, 0, 0, 0, 3, 0xf, 0, 0, 0, 0, 0, 0, 0, 0, 0x2d, 0xf8, 0, 0, 3, 0xf, 0, 0, 0, 0, 0, 0, 0x89, 0xfe, 0xff, 0xff, 0, 3, 0, 0, 0, 0}, 0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}, 0, {0x80, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}, 0};

unsigned char g_rawData_007f9764[4] = {0x88, 0x00, 0x00, 0x00};
unsigned char g_rawData_007f9768[8] = {0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007f9770
int g_BattleSummonNeoBahamutCloudSeaMatrix[2] = {16384, 0};
unsigned char g_rawData_007f9778[24] = {0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x88, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007f9790
int g_BattleSummonNeoBahamutBackSkyShadeColor1 = 0x00802000;
// GLOBAL: FF7 0x007f9794
int g_BattleSummonNeoBahamutBackSkyShadeColor2 = 0x00FFFFFF;
// GLOBAL: FF7 0x007f9798
unsigned char g_BattleSummonNeoBahamutBackSkyVertex1[8] = {0x00, 0x00, 0x94, 0x05, 0x28, 0x23, 0x00, 0x00};
// GLOBAL: FF7 0x007f97a0
struct SVECTOR g_BattleSummonNeoBahamutBackSkyVertex2 = {0, -72, 0x2328, 0};
// GLOBAL: FF7 0x007f97a8
unsigned char g_BattleSummonNeoBahamutBackSkyVertex3[16] = {0x00, 0x00, 0x44, 0x0a, 0x28, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// GLOBAL: FF7 0x007f97b8
struct CVECTOR g_BattleSummonNeoBahamutBallSpriteColor1[2] = {{0x80, 0x80, 0x80, 0x2e}, {0}};
// GLOBAL: FF7 0x007f97c0
struct CVECTOR g_BattleSummonNeoBahamutBallSpriteColor2[3] = {{0x80, 0x80, 0x80, 0x2e}, {0}, {0}};

unsigned char g_rawData_007f97cc[4] = {0xa8, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007f97d0
unsigned char g_BattleSummonNeoBahamutLineLight1WorkingBlob[16] = {0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// GLOBAL: FF7 0x007f97e0
struct SVECTOR g_BattleSummonNeoBahamutLineLight1Param1 = {-50, 400, 0, 0};
// GLOBAL: FF7 0x007f97e8
struct BattleFxNeoBahamutParam g_BattleSummonNeoBahamutLineLight1Param2 = {{0x32, 0, 0x90, 1, 0, 0, 0, 0}, 0, 136, {0}, 0, {0x20, 0, 0, 0}, 0};

unsigned char g_rawData_007f9804[4] = {0xa8, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007f9808
struct BattleFxNeoBahamutRockRequest g_BattleSummonNeoBahamutLineLight2WorkingBlob = {{0, 0, 0, 0, 0x20, 0, 0, 0}, 0, {0x80, 0, 0, 0, 0, 0}, 0};

unsigned char g_rawData_007f981c[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007f9820
struct BattleSummonNeoBahamutBreathParam g_BattleSummonNeoBahamutBreathChildModelParam = {{0x80, 0xbd, 0xaa, 0, 0, 0, 0, 0}, 0, {0xa8, 0, 0, 0, 0, 0}, 0};

unsigned char g_rawData_007f9834[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007f9838
int g_BattleSummonNeoBahamutBillboardWorkingBlob[3] = {-1310659464, 0, 0};

unsigned char g_rawData_007f9844[4] = {0x80, 0x00, 0x00, 0x00};
unsigned char g_rawData_007f9848[8] = {0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007f9850
struct BattleFxNeoBahamutSceneActor *g_BattleSummonNeoBahamutSceneActorRef = (struct BattleFxNeoBahamutSceneActor *)&g_BattleActorRecords[3].modelId;
unsigned char g_BattleFxSummonNeoBahamutAssetPrefix[4] = {0x42, 0x52, 0x45, 0x53};
unsigned char g_BattleFxSummonNeoBahamutResourceNames[276] = {
0x53, 0x5f, 0x31, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x46, 0x4c, 0x41, 0x53, 0x48, 0x5f, 0x31, 0x2e,
0x52, 0x53, 0x44, 0x00, 0x46, 0x4c, 0x41, 0x53, 0x48, 0x5f, 0x32, 0x2e, 0x52, 0x53, 0x44, 0x00,
0x4c, 0x49, 0x4e, 0x45, 0x5f, 0x31, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x4d, 0x41, 0x53, 0x4b,
0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x00, 0x00, 0x52, 0x4f, 0x43, 0x4b, 0x5f, 0x31, 0x2e, 0x52,
0x53, 0x44, 0x00, 0x00, 0x52, 0x4f, 0x43, 0x4b, 0x5f, 0x31, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00,
0x52, 0x4f, 0x43, 0x4b, 0x5f, 0x32, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x52, 0x4f, 0x43, 0x4b,
0x5f, 0x32, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x52, 0x4f, 0x43, 0x4b, 0x5f, 0x33, 0x2e, 0x52,
0x53, 0x44, 0x00, 0x00, 0x52, 0x4f, 0x43, 0x4b, 0x5f, 0x33, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00,
0x53, 0x4b, 0x59, 0x5f, 0x41, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x00, 0x53, 0x4b, 0x59, 0x5f,
0x41, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x00, 0x53, 0x4b, 0x59, 0x5f, 0x42, 0x2e, 0x52, 0x53,
0x44, 0x00, 0x00, 0x00, 0x53, 0x4b, 0x59, 0x5f, 0x43, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x00,
0x53, 0x4f, 0x4e, 0x49, 0x43, 0x5f, 0x31, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x56, 0x41, 0x48, 0x41,
0x32, 0x5f, 0x31, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x45, 0x4c, 0x45, 0x43, 0x5f, 0x31, 0x2e, 0x53,
0x50, 0x54, 0x00, 0x00, 0x56, 0x41, 0x48, 0x41, 0x32, 0x5f, 0x30, 0x30, 0x2e, 0x54, 0x49, 0x4d,
0x00, 0x00, 0x00, 0x00, 0x54, 0x41, 0x4d, 0x41, 0x31, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00,
0x53, 0x4d, 0x4f, 0x4b, 0x45, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x56, 0x41, 0x48, 0x41,
0x32, 0x5f, 0x4e, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x45, 0x4c, 0x45, 0x43, 0x5f, 0x31, 0x2e, 0x53,
0x50, 0x54, 0x00, 0x00};
// GLOBAL: FF7 0x00aa6730
BattleSummonCloudSlot *g_BattleSummonNeoBahamutCloud2TaskSlot = 0;
// GLOBAL: FF7 0x00aa6734
struct Vec3sPacked *g_BattleSummonNeoBahamutActorRiseAndFallTaskSlot = 0;
// GLOBAL: FF7 0x00aa6738
PsxMatrix g_BattleSummonNeoBahamutWorldMatrix = {0};
// GLOBAL: FF7 0x00aa6758
char g_BattleSummonNeoBahamutFragmentUpdateInitFlag = 0;
// GLOBAL: FF7 0x00aa675c
BattleFxDebrisSpawnTask *g_BattleSummonNeoBahamutRock2SpawnTaskSlot = 0;
// GLOBAL: FF7 0x00aa6760
char g_BattleSummonNeoBahamutRock3InitFlag = 0;
// GLOBAL: FF7 0x00aa6764
BattleFxNeoBahamutBackSkyState *g_BattleSummonNeoBahamutBackSkyTaskSlot = 0;
// GLOBAL: FF7 0x00aa6768
int g_BattleSummonNeoBahamutLineLight2State = 0;
unsigned char g_bssPad_00aa676c[4] = {0};
// GLOBAL: FF7 0x00aa6770
PsxMatrix g_BattleSummonNeoBahamutComposedMatrix = {0};
/* The breath child task's scratch; the camera-script operand pool starts at operandBase. */
typedef struct {
    BattleFxNeoBahamutBreathScratch *scratch;
    short unused4;
    short operandBase;
} BattleFxNeoBahamutBreathScratchSlot;
// GLOBAL: FF7 0x00aa6790
BattleFxNeoBahamutBreathScratchSlot g_BattleSummonNeoBahamutBreathScratch = {0};
unsigned char g_bssPad_00aa6798[24] = {0};
// GLOBAL: FF7 0x00aa67b0
PsxMatrix g_BattleSummonNeoBahamutBillboardMatrix = {0};
unsigned char g_bssPad_00aa67d0[8] = {0};
// GLOBAL: FF7 0x00aa67d8
short g_BattleSummonNeoBahamutLineLight2Rotation[4] = {0};
// GLOBAL: FF7 0x00aa67e0
short g_BattleSummonNeoBahamutLineLight2Offset[4] = {0};
// GLOBAL: FF7 0x00aa67e8
short g_BattleSummonNeoBahamutLineLight2Step[7] = {0};
// GLOBAL: FF7 0x00aa67f8
BattleFxNeoBahamutCloud1Slot *g_BattleSummonNeoBahamutCloud1TaskSlot = 0;
unsigned char g_bssPad_00aa67fc[4] = {0};
// GLOBAL: FF7 0x00aa6800
struct BattleSummonLightRigAngle g_BattleSummonNeoBahamutLightRigAngle = {0};
// GLOBAL: FF7 0x00aa6808
BattleFxShortOffsetVec *g_BattleSummonNeoBahamutRock1SpawnTaskSlot = 0;
unsigned char g_bssPad_00aa680c[4] = {0};
// GLOBAL: FF7 0x00aa6810
BattleFxNeoBahamutShardMatrixBuf g_BattleSummonNeoBahamutGroundVertexBlob[651] = {0};
unsigned char g_bssPad_00aa9aec[4] = {0};
// GLOBAL: FF7 0x00aa9af0
int g_BattleSummonNeoBahamutRotationMatrix = 0;
unsigned char g_bssPad_00aa9af4[4] = {0};
unsigned char g_bssPad_00aa9af8[8] = {0};
/* The end of the rotation matrix that starts at g_BattleSummonNeoBahamutRotationMatrix: m[2][2]
   and the translation, which the summon sets to the anchor position. */
#pragma pack(push, 2)
typedef struct {
    short m22;
    int t[3];
    short pad_0E;
} BattleSummonNeoBahamutMatrixTail;
#pragma pack(pop)
// GLOBAL: FF7 0x00aa9b00
BattleSummonNeoBahamutMatrixTail g_BattleSummonNeoBahamutAnchorWorkingBlob = {0};
// GLOBAL: FF7 0x00aa9b10
short g_BattleSummonNeoBahamutAngleVector[3] = {0};
// g_BattleSummonNeoBahamutCloudModelBuffer, g_BattleSummonNeoBahamutCloudModelBuffer + 0x100 are
// indexed by the code as fields of records. The span runs to the next named definition of the TU;
// the TU's pieces inside 0xaa9b18..0xaabd18 are aliases.
// GLOBAL: FF7 0x00aa9b18
unsigned char g_BattleSummonNeoBahamutCloudModelBuffer[0x2200] = {0};

// GLOBAL: FF7 0x00aabd18
BattleFxNeoBahamutRock2State *g_BattleSummonNeoBahamutRock2TaskSlot = 0;
// GLOBAL: FF7 0x00aabd1c
BattleFxNeoBahamutCloudSea *g_BattleSummonNeoBahamutCloudSeaTaskSlot = 0;
// GLOBAL: FF7 0x00aabd20
BattleFxNeoBahamutLineLight1Slot *g_BattleSummonNeoBahamutLineLight1TaskSlot = 0;
// GLOBAL: FF7 0x00aabd24
char g_BattleSummonNeoBahamutRock2InitFlag = 0;
// GLOBAL: FF7 0x00aabd28
CameraScriptTrack g_BattleSummonNeoBahamutCameraFocus = {0};
// GLOBAL: FF7 0x00aabd54
int g_BattleSummonNeoBahamutGroundTaskSlot = 0;
// GLOBAL: FF7 0x00aabd58
unsigned char g_BattleSummonNeoBahamutLightRigInitFlag = 0;
// GLOBAL: FF7 0x00aabd5c
unsigned char g_BattleSummonNeoBahamutLineLight2InitFlag[4] = {0};
// GLOBAL: FF7 0x00aabd60
PsxMatrix g_BattleSummonNeoBahamutLineLightMatrix = {0};
unsigned char g_bssPad_00aabd80[800] = {0};
// GLOBAL: FF7 0x00aac0a0
unsigned char g_BattleSummonNeoBahamutRock2SpawnInitFlag[4] = {0};
// GLOBAL: FF7 0x00aac0a4
char g_BattleSummonNeoBahamutRock1InitFlag = 0;
// GLOBAL: FF7 0x00aac0a8
int g_BattleSummonNeoBahamutTargetMask = 0;
unsigned char g_bssPad_00aac0ac[4] = {0};
// GLOBAL: FF7 0x00aac0b0
struct BattleFxNeoBahamutSkyPrim g_BattleSummonNeoBahamutBackSkyPrim1 = {0};
unsigned char g_bssPad_00aac0d4[4] = {0};
// GLOBAL: FF7 0x00aac0d8
struct BattleFxNeoBahamutSkyPrim g_BattleSummonNeoBahamutBackSkyPrim2 = {0};
unsigned char g_bssPad_00aac0fc[4] = {0};
// GLOBAL: FF7 0x00aac100
PsxMatrix g_BattleSummonNeoBahamutGroundMatrix = {0};
// GLOBAL: FF7 0x00aac120
BattleFxShardMeshRotation *g_BattleSummonNeoBahamutRock3TaskSlot = 0;
// GLOBAL: FF7 0x00aac124
char g_BattleSummonNeoBahamutBackSkyInitFlag = 0;
// GLOBAL: FF7 0x00aac128
char g_BattleSummonNeoBahamutBreathChildInitFlag = 0;
// GLOBAL: FF7 0x00aac12c
char g_BattleSummonNeoBahamutCloudSeaInitFlag = 0;
// GLOBAL: FF7 0x00aac130
struct BattleFxNeoBahamutFaceSlot *g_BattleSummonNeoBahamutPsxMeshTaskSlot = 0;
// GLOBAL: FF7 0x00aac134
char g_BattleSummonNeoBahamutGroundInitFlag = 0;
// GLOBAL: FF7 0x00aac138
char g_BattleSummonNeoBahamutCloud2InitFlag = 0;
// GLOBAL: FF7 0x00aac13c
char g_BattleSummonNeoBahamutRock1SpawnInitFlag = 0;
// GLOBAL: FF7 0x00aac140
CameraScriptTrack * g_pBattleSummonNeoBahamutCameraTrack = 0;
// GLOBAL: FF7 0x00aac144
BattleFxNeoBahamutLineLight3Slot *g_BattleSummonNeoBahamutLineLight3TaskSlot = 0;
// GLOBAL: FF7 0x00aac148
BattleFxNeoBahamutFragment g_BattleSummonNeoBahamutPsxMeshScratch[434] = {0};
// GLOBAL: FF7 0x00aaf0c0
struct BattleFxNeoBahamutLightRig *g_BattleSummonNeoBahamutLightRigTaskSlot = 0;
// GLOBAL: FF7 0x00aaf0c4
PsxMatrix *g_BattleSummonNeoBahamutFragmentMatrixTaskSlot = 0;
// GLOBAL: FF7 0x00aaf0c8
CameraScriptTrack * g_pCameraScriptOtherTrack = 0;
// GLOBAL: FF7 0x00aaf0cc
BattleFxShardMeshRotation *g_BattleSummonNeoBahamutRock1TaskSlot = 0;
// GLOBAL: FF7 0x00aaf0d0
BattleFxLineLight2Task *g_BattleSummonNeoBahamutLineLight2TaskSlot = 0;
// GLOBAL: FF7 0x00aaf0d4
char g_BattleSummonNeoBahamutActorRiseAndFallInitFlag = 0;
// GLOBAL: FF7 0x00aaf0d8
BattleFxNeoBahamutLine3Blob g_BattleSummonNeoBahamutLineLight3WorkingBlob = {0};
// GLOBAL: FF7 0x00aaf100
CameraScriptTrack g_BattleSummonNeoBahamutCameraEye = {0};
unsigned char g_bssPad_00aaf12c[4] = {0};
// GLOBAL: FF7 0x00aaf130
BattleFxNeoBahamutBallBlob g_BattleSummonNeoBahamutBallSpriteWorkingBlob = {0};
// GLOBAL: FF7 0x00aaf158
int g_BattleSummonNeoBahamutCircleModelAngle = 0;
unsigned char g_bssPad_00aaf15c[4] = {0};
// GLOBAL: FF7 0x00aaf160
struct BattleFxNeoBahamutSkyTile g_BattleSummonNeoBahamutBackSkyPrim3 = {0};
#define g_BattleSummonNeoBahamutBackSkyPrims1 (&g_BattleSummonNeoBahamutBackSkyPrim1)
#define g_BattleSummonNeoBahamutBackSkyPrims2 (&g_BattleSummonNeoBahamutBackSkyPrim2)
#define g_BattleSummonNeoBahamutBackSkyPrims3 (&g_BattleSummonNeoBahamutBackSkyPrim3)
// g_BattleSummonNeoBahamutActorMaskFlags is indexed by the code as an array (read by
// BattleFx_SummonNeoBahamutToggleActorMaskFlags). The span runs to the next named definition of the
// TU; the TU's pieces inside 0xaaf170..0xaaf17c are aliases.
// GLOBAL: FF7 0x00aaf170
unsigned char g_BattleSummonNeoBahamutActorMaskFlags[12] = {0};
// GLOBAL: FF7 0x00aaf17c
char g_BattleSummonNeoBahamutPsxMeshInitFlag = 0;
// GLOBAL: FF7 0x00aaf180
char g_BattleSummonNeoBahamutCloud1InitFlag = 0;
// GLOBAL: FF7 0x00aaf184
char g_BattleSummonNeoBahamutLineLight1InitFlag = 0;
// GLOBAL: FF7 0x00aaf188
struct GfxTableCursor *g_BattleSummonNeoBahamutLoadedResTable = 0;
// GLOBAL: FF7 0x00aaf18c
int g_BattleSummonNeoBahamutCloud2ModelDrawHandle = 0;
// GLOBAL: FF7 0x00aaf190
int g_BattleSummonNeoBahamutBackSkyPrimIndex = 0;
unsigned char g_bssPad_00aaf194[4] = {0};
// clang-format on

// Once per battle scene, loads the SUMMON/VAHAMUT2 model table described by
// g_BattleSummonNeoBahamutResourceDef into g_BattleSummonNeoBahamutLoadedResTable and registers the
// summon's sound groups.
// FUNCTION: FF7 0x0048c200
void __cdecl BattleFx_SummonLoadResNeoBahamut(void)
{
    GfxModelTableBank *sceneContext;
    GfxResourceRequest record;
    int id;
    char pathBuffer[200];
    GfxResourceRequest *request;
    int defPtr;

    if (g_pBattle3dScene != 0 && g_BattleSummonNeoBahamutLoadedResTable == 0) {
        id = 0x14;
        defPtr = (int)&g_BattleSummonNeoBahamutResourceDef;
        sceneContext = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &record;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x007f996c
        request->resourcePath = BattleFx_BuildResourcePath("SUMMON/VAHAMUT2", pathBuffer);
        Gfx_LoadModelTableIfAbsent(id, defPtr, request, &g_BattleSummonNeoBahamutLoadedResTable, sceneContext);
        SfxRegisterSoundGroups((struct SoundGroup **)&g_BattleSummonNeoBahamutSoundTable);
    }
}

// Initializes retained Neo Bahamut state: clears three rotation shorts and three unaligned
// translation dwords, writes position triples (0,0xD8F0,0xD8F0) and (0,0xFA24,0xE2B4), and clears
// two control values. Zeroes four thirty-byte blocks and one 38-byte block, setting
// g_BattleSummonNeoBahamutGroundMatrix diagonal shorts to 4096 and
// g_BattleSummonNeoBahamutLineLight2Rotation diagonal fields to 0xFB00/0x500. Initializes a
// 0x03000000 primitive with bytes (0,0x20,0x80,0x60) and the four retained viewport shorts. Builds
// two 0x08000000/0x3A primitives at g_BattleSummonNeoBahamutBackSkyPrim1 and
// g_BattleSummonNeoBahamutBackSkyPrim2 with opposite black/white endpoint colour arrangements and
// repeated top-row X coordinates at viewport origin and origin plus width, all Y from viewport
// origin. Calls BattleFx_SummonLoadResNeoBahamut, then BattleFx_NeoBahamutBeginEffect with both
// arguments and returns zero.
// FUNCTION: FF7 0x0048c2a1
int __cdecl BattleFx_SummonEnterNeoBahamut(int targetMask, int actorIdx)
{
    g_BattleSummonNeoBahamutAngleVector[2] = 0;
    g_BattleSummonNeoBahamutAngleVector[1] = g_BattleSummonNeoBahamutAngleVector[2];
    g_BattleSummonNeoBahamutAngleVector[0] = g_BattleSummonNeoBahamutAngleVector[1];
    g_BattleSummonNeoBahamutWorldMatrix.t[2] = 0;
    g_BattleSummonNeoBahamutWorldMatrix.t[1] = g_BattleSummonNeoBahamutWorldMatrix.t[2];
    g_BattleSummonNeoBahamutWorldMatrix.t[0] = g_BattleSummonNeoBahamutWorldMatrix.t[1];
    g_BattleSummonNeoBahamutLightRigAngle.vectorX = 0;
    g_BattleSummonNeoBahamutLightRigAngle.angleX = 0xD8F0;
    g_BattleSummonNeoBahamutLightRigAngle.angleY = 0xD8F0;
    g_BattleSummonNeoBahamutAnchorPosition[0] = 0;
    g_BattleSummonNeoBahamutAnchorPosition[1] = -1500;
    g_BattleSummonNeoBahamutAnchorPosition[2] = -7500;
    g_BattleSummonNeoBahamutCircleModelAngle = 0;
    g_BattleSummonNeoBahamutLineLight2State = 0;
    memset(&g_BattleSummonNeoBahamutGroundMatrix, 0, sizeof(g_BattleSummonNeoBahamutGroundMatrix));
    g_BattleSummonNeoBahamutGroundMatrix.m[2][2] = 0x1000;
    g_BattleSummonNeoBahamutGroundMatrix.m[1][1] = g_BattleSummonNeoBahamutGroundMatrix.m[2][2];
    g_BattleSummonNeoBahamutGroundMatrix.m[0][0] = g_BattleSummonNeoBahamutGroundMatrix.m[1][1];
    memset(g_BattleSummonNeoBahamutLineLight2Rotation, 0, 30);
    g_BattleSummonNeoBahamutLineLight2Rotation[0] = 0xFB00 /* 5647.5 deg */;
    g_BattleSummonNeoBahamutLineLight2Offset[0] = 0x500;
    memset(&g_BattleSummonNeoBahamutBillboardMatrix, 0, sizeof(g_BattleSummonNeoBahamutBillboardMatrix));
    memset(&g_BattleSummonNeoBahamutLineLightMatrix, 0, sizeof(g_BattleSummonNeoBahamutLineLightMatrix));
    memset(&g_BattleSummonNeoBahamutBallSpriteWorkingBlob, 0, sizeof(g_BattleSummonNeoBahamutBallSpriteWorkingBlob));
    g_BattleSummonNeoBahamutBackSkyPrim3.tag = 0x3000000;
    g_BattleSummonNeoBahamutBackSkyPrim3.colorR = 0;
    g_BattleSummonNeoBahamutBackSkyPrim3.colorG = 0x20;
    g_BattleSummonNeoBahamutBackSkyPrim3.colorB = 0x80;
    g_BattleSummonNeoBahamutBackSkyPrim3.code = PSX_PRIM_TILE;
    g_BattleSummonNeoBahamutBackSkyPrim3.viewportX = (short)g_BattleViewportOriginX;
    g_BattleSummonNeoBahamutBackSkyPrim3.viewportY = (short)g_BattleViewportOriginY;
    g_BattleSummonNeoBahamutBackSkyPrim3.viewportWidth = (short)g_BattleViewportWidth;
    g_BattleSummonNeoBahamutBackSkyPrim3.viewportHeight = (short)g_BattleSceneViewportHeight;
    g_BattleSummonNeoBahamutBackSkyPrim1.tag = 0x8000000;
    g_BattleSummonNeoBahamutBackSkyPrim1.color0R = 0;
    g_BattleSummonNeoBahamutBackSkyPrim1.color0G = 0;
    g_BattleSummonNeoBahamutBackSkyPrim1.color0B = 0;
    g_BattleSummonNeoBahamutBackSkyPrim1.code = PSX_PRIM_POLY_G4_TRANS;
    g_BattleSummonNeoBahamutBackSkyPrim1.vertex0X = (short)g_BattleViewportOriginX;
    g_BattleSummonNeoBahamutBackSkyPrim1.vertex0Y = (short)g_BattleViewportOriginY;
    g_BattleSummonNeoBahamutBackSkyPrim1.color1R = 0;
    g_BattleSummonNeoBahamutBackSkyPrim1.color1G = 0;
    g_BattleSummonNeoBahamutBackSkyPrim1.color1B = 0;
    g_BattleSummonNeoBahamutBackSkyPrim1.unused1 = 0;
    g_BattleSummonNeoBahamutBackSkyPrim1.vertex1X = g_BattleViewportOriginX + g_BattleViewportWidth;
    g_BattleSummonNeoBahamutBackSkyPrim1.vertex1Y = (short)g_BattleViewportOriginY;
    g_BattleSummonNeoBahamutBackSkyPrim1.color2R = 0xFF;
    g_BattleSummonNeoBahamutBackSkyPrim1.color2G = 0xFF;
    g_BattleSummonNeoBahamutBackSkyPrim1.color2B = 0xFF;
    g_BattleSummonNeoBahamutBackSkyPrim1.unused2 = 0;
    g_BattleSummonNeoBahamutBackSkyPrim1.vertex2X = (short)g_BattleViewportOriginX;
    g_BattleSummonNeoBahamutBackSkyPrim1.vertex2Y = (short)g_BattleViewportOriginY;
    g_BattleSummonNeoBahamutBackSkyPrim1.color3R = 0xFF;
    g_BattleSummonNeoBahamutBackSkyPrim1.color3G = 0xFF;
    g_BattleSummonNeoBahamutBackSkyPrim1.color3B = 0xFF;
    g_BattleSummonNeoBahamutBackSkyPrim1.unused3 = 0;
    g_BattleSummonNeoBahamutBackSkyPrim1.vertex3X = g_BattleViewportOriginX + g_BattleViewportWidth;
    g_BattleSummonNeoBahamutBackSkyPrim1.vertex3Y = (short)g_BattleViewportOriginY;
    g_BattleSummonNeoBahamutBackSkyPrim2.tag = 0x8000000;
    g_BattleSummonNeoBahamutBackSkyPrim2.color0R = 0xFF;
    g_BattleSummonNeoBahamutBackSkyPrim2.color0G = 0xFF;
    g_BattleSummonNeoBahamutBackSkyPrim2.color0B = 0xFF;
    g_BattleSummonNeoBahamutBackSkyPrim2.code = PSX_PRIM_POLY_G4_TRANS;
    g_BattleSummonNeoBahamutBackSkyPrim2.vertex0X = (short)g_BattleViewportOriginX;
    g_BattleSummonNeoBahamutBackSkyPrim2.vertex0Y = (short)g_BattleViewportOriginY;
    g_BattleSummonNeoBahamutBackSkyPrim2.color1R = 0xFF;
    g_BattleSummonNeoBahamutBackSkyPrim2.color1G = 0xFF;
    g_BattleSummonNeoBahamutBackSkyPrim2.color1B = 0xFF;
    g_BattleSummonNeoBahamutBackSkyPrim2.unused1 = 0;
    g_BattleSummonNeoBahamutBackSkyPrim2.vertex1X = g_BattleViewportOriginX + g_BattleViewportWidth;
    g_BattleSummonNeoBahamutBackSkyPrim2.vertex1Y = (short)g_BattleViewportOriginY;
    g_BattleSummonNeoBahamutBackSkyPrim2.color2R = 0;
    g_BattleSummonNeoBahamutBackSkyPrim2.color2G = 0;
    g_BattleSummonNeoBahamutBackSkyPrim2.color2B = 0;
    g_BattleSummonNeoBahamutBackSkyPrim2.unused2 = 0;
    g_BattleSummonNeoBahamutBackSkyPrim2.vertex2X = (short)g_BattleViewportOriginX;
    g_BattleSummonNeoBahamutBackSkyPrim2.vertex2Y = (short)g_BattleViewportOriginY;
    g_BattleSummonNeoBahamutBackSkyPrim2.color3R = 0;
    g_BattleSummonNeoBahamutBackSkyPrim2.color3G = 0;
    g_BattleSummonNeoBahamutBackSkyPrim2.color3B = 0;
    g_BattleSummonNeoBahamutBackSkyPrim2.unused3 = 0;
    g_BattleSummonNeoBahamutBackSkyPrim2.vertex3X = g_BattleViewportOriginX + g_BattleViewportWidth;
    g_BattleSummonNeoBahamutBackSkyPrim2.vertex3Y = (short)g_BattleViewportOriginY;
    BattleFx_SummonLoadResNeoBahamut();
    BattleFx_NeoBahamutBeginEffect(targetMask, actorIdx);
    return 0;
}

// Setup for an actor-targeted effect: stores targetMask into g_BattleSummonNeoBahamutTargetMask and
// gets the actor mask centroid into a local short[3]; if its z is below the short at
// g_BattleActorRecords[actorIndex].posZ it sets g_BattleSummonNeoBahamutAngleVector[1] to 0x800.
// Builds the rotation matrix g_BattleSummonNeoBahamutWorldMatrix from g_BattleSummonNeoBahamutAngleVector,
// saves z in its translation, builds a second matrix
// g_BattleSummonNeoBahamutRotationMatrix from angles {-0x15, 0, 0}, copies the
// three shorts of g_BattleSummonNeoBahamutAnchorPosition into the ints of
// g_BattleSummonNeoBahamutAnchorWorkingBlob, allocates a slot running BattleFx_NeoBahamutRunLightRig, calls
// BattleFx_NeoBahamutSpawnCameraScript with either g_BattleSummonNeoBahamutCameraScript1
// or g_BattleSummonNeoBahamutCameraScript2 (chosen by rand() & 0x100)
// plus g_BattleSummonNeoBahamutWorldMatrix and actorIndex, allocates a slot running BattleFx_SummonNeoBahamutTimelineTask
// and sets mask 8 in g_BattleStageHideFlags.
// FUNCTION: FF7 0x0048c60b
void __cdecl BattleFx_NeoBahamutBeginEffect(int targetMask, int actorIdx)
{
    short centroid[3];

    Battle_GetActorMaskCentroid(g_BattleSummonNeoBahamutTargetMask = targetMask, centroid);
    if (centroid[2] < g_BattleActorRecords[actorIdx].posZ) {
        g_BattleSummonNeoBahamutAngleVector[1] = 0x800 /* 180 deg */;
    }
    RotMatrix((SVECTOR *)g_BattleSummonNeoBahamutAngleVector, (int *)&g_BattleSummonNeoBahamutWorldMatrix);
    g_BattleSummonNeoBahamutWorldMatrix.t[2] = centroid[2];
    centroid[0] = -0x15;
    centroid[2] = 0;
    centroid[1] = centroid[2];
    RotMatrix((SVECTOR *)centroid, (int *)&g_BattleSummonNeoBahamutRotationMatrix);
    g_BattleSummonNeoBahamutAnchorWorkingBlob.t[0] = g_BattleSummonNeoBahamutAnchorPosition[0];
    g_BattleSummonNeoBahamutAnchorWorkingBlob.t[1] = g_BattleSummonNeoBahamutAnchorPosition[1];
    g_BattleSummonNeoBahamutAnchorWorkingBlob.t[2] = g_BattleSummonNeoBahamutAnchorPosition[2];
    Battle_AllocateResourceSlot(BattleFx_NeoBahamutRunLightRig);
    if (rand() & 0x100) {
        BattleFx_NeoBahamutSpawnCameraScript(&g_BattleSummonNeoBahamutCameraScript1, (int)&g_BattleSummonNeoBahamutWorldMatrix, actorIdx);
    } else {
        BattleFx_NeoBahamutSpawnCameraScript(&g_BattleSummonNeoBahamutCameraScript2, (int)&g_BattleSummonNeoBahamutWorldMatrix, actorIdx);
    }
    Battle_AllocateResourceSlot(BattleFx_SummonNeoBahamutTimelineTask);
    g_BattleStageHideFlags |= 8;
}

// Allocates a 0x20-byte resource slot for BattleFx_NeoBahamutRunCameraScript and stores the camera
// script, the matrix and the actor index in it. The actor index is stored as a word.
// FUNCTION: FF7 0x0048c71d
void __cdecl BattleFx_NeoBahamutSpawnCameraScript(void *cameraScript, int matrix, int actorIndex)
{
    BattleFxCameraTrackSlot *effect;

    effect = (BattleFxCameraTrackSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_NeoBahamutRunCameraScript);
    effect->cameraScript = (int)cameraScript;
    effect->matrix = matrix;
    effect->actorIndex = actorIndex;
}

// Interprets the Neo Bahamut camera script using eye/focus tracks. First unpaused call clears
// durations, sets actor selectors minus one and marks the task initialized. While either duration
// is zero consumes short commands selected by bit 0x40: duration, absolute XYZ, linear velocity,
// distance-following, acceleration, eased interpolation, actor/bone selection, selector-only
// detachment, suppression duration and global control write. Actor zero falls back to task actor;
// attachment only changes selectors without subtracting position. Unknown commands retire and
// return. Distance setup predicts a linear other track using that other track remaining duration.
// Updates eye before focus, acceleration falling into linear motion, normalized distance
// following and BattleFx_NeoBahamutApplyCameraEasing interpolation, then decrements both durations. Publication continues
// while paused: with task matrix, unattached positions transform directly and attached positions
// rotate offsets then add bone/world position. Eye installs and, after attachment lookup,
// reinstalls the task matrix; focus relies on installed matrix state, including when eye is
// suppressed. Without task matrix copies nonsuppressed position bytes directly.
// FUNCTION: FF7 0x0048c75d
void __cdecl BattleFx_NeoBahamutRunCameraScript(void)
{
    /* VC5 hash order fixes the slots: p=easing, q=wide vector, r=task,
       s=transform flags, t=track index, u/w=scratch vectors. v and x retain
       the original unused frame space (v is the easing-loop index in the
       nine-command dialect). The opcode i follows that space. */
    CameraScriptTask *taskPtr;
    int *wideVec;
    int easedVal;
    CameraScriptVector *scratchVector;
    int trackIndex;
    int bits;
    CameraScriptVector *pOtherVec;
    int v;
    int x[66];
    short opcode;

    taskPtr = (CameraScriptTask *)((char *)BATTLE_CURRENT_TASK_SLOT);
    scratchVector = (CameraScriptVector *)Gfx_GetPsxScratchpadPtr(0);
    pOtherVec = scratchVector + 1;
    wideVec = (int *)(pOtherVec + 1);
    if (g_BattleIsPaused == 0) {
        if (taskPtr->initialized == 0) {
            g_BattleSummonNeoBahamutCameraEye.frames = 0;
            g_BattleSummonNeoBahamutCameraFocus.frames = 0;
            g_BattleSummonNeoBahamutCameraEye.actor = -1;
            g_BattleSummonNeoBahamutCameraFocus.actor = -1;
            taskPtr->initialized = 1;
        }
        while (g_BattleSummonNeoBahamutCameraEye.frames == 0 || g_BattleSummonNeoBahamutCameraFocus.frames == 0) {
            opcode = *taskPtr->cursor++;
            if (opcode & 0x40) {
                g_pBattleSummonNeoBahamutCameraTrack = &g_BattleSummonNeoBahamutCameraEye;
                g_pCameraScriptOtherTrack = &g_BattleSummonNeoBahamutCameraFocus;
            } else {
                g_pBattleSummonNeoBahamutCameraTrack = &g_BattleSummonNeoBahamutCameraFocus;
                g_pCameraScriptOtherTrack = &g_BattleSummonNeoBahamutCameraEye;
            }
            g_pBattleSummonNeoBahamutCameraTrack->mode = opcode & ~0xc0;
            switch (g_pBattleSummonNeoBahamutCameraTrack->mode) {
            case SUMMON_CAMERA_WAIT:
                g_pBattleSummonNeoBahamutCameraTrack->frames = BattleFx_NeoBahamutReadCameraScriptOperand(&taskPtr->cursor);
                break;
            case SUMMON_CAMERA_SET_POSITION:
                g_pBattleSummonNeoBahamutCameraTrack->position.x = *taskPtr->cursor++;
                g_pBattleSummonNeoBahamutCameraTrack->position.y = *taskPtr->cursor++;
                g_pBattleSummonNeoBahamutCameraTrack->position.z = *taskPtr->cursor++;
                break;
            case SUMMON_CAMERA_MOVE_LINEAR:
                g_pBattleSummonNeoBahamutCameraTrack->frames = BattleFx_NeoBahamutReadCameraScriptOperand(&taskPtr->cursor);
                g_pBattleSummonNeoBahamutCameraTrack->velocity.x = (*taskPtr->cursor++ - g_pBattleSummonNeoBahamutCameraTrack->position.x) / g_pBattleSummonNeoBahamutCameraTrack->frames;
                g_pBattleSummonNeoBahamutCameraTrack->velocity.y = (*taskPtr->cursor++ - g_pBattleSummonNeoBahamutCameraTrack->position.y) / g_pBattleSummonNeoBahamutCameraTrack->frames;
                g_pBattleSummonNeoBahamutCameraTrack->velocity.z = (*taskPtr->cursor++ - g_pBattleSummonNeoBahamutCameraTrack->position.z) / g_pBattleSummonNeoBahamutCameraTrack->frames;
                break;
            case SUMMON_CAMERA_MOVE_ORBIT:
                scratchVector->x = g_pCameraScriptOtherTrack->position.x - g_pBattleSummonNeoBahamutCameraTrack->position.x;
                scratchVector->y = g_pCameraScriptOtherTrack->position.y - g_pBattleSummonNeoBahamutCameraTrack->position.y;
                scratchVector->z = g_pCameraScriptOtherTrack->position.z - g_pBattleSummonNeoBahamutCameraTrack->position.z;
                g_pBattleSummonNeoBahamutCameraTrack->distance = SqrtToInt(scratchVector->x * scratchVector->x + scratchVector->y * scratchVector->y + scratchVector->z * scratchVector->z);
                g_pBattleSummonNeoBahamutCameraTrack->base = g_pBattleSummonNeoBahamutCameraTrack->position;
                g_pBattleSummonNeoBahamutCameraTrack->frames = BattleFx_NeoBahamutReadCameraScriptOperand(&taskPtr->cursor);
                scratchVector->x = *taskPtr->cursor++;
                scratchVector->y = *taskPtr->cursor++;
                scratchVector->z = *taskPtr->cursor++;
                g_pBattleSummonNeoBahamutCameraTrack->velocity.x = (scratchVector->x - g_pBattleSummonNeoBahamutCameraTrack->position.x) / g_pBattleSummonNeoBahamutCameraTrack->frames;
                g_pBattleSummonNeoBahamutCameraTrack->velocity.y = (scratchVector->y - g_pBattleSummonNeoBahamutCameraTrack->position.y) / g_pBattleSummonNeoBahamutCameraTrack->frames;
                g_pBattleSummonNeoBahamutCameraTrack->velocity.z = (scratchVector->z - g_pBattleSummonNeoBahamutCameraTrack->position.z) / g_pBattleSummonNeoBahamutCameraTrack->frames;
                switch (g_pCameraScriptOtherTrack->mode) {
                case SUMMON_CAMERA_MOVE_LINEAR:
                    pOtherVec->x = g_pCameraScriptOtherTrack->position.x + g_pCameraScriptOtherTrack->velocity.x * g_pCameraScriptOtherTrack->frames;
                    pOtherVec->y = g_pCameraScriptOtherTrack->position.y + g_pCameraScriptOtherTrack->velocity.y * g_pCameraScriptOtherTrack->frames;
                    pOtherVec->z = g_pCameraScriptOtherTrack->position.z + g_pCameraScriptOtherTrack->velocity.z * g_pCameraScriptOtherTrack->frames;
                    break;
                default:
                    pOtherVec->x = g_pCameraScriptOtherTrack->position.x;
                    pOtherVec->y = g_pCameraScriptOtherTrack->position.y;
                    pOtherVec->z = g_pCameraScriptOtherTrack->position.z;
                    break;
                }
                scratchVector->x -= pOtherVec->x;
                scratchVector->y -= pOtherVec->y;
                scratchVector->z -= pOtherVec->z;
                g_pBattleSummonNeoBahamutCameraTrack->delta = (SqrtToInt(scratchVector->x * scratchVector->x + scratchVector->y * scratchVector->y + scratchVector->z * scratchVector->z) - g_pBattleSummonNeoBahamutCameraTrack->distance) / g_pBattleSummonNeoBahamutCameraTrack->frames;
                break;
            case SUMMON_CAMERA_MOVE_ACCEL:
                g_pBattleSummonNeoBahamutCameraTrack->frames = BattleFx_NeoBahamutReadCameraScriptOperand(&taskPtr->cursor);
                scratchVector->x = *taskPtr->cursor++ - g_pBattleSummonNeoBahamutCameraTrack->position.x;
                scratchVector->y = *taskPtr->cursor++ - g_pBattleSummonNeoBahamutCameraTrack->position.y;
                scratchVector->z = *taskPtr->cursor++ - g_pBattleSummonNeoBahamutCameraTrack->position.z;
                g_pBattleSummonNeoBahamutCameraTrack->base.x = ((scratchVector->x - g_pBattleSummonNeoBahamutCameraTrack->velocity.x * g_pBattleSummonNeoBahamutCameraTrack->frames) * 2) / (g_pBattleSummonNeoBahamutCameraTrack->frames * g_pBattleSummonNeoBahamutCameraTrack->frames);
                g_pBattleSummonNeoBahamutCameraTrack->base.y = ((scratchVector->y - g_pBattleSummonNeoBahamutCameraTrack->velocity.y * g_pBattleSummonNeoBahamutCameraTrack->frames) * 2) / (g_pBattleSummonNeoBahamutCameraTrack->frames * g_pBattleSummonNeoBahamutCameraTrack->frames);
                g_pBattleSummonNeoBahamutCameraTrack->base.z = ((scratchVector->z - g_pBattleSummonNeoBahamutCameraTrack->velocity.z * g_pBattleSummonNeoBahamutCameraTrack->frames) * 2) / (g_pBattleSummonNeoBahamutCameraTrack->frames * g_pBattleSummonNeoBahamutCameraTrack->frames);
                break;
            case SUMMON_CAMERA_MOVE_EASED:
                g_pBattleSummonNeoBahamutCameraTrack->frames = BattleFx_NeoBahamutReadCameraScriptOperand(&taskPtr->cursor);
                g_pBattleSummonNeoBahamutCameraTrack->easing = *taskPtr->cursor++;
                g_pBattleSummonNeoBahamutCameraTrack->distance = 0;
                g_pBattleSummonNeoBahamutCameraTrack->delta = 0x1000 / g_pBattleSummonNeoBahamutCameraTrack->frames;
                g_pBattleSummonNeoBahamutCameraTrack->base.x = g_pBattleSummonNeoBahamutCameraTrack->position.x;
                g_pBattleSummonNeoBahamutCameraTrack->base.y = g_pBattleSummonNeoBahamutCameraTrack->position.y;
                g_pBattleSummonNeoBahamutCameraTrack->base.z = g_pBattleSummonNeoBahamutCameraTrack->position.z;
                g_pBattleSummonNeoBahamutCameraTrack->velocity.x = *taskPtr->cursor++ - g_pBattleSummonNeoBahamutCameraTrack->position.x;
                g_pBattleSummonNeoBahamutCameraTrack->velocity.y = *taskPtr->cursor++ - g_pBattleSummonNeoBahamutCameraTrack->position.y;
                g_pBattleSummonNeoBahamutCameraTrack->velocity.z = *taskPtr->cursor++ - g_pBattleSummonNeoBahamutCameraTrack->position.z;
                break;
            case SUMMON_CAMERA_ATTACH_BONE:
                if ((g_pBattleSummonNeoBahamutCameraTrack->actor = *taskPtr->cursor++) == 0)
                    g_pBattleSummonNeoBahamutCameraTrack->actor = taskPtr->actor;
                g_pBattleSummonNeoBahamutCameraTrack->bone = *taskPtr->cursor++;
                break;
            case SUMMON_CAMERA_DETACH:
                g_pBattleSummonNeoBahamutCameraTrack->actor = -1;
                break;
            case SUMMON_CAMERA_WAIT_NO_WRITEBACK:
                g_pBattleSummonNeoBahamutCameraTrack->frames = BattleFx_NeoBahamutReadCameraScriptOperand(&taskPtr->cursor);
                break;
            case SUMMON_CAMERA_SET_SCRIPT_VALUE:
                g_BattleCameraScriptValue = *taskPtr->cursor++;
                break;
            default:
                taskPtr->active = -1;
                return;
            }
        }
        g_pBattleSummonNeoBahamutCameraTrack = &g_BattleSummonNeoBahamutCameraEye;
        g_pCameraScriptOtherTrack = &g_BattleSummonNeoBahamutCameraFocus;
        for (trackIndex = 0; trackIndex < 2; trackIndex++) {
            switch (g_pBattleSummonNeoBahamutCameraTrack->mode) {
            case SUMMON_CAMERA_WAIT:
                break;
            case SUMMON_CAMERA_MOVE_ACCEL:
                g_pBattleSummonNeoBahamutCameraTrack->velocity.x += g_pBattleSummonNeoBahamutCameraTrack->base.x;
                g_pBattleSummonNeoBahamutCameraTrack->velocity.y += g_pBattleSummonNeoBahamutCameraTrack->base.y;
                g_pBattleSummonNeoBahamutCameraTrack->velocity.z += g_pBattleSummonNeoBahamutCameraTrack->base.z;
            case SUMMON_CAMERA_MOVE_LINEAR:
                g_pBattleSummonNeoBahamutCameraTrack->position.x += g_pBattleSummonNeoBahamutCameraTrack->velocity.x;
                g_pBattleSummonNeoBahamutCameraTrack->position.y += g_pBattleSummonNeoBahamutCameraTrack->velocity.y;
                g_pBattleSummonNeoBahamutCameraTrack->position.z += g_pBattleSummonNeoBahamutCameraTrack->velocity.z;
                break;
            case SUMMON_CAMERA_MOVE_ORBIT:
                g_pBattleSummonNeoBahamutCameraTrack->base.x += g_pBattleSummonNeoBahamutCameraTrack->velocity.x;
                g_pBattleSummonNeoBahamutCameraTrack->base.y += g_pBattleSummonNeoBahamutCameraTrack->velocity.y;
                g_pBattleSummonNeoBahamutCameraTrack->base.z += g_pBattleSummonNeoBahamutCameraTrack->velocity.z;
                g_pBattleSummonNeoBahamutCameraTrack->distance += g_pBattleSummonNeoBahamutCameraTrack->delta;
                wideVec[0] = g_pCameraScriptOtherTrack->position.x - g_pBattleSummonNeoBahamutCameraTrack->base.x;
                wideVec[1] = g_pCameraScriptOtherTrack->position.y - g_pBattleSummonNeoBahamutCameraTrack->base.y;
                wideVec[2] = g_pCameraScriptOtherTrack->position.z - g_pBattleSummonNeoBahamutCameraTrack->base.z;
                VectorNormalToSVector(wideVec, (short *)scratchVector);
                g_pBattleSummonNeoBahamutCameraTrack->position.x = g_pCameraScriptOtherTrack->position.x - ((scratchVector->x * g_pBattleSummonNeoBahamutCameraTrack->distance) >> 12);
                g_pBattleSummonNeoBahamutCameraTrack->position.y = g_pCameraScriptOtherTrack->position.y - ((scratchVector->y * g_pBattleSummonNeoBahamutCameraTrack->distance) >> 12);
                g_pBattleSummonNeoBahamutCameraTrack->position.z = g_pCameraScriptOtherTrack->position.z - ((scratchVector->z * g_pBattleSummonNeoBahamutCameraTrack->distance) >> 12);
                break;
            case SUMMON_CAMERA_MOVE_EASED:
                g_pBattleSummonNeoBahamutCameraTrack->distance += g_pBattleSummonNeoBahamutCameraTrack->delta;
                easedVal = g_pBattleSummonNeoBahamutCameraTrack->distance;
                easedVal = BattleFx_NeoBahamutApplyCameraEasing(easedVal, g_pBattleSummonNeoBahamutCameraTrack->easing);
                g_pBattleSummonNeoBahamutCameraTrack->position.x = g_pBattleSummonNeoBahamutCameraTrack->base.x + ((g_pBattleSummonNeoBahamutCameraTrack->velocity.x * easedVal) >> 12);
                g_pBattleSummonNeoBahamutCameraTrack->position.y = g_pBattleSummonNeoBahamutCameraTrack->base.y + ((g_pBattleSummonNeoBahamutCameraTrack->velocity.y * easedVal) >> 12);
                g_pBattleSummonNeoBahamutCameraTrack->position.z = g_pBattleSummonNeoBahamutCameraTrack->base.z + ((g_pBattleSummonNeoBahamutCameraTrack->velocity.z * easedVal) >> 12);
                break;
            }
            g_pBattleSummonNeoBahamutCameraTrack->frames--;
            g_pBattleSummonNeoBahamutCameraTrack = &g_BattleSummonNeoBahamutCameraFocus;
            g_pCameraScriptOtherTrack = &g_BattleSummonNeoBahamutCameraEye;
        }
    }
    if (taskPtr->matrix) {
        if (g_BattleSummonNeoBahamutCameraEye.mode != SUMMON_CAMERA_WAIT_NO_WRITEBACK) {
            SetRotMatrix((void *)taskPtr->matrix);
            SetTransMatrix((void *)taskPtr->matrix);
            if (g_BattleSummonNeoBahamutCameraEye.actor == -1) {
                RotTransPoint(&g_BattleSummonNeoBahamutCameraEye.position, wideVec, &bits);
                g_BattleCameraPosition.cameraPosX = wideVec[0];
                g_BattleCameraPosition.cameraPosY = wideVec[1];
                g_BattleCameraPosZ = wideVec[2];
            } else {
                RotateSVectorByInstalledMatrix(&g_BattleSummonNeoBahamutCameraEye.position, wideVec);
                if (g_BattleSummonNeoBahamutCameraEye.bone != -1) {
                    Battle_GetBoneWorldPos(g_BattleSummonNeoBahamutCameraEye.actor, g_BattleSummonNeoBahamutCameraEye.bone, (short *)scratchVector);
                } else {
                    scratchVector->x = g_BattleActorRecords[g_BattleSummonNeoBahamutCameraEye.actor].posX;
                    scratchVector->y = g_BattleActorRecords[g_BattleSummonNeoBahamutCameraEye.actor].posY;
                    scratchVector->z = g_BattleActorRecords[g_BattleSummonNeoBahamutCameraEye.actor].posZ;
                }
                g_BattleCameraPosition.cameraPosX = wideVec[0] + scratchVector->x;
                g_BattleCameraPosition.cameraPosY = wideVec[1] + scratchVector->y;
                g_BattleCameraPosZ = wideVec[2] + scratchVector->z;
                SetRotMatrix((void *)taskPtr->matrix);
                SetTransMatrix((void *)taskPtr->matrix);
            }
        }
        if (g_BattleSummonNeoBahamutCameraFocus.mode != SUMMON_CAMERA_WAIT_NO_WRITEBACK) {
            if (g_BattleSummonNeoBahamutCameraFocus.actor == -1) {
                RotTransPoint(&g_BattleSummonNeoBahamutCameraFocus.position, wideVec, &bits);
                g_BattleCameraFocalPoint.focalPointX = wideVec[0];
                g_BattleCameraFocalPoint.focalPointY = wideVec[1];
                g_BattleCameraFocusZ = wideVec[2];
            } else {
                RotateSVectorByInstalledMatrix(&g_BattleSummonNeoBahamutCameraFocus.position, wideVec);
                if (g_BattleSummonNeoBahamutCameraFocus.bone != -1) {
                    Battle_GetBoneWorldPos(g_BattleSummonNeoBahamutCameraFocus.actor, g_BattleSummonNeoBahamutCameraFocus.bone, (short *)scratchVector);
                } else {
                    scratchVector->x = g_BattleActorRecords[g_BattleSummonNeoBahamutCameraFocus.actor].posX;
                    scratchVector->y = g_BattleActorRecords[g_BattleSummonNeoBahamutCameraFocus.actor].posY;
                    scratchVector->z = g_BattleActorRecords[g_BattleSummonNeoBahamutCameraFocus.actor].posZ;
                }
                g_BattleCameraFocalPoint.focalPointX = wideVec[0] + scratchVector->x;
                g_BattleCameraFocalPoint.focalPointY = wideVec[1] + scratchVector->y;
                g_BattleCameraFocusZ = wideVec[2] + scratchVector->z;
            }
        }
    } else {
        if (g_BattleSummonNeoBahamutCameraEye.mode != SUMMON_CAMERA_WAIT_NO_WRITEBACK) {
            *(int *)&g_BattleCameraPosition = *(int *)&g_BattleSummonNeoBahamutCameraEye.position;
            *(int *)&g_BattleCameraPosZ = *(int *)&g_BattleSummonNeoBahamutCameraEye.position.z;
        }
        if (g_BattleSummonNeoBahamutCameraFocus.mode != SUMMON_CAMERA_WAIT_NO_WRITEBACK) {
            *(int *)&g_BattleCameraFocalPoint = *(int *)&g_BattleSummonNeoBahamutCameraFocus.position;
            *(int *)&g_BattleCameraFocusZ = *(int *)&g_BattleSummonNeoBahamutCameraFocus.position.z;
        }
    }
}

// Reads one short from the cursor whose address is in cursorPtr and advances that cursor by 2. A
// negative token is not a literal: it is negated and used as a short index into the pool at
// g_BattleSummonNeoBahamutBreathScratch.operandBase, and that entry is returned instead.
// FUNCTION: FF7 0x0048d6a3
short __cdecl BattleFx_NeoBahamutReadCameraScriptOperand(void *cursorPtr)
{
    int token;

    token = *(short *)((char *)*(int *)cursorPtr);
    *(int *)cursorPtr += 2;
    if (token < 0) {
        token = (&g_BattleSummonNeoBahamutBreathScratch.operandBase)[-token];
    }
    return (short)token;
}

// Pure arithmetic, no globals. Saves value, then for control > 0 applies value = SinFixed(value / 4)
// (control >> 8) times; for control <= 0 negates control, mirrors value as 0x1000 - value, applies the same
// iteration and mirrors back. Finally blends the result with the saved input by weight (control &
// 0xFF) + 1 out of 0x100 and returns it: an easing curve on a 0..0x1000 value.
// FUNCTION: FF7 0x0048d6dd
int __cdecl BattleFx_NeoBahamutApplyCameraEasing(int value, int control)
{
    int iteration;
    int count;
    int saved;

    saved = value;
    if (control > 0) {
        count = control >> 8;
        for (iteration = 0; iteration < count; iteration++) {
            value = SinFixed(value / 4);
        }
    } else {
        control = -control;
        count = control >> 8;
        value = 0x1000 - value;
        for (iteration = 0; iteration < count; iteration++) {
            value = SinFixed(value / 4);
        }
        value = 0x1000 - value;
    }
    count = (control & 0xFF) + 1;
    value = (value * count + saved * (0x100 - count)) >> 8;
    return value;
}

// Acquires retained scratch before pause. Unpaused first 207 counts only advance the task count.
// Next 110 allocate ring record one on entry with transformed retained position plus translation
// and angle g_BattleSummonNeoBahamutAngleVector + 2 plus 0x800, then ease retained Y from minus
// 10000 toward 1200 using BattleFx_NeoBahamutApplyCameraEasing parameter 0x3E0. Next 112 hold
// position. Next fifteen use parameter 0x1C0 to set X zero/Y 1200 through explicit zero
// multiplication and ease Z from minus 10000 toward minus 18000; copy a related point to
// g_BattleSummonNeoBahamutAnchorPosition with Y minus 2700 and Z plus 2500. Next 75 hold values and
// allocate ring record two at elapsed 74; later retire and return. After the initial wait, every
// ordinary unpaused call transforms retained position through g_BattleSummonNeoBahamutWorldMatrix,
// adds retained translation into the actor pointer g_BattleSummonNeoBahamutSceneActorRef and
// advances count.
// FUNCTION: FF7 0x0048d7bc
void __cdecl BattleFx_NeoBahamutRunLightRig(void)
{
    int elapsed;
    BattleFxLightRigSlot *ring;
    BattleTaskSlot *taskPtr;
    int easedProgress;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonNeoBahamutLightRigInitFlag & 1) == 0) {
        g_BattleSummonNeoBahamutLightRigInitFlag |= 1;
        g_BattleSummonNeoBahamutLightRigTaskSlot = (BattleFxNeoBahamutLightRig *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    elapsed = taskPtr->frameCounter;
    if (elapsed < 207) {
        (taskPtr->frameCounter)++;
    } else {
        elapsed -= 207;
        if (elapsed < 110) {
            if (elapsed == 0) {
                ring = (BattleFxLightRigSlot *)Battle_AllocSceneMessage(1);
                TransformVectorToInt16(&g_BattleSummonNeoBahamutWorldMatrix, (short *)&g_BattleSummonNeoBahamutLightRigAngle, (void *)g_BattleSummonNeoBahamutLightRigTaskSlot);
                ring->posX = g_BattleSummonNeoBahamutLightRigTaskSlot->rigX + g_BattleSummonNeoBahamutWorldMatrix.t[0];
                ring->posY = g_BattleSummonNeoBahamutLightRigTaskSlot->rigY + g_BattleSummonNeoBahamutWorldMatrix.t[1];
                ring->posZ = g_BattleSummonNeoBahamutLightRigTaskSlot->rigZ + g_BattleSummonNeoBahamutWorldMatrix.t[2];
                ring->rotationY = g_BattleSummonNeoBahamutAngleVector[1] + 0x800;
            }
            easedProgress = (elapsed << 12) / 110;
            easedProgress = BattleFx_NeoBahamutApplyCameraEasing(easedProgress, 0x3E0);
            (g_BattleSummonNeoBahamutLightRigAngle.angleX) = (easedProgress * 0x2BC0 >> 12) - 0x2710;
        } else {
            elapsed -= 110;
            if (elapsed < 112) { /* empty first arm: lib/codegen.h */
            } else {
                elapsed -= 112;
                if (elapsed < 15) {
                    easedProgress = (elapsed << 12) / 15;
                    easedProgress = BattleFx_NeoBahamutApplyCameraEasing(easedProgress, 0x1C0);
                    g_BattleSummonNeoBahamutLightRigAngle.vectorX = (easedProgress * 0) >> 12;
                    g_BattleSummonNeoBahamutAnchorPosition[0] = g_BattleSummonNeoBahamutLightRigAngle.vectorX;
                    (g_BattleSummonNeoBahamutLightRigAngle.angleX) = ((easedProgress * 0) >> 12) + 0x4B0;
                    g_BattleSummonNeoBahamutAnchorPosition[1] = (g_BattleSummonNeoBahamutLightRigAngle.angleX) - 2700;
                    (g_BattleSummonNeoBahamutLightRigAngle.angleY) = (easedProgress * -0x1F40 >> 12) - 0x2710;
                    g_BattleSummonNeoBahamutAnchorPosition[2] = (g_BattleSummonNeoBahamutLightRigAngle.angleY) + 2500;
                } else {
                    elapsed -= 15;
                    if (elapsed < 75) {
                        if (elapsed == 74) {
                            Battle_AllocSceneMessage(2);
                        }
                    } else {
                        taskPtr->ownerSlot = BATTLE_TASK_FREE;
                        return;
                    }
                }
            }
        }
        TransformVectorToInt16(&g_BattleSummonNeoBahamutWorldMatrix, (short *)&g_BattleSummonNeoBahamutLightRigAngle, (void *)g_BattleSummonNeoBahamutLightRigTaskSlot);
        g_BattleSummonNeoBahamutSceneActorRef->posX = g_BattleSummonNeoBahamutLightRigTaskSlot->rigX + g_BattleSummonNeoBahamutWorldMatrix.t[0];
        g_BattleSummonNeoBahamutSceneActorRef->posY = g_BattleSummonNeoBahamutLightRigTaskSlot->rigY + g_BattleSummonNeoBahamutWorldMatrix.t[1];
        g_BattleSummonNeoBahamutSceneActorRef->posZ = g_BattleSummonNeoBahamutLightRigTaskSlot->rigZ + g_BattleSummonNeoBahamutWorldMatrix.t[2];
        (taskPtr->frameCounter)++;
    }
}

// Runs the Neo Bahamut master timeline, multiplying g_BattleCameraTransform with g_BattleSummonNeoBahamutWorldMatrix into
// g_BattleSummonNeoBahamutComposedMatrix before the pause return. Unpaused count zero clears control short; at four
// logs/scans ground. At five starts thunder task/ring record seven with selector five, invokes
// BattleFx_SummonBeginNeoBahamutJitterEnvelope and sound group zero; ten clears RGB controls and starts a 25-count ramp to 0x800.
// At 24 creates mask 0x3F0 callbacks and calls BattleFx_SummonNeoBahamutToggleActorMaskFlags. At 25 schedules ground/dust/rock
// tasks and another BattleFx_SummonBeginNeoBahamutJitterEnvelope; 68 sets flag bits zero through two. At seventy schedules
// cloud/sky tasks and ring selector six; 85 initializes the second cloud task, 100 starts line-
// light manager, 150 starts flash and 160 sets flags/control. At 170 fades flash, schedules
// cloudsea/backsky and sound group one. After the retained 163-count empty stage, 388 schedules
// ball/circle/line-light tasks and sound group two, releasing buffers 0x288 through 0x28A. At 430
// starts breath, 445 rock manager, and 520 flash/rock/line-light tasks with sound group three and
// releases 0x28B through 0x28D. At 540 starts flash; 559 clears flag bits and calls BattleFx_SummonNeoBahamutToggleActorMaskFlags;
// 560 fades flash. At exhaustion 561 allocates selected actor records, clears flags, retires,
// releases each nonnegative offscreen slot and marks it minus one, releases buffers 0x28E through
// 0x293 and logs completion, returning without counter increment. Ordinary calls increment count.
// FUNCTION: FF7 0x0048da7a
void __cdecl BattleFx_SummonNeoBahamutTimelineTask(void)
{
    int stage;
    int targetIndex;
    BattleFxDualCounterTask *task;
    int ringData;
    BattleFxNeoBahamutTimeline *newTask;

    task = (BattleFxDualCounterTask *)BATTLE_CURRENT_TASK_SLOT;
    MultiplyMatrix((void *)g_BattleCameraTransform.matrix, &g_BattleSummonNeoBahamutWorldMatrix, &g_BattleSummonNeoBahamutComposedMatrix);
    if (g_BattleIsPaused != 0) {
        return;
    }
    stage = task->nextIndex;
    if (stage < 5) {
        if (stage == 0) {
            g_BattleFxTimelineState.startValue = 0;
        }
        if (stage == 4) {
            // STRING: FF7 0x007f997c
            Gfx_DebugPrint("[Neo] begin scan ground\n");
            BattleFx_SummonNeoBahamutPrepareStageOffscreenRequests();
            // STRING: FF7 0x007f9998
            Gfx_DebugPrint("[Neo] finish scan ground\n");
        }
    } else {
        stage -= 5;
        if (stage < 10) {
            if (stage == 0) {
                // STRING: FF7 0x007f99b4
                Gfx_DebugPrint("[Neo] begin thunder\n");
                newTask = (BattleFxNeoBahamutTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutSpawnThunderSpritesTask);
                ringData = Battle_AllocSceneMessage(7);
                *(short *)((char *)ringData + 0x10) = 5;
            }
            if (stage == 0) {
                BattleFx_SummonBeginNeoBahamutJitterEnvelope(0x32, 0, 0x14, 0);
            }
            if (stage == 5) {
                g_BattleFxTimelineState.red = 0;
                g_BattleFxTimelineState.channel1 = 0;
                g_BattleFxTimelineState.channel2 = 0;
                Battle_StartFixedPointRamp(0x800, 25);
            }
            if (stage == 0) {
                // STRING: FF7 0x007f99cc
                Gfx_DebugPrint("[Neo] Effect_play3 group 0\n");
                Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 0);
                // STRING: FF7 0x007f99e8
                Gfx_DebugPrint("[Neo] finished effect play\n");
            }
        } else {
            stage -= 10;
            if (stage < 10) {
                if (stage == 9) {
                    Battle_CreateBitPatternCallbackTask(BATTLE_ACTOR_MASK_ENEMIES, 0, 0, BattleFx_SummonSpawnNeoBahamutPerTarget);
                    BattleFx_SummonNeoBahamutToggleActorMaskFlags();
                }
            } else {
                stage -= 10;
                if (stage < 25) {
                    if (stage == 0) {
                        // STRING: FF7 0x007f9a04
                        Gfx_DebugPrint("[Neo] Link ground, dust_manager, rock1_manager\n");
                        newTask = (BattleFxNeoBahamutTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutGroundModelTask);
                        newTask = (BattleFxNeoBahamutTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutSpawnDustSpritesTask);
                        newTask = (BattleFxNeoBahamutTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutSpawnRock1ModelsTask);
                    }
                    if (stage == 0) {
                        BattleFx_SummonBeginNeoBahamutJitterEnvelope(0x64, 0, 0x2D, 0);
                    }
                } else {
                    stage -= 25;
                    if (stage < 20) {
                        if (stage == 18) {
                            g_BattleStageHideFlags |= 7;
                        }
                    } else {
                        stage -= 20;
                        if (stage < 100) {
                            if (stage == 0) {
                                // STRING: FF7 0x007f9a34
                                Gfx_DebugPrint("[Neo] link cloud1, sky_mask\n");
                                newTask = (BattleFxNeoBahamutTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutCloud1ModelTask);
                                newTask = (BattleFxNeoBahamutTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutSkyMaskModelTask);
                                ringData = Battle_AllocSceneMessage(7);
                                *(short *)((char *)ringData + 0x10) = 6;
                            }
                            if (stage == 15) {
                                // STRING: FF7 0x007f9a54
                                Gfx_DebugPrint("[Neo] link cloud2\n");
                                newTask = (BattleFxNeoBahamutTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutCloud2ModelTask);
                                newTask->posX = 9600;
                                newTask->posY = 0xB1E0;
                                newTask->posZ = 0;
                                newTask->clearedWordC = 0;
                                newTask->clearedWordB = 0;
                                newTask->clearedWordA = 0;
                                newTask->scale = 0x1F00 /* 1.9375 */;
                            }
                            if (stage == 30) {
                                // STRING: FF7 0x007f9a68
                                Gfx_DebugPrint("[Neo] link line_light3_manager\n");
                                newTask = (BattleFxNeoBahamutTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutSpawnLineLight3ModelsTask);
                            }
                            if (stage == 80) {
                                // STRING: FF7 0x007f9a88
                                Gfx_DebugPrint("[Neo] white out\n");
                                BattleFx_StartScreenFlashRamp(10);
                            }
                            if (stage == 90) {
                                g_BattleStageHideFlags |= 7;
                                g_BattleFxTimelineState.startValue = 0;
                            }
                        } else {
                            stage -= 100;
                            if (stage < 55) {
                                if (stage == 0) {
                                    // STRING: FF7 0x007f9a9c
                                    Gfx_DebugPrint("[Neo] white-in, link cloudsea, backsky\n");
                                    BattleFx_FadeOutScreenFlashRamp(15);
                                    newTask = (BattleFxNeoBahamutTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutCloudSeaModelTask);
                                    newTask = (BattleFxNeoBahamutTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutBackSkyPrimitiveTask);
                                }
                                if (stage == 0) {
                                    // STRING: FF7 0x007f9ac4
                                    Gfx_DebugPrint("[Neo] effect play3 group 1\n");
                                    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 1);
                                    // STRING: FF7 0x007f9ae0
                                    Gfx_DebugPrint("[Neo] finished effect play 3\n");
                                }
                            } else {
                                stage -= 55;
                                if (stage < 163) { /* empty first arm: lib/codegen.h */
                                } else {
                                    stage -= 163;
                                    if (stage < 42) {
                                        if (stage == 0) {
                                            // STRING: FF7 0x007f9b00
                                            Gfx_DebugPrint("[Neo] linke ball_manager, circle, line_light1_manager\n");
                                            newTask = (BattleFxNeoBahamutTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutSpawnBallSpritesTask);
                                            newTask = (BattleFxNeoBahamutTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutCircleModelTask);
                                            newTask = (BattleFxNeoBahamutTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutSpawnLineLight1Task);
                                        }
                                        if (stage == 0) {
                                            // STRING: FF7 0x007f9b38
                                            Gfx_DebugPrint("[Neo] effect_play3 group 2\n");
                                            Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 2);
                                            // STRING: FF7 0x007f9b54
                                            Gfx_DebugPrint("[Neo] finished effect play3\n");
                                            Sfx_ReleaseCachedBuffer(0x288, 0);
                                            Sfx_ReleaseCachedBuffer(0x289, 0);
                                            Sfx_ReleaseCachedBuffer(0x28A, 0);
                                        }
                                    } else {
                                        stage -= 42;
                                        if (stage < 15) {
                                            if (stage == 0) {
                                                // STRING: FF7 0x007f9b74
                                                Gfx_DebugPrint("[Neo] link breath\n");
                                                newTask = (BattleFxNeoBahamutTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutBreathModelTask);
                                            }
                                        } else {
                                            stage -= 15;
                                            if (stage < 45) {
                                                if (stage == 0) {
                                                    // STRING: FF7 0x007f9b88
                                                    Gfx_DebugPrint("[Neo] rock2_manager\n");
                                                    newTask = (BattleFxNeoBahamutTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutSpawnRock2ModelsTask);
                                                }
                                            } else {
                                                stage -= 45;
                                                if (stage < 30) { /* empty first arm: lib/codegen.h */
                                                } else {
                                                    stage -= 30;
                                                    if (stage < 40) {
                                                        if (stage == 0) {
                                                            // STRING: FF7 0x007f9ba0
                                                            Gfx_DebugPrint("[Neo] link flash2, rock3_manager, line_light2_manager\n");
                                                            newTask = (BattleFxNeoBahamutTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutFlash2ModelTask);
                                                            newTask = (BattleFxNeoBahamutTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutSpawnRock3ModelsTask);
                                                            newTask = (BattleFxNeoBahamutTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutSpawnLineLight2Task);
                                                        }
                                                        if (stage == 20) {
                                                            // STRING: FF7 0x007f9bd8
                                                            Gfx_DebugPrint("[Neo] white-out\n");
                                                            BattleFx_StartScreenFlashRamp(10);
                                                        }
                                                        if (stage == 39) {
                                                            g_BattleStageHideFlags &= 0xF8;
                                                            BattleFx_SummonNeoBahamutToggleActorMaskFlags();
                                                        }
                                                        if (stage == 0) {
                                                            // STRING: FF7 0x007f9bec
                                                            Gfx_DebugPrint("[Neo] effect_play3 group 3\n");
                                                            Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 3);
                                                            // STRING: FF7 0x007f9c08
                                                            Gfx_DebugPrint("[Neo] finished effect_play3\n");
                                                            Sfx_ReleaseCachedBuffer(0x28B, 1);
                                                            Sfx_ReleaseCachedBuffer(0x28C, 1);
                                                            Sfx_ReleaseCachedBuffer(0x28D, 1);
                                                        }
                                                    } else {
                                                        stage -= 40;
                                                        if (stage < 1) {
                                                            if (stage == 0) {
                                                                // STRING: FF7 0x007f9c28
                                                                Gfx_DebugPrint("[Neo] white-in\n");
                                                                BattleFx_FadeOutScreenFlashRamp(10);
                                                            }
                                                        } else {
                                                            for (targetIndex = 0; targetIndex < 0xA; targetIndex++) {
                                                                if ((g_BattleSummonNeoBahamutTargetMask & 1 << targetIndex) != 0) {
                                                                    Battle_PostActorHitOrHurtMessage(targetIndex);
                                                                }
                                                            }
                                                            g_BattleStageHideFlags = 0;
                                                            task->ownerSlot = BATTLE_TASK_FREE;
                                                            if (g_BattleNeoBahamutField12OffscreenSlot >= 0) {
                                                                Battle3d_ReleaseOffscreenSlot(g_BattleNeoBahamutField12OffscreenSlot);
                                                                g_BattleNeoBahamutField12OffscreenSlot = 0xFFFFFFFF;
                                                            }
                                                            if (g_BattleNeoBahamutField4OffscreenSlot >= 0) {
                                                                Battle3d_ReleaseOffscreenSlot(g_BattleNeoBahamutField4OffscreenSlot);
                                                                g_BattleNeoBahamutField4OffscreenSlot = 0xFFFFFFFF;
                                                            }
                                                            Sfx_ReleaseCachedBuffer(0x28E, 2);
                                                            Sfx_ReleaseCachedBuffer(0x28F, 2);
                                                            Sfx_ReleaseCachedBuffer(0x290, 2);
                                                            Sfx_ReleaseCachedBuffer(0x291, 3);
                                                            Sfx_ReleaseCachedBuffer(0x292, 3);
                                                            Sfx_ReleaseCachedBuffer(0x293, 3);
                                                            // STRING: FF7 0x007f9c38
                                                            Gfx_DebugPrint("[Neo] finish Neo Bahamut\n");
                                                            return;
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
    (task->nextIndex)++;
}

// Allocates a resource slot for BattleFx_SummonNeoBahamutJitterEnvelopeTask and stores its four arguments as words at
// +4/+6/+8/+0xA.
// FUNCTION: FF7 0x0048e27e
void __cdecl BattleFx_SummonBeginNeoBahamutJitterEnvelope(int value, int second, int third, int fourth)
{
    BattleFxNeoBahamutJitterRecord *slot;

    slot = (BattleFxNeoBahamutJitterRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutJitterEnvelopeTask);
    slot->first = value;
    slot->second = second;
    slot->third = third;
    slot->fourth = fourth;
}

// Returns while paused. Computes amplitude from task short four through consecutive rise/hold/fall
// durations at six/eight/0xA: linear fixed-point rise, full amplitude hold and fall using 4096
// minus elapsed fraction. On exhaustion retires and clears g_BattleCameraTransform.posZ without count advancement.
// Otherwise writes positive amplitude on odd task counts and negative amplitude on even counts to
// that global, then increments count.
// FUNCTION: FF7 0x0048e2cd
void __cdecl BattleFx_SummonNeoBahamutJitterEnvelopeTask(void)
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
                g_BattleCameraTransform.posZ = 0;
                return;
            }
        }
    }
    g_BattleCameraTransform.posZ = (local.p->frameCount & 1) ? local.value : -local.value;
    (local.p->frameCount)++;
}

// Fills the screen rectangle of g_BattleSummonNeoBahamutStageOffscreenRequest from the viewport origin
// with a 0x40 by 0x80 size (0x80 by 0x100 when g_BattleUiScalingMode == 2), gives the stage scale
// matrix the world matrix's scale and +/- its z translation (by g_BattleSummonNeoBahamutAngleVector[1]),
// multiplies it into a local matrix and queues the stage offscreen requests with it and the rectangle.
// FUNCTION: FF7 0x0048e3f6
void __cdecl BattleFx_SummonNeoBahamutPrepareStageOffscreenRequests(void)
{
    int m[8];

    if (g_BattleUiScalingMode != 2) {
        g_BattleSummonNeoBahamutStageOffscreenRequest.rect.x = (short)g_BattleViewportOriginX;
        g_BattleSummonNeoBahamutStageOffscreenRequest.rect.y = (short)g_BattleViewportOriginY;
        g_BattleSummonNeoBahamutStageOffscreenRequest.rect.width = 64;
        g_BattleSummonNeoBahamutStageOffscreenRequest.rect.height = 128;
    } else {
        g_BattleSummonNeoBahamutStageOffscreenRequest.rect.x = (short)g_BattleViewportOriginX;
        g_BattleSummonNeoBahamutStageOffscreenRequest.rect.y = (short)g_BattleViewportOriginY;
        g_BattleSummonNeoBahamutStageOffscreenRequest.rect.width = 128;
        g_BattleSummonNeoBahamutStageOffscreenRequest.rect.height = 256;
    }
    g_BattleSummonNeoBahamutStageOffscreenRequest.m[4] = g_BattleSummonNeoBahamutWorldMatrix.m[0][0];
    *(short *)((char *)&g_BattleSummonNeoBahamutStageOffscreenParam + 0x18) = g_BattleSummonNeoBahamutStageOffscreenRequest.m[4];
    if (g_BattleSummonNeoBahamutAngleVector[1] != 0) {
        g_BattleSummonNeoBahamutStageOffscreenRequest.t[2] = g_BattleSummonNeoBahamutWorldMatrix.t[2];
    } else {
        g_BattleSummonNeoBahamutStageOffscreenRequest.t[2] = -g_BattleSummonNeoBahamutWorldMatrix.t[2];
    }
    MultiplyMatrix((void *)(((int)&g_BattleSummonNeoBahamutCameraScript2 + 344)), (char *)&g_BattleSummonNeoBahamutStageOffscreenParam + 0x18, m);
    BattleFx_SummonNeoBahamutQueueStageOffscreenRequests(m, &g_BattleSummonNeoBahamutStageOffscreenRequest.rect);
}

// Installs the supplied rotation/translation even without a scene. For a nonnull scene and stage
// pointer independently acquires two offscreen requests; each nonnull request gets type one, stage
// mesh part one, transposed float matrix, zero field 0xC8, field 0xD0 four, four signed shorts from
// argument two expanded into 0xDC through 0xE8 and both flags 0x10C/0x110 set one. First request
// uses field 0xCC twelve and saves slot 0x114 to g_BattleNeoBahamutField12OffscreenSlot; second
// uses four and saves to g_BattleNeoBahamutField4OffscreenSlot. Failure of one acquisition does not
// prevent the other and leaves its saved slot unchanged.
// FUNCTION: FF7 0x0048e4cd
void __cdecl BattleFx_SummonNeoBahamutQueueStageOffscreenRequests(void *matrix, BattleFxScreenRect *rect)
{
    GfxOffscreenRequest *pRequest;
    int stage;

    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    if (g_pBattle3dScene != 0 && g_pBattle3dScene->stageId != 0) {
        stage = g_pBattle3dScene->stageId;
        pRequest = (GfxOffscreenRequest *)Battle3d_AcquireOffscreenRequest();
        if (pRequest != 0) {
            pRequest->kind = 1;
            pRequest->mesh = (int)BattleStage_GetPartMesh(1, (Battle3dStageRecord *)stage);
            GtePsxMatrixToFloat4x4Transposed(matrix, (char *)pRequest + 0x88);
            pRequest->pageFillEnabled = 0;
            pRequest->renderKind = 0xC;
            pRequest->blendMode = BLEND_NONE;
            pRequest->viewportX = rect->x;
            pRequest->viewportY = rect->y;
            pRequest->viewportW = rect->width;
            pRequest->viewportH = rect->height;
            pRequest->pageFillStepX = 1;
            pRequest->pageFillStepY = 1;
            g_BattleNeoBahamutField12OffscreenSlot = pRequest->slotIndex;
        }
        pRequest = (GfxOffscreenRequest *)Battle3d_AcquireOffscreenRequest();
        if (pRequest != 0) {
            pRequest->kind = 1;
            pRequest->mesh = (int)BattleStage_GetPartMesh(1, (Battle3dStageRecord *)stage);
            GtePsxMatrixToFloat4x4Transposed(matrix, (char *)pRequest + 0x88);
            pRequest->pageFillEnabled = 0;
            pRequest->renderKind = 4;
            pRequest->blendMode = BLEND_NONE;
            pRequest->viewportX = rect->x;
            pRequest->viewportY = rect->y;
            pRequest->viewportW = rect->width;
            pRequest->viewportH = rect->height;
            pRequest->pageFillStepX = 1;
            pRequest->pageFillStepY = 1;
            g_BattleNeoBahamutField4OffscreenSlot = pRequest->slotIndex;
        }
    }
}

// Acquires data block
// zero once. First 465 counts build a ground matrix from g_BattleSummonNeoBahamutComposedMatrix and
// g_BattleSummonNeoBahamutGroundMatrix: first 45 reduce the ground matrix descriptor from current control
// short toward zero and subtract 111 from Y translation only while unpaused; next 100 reset
// Y/descriptor on entry and subtract 200 from Y only while unpaused; next 320 reset Y on entry and,
// from elapsed 275, alternate X translation minus/plus thirty. Installs the composed matrix,
// selects signed control RGB, refreshes transform state, binds both offscreen objects through
// opcode nine/eleven fields four/one and calls BattleFx_SummonNeoBahamutDrawPsxMesh,
// chaining its return. Next 75 call BattleFx_SummonNeoBahamutInitFragmentTable on
// entry. First thirty build X/Z rotations from elapsed fractions, alternate X translation
// minus/plus fifty and set Y/Z translation to elapsed times thirty, composing
// g_BattleSummonNeoBahamutComposedMatrix. At elapsed thirty set retained diagonal shorts 256 and
// translation (-5000,-20000,0), composing it thereafter. Calls
// BattleFx_SummonNeoBahamutUpdateFragmentMatrices with the matrix, binds the same
// offscreen objects and calls BattleFx_SummonNeoBahamutDrawPsxMeshPerFaceMatrix with
// extra g_BattleSummonNeoBahamutGroundVertexBlob. Later retires and returns. Only
// counter and the first two Y decrements are pause-gated; entry calls/resets, matrix changes and
// drawing remain active while paused.
// FUNCTION: FF7 0x0048e6bd
void __cdecl BattleFx_SummonNeoBahamutGroundModelTask(void)
{
    int stage;
    int unusedSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxDualCounterTask *taskPtr;
    int progress;

    taskPtr = (BattleFxDualCounterTask *)BATTLE_CURRENT_TASK_SLOT;
    if (((unsigned char)g_BattleSummonNeoBahamutGroundInitFlag & 1) == 0) {
        g_BattleSummonNeoBahamutGroundInitFlag |= 1;
        g_BattleSummonNeoBahamutGroundTaskSlot = Gfx_GetPsxScratchpadPtr(0);
    }
    stage = taskPtr->nextIndex;
    if (stage < 465) {
        if (stage < 45) {
            progress = (stage << 12) / 45;
            g_BattleSummonNeoBahamutStageOffscreenRequest.mesh.vertexShade = g_BattleFxTimelineState.startValue + ((0 - g_BattleFxTimelineState.startValue) * progress >> 12);
            if (g_BattleIsPaused == 0) {
                g_BattleSummonNeoBahamutGroundMatrix.t[1] -= 0x6F;
            }
        } else {
            stage -= 45;
            if (stage < 100) {
                if (stage == 0) {
                    g_BattleSummonNeoBahamutGroundMatrix.t[1] = 0;
                    g_BattleSummonNeoBahamutStageOffscreenRequest.mesh.vertexShade = 0;
                }
                if (g_BattleIsPaused == 0) {
                    g_BattleSummonNeoBahamutGroundMatrix.t[1] -= 0xC8;
                }
            } else {
                stage -= 100;
                if (stage < 320) {
                    if (stage == 0) {
                        g_BattleSummonNeoBahamutGroundMatrix.t[1] = 0;
                    }
                    if (stage >= 275) {
                        g_BattleSummonNeoBahamutGroundMatrix.t[0] = (stage & 1) != 0 ? 0x1E : -0x1E;
                    }
                }
            }
        }
        MultiplyMatrix(&g_BattleSummonNeoBahamutComposedMatrix, (short *)&g_BattleSummonNeoBahamutGroundMatrix, (void *)(g_BattleSummonNeoBahamutGroundTaskSlot + 8));
        SetRotMatrix((void *)(g_BattleSummonNeoBahamutGroundTaskSlot + 8));
        SetTransMatrix((void *)(g_BattleSummonNeoBahamutGroundTaskSlot + 8));
        SetDrawColorRGB(g_BattleFxTimelineState.red, g_BattleFxTimelineState.channel1, g_BattleFxTimelineState.channel2);
        RefreshGlobalTransformState();
        Gfx_SetPsxPrimQuadState(9, 4, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleNeoBahamutField4OffscreenSlot));
        Gfx_SetPsxPrimQuadState(9, 1, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleNeoBahamutField4OffscreenSlot));
        Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleNeoBahamutField12OffscreenSlot));
        Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleNeoBahamutField12OffscreenSlot));
        g_BattleOverlayDrawListHead = BattleFx_SummonNeoBahamutDrawPsxMesh(&g_BattleSummonNeoBahamutStageOffscreenRequest.mesh, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    } else {
        stage -= 465;
        if (stage < 75) {
            if (stage == 0) {
                BattleFx_SummonNeoBahamutInitFragmentTable();
            }
            if (stage < 30) {
                progress = (stage << 12) / 30;
                ((BattleSummonTaskRecord *)g_BattleSummonNeoBahamutGroundTaskSlot)->offsetX = progress * -0x40 >> 12;
                ((BattleSummonTaskRecord *)g_BattleSummonNeoBahamutGroundTaskSlot)->offsetY = 0;
                ((BattleSummonTaskRecord *)g_BattleSummonNeoBahamutGroundTaskSlot)->offsetZ = progress << 7 >> 12;
                RotMatrix((SVECTOR *)g_BattleSummonNeoBahamutGroundTaskSlot, (void *)(g_BattleSummonNeoBahamutGroundTaskSlot + 8));
                *(int *)((char *)g_BattleSummonNeoBahamutGroundTaskSlot + 0x1A) = (stage & 1) != 0 ? 0x32 : -0x32;
                *(int *)((char *)g_BattleSummonNeoBahamutGroundTaskSlot + 0x22) = stage * 0x1E;
                *(int *)((char *)g_BattleSummonNeoBahamutGroundTaskSlot + 0x1E) = *(int *)((char *)g_BattleSummonNeoBahamutGroundTaskSlot + 0x22);
                MultiplyMatrix(&g_BattleSummonNeoBahamutComposedMatrix, (void *)(g_BattleSummonNeoBahamutGroundTaskSlot + 8), (void *)(g_BattleSummonNeoBahamutGroundTaskSlot + 8));
            } else {
                if (stage == 30) {
                    g_BattleSummonNeoBahamutGroundMatrix.m[2][2] = 0x100;
                    g_BattleSummonNeoBahamutGroundMatrix.m[1][1] = g_BattleSummonNeoBahamutGroundMatrix.m[2][2];
                    g_BattleSummonNeoBahamutGroundMatrix.m[0][0] = g_BattleSummonNeoBahamutGroundMatrix.m[1][1];
                    g_BattleSummonNeoBahamutGroundMatrix.t[0] = 0xFFFFEC78;
                    g_BattleSummonNeoBahamutGroundMatrix.t[1] = 0xFFFFB1E0;
                    g_BattleSummonNeoBahamutGroundMatrix.t[2] = 0;
                }
                MultiplyMatrix(&g_BattleSummonNeoBahamutComposedMatrix, (short *)&g_BattleSummonNeoBahamutGroundMatrix, (void *)(g_BattleSummonNeoBahamutGroundTaskSlot + 8));
            }
            BattleFx_SummonNeoBahamutUpdateFragmentMatrices(g_BattleSummonNeoBahamutGroundTaskSlot + 8);
            Gfx_SetPsxPrimQuadState(9, 4, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleNeoBahamutField4OffscreenSlot));
            Gfx_SetPsxPrimQuadState(9, 1, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleNeoBahamutField4OffscreenSlot));
            Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleNeoBahamutField12OffscreenSlot));
            Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleNeoBahamutField12OffscreenSlot));
            g_BattleOverlayDrawListHead = BattleFx_SummonNeoBahamutDrawPsxMeshPerFaceMatrix((BattleFxPsxMeshResource *)&g_BattleSummonNeoBahamutStageOffscreenRequest.mesh, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead, (int)&g_BattleSummonNeoBahamutGroundVertexBlob);
        } else {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    if (g_BattleIsPaused == 0) {
        (taskPtr->nextIndex)++;
    }
}

// Draws a PSX mesh: its textured triangles, textured quads, gouraud triangles and gouraud quads, in
// that order. Mesh flag 0x20 draws back faces too, flag 0x80 greys the textured polygons by their
// record colour (shaded by mesh->vertexShade when set) and flag 8 makes them semi-transparent. The
// texture coordinates are halved unless g_BattleUiScalingMode is 2. Without flag 0x80 the textured
// triangles use tint before any assignment, as the original does. Returns the next free packet.
// FUNCTION: FF7 0x0048eb0b
int __cdecl BattleFx_SummonNeoBahamutDrawPsxMesh(BattleFxProceduralMesh *mesh, int orderingTable, int depthShift, int packetCursor)
{
    int padSlot; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    unsigned short texturePage;
    int origin;
    BattleFxPsxQuadPacket *workTriPrim;
    int tint[2];
    int supported;
    BattleFxPsxQuadPacket *tmpQuadPrim;
    int index;
    int numElements;
    BattleFxPsxTexturedTriPrim *texQuad;
    int stackHole; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int ptA;
    int ptB;
    int streamCnt;
    int vertC;
    int unsupportedFlags;
    BattleSummonBahamutMeshRecord *streamPtr;
    BattleFxPsxTexturedTriPrim *renderBlock;

    origin = mesh->meshData + 4;
    streamPtr = (BattleSummonBahamutMeshRecord *)(origin + *(int *)mesh->meshData);
    renderBlock = (BattleFxPsxTexturedTriPrim *)Gfx_GetPsxPacketScratchPtr();
    streamCnt = streamPtr->streamCnt;
    streamPtr = (BattleSummonBahamutMeshRecord *)((char *)streamPtr + 2);
    texturePage = streamPtr->streamCnt & 0x19F | mesh->attrBits;
    streamPtr = (BattleSummonBahamutMeshRecord *)((char *)streamPtr + 2);
    ptA = origin + streamPtr->streamCnt;
    ptB = origin + streamPtr->vertexBOffset;
    vertC = origin + streamPtr->vertexCOffset;
    for (index = 0; index < streamCnt; index++) {
        StoreDeferredDrawArgs((const void *)ptA, (const void *)ptB, (const void *)vertC);
        ApplyDeferredDrawArgs();
        ptA = origin + streamPtr->vertexAOffset;
        ptB = origin + streamPtr->tintB;
        vertC = origin + streamPtr->nextVertexOffset0;
        Gfx_PsxStoreClearFlag(&unsupportedFlags);
        if ((unsupportedFlags & 0x60000) == 0) {
            if ((mesh->flags & 0x20) != 0) {
                supported = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&supported);
            }
            if (supported > 0) {
                renderBlock->tag = 0x7000000;
                Gfx_ReadTransformOrderingDepth2(&numElements);
                renderBlock->primWord = streamPtr->primWord;
                renderBlock->uvWord0 = streamPtr->uvWord0;
                renderBlock->uvWord1 = streamPtr->uvWord1;
                renderBlock->texturePage = texturePage;
                if (g_BattleUiScalingMode != 2) {
                    renderBlock->texU0 >>= 1;
                    renderBlock->texV0 >>= 1;
                    renderBlock->texU1 >>= 1;
                    renderBlock->texV1 >>= 1;
                    renderBlock->texU2 >>= 1;
                    renderBlock->texV2 >>= 1;
                }
                if ((mesh->flags & 0x80) != 0) {
                    tint[0] = streamPtr->tintA & 0xFF;
                    tint[0] = tint[0] << 0x10 | 0x24000000 | tint[0] << 8 | tint[0];
                    if (mesh->vertexShade != 0) {
                        Gfx_ShadePsxVertexColor((CVECTOR *)tint, mesh->vertexShade, &renderBlock->colorRgb);
                        renderBlock->colorRgb.cd = 0x24;
                    } else {
                        renderBlock->color = tint[0];
                    }
                } else {
                    renderBlock->color = tint[0] << 0x10 | 0x24000000 | tint[0] << 8 | tint[0];
                }
                if ((mesh->flags & 8) != 0) {
                    renderBlock->colorRgb.cd |= 2;
                }
                Gfx_DrawPsxPrimitive(renderBlock);
            }
        }
        streamPtr = (BattleSummonBahamutMeshRecord *)((char *)streamPtr + 0x10);
    }
    streamCnt = streamPtr->streamCnt;
    streamPtr = (BattleSummonBahamutMeshRecord *)((char *)streamPtr + 4);
    texQuad = (BattleFxPsxTexturedTriPrim *)renderBlock;
    ptA = origin + streamPtr->streamCnt;
    ptB = origin + streamPtr->vertexBOffset;
    vertC = origin + streamPtr->vertexCOffset;
    for (index = 0; index < streamCnt; index++) {
        StoreDeferredDrawArgs((const void *)ptA, (const void *)ptB, (const void *)vertC);
        ApplyDeferredDrawArgs();
        ptA = origin + streamPtr->nextVertexOffset0;
        ptB = origin + streamPtr->nextVertexOffset1;
        vertC = origin + streamPtr->nextVertexOffset2;
        Gfx_PsxStoreClearFlag(&unsupportedFlags);
        if ((unsupportedFlags & 0x60000) == 0) {
            if ((mesh->flags & 0x20) != 0) {
                supported = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&supported);
            }
            if (supported >= 0) {
                SetPendingResourceHandle((GfxPsxShortVertexSlot *)(origin + streamPtr->tintA));
                ApplyPendingResourceHandle();
                Gfx_ReadTransformOrderingDepth2(&numElements);
                texQuad->tag = 0x9000000;
                texQuad->primWord = streamPtr->primWord;
                texQuad->uvWord0 = streamPtr->uvWord0;
                texQuad->uvWord1 = streamPtr->uvWord1;
                texQuad->uvWord2 = streamPtr->vertexAOffset;
                if (g_BattleUiScalingMode != 2) {
                    texQuad->texU0 >>= 1;
                    texQuad->texV0 >>= 1;
                    texQuad->texU1 >>= 1;
                    texQuad->texV1 >>= 1;
                    texQuad->texU2 >>= 1;
                    texQuad->texV2 >>= 1;
                    texQuad->texU3 >>= 1;
                    texQuad->texV3 >>= 1;
                }
                texQuad->texturePage = texturePage;
                if ((mesh->flags & 0x80) != 0) {
                    tint[0] = streamPtr->tintB & 0xFF;
                    tint[0] = tint[0] << 0x10 | 0x2C000000 | tint[0] << 8 | tint[0];
                    if (mesh->vertexShade != 0) {
                        Gfx_ShadePsxVertexColor((CVECTOR *)tint, mesh->vertexShade, &texQuad->colorRgb);
                        texQuad->colorRgb.cd = 0x2C;
                    } else {
                        texQuad->color = tint[0];
                    }
                } else {
                    tint[0] = mesh->vertexShade & 0xFF;
                    texQuad->color = tint[0] << 0x10 | 0x2C000000 | tint[0] << 8 | tint[0];
                }
                if ((mesh->flags & 8) != 0) {
                    texQuad->colorRgb.cd |= 2;
                }
                Gfx_DrawPsxPrimitive(texQuad);
            }
        }
        streamPtr = (BattleSummonBahamutMeshRecord *)((char *)streamPtr + 0x14);
    }
    streamCnt = streamPtr->streamCnt;
    streamPtr = (BattleSummonBahamutMeshRecord *)((char *)streamPtr + 4);
    workTriPrim = (BattleFxPsxQuadPacket *)texQuad;
    ptA = origin + streamPtr->streamCnt;
    ptB = origin + streamPtr->vertexBOffset;
    vertC = origin + streamPtr->vertexCOffset;
    for (index = 0; index < streamCnt; index++) {
        StoreDeferredDrawArgs((const void *)ptA, (const void *)ptB, (const void *)vertC);
        ApplyDeferredDrawArgs();
        ptA = origin + streamPtr->nextVertexOffset0;
        ptB = origin + streamPtr->nextVertexOffset1;
        vertC = origin + streamPtr->nextVertexOffset2;
        Gfx_PsxStoreClearFlag(&unsupportedFlags);
        if ((unsupportedFlags & 0x60000) == 0) {
            if ((mesh->flags & 0x20) != 0) {
                supported = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&supported);
            }
            if (supported >= 0) {
                workTriPrim->tag = 0x6000000;
                Gfx_ReadTransformOrderingDepth2(&numElements);
                if (mesh->vertexShade != 0) {
                    Gfx_ShadePsxTriVertexColors((CVECTOR *)((int)streamPtr + 8), (CVECTOR *)((int)streamPtr + 0xC), (CVECTOR *)((int)streamPtr + 0x10), mesh->vertexShade, (CVECTOR *)((int)workTriPrim + 4), (CVECTOR *)((int)workTriPrim + 0xC), (CVECTOR *)((int)workTriPrim + 0x14));
                } else {
                    workTriPrim->primWord = streamPtr->primWord;
                    workTriPrim->color1 = streamPtr->color1;
                    workTriPrim->color2 = streamPtr->color2;
                }
                if ((mesh->flags & 8) != 0) {
                    workTriPrim->primCode |= 2;
                }
                Gfx_DrawPsxPrimitive(workTriPrim);
            }
        }
        streamPtr = (BattleSummonBahamutMeshRecord *)((char *)streamPtr + 0x14);
    }
    streamCnt = streamPtr->streamCnt;
    streamPtr = (BattleSummonBahamutMeshRecord *)((char *)streamPtr + 4);
    tmpQuadPrim = (BattleFxPsxQuadPacket *)workTriPrim;
    ptA = origin + streamPtr->streamCnt;
    ptB = origin + streamPtr->vertexBOffset;
    vertC = origin + streamPtr->vertexCOffset;
    for (index = 0; index < streamCnt; index++) {
        StoreDeferredDrawArgs((const void *)ptA, (const void *)ptB, (const void *)vertC);
        ApplyDeferredDrawArgs();
        ptA = origin + streamPtr->nextVertexOffset2;
        ptB = origin + streamPtr->nextVertexOffset3;
        vertC = origin + streamPtr->nextVertexOffset4;
        Gfx_PsxStoreClearFlag(&unsupportedFlags);
        if ((unsupportedFlags & 0x60000) == 0) {
            if ((mesh->flags & 0x20) != 0) {
                supported = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&supported);
            }
            if (supported >= 0) {
                SetPendingResourceHandle((GfxPsxShortVertexSlot *)(origin + streamPtr->tintA));
                ApplyPendingResourceHandle();
                Gfx_ReadTransformOrderingDepth2(&numElements);
                tmpQuadPrim->tag = 0x8000000;
                if (mesh->vertexShade != 0) {
                    Gfx_ShadePsxTriVertexColors((CVECTOR *)((int)streamPtr + 8), (CVECTOR *)((int)streamPtr + 0xC), (CVECTOR *)((int)streamPtr + 0x10), mesh->vertexShade, (CVECTOR *)((int)tmpQuadPrim + 4), (CVECTOR *)((int)tmpQuadPrim + 0xC), (CVECTOR *)((int)tmpQuadPrim + 0x14));
                    Gfx_ShadePsxVertexColor((CVECTOR *)((int)streamPtr + 0x14), mesh->vertexShade, (CVECTOR *)((int)tmpQuadPrim + 0x1C));
                } else {
                    tmpQuadPrim->primWord = streamPtr->primWord;
                    tmpQuadPrim->color1 = streamPtr->color1;
                    tmpQuadPrim->color2 = streamPtr->color2;
                    tmpQuadPrim->color3 = streamPtr->color3;
                }
                if ((mesh->flags & 8) != 0) {
                    tmpQuadPrim->primCode |= 2;
                }
                Gfx_DrawPsxPrimitive(tmpQuadPrim);
            }
        }
        streamPtr = (BattleSummonBahamutMeshRecord *)((char *)streamPtr + 0x18);
    }
    return (int)tmpQuadPrim;
}

// Draws the textured triangles and quads of a PSX mesh, each face about its own centroid with the
// next matrix of the array at matrix, so the faces can fly apart. Faces are greyed by their record
// colour, and their texture coordinates are halved unless g_BattleUiScalingMode is 2. Arguments two
// to four are unused. Returns the next free packet.
// FUNCTION: FF7 0x0048f3ed
int __cdecl BattleFx_SummonNeoBahamutDrawPsxMeshPerFaceMatrix(BattleFxPsxMeshResource *resource, int unused2, int unused3, int unused4, int matrix)
{
    Vec3sPacked *vert0;
    int color;
    Vec3sPacked *vert3;
    int count;
    Vec3sPacked *vertC;
    int baseOfs;
    unsigned short texFlags;
    BattleFxPsxTexturedQuad *record;
    int faceIdx;
    BattleFxPsxPerFaceQuadPrim *primitive;
    BattleFxPsxPerFaceQuadPrim *quadPacket;
    Vec3sPacked *vertA;

    if (((unsigned char)g_BattleSummonNeoBahamutPsxMeshInitFlag & 1) == 0) {
        g_BattleSummonNeoBahamutPsxMeshInitFlag |= 1;
        g_BattleSummonNeoBahamutPsxMeshTaskSlot = (struct BattleFxNeoBahamutFaceSlot *)(Gfx_GetPsxScratchpadPtr(0xC0));
    }
    SetDrawColorRGB(0, 0, 0);
    baseOfs = resource->dataPtr + 4;
    record = (BattleFxPsxTexturedQuad *)(baseOfs + *(int *)resource->dataPtr);
    primitive = (BattleFxPsxPerFaceQuadPrim *)Gfx_GetPsxPacketScratchPtr();
    count = record->flags;
    record = (BattleFxPsxTexturedQuad *)((char *)record + 2);
    texFlags = record->flags & 0x19F | resource->textureFlags;
    record = (BattleFxPsxTexturedQuad *)((char *)record + 2);
    vert0 = (Vec3sPacked *)(baseOfs + record->flags);
    vertA = (Vec3sPacked *)(baseOfs + record->vertex1);
    vertC = (Vec3sPacked *)(baseOfs + record->vertex2);
    for (faceIdx = 0; faceIdx < count; faceIdx++) {
        g_BattleSummonNeoBahamutPsxMeshTaskSlot->centerX = (vert0->vx + vertA->vx + vertC->vx) / 3;
        g_BattleSummonNeoBahamutPsxMeshTaskSlot->centerY = (vert0->vy + vertA->vy + vertC->vy) / 3;
        g_BattleSummonNeoBahamutPsxMeshTaskSlot->centerZ = (vert0->vz + vertA->vz + vertC->vz) / 3;
        g_BattleSummonNeoBahamutPsxMeshTaskSlot->pRelX = vert0->vx - g_BattleSummonNeoBahamutPsxMeshTaskSlot->centerX;
        g_BattleSummonNeoBahamutPsxMeshTaskSlot->pRelY = vert0->vy - g_BattleSummonNeoBahamutPsxMeshTaskSlot->centerY;
        g_BattleSummonNeoBahamutPsxMeshTaskSlot->pRelZ = vert0->vz - g_BattleSummonNeoBahamutPsxMeshTaskSlot->centerZ;
        g_BattleSummonNeoBahamutPsxMeshTaskSlot->aRelX = vertA->vx - g_BattleSummonNeoBahamutPsxMeshTaskSlot->centerX;
        g_BattleSummonNeoBahamutPsxMeshTaskSlot->aRelY = vertA->vy - g_BattleSummonNeoBahamutPsxMeshTaskSlot->centerY;
        g_BattleSummonNeoBahamutPsxMeshTaskSlot->aRelZ = vertA->vz - g_BattleSummonNeoBahamutPsxMeshTaskSlot->centerZ;
        g_BattleSummonNeoBahamutPsxMeshTaskSlot->sRelX = vertC->vx - g_BattleSummonNeoBahamutPsxMeshTaskSlot->centerX;
        g_BattleSummonNeoBahamutPsxMeshTaskSlot->sRelY = vertC->vy - g_BattleSummonNeoBahamutPsxMeshTaskSlot->centerY;
        g_BattleSummonNeoBahamutPsxMeshTaskSlot->sRelZ = vertC->vz - g_BattleSummonNeoBahamutPsxMeshTaskSlot->centerZ;
        SetRotMatrix((void *)matrix);
        SetTransMatrix((void *)matrix);
        RefreshGlobalTransformState();
        matrix += 0x1E;
        StoreDeferredDrawArgs(g_BattleSummonNeoBahamutPsxMeshTaskSlot, (char *)g_BattleSummonNeoBahamutPsxMeshTaskSlot + 8, (char *)g_BattleSummonNeoBahamutPsxMeshTaskSlot + 0x10);
        ApplyDeferredDrawArgs();
        vert0 = (Vec3sPacked *)(baseOfs + record->offsetC);
        vertA = (Vec3sPacked *)(baseOfs + record->offsetD);
        vertC = (Vec3sPacked *)(baseOfs + record[1].flags);
        Gfx_PsxStoreClearFlag(&g_BattleSummonNeoBahamutPsxMeshTaskSlot->flags);
        if ((g_BattleSummonNeoBahamutPsxMeshTaskSlot->flags & 0x60000) == 0) {
            primitive->tag = 0x7000000;
            Gfx_ReadTransformOrderingDepth2(&g_BattleSummonNeoBahamutPsxMeshTaskSlot->depth);
            primitive->textureFlags = texFlags;
            color = record->vertex3 & 0xFF;
            color = color | color << 8 | color << 16;
            primitive->color = color | 0x24000000;
            primitive->colorWord = record->colorWord;
            primitive->uvWord0 = record->uvWord0;
            primitive->uvWord1 = record->uvWord1;
            if (g_BattleUiScalingMode != 2) {
                primitive->texU0 >>= 1;
                primitive->texV0 >>= 1;
                primitive->texU1 >>= 1;
                primitive->texV1 >>= 1;
                primitive->texU2 >>= 1;
                primitive->texV2 >>= 1;
            }
            Gfx_DrawPsxPrimitive(primitive);
        }
        record = (BattleFxPsxTexturedQuad *)((char *)record + 0x10);
    }
    quadPacket = (BattleFxPsxPerFaceQuadPrim *)primitive;
    count = record->flags;
    record = (BattleFxPsxTexturedQuad *)((char *)record + 4);
    vert0 = (Vec3sPacked *)(baseOfs + record->flags);
    vertA = (Vec3sPacked *)(baseOfs + record->vertex1);
    vertC = (Vec3sPacked *)(baseOfs + record->vertex2);
    vert3 = (Vec3sPacked *)(baseOfs + record->vertex3);
    for (faceIdx = 0; faceIdx < count; faceIdx++) {
        g_BattleSummonNeoBahamutPsxMeshTaskSlot->centerX = (vert0->vx + vertA->vx + vertC->vx + vert3->vx) / 4;
        g_BattleSummonNeoBahamutPsxMeshTaskSlot->centerY = (vert0->vy + vertA->vy + vertC->vy + vert3->vy) / 4;
        g_BattleSummonNeoBahamutPsxMeshTaskSlot->centerZ = (vert0->vz + vertA->vz + vertC->vz + vert3->vz) / 4;
        g_BattleSummonNeoBahamutPsxMeshTaskSlot->pRelX = vert0->vx - g_BattleSummonNeoBahamutPsxMeshTaskSlot->centerX;
        g_BattleSummonNeoBahamutPsxMeshTaskSlot->pRelY = vert0->vy - g_BattleSummonNeoBahamutPsxMeshTaskSlot->centerY;
        g_BattleSummonNeoBahamutPsxMeshTaskSlot->pRelZ = vert0->vz - g_BattleSummonNeoBahamutPsxMeshTaskSlot->centerZ;
        g_BattleSummonNeoBahamutPsxMeshTaskSlot->aRelX = vertA->vx - g_BattleSummonNeoBahamutPsxMeshTaskSlot->centerX;
        g_BattleSummonNeoBahamutPsxMeshTaskSlot->aRelY = vertA->vy - g_BattleSummonNeoBahamutPsxMeshTaskSlot->centerY;
        g_BattleSummonNeoBahamutPsxMeshTaskSlot->aRelZ = vertA->vz - g_BattleSummonNeoBahamutPsxMeshTaskSlot->centerZ;
        g_BattleSummonNeoBahamutPsxMeshTaskSlot->sRelX = vertC->vx - g_BattleSummonNeoBahamutPsxMeshTaskSlot->centerX;
        g_BattleSummonNeoBahamutPsxMeshTaskSlot->sRelY = vertC->vy - g_BattleSummonNeoBahamutPsxMeshTaskSlot->centerY;
        g_BattleSummonNeoBahamutPsxMeshTaskSlot->sRelZ = vertC->vz - g_BattleSummonNeoBahamutPsxMeshTaskSlot->centerZ;
        g_BattleSummonNeoBahamutPsxMeshTaskSlot->uRelX = vert3->vx - g_BattleSummonNeoBahamutPsxMeshTaskSlot->centerX;
        g_BattleSummonNeoBahamutPsxMeshTaskSlot->uRelY = vert3->vy - g_BattleSummonNeoBahamutPsxMeshTaskSlot->centerY;
        g_BattleSummonNeoBahamutPsxMeshTaskSlot->uRelZ = vert3->vz - g_BattleSummonNeoBahamutPsxMeshTaskSlot->centerZ;
        SetRotMatrix((void *)matrix);
        SetTransMatrix((void *)matrix);
        RefreshGlobalTransformState();
        matrix += 0x1E;
        StoreDeferredDrawArgs(g_BattleSummonNeoBahamutPsxMeshTaskSlot, (char *)g_BattleSummonNeoBahamutPsxMeshTaskSlot + 8, (char *)g_BattleSummonNeoBahamutPsxMeshTaskSlot + 0x10);
        ApplyDeferredDrawArgs();
        vert0 = (Vec3sPacked *)(baseOfs + record[1].flags);
        vertA = (Vec3sPacked *)(baseOfs + record[1].vertex1);
        vertC = (Vec3sPacked *)(baseOfs + record[1].vertex2);
        vert3 = (Vec3sPacked *)(baseOfs + record[1].vertex3);
        Gfx_PsxStoreClearFlag(&g_BattleSummonNeoBahamutPsxMeshTaskSlot->flags);
        if ((g_BattleSummonNeoBahamutPsxMeshTaskSlot->flags & 0x60000) == 0) {
            SetPendingResourceHandle((GfxPsxShortVertexSlot *)(((int)g_BattleSummonNeoBahamutPsxMeshTaskSlot) + 0x18));
            ApplyPendingResourceHandle();
            quadPacket->tag = 0x9000000;
            quadPacket->textureFlags = texFlags;
            color = record->offsetD & 0xFF;
            color = color | color << 8 | color << 16;
            quadPacket->color = color | 0x2C000000;
            quadPacket->colorWord = record->colorWord;
            quadPacket->uvWord0 = record->uvWord0;
            quadPacket->uvWord1 = record->uvWord1;
            quadPacket->offsetC = record->offsetC;
            if (g_BattleUiScalingMode != 2) {
                quadPacket->texU0 >>= 1;
                quadPacket->texV0 >>= 1;
                quadPacket->texU1 >>= 1;
                quadPacket->texV1 >>= 1;
                quadPacket->texU2 >>= 1;
                quadPacket->texV2 >>= 1;
                quadPacket->texU3 >>= 1;
                quadPacket->texV3 >>= 1;
            }
            Gfx_ReadTransformOrderingDepth2(&g_BattleSummonNeoBahamutPsxMeshTaskSlot->depth);
            Gfx_DrawPsxPrimitive(quadPacket);
        }
        record++;
    }
    return (int)quadPacket;
}

// Initializes consecutive 0x1C-byte records at g_BattleSummonNeoBahamutPsxMeshScratch from triangle then quad streams rooted
// at four bytes into g_BattleSummonNeoBahamutPolygonVertexBlob. Uses signed-short counts and signed vertex offsets with
// sixteen/twenty-byte topology strides. Each record saves narrowed average XYZ, random scale
// factor 0x200 through 0x2FF, X velocity from centroid X times factor shifted twelve, Y velocity
// from centroid Y minus 1000 times that factor and independent Z velocity random zero through 99.
// Sets first-short delay from (centroid Z plus 4500) times 45 divided by 9000 and clears short two
// and three rotation shorts. No pause check appears.
// FUNCTION: FF7 0x0048fc3e
void __cdecl BattleFx_SummonNeoBahamutInitFragmentTable(void)
{
    Vec3sPacked *vertP;
    int vertBase;
    int triZ;
    int triValue;
    BattleFxNeoBahamutFragment *pFragment;
    int unusedWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int i;
    BattleFxNeoBahamutFragmentHeader *pHeader;
    int numberFaces;
    int quadSpeed;
    int quadDepthVel;
    Vec3sPacked *vertA;
    Vec3sPacked *vertB;
    Vec3sPacked *pVertexC;
    int speedScale;

    vertBase = (int)((char *)&g_BattleSummonNeoBahamutPolygonVertexBlob + 0x4);
    pHeader = (BattleFxNeoBahamutFragmentHeader *)(vertBase + (*(int *)&g_BattleSummonNeoBahamutPolygonVertexBlob));
    pFragment = (BattleFxNeoBahamutFragment *)&g_BattleSummonNeoBahamutPsxMeshScratch;
    numberFaces = pHeader->vertexPOffset;
    pHeader = (BattleFxNeoBahamutFragmentHeader *)((char *)pHeader + 4);
    for (i = 0; i < numberFaces; i++) {
        vertP = (Vec3sPacked *)(vertBase + pHeader->vertexPOffset);
        vertA = (Vec3sPacked *)(vertBase + pHeader->vertexAOffset);
        vertB = (Vec3sPacked *)(vertBase + pHeader->vertexBOffset);
        pFragment->posX = (vertP->vx + vertA->vx + vertB->vx) / 3;
        pFragment->posY = (vertP->vy + vertA->vy + vertB->vy) / 3;
        pFragment->posZ = (vertP->vz + vertA->vz + vertB->vz) / 3;
        if (DEAD_IN_RETAIL != 0) {
            triValue = 0x100;
        } else {
            triValue = rand() % 256 + 512;
        }
        speedScale = triValue;
        pFragment->velocityX = pFragment->posX * speedScale >> 12;
        pFragment->velocityY = (pFragment->posY - 1000) * speedScale >> 12;
        if (DEAD_IN_RETAIL != 0) {
            triZ = 100;
        } else {
            triZ = rand() % 100;
        }
        pFragment->velocityZ = triZ;
        pFragment->timer = (pFragment->posZ + 4500) * 45 / 9000;
        pFragment->clearedWord = 0;
        pFragment->angleX = 0;
        pFragment->angleY = 0;
        pFragment->angleZ = 0;
        pFragment++;
        pHeader += 2;
    }
    numberFaces = pHeader->vertexPOffset;
    pHeader = (BattleFxNeoBahamutFragmentHeader *)((char *)pHeader + 4);
    for (i = 0; i < numberFaces; i++) {
        vertP = (Vec3sPacked *)(vertBase + pHeader->vertexPOffset);
        vertA = (Vec3sPacked *)(vertBase + pHeader->vertexAOffset);
        vertB = (Vec3sPacked *)(vertBase + pHeader->vertexBOffset);
        pVertexC = (Vec3sPacked *)(vertBase + pHeader->vertexCOffset);
        pFragment->posX = (vertP->vx + vertA->vx + vertB->vx + pVertexC->vx) / 4;
        pFragment->posY = (vertP->vy + vertA->vy + vertB->vy + pVertexC->vy) / 4;
        pFragment->posZ = (vertP->vz + vertA->vz + vertB->vz + pVertexC->vz) / 4;
        if (DEAD_IN_RETAIL != 0) {
            quadSpeed = 256;
        } else {
            quadSpeed = rand() % 256 + 512;
        }
        speedScale = quadSpeed;
        pFragment->velocityX = pFragment->posX * speedScale >> 12;
        pFragment->velocityY = (pFragment->posY - 1000) * speedScale >> 12;
        if (DEAD_IN_RETAIL != 0) {
            quadDepthVel = 100;
        } else {
            quadDepthVel = rand() % 100;
        }
        pFragment->velocityZ = quadDepthVel;
        pFragment->timer = (pFragment->posZ + 4500) * 45 / 9000;
        pFragment->clearedWord = 0;
        pFragment->angleX = 0;
        pFragment->angleY = 0;
        pFragment->angleZ = 0;
        pFragment++;
        pHeader = (BattleFxNeoBahamutFragmentHeader *)((char *)pHeader + 0x14);
    }
}

// Acquires retained 0xC0 scratch and visits exactly 434 records at
// g_BattleSummonNeoBahamutPsxMeshScratch with stride 0x1C, writing thirty-byte matrices at
// g_BattleSummonNeoBahamutGroundVertexBlob. Unpaused positive delay decrements without
// movement; otherwise if record Z is below 25000 advances XYZ by velocity, damps all velocities by
// 0xF80 shifted twelve with Z plus fifty and adds rotation increments 0x12/0x234/0x456. At Z at
// least 25000 clears only the output matrix nine rotation shorts, retaining translation, and skips
// normal composition. Other cases, including paused records, build rotation and centroid
// translation and multiply the supplied matrix into output. No task retirement occurs.
// Paused rebuilding can replace a previously cleared rotation.
// FUNCTION: FF7 0x0048fff3
void __cdecl BattleFx_SummonNeoBahamutUpdateFragmentMatrices(int matrix)
{
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int i;
    struct BattleFxNeoBahamutShardMatrixBuf *matrixOut;
    BattleFxNeoBahamutFragment *fragment;
    int padField; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    if (((unsigned char)g_BattleSummonNeoBahamutFragmentUpdateInitFlag & 1) == 0) {
        g_BattleSummonNeoBahamutFragmentUpdateInitFlag |= 1;
        g_BattleSummonNeoBahamutFragmentMatrixTaskSlot = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0xC0);
    }
    fragment = (BattleFxNeoBahamutFragment *)&g_BattleSummonNeoBahamutPsxMeshScratch;
    matrixOut = g_BattleSummonNeoBahamutGroundVertexBlob;
    for (i = 0; i < 434; i++) {
        if (g_BattleIsPaused == 0) {
            if (fragment->timer > 0) {
                (fragment->timer)--;
            } else if (fragment->posZ < 25000) {
                fragment->posX += fragment->velocityX;
                fragment->posY += fragment->velocityY;
                fragment->posZ += fragment->velocityZ;
                fragment->velocityX = fragment->velocityX * 0xF80 >> 12;
                fragment->velocityY = fragment->velocityY * 0xF80 >> 12;
                fragment->velocityZ = (fragment->velocityZ * 0xF80 >> 12) + 50;
                fragment->angleX += 0x12;
                fragment->angleY += 0x234;
                fragment->angleZ += 0x456;
            } else {
                matrixOut->m22 = 0;
                matrixOut->m21 = 0;
                matrixOut->m20 = 0;
                matrixOut->m12 = 0;
                matrixOut->m11 = 0;
                matrixOut->m10 = 0;
                matrixOut->m02 = 0;
                matrixOut->m01 = 0;
                matrixOut->m00 = 0;
                fragment++;
                matrixOut = (struct BattleFxNeoBahamutShardMatrixBuf *)((char *)matrixOut + 0x1E);
                continue;
            }
        }
        RotMatrix((SVECTOR *)&fragment->angleX, (void *)g_BattleSummonNeoBahamutFragmentMatrixTaskSlot);
        g_BattleSummonNeoBahamutFragmentMatrixTaskSlot->t[0] = fragment->posX;
        g_BattleSummonNeoBahamutFragmentMatrixTaskSlot->t[1] = fragment->posY;
        g_BattleSummonNeoBahamutFragmentMatrixTaskSlot->t[2] = fragment->posZ;
        MultiplyMatrix((void *)matrix, g_BattleSummonNeoBahamutFragmentMatrixTaskSlot, matrixOut);
        fragment++;
        matrixOut = (struct BattleFxNeoBahamutShardMatrixBuf *)((char *)matrixOut + 0x1E);
    }
}

// Returns while paused. First twenty counts ramp emission term from 128 toward 256, then hold 256
// for 25; later retire and return. Adds the low byte of term times total count to the term and
// emits BattleFx_SummonNeoBahamutThunderSpriteTask while at least 256, discarding remainder. Sets
// each record first short to the parent count, fills its position through
// BattleFx_SummonNeoBahamutMakeRandomLoopPoint and saves random size 0x3000 through 0x3FFF at 0x18.
// Advances count.
// FUNCTION: FF7 0x00490234
void __cdecl BattleFx_SummonNeoBahamutSpawnThunderSpritesTask(void)
{
    int stage;
    BattleFxBurstSpawnerTask *taskPtr;
    int progress;
    int emitAccum;
    BattleFxNeoBahamutThunderSprite *pSprite;
    int bytes;

    taskPtr = (BattleFxBurstSpawnerTask *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    stage = taskPtr->spawnIndex;
    if (stage < 20) {
        progress = (stage << 12) / 20;
        emitAccum = (progress << 7 >> 12) + 0x80;
    } else {
        stage -= 20;
        if (stage < 25) {
            emitAccum = 0x100;
        } else {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    emitAccum = (emitAccum * taskPtr->spawnIndex & 0xFF) + emitAccum;
    while (emitAccum >= 256) {
        pSprite = (BattleFxNeoBahamutThunderSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutThunderSpriteTask);
        pSprite->spawnIndex = taskPtr->spawnIndex;
        BattleFx_SummonNeoBahamutMakeRandomLoopPoint((Vec3sPacked *)((int)pSprite + 4));
        if (DEAD_IN_RETAIL != 0) {
            bytes = 0x4000;
        } else {
            bytes = (rand() % 4096) + 12288;
        }
        pSprite->scale = bytes;
        emitAccum -= 0x100;
    }
    (taskPtr->spawnIndex)++;
}

// Selects sprite fields sixteen and nineteen and installs a matrix through BattleFx_SummonNeoBahamutInstallBillboardMatrix using slot
// scale and third argument -1000. With positive matrix Z copies the counter into both requests and
// draws them; when unpaused advances only with a nonnull first sprite, retires at its header count
// before the final check, and otherwise retires when counter plus first slot word reaches forty-
// five.
// FUNCTION: FF7 0x0049036d
void __cdecl BattleFx_SummonNeoBahamutThunderSpriteTask(void)
{
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxNeoBahamutThunderSlot *task;
    int matrix;

    task = (BattleFxNeoBahamutThunderSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_NeoBahamutThunderSpriteDrawRequest1.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(16, g_BattleSummonNeoBahamutLoadedResTable);
    g_NeoBahamutThunderSpriteDrawRequest1.drawObj = Gfx_ModelTableGetSpriteObject(16, g_BattleSummonNeoBahamutLoadedResTable);
    g_NeoBahamutThunderSpriteDrawRequest2.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(19, g_BattleSummonNeoBahamutLoadedResTable);
    g_NeoBahamutThunderSpriteDrawRequest2.drawObj = Gfx_ModelTableGetSpriteObject(19, g_BattleSummonNeoBahamutLoadedResTable);
    matrix = BattleFx_SummonNeoBahamutInstallBillboardMatrix(&task->posX, (short)task->scale, 0xFFFFFC18);
    if (((PsxMatrix *)matrix)->t[2] > 0) {
        g_NeoBahamutThunderSpriteDrawRequest2.frameIndex = task->frameIndex;
        g_NeoBahamutThunderSpriteDrawRequest1.frameIndex = g_NeoBahamutThunderSpriteDrawRequest2.frameIndex;
        g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_NeoBahamutThunderSpriteDrawRequest1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
        g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_NeoBahamutThunderSpriteDrawRequest2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    }
    if (g_BattleIsPaused == 0) {
        if (g_NeoBahamutThunderSpriteDrawRequest1.spriteGroup != 0) {
            (task->frameIndex)++;
            if (task->frameIndex >= g_NeoBahamutThunderSpriteDrawRequest1.spriteGroup->header->frameCount) {
                task->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
        if (task->frameIndex + task->ownerSlot >= 45) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

// Copies scale into the three diagonal entries of g_BattleSummonNeoBahamutBillboardMatrix,
// installs g_BattleSummonNeoBahamutComposedMatrix as rotation
// and translation, and RotTransPoint transforms point into the matrix's translation. When
// offset is nonzero it normalises that vector with NormalizeVectorFixed and adds unit * offset >>
// 0xC to the translation. Then installs g_BattleSummonNeoBahamutBillboardMatrix as rotation and
// translation and returns its address.
// FUNCTION: FF7 0x004904cd
int __cdecl BattleFx_SummonNeoBahamutInstallBillboardMatrix(void *point, short scale, int offset)
{
    int transformed;
    VECTOR nx;

    g_BattleSummonNeoBahamutBillboardMatrix.m[2][2] = scale;
    g_BattleSummonNeoBahamutBillboardMatrix.m[1][1] = g_BattleSummonNeoBahamutBillboardMatrix.m[2][2];
    g_BattleSummonNeoBahamutBillboardMatrix.m[0][0] = g_BattleSummonNeoBahamutBillboardMatrix.m[1][1];
    SetRotMatrix(&g_BattleSummonNeoBahamutComposedMatrix);
    SetTransMatrix(&g_BattleSummonNeoBahamutComposedMatrix);
    RotTransPoint(point, g_BattleSummonNeoBahamutBillboardMatrix.t, &transformed);
    if (offset != 0) {
        NormalizeVectorFixed(g_BattleSummonNeoBahamutBillboardMatrix.t, &nx);
        g_BattleSummonNeoBahamutBillboardMatrix.t[0] += nx.vx * offset >> 12;
        g_BattleSummonNeoBahamutBillboardMatrix.t[1] += nx.vy * offset >> 12;
        g_BattleSummonNeoBahamutBillboardMatrix.t[2] += nx.vz * offset >> 12;
    }
    SetRotMatrix((short *)&g_BattleSummonNeoBahamutBillboardMatrix);
    SetTransMatrix((short *)&g_BattleSummonNeoBahamutBillboardMatrix);
    return (int)&g_BattleSummonNeoBahamutBillboardMatrix;
}

// Writes three interpolated shorts to outPoint+0/+2/+4. Picks a random row i = rand() % 24 of the
// 8-byte-stride short table at 0x188 into g_BattleSummonNeoBahamutPolygonVertexBlob (past its 4-byte header) and the next row (i+1) % 24, then blends each of
// the first three shorts of the two rows with a random 12-bit weight t = rand() & 0xFFF: (a*t +
// b*(0x1000-t)) >> 12. No return value.
// FUNCTION: FF7 0x004905a8
void __cdecl BattleFx_SummonNeoBahamutMakeRandomLoopPoint(Vec3sPacked *outPoint)
{
    Vec3sPacked *rowA;
    int idx;
    Vec3sPacked *rowB;
    int base;

    base = (int)((char *)&g_BattleSummonNeoBahamutPolygonVertexBlob + 0x4);
    idx = rand() % 24;
    rowA = (Vec3sPacked *)(base + 0x188 + idx * 8);
    rowB = (Vec3sPacked *)(base + 0x188 + (idx + 1) % 24 * 8);
    idx = rand() & 0xFFF;
    outPoint->vx = (rowA->vx * idx + rowB->vx * (4096 - idx)) >> 12;
    outPoint->vy = (rowA->vy * idx + rowB->vy * (4096 - idx)) >> 12;
    outPoint->vz = (rowA->vz * idx + rowB->vz * (4096 - idx)) >> 12;
}

// Per-frame spawner for the dust effect slot, which does nothing while the battle is paused. A
// budget ((spawnIndex << 10) & 0xFF) + 0x400 is spent 0x100 at a time while it is at least 0x100.
// Each unit allocates a resource slot for BattleFx_SummonNeoBahamutDustSpriteTask, seeds its loop
// point with BattleFx_SummonNeoBahamutMakeRandomLoopPoint at slot+4 and writes rand() % 12288 +
// 0x5000 to the short at slot+0x18. The DEAD_IN_RETAIL branch is dead in retail; it would write
// 0x8000 instead. Then spawnIndex advances, and the slot is retired (ownerSlot = BATTLE_TASK_FREE) once
// spawnIndex reaches 4.
// FUNCTION: FF7 0x00490679
void __cdecl BattleFx_SummonNeoBahamutSpawnDustSpritesTask(void)
{
    BattleFxBurstSpawnerTask *task;
    struct BattleTaskSlot *dust;
    int budget;
    int bytes;

    task = (BattleFxBurstSpawnerTask *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    budget = ((task->spawnIndex << 10) & 0xFF) + 0x400;
    while (budget >= 256) {
        dust = BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutDustSpriteTask);
        BattleFx_SummonNeoBahamutMakeRandomLoopPoint((Vec3sPacked *)dust->data);
        if (DEAD_IN_RETAIL) {
            bytes = 0x8000;
        } else {
            bytes = rand() % 12288 + 20480;
        }
        ((BattleFxDeathForceAnimSprite *)dust)->scale = bytes;
        budget -= 0x100;
    }
    (task->spawnIndex)++;
    if (task->spawnIndex >= 4) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Per-frame handler for the dust sprite effect in the current slot. It sets the request's resource
// from row 0x12 of g_BattleSummonNeoBahamutLoadedResTable and its scene field from
// Battle3d_GetSceneRenderObject(1, g_pBattle3dScene). It installs the billboard matrix for the slot's
// position with the short scale, writes the frame with bit 0x8000 set, and submits the sprite into
// g_BattleOverlayDrawListHead through BattleFx_SubmitSpriteDraw. With a nonnull resource it
// advances the frame and retires the slot (ownerSlot = BATTLE_TASK_FREE) once the frame reaches the count at
// +4 of that resource. Unlike the spawner, it has no pause guard.
// FUNCTION: FF7 0x00490755
void __cdecl BattleFx_SummonNeoBahamutDustSpriteTask(void)
{
    BattleFxDeathForceAnimSprite *task;

    task = (BattleFxDeathForceAnimSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSummonNeoBahamutDustSpriteDrawRequest.animTable = Gfx_ModelTableGetLoadedRecord(18, g_BattleSummonNeoBahamutLoadedResTable);
    g_BattleSummonNeoBahamutDustSpriteDrawRequest.sceneField = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    BattleFx_SummonNeoBahamutInstallBillboardMatrix(&task->posX, (short)task->scale, 0);
    g_BattleSummonNeoBahamutDustSpriteDrawRequest.frame = task->frame | 0x8000;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSummonNeoBahamutDustSpriteDrawRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleSummonNeoBahamutDustSpriteDrawRequest.animTable != 0) {
        (task->frame)++;
        if (task->frame >= *(int *)((char *)*(int *)g_BattleSummonNeoBahamutDustSpriteDrawRequest.animTable + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

// Acquires retained scratch before pause. Unpaused first 25 counts emit five
// BattleFx_SummonNeoBahamutRock1ModelTask records with base Y count times minus 111 and phase short
// zero; next twenty only advance count/return; final 75 emit one with base Y elapsed times minus
// 200 and phase short 0x200; later retire and return. Each gets variant zero through two, an
// initial random angle for radius-4000 X/Z, then a second random low-twelve-bit value used both as
// radial interpolation fraction and as the angle for horizontal velocity. Position Y is base plus
// (4096 minus fraction) times 4000 shifted twelve; velocity magnitude is random zero through 99
// with Y zero. Saves size 256 through 511, low-24-bit random colour and phase short. Ordinary calls
// advance count.
// FUNCTION: FF7 0x0049081f
void __cdecl BattleFx_SummonNeoBahamutSpawnRock1ModelsTask(void)
{
    int stage;
    BattleTaskSlot *taskPtr;
    int phaseShort;
    int index;
    int unusedEntry; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int velocityMagnitude;
    int newCount;
    int baseY;
    BattleFxNeoBahamutRock *pRockTask;
    int sizeRand;
    int randVel;
    int fraction;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if (((unsigned char)g_BattleSummonNeoBahamutRock1SpawnInitFlag & 1) == 0) {
        g_BattleSummonNeoBahamutRock1SpawnInitFlag |= 1;
        g_BattleSummonNeoBahamutRock1SpawnTaskSlot = (BattleFxShortOffsetVec *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    stage = taskPtr->frameCounter;
    if (stage < 25) {
        baseY = stage * -0x6F;
        newCount = 5;
        phaseShort = 0;
    } else {
        stage -= 25;
        if (stage < 20) {
            (taskPtr->frameCounter)++;
            return;
        }
        stage -= 20;
        if (stage < 75) {
            baseY = stage * -0xC8;
            newCount = 1;
            phaseShort = 0x200;
        } else {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    for (index = 0; index < newCount; index++) {
        pRockTask = (BattleFxNeoBahamutRock *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutRock1ModelTask);
        pRockTask->resourceIndex = rand() % 3;
        fraction = rand() & 0xFFF;
        g_BattleSummonNeoBahamutRock1SpawnTaskSlot->offsetX = SinFixed(fraction) * 0xFA0 >> 12;
        g_BattleSummonNeoBahamutRock1SpawnTaskSlot->offsetZ = CosFixed(fraction) * 0xFA0 >> 12;
        fraction = rand() & 0xFFF;
        pRockTask->posX = g_BattleSummonNeoBahamutRock1SpawnTaskSlot->offsetX * fraction >> 12;
        pRockTask->posY = baseY + ((4096 - fraction) * 0xFA0 >> 12);
        pRockTask->posZ = g_BattleSummonNeoBahamutRock1SpawnTaskSlot->offsetZ * fraction >> 12;
        if (DEAD_IN_RETAIL != 0) {
            randVel = 100;
        } else {
            randVel = rand() % 100;
        }
        velocityMagnitude = randVel;
        pRockTask->velocityX = velocityMagnitude * SinFixed(fraction) >> 12;
        pRockTask->velocityY = 0;
        pRockTask->velocityZ = velocityMagnitude * CosFixed(fraction) >> 12;
        if (DEAD_IN_RETAIL != 0) {
            sizeRand = 512;
        } else {
            sizeRand = rand() % 256 + 256;
        }
        pRockTask->scale = sizeRand;
        pRockTask->randomAngles = rand() & 0xFFFFFF;
        pRockTask->alpha = phaseShort;
    }
    (taskPtr->frameCounter)++;
}

// Acquires retained scratch. First twenty counts use descriptor flags 0x80/term zero and variant-
// indexed resource from g_BattleNeoBahamutPrimaryResourceIds; next five use flags 0x88, a rising fraction and
// g_BattleNeoBahamutPrimaryResourceIds.fadeResourceIds; later retire before drawing/pause. Optional task short 0x1A adjusts term as 4096
// minus remaining fraction times 4096 minus phase shifted twelve. Builds rotation directly from
// three unsigned bytes at 0x14 through 0x16, task XYZ translation and uniform saved size,
// multiplies g_BattleSummonNeoBahamutComposedMatrix and submits black into g_BattleCurrentOrderingTable plus 0x70 with shift twelve. While
// unpaused advances count and X/Y; if new Y is positive retires and skips the remaining update,
// otherwise advances Z, adds fifty to Y velocity and increments rotation bytes by eight/0x20/0x30.
// FUNCTION: FF7 0x00490abc
void __cdecl BattleFx_SummonNeoBahamutRock1ModelTask(void)
{
    int stage;
    int progress;
    BattleFxNeoBahamutRock *rockPtr;

    rockPtr = (BattleFxNeoBahamutRock *)BATTLE_CURRENT_TASK_SLOT;
    if (((unsigned char)g_BattleSummonNeoBahamutRock1InitFlag & 1) == 0) {
        g_BattleSummonNeoBahamutRock1InitFlag |= 1;
        g_BattleSummonNeoBahamutRock1TaskSlot = (BattleFxShardMeshRotation *)Gfx_GetPsxScratchpadPtr(0);
    }
    stage = rockPtr->frame;
    if (stage < 20) {
        g_BattleNeoBahamutParticleDrawRequest.flags = 0x80;
        g_BattleNeoBahamutParticleDrawRequest.fadeLevel = 0;
        g_BattleNeoBahamutParticleDrawRequest.modelGroup = Gfx_ModelTableGetRenderObject(((int *)&g_BattleNeoBahamutPrimaryResourceIds)[rockPtr->resourceIndex], g_BattleSummonNeoBahamutLoadedResTable);
    } else {
        stage -= 20;
        if (stage < 5) {
            progress = (stage << 12) / 5;
            g_BattleNeoBahamutParticleDrawRequest.flags = 0x88;
            g_BattleNeoBahamutParticleDrawRequest.fadeLevel = progress;
            g_BattleNeoBahamutParticleDrawRequest.modelGroup = Gfx_ModelTableGetRenderObject((&g_BattleNeoBahamutPrimaryResourceIds.fadeResourceIds)[rockPtr->resourceIndex], g_BattleSummonNeoBahamutLoadedResTable);
        } else {
            rockPtr->resourceIndex = 0xFFFF;
            return;
        }
    }
    if (rockPtr->alpha != 0) {
        progress = 0x1000 - g_BattleNeoBahamutParticleDrawRequest.fadeLevel;
        g_BattleNeoBahamutParticleDrawRequest.fadeLevel = 4096 - (progress * (4096 - rockPtr->alpha) >> 12);
    }
    g_BattleSummonNeoBahamutRock1TaskSlot->angleX = rockPtr->angleX;
    g_BattleSummonNeoBahamutRock1TaskSlot->angleY = rockPtr->angleY;
    g_BattleSummonNeoBahamutRock1TaskSlot->angleZ = rockPtr->angleZ;
    RotMatrix((SVECTOR *)g_BattleSummonNeoBahamutRock1TaskSlot, &g_BattleSummonNeoBahamutRock1TaskSlot->matrix);
    g_BattleSummonNeoBahamutRock1TaskSlot->matrix.t[0] = rockPtr->posX;
    g_BattleSummonNeoBahamutRock1TaskSlot->matrix.t[1] = rockPtr->posY;
    g_BattleSummonNeoBahamutRock1TaskSlot->matrix.t[2] = rockPtr->posZ;
    g_BattleSummonNeoBahamutRock1TaskSlot->valueNew = rockPtr->scale;
    g_BattleSummonNeoBahamutRock1TaskSlot->valuePrev = g_BattleSummonNeoBahamutRock1TaskSlot->valueNew;
    g_BattleSummonNeoBahamutRock1TaskSlot->valueOld = g_BattleSummonNeoBahamutRock1TaskSlot->valuePrev;
    ScaleMatrix(&g_BattleSummonNeoBahamutRock1TaskSlot->matrix, &g_BattleSummonNeoBahamutRock1TaskSlot->valueOld);
    MultiplyMatrix(&g_BattleSummonNeoBahamutComposedMatrix, &g_BattleSummonNeoBahamutRock1TaskSlot->matrix, &g_BattleSummonNeoBahamutRock1TaskSlot->matrix);
    SetRotMatrix(&g_BattleSummonNeoBahamutRock1TaskSlot->matrix);
    SetTransMatrix(&g_BattleSummonNeoBahamutRock1TaskSlot->matrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleNeoBahamutParticleDrawRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (rockPtr->frame)++;
        rockPtr->posX += rockPtr->velocityX;
        rockPtr->posY += rockPtr->velocityY;
        if (rockPtr->posY > 0) {
            rockPtr->resourceIndex = 0xFFFF;
        } else {
            rockPtr->posZ += rockPtr->velocityZ;
            rockPtr->velocityY += 50;
            rockPtr->angleX += 8;
            rockPtr->angleY += 0x20;
            rockPtr->angleZ += 0x30;
        }
    }
}

// Lazily acquires data block zero and selects model field fourteen. Sets its request short to zero
// below counter fifteen, ramps it over the next thirty counters, and retires at forty-five or
// above before drawing even while paused. Composes shared matrices into data block +0x10, draws in
// black and advances its counter only when unpaused.
// FUNCTION: FF7 0x00490de5
void __cdecl BattleFx_SummonNeoBahamutCloud1ModelTask(void)
{
    int stage;
    BattleTaskSlot *taskPtr;
    int progress;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if (((unsigned char)g_BattleSummonNeoBahamutCloud1InitFlag & 1) == 0) {
        g_BattleSummonNeoBahamutCloud1InitFlag |= 1;
        g_BattleSummonNeoBahamutCloud1TaskSlot = (BattleFxNeoBahamutCloud1Slot *)Gfx_GetPsxScratchpadPtr(0);
    }
    g_BattleSummonNeoBahamutCloud1ModelDrawRequest.modelGroup = Gfx_ModelTableGetRenderObject(14, g_BattleSummonNeoBahamutLoadedResTable);
    stage = taskPtr->frameCounter;
    if (stage < 15) {
        g_BattleSummonNeoBahamutCloud1ModelDrawRequest.fadeLevel = 0;
    } else {
        stage -= 15;
        if (stage < 30) {
            progress = (stage << 12) / 30;
            g_BattleSummonNeoBahamutCloud1ModelDrawRequest.fadeLevel = progress;
        } else {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    MultiplyMatrix(&g_BattleSummonNeoBahamutComposedMatrix, (int *)&g_BattleSummonNeoBahamutCloud1Matrix, &g_BattleSummonNeoBahamutCloud1TaskSlot->matrix);
    SetRotMatrix(&g_BattleSummonNeoBahamutCloud1TaskSlot->matrix);
    SetTransMatrix(&g_BattleSummonNeoBahamutCloud1TaskSlot->matrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonNeoBahamutCloud1ModelDrawRequest, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST_2, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (taskPtr->frameCounter)++;
    }
}

// Acquires retained scratch. Count zero initializes a centered viewport primitive using
// render-width multiplier times 160; count fifteen restores full viewport X/width, including while
// paused. Refreshes resource fields eleven/twelve. Negative counts use descriptor terms 4096;
// counts zero through 29 reduce both to zero, then hold zero until count 85, where retirement
// returns before drawing. Eases total count over 85 with parameter 0x1C0 and overwrites task X as
// 9600 plus fraction times 20400. Draws four black model passes through
// g_BattleSummonNeoBahamutComposedMatrix: resource eleven at mirrored plus/minus task X with
// opposite Z rotation count times 24 and saved uniform scale, then resource twelve at the same
// mirrored positions with opposite Z rotation count shifted six and uniform scale 0x2000. Uses the
// selected 0x1100 scratch bank at g_BattleSummonNeoBahamutCloudModelBuffer + 0x100 for the first
// pair and g_BattleSummonNeoBahamutCloudModelBuffer for the second, both shift ten. Sets
// blend-factor pair (0x3F7FFF58,0x3F800000) and submits the selected viewport primitive. Only
// count advancement is pause-gated.
// FUNCTION: FF7 0x00490f2a
void __cdecl BattleFx_SummonNeoBahamutCloud2ModelTask(void)
{
    int stage;
    BattleFxNeoBahamutCloud2Record *taskPtr;
    int progress;

    taskPtr = (BattleFxNeoBahamutCloud2Record *)BATTLE_CURRENT_TASK_SLOT;
    if (((unsigned char)g_BattleSummonNeoBahamutCloud2InitFlag & 1) == 0) {
        g_BattleSummonNeoBahamutCloud2InitFlag |= 1;
        g_BattleSummonNeoBahamutCloud2TaskSlot = (BattleSummonCloudSlot *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (taskPtr->phase == 0) {
        g_BattleSummonNeoBahamutCloud2VertexData.viewportOriginX = ((g_BattleViewportWidth - g_BattleUiScale * 160) / 2) + g_BattleViewportOriginX;
        (g_BattleSummonNeoBahamutCloud2VertexData.viewportOriginY) = (short)g_BattleViewportOriginY;
        (g_BattleSummonNeoBahamutCloud2VertexData.dataOffset) = g_BattleUiScale * 160;
        (g_BattleSummonNeoBahamutCloud2VertexData.viewportBottomY) = g_BattleSceneViewportHeight + g_BattleViewportOriginY;
    }
    g_BattleSummonNeoBahamutCloud2ModelDrawRequest.layer1.modelGroup = Gfx_ModelTableGetRenderObject(11, g_BattleSummonNeoBahamutLoadedResTable);
    g_BattleSummonNeoBahamutCloud2ModelDrawRequest.layer2ModelGroup = Gfx_ModelTableGetRenderObject(12, g_BattleSummonNeoBahamutLoadedResTable);
    stage = taskPtr->phase;
    if (stage < 0) {
        g_BattleSummonNeoBahamutCloud2ModelDrawRequest.layer1.fadeLevel = 4096;
    } else {
        stage = stage;
        if (stage < 30) {
            progress = (stage << 12) / 30;
            g_BattleSummonNeoBahamutCloud2ModelDrawRequest.layer1.fadeLevel = 4096 - progress;
        } else {
            stage -= 30;
            if (stage < 55) {
                g_BattleSummonNeoBahamutCloud2ModelDrawRequest.layer1.fadeLevel = 0;
            } else {
                taskPtr->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    stage = taskPtr->phase;
    if (stage < 0) {
        g_BattleSummonNeoBahamutCloud2ModelDrawRequest.layer2FadeLevel = 4096;
    } else {
        stage = stage;
        if (stage < 30) {
            progress = (stage << 12) / 30;
            g_BattleSummonNeoBahamutCloud2ModelDrawRequest.layer2FadeLevel = 4096 - progress;
        } else {
            g_BattleSummonNeoBahamutCloud2ModelDrawRequest.layer2FadeLevel = 0;
        }
    }
    if (taskPtr->phase == 0xF) {
        g_BattleSummonNeoBahamutCloud2VertexData.viewportOriginX = (short)g_BattleViewportOriginX;
        (g_BattleSummonNeoBahamutCloud2VertexData.dataOffset) = (short)g_BattleViewportWidth;
    }
    progress = (taskPtr->phase << 12) / 85;
    progress = BattleFx_NeoBahamutApplyCameraEasing(progress, 0x1C0);
    taskPtr->posX = (progress * 0x4FB0 >> 12) + 9600;
    SetDrawColorRGB(0, 0, 0);
    g_BattleSummonNeoBahamutCloud2TaskSlot->angleY = 0;
    g_BattleSummonNeoBahamutCloud2TaskSlot->angleX = 0;
    g_BattleSummonNeoBahamutCloud2TaskSlot->phaseStep = taskPtr->phase * 24;
    RotMatrix(&g_BattleSummonNeoBahamutCloud2TaskSlot->rotation, &g_BattleSummonNeoBahamutCloud2TaskSlot->matrix);
    g_BattleSummonNeoBahamutCloud2TaskSlot->matrix.t[0] = taskPtr->posX;
    g_BattleSummonNeoBahamutCloud2TaskSlot->matrix.t[1] = taskPtr->posY;
    g_BattleSummonNeoBahamutCloud2TaskSlot->matrix.t[2] = taskPtr->posZ;
    g_BattleSummonNeoBahamutCloud2TaskSlot->valueLatest = taskPtr->cloudValue;
    g_BattleSummonNeoBahamutCloud2TaskSlot->valueMid = g_BattleSummonNeoBahamutCloud2TaskSlot->valueLatest;
    g_BattleSummonNeoBahamutCloud2TaskSlot->valueOld = g_BattleSummonNeoBahamutCloud2TaskSlot->valueMid;
    ScaleMatrix(&g_BattleSummonNeoBahamutCloud2TaskSlot->matrix, &g_BattleSummonNeoBahamutCloud2TaskSlot->valueOld);
    MultiplyMatrix(&g_BattleSummonNeoBahamutComposedMatrix, &g_BattleSummonNeoBahamutCloud2TaskSlot->matrix, &g_BattleSummonNeoBahamutCloud2TaskSlot->matrix);
    SetRotMatrix(&g_BattleSummonNeoBahamutCloud2TaskSlot->matrix);
    SetTransMatrix(&g_BattleSummonNeoBahamutCloud2TaskSlot->matrix);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonNeoBahamutCloud2ModelDrawRequest, g_BattleSummonNeoBahamutCloud2ModelDrawHandle * 0x1100 + (int)((char *)&g_BattleSummonNeoBahamutCloudModelBuffer + 0x100), 0xA, g_BattleOverlayDrawListHead);
    g_BattleSummonNeoBahamutCloud2TaskSlot->phaseStep = -g_BattleSummonNeoBahamutCloud2TaskSlot->phaseStep;
    RotMatrix(&g_BattleSummonNeoBahamutCloud2TaskSlot->rotation, &g_BattleSummonNeoBahamutCloud2TaskSlot->matrix);
    g_BattleSummonNeoBahamutCloud2TaskSlot->matrix.t[0] = -taskPtr->posX;
    g_BattleSummonNeoBahamutCloud2TaskSlot->matrix.t[1] = taskPtr->posY;
    g_BattleSummonNeoBahamutCloud2TaskSlot->matrix.t[2] = taskPtr->posZ;
    g_BattleSummonNeoBahamutCloud2TaskSlot->valueLatest = taskPtr->cloudValue;
    g_BattleSummonNeoBahamutCloud2TaskSlot->valueMid = g_BattleSummonNeoBahamutCloud2TaskSlot->valueLatest;
    g_BattleSummonNeoBahamutCloud2TaskSlot->valueOld = g_BattleSummonNeoBahamutCloud2TaskSlot->valueMid;
    ScaleMatrix(&g_BattleSummonNeoBahamutCloud2TaskSlot->matrix, &g_BattleSummonNeoBahamutCloud2TaskSlot->valueOld);
    MultiplyMatrix(&g_BattleSummonNeoBahamutComposedMatrix, &g_BattleSummonNeoBahamutCloud2TaskSlot->matrix, &g_BattleSummonNeoBahamutCloud2TaskSlot->matrix);
    SetRotMatrix(&g_BattleSummonNeoBahamutCloud2TaskSlot->matrix);
    SetTransMatrix(&g_BattleSummonNeoBahamutCloud2TaskSlot->matrix);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonNeoBahamutCloud2ModelDrawRequest, g_BattleSummonNeoBahamutCloud2ModelDrawHandle * 0x1100 + (int)((char *)&g_BattleSummonNeoBahamutCloudModelBuffer + 0x100), 0xA, g_BattleOverlayDrawListHead);
    g_BattleSummonNeoBahamutCloud2TaskSlot->phaseStep = taskPtr->phase << 6;
    RotMatrix(&g_BattleSummonNeoBahamutCloud2TaskSlot->rotation, &g_BattleSummonNeoBahamutCloud2TaskSlot->matrix);
    g_BattleSummonNeoBahamutCloud2TaskSlot->matrix.t[0] = taskPtr->posX;
    g_BattleSummonNeoBahamutCloud2TaskSlot->matrix.t[1] = taskPtr->posY;
    g_BattleSummonNeoBahamutCloud2TaskSlot->matrix.t[2] = taskPtr->posZ;
    g_BattleSummonNeoBahamutCloud2TaskSlot->valueLatest = 0x2000;
    g_BattleSummonNeoBahamutCloud2TaskSlot->valueMid = 0x2000;
    g_BattleSummonNeoBahamutCloud2TaskSlot->valueOld = 0x2000;
    ScaleMatrix(&g_BattleSummonNeoBahamutCloud2TaskSlot->matrix, &g_BattleSummonNeoBahamutCloud2TaskSlot->valueOld);
    MultiplyMatrix(&g_BattleSummonNeoBahamutComposedMatrix, &g_BattleSummonNeoBahamutCloud2TaskSlot->matrix, &g_BattleSummonNeoBahamutCloud2TaskSlot->matrix);
    SetRotMatrix(&g_BattleSummonNeoBahamutCloud2TaskSlot->matrix);
    SetTransMatrix(&g_BattleSummonNeoBahamutCloud2TaskSlot->matrix);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonNeoBahamutCloud2ModelDrawRequest.layer2ModelGroup, g_BattleSummonNeoBahamutCloud2ModelDrawHandle * 0x1100 + (int)&g_BattleSummonNeoBahamutCloudModelBuffer, 0xA, g_BattleOverlayDrawListHead);
    g_BattleSummonNeoBahamutCloud2TaskSlot->phaseStep = -g_BattleSummonNeoBahamutCloud2TaskSlot->phaseStep;
    RotMatrix(&g_BattleSummonNeoBahamutCloud2TaskSlot->rotation, &g_BattleSummonNeoBahamutCloud2TaskSlot->matrix);
    g_BattleSummonNeoBahamutCloud2TaskSlot->matrix.t[0] = -taskPtr->posX;
    g_BattleSummonNeoBahamutCloud2TaskSlot->matrix.t[1] = taskPtr->posY;
    g_BattleSummonNeoBahamutCloud2TaskSlot->matrix.t[2] = taskPtr->posZ;
    g_BattleSummonNeoBahamutCloud2TaskSlot->valueLatest = 0x2000;
    g_BattleSummonNeoBahamutCloud2TaskSlot->valueMid = 0x2000;
    g_BattleSummonNeoBahamutCloud2TaskSlot->valueOld = 0x2000;
    ScaleMatrix(&g_BattleSummonNeoBahamutCloud2TaskSlot->matrix, &g_BattleSummonNeoBahamutCloud2TaskSlot->valueOld);
    MultiplyMatrix(&g_BattleSummonNeoBahamutComposedMatrix, &g_BattleSummonNeoBahamutCloud2TaskSlot->matrix, &g_BattleSummonNeoBahamutCloud2TaskSlot->matrix);
    SetRotMatrix(&g_BattleSummonNeoBahamutCloud2TaskSlot->matrix);
    SetTransMatrix(&g_BattleSummonNeoBahamutCloud2TaskSlot->matrix);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonNeoBahamutCloud2ModelDrawRequest.layer2ModelGroup, g_BattleSummonNeoBahamutCloud2ModelDrawHandle * 0x1100 + (int)&g_BattleSummonNeoBahamutCloudModelBuffer, 0xA, g_BattleOverlayDrawListHead);
    Gfx_SetBlendFactorPair(FLOAT_0_99999_BITS, FLOAT_ONE_BITS);
    Gfx_DrawPsxPrimitive((g_BattleSummonNeoBahamutCloud2ModelDrawHandle << 4) + (char *)&g_BattleSummonNeoBahamutCloud2Prim);
    if (g_BattleIsPaused == 0) {
        (taskPtr->phase)++;
    }
}

// Selects model field four, initializes two shared scale/translation matrices below counter ten,
// then grows the first matrix X/Z scale and lowers its Y translation over the next ninety
// counters. Retires at one hundred or above before drawing even while paused; otherwise installs
// the first matrix, draws and advances only when unpaused.
// FUNCTION: FF7 0x004915e9
void __cdecl BattleFx_SummonNeoBahamutSkyMaskModelTask(void)
{
    int stage;
    BattleTaskSlot *taskPtr;
    int progress;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSummonNeoBahamutCloud2VertexData.resource) = Gfx_ModelTableGetRenderObject(4, g_BattleSummonNeoBahamutLoadedResTable);
    stage = taskPtr->frameCounter;
    if (stage < 10) {
        (g_BattleSummonNeoBahamutSkyMaskVertexData1.scale) = 0xF00 /* 0.9375 */;
        g_BattleSummonNeoBahamutSkyMaskVertexData1.m00 = (g_BattleSummonNeoBahamutSkyMaskVertexData1.scale);
        (g_BattleSummonNeoBahamutSkyMaskVertexData1.translateY) = 0xF830;
        (g_BattleSummonNeoBahamutSkyMaskVertexData2.maskScale) = 0xF00;
        g_BattleSummonNeoBahamutSkyMaskVertexData2.m00 = (g_BattleSummonNeoBahamutSkyMaskVertexData2.maskScale);
        (g_BattleSummonNeoBahamutSkyMaskVertexData2.translateY) = 0xF830;
    } else {
        stage -= 10;
        if (stage < 90) {
            progress = (stage << 12) / 90;
            (g_BattleSummonNeoBahamutSkyMaskVertexData1.scale) = (progress * 0x780 >> 12) + 0xF00;
            g_BattleSummonNeoBahamutSkyMaskVertexData1.m00 = (g_BattleSummonNeoBahamutSkyMaskVertexData1.scale);
            (g_BattleSummonNeoBahamutSkyMaskVertexData1.translateY) = -((progress * 0x3E8 >> 12) + 2000);
        } else {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    SetRotMatrix((short *)&g_BattleSummonNeoBahamutSkyMaskVertexData1);
    SetTransMatrix((short *)&g_BattleSummonNeoBahamutSkyMaskVertexData1);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)(((char *)&g_BattleSummonNeoBahamutCloud2VertexData + 0x8)), g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (taskPtr->frameCounter)++;
    }
}

// Returns while paused. Computes local emission term (count times 0x60 AND 0xFF) plus 0x60 and
// emits BattleFx_SummonNeoBahamutLineLight3ModelTask while at least 256, discarding remainder. Each
// record first short receives parent count, X random 5000 through 5999, Y 0xB1E0, Z random -10000
// through 9999 and X velocity 64 through 127. A further random bit 0x1000 simultaneously negates X
// and X velocity; Y velocity is 0x400 and Z velocity zero. Saves random size 0x5000 through 0x7FFF.
// Advances count and retires at thirty.
// FUNCTION: FF7 0x00491738
void __cdecl BattleFx_SummonNeoBahamutSpawnLineLight3ModelsTask(void)
{
    BattleFxBurstSpawnerTask *taskPtr;
    int spawnBudget;
    BattleFxLineLightTask *light;
    int xRoll;
    int zRoll;
    int speedRoll;
    int bytesRoll;

    taskPtr = (BattleFxBurstSpawnerTask *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    spawnBudget = (taskPtr->spawnIndex * 0x60 & 0xFF) + 0x60;
    while (spawnBudget >= 256) {
        light = (BattleFxLineLightTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutLineLight3ModelTask);
        light->spawnIndex = taskPtr->spawnIndex;
        if (DEAD_IN_RETAIL != 0) {
            xRoll = 0x1770;
        } else {
            xRoll = rand() % 0x3E8 + 0x1388;
        }
        light->offsetX = xRoll;
        light->posY = 0xB1E0;
        if (DEAD_IN_RETAIL != 0) {
            zRoll = 0x2710;
        } else {
            zRoll = rand() % 0x4E20 - 0x2710;
        }
        light->posZ = zRoll;
        if (DEAD_IN_RETAIL != 0) {
            speedRoll = 0x80;
        } else {
            speedRoll = (rand() % 0x40) + 0x40;
        }
        light->offsetY = speedRoll;
        if ((rand() & 0x1000) != 0) {
            light->offsetX = -light->offsetX;
            light->offsetY = -light->offsetY;
        }
        light->rotY = 0x400 /* 90 deg */;
        light->rotZ = 0;
        if (DEAD_IN_RETAIL != 0) {
            bytesRoll = 0x8000 /* 2880 deg */;
        } else {
            bytesRoll = rand() % 0x3000 + 0x5000;
        }
        light->intensity = bytesRoll;
        spawnBudget -= 0x100;
    }
    (taskPtr->spawnIndex)++;
    if (taskPtr->spawnIndex >= 30) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Acquires retained scratch and refreshes resource field three. Descriptor term falls from 4096
// over ten counts, stays zero for five, then rises for ten; later retire before drawing/pause.
// Builds rotation from task shorts 0xC through 0x10 (also populated as velocities by its emitter),
// with task XYZ translation and X scale saved size while Y/Z are 0x2000. Scales, multiplies
// g_BattleSummonNeoBahamutComposedMatrix, installs and submits black into
// g_BattleCurrentOrderingTable plus 0x70 with shift twelve. While unpaused advances count and
// additionally retires when new count plus first task short is at least seventy; no position or
// rotation update appears.
// FUNCTION: FF7 0x004918f1
void __cdecl BattleFx_SummonNeoBahamutLineLight3ModelTask(void)
{
    int stage;
    int progress;
    BattleFxNeoBahamutLineLight *pLight;

    pLight = (BattleFxNeoBahamutLineLight *)BATTLE_CURRENT_TASK_SLOT;
    if (((unsigned char)g_BattleSummonNeoBahamutLineLight3WorkingBlob.initFlag & 1) == 0) {
        g_BattleSummonNeoBahamutLineLight3WorkingBlob.initFlag |= 1;
        g_BattleSummonNeoBahamutLineLight3TaskSlot = (BattleFxNeoBahamutLineLight3Slot *)Gfx_GetPsxScratchpadPtr(0);
    }
    (g_BattleSummonNeoBahamutSkyMaskWorkingBlob.lineLightResource) = Gfx_ModelTableGetRenderObject(3, g_BattleSummonNeoBahamutLoadedResTable);
    stage = pLight->step;
    if (stage < 10) {
        progress = (stage << 12) / 10;
        (g_BattleSummonNeoBahamutSkyMaskWorkingBlob.phase) = 0x1000 - progress;
    } else {
        stage -= 10;
        if (stage < 5) {
            (g_BattleSummonNeoBahamutSkyMaskWorkingBlob.phase) = 0;
        } else {
            stage -= 5;
            if (stage < 10) {
                progress = (stage << 12) / 10;
                (g_BattleSummonNeoBahamutSkyMaskWorkingBlob.phase) = progress;
            } else {
                pLight->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    RotMatrix((SVECTOR *)&pLight->rotX, &g_BattleSummonNeoBahamutLineLight3TaskSlot->matrix);
    g_BattleSummonNeoBahamutLineLight3TaskSlot->matrix.t[0] = pLight->posX;
    g_BattleSummonNeoBahamutLineLight3TaskSlot->matrix.t[1] = pLight->posY;
    g_BattleSummonNeoBahamutLineLight3TaskSlot->matrix.t[2] = pLight->posZ;
    g_BattleSummonNeoBahamutLineLight3TaskSlot->intensity = pLight->intensity;
    g_BattleSummonNeoBahamutLineLight3TaskSlot->scaleZ = 0x2000 /* 2.0 */;
    g_BattleSummonNeoBahamutLineLight3TaskSlot->scaleY = 0x2000 /* 2.0 */;
    ScaleMatrix(&g_BattleSummonNeoBahamutLineLight3TaskSlot->matrix, g_BattleSummonNeoBahamutLineLight3TaskSlot);
    MultiplyMatrix(&g_BattleSummonNeoBahamutComposedMatrix, &g_BattleSummonNeoBahamutLineLight3TaskSlot->matrix, &g_BattleSummonNeoBahamutLineLight3TaskSlot->matrix);
    SetRotMatrix(&g_BattleSummonNeoBahamutLineLight3TaskSlot->matrix);
    SetTransMatrix(&g_BattleSummonNeoBahamutLineLight3TaskSlot->matrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)(((char *)&g_BattleSummonNeoBahamutSkyMaskWorkingBlob + 0x70)), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (pLight->step)++;
        if (pLight->step + pLight->ownerSlot >= 70) {
            pLight->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

// Acquires retained scratch and refreshes resource field thirteen plus the scene resource at offset
// eight, using zero when scene is absent. Count zero builds retained Y rotation 0x400, translation
// (1000,5000,0) and uniform scale 0x3800, repeating initialization while paused at zero. Below
// count 218 clears descriptor term; next 42 ramp it toward 0xC00; later retain two negative-only
// tests and retire/return for ordinary nonnegative elapsed values. Composes
// g_BattleSummonNeoBahamutComposedMatrix with the retained matrix, installs and draws black
// resource thirteen into g_BattleCurrentOrderingTable plus 0x4068, then composes
// g_BattleSummonNeoBahamutCloudSeaMatrix and draws the scene resource into plus 0x4064, both shift
// zero. While unpaused advances count and changes retained X translation to 1000 minus count
// fraction over 260 times 2000, holding Y 5000/Z zero through comma-zero expressions.
// FUNCTION: FF7 0x00491b0e
void __cdecl BattleFx_SummonNeoBahamutCloudSeaModelTask(void)
{
    int stage;
    BattleTaskSlot *taskPtr;
    int progress;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if (((unsigned char)g_BattleSummonNeoBahamutCloudSeaInitFlag & 1) == 0) {
        g_BattleSummonNeoBahamutCloudSeaInitFlag |= 1;
        g_BattleSummonNeoBahamutCloudSeaTaskSlot = (BattleFxNeoBahamutCloudSea *)Gfx_GetPsxScratchpadPtr(0);
    }
    (g_BattleSummonNeoBahamutSkyMaskWorkingBlob.cloudSeaResource) = Gfx_ModelTableGetRenderObject(13, g_BattleSummonNeoBahamutLoadedResTable);
    if (g_pBattle3dScene != 0) {
        (g_BattleSummonNeoBahamutSkyMaskWorkingBlob.modelRequest) = g_pBattle3dScene->modelRequest;
    } else {
        (g_BattleSummonNeoBahamutSkyMaskWorkingBlob.modelRequest) = 0;
    }
    if (taskPtr->frameCounter == 0) {
        g_BattleSummonNeoBahamutCloudSeaTaskSlot->rotX = 0;
        g_BattleSummonNeoBahamutCloudSeaTaskSlot->rotY = 0x400 /* 90 deg */;
        g_BattleSummonNeoBahamutCloudSeaTaskSlot->rotZ = 0;
        RotMatrix(&g_BattleSummonNeoBahamutCloudSeaTaskSlot->rotation, &g_BattleSummonNeoBahamutLineLight3WorkingBlob.matrix);
        g_BattleSummonNeoBahamutLineLight3WorkingBlob.matrix.t[0] = 0x3E8;
        g_BattleSummonNeoBahamutLineLight3WorkingBlob.matrix.t[1] = 0x1388;
        g_BattleSummonNeoBahamutLineLight3WorkingBlob.matrix.t[2] = 0;
        g_BattleSummonNeoBahamutCloudSeaTaskSlot->scaleZ = 0x3800 /* 3.5 */;
        g_BattleSummonNeoBahamutCloudSeaTaskSlot->scaleY = 0x3800 /* 3.5 */;
        g_BattleSummonNeoBahamutCloudSeaTaskSlot->scaleX = 0x3800 /* 3.5 */;
        ScaleMatrix(&g_BattleSummonNeoBahamutLineLight3WorkingBlob.matrix, &g_BattleSummonNeoBahamutCloudSeaTaskSlot->scaleX);
    }
    stage = taskPtr->frameCounter;
    if (stage < 218) {
        (g_BattleSummonNeoBahamutSkyMaskWorkingBlob.cloudScroll) = 0;
    } else {
        stage -= 218;
        if (stage < 42) {
            progress = (stage << 12) / 42;
            (g_BattleSummonNeoBahamutSkyMaskWorkingBlob.cloudScroll) = progress * 0xC00 /* 0.75 */ >> 12;
        } else {
            stage -= 42;
            if (stage < 0) {
                progress = 0x1000 - (stage << 12) / 42;
                (g_BattleSummonNeoBahamutSkyMaskWorkingBlob.cloudScroll) = progress * 0xC00 /* 0.75 */ >> 12;
            } else {
                stage = stage;
                if (stage < 0) {
                    (g_BattleSummonNeoBahamutSkyMaskWorkingBlob.cloudScroll) = 0;
                } else {
                    taskPtr->ownerSlot = BATTLE_TASK_FREE;
                    return;
                }
            }
        }
    }
    MultiplyMatrix(&g_BattleSummonNeoBahamutComposedMatrix, &g_BattleSummonNeoBahamutLineLight3WorkingBlob.matrix, &g_BattleSummonNeoBahamutCloudSeaTaskSlot->matrix);
    SetRotMatrix(&g_BattleSummonNeoBahamutCloudSeaTaskSlot->matrix);
    SetTransMatrix(&g_BattleSummonNeoBahamutCloudSeaTaskSlot->matrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)(((char *)&g_BattleSummonNeoBahamutSkyMaskWorkingBlob + 0x80)), g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST_1, 0, g_BattleOverlayDrawListHead);
    MultiplyMatrix(&g_BattleSummonNeoBahamutComposedMatrix, (int *)&g_BattleSummonNeoBahamutCloudSeaMatrix, &g_BattleSummonNeoBahamutCloudSeaTaskSlot->matrix);
    SetRotMatrix(&g_BattleSummonNeoBahamutCloudSeaTaskSlot->matrix);
    SetTransMatrix(&g_BattleSummonNeoBahamutCloudSeaTaskSlot->matrix);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)(((char *)&g_BattleSummonNeoBahamutSkyMaskWorkingBlob + 0x90)), g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST_2, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (taskPtr->frameCounter)++;
        progress = (taskPtr->frameCounter << 12) / 260;
        g_BattleSummonNeoBahamutLineLight3WorkingBlob.matrix.t[0] = (progress * -0x7D0 >> 12) + 0x3E8;
        g_BattleSummonNeoBahamutLineLight3WorkingBlob.matrix.t[1] = (UNFOLDED(0) >> 12) + 0x1388;
        g_BattleSummonNeoBahamutLineLight3WorkingBlob.matrix.t[2] = UNFOLDED(0) >> 12;
    }
}

// Acquires retained scratch and leaves primitive colours unchanged for first 218 counts. Next 42
// shade two retained base colours with a fraction scaled by 0xC00, then fade that weight over
// fifteen, writing selected primitive-bank colours with commands 0x60/0x3A. Next 95 restore only
// bank zero colours on entry through a one-iteration loop; later retire before drawing/pause.
// Draws the selected first primitive with blend pair (0x3F7FFF58,0x3F800000). Copies thirty bytes
// of g_BattleSummonNeoBahamutComposedMatrix, orthonormalizes from Y, scales uniformly 0x3800 and installs the matrix.
// Projects three retained points and uses their output Y shorts to update eight Y fields across the
// two selected 0x24-byte primitive banks without testing projection flags. Below total count 350
// draws both with blend pair (0x3F7FF972,0x3F800000); later still projects/updates them but skips
// those draws. Only count advancement is pause-gated.
// FUNCTION: FF7 0x00491e24
void __cdecl BattleFx_SummonNeoBahamutBackSkyPrimitiveTask(void)
{
    int stage;
    BattleTaskSlot *taskPtr;
    int primIdx;
    int weight;
    int colorB;
    int colorA;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if (((unsigned char)g_BattleSummonNeoBahamutBackSkyInitFlag & 1) == 0) {
        g_BattleSummonNeoBahamutBackSkyInitFlag |= 1;
        g_BattleSummonNeoBahamutBackSkyTaskSlot = (BattleFxNeoBahamutBackSkyState *)Gfx_GetPsxScratchpadPtr(0);
    }
    stage = taskPtr->frameCounter;
    if (stage < 218) { /* empty first arm: lib/codegen.h */
    } else {
        stage -= 218;
        if (stage < 42) {
            weight = (stage << 12) / 42;
            weight = weight * 0xC00 /* 0.75 */ >> 12;
            SetDrawColorRGB(0, 0, 0);
            Gfx_ShadePsxVertexColor((CVECTOR *)&g_BattleSummonNeoBahamutBackSkyShadeColor1, weight, (CVECTOR *)&colorA);
            Gfx_ShadePsxVertexColor((CVECTOR *)&g_BattleSummonNeoBahamutBackSkyShadeColor2, weight, (CVECTOR *)&colorB);
            g_BattleSummonNeoBahamutBackSkyPrims3[g_BattleSummonNeoBahamutBackSkyPrimIndex].color = colorA & 0xFFFFFF | 0x60000000;
            g_BattleSummonNeoBahamutBackSkyPrims2[g_BattleSummonNeoBahamutBackSkyPrimIndex].color1 = colorB & 0xFFFFFF | 0x3A000000;
            g_BattleSummonNeoBahamutBackSkyPrims2[g_BattleSummonNeoBahamutBackSkyPrimIndex].color0 = g_BattleSummonNeoBahamutBackSkyPrims2[g_BattleSummonNeoBahamutBackSkyPrimIndex].color1;
            g_BattleSummonNeoBahamutBackSkyPrims1[g_BattleSummonNeoBahamutBackSkyPrimIndex].color3 = g_BattleSummonNeoBahamutBackSkyPrims2[g_BattleSummonNeoBahamutBackSkyPrimIndex].color0;
            g_BattleSummonNeoBahamutBackSkyPrims1[g_BattleSummonNeoBahamutBackSkyPrimIndex].color2 = g_BattleSummonNeoBahamutBackSkyPrims1[g_BattleSummonNeoBahamutBackSkyPrimIndex].color3;
        } else {
            stage -= 42;
            if (stage < 15) {
                weight = 0x1000 - (stage << 12) / 15;
                weight = weight * 0xC00 /* 0.75 */ >> 12;
                SetDrawColorRGB(0, 0, 0);
                Gfx_ShadePsxVertexColor((CVECTOR *)&g_BattleSummonNeoBahamutBackSkyShadeColor1, weight, (CVECTOR *)&colorA);
                Gfx_ShadePsxVertexColor((CVECTOR *)&g_BattleSummonNeoBahamutBackSkyShadeColor2, weight, (CVECTOR *)&colorB);
                g_BattleSummonNeoBahamutBackSkyPrims3[g_BattleSummonNeoBahamutBackSkyPrimIndex].color = colorA & 0xFFFFFF | 0x60000000;
                g_BattleSummonNeoBahamutBackSkyPrims2[g_BattleSummonNeoBahamutBackSkyPrimIndex].color1 = colorB & 0xFFFFFF | 0x3A000000;
                g_BattleSummonNeoBahamutBackSkyPrims2[g_BattleSummonNeoBahamutBackSkyPrimIndex].color0 = g_BattleSummonNeoBahamutBackSkyPrims2[g_BattleSummonNeoBahamutBackSkyPrimIndex].color1;
                g_BattleSummonNeoBahamutBackSkyPrims1[g_BattleSummonNeoBahamutBackSkyPrimIndex].color3 = g_BattleSummonNeoBahamutBackSkyPrims2[g_BattleSummonNeoBahamutBackSkyPrimIndex].color0;
                g_BattleSummonNeoBahamutBackSkyPrims1[g_BattleSummonNeoBahamutBackSkyPrimIndex].color2 = g_BattleSummonNeoBahamutBackSkyPrims1[g_BattleSummonNeoBahamutBackSkyPrimIndex].color3;
            } else {
                stage -= 15;
                if (stage < 95) {
                    if (stage == 0) {
                        for (primIdx = 0; primIdx < 1; primIdx++) {
                            g_BattleSummonNeoBahamutBackSkyPrims3[primIdx].color = g_BattleSummonNeoBahamutBackSkyShadeColor1 | 0x60000000;
                            g_BattleSummonNeoBahamutBackSkyPrims2[primIdx].color1 = g_BattleSummonNeoBahamutBackSkyShadeColor2 | 0x3A000000;
                            g_BattleSummonNeoBahamutBackSkyPrims2[primIdx].color0 = g_BattleSummonNeoBahamutBackSkyPrims2[primIdx].color1;
                            g_BattleSummonNeoBahamutBackSkyPrims1[primIdx].color3 = g_BattleSummonNeoBahamutBackSkyPrims2[primIdx].color0;
                            g_BattleSummonNeoBahamutBackSkyPrims1[primIdx].color2 = g_BattleSummonNeoBahamutBackSkyPrims1[primIdx].color3;
                        }
                    }
                } else {
                    taskPtr->ownerSlot = BATTLE_TASK_FREE;
                    return;
                }
            }
        }
    }
    Gfx_SetBlendFactorPair(FLOAT_0_99999_BITS, FLOAT_ONE_BITS);
    Gfx_DrawPsxPrimitive((g_BattleSummonNeoBahamutBackSkyPrimIndex << 4) + (char *)&g_BattleSummonNeoBahamutBackSkyPrim3);
    g_BattleSummonNeoBahamutBackSkyTaskSlot->matrix = *&g_BattleSummonNeoBahamutComposedMatrix;
    Gte_OrthonormaliseFromYAxis((BattleOrthoMatrix *)g_BattleSummonNeoBahamutBackSkyTaskSlot);
    g_BattleSummonNeoBahamutBackSkyTaskSlot->scaleZ = 0x3800 /* 3.5 */;
    g_BattleSummonNeoBahamutBackSkyTaskSlot->scaleY = 0x3800 /* 3.5 */;
    g_BattleSummonNeoBahamutBackSkyTaskSlot->scaleX = 0x3800 /* 3.5 */;
    ScaleMatrix(g_BattleSummonNeoBahamutBackSkyTaskSlot, &g_BattleSummonNeoBahamutBackSkyTaskSlot->scaleX);
    SetRotMatrix((void *)g_BattleSummonNeoBahamutBackSkyTaskSlot);
    SetTransMatrix(g_BattleSummonNeoBahamutBackSkyTaskSlot);
    RefreshGlobalTransformState();
    Gfx_ProjectTriangleToScreenAlt((int *)&g_BattleSummonNeoBahamutBackSkyVertex1, &g_BattleSummonNeoBahamutBackSkyVertex2, (SVECTOR *)&g_BattleSummonNeoBahamutBackSkyVertex3, &g_BattleSummonNeoBahamutBackSkyTaskSlot->projectedX1, &g_BattleSummonNeoBahamutBackSkyTaskSlot->projectedX2, &g_BattleSummonNeoBahamutBackSkyTaskSlot->projectedX3, &g_BattleSummonNeoBahamutBackSkyTaskSlot->unusedOut, &g_BattleSummonNeoBahamutBackSkyTaskSlot->projectFlags);
    g_BattleSummonNeoBahamutBackSkyPrims1[g_BattleSummonNeoBahamutBackSkyPrimIndex].vertex1Y = g_BattleSummonNeoBahamutBackSkyTaskSlot->projectedY2;
    g_BattleSummonNeoBahamutBackSkyPrims1[g_BattleSummonNeoBahamutBackSkyPrimIndex].vertex0Y = g_BattleSummonNeoBahamutBackSkyPrims1[g_BattleSummonNeoBahamutBackSkyPrimIndex].vertex1Y;
    g_BattleSummonNeoBahamutBackSkyPrims1[g_BattleSummonNeoBahamutBackSkyPrimIndex].vertex3Y = g_BattleSummonNeoBahamutBackSkyTaskSlot->projectedY1;
    g_BattleSummonNeoBahamutBackSkyPrims1[g_BattleSummonNeoBahamutBackSkyPrimIndex].vertex2Y = g_BattleSummonNeoBahamutBackSkyPrims1[g_BattleSummonNeoBahamutBackSkyPrimIndex].vertex3Y;
    g_BattleSummonNeoBahamutBackSkyPrims2[g_BattleSummonNeoBahamutBackSkyPrimIndex].vertex1Y = g_BattleSummonNeoBahamutBackSkyTaskSlot->projectedY1;
    g_BattleSummonNeoBahamutBackSkyPrims2[g_BattleSummonNeoBahamutBackSkyPrimIndex].vertex0Y = g_BattleSummonNeoBahamutBackSkyPrims2[g_BattleSummonNeoBahamutBackSkyPrimIndex].vertex1Y;
    g_BattleSummonNeoBahamutBackSkyPrims2[g_BattleSummonNeoBahamutBackSkyPrimIndex].vertex3Y = g_BattleSummonNeoBahamutBackSkyTaskSlot->projectedY3;
    g_BattleSummonNeoBahamutBackSkyPrims2[g_BattleSummonNeoBahamutBackSkyPrimIndex].vertex2Y = g_BattleSummonNeoBahamutBackSkyPrims2[g_BattleSummonNeoBahamutBackSkyPrimIndex].vertex3Y;
    if (taskPtr->frameCounter < 350) {
        Gfx_SetBlendFactorPair(FLOAT_0_9999_BITS, FLOAT_ONE_BITS);
        Gfx_DrawPsxPrimitive(g_BattleSummonNeoBahamutBackSkyPrimIndex * 0x24 + (char *)&g_BattleSummonNeoBahamutBackSkyPrim1);
        Gfx_SetBlendFactorPair(FLOAT_0_9999_BITS, FLOAT_ONE_BITS);
        Gfx_DrawPsxPrimitive(g_BattleSummonNeoBahamutBackSkyPrimIndex * 0x24 + (char *)&g_BattleSummonNeoBahamutBackSkyPrim2);
    }
    if (g_BattleIsPaused == 0) {
        (taskPtr->frameCounter)++;
    }
}

// Returns while paused and emits three BattleFx_SummonNeoBahamutBallSpriteTask records. Builds a
// retained rotation from random low-twelve-bit X, Y 0xFC00 and random Z zero through 0x2FF, then
// rotates an X vector of random magnitude 3000 through 4999 in place. Places each record at
// g_BattleSummonNeoBahamutAnchorPosition plus that vector, sets velocity to each negative vector
// component divided by twenty and random size 0x300 through 0x5FF. Advances count and retires at
// 22.
// FUNCTION: FF7 0x004923ab
void __cdecl BattleFx_SummonNeoBahamutSpawnBallSpritesTask(void)
{
    BattleTaskSlot *taskPtr;
    BattleFxNeoBahamutBallSprite *ballPtr;
    int ballIdx;
    int magnitude;
    int bytes;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    for (ballIdx = 0; ballIdx < 3; ballIdx++) {
        ballPtr = (BattleFxNeoBahamutBallSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutBallSpriteTask);
        g_BattleSummonNeoBahamutBallSpriteWorkingBlob.vector[0] = rand() & 0xFFF;
        g_BattleSummonNeoBahamutBallSpriteWorkingBlob.vector[1] = 0xFC00;
        g_BattleSummonNeoBahamutBallSpriteWorkingBlob.vector[2] = rand() % 768;
        RotMatrix((SVECTOR *)g_BattleSummonNeoBahamutBallSpriteWorkingBlob.vector, &g_BattleSummonNeoBahamutBallSpriteWorkingBlob.matrix);
        if (DEAD_IN_RETAIL != 0) {
            magnitude = 0x1388;
        } else {
            magnitude = rand() % 2000 + 3000;
        }
        g_BattleSummonNeoBahamutBallSpriteWorkingBlob.vector[0] = magnitude;
        g_BattleSummonNeoBahamutBallSpriteWorkingBlob.vector[1] = 0;
        g_BattleSummonNeoBahamutBallSpriteWorkingBlob.vector[2] = 0;
        TransformVectorToInt16((void *)&g_BattleSummonNeoBahamutBallSpriteWorkingBlob.matrix, (void *)g_BattleSummonNeoBahamutBallSpriteWorkingBlob.vector, (void *)g_BattleSummonNeoBahamutBallSpriteWorkingBlob.vector);
        ballPtr->posX = (g_BattleSummonNeoBahamutAnchorPosition[0]) + g_BattleSummonNeoBahamutBallSpriteWorkingBlob.vector[0];
        ballPtr->posY = (g_BattleSummonNeoBahamutAnchorPosition[1]) + g_BattleSummonNeoBahamutBallSpriteWorkingBlob.vector[1];
        ballPtr->posZ = (g_BattleSummonNeoBahamutAnchorPosition[2]) + g_BattleSummonNeoBahamutBallSpriteWorkingBlob.vector[2];
        ballPtr->velocityX = -g_BattleSummonNeoBahamutBallSpriteWorkingBlob.vector[0] / 20;
        ballPtr->velocityY = -g_BattleSummonNeoBahamutBallSpriteWorkingBlob.vector[1] / 20;
        ballPtr->velocityZ = -g_BattleSummonNeoBahamutBallSpriteWorkingBlob.vector[2] / 20;
        if (DEAD_IN_RETAIL != 0) {
            bytes = 0x600;
        } else {
            bytes = rand() % 768 + 768;
        }
        ballPtr->scale = bytes;
    }
    (taskPtr->frameCounter)++;
    if (taskPtr->frameCounter >= 22) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Selects sprite field seventeen, shades its request from black over counters below ten, uses
// packed 0x2E808080 for ten through nineteen, and retires at twenty or above before drawing even
// while paused. Installs slot position and scale with BattleFx_SummonNeoBahamutInstallBillboardMatrix, draws, then when unpaused
// advances the counter and integrates position.
// FUNCTION: FF7 0x0049258f
void __cdecl BattleFx_SummonNeoBahamutBallSpriteTask(void)
{
    int stage;
    int progress;
    BattleFxNeoBahamutBallSprite *ballPtr;

    ballPtr = (BattleFxNeoBahamutBallSprite *)BATTLE_CURRENT_TASK_SLOT;
    (((BattleFxNeoBahamutParam *)&g_BattleSummonNeoBahamutBackSkyVertex3)->resourceHandle) = Gfx_ModelTableGetLoadedRecord(17, g_BattleSummonNeoBahamutLoadedResTable);
    (((BattleFxNeoBahamutParam *)&g_BattleSummonNeoBahamutBackSkyVertex3)->drawObj) = Gfx_ModelTableGetSpriteObject(17, g_BattleSummonNeoBahamutLoadedResTable);
    stage = ballPtr->frameIndex;
    if (stage < 10) {
        progress = (stage << 12) / 10;
        SetDrawColorRGB(0, 0, 0);
        Gfx_ShadePsxVertexColor(g_BattleSummonNeoBahamutBallSpriteColor2, 0x1000 - progress, g_BattleSummonNeoBahamutBallSpriteColor1);
    } else {
        stage -= 10;
        if (stage < 10) {
            (*(int *)&g_BattleSummonNeoBahamutBallSpriteColor1) = 0x2E808080;
        } else {
            ballPtr->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    BattleFx_SummonNeoBahamutInstallBillboardMatrix(&ballPtr->posX, (short)ballPtr->scale, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw((char *)&g_BattleSummonNeoBahamutBackSkyVertex3 + 0x8, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (ballPtr->frameIndex)++;
        ballPtr->posX += ballPtr->velocityX;
        ballPtr->posY += ballPtr->velocityY;
        ballPtr->posZ += ballPtr->velocityZ;
    }
}

// Selects model field one and computes scale through a thirty-seven-counter rise to 0x800, ten-
// counter rise toward 0x2000 and fifteen-counter fall, retiring after those phases before drawing
// even while paused. Installs g_BattleSummonNeoBahamutAnchorPosition through BattleFx_SummonNeoBahamutInstallBillboardMatrix with that scale, submits the model
// and advances the counter only when unpaused.
// FUNCTION: FF7 0x004926e5
void __cdecl BattleFx_SummonNeoBahamutCircleModelTask(void)
{
    int scale;
    int stage;
    BattleTaskSlot *taskPtr;
    int progress;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    (((BattleFxFlareSpriteSlot *)&g_BattleSummonNeoBahamutBallSpriteColor2)->tableValue) = Gfx_ModelTableGetRenderObject(1, g_BattleSummonNeoBahamutLoadedResTable);
    stage = taskPtr->frameCounter;
    if (stage < 37) {
        progress = (stage << 12) / 37;
        scale = progress << 11 >> 12;
    } else {
        stage -= 37;
        if (stage < 10) {
            progress = (stage << 12) / 10;
            scale = (progress * 0x1800 /* 1.5 */ >> 12) + 0x800;
        } else {
            stage -= 10;
            if (stage < 15) {
                progress = (stage << 12) / 15;
                scale = (progress * -0x1800 /* 1.5 */ >> 12) + 0x2000;
            } else {
                taskPtr->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    BattleFx_SummonNeoBahamutInstallBillboardMatrix(&g_BattleSummonNeoBahamutAnchorPosition[0], (short)scale, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)(((char *)&g_BattleSummonNeoBahamutBallSpriteColor2 + 0x8)), g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (taskPtr->frameCounter)++;
    }
}

// Returns while paused. Local emission term is (count shifted seven AND 0xFF) plus 128, emitting
// one BattleFx_SummonNeoBahamutLineLight1PrimitiveTask on odd counts and none on even counts,
// discarding remainder. Saves parent count as first short, copies eight bytes from
// g_BattleSummonNeoBahamutAnchorPosition, clears two rotation shorts and increases retained angle
// accumulator g_BattleSummonNeoBahamutCircleModelAngle by random low-eleven-bit value plus 0x400,
// saving its low short as Z rotation. Assigns random signed angular increment of magnitude 32
// through 63 and random size 0x800 through 0x17FF. Advances count and retires at 27.
// FUNCTION: FF7 0x00492816
void __cdecl BattleFx_SummonNeoBahamutSpawnLineLight1Task(void)
{
    BattleFxBurstSpawnerTask *taskPtr;
    int spawnBudget;
    BattleFxNeoBahamutLineSpawn *pLine;
    int angularStep;
    int bytes;

    taskPtr = (BattleFxBurstSpawnerTask *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    spawnBudget = (taskPtr->spawnIndex << 7 & 0xFF) + 0x80;
    while (spawnBudget >= 256) {
        pLine = (BattleFxNeoBahamutLineSpawn *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutLineLight1PrimitiveTask);
        pLine->spawnIndex = taskPtr->spawnIndex;
        *(Int64Pair *)((char *)pLine + 4) = *(Int64Pair *)(&g_BattleSummonNeoBahamutAnchorPosition);
        pLine->rotY = 0;
        pLine->rotX = 0;
        g_BattleSummonNeoBahamutCircleModelAngle = g_BattleSummonNeoBahamutCircleModelAngle + (rand() & 0x7FF) + 1024;
        pLine->angle = (short)g_BattleSummonNeoBahamutCircleModelAngle;
        if (DEAD_IN_RETAIL != 0) {
            angularStep = 64;
        } else {
            angularStep = rand() % 32 + 32;
        }
        pLine->angleStep = angularStep;
        if ((rand() & 0x800) != 0) {
            pLine->angleStep = -pLine->angleStep;
        }
        if (DEAD_IN_RETAIL != 0) {
            bytes = 0x1800;
        } else {
            bytes = rand() % 4096 + 2048;
        }
        pLine->size = bytes;
        spawnBudget -= 0x100;
    }
    (taskPtr->spawnIndex)++;
    if (taskPtr->spawnIndex >= 27) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Acquires retained scratch and ramps X scale from zero to saved size over five counts, holds five
// and falls five; later retires before draw/pause. Projects task position through g_BattleSummonNeoBahamutComposedMatrix
// without testing flags or return, builds task rotation and places its matrix at projected screen
// coordinates minus geometry offset with Z equal to geometry screen. Keeps Y/Z scale 4096, scales
// and installs that matrix, refreshes transform state and obtains a primitive from BattleFx_SummonNeoBahamutPrepareLineLightPrimitive.
// Writes three colour/command words 0x32808080 and twice 0x32808040, then submits. While unpaused
// advances count and adds saved angular increment 0x1A to task Z rotation.
// FUNCTION: FF7 0x0049299c
void __cdecl BattleFx_SummonNeoBahamutLineLight1PrimitiveTask(void)
{
    int stage;
    int progress;
    BattleFxNeoBahamutLineLight *pLight;
    BattleFxLineLightPrim *primitive;

    pLight = (BattleFxNeoBahamutLineLight *)BATTLE_CURRENT_TASK_SLOT;
    if (((unsigned char)g_BattleSummonNeoBahamutLineLight1InitFlag & 1) == 0) {
        g_BattleSummonNeoBahamutLineLight1InitFlag |= 1;
        g_BattleSummonNeoBahamutLineLight1TaskSlot = (BattleFxNeoBahamutLineLight1Slot *)Gfx_GetPsxScratchpadPtr(0);
    }
    stage = pLight->step;
    if (stage < 5) {
        progress = (stage << 12) / 5;
        g_BattleSummonNeoBahamutLineLight1TaskSlot->blendedValue = pLight->intensity * progress >> 12;
    } else {
        stage -= 5;
        if (stage < 5) {
            g_BattleSummonNeoBahamutLineLight1TaskSlot->blendedValue = pLight->intensity;
        } else {
            stage -= 5;
            if (stage < 5) {
                progress = (stage << 12) / 5;
                g_BattleSummonNeoBahamutLineLight1TaskSlot->blendedValue = pLight->intensity * (0x1000 - progress) >> 12;
            } else {
                pLight->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    SetRotMatrix(&g_BattleSummonNeoBahamutComposedMatrix);
    SetTransMatrix(&g_BattleSummonNeoBahamutComposedMatrix);
    g_BattleSummonNeoBahamutLineLight1TaskSlot->depth = RotTransPers((SVECTOR *)&pLight->posX, (short *)&g_BattleSummonNeoBahamutLineLight1TaskSlot->screenX, (int *)((char *)g_BattleSummonNeoBahamutLineLight1TaskSlot + 0x38), (int *)((char *)g_BattleSummonNeoBahamutLineLight1TaskSlot + 0x3C));
    RotMatrix((SVECTOR *)&pLight->rotX, &g_BattleSummonNeoBahamutLineLight1TaskSlot->matrix);
    GetGeomOffset(&g_BattleSummonNeoBahamutLineLight1TaskSlot->geomOffsetX, &g_BattleSummonNeoBahamutLineLight1TaskSlot->geomOffsetY);
    g_BattleSummonNeoBahamutLineLight1TaskSlot->matrix.t[0] = g_BattleSummonNeoBahamutLineLight1TaskSlot->screenX - g_BattleSummonNeoBahamutLineLight1TaskSlot->geomOffsetX;
    g_BattleSummonNeoBahamutLineLight1TaskSlot->matrix.t[1] = g_BattleSummonNeoBahamutLineLight1TaskSlot->screenY - g_BattleSummonNeoBahamutLineLight1TaskSlot->geomOffsetY;
    g_BattleSummonNeoBahamutLineLight1TaskSlot->matrix.t[2] = GetGeomScreen();
    g_BattleSummonNeoBahamutLineLight1TaskSlot->scaleZ = 0x1000 /* 1.0 */;
    g_BattleSummonNeoBahamutLineLight1TaskSlot->scaleY = 0x1000 /* 1.0 */;
    ScaleMatrix(&g_BattleSummonNeoBahamutLineLight1TaskSlot->matrix, g_BattleSummonNeoBahamutLineLight1TaskSlot);
    SetRotMatrix(&g_BattleSummonNeoBahamutLineLight1TaskSlot->matrix);
    SetTransMatrix(&g_BattleSummonNeoBahamutLineLight1TaskSlot->matrix);
    RefreshGlobalTransformState();
    primitive = (BattleFxLineLightPrim *)BattleFx_SummonNeoBahamutPrepareLineLightPrimitive();
    primitive->color0 = 0x32808080;
    primitive->color2 = 0x32808040;
    primitive->color1 = 0x32808040;
    Gfx_DrawPsxPrimitive(primitive);
    if (g_BattleIsPaused == 0) {
        (pLight->step)++;
        pLight->angle += pLight->angleStep;
    }
}

// Fetches a render state block with Gfx_GetPsxPacketScratchPtr, calls Gfx_TransformTriangleReturnDepthAlt on g_BattleSummonNeoBahamutLineLight1WorkingBlob + 8,
// g_BattleSummonNeoBahamutLineLight1Param1 and g_BattleSummonNeoBahamutLineLight1Param2 with two local out-parameters that are never read afterwards,
// stores the PSX primitive tag 0x6000000 into the block's first word and returns the block.
// FUNCTION: FF7 0x00492c0f
int __cdecl BattleFx_SummonNeoBahamutPrepareLineLightPrimitive(void)
{
    int depthA;
    BattleFxLineLightPrim *block;
    int padStorage; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int depthB;

    block = (BattleFxLineLightPrim *)Gfx_GetPsxPacketScratchPtr();
    Gfx_TransformTriangleReturnDepthAlt((char *)&g_BattleSummonNeoBahamutLineLight1WorkingBlob + 0x8, &g_BattleSummonNeoBahamutLineLight1Param1, (SVECTOR *)&g_BattleSummonNeoBahamutLineLight1Param2, &depthA, &depthB);
    block->command = 0x6000000;
    return (int)block;
}

// Refreshes Neo Bahamut resource field zero. First four counts compute travel value from count
// fraction times 0x7B0C and set retained matrix field g_BattleSummonNeoBahamutLineLight2Step to
// negative travel shifted twelve divided by 9000. While unpaused emits two
// BattleFx_SummonNeoBahamutBreathChildModelTask records with zero X/Y, successive Z positions
// travel minus 500 separated by 0xF61 and random Z rotation. Next 35 hold matrix fields, using
// destination plus 0x70 and shift twelve for both early stages. Next forty ease retained diagonal
// g_BattleSummonNeoBahamutLineLight2Offset from 0x500 toward 0x3000 and set
// g_BattleSummonNeoBahamutLineLight2Rotation to its negative, switching destination to plus 0x78
// and shift zero; next thirty hold, then retire before draw. Multiplies matrix at
// g_BattleSummonNeoBahamutRotationMatrix with retained matrix into
// g_BattleSummonNeoBahamutLineLightMatrix, adds retained point with Y plus 1500/Z plus 7500 and
// invokes MultiplyMatrix using the current source cast of g_BattleSummonNeoBahamutComposedMatrix as
// first argument, then installs/submits. Only counter/emission are pause-gated.
// FUNCTION: FF7 0x00492c4c
void __cdecl BattleFx_SummonNeoBahamutBreathModelTask(void)
{
    int stage;
    int positionZ;
    BattleTaskSlot *taskPtr;
    int spawnBudget;
    int progress;
    int padBlock; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int orderingTable;
    int separation;
    int shift;
    BattleFxNeoBahamutBreathSlot *pChild;
    int travel;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSummonNeoBahamutLineLight1Param2.breathModelGroup) = Gfx_ModelTableGetRenderObject(0, g_BattleSummonNeoBahamutLoadedResTable);
    stage = taskPtr->frameCounter;
    if (stage < 4) {
        progress = (stage << 12) / 4;
        travel = progress * 0x7B0C >> 12;
        g_BattleSummonNeoBahamutLineLight2Step[0] = -(travel << 12) / 9000;
        if (g_BattleIsPaused == 0) {
            spawnBudget = (stage << 9 & 0xFF) + 0x200;
            positionZ = travel - 500;
            separation = 0xF61;
            while (spawnBudget >= 256) {
                pChild = (BattleFxNeoBahamutBreathSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutBreathChildModelTask);
                pChild->posX = 0;
                pChild->posY = 0;
                pChild->posZ = positionZ;
                pChild->rotX = 0;
                pChild->rotY = 0;
                pChild->angle = rand() & 0xFFF;
                positionZ -= separation;
                spawnBudget -= 0x100;
            }
        }
        orderingTable = g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED;
        shift = 0xC;
    } else {
        stage -= 4;
        if (stage < 35) {
            orderingTable = g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED;
            shift = 0xC;
        } else {
            stage -= 35;
            if (stage < 40) {
                progress = (stage << 12) / 40;
                progress = BattleFx_NeoBahamutApplyCameraEasing(progress, 0x180);
                g_BattleSummonNeoBahamutLineLight2Offset[0] = (progress * 0x2B00 /* 2.6875 */ >> 12) + 0x500;
                g_BattleSummonNeoBahamutLineLight2Rotation[0] = -g_BattleSummonNeoBahamutLineLight2Offset[0];
                orderingTable = g_BattleCurrentOrderingTable + 0x78;
                shift = 0;
            } else {
                stage -= 40;
                if (stage < 30) {
                    orderingTable = g_BattleCurrentOrderingTable + 0x78;
                    shift = 0;
                } else {
                    taskPtr->ownerSlot = BATTLE_TASK_FREE;
                    return;
                }
            }
        }
    }
    MultiplyMatrix(&g_BattleSummonNeoBahamutRotationMatrix, g_BattleSummonNeoBahamutLineLight2Rotation, &g_BattleSummonNeoBahamutLineLightMatrix);
    g_BattleSummonNeoBahamutLineLightMatrix.t[0] += (g_BattleSummonNeoBahamutAnchorPosition[0]);
    g_BattleSummonNeoBahamutLineLightMatrix.t[1] = g_BattleSummonNeoBahamutLineLightMatrix.t[1] + (g_BattleSummonNeoBahamutAnchorPosition[1]) + 0x5DC;
    g_BattleSummonNeoBahamutLineLightMatrix.t[2] = g_BattleSummonNeoBahamutLineLightMatrix.t[2] + (g_BattleSummonNeoBahamutAnchorPosition[2]) + 0x1D4C;
    MultiplyMatrix(&g_BattleSummonNeoBahamutComposedMatrix, &g_BattleSummonNeoBahamutLineLightMatrix, &g_BattleSummonNeoBahamutLineLightMatrix);
    SetRotMatrix(&g_BattleSummonNeoBahamutLineLightMatrix);
    SetTransMatrix(&g_BattleSummonNeoBahamutLineLightMatrix);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)(((char *)&g_BattleSummonNeoBahamutLineLight1Param2 + 0x18)), orderingTable, shift, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (taskPtr->frameCounter)++;
    }
}

// Draws one puff of the breath for ten frames: eases its uniform scale up from 0x400 and its fade
// in, places it at the task's position and rotation in the summon's frame, and spins it about Z.
// FUNCTION: FF7 0x00492f27
void __cdecl BattleFx_SummonNeoBahamutBreathChildModelTask(void)
{
    int stage;
    BattleFxNeoBahamutBreathChild *child;
    int ratio;

    child = (BattleFxNeoBahamutBreathChild *)BATTLE_CURRENT_TASK_SLOT;
    if (((unsigned char)g_BattleSummonNeoBahamutBreathChildInitFlag & 1) == 0) {
        g_BattleSummonNeoBahamutBreathChildInitFlag |= 1;
        g_BattleSummonNeoBahamutBreathScratch.scratch = (BattleFxNeoBahamutBreathScratch *)Gfx_GetPsxScratchpadPtr(0);
    }
    (g_BattleSummonNeoBahamutLineLight1Param2.resourceHandle) = Gfx_ModelTableGetRenderObject(15, g_BattleSummonNeoBahamutLoadedResTable);
    stage = child->frameIndex;
    if (stage < 10) {
        ratio = (stage << 12) / 10;
        ratio = BattleFx_NeoBahamutApplyCameraEasing(ratio, 0x1FF);
        g_BattleSummonNeoBahamutBreathScratch.scratch->scaleZ = (ratio << 10 >> 12) + 0x400;
        g_BattleSummonNeoBahamutBreathScratch.scratch->scaleY = g_BattleSummonNeoBahamutBreathScratch.scratch->scaleZ;
        g_BattleSummonNeoBahamutBreathScratch.scratch->scaleX = g_BattleSummonNeoBahamutBreathScratch.scratch->scaleY;
        (g_BattleSummonNeoBahamutLineLight1Param2.childFadeLevel) = ratio;
    } else {
        child->ownerSlot = BATTLE_TASK_FREE;
        return;
    }
    RotMatrix((SVECTOR *)((char *)child + 0xC), &g_BattleSummonNeoBahamutBreathScratch.scratch->matrix);
    g_BattleSummonNeoBahamutBreathScratch.scratch->matrix.t[0] = child->posX;
    g_BattleSummonNeoBahamutBreathScratch.scratch->matrix.t[1] = child->posY;
    g_BattleSummonNeoBahamutBreathScratch.scratch->matrix.t[2] = child->posZ;
    ScaleMatrix(&g_BattleSummonNeoBahamutBreathScratch.scratch->matrix, g_BattleSummonNeoBahamutBreathScratch.scratch);
    MultiplyMatrix(&g_BattleSummonNeoBahamutRotationMatrix, &g_BattleSummonNeoBahamutBreathScratch.scratch->matrix, &g_BattleSummonNeoBahamutBreathScratch.scratch->matrix);
    MultiplyMatrix(&g_BattleSummonNeoBahamutComposedMatrix, &g_BattleSummonNeoBahamutBreathScratch.scratch->matrix, &g_BattleSummonNeoBahamutBreathScratch.scratch->matrix);
    SetRotMatrix(&g_BattleSummonNeoBahamutBreathScratch.scratch->matrix);
    SetTransMatrix(&g_BattleSummonNeoBahamutBreathScratch.scratch->matrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)(((char *)&g_BattleSummonNeoBahamutLineLight1Param2 + 0x8)), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (child->frameIndex)++;
        child->angle += 0x80;
    }
}

// Acquires retained scratch before pause. Unpaused allocates one BattleFx_SummonNeoBahamutRock2ModelTask in a one-iteration
// loop, selecting variant zero through two and ground X/Z random -3000 through 2999 with Y zero.
// Forms integer direction toward g_BattleSummonNeoBahamutAnchorPosition, arithmetic-halves all components until within
// plus/minus 0x4000 and normalizes. Sets velocity from that direction times random 300 through 499
// shifted twelve and opposite X/Z acceleration from random 70 through 99 with Y acceleration zero.
// Writes random short 32 through 287 at offset sixteen through pointer
// g_BattleSummonNeoBahamutBreathChildModelParam and saves that shared pointer in the record, then
// advances count/retire at forty.
// FUNCTION: FF7 0x00493133
void __cdecl BattleFx_SummonNeoBahamutSpawnRock2ModelsTask(void)
{
    BattleTaskSlot *task;
    int rollVal;
    int zRand;
    int xRand;
    BattleFxNeoBahamutRock2Spawn *pRockTask;
    int rockIndex;
    int sizeRand;
    int randAccel;
    int randVel;

    task = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonNeoBahamutRock2SpawnInitFlag[0] & 1) == 0) {
        g_BattleSummonNeoBahamutRock2SpawnInitFlag[0] |= 1;
        g_BattleSummonNeoBahamutRock2SpawnTaskSlot = (BattleFxDebrisSpawnTask *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    for (rockIndex = 0; rockIndex < 1; rockIndex++) {
        pRockTask = (BattleFxNeoBahamutRock2Spawn *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutRock2ModelTask);
        pRockTask->variant = rand() % 3;
        if (DEAD_IN_RETAIL != 0) {
            xRand = 3000;
        } else {
            xRand = rand() % 6000 - 3000;
        }
        pRockTask->posX = xRand;
        pRockTask->posY = 0;
        if (DEAD_IN_RETAIL != 0) {
            zRand = 3000;
        } else {
            zRand = rand() % 6000 - 3000;
        }
        pRockTask->posZ = zRand;
        g_BattleSummonNeoBahamutRock2SpawnTaskSlot->offsetX = (g_BattleSummonNeoBahamutAnchorPosition[0]) - pRockTask->posX;
        g_BattleSummonNeoBahamutRock2SpawnTaskSlot->offsetY = (g_BattleSummonNeoBahamutAnchorPosition[1]) - pRockTask->posY;
        g_BattleSummonNeoBahamutRock2SpawnTaskSlot->offsetZ = (g_BattleSummonNeoBahamutAnchorPosition[2]) - pRockTask->posZ;
        while (g_BattleSummonNeoBahamutRock2SpawnTaskSlot->offsetX < -0x4000 || (g_BattleSummonNeoBahamutRock2SpawnTaskSlot->offsetX > 0x4000 || (g_BattleSummonNeoBahamutRock2SpawnTaskSlot->offsetY < -0x4000 || (g_BattleSummonNeoBahamutRock2SpawnTaskSlot->offsetY > 0x4000 || (g_BattleSummonNeoBahamutRock2SpawnTaskSlot->offsetZ < -0x4000 || g_BattleSummonNeoBahamutRock2SpawnTaskSlot->offsetZ > 0x4000))))) {
            g_BattleSummonNeoBahamutRock2SpawnTaskSlot->offsetX >>= 1;
            g_BattleSummonNeoBahamutRock2SpawnTaskSlot->offsetY >>= 1;
            g_BattleSummonNeoBahamutRock2SpawnTaskSlot->offsetZ >>= 1;
        }
        NormalizeVectorFixed(g_BattleSummonNeoBahamutRock2SpawnTaskSlot, g_BattleSummonNeoBahamutRock2SpawnTaskSlot);
        if (DEAD_IN_RETAIL != 0) {
            randVel = 500;
        } else {
            randVel = rand() % 200 + 300;
        }
        rollVal = randVel;
        pRockTask->velocityX = g_BattleSummonNeoBahamutRock2SpawnTaskSlot->offsetX * rollVal >> 12;
        pRockTask->velocityY = g_BattleSummonNeoBahamutRock2SpawnTaskSlot->offsetY * rollVal >> 12;
        pRockTask->velocityZ = g_BattleSummonNeoBahamutRock2SpawnTaskSlot->offsetZ * rollVal >> 12;
        if (DEAD_IN_RETAIL != 0) {
            randAccel = 100;
        } else {
            randAccel = rand() % 30 + 70;
        }
        rollVal = randAccel;
        pRockTask->dirX = -g_BattleSummonNeoBahamutRock2SpawnTaskSlot->offsetX * rollVal >> 12;
        pRockTask->accelerationY = 0;
        pRockTask->dirZ = -g_BattleSummonNeoBahamutRock2SpawnTaskSlot->offsetZ * rollVal >> 12;
        if (DEAD_IN_RETAIL != 0) {
            sizeRand = 288;
        } else {
            sizeRand = rand() % 256 + 32;
        }
        *(short *)((char *)(*(int *)&g_BattleSummonNeoBahamutBreathChildModelParam) + 0x10) = sizeRand;
        pRockTask->childModelParam = (*(int *)&g_BattleSummonNeoBahamutBreathChildModelParam);
    }
    (task->frameCounter)++;
    if (task->frameCounter >= 40) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Acquires retained scratch. Below count twenty clears descriptor term; next ten ramp it to 4096;
// later retire before draw/pause. Builds rotation and uniform scale from the shared pointer at
// task 0x1C, using its first rotation shorts and size at sixteen with task XYZ translation.
// Composes g_BattleSummonNeoBahamutComposedMatrix, installs and draws black variant-indexed
// resource from g_BattleNeoBahamutPrimaryResourceIds into g_BattleCurrentOrderingTable plus 0x70
// with shift twelve. While unpaused advances XYZ, damps velocity by 0xF80 shifted twelve with saved
// X/Z acceleration, then eases old count divided by thirty using parameter 0xFFFFFD20 and
// accumulates shared X/Z rotation by term times 0x312/0x25, retaining a zero Y increment. Advances
// count last.
// FUNCTION: FF7 0x0049346b
void __cdecl BattleFx_SummonNeoBahamutRock2ModelTask(void)
{
    int stage;
    int progress;
    BattleFxNeoBahamutRock2Task *rockPtr;

    rockPtr = (BattleFxNeoBahamutRock2Task *)BATTLE_CURRENT_TASK_SLOT;
    if (((unsigned char)g_BattleSummonNeoBahamutRock2InitFlag & 1) == 0) {
        g_BattleSummonNeoBahamutRock2InitFlag |= 1;
        g_BattleSummonNeoBahamutRock2TaskSlot = (BattleFxNeoBahamutRock2State *)Gfx_GetPsxScratchpadPtr(0);
    }
    stage = rockPtr->frameIndex;
    if (stage < 20) {
        (g_BattleSummonNeoBahamutLineLight2WorkingBlob.blendFraction) = 0;
    } else {
        stage -= 20;
        if (stage < 10) {
            progress = (stage << 12) / 10;
            (g_BattleSummonNeoBahamutLineLight2WorkingBlob.blendFraction) = progress;
        } else {
            rockPtr->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    RotMatrix((SVECTOR *)rockPtr->rotation, &g_BattleSummonNeoBahamutRock2TaskSlot->matrix);
    g_BattleSummonNeoBahamutRock2TaskSlot->matrix.t[0] = rockPtr->posX;
    g_BattleSummonNeoBahamutRock2TaskSlot->matrix.t[1] = rockPtr->posY;
    g_BattleSummonNeoBahamutRock2TaskSlot->matrix.t[2] = rockPtr->posZ;
    g_BattleSummonNeoBahamutRock2TaskSlot->targetValue = rockPtr->rotation->size;
    g_BattleSummonNeoBahamutRock2TaskSlot->curValue = g_BattleSummonNeoBahamutRock2TaskSlot->targetValue;
    g_BattleSummonNeoBahamutRock2TaskSlot->prevValue = g_BattleSummonNeoBahamutRock2TaskSlot->curValue;
    ScaleMatrix(&g_BattleSummonNeoBahamutRock2TaskSlot->matrix, &g_BattleSummonNeoBahamutRock2TaskSlot->prevValue);
    MultiplyMatrix(&g_BattleSummonNeoBahamutComposedMatrix, &g_BattleSummonNeoBahamutRock2TaskSlot->matrix, &g_BattleSummonNeoBahamutRock2TaskSlot->matrix);
    SetRotMatrix(&g_BattleSummonNeoBahamutRock2TaskSlot->matrix);
    SetTransMatrix(&g_BattleSummonNeoBahamutRock2TaskSlot->matrix);
    SetDrawColorRGB(0, 0, 0);
    (g_BattleSummonNeoBahamutLineLight2WorkingBlob.modelGroup) = Gfx_ModelTableGetRenderObject(((int *)&g_BattleNeoBahamutPrimaryResourceIds)[rockPtr->ownerSlot], g_BattleSummonNeoBahamutLoadedResTable);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)(((char *)&g_BattleSummonNeoBahamutLineLight2WorkingBlob + 0x8)), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        rockPtr->posX += rockPtr->velocityX;
        rockPtr->posY += rockPtr->velocityY;
        rockPtr->posZ += rockPtr->velocityZ;
        rockPtr->velocityX = (rockPtr->velocityX * 0xF80 >> 12) + rockPtr->accelerationX;
        rockPtr->velocityY = rockPtr->velocityY * 0xF80 >> 12;
        rockPtr->velocityZ = (rockPtr->velocityZ * 0xF80 >> 12) + rockPtr->accelerationZ;
        progress = (rockPtr->frameIndex << 12) / 30;
        progress = BattleFx_NeoBahamutApplyCameraEasing(progress, 0xFFFFFD20);
        rockPtr->rotation->angleX += (short)(progress * 0x312 >> 12);
        rockPtr->rotation->angleY += (short)(UNFOLDED(0) >> 12);
        rockPtr->rotation->angleZ += (short)(progress * 0x25 >> 12);
        (rockPtr->frameIndex)++;
    }
}

// Selects model field two, holds its request short at zero for counters below fifteen and ramps it
// during the next fifteen, retiring at thirty or above before drawing even while paused. Computes
// sine-eased scale from the counter, installs g_BattleSummonNeoBahamutBillboardWorkingBlob through BattleFx_SummonNeoBahamutInstallBillboardMatrix, draws in black
// and advances only when unpaused.
// FUNCTION: FF7 0x00493775
void __cdecl BattleFx_SummonNeoBahamutFlash2ModelTask(void)
{
    struct {
        int temp;
        int unused;
        int p;
        int frame;
        int scale;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.p = (int)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSummonNeoBahamutBreathChildModelParam.modelResource) = Gfx_ModelTableGetRenderObject(2, g_BattleSummonNeoBahamutLoadedResTable);
    local.frame = *(short *)((char *)local.p + 2);
    if (local.frame < 15) {
        (g_BattleSummonNeoBahamutBreathChildModelParam.rampValue) = 0;
    } else {
        local.frame -= 15;
        if (local.frame < 15) {
            local.temp = (local.frame << 12) / 15;
            (g_BattleSummonNeoBahamutBreathChildModelParam.rampValue) = (short)local.temp;
        } else {
            *(short *)local.p = 0xFFFF;
            return;
        }
    }
    local.temp = (*(short *)((char *)local.p + 2) << 12) / 0x1E;
    local.temp = BattleFx_NeoBahamutApplyCameraEasing(local.temp, -0x210);
    local.scale = (local.temp * 0x4B00 /* 4.6875 */ >> 12) + 0x500;
    BattleFx_SummonNeoBahamutInstallBillboardMatrix((int *)&g_BattleSummonNeoBahamutBillboardWorkingBlob, (short)local.scale, 0);
    SetDrawColorRGB(0, 0, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)((&g_BattleSummonNeoBahamutBreathChildModelParam.modelResource)), g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST_3, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (*(short *)((char *)local.p + 2))++;
    }
}

// Returns while paused and emits six BattleFx_SummonNeoBahamutRock3ModelTask records. Sets variant
// zero through two, random X/Z -2000 through 1999, Y 0xB1E0, Y velocity random zero through 99,
// size 80 through 575 and random low-24-bit colour. Leaves other record fields to
// allocator/previous state without initialization here. Advances count and retires at ten.
// FUNCTION: FF7 0x0049389f
void __cdecl BattleFx_SummonNeoBahamutSpawnRock3ModelsTask(void)
{
    BattleTaskSlot *taskPtr;
    int rockIdx;
    BattleFxNeoBahamutRock3Model *rock;
    int zRand;
    int xRand;
    int sizeRand;
    int randVel;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    for (rockIdx = 0; rockIdx < 6; rockIdx++) {
        rock = (BattleFxNeoBahamutRock3Model *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutRock3ModelTask);
        rock->variant = rand() % 3;
        if (DEAD_IN_RETAIL != 0) {
            xRand = 2000;
        } else {
            xRand = rand() % 4000 - 2000;
        }
        rock->posX = xRand;
        rock->posY = 0xB1E0;
        if (DEAD_IN_RETAIL != 0) {
            zRand = 2000;
        } else {
            zRand = rand() % 4000 - 2000;
        }
        rock->posZ = zRand;
        if (DEAD_IN_RETAIL != 0) {
            randVel = 100;
        } else {
            randVel = rand() % 100;
        }
        rock->spawnX = randVel;
        if (DEAD_IN_RETAIL != 0) {
            sizeRand = 576;
        } else {
            sizeRand = rand() % 496 + 80;
        }
        rock->scale = sizeRand;
        rock->randomValue = rand() & 0xFFFFFF;
    }
    (taskPtr->frameCounter)++;
    if (taskPtr->frameCounter >= 10) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Acquires retained scratch and builds rotation from task unsigned bytes 0x14 through 0x16 shifted
// four. Uses task X plus BattleFx_SummonNeoBahamutHeightToXOffset(task Y), unchanged task Y/Z and uniform saved size; scales,
// multiplies g_BattleSummonNeoBahamutComposedMatrix and installs. Selects variant-indexed resource through g_BattleNeoBahamutPrimaryResourceIds and
// submits into g_BattleCurrentOrderingTable plus 0x70 with shift twelve, retaining draw colour. While unpaused
// advances only Y by its velocity; at nonnegative new Y retires and skips remaining updates.
// Otherwise damps Y velocity by 0xFC0 shifted twelve plus sixty, increments rotation bytes by
// one/eight/sixteen and advances count.
// FUNCTION: FF7 0x00493a11
void __cdecl BattleFx_SummonNeoBahamutRock3ModelTask(void)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxNeoBahamutRockModel *q;
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    q = (BattleFxNeoBahamutRockModel *)BATTLE_CURRENT_TASK_SLOT;
    if (((unsigned char)g_BattleSummonNeoBahamutRock3InitFlag & 1) == 0) {
        g_BattleSummonNeoBahamutRock3InitFlag |= 1;
        g_BattleSummonNeoBahamutRock3TaskSlot = (BattleFxShardMeshRotation *)Gfx_GetPsxScratchpadPtr(0);
    }
    g_BattleSummonNeoBahamutRock3TaskSlot->angleX = q->rotX << 4;
    g_BattleSummonNeoBahamutRock3TaskSlot->angleY = q->rotY << 4;
    g_BattleSummonNeoBahamutRock3TaskSlot->angleZ = q->rotZ << 4;
    RotMatrix((SVECTOR *)g_BattleSummonNeoBahamutRock3TaskSlot, &g_BattleSummonNeoBahamutRock3TaskSlot->matrix);
    g_BattleSummonNeoBahamutRock3TaskSlot->matrix.t[0] = q->baseX + BattleFx_SummonNeoBahamutHeightToXOffset(q->height);
    g_BattleSummonNeoBahamutRock3TaskSlot->matrix.t[1] = q->height;
    g_BattleSummonNeoBahamutRock3TaskSlot->matrix.t[2] = q->baseZ;
    g_BattleSummonNeoBahamutRock3TaskSlot->valueNew = q->scale;
    g_BattleSummonNeoBahamutRock3TaskSlot->valuePrev = g_BattleSummonNeoBahamutRock3TaskSlot->valueNew;
    g_BattleSummonNeoBahamutRock3TaskSlot->valueOld = g_BattleSummonNeoBahamutRock3TaskSlot->valuePrev;
    ScaleMatrix(&g_BattleSummonNeoBahamutRock3TaskSlot->matrix, &g_BattleSummonNeoBahamutRock3TaskSlot->valueOld);
    MultiplyMatrix(&g_BattleSummonNeoBahamutComposedMatrix, &g_BattleSummonNeoBahamutRock3TaskSlot->matrix, &g_BattleSummonNeoBahamutRock3TaskSlot->matrix);
    SetRotMatrix(&g_BattleSummonNeoBahamutRock3TaskSlot->matrix);
    SetTransMatrix(&g_BattleSummonNeoBahamutRock3TaskSlot->matrix);
    (*(int *)((char *)&g_BattleSummonNeoBahamutBillboardWorkingBlob + 0x8)) = Gfx_ModelTableGetRenderObject(((int *)&g_BattleNeoBahamutPrimaryResourceIds)[q->ownerSlot], g_BattleSummonNeoBahamutLoadedResTable);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)(((char *)&g_BattleSummonNeoBahamutBillboardWorkingBlob + 0x8)), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        q->height += q->riseSpeed;
        if (q->height >= 0) {
            q->ownerSlot = BATTLE_TASK_FREE;
        } else {
            q->riseSpeed = (q->riseSpeed * 0xFC0 >> 12) + 60;
            q->rotX++;
            q->rotY += 8;
            q->rotZ += 0x10;
            (q->timer)++;
        }
    }
}

// Returns height multiplied by -5000 and then divided by -20000 using signed integer arithmetic.
// FUNCTION: FF7 0x00493c5a
int __cdecl BattleFx_SummonNeoBahamutHeightToXOffset(int height)
{
    return height * -5000 / -20000;
}

// Allocates a slot for BattleFx_SummonNeoBahamutActorRiseAndFallTask and stores actor in its first word.
// FUNCTION: FF7 0x00493c70
void __cdecl BattleFx_SummonSpawnNeoBahamutPerTarget(int actor)
{
    int slot;

    slot = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutActorRiseAndFallTask);
    *(short *)slot = actor;
}

// BattleFx_SummonNeoBahamutActorRiseAndFallTask -- 684 bytes. Complete code through the ret at 0x00493f48;
// Acquires retained scratch before pause, then returns while paused. Count zero saves three actor
// position shorts at task offsets four/six/eight. First 45 counts set actor Y to count fraction
// times -5000; next hundred set Y to elapsed fraction times -20000. Next 320 reset Y to zero on
// entry; next thirty leave it unchanged. Last forty reset saved velocity on entry and add 0xB1E0
// to Y, then while pre-update Y is negative advance it and damp velocity by 0xFC0 shifted twelve
// plus sixty. Transforms an X vector from BattleFx_SummonNeoBahamutHeightToXOffset(new Y) through g_BattleSummonNeoBahamutWorldMatrix and adds its
// X/Z to saved position; otherwise sets Y zero. Later copies eight bytes from task offset four
// back to actor position, retires and returns; only six of those saved bytes are initialized here.
// Advances count after active phases.
// FUNCTION: FF7 0x00493c9d
void __cdecl BattleFx_SummonNeoBahamutActorRiseAndFallTask(void)
{
    int stage;
    BattleFxNeoBahamutRiseFall *taskPtr;
    int progress;
    BattleActorRiseFallRecord *actor;

    taskPtr = (BattleFxNeoBahamutRiseFall *)BATTLE_CURRENT_TASK_SLOT;
    actor = (BattleActorRiseFallRecord *)&g_BattleActorRecords[taskPtr->actorIndex].modelId;
    if (((unsigned char)g_BattleSummonNeoBahamutActorRiseAndFallInitFlag & 1) == 0) {
        g_BattleSummonNeoBahamutActorRiseAndFallInitFlag |= 1;
        g_BattleSummonNeoBahamutActorRiseAndFallTaskSlot = (Vec3sPacked *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->timer == 0) {
        taskPtr->baseX = actor->posA;
        taskPtr->baseY = actor->posB;
        taskPtr->baseZ = actor->posC;
    }
    stage = taskPtr->timer;
    if (stage < 45) {
        progress = (stage << 12) / 45;
        actor->posB = progress * -0x1388 >> 12;
    } else {
        stage -= 45;
        if (stage < 100) {
            progress = (stage << 12) / 100;
            actor->posB = progress * -0x4E20 >> 12;
        } else {
            stage -= 100;
            if (stage < 320) {
                if (stage == 0) {
                    actor->posB = 0;
                }
            } else {
                stage -= 320;
                if (stage < 30) { /* empty first arm: lib/codegen.h */
                } else {
                    stage -= 30;
                    if (stage < 40) {
                        if (stage == 0) {
                            taskPtr->velocityY = 0;
                            actor->posB += 0xB1E0;
                        }
                        if (actor->posB < 0) {
                            actor->posB += taskPtr->velocityY;
                            taskPtr->velocityY = (taskPtr->velocityY * 0xFC0 >> 12) + 60;
                            g_BattleSummonNeoBahamutActorRiseAndFallTaskSlot->vx = BattleFx_SummonNeoBahamutHeightToXOffset(actor->posB);
                            g_BattleSummonNeoBahamutActorRiseAndFallTaskSlot->vy = 0;
                            g_BattleSummonNeoBahamutActorRiseAndFallTaskSlot->vz = 0;
                            TransformVectorToInt16(&g_BattleSummonNeoBahamutWorldMatrix, (void *)g_BattleSummonNeoBahamutActorRiseAndFallTaskSlot, (void *)g_BattleSummonNeoBahamutActorRiseAndFallTaskSlot);
                            actor->posA = taskPtr->baseX + g_BattleSummonNeoBahamutActorRiseAndFallTaskSlot->vx;
                            actor->posC = taskPtr->baseZ + g_BattleSummonNeoBahamutActorRiseAndFallTaskSlot->vz;
                        } else {
                            actor->posB = 0;
                        }
                    } else {
                        *(Int64Pair *)((char *)actor + 0x166) = *(Int64Pair *)((char *)taskPtr + 4);
                        taskPtr->actorIndex = 0xFFFF;
                        return;
                    }
                }
            }
        }
    }
    (taskPtr->timer)++;
}

// Returns while paused. Local emission term is parent count shifted eight AND 0xFF plus 0x100,
// yielding one BattleFx_SummonNeoBahamutLineLight2PrimitiveTask record for ordinary counts. Saves
// parent count as first short, sets position (-5000,-20000,0), clears X/Y rotation and assigns
// random Z rotation -0x200 through 0x5FF and size 0x800 through 0x17FF. Discards emission
// remainder, advances count and retires at forty.
// FUNCTION: FF7 0x00493f49
void __cdecl BattleFx_SummonNeoBahamutSpawnLineLight2Task(void)
{
    BattleFxBurstSpawnerTask *taskPtr;
    int spawnBudget;
    int rotRoll;
    BattleFxNeoBahamutLine2Spawn *pLine;
    int bytes;

    taskPtr = (BattleFxBurstSpawnerTask *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    spawnBudget = (taskPtr->spawnIndex << 8 & 0xFF) + 0x100;
    while (spawnBudget >= 256) {
        pLine = (BattleFxNeoBahamutLine2Spawn *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonNeoBahamutLineLight2PrimitiveTask);
        pLine->spawnIndex = taskPtr->spawnIndex;
        pLine->posX = 0xEC78;
        pLine->posY = 0xB1E0;
        pLine->posZ = 0;
        pLine->rotY = 0;
        pLine->rotX = 0;
        if (DEAD_IN_RETAIL != 0) {
            rotRoll = 0x600 /* 135 deg */;
        } else {
            rotRoll = (rand() % 0x800) - 0x200;
        }
        pLine->rotZ = rotRoll;
        if (DEAD_IN_RETAIL != 0) {
            bytes = 0x1800;
        } else {
            bytes = (rand() % 4096) + 2048;
        }
        pLine->size = bytes;
        spawnBudget -= 0x100;
    }
    (taskPtr->spawnIndex)++;
    if (taskPtr->spawnIndex >= 40) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Acquires retained scratch. Ramps X scale to saved size over five counts, then falls over five,
// retaining an empty negative-duration hold branch; later retires before draw/pause. Projects task
// position through g_BattleSummonNeoBahamutComposedMatrix without testing projection flags, builds saved rotation and places
// matrix at projected coordinates minus geometry offset with Z equal to geometry screen. Keeps Y/Z
// scale 4096, scales and installs matrix, refreshes transform state, obtains a primitive from
// BattleFx_SummonNeoBahamutPrepareLineLightPrimitive, writes colour/command words 0x32808080 and twice 0x32208080 and submits. While
// unpaused advances count and retires when new count plus first task short reaches forty.
// FUNCTION: FF7 0x00494091
void __cdecl BattleFx_SummonNeoBahamutLineLight2PrimitiveTask(void)
{
    int fraction;
    BattleFxNeoBahamutLineLight *line;
    int stage;
    BattleFxLineLightPrim *primitive;

    line = (BattleFxNeoBahamutLineLight *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonNeoBahamutLineLight2InitFlag[0] & 1) == 0) {
        g_BattleSummonNeoBahamutLineLight2InitFlag[0] |= 1;
        g_BattleSummonNeoBahamutLineLight2TaskSlot = (BattleFxLineLight2Task *)Gfx_GetPsxScratchpadPtr(0);
    }
    stage = line->step;
    if (stage < 5) {
        fraction = (stage << 12) / 5;
        g_BattleSummonNeoBahamutLineLight2TaskSlot->lerpedValue = line->intensity * fraction >> 12;
    } else {
        stage -= 5;
        if (stage < 0) {
            g_BattleSummonNeoBahamutLineLight2TaskSlot->lerpedValue = line->intensity;
        } else {
            stage = stage;
            if (stage < 5) {
                fraction = (stage << 12) / 5;
                g_BattleSummonNeoBahamutLineLight2TaskSlot->lerpedValue = line->intensity * (0x1000 - fraction) >> 12;
            } else {
                line->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    SetRotMatrix(&g_BattleSummonNeoBahamutComposedMatrix);
    SetTransMatrix(&g_BattleSummonNeoBahamutComposedMatrix);
    g_BattleSummonNeoBahamutLineLight2TaskSlot->projectedDepth = RotTransPers((SVECTOR *)&line->posX, (short *)&g_BattleSummonNeoBahamutLineLight2TaskSlot->screenX, (int *)((char *)g_BattleSummonNeoBahamutLineLight2TaskSlot + 0x38), (int *)((char *)g_BattleSummonNeoBahamutLineLight2TaskSlot + 0x3C));
    RotMatrix((SVECTOR *)&line->rotX, &g_BattleSummonNeoBahamutLineLight2TaskSlot->matrix);
    GetGeomOffset(&g_BattleSummonNeoBahamutLineLight2TaskSlot->geomOffsetX, &g_BattleSummonNeoBahamutLineLight2TaskSlot->geomOffsetY);
    g_BattleSummonNeoBahamutLineLight2TaskSlot->matrix.t[0] = g_BattleSummonNeoBahamutLineLight2TaskSlot->screenX - g_BattleSummonNeoBahamutLineLight2TaskSlot->geomOffsetX;
    g_BattleSummonNeoBahamutLineLight2TaskSlot->matrix.t[1] = g_BattleSummonNeoBahamutLineLight2TaskSlot->screenY - g_BattleSummonNeoBahamutLineLight2TaskSlot->geomOffsetY;
    g_BattleSummonNeoBahamutLineLight2TaskSlot->matrix.t[2] = GetGeomScreen();
    g_BattleSummonNeoBahamutLineLight2TaskSlot->scaleZ = 0x1000 /* 1.0 */;
    g_BattleSummonNeoBahamutLineLight2TaskSlot->scaleY = 0x1000 /* 1.0 */;
    ScaleMatrix(&g_BattleSummonNeoBahamutLineLight2TaskSlot->matrix, g_BattleSummonNeoBahamutLineLight2TaskSlot);
    SetRotMatrix(&g_BattleSummonNeoBahamutLineLight2TaskSlot->matrix);
    SetTransMatrix(&g_BattleSummonNeoBahamutLineLight2TaskSlot->matrix);
    RefreshGlobalTransformState();
    primitive = (BattleFxLineLightPrim *)BattleFx_SummonNeoBahamutPrepareLineLightPrimitive();
    primitive->color0 = 0x32808080;
    primitive->color2 = 0x32208080;
    primitive->color1 = 0x32208080;
    Gfx_DrawPsxPrimitive(primitive);
    if (g_BattleIsPaused == 0) {
        (line->step)++;
        if (line->step + line->ownerSlot >= 40) {
            line->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

// Saves or restores one flag byte per actor, toggling g_BattleSummonNeoBahamutLineLight2State between 0 and 1 at the end to
// say which half ran. Over the ten actor slots selected by the bitmask g_BattleSummonNeoBahamutTargetMask: when
// g_BattleSummonNeoBahamutLineLight2State was 0 it copies the byte at g_BattleActorRecords[slot].drawFlags into g_BattleSummonNeoBahamutActorMaskFlags[slot]
// and then sets bit 0x10 in the original; when it was 1 it copies the saved byte back. 0x1AEC is
// the actor record stride.
// FUNCTION: FF7 0x00494308
void __cdecl BattleFx_SummonNeoBahamutToggleActorMaskFlags(void)
{
    int i;

    if (g_BattleSummonNeoBahamutLineLight2State == 0) {
        for (i = 0; i < 10; i++) {
            if ((g_BattleSummonNeoBahamutTargetMask & 1 << i) != 0) {
                g_BattleSummonNeoBahamutActorMaskFlags[i] = g_BattleActorRecords[i].drawFlags;
                g_BattleActorRecords[i].drawFlags |= 0x10;
            }
        }
    } else {
        for (i = 0; i < 10; i++) {
            if ((g_BattleSummonNeoBahamutTargetMask & 1 << i) != 0) {
                g_BattleActorRecords[i].drawFlags = (char)g_BattleSummonNeoBahamutActorMaskFlags[i];
            }
        }
    }
    g_BattleSummonNeoBahamutLineLight2State ^= 1;
}
} /* extern "C" */
