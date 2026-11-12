/* include/lib/types/input.h: the types lib/input and input-polling TUs share.
 * One definition per type. */
#ifndef LIB_TYPES_INPUT_H
#define LIB_TYPES_INPUT_H

/* The joystick state Joystick_Poll fills (g_JoystickOutput): the axes, the four directions the
 * thresholds give, and the button bits; Input_Update tests the directions and buttons[0..9] for
 * the binding codes INPUT_SOURCE_JOY_UP..INPUT_SOURCE_JOY_BUTTON_9. */
typedef struct JoystickOutput {
    int x, y;
    int up, down, left, right;
    int buttons[16];
} JoystickOutput;

/* DirectInput mouse state read by Input_Update: two deltas and three button bytes (0x80 is down). */
typedef struct InputMouseState2 {
    int deltaX;
    int deltaY;
    unsigned char pad_08[0x4];
    unsigned char button0;
    unsigned char button1;
    unsigned char button2;
} InputMouseState2;

#endif /* LIB_TYPES_INPUT_H */
