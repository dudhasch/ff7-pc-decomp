// src/lib/src/file/file_input_poll_bindings.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Sets up DirectInput and polls keyboard, mouse and joystick input, maps buttons through
//   the binding table with default bindings, and handles auto-repeat and cursor position.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <stdio.h>
#include "lib/api/file.h"
#include "lib/api/input.h"
#include "lib/api/token.h"
#include <string.h>
#include "lib/enums/file.h"
#include "lib/types/file.h"
#include "lib/types/graphics.h"
#include "lib/types/input.h"
extern "C" {

#pragma intrinsic(memset, strcpy, strcat)

__declspec(dllimport) unsigned int __stdcall timeGetTime(void);

// clang-format off
// GLOBAL: FF7 0x009a85c0
int g_InputCursorX = 0;
// GLOBAL: FF7 0x009a85c4
int g_InputCursorY = 0;
// GLOBAL: FF7 0x009a85c8
int g_InputRepeatFirstDelay = 0;
// GLOBAL: FF7 0x009a85cc
unsigned char *g_InputKeyboardStateBuffer = 0;
// GLOBAL: FF7 0x009a85d0
GfxEngineContext *g_InputContext = 0;
// GLOBAL: FF7 0x009a85d4
int g_InputHeldMask = 0;
// GLOBAL: FF7 0x009a85d8
int g_InputMouseButtonFlags = 0;
// GLOBAL: FF7 0x009a85dc
struct InputMouseState2 *g_InputMouseStatePointer = 0;
// GLOBAL: FF7 0x009a85e0
int g_InputPressedMask = 0;
// GLOBAL: FF7 0x009a85e4
int DAT_009a85e4 = 0;
// GLOBAL: FF7 0x009a85e8
int DAT_009a85e8[3][25] = {0};
// GLOBAL: FF7 0x009a8714
int g_InputRepeatNextThreshold = 0;
// GLOBAL: FF7 0x009a8718
int g_InputTimeGetTimeSample = 0;
// GLOBAL: FF7 0x009a871c
struct JoystickOutput *g_InputJoystickPollTable = 0;
// GLOBAL: FF7 0x009a8720
int g_InputLastTickSample = 0;
// GLOBAL: FF7 0x009a8724
int g_InputMouseRepeatMask = 0;
// Input_Update's previous-frame held bits (written before the binding scan, read for the pressed edges).
// GLOBAL: FF7 0x009a8728
int g_InputPrevHeldBits = 0;
// GLOBAL: FF7 0x009a872c
int g_InputRepeatActiveFlag = 0;
// GLOBAL: FF7 0x009a8730
int g_InputRepeatElapsedTicks = 0;
// GLOBAL: FF7 0x009a8734
int g_InputRepeatStateFlag = 0;
unsigned char g_rawData_009a8738[4] = {0};
// GLOBAL: FF7 0x009a873c
int g_InputMouseDeltaAccumulator = 0;
// GLOBAL: FF7 0x009a8740
int g_InputMouseDeltaSecondAccumulator = 0;
unsigned char g_rawData_009a8744[4] = {0};
// clang-format on

// Clears pressed/held masks, stores context as the input context and passes it to DI_InputInit, loads
// default bindings, initializes g_InputTimeGetTimeSample and g_InputLastTickSample from timeGetTime, and clears
// g_InputRepeatElapsedTicks.
// FUNCTION: FF7 0x0041a1b0
void __cdecl Input_Init(GfxEngineContext *context)
{
    g_InputPressedMask ^= g_InputPressedMask;
    g_InputHeldMask ^= g_InputHeldMask;
    g_InputContext = context;
    DI_InputInit(g_InputContext);
    Input_LoadAllDefaults();
    g_InputTimeGetTimeSample = timeGetTime();
    g_InputLastTickSample = g_InputTimeGetTimeSample;
    g_InputRepeatElapsedTicks = 0;
}

// Shuts DirectInput down.
// FUNCTION: FF7 0x0041a214
void __cdecl Input_Shutdown(void)
{
    DI_InputShutdown();
}

// Polls mouse, keyboard and joystick before saving a nonzero context or returning when no context
// is available. Adds mouse deltas to context XY and clamps to zero then width/height minus one,
// publishing the result. Saves prior held bits, clears held bits and ORs twenty-five actions
// across three mapping sets from keyboard high bits, three mouse buttons or designated joystick
// fields. Computes newly pressed bits as current minus prior overlap. With a nonzero repeat flag
// updates elapsed time and ticks repeat; mappings read j+1 through the written twenty-five-column
// view without a bound guard.
// FUNCTION: FF7 0x0041a21e
void __cdecl Input_Update(GfxEngineContext *ctx)
{
    int mapIdx;
    int actionIdx;

    g_InputMouseStatePointer = (InputMouseState2 *)DI_MouseGetState();
    g_InputKeyboardStateBuffer = DI_KeyboardGetState();
    g_InputJoystickPollTable = Joystick_Poll();
    if (ctx != 0) {
        g_InputContext = ctx;
    } else if (g_InputContext == 0) {
        return;
    }
    if (g_InputMouseStatePointer != 0) {
        g_InputContext->cursorX =
            g_InputContext->cursorX + g_InputMouseStatePointer->deltaX;
        if (g_InputContext->cursorX < 0) {
            g_InputContext->cursorX = 0;
        }
        if (g_InputContext->cursorX >= g_InputContext->displayWidth) {
            g_InputContext->cursorX = g_InputContext->displayWidth - 1;
        }
        g_InputContext->cursorY =
            g_InputContext->cursorY + g_InputMouseStatePointer->deltaY;
        if (g_InputContext->cursorY < 0) {
            g_InputContext->cursorY = 0;
        }
        if (g_InputContext->cursorY >= g_InputContext->displayHeight) {
            g_InputContext->cursorY = g_InputContext->displayHeight - 1;
        }
        g_InputCursorX = g_InputContext->cursorX;
        g_InputCursorY = g_InputContext->cursorY;
    }
    g_InputPrevHeldBits = g_InputHeldMask;
    g_InputHeldMask = g_InputHeldMask ^ g_InputHeldMask;
    for (mapIdx = 0; mapIdx < 3; mapIdx++) {
        for (actionIdx = 0; actionIdx < 0x19; actionIdx++) {
            if ((unsigned int)g_InputBindings[mapIdx][actionIdx] < INPUT_SOURCE_KEY_END) {
                if (g_InputKeyboardStateBuffer != 0) {
                    if ((g_InputKeyboardStateBuffer[g_InputBindings[mapIdx][actionIdx]] & 0x80) != 0) {
                        g_InputHeldMask = g_InputHeldMask | (1 << actionIdx);
                    }
                }
            } else {
                if (g_InputMouseStatePointer != 0) {
                    switch (g_InputBindings[mapIdx][actionIdx]) {
                    case INPUT_SOURCE_MOUSE_BUTTON_0:
                        if ((g_InputMouseStatePointer->button0 & 0x80) != 0) {
                            g_InputHeldMask = g_InputHeldMask | (1 << actionIdx);
                        }
                        break;
                    case INPUT_SOURCE_MOUSE_BUTTON_1:
                        if ((g_InputMouseStatePointer->button1 & 0x80) != 0) {
                            g_InputHeldMask = g_InputHeldMask | (1 << actionIdx);
                        }
                        break;
                    case INPUT_SOURCE_MOUSE_BUTTON_2:
                        if ((g_InputMouseStatePointer->button2 & 0x80) != 0) {
                            g_InputHeldMask = g_InputHeldMask | (1 << actionIdx);
                        }
                        break;
                    }
                }
                if (g_InputJoystickPollTable != 0) {
                    switch (g_InputBindings[mapIdx][actionIdx]) {
                    case INPUT_SOURCE_JOY_UP:
                        if (g_InputJoystickPollTable->up != 0) {
                            g_InputHeldMask = g_InputHeldMask | (1 << actionIdx);
                        }
                        break;
                    case INPUT_SOURCE_JOY_DOWN:
                        if (g_InputJoystickPollTable->down != 0) {
                            g_InputHeldMask = g_InputHeldMask | (1 << actionIdx);
                        }
                        break;
                    case INPUT_SOURCE_JOY_LEFT:
                        if (g_InputJoystickPollTable->left != 0) {
                            g_InputHeldMask = g_InputHeldMask | (1 << actionIdx);
                        }
                        break;
                    case INPUT_SOURCE_JOY_RIGHT:
                        if (g_InputJoystickPollTable->right != 0) {
                            g_InputHeldMask = g_InputHeldMask | (1 << actionIdx);
                        }
                        break;
                    case INPUT_SOURCE_JOY_BUTTON_0:
                        if (g_InputJoystickPollTable->buttons[0] != 0) {
                            g_InputHeldMask = g_InputHeldMask | (1 << actionIdx);
                        }
                        break;
                    case INPUT_SOURCE_JOY_BUTTON_1:
                        if (g_InputJoystickPollTable->buttons[1] != 0) {
                            g_InputHeldMask = g_InputHeldMask | (1 << actionIdx);
                        }
                        break;
                    case INPUT_SOURCE_JOY_BUTTON_2:
                        if (g_InputJoystickPollTable->buttons[2] != 0) {
                            g_InputHeldMask = g_InputHeldMask | (1 << actionIdx);
                        }
                        break;
                    case INPUT_SOURCE_JOY_BUTTON_3:
                        if (g_InputJoystickPollTable->buttons[3] != 0) {
                            g_InputHeldMask = g_InputHeldMask | (1 << actionIdx);
                        }
                        break;
                    case INPUT_SOURCE_JOY_BUTTON_4:
                        if (g_InputJoystickPollTable->buttons[4] != 0) {
                            g_InputHeldMask = g_InputHeldMask | (1 << actionIdx);
                        }
                        break;
                    case INPUT_SOURCE_JOY_BUTTON_5:
                        if (g_InputJoystickPollTable->buttons[5] != 0) {
                            g_InputHeldMask = g_InputHeldMask | (1 << actionIdx);
                        }
                        break;
                    case INPUT_SOURCE_JOY_BUTTON_6:
                        if (g_InputJoystickPollTable->buttons[6] != 0) {
                            g_InputHeldMask = g_InputHeldMask | (1 << actionIdx);
                        }
                        break;
                    case INPUT_SOURCE_JOY_BUTTON_7:
                        if (g_InputJoystickPollTable->buttons[7] != 0) {
                            g_InputHeldMask = g_InputHeldMask | (1 << actionIdx);
                        }
                        break;
                    case INPUT_SOURCE_JOY_BUTTON_8:
                        if (g_InputJoystickPollTable->buttons[8] != 0) {
                            g_InputHeldMask = g_InputHeldMask | (1 << actionIdx);
                        }
                        break;
                    case INPUT_SOURCE_JOY_BUTTON_9:
                        if (g_InputJoystickPollTable->buttons[9] != 0) {
                            g_InputHeldMask = g_InputHeldMask | (1 << actionIdx);
                        }
                        break;
                    }
                }
            }
        }
    }
    g_InputPressedMask = ((g_InputHeldMask & g_InputPrevHeldBits) ^ -1) & g_InputHeldMask;
    if (g_InputRepeatInterval != 0) {
        g_InputTimeGetTimeSample = timeGetTime();
        g_InputRepeatElapsedTicks =
            g_InputRepeatElapsedTicks + (g_InputTimeGetTimeSample - g_InputLastTickSample);
        g_InputLastTickSample = g_InputTimeGetTimeSample;
        Input_TickRepeat();
    }
}

// Finds a one-based bit index by shifting mask until no set bits remain above it, then stores
// binding at that index in the selected 0x19-int row of g_InputBindings. A zero mask selects
// index 1. The original indexes from the repeat interval word before the table, so its one-based
// bit is the table's column bit - 1.
// FUNCTION: FF7 0x0041a7ef
void __cdecl Input_SetBinding(int device, unsigned int mask, int binding)
{
    int bit = 1;

    while ((mask >> bit) != 0) {
        bit = bit + 1;
    }
    g_InputBindings[device][bit - 1] = binding;
}

// Clears the binding table, loads keyboard and joystick defaults, and installs fifteen additional
// fixed keyboard/joystick bindings through Input_SetBinding.
// FUNCTION: FF7 0x0041a828
void __cdecl Input_LoadAllDefaults(void)
{
    memset(g_InputBindings, 0, sizeof(g_InputBindings));
    Input_LoadKeyboardDefaults();
    Input_LoadJoystickDefaults();
    Input_SetBinding(0, 0x10000, DIK_ESCAPE);
    Input_SetBinding(1, 0x10000, INPUT_SOURCE_JOY_BUTTON_1);
    Input_SetBinding(0, 0x200000, DIK_RETURN);
    Input_SetBinding(0, INPUT_CURSOR_UP, DIK_UP);
    Input_SetBinding(0, INPUT_CURSOR_DOWN, DIK_DOWN);
    Input_SetBinding(0, INPUT_CURSOR_LEFT, DIK_LEFT);
    Input_SetBinding(0, INPUT_CURSOR_RIGHT, DIK_RIGHT);
    Input_SetBinding(1, INPUT_CURSOR_UP, INPUT_SOURCE_JOY_UP);
    Input_SetBinding(1, INPUT_CURSOR_DOWN, INPUT_SOURCE_JOY_DOWN);
    Input_SetBinding(1, INPUT_CURSOR_LEFT, INPUT_SOURCE_JOY_LEFT);
    Input_SetBinding(1, INPUT_CURSOR_RIGHT, INPUT_SOURCE_JOY_RIGHT);
    Input_SetBinding(0, 0x400000, INPUT_SOURCE_MOUSE_BUTTON_0);
    Input_SetBinding(1, 0x400000, INPUT_SOURCE_MOUSE_BUTTON_1);
    Input_SetBinding(0, INPUT_JOY_BUTTON_1, INPUT_SOURCE_JOY_BUTTON_0);
    Input_SetBinding(0, INPUT_JOY_BUTTON_3, INPUT_SOURCE_JOY_BUTTON_2);
}

// Installs the keyboard default bindings (device 0): the fifteen button masks to their DirectInput
// scan codes (numpad and adjacent keys).
// FUNCTION: FF7 0x0041a96d
void __cdecl Input_LoadKeyboardDefaults(void)
{
    Input_SetBinding(0, INPUT_CIRCLE, DIK_NUMPADENTER);
    Input_SetBinding(0, INPUT_CROSS, DIK_NUMPAD0);
    Input_SetBinding(0, INPUT_TRIANGLE, DIK_ADD);
    Input_SetBinding(0, INPUT_SELECT, DIK_SUBTRACT);
    Input_SetBinding(0, INPUT_SQUARE, DIK_DECIMAL);
    Input_SetBinding(0, INPUT_UP, DIK_NUMPAD8);
    Input_SetBinding(0, INPUT_RIGHT, DIK_NUMPAD6);
    Input_SetBinding(0, INPUT_DOWN, DIK_NUMPAD2);
    Input_SetBinding(0, INPUT_LEFT, DIK_NUMPAD4);
    Input_SetBinding(0, INPUT_L2, DIK_NUMPAD7);
    Input_SetBinding(0, INPUT_L1, DIK_NUMPAD9);
    Input_SetBinding(0, INPUT_R2, DIK_NUMPAD1);
    Input_SetBinding(0, INPUT_R1, DIK_NUMPAD3);
    Input_SetBinding(0, INPUT_SELECT, DIK_SUBTRACT);
    Input_SetBinding(0, INPUT_START, DIK_NUMPAD5);
}

// Installs the joystick default bindings (device 1): the fourteen button masks to joystick button
// and axis codes (INPUT_SOURCE_JOY_*).
// FUNCTION: FF7 0x0041aa5f
void __cdecl Input_LoadJoystickDefaults(void)
{
    Input_SetBinding(1, INPUT_L2, INPUT_SOURCE_JOY_BUTTON_6);
    Input_SetBinding(1, INPUT_R2, INPUT_SOURCE_JOY_BUTTON_7);
    Input_SetBinding(1, INPUT_L1, INPUT_SOURCE_JOY_BUTTON_4);
    Input_SetBinding(1, INPUT_R1, INPUT_SOURCE_JOY_BUTTON_5);
    Input_SetBinding(1, INPUT_TRIANGLE, INPUT_SOURCE_JOY_BUTTON_3);
    Input_SetBinding(1, INPUT_CIRCLE, INPUT_SOURCE_JOY_BUTTON_2);
    Input_SetBinding(1, INPUT_CROSS, INPUT_SOURCE_JOY_BUTTON_1);
    Input_SetBinding(1, INPUT_SQUARE, INPUT_SOURCE_JOY_BUTTON_0);
    Input_SetBinding(1, INPUT_SELECT, INPUT_SOURCE_JOY_BUTTON_8);
    Input_SetBinding(1, INPUT_START, INPUT_SOURCE_JOY_BUTTON_9);
    Input_SetBinding(1, INPUT_UP, INPUT_SOURCE_JOY_UP);
    Input_SetBinding(1, INPUT_RIGHT, INPUT_SOURCE_JOY_RIGHT);
    Input_SetBinding(1, INPUT_DOWN, INPUT_SOURCE_JOY_DOWN);
    Input_SetBinding(1, INPUT_LEFT, INPUT_SOURCE_JOY_LEFT);
}

// Returns the bits of `mask` (`InputButton` values, INPUT_ANY for all) whose buttons are held
// down this frame: `g_InputHeldMask`, which `Input_Update` rebuilds every frame from the
// keyboard, mouse and joystick bindings. Nonzero means at least one of them is held.
// FUNCTION: FF7 0x0041ab67
int __cdecl Input_GetHeld(int mask)
{
    return g_InputHeldMask & mask;
}

// Returns the bits of `mask` (`InputButton` values, INPUT_ANY for all) whose buttons went down
// this frame: `g_InputPressedMask`, which `Input_Update` sets to the held bits that were not held
// the frame before. A button kept held reports once, not every frame. Nonzero means at least one
// of them was pressed.
// FUNCTION: FF7 0x0041ab74
int __cdecl Input_GetPressed(int mask)
{
    return g_InputPressedMask & mask;
}

// Returns the first pressed pending keyboard event, querying pending count anew each loop.
// Otherwise prioritizes three shared mouse bits, four directional and two designated pressed-
// action masks, then polls joystick and checks eight button fields in order. Returns the
// associated source code 0xDF through 0xF4 or zero when none qualifies; keyboard release/null
// events are skipped.
// FUNCTION: FF7 0x0041ab81
int __cdecl Input_CaptureAnySource(void)
{
    int eventIdx;
    int *event;
    JoystickOutput *joy;
    for (eventIdx = 0; eventIdx < DI_KeyboardGetPendingCount(); eventIdx++) {
        event = (int *)DI_KeyboardGetNextEvent();
        if (event && (event[1] & 0x80))
            return event[0];
    }
    if (g_InputMouseButtonFlags & MOUSE_BUTTON_0_PRESSED)
        return INPUT_SOURCE_MOUSE_BUTTON_0;
    if (g_InputMouseButtonFlags & MOUSE_BUTTON_1_PRESSED)
        return INPUT_SOURCE_MOUSE_BUTTON_1;
    if (g_InputMouseButtonFlags & MOUSE_BUTTON_2_PRESSED)
        return INPUT_SOURCE_MOUSE_BUTTON_2;
    if (Input_GetPressed(INPUT_CURSOR_UP))
        return INPUT_SOURCE_JOY_UP;
    if (Input_GetPressed(INPUT_CURSOR_DOWN))
        return INPUT_SOURCE_JOY_DOWN;
    if (Input_GetPressed(INPUT_CURSOR_LEFT))
        return INPUT_SOURCE_JOY_LEFT;
    if (Input_GetPressed(INPUT_CURSOR_RIGHT))
        return INPUT_SOURCE_JOY_RIGHT;
    if (Input_GetPressed(INPUT_JOY_BUTTON_1))
        return INPUT_SOURCE_JOY_BUTTON_0;
    if (Input_GetPressed(INPUT_JOY_BUTTON_3))
        return INPUT_SOURCE_JOY_BUTTON_2;
    joy = Joystick_Poll();
    if (joy) {
        if (joy->buttons[1])
            return INPUT_SOURCE_JOY_BUTTON_1;
        if (joy->buttons[3])
            return INPUT_SOURCE_JOY_BUTTON_3;
        if (joy->buttons[4])
            return INPUT_SOURCE_JOY_BUTTON_4;
        if (joy->buttons[5])
            return INPUT_SOURCE_JOY_BUTTON_5;
        if (joy->buttons[6])
            return INPUT_SOURCE_JOY_BUTTON_6;
        if (joy->buttons[7])
            return INPUT_SOURCE_JOY_BUTTON_7;
        if (joy->buttons[8])
            return INPUT_SOURCE_JOY_BUTTON_8;
        if (joy->buttons[9])
            return INPUT_SOURCE_JOY_BUTTON_9;
    }
    return 0;
}

// Clears buffered mouse and repeat bits, resetting accumulated XY only when repeat is disabled.
// Consumes pending mouse events, accumulating axis deltas and ORing three button-press bits. For
// each axis exceeding dead zone fifteen sets its direction bit, optionally clamps magnitude to two
// hundred and compares unsigned elapsed time to repeat interval minus interval times integer
// magnitude divided by two hundred. Clears that axis accumulator after triggering, leaving smaller
// motion accumulated. Returns mouse bits; release events and other axes are ignored.
// FUNCTION: FF7 0x0041ad4b
int __cdecl Input_PollMouseBuffered(void)
{
    int i;
    int range;
    int dead;
    int limit;
    int *obj;

    dead = 0xF;
    range = 0xC8;
    g_InputMouseButtonFlags = g_InputMouseButtonFlags ^ g_InputMouseButtonFlags;
    g_InputMouseRepeatMask = g_InputMouseRepeatMask ^ g_InputMouseRepeatMask;
    if (g_InputRepeatInterval == 0) {
        g_InputMouseDeltaAccumulator = 0;
        g_InputMouseDeltaSecondAccumulator = 0;
    }
    for (i = 0; i < DI_MouseGetPendingCount(); i++) {
        obj = (int *)DI_MouseGetNextEvent();
        if (obj != 0) {
            switch (obj[0]) {
            case DIMOFS_X:
                g_InputMouseDeltaAccumulator =
                    g_InputMouseDeltaAccumulator + obj[1];
                break;
            case DIMOFS_Y:
                g_InputMouseDeltaSecondAccumulator =
                    g_InputMouseDeltaSecondAccumulator + obj[1];
                break;
            case DIMOFS_BUTTON0:
                if ((obj[1] & 0x80) != 0) {
                    g_InputMouseButtonFlags = g_InputMouseButtonFlags | MOUSE_BUTTON_0_PRESSED;
                }
                break;
            case DIMOFS_BUTTON1:
                if ((obj[1] & 0x80) != 0) {
                    g_InputMouseButtonFlags = g_InputMouseButtonFlags | MOUSE_BUTTON_1_PRESSED;
                }
                break;
            case DIMOFS_BUTTON2:
                if ((obj[1] & 0x80) != 0) {
                    g_InputMouseButtonFlags = g_InputMouseButtonFlags | MOUSE_BUTTON_2_PRESSED;
                }
                break;
            }
        }
    }
    if (g_InputMouseDeltaAccumulator < -dead) {
        g_InputMouseButtonFlags = g_InputMouseButtonFlags | MOUSE_DIR_LEFT;
        if (g_InputRepeatInterval != 0) {
            if (g_InputMouseDeltaAccumulator < -range) {
                g_InputMouseDeltaAccumulator = -range;
            }
            limit = g_InputRepeatInterval - g_InputRepeatInterval * (-g_InputMouseDeltaAccumulator / range);
            if ((unsigned int)g_InputRepeatElapsedTicks >= (unsigned int)limit) {
                g_InputMouseRepeatMask = g_InputMouseRepeatMask | MOUSE_DIR_LEFT;
            }
        }
        g_InputMouseDeltaAccumulator = 0;
    }
    if (g_InputMouseDeltaAccumulator > dead) {
        g_InputMouseButtonFlags = g_InputMouseButtonFlags | MOUSE_DIR_RIGHT;
        if (g_InputRepeatInterval != 0) {
            if (g_InputMouseDeltaAccumulator > range) {
                g_InputMouseDeltaAccumulator = range;
            }
            limit = g_InputRepeatInterval - g_InputRepeatInterval * (g_InputMouseDeltaAccumulator / range);
            if ((unsigned int)g_InputRepeatElapsedTicks >= (unsigned int)limit) {
                g_InputMouseRepeatMask = g_InputMouseRepeatMask | MOUSE_DIR_RIGHT;
            }
        }
        g_InputMouseDeltaAccumulator = 0;
    }
    if (g_InputMouseDeltaSecondAccumulator < -dead) {
        g_InputMouseButtonFlags = g_InputMouseButtonFlags | MOUSE_DIR_UP;
        if (g_InputRepeatInterval != 0) {
            if (g_InputMouseDeltaSecondAccumulator < -range) {
                g_InputMouseDeltaSecondAccumulator = -range;
            }
            limit = g_InputRepeatInterval - g_InputRepeatInterval * (-g_InputMouseDeltaSecondAccumulator / range);
            if ((unsigned int)g_InputRepeatElapsedTicks >= (unsigned int)limit) {
                g_InputMouseRepeatMask = g_InputMouseRepeatMask | MOUSE_DIR_UP;
            }
        }
        g_InputMouseDeltaSecondAccumulator = 0;
    }
    if (g_InputMouseDeltaSecondAccumulator > dead) {
        g_InputMouseButtonFlags = g_InputMouseButtonFlags | MOUSE_DIR_DOWN;
        if (g_InputRepeatInterval != 0) {
            if (g_InputMouseDeltaSecondAccumulator > range) {
                g_InputMouseDeltaSecondAccumulator = range;
            }
            limit = g_InputRepeatInterval - g_InputRepeatInterval * (g_InputMouseDeltaSecondAccumulator / range);
            if ((unsigned int)g_InputRepeatElapsedTicks >= (unsigned int)limit) {
                g_InputMouseRepeatMask = g_InputMouseRepeatMask | MOUSE_DIR_DOWN;
            }
        }
        g_InputMouseDeltaSecondAccumulator = 0;
    }
    return g_InputMouseButtonFlags;
}

// Returns the mouse direction mask g_InputMouseButtonFlags.
// FUNCTION: FF7 0x0041b08f
int __cdecl Input_GetMouseDirMask(void)
{
    return g_InputMouseButtonFlags;
}

// Auto-repeat read: with no repeat rate configured it is the held mask; otherwise the repeat pulse
// g_InputRepeatActiveFlag while the button is held.
// FUNCTION: FF7 0x0041b099
int __cdecl Input_GetRepeat(int mask)
{
    if (g_InputRepeatInterval == 0) {
        return Input_GetHeld(mask);
    }
    if (Input_GetHeld(mask) != 0) {
        return g_InputRepeatActiveFlag;
    }
    return 0;
}

// Returns the mouse repeat mask g_InputMouseRepeatMask.
// FUNCTION: FF7 0x0041b0ce
int __cdecl Input_GetMouseRepeatMask(void)
{
    return g_InputMouseRepeatMask;
}

// Sets the auto-repeat rates: first delay g_InputRepeatFirstDelay and repeat interval
// g_InputRepeatInterval.
// FUNCTION: FF7 0x0041b0d8
void __cdecl Input_SetRepeatRates(int first, int repeat)
{
    g_InputRepeatFirstDelay = first;
    g_InputRepeatInterval = repeat;
}

// Reads the auto-repeat rates into *first and *repeat.
// FUNCTION: FF7 0x0041b0ee
void __cdecl Input_GetRepeatRates(int *first, int *repeat)
{
    *first = g_InputRepeatFirstDelay;
    *repeat = g_InputRepeatInterval;
}

// Advances auto-repeat once per frame: a fresh press restarts the clock with the first delay and
// pulses; a held button pulses again each time the clock reaches the current delay, which becomes
// the repeat interval after the first pulse.
// FUNCTION: FF7 0x0041b108
void __cdecl Input_TickRepeat(void)
{
    g_InputRepeatActiveFlag = 0;
    if (g_InputPressedMask != 0) {
        g_InputRepeatStateFlag = 0;
        g_InputRepeatNextThreshold = g_InputRepeatFirstDelay;
        g_InputRepeatActiveFlag = 1;
        g_InputRepeatElapsedTicks = 0;
    } else if (g_InputHeldMask != 0 && (unsigned int)g_InputRepeatElapsedTicks >= (unsigned int)g_InputRepeatNextThreshold) {
        g_InputRepeatNextThreshold = g_InputRepeatInterval;
        g_InputRepeatStateFlag = 1;
        g_InputRepeatActiveFlag = 1;
        g_InputRepeatElapsedTicks = 0;
    }
}

// Clears the auto-repeat state: clock, both rates and the repeating flag.
// FUNCTION: FF7 0x0041b18b
void __cdecl Input_ResetRepeatState(void)
{
    g_InputRepeatElapsedTicks = 0;
    g_InputRepeatFirstDelay = 0;
    g_InputRepeatInterval = g_InputRepeatFirstDelay;
    g_InputRepeatStateFlag = 0;
}

// Returns the 0x80 bit of the button0, button1 or button2 byte of g_InputMouseStatePointer for
// INPUT_SOURCE_MOUSE_BUTTON_0, _1 or _2. The result is that masked byte, so it is 0x80 or 0, not 1. Other
// codes return 0, as does a null state pointer.
// FUNCTION: FF7 0x0041b1b8
int __cdecl Input_IsMouseButtonDown(int button)
{
    if (g_InputMouseStatePointer != 0) {
        switch (button) {
        case INPUT_SOURCE_MOUSE_BUTTON_0:
            return g_InputMouseStatePointer->button0 & 0x80;
        case INPUT_SOURCE_MOUSE_BUTTON_1:
            return g_InputMouseStatePointer->button1 & 0x80;
        case INPUT_SOURCE_MOUSE_BUTTON_2:
            return g_InputMouseStatePointer->button2 & 0x80;
        default:
            return 0;
        }
    }
    return 0;
}

// Returns whether keyboard key `key` (0..255) is down in the DirectInput state buffer
// g_InputKeyboardStateBuffer.
// FUNCTION: FF7 0x0041b22b
int __cdecl Input_IsKeyDown(unsigned int key)
{
    if (key > 0xff) {
        return 0;
    }
    if (g_InputKeyboardStateBuffer != 0) {
        return g_InputKeyboardStateBuffer[key] & 0x80;
    }
    return 0;
}

// Returns the address of the cursor position pair g_InputCursorX.
// FUNCTION: FF7 0x0041b25d
int __cdecl Input_GetCursorPos(void)
{
    return (int)&g_InputCursorX;
}

// Sets the cursor position. Nothing happens if either coordinate is negative or g_InputContext is
// null. Otherwise x is stored in g_InputCursorX only when it does not exceed the context's
// displayWidth, and y is stored in g_InputCursorY only when it does not exceed its displayHeight.
// Each stored value is mirrored into the context's cursorX and cursorY.
// FUNCTION: FF7 0x0041b267
void __cdecl Input_SetCursorPos(int x, int y)
{
    if (x < 0 || y < 0) { /* empty first arm: lib/codegen.h */
    } else if (g_InputContext != 0) {
        if (g_InputContext->displayWidth >= x) {
            g_InputCursorX = x;
            g_InputContext->cursorX = g_InputCursorX;
        }
        if (g_InputContext->displayHeight >= y) {
            g_InputCursorY = y;
            g_InputContext->cursorY = g_InputCursorY;
        }
    }
}

// Resets the auto-repeat clock and repeating flag.
// FUNCTION: FF7 0x0041b2d8
void __cdecl Input_ResetRepeatClock(void)
{
    g_InputRepeatElapsedTicks = 0;
    g_InputRepeatStateFlag = 0;
}
} /* extern "C" */
