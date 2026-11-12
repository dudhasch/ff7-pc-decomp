// src/ff7/src/battle/battle_fx_magic_quake3.cpp
/* Magic Effect [38]: Quake3 (Japanese: クエイガ Quakega, dev tag: "MAGIC/QUEIG" / Queig) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle visual effect for the Queig magic: it loads resources, builds triangle and quad
//   shard particles from a mesh, and runs single-target or multi-target timeline tasks that burst,
//   flatten, pulse and draw the shards with sine and cosine rotation helpers.

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
#include "lib/api/sound.h"
extern "C" {

struct BattleFxFixedScaleVec;
struct GfxPsxShortVertexSlot;
struct BattleRotationMatrix;
struct BattleFxEulerAngles16;
struct GfxPsxSpriteQuadState;
struct BattleFxQuake3MeshDraw;
struct BattleFxModelDraw;

#pragma intrinsic(abs)

// clang-format off
unsigned char g_rawData_008b0370[16] = {0x58, 0x00, 0x00, 0x00, 0x59, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008b0380
int g_BattleMagicQuake3SoundGroups = 0x008B0370;
// GLOBAL: FF7 0x008b0388
struct BattleMagicQuake3MeshSource g_BattleMagicQuake3MeshPayload = {0x308, {{0, -661, -645, {0}}, {0, -389, -1042, {0}}, {0, 0, -1283, {0}}, {456, -661, -456, {0}}, {736, -389, -736, {0}}, {907, 0, -907, {0}}, {645, -661, 0, {0}}, {1042, -389, 0, {0}}, {1283, 0, 0, {0}}, {456, -661, 456, {0}}, {736, -389, 736, {0}}, {907, 0, 907, {0}}, {0, -661, 645, {0}}, {0, -389, 1042, {0}}, {0, 0, 1283, {0}}, {-456, -661, 456, {0}}, {-736, -389, 736, {0}}, {-907, 0, 907, {0}}, {-645, -661, 0, {0}}, {-1042, -389, 0, {0}}, {-1283, 0, 0, {0}}, {-456, -661, -456, {0}}, {-736, -389, -736, {0}}, {-907, 0, -907, {0}}, {-22, -424, -1097, {0}}, {-22, -34, -1338, {0}}, {-930, -34, -962, {0}}, {-759, -424, -792, {0}}, {759, -424, -792, {0}}, {930, -34, -962, {0}}, {22, -34, -1338, {0}}, {22, -424, -1097, {0}}, {-792, -424, -759, {0}}, {-962, -34, -930, {0}}, {-1338, -34, -22, {0}}, {-1097, -424, -22, {0}}, {1097, -424, -22, {0}}, {1338, -34, -22, {0}}, {962, -34, -930, {0}}, {792, -424, -759, {0}}, {792, -424, 759, {0}}, {962, -34, 930, {0}}, {1338, -34, 22, {0}}, {1097, -424, 22, {0}}, {-1097, -424, 22, {0}}, {-1338, -34, 22, {0}}, {-962, -34, 930, {0}}, {-792, -424, 759, {0}}, {-759, -424, 792, {0}}, {-930, -34, 962, {0}}, {-22, -34, 1338, {0}}, {-22, -424, 1097, {0}}, {22, -424, 1097, {0}}, {22, -34, 1338, {0}}, {930, -34, 962, {0}}, {759, -424, 792, {0}}, {-752, -443, 773, {0}}, {-15, -443, 1078, {0}}, {-15, -714, 682, {0}}, {-471, -714, 493, {0}}, {15, -443, 1078, {0}}, {752, -443, 773, {0}}, {471, -714, 493, {0}}, {15, -714, 682, {0}}, {773, -443, 752, {0}}, {1078, -443, 15, {0}}, {682, -714, 15, {0}}, {493, -714, 471, {0}}, {1078, -443, -15, {0}}, {773, -443, -752, {0}}, {493, -714, -471, {0}}, {682, -714, -15, {0}}, {752, -443, -773, {0}}, {15, -443, -1078, {0}}, {15, -714, -682, {0}}, {471, -714, -493, {0}}, {-15, -443, -1078, {0}}, {-752, -443, -773, {0}}, {-471, -714, -493, {0}}, {-15, -714, -682, {0}}, {-773, -443, -752, {0}}, {-1078, -443, -15, {0}}, {-682, -714, -15, {0}}, {-493, -714, -471, {0}}, {-1078, -443, 15, {0}}, {-773, -443, 752, {0}}, {-493, -714, 471, {0}}, {-682, -714, 15, {0}}, {645, -732, 0, {0}}, {456, -732, -456, {0}}, {0, -732, -645, {0}}, {-456, -732, -456, {0}}, {-645, -732, 0, {0}}, {-456, -732, 456, {0}}, {0, -732, 645, {0}}, {456, -732, 456, {0}}, {0, -732, 0, {0}}}, {8, 0, 0x2c, 0, 0xf8, 2, 0xc0, 2, 0, 3, 0x80, 0, 0x6a, 0xea, 8, 0x79, 0x3d, 0xfd, 0x40, 0xc0, 0xf0, 2, 0xf8, 2, 0, 3, 0x80, 0, 0x7d, 0xc0, 8, 0x79, 0x6a, 0xea, 0x40, 0xc0, 0xe8, 2, 0xf0, 2, 0, 3, 0x80, 0, 0x6a, 0x92, 8, 0x79, 0x7d, 0xc0, 0x40, 0xc0, 0xe0, 2, 0xe8, 2, 0, 3, 0x80, 0, 0x3d, 0x80, 8, 0x79, 0x6a, 0x92, 0x40, 0xc0, 0xd8, 2, 0xe0, 2, 0, 3, 0x80, 0, 0x12, 0x92, 8, 0x79, 0x3d, 0x80, 0x40, 0xc0, 0xd0, 2, 0xd8, 2, 0, 3, 0x80, 0, 0, 0xc0, 8, 0x79, 0x12, 0x92, 0x40, 0xc0, 0xc8, 2, 0xd0, 2, 0, 3, 0x80, 0, 0x12, 0xea, 8, 0x79, 0, 0xc0, 0x40, 0xc0, 0xc0, 2, 0xc8, 2, 0, 3, 0x80, 0, 0x3d, 0xfd, 8, 0x79, 0x12, 0xea, 0x40, 0xc0, 0x58, 0, 0, 0, 0x48, 0, 0x30, 0, 0xf8, 2, 0xc0, 2, 0x3d, 0x7c, 8, 0x78, 0x1e, 0x7e, 0x3d, 1, 0x1e, 0, 0x80, 0, 0x60, 0, 0x48, 0, 0xf0, 2, 0xf8, 2, 0x5e, 0x7e, 0x48, 0x78, 0x40, 0x7c, 0x5e, 0, 0x40, 1, 0x80, 0, 0x78, 0, 0x60, 0, 0xe8, 2, 0xf0, 2, 0x7d, 0x7c, 0x48, 0x78, 0x5e, 0x7e, 0x7d, 1, 0x5e, 0, 0x80, 0, 0x90, 0, 0x78, 0, 0xe0, 2, 0xe8, 2, 0x9e, 0x7e, 0x88, 0x78, 0x80, 0x7c, 0x9e, 0, 0x80, 1, 0x80, 0, 0xa8, 0, 0x90, 0, 0xd8, 2, 0xe0, 2, 0xbd, 0x7c, 0x88, 0x78, 0x9e, 0x7e, 0xbd, 1, 0x9e, 0, 0x80, 0, 0, 0, 0xa8, 0, 0xd0, 2, 0xd8, 2, 0xde, 0x7e, 0xc8, 0x78, 0xc0, 0x7c, 0xde, 0, 0xc0, 1, 0x80, 0, 0x18, 0, 0, 0, 0xc8, 2, 0xd0, 2, 0xfd, 0x7c, 0xc8, 0x78, 0xde, 0x7e, 0xfd, 1, 0xde, 0, 0x80, 0, 0x30, 0, 0x18, 0, 0xc0, 2, 0xc8, 2, 0x1e, 0x7e, 8, 0x78, 0, 0x7c, 0x1e, 0, 0, 1, 0x80, 0, 0x90, 0, 0x98, 0, 0xb8, 2, 0xa0, 2, 0xbb, 0x7e, 0x88, 0x78, 0x80, 0x7e, 0xbd, 0, 0x84, 0, 0x80, 0, 0x78, 0, 0x90, 0, 0xb0, 2, 0xb8, 2, 0xfb, 0x7d, 0xc8, 0x78, 0xc0, 0x7d, 0xfb, 0, 0xc0, 0, 0x80, 0, 0x80, 0, 0x78, 0, 0xa8, 2, 0xb0, 2, 0x3d, 0x7e, 8, 0x78, 2, 0x7e, 0x3a, 0, 0, 0, 0x80, 0, 0x98, 0, 0x80, 0, 0xa0, 2, 0xa8, 2, 0x7d, 0x7d, 0x48, 0x78, 0x40, 0x7d, 0x7d, 0, 0x40, 0, 0x80, 0, 0xa8, 0, 0xb0, 0, 0x98, 2, 0x80, 2, 0xbd, 0x7d, 0x88, 0x78, 0x82, 0x7d, 0xbb, 0, 0x80, 2, 0x80, 0, 0x90, 0, 0xa8, 0, 0x90, 2, 0x98, 2, 0xfb, 0x7c, 0xc8, 0x78, 0xc0, 0x7c, 0xfb, 0, 0xc0, 0, 0x80, 0, 0x98, 0, 0x90, 0, 0x88, 2, 0x90, 2, 0x3d, 0x7d, 8, 0x78, 2, 0x7d, 0x3a, 0, 0, 0, 0x80, 0, 0xb0, 0, 0x98, 0, 0x80, 2, 0x88, 2, 0x7d, 0x7c, 0x48, 0x78, 0x40, 0x7c, 0x7d, 0, 0x40, 0, 0x80, 0, 0, 0, 8, 0, 0x78, 2, 0x60, 2, 0xbb, 0x7e, 0x88, 0x78, 0x80, 0x7c, 0xbd, 0, 0x82, 0, 0x80, 0, 0xa8, 0, 0, 0, 0x70, 2, 0x78, 2, 0xfb, 0x7c, 0xc8, 0x78, 0xc0, 0x7c, 0xfb, 0, 0xc0, 0, 0x80, 0, 0xb0, 0, 0xa8, 0, 0x68, 2, 0x70, 2, 0x3c, 0x7b, 8, 0x78, 3, 0x7d, 0x39, 0, 0, 0, 0x80, 0, 8, 0, 0xb0, 0, 0x60, 2, 0x68, 2, 0x7d, 0x7c, 0x48, 0x78, 0x40, 0x7c, 0x7d, 0, 0x40, 0, 0x80, 0, 0x18, 0, 0x20, 0, 0x58, 2, 0x40, 2, 0xbb, 0x7e, 0x88, 0x78, 0x80, 0x7d, 0xbd, 0, 0x84, 0, 0x80, 0, 0, 0, 0x18, 0, 0x50, 2, 0x58, 2, 0xfb, 0x7d, 0xc8, 0x78, 0xc0, 0x7d, 0xfb, 0, 0xc0, 0, 0x80, 0, 8, 0, 0, 0, 0x48, 2, 0x50, 2, 0x36, 0x7e, 8, 0x78, 0, 0x7e, 0x3c, 0, 3, 0, 0x80, 0, 0x20, 0, 8, 0, 0x40, 2, 0x48, 2, 0x7d, 0x7d, 0x48, 0x78, 0x40, 0x7d, 0x7d, 0, 0x40, 0, 0x80, 0, 0x30, 0, 0x38, 0, 0x38, 2, 0x20, 2, 0xbb, 0x7e, 0x88, 0x78, 0x80, 0x7c, 0xbd, 1, 0x82, 0, 0x80, 0, 0x18, 0, 0x30, 0, 0x30, 2, 0x38, 2, 0xfb, 0x7d, 0xc8, 0x78, 0xc0, 0x7d, 0xfb, 0, 0xc0, 0, 0x80, 0, 0x20, 0, 0x18, 0, 0x28, 2, 0x30, 2, 0x3d, 0x7e, 8, 0x78, 5, 0x7e, 0x3a, 0, 0, 0, 0x80, 0, 0x38, 0, 0x20, 0, 0x20, 2, 0x28, 2, 0x7d, 0x7d, 0x48, 0x78, 0x40, 0x7d, 0x7d, 0, 0x40, 0, 0x80, 0, 0x48, 0, 0x50, 0, 0x18, 2, 0, 2, 0xbd, 0x7d, 0x88, 0x78, 0x80, 0x7d, 0xbd, 0, 0x80, 0, 0x80, 0, 0x30, 0, 0x48, 0, 0x10, 2, 0x18, 2, 0xfb, 0x7b, 0xc8, 0x78, 0xc0, 0x7b, 0xfb, 0, 0xc0, 0, 0x80, 0, 0x38, 0, 0x30, 0, 8, 2, 0x10, 2, 0x3d, 0x7d, 8, 0x78, 2, 0x7d, 0x3b, 0, 0, 2, 0x80, 0, 0x50, 0, 0x38, 0, 0, 2, 8, 2, 0x7d, 0x7b, 0x48, 0x78, 0x40, 0x7b, 0x7d, 0, 0x40, 0, 0x80, 0, 0x60, 0, 0x68, 0, 0xf8, 1, 0xe0, 1, 0xbb, 0x7e, 0x88, 0x78, 0x80, 0x7c, 0xbd, 0, 0x82, 0, 0x80, 0, 0x48, 0, 0x60, 0, 0xf0, 1, 0xf8, 1, 0xfb, 0x7c, 0xc8, 0x78, 0xc0, 0x7c, 0xfb, 0, 0xc0, 0, 0x80, 0, 0x50, 0, 0x48, 0, 0xe8, 1, 0xf0, 1, 0x3d, 0x7c, 8, 0x78, 2, 0x7e, 0x39, 0, 0, 0, 0x80, 0, 0x68, 0, 0x50, 0, 0xe0, 1, 0xe8, 1, 0x7d, 0x7c, 0x48, 0x78, 0x40, 0x7c, 0x7d, 0, 0x40, 0, 0x80, 0, 0x78, 0, 0x80, 0, 0xd8, 1, 0xc0, 1, 0xbb, 0x7e, 0x88, 0x78, 0x80, 0x7e, 0xbd, 0, 0x82, 0, 0x80, 0, 0x60, 0, 0x78, 0, 0xd0, 1, 0xd8, 1, 0xfb, 0x7c, 0xc8, 0x78, 0xc0, 0x7c, 0xfb, 0, 0xc0, 0, 0x80, 0, 0x68, 0, 0x60, 0, 0xc8, 1, 0xd0, 1, 0x3d, 0x7e, 8, 0x78, 2, 0x7e, 0x3b, 0, 0, 0, 0x80, 0, 0x80, 0, 0x68, 0, 0xc0, 1, 0xc8, 1, 0x7d, 0x7c, 0x48, 0x78, 0x40, 0x7c, 0x7d, 0, 0x40, 0, 0x80, 0, 0x50, 0, 0x68, 0, 0xb8, 1, 0xa0, 1, 0xfd, 0x78, 0xc8, 0x78, 0xc0, 0x78, 0xfd, 0, 0xc0, 0, 0x80, 0, 0x58, 0, 0x50, 0, 0xb0, 1, 0xb8, 1, 0x3d, 0x7b, 8, 0x78, 0, 0x7b, 0x3d, 0, 0, 0, 0x80, 0, 0x70, 0, 0x58, 0, 0xa8, 1, 0xb0, 1, 0x7d, 0x78, 0x48, 0x78, 0x40, 0x78, 0x7d, 0, 0x40, 0, 0x80, 0, 0x68, 0, 0x70, 0, 0xa0, 1, 0xa8, 1, 0xbd, 0x7b, 0x88, 0x78, 0x80, 0x7b, 0xbd, 0, 0x80, 0, 0x80, 0, 0x68, 0, 0x80, 0, 0x98, 1, 0x80, 1, 0xfd, 0x7b, 0xc8, 0x78, 0xc0, 0x7b, 0xfd, 0, 0xc0, 0, 0x80, 0, 0x70, 0, 0x68, 0, 0x90, 1, 0x98, 1, 0x3d, 0x7d, 8, 0x78, 0, 0x7d, 0x3d, 0, 0, 0, 0x80, 0, 0x88, 0, 0x70, 0, 0x88, 1, 0x90, 1, 0x7d, 0x7b, 0x48, 0x78, 0x40, 0x7b, 0x7d, 0, 0x40, 0, 0x80, 0, 0x80, 0, 0x88, 0, 0x80, 1, 0x88, 1, 0xbd, 0x7d, 0x88, 0x78, 0x80, 0x7a, 0xbd, 0, 0x82, 0, 0x80, 0, 0x80, 0, 0x98, 0, 0x78, 1, 0x60, 1, 0xfd, 0x7c, 0xc8, 0x78, 0xc0, 0x7c, 0xfd, 0, 0xc0, 0, 0x80, 0, 0x88, 0, 0x80, 0, 0x70, 1, 0x78, 1, 0x3d, 0x7e, 8, 0x78, 0, 0x7e, 0x3d, 0, 0, 0, 0x80, 0, 0xa0, 0, 0x88, 0, 0x68, 1, 0x70, 1, 0x7d, 0x7c, 0x48, 0x78, 0x40, 0x7c, 0x7d, 0, 0x40, 0, 0x80, 0, 0x98, 0, 0xa0, 0, 0x60, 1, 0x68, 1, 0xbd, 0x7e, 0x88, 0x78, 0x80, 0x7e, 0xbd, 0, 0x82, 0, 0x80, 0, 0x38, 0, 0x50, 0, 0x58, 1, 0x40, 1, 0xfd, 0x7d, 0xc8, 0x78, 0xc0, 0x7d, 0xfd, 0, 0xc0, 0, 0x80, 0, 0x40, 0, 0x38, 0, 0x50, 1, 0x58, 1, 0x3d, 0x7e, 8, 0x78, 0, 0x7e, 0x3d, 0, 0, 0, 0x80, 0, 0x58, 0, 0x40, 0, 0x48, 1, 0x50, 1, 0x7d, 0x7d, 0x48, 0x78, 0x40, 0x7d, 0x7d, 0, 0x40, 0, 0x80, 0, 0x50, 0, 0x58, 0, 0x40, 1, 0x48, 1, 0xbd, 0x7e, 0x88, 0x78, 0x80, 0x7e, 0xbd, 0, 0x82, 0, 0x80, 0, 0x20, 0, 0x38, 0, 0x38, 1, 0x20, 1, 0xfd, 0x7b, 0xc8, 0x78, 0xc0, 0x7b, 0xfd, 0, 0xc0, 0, 0x80, 0, 0x28, 0, 0x20, 0, 0x30, 1, 0x38, 1, 0x3d, 0x7d, 8, 0x78, 0, 0x7d, 0x3d, 0, 0, 0, 0x80, 0, 0x40, 0, 0x28, 0, 0x28, 1, 0x30, 1, 0x7d, 0x7b, 0x48, 0x78, 0x40, 0x7b, 0x7d, 0, 0x40, 0, 0x80, 0, 0x38, 0, 0x40, 0, 0x20, 1, 0x28, 1, 0xbd, 0x7d, 0x88, 0x78, 0x80, 0x7a, 0xbd, 0, 0x82, 0, 0x80, 0, 0x98, 0, 0xb0, 0, 0x18, 1, 0, 1, 0xfd, 0x7d, 0xc8, 0x78, 0xc0, 0x7d, 0xfd, 0, 0xc0, 0, 0x80, 0, 0xa0, 0, 0x98, 0, 0x10, 1, 0x18, 1, 0x3d, 0x7e, 8, 0x78, 0, 0x7e, 0x3d, 0, 0, 0, 0x80, 0, 0xb8, 0, 0xa0, 0, 8, 1, 0x10, 1, 0x7d, 0x7d, 0x48, 0x78, 0x40, 0x7d, 0x7d, 0, 0x40, 0, 0x80, 0, 0xb0, 0, 0xb8, 0, 0, 1, 8, 1, 0xbd, 0x7e, 0x88, 0x78, 0x80, 0x7e, 0xbd, 0, 0x82, 0, 0x80, 0, 8, 0, 0x20, 0, 0xf8, 0, 0xe0, 0, 0xfd, 0x78, 0xc8, 0x78, 0xc0, 0x78, 0xfd, 0, 0xc0, 0, 0x80, 0, 0x10, 0, 8, 0, 0xf0, 0, 0xf8, 0, 0x3d, 0x7b, 8, 0x78, 0, 0x7b, 0x3d, 0, 0, 0, 0x80, 0, 0x28, 0, 0x10, 0, 0xe8, 0, 0xf0, 0, 0x7d, 0x78, 0x48, 0x78, 0x40, 0x78, 0x7d, 0, 0x40, 0, 0x80, 0, 0x20, 0, 0x28, 0, 0xe0, 0, 0xe8, 0, 0xbd, 0x7b, 0x88, 0x78, 0x80, 0x7b, 0xbd, 0, 0x80, 0, 0x80, 0, 0xb0, 0, 8, 0, 0xd8, 0, 0xc0, 0, 0xfd, 0x7c, 0xc8, 0x78, 0xc0, 0x7c, 0xfd, 0, 0xc0, 0, 0x80, 0, 0xb8, 0, 0xb0, 0, 0xd0, 0, 0xd8, 0, 0x3d, 0x7d, 8, 0x78, 0, 0x7d, 0x3d, 0, 0, 0, 0x80, 0, 0x10, 0, 0xb8, 0, 0xc8, 0, 0xd0, 0, 0x7d, 0x7c, 0x48, 0x78, 0x40, 0x7c, 0x7d, 0, 0x40, 0, 0x80, 0, 8, 0, 0x10, 0, 0xc0, 0, 0xc8, 0, 0xbd, 0x7d, 0x88, 0x78, 0x80, 0x7d, 0xbd, 0, 0x82, 0, 0x80, 0, 0xc8, 0, 0xd0, 0, 0xc0, 0, 0xd8, 0, 0xfd, 0xfb, 8, 0x7a, 0xc0, 0xfb, 0xf7, 0xc0, 0xc4, 0xc0, 0x80, 0, 0x68, 2, 0x70, 2, 0x60, 2, 0x78, 2, 0x80, 0xfb, 0xc8, 0x79, 0x8d, 0xc0, 0xbd, 0xfb, 0xb2, 0xc0, 0x80, 0, 8, 1, 0x10, 1, 0, 1, 0x18, 1, 0xfe, 0xfc, 8, 0x7a, 0xc0, 0xfc, 0xf8, 0xc0, 0xc5, 0xc0, 0x80, 0, 0x88, 2, 0x90, 2, 0x80, 2, 0x98, 2, 0, 0xf6, 8, 0x79, 0x1a, 0x80, 0x7a, 0xf6, 0x65, 0x80, 0x80, 0, 0x68, 1, 0x70, 1, 0x60, 1, 0x78, 1, 0x7b, 0xf6, 8, 0x79, 0, 0xf6, 0x6e, 0x80, 8, 0x80, 0x80, 0, 0xa8, 2, 0xb0, 2, 0xa0, 2, 0xb8, 2, 0x80, 0xfc, 0xc8, 0x79, 0x8c, 0xc0, 0xbd, 0xfc, 0xb1, 0xc0, 0x80, 0, 0x88, 1, 0x90, 1, 0x80, 1, 0x98, 1, 0xfe, 0xbc, 0x88, 0x79, 0xc0, 0xbc, 0xf8, 0x80, 0xc5, 0x80, 0x80, 0, 0xc8, 1, 0xd0, 1, 0xc0, 1, 0xd8, 1, 0, 0xf6, 8, 0x79, 0x1a, 0x80, 0x7a, 0xf6, 0x65, 0x80, 0x80, 0, 0xa8, 1, 0xb0, 1, 0xa0, 1, 0xb8, 1, 0xbe, 0xbc, 0x48, 0x79, 0x80, 0xbc, 0xb8, 0x80, 0x85, 0x80, 0x80, 0, 0xe8, 1, 0xf0, 1, 0xe0, 1, 0xf8, 1, 0xc0, 0xfc, 8, 0x7a, 0xcc, 0xc0, 0xfd, 0xfc, 0xf1, 0xc0, 0x80, 0, 0x48, 1, 0x50, 1, 0x40, 1, 0x58, 1, 0xfd, 0xbb, 0x88, 0x79, 0xc0, 0xbb, 0xf7, 0x80, 0xc4, 0x80, 0x80, 0, 8, 2, 0x10, 2, 0, 2, 0x18, 2, 0, 0xf6, 8, 0x79, 0x1a, 0x80, 0x7a, 0xf6, 0x65, 0x80, 0x80, 0, 0x28, 1, 0x30, 1, 0x20, 1, 0x38, 1, 0x7b, 0xf6, 8, 0x79, 0, 0xf6, 0x6e, 0x80, 8, 0x80, 0x80, 0, 0x28, 2, 0x30, 2, 0x20, 2, 0x38, 2, 0xc0, 0xfb, 8, 0x7a, 0xcd, 0xc0, 0xfd, 0xfb, 0xf2, 0xc0, 0x80, 0, 0xe8, 0, 0xf0, 0, 0xe0, 0, 0xf8, 0, 0xbd, 0xbb, 0x48, 0x79, 0x80, 0xbb, 0xb7, 0x80, 0x84, 0x80, 0x80, 0, 0x48, 2, 0x50, 2, 0x40, 2, 0x58, 2, 0xc0, 0xfc, 8, 0x7a, 0xcc, 0xc0, 0xfd, 0xfc, 0xf1, 0xc0, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {{0, -661, -645, {0}}, {0, -389, -1042, {0}}, {0, 0, -1283, {0}}, {456, -661, -456, {0}}, {736, -389, -736, {0}}, {907, 0, -907, {0}}, {645, -661, 0, {0}}, {1042, -389, 0, {0}}, {1283, 0, 0, {0}}, {456, -661, 456, {0}}, {736, -389, 736, {0}}, {907, 0, 907, {0}}, {0, -661, 645, {0}}, {0, -389, 1042, {0}}, {0, 0, 1283, {0}}, {-456, -661, 456, {0}}, {-736, -389, 736, {0}}, {-907, 0, 907, {0}}, {-645, -661, 0, {0}}, {-1042, -389, 0, {0}}, {-1283, 0, 0, {0}}, {-456, -661, -456, {0}}, {-736, -389, -736, {0}}, {-907, 0, -907, {0}}, {-22, -424, -1097, {0}}, {-22, -34, -1338, {0}}, {-930, -34, -962, {0}}, {-759, -424, -792, {0}}, {759, -424, -792, {0}}, {930, -34, -962, {0}}, {22, -34, -1338, {0}}, {22, -424, -1097, {0}}, {-792, -424, -759, {0}}, {-962, -34, -930, {0}}, {-1338, -34, -22, {0}}, {-1097, -424, -22, {0}}, {1097, -424, -22, {0}}, {1338, -34, -22, {0}}, {962, -34, -930, {0}}, {792, -424, -759, {0}}, {792, -424, 759, {0}}, {962, -34, 930, {0}}, {1338, -34, 22, {0}}, {1097, -424, 22, {0}}, {-1097, -424, 22, {0}}, {-1338, -34, 22, {0}}, {-962, -34, 930, {0}}, {-792, -424, 759, {0}}, {-759, -424, 792, {0}}, {-930, -34, 962, {0}}, {-22, -34, 1338, {0}}, {-22, -424, 1097, {0}}, {22, -424, 1097, {0}}, {22, -34, 1338, {0}}, {930, -34, 962, {0}}, {759, -424, 792, {0}}, {-752, -443, 773, {0}}, {-15, -443, 1078, {0}}, {-15, -714, 682, {0}}, {-471, -714, 493, {0}}, {15, -443, 1078, {0}}, {752, -443, 773, {0}}, {471, -714, 493, {0}}, {15, -714, 682, {0}}, {773, -443, 752, {0}}, {1078, -443, 15, {0}}, {682, -714, 15, {0}}, {493, -714, 471, {0}}, {1078, -443, -15, {0}}, {773, -443, -752, {0}}, {493, -714, -471, {0}}, {682, -714, -15, {0}}, {752, -443, -773, {0}}, {15, -443, -1078, {0}}, {15, -714, -682, {0}}, {471, -714, -493, {0}}, {-15, -443, -1078, {0}}, {-752, -443, -773, {0}}, {-471, -714, -493, {0}}, {-15, -714, -682, {0}}, {-773, -443, -752, {0}}, {-1078, -443, -15, {0}}, {-682, -714, -15, {0}}, {-493, -714, -471, {0}}, {-1078, -443, 15, {0}}, {-773, -443, 752, {0}}, {-493, -714, 471, {0}}, {-682, -714, 15, {0}}, {645, -732, 0, {0}}, {456, -732, -456, {0}}, {0, -732, -645, {0}}, {-456, -732, -456, {0}}, {-645, -732, 0, {0}}, {-456, -732, 456, {0}}, {0, -732, 645, {0}}, {456, -732, 456, {0}}, {0, -732, 0, {0}}}};
// GLOBAL: FF7 0x008b1110
struct SVECTOR g_BattleMagicQuake3ReferenceVertices[97] = {{0, -661, -645, {0}}, {0, -389, -1042, {0}}, {0, 0, -1283, {0}}, {456, -661, -456, {0}}, {736, -389, -736, {0}}, {907, 0, -907, {0}}, {645, -661, 0, {0}}, {1042, -389, 0, {0}}, {1283, 0, 0, {0}}, {456, -661, 456, {0}}, {736, -389, 736, {0}}, {907, 0, 907, {0}}, {0, -661, 645, {0}}, {0, -389, 1042, {0}}, {0, 0, 1283, {0}}, {-456, -661, 456, {0}}, {-736, -389, 736, {0}}, {-907, 0, 907, {0}}, {-645, -661, 0, {0}}, {-1042, -389, 0, {0}}, {-1283, 0, 0, {0}}, {-456, -661, -456, {0}}, {-736, -389, -736, {0}}, {-907, 0, -907, {0}}, {-378, -954, -1954, {0}}, {-378, -564, -2196, {0}}, {-1285, -564, -1820, {0}}, {-1114, -954, -1649, {0}}, {931, -681, -1207, {0}}, {1102, -291, -1378, {0}}, {195, -291, -1754, {0}}, {195, -681, -1512, {0}}, {-2069, -1214, -1289, {0}}, {-2240, -824, -1459, {0}}, {-2616, -824, -552, {0}}, {-2375, -1214, -552, {0}}, {1792, -854, -310, {0}}, {2033, -464, -310, {0}}, {1657, -464, -1218, {0}}, {1486, -854, -1047, {0}}, {2062, -1210, 1285, {0}}, {2233, -820, 1456, {0}}, {2609, -820, 549, {0}}, {2367, -1210, 549, {0}}, {-2152, -1076, 459, {0}}, {-2393, -686, 459, {0}}, {-2017, -686, 1367, {0}}, {-1846, -1076, 1196, {0}}, {-1061, -875, 1521, {0}}, {-1232, -485, 1692, {0}}, {-324, -485, 2067, {0}}, {-324, -875, 1826, {0}}, {195, -681, 1512, {0}}, {195, -291, 1754, {0}}, {1102, -291, 1378, {0}}, {931, -681, 1207, {0}}, {-1036, -1446, 1459, {0}}, {-299, -1446, 1764, {0}}, {-299, -1717, 1367, {0}}, {-755, -1717, 1178, {0}}, {332, -1565, 1845, {0}}, {1069, -1565, 1540, {0}}, {789, -1836, 1259, {0}}, {332, -1836, 1448, {0}}, {1290, -1199, 966, {0}}, {1595, -1199, 229, {0}}, {1198, -1470, 229, {0}}, {1009, -1470, 685, {0}}, {1875, -1609, -345, {0}}, {1570, -1609, -1082, {0}}, {1289, -1880, -801, {0}}, {1478, -1880, -345, {0}}, {1152, -1858, -1740, {0}}, {415, -1858, -2045, {0}}, {415, -2129, -1648, {0}}, {872, -2129, -1459, {0}}, {-332, -1565, -1845, {0}}, {-1069, -1565, -1540, {0}}, {-789, -1836, -1259, {0}}, {-332, -1836, -1448, {0}}, {-1351, -1289, -991, {0}}, {-1656, -1289, -254, {0}}, {-1260, -1560, -254, {0}}, {-1071, -1560, -711, {0}}, {-2034, -1841, 410, {0}}, {-1729, -1841, 1147, {0}}, {-1448, -2112, 867, {0}}, {-1637, -2112, 410, {0}}, {645, -3261, 0, {0}}, {456, -3261, -456, {0}}, {0, -3261, -645, {0}}, {-456, -3261, -456, {0}}, {-645, -3261, 0, {0}}, {-456, -3261, 456, {0}}, {0, -3261, 645, {0}}, {456, -3261, 456, {0}}, {0, -3261, 0, {0}}};
typedef struct {
    unsigned int descriptor;
    unsigned char *name;
    int reserved[3];
} BattleMagicQuake3ResourceDef;
// GLOBAL: FF7 0x008b1418
BattleMagicQuake3ResourceDef g_BattleMagicQuake3ResourceDefs[12] = {
    {0x001900b1, g_BattleMagicQuake3FilenamesLight1, {0, 0, 0}},
    {0x001900b1, g_BattleMagicQuake3FilenamesLight1 + 0xc, {0, 0, 0}},
    {0x001900b1, g_BattleMagicQuake3FilenamesLight2, {0, 0, 0}},
    {0x001900b1, g_BattleMagicQuake3FilenamesLight2 + 0xc, {0, 0, 0}},
    {0x001900b1, g_BattleMagicQuake3FilenamesSmoke, {0, 0, 0}},
    {0x00192a39, g_BattleMagicQuake3FilenamesSmoke + 0xc, {0, 0, 0}},
    {0x0019783c, g_BattleMagicQuake3FilenamesQueig1, {0, 0, 0}},
    {0x00197839, g_BattleMagicQuake3FilenamesQueig1 + 0xc, {0, 0, 0}},
    {0x00197c3c, g_BattleMagicQuake3FilenamesQueig0, {0, 0, 0}},
    {0x00197c39, g_BattleMagicQuake3FilenamesQueig0 + 0xc, {0, 0, 0}},
    {0x0019783c, g_BattleMagicQuake3FilenamesQueig0Alt, {0, 0, 0}},
    {0x00197839, g_BattleMagicQuake3FilenamesQueig0Alt + 0xc, {0, 0, 0}}
};
// GLOBAL: FF7 0x008b1508
short g_BattleMagicQueigSineTable[1025] = {
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
typedef struct {
    BattleSpriteQuadRequest request;
    unsigned char pad_14[0x4];
} BattleMagicQuake3SpriteDrawReq;
// GLOBAL: FF7 0x008b1d10
BattleMagicQuake3SpriteDrawReq g_BattleMagicQuake3SpriteDrawReq = {{{0xf0, 0xff, 0xe0, 0xff}, -128, {0x80, 0x1f, 0x1f}, 0x2c808080, {0x2d, 0x00, 0xc8, 0x7a}, 0}, {0, 0, 0, 0}};

// GLOBAL: FF7 0x008b1d28
short g_BattleMagicQuake3TransformMatrix[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0};

// GLOBAL: FF7 0x008b1d48
int g_BattleMagicQuake3ModelIndices[4] = {0, 1, 2, 3};
// GLOBAL: FF7 0x008b1d58
short g_BattleMagicQueigBurstFrameDelays1[12] = {1, 2, 4, 5, 7, 8, 9, 11, 12, 15, 17, 19};
// GLOBAL: FF7 0x008b1d70
short g_BattleMagicQueigBurstScales1[12] = {256, 2600, 1024, 1800, 3980, 1600, 3100, 500, 1100, 2400, 3800, 1900};
// GLOBAL: FF7 0x008b1d88
short g_BattleMagicQueigBurstScaleRates1[12] = {4096, 3072, 2400, 2900, 3600, 4096, 2200, 3800, 3000, 4096, 2800, 4096};
// GLOBAL: FF7 0x008b1da0
short g_BattleMagicQueigBurstFrameDelays2[7] = {5, 6, 7, 8, 9, 13, 15};
// GLOBAL: FF7 0x008b1db0
short g_BattleMagicQueigBurstOffsetsX[7] = {-1400, -400, 900, -850, -550, 450, 1200};
// GLOBAL: FF7 0x008b1dc0
short g_BattleMagicQueigBurstOffsetsZ[7] = {1000, -1400, 800, -700, 600, 900, -950};
// GLOBAL: FF7 0x008b1dd0
short g_BattleMagicQueigBurstFrameDelays3[8] = {1, 3, 5, 8, 11, 13, 16, 18};
// GLOBAL: FF7 0x008b1de0
short g_BattleMagicQueigBurstScales3[8] = {256, 3200, 1024, 2200, 3980, 1600, 2680, 768};
// GLOBAL: FF7 0x008b1df0
short g_BattleMagicQueigBurstScaleRates3[8] = {4096, 3072, 4096, 2600, 3600, 4096, 2200, 3800};
// GLOBAL: FF7 0x008b1e00
unsigned char g_BattleMagicQuake3FilenamesLight1[24] = {0x6c, 0x69, 0x67, 0x68, 0x74, 0x5f, 0x31, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x6c, 0x69, 0x67, 0x68, 0x74, 0x5f, 0x32, 0x2e, 0x52, 0x53, 0x44, 0x00};
// GLOBAL: FF7 0x008b1e18
unsigned char g_BattleMagicQuake3FilenamesLight2[24] =
    "light_3.RSD\0"
    "light_4.RSD";
// GLOBAL: FF7 0x008b1e30
unsigned char g_BattleMagicQuake3FilenamesSmoke[24] =
    "smoke.RSD\0\0\0"
    "QUEIG01.TIM";
// GLOBAL: FF7 0x008b1e48
unsigned char g_BattleMagicQuake3FilenamesQueig1[24] =
    "QUEIG01.TIM\0"
    "QUEIG01.TIM";
// GLOBAL: FF7 0x008b1e60
unsigned char g_BattleMagicQuake3FilenamesQueig0[24] =
    "QUEIG00.TIM\0"
    "QUEIG00.TIM";
// GLOBAL: FF7 0x008b1e78
unsigned char g_BattleMagicQuake3FilenamesQueig0Alt[24] =
    "QUEIG00.TIM\0"
    "QUEIG00.TIM";
// GLOBAL: FF7 0x00bb0848
struct BattleMagicQuake3Shard g_BattleMagicQuake3ShardRecords[96] = {0};
// GLOBAL: FF7 0x00bb1148
int g_BattleMagicQuake3TargetMask = 0;
unsigned char g_bssPad_00bb114c[4] = {0};
// GLOBAL: FF7 0x00bb1150
BattleFxShatterMeshDelta g_BattleMagicQuake3MeshVertexDeltas[97] = {0};
// GLOBAL: FF7 0x00bb1458
struct GfxTableCursor *g_BattleMagicQuake3LoadedResTable = 0;
unsigned char g_bssPad_00bb145c[4] = {0};
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/*
 * Loads resources and registers sound groups for Queig.
 */
