// src/ff7/src/wm/wm_world_message_helpers.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Holds the internal helpers behind the world map's message windows: opening, text
//   reveal, scrolling, closing and the window table setup. It also formats numbers and decodes
//   message text, and contains placeholder functions that do nothing.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <string.h>
#include "ff7/world.h"
#include "ff7/types/wm.h"
#include "ff7/enums/field.h"
#include "ff7/enums/main.h"
#include "lib/enums/sound.h"
#include "ff7/api/field.h"
#include "ff7/field.h"
#include "ff7/api/wm.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/api/token.h"
#include "lib/codegen.h"
#include "lib/enums/file.h"
#include "lib/api/sound.h"
#include "ff7/savemap.h"
#include "ff7/types/field.h"
extern "C" {

#pragma intrinsic(strcpy, memcpy)
// clang-format off
// GLOBAL: FF7 0x0096e0b0
unsigned char g_WorldMsgDigitGlyphs[0x10] = {0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9};
// The index of each window's next operand slot in the two operand tables below.
// GLOBAL: FF7 0x00e3b130
short g_WorldMsgWindowOperandIndex[4] = {0};
// Each of the four windows' eight operand values, read through the kind in g_WorldMsgWindowOperandKinds.
// GLOBAL: FF7 0x00e3b138
short g_WorldMsgWindowOperandValues[4][8] = {0};
// The dialogue table of the field script: an offset word per message, then the messages.
// GLOBAL: FF7 0x00e3b178
unsigned char *g_WorldMsgDialogTable = 0;
unsigned char g_WorldMsgAlignPad[4] = {0};
// Each window's eight operand kinds (how WorldMsg_ReadScriptOperand reads the value).
// GLOBAL: FF7 0x00e3b180
unsigned char g_WorldMsgWindowOperandKinds[4][8] = {0};
// Each window's 16-byte number text, filled by the WorldMsg_Format* helpers.
// GLOBAL: FF7 0x00e3b1a0
unsigned char g_WorldMsgWindowNumberText[4][16] = {0};
// Each window's read position in its number text, or -1 when no number is being shown.
// GLOBAL: FF7 0x00e3b1e0
short g_WorldMsgWindowNumberPos[4] = {0};
// Each window's text reveal budget: the glyph count left to reveal this step.
// GLOBAL: FF7 0x00e3b1e8
short g_WorldMsgWindowRevealBudget[4] = {0};
// Each window's text reveal and scroll speed.
// GLOBAL: FF7 0x00e3b1f0
short g_WorldMsgWindowScrollX[4] = {0};
// Each window's read position in the character name being copied.
// GLOBAL: FF7 0x00e3b1f8
short g_WorldMsgWindowNamePos[4] = {0};
// Each window's page scroll limit.
// GLOBAL: FF7 0x00e3b200
short g_WorldMsgWindowCursorPos[4] = {0};
// Each window's frames left in a text wait.
// GLOBAL: FF7 0x00e3b208
short g_WorldMsgWindowCursorState[4] = {0};
// GLOBAL: FF7 0x00e3b210
unsigned char *g_WorldMsgWindowStringPointers[4] = {0};
// Each window's 256-byte text buffer.
// GLOBAL: FF7 0x00e3b220
unsigned char g_WorldMsgTextBuffer[4][256] = {0};
// The rows each window has scrolled so far.
// GLOBAL: FF7 0x00e3b620
short g_WorldMsgWindowScrolledRows[4] = {0};
// clang-format on

/*
 * Empty. A window hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x00768c70
void __cdecl World_UnusedNoOp_2(void)
{
    /* intentionally empty, as in the original */
}

/*
 * Empty. A window hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x00768c75
void __cdecl World_DebugLogNoOp(char *message)
{
    /* intentionally empty, as in the original */
}

/*
 * Initialises the world map's four text windows: clears g_FieldOpenWindowCount,
 * resets each window through WorldMsg_ResetWindow, and points
 * g_WorldMsgDialogTable at the field script's dialogue table (the one
 * g_FieldScripts->dialogTextOffset gives) or to 0 when there is none.
 */
// FUNCTION: FF7 0x00768c7a
void __cdecl WorldMsg_InitWindowsAndMessageTable(void)
{
    int i;

    g_FieldOpenWindowCount = 0;
    for (i = 0; i < 4; i++) {
        WorldMsg_ResetWindow(i);
    }
    if (g_FieldScripts->dialogTextOffset != 0) {
        g_WorldMsgDialogTable = (unsigned char *)g_FieldScripts + g_FieldScripts->dialogTextOffset;
    } else {
        g_WorldMsgDialogTable = 0;
    }
}

/*
 * Resets the indexed field-window geometry and state with X 8, width 304,
 * height 73 and Y 8 for window one or 0x95 otherwise. Clears its owner to
 * 0xFF, its eight operand kinds and values, and its wait frames.
 */
// FUNCTION: FF7 0x00768ce4
void __cdecl WorldMsg_ResetWindow(short windowIndex)
{
    int i;

    /* Numeric message window slot index. */
    switch (windowIndex) {
    case FIELD_WINDOW_SLOT_TOP:
        g_FieldWindows[windowIndex].y = 8;
        break;
    default:
        g_FieldWindows[windowIndex].y = 0x95;
    }
    g_FieldWindows[windowIndex].state = MSG_WINDOW_CLOSED;
    g_FieldWindows[windowIndex].style = 0;
    g_FieldWindows[windowIndex].x = 8;
    g_FieldWindows[windowIndex].width = 304;
    g_FieldWindows[windowIndex].height = 73;
    g_FieldWindows[windowIndex].currentWidth = 1;
    g_FieldWindows[windowIndex].currentHeight = 1;
    g_FieldWindows[windowIndex].numDisplayType = 0;
    g_FieldWindows[windowIndex].numDisplayFlags = 0;
    g_FieldWindows[windowIndex].numDisplayLength = 6;
    g_FieldWindows[windowIndex].numDisplayX = 0;
    g_FieldWindows[windowIndex].numDisplayY = 0;
    g_FieldWindows[windowIndex].preventClose = 0;

    g_FieldWindowOwner[windowIndex] = 0xff;
    for (i = 0; i < 8; i++) {
        g_WorldMsgWindowOperandKinds[windowIndex][i] = FIELD_BANK_LITERAL;
        g_WorldMsgWindowOperandValues[windowIndex][i] = 0;
    }
    g_WorldMsgWindowCursorState[windowIndex] = 0;
}

