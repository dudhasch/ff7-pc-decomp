// src/ff7/src/menu/english/menu_load_fade.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The fade for the load menu: it steps the fade level, draws the full-screen fade quad,
//   and clamps the updated level.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/menu.h"
#include <stdlib.h>
#include "lib/crt.h"
#include <math.h>
#include <string.h>
#include "ff7/api/menu.h"
#include "lib/pfile.h"

#pragma intrinsic(abs)
#pragma intrinsic(memset)
extern "C" {

#pragma intrinsic(abs)
#pragma intrinsic(cos)
#pragma intrinsic(sin)
#pragma intrinsic(sqrt, atan2)

// The load menu's fade level (0..255).
// GLOBAL: FF7 0x00926288
int g_LoadMenuFadeLevel = 0xFF;

/*
 * Steps the load menu fade level, renders the full-screen fade quad,
 * and clamps the updated level to [0, 255]. Returns the previous level.
 */
// FUNCTION: FF7 0x00722bb0
int __cdecl Menu_LoadScreenFadeStep(int step)
{
    int prevLevel;

    Menu_FillViewportQuad(*(unsigned char *)&g_LoadMenuFadeLevel, 0.0f);
    prevLevel = g_LoadMenuFadeLevel;
    g_LoadMenuFadeLevel += step;
    if (g_LoadMenuFadeLevel < 0) {
        g_LoadMenuFadeLevel = 0;
    }
    if (g_LoadMenuFadeLevel > 0xFF) {
        g_LoadMenuFadeLevel = 0xFF;
    }
    return prevLevel;
}
} /* extern "C" */