// FUNCTION: FF7 0x00581d50
void __cdecl BattleFx_MagicLoadResQuake3(void)
{
    char pathBuf[200];
    GfxResourceRequest requestBuf;
    int kind;
    GfxModelTableBank *sceneOwner;
    GfxResourceRequest *requestPtr;
    int defsTblRef;

    if (g_pBattle3dScene != 0 && g_BattleMagicQuake3LoadedResTable == 0) {
        kind = 0xC;
        defsTblRef = (int)g_BattleMagicQuake3ResourceDefs;
        sceneOwner = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        requestPtr = &requestBuf;
        Gfx_InitResourceRequest(requestPtr);
        requestPtr->groupTag = 0;
        // STRING: FF7 0x008b1e90
        requestPtr->resourcePath = BattleFx_BuildResourcePath("magic/queig", pathBuf);
        Gfx_LoadModelTableIfAbsent(kind, defsTblRef, requestPtr, &g_BattleMagicQuake3LoadedResTable, sceneOwner);
        SfxRegisterSoundGroups((struct SoundGroup **)&g_BattleMagicQuake3SoundGroups);
    }
}

/*
 * Entry point for Queig magic effect.
 */
// FUNCTION: FF7 0x00581df1
void __cdecl BattleFx_MagicEnterQuake3(int targetMask, int actorIdx)
{
    BattleFx_MagicBeginQuake3(targetMask, actorIdx);
}

