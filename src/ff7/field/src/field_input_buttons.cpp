// src/ff7/field/src/field_input_buttons.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Tracks field button input: clears and updates the button latches, tests held and
//   pressed buttons, detects Ctrl+F2, and packs the controller state each frame.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "lib/enums/win32.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "ff7/types/field.h"
#include "ff7/api/field.h"
#include "lib/api/file.h"
#include "lib/api/graphics.h"
#include "lib/enums/file.h"
extern "C" {

__declspec(dllimport) short __stdcall GetAsyncKeyState(int key);
__declspec(dllimport) short __stdcall GetKeyState(int key);
#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x00d011a8
int g_InputLatchLeft = 0;
// GLOBAL: FF7 0x00d011ac
int g_InputLatchUp = 0;
// GLOBAL: FF7 0x00d011b0
int g_InputLatchDown = 0;
// GLOBAL: FF7 0x00d011b4
int g_InputLatchRight = 0;
unsigned char g_bssPad_00d011b8[4] = {0};
// GLOBAL: FF7 0x00d011bc
int g_InputLatchSquare = 0;
// GLOBAL: FF7 0x00d011c0
int g_InputLatchCircle = 0;
// GLOBAL: FF7 0x00d011c4
int g_InputLatchTriangle = 0;
// GLOBAL: FF7 0x00d011c8
int g_InputLatchCross = 0;
// GLOBAL: FF7 0x00d011cc
int g_InputLatchR1 = 0;
// GLOBAL: FF7 0x00d011d0
int g_InputLatchR2 = 0;
// GLOBAL: FF7 0x00d011d4
int g_InputLatchL1 = 0;
// GLOBAL: FF7 0x00d011d8
int g_InputLatchL2 = 0;
// GLOBAL: FF7 0x00d011dc
int g_InputLatchStart = 0;
// GLOBAL: FF7 0x00d011e0
int g_InputLatchSelect = 0;
// GLOBAL: FF7 0x00d011e4
struct FieldFxParams *g_pFieldFx = 0;
unsigned char g_bssPad_00d011e8[8] = {0};
// GLOBAL: FF7 0x00d011f0
struct FieldModelLightCache g_FieldModelLightCaches[18] = {0};
unsigned char g_bssPad_00d84f98[4] = {0};
// clang-format on

#pragma intrinsic(pow)
#pragma intrinsic(sqrt)

#pragma intrinsic(memcpy)
#pragma function(memcpy)

/*
 * Clears all 14 button latch status globals.
 */
// FUNCTION: FF7 0x00649770
void __cdecl Input_ClearButtonLatches(void)
{
    g_InputLatchLeft = 0;
    g_InputLatchUp = 0;
    g_InputLatchDown = 0;
    g_InputLatchRight = 0;
    g_InputLatchSquare = 0;
    g_InputLatchCircle = 0;
    g_InputLatchTriangle = 0;
    g_InputLatchCross = 0;
    g_InputLatchR1 = 0;
    g_InputLatchR2 = 0;
    g_InputLatchL1 = 0;
    g_InputLatchL2 = 0;
    g_InputLatchStart = 0;
    g_InputLatchSelect = 0;
}

/*
 * Tests whether the given button mask is held down.
 */
// FUNCTION: FF7 0x00649801
int __cdecl Input_TestHeld(int buttonMask)
{
    if (Input_GetHeld(buttonMask) != 0) {
        return 1;
    } else {
        return 0;
    }
}

/*
 * Tests whether the given button mask was pressed this frame.
 */
// FUNCTION: FF7 0x0064981f
int __cdecl Input_TestPressed(int buttonMask)
{
    if (Input_GetPressed(buttonMask) != 0) {
        return 1;
    } else {
        return 0;
    }
}

/*
 * Tests whether the Ctrl+F2 key combination is pressed.
 */
// FUNCTION: FF7 0x0064983d
int __cdecl Input_IsCtrlF2Down(void)
{
    short val;

    val = GetAsyncKeyState(VK_CONTROL);
    if (val < 0) {
        val = GetKeyState(VK_F2);
        if (val < 0) {
            return 1;
        }
    }
    return 0;
}

/*
 * Updates input device state and sets latch flags for all active buttons.
 */
// FUNCTION: FF7 0x00649876
void __cdecl Input_UpdateButtonLatches(void)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    Input_Update(ctx);
    if (Input_TestHeld(INPUT_UP) != 0) {
        g_InputLatchUp = 1;
    }
    if (Input_TestHeld(INPUT_DOWN) != 0) {
        g_InputLatchDown = 1;
    }
    if (Input_TestHeld(INPUT_LEFT) != 0) {
        g_InputLatchLeft = 1;
    }
    if (Input_TestHeld(INPUT_RIGHT) != 0) {
        g_InputLatchRight = 1;
    }
    if (Input_TestHeld(INPUT_SQUARE) != 0) {
        g_InputLatchSquare = 1;
    }
    if (Input_TestHeld(INPUT_CIRCLE) != 0) {
        g_InputLatchCircle = 1;
    }
    if (Input_TestHeld(INPUT_TRIANGLE) != 0) {
        g_InputLatchTriangle = 1;
    }
    if (Input_TestHeld(INPUT_START) != 0) {
        g_InputLatchStart = 1;
    }
    if (Input_TestPressed(INPUT_SELECT) != 0) {
        g_InputLatchSelect = 1;
    }
    if (Input_TestHeld(INPUT_R1) != 0) {
        g_InputLatchR1 = 1;
    }
    if (Input_TestPressed(INPUT_R2) != 0) {
        g_InputLatchR2 = 1;
    }
    if (Input_TestHeld(INPUT_L1) != 0) {
        g_InputLatchL1 = 1;
    }
    if (Input_TestPressed(INPUT_L2) != 0) {
        g_InputLatchL2 = 1;
    }
    if (Input_TestHeld(INPUT_CROSS) != 0) {
        g_InputLatchCross = 1;
    }
}

