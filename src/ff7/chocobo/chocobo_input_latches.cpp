// src/ff7/chocobo/chocobo_input_latches.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Input handling for the chocobo race: maps the minigame's input bindings to input bits,
//   installs the alternate keyboard bindings, polls and clears the input latches, and tests or
//   packs their states.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "lib/api/graphics.h"
#include "ff7/api/chocobo.h"
#include "lib/api/file.h"
#include "ff7/types/chocobo.h"
#include "lib/enums/file.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x0096e518
int g_ChocoboKeyboardBindings[14][3] = {203, 0, 229, 208, 0, 228, 205, 0, 230, 200, 0, 227, 68, 0, 235, 63, 0, 236, 60, 0, 237, 54, 0, 238, 57, 0, 239, 62, 0, 240, 64, 0, 241, 65, 0, 242, 66, 0, 243, 67, 0, 244};
// GLOBAL: FF7 0x00e71628
int g_ChocoboInputLatchLeft = 0;
// GLOBAL: FF7 0x00e7162c
int g_ChocoboInputLatchUp = 0;
// GLOBAL: FF7 0x00e71630
int g_ChocoboInputLatchDown = 0;
// GLOBAL: FF7 0x00e71634
int g_ChocoboInputLatchRight = 0;
unsigned char g_bssPad_00e71638[4] = {0};
// GLOBAL: FF7 0x00e7163c
int g_ChocoboInputLatchSquare = 0;
// GLOBAL: FF7 0x00e71640
int g_ChocoboInputLatchCircle = 0;
// GLOBAL: FF7 0x00e71644
int g_ChocoboInputLatchTriangle = 0;
// GLOBAL: FF7 0x00e71648
int g_ChocoboInputLatchCross = 0;
// GLOBAL: FF7 0x00e7164c
int g_ChocoboInputLatchR1 = 0;
// GLOBAL: FF7 0x00e71650
int g_ChocoboInputLatchR2 = 0;
// GLOBAL: FF7 0x00e71654
int g_ChocoboInputLatchL1 = 0;
// GLOBAL: FF7 0x00e71658
int g_ChocoboInputLatchL2 = 0;
// GLOBAL: FF7 0x00e7165c
int g_ChocoboInputLatchStart = 0;
// GLOBAL: FF7 0x00e71660
int g_ChocoboInputState = 0;
// clang-format on
/*
 * Clears the fourteen input latches used by the Chocobo race.
 */
// FUNCTION: FF7 0x0076ddf0
void __cdecl Mini_ChocoboClearInputLatches(void)
{
    g_ChocoboInputLatchLeft = 0;
    g_ChocoboInputLatchUp = 0;
    g_ChocoboInputLatchDown = 0;
    g_ChocoboInputLatchRight = 0;
    g_ChocoboInputLatchSquare = 0;
    g_ChocoboInputLatchCircle = 0;
    g_ChocoboInputLatchTriangle = 0;
    g_ChocoboInputLatchCross = 0;
    g_ChocoboInputLatchR1 = 0;
    g_ChocoboInputLatchR2 = 0;
    g_ChocoboInputLatchL1 = 0;
    g_ChocoboInputLatchL2 = 0;
    g_ChocoboInputLatchStart = 0;
    g_ChocoboInputState = 0;
}
/*
 * Installs the fourteen alternate keyboard bindings.
 */
// FUNCTION: FF7 0x0076de81
void __cdecl Input_ApplyAltKeyboardBindings_B(void)
{
    int mask;
    int i;
    for (i = 0; i < 14; i++) {
        mask = Mini_ChocoboGetBindingInputMask(i);
        Input_SetBinding(0, mask, g_ChocoboKeyboardBindings[i][0]);
    }
}
/*
 * Maps binding index 0..13 (the rows of g_ChocoboKeyboardBindings) to the input buttons Left, Down,
 * Right, Up, Start, Select, Square, Cross, Circle, Triangle, R1, R2, L1 and L2. Returns decimal 200
 * for any other index.
 */