/*
 * Initializes Queig effect and schedules timeline selection task.
 */
// FUNCTION: FF7 0x00581e06
void __cdecl BattleFx_MagicBeginQuake3(int targetMask, int actorIdx)
{
    memset(g_BattleMagicQuake3MeshVertexDeltas, 0, 0x308);
    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    g_BattleFxActiveDrawListPtr = Gfx_GetPsxPacketScratchPtr();
    BattleFx_MagicLoadResQuake3();
    Battle_AllocateResourceSlot(BattleFx_MagicQuake3SelectTimelineTask);
    g_BattleMagicQuake3TargetMask = targetMask;
}

/*
 * Selects single- or multi-target Queig timeline task.
 */
// FUNCTION: FF7 0x00581e5d
void __cdecl BattleFx_MagicQuake3SelectTimelineTask(void)
{
    BattleFxQuake3ShardState *shards;
    int idx;
    int targetCount;
    BattleFxQuake3BurstTimeline *task;
    int firstTarget;

    idx = 0;
    for (targetCount = 0; idx < 10; idx++) {
        if ((g_BattleMagicQuake3TargetMask & 1 << idx) != 0) {
            targetCount++;
            if (targetCount == 1) {
                firstTarget = idx;
            }
        }
    }
    task = (BattleFxQuake3BurstTimeline *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    shards = (BattleFxQuake3ShardState *)Gfx_GetPsxScratchpadPtr(0);
    shards->meshPayload = (int)&g_BattleMagicQuake3MeshPayload;
    shards->referenceVertices = (int)&g_BattleMagicQuake3ReferenceVertices;
    if (targetCount == 1) {
        task = (BattleFxQuake3BurstTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicQuake3SingleTargetTimelineTask);
        task->burstIndexA = 0;
        Battle_GetBoneWorldPos(firstTarget, g_BattleActorRecords[firstTarget].targetBone, &task->originX);
        task->originY = 0;
        task->size = g_BattleActorRecords[firstTarget].radius + 512;
        if (task->size > 1900) {
            task->size = 1900;
        }
        shards->shardCount = 11;
        shards->baseVelocity = 8;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(g_BattleMagicQuake3TargetMask), 0x58);
    } else {
        task = (BattleFxQuake3BurstTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicQuake3MultiTargetTimelineTask);
        task->burstIndexB = 0;
        task->burstIndexA = 0;
        Battle_GetActorMaskCentroid(g_BattleMagicQuake3TargetMask, &task->originX);
        task->size = 2048;
        shards->shardCount = 11;
        shards->baseVelocity = 0x10;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_2, Battle_ComputePanForEntity(g_BattleMagicQuake3TargetMask), 0);
    }
    BattleFx_MagicQuake3BuildShardRecords(shards);
    BattleFx_MagicQuake3CopyMeshVertices(g_BattleMagicQuake3MeshPayload.seedVertices);
}

