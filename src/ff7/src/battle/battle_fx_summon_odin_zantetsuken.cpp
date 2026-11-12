// src/ff7/src/battle/battle_fx_summon_odin_zantetsuken.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Odin Zantetsuken summon effect: loads its resources, runs its camera script, and drives
//   a timeline. Its tasks draw PSX gouraud and textured meshes, spawn bone sprites, afterimages and
//   trail segments, split the actor into two quads, and toggle masked actor bits.

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
#include "lib/types/sound.h"
#include "lib/enums/graphics.h"
#include "ff7/enums/battle.h"
#include "lib/codegen.h"
#include "ff7/enums/main.h"
extern "C" {

#pragma intrinsic(memset)
// clang-format off
// clang-format on

// The scrolling sprite matrix is one PsxMatrix split over three globals: the first row's first
// two entries (g_BattleSummonOdinZScrollMatrix), padding, and this tail from m[1][1] through
// the translation's X.
#pragma pack(push, 1)
typedef struct {
    short rotation[5];   /* m[1][1] .. m[2][2] */
    int translationX;    /* t[0] */
    short translationYLow;
} BattleSummonOdinZMatrixTail;
#pragma pack(pop)

// The camera-relative matrices of the nine afterimages.
typedef struct {
    PsxMatrix matrices[9];
    short pad_10e;
} BattleSummonOdinZAfterimageMatrices;

// clang-format off
extern BattleSummonOdinZMatrixTail g_BattleSummonOdinZVectorC0;
extern BattleSummonOdinZAfterimageMatrices g_BattleSummonOdinZAfterimageMatrices; /* Defined later in this TU (retail .bss order). */
// clang-format on

// clang-format off
struct SoundGroup g_BattleFxSummonOdinZantetsukenSoundGroups[5] = {
    {{0x239, 0x23a, 0x23b, 0}},
    {{0x23c, 0x23d, 0x23e, 0}},
    {{0x247, 0x248, 0x249, 0}},
    {{0x1a3, 0, 0, 0}},
    {{0x24a, 0x24b, 0x24c, 0}}
};
struct BattleFxOdinZTaskSlot;
// GLOBAL: FF7 0x00803e98
struct SoundGroup *g_BattleSummonOdinZSoundGroup = (struct SoundGroup *)0x00803E48;
struct SoundGroup *g_BattleFxSummonOdinZantetsukenSoundGroups2[1] = {
    &g_BattleFxSummonOdinZantetsukenSoundGroups[1]
};
// clang-format on

// clang-format off
struct SoundGroup *g_BattleFxSummonOdinZantetsukenSoundGroups3[3] = {
    &g_BattleFxSummonOdinZantetsukenSoundGroups[2], &g_BattleFxSummonOdinZantetsukenSoundGroups[3], &g_BattleFxSummonOdinZantetsukenSoundGroups[4]
};
// GLOBAL: FF7 0x00803eb0
unsigned char g_BattleSummonOdinZMesh[932] = {0xd8, 0x00, 0x00, 0x00, 0xea, 0xff, 0x4c, 0xff, 0x77, 0xff, 0x00, 0x00, 0x64, 0x00, 0x4c, 0xff, 0x78, 0xff, 0x00, 0x00, 0x64, 0x00, 0xf5, 0xfe, 0xd7, 0xff, 0x00, 0x00, 0xea, 0xff, 0xf5, 0xfe, 0xd7, 0xff, 0x00, 0x00, 0xea, 0xff, 0xa2, 0xff, 0x19, 0xff, 0x00, 0x00, 0x64, 0x00, 0xa2, 0xff, 0x19, 0xff, 0x00, 0x00, 0xea, 0xff, 0x9d, 0xf9, 0x8f, 0xff, 0x00, 0x00, 0xea, 0xff, 0x0f, 0xfa, 0x72, 0xff, 0x00, 0x00, 0xea, 0xff, 0x11, 0xfa, 0x9f, 0xff, 0x00, 0x00, 0xea, 0xff, 0x53, 0xfc, 0x28, 0xff, 0x00, 0x00, 0xea, 0xff, 0x4d, 0xfc, 0x74, 0xff, 0x00, 0x00, 0x64, 0x00, 0x53, 0xfc, 0x28, 0xff, 0x00, 0x00, 0x64, 0x00, 0x4d, 0xfc, 0x74, 0xff, 0x00, 0x00, 0x64, 0x00, 0x9d, 0xf9, 0x8f, 0xff, 0x00, 0x00, 0x64, 0x00, 0x0f, 0xfa, 0x72, 0xff, 0x00, 0x00, 0x64, 0x00, 0x0d, 0xfa, 0x44, 0xff, 0x00, 0x00, 0x64, 0x00, 0x59, 0xfc, 0xdd, 0xfe, 0x00, 0x00, 0x27, 0x00, 0xd9, 0xff, 0xdf, 0xfe, 0x00, 0x00, 0x27, 0x00, 0xc5, 0xfd, 0xd8, 0xff, 0x00, 0x00, 0xea, 0xff, 0x0d, 0xfa, 0x44, 0xff, 0x00, 0x00, 0x27, 0x00, 0x6e, 0xf9, 0x88, 0xff, 0x00, 0x00, 0x27, 0x00, 0x12, 0xfa, 0x17, 0xff, 0x00, 0x00, 0x27, 0x00, 0x42, 0xfc, 0xa5, 0xfe, 0x00, 0x00, 0xea, 0xff, 0x59, 0xfc, 0xdd, 0xfe, 0x00, 0x00, 0x27, 0x00, 0x4e, 0xfc, 0x99, 0xff, 0x00, 0x00, 0x27, 0x00, 0x35, 0xfa, 0xb7, 0xff, 0x00, 0x00, 0x64, 0x00, 0x11, 0xfa, 0x9f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x30, 0x00, 0x38, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x40, 0x80, 0x00, 0x00, 0x10, 0x40, 0x00, 0x68, 0x00, 0x70, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x40, 0x80, 0x00, 0x00, 0x10, 0x40, 0x00, 0x28, 0x00, 0x20, 0x00, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x10, 0x00, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x80, 0x00, 0x30, 0x00, 0x98, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x10, 0x40, 0x00, 0x00, 0x40, 0x80, 0x00, 0x50, 0x00, 0x90, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0x30, 0x00, 0x20, 0x80, 0x00, 0x00, 0x80, 0xff, 0x00, 0x90, 0x00, 0x50, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x20, 0x80, 0x30, 0x00, 0x80, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x00, 0xa0, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc8, 0x00, 0xd0, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x20, 0x80, 0x30, 0x00, 0x10, 0x40, 0x00, 0x00, 0x10, 0x40, 0x00, 0x70, 0x00, 0x68, 0x00, 0xd0, 0x00, 0x00, 0x00, 0x00, 0x40, 0x80, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x40, 0x00, 0xc0, 0x00, 0x90, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0x30, 0x00, 0x20, 0x80, 0x00, 0x00, 0x80, 0xff, 0x00, 0x60, 0x00, 0x90, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0x30, 0x00, 0x20, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x18, 0x00, 0x10, 0x00, 0x00, 0x20, 0x80, 0x38, 0x00, 0x20, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x28, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x80, 0x00, 0x00, 0x20, 0x80, 0x00, 0x38, 0x00, 0x48, 0x00, 0x40, 0x00, 0x50, 0x00, 0x00, 0x40, 0x80, 0x38, 0xff, 0xff, 0xff, 0x00, 0x00, 0x10, 0x40, 0x00, 0x00, 0x80, 0xff, 0x00, 0x48, 0x00, 0x00, 0x00, 0x50, 0x00, 0x18, 0x00, 0xff, 0xff, 0xff, 0x38, 0x00, 0x20, 0x80, 0x00, 0x00, 0x80, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x58, 0x00, 0x10, 0x00, 0x60, 0x00, 0x00, 0x20, 0x80, 0x38, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0x00, 0x70, 0x00, 0x58, 0x00, 0x78, 0x00, 0x80, 0x00, 0x00, 0x40, 0x80, 0x38, 0xff, 0xff, 0xff, 0x00, 0x00, 0x10, 0x40, 0x00, 0x00, 0x80, 0xff, 0x00, 0x98, 0x00, 0x30, 0x00, 0xa8, 0x00, 0xa0, 0x00, 0x00, 0x10, 0x40, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb0, 0x00, 0xb8, 0x00, 0xa8, 0x00, 0x98, 0x00, 0x00, 0x20, 0x80, 0x38, 0x00, 0x80, 0xff, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x10, 0x40, 0x00, 0xb8, 0x00, 0x20, 0x00, 0x48, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0x38, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x20, 0x80, 0x00, 0x98, 0x00, 0xb8, 0x00, 0x38, 0x00, 0x48, 0x00, 0x00, 0x10, 0x40, 0x38, 0x00, 0x80, 0xff, 0x00, 0x00, 0x40, 0x80, 0x00, 0xff, 0xff, 0xff, 0x00, 0x50, 0x00, 0xc0, 0x00, 0x40, 0x00, 0xc8, 0x00, 0x00, 0x80, 0xff, 0x38, 0x00, 0x80, 0xff, 0x00, 0x00, 0x10, 0x40, 0x00, 0x00, 0x20, 0x80, 0x00, 0x88, 0x00, 0x20, 0x00, 0xb0, 0x00, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x80, 0x00, 0x00, 0x80, 0xff, 0x00, 0x68, 0x00, 0x30, 0x00, 0xd0, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x40, 0x00, 0x00, 0x10, 0x40, 0x00, 0xa8, 0x00, 0xa0, 0x00, 0x78, 0x00, 0x68, 0x00, 0x00, 0x00, 0x20, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0x00, 0x70, 0x00, 0x60, 0x00, 0xd0, 0x00, 0xff, 0xff, 0xff, 0x38, 0x00, 0x40, 0x80, 0x00, 0x00, 0x80, 0xff, 0x00, 0x00, 0x10, 0x40, 0x00, 0xb0, 0x00, 0xa8, 0x00, 0x80, 0x00, 0x78, 0x00, 0x00, 0x20, 0x80, 0x38, 0x00, 0x00, 0x20, 0x00, 0x00, 0x80, 0xff, 0x00, 0x00, 0x10, 0x40, 0x00, 0xc8, 0x00, 0xc0, 0x00, 0xd0, 0x00, 0x60, 0x00, 0x00, 0x20, 0x80, 0x38, 0x00, 0x80, 0xff, 0x00, 0x00, 0x10, 0x40, 0x00, 0x00, 0x80, 0xff, 0x00, 0x28, 0x00, 0x80, 0x00, 0x08, 0x00, 0x58, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x80, 0xff, 0x00, 0x00, 0x20, 0x80, 0x00, 0xff, 0xff, 0xff, 0x00, 0x88, 0x00, 0xb0, 0x00, 0x28, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x20, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0x00};
// GLOBAL: FF7 0x00804258
unsigned int g_BattleSummonOdinZResourceDef[25] = {0x00c800b6, (unsigned int)"iwa.rsd", 0x00000000, 0x00000000, 0x00000000, 0x00c82a39, (unsigned int)"sky_2.tim", (unsigned int)"sky_2.spt", 0x00000000, 0x00000000, 0x00c82a39, (unsigned int)"moon_1.tim", (unsigned int)"moon_1.spt", 0x00000000, 0x00000000, 0x00c82a39, (unsigned int)"kiri_2.tim", (unsigned int)"kiri_2.spt", 0x00000000, 0x00000000, 0x00c82a39, 0x00000000, (unsigned int)"smoke.spt", 0x00000000, 0x00000000};
// GLOBAL: FF7 0x008042c0
BattleFxEulerAngles16 g_BattleSummonOdinZInitialAngles = {0, -900, -2000};
// GLOBAL: FF7 0x008042c8
short g_BattleSummonOdinZSubeffectTable1[124] = {0x0088, 9, 0x0048, 9, 0x0081, 2400, -1752, -5000, 0x0041, 2400, -1752, -904, 0x0082, 75, 0, -1752, -5000, 0x0042, 75, 0, -1752, -904, 0x0082, 40, 0, -1500, -5000, 0x0042, 40, 0, 600, 3192, 0x0080, 20, 0x0040, 20, 0x0086, 3, -1, 0x0046, 3, -1, 0x0081, 0, -1000, 0, 0x0041, 0, 0, 5000, 0x0080, 25, 0x0040, 25, 0x0080, 35, 0x0043, 17, -7000, 3000, 3000, 0x0043, 18, -10000, -3000, 0, 0x0087, 0x0047, 0x0081, 0, -2000, 2000, 0x0041, 3000, -3500, -1500, 0x0080, 25, 0x0040, 25, 0x0081, 0, -2000, 3000, 0x0041, 1000, -7000, -4000, 0x0080, 15, 0x0040, 15, 0x0081, 0, -2000, 3000, 0x0041, 0, 0, 9000, 0x0080, 15, 0x0040, 15, 0x0088, -1, 0x0048, -1, 0x0086, 3, -1, 0x0081, 0, -1000, 0, 0x0041, -500, -2000, -5000, 0x0080, 15, 0x0040, 15, -1};
// GLOBAL: FF7 0x008043c0
short g_BattleSummonOdinZSubeffectTable2[124] = {0x0088, 9, 0x0048, 9, 0x0081, 2400, -1752, -5000, 0x0041, 2400, -1752, -904, 0x0082, 75, 0, -1752, -5000, 0x0042, 75, 0, -1752, -904, 0x0082, 40, 0, -1500, -5000, 0x0042, 40, 0, 600, 3192, 0x0080, 20, 0x0040, 20, 0x0086, 3, -1, 0x0046, 3, -1, 0x0081, 0, -1000, 0, 0x0041, 0, 0, 5000, 0x0080, 25, 0x0040, 25, 0x0080, 35, 0x0043, 17, -7000, 3000, 3000, 0x0043, 18, -10000, -3000, 0, 0x0087, 0x0047, 0x0081, 0, -2000, 2000, 0x0041, 3000, -3500, -1500, 0x0080, 25, 0x0040, 25, 0x0081, 0, -2000, 3000, 0x0041, 1000, -7000, -4000, 0x0080, 15, 0x0040, 15, 0x0081, 0, -2000, 3000, 0x0041, 0, 0, 9000, 0x0080, 15, 0x0040, 15, 0x0088, -1, 0x0048, -1, 0x0086, 3, -1, 0x0081, 0, -1000, 0, 0x0041, -500, -2000, -5000, 0x0080, 15, 0x0040, 15, -1};
// GLOBAL: FF7 0x008044b8
BattleSummonOdinZRefVector g_BattleSummonOdinZRefVector = {0, 0, 0x2e808080, {0}};
// GLOBAL: FF7 0x008044c8
int g_BattleSummonOdinZScale = 1024;
// GLOBAL: FF7 0x008044d0
BattleSummonOdinZSpriteDrawRequest g_BattleSummonOdinZSpriteDrawRequest = {0, 0, {0x80, 0x80, 0x80, 0x2c, 0, 0}};
// GLOBAL: FF7 0x008044e0
BattleFxModelDrawHead g_BattleSummonOdinZAnimParams1 = {0, BATTLE_FX_BZ_MORPH_FACE_GREY, 0, 0, BATTLE_FX_BZ_MORPH_DOUBLE_SIDED, 0};
// GLOBAL: FF7 0x008044f0
PsxMatrix g_BattleSummonOdinZMatrix1 = {{{0x200, 0, 0}, {0, 0x200, 0}, {0, 0, 0x200}}, {0, 0, -2000}};
// GLOBAL: FF7 0x00804510
BattleSummonOdinZVector2 g_BattleSummonOdinZVector2 = {0, 0, 0x2e404040, {0}};
// GLOBAL: FF7 0x00804520
int g_BattleSummonOdinZState = (-1);
// GLOBAL: FF7 0x00804528
BattleSummonOdinZTaskParam g_BattleSummonOdinZTaskParam = {{768, 0, 256, 256}, 0, 0};
// GLOBAL: FF7 0x00804538
struct BattleSummonOdinZColorData g_BattleSummonOdinZColorData = {{0x80, 0x80, 0x80, 0x2e}, 0, {0, 2}};
// GLOBAL: FF7 0x00804540
short g_BattleSummonOdinZRangeTable[2] = {22, 26};
unsigned char g_rawData_00804544[4] = {0x1e, 0x00, 0x22, 0x00};
unsigned char g_rawData_00804548[8] = {0x2a, 0x00, 0x2f, 0x00, 0x00, 0x00, 0x00, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x00804550
struct BattleSummonOdinZAnimDef g_BattleSummonOdinZAnimDef = {{0xb0, 0x3e, 0x80, 0, 0x88, 0, 0, 0, 0, 0}, 0};
unsigned char g_rawData_0080455c[4] = {0x20, 0x00, 0x00, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x00804560
struct CVECTOR g_BattleSummonOdinZOffsetTable[2] = {{0, 0x80, 0xff, 0}, {0}};
// GLOBAL: FF7 0x00804568
unsigned char g_BattleSummonOdinZInitialY[8] = {0x00, 0x00, 0x9c, 0xff, 0x0c, 0xfe, 0x00, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x00804570
int g_BattleSummonOdinZLimitY = (int)0xFA240000;
unsigned char g_rawData_00804574[4] = {0x0c, 0xfe, 0x00, 0x00};
// GLOBAL: FF7 0x00804578
struct BattleActor *g_BattleSummonOdinZCameraTarget = &g_BattleActorRecords[3];
// GLOBAL: FF7 0x00804580
PsxMatrix g_BattleSummonOdinZMatrix2 = {{{0x1000, 0, 0}, {0, 0x1000, 0}, {0, 0, 0x2000}}, {0, 0, 0}};
// GLOBAL: FF7 0x008045a0
BattleFxModelDrawHead g_BattleSummonOdinZAnimParams2 = {0, BATTLE_FX_BZ_MORPH_FACE_GREY | BATTLE_FX_BZ_MORPH_SEMI_TRANSPARENT, 0, 0, BATTLE_FX_BZ_MORPH_DOUBLE_SIDED, 0};
// GLOBAL: FF7 0x008045b0
BattleFxPsxMeshHeader g_BattleSummonOdinZMeshDef = {(int *)g_BattleSummonOdinZMesh, BATTLE_FX_BZ_MORPH_FACE_GREY | BATTLE_FX_BZ_MORPH_SEMI_TRANSPARENT, {0}, 0};
unsigned char g_rawData_008045bc[4] = {0x20, 0x00, 0x00, 0x00};
// clang-format on

// clang-format off
unsigned char g_rawData_008045c0[44] = {
    0x40, 0x01, 0x00, 0x00, 0xf8, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0xf8, 0x01, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00
};
unsigned char g_rawData_008045ec[4] = {0x08, 0x00, 0x00, 0x00};
// clang-format on

#pragma intrinsic(memcpy)
#pragma function(memcpy)

// When the scene pointer is nonzero and g_BattleSummonOdinZLoadedResTable is zero, initializes a
// local resource request with default duration, clears its groupTag and builds the SUMMON/odin2
// path into its resourcePath. Calls Gfx_LoadModelTableIfAbsent with type five, descriptor
// g_BattleSummonOdinZResourceDef, the request, cache g_BattleSummonOdinZLoadedResTable and the
// scene's modelTableBank, then registers g_BattleSummonOdinZSoundGroup sound groups within the same
// guard. Otherwise does nothing. Resource helpers determine allocation and failure behavior; sound
// registration has no separate success test.
// FUNCTION: FF7 0x004a5ac0
void __cdecl BattleFx_SummonLoadResOdinZantetsuken(void)
{
    GfxModelTableBank *sceneField;
    GfxResourceRequest record;
    int kind;
    char pathOut[200];
    GfxResourceRequest *request;
    unsigned int *descriptor;

    if (g_pBattle3dScene != 0 && g_BattleSummonOdinZLoadedResTable == 0) {
        kind = 5;
        descriptor = g_BattleSummonOdinZResourceDef;
        sceneField = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &record;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x0080464c
        request->resourcePath = (char *)BattleFx_BuildResourcePath("SUMMON/odin2", pathOut);
        Gfx_LoadModelTableIfAbsent(kind, (int)descriptor, request, &g_BattleSummonOdinZLoadedResTable, sceneField);
        SfxRegisterSoundGroups(&g_BattleSummonOdinZSoundGroup);
    }
}

// Clears the billboard and scroll matrices, sets the scroll matrix's scale (0x1000, 0x800), the
// scroll offset, g_BattleSummonOdinZStateWord, g_BattleSummonOdinZVar98 and
// g_BattleSummonOdinZInitialAngles, calls BattleFx_SummonBeginOdinZantetsuken, and returns zero.
// FUNCTION: FF7 0x004a5b61
int __cdecl BattleFx_SummonEnterOdinZantetsuken(int targetMask, int actorIdx)
{
    memset((void *)&g_BattleSummonOdinZBillboardMatrix, 0, 0x1E);
    memset((void *)&g_BattleSummonOdinZScrollMatrix, 0, 0x1E);
    g_BattleSummonOdinZScrollMatrix[0] = 0x1000;
    g_BattleSummonOdinZVectorC0.rotation[0] = 0x800 /* 180 deg */;
    g_BattleSummonOdinZScrollOffset = 0xFFFFF400;
    g_BattleSummonOdinZStateWord = 0;
    g_BattleSummonOdinZVar98 = 0;
    g_BattleSummonOdinZInitialAngles.angleX = 0;
    g_BattleSummonOdinZInitialAngles.angleY = 0xFC7C;
    g_BattleSummonOdinZInitialAngles.angleZ = 0xF830;
    BattleFx_SummonBeginOdinZantetsuken(targetMask, actorIdx);
    return 0;
}

// Entry for the Odin Zantetsuken summon. Stores Gfx_GetPsxPacketScratchPtr() in
// g_BattleOverlayDrawListHead, loads resources via BattleFx_SummonLoadResOdinZantetsuken, writes
// popcount(actorMask)*0xF to g_BattleSummonOdinZFrames and g_BattleSummonOdinZCameraScriptTable.frames,
// stores actorMask in g_BattleSummonOdinZWordB0 and takes the target mask centroid; its Z (third
// short) is compared with g_BattleActorRecords[actorIndex].posZ to put
// g_BattleSummonOdinZMatrixD8.t[2] at Z-0x1388 or, when not beyond it, set
// g_BattleSummonOdinZAnglesF8[1] to 0x800 and Z+0x1388.
// Builds g_BattleSummonOdinZMatrixD8 from the rotation in
// g_BattleSummonOdinZAnglesF8 with RotMatrix, allocates a slot for
// BattleFx_SummonOdinZantetsukenTimelineTask (zeroing its ownerSlot) and one for
// BattleFx_SummonOdinZantetsukenActorMotionTask, then calls BattleFx_OdinZantetsuSpawnCameraScript
// with camera script g_BattleSummonOdinZSubeffectTable1 or
// g_BattleSummonOdinZSubeffectTable2 chosen by bit 8 of rand().
// FUNCTION: FF7 0x004a5be5
void __cdecl BattleFx_SummonBeginOdinZantetsuken(int targetMask, int actorIdx)
{
    short position[4];
    struct BattleTaskSlot *slot;

    g_BattleOverlayDrawListHead = (int)Gfx_GetPsxPacketScratchPtr();
    BattleFx_SummonLoadResOdinZantetsuken();
    g_BattleSummonOdinZFrames = Battle_PopCountActorMask(targetMask) * 15;
    g_BattleSummonOdinZCameraScriptTable.frames = g_BattleSummonOdinZFrames;
    Battle_GetActorMaskCentroid(g_BattleSummonOdinZWordB0 = targetMask, position);
    if (position[2] > g_BattleActorRecords[actorIdx].posZ) {
        g_BattleSummonOdinZMatrixD8.t[2] = position[2] - 0x1388;
    } else {
        g_BattleSummonOdinZAnglesF8[1] = 0x800 /* 180 deg */;
        g_BattleSummonOdinZMatrixD8.t[2] = position[2] + 0x1388;
    }
    RotMatrix((SVECTOR *)g_BattleSummonOdinZAnglesF8, &g_BattleSummonOdinZMatrixD8);
    slot = BATTLE_NEW_TASK_SLOT(BattleFx_SummonOdinZantetsukenTimelineTask);
    slot->ownerSlot = 0;
    Battle_AllocateResourceSlot(BattleFx_SummonOdinZantetsukenActorMotionTask);
    if ((rand() & 0x100) != 0) {
        BattleFx_OdinZantetsuSpawnCameraScript(g_BattleSummonOdinZSubeffectTable1, &g_BattleSummonOdinZMatrixD8, actorIdx);
    } else {
        BattleFx_OdinZantetsuSpawnCameraScript(g_BattleSummonOdinZSubeffectTable2, &g_BattleSummonOdinZMatrixD8, actorIdx);
    }
}

// Allocates a task slot for BattleFx_OdinZantetsuRunCameraScript and stores the script table, the
// matrix and the actor index in it.
// FUNCTION: FF7 0x004a5cfc
void __cdecl BattleFx_OdinZantetsuSpawnCameraScript(short *scriptTable, PsxMatrix *matrix, int actorIndex)
{
    CameraScriptTask *slot;

    slot = (CameraScriptTask *)BATTLE_NEW_TASK_SLOT(BattleFx_OdinZantetsuRunCameraScript);
    slot->cursor = scriptTable;
    slot->matrix = (int)matrix;
    slot->actor = actorIndex;
}

// Acquires scratch and interprets eye/focus camera tracks while unpaused, initializing only
// durations and actor selectors once. Reads commands until both durations nonzero; bit 0x40
// selects eye and masking 0xC0 selects hold, direct position, linear target, distance target,
// accelerated target, repeated-sine eased target, actor/bone attachment, detach or output hold
// mode eight. Attachment zero uses task actor without changing offsets; detach clears selector
// only; invalid opcode retires/returns. Distance prediction uses other linear track remaining
// duration; acceleration updates velocity before position; distance advances base/radius then
// normalizes toward other track. Easing increments 4096/duration fraction and repeatedly applies
// SinFixed(fraction/4) for signed easing count; nonpositive count leaves fraction unchanged.
// Updates eye then focus and decrements durations. Output runs even paused. With task matrix,
// unattached positions transform fully and attached offsets rotate then add bone/actor world
// position. Eye installs/restores matrix, focus uses retained installed state, including prior
// state when eye mode eight. Without matrix copies position groups directly, ignoring attachments;
// mode eight preserves output.
// FUNCTION: FF7 0x004a5d3c
void __cdecl BattleFx_OdinZantetsuRunCameraScript(void)
{
    /* VC5 hash order fixes the slots: p=easing, q=wide vector, r=task,
       s=transform flags, t=track index, u/w=scratch vectors. v and x retain
       the original unused frame space (v is the easing-loop index in the
       nine-command dialect). The opcode i follows that space. */
    CameraScriptTask *taskPtr;
    int *wideVec;
    int eased;
    CameraScriptVector *scratchVector;
    int trackIndex;
    int bits;
    CameraScriptVector *predictVec;
    int loop;
    int x[64];
    short opcode;

    taskPtr = (CameraScriptTask *)((char *)BATTLE_CURRENT_TASK_SLOT);
    scratchVector = (CameraScriptVector *)Gfx_GetPsxScratchpadPtr(0);
    predictVec = scratchVector + 1;
    wideVec = (int *)(predictVec + 1);
    if (g_BattleIsPaused == 0) {
        if (taskPtr->initialized == 0) {
            g_BattleSummonOdinCameraEye.frames = 0;
            g_BattleSummonOdinCameraFocus.frames = 0;
            g_BattleSummonOdinCameraEye.actor = -1;
            g_BattleSummonOdinCameraFocus.actor = -1;
            taskPtr->initialized = 1;
        }
        while (g_BattleSummonOdinCameraEye.frames == 0 || g_BattleSummonOdinCameraFocus.frames == 0) {
            opcode = *taskPtr->cursor++;
            if (opcode & 0x40) {
                g_pBattleSummonOdinZCameraTrack = &g_BattleSummonOdinCameraEye;
                g_pBattleSummonOdinZCameraOther = &g_BattleSummonOdinCameraFocus;
            } else {
                g_pBattleSummonOdinZCameraTrack = &g_BattleSummonOdinCameraFocus;
                g_pBattleSummonOdinZCameraOther = &g_BattleSummonOdinCameraEye;
            }
            g_pBattleSummonOdinZCameraTrack->mode = opcode & ~0xc0;
            switch (g_pBattleSummonOdinZCameraTrack->mode) {
            case SUMMON_CAMERA_WAIT:
                g_pBattleSummonOdinZCameraTrack->frames = BattleFx_OdinZantetsuReadCameraScriptOperand(&taskPtr->cursor);
                break;
            case SUMMON_CAMERA_SET_POSITION:
                g_pBattleSummonOdinZCameraTrack->position.x = *taskPtr->cursor++;
                g_pBattleSummonOdinZCameraTrack->position.y = *taskPtr->cursor++;
                g_pBattleSummonOdinZCameraTrack->position.z = *taskPtr->cursor++;
                break;
            case SUMMON_CAMERA_MOVE_LINEAR:
                g_pBattleSummonOdinZCameraTrack->frames = BattleFx_OdinZantetsuReadCameraScriptOperand(&taskPtr->cursor);
                g_pBattleSummonOdinZCameraTrack->velocity.x = (*taskPtr->cursor++ - g_pBattleSummonOdinZCameraTrack->position.x) / g_pBattleSummonOdinZCameraTrack->frames;
                g_pBattleSummonOdinZCameraTrack->velocity.y = (*taskPtr->cursor++ - g_pBattleSummonOdinZCameraTrack->position.y) / g_pBattleSummonOdinZCameraTrack->frames;
                g_pBattleSummonOdinZCameraTrack->velocity.z = (*taskPtr->cursor++ - g_pBattleSummonOdinZCameraTrack->position.z) / g_pBattleSummonOdinZCameraTrack->frames;
                break;
            case SUMMON_CAMERA_MOVE_ORBIT:
                scratchVector->x = g_pBattleSummonOdinZCameraOther->position.x - g_pBattleSummonOdinZCameraTrack->position.x;
                scratchVector->y = g_pBattleSummonOdinZCameraOther->position.y - g_pBattleSummonOdinZCameraTrack->position.y;
                scratchVector->z = g_pBattleSummonOdinZCameraOther->position.z - g_pBattleSummonOdinZCameraTrack->position.z;
                g_pBattleSummonOdinZCameraTrack->distance = SqrtToInt(scratchVector->x * scratchVector->x + scratchVector->y * scratchVector->y + scratchVector->z * scratchVector->z);
                g_pBattleSummonOdinZCameraTrack->base = g_pBattleSummonOdinZCameraTrack->position;
                g_pBattleSummonOdinZCameraTrack->frames = BattleFx_OdinZantetsuReadCameraScriptOperand(&taskPtr->cursor);
                scratchVector->x = *taskPtr->cursor++;
                scratchVector->y = *taskPtr->cursor++;
                scratchVector->z = *taskPtr->cursor++;
                g_pBattleSummonOdinZCameraTrack->velocity.x = (scratchVector->x - g_pBattleSummonOdinZCameraTrack->position.x) / g_pBattleSummonOdinZCameraTrack->frames;
                g_pBattleSummonOdinZCameraTrack->velocity.y = (scratchVector->y - g_pBattleSummonOdinZCameraTrack->position.y) / g_pBattleSummonOdinZCameraTrack->frames;
                g_pBattleSummonOdinZCameraTrack->velocity.z = (scratchVector->z - g_pBattleSummonOdinZCameraTrack->position.z) / g_pBattleSummonOdinZCameraTrack->frames;
                switch (g_pBattleSummonOdinZCameraOther->mode) {
                case SUMMON_CAMERA_MOVE_LINEAR:
                    predictVec->x = g_pBattleSummonOdinZCameraOther->position.x + g_pBattleSummonOdinZCameraOther->velocity.x * g_pBattleSummonOdinZCameraOther->frames;
                    predictVec->y = g_pBattleSummonOdinZCameraOther->position.y + g_pBattleSummonOdinZCameraOther->velocity.y * g_pBattleSummonOdinZCameraOther->frames;
                    predictVec->z = g_pBattleSummonOdinZCameraOther->position.z + g_pBattleSummonOdinZCameraOther->velocity.z * g_pBattleSummonOdinZCameraOther->frames;
                    break;
                default:
                    predictVec->x = g_pBattleSummonOdinZCameraOther->position.x;
                    predictVec->y = g_pBattleSummonOdinZCameraOther->position.y;
                    predictVec->z = g_pBattleSummonOdinZCameraOther->position.z;
                    break;
                }
                scratchVector->x -= predictVec->x;
                scratchVector->y -= predictVec->y;
                scratchVector->z -= predictVec->z;
                g_pBattleSummonOdinZCameraTrack->delta = (SqrtToInt(scratchVector->x * scratchVector->x + scratchVector->y * scratchVector->y + scratchVector->z * scratchVector->z) - g_pBattleSummonOdinZCameraTrack->distance) / g_pBattleSummonOdinZCameraTrack->frames;
                break;
            case SUMMON_CAMERA_MOVE_ACCEL:
                g_pBattleSummonOdinZCameraTrack->frames = BattleFx_OdinZantetsuReadCameraScriptOperand(&taskPtr->cursor);
                scratchVector->x = *taskPtr->cursor++ - g_pBattleSummonOdinZCameraTrack->position.x;
                scratchVector->y = *taskPtr->cursor++ - g_pBattleSummonOdinZCameraTrack->position.y;
                scratchVector->z = *taskPtr->cursor++ - g_pBattleSummonOdinZCameraTrack->position.z;
                g_pBattleSummonOdinZCameraTrack->base.x = ((scratchVector->x - g_pBattleSummonOdinZCameraTrack->velocity.x * g_pBattleSummonOdinZCameraTrack->frames) * 2) / (g_pBattleSummonOdinZCameraTrack->frames * g_pBattleSummonOdinZCameraTrack->frames);
                g_pBattleSummonOdinZCameraTrack->base.y = ((scratchVector->y - g_pBattleSummonOdinZCameraTrack->velocity.y * g_pBattleSummonOdinZCameraTrack->frames) * 2) / (g_pBattleSummonOdinZCameraTrack->frames * g_pBattleSummonOdinZCameraTrack->frames);
                g_pBattleSummonOdinZCameraTrack->base.z = ((scratchVector->z - g_pBattleSummonOdinZCameraTrack->velocity.z * g_pBattleSummonOdinZCameraTrack->frames) * 2) / (g_pBattleSummonOdinZCameraTrack->frames * g_pBattleSummonOdinZCameraTrack->frames);
                break;
            case SUMMON_CAMERA_MOVE_EASED:
                g_pBattleSummonOdinZCameraTrack->frames = BattleFx_OdinZantetsuReadCameraScriptOperand(&taskPtr->cursor);
                g_pBattleSummonOdinZCameraTrack->easing = *taskPtr->cursor++;
                g_pBattleSummonOdinZCameraTrack->distance = 0;
                g_pBattleSummonOdinZCameraTrack->delta = 0x1000 / g_pBattleSummonOdinZCameraTrack->frames;
                g_pBattleSummonOdinZCameraTrack->base.x = g_pBattleSummonOdinZCameraTrack->position.x;
                g_pBattleSummonOdinZCameraTrack->base.y = g_pBattleSummonOdinZCameraTrack->position.y;
                g_pBattleSummonOdinZCameraTrack->base.z = g_pBattleSummonOdinZCameraTrack->position.z;
                g_pBattleSummonOdinZCameraTrack->velocity.x = *taskPtr->cursor++ - g_pBattleSummonOdinZCameraTrack->position.x;
                g_pBattleSummonOdinZCameraTrack->velocity.y = *taskPtr->cursor++ - g_pBattleSummonOdinZCameraTrack->position.y;
                g_pBattleSummonOdinZCameraTrack->velocity.z = *taskPtr->cursor++ - g_pBattleSummonOdinZCameraTrack->position.z;
                break;
            case SUMMON_CAMERA_ATTACH_BONE:
                if ((g_pBattleSummonOdinZCameraTrack->actor = *taskPtr->cursor++) == 0)
                    g_pBattleSummonOdinZCameraTrack->actor = taskPtr->actor;
                g_pBattleSummonOdinZCameraTrack->bone = *taskPtr->cursor++;
                break;
            case SUMMON_CAMERA_DETACH:
                g_pBattleSummonOdinZCameraTrack->actor = -1;
                break;
            case SUMMON_CAMERA_HOLD_OUTPUT:
                g_pBattleSummonOdinZCameraTrack->frames = BattleFx_OdinZantetsuReadCameraScriptOperand(&taskPtr->cursor);
                break;
            default:
                taskPtr->active = -1;
                return;
            }
        }
        g_pBattleSummonOdinZCameraTrack = &g_BattleSummonOdinCameraEye;
        g_pBattleSummonOdinZCameraOther = &g_BattleSummonOdinCameraFocus;
        for (trackIndex = 0; trackIndex < 2; trackIndex++) {
            switch (g_pBattleSummonOdinZCameraTrack->mode) {
            case SUMMON_CAMERA_WAIT:
                break;
            case SUMMON_CAMERA_MOVE_ACCEL:
                g_pBattleSummonOdinZCameraTrack->velocity.x += g_pBattleSummonOdinZCameraTrack->base.x;
                g_pBattleSummonOdinZCameraTrack->velocity.y += g_pBattleSummonOdinZCameraTrack->base.y;
                g_pBattleSummonOdinZCameraTrack->velocity.z += g_pBattleSummonOdinZCameraTrack->base.z;
            case SUMMON_CAMERA_MOVE_LINEAR:
                g_pBattleSummonOdinZCameraTrack->position.x += g_pBattleSummonOdinZCameraTrack->velocity.x;
                g_pBattleSummonOdinZCameraTrack->position.y += g_pBattleSummonOdinZCameraTrack->velocity.y;
                g_pBattleSummonOdinZCameraTrack->position.z += g_pBattleSummonOdinZCameraTrack->velocity.z;
                break;
            case SUMMON_CAMERA_MOVE_ORBIT:
                g_pBattleSummonOdinZCameraTrack->base.x += g_pBattleSummonOdinZCameraTrack->velocity.x;
                g_pBattleSummonOdinZCameraTrack->base.y += g_pBattleSummonOdinZCameraTrack->velocity.y;
                g_pBattleSummonOdinZCameraTrack->base.z += g_pBattleSummonOdinZCameraTrack->velocity.z;
                g_pBattleSummonOdinZCameraTrack->distance += g_pBattleSummonOdinZCameraTrack->delta;
                wideVec[0] = g_pBattleSummonOdinZCameraOther->position.x - g_pBattleSummonOdinZCameraTrack->base.x;
                wideVec[1] = g_pBattleSummonOdinZCameraOther->position.y - g_pBattleSummonOdinZCameraTrack->base.y;
                wideVec[2] = g_pBattleSummonOdinZCameraOther->position.z - g_pBattleSummonOdinZCameraTrack->base.z;
                VectorNormalToSVector(wideVec, (short *)scratchVector);
                g_pBattleSummonOdinZCameraTrack->position.x = g_pBattleSummonOdinZCameraOther->position.x - ((scratchVector->x * g_pBattleSummonOdinZCameraTrack->distance) >> 12);
                g_pBattleSummonOdinZCameraTrack->position.y = g_pBattleSummonOdinZCameraOther->position.y - ((scratchVector->y * g_pBattleSummonOdinZCameraTrack->distance) >> 12);
                g_pBattleSummonOdinZCameraTrack->position.z = g_pBattleSummonOdinZCameraOther->position.z - ((scratchVector->z * g_pBattleSummonOdinZCameraTrack->distance) >> 12);
                break;
            case SUMMON_CAMERA_MOVE_EASED:
                g_pBattleSummonOdinZCameraTrack->distance += g_pBattleSummonOdinZCameraTrack->delta;
                eased = g_pBattleSummonOdinZCameraTrack->distance;
                for (loop = 0; loop < g_pBattleSummonOdinZCameraTrack->easing; loop++)
                    eased = SinFixed(eased / 4);
                g_pBattleSummonOdinZCameraTrack->position.x = g_pBattleSummonOdinZCameraTrack->base.x + ((g_pBattleSummonOdinZCameraTrack->velocity.x * eased) >> 12);
                g_pBattleSummonOdinZCameraTrack->position.y = g_pBattleSummonOdinZCameraTrack->base.y + ((g_pBattleSummonOdinZCameraTrack->velocity.y * eased) >> 12);
                g_pBattleSummonOdinZCameraTrack->position.z = g_pBattleSummonOdinZCameraTrack->base.z + ((g_pBattleSummonOdinZCameraTrack->velocity.z * eased) >> 12);
                break;
            }
            g_pBattleSummonOdinZCameraTrack->frames--;
            g_pBattleSummonOdinZCameraTrack = &g_BattleSummonOdinCameraFocus;
            g_pBattleSummonOdinZCameraOther = &g_BattleSummonOdinCameraEye;
        }
    }
    if (taskPtr->matrix) {
        if (g_BattleSummonOdinCameraEye.mode != SUMMON_CAMERA_HOLD_OUTPUT) {
            SetRotMatrix(taskPtr->matrixPtr);
            SetTransMatrix(taskPtr->matrixPtr);
            if (g_BattleSummonOdinCameraEye.actor == -1) {
                RotTransPoint(&g_BattleSummonOdinCameraEye.position, wideVec, &bits);
                g_BattleCameraPosition.cameraPosX = wideVec[0];
                g_BattleCameraPosition.cameraPosY = wideVec[1];
                g_BattleCameraPosZ = wideVec[2];
            } else {
                RotateSVectorByInstalledMatrix(&g_BattleSummonOdinCameraEye.position, wideVec);
                if (g_BattleSummonOdinCameraEye.bone != -1) {
                    Battle_GetBoneWorldPos(g_BattleSummonOdinCameraEye.actor, g_BattleSummonOdinCameraEye.bone, (short *)scratchVector);
                } else {
                    scratchVector->x = g_BattleActorRecords[g_BattleSummonOdinCameraEye.actor].posX;
                    scratchVector->y = g_BattleActorRecords[g_BattleSummonOdinCameraEye.actor].posY;
                    scratchVector->z = g_BattleActorRecords[g_BattleSummonOdinCameraEye.actor].posZ;
                }
                g_BattleCameraPosition.cameraPosX = wideVec[0] + scratchVector->x;
                g_BattleCameraPosition.cameraPosY = wideVec[1] + scratchVector->y;
                g_BattleCameraPosZ = wideVec[2] + scratchVector->z;
                SetRotMatrix(taskPtr->matrixPtr);
                SetTransMatrix(taskPtr->matrixPtr);
            }
        }
        if (g_BattleSummonOdinCameraFocus.mode != SUMMON_CAMERA_HOLD_OUTPUT) {
            if (g_BattleSummonOdinCameraFocus.actor == -1) {
                RotTransPoint(&g_BattleSummonOdinCameraFocus.position, wideVec, &bits);
                g_BattleCameraFocalPoint.focalPointX = wideVec[0];
                g_BattleCameraFocalPoint.focalPointY = wideVec[1];
                g_BattleCameraFocusZ = wideVec[2];
            } else {
                RotateSVectorByInstalledMatrix(&g_BattleSummonOdinCameraFocus.position, wideVec);
                if (g_BattleSummonOdinCameraFocus.bone != -1) {
                    Battle_GetBoneWorldPos(g_BattleSummonOdinCameraFocus.actor, g_BattleSummonOdinCameraFocus.bone, (short *)scratchVector);
                } else {
                    scratchVector->x = g_BattleActorRecords[g_BattleSummonOdinCameraFocus.actor].posX;
                    scratchVector->y = g_BattleActorRecords[g_BattleSummonOdinCameraFocus.actor].posY;
                    scratchVector->z = g_BattleActorRecords[g_BattleSummonOdinCameraFocus.actor].posZ;
                }
                g_BattleCameraFocalPoint.focalPointX = wideVec[0] + scratchVector->x;
                g_BattleCameraFocalPoint.focalPointY = wideVec[1] + scratchVector->y;
                g_BattleCameraFocusZ = wideVec[2] + scratchVector->z;
            }
        }
    } else {
        if (g_BattleSummonOdinCameraEye.mode != SUMMON_CAMERA_HOLD_OUTPUT) {
            *(int *)&g_BattleCameraPosition = *(int *)&g_BattleSummonOdinCameraEye.position;
            *(int *)&g_BattleCameraPosZ = *(int *)&g_BattleSummonOdinCameraEye.position.z;
        }
        if (g_BattleSummonOdinCameraFocus.mode != SUMMON_CAMERA_HOLD_OUTPUT) {
            *(int *)&g_BattleCameraFocalPoint = *(int *)&g_BattleSummonOdinCameraFocus.position;
            *(int *)&g_BattleCameraFocusZ = *(int *)&g_BattleSummonOdinCameraFocus.position.z;
        }
    }
}

// Same token reader as BattleFx_NeoBahamutReadCameraScriptOperand with the pool at
// g_BattleSummonOdinZCameraScriptTable.operands: reads a short at the cursor, advances it by 2,
// and resolves a negative token as (&g_BattleSummonOdinZCameraScriptTable.operands)[-token].
// FUNCTION: FF7 0x004a6c7e
short __cdecl BattleFx_OdinZantetsuReadCameraScriptOperand(short **cursor)
{
    int token;

    token = **cursor;
    *cursor += 1;
    if (token < 0) {
        token = (&g_BattleSummonOdinZCameraScriptTable.operands)[-token];
    }
    return (short)token;
}

// Acquires retained scratch and refreshes scene resource field eight or zero. Before pause check,
// when g_BattleSummonOdinZStateWord is active sets descriptor sine pulse from g_BattleSummonOdinZVar98 modulo five, gets
// actor-three attachment eighteen, installs/refreshes and calls BattleFx_SummonOdinZantetsukenDrawPsxGouraudMesh. Unpaused timeline
// holds 84, then on entry allocates ring one at (0,-30000,0), clears actor RGB, sets field 0x14
// 4096/field6 512 and enables attachment draw for sixty. Next 25 reset actor fields and point Y/Z
// -5000/-4500, advancing pulse counter. Next 35 reset Y velocity -150, emit BattleFx_SummonOdinZantetsukenSpawnBoneSprites(3) first
// fifteen, advance Y/Z by velocity/100 and velocity by five, advancing pulse. Next 25 reset point
// -300/-3000, first nineteen raise negative Y by fifteen and Z by 300 with pulse updates; at
// elapsed ten schedule BattleFx_SummonOdinZantetsukenSpawnAfterimageTask/004a8431 variant zero. Two following fifteen-count phases
// reset point -150/zero and repeat first-nine motion/pulse, each scheduling the same pair on
// entry. Next g_BattleSummonOdinZFrames counts set actor bit two and disable attachment draw on entry,
// increment task count and return before actor position update. Final fifteen clear actor bits
// zero/one, enable draw and reset Y/Z -1600/zero; first eight raise Y 200, eighth sets velocity
// -500, clears RGB/field 0x14 and writes eight through actor byte span sized by signed field 0x10.
// Later fade field 0x14 over seven and advance Y/velocity 50; all advance Z 500/pulse. Except
// entry draw scene resource with transformed retained translation into destination plus 0x4078
// shift zero; last allocates ring two. Later retire but common update persists: rotates retained
// point by g_BattleSummonOdinZMatrixD8, adds translation and writes actor XYZ, then increments count.
// FUNCTION: FF7 0x004a6cb8
void __cdecl BattleFx_SummonOdinZantetsukenActorMotionTask(void)
{
    int frameIndex;
    BattleFxOdinZantetsukenMotionRecord *ringRecord;
    BattleFxOdinZantetsukenMotion *taskPtr;
    int trackIndex;
    int pulse;
    struct BattleTaskSlot *childTask;

    taskPtr = (BattleFxOdinZantetsukenMotion *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonOdinZByteBC & 1) == 0) {
        g_BattleSummonOdinZByteBC |= 1;
        g_pBattleSummonOdinZMotionScratch = (BattleFxOdinZMotionScratch *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_pBattle3dScene != 0) {
        g_BattleSummonOdinZAnimParams2.modelGroup = g_pBattle3dScene->modelRequest;
    } else {
        g_BattleSummonOdinZAnimParams2.modelGroup = 0;
    }
    if (g_BattleSummonOdinZStateWord != 0) {
        frameIndex = g_BattleSummonOdinZVar98 % 5;
        pulse = (frameIndex << 12) / 5;
        g_BattleSummonOdinZMeshDef.shade = (SinFixed(pulse) << 10 >> 12) + 0x400;
        Battle3d_GetActorAttachMatrix(3, 18, (PsxMatrix *)&g_BattleActorRecords[3].boneNodes5[0x2a4]);
        SetRotMatrix(&g_BattleActorRecords[3].boneNodes5[0x2a4]);
        SetTransMatrix(&g_BattleActorRecords[3].boneNodes5[0x2a4]);
        SetDrawColorRGB(0, 0, 0);
        RefreshGlobalTransformState();
        g_BattleOverlayDrawListHead = (int)BattleFx_SummonOdinZantetsukenDrawPsxGouraudMesh(&g_BattleSummonOdinZMeshDef, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frameIndex = taskPtr->frameIndex;
    if (frameIndex < 84) { /* empty first arm: lib/codegen.h */
    } else {
        frameIndex -= 84;
        if (frameIndex < 60) {
            if (frameIndex == 0) {
                ringRecord = (BattleFxOdinZantetsukenMotionRecord *)Battle_AllocSceneMessage(1);
                ringRecord->posX = 0;
                ringRecord->posY = 0x8AD0;
                ringRecord->posZ = 0;
                ringRecord->rotationY = g_BattleSummonOdinZAnglesF8[1] + 0x800;
                g_BattleSummonOdinZCameraTarget->tintR = 0;
                g_BattleSummonOdinZCameraTarget->tintG = 0;
                g_BattleSummonOdinZCameraTarget->tintB = 0;
                g_BattleSummonOdinZCameraTarget->tintPhase = 0x1000;
                g_BattleSummonOdinZCameraTarget->scale = 0x200 /* 0.125 */;
                g_BattleSummonOdinZStateWord = 1;
            }
        } else {
            frameIndex -= 60;
            if (frameIndex < 25) {
                if (frameIndex == 0) {
                    g_BattleSummonOdinZCameraTarget->tintPhase = 0;
                    g_BattleSummonOdinZCameraTarget->scale = 0x1000 /* 1.0 */;
                    g_BattleSummonOdinZInitialAngles.angleY = 0xEC78;
                    g_BattleSummonOdinZInitialAngles.angleZ = 0xEE6C;
                }
                g_BattleSummonOdinZVar98++;
            } else {
                frameIndex -= 25;
                if (frameIndex < 35) {
                    if (frameIndex == 0) {
                        taskPtr->angularVelocity = 0xFF6A /* -150 */;
                    }
                    if (frameIndex < 15) {
                        BattleFx_SummonOdinZantetsukenSpawnBoneSprites(3);
                    }
                    g_BattleSummonOdinZInitialAngles.angleY += taskPtr->angularVelocity;
                    g_BattleSummonOdinZInitialAngles.angleZ += 0x64;
                    taskPtr->angularVelocity += 5;
                    g_BattleSummonOdinZVar98++;
                } else {
                    frameIndex -= 35;
                    if (frameIndex < 25) {
                        if (frameIndex == 0) {
                            g_BattleSummonOdinZInitialAngles.angleY = 0xFED4;
                            g_BattleSummonOdinZInitialAngles.angleZ = 0xF448;
                        }
                        if (frameIndex < 19) {
                            if (g_BattleSummonOdinZInitialAngles.angleY < 0) {
                                g_BattleSummonOdinZInitialAngles.angleY += 0xF;
                                g_BattleSummonOdinZInitialAngles.angleZ += 0x12C;
                            }
                            g_BattleSummonOdinZVar98++;
                        }
                        if (frameIndex == 10) {
                            Battle_AllocateResourceSlot(BattleFx_SummonOdinZantetsukenSpawnAfterimageTask);
                            childTask = BATTLE_NEW_TASK_SLOT(BattleFx_SummonOdinZantetsukenSpawnTrailSegmentsTask);
                            childTask->ownerSlot = 0;
                        }
                    } else {
                        frameIndex -= 25;
                        if (frameIndex < 15) {
                            if (frameIndex == 0) {
                                g_BattleSummonOdinZInitialAngles.angleY = 0xFF6A;
                                g_BattleSummonOdinZInitialAngles.angleZ = 0;
                            }
                            if (frameIndex < 9) {
                                if (g_BattleSummonOdinZInitialAngles.angleY < 0) {
                                    g_BattleSummonOdinZInitialAngles.angleY += 0xF;
                                    g_BattleSummonOdinZInitialAngles.angleZ += 0x12C;
                                }
                                g_BattleSummonOdinZVar98++;
                            }
                            if (frameIndex == 0) {
                                Battle_AllocateResourceSlot(BattleFx_SummonOdinZantetsukenSpawnAfterimageTask);
                                childTask = BATTLE_NEW_TASK_SLOT(BattleFx_SummonOdinZantetsukenSpawnTrailSegmentsTask);
                                childTask->ownerSlot = 0;
                            }
                        } else {
                            frameIndex -= 15;
                            if (frameIndex < 15) {
                                if (frameIndex == 0) {
                                    g_BattleSummonOdinZInitialAngles.angleY = 0xFF6A;
                                    g_BattleSummonOdinZInitialAngles.angleZ = 0;
                                }
                                if (frameIndex < 9) {
                                    if (g_BattleSummonOdinZInitialAngles.angleY < 0) {
                                        g_BattleSummonOdinZInitialAngles.angleY += 0xF;
                                        g_BattleSummonOdinZInitialAngles.angleZ += 0x12C;
                                    }
                                    g_BattleSummonOdinZVar98++;
                                }
                                if (frameIndex == 0) {
                                    Battle_AllocateResourceSlot(BattleFx_SummonOdinZantetsukenSpawnAfterimageTask);
                                    childTask = BATTLE_NEW_TASK_SLOT(BattleFx_SummonOdinZantetsukenSpawnTrailSegmentsTask);
                                    childTask->ownerSlot = 0;
                                }
                            } else {
                                frameIndex -= 15;
                                if (frameIndex < g_BattleSummonOdinZFrames) {
                                    if (frameIndex == 0) {
                                        g_BattleSummonOdinZCameraTarget->drawFlags |= 2;
                                        g_BattleSummonOdinZStateWord = 0;
                                    }
                                    (taskPtr->frameIndex)++;
                                    return;
                                }
                                frameIndex -= g_BattleSummonOdinZFrames;
                                if (frameIndex < 15) {
                                    if (frameIndex == 0) {
                                        g_BattleSummonOdinZCameraTarget->drawFlags &= 0xFC;
                                        g_BattleSummonOdinZStateWord = 1;
                                        g_BattleSummonOdinZInitialAngles.angleY = 0xF9C0;
                                        g_BattleSummonOdinZInitialAngles.angleZ = 0;
                                    }
                                    if (frameIndex < 8) {
                                        g_BattleSummonOdinZInitialAngles.angleY += 0xC8;
                                    } else if (frameIndex == 8) {
                                        taskPtr->angularVelocity = 0xFE0C /* -500 */;
                                        g_BattleSummonOdinZCameraTarget->tintB = 0;
                                        g_BattleSummonOdinZCameraTarget->tintG = 0;
                                        g_BattleSummonOdinZCameraTarget->tintR = 0;
                                        g_BattleSummonOdinZCameraTarget->tintPhase = 0;
                                        for (trackIndex = 0; trackIndex < g_BattleSummonOdinZCameraTarget->boneCount; trackIndex++) {
                                            (&g_BattleSummonOdinZCameraTarget->fadeFlags)[trackIndex] = 8;
                                        }
                                    } else {
                                        g_BattleSummonOdinZCameraTarget->tintPhase = (frameIndex - 8 << 12) / 7;
                                        g_BattleSummonOdinZInitialAngles.angleY += taskPtr->angularVelocity;
                                        taskPtr->angularVelocity += 50;
                                    }
                                    g_BattleSummonOdinZInitialAngles.angleZ += 0x1F4;
                                    if (frameIndex != 0) {
                                        g_BattleSummonOdinZMatrix2.t[0] = g_BattleSummonOdinZInitialAngles.angleX;
                                        g_BattleSummonOdinZMatrix2.t[1] = 0;
                                        g_BattleSummonOdinZMatrix2.t[2] = g_BattleSummonOdinZInitialAngles.angleZ - 0x3E8;
                                        MultiplyMatrix(&g_BattleSummonOdinZMatrixE0, &g_BattleSummonOdinZMatrix2, &g_pBattleSummonOdinZMotionScratch->matrix);
                                        SetRotMatrix(&g_pBattleSummonOdinZMotionScratch->matrix);
                                        SetTransMatrix(&g_pBattleSummonOdinZMotionScratch->matrix);
                                        g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonOdinZAnimParams2, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_2, 0, g_BattleOverlayDrawListHead);
                                    }
                                    if (frameIndex == 14) {
                                        Battle_AllocSceneMessage(2);
                                    }
                                    g_BattleSummonOdinZVar98++;
                                } else {
                                    taskPtr->ownerSlot = BATTLE_TASK_FREE;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    TransformVectorToInt16(&g_BattleSummonOdinZMatrixD8, &g_BattleSummonOdinZInitialAngles.angleX, &g_pBattleSummonOdinZMotionScratch->pos);
    g_BattleSummonOdinZCameraTarget->posX = g_BattleSummonOdinZMatrixD8.t[0] + g_pBattleSummonOdinZMotionScratch->pos.vx;
    g_BattleSummonOdinZCameraTarget->posY = g_BattleSummonOdinZMatrixD8.t[1] + g_pBattleSummonOdinZMotionScratch->pos.vy;
    g_BattleSummonOdinZCameraTarget->posZ = g_BattleSummonOdinZMatrixD8.t[2] + g_pBattleSummonOdinZMotionScratch->pos.vz;
    (taskPtr->frameIndex)++;
}

// Skips eight bytes at mesh command start, then walks signed-count gouraud triangles in twenty-
// byte records and gouraud quads in twenty-four-byte records through shared render-state storage.
// Transforms first three signed-offset vertices and loads following topology indices before
// filtering, including after final records. Rejects flags 0x60000 and negative orientation unless
// descriptor bit 0x20 bypasses orientation; quad field six supplies fourth vertex through
// SetPendingResourceHandle/ApplyPendingResourceHandle. Queries depth, subtracts 0x60 and floors
// 0x40 locally without using that result. Nonzero descriptor shade short ten shades colours and
// sets commands 0x30/0x38; zero copies three/four record colours directly, retaining their command
// byte. Descriptor bit eight ORs two into command. Submits accepted primitives and returns shared
// render-state pointer even for empty streams.
// FUNCTION: FF7 0x004a7411
int __cdecl BattleFx_SummonOdinZantetsukenDrawPsxGouraudMesh(BattleFxPsxMeshHeader *mesh, int orderingTable, int depthShift, int packetCursor)
{
    int origin;
    int padBlock; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int vert1;
    int count;
    int padWord[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int facing;
    int i;
    BattleFxStingMeshHeader *recordPointer;
    int zDepth[4];
    int unusedSlot; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int vertA;
    int vertC;
    BattleFxZantetsukenQuadPacket *primitive;
    BattleFxZantetsukenQuadPacket *quadPacket;
    int rejectFlags;

    origin = mesh->dataPtr + 4;
    recordPointer = (BattleFxStingMeshHeader *)(origin + *(int *)mesh->dataPtr);
    recordPointer = (BattleFxStingMeshHeader *)((char *)recordPointer + 4);
    recordPointer = (BattleFxStingMeshHeader *)((char *)recordPointer + 4);
    count = recordPointer->vertexOffsetA;
    recordPointer = (BattleFxStingMeshHeader *)((char *)recordPointer + 4);
    primitive = (BattleFxZantetsukenQuadPacket *)Gfx_GetPsxPacketScratchPtr();
    vertA = origin + recordPointer->vertexOffsetA;
    vert1 = origin + recordPointer->vertexOffset1;
    vertC = origin + recordPointer->vertexOffsetC;
    for (i = 0; i < count; i++) {
        StoreDeferredDrawArgs((void *)vertA, (void *)vert1, (void *)vertC);
        ApplyDeferredDrawArgs();
        vertA = origin + recordPointer->vertexSlotA;
        vert1 = origin + recordPointer->vertexSlot1;
        vertC = origin + recordPointer[1].vertexOffsetA;
        Gfx_PsxStoreClearFlag(&rejectFlags);
        if ((rejectFlags & 0x60000) == 0) {
            if ((mesh->flags & 0x20) != 0) {
                facing = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&facing);
            }
            if (facing >= 0) {
                primitive->tag = 0x6000000;
                Gfx_ReadTransformOrderingDepth2(zDepth);
                zDepth[0] -= 96;
                if (zDepth[0] < 64) {
                    zDepth[0] = 64;
                }
                if (mesh->shade != 0) {
                    Gfx_ShadePsxTriVertexColors(&recordPointer->color0Vec, &recordPointer->color1Vec, &recordPointer->color2Vec, mesh->shade, &primitive->color0Vec, &primitive->texWordVec, &primitive->attrWordVec);
                    primitive->primCode = 0x30;
                } else {
                    primitive->color0 = recordPointer->color0;
                    primitive->texWord = recordPointer->color1;
                    primitive->attrWord = recordPointer->color2;
                }
                if ((mesh->flags & 8) != 0) {
                    primitive->primCode |= 2;
                }
                Gfx_DrawPsxPrimitive(primitive);
            }
        }
        recordPointer = (BattleFxStingMeshHeader *)((char *)recordPointer + 0x14);
    }
    count = recordPointer->vertexOffsetA;
    recordPointer = (BattleFxStingMeshHeader *)((char *)recordPointer + 4);
    quadPacket = (BattleFxZantetsukenQuadPacket *)primitive;
    vertA = origin + recordPointer->vertexOffsetA;
    vert1 = origin + recordPointer->vertexOffset1;
    vertC = origin + recordPointer->vertexOffsetC;
    for (i = 0; i < count; i++) {
        StoreDeferredDrawArgs((void *)vertA, (void *)vert1, (void *)vertC);
        ApplyDeferredDrawArgs();
        vertA = origin + recordPointer[1].vertexOffsetA;
        vert1 = origin + recordPointer[1].vertexOffset1;
        vertC = origin + recordPointer[1].vertexOffsetC;
        Gfx_PsxStoreClearFlag(&rejectFlags);
        if ((rejectFlags & 0x60000) == 0) {
            if ((mesh->flags & 0x20) != 0) {
                facing = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&facing);
            }
            if (facing >= 0) {
                SetPendingResourceHandle((GfxPsxShortVertexSlot *)(origin + recordPointer->resourceOffset));
                ApplyPendingResourceHandle();
                Gfx_ReadTransformOrderingDepth2(zDepth);
                zDepth[0] -= 96;
                if (zDepth[0] < 64) {
                    zDepth[0] = 64;
                }
                quadPacket->tag = 0x8000000;
                if (mesh->shade != 0) {
                    Gfx_ShadePsxTriVertexColors(&recordPointer->color0Vec, &recordPointer->color1Vec, &recordPointer->color2Vec, mesh->shade, (CVECTOR *)&quadPacket->color0, (CVECTOR *)&quadPacket->texWord, (CVECTOR *)&quadPacket->attrWord);
                    Gfx_ShadePsxVertexColor(&recordPointer->color3Vec, mesh->shade, (CVECTOR *)&quadPacket->color3);
                    quadPacket->primCode = 0x38;
                } else {
                    quadPacket->color0 = recordPointer->color0;
                    quadPacket->texWord = recordPointer->color1;
                    quadPacket->attrWord = recordPointer->color2;
                    quadPacket->color3 = recordPointer->color3;
                }
                if ((mesh->flags & 8) != 0) {
                    quadPacket->primCode |= 2;
                }
                Gfx_DrawPsxPrimitive(quadPacket);
            }
        }
        recordPointer++;
    }
    return (int)quadPacket;
}

// For each iteration below count allocates BattleFx_SummonOdinZantetsukenAnimatedSpriteTask at one of six randomly selected actor-three
// bones, jitters each coordinate -200 through 199 and assigns random scale 0x1000 through 0x1FFF.
// No pause guard appears.
// FUNCTION: FF7 0x004a7815
void __cdecl BattleFx_SummonOdinZantetsukenSpawnBoneSprites(int count)
{
    int idx;
    BattleFxZantetsukenBoneSpriteTask *sprite;
    int boneIndex;
    int scaleValue;

    for (idx = 0; idx < count; idx++) {
        sprite = (BattleFxZantetsukenBoneSpriteTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonOdinZantetsukenAnimatedSpriteTask);
        boneIndex = g_BattleSummonOdinZRangeTable[rand() % 6];
        Battle_GetBoneWorldPos(3, boneIndex, &sprite->posX);
        sprite->posX += rand() % 400 - 200;
        sprite->posY += rand() % 400 - 200;
        sprite->posZ += rand() % 400 - 200;
        if (DEAD_IN_RETAIL) {
            scaleValue = 0x2000 /* 2.0 */;
        } else {
            scaleValue = rand() % 0x1000 + 0x1000;
        }
        sprite->scale = scaleValue;
    }
}

// Per-frame bone sprite task. Loads g_BattleSummonOdinZTaskParam.spriteField from
// Gfx_ModelTableGetLoadedRecord(4, g_BattleSummonOdinZLoadedResTable) and
// g_BattleSummonOdinZTaskParam.objField from Battle3d_GetSceneRenderObject(1, g_pBattle3dScene),
// installs a billboard matrix for the task position using its scale at depth 0, puts the frame
// counter with bit 0x8000 set into g_BattleSummonOdinZColorData.word4, and submits the sprite
// into g_BattleOverlayDrawListHead. When g_BattleIsPaused is clear and
// g_BattleSummonOdinZTaskParam.spriteField is not null it advances the frame counter and retires
// the slot once the counter reaches the sprite group's frame count (its second word).
// FUNCTION: FF7 0x004a792c
void __cdecl BattleFx_SummonOdinZantetsukenAnimatedSpriteTask(void)
{
    BattleFxZantetsukenAnimatedSpriteTask *task;

    task = (BattleFxZantetsukenAnimatedSpriteTask *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSummonOdinZTaskParam.spriteField = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(4, g_BattleSummonOdinZLoadedResTable);
    g_BattleSummonOdinZTaskParam.objField = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->scale, 0);
    g_BattleSummonOdinZColorData.frameIndex = task->frame | 0x8000;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSummonOdinZTaskParam.spriteField, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0 && g_BattleSummonOdinZTaskParam.spriteField != 0) {
        (task->frame)++;
        if (task->frame >= g_BattleSummonOdinZTaskParam.spriteField->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

// When unpaused allocates BattleFx_SummonOdinZantetsukenAfterimageTask with the parent counter as its first word, obtains actor-
// three attachment matrix 0x12, composes it relative to g_BattleSummonOdinZMatrixE0 into
// g_BattleSummonOdinZAfterimageMatrices.matrices[counter] and saves that pointer in the child. Advances the parent counter and
// retires at nine or above.
// FUNCTION: FF7 0x004a7a03
void __cdecl BattleFx_SummonOdinZantetsukenSpawnAfterimageTask(void)
{
    BattleFxAfterimageSlot *task;
    BattleFxZantetsukenAfterimage *child;

    task = (BattleFxAfterimageSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    child = (BattleFxZantetsukenAfterimage *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonOdinZantetsukenAfterimageTask);
    child->ownerSlot = task->recordIndex;
    Battle3d_GetActorAttachMatrix(3, 18, (PsxMatrix *)&g_BattleActorRecords[3].boneNodes5[0x2a4]);
    Battle_ComposeMatrixRelativeToCamera((short *)&g_BattleSummonOdinZMatrixE0, (void *)&g_BattleActorRecords[3].boneNodes5[0x2a4], &g_BattleSummonOdinZAfterimageMatrices.matrices[task->recordIndex]);
    child->matrix = &g_BattleSummonOdinZAfterimageMatrices.matrices[task->recordIndex];
    (task->recordIndex)++;
    if (task->recordIndex >= 9) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Lazily acquires data block zero; retires before drawing when counter plus first slot word
// reaches fifteen or counter reaches nine, including while paused. Otherwise sets a counter-
// derived request short, composes g_BattleSummonOdinZMatrixE0 with the saved slot matrix pointer, installs the
// result and emits a black primitive through BattleFx_SummonOdinZantetsukenDrawPsxMesh; advances the counter only when
// unpaused.
// FUNCTION: FF7 0x004a7ac8
void __cdecl BattleFx_SummonOdinZantetsukenAfterimageTask(void)
{
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxZantetsukenAfterimage *task;
    int frameSum;

    task = (BattleFxZantetsukenAfterimage *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonOdinZFlag1 & 1) == 0) {
        g_BattleSummonOdinZFlag1 |= 1;
        g_pBattleSummonOdinZAfterimageMatrix = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    }
    frameSum = task->frameCount + task->ownerSlot;
    if (frameSum < 15) {
        if (task->frameCount < 9) {
            g_BattleSummonOdinZAnimDef.shade = (task->frameCount << 12) / 9;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    } else {
        task->ownerSlot = BATTLE_TASK_FREE;
        return;
    }
    MultiplyMatrix(&g_BattleSummonOdinZMatrixE0, task->matrix, g_pBattleSummonOdinZAfterimageMatrix);
    SetRotMatrix(g_pBattleSummonOdinZAfterimageMatrix);
    SetTransMatrix(g_pBattleSummonOdinZAfterimageMatrix);
    SetDrawColorRGB(0, 0, 0);
    RefreshGlobalTransformState();
    g_BattleOverlayDrawListHead = (int)BattleFx_SummonOdinZantetsukenDrawPsxMesh((BattleFxZantetsukenMeshBlock *)&g_BattleSummonOdinZAnimDef, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frameCount)++;
    }
}

// Walks textured triangles/quads and gouraud triangles/quads in sixteen/twenty/twenty/twenty-four
// byte mesh records through shared render-state storage. Adds descriptor page offset to masked mesh
// page and packs signed descriptor UV offsets, adding them to record UV values with original
// narrowing. Transforms first three signed-offset vertices and fetches following topology before
// filtering, including after final records. Rejects flags 0x60000; descriptor bit 0x20 bypasses
// orientation, otherwise triangles require positive orientation and other streams permit zero.
// Textured triangles with descriptor bit 0x80 use record colour, optionally shaded, with command
// 0x24; the clear branch uses local t without initializing it for that branch, so its current C
// colour depends on earlier assignment or an uninitialized local. Textured quads use record colour
// under bit 0x80 or descriptor grey otherwise, optionally shading the record-colour branch, with
// command 0x2C. Quad field six supplies the fourth vertex through
// SetPendingResourceHandle/ApplyPendingResourceHandle. Gouraud streams shade three/four colours
// when descriptor shade is nonzero or copy record colours directly otherwise. Descriptor bit eight
// ORs two into command. Submits accepted primitives and returns shared render-state pointer even
// for empty streams.
// FUNCTION: FF7 0x004a7bf9
int __cdecl BattleFx_SummonOdinZantetsukenDrawPsxMesh(BattleFxZantetsukenMeshBlock *meshBlock, int orderingTable, int depthShift, int packetCursor)
{
    int origin;
    unsigned short localTexPageWord;
    int vert1;
    int color[2];
    int orientation;
    int i;
    BattleFxOdinMeshPrim *recordPointer;
    int queryDepth;
    BattleFxOdinQuadPrim *texQuadOut;
    int offsetXy;
    int padBlock; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int vertA;
    int count;
    int vertC;
    BattleFxOdinQuadPacket *gouraudTriPrim;
    BattleFxOdinQuadPacket *gouraudQuad;
    int rejectFlags;
    int unusedEntry; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxOdinQuadPrim *textureTriPrim;

    origin = meshBlock->meshData + 4;
    recordPointer = (BattleFxOdinMeshPrim *)(origin + *(int *)meshBlock->meshData);
    offsetXy = meshBlock->offsetY << 0x10 | meshBlock->offsetX;
    textureTriPrim = (BattleFxOdinQuadPrim *)Gfx_GetPsxPacketScratchPtr();
    count = recordPointer->count;
    recordPointer = (BattleFxOdinMeshPrim *)((int)recordPointer + 2);
    localTexPageWord = (recordPointer->texPage & 0x19F) + meshBlock->texPageOffset;
    recordPointer = (BattleFxOdinMeshPrim *)((int)recordPointer + 2);
    vertA = origin + recordPointer->texTri.vertexA;
    vert1 = origin + recordPointer->texTri.vertexB;
    vertC = origin + recordPointer->texTri.vertexC;
    for (i = 0; i < count; i++) {
        StoreDeferredDrawArgs((void *)vertA, (void *)vert1, (void *)vertC);
        ApplyDeferredDrawArgs();
        vertA = origin + recordPointer->texTris[1].vertexA;
        vert1 = origin + recordPointer->texTris[1].vertexB;
        vertC = origin + recordPointer->texTris[1].vertexC;
        Gfx_PsxStoreClearFlag(&rejectFlags);
        if ((rejectFlags & 0x60000) == 0) {
            if ((meshBlock->flags & 0x20) != 0) {
                orientation = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&orientation);
            }
            if (orientation > 0) {
                textureTriPrim->tag = 0x7000000;
                Gfx_ReadTransformOrderingDepth2(&queryDepth);
                textureTriPrim->xy0 = recordPointer->texTri.uv0 + offsetXy;
                textureTriPrim->xy1 = recordPointer->texTri.uv1 + offsetXy;
                textureTriPrim->xy2 = recordPointer->texTri.uv2 + offsetXy;
                textureTriPrim->texPage = localTexPageWord;
                if ((meshBlock->flags & 0x80) != 0) {
                    color[0] = recordPointer->texTri.colorIndex & 0xFF;
                    color[0] = color[0] << 0x10 | 0x24000000 | color[0] << 8 | color[0];
                    if (meshBlock->shade != 0) {
                        Gfx_ShadePsxVertexColor((CVECTOR *)color, meshBlock->shade, (CVECTOR *)&textureTriPrim->colorCmd);
                        textureTriPrim->primCode = 0x24;
                    } else {
                        textureTriPrim->colorCmd = color[0];
                    }
                } else {
                    textureTriPrim->colorCmd = color[0] << 0x10 | 0x24000000 | color[0] << 8 | color[0];
                }
                if ((meshBlock->flags & 8) != 0) {
                    textureTriPrim->primCode |= 2;
                }
                Gfx_DrawPsxPrimitive(textureTriPrim);
            }
        }
        recordPointer = (BattleFxOdinMeshPrim *)((int)recordPointer + 0x10);
    }
    count = recordPointer->count;
    recordPointer = (BattleFxOdinMeshPrim *)((int)recordPointer + 4);
    texQuadOut = (BattleFxOdinQuadPrim *)textureTriPrim;
    vertA = origin + recordPointer->texQuad.vertexA;
    vert1 = origin + recordPointer->texQuad.vertexB;
    vertC = origin + recordPointer->texQuad.vertexC;
    for (i = 0; i < count; i++) {
        StoreDeferredDrawArgs((void *)vertA, (void *)vert1, (void *)vertC);
        ApplyDeferredDrawArgs();
        vertA = origin + recordPointer->texQuads[1].vertexA;
        vert1 = origin + recordPointer->texQuads[1].vertexB;
        vertC = origin + recordPointer->texQuads[1].vertexC;
        Gfx_PsxStoreClearFlag(&rejectFlags);
        if ((rejectFlags & 0x60000) == 0) {
            if ((meshBlock->flags & 0x20) != 0) {
                orientation = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&orientation);
            }
            if (orientation >= 0) {
                SetPendingResourceHandle((GfxPsxShortVertexSlot *)(origin + recordPointer->texQuad.vertexD));
                ApplyPendingResourceHandle();
                Gfx_ReadTransformOrderingDepth2(&queryDepth);
                texQuadOut->tag = 0x9000000;
                texQuadOut->xy0 = recordPointer->texQuad.uv0 + offsetXy;
                texQuadOut->xy1 = recordPointer->texQuad.uv1 + offsetXy;
                texQuadOut->xy2 = recordPointer->texQuad.uv2 + offsetXy;
                texQuadOut->xy3 = recordPointer->texQuad.uv3 + offsetXy;
                texQuadOut->texPage = localTexPageWord;
                if ((meshBlock->flags & 0x80) != 0) {
                    color[0] = recordPointer->texQuad.colorIndex & 0xFF;
                    color[0] = color[0] << 0x10 | 0x2C000000 | color[0] << 8 | color[0];
                    if (meshBlock->shade != 0) {
                        Gfx_ShadePsxVertexColor((CVECTOR *)color, meshBlock->shade, (CVECTOR *)&texQuadOut->colorCmd);
                        texQuadOut->primCode = 0x2C;
                    } else {
                        texQuadOut->colorCmd = color[0];
                    }
                } else {
                    color[0] = meshBlock->shade & 0xFF;
                    texQuadOut->colorCmd = color[0] << 0x10 | 0x2C000000 | color[0] << 8 | color[0];
                }
                if ((meshBlock->flags & 8) != 0) {
                    texQuadOut->primCode |= 2;
                }
                Gfx_DrawPsxPrimitive(texQuadOut);
            }
        }
        recordPointer = (BattleFxOdinMeshPrim *)((int)recordPointer + 0x14);
    }
    count = recordPointer->count;
    recordPointer = (BattleFxOdinMeshPrim *)((int)recordPointer + 4);
    gouraudTriPrim = (BattleFxOdinQuadPacket *)texQuadOut;
    vertA = origin + recordPointer->gouraudTri.vertexA;
    vert1 = origin + recordPointer->gouraudTri.vertexB;
    vertC = origin + recordPointer->gouraudTri.vertexC;
    for (i = 0; i < count; i++) {
        StoreDeferredDrawArgs((void *)vertA, (void *)vert1, (void *)vertC);
        ApplyDeferredDrawArgs();
        vertA = origin + recordPointer->gouraudTris[1].vertexA;
        vert1 = origin + recordPointer->gouraudTris[1].vertexB;
        vertC = origin + recordPointer->gouraudTris[1].vertexC;
        Gfx_PsxStoreClearFlag(&rejectFlags);
        if ((rejectFlags & 0x60000) == 0) {
            if ((meshBlock->flags & 0x20) != 0) {
                orientation = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&orientation);
            }
            if (orientation >= 0) {
                gouraudTriPrim->tag = 0x6000000;
                Gfx_ReadTransformOrderingDepth2(&queryDepth);
                if (meshBlock->shade != 0) {
                    Gfx_ShadePsxTriVertexColors(&recordPointer->gouraudTri.color0Vec, &recordPointer->gouraudTri.color1Vec, &recordPointer->gouraudTri.color2Vec, meshBlock->shade, &gouraudTriPrim->colorCmdVec, &gouraudTriPrim->color1Vec, &gouraudTriPrim->color2Vec);
                } else {
                    gouraudTriPrim->colorCmd = recordPointer->gouraudTri.color0;
                    gouraudTriPrim->color1 = recordPointer->gouraudTri.color1;
                    gouraudTriPrim->color2 = recordPointer->gouraudTri.color2;
                }
                if ((meshBlock->flags & 8) != 0) {
                    gouraudTriPrim->primCode |= 2;
                }
                Gfx_DrawPsxPrimitive(gouraudTriPrim);
            }
        }
        recordPointer = (BattleFxOdinMeshPrim *)((int)recordPointer + 0x14);
    }
    count = recordPointer->count;
    recordPointer = (BattleFxOdinMeshPrim *)((int)recordPointer + 4);
    gouraudQuad = (BattleFxOdinQuadPacket *)gouraudTriPrim;
    vertA = origin + recordPointer->gouraudQuad.vertexA;
    vert1 = origin + recordPointer->gouraudQuad.vertexB;
    vertC = origin + recordPointer->gouraudQuad.vertexC;
    for (i = 0; i < count; i++) {
        StoreDeferredDrawArgs((void *)vertA, (void *)vert1, (void *)vertC);
        ApplyDeferredDrawArgs();
        vertA = origin + recordPointer->gouraudQuads[1].vertexA;
        vert1 = origin + recordPointer->gouraudQuads[1].vertexB;
        vertC = origin + recordPointer->gouraudQuads[1].vertexC;
        Gfx_PsxStoreClearFlag(&rejectFlags);
        if ((rejectFlags & 0x60000) == 0) {
            if ((meshBlock->flags & 0x20) != 0) {
                orientation = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&orientation);
            }
            if (orientation >= 0) {
                SetPendingResourceHandle((GfxPsxShortVertexSlot *)(origin + recordPointer->gouraudQuad.vertexD));
                ApplyPendingResourceHandle();
                Gfx_ReadTransformOrderingDepth2(&queryDepth);
                gouraudQuad->tag = 0x8000000;
                if (meshBlock->shade != 0) {
                    Gfx_ShadePsxTriVertexColors(&recordPointer->gouraudQuad.color0Vec, &recordPointer->gouraudQuad.color1Vec, &recordPointer->gouraudQuad.color2Vec, meshBlock->shade, &gouraudQuad->colorCmdVec, &gouraudQuad->color1Vec, &gouraudQuad->color2Vec);
                    Gfx_ShadePsxVertexColor(&recordPointer->gouraudQuad.color3Vec, meshBlock->shade, &gouraudQuad->color3Vec);
                } else {
                    gouraudQuad->colorCmd = recordPointer->gouraudQuad.color0;
                    gouraudQuad->color1 = recordPointer->gouraudQuad.color1;
                    gouraudQuad->color2 = recordPointer->gouraudQuad.color2;
                    gouraudQuad->color3 = recordPointer->gouraudQuad.color3;
                }
                if ((meshBlock->flags & 8) != 0) {
                    gouraudQuad->primCode |= 2;
                }
                Gfx_DrawPsxPrimitive(gouraudQuad);
            }
        }
        recordPointer = (BattleFxOdinMeshPrim *)((int)recordPointer + 0x18);
    }
    return (int)gouraudQuad;
}

// Acquires retained scratch once, then returns while paused. Gets actor-three attachment eighteen,
// composes matrix g_BattleSummonOdinZMatrixE0 relative to camera, installs it and transforms two retained points.
// If task sample count is zero, stores the two transformed XYZ groups into the sixteen-byte sample
// bank and advances sample count. Otherwise copies the preceding sample into scratch and generates
// four quarter-step interpolated endpoint pairs. For each calls
// BattleFx_SummonOdinZantetsukenOffsetPointAlongLine with 0x578, adds its offset once to the first
// endpoint and twice to the second, stores the pair and allocates
// BattleFx_SummonOdinZantetsukenTrailSegmentQuadTask with sequence index q plus task counter times
// four minus one, countdown four minus q and pointer to that sample; advances sample count each
// time. Advances task counter after either branch and sets sample count to signed -1 once counter
// reaches nine.
// FUNCTION: FF7 0x004a8431
void __cdecl BattleFx_SummonOdinZantetsukenSpawnTrailSegmentsTask(void)
{
    BattleFxOdinZTrailSpawner *task;
    int fixedFraction;
    int quarter;
    BattleFxOdinZantetsukenTrailSpawnTask *childTask;

    task = (BattleFxOdinZTrailSpawner *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonOdinZByteC8 & 1) == 0) {
        g_BattleSummonOdinZByteC8 |= 1;
        g_BattleSummonOdinZTrailDataBlock = (BattleFxOdinTrailSegment *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    Battle3d_GetActorAttachMatrix(3, 18, (PsxMatrix *)&g_BattleActorRecords[3].boneNodes5[0x2a4]);
    Battle_ComposeMatrixRelativeToCamera((short *)&g_BattleSummonOdinZMatrixE0, (void *)&g_BattleActorRecords[3].boneNodes5[0x2a4], &g_BattleSummonOdinZTrailDataBlock->matrix);
    SetRotMatrix(&g_BattleSummonOdinZTrailDataBlock->matrix);
    SetTransMatrix(&g_BattleSummonOdinZTrailDataBlock->matrix);
    RotTransPoint(&g_BattleSummonOdinZInitialY, &g_BattleSummonOdinZTrailDataBlock->sourceX, &g_BattleSummonOdinZTrailDataBlock->flags);
    RotTransPoint(&g_BattleSummonOdinZLimitY, &g_BattleSummonOdinZTrailDataBlock->sourceX2, &g_BattleSummonOdinZTrailDataBlock->flags);
    if (task->recordIndex != 0) {
        *(Int64Pair *)&g_BattleSummonOdinZTrailDataBlock->prevX = *(Int64Pair *)&g_BattleSummonOdinZRecordBuffer1[task->recordIndex - 1].posX;
        *(Int64Pair *)&g_BattleSummonOdinZTrailDataBlock->prevX2 = *(Int64Pair *)&g_BattleSummonOdinZRecordBuffer1[task->recordIndex - 1].trailX;
        for (quarter = 1; quarter <= 4; quarter++) {
            fixedFraction = (quarter << 12) / 4;
            g_BattleSummonOdinZTrailDataBlock->currentX = g_BattleSummonOdinZTrailDataBlock->sourceX * fixedFraction + g_BattleSummonOdinZTrailDataBlock->prevX * (4096 - fixedFraction) >> 12;
            g_BattleSummonOdinZTrailDataBlock->currentY = g_BattleSummonOdinZTrailDataBlock->sourceY * fixedFraction + g_BattleSummonOdinZTrailDataBlock->prevY * (4096 - fixedFraction) >> 12;
            g_BattleSummonOdinZTrailDataBlock->currentZ = g_BattleSummonOdinZTrailDataBlock->sourceZ * fixedFraction + g_BattleSummonOdinZTrailDataBlock->prevZ * (4096 - fixedFraction) >> 12;
            g_BattleSummonOdinZTrailDataBlock->currentX2 = g_BattleSummonOdinZTrailDataBlock->sourceX2 * fixedFraction + g_BattleSummonOdinZTrailDataBlock->prevX2 * (4096 - fixedFraction) >> 12;
            g_BattleSummonOdinZTrailDataBlock->currentY2 = g_BattleSummonOdinZTrailDataBlock->sourceY2 * fixedFraction + g_BattleSummonOdinZTrailDataBlock->prevY2 * (4096 - fixedFraction) >> 12;
            g_BattleSummonOdinZTrailDataBlock->currentZ2 = g_BattleSummonOdinZTrailDataBlock->sourceZ2 * fixedFraction + g_BattleSummonOdinZTrailDataBlock->prevZ2 * (4096 - fixedFraction) >> 12;
            BattleFx_SummonOdinZantetsukenOffsetPointAlongLine(&g_BattleSummonOdinZTrailDataBlock->current, &g_BattleSummonOdinZTrailDataBlock->current2, 1400, &g_BattleSummonOdinZTrailDataBlock->trail);
            g_BattleSummonOdinZRecordBuffer1[task->recordIndex].posX = g_BattleSummonOdinZTrailDataBlock->currentX + g_BattleSummonOdinZTrailDataBlock->trailX;
            g_BattleSummonOdinZRecordBuffer1[task->recordIndex].posY = g_BattleSummonOdinZTrailDataBlock->currentY + g_BattleSummonOdinZTrailDataBlock->trailY;
            g_BattleSummonOdinZRecordBuffer1[task->recordIndex].posZ = g_BattleSummonOdinZTrailDataBlock->currentZ + g_BattleSummonOdinZTrailDataBlock->trailZ;
            g_BattleSummonOdinZRecordBuffer1[task->recordIndex].trailX = g_BattleSummonOdinZTrailDataBlock->currentX2 + g_BattleSummonOdinZTrailDataBlock->trailX * 2;
            g_BattleSummonOdinZRecordBuffer1[task->recordIndex].trailY = g_BattleSummonOdinZTrailDataBlock->currentY2 + g_BattleSummonOdinZTrailDataBlock->trailY * 2;
            g_BattleSummonOdinZRecordBuffer1[task->recordIndex].trailZ = g_BattleSummonOdinZTrailDataBlock->currentZ2 + g_BattleSummonOdinZTrailDataBlock->trailZ * 2;
            childTask = (BattleFxOdinZantetsukenTrailSpawnTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonOdinZantetsukenTrailSegmentQuadTask);
            childTask->endIndex = quarter + task->segmentCount * 4 + -1;
            childTask->frameCounter = 4 - quarter;
            childTask->record = &g_BattleSummonOdinZRecordBuffer1[task->recordIndex];
            (task->recordIndex)++;
        }
    } else {
        g_BattleSummonOdinZRecordBuffer1[task->recordIndex].posX = g_BattleSummonOdinZTrailDataBlock->sourceXShort;
        g_BattleSummonOdinZRecordBuffer1[task->recordIndex].posY = g_BattleSummonOdinZTrailDataBlock->sourceYShort;
        g_BattleSummonOdinZRecordBuffer1[task->recordIndex].posZ = g_BattleSummonOdinZTrailDataBlock->sourceZShort;
        g_BattleSummonOdinZRecordBuffer1[task->recordIndex].trailX = g_BattleSummonOdinZTrailDataBlock->sourceX2Short;
        g_BattleSummonOdinZRecordBuffer1[task->recordIndex].trailY = g_BattleSummonOdinZTrailDataBlock->sourceY2Short;
        g_BattleSummonOdinZRecordBuffer1[task->recordIndex].trailZ = g_BattleSummonOdinZTrailDataBlock->sourceZ2Short;
        (task->recordIndex)++;
    }
    (task->segmentCount)++;
    if (task->segmentCount >= 9) {
        task->recordIndex = 0xFFFF;
    }
}

// Takes the vector from the short triple at startPoint to the one at endPoint, normalizes it with
// NormalizeVectorFixed, and writes startPoint + unit * distance (Q12, >> 12) - endPoint into three shorts at
// outPoint, that is the point distance fixed-point units along the line from startPoint toward endPoint, expressed
// relative to endPoint. The function normalizes in place into the first component's slot and reads
// the other two components from the adjacent stack slots.
// FUNCTION: FF7 0x004a8904
void __cdecl BattleFx_SummonOdinZantetsukenOffsetPointAlongLine(Vec3sPacked *startPoint, Vec3sPacked *endPoint, int distance, Vec3sPacked *outPoint)
{
    VECTOR moveDir;

    moveDir.vx = endPoint->vx - startPoint->vx;
    moveDir.vy = endPoint->vy - startPoint->vy;
    moveDir.vz = endPoint->vz - startPoint->vz;
    NormalizeVectorFixed(&moveDir, &moveDir);
    outPoint->vx = startPoint->vx + (moveDir.vx * distance >> 12) - endPoint->vx;
    outPoint->vy = startPoint->vy + (moveDir.vy * distance >> 12) - endPoint->vy;
    outPoint->vz = startPoint->vz + (moveDir.vz * distance >> 12) - endPoint->vz;
}

// Uses task counter plus saved sequence index to select a thirty-six-count colour ramp followed by
// a twenty-four-count hold; retires before drawing once combined time reaches sixty, with an
// additional counter-at-least-thirty-six check in the first branch. Computes two adjacent
// fixed-point colour fractions from counter, freezing them during the hold by subtracting elapsed
// hold time. Installs matrix g_BattleSummonOdinZMatrixE0, refreshes and projects four endpoints
// spanning the preceding and current sixteen-byte sample records. If flags 0x60000 are clear,
// writes an eight-word gouraud quad with first two colours shaded at the upper fraction and command
// 0x3A, last two shaded at the lower fraction, submits and chains g_BattleOverlayDrawListHead. Only
// counter advancement by four is pause-gated.
// FUNCTION: FF7 0x004a89bb
void __cdecl BattleFx_SummonOdinZantetsukenTrailSegmentQuadTask(void)
{
    BattleFxTrailQuadPacket *primitivePtr;
    int bits;
    BattleFxOdinZantetsukenSegment *taskSlotPtr;
    int combined;
    int otz;
    int upper;
    int shade;
    int lowerFraction;
    int avgZ;

    taskSlotPtr = (BattleFxOdinZantetsukenSegment *)BATTLE_CURRENT_TASK_SLOT;
    combined = taskSlotPtr->step + taskSlotPtr->ownerSlot;
    if (combined < 36) {
        if (taskSlotPtr->step >= 36) {
            taskSlotPtr->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
        upper = (taskSlotPtr->step + 1 << 12) / 36;
        lowerFraction = (taskSlotPtr->step << 12) / 36;
    } else {
        combined -= 0x24;
        if (combined < 24) {
            upper = (taskSlotPtr->step - combined + 1 << 12) / 36;
            lowerFraction = (taskSlotPtr->step - combined << 12) / 36;
        } else {
            taskSlotPtr->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    SetRotMatrix(&g_BattleSummonOdinZMatrixE0);
    SetTransMatrix(&g_BattleSummonOdinZMatrixE0);
    RefreshGlobalTransformState();
    primitivePtr = (BattleFxTrailQuadPacket *)Gfx_GetPsxPacketScratchPtr();
    avgZ = Gfx_TransformQuadReturnDepth(&taskSlotPtr->record[-1].pos, &taskSlotPtr->record[-1].trail, &taskSlotPtr->record[0].pos, &taskSlotPtr->record[0].trail, &otz, &bits);
    if ((bits & 0x60000) == 0) {
        primitivePtr->tag = 0x8000000;
        Gfx_ShadePsxVertexColor(g_BattleSummonOdinZOffsetTable, upper, (CVECTOR *)&shade);
        primitivePtr->colorCommand = shade | 0x3A000000;
        primitivePtr->colorWord = primitivePtr->colorCommand;
        Gfx_ShadePsxVertexColor(g_BattleSummonOdinZOffsetTable, lowerFraction, (CVECTOR *)&shade);
        primitivePtr->color3 = shade;
        primitivePtr->color2 = shade;
        Gfx_DrawPsxPrimitive(primitivePtr);
        g_BattleOverlayDrawListHead = (int)primitivePtr;
    }
    if (g_BattleIsPaused == 0) {
        taskSlotPtr->step += 4;
    }
}

// Keep this reconstructed function compact so later VC5/PDB markers stay below line 65536.
// clang-format off
// Acquires retained scratch once and multiplies g_BattleCameraTransform with g_BattleSummonOdinZMatrixD8 into g_BattleSummonOdinZMatrixE0
// before pause return. Retains a constant-false offscreen branch. Unpaused timeline runs phases of
// ten, fifty, twenty-five, forty, twenty, twenty-five, thirty-five, twenty-five, fifteen and
// fifteen counts, then the signed g_BattleSummonOdinZFrames hold and final fifteen. First phase clears screen
// RGB and starts 4096 ramp for ten, setting low flag nibble at nine. Fifty phase schedules
// BattleFx_SummonOdinZantetsukenFadeInTripleSpriteTask/004a942f at (0,0,-5000) and dispatches sound group zero. Forty phase schedules
// BattleFx_SummonOdinZantetsukenStaticModelTask and BattleFx_SummonOdinZantetsukenScrollingTripleSpriteTask at (0,-70,-904); following twenty-five phase dispatches group one.
// Thirty-five phase masks flags with 0xF0 and ramps to zero. Next three phases dispatch group two
// and release sound triples 0x239..0x23B slot zero at ten, 0x23C..0x23E slot one at entry and
// 0x247..0x249 slot two at entry. At relative eighteen/eight/eight schedules
// BattleFx_SummonOdinZantetsukenScreenFlashPulseTask and calls
// BattleFx_SummonOdinZantetsukenToggleMaskedActorBit1, repeating helper at each last count. Last
// fifteen-count phase also clears RGB, sets magnitude 0x800 and schedules
// BattleFx_SummonOdinZantetsukenTargetSequenceTask at its final count. Parameterized hold clears
// magnitude at its last count; final phase dispatches group four on entry. Later retires,
// conditionally releases/reset offscreen slot and releases sounds 0x1A3 slot fifteen and
// 0x24A..0x24C slot four, then still increments task counter.
// FUNCTION: FF7 0x004a8b86
void __cdecl BattleFx_SummonOdinZantetsukenTimelineTask(void)
{
    int p;
    int spareValue[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxOdinSpriteSpawnSlot *r;
    int s;
    int t;
    int u;
    int v;
    int w;
    int x;
    int y;
    int z;
    BattleTaskSlot *a;

    a = BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonOdinZTimelineScratchReady[0] & 1) == 0) {
        g_BattleSummonOdinZTimelineScratchReady[0] |= 1; g_BattleSummonOdinZTimelineScratch = Gfx_GetPsxScratchpadPtr(0);
    }
    MultiplyMatrix(&g_BattleCameraTransform, &g_BattleSummonOdinZMatrixD8, &g_BattleSummonOdinZMatrixE0);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (DEAD_IN_RETAIL != 0) {
        w = g_BattleSummonOdinZDrawBufferCurrent % 512; g_BattleSummonOdinZDrawBufferCurrent++; x = 0; s = 0x100; z = 256; u = 0; v = 0; y = 0; t = 0xFFFFFFFF;
        if (Battle3d_GetOffscreenRenderObject(g_BattleSummonOdinZState) != 0) {
            Gfx_EmitPsxTexturedSpriteQuad(w, x, s, z, u, v, y, FLOAT_ONE_BITS, t, 0, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleSummonOdinZState));
        }
    }
    p = a->frameCounter;
    if (p < 10) {
        if (p == 0) {
            g_BattleFxTimelineState.red = 0; g_BattleFxTimelineState.channel1 = 0; g_BattleFxTimelineState.channel2 = 0; Battle_StartFixedPointRamp(0x1000, 10);
        }
        if (p == 9) {
            g_BattleStageHideFlags |= 0xF;
        }
    } else {
        p -= 0xA;
        if (p < 50) {
            if (p == 0) {
                r = (BattleFxOdinSpriteSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonOdinZantetsukenFadeInTripleSpriteTask); r->posX = 0; r->posY = 0;
                r->posZ = 0xEC78; r = (BattleFxOdinSpriteSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonOdinZantetsukenStaticSpriteTask); r->posX = 0;
                r->posY = 0; r->posZ = 0xEC78;
            }
            if (p == 0) {
                Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 0);
            }
        } else {
            p -= 0x32;
            if (p < 25) { /* empty first arm: lib/codegen.h */
            } else {
                p -= 0x19;
                if (p < 40) {
                    if (p == 0) {
                        r = (BattleFxOdinSpriteSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonOdinZantetsukenStaticModelTask);
                        r = (BattleFxOdinSpriteSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonOdinZantetsukenScrollingTripleSpriteTask); r->posX = 0; r->posY = 0xFFBA;
                        r->posZ = 0xFC78;
                    }
                } else {
                    p -= 0x28;
                    if (p < 20) { /* empty first arm: lib/codegen.h */
                    } else {
                        p -= 0x14;
                        if (p < 25) {
                            if (p == 0) {
                                Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 1);
                            }
                        } else {
                            p -= 0x19;
                            if (p < 35) {
                                if (p == 0) {
                                    g_BattleStageHideFlags &= 0xF0; Battle_StartFixedPointRamp(0, 35);
                                }
                            } else {
                                p -= 0x23;
                                if (p < 25) {
                                    if (p == 0x12) {
                                        Battle_AllocateResourceSlot(BattleFx_SummonOdinZantetsukenScreenFlashPulseTask); BattleFx_SummonOdinZantetsukenToggleMaskedActorBit1();
                                    }
                                    if (p == 0x18) {
                                        BattleFx_SummonOdinZantetsukenToggleMaskedActorBit1();
                                    }
                                    if (p == 0xA) {
                                        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 2); Sfx_ReleaseCachedBuffer(0x239, 0); Sfx_ReleaseCachedBuffer(0x23A, 0);
                                        Sfx_ReleaseCachedBuffer(0x23B, 0);
                                    }
                                } else {
                                    p -= 0x19;
                                    if (p < 15) {
                                        if (p == 8) {
                                            Battle_AllocateResourceSlot(BattleFx_SummonOdinZantetsukenScreenFlashPulseTask); BattleFx_SummonOdinZantetsukenToggleMaskedActorBit1();
                                        }
                                        if (p == 0xE) {
                                            BattleFx_SummonOdinZantetsukenToggleMaskedActorBit1();
                                        }
                                        if (p == 0) {
                                            Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 2); Sfx_ReleaseCachedBuffer(0x23C, 1); Sfx_ReleaseCachedBuffer(0x23D, 1);
                                            Sfx_ReleaseCachedBuffer(0x23E, 1);
                                        }
                                    } else {
                                        p -= 0xF;
                                        if (p < 15) {
                                            if (p == 8) {
                                                Battle_AllocateResourceSlot(BattleFx_SummonOdinZantetsukenScreenFlashPulseTask); BattleFx_SummonOdinZantetsukenToggleMaskedActorBit1();
                                            }
                                            if (p == 0xE) {
                                                g_BattleFxTimelineState.red = 0; g_BattleFxTimelineState.channel1 = 0; g_BattleFxTimelineState.channel2 = 0; g_BattleFxTimelineState.startValue = 0x800;
                                                r = (BattleFxOdinSpriteSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonOdinZantetsukenTargetSequenceTask); r->bitIndex = 0; BattleFx_SummonOdinZantetsukenToggleMaskedActorBit1();
                                            }
                                            if (p == 0) {
                                                Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 2); Sfx_ReleaseCachedBuffer(0x247, 2); Sfx_ReleaseCachedBuffer(0x248, 2);
                                                Sfx_ReleaseCachedBuffer(0x249, 2);
                                            }
                                        } else {
                                            p -= 0xF;
                                            if (p < 0) { /* empty first arm: lib/codegen.h */
                                            } else {
                                                p = p;
                                                if (p < g_BattleSummonOdinZFrames) {
                                                    if (p == g_BattleSummonOdinZFrames - 1) {
                                                        g_BattleFxTimelineState.startValue = 0;
                                                    }
                                                } else {
                                                    p -= g_BattleSummonOdinZFrames;
                                                    if (p < 15) {
                                                        if (p == 0) {
                                                            Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_3, 0x40, 4);
                                                        }
                                                    } else {
                                                        a->ownerSlot = BATTLE_TASK_FREE;
                                                        if (g_BattleSummonOdinZState >= 0) {
                                                            Battle3d_ReleaseOffscreenSlot(g_BattleSummonOdinZState); g_BattleSummonOdinZState = 0xFFFFFFFF;
                                                        }
                                                        Sfx_ReleaseCachedBuffer(0x1A3, 0xF); Sfx_ReleaseCachedBuffer(0x24A, 4); Sfx_ReleaseCachedBuffer(0x24B, 4);
                                                        Sfx_ReleaseCachedBuffer(0x24C, 4);
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
    (a->frameCounter)++;
}

// clang-format on

// Refreshes Odin-two resource field one and applies helper value 0x10000. Initializes retained
// offset 0x400 whenever task count is zero, including paused calls. Ramps equal descriptor RGB
// toward 128 over ten counts, then holds packed 0x2E808080 for 125; retains the unused fraction
// calculation in the hold. Following twenty-five counts reset task XYZ to (0,-4900,-7692) on entry;
// later retires before draw/pause. Calls BattleFx_SummonOdinZantetsukenInstallBillboardMatrix with
// task point, 0x2000 and zero, adds retained offset to the matrix's t[0] and draws resource
// three times at destination plus 0x4068, shift zero: initial translation, minus 4096, then plus
// 8192. Only counter increment and offset decrement sixteen are pause-gated.
// FUNCTION: FF7 0x004a911a
void __cdecl BattleFx_SummonOdinZantetsukenFadeInTripleSpriteTask(void)
{
    int frameIndex;
    BattleFxLimitCosmoMemoryGrowState *taskPtr;
    int frac;
    PsxMatrix *matrix;
    int flagVal;

    taskPtr = (BattleFxLimitCosmoMemoryGrowState *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSummonOdinZRefVector.resourceField) = Gfx_ModelTableGetLoadedRecord(1, g_BattleSummonOdinZLoadedResTable);
    g_BattleSummonOdinZRefVector.drawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleSummonOdinZLoadedResTable);
    flagVal = 0x10000;
    Gfx_OrRenderObjectPolygonSetFlags(flagVal, g_BattleSummonOdinZRefVector.drawObj);
    if (taskPtr->frameIndex == 0) {
        g_BattleSummonOdinZScale = 0x400 /* 0.25 */;
    }
    frameIndex = taskPtr->frameIndex;
    if (frameIndex < 10) {
        frac = (frameIndex << 12) / 10;
        g_BattleSummonOdinZRefVector.color.b = frac << 7 >> 12;
        g_BattleSummonOdinZRefVector.color.g = g_BattleSummonOdinZRefVector.color.b;
        g_BattleSummonOdinZRefVector.color.r = g_BattleSummonOdinZRefVector.color.g;
    } else {
        frameIndex -= 10;
        if (frameIndex < 125) {
            frac = (frameIndex << 12) / 125;
            g_BattleSummonOdinZRefVector.colorWord = 0x2E808080;
        } else {
            frameIndex -= 125;
            if (frameIndex < 25) {
                if (frameIndex == 0) {
                    taskPtr->posX = 0;
                    taskPtr->posY = 0xECDC;
                    taskPtr->posZ = 0xE1F4;
                }
            } else {
                taskPtr->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    matrix = (PsxMatrix *)BattleFx_SummonOdinZantetsukenInstallBillboardMatrix((char *)taskPtr + 4, 0x2000, 0);
    matrix->t[0] += g_BattleSummonOdinZScale;
    SetTransMatrix(matrix);
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSummonOdinZRefVector, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST_1, 0, g_BattleOverlayDrawListHead);
    matrix->t[0] -= 0x1000;
    SetTransMatrix(matrix);
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSummonOdinZRefVector, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST_1, 0, g_BattleOverlayDrawListHead);
    matrix->t[0] += 0x2000;
    SetTransMatrix(matrix);
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSummonOdinZRefVector, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST_1, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (taskPtr->frameIndex)++;
        g_BattleSummonOdinZScale -= 0x10;
    }
}

// Stores value as the short at the start of g_BattleSummonOdinZSpriteReq1, then copies it to
// g_BattleSummonOdinZTimer1 and g_BattleSummonOdinZBillboardMatrix's m[0][0]; loads the
// rotation and translation matrix from g_BattleSummonOdinZMatrixE0 and RotTransPoint(point, ...)
// transforms the point into the three ints that follow the short in g_BattleSummonOdinZSpriteReq1; when
// distance is nonzero, adds unit-vector * distance >> 12 to each (NormalizeVectorFixed); finally
// sets the rotation and translation matrix from g_BattleSummonOdinZBillboardMatrix and returns its address.
// FUNCTION: FF7 0x004a9354
int __cdecl BattleFx_SummonOdinZantetsukenInstallBillboardMatrix(void *point, int value, int distance)
{
    int depth;
    VECTOR normal;

    *(short *)&g_BattleSummonOdinZSpriteReq1 = (short)value;
    g_BattleSummonOdinZTimer1 = *(short *)&g_BattleSummonOdinZSpriteReq1;
    g_BattleSummonOdinZBillboardMatrix[0] = g_BattleSummonOdinZTimer1;
    SetRotMatrix(&g_BattleSummonOdinZMatrixE0);
    SetTransMatrix(&g_BattleSummonOdinZMatrixE0);
    RotTransPoint(point, (char *)&g_BattleSummonOdinZSpriteReq1 + 2, &depth);
    if (distance != 0) {
        NormalizeVectorFixed((char *)&g_BattleSummonOdinZSpriteReq1 + 2, &normal);
        *(int *)((char *)&g_BattleSummonOdinZSpriteReq1 + 2) += normal.vx * distance >> 12;
        *(int *)((char *)&g_BattleSummonOdinZSpriteReq1 + 6) += normal.vy * distance >> 12;
        *(int *)((char *)&g_BattleSummonOdinZSpriteReq1 + 10) += normal.vz * distance >> 12;
    }
    SetRotMatrix((PsxMatrix *)g_BattleSummonOdinZBillboardMatrix);
    SetTransMatrix((PsxMatrix *)g_BattleSummonOdinZBillboardMatrix);
    return (int)g_BattleSummonOdinZBillboardMatrix;
}

// Per-frame static sprite task. Loads g_BattleSummonOdinZSpriteDrawRequest.spriteGroup from
// Gfx_ModelTableGetLoadedRecord(2, g_BattleSummonOdinZLoadedResTable) and .drawObj from
// Gfx_ModelTableGetSpriteObject(2, g_BattleSummonOdinZLoadedResTable), ORs 0x10000 into that object's
// flags via Gfx_OrRenderObjectPolygonSetFlags, and once the frame counter reaches 0x87 either
// resets the task position to (0, 0xECDC, 0xE1F4) on the first such frame or, 0x19
// frames later, retires the slot and returns. Otherwise it calls
// BattleFx_SummonOdinZantetsukenInstallBillboardMatrix(task position, 0x2000, 0), submits
// g_BattleSummonOdinZSpriteDrawRequest into ordering table bucket BATTLE_OT_SORTED_LAST, and
// advances the counter when g_BattleIsPaused is clear.
// FUNCTION: FF7 0x004a942f
void __cdecl BattleFx_SummonOdinZantetsukenStaticSpriteTask(void)
{
    int frame;
    int flagBits;
    BattleFxStaticSpriteSlot *task;

    task = (BattleFxStaticSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSummonOdinZSpriteDrawRequest.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(2, g_BattleSummonOdinZLoadedResTable);
    g_BattleSummonOdinZSpriteDrawRequest.drawObj = Gfx_ModelTableGetSpriteObject(2, g_BattleSummonOdinZLoadedResTable);
    flagBits = 0x10000;
    Gfx_OrRenderObjectPolygonSetFlags(flagBits, g_BattleSummonOdinZSpriteDrawRequest.drawObj);
    frame = task->spawnIndex;
    if (frame < 135) { /* empty first arm: lib/codegen.h */
    } else {
        frame = frame - 135;
        if (frame < 25) {
            if (frame == 0) {
                task->posX = 0;
                task->posY = 0xECDC;
                task->posZ = 0xE1F4;
            }
        } else {
            task->step = 0xFFFF;
            return;
        }
    }
    BattleFx_SummonOdinZantetsukenInstallBillboardMatrix(&task->posX, 0x2000, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSummonOdinZSpriteDrawRequest, g_BattleCurrentOrderingTable + BATTLE_OT_SORTED_LAST, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->spawnIndex)++;
    }
}

// Draws the model of the current task. On first use (bit 0 of g_BattleSummonOdinZModelMatrixReady clear) it
// sets that bit and caches Gfx_GetPsxScratchpadPtr(0) in g_pBattleSummonOdinZModelMatrix; then stores
// Gfx_ModelTableGetRenderObject(0, g_BattleSummonOdinZLoadedResTable) in the model handle of
// g_BattleSummonOdinZAnimParams1, composes MultiplyMatrix(&g_BattleSummonOdinZMatrixE0,
// g_BattleSummonOdinZMatrix1, block), installs the block with
// SetRotMatrix/SetTransMatrix and submits g_BattleSummonOdinZAnimParams1 through BattleFx_SubmitModelDraw
// into bucket BATTLE_OT_DEPTH_SORTED at shift 0xC, threading g_BattleOverlayDrawListHead.
// When g_BattleIsPaused is clear it advances frameCounter and frees the slot once it reaches 0x3C.
// FUNCTION: FF7 0x004a953f
void __cdecl BattleFx_SummonOdinZantetsukenStaticModelTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (!(g_BattleSummonOdinZModelMatrixReady & 1)) {
        g_BattleSummonOdinZModelMatrixReady |= 1;
        g_pBattleSummonOdinZModelMatrix = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    }
    g_BattleSummonOdinZAnimParams1.modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSummonOdinZLoadedResTable);
    MultiplyMatrix(&g_BattleSummonOdinZMatrixE0, &g_BattleSummonOdinZMatrix1, g_pBattleSummonOdinZModelMatrix);
    SetRotMatrix(g_pBattleSummonOdinZModelMatrix);
    SetTransMatrix(g_pBattleSummonOdinZModelMatrix);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSummonOdinZAnimParams1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, (int)(g_BattleOverlayDrawListHead));
    if (!g_BattleIsPaused) {
        task->frameCounter += 1;
        if (task->frameCounter >= 60) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

// Refreshes Odin-two resource field three and applies helper value 0x8000. Sets packed black
// command 0x2E000000 for thirty-five counts, ramps equal RGB toward 64 over ten and holds 64 for
// fifteen; later retires before drawing/pause. Calls
// BattleFx_SummonOdinZantetsukenMakeViewSpacePointPushed with task point and zero into matrix
// g_BattleSummonOdinZScrollMatrix, installs rotation and adds the retained offset
// g_BattleSummonOdinZScrollOffset to the matrix's X translation. Draws
// resource three times at destination plus 0x4084 shift zero, adding 0x800 translation between
// draws. Unpaused increments task count and retained offset by eight, subtracting 0x800 whenever it
// reaches at least -0x400.
// FUNCTION: FF7 0x004a9627
void __cdecl BattleFx_SummonOdinZantetsukenScrollingTripleSpriteTask(void)
{
    BattleTaskSlot *taskSlot;
    int cnt;
    int scrollOfs;
    int frac;
    int objectFlags;

    taskSlot = BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSummonOdinZVector2.resourceField) = Gfx_ModelTableGetLoadedRecord(3, g_BattleSummonOdinZLoadedResTable);
    g_BattleSummonOdinZVector2.drawObj = Gfx_ModelTableGetSpriteObject(3, g_BattleSummonOdinZLoadedResTable);
    objectFlags = 0x8000;
    Gfx_OrRenderObjectPolygonSetFlags(objectFlags, g_BattleSummonOdinZVector2.drawObj);
    cnt = taskSlot->frameCounter;
    if (cnt < 35) {
        g_BattleSummonOdinZVector2.colorWord = 0x2E000000;
    } else {
        cnt -= 35;
        if (cnt < 10) {
            frac = (cnt << 12) / 10;
            g_BattleSummonOdinZVector2.color.r = frac << 6 >> 12;
            g_BattleSummonOdinZVector2.color.g = frac << 6 >> 12;
            g_BattleSummonOdinZVector2.color.b = frac << 6 >> 12;
        } else {
            cnt -= 10;
            if (cnt < 15) {
                g_BattleSummonOdinZVector2.color.r = 0x40;
                g_BattleSummonOdinZVector2.color.g = 0x40;
                g_BattleSummonOdinZVector2.color.b = 0x40;
            } else {
                taskSlot->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    BattleFx_SummonOdinZantetsukenMakeViewSpacePointPushed(taskSlot->data, 0, (PsxMatrix *)g_BattleSummonOdinZScrollMatrix);
    SetRotMatrix((PsxMatrix *)g_BattleSummonOdinZScrollMatrix);
    scrollOfs = g_BattleSummonOdinZVectorC0.translationX;
    g_BattleSummonOdinZVectorC0.translationX = scrollOfs + g_BattleSummonOdinZScrollOffset;
    SetTransMatrix((PsxMatrix *)g_BattleSummonOdinZScrollMatrix);
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSummonOdinZVector2, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_BattleOverlayDrawListHead);
    g_BattleSummonOdinZVectorC0.translationX += 2048;
    SetTransMatrix((PsxMatrix *)g_BattleSummonOdinZScrollMatrix);
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSummonOdinZVector2, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_BattleOverlayDrawListHead);
    g_BattleSummonOdinZVectorC0.translationX += 2048;
    SetTransMatrix((PsxMatrix *)g_BattleSummonOdinZScrollMatrix);
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSummonOdinZVector2, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (taskSlot->frameCounter)++;
        g_BattleSummonOdinZScrollOffset += 8;
        if (g_BattleSummonOdinZScrollOffset >= -0x400) {
            g_BattleSummonOdinZScrollOffset -= 0x800;
        }
    }
}

// Builds the view-space point for the Zantetsuken effect. It loads the rotation and translation
// matrix from g_BattleSummonOdinZMatrixE0, then RotTransPoint transforms point into the three ints
// in target->t. When distance is nonzero, it normalizes that vector with NormalizeVectorFixed and
// adds the unit vector times distance (Q12, >> 12) to target->t[0], t[1] and t[2]. The second and
// third components are read from the stack slots dirY and unitZ beside the normalize output, not
// from its result. An unreferenced padding slot (unusedSpace) preserves VC5 /Od stack layout. Returns
// target unchanged.
// FUNCTION: FF7 0x004a9869
PsxMatrix *__cdecl BattleFx_SummonOdinZantetsukenMakeViewSpacePointPushed(void *point, int distance, PsxMatrix *target)
{
    int depth;
    VECTOR normal;

    SetRotMatrix(&g_BattleSummonOdinZMatrixE0);
    SetTransMatrix(&g_BattleSummonOdinZMatrixE0);
    RotTransPoint(point, target->t, &depth);
    if (distance != 0) {
        NormalizeVectorFixed(target->t, &normal);
        target->t[0] += normal.vx * distance >> 12;
        target->t[1] += normal.vy * distance >> 12;
        target->t[2] += normal.vz * distance >> 12;
    }
    return target;
}

// Acquires retained scratch once, then returns while paused. Every fifteen counts scans task actor
// index upward until g_BattleSummonOdinZWordB0 contains its bit, without a bound or empty-mask
// exit. Actors present in g_BattleTimelineTargetMask have their byte saved, bit 0x10 set and sound
// 0x1A3 dispatched; others receive Battle_PostActorHitOrHurtMessage. Gets selected actor bone position, builds
// random rotation X rand modulo 0x400 minus 0x200, Y 0x400 and Z negative rand masked 0x7FF,
// retaining a constant-false fixed-X branch. Rotates a vector whose X is actor short times twenty
// and adds the bone position into retained camera point. At remainder three, schedules
// BattleFx_SummonOdinZantetsukenSplitActorTask only for actors in g_BattleTimelineTargetMask,
// passing index, then advances actor index in either case. Every active call copies retained
// endpoint and bone-position DWORD pairs into camera globals, increments counter and retires when
// it reaches signed g_BattleSummonOdinZFrames.
// FUNCTION: FF7 0x004a9908
void __cdecl BattleFx_SummonOdinZantetsukenTargetSequenceTask(void)
{
    BattleFxOdinTargetSeq *task;
    int rotationX;
    BattleFxOdinZantetsukenSplitActor *splitTask;

    task = (BattleFxOdinTargetSeq *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonOdinZFlag2 & 1) == 0) {
        g_BattleSummonOdinZFlag2 |= 1;
        g_pBattleSummonOdinZTargetScratch = (BattleFxOdinZTargetScratch *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->phase % 15 == 0) {
        while ((g_BattleSummonOdinZWordB0 & 1 << task->bitIndex) == 0) {
            (task->bitIndex)++;
        }
        if ((g_BattleTimelineTargetMask & 1 << task->bitIndex) != 0) {
            g_BattleSummonOdinZRecordBuffer2[task->bitIndex] = g_BattleActorRecords[task->bitIndex].drawFlags;
            g_BattleActorRecords[task->bitIndex].drawFlags |= 0x10;
            Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, 0x40, 0x1A3);
        } else {
            Battle_PostActorHitOrHurtMessage(task->bitIndex);
        }
        Battle_GetBoneWorldPos(task->bitIndex, g_BattleActorRecords[task->bitIndex].targetBone, (short *)&g_BattleSummonOdinZCameraScriptTable);
        if (DEAD_IN_RETAIL != 0) {
            rotationX = 0x200 /* 45 deg */;
        } else {
            rotationX = (rand() % 0x400) - 0x200;
        }
        g_pBattleSummonOdinZTargetScratch->angle[0] = rotationX;
        g_pBattleSummonOdinZTargetScratch->angle[1] = 0x400 /* 90 deg */;
        g_pBattleSummonOdinZTargetScratch->angle[2] = -(rand() & 0x7FF);
        RotMatrix((SVECTOR *)g_pBattleSummonOdinZTargetScratch->angle, &g_pBattleSummonOdinZTargetScratch->matrix);
        g_pBattleSummonOdinZTargetScratch->angle[0] = g_BattleActorRecords[task->bitIndex].radius * 0x14;
        g_pBattleSummonOdinZTargetScratch->angle[1] = 0;
        g_pBattleSummonOdinZTargetScratch->angle[2] = 0;
        TransformVectorToInt16(&g_pBattleSummonOdinZTargetScratch->matrix, g_pBattleSummonOdinZTargetScratch->angle, g_pBattleSummonOdinZTargetScratch->angle);
        g_BattleSummonOdinZCameraEye.cameraPosX = g_pBattleSummonOdinZTargetScratch->angle[0] + g_BattleSummonOdinZCameraScriptTable.posX;
        g_BattleSummonOdinZCameraEye.cameraPosY = g_pBattleSummonOdinZTargetScratch->angle[1] + g_BattleSummonOdinZCameraScriptTable.posY;
        g_BattleSummonOdinZCameraEyeZ = g_pBattleSummonOdinZTargetScratch->angle[2] + g_BattleSummonOdinZCameraScriptTable.posZ;
    } else if (task->phase % 15 == 3) {
        if ((g_BattleTimelineTargetMask & 1 << task->bitIndex) != 0) {
            splitTask = (BattleFxOdinZantetsukenSplitActor *)BATTLE_NEW_TASK_SLOT(BattleFx_SummonOdinZantetsukenSplitActorTask);
            splitTask->actorIndex = task->bitIndex;
            (task->bitIndex)++;
        } else {
            (task->bitIndex)++;
        }
    }
    *(int *)&g_BattleCameraPosition = *(int *)&g_BattleSummonOdinZCameraEye;
    *(int *)&g_BattleCameraPosZ = *(int *)&g_BattleSummonOdinZCameraEyeZ;
    *(int *)&g_BattleCameraFocalPoint = *(int *)&g_BattleSummonOdinZCameraScriptTable.posX;
    *(int *)&g_BattleCameraFocusZ = *(int *)&g_BattleSummonOdinZCameraScriptTable.posZ;
    (task->phase)++;
    if (task->phase >= g_BattleSummonOdinZFrames) {
        task->bitIndex = 0xFFFF;
    }
}

// Acquires retained scratch once. At count zero refreshes viewport origin and sets both extents to
// 128 or 256 when g_BattleUiScalingMode equals two, even paused. Unpaused zero initializes actor
// capture via BattleFx_SummonOdinZantetsukenQueueActorOffscreenRequest, restores saved actor byte,
// calls Battle_RestoreActorAfterEffect with narrowed actor index, initializes
// BattleFx_SummonOdinZantetsukenInitSplitActorQuads and shared offsets/velocity/colour,
// then starts screen flash. Negative counts retain an empty branch. Counts one through six start
// three-count flash fade on entry, move paired offsets oppositely by retained velocity, damp
// velocity by 0xFF0 and call BattleFx_SummonOdinZantetsukenMoveSplitQuadHalf for two
// sides. Counts seven/eight repeat motion and subtract packed grey 0x00404040 from retained colour;
// later retires before counter update. Active nonzero phases set two offscreen object state fields
// and submit both retained primitives with blend factors. Only counter increment is pause-gated
// after zero initialization; motion, fade, colour and draws at nonzero counts remain pause-active.
// FUNCTION: FF7 0x004a9c32
void __cdecl BattleFx_SummonOdinZantetsukenSplitActorTask(void)
{
    int frame;
    int stackWords[4]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxOdinZantetsukenSplitActor *task;

    task = (BattleFxOdinZantetsukenSplitActor *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleSummonOdinZByteA0 & 1) == 0) {
        g_BattleSummonOdinZByteA0 |= 1;
        g_BattleSummonOdinZSplitScratch = Gfx_GetPsxScratchpadPtr(0);
    }
    frame = task->frameIndex;
    if (frame == 0) {
        if (g_BattleUiScalingMode != 2) {
            g_BattleSummonOdinZTaskParam.captureRect.posX = (short)g_BattleViewportOriginX;
            g_BattleSummonOdinZTaskParam.captureRect.posY = (short)g_BattleViewportOriginY;
            g_BattleSummonOdinZTaskParam.captureRect.height = 128;
            g_BattleSummonOdinZTaskParam.captureRect.width = g_BattleSummonOdinZTaskParam.captureRect.height;
        } else {
            g_BattleSummonOdinZTaskParam.captureRect.posX = (short)g_BattleViewportOriginX;
            g_BattleSummonOdinZTaskParam.captureRect.posY = (short)g_BattleViewportOriginY;
            g_BattleSummonOdinZTaskParam.captureRect.height = 256;
            g_BattleSummonOdinZTaskParam.captureRect.width = g_BattleSummonOdinZTaskParam.captureRect.height;
        }
        if (g_BattleIsPaused == 0) {
            BattleFx_SummonOdinZantetsukenQueueActorOffscreenRequest(task->actorIndex, &g_BattleSummonOdinZTaskParam.captureRect, &task->screen);
            g_BattleActorRecords[task->actorIndex].drawFlags = g_BattleSummonOdinZRecordBuffer2[task->actorIndex];
            Battle_RestoreActorAfterEffect(*(char *)&task->actorIndex);
            g_BattleSummonOdinZSplitScale = 0;
            BattleFx_SummonOdinZantetsukenInitSplitActorQuads(&task->screen, g_BattleSummonOdinZSplitScale);
            g_BattleSummonOdinZSplitOffsetA = 0;
            g_BattleSummonOdinZSplitOffsetB = 0;
            g_BattleSummonOdinZSplitSpeed = 8;
            g_BattleSummonOdinZSplitColor = 0x2C808080;
            g_BattleSummonOdinZSplitColorStep = 0x40;
            g_BattleSummonOdinZSplitColorStep |= g_BattleSummonOdinZSplitColorStep << 0x10 | g_BattleSummonOdinZSplitColorStep << 8;
            BattleFx_StartScreenFlashRamp(1);
        }
    } else if (frame < 1) { /* empty first arm: lib/codegen.h */
    } else {
        frame--;
        if (frame < 6) {
            if (frame == 0) {
                BattleFx_FadeOutScreenFlashRamp(3);
            }
            g_BattleSummonOdinZSplitOffsetA += g_BattleSummonOdinZSplitSpeed;
            g_BattleSummonOdinZSplitOffsetB -= g_BattleSummonOdinZSplitSpeed;
            g_BattleSummonOdinZSplitSpeed = g_BattleSummonOdinZSplitSpeed * 0xFF0 >> 12;
            BattleFx_SummonOdinZantetsukenMoveSplitQuadHalf(g_BattleSummonOdinZSplitOffsetA, g_BattleSummonOdinZSplitScale, g_BattleSummonOdinZDrawListTaskSlot, 0);
            BattleFx_SummonOdinZantetsukenMoveSplitQuadHalf(g_BattleSummonOdinZSplitOffsetB, g_BattleSummonOdinZSplitScale, g_BattleSummonOdinZDrawListTaskSlot, 1);
        } else {
            frame -= 6;
            if (frame < 2) {
                g_BattleSummonOdinZSplitOffsetA += g_BattleSummonOdinZSplitSpeed;
                g_BattleSummonOdinZSplitOffsetB -= g_BattleSummonOdinZSplitSpeed;
                g_BattleSummonOdinZSplitSpeed = g_BattleSummonOdinZSplitSpeed * 0xFF0 >> 12;
                g_BattleSummonOdinZSplitColor -= g_BattleSummonOdinZSplitColorStep;
                BattleFx_SummonOdinZantetsukenMoveSplitQuadHalf(g_BattleSummonOdinZSplitOffsetA, g_BattleSummonOdinZSplitScale, g_BattleSummonOdinZDrawListTaskSlot, 0);
                BattleFx_SummonOdinZantetsukenMoveSplitQuadHalf(g_BattleSummonOdinZSplitOffsetB, g_BattleSummonOdinZSplitScale, g_BattleSummonOdinZDrawListTaskSlot, 1);
            } else {
                task->actorIndex = 0xFFFF;
                return;
            }
        }
        Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleSummonOdinZState));
        Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleSummonOdinZState));
        Gfx_SetBlendFactorPair(g_BattleSummonOdinZBlendFactorZ, g_BattleSummonOdinZBlendFactorW);
        Gfx_DrawPsxPrimitive(&g_BattleSummonOdinZMatrixBuffer[g_BattleSummonOdinZDrawListTaskSlot][0]);
        Gfx_SetBlendFactorPair(g_BattleSummonOdinZBlendFactorZ, g_BattleSummonOdinZBlendFactorW);
        Gfx_DrawPsxPrimitive(&g_BattleSummonOdinZMatrixBuffer[g_BattleSummonOdinZDrawListTaskSlot][1]);
    }
    if (g_BattleIsPaused == 0) {
        (task->frameIndex)++;
    }
}

// Releases any nonnegative retained offscreen slot and resets its sentinel. Gets actor attachment
// zero into selected bone matrix, installs/refreshes it and projects retained point g_BattleSummonOdinZAngles00
// into caller output, saving projection return as short plus last NDC Z/W into blend globals. When
// rendering context and its first pointer are nonzero, acquires request; on success fills actor
// model/material pointers, matrix conversion and request flags, including overwriting field 0xD0
// from one to four. Mode zero selects 320 by 240 with zero origin, mode one the same size with
// origin (160,120), mode two 640 by 480 with zero origin; other modes leave local
// dimensions/origin uninitialized in current C. Copies caller rectangle shorts, then overrides
// capture position with projected XY minus 64 or 128 in mode two. All branches copy actor 64-byte
// matrix and 116-byte record, clear fields 0x80/0x84, set dimensions/final flags and retain
// request slot field 0x114. Unused mode-local values and context local remain.
// Failure leaves slot sentinel and projection flags are unused.
// FUNCTION: FF7 0x004a9fd1
void __cdecl BattleFx_SummonOdinZantetsukenQueueActorOffscreenRequest(int actorIndex, BattleFxOdinZantetsukenActorPos *rect, BattleFxActorOffscreenRequest *capture)
{
    int projectFlags;
    PsxMatrix *mtx;
    int offsetX;
    int zeroField;
    int widthPx;
    int screenHeight;
    int padField[8]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int depth;
    GfxOffscreenRequest *request;
    int dataHandle;
    int originY;
    int extent;
    int ratio;

    if (g_BattleSummonOdinZState >= 0) {
        Battle3d_ReleaseOffscreenSlot(g_BattleSummonOdinZState);
        g_BattleSummonOdinZState = 0xFFFFFFFF;
    }
    Battle3d_GetActorAttachMatrix(actorIndex, 0, (PsxMatrix *)&g_BattleActorRecords[actorIndex].boneNodes[g_BattleActorRecords[actorIndex].targetBone * 0x34]);
    mtx = (PsxMatrix *)&g_BattleActorRecords[actorIndex].boneNodes[g_BattleActorRecords[actorIndex].targetBone * 0x34];
    SetRotMatrix(mtx);
    SetTransMatrix(mtx);
    RefreshGlobalTransformState();
    capture->projectResult = ProjectPointHardware(g_BattleSummonOdinZAngles00, (short *)capture, &projectFlags, &depth);
    *(float *)&g_BattleSummonOdinZBlendFactorZ = GetLastProjectedNdcZ();
    *(float *)&g_BattleSummonOdinZBlendFactorW = GetLastProjectedNdcW();
    if (g_pBattle3dScene != 0 && g_pBattle3dScene->stageId != 0) {
        dataHandle = g_pBattle3dScene->stageId;
        request = (GfxOffscreenRequest *)Battle3d_AcquireOffscreenRequest();
        if (request != 0) {
            request->kind = 2;
            request->skeleton = g_BattleActorRecords[actorIndex].animBank;
            request->animFrame = ((int *)(g_BattleActorRecords[actorIndex].model->frameTable))[g_BattleActorRecords[actorIndex].animFrame];
            request->drawSpec = g_BattleActorRecords[actorIndex].model->modelParts;
            GtePsxMatrixToFloat4x4Transposed(mtx, request->modelMatrix);
            request->pageFillEnabled = 1;
            request->renderKind = 0xC;
            request->blendMode = BLEND_ADDITIVE;
            request->blendMode = BLEND_NONE;
            switch (g_BattleUiScalingMode) {
            case MAIN_GRAPHICS_MODE_320X240:
                zeroField = 0;
                ratio = 0x25;
                offsetX = 0;
                originY = 0;
                widthPx = 320;
                screenHeight = 240;
                extent = 0xA6;
                break;
            case MAIN_GRAPHICS_MODE_320X240_CENTERED:
                zeroField = 0;
                ratio = 0x25;
                offsetX = 0xA0;
                originY = 120;
                widthPx = 320;
                screenHeight = 240;
                extent = 0xA6;
                break;
            case MAIN_GRAPHICS_MODE_640X480:
                zeroField = 0;
                ratio = 0x4A;
                offsetX = 0;
                originY = 0;
                widthPx = 640;
                screenHeight = 480;
                extent = 0x14C;
            }
            request->viewportX = rect->posX;
            request->viewportY = rect->posY;
            request->viewportW = rect->width;
            request->viewportH = rect->height;
            request->altViewportX = offsetX;
            request->altViewportY = originY;
            request->projOffsetX = zeroField;
            if (g_BattleUiScalingMode == 2) {
                request->altViewportW = widthPx;
                request->altViewportH = screenHeight;
                request->viewportHeightOverride = 0;
                request->projOffsetX = zeroField;
                request->projOffsetY = 0;
                request->viewportX = capture->posX - 128;
                request->viewportY = capture->posY - 128;
                *(GfxMatrix4x4 *)request->modelMatrix = *(GfxMatrix4x4 *)g_BattleActorRecords[actorIndex].worldMatrix;
                *(BattleModelDrawState *)request->placement = *(BattleModelDrawState *)(actorIndex * 0x74 + (char *)&g_BattleSummonOdinZTrackBuffer);
                request->placementOutPos = 0;
                request->placementTransformBase = 0;
            } else if (g_BattleUiScalingMode == 1) {
                request->altViewportW = widthPx;
                request->altViewportH = screenHeight;
                request->viewportHeightOverride = 0;
                request->projOffsetX = zeroField;
                request->projOffsetY = 0;
                request->viewportX = capture->posX - 64;
                request->viewportY = capture->posY - 64;
                *(GfxMatrix4x4 *)request->modelMatrix = *(GfxMatrix4x4 *)g_BattleActorRecords[actorIndex].worldMatrix;
                *(BattleModelDrawState *)request->placement = *(BattleModelDrawState *)(actorIndex * 0x74 + (char *)&g_BattleSummonOdinZTrackBuffer);
                request->placementOutPos = 0;
                request->placementTransformBase = 0;
            } else {
                request->altViewportW = widthPx;
                request->altViewportH = screenHeight;
                request->viewportHeightOverride = 0;
                request->projOffsetX = zeroField;
                request->projOffsetY = 0;
                request->viewportX = capture->posX - 64;
                request->viewportY = capture->posY - 64;
                *(GfxMatrix4x4 *)request->modelMatrix = *(GfxMatrix4x4 *)g_BattleActorRecords[actorIndex].worldMatrix;
                *(BattleModelDrawState *)request->placement = *(BattleModelDrawState *)(actorIndex * 0x74 + (char *)&g_BattleSummonOdinZTrackBuffer);
                request->placementOutPos = 0;
                request->placementTransformBase = 0;
            }
            request->pageFillStepX = 1;
            request->pageFillStepY = 1;
            g_BattleSummonOdinZState = request->slotIndex;
        }
    }
}

// Initializes one pair of retained textured quad records with nine-word headers, command/grey
// 0x2C808080 and page 0x12C. Sets horizontal UV endpoints zero/255 and vertical boundaries zero,
// 128 minus scale times 128 shifted twelve, 128 plus scale times 127 shifted twelve and 255. For
// modes other than two halves UV bytes after narrowing; mode two retains full range. Builds eight
// retained XY corners from caller point: horizontal sides point X minus 128/plus 127 in mode two,
// otherwise minus 64/plus 63. Vertical outer boundaries use the same extents and middle boundaries
// add signed fixed-point scale times negative lower/positive upper extent.
// FUNCTION: FF7 0x004aa516
void __cdecl BattleFx_SummonOdinZantetsukenInitSplitActorQuads(BattleFxActorOffscreenRequest *center, int scale)
{
    int extentMinus;
    int column;
    int rowIdx;
    int extentHigh;

    for (rowIdx = 0; rowIdx < 1; rowIdx++) {
        for (column = 0; column < 2; column++) {
            g_BattleSummonOdinZMatrixBuffer[rowIdx][column].tag = 0x9000000;
            g_BattleSummonOdinZMatrixBuffer[rowIdx][column].color = 0x2C808080;
            g_BattleSummonOdinZMatrixBuffer[rowIdx][column].tpage = 0x12C;
            g_BattleSummonOdinZMatrixBuffer[rowIdx][column].u2 = 0;
            g_BattleSummonOdinZMatrixBuffer[rowIdx][column].u0 = 0;
            g_BattleSummonOdinZMatrixBuffer[rowIdx][column].u3 = 0xFF;
            g_BattleSummonOdinZMatrixBuffer[rowIdx][column].u1 = 0xFF;
            if (g_BattleUiScalingMode != 2) {
                g_BattleSummonOdinZMatrixBuffer[rowIdx][column].u0 >>= 1;
                g_BattleSummonOdinZMatrixBuffer[rowIdx][column].u1 >>= 1;
                g_BattleSummonOdinZMatrixBuffer[rowIdx][column].u2 >>= 1;
                g_BattleSummonOdinZMatrixBuffer[rowIdx][column].u3 >>= 1;
            }
        }
        g_BattleSummonOdinZMatrixBuffer[rowIdx][0].v1 = 0;
        g_BattleSummonOdinZMatrixBuffer[rowIdx][0].v0 = 0;
        g_BattleSummonOdinZMatrixBuffer[rowIdx][1].v0 = (scale * -0x80 >> 12) + 0x80;
        g_BattleSummonOdinZMatrixBuffer[rowIdx][0].v2 = g_BattleSummonOdinZMatrixBuffer[rowIdx][1].v0;
        g_BattleSummonOdinZMatrixBuffer[rowIdx][1].v1 = (scale * 0x7F >> 12) + 0x80;
        g_BattleSummonOdinZMatrixBuffer[rowIdx][0].v3 = g_BattleSummonOdinZMatrixBuffer[rowIdx][1].v1;
        g_BattleSummonOdinZMatrixBuffer[rowIdx][1].v3 = 0xFF;
        g_BattleSummonOdinZMatrixBuffer[rowIdx][1].v2 = 0xFF;
        if (g_BattleUiScalingMode != 2) {
            g_BattleSummonOdinZMatrixBuffer[rowIdx][0].v2 >>= 1;
            g_BattleSummonOdinZMatrixBuffer[rowIdx][1].v0 >>= 1;
            g_BattleSummonOdinZMatrixBuffer[rowIdx][0].v3 >>= 1;
            g_BattleSummonOdinZMatrixBuffer[rowIdx][1].v1 >>= 1;
            g_BattleSummonOdinZMatrixBuffer[rowIdx][1].v2 >>= 1;
            g_BattleSummonOdinZMatrixBuffer[rowIdx][1].v3 >>= 1;
        }
    }
    if (g_BattleUiScalingMode == 2) {
        extentMinus = 0x80;
        extentHigh = 0x7F;
    } else {
        extentMinus = 0x40;
        extentHigh = 0x3F;
    }
    g_BattleSummonOdinZAnimState[1].x2 = center->posX - extentMinus;
    g_BattleSummonOdinZAnimState[1].x0 = g_BattleSummonOdinZAnimState[1].x2;
    g_BattleSummonOdinZAnimState[0].x2 = g_BattleSummonOdinZAnimState[1].x0;
    g_BattleSummonOdinZAnimState[0].x0 = g_BattleSummonOdinZAnimState[0].x2;
    g_BattleSummonOdinZAnimState[1].x3 = center->posX + extentHigh;
    g_BattleSummonOdinZAnimState[1].x1 = g_BattleSummonOdinZAnimState[1].x3;
    g_BattleSummonOdinZAnimState[0].x3 = g_BattleSummonOdinZAnimState[1].x1;
    g_BattleSummonOdinZAnimState[0].x1 = g_BattleSummonOdinZAnimState[0].x3;
    g_BattleSummonOdinZAnimState[0].y1 = center->posY - extentMinus;
    g_BattleSummonOdinZAnimState[0].y0 = g_BattleSummonOdinZAnimState[0].y1;
    g_BattleSummonOdinZAnimState[1].y0 = center->posY + (scale * -extentMinus >> 12);
    g_BattleSummonOdinZAnimState[0].y2 = g_BattleSummonOdinZAnimState[1].y0;
    g_BattleSummonOdinZAnimState[1].y1 = center->posY + (scale * extentHigh >> 12);
    g_BattleSummonOdinZAnimState[0].y3 = g_BattleSummonOdinZAnimState[1].y1;
    g_BattleSummonOdinZAnimState[1].y3 = center->posY + extentHigh;
    g_BattleSummonOdinZAnimState[1].y2 = g_BattleSummonOdinZAnimState[1].y3;
}

// Negates extent when scale is negative and computes scale times adjusted extent shifted twelve. At
// row*0x50 plus column*0x28 writes four horizontal shorts from the selected baseline pair plus
// adjusted extent, duplicating each across paired vertices, and four vertical shorts from the
// baseline row plus the computed product; no index guards appear.
// FUNCTION: FF7 0x004aa90d
void __cdecl BattleFx_SummonOdinZantetsukenMoveSplitQuadHalf(int extent, int scale, int row, int column)
{
    int deltaY;

    if (scale < 0) {
        extent = -extent;
    }
    deltaY = scale * extent >> 12;
    g_BattleSummonOdinZMatrixBuffer[row][column].x0 =
        g_BattleSummonOdinZMatrixBuffer[row][column].x2 =
            g_BattleSummonOdinZAnimState[column].x0 + extent;
    g_BattleSummonOdinZMatrixBuffer[row][column].x1 =
        g_BattleSummonOdinZMatrixBuffer[row][column].x3 =
            g_BattleSummonOdinZAnimState[column].x1 + extent;
    g_BattleSummonOdinZMatrixBuffer[row][column].y0 = g_BattleSummonOdinZAnimState[column].y0 + deltaY;
    g_BattleSummonOdinZMatrixBuffer[row][column].y1 = g_BattleSummonOdinZAnimState[column].y1 + deltaY;
    g_BattleSummonOdinZMatrixBuffer[row][column].y2 = g_BattleSummonOdinZAnimState[column].y2 + deltaY;
    g_BattleSummonOdinZMatrixBuffer[row][column].y3 = g_BattleSummonOdinZAnimState[column].y3 + deltaY;
}

// For each of the ten slots whose bit is set in the mask g_BattleSummonOdinZWordB0, XORs bit 1 (value 2) of the
// short at g_BattleActorStates[slot].flags. 0x74 is the record stride; nothing else is read.
// FUNCTION: FF7 0x004aaa5a
void __cdecl BattleFx_SummonOdinZantetsukenToggleMaskedActorBit1(void)
{
    int i;

    for (i = 0; i < 10; i++) {
        if ((g_BattleSummonOdinZWordB0 & 1 << i) != 0) {
            g_BattleActorStates[i].flags ^= 2;
        }
    }
}

// Per-frame screen flash task; does nothing while g_BattleIsPaused is set. On
// frame 0 it sets the three colour bytes g_BattleFxTimelineState, g_BattleFxTimelineState.channel1
// and g_BattleFxTimelineState.channel2 to 0x80 and g_BattleFxTimelineState.startValue to 0x1000; on
// frame 6 it zeroes the three colour bytes, sets g_BattleFxTimelineState.startValue to 0x800 and
// frees the slot. frameCounter is incremented every call.
// FUNCTION: FF7 0x004aaaaf
void __cdecl BattleFx_SummonOdinZantetsukenScreenFlashPulseTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0) {
        g_BattleFxTimelineState.red = 0x80;
        g_BattleFxTimelineState.channel1 = 0x80;
        g_BattleFxTimelineState.channel2 = 0x80;
        g_BattleFxTimelineState.startValue = 0x1000;
    } else if (task->frameCounter == 6) {
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        g_BattleFxTimelineState.startValue = 0x800;
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

// clang-format off
// GLOBAL: FF7 0x00aed598
short g_BattleSummonOdinZBillboardMatrix[2] = {0};
unsigned char g_bssPad_00aed59c[4] = {0};
// GLOBAL: FF7 0x00aed5a0
short g_BattleSummonOdinZTimer1 = 0;
unsigned char g_bssPad_00aed5a4[4] = {0};
// GLOBAL: FF7 0x00aed5a8
unsigned char g_BattleSummonOdinZSpriteReq1[14] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00aed5b8
BattleFxOdinTrailSegment *g_BattleSummonOdinZTrailDataBlock = 0;
unsigned char g_bssPad_00aed5bc[4] = {0};
// GLOBAL: FF7 0x00aed5c0
unsigned char g_BattleSummonOdinZTrackBuffer[116] = {0};
unsigned char g_bssPad_00aed634[4] = {0};
unsigned char g_bssPad_00aed638[1040] = {0};
// GLOBAL: FF7 0x00aeda48
BattleFxOdinZTargetScratch *g_pBattleSummonOdinZTargetScratch = 0;
unsigned char g_bssPad_00aeda4c[4] = {0};
// The target bone position and the camera script's operand pool (see
// BattleFx_OdinZantetsuReadCameraScriptOperand).
// GLOBAL: FF7 0x00aeda50
struct BattleFxOdinZCameraScript g_BattleSummonOdinZCameraScriptTable = {0};
unsigned char g_bssPad_00aeda60[16] = {0};
// GLOBAL: FF7 0x00aeda70
BattleFxOdinZSplitPacket g_BattleSummonOdinZMatrixBuffer[1][2] = {0};
unsigned char g_bssPad_00aedac0[80] = {0};
// GLOBAL: FF7 0x00aedb10
unsigned char g_BattleSummonOdinZTimelineScratchReady[0x4] = {0};
unsigned char g_bssPad_00aedb14[4] = {0};
// GLOBAL: FF7 0x00aedb18
BattleSummonOdinZSplitHalf g_BattleSummonOdinZAnimState[2] = {0};
// GLOBAL: FF7 0x00aedb38
CameraScriptTrack g_BattleSummonOdinCameraFocus = {0};
unsigned char g_bssPad_00aedb64[4] = {0};
unsigned char g_bssPad_00aedb68[12] = {0};
// GLOBAL: FF7 0x00aedb74
unsigned char g_BattleSummonOdinZFlag1 = 0;
// GLOBAL: FF7 0x00aedb78
unsigned char g_BattleSummonOdinZFlag2 = 0;
// GLOBAL: FF7 0x00aedb7c
int g_BattleSummonOdinZStateWord = 0;
// GLOBAL: FF7 0x00aedb80
int g_BattleSummonOdinZScrollOffset = 0;
unsigned char g_bssPad_00aedb84[4] = {0};
unsigned char g_bssPad_00aedb88[4096] = {0};
// GLOBAL: FF7 0x00aeeb88
int g_BattleSummonOdinZSplitSpeed = 0;
// GLOBAL: FF7 0x00aeeb8c
int g_BattleSummonOdinZBlendFactorW = 0;
// GLOBAL: FF7 0x00aeeb90
int g_BattleSummonOdinZSplitScratch = 0;
// GLOBAL: FF7 0x00aeeb94
int g_BattleSummonOdinZBlendFactorZ = 0;
// GLOBAL: FF7 0x00aeeb98
int g_BattleSummonOdinZVar98 = 0;
unsigned char g_bssPad_00aeeb9c[4] = {0};
// GLOBAL: FF7 0x00aeeba0
unsigned char g_BattleSummonOdinZByteA0 = 0;
unsigned char g_bssPad_00aeeba4[4] = {0};
unsigned char g_bssPad_00aeeba8[8] = {0};
// GLOBAL: FF7 0x00aeebb0
int g_BattleSummonOdinZWordB0 = 0;
// GLOBAL: FF7 0x00aeebb4
int g_BattleSummonOdinZSplitColor = 0;
// GLOBAL: FF7 0x00aeebb8
short g_BattleSummonOdinZScrollMatrix[2] = {0};
unsigned char g_bssPad_00aeebbc[4] = {0};
// GLOBAL: FF7 0x00aeebc0
BattleSummonOdinZMatrixTail g_BattleSummonOdinZVectorC0 = {0};
unsigned char g_bssPad_00aeebd0[8] = {0};
// GLOBAL: FF7 0x00aeebd8
int g_BattleSummonOdinZSplitColorStep = 0;
unsigned char g_bssPad_00aeebdc[4] = {0};
// GLOBAL: FF7 0x00aeebe0
PsxMatrix g_BattleSummonOdinZMatrixE0 = {0};
// GLOBAL: FF7 0x00aeec00
CameraScriptTrack * g_pBattleSummonOdinZCameraTrack = 0;
// GLOBAL: FF7 0x00aeec04
int g_BattleSummonOdinZSplitOffsetB = 0;
// GLOBAL: FF7 0x00aeec08
CameraScriptTrack * g_pBattleSummonOdinZCameraOther = 0;
// GLOBAL: FF7 0x00aeec0c
BattleFxOdinZMotionScratch *g_pBattleSummonOdinZMotionScratch = 0;
// GLOBAL: FF7 0x00aeec10
unsigned char g_BattleSummonOdinZModelMatrixReady = 0;
// GLOBAL: FF7 0x00aeec14
short g_BattleSummonOdinZFrames = 0;
// The blade trail's samples: both endpoints of the blade, one record per sample.
// GLOBAL: FF7 0x00aeec18
BattleSummonOdinZRecordBuffer1Entry g_BattleSummonOdinZRecordBuffer1[0x24] = {0};
// GLOBAL: FF7 0x00aeee58
char g_BattleSummonOdinZRecordBuffer2[10] = {0};
// GLOBAL: FF7 0x00aeee64
int g_BattleSummonOdinZTimelineScratch = 0;
unsigned char g_bssPad_00aeee68[8] = {0};
// GLOBAL: FF7 0x00aeee70
BattleSummonOdinZAfterimageMatrices g_BattleSummonOdinZAfterimageMatrices = {0};
// GLOBAL: FF7 0x00aeef80
int g_BattleSummonOdinZSplitOffsetA = 0;
unsigned char g_bssPad_00aeef84[4] = {0};
// GLOBAL: FF7 0x00aeef88
CameraScriptTrack g_BattleSummonOdinCameraEye = {0};
// GLOBAL: FF7 0x00aeefb4
PsxMatrix *g_pBattleSummonOdinZModelMatrix = 0;
// GLOBAL: FF7 0x00aeefb8
int g_BattleSummonOdinZSplitScale = 0;
// GLOBAL: FF7 0x00aeefbc
unsigned char g_BattleSummonOdinZByteBC = 0;
// GLOBAL: FF7 0x00aeefc0
struct BattleFxOdinZTaskSlot g_BattleSummonOdinZCameraEye = {0};
// GLOBAL: FF7 0x00aeefc4
short g_BattleSummonOdinZCameraEyeZ = 0;
// GLOBAL: FF7 0x00aeefc8
unsigned char g_BattleSummonOdinZByteC8 = 0;
// GLOBAL: FF7 0x00aeefcc
PsxMatrix *g_pBattleSummonOdinZAfterimageMatrix = 0;
// GLOBAL: FF7 0x00aeefd0
struct GfxTableCursor *g_BattleSummonOdinZLoadedResTable = 0;
unsigned char g_bssPad_00aeefd4[4] = {0};
// GLOBAL: FF7 0x00aeefd8
PsxMatrix g_BattleSummonOdinZMatrixD8 = {0};
// GLOBAL: FF7 0x00aeeff8
short g_BattleSummonOdinZAnglesF8[3] = {0};
// GLOBAL: FF7 0x00aef000
short g_BattleSummonOdinZAngles00[3] = {0};
// GLOBAL: FF7 0x00aef008
int g_BattleSummonOdinZDrawListTaskSlot = 0;
// GLOBAL: FF7 0x00aef00c
int g_BattleSummonOdinZDrawBufferCurrent = 0;
// clang-format on
} /* extern "C" */
