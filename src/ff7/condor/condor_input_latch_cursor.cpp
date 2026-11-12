// src/ff7/condor/condor_input_latch_cursor.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Handles input for the Fort Condor minigame: latches button presses, tests held and
//   pressed buttons through alternate keyboard bindings, and tracks cursor position and motion with
//   history and clamping.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/condor.h"
#include "lib/types/graphics.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/api/condor.h"
#include "lib/api/graphics.h"
#include "lib/api/file.h"
#include "lib/enums/file.h"
extern "C" {

#pragma intrinsic(strcpy)
#pragma intrinsic(strcat)
#pragma intrinsic(strlen)

#pragma intrinsic(memset)

#pragma intrinsic(abs)

// clang-format off
// Alternate keyboard bindings: 14 three-dword records (DIK scan code, secondary key, action code).
// GLOBAL: FF7 0x00905480
int g_CondorAltKeyBindings[14][3] = {
    {0xcb, 0, 0xe5}, {0xd0, 0, 0xe4}, {0xcd, 0, 0xe6}, {0xc8, 0, 0xe3},
    {0x44, 0, 0xeb}, {0x3f, 0, 0xec}, {0x3c, 0, 0xed}, {0x36, 0, 0xee},
    {0x39, 0, 0xef}, {0x3e, 0, 0xf0}, {0x40, 0, 0xf1}, {0x41, 0, 0xf2},
    {0x42, 0, 0xf3}, {0x43, 0, 0xf4}
};
// clang-format on

/*
 * Clears all 14 latched button input flags.
 */
// FUNCTION: FF7 0x005fab70
void __cdecl Input_ClearLatchedFlags(void)
{
    g_CondorInputLeft = 0;
    g_CondorInputUp = 0;
    g_CondorInputDown = 0;
    g_CondorInputRight = 0;
    g_CondorInputSquare = 0;
    g_CondorInputCircle = 0;
    g_CondorInputTriangle = 0;
    g_CondorInputCross = 0;
    g_CondorInputR1 = 0;
    g_CondorInputR2 = 0;
    g_CondorInputL1 = 0;
    g_CondorInputL2 = 0;
    g_CondorInputStart = 0;
    g_CondorInputSelect = 0;
}

/*
 * Applies alternate keyboard bindings for 14 buttons.
 */
// FUNCTION: FF7 0x005fac01
void __cdecl Input_ApplyAltKeyboardBindings_A(void)
{
    int mask;
    int buttonIndex;

    for (buttonIndex = 0; buttonIndex < 0xE; buttonIndex++) {
        mask = Input_AltBindingIndexToMask(buttonIndex);
        Input_SetBinding(0, mask, g_CondorAltKeyBindings[buttonIndex][0]);
    }
}

/*
 * Maps alternate key binding index to button bitmask.
 */
// FUNCTION: FF7 0x005fac4f
int __cdecl Input_AltBindingIndexToMask(int bindingIndex)
{
    switch (bindingIndex) {
    case INPUT_BINDING_LEFT:
        return INPUT_LEFT;
    case INPUT_BINDING_RIGHT:
        return INPUT_DOWN;
    case INPUT_BINDING_UP:
        return INPUT_RIGHT;
    case INPUT_BINDING_DOWN:
        return INPUT_UP;
    case INPUT_BINDING_TRIANGLE:
        return INPUT_START;
    case INPUT_BINDING_CIRCLE:
        return INPUT_SELECT;
    case INPUT_BINDING_CROSS:
        return INPUT_SQUARE;
    case INPUT_BINDING_SQUARE:
        return INPUT_CROSS;
    case INPUT_BINDING_SELECT:
        return INPUT_CIRCLE;
    case INPUT_BINDING_START:
        return INPUT_TRIANGLE;
    case INPUT_BINDING_L1:
        return INPUT_R1;
    case INPUT_BINDING_R1:
        return INPUT_R2;
    case INPUT_BINDING_R2:
        return INPUT_L1;
    case INPUT_BINDING_L2:
        return INPUT_L2;
    default:
        return 0xC8;
    }
}

/*
 * Returns latched input flag for button g_CondorInputCircle.
 */
// FUNCTION: FF7 0x005fad0c
int __cdecl Input_GetLatchedCircle_1(void)
{
    return g_CondorInputCircle;
}

/*
 * Returns latched input flag for button g_CondorInputCircle.
 */
// FUNCTION: FF7 0x005fad16
int __cdecl Input_GetLatchedCircle_2(void)
{
    return g_CondorInputCircle;
}

/*
 * Returns latched input flag for button g_CondorInputSquare.
 */
// FUNCTION: FF7 0x005fad20
int __cdecl Input_GetLatchedSquare(void)
{
    return g_CondorInputSquare;
}

/*
 * Returns latched input flag for button g_CondorInputTriangle.
 */
// FUNCTION: FF7 0x005fad2a
int __cdecl Input_GetLatchedTriangle(void)
{
    return g_CondorInputTriangle;
}

/*
 * Returns latched input flag for button g_CondorInputCross.
 */
// FUNCTION: FF7 0x005fad34
int __cdecl Input_GetLatchedCross(void)
{
    return g_CondorInputCross;
}

/*
 * Tests whether any of the buttons in mask 0x26 is held (result unused).
 * 0x26 is the Windows key code VK_UP, not an InputButton mask.
 */
// FUNCTION: FF7 0x005fad3e
void __cdecl Input_TestHeldMask26(void)
{
    Input_IsHeld(0x26);
}

/*
 * Tests whether any of the buttons in mask 0x28 is held (result unused).
 * 0x28 is the Windows key code VK_DOWN, not an InputButton mask.
 */
// FUNCTION: FF7 0x005fad4d
void __cdecl Input_TestHeldMask28(void)
{
    Input_IsHeld(0x28);
}

/*
 * Tests whether any of the buttons in mask 0x25 is held (result unused).
 * 0x25 is the Windows key code VK_LEFT, not an InputButton mask.
 */
// FUNCTION: FF7 0x005fad5c
void __cdecl Input_TestHeldMask25(void)
{
    Input_IsHeld(0x25);
}

/*
 * Tests whether any of the buttons in mask 0x27 is held (result unused).
 * 0x27 is the Windows key code VK_RIGHT, not an InputButton mask.
 */
// FUNCTION: FF7 0x005fad6b
void __cdecl Input_TestHeldMask27(void)
{
    Input_IsHeld(0x27);
}

/*
 * Returns latched input flag for button g_CondorInputSelect.
 */
// FUNCTION: FF7 0x005fad7a
int __cdecl Input_GetLatchedSelect(void)
{
    return g_CondorInputSelect;
}

/*
 * Returns latched input flag for button g_CondorInputStart.
 */
// FUNCTION: FF7 0x005fad84
int __cdecl Input_GetLatchedStart(void)
{
    return g_CondorInputStart;
}

/*
 * Returns latched input flag for button g_CondorInputR2.
 */
// FUNCTION: FF7 0x005fad8e
int __cdecl Input_GetLatchedR2(void)
{
    return g_CondorInputR2;
}

/*
 * Returns latched input flag for button g_CondorInputR1.
 */
// FUNCTION: FF7 0x005fad98
int __cdecl Input_GetLatchedR1(void)
{
    return g_CondorInputR1;
}

/*
 * Returns latched input flag for button g_CondorInputL2.
 */
// FUNCTION: FF7 0x005fada2
int __cdecl Input_GetLatchedL2(void)
{
    return g_CondorInputL2;
}

/*
 * Returns latched input flag for button g_CondorInputL1.
 */
// FUNCTION: FF7 0x005fadac
int __cdecl Input_GetLatchedL1(void)
{
    return g_CondorInputL1;
}

/*
 * Tests whether specified buttons are currently held down.
 */
// FUNCTION: FF7 0x005fadb6
int __cdecl Input_IsHeld(int buttons)
{
    if (Input_GetHeld(buttons) != 0) {
        return 1;
    } else {
        return 0;
    }
}

/*
 * Reads current input state and latches button flags and cursor motion.
 */
// FUNCTION: FF7 0x005fadd4
void __cdecl Input_LatchButtonFlags(void)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    Input_Update(ctx);
    if (Input_IsHeld(INPUT_UP) != 0) {
        g_CondorInputUp = 1;
    }
    if (Input_IsHeld(INPUT_DOWN) != 0) {
        g_CondorInputDown = 1;
    }
    if (Input_IsHeld(INPUT_LEFT) != 0) {
        g_CondorInputLeft = 1;
    }
    if (Input_IsHeld(INPUT_RIGHT) != 0) {
        g_CondorInputRight = 1;
    }
    if (Input_IsPressed(INPUT_SQUARE) != 0) {
        g_CondorInputSquare = 1;
    }
    if (Input_IsPressed(INPUT_CIRCLE) != 0) {
        g_CondorInputCircle = 1;
    }
    if (Input_IsPressed(INPUT_TRIANGLE) != 0) {
        g_CondorInputTriangle = 1;
    }
    if (Input_IsPressed(INPUT_SELECT) != 0) {
        g_CondorInputSelect = 1;
    }
    if (Input_IsHeld(INPUT_R1) != 0) {
        g_CondorInputR1 = 1;
    }
    if (Input_IsPressed(INPUT_R2) != 0) {
        g_CondorInputR2 = 1;
    }
    if (Input_IsHeld(INPUT_L1) != 0) {
        g_CondorInputL1 = 1;
    }
    if (Input_IsPressed(INPUT_L2) != 0) {
        g_CondorInputL2 = 1;
    }
    if (Input_IsHeld(INPUT_CROSS) != 0) {
        g_CondorInputCross = 1;
    }
    if (Input_IsPressed(INPUT_START) != 0) {
        g_CondorInputStart = 1;
    }
    if (g_CondorCursorMode != 0) {
        if (Input_IsMouseButtonDown(INPUT_SOURCE_MOUSE_BUTTON_0) != 0) {
            g_CondorInputCircle = 1;
        }
        if (Input_IsMouseButtonDown(INPUT_SOURCE_MOUSE_BUTTON_1) != 0) {
            g_CondorInputCross = 1;
        }
        if (g_CondorPhase == 1 || g_CondorPhase == 2) {
            if (g_CondorCursorResetFlag != 0) {
                Input_BeginCursorTracking();
            } else {
                Input_UpdateCursorMotion();
            }
        }
    }
}

