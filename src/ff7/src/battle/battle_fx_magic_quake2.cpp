// src/ff7/src/battle/battle_fx_magic_quake2.cpp
/* Magic Effect [37]: Quake2 (Japanese: クエイラ Quakera, dev tag: "MAGIC/QUEIR" / Queir) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle visual effect for the Queir magic: it builds exploding shard particles from a
//   mesh, then drives single-target or multi-target timeline tasks with shard bursts, shrinking and
//   growing models, and fading quads.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle_magic.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/codegen.h"
extern "C" {

struct GfxPsxShortVertexSlot;
struct BattleRotationMatrix;
struct BattleFxEulerAngles16;
struct GfxPsxSpriteQuadState;
struct BattleFxModelDraw;

#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x008b1ea0
unsigned char g_BattleMagicQuake2MeshBurstPayload[2656] = {
    0x08, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x98, 0xf8, 0x00, 0x00, 0xd5, 0x02, 0x01, 0x00,
    0x28, 0xf9, 0x00, 0x00, 0x3d, 0x05, 0x01, 0x00, 0xc3, 0xfa, 0x00, 0x00, 0xd8, 0x06, 0x01, 0x00,
    0x2b, 0xfd, 0x00, 0x00, 0x68, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x06, 0x01, 0x00,
    0xd5, 0x02, 0x00, 0x00, 0x3d, 0x05, 0x01, 0x00, 0x3d, 0x05, 0x00, 0x00, 0xd5, 0x02, 0x01, 0x00,
    0xd8, 0x06, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x68, 0x07, 0x00, 0x00, 0x2b, 0xfd, 0x01, 0x00,
    0xd8, 0x06, 0x00, 0x00, 0xc3, 0xfa, 0x01, 0x00, 0x3d, 0x05, 0x00, 0x00, 0x28, 0xf9, 0x01, 0x00,
    0xd5, 0x02, 0x00, 0x00, 0x98, 0xf8, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0xf9, 0x01, 0x00,
    0x2b, 0xfd, 0x00, 0x00, 0xc3, 0xfa, 0x01, 0x00, 0xc3, 0xfa, 0x00, 0x00, 0x2b, 0xfd, 0x01, 0x00,
    0x28, 0xf9, 0x00, 0x00, 0xbf, 0x02, 0xdc, 0x00, 0x5d, 0xf9, 0x00, 0x00, 0x00, 0x00, 0xdc, 0x00,
    0xd1, 0xf8, 0x00, 0x00, 0x41, 0xfd, 0xdc, 0x00, 0x5d, 0xf9, 0x00, 0x00, 0xec, 0xfa, 0xdc, 0x00,
    0xec, 0xfa, 0x00, 0x00, 0x5d, 0xf9, 0xdc, 0x00, 0x41, 0xfd, 0x00, 0x00, 0xd1, 0xf8, 0xdc, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x5d, 0xf9, 0xdc, 0x00, 0xbf, 0x02, 0x00, 0x00, 0xec, 0xfa, 0xdc, 0x00,
    0x14, 0x05, 0x00, 0x00, 0x41, 0xfd, 0xdc, 0x00, 0xa3, 0x06, 0x00, 0x00, 0x00, 0x00, 0xdc, 0x00,
    0x2f, 0x07, 0x00, 0x00, 0xbf, 0x02, 0xdc, 0x00, 0xa3, 0x06, 0x00, 0x00, 0x14, 0x05, 0xdc, 0x00,
    0x14, 0x05, 0x00, 0x00, 0xa3, 0x06, 0xdc, 0x00, 0xbf, 0x02, 0x00, 0x00, 0x2f, 0x07, 0xdc, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xa3, 0x06, 0xdc, 0x00, 0x41, 0xfd, 0x00, 0x00, 0x14, 0x05, 0xdc, 0x00,
    0xec, 0xfa, 0x00, 0x00, 0x98, 0x04, 0xdc, 0x00, 0x68, 0xfb, 0x00, 0x00, 0x7c, 0x02, 0xdc, 0x00,
    0x00, 0xfa, 0x00, 0x00, 0x00, 0x00, 0xdc, 0x00, 0x81, 0xf9, 0x00, 0x00, 0x84, 0xfd, 0xdc, 0x00,
    0x00, 0xfa, 0x00, 0x00, 0x68, 0xfb, 0xdc, 0x00, 0x68, 0xfb, 0x00, 0x00, 0x00, 0xfa, 0xdc, 0x00,
    0x84, 0xfd, 0x00, 0x00, 0x81, 0xf9, 0xdc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfa, 0xdc, 0x00,
    0x7c, 0x02, 0x00, 0x00, 0x68, 0xfb, 0xdc, 0x00, 0x98, 0x04, 0x00, 0x00, 0x84, 0xfd, 0xdc, 0x00,
    0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0xdc, 0x00, 0x7f, 0x06, 0x00, 0x00, 0x7c, 0x02, 0xdc, 0x00,
    0x00, 0x06, 0x00, 0x00, 0x98, 0x04, 0xdc, 0x00, 0x98, 0x04, 0x00, 0x00, 0x00, 0x06, 0xdc, 0x00,
    0x7c, 0x02, 0x00, 0x00, 0x7f, 0x06, 0xdc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0xdc, 0x00,
    0x84, 0xfd, 0x00, 0x00, 0x36, 0x04, 0xfa, 0x01, 0xca, 0xfb, 0x00, 0x00, 0x47, 0x02, 0xfa, 0x01,
    0x7f, 0xfa, 0x00, 0x00, 0x00, 0x00, 0xfa, 0x01, 0x0b, 0xfa, 0x00, 0x00, 0xb9, 0xfd, 0xfa, 0x01,
    0x7f, 0xfa, 0x00, 0x00, 0xca, 0xfb, 0xfa, 0x01, 0xca, 0xfb, 0x00, 0x00, 0x7f, 0xfa, 0xfa, 0x01,
    0xb9, 0xfd, 0x00, 0x00, 0x0b, 0xfa, 0xfa, 0x01, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfa, 0xfa, 0x01,
    0x47, 0x02, 0x00, 0x00, 0xca, 0xfb, 0xfa, 0x01, 0x36, 0x04, 0x00, 0x00, 0xb9, 0xfd, 0xfa, 0x01,
    0x81, 0x05, 0x00, 0x00, 0x00, 0x00, 0xfa, 0x01, 0xf5, 0x05, 0x00, 0x00, 0x47, 0x02, 0xfa, 0x01,
    0x81, 0x05, 0x00, 0x00, 0x36, 0x04, 0xfa, 0x01, 0x36, 0x04, 0x00, 0x00, 0x81, 0x05, 0xfa, 0x01,
    0x47, 0x02, 0x00, 0x00, 0xf5, 0x05, 0xfa, 0x01, 0x00, 0x00, 0x00, 0x00, 0x81, 0x05, 0xfa, 0x01,
    0xb9, 0xfd, 0x00, 0x00, 0x0a, 0x03, 0xfa, 0x01, 0xf6, 0xfc, 0x00, 0x00, 0xa5, 0x01, 0xfa, 0x01,
    0x07, 0xfc, 0x00, 0x00, 0x00, 0x00, 0xfa, 0x01, 0xb3, 0xfb, 0x00, 0x00, 0x5b, 0xfe, 0xfa, 0x01,
    0x07, 0xfc, 0x00, 0x00, 0xf6, 0xfc, 0xfa, 0x01, 0xf6, 0xfc, 0x00, 0x00, 0x07, 0xfc, 0xfa, 0x01,
    0x5b, 0xfe, 0x00, 0x00, 0xb3, 0xfb, 0xfa, 0x01, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfc, 0xfa, 0x01,
    0xa5, 0x01, 0x00, 0x00, 0xf6, 0xfc, 0xfa, 0x01, 0x0a, 0x03, 0x00, 0x00, 0x5b, 0xfe, 0xfa, 0x01,
    0xf9, 0x03, 0x00, 0x00, 0x00, 0x00, 0xfa, 0x01, 0x4d, 0x04, 0x00, 0x00, 0xa5, 0x01, 0xfa, 0x01,
    0xf9, 0x03, 0x00, 0x00, 0x0a, 0x03, 0xfa, 0x01, 0x0a, 0x03, 0x00, 0x00, 0xf9, 0x03, 0xfa, 0x01,
    0xa5, 0x01, 0x00, 0x00, 0x4d, 0x04, 0xfa, 0x01, 0x00, 0x00, 0x00, 0x00, 0xf9, 0x03, 0xfa, 0x01,
    0x5b, 0xfe, 0x00, 0x00, 0xdd, 0x01, 0x54, 0x03, 0x23, 0xfe, 0x00, 0x00, 0x02, 0x01, 0x54, 0x03,
    0x90, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x54, 0x03, 0x5d, 0xfd, 0x00, 0x00, 0xfe, 0xfe, 0x54, 0x03,
    0x90, 0xfd, 0x00, 0x00, 0x23, 0xfe, 0x54, 0x03, 0x23, 0xfe, 0x00, 0x00, 0x90, 0xfd, 0x54, 0x03,
    0xfe, 0xfe, 0x00, 0x00, 0x5d, 0xfd, 0x54, 0x03, 0x00, 0x00, 0x00, 0x00, 0x90, 0xfd, 0x54, 0x03,
    0x02, 0x01, 0x00, 0x00, 0x23, 0xfe, 0x54, 0x03, 0xdd, 0x01, 0x00, 0x00, 0xfe, 0xfe, 0x54, 0x03,
    0x70, 0x02, 0x00, 0x00, 0x00, 0x00, 0x54, 0x03, 0xa3, 0x02, 0x00, 0x00, 0x02, 0x01, 0x54, 0x03,
    0x70, 0x02, 0x00, 0x00, 0xdd, 0x01, 0x54, 0x03, 0xdd, 0x01, 0x00, 0x00, 0x70, 0x02, 0x54, 0x03,
    0x02, 0x01, 0x00, 0x00, 0xa3, 0x02, 0x54, 0x03, 0x00, 0x00, 0x00, 0x00, 0x70, 0x02, 0x54, 0x03,
    0xfe, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x54, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x2c, 0x00,
    0xf8, 0x02, 0x80, 0x02, 0x00, 0x03, 0x80, 0x00, 0xa7, 0x8f, 0x08, 0x78, 0x9d, 0x9d, 0x7e, 0x7e,
    0xf0, 0x02, 0xf8, 0x02, 0x00, 0x03, 0x80, 0x00, 0xaa, 0x7e, 0x08, 0x78, 0xa7, 0x8f, 0x7e, 0x7e,
    0xe8, 0x02, 0xf0, 0x02, 0x00, 0x03, 0x80, 0x00, 0xa7, 0x6e, 0x08, 0x78, 0xaa, 0x7e, 0x7e, 0x7e,
    0xe0, 0x02, 0xe8, 0x02, 0x00, 0x03, 0x80, 0x00, 0x9d, 0x61, 0x08, 0x78, 0xa7, 0x6e, 0x7e, 0x7e,
    0xd8, 0x02, 0xe0, 0x02, 0x00, 0x03, 0x80, 0x00, 0x8f, 0x57, 0x08, 0x78, 0x9d, 0x61, 0x7e, 0x7e,
    0xd0, 0x02, 0xd8, 0x02, 0x00, 0x03, 0x80, 0x00, 0x7e, 0x53, 0x08, 0x78, 0x8f, 0x57, 0x7e, 0x7e,
    0xc8, 0x02, 0xd0, 0x02, 0x00, 0x03, 0x80, 0x00, 0x6e, 0x57, 0x08, 0x78, 0x7e, 0x53, 0x7e, 0x7e,
    0xc0, 0x02, 0xc8, 0x02, 0x00, 0x03, 0x80, 0x00, 0x61, 0x61, 0x08, 0x78, 0x6e, 0x57, 0x7e, 0x7e,
    0xb8, 0x02, 0xc0, 0x02, 0x00, 0x03, 0x80, 0x00, 0x57, 0x6e, 0x08, 0x78, 0x61, 0x61, 0x7e, 0x7e,
    0xb0, 0x02, 0xb8, 0x02, 0x00, 0x03, 0x80, 0x00, 0x53, 0x7e, 0x08, 0x78, 0x57, 0x6e, 0x7e, 0x7e,
    0xa8, 0x02, 0xb0, 0x02, 0x00, 0x03, 0x80, 0x00, 0x57, 0x8f, 0x08, 0x78, 0x53, 0x7e, 0x7e, 0x7e,
    0xa0, 0x02, 0xa8, 0x02, 0x00, 0x03, 0x80, 0x00, 0x61, 0x9d, 0x08, 0x78, 0x57, 0x8f, 0x7e, 0x7e,
    0x98, 0x02, 0xa0, 0x02, 0x00, 0x03, 0x80, 0x00, 0x6e, 0xa7, 0x08, 0x78, 0x61, 0x9d, 0x7e, 0x7e,
    0x90, 0x02, 0x98, 0x02, 0x00, 0x03, 0x80, 0x00, 0x7e, 0xaa, 0x08, 0x78, 0x6e, 0xa7, 0x7e, 0x7e,
    0x88, 0x02, 0x90, 0x02, 0x00, 0x03, 0x80, 0x00, 0x8f, 0xa7, 0x08, 0x78, 0x7e, 0xaa, 0x7e, 0x7e,
    0x80, 0x02, 0x88, 0x02, 0x00, 0x03, 0x80, 0x00, 0x9d, 0x9d, 0x08, 0x78, 0x8f, 0xa7, 0x7e, 0x7e,
    0x50, 0x00, 0x00, 0x00, 0x78, 0x02, 0x00, 0x02, 0xf8, 0x02, 0x80, 0x02, 0xc0, 0x9b, 0x08, 0x78,
    0xb1, 0xb1, 0xa7, 0x8f, 0x9d, 0x9d, 0x80, 0x00, 0x70, 0x02, 0x78, 0x02, 0xf0, 0x02, 0xf8, 0x02,
    0xc5, 0x7e, 0x08, 0x78, 0xc0, 0x9b, 0xaa, 0x7e, 0xa7, 0x8f, 0x80, 0x00, 0x68, 0x02, 0x70, 0x02,
    0xe8, 0x02, 0xf0, 0x02, 0xc0, 0x62, 0x08, 0x78, 0xc5, 0x7e, 0xa7, 0x6e, 0xaa, 0x7e, 0x80, 0x00,
    0x60, 0x02, 0x68, 0x02, 0xe0, 0x02, 0xe8, 0x02, 0xb1, 0x4c, 0x08, 0x78, 0xc0, 0x62, 0x9d, 0x61,
    0xa7, 0x6e, 0x80, 0x00, 0x58, 0x02, 0x60, 0x02, 0xd8, 0x02, 0xe0, 0x02, 0x9b, 0x3d, 0x08, 0x78,
    0xb1, 0x4c, 0x8f, 0x57, 0x9d, 0x61, 0x80, 0x00, 0x50, 0x02, 0x58, 0x02, 0xd0, 0x02, 0xd8, 0x02,
    0x7e, 0x38, 0x08, 0x78, 0x9b, 0x3d, 0x7e, 0x53, 0x8f, 0x57, 0x80, 0x00, 0x48, 0x02, 0x50, 0x02,
    0xc8, 0x02, 0xd0, 0x02, 0x62, 0x3d, 0x08, 0x78, 0x7e, 0x38, 0x6e, 0x57, 0x7e, 0x53, 0x80, 0x00,
    0x40, 0x02, 0x48, 0x02, 0xc0, 0x02, 0xc8, 0x02, 0x4c, 0x4c, 0x08, 0x78, 0x62, 0x3d, 0x61, 0x61,
    0x6e, 0x57, 0x80, 0x00, 0x38, 0x02, 0x40, 0x02, 0xb8, 0x02, 0xc0, 0x02, 0x3d, 0x62, 0x08, 0x78,
    0x4c, 0x4c, 0x57, 0x6e, 0x61, 0x61, 0x80, 0x00, 0x30, 0x02, 0x38, 0x02, 0xb0, 0x02, 0xb8, 0x02,
    0x38, 0x80, 0x08, 0x78, 0x3d, 0x62, 0x53, 0x7e, 0x57, 0x6e, 0x80, 0x00, 0x28, 0x02, 0x30, 0x02,
    0xa8, 0x02, 0xb0, 0x02, 0x3d, 0x9b, 0x08, 0x78, 0x38, 0x80, 0x57, 0x8f, 0x53, 0x7e, 0x80, 0x00,
    0x20, 0x02, 0x28, 0x02, 0xa0, 0x02, 0xa8, 0x02, 0x4c, 0xb1, 0x08, 0x78, 0x3d, 0x9b, 0x61, 0x9d,
    0x57, 0x8f, 0x80, 0x00, 0x18, 0x02, 0x20, 0x02, 0x98, 0x02, 0xa0, 0x02, 0x62, 0xc0, 0x08, 0x78,
    0x4c, 0xb1, 0x6e, 0xa7, 0x61, 0x9d, 0x80, 0x00, 0x10, 0x02, 0x18, 0x02, 0x90, 0x02, 0x98, 0x02,
    0x80, 0xc5, 0x08, 0x78, 0x62, 0xc0, 0x7e, 0xaa, 0x6e, 0xa7, 0x80, 0x00, 0x08, 0x02, 0x10, 0x02,
    0x88, 0x02, 0x90, 0x02, 0x9b, 0xc0, 0x08, 0x78, 0x80, 0xc5, 0x8f, 0xa7, 0x7e, 0xaa, 0x80, 0x00,
    0x00, 0x02, 0x08, 0x02, 0x80, 0x02, 0x88, 0x02, 0xb1, 0xb1, 0x08, 0x78, 0x9b, 0xc0, 0x9d, 0x9d,
    0x8f, 0xa7, 0x80, 0x00, 0xf8, 0x01, 0x80, 0x01, 0x78, 0x02, 0x00, 0x02, 0xda, 0xa5, 0x08, 0x78,
    0xc5, 0xc5, 0xc0, 0x9b, 0xb1, 0xb1, 0x80, 0x00, 0xf0, 0x01, 0xf8, 0x01, 0x70, 0x02, 0x78, 0x02,
    0xe1, 0x7e, 0x08, 0x78, 0xda, 0xa5, 0xc5, 0x7e, 0xc0, 0x9b, 0x80, 0x00, 0xe8, 0x01, 0xf0, 0x01,
    0x68, 0x02, 0x70, 0x02, 0xda, 0x58, 0x08, 0x78, 0xe1, 0x7e, 0xc0, 0x62, 0xc5, 0x7e, 0x80, 0x00,
    0xe0, 0x01, 0xe8, 0x01, 0x60, 0x02, 0x68, 0x02, 0xc5, 0x38, 0x08, 0x78, 0xda, 0x58, 0xb1, 0x4c,
    0xc0, 0x62, 0x80, 0x00, 0xd8, 0x01, 0xe0, 0x01, 0x58, 0x02, 0x60, 0x02, 0xa5, 0x23, 0x08, 0x78,
    0xc5, 0x38, 0x9b, 0x3d, 0xb1, 0x4c, 0x80, 0x00, 0xd0, 0x01, 0xd8, 0x01, 0x50, 0x02, 0x58, 0x02,
    0x7e, 0x1d, 0x08, 0x78, 0xa5, 0x23, 0x7e, 0x38, 0x9b, 0x3d, 0x80, 0x00, 0xc8, 0x01, 0xd0, 0x01,
    0x48, 0x02, 0x50, 0x02, 0x58, 0x23, 0x08, 0x78, 0x7e, 0x1d, 0x62, 0x3d, 0x7e, 0x38, 0x80, 0x00,
    0xc0, 0x01, 0xc8, 0x01, 0x40, 0x02, 0x48, 0x02, 0x38, 0x38, 0x08, 0x78, 0x58, 0x23, 0x4c, 0x4c,
    0x62, 0x3d, 0x80, 0x00, 0xb8, 0x01, 0xc0, 0x01, 0x38, 0x02, 0x40, 0x02, 0x23, 0x58, 0x08, 0x78,
    0x38, 0x38, 0x3d, 0x62, 0x4c, 0x4c, 0x80, 0x00, 0xb0, 0x01, 0xb8, 0x01, 0x30, 0x02, 0x38, 0x02,
    0x1d, 0x80, 0x08, 0x78, 0x23, 0x58, 0x38, 0x80, 0x3d, 0x62, 0x80, 0x00, 0xa8, 0x01, 0xb0, 0x01,
    0x28, 0x02, 0x30, 0x02, 0x23, 0xa5, 0x08, 0x78, 0x1d, 0x80, 0x3d, 0x9b, 0x38, 0x80, 0x80, 0x00,
    0xa0, 0x01, 0xa8, 0x01, 0x20, 0x02, 0x28, 0x02, 0x38, 0xc5, 0x08, 0x78, 0x23, 0xa5, 0x4c, 0xb1,
    0x3d, 0x9b, 0x80, 0x00, 0x98, 0x01, 0xa0, 0x01, 0x18, 0x02, 0x20, 0x02, 0x58, 0xda, 0x08, 0x78,
    0x38, 0xc5, 0x62, 0xc0, 0x4c, 0xb1, 0x80, 0x00, 0x90, 0x01, 0x98, 0x01, 0x10, 0x02, 0x18, 0x02,
    0x80, 0xe1, 0x08, 0x78, 0x58, 0xda, 0x80, 0xc5, 0x62, 0xc0, 0x80, 0x00, 0x88, 0x01, 0x90, 0x01,
    0x08, 0x02, 0x10, 0x02, 0xa5, 0xda, 0x08, 0x78, 0x80, 0xe1, 0x9b, 0xc0, 0x80, 0xc5, 0x80, 0x00,
    0x80, 0x01, 0x88, 0x01, 0x00, 0x02, 0x08, 0x02, 0xc5, 0xc5, 0x08, 0x78, 0xa5, 0xda, 0xb1, 0xb1,
    0x9b, 0xc0, 0x80, 0x00, 0x78, 0x01, 0x00, 0x01, 0xf8, 0x01, 0x80, 0x01, 0xe4, 0xa8, 0x08, 0x78,
    0xcc, 0xcc, 0xda, 0xa5, 0xc5, 0xc5, 0x80, 0x00, 0x70, 0x01, 0x78, 0x01, 0xf0, 0x01, 0xf8, 0x01,
    0xed, 0x7e, 0x08, 0x78, 0xe4, 0xa8, 0xe1, 0x7e, 0xda, 0xa5, 0x80, 0x00, 0x68, 0x01, 0x70, 0x01,
    0xe8, 0x01, 0xf0, 0x01, 0xe4, 0x55, 0x08, 0x78, 0xed, 0x7e, 0xda, 0x58, 0xe1, 0x7e, 0x80, 0x00,
    0x60, 0x01, 0x68, 0x01, 0xe0, 0x01, 0xe8, 0x01, 0xcc, 0x31, 0x08, 0x78, 0xe4, 0x55, 0xc5, 0x38,
    0xda, 0x58, 0x80, 0x00, 0x58, 0x01, 0x60, 0x01, 0xd8, 0x01, 0xe0, 0x01, 0xa8, 0x19, 0x08, 0x78,
    0xcc, 0x31, 0xa5, 0x23, 0xc5, 0x38, 0x80, 0x00, 0x50, 0x01, 0x58, 0x01, 0xd0, 0x01, 0xd8, 0x01,
    0x7e, 0x11, 0x08, 0x78, 0xa8, 0x19, 0x7e, 0x1d, 0xa5, 0x23, 0x80, 0x00, 0x48, 0x01, 0x50, 0x01,
    0xc8, 0x01, 0xd0, 0x01, 0x55, 0x19, 0x08, 0x78, 0x7e, 0x11, 0x58, 0x23, 0x7e, 0x1d, 0x80, 0x00,
    0x40, 0x01, 0x48, 0x01, 0xc0, 0x01, 0xc8, 0x01, 0x31, 0x31, 0x08, 0x78, 0x55, 0x19, 0x38, 0x38,
    0x58, 0x23, 0x80, 0x00, 0x38, 0x01, 0x40, 0x01, 0xb8, 0x01, 0xc0, 0x01, 0x19, 0x55, 0x08, 0x78,
    0x31, 0x31, 0x23, 0x58, 0x38, 0x38, 0x80, 0x00, 0x30, 0x01, 0x38, 0x01, 0xb0, 0x01, 0xb8, 0x01,
    0x11, 0x80, 0x08, 0x78, 0x19, 0x55, 0x1d, 0x80, 0x23, 0x58, 0x80, 0x00, 0x28, 0x01, 0x30, 0x01,
    0xa8, 0x01, 0xb0, 0x01, 0x19, 0xa8, 0x08, 0x78, 0x11, 0x80, 0x23, 0xa5, 0x1d, 0x80, 0x80, 0x00,
    0x20, 0x01, 0x28, 0x01, 0xa0, 0x01, 0xa8, 0x01, 0x31, 0xcc, 0x08, 0x78, 0x19, 0xa8, 0x38, 0xc5,
    0x23, 0xa5, 0x80, 0x00, 0x18, 0x01, 0x20, 0x01, 0x98, 0x01, 0xa0, 0x01, 0x55, 0xe4, 0x08, 0x78,
    0x31, 0xcc, 0x58, 0xda, 0x38, 0xc5, 0x80, 0x00, 0x10, 0x01, 0x18, 0x01, 0x90, 0x01, 0x98, 0x01,
    0x80, 0xed, 0x08, 0x78, 0x55, 0xe4, 0x80, 0xe1, 0x58, 0xda, 0x80, 0x00, 0x08, 0x01, 0x10, 0x01,
    0x88, 0x01, 0x90, 0x01, 0xa8, 0xe4, 0x08, 0x78, 0x80, 0xed, 0xa5, 0xda, 0x80, 0xe1, 0x80, 0x00,
    0x00, 0x01, 0x08, 0x01, 0x80, 0x01, 0x88, 0x01, 0xcc, 0xcc, 0x08, 0x78, 0xa8, 0xe4, 0xc5, 0xc5,
    0xa5, 0xda, 0x80, 0x00, 0xf0, 0x00, 0xf8, 0x00, 0x78, 0x01, 0x00, 0x01, 0xf0, 0xae, 0x08, 0x78,
    0xd5, 0xd5, 0xe4, 0xa8, 0xcc, 0xcc, 0x80, 0x00, 0xe8, 0x00, 0xf0, 0x00, 0x70, 0x01, 0x78, 0x01,
    0xf9, 0x7e, 0x08, 0x78, 0xf0, 0xae, 0xed, 0x7e, 0xe4, 0xa8, 0x80, 0x00, 0xe0, 0x00, 0xe8, 0x00,
    0x68, 0x01, 0x70, 0x01, 0xf0, 0x50, 0x08, 0x78, 0xf9, 0x7e, 0xe4, 0x55, 0xed, 0x7e, 0x80, 0x00,
    0xd8, 0x00, 0xe0, 0x00, 0x60, 0x01, 0x68, 0x01, 0xd5, 0x28, 0x08, 0x78, 0xf0, 0x50, 0xcc, 0x31,
    0xe4, 0x55, 0x80, 0x00, 0xd0, 0x00, 0xd8, 0x00, 0x58, 0x01, 0x60, 0x01, 0xae, 0x0d, 0x08, 0x78,
    0xd5, 0x28, 0xa8, 0x19, 0xcc, 0x31, 0x80, 0x00, 0xc8, 0x00, 0xd0, 0x00, 0x50, 0x01, 0x58, 0x01,
    0x7e, 0x05, 0x08, 0x78, 0xae, 0x0d, 0x7e, 0x11, 0xa8, 0x19, 0x80, 0x00, 0xc0, 0x00, 0xc8, 0x00,
    0x48, 0x01, 0x50, 0x01, 0x50, 0x0d, 0x08, 0x78, 0x7e, 0x05, 0x55, 0x19, 0x7e, 0x11, 0x80, 0x00,
    0xb8, 0x00, 0xc0, 0x00, 0x40, 0x01, 0x48, 0x01, 0x28, 0x28, 0x08, 0x78, 0x50, 0x0d, 0x31, 0x31,
    0x55, 0x19, 0x80, 0x00, 0xb0, 0x00, 0xb8, 0x00, 0x38, 0x01, 0x40, 0x01, 0x0d, 0x50, 0x08, 0x78,
    0x28, 0x28, 0x19, 0x55, 0x31, 0x31, 0x80, 0x00, 0xa8, 0x00, 0xb0, 0x00, 0x30, 0x01, 0x38, 0x01,
    0x05, 0x80, 0x08, 0x78, 0x0d, 0x50, 0x11, 0x80, 0x19, 0x55, 0x80, 0x00, 0xa0, 0x00, 0xa8, 0x00,
    0x28, 0x01, 0x30, 0x01, 0x0d, 0xae, 0x08, 0x78, 0x05, 0x80, 0x19, 0xa8, 0x11, 0x80, 0x80, 0x00,
    0x98, 0x00, 0xa0, 0x00, 0x20, 0x01, 0x28, 0x01, 0x28, 0xd5, 0x08, 0x78, 0x0d, 0xae, 0x31, 0xcc,
    0x19, 0xa8, 0x80, 0x00, 0x90, 0x00, 0x98, 0x00, 0x18, 0x01, 0x20, 0x01, 0x50, 0xf0, 0x08, 0x78,
    0x28, 0xd5, 0x55, 0xe4, 0x31, 0xcc, 0x80, 0x00, 0x88, 0x00, 0x90, 0x00, 0x10, 0x01, 0x18, 0x01,
    0x80, 0xf9, 0x08, 0x78, 0x50, 0xf0, 0x80, 0xed, 0x55, 0xe4, 0x80, 0x00, 0x80, 0x00, 0x88, 0x00,
    0x08, 0x01, 0x10, 0x01, 0xae, 0xf0, 0x08, 0x78, 0x80, 0xf9, 0xa8, 0xe4, 0x80, 0xed, 0x80, 0x00,
    0xf8, 0x00, 0x80, 0x00, 0x00, 0x01, 0x08, 0x01, 0xd5, 0xd5, 0x08, 0x78, 0xae, 0xf0, 0xcc, 0xcc,
    0xa8, 0xe4, 0x80, 0x00, 0x10, 0x00, 0x08, 0x00, 0xf8, 0x00, 0x80, 0x00, 0xda, 0xda, 0x08, 0x78,
    0xaf, 0xf5, 0xd5, 0xd5, 0xae, 0xf0, 0x80, 0x00, 0x18, 0x00, 0x10, 0x00, 0xf0, 0x00, 0xf8, 0x00,
    0xf5, 0xaf, 0x08, 0x78, 0xda, 0xda, 0xf0, 0xae, 0xd5, 0xd5, 0x80, 0x00, 0x20, 0x00, 0x18, 0x00,
    0xe8, 0x00, 0xf0, 0x00, 0xfe, 0x7e, 0x08, 0x78, 0xf5, 0xaf, 0xf9, 0x7e, 0xf0, 0xae, 0x80, 0x00,
    0x28, 0x00, 0x20, 0x00, 0xe0, 0x00, 0xe8, 0x00, 0xf5, 0x4e, 0x08, 0x78, 0xfe, 0x7e, 0xf0, 0x50,
    0xf9, 0x7e, 0x80, 0x00, 0x30, 0x00, 0x28, 0x00, 0xd8, 0x00, 0xe0, 0x00, 0xda, 0x23, 0x08, 0x78,
    0xf5, 0x4e, 0xd5, 0x28, 0xf0, 0x50, 0x80, 0x00, 0x38, 0x00, 0x30, 0x00, 0xd0, 0x00, 0xd8, 0x00,
    0xaf, 0x08, 0x08, 0x78, 0xda, 0x23, 0xae, 0x0d, 0xd5, 0x28, 0x80, 0x00, 0x40, 0x00, 0x38, 0x00,
    0xc8, 0x00, 0xd0, 0x00, 0x7e, 0x00, 0x08, 0x78, 0xaf, 0x08, 0x7e, 0x05, 0xae, 0x0d, 0x80, 0x00,
    0x48, 0x00, 0x40, 0x00, 0xc0, 0x00, 0xc8, 0x00, 0x4e, 0x08, 0x08, 0x78, 0x7e, 0x00, 0x50, 0x0d,
    0x7e, 0x05, 0x80, 0x00, 0x50, 0x00, 0x48, 0x00, 0xb8, 0x00, 0xc0, 0x00, 0x23, 0x23, 0x08, 0x78,
    0x4e, 0x08, 0x28, 0x28, 0x50, 0x0d, 0x80, 0x00, 0x58, 0x00, 0x50, 0x00, 0xb0, 0x00, 0xb8, 0x00,
    0x08, 0x4e, 0x08, 0x78, 0x23, 0x23, 0x0d, 0x50, 0x28, 0x28, 0x80, 0x00, 0x60, 0x00, 0x58, 0x00,
    0xa8, 0x00, 0xb0, 0x00, 0x00, 0x80, 0x08, 0x78, 0x08, 0x4e, 0x05, 0x80, 0x0d, 0x50, 0x80, 0x00,
    0x68, 0x00, 0x60, 0x00, 0xa0, 0x00, 0xa8, 0x00, 0x08, 0xaf, 0x08, 0x78, 0x00, 0x80, 0x0d, 0xae,
    0x05, 0x80, 0x80, 0x00, 0x70, 0x00, 0x68, 0x00, 0x98, 0x00, 0xa0, 0x00, 0x23, 0xda, 0x08, 0x78,
    0x08, 0xaf, 0x28, 0xd5, 0x0d, 0xae, 0x80, 0x00, 0x78, 0x00, 0x70, 0x00, 0x90, 0x00, 0x98, 0x00,
    0x4e, 0xf5, 0x08, 0x78, 0x23, 0xda, 0x50, 0xf0, 0x28, 0xd5, 0x80, 0x00, 0x00, 0x00, 0x78, 0x00,
    0x88, 0x00, 0x90, 0x00, 0x80, 0xfe, 0x08, 0x78, 0x4e, 0xf5, 0x80, 0xf9, 0x50, 0xf0, 0x80, 0x00,
    0x08, 0x00, 0x00, 0x00, 0x80, 0x00, 0x88, 0x00, 0xaf, 0xf5, 0x08, 0x78, 0x80, 0xfe, 0xae, 0xf0,
    0x80, 0xf9, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// GLOBAL: FF7 0x008b2900
int g_BattleMagicQuake2ResourceDefs = 79929;
int g_BattleMagicQuake2ResourceDefsHeadPtr[1] = {
    (int)&g_BattleFxMagicQuake2ResourceNames[0x10]
};
int g_BattleMagicQuake2ResourceDefsDataTail[54] = {
    0x0, 0x0, 0x0, 0x13839, (int)&g_BattleFxMagicQuake2ResourceNames[0x1c], 0x0,
    0x0, 0x0, 0x1283c, (int)&g_BattleFxMagicQuake2ResourceNames[0x28], 0x0, 0x0,
    0x0, 0x12839, (int)&g_BattleFxMagicQuake2ResourceNames[0x34], 0x0, 0x0, 0x0,
    0x13c39, (int)&g_BattleFxMagicQuake2ResourceNames[0x40], 0x0, 0x0, 0x0, 0x13839,
    (int)&g_BattleFxMagicQuake2ResourceNames[0x4c], 0x0, 0x0, 0x0, 0x1900b1, (int)&g_BattleFxMagicQuake2ResourceNames[0x58],
    0x0, 0x0, 0x0, 0x1900b1, (int)&g_BattleFxMagicQuake2ResourceNames[0x64], 0x0,
    0x0, 0x0, 0x1c00b4, (int)&g_BattleFxMagicQuake2ResourceNames[0x70], 0x0, 0x0,
    0x0, 0x13c3c, (int)&g_BattleFxMagicQuake2ResourceNames[0x7c], 0x0, 0x0, 0x0,
    0x1383c, (int)&g_BattleFxMagicQuake2ResourceNames[0x88], 0x0, 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x008b29e0
short g_BattleMagicQueirSineTable[1025] = {
    0, 6, 13, 19, 25, 31, 38, 44, 50, 57, 63, 69, 75, 82, 88, 94,
    101, 107, 113, 119, 126, 132, 138, 144, 151, 157, 163, 170, 176, 182, 188, 195,
    201, 207, 214, 220, 226, 232, 239, 245, 251, 257, 264, 270, 276, 283, 289, 295,
    301, 308, 314, 320, 326, 333, 339, 345, 351, 358, 364, 370, 376, 383, 389, 395,
    401, 408, 414, 420, 426, 433, 439, 445, 451, 458, 464, 470, 476, 483, 489, 495,
    501, 508, 514, 520, 526, 533, 539, 545, 551, 557, 564, 570, 576, 582, 589, 595,
    601, 607, 613, 620, 626, 632, 638, 644, 651, 657, 663, 669, 675, 682, 688, 694,
    700, 706, 713, 719, 725, 731, 737, 744, 750, 756, 762, 768, 774, 781, 787, 793,
    799, 805, 811, 818, 824, 830, 836, 842, 848, 854, 861, 867, 873, 879, 885, 891,
    897, 904, 910, 916, 922, 928, 934, 940, 946, 953, 959, 965, 971, 977, 983, 989,
    995, 1001, 1007, 1014, 1020, 1026, 1032, 1038, 1044, 1050, 1056, 1062, 1068, 1074, 1080, 1086,
    1092, 1099, 1105, 1111, 1117, 1123, 1129, 1135, 1141, 1147, 1153, 1159, 1165, 1171, 1177, 1183,
    1189, 1195, 1201, 1207, 1213, 1219, 1225, 1231, 1237, 1243, 1249, 1255, 1261, 1267, 1273, 1279,
    1285, 1291, 1297, 1303, 1309, 1315, 1321, 1327, 1332, 1338, 1344, 1350, 1356, 1362, 1368, 1374,
    1380, 1386, 1392, 1398, 1404, 1409, 1415, 1421, 1427, 1433, 1439, 1445, 1451, 1457, 1462, 1468,
    1474, 1480, 1486, 1492, 1498, 1503, 1509, 1515, 1521, 1527, 1533, 1538, 1544, 1550, 1556, 1562,
    1567, 1573, 1579, 1585, 1591, 1596, 1602, 1608, 1614, 1620, 1625, 1631, 1637, 1643, 1648, 1654,
    1660, 1666, 1671, 1677, 1683, 1689, 1694, 1700, 1706, 1711, 1717, 1723, 1729, 1734, 1740, 1746,
    1751, 1757, 1763, 1768, 1774, 1780, 1785, 1791, 1797, 1802, 1808, 1813, 1819, 1825, 1830, 1836,
    1842, 1847, 1853, 1858, 1864, 1870, 1875, 1881, 1886, 1892, 1898, 1903, 1909, 1914, 1920, 1925,
    1931, 1936, 1942, 1947, 1953, 1958, 1964, 1970, 1975, 1981, 1986, 1992, 1997, 2002, 2008, 2013,
    2019, 2024, 2030, 2035, 2041, 2046, 2052, 2057, 2062, 2068, 2073, 2079, 2084, 2090, 2095, 2100,
    2106, 2111, 2117, 2122, 2127, 2133, 2138, 2143, 2149, 2154, 2159, 2165, 2170, 2175, 2181, 2186,
    2191, 2197, 2202, 2207, 2213, 2218, 2223, 2228, 2234, 2239, 2244, 2249, 2255, 2260, 2265, 2270,
    2276, 2281, 2286, 2291, 2296, 2302, 2307, 2312, 2317, 2322, 2328, 2333, 2338, 2343, 2348, 2353,
    2359, 2364, 2369, 2374, 2379, 2384, 2389, 2394, 2399, 2405, 2410, 2415, 2420, 2425, 2430, 2435,
    2440, 2445, 2450, 2455, 2460, 2465, 2470, 2475, 2480, 2485, 2490, 2495, 2500, 2505, 2510, 2515,
    2520, 2525, 2530, 2535, 2540, 2545, 2550, 2555, 2559, 2564, 2569, 2574, 2579, 2584, 2589, 2594,
    2598, 2603, 2608, 2613, 2618, 2623, 2628, 2632, 2637, 2642, 2647, 2652, 2656, 2661, 2666, 2671,
    2675, 2680, 2685, 2690, 2694, 2699, 2704, 2709, 2713, 2718, 2723, 2727, 2732, 2737, 2741, 2746,
    2751, 2755, 2760, 2765, 2769, 2774, 2779, 2783, 2788, 2792, 2797, 2802, 2806, 2811, 2815, 2820,
    2824, 2829, 2833, 2838, 2843, 2847, 2852, 2856, 2861, 2865, 2870, 2874, 2878, 2883, 2887, 2892,
    2896, 2901, 2905, 2910, 2914, 2918, 2923, 2927, 2932, 2936, 2940, 2945, 2949, 2953, 2958, 2962,
    2967, 2971, 2975, 2979, 2984, 2988, 2992, 2997, 3001, 3005, 3009, 3014, 3018, 3022, 3026, 3031,
    3035, 3039, 3043, 3048, 3052, 3056, 3060, 3064, 3068, 3073, 3077, 3081, 3085, 3089, 3093, 3097,
    3102, 3106, 3110, 3114, 3118, 3122, 3126, 3130, 3134, 3138, 3142, 3146, 3150, 3154, 3158, 3162,
    3166, 3170, 3174, 3178, 3182, 3186, 3190, 3194, 3198, 3202, 3206, 3210, 3214, 3217, 3221, 3225,
    3229, 3233, 3237, 3241, 3244, 3248, 3252, 3256, 3260, 3264, 3267, 3271, 3275, 3279, 3282, 3286,
    3290, 3294, 3297, 3301, 3305, 3309, 3312, 3316, 3320, 3323, 3327, 3331, 3334, 3338, 3342, 3345,
    3349, 3352, 3356, 3360, 3363, 3367, 3370, 3374, 3378, 3381, 3385, 3388, 3392, 3395, 3399, 3402,
    3406, 3409, 3413, 3416, 3420, 3423, 3426, 3430, 3433, 3437, 3440, 3444, 3447, 3450, 3454, 3457,
    3461, 3464, 3467, 3471, 3474, 3477, 3481, 3484, 3487, 3490, 3494, 3497, 3500, 3504, 3507, 3510,
    3513, 3516, 3520, 3523, 3526, 3529, 3532, 3536, 3539, 3542, 3545, 3548, 3551, 3555, 3558, 3561,
    3564, 3567, 3570, 3573, 3576, 3579, 3582, 3585, 3588, 3591, 3594, 3597, 3600, 3603, 3606, 3609,
    3612, 3615, 3618, 3621, 3624, 3627, 3630, 3633, 3636, 3639, 3642, 3644, 3647, 3650, 3653, 3656,
    3659, 3661, 3664, 3667, 3670, 3673, 3675, 3678, 3681, 3684, 3686, 3689, 3692, 3695, 3697, 3700,
    3703, 3705, 3708, 3711, 3713, 3716, 3719, 3721, 3724, 3727, 3729, 3732, 3734, 3737, 3739, 3742,
    3745, 3747, 3750, 3752, 3755, 3757, 3760, 3762, 3765, 3767, 3770, 3772, 3775, 3777, 3779, 3782,
    3784, 3787, 3789, 3791, 3794, 3796, 3798, 3801, 3803, 3805, 3808, 3810, 3812, 3815, 3817, 3819,
    3822, 3824, 3826, 3828, 3831, 3833, 3835, 3837, 3839, 3842, 3844, 3846, 3848, 3850, 3852, 3854,
    3857, 3859, 3861, 3863, 3865, 3867, 3869, 3871, 3873, 3875, 3877, 3879, 3881, 3883, 3885, 3887,
    3889, 3891, 3893, 3895, 3897, 3899, 3901, 3903, 3905, 3907, 3909, 3910, 3912, 3914, 3916, 3918,
    3920, 3921, 3923, 3925, 3927, 3929, 3930, 3932, 3934, 3936, 3937, 3939, 3941, 3943, 3944, 3946,
    3948, 3949, 3951, 3953, 3954, 3956, 3958, 3959, 3961, 3962, 3964, 3965, 3967, 3969, 3970, 3972,
    3973, 3975, 3976, 3978, 3979, 3981, 3982, 3984, 3985, 3987, 3988, 3989, 3991, 3992, 3994, 3995,
    3996, 3998, 3999, 4001, 4002, 4003, 4005, 4006, 4007, 4008, 4010, 4011, 4012, 4014, 4015, 4016,
    4017, 4019, 4020, 4021, 4022, 4023, 4024, 4026, 4027, 4028, 4029, 4030, 4031, 4032, 4034, 4035,
    4036, 4037, 4038, 4039, 4040, 4041, 4042, 4043, 4044, 4045, 4046, 4047, 4048, 4049, 4050, 4051,
    4052, 4053, 4053, 4054, 4055, 4056, 4057, 4058, 4059, 4060, 4060, 4061, 4062, 4063, 4064, 4064,
    4065, 4066, 4067, 4067, 4068, 4069, 4070, 4070, 4071, 4072, 4072, 4073, 4074, 4074, 4075, 4076,
    4076, 4077, 4077, 4078, 4079, 4079, 4080, 4080, 4081, 4081, 4082, 4082, 4083, 4083, 4084, 4084,
    4085, 4085, 4086, 4086, 4087, 4087, 4088, 4088, 4088, 4089, 4089, 4089, 4090, 4090, 4090, 4091,
    4091, 4091, 4092, 4092, 4092, 4092, 4093, 4093, 4093, 4093, 4094, 4094, 4094, 4094, 4094, 4095,
    4095, 4095, 4095, 4095, 4095, 4095, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096};
// GLOBAL: FF7 0x008b31e8
unsigned char g_BattleMagicQuake2SpriteDrawReq[8] = {0xf0, 0xff, 0xe0, 0xff, 0x00, 0x00, 0x1f, 0x1f};
// GLOBAL: FF7 0x008b31f0
unsigned char g_BattleMagicQuake2ModelConfig[28] = {
    0x80, 0x80, 0x80, 0x2c, 0x2d, 0x00, 0xc8, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};
unsigned char g_BattleFxMagicQuake2ResourceNames[148] = {
0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x4c, 0x49, 0x47, 0x48, 0x54, 0x33, 0x34, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x4c, 0x49, 0x47, 0x48,
0x54, 0x5f, 0x35, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x53, 0x54, 0x4f, 0x4e, 0x45, 0x2e, 0x54, 0x49,
0x4d, 0x00, 0x00, 0x00, 0x53, 0x54, 0x4f, 0x4e, 0x45, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x00,
0x51, 0x55, 0x45, 0x49, 0x52, 0x5f, 0x31, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x51, 0x55, 0x45, 0x49,
0x52, 0x5f, 0x31, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x6c, 0x69, 0x67, 0x68, 0x74, 0x5f, 0x31, 0x2e,
0x72, 0x73, 0x64, 0x00, 0x6c, 0x69, 0x67, 0x68, 0x74, 0x5f, 0x32, 0x2e, 0x72, 0x73, 0x64, 0x00,
0x71, 0x75, 0x65, 0x69, 0x72, 0x2e, 0x72, 0x73, 0x64, 0x00, 0x00, 0x00, 0x51, 0x55, 0x45, 0x49,
0x52, 0x5f, 0x31, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x51, 0x55, 0x45, 0x49, 0x52, 0x5f, 0x31, 0x2e,
0x54, 0x49, 0x4d, 0x00};
// GLOBAL: FF7 0x00bb1460
struct BattleMagicQuake2Shard g_BattleMagicQuake2ShardRecords[96] = {0};
// GLOBAL: FF7 0x00bb1d60
int g_BattleMagicQuake2TargetMask = 0;
// GLOBAL: FF7 0x00bb1d64
struct GfxTableCursor *g_BattleMagicQuake2LoadedResTable = 0;
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/*
 * Loads resources for the Queir magic effect.
 */