/*
 * Builds triangle and quad shard records for Queig mesh.
 */
// FUNCTION: FF7 0x00582042
void __cdecl BattleFx_MagicQuake3BuildShardRecords(BattleFxQuake3ShardState *shardState)
{
    BattleFxShardTriSlot *pRecords;
    int refVertices;

    refVertices = shardState->referenceVertices;
    pRecords = (BattleFxShardTriSlot *)(shardState->meshPayload + 4 + *(int *)shardState->meshPayload);
    shardState->shardIndex = 0;
    pRecords = (BattleFxShardTriSlot *)BattleFx_MagicQuake3BuildTriangleShardRecords(shardState, refVertices, pRecords);
    pRecords = (BattleFxShardTriSlot *)BattleFx_MagicQuake3BuildQuadShardRecords(shardState, refVertices, pRecords);
}

/*
 * Builds triangle shard particle records for Queig.
 */
// FUNCTION: FF7 0x005820a0
int __cdecl BattleFx_MagicQuake3BuildTriangleShardRecords(BattleFxQuake3ShardState *shardState, int vertexBase, struct BattleFxShardTriSlot *records)
{
    int sumX;
    BattleFxTexturedTriRecord *recordCursor;
    int sumY;
    int shardIndex;
    int i;
    Vec3sPacked *vertex;
    int sumZ;
    int randomBits;
    int unitScale;
    int recordCount;
    int baseVelocity;

    recordCount = *(int *)records & 0xFFFF;
    shardIndex = shardState->shardIndex;
    baseVelocity = shardState->baseVelocity;
    recordCursor = (BattleFxTexturedTriRecord *)((char *)records + 4);
    unitScale = 0x2EBAE40 / shardState->shardCount;
    for (i = 0; i < recordCount; i++) {
        vertex = (Vec3sPacked *)(vertexBase + recordCursor->vertexOffset0);
        sumX = vertex->vx;
        sumY = vertex->vy;
        sumZ = vertex->vz;
        vertex = (Vec3sPacked *)(vertexBase + recordCursor->vertexOffset1);
        sumX += vertex->vx;
        sumY += vertex->vy;
        sumZ += vertex->vz;
        vertex = (Vec3sPacked *)(vertexBase + recordCursor->vertexOffset2);
        sumX += vertex->vx;
        sumY += vertex->vy;
        sumZ += vertex->vz;
        sumX = sumX / 3;
        sumY = sumY / 3;
        sumZ = sumZ / 3;
        g_BattleMagicQuake3ShardRecords[shardIndex].positionX = sumX;
        g_BattleMagicQuake3ShardRecords[shardIndex].centroidY = sumY;
        g_BattleMagicQuake3ShardRecords[shardIndex].centroidZ = sumZ;
        sumY = sumY * sumY / unitScale;
        g_BattleMagicQuake3ShardRecords[shardIndex].positionY = sumY;
        g_BattleMagicQuake3ShardRecords[shardIndex].fadeCounter = 0;
        g_BattleMagicQuake3ShardRecords[shardIndex].rotationZ = 0;
        g_BattleMagicQuake3ShardRecords[shardIndex].rotationY = 0;
        g_BattleMagicQuake3ShardRecords[shardIndex].rotationX = 0;
        randomBits = rand();
        if ((randomBits & 0x800) != 0) {
            g_BattleMagicQuake3ShardRecords[shardIndex].rotationSpeedX = (randomBits & 7) << 4;
        } else {
            g_BattleMagicQuake3ShardRecords[shardIndex].rotationSpeedX = -((randomBits & 7) << 4);
        }
        g_BattleMagicQuake3ShardRecords[shardIndex].velocityY = 0;
        if ((randomBits & 0x204080) != 0) {
            g_BattleMagicQuake3ShardRecords[shardIndex].rotationSpeedZ = (randomBits >> 3 & 7) << 4;
        } else {
            g_BattleMagicQuake3ShardRecords[shardIndex].rotationSpeedZ = -((randomBits >> 3 & 7) << 4);
        }
        g_BattleMagicQuake3ShardRecords[shardIndex].accelerationY = (sumY + baseVelocity) * ((randomBits & 1) + 3);
        shardIndex++;
        recordCursor++;
    }
    shardState->shardIndex = shardIndex;
    return (int)recordCursor;
}

/*
 * Builds quad shard particle records for Queig.
 */
// FUNCTION: FF7 0x00582338
int __cdecl BattleFx_MagicQuake3BuildQuadShardRecords(BattleFxQuake3ShardState *shardState, int vertexBase, struct BattleFxShardTriSlot *records)
{
    int sumX;
    BattleFxShardQuadSlot *recordCursor;
    int sumY;
    int shardIndex;
    int i;
    Vec3sPacked *vertex;
    int sumZ;
    int randomBits;
    int stageSpeedBase;
    int unitScale;
    int recordCount;

    recordCount = *(int *)records & 0xFFFF;
    shardIndex = shardState->shardIndex;
    stageSpeedBase = shardState->baseVelocity;
    recordCursor = (BattleFxShardQuadSlot *)((char *)records + 4);
    unitScale = 0x00A12BC4 / shardState->shardCount;
    for (i = 0; i < recordCount; i++) {
        vertex = (Vec3sPacked *)(vertexBase + recordCursor->vertex0Offset);
        sumX = vertex->vx;
        sumY = vertex->vy;
        sumZ = vertex->vz;
        vertex = (Vec3sPacked *)(vertexBase + recordCursor->vertex1Offset);
        sumX += vertex->vx;
        sumY += vertex->vy;
        sumZ += vertex->vz;
        vertex = (Vec3sPacked *)(vertexBase + recordCursor->vertex2Offset);
        sumX += vertex->vx;
        sumY += vertex->vy;
        sumZ += vertex->vz;
        vertex = (Vec3sPacked *)(vertexBase + recordCursor->vertex3Offset);
        sumX += vertex->vx;
        sumY += vertex->vy;
        sumZ += vertex->vz;
        sumX /= 4;
        sumY /= 4;
        sumZ /= 4;
        g_BattleMagicQuake3ShardRecords[shardIndex].positionX = sumX;
        g_BattleMagicQuake3ShardRecords[shardIndex].centroidY = sumY;
        g_BattleMagicQuake3ShardRecords[shardIndex].centroidZ = sumZ;
        sumY = sumY * sumY / unitScale;
        g_BattleMagicQuake3ShardRecords[shardIndex].positionY = sumY;
        g_BattleMagicQuake3ShardRecords[shardIndex].fadeCounter = 0;
        g_BattleMagicQuake3ShardRecords[shardIndex].rotationZ = 0;
        g_BattleMagicQuake3ShardRecords[shardIndex].rotationY = 0;
        g_BattleMagicQuake3ShardRecords[shardIndex].rotationX = 0;
        randomBits = rand();
        if ((randomBits & 0x800) != 0) {
            g_BattleMagicQuake3ShardRecords[shardIndex].rotationSpeedX = (randomBits & 7) << 4;
        } else {
            g_BattleMagicQuake3ShardRecords[shardIndex].rotationSpeedX = -((randomBits & 7) << 4);
        }
        g_BattleMagicQuake3ShardRecords[shardIndex].velocityY = 0;
        if ((randomBits & 0x204080) != 0) {
            g_BattleMagicQuake3ShardRecords[shardIndex].rotationSpeedZ = (randomBits >> 3 & 7) << 4;
        } else {
            g_BattleMagicQuake3ShardRecords[shardIndex].rotationSpeedZ = -((randomBits >> 3 & 7) << 4);
        }
        g_BattleMagicQuake3ShardRecords[shardIndex].accelerationY = (sumY + stageSpeedBase) * ((randomBits & 1) + 3);
        shardIndex++;
        recordCursor++;
    }
    shardState->shardIndex = shardIndex;
    return (int)recordCursor;
}

/*
 * Copies vertex records into Queig mesh buffer.
 */
