// src/ff7/highway/highway_input_state_polling.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Input handling for the highway minigame: it clears the input state, polls the held and
//   newly pressed buttons, and decodes the raw button bits into action and directional codes.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/file.h"
#include "ff7/enums/highway.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "ff7/api/highway.h"
#include "lib/api/file.h"
extern "C" {

#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x00d859bc
int g_HighwayInputRawMask = 0;
// GLOBAL: FF7 0x00d859c0
int g_HighwayInputAction = 0;
// GLOBAL: FF7 0x00d859c4
int g_HighwayInputDirection = 0;
// GLOBAL: FF7 0x00d859c8
int g_HighwayInputButtonMask = 0;
// GLOBAL: FF7 0x00d859cc
int g_HighwayInputLock = 0;
// clang-format on

#pragma intrinsic(pow)
#pragma intrinsic(sqrt)

#pragma intrinsic(memcpy)
#pragma function(memcpy)

/*
 * Resets the highway minigame input state variables to zero.
 */
// FUNCTION: FF7 0x006520c0
void __cdecl Mini_HighwayResetInputState(void)
{
    g_HighwayInputRawMask = 0;
    g_HighwayInputAction = 0;
    g_HighwayInputDirection = 0;
    g_HighwayInputButtonMask = 0;
    g_HighwayInputLock = 0;
}

/*
 * Decodes raw button bits into action and directional movement codes.
 */
// FUNCTION: FF7 0x006520f7
void __cdecl Mini_HighwayDecodeInputMask(void)
{
    g_HighwayInputDirection = 0;
    g_HighwayInputAction = g_HighwayInputDirection;
    g_HighwayInputButtonMask = g_HighwayInputRawMask;
    if (g_HighwayInputLock == 0) {
        if ((g_HighwayInputRawMask & HIGHWAY_BUTTON_SQUARE) != 0) {
            g_HighwayInputAction = HIGHWAY_ACTION_CIRCLE;
        }
        if ((g_HighwayInputRawMask & HIGHWAY_BUTTON_CIRCLE) != 0) {
            g_HighwayInputAction = HIGHWAY_ACTION_SQUARE;
        }
        if ((g_HighwayInputRawMask & HIGHWAY_BUTTON_UP) != 0) {
            g_HighwayInputDirection = HIGHWAY_DIR_UP;
        }
        if ((g_HighwayInputRawMask & HIGHWAY_BUTTON_DOWN) != 0) {
            g_HighwayInputDirection = HIGHWAY_DIR_DOWN;
        }
        if ((g_HighwayInputRawMask & HIGHWAY_BUTTON_RIGHT) != 0) {
            if ((g_HighwayInputRawMask & HIGHWAY_BUTTON_UP) != 0) {
                g_HighwayInputDirection = HIGHWAY_DIR_UP_RIGHT;
            } else if ((g_HighwayInputRawMask & HIGHWAY_BUTTON_DOWN) != 0) {
                g_HighwayInputDirection = HIGHWAY_DIR_DOWN_RIGHT;
            } else {
                g_HighwayInputDirection = HIGHWAY_DIR_RIGHT;
            }
        }
        if ((g_HighwayInputRawMask & HIGHWAY_BUTTON_LEFT) != 0) {
            if ((g_HighwayInputRawMask & HIGHWAY_BUTTON_UP) != 0) {
                g_HighwayInputDirection = HIGHWAY_DIR_UP_LEFT;
            } else if ((g_HighwayInputRawMask & HIGHWAY_BUTTON_DOWN) != 0) {
                g_HighwayInputDirection = HIGHWAY_DIR_DOWN_LEFT;
            } else {
                g_HighwayInputDirection = HIGHWAY_DIR_LEFT;
            }
        }
    }
}

/*
 * Polls held and pressed input buttons from the input system and builds the input snapshot.
 */
// FUNCTION: FF7 0x00652214
void __cdecl Mini_HighwayReadInput(void)
{
    g_HighwayInputButtonMask = 0;
    g_HighwayInputDirection = g_HighwayInputButtonMask;
    g_HighwayInputAction = g_HighwayInputDirection;
    if (g_HighwayInputLock == 0) {
        if (Input_GetPressed(INPUT_CIRCLE) != 0) {
            g_HighwayInputButtonMask |= HIGHWAY_BUTTON_CIRCLE;
            g_HighwayInputAction = HIGHWAY_ACTION_CIRCLE;
        }
        if (Input_GetPressed(INPUT_SQUARE) != 0) {
            g_HighwayInputAction = HIGHWAY_ACTION_SQUARE;
            g_HighwayInputButtonMask |= HIGHWAY_BUTTON_SQUARE;
        }
        if (Input_GetHeld(INPUT_UP) != 0) {
            g_HighwayInputDirection = HIGHWAY_DIR_UP;
            g_HighwayInputButtonMask |= HIGHWAY_BUTTON_UP;
        }
        if (Input_GetHeld(INPUT_DOWN) != 0) {
            g_HighwayInputDirection = HIGHWAY_DIR_DOWN;
            g_HighwayInputButtonMask |= HIGHWAY_BUTTON_DOWN;
        }
        if (Input_GetHeld(INPUT_RIGHT) != 0) {
            g_HighwayInputButtonMask |= HIGHWAY_BUTTON_RIGHT;
            if (Input_GetHeld(INPUT_UP) != 0) {
                g_HighwayInputDirection = HIGHWAY_DIR_UP_RIGHT;
            } else if (Input_GetHeld(INPUT_DOWN) != 0) {
                g_HighwayInputDirection = HIGHWAY_DIR_DOWN_RIGHT;
            } else {
                g_HighwayInputDirection = HIGHWAY_DIR_RIGHT;
            }
        }
        if (Input_GetHeld(INPUT_LEFT) != 0) {
            g_HighwayInputButtonMask |= HIGHWAY_BUTTON_LEFT;
            if (Input_GetHeld(INPUT_UP) != 0) {
                g_HighwayInputDirection = HIGHWAY_DIR_UP_LEFT;
            } else if (Input_GetHeld(INPUT_DOWN) != 0) {
                g_HighwayInputDirection = HIGHWAY_DIR_DOWN_LEFT;
            } else {
                g_HighwayInputDirection = HIGHWAY_DIR_LEFT;
            }
        }
    }
}
} /* extern "C" */