// FUNCTION: FF7 0x00585330
void __cdecl BattleFx_MagicLoadResQuake2(void)
{
    GfxResourceRequest requestData;
    GfxModelTableBank *context;
    int defCount;
    char pathBuffer[200];
    GfxResourceRequest *resourceRequest;
    int resultDefRecs;

    if (g_pBattle3dScene != 0 && g_BattleMagicQuake2LoadedResTable == 0) {
        defCount = 11;
        resultDefRecs = (int)&g_BattleMagicQuake2ResourceDefs;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        resourceRequest = &requestData;
        Gfx_InitResourceRequest(resourceRequest);
        resourceRequest->groupTag = 0;
        // STRING: FF7 0x008b32a4
        resourceRequest->resourcePath = BattleFx_BuildResourcePath("magic/queir", pathBuffer);
        Gfx_LoadModelTableIfAbsent(defCount, resultDefRecs, resourceRequest, &g_BattleMagicQuake2LoadedResTable, context);
    }
}

/*
 * Entry point for the Queir magic effect.
 */
// FUNCTION: FF7 0x005853c0
void __cdecl BattleFx_MagicEnterQuake2(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResQuake2();
    BattleFx_MagicBeginQuake2(targetMask, actorIdx);
}

/*
 * Initializes Queir effect and schedules timeline selection task.
 */
