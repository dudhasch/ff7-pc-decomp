// src/ff7/src/credits/credits_animation_frame_loop.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Runs the credits module: it sets up the animation records and fade pair, steps the
//   sequence and text states, and paces each frame from the tick duration. Enter, exit, input and
//   frame callbacks sit beside small setters for the variant and start section.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/mem.h"
#include "ff7/api/credits.h"
#include "ff7/enums/credits.h"
#include "ff7/api/field.h"
#include "lib/api/movie.h"
#include "ff7/api/main.h"
#include "lib/api/file.h"
#include "lib/api/graphics.h"
#include "lib/api/mem.h"
#include "lib/api/sound.h"
#include "lib/enums/file.h"
#include "lib/enums/graphics.h"
#include "lib/enums/sound.h"
#include "ff7/types/credits.h"
#include "lib/types/graphics.h"
#include "ff7/enums/main.h"
#include "lib/codegen.h"
#include <math.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <mmsystem.h>
#include "ff7/gte.h"
#include "ff7/field.h"

struct GfxEngineContext;
extern "C" {
// clang-format off
// GLOBAL: FF7 0x0099b4d0
int g_CreditsQuadVertexScale = 256;
// GLOBAL: FF7 0x0099b4d4
int g_CreditsScaleShift = 2;
// GLOBAL: FF7 0x0099b4d8
int g_CreditsGlyphDrawFunctions[10] = {(int)&Credits_DrawGlyphString32x40, (int)&Credits_DrawGlyphString32x40TextBufferNodeA, (int)&Credits_DrawGlyphString32x40TextBufferNode0, (int)&Credits_DrawGlyphString16x20, (int)&Credits_DrawGlyphString16x20VHalf, (int)&Credits_DrawGlyphString16x20UHalf, (int)&Credits_DrawGlyphString8x10UVHalf, (int)&Credits_DrawGlyphString8x10UHalfV3Quarter, (int)&Credits_DrawGlyphString8x10U3QuarterVHalf, 0};
// GLOBAL: FF7 0x0099b500
struct CreditsTimingRange g_CreditsTimingTable[29] = {{0, 2}, {2, 4}, {4, 6}, {6, 8}, {8, 10}, {10, 12}, {12, 14}, {14, 16}, {16, 18}, {18, 20}, {20, 22}, {22, 24}, {24, 26}, {26, 28}, {28, 30}, {30, 32}, {32, 34}, {34, 36}, {36, 40}, {40, 42}, {42, 44}, {44, 46}, {46, 48}, {48, 50}, {50, 52}, {52, 54}, {54, 56}, {56, 58}, {58, 60}};

// GLOBAL: FF7 0x0099b5e8
int g_CreditsAnimationTable[60][8] = {
    {368, 8, -272, 8, 258, 0, 260, 0},
    {187, -144, 187, 336, 0, 40, 0, 41},
    {-304, 200, 656, 200, 16, 0, 17, 0},
    {16, 480, 16, -480, 0, 168, 0, 169},
    {-304, 8, 656, 8, 16, 0, 16, 0},
    {16, -144, 16, 336, 0, 40, 0, 41},
    {336, 200, -304, 200, 245, 0, 246, 0},
    {220, 480, 220, -160, 0, 168, 0, 169},
    {368, 8, -272, 8, 254, 0, 255, 0},
    {215, -144, 215, 336, 0, 40, 0, 41},
    {-304, 200, 656, 200, 16, 0, 17, 0},
    {16, 480, 16, -480, 0, 168, 0, 169},
    {-304, 8, 656, 8, 16, 0, 16, 0},
    {16, -144, 16, 336, 0, 40, 0, 41},
    {336, 200, -304, 200, 196, 0, 197, 0},
    {240, 480, 240, -160, 0, 168, 0, 169},
    {368, 8, -272, 8, 244, 0, 245, 0},
    {197, -144, 197, 336, 0, 40, 0, 41},
    {-304, 200, 656, 200, 16, 0, 17, 0},
    {16, 480, 16, -480, 0, 168, 0, 169},
    {-304, 8, 656, 8, 16, 0, 16, 0},
    {16, -144, 16, 336, 0, 40, 0, 41},
    {336, 200, -304, 200, 224, 0, 225, 0},
    {190, 480, 190, -160, 0, 168, 0, 169},
    {368, 8, -272, 8, 214, 0, 215, 0},
    {247, -144, 247, 336, 0, 40, 0, 41},
    {-304, 200, 656, 200, 16, 0, 17, 0},
    {16, 480, 16, -480, 0, 168, 0, 169},
    {-304, 8, 656, 8, 16, 0, 16, 0},
    {16, -144, 16, 336, 0, 40, 0, 41},
    {336, 200, -304, 200, 224, 0, 225, 0},
    {220, 480, 220, -160, 0, 168, 0, 169},
    {368, 8, -272, 8, 224, 0, 225, 0},
    {200, -144, 200, 336, 0, 40, 0, 41},
    {-304, 200, 656, 200, 16, 0, 17, 0},
    {16, 480, 16, -480, 0, 168, 0, 169},
    {-304, 8, 656, 8, 16, 0, 16, 0},
    {16, -144, 16, 336, 0, 40, 0, 41},
    {-304, 56, 656, 56, 16, 0, 16, 0},
    {16, -144, 16, 336, 0, 72, 0, 73},
    {368, 8, -272, 8, 204, 0, 205, 0},
    {247, -144, 247, 336, 0, 40, 0, 41},
    {-304, 200, 656, 200, 16, 0, 17, 0},
    {16, 480, 16, -480, 0, 168, 0, 169},
    {-304, 8, 656, 8, 16, 0, 16, 0},
    {16, -144, 16, 336, 0, 40, 0, 41},
    {336, 200, -304, 200, 244, 0, 245, 0},
    {200, 480, 200, -160, 0, 168, 0, 169},
    {368, 8, -272, 8, 224, 0, 225, 0},
    {240, -144, 240, 336, 0, 40, 0, 41},
    {-304, 200, 656, 200, 16, 0, 17, 0},
    {16, 480, 16, -480, 0, 168, 0, 169},
    {-304, 8, 656, 8, 16, 0, 16, 0},
    {16, -144, 16, 336, 0, 40, 0, 41},
    {336, 200, -304, 200, 244, 0, 245, 0},
    {244, 480, 244, -160, 0, 168, 0, 169},
    {368, 8, -272, 8, 204, 0, 205, 0},
    {207, -144, 207, 336, 0, 40, 0, 41},
    {-304, 200, 656, 200, 16, 0, 17, 0},
    {16, 480, 16, -480, 0, 168, 0, 169},
};
unsigned char g_rawData_0099bd68[1920] = {
0x70, 0x01, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0xf0, 0xfe, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00,
0xe4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x9d, 0x00, 0x00, 0x00, 0x70, 0xff, 0xff, 0xff, 0x9d, 0x00, 0x00, 0x00, 0x50, 0x01, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00,
0xd0, 0xfe, 0xff, 0xff, 0xc8, 0x00, 0x00, 0x00, 0x90, 0x02, 0x00, 0x00, 0xc8, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0xfe, 0xff, 0xff,
0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x00, 0x00, 0x00,
0xd0, 0xfe, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00, 0x90, 0x02, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0x70, 0xff, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00, 0x50, 0x01, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00,
0x50, 0x01, 0x00, 0x00, 0xc8, 0x00, 0x00, 0x00, 0xd0, 0xfe, 0xff, 0xff, 0xc8, 0x00, 0x00, 0x00,
0xd7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xbe, 0x00, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00, 0xbe, 0x00, 0x00, 0x00, 0x60, 0xff, 0xff, 0xff,
0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x00, 0x00, 0x00,
0x70, 0x01, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0xf0, 0xfe, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00,
0x9a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xa5, 0x00, 0x00, 0x00, 0x70, 0xff, 0xff, 0xff, 0xa5, 0x00, 0x00, 0x00, 0x50, 0x01, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00,
0xd0, 0xfe, 0xff, 0xff, 0xc8, 0x00, 0x00, 0x00, 0x90, 0x02, 0x00, 0x00, 0xc8, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0xfe, 0xff, 0xff,
0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x00, 0x00, 0x00,
0xd0, 0xfe, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00, 0x90, 0x02, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0x70, 0xff, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00, 0x50, 0x01, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00,
0x50, 0x01, 0x00, 0x00, 0xc8, 0x00, 0x00, 0x00, 0xd0, 0xfe, 0xff, 0xff, 0xc8, 0x00, 0x00, 0x00,
0x9c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xd2, 0x00, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00, 0xd2, 0x00, 0x00, 0x00, 0x60, 0xff, 0xff, 0xff,
0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x00, 0x00, 0x00,
0x70, 0x01, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0xf0, 0xfe, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00,
0xd6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xa7, 0x00, 0x00, 0x00, 0x70, 0xff, 0xff, 0xff, 0xa7, 0x00, 0x00, 0x00, 0x50, 0x01, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00,
0xd0, 0xfe, 0xff, 0xff, 0xc8, 0x00, 0x00, 0x00, 0x90, 0x02, 0x00, 0x00, 0xc8, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0xfe, 0xff, 0xff,
0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x00, 0x00, 0x00,
0xd0, 0xfe, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00, 0x90, 0x02, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0x70, 0xff, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00, 0x50, 0x01, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00,
0x50, 0x01, 0x00, 0x00, 0xc8, 0x00, 0x00, 0x00, 0xd0, 0xfe, 0xff, 0xff, 0xc8, 0x00, 0x00, 0x00,
0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x8c, 0x00, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00, 0x8c, 0x00, 0x00, 0x00, 0x60, 0xff, 0xff, 0xff,
0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x00, 0x00, 0x00,
0x70, 0x01, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0xf0, 0xfe, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00,
0xb1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xd7, 0x00, 0x00, 0x00, 0x70, 0xff, 0xff, 0xff, 0xd7, 0x00, 0x00, 0x00, 0x50, 0x01, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00,
0xd0, 0xfe, 0xff, 0xff, 0xc8, 0x00, 0x00, 0x00, 0x90, 0x02, 0x00, 0x00, 0xc8, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0xfe, 0xff, 0xff,
0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x00, 0x00, 0x00,
0xd0, 0xfe, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00, 0x90, 0x02, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0x70, 0xff, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00, 0x50, 0x01, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00,
0x50, 0x01, 0x00, 0x00, 0xc8, 0x00, 0x00, 0x00, 0xd0, 0xfe, 0xff, 0xff, 0xc8, 0x00, 0x00, 0x00,
0xae, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xaa, 0x00, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00, 0xaa, 0x00, 0x00, 0x00, 0x60, 0xff, 0xff, 0xff,
0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x00, 0x00, 0x00,
0x70, 0x01, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0xf0, 0xfe, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00,
0xb6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x9e, 0x00, 0x00, 0x00, 0x70, 0xff, 0xff, 0xff, 0x9e, 0x00, 0x00, 0x00, 0x50, 0x01, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00,
0xd0, 0xfe, 0xff, 0xff, 0xc8, 0x00, 0x00, 0x00, 0x90, 0x02, 0x00, 0x00, 0xc8, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0xfe, 0xff, 0xff,
0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x00, 0x00, 0x00,
0xd0, 0xfe, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00, 0x90, 0x02, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0x70, 0xff, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00, 0x50, 0x01, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00,
0xd0, 0xfe, 0xff, 0xff, 0x3c, 0x00, 0x00, 0x00, 0x90, 0x02, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0x70, 0xff, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00, 0x50, 0x01, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x51, 0x00, 0x00, 0x00,
0x70, 0x01, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0xf0, 0xfe, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00,
0xa4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xca, 0x00, 0x00, 0x00, 0x70, 0xff, 0xff, 0xff, 0xca, 0x00, 0x00, 0x00, 0x50, 0x01, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00,
0xd0, 0xfe, 0xff, 0xff, 0xc8, 0x00, 0x00, 0x00, 0x90, 0x02, 0x00, 0x00, 0xc8, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0xfe, 0xff, 0xff,
0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x00, 0x00, 0x00,
0xd0, 0xfe, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00, 0x90, 0x02, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0x70, 0xff, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00, 0x50, 0x01, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00,
0x50, 0x01, 0x00, 0x00, 0xc8, 0x00, 0x00, 0x00, 0xd0, 0xfe, 0xff, 0xff, 0xc8, 0x00, 0x00, 0x00,
0xcc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xa0, 0x00, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x60, 0xff, 0xff, 0xff,
0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x00, 0x00, 0x00,
0x70, 0x01, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0xf0, 0xfe, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00,
0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xc8, 0x00, 0x00, 0x00, 0x70, 0xff, 0xff, 0xff, 0xc8, 0x00, 0x00, 0x00, 0x50, 0x01, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00,
0xd0, 0xfe, 0xff, 0xff, 0xc8, 0x00, 0x00, 0x00, 0x90, 0x02, 0x00, 0x00, 0xc8, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0xfe, 0xff, 0xff,
0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x00, 0x00, 0x00,
0xd0, 0xfe, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00, 0x90, 0x02, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0x70, 0xff, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00, 0x50, 0x01, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00,
0x50, 0x01, 0x00, 0x00, 0xc8, 0x00, 0x00, 0x00, 0xd0, 0xfe, 0xff, 0xff, 0xc8, 0x00, 0x00, 0x00,
0xd6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xd6, 0x00, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00, 0xd6, 0x00, 0x00, 0x00, 0x60, 0xff, 0xff, 0xff,
0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x00, 0x00, 0x00,
0x70, 0x01, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0xf0, 0xfe, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00,
0xc2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xca, 0x00, 0x00, 0x00, 0x70, 0xff, 0xff, 0xff, 0xca, 0x00, 0x00, 0x00, 0x50, 0x01, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00,
0xd0, 0xfe, 0xff, 0xff, 0xc8, 0x00, 0x00, 0x00, 0x90, 0x02, 0x00, 0x00, 0xc8, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x10, 0x00, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0xfe, 0xff, 0xff,
0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0099c4e8
int g_CreditsEndModes[8] = {CREDITS_ANIM_MODE_X_DEC_Y_INC, CREDITS_ANIM_MODE_X_DEC_Y_INC, CREDITS_ANIM_MODE_X_INC_Y_DEC, CREDITS_ANIM_MODE_X_INC_Y_DEC, CREDITS_ANIM_MODE_X_INC_Y_INC, CREDITS_ANIM_MODE_X_INC_Y_INC, CREDITS_ANIM_MODE_X_DEC_Y_DEC, CREDITS_ANIM_MODE_X_DEC_Y_DEC};
// GLOBAL: FF7 0x0099c508
int g_CreditsEndVBaseBitsQuarter[4] = {0, 1048576000, 1061158912, 1056964608};
// GLOBAL: FF7 0x0099c518
int g_CreditsEndVBaseBitsEighth[8] = {0, 1040187392, 1052770304, 1048576000, 1056964608, 1059061760, 1063256064, 1061158912};
// GLOBAL: FF7 0x0099c538
int g_CreditsEndVelocity0[8] = {-4, 0, 4, 0, 4, 0, -4, 0};
// GLOBAL: FF7 0x0099c558
int g_CreditsEndVelocity8[8] = {4, 0, -4, 0, -4, 0, 4, 0};
// GLOBAL: FF7 0x0099c578
int g_CreditsEndVelocity4[8] = {0, 4, 0, -4, 0, 4, 0, -4};
// GLOBAL: FF7 0x0099c598
int g_CreditsEndVelocityC[8] = {0, -4, 0, 4, 0, -4, 0, 4};
// GLOBAL: FF7 0x0099c5b8
int g_CreditsLayoutScale = 2;
unsigned char g_rawData_0099c5bc[4] = {0x04, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00f4d6a8
double g_CreditsFrameDuration = 0.0;
// GLOBAL: FF7 0x00f4d6b0
Timestamp64 g_CreditsTimestampNow = {0};
// GLOBAL: FF7 0x00f4d6b8
int g_CreditsFrameDurationTarget = 0;
unsigned char g_bssPad_00f4d6bc[4] = {0};
// GLOBAL: FF7 0x00f4d6c0
Timestamp64 g_CreditsTimestampFrameStart = {0};
// GLOBAL: FF7 0x00f4d6c8
struct CreditsAnimationKey *g_CreditsActiveAnimationPointer = 0;
unsigned char g_bssPad_00f4d6cc[4] = {0};
// 32-byte GameModule split by interior public callback markers at +0x08..+0x10.
// GLOBAL: FF7 0x00f4d6d0
int g_CreditsActiveModuleInfo[2] = {0};
// GLOBAL: FF7 0x00f4d6d8
GameModuleCallback g_CreditsDispatcherEnter = 0;
// GLOBAL: FF7 0x00f4d6dc
GameModuleCallback g_CreditsDispatcherExit = 0;
// GLOBAL: FF7 0x00f4d6e0
GameModuleCallback g_CreditsDispatcherFrame = 0;
// GLOBAL: FF7 0x00f4d6e4
GameModuleMessageCallback g_CreditsAppMouseCallback = 0;
// GLOBAL: FF7 0x00f4d6e8
GameModuleMessageCallback g_CreditsAppKeyCallback = 0;
unsigned char g_bssPad_00f4d6ec[4] = {0};
// GLOBAL: FF7 0x00f4d6f0
struct CreditsAnimationRecord g_CreditsAnimationRecords[60] = {0};
// GLOBAL: FF7 0x00f4f310
int g_CreditsFrameDelayFlag[2] = {0};
// GLOBAL: FF7 0x00f4f318
double g_CreditsFrameExcessTime = 0.0;
// GLOBAL: FF7 0x00f4f320
int g_CreditsSequenceTickCount = 0;
// GLOBAL: FF7 0x00f4f324
int g_CreditsTotalTickCounter = 0;
// GLOBAL: FF7 0x00f4f328
int g_CreditsSelectedSectionIndex = 0;
// GLOBAL: FF7 0x00f4f32c
int g_CreditsSectionTimer = 0;
// GLOBAL: FF7 0x00f4f330
struct CreditsSectionFlags g_CreditsSectionFlags = {0};
// GLOBAL: FF7 0x00f4f338
int g_CreditsFadeOverlayPending = 0;
// GLOBAL: FF7 0x00f4f33c
int g_CreditsFadeCounter = 0;
// GLOBAL: FF7 0x00f4f340
int g_CreditsFadeStepOffset = 0;
unsigned char g_bssPad_00f4f344[4] = {0};
// GLOBAL: FF7 0x00f4f348
struct CreditsFadeOverlayBatch g_CreditsFadeOverlayBatch = {0};
// GLOBAL: FF7 0x00f4f364
int g_CreditsQuadBatchListA = 0;
unsigned char g_bssPad_00f4f368[60] = {0};
// GLOBAL: FF7 0x00f4f3a4
int g_CreditsQuadBatchListB = 0;
unsigned char g_bssPad_00f4f3a8[60] = {0};
// GLOBAL: FF7 0x00f4f3e4
GfxRenderObject *g_CreditsLogoQuadBatches = 0;
// GLOBAL: FF7 0x00f4f3e8
GfxRenderObject *g_CreditsUnusedFlagE8 = 0;
// GLOBAL: FF7 0x00f4f3ec
GfxRenderObject *g_CreditsLogoDrawState = 0;
// GLOBAL: FF7 0x00f4f3f0
GfxRenderObject *g_CreditsUnusedFlagF0 = 0;
// GLOBAL: FF7 0x00f4f3f4
GfxRenderObject *g_CreditsQuadBatchListD = 0;
// GLOBAL: FF7 0x00f4f3f8
struct ChocoboCreditsGlyphBuffer g_CreditsTextBuffer = {0};
// GLOBAL: FF7 0x00f4f404
GfxRenderObject *g_CreditsTextState = 0;
// GLOBAL: FF7 0x00f4f408
int g_CreditsVariant = 0;
// GLOBAL: FF7 0x00f4f40c
int g_CreditsStartSection = 0;
// GLOBAL: FF7 0x00f4f410
int g_CreditsSequenceState = 0;
// GLOBAL: FF7 0x00f4f414
int g_CreditsTextPhase = 0;
// GLOBAL: FF7 0x00f4f418
int g_CreditsViewportX = 0;
// GLOBAL: FF7 0x00f4f41c
int g_CreditsViewportY = 0;
// GLOBAL: FF7 0x00f4f420
int g_CreditsViewportWidth = 0;
// GLOBAL: FF7 0x00f4f424
int g_CreditsViewportHeight = 0;
// GLOBAL: FF7 0x00f4f428
unsigned char g_CreditsMovieAlpha[4] = {0};
// GLOBAL: FF7 0x00f4f42c
struct CreditsScriptRecord *g_CreditsScriptDataBase = 0;
// GLOBAL: FF7 0x00f4f430
int g_CreditsMovieCurrentFrame = 0;
// GLOBAL: FF7 0x00f4f434
int g_CreditsUnusedFlag34 = 0;
// GLOBAL: FF7 0x00f4f438
int g_CreditsUnusedFlag38 = 0;
// GLOBAL: FF7 0x00f4f43c
int g_CreditsSmallViewport = 0;
// GLOBAL: FF7 0x00f4f440
int g_CreditsMovieRunning = 0;
// GLOBAL: FF7 0x00f4f444
int g_CreditsEndOverlayFlag = 0;
// GLOBAL: FF7 0x00f4f448
int g_CreditsTextScrollMode = 0;
// GLOBAL: FF7 0x00f4f44c
int g_CreditsMoviePlaybackDone = 0;
// GLOBAL: FF7 0x00f4f450
int g_CreditsTextDisplayTimer = 0;
// GLOBAL: FF7 0x00f4f454
int g_CreditsWindowTimer = 0;
// GLOBAL: FF7 0x00f4f458
Timestamp64 g_CreditsFrameTimestampData[3] = {0};
// GLOBAL: FF7 0x00f4f470
double g_CreditsFrameElapsedFraction = 0.0;
// GLOBAL: FF7 0x00f4f478
double g_CreditsTickDuration = 0.0;
// GLOBAL: FF7 0x00f4f480
double g_CreditsScaledTickDuration = 0.0;
// GLOBAL: FF7 0x00f4f488
int g_CreditsFrameDurationDivisorLo = 0;
// GLOBAL: FF7 0x00f4f48c
int g_CreditsFrameDurationDivisorHi = 0;
// clang-format on

#pragma intrinsic(abs)


#pragma intrinsic(strcpy, strcat)

/* Retail caller zero-extends unsigned char alpha; definition takes signed char. */
extern void __cdecl Credits_DrawLogoWithAlpha2x(unsigned char alpha);
/* Retail caller zero-extends unsigned char alpha; definition takes signed char. */
extern void __cdecl Credits_DrawLogoWithAlpha1x(unsigned char alpha);


#pragma data_seg(".data$u77_lut")
#pragma bss_seg(".data$u77_lut")
#pragma data_seg()
#pragma bss_seg()

#pragma intrinsic(sqrt)

/*
 * Multiplies the double g_CreditsTickDuration by scale and stores the result in
 * g_CreditsScaledTickDuration.
 */
// FUNCTION: FF7 0x007a5060
void __cdecl Credits_StoreScaledTickDuration(double scale)
{
    g_CreditsScaledTickDuration = g_CreditsTickDuration * scale;
}

/*
 * Effectively a no-op as written: returns immediately when
 * g_CreditsFadeOverlayPending is zero, and otherwise stores 0 into
 * g_CreditsFadeCounter only when g_CreditsFadeCounter is already 0. Nothing
 * else is read or written.
 */
// FUNCTION: FF7 0x007a5074
void __cdecl Credits_ClearFadeCounterIfFlagged(void)
{
    if (g_CreditsFadeOverlayPending == 0) {
        return;
    }
    if (g_CreditsFadeCounter == 0) {
        g_CreditsFadeCounter = 0;
    }
}

/*
 * Advances a 0x78-byte credits animation record by its CreditsAnimationMode,
 * adding configured steps to paired coordinates and clamping them at targets.
 * The mode picks the direction positionX and positionY move; current8 and
 * currentC move the opposite way.
 */
// FUNCTION: FF7 0x007a5097
void __cdecl Credits_AdvanceAnimationRecord(int index)
{
    switch (g_CreditsAnimationRecords[index].mode) {
    case CREDITS_ANIM_MODE_X_INC_Y_INC:
        g_CreditsAnimationRecords[index].flags = 0;
        g_CreditsAnimationRecords[index].current8 += g_CreditsAnimationRecords[index].velocity8;
        if (g_CreditsAnimationRecords[index].current8 < g_CreditsAnimationRecords[index].target8) {
            g_CreditsAnimationRecords[index].current8 = g_CreditsAnimationRecords[index].target8;
            g_CreditsAnimationRecords[index].flags = 1;
        }
        if (g_CreditsAnimationRecords[index].current8 - g_CreditsAnimationRecords[index].target8 <= g_CreditsAnimationRecords[index].threshold) {
            g_CreditsAnimationRecords[index].positionX += g_CreditsAnimationRecords[index].velocity0;
            if (g_CreditsAnimationRecords[index].positionX > g_CreditsAnimationRecords[index].target0) {
                g_CreditsAnimationRecords[index].positionX = g_CreditsAnimationRecords[index].target0;
                g_CreditsAnimationRecords[index].flags |= 2;
            }
        }
        if (g_CreditsAnimationRecords[index].flag != 0) {
            g_CreditsAnimationRecords[index].flags = 0;
            if (g_CreditsAnimationRecords[(index - 1)].current8 - g_CreditsAnimationRecords[(index - 1)].target8 <= g_CreditsAnimationRecords[index].maxDistance) {
                g_CreditsAnimationRecords[index].currentC += g_CreditsAnimationRecords[index].velocityC;
                if (g_CreditsAnimationRecords[index].currentC < g_CreditsAnimationRecords[index].targetC) {
                    g_CreditsAnimationRecords[index].currentC = g_CreditsAnimationRecords[index].targetC;
                    g_CreditsAnimationRecords[index].flags = 1;
                }
                if (g_CreditsAnimationRecords[index].targetC - g_CreditsAnimationRecords[index].currentC >= g_CreditsAnimationRecords[index].threshold) {
                    g_CreditsAnimationRecords[index].positionY += g_CreditsAnimationRecords[index].velocity4;
                    if (g_CreditsAnimationRecords[index].positionY > g_CreditsAnimationRecords[index].target4) {
                        g_CreditsAnimationRecords[index].positionY = g_CreditsAnimationRecords[index].target4;
                        g_CreditsAnimationRecords[index].flags |= 2;
                    }
                }
                if ((g_CreditsAnimationRecords[(index - 1)].flags & g_CreditsAnimationRecords[index].flags == 3) != 0) {
                    g_CreditsFadeOverlayPending = 1;
                    Credits_ClearFadeCounterIfFlagged();
                } else {
                    g_CreditsFadeOverlayPending = 0;
                }
            }
        }
        break;
    case CREDITS_ANIM_MODE_X_DEC_Y_INC:
        g_CreditsAnimationRecords[index].flags = 0;
        g_CreditsAnimationRecords[index].current8 += g_CreditsAnimationRecords[index].velocity8;
        if (g_CreditsAnimationRecords[index].current8 > g_CreditsAnimationRecords[index].target8) {
            g_CreditsAnimationRecords[index].current8 = g_CreditsAnimationRecords[index].target8;
            g_CreditsAnimationRecords[index].flags = 1;
        }
        if (g_CreditsAnimationRecords[index].target8 - g_CreditsAnimationRecords[index].current8 <= g_CreditsAnimationRecords[index].threshold) {
            g_CreditsAnimationRecords[index].positionX += g_CreditsAnimationRecords[index].velocity0;
            if (g_CreditsAnimationRecords[index].positionX < g_CreditsAnimationRecords[index].target0) {
                g_CreditsAnimationRecords[index].positionX = g_CreditsAnimationRecords[index].target0;
                g_CreditsAnimationRecords[index].flags |= 2;
            }
        }
        if (g_CreditsAnimationRecords[index].flag != 0) {
            g_CreditsAnimationRecords[index].flags = 0;
            if (g_CreditsAnimationRecords[(index - 1)].target8 - g_CreditsAnimationRecords[(index - 1)].current8 <= g_CreditsAnimationRecords[index].maxDistance) {
                g_CreditsAnimationRecords[index].currentC += g_CreditsAnimationRecords[index].velocityC;
                if (g_CreditsAnimationRecords[index].currentC < g_CreditsAnimationRecords[index].targetC) {
                    g_CreditsAnimationRecords[index].currentC = g_CreditsAnimationRecords[index].targetC;
                    g_CreditsAnimationRecords[index].flags = 1;
                }
                if (g_CreditsAnimationRecords[index].targetC - g_CreditsAnimationRecords[index].currentC >= g_CreditsAnimationRecords[index].threshold) {
                    g_CreditsAnimationRecords[index].positionY += g_CreditsAnimationRecords[index].velocity4;
                    if (g_CreditsAnimationRecords[index].positionY > g_CreditsAnimationRecords[index].target4) {
                        g_CreditsAnimationRecords[index].positionY = g_CreditsAnimationRecords[index].target4;
                        g_CreditsAnimationRecords[index].flags |= 2;
                    }
                }
                if ((g_CreditsAnimationRecords[(index - 1)].flags & g_CreditsAnimationRecords[index].flags == 3) != 0) {
                    g_CreditsFadeOverlayPending = 1;
                    Credits_ClearFadeCounterIfFlagged();
                } else {
                    g_CreditsFadeOverlayPending = 0;
                }
            }
        }
        break;
    case CREDITS_ANIM_MODE_X_INC_Y_DEC:
        g_CreditsAnimationRecords[index].flags = 0;
        g_CreditsAnimationRecords[index].current8 += g_CreditsAnimationRecords[index].velocity8;
        if (g_CreditsAnimationRecords[index].current8 < g_CreditsAnimationRecords[index].target8) {
            g_CreditsAnimationRecords[index].current8 = g_CreditsAnimationRecords[index].target8;
            g_CreditsAnimationRecords[index].flags = 1;
        }
        if (g_CreditsAnimationRecords[index].current8 - g_CreditsAnimationRecords[index].target8 <= g_CreditsAnimationRecords[index].threshold) {
            g_CreditsAnimationRecords[index].positionX += g_CreditsAnimationRecords[index].velocity0;
            if (g_CreditsAnimationRecords[index].positionX > g_CreditsAnimationRecords[index].target0) {
                g_CreditsAnimationRecords[index].positionX = g_CreditsAnimationRecords[index].target0;
                g_CreditsAnimationRecords[index].flags |= 2;
            }
        }
        if (g_CreditsAnimationRecords[index].flag != 0) {
            g_CreditsAnimationRecords[index].flags = 0;
            if (g_CreditsAnimationRecords[(index - 1)].current8 - g_CreditsAnimationRecords[(index - 1)].target8 <= g_CreditsAnimationRecords[index].maxDistance) {
                g_CreditsAnimationRecords[index].currentC += g_CreditsAnimationRecords[index].velocityC;
                if (g_CreditsAnimationRecords[index].currentC > g_CreditsAnimationRecords[index].targetC) {
                    g_CreditsAnimationRecords[index].currentC = g_CreditsAnimationRecords[index].targetC;
                    g_CreditsAnimationRecords[index].flags = 1;
                }
                if (g_CreditsAnimationRecords[index].targetC - g_CreditsAnimationRecords[index].currentC <= g_CreditsAnimationRecords[index].threshold) {
                    g_CreditsAnimationRecords[index].positionY += g_CreditsAnimationRecords[index].velocity4;
                    if (g_CreditsAnimationRecords[index].positionY < g_CreditsAnimationRecords[index].target4) {
                        g_CreditsAnimationRecords[index].positionY = g_CreditsAnimationRecords[index].target4;
                        g_CreditsAnimationRecords[index].flags |= 2;
                    }
                }
                if ((g_CreditsAnimationRecords[(index - 1)].flags & g_CreditsAnimationRecords[index].flags == 3) != 0) {
                    g_CreditsFadeOverlayPending = 1;
                    Credits_ClearFadeCounterIfFlagged();
                } else {
                    g_CreditsFadeOverlayPending = 0;
                }
            }
        }
        break;
    case CREDITS_ANIM_MODE_X_DEC_Y_DEC:
        g_CreditsAnimationRecords[index].flags = 0;
        g_CreditsAnimationRecords[index].current8 += g_CreditsAnimationRecords[index].velocity8;
        if (g_CreditsAnimationRecords[index].current8 > g_CreditsAnimationRecords[index].target8) {
            g_CreditsAnimationRecords[index].current8 = g_CreditsAnimationRecords[index].target8;
            g_CreditsAnimationRecords[index].flags = 1;
        }
        if (g_CreditsAnimationRecords[index].target8 - g_CreditsAnimationRecords[index].current8 <= g_CreditsAnimationRecords[index].threshold) {
            g_CreditsAnimationRecords[index].positionX += g_CreditsAnimationRecords[index].velocity0;
            if (g_CreditsAnimationRecords[index].positionX < g_CreditsAnimationRecords[index].target0) {
                g_CreditsAnimationRecords[index].positionX = g_CreditsAnimationRecords[index].target0;
                g_CreditsAnimationRecords[index].flags |= 2;
            }
        }
        if (g_CreditsAnimationRecords[index].flag != 0) {
            g_CreditsAnimationRecords[index].flags = 0;
            if (g_CreditsAnimationRecords[(index - 1)].target8 - g_CreditsAnimationRecords[(index - 1)].current8 <= g_CreditsAnimationRecords[index].maxDistance) {
                g_CreditsAnimationRecords[index].currentC += g_CreditsAnimationRecords[index].velocityC;
                if (g_CreditsAnimationRecords[index].currentC > g_CreditsAnimationRecords[index].targetC) {
                    g_CreditsAnimationRecords[index].currentC = g_CreditsAnimationRecords[index].targetC;
                    g_CreditsAnimationRecords[index].flags = 1;
                }
                if (g_CreditsAnimationRecords[index].targetC - g_CreditsAnimationRecords[index].currentC <= g_CreditsAnimationRecords[index].threshold) {
                    g_CreditsAnimationRecords[index].positionY += g_CreditsAnimationRecords[index].velocity4;
                    if (g_CreditsAnimationRecords[index].positionY < g_CreditsAnimationRecords[index].target4) {
                        g_CreditsAnimationRecords[index].positionY = g_CreditsAnimationRecords[index].target4;
                        g_CreditsAnimationRecords[index].flags |= 2;
                    }
                }
                if ((g_CreditsAnimationRecords[(index - 1)].flags & g_CreditsAnimationRecords[index].flags == 3) != 0) {
                    g_CreditsFadeOverlayPending = 1;
                    Credits_ClearFadeCounterIfFlagged();
                } else {
                    g_CreditsFadeOverlayPending = 0;
                }
            }
        }
    }
}

/*
 * Advances the credits transition when either active flag is set, or forces
 * its second flag after the counter exceeds 3. Calls
 * Credits_BusyWaitTicks(1000.0) once when that second flag first becomes
 * active.
 */
// FUNCTION: FF7 0x007a5b5a
void __cdecl Credits_StepFadeColorPair(int *colorA, int *colorB)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[8];
        int colorBValue;
        struct { unsigned char pad_to_1[0x1]; char tmpChannelA; };
        struct { unsigned char pad_to_2[0x2]; char tmpChannelB; };
        struct { unsigned char pad_to_3[0x3]; char fadeChannel; };
        struct { unsigned char pad_to_4[0x4]; int colorAValue; };
        struct { unsigned char pad_to_7[0x7]; char fadeAlpha; };
    } frame;
#pragma pack(pop)

    if ((g_CreditsSectionFlags.fadeCounter) > 3) {
        (g_CreditsSectionFlags.fadeActive) = 1;
    }
    if (g_CreditsFadeOverlayPending == 0 && (g_CreditsSectionFlags.fadeActive) == 0) { /* empty first arm: lib/codegen.h */
    } else {
        (g_CreditsSectionFlags.fadeCounter)++;
        if (g_CreditsSectionTimer == 0 && (g_CreditsSectionFlags.fadeActive) != 0) {
            Credits_BusyWaitTicks(1000.0);
            g_CreditsSectionTimer = 1;
        }
        frame.colorAValue = *colorA;
        frame.colorBValue = *colorB;
        if ((unsigned int)g_CreditsFadeCounter > (unsigned int)0x5B) {
            frame.colorAValue = 0;
            frame.fadeAlpha -= *(char *)&g_CreditsFadeCounter;
        }
        frame.fadeChannel -= *(char *)&g_CreditsFadeCounter;
        *(char *)&frame.colorBValue = frame.fadeChannel;
        frame.tmpChannelA = *(char *)&frame.colorBValue;
        frame.tmpChannelB = frame.tmpChannelA;
        *colorA = frame.colorAValue;
        *colorB = frame.colorBValue;
        g_CreditsFadeCounter += 4;
        if ((unsigned int)g_CreditsFadeCounter > (unsigned int)0x100) {
            g_CreditsTotalTickCounter++;
            g_CreditsFadeCounter = 0;
            g_CreditsFadeOverlayPending = 0;
            (g_CreditsSectionFlags.fadeActive) = 0;
            g_CreditsSectionTimer = (g_CreditsSectionFlags.fadeActive);
            (g_CreditsSectionFlags.fadeCounter) = g_CreditsSectionTimer;
        }
    }
}

/*
 * Visits the current credits range of 0x78-byte records and emits two
 * independent quads per record when their respective acquisitions succeed.
 * Credits_StepFadeColorPair supplies the shared background/foreground colors
 * from initial 0xA0000000 and white.
 */
// FUNCTION: FF7 0x007a5c7e
void __cdecl Credits_EmitRecordQuadPairs(void)
{
    int pairIndexB;
    float uCoord;
    int depth;
    int positionY;
    int xPosition;
    int fgColor;
    float texW;
    GfxTlVertex *backBLPtr;
    GfxTlVertex *bgBRPtr;
    GfxTlVertex *bgTopRight;
    GfxTlVertex *pBottomRightFore;
    int recordIdx;
    int pairIndexA;
    int quadHeight;
    float vBase;
    int backColor;
    int tickIndex;
    GfxTlVertex *bgTL;
    float texH;
    GfxTlVertex *fgTopRight;
    GfxTlVertex *fgBLPtr;
    GfxTlVertex *pForeTL;

    backColor = 0xA0000000;
    fgColor = 0xFFFFFFFF;
    Credits_StepFadeColorPair(&backColor, &fgColor);
    tickIndex = g_CreditsTotalTickCounter;
    for (recordIdx = g_CreditsTimingTable[tickIndex].start; recordIdx < g_CreditsTimingTable[tickIndex].end; recordIdx++) {
        depth = FLOAT_0_1_BITS;
        pairIndexB = g_CreditsAnimationRecords[recordIdx].pairIndexB;
        uCoord = g_CreditsAnimationRecords[recordIdx].uCoord;
        vBase = g_CreditsAnimationRecords[recordIdx].vBase;
        texW = g_CreditsAnimationRecords[recordIdx].texWidth;
        texH = g_CreditsAnimationRecords[recordIdx].texHeight;
        quadHeight = g_CreditsAnimationRecords[recordIdx].shade;
        if (AcquireBatchNode(1, (void *)(g_CreditsQuadBatchListsB[pairIndexB])) != 0) {
            xPosition = g_CreditsAnimationRecords[recordIdx].current8 + g_CreditsViewportX;
            positionY = g_CreditsAnimationRecords[recordIdx].currentC + g_CreditsViewportY;
            bgTL = (GfxTlVertex *)(((GfxRenderObject *)g_CreditsQuadBatchListsB[pairIndexB])->vertexCursor);
            bgTL->x = (float)xPosition;
            bgTL->y = (float)positionY;
            bgTL->zBits = depth;
            bgTL->rhwBits = FLOAT_ONE_BITS;
            bgTL->color = backColor;
            bgTL->specular = 0xFF000000;
            bgTL->u = uCoord;
            bgTL->v = vBase;
            backBLPtr = (GfxTlVertex *)(((GfxRenderObject *)g_CreditsQuadBatchListsB[pairIndexB])->vertexCursor + 0x20);
            backBLPtr->x = (float)xPosition;
            backBLPtr->y = (float)positionY + quadHeight;
            backBLPtr->zBits = depth;
            backBLPtr->rhwBits = FLOAT_ONE_BITS;
            backBLPtr->color = backColor;
            backBLPtr->specular = 0xFF000000;
            backBLPtr->u = uCoord;
            backBLPtr->v = vBase + texH;
            bgTopRight = (GfxTlVertex *)(((GfxRenderObject *)g_CreditsQuadBatchListsB[pairIndexB])->vertexCursor + 0x40);
            bgTopRight->x = (float)xPosition + g_CreditsQuadVertexScale;
            bgTopRight->y = (float)positionY;
            bgTopRight->zBits = depth;
            bgTopRight->rhwBits = FLOAT_ONE_BITS;
            bgTopRight->color = backColor;
            bgTopRight->specular = 0xFF000000;
            bgTopRight->u = uCoord + texW;
            bgTopRight->v = vBase;
            bgBRPtr = (GfxTlVertex *)(((GfxRenderObject *)g_CreditsQuadBatchListsB[pairIndexB])->vertexCursor + 0x60);
            bgBRPtr->x = (float)xPosition + g_CreditsQuadVertexScale;
            bgBRPtr->y = (float)positionY + quadHeight;
            bgBRPtr->zBits = depth;
            bgBRPtr->rhwBits = FLOAT_ONE_BITS;
            bgBRPtr->color = backColor;
            bgBRPtr->specular = 0xFF000000;
            bgBRPtr->u = uCoord + texW;
            bgBRPtr->v = vBase + texH;
        }
        pairIndexA = g_CreditsAnimationRecords[recordIdx].pairIndexA;
        depth = 0;
        if (AcquireBatchNode(1, (void *)(g_CreditsQuadBatchListsA[pairIndexA])) != 0) {
            xPosition = g_CreditsAnimationRecords[recordIdx].positionX + g_CreditsViewportX;
            positionY = g_CreditsAnimationRecords[recordIdx].positionY + g_CreditsViewportY;
            pForeTL = (GfxTlVertex *)(((GfxRenderObject *)g_CreditsQuadBatchListsA[pairIndexA])->vertexCursor);
            pForeTL->x = (float)xPosition;
            pForeTL->y = (float)positionY;
            pForeTL->zBits = depth;
            pForeTL->rhwBits = FLOAT_ONE_BITS;
            pForeTL->color = fgColor;
            pForeTL->specular = 0xFF000000;
            pForeTL->u = uCoord;
            pForeTL->v = vBase;
            fgBLPtr = (GfxTlVertex *)(((GfxRenderObject *)g_CreditsQuadBatchListsA[pairIndexA])->vertexCursor + 0x20);
            fgBLPtr->x = (float)xPosition;
            fgBLPtr->y = (float)positionY + quadHeight;
            fgBLPtr->zBits = depth;
            fgBLPtr->rhwBits = FLOAT_ONE_BITS;
            fgBLPtr->color = fgColor;
            fgBLPtr->specular = 0xFF000000;
            fgBLPtr->u = uCoord;
            fgBLPtr->v = vBase + texH;
            fgTopRight = (GfxTlVertex *)(((GfxRenderObject *)g_CreditsQuadBatchListsA[pairIndexA])->vertexCursor + 0x40);
            fgTopRight->x = (float)xPosition + g_CreditsQuadVertexScale;
            fgTopRight->y = (float)positionY;
            fgTopRight->zBits = depth;
            fgTopRight->rhwBits = FLOAT_ONE_BITS;
            fgTopRight->color = fgColor;
            fgTopRight->specular = 0xFF000000;
            fgTopRight->u = uCoord + texW;
            fgTopRight->v = vBase;
            pBottomRightFore = (GfxTlVertex *)(((GfxRenderObject *)g_CreditsQuadBatchListsA[pairIndexA])->vertexCursor + 0x60);
            pBottomRightFore->x = (float)xPosition + g_CreditsQuadVertexScale;
            pBottomRightFore->y = (float)positionY + quadHeight;
            pBottomRightFore->zBits = depth;
            pBottomRightFore->rhwBits = FLOAT_ONE_BITS;
            pBottomRightFore->color = fgColor;
            pBottomRightFore->specular = 0xFF000000;
            pBottomRightFore->u = uCoord + texW;
            pBottomRightFore->v = vBase + texH;
        }
        Credits_AdvanceAnimationRecord(recordIdx);
    }
}

/*
 * Initializes 60 credits animation records from 32-byte source entries,
 * setting paired coordinates/targets, absolute travel thresholds, four step
 * values, linkage/mode and atlas- related constants. Non-single-scale mode
 * groups every four records and uses the existing coordinate multiplier; mode
 * one forces multiplier one, groups every eight and arithmetic-shifts step
 * values by one.
 */
// FUNCTION: FF7 0x007a6106
void __cdecl Credits_InitAnimationRecords(void)
{
    int pairGroup;
    int recordIndex;
    int zero;
    int tailFlag;
    int tailSlot;
    int special;
    int endIndex;
    int preset;
    int colorBits;

    pairGroup = 0;
    zero = 0;
    if (g_CreditsSmallViewport != 1) {
        for (recordIndex = 0; recordIndex < 60; recordIndex++) {
            if (((recordIndex % 4) == 0 & recordIndex != 0) != 0) {
                pairGroup++;
            }
            g_CreditsAnimationRecords[recordIndex].pairIndexA = pairGroup;
            g_CreditsAnimationRecords[recordIndex].pairIndexB = pairGroup;
            if (recordIndex == 54 || (recordIndex == 58 || recordIndex == 59)) {
                (g_CreditsAnimationRecords[recordIndex].pairIndexA)++;
                (g_CreditsAnimationRecords[recordIndex].pairIndexB)++;
            }
            g_CreditsAnimationRecords[recordIndex].positionX = g_CreditsActiveAnimationPointer[recordIndex].start0 * g_CreditsLayoutScale;
            g_CreditsAnimationRecords[recordIndex].positionY = g_CreditsActiveAnimationPointer[recordIndex].start1 * g_CreditsLayoutScale;
            g_CreditsAnimationRecords[recordIndex].current8 = g_CreditsActiveAnimationPointer[recordIndex].start2 * g_CreditsLayoutScale + 1;
            g_CreditsAnimationRecords[recordIndex].currentC = g_CreditsActiveAnimationPointer[recordIndex].start3 * g_CreditsLayoutScale + 1;
            if ((recordIndex % 2) == 0) {
                g_CreditsAnimationRecords[recordIndex].threshold = abs(g_CreditsAnimationRecords[recordIndex].positionX - g_CreditsActiveAnimationPointer[recordIndex].end0 * g_CreditsLayoutScale);
                g_CreditsAnimationRecords[recordIndex].maxDistance = abs(g_CreditsAnimationRecords[recordIndex].current8 - g_CreditsActiveAnimationPointer[recordIndex].end2 * g_CreditsLayoutScale);
                g_CreditsAnimationRecords[recordIndex].target0 = g_CreditsActiveAnimationPointer[recordIndex].end0 * g_CreditsLayoutScale;
                g_CreditsAnimationRecords[recordIndex].target8 = g_CreditsActiveAnimationPointer[recordIndex].end2 * g_CreditsLayoutScale;
                g_CreditsAnimationRecords[recordIndex].target4 = g_CreditsAnimationRecords[recordIndex].positionY;
                g_CreditsAnimationRecords[recordIndex].targetC = g_CreditsAnimationRecords[recordIndex].currentC;
            } else {
                if ((recordIndex % 4) == 1 || recordIndex == 39) {
                    g_CreditsAnimationRecords[recordIndex].threshold = -(abs(g_CreditsAnimationRecords[recordIndex].positionY - g_CreditsActiveAnimationPointer[recordIndex].end1 * g_CreditsLayoutScale));
                } else {
                    g_CreditsAnimationRecords[recordIndex].threshold = abs(g_CreditsAnimationRecords[recordIndex].positionY - g_CreditsActiveAnimationPointer[recordIndex].end1 * g_CreditsLayoutScale);
                }
                g_CreditsAnimationRecords[recordIndex].maxDistance = abs(g_CreditsAnimationRecords[recordIndex].currentC - g_CreditsActiveAnimationPointer[recordIndex].end3 * g_CreditsLayoutScale);
                g_CreditsAnimationRecords[recordIndex].target0 = g_CreditsAnimationRecords[recordIndex].positionX;
                g_CreditsAnimationRecords[recordIndex].target4 = g_CreditsActiveAnimationPointer[recordIndex].end1 * g_CreditsLayoutScale;
                g_CreditsAnimationRecords[recordIndex].target8 = g_CreditsAnimationRecords[recordIndex].current8;
                g_CreditsAnimationRecords[recordIndex].targetC = g_CreditsActiveAnimationPointer[recordIndex].end3 * g_CreditsLayoutScale;
            }
            if (recordIndex > 37 && recordIndex < 40) {
                endIndex = (recordIndex == 39) + 4;
                g_CreditsAnimationRecords[recordIndex].velocity0 = g_CreditsEndVelocity0[endIndex];
                g_CreditsAnimationRecords[recordIndex].velocity8 = g_CreditsEndVelocity8[endIndex];
                g_CreditsAnimationRecords[recordIndex].velocity4 = g_CreditsEndVelocity4[endIndex];
                g_CreditsAnimationRecords[recordIndex].velocityC = g_CreditsEndVelocityC[endIndex];
                g_CreditsAnimationRecords[recordIndex].uCoordBits = 0;
                if (recordIndex == 39) {
                    colorBits = 0x3F0D0000;
                } else {
                    colorBits = 0x3ECC0000;
                }
                g_CreditsAnimationRecords[recordIndex].vBaseBits = colorBits;
                g_CreditsAnimationRecords[recordIndex].texWidthBits = FLOAT_ONE_BITS;
                g_CreditsAnimationRecords[recordIndex].texHeightBits = 0x3E1C0000;
                g_CreditsAnimationRecords[recordIndex].shade = 0x27;
                g_CreditsAnimationRecords[recordIndex].mode = g_CreditsEndModes[endIndex];
                g_CreditsAnimationRecords[recordIndex].flag = recordIndex == 39;
            } else {
                if (recordIndex != 37 && recordIndex != 56 && recordIndex != 57 && recordIndex != 58) {
                    preset = 0;
                } else {
                    preset = 1;
                }
                special = preset;
                g_CreditsAnimationRecords[recordIndex].velocity0 = g_CreditsEndVelocity0[(recordIndex % 8)];
                g_CreditsAnimationRecords[recordIndex].velocity8 = g_CreditsEndVelocity8[(recordIndex % 8)];
                g_CreditsAnimationRecords[recordIndex].velocity4 = g_CreditsEndVelocity4[(recordIndex % 8)];
                g_CreditsAnimationRecords[recordIndex].velocityC = g_CreditsEndVelocityC[(recordIndex % 8)];
                g_CreditsAnimationRecords[recordIndex].uCoordBits = 0;
                g_CreditsAnimationRecords[recordIndex].vBaseBits = g_CreditsEndVBaseBitsQuarter[(recordIndex % 4)];
                g_CreditsAnimationRecords[recordIndex].texWidthBits = FLOAT_ONE_BITS;
                if (recordIndex == 54) {
                    g_CreditsAnimationRecords[recordIndex].vBaseBits = 0;
                }
                if (recordIndex == 56) {
                    g_CreditsAnimationRecords[recordIndex].vBaseBits = FLOAT_HALF_BITS;
                }
                if (recordIndex == 57) {
                    g_CreditsAnimationRecords[recordIndex].vBaseBits = 0x3F380000;
                }
                if (recordIndex == 58) {
                    g_CreditsAnimationRecords[recordIndex].vBaseBits = FLOAT_HALF_BITS;
                }
                if (recordIndex == 59) {
                    g_CreditsAnimationRecords[recordIndex].vBaseBits = FLOAT_0_25_BITS;
                }
                if (special != 0) {
                    /* Numeric credits strip array index within 0..0x3B. */
                    switch (recordIndex) {
                    case 0x25:
                        g_CreditsAnimationRecords[recordIndex].texHeightBits = 0x3E1C0000;
                        g_CreditsAnimationRecords[recordIndex].shade = 0x27;
                        break;
                    case 0x38:
                        g_CreditsAnimationRecords[recordIndex].texHeightBits = 0x3E580000;
                        g_CreditsAnimationRecords[recordIndex].shade = 0x36;
                        break;
                    case 0x39:
                    case 0x3A:
                        g_CreditsAnimationRecords[recordIndex].texHeightBits = 0x3E900000;
                        g_CreditsAnimationRecords[recordIndex].shade = 0x48;
                    }
                } else {
                    g_CreditsAnimationRecords[recordIndex].texHeightBits = FLOAT_0_25_BITS;
                    g_CreditsAnimationRecords[recordIndex].shade = 0x40;
                }
                g_CreditsAnimationRecords[recordIndex].mode = g_CreditsEndModes[(recordIndex % 8)];
                g_CreditsAnimationRecords[recordIndex].flag = (recordIndex % 2) != 0;
            }
        }
    } else {
        g_CreditsLayoutScale = 1;
        for (recordIndex = 0; recordIndex < 60; recordIndex++) {
            if (((recordIndex % 8) == 0 & recordIndex != 0) != 0) {
                pairGroup++;
            }
            g_CreditsAnimationRecords[recordIndex].pairIndexA = pairGroup;
            g_CreditsAnimationRecords[recordIndex].pairIndexB = pairGroup;
            if (recordIndex == 54) {
                (g_CreditsAnimationRecords[recordIndex].pairIndexA)++;
                (g_CreditsAnimationRecords[recordIndex].pairIndexB)++;
            }
            g_CreditsAnimationRecords[recordIndex].positionX = g_CreditsActiveAnimationPointer[recordIndex].start0 * g_CreditsLayoutScale;
            g_CreditsAnimationRecords[recordIndex].positionY = g_CreditsActiveAnimationPointer[recordIndex].start1 * g_CreditsLayoutScale;
            g_CreditsAnimationRecords[recordIndex].current8 = g_CreditsActiveAnimationPointer[recordIndex].start2 * g_CreditsLayoutScale + 1;
            g_CreditsAnimationRecords[recordIndex].currentC = g_CreditsActiveAnimationPointer[recordIndex].start3 * g_CreditsLayoutScale + 1;
            if ((recordIndex % 2) == 0) {
                g_CreditsAnimationRecords[recordIndex].threshold = abs(g_CreditsAnimationRecords[recordIndex].positionX - g_CreditsActiveAnimationPointer[recordIndex].end0 * g_CreditsLayoutScale);
                g_CreditsAnimationRecords[recordIndex].maxDistance = abs(g_CreditsAnimationRecords[recordIndex].current8 - g_CreditsActiveAnimationPointer[recordIndex].end2 * g_CreditsLayoutScale);
                g_CreditsAnimationRecords[recordIndex].target0 = g_CreditsActiveAnimationPointer[recordIndex].end0 * g_CreditsLayoutScale;
                g_CreditsAnimationRecords[recordIndex].target8 = g_CreditsActiveAnimationPointer[recordIndex].end2 * g_CreditsLayoutScale;
                g_CreditsAnimationRecords[recordIndex].target4 = g_CreditsAnimationRecords[recordIndex].positionY;
                g_CreditsAnimationRecords[recordIndex].targetC = g_CreditsAnimationRecords[recordIndex].currentC;
            } else {
                if ((recordIndex % 4) == 1 || recordIndex == 39) {
                    g_CreditsAnimationRecords[recordIndex].threshold = -(abs(g_CreditsAnimationRecords[recordIndex].positionY - g_CreditsActiveAnimationPointer[recordIndex].end1 * g_CreditsLayoutScale));
                } else {
                    g_CreditsAnimationRecords[recordIndex].threshold = abs(g_CreditsAnimationRecords[recordIndex].positionY - g_CreditsActiveAnimationPointer[recordIndex].end1 * g_CreditsLayoutScale);
                }
                g_CreditsAnimationRecords[recordIndex].maxDistance = abs(g_CreditsAnimationRecords[recordIndex].currentC - g_CreditsActiveAnimationPointer[recordIndex].end3 * g_CreditsLayoutScale);
                g_CreditsAnimationRecords[recordIndex].target0 = g_CreditsAnimationRecords[recordIndex].positionX;
                g_CreditsAnimationRecords[recordIndex].target4 = g_CreditsActiveAnimationPointer[recordIndex].end1 * g_CreditsLayoutScale;
                g_CreditsAnimationRecords[recordIndex].target8 = g_CreditsAnimationRecords[recordIndex].current8;
                g_CreditsAnimationRecords[recordIndex].targetC = g_CreditsActiveAnimationPointer[recordIndex].end3 * g_CreditsLayoutScale;
            }
            if (recordIndex > 37 && recordIndex < 40) {
                tailSlot = (recordIndex == 39) + 4;
                g_CreditsAnimationRecords[recordIndex].velocity0 = g_CreditsEndVelocity0[tailSlot] >> 1;
                g_CreditsAnimationRecords[recordIndex].velocity8 = g_CreditsEndVelocity8[tailSlot] >> 1;
                g_CreditsAnimationRecords[recordIndex].velocity4 = g_CreditsEndVelocity4[tailSlot] >> 1;
                g_CreditsAnimationRecords[recordIndex].velocityC = g_CreditsEndVelocityC[tailSlot] >> 1;
                g_CreditsAnimationRecords[recordIndex].uCoordBits = 0;
                g_CreditsAnimationRecords[recordIndex].vBase = (recordIndex == 39) ? 0.859375f : 0.7265625f;
                g_CreditsAnimationRecords[recordIndex].texWidthBits = FLOAT_ONE_BITS;
                g_CreditsAnimationRecords[recordIndex].texHeightBits = 0x3DB00000;
                g_CreditsAnimationRecords[recordIndex].shade = 0x16;
                g_CreditsAnimationRecords[recordIndex].mode = g_CreditsEndModes[tailSlot];
                g_CreditsAnimationRecords[recordIndex].flag = recordIndex == 39;
            } else {
                tailFlag = recordIndex == 37 || recordIndex == 54 || recordIndex == 56 || recordIndex == 57 || recordIndex == 58;
                g_CreditsAnimationRecords[recordIndex].velocity0 = g_CreditsEndVelocity0[(recordIndex % 8)] >> 1;
                g_CreditsAnimationRecords[recordIndex].velocity8 = g_CreditsEndVelocity8[(recordIndex % 8)] >> 1;
                g_CreditsAnimationRecords[recordIndex].velocity4 = g_CreditsEndVelocity4[(recordIndex % 8)] >> 1;
                g_CreditsAnimationRecords[recordIndex].velocityC = g_CreditsEndVelocityC[(recordIndex % 8)] >> 1;
                g_CreditsAnimationRecords[recordIndex].uCoordBits = 0;
                g_CreditsAnimationRecords[recordIndex].vBaseBits = g_CreditsEndVBaseBitsEighth[(recordIndex % 8)];
                g_CreditsAnimationRecords[recordIndex].texWidthBits = FLOAT_ONE_BITS;
                if (recordIndex == 59) {
                    g_CreditsAnimationRecords[recordIndex].vBaseBits = FLOAT_0_625_BITS;
                }
                if (tailFlag != 0) {
                    /* Numeric credits strip array index within 0..0x3B. */
                    switch (recordIndex) {
                    case 0x25:
                        g_CreditsAnimationRecords[recordIndex].texHeightBits = 0x3DB00000;
                        g_CreditsAnimationRecords[recordIndex].shade = 0x16;
                        break;
                    case 0x36:
                        g_CreditsAnimationRecords[recordIndex].vBaseBits = 0;
                        g_CreditsAnimationRecords[recordIndex].texHeightBits = 0x3E180000;
                        g_CreditsAnimationRecords[recordIndex].shade = 0x26;
                        break;
                    case 0x38:
                        g_CreditsAnimationRecords[recordIndex].vBaseBits = FLOAT_0_25_BITS;
                        g_CreditsAnimationRecords[recordIndex].texHeightBits = 0x3DD00000;
                        g_CreditsAnimationRecords[recordIndex].shade = 0x1A;
                        break;
                    case 0x39:
                        g_CreditsAnimationRecords[recordIndex].vBaseBits = 0x3EB40000;
                        g_CreditsAnimationRecords[recordIndex].texHeightBits = 0x3E300000;
                        g_CreditsAnimationRecords[recordIndex].shade = 0x2C;
                        break;
                    case 0x3A:
                        g_CreditsAnimationRecords[recordIndex].vBaseBits = FLOAT_0_75_BITS;
                        g_CreditsAnimationRecords[recordIndex].texHeightBits = 0x3E180000;
                        g_CreditsAnimationRecords[recordIndex].shade = 0x26;
                    }
                } else {
                    g_CreditsAnimationRecords[recordIndex].texHeightBits = FLOAT_0_125_BITS;
                    g_CreditsAnimationRecords[recordIndex].shade = 0x20;
                }
                g_CreditsAnimationRecords[recordIndex].mode = g_CreditsEndModes[(recordIndex % 8)];
                g_CreditsAnimationRecords[recordIndex].flag = (recordIndex % 2) != 0;
            }
        }
    }
}

/*
 * Busy-waits using timestamp differences until elapsed time reaches the
 * credits tick duration times the supplied multiplier. Performs at least one
 * measurement after taking the starting timestamp; the engine context lookup
 * is unused.
 */
// FUNCTION: FF7 0x007a6fa4
void __cdecl Credits_BusyWaitTicks(double multiplier)
{
    Timestamp64 startStamp;
    double tickPeriod;
    GfxEngineContext *context;
    double targetDuration;
    Timestamp64 now;
    double dstElapsed;
    Timestamp64 stampDelta;

    context = GetEngineContext();
    dstElapsed = 0.0;
    tickPeriod = g_CreditsTickDuration;
    targetDuration = tickPeriod * multiplier;
    ReadTimestampCounter(&startStamp);
    do {
        ReadTimestampCounter(&now);
        Sub64(&now, &startStamp, &stampDelta);
        dstElapsed = CombineSplitValueToFloat(&stampDelta);
    } while (dstElapsed < targetDuration);
}

/*
 * Credits teardown-and-restart helper: calls Credits_OpenMovie(1), clears
 * g_CreditsTotalTickCounter, then calls Credits_InitAnimationRecords and
 * Credits_CreateRenderObjectSets. Mechanical -- the sequence is the whole
 * behaviour.
 */
// FUNCTION: FF7 0x007a7026
void __cdecl Credits_ReopenMovieAndRecreateRenderSets(void)
{
    Credits_OpenMovie(1);
    g_CreditsTotalTickCounter = 0;
    Credits_InitAnimationRecords();
    Credits_CreateRenderObjectSets();
}

/*
 * Two-way dispatch on g_CreditsSmallViewport, the credits layout variant flag:
 * calls Credits_DrawBackground1x when it is 1 and Credits_DrawBackground2x
 * otherwise. Nothing else is touched.
 */
// FUNCTION: FF7 0x007a7049
void __cdecl Credits_DrawBackgroundForLayout(void)
{
    if (g_CreditsSmallViewport != 1) {
        Credits_DrawBackground2x();
    } else {
        Credits_DrawBackground1x();
    }
}

/*
 * Advances the credits/movie sequence through the CreditsSequenceState states.
 * CREDITS_SEQ_MOVIE, _LOGO_MOVIE and _FIRST_MOVIE start a movie once (g_CreditsMovieRunning)
 * and release it on stop; CREDITS_SEQ_MOVIE enters the timed
 * credits, CREDITS_SEQ_LOGO_MOVIE draws a scaling-selected overlay with a byte alpha
 * from the movie helper and begins its hold, and CREDITS_SEQ_FIRST_MOVIE returns to
 * the initial movie selection.
 */
// FUNCTION: FF7 0x007a7063
void __cdecl Credits_TickSequenceState(void)
{
    switch (g_CreditsSequenceState) {
    case CREDITS_SEQ_MOVIE:
        if (g_CreditsMovieRunning == 0) {
            Movie_Run();
            g_CreditsMovieRunning = 1;
        }
        if (Movie_IsStopRequested() != 0) {
            Movie_ReleaseGraph();
            g_CreditsMovieRunning = 0;
            g_CreditsSequenceState = CREDITS_SEQ_START_ROLL;
            Credits_InitAnimationRecords();
            SoundDispatchCommand(SOUND_MUSIC_VOLUME, 0x7F, 0, 0, 0, 0, 0, 0, 0);
        }
        break;
    case CREDITS_SEQ_START_ROLL:
        SoundDispatchCommand(SOUND_MUSIC_PLAY, 0x5F, 0, 0, 0, 0, 0, 0, 0);
        Credits_BusyWaitTicks(3000.0);
        Credits_SetFadeOverlayRect(g_CreditsViewportX, g_CreditsViewportY, g_CreditsViewportWidth, g_CreditsViewportHeight);
        Credits_StartFadeOverlayFromOpaque(10);
        g_CreditsSequenceState = CREDITS_SEQ_ROLL;
        g_CreditsTextDisplayTimer = 0;
        break;
    case CREDITS_SEQ_ROLL:
        if (g_CreditsTotalTickCounter >= 29) {
            Credits_OpenMovie(2);
            g_CreditsSequenceState = CREDITS_SEQ_LOGO_MOVIE;
            g_CreditsTextDisplayTimer = 1;
            Credits_ReleaseRenderObjectSets();
            Credits_CreateFourRenderObjects();
        } else {
            Credits_DrawBackgroundForLayout();
            Credits_EmitRecordQuadPairs();
        }
        break;
    case CREDITS_SEQ_LOGO_MOVIE:
        if (g_CreditsMovieRunning == 0) {
            Movie_Run();
            g_CreditsMovieRunning = 1;
        }
        g_CreditsMovieCurrentFrame = Movie_GetCurrentFrame();
        *(unsigned char *)&g_CreditsMovieAlpha = (unsigned char)(g_CreditsMovieCurrentFrame * 8.5f);
        if (g_CreditsSmallViewport != 1) {
            Credits_DrawLogoWithAlpha2x(*(unsigned char *)&g_CreditsMovieAlpha);
        } else {
            Credits_DrawLogoWithAlpha1x(*(unsigned char *)&g_CreditsMovieAlpha);
        }
        if (Movie_IsStopRequested() != 0) {
            Movie_ReleaseGraph();
            g_CreditsMovieRunning = 0;
            g_CreditsSequenceState = CREDITS_SEQ_LOGO_HOLD;
            ReadTimestampCounter(&g_CreditsFrameTimestampData[0]);
            Credits_StoreScaledTickDuration(3000.0);
        }
        break;
    case CREDITS_SEQ_LOGO_HOLD:
        ReadTimestampCounter(&g_CreditsFrameTimestampData[1]);
        Sub64(&g_CreditsFrameTimestampData[1], &g_CreditsFrameTimestampData[0], &g_CreditsFrameTimestampData[2]);
        g_CreditsFrameElapsedFraction = CombineSplitValueToFloat(&g_CreditsFrameTimestampData[2]);
        if (g_CreditsSmallViewport != 1) {
            Credits_DrawLogoWithAlpha2x(0xFF);
            if (g_CreditsFrameElapsedFraction > g_CreditsScaledTickDuration) {
                Credits_SetFadeOverlayRect(160, 408, 320, 32);
                Credits_DrawLogoStrip2x();
                Credits_StartFadeOverlayFromOpaque(4);
                g_CreditsSequenceState = CREDITS_SEQ_LOGO_STRIP;
                ReadTimestampCounter(&g_CreditsFrameTimestampData[0]);
                Credits_StoreScaledTickDuration(16000.0);
            }
        } else {
            Credits_DrawLogoWithAlpha1x(0xFF);
            if (g_CreditsFrameElapsedFraction > g_CreditsScaledTickDuration) {
                Credits_SetFadeOverlayRect(g_CreditsViewportX + 0x50, g_CreditsViewportY + 0xCC, 160, 16);
                Credits_DrawLogoStrip1x();
                Credits_StartFadeOverlayFromOpaque(4);
                g_CreditsSequenceState = CREDITS_SEQ_LOGO_STRIP;
                ReadTimestampCounter(&g_CreditsFrameTimestampData[0]);
                Credits_StoreScaledTickDuration(16000.0);
            }
        }
        break;
    case CREDITS_SEQ_LOGO_STRIP:
        ReadTimestampCounter(&g_CreditsFrameTimestampData[1]);
        Sub64(&g_CreditsFrameTimestampData[1], &g_CreditsFrameTimestampData[0], &g_CreditsFrameTimestampData[2]);
        g_CreditsFrameElapsedFraction = CombineSplitValueToFloat(&g_CreditsFrameTimestampData[2]);
        if (g_CreditsSmallViewport != 1) {
            Credits_DrawLogoWithAlpha2x(0xFF);
            Credits_DrawLogoStrip2x();
        } else {
            Credits_DrawLogoWithAlpha1x(0xFF);
            Credits_DrawLogoStrip1x();
        }
        if (g_CreditsFrameElapsedFraction > g_CreditsScaledTickDuration) {
            Credits_SetFadeOverlayRect(g_CreditsViewportX, g_CreditsViewportY, g_CreditsViewportWidth, g_CreditsViewportHeight);
            Credits_StartFadeOverlayFromClear(0xA);
            g_CreditsSequenceState = CREDITS_SEQ_FADE_OUT;
            SoundDispatchCommand(SOUND_MUSIC_VOLUME_TRANS, 0x3C, 0, 0, 0, 0, 0, 0, 0);
        }
        break;
    case CREDITS_SEQ_FADE_OUT:
        if (Credits_IsFadeOverlayActive() == 0) {
            SoundDispatchCommand(SOUND_MUSIC_STOP, 0, 0, 0, 0, 0, 0, 0, 0);
            Credits_ReleaseFourRenderObjects();
            Credits_ReopenMovieAndRecreateRenderSets();
            g_CreditsSequenceState = CREDITS_SEQ_MOVIE;
        } else if (g_CreditsSmallViewport != 1) {
            Credits_DrawLogoWithAlpha2x(0xFF);
            Credits_DrawLogoStrip2x();
        } else {
            Credits_DrawLogoWithAlpha1x(0xFF);
            Credits_DrawLogoStrip1x();
        }
        break;
    case CREDITS_SEQ_FIRST_MOVIE:
        if (g_CreditsMovieRunning == 0) {
            Movie_Run();
            g_CreditsMovieRunning = 1;
        }
        if (Movie_IsStopRequested() != 0) {
            Movie_ReleaseGraph();
            g_CreditsMovieRunning = 0;
            g_CreditsSequenceState = CREDITS_SEQ_MOVIE;
            Credits_OpenMovie(1);
        }
        break;
    }
    if (g_CreditsTextScrollMode == 1) {
        Credits_SetFadeOverlayRect(g_CreditsViewportX, g_CreditsViewportY, g_CreditsViewportWidth, g_CreditsViewportHeight);
    }
}

/*
 * Draws eleven consecutive credits records through the indexed renderer table,
 * selecting a function offset from UI scaling mode. Accumulates record heights
 * into their Y positions and stores the first record height in
 * g_CreditsFrameDurationTarget.
 */
// FUNCTION: FF7 0x007a753a
void __cdecl Credits_DrawRecordWindow(void)
{
    // Single-letter locals preserve the five original VC5 stack slots.
    int drawIdx;
    int drawOffset;
    int yOffset;
    int yCopy;
    int rowIndex;

    drawOffset = (((GetUiScalingMode() - MAIN_GRAPHICS_MODE_640X480) == 0) - 1) & 3;
    yOffset = 0;
    for (rowIndex = 0; rowIndex < 11; rowIndex++) {
        drawIdx = g_CreditsScriptDataBase[rowIndex + g_CreditsSelectedSectionIndex].textOffset + drawOffset;
        yCopy = yOffset;
        /* Invokes untyped callback slot `g_CreditsGlyphDrawFunctions[drawIdx]` with its runtime (int, int, int, int) -> void signature. */
        ((void(__cdecl *)(int, int, int, int))g_CreditsGlyphDrawFunctions[drawIdx])(
            g_CreditsScriptDataBase[rowIndex + g_CreditsSelectedSectionIndex].posX,
            yCopy + g_CreditsFadeStepOffset,
            (int)g_CreditsScriptDataBase[rowIndex + g_CreditsSelectedSectionIndex].text,
            g_CreditsScriptDataBase[rowIndex + g_CreditsSelectedSectionIndex].renderOffset);
        yOffset += g_CreditsScriptDataBase[rowIndex + g_CreditsSelectedSectionIndex].duration;
        if (rowIndex == 0) {
            g_CreditsFrameDurationTarget = g_CreditsScriptDataBase[rowIndex + g_CreditsSelectedSectionIndex].duration;
        }
    }
}

/*
 * Drives the ending credits' g_CreditsTextPhase: the roll calls
 * Credits_DrawRecordWindow; the after-roll phase draws the after-text quads and,
 * when the fade strips complete, resets g_CreditsWindowTimer, configures the
 * viewport rectangle, begins a ten-step fade and advances to the fade-out. The
 * fade-out draws the quads only while Credits_IsFadeOverlayActive remains nonzero;
 * on zero, opens movie 3 and advances to the end movie, which ends the module.
 */
// FUNCTION: FF7 0x007a7636
void __cdecl Credits_TickTextPhase(void)
{
    switch (g_CreditsTextPhase) {
    case CREDITS_TEXT_PHASE_ROLL:
        Credits_DrawRecordWindow();
        break;
    case CREDITS_TEXT_PHASE_AFTER_ROLL:
        if (g_CreditsSmallViewport != 1) {
            Credits_DrawAfterTextQuads2x();
        } else {
            Credits_DrawAfterTextQuads1x();
        }
        if (Credits_UpdateFadeStrips() != 0) {
            g_CreditsWindowTimer = 0;
            Credits_SetFadeOverlayRect(g_CreditsViewportX, g_CreditsViewportY, g_CreditsViewportWidth, g_CreditsViewportHeight);
            Credits_StartFadeOverlayFromClear(0xA);
            g_CreditsTextPhase = CREDITS_TEXT_PHASE_FADE_OUT;
        }
        break;
    case CREDITS_TEXT_PHASE_FADE_OUT:
        if (Credits_IsFadeOverlayActive() == 0) {
            Credits_OpenMovie(3);
            g_CreditsTextPhase = CREDITS_TEXT_PHASE_END_MOVIE;
        } else if (g_CreditsSmallViewport != 1) {
            Credits_DrawAfterTextQuads2x();
        } else {
            Credits_DrawAfterTextQuads1x();
        }
        break;
    case CREDITS_TEXT_PHASE_END_MOVIE:
        if (g_CreditsMovieRunning == 0) {
            Movie_Run();
            g_CreditsMovieRunning = 1;
        }
        if (Movie_IsStopRequested() != 0) {
            Movie_ReleaseGraph();
            g_CreditsMovieRunning = 0;
            g_CreditsEndOverlayFlag = 1;
        }
    }
}

/*
 * Calls Credits_TickSequenceState for the opening variant or
 * Credits_TickTextPhase for the ending variant, and does nothing for other values.
 */
// FUNCTION: FF7 0x007a7745
void __cdecl Credits_TickSelectedMode(void)
{
    switch (g_CreditsVariant) {
    case CREDITS_VARIANT_OPENING:
        Credits_TickSequenceState();
        break;
    case CREDITS_VARIANT_ENDING:
        Credits_TickTextPhase();
    }
}

/*
 * Logs credits entry, derives tick duration from context frequency and
 * configures the viewport origin/size for UI scale. In normal credits mode
 * initializes assets and text data, sets the texture width to 256 or 128,
 * selects setup from g_CreditsStartSection, resets transition state and
 * records that mode.
 */
// FUNCTION: FF7 0x007a776f
void __cdecl Credits_ModuleEnter(struct GfxEngineContext *ctx)
{
    int uiMode;
    int lowResFlag;
    int reduceShift;
    Gfx_DebugPrint(
        // STRING: FF7 0x0099c5c0
        "START OF CREDITS!!!\n");
    g_CreditsTickDuration = ctx->tscFrequency / 1000.0;
    uiMode = GetUiScalingMode();
    g_CreditsSequenceTickCount = 0;
    if (uiMode == MAIN_GRAPHICS_MODE_320X240) {
        g_CreditsViewportX = 0;
        g_CreditsViewportY = 0;
        g_CreditsViewportWidth = 320;
        g_CreditsViewportHeight = 240;
        g_CreditsSmallViewport = 1;
    } else if (uiMode == MAIN_GRAPHICS_MODE_320X240_CENTERED) {
        g_CreditsViewportX = 160;
        g_CreditsViewportY = 120;
        g_CreditsViewportWidth = 320;
        g_CreditsViewportHeight = 240;
        g_CreditsSmallViewport = 1;
    } else {
        g_CreditsViewportX = 0;
        g_CreditsViewportY = 0;
        g_CreditsViewportWidth = 640;
        g_CreditsViewportHeight = 480;
        g_CreditsSmallViewport = 0;
    }
    if (g_CreditsVariant == CREDITS_VARIANT_OPENING) {
        lowResFlag = uiMode != MAIN_GRAPHICS_MODE_640X480;
        Credits_InitMovieTimingVariant0();
        g_CreditsQuadVertexScale = 256 >> lowResFlag;
        if (g_CreditsSmallViewport != 1) {
            g_CreditsActiveAnimationPointer = (struct CreditsAnimationKey *)&g_CreditsAnimationTable;
        } else {
            g_CreditsActiveAnimationPointer = (struct CreditsAnimationKey *)&g_CreditsAnimationTable;
        }
        if (g_CreditsStartSection == CREDITS_SEQ_FIRST_MOVIE) {
            Credits_OpenMovie(0);
        } else {
            Credits_OpenMovie(1);
        }
        Credits_CreateRenderSetsAndObject();
        (g_CreditsSectionFlags.fadeActive) = 0;
        g_CreditsSectionTimer = (g_CreditsSectionFlags.fadeActive);
        (g_CreditsSectionFlags.fadeCounter) = g_CreditsSectionTimer;
        g_CreditsTotalTickCounter = 0;
        g_CreditsSequenceState = g_CreditsStartSection;
    } else {
        reduceShift = uiMode != MAIN_GRAPHICS_MODE_640X480;
        g_CreditsViewportY += 62 >> reduceShift;
        g_CreditsViewportWidth = 640 >> reduceShift;
        g_CreditsViewportHeight = 356 >> reduceShift;
        Credits_InitMovieTimingVariantNonZero();
        Credits_LoadPeopleBinAndCreateRenderObjects();
        g_CreditsTextPhase = CREDITS_TEXT_PHASE_ROLL;
        g_CreditsSelectedSectionIndex = 0;
        g_CreditsFadeStepOffset = 0;
        g_CreditsFrameDuration = ctx->tscFrequency / *(double *)&g_CreditsFrameDurationDivisorLo;
        g_CreditsScaleShift = 2 >> reduceShift;
        SoundDispatchCommand(SOUND_MUSIC_VOLUME, 127, 0, 0, 0, 0, 0, 0, 0);
        SoundDispatchCommand(SOUND_MUSIC_PLAY, 98, 0, 0, 0, 0, 0, 0, 0);
    }
}

/*
 * Exit handler for the credits module: clears the movie-running flag
 * g_CreditsMovieRunning and calls Movie_ReleaseGraph, then branches on the
 * variant g_CreditsVariant -- the opening variant calls
 * Credits_ReleaseRenderObjectSetsAndBackground and sets g_EnginePreviousMode to
 * 0x1B and g_EngineGameMode to 0x14, any other value calls
 * Credits_ReleasePeopleBinAndRenderObjects, clears the byte at g_FieldActorState.state.requestCode
 * and sets g_EnginePreviousMode to 0x1C and g_EngineGameMode to 0x13. Finally it
 * closes LGP archive 0xF and calls Gfx_DebugPrint with the END OF
 * CREDITS message.
 */
// FUNCTION: FF7 0x007a79be
void __cdecl Credits_ModuleExit(void)
{
    g_CreditsMovieRunning = 0;
    Movie_ReleaseGraph();
    if (g_CreditsVariant == CREDITS_VARIANT_OPENING) {
        Credits_ReleaseRenderObjectSetsAndBackground();
        g_EnginePreviousMode = GAME_MODE_CREDITS;
        g_EngineGameMode = GAME_MODE_TITLE_MENU;
    } else {
        Credits_ReleasePeopleBinAndRenderObjects();
        g_FieldActorState.state.requestCode = FIELD_REQUEST_NONE;
        g_EnginePreviousMode = GAME_MODE_ENDING_CREDITS;
        g_EngineGameMode = GAME_MODE_QUIT;
    }
    CloseLgpArchive(LGP_ARCHIVE_CREDITS);
    // STRING: FF7 0x0099c5d8
    Gfx_DebugPrint((const char *)"END OF CREDITS!!!\n");
}

/*
 * Empty credits mouse callback; reads no state and makes no calls.
 */
// FUNCTION: FF7 0x007a7a29
void __cdecl Credits_ModuleMouse(void)
{
    // The original is empty (five code bytes plus any alignment padding).
}

/*
 * Empty credits keyboard callback; reads no state and makes no calls.
 */
// FUNCTION: FF7 0x007a7a2e
void __cdecl Credits_ModuleKeyboard(void)
{
    // The original is empty (five code bytes plus any alignment padding).
}

/*
 * Runs the credits frame: optionally updates timestamp pacing, polls input,
 * resets draw pools and clears the background. On successful scene begin,
 * updates credits state, optionally blits the movie, advances the overlay and
 * marks exit when its closing operation completes, then draws and ends the
 * scene.
 */
// FUNCTION: FF7 0x007a7a33
void __cdecl Credits_ModuleFrame(struct GfxEngineContext *renderContext)
{
    GfxClearColorBgra clearColor;
    int flag;
    int uiShift;

    if (g_CreditsWindowTimer != 0) {
        if (g_CreditsSequenceTickCount != 0) {
            Credits_WaitForFrameDuration(renderContext);
        }
        g_CreditsSequenceTickCount = 1;
        ReadTimestampCounter(&g_CreditsTimestampFrameStart);
    }
    flag = 1;
    Input_Update(0);
    ResetFrameDrawPool(renderContext);
    ResetRenderPools(renderContext);
    clearColor.r = 0;
    clearColor.g = 0;
    clearColor.b = 0;
    clearColor.a = FLOAT_ONE_BITS;
    GfxSetBackgroundColorCall(&clearColor, renderContext);
    GfxClearCall(g_CreditsTextDisplayTimer, 1, 0, renderContext);
    if (((int (__cdecl *)(int scene, int context))Gfx_BeginSceneCall)(0, (int)renderContext) != 0) { /* reads eax after a void call, as the original does */
        Credits_TickSelectedMode();
        if (g_CreditsMovieRunning != 0) {
            MovieBltFrameToBackBuffer(GfxGetSurfaceByIndex(1));
        }
        Credits_UpdateAndDrawFadeOverlay();
        if (g_CreditsTextScrollMode == 1) {
            if (Credits_IsFadeOverlayActive() == 0) {
                g_CreditsEndOverlayFlag = 1;
                SoundDispatchCommand(SOUND_MUSIC_STOP, 0, 0, 0, 0, 0, 0, 0, 0);
            }
        }
        Credits_EmitRenderPasses(renderContext);
        Gfx_EndSceneCall(renderContext);
    }
    Gfx_StatsClearRecord(renderContext->statsRecord);
    if (g_CreditsVariant == CREDITS_VARIANT_OPENING) {
        if (Input_GetPressed(INPUT_ANY) != 0 && g_CreditsTextScrollMode != 1) {
            Credits_SetFadeOverlayRect(g_CreditsViewportX, g_CreditsViewportY, g_CreditsViewportWidth, g_CreditsViewportHeight);
            Credits_StartFadeOverlayFromClear(4);
            g_CreditsTextScrollMode = 1;
            SoundDispatchCommand(SOUND_MUSIC_VOLUME_TRANS, 0x3C, 0, 0, 0, 0, 0, 0, 0);
        }
    } else if (g_CreditsMoviePlaybackDone != 0) {
        g_CreditsFadeStepOffset -= g_CreditsScaleShift;
        if (g_CreditsFadeStepOffset <= -g_CreditsFrameDurationTarget) {
            g_CreditsFadeStepOffset = 0;
            if (g_CreditsSelectedSectionIndex != 528) {
                g_CreditsSelectedSectionIndex++;
            } else {
                uiShift = GetUiScalingMode() != MAIN_GRAPHICS_MODE_640X480;
                g_CreditsTextPhase = CREDITS_TEXT_PHASE_AFTER_ROLL;
                g_CreditsMoviePlaybackDone = 0;
                g_CreditsViewportY -= 62 >> uiShift;
                g_CreditsViewportHeight = 480 >> uiShift;
                Credits_InitParticleRecords();
                Credits_SetFrameDurationDivisor(0, 0x40240000);
                SoundDispatchCommand(SOUND_MUSIC_VOLUME_TRANS, 0x3C, 0, 0, 0, 0, 0, 0, 0);
            }
        }
    } else if (UNFOLDED(0) && g_CreditsTextScrollMode != 1) {
        Credits_SetFadeOverlayRect(g_CreditsViewportX, g_CreditsViewportY, g_CreditsViewportWidth, g_CreditsViewportHeight);
        Credits_StartFadeOverlayFromClear(4);
        g_CreditsTextScrollMode = 1;
        SoundDispatchCommand(SOUND_MUSIC_STOP, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    if (g_CreditsEndOverlayFlag != 0) {
        GetActiveModule((GameModule *)&g_CreditsActiveModuleInfo, renderContext);
        g_CreditsDispatcherEnter = GameModeDispatcher_Enter;
        g_CreditsAppMouseCallback = App_OnMouseMessage;
        g_CreditsAppKeyCallback = App_OnKeyMessage;
        g_CreditsDispatcherFrame = GameModeDispatcher_Frame;
        g_CreditsDispatcherExit = GameModeDispatcher_Exit;
        RequestModuleSwitch((GameModule *)&g_CreditsActiveModuleInfo, renderContext);
        g_CreditsEndOverlayFlag = 0;
    }
    SoundUpdateFades();
}

/*
 * Selects pass 0 and render state 2. For the opening variant, emits six
 * or two g_CreditsFadeOverlayBatch.backdropSlotA objects according to g_CreditsStartSection, then selects
 * pass 1 and emits sixteen or eight paired objects from
 * g_CreditsQuadBatchListB and g_CreditsQuadBatchListA followed by four or one
 * g_CreditsLogoQuadBatches objects.
 */
// FUNCTION: FF7 0x007a7d94
void __cdecl Credits_EmitRenderPasses(struct GfxEngineContext *renderContext)
{
    int i;

    Gfx_SetScenePassCall(0, (struct GfxEngineContext *)(renderContext));
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, (struct GfxEngineContext *)(renderContext));
    if (g_CreditsVariant == CREDITS_VARIANT_OPENING) {
        if (g_CreditsSmallViewport != 1) {
            for (i = 0; i < 6; i++) {
                Gfx_EmitPrimitiveBatchList((void *)(g_CreditsFadeOverlayBatch.backdropSlots[i]), renderContext);
            }
        } else {
            for (i = 0; i < 2; i++) {
                Gfx_EmitPrimitiveBatchList((void *)(g_CreditsFadeOverlayBatch.backdropSlots[i]), renderContext);
            }
        }
        Gfx_SetScenePassCall(1, (struct GfxEngineContext *)(renderContext));
        Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, (struct GfxEngineContext *)(renderContext));
        if (g_CreditsSmallViewport != 1) {
            for (i = 0; i < 16; i++) {
                Gfx_EmitPrimitiveBatchList((void *)(g_CreditsQuadBatchListsB[i]), renderContext);
                Gfx_EmitPrimitiveBatchList((void *)(g_CreditsQuadBatchListsA[i]), renderContext);
            }
            for (i = 0; i < 4; i++) {
                Gfx_EmitPrimitiveBatchList(g_CreditsLogoQuadBatchList[i], renderContext);
            }
        } else {
            for (i = 0; i < 8; i++) {
                Gfx_EmitPrimitiveBatchList((void *)(g_CreditsQuadBatchListsB[i]), renderContext);
                Gfx_EmitPrimitiveBatchList((void *)(g_CreditsQuadBatchListsA[i]), renderContext);
            }
            Gfx_EmitPrimitiveBatchList(g_CreditsLogoQuadBatches, renderContext);
        }
    } else {
        for (i = 0; i < 5; i++) {
            Gfx_EmitPrimitiveBatchList(g_CreditsQuadBatchListsD[i], renderContext);
        }
        Gfx_SetScenePassCall(1, (struct GfxEngineContext *)(renderContext));
        Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, (struct GfxEngineContext *)(renderContext));
    }
    Gfx_SetRenderStateCall(RENDER_STATE_DITHER, 1, (struct GfxEngineContext *)(renderContext));
    Gfx_EmitPrimitiveBatchList((void *)(g_CreditsFadeOverlayBatch.renderObject), renderContext);
    GfxSetViewportRect(g_CreditsViewportX, g_CreditsViewportY, g_CreditsViewportWidth, g_CreditsViewportHeight, renderContext);
}

