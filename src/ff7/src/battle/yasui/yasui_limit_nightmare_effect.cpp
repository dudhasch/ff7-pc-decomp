// src/ff7/src/battle/yasui/yasui_limit_nightmare_effect.cpp
// Limit Break [67] (Vincent Lv 3 Hellmasker): Nightmare (Japanese: ナイトメア, dev tag: "limit2/night" / Night)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Night limit break effect: an expanding dome model, trailing particle streams, spinning
//   Gouraud-shaded models and animated sprite bursts.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_boss.h"
#include "ff7/battle_tasks.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/codegen.h"
extern "C" {

struct GfxPsxShortVertexSlot;
struct GfxPsxSpriteQuadState;
struct BattleFxModelDraw;
struct BattleFxVector3;
// clang-format off
// GLOBAL: FF7 0x007ec490
unsigned char g_YasuiLimitNightmareMeshHeader1[1560] = {0x40, 0x02, 0x00, 0x00, 0x00, 0x00, 0xd6, 0xfd, 0x8d, 0xfc, 0x00, 0x00, 0xe4, 0x00, 0xd6, 0xfd, 0xab, 0xfc, 0x00, 0x00, 0xc2, 0x00, 0x00, 0x00, 0x2c, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0xfd, 0x00, 0x00, 0xb9, 0x01, 0xd6, 0xfd, 0x03, 0xfd, 0x00, 0x00, 0x77, 0x01, 0x00, 0x00, 0x77, 0xfd, 0x00, 0x00, 0x70, 0x02, 0xd6, 0xfd, 0x90, 0xfd, 0x00, 0x00, 0x12, 0x02, 0x00, 0x00, 0xee, 0xfd, 0x00, 0x00, 0xfd, 0x02, 0xd6, 0xfd, 0x47, 0xfe, 0x00, 0x00, 0x89, 0x02, 0x00, 0x00, 0x89, 0xfe, 0x00, 0x00, 0x55, 0x03, 0xd6, 0xfd, 0x1c, 0xff, 0x00, 0x00, 0xd4, 0x02, 0x00, 0x00, 0x3e, 0xff, 0x00, 0x00, 0x73, 0x03, 0xd6, 0xfd, 0x00, 0x00, 0x00, 0x00, 0xee, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x03, 0xd6, 0xfd, 0xe4, 0x00, 0x00, 0x00, 0xd4, 0x02, 0x00, 0x00, 0xc2, 0x00, 0x00, 0x00, 0xfd, 0x02, 0xd6, 0xfd, 0xb9, 0x01, 0x00, 0x00, 0x89, 0x02, 0x00, 0x00, 0x77, 0x01, 0x00, 0x00, 0x70, 0x02, 0xd6, 0xfd, 0x70, 0x02, 0x00, 0x00, 0x12, 0x02, 0x00, 0x00, 0x12, 0x02, 0x00, 0x00, 0xb9, 0x01, 0xd6, 0xfd, 0xfd, 0x02, 0x00, 0x00, 0x77, 0x01, 0x00, 0x00, 0x89, 0x02, 0x00, 0x00, 0xe4, 0x00, 0xd6, 0xfd, 0x55, 0x03, 0x00, 0x00, 0xc2, 0x00, 0x00, 0x00, 0xd4, 0x02, 0x00, 0x00, 0x00, 0x00, 0xd6, 0xfd, 0x73, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xee, 0x02, 0x00, 0x00, 0x1c, 0xff, 0xd6, 0xfd, 0x55, 0x03, 0x00, 0x00, 0x3e, 0xff, 0x00, 0x00, 0xd4, 0x02, 0x00, 0x00, 0x47, 0xfe, 0xd6, 0xfd, 0xfd, 0x02, 0x00, 0x00, 0x89, 0xfe, 0x00, 0x00, 0x89, 0x02, 0x00, 0x00, 0x90, 0xfd, 0xd6, 0xfd, 0x70, 0x02, 0x00, 0x00, 0xee, 0xfd, 0x00, 0x00, 0x12, 0x02, 0x00, 0x00, 0x03, 0xfd, 0xd6, 0xfd, 0xb9, 0x01, 0x00, 0x00, 0x77, 0xfd, 0x00, 0x00, 0x76, 0x01, 0x00, 0x00, 0xab, 0xfc, 0xd6, 0xfd, 0xe4, 0x00, 0x00, 0x00, 0x2c, 0xfd, 0x00, 0x00, 0xc2, 0x00, 0x00, 0x00, 0x8d, 0xfc, 0xd6, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x12, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xab, 0xfc, 0xd6, 0xfd, 0x1c, 0xff, 0x00, 0x00, 0x2c, 0xfd, 0x00, 0x00, 0x3e, 0xff, 0x00, 0x00, 0x03, 0xfd, 0xd6, 0xfd, 0x47, 0xfe, 0x00, 0x00, 0x77, 0xfd, 0x00, 0x00, 0x8a, 0xfe, 0x00, 0x00, 0x90, 0xfd, 0xd6, 0xfd, 0x90, 0xfd, 0x00, 0x00, 0xee, 0xfd, 0x00, 0x00, 0xee, 0xfd, 0x00, 0x00, 0x47, 0xfe, 0xd6, 0xfd, 0x03, 0xfd, 0x00, 0x00, 0x89, 0xfe, 0x00, 0x00, 0x77, 0xfd, 0x00, 0x00, 0x1c, 0xff, 0xd6, 0xfd, 0xab, 0xfc, 0x00, 0x00, 0x3e, 0xff, 0x00, 0x00, 0x2c, 0xfd, 0x00, 0x00, 0xd6, 0x03, 0xad, 0xfb, 0x07, 0x01, 0x00, 0x00, 0x71, 0x03, 0xad, 0xfb, 0xfc, 0x01, 0x00, 0x00, 0xf9, 0x03, 0xad, 0xfb, 0x00, 0x00, 0x00, 0x00, 0xf9, 0xfe, 0xad, 0xfb, 0x2a, 0xfc, 0x00, 0x00, 0x00, 0x00, 0xad, 0xfb, 0x07, 0xfc, 0x00, 0x00, 0x04, 0xfe, 0xad, 0xfb, 0x8f, 0xfc, 0x00, 0x00, 0x31, 0xfd, 0xad, 0xfb, 0x31, 0xfd, 0x00, 0x00, 0x8f, 0xfc, 0xad, 0xfb, 0x04, 0xfe, 0x00, 0x00, 0x2a, 0xfc, 0xad, 0xfb, 0xf9, 0xfe, 0x00, 0x00, 0x07, 0xfc, 0xad, 0xfb, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xfc, 0xad, 0xfb, 0x07, 0x01, 0x00, 0x00, 0x8f, 0xfc, 0xad, 0xfb, 0xfc, 0x01, 0x00, 0x00, 0x31, 0xfd, 0xad, 0xfb, 0xcf, 0x02, 0x00, 0x00, 0x04, 0xfe, 0xad, 0xfb, 0x71, 0x03, 0x00, 0x00, 0xf9, 0xfe, 0xad, 0xfb, 0xd6, 0x03, 0x00, 0x00, 0x00, 0x00, 0xad, 0xfb, 0xf9, 0x03, 0x00, 0x00, 0x07, 0x01, 0xad, 0xfb, 0xd6, 0x03, 0x00, 0x00, 0xfc, 0x01, 0xad, 0xfb, 0x71, 0x03, 0x00, 0x00, 0xcf, 0x02, 0xad, 0xfb, 0xcf, 0x02, 0x00, 0x00, 0xd6, 0x03, 0xad, 0xfb, 0xf9, 0xfe, 0x00, 0x00, 0x71, 0x03, 0xad, 0xfb, 0x04, 0xfe, 0x00, 0x00, 0xcf, 0x02, 0xad, 0xfb, 0x31, 0xfd, 0x00, 0x00, 0xfc, 0x01, 0xad, 0xfb, 0x8f, 0xfc, 0x00, 0x00, 0x07, 0x01, 0xad, 0xfb, 0x2a, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x2d, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x18, 0x00, 0x10, 0x00, 0x7f, 0xc0, 0x08, 0x79, 0x9a, 0xc0, 0x7f, 0xfe, 0x96, 0xfe, 0x80, 0x00, 0x08, 0x00, 0x20, 0x00, 0x10, 0x00, 0x28, 0x00, 0x9a, 0xc0, 0x08, 0x79, 0xb5, 0xc0, 0x96, 0xfe, 0xac, 0xfe, 0x80, 0x00, 0x20, 0x00, 0x30, 0x00, 0x28, 0x00, 0x38, 0x00, 0xb5, 0xc0, 0x08, 0x79, 0xcd, 0xc0, 0xac, 0xfe, 0xc1, 0xfe, 0x80, 0x00, 0x30, 0x00, 0x40, 0x00, 0x38, 0x00, 0x48, 0x00, 0xcd, 0xc0, 0x08, 0x79, 0xde, 0xc0, 0xc1, 0xfe, 0xce, 0xfe, 0x80, 0x00, 0x40, 0x00, 0x50, 0x00, 0x48, 0x00, 0x58, 0x00, 0xde, 0xc0, 0x08, 0x79, 0xe8, 0xc0, 0xce, 0xfe, 0xd9, 0xfe, 0x80, 0x00, 0x50, 0x00, 0x60, 0x00, 0x58, 0x00, 0x68, 0x00, 0xe8, 0xc0, 0x08, 0x79, 0xed, 0xc0, 0xd9, 0xfe, 0xdc, 0xfe, 0x80, 0x00, 0x60, 0x00, 0x70, 0x00, 0x68, 0x00, 0x78, 0x00, 0xed, 0xc0, 0x08, 0x79, 0xe8, 0xc0, 0xdc, 0xfe, 0xd9, 0xfe, 0x80, 0x00, 0x70, 0x00, 0x80, 0x00, 0x78, 0x00, 0x88, 0x00, 0xe8, 0xc0, 0x08, 0x79, 0xde, 0xc0, 0xd9, 0xfe, 0xce, 0xfe, 0x80, 0x00, 0x80, 0x00, 0x90, 0x00, 0x88, 0x00, 0x98, 0x00, 0xde, 0xc0, 0x08, 0x79, 0xcd, 0xc0, 0xce, 0xfe, 0xc1, 0xfe, 0x80, 0x00, 0x90, 0x00, 0xa0, 0x00, 0x98, 0x00, 0xa8, 0x00, 0xcd, 0xc0, 0x08, 0x79, 0xb5, 0xc0, 0xc1, 0xfe, 0xac, 0xfe, 0x80, 0x00, 0xa0, 0x00, 0xb0, 0x00, 0xa8, 0x00, 0xb8, 0x00, 0xb5, 0xc0, 0x08, 0x79, 0x9a, 0xc0, 0xac, 0xfe, 0x96, 0xfe, 0x80, 0x00, 0xb0, 0x00, 0xc0, 0x00, 0xb8, 0x00, 0xc8, 0x00, 0x9a, 0xc0, 0x08, 0x79, 0x7d, 0xc0, 0x96, 0xfe, 0x7d, 0xfe, 0x80, 0x00, 0xc0, 0x00, 0xd0, 0x00, 0xc8, 0x00, 0xd8, 0x00, 0x7d, 0xc0, 0x08, 0x79, 0x62, 0xc0, 0x7d, 0xfe, 0x65, 0xfe, 0x80, 0x00, 0xd0, 0x00, 0xe0, 0x00, 0xd8, 0x00, 0xe8, 0x00, 0x62, 0xc0, 0x08, 0x79, 0x47, 0xc0, 0x65, 0xfe, 0x4f, 0xfe, 0x80, 0x00, 0xe0, 0x00, 0xf0, 0x00, 0xe8, 0x00, 0xf8, 0x00, 0x47, 0xc0, 0x08, 0x79, 0x2f, 0xc0, 0x4f, 0xfe, 0x3b, 0xfe, 0x80, 0x00, 0xf0, 0x00, 0x00, 0x01, 0xf8, 0x00, 0x08, 0x01, 0x2f, 0xc0, 0x08, 0x79, 0x1e, 0xc0, 0x3b, 0xfe, 0x2d, 0xfe, 0x80, 0x00, 0x00, 0x01, 0x10, 0x01, 0x08, 0x01, 0x18, 0x01, 0x1e, 0xc0, 0x08, 0x79, 0x14, 0xc0, 0x2d, 0xfe, 0x23, 0xfe, 0x80, 0x00, 0x10, 0x01, 0x20, 0x01, 0x18, 0x01, 0x28, 0x01, 0x14, 0xc0, 0x08, 0x79, 0x0f, 0xc0, 0x23, 0xfe, 0x20, 0xfe, 0x80, 0x00, 0x20, 0x01, 0x30, 0x01, 0x28, 0x01, 0x38, 0x01, 0x0f, 0xc0, 0x08, 0x79, 0x14, 0xc0, 0x20, 0xfe, 0x23, 0xfe, 0x80, 0x00, 0x30, 0x01, 0x40, 0x01, 0x38, 0x01, 0x48, 0x01, 0x14, 0xc0, 0x08, 0x79, 0x1e, 0xc0, 0x23, 0xfe, 0x2d, 0xfe, 0x80, 0x00, 0x40, 0x01, 0x50, 0x01, 0x48, 0x01, 0x58, 0x01, 0x1e, 0xc0, 0x08, 0x79, 0x2f, 0xc0, 0x2d, 0xfe, 0x3b, 0xfe, 0x80, 0x00, 0x50, 0x01, 0x60, 0x01, 0x58, 0x01, 0x68, 0x01, 0x2f, 0xc0, 0x08, 0x79, 0x47, 0xc0, 0x3b, 0xfe, 0x4f, 0xfe, 0x80, 0x00, 0x60, 0x01, 0x70, 0x01, 0x68, 0x01, 0x78, 0x01, 0x47, 0xc0, 0x08, 0x79, 0x62, 0xc0, 0x4f, 0xfe, 0x65, 0xfe, 0x80, 0x00, 0x70, 0x01, 0x00, 0x00, 0x78, 0x01, 0x18, 0x00, 0x62, 0xc0, 0x08, 0x79, 0x7f, 0xc0, 0x65, 0xfe, 0x7f, 0xfe, 0x80, 0x00, 0x80, 0x00, 0x70, 0x00, 0x88, 0x01, 0x80, 0x01, 0xde, 0xc0, 0x08, 0x79, 0xe8, 0xc0, 0xeb, 0x80, 0xf9, 0x80, 0x80, 0x00, 0x70, 0x00, 0x60, 0x00, 0x80, 0x01, 0x90, 0x01, 0xe8, 0xc0, 0x08, 0x79, 0xed, 0xc0, 0xf9, 0x80, 0xfe, 0x80, 0x80, 0x00, 0x00, 0x00, 0x70, 0x01, 0xa0, 0x01, 0x98, 0x01, 0x7f, 0xc0, 0x08, 0x79, 0x62, 0xc0, 0x7f, 0x80, 0x5d, 0x80, 0x80, 0x00, 0x70, 0x01, 0x60, 0x01, 0x98, 0x01, 0xa8, 0x01, 0x62, 0xc0, 0x08, 0x79, 0x47, 0xc0, 0x5d, 0x80, 0x3e, 0x80, 0x80, 0x00, 0x60, 0x01, 0x50, 0x01, 0xa8, 0x01, 0xb0, 0x01, 0x47, 0xc0, 0x08, 0x79, 0x2f, 0xc0, 0x3e, 0x80, 0x23, 0x80, 0x80, 0x00, 0x50, 0x01, 0x40, 0x01, 0xb0, 0x01, 0xb8, 0x01, 0x2f, 0xc0, 0x08, 0x79, 0x1e, 0xc0, 0x23, 0x80, 0x10, 0x80, 0x80, 0x00, 0x40, 0x01, 0x30, 0x01, 0xb8, 0x01, 0xc0, 0x01, 0x1e, 0xc0, 0x08, 0x79, 0x14, 0xc0, 0x10, 0x80, 0x03, 0x80, 0x80, 0x00, 0x30, 0x01, 0x20, 0x01, 0xc0, 0x01, 0xc8, 0x01, 0x14, 0xc0, 0x08, 0x79, 0x0f, 0xc0, 0x03, 0x80, 0x00, 0x80, 0x80, 0x00, 0x20, 0x01, 0x10, 0x01, 0xc8, 0x01, 0xd0, 0x01, 0x0f, 0xc0, 0x08, 0x79, 0x14, 0xc0, 0x00, 0x80, 0x03, 0x80, 0x80, 0x00, 0x10, 0x01, 0x00, 0x01, 0xd0, 0x01, 0xd8, 0x01, 0x14, 0xc0, 0x08, 0x79, 0x1e, 0xc0, 0x03, 0x80, 0x10, 0x80, 0x80, 0x00, 0x00, 0x01, 0xf0, 0x00, 0xd8, 0x01, 0xe0, 0x01, 0x1e, 0xc0, 0x08, 0x79, 0x2f, 0xc0, 0x10, 0x80, 0x23, 0x80, 0x80, 0x00, 0xf0, 0x00, 0xe0, 0x00, 0xe0, 0x01, 0xe8, 0x01, 0x2f, 0xc0, 0x08, 0x79, 0x47, 0xc0, 0x23, 0x80, 0x3e, 0x80, 0x80, 0x00, 0xe0, 0x00, 0xd0, 0x00, 0xe8, 0x01, 0xf0, 0x01, 0x47, 0xc0, 0x08, 0x79, 0x62, 0xc0, 0x3e, 0x80, 0x5d, 0x80, 0x80, 0x00, 0xd0, 0x00, 0xc0, 0x00, 0xf0, 0x01, 0xf8, 0x01, 0x62, 0xc0, 0x08, 0x79, 0x7d, 0xc0, 0x5d, 0x80, 0x7d, 0x80, 0x80, 0x00, 0xc0, 0x00, 0xb0, 0x00, 0xf8, 0x01, 0x00, 0x02, 0x7d, 0xc0, 0x08, 0x79, 0x9a, 0xc0, 0x7d, 0x80, 0x9f, 0x80, 0x80, 0x00, 0xb0, 0x00, 0xa0, 0x00, 0x00, 0x02, 0x08, 0x02, 0x9a, 0xc0, 0x08, 0x79, 0xb5, 0xc0, 0x9f, 0x80, 0xbd, 0x80, 0x80, 0x00, 0xa0, 0x00, 0x90, 0x00, 0x08, 0x02, 0x10, 0x02, 0xb5, 0xc0, 0x08, 0x79, 0xcd, 0xc0, 0xbd, 0x80, 0xd9, 0x80, 0x80, 0x00, 0x90, 0x00, 0x80, 0x00, 0x10, 0x02, 0x88, 0x01, 0xcd, 0xc0, 0x08, 0x79, 0xde, 0xc0, 0xd9, 0x80, 0xeb, 0x80, 0x80, 0x00, 0x60, 0x00, 0x50, 0x00, 0x90, 0x01, 0x18, 0x02, 0xed, 0xc0, 0x08, 0x79, 0xe8, 0xc0, 0xfe, 0x80, 0xf9, 0x80, 0x80, 0x00, 0x50, 0x00, 0x40, 0x00, 0x18, 0x02, 0x20, 0x02, 0xe8, 0xc0, 0x08, 0x79, 0xde, 0xc0, 0xf9, 0x80, 0xeb, 0x80, 0x80, 0x00, 0x40, 0x00, 0x30, 0x00, 0x20, 0x02, 0x28, 0x02, 0xde, 0xc0, 0x08, 0x79, 0xcd, 0xc0, 0xeb, 0x80, 0xd9, 0x80, 0x80, 0x00, 0x30, 0x00, 0x20, 0x00, 0x28, 0x02, 0x30, 0x02, 0xcd, 0xc0, 0x08, 0x79, 0xb5, 0xc0, 0xd9, 0x80, 0xbd, 0x80, 0x80, 0x00, 0x20, 0x00, 0x08, 0x00, 0x30, 0x02, 0x38, 0x02, 0xb5, 0xc0, 0x08, 0x79, 0x9a, 0xc0, 0xbd, 0x80, 0x9f, 0x80, 0x80, 0x00, 0x08, 0x00, 0x00, 0x00, 0x38, 0x02, 0xa0, 0x01, 0x9a, 0xc0, 0x08, 0x79, 0x7f, 0xc0, 0x9f, 0x80, 0x7f, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x007ecaa8
unsigned char g_YasuiLimitNightmareMeshHeader2[1368] = {0xb0, 0x01, 0x00, 0x00, 0x00, 0x00, 0xd9, 0x00, 0x26, 0xfb, 0x00, 0x00, 0xdb, 0x01, 0xd9, 0x00, 0x85, 0xfb, 0x00, 0x00, 0x00, 0x00, 0x47, 0x01, 0x53, 0xfb, 0x00, 0x00, 0x6e, 0x03, 0xd9, 0x00, 0x92, 0xfc, 0x00, 0x00, 0xdb, 0x01, 0xd9, 0x00, 0x7c, 0x04, 0x00, 0x00, 0x00, 0x00, 0xd9, 0x00, 0xda, 0x04, 0x00, 0x00, 0x00, 0x00, 0x47, 0x01, 0xad, 0x04, 0x00, 0x00, 0x23, 0xfe, 0xd9, 0x00, 0x7a, 0x04, 0x00, 0x00, 0x36, 0xfe, 0x47, 0x01, 0x52, 0x04, 0x00, 0x00, 0x94, 0xfc, 0xd9, 0x00, 0x6f, 0x03, 0x00, 0x00, 0xb2, 0xfc, 0x47, 0x01, 0x4e, 0x03, 0x00, 0x00, 0x84, 0xfb, 0xd9, 0x00, 0xdb, 0x01, 0x00, 0x00, 0xae, 0xfb, 0x47, 0x01, 0xca, 0x01, 0x00, 0x00, 0x26, 0xfb, 0xd9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x53, 0xfb, 0x47, 0x01, 0x00, 0x00, 0x00, 0x00, 0x85, 0xfb, 0xd9, 0x00, 0x25, 0xfe, 0x00, 0x00, 0xae, 0xfb, 0x47, 0x01, 0x36, 0xfe, 0x00, 0x00, 0x8f, 0xfc, 0xd9, 0x00, 0x93, 0xfc, 0x00, 0x00, 0xb2, 0xfc, 0x47, 0x01, 0xb2, 0xfc, 0x00, 0x00, 0x27, 0xfe, 0xd9, 0x00, 0x83, 0xfb, 0x00, 0x00, 0x36, 0xfe, 0x47, 0x01, 0xae, 0xfb, 0x00, 0x00, 0x7b, 0xfc, 0x00, 0x00, 0x71, 0xfc, 0x00, 0x00, 0x5c, 0xfb, 0x00, 0x00, 0x14, 0xfe, 0x00, 0x00, 0x87, 0xfb, 0x26, 0xff, 0x23, 0xfe, 0x00, 0x00, 0x8f, 0xfc, 0x26, 0xff, 0x95, 0xfc, 0x00, 0x00, 0xfb, 0xfa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x26, 0xfb, 0x26, 0xff, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xf8, 0xfa, 0x00, 0x00, 0x0a, 0xfe, 0x00, 0x00, 0x61, 0xfb, 0x00, 0x00, 0x27, 0xfe, 0x27, 0xff, 0x82, 0xfb, 0x00, 0x00, 0x00, 0x00, 0x26, 0xff, 0x26, 0xfb, 0x00, 0x00, 0x5d, 0xfb, 0x00, 0x00, 0xeb, 0x01, 0x00, 0x00, 0x84, 0xfb, 0x26, 0xff, 0xdb, 0x01, 0x00, 0x00, 0x73, 0xfc, 0x00, 0x00, 0x8c, 0x03, 0x00, 0x00, 0x92, 0xfc, 0x27, 0xff, 0x6e, 0x03, 0x00, 0x00, 0x14, 0xfe, 0x00, 0x00, 0xa3, 0x04, 0x00, 0x00, 0x25, 0xfe, 0x26, 0xff, 0x7b, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x26, 0xff, 0xda, 0x04, 0x00, 0x00, 0xeb, 0x01, 0x00, 0x00, 0xa3, 0x04, 0x00, 0x00, 0xdb, 0x01, 0x26, 0xff, 0x7c, 0x04, 0x00, 0x00, 0x8d, 0x03, 0x00, 0x00, 0x74, 0xfc, 0x00, 0x00, 0x6e, 0x03, 0x26, 0xff, 0x92, 0xfc, 0x00, 0x00, 0xea, 0x01, 0x00, 0x00, 0x5e, 0xfb, 0x00, 0x00, 0xdb, 0x01, 0x26, 0xff, 0x85, 0xfb, 0x00, 0x00, 0xb2, 0xfc, 0xb9, 0xfe, 0xb2, 0xfc, 0x00, 0x00, 0x36, 0xfe, 0xb9, 0xfe, 0xae, 0xfb, 0x00, 0x00, 0xae, 0xfb, 0xb9, 0xfe, 0x36, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xb9, 0xfe, 0x53, 0xfb, 0x00, 0x00, 0x53, 0xfb, 0xb9, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xae, 0xfb, 0xb9, 0xfe, 0xca, 0x01, 0x00, 0x00, 0xb2, 0xfc, 0xb9, 0xfe, 0x4e, 0x03, 0x00, 0x00, 0x36, 0xfe, 0xb9, 0xfe, 0x52, 0x04, 0x00, 0x00, 0x00, 0x00, 0xb9, 0xfe, 0xad, 0x04, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x28, 0x00, 0x38, 0x00, 0x30, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x38, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x48, 0x00, 0x40, 0x00, 0x50, 0x00, 0x20, 0x20, 0x20, 0x38, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x00, 0x58, 0x00, 0x50, 0x00, 0x60, 0x00, 0x30, 0x30, 0x30, 0x38, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0x00, 0x68, 0x00, 0x60, 0x00, 0x70, 0x00, 0x40, 0x40, 0x40, 0x38, 0x50, 0x50, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x00, 0x78, 0x00, 0x70, 0x00, 0x80, 0x00, 0x50, 0x50, 0x50, 0x38, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x88, 0x00, 0x80, 0x00, 0x90, 0x00, 0x60, 0x60, 0x60, 0x38, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x00, 0x98, 0x00, 0x90, 0x00, 0xa0, 0x00, 0x40, 0x40, 0x40, 0x38, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0xa0, 0x00, 0x10, 0x00, 0x20, 0x20, 0x20, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0xb0, 0x00, 0xc0, 0x00, 0xb8, 0x00, 0x80, 0x80, 0x80, 0x38, 0x70, 0x70, 0x70, 0x00, 0x40, 0x40, 0x40, 0x00, 0x60, 0x60, 0x60, 0x00, 0xb0, 0x00, 0xc8, 0x00, 0xb8, 0x00, 0xd0, 0x00, 0x70, 0x70, 0x70, 0x38, 0x60, 0x60, 0x60, 0x00, 0x60, 0x60, 0x60, 0x00, 0x50, 0x50, 0x50, 0x00, 0xd8, 0x00, 0xe0, 0x00, 0xf0, 0x00, 0xe8, 0x00, 0x40, 0x40, 0x40, 0x38, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x20, 0x00, 0xe0, 0x00, 0xa8, 0x00, 0xe8, 0x00, 0xc0, 0x00, 0x60, 0x60, 0x60, 0x38, 0x80, 0x80, 0x80, 0x00, 0x20, 0x20, 0x20, 0x00, 0x40, 0x40, 0x40, 0x00, 0xc8, 0x00, 0xf8, 0x00, 0xd0, 0x00, 0x00, 0x01, 0x60, 0x60, 0x60, 0x38, 0x50, 0x50, 0x50, 0x00, 0x50, 0x50, 0x50, 0x00, 0x40, 0x40, 0x40, 0x00, 0xf8, 0x00, 0x08, 0x01, 0x00, 0x01, 0x10, 0x01, 0x50, 0x50, 0x50, 0x38, 0x40, 0x40, 0x40, 0x00, 0x40, 0x40, 0x40, 0x00, 0x30, 0x30, 0x30, 0x00, 0x08, 0x01, 0x18, 0x01, 0x10, 0x01, 0x20, 0x01, 0x40, 0x40, 0x40, 0x38, 0x30, 0x30, 0x30, 0x00, 0x30, 0x30, 0x30, 0x00, 0x20, 0x20, 0x20, 0x00, 0x18, 0x01, 0x28, 0x01, 0x20, 0x01, 0x30, 0x01, 0x30, 0x30, 0x30, 0x38, 0x20, 0x20, 0x20, 0x00, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x01, 0x38, 0x01, 0x30, 0x01, 0x40, 0x01, 0x20, 0x20, 0x20, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x01, 0x58, 0x01, 0x50, 0x01, 0x60, 0x01, 0x00, 0x00, 0x00, 0x38, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0x01, 0xd8, 0x00, 0x60, 0x01, 0xf0, 0x00, 0x20, 0x20, 0x20, 0x38, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe8, 0x00, 0xc0, 0x00, 0x70, 0x01, 0x68, 0x01, 0x20, 0x20, 0x20, 0x38, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0xb8, 0x00, 0x68, 0x01, 0x78, 0x01, 0x40, 0x40, 0x40, 0x38, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0xe8, 0x00, 0x80, 0x01, 0x70, 0x01, 0x00, 0x00, 0x00, 0x38, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb8, 0x00, 0xd0, 0x00, 0x78, 0x01, 0x88, 0x01, 0x60, 0x60, 0x60, 0x38, 0x50, 0x50, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd0, 0x00, 0x00, 0x01, 0x88, 0x01, 0x90, 0x01, 0x50, 0x50, 0x50, 0x38, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0x01, 0x90, 0x01, 0x98, 0x01, 0x40, 0x40, 0x40, 0x38, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x01, 0x20, 0x01, 0x98, 0x01, 0xa0, 0x01, 0x30, 0x30, 0x30, 0x38, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x01, 0x30, 0x01, 0xa0, 0x01, 0xa8, 0x01, 0x20, 0x20, 0x20, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x98, 0x00, 0x88, 0x00, 0xe0, 0x00, 0xa8, 0x00, 0x20, 0x20, 0x20, 0x38, 0x40, 0x40, 0x40, 0x00, 0x60, 0x60, 0x60, 0x00, 0x80, 0x80, 0x80, 0x00, 0x88, 0x00, 0x78, 0x00, 0xa8, 0x00, 0xb0, 0x00, 0x40, 0x40, 0x40, 0x38, 0x60, 0x60, 0x60, 0x00, 0x80, 0x80, 0x80, 0x00, 0x70, 0x70, 0x70, 0x00, 0x08, 0x00, 0x00, 0x00, 0x58, 0x01, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x20, 0x00, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 0x98, 0x00, 0xd8, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x38, 0x20, 0x20, 0x20, 0x00, 0x40, 0x40, 0x40, 0x00, 0x60, 0x60, 0x60, 0x00, 0x18, 0x00, 0x08, 0x00, 0x48, 0x01, 0x58, 0x01, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x20, 0x00, 0x28, 0x00, 0x20, 0x00, 0x28, 0x01, 0x38, 0x01, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x28, 0x00, 0x18, 0x01, 0x28, 0x01, 0x20, 0x20, 0x20, 0x38, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x30, 0x00, 0x20, 0x20, 0x20, 0x00, 0x48, 0x00, 0x38, 0x00, 0x08, 0x01, 0x18, 0x01, 0x30, 0x30, 0x30, 0x38, 0x20, 0x20, 0x20, 0x00, 0x40, 0x40, 0x40, 0x00, 0x30, 0x30, 0x30, 0x00, 0x58, 0x00, 0x48, 0x00, 0xf8, 0x00, 0x08, 0x01, 0x40, 0x40, 0x40, 0x38, 0x30, 0x30, 0x30, 0x00, 0x50, 0x50, 0x50, 0x00, 0x40, 0x40, 0x40, 0x00, 0x68, 0x00, 0x58, 0x00, 0xc8, 0x00, 0xf8, 0x00, 0x50, 0x50, 0x50, 0x38, 0x40, 0x40, 0x40, 0x00, 0x60, 0x60, 0x60, 0x00, 0x50, 0x50, 0x50, 0x00, 0x78, 0x00, 0x68, 0x00, 0xb0, 0x00, 0xc8, 0x00, 0x60, 0x60, 0x60, 0x38, 0x50, 0x50, 0x50, 0x00, 0x70, 0x70, 0x70, 0x00, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
// clang-format on

// Eight 20-byte effect resource records, read through their base. Their names are this TU's first
// string literals (parse-time units before the code's literals).
typedef struct {
    int id;
    char *name;
    char *name2;
    int pad0;
    int pad1;
} EffectResourceRecord;
// clang-format off
// GLOBAL: FF7 0x007ed000
EffectResourceRecord g_YasuiLimitNightmareResourceDef[8] = {
    {0x192a39, 
        // STRING: FF7 0x007ed0d0
        "SPARK_1.TIM", 
        // STRING: FF7 0x007ed0dc
        "SPARK_1.SPT", 0, 0},
    {0x192a39, 
        // STRING: FF7 0x007ed0e8
        "SMOKE_X.TIM", 
        // STRING: FF7 0x007ed0f4
        "SMOKE_X.SPT", 0, 0},
    {0x6400b1, 
        // STRING: FF7 0x007ed100
        "satan_2.rsd", 0, 0, 0},
    {0x4b00b1, 
        // STRING: FF7 0x007ed10c
        "air_1.rsd", 0, 0, 0},
    {0x197c3c, 
        // STRING: FF7 0x007ed118
        "air_1.TIM", 0, 0, 0},
    {0x197c39, 
        // STRING: FF7 0x007ed124
        "air_1.TIM", 0, 0, 0},
    {0x19783c, 
        // STRING: FF7 0x007ed130
        "air_1.TIM", 0, 0, 0},
    {0x197839, 
        // STRING: FF7 0x007ed13c
        "air_1.TIM", 0, 0, 0},
};
// GLOBAL: FF7 0x007ed0a0
struct BattleFxVector3 g_YasuiLimitNightmareSpriteRequest = {0, 0, -0x1000, {0}, 0, 0, -128, -128, -128, {0x2e}, 0, {0}, 0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, 0};
// clang-format on

#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
#pragma intrinsic(abs)


// clang-format off
// GLOBAL: FF7 0x00a41018
int g_YasuiLimitNightmareTargetActor = 0;
unsigned char g_bssPad_00a4101c[4] = {0};
// GLOBAL: FF7 0x00a41020
BattleFxLimitNightmareTrailRing g_YasuiLimitNightmareTrailRingBuffers[8] = {0};
// GLOBAL: FF7 0x00a41360
SVECTOR g_YasuiLimitNightmareTrailPoints[8][20] = {0};
// GLOBAL: FF7 0x00a41860
int g_YasuiLimitNightmareTargetMask = 0;
// GLOBAL: FF7 0x00a41864
int g_YasuiLimitNightmareSourceActor = 0;
// GLOBAL: FF7 0x00a41868
YasuiNightQuadSortEntry g_YasuiLimitNightmareQuadSortTable1[2] = {0};
unsigned char g_bssPad_00a41870[88] = {0};
// GLOBAL: FF7 0x00a418c8
YasuiNightQuadSortEntry g_YasuiLimitNightmareQuadSortTable2[2] = {0};
unsigned char g_bssPad_00a418d0[88] = {0};
// GLOBAL: FF7 0x00a41928
struct GfxTableCursor *g_YasuiLimitNightmareLoadedResTable = 0;
unsigned char g_bssPad_00a4192c[4] = {0};
// clang-format on

/*
 * Loads the "limit2/night" resource package for the Night limit break.
 */
// FUNCTION: FF7 0x00461660
void __cdecl BattleFx_LimitLoadResNightmare(void)
{
    GfxModelTableBank *parent;
    GfxResourceRequest requestBuffer;
    int resultType;
    char pathBuffer[200];
    GfxResourceRequest *request;
    int resDesc;

    if (g_pBattle3dScene != 0 && g_YasuiLimitNightmareLoadedResTable == 0) {
        resultType = 8;
        resDesc = (int)&g_YasuiLimitNightmareResourceDef;
        parent = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestBuffer;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x007ed148
        request->resourcePath = BattleFx_BuildResourcePath("limit2/night", pathBuffer);
        Gfx_LoadModelTableIfAbsent(resultType, resDesc, request, &g_YasuiLimitNightmareLoadedResTable, parent);
    }
}

/*
 * Entry point for Night limit effect; loads resources and begins effect.
 */
// FUNCTION: FF7 0x004616f0
int __cdecl BattleFx_LimitEnterNightmare(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResNightmare();
    BattleFx_LimitBeginNightmare(targetMask, actorIdx);
    return 0;
}

/*
 * Sets up render state, records target/actor indices, and spawns timeline task.
 */
// FUNCTION: FF7 0x0046170c
void __cdecl BattleFx_LimitBeginNightmare(int targetMask, int actorIdx)
{
    int actorIndex;
    int targetCnt;

    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    g_BattleFxActiveDrawListPtr = Gfx_GetPsxPacketScratchPtr();
    g_YasuiLimitNightmareTargetMask = targetMask;
    actorIndex = 0;
    for (targetCnt = 0; actorIndex < 10; actorIndex++) {
        if ((targetMask & 1 << actorIndex) != 0) {
            targetCnt++;
            if (targetCnt == 1) {
                g_YasuiLimitNightmareTargetActor = actorIndex;
            }
        }
    }
    g_YasuiLimitNightmareSourceActor = actorIdx;
    Battle_AllocateResourceSlot(BattleFx_LimitNightmareSpawnTimeline);
}

/*
 * Retires current task slot, spawns timeline task at target bone, and prepares mesh UVs.
 */
// FUNCTION: FF7 0x004617a7
void __cdecl BattleFx_LimitNightmareSpawnTimeline(void)
{
    int size;
    BattleFxNightmareUvScratch *pUvScratch;
    BattleFxBlueDeathForceTask *task;

    task = (BattleFxBlueDeathForceTask *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    task = (BattleFxBlueDeathForceTask *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitNightmareTimelineTask);
    Battle_GetBoneWorldPos(g_YasuiLimitNightmareTargetActor, g_BattleActorRecords[g_YasuiLimitNightmareTargetActor].targetBone, &task->posX);
    size = g_BattleActorRecords[g_YasuiLimitNightmareTargetActor].radius + 432;
    if (size > 1900) {
        size = 1900;
    }
    task->radius = size;
    pUvScratch = (BattleFxNightmareUvScratch *)Gfx_GetPsxScratchpadPtr(0);
    pUvScratch->dataPtr = (int)&g_YasuiLimitNightmareMeshHeader1;
    BattleFx_LimitNightmarePrepareMeshUvBlock(pUvScratch);
}

/*
 * Initializes mesh block header and assigns quad UVs by angle.
 */
// FUNCTION: FF7 0x00461869
int __cdecl BattleFx_LimitNightmarePrepareMeshUvBlock(BattleFxNightmareUvScratch *block)
{
    int cursor;
    int base;

    base = block->dataPtr + 4;
    cursor = base + *(int *)block->dataPtr;
    block->highWordMask = *(int *)cursor & 0xFFFF0000;
    cursor += 4;
    cursor = BattleFx_LimitNightmareAssignQuadUvsByAngle(block, base, cursor);
    return block->unusedReturn;
}

/*
 * Computes polygon centroids and angle-sorts quad UV mappings into two hemispheric groups.
 */
// FUNCTION: FF7 0x004618c1
int __cdecl BattleFx_LimitNightmareAssignQuadUvsByAngle(BattleFxNightmareUvScratch *quadUv, int fragBase, int block)
{
    BattleFxNightmareUvQuadOffsets *cursor;
    int sumX;
    int sumY;
    int i;
    Vec3sPacked *pFragment;
    int sumZ;
    int uvCursor;
    int angleValue;
    int secondCount;
    int numQuads;
    int remCount;

    quadUv->refAxisX = 0;
    quadUv->refAxisY = 0;
    quadUv->refAxisZ = 0xFFFFF000;
    numQuads = *(int *)block;
    uvCursor = 0;
    secondCount = 0;
    cursor = (BattleFxNightmareUvQuadOffsets *)(block + 4);
    for (i = 0; i < numQuads; i++, cursor++) {
        pFragment = (Vec3sPacked *)(fragBase + cursor->fragment0Offset);
        sumX = pFragment->vx;
        sumY = pFragment->vy;
        sumZ = pFragment->vz;
        pFragment = (Vec3sPacked *)(fragBase + cursor->fragment1Offset);
        sumX += pFragment->vx;
        sumY += pFragment->vy;
        sumZ += pFragment->vz;
        pFragment = (Vec3sPacked *)(fragBase + cursor->fragment2Offset);
        sumX += pFragment->vx;
        sumY += pFragment->vy;
        sumZ += pFragment->vz;
        pFragment = (Vec3sPacked *)(fragBase + cursor->fragment3Offset);
        sumX += pFragment->vx;
        sumY += pFragment->vy;
        sumZ += pFragment->vz;
        cursor->u3 = 0;
        cursor->u2 = 0;
        cursor->u1 = 0;
        cursor->u0 = 0;
        sumX /= 4;
        sumY /= 4;
        sumZ /= 4;
        quadUv->sumX = sumX;
        quadUv->sumY = 0;
        quadUv->sumZ = sumZ;
        NormalizeVectorFixed((VECTOR *)&quadUv->sumX, (VECTOR *)&quadUv->sumX);
        angleValue = ComputeAngleBetweenVectors((VECTOR *)&quadUv->refAxisX, (VECTOR *)&quadUv->sumX, (VECTOR *)&quadUv->crossX);
        if (quadUv->crossY > 0) {
            angleValue = 0x1000 - angleValue;
        }
        if (sumY < -555) {
            g_YasuiLimitNightmareQuadSortTable1[uvCursor].f.angle = (short)angleValue;
            g_YasuiLimitNightmareQuadSortTable1[uvCursor].f.quadIndex = (char)i;
            g_YasuiLimitNightmareQuadSortTable1[uvCursor].f.hemisphere = 1;
            uvCursor++;
        } else {
            g_YasuiLimitNightmareQuadSortTable2[secondCount].f.angle = (short)angleValue;
            g_YasuiLimitNightmareQuadSortTable2[secondCount].f.quadIndex = (char)i;
            g_YasuiLimitNightmareQuadSortTable2[secondCount].f.hemisphere = 0;
            secondCount++;
        }
    }
    angleValue = numQuads / 2;
    for (i = 0; i < angleValue - 1; i++) {
        for (uvCursor = i + 1; uvCursor < angleValue; uvCursor++) {
            if (g_YasuiLimitNightmareQuadSortTable1[i].f.angle > g_YasuiLimitNightmareQuadSortTable1[uvCursor].f.angle) {
                secondCount = g_YasuiLimitNightmareQuadSortTable1[i].word;
                g_YasuiLimitNightmareQuadSortTable1[i].word = g_YasuiLimitNightmareQuadSortTable1[uvCursor].word;
                g_YasuiLimitNightmareQuadSortTable1[uvCursor].word = secondCount;
            }
            if (g_YasuiLimitNightmareQuadSortTable2[i].f.angle > g_YasuiLimitNightmareQuadSortTable2[uvCursor].f.angle) {
                secondCount = g_YasuiLimitNightmareQuadSortTable2[i].word;
                g_YasuiLimitNightmareQuadSortTable2[i].word = g_YasuiLimitNightmareQuadSortTable2[uvCursor].word;
                g_YasuiLimitNightmareQuadSortTable2[uvCursor].word = secondCount;
            }
        }
    }
    uvCursor = 0xFF;
    remCount = 256 % angleValue;
    cursor = (BattleFxNightmareUvQuadOffsets *)(block + 4);
    for (i = 0; i < angleValue; i++) {
        sumX = g_YasuiLimitNightmareQuadSortTable1[i].f.quadIndex;
        sumY = g_YasuiLimitNightmareQuadSortTable2[i].f.quadIndex;
        if (i < angleValue - remCount) {
            secondCount = uvCursor - 256 / angleValue;
        } else {
            secondCount = uvCursor - 256 / angleValue - 1;
            if (secondCount < 0) {
                secondCount = 0;
            }
        }
        cursor[sumX].u2 = (char)secondCount;
        cursor[sumX].u0 = (char)secondCount;
        cursor[sumX].u3 = (char)uvCursor;
        cursor[sumX].u1 = (char)uvCursor;
        cursor[sumX].v1 = 0xC0;
        cursor[sumX].v0 = 0xC0;
        cursor[sumX].v3 = 0x80;
        cursor[sumX].v2 = 0x80;
        cursor[sumY].u2 = (char)uvCursor;
        cursor[sumY].u0 = (char)uvCursor;
        cursor[sumY].u3 = (char)secondCount;
        cursor[sumY].u1 = (char)secondCount;
        cursor[sumY].v1 = 0xC0;
        cursor[sumY].v0 = 0xC0;
        cursor[sumY].v3 = 0xFF;
        cursor[sumY].v2 = 0xFF;
        uvCursor = secondCount;
    }
    return (int)cursor;
}

/*
 * Master timeline task controlling screen flash, expanding model, sprite bursts, and trailing models.
 */
// FUNCTION: FF7 0x00461dcf
void __cdecl BattleFx_LimitNightmareTimelineTask(void)
{
    int burstIndex;
    unsigned short flashVal;
    int spreadRange;
    int scaleJitter;
    int scaleVal;
    BattleFxLimitNightmareParticle *particle;
    BattleFxNightmareSlot *task;

    if (g_BattleIsPaused != 0) {
        return;
    }
    task = (BattleFxNightmareSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (task->phase >= 13 && task->phase < 21) {
        if (task->phase == 0xD) {
            BattleFx_StartScreenFlashRamp(1);
        }
        if (task->phase == 0xF) {
            BattleFx_FadeOutScreenFlashRamp(2);
        }
    }
    if (task->phase == 0x15) {
        particle = (BattleFxLimitNightmareParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitNightmareExpandingModelTask);
        *(Int64Pair *)&particle->velocityX = *(Int64Pair *)&task->posX;
        particle->velocityY = 0;
        particle->phase = 0;
        particle->speed = 288;
        particle->scale = (task->elapsed << 12) / 655;
        particle->scaleStep = (particle->scale >> 1) / 22;
        particle->angle = 0;
        particle->scaleStepY = (particle->scale / 3 << 1) / 22;
    }
    if (task->phase >= 36 && task->phase <= 56) {
        scaleVal = task->elapsed / 3 << 1;
        spreadRange = scaleVal;
        if (scaleVal > 0x4F2) {
            scaleVal = 0x4F2;
        }
        scaleVal = (scaleVal << 12) / 256;
        for (burstIndex = 0; burstIndex < 2; burstIndex++) {
            particle = (BattleFxLimitNightmareParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitNightmareAnimatedSpriteTask);
            *(Int64Pair *)&particle->velocityX = *(Int64Pair *)&task->posX;
            particle->velocityX += (short)(rand() % (spreadRange << 1) - spreadRange);
            particle->velocityY += (short)(rand() % (spreadRange << 1) - spreadRange);
            particle->velocityZ += (short)(rand() % (spreadRange << 1) - spreadRange);
            scaleJitter = rand() % 0x900 + 0x600;
            particle->uniformScale = scaleVal * scaleJitter >> 12;
            particle->stepOrTexRow = rand() % 3 << 6;
        }
    }
    if (task->phase >= 50 && task->phase <= 64 && (task->phase & 1) != 0) {
        particle = (BattleFxLimitNightmareParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitNightmareSpinningGradientModelTask);
        *(Int64Pair *)&particle->velocityX = *(Int64Pair *)&task->posX;
        particle->phase = rand() % 4096;
        particle->speed = rand() % 170 + 128;
        if ((particle->phase & 1) != 0) {
            particle->speed = -particle->speed;
        }
        particle->scale = rand() % 0x400 - 0x200;
        particle->angle = (task->phase * 0x2BC & 0xFFF) + rand() % 0x1F4;
        scaleVal = (task->elapsed << 12) / 1350;
        scaleJitter = rand() % 0x600 + 0xB00;
        particle->uniformScale = scaleVal * scaleJitter >> 12;
        particle->stepOrTexRow = scaleVal >> 4;
        particle->lifetime = rand() % 9 + 17;
    }
    if (task->phase == 0x3C) {
        spreadRange = task->elapsed + (task->elapsed >> 1);
        for (burstIndex = 0; burstIndex < 4; burstIndex++) {
            particle = (BattleFxLimitNightmareParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitNightmareTrailingModelTask);
            *(Int64Pair *)&particle->velocityX = *(Int64Pair *)&task->posX;
            particle->velocityY = 0xFF94;
            particle->startDelay = rand() % 8 + burstIndex;
            particle->phase = rand() % 4096;
            particle->speed = rand() % 120 + 70;
            if ((particle->phase & 1) != 0) {
                particle->speed = -particle->speed;
            }
            particle->scale = spreadRange + rand() % 0xC8;
            particle->uniformScale = 0x1851;
            particle->stepOrTexRow = -(rand() % 50 + 28);
            particle->lifetime = burstIndex;
            particle->timer = 0;
            g_YasuiLimitNightmareTrailRingBuffers[burstIndex].count = 0;
            g_YasuiLimitNightmareTrailRingBuffers[burstIndex].tail = 0;
            g_YasuiLimitNightmareTrailRingBuffers[burstIndex].writeIndex = 0;
            g_YasuiLimitNightmareTrailRingBuffers[burstIndex].capacity = 8;
            g_YasuiLimitNightmareTrailRingBuffers[burstIndex].lastIndex = 0xFFFFFFFF;
        }
    }
    if (task->phase == 0x32) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition(&task->posX), 0x16A);
    } else if (task->phase == 0x3C) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH2, Battle_ComputeScreenPanFromPosition(&task->posX), 0x17B);
    } else if (task->phase == 0x55) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH2, Battle_ComputeScreenPanFromPosition(&task->posX), 0);
    }
    if (task->phase < 6) {
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        flashVal = task->phase * 341;
    } else if (task->phase >= 77) {
        flashVal = 0x800 - (task->phase - 0x4D << 8);
    } else {
        flashVal = 0x800;
    }
    if (task->phase == 0x41) {
        Battle_PostActorHitOrHurtMessage(g_YasuiLimitNightmareTargetActor);
    }
    if (task->phase >= 85) {
        flashVal = 0;
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    g_BattleFxTimelineState.startValue = flashVal;
    (task->phase)++;
}