// FUNCTION: FF7 0x005853da
void __cdecl BattleFx_MagicBeginQuake2(int targetMask, int actorIdx)
{
    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    Battle_AllocateResourceSlot(BattleFx_MagicQuake2SelectTimelineTask);
    g_BattleMagicQuake2TargetMask = targetMask;
}

/*
 * Selects single- or multi-target Queir timeline task.
 */
// FUNCTION: FF7 0x00585412
void __cdecl BattleFx_MagicQuake2SelectTimelineTask(void)
{
    BattleFxQuake2ShardBurst *shards;
    int idx;
    int targetCount;
    BattleFxQuake2SelectTask *task;
    int firstTarget;

    idx = 0;
    for (targetCount = 0; idx < 10; idx++) {
        if ((g_BattleMagicQuake2TargetMask & 1 << idx) != 0) {
            targetCount++;
            if (targetCount == 1) {
                firstTarget = idx;
            }
        }
    }
    task = (BattleFxQuake2SelectTask *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    if (targetCount == 1) {
        task = (BattleFxQuake2SelectTask *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicQuake2SingleTargetTimelineTask);
        task->phase = 0;
        Battle_GetBoneWorldPos(firstTarget, g_BattleActorRecords[firstTarget].targetBone, &task->posX);
        task->originY = 0;
        task->size = g_BattleActorRecords[firstTarget].radius + 224;
        if (task->size > 1900) {
            task->size = 1900;
        }
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(g_BattleMagicQuake2TargetMask), 0x40);
    } else {
        task = (BattleFxQuake2SelectTask *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicQuake2MultiTargetTimelineTask);
        task->phase = 0;
        Battle_GetActorMaskCentroid(g_BattleMagicQuake2TargetMask, &task->posX);
        task->size = 1120;
        shards = (BattleFxQuake2ShardBurst *)Gfx_GetPsxScratchpadPtr(0);
        shards->meshPayload = (int)&g_BattleMagicQuake2MeshBurstPayload;
        BattleFx_MagicQuake2BuildShardRecords(shards);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(g_BattleMagicQuake2TargetMask), 0xD4);
    }
}