/*
 * Measures timestamp elapsed from credits frame start. With delay flag clear
 * waits to the full frame duration only when not already late; with delay flag
 * set waits to duration minus stored excess.
 */
// FUNCTION: FF7 0x007a7fcb
void __cdecl Credits_WaitForFrameDuration(struct GfxEngineContext *context)
{
    double elapsed;
    Timestamp64 delta;

    elapsed = 0.0;
    ReadTimestampCounter(&g_CreditsTimestampNow);
    Sub64(&g_CreditsTimestampNow, &g_CreditsTimestampFrameStart, &delta);
    elapsed = CombineSplitValueToFloat(&delta);
    if (g_CreditsFrameDelayFlag[0] == 0) {
        if (elapsed <= g_CreditsFrameDuration) {
            while (elapsed < g_CreditsFrameDuration) {
                ReadTimestampCounter(&g_CreditsTimestampNow);
                Sub64(&g_CreditsTimestampNow, &g_CreditsTimestampFrameStart, &delta);
                elapsed = CombineSplitValueToFloat(&delta);
            }
            g_CreditsFrameDelayFlag[0] = 0;
            ((int *)&g_CreditsFrameExcessTime)[0] = 0;
            ((int *)&g_CreditsFrameExcessTime)[1] = 0;
        }
    } else {
        while (g_CreditsFrameDuration - g_CreditsFrameExcessTime > elapsed) {
            ReadTimestampCounter(&g_CreditsTimestampNow);
            Sub64(&g_CreditsTimestampNow, &g_CreditsTimestampFrameStart, &delta);
            elapsed = CombineSplitValueToFloat(&delta);
        }
        g_CreditsFrameDelayFlag[0] = 0;
        ((int *)&g_CreditsFrameExcessTime)[0] = 0;
        ((int *)&g_CreditsFrameExcessTime)[1] = 0;
    }
}

