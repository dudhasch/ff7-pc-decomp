/* include/lib/enums/win32.h: the Win32 codes, with their <windows.h> names, for the TUs that declare
 * the Win32 functions they call themselves (with `int` handles) and so do not include <windows.h>
 * (directx.cpp, field_input_buttons.cpp, menu_quit_confirm_dialog.cpp). Enums only. A TU that
 * includes <windows.h> first gets its macros instead: each group is skipped when the header that
 * defines it (winuser.h, wingdi.h) was read. */
#ifndef LIB_ENUMS_WIN32_H
#define LIB_ENUMS_WIN32_H

#ifndef _WINUSER_
/* Window messages (GameWindowProc's cases, SendMessageA). */
enum Win32WindowMessage {
    WM_CREATE = 0x0001,
    WM_DESTROY = 0x0002,
    WM_MOVE = 0x0003,
    WM_SIZE = 0x0005,
    WM_ACTIVATE = 0x0006,
    WM_PAINT = 0x000F,
    WM_CLOSE = 0x0010,
    WM_QUIT = 0x0012,
    WM_ACTIVATEAPP = 0x001C,
    WM_KEYDOWN = 0x0100,
    WM_KEYUP = 0x0101,
    WM_CHAR = 0x0102,
    WM_TIMER = 0x0113,
    WM_MOUSEMOVE = 0x0200,
    WM_LBUTTONDOWN = 0x0201,
    WM_LBUTTONUP = 0x0202,
    WM_LBUTTONDBLCLK = 0x0203,
    WM_RBUTTONDOWN = 0x0204,
    WM_RBUTTONUP = 0x0205,
    WM_RBUTTONDBLCLK = 0x0206,
    WM_MBUTTONDOWN = 0x0207,
    WM_MBUTTONUP = 0x0208,
    WM_MBUTTONDBLCLK = 0x0209,
    WM_DEVICECHANGE = 0x0219
};

/* WM_SIZE's wParam. */
enum Win32SizeType {
    SIZE_RESTORED = 0,
    SIZE_MINIMIZED = 1
};

/* Virtual-key codes (GetAsyncKeyState, GetKeyState). */
enum Win32VirtualKey {
    VK_CONTROL = 0x11,
    VK_F2 = 0x71
};

/* Window class styles, window styles, CreateWindowExA's default position and ShowWindow's command. */
enum Win32WindowStyle {
    CS_VREDRAW = 0x0001,
    CS_HREDRAW = 0x0002,
    CS_DBLCLKS = 0x0008,
    WS_POPUP = (int)0x80000000,
    WS_CAPTION = 0x00C00000,
    WS_SYSMENU = 0x00080000,
    WS_MINIMIZEBOX = 0x00020000,
    WS_CLIPSIBLINGS = 0x04000000,
    WS_CLIPCHILDREN = 0x02000000,
    WS_EX_APPWINDOW = 0x00040000,
    CW_USEDEFAULT = (int)0x80000000,
    SW_SHOWNORMAL = 1
};

/* The arrow cursor's resource id (LoadCursorA's MAKEINTRESOURCE(32512)). */
enum Win32CursorId {
    IDC_ARROW = 32512
};
#endif

#ifndef _INC_MMSYSTEM
/* MIDI output callback messages (the stream callback's uMsg, mmsystem.h's MOM_*). */
enum Win32MidiOutMessage {
    MOM_OPEN = 0x3C7,
    MOM_CLOSE = 0x3C8,
    MOM_DONE = 0x3C9
};
#endif

#ifndef _WINGDI_
/* GetStockObject's brush and GetDeviceCaps' indices. */
enum Win32GdiIndex {
    BLACK_BRUSH = 4,
    HORZRES = 8,
    VERTRES = 10,
    BITSPIXEL = 12
};
#endif

#endif