/*
 * Builds triangle and quad shard records for Queir mesh.
 */
// FUNCTION: FF7 0x005855b7
void __cdecl BattleFx_MagicQuake2BuildShardRecords(BattleFxQuake2ShardBurst *burst)
{
    int meshBase;
    BattleFxQuake2ShardRecord *records;

    meshBase = burst->meshPayload + 4;
    records = (BattleFxQuake2ShardRecord *)(meshBase + *(int *)burst->meshPayload);
    burst->shardIndex = 0;
    records = (BattleFxQuake2ShardRecord *)BattleFx_MagicQuake2BuildTriangleShardRecords(burst, meshBase, records);
    records = (BattleFxQuake2ShardRecord *)BattleFx_MagicQuake2BuildQuadShardRecords(burst, meshBase, records);
}

/*
 * Builds triangle shard particle records for Queir.
 */
// FUNCTION: FF7 0x00585611
int __cdecl BattleFx_MagicQuake2BuildTriangleShardRecords(BattleFxQuake2ShardBurst *burst, int meshBase, struct BattleFxQuake2ShardRecord *recordHeader)
{
    BattleFxQuake2ShardRecord *recordPtr;
    int sumX;
    int sumY;
    int shardIndex;
    int i;
    Vec3sPacked *vertex;
    int sumZ;
    int rnd;
    int normDivisor;
    int numTris;

    numTris = *(int *)recordHeader & 0xFFFF;
    shardIndex = burst->shardIndex;
    recordPtr = (BattleFxQuake2ShardRecord *)((char *)recordHeader + 4);
    normDivisor = 0x24B90;
    for (i = 0; i < numTris; i++) {
        vertex = (Vec3sPacked *)(meshBase + recordPtr->shardOffset0);
        sumX = vertex->vx;
        sumY = vertex->vy;
        sumZ = vertex->vz;
        vertex = (Vec3sPacked *)(meshBase + recordPtr->shardOffset1);
        sumX += vertex->vx;
        sumY += vertex->vy;
        sumZ += vertex->vz;
        vertex = (Vec3sPacked *)(meshBase + recordPtr->shardOffset2);
        sumX += vertex->vx;
        sumY += vertex->vy;
        sumZ += vertex->vz;
        sumX = sumX / 3;
        sumY = sumY / 3;
        sumZ = sumZ / 3;
        g_BattleMagicQuake2ShardRecords[shardIndex].positionX = sumX;
        g_BattleMagicQuake2ShardRecords[shardIndex].centroidY = sumY;
        g_BattleMagicQuake2ShardRecords[shardIndex].centroidZ = sumZ;
        sumY = (sumX * sumX + sumZ * sumZ) / normDivisor;
        g_BattleMagicQuake2ShardRecords[shardIndex].positionY = sumY;
        g_BattleMagicQuake2ShardRecords[shardIndex].fadeCounter = 0;
        g_BattleMagicQuake2ShardRecords[shardIndex].rotationZ = 0;
        g_BattleMagicQuake2ShardRecords[shardIndex].rotationY = 0;
        g_BattleMagicQuake2ShardRecords[shardIndex].rotationX = 0;
        rnd = rand();
        if ((rnd & 0x800) != 0) {
            g_BattleMagicQuake2ShardRecords[shardIndex].rotationSpeedX = (rnd & 7) << 4;
        } else {
            g_BattleMagicQuake2ShardRecords[shardIndex].rotationSpeedX = -((rnd & 7) << 4);
        }
        g_BattleMagicQuake2ShardRecords[shardIndex].offsetY = 0;
        if ((rnd & 0x204080) != 0) {
            g_BattleMagicQuake2ShardRecords[shardIndex].rotationSpeedZ = (rnd >> 3 & 7) << 4;
        } else {
            g_BattleMagicQuake2ShardRecords[shardIndex].rotationSpeedZ = -((rnd >> 3 & 7) << 4);
        }
        g_BattleMagicQuake2ShardRecords[shardIndex].fallSpeed = (28 - sumY) * ((rnd & 1) + 3);
        shardIndex++;
        recordPtr += 2;
    }
    burst->shardIndex = shardIndex;
    return (int)recordPtr;
}