/*
 * Tests whether specified buttons were pressed this frame.
 */
// FUNCTION: FF7 0x005fafbb
int __cdecl Input_IsPressed(int buttons)
{
    if (Input_GetPressed(buttons) != 0) {
        return 1;
    } else {
        return 0;
    }
}

/*
 * Reads current cursor coordinates from engine context.
 */
// FUNCTION: FF7 0x005fafd9
void __cdecl Input_ReadCursorPos(CondorCursorPos *cursor)
{
    GfxEngineContext *context;

    context = GetEngineContext();
    cursor->cursorX = context->cursorX;
    cursor->cursorY = context->cursorY;
}

/*
 * Writes cursor coordinates to engine context.
 */
// FUNCTION: FF7 0x005fb006
void __cdecl Input_WriteCursorPos(int cursorX, int cursorY)
{
    GfxEngineContext *context;

    context = GetEngineContext();
    context->cursorX = cursorX;
    context->cursorY = cursorY;
}

/*
 * Updates cursor position, saving previous position history.
 */
// FUNCTION: FF7 0x005fb02e
void __cdecl Input_MoveCursorWithHistory(int newX, int newY)
{
    g_CondorCursorPrevX = g_CondorCursorPosX;
    g_CondorCursorPrevY = g_CondorCursorPosY;
    Input_WriteCursorPos(newX, newY);
    g_CondorCursorPosX = newX;
    g_CondorCursorPosY = newY;
}

