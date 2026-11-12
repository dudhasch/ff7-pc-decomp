/* include/lib/enums/file.h: named codes of lib/file and the TUs that use them.
 * Enums only, so any TU can include it: no types, no declarations. */
#ifndef LIB_ENUMS_FILE_H
#define LIB_ENUMS_FILE_H

/* The source codes of the input binding table (g_InputBindings) above the DirectInput keyboard scan
 * codes: Input_Update reads a code below INPUT_SOURCE_KEY_END as a key of the keyboard state, the
 * three mouse buttons from the mouse state and the rest from Joystick_Poll's JoystickOutput (the
 * directions, then buttons[0..9] from 0xEB); Input_CaptureAnySource returns them. */
enum InputSourceCode {
    INPUT_SOURCE_KEY_END = 0xDE,
    INPUT_SOURCE_MOUSE_BUTTON_0 = 0xDF,
    INPUT_SOURCE_MOUSE_BUTTON_1 = 0xE0,
    INPUT_SOURCE_MOUSE_BUTTON_2 = 0xE1,
    INPUT_SOURCE_JOY_UP = 0xE3,
    INPUT_SOURCE_JOY_DOWN = 0xE4,
    INPUT_SOURCE_JOY_LEFT = 0xE5,
    INPUT_SOURCE_JOY_RIGHT = 0xE6,
    INPUT_SOURCE_JOY_BUTTON_0 = 0xEB,
    INPUT_SOURCE_JOY_BUTTON_1 = 0xEC,
    INPUT_SOURCE_JOY_BUTTON_2 = 0xED,
    INPUT_SOURCE_JOY_BUTTON_3 = 0xEE,
    INPUT_SOURCE_JOY_BUTTON_4 = 0xEF,
    INPUT_SOURCE_JOY_BUTTON_5 = 0xF0,
    INPUT_SOURCE_JOY_BUTTON_6 = 0xF1,
    INPUT_SOURCE_JOY_BUTTON_7 = 0xF2,
    INPUT_SOURCE_JOY_BUTTON_8 = 0xF3,
    INPUT_SOURCE_JOY_BUTTON_9 = 0xF4
};

/* Button masks of the pressed/held input words (PSX pad bits plus fixed PC bits); Input_GetPressed */
enum InputButton {
    INPUT_ANY = -1,
    INPUT_L2 = 0x01,
    INPUT_R2 = 0x02,
    INPUT_L1 = 0x04,
    INPUT_R1 = 0x08,
    INPUT_TRIANGLE = 0x10,
    INPUT_CIRCLE = 0x20,
    INPUT_CROSS = 0x40,
    INPUT_SQUARE = 0x80,
    INPUT_SELECT = 0x100,
    INPUT_START = 0x800,
    INPUT_UP = 0x1000,
    INPUT_RIGHT = 0x2000,
    INPUT_DOWN = 0x4000,
    INPUT_LEFT = 0x8000,
    INPUT_CURSOR_UP = 0x20000,
    INPUT_CURSOR_DOWN = 0x40000,
    INPUT_CURSOR_LEFT = 0x80000,
    INPUT_CURSOR_RIGHT = 0x100000,
    INPUT_JOY_BUTTON_1 = 0x800000, /* fixed binding, INPUT_SOURCE_JOY_BUTTON_0 (0xEB) */
    INPUT_JOY_BUTTON_3 = 0x1000000, /* fixed binding, INPUT_SOURCE_JOY_BUTTON_2 (0xED) */
    INPUT_DPAD_MASK = 0xF000,
    INPUT_ANY_BUTTON = 0xFFFF
};

/* Mouse direction bits of Input_GetMouseDirMask and Input_GetMouseRepeatMask, which the menu
 * cursor reads beside the pad's direction buttons. */
enum MouseDir {
    MOUSE_DIR_UP = 1,
    MOUSE_DIR_DOWN = 2,
    MOUSE_DIR_RIGHT = 4,
    MOUSE_DIR_LEFT = 8
};

/* The other bits of the same mask (Input_PollMouseBuffered): a buffered press of a mouse button
 * during this poll. */
enum MouseButtonPress {
    MOUSE_BUTTON_0_PRESSED = 0x10,
    MOUSE_BUTTON_1_PRESSED = 0x20,
    MOUSE_BUTTON_2_PRESSED = 0x40
};

#ifndef __DINPUT_INCLUDED__
/* The DX5 SDK's DIMOFS_* offsets (<dinput.h>): the DIDEVICEOBJECTDATA.dwOfs of a buffered mouse
 * event, which Input_PollMouseBuffered dispatches on; a TU with <dinput.h> in scope has the SDK's
 * macros instead. */
enum DirectInputMouseOffset {
    DIMOFS_X = 0x0,
    DIMOFS_Y = 0x4,
    DIMOFS_BUTTON0 = 0xC,
    DIMOFS_BUTTON1 = 0xD,
    DIMOFS_BUTTON2 = 0xE
};

/* The DX5 SDK's DIK_* keyboard scan codes (<dinput.h>) that the default input bindings
 * (Input_LoadKeyboardDefaults, Input_LoadAllDefaults) bind, and those the name entry screen's
 * typed keys test (Menu_DebugHotkeyPoll). */