/*
 * Builds quad shard particle records for Queir.
 */
// FUNCTION: FF7 0x005858a1
int __cdecl BattleFx_MagicQuake2BuildQuadShardRecords(BattleFxQuake2ShardBurst *burst, int base, struct BattleFxQuake2ShardRecord *recordList)
{
    BattleFxQuake2ShardRecord *recordPtr;
    int sumX;
    int sumY;
    int i;
    Vec3sPacked *entry;
    int sumZ;
    int numberRecords;
    int normFactor;
    int rnd;
    int dstIndex;

    numberRecords = *(int *)recordList & 0xFFFF;
    dstIndex = burst->shardIndex;
    recordPtr = (BattleFxQuake2ShardRecord *)((char *)recordList + 4);
    normFactor = 0x24B90;
    for (i = 0; i < numberRecords; i++) {
        entry = (Vec3sPacked *)(base + recordPtr->shardOffset0);
        sumX = entry->vx;
        sumY = entry->vy;
        sumZ = entry->vz;
        entry = (Vec3sPacked *)(base + recordPtr->shardOffset1);
        sumX += entry->vx;
        sumY += entry->vy;
        sumZ += entry->vz;
        entry = (Vec3sPacked *)(base + recordPtr->shardOffset2);
        sumX += entry->vx;
        sumY += entry->vy;
        sumZ += entry->vz;
        entry = (Vec3sPacked *)(base + recordPtr->shardOffset3);
        sumX += entry->vx;
        sumY += entry->vy;
        sumZ += entry->vz;
        sumX /= 4;
        sumY /= 4;
        sumZ /= 4;
        g_BattleMagicQuake2ShardRecords[dstIndex].positionX = sumX;
        g_BattleMagicQuake2ShardRecords[dstIndex].centroidY = sumY;
        g_BattleMagicQuake2ShardRecords[dstIndex].centroidZ = sumZ;
        sumY = (sumX * sumX + sumZ * sumZ) / normFactor;
        g_BattleMagicQuake2ShardRecords[dstIndex].positionY = sumY;
        g_BattleMagicQuake2ShardRecords[dstIndex].fadeCounter = 0;
        g_BattleMagicQuake2ShardRecords[dstIndex].rotationZ = 0;
        g_BattleMagicQuake2ShardRecords[dstIndex].rotationY = 0;
        g_BattleMagicQuake2ShardRecords[dstIndex].rotationX = 0;
        rnd = rand();
        if ((rnd & 0x800) != 0) {
            g_BattleMagicQuake2ShardRecords[dstIndex].rotationSpeedX = (rnd & 7) << 4;
        } else {
            g_BattleMagicQuake2ShardRecords[dstIndex].rotationSpeedX = -((rnd & 7) << 4);
        }
        g_BattleMagicQuake2ShardRecords[dstIndex].offsetY = 0;
        if ((rnd & 0x204080) != 0) {
            g_BattleMagicQuake2ShardRecords[dstIndex].rotationSpeedZ = (rnd >> 3 & 7) << 4;
        } else {
            g_BattleMagicQuake2ShardRecords[dstIndex].rotationSpeedZ = -((rnd >> 3 & 7) << 4);
        }
        g_BattleMagicQuake2ShardRecords[dstIndex].fallSpeed = (28 - sumY) * ((rnd & 1) + 3);
        dstIndex++;
        recordPtr = (BattleFxQuake2ShardRecord *)((char *)recordPtr + 0x14);
    }
    burst->shardIndex = dstIndex;
    return (int)recordPtr;
}

/*
 * Timeline task driving multi-target Queir effect.
 */
