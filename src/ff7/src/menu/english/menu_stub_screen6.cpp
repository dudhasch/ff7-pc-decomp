// src/ff7/src/menu/english/menu_stub_screen6.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: A menu stub screen, number 6, with its setup and per-frame update and drawing, plus an
//   empty hook.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <string.h>
#include "ff7/types/menu.h"
#include "lib/enums/file.h"
#include <stdlib.h>
#include "lib/api/file.h"
#include "ff7/api/menu.h"
#include "ff7/enums/menu.h"
extern "C" {

struct EnglishWindowFrame;
#pragma intrinsic(abs, memcpy, memset)

// clang-format off
// GLOBAL: FF7 0x00920520
int g_MenuStubScreen6State = (-1);
// GLOBAL: FF7 0x00920528
unsigned char g_MenuStubScreen6WindowRects[12] = {0, 0, 0, 0, 0x80, 2, 0x33, 0, 0, 0, 0x33};
unsigned char g_rawData_00920534[4] = {0x80, 0x02, 0xad, 0x01};
// GLOBAL: FF7 0x00920538
unsigned char g_MenuStubScreen6Text[8] = {0x53, 0x69, 0x17, 0x61, 0xa1, 0x69, 0xff};
// GLOBAL: FF7 0x00dca3d0
int g_MenuStubScreen6Cursor0[0xe] = {0};
// GLOBAL: FF7 0x00dca408
int g_MenuStubScreen6Cursor1[0xe] = {0};
unsigned char g_bssPad_00dca440[56] = {0};
// clang-format on

// clang-format off
#define g_MenuStubScreen6Window1Y (((MenuStubScreen6WindowRects *)&g_MenuStubScreen6WindowRects)->window1Y)
// clang-format on

/*
 * Initializes the two menu cursors of stub screen 6 and its state.
 */
// FUNCTION: FF7 0x00703610
void __cdecl Menu_StubScreen6Init(int arg)
{
    int junk[8];

    MenuCursor_Init((MenuCursor *)g_MenuStubScreen6Cursor0, 0, 0, 1, 3, 0, 0, 1, 3, 0, 0, 0, 1, 0,
                    0);
    MenuCursor_Init((MenuCursor *)g_MenuStubScreen6Cursor1, 0, 0, 1, 3, 0, 0, 1, 9, 0, 0, 0, 0, 0,
                    0);
    g_MenuStubScreen6State = 0;
}

/*
 * Draws one frame of stub screen 6 and updates its cursor; Cross returns to the main menu.
 */
// FUNCTION: FF7 0x00703676
void __cdecl Menu_StubScreen6Frame(unsigned int frameCounter)
{
    int p;
    int q;
    int r;
    int s;
    int t;
    int x;
    int y;
    int i;
    int z;
    int j;
    int k;
    int n;

    Menu_DrawCommandWindow2x(g_MenuModuleId);
    /* Unused stub menu cursor/state index (0). */
    switch (g_MenuStubScreen6State) {
    case 0:
        Menu_DrawMoreTextArrow2x(0, g_MenuStubScreen6Window1Y + (g_MenuStubScreen6Cursor0[1] << 6) + 0x20,
                            0.0f);
        break;
    }
    Menu_DrawEncodedString2x(0x10, 0xb, g_MenuStubScreen6Text, 7, 0.0f);
    for (y = 0; y < 2; y++) {
        Menu_DrawWindowFrame2x((EnglishWindowFrame *)(&g_MenuStubScreen6WindowRects[y * 8]), 0.0f);
    }
    MenuCursor_Update(
        (MenuCursor *)((char *)g_MenuStubScreen6Cursor0 + g_MenuStubScreen6State * 0x38));
    /* Unused stub menu cursor/state index (0). */
    switch (g_MenuStubScreen6State) {
    case 0:
        Input_GetPressed(INPUT_CIRCLE);
        break;
    }
    if (BtlMenu_GetInputRepeat(INPUT_CROSS) != 0) {
        Menu_SetPhase(MENU_PHASE_CLOSE_SCREEN, MENU_SCREEN_MAIN);
        Menu_SetScreen(MENU_SCREEN_MAIN);
    }
}

/*
 * Empty no-op menu handler hook.
 */
// FUNCTION: FF7 0x00703759
void __cdecl Menu_UnusedStubScreen6NoOp(void)
{
}
} /* extern "C" */
