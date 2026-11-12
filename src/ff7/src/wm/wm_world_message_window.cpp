// src/ff7/src/wm/wm_world_message_window.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Opens, answers, closes and polls the world map's main text message window. It also sets
//   the window's rectangle and mode and reports the chosen answer to the script.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/wm.h"
#include <string.h>
#include "ff7/world.h"
#include "lib/codegen.h"
#include "ff7/api/field.h"
#include "ff7/api/wm.h"
#include "ff7/api/menu.h"
#include "lib/api/token.h"
#include "ff7/enums/field.h"
#include "ff7/field.h"
#include "ff7/types/field.h"
extern "C" {

#pragma intrinsic(strcpy, memcpy)

// clang-format off
// GLOBAL: FF7 0x00e36100
short g_WorldMsgChoiceLastLine = 0;
// GLOBAL: FF7 0x00e36104
short g_WorldMsgChoiceAnswer = 0;
// GLOBAL: FF7 0x00e36108
short g_WorldMsgChoiceFirstLine = 0;
// GLOBAL: FF7 0x00e3610c
int g_WorldMsgInputUpdated = 0;
// clang-format on

/*
 * Resets the first field-window owner and current entity to 0xFF, installs script in
 * g_FieldScripts, sets its dialogTextOffset to 8, calls WorldMsg_InitWindowsAndMessageTable and
 * points g_pFieldState at g_FieldActorState.
 */
// FUNCTION: FF7 0x0075ee50
void __cdecl WorldMsg_InitScriptContextAndWindows(void *script)
{
    g_FieldWindowOwner[0] = 0xff;
    g_CurrentEntity = 0xff;
    g_FieldScripts = (FieldScriptHeader *)script;
    g_FieldScripts->dialogTextOffset = 8;
    WorldMsg_InitWindowsAndMessageTable();
    (*(void **)&g_pFieldState) = (void *)&g_FieldActorState.state;
}

/*
 * Opens world message window 0 with message `value` through WorldMsg_DispatchWindowState,
 * but only when no window is open. Clears the question bookkeeping first.
 */
// FUNCTION: FF7 0x0075ee86
void __cdecl WorldMsg_Show(char value)
{
    if (g_FieldWindows[0].state == MSG_WINDOW_CLOSED) {
        g_WorldMsgChoiceLastLine = 0;
        g_WorldMsgChoiceFirstLine = g_WorldMsgChoiceLastLine;
        WorldMsg_DispatchWindowState(0, value);
    }
}

/*
 * Opens window 0 as a question: WorldMsg_UpdateChoiceWindow gets the message, the first and last
 * selectable line (firstLine and lastLine) and the address the answer is written to. Only when the
 * window is idle.
 */
// FUNCTION: FF7 0x0075eebb
void __cdecl WorldMsg_Ask(char messageId, short firstLine, short lastLine)
{
    if (g_FieldWindows[0].state == MSG_WINDOW_CLOSED) {
        g_WorldMsgChoiceFirstLine = firstLine;
        g_WorldMsgChoiceLastLine = lastLine;
        WorldMsg_UpdateChoiceWindow(0, messageId, *(char *)&g_WorldMsgChoiceFirstLine, *(char *)&g_WorldMsgChoiceLastLine, &g_WorldMsgChoiceAnswer);
    }
}

/*
 * Returns whether world message window 0 is open (state != 0).
 */
// FUNCTION: FF7 0x0075ef01
int __cdecl WorldMsg_IsOpen(void)
{
    return g_FieldWindows[0].state != 0;
}

/*
 * Asks window 0 to close through WorldMsg_RequestWindowClose unless it is already idle or in
 * state 7, and returns whether it is still open. Scripts poll this until it reports 0.
 */
// FUNCTION: FF7 0x0075ef13
int __cdecl WorldMsg_RequestCloseAndPoll(void)
{
    if (g_FieldWindows[0].state != MSG_WINDOW_CLOSED && g_FieldWindows[0].state != MSG_WINDOW_CLOSING) {
        WorldMsg_RequestWindowClose(0);
    }
    return g_FieldWindows[0].state != 0;
}

/*
 * When field window zero is active, reads raw held input twice and updates two independent newly-
 * held masks and previous-held masks in the field state. Dispatches either the basic dialog helper
 * or the configured dialog helper according to the two flags, then sets g_WorldMsgInputUpdated to 1.
 */
// FUNCTION: FF7 0x0075ef46
void __cdecl WorldMsg_PollInputAndUpdateWindow(void)
{
    unsigned int heldKeys;
    int res;

    if (g_FieldWindows[0].state != 0) {
        heldKeys = Input_GetHeldRaw();
        (*(int *)&g_pFieldState->newActiveKeys) =
            heldKeys & ~(*(int *)&g_pFieldState->activeKeys);
        (*(int *)&g_pFieldState->activeKeys) = heldKeys;
        heldKeys = Input_GetHeldRaw();
        (*(int *)&g_pFieldState->newActiveKeys2) =
            heldKeys & ~(*(int *)&g_pFieldState->activeKeys2);
        (*(int *)&g_pFieldState->activeKeys2) = heldKeys;
        if (g_WorldMsgChoiceFirstLine == 0 && g_WorldMsgChoiceLastLine == 0) {
            res = WorldMsg_DispatchWindowState(0, 0);
        } else {
            res = WorldMsg_UpdateChoiceWindow(
                0, 0, *(char *)&g_WorldMsgChoiceFirstLine, *(char *)&g_WorldMsgChoiceLastLine, &g_WorldMsgChoiceAnswer);
        }
        g_WorldMsgInputUpdated = 1;
    }
}

/*
 * Returns the line chosen in the last WorldMsg_Ask once window 0 has closed, or -1 while it is
 * still open.
 */
// FUNCTION: FF7 0x0075f00e
short __cdecl WorldMsg_GetAnswer(void)
{
    int answer;

    if (g_FieldWindows[0].state == MSG_WINDOW_CLOSED) {
        answer = g_WorldMsgChoiceAnswer;
    } else {
        answer = -1;
    }
    return (short)answer;
}

/*
 * Sets window 0's rectangle from a four-short array (x, y, width, height) through
 * WorldMsg_SetWindowRectClamped; a null pointer is ignored.
 */
// FUNCTION: FF7 0x0075f038
void __cdecl WorldMsg_SetWindowRect(short *rect)
{
    if (rect != 0) {
        WorldMsg_SetWindowRectClamped(0, rect[0], rect[1], rect[2], rect[3]);
    }
}

/*
 * Sets window 0's mode pair (a, b) through WorldMsg_SetWindowStyleAndPreventClose.
 */
// FUNCTION: FF7 0x0075f06c
void __cdecl WorldMsg_SetWindowMode(short style, short preventClose)
{
    WorldMsg_SetWindowStyleAndPreventClose(0, style, preventClose);
}
} /* extern "C" */