// FUNCTION: FF7 0x00585b73
void __cdecl BattleFx_MagicQuake2MultiTargetTimelineTask(void)
{
    BattleFxQuake2TimelineRecord *record;
    BattleFxQuake2TimelineTask *task;

    if (g_BattleIsPaused != 0) {
        return;
    }
    task = (BattleFxQuake2TimelineTask *)BATTLE_CURRENT_TASK_SLOT;
    /* Numeric timeline step index. */
    switch (task->phase) {
    case 0:
        if (task->frameCount < 1) {
            break;
        }
        (task->phase)++;
        record = (BattleFxQuake2TimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicQuake2FadingModelTask);
        record->fadeStart = 23;
        *(Int64Pair *)&record->posX = *(Int64Pair *)&task->posX;
        record->spinAngleY = 0;
        record->baseAngleY = 0x100 /* 22.5 deg */;
        record->size = task->size << 1;
        record->sizeStep = 0;
        record->fadeLevel = 0;
        record->fadeStep = 186;
        record->isActive = 1;
        break;
    case 1:
        if (task->frameCount < 5) {
            break;
        }
        (task->phase)++;
        record = (BattleFxQuake2TimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicQuake2FadingModelTask);
        record->fadeStart = 25;
        *(Int64Pair *)&record->posX = *(Int64Pair *)&task->posX;
        record->spinAngleY = 0;
        record->baseAngleY = 0x900 /* 202.5 deg */;
        record->size = task->size << 1;
        record->sizeStep = 0;
        record->fadeLevel = 0;
        record->fadeStep = 195;
        record->isActive = 1;
        break;
    case 2:
        if (task->frameCount < 10) {
            break;
        }
        (task->phase)++;
        record = (BattleFxQuake2TimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicQuake2FadingModelTask);
        record->fadeStart = 26;
        *(Int64Pair *)&record->posX = *(Int64Pair *)&task->posX;
        record->spinAngleY = 0;
        record->baseAngleY = 0x100 /* 22.5 deg */;
        record->size = task->size;
        record->sizeStep = 0;
        record->fadeLevel = 0;
        record->fadeStep = 273;
        record->isActive = 1;
        break;
    case 3:
        if (task->frameCount < 14) {
            break;
        }
        (task->phase)++;
        record = (BattleFxQuake2TimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicQuake2FadingModelTask);
        record->fadeStart = 27;
        *(Int64Pair *)&record->posX = *(Int64Pair *)&task->posX;
        record->spinAngleY = 0;
        record->baseAngleY = 0x900 /* 202.5 deg */;
        record->size = task->size;
        record->sizeStep = 0;
        record->fadeLevel = 0;
        record->fadeStep = 315;
        record->isActive = 1;
        break;
    case 4:
        if (task->frameCount < 20) {
            break;
        }
        (task->phase)++;
        record = (BattleFxQuake2TimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicQuake2ShrinkingSquareTask);
        record->posX = task->posX + task->size;
        record->posY = 0;
        record->posZ = task->posZ + task->size - (task->size >> 2);
        break;
    case 5:
        if (task->frameCount < 26) {
            break;
        }
        (task->phase)++;
        record = (BattleFxQuake2TimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicQuake2ShrinkingSquareTask);
        record->posX = task->posX - task->size - (task->size >> 2);
        record->posY = 0;
        record->posZ = task->posZ + task->size - (task->size >> 3);
        break;
    case 6:
        if (task->frameCount < 30) {
            break;
        }
        (task->phase)++;
        record = (BattleFxQuake2TimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicQuake2ShrinkingSquareTask);
        record->posX = task->posX + (task->size >> 1);
        record->posY = 0;
        record->posZ = task->posZ - task->size - (task->size >> 3);
        break;
    case 7:
        (task->phase)++;
        record = (BattleFxQuake2TimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicQuake2ShardBurstTask);
        *(Int64Pair *)&record->posX = *(Int64Pair *)&task->posX;
        record->size = 0;
        record->sizeStep = (task->size + 896) / 24;
    default:
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCount)++;
}

#pragma pack(push, 1)

/*
 * Updates and animates exploding shard particles for Queir.
 */
// FUNCTION: FF7 0x0058608a
void __cdecl BattleFx_MagicQuake2ShardBurstTask(void)
{
    struct {
        short rotation[3];
        short pad;
        BattleFxQuake2BurstTask *slot;
        BattleFxDebrisParticle *output;
        int choice;
        int shardIndex;
        BattleFxQuake2Shard *buffer;
        int scale;
        PsxMatrix matrix;
        short pad2;
        BattleFxModelDraw *packet;
        int vector[3];
        int unused;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.slot = (BattleFxQuake2BurstTask *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.slot->frame < 32) {
        frame.rotation[2] = 0;
        frame.rotation[1] = frame.rotation[2];
        frame.rotation[0] = frame.rotation[1];
        RotMatrixXYZ((void *)frame.rotation, &frame.matrix);
        frame.matrix.t[0] = frame.slot->posX;
        frame.matrix.t[1] = frame.slot->posY;
        frame.matrix.t[2] = frame.slot->posZ;
        frame.scale = (frame.slot->size << 12) / 1900;
        frame.scale = frame.scale < 0 ? 0 : frame.scale > 0x1000 ? 0x1000
                                                                 : frame.scale;
        if (frame.slot->frame < 24 && g_BattleIsPaused == 0)
            frame.slot->size += frame.slot->sizeStep;
        frame.vector[2] = frame.scale;
        frame.vector[1] = frame.vector[2];
        frame.vector[0] = frame.vector[1];
        ScaleMatrix(&frame.matrix, (void *)frame.vector);
        MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
        SetRotMatrix(&frame.matrix);
        SetTransMatrix(&frame.matrix);
        frame.packet = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
        frame.packet->modelGroup = Gfx_ModelTableGetRenderObject(8, g_BattleMagicQuake2LoadedResTable);
        frame.packet->flags = 0;
        frame.packet->uvScroll = 0;
        frame.packet->fadeLevel = 128;
        frame.packet->stateFlags = 0;
        frame.packet->texturePageBits = 0;
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.packet, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 12, g_BattleFxActiveDrawListPtr);
    } else {
        SetRotMatrix(&g_BattleCameraTransform);
        SetTransMatrix(&g_BattleCameraTransform);
        RefreshGlobalTransformState();
        Gfx_SetPsxPrimQuadState(9, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(4, g_BattleMagicQuake2LoadedResTable)));
        Gfx_SetPsxPrimQuadState(11, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(5, g_BattleMagicQuake2LoadedResTable)));
        Gfx_SetPsxPrimQuadState(9, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(9, g_BattleMagicQuake2LoadedResTable)));
        Gfx_SetPsxPrimQuadState(11, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(10, g_BattleMagicQuake2LoadedResTable)));
        frame.buffer = (BattleFxQuake2Shard *)Gfx_GetPsxScratchpadPtr(0);
        frame.buffer->meshBase = (int)&g_BattleMagicQuake2MeshBurstPayload;
        frame.buffer->orderingTable = g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED;
        frame.buffer->drawMode = 12;
        frame.buffer->primitive = Gfx_GetPsxPacketScratchPtr();
        frame.buffer->cameraMatrix = *(PsxMatrix *)(int *)&g_BattleCameraTransform;
        *(BattleSchedulePair *)&frame.buffer->burstPos = *(BattleSchedulePair *)&frame.slot->posX;
        g_BattleFxActiveDrawListPtr = BattleFx_MagicQuake2DrawShardMesh(frame.buffer, frame.slot->frame - 28);
    }
    if (frame.slot->frame >= 74)
        frame.slot->ownerSlot = -1;
    if (g_BattleIsPaused == 0) {
        if (frame.slot->frame == 32) {
            for (frame.scale = 0; frame.scale < 10; frame.scale++)
                if (g_BattleMagicQuake2TargetMask & (1 << frame.scale))
                    Battle_PostActorHitOrHurtMessage(frame.scale);
        }
        ++frame.slot->frame;
        if (frame.slot->frame < 24) {
            frame.choice = rand() % 4;
            for (frame.shardIndex = 0; frame.shardIndex < 4; frame.shardIndex++) {
                frame.output = (BattleFxDebrisParticle *)&g_BattleTaskSlotBuffer.mainSlots[Battle_AllocateResourceSlot(BattleFx_MagicQuake2FadingQuadParticleTask)];
                frame.scale = frame.slot->size;
                frame.output->posX = frame.slot->posX + rand() % (frame.scale * 2) - frame.scale;
                frame.output->position = frame.slot->posY - rand() % (frame.slot->size >> 3);
                frame.output->posZ = frame.slot->posZ + rand() % (frame.scale * 2) - frame.scale;
                frame.output->fadeWord = ((frame.choice + frame.shardIndex) & 3) << 5;
                frame.output->heightOffset = (rand() % 4) * 1365 + 2048;
                frame.output->velocity = frame.output->fadeWord + frame.slot->frame * 2 - 384;
                frame.output->acceleration = rand() % 12 + frame.slot->frame + 12;
                frame.output->fadeWord += 0x80;
            }
        }
    }
}
#pragma pack(pop)

/*
 * Submits triangle and quad shard meshes for drawing.
 */
// FUNCTION: FF7 0x0058651f
int __cdecl BattleFx_MagicQuake2DrawShardMesh(BattleFxQuake2Shard *shard, int threshold)
{
    int vertexBase;
    int triangleList;

    vertexBase = shard->meshBase + 4;
    triangleList = vertexBase + **(int **)shard;
    shard->flags = *(int *)triangleList & 0xFFFF0000;
    shard->recordIndex = 0;
    triangleList = BattleFx_MagicQuake2DrawShardTriangles(shard, (void *)vertexBase, triangleList, threshold);
    triangleList = BattleFx_MagicQuake2DrawShardQuads(shard, (void *)vertexBase, triangleList, threshold);
    return shard->primitive;
}

/*
 * Renders animated triangle shards for Queir mesh.
 */