enum DirectInputKey {
    DIK_ESCAPE = 0x01,
    DIK_EQUALS = 0x0D,
    DIK_BACK = 0x0E,
    DIK_RETURN = 0x1C,
    DIK_SEMICOLON = 0x27,
    DIK_LSHIFT = 0x2A,
    DIK_RSHIFT = 0x36,
    DIK_SPACE = 0x39,
    DIK_NUMPAD7 = 0x47,
    DIK_NUMPAD8 = 0x48,
    DIK_NUMPAD9 = 0x49,
    DIK_SUBTRACT = 0x4A,
    DIK_NUMPAD4 = 0x4B,
    DIK_NUMPAD5 = 0x4C,
    DIK_NUMPAD6 = 0x4D,
    DIK_ADD = 0x4E,
    DIK_NUMPAD1 = 0x4F,
    DIK_NUMPAD2 = 0x50,
    DIK_NUMPAD3 = 0x51,
    DIK_NUMPAD0 = 0x52,
    DIK_DECIMAL = 0x53,
    DIK_NUMPADENTER = 0x9C,
    DIK_UP = 0xC8,
    DIK_LEFT = 0xCB,
    DIK_RIGHT = 0xCD,
    DIK_DOWN = 0xD0
};
#endif /* __DINPUT_INCLUDED__ */

/* Text groups of Kernel_LookupUiText, mapped to kernel2.bin banks by g_KernelUiTextGroupBankMap */
enum KernelTextGroup {
    KERNEL_TEXT_MAGIC = 0x00,
    KERNEL_TEXT_SUMMON = 0x01,
    KERNEL_TEXT_ENEMY_SKILL = 0x02,
    KERNEL_TEXT_LIMIT = 0x03,
    KERNEL_TEXT_ITEM = 0x04,               /* inventory id; 128+ maps to groups 10..13 */
    KERNEL_TEXT_COMMAND = 0x05,
    KERNEL_TEXT_SUMMON_ATTACK = 0x06,
    KERNEL_TEXT_ENEMY_NAME = 0x07,
    KERNEL_TEXT_BATTLE_MESSAGE = 0x08,
    KERNEL_TEXT_ENEMY_ATTACK = 0x09,
    KERNEL_TEXT_MATERIA = 0x0D,
    KERNEL_TEXT_KEY_ITEM = 0x0E
};

/* OpenFileEx's open modes (FileOpenRequest.mode), the first argument of OpenFileSimple: from disk
 * they open with _O_BINARY|_O_RDONLY, _O_TEXT|_O_RDONLY, _O_BINARY|_O_RDWR|_O_CREAT and the same
 * with _O_TRUNC; from an archive only FILE_OPEN_READ finds the entry. */
enum FileOpenMode {
    FILE_OPEN_READ = 0,
    FILE_OPEN_READ_TEXT = 1,
    FILE_OPEN_WRITE = 2,
    FILE_OPEN_CREATE = 3
};

/* The entries CountDirectoryEntries and BuildDirectoryListing take from a search (their mode):
 * files only, subdirectories only, or both; "." and ".." are always skipped. */
enum FileDirectoryListMode {
    FILE_DIR_LIST_FILES = 0,
    FILE_DIR_LIST_DIRECTORIES = 1,
    FILE_DIR_LIST_ALL = 2
};

/* What DebugDumpDirectoryListing lists (its mode), by the pattern and list mode it searches with. */
enum FileDirectoryDumpMode {
    FILE_DIR_DUMP_NONE = 0,
    FILE_DIR_DUMP_C_FILES = 1,           /* "*.C" */
    FILE_DIR_DUMP_X_FILES = 2,           /* "*.X" */
    FILE_DIR_DUMP_FILES = 3,             /* "*.*" */
    FILE_DIR_DUMP_DIRECTORIES = 4,       /* "*.*" */
    FILE_DIR_DUMP_EXTENSIONLESS_DIRECTORIES = 5, /* "*." */
    FILE_DIR_DUMP_ALL = 6                /* "*.*" */
};

/* Archive ids BuildArchivePath maps to LGP files. */
enum LgpArchive {
    LGP_ARCHIVE_FIELD_CHAR = 0,
    LGP_ARCHIVE_FIELD_LEVEL = 1,
    LGP_ARCHIVE_BATTLE = 2,
    LGP_ARCHIVE_MAGIC = 3,
    LGP_ARCHIVE_MENU = 4,
    LGP_ARCHIVE_WORLD = 5,
    LGP_ARCHIVE_CONDOR = 6,
    LGP_ARCHIVE_CHOCOBO = 7,
    LGP_ARCHIVE_HIGHWAY = 8,
    LGP_ARCHIVE_COASTER = 9,
    LGP_ARCHIVE_SNOWBOARD = 10,
    LGP_ARCHIVE_MIDI = 11,
    LGP_ARCHIVE_MOVIE_CAMERA = 14,
    LGP_ARCHIVE_CREDITS = 15,
    LGP_ARCHIVE_DISC = 16,
    LGP_ARCHIVE_SUBMARINE = 17
};



enum InputBindingSlot {
    INPUT_BINDING_LEFT = 0,
    INPUT_BINDING_RIGHT = 1,
    INPUT_BINDING_UP = 2,
    INPUT_BINDING_DOWN = 3,
    INPUT_BINDING_TRIANGLE = 4,
    INPUT_BINDING_CIRCLE = 5,
    INPUT_BINDING_CROSS = 6,
    INPUT_BINDING_SQUARE = 7,
    INPUT_BINDING_SELECT = 8,
    INPUT_BINDING_START = 9,
    INPUT_BINDING_L1 = 10,
    INPUT_BINDING_R1 = 11,
    INPUT_BINDING_R2 = 12,
    INPUT_BINDING_L2 = 13
};
#endif