/*
 * Reads input latches, packs them into controller bit fields, and updates transition masks.
 */
// FUNCTION: FF7 0x006499f7
int __cdecl Field_ReadInput(short *scrollX, short *scrollY)
{
    int val;

    Input_UpdateButtonLatches();
    val = g_InputLatchLeft << 0xF | g_InputLatchDown << 0xE | g_InputLatchRight << 0xD | g_InputLatchUp << 0xC | g_InputLatchStart << 0xB | g_InputLatchSelect << 8 | g_InputLatchSquare << 7 | g_InputLatchCross << 6 | g_InputLatchCircle << 5 | g_InputLatchTriangle << 4 | g_InputLatchR1 << 3 | g_InputLatchR2 << 1 | g_InputLatchL1 << 2 | g_InputLatchL2;
    ((int *)&g_EnginePreviousMode)[28] = ((int *)&g_EnginePreviousMode)[27];
    ((int *)&g_EnginePreviousMode)[27] = val;
    ((int *)&g_EnginePreviousMode)[29] = (((int *)&g_EnginePreviousMode)[27] ^ ((int *)&g_EnginePreviousMode)[28]) & ((int *)&g_EnginePreviousMode)[27];
    ((int *)&g_EnginePreviousMode)[30] = (((int *)&g_EnginePreviousMode)[27] ^ ((int *)&g_EnginePreviousMode)[28]) & ~((int *)&g_EnginePreviousMode)[27];
    ((int *)&g_EnginePreviousMode)[32] = ((int *)&g_EnginePreviousMode)[31];
    ((int *)&g_EnginePreviousMode)[31] = val;
    ((int *)&g_EnginePreviousMode)[33] = (((int *)&g_EnginePreviousMode)[31] ^ ((int *)&g_EnginePreviousMode)[32]) & ((int *)&g_EnginePreviousMode)[31];
    ((int *)&g_EnginePreviousMode)[34] = (((int *)&g_EnginePreviousMode)[31] ^ ((int *)&g_EnginePreviousMode)[32]) & ~((int *)&g_EnginePreviousMode)[31];
    return ((int *)&g_EnginePreviousMode)[31];
}
} /* extern "C" */