/*
 * Manages trailing particle streams, updates orbital positions, and submits trail billboard sprites.
 */
// FUNCTION: FF7 0x0046245f
void __cdecl BattleFx_LimitNightmareTrailingModelTask(void)
{
    BattleFxModelDraw *pModelDraw;
    VECTOR uniformVec;
    char tintByte;
    VECTOR rotAxis;
    int angle;
    int prevPoint;
    int ringIndex;
    SVECTOR rotAngles;
    char alignMtx[32];
    SVECTOR *pPts;
    BattleFxLimitNightmareTrailRing *ringBuffer;
    VECTOR direction;
    SVECTOR newPoint;
    BattleFxLimitNightmareTrail *task;
    char trailMatrix[32];

    task = (BattleFxLimitNightmareTrail *)BATTLE_CURRENT_TASK_SLOT;
    ringBuffer = &g_YasuiLimitNightmareTrailRingBuffers[task->ringIndex];
    pPts = (SVECTOR *)(task->ringIndex * 0xA0 + (char *)&g_YasuiLimitNightmareTrailPoints);
    if (task->timer < task->limit) {
        if (g_BattleIsPaused == 0) {
            (task->timer)++;
        }
        return;
    }
    rotAngles.vx = 0;
    rotAngles.vy = task->trailY;
    rotAngles.vz = 0;
    RotMatrixXYZ(&rotAngles, trailMatrix);
    newPoint.vx = 0;
    newPoint.vy = 0;
    newPoint.vz = -task->orbitRadius;
    TransformVectorToInt16(trailMatrix, &newPoint, &newPoint);
    newPoint.vx += task->stepX;
    newPoint.vy += task->stepY;
    newPoint.vz += task->stepZ;
    if (g_BattleIsPaused == 0) {
        pPts[ringBuffer->writeIndex] = newPoint;
        BattleFx_LimitNightmarePushTrailPoint(ringBuffer, (int)&pPts[ringBuffer->writeIndex]);
    }
    if (task->stage == 0) {
        if (g_BattleIsPaused == 0) {
            (task->stage)++;
            task->trailY += task->trailYStep;
        }
        return;
    }
    ringIndex = ringBuffer->lastIndex;
    prevPoint = ringBuffer->prevIndex;
    direction.vx = pPts[ringIndex].vx - pPts[prevPoint].vx;
    direction.vy = 0;
    direction.vz = pPts[ringIndex].vz - pPts[prevPoint].vz;
    NormalizeVectorFixed(&direction, &direction);
    angle = ComputeAngleBetweenVectors((VECTOR *)&g_YasuiLimitNightmareSpriteRequest, &direction, &rotAxis);
    Gte_BuildAxisAngleMatrix(angle, (PsxMatrix *)trailMatrix, &rotAxis);
    rotAxis.vx = pPts[ringIndex].vx - pPts[prevPoint].vx;
    rotAxis.vy = pPts[ringIndex].vy - pPts[prevPoint].vy;
    rotAxis.vz = pPts[ringIndex].vz - pPts[prevPoint].vz;
    NormalizeVectorFixed(&rotAxis, &rotAxis);
    angle = ComputeAngleBetweenVectors(&direction, &rotAxis, &rotAxis);
    Gte_BuildAxisAngleMatrix(angle, (PsxMatrix *)alignMtx, &rotAxis);
    ConcatMatrixInPlace(alignMtx, trailMatrix);
    ((PsxMatrix *)trailMatrix)->t[0] = pPts[ringIndex].vx;
    ((PsxMatrix *)trailMatrix)->t[1] = pPts[ringIndex].vy;
    ((PsxMatrix *)trailMatrix)->t[2] = pPts[ringIndex].vz;
    uniformVec.vz = task->uniformScale;
    uniformVec.vy = uniformVec.vz;
    uniformVec.vx = uniformVec.vy;
    ScaleMatrix(trailMatrix, &uniformVec);
    MultiplyMatrix(&g_BattleCameraTransform, trailMatrix, trailMatrix);
    SetRotMatrix(trailMatrix);
    SetTransMatrix(trailMatrix);
    pModelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    pModelDraw->modelGroup = Gfx_ModelTableGetRenderObject(2, g_YasuiLimitNightmareLoadedResTable);
    pModelDraw->flags = 0x88;
    pModelDraw->uvScroll = 0;
    pModelDraw->fadeLevel = 0;
    pModelDraw->stateFlags = 0;
    pModelDraw->texturePageBits = 0;
    if (task->stage < 4) {
        pModelDraw->fadeLevel = 4096 - (task->stage << 9);
        tintByte = (char)(task->stage * 14);
    } else if (task->stage >= 20) {
        ringIndex = task->stage - 20;
        pModelDraw->fadeLevel = (short)(ringIndex << 9);
        tintByte = (char)(0x70 - ringIndex * 14);
    } else {
        pModelDraw->fadeLevel = 0;
        tintByte = 0x70;
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(pModelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    g_YasuiLimitNightmareSpriteRequest.currentIndex = tintByte;
    g_YasuiLimitNightmareSpriteRequest.prevIndex = g_YasuiLimitNightmareSpriteRequest.currentIndex;
    g_YasuiLimitNightmareSpriteRequest.colorR = g_YasuiLimitNightmareSpriteRequest.prevIndex;
    prevPoint = 0x1C20;
    g_YasuiLimitNightmareSpriteRequest.limitSparkVectorAt16 = Gfx_ModelTableGetLoadedRecord(1, g_YasuiLimitNightmareLoadedResTable);
    g_YasuiLimitNightmareSpriteRequest.drawObj = Gfx_ModelTableGetSpriteObject(1, g_YasuiLimitNightmareLoadedResTable);
    ringIndex = ringBuffer->tail;
    for (angle = 0; angle < ringBuffer->count; ringIndex++, angle++) {
        if (ringIndex >= 8) {
            ringIndex = 0;
        }
        g_YasuiLimitNightmareSpriteRequest.frame = (short)(angle * 2);
        BattleFx_InstallBillboardMatrix((short *)ringBuffer->points[ringIndex], (short)prevPoint, -(prevPoint >> 4));
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_YasuiLimitNightmareSpriteRequest.limitSparkVectorAt16, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    }
    if (task->stage >= 28) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->trailY += task->trailYStep;
    task->trailYStep += task->trailYStep >> 6;
    task->stepY += task->accel;
    if (task->stage < 12) {
        task->accel += task->accel >> 4;
    } else {
        task->accel += (short)(task->accel / 12);
    }
    (task->stage)++;
}

/*
 * Pushes a new world-space position into the trailing point ring buffer.
 */
// FUNCTION: FF7 0x00462a3a
void __cdecl BattleFx_LimitNightmarePushTrailPoint(BattleFxLimitNightmareTrailRing *trail, int point)
{
    int tail;
    int head;
    int count;
    int capacity;

    trail->prevIndex = trail->lastIndex;
    trail->lastIndex = trail->writeIndex;
    head = trail->lastIndex;
    tail = trail->tail;
    count = trail->count;
    capacity = trail->capacity;
    trail->points[head] = point;
    head++;
    count++;
    if (head >= capacity) {
        head = 0;
    }
    if (count >= capacity) {
        count = capacity;
    }
    if (head == tail) {
        tail++;
        count--;
        if (tail >= capacity) {
            tail = 0;
        }
    }
    trail->writeIndex = head;
    trail->tail = tail;
    trail->count = count;
    trail->capacity = capacity;
}

/*
 * Updates spinning gradient model animation and submits Gouraud-shaded mesh for rendering.
 */
// FUNCTION: FF7 0x00462b07
void __cdecl BattleFx_LimitNightmareSpinningGradientModelTask(void)
{
    struct {
        PsxMatrix matrix;
        unsigned char pad_1E[0x2];
        BattleFxNightmareSpinGradient *task;
        unsigned char pad_24[0x4];
        BattleFxNightmareQuadState *model;
        int weight;
        int phaseLength;
        SVECTOR rotAngles;
        PsxMatrix tiltMatrix;
        unsigned char pad_5A[0x2];
        VECTOR scale;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.task = (BattleFxNightmareSpinGradient *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotAngles.vx = 0;
    frame.rotAngles.vy = frame.task->angle;
    frame.rotAngles.vz = 0;
    RotMatrixXYZ(&frame.rotAngles, &frame.matrix);
    frame.matrix.t[0] = frame.task->posX;
    frame.matrix.t[1] = frame.task->posY;
    frame.matrix.t[2] = frame.task->posZ;
    frame.rotAngles.vx = frame.task->tiltX;
    frame.rotAngles.vy = 0;
    frame.rotAngles.vz = frame.task->tiltZ;
    RotMatrixXYZ(&frame.rotAngles, &frame.tiltMatrix);
    ConcatMatrixInPlace(&frame.tiltMatrix, &frame.matrix);
    frame.scale.vz = frame.task->spinAngle;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.matrix, &frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    RefreshGlobalTransformState();
    frame.model = (BattleFxNightmareQuadState *)Gfx_GetPsxScratchpadPtr(0);
    frame.model->dataPtr = (int)&g_YasuiLimitNightmareMeshHeader2;
    frame.model->drawList = g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED;
    frame.model->otShift = 0xC;
    frame.model->quadPtr = Gfx_GetPsxPacketScratchPtr();
    frame.phaseLength = frame.task->duration / 3;
    if (frame.task->elapsed < frame.phaseLength) {
        frame.model->baseColor.r = 0x80;
        frame.model->baseColor.g = 8;
        frame.model->baseColor.b = 0x10;
        frame.model->tintColor.r = 0x60;
        frame.model->tintColor.g = 8;
        frame.model->tintColor.b = 0x80;
        frame.weight = 0x1000 / frame.phaseLength * frame.task->elapsed;
    } else if (frame.task->elapsed < frame.phaseLength << 1) {
        frame.model->baseColor.r = 0x60;
        frame.model->baseColor.g = 8;
        frame.model->baseColor.b = 0x80;
        frame.model->tintColor.r = 0x10;
        frame.model->tintColor.g = 8;
        frame.model->tintColor.b = 0x80;
        frame.weight = 0x1000 / frame.phaseLength * (frame.task->elapsed - frame.phaseLength);
    } else if (frame.task->elapsed < 9) {
        frame.model->baseColor.r = 0x10;
        frame.model->baseColor.g = 8;
        frame.model->baseColor.b = 0x80;
        frame.model->tintColor.r = 0x10;
        frame.model->tintColor.g = 8;
        frame.model->tintColor.b = 0x80;
        frame.weight = 0;
    } else {
        frame.model->baseColor.r = 0x10;
        frame.model->baseColor.g = 8;
        frame.model->baseColor.b = 0x80;
        frame.model->tintColor.r = 0;
        frame.model->tintColor.g = 0;
        frame.model->tintColor.b = 0;
        frame.weight = frame.task->elapsed - 9 << 9;
    }
    g_BattleFxActiveDrawListPtr = BattleFx_LimitNightmareDrawPsxWeightedGouraudMesh(frame.model, frame.weight);
    if (frame.task->elapsed >= frame.task->duration) {
        frame.task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.task->angle += frame.task->angleStep;
    frame.task->spinAngle += frame.task->spinSpeed;
    frame.task->spinSpeed += (short)(frame.task->spinSpeed >> 4);
    (frame.task->elapsed)++;
}

/*
 * Prepares weighted Gouraud quad primitive stream for drawing.
 */
// FUNCTION: FF7 0x00462e45
int __cdecl BattleFx_LimitNightmareDrawPsxWeightedGouraudMesh(BattleFxNightmareQuadState *drawState, int weight)
{
    int cursor;
    int base;

    base = drawState->dataPtr + 4;
    cursor = base + *(int *)drawState->dataPtr;
    drawState->highWordMask = *(int *)cursor & 0xFFFF0000;
    cursor += 0xC;
    cursor = BattleFx_LimitNightmareDrawPsxWeightedGouraudQuads(drawState, base, cursor, weight);
    return drawState->quadPtr;
}

/*
 * Submits weighted Gouraud textured quads with distance attenuation.
 */
// FUNCTION: FF7 0x00462ea1
int __cdecl BattleFx_LimitNightmareDrawPsxWeightedGouraudQuads(BattleFxNightmareQuadState *p, int verts, int hdr, int weight)
{
    BattleFxNightmareQuad *facePtr;
    int depthBias;
    int tex;
    BattleFxLimitWeightedQuad *pr;
    int faceIndex;
    int shade;
    int numQuads;

    numQuads = *(int *)hdr;
    depthBias = 0xE - p->otShift;
    facePtr = (BattleFxNightmareQuad *)(hdr + 4);
    pr = (BattleFxLimitWeightedQuad *)p->quadPtr;
    for (faceIndex = 0; faceIndex < numQuads; faceIndex++, facePtr++) {
        p->vertex[0] = *(Int64Pair *)(verts + facePtr->vertexIndex[0]);
        p->vertex[1] = *(Int64Pair *)(verts + facePtr->vertexIndex[1]);
        p->vertex[2] = *(Int64Pair *)(verts + facePtr->vertexIndex[2]);
        p->vertex[3] = *(Int64Pair *)(verts + facePtr->vertexIndex[3]);
        StoreDeferredDrawArgs(&p->vertex[0], &p->vertex[1], &p->vertex[2]);
        ApplyDeferredDrawArgs();
        Gfx_PsxStoreFrontFacing(&p->frontFacing);
        if (p->frontFacing != 0) {
            SetPendingResourceHandle((GfxPsxShortVertexSlot *)&p->vertex[3]);
            ApplyPendingResourceHandle();
            Gfx_ReadTransformOrderingDepth2(&p->orderingDepth);
            tex = facePtr->tex0 & 0xFF;
            if (tex != 0) {
                shade = 0x1000 - (tex << 5);
                SetDrawColorRGB(0, 0, 0);
                Gfx_ShadePsxVertexColor(&p->baseColor, shade, &p->shadeTemp);
                SetDrawColorRGB(p->tintColor.r, p->tintColor.g, p->tintColor.b);
                Gfx_ShadePsxVertexColor(&p->shadeTemp, weight, (CVECTOR *)&pr->color0);
            } else {
                pr->color0 = 0;
            }
            tex = facePtr->tex1 & 0xFF;
            if (tex != 0) {
                shade = 0x1000 - (tex << 5);
                SetDrawColorRGB(0, 0, 0);
                Gfx_ShadePsxVertexColor(&p->baseColor, shade, &p->shadeTemp);
                SetDrawColorRGB(p->tintColor.r, p->tintColor.g, p->tintColor.b);
                Gfx_ShadePsxVertexColor(&p->shadeTemp, weight, (CVECTOR *)&pr->color1);
            } else {
                pr->color1 = 0;
            }
            tex = facePtr->tex2 & 0xFF;
            if (tex != 0) {
                shade = 0x1000 - (tex << 5);
                SetDrawColorRGB(0, 0, 0);
                Gfx_ShadePsxVertexColor(&p->baseColor, shade, &p->shadeTemp);
                SetDrawColorRGB(p->tintColor.r, p->tintColor.g, p->tintColor.b);
                Gfx_ShadePsxVertexColor(&p->shadeTemp, weight, (CVECTOR *)&pr->color2);
            } else {
                pr->color2 = 0;
            }
            tex = facePtr->tex3 & 0xFF;
            if (tex != 0) {
                shade = 0x1000 - (tex << 5);
                SetDrawColorRGB(0, 0, 0);
                Gfx_ShadePsxVertexColor(&p->baseColor, shade, &p->shadeTemp);
                SetDrawColorRGB(p->tintColor.r, p->tintColor.g, p->tintColor.b);
                Gfx_ShadePsxVertexColor(&p->shadeTemp, weight, (CVECTOR *)&pr->color3);
            } else {
                pr->color3 = 0;
            }
            pr->packetTag = (pr->packetTag & 0xFFFFFF) | 0x8000000;
            *((char *)&pr->color0 + 3) = 0x3A;
            Gfx_DrawPsxPrimitive(pr);
        }
    }
    p->quadPtr = (int)pr;
    return (int)facePtr;
}

/*
 * Animates expanding spark/smoke billboard sprites with color fading.
 */
// FUNCTION: FF7 0x004632ae
void __cdecl BattleFx_LimitNightmareAnimatedSpriteTask(void)
{
    BattleFxLimitNightmareSprite *task;

    task = (BattleFxLimitNightmareSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_YasuiLimitNightmareSpriteRequest.frameWord = task->frame | 0x8000;
    g_YasuiLimitNightmareSpriteRequest.texRowPacked = task->textureRow;
    g_YasuiLimitNightmareSpriteRequest.spriteGroup2 = Gfx_ModelTableGetLoadedRecord(0, g_YasuiLimitNightmareLoadedResTable);
    g_YasuiLimitNightmareSpriteRequest.drawObj2 = Gfx_ModelTableGetSpriteObject(0, g_YasuiLimitNightmareLoadedResTable);
    BattleFx_InstallBillboardMatrix(&task->posX, task->angle, -(task->angle >> 4));
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_YasuiLimitNightmareSpriteRequest.spriteGroup2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->frame >= 8) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (task->frame)++;
}

/*
 * Animates expanding dome mesh model with scaling and transparency transitions.
 */
// FUNCTION: FF7 0x0046338e
void __cdecl BattleFx_LimitNightmareExpandingModelTask(void)
{
    struct {
        PsxMatrix matrix;
        unsigned char pad_1E[0x2];
        BattleFxNightmareExpandState *task;
        SVECTOR rotAngles;
        YasuiLimitNightmareMeshState *mesh;
        VECTOR scale;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.task = (BattleFxNightmareExpandState *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotAngles.vx = 0;
    frame.rotAngles.vy = frame.task->positionA;
    frame.rotAngles.vz = 0;
    RotMatrixXYZ(&frame.rotAngles, &frame.matrix);
    frame.matrix.t[0] = frame.task->posX;
    frame.matrix.t[1] = frame.task->posY;
    frame.matrix.t[2] = frame.task->posZ;
    frame.scale.vz = frame.task->positionB;
    frame.scale.vx = frame.scale.vz;
    frame.scale.vy = frame.task->positionC;
    ScaleMatrix(&frame.matrix, &frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    frame.mesh = (YasuiLimitNightmareMeshState *)Gfx_GetPsxScratchpadPtr(0);
    frame.mesh->meshHeader = (int)&g_YasuiLimitNightmareMeshHeader1;
    RefreshGlobalTransformState();
    Gfx_SetPsxPrimQuadState(9, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(4, g_YasuiLimitNightmareLoadedResTable)));
    Gfx_SetPsxPrimQuadState(9, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(5, g_YasuiLimitNightmareLoadedResTable)));
    Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(6, g_YasuiLimitNightmareLoadedResTable)));
    Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(7, g_YasuiLimitNightmareLoadedResTable)));
    g_BattleFxActiveDrawListPtr = BattleFx_LimitNightmareDrawPsxMesh((BattleFxLimitNightmarePsxMesh *)frame.mesh, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    frame.mesh->flags = 0x28;
    frame.mesh->uvScroll = 0;
    frame.mesh->expandRemaining = 128;
    frame.mesh->attrMask = 0x20;
    frame.mesh->pad_0E = 0;
    if (frame.task->timer >= 24) {
        frame.mesh->expandRemaining -= (short)((frame.task->timer - 24) * 10);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_LimitNightmareDrawPsxMesh((BattleFxLimitNightmarePsxMesh *)frame.mesh, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.task->timer >= 36) {
        frame.task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.task->positionA += frame.task->velocityA;
    frame.task->velocityA -= (short)(frame.task->velocityA >> 6);
    frame.task->positionB += frame.task->velocityB;
    if (frame.task->timer < 22) {
        frame.task->positionC += frame.task->velocityC;
    }
    if (frame.task->timer == 0) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH2, Battle_ComputeScreenPanFromPosition(&frame.task->posX), 0x21B);
    }
    (frame.task->timer)++;
}

/*
 * Dispatches PSX mesh primitive streams (textured/colored triangles and quads) to the render queue.
 */
// FUNCTION: FF7 0x0046363c
int __cdecl BattleFx_LimitNightmareDrawPsxMesh(BattleFxLimitNightmarePsxMesh *mesh, int orderingTable, int depthShift, int packetCursor)
{
    int vertA;
    int vertB;
    int color[2];
    int count;
    int supported;
    BattleFxLimitNightmarePsxPacket *primitive;
    int i;
    int numElements;
    BattleFxLimitNightmareQuadPrim *quadCurrent;
    int stackHole; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int vertC;
    int streamBase;
    unsigned short bits;
    BattleFxLimitNightmarePsxPacket *primTri;
    int unsupportedFlags;
    YasuiLimitNightmareMeshStream *dataPointer;
    BattleFxLimitNightmareQuadPrim *renderBlock;

    streamBase = mesh->primData + 4;
    dataPointer = (YasuiLimitNightmareMeshStream *)(streamBase + *(int *)mesh->primData);
    renderBlock = (BattleFxLimitNightmareQuadPrim *)Gfx_GetPsxPacketScratchPtr();
    count = dataPointer->vertexOffsetA;
    dataPointer = (YasuiLimitNightmareMeshStream *)((int)dataPointer + 2);
    bits = dataPointer->vertexOffsetA & 0x19F | mesh->attrMask;
    dataPointer = (YasuiLimitNightmareMeshStream *)((int)dataPointer + 2);
    vertA = streamBase + dataPointer->vertexOffsetA;
    vertB = streamBase + dataPointer->vertexOffsetB;
    vertC = streamBase + dataPointer->vertexOffsetC;
    for (i = 0; i < count; i++) {
        StoreDeferredDrawArgs((const void *)vertA, (const void *)vertB, (const void *)vertC);
        ApplyDeferredDrawArgs();
        vertA = streamBase + dataPointer->quadVertexOffset;
        vertB = streamBase + dataPointer->vertBWord;
        vertC = streamBase + dataPointer->vertexOffsetD;
        Gfx_PsxStoreClearFlag(&unsupportedFlags);
        if ((unsupportedFlags & 0x60000) == 0) {
            if ((mesh->flags & 0x20) != 0) {
                supported = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&supported);
            }
            if (supported > 0) {
                renderBlock->command = 0x7000000;
                Gfx_ReadTransformOrderingDepth2(&numElements);
                renderBlock->primWord = dataPointer->primWord;
                renderBlock->uv1 = dataPointer->uv1;
                renderBlock->uv2 = dataPointer->uv2;
                renderBlock->bits = bits;
                if ((mesh->flags & 0x80) != 0) {
                    color[0] = dataPointer->resourceOffset & 0xFF;
                    color[0] = color[0] << 0x10 | 0x24000000 | color[0] << 8 | color[0];
                    if (mesh->shade != 0) {
                        Gfx_ShadePsxVertexColor((CVECTOR *)color, mesh->shade, (CVECTOR *)&renderBlock->color);
                        *((char *)&renderBlock->color + 3) = 0x24;
                    } else {
                        renderBlock->color = color[0];
                    }
                } else {
                    renderBlock->color = color[0] << 0x10 | 0x24000000 | color[0] << 8 | color[0];
                }
                if ((mesh->flags & 8) != 0) {
                    *((char *)&renderBlock->color + 3) |= 2;
                }
                Gfx_DrawPsxPrimitive(renderBlock);
            }
        }
        dataPointer = (YasuiLimitNightmareMeshStream *)((int)dataPointer + 0x10);
    }
    count = dataPointer->vertexOffsetA;
    dataPointer = (YasuiLimitNightmareMeshStream *)((int)dataPointer + 4);
    quadCurrent = (BattleFxLimitNightmareQuadPrim *)renderBlock;
    vertA = streamBase + dataPointer->vertexOffsetA;
    vertB = streamBase + dataPointer->vertexOffsetB;
    vertC = streamBase + dataPointer->vertexOffsetC;
    for (i = 0; i < count; i++) {
        StoreDeferredDrawArgs((const void *)vertA, (const void *)vertB, (const void *)vertC);
        ApplyDeferredDrawArgs();
        vertA = streamBase + dataPointer->vertexOffsetD;
        vertB = streamBase + dataPointer->vertexOffsetE;
        vertC = streamBase + dataPointer->vertexOffsetF;
        Gfx_PsxStoreClearFlag(&unsupportedFlags);
        if ((unsupportedFlags & 0x60000) == 0) {
            if ((mesh->flags & 0x20) != 0) {
                supported = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&supported);
            }
            if (supported >= 0) {
                SetPendingResourceHandle((GfxPsxShortVertexSlot *)(streamBase + dataPointer->resourceOffset));
                ApplyPendingResourceHandle();
                Gfx_ReadTransformOrderingDepth2(&numElements);
                quadCurrent->command = 0x9000000;
                quadCurrent->primWord = dataPointer->primWord;
                quadCurrent->uv1 = dataPointer->uv1;
                quadCurrent->uv2 = dataPointer->uv2;
                quadCurrent->quadVertexOffset = dataPointer->quadVertexOffset;
                quadCurrent->bits = bits;
                if ((mesh->flags & 0x80) != 0) {
                    color[0] = dataPointer->vertBWord & 0xFF;
                    color[0] = color[0] << 0x10 | 0x2C000000 | color[0] << 8 | color[0];
                    if (mesh->shade != 0) {
                        Gfx_ShadePsxVertexColor((CVECTOR *)color, mesh->shade, (CVECTOR *)&quadCurrent->color);
                        *((char *)&quadCurrent->color + 3) = 0x2C;
                    } else {
                        quadCurrent->color = color[0];
                    }
                } else {
                    color[0] = mesh->shade & 0xFF;
                    quadCurrent->color = color[0] << 0x10 | 0x2C000000 | color[0] << 8 | color[0];
                }
                if ((mesh->flags & 8) != 0) {
                    *((char *)&quadCurrent->color + 3) |= 2;
                }
                Gfx_DrawPsxPrimitive(quadCurrent);
            }
        }
        dataPointer = (YasuiLimitNightmareMeshStream *)((int)dataPointer + 0x14);
    }
    count = dataPointer->vertexOffsetA;
    dataPointer = (YasuiLimitNightmareMeshStream *)((int)dataPointer + 4);
    primTri = (BattleFxLimitNightmarePsxPacket *)quadCurrent;
    vertA = streamBase + dataPointer->vertexOffsetA;
    vertB = streamBase + dataPointer->vertexOffsetB;
    vertC = streamBase + dataPointer->vertexOffsetC;
    for (i = 0; i < count; i++) {
        StoreDeferredDrawArgs((const void *)vertA, (const void *)vertB, (const void *)vertC);
        ApplyDeferredDrawArgs();
        vertA = streamBase + dataPointer->vertexOffsetD;
        vertB = streamBase + dataPointer->vertexOffsetE;
        vertC = streamBase + dataPointer->vertexOffsetF;
        Gfx_PsxStoreClearFlag(&unsupportedFlags);
        if ((unsupportedFlags & 0x60000) == 0) {
            if ((mesh->flags & 0x20) != 0) {
                supported = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&supported);
            }
            if (supported >= 0) {
                primTri->tag = 0x6000000;
                Gfx_ReadTransformOrderingDepth2(&numElements);
                if (mesh->shade != 0) {
                    Gfx_ShadePsxTriVertexColors((CVECTOR *)&dataPointer->primWord, (CVECTOR *)&dataPointer->colorCmd, (CVECTOR *)&dataPointer->color2, mesh->shade, (CVECTOR *)&primTri->primWord, (CVECTOR *)&primTri->colorCmd, (CVECTOR *)&primTri->color2);
                } else {
                    primTri->primWord = dataPointer->primWord;
                    primTri->colorCmd = dataPointer->colorCmd;
                    primTri->color2 = dataPointer->color2;
                }
                if ((mesh->flags & 8) != 0) {
                    *((char *)&primTri->primWord + 3) |= 2;
                }
                Gfx_DrawPsxPrimitive(primTri);
            }
        }
        dataPointer = (YasuiLimitNightmareMeshStream *)((int)dataPointer + 0x14);
    }
    count = dataPointer->vertexOffsetA;
    dataPointer = (YasuiLimitNightmareMeshStream *)((int)dataPointer + 4);
    primitive = (BattleFxLimitNightmarePsxPacket *)primTri;
    vertA = streamBase + dataPointer->vertexOffsetA;
    vertB = streamBase + dataPointer->vertexOffsetB;
    vertC = streamBase + dataPointer->vertexOffsetC;
    for (i = 0; i < count; i++) {
        StoreDeferredDrawArgs((const void *)vertA, (const void *)vertB, (const void *)vertC);
        ApplyDeferredDrawArgs();
        vertA = streamBase + dataPointer->vertexOffsetF;
        vertB = streamBase + dataPointer->vertBOffset;
        vertC = streamBase + dataPointer->vertCOffset;
        Gfx_PsxStoreClearFlag(&unsupportedFlags);
        if ((unsupportedFlags & 0x60000) == 0) {
            if ((mesh->flags & 0x20) != 0) {
                supported = 1;
            } else {
                Gfx_PsxStoreFrontFacing(&supported);
            }
            if (supported >= 0) {
                SetPendingResourceHandle((GfxPsxShortVertexSlot *)(streamBase + dataPointer->resourceOffset));
                ApplyPendingResourceHandle();
                Gfx_ReadTransformOrderingDepth2(&numElements);
                primitive->tag = 0x8000000;
                if (mesh->shade != 0) {
                    Gfx_ShadePsxTriVertexColors((CVECTOR *)&dataPointer->primWord, (CVECTOR *)&dataPointer->colorCmd, (CVECTOR *)&dataPointer->color2, mesh->shade, (CVECTOR *)&primitive->primWord, (CVECTOR *)&primitive->colorCmd, (CVECTOR *)&primitive->color2);
                    Gfx_ShadePsxVertexColor((CVECTOR *)&dataPointer->color3, mesh->shade, (CVECTOR *)&primitive->color3);
                } else {
                    primitive->primWord = dataPointer->primWord;
                    primitive->colorCmd = dataPointer->colorCmd;
                    primitive->color2 = dataPointer->color2;
                    primitive->color3 = dataPointer->color3;
                }
                if ((mesh->flags & 8) != 0) {
                    *((char *)&primitive->primWord + 3) |= 2;
                }
                Gfx_DrawPsxPrimitive(primitive);
            }
        }
        dataPointer = (YasuiLimitNightmareMeshStream *)((int)dataPointer + 0x18);
    }
    return (int)primitive;
}
} /* extern "C" */
