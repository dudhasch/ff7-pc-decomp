// src/ff7/field/src/field_message_window.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Manages the field's message windows: resets them when a field starts, opens, grows,
//   moves, scrolls and closes each window for its owning entity, and formats numbers into window
//   text.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/field.h"
#include "ff7/enums/field.h"
#include "ff7/enums/main.h"
#include "ff7/field.h"
#include "ff7/api/field.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "ff7/window.h"
#include "ff7/savemap.h"
#include "ff7/enums/menu.h"
#include "lib/enums/file.h"
#include "lib/enums/sound.h"
extern "C" {

/* Per window, how many bytes of a character name Field_WindowStepTextReveal has copied: shorts at +0x30 of
 * g_FieldEntityLine, which the data layout leaves unnamed. */
#define FIELD_WINDOW_NAME_POS(window) (((short *)((char *)&g_FieldEntityLine + 0x30))[window])
// clang-format on

/*
 * Resets the message windows as a field starts: clears g_FieldOpenWindowCount, closes all four windows
 * through Field_WindowClose, then points g_FieldDialogTextPtr at g_FieldScripts plus the offset word at
 * its +4, or at nothing when that word is zero.
 */
// FUNCTION: FF7 0x00630940
void __cdecl Field_ResetMessageWindowsForField(void)
{
    int i;

    g_FieldOpenWindowCount = 0;
    for (i = 0; i < 4; i++) {
        Field_WindowClose((short)i);
    }
    if (g_FieldScripts->dialogTextOffset != 0) {
        g_FieldDialogTextPtr = (void *)((int)g_FieldScripts + g_FieldScripts->dialogTextOffset);
    } else {
        g_FieldDialogTextPtr = 0;
    }
}

/*
 * The tail of Field_ResetMessageWindowsForField on its own: sets g_FieldDialogTextPtr to g_FieldScripts plus the offset word at
 * its +4, or to zero when that word is zero.
 */
// FUNCTION: FF7 0x006309aa
void __cdecl Field_SetMessageTableFromScriptHeader(void)
{
    if (g_FieldScripts->dialogTextOffset != 0) {
        g_FieldDialogTextPtr = (void *)((int)g_FieldScripts + g_FieldScripts->dialogTextOffset);
    } else {
        g_FieldDialogTextPtr = 0;
    }
}

/*
 * Resets selected window fields to fixed closed/default geometry and states, with Y default eight
 * for window one else 149. Clears owner and eight replacement bank/address entries plus per-window
 * global. Does not free resources or validate signed window index.
 */
// FUNCTION: FF7 0x006309e4
void __cdecl Field_WindowClose(short windowIndex)
{
    int i;

    /* Numeric message window slot index. */
    switch (windowIndex) {
    case FIELD_WINDOW_SLOT_TOP:
        g_FieldWindows[windowIndex].y = 8;
        break;
    default:
        g_FieldWindows[windowIndex].y = 149;
    }
    g_FieldWindows[windowIndex].state = MSG_WINDOW_CLOSED;
    g_FieldWindows[windowIndex].style = 0;
    g_FieldWindows[windowIndex].x = 8;
    g_FieldWindows[windowIndex].width = 304;
    g_FieldWindows[windowIndex].height = 74;
    g_FieldWindows[windowIndex].currentWidth = 1;
    g_FieldWindows[windowIndex].currentHeight = 1;
    g_FieldWindows[windowIndex].numDisplayType = MESSAGE_WINDOW_ADDON_NONE;
    g_FieldWindows[windowIndex].numDisplayFlags = 0;
    g_FieldWindows[windowIndex].numDisplayLength = 6;
    g_FieldWindows[windowIndex].numDisplayX = 0;
    g_FieldWindows[windowIndex].numDisplayY = 0;
    g_FieldWindows[windowIndex].preventClose = 0;
    g_FieldWindowOwner[windowIndex] = 0xFF;
    for (i = 0; i < 8; i++) {
        g_WindowReplaceBank[windowIndex][i] = 0;
        g_WindowReplaceBankAddr[windowIndex][i] = 0;
    }
    g_FieldWindowDelay[windowIndex] = 0;
}

/*
 * Reports whether window `window` counts as closed by its state: an opening window is not
 * (returns 0); a window revealing, scrolling or waiting for input, or done, is moved to
 * MSG_WINDOW_CLOSING on the way out; every other state returns 1 untouched.
 */
// FUNCTION: FF7 0x00630b3d
int __cdecl Field_WindowIsClosed(short window)
{
    switch (g_FieldWindows[window].state) {
    case MSG_WINDOW_OPENING:
        return 0;
    case MSG_WINDOW_TEXT_REVEAL:
    case MSG_WINDOW_WAIT_INPUT_SCROLL_ROW:
    case MSG_WINDOW_TEXT_DONE:
    case MSG_WINDOW_ROW_SCROLL:
    case MSG_WINDOW_WAIT_INPUT_SCROLL_PAGE:
    case MSG_WINDOW_WAIT_INPUT_RESUME:
        g_FieldWindows[window].state = MSG_WINDOW_CLOSING;
        return 1;
    }
    return 1;
}

/*
 * Sets a window's style byte and its preventClose word. The style argument arrives as a short and
 * only its low byte is stored.
 */
// FUNCTION: FF7 0x00630bc3
void __cdecl Field_WindowSetMode(short window, short style, short preventClose)
{
    g_FieldWindows[window].style = (unsigned char)style;
    g_FieldWindows[window].preventClose = preventClose;
}

/*
 * Resets the four message windows: state and stringLength to 0, g_FieldWindowOwner to 0xFF (no
 * entity), g_FieldWindowDelay to 0, then clears g_FieldOpenWindowCount.
 */
// FUNCTION: FF7 0x00630bea
void __cdecl Field_ResetMessageWindowState(void)
{
    int i;

    for (i = 0; i < 4; i++) {
        g_FieldWindows[i].state = MSG_WINDOW_CLOSED;
        g_FieldWindows[i].stringLength = 0;
        g_FieldWindowOwner[i] = 0xFF;
        g_FieldWindowDelay[i] = 0;
    }
    g_FieldOpenWindowCount = 0;
}

/*
 * Clamps x and y to at least eight then adjusts them against x+width<=312 and y+height<=224,
 * storing x, y, width, height as shorts in the indexed window. Oversized dimensions can drive
 * adjusted origins below eight; no second lower clamp or window-index check is present.
 */
// FUNCTION: FF7 0x00630c48
void __cdecl Field_WindowSetRectClamped(short windowIndex, short x, short y, short width, short height)
{
    if (x < 8) {
        x = 8;
    }
    if (x + width > 312) {
        x = 312 - width;
    }
    if (y < 8) {
        y = 8;
    }
    if (y + height > 224) {
        y = 224 - height;
    }
    g_FieldWindows[windowIndex].x = x;
    g_FieldWindows[windowIndex].y = y;
    g_FieldWindows[windowIndex].width = width;
    g_FieldWindows[windowIndex].height = height;
}

/*
 * Moves window `window` by (dx, dy), adding to its x and y in place as 16-bit adds.
 */
// FUNCTION: FF7 0x00630cf4
void __cdecl Field_WindowMove(short window, short dx, short dy)
{
    g_FieldWindows[window].x += dx;
    g_FieldWindows[window].y += dy;
}

/*
 * Sets a window's height.
 */
// FUNCTION: FF7 0x00630d39
void __cdecl Field_WindowSetHeight(short window, short height)
{
    g_FieldWindows[window].height = height;
}

/*
 * Dispatches a window message state through initialization, expansion, text, scrolling, and
 * closing helpers. A closed window returns one only when opening reports failure; a closing window
 * returns one when closing reports completion. Other handled states wait for Circle, a delay
 * counter, or a line-position condition before changing state and associated scroll fields.
 * MSG_WINDOW_PAGE_CLEAR invokes its helper unconditionally, while a finished window ignores Circle
 * when its MSG_WINDOW_MODE_NO_PLAYER_CLOSE bit is set. Unsupported states return zero without changes.
 */
// FUNCTION: FF7 0x00630d50
int __cdecl Field_WindowShowMessage(unsigned char windowIndex, unsigned char textIndex)
{
    switch (g_FieldWindows[windowIndex].state) {
    case MSG_WINDOW_CLOSED:
        if (Field_WindowBeginOpen(windowIndex, textIndex) != 0) {
            return 1;
        }
        break;
    case MSG_WINDOW_OPENING:
        Field_WindowStepOpenGrow(windowIndex);
        break;
    case MSG_WINDOW_TEXT_REVEAL:
        Field_WindowStepTextReveal(windowIndex);
        break;
    case MSG_WINDOW_ROW_SCROLL:
        Field_WindowStepRowScroll(windowIndex);
        break;
    case MSG_WINDOW_PAGE_SCROLL_OUT:
        Field_WindowStepPageScrollOut(windowIndex);
        break;
    case MSG_WINDOW_WAIT_INPUT_RESUME:
        if ((*(int *)&g_pFieldState->newActiveKeys2 & INPUT_CIRCLE) != 0) {
            g_FieldWindows[windowIndex].state = MSG_WINDOW_TEXT_REVEAL;
        }
        break;
    case MSG_WINDOW_WAIT_DELAY:
        if (g_FieldWindowDelay[windowIndex] == 0) {
            g_FieldWindows[windowIndex].state = MSG_WINDOW_TEXT_REVEAL;
        } else {
            (g_FieldWindowDelay[windowIndex])--;
        }
        break;
    case MSG_WINDOW_WAIT_INPUT_SCROLL_ROW:
        if ((*(int *)&g_pFieldState->newActiveKeys2 & INPUT_CIRCLE) != 0 && g_FieldWindows[windowIndex].currentRow == ((g_FieldWindows[windowIndex].height - 9) / 16) + g_FieldFollowerCoords[windowIndex] + -1) {
            g_FieldWindows[windowIndex].state = MSG_WINDOW_ROW_SCROLL;
            g_FieldWindows[windowIndex].textScrolling -= 2;
            (g_FieldFollowerCoords[windowIndex])++;
        }
        break;
    case MSG_WINDOW_TEXT_DONE:
        if (((short)g_FieldWindows[windowIndex].preventClose & MSG_WINDOW_MODE_NO_PLAYER_CLOSE) != 0) { /* empty first arm: lib/codegen.h */
        } else {
            if ((*(int *)&g_pFieldState->newActiveKeys2 & INPUT_CIRCLE) != 0) {
                g_FieldWindows[windowIndex].state = MSG_WINDOW_CLOSING;
                Field_WindowStepCloseAnimation(windowIndex);
            }
        }
        break;
    case MSG_WINDOW_WAIT_INPUT_CLEAR_PAGE:
        if ((*(int *)&g_pFieldState->newActiveKeys2 & INPUT_CIRCLE) != 0) {
            Field_WindowClearText(windowIndex);
        }
        break;
    case MSG_WINDOW_WAIT_INPUT_SCROLL_PAGE:
        if ((*(int *)&g_pFieldState->newActiveKeys2 & INPUT_CIRCLE) != 0) {
            g_FieldWindows[windowIndex].state = MSG_WINDOW_PAGE_SCROLL_OUT;
            ((short *)&g_FieldMovieOffset)[windowIndex] = (g_FieldWindows[windowIndex].currentRow << 4) + 0x11;
            g_FieldWindows[windowIndex].textScrolling -= 2;
        }
        break;
    case MSG_WINDOW_PAGE_CLEAR:
        Field_WindowClearText(windowIndex);
        break;
    case MSG_WINDOW_CLOSING_ALT:
    case MSG_WINDOW_CLOSING:
        if (Field_WindowStepCloseAnimation(windowIndex) != 0) {
            return 1;
        }
        break;
    }
    return 0;
}

/*
 * Advances one field window state, returning one only when its opening helper or its close helper
 * reports completion; otherwise returns zero. Dispatches opening, scrolling, delayed, and closing
 * states to their helpers, uses Circle for state transitions, and decrements the per-window delay
 * in MSG_WINDOW_WAIT_DELAY. In MSG_WINDOW_TEXT_DONE, unless MSG_WINDOW_MODE_NO_PLAYER_CLOSE blocks
 * input, processes Up and Down in turn, clamps the caller SHORT between minChoice and maxChoice,
 * updates cursor coordinates, and on Circle enters MSG_WINDOW_CLOSING. A closing window toggles the
 * cursor-visible byte while waiting and clears it on completion. Unknown states do nothing.
 */
// FUNCTION: FF7 0x006310a1
int __cdecl Field_WindowUpdateChoice(unsigned char windowIndex, unsigned char textIndex, unsigned char minChoice, unsigned char maxChoice, short *choicePtr)
{
    switch (g_FieldWindows[windowIndex].state) {
    case MSG_WINDOW_CLOSED:
        if (Field_WindowBeginOpen(windowIndex, textIndex) != 0) {
            return 1;
        }
        break;
    case MSG_WINDOW_OPENING:
        Field_WindowStepOpenGrow(windowIndex);
        break;
    case MSG_WINDOW_TEXT_REVEAL:
        Field_WindowStepTextReveal(windowIndex);
        break;
    case MSG_WINDOW_ROW_SCROLL:
        Field_WindowStepRowScroll(windowIndex);
        break;
    case MSG_WINDOW_PAGE_SCROLL_OUT:
        Field_WindowStepPageScrollOut(windowIndex);
        break;
    case MSG_WINDOW_WAIT_INPUT_RESUME:
        if ((*(int *)&g_pFieldState->newActiveKeys2 & INPUT_CIRCLE) != 0) {
            g_FieldWindows[windowIndex].state = MSG_WINDOW_TEXT_REVEAL;
        }
        break;
    case MSG_WINDOW_WAIT_DELAY:
        if (g_FieldWindowDelay[windowIndex] == 0) {
            g_FieldWindows[windowIndex].state = MSG_WINDOW_TEXT_REVEAL;
        } else {
            (g_FieldWindowDelay[windowIndex])--;
        }
        break;
    case MSG_WINDOW_WAIT_INPUT_SCROLL_ROW:
        if ((*(int *)&g_pFieldState->newActiveKeys2 & INPUT_CIRCLE) != 0 && g_FieldWindows[windowIndex].currentRow == ((g_FieldWindows[windowIndex].height - 9) / 16) + g_FieldFollowerCoords[windowIndex] + -1) {
            g_FieldWindows[windowIndex].state = MSG_WINDOW_ROW_SCROLL;
            g_FieldWindows[windowIndex].textScrolling -= 2;
            (g_FieldFollowerCoords[windowIndex])++;
        }
        break;
    case MSG_WINDOW_TEXT_DONE:
        if (((short)g_FieldWindows[windowIndex].preventClose & MSG_WINDOW_MODE_NO_PLAYER_CLOSE) != 0) { /* empty first arm: lib/codegen.h */
        } else {
            g_FieldWindows[windowIndex].pointerEnabled = 1;
            if ((*(int *)&g_pFieldState->newActiveKeys & INPUT_UP) != 0) {
                if (*choicePtr > minChoice) {
                    Field_WindowPlayCursorSound();
                }
                (*choicePtr)--;
            }
            if ((*(int *)&g_pFieldState->newActiveKeys & INPUT_DOWN) != 0) {
                if (*choicePtr < maxChoice) {
                    Field_WindowPlayCursorSound();
                }
                (*choicePtr)++;
            }
            if (*choicePtr < minChoice) {
                *choicePtr = minChoice;
            }
            if (*choicePtr > maxChoice) {
                *choicePtr = maxChoice;
            }
            g_FieldWindows[windowIndex].pointerX = 5;
            g_FieldWindows[windowIndex].pointerY = (*choicePtr << 4) + 6;
            if ((*(int *)&g_pFieldState->newActiveKeys2 & INPUT_CIRCLE) != 0) {
                Field_WindowPlayCursorSound();
                g_FieldWindows[windowIndex].state = MSG_WINDOW_CLOSING;
                Field_WindowStepCloseAnimation(windowIndex);
            }
        }
        break;
    case MSG_WINDOW_WAIT_INPUT_CLEAR_PAGE:
        if ((*(int *)&g_pFieldState->newActiveKeys2 & INPUT_CIRCLE) != 0) {
            Field_WindowClearText(windowIndex);
        }
        break;
    case MSG_WINDOW_WAIT_INPUT_SCROLL_PAGE:
        if ((*(int *)&g_pFieldState->newActiveKeys2 & INPUT_CIRCLE) != 0) {
            g_FieldWindows[windowIndex].state = MSG_WINDOW_PAGE_SCROLL_OUT;
            ((short *)&g_FieldMovieOffset)[windowIndex] = (g_FieldWindows[windowIndex].currentRow << 4) + 0x11;
            g_FieldWindows[windowIndex].textScrolling -= 2;
        }
        break;
    case MSG_WINDOW_PAGE_CLEAR:
        Field_WindowClearText(windowIndex);
        break;
    case MSG_WINDOW_CLOSING_ALT:
    case MSG_WINDOW_CLOSING:
        if (Field_WindowStepCloseAnimation(windowIndex) != 0) {
            g_FieldWindows[windowIndex].pointerEnabled = 0;
            return 1;
        }
        g_FieldWindows[windowIndex].pointerEnabled ^= 1;
    }
    return 0;
}

/*
 * Plays the menu cursor sound: posts SOUND_SFX_PRELOAD_PLAY_CH5 with SFX_MENU_CURSOR and 0x40; clears the request block with
 * Field_ClearRequest, fills code and args[0..1], and hands them to SoundDispatchCommand with the
 * other six arguments zero.
 */
// FUNCTION: FF7 0x00631527
void __cdecl Field_WindowPlayCursorSound(void)
{
    Field_ClearRequest();
    g_pFieldRequest->code = SOUND_SFX_PRELOAD_PLAY_CH5;
    g_pFieldRequest->args[0] = SFX_MENU_CURSOR;
    g_pFieldRequest->args[1] = 0x40;
    SoundDispatchCommand(g_pFieldRequest->code, g_pFieldRequest->args[0], g_pFieldRequest->args[1], 0, 0, 0, 0, 0, 0);
}

/*
 * Returns one when field text data is absent and zero when the window is already owned. Otherwise
 * assigns the current entity as owner, initializes two window size increments to dimensions
 * divided by four with minimum eight, clears the listed window counters and flags, and points the
 * window at its 256-byte text buffer with a terminator in the first byte. Selects the source text
 * by its little-endian offset table, initializes associated state arrays and window field 44,
 * increments the global active count, and returns zero.
 */
// FUNCTION: FF7 0x00631586
int __cdecl Field_WindowBeginOpen(short windowIndex, short textIndex)
{
    if (g_FieldDialogTextPtr == 0) {
        return 1;
    } else if (g_FieldWindowOwner[windowIndex] != 0xFF) {
        return 0;
    } else {
        g_FieldWindowOwner[windowIndex] = g_CurrentEntity;
        g_FieldWindows[windowIndex].currentWidth = (g_FieldWindows[windowIndex].width / 4);
        g_FieldWindows[windowIndex].currentHeight = (g_FieldWindows[windowIndex].height / 4);
        if (g_FieldWindows[windowIndex].currentHeight < 8) {
            g_FieldWindows[windowIndex].currentHeight = 8;
        }
        if (g_FieldWindows[windowIndex].currentWidth < 8) {
            g_FieldWindows[windowIndex].currentWidth = 8;
        }
        g_FieldWindows[windowIndex].textScrolling = 0;
        g_FieldWindows[windowIndex].stringLength = 0;
        g_FieldWindows[windowIndex].stringByteLength = 0;
        g_FieldWindows[windowIndex].currentRow = 0;
        g_FieldWindows[windowIndex].pointerEnabled = 0;
        g_FieldWindows[windowIndex].text = g_FieldWindowText[windowIndex];
        g_FieldWindowText[windowIndex][0] = TEXT_END;
        g_FieldWindowTextPointers[windowIndex] = (int)g_FieldDialogTextPtr;
        g_FieldWindowTextPointers[windowIndex] += *((unsigned char *)g_FieldDialogTextPtr + textIndex * 2 + 2);
        g_FieldWindowTextPointers[windowIndex] += *((unsigned char *)g_FieldDialogTextPtr + textIndex * 2 + 3) << 8;
        g_FieldWindowSpeed[windowIndex] = 1;
        g_FieldWindowRevealProgress[windowIndex] = 0;
        g_FieldFollowerCoords[windowIndex] = 0;
        FIELD_WINDOW_NAME_POS(windowIndex) = 0;
        g_FieldEntityParams[windowIndex] = 0;
        g_FieldWindowPos[windowIndex] = 0xFFFF;
        g_FieldWindows[windowIndex].state = MSG_WINDOW_OPENING;
        g_FieldOpenWindowCount++;
        return 0;
    }
}

/*
 * Only owning current entity expands window width/height by respective target dimension divided by
 * four, lower-clamps eight then upper-clamps target. Marks MSG_WINDOW_TEXT_REVEAL when both equal targets.
 * Targets below eight override lower clamp; signed-short accumulation can wrap and zero step can
 * stall.
 */
// FUNCTION: FF7 0x006317a9
void __cdecl Field_WindowStepOpenGrow(short windowIndex)
{
    if (g_FieldWindowOwner[windowIndex] != g_CurrentEntity) {
        return;
    }
    g_FieldWindows[windowIndex].currentWidth += (short)((g_FieldWindows[windowIndex].width / 4));
    if (g_FieldWindows[windowIndex].currentWidth < 8) {
        g_FieldWindows[windowIndex].currentWidth = 8;
    }
    if (g_FieldWindows[windowIndex].currentWidth > g_FieldWindows[windowIndex].width) {
        g_FieldWindows[windowIndex].currentWidth = g_FieldWindows[windowIndex].width;
    }
    g_FieldWindows[windowIndex].currentHeight += (short)((g_FieldWindows[windowIndex].height / 4));
    if (g_FieldWindows[windowIndex].currentHeight < 8) {
        g_FieldWindows[windowIndex].currentHeight = 8;
    }
    if (g_FieldWindows[windowIndex].currentHeight > g_FieldWindows[windowIndex].height) {
        g_FieldWindows[windowIndex].currentHeight = g_FieldWindows[windowIndex].height;
    }
    if (g_FieldWindows[windowIndex].currentWidth == g_FieldWindows[windowIndex].width && g_FieldWindows[windowIndex].currentHeight == g_FieldWindows[windowIndex].height) {
        g_FieldWindows[windowIndex].state = MSG_WINDOW_TEXT_REVEAL;
    }
}

/*
 * Advances text for a window owned by the current entity using speed accumulation and input-
 * dependent acceleration. It expands character or party names, numeric substitutions and savemap
 * text; copies ordinary and extended control bytes; and stops at end, page, wait or timed-control
 * states. It maintains output/line counters and terminates the output with 0xFF. The savemap
 * substitution length and output positions are unchecked, so nominal per-window buffers can be
 * overrun; source operands and window index are assumed valid.
 */
// FUNCTION: FF7 0x00631945
void __cdecl Field_WindowStepTextReveal(short windowIndex)
{
    int flags;
    unsigned char *namePtr;
    unsigned short val;
    short stepScale;
    short baseStep;
    short index;
    unsigned short bytes;

    flags = 0;
    if (g_FieldWindowOwner[windowIndex] != g_CurrentEntity) {
        return;
    }
    if (((short)g_FieldWindows[windowIndex].preventClose & MSG_WINDOW_MODE_INSTANT_TEXT) != 0) {
        baseStep = 0x100;
        stepScale = 1;
    } else {
        if ((*(int *)&g_pFieldState->activeKeys2 & INPUT_CIRCLE) != 0) {
            (g_FieldWindowSpeed[windowIndex])++;
            if (g_FieldWindowSpeed[windowIndex] > 128) {
                g_FieldWindowSpeed[windowIndex] = 128;
            }
        } else {
            (g_FieldWindowSpeed[windowIndex])--;
            if (g_FieldWindowSpeed[windowIndex] <= 1) {
                g_FieldWindowSpeed[windowIndex] = 1;
            }
        }
        if (gSavemap.fieldMessageSpeed < 128) {
            baseStep = (0x80 - gSavemap.fieldMessageSpeed >> 5) + 2;
            stepScale = 1;
        } else {
            baseStep = 2;
            stepScale = (gSavemap.fieldMessageSpeed - 0x80 >> 5) + 1;
        }
    }
    g_FieldWindowRevealProgress[windowIndex] += (short)(baseStep + (g_FieldWindowSpeed[windowIndex] >> 4) * stepScale);
    while (g_FieldWindowRevealProgress[windowIndex] > stepScale) {
        switch (((FieldWindowTextState *)(g_FieldWindowTextPointers[windowIndex]))->code) {
        case TEXT_END:
            g_FieldWindows[windowIndex].state = MSG_WINDOW_TEXT_DONE;
            g_FieldWindowRevealProgress[windowIndex] = 0;
            goto done;
        case TEXT_NEWLINE:
            if (g_FieldWindows[windowIndex].currentRow == ((g_FieldWindows[windowIndex].height - 9) / 16) + g_FieldFollowerCoords[windowIndex] + -1) {
                g_FieldWindows[windowIndex].state = MSG_WINDOW_WAIT_INPUT_SCROLL_ROW;
                g_FieldWindowSpeed[windowIndex] = 1;
                g_FieldWindowRevealProgress[windowIndex] = 0;
                goto done;
            }
            g_FieldWindowText[windowIndex][g_FieldWindows[windowIndex].stringByteLength] = ((FieldWindowTextState *)(g_FieldWindowTextPointers[windowIndex]))->code;
            (g_FieldWindowTextPointers[windowIndex])++;
            (g_FieldWindows[windowIndex].stringByteLength)++;
            (g_FieldWindows[windowIndex].currentRow)++;
            break;
        case TEXT_NEW_PAGE:
        case TEXT_NEW_PAGE_ALT:
            (g_FieldWindowTextPointers[windowIndex])++;
            g_FieldWindows[windowIndex].state = MSG_WINDOW_WAIT_INPUT_CLEAR_PAGE;
            g_FieldWindowSpeed[windowIndex] = 1;
            g_FieldWindowRevealProgress[windowIndex] = 0;
            goto done;
        case TEXT_TAB:
            (g_FieldWindowTextPointers[windowIndex])++;
            for (index = 0; index < 4; index++) {
                g_FieldWindowText[windowIndex][g_FieldWindows[windowIndex].stringByteLength] = 0;
                (g_FieldWindows[windowIndex].stringByteLength)++;
                (g_FieldWindows[windowIndex].stringLength)++;
            }
            break;
        case TEXT_WIDE_TAB:
            (g_FieldWindowTextPointers[windowIndex])++;
            for (index = 0; index < 10; index++) {
                g_FieldWindowText[windowIndex][g_FieldWindows[windowIndex].stringByteLength] = 0;
                (g_FieldWindows[windowIndex].stringByteLength)++;
                (g_FieldWindows[windowIndex].stringLength)++;
            }
            break;
        case TEXT_COMMA_SPACE:
            (g_FieldWindowTextPointers[windowIndex])++;
            g_FieldWindowText[windowIndex][g_FieldWindows[windowIndex].stringByteLength] = 0xC;
            (g_FieldWindows[windowIndex].stringByteLength)++;
            g_FieldWindowText[windowIndex][g_FieldWindows[windowIndex].stringByteLength] = 0;
            (g_FieldWindows[windowIndex].stringByteLength)++;
            g_FieldWindows[windowIndex].stringLength += 2;
            break;
        case TEXT_PERIOD_QUOTE:
            (g_FieldWindowTextPointers[windowIndex])++;
            g_FieldWindowText[windowIndex][g_FieldWindows[windowIndex].stringByteLength] = 0xE;
            (g_FieldWindows[windowIndex].stringByteLength)++;
            g_FieldWindowText[windowIndex][g_FieldWindows[windowIndex].stringByteLength] = 2;
            (g_FieldWindows[windowIndex].stringByteLength)++;
            g_FieldWindows[windowIndex].stringLength += 2;
            break;
        case TEXT_ELLIPSIS_QUOTE:
            (g_FieldWindowTextPointers[windowIndex])++;
            g_FieldWindowText[windowIndex][g_FieldWindows[windowIndex].stringByteLength] = 0xA9;
            (g_FieldWindows[windowIndex].stringByteLength)++;
            g_FieldWindowText[windowIndex][g_FieldWindows[windowIndex].stringByteLength] = 2;
            (g_FieldWindows[windowIndex].stringByteLength)++;
            g_FieldWindows[windowIndex].stringLength += 2;
            break;
        case TEXT_NAME_CLOUD:
        case TEXT_NAME_BARRET:
        case TEXT_NAME_TIFA:
        case TEXT_NAME_AERIS:
        case TEXT_NAME_RED_XIII:
        case TEXT_NAME_YUFFIE:
        case TEXT_NAME_CAIT_SITH:
        case TEXT_NAME_VINCENT:
        case TEXT_NAME_CID:
            val = ((FieldWindowTextState *)(g_FieldWindowTextPointers[windowIndex]))->code - TEXT_NAME_CLOUD;
            namePtr = (unsigned char *)Savemap_GetCharacterName(val);
            if (namePtr[FIELD_WINDOW_NAME_POS(windowIndex)] == TEXT_END || FIELD_WINDOW_NAME_POS(windowIndex) >= 9) {
                (g_FieldWindowTextPointers[windowIndex])++;
                FIELD_WINDOW_NAME_POS(windowIndex) = 0;
            } else {
                g_FieldWindowText[windowIndex][g_FieldWindows[windowIndex].stringByteLength] = namePtr[FIELD_WINDOW_NAME_POS(windowIndex)];
                (g_FieldWindows[windowIndex].stringByteLength)++;
                (FIELD_WINDOW_NAME_POS(windowIndex))++;
                (g_FieldWindows[windowIndex].stringLength)++;
                g_FieldWindowRevealProgress[windowIndex] -= stepScale;
            }
            break;
        case TEXT_PARTY_1:
        case TEXT_PARTY_2:
        case TEXT_PARTY_3:
            /* the party member's id: fieldParty[code - TEXT_PARTY_1], the subtraction folded into the address */
            val = *(unsigned char *)((char *)&gSavemap.fieldParty[-TEXT_PARTY_1] + ((FieldWindowTextState *)(g_FieldWindowTextPointers[windowIndex]))->code);
            if (val == 0xFF) {
                if (FIELD_WINDOW_NAME_POS(windowIndex) >= 9) {
                    (g_FieldWindowTextPointers[windowIndex])++;
                    FIELD_WINDOW_NAME_POS(windowIndex) = 0;
                } else {
                    g_FieldWindowText[windowIndex][g_FieldWindows[windowIndex].stringByteLength] = 0xA9;
                    (g_FieldWindows[windowIndex].stringByteLength)++;
                    (FIELD_WINDOW_NAME_POS(windowIndex))++;
                    (g_FieldWindows[windowIndex].stringLength)++;
                    g_FieldWindowRevealProgress[windowIndex] -= stepScale;
                }
            } else {
                namePtr = (unsigned char *)Savemap_GetCharacterName(val);
                if (namePtr[FIELD_WINDOW_NAME_POS(windowIndex)] == TEXT_END || FIELD_WINDOW_NAME_POS(windowIndex) >= 9) {
                    (g_FieldWindowTextPointers[windowIndex])++;
                    FIELD_WINDOW_NAME_POS(windowIndex) = 0;
                } else {
                    g_FieldWindowText[windowIndex][g_FieldWindows[windowIndex].stringByteLength] = namePtr[FIELD_WINDOW_NAME_POS(windowIndex)];
                    (g_FieldWindows[windowIndex].stringByteLength)++;
                    (FIELD_WINDOW_NAME_POS(windowIndex))++;
                    (g_FieldWindows[windowIndex].stringLength)++;
                    g_FieldWindowRevealProgress[windowIndex] -= stepScale;
                }
            }
            break;
        case TEXT_EXTENDED:
            g_FieldWindowText[windowIndex][g_FieldWindows[windowIndex].stringByteLength] = ((FieldWindowTextState *)(g_FieldWindowTextPointers[windowIndex]))->code;
            (g_FieldWindowTextPointers[windowIndex])++;
            (g_FieldWindows[windowIndex].stringByteLength)++;
            switch (((FieldWindowTextState *)(g_FieldWindowTextPointers[windowIndex]))->code) {
            case TEXT_FE_PAUSE:
                (g_FieldWindows[windowIndex].stringByteLength)--;
                (g_FieldWindowTextPointers[windowIndex])++;
                g_FieldWindows[windowIndex].state = MSG_WINDOW_WAIT_INPUT_RESUME;
                g_FieldWindowSpeed[windowIndex] = 1;
                g_FieldWindowRevealProgress[windowIndex] = 0;
                goto done;
            case TEXT_FE_WAIT_SCROLL_PAGE:
                (g_FieldWindows[windowIndex].stringByteLength)--;
                (g_FieldWindowTextPointers[windowIndex])++;
                g_FieldWindows[windowIndex].state = MSG_WINDOW_WAIT_INPUT_SCROLL_PAGE;
                g_FieldWindowSpeed[windowIndex] = 1;
                g_FieldWindowRevealProgress[windowIndex] = 0;
                goto done;
            case TEXT_FE_NUM_DECIMAL:
            case TEXT_FE_NUM_HEX:
            case TEXT_FE_NUM_DECIMAL_PADDED:
                (g_FieldWindows[windowIndex].stringByteLength)--;
                (g_FieldWindowTextPointers[windowIndex])--;
                if (g_FieldWindowPos[windowIndex] == -1) {
                    val = Field_WindowReadReplacementValue((short)windowIndex);
                    switch (((FieldWindowTextState *)(g_FieldWindowTextPointers[windowIndex]))->command) {
                    case TEXT_FE_NUM_DECIMAL:
                        Field_FormatDecimalTextTrimmed((short)val, &g_FieldEntityScriptTable[windowIndex << 4]);
                        break;
                    case TEXT_FE_NUM_DECIMAL_PADDED:
                        Field_FormatDecimalTextPadded((short)val, &g_FieldEntityScriptTable[windowIndex << 4]);
                        break;
                    case TEXT_FE_NUM_HEX:
                        Field_FormatHexText((short)val, &g_FieldEntityScriptTable[windowIndex << 4]);
                    }
                    (g_FieldWindowPos[windowIndex])++;
                } else if (g_FieldEntityScriptTable[(windowIndex << 4) + g_FieldWindowPos[windowIndex]] == TEXT_END || g_FieldWindowPos[windowIndex] >= 16) {
                    g_FieldWindowTextPointers[windowIndex] += 2;
                    g_FieldWindowPos[windowIndex] = 0xFFFF;
                    (g_FieldEntityParams[windowIndex])++;
                } else {
                    g_FieldWindowText[windowIndex][g_FieldWindows[windowIndex].stringByteLength] = g_FieldEntityScriptTable[(windowIndex << 4) + g_FieldWindowPos[windowIndex]];
                    (g_FieldWindows[windowIndex].stringByteLength)++;
                    (g_FieldWindowPos[windowIndex])++;
                    (g_FieldWindows[windowIndex].stringLength)++;
                    g_FieldWindowRevealProgress[windowIndex] -= stepScale;
                }
                break;
            case TEXT_FE_BANK_STRING:
                (g_FieldWindows[windowIndex].stringByteLength)--;
                (g_FieldWindowTextPointers[windowIndex])--;
                if (g_FieldWindowPos[windowIndex] == -1) {
                    val = ((FieldWindowTextState *)(g_FieldWindowTextPointers[windowIndex]))->valueLo;
                    val |= (short)(((FieldWindowTextState *)(g_FieldWindowTextPointers[windowIndex]))->valueHi << 8);
                    bytes = ((FieldWindowTextState *)(g_FieldWindowTextPointers[windowIndex]))->countLo;
                    bytes |= (short)(((FieldWindowTextState *)(g_FieldWindowTextPointers[windowIndex]))->countHi << 8);
                    for (index = 0; index < bytes; index++) {
                        g_FieldEntityScriptTable[(windowIndex << 4) + index] = gSavemap.scriptBanks[val + index];
                    }
                    g_FieldEntityScriptTable[(windowIndex << 4) + index] = TEXT_END;
                    (g_FieldWindowPos[windowIndex])++;
                } else if (g_FieldEntityScriptTable[(windowIndex << 4) + g_FieldWindowPos[windowIndex]] == TEXT_END) {
                    g_FieldWindowTextPointers[windowIndex] += 6;
                    g_FieldWindowPos[windowIndex] = 0xFFFF;
                } else {
                    g_FieldWindowText[windowIndex][g_FieldWindows[windowIndex].stringByteLength] = g_FieldEntityScriptTable[(windowIndex << 4) + g_FieldWindowPos[windowIndex]];
                    (g_FieldWindows[windowIndex].stringByteLength)++;
                    (g_FieldWindowPos[windowIndex])++;
                    (g_FieldWindows[windowIndex].stringLength)++;
                    g_FieldWindowRevealProgress[windowIndex] -= stepScale;
                }
                break;
            case TEXT_FE_COLOR_0:
            case TEXT_FE_COLOR_1:
            case TEXT_FE_COLOR_2:
            case TEXT_FE_COLOR_3:
            case TEXT_FE_COLOR_4:
            case TEXT_FE_COLOR_5:
            case TEXT_FE_COLOR_6:
            case TEXT_FE_COLOR_7:
            case TEXT_FE_BLINK:
            case TEXT_FE_RAINBOW:
            case TEXT_FE_FIXED_PITCH:
                g_FieldWindowText[windowIndex][g_FieldWindows[windowIndex].stringByteLength] = ((FieldWindowTextState *)(g_FieldWindowTextPointers[windowIndex]))->code;
                (g_FieldWindowTextPointers[windowIndex])++;
                (g_FieldWindows[windowIndex].stringByteLength)++;
                break;
            case TEXT_FE_WAIT:
                g_FieldWindows[windowIndex].state = MSG_WINDOW_WAIT_DELAY;
                (g_FieldWindowTextPointers[windowIndex])++;
                (g_FieldWindows[windowIndex].stringByteLength)++;
                g_FieldWindowDelay[windowIndex] = (short)*(volatile unsigned char *)g_FieldWindowTextPointers[windowIndex];
                (g_FieldWindowTextPointers[windowIndex])++;
                (g_FieldWindows[windowIndex].stringByteLength)++;
                g_FieldWindowDelay[windowIndex] |= (short)(((FieldWindowTextState *)(g_FieldWindowTextPointers[windowIndex]))->code << 8);
                (g_FieldWindowTextPointers[windowIndex])++;
                (g_FieldWindows[windowIndex].stringByteLength)++;
                goto done;
            default:
                g_FieldWindowText[windowIndex][g_FieldWindows[windowIndex].stringByteLength] = ((FieldWindowTextState *)(g_FieldWindowTextPointers[windowIndex]))->code;
                (g_FieldWindowTextPointers[windowIndex])++;
                (g_FieldWindows[windowIndex].stringByteLength)++;
                (g_FieldWindows[windowIndex].stringLength)++;
                g_FieldWindowRevealProgress[windowIndex] -= stepScale;
            }
            break;
        case TEXT_FONT_PAGE_1:
        case TEXT_FONT_PAGE_2:
        case TEXT_FONT_PAGE_3:
        case TEXT_FONT_PAGE_4:
            g_FieldWindowText[windowIndex][g_FieldWindows[windowIndex].stringByteLength] = ((FieldWindowTextState *)(g_FieldWindowTextPointers[windowIndex]))->code;
            (g_FieldWindowTextPointers[windowIndex])++;
            (g_FieldWindows[windowIndex].stringByteLength)++;
        default:
            g_FieldWindowText[windowIndex][g_FieldWindows[windowIndex].stringByteLength] = ((FieldWindowTextState *)(g_FieldWindowTextPointers[windowIndex]))->code;
            (g_FieldWindowTextPointers[windowIndex])++;
            (g_FieldWindows[windowIndex].stringByteLength)++;
            (g_FieldWindows[windowIndex].stringLength)++;
            g_FieldWindowRevealProgress[windowIndex] -= stepScale;
        }
    }
done:
    g_FieldWindowText[windowIndex][g_FieldWindows[windowIndex].stringByteLength] = TEXT_END;
    return;
}

/*
 * Scrolls a window's text one step for the entity that owns it: does nothing unless
 * g_FieldWindowOwner[window] is g_CurrentEntity; then, while textScrolling has any of its low four
 * bits set, subtracts 2 from it, and otherwise sets the window's state to MSG_WINDOW_TEXT_REVEAL.
 */
// FUNCTION: FF7 0x00632caa
void __cdecl Field_WindowStepRowScroll(short window)
{
    if (g_FieldWindowOwner[window] != g_CurrentEntity) {
        return;
    }
    if ((g_FieldWindows[window].textScrolling & 0xF) != 0) {
        g_FieldWindows[window].textScrolling -= 2;
    } else {
        g_FieldWindows[window].state = MSG_WINDOW_TEXT_REVEAL;
    }
}

/*
 * Returns unless current entity owns the window. While offset+0x10 plus its companion short is
 * positive subtracts speed>>2, then increases speed up to 0x80 while Circle is held or
 * decreases it to at least one otherwise. When the sum is not positive writes MSG_WINDOW_PAGE_CLEAR. No
 * window-index check is present.
 */
// FUNCTION: FF7 0x00632d10
void __cdecl Field_WindowStepPageScrollOut(short windowIndex)
{
    if (g_FieldWindowOwner[windowIndex] != g_CurrentEntity) {
        return;
    }
    if (g_FieldWindows[windowIndex].textScrolling + ((short *)&g_FieldMovieOffset)[windowIndex] > 0) {
        g_FieldWindows[windowIndex].textScrolling -= (short)(g_FieldWindowSpeed[windowIndex] >> 2);
        if ((*(int *)&g_pFieldState->activeKeys2 & INPUT_CIRCLE) != 0) {
            (g_FieldWindowSpeed[windowIndex])++;
            if (g_FieldWindowSpeed[windowIndex] > 128) {
                g_FieldWindowSpeed[windowIndex] = 128;
            }
        } else {
            (g_FieldWindowSpeed[windowIndex])--;
            if (g_FieldWindowSpeed[windowIndex] <= 1) {
                g_FieldWindowSpeed[windowIndex] = 1;
            }
        }
    } else {
        g_FieldWindows[windowIndex].state = MSG_WINDOW_PAGE_CLEAR;
    }
}

/*
 * Clears a window's text for the entity that owns it (g_FieldWindowOwner[window] ==
 * g_CurrentEntity): stringByteLength, stringLength, textScrolling and currentRow to 0, state to MSG_WINDOW_TEXT_REVEAL,
 * the first byte of its g_FieldWindowText text row to the 0xFF terminator, g_FieldFollowerCoords[window] to 0
 * and g_FieldWindowSpeed[window] to 1.
 */
// FUNCTION: FF7 0x00632e1f
void __cdecl Field_WindowClearText(short window)
{
    if (g_FieldWindowOwner[window] != g_CurrentEntity) {
        return;
    }
    g_FieldWindows[window].stringByteLength = 0;
    g_FieldWindows[window].stringLength = 0;
    g_FieldWindows[window].textScrolling = 0;
    g_FieldWindows[window].currentRow = 0;
    g_FieldWindows[window].state = MSG_WINDOW_TEXT_REVEAL;
    g_FieldWindowText[window][0] = TEXT_END;
    g_FieldFollowerCoords[window] = 0;
    g_FieldWindowSpeed[window] = 1;
}

/*
 * Returns zero unless current entity owns the window. For each current dimension sets it to eight
 * when below eight, otherwise subtracts configured dimension/4. When both are at most eight clears
 * the selected field/state, releases owner to 0xFF, decrements window count and returns one;
 * otherwise returns zero. The subtraction may pass below eight.
 */
// FUNCTION: FF7 0x00632eb8
int __cdecl Field_WindowStepCloseAnimation(short windowIndex)
{
    if (g_FieldWindowOwner[windowIndex] != g_CurrentEntity) {
        return 0;
    } else if (g_FieldWindows[windowIndex].currentWidth < 8) {
        g_FieldWindows[windowIndex].currentWidth = 8;
    } else {
        g_FieldWindows[windowIndex].currentWidth -= (short)((g_FieldWindows[windowIndex].width / 4));
    }
    if (g_FieldWindows[windowIndex].currentHeight < 8) {
        g_FieldWindows[windowIndex].currentHeight = 8;
    } else {
        g_FieldWindows[windowIndex].currentHeight -= (short)((g_FieldWindows[windowIndex].height / 4));
    }
    if (g_FieldWindows[windowIndex].currentWidth <= 8 && g_FieldWindows[windowIndex].currentHeight <= 8) {
        g_FieldWindows[windowIndex].stringLength = 0;
        g_FieldWindows[windowIndex].state = MSG_WINDOW_CLOSED;
        g_FieldWindowOwner[windowIndex] = 0xFF;
        g_FieldOpenWindowCount--;
        return 1;
    } else {
        return 0;
    }
}

/*
 * Resolves the current window replacement value from its bank selector and stored address.
 * FIELD_BANK_LITERAL returns the narrowed literal. The savemap banks read bytes or little-endian
 * SHORTs from regions at offsets zero, 256, 512, 768, and 1024, and the temp banks a byte or SHORT
 * of g_FieldTempBank. Unsupported banks return zero. Returned bits preserve the selected SHORT
 * value.
 */
// FUNCTION: FF7 0x00632ffb
unsigned short __cdecl Field_WindowReadReplacementValue(short windowIndex)
{
    short replaceValue;
    unsigned short address;

    switch (g_WindowReplaceBank[windowIndex][g_FieldEntityParams[windowIndex]]) {
    case FIELD_BANK_LITERAL:
        replaceValue = (short)g_WindowReplaceBankAddr[windowIndex][g_FieldEntityParams[windowIndex]];
        break;
    case FIELD_BANK_SAVE_000_BYTE:
        address = (short)g_WindowReplaceBankAddr[windowIndex][g_FieldEntityParams[windowIndex]];
        replaceValue = gSavemap.scriptBanks[address];
        break;
    case FIELD_BANK_SAVE_000_WORD:
        address = (short)g_WindowReplaceBankAddr[windowIndex][g_FieldEntityParams[windowIndex]];
        replaceValue = gSavemap.scriptBanks[address];
        replaceValue |= (short)(g_SavemapScriptBanksHi[address] << 8);
        break;
    case FIELD_BANK_SAVE_100_BYTE:
        address = (short)g_WindowReplaceBankAddr[windowIndex][g_FieldEntityParams[windowIndex]] + 0x100;
        replaceValue = gSavemap.scriptBanks[address];
        break;
    case FIELD_BANK_SAVE_100_WORD:
        address = (short)g_WindowReplaceBankAddr[windowIndex][g_FieldEntityParams[windowIndex]] + 0x100;
        replaceValue = gSavemap.scriptBanks[address];
        replaceValue |= (short)(g_SavemapScriptBanksHi[address] << 8);
        break;
    case FIELD_BANK_SAVE_200_BYTE:
        address = (short)g_WindowReplaceBankAddr[windowIndex][g_FieldEntityParams[windowIndex]] + 0x200;
        replaceValue = gSavemap.scriptBanks[address];
        break;
    case FIELD_BANK_SAVE_200_WORD:
        address = (short)g_WindowReplaceBankAddr[windowIndex][g_FieldEntityParams[windowIndex]] + 0x200;
        replaceValue = gSavemap.scriptBanks[address];
        replaceValue |= (short)(g_SavemapScriptBanksHi[address] << 8);
        break;
    case FIELD_BANK_SAVE_300_BYTE:
        address = (short)g_WindowReplaceBankAddr[windowIndex][g_FieldEntityParams[windowIndex]] + 0x300;
        replaceValue = gSavemap.scriptBanks[address];
        break;
    case FIELD_BANK_SAVE_400_BYTE:
        address = (short)g_WindowReplaceBankAddr[windowIndex][g_FieldEntityParams[windowIndex]] + 0x400;
        replaceValue = gSavemap.scriptBanks[address];
        break;
    case FIELD_BANK_SAVE_300_WORD:
        address = (short)g_WindowReplaceBankAddr[windowIndex][g_FieldEntityParams[windowIndex]] + 0x300;
        replaceValue = gSavemap.scriptBanks[address];
        replaceValue |= (short)(g_SavemapScriptBanksHi[address] << 8);
        break;
    case FIELD_BANK_SAVE_400_WORD:
        address = (short)g_WindowReplaceBankAddr[windowIndex][g_FieldEntityParams[windowIndex]] + 0x400;
        replaceValue = gSavemap.scriptBanks[address];
        replaceValue |= (short)(g_SavemapScriptBanksHi[address] << 8);
        break;
    case FIELD_BANK_TEMP_BYTE:
        address = (short)g_WindowReplaceBankAddr[windowIndex][g_FieldEntityParams[windowIndex]];
        replaceValue = g_FieldTempBank[address];
        break;
    case FIELD_BANK_TEMP_WORD:
        address = (short)g_WindowReplaceBankAddr[windowIndex][g_FieldEntityParams[windowIndex]];
        replaceValue = g_FieldTempBank[address];
        replaceValue |= (short)((&g_FieldTempBank[1])[address] << 8);
        break;
    default:
        replaceValue = 0;
    }
    return (short)replaceValue;
}

/*
 * Formats unsigned-short decimal digits using the mapping table g_FieldDigitMapping, starting with
 * divisor 10000 and suppressing leading zero digits; always emits the units digit and 0xFF
 * terminator.
 */
// FUNCTION: FF7 0x00633433
void __cdecl Field_FormatDecimalTextTrimmed(unsigned short number, unsigned char *dst)
{
    short digitValue;
    short divisor;
    short outPos;
    int started;

    started = 0;
    divisor = 0x2710;
    for (outPos = 0; divisor > 1; divisor = divisor / 10) {
        digitValue = number / divisor;
        if (started != 0 || digitValue != 0) {
            started = 1;
            dst[outPos] = *(g_FieldDigitMapping + digitValue);
            outPos++;
        }
        number -= (short)(digitValue * divisor);
    }
    dst[outPos] = *(g_FieldDigitMapping + number);
    *(dst + outPos + 1) = TEXT_END;
}

/*
 * Formats unsigned-short decimal digits using g_FieldDigitMapping with four leading positions emitted as
 * zero bytes until the first nonzero digit, followed by mapped digits, units and 0xFF terminator.
 */
// FUNCTION: FF7 0x006334f0
void __cdecl Field_FormatDecimalTextPadded(unsigned short number, unsigned char *dst)
{
    short digitValue;
    short divisor;
    short outPos;
    int started;

    started = 0;
    divisor = 0x2710;
    for (outPos = 0; divisor > 1; divisor = divisor / 10) {
        digitValue = number / divisor;
        if (started != 0 || digitValue != 0) {
            started = 1;
            dst[outPos] = *(g_FieldDigitMapping + digitValue);
            outPos++;
        } else if (started == 0) {
            dst[outPos] = 0;
            outPos++;
        }
        number -= (short)(digitValue * divisor);
    }
    dst[outPos] = *(g_FieldDigitMapping + number);
    *(dst + outPos + 1) = TEXT_END;
}

/*
 * Formats unsigned-short hexadecimal digits using g_FieldDigitMapping starting at divisor 4096 and
 * suppressing leading zero digits; emits the final digit and 0xFF terminator.
 */
// FUNCTION: FF7 0x006335d3
void __cdecl Field_FormatHexText(unsigned short number, unsigned char *dst)
{
    short digit;
    short place;
    int nonzero;
    short pos;

    nonzero = 0;
    place = 0x1000;
    for (pos = 0; place > 1; place = place / 16) {
        digit = number / place;
        if (nonzero != 0 || digit != 0) {
            nonzero = 1;
            dst[pos] = *(g_FieldDigitMapping + digit);
            pos++;
        }
        number -= (short)(digit * place);
    }
    dst[pos] = *(g_FieldDigitMapping + number);
    *(dst + pos + 1) = TEXT_END;
}

/*
 * Copies field text string `id` into the savemap's location name (gSavemap.location), at most
 * 0x17 bytes: the comma-space code and the period and ellipsis quote codes become two-byte pairs, a
 * character-name code inserts that character's name from the savemap (up to nine bytes), and
 * every other byte is copied; the name ends with 0xFF. Returns 0 when the field has no text, else
 * 1.
 */
// FUNCTION: FF7 0x00633691
int __cdecl Field_BuildLocationName(short id)
{
    short characterIdx;
    unsigned char *namePointer;
    int unused;
    short outPos;
    short nameOfs;
    unsigned char *textPointer;

    unused = 0;
    if (g_FieldDialogTextPtr == 0) {
        return 0;
    }
    textPointer = (unsigned char *)g_FieldDialogTextPtr;
    textPointer += *((unsigned char *)g_FieldDialogTextPtr + id * 2 + 2);
    textPointer += *((unsigned char *)g_FieldDialogTextPtr + id * 2 + 3) << 8;
    nameOfs = 0;
    outPos = 0;
    while (outPos < 23) {
        switch (*textPointer) {
        case TEXT_END:
            goto done;
        case TEXT_COMMA_SPACE:
            textPointer++;
            gSavemap.location[outPos] = 0x0C;
            outPos++;
            gSavemap.location[outPos] = 0;
            outPos++;
            break;
        case TEXT_PERIOD_QUOTE:
            textPointer++;
            gSavemap.location[outPos] = 0x0E;
            outPos++;
            gSavemap.location[outPos] = 2;
            outPos++;
            break;
        case TEXT_ELLIPSIS_QUOTE:
            textPointer++;
            gSavemap.location[outPos] = (char)0xA9;
            outPos++;
            gSavemap.location[outPos] = 2;
            outPos++;
            break;
        case TEXT_NAME_CLOUD:
        case TEXT_NAME_BARRET:
        case TEXT_NAME_TIFA:
        case TEXT_NAME_AERIS:
        case TEXT_NAME_RED_XIII:
        case TEXT_NAME_YUFFIE:
        case TEXT_NAME_CAIT_SITH:
        case TEXT_NAME_VINCENT:
        case TEXT_NAME_CID:
            characterIdx = *textPointer - TEXT_NAME_CLOUD;
            namePointer = (unsigned char *)Savemap_GetCharacterName(characterIdx);
            if (namePointer[nameOfs] == TEXT_END || nameOfs >= 9) {
                textPointer++;
                nameOfs = 0;
            } else {
                gSavemap.location[outPos] = namePointer[nameOfs];
                nameOfs++;
                outPos++;
            }
            break;
        case TEXT_FONT_PAGE_1:
        case TEXT_FONT_PAGE_2:
        case TEXT_FONT_PAGE_3:
        case TEXT_FONT_PAGE_4:
        case TEXT_EXTENDED:
            gSavemap.location[outPos] = *textPointer;
            textPointer++;
            outPos++;
            break;
        default:
            gSavemap.location[outPos] = *textPointer;
            textPointer++;
            outPos++;
            break;
        }
    }
done:
    gSavemap.location[outPos] = (char)TEXT_END;
    return 1;
}

/*
 * For a nonnull dialogue base reads the indexed little-endian offset and copies bytes until 0xFF
 * into the character-name pointer. Writes a terminator only when copied count is below nine; copy
 * length itself is not bounded to nine.
 */
// FUNCTION: FF7 0x00633909
void __cdecl Field_SetCharacterNameFromDialogueString(short charIndex, short stringIndex)
{
    char *namePtr;
    short count;
    unsigned char *textPointer;

    if (g_FieldDialogTextPtr == 0) {
        return;
    }
    textPointer = (unsigned char *)g_FieldDialogTextPtr;
    textPointer += *((unsigned char *)g_FieldDialogTextPtr + stringIndex * 2 + 2);
    textPointer += *((unsigned char *)g_FieldDialogTextPtr + stringIndex * 2 + 3) << 8;
    count = 0;
    namePtr = (char *)Savemap_GetCharacterName(charIndex);
    while (*textPointer != TEXT_END) {
        *namePtr = *textPointer;
        namePtr++;
        textPointer++;
        count++;
    }
    if (count < 9) {
        *namePtr = TEXT_END;
    }
}
} /* extern "C" */