// FUNCTION: FF7 0x00582610
void __cdecl BattleFx_MagicQuake3CopyMeshVertices(SVECTOR *srcVertices)
{
    int index;
    SVECTOR *dstVertices;

    dstVertices = g_BattleMagicQuake3MeshPayload.vertices;
    for (index = 0; index < 97; index++) {
        dstVertices[index] = srcVertices[index];
    }
}

/*
 * Timeline task driving multi-target Queig effect.
 */
// FUNCTION: FF7 0x00582657
void __cdecl BattleFx_MagicQuake3MultiTargetTimelineTask(void)
{
    BattleFxQuake3Burst *burstPtr;
    BattleFxQuake3RingSlot *ring;
    BattleFxQuake3BurstTimeline *taskPtr;

    if (g_BattleIsPaused)
        return;
    taskPtr = (BattleFxQuake3BurstTimeline *)&g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx];
    /* Numeric burst-delay table index. */
    switch (taskPtr->burstIndexA) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        if (taskPtr->timer < g_BattleMagicQueigBurstFrameDelays1[taskPtr->burstIndexA])
            break;
        burstPtr = (BattleFxQuake3Burst *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicQuake3PulsingIndexedModelTask);
        burstPtr->clearedWord = 0;
        *(BattleSchedulePair *)&burstPtr->posX = *(BattleSchedulePair *)&taskPtr->originX;
        burstPtr->angle = 0;
        burstPtr->scale = g_BattleMagicQueigBurstScales1[taskPtr->burstIndexA];
        burstPtr->radius = ((taskPtr->size * g_BattleMagicQueigBurstScaleRates1[taskPtr->burstIndexA] >> 12) << 12) / 2500;
        burstPtr->baseRadius = taskPtr->burstIndexA & 3;
        burstPtr->growStep = burstPtr->radius / 7;
        taskPtr->burstIndexA += 1;
        break;
    default:
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
    /* Numeric burst-delay table index. */
    switch (taskPtr->burstIndexB) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        if (taskPtr->timer < g_BattleMagicQueigBurstFrameDelays2[taskPtr->burstIndexB])
            break;
        burstPtr = (BattleFxQuake3Burst *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicQuake3GroundQuadTask);
        burstPtr->posX = taskPtr->originX + g_BattleMagicQueigBurstOffsetsX[taskPtr->burstIndexB];
        burstPtr->posY = 0;
        burstPtr->posZ = taskPtr->originZ + g_BattleMagicQueigBurstOffsetsZ[taskPtr->burstIndexB];
        taskPtr->burstIndexB += 1;
    }
    if (taskPtr->timer == 12) {
        burstPtr = (BattleFxQuake3Burst *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicQuake3MultiTargetBurstModelTask);
        *(BattleSchedulePair *)&burstPtr->posX = *(BattleSchedulePair *)&taskPtr->originX;
        burstPtr->radius = 0;
        burstPtr->baseRadius = taskPtr->size / 12;
        burstPtr->morphWeight = 0;
        burstPtr->growStep = 1024;
        BattleFx_MagicQuake3BuildMeshVertexDeltas(g_BattleMagicQuake3MeshPayload.seedVertices, g_BattleMagicQuake3ReferenceVertices, g_BattleMagicQuake3MeshVertexDeltas);
        ring = (BattleFxQuake3RingSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicQuake3FlatteningRingModelTask);
        *(BattleSchedulePair *)&ring->posX = *(BattleSchedulePair *)&taskPtr->originX;
        ring->radius = burstPtr->baseRadius;
        ring->growth = burstPtr->baseRadius + (burstPtr->baseRadius >> 1);
    }
    taskPtr->timer += 1;
}

/*
 * Updates and renders flattening ring model for Queig.
 */
// FUNCTION: FF7 0x00582924
void __cdecl BattleFx_MagicQuake3FlatteningRingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[80];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxQuake3RingSlot *ringSlot; };
        struct { unsigned char pad_to_14[0x14]; int flattenScale; };
        struct { unsigned char pad_to_18[0x18]; int ringScale; };
        struct { unsigned char pad_to_1C[0x1C]; PsxMatrix modelMatrix; };
        struct { unsigned char pad_to_3C[0x3C]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_40[0x40]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.ringSlot = (BattleFxQuake3RingSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.modelMatrix);
    frame.modelMatrix.t[0] = frame.ringSlot->posX;
    frame.modelMatrix.t[1] = frame.ringSlot->posY;
    frame.modelMatrix.t[2] = frame.ringSlot->posZ;
    frame.ringScale = (frame.ringSlot->radius << 11) / 1450;
    frame.flattenScale = 0x1000 - frame.ringSlot->step * 0x155;
    frame.scaleVec.vx = frame.ringScale;
    frame.scaleVec.vy = frame.flattenScale;
    frame.scaleVec.vz = frame.ringScale;
    ScaleMatrix(&frame.modelMatrix, &frame.scaleVec);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(4, g_BattleMagicQuake3LoadedResTable);
    frame.modelDraw->flags = 8;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 128;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.ringSlot->step >= 12) {
        frame.ringSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.ringSlot->radius += frame.ringSlot->growth;
    (frame.ringSlot->step)++;
}

/*
 * Computes vertex differences between two Queig mesh poses.
 */
// FUNCTION: FF7 0x00582ab9
void __cdecl BattleFx_MagicQuake3BuildMeshVertexDeltas(SVECTOR *prevPose, SVECTOR *nextPose, BattleFxShatterMeshDelta *deltas)
{
    int index;

    for (index = 0; index < 97; index++) {
        deltas[index].deltaX = nextPose[index].vx - prevPose[index].vx;
        deltas[index].deltaY = nextPose[index].vy - prevPose[index].vy;
        deltas[index].deltaZ = nextPose[index].vz - prevPose[index].vz;
    }
}

#pragma pack(push, 1)

/*
 * Multi-target burst model animation task for Queig.
 */
// FUNCTION: FF7 0x00582b41
void __cdecl BattleFx_MagicQuake3MultiTargetBurstModelTask(void)
{
    struct {
        short rotation[3];
        short pad;
        BattleFxQuake3Burst *slot;
        BattleFxDebrisParticle *output;
        int choice;
        int scaleY;
        BattleFxQuake3Shard *buffer;
        int scale;
        PsxMatrix matrix;
        short pad2;
        BattleFxQuake3MeshDraw *packet;
        int vector[3];
        int unused;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.slot = (BattleFxQuake3Burst *)BATTLE_CURRENT_TASK_SLOT;
    frame.scale = (frame.slot->radius << 12) / 1450;
    if (frame.slot->timer < 12)
        frame.scaleY = 16;
    else if (frame.slot->timer < 16)
        frame.scaleY = frame.scale - (frame.scale / 4) * (16 - frame.slot->timer);
    else
        frame.scaleY = frame.scale;
    if (frame.slot->timer < 26) {
        frame.rotation[2] = 0;
        frame.rotation[1] = frame.rotation[2];
        frame.rotation[0] = frame.rotation[1];
        RotMatrixXYZ(frame.rotation, &frame.matrix);
        frame.matrix.t[0] = frame.slot->posX;
        frame.matrix.t[1] = frame.slot->posY;
        frame.matrix.t[2] = frame.slot->posZ;
        frame.vector[0] = frame.scale;
        frame.vector[1] = frame.scaleY;
        frame.vector[2] = frame.scale;
        ScaleMatrix(&frame.matrix, (void *)frame.vector);
        MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
        SetRotMatrix(&frame.matrix);
        SetTransMatrix(&frame.matrix);
        RefreshGlobalTransformState();
        frame.packet = (BattleFxQuake3MeshDraw *)Gfx_GetPsxScratchpadPtr(0);
        frame.packet->meshData = (int)&g_BattleMagicQuake3MeshPayload;
        frame.packet->flags = 0;
        frame.packet->unused08 = 0;
        frame.packet->vertexShade = 0x80;
        frame.packet->attrBits = 0;
        frame.packet->unused0E = 0;
        Gfx_SetPsxPrimQuadState(9, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(8, g_BattleMagicQuake3LoadedResTable)));
        Gfx_SetPsxPrimQuadState(9, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(9, g_BattleMagicQuake3LoadedResTable)));
        Gfx_SetPsxPrimQuadState(11, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(10, g_BattleMagicQuake3LoadedResTable)));
        Gfx_SetPsxPrimQuadState(11, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(11, g_BattleMagicQuake3LoadedResTable)));
        g_BattleFxActiveDrawListPtr = BattleFx_MagicQuake3DrawPsxMesh(frame.packet, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 12, g_BattleFxActiveDrawListPtr);
    } else {
        frame.buffer = (BattleFxQuake3Shard *)Gfx_GetPsxScratchpadPtr(0);
        frame.buffer->meshBase = (int)&g_BattleMagicQuake3MeshPayload;
        frame.buffer->orderingTable = g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED;
        frame.buffer->drawMode = 12;
        frame.buffer->primitive = g_BattleFxActiveDrawListPtr;
        frame.buffer->fadeStart = 3;
        frame.buffer->cameraMatrix = *(PsxMatrix *)(int *)&g_BattleCameraTransform;
        frame.buffer->scale.scaleZ = frame.scale;
        frame.buffer->scale.scaleY = frame.scale;
        frame.buffer->scale.scaleX = frame.scale;
        *(BattleSchedulePair *)&frame.buffer->baseX = *(BattleSchedulePair *)&frame.slot->posX;
        Gfx_SetPsxPrimQuadState(9, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(8, g_BattleMagicQuake3LoadedResTable)));
        Gfx_SetPsxPrimQuadState(9, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(9, g_BattleMagicQuake3LoadedResTable)));
        Gfx_SetPsxPrimQuadState(11, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(10, g_BattleMagicQuake3LoadedResTable)));
        Gfx_SetPsxPrimQuadState(11, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(11, g_BattleMagicQuake3LoadedResTable)));
        g_BattleFxActiveDrawListPtr = BattleFx_MagicQuake3DrawShardMesh(frame.buffer, frame.slot->timer - 26);
    }
    if (frame.slot->timer >= 45)
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    if (g_BattleIsPaused != 0)
        return;
    if (frame.slot->timer == 26) {
        for (frame.scale = 0; frame.scale < 10; frame.scale++) {
            if (g_BattleMagicQuake3TargetMask & (1 << frame.scale))
                Battle_PostActorHitOrHurtMessage(frame.scale);
        }
    }
    if (frame.slot->timer < 12) {
        frame.slot->radius += frame.slot->baseRadius;
        frame.choice = rand() % 4;
        frame.scale = frame.slot->radius >> 1;
        for (frame.scaleY = 0; frame.scaleY < 4; frame.scaleY++) {
            frame.output = (BattleFxDebrisParticle *)&g_BattleTaskSlotBuffer.mainSlots[Battle_AllocateResourceSlot(BattleFx_MagicQuake3FadingQuadParticleTask)];
            frame.output->posX = frame.slot->posX + rand() % (frame.scale * 2) - frame.scale;
            frame.output->position = frame.slot->posY - rand() % (frame.scale >> 3);
            frame.output->posZ = frame.slot->posZ + rand() % (frame.scale * 2) - frame.scale;
            frame.output->fadeWord = ((frame.choice + frame.scaleY) & 3) << 5;
            frame.output->heightOffset = (rand() % 4) * 1365 + 2048;
            frame.output->velocity = frame.output->fadeWord + frame.slot->timer * 2 - 384;
            frame.output->acceleration = rand() % 12 + frame.slot->timer + 12;
            frame.output->fadeWord += 0x80;
        }
    } else if (frame.slot->timer >= 15 && frame.slot->timer < 20) {
        BattleFx_MagicQuake3CopyMeshVertices(g_BattleMagicQuake3MeshPayload.seedVertices);
        BattleFx_MagicQuake3AddScaledMeshVertexDeltas(g_BattleMagicQuake3MeshPayload.vertices, g_BattleMagicQuake3MeshVertexDeltas, 97, frame.slot->morphWeight);
        frame.slot->morphWeight += frame.slot->growStep;
        if (frame.slot->morphWeight > 4096)
            frame.slot->morphWeight = 0x1000;
    }
    ++frame.slot->timer;
}
#pragma pack(pop)

/*
 * Draws PSX textured mesh geometry for Queig effect.
 */
