/* include/ff7/types/main.h: the types ff7/main TUs share. */
#ifndef FF7_TYPES_MAIN_H
#define FF7_TYPES_MAIN_H

/* The app's 0x90-byte input-state block: App_AllocInputState allocates it (and sets initHeaderA, initHeaderB, initHeaderC
 * and initHeaderD), g_Main_AppContext.inputState holds it, App_ResetInputStateFlags clears the 22 words
 * from +0x34 and sets resetFlagA..resetFlagE to 1, App_OnKeyMessage keeps the Ctrl and Shift keys and the F1
 * toggle in it, and App_FreeInputStateAndRsdModel frees the RSD model at +0x8C with it. */
typedef struct MainInputState {
    int initHeaderA;
    int initHeaderB;
    unsigned char pad_08[0x4];
    int initHeaderC;
    unsigned char pad_10[0x8];
    int initHeaderD;
    unsigned char pad_1C[0xC];
    int ctrlDown; /* +0x28: VK_CONTROL is down */
    int shiftDown; /* +0x2C: VK_SHIFT is down */
    unsigned char pad_30[0x4];
    int f1DebugToggle; /* +0x34: F1 with Shift down toggles it (F1 alone reports "F1 KEYPRESS") */
    unsigned char pad_38[0x4];
    int resetFlagA;
    int resetFlagB;
    unsigned char pad_44[0x14];
    int resetFlagC;
    unsigned char pad_5C[0x14];
    int resetFlagD;
    int resetFlagE;
    unsigned char pad_78[0x14];
    int rsdModel; /* +0x8C: the RSD model handle App_FreeInputStateAndRsdModel frees */
} MainInputState;

/* g_Main_AppContext (0x009A06D8, 16 bytes): main reads only +0x08, the input-state block. */
typedef struct MainAppContext {
    unsigned char pad_00[0x8];
    struct MainInputState *inputState; /* +0x08 */
    unsigned char pad_0C[0x4];
} MainAppContext;

/* Graphics registry configuration read by ReadGraphicsRegistryConfig. */
typedef struct MainGraphicsConfig {
    unsigned char ddGuid[0x10];
    int isEnabled;
    int driver;
    char driverDllPath[0x104];
    int mode;
    int options;
    unsigned char pad_124[0x188];
} MainGraphicsConfig;

#endif /* FF7_TYPES_MAIN_H */