/*
 * Asks window windowIndex to close by its state: state 1 (opening) refuses with 0;
 * states 2, 4, 6, 8, 0xB and 0xD (the open and waiting states) become 7
 * (closing) and return 1; anything else returns 1 without a change.
 */
// FUNCTION: FF7 0x00768e3d
int __cdecl WorldMsg_RequestWindowClose(short windowIndex)
{
    switch (g_FieldWindows[windowIndex].state) {
    case MSG_WINDOW_OPENING:
        return 0;
    case MSG_WINDOW_TEXT_REVEAL:
    case MSG_WINDOW_WAIT_INPUT_SCROLL_ROW:
    case MSG_WINDOW_TEXT_DONE:
    case MSG_WINDOW_ROW_SCROLL:
    case MSG_WINDOW_WAIT_INPUT_SCROLL_PAGE:
    case MSG_WINDOW_WAIT_INPUT_RESUME:
        g_FieldWindows[windowIndex].state = MSG_WINDOW_CLOSING;
        return 1;
    default:
        return 1;
    }
}

/*
 * Sets a window's style and preventClose fields.
 */
// FUNCTION: FF7 0x00768ec3
void __cdecl WorldMsg_SetWindowStyleAndPreventClose(short index, short style, short preventClose)
{
    g_FieldWindows[index].style = style;
    g_FieldWindows[index].preventClose = preventClose;
}

/*
 * Clears all four windows: state and stringLength to 0, owner to 0xFF,
 * g_WorldMsgWindowCursorState to 0, and g_FieldOpenWindowCount to 0.
 */
// FUNCTION: FF7 0x00768eea
void __cdecl WorldMsg_ClearAllWindows(void)
{
    int i;

    for (i = 0; i < 4; i++) {
        g_FieldWindows[i].state = MSG_WINDOW_CLOSED;
        g_FieldWindows[i].stringLength = 0;
        g_FieldWindowOwner[i] = 0xFF;
        g_WorldMsgWindowCursorState[i] = 0;
    }
    g_FieldOpenWindowCount = 0;
}

/*
 * Stores x/y as the indexed field window position and width/height as its
 * size. First raises each position below 8 to 8, then adjusts it down when the
 * far edge exceeds 0x138 horizontally or 0xE0 vertically.
 */
// FUNCTION: FF7 0x00768f48
void __cdecl WorldMsg_SetWindowRectClamped(short windowIndex, short x, short y, short width, short height)
{
    if (x < 8) {
        x = 8;
    }
    if (x + width > 0x138) {
        x = 0x138 - width;
    }
    if (y < 8) {
        y = 8;
    }
    if (y + height > 0xE0) {
        y = 0xE0 - height;
    }
    g_FieldWindows[windowIndex].x = x;
    g_FieldWindows[windowIndex].y = y;
    g_FieldWindows[windowIndex].width = width;
    g_FieldWindows[windowIndex].height = height;
}

/*
 * Moves window windowIndex by (dx, dy).
 */
// FUNCTION: FF7 0x00768ff4
void __cdecl WorldMsg_MoveWindow(short windowIndex, int dx, int dy)
{
    g_FieldWindows[windowIndex].x += dx;
    g_FieldWindows[windowIndex].y += dy;
}

/*
 * Sets a window's height.
 */
// FUNCTION: FF7 0x00769039
void __cdecl WorldMsg_SetWindowHeight(short index, short value)
{
    g_FieldWindows[index].height = value;
}

/*
 * Dispatches the selected field window state to its opening, text, scrolling
 * or closing helper. State 3 counts down before returning to state 2.
 */
// FUNCTION: FF7 0x00769050
int __cdecl WorldMsg_DispatchWindowState(unsigned char index, unsigned char mode)
{
    switch (g_FieldWindows[index].state) {
    case MSG_WINDOW_CLOSED:
        if (WorldMsg_BeginOpenWindow(index, mode)) {
            return 1;
        }
        break;
    case MSG_WINDOW_OPENING:
        WorldMsg_StepWindowOpenGrow(index);
        break;
    case MSG_WINDOW_TEXT_REVEAL:
        WorldMsg_StepWindowTextReveal(index);
        break;
    case MSG_WINDOW_ROW_SCROLL:
        WorldMsg_StepWindowRowScroll(index);
        break;
    case MSG_WINDOW_PAGE_SCROLL_OUT:
        WorldMsg_StepWindowPageScroll(index);
        break;
    case MSG_WINDOW_WAIT_INPUT_RESUME:
        if ((*(int *)&g_pFieldState->newActiveKeys2) & INPUT_CIRCLE) {
            g_FieldWindows[index].state = MSG_WINDOW_TEXT_REVEAL;
        }
        break;
    case MSG_WINDOW_WAIT_DELAY:
        if (g_WorldMsgWindowCursorState[index] == 0) {
            g_FieldWindows[index].state = MSG_WINDOW_TEXT_REVEAL;
        } else {
            g_WorldMsgWindowCursorState[index]--;
        }
        break;
    case MSG_WINDOW_WAIT_INPUT_SCROLL_ROW:
        if ((*(int *)&g_pFieldState->newActiveKeys2) & INPUT_CIRCLE) {
            if (g_FieldWindows[index].currentRow == (g_FieldWindows[index].height - 9) / 16 + g_WorldMsgWindowScrolledRows[index] - 1) {
                g_FieldWindows[index].state = MSG_WINDOW_ROW_SCROLL;
                g_FieldWindows[index].textScrolling -= 2;
                g_WorldMsgWindowScrolledRows[index]++;
            }
        }
        break;
    case MSG_WINDOW_TEXT_DONE:
        if (*(short *)&g_FieldWindows[index].preventClose & MSG_WINDOW_MODE_NO_PLAYER_CLOSE) { /* empty first arm: lib/codegen.h */
        } else if ((*(int *)&g_pFieldState->newActiveKeys2) & INPUT_CIRCLE) {
            g_FieldWindows[index].state = MSG_WINDOW_CLOSING;
            WorldMsg_StepWindowClose(index);
        }
        break;
    case MSG_WINDOW_WAIT_INPUT_CLEAR_PAGE:
        if ((*(int *)&g_pFieldState->newActiveKeys2) & INPUT_CIRCLE) {
            WorldMsg_ResetWindowTextRun(index);
        }
        break;
    case MSG_WINDOW_WAIT_INPUT_SCROLL_PAGE:
        if ((*(int *)&g_pFieldState->newActiveKeys2) & INPUT_CIRCLE) {
            g_FieldWindows[index].state = MSG_WINDOW_PAGE_SCROLL_OUT;
            g_WorldMsgWindowCursorPos[index] = g_FieldWindows[index].currentRow * 16 + 0x11;
            g_FieldWindows[index].textScrolling -= 2;
        }
        break;
    case MSG_WINDOW_PAGE_CLEAR:
        WorldMsg_ResetWindowTextRun(index);
        break;
    case MSG_WINDOW_CLOSING_ALT:
    case MSG_WINDOW_CLOSING:
        if (WorldMsg_StepWindowClose(index)) {
            return 1;
        }
        break;
    }
    return 0;
}

