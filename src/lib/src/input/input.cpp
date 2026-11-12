// src/lib/src/input/input.cpp
// Original: C:\lib\src\input\input.cpp
// Contents: DirectInput layer: creates, acquires, polls and releases the keyboard and mouse
//   devices, reads their state and buffered events, and sets up the joystick.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#define DIRECTINPUT_VERSION 0x0500
#include <windows.h>
#include <dinput.h>
#include <mmsystem.h>
#include "lib/api/input.h"
#include "lib/types/file.h"
#include "lib/api/graphics.h"
#include "lib/types/input.h"
#include "lib/com_abi.h"

extern "C" {

// GLOBAL: FF7 0x007ad910
extern const DIDATAFORMAT c_dfDIMouse;
// GLOBAL: FF7 0x007ae930
extern const DIDATAFORMAT c_dfDIKeyboard;
// GLOBAL: FF7 0x007b8280
extern const GUID GUID_SysMouse;
// GLOBAL: FF7 0x007b8290
extern const GUID GUID_SysKeyboard;
typedef char InputPropertySize[(sizeof(DIPROPDWORD) == 20) ? 1 : -1];
typedef char InputCapabilitiesSize[(sizeof(DIDEVCAPS) == 44) ? 1 : -1];
// clang-format off
struct InputEventRecord {
    unsigned long offset, data, timestamp, sequence;
};
struct InputMouseState {
    long x, y, z;
    unsigned char buttons[4];
};
// The keyboard and the mouse each have one record: the device, its buffer-size property, its capabilities, the last
// buffered event, the immediate state and two flags. Each DIPROPDWORD sits at 4 mod 8, which VC5 gives a
// 20-byte global only as the interior of a larger object, so the property is a member of the record.
struct InputKeyboardRecord {
    ComDirectInputDevice *device;  /* +0x000 */
    DIPROPDWORD property;       /* +0x004 */
    DIDEVCAPS caps;             /* +0x018 */
    InputEventRecord event;     /* +0x044 */
    unsigned char state[256];   /* +0x054 */
    int acquired;               /* +0x154 */
    int flag158;                /* +0x158 */
};

struct JoystickDevData {
    JOYCAPSA caps;
    JOYINFOEX info;
};

struct InputMouseRecord {
    ComDirectInputDevice *device;  /* +0x00 */
    DIPROPDWORD property;       /* +0x04 */
    DIDEVCAPS caps;             /* +0x18 */
    InputEventRecord event;     /* +0x44 */
    InputMouseState state;      /* +0x54 */
    int acquired;               /* +0x64 */
    int flag68;                 /* +0x68 */
};
// GLOBAL: FF7 0x009ada90
struct InputKeyboardRecord g_InputKeyboard = {0};
unsigned char g_bssPad_009adbec[4] = {0};
// GLOBAL: FF7 0x009adbf0
struct InputMouseRecord g_InputMouse = {0};
unsigned char g_bssPad_009adc5c[4] = {0};

// GLOBAL: FF7 0x009adc60
struct JoystickDevData g_JoystickDevData = {0};

// GLOBAL: FF7 0x009ade28
struct JoystickOutput g_JoystickOutput = {0};

unsigned char g_bssPad_009ade80[64] = {0};

// GLOBAL: FF7 0x009adec0
int g_InputJoystickAvailable = 0;

// GLOBAL: FF7 0x009adec4
int g_JoystickThresholdLeft = 0;

// GLOBAL: FF7 0x009adec8
int g_JoystickThresholdRight = 0;

// GLOBAL: FF7 0x009adecc
int g_JoystickThresholdUp = 0;

// GLOBAL: FF7 0x009aded0
int g_JoystickThresholdDown = 0;

// GLOBAL: FF7 0x009aded4
ComDirectInput *g_pDirectInput = 0;
// clang-format on

// Layouts follow the DX5 DIDEVICEOBJECTDATA and DIMOUSESTATE.
typedef char InputEventRecordSize[(sizeof(InputEventRecord) == 16) ? 1 : -1];
typedef char InputMouseStateSize[(sizeof(InputMouseState) == 16) ? 1 : -1];

/*
 * Initializes DirectInput, keyboard, mouse, and joystick subsystems.
 * Creates the IDirectInput instance and acquires keyboard and mouse devices.
 * Returns 1 on success, or 0 on error.
 */
// FUNCTION: FF7 0x0041eea0
int __cdecl DI_InputInit(void *context)
{
    if (CheckDirectDrawResult(DirectInputCreateA(*(HINSTANCE *)((char *)context + 0x58), 0x300, (LPDIRECTINPUTA *)&g_pDirectInput, 0),
                              // STRING: FF7 0x007c0e84
                              "C:\\lib\\src\\input\\input.cpp", 0xF) != 0) {
        if (DI_KeyboardCreate(context) != 0)
            DI_KeyboardAcquire();
        if (DI_MouseCreate(context) != 0)
            DI_MouseAcquire();
        g_InputJoystickAvailable = Joystick_Init();
        return 1;
    }
    return 0;
}

/*
 * Releases the DirectInput interface and clears its global pointer.
 */
// FUNCTION: FF7 0x0041ef0e
void __cdecl DI_InputShutdown(void)
{
    DI_KeyboardUnacquire();
    DI_MouseUnacquire();
    if (g_pDirectInput != 0) {
        g_pDirectInput->Release();
        g_pDirectInput = 0;
    }
    if (g_InputMouse.device != 0) {
        g_InputMouse.device->Release();
        g_InputMouse.device = 0;
    }
    if (g_InputKeyboard.device != 0) {
        g_InputKeyboard.device->Release();
        g_InputKeyboard.device = 0;
    }
}

/*
 * Creates and initializes the DirectInput mouse device.
 * Sets the mouse data format, cooperative level, and input buffer size (64 elements).
 * Returns 1 on success, or 0 on error.
 */
// FUNCTION: FF7 0x0041ef89
int __cdecl DI_MouseCreate(void *context)
{
    if (CheckDirectDrawResult(g_pDirectInput->CreateDevice(GUID_SysMouse, &g_InputMouse.device, 0),
                              // STRING: FF7 0x007c0ea0
                              "C:\\lib\\src\\input\\input.cpp", 58) == 0)
        return 0;
    if (CheckDirectDrawResult(g_InputMouse.device->SetDataFormat(&c_dfDIMouse),
                              // STRING: FF7 0x007c0ebc
                              "C:\\lib\\src\\input\\input.cpp", 60) == 0)
        return 0;
    if (CheckDirectDrawResult(g_InputMouse.device->SetCooperativeLevel(*(HWND *)((char *)context + 0x5c), 5),
                              // STRING: FF7 0x007c0ed8
                              "C:\\lib\\src\\input\\input.cpp", 62) == 0)
        return 0;
    g_InputMouse.property.diph.dwSize = 20;
    g_InputMouse.property.diph.dwHeaderSize = 16;
    g_InputMouse.property.diph.dwObj = 0;
    g_InputMouse.property.diph.dwHow = 0;
    g_InputMouse.property.dwData = 64;
    if (CheckDirectDrawResult(g_InputMouse.device->SetProperty(DIPROP_BUFFERSIZE, &g_InputMouse.property.diph),
                              // STRING: FF7 0x007c0ef4
                              "C:\\lib\\src\\input\\input.cpp", 71) == 0)
        return 0;
    g_InputMouse.caps.dwSize = 44;
    if (CheckDirectDrawResult(g_InputMouse.device->GetCapabilities(&g_InputMouse.caps),
                              // STRING: FF7 0x007c0f10
                              "C:\\lib\\src\\input\\input.cpp", 75) == 0)
        return 0;
    return 1;
}

/*
 * Acquires the DirectInput mouse device for input polling.
 * Returns 1 on success, or 0 on error.
 */
// FUNCTION: FF7 0x0041f0ce
int __cdecl DI_MouseAcquire(void)
{
    if (g_InputMouse.device != 0 && g_InputMouse.acquired == 0) {
        g_InputMouse.acquired = CheckDirectDrawResult(g_InputMouse.device->Acquire(),
                                             // STRING: FF7 0x007c0f2c
                                             "C:\\lib\\src\\input\\input.cpp", 0x54);
    }
    return g_InputMouse.acquired;
}

/*
 * Releases acquisition of the DirectInput mouse device.
 */
// FUNCTION: FF7 0x0041f110
void __cdecl DI_MouseUnacquire(void)
{
    if (g_InputMouse.device != 0) {
        g_InputMouse.device->Unacquire();
    }
    g_InputMouse.acquired = 0;
}

/*
 * Retrieves the current mouse position and button state from DirectInput.
 * Automatically reacquires the device if input was lost.
 * Returns 1 on success, or 0 on error.
 */
// FUNCTION: FF7 0x0041f139
int __cdecl DI_MouseGetState(void)
{
    int val;
    if (g_InputMouse.device != 0) {
        val = g_InputMouse.device->GetDeviceState(0x10, &g_InputMouse.state);
        if (val == 0)
            return (int)&g_InputMouse.state;
        if (val == DIERR_INPUTLOST) {
            g_InputMouse.acquired = 0;
            if (DI_MouseAcquire() != 0) {
                if (CheckDirectDrawResult(g_InputMouse.device->GetDeviceState(0x10, &g_InputMouse.state),
                                          // STRING: FF7 0x007c0f48
                                          "C:\\lib\\src\\input\\input.cpp", 0x72) != 0)
                    return (int)&g_InputMouse.state;
            }
        }
    }
    return 0;
}

/*
 * Retrieves the next buffered mouse input event from DirectInput.
 * Automatically reacquires the device if input was lost.
 * Returns 1 if an event was retrieved, or 0 if queue is empty/failed.
 */
// FUNCTION: FF7 0x0041f1c3
int __cdecl DI_MouseGetNextEvent(void)
{
    int count;
    int res;
    count = 1;
    if (g_InputMouse.device != 0) {
        res = g_InputMouse.device->GetDeviceData(0x10, &g_InputMouse.event, &count, 0);
        if (res == 0 || res == 1) {
            g_InputMouse.flag68 = 0;
            return count ? (int)&g_InputMouse.event : 0;
        }
        if (res == DIERR_INPUTLOST) {
            g_InputMouse.acquired = 0;
            DI_MouseAcquire();
        }
    }
    return 0;
}

/*
 * Flushes all pending buffered events from the DirectInput mouse device.
 * Returns 1 on success, or 0 on error.
 */
// FUNCTION: FF7 0x0041f23c
int __cdecl DI_MouseFlushBuffer(void)
{
    int count;
    int res;
    count = -1;
    if (g_InputMouse.device != 0) {
        res = g_InputMouse.device->GetDeviceData(0x10, 0, &count, 0);
        g_InputMouse.flag68 = 0;
        if (res == 0 || res == 1)
            return count;
        if (res == DIERR_INPUTLOST) {
            g_InputMouse.acquired = 0;
            DI_MouseAcquire();
        }
    }
    return 0;
}

/*
 * Checks if the DirectInput mouse input buffer has overflowed.
 * Returns 1 if buffer overflowed (DI_BUFFEROVERFLOW), or 0 otherwise.
 */
// FUNCTION: FF7 0x0041f2a9
int __cdecl DI_MouseBufferOverflowed(void)
{
    int count;
    int res;
    count = 0;
    if (g_InputMouse.device != 0) {
        if (g_InputMouse.flag68 != 0)
            return 1;
        res = g_InputMouse.device->GetDeviceData(0x10, 0, &count, 0);
        g_InputMouse.flag68 = (res == 1);
        if (g_InputMouse.flag68 != 0)
            return 1;
        if (res == DIERR_INPUTLOST) {
            g_InputMouse.acquired = 0;
            DI_MouseAcquire();
        }
    }
    return 0;
}

/*
 * Queries the number of buffered input events pending in the mouse device queue.
 * Returns the number of events, or 0 on failure.
 */
// FUNCTION: FF7 0x0041f32a
int __cdecl DI_MouseGetPendingCount(void)
{
    int pendingCount;
    int res;
    pendingCount = -1;
    if (g_InputMouse.device != 0) {
        res = g_InputMouse.device->GetDeviceData(0x10, 0, &pendingCount, 1);
        if (res == 0)
            return pendingCount;
        if (res == 1) {
            g_InputMouse.flag68 = 1;
            return pendingCount;
        }
        if (res == DIERR_INPUTLOST) {
            g_InputMouse.acquired = 0;
            DI_MouseAcquire();
        }
    }
    return 0;
}

/*
 * Creates and initializes the DirectInput keyboard device.
 * Sets the keyboard data format, cooperative level, and input buffer size (64 elements).
 * Returns 1 on success, or 0 on error.
 */
// FUNCTION: FF7 0x0041f39c
int __cdecl DI_KeyboardCreate(void *context)
{
    if (CheckDirectDrawResult(g_pDirectInput->CreateDevice(GUID_SysKeyboard, &g_InputKeyboard.device, 0),
                              // STRING: FF7 0x007c0f64
                              "C:\\lib\\src\\input\\input.cpp", 230) == 0)
        return 0;
    if (CheckDirectDrawResult(g_InputKeyboard.device->SetDataFormat(&c_dfDIKeyboard),
                              // STRING: FF7 0x007c0f80
                              "C:\\lib\\src\\input\\input.cpp", 232) == 0)
        return 0;
    if (CheckDirectDrawResult(g_InputKeyboard.device->SetCooperativeLevel(*(HWND *)((char *)context + 0x5c), 6),
                              // STRING: FF7 0x007c0f9c
                              "C:\\lib\\src\\input\\input.cpp", 234) == 0)
        return 0;
    g_InputKeyboard.property.diph.dwSize = 20;
    g_InputKeyboard.property.diph.dwHeaderSize = 16;
    g_InputKeyboard.property.diph.dwObj = 0;
    g_InputKeyboard.property.diph.dwHow = 0;
    g_InputKeyboard.property.dwData = 32;
    if (CheckDirectDrawResult(g_InputKeyboard.device->SetProperty(DIPROP_BUFFERSIZE, &g_InputKeyboard.property.diph),
                              // STRING: FF7 0x007c0fb8
                              "C:\\lib\\src\\input\\input.cpp", 243) == 0)
        return 0;
    g_InputKeyboard.caps.dwSize = 44;
    if (CheckDirectDrawResult(g_InputKeyboard.device->GetCapabilities(&g_InputKeyboard.caps),
                              // STRING: FF7 0x007c0fd4
                              "C:\\lib\\src\\input\\input.cpp", 247) == 0)
        return 0;
    return 1;
}

/*
 * Acquires the DirectInput keyboard device for input polling.
 * Returns 1 on success, or 0 on error.
 */
// FUNCTION: FF7 0x0041f4f0
int __cdecl DI_KeyboardAcquire(void)
{
    if (g_InputKeyboard.device != 0 && g_InputKeyboard.acquired == 0) {
        g_InputKeyboard.acquired = CheckDirectDrawResult(g_InputKeyboard.device->Acquire(),
                                             // STRING: FF7 0x007c0ff0
                                             "C:\\lib\\src\\input\\input.cpp", 0x100);
    }
    return g_InputKeyboard.acquired;
}

/*
 * Releases acquisition of the DirectInput keyboard device.
 */
// FUNCTION: FF7 0x0041f535
void __cdecl DI_KeyboardUnacquire(void)
{
    if (g_InputKeyboard.device != 0) {
        g_InputKeyboard.device->Unacquire();
    }
    g_InputKeyboard.acquired = 0;
}

/*
 * Reads the immediate 256-byte keyboard key state array from DirectInput.
 * Automatically reacquires the device if input was lost.
 * Returns pointer to the 256-byte keyboard state array on success, or NULL on error.
 */
// FUNCTION: FF7 0x0041f55e
unsigned char *__cdecl DI_KeyboardGetState(void)
{
    int result;
    if (g_InputKeyboard.device != 0) {
        result = g_InputKeyboard.device->GetDeviceState(0x100, g_InputKeyboard.state);
        if (result == 0)
            return g_InputKeyboard.state;
        if (result == DIERR_INPUTLOST) {
            g_InputKeyboard.acquired = 0;
            if (DI_KeyboardAcquire() != 0) {
                if (CheckDirectDrawResult(g_InputKeyboard.device->GetDeviceState(0x100, g_InputKeyboard.state),
                                          // STRING: FF7 0x007c100c
                                          "C:\\lib\\src\\input\\input.cpp", 0x129) != 0)
                    return g_InputKeyboard.state;
            }
        }
    }
    return 0;
}
/*
 * Retrieves the next buffered keyboard input event from DirectInput.
 * Automatically reacquires the device if input was lost.
 * Returns 1 if an event was retrieved, or 0 if queue is empty/failed.
 */
// FUNCTION: FF7 0x0041f5f5
int __cdecl DI_KeyboardGetNextEvent(void)
{
    int count;
    int res;
    count = 1;
    if (g_InputKeyboard.device != 0) {
        res = g_InputKeyboard.device->GetDeviceData(0x10, &g_InputKeyboard.event, &count, 0);
        if (res == 0 || res == 1) {
            if (count == 0)
                return 0;
            g_InputKeyboard.flag158 = 0;
            return count ? (int)&g_InputKeyboard.event : 0;
        }
        if (res == DIERR_INPUTLOST) {
            g_InputKeyboard.acquired = 0;
            DI_KeyboardAcquire();
        }
    }
    return 0;
}

/*
 * Flushes all pending buffered events from the DirectInput keyboard device.
 * Returns 1 on success, or 0 on error.
 */
// FUNCTION: FF7 0x0041f678
int __cdecl DI_KeyboardFlushBuffer(void)
{
    int count;
    int res;
    count = -1;
    if (g_InputKeyboard.device != 0) {
        res = g_InputKeyboard.device->GetDeviceData(0x10, 0, &count, 0);
        g_InputKeyboard.flag158 = 0;
        if (res == 0 || res == 1)
            return count;
        if (res == DIERR_INPUTLOST) {
            g_InputKeyboard.acquired = 0;
            DI_KeyboardAcquire();
        }
    }
    return 0;
}

/*
 * Checks if the DirectInput keyboard input buffer has overflowed.
 * Returns 1 if buffer overflowed (DI_BUFFEROVERFLOW), or 0 otherwise.
 */
// FUNCTION: FF7 0x0041f6e5
int __cdecl DI_KeyboardBufferOverflowed(void)
{
    int count;
    int res;
    count = 0;
    if (g_InputKeyboard.device != 0) {
        if (g_InputKeyboard.flag158 != 0)
            return 1;
        res = g_InputKeyboard.device->GetDeviceData(0x10, 0, &count, 0);
        g_InputKeyboard.flag158 = (res == 1);
        if (g_InputKeyboard.flag158 != 0)
            return 1;
        if (res == DIERR_INPUTLOST) {
            g_InputKeyboard.acquired = 0;
            DI_KeyboardAcquire();
        }
    }
    return 0;
}

/*
 * Queries the number of buffered input events pending in the keyboard device queue.
 * Returns the number of events, or 0 on failure.
 */
// FUNCTION: FF7 0x0041f766
int __cdecl DI_KeyboardGetPendingCount(void)
{
    int pendingCount;
    int res;
    pendingCount = -1;
    if (g_InputKeyboard.device != 0) {
        res = g_InputKeyboard.device->GetDeviceData(0x10, 0, &pendingCount, 1);
        if (res == 0)
            return pendingCount;
        if (res == 1) {
            g_InputKeyboard.flag158 = 1;
            return pendingCount;
        }
        if (res == DIERR_INPUTLOST) {
            g_InputKeyboard.acquired = 0;
            DI_KeyboardAcquire();
        }
    }
    return 0;
}

/*
 * Polls Windows Multimedia joystick device 0 position and button states via joyGetPosEx.
 * Computes digital directional flags (up, down, left, right) from axis thresholds.
 * Returns pointer to g_JoystickOutput on success, or NULL if unavailable/failed.
 */
// FUNCTION: FF7 0x0041f7d8
struct JoystickOutput *__cdecl Joystick_Poll(void)
{
    int result;
    JOYINFOEX *state;
    if (g_InputJoystickAvailable != 0) {
        state = &g_JoystickDevData.info;
        state->dwSize = sizeof(JOYINFOEX);
        state->dwFlags = JOY_RETURNX | JOY_RETURNY | JOY_RETURNBUTTONS;
        result = joyGetPosEx(0, state);
        if (result == 0) {
            g_JoystickOutput.buttons[0] = state->dwButtons & 1;
            g_JoystickOutput.buttons[1] = state->dwButtons & 2;
            g_JoystickOutput.buttons[2] = state->dwButtons & 4;
            g_JoystickOutput.buttons[3] = state->dwButtons & 8;
            g_JoystickOutput.buttons[4] = state->dwButtons & 0x10;
            g_JoystickOutput.buttons[5] = state->dwButtons & 0x20;
            g_JoystickOutput.buttons[6] = state->dwButtons & 0x40;
            g_JoystickOutput.buttons[7] = state->dwButtons & 0x80;
            g_JoystickOutput.buttons[8] = state->dwButtons & 0x100;
            g_JoystickOutput.buttons[9] = state->dwButtons & 0x200;
            g_JoystickOutput.buttons[10] = state->dwButtons & 0x400;
            g_JoystickOutput.buttons[11] = state->dwButtons & 0x800;
            g_JoystickOutput.buttons[12] = state->dwButtons & 0x1000;
            g_JoystickOutput.buttons[13] = state->dwButtons & 0x2000;
            g_JoystickOutput.buttons[14] = state->dwButtons & 0x4000;
            g_JoystickOutput.buttons[15] = state->dwButtons & 0x8000;
            g_JoystickOutput.x = state->dwXpos;
            g_JoystickOutput.y = state->dwYpos;
            g_JoystickOutput.up = g_JoystickOutput.y < g_JoystickThresholdUp;
            g_JoystickOutput.down = g_JoystickOutput.y > g_JoystickThresholdDown;
            g_JoystickOutput.left = g_JoystickOutput.x < g_JoystickThresholdLeft;
            g_JoystickOutput.right = g_JoystickOutput.x > g_JoystickThresholdDown;
            return &g_JoystickOutput;
        }
    }
    return 0;
}

/*
 * Queries Windows Multimedia joystick 0 capabilities via joyGetDevCapsA.
 * Calculates axis deadzone thresholds (35% and 65% of axis max values).
 * Returns 1 if joystick device 0 is available, or 0 otherwise.
 */
// FUNCTION: FF7 0x0041f99e
int __cdecl Joystick_Init(void)
{
    int result;
    result = joyGetDevCapsA(0, &g_JoystickDevData.caps, sizeof(g_JoystickDevData.caps));
    if (result == 0) {
        g_JoystickThresholdLeft = (int)(g_JoystickDevData.caps.wXmax * 0.35);
        g_JoystickThresholdRight = (int)(g_JoystickDevData.caps.wXmax * 0.65);
        g_JoystickThresholdUp = (int)(g_JoystickDevData.caps.wYmax * 0.35);
        g_JoystickThresholdDown = (int)(g_JoystickDevData.caps.wYmax * 0.65);
        return 1;
    }
    return 0;
}
/*
 * Returns the number of buttons reported by joystick device 0 capabilities.
 */
// FUNCTION: FF7 0x0041fa5a
int __cdecl Joystick_GetButtonCount(void)
{
    return g_JoystickDevData.caps.wNumButtons;
}
} /* extern "C" */
