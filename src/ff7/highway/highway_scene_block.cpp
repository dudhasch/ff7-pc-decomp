// src/ff7/highway/highway_scene_block.cpp
// Data-only object: .bss 0x00d8d458..0x00d8d45c. No code.
// A data-only member of the highway library, which its neighbour highway_scene_camera_effects.cpp never
// references: the highway scene block pointer.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <string.h>
#include "ff7/highway_palette.h"
#include "ff7/highway_psxdata.h"
#include <math.h>
#include <stdlib.h>
#include "ff7/types/highway.h"
#include "lib/types/graphics.h"
#include "ff7/api/highway.h"
#include "ff7/api/snobo.h"
#include "lib/api/graphics.h"
#include "lib/pfile.h"
#include "ff7/enums/highway.h"
#include "lib/codegen.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x00d8d458
struct HighwaySceneBlock2 *g_pHighwaySceneBlock = 0;
// clang-format on

} /* extern "C" */
