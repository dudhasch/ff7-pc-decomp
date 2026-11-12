// src/ff7/submarine/sub_mode_batches.cpp
// Data-only object: .bss 0x00eed688..0x00f4d6a8. No code.
// A data-only member of the sub library, which its neighbour sub_record_flight_update.cpp never references:
// the submarine mode batch lists and the large grid buffer.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "ff7/api/submarine.h"
#include "lib/api/graphics.h"
#include "lib/api/sound.h"
#include "ff7/types/submarine.h"
#include "lib/enums/graphics.h"
#include "ff7/enums/submarine.h"
#include "lib/enums/sound.h"
#include <math.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <mmsystem.h>
#include "ff7/gte.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x00eed688
struct GfxRenderObject *g_SubmarineModeBatchList0 = 0;
// GLOBAL: FF7 0x00eed68c
struct GfxRenderObject *g_SubmarineModeBatchList1 = 0;
// GLOBAL: FF7 0x00eed690
struct GfxRenderObject *g_SubmarineModeBatchList2 = 0;
unsigned char g_bssPad_00eed694[4] = {0};
// GLOBAL: FF7 0x00eed698
struct SubmarineLargeGridBuffer g_SubmarineLargeGridBuffer = {0};
// clang-format on

} /* extern "C" */