// FUNCTION: FF7 0x005830c7
int __cdecl BattleFx_MagicQuake3DrawPsxMesh(BattleFxQuake3MeshDraw *mesh, int orderingTable, int depthShift, int packetCursor)
{
    int unusedSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int origin;
    unsigned short blendMode;
    int color[2];
    int state;
    BattleFxQuake3PsxPacket *primPrevious;
    int i;
    int padLocal; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int bytes;
    BattleFxQuake3Prim *primB;
    int vert0;
    int vert1;
    int vert2;
    int numberEntries;
    BattleFxQuake3PsxPacket *triPtr;
    int capBits;
    BattleMagicQuake3MeshRecord *entry;
    BattleFxQuake3Prim *renderStatus;

    origin = mesh->meshData + 4;
    entry = (BattleMagicQuake3MeshRecord *)(origin + *(int *)mesh->meshData);
    renderStatus = (BattleFxQuake3Prim *)Gfx_GetPsxPacketScratchPtr();
    numberEntries = entry->numberEntries;
    entry = (BattleMagicQuake3MeshRecord *)((int)entry + 2);
    blendMode = entry->numberEntries & 0x19F | mesh->attrBits;
    entry = (BattleMagicQuake3MeshRecord *)((int)entry + 2);
    vert0 = origin + entry->numberEntries;
    vert1 = origin + entry->vertex1Offset;
    vert2 = origin + entry->vertex2Offset;
    for (i = 0; i < numberEntries; i++) {
        StoreDeferredDrawArgs((const void *)vert0, (const void *)vert1, (const void *)vert2);
        ApplyDeferredDrawArgs();
        vert0 = origin + entry->vertex0Offset;
        vert1 = origin + entry->colorB;
        vert2 = origin + entry->vertexDeltaA;
        Gfx_PsxStoreClearFlag(&capBits);
        if ((capBits & 0x60000) == 0) {
            if ((mesh->flags & BATTLE_FX_BZ_MORPH_DOUBLE_SIDED) != 0) {
                state = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&state);
            }
            if (state > 0) {
                renderStatus->tag = 0x7000000;
                Gfx_ReadTransformOrderingDepth2(&bytes);
                renderStatus->primWord = entry->primWord;
                renderStatus->uvWord0 = entry->colorC;
                renderStatus->uvWord1 = entry->colorD;
                renderStatus->blendMode = blendMode;
                if ((mesh->flags & BATTLE_FX_BZ_MORPH_FACE_GREY) != 0) {
                    color[0] = entry->colorA & 0xFF;
                    color[0] = color[0] << 0x10 | 0x24000000 | color[0] << 8 | color[0];
                    if (mesh->vertexShade != 0) {
                        Gfx_ShadePsxVertexColor((CVECTOR *)color, mesh->vertexShade, (CVECTOR *)&renderStatus->colorCmd);
                        renderStatus->gpuCode = 0x24;
                    } else {
                        renderStatus->colorCmd = color[0];
                    }
                } else {
                    renderStatus->colorCmd = color[0] << 0x10 | 0x24000000 | color[0] << 8 | color[0];
                }
                if ((mesh->flags & BATTLE_FX_BZ_MORPH_SEMI_TRANSPARENT) != 0) {
                    renderStatus->gpuCode |= 2;
                }
                Gfx_DrawPsxPrimitive(renderStatus);
            }
        }
        entry = (BattleMagicQuake3MeshRecord *)((int)entry + 0x10);
    }
    numberEntries = entry->numberEntries;
    entry = (BattleMagicQuake3MeshRecord *)((int)entry + 4);
    primB = (BattleFxQuake3Prim *)renderStatus;
    vert0 = origin + entry->numberEntries;
    vert1 = origin + entry->vertex1Offset;
    vert2 = origin + entry->vertex2Offset;
    for (i = 0; i < numberEntries; i++) {
        StoreDeferredDrawArgs((const void *)vert0, (const void *)vert1, (const void *)vert2);
        ApplyDeferredDrawArgs();
        vert0 = origin + entry->vertexDeltaA;
        vert1 = origin + entry->vertexDeltaB;
        vert2 = origin + entry->vertexDeltaC;
        Gfx_PsxStoreClearFlag(&capBits);
        if ((capBits & 0x60000) == 0) {
            if ((mesh->flags & BATTLE_FX_BZ_MORPH_DOUBLE_SIDED) != 0) {
                state = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&state);
            }
            if (state >= 0) {
                SetPendingResourceHandle((GfxPsxShortVertexSlot *)(origin + entry->colorA));
                ApplyPendingResourceHandle();
                Gfx_ReadTransformOrderingDepth2(&bytes);
                primB->tag = 0x9000000;
                primB->primWord = entry->primWord;
                primB->uvWord0 = entry->colorC;
                primB->uvWord1 = entry->colorD;
                primB->vertex0Offset = entry->vertex0Offset;
                primB->blendMode = blendMode;
                if ((mesh->flags & BATTLE_FX_BZ_MORPH_FACE_GREY) != 0) {
                    color[0] = entry->colorB & 0xFF;
                    color[0] = color[0] << 0x10 | 0x2C000000 | color[0] << 8 | color[0];
                    if (mesh->vertexShade != 0) {
                        Gfx_ShadePsxVertexColor((CVECTOR *)color, mesh->vertexShade, (CVECTOR *)&primB->colorCmd);
                        primB->gpuCode = 0x2C;
                    } else {
                        primB->colorCmd = color[0];
                    }
                } else {
                    color[0] = mesh->vertexShade & 0xFF;
                    primB->colorCmd = color[0] << 0x10 | 0x2C000000 | color[0] << 8 | color[0];
                }
                if ((mesh->flags & BATTLE_FX_BZ_MORPH_SEMI_TRANSPARENT) != 0) {
                    primB->gpuCode |= 2;
                }
                Gfx_DrawPsxPrimitive(primB);
            }
        }
        entry = (BattleMagicQuake3MeshRecord *)((int)entry + 0x14);
    }
    numberEntries = entry->numberEntries;
    entry = (BattleMagicQuake3MeshRecord *)((int)entry + 4);
    triPtr = (BattleFxQuake3PsxPacket *)primB;
    vert0 = origin + entry->numberEntries;
    vert1 = origin + entry->vertex1Offset;
    vert2 = origin + entry->vertex2Offset;
    for (i = 0; i < numberEntries; i++) {
        StoreDeferredDrawArgs((const void *)vert0, (const void *)vert1, (const void *)vert2);
        ApplyDeferredDrawArgs();
        vert0 = origin + entry->vertexDeltaA;
        vert1 = origin + entry->vertexDeltaB;
        vert2 = origin + entry->vertexDeltaC;
        Gfx_PsxStoreClearFlag(&capBits);
        if ((capBits & 0x60000) == 0) {
            if ((mesh->flags & BATTLE_FX_BZ_MORPH_DOUBLE_SIDED) != 0) {
                state = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&state);
            }
            if (state >= 0) {
                triPtr->tag = 0x6000000;
                Gfx_ReadTransformOrderingDepth2(&bytes);
                if (mesh->vertexShade != 0) {
                    Gfx_ShadePsxTriVertexColors((CVECTOR *)((int)entry + 8), (CVECTOR *)((int)entry + 0xC), (CVECTOR *)((int)entry + 0x10), mesh->vertexShade, (CVECTOR *)&triPtr->colorCmd, (CVECTOR *)&triPtr->primWord, (CVECTOR *)&triPtr->texWord);
                } else {
                    triPtr->colorCmd = entry->primWord;
                    triPtr->primWord = entry->texturedPrimWord;
                    triPtr->texWord = entry->texWord;
                }
                if ((mesh->flags & BATTLE_FX_BZ_MORPH_SEMI_TRANSPARENT) != 0) {
                    triPtr->gpuCode |= 2;
                }
                Gfx_DrawPsxPrimitive(triPtr);
            }
        }
        entry = (BattleMagicQuake3MeshRecord *)((int)entry + 0x14);
    }
    numberEntries = entry->numberEntries;
    entry = (BattleMagicQuake3MeshRecord *)((int)entry + 4);
    primPrevious = (BattleFxQuake3PsxPacket *)triPtr;
    vert0 = origin + entry->numberEntries;
    vert1 = origin + entry->vertex1Offset;
    vert2 = origin + entry->vertex2Offset;
    for (i = 0; i < numberEntries; i++) {
        StoreDeferredDrawArgs((const void *)vert0, (const void *)vert1, (const void *)vert2);
        ApplyDeferredDrawArgs();
        vert0 = origin + entry->vertexDeltaC;
        vert1 = origin + entry->nextVertex1Offset;
        vert2 = origin + entry->nextVertex2Offset;
        Gfx_PsxStoreClearFlag(&capBits);
        if ((capBits & 0x60000) == 0) {
            if ((mesh->flags & BATTLE_FX_BZ_MORPH_DOUBLE_SIDED) != 0) {
                state = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&state);
            }
            if (state >= 0) {
                SetPendingResourceHandle((GfxPsxShortVertexSlot *)(origin + entry->colorA));
                ApplyPendingResourceHandle();
                Gfx_ReadTransformOrderingDepth2(&bytes);
                primPrevious->tag = 0x8000000;
                if (mesh->vertexShade != 0) {
                    Gfx_ShadePsxTriVertexColors((CVECTOR *)((int)entry + 8), (CVECTOR *)((int)entry + 0xC), (CVECTOR *)((int)entry + 0x10), mesh->vertexShade, (CVECTOR *)&primPrevious->colorCmd, (CVECTOR *)&primPrevious->primWord, (CVECTOR *)&primPrevious->texWord);
                    Gfx_ShadePsxVertexColor((CVECTOR *)((int)entry + 0x14), mesh->vertexShade, (CVECTOR *)&primPrevious->texWord2);
                } else {
                    primPrevious->colorCmd = entry->primWord;
                    primPrevious->primWord = entry->texturedPrimWord;
                    primPrevious->texWord = entry->texWord;
                    primPrevious->texWord2 = entry->texWord2;
                }
                if ((mesh->flags & BATTLE_FX_BZ_MORPH_SEMI_TRANSPARENT) != 0) {
                    primPrevious->gpuCode |= 2;
                }
                Gfx_DrawPsxPrimitive(primPrevious);
            }
        }
        entry = (BattleMagicQuake3MeshRecord *)((int)entry + 0x18);
    }
    return (int)primPrevious;
}

/*
 * Accumulates scaled vertex deltas into Queig vertex array.
 */
// FUNCTION: FF7 0x005838d3
void __cdecl BattleFx_MagicQuake3AddScaledMeshVertexDeltas(SVECTOR *dstVertices, BattleFxShatterMeshDelta *srcDeltas, int count, int scale)
{
    int i;

    for (i = 0; i < count; i++) {
        dstVertices[i].vx += (short)(srcDeltas[i].deltaX * scale >> 12);
        dstVertices[i].vy += (short)(srcDeltas[i].deltaY * scale >> 12);
        dstVertices[i].vz += (short)(srcDeltas[i].deltaZ * scale >> 12);
    }
}

/*
 * Submits triangle and quad shard meshes for drawing.
 */
// FUNCTION: FF7 0x0058397c
int __cdecl BattleFx_MagicQuake3DrawShardMesh(BattleFxQuake3Shard *shard, int threshold)
{
    int base;
    int cursor;

    base = shard->meshBase + 4;
    cursor = base + *(int *)shard->meshBase;
    shard->flags = *(int *)cursor & 0xFFFF0000;
    shard->recordIndex = 0;
    cursor = BattleFx_MagicQuake3DrawShardTriangles(shard, (void *)base, cursor, threshold);
    cursor = BattleFx_MagicQuake3DrawShardQuads(shard, (void *)base, cursor, threshold);
    return shard->primitive;
}

/*
 * Renders animated triangle shards for Queig mesh.
 */