/*
 * Clamps cursor position to valid screen bounds.
 */
// FUNCTION: FF7 0x005fb06b
void __cdecl Input_ClampCursorPosition(CondorCursorPos *cursor)
{
    int padSlot[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    Input_ReadCursorPos(cursor);
    if (cursor->cursorX < 0) {
        cursor->cursorX = 0;
    }
    if (cursor->cursorX > 0x1F0) {
        cursor->cursorX = 0x1F0;
    }
    if (cursor->cursorY < -256) {
        cursor->cursorY = 0xFFFFFF00;
    }
    if (cursor->cursorY > 0xF0) {
        cursor->cursorY = 0xF0;
    }
}

/*
 * Resets cursor position, motion delta, and mode state.
 */
// FUNCTION: FF7 0x005fb0d2
void __cdecl Input_ResetCursorState(void)
{
    g_CondorCursorMode = 0;
    g_CondorCursorTargetX = 0;
    g_CondorCursorTargetY = 0;
    g_CondorCursorPosX = 0;
    g_CondorCursorPosY = 0;
    g_CondorCursorSpeedX = 0;
    g_CondorCursorSpeedY = 0;
    g_CondorCursorPrevX = 0;
    g_CondorCursorPrevY = 0;
    g_CondorCursorResetFlag = 1;
}

/*
 * Resets cursor position and delta while preserving cursor mode.
 */
// FUNCTION: FF7 0x005fb13b
void __cdecl Input_ResetCursorStateKeepMode(void)
{
    g_CondorCursorTargetX = 0;
    g_CondorCursorTargetY = 0;
    g_CondorCursorPosX = 0;
    g_CondorCursorPosY = 0;
    g_CondorCursorSpeedX = 0;
    g_CondorCursorSpeedY = 0;
    g_CondorCursorPrevX = 0;
    g_CondorCursorPrevY = 0;
    g_CondorCursorResetFlag = 1;
}

/*
 * Initializes cursor tracking at default center position.
 */
// FUNCTION: FF7 0x005fb19a
void __cdecl Input_BeginCursorTracking(void)
{
    CondorCursorPos cursorX;

    Input_WriteCursorPos(0x80, 0x80);
    Input_ReadCursorPos(&cursorX);
    g_CondorCursorTargetX = cursorX.cursorX;
    g_CondorCursorTargetY = cursorX.cursorY;
    g_CondorCursorResetFlag = 0;
    g_CondorCursorPrevX = g_CondorCursorTargetX;
    g_CondorCursorPosX = g_CondorCursorPrevX;
    g_CondorCursorPrevY = g_CondorCursorTargetY;
    g_CondorCursorPosY = g_CondorCursorPrevY;
}

/*
 * Updates cursor motion deltas and speeds based on input state.
 */
// FUNCTION: FF7 0x005fb20a
void __cdecl Input_UpdateCursorMotion(void)
{
    int moved;
    CondorCursorPos clampedX;

    moved = 0;
    g_CondorCursorSpeedX = 1;
    g_CondorCursorSpeedY = 1;
    Input_ClampCursorPosition(&clampedX);
    if (clampedX.cursorX != g_CondorCursorPosX) {
        g_CondorCursorSpeedX = abs(clampedX.cursorX - g_CondorCursorPosX) / 2;
        if (g_CondorCursorSpeedX > 0) {
            if (clampedX.cursorX < g_CondorCursorPosX) {
                g_CondorInputLeft = 1;
            } else {
                g_CondorInputRight = 1;
            }
        }
        moved = 1;
    }
    if (clampedX.cursorY != g_CondorCursorPosY) {
        g_CondorCursorSpeedY = abs(clampedX.cursorY - g_CondorCursorPosY) / 2;
        if (g_CondorCursorSpeedY > 0) {
            if (clampedX.cursorY < g_CondorCursorPosY) {
                g_CondorInputUp = 1;
            } else {
                g_CondorInputDown = 1;
            }
        }
        moved = 1;
    }
    if (moved != 0) {
        Input_WriteCursorPos(0x80, 0x80);
    }
}
} /* extern "C" */