/*
 * Dispatches the field window state to opening, text, scrolling and closing
 * helpers, handling input pauses and row-scroll transitions. In state 6,
 * unless closing is prevented, enables the choice pointer, adjusts choice from
 * up and down held bits, clamps it to first through last and places the
 * pointer at X 5 and Y choice times 16 plus 6.
 */
// FUNCTION: FF7 0x007693a1
int __cdecl WorldMsg_UpdateChoiceWindow(unsigned char index, unsigned char mode, unsigned char first, unsigned char last, short *choice)
{
    switch (g_FieldWindows[index].state) {
    case MSG_WINDOW_CLOSED:
        if (WorldMsg_BeginOpenWindow(index, mode)) {
            return 1;
        }
        break;
    case MSG_WINDOW_OPENING:
        WorldMsg_StepWindowOpenGrow(index);
        break;
    case MSG_WINDOW_TEXT_REVEAL:
        WorldMsg_StepWindowTextReveal(index);
        break;
    case MSG_WINDOW_ROW_SCROLL:
        WorldMsg_StepWindowRowScroll(index);
        break;
    case MSG_WINDOW_PAGE_SCROLL_OUT:
        WorldMsg_StepWindowPageScroll(index);
        break;
    case MSG_WINDOW_WAIT_INPUT_RESUME:
        if ((*(int *)&g_pFieldState->newActiveKeys2) & INPUT_CIRCLE) {
            g_FieldWindows[index].state = MSG_WINDOW_TEXT_REVEAL;
        }
        break;
    case MSG_WINDOW_WAIT_DELAY:
        if (g_WorldMsgWindowCursorState[index] == 0) {
            g_FieldWindows[index].state = MSG_WINDOW_TEXT_REVEAL;
        } else {
            g_WorldMsgWindowCursorState[index]--;
        }
        break;
    case MSG_WINDOW_WAIT_INPUT_SCROLL_ROW:
        if ((*(int *)&g_pFieldState->newActiveKeys2) & INPUT_CIRCLE) {
            if (g_FieldWindows[index].currentRow == (g_FieldWindows[index].height - 9) / 16 + g_WorldMsgWindowScrolledRows[index] - 1) {
                g_FieldWindows[index].state = MSG_WINDOW_ROW_SCROLL;
                g_FieldWindows[index].textScrolling -= 2;
                g_WorldMsgWindowScrolledRows[index]++;
            }
        }
        break;
    case MSG_WINDOW_TEXT_DONE:
        if (*(short *)&g_FieldWindows[index].preventClose & MSG_WINDOW_MODE_NO_PLAYER_CLOSE) { /* empty first arm: lib/codegen.h */
        } else {
            g_FieldWindows[index].pointerEnabled = 1;
            if ((*(int *)&g_pFieldState->newActiveKeys) & INPUT_UP) {
                if (*choice > first)
                    WorldMsg_PlayCursorSound();
                (*choice)--;
            }
            if ((*(int *)&g_pFieldState->newActiveKeys) & INPUT_DOWN) {
                if (*choice < last)
                    WorldMsg_PlayCursorSound();
                (*choice)++;
            }
            if (*choice < first)
                *choice = first;
            if (*choice > last)
                *choice = last;
            g_FieldWindows[index].pointerX = 5;
            g_FieldWindows[index].pointerY = *choice * 16 + 6;
            if ((*(int *)&g_pFieldState->newActiveKeys2) & INPUT_CIRCLE) {
                WorldMsg_PlayCursorSound();
                g_FieldWindows[index].state = MSG_WINDOW_CLOSING;
                WorldMsg_StepWindowClose(index);
            }
        }
        break;
    case MSG_WINDOW_WAIT_INPUT_CLEAR_PAGE:
        if ((*(int *)&g_pFieldState->newActiveKeys2) & INPUT_CIRCLE) {
            WorldMsg_ResetWindowTextRun(index);
        }
        break;
    case MSG_WINDOW_WAIT_INPUT_SCROLL_PAGE:
        if ((*(int *)&g_pFieldState->newActiveKeys2) & INPUT_CIRCLE) {
            g_FieldWindows[index].state = MSG_WINDOW_PAGE_SCROLL_OUT;
            g_WorldMsgWindowCursorPos[index] = g_FieldWindows[index].currentRow * 16 + 0x11;
            g_FieldWindows[index].textScrolling -= 2;
        }
        break;
    case MSG_WINDOW_PAGE_CLEAR:
        WorldMsg_ResetWindowTextRun(index);
        break;
    case MSG_WINDOW_CLOSING_ALT:
    case MSG_WINDOW_CLOSING:
        if (WorldMsg_StepWindowClose(index)) {
            g_FieldWindows[index].pointerEnabled = 0;
            return 1;
        }
        g_FieldWindows[index].pointerEnabled ^= 1;
        break;
    }
    return 0;
}

/*
 * Plays the cursor-move sound effect.
 */
// FUNCTION: FF7 0x00769827
void __cdecl WorldMsg_PlayCursorSound(void)
{
    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
}

/*
 * Returns one after reporting absent message data, or zero without changing an
 * already-owned dialog. Otherwise assigns the current entity, initializes
 * quarter-size dimensions with minimum eight, clears text state and buffer,
 * resolves the selected message offset, initializes dialog controls, sets
 * window state one and increments the open-dialog count.
 */