/*
 * Chooses which sequence the credits module plays next: stores `variant` in g_CreditsVariant.
 * GameModeDispatcher_Frame passes 0 for GAME_MODE_CREDITS (the opening logo movies and name cards
 * before the title menu) and 1 for GAME_MODE_ENDING_CREDITS (the staff roll from people.bin after
 * the ending). Credits_ModuleEnter, Credits_TickSelectedMode, the render passes and
 * Credits_ModuleExit branch on it (exit returns to the title menu after variant 0 and quits the
 * game after the other).
 */
// FUNCTION: FF7 0x007a80fa
void __cdecl Credits_SetVariant(int variant)
{
    g_CreditsVariant = variant;
}

/*
 * Chooses where the opening sequence (credits variant 0) starts: stores `startSection`, an
 * `enum CreditsSequenceState` value, in g_CreditsStartSection. GameModeDispatcher_Frame passes
 * CREDITS_SEQ_FIRST_MOVIE (7) at start-up, which plays movie 0 (eidoslogo.avi) first, and
 * CREDITS_SEQ_MOVIE (0) after a game over, which starts at movie 1 (sqlogo.avi).
 * Credits_ModuleEnter opens the movie for it and makes it the first sequence state; the render
 * passes also read it to pick six or two backdrop objects.
 */
// FUNCTION: FF7 0x007a8107
void __cdecl Credits_SetStartSection(int startSection)
{
    g_CreditsStartSection = startSection;
}

/*
 * Sets the credits scroll divisor. The two int arguments are the low and high
 * halves of an IEEE double stored at g_CreditsFrameDurationDivisorLo and
 * g_CreditsFrameDurationDivisorHi, and the function then divides the engine
 * context's tscFrequency by that double, leaving the quotient in
 * g_CreditsFrameDuration.
 */
// FUNCTION: FF7 0x007a8114
void __cdecl Credits_SetFrameDurationDivisor(int divisorLo, int divisorHi)
{
    GfxEngineContext *context;

    context = GetEngineContext();
    g_CreditsFrameDurationDivisorLo = divisorLo;
    g_CreditsFrameDurationDivisorHi = divisorHi;
    g_CreditsFrameDuration = context->tscFrequency / *(double *)&g_CreditsFrameDurationDivisorLo;
}
} /* extern "C" */