// FUNCTION: FF7 0x005839f4
int __cdecl BattleFx_MagicQuake3DrawShardTriangles(BattleFxQuake3Shard *shard, void *vertexBase, int meshData, int threshold)
{
    int shift;
    int colorWord;
    BattleFxTexturedTriRecord *record;
    int fadeStart;
    int stepFactor;
    BattleFxShardTriPrim *primitive;
    int shardIndex;
    int i;
    int fadeVal;
    int numTris;

    numTris = *(int *)meshData & 0xFFFF;
    shardIndex = shard->recordIndex;
    fadeVal = 0;
    shift = 0xE - shard->drawMode;
    colorWord = 0x24808080;
    stepFactor = shard->scale.scaleX;
    fadeStart = shard->fadeStart;
    record = (BattleFxTexturedTriRecord *)(meshData + 4);
    primitive = (BattleFxShardTriPrim *)Gfx_GetPsxPacketScratchPtr();
    for (i = 0; i < numTris; i++, shardIndex++, record++) {
        if (g_BattleMagicQuake3ShardRecords[shardIndex].fadeCounter >= fadeStart) {
            fadeVal = (g_BattleMagicQuake3ShardRecords[shardIndex].fadeCounter - fadeStart) * 0xA0A0A;
            if (fadeVal > 8421504) {
                continue;
            }
        }
        *(Int64Pair *)&shard->velocityX = *(Int64Pair *)&g_BattleMagicQuake3ShardRecords[shardIndex].positionX;
        *(Int64Pair *)&shard->point0X = *(Int64Pair *)((char *)vertexBase + record->vertexOffset0);
        shard->point0X -= shard->velocityX;
        shard->point0Y -= shard->velocityY;
        shard->point0Z -= shard->velocityZ;
        *(Int64Pair *)&shard->point1X = *(Int64Pair *)((char *)vertexBase + record->vertexOffset1);
        shard->point1X -= shard->velocityX;
        shard->point1Y -= shard->velocityY;
        shard->point1Z -= shard->velocityZ;
        *(Int64Pair *)&shard->point2X = *(Int64Pair *)((char *)vertexBase + record->vertexOffset2);
        shard->point2X -= shard->velocityX;
        shard->point2Y -= shard->velocityY;
        shard->point2Z -= shard->velocityZ;
        shard->posX = (shard->velocityX * stepFactor >> 12) + shard->baseX;
        shard->posY = (shard->velocityY * stepFactor >> 12) + shard->baseY;
        shard->posZ = (shard->velocityZ * stepFactor >> 12) + shard->baseZ;
        if (g_BattleMagicQuake3ShardRecords[shardIndex].positionY < threshold) {
            shard->posY += g_BattleMagicQuake3ShardRecords[shardIndex].velocityY;
            if (g_BattleIsPaused == 0) {
                g_BattleMagicQuake3ShardRecords[shardIndex].rotationX += g_BattleMagicQuake3ShardRecords[shardIndex].rotationSpeedX;
                g_BattleMagicQuake3ShardRecords[shardIndex].rotationZ += g_BattleMagicQuake3ShardRecords[shardIndex].rotationSpeedZ;
                g_BattleMagicQuake3ShardRecords[shardIndex].velocityY += g_BattleMagicQuake3ShardRecords[shardIndex].accelerationY;
                (g_BattleMagicQuake3ShardRecords[shardIndex].fadeCounter)++;
            }
        }
        BattleFx_MagicQuake3BuildScaledRotationMatrix((BattleFxEulerAngles16 *)&g_BattleMagicQuake3ShardRecords[shardIndex].rotationX, &shard->rotation, &shard->scale);
        MultiplyMatrix(&shard->cameraMatrix, &shard->rotation, &shard->rotation);
        SetRotMatrix(&shard->rotation);
        SetTransMatrix(&shard->rotation);
        RefreshGlobalTransformState();
        StoreDeferredDrawArgs(&shard->point0X, &shard->point1X, &shard->point2X);
        ApplyDeferredDrawArgs();
        primitive->tag = primitive->tag & 0xFFFFFF | 0x7000000;
        primitive->gpuCode = 0x24;
        if (g_BattleMagicQuake3ShardRecords[shardIndex].fadeCounter < fadeStart) {
            primitive->color = colorWord;
        } else {
            primitive->color = colorWord - fadeVal | 0x26000000;
        }
        primitive->flags = record->uv1 | shard->flags;
        primitive->uv0Clut = record->uv0Clut;
        primitive->uv2 = record->uv2;
        Gfx_PsxStoreFrontFacing(&shard->frontFacing);
        if (shard->frontFacing > 0) {
            Gfx_ReadTransformOrderingDepth2(&shard->orderingDepth);
            Gfx_DrawPsxPrimitive(primitive);
        }
    }
    shard->recordIndex = shardIndex;
    shard->primitive = (int)primitive;
    return (int)record;
}

/*
 * Builds scaled rotation matrix from three Euler angles.
 */
// FUNCTION: FF7 0x00583ee3
void __cdecl BattleFx_MagicQuake3BuildScaledRotationMatrix(BattleFxEulerAngles16 *angles, BattleRotationMatrix *matrix, BattleFxFixedScaleVec *scaleVec)
{
    int cosA;
    int cy;
    int sx;
    int cz;
    int sy;
    int sxScale;
    int sz;
    int zScale;
    int innerScaleY;

    cosA = BattleFx_MagicQuake3LookupCosine(angles->angleX);
    cy = BattleFx_MagicQuake3LookupCosine(angles->angleY);
    cz = BattleFx_MagicQuake3LookupCosine(angles->angleZ);
    sx = BattleFx_MagicQuake3LookupSine(angles->angleX);
    sy = -BattleFx_MagicQuake3LookupSine(angles->angleY);
    sz = BattleFx_MagicQuake3LookupSine(angles->angleZ);
    sxScale = scaleVec->scaleX;
    innerScaleY = scaleVec->scaleY;
    zScale = scaleVec->scaleZ;
    matrix->m00 = (cz * cy >> 12) * sxScale >> 12;
    matrix->m10 = ((sz * cosA >> 12) - ((cz * sy >> 12) * sx >> 12)) * sxScale >> 12;
    matrix->m20 = ((sz * sx >> 12) + ((cz * sy >> 12) * cosA >> 12)) * sxScale >> 12;
    matrix->m01 = (-sz * cy >> 12) * innerScaleY >> 12;
    matrix->m11 = ((cz * cosA >> 12) + ((sz * sy >> 12) * sx >> 12)) * innerScaleY >> 12;
    matrix->m21 = ((cz * sx >> 12) - ((sz * sy >> 12) * cosA >> 12)) * innerScaleY >> 12;
    matrix->m02 = -sy * zScale >> 12;
    matrix->m12 = (-cy * sx >> 12) * zScale >> 12;
    matrix->m22 = (cy * cosA >> 12) * zScale >> 12;
}

/*
 * Sine table lookup helper with symmetry folding.
 */
// FUNCTION: FF7 0x0058409b
int __cdecl BattleFx_MagicQuake3LookupSine(int angle)
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
            local.value = g_BattleMagicQueigSineTable[angle];
        } else {
            local.value = g_BattleMagicQueigSineTable[0x800 - angle];
        }
        local.result = local.value;
    } else {
        if (angle < 0xC00) {
            local.neg = -g_BattleMagicQueigSineTable[angle - 2048];
        } else {
            local.neg = -g_BattleMagicQueigSineTable[0x1000 - angle];
        }
        local.result = local.neg;
    }
    return local.result;
}

/*
 * Cosine table lookup helper with symmetry folding.
 */
// FUNCTION: FF7 0x00584129
int __cdecl BattleFx_MagicQuake3LookupCosine(int angle)
{
    int magnitude;
    int cosine;
    int negValue;

    angle &= 0xFFF;
    if (angle < 0x800) {
        if (angle < 0x400) {
            magnitude = g_BattleMagicQueigSineTable[0x400 - angle];
        } else {
            magnitude = -g_BattleMagicQueigSineTable[angle - 1024];
        }
        cosine = magnitude;
    } else {
        if (angle < 0xC00) {
            negValue = -g_BattleMagicQueigSineTable[0xC00 - angle];
        } else {
            negValue = g_BattleMagicQueigSineTable[angle - 3072];
        }
        cosine = negValue;
    }
    return cosine;
}

/*
 * Renders animated quad shards for Queig mesh.
 */
// FUNCTION: FF7 0x005841b4
int __cdecl BattleFx_MagicQuake3DrawShardQuads(BattleFxQuake3Shard *shard, void *vertexBase, int meshData, int threshold)
{
    int shift;
    int colorWord;
    BattleFxShardQuadSlot *record;
    int fadeStart;
    BattleFxShardQuadPrim *primitive;
    int shardIndex;
    int i;
    int speedScale;
    int fadeVal;
    int numQuads;

    numQuads = *(int *)meshData & 0xFFFF;
    shardIndex = shard->recordIndex;
    fadeVal = 0;
    shift = 0xE - shard->drawMode;
    colorWord = 0x2C808080;
    speedScale = shard->scale.scaleX;
    fadeStart = shard->fadeStart;
    record = (BattleFxShardQuadSlot *)(meshData + 4);
    primitive = (BattleFxShardQuadPrim *)Gfx_GetPsxPacketScratchPtr();
    for (i = 0; i < numQuads; i++, shardIndex++, record++) {
        if (g_BattleMagicQuake3ShardRecords[shardIndex].fadeCounter >= fadeStart) {
            fadeVal = (g_BattleMagicQuake3ShardRecords[shardIndex].fadeCounter - fadeStart) * 0xA0A0A;
            if (fadeVal > 8421504) {
                continue;
            }
        }
        *(Int64Pair *)&shard->velocityX = *(Int64Pair *)&g_BattleMagicQuake3ShardRecords[shardIndex].positionX;
        *(Int64Pair *)&shard->point0X = *(Int64Pair *)((char *)vertexBase + record->vertex0Offset);
        shard->point0X -= shard->velocityX;
        shard->point0Y -= shard->velocityY;
        shard->point0Z -= shard->velocityZ;
        *(Int64Pair *)&shard->point1X = *(Int64Pair *)((char *)vertexBase + record->vertex1Offset);
        shard->point1X -= shard->velocityX;
        shard->point1Y -= shard->velocityY;
        shard->point1Z -= shard->velocityZ;
        *(Int64Pair *)&shard->point2X = *(Int64Pair *)((char *)vertexBase + record->vertex2Offset);
        shard->point2X -= shard->velocityX;
        shard->point2Y -= shard->velocityY;
        shard->point2Z -= shard->velocityZ;
        *(Int64Pair *)&shard->point3X = *(Int64Pair *)((char *)vertexBase + record->vertex3Offset);
        shard->point3X -= shard->velocityX;
        shard->point3Y -= shard->velocityY;
        shard->point3Z -= shard->velocityZ;
        shard->posX = (shard->velocityX * speedScale >> 12) + shard->baseX;
        shard->posY = (shard->velocityY * speedScale >> 12) + shard->baseY;
        shard->posZ = (shard->velocityZ * speedScale >> 12) + shard->baseZ;
        if (g_BattleMagicQuake3ShardRecords[shardIndex].positionY < threshold) {
            shard->posY += g_BattleMagicQuake3ShardRecords[shardIndex].velocityY;
            if (g_BattleIsPaused == 0) {
                g_BattleMagicQuake3ShardRecords[shardIndex].rotationX += g_BattleMagicQuake3ShardRecords[shardIndex].rotationSpeedX;
                g_BattleMagicQuake3ShardRecords[shardIndex].rotationZ += g_BattleMagicQuake3ShardRecords[shardIndex].rotationSpeedZ;
                g_BattleMagicQuake3ShardRecords[shardIndex].velocityY += g_BattleMagicQuake3ShardRecords[shardIndex].accelerationY;
                (g_BattleMagicQuake3ShardRecords[shardIndex].fadeCounter)++;
            }
        }
        BattleFx_MagicQuake3BuildScaledRotationMatrix((BattleFxEulerAngles16 *)&g_BattleMagicQuake3ShardRecords[shardIndex].rotationX, &shard->rotation, &shard->scale);
        MultiplyMatrix(&shard->cameraMatrix, &shard->rotation, &shard->rotation);
        SetRotMatrix(&shard->rotation);
        SetTransMatrix(&shard->rotation);
        RefreshGlobalTransformState();
        StoreDeferredDrawArgs(&shard->point0X, &shard->point1X, &shard->point2X);
        ApplyDeferredDrawArgs();
        primitive->tag = primitive->tag & 0xFFFFFF | 0x9000000;
        primitive->gpuCode = 0x2C;
        if (g_BattleMagicQuake3ShardRecords[shardIndex].fadeCounter < fadeStart) {
            primitive->color = colorWord;
        } else {
            primitive->color = colorWord - fadeVal | 0x2E000000;
        }
        primitive->flags = record->flags | shard->flags;
        primitive->uv0Clut = record->uv0Clut;
        primitive->uv2 = record->uv2;
        primitive->uv3 = record->uv3;
        Gfx_PsxStoreFrontFacing(&shard->frontFacing);
        if (shard->frontFacing > 0) {
            SetPendingResourceHandle((GfxPsxShortVertexSlot *)&shard->point3X);
            ApplyPendingResourceHandle();
            Gfx_ReadTransformOrderingDepth2(&shard->orderingDepth);
            Gfx_DrawPsxPrimitive(primitive);
        }
    }
    shard->recordIndex = shardIndex;
    shard->primitive = (int)primitive;
    return (int)record;
}

/*
 * Updates physics and renders fading quad particles for Queig.
 */
