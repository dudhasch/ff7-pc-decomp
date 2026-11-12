// src/ff7/src/menu/english/menu_quit_screen.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The quit menu screen, whose init and per-frame functions have empty bodies that still
//   reserve stack space, plus an empty no-op hook.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <string.h>
#include <stdlib.h>
#include "ff7/types/menu.h"
#include "ff7/api/menu.h"
#include "lib/api/graphics.h"
extern "C" {

#pragma intrinsic(abs, memcpy, memset)

/*
 * Quit screen init: does nothing. The unused local keeps the original's 0x20-byte stack frame.
 */
// FUNCTION: FF7 0x006feb00
void __cdecl Menu_QuitScreenInit(void)
{
    int unused[8];
}

/*
 * A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006feb0a
void Menu_UnusedQuitScreenNoOp(void)
{
}

/*
 * Quit screen per-frame update: does nothing. The unused local keeps the original's 0x2C-byte stack frame.
 */
// FUNCTION: FF7 0x006feb0f
void __cdecl Menu_QuitScreenFrame(void)
{
    int unused[11];
}
} /* extern "C" */