// FUNCTION: FF7 0x0076decf
int __cdecl Mini_ChocoboGetBindingInputMask(int index)
{
    switch (index) {
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
    }
    return 200;
}
/*
 * Updates input using the engine context, then latches fourteen minigame input flags from the
 * selected held/edge helper masks. Only sets flags for active input and does not clear them in
 * this routine; Triangle, Select (into g_ChocoboInputState), L2 and Start latch on a press, the
 * rest while held.
 */
// FUNCTION: FF7 0x0076df8c
void __cdecl Mini_ChocoboPollInputLatches(void)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    Input_Update(ctx);
    if (Mini_ChocoboIsInputHeld(INPUT_UP) != 0) {
        g_ChocoboInputLatchUp = 1;
    }
    if (Mini_ChocoboIsInputHeld(INPUT_DOWN) != 0) {
        g_ChocoboInputLatchDown = 1;
    }
    if (Mini_ChocoboIsInputHeld(INPUT_LEFT) != 0) {
        g_ChocoboInputLatchLeft = 1;
    }
    if (Mini_ChocoboIsInputHeld(INPUT_RIGHT) != 0) {
        g_ChocoboInputLatchRight = 1;
    }
    if (Mini_ChocoboIsInputHeld(INPUT_SQUARE) != 0) {
        g_ChocoboInputLatchSquare = 1;
    }
    if (Mini_ChocoboIsInputHeld(INPUT_CIRCLE) != 0) {
        g_ChocoboInputLatchCircle = 1;
    }
    if (Mini_ChocoboIsInputPressed(INPUT_TRIANGLE) != 0) {
        g_ChocoboInputLatchTriangle = 1;
    }
    if (Mini_ChocoboIsInputPressed(INPUT_SELECT) != 0) {
        g_ChocoboInputState = 1;
    }
    if (Mini_ChocoboIsInputHeld(INPUT_R1) != 0) {
        g_ChocoboInputLatchR1 = 1;
    }
    if (Mini_ChocoboIsInputHeld(INPUT_R2) != 0) {
        g_ChocoboInputLatchR2 = 1;
    }
    if (Mini_ChocoboIsInputHeld(INPUT_L1) != 0) {
        g_ChocoboInputLatchL1 = 1;
    }
    if (Mini_ChocoboIsInputPressed(INPUT_L2) != 0) {
        g_ChocoboInputLatchL2 = 1;
    }
    if (Mini_ChocoboIsInputHeld(INPUT_CROSS) != 0) {
        g_ChocoboInputLatchCross = 1;
    }
    if (Mini_ChocoboIsInputPressed(INPUT_START) != 0) {
        g_ChocoboInputLatchStart = 1;
    }
}
/*
 * Returns whether Input_GetPressed(mask) is nonzero.
 */
// FUNCTION: FF7 0x0076e10d
int __cdecl Mini_ChocoboIsInputPressed(int mask)
{
    if (Input_GetPressed(mask) != 0) {
        return 1;
    }
    return 0;
}
/*
 * Returns whether Input_GetHeld(mask) is nonzero.
 */
// FUNCTION: FF7 0x0076e12b
int __cdecl Mini_ChocoboIsInputHeld(int mask)
{
    if (Input_GetHeld(mask) != 0) {
        return 1;
    }
    return 0;
}
/*
 * ORs fourteen input-state globals into the packed word using shifts for bits 15, 14,
 * 13, 12, 11, 8, 7, 6, 5, 4, 3, 1 and 2, with the last value unshifted.
 */
// FUNCTION: FF7 0x0076e149
void __cdecl Mini_ChocoboPackInputLatches(void)
{
    *(int *)&g_ChocoboPackedInputLatches = g_ChocoboInputLatchLeft << 0xF | g_ChocoboInputLatchDown << 0xE | g_ChocoboInputLatchRight << 0xD | g_ChocoboInputLatchUp << 0xC | g_ChocoboInputLatchStart << 0xB | g_ChocoboInputState << 8 | g_ChocoboInputLatchSquare << 7 | g_ChocoboInputLatchCross << 6 | g_ChocoboInputLatchCircle << 5 | g_ChocoboInputLatchTriangle << 4 | g_ChocoboInputLatchR1 << 3 | g_ChocoboInputLatchR2 << 1 | g_ChocoboInputLatchL1 << 2 | g_ChocoboInputLatchL2;
}
} /* extern "C" */
