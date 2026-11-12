// src/ff7/src/menu/btlmenu/english/scripted_input_stream.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Reads controller input with an override from a scripted input stream: it sets, steps
//   and clears the stream and runs its commands. It also provides the held, pressed and raw input
//   masks and the popup gating for the menus.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <string.h>
#include <stdlib.h>
#include "ff7/types/menu.h"
#include "ff7/api/menu.h"
#include "lib/api/file.h"
#include "ff7/enums/menu.h"
#include "lib/enums/file.h"
#include "ff7/enums/main.h"
#include "lib/codegen.h"
extern "C" {

#pragma intrinsic(abs, memcpy, memset)
// clang-format off
// Leading 4-byte .bss word at 0x00DD1B98 ahead of g_InputScriptButtonState and 8-byte-aligned
// g_bssPad_00dd1ba0. Bytes +2 and +3 back the 1-byte pad status globals g_PadBufferStatus and
// g_PadBufferType, which cannot be standalone C definitions at offsets 2 and 3 mod 4
// under VC5's global alignment rules.
// GLOBAL: FF7 0x00dd1b98
unsigned char g_InputScriptPadState[4] = {0};
// GLOBAL: FF7 0x00dd1b9c
InputScriptButtonState g_InputScriptButtonState = {0};
unsigned char g_bssPad_00dd1ba0[28] = {0};
// GLOBAL: FF7 0x00dd1bbc
int g_InputScriptWaitFrames = 0;
// GLOBAL: FF7 0x00dd1bc0
short g_InputScriptCounter = 0;
unsigned char g_bssPad_00dd1bc4[4] = {0};
// GLOBAL: FF7 0x00dd1bc8
unsigned char *g_pInputScriptStream = 0;
unsigned char g_bssPad_00dd1bcc[4] = {0};
unsigned char g_bssPad_00dd1bd0[36] = {0};
// GLOBAL: FF7 0x00dd1bf4
unsigned char g_InputPopupGate = 0;
// GLOBAL: FF7 0x00dd1bf8
short g_InputScriptCommand = 0;
// GLOBAL: FF7 0x00dd1bfc
short g_InputScriptDelay = 0;
unsigned char g_bssPad_00dd1c00[10240] = {0};
// clang-format on

#define g_PadBufferStatus (*((unsigned char *)&g_InputScriptPadState + 2))
#define g_PadBufferType (*((unsigned char *)&g_InputScriptPadState + 3))


/*
 * Clears scripted input active state flag.
 */
// FUNCTION: FF7 0x00718200
void __cdecl Input_ClearScriptedInputActive(void)
{
    g_ScriptedInputActive = 0;
}

/*
 * Sets active scripted input stream pointer and counter.
 */
// FUNCTION: FF7 0x0071820f
void __cdecl Input_SetScriptedInputStream(int value)
{
    g_pInputScriptStream = (unsigned char *)value;
    g_InputScriptWaitFrames = 0x14;
}

/*
 * Reads next scripted input word and advances stream.
 */
// FUNCTION: FF7 0x00718226
int __cdecl Input_UpdateAndConsume(void)
{
    int pressed;

    Input_Update(0);
    pressed = g_InputPressedMask;
    g_InputHeldMask = 0;
    g_InputPressedMask = g_InputHeldMask;
    return pressed;
}

/*
 * Clears popup gating flag if no popup active.
 */
// FUNCTION: FF7 0x00718259
void __cdecl Input_ClearPopupGateIfIdle(void)
{
    if (Menu_IsPopupActive() == 0) {
        g_InputPopupGate = 0;
    }
}

/*
 * Reads user input controllers with scripted override support.
 */
// FUNCTION: FF7 0x0071826e
unsigned int __cdecl Input_ReadWithScriptOverride(void)
{
    int spare;
    int raw;
    int hi;

    if (g_ScriptedInputActive == 0) { /* empty first arm: lib/codegen.h */
    } else {
        raw = Input_StepScriptedInputStream();
        g_InputPressedMask = raw;
        g_InputHeldMask = g_InputPressedMask;
    }
    if (g_PadBufferStatus != 0xff && g_PadBufferType == 0x41) {
        hi = ~(g_InputScriptButtonState.buttonLowByte | (g_InputScriptButtonState.buttonHighByte << 8));
    } else {
        hi = 0;
    }
    return ((unsigned int)raw) | (hi << 0x10);
}

/*
 * Steps scripted input stream delay counters.
 */
// FUNCTION: FF7 0x007182e9
int __cdecl Input_StepScriptedInputStream(void)
{
    int result;

    result = 0;
    if (g_InputScriptWaitFrames != 0) {
        g_InputScriptWaitFrames--;
    } else if (Menu_GetPhase() == 0 || Menu_GetPhase() == 1) {
        result = Input_ReadScriptedInputCommand();
    }
    return result;
}

/*
 * Dispatches scripted input commands and script state transitions.
 */
// FUNCTION: FF7 0x0071832e
int __cdecl Input_ReadScriptedInputCommand(void)
{
    unsigned char command;
    int buttonMask;

    buttonMask = 0;
    if (g_InputPopupGate != 0) {
        Input_ClearPopupGateIfIdle();
    } else {
        command = *(char *)g_pInputScriptStream;
        g_pInputScriptStream++;
        switch (command) {
        case MENU_INPUT_SCRIPT_WAIT:
            g_InputScriptWaitFrames = *g_pInputScriptStream;
            g_pInputScriptStream++;
            g_InputScriptWaitFrames += *g_pInputScriptStream << 8;
            g_pInputScriptStream++;
            break;
        case MENU_INPUT_SCRIPT_PRESS_UP:
            buttonMask = INPUT_UP;
            g_InputScriptWaitFrames = 0x14;
            break;
        case MENU_INPUT_SCRIPT_PRESS_DOWN:
            buttonMask = INPUT_DOWN;
            g_InputScriptWaitFrames = 0x14;
            break;
        case MENU_INPUT_SCRIPT_PRESS_LEFT:
            buttonMask = INPUT_LEFT;
            g_InputScriptWaitFrames = 0x14;
            break;
        case MENU_INPUT_SCRIPT_PRESS_RIGHT:
            buttonMask = INPUT_RIGHT;
            g_InputScriptWaitFrames = 0x14;
            break;
        case MENU_INPUT_SCRIPT_PRESS_SQUARE:
            buttonMask = INPUT_SQUARE;
            g_InputScriptWaitFrames = 0x14;
            break;
        case MENU_INPUT_SCRIPT_PRESS_CIRCLE:
            buttonMask = INPUT_CIRCLE;
            g_InputScriptWaitFrames = 0x14;
            break;
        case MENU_INPUT_SCRIPT_PRESS_TRIANGLE:
            buttonMask = INPUT_TRIANGLE;
            g_InputScriptWaitFrames = 0x14;
            break;
        case MENU_INPUT_SCRIPT_PRESS_CROSS:
            buttonMask = INPUT_CROSS;
            g_InputScriptWaitFrames = 0x14;
            break;
        case MENU_INPUT_SCRIPT_PRESS_R1:
            buttonMask = INPUT_R1;
            g_InputScriptWaitFrames = 0x14;
            break;
        case MENU_INPUT_SCRIPT_PRESS_R2:
            buttonMask = INPUT_R2;
            g_InputScriptWaitFrames = 0x14;
            break;
        case MENU_INPUT_SCRIPT_PRESS_L1:
            buttonMask = INPUT_L1;
            g_InputScriptWaitFrames = 0x14;
            break;
        case MENU_INPUT_SCRIPT_PRESS_L2:
            buttonMask = INPUT_L2;
            g_InputScriptWaitFrames = 0x14;
            break;
        case MENU_INPUT_SCRIPT_SHOW_TEXT:
            g_pInputScriptStream = Menu_ShowPopupTextAndAdvance(g_pInputScriptStream);
            g_InputScriptWaitFrames = 0x50;
            break;
        case MENU_INPUT_SCRIPT_SET_POPUP_POS:
            g_InputScriptCommand = *g_pInputScriptStream;
            g_pInputScriptStream++;
            g_InputScriptCommand += (short)(*g_pInputScriptStream << 8);
            g_pInputScriptStream++;
            g_InputScriptDelay = *g_pInputScriptStream;
            g_pInputScriptStream++;
            g_InputScriptDelay += (short)(*g_pInputScriptStream << 8);
            g_pInputScriptStream++;
            g_InputScriptWaitFrames = 0x3C;
            break;
        case MENU_INPUT_SCRIPT_END:
            Menu_SetPopupPlacement(0, 0, 0);
            g_ScriptedInputActive = 0;
            g_InputScriptWaitFrames = 0;
        }
    }
    return buttonMask;
}

/*
 * Displays popup message window and advances pointer.
 */
// FUNCTION: FF7 0x00718613
unsigned char *__cdecl Menu_ShowPopupTextAndAdvance(unsigned char *text)
{
    int i;

    g_InputPopupGate = 1;
    Menu_SetPopupPlacement(1, g_InputScriptCommand, g_InputScriptDelay);
    Menu_OpenPopupMessage(text, 7);
    for (i = 0; i < 0x100; i++) {
        switch (*text) {
        case TEXT_FONT_PAGE_1:
        case TEXT_FONT_PAGE_2:
        case TEXT_FONT_PAGE_3:
        case TEXT_FONT_PAGE_4:
        case TEXT_EXTENDED:
            text++;
            text++;
            break;
        default:
            text++;
            break;
        }
        if (*text == TEXT_END) {
            text++;
            break;
        }
    }
    return text;
}

/*
 * Returns unmasked raw held button mask.
 */
// FUNCTION: FF7 0x007186b9
int __cdecl Input_GetHeldRaw(void)
{
    return g_InputHeldMask;
}

/*
 * Empty no-op input hook function.
 */
// FUNCTION: FF7 0x007186c3
void __cdecl Input_UnusedStreamNoOp(void)
{
    /* The original is empty; an empty C body is the whole of it. */
}

/*
 * Snapshots input button states to field system variables.
 */
// FUNCTION: FF7 0x007186c8
void __cdecl Input_SnapshotToFieldGlobals(void)
{
    Input_ReadWithScriptOverride();
    g_InputPressedSnapshot = g_InputPressedMask;
    g_InputHeldSnapshot = g_InputHeldMask;
}

/*
 * Clears pressed input button mask.
 */
// FUNCTION: FF7 0x007186e8
void __cdecl Input_ClearPressed(void)
{
    g_InputPressedMask = 0;
}

/*
 * Clears held input button mask.
 */
// FUNCTION: FF7 0x007186f7
void __cdecl Input_ClearHeld(void)
{
    g_InputHeldMask = 0;
}
} /* extern "C" */
