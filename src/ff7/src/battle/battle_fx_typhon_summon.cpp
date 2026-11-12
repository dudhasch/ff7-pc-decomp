// src/ff7/src/battle/battle_fx_typhon_summon.cpp
/* Summon [13]: Typhon (Japanese: テュポーン, dev tag: "SUMMON/TYPHOON") */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Runs the Typhon summon sequence: its camera script interpreter, timeline task, particle
//   callbacks, screen fades, windstorm rings and stage-background warp, plus the model drawing
//   helpers. Together they drive the whole summon from start to finish.

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
#include "lib/codegen.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle_summon.h"

typedef union BattleFxTyphonMeshCursor {
    short countWord;
    int headerInt;
    BattleSummonTyphonMeshRecord record;
    BattleFxPsxMeshPacket ifritRecord;
} BattleFxTyphonMeshCursor;
#include "ff7/types/battle_3d.h"
#include "ff7/enums/battle.h"
#include "lib/enums/graphics.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x0082eff0
unsigned char g_BattleSummonTyphonAnimTable0[16] = {0x1a, 0x02, 0x00, 0x00, 0x1b, 0x02, 0x00, 0x00, 0x1c, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0082f000
unsigned char g_BattleSummonTyphonAnimTable1[16] = {0x1d, 0x02, 0x00, 0x00, 0x1e, 0x02, 0x00, 0x00, 0x1f, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0082f010
unsigned char g_BattleSummonTyphonAnimTable2[16] = {0x20, 0x02, 0x00, 0x00, 0x21, 0x02, 0x00, 0x00, 0x22, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0082f020
unsigned char g_BattleSummonTyphonAnimTable3[16] = {0x23, 0x02, 0x00, 0x00, 0x24, 0x02, 0x00, 0x00, 0x25, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0082f030
unsigned int g_BattleSummonTyphonResourceDef[5] = {(unsigned int)&g_BattleSummonTyphonAnimTable0, (unsigned int)&g_BattleSummonTyphonAnimTable1, (unsigned int)&g_BattleSummonTyphonAnimTable2, (unsigned int)&g_BattleSummonTyphonAnimTable3, 0x00000000};
// GLOBAL: FF7 0x0082f048
unsigned char g_BattleSummonTyphonModelMesh0[440] = {0x60, 0x00, 0x00, 0x00, 0x44, 0xff, 0x82, 0x05, 0x0e, 0x01, 0x00, 0x00, 0x0e, 0xf8, 0xa9, 0x00, 0x4b, 0x03, 0x00, 0x00, 0x78, 0xfc, 0x5c, 0x03, 0xd0, 0xf7, 0x00, 0x00, 0x03, 0xff, 0x8b, 0x03, 0xfd, 0x08, 0x00, 0x00, 0xc1, 0x09, 0x13, 0x01, 0x5b, 0x05, 0x00, 0x00, 0x69, 0x04, 0x09, 0x03, 0x33, 0xf8, 0x00, 0x00, 0x0e, 0xf8, 0xaf, 0xfc, 0xfa, 0xfe, 0x00, 0x00, 0x72, 0xff, 0xe2, 0xfd, 0x64, 0xf4, 0x00, 0x00, 0x5f, 0xf9, 0x2f, 0xfe, 0x5a, 0x09, 0x00, 0x00, 0x8a, 0x04, 0xc9, 0xfc, 0x76, 0x07, 0x00, 0x00, 0xc1, 0x09, 0xf0, 0xfe, 0x60, 0xfe, 0x00, 0x00, 0x68, 0xff, 0xa9, 0xfa, 0x1a, 0x01, 0x00, 0x00, 0x14, 0x00, 0x2c, 0x01, 0x00, 0x00, 0x08, 0x00, 0x10, 0x00, 0x60, 0x00, 0x8e, 0x11, 0x00, 0x00, 0x80, 0x0c, 0x88, 0x23, 0x00, 0x00, 0x18, 0x00, 0x08, 0x00, 0x60, 0x00, 0x8e, 0x11, 0x00, 0x00, 0x8d, 0x01, 0x80, 0x0c, 0x00, 0x00, 0x20, 0x00, 0x18, 0x00, 0x60, 0x00, 0x8e, 0x11, 0x00, 0x00, 0xa2, 0x08, 0x8d, 0x01, 0x00, 0x00, 0x28, 0x00, 0x20, 0x00, 0x60, 0x00, 0x8e, 0x11, 0x00, 0x00, 0x98, 0x23, 0xa2, 0x08, 0x00, 0x00, 0x10, 0x00, 0x28, 0x00, 0x60, 0x00, 0x8e, 0x11, 0x00, 0x00, 0x88, 0x23, 0x98, 0x23, 0x30, 0x00, 0x38, 0x00, 0x10, 0x00, 0xa5, 0x00, 0x80, 0x15, 0x00, 0x00, 0x8e, 0x2a, 0x88, 0x23, 0x30, 0x00, 0x10, 0x00, 0x08, 0x00, 0x77, 0x00, 0x80, 0x15, 0x00, 0x00, 0x88, 0x23, 0x80, 0x0c, 0x40, 0x00, 0x30, 0x00, 0x08, 0x00, 0xa3, 0x00, 0x82, 0x00, 0x00, 0x00, 0x80, 0x15, 0x80, 0x0c, 0x40, 0x00, 0x08, 0x00, 0x18, 0x00, 0x60, 0x00, 0x82, 0x00, 0x00, 0x00, 0x80, 0x0c, 0x8d, 0x01, 0x48, 0x00, 0x40, 0x00, 0x18, 0x00, 0x60, 0x00, 0x98, 0x04, 0x00, 0x00, 0x82, 0x00, 0x8d, 0x01, 0x48, 0x00, 0x18, 0x00, 0x20, 0x00, 0x60, 0x00, 0x98, 0x04, 0x00, 0x00, 0x8d, 0x01, 0xa2, 0x08, 0x50, 0x00, 0x48, 0x00, 0x20, 0x00, 0x84, 0x00, 0xa2, 0x16, 0x00, 0x00, 0x98, 0x04, 0xa2, 0x08, 0x50, 0x00, 0x20, 0x00, 0x28, 0x00, 0x60, 0x00, 0xa2, 0x16, 0x00, 0x00, 0xa2, 0x08, 0x98, 0x23, 0x38, 0x00, 0x50, 0x00, 0x28, 0x00, 0x60, 0x00, 0x8e, 0x2a, 0x00, 0x00, 0xa2, 0x16, 0x98, 0x23, 0x38, 0x00, 0x28, 0x00, 0x10, 0x00, 0x60, 0x00, 0x8e, 0x2a, 0x00, 0x00, 0x98, 0x23, 0x88, 0x23, 0x58, 0x00, 0x38, 0x00, 0x30, 0x00, 0xdb, 0x00, 0x8e, 0x11, 0x00, 0x00, 0x8e, 0x2a, 0x80, 0x15, 0x58, 0x00, 0x30, 0x00, 0x40, 0x00, 0xd4, 0x00, 0x8e, 0x11, 0x00, 0x00, 0x80, 0x15, 0x82, 0x00, 0x58, 0x00, 0x40, 0x00, 0x48, 0x00, 0xc1, 0x00, 0x8e, 0x11, 0x00, 0x00, 0x82, 0x00, 0x98, 0x04, 0x58, 0x00, 0x48, 0x00, 0x50, 0x00, 0xb2, 0x00, 0x8e, 0x11, 0x00, 0x00, 0x98, 0x04, 0xa2, 0x16, 0x58, 0x00, 0x50, 0x00, 0x38, 0x00, 0xbc, 0x00, 0x8e, 0x11, 0x00, 0x00, 0xa2, 0x16, 0x8e, 0x2a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0082f200
unsigned char g_BattleSummonTyphonModelMesh1[440] = {0x60, 0x00, 0x00, 0x00, 0x1a, 0x00, 0xf0, 0x0c, 0x05, 0x00, 0x00, 0x00, 0xb2, 0xf7, 0x78, 0x07, 0xc0, 0x02, 0x00, 0x00, 0x81, 0xf9, 0x2e, 0x03, 0xc2, 0xf6, 0x00, 0x00, 0x1a, 0x00, 0x2c, 0x04, 0xdd, 0x08, 0x00, 0x00, 0x84, 0x08, 0xb9, 0x03, 0xc0, 0x02, 0x00, 0x00, 0x4d, 0x05, 0xb9, 0x03, 0xde, 0xf8, 0x00, 0x00, 0x4a, 0xf6, 0x56, 0xfa, 0x2e, 0xfb, 0x00, 0x00, 0x4f, 0xff, 0xa1, 0xf3, 0x28, 0xf9, 0x00, 0x00, 0xe8, 0xfa, 0xe2, 0xfa, 0x2d, 0x07, 0x00, 0x00, 0x4d, 0x05, 0x9e, 0xf1, 0x2d, 0x07, 0x00, 0x00, 0x2e, 0x06, 0x1e, 0xf3, 0x5b, 0xfe, 0x00, 0x00, 0x1a, 0x00, 0x4a, 0xec, 0x05, 0x00, 0x00, 0x00, 0x14, 0x00, 0x2c, 0x01, 0x00, 0x00, 0x08, 0x00, 0x10, 0x00, 0x60, 0x00, 0x94, 0x13, 0x00, 0x00, 0x82, 0x0d, 0x86, 0x28, 0x00, 0x00, 0x18, 0x00, 0x08, 0x00, 0x60, 0x00, 0x94, 0x13, 0x00, 0x00, 0x94, 0x00, 0x82, 0x0d, 0x00, 0x00, 0x20, 0x00, 0x18, 0x00, 0x60, 0x00, 0x94, 0x13, 0x00, 0x00, 0xa6, 0x0d, 0x94, 0x00, 0x00, 0x00, 0x28, 0x00, 0x20, 0x00, 0x60, 0x00, 0x94, 0x13, 0x00, 0x00, 0x9f, 0x22, 0xa6, 0x0d, 0x00, 0x00, 0x10, 0x00, 0x28, 0x00, 0x60, 0x00, 0x94, 0x13, 0x00, 0x00, 0x86, 0x28, 0x9f, 0x22, 0x30, 0x00, 0x38, 0x00, 0x10, 0x00, 0x60, 0x00, 0x80, 0x1d, 0x00, 0x00, 0x92, 0x22, 0x86, 0x28, 0x30, 0x00, 0x10, 0x00, 0x08, 0x00, 0x60, 0x00, 0x80, 0x1d, 0x00, 0x00, 0x86, 0x28, 0x82, 0x0d, 0x40, 0x00, 0x30, 0x00, 0x08, 0x00, 0x60, 0x00, 0x89, 0x03, 0x00, 0x00, 0x80, 0x1d, 0x82, 0x0d, 0x40, 0x00, 0x08, 0x00, 0x18, 0x00, 0x60, 0x00, 0x89, 0x03, 0x00, 0x00, 0x82, 0x0d, 0x94, 0x00, 0x48, 0x00, 0x40, 0x00, 0x18, 0x00, 0x90, 0x00, 0x9f, 0x03, 0x00, 0x00, 0x89, 0x03, 0x94, 0x00, 0x48, 0x00, 0x18, 0x00, 0x20, 0x00, 0x9c, 0x00, 0x9f, 0x03, 0x00, 0x00, 0x94, 0x00, 0xa6, 0x0d, 0x50, 0x00, 0x48, 0x00, 0x20, 0x00, 0xae, 0x00, 0xa1, 0x16, 0x00, 0x00, 0x9f, 0x03, 0xa6, 0x0d, 0x50, 0x00, 0x20, 0x00, 0x28, 0x00, 0x91, 0x00, 0xa1, 0x16, 0x00, 0x00, 0xa6, 0x0d, 0x9f, 0x22, 0x38, 0x00, 0x50, 0x00, 0x28, 0x00, 0x78, 0x00, 0x92, 0x22, 0x00, 0x00, 0xa1, 0x16, 0x9f, 0x22, 0x38, 0x00, 0x28, 0x00, 0x10, 0x00, 0x60, 0x00, 0x92, 0x22, 0x00, 0x00, 0x9f, 0x22, 0x86, 0x28, 0x58, 0x00, 0x38, 0x00, 0x30, 0x00, 0x62, 0x00, 0x94, 0x13, 0x00, 0x00, 0x92, 0x22, 0x80, 0x1d, 0x58, 0x00, 0x30, 0x00, 0x40, 0x00, 0x6c, 0x00, 0x94, 0x13, 0x00, 0x00, 0x80, 0x1d, 0x89, 0x03, 0x58, 0x00, 0x40, 0x00, 0x48, 0x00, 0x99, 0x00, 0x94, 0x13, 0x00, 0x00, 0x89, 0x03, 0x9f, 0x03, 0x58, 0x00, 0x48, 0x00, 0x50, 0x00, 0xd0, 0x00, 0x94, 0x13, 0x00, 0x00, 0x9f, 0x03, 0xa1, 0x16, 0x58, 0x00, 0x50, 0x00, 0x38, 0x00, 0x9b, 0x00, 0x94, 0x13, 0x00, 0x00, 0xa1, 0x16, 0x92, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0082f3b8
unsigned char g_BattleSummonTyphonModelMesh2[440] = {0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x99, 0x04, 0x02, 0x00, 0x00, 0x00, 0xa3, 0xfa, 0xa8, 0x02, 0x9b, 0x01, 0x00, 0x00, 0x45, 0xfd, 0x3c, 0x01, 0xab, 0xfa, 0x00, 0x00, 0x08, 0x00, 0xc3, 0x00, 0xb3, 0x06, 0x00, 0x00, 0x1c, 0x05, 0xb9, 0x02, 0x67, 0x00, 0x00, 0x00, 0x69, 0x02, 0xd9, 0x00, 0xdc, 0xfa, 0x00, 0x00, 0x27, 0xfa, 0xb3, 0xfb, 0x9c, 0xfc, 0x00, 0x00, 0xe0, 0xff, 0xf8, 0xfb, 0x38, 0xf9, 0x00, 0x00, 0x83, 0xfc, 0xb0, 0xfa, 0xae, 0x06, 0x00, 0x00, 0x49, 0x05, 0xfb, 0xfb, 0x63, 0x05, 0x00, 0x00, 0x61, 0x06, 0x05, 0xfd, 0x83, 0xfb, 0x00, 0x00, 0xd9, 0xff, 0xa0, 0xf6, 0x5e, 0xff, 0x00, 0x00, 0x14, 0x00, 0x2c, 0x01, 0x00, 0x00, 0x08, 0x00, 0x10, 0x00, 0x60, 0x00, 0x8a, 0x0d, 0x00, 0x00, 0x81, 0x09, 0x86, 0x17, 0x00, 0x00, 0x18, 0x00, 0x08, 0x00, 0x60, 0x00, 0x8a, 0x0d, 0x00, 0x00, 0x8b, 0x00, 0x81, 0x09, 0x00, 0x00, 0x20, 0x00, 0x18, 0x00, 0x60, 0x00, 0x8a, 0x0d, 0x00, 0x00, 0x94, 0x0c, 0x8b, 0x00, 0x00, 0x00, 0x28, 0x00, 0x20, 0x00, 0x60, 0x00, 0x8a, 0x0d, 0x00, 0x00, 0x8f, 0x16, 0x94, 0x0c, 0x00, 0x00, 0x10, 0x00, 0x28, 0x00, 0x60, 0x00, 0x8a, 0x0d, 0x00, 0x00, 0x86, 0x17, 0x8f, 0x16, 0x30, 0x00, 0x38, 0x00, 0x10, 0x00, 0x60, 0x00, 0x80, 0x13, 0x00, 0x00, 0x8a, 0x1a, 0x86, 0x17, 0x30, 0x00, 0x10, 0x00, 0x08, 0x00, 0x60, 0x00, 0x80, 0x13, 0x00, 0x00, 0x86, 0x17, 0x81, 0x09, 0x40, 0x00, 0x30, 0x00, 0x08, 0x00, 0x60, 0x00, 0x85, 0x00, 0x00, 0x00, 0x80, 0x13, 0x81, 0x09, 0x40, 0x00, 0x08, 0x00, 0x18, 0x00, 0x60, 0x00, 0x85, 0x00, 0x00, 0x00, 0x81, 0x09, 0x8b, 0x00, 0x48, 0x00, 0x40, 0x00, 0x18, 0x00, 0x60, 0x00, 0x94, 0x02, 0x00, 0x00, 0x85, 0x00, 0x8b, 0x00, 0x48, 0x00, 0x18, 0x00, 0x20, 0x00, 0x60, 0x00, 0x94, 0x02, 0x00, 0x00, 0x8b, 0x00, 0x94, 0x0c, 0x50, 0x00, 0x48, 0x00, 0x20, 0x00, 0x9f, 0x00, 0x96, 0x15, 0x00, 0x00, 0x94, 0x02, 0x94, 0x0c, 0x50, 0x00, 0x20, 0x00, 0x28, 0x00, 0x66, 0x00, 0x96, 0x15, 0x00, 0x00, 0x94, 0x0c, 0x8f, 0x16, 0x38, 0x00, 0x50, 0x00, 0x28, 0x00, 0x7f, 0x00, 0x8a, 0x1a, 0x00, 0x00, 0x96, 0x15, 0x8f, 0x16, 0x38, 0x00, 0x28, 0x00, 0x10, 0x00, 0x60, 0x00, 0x8a, 0x1a, 0x00, 0x00, 0x8f, 0x16, 0x86, 0x17, 0x58, 0x00, 0x38, 0x00, 0x30, 0x00, 0x9a, 0x00, 0x8a, 0x0e, 0x00, 0x00, 0x8a, 0x1a, 0x80, 0x13, 0x58, 0x00, 0x30, 0x00, 0x40, 0x00, 0x6e, 0x00, 0x8a, 0x0e, 0x00, 0x00, 0x80, 0x13, 0x85, 0x00, 0x58, 0x00, 0x40, 0x00, 0x48, 0x00, 0xb2, 0x00, 0x8a, 0x0e, 0x00, 0x00, 0x85, 0x00, 0x94, 0x02, 0x58, 0x00, 0x48, 0x00, 0x50, 0x00, 0xdc, 0x00, 0x8a, 0x0e, 0x00, 0x00, 0x94, 0x02, 0x96, 0x15, 0x58, 0x00, 0x50, 0x00, 0x38, 0x00, 0xce, 0x00, 0x8a, 0x0e, 0x00, 0x00, 0x96, 0x15, 0x8a, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0082f570
unsigned int g_BattleSummonTyphonModelResourceDefs[125] = {
    0x00c800b1, (unsigned int)&g_BattleSummonTyphonResEyeSkyRsd, 0, 0, 0, 0x00c840b6, (unsigned int)&g_BattleSummonTyphonResEyeSkyRsd[12], 0, 0, 0, 0x00c840b1, (unsigned int)&g_BattleSummonTyphonResWindRsd, 0, 0, 0, 0x00c840b1,
    (unsigned int)&g_BattleSummonTyphonResWindRsd[12], 0, 0, 0, 0x00c8aa39, (unsigned int)&g_BattleSummonTyphonResSmoke1TimSpt, (unsigned int)&g_BattleSummonTyphonResSmoke1TimSpt[12], 0, 0, 0x00c8aa39, (unsigned int)&g_BattleSummonTyphonResSmoke2TimSpt, (unsigned int)&g_BattleSummonTyphonResSmoke2TimSpt[12], 0, 0, 0x00c8aa39, (unsigned int)&g_BattleSummonTyphonResSmoke3TimSpt,
    (unsigned int)&g_BattleSummonTyphonResSmoke3TimSpt[12], 0, 0, 0x00c8aa39, (unsigned int)&g_BattleSummonTyphonResSmoke4TimSpt, (unsigned int)&g_BattleSummonTyphonResSmoke4TimSpt[12], 0, 0, 0x00c8aa39, (unsigned int)&g_BattleSummonTyphonResSmoke5TimSpt, (unsigned int)&g_BattleSummonTyphonResSmoke5TimSpt[12], 0, 0, 0x00c8aa39, (unsigned int)&g_BattleSummonTyphonResSmoke6TimSpt, (unsigned int)&g_BattleSummonTyphonResSmoke6TimSpt[12],
    0, 0, 0x00c8aa39, (unsigned int)&g_BattleSummonTyphonResSmoke7TimSpt, (unsigned int)&g_BattleSummonTyphonResSmoke7TimSpt[12], 0, 0, 0x00c8aa39, (unsigned int)&g_BattleSummonTyphonResSmoke8TimSpt, (unsigned int)&g_BattleSummonTyphonResSmoke8TimSpt[12], 0, 0, 0x00c8aa39, (unsigned int)&g_BattleSummonTyphonResSmoke9TimSpt, (unsigned int)&g_BattleSummonTyphonResSmoke9TimSpt[12], 0,
    0, 0x00c8aa39, (unsigned int)&g_BattleSummonTyphonResSmoke10Tim, (unsigned int)&g_BattleSummonTyphonResSmoke10Spt, 0, 0, 0x00c8aa39, (unsigned int)&g_BattleSummonTyphonResSmoke11Tim, (unsigned int)&g_BattleSummonTyphonResSmoke11Spt, 0, 0, 0x00c8aa39, (unsigned int)&g_BattleSummonTyphonResSmoke12Tim, (unsigned int)&g_BattleSummonTyphonResSmoke12Spt, 0, 0,
    0x00c8aa39, (unsigned int)&g_BattleSummonTyphonResSmoke13Tim, (unsigned int)&g_BattleSummonTyphonResSmoke13Spt, 0, 0, 0x00c8aa39, (unsigned int)&g_BattleSummonTyphonResSmoke14Tim, (unsigned int)&g_BattleSummonTyphonResSmoke14Spt, 0, 0, 0x00c8aa39, (unsigned int)&g_BattleSummonTyphonResSmoke15Tim, (unsigned int)&g_BattleSummonTyphonResSmoke15Spt, 0, 0, 0x00c8aa39,
    (unsigned int)&g_BattleSummonTyphonResSmoke16Tim, (unsigned int)&g_BattleSummonTyphonResSmoke16Spt, 0, 0, 0x00c8aa39, (unsigned int)&g_BattleSummonTyphonResSmokeATimSpt, (unsigned int)&g_BattleSummonTyphonResSmokeATimSpt[12], 0, 0, 0x0019783c, (unsigned int)&g_BattleSummonTyphonResTupon1rTim, 0, 0, 0, 0x00197839, (unsigned int)&g_BattleSummonTyphonResTupon1rTim[12],
    0, 0, 0, 0x0019783c, (unsigned int)&g_BattleSummonTyphonResTupon2rTim, 0, 0, 0, 0x00197839, (unsigned int)&g_BattleSummonTyphonResTupon2rTim[12], 0, 0, 0
};
// GLOBAL: FF7 0x0082f764
int g_BattleSummonTyphonEndMarker = (-1);
// GLOBAL: FF7 0x0082f768
int g_BattleSummonTyphonPairMarker[2] = {-1, 0};
// GLOBAL: FF7 0x0082f770
int g_BattleSummonTyphonCameraScriptHeader = 129;
unsigned char g_rawData_0082f778[320] = {
0x41, 0x00, 0x88, 0x13, 0x60, 0xf0, 0x88, 0x13, 0x80, 0x00, 0x04, 0x00, 0x40, 0x00, 0x04, 0x00,
0x82, 0x00, 0x46, 0x00, 0x00, 0x00, 0x30, 0xf8, 0xc4, 0x09, 0x43, 0x00, 0x46, 0x00, 0xb8, 0x0b,
0x68, 0xc5, 0x98, 0x3a, 0x81, 0x00, 0x00, 0x00, 0x60, 0xf0, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00,
0x60, 0xf0, 0x58, 0x1b, 0x80, 0x00, 0x0f, 0x00, 0x40, 0x00, 0x0f, 0x00, 0x80, 0x00, 0x1e, 0x00,
0x40, 0x00, 0x05, 0x00, 0x45, 0x00, 0x19, 0x00, 0x04, 0x00, 0x00, 0x00, 0x60, 0xf0, 0x68, 0x42,
0x81, 0x00, 0x00, 0x00, 0x60, 0xf0, 0xd0, 0x07, 0x41, 0x00, 0x68, 0xc5, 0x48, 0xf4, 0x10, 0x27,
0x82, 0x00, 0x23, 0x00, 0x00, 0x00, 0x48, 0xf4, 0xd0, 0x07, 0x42, 0x00, 0x23, 0x00, 0x68, 0xc5,
0x0c, 0xfe, 0x10, 0x27, 0x82, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x0c, 0xfe, 0x58, 0x1b, 0x40, 0x00,
0x0f, 0x00, 0x82, 0x00, 0x32, 0x00, 0x00, 0x00, 0x48, 0xf4, 0x58, 0x1b, 0x43, 0x00, 0x32, 0x00,
0xf0, 0xd8, 0x78, 0xec, 0xa8, 0x61, 0x81, 0x00, 0x00, 0x00, 0x60, 0xf0, 0x00, 0x00, 0x41, 0x00,
0xc0, 0xe0, 0x78, 0xec, 0x40, 0x1f, 0x82, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x48, 0xf4, 0xdc, 0x05,
0x42, 0x00, 0x1e, 0x00, 0xcc, 0xde, 0x90, 0xe8, 0x10, 0x27, 0x80, 0x00, 0x23, 0x00, 0x40, 0x00,
0x23, 0x00, 0x81, 0x00, 0x00, 0x00, 0xf0, 0xd8, 0x58, 0x1b, 0x41, 0x00, 0xd0, 0x8a, 0xf0, 0xd8,
0x58, 0x1b, 0x80, 0x00, 0x0a, 0x00, 0x40, 0x00, 0x0a, 0x00, 0x80, 0x00, 0x0f, 0x00, 0x40, 0x00,
0x0f, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0x1b, 0x41, 0x00, 0xe0, 0xb1, 0x78, 0xec,
0x58, 0x1b, 0x80, 0x00, 0x14, 0x00, 0x40, 0x00, 0x14, 0x00, 0x82, 0x00, 0x19, 0x00, 0x00, 0x00,
0xe0, 0xb1, 0x58, 0x1b, 0x42, 0x00, 0x19, 0x00, 0xf0, 0xd8, 0x68, 0xc5, 0x58, 0x1b, 0x82, 0x00,
0x32, 0x00, 0x48, 0xf4, 0xe0, 0xb1, 0x58, 0x1b, 0x42, 0x00, 0x32, 0x00, 0xf0, 0xd8, 0x00, 0x00,
0xd0, 0x07, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0x1b, 0x41, 0x00, 0xf0, 0xd8, 0x78, 0xec,
0xd0, 0x07, 0x80, 0x00, 0xff, 0xff, 0x40, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0082f8b8
struct BattleFxTyphonFadeConfig g_BattleSummonTyphonSpriteConfig = {{0, 0, 0, 3}, 0, 0, 0, {0x62}, 0, 0, 640, 332};
// GLOBAL: FF7 0x0082f8c8
struct BattleFxTyphonColorEntry g_BattleSummonTyphonColorPalette[6] = {{0x90, 0x90, 0, 0}, {0xb0, 0xb0, 0xb0, 0}, {0x40, 0x90, 0x40, 0}, {0x40, 0x40, 0x40, 0}, {0, 0x90, 0x90, 0}, {0xa0, 0, 0xa0, 0}};
// GLOBAL: FF7 0x0082f8e0
struct BattleSummonTyphonRingConfig0 g_BattleSummonTyphonRingConfig0 = {0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x0082f8f0
struct BattleSummonTyphonRingConfig0 g_BattleSummonTyphonRingConfig1 = {0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x0082f900
struct BattleSummonTyphonRingConfig2 g_BattleSummonTyphonRingConfig2 = {0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x0082f910
int g_BattleSummonTyphonRingVertexOffsets[16] = {4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x10, 0x11, 0x12, 0x13};
// GLOBAL: FF7 0x0082f950
struct BattleSummonTyphonParticleParams0 g_BattleSummonTyphonParticleParams0 = {0, 0, {0x80, 0x80, 0x80, 0x2e, 0, 0, 0, 0}};
// GLOBAL: FF7 0x0082f960
BattleFxTyphonInitialAngles g_BattleSummonTyphonInitialAngles = {{-0xce40000, 2000}};
/* A PSX matrix (rotation and translation) padded to 32 bytes. */
typedef struct {
    PsxMatrix matrix;
    short pad;
} BattleSummonTyphonMatrix;

/* The stage background's warp matrix, which g_BattleSummonTyphonMatrixData1 scales and moves. */
// GLOBAL: FF7 0x0082f968
BattleSummonTyphonMatrix g_BattleSummonTyphonMatrixData0 = {{{{-512, 0, 0}, {0, 0, 512}, {0, 512, 0}}, {0, -875, 0x2000}}, 0};
/* The scale and depth BattleFx_SummonTyphonPrepareStageOffscreenRequests sets from the basis. */
// GLOBAL: FF7 0x0082f988
BattleSummonTyphonMatrix g_BattleSummonTyphonMatrixData1 = {{{{0x1000, 0, 0}, {0, 0x1000, 0}, {0, 0, 0x1000}}, {0, 0, 0}}, 0};
// The offscreen request the stage warp fills and queues.
// GLOBAL: FF7 0x0082f9a8
struct BattleFxTyphonBlendConfig g_BattleSummonTyphonBlendConfig = {896, 0, 256, 256};
// GLOBAL: FF7 0x0082f9b0
struct BattleSummonTyphonParticleParams1 g_BattleSummonTyphonParticleParams1 = {0, {0x80, 0, 0, 0}, 0, {0, 0, 0x20, 0, 0, 0}};
// GLOBAL: FF7 0x0082f9c0
unsigned int g_BattleSummonTyphonModelPointers[3] = {(unsigned int)&g_BattleSummonTyphonModelMesh0, (unsigned int)&g_BattleSummonTyphonModelMesh1, (unsigned int)&g_BattleSummonTyphonModelMesh2};
// The rotating model's draw request.
// GLOBAL: FF7 0x0082f9d0
BattleFxModelDrawHead g_BattleSummonTyphonOffsetConfig = {0, 0xA0, 0, 0, 0x20, 0};
/* The rotating model's matrix, turned about Y each frame. */
// GLOBAL: FF7 0x0082f9e0
BattleSummonTyphonMatrix g_BattleSummonTyphonMatrixData2 = {{{{0x4000, 0, 0}, {0, 0x4000, 0}, {0, 0, 0x4000}}, {0, -20000, 7000}}, 0};
// GLOBAL: FF7 0x0082fa00
struct BattleSummonTyphonParticleParams2 g_BattleSummonTyphonParticleParams2 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
// GLOBAL: FF7 0x0082fa10
struct BattleSummonTyphonParticleParams3 g_BattleSummonTyphonParticleParams3 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0, 0, 0, 0, 0x48, 0xf4, 0, 0, 0, 0}};
/* The white fade's screen-sized PSX tile (tag, colour, position and size), then the colour the
 * fade shades it from. */
typedef struct {
    int tag;
    CVECTOR color;
    short x;
    short y;
    short width;
    short height;
    CVECTOR baseColor;
    int pad;
} BattleSummonTyphonFadeTile;

// GLOBAL: FF7 0x0082fa28
BattleSummonTyphonFadeTile g_BattleSummonTyphonFadeTile = {0x03000000, {0, 0, 0, 0x62}, 0, 0, 640, 332, {0, 0, 0, 0x62}, 0};
// GLOBAL: FF7 0x0082fa40
Int64Pair g_BattleSummonTyphonPairedModelPoints[2] = {{(-288293264), 400}, {(-288358000), 400}};
// GLOBAL: FF7 0x0082fa50
unsigned char g_BattleSummonTyphonResEyeSkyRsd[22] =
    "eye_1.rsd\0\0\0"
    "sky_1.rsd";
// GLOBAL: FF7 0x0082fa68
unsigned char g_BattleSummonTyphonResWindRsd[23] =
    "wind_1.rsd\0\0"
    "wind_2.rsd";
// GLOBAL: FF7 0x0082fa80
unsigned char g_BattleSummonTyphonResSmoke1TimSpt[24] =
    "smoke_1.tim\0"
    "smoke_1.spt";
// GLOBAL: FF7 0x0082fa98
unsigned char g_BattleSummonTyphonResSmoke2TimSpt[24] =
    "smoke_2.tim\0"
    "smoke_2.spt";
// GLOBAL: FF7 0x0082fab0
unsigned char g_BattleSummonTyphonResSmoke3TimSpt[24] =
    "smoke_3.tim\0"
    "smoke_3.spt";
// GLOBAL: FF7 0x0082fac8
unsigned char g_BattleSummonTyphonResSmoke4TimSpt[24] =
    "smoke_4.tim\0"
    "smoke_4.spt";
// GLOBAL: FF7 0x0082fae0
unsigned char g_BattleSummonTyphonResSmoke5TimSpt[24] =
    "smoke_5.tim\0"
    "smoke_5.spt";
// GLOBAL: FF7 0x0082faf8
unsigned char g_BattleSummonTyphonResSmoke6TimSpt[24] =
    "smoke_6.tim\0"
    "smoke_6.spt";
// GLOBAL: FF7 0x0082fb10
unsigned char g_BattleSummonTyphonResSmoke7TimSpt[24] =
    "smoke_7.tim\0"
    "smoke_7.spt";
// GLOBAL: FF7 0x0082fb28
unsigned char g_BattleSummonTyphonResSmoke8TimSpt[24] =
    "smoke_8.tim\0"
    "smoke_8.spt";
// GLOBAL: FF7 0x0082fb40
unsigned char g_BattleSummonTyphonResSmoke9TimSpt[24] =
    "smoke_9.tim\0"
    "smoke_9.spt";
// GLOBAL: FF7 0x0082fb58
unsigned char g_BattleSummonTyphonResSmoke10Tim[13] = "smoke_10.tim";
// GLOBAL: FF7 0x0082fb68
unsigned char g_BattleSummonTyphonResSmoke10Spt[13] = "smoke_10.spt";
// GLOBAL: FF7 0x0082fb78
unsigned char g_BattleSummonTyphonResSmoke11Tim[13] = "smoke_11.tim";
// GLOBAL: FF7 0x0082fb88
unsigned char g_BattleSummonTyphonResSmoke11Spt[13] = "smoke_11.spt";
// GLOBAL: FF7 0x0082fb98
unsigned char g_BattleSummonTyphonResSmoke12Tim[13] = "smoke_12.tim";
// GLOBAL: FF7 0x0082fba8
unsigned char g_BattleSummonTyphonResSmoke12Spt[13] = "smoke_12.spt";
// GLOBAL: FF7 0x0082fbb8
unsigned char g_BattleSummonTyphonResSmoke13Tim[13] = "smoke_13.tim";
// GLOBAL: FF7 0x0082fbc8
unsigned char g_BattleSummonTyphonResSmoke13Spt[13] = "smoke_13.spt";
// GLOBAL: FF7 0x0082fbd8
unsigned char g_BattleSummonTyphonResSmoke14Tim[13] = "smoke_14.tim";
// GLOBAL: FF7 0x0082fbe8
unsigned char g_BattleSummonTyphonResSmoke14Spt[13] = "smoke_14.spt";
// GLOBAL: FF7 0x0082fbf8
unsigned char g_BattleSummonTyphonResSmoke15Tim[13] = "smoke_15.tim";
// GLOBAL: FF7 0x0082fc08
unsigned char g_BattleSummonTyphonResSmoke15Spt[13] = "smoke_15.spt";
// GLOBAL: FF7 0x0082fc18
unsigned char g_BattleSummonTyphonResSmoke16Tim[13] = "smoke_16.tim";
// GLOBAL: FF7 0x0082fc28
unsigned char g_BattleSummonTyphonResSmoke16Spt[13] = "smoke_16.spt";
// GLOBAL: FF7 0x0082fc38
unsigned char g_BattleSummonTyphonResSmokeATimSpt[24] =
    "smoke_a.tim\0"
    "smoke_a.spt";
// GLOBAL: FF7 0x0082fc50
unsigned char g_BattleSummonTyphonResTupon1rTim[24] =
    "Tupon1r.TIM\0"
    "Tupon1r.TIM";
// GLOBAL: FF7 0x0082fc68
unsigned char g_BattleSummonTyphonResTupon2rTim[24] =
    "Tupon2r.TIM\0"
    "Tupon2r.TIM";
unsigned char g_bssPad_00b3ac20[4] = {0};
// GLOBAL: FF7 0x00b3ac24
BattleFxTyphonTumbleState *g_BattleSummonTyphonTumbleState = 0;
// The callback of each of the 300 particle records in g_BattleSummonTyphonRingBuffer; 0 marks a
// free record.
// GLOBAL: FF7 0x00b3ac28
void (__cdecl *g_BattleSummonTyphonScratchBuffer[300])(int) = {0};
/* The GTE scratch block (Gfx_GetPsxScratchpadPtr(0)) the ring update uses: a vector (a ring's tilt angles,
 * then its offset) and the rotation matrix built from the angles. */
typedef struct {
    SVECTOR v;
    unsigned char pad08[0x10];
    PsxMatrix matrix;
} BattleSummonTyphonRingScratch;

// GLOBAL: FF7 0x00b3b0d8
BattleSummonTyphonRingScratch *g_BattleSummonTyphonRingScratch = 0;
// GLOBAL: FF7 0x00b3b0dc
unsigned char g_BattleSummonTyphonRingTaskInitFlag = 0;
// GLOBAL: FF7 0x00b3b0e0
PsxMatrix g_BattleSummonTyphonViewMatrix = {0};
// GLOBAL: FF7 0x00b3b100
struct BattleSummonTyphonBoundingCenter g_BattleSummonTyphonBoundingCenter = {0};
// GLOBAL: FF7 0x00b3b108
short g_BattleSummonTyphonTargetCount = 0;
unsigned char g_bssPad_00b3b10c[4] = {0};
unsigned char g_bssPad_00b3b110[12] = {0};
// GLOBAL: FF7 0x00b3b11c
int g_BattleSummonTyphonRingTaskScratch = 0;
// GLOBAL: FF7 0x00b3b120
int g_BattleSummonTyphonTimelineCounter = 0;
/* The GTE scratch block (Gfx_GetPsxScratchpadPtr(0)) of the camera rotation: rotation angles, the vector a
 * rotation writes (read whole, or by its low halves), and the matrix built from the angles. */
typedef struct {
    SVECTOR angles;
    union {
        int offsetX;
        short posX;
    };
    union {
        int offsetY;
        short posY;
    };
    union {
        int offsetZ;
        short posZ;
    };
    int pad14;
    PsxMatrix matrix;
} BattleSummonTyphonRotationScratch;

// GLOBAL: FF7 0x00b3b124
BattleSummonTyphonRotationScratch *g_BattleSummonTyphonRotationState = 0;
// GLOBAL: FF7 0x00b3b128
BattleFxTyphonDrawHandle *g_BattleSummonTyphonDrawHandle = 0;
// GLOBAL: FF7 0x00b3b12c
int g_BattleSummonTyphonQuadScratch = 0;
// GLOBAL: FF7 0x00b3b130
int g_BattleSummonTyphonOrbitSpawnScratch = 0;
unsigned char g_bssPad_00b3b134[4] = {0};
// GLOBAL: FF7 0x00b3b138
struct BattleFxTyphonOrigin g_BattleSummonTyphonOriginCoords = {0};
// GLOBAL: FF7 0x00b3b148
struct BattleFxTyphonModelVariant *g_BattleSummonTyphonModelVariant = 0;
// GLOBAL: FF7 0x00b3b14c
unsigned char g_BattleSummonTyphonColorCycleIndex = 0;
// GLOBAL: FF7 0x00b3b150
struct BattleSummonTyphonFallParam0 g_BattleSummonTyphonCameraTrackId = {0};
unsigned char g_bssPad_00b3b154[4] = {0};
unsigned char g_bssPad_00b3b158[4] = {0};
// GLOBAL: FF7 0x00b3b15c
BattleFxFallMotionParams g_BattleSummonTyphonTargetBone = {0};
// GLOBAL: FF7 0x00b3b160
int g_BattleSummonTyphonRotationSpeed = 0;
// GLOBAL: FF7 0x00b3b164
int g_BattleSummonTyphonRingVertexCount = 0;
unsigned char g_bssPad_00b3b168[20] = {0};
// GLOBAL: FF7 0x00b3b17c
unsigned char g_BattleSummonTyphonRingPhase = 0;
// GLOBAL: FF7 0x00b3b180
unsigned char g_BattleSummonTyphonRingFlag = 0;
// GLOBAL: FF7 0x00b3b184
int g_BattleSummonTyphonTargetMaskB = 0;
// GLOBAL: FF7 0x00b3b188
unsigned char g_BattleSummonTyphonFlashFlag = 0;
// GLOBAL: FF7 0x00b3b18c
BattleFxTyphonBouncingState *g_BattleSummonTyphonBouncingState = 0;
/* Actor 3's position when the camera rotation starts: x and y, then z and a pad word, each pair
 * copied as one word. */
typedef union {
    int word;
    struct {
        short x;
        short y;
    };
} BattleSummonTyphonSavedXY;

typedef union {
    int word;
    struct {
        short z;
        short pad;
    };
} BattleSummonTyphonSavedZ;

// GLOBAL: FF7 0x00b3b190
BattleSummonTyphonSavedXY g_BattleSummonTyphonSavedPosXY = {0};
// GLOBAL: FF7 0x00b3b194
BattleSummonTyphonSavedZ g_BattleSummonTyphonSavedPosZ = {0};
// GLOBAL: FF7 0x00b3b198
unsigned char g_BattleSummonTyphonEmitterFlag = 0;
// GLOBAL: FF7 0x00b3b19c
struct Vec3sPacked *g_BattleSummonTyphonOrbitPoint = 0;
// GLOBAL: FF7 0x00b3b1a0
unsigned char g_BattleSummonTyphonOrbitState = 0;
// GLOBAL: FF7 0x00b3b1a4
BattleFxCameraOrbitState *g_pBattleSummonTyphonCameraTrack = 0;
// GLOBAL: FF7 0x00b3b1a8
BattleFxCameraOrbitState *g_BattleSummonTyphonCameraPitch = 0;
unsigned char g_bssPad_00b3b1ac[4] = {0};
// The height (posY) of each actor when the summon begins.
// GLOBAL: FF7 0x00b3b1b0
short g_BattleSummonTyphonActorTargets[12] = {0};
// GLOBAL: FF7 0x00b3b1c8
BattleFxTyphonRingQuad g_BattleSummonTyphonRingVertexArray[250] = {0};
// GLOBAL: FF7 0x00b3e0a8
unsigned char g_BattleSummonTyphonRingDrawMode = 0;
// GLOBAL: FF7 0x00b3e0ac
BattleFxTyphonRingState *g_BattleSummonTyphonRingState = 0;
// GLOBAL: FF7 0x00b3e0b0
int g_BattleSummonTyphonRingStep = 0;
unsigned char g_bssPad_00b3e0b4[4] = {0};
// GLOBAL: FF7 0x00b3e0b8
unsigned char g_BattleSummonTyphonRingPad[0x4b00] = {0};
// The 300 particle records whose callbacks are in g_BattleSummonTyphonScratchBuffer.
// GLOBAL: FF7 0x00b42bb8
struct BattleMagicFullParticle g_BattleSummonTyphonRingBuffer[300] = {0};
// GLOBAL: FF7 0x00b44c88
PsxMatrix *g_BattleSummonTyphonModelMatrixScratch = 0;
unsigned char g_bssPad_00b44c8c[4] = {0};
// GLOBAL: FF7 0x00b44c90
struct BattleSummonTyphonFallParam0 g_BattleSummonTyphonFallParam0 = {0};
unsigned char g_bssPad_00b44c94[4] = {0};
unsigned char g_bssPad_00b44c98[4] = {0};
// GLOBAL: FF7 0x00b44c9c
BattleFxFallMotionParams g_BattleSummonTyphonFallParam1 = {0};
// GLOBAL: FF7 0x00b44ca0
int g_BattleSummonTyphonFallParam2 = 0;
// GLOBAL: FF7 0x00b44ca4
int g_BattleSummonTyphonFallParam3 = 0;
unsigned char g_bssPad_00b44ca8[20] = {0};
// GLOBAL: FF7 0x00b44cbc
unsigned char g_BattleSummonTyphonFallParam4 = 0;
// GLOBAL: FF7 0x00b44cc0
short g_BattleSummonTyphonFallParam5[4] = {0};
/* The 16 rings of the windstorm are 20-byte records that start at g_BattleSummonTyphonFallParam5
 * (their alpha, phase and height), run on through g_BattleSummonTyphonFallParam6 (ring 0's angles)
 * and g_BattleSummonTyphonFallParam7 (ring 0's scale, then rings 1 to 15). The code reads ring i's
 * fields at i * 0x14 past the field of ring 0, through BattleFxTyphonRingRecord and
 * BattleFxTyphonRingScale views. */
typedef struct {
    short angleX;
    short angleY;
    short angleZ;
    short pad;
} BattleSummonTyphonRingAngles;

// GLOBAL: FF7 0x00b44cc8
BattleSummonTyphonRingAngles g_BattleSummonTyphonFallParam6 = {0};
// GLOBAL: FF7 0x00b44cd0
unsigned char g_BattleSummonTyphonFallParam7[0x130] = {0};
// GLOBAL: FF7 0x00b44e00
unsigned char g_BattleSummonTyphonFallParam8 = 0;
unsigned char g_bssPad_00b44e04[4] = {0};
// GLOBAL: FF7 0x00b44e08
struct GfxTableCursor *g_BattleSummonTyphonFallParam9 = 0;
unsigned char g_bssPad_00b44e0c[4] = {0};
// GLOBAL: FF7 0x00b44e10
struct BattleSummonTyphonFallParam10 g_BattleSummonTyphonFallParam10 = {0};
/* The X and Y rotation of the summon's basis matrix; its Z rotation is
 * g_BattleSummonTyphonFallParam12, which follows (RotMatrix reads the three as one vector). */
typedef struct {
    short rotX;
    short rotY;
} BattleSummonTyphonBasisAngles;

// GLOBAL: FF7 0x00b44e30
BattleSummonTyphonBasisAngles g_BattleSummonTyphonFallParam11 = {0};
/* The Z rotation of the summon's basis matrix. */
typedef struct {
    short rotZ;
    short pad;
} BattleSummonTyphonBasisAngleZ;

// GLOBAL: FF7 0x00b44e34
BattleSummonTyphonBasisAngleZ g_BattleSummonTyphonFallParam12 = {0};
// GLOBAL: FF7 0x00b44e38
int g_BattleSummonTyphonFallParam13 = 0;
unsigned char g_bssPad_00b44e3c[4] = {0};
// GLOBAL: FF7 0x00b44e40
int g_BattleSummonTyphonFallParam14 = 0;
unsigned char g_bssPad_00b44e44[4] = {0};
// GLOBAL: FF7 0x00b44e48
unsigned char g_BattleSummonTyphonFallParam15[0x10] = {0};
// GLOBAL: FF7 0x00b44e58
struct BattleScreenFlashRamp *g_BattleSummonTyphonFadeRamp = 0;
// GLOBAL: FF7 0x00b44e5c
int g_BattleSummonTyphonLoadedResTable2 = 0;
// clang-format on

#pragma function(memcpy)

/* Loads Typhon summon models, sprites, and animation textures. */
// FUNCTION: FF7 0x004d56b0
void __cdecl BattleFx_SummonLoadResTyphon(void)
{
    char pathScratch[200];
    GfxModelTableBank *resourceBits;
    int kind;
    GfxResourceRequest requestBuffer;
    GfxResourceRequest *request;
    int defPtr;

    if (g_pBattle3dScene != 0 && g_BattleSummonTyphonFallParam9 == 0) {
        kind = 0x19;
        defPtr = (int)&g_BattleSummonTyphonModelResourceDefs;
        resourceBits = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestBuffer;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x0082fc80
        request->resourcePath = (char *)BattleFx_BuildResourcePath("SUMMON/tupon", pathScratch);
        Gfx_LoadModelTableIfAbsent(kind, defPtr, request, &g_BattleSummonTyphonFallParam9, resourceBits);
        SfxRegisterSoundGroups((struct SoundGroup **)&g_BattleSummonTyphonResourceDef);
    }
}

/* Entry point for Typhon summon sequence; clears state and initializes effect. */
// FUNCTION: FF7 0x004d5751
#pragma intrinsic(memset)
int __cdecl BattleFx_SummonEnterTyphon(int targetMask, int actorIdx)
{
    memset((void *)&g_BattleSummonTyphonFallParam10, 0, 0x1e);
    g_BattleSummonTyphonFallParam12.rotZ = 0;
    g_BattleSummonTyphonFallParam11.rotY = g_BattleSummonTyphonFallParam12.rotZ;
    g_BattleSummonTyphonFallParam11.rotX = g_BattleSummonTyphonFallParam11.rotY;
    g_BattleSummonTyphonOriginCoords.tiltZ = 0;
    g_BattleSummonTyphonOriginCoords.tiltY = g_BattleSummonTyphonOriginCoords.tiltZ;
    g_BattleSummonTyphonOriginCoords.tiltX = g_BattleSummonTyphonOriginCoords.tiltY;
    g_BattleSummonTyphonOriginCoords.phaseZ = 0;
    g_BattleSummonTyphonOriginCoords.phaseY = g_BattleSummonTyphonOriginCoords.phaseZ;
    g_BattleSummonTyphonOriginCoords.phaseX = g_BattleSummonTyphonOriginCoords.phaseY;
    g_BattleSummonTyphonTimelineCounter = 0;
    BattleFx_SummonBeginTyphon(targetMask, actorIdx);
    return 0;
}
#pragma function(memset)

/* Computes target bounding box, installs matrices, and schedules camera tasks. */
// FUNCTION: FF7 0x004d57ef
void __cdecl BattleFx_SummonBeginTyphon(int targetMask, int actorIdx)
{
    SVECTOR center;
    int actorIndex;

    BattleFx_SummonLoadResTyphon();
    Battle_GetActorMaskCentroid(g_BattleSummonTyphonTargetMaskB = targetMask, &center.vx);
    if (center.vz < g_BattleActorRecords[actorIdx].posZ) {
        g_BattleSummonTyphonFallParam11.rotY = 0x800;
        g_BattleSummonTyphonFallParam10.position = center.vz + 0x1B58;
    } else {
        g_BattleSummonTyphonFallParam10.position = center.vz - 0x1B58;
    }
    RotMatrix((SVECTOR *)&g_BattleSummonTyphonFallParam11, (int *)&g_BattleSummonTyphonFallParam10);
    for (actorIndex = 0; actorIndex < 0xA; actorIndex++) {
        g_BattleSummonTyphonActorTargets[actorIndex] = g_BattleActorRecords[actorIndex].posY;
    }
    BattleFx_SummonTyphonClearParticleCallbacks();
    Battle_AllocateResourceSlot(BattleFx_SummonTyphonParticleCallbackTask);
    if ((g_BattleSummonTyphonTargetMaskB & ~g_BattleTimelineTargetMask) != 0) {
        g_BattleSummonTyphonTargetCount = 0xF;
    } else {
        g_BattleSummonTyphonTargetCount = 0;
    }
    BattleFx_TyphonSpawnCameraScript((int)&g_BattleSummonTyphonCameraScriptHeader, (int)&g_BattleSummonTyphonFallParam10, actorIdx);
    Battle_AllocateResourceSlot(BattleFx_SummonTyphonTimelineTask);
    g_BattleStageHideFlags |= 8;
}

/* Spawns camera motion script interpreter for Typhon summon sequence. */
// FUNCTION: FF7 0x004d590c
void __cdecl BattleFx_TyphonSpawnCameraScript(int scriptData, int basisMatrix, int actorIdx)
{
    BattleFxTyphonCameraTask *slot;

    slot = (BattleFxTyphonCameraTask *)BATTLE_NEW_TASK_SLOT(BattleFx_TyphonRunCameraScript);
    slot->scriptData = scriptData;
    slot->basisMatrix = basisMatrix;
    slot->actorIdx = actorIdx;
}

/* Interprets bytecode script driving camera eye and focus positions. */
// FUNCTION: FF7 0x004d594c
void __cdecl BattleFx_TyphonRunCameraScript(void)
{
    int res;
    int padLocal[43]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int unusedStorage[6]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int spareSlots[15]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    Vec3sPacked *offsetPtr;
    int easeIndex;
    short opcode;
    BattleFxDebrisSpawnTask *pVector;
    SVECTOR *delta;
    int easeVal;
    int pass;
    BattleFxSuperNovaCameraScriptState *state;

    state = (BattleFxSuperNovaCameraScriptState *)BATTLE_CURRENT_TASK_SLOT;
    delta = (SVECTOR *)Gfx_GetPsxScratchpadPtr(0);
    offsetPtr = (Vec3sPacked *)((int)delta + 8);
    pVector = (BattleFxDebrisSpawnTask *)((int)offsetPtr + 8);
    if (g_BattleIsPaused == 0) {
        if (state->initialized == 0) {
            g_BattleSummonTyphonFallParam0.isReady = 0;
            g_BattleSummonTyphonCameraTrackId.isReady = 0;
            (g_BattleSummonTyphonFallParam1.actorIndex) = 0xFFFF;
            (g_BattleSummonTyphonTargetBone.actorIndex) = 0xFFFF;
            state->initialized = 1;
        }
        while (g_BattleSummonTyphonFallParam0.isReady == 0 || g_BattleSummonTyphonCameraTrackId.isReady == 0) {
            opcode = *state->scriptPtr;
            state->scriptPtr++;
            if ((opcode & SUMMON_CAMERA_OP_EYE) != 0) {
                g_pBattleSummonTyphonCameraTrack = (struct BattleFxCameraOrbitState *)&g_BattleSummonTyphonFallParam0;
                g_BattleSummonTyphonCameraPitch = (struct BattleFxCameraOrbitState *)&g_BattleSummonTyphonCameraTrackId;
            } else {
                g_pBattleSummonTyphonCameraTrack = (struct BattleFxCameraOrbitState *)&g_BattleSummonTyphonCameraTrackId;
                g_BattleSummonTyphonCameraPitch = (struct BattleFxCameraOrbitState *)&g_BattleSummonTyphonFallParam0;
            }
            g_pBattleSummonTyphonCameraTrack->mode = opcode & ~0xC0;
            switch (g_pBattleSummonTyphonCameraTrack->mode) {
            case SUMMON_CAMERA_WAIT:
                g_pBattleSummonTyphonCameraTrack->stepCount = BattleFx_TyphonReadCameraScriptOperand(&state->scriptPtr);
                break;
            case SUMMON_CAMERA_SET_POSITION:
                g_pBattleSummonTyphonCameraTrack->posX = *state->scriptPtr;
                state->scriptPtr++;
                g_pBattleSummonTyphonCameraTrack->posY = *state->scriptPtr;
                state->scriptPtr++;
                g_pBattleSummonTyphonCameraTrack->posZ = *state->scriptPtr;
                state->scriptPtr++;
                break;
            case SUMMON_CAMERA_MOVE_LINEAR:
                g_pBattleSummonTyphonCameraTrack->stepCount = BattleFx_TyphonReadCameraScriptOperand(&state->scriptPtr);
                g_pBattleSummonTyphonCameraTrack->stepX = (*state->scriptPtr - g_pBattleSummonTyphonCameraTrack->posX) / g_pBattleSummonTyphonCameraTrack->stepCount;
                state->scriptPtr++;
                g_pBattleSummonTyphonCameraTrack->stepY = (*state->scriptPtr - g_pBattleSummonTyphonCameraTrack->posY) / g_pBattleSummonTyphonCameraTrack->stepCount;
                state->scriptPtr++;
                g_pBattleSummonTyphonCameraTrack->stepZ = (*state->scriptPtr - g_pBattleSummonTyphonCameraTrack->posZ) / g_pBattleSummonTyphonCameraTrack->stepCount;
                state->scriptPtr++;
                break;
            case SUMMON_CAMERA_MOVE_ORBIT:
                delta->vx = g_BattleSummonTyphonCameraPitch->posX - g_pBattleSummonTyphonCameraTrack->posX;
                delta->vy = g_BattleSummonTyphonCameraPitch->posY - g_pBattleSummonTyphonCameraTrack->posY;
                delta->vz = g_BattleSummonTyphonCameraPitch->posZ - g_pBattleSummonTyphonCameraTrack->posZ;
                g_pBattleSummonTyphonCameraTrack->progress = SqrtToInt(delta->vx * delta->vx + delta->vy * delta->vy + delta->vz * delta->vz);
                g_pBattleSummonTyphonCameraTrack->delta = g_pBattleSummonTyphonCameraTrack->pos;
                g_pBattleSummonTyphonCameraTrack->stepCount = BattleFx_TyphonReadCameraScriptOperand(&state->scriptPtr);
                delta->vx = *state->scriptPtr;
                state->scriptPtr++;
                delta->vy = *state->scriptPtr;
                state->scriptPtr++;
                delta->vz = *state->scriptPtr;
                state->scriptPtr++;
                g_pBattleSummonTyphonCameraTrack->stepX = (delta->vx - g_pBattleSummonTyphonCameraTrack->posX) / g_pBattleSummonTyphonCameraTrack->stepCount;
                g_pBattleSummonTyphonCameraTrack->stepY = (delta->vy - g_pBattleSummonTyphonCameraTrack->posY) / g_pBattleSummonTyphonCameraTrack->stepCount;
                g_pBattleSummonTyphonCameraTrack->stepZ = (delta->vz - g_pBattleSummonTyphonCameraTrack->posZ) / g_pBattleSummonTyphonCameraTrack->stepCount;
                switch (g_BattleSummonTyphonCameraPitch->mode) {
                case SUMMON_CAMERA_MOVE_LINEAR:
                    offsetPtr->vx = g_BattleSummonTyphonCameraPitch->posX + g_BattleSummonTyphonCameraPitch->stepX * g_BattleSummonTyphonCameraPitch->stepCount;
                    offsetPtr->vy = g_BattleSummonTyphonCameraPitch->posY + g_BattleSummonTyphonCameraPitch->stepY * g_BattleSummonTyphonCameraPitch->stepCount;
                    offsetPtr->vz = g_BattleSummonTyphonCameraPitch->posZ + g_BattleSummonTyphonCameraPitch->stepZ * g_BattleSummonTyphonCameraPitch->stepCount;
                    break;
                default:
                    offsetPtr->vx = g_BattleSummonTyphonCameraPitch->posX;
                    offsetPtr->vy = g_BattleSummonTyphonCameraPitch->posY;
                    offsetPtr->vz = g_BattleSummonTyphonCameraPitch->posZ;
                }
                delta->vx -= offsetPtr->vx;
                delta->vy -= offsetPtr->vy;
                delta->vz -= offsetPtr->vz;
                g_pBattleSummonTyphonCameraTrack->stepSize = (SqrtToInt(delta->vx * delta->vx + delta->vy * delta->vy + delta->vz * delta->vz) - g_pBattleSummonTyphonCameraTrack->progress) / g_pBattleSummonTyphonCameraTrack->stepCount;
                break;
            case SUMMON_CAMERA_MOVE_ACCEL:
                g_pBattleSummonTyphonCameraTrack->stepCount = BattleFx_TyphonReadCameraScriptOperand(&state->scriptPtr);
                delta->vx = *state->scriptPtr - g_pBattleSummonTyphonCameraTrack->posX;
                state->scriptPtr++;
                delta->vy = *state->scriptPtr - g_pBattleSummonTyphonCameraTrack->posY;
                state->scriptPtr++;
                delta->vz = *state->scriptPtr - g_pBattleSummonTyphonCameraTrack->posZ;
                state->scriptPtr++;
                g_pBattleSummonTyphonCameraTrack->deltaX = (delta->vx - g_pBattleSummonTyphonCameraTrack->stepX * g_pBattleSummonTyphonCameraTrack->stepCount << 1) / (g_pBattleSummonTyphonCameraTrack->stepCount * g_pBattleSummonTyphonCameraTrack->stepCount);
                g_pBattleSummonTyphonCameraTrack->deltaY = (delta->vy - g_pBattleSummonTyphonCameraTrack->stepY * g_pBattleSummonTyphonCameraTrack->stepCount << 1) / (g_pBattleSummonTyphonCameraTrack->stepCount * g_pBattleSummonTyphonCameraTrack->stepCount);
                g_pBattleSummonTyphonCameraTrack->deltaZ = (delta->vz - g_pBattleSummonTyphonCameraTrack->stepZ * g_pBattleSummonTyphonCameraTrack->stepCount << 1) / (g_pBattleSummonTyphonCameraTrack->stepCount * g_pBattleSummonTyphonCameraTrack->stepCount);
                break;
            case SUMMON_CAMERA_MOVE_EASED:
                g_pBattleSummonTyphonCameraTrack->stepCount = BattleFx_TyphonReadCameraScriptOperand(&state->scriptPtr);
                g_pBattleSummonTyphonCameraTrack[1].mode = *state->scriptPtr;
                state->scriptPtr++;
                g_pBattleSummonTyphonCameraTrack->progress = 0;
                g_pBattleSummonTyphonCameraTrack->stepSize = 0x1000 / g_pBattleSummonTyphonCameraTrack->stepCount;
                g_pBattleSummonTyphonCameraTrack->deltaX = g_pBattleSummonTyphonCameraTrack->posX;
                g_pBattleSummonTyphonCameraTrack->deltaY = g_pBattleSummonTyphonCameraTrack->posY;
                g_pBattleSummonTyphonCameraTrack->deltaZ = g_pBattleSummonTyphonCameraTrack->posZ;
                g_pBattleSummonTyphonCameraTrack->stepX = *state->scriptPtr - g_pBattleSummonTyphonCameraTrack->posX;
                state->scriptPtr++;
                g_pBattleSummonTyphonCameraTrack->stepY = *state->scriptPtr - g_pBattleSummonTyphonCameraTrack->posY;
                state->scriptPtr++;
                g_pBattleSummonTyphonCameraTrack->stepZ = *state->scriptPtr - g_pBattleSummonTyphonCameraTrack->posZ;
                state->scriptPtr++;
                break;
            case SUMMON_CAMERA_ATTACH_BONE:
                if ((g_pBattleSummonTyphonCameraTrack->attachPoint = *(*(short **)&state->scriptCursor)++) == 0) {
                    g_pBattleSummonTyphonCameraTrack->attachPoint = state->attachPoint;
                }
                g_pBattleSummonTyphonCameraTrack->attachIndex = *state->scriptPtr;
                state->scriptPtr++;
                break;
            case SUMMON_CAMERA_DETACH:
                g_pBattleSummonTyphonCameraTrack->attachPoint = 0xFFFF;
                break;
            case SUMMON_CAMERA_HOLD_OUTPUT:
                g_pBattleSummonTyphonCameraTrack->stepCount = BattleFx_TyphonReadCameraScriptOperand(&state->scriptPtr);
                break;
            default:
                state->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
        g_pBattleSummonTyphonCameraTrack = (struct BattleFxCameraOrbitState *)&g_BattleSummonTyphonFallParam0;
        g_BattleSummonTyphonCameraPitch = (struct BattleFxCameraOrbitState *)&g_BattleSummonTyphonCameraTrackId;
        for (pass = 0; pass < 2; pass++) {
            switch (g_pBattleSummonTyphonCameraTrack->mode) {
            case SUMMON_CAMERA_WAIT:
                break;
            case SUMMON_CAMERA_MOVE_ACCEL:
                g_pBattleSummonTyphonCameraTrack->stepX += g_pBattleSummonTyphonCameraTrack->deltaX;
                g_pBattleSummonTyphonCameraTrack->stepY += g_pBattleSummonTyphonCameraTrack->deltaY;
                g_pBattleSummonTyphonCameraTrack->stepZ += g_pBattleSummonTyphonCameraTrack->deltaZ;
            case SUMMON_CAMERA_MOVE_LINEAR:
                g_pBattleSummonTyphonCameraTrack->posX += g_pBattleSummonTyphonCameraTrack->stepX;
                g_pBattleSummonTyphonCameraTrack->posY += g_pBattleSummonTyphonCameraTrack->stepY;
                g_pBattleSummonTyphonCameraTrack->posZ += g_pBattleSummonTyphonCameraTrack->stepZ;
                break;
            case SUMMON_CAMERA_MOVE_ORBIT:
                g_pBattleSummonTyphonCameraTrack->deltaX += g_pBattleSummonTyphonCameraTrack->stepX;
                g_pBattleSummonTyphonCameraTrack->deltaY += g_pBattleSummonTyphonCameraTrack->stepY;
                g_pBattleSummonTyphonCameraTrack->deltaZ += g_pBattleSummonTyphonCameraTrack->stepZ;
                g_pBattleSummonTyphonCameraTrack->progress += g_pBattleSummonTyphonCameraTrack->stepSize;
                pVector->offsetX = g_BattleSummonTyphonCameraPitch->posX - g_pBattleSummonTyphonCameraTrack->deltaX;
                pVector->offsetY = g_BattleSummonTyphonCameraPitch->posY - g_pBattleSummonTyphonCameraTrack->deltaY;
                pVector->offsetZ = g_BattleSummonTyphonCameraPitch->posZ - g_pBattleSummonTyphonCameraTrack->deltaZ;
                VectorNormalToSVector((int *)pVector, (short *)delta);
                g_pBattleSummonTyphonCameraTrack->posX = g_BattleSummonTyphonCameraPitch->posX - (delta->vx * g_pBattleSummonTyphonCameraTrack->progress >> 0xC);
                g_pBattleSummonTyphonCameraTrack->posY = g_BattleSummonTyphonCameraPitch->posY - (delta->vy * g_pBattleSummonTyphonCameraTrack->progress >> 0xC);
                g_pBattleSummonTyphonCameraTrack->posZ = g_BattleSummonTyphonCameraPitch->posZ - (delta->vz * g_pBattleSummonTyphonCameraTrack->progress >> 0xC);
                break;
            case SUMMON_CAMERA_MOVE_EASED:
                g_pBattleSummonTyphonCameraTrack->progress += g_pBattleSummonTyphonCameraTrack->stepSize;
                easeVal = g_pBattleSummonTyphonCameraTrack->progress;
                for (easeIndex = 0; easeIndex < g_pBattleSummonTyphonCameraTrack[1].mode; easeIndex++) {
                    easeVal = SinFixed(easeVal / 4);
                }
                g_pBattleSummonTyphonCameraTrack->posX = g_pBattleSummonTyphonCameraTrack->deltaX + (g_pBattleSummonTyphonCameraTrack->stepX * easeVal >> 0xC);
                g_pBattleSummonTyphonCameraTrack->posY = g_pBattleSummonTyphonCameraTrack->deltaY + (g_pBattleSummonTyphonCameraTrack->stepY * easeVal >> 0xC);
                g_pBattleSummonTyphonCameraTrack->posZ = g_pBattleSummonTyphonCameraTrack->deltaZ + (g_pBattleSummonTyphonCameraTrack->stepZ * easeVal >> 0xC);
            }
            (g_pBattleSummonTyphonCameraTrack->stepCount)--;
            g_pBattleSummonTyphonCameraTrack = (struct BattleFxCameraOrbitState *)&g_BattleSummonTyphonCameraTrackId;
            g_BattleSummonTyphonCameraPitch = (struct BattleFxCameraOrbitState *)&g_BattleSummonTyphonFallParam0;
        }
    }
    if (state->matrixPtr != 0) {
        if ((g_BattleSummonTyphonFallParam0.state) != SUMMON_CAMERA_HOLD_OUTPUT) {
            SetRotMatrix(state->matrix);
            SetTransMatrix(state->matrix);
            if ((g_BattleSummonTyphonFallParam1.actorIndex) == -1) {
                RotTransPoint(&g_BattleSummonTyphonFallParam2, (int *)pVector, &res);
                g_BattleCameraPosition.cameraPosX = pVector->offsetXShort;
                g_BattleCameraPosition.cameraPosY = pVector->offsetYShort;
                g_BattleCameraPosZ = pVector->offsetZShort;
            } else {
                RotateSVectorByInstalledMatrix(&g_BattleSummonTyphonFallParam2, (void *)pVector);
                if (g_BattleSummonTyphonFallParam1.boneIdx != -1) {
                    Battle_GetBoneWorldPos((g_BattleSummonTyphonFallParam1.actorIndex), g_BattleSummonTyphonFallParam1.boneIdx, (short *)delta);
                } else {
                    delta->vx = g_BattleActorRecords[(g_BattleSummonTyphonFallParam1.actorIndex)].posX;
                    delta->vy = g_BattleActorRecords[(g_BattleSummonTyphonFallParam1.actorIndex)].posY;
                    delta->vz = g_BattleActorRecords[(g_BattleSummonTyphonFallParam1.actorIndex)].posZ;
                }
                g_BattleCameraPosition.cameraPosX = pVector->offsetX + delta->vx;
                g_BattleCameraPosition.cameraPosY = pVector->offsetY + delta->vy;
                g_BattleCameraPosZ = pVector->offsetZ + delta->vz;
                SetRotMatrix(state->matrix);
                SetTransMatrix(state->matrix);
            }
        }
        if ((g_BattleSummonTyphonCameraTrackId.state) != SUMMON_CAMERA_HOLD_OUTPUT) {
            if ((g_BattleSummonTyphonTargetBone.actorIndex) == -1) {
                RotTransPoint(&g_BattleSummonTyphonRotationSpeed, (int *)pVector, &res);
                g_BattleCameraFocalPoint.focalPointX = pVector->offsetXShort;
                g_BattleCameraFocalPoint.focalPointY = pVector->offsetYShort;
                g_BattleCameraFocusZ = pVector->offsetZShort;
            } else {
                RotateSVectorByInstalledMatrix(&g_BattleSummonTyphonRotationSpeed, (void *)pVector);
                if (g_BattleSummonTyphonTargetBone.boneIdx != -1) {
                    Battle_GetBoneWorldPos((g_BattleSummonTyphonTargetBone.actorIndex), g_BattleSummonTyphonTargetBone.boneIdx, (short *)delta);
                } else {
                    delta->vx = g_BattleActorRecords[(g_BattleSummonTyphonTargetBone.actorIndex)].posX;
                    delta->vy = g_BattleActorRecords[(g_BattleSummonTyphonTargetBone.actorIndex)].posY;
                    delta->vz = g_BattleActorRecords[(g_BattleSummonTyphonTargetBone.actorIndex)].posZ;
                }
                g_BattleCameraFocalPoint.focalPointX = pVector->offsetX + delta->vx;
                g_BattleCameraFocalPoint.focalPointY = pVector->offsetY + delta->vy;
                g_BattleCameraFocusZ = pVector->offsetZ + delta->vz;
            }
        }
    } else {
        if ((g_BattleSummonTyphonFallParam0.state) != SUMMON_CAMERA_HOLD_OUTPUT) {
            *(int *)&g_BattleCameraPosition = g_BattleSummonTyphonFallParam2;
            *(int *)&g_BattleCameraPosZ = g_BattleSummonTyphonFallParam3;
        }
        if ((g_BattleSummonTyphonCameraTrackId.state) != SUMMON_CAMERA_HOLD_OUTPUT) {
            *(int *)&g_BattleCameraFocalPoint = g_BattleSummonTyphonRotationSpeed;
            *(int *)&g_BattleCameraFocusZ = g_BattleSummonTyphonRingVertexCount;
        }
    }
}

/* Reads next variable-length operand from active camera script stream. */
// FUNCTION: FF7 0x004d688e
short __cdecl BattleFx_TyphonReadCameraScriptOperand(short **cursorPtr)
{
    int operand;

    operand = **cursorPtr;
    (*cursorPtr)++;
    if (operand < 0) {
        operand = (&g_BattleSummonTyphonBoundingCenter.cameraVarTable)[-operand];
    }
    return (short)operand;
}

/* Clears active particle callback table for Typhon summon. */
// FUNCTION: FF7 0x004d68c8
void __cdecl BattleFx_SummonTyphonClearParticleCallbacks(void)
{
    int i;

    for (i = 0; i < 0x12C; i++) {
        g_BattleSummonTyphonScratchBuffer[i] = 0;
    }
}

/* Executes particle callbacks and reaps expired particle records. */
// FUNCTION: FF7 0x004d68fb
void __cdecl BattleFx_SummonTyphonParticleCallbackTask(void)
{
    struct {
        int recordIndex;
        int slot;
        int active;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = (int)BATTLE_CURRENT_TASK_SLOT;
    frame.active = 0;
    for (frame.recordIndex = 0; frame.recordIndex < 0x12c; frame.recordIndex++) {
        if (g_BattleSummonTyphonScratchBuffer[frame.recordIndex] != 0) {
            g_BattleSummonTyphonScratchBuffer[frame.recordIndex]((int)&g_BattleSummonTyphonRingBuffer[frame.recordIndex]);
            if (g_BattleSummonTyphonRingBuffer[frame.recordIndex].state == -1) {
                g_BattleSummonTyphonScratchBuffer[frame.recordIndex] = 0;
            } else {
                frame.active++;
            }
        }
    }
    if (frame.active == 0 && g_BattleSecondaryTaskActiveCount <= 1) {
        *(short *)frame.slot = -1;
    }
}

/* Main timeline task coordinating sound cues, overlays, and model spawning. */
// FUNCTION: FF7 0x004d69a6
void __cdecl BattleFx_SummonTyphonTimelineTask(void)
{
    int frameIndex;
    BattleFxTyphonTimelineRecord *ringRecord;
    BattleTaskSlot *taskPtr;
    int actorIndex;
    BattleFxTyphonTimelineSlot *spawned;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    MultiplyMatrix(g_BattleCameraTransform.matrix, &g_BattleSummonTyphonFallParam10, &g_BattleSummonTyphonViewMatrix);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frameIndex = taskPtr->frameCounter;
    if (frameIndex < 5) {
        if (frameIndex == 0) {
            Battle_AllocateResourceSlot(BattleFx_SummonTyphonFlashColorCycleTask);
            Battle_StartFixedPointRamp(0x1000, 5);
        }
    } else {
        frameIndex -= 5;
        if (frameIndex < 0x32) {
            if (frameIndex == 0) {
                spawned = (BattleFxTyphonTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonTyphonParticleModel2EmitterTask);
                spawned = (BattleFxTyphonTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonTyphonParticleModel3EmitterTask);
            }
            if (frameIndex == 0) {
                Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 0);
            }
        } else {
            frameIndex -= 0x32;
            if (frameIndex < 0xA) { /* empty first arm: lib/codegen.h */
            } else {
                frameIndex -= 0xA;
                if (frameIndex < 0xA) {
                    if (frameIndex == 9) {
                        ringRecord = (BattleFxTyphonTimelineRecord *)Battle_AllocSceneMessage(1);
                        ringRecord->vectorX = 0;
                        ringRecord->vectorY = 0xB1E0;
                        ringRecord->fallPosition = *(short *)&g_BattleSummonTyphonFallParam10.position;
                        ringRecord->rotationY = g_BattleSummonTyphonFallParam11.rotY + 0x800;
                        Battle_AllocateResourceSlot(BattleFx_SummonTyphonActorFlagFadeTask);
                    }
                } else {
                    frameIndex -= 0xA;
                    if (frameIndex < 0xF) {
                        if (frameIndex == 0) {
                            spawned = (BattleFxTyphonTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonTyphonPairedModelTask);
                            spawned->pointA = g_BattleSummonTyphonPairedModelPoints[0];
                            spawned->pointB = g_BattleSummonTyphonPairedModelPoints[1];
                        }
                    } else {
                        frameIndex -= 0xF;
                        if (frameIndex < 0x1E) { /* empty first arm: lib/codegen.h */
                        } else {
                            frameIndex -= 0x1E;
                            if (frameIndex < 0x23) {
                                if (frameIndex == 0) {
                                    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 1);
                                }
                                if (frameIndex == 0x22) {
                                    BattleFx_SummonTyphonPrepareStageOffscreenRequests();
                                }
                            } else {
                                frameIndex -= 0x23;
                                if (frameIndex < 0xF) {
                                    if (frameIndex == 0) {
                                        spawned = (BattleFxTyphonTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonTyphonOrbitSpawnTask);
                                    }
                                } else {
                                    frameIndex -= 0xF;
                                    if (frameIndex < 0x1E) {
                                        if (frameIndex == 0) {
                                            spawned = (BattleFxTyphonTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonTyphonDeformingRingsTask);
                                            spawned->angleX = rand() & 0xFFF;
                                            spawned->angleY = rand() & 0xFFF;
                                            spawned->angleZ = rand() & 0xFFF;
                                            spawned->clearedShort2 = 0;
                                            spawned->clearedShort1 = 0;
                                            spawned->clearedShort0 = 0;
                                        }
                                        if (frameIndex == 0) {
                                            spawned = (BattleFxTyphonTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonTyphonParticleEmitterTask);
                                        }
                                    } else {
                                        frameIndex -= 0x1E;
                                        if (frameIndex < 0x14) { /* empty first arm: lib/codegen.h */
                                        } else {
                                            frameIndex -= 0x14;
                                            if (frameIndex < 0x1E) { /* empty first arm: lib/codegen.h */
                                            } else {
                                                frameIndex -= 0x1E;
                                                if (frameIndex < 0x23) {
                                                    if (frameIndex == 0x22) {
                                                        Battle_AllocateResourceSlot(BattleFx_SummonTyphonCameraRotationTask);
                                                    }
                                                    if (frameIndex == 0x1E) {
                                                        Battle_AllocateResourceSlot(BattleFx_SummonTyphonFadeOverlayPulseTask);
                                                    }
                                                    if (frameIndex == 0x1E) {
                                                        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 2);
                                                        Sfx_ReleaseCachedBuffer(0x21A, 0);
                                                        Sfx_ReleaseCachedBuffer(0x21B, 0);
                                                        Sfx_ReleaseCachedBuffer(0x21C, 0);
                                                    }
                                                } else {
                                                    frameIndex -= 0x23;
                                                    if (frameIndex < 0x19) {
                                                        if (frameIndex == 0) {
                                                            spawned = (BattleFxTyphonTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonTyphonRotatingModelTask);
                                                        }
                                                        if (frameIndex == 0x17) {
                                                            Battle_AllocSceneMessage(2);
                                                        }
                                                    } else {
                                                        frameIndex -= 0x19;
                                                        if (frameIndex < 0x2D) {
                                                            if (frameIndex == 0) {
                                                                spawned = (BattleFxTyphonTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonTyphonModelHexagonEmitterTask);
                                                                spawned = (BattleFxTyphonTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonTyphonHexagonQuadsTask);
                                                                spawned = (BattleFxTyphonTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonTyphonWhiteFadeInTask);
                                                            }
                                                            if (frameIndex == 0) {
                                                                Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 3);
                                                                Sfx_ReleaseCachedBuffer(0x21D, 1);
                                                                Sfx_ReleaseCachedBuffer(0x21E, 1);
                                                                Sfx_ReleaseCachedBuffer(0x21F, 1);
                                                            }
                                                            if (frameIndex == 0xA && g_BattleTimelineTargetMask != 0) {
                                                                Battle_CreateBitPatternCallbackTask(g_BattleTimelineTargetMask, 0, 2, BattleFx_SummonSpawnTyphonActorFallPerTarget);
                                                            }
                                                        } else {
                                                            frameIndex -= 0x2D;
                                                            if (frameIndex < 0x32) {
                                                                if (frameIndex == 0x19) {
                                                                    BattleFx_SummonTyphonBeginFadeOverlay(0xF);
                                                                }
                                                                if (frameIndex == 0x31) {
                                                                    for (actorIndex = 0; actorIndex < 0xA; actorIndex++) {
                                                                        if ((g_BattleSummonTyphonTargetMaskB & 1 << actorIndex) != 0) {
                                                                            g_BattleActorRecords[actorIndex].posY = g_BattleSummonTyphonActorTargets[actorIndex];
                                                                        }
                                                                    }
                                                                    g_BattleFxTimelineState.startValue = 0;
                                                                    g_BattleStageHideFlags &= 0xFB;
                                                                }
                                                            } else {
                                                                BattleFx_SummonTyphonSetFadeOverlayFadeOutRate(5);
                                                                if ((g_BattleSummonTyphonTargetMaskB & ~g_BattleTimelineTargetMask) != 0) {
                                                                    for (actorIndex = 0; actorIndex < 0xA; actorIndex++) {
                                                                        if ((g_BattleSummonTyphonTargetMaskB & ~g_BattleTimelineTargetMask & 1 << actorIndex) != 0) {
                                                                            Battle_PostActorHitOrHurtMessage(actorIndex);
                                                                        }
                                                                    }
                                                                }
                                                                g_BattleStageHideFlags = 0;
                                                                taskPtr->ownerSlot = BATTLE_TASK_FREE;
                                                                if (g_BattleSummonTyphonEndMarker >= 0) {
                                                                    Battle3d_ReleaseOffscreenSlot(g_BattleSummonTyphonEndMarker);
                                                                    g_BattleSummonTyphonEndMarker = 0xFFFFFFFF;
                                                                }
                                                                if (g_BattleSummonTyphonPairMarker[0] >= 0) {
                                                                    Battle3d_ReleaseOffscreenSlot(g_BattleSummonTyphonPairMarker[0]);
                                                                    g_BattleSummonTyphonPairMarker[0] = 0xFFFFFFFF;
                                                                }
                                                                Sfx_ReleaseCachedBuffer(0x220, 2);
                                                                Sfx_ReleaseCachedBuffer(0x221, 2);
                                                                Sfx_ReleaseCachedBuffer(0x222, 2);
                                                                Sfx_ReleaseCachedBuffer(0x223, 3);
                                                                Sfx_ReleaseCachedBuffer(0x224, 3);
                                                                Sfx_ReleaseCachedBuffer(0x225, 3);
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
    (taskPtr->frameCounter)++;
}

/* Initializes full-screen color fade overlay task. */
// FUNCTION: FF7 0x004d6ff8
void __cdecl BattleFx_SummonTyphonBeginFadeOverlay(int frames)
{
    if (g_BattleSummonTyphonFadeRamp != 0) {
        return;
    }
    g_BattleSummonTyphonFadeRamp = (struct BattleScreenFlashRamp *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonTyphonFadeOverlayTask);
    g_BattleSummonTyphonFadeRamp->value = 0;
    g_BattleSummonTyphonFadeRamp->step = 0x10000 / frames;
}

/* Updates screen fade overlay alpha transition. */
// FUNCTION: FF7 0x004d7044
void __cdecl BattleFx_SummonTyphonFadeOverlayTask(void)
{
    BattleFxFadeOverlayTask *task;
    int fadeByte;

    task = (BattleFxFadeOverlayTask *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSummonTyphonSpriteConfig.viewportOriginX = *(short *)&g_BattleViewportOriginX;
    g_BattleSummonTyphonSpriteConfig.viewportOriginY = *(short *)&g_BattleViewportOriginY;
    g_BattleSummonTyphonSpriteConfig.viewportWidth = (short)g_BattleViewportWidth;
    g_BattleSummonTyphonSpriteConfig.viewportHeight = *(short *)&g_BattleSceneViewportHeight;
    if (g_BattleIsPaused == 0 && task->pendingStep != 0) {
        task->fadeLevel += task->pendingStep;
        if (task->fadeLevel <= 0) {
            task->ownerSlot = BATTLE_TASK_FREE;
            g_BattleSummonTyphonFadeRamp = 0;
            return;
        }
        if (task->fadeLevel > 0xFFFF) {
            task->fadeLevel = 0xFFFF;
            task->pendingStep = 0;
        }
    }
    fadeByte = (task->fadeLevel >> 8) & 0xFF;
    g_BattleSummonTyphonSpriteConfig.fadeHistory2 = (unsigned char)fadeByte;
    g_BattleSummonTyphonSpriteConfig.fadeHistory1 = g_BattleSummonTyphonSpriteConfig.fadeHistory2;
    g_BattleSummonTyphonSpriteConfig.fadeHistory0 = g_BattleSummonTyphonSpriteConfig.fadeHistory1;
    Gfx_SetBlendFactorPair(FLOAT_0_1_BITS, FLOAT_ONE_BITS);
    Gfx_DrawPsxPrimitive(&g_BattleSummonTyphonSpriteConfig);
}

/* Sets fade-out speed modifier for screen overlay. */
// FUNCTION: FF7 0x004d714b
void __cdecl BattleFx_SummonTyphonSetFadeOverlayFadeOutRate(int frames)
{
    if (g_BattleSummonTyphonFadeRamp == 0) {
        return;
    }
    g_BattleSummonTyphonFadeRamp->step = (0 - g_BattleSummonTyphonFadeRamp->value) / frames;
}

/* Cycles background flash colors during summon windstorm. */
// FUNCTION: FF7 0x004d7174
void __cdecl BattleFx_SummonTyphonFlashColorCycleTask(void)
{
    int subStep;
    BattleFxBlasterFlashSlot *taskPtr;
    int fromIndex;
    int toIndex;
    int blend;

    taskPtr = (BattleFxBlasterFlashSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    subStep = taskPtr->phase % 0x10;
    blend = (subStep << 0xC) / 0x10;
    fromIndex = (taskPtr->phase / 0x10) % 6;
    toIndex = (fromIndex + 1) % 6;
    g_BattleFxTimelineState.red = g_BattleSummonTyphonColorPalette[fromIndex].red + ((g_BattleSummonTyphonColorPalette[toIndex].red - g_BattleSummonTyphonColorPalette[fromIndex].red) * blend >> 0xC);
    g_BattleFxTimelineState.channel1 = g_BattleSummonTyphonColorPalette[fromIndex].channel1 + ((g_BattleSummonTyphonColorPalette[toIndex].channel1 - g_BattleSummonTyphonColorPalette[fromIndex].channel1) * blend >> 0xC);
    g_BattleFxTimelineState.channel2 = g_BattleSummonTyphonColorPalette[fromIndex].channel2 + ((g_BattleSummonTyphonColorPalette[toIndex].channel2 - g_BattleSummonTyphonColorPalette[fromIndex].channel2) * blend >> 0xC);
    (taskPtr->phase)++;
    if (taskPtr->phase >= 0x195) {
        g_BattleFxTimelineState.red = 0;
        g_BattleFxTimelineState.channel1 = 0;
        g_BattleFxTimelineState.channel2 = 0;
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Spawns swirling wind and debris particle model 2 emitters. */
// FUNCTION: FF7 0x004d72d0
void __cdecl BattleFx_SummonTyphonParticleModel2EmitterTask(void)
{
    int frameIndex;
    BattleTaskSlot *taskPtr;
    int progress;
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int emitCount;
    int spreadFactor;
    BattleFxTyphonEmitterRecord *particle;
    int radiusRand;
    int baseX;
    int baseY;
    int baseZ;
    int heading;
    int ring;
    int zoomScale;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    frameIndex = taskPtr->frameCounter;
    if (frameIndex < 0x3C) {
        progress = (frameIndex << 0xC) / 0x3C;
        emitCount = (progress << 7 >> 0xC) + 0x80;
        spreadFactor = progress;
        zoomScale = (progress * 0xC00 >> 0xC) + 0x400;
    } else {
        frameIndex -= 0x3C;
        if (frameIndex < 0x19) {
            emitCount = 0x100;
            spreadFactor = 0x1000;
            zoomScale = 0x1000;
        } else {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    emitCount = (emitCount * taskPtr->frameCounter & 0xFF) + emitCount;
    while (emitCount >= 0x100) {
        particle = (BattleFxTyphonEmitterRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonTyphonParticleModel2Task);
        particle->frameCount = taskPtr->frameCounter;
        heading = rand() & 0xFFF;
        if (DEAD_IN_RETAIL != 0) {
            radiusRand = 0x12C;
        } else {
            radiusRand = rand() % 0x12C;
        }
        ring = radiusRand;
        particle->sinOffset = ring * SinFixed(heading) >> 0xC;
        if (DEAD_IN_RETAIL != 0) {
            baseX = 0xFFFFF060;
        } else {
            baseX = rand() % 0xFA0 - 0x1F40;
        }
        particle->offsetX = baseX * spreadFactor >> 0xC;
        particle->cosOffset = ring * CosFixed(heading) >> 0xC;
        if (DEAD_IN_RETAIL != 0) {
            baseY = 0x32;
        } else {
            baseY = rand() % 0x14 + 0x1E;
        }
        particle->offsetY = baseY * spreadFactor >> 0xC;
        particle->jitterA = (rand() % 0x40) - 0x20 >> 4;
        particle->phase = rand() & 0xFF;
        particle->jitterB = (rand() % 0x40) - 0x20 >> 4;
        if (DEAD_IN_RETAIL != 0) {
            baseZ = 0x900;
        } else {
            baseZ = (rand() % 0x200) + 0x700;
        }
        particle->offsetZ = baseZ * zoomScale >> 0xC;
        emitCount -= 0x100;
    }
    (taskPtr->frameCounter)++;
}

/* Updates motion and scaling of swirling particle model 2 instances. */
// FUNCTION: FF7 0x004d7549
void __cdecl BattleFx_SummonTyphonParticleModel2Task(void)
{
    int elapsed;
    int i;
    BattleFxTyphonParticleSlot *particle;
    int ratio;

    particle = (BattleFxTyphonParticleSlot *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonTyphonOrbitState & 1) == 0) {
        g_BattleSummonTyphonOrbitState |= 1;
        g_BattleSummonTyphonModelVariant = (struct BattleFxTyphonModelVariant *)(Gfx_GetPsxScratchpadPtr(0));
    }
    (g_BattleSummonTyphonRingConfig0.ringValue) = Gfx_ModelTableGetRenderObject(2, g_BattleSummonTyphonFallParam9);
    if (particle->phase + particle->ownerSlot >= 0x5A) {
        elapsed = particle->phase + particle->ownerSlot + -0x5A;
        if (elapsed >= 0xA) {
            particle->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
        ratio = (elapsed << 0xC) / 0xA;
        g_BattleSummonTyphonRingConfig0.blend = ratio;
        for (i = 0; i < 3; i++) {
            ratio = SinFixed(ratio / 4);
        }
        g_BattleSummonTyphonModelVariant->offset = particle->scale * ((ratio << 0xC >> 0xC) + 0x1000) >> 0xC;
        g_BattleSummonTyphonModelVariant->prevOffset = g_BattleSummonTyphonModelVariant->offset;
        g_BattleSummonTyphonModelVariant->scale = particle->scale;
    } else {
        elapsed = particle->phase;
        if (elapsed < 5) {
            ratio = (elapsed << 0xC) / 5;
            g_BattleSummonTyphonRingConfig0.blend = 0x1000 - ratio;
        } else {
            elapsed -= 5;
            if (elapsed < 5) {
                g_BattleSummonTyphonRingConfig0.blend = 0;
            } else {
                elapsed -= 5;
                if (elapsed < 5) {
                    ratio = (elapsed << 0xC) / 5;
                    g_BattleSummonTyphonRingConfig0.blend = ratio;
                } else {
                    particle->ownerSlot = BATTLE_TASK_FREE;
                    return;
                }
            }
        }
        ratio = (particle->phase << 0xC) / 0xF;
        g_BattleSummonTyphonModelVariant->offset = particle->scale * ((ratio * -0x800 >> 0xC) + 0x1000) >> 0xC;
        g_BattleSummonTyphonModelVariant->scale = g_BattleSummonTyphonModelVariant->offset;
        g_BattleSummonTyphonModelVariant->prevOffset = g_BattleSummonTyphonModelVariant->scale;
    }
    g_BattleSummonTyphonModelVariant->x = particle->gridX << 4;
    g_BattleSummonTyphonModelVariant->y = particle->gridY << 4;
    g_BattleSummonTyphonModelVariant->z = particle->gridZ << 4;
    RotMatrix((SVECTOR *)g_BattleSummonTyphonModelVariant, &g_BattleSummonTyphonModelVariant->matrix);
    ScaleMatrix(&g_BattleSummonTyphonModelVariant->matrix, &g_BattleSummonTyphonModelVariant->prevOffset);
    g_BattleSummonTyphonModelVariant->transX = particle->posX;
    g_BattleSummonTyphonModelVariant->transY = particle->posY;
    g_BattleSummonTyphonModelVariant->transZ = particle->posZ;
    MultiplyMatrix(&g_BattleSummonTyphonViewMatrix, &g_BattleSummonTyphonModelVariant->matrix, &g_BattleSummonTyphonModelVariant->matrix);
    SetRotMatrix(&g_BattleSummonTyphonModelVariant->matrix);
    SetTransMatrix(&g_BattleSummonTyphonModelVariant->matrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonTyphonRingConfig0, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (particle->phase)++;
        particle->posY += particle->velocityY;
        *(char *)&particle->gridY += 0x10;
    }
}

/* Spawns swirling wind and debris particle model 3 emitters. */
// FUNCTION: FF7 0x004d78b9
void __cdecl BattleFx_SummonTyphonParticleModel3EmitterTask(void)
{
    int angle;
    int range;
    int scaleRamp;
    int tempEmitRate;
    int unusedEntry; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int ratio;
    int liftRamp;
    BattleFxTyphonParticleModel3 *spawned;
    int rollDist;
    int sizeRand;
    int rawVelY;
    int age;
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    age = task->frameCounter;
    if (age < 0x3C) {
        ratio = (age << 0xC) / 0x3C;
        tempEmitRate = (ratio * 0x30 >> 0xC) + 0x10;
        liftRamp = ratio;
        scaleRamp = (ratio * 0xC00 >> 0xC) + 0x400;
    } else {
        age -= 0x3C;
        if (age < 0x19) {
            tempEmitRate = 0x40;
            liftRamp = 0x1000;
            scaleRamp = 0x1000;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    tempEmitRate = (tempEmitRate * task->frameCounter & 0xFF) + tempEmitRate;
    while (tempEmitRate >= 0x100) {
        spawned = (BattleFxTyphonParticleModel3 *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonTyphonParticleModel3Task);
        spawned->frameCount = task->frameCounter;
        angle = rand() & 0xFFF;
        if (DEAD_IN_RETAIL != 0) {
            rollDist = 0xC8;
        } else {
            rollDist = rand() % 0xC8;
        }
        range = rollDist;
        spawned->velocityX = range * SinFixed(angle) >> 0xC;
        if (DEAD_IN_RETAIL != 0) {
            rawVelY = 0xFFFFF060;
        } else {
            rawVelY = rand() % 0xFA0 - 0x1F40;
        }
        spawned->velocityY = rawVelY * liftRamp >> 0xC;
        spawned->velocityZ = range * CosFixed(angle) >> 0xC;
        spawned->angleX = 0;
        spawned->phase = rand() & 0xFFF;
        spawned->angleZ = 0;
        if (DEAD_IN_RETAIL != 0) {
            sizeRand = 0x850;
        } else {
            sizeRand = rand() % 0x250 + 0x600;
        }
        spawned->scale = sizeRand * scaleRamp >> 0xC;
        tempEmitRate -= 0x100;
    }
    (task->frameCounter)++;
}

/* Updates motion and scaling of swirling particle model 3 instances. */
// FUNCTION: FF7 0x004d7ad0
void __cdecl BattleFx_SummonTyphonParticleModel3Task(void)
{
    int elapsed;
    int i;
    BattleFxTyphonParticle3 *particle;
    int ratio;

    particle = (BattleFxTyphonParticle3 *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonTyphonBoundingCenter.flags & 1) == 0) {
        g_BattleSummonTyphonBoundingCenter.flags |= 1;
        g_BattleSummonTyphonDrawHandle = (BattleFxTyphonDrawHandle *)Gfx_GetPsxScratchpadPtr(0);
    }
    (g_BattleSummonTyphonRingConfig1.ringValue) = Gfx_ModelTableGetRenderObject(3, g_BattleSummonTyphonFallParam9);
    if (particle->step + particle->phase >= 0x5A) {
        elapsed = particle->step + particle->phase + -0x5A;
        if (elapsed >= 0xA) {
            particle->phase = 0xFFFF;
            return;
        }
        ratio = (elapsed << 0xC) / 0xA;
        g_BattleSummonTyphonRingConfig1.blend = ratio;
        for (i = 0; i < 3; i++) {
            ratio = SinFixed(ratio / 4);
        }
        g_BattleSummonTyphonDrawHandle->scale = particle->scale * ((ratio << 0xC >> 0xC) + 0x1000) >> 0xC;
        g_BattleSummonTyphonDrawHandle->scalePrev = g_BattleSummonTyphonDrawHandle->scale;
        g_BattleSummonTyphonDrawHandle->scaleY = 0x400;
    } else {
        elapsed = particle->step;
        if (elapsed < 0xA) {
            ratio = (elapsed << 0xC) / 0xA;
            g_BattleSummonTyphonRingConfig1.blend = 0x1000 - ratio;
        } else {
            elapsed -= 0xA;
            if (elapsed < 0xA) {
                g_BattleSummonTyphonRingConfig1.blend = 0;
            } else {
                elapsed -= 0xA;
                if (elapsed < 0xA) {
                    ratio = (elapsed << 0xC) / 0xA;
                    g_BattleSummonTyphonRingConfig1.blend = ratio;
                } else {
                    particle->phase = 0xFFFF;
                    return;
                }
            }
        }
        g_BattleSummonTyphonDrawHandle->scale = particle->scale;
        g_BattleSummonTyphonDrawHandle->scalePrev = g_BattleSummonTyphonDrawHandle->scale;
        g_BattleSummonTyphonDrawHandle->scaleY = 0x400;
    }
    RotMatrix((SVECTOR *)&particle->angleX, &g_BattleSummonTyphonDrawHandle->matrix);
    g_BattleSummonTyphonDrawHandle->transX = particle->x;
    g_BattleSummonTyphonDrawHandle->transY = particle->y;
    g_BattleSummonTyphonDrawHandle->transZ = particle->z;
    ScaleMatrix(&g_BattleSummonTyphonDrawHandle->matrix, &g_BattleSummonTyphonDrawHandle->scalePrev);
    MultiplyMatrix(&g_BattleSummonTyphonViewMatrix, &g_BattleSummonTyphonDrawHandle->matrix, &g_BattleSummonTyphonDrawHandle->matrix);
    SetRotMatrix(&g_BattleSummonTyphonDrawHandle->matrix);
    SetTransMatrix(&g_BattleSummonTyphonDrawHandle->matrix);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonTyphonRingConfig1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (particle->step)++;
        particle->angle += 0x40;
    }
}

/* Updates paired orbiting models around summon center. */
// FUNCTION: FF7 0x004d7daa
void __cdecl BattleFx_SummonTyphonPairedModelTask(void)
{
    int size;
    int localFrame;
    BattleTaskSlot *task;
    int ratio;

    task = BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSummonTyphonRingConfig2.fallParam) = Gfx_ModelTableGetRenderObject(0, g_BattleSummonTyphonFallParam9);
    localFrame = task->frameCounter;
    if (localFrame < 3) {
        ratio = (localFrame << 0xC) / 3;
        size = ratio << 0xB >> 0xC;
        g_BattleSummonTyphonRingConfig2.blend = 0;
    } else {
        localFrame -= 3;
        if (localFrame < 0xC) {
            ratio = (localFrame << 0xC) / 0xC;
            size = 0x1000 - ratio << 0xB >> 0xC;
            g_BattleSummonTyphonRingConfig2.blend = ratio;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    SetDrawColorRGB(0, 0, 0);
    BattleFx_SummonTyphonInstallScaledPointMatrix((BattleFxSourcePoint *)((int)task + 4), (short)size, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonTyphonRingConfig2, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_BattleOverlayDrawListHead);
    BattleFx_SummonTyphonInstallScaledPointMatrix((BattleFxSourcePoint *)((int)task + 0xC), (short)size, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonTyphonRingConfig2, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
    }
}

/* Computes and installs scaled billboard matrix for summon vertex. */
// FUNCTION: FF7 0x004d7f04
int __cdecl BattleFx_SummonTyphonInstallScaledPointMatrix(BattleFxSourcePoint *point, short value, int distance)
{
    int depth;
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int normal[3];

    *(short *)&g_BattleSummonTyphonFallParam15 = value;
    *(short *)&g_BattleSummonTyphonFallParam14 = *(short *)&g_BattleSummonTyphonFallParam15;
    *(short *)&g_BattleSummonTyphonFallParam13 = *(short *)&g_BattleSummonTyphonFallParam14;
    SetRotMatrix(&g_BattleSummonTyphonViewMatrix);
    SetTransMatrix(&g_BattleSummonTyphonViewMatrix);
    RotTransPoint((void *)point, (int *)((char *)&g_BattleSummonTyphonFallParam15 + 0x2), &depth);
    if (distance != 0) {
        NormalizeVectorFixed((int *)((char *)&g_BattleSummonTyphonFallParam15 + 0x2), normal);
        *(int *)((char *)&g_BattleSummonTyphonFallParam15 + 0x2) += normal[0] * distance >> 0xC;
        *(int *)((char *)&g_BattleSummonTyphonFallParam15 + 0x6) += normal[1] * distance >> 0xC;
        *(int *)((char *)&g_BattleSummonTyphonFallParam15 + 0xa) += normal[2] * distance >> 0xC;
    }
    SetRotMatrix(&g_BattleSummonTyphonFallParam13);
    SetTransMatrix(&g_BattleSummonTyphonFallParam13);
    return (int)&g_BattleSummonTyphonFallParam13;
}

/* Spawns orbiting cloud particles around Typhon body. */
// FUNCTION: FF7 0x004d7fdf
void __cdecl BattleFx_SummonTyphonOrbitSpawnTask(void)
{
    struct {
        int scaleVal, randRadius;
        BattleFxTyphonOrbitTask *particle;
        int spawnIdx;
        BattleTaskSlot *spawner;
        int radius, angle;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.spawner = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonTyphonFlashFlag & 1) == 0) {
        g_BattleSummonTyphonFlashFlag |= 1;
        g_BattleSummonTyphonOrbitSpawnScratch = Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_BattleIsPaused != 0)
        return;
    for (frame.spawnIdx = 0; frame.spawnIdx < 2; frame.spawnIdx++) {
        frame.particle = (BattleFxTyphonOrbitTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonTyphonOrbitParticleTask);
        frame.particle->origin = g_BattleSummonTyphonInitialAngles.pair;
        frame.angle = rand() & 0xFFF;
        if (DEAD_IN_RETAIL != 0)
            frame.randRadius = 0x7D0;
        else
            frame.randRadius = rand() % 0x7D0;
        frame.radius = frame.randRadius;
        frame.particle->rateX = (frame.radius * SinFixed(frame.angle) >> 12) - g_BattleSummonTyphonInitialAngles.x;
        frame.particle->rateY = -g_BattleSummonTyphonInitialAngles.y;
        frame.particle->rateZ = (frame.radius * CosFixed(frame.angle) >> 12) + 0x1B58 - g_BattleSummonTyphonInitialAngles.z;
        if (DEAD_IN_RETAIL != 0)
            frame.scaleVal = 0x3000;
        else
            frame.scaleVal = rand() % 0x1000 + 0x2000;
        frame.particle->scale = (short)frame.scaleVal;
    }
    frame.spawner->frameCounter += 1;
    if (frame.spawner->frameCounter >= 0x28)
        frame.spawner->ownerSlot = BATTLE_TASK_FREE;
}

/* Updates orbital trajectory of individual cloud particle. */
// FUNCTION: FF7 0x004d8177
void __cdecl BattleFx_SummonTyphonOrbitParticleTask(void)
{
    int age;
    BattleFxTyphonOrbitTask *task;
    int tableIdx;
    int ratio;

    task = (BattleFxTyphonOrbitTask *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonTyphonRingPhase & 1) == 0) {
        g_BattleSummonTyphonRingPhase |= 1;
        g_BattleSummonTyphonOrbitPoint = (Vec3sPacked *)Gfx_GetPsxScratchpadPtr(0);
    }
    age = task->frameCount;
    if (age < 0x10) {
        ratio = (age << 0xC) / 16;
        g_BattleSummonTyphonOrbitPoint->vx = task->originX + (task->rateX * ratio >> 0xC);
        g_BattleSummonTyphonOrbitPoint->vy = task->originY + (task->rateY * ratio >> 0xC);
        g_BattleSummonTyphonOrbitPoint->vz = task->originZ + (task->rateZ * ratio >> 0xC);
        g_BattleSummonTyphonOrbitPoint->vy += (short)((task->rateZ * SinFixed(ratio / 2) >> 0xC) * 0x280 >> 0xC);
        g_BattleSummonTyphonOrbitPoint->vz -= (short)((task->rateY * SinFixed(ratio / 2) >> 0xC) * 0x280 >> 0xC);
        tableIdx = ratio << 4 >> 0xC;
    } else {
        task->ownerSlot = BATTLE_TASK_FREE;
        return;
    }
    BattleFx_SummonTyphonInstallScaledPointMatrix((BattleFxSourcePoint *)g_BattleSummonTyphonOrbitPoint, (short)task->scale, 0);
    (g_BattleSummonTyphonParticleParams0.orbitValue) = Gfx_ModelTableGetLoadedRecord(g_BattleSummonTyphonRingVertexOffsets[tableIdx], g_BattleSummonTyphonFallParam9);
    g_BattleSummonTyphonParticleParams0.orbitLookup = Gfx_ModelTableGetSpriteObject(g_BattleSummonTyphonRingVertexOffsets[tableIdx], g_BattleSummonTyphonFallParam9);
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSummonTyphonParticleParams0, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frameCount)++;
    }
}

/* Updates geometry and radius of deforming windstorm rings. */
// FUNCTION: FF7 0x004d8376
void __cdecl BattleFx_SummonTyphonDeformingRingsTask(void)
{
    int ringBytes;
    int localFrame;
    BattleTaskSlot *pTaskSlot;
    int easing;
    int loopIdx;
    int calcRatio;
    int tiltParam;
    int fade;
    int ringW;
    int ringHeight;

    pTaskSlot = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonTyphonRingTaskInitFlag & 1) == 0) {
        g_BattleSummonTyphonRingTaskInitFlag |= 1;
        g_BattleSummonTyphonRingTaskScratch = Gfx_GetPsxScratchpadPtr(0);
    }
    if (pTaskSlot->frameCounter == 0 && g_BattleIsPaused == 0) {
        BattleFx_SummonTyphonInitRingPackets();
    }
    if (pTaskSlot->frameCounter < 0xA) {
        fade = (pTaskSlot->frameCounter << 7) / 0xA;
    } else {
        fade = 0x80;
    }
    if (g_BattleIsPaused == 0) {
        localFrame = pTaskSlot->frameCounter;
        if (localFrame < 0x32) {
            calcRatio = (localFrame << 0xC) / 0x32;
            ringHeight = (calcRatio * 0x4A38 >> 0xC) + 0x3E8;
            ringBytes = (calcRatio * 0xBB8 >> 0xC) + 0x3E8;
            ringW = (calcRatio * 0x384 >> 0xC) + 0x64;
            easing = 0x1000 - calcRatio;
            for (loopIdx = 0; loopIdx < 1; loopIdx++) {
                easing = SinFixed(easing / 4);
            }
            easing = 0x1000 - easing;
            calcRatio = easing << 0xC >> 0xC;
            tiltParam = (calcRatio * -0x800 >> 0xC) + 0x800;
        } else {
            localFrame -= 0x32;
            if (localFrame < 0x78) {
                ringHeight = 0x4E20;
                ringBytes = 0xFA0;
                ringW = 0x3E8;
                tiltParam = 0;
            } else {
                localFrame -= 0x78;
                if (localFrame < 0xF) {
                    calcRatio = (localFrame << 0xC) / 0xF;
                    fade = (calcRatio * -0x80 >> 0xC) + 0x80;
                    ringHeight = 0x4E20;
                    ringBytes = 0xFA0;
                    ringW = 0x3E8;
                    tiltParam = 0;
                } else {
                    pTaskSlot->ownerSlot = BATTLE_TASK_FREE;
                    return;
                }
            }
        }
        BattleFx_SummonTyphonUpdateRingRecords(ringHeight, ringBytes, ringW, tiltParam);
    }
    BattleFx_SummonTyphonDrawRingQuads(pTaskSlot->frameCounter << 7, fade);
    if (g_BattleIsPaused == 0) {
        (pTaskSlot->frameCounter)++;
    }
}

/* Initializes packet headers and texture coordinates for wind rings. */
// FUNCTION: FF7 0x004d85b6
void __cdecl BattleFx_SummonTyphonInitRingPackets(void)
{
    unsigned char vBottom;
    int band;
    unsigned char textureBase;
    int temp;
    int pass;
    int seg;
    unsigned char u1;
    unsigned char uLeft;
    unsigned short clut;
    BattleFxTyphonRingPacket *packet;
    int cell;
    unsigned char vHigh;

    for (band = 0; band < 0x10; band++) {
        if (band < 8) {
            cell = band;
            textureBase = 0;
            clut = 0x3FB0;
        } else {
            cell = band - 8;
            textureBase = 0;
            clut = 0x3F70;
        }
        temp = (cell << 0xC) / 8;
        vBottom = 0x7F - (cell << 4);
        temp = ((cell + 1) << 0xC) / 8;
        vHigh = 0x80 - (cell + 1 << 4);
        for (seg = 0; seg < 0x10; seg++) {
            cell = seg % 8;
            if (cell < 4) {
                uLeft = (cell << 4) + textureBase;
                u1 = uLeft + 0xF;
            } else {
                cell = 8 - cell;
                uLeft = (cell << 4) + textureBase + -1;
                u1 = uLeft - 0xF;
            }
            for (pass = 0; pass < 2; pass++) {
                packet = (BattleFxTyphonRingPacket *)((char *)&g_BattleSummonTyphonRingPad + band * 0x500 + seg * 0x50 + pass * 0x28);
                packet->tag = 0x9000000;
                packet->colorCommand = 0x2E808080;
                packet->u2 = uLeft;
                packet->u0 = uLeft;
                packet->u3 = u1;
                packet->u1 = u1;
                packet->v1 = vHigh;
                packet->v0 = vHigh;
                packet->v3 = vBottom;
                packet->v2 = vBottom;
                packet->clut = clut;
                packet->texPage = 0xAF;
            }
        }
    }
    for (band = 0; band < 0x10; band++) {
        ((BattleSummonTyphonFallParam5 *)(g_BattleSummonTyphonFallParam5))[band].alpha = 0;
        ((BattleSummonTyphonFallParam5 *)(g_BattleSummonTyphonFallParam5))[band].ringPhase = band * 0x3E8 / 0xF;
        ((BattleSummonTyphonFallParam5 *)(g_BattleSummonTyphonFallParam5))[band].heightOffset = 0x1B58;
        ((BattleFxTyphonRingScale *)&g_BattleSummonTyphonFallParam7)[band].scale = 0x3E8;
    }
}

/* Emits transformed Gouraud quad primitives for windstorm rings. */
// FUNCTION: FF7 0x004d87f9
void __cdecl BattleFx_SummonTyphonDrawRingQuads(int phase, int shade)
{
    int idx;
    int scale;
    int segment;
    int color;
    int primitive;
    int bank;

    if ((g_BattleSummonTyphonRingFlag & 1) == 0) {
        g_BattleSummonTyphonRingFlag |= 1;
        g_BattleSummonTyphonRingState = (BattleFxTyphonRingState *)Gfx_GetPsxScratchpadPtr(0);
    }
    SetRotMatrix(&g_BattleSummonTyphonViewMatrix);
    SetTransMatrix(&g_BattleSummonTyphonViewMatrix);
    RefreshGlobalTransformState();
    color = shade | shade << 8 | shade << 0x10 | 0x2E000000;
    *(Int64Pair *)g_BattleSummonTyphonRingState = *(Int64Pair *)(&g_BattleSummonTyphonFallParam5);
    scale = ((BattleFxTyphonRingScale *)&g_BattleSummonTyphonFallParam7)[0].scale;
    bank = 0;
    for (idx = 0; idx < 0x10; idx++) {
        g_BattleSummonTyphonRingState->vertices[bank][idx].vx = g_BattleSummonTyphonRingState->centerX + (scale * SinFixed(((idx << 0xC) / 0x10) + phase) >> 0xC);
        g_BattleSummonTyphonRingState->vertices[bank][idx].vy = g_BattleSummonTyphonRingState->centerY;
        g_BattleSummonTyphonRingState->vertices[bank][idx].vz = g_BattleSummonTyphonRingState->centerZ + (scale * CosFixed(((idx << 0xC) / 0x10) + phase) >> 0xC);
    }
    for (idx = 1; idx < 0x10; idx++) {
        *(Int64Pair *)g_BattleSummonTyphonRingState = *(Int64Pair *)&((BattleSummonTyphonFallParam5 *)(g_BattleSummonTyphonFallParam5))[idx].alpha;
        scale = ((BattleFxTyphonRingScale *)&g_BattleSummonTyphonFallParam7)[idx].scale;
        bank ^= 1;
        phase -= 0x40;
        for (segment = 0; segment < 0x10; segment++) {
            g_BattleSummonTyphonRingState->vertices[bank][segment].vx = g_BattleSummonTyphonRingState->centerX + (scale * SinFixed(((segment << 0xC) / 0x10) + phase) >> 0xC);
            g_BattleSummonTyphonRingState->vertices[bank][segment].vy = g_BattleSummonTyphonRingState->centerY;
            g_BattleSummonTyphonRingState->vertices[bank][segment].vz = g_BattleSummonTyphonRingState->centerZ + (scale * CosFixed(((segment << 0xC) / 0x10) + phase) >> 0xC);
        }
        for (segment = 0; segment < 0x10; segment++) {
            primitive = (int)&g_BattleSummonTyphonRingPad + (idx - 1) * 0x500 + segment * 0x50 + g_BattleSummonTyphonLoadedResTable2 * 0x28;
            g_BattleSummonTyphonRingState->depth = Gfx_TransformQuadReturnDepth(&g_BattleSummonTyphonRingState->vertices[bank][segment], &g_BattleSummonTyphonRingState->vertices[bank][(segment + 1) % 0x10], &g_BattleSummonTyphonRingState->vertices[bank ^ 1][segment], &g_BattleSummonTyphonRingState->vertices[bank ^ 1][(segment + 1) % 0x10], &g_BattleSummonTyphonRingState->projectScratch, &g_BattleSummonTyphonRingState->flags);
            if ((g_BattleSummonTyphonRingState->flags & 0x60000) == 0) {
                *(int *)((char *)primitive + 4) = color;
                if (idx < 8) {
                    Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(0x17, g_BattleSummonTyphonFallParam9)));
                    Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(0x18, g_BattleSummonTyphonFallParam9)));
                    Gfx_DrawPsxPrimitive((void *)primitive);
                } else {
                    Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(0x15, g_BattleSummonTyphonFallParam9)));
                    Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(0x16, g_BattleSummonTyphonFallParam9)));
                    Gfx_DrawPsxPrimitive((void *)primitive);
                }
            }
        }
    }
    g_BattleSummonTyphonLoadedResTable2 ^= 1;
}

/* Updates vertex coordinates across windstorm ring deformation rings. */
// FUNCTION: FF7 0x004d8c49
void __cdecl BattleFx_SummonTyphonUpdateRingRecords(int angle, int radius, int scale, int phaseOffset)
{
    int value;
    int ring;
    int i;
    int ratio;

    if ((g_BattleSummonTyphonFallParam4 & 1) == 0) {
        g_BattleSummonTyphonFallParam4 |= 1;
        g_BattleSummonTyphonRingScratch = (BattleSummonTyphonRingScratch *)Gfx_GetPsxScratchpadPtr(0);
    }
    for (ring = 0xF; ring >= 0; ring--) {
        ratio = (ring << 0xC) / 0xF;
        if (ring > 0) {
            ((BattleFxTyphonRingRecord *)&g_BattleSummonTyphonFallParam6.angleX)[ring].angle = ((BattleFxTyphonRingRecord *)&g_BattleSummonTyphonFallParam6.angleX)[ring - 1].angle + (rand() % 0x20) + -0x10;
            ((BattleFxTyphonRingRecord *)&g_BattleSummonTyphonFallParam6.angleY)[ring].angle = ((BattleFxTyphonRingRecord *)&g_BattleSummonTyphonFallParam6.angleY)[ring - 1].angle + (rand() % 0x20) + -0x10;
            ((BattleFxTyphonRingRecord *)&g_BattleSummonTyphonFallParam6.angleZ)[ring].angle = ((BattleFxTyphonRingRecord *)&g_BattleSummonTyphonFallParam6.angleZ)[ring - 1].angle + (rand() % 0x20) + -0x10;
        } else {
            ((BattleFxTyphonRingRecord *)&g_BattleSummonTyphonFallParam6.angleX)[ring].angle = g_BattleSummonTyphonOriginCoords.tiltX;
            ((BattleFxTyphonRingRecord *)&g_BattleSummonTyphonFallParam6.angleY)[ring].angle = g_BattleSummonTyphonOriginCoords.tiltY;
            ((BattleFxTyphonRingRecord *)&g_BattleSummonTyphonFallParam6.angleZ)[ring].angle = g_BattleSummonTyphonOriginCoords.tiltZ;
            g_BattleSummonTyphonOriginCoords.phaseX += (short)(rand() % 0x100 - 0x80);
            g_BattleSummonTyphonOriginCoords.phaseY += (short)(rand() % 0x100 - 0x80);
            g_BattleSummonTyphonOriginCoords.phaseZ += (short)(rand() % 0x100 - 0x80);
            g_BattleSummonTyphonOriginCoords.tiltX += (short)(SinFixed(g_BattleSummonTyphonOriginCoords.phaseX) << 9 >> 0xC);
            g_BattleSummonTyphonOriginCoords.tiltY += (short)(SinFixed(g_BattleSummonTyphonOriginCoords.phaseY) << 9 >> 0xC);
            g_BattleSummonTyphonOriginCoords.tiltZ += (short)(SinFixed(g_BattleSummonTyphonOriginCoords.phaseZ) << 9 >> 0xC);
        }
        g_BattleSummonTyphonRingScratch->v.vx = SinFixed(((BattleFxTyphonRingRecord *)&g_BattleSummonTyphonFallParam6.angleX)[ring].angle) << 9 >> 0xC;
        g_BattleSummonTyphonRingScratch->v.vy = SinFixed(((BattleFxTyphonRingRecord *)&g_BattleSummonTyphonFallParam6.angleY)[ring].angle) << 9 >> 0xC;
        g_BattleSummonTyphonRingScratch->v.vz = SinFixed(((BattleFxTyphonRingRecord *)&g_BattleSummonTyphonFallParam6.angleZ)[ring].angle) << 9 >> 0xC;
        RotMatrix(&g_BattleSummonTyphonRingScratch->v, &g_BattleSummonTyphonRingScratch->matrix);
        g_BattleSummonTyphonRingScratch->v.vx = scale * ((ratio * 0xE00 >> 0xC) + 0x200) >> 0xC;
        g_BattleSummonTyphonRingScratch->v.vy = 0;
        g_BattleSummonTyphonRingScratch->v.vz = 0;
        TransformVectorToInt16(&g_BattleSummonTyphonRingScratch->matrix, &g_BattleSummonTyphonRingScratch->v, &g_BattleSummonTyphonRingScratch->v.vx);
        ((BattleSummonTyphonFallParam5 *)(g_BattleSummonTyphonFallParam5))[ring].alpha = g_BattleSummonTyphonRingScratch->v.vx;
        ((BattleSummonTyphonFallParam5 *)(g_BattleSummonTyphonFallParam5))[ring].ringPhase = -(angle * ratio >> 0xC) + (g_BattleSummonTyphonRingScratch->v.vy >> 4);
        ((BattleSummonTyphonFallParam5 *)(g_BattleSummonTyphonFallParam5))[ring].heightOffset = g_BattleSummonTyphonRingScratch->v.vz + 0x1B58;
        value = SinFixed((ratio << 0xB >> 0xC) + phaseOffset);
        for (i = 0; i < 1; i++) {
            value = SinFixed(value / 4);
        }
        ((BattleFxTyphonRingScale *)&g_BattleSummonTyphonFallParam7)[ring].scale = radius * (0x1000 - (value << 0xB >> 0xC)) >> 0xC;
    }
}

/* Prepares offscreen draw requests for stage background warping. */
// FUNCTION: FF7 0x004d902f
void __cdecl BattleFx_SummonTyphonPrepareStageOffscreenRequests(void)
{
    int m[8];

    if (g_BattleUiScalingMode != 2) {
        g_BattleSummonTyphonBlendConfig.viewportOriginX = *(short *)&g_BattleViewportOriginX;
        g_BattleSummonTyphonBlendConfig.viewportOriginY = *(short *)&g_BattleViewportOriginY;
        g_BattleSummonTyphonBlendConfig.blendLevel = 0x80;
        g_BattleSummonTyphonBlendConfig.prevBlendLevel = g_BattleSummonTyphonBlendConfig.blendLevel;
    } else {
        g_BattleSummonTyphonBlendConfig.viewportOriginX = *(short *)&g_BattleViewportOriginX;
        g_BattleSummonTyphonBlendConfig.viewportOriginY = *(short *)&g_BattleViewportOriginY;
        g_BattleSummonTyphonBlendConfig.blendLevel = 0x100;
        g_BattleSummonTyphonBlendConfig.prevBlendLevel = g_BattleSummonTyphonBlendConfig.blendLevel;
    }
    g_BattleSummonTyphonMatrixData1.matrix.m[2][2] = (short)(g_BattleSummonTyphonFallParam10.fallParam);
    g_BattleSummonTyphonMatrixData1.matrix.m[0][0] = g_BattleSummonTyphonMatrixData1.matrix.m[2][2];
    if (g_BattleSummonTyphonFallParam11.rotY != 0) {
        g_BattleSummonTyphonMatrixData1.matrix.t[2] = g_BattleSummonTyphonFallParam10.position;
    } else {
        g_BattleSummonTyphonMatrixData1.matrix.t[2] = -g_BattleSummonTyphonFallParam10.position;
    }
    MultiplyMatrix(&g_BattleSummonTyphonMatrixData0, &g_BattleSummonTyphonMatrixData1, m);
    BattleFx_SummonTyphonQueueStageOffscreenRequests(m, (BattleFxScreenRect *)&g_BattleSummonTyphonBlendConfig);
}

/* Queues background screen copy and displacement draw requests. */
// FUNCTION: FF7 0x004d910c
void __cdecl BattleFx_SummonTyphonQueueStageOffscreenRequests(void *matrix, BattleFxScreenRect *params)
{
    GfxOffscreenRequest *request;
    int partId;

    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    if (g_pBattle3dScene != 0 && g_pBattle3dScene->stageId != 0) {
        partId = g_pBattle3dScene->stageId;
        request = (GfxOffscreenRequest *)Battle3d_AcquireOffscreenRequest();
        if (request != 0) {
            request->kind = GFX_OFFSCREEN_REQUEST_MESH;
            request->mesh = (int)BattleStage_GetPartMesh(1, (Battle3dStageRecord *)partId);
            GtePsxMatrixToFloat4x4Transposed(matrix, (char *)request + 0x88);
            request->pageFillEnabled = 1;
            request->renderKind = GFX_DRAW_TEXTURED_QUAD_2D;
            request->blendMode = BLEND_ADDITIVE;
            request->blendMode = BLEND_NONE;
            request->viewportX = params->x;
            request->viewportY = params->y;
            request->viewportW = params->width;
            request->viewportH = params->height;
            request->pageFillStepX = 1;
            request->pageFillStepY = 1;
            g_BattleSummonTyphonPairMarker[0] = request->slotIndex;
        }
        request = (GfxOffscreenRequest *)Battle3d_AcquireOffscreenRequest();
        if (request != 0) {
            request->kind = GFX_OFFSCREEN_REQUEST_MESH;
            request->mesh = (int)BattleStage_GetPartMesh(1, (Battle3dStageRecord *)partId);
            GtePsxMatrixToFloat4x4Transposed(matrix, (char *)request + 0x88);
            request->pageFillEnabled = 1;
            request->renderKind = GFX_DRAW_TEXTURED_TRI_2D;
            request->blendMode = BLEND_ADDITIVE;
            request->blendMode = BLEND_NONE;
            request->viewportX = params->x;
            request->viewportY = params->y;
            request->viewportW = params->width;
            request->viewportH = params->height;
            request->pageFillStepX = 1;
            request->pageFillStepY = 1;
            g_BattleSummonTyphonEndMarker = request->slotIndex;
        }
    }
}

/* Spawns tumbling debris particle emitter task. */
// FUNCTION: FF7 0x004d9314
void __cdecl BattleFx_SummonTyphonParticleEmitterTask(void)
{
    int angle;
    BattleTaskSlot *pTaskSlot;
    unsigned char texU;
    int spawnIdx;
    int offsetZ;
    int offX;
    unsigned char texV;
    int hRoll;
    int fRoll;
    int rollDist;
    BattleFxTyphonEmittedParticle *pTumbler;
    int rad;

    pTaskSlot = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    for (spawnIdx = 0; spawnIdx < 2; spawnIdx++) {
        pTumbler = (BattleFxTyphonEmittedParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonTyphonEmittedTumblingModelTask);
        pTumbler->variant = rand() % 3;
        angle = rand() & 0xFFF;
        if (DEAD_IN_RETAIL != 0) {
            rollDist = 0x1388;
        } else {
            rollDist = rand() % 0xFA0 + 0x3E8;
        }
        rad = rollDist;
        offX = rad * SinFixed(angle) >> 0xC;
        offsetZ = rad * CosFixed(angle) >> 0xC;
        pTumbler->startX = offX;
        pTumbler->startY = 0;
        pTumbler->startZ = offsetZ + 0x1B58;
        if (DEAD_IN_RETAIL != 0) {
            fRoll = 0x258;
        } else {
            fRoll = rand() % 0xC8 + 0x190;
        }
        pTumbler->velocityY = -fRoll;
        pTumbler->colorWord = rand() & 0xFFFFFF;
        if (DEAD_IN_RETAIL != 0) {
            hRoll = 0x200;
        } else {
            hRoll = rand() % 0x180 + 0x80;
        }
        pTumbler->posZ = hRoll;
        texU = (offX + 0x1388) * 0x28 / 0x2710 + 0x14;
        texV = (offsetZ + 0x1388) * 0x28 / 0x2710 + 0x14;
        pTumbler->drawParam = texV << 8 | texU;
    }
    (pTaskSlot->frameCounter)++;
    if (pTaskSlot->frameCounter >= 0x32) {
        pTaskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Updates trajectory and rotation of emitted tumbling debris model. */
// FUNCTION: FF7 0x004d9502
void __cdecl BattleFx_SummonTyphonEmittedTumblingModelTask(void)
{
    BattleFxTyphonTumbleRecord *task;

    task = (BattleFxTyphonTumbleRecord *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonTyphonEmitterFlag & 1) == 0) {
        g_BattleSummonTyphonEmitterFlag |= 1;
        g_BattleSummonTyphonTumbleState = (BattleFxTyphonTumbleState *)Gfx_GetPsxScratchpadPtr(0);
    }
    g_BattleSummonTyphonTumbleState->angleX = task->angleX << 4;
    g_BattleSummonTyphonTumbleState->angleY = task->angleY << 4;
    g_BattleSummonTyphonTumbleState->angleZ = task->angleZ << 4;
    RotMatrix((SVECTOR *)g_BattleSummonTyphonTumbleState, &g_BattleSummonTyphonTumbleState->matrix);
    g_BattleSummonTyphonTumbleState->transX = task->posX;
    g_BattleSummonTyphonTumbleState->transY = task->posY;
    g_BattleSummonTyphonTumbleState->transZ = task->posZ;
    g_BattleSummonTyphonTumbleState->param = task->param;
    g_BattleSummonTyphonTumbleState->valuePrev = g_BattleSummonTyphonTumbleState->param;
    g_BattleSummonTyphonTumbleState->valueOld = g_BattleSummonTyphonTumbleState->valuePrev;
    ScaleMatrix(&g_BattleSummonTyphonTumbleState->matrix, &g_BattleSummonTyphonTumbleState->valueOld);
    MultiplyMatrix(&g_BattleSummonTyphonViewMatrix, &g_BattleSummonTyphonTumbleState->matrix, &g_BattleSummonTyphonTumbleState->matrix);
    BattleFx_SummonTyphonDrawIndexedModel(&g_BattleSummonTyphonTumbleState->matrix, task->modelIndex, (short)task->drawParam);
    if (g_BattleIsPaused == 0) {
        (task->timer)++;
        if (task->timer >= 0xF) {
            task->modelIndex = 0xFFFF;
        } else {
            task->posY += task->velY;
            (*(char *)&task->angleX)++;
            *(char *)&task->angleY += 0x10;
            *(char *)&task->angleZ += 0x20;
        }
    }
}

/* Renders indexed 3D mesh model with installed transform matrix. */
// FUNCTION: FF7 0x004d96d4
void __cdecl BattleFx_SummonTyphonDrawIndexedModel(void *matrix, int modelIndex, short param)
{
    (g_BattleSummonTyphonParticleParams1.modelPointer) = g_BattleSummonTyphonModelPointers[modelIndex];
    g_BattleSummonTyphonParticleParams1.paramWord = param;
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    RefreshGlobalTransformState();
    Gfx_SetPsxPrimQuadState(9, 4, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleSummonTyphonEndMarker));
    Gfx_SetPsxPrimQuadState(9, 1, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleSummonTyphonEndMarker));
    Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleSummonTyphonPairMarker[0]));
    Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleSummonTyphonPairMarker[0]));
    g_BattleOverlayDrawListHead = BattleFx_SummonTyphonDrawPsxMesh((BattleFxProceduralMesh *)&g_BattleSummonTyphonParticleParams1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
}

/* Transforms, clips, and renders PlayStation polygon mesh primitive list. */
// FUNCTION: FF7 0x004d97a4
int __cdecl BattleFx_SummonTyphonDrawPsxMesh(BattleFxProceduralMesh *psxMesh, int drawList, int layer, int listHead)
{
    int vertA;
    int vertB;
    int color[2];
    int numberEntries;
    int state;
    BattleFxPsxQuadPacket *primPointer;
    int index;
    BattleFxPsxTyphonTriPrim *primB;
    int padField; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int bytesOut;
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int vertC;
    int meshBlock;
    unsigned short gpuAttrs;
    BattleFxPsxQuadPacket *cPacket;
    int unsupportedFlags;
    BattleFxTyphonMeshCursor *src;
    BattleFxPsxTyphonTriPrim *renderBlock;

    meshBlock = psxMesh->meshData + 4;
    src = (BattleFxTyphonMeshCursor *)(meshBlock + *(int *)psxMesh->meshData);
    renderBlock = (BattleFxPsxTyphonTriPrim *)Gfx_GetPsxPacketScratchPtr();
    numberEntries = src->record.numberEntries;
    src = (BattleFxTyphonMeshCursor *)((char *)src + 2);
    gpuAttrs = src->record.numberEntries & 0x19F | psxMesh->attrBits;
    src = (BattleFxTyphonMeshCursor *)((char *)src + 2);
    vertA = meshBlock + src->record.numberEntries;
    vertB = meshBlock + src->record.vertexBOffset;
    vertC = meshBlock + src->record.vertexCOffset;
    for (index = 0; index < numberEntries; index++) {
        StoreDeferredDrawArgs((void *)vertA, (void *)vertB, (void *)vertC);
        ApplyDeferredDrawArgs();
        vertA = meshBlock + src->record.vertexAOffset;
        vertB = meshBlock + src->record.colorB;
        vertC = meshBlock + src->record.vertexOffsetAlt1;
        Gfx_PsxStoreClearFlag(&unsupportedFlags);
        if ((unsupportedFlags & 0x60000) == 0) {
            if ((psxMesh->flags & 0x20) != 0) {
                state = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&state);
            }
            if (state > 0) {
                renderBlock->tag = 0x7000000;
                Gfx_ReadTransformOrderingDepth2(&bytesOut);
                renderBlock->primWord = src->record.primWord;
                renderBlock->uvWord0 = src->record.uvWord0;
                renderBlock->uvWord1 = src->record.uvWord1;
                renderBlock->gpuAttrs = gpuAttrs;
                if (g_BattleUiScalingMode != 2) {
                    renderBlock->primWordLo >>= 1;
                    renderBlock->shade0 >>= 1;
                    renderBlock->uvWord0Lo >>= 1;
                    renderBlock->shade1 >>= 1;
                    renderBlock->uvWord1Lo >>= 1;
                    renderBlock->shade2 >>= 1;
                }
                if ((psxMesh->flags & 0x80) != 0) {
                    color[0] = src->record.colorA & 0xFF;
                    color[0] = color[0] << 0x10 | 0x24000000 | color[0] << 8 | color[0];
                    if (psxMesh->vertexShade != 0) {
                        Gfx_ShadePsxVertexColor((CVECTOR *)color, psxMesh->vertexShade, &renderBlock->colorVec);
                        renderBlock->primCode = 0x24;
                    } else {
                        renderBlock->color = color[0];
                    }
                } else {
                    renderBlock->color = color[0] << 0x10 | 0x24000000 | color[0] << 8 | color[0];
                }
                if ((psxMesh->flags & 8) != 0) {
                    renderBlock->primCode |= 2;
                }
                Gfx_DrawPsxPrimitive(renderBlock);
            }
        }
        src = (BattleFxTyphonMeshCursor *)((char *)src + 0x10);
    }
    numberEntries = src->record.numberEntries;
    src = (BattleFxTyphonMeshCursor *)((char *)src + 4);
    primB = (BattleFxPsxTyphonTriPrim *)renderBlock;
    vertA = meshBlock + src->record.numberEntries;
    vertB = meshBlock + src->record.vertexBOffset;
    vertC = meshBlock + src->record.vertexCOffset;
    for (index = 0; index < numberEntries; index++) {
        StoreDeferredDrawArgs((void *)vertA, (void *)vertB, (void *)vertC);
        ApplyDeferredDrawArgs();
        vertA = meshBlock + src->record.vertexOffsetAlt1;
        vertB = meshBlock + src->record.vertexOffsetAlt2;
        vertC = meshBlock + src->record.vertexOffsetAlt3;
        Gfx_PsxStoreClearFlag(&unsupportedFlags);
        if ((unsupportedFlags & 0x60000) == 0) {
            if ((psxMesh->flags & 0x20) != 0) {
                state = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&state);
            }
            if (state >= 0) {
                SetPendingResourceHandle((GfxPsxShortVertexSlot *)(meshBlock + src->record.colorA));
                ApplyPendingResourceHandle();
                Gfx_ReadTransformOrderingDepth2(&bytesOut);
                primB->tag = 0x9000000;
                primB->primWord = src->record.primWord;
                primB->uvWord0 = src->record.uvWord0;
                primB->uvWord1 = src->record.uvWord1;
                primB->uvWord2 = src->record.vertexAOffset;
                primB->gpuAttrs = gpuAttrs;
                if (g_BattleUiScalingMode != 2) {
                    primB->primWordLo >>= 1;
                    primB->shade0 >>= 1;
                    primB->uvWord0Lo >>= 1;
                    primB->shade1 >>= 1;
                    primB->uvWord1Lo >>= 1;
                    primB->shade2 >>= 1;
                    primB->uvWord2Lo >>= 1;
                    primB->texV3 >>= 1;
                }
                if ((psxMesh->flags & 0x80) != 0) {
                    color[0] = src->record.colorB & 0xFF;
                    color[0] = color[0] << 0x10 | 0x2C000000 | color[0] << 8 | color[0];
                    if (psxMesh->vertexShade != 0) {
                        Gfx_ShadePsxVertexColor((CVECTOR *)color, psxMesh->vertexShade, &primB->colorVec);
                        primB->primCode = 0x2C;
                    } else {
                        primB->color = color[0];
                    }
                } else {
                    color[0] = psxMesh->vertexShade & 0xFF;
                    primB->color = color[0] << 0x10 | 0x2C000000 | color[0] << 8 | color[0];
                }
                if ((psxMesh->flags & 8) != 0) {
                    primB->primCode |= 2;
                }
                Gfx_DrawPsxPrimitive(primB);
            }
        }
        src = (BattleFxTyphonMeshCursor *)((char *)src + 0x14);
    }
    numberEntries = src->record.numberEntries;
    src = (BattleFxTyphonMeshCursor *)((char *)src + 4);
    cPacket = (BattleFxPsxQuadPacket *)primB;
    vertA = meshBlock + src->record.numberEntries;
    vertB = meshBlock + src->record.vertexBOffset;
    vertC = meshBlock + src->record.vertexCOffset;
    for (index = 0; index < numberEntries; index++) {
        StoreDeferredDrawArgs((void *)vertA, (void *)vertB, (void *)vertC);
        ApplyDeferredDrawArgs();
        vertA = meshBlock + src->record.vertexOffsetAlt1;
        vertB = meshBlock + src->record.vertexOffsetAlt2;
        vertC = meshBlock + src->record.vertexOffsetAlt3;
        Gfx_PsxStoreClearFlag(&unsupportedFlags);
        if ((unsupportedFlags & 0x60000) == 0) {
            if ((psxMesh->flags & 0x20) != 0) {
                state = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&state);
            }
            if (state >= 0) {
                cPacket->tag = 0x6000000;
                Gfx_ReadTransformOrderingDepth2(&bytesOut);
                if (psxMesh->vertexShade != 0) {
                    Gfx_ShadePsxTriVertexColors(&src->record.color0Vec, &src->ifritRecord.color1Vec, &src->ifritRecord.color2Vec, psxMesh->vertexShade, &cPacket->colorVec, &cPacket->color1Vec, &cPacket->color2Vec);
                } else {
                    cPacket->primWord = src->record.primWord;
                    cPacket->color1 = src->ifritRecord.color1;
                    cPacket->color2 = src->ifritRecord.color2;
                }
                if ((psxMesh->flags & 8) != 0) {
                    cPacket->primCode |= 2;
                }
                Gfx_DrawPsxPrimitive(cPacket);
            }
        }
        src = (BattleFxTyphonMeshCursor *)((char *)src + 0x14);
    }
    numberEntries = src->record.numberEntries;
    src = (BattleFxTyphonMeshCursor *)((char *)src + 4);
    primPointer = (BattleFxPsxQuadPacket *)cPacket;
    vertA = meshBlock + src->record.numberEntries;
    vertB = meshBlock + src->record.vertexBOffset;
    vertC = meshBlock + src->record.vertexCOffset;
    for (index = 0; index < numberEntries; index++) {
        StoreDeferredDrawArgs((void *)vertA, (void *)vertB, (void *)vertC);
        ApplyDeferredDrawArgs();
        vertA = meshBlock + src->record.vertexOffsetAlt3;
        vertB = meshBlock + src->record.vertexOffsetAlt4;
        vertC = meshBlock + src->record.vertexOffsetAlt5;
        Gfx_PsxStoreClearFlag(&unsupportedFlags);
        if ((unsupportedFlags & 0x60000) == 0) {
            if ((psxMesh->flags & 0x20) != 0) {
                state = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&state);
            }
            if (state >= 0) {
                SetPendingResourceHandle((GfxPsxShortVertexSlot *)(meshBlock + src->record.colorA));
                ApplyPendingResourceHandle();
                Gfx_ReadTransformOrderingDepth2(&bytesOut);
                primPointer->tag = 0x8000000;
                if (psxMesh->vertexShade != 0) {
                    Gfx_ShadePsxTriVertexColors(&src->record.color0Vec, &src->ifritRecord.color1Vec, &src->ifritRecord.color2Vec, psxMesh->vertexShade, &primPointer->colorVec, &primPointer->color1Vec, &primPointer->color2Vec);
                    Gfx_ShadePsxVertexColor(&src->ifritRecord.color3Vec, psxMesh->vertexShade, &primPointer->color3Vec);
                } else {
                    primPointer->primWord = src->record.primWord;
                    primPointer->color1 = src->ifritRecord.color1;
                    primPointer->color2 = src->ifritRecord.color2;
                    primPointer->color3 = src->ifritRecord.color3;
                }
                if ((psxMesh->flags & 8) != 0) {
                    primPointer->primCode |= 2;
                }
                Gfx_DrawPsxPrimitive(primPointer);
            }
        }
        src = (BattleFxTyphonMeshCursor *)((char *)src + 0x18);
    }
    return (int)primPointer;
}

/* Updates rotation and render state of main Typhon body mesh. */
// FUNCTION: FF7 0x004da086
void __cdecl BattleFx_SummonTyphonRotatingModelTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if ((*(unsigned char *)&g_BattleSummonTyphonRingStep & 1) == 0) {
        *(unsigned char *)&g_BattleSummonTyphonRingStep |= 1;
        g_BattleSummonTyphonModelMatrixScratch = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    }
    g_BattleSummonTyphonOffsetConfig.modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleSummonTyphonFallParam9);
    MultiplyMatrix(&g_BattleSummonTyphonViewMatrix, &g_BattleSummonTyphonMatrixData2, g_BattleSummonTyphonModelMatrixScratch);
    SetRotMatrix(g_BattleSummonTyphonModelMatrixScratch);
    SetTransMatrix(g_BattleSummonTyphonModelMatrixScratch);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonTyphonOffsetConfig, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= 0x78) {
            task->ownerSlot = BATTLE_TASK_FREE;
        } else {
            RotateMatrixY(0x10, &g_BattleSummonTyphonMatrixData2);
        }
    }
}

/* Updates rotation matrices of cinematic camera orbiting Typhon. */
// FUNCTION: FF7 0x004da182
void __cdecl BattleFx_SummonTyphonCameraRotationTask(void)
{
    int age;
    int i;
    int amplitude;
    BattleTaskSlot *pSlot;
    int sineVal;
    int easedValue;
    int localShakeOffset;

    pSlot = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonTyphonRingDrawMode & 1) == 0) {
        g_BattleSummonTyphonRingDrawMode |= 1;
        g_BattleSummonTyphonRotationState = (BattleSummonTyphonRotationScratch *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_BattleIsPaused == 0) {
        if (pSlot->frameCounter == 0) {
            g_BattleSummonTyphonSavedPosXY.word = *(int *)&g_BattleActorRecords[3].posX;
            g_BattleSummonTyphonSavedPosZ.word = *(int *)&g_BattleActorRecords[3].posZ;
        }
        age = pSlot->frameCounter;
        if (age < 0xA) {
            easedValue = (pSlot->frameCounter + 1 << 0xC) / 0xA;
            sineVal = 0x1000 - easedValue;
            for (i = 0; i < 2; i++) {
                sineVal = SinFixed(sineVal / 4);
            }
            sineVal = 0x1000 - sineVal;
            easedValue = sineVal * 0xA00 + easedValue * 0x600 >> 0xC;
            g_BattleCameraTransform.rotX = easedValue << 0xB >> 0xC;
        } else {
            age -= 0xA;
            if (age < 0xA) {
                easedValue = 0x1000 - (age << 0xC) / 0xF - 5;
                amplitude = easedValue << 5 >> 0xC;
                if ((age & 1) != 0) {
                    localShakeOffset = amplitude;
                } else {
                    localShakeOffset = -amplitude;
                }
                g_BattleCameraTransform.rotX = localShakeOffset + 0x800;
            } else {
                age -= 0xA;
                if (age < 0x64) {
                    g_BattleCameraTransform.rotX = 0x800;
                } else {
                    g_BattleCameraTransform.rotX = 0;
                    pSlot->ownerSlot = BATTLE_TASK_FREE;
                }
            }
        }
    }
    if (g_BattleActorRecords[3].rootParentMatrix == (int)&g_BattleCameraTransform.matrix[0]) {
        g_BattleSummonTyphonRotationState->angles.vx = -g_BattleCameraTransform.rotX;
        g_BattleActorRecords[3].rotX = g_BattleSummonTyphonRotationState->angles.vx;
        g_BattleSummonTyphonRotationState->angles.vz = 0;
        g_BattleSummonTyphonRotationState->angles.vy = 0;
        RotMatrix(&g_BattleSummonTyphonRotationState->angles, &g_BattleSummonTyphonRotationState->matrix);
        SetRotMatrix(&g_BattleSummonTyphonRotationState->matrix);
        g_BattleSummonTyphonRotationState->angles.vx = g_BattleSummonTyphonSavedPosXY.x - g_BattleCameraFocalPoint.focalPointX;
        g_BattleSummonTyphonRotationState->angles.vy = g_BattleSummonTyphonSavedPosXY.y - g_BattleCameraFocalPoint.focalPointY;
        g_BattleSummonTyphonRotationState->angles.vz = g_BattleSummonTyphonSavedPosZ.z - g_BattleCameraFocusZ;
        RotateSVectorByInstalledMatrix(&g_BattleSummonTyphonSavedPosXY, &g_BattleSummonTyphonRotationState->offsetX);
        g_BattleActorRecords[3].posX = g_BattleCameraFocalPoint.focalPointX + g_BattleSummonTyphonRotationState->offsetX;
        g_BattleActorRecords[3].posY = g_BattleCameraFocalPoint.focalPointY + g_BattleSummonTyphonRotationState->offsetY;
        g_BattleActorRecords[3].posZ = g_BattleCameraFocusZ + g_BattleSummonTyphonRotationState->offsetZ;
    }
    RotMatrix((SVECTOR *)&g_BattleCameraTransform.rotX, &g_BattleSummonTyphonRotationState->matrix);
    SetRotMatrix(&g_BattleSummonTyphonRotationState->matrix);
    RotateSVectorByInstalledMatrix(&g_BattleCameraFocalPoint, &g_BattleSummonTyphonRotationState->offsetX);
    g_BattleCameraFocalPoint.focalPointX = g_BattleSummonTyphonRotationState->posX;
    g_BattleCameraFocalPoint.focalPointY = g_BattleSummonTyphonRotationState->posY;
    g_BattleCameraFocusZ = g_BattleSummonTyphonRotationState->posZ;
    RotateSVectorByInstalledMatrix(&g_BattleCameraPosition, &g_BattleSummonTyphonRotationState->offsetX);
    g_BattleCameraPosition.cameraPosX = g_BattleSummonTyphonRotationState->posX;
    g_BattleCameraPosition.cameraPosY = g_BattleSummonTyphonRotationState->posY;
    g_BattleCameraPosZ = g_BattleSummonTyphonRotationState->posZ;
    if (g_BattleIsPaused == 0) {
        (pSlot->frameCounter)++;
    }
}

/* Updates and renders animated hexagon quad particles. */
// FUNCTION: FF7 0x004da516
void __cdecl BattleFx_SummonTyphonHexagonQuadsTask(void)
{
    BattleTaskSlot *task;
    BattleFxTyphonRenderStateBlock *renderBlock;
    int quadIdx;

    task = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonTyphonFallParam8 & 1) == 0) {
        g_BattleSummonTyphonFallParam8 |= 1;
        g_BattleSummonTyphonQuadScratch = Gfx_GetPsxScratchpadPtr(0);
    }
    renderBlock = (BattleFxTyphonRenderStateBlock *)Gfx_GetPsxPacketScratchPtr();
    SetRotMatrix((int *)&g_BattleSummonTyphonViewMatrix);
    SetTransMatrix(&g_BattleSummonTyphonViewMatrix);
    RefreshGlobalTransformState();
    for (quadIdx = 0; quadIdx < g_BattleSummonTyphonTimelineCounter; quadIdx++) {
        Gfx_TransformQuadReturnDepthAlt(&g_BattleSummonTyphonRingVertexArray[quadIdx].v[1], &g_BattleSummonTyphonRingVertexArray[quadIdx].v[2], &g_BattleSummonTyphonRingVertexArray[quadIdx].v[0], &g_BattleSummonTyphonRingVertexArray[quadIdx].v[3], (int *)g_BattleSummonTyphonQuadScratch, (int *)(g_BattleSummonTyphonQuadScratch + 4));
        if ((*(int *)((char *)g_BattleSummonTyphonQuadScratch + 4) & 0x60000) == 0) {
            renderBlock->renderWord0 = 0x5000000;
            renderBlock->renderWord1 = 0x28000000;
            Gfx_DrawPsxPrimitive(renderBlock);
        }
        Gfx_TransformQuadReturnDepthAlt(&g_BattleSummonTyphonRingVertexArray[quadIdx].v[0], &g_BattleSummonTyphonRingVertexArray[quadIdx].v[3], &g_BattleSummonTyphonRingVertexArray[quadIdx].v[5], &g_BattleSummonTyphonRingVertexArray[quadIdx].v[4], (int *)g_BattleSummonTyphonQuadScratch, (int *)(g_BattleSummonTyphonQuadScratch + 4));
        if ((*(int *)((char *)g_BattleSummonTyphonQuadScratch + 4) & 0x60000) == 0) {
            renderBlock->renderWord0 = 0x5000000;
            renderBlock->renderWord1 = 0x28000000;
            Gfx_DrawPsxPrimitive(renderBlock);
        }
    }
    g_BattleOverlayDrawListHead = (int)renderBlock;
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= 0x2D) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/* Spawns hexagon particle emitters attached to summon model. */
// FUNCTION: FF7 0x004da6df
void __cdecl BattleFx_SummonTyphonModelHexagonEmitterTask(void)
{
    int angle;
    BattleTaskSlot *pTaskSlot;
    int posZ;
    int offsetX;
    unsigned char paramHi;
    unsigned char uByte;
    int vertex;
    int i;
    BattleFxTyphonTaskRecord *pRecord;
    int spreadRand;
    int rollDist;
    int rad;

    pTaskSlot = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    for (i = 0; i < 5; i++) {
        pRecord = (BattleFxTyphonTaskRecord *)(BattleFx_SummonTyphonClaimTaskRecord((int)BattleFx_SummonTyphonBouncingModelRecordTask));
        pRecord->kind = rand() % 3;
        angle = rand() & 0xFFF;
        if (DEAD_IN_RETAIL != 0) {
            rollDist = 0x2710;
        } else {
            rollDist = rand() % 0x1B58 + 0xBB8;
        }
        rad = rollDist;
        offsetX = rad * SinFixed(angle) >> 0xC;
        posZ = rad * CosFixed(angle) >> 0xC;
        pRecord->posX = offsetX;
        pRecord->posY = 0;
        pRecord->posZ = posZ + 0x1B58;
        pRecord->velocityY = 0;
        pRecord->packedSpinAngles = rand() & 0xFFFFFF;
        if (DEAD_IN_RETAIL != 0) {
            spreadRand = 0x700;
        } else {
            spreadRand = rand() % 0x600 + 0x100;
        }
        pRecord->spread = spreadRand;
        uByte = (offsetX + 0x2710) * 0x50 / 0x4E20;
        paramHi = (posZ + 0x2710) * 0x50 / 0x4E20;
        pRecord->drawParam = paramHi << 8 | uByte;
        rad = pRecord->spread * 0x1388 >> 0xC;
        for (vertex = 0; vertex < 6; vertex++) {
            angle = vertex * 0x2AA + rand() % 0x200;
            g_BattleSummonTyphonRingVertexArray[g_BattleSummonTyphonTimelineCounter].v[vertex].vx = (rad * SinFixed(angle) >> 0xC) + offsetX;
            g_BattleSummonTyphonRingVertexArray[g_BattleSummonTyphonTimelineCounter].v[vertex].vy = 0;
            g_BattleSummonTyphonRingVertexArray[g_BattleSummonTyphonTimelineCounter].v[vertex].vz = (rad * CosFixed(angle) >> 0xC) + posZ + 0x1B58;
        }
        g_BattleSummonTyphonTimelineCounter++;
    }
    (pTaskSlot->frameCounter)++;
    if (pTaskSlot->frameCounter >= 0x32) {
        pTaskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Claims available task record slot from global particle pool. */
// FUNCTION: FF7 0x004da96d
int __cdecl BattleFx_SummonTyphonClaimTaskRecord(int taskFunc)
{
    int slot;

    for (slot = 0; slot < 0x12C; slot++) {
        if (g_BattleSummonTyphonScratchBuffer[slot] == 0) {
            g_BattleSummonTyphonScratchBuffer[slot] = (void (__cdecl *)(int))taskFunc;
            g_BattleSummonTyphonRingBuffer[slot].state = 0;
            g_BattleSummonTyphonRingBuffer[slot].stepCounter = 0;
            return (int)&g_BattleSummonTyphonRingBuffer[slot];
        }
    }
    return 0;
}

/* Updates bouncing particle model bouncing off battle stage ground. */
// FUNCTION: FF7 0x004da9d9
void __cdecl BattleFx_SummonTyphonBouncingModelRecordTask(BattleFxTyphonBouncingRecord *bouncer)
{
    BattleFxTyphonTaskRecord *spawnRecord;
    int spread;
    int bounceVelocity;

    if ((g_BattleSummonTyphonColorCycleIndex & 1) == 0) {
        g_BattleSummonTyphonColorCycleIndex |= 1;
        g_BattleSummonTyphonBouncingState = (BattleFxTyphonBouncingState *)Gfx_GetPsxScratchpadPtr(0);
    }
    g_BattleSummonTyphonBouncingState->spinX = bouncer->spinX << 4;
    g_BattleSummonTyphonBouncingState->spinY = bouncer->spinY << 4;
    g_BattleSummonTyphonBouncingState->spinZ = bouncer->spinZ << 4;
    RotMatrix((SVECTOR *)g_BattleSummonTyphonBouncingState, &g_BattleSummonTyphonBouncingState->matrix);
    g_BattleSummonTyphonBouncingState->transX = bouncer->posX;
    g_BattleSummonTyphonBouncingState->transY = bouncer->posY;
    g_BattleSummonTyphonBouncingState->transZ = bouncer->posZ;
    g_BattleSummonTyphonBouncingState->valueNew = bouncer[1].modelIndex;
    g_BattleSummonTyphonBouncingState->valuePrev = g_BattleSummonTyphonBouncingState->valueNew;
    g_BattleSummonTyphonBouncingState->valueOld = g_BattleSummonTyphonBouncingState->valuePrev;
    ScaleMatrix(&g_BattleSummonTyphonBouncingState->matrix, &g_BattleSummonTyphonBouncingState->valueOld);
    MultiplyMatrix(&g_BattleSummonTyphonViewMatrix, &g_BattleSummonTyphonBouncingState->matrix, &g_BattleSummonTyphonBouncingState->matrix);
    BattleFx_SummonTyphonDrawIndexedModel(&g_BattleSummonTyphonBouncingState->matrix, bouncer->modelIndex, (short)bouncer[1].callCount);
    if (g_BattleIsPaused == 0) {
        bouncer->posY += bouncer->velocityY;
        if (bouncer->posY <= -0x4E20) {
            spawnRecord = (BattleFxTyphonTaskRecord *)(BattleFx_SummonTyphonClaimTaskRecord((int)BattleFx_SummonTyphonScaledPointSpriteRecordTask));
            if (spawnRecord != 0) {
                spawnRecord->posX = bouncer->posX;
                spawnRecord->posY = 0xB1E0;
                spawnRecord->posZ = bouncer->posZ;
                if (DEAD_IN_RETAIL != 0) {
                    spread = 0x8000;
                } else {
                    spread = rand() % 0x5000 + 0x3000;
                }
                spawnRecord->spread = spread;
            }
            if ((bouncer->callCount)++ == 0) {
                bouncer->posY = 0xB1E0;
                if (DEAD_IN_RETAIL != 0) {
                    bounceVelocity = 0x12C;
                } else {
                    bounceVelocity = rand() % 0xC8 + 0x64;
                }
                bouncer->velocityY = bounceVelocity;
            } else {
                bouncer->modelIndex = 0xFFFF;
            }
        } else {
            bouncer->velocityY = (bouncer->velocityY * 0xF80 >> 0xC) - 0x32;
            (*(char *)&bouncer->spinX)++;
            *(char *)&bouncer->spinY += 8;
            *(char *)&bouncer->spinZ += 0x10;
        }
    }
}

/* Updates scaled point sprite particle trajectory and opacity. */
// FUNCTION: FF7 0x004dac67
void __cdecl BattleFx_SummonTyphonScaledPointSpriteRecordTask(BattleFxTyphonBillboardRecord *record)
{
    (g_BattleSummonTyphonParticleParams2.scaledEntry) = Gfx_ModelTableGetLoadedRecord(0x14, g_BattleSummonTyphonFallParam9);
    g_BattleSummonTyphonParticleParams2.scaledParam = Gfx_ModelTableGetSpriteObject(0x14, g_BattleSummonTyphonFallParam9);
    BattleFx_SummonTyphonInstallScaledPointMatrix((BattleFxSourcePoint *)((char *)record + 4), (short)record->scale, 0xFFFFFC18);
    g_BattleSummonTyphonParticleParams2.scaledSpriteFrame = record->frame;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSummonTyphonParticleParams2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0 && (g_BattleSummonTyphonParticleParams2.scaledEntry) != 0) {
        (record->frame)++;
        if (record->frame == ((BattleFxSpriteGroup *)g_BattleSummonTyphonParticleParams2.scaledEntry)->header->frameCount) {
            record->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/* Schedules target actor falling animation task for selected target. */
// FUNCTION: FF7 0x004dad26
void __cdecl BattleFx_SummonSpawnTyphonActorFallPerTarget(short actor)
{
    BattleFxTyphonActorFall *slot;

    slot = (BattleFxTyphonActorFall *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonTyphonActorFallTask);
    slot->actorIndex = actor;
    slot->fallVelocity = 0;
}

/* Updates target actor falling and tumbling downward after wind blast. */
// FUNCTION: FF7 0x004dad5c
void __cdecl BattleFx_SummonTyphonActorFallTask(void)
{
    int angle;
    BattleFxTyphonActorFall *fallTask;
    int i;
    BattleActorFallState *pActorFall;
    BattleFxTyphonTaskRecord *dust;
    int bounceSpeed;
    int sRoll;

    fallTask = (BattleFxTyphonActorFall *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    pActorFall = (BattleActorFallState *)&g_BattleActorRecords[fallTask->actorIndex].modelId;
    fallTask->fallVelocity -= 0x32;
    pActorFall->heightOffset += fallTask->fallVelocity;
    if (pActorFall->heightOffset - pActorFall->radius <= -0x4E20) {
        pActorFall->heightOffset = pActorFall->radius - 0x4E20;
        for (i = 0; i < 5; i++) {
            dust = (BattleFxTyphonTaskRecord *)(BattleFx_SummonTyphonClaimTaskRecord((int)BattleFx_SummonTyphonBillboardSpriteRecordTask));
            if (dust != 0) {
                angle = i * 0x333 + rand() % 0x333;
                dust->posX = pActorFall->baseX + (pActorFall->radius * SinFixed(angle) >> 0xC);
                dust->posY = 0xB1E0;
                dust->posZ = pActorFall->baseZ + (pActorFall->radius * CosFixed(angle) >> 0xC);
                if (DEAD_IN_RETAIL != 0) {
                    sRoll = 0x8000;
                } else {
                    sRoll = rand() % 0x5000 + 0x3000;
                }
                dust->spread = sRoll;
            }
        }
        if (fallTask->frameCount == 0) {
            if (DEAD_IN_RETAIL != 0) {
                bounceSpeed = 0x190;
            } else {
                bounceSpeed = rand() % 0xC8 + 0xC8;
            }
            fallTask->fallVelocity = bounceSpeed;
            (fallTask->frameCount)++;
        } else {
            Battle_PostActorHitOrHurtMessage(fallTask->actorIndex);
            fallTask->actorIndex = 0xFFFF;
        }
    }
}

/* Updates billboard sprite particle billboard orientation and scaling. */
// FUNCTION: FF7 0x004daf50
void __cdecl BattleFx_SummonTyphonBillboardSpriteRecordTask(BattleFxTyphonBillboardRecord *record)
{
    (g_BattleSummonTyphonParticleParams3.billboardEntry) = Gfx_ModelTableGetLoadedRecord(0x14, g_BattleSummonTyphonFallParam9);
    g_BattleSummonTyphonParticleParams3.billboardParam = Gfx_ModelTableGetSpriteObject(0x14, g_BattleSummonTyphonFallParam9);
    BattleFx_InstallBillboardMatrix((char *)record + 4, (short)record->scale, 0xFFFFFC18);
    g_BattleSummonTyphonParticleParams3.billboardSpriteFrame = record->frame;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSummonTyphonParticleParams3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0 && (g_BattleSummonTyphonParticleParams3.billboardEntry) != 0) {
        (record->frame)++;
        if (record->frame == ((BattleFxSpriteGroup *)g_BattleSummonTyphonParticleParams3.billboardEntry)->header->frameCount) {
            record->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/* Fades target actor color blend flags on conclusion of summon damage. */
// FUNCTION: FF7 0x004db00f
void __cdecl BattleFx_SummonTyphonActorFlagFadeTask(void)
{
    int age;
    BattleTaskSlot *task;
    int ratio;
    int index;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0) {
        (task->frameCounter)++;
    } else {
        age = task->frameCounter;
        if (age == 1) {
            g_BattleActorRecords[3].posY = 0xF448;
            g_BattleActorRecords[3].tintB = 0;
            g_BattleActorRecords[3].tintG = g_BattleActorRecords[3].tintB;
            g_BattleActorRecords[3].tintR = g_BattleActorRecords[3].tintG;
            g_BattleActorRecords[3].tintPhase = 0x1000;
            g_BattleActorRecords[3].scale = 0xC00;
            for (index = 0; index < g_BattleActorRecords[3].boneCount; index++) {
                (&g_BattleActorRecords[3].fadeFlags)[index] |= 8;
            }
        }
        if (age >= 5) {
            ratio = (age - 5 << 0xC) / 0xA;
            g_BattleActorRecords[3].tintPhase = 0x1000 - ratio;
        }
        (task->frameCounter)++;
        if (task->frameCounter >= 0xF) {
            for (index = 0; index < g_BattleActorRecords[3].boneCount; index++) {
                (&g_BattleActorRecords[3].fadeFlags)[index] &= 0xF7;
            }
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/* Drives white flash fade-in effect concluding summon sequence. */
// FUNCTION: FF7 0x004db15f
void __cdecl BattleFx_SummonTyphonWhiteFadeInTask(void)
{
    int age;
    BattleTaskSlot *task;
    int ratio;
    int quadIdx;

    task = BATTLE_CURRENT_TASK_SLOT;
    quadIdx = 0;
    g_BattleSummonTyphonFadeTile.x = *(short *)&g_BattleViewportOriginX;
    g_BattleSummonTyphonFadeTile.y = *(short *)&g_BattleViewportOriginY;
    g_BattleSummonTyphonFadeTile.width = (short)g_BattleViewportWidth;
    g_BattleSummonTyphonFadeTile.height = *(short *)&g_BattleSceneViewportHeight;
    age = task->frameCounter;
    if (age < 0x14) {
        ratio = (age << 0xC) / 0x14;
        SetDrawColorRGB(0xFF, 0xFF, 0xFF);
        Gfx_ShadePsxVertexColor(&g_BattleSummonTyphonFadeTile.baseColor, ratio, (CVECTOR *)((quadIdx << 4) + (char *)&g_BattleSummonTyphonFadeTile.color));
        Gfx_SetBlendFactorPair(FLOAT_0_9999_BITS, FLOAT_ONE_BITS);
        Gfx_DrawPsxPrimitive((quadIdx << 4) + (char *)&g_BattleSummonTyphonFadeTile);
    } else {
        g_BattleStageHideFlags |= 4;
        task->ownerSlot = BATTLE_TASK_FREE;
        return;
    }
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
    }
}

/* Pulses full-screen overlay color during summon climax. */
// FUNCTION: FF7 0x004db26e
void __cdecl BattleFx_SummonTyphonFadeOverlayPulseTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0) {
        BattleFx_SummonTyphonBeginFadeOverlay(4);
    } else if (task->frameCounter == 4) {
        BattleFx_SummonTyphonSetFadeOverlayFadeOutRate(4);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}
} /* extern "C" */