// FUNCTION: FF7 0x00586597
int __cdecl BattleFx_MagicQuake2DrawShardTriangles(BattleFxQuake2Shard *shard, void *vertexBase, int triList, int threshold)
{
    int colorWord;
    int bias;
    BattleFxTexturedTriRecord *recordPointer;
    BattleFxShardTriPrim *primPointer;
    int fadeOffset;
    int index;
    int counter;
    int numTris;

    numTris = *(int *)triList & 0xFFFF;
    index = shard->recordIndex;
    fadeOffset = 0;
    bias = 0xE - shard->drawMode;
    colorWord = 0x24808080;
    recordPointer = (BattleFxTexturedTriRecord *)(triList + 4);
    primPointer = (BattleFxShardTriPrim *)(shard->primitive);
    for (counter = 0; counter < numTris; counter++, index++, recordPointer++) {
        if (g_BattleMagicQuake2ShardRecords[index].fadeCounter >= 7) {
            fadeOffset = (g_BattleMagicQuake2ShardRecords[index].fadeCounter - 7) * 394758;
            if (fadeOffset > 0x808080) {
                continue;
            }
        }
        *(Int64Pair *)&shard->baseX = *(Int64Pair *)&g_BattleMagicQuake2ShardRecords[index].positionX;
        *(Int64Pair *)&shard->point0X = *(Int64Pair *)((char *)vertexBase + recordPointer->vertexOffset0);
        shard->point0X -= shard->baseX;
        shard->point0Y -= shard->baseY;
        shard->point0Z -= shard->baseZ;
        *(Int64Pair *)&shard->point1X = *(Int64Pair *)((char *)vertexBase + recordPointer->vertexOffset1);
        shard->point1X -= shard->baseX;
        shard->point1Y -= shard->baseY;
        shard->point1Z -= shard->baseZ;
        *(Int64Pair *)&shard->point2X = *(Int64Pair *)((char *)vertexBase + recordPointer->vertexOffset2);
        shard->point2X -= shard->baseX;
        shard->point2Y -= shard->baseY;
        shard->point2Z -= shard->baseZ;
        shard->posX = shard->baseX;
        shard->posZ = shard->baseZ;
        if (g_BattleMagicQuake2ShardRecords[index].positionY < threshold) {
            shard->posY = shard->baseY + g_BattleMagicQuake2ShardRecords[index].offsetY;
            if (g_BattleIsPaused == 0) {
                g_BattleMagicQuake2ShardRecords[index].rotationX += g_BattleMagicQuake2ShardRecords[index].rotationSpeedX;
                g_BattleMagicQuake2ShardRecords[index].rotationZ += g_BattleMagicQuake2ShardRecords[index].rotationSpeedZ;
                g_BattleMagicQuake2ShardRecords[index].offsetY -= g_BattleMagicQuake2ShardRecords[index].fallSpeed;
                (g_BattleMagicQuake2ShardRecords[index].fadeCounter)++;
            }
        } else {
            shard->posY = shard->baseY;
        }
        BattleFx_MagicQuake2BuildRotationMatrix((BattleFxEulerAngles16 *)&g_BattleMagicQuake2ShardRecords[index].rotationX, &shard->rotation);
        MultiplyMatrix(&shard->cameraMatrix, &shard->rotation, &shard->rotation);
        ApplyMatrixToSVectorInt32(&shard->cameraMatrix, &shard->burstPos, &shard->offsetX);
        shard->posX += shard->offsetX;
        shard->posY += shard->offsetY;
        shard->posZ += shard->offsetZ;
        SetRotMatrix(&shard->rotation);
        SetTransMatrix(&shard->rotation);
        RefreshGlobalTransformState();
        StoreDeferredDrawArgs(&shard->point0X, &shard->point1X, &shard->point2X);
        ApplyDeferredDrawArgs();
        primPointer->tag = primPointer->tag & 0xFFFFFF | 0x7000000;
        primPointer->gpuCode = 0x24;
        if (g_BattleMagicQuake2ShardRecords[index].fadeCounter < 7) {
            primPointer->color = colorWord;
        } else {
            primPointer->color = colorWord - fadeOffset | 0x26000000;
        }
        primPointer->flags = recordPointer->uv1 | shard->flags;
        primPointer->uv0Clut = recordPointer->uv0Clut;
        primPointer->uv2 = recordPointer->uv2;
        Gfx_PsxStoreFrontFacing(&shard->frontFacing);
        if (shard->frontFacing > 0) {
            Gfx_ReadTransformOrderingDepth2(&shard->orderingDepth);
            Gfx_DrawPsxPrimitive(primPointer);
        }
    }
    shard->recordIndex = index;
    shard->primitive = (int)primPointer;
    return (int)recordPointer;
}

/*
 * Builds rotation matrix from three Euler angles for Queir shards.
 */
// FUNCTION: FF7 0x00586a8b
int __cdecl BattleFx_MagicQuake2BuildRotationMatrix(BattleFxEulerAngles16 *angles, BattleRotationMatrix *matrix)
{
    int cx;
    int cy;
    int sx;
    int cz;
    int sy;
    int sz;

    cx = BattleFx_MagicQuake2LookupCosine(angles->angleX);
    cy = BattleFx_MagicQuake2LookupCosine(angles->angleY);
    cz = BattleFx_MagicQuake2LookupCosine(angles->angleZ);
    sx = BattleFx_MagicQuake2LookupSine(angles->angleX);
    sy = -BattleFx_MagicQuake2LookupSine(angles->angleY);
    sz = BattleFx_MagicQuake2LookupSine(angles->angleZ);
    matrix->m00 = cz * cy >> 12;
    matrix->m10 = (sz * cx >> 12) - ((cz * sy >> 12) * sx >> 12);
    matrix->m20 = (sz * sx >> 12) + ((cz * sy >> 12) * cx >> 12);
    matrix->m01 = -sz * cy >> 12;
    matrix->m11 = (cz * cx >> 12) + ((sz * sy >> 12) * sx >> 12);
    matrix->m21 = (cz * sx >> 12) - ((sz * sy >> 12) * cx >> 12);
    matrix->m02 = -sy;
    matrix->m12 = -cy * sx >> 12;
    matrix->m22 = cy * cx >> 12;
    return (int)matrix;
}

/*
 * Sine table lookup helper with symmetry folding for Queir.
 */
