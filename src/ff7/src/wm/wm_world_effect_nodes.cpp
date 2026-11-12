// src/ff7/src/wm/wm_world_effect_nodes.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Holds the world map's effect node pool and its per-frame spawn, update and draw code
//   for terrain and actor effects and shadow quads. It also covers the world music track switching
//   and the sound-effect helpers that go with the world map.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/enums/wm.h"
#include <string.h>
#include "ff7/world.h"
#include "ff7/types/wm.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "lib/api/polygon.h"
#include "ff7/api/wm.h"
#include "lib/api/graphics.h"
#include "lib/api/sound.h"
#include "lib/api/token.h"
#include "lib/codegen.h"
#include "lib/enums/graphics.h"
#include "lib/enums/sound.h"
extern "C" {

#pragma intrinsic(strcpy, memcpy)

struct GfxFloatColorAddend;
struct PolygonModelVertexColors;

typedef short WorldEffectMatrix[16];

// clang-format off
// GLOBAL: FF7 0x0096d598
unsigned char g_WorldMusicTrackRecord0[8] = {0x0d, 0x33, 0x2f, 0x07, 0x2c, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0096d5a0
unsigned char g_WorldMusicTrackRecord1[8] = {0x47, 0x33, 0x2f, 0x07, 0x2c, 0x46, 0x48, 0x00};
// GLOBAL: FF7 0x0096d5a8
int g_WorldMusicTrackRecord2 = 0x07000055;
// GLOBAL: FF7 0x0096d5b0
unsigned char g_WorldEffectConfig[12] = {0x4d, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};
unsigned char g_rawData_0096d5bc[4] = {0x05, 0x01, 0x00, 0x00};
unsigned char g_rawData_0096d5c0[20] = {0x0a, 0x01, 0x00, 0x00, 0x0b, 0x01, 0x00, 0x00, 0x0c, 0x01, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0096d5d4
unsigned char g_WorldEffectRecordIndex[4] = {0x13, 0x01, 0x00, 0x00};
unsigned char g_rawData_0096d5d8[196] = {
    0x19, 0x00, 0x00, 0x00, 0x1e, 0x01, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x27, 0x01, 0x00, 0x00, 0x2a, 0x01, 0x00, 0x00, 0x2d, 0x00, 0x00, 0x00,
    0x31, 0x00, 0x00, 0x00, 0x35, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00,
    0x44, 0x00, 0x00, 0x00, 0x4a, 0x00, 0x00, 0x00, 0x4e, 0x00, 0x00, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x52, 0x01, 0x00, 0x00, 0x56, 0x01, 0x00, 0x00,
    0x5a, 0x01, 0x00, 0x00, 0x5e, 0x01, 0x00, 0x00, 0x62, 0x01, 0x00, 0x00, 0x63, 0x01, 0x00, 0x00, 0x64, 0x01, 0x00, 0x00, 0x65, 0x01, 0x00, 0x00,
    0x66, 0x01, 0x00, 0x00, 0x67, 0x01, 0x00, 0x00, 0x68, 0x01, 0x00, 0x00, 0x69, 0x01, 0x00, 0x00, 0x6a, 0x00, 0x00, 0x00, 0x6b, 0x00, 0x00, 0x00,
    0x6c, 0x00, 0x00, 0x00, 0x6d, 0x00, 0x00, 0x00, 0x6e, 0x00, 0x00, 0x00, 0x6f, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x71, 0x00, 0x00, 0x00,
    0x72, 0x00, 0x00, 0x00, 0x73, 0x01, 0x00, 0x00, 0x74, 0x01, 0x00, 0x00, 0x75, 0x01, 0x00, 0x00, 0x76, 0x01, 0x00, 0x00, 0x77, 0x00, 0x00, 0x00,
    0x7b, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x7d, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
    0x81, 0x00, 0x00, 0x00
};
// Record index read as a byte by World_SpawnActorEffectsByKind (WORLD_ACTOR_EFFECT_PROPELLER_A).
// GLOBAL: FF7 0x0096d69c
int g_WorldMiniMapRecordIndex = 132;
unsigned char g_rawData_0096d6a0[8] = {0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// The 12-byte WorldEffectRecord entries, indexed by a node's recordIndex; the table runs to the next
// named definition.
// GLOBAL: FF7 0x0096d6a8
unsigned char g_WorldEffectRecordTable[1744] = {0x81, 0x01, 0x20, 0x20, 0x00, 0x20, 0x0f, 0x2f, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x20, 0x20, 0x10, 0x20, 0x1f, 0x2f, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x20, 0x20, 0x20, 0x20, 0x2f, 0x2f, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x20, 0x20, 0x30, 0x20, 0x3f, 0x2f, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x20, 0x20, 0x40, 0x20, 0x4f, 0x2f, 0x00, 0x00, 0x00, 0x00, 0x81, 0x01, 0x20, 0x20, 0x0f, 0x20, 0x00, 0x2f, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x20, 0x20, 0x1f, 0x20, 0x10, 0x2f, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x20, 0x20, 0x2f, 0x20, 0x20, 0x2f, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x20, 0x20, 0x3f, 0x20, 0x30, 0x2f, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x20, 0x20, 0x4f, 0x20, 0x40, 0x2f, 0x00, 0x00, 0x00, 0x00, 0xff, 0x02, 0x08, 0x08, 0x38, 0x00, 0x3f, 0x0f, 0x01, 0x00, 0x00, 0x00, 0xff, 0x02, 0x08, 0x08, 0x30, 0x00, 0x37, 0x0f, 0x01, 0x00, 0x00, 0x00, 0xff, 0x02, 0x10, 0x10, 0x00, 0x00, 0x0f, 0x0f, 0x02, 0x00, 0x00, 0x00, 0x81, 0x01, 0x20, 0x20, 0x00, 0x30, 0x0f, 0x3f, 0x03, 0x00, 0x00, 0x00, 0x01, 0x01, 0x20, 0x20, 0x10, 0x30, 0x1f, 0x3f, 0x03, 0x00, 0x00, 0x00, 0x01, 0x01, 0x20, 0x20, 0x20, 0x30, 0x2f, 0x3f, 0x03, 0x00, 0x00, 0x00, 0x82, 0x01, 0x20, 0x20, 0x00, 0x10, 0x0f, 0x1f, 0x04, 0x00, 0x00, 0x00, 0x02, 0x01, 0x20, 0x20, 0x10, 0x10, 0x1f, 0x1f, 0x04, 0x00, 0x00, 0x00, 0x03, 0x01, 0x20, 0x20, 0x20, 0x10, 0x2f, 0x1f, 0x04, 0x00, 0x00, 0x00, 0x8a, 0x05, 0x00, 0x00, 0xc0, 0x00, 0xff, 0x3f, 0x19, 0x00, 0x00, 0x00, 0x83, 0x00, 0x20, 0x20, 0x80, 0x80, 0x9f, 0x9f, 0x05, 0x00, 0x00, 0x00, 0x03, 0x00, 0x20, 0x20, 0xa0, 0x80, 0xbf, 0x9f, 0x05, 0x00, 0x00, 0x00, 0x03, 0x00, 0x20, 0x20, 0x80, 0xa0, 0x9f, 0xbf, 0x05, 0x00, 0x00, 0x00, 0x03, 0x00, 0x20, 0x20, 0xa0, 0xa0, 0xbf, 0xbf, 0x05, 0x00, 0x00, 0x00, 0x03, 0x00, 0x20, 0x20, 0xa0, 0xc0, 0xbf, 0xdf, 0x05, 0x00, 0x00, 0x00, 0x81, 0x01, 0x10, 0x10, 0x00, 0x40, 0x07, 0x47, 0x06, 0x00, 0x00, 0x00, 0x01, 0x01, 0x10, 0x10, 0x08, 0x40, 0x0f, 0x47, 0x06, 0x00, 0x00, 0x00, 0x01, 0x01, 0x10, 0x10, 0x10, 0x40, 0x17, 0x47, 0x06, 0x00, 0x00, 0x00, 0x02, 0x01, 0x10, 0x10, 0x18, 0x40, 0x1f, 0x47, 0x06, 0x00, 0x00, 0x00, 0x02, 0x01, 0x10, 0x10, 0x20, 0x40, 0x27, 0x47, 0x06, 0x00, 0x00, 0x00, 0x81, 0x00, 0x20, 0x20, 0x00, 0x58, 0x0f, 0x67, 0x07, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0x20, 0x10, 0x58, 0x1f, 0x67, 0x07, 0x00, 0x00, 0x00, 0x02, 0x00, 0x20, 0x20, 0x20, 0x58, 0x2f, 0x67, 0x07, 0x00, 0x00, 0x00, 0x02, 0x00, 0x20, 0x20, 0x30, 0x58, 0x3f, 0x67, 0x07, 0x00, 0x00, 0x00, 0x81, 0x00, 0x18, 0x18, 0x00, 0x68, 0x0f, 0x77, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x18, 0x10, 0x68, 0x1f, 0x7f, 0x08, 0x00, 0x00, 0x00, 0x02, 0x00, 0x18, 0x18, 0x20, 0x68, 0x2f, 0x77, 0x08, 0x00, 0x00, 0x00, 0x02, 0x00, 0x18, 0x18, 0x30, 0x68, 0x3f, 0x77, 0x08, 0x00, 0x00, 0x00, 0x02, 0x00, 0x18, 0x18, 0x40, 0x68, 0x4f, 0x77, 0x08, 0x00, 0x00, 0x00, 0x82, 0x00, 0x20, 0x20, 0x00, 0x90, 0x17, 0xa7, 0x09, 0x00, 0x00, 0x00, 0x03, 0x00, 0x20, 0x20, 0x18, 0x90, 0x2f, 0xa7, 0x09, 0x00, 0x00, 0x00, 0x04, 0x00, 0x20, 0x20, 0x30, 0x90, 0x47, 0xa7, 0x09, 0x00, 0x00, 0x00, 0x82, 0x00, 0x18, 0x18, 0x00, 0x80, 0x0f, 0x8f, 0x09, 0x00, 0x00, 0x00, 0x02, 0x00, 0x18, 0x18, 0x10, 0x80, 0x1f, 0x8f, 0x09, 0x00, 0x00, 0x00, 0x02, 0x00, 0x18, 0x18, 0x20, 0x80, 0x2f, 0x8f, 0x09, 0x00, 0x00, 0x00, 0x82, 0x00, 0x20, 0x20, 0x00, 0xa8, 0x17, 0xc7, 0x0a, 0x00, 0x00, 0x00, 0x02, 0x00, 0x20, 0x20, 0x18, 0xa8, 0x2f, 0xc7, 0x0a, 0x00, 0x00, 0x00, 0x02, 0x00, 0x20, 0x20, 0x30, 0xa8, 0x47, 0xc7, 0x0a, 0x00, 0x00, 0x00, 0x02, 0x00, 0x20, 0x20, 0x48, 0xa8, 0x5f, 0xc7, 0x0a, 0x00, 0x00, 0x00, 0x82, 0x00, 0x30, 0x30, 0x00, 0xc8, 0x17, 0xdf, 0x0b, 0x00, 0x00, 0x00, 0x02, 0x00, 0x30, 0x30, 0x18, 0xc8, 0x2f, 0xdf, 0x0b, 0x00, 0x00, 0x00, 0x02, 0x00, 0x30, 0x30, 0x30, 0xc8, 0x47, 0xdf, 0x0b, 0x00, 0x00, 0x00, 0x02, 0x00, 0x30, 0x30, 0x48, 0xc8, 0x5f, 0xdf, 0x0b, 0x00, 0x00, 0x00, 0x82, 0x00, 0x20, 0x20, 0x60, 0xe0, 0x77, 0xf7, 0x0c, 0x00, 0x00, 0x00, 0x02, 0x00, 0x20, 0x20, 0x78, 0xe0, 0x8f, 0xf7, 0x0c, 0x00, 0x00, 0x00, 0x02, 0x00, 0x20, 0x20, 0x90, 0xe0, 0xa7, 0xf7, 0x0c, 0x00, 0x00, 0x00, 0x82, 0x00, 0x20, 0x20, 0x00, 0xe0, 0x17, 0xf7, 0x0d, 0x00, 0x00, 0x00, 0x02, 0x00, 0x20, 0x20, 0x18, 0xe0, 0x2f, 0xf7, 0x0d, 0x00, 0x00, 0x00, 0x02, 0x00, 0x20, 0x20, 0x30, 0xe0, 0x47, 0xf7, 0x0d, 0x00, 0x00, 0x00, 0x02, 0x00, 0x20, 0x20, 0x48, 0xe0, 0x5f, 0xf7, 0x0d, 0x00, 0x00, 0x00, 0x82, 0x00, 0x18, 0x18, 0x40, 0x00, 0x4f, 0x0f, 0x0e, 0x00, 0x00, 0x00, 0x02, 0x00, 0x18, 0x18, 0x50, 0x00, 0x5f, 0x0f, 0x0e, 0x00, 0x00, 0x00, 0x02, 0x00, 0x18, 0x18, 0x60, 0x00, 0x6f, 0x0f, 0x0e, 0x00, 0x00, 0x00, 0x02, 0x00, 0x18, 0x18, 0x70, 0x00, 0x7f, 0x0f, 0x0e, 0x00, 0x00, 0x00, 0x82, 0x00, 0x18, 0x18, 0x80, 0x00, 0x8f, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x02, 0x00, 0x18, 0x18, 0x90, 0x00, 0x9f, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x02, 0x00, 0x18, 0x18, 0xa0, 0x00, 0xaf, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x02, 0x00, 0x18, 0x18, 0xb0, 0x00, 0xbf, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x81, 0x00, 0x20, 0x20, 0x30, 0x10, 0x3f, 0x1f, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0x20, 0x40, 0x10, 0x4f, 0x1f, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0x20, 0x50, 0x10, 0x5f, 0x1f, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0x20, 0x60, 0x10, 0x6f, 0x1f, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0x20, 0x70, 0x10, 0x7f, 0x1f, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0x20, 0x80, 0x10, 0x8f, 0x1f, 0x10, 0x00, 0x00, 0x00, 0x81, 0x00, 0x20, 0x20, 0x90, 0x10, 0x9f, 0x1f, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0x20, 0xa0, 0x10, 0xaf, 0x1f, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0x20, 0xb0, 0x10, 0xbf, 0x1f, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0x20, 0xa0, 0x10, 0xaf, 0x1f, 0x10, 0x00, 0x00, 0x00, 0x81, 0x00, 0x20, 0x20, 0x90, 0x10, 0x9f, 0x1f, 0x10, 0x00, 0x00, 0x00, 0x83, 0x00, 0x20, 0x20, 0x50, 0x20, 0x5f, 0x2f, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x20, 0x20, 0x60, 0x20, 0x6f, 0x2f, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x20, 0x20, 0x70, 0x20, 0x7f, 0x2f, 0x10, 0x00, 0x00, 0x00, 0x85, 0x00, 0x80, 0x80, 0xc0, 0x40, 0xe0, 0x60, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x80, 0x80, 0xc0, 0x80, 0xe0, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x80, 0x80, 0xc0, 0xc0, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x80, 0x80, 0x80, 0x40, 0xa0, 0x60, 0x00, 0x00, 0x00, 0x00, 0x85, 0x00, 0x80, 0x80, 0xe0, 0x40, 0xff, 0x60, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x80, 0x80, 0xe0, 0x80, 0xff, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x80, 0x80, 0xe0, 0xc0, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x80, 0x80, 0xa0, 0x40, 0xbf, 0x60, 0x00, 0x00, 0x00, 0x00, 0x85, 0x00, 0x80, 0x80, 0xc0, 0x60, 0xe0, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x80, 0x80, 0xc0, 0xa0, 0xe0, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x80, 0x80, 0xc0, 0xe0, 0xe0, 0xff, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x80, 0x80, 0x80, 0x60, 0xa0, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x85, 0x00, 0x80, 0x80, 0xe0, 0x60, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x80, 0x80, 0xe0, 0xa0, 0xff, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x80, 0x80, 0xe0, 0xe0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x80, 0x80, 0xa0, 0x60, 0xbf, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x9e, 0x09, 0x40, 0x20, 0x40, 0x80, 0x57, 0x8f, 0x11, 0x00, 0x00, 0x00, 0x9e, 0x09, 0x40, 0x40, 0x5f, 0x90, 0x48, 0xa7, 0x11, 0x00, 0x00, 0x00, 0x9e, 0x09, 0x20, 0x40, 0x3f, 0x40, 0x30, 0x57, 0x11, 0x00, 0x00, 0x00, 0x9e, 0x09, 0x40, 0x40, 0x5f, 0xa7, 0x48, 0x90, 0x11, 0x00, 0x00, 0x00, 0x9e, 0x09, 0x40, 0x20, 0x40, 0x8f, 0x57, 0x80, 0x11, 0x00, 0x00, 0x00, 0x9e, 0x09, 0x40, 0x40, 0x48, 0xa7, 0x5f, 0x90, 0x11, 0x00, 0x00, 0x00, 0x9e, 0x09, 0x20, 0x40, 0x30, 0x40, 0x3f, 0x57, 0x11, 0x00, 0x00, 0x00, 0x9e, 0x09, 0x40, 0x40, 0x48, 0x90, 0x5f, 0xa7, 0x11, 0x00, 0x00, 0x00, 0x81, 0x01, 0x10, 0x10, 0x30, 0x30, 0x3f, 0x3f, 0x18, 0x00, 0x00, 0x00, 0x81, 0x01, 0x10, 0x10, 0x40, 0x30, 0x4f, 0x3f, 0x18, 0x00, 0x00, 0x00, 0x81, 0x01, 0x10, 0x10, 0x50, 0x30, 0x5f, 0x3f, 0x18, 0x00, 0x00, 0x00, 0x81, 0x01, 0x10, 0x10, 0x3f, 0x30, 0x30, 0x3f, 0x18, 0x00, 0x00, 0x00, 0x81, 0x01, 0x10, 0x10, 0x4f, 0x30, 0x40, 0x3f, 0x18, 0x00, 0x00, 0x00, 0x81, 0x01, 0x10, 0x10, 0x5f, 0x30, 0x50, 0x3f, 0x18, 0x00, 0x00, 0x00, 0x81, 0x01, 0x10, 0x10, 0x60, 0x30, 0x6f, 0x3f, 0x18, 0x00, 0x00, 0x00, 0x81, 0x01, 0x10, 0x10, 0x70, 0x30, 0x7f, 0x3f, 0x18, 0x00, 0x00, 0x00, 0x81, 0x01, 0x10, 0x10, 0x80, 0x30, 0x8f, 0x3f, 0x18, 0x00, 0x00, 0x00, 0x82, 0x00, 0x18, 0x18, 0x80, 0x20, 0x8f, 0x2f, 0x18, 0x00, 0x00, 0x00, 0x82, 0x00, 0x18, 0x18, 0x90, 0x20, 0x9f, 0x2f, 0x18, 0x00, 0x00, 0x00, 0x82, 0x00, 0x18, 0x18, 0xa0, 0x20, 0xaf, 0x2f, 0x18, 0x00, 0x00, 0x00, 0x82, 0x00, 0x18, 0x18, 0xb0, 0x20, 0xbf, 0x2f, 0x18, 0x00, 0x00, 0x00, 0x84, 0x00, 0x30, 0x30, 0x40, 0x40, 0x57, 0x57, 0x13, 0x00, 0x00, 0x00, 0x04, 0x00, 0x30, 0x30, 0x58, 0x40, 0x6f, 0x57, 0x13, 0x00, 0x00, 0x00, 0x04, 0x14, 0x30, 0x40, 0x70, 0x40, 0x8f, 0x5f, 0x13, 0x00, 0x00, 0x00, 0x04, 0x14, 0x30, 0x40, 0x90, 0x40, 0xaf, 0x5f, 0x13, 0x00, 0x00, 0x00, 0x81, 0x02, 0x14, 0x14, 0x90, 0x30, 0x9f, 0x3f, 0x14, 0x00, 0x00, 0x00, 0x81, 0x02, 0x14, 0x14, 0xa0, 0x30, 0xaf, 0x3f, 0x14, 0x00, 0x00, 0x00, 0x81, 0x02, 0x14, 0x14, 0xb0, 0x30, 0xbf, 0x3f, 0x14, 0x00, 0x00, 0x00, 0x81, 0x02, 0x14, 0x14, 0xaf, 0x30, 0xa0, 0x3f, 0x14, 0x00, 0x00, 0x00, 0x81, 0x02, 0x14, 0x14, 0x9f, 0x30, 0x90, 0x3f, 0x14, 0x00, 0x00, 0x00, 0x81, 0x02, 0x14, 0x14, 0xb0, 0x40, 0xbf, 0x4f, 0x14, 0x00, 0x00, 0x00, 0x94, 0x01, 0x30, 0x30, 0xc0, 0x40, 0xdf, 0x5f, 0x15, 0x00, 0x00, 0x00, 0x14, 0x01, 0x30, 0x30, 0xe0, 0x40, 0xff, 0x5f, 0x15, 0x00, 0x00, 0x00, 0x28, 0x11, 0x30, 0x30, 0xc0, 0x60, 0xdf, 0x7f, 0x15, 0x00, 0x00, 0x00, 0x81, 0x19, 0x10, 0x10, 0x70, 0x60, 0x8f, 0x7f, 0x16, 0x00, 0x00, 0x00, 0x01, 0x19, 0x10, 0x10, 0x90, 0x60, 0xaf, 0x7f, 0x16, 0x00, 0x00, 0x00, 0x01, 0x19, 0x10, 0x10, 0x70, 0x80, 0x8f, 0x9f, 0x16, 0x00, 0x00, 0x00, 0x01, 0x19, 0x10, 0x10, 0x90, 0x80, 0xaf, 0x9f, 0x16, 0x00, 0x00, 0x00, 0x82, 0x19, 0x20, 0x20, 0xe0, 0xe0, 0xff, 0xff, 0x17, 0x00, 0x00, 0x00, 0x02, 0x19, 0x20, 0x20, 0xc0, 0xe0, 0xdf, 0xff, 0x17, 0x00, 0x00, 0x00, 0x02, 0x19, 0x20, 0x20, 0xe0, 0xc0, 0xff, 0xdf, 0x17, 0x00, 0x00, 0x00, 0x02, 0x19, 0x20, 0x20, 0xc0, 0xc0, 0xdf, 0xdf, 0x17, 0x00, 0x00, 0x00, 0x02, 0x19, 0x20, 0x20, 0xe0, 0xa0, 0xff, 0xbf, 0x17, 0x00, 0x00, 0x00, 0x02, 0x19, 0x20, 0x20, 0xc0, 0xa0, 0xdf, 0xbf, 0x17, 0x00, 0x00, 0x00, 0x02, 0x19, 0x20, 0x20, 0xe0, 0x80, 0xff, 0x9f, 0x17, 0x00, 0x00, 0x00, 0x02, 0x19, 0x20, 0x20, 0xc0, 0x80, 0xdf, 0x9f, 0x17, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0096dd78
short g_WorldModelTintTable[7][4] = {
    {50, 50, -30, 0},
    {-30, 50, -30, 0},
    {-30, 50, 50, 0},
    {-80, -80, -80, 0},
    {69, 17, -147, 0},
    {0, -255, -255, 0},
    {-17, -15, 10, 0}
};
// GLOBAL: FF7 0x00e2d608
unsigned char *g_pWorldMusicTrackTable = 0;
// GLOBAL: FF7 0x00e2d60c
WorldEffectNode *g_pWorldEffectRetiredLast = 0;
unsigned char g_bssPad_00e2d610[32768] = {0};
// GLOBAL: FF7 0x00e35610
int g_WorldEffectState = 0;
unsigned char g_bssPad_00e35614[4] = {0};
unsigned char g_bssPad_00e35618[32] = {0};
// GLOBAL: FF7 0x00e35638
unsigned char g_WorldEffectEmitterTable[2][4] = {0};
// GLOBAL: FF7 0x00e35640
WorldEffectNode *g_pWorldEffectNodeFreeList = 0;
unsigned char g_bssPad_00e35644[4] = {0};
// GLOBAL: FF7 0x00e35648
WorldEffectNode *g_pWorldEffectActiveList = 0;
// GLOBAL: FF7 0x00e3564c
int g_WorldPrevMusicTrack = 0;
// GLOBAL: FF7 0x00e35650
WorldEffectNode *g_pWorldEffectRetiredList = 0;
// GLOBAL: FF7 0x00e35654
int g_WorldMusicChangeGate = 0;
// GLOBAL: FF7 0x00e35658
int g_WorldMusicTrack = 0;
unsigned char g_bssPad_00e3565c[4] = {0};
// GLOBAL: FF7 0x00e35660
unsigned char g_WorldEffectNodePool[0x20 * 0x54] = {0};
unsigned char g_bssPad_00e360e0[12] = {0};
// GLOBAL: FF7 0x00e360ec
int g_WorldAmbienceId = 0;
// clang-format on

/*
 * Thread the world's thirty-two effect nodes into a free list and empty the
 * active and retired lists. The last node's link is written after the loop.
 */
// FUNCTION: FF7 0x0075ac80
void __cdecl World_InitEffectNodePool(void)
{
    int i;

    for (i = 0; i < 31; i++) {
        ((WorldEffectNode *)g_WorldEffectNodePool)[i].nextNode =
            &((WorldEffectNode *)g_WorldEffectNodePool)[i + 1];
    }
    ((WorldEffectNode *)g_WorldEffectNodePool)[i].nextNode = 0;
    g_pWorldEffectNodeFreeList = (WorldEffectNode *)g_WorldEffectNodePool;
    g_pWorldEffectRetiredLast = 0;
    g_pWorldEffectRetiredList = g_pWorldEffectRetiredLast;
    g_pWorldEffectActiveList = g_pWorldEffectRetiredList;
    g_WorldEffectState = 0;
    for (i = 0; i < 2; i++) {
        g_WorldEffectEmitterTable[i][2] = 0;
    }
}

/*
 * Selects world-actor terrain effects from low-five-bit walkmesh terrain,
 * actor type/masks, animation frame and active state. Resets shared effect
 * origin to altPos and velocity/offset globals before ordered cases.
 */
// FUNCTION: FF7 0x0075ad28
void __cdecl WorldEntity_SpawnTerrainEffects(WorldActor *actor, int active)
{
    struct {
        WorldEffectMatrix matrix;
        VECTOR transformed;
        short sprite, pad;
        short angles[4];
        int phase6, phase3, angle, tilt, direction, terrain, flags;
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    s.terrain = actor->walkmesh & 31;
    g_WorldEffectAngle = s.direction = actor->heading + actor->headingOffset;
    g_WorldEffectOrigin = actor->altPos;
    g_WorldEffectOffset.x = g_WorldEffectOffset.y = g_WorldEffectOffsetZ = 0;
    g_WorldEffectVelocity.x = g_WorldEffectVelocity.y = g_WorldEffectVelocityZ = 0;
    if (actor->actorType == WORLD_MODEL_HIGHWIND) {
        if (active && World_GetStageVariantFlag() && World_GetCameraBlendDirection() >= 0 &&
            (actor->pos.vx - actor->altPos.vx > 0 ? actor->pos.vx - actor->altPos.vx : -(actor->pos.vx - actor->altPos.vx)) +
                    (actor->pos.vz - actor->altPos.vz > 0 ? actor->pos.vz - actor->altPos.vz : -(actor->pos.vz - actor->altPos.vz)) >
                100) {
            s.phase3 = (unsigned int)g_WorldEffectState % 3;
            s.phase6 = (unsigned int)g_WorldEffectState % 6;
            s.tilt = World_GetCameraRoll();
            s.angle = s.tilt * 4 + s.direction;
            g_WorldEffectOrigin = actor->pos;
            g_WorldEffectVelocity.x = 300;
            g_WorldEffectVelocity.y = 400 - (0 > s.tilt ? 0 : s.tilt);
            g_WorldEffectVelocityZ = -90;
            World_SpawnEffectNode(s.angle, 0, (s.tilt * 4 < 200 ? 36 : 39) + s.phase3, 0);
            World_SpawnEffectNode(s.angle, 0, s.phase6 + 50, 0);
            g_WorldEffectVelocity.x = -300;
            g_WorldEffectVelocity.y = 400 + (0 < s.tilt ? 0 : s.tilt);
            World_SpawnEffectNode(s.angle, 0, (s.tilt * 4 > -200 ? 39 : 36) + s.phase3, 0);
            World_SpawnEffectNode(s.angle, 0, s.phase6 + 50, 0);
            g_WorldEffectVelocity.x = 0;
            g_WorldEffectVelocity.y = 300;
            g_WorldEffectVelocityZ = -300;
            World_SpawnEffectNode(s.angle, 0, s.phase3 + 42, 0);
            World_SpawnEffectNode(s.angle, 0, s.phase6 + 50, 0);
        }
        if (s.terrain >= WORLD_TERRAIN_SEA && s.terrain <= WORLD_TERRAIN_SWAMP && actor->pos.vy < actor->groundY + 1000 && (*(unsigned char *)&actor->animFrame % 3) == 0) {
            g_WorldEffectVelocity.x = g_WorldEffectVelocity.y = g_WorldEffectVelocityZ = 0;
            g_WorldEffectOffset.x = (actor->pos.vx - actor->altPos.vx) >> 1;
            g_WorldEffectOffset.y = 50;
            g_WorldEffectOffsetZ = (actor->pos.vz - actor->altPos.vz) >> 1;
            if ((g_WorldEffectOffset.x > 0 ? g_WorldEffectOffset.x : -g_WorldEffectOffset.x) + (g_WorldEffectOffsetZ > 0 ? g_WorldEffectOffsetZ : -g_WorldEffectOffsetZ) > 25) {
                g_WorldEffectOrigin.vx -= g_WorldEffectOffset.x * 4;
                g_WorldEffectOrigin.vy = actor->groundY;
                g_WorldEffectOrigin.vz -= g_WorldEffectOffsetZ * 4;
                World_SpawnEffectNode(0, 0x700, 0, 0);
                World_SpawnEffectNode(0, 0x900, 1, 0);
            }
        }
        return;
    }
    /* Retail caller prototype (unsigned char) -> int preserved for /Od argument promotion on `World_IsRideableModel`. */
    if (active && ((int(__cdecl *)(unsigned char))World_IsRideableModel)(actor->actorType) && (*(unsigned char *)&actor->animFrame == 2 || *(unsigned char *)&actor->animFrame == 14) && ((WORLD_TERRAIN_MASK_DUST_STEP >> s.terrain) & 1)) {
        g_WorldEffectOffset.x = actor->pos.vx - actor->altPos.vx;
        g_WorldEffectOffset.y = 100;
        g_WorldEffectOffsetZ = actor->pos.vz - actor->altPos.vz;
        g_WorldEffectOrigin.vx -= g_WorldEffectOffset.x;
        g_WorldEffectOrigin.vy += 100;
        g_WorldEffectOrigin.vz -= g_WorldEffectOffsetZ;
        World_SpawnEffectNode(0, 0x800, (s.terrain != WORLD_TERRAIN_SNOW) + 5, 50);
        return;
    }
    if (s.terrain == WORLD_TERRAIN_SNOW && World_GetMapVariant() != 3 && active) {
        /* Retail caller prototype (int, unsigned char) -> int preserved for /Od argument promotion on `World_IsModelInMask`. */
        if (((int(__cdecl *)(int, unsigned char))World_IsModelInMask)(7, actor->actorType) && (*(unsigned char *)&actor->animFrame == 1 || *(unsigned char *)&actor->animFrame == 8)) {
            g_WorldEffectVelocityZ = 40;
            World_SpawnEffectNode(((*(unsigned char *)&actor->animFrame >> 3) ? 0x400 : 0xc00) + s.direction, 0, (*(unsigned char *)&actor->animFrame >> 3) ? 2 : 3, 0);
        /* Retail caller prototype (unsigned char) -> int preserved for /Od argument promotion on `World_IsRideableModel`. */
        } else if (((int(__cdecl *)(unsigned char))World_IsRideableModel)(actor->actorType) && (*(unsigned char *)&actor->animFrame == 1 || *(unsigned char *)&actor->animFrame == 13)) {
            g_WorldEffectVelocityZ = 120;
            World_SpawnEffectNode(((*(unsigned char *)&actor->animFrame >> 3) ? 0xc00 : 0x400) + s.direction, 0, 4, 0);
        }
        return;
    }
    /* Retail caller prototype (int, unsigned char) -> int preserved for /Od argument promotion on `World_IsModelInMask`. */
    if (((int(__cdecl *)(int, unsigned char))World_IsModelInMask)(7, actor->actorType) && ((s.terrain & 15) == WORLD_TERRAIN_DESERT || s.terrain == WORLD_TERRAIN_GOLD_SAUCER_DESERT_EDGE) && (*(unsigned char *)&actor->animFrame == 1 || *(unsigned char *)&actor->animFrame == 8)) {
        g_WorldEffectOffset.x = 0;
        g_WorldEffectOffset.y = 20;
        g_WorldEffectOffsetZ = 0;
        World_SpawnEffectNode(0, 0, 8, 0);
        return;
    }
    /* Retail caller prototype (int, unsigned char) -> int preserved for /Od argument promotion on `World_IsModelInMask`. */
    if (((int(__cdecl *)(int, unsigned char))World_IsModelInMask)(32, actor->actorType) && WorldEntity_IsPlayerAltitudeScripted() == 0) {
        if (active && (*(unsigned char *)&actor->animFrame % 3) == 0) {
            g_WorldEffectVelocity.x = 400;
            World_SpawnEffectNode(s.direction, 0, 9, 0);
            g_WorldEffectVelocity.x = -500;
            World_SpawnEffectNode(s.direction, 0, 9, 0);
        }
        return;
    }
    /* Retail caller prototype (int, unsigned char) -> int preserved for /Od argument promotion on `World_IsModelInMask`. */
    if (active && (((int(__cdecl *)(int, unsigned char))World_IsModelInMask)(7, actor->actorType) || ((int(__cdecl *)(unsigned char))World_IsRideableModel)(actor->actorType)) && (s.terrain == WORLD_TERRAIN_FOREST || s.terrain == WORLD_TERRAIN_JUNGLE) && (*(unsigned char *)&actor->animFrame % 3) == 0) {
        g_WorldEffectOrigin.vy += 300;
        World_SpawnEffectNode(s.direction, 0, 10, 0);
        return;
    }
    /* Retail caller prototype (int, unsigned char) -> int preserved for /Od argument promotion on `World_IsModelInMask`. */
    if (((int(__cdecl *)(int, unsigned char))World_IsModelInMask)(7, actor->actorType) && s.terrain == WORLD_TERRAIN_SWAMP) {
        if (active && (*(unsigned char *)&actor->animFrame == 1 || *(unsigned char *)&actor->animFrame == 8)) {
            g_WorldEffectOffset.x = actor->pos.vx - actor->altPos.vx;
            g_WorldEffectOffsetZ = actor->pos.vz - actor->altPos.vz;
            World_SpawnEffectNode(0, 0x800, 12, 0);
        }
        return;
    }
    /* Retail caller prototype (unsigned char) -> int preserved for /Od argument promotion on `World_IsRideableModel`. */
    if (active && ((int(__cdecl *)(unsigned char))World_IsRideableModel)(actor->actorType) && s.terrain >= WORLD_TERRAIN_SEA && s.terrain <= WORLD_TERRAIN_SWAMP && (*(unsigned char *)&actor->animFrame == 1 || *(unsigned char *)&actor->animFrame == 13)) {
        g_WorldEffectVelocityZ = -100;
        World_SpawnEffectNode(s.direction, 0, 13, 0);
        return;
    }
    if (active && actor->actorType == WORLD_MODEL_BUGGY) {
        if ((s.terrain == WORLD_TERRAIN_FOREST || s.terrain == WORLD_TERRAIN_JUNGLE) && (*(unsigned char *)&actor->animFrame % 3) == 0) {
            g_WorldEffectOrigin.vy += 300;
            World_SpawnEffectNode(0, 0, 14, 0);
        } else if ((*(unsigned char *)&actor->animFrame % 3) == 0) {
            s.sprite = s.terrain != WORLD_TERRAIN_RIVER_CROSSING ? 16 : 15;
            g_WorldEffectOffset.y = 20;
            g_WorldEffectVelocity.y = 100;
            g_WorldEffectVelocityZ = -300;
            g_WorldEffectVelocity.x = 150;
            ((PsxMatrix *)&s.matrix)->t[0] = ((PsxMatrix *)&s.matrix)->t[1] = ((PsxMatrix *)&s.matrix)->t[2] = 0;
            SetTransMatrix(&s.matrix);
            s.matrix[0] = s.matrix[4] = s.matrix[8] = 4096;
            s.matrix[1] = s.matrix[2] = s.matrix[3] = s.matrix[5] = s.matrix[6] = s.matrix[7] = 0;
            WorldEntity_GetPlayerSlopeTiltAngles(s.angles);
            RotateMatrixY(s.direction, &s.matrix);
            RotateMatrixX(s.angles[0], &s.matrix);
            RotateMatrixZ(s.angles[2], &s.matrix);
            SetRotMatrix(&s.matrix);
            RotTransPoint(&g_WorldEffectVelocity.x, &s.transformed, &s.flags);
            g_WorldEffectVelocity.x = s.transformed.vx;
            g_WorldEffectVelocity.y = s.transformed.vy;
            g_WorldEffectVelocityZ = s.transformed.vz;
            World_SpawnEffectNode(0, 0, s.sprite, 10);
            SetRotMatrix(&s.matrix);
            g_WorldEffectVelocity.x = -150;
            g_WorldEffectVelocity.y = 100;
            g_WorldEffectVelocityZ = -300;
            RotTransPoint(&g_WorldEffectVelocity.x, &s.transformed, &s.flags);
            g_WorldEffectVelocity.x = s.transformed.vx;
            g_WorldEffectVelocity.y = s.transformed.vy;
            g_WorldEffectVelocityZ = s.transformed.vz;
            World_SpawnEffectNode(0, 0, s.sprite, 10);
        }
        return;
    }
    if (active && actor->actorType == WORLD_MODEL_SUBMARINE) {
        if (World_GetMapVariant() == 2 && (*(unsigned char *)&actor->animFrame & 7) == 0) {
            g_WorldEffectVelocity.y = 200;
            g_WorldEffectVelocityZ = -400;
            g_WorldEffectOffset.y = 50;
            World_SpawnEffectNode(s.direction, s.direction, 17, 0);
        } else if (World_GetMapVariant() == 0) {
            g_WorldEffectVelocity.y = 240;
            g_WorldEffectVelocityZ = -500;
            World_SpawnEffectNode(s.direction, 0, (g_WorldEffectState & 3) + 45, 0);
        }
        return;
    }
    if (active && actor->actorType == 30 && (*(unsigned char *)&actor->animFrame & 15) == 0) {
        g_WorldEffectVelocity.x = 600;
        g_WorldEffectVelocity.y = 1000;
        g_WorldEffectVelocityZ = -400;
        g_WorldEffectOffset.y = 50;
        World_SpawnEffectNode(s.direction, s.direction, 17, 0);
        g_WorldEffectVelocity.x = -600;
        g_WorldEffectVelocity.y = 1000;
        g_WorldEffectVelocityZ = -400;
        g_WorldEffectOffset.y = 50;
        World_SpawnEffectNode(s.direction, s.direction, 17, 0);
        return;
    }
    if (active && actor->actorType == WORLD_MODEL_SHIP && (*(unsigned char *)&actor->animFrame & 3) == 0) {
        g_WorldEffectVelocityZ = -700;
        World_SpawnEffectNode(s.direction, 0, 18, 0);
        return;
    }
}

/*
 * Pops a free world node into the active list, returning without changes when
 * none exists. Installs zero translation and two yaw rotations to derive the
 * node motion vector and position from the global vectors.
 */
// FUNCTION: FF7 0x0075b8f3
void __cdecl World_SpawnEffectNode(short spawnYaw, short vectorYaw, short effect, int byte1F)
{
    struct {
        VECTOR rotatedOffset;
        WorldEffectMatrix transformMatrix;
        int dwFlags_unused;
        short eulerAngles[4];
        int i;
        WorldEffectNode *effectNode;
    } frameSlot; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frameSlot.effectNode = g_pWorldEffectNodeFreeList;
    if (frameSlot.effectNode) {
        g_pWorldEffectNodeFreeList = frameSlot.effectNode->nextNode;
        frameSlot.effectNode->nextNode = g_pWorldEffectActiveList;
        g_pWorldEffectActiveList = frameSlot.effectNode;

        ((PsxMatrix *)frameSlot.transformMatrix)->t[0] =
            ((PsxMatrix *)frameSlot.transformMatrix)->t[1] =
                ((PsxMatrix *)frameSlot.transformMatrix)->t[2] = 0;
        SetTransMatrix((void *)frameSlot.transformMatrix);

        frameSlot.eulerAngles[0] = frameSlot.eulerAngles[2] = 0;
        frameSlot.eulerAngles[1] = vectorYaw;
        RotMatrixXYZ((void *)frameSlot.eulerAngles, (void *)frameSlot.transformMatrix);
        SetRotMatrix((void *)frameSlot.transformMatrix);
        RotTransPoint(&g_WorldEffectOffset.x, (int *)&frameSlot.rotatedOffset,
                      (int *)&frameSlot.dwFlags_unused);
        frameSlot.effectNode->velocityX = frameSlot.rotatedOffset.vx;
        frameSlot.effectNode->velocityY = frameSlot.rotatedOffset.vy;
        frameSlot.effectNode->velocityZ = frameSlot.rotatedOffset.vz;

        *(VECTOR *)&frameSlot.effectNode->posX = g_WorldEffectOrigin;
        frameSlot.eulerAngles[1] = spawnYaw;
        RotMatrixXYZ((void *)frameSlot.eulerAngles, (void *)frameSlot.transformMatrix);
        SetRotMatrix((void *)frameSlot.transformMatrix);
        RotTransPoint(&g_WorldEffectVelocity.x, (int *)&frameSlot.rotatedOffset,
                      (int *)&frameSlot.dwFlags_unused);
        frameSlot.effectNode->posX = g_WorldEffectOrigin.vx + frameSlot.rotatedOffset.vx;
        frameSlot.effectNode->posY = (&g_WorldEffectOrigin)->vy + frameSlot.rotatedOffset.vy;
        frameSlot.effectNode->posZ = (&g_WorldEffectOrigin)->vz + frameSlot.rotatedOffset.vz;

        frameSlot.effectNode->yaw = g_WorldEffectAngle ^ 0x800;
        frameSlot.effectNode->deceleration = byte1F;
        frameSlot.effectNode->recordIndex = (&g_WorldEffectConfig[8])[effect * 4];
        frameSlot.effectNode->horizontal = (&g_WorldEffectConfig[8])[effect * 4 + 1];
        frameSlot.effectNode->timer =
            g_WorldEffectRecordTable[frameSlot.effectNode->recordIndex * 0xC] & ~0x80;
        frameSlot.effectNode->active = 0;
        for (frameSlot.i = 0; frameSlot.i < 2; frameSlot.i++) {
            frameSlot.effectNode->fadeLevel =
                (g_WorldEffectRecordTable[frameSlot.effectNode->recordIndex * 0xC + 1] &
                 0x1C) == 8
                    ? 0
                    : 128;
            frameSlot.effectNode->quad.color =
                (g_WorldEffectRecordTable[frameSlot.effectNode->recordIndex * 0xC + 1] &
                 0x1C) == 8
                    ? 0x80000000
                    : 0x80808080;
            frameSlot.effectNode->quad.batch = (GfxRenderObject *)(g_WorldDfxRenderObject0);
        }
    }
}

/*
 * Spawns effect node 7 at the player's position (offset height 0x32, no velocity), then sets the
 * current and next frame of the WorldEffectRecord selected by g_WorldEffectRecordIndex to 0x10.
 */
// FUNCTION: FF7 0x0075bb25
void __cdecl World_SpawnPlayerEffectNode7Sized16(void)
{
    WorldEffectRecord *rec;

    g_WorldEffectOffset.x = 0;
    g_WorldEffectOffset.y = 50;
    g_WorldEffectOffsetZ = 0;
    g_WorldEffectAngle = 0;
    WorldEntity_GetPlayerPosition(&g_WorldEffectOrigin);
    g_WorldEffectVelocityZ = 0;
    g_WorldEffectVelocity.y = g_WorldEffectVelocityZ;
    g_WorldEffectVelocity.x = g_WorldEffectVelocity.y;
    World_SpawnEffectNode(0, 0, 7, 0);
    rec = (WorldEffectRecord *)((*(unsigned char *)&g_WorldEffectRecordIndex) * 0xC + (char *)&g_WorldEffectRecordTable);
    rec->next = 0x10;
    rec->current = 0x10;
}

/*
 * Seeds world effect globals from the actor heading and alternate position,
 * clears motion offsets and dispatches by effect kind. Kinds 20 through 23 use
 * camera-dependent height and rear offset; kind 1 emits eight surrounding
 * effects with signed offsets and motion components.
 */
// FUNCTION: FF7 0x0075bbb3
void __cdecl World_SpawnActorEffectsByKind(WorldActor *actor, int effect)
{
    struct {
        int angle, unused;
        WorldEffectRecord *record;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.unused = actor->heading + actor->headingOffset;
    g_WorldEffectAngle = actor->heading;
    g_WorldEffectOrigin = actor->altPos;
    g_WorldEffectOffset.x = g_WorldEffectOffset.y = g_WorldEffectOffsetZ = 0;
    g_WorldEffectVelocity.x = g_WorldEffectVelocity.y = g_WorldEffectVelocityZ = 0;
    switch (effect) {
    case WORLD_ACTOR_EFFECT_DUST_20:
    case WORLD_ACTOR_EFFECT_DUST_21:
    case WORLD_ACTOR_EFFECT_DUST_22:
        g_WorldEffectVelocity.y = World_GetCameraMode() ? 300 : 600;
        g_WorldEffectVelocityZ = -200;
        World_SpawnEffectNode(actor->heading, 0, effect, 0);
        break;
    case WORLD_ACTOR_EFFECT_DUST_HIGH:
        g_WorldEffectVelocity.y = World_GetCameraMode() ? 300 : 600;
        g_WorldEffectVelocityZ = -200;
        g_WorldEffectOffset.y = 50;
        World_SpawnEffectNode(actor->heading, 0, effect, 0);
        break;
    case WORLD_ACTOR_EFFECT_SPLASH_RING:
        g_WorldEffectAngle = 0;
        g_WorldEffectVelocityZ = 500;
        g_WorldEffectOffsetZ = 30;
        World_SpawnEffectNode(0, 0, 0x1c, 0);
        g_WorldEffectVelocityZ = -500;
        g_WorldEffectOffsetZ = -30;
        World_SpawnEffectNode(0, 0, 0x20, 0);
        g_WorldEffectVelocityZ = 353;
        g_WorldEffectOffsetZ = 21;
        g_WorldEffectVelocity.x = 353;
        g_WorldEffectOffset.x = 21;
        World_SpawnEffectNode(0, 0, 0x23, 0);
        g_WorldEffectVelocity.x = -353;
        g_WorldEffectOffset.x = -21;
        World_SpawnEffectNode(0, 0, 0x1d, 0);
        g_WorldEffectVelocityZ = -353;
        g_WorldEffectOffsetZ = -21;
        World_SpawnEffectNode(0, 0, 0x1f, 0);
        g_WorldEffectVelocity.x = 353;
        g_WorldEffectOffset.x = 21;
        World_SpawnEffectNode(0, 0, 0x21, 0);
        g_WorldEffectVelocityZ = 0;
        g_WorldEffectOffsetZ = 0;
        g_WorldEffectVelocity.x = 500;
        g_WorldEffectOffset.x = 30;
        World_SpawnEffectNode(0, 0, 0x22, 0);
        g_WorldEffectVelocity.x = -500;
        g_WorldEffectOffset.x = -30;
        World_SpawnEffectNode(0, 0, 0x1e, 0);
        break;
    case WORLD_ACTOR_EFFECT_SAND_PUFF:
        frame.angle = WorldMap_NextRandomByte() << 4;
        g_WorldEffectOffset.x = CosFixed(frame.angle) >> 6;
        g_WorldEffectOffset.y = 120;
        g_WorldEffectVelocityZ = 500;
        World_SpawnEffectNode(frame.angle, 0, 0x31, 20);
        break;
    case WORLD_ACTOR_EFFECT_WAKE_SPRAY:
        g_WorldEffectVelocity.y = 300;
        g_WorldEffectVelocityZ = -1000;
        g_WorldEffectOffset.y = 20;
        g_WorldEffectOffsetZ = -10;
        World_SpawnEffectNode(actor->heading, actor->heading + WorldMap_NextRandomByte() * 8 - 0x400, 0x38, 1);
        break;
    case WORLD_ACTOR_EFFECT_PROPELLER_A:
        g_WorldEffectVelocity.x = 30;
        g_WorldEffectVelocity.y = 400;
        g_WorldEffectVelocityZ = 500;
        World_SpawnEffectNode(actor->heading, 0, 0x39, 0);
        frame.record = (WorldEffectRecord *)((char *)&g_WorldEffectRecordTable + *(unsigned char *)&g_WorldMiniMapRecordIndex * 12);
        frame.record->next = 11;
        frame.record->current = 11;
        break;
    case WORLD_ACTOR_EFFECT_PROPELLER_B:
        g_WorldEffectVelocity.x = 30;
        g_WorldEffectVelocity.y = 400;
        g_WorldEffectVelocityZ = 500;
        World_SpawnEffectNode(actor->heading, 0, 0x3a, 0);
        break;
    case WORLD_ACTOR_EFFECT_SHOCKWAVE:
        g_WorldEffectOffset.y = 50;
        g_WorldEffectAngle = 0;
        World_SpawnEffectNode(0, 0, 7, 0);
        frame.record = (WorldEffectRecord *)((char *)&g_WorldEffectRecordTable + (*(unsigned char *)&g_WorldEffectRecordIndex) * 12);
        frame.record->next = 16;
        frame.record->current = 16;
    }
}

/*
 * Writes one RGB triple into the world colour table: row
 * g_WorldEffectEmitterTable[index] gets a, b, c and a zero fourth byte.
 */
// FUNCTION: FF7 0x0075bfe5
void __cdecl World_SetEffectEmitterRecord(int index, char red, char green, char blue)
{
    unsigned char *e;

    e = g_WorldEffectEmitterTable[index];
    e[0] = red;
    e[1] = green;
    e[2] = blue;
    e[3] = 0;
}

/*
 * Clears byte 2 of the unchecked color-table row
 * g_WorldEffectEmitterTable[index].
 */
// FUNCTION: FF7 0x0075c01b
void __cdecl World_DisableEffectEmitterRecord(int index)
{
    g_WorldEffectEmitterTable[index][2] = 0;
}

/*
 * Returns a completed pending chain to the free list when both completion
 * pointers are set, clears those pointers and runs the two update helpers.
 * Walks four-byte timed actor records: a positive interval with an expiring
 * byte countdown selects its actor, resets the countdown and dispatches the
 * recorded action, or disables the interval when actor selection fails.
 */
// FUNCTION: FF7 0x0075c02b
void __cdecl World_RunEffectNodeFrame(void)
{
    char *record;

    if (g_pWorldEffectRetiredList != 0 && g_pWorldEffectRetiredLast != 0) {
        g_pWorldEffectRetiredLast->nextNode = g_pWorldEffectNodeFreeList;
        g_pWorldEffectNodeFreeList = g_pWorldEffectRetiredList;
        g_pWorldEffectRetiredList = g_pWorldEffectRetiredLast = 0;
    }
    World_DrawEffectNodes();
    World_UpdateEffectNodes();
    for (record = (char *)g_WorldEffectEmitterTable[0]; record < (char *)&g_pWorldEffectNodeFreeList; record += 4) {
        if (record[2] > 0 && (record[3]-- <= 1)) {
            if (WorldEntity_SetCurrentById(record[0])) {
                record[3] = record[2];
                World_SpawnActorEffectsByKind(WorldEntity_GetCurrent(), record[1]);
            } else {
                record[2] = 0;
            }
        }
    }
}

/*
 * Builds yaw-only and combined camera-pitch matrices, gets the stream-focus
 * position and visits the linked effect nodes. For nodes within the strict
 * 30000 X/Z bounds, computes terrain-adjusted relative coordinates, selects
 * either the node yaw matrix or camera matrix according to its flag, and draws
 * the node using its indexed descriptor and flag.
 */
// FUNCTION: FF7 0x0075c0fd
void __cdecl World_DrawEffectNodes(void)
{
    struct {
        WorldEffectMatrix nodeMatrix;
        WorldEffectMatrix yawMatrix;
        int relZ;
        WorldEffectMatrix cameraMatrix;
        int relY;
        int relX;
        short position[4];
        short angles[4];
        WorldStreamFocusData focus;
        WorldEffectNode *node;
    } local_frame;

    local_frame.angles[0] = 0;
    local_frame.angles[1] = -World_GetCameraRotationY();
    local_frame.angles[2] = 0;
    RotMatrix((SVECTOR *)local_frame.angles, (int *)local_frame.yawMatrix);
    local_frame.angles[0] = -World_GetCurrentCameraRotationX();
    RotMatrix((SVECTOR *)local_frame.angles, (int *)local_frame.cameraMatrix);
    World_GetStreamFocus(&local_frame.focus);
    for (local_frame.node = g_pWorldEffectActiveList;
         local_frame.node != 0;
         local_frame.node = local_frame.node->nextNode) {
        local_frame.relX =
            local_frame.node->posX - local_frame.focus.x;
        local_frame.relY =
            local_frame.node->posY - g_WorldCameraDeltaY;
        local_frame.relZ =
            local_frame.node->posZ - local_frame.focus.z;
        if (local_frame.relX > -30000 && local_frame.relX < 30000 &&
            local_frame.relZ > -30000 && local_frame.relZ < 30000) {
            local_frame.position[0] = local_frame.relX;
            local_frame.position[1] =
                local_frame.relY -
                World_GetPointFadeWeight(local_frame.relX,
                                                                     local_frame.relZ);
            local_frame.position[2] = local_frame.relZ;
            if (local_frame.node->horizontal != 0) {
                local_frame.angles[2] = 0;
                local_frame.angles[0] = local_frame.angles[2];
                local_frame.angles[1] =
                    local_frame.node->yaw;
                RotMatrixXYZ(local_frame.angles, (int *)local_frame.nodeMatrix);
                SetRotMatrix(local_frame.nodeMatrix);
            } else {
                SetRotMatrix(local_frame.cameraMatrix);
            }
            World_DrawEffectNodeQuad(
                &local_frame.node->quad,
                (WorldEffectSprite *)((char *)&g_WorldEffectRecordTable +
                                      local_frame.node->recordIndex * 0xC),
                local_frame.position,
                local_frame.node->horizontal);
        }
    }
}

/*
 * Builds an effect quad from sprite width/height in the XY or XZ plane,
 * rotating its signed-short corners through the currently installed matrix
 * before installing the position transform. Copies sprite UV/palette data and
 * selects the dfx batch whose blend mode the low two flag bits name; a subtractive
 * sprite draws additive, or averaged for palette 20.
 */
// FUNCTION: FF7 0x0075c283
void __cdecl World_DrawEffectNodeQuad(WorldShadowQuad *quad, WorldEffectSprite *sprite, short *position, short horizontal)
{
    struct {
        GfxTlVertex *v3, *v2, *v1, *v0;
        float tv3, tv2, tv1, tv0, tu3, tu2, tu1, tu0;
        short corner[4];
        int bucket;
        float iw3, iw2;
        WorldEffectMatrix matrix;
        FloatVector3 z3;
        float iw1;
        FloatVector3 z2;
        float iw0;
        FloatVector3 z1;
        short corner3[4];
        FloatVector3 z0;
        short corner2[4];
        VECTOR zero;
        short corner1[4], corner0[4];
        int flags;
        float screen[4][4];
        FloatVector3 p3;
        float clip3[4];
        float z;
        FloatVector3 p2;
        float clip2[4];
        float camera[4][4];
        FloatVector3 p1;
        float clip1[4];
        FloatVector3 p0;
        float clip0[4];
        VECTOR transformed;
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    s.zero.vx = s.zero.vy = s.zero.vz = 0;
    SetMatrixTranslation(&s.matrix, &s.zero);
    SetTransMatrix(&s.matrix);
    s.corner[0] = (-sprite->width) * 8;
    if (horizontal) {
        s.corner[1] = 0;
        s.corner[2] = (-sprite->height) * 8;
    } else {
        s.corner[1] = (-sprite->height) * 8;
        s.corner[2] = 0;
    }
    RotTransPoint((void *)s.corner, &s.transformed, &s.flags);
    s.corner0[0] = s.transformed.vx;
    s.corner0[1] = s.transformed.vy;
    s.corner0[2] = s.transformed.vz;
    s.corner[0] += sprite->width * 16;
    RotTransPoint((void *)s.corner, &s.transformed, &s.flags);
    s.corner1[0] = s.transformed.vx;
    s.corner1[1] = s.transformed.vy;
    s.corner1[2] = s.transformed.vz;
    if (horizontal)
        s.corner[2] += sprite->height * 16;
    else
        s.corner[1] += sprite->height * 16;
    RotTransPoint((void *)s.corner, &s.transformed, &s.flags);
    s.corner3[0] = s.transformed.vx;
    s.corner3[1] = s.transformed.vy;
    s.corner3[2] = s.transformed.vz;
    s.corner[0] -= sprite->width * 16;
    RotTransPoint((void *)s.corner, &s.transformed, &s.flags);
    s.corner2[0] = s.transformed.vx;
    s.corner2[1] = s.transformed.vy;
    s.corner2[2] = s.transformed.vz;
    quad->u0 = quad->u2 = sprite->u0;
    quad->u1 = quad->u3 = sprite->u1;
    quad->v0 = quad->v1 = sprite->v0;
    quad->v2 = quad->v3 = sprite->v1;
    quad->clut = sprite->palette;
    switch (sprite->flags & 3) {
    case BLEND_AVERAGE:
        quad->batch = (GfxRenderObject *)(g_WorldDfxRenderObject0);
        break;
    case BLEND_ADDITIVE:
        quad->batch = (GfxRenderObject *)(g_WorldDfxRenderObject1);
        break;
    case BLEND_SUBTRACTIVE:
        /* no subtractive batch: palette 20 draws averaged, the rest additive */
        quad->batch = (short)sprite->palette == 20 ? (GfxRenderObject *)(g_WorldDfxRenderObject1) : (GfxRenderObject *)(g_WorldDfxRenderObject0);
        break;
    case BLEND_QUARTER_ADDITIVE:
        quad->batch = (GfxRenderObject *)(g_WorldDfxRenderObject3);
        break;
    default:
        quad->batch = (GfxRenderObject *)(g_WorldDfxRenderObject0);
    }
    World_LoadCameraMatrixToGte();
    RotTransPoint(position, &s.transformed, &s.flags);
    SetMatrixTranslation(&s.matrix, &s.transformed);
    SetTransMatrix(&s.matrix);
    GtePsxMatrixToFloat4x4Transposed(GetInstalledRotMatrixPtr(), s.camera);
    ComposeWithScreenMatrix(s.camera, s.screen);
    s.p0.x = (float)s.corner0[0];
    s.p0.y = (float)s.corner0[1];
    s.p0.z = (float)s.corner0[2];
    s.p1.x = (float)s.corner1[0];
    s.p1.y = (float)s.corner1[1];
    s.p1.z = (float)s.corner1[2];
    s.p2.x = (float)s.corner2[0];
    s.p2.y = (float)s.corner2[1];
    s.p2.z = (float)s.corner2[2];
    s.p3.x = (float)s.corner3[0];
    s.p3.y = (float)s.corner3[1];
    s.p3.z = (float)s.corner3[2];
    if (TransformPointByMatrix4x4(s.screen, &s.p0, s.clip0), s.clip0[3] > 0.0f) {
        TransformPointDepthOnly((float *)s.camera, (float *)&s.p0, (float *)&s.z0);
        if (TransformPointByMatrix4x4(s.screen, &s.p1, s.clip1), s.clip1[3] > 0.0f) {
            TransformPointDepthOnly((float *)s.camera, (float *)&s.p1, (float *)&s.z1);
            if (TransformPointByMatrix4x4(s.screen, &s.p2, s.clip2), s.clip2[3] > 0.0f) {
                TransformPointDepthOnly((float *)s.camera, (float *)&s.p2, (float *)&s.z2);
                if (TransformPointByMatrix4x4(s.screen, &s.p3, s.clip3), s.clip3[3] > 0.0f) {
                    TransformPointDepthOnly((float *)s.camera, (float *)&s.p3, (float *)&s.z3);
                    s.bucket = (int)((s.z0.z < s.z1.z ? s.z0.z : s.z1.z) < s.z2.z ? (s.z0.z < s.z1.z ? s.z0.z : s.z1.z) : s.z2.z);
                    s.bucket = (s.bucket < World_RoundFloatToInt(s.z3.z) ? s.bucket : World_RoundFloatToInt(s.z3.z)) >> 4;
                    if (s.bucket >= 0 && s.bucket < 4096) {
                        if (AcquireBatchNode(1, (void *)quad->batch)) {
                            if (World_IsTerrainRenderEnabled()) {
                                s.tu0 = (float)quad->u0 * quad->batch->invWidth;
                                s.tv0 = (float)quad->v0 * quad->batch->invHeight;
                                s.tu1 = (float)quad->u1 * quad->batch->invWidth;
                                s.tv1 = (float)quad->v1 * quad->batch->invHeight;
                                s.tu2 = (float)quad->u2 * quad->batch->invWidth;
                                s.tv2 = (float)quad->v2 * quad->batch->invHeight;
                                s.tu3 = (float)quad->u3 * quad->batch->invWidth;
                                s.tv3 = (float)quad->v3 * quad->batch->invHeight;
                                s.iw0 = 1.0f / s.clip0[3];
                                s.iw1 = 1.0f / s.clip1[3];
                                s.iw2 = 1.0f / s.clip2[3];
                                s.iw3 = 1.0f / s.clip3[3];
                                quad->x0 = World_RoundFloatToInt(s.clip0[0] * s.iw0);
                                quad->y0 = World_RoundFloatToInt(s.clip0[1] * s.iw0);
                                quad->x1 = World_RoundFloatToInt(s.clip1[0] * s.iw1);
                                quad->y1 = World_RoundFloatToInt(s.clip1[1] * s.iw1);
                                quad->x2 = World_RoundFloatToInt(s.clip2[0] * s.iw2);
                                quad->y2 = World_RoundFloatToInt(s.clip2[1] * s.iw2);
                                quad->x3 = World_RoundFloatToInt(s.clip3[0] * s.iw3);
                                quad->y3 = World_RoundFloatToInt(s.clip3[1] * s.iw3);
                                if (s.clip0[2] < s.clip1[2]) {
                                    s.z = s.clip0[2];
                                } else {
                                    s.z = s.clip1[2];
                                    s.iw0 = s.iw1;
                                }
                                if (s.z > s.clip2[2]) {
                                    s.z = s.clip2[2];
                                    s.iw0 = s.iw2;
                                }
                                if (s.z > s.clip3[2]) {
                                    s.z = s.clip3[2];
                                    s.iw0 = s.iw3;
                                }
                                s.z *= s.iw0;
                                s.v0 = &quad->batch->vertices[0];
                                s.v0->x = (float)quad->x0;
                                s.v0->y = (float)quad->y0;
                                s.v0->z = s.z;
                                s.v0->rhw = 1.0f;
                                s.v0->colorUnsigned = quad->color;
                                s.v0->specularUnsigned = 0xff000000;
                                s.v0->u = s.tu0;
                                s.v0->v = s.tv0;
                                s.v1 = &quad->batch->vertices[1];
                                s.v1->x = (float)quad->x1;
                                s.v1->y = (float)quad->y1;
                                s.v1->z = s.z;
                                s.v1->rhw = 1.0f;
                                s.v1->colorUnsigned = quad->color;
                                s.v1->specularUnsigned = 0xff000000;
                                s.v1->u = s.tu1;
                                s.v1->v = s.tv1;
                                s.v2 = &quad->batch->vertices[2];
                                s.v2->x = (float)quad->x2;
                                s.v2->y = (float)quad->y2;
                                s.v2->z = s.z;
                                s.v2->rhw = 1.0f;
                                s.v2->colorUnsigned = quad->color;
                                s.v2->specularUnsigned = 0xff000000;
                                s.v2->u = s.tu2;
                                s.v2->v = s.tv2;
                                s.v3 = &quad->batch->vertices[3];
                                s.v3->x = (float)quad->x3;
                                s.v3->y = (float)quad->y3;
                                s.v3->z = s.z;
                                s.v3->rhw = 1.0f;
                                s.v3->colorUnsigned = quad->color;
                                s.v3->specularUnsigned = 0xff000000;
                                s.v3->u = s.tu3;
                                s.v3->v = s.tv3;
                                *quad->batch->palette = quad->clut;
                                quad->batch->lastPalette = (int)quad->clut;
                            }
                        }
                    }
                }
            }
        }
    }
}

/*
 * Walks the active world-effect linked list, adds signed velocities to
 * position and subtracts acceleration from vertical velocity. Applies the
 * current descriptor mode to intensity, packed color, one-time upward velocity
 * or shared descriptor bytes: supported modes fade by one, eight, twelve or
 * sixteen, ramp then fade, launch once, or adjust descriptor values.
 */
// FUNCTION: FF7 0x0075ceed
void __cdecl World_UpdateEffectNodes(void)
{
    WorldEffectNode *node;
    WorldEffectRecord *recordPtr;
    WorldShadowQuad *tint;
    WorldEffectNode *last;
    WorldBlendColor color;

    last = 0;
    node = g_pWorldEffectActiveList;
    while (node != 0) {
        tint = &node->quad;
        node->posX += node->velocityX;
        node->posY += node->velocityY;
        node->posZ += node->velocityZ;
        node->velocityY -= node->deceleration;
        recordPtr = (WorldEffectRecord *)(node->recordIndex * 0xC + (char *)&g_WorldEffectRecordTable);
        if (recordPtr->flags == WORLD_EFFECT_RECORD_FADE_SLOW) {
            node->fadeLevel -= 1;
            tint->r = tint->g =
                tint->b = *(unsigned char *)&node->fadeLevel;
            color.value = tint->color;
            if (node->recordIndex == 124) {
                color.c.b -= 1;
                color.c.r = color.c.g =
                    color.c.b;
                tint->color = color.value;
            } else {
                color.c.r = color.c.g =
                    color.c.b = 0;
                color.c.a -= 1;
                tint->color = color.value;
            }
        } else {
            switch (recordPtr->flags & WORLD_EFFECT_RECORD_MODE_MASK) {
            case WORLD_EFFECT_RECORD_FADE_SCROLL:
                node->fadeLevel -= 12;
                if (node->fadeLevel < 0) {
                    node->fadeLevel = 0;
                }
                tint->r = tint->g =
                    tint->b = *(unsigned char *)&node->fadeLevel;
                color.value = tint->color;
                color.c.r = color.c.g =
                    color.c.b = *(unsigned char *)&node->fadeLevel;
                tint->color = color.value;
                recordPtr->next += 0x10;
                recordPtr->current = recordPtr->next;
                break;
            case WORLD_EFFECT_RECORD_FADE_IN_OUT:
                if (node->active != 0) {
                    node->fadeLevel -= 8;
                    if (node->fadeLevel < 0) {
                        node->fadeLevel = 0;
                    }
                    tint->r = tint->g =
                        tint->b =
                            *(unsigned char *)&node->fadeLevel;
                    color.value = tint->color;
                    color.c.r = color.c.g =
                        color.c.b = *(unsigned char *)&node->fadeLevel;
                    tint->color = color.value;
                } else {
                    node->fadeLevel += 8;
                    tint->r = tint->g =
                        tint->b =
                            *(unsigned char *)&node->fadeLevel;
                    if (tint->r >= 128) {
                        node->active = 1;
                    }
                }
                break;
            case WORLD_EFFECT_RECORD_LAUNCH:
                if (node->active == 0) {
                    node->active = 1;
                    node->velocityY = 200;
                    node->deceleration = 0x1E;
                }
                break;
            case WORLD_EFFECT_RECORD_FADE_FAST:
                node->fadeLevel -= 16;
                if (node->fadeLevel < 0) {
                    node->fadeLevel = 0;
                }
                tint->r = tint->g =
                    tint->b = *(unsigned char *)&node->fadeLevel;
                color.value = tint->color;
                color.c.r = color.c.g =
                    color.c.b = *(unsigned char *)&node->fadeLevel;
                tint->color = color.value;
                break;
            case WORLD_EFFECT_RECORD_FADE:
                node->fadeLevel -= 8;
                if (node->fadeLevel < 0) {
                    node->fadeLevel = 0;
                }
                tint->r = tint->g =
                    tint->b = *(unsigned char *)&node->fadeLevel;
                color.value = tint->color;
                color.c.r = color.c.g =
                    color.c.b = *(unsigned char *)&node->fadeLevel;
                tint->color = color.value;
                break;
            case WORLD_EFFECT_RECORD_LOOP_FRAMES:
                recordPtr->next +=
                    recordPtr->current < 32
                        ? (recordPtr->current & 0xF ? 3 : -5)
                        : 0;
                recordPtr->current = recordPtr->next;
                break;
            }
        }
        node->timer -= 1;
        if (node->timer == 0) {
            node->recordIndex += 1;
            node->timer = ((WorldEffectRecord *)&g_WorldEffectRecordTable)[node->recordIndex].timer;
            if ((node->timer & 0x80) != 0) {
                if ((recordPtr->flags & WORLD_EFFECT_RECORD_MODE_MASK) == WORLD_EFFECT_RECORD_LOOP_FRAMES) {
                    for (;;) {
                        node->recordIndex -= 1;
                        node->timer = ((WorldEffectRecord *)&g_WorldEffectRecordTable)[node->recordIndex].timer;
                        if ((node->timer & 0x80) == 0) {
                            continue;
                        }
                        break;
                    }
                    node->timer &= 0x7F;
                } else {
                    if (last != 0) {
                        last->nextNode = node->nextNode;
                    } else {
                        g_pWorldEffectActiveList = node->nextNode;
                    }
                    node->nextNode = g_pWorldEffectRetiredList;
                    g_pWorldEffectRetiredList = node;
                    if (g_pWorldEffectRetiredLast == 0) {
                        g_pWorldEffectRetiredLast = node;
                    }
                    node = last != 0 ? last->nextNode : g_pWorldEffectActiveList;
                    continue;
                }
            }
        }
        last = node;
        node = node->nextNode;
    }
    g_WorldEffectState += 1;
}

// Copies the rectangle in src (shorts x, y, width, height) into the texture corners of the shadow
// quad dst, and does nothing when either pointer is null. u2 and v1 take the low bytes of x and y,
// the left and top, and u0 and v0 copy them; u3 and v3 are x plus width and y plus height, the
// right and bottom, and u1 and v2 copy those. The colour is set to 0x80404040 and the CLUT is
// cleared.
// FUNCTION: FF7 0x0075d482
void __cdecl World_SetShadowQuadTextureRect(WorldShadowQuad *dst, WorldShadowTextureRect *src)
{
    int i;

    if (dst != 0 && src != 0) {
        for (i = 0; i < 1; i++) {
            dst->color = 0x80404040;
            dst->clut = 0;
            dst->u2 = src->srcXLow;
            dst->u0 = dst->u2;
            dst->v1 = src->srcYLow;
            dst->v0 = dst->v1;
            dst->u3 = src->srcX + src->width;
            dst->u1 = dst->u3;
            dst->v3 = src->srcY + src->height;
            dst->v2 = dst->v3;
        }
    }
}

/*
 * Projects four corners formed from signed width/depth, common height and yaw
 * through the installed world/view and screen matrices, writes rounded screen
 * XY into quad and adds offset to every Y. Computes the depth bucket from the
 * minimum camera-space depth, shifting by four, and emits only for buckets 0
 * through 4095 and positive rounded clip W at all corners.
 */
// FUNCTION: FF7 0x0075d544
void __cdecl World_DrawShadowQuad(short width, short depth, short height, short yaw, WorldShadowQuad *quad, short offset)
{
    struct {
        GfxTlVertex *v3, *v2, *v1, *v0;
        float tv3, tv2, tv1, tv0, tu3, tu2, tu1, tu0;
        int bucket, unused107[2];
        float iw3, iw2;
        WorldEffectMatrix rotation;
        FloatVector3 z3;
        float iw1;
        FloatVector3 z2;
        float iw0;
        WorldEffectMatrix installed;
        FloatVector3 z1, z0;
        short translation[4];
        float screen[4][4];
        FloatVector3 p3;
        float clip3[4];
        float z;
        FloatVector3 p2;
        float clip2[4];
        float camera[4][4];
        int unused24[2];
        short angles[4];
        FloatVector3 p1;
        float clip1[4];
        int unused13[3];
        FloatVector3 p0;
        float clip0[4];
        int unused3[3];
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    s.translation[0] = quad->pad1;
    s.translation[1] = -g_WorldCameraDeltaY;
    s.translation[2] = quad->pad2;
    World_SetViewTransformForPosition((SVECTOR *)s.translation);
    s.p1.x = s.p3.x = (float)width;
    s.p2.z = s.p3.z = (float)depth;
    s.p0.x = s.p2.x = -s.p1.x;
    s.p0.z = s.p1.z = -s.p2.z;
    s.p0.y = s.p1.y = s.p2.y = s.p3.y = (float)height;
    s.angles[0] = s.angles[2] = 0;
    s.angles[1] = yaw;
    RotMatrixXYZ((void *)s.angles, &s.rotation);
    MultiplyByInstalledMatrixInto((PsxMatrix *)&s.rotation, (PsxMatrix *)&s.installed);
    SetRotMatrix(&s.installed);
    GtePsxMatrixToFloat4x4Transposed(GetInstalledRotMatrixPtr(), s.camera);
    ComposeWithScreenMatrix(s.camera, s.screen);
    TransformPointByMatrix4x4(s.screen, &s.p0, s.clip0);
    TransformPointByMatrix4x4(s.screen, &s.p1, s.clip1);
    TransformPointByMatrix4x4(s.screen, &s.p2, s.clip2);
    TransformPointByMatrix4x4(s.screen, &s.p3, s.clip3);
    s.iw0 = 1.0f / s.clip0[3];
    s.iw1 = 1.0f / s.clip1[3];
    s.iw2 = 1.0f / s.clip2[3];
    s.iw3 = 1.0f / s.clip3[3];
    quad->x0 = World_RoundFloatToInt(s.clip0[0] * s.iw0);
    quad->y0 = World_RoundFloatToInt(s.clip0[1] * s.iw0);
    quad->x1 = World_RoundFloatToInt(s.clip1[0] * s.iw1);
    quad->y1 = World_RoundFloatToInt(s.clip1[1] * s.iw1);
    quad->x2 = World_RoundFloatToInt(s.clip2[0] * s.iw2);
    quad->y2 = World_RoundFloatToInt(s.clip2[1] * s.iw2);
    quad->x3 = World_RoundFloatToInt(s.clip3[0] * s.iw3);
    quad->y3 = World_RoundFloatToInt(s.clip3[1] * s.iw3);
    TransformPointDepthOnly((float *)s.camera, (float *)&s.p0, (float *)&s.z0);
    TransformPointDepthOnly((float *)s.camera, (float *)&s.p1, (float *)&s.z1);
    TransformPointDepthOnly((float *)s.camera, (float *)&s.p2, (float *)&s.z2);
    TransformPointDepthOnly((float *)s.camera, (float *)&s.p3, (float *)&s.z3);
    s.bucket = (int)((s.z0.z < s.z1.z ? s.z0.z : s.z1.z) < s.z2.z ? (s.z0.z < s.z1.z ? s.z0.z : s.z1.z) : s.z2.z);
    quad->y0 += offset;
    quad->y1 += offset;
    quad->y2 += offset;
    quad->y3 += offset;
    s.bucket = (s.bucket < World_RoundFloatToInt(s.z3.z) ? s.bucket : World_RoundFloatToInt(s.z3.z)) >> 4;
    if (s.bucket >= 0 && s.bucket < 4096 && World_RoundFloatToInt(s.clip0[3]) > 0 && World_RoundFloatToInt(s.clip1[3]) > 0 && World_RoundFloatToInt(s.clip2[3]) > 0 && World_RoundFloatToInt(s.clip3[3]) > 0) {
        if (AcquireBatchNode(1, (void *)quad->batch)) {
            if (World_IsTerrainRenderEnabled()) {
                s.tu0 = (float)quad->u0 * quad->batch->invWidth;
                s.tv0 = (float)quad->v0 * quad->batch->invHeight;
                s.tu1 = (float)quad->u1 * quad->batch->invWidth;
                s.tv1 = (float)quad->v1 * quad->batch->invHeight;
                s.tu2 = (float)quad->u2 * quad->batch->invWidth;
                s.tv2 = (float)quad->v2 * quad->batch->invHeight;
                s.tu3 = (float)quad->u3 * quad->batch->invWidth;
                s.tv3 = (float)quad->v3 * quad->batch->invHeight;
                if (s.clip0[2] < s.clip1[2]) {
                    s.z = s.clip0[2];
                } else {
                    s.z = s.clip1[2];
                    s.iw0 = s.iw1;
                }
                if (s.z > s.clip2[2]) {
                    s.z = s.clip2[2];
                    s.iw0 = s.iw2;
                }
                if (s.z > s.clip3[2]) {
                    s.z = s.clip3[2];
                    s.iw0 = s.iw3;
                }
                s.z *= s.iw0;
                s.v0 = &quad->batch->vertices[0];
                s.v0->x = (float)quad->x0;
                s.v0->y = (float)quad->y0;
                s.v0->z = s.z;
                s.v0->rhw = 1.0f;
                s.v0->colorUnsigned = quad->color;
                s.v0->specularUnsigned = 0xff000000;
                s.v0->u = s.tu0;
                s.v0->v = s.tv0;
                s.v1 = &quad->batch->vertices[1];
                s.v1->x = (float)quad->x1;
                s.v1->y = (float)quad->y1;
                s.v1->z = s.z;
                s.v1->rhw = 1.0f;
                s.v1->colorUnsigned = quad->color;
                s.v1->specularUnsigned = 0xff000000;
                s.v1->u = s.tu1;
                s.v1->v = s.tv1;
                s.v2 = &quad->batch->vertices[2];
                s.v2->x = (float)quad->x2;
                s.v2->y = (float)quad->y2;
                s.v2->z = s.z;
                s.v2->rhw = 1.0f;
                s.v2->colorUnsigned = quad->color;
                s.v2->specularUnsigned = 0xff000000;
                s.v2->u = s.tu2;
                s.v2->v = s.tv2;
                s.v3 = &quad->batch->vertices[3];
                s.v3->x = (float)quad->x3;
                s.v3->y = (float)quad->y3;
                s.v3->z = s.z;
                s.v3->rhw = 1.0f;
                s.v3->colorUnsigned = quad->color;
                s.v3->specularUnsigned = 0xff000000;
                s.v3->u = s.tu3;
                s.v3->v = s.tv3;
                *quad->batch->palette = quad->clut;
                quad->batch->lastPalette = (int)quad->clut;
            }
        }
    }
}

/*
 * Computes and clamps a height-derived value for the actor color bytes, then
 * derives effect dimensions from a second height value and actor type. Uses
 * enlarged dimensions for types 3 and 11 and scaled dimensions for types
 * 4/19/41/42, then updates the effect at the stored ground height with actor
 * angles plus camera roll.
 */
// FUNCTION: FF7 0x0075deaa
void __cdecl World_DrawActorShadow(WorldActor *actor)
{
    int shade;
    unsigned char *colorPtr;
    short lengthX;
    short extentY;

    shade = 0x20 - ((actor->pos.vy - actor->curvedGroundY) >> 8);
    if (shade < 0) {
        shade = 0;
    }
    colorPtr = actor->shadowPoly;
    colorPtr[4] = colorPtr[5] = colorPtr[6] = shade;
    shade = 100 - ((actor->pos.vy - actor->curvedGroundY) >> 6);
    switch (actor->actorType) {
    case WORLD_MODEL_CHOCOBO_4:
    case WORLD_MODEL_CHOCOBO:
    case WORLD_MODEL_OVERLAY_CHOCOBO_41:
    case WORLD_MODEL_OVERLAY_CHOCOBO_42:
        extentY = (shade * 3) >> 1;
        lengthX = extentY;
        break;
    case WORLD_MODEL_HIGHWIND:
        lengthX = shade * 2;
        extentY = (shade * 3) * 2;
        break;
    case WORLD_MODEL_ULTIMA_WEAPON:
        lengthX = (shade * 3) * 2;
        extentY = (shade * 3) * 4;
        break;
    default:
        lengthX = shade;
        extentY = shade;
        break;
    }
    World_DrawShadowQuad(
        lengthX, extentY, actor->curvedGroundY,
        0x800 + actor->heading + actor->headingOffset + World_GetCameraRoll() * 4, (WorldShadowQuad *)colorPtr, 0);
}

/*
 * Empty. A hook in the world map's frame sequence whose body the shipped build
 * left out entirely.
 */
// FUNCTION: FF7 0x0075e024
void __cdecl World_ModelEffect14EndNoOp(WorldModelRecord *model)
{
    /* The original is empty. */
}

/*
 * Identity, then m[1][1]=0, m[1][2]=1, m[2][1]=-1, m[2][2]=0: a quarter turn
 * about X. The callee's name is what makes the four scaled displacements
 * readable as a 4x4 of floats.
 */
// FUNCTION: FF7 0x0075e029
void __cdecl World_SetQuarterTurnXMatrix4x4(float m[4][4])
{
    SetIdentityMatrix4x4(m);
    m[1][1] = 0.0f;
    m[1][2] = 1.0f;
    m[2][1] = -1.0f;
    m[2][2] = 0.0f;
}

/*
 * Copies three dwords from v into a local triple, then passes a, the local
 * triple and c to TransformPointHomogeneous.
 */
// FUNCTION: FF7 0x0075e062
void __cdecl World_TransformPositionByMatrix4x4(GfxMatrix4x4 *matrix, int *v, GfxFloat4 *outVec)
{
    int tmp[3];

    tmp[0] = v[0];
    tmp[1] = v[1];
    tmp[2] = v[2];
    TransformPointHomogeneous(matrix, (struct GfxHomogeneousPoint *)tmp, (struct GfxHomogeneousVec4 *)outVec);
}

/*
 * Returns 1 for values 0x12, 0x11 or 0x1b, and zero for every other value.
 */
// FUNCTION: FF7 0x0075e09a
int __cdecl World_ModelUsesBlendKind11(int value)
{
    if (value == 0x12 || value == 0x11 || value == 0x1b) {
        return 1;
    } else {
        return 0;
    }
}

/*
 * Obtains the model transform, converts it to a float camera matrix, scales
 * model rotation components by 1.40625 and transforms the model position.
 * Combines X and Z rotation matrices with camera and axis matrices, copies the
 * result into the model matrix and clears rotation components.
 */
// FUNCTION: FF7 0x0075e0ba
void __cdecl World_ApplyActorPoseToModelRecord(WorldModelRecord *model, WorldActor *actor, short animation, short frame, void *rotation)
{
    struct {
        WorldBlendColor returned;
        struct {
            WorldBlendSpec blend;
            int alpha, blendKind;
            short terrain, padTerrain;
            float combined[4][4], rotations[4][4], finalMatrix[4][4], identity[4][4];
            unsigned char type, padType[3];
            float zRotation[4][4], camera[4][4];
            WorldEffectMatrix psx;
            float position[4];
            float axes[4][4], xRotation[4][4];
            void *context;
            VECTOR *origin;
        } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    } f;
    f.s.origin = &g_WorldEffectOrigin;
    f.s.type = actor ? actor->actorType : -1;
    World_BuildModelPsxMatrix((WorldMapModel *)model, (SVECTOR *)rotation, &f.s.psx, f.s.type == WorldEntity_GetPlayerModelId());
    f.s.terrain = actor ? actor->walkmesh & 31 : 0;
    f.s.context = GetEngineContext();
    SetIdentityMatrix4x4(f.s.identity);
    Gfx_SetContextMatrix((void *)f.s.identity, (struct GfxEngineContext *)f.s.context);
    GtePsxMatrixToFloat4x4ColVec((PsxMatrix *)&f.s.psx, f.s.camera);
    model->rotation.x *= 1.40625f;
    model->rotation.y *= 1.40625f;
    model->rotation.z *= 1.40625f;
    World_SetQuarterTurnXMatrix4x4(f.s.axes);
    World_TransformPositionByMatrix4x4((GfxMatrix4x4 *)f.s.camera, (int *)&model->position, (GfxFloat4 *)f.s.position);
    model->position.x = f.s.position[0];
    model->position.y = f.s.position[1];
    model->position.z = f.s.position[2];
    BuildRotationXColVec(model->rotation.x, (GfxMatrix4x4 *)f.s.xRotation);
    BuildRotationZColVec(model->rotation.y, (GfxMatrix4x4 *)f.s.zRotation);
    MultiplyMatrix4x4Core(f.s.xRotation, f.s.zRotation, f.s.rotations);
    MultiplyMatrix4x4Core(f.s.camera, f.s.rotations, f.s.combined);
    MultiplyMatrix4x4Core(f.s.combined, f.s.axes, f.s.finalMatrix);
    ClearMatrixTranslationAndW(model->matrix);
    TransposeMatrix3x3(f.s.finalMatrix, model->matrix);
    model->rotation.x = 0;
    model->rotation.y = 0;
    model->rotation.z = 0;
    model->animation = animation;
    model->frame = frame;
    model->active = 1;
    model->effect = 0;
    if ((((WORLD_TERRAIN_MASK_SPLASH_WATER >> f.s.terrain) & 1) && World_GetMapVariant() != 2 && f.s.type != 3 && f.s.type != 25) || f.s.type == 29)
        model->effect = 14;
    if (actor && actor->effect == 14 && model->effect <= 0)
        World_ModelEffect14EndNoOp(model);
    if (actor)
        actor->effect = model->effect;
    if (0) /* Disabled in retail (/Od keeps the zero-test and body); see lib/codegen.h. */
        ;
    if (f.s.type == 18 || f.s.type == 17 || f.s.type == 26 || f.s.type == 27 || f.s.type == 28 || f.s.type == 30) {
        f.s.alpha = 255 - ((((PsxMatrix *)&f.s.psx)->t[2] / 4 - 4000) >> 3);
        if (f.s.alpha < 0)
            f.s.alpha = 0;
        else if (f.s.alpha > 255)
            f.s.alpha = 255;
        f.s.blendKind = World_ModelUsesBlendKind11(f.s.type) ? 11 : 9;
        Gfx_InitDrawSpec(f.s.blendKind, (GfxDrawSpec *)&f.s.blend);
        f.returned.value = GetDrawColorPacked();
        f.s.blend.color = f.returned;
        f.s.blend.color.c.a = f.s.alpha;
        if (f.s.blendKind == 11)
            f.s.blend.flags[0] = 0;
        Gfx_ApplyDrawSpecToModel((GfxDrawSpec *)&f.s.blend, (PolygonModel *)model->skeleton);
    }
}

/*
 * Looks up the selected model and engine context. For a present model,
 * converts three signed-short table color values to floats scaled by 1/255 and
 * passes the local color values through the model-color helpers, then
 * finalizes the model update.
 */
// FUNCTION: FF7 0x0075e4d6
void __cdecl World_SetModelRecordTintFromTable(short modelId, short tableIndex)
{
    GfxEngineContext *ctx;
    int subOrigin;
    int b;
    WorldModelRecord *model;
    float one;
    GfxFloatColorAddend blue;

    subOrigin = (int)&g_WorldEffectOrigin;
    model = World_GetModelRecord(modelId);
    ctx = GetEngineContext();
    if (model != 0) {
        blue.red = g_WorldModelTintTable[tableIndex][0] * 0.0039215689f;
        blue.green = g_WorldModelTintTable[tableIndex][1] * 0.0039215689f;
        blue.blue = g_WorldModelTintTable[tableIndex][2] * 0.0039215689f;
        one = 1.0f;
        Gfx_RestoreModelVertexColors((PolygonModelVertexColors *)model->skeleton, (int)ctx);
        Gfx_AddColorToModelVertexColors(&blue, (PolygonModelVertexColors *)model->skeleton, (int)ctx);
        Gfx_SaveModelVertexColors((PolygonModel *)model->skeleton);
    }
}

/*
 * Resets the world music state: the change gate g_WorldMusicChangeGate and the
 * current track g_WorldMusicTrack both to 0.
 */
// FUNCTION: FF7 0x0075e59a
void __cdecl World_ResetMusicState(int music)
{
    g_WorldMusicChangeGate = 0;
    g_WorldMusicTrack = 0;
}

/*
 * Sets the world music change gate g_WorldMusicChangeGate. World_SwitchMusicTrack
 * only issues a sound command while it is nonzero.
 */
// FUNCTION: FF7 0x0075e5b3
void __cdecl World_SetMusicChangeGate(int value)
{
    g_WorldMusicChangeGate = value;
}

/*
 * Switches the world map music to track track. While the gate
 * g_WorldMusicChangeGate is set it sends SoundDispatchCommand a crossfade for
 * track 1, otherwise a play (the alternate play code when the previous track was
 * 1), carrying the byte at g_pWorldMusicTrackTable[track - 1].
 */
// FUNCTION: FF7 0x0075e5c0
void __cdecl World_SwitchMusicTrack(int track)
{
    int cmdCode;
    int opcode;

    if (g_WorldMusicChangeGate != 0) {
        if (track == 1) {
            opcode = SOUND_MUSIC_CROSSFADE;
        } else {
            opcode = g_WorldMusicTrack - 1 != 0 ? SOUND_MUSIC_PLAY : SOUND_MUSIC_PLAY_ALT;
        }
        cmdCode = opcode;
        SoundDispatchCommand((short)cmdCode, *(g_pWorldMusicTrackTable + track - 1), 4, 0, 0, 0, 0, 0, 0);
    }
    g_WorldPrevMusicTrack = g_WorldMusicTrack;
    g_WorldMusicTrack = track;
}

/*
 * Re-issues the current world track (g_WorldMusicTrack) through
 * World_SwitchMusicTrack, which restarts the music after an interruption.
 */
// FUNCTION: FF7 0x0075e63a
void __cdecl World_ReplayCurrentMusicTrack(void)
{
    World_SwitchMusicTrack(g_WorldMusicTrack);
}

/*
 * Returns the current world music track, g_WorldMusicTrack.
 */
// FUNCTION: FF7 0x0075e64d
int __cdecl World_GetMusicTrack(void)
{
    return g_WorldMusicTrack;
}

/*
 * Passes every value to SfxPreloadAndPlayOnChannel5. Then calls
 * World_ArmScreenFlash for value 0x0b or
 * World_StartFlashSoundAndArmScreenFlash for value 0x1b1, with no extra action
 * for other values.
 */
// FUNCTION: FF7 0x0075e657
void __cdecl World_PlaySfxAndArmScreenFlash(int value)
{
    SfxPreloadAndPlayOnChannel5(value);
    if (value == 0xB) {
        World_ArmScreenFlash();
    } else if (value == 0x1B1) {
        World_StartFlashSoundAndArmScreenFlash();
    }
}

/*
 * Silences the world map's sound effects: stops every channel and resets the
 * all-channel transpose to 0.
 */
// FUNCTION: FF7 0x0075e683
void __cdecl World_StopAllSfxAndResetTranspose(void)
{
    SfxStopAll();
    SfxSetAllChannelsTranspose(0);
}

/*
 * Sets the MIDI volume for the world map; a wrapper over MidiSetVolume.
 */
// FUNCTION: FF7 0x0075e697
void __cdecl World_SetMidiVolume(int value)
{
    MidiSetVolume(value);
}

/*
 * Passes b as the transpose value and a as the transition argument to
 * SfxSetAllChannelsTransposeTrans.
 */
// FUNCTION: FF7 0x0075e6a8
void __cdecl World_SetAllSfxChannelsTransposeTrans(char steps, char transpose)
{
    SfxSetAllChannelsTransposeTrans(transpose, steps);
}

/*
 * Clears g_WorldAmbienceId, the id of the ambience that is playing (World_RequestAmbienceSfx).
 */
// FUNCTION: FF7 0x0075e6bd
void __cdecl World_ClearAmbienceId(void)
{
    g_WorldAmbienceId = 0;
}

/*
 * Start or stop the world-map ambience. g_WorldAmbienceId holds the id that is
 * currently playing; a request higher than it takes over, and the negation of the id that is
 * playing stops everything.
 */
// FUNCTION: FF7 0x0075e6cc
void __cdecl World_RequestAmbienceSfx(int req)
{
    if (req > g_WorldAmbienceId) {
        g_WorldAmbienceId = req;
        SfxPlayUpToFour(0x40, req, 0, 0, 0);
    } else if (req == -g_WorldAmbienceId) {
        g_WorldAmbienceId = 0;
        SfxStopAll();
        SfxSetAllChannelsTranspose(0);
    }
}

/*
 * Point g_pWorldMusicTrackTable at one of four eight-byte records by world stage
 * (WorldMap_LoadStageResources passes it): the early overworld and the warship share one,
 * the other overworld stages and the late warship the second, underwater and the snowfield
 * get their own.
 */
// FUNCTION: FF7 0x0075e720
void __cdecl World_SelectMusicParamTableByState(int state)
{
    switch (state) {
    case WORLD_STAGE_OVERWORLD_EARLY:
    case WORLD_STAGE_WARSHIP:
        g_pWorldMusicTrackTable = g_WorldMusicTrackRecord0;
        break;
    case WORLD_STAGE_OVERWORLD_DIAMOND_CANNON:
    case WORLD_STAGE_OVERWORLD_DIAMOND_MIDGUL_CANNON:
    case WORLD_STAGE_OVERWORLD_DIAMOND_CANNON_HW02:
    case WORLD_STAGE_OVERWORLD_DIAMOND_MIDGUL_HW02:
    case WORLD_STAGE_OVERWORLD_ULTIMA:
    case WORLD_STAGE_OVERWORLD_ULTIMA_HW02:
    case WORLD_STAGE_OVERWORLD_RUBY:
    case WORLD_STAGE_OVERWORLD_RUBY_HW02:
    case WORLD_STAGE_WARSHIP_LATE:
        g_pWorldMusicTrackTable = g_WorldMusicTrackRecord1;
        break;
    case WORLD_STAGE_UNDERWATER:
        g_pWorldMusicTrackTable = (unsigned char *)&g_WorldMusicTrackRecord2;
        break;
    case WORLD_STAGE_SNOWFIELD:
        g_pWorldMusicTrackTable = (unsigned char *)&g_WorldEffectConfig;
    }
}
} /* extern "C" */
