// src/ff7/src/menu/btlmenu/english/menu_cursor_input.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Initialises and updates the menu cursor, and reads its held, pressed and repeated input
//   from the pad. A helper forwards the pad state to the runtime for these reads.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/menu.h"
#include "ff7/enums/menu.h"
#include "lib/enums/file.h"
#include "lib/enums/sound.h"

#include "lib/api/sound.h"
#include "ff7/api/menu.h"
#include "lib/api/file.h"
#include "lib/api/graphics.h"
extern "C" {

/*
 * Stores its fourteen arguments into the fourteen members of the MenuCursor, in order.
 */
// FUNCTION: FF7 0x006f4d30
void __cdecl MenuCursor_Init(MenuCursor *cursor, int col, int row, int colCount,
                             int rowCount, int unused4, int scrollTop,
                             int rowStep, int totalRows, int unused8,
                             int scrollAnim, int colWrapMode,
                             int rowWrapMode, int scrollState,
                             int pageScrollEnable)
{
    cursor->col = col;
    cursor->row = row;
    cursor->colCount = colCount;
    cursor->rowCount = rowCount;
    cursor->unused4 = unused4;
    cursor->scrollTop = scrollTop;
    cursor->rowStep = rowStep;
    cursor->totalRows = totalRows;
    cursor->unused8 = unused8;
    cursor->scrollAnim = scrollAnim;
    cursor->colWrapMode = colWrapMode;
    cursor->rowWrapMode = rowWrapMode;
    cursor->scrollState = scrollState;
    cursor->pageScrollEnable = pageScrollEnable;
}

/*
 * Updates a menu cursor from the pad and mouse: up, down, left and right in that priority, then
 * L1/R1 paging when pageScrollEnable is set. A clamped row cursor starts a three-step scroll at
 * the list's edges when more rows exist; scrolling up moves scrollTop at once, scrolling down
 * when the animation reaches -4.
 */
// FUNCTION: FF7 0x006f4db2
void __cdecl MenuCursor_Update(MenuCursor *cursor)
{
    if (cursor->scrollState == MENU_SCROLL_NONE) {
        if (MenuCursor_ReadRepeat(INPUT_UP, MOUSE_DIR_UP)) {
            cursor->row = cursor->row - 1;
            switch (cursor->rowWrapMode) {
            case MENU_CURSOR_CLAMP:
                if (cursor->row < 0) {
                    cursor->row = 0;
                    if (cursor->scrollTop > 0) {
                        cursor->scrollTop = cursor->scrollTop - 1;
                        cursor->scrollAnim = -3;
                        cursor->scrollState = MENU_SCROLL_UP;
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    }
                } else {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                }
                break;
            case MENU_CURSOR_WRAP:
                if (cursor->row < 0) {
                    cursor->row = cursor->rowCount - 1;
                }
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                break;
            }
        } else if (MenuCursor_ReadRepeat(INPUT_DOWN, MOUSE_DIR_DOWN)) {
            cursor->row = cursor->row + 1;
            switch (cursor->rowWrapMode) {
            case MENU_CURSOR_CLAMP:
                if (cursor->row >= cursor->rowCount) {
                    cursor->row = cursor->rowCount - 1;
                    if (cursor->scrollTop < cursor->totalRows - cursor->rowCount) {
                        cursor->scrollAnim = -1;
                        cursor->scrollState = MENU_SCROLL_DOWN;
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    }
                } else {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                }
                break;
            case MENU_CURSOR_WRAP:
                if (cursor->row >= cursor->rowCount) {
                    cursor->row = 0;
                }
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                break;
            }
        } else if (MenuCursor_ReadRepeat(INPUT_LEFT, MOUSE_DIR_LEFT)) {
            switch (cursor->colWrapMode) {
            case MENU_CURSOR_CLAMP:
                cursor->col = cursor->col - 1;
                if (cursor->col < 0) {
                    cursor->col = 0;
                } else {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                }
                break;
            case MENU_CURSOR_WRAP:
                cursor->col = cursor->col - 1;
                if (cursor->col < 0) {
                    cursor->col = cursor->colCount - 1;
                }
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                break;
            case MENU_CURSOR_WRAP_INTO_ROWS:
                if (cursor->col == 0 && cursor->row == 0 && cursor->scrollTop == 0) {
                    break;
                }
                cursor->col = cursor->col - 1;
                if (cursor->col < 0) {
                    cursor->col = cursor->colCount - 1;
                    cursor->row = cursor->row - 1;
                    if (cursor->row < 0) {
                        cursor->row = 0;
                        if (cursor->scrollTop > 0) {
                            cursor->scrollTop = cursor->scrollTop - 1;
                            cursor->scrollAnim = -3;
                            cursor->scrollState = MENU_SCROLL_UP;
                        }
                    }
                }
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                break;
            }
        } else if (MenuCursor_ReadRepeat(INPUT_RIGHT, MOUSE_DIR_RIGHT)) {
            switch (cursor->colWrapMode) {
            case MENU_CURSOR_CLAMP:
                cursor->col = cursor->col + 1;
                if (cursor->col >= cursor->colCount) {
                    cursor->col = cursor->colCount - 1;
                } else {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                }
                break;
            case MENU_CURSOR_WRAP:
                cursor->col = cursor->col + 1;
                if (cursor->col >= cursor->colCount) {
                    cursor->col = 0;
                }
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                break;
            case MENU_CURSOR_WRAP_INTO_ROWS:
                if (cursor->col == cursor->colCount - 1 && cursor->row == cursor->rowCount - 1 &&
                    cursor->scrollTop == cursor->totalRows - cursor->rowCount) {
                    break;
                }
                cursor->col = cursor->col + 1;
                if (cursor->col >= cursor->colCount) {
                    cursor->col = 0;
                    if (cursor->row >= cursor->rowCount) {
                        cursor->col = 0;
                    }
                    cursor->row = cursor->row + 1;
                    if (cursor->row >= cursor->rowCount) {
                        cursor->row = cursor->rowCount - 1;
                        if (cursor->scrollTop < cursor->totalRows - cursor->rowCount) {
                            cursor->scrollAnim = -1;
                            cursor->scrollState = MENU_SCROLL_DOWN;
                        }
                    }
                }
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                break;
            }
        } else if (cursor->pageScrollEnable != 0) {
            if (BtlMenu_GetInputRepeat(INPUT_R1)) {
                cursor->scrollTop = cursor->scrollTop + cursor->rowCount;
                if (cursor->scrollTop > cursor->totalRows - cursor->rowCount) {
                    cursor->scrollTop = cursor->totalRows - cursor->rowCount;
                } else {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                }
            } else if (BtlMenu_GetInputRepeat(INPUT_L1)) {
                cursor->scrollTop = cursor->scrollTop - cursor->rowCount;
                if (cursor->scrollTop < 0) {
                    cursor->scrollTop = 0;
                } else {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                }
            }
        }
    } else {
        switch (cursor->scrollState) {
        case MENU_SCROLL_UP:
            cursor->scrollAnim = cursor->scrollAnim + 1;
            if (cursor->scrollAnim == 0) {
                cursor->scrollState = MENU_SCROLL_NONE;
                cursor->scrollAnim = 0;
            }
            break;
        case MENU_SCROLL_DOWN:
            cursor->scrollAnim = cursor->scrollAnim - 1;
            if (cursor->scrollAnim == -4) {
                cursor->scrollTop = cursor->scrollTop + 1;
                cursor->scrollState = MENU_SCROLL_NONE;
                cursor->scrollAnim = 0;
            }
            break;
        }
    }
}

/*
 * Returns true when the pad button is held or the mouse direction mask intersects mouseMask.
 */
// FUNCTION: FF7 0x006f5317
int __cdecl MenuCursor_ReadHeld(unsigned int padMask,
                                unsigned int mouseMask)
{
    int mouse;
    int ok;

    mouse = Input_GetMouseDirMask();
    ok = Input_GetHeld(padMask) != 0 || (mouse & mouseMask) != 0;
    return ok;
}

/*
 * Returns true when the pad button has a pressed event or the current mouse direction mask
 * intersects mouseMask; otherwise false.
 */
// FUNCTION: FF7 0x006f535c
int __cdecl MenuCursor_ReadPressed(unsigned int padMask,
                                   unsigned int mouseMask)
{
    int mouse;
    int ok;

    mouse = Input_GetMouseDirMask();
    ok = Input_GetPressed(padMask) != 0 || (mouse & mouseMask) != 0;
    return ok;
}

/*
 * Polls Input_GetPressed for `value` and discards the result.
 */
// FUNCTION: FF7 0x006f53a1
void __cdecl MenuCursor_ForwardPadPressed(int value)
{
    Input_GetPressed(value);
}

/*
 * Returns true when the pad button repeats or the mouse repeat mask intersects mouseMask.
 */
// FUNCTION: FF7 0x006f53b2
int __cdecl MenuCursor_ReadRepeat(unsigned int padMask,
                                  unsigned int mouseMask)
{
    int mouse;

    mouse = Input_GetMouseRepeatMask();
    return Input_GetRepeat(padMask) != 0 || (mouse & mouseMask) != 0;
}

/*
 * Returns Input_GetRepeat(value).
 */
// FUNCTION: FF7 0x006f53f1
int __cdecl BtlMenu_GetInputRepeat(int value)
{
    return Input_GetRepeat(value);
}

/*
 * Pad or mouse, pressed or repeating: one answer for both devices, with `repeat` choosing which
 * pair of readers to use.
 */
// FUNCTION: FF7 0x006f5402
int __cdecl MenuCursor_ReadPressedOrRepeat(unsigned int padMask, unsigned int mouseMask,
                                           int repeat)
{
    int mouse;
    int ok;

    if (repeat == 0) {
        mouse = Input_GetMouseDirMask();
        ok = Input_GetPressed(padMask) != 0 || (mouse & mouseMask) != 0;
    } else {
        mouse = Input_GetMouseRepeatMask();
        ok = Input_GetRepeat(padMask) != 0 || (mouse & mouseMask) != 0;
    }
    return ok;
}
} /* extern "C" */