// FUNCTION: FF7 0x00586bed
int __cdecl BattleFx_MagicQuake2LookupSine(int angle)
{
    struct {
        int neg;
        int result;
        short pad;
        short value;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    angle &= 0xFFF;
    if (angle < 0x800) {
        if (angle < 0x400) {
            local.value = g_BattleMagicQueirSineTable[angle];
        } else {
            local.value = g_BattleMagicQueirSineTable[0x800 - angle];
        }
        local.result = local.value;
    } else {
        if (angle < 0xC00) {
            local.neg = -g_BattleMagicQueirSineTable[angle - 2048];
        } else {
            local.neg = -g_BattleMagicQueirSineTable[0x1000 - angle];
        }
        local.result = local.neg;
    }
    return local.result;
}

/*
 * Cosine table lookup helper with symmetry folding for Queir.
 */
// FUNCTION: FF7 0x00586c7b
int __cdecl BattleFx_MagicQuake2LookupCosine(int angle)
{
    int sample;
    int res;
    int upperValue;

    angle &= 0xFFF;
    if (angle < 0x800) {
        if (angle < 0x400) {
            sample = g_BattleMagicQueirSineTable[0x400 - angle];
        } else {
            sample = -g_BattleMagicQueirSineTable[angle - 1024];
        }
        res = sample;
    } else {
        if (angle < 0xC00) {
            upperValue = -g_BattleMagicQueirSineTable[0xC00 - angle];
        } else {
            upperValue = g_BattleMagicQueirSineTable[angle - 3072];
        }
        res = upperValue;
    }
    return res;
}

/*
 * Renders animated quad shards for Queir mesh.
 */
// FUNCTION: FF7 0x00586d06
int __cdecl BattleFx_MagicQuake2DrawShardQuads(BattleFxQuake2Shard *shard, void *vertexBase, int quadList, int threshold)
{
    int colorWord;
    int bias;
    BattleFxShardQuadSlot *recordPointer;
    BattleFxShardQuadPrim *primPointer;
    int fadeOffset;
    int index;
    int counter;
    int numQuads;

    numQuads = *(int *)quadList & 0xFFFF;
    index = shard->recordIndex;
    fadeOffset = 0;
    bias = 0xE - shard->drawMode;
    colorWord = 0x2C808080;
    recordPointer = (BattleFxShardQuadSlot *)(quadList + 4);
    primPointer = (BattleFxShardQuadPrim *)(shard->primitive);
    for (counter = 0; counter < numQuads; counter++, index++, recordPointer++) {
        if (g_BattleMagicQuake2ShardRecords[index].fadeCounter >= 7) {
            fadeOffset = (g_BattleMagicQuake2ShardRecords[index].fadeCounter - 7) * 394758;
            if (fadeOffset > 0x808080) {
                continue;
            }
        }
        *(Int64Pair *)&shard->baseX = *(Int64Pair *)&g_BattleMagicQuake2ShardRecords[index].positionX;
        *(Int64Pair *)&shard->point0X = *(Int64Pair *)((char *)vertexBase + recordPointer->vertex0Offset);
        shard->point0X -= shard->baseX;
        shard->point0Y -= shard->baseY;
        shard->point0Z -= shard->baseZ;
        *(Int64Pair *)&shard->point1X = *(Int64Pair *)((char *)vertexBase + recordPointer->vertex1Offset);
        shard->point1X -= shard->baseX;
        shard->point1Y -= shard->baseY;
        shard->point1Z -= shard->baseZ;
        *(Int64Pair *)&shard->point2X = *(Int64Pair *)((char *)vertexBase + recordPointer->vertex2Offset);
        shard->point2X -= shard->baseX;
        shard->point2Y -= shard->baseY;
        shard->point2Z -= shard->baseZ;
        *(Int64Pair *)&shard->point3X = *(Int64Pair *)((char *)vertexBase + recordPointer->vertex3Offset);
        shard->point3X -= shard->baseX;
        shard->point3Y -= shard->baseY;
        shard->point3Z -= shard->baseZ;
        shard->posX = shard->baseX;
        shard->posZ = shard->baseZ;
        if (g_BattleMagicQuake2ShardRecords[index].positionY < threshold) {
            shard->posY = shard->baseY + g_BattleMagicQuake2ShardRecords[index].offsetY;
            if (g_BattleIsPaused == 0) {
                g_BattleMagicQuake2ShardRecords[index].rotationX += g_BattleMagicQuake2ShardRecords[index].rotationSpeedX;
                g_BattleMagicQuake2ShardRecords[index].rotationZ += g_BattleMagicQuake2ShardRecords[index].rotationSpeedZ;
                g_BattleMagicQuake2ShardRecords[index].offsetY -= g_BattleMagicQuake2ShardRecords[index].fallSpeed;
                (g_BattleMagicQuake2ShardRecords[index].fadeCounter)++;
            }
        } else {
            shard->posY = shard->baseY;
        }
        BattleFx_MagicQuake2BuildRotationMatrix((BattleFxEulerAngles16 *)&g_BattleMagicQuake2ShardRecords[index].rotationX, &shard->rotation);
        MultiplyMatrix(&shard->cameraMatrix, &shard->rotation, &shard->rotation);
        ApplyMatrixToSVectorInt32(&shard->cameraMatrix, &shard->burstPos, &shard->offsetX);
        shard->posX += shard->offsetX;
        shard->posY += shard->offsetY;
        shard->posZ += shard->offsetZ;
        SetRotMatrix(&shard->rotation);
        SetTransMatrix(&shard->rotation);
        RefreshGlobalTransformState();
        StoreDeferredDrawArgs(&shard->point0X, &shard->point1X, &shard->point2X);
        ApplyDeferredDrawArgs();
        primPointer->tag = primPointer->tag & 0xFFFFFF | 0x9000000;
        primPointer->gpuCode = 0x2C;
        if (g_BattleMagicQuake2ShardRecords[index].fadeCounter < 7) {
            primPointer->color = colorWord;
        } else {
            primPointer->color = colorWord - fadeOffset | 0x2E000000;
        }
        primPointer->flags = recordPointer->flags | shard->flags;
        primPointer->uv0Clut = recordPointer->uv0Clut;
        primPointer->uv2 = recordPointer->uv2;
        primPointer->uv3 = recordPointer->uv3;
        Gfx_PsxStoreFrontFacing(&shard->frontFacing);
        if (shard->frontFacing > 0) {
            SetPendingResourceHandle((GfxPsxShortVertexSlot *)&shard->point3X);
            ApplyPendingResourceHandle();
            Gfx_ReadTransformOrderingDepth2(&shard->orderingDepth);
            Gfx_DrawPsxPrimitive(primPointer);
        }
    }
    shard->recordIndex = index;
    shard->primitive = (int)primPointer;
    return (int)recordPointer;
}

/*
 * Updates physics and renders fading quad particles for Queir.
 */
// FUNCTION: FF7 0x0058729b
void __cdecl BattleFx_MagicQuake2FadingQuadParticleTask(void)
{
    int color;
    BattleFxDebrisParticle *particle;

    particle = (BattleFxDebrisParticle *)BATTLE_CURRENT_TASK_SLOT;
    (((BattleSpriteQuadRequest *)&g_BattleMagicQuake2SpriteDrawReq)->fadeLevel) = particle->fadeLevel;
    (((BattleMagicQuake2QuadConfig *)&g_BattleMagicQuake2ModelConfig)->heightOffset) = particle->heightOffset;
    (((BattleMagicQuake2QuadConfig *)&g_BattleMagicQuake2ModelConfig)->heightOffsetB) = particle->heightOffset;
    BattleFx_MakeViewSpacePointPushed(&particle->posX, -(particle->heightOffset >> 4), (Vec3sPacked *)(((char *)&g_BattleMagicQuake2ModelConfig + 16)));
    SetRotMatrix((void *)((char *)&g_BattleMagicQuake2ModelConfig + 16));
    SetTransMatrix((char *)&g_BattleMagicQuake2ModelConfig + 16);
    if (particle->step > 3) {
        color = 0x2E808080;
        color -= (particle->step - 4) * 2105376;
        (*(int *)&g_BattleMagicQuake2ModelConfig) = color;
        (((BattleMagicQuake2QuadConfig *)&g_BattleMagicQuake2ModelConfig)->resourceEntry) = Gfx_ModelTableGetSpriteObject(3, g_BattleMagicQuake2LoadedResTable);
    } else {
        (((BattleMagicQuake2QuadConfig *)&g_BattleMagicQuake2ModelConfig)->resourceEntry) = Gfx_ModelTableGetSpriteObject(2, g_BattleMagicQuake2LoadedResTable);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)&g_BattleMagicQuake2SpriteDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (g_BattleIsPaused == 0) {
        particle->position += particle->velocity;
        particle->velocity += particle->acceleration;
        (particle->step)++;
        if (particle->step > 7) {
            particle->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Updates and renders shrinking square model for Queir.
 */
// FUNCTION: FF7 0x005873fa
void __cdecl BattleFx_MagicQuake2ShrinkingSquareTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[116];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; int depthOutB; };
        struct { unsigned char pad_to_C[0xC]; BattleFxCosmoMemoryTriSpin *squareTask; };
        struct { unsigned char pad_to_10[0x10]; SVECTOR stripDx; };
        struct { unsigned char pad_to_18[0x18]; int shrinkScale; };
        struct { unsigned char pad_to_1C[0x1C]; SVECTOR shardCx; };
        struct { unsigned char pad_to_24[0x24]; SVECTOR stripBx; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxGroundTilePacket *shrinkPacket; };
        struct { unsigned char pad_to_30[0x30]; SVECTOR shardAx; };
        struct { unsigned char pad_to_38[0x38]; PsxMatrix modelMatrix; };
        struct { unsigned char pad_to_58[0x58]; int depthResult; };
        struct { unsigned char pad_to_5C[0x5C]; int depthOutA; };
        struct { unsigned char pad_to_64[0x64]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.squareTask = (BattleFxCosmoMemoryTriSpin *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.modelMatrix);
    frame.modelMatrix.t[0] = frame.squareTask->posX;
    frame.modelMatrix.t[1] = frame.squareTask->posY;
    frame.modelMatrix.t[2] = frame.squareTask->posZ;
    frame.shrinkScale = 0x1000 /* 1.0 */;
    if (frame.squareTask->step >= 1) {
        frame.shrinkScale -= frame.squareTask->step - 1 << 11;
    }
    frame.scaleVec.vx = frame.shrinkScale;
    frame.scaleVec.vy = frame.shrinkScale;
    frame.scaleVec.vz = frame.shrinkScale;
    ScaleMatrix(&frame.modelMatrix, &frame.scaleVec);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    RefreshGlobalTransformState();
    frame.shrinkPacket = (BattleFxGroundTilePacket *)Gfx_GetPsxPacketScratchPtr();
    frame.shardAx.vx = 0xF600;
    frame.shardAx.vy = 0;
    frame.shardAx.vz = 2560;
    frame.stripBx.vx = 2560;
    frame.stripBx.vy = 0;
    frame.stripBx.vz = 2560;
    frame.shardCx.vx = 0xF600;
    frame.shardCx.vy = 0;
    frame.shardCx.vz = 0xF600;
    frame.stripDx.vx = 2560;
    frame.stripDx.vy = 0;
    frame.stripDx.vz = 0xF600;
    Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(1, g_BattleMagicQuake2LoadedResTable)));
    frame.depthResult = Gfx_TransformQuadReturnDepth(&frame.shardAx, &frame.stripBx, &frame.shardCx, &frame.stripDx, &frame.depthOutA, &frame.depthOutB);
    frame.shrinkPacket->tag = frame.shrinkPacket->tag & 0xFFFFFF | 0x9000000;
    frame.shrinkPacket->gpuCode = 0x2C;
    frame.shrinkPacket->command = 0x2E808080;
    frame.shrinkPacket->tpage = 0x2D;
    frame.shrinkPacket->clut = 0x7888;
    frame.shrinkPacket->uv0 = 0x8000;
    frame.shrinkPacket->uv1 = 0x8080;
    frame.shrinkPacket->uv2 = 0xFF00;
    frame.shrinkPacket->uv3 = 0xFF80;
    Gfx_DrawPsxPrimitive(frame.shrinkPacket);
    if (frame.squareTask->step >= 3) {
        frame.squareTask->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused == 0) {
        (frame.squareTask->step)++;
    }
}

/*
 * Updates and renders fading model for Queir.
 */
// FUNCTION: FF7 0x005875fd
void __cdecl BattleFx_MagicQuake2FadingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        int modelGroup;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; BattleFxQuake2FadeSlot *fadeSlot; };
        struct { unsigned char pad_to_10[0x10]; int fadeScale; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix modelMatrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_38[0x38]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.fadeSlot = (BattleFxQuake2FadeSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.fadeSlot->spinAngleY + frame.fadeSlot->baseAngleY;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.modelMatrix);
    frame.modelMatrix.t[0] = frame.fadeSlot->posX;
    frame.modelMatrix.t[1] = frame.fadeSlot->posY;
    frame.modelMatrix.t[2] = frame.fadeSlot->posZ;
    frame.fadeScale = (frame.fadeSlot->modelSize << 12) / 2500;
    frame.scaleVec.vx = frame.fadeScale;
    frame.scaleVec.vy = frame.fadeSlot->fadeLevel;
    frame.scaleVec.vz = frame.fadeScale;
    ScaleMatrix(&frame.modelMatrix, &frame.scaleVec);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->flags = 8;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 128;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    if (frame.fadeSlot->altModelEnabled != 0) {
        frame.modelGroup = Gfx_ModelTableGetRenderObject(7, g_BattleMagicQuake2LoadedResTable);
    } else {
        frame.modelGroup = Gfx_ModelTableGetRenderObject(6, g_BattleMagicQuake2LoadedResTable);
    }
    frame.modelDraw->modelGroup = frame.modelGroup;
    if (frame.fadeSlot->timer > frame.fadeSlot->fadeStart) {
        frame.fadeScale = frame.fadeSlot->timer - frame.fadeSlot->fadeStart << 4;
        if (frame.fadeScale > 0x80) {
            frame.fadeScale = 0x80;
        }
        frame.modelDraw->fadeLevel -= *(short *)&frame.fadeScale;
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.fadeSlot->timer >= frame.fadeSlot->fadeStart + 8) {
        frame.fadeSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.fadeSlot->spinAngleY -= 0x20;
    frame.fadeSlot->fadeLevel += frame.fadeSlot->fadeStep;
    if (frame.fadeSlot->fadeLevel > 4096) {
        frame.fadeSlot->fadeLevel = 4096;
    }
    (frame.fadeSlot->timer)++;
}

/*
 * Timeline task driving single-target Queir effect.
 */
// FUNCTION: FF7 0x0058782f
void __cdecl BattleFx_MagicQuake2SingleTargetTimelineTask(void)
{
    BattleFxMagicQuake2Timeline *pChild;
    BattleFxQuake2SingleTarget *task;

    if (g_BattleIsPaused != 0) {
        return;
    }
    task = (BattleFxQuake2SingleTarget *)BATTLE_CURRENT_TASK_SLOT;
    /* Numeric timeline step index. */
    switch (task->phase) {
    case 0:
        if (task->stage < 1) {
            break;
        }
        (task->phase)++;
        pChild = (BattleFxMagicQuake2Timeline *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicQuake2FadingModelTask);
        pChild->frameCount = 23;
        *(Int64Pair *)&pChild->posX = *(Int64Pair *)&task->posX;
        pChild->spinAngle = 0;
        pChild->scale = 0x100 /* 0.0625 */;
        pChild->progress = task->progress;
        pChild->stepSize = 0;
        pChild->fadeLevel = 0;
        pChild->shade = 0xBA;
        pChild->clearedWord = 0;
        break;
    case 1:
        if (task->stage < 5) {
            break;
        }
        (task->phase)++;
        pChild = (BattleFxMagicQuake2Timeline *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicQuake2FadingModelTask);
        pChild->frameCount = 26;
        *(Int64Pair *)&pChild->posX = *(Int64Pair *)&task->posX;
        pChild->spinAngle = 0;
        pChild->scale = 0x900 /* 0.5625 */;
        pChild->progress = task->progress;
        pChild->stepSize = 0;
        pChild->fadeLevel = 0;
        pChild->shade = 0xC3;
        pChild->clearedWord = 0;
        break;
    case 2:
        if (task->stage < 18) {
            break;
        }
        (task->phase)++;
        pChild = (BattleFxMagicQuake2Timeline *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicQuake2GrowShrinkModelTask);
        *(Int64Pair *)&pChild->posX = *(Int64Pair *)&task->posX;
        pChild->progress = 0;
        pChild->stepSize = task->progress / 6;
        break;
    default:
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->stage)++;
}

/*
 * Updates and renders expanding and shrinking model for Queir.
 */
// FUNCTION: FF7 0x00587a50
void __cdecl BattleFx_MagicQuake2GrowShrinkModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        int tmpClamp;
        struct { unsigned char pad_to_4[0x4]; int clampedScale; };
        struct { unsigned char pad_to_8[0x8]; SVECTOR rotX; };
        struct { unsigned char pad_to_10[0x10]; BattleFxQuake2GrowShrinkModel *growModel; };
        struct { unsigned char pad_to_14[0x14]; int growScale; };
        struct { unsigned char pad_to_18[0x18]; PsxMatrix modelMatrix; };
        struct { unsigned char pad_to_38[0x38]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_3C[0x3C]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.growModel = (BattleFxQuake2GrowShrinkModel *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vz = 0;
    frame.rotX.vy = frame.rotX.vz;
    frame.rotX.vx = frame.rotX.vy;
    RotMatrixXYZ(&frame.rotX, &frame.modelMatrix);
    frame.modelMatrix.t[0] = frame.growModel->posX;
    frame.modelMatrix.t[1] = frame.growModel->posY;
    frame.modelMatrix.t[2] = frame.growModel->posZ;
    frame.growScale = (frame.growModel->scale << 12) / 1900;
    if (frame.growScale < 0) {
        frame.clampedScale = 0;
    } else {
        if (frame.growScale > 0x1000) {
            frame.tmpClamp = 0x1000;
        } else {
            frame.tmpClamp = frame.growScale;
        }
        frame.clampedScale = frame.tmpClamp;
    }
    frame.growScale = frame.clampedScale;
    frame.scaleVec.vz = frame.growScale;
    frame.scaleVec.vy = frame.scaleVec.vz;
    frame.scaleVec.vx = frame.scaleVec.vy;
    ScaleMatrix(&frame.modelMatrix, &frame.scaleVec);
    if (g_BattleIsPaused == 0) {
        if (frame.growModel->phase < 6) {
            frame.growModel->scale += frame.growModel->scaleStep;
        } else if (frame.growModel->phase >= 18) {
            frame.growModel->scale -= frame.growModel->scaleStep;
        }
    }
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->flags = 0;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 128;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(8, g_BattleMagicQuake2LoadedResTable);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.growModel->phase >= 24) {
        frame.growModel->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused == 0) {
        if (frame.growModel->phase == 0x12) {
            for (frame.growScale = 0; frame.growScale < 0xA; frame.growScale++) {
                if ((g_BattleMagicQuake2TargetMask & 1 << frame.growScale) != 0) {
                    Battle_PostActorHitOrHurtMessage(frame.growScale);
                }
            }
        }
        (frame.growModel->phase)++;
    }
}
} /* extern "C" */
