// src/ff7/coaster/coaster_clip_state.cpp
// Data-only object: .bss 0x00c3f788..0x00c3f870. No code.
// A data-only member of the coaster library, which its neighbour coaster.cpp never references: the coaster's
// clip rectangle and saved directory (from 0x00C3F788: the run from 0x00C3F784 would start its 8-aligned
// section at 4 mod 8).

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/coaster.h"
#include "ff7/coaster_psxdata.h"
#include "ff7/coaster_renderer.h"
#include <windows.h>
#include <ddraw.h>
#include "ff7/api/coaster.h"
#include "lib/api/sound.h"
#include "lib/enums/sound.h"
#include "lib/enums/sound.h"
#include "lib/types/graphics.h"
#include "lib/crt.h"
#include <time.h>
#include "lib/api/graphics.h"
#include "ff7/enums/main.h"
#include "ff7/api/main.h"
#include "lib/api/file.h"
#include "lib/enums/file.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x00c3f788
int g_CoasterClipTop = 0;
unsigned char g_bssPad_00c3f78c[4] = {0};
unsigned char g_bssPad_00c3f790[24] = {0};
// GLOBAL: FF7 0x00c3f7a8
char g_CoasterSavedDirectory[200] = {0};
// clang-format on

} /* extern "C" */