// FUNCTION: FF7 0x00584744
void __cdecl BattleFx_MagicQuake3FadingQuadParticleTask(void)
{
    int color;
    BattleFxDebrisParticle *particle;

    particle = (BattleFxDebrisParticle *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicQuake3SpriteDrawReq.request.fadeLevel = particle->fadeLevel;
    g_BattleMagicQuake3TransformMatrix[0] = particle->heightOffset;
    g_BattleMagicQuake3TransformMatrix[4] = particle->heightOffset;
    BattleFx_MakeViewSpacePointPushed(&particle->posX, -(particle->heightOffset >> 4), (Vec3sPacked *)g_BattleMagicQuake3TransformMatrix);
    SetRotMatrix(g_BattleMagicQuake3TransformMatrix);
    SetTransMatrix(g_BattleMagicQuake3TransformMatrix);
    if (particle->step > 3) {
        color = 0x2E808080;
        color -= (particle->step - 4) * 2105376;
        g_BattleMagicQuake3SpriteDrawReq.request.sourcePtr = color;
    }
    g_BattleMagicQuake3SpriteDrawReq.request.resource = Gfx_ModelTableGetSpriteObject(5, g_BattleMagicQuake3LoadedResTable);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)&g_BattleMagicQuake3SpriteDrawReq.request, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
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
 * Renders ground-aligned textured quad for Queig.
 */
// FUNCTION: FF7 0x0058488b
void __cdecl BattleFx_MagicQuake3GroundQuadTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[116];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; int projectOutB; };
        struct { unsigned char pad_to_C[0xC]; BattleFxQuake3GroundQuadRecord *groundRecord; };
        struct { unsigned char pad_to_10[0x10]; SVECTOR stripDx; };
        struct { unsigned char pad_to_1C[0x1C]; SVECTOR shardCx; };
        struct { unsigned char pad_to_24[0x24]; SVECTOR stripBx; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxGroundTilePacket *groundPacket; };
        struct { unsigned char pad_to_30[0x30]; SVECTOR shardAx; };
        struct { unsigned char pad_to_38[0x38]; PsxMatrix modelMatrix; };
        struct { unsigned char pad_to_58[0x58]; int projectResult; };
        struct { unsigned char pad_to_5C[0x5C]; int projectOutA; };
    } frame;
#pragma pack(pop)

    frame.groundRecord = (BattleFxQuake3GroundQuadRecord *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.modelMatrix);
    frame.modelMatrix.t[0] = frame.groundRecord->posX;
    frame.modelMatrix.t[1] = frame.groundRecord->posY;
    frame.modelMatrix.t[2] = frame.groundRecord->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    RefreshGlobalTransformState();
    frame.groundPacket = (BattleFxGroundTilePacket *)g_BattleFxActiveDrawListPtr;
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
    frame.projectResult = Gfx_ProjectQuadToScreen(&frame.shardAx, &frame.stripBx, &frame.shardCx, &frame.stripDx, &frame.groundPacket->screenXy0, &frame.groundPacket->screenXy1, &frame.groundPacket->screenXy2, &frame.groundPacket->screenXy3, &frame.projectOutA, &frame.projectOutB);
    frame.groundPacket->tag = frame.groundPacket->tag & 0xFFFFFF | 0x9000000;
    frame.groundPacket->gpuCode = 0x2C;
    frame.groundPacket->command = 0x2E808080;
    frame.groundPacket->tpage = 0x2D;
    frame.groundPacket->clut = 0x7A88;
    frame.groundPacket->uv0 = 0x8000;
    frame.groundPacket->uv1 = 0x8080;
    frame.groundPacket->uv2 = 0xFF00;
    frame.groundPacket->uv3 = 0xFF80;
    Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(6, g_BattleMagicQuake3LoadedResTable)));
    Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(7, g_BattleMagicQuake3LoadedResTable)));
    Gfx_DrawPsxPrimitive(frame.groundPacket);
    g_BattleFxActiveDrawListPtr = (int)frame.groundPacket;
    frame.groundRecord->ownerSlot = BATTLE_TASK_FREE;
}

/*
 * Updates and renders pulsing indexed model for Queig.
 */
// FUNCTION: FF7 0x00584a5e
void __cdecl BattleFx_MagicQuake3PulsingIndexedModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxQuake3PulseRecord *pulseRecord; };
        struct { unsigned char pad_to_14[0x14]; int pulseLevel; };
        struct { unsigned char pad_to_18[0x18]; PsxMatrix modelMatrix; };
        struct { unsigned char pad_to_38[0x38]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_3C[0x3C]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.pulseRecord = (BattleFxQuake3PulseRecord *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.pulseRecord->angle + frame.pulseRecord->angleOffset;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.modelMatrix);
    frame.modelMatrix.t[0] = frame.pulseRecord->posX;
    frame.modelMatrix.t[1] = frame.pulseRecord->posY;
    frame.modelMatrix.t[2] = frame.pulseRecord->posZ;
    if (frame.pulseRecord->timer < 7) {
        frame.pulseLevel = frame.pulseRecord->baseLevel - frame.pulseRecord->levelStep * (7 - frame.pulseRecord->timer);
    } else if (frame.pulseRecord->timer < 9) {
        frame.pulseLevel = frame.pulseRecord->baseLevel;
    } else {
        frame.pulseLevel = frame.pulseRecord->baseLevel - frame.pulseRecord->levelStep * (frame.pulseRecord->timer - 9);
    }
    frame.scaleVec.vx = frame.pulseLevel;
    frame.scaleVec.vy = 0x1000 /* 1.0 */;
    frame.scaleVec.vz = frame.pulseRecord->baseLevel;
    ScaleMatrix(&frame.modelMatrix, &frame.scaleVec);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleMagicQuake3ModelIndices[frame.pulseRecord->modelIndex], g_BattleMagicQuake3LoadedResTable);
    frame.modelDraw->flags = 8;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 128;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.pulseRecord->timer >= 16) {
        frame.pulseRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.pulseRecord->angle -= 0x40;
    (frame.pulseRecord->timer)++;
}

/*
 * Timeline task driving single-target Queig effect.
 */
// FUNCTION: FF7 0x00584c4f
void __cdecl BattleFx_MagicQuake3SingleTargetTimelineTask(void)
{
    BattleFxQuake3BurstSprite *pBurst;
    BattleFxQuake3BurstTimeline *task;

    if (g_BattleIsPaused)
        return;
    task = (BattleFxQuake3BurstTimeline *)&g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx];
    /* Numeric burst-delay table index. */
    switch (task->burstIndexA) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        if (task->timer < g_BattleMagicQueigBurstFrameDelays3[task->burstIndexA])
            break;
        pBurst = (BattleFxQuake3BurstSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicQuake3PulsingIndexedModelTask);
        pBurst->clearedWord = 0;
        *(BattleSchedulePair *)&pBurst->posX = *(BattleSchedulePair *)&task->originX;
        pBurst->angle = 0;
        pBurst->scale = g_BattleMagicQueigBurstScales3[task->burstIndexA];
        pBurst->burstSize = ((task->size * g_BattleMagicQueigBurstScaleRates3[task->burstIndexA] >> 12) << 12) / 2500;
        pBurst->burstIndex = task->burstIndexA & 3;
        pBurst->growStep = pBurst->burstSize / 7;
        task->burstIndexA += 1;
        break;
    default:
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (task->timer == 8) {
        pBurst = (BattleFxQuake3BurstSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicQuake3SingleTargetBurstModelTask);
        *(BattleSchedulePair *)&pBurst->posX = *(BattleSchedulePair *)&task->originX;
        pBurst->burstSize = 0;
        pBurst->burstIndex = task->size / 12;
        pBurst->morphWeight = 0;
        pBurst->growStep = 1024;
        BattleFx_MagicQuake3BuildMeshVertexDeltas(g_BattleMagicQuake3MeshPayload.seedVertices, g_BattleMagicQuake3ReferenceVertices, g_BattleMagicQuake3MeshVertexDeltas);
    }
    task->timer += 1;
}

#pragma pack(push, 1)

/*
 * Single-target burst model animation task for Queig.
 */
// FUNCTION: FF7 0x00584e14
void __cdecl BattleFx_MagicQuake3SingleTargetBurstModelTask(void)
{
    struct {
        short rotation[3];
        short pad;
        BattleFxQuake3BurstSprite *slot;
        BattleFxDebrisParticle *output;
        int choice;
        int scaleY;
        BattleFxQuake3Shard *buffer;
        int scale;
        PsxMatrix matrix;
        short pad2;
        BattleFxQuake3MeshDraw *packet;
        int vector[3];
        int unused;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.slot = (BattleFxQuake3BurstSprite *)BATTLE_CURRENT_TASK_SLOT;
    frame.scale = (frame.slot->burstSize << 12) / 1450;
    if (frame.slot->timer < 12)
        frame.scaleY = 16;
    else if (frame.slot->timer < 16)
        frame.scaleY = frame.scale - (frame.scale / 4) * (16 - frame.slot->timer);
    else
        frame.scaleY = frame.scale;
    if (frame.slot->timer < 24) {
        frame.rotation[2] = 0;
        frame.rotation[1] = frame.rotation[2];
        frame.rotation[0] = frame.rotation[1];
        RotMatrixXYZ(frame.rotation, &frame.matrix);
        frame.matrix.t[0] = frame.slot->posX;
        frame.matrix.t[1] = frame.slot->posY;
        frame.matrix.t[2] = frame.slot->posZ;
        frame.vector[0] = frame.scale;
        frame.vector[1] = frame.scaleY;
        frame.vector[2] = frame.scale;
        ScaleMatrix(&frame.matrix, (void *)frame.vector);
        MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
        SetRotMatrix(&frame.matrix);
        SetTransMatrix(&frame.matrix);
        RefreshGlobalTransformState();
        frame.packet = (BattleFxQuake3MeshDraw *)Gfx_GetPsxScratchpadPtr(0);
        frame.packet->meshData = (int)&g_BattleMagicQuake3MeshPayload;
        frame.packet->flags = 0;
        frame.packet->unused08 = 0;
        frame.packet->vertexShade = 0x80;
        frame.packet->attrBits = 0;
        frame.packet->unused0E = 0;
        Gfx_SetPsxPrimQuadState(9, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(8, g_BattleMagicQuake3LoadedResTable)));
        Gfx_SetPsxPrimQuadState(9, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(9, g_BattleMagicQuake3LoadedResTable)));
        Gfx_SetPsxPrimQuadState(11, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(10, g_BattleMagicQuake3LoadedResTable)));
        Gfx_SetPsxPrimQuadState(11, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(11, g_BattleMagicQuake3LoadedResTable)));
        g_BattleFxActiveDrawListPtr = BattleFx_MagicQuake3DrawPsxMesh(frame.packet, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 12, g_BattleFxActiveDrawListPtr);
    } else {
        frame.buffer = (BattleFxQuake3Shard *)Gfx_GetPsxScratchpadPtr(0);
        frame.buffer->meshBase = (int)&g_BattleMagicQuake3MeshPayload;
        frame.buffer->orderingTable = g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED;
        frame.buffer->drawMode = 12;
        frame.buffer->primitive = g_BattleFxActiveDrawListPtr;
        frame.buffer->fadeStart = 3;
        frame.buffer->cameraMatrix = *(PsxMatrix *)(int *)&g_BattleCameraTransform;
        frame.buffer->scale.scaleZ = frame.scale;
        frame.buffer->scale.scaleY = frame.scale;
        frame.buffer->scale.scaleX = frame.scale;
        *(BattleSchedulePair *)&frame.buffer->baseX = *(BattleSchedulePair *)&frame.slot->posX;
        g_BattleFxActiveDrawListPtr = BattleFx_MagicQuake3DrawShardMesh(frame.buffer, frame.slot->timer - 24);
    }
    if (frame.slot->timer >= 43)
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    if (g_BattleIsPaused != 0)
        return;
    if (frame.slot->timer == 24) {
        for (frame.scale = 0; frame.scale < 10; frame.scale++) {
            if (g_BattleMagicQuake3TargetMask & (1 << frame.scale))
                Battle_PostActorHitOrHurtMessage(frame.scale);
        }
    }
    if (frame.slot->timer < 12) {
        frame.slot->burstSize += frame.slot->burstIndex;
        frame.choice = rand() % 4;
        for (frame.scaleY = 0; frame.scaleY < 3; frame.scaleY++) {
            frame.output = (BattleFxDebrisParticle *)&g_BattleTaskSlotBuffer.mainSlots[Battle_AllocateResourceSlot(BattleFx_MagicQuake3FadingQuadParticleTask)];
            frame.scale = frame.slot->burstSize >> 1;
            frame.output->posX = frame.slot->posX + rand() % (frame.scale * 2) - frame.scale;
            frame.output->position = frame.slot->posY - rand() % (frame.scale >> 3);
            frame.output->posZ = frame.slot->posZ + rand() % (frame.scale * 2) - frame.scale;
            frame.output->fadeWord = ((frame.choice + frame.scaleY) & 3) << 5;
            frame.output->heightOffset = (rand() % 4) * 1365 + 1024;
            frame.output->velocity = frame.output->fadeWord + frame.slot->timer * 2 - 384;
            frame.output->acceleration = rand() % 12 + frame.slot->timer + 12;
            frame.output->fadeWord += 0x80;
        }
    } else if (frame.slot->timer >= 15 && frame.slot->timer < 20) {
        BattleFx_MagicQuake3CopyMeshVertices(g_BattleMagicQuake3MeshPayload.seedVertices);
        BattleFx_MagicQuake3AddScaledMeshVertexDeltas(g_BattleMagicQuake3MeshPayload.vertices, g_BattleMagicQuake3MeshVertexDeltas, 97, frame.slot->morphWeight);
        frame.slot->morphWeight += frame.slot->growStep;
        if (frame.slot->morphWeight > 4096)
            frame.slot->morphWeight = 0x1000;
    }
    ++frame.slot->timer;
}
} /* extern "C" */