// FUNCTION: FF7 0x00769836
int __cdecl WorldMsg_BeginOpenWindow(short wDialogId, short wScriptId)
{
    if (g_WorldMsgDialogTable == 0) {
        // STRING: FF7 0x0096e0c0
        World_DebugLogNoOp("No mes data!");
        return 1;
    }
    if (g_FieldWindowOwner[wDialogId] != 0xFF) {
        return 0;
    }
    g_FieldWindowOwner[wDialogId] = g_CurrentEntity;
    g_FieldWindows[wDialogId].currentWidth =
        g_FieldWindows[wDialogId].width / 4;
    g_FieldWindows[wDialogId].currentHeight =
        g_FieldWindows[wDialogId].height / 4;
    if (g_FieldWindows[wDialogId].currentHeight < 8) {
        g_FieldWindows[wDialogId].currentHeight = 8;
    }
    if (g_FieldWindows[wDialogId].currentWidth < 8) {
        g_FieldWindows[wDialogId].currentWidth = 8;
    }
    g_FieldWindows[wDialogId].textScrolling = 0;
    g_FieldWindows[wDialogId].stringLength = 0;
    g_FieldWindows[wDialogId].stringByteLength = 0;
    g_FieldWindows[wDialogId].currentRow = 0;
    g_FieldWindows[wDialogId].pointerEnabled = 0;
    g_FieldWindows[wDialogId].text = g_WorldMsgTextBuffer[wDialogId];
    g_WorldMsgTextBuffer[wDialogId][0] = TEXT_END;
    g_WorldMsgWindowStringPointers[wDialogId] = g_WorldMsgDialogTable;
    g_WorldMsgWindowStringPointers[wDialogId] +=
        g_WorldMsgDialogTable[wScriptId * 2 + 2];
    g_WorldMsgWindowStringPointers[wDialogId] +=
        g_WorldMsgDialogTable[wScriptId * 2 + 3] << 8;
    g_WorldMsgWindowScrollX[wDialogId] = 1;
    g_WorldMsgWindowRevealBudget[wDialogId] = 0;
    g_WorldMsgWindowScrolledRows[wDialogId] = 0;
    g_WorldMsgWindowNamePos[wDialogId] = 0;
    g_WorldMsgWindowOperandIndex[wDialogId] = 0;
    g_WorldMsgWindowNumberPos[wDialogId] = -1;
    g_FieldWindows[wDialogId].state = MSG_WINDOW_OPENING;
    g_FieldOpenWindowCount++;
    return 0;
}

/*
 * For a dialog owned by the current entity, grows current width/height by one
 * quarter of their target dimensions, clamps each to at least eight and at
 * most its target, and sets state two when both reach the targets. Other
 * owners leave the window unchanged.
 */
// FUNCTION: FF7 0x00769a66
void __cdecl WorldMsg_StepWindowOpenGrow(short wDialogId)
{
    if (g_FieldWindowOwner[wDialogId] != g_CurrentEntity) {
        return;
    }
    g_FieldWindows[wDialogId].currentWidth +=
        g_FieldWindows[wDialogId].width / 4;
    if (g_FieldWindows[wDialogId].currentWidth < 8) {
        g_FieldWindows[wDialogId].currentWidth = 8;
    }
    if (g_FieldWindows[wDialogId].currentWidth >
        g_FieldWindows[wDialogId].width) {
        g_FieldWindows[wDialogId].currentWidth =
            g_FieldWindows[wDialogId].width;
    }
    g_FieldWindows[wDialogId].currentHeight +=
        g_FieldWindows[wDialogId].height / 4;
    if (g_FieldWindows[wDialogId].currentHeight < 8) {
        g_FieldWindows[wDialogId].currentHeight = 8;
    }
    if (g_FieldWindows[wDialogId].currentHeight >
        g_FieldWindows[wDialogId].height) {
        g_FieldWindows[wDialogId].currentHeight =
            g_FieldWindows[wDialogId].height;
    }
    if (g_FieldWindows[wDialogId].currentWidth ==
            g_FieldWindows[wDialogId].width &&
        g_FieldWindows[wDialogId].currentHeight ==
            g_FieldWindows[wDialogId].height) {
        g_FieldWindows[wDialogId].state = MSG_WINDOW_TEXT_REVEAL;
    }
}

/*
 * Advances the current entity-owned world text window according to configured
 * reveal speed and held-input acceleration. Appends ordinary or two-byte
 * glyphs, character/party-name substitutions and formatted variable or save-
 * bank substitutions to the window buffer.
 */
// FUNCTION: FF7 0x00769c02
void __cdecl WorldMsg_StepWindowTextReveal(short id)
{
    struct {
        unsigned short count, pad18;
        short i, pad14, rate, pad10, step, pad0c;
        unsigned char *name;
        unsigned short value, pad04;
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (g_FieldWindowOwner[id] != g_CurrentEntity)
        return;
    if ((short)g_FieldWindows[id].preventClose & MSG_WINDOW_MODE_INSTANT_TEXT) {
        s.rate = 256;
        s.step = 1;
    } else {
        if ((*(int *)&g_pFieldState->activeKeys2) & INPUT_CIRCLE) {
            g_WorldMsgWindowScrollX[id]++;
            if (g_WorldMsgWindowScrollX[id] > 128)
                g_WorldMsgWindowScrollX[id] = 128;
        } else {
            g_WorldMsgWindowScrollX[id]--;
            if (g_WorldMsgWindowScrollX[id] <= 1)
                g_WorldMsgWindowScrollX[id] = 1;
        }
        if (gSavemap.fieldMessageSpeed < 128) {
            s.rate = ((128 - gSavemap.fieldMessageSpeed) >> 5) + 2;
            s.step = 1;
        } else {
            s.rate = 2;
            s.step = ((gSavemap.fieldMessageSpeed - 128) >> 5) + 1;
        }
    }
    g_WorldMsgWindowRevealBudget[id] += s.rate + (g_WorldMsgWindowScrollX[id] >> 4) * s.step;
    while (g_WorldMsgWindowRevealBudget[id] > s.step) {
        switch (*g_WorldMsgWindowStringPointers[id]) {
        case TEXT_END:
            g_FieldWindows[id].state = MSG_WINDOW_TEXT_DONE;
            g_WorldMsgWindowRevealBudget[id] = 0;
            goto done;
        case TEXT_NEWLINE:
            if (g_FieldWindows[id].currentRow == (g_FieldWindows[id].height - 9) / 16 + g_WorldMsgWindowScrolledRows[id] - 1) {
                g_FieldWindows[id].state = MSG_WINDOW_WAIT_INPUT_SCROLL_ROW;
                g_WorldMsgWindowScrollX[id] = 1;
                g_WorldMsgWindowRevealBudget[id] = 0;
                goto done;
            }
            g_WorldMsgTextBuffer[id][g_FieldWindows[id].stringByteLength] = *g_WorldMsgWindowStringPointers[id];
            *g_WorldMsgWindowStringPointers[id]++;
            g_FieldWindows[id].stringByteLength++;
            g_FieldWindows[id].currentRow++;
            break;
        case TEXT_NEW_PAGE:
        case TEXT_NEW_PAGE_ALT:
            g_WorldMsgWindowStringPointers[id]++;
            g_FieldWindows[id].state = MSG_WINDOW_WAIT_INPUT_CLEAR_PAGE;
            g_WorldMsgWindowScrollX[id] = 1;
            g_WorldMsgWindowRevealBudget[id] = 0;
            goto done;
        case TEXT_NAME_CLOUD:
        case TEXT_NAME_BARRET:
        case TEXT_NAME_TIFA:
        case TEXT_NAME_AERIS:
        case TEXT_NAME_RED_XIII:
        case TEXT_NAME_YUFFIE:
        case TEXT_NAME_CAIT_SITH:
        case TEXT_NAME_VINCENT:
        case TEXT_NAME_CID:
            s.value = *g_WorldMsgWindowStringPointers[id] - TEXT_NAME_CLOUD;
            s.name = (unsigned char *)Savemap_GetCharacterName(s.value);
            if (s.name[g_WorldMsgWindowNamePos[id]] == TEXT_END || g_WorldMsgWindowNamePos[id] >= 9) {
                g_WorldMsgWindowStringPointers[id]++;
                g_WorldMsgWindowNamePos[id] = 0;
            } else {
                g_WorldMsgTextBuffer[id][g_FieldWindows[id].stringByteLength] = s.name[g_WorldMsgWindowNamePos[id]];
                g_FieldWindows[id].stringByteLength++;
                g_WorldMsgWindowNamePos[id]++;
                g_FieldWindows[id].stringLength++;
                g_WorldMsgWindowRevealBudget[id] -= s.step;
            }
            break;
        case TEXT_PARTY_1:
        case TEXT_PARTY_2:
        case TEXT_PARTY_3:
            /* the party member's id: fieldParty[code - TEXT_PARTY_1], the subtraction folded into the address */
            s.value = (&gSavemap.fieldParty[-TEXT_PARTY_1])[*g_WorldMsgWindowStringPointers[id]];
            if (s.value == 255) {
                if (g_WorldMsgWindowNamePos[id] >= 9) {
                    g_WorldMsgWindowStringPointers[id]++;
                    g_WorldMsgWindowNamePos[id] = 0;
                } else {
                    g_WorldMsgTextBuffer[id][g_FieldWindows[id].stringByteLength] = 0xd2;
                    g_FieldWindows[id].stringByteLength++;
                    g_WorldMsgWindowNamePos[id]++;
                    g_FieldWindows[id].stringLength++;
                    g_WorldMsgWindowRevealBudget[id] -= s.step;
                }
            } else {
                s.name = (unsigned char *)Savemap_GetCharacterName(s.value);
                if (s.name[g_WorldMsgWindowNamePos[id]] == TEXT_END || g_WorldMsgWindowNamePos[id] >= 9) {
                    g_WorldMsgWindowStringPointers[id]++;
                    g_WorldMsgWindowNamePos[id] = 0;
                } else {
                    g_WorldMsgTextBuffer[id][g_FieldWindows[id].stringByteLength] = s.name[g_WorldMsgWindowNamePos[id]];
                    g_FieldWindows[id].stringByteLength++;
                    g_WorldMsgWindowNamePos[id]++;
                    g_FieldWindows[id].stringLength++;
                    g_WorldMsgWindowRevealBudget[id] -= s.step;
                }
            }
            break;
        case TEXT_EXTENDED:
            g_WorldMsgTextBuffer[id][g_FieldWindows[id].stringByteLength] = *g_WorldMsgWindowStringPointers[id];
            g_WorldMsgWindowStringPointers[id]++;
            g_FieldWindows[id].stringByteLength++;
            switch (*g_WorldMsgWindowStringPointers[id]) {
            case TEXT_FE_PAUSE:
                g_FieldWindows[id].stringByteLength--;
                g_WorldMsgWindowStringPointers[id]++;
                g_FieldWindows[id].state = MSG_WINDOW_WAIT_INPUT_RESUME;
                g_WorldMsgWindowScrollX[id] = 1;
                g_WorldMsgWindowRevealBudget[id] = 0;
                goto done;
            case TEXT_FE_WAIT_SCROLL_PAGE:
                g_FieldWindows[id].stringByteLength--;
                g_WorldMsgWindowStringPointers[id]++;
                g_FieldWindows[id].state = MSG_WINDOW_WAIT_INPUT_SCROLL_PAGE;
                g_WorldMsgWindowScrollX[id] = 1;
                g_WorldMsgWindowRevealBudget[id] = 0;
                goto done;
            case TEXT_FE_NUM_DECIMAL:
            case TEXT_FE_NUM_HEX:
            case TEXT_FE_NUM_DECIMAL_PADDED:
                g_FieldWindows[id].stringByteLength--;
                g_WorldMsgWindowStringPointers[id]--;
                if (g_WorldMsgWindowNumberPos[id] == -1) {
                    s.value = WorldMsg_ReadScriptOperand(id);
                    switch (g_WorldMsgWindowStringPointers[id][1]) {
                    case TEXT_FE_NUM_DECIMAL:
                        WorldMsg_FormatDecimalNoPad(s.value, g_WorldMsgWindowNumberText[id]);
                        break;
                    case TEXT_FE_NUM_DECIMAL_PADDED:
                        WorldMsg_FormatDecimalPadded5(s.value, g_WorldMsgWindowNumberText[id]);
                        break;
                    case TEXT_FE_NUM_HEX:
                        WorldMsg_FormatHexNoPad(s.value, g_WorldMsgWindowNumberText[id]);
                        break;
                    }
                    g_WorldMsgWindowNumberPos[id]++;
                } else if (g_WorldMsgWindowNumberText[id][g_WorldMsgWindowNumberPos[id]] == TEXT_END || g_WorldMsgWindowNumberPos[id] >= 16) {
                    g_WorldMsgWindowStringPointers[id] += 2;
                    g_WorldMsgWindowNumberPos[id] = -1;
                    g_WorldMsgWindowOperandIndex[id]++;
                } else {
                    g_WorldMsgTextBuffer[id][g_FieldWindows[id].stringByteLength] = g_WorldMsgWindowNumberText[id][g_WorldMsgWindowNumberPos[id]];
                    g_FieldWindows[id].stringByteLength++;
                    g_WorldMsgWindowNumberPos[id]++;
                    g_FieldWindows[id].stringLength++;
                    g_WorldMsgWindowRevealBudget[id] -= s.step;
                }
                break;
            case TEXT_FE_BANK_STRING:
                g_FieldWindows[id].stringByteLength--;
                g_WorldMsgWindowStringPointers[id]--;
                if (g_WorldMsgWindowNumberPos[id] == -1) {
                    s.value = g_WorldMsgWindowStringPointers[id][2];
                    s.value |= g_WorldMsgWindowStringPointers[id][3] << 8;
                    s.count = g_WorldMsgWindowStringPointers[id][4];
                    s.count |= g_WorldMsgWindowStringPointers[id][5] << 8;
                    for (s.i = 0; s.i < s.count; s.i++)
                        g_WorldMsgWindowNumberText[id][s.i] = gSavemap.scriptBanks[s.value + s.i];
                    g_WorldMsgWindowNumberText[id][s.i] = TEXT_END;
                    g_WorldMsgWindowNumberPos[id]++;
                } else if (g_WorldMsgWindowNumberText[id][g_WorldMsgWindowNumberPos[id]] == TEXT_END) {
                    g_WorldMsgWindowStringPointers[id] += 6;
                    g_WorldMsgWindowNumberPos[id] = -1;
                } else {
                    g_WorldMsgTextBuffer[id][g_FieldWindows[id].stringByteLength] = g_WorldMsgWindowNumberText[id][g_WorldMsgWindowNumberPos[id]];
                    g_FieldWindows[id].stringByteLength++;
                    g_WorldMsgWindowNumberPos[id]++;
                    g_FieldWindows[id].stringLength++;
                    g_WorldMsgWindowRevealBudget[id] -= s.step;
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
                g_WorldMsgTextBuffer[id][g_FieldWindows[id].stringByteLength] = *g_WorldMsgWindowStringPointers[id];
                g_WorldMsgWindowStringPointers[id]++;
                g_FieldWindows[id].stringByteLength++;
                break;
            case TEXT_FE_WAIT:
                g_FieldWindows[id].state = MSG_WINDOW_WAIT_DELAY;
                g_WorldMsgWindowStringPointers[id]++;
                g_FieldWindows[id].stringByteLength++;
                g_WorldMsgWindowCursorState[id] = (short)*g_WorldMsgWindowStringPointers[id];
                g_WorldMsgWindowStringPointers[id]++;
                g_FieldWindows[id].stringByteLength++;
                g_WorldMsgWindowCursorState[id] |= *g_WorldMsgWindowStringPointers[id] << 8;
                g_WorldMsgWindowStringPointers[id]++;
                g_FieldWindows[id].stringByteLength++;
                goto done;
            default:
                g_WorldMsgTextBuffer[id][g_FieldWindows[id].stringByteLength] = *g_WorldMsgWindowStringPointers[id];
                g_WorldMsgWindowStringPointers[id]++;
                g_FieldWindows[id].stringByteLength++;
                g_FieldWindows[id].stringLength++;
                g_WorldMsgWindowRevealBudget[id] -= s.step;
            }
            break;
        case TEXT_FONT_PAGE_1:
        case TEXT_FONT_PAGE_2:
        case TEXT_FONT_PAGE_3:
        case TEXT_FONT_PAGE_4:
            g_WorldMsgTextBuffer[id][g_FieldWindows[id].stringByteLength] = *g_WorldMsgWindowStringPointers[id];
            g_WorldMsgWindowStringPointers[id]++;
            g_FieldWindows[id].stringByteLength++;
        default:
            g_WorldMsgTextBuffer[id][g_FieldWindows[id].stringByteLength] = *g_WorldMsgWindowStringPointers[id];
            g_WorldMsgWindowStringPointers[id]++;
            g_FieldWindows[id].stringByteLength++;
            g_FieldWindows[id].stringLength++;
            g_WorldMsgWindowRevealBudget[id] -= s.step;
        }
    }
done:
    g_WorldMsgTextBuffer[id][g_FieldWindows[id].stringByteLength] = TEXT_END;
    return;
}

/*
 * Advances the text of window windowIndex for the entity that owns it: while the low
 * nibble of textScrolling is nonzero it decrements it by 2, otherwise it sets
 * the window state to MSG_WINDOW_TEXT_REVEAL. Does nothing when the window
 * belongs to a different entity.
 */
// FUNCTION: FF7 0x0076abe9
void __cdecl WorldMsg_StepWindowRowScroll(short windowIndex)
{
    if (g_FieldWindowOwner[windowIndex] != g_CurrentEntity) {
        return;
    }
    if ((g_FieldWindows[windowIndex].textScrolling & 0xF) != 0) {
        g_FieldWindows[windowIndex].textScrolling -= 2;
    } else {
        g_FieldWindows[windowIndex].state = MSG_WINDOW_TEXT_REVEAL;
    }
}

/*
 * For a window owned by the current entity, decreases its text offset while
 * offset plus the indexed limit is positive. Adjusts the indexed speed toward
 * 0x80 while input bit 0x20 is held or toward one otherwise.
 */
// FUNCTION: FF7 0x0076ac4f
void __cdecl WorldMsg_StepWindowPageScroll(short windowIndex)
{
    if (g_FieldWindowOwner[windowIndex] != g_CurrentEntity) {
        return;
    }
    if (g_FieldWindows[windowIndex].textScrolling + g_WorldMsgWindowCursorPos[windowIndex] > 0) {
        g_FieldWindows[windowIndex].textScrolling -= g_WorldMsgWindowScrollX[windowIndex] >> 2;
        if ((*(int *)&g_pFieldState->activeKeys2) & INPUT_CIRCLE) {
            g_WorldMsgWindowScrollX[windowIndex]++;
            if (g_WorldMsgWindowScrollX[windowIndex] > 0x80) {
                g_WorldMsgWindowScrollX[windowIndex] = 0x80;
            }
        } else {
            g_WorldMsgWindowScrollX[windowIndex]--;
            if (g_WorldMsgWindowScrollX[windowIndex] <= 1) {
                g_WorldMsgWindowScrollX[windowIndex] = 1;
            }
        }
    } else {
        g_FieldWindows[windowIndex].state = MSG_WINDOW_PAGE_CLEAR;
    }
}

// Resets the text run of window windowIndex, but only for a window owned by g_CurrentEntity;
// otherwise it returns at once. It zeroes stringByteLength, stringLength, textScrolling and
// currentRow, and sets the state to MSG_WINDOW_TEXT_REVEAL. It terminates the window's text buffer
// with TEXT_END, clears g_WorldMsgWindowScrolledRows[windowIndex], and sets
// g_WorldMsgWindowScrollX[windowIndex] to 1.
// FUNCTION: FF7 0x0076ad5e
void __cdecl WorldMsg_ResetWindowTextRun(short windowIndex)
{
    if (g_FieldWindowOwner[windowIndex] != g_CurrentEntity) {
        return;
    }
    g_FieldWindows[windowIndex].stringByteLength = 0;
    g_FieldWindows[windowIndex].stringLength = 0;
    g_FieldWindows[windowIndex].textScrolling = 0;
    g_FieldWindows[windowIndex].currentRow = 0;
    g_FieldWindows[windowIndex].state = MSG_WINDOW_TEXT_REVEAL;
    g_WorldMsgTextBuffer[windowIndex][0] = TEXT_END;
    g_WorldMsgWindowScrolledRows[windowIndex] = 0;
    g_WorldMsgWindowScrollX[windowIndex] = 1;
}

/*
 * Returns one immediately for a dialog not owned by the current entity.
 * Otherwise shrinks each current dimension by one quarter of its target unless
 * already below eight, in which case it sets eight.
 */
// FUNCTION: FF7 0x0076adf7
int __cdecl WorldMsg_StepWindowClose(short windowIndex)
{
    if (g_FieldWindowOwner[windowIndex] != g_CurrentEntity) {
        return 1;
    }
    if (g_FieldWindows[windowIndex].currentWidth < 8) {
        g_FieldWindows[windowIndex].currentWidth = 8;
    } else {
        g_FieldWindows[windowIndex].currentWidth -=
            g_FieldWindows[windowIndex].width / 4;
    }
    if (g_FieldWindows[windowIndex].currentHeight < 8) {
        g_FieldWindows[windowIndex].currentHeight = 8;
    } else {
        g_FieldWindows[windowIndex].currentHeight -=
            g_FieldWindows[windowIndex].height / 4;
    }
    if (g_FieldWindows[windowIndex].currentWidth <= 8 &&
        g_FieldWindows[windowIndex].currentHeight <= 8) {
        g_FieldWindows[windowIndex].stringLength = 0;
        g_FieldWindows[windowIndex].state = MSG_WINDOW_CLOSED;
        g_FieldWindowOwner[windowIndex] = 0xFF;
        g_FieldOpenWindowCount--;
        return 1;
    }
    return 0;
}

/*
 * Reads a scripted operand selected by windowIndex and its current operand index. The operand's
 * FieldScriptBank says where it lives: FIELD_BANK_LITERAL returns the stored short itself, the
 * savemap and temp banks read the byte or little-endian word at the stored offset, and any other
 * bank reads 0.
 */
// FUNCTION: FF7 0x0076af3d
short __cdecl WorldMsg_ReadScriptOperand(short windowIndex)
{
    unsigned short value;
    unsigned short address;

    switch (g_WorldMsgWindowOperandKinds[windowIndex][g_WorldMsgWindowOperandIndex[windowIndex]]) {
    case FIELD_BANK_LITERAL:
        value = g_WorldMsgWindowOperandValues[windowIndex][g_WorldMsgWindowOperandIndex[windowIndex]];
        break;
    case FIELD_BANK_SAVE_000_BYTE:
        address = g_WorldMsgWindowOperandValues[windowIndex][g_WorldMsgWindowOperandIndex[windowIndex]];
        value = gSavemap.scriptBanks[address];
        break;
    case FIELD_BANK_SAVE_000_WORD:
        address = g_WorldMsgWindowOperandValues[windowIndex][g_WorldMsgWindowOperandIndex[windowIndex]];
        value = gSavemap.scriptBanks[address];
        value |= (short)(g_SavemapScriptBanksHi[address] << 8);
        break;
    case FIELD_BANK_SAVE_100_BYTE:
        address = g_WorldMsgWindowOperandValues[windowIndex][g_WorldMsgWindowOperandIndex[windowIndex]] + 0x100;
        value = gSavemap.scriptBanks[address];
        break;
    case FIELD_BANK_SAVE_100_WORD:
        address = g_WorldMsgWindowOperandValues[windowIndex][g_WorldMsgWindowOperandIndex[windowIndex]] + 0x100;
        value = gSavemap.scriptBanks[address];
        value |= (short)(g_SavemapScriptBanksHi[address] << 8);
        break;
    case FIELD_BANK_SAVE_200_BYTE:
        address = g_WorldMsgWindowOperandValues[windowIndex][g_WorldMsgWindowOperandIndex[windowIndex]] + 0x200;
        value = gSavemap.scriptBanks[address];
        break;
    case FIELD_BANK_SAVE_200_WORD:
        address = g_WorldMsgWindowOperandValues[windowIndex][g_WorldMsgWindowOperandIndex[windowIndex]] + 0x200;
        value = gSavemap.scriptBanks[address];
        value |= (short)(g_SavemapScriptBanksHi[address] << 8);
        break;
    case FIELD_BANK_SAVE_300_BYTE:
        address = g_WorldMsgWindowOperandValues[windowIndex][g_WorldMsgWindowOperandIndex[windowIndex]] + 0x300;
        value = gSavemap.scriptBanks[address];
        break;
    case FIELD_BANK_SAVE_400_BYTE:
        address = g_WorldMsgWindowOperandValues[windowIndex][g_WorldMsgWindowOperandIndex[windowIndex]] + 0x400;
        value = gSavemap.scriptBanks[address];
        break;
    case FIELD_BANK_SAVE_300_WORD:
        address = g_WorldMsgWindowOperandValues[windowIndex][g_WorldMsgWindowOperandIndex[windowIndex]] + 0x300;
        value = gSavemap.scriptBanks[address];
        value |= (short)(g_SavemapScriptBanksHi[address] << 8);
        break;
    case FIELD_BANK_SAVE_400_WORD:
        address = g_WorldMsgWindowOperandValues[windowIndex][g_WorldMsgWindowOperandIndex[windowIndex]] + 0x400;
        value = gSavemap.scriptBanks[address];
        value |= (short)(g_SavemapScriptBanksHi[address] << 8);
        break;
    case FIELD_BANK_TEMP_BYTE:
        address = g_WorldMsgWindowOperandValues[windowIndex][g_WorldMsgWindowOperandIndex[windowIndex]];
        value = g_FieldTempBank[address];
        break;
    case FIELD_BANK_TEMP_WORD:
        address = g_WorldMsgWindowOperandValues[windowIndex][g_WorldMsgWindowOperandIndex[windowIndex]];
        value = g_FieldTempBank[address];
        value |= (short)(g_FieldTempBank[address + 1] << 8);
        break;
    default:
        value = 0;
    }
    return (short)value;
}

/*
 * Formats the unsigned-short value as decimal using the encoded digit table,
 * suppressing leading zero digits and retaining the units digit. Appends
 * terminator 0xFF.
 */
// FUNCTION: FF7 0x0076b375
void __cdecl WorldMsg_FormatDecimalNoPad(unsigned short value, unsigned char *dst)
{
    int started;
    short place;
    short outPos;
    short digit;

    started = 0;
    place = 0x2710;
    outPos = 0;
    for (; place > 1; place /= 10) {
        digit = value / place;
        if (started != 0 || digit != 0) {
            started = 1;
            dst[outPos] = g_WorldMsgDigitGlyphs[digit];
            outPos++;
        }
        value -= digit * place;
    }
    dst[outPos] = g_WorldMsgDigitGlyphs[value];
    dst[outPos + 1] = TEXT_END;
}

/*
 * Formats the unsigned-short value into five decimal positions using the
 * encoded digit table, filling leading zero positions with 0x3F. Appends
 * terminator 0xFF.
 */
// FUNCTION: FF7 0x0076b432
void __cdecl WorldMsg_FormatDecimalPadded5(unsigned short value, unsigned char *dst)
{
    int started;
    short place;
    short outPos;
    short digit;

    started = 0;
    place = 0x2710;
    outPos = 0;
    for (; place > 1; place /= 10) {
        digit = value / place;
        if (started != 0 || digit != 0) {
            started = 1;
            dst[outPos] = g_WorldMsgDigitGlyphs[digit];
            outPos++;
        } else if (started == 0) {
            dst[outPos] = 0x3f;
            outPos++;
        }
        value -= digit * place;
    }
    dst[outPos] = g_WorldMsgDigitGlyphs[value];
    dst[outPos + 1] = TEXT_END;
}

/*
 * Formats the unsigned-short value as hexadecimal using the encoded digit
 * table, suppressing leading zero digits and retaining the final digit.
 * Appends terminator 0xFF.
 */
// FUNCTION: FF7 0x0076b515
void __cdecl WorldMsg_FormatHexNoPad(unsigned short value, unsigned char *dst)
{
    int started;
    short place;
    short outPos;
    short digit;

    started = 0;
    place = 0x1000;
    outPos = 0;
    for (; place > 1; place /= 0x10) {
        digit = value / place;
        if (started != 0 || digit != 0) {
            started = 1;
            dst[outPos] = g_WorldMsgDigitGlyphs[digit];
            outPos++;
        }
        value -= digit * place;
    }
    dst[outPos] = g_WorldMsgDigitGlyphs[value];
    dst[outPos + 1] = TEXT_END;
}

/*
 * Decodes the message idx of the dialogue table into gSavemap.location. Expands
 * the TEXT_NAME_* codes into character names of at most nine bytes, copies the font
 * page and extended codes plus their following byte, copies other bytes directly
 * and stops at TEXT_END.
 */
// FUNCTION: FF7 0x0076b5d3
int __cdecl WorldMsg_DecodeMessageToTextBuffer(short idx)
{
    short charId;
    unsigned char *si;
    unsigned char *pSrc;
    short pos;
    short auxOutIdx;

    if (g_WorldMsgDialogTable == 0) {
        // STRING: FF7 0x0096e0d0
        World_DebugLogNoOp("No mes data!");
        return 0;
    }
    pSrc = g_WorldMsgDialogTable;
    pSrc += g_WorldMsgDialogTable[idx * 2 + 2];
    pSrc += g_WorldMsgDialogTable[idx * 2 + 3] << 8;
    pos = 0;
    auxOutIdx = 0;
    while (auxOutIdx < 0x17) {
        switch (*pSrc) {
        case TEXT_END:
            goto done;
        case TEXT_NAME_CLOUD:
        case TEXT_NAME_BARRET:
        case TEXT_NAME_TIFA:
        case TEXT_NAME_AERIS:
        case TEXT_NAME_RED_XIII:
        case TEXT_NAME_YUFFIE:
        case TEXT_NAME_CAIT_SITH:
        case TEXT_NAME_VINCENT:
        case TEXT_NAME_CID:
            charId = *pSrc - TEXT_NAME_CLOUD;
            si = (unsigned char *)Savemap_GetCharacterName(charId);
            if (si[pos] == TEXT_END || pos >= 9) {
                pSrc++;
                pos = 0;
            } else {
                gSavemap.location[auxOutIdx] = si[pos];
                pos++;
                auxOutIdx++;
            }
            break;
        case TEXT_FONT_PAGE_1:
        case TEXT_FONT_PAGE_2:
        case TEXT_FONT_PAGE_3:
        case TEXT_FONT_PAGE_4:
        case TEXT_EXTENDED:
            gSavemap.location[auxOutIdx] = *pSrc;
            pSrc++;
            auxOutIdx++;
        default:
            gSavemap.location[auxOutIdx] = *pSrc;
            pSrc++;
            auxOutIdx++;
            break;
        }
    }
done:
    gSavemap.location[auxOutIdx] = TEXT_END;
    return 1;
}

/*
 * Reports absent message data and returns when unavailable. Otherwise resolves
 * the selected message and copies bytes until TEXT_END into the selected
 * character-name storage.
 */
// FUNCTION: FF7 0x0076b787
void __cdecl WorldMsg_CopyMessageToCharacterName(short charIndex, short messageIndex)
{
    struct {
        unsigned char *message;
        short length;
        unsigned char *name;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if (g_WorldMsgDialogTable == 0) {
        // STRING: FF7 0x0096e0e0
        World_DebugLogNoOp("No mes data!");
        return;
    }
    frame.message = g_WorldMsgDialogTable;
    frame.message += g_WorldMsgDialogTable[messageIndex * 2 + 2];
    frame.message += g_WorldMsgDialogTable[messageIndex * 2 + 3] << 8;
    frame.length = 0;
    frame.name = (unsigned char *)Savemap_GetCharacterName(charIndex);
    while (*frame.message != TEXT_END) {
        *frame.name = *frame.message;
        frame.name++;
        frame.message++;
        frame.length++;
    }
    if (frame.length < 9) {
        *frame.name = TEXT_END;
    }
}
} /* extern "C" */
