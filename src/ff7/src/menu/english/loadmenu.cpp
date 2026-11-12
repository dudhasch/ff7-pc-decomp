// src/ff7/src/menu/english/loadmenu.cpp
// Original: C:\FF7\src\menu\English\loadmenu.cpp
// Contents: The load menu for save files: allocating and freeing the save buffer, scanning the save
//   directory, loading a slot, the per-frame state machine and shutdown, and the preview portrait
//   quads drawn at both UI scales.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
// Also: Save-slot handling for the load screen: restoring a slot's unpacked data into the
//   savemap, caching and drawing slot previews at both scales, scrolling the slot cursor, and
//   setting up the load menu.
#include "ff7/types/menu.h"
#include "lib/types/graphics.h"
#include "ff7/enums/menu.h"
#include "lib/enums/file.h"
#include "lib/enums/sound.h"
#include <string.h>
#include "lib/api/sound.h"
#include "ff7/api/menu.h"
#include "lib/api/graphics.h"
#include "ff7/savemap.h"
#include "ff7/enums/main.h"
#include "lib/codegen.h"
#include "lib/api/mem.h"
#include "ff7/api/battle.h"
#include "ff7/api/main.h"
#include <stdlib.h>
#include <direct.h>
#include "lib/crt.h"
#include <math.h>
#include "lib/api/file.h"
#include "lib/pfile.h"
extern "C" {

struct EnglishWindowFrame;

#pragma intrinsic(memcpy)

struct MenuCursor;
// clang-format on

/*
 * Restores unpacked save slot buffer data into active savemap.
 */
// FUNCTION: FF7 0x00720080
unsigned short __cdecl Menu_RestoreSaveSlotToSavemap(int slotIndex)
{
    int colourIndex;
    int cornerOffset;

    memcpy(&gSavemap, &gSaveFileBuffer[slotIndex], sizeof(gSavemap));
    for (colourIndex = 0; colourIndex < 0xc; colourIndex++) {
        if (colourIndex > 2 && colourIndex < 6) {
            g_FieldWindowColours[colourIndex] = gSavemap.windowColors[colourIndex + 3];
        } else if (colourIndex > 5 && colourIndex < 9) {
            g_FieldWindowColours[colourIndex] = gSavemap.descLocation[colourIndex + 29];
        } else {
            g_FieldWindowColours[colourIndex] = gSavemap.windowColors[colourIndex];
        }
    }
    /* the savemap's corners are top-left, bottom-left, top-right, bottom-right: 0, 3, 6, 9 */
    cornerOffset = 0;
    Menu_PackWindowColor(g_MenuCornerColorTL, gSavemap.windowColors[cornerOffset],
                    gSavemap.windowColors[cornerOffset + 1],
                    gSavemap.windowColors[cornerOffset + 2]);
    cornerOffset = 6;
    Menu_PackWindowColor(g_MenuCornerColorTR, gSavemap.windowColors[cornerOffset],
                    gSavemap.windowColors[cornerOffset + 1],
                    gSavemap.windowColors[cornerOffset + 2]);
    cornerOffset = 3;
    Menu_PackWindowColor(g_MenuCornerColorBL, gSavemap.windowColors[cornerOffset],
                    gSavemap.windowColors[cornerOffset + 1],
                    gSavemap.windowColors[cornerOffset + 2]);
    cornerOffset = 9;
    Menu_PackWindowColor(g_MenuCornerColorBR, gSavemap.windowColors[cornerOffset],
                    gSavemap.windowColors[cornerOffset + 1],
                    gSavemap.windowColors[cornerOffset + 2]);
    return 0;
}

/*
 * MenuCursor_Update for the save slot list: the same moves with a seven-step scroll
 * animation (MenuCursor_Update's is three or four) and with L1/R1 paging always on.
 */
// FUNCTION: FF7 0x007201e2
void __cdecl MenuCursor_UpdateScrolling(MenuCursor *cursor)
{
    if (cursor->scrollState == MENU_SCROLL_NONE) {
        if (BtlMenu_GetInputRepeat(INPUT_UP) != 0) {
            cursor->row--;
            switch (cursor->rowWrapMode) {
            case MENU_CURSOR_CLAMP:
                if (cursor->row < 0) {
                    cursor->row = 0;
                    if (cursor->scrollTop > 0) {
                        cursor->scrollTop--;
                        cursor->scrollAnim = -7;
                        cursor->scrollState = MENU_SCROLL_UP;
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    }
                } else {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                }
                break;
            case MENU_CURSOR_WRAP:
            case MENU_CURSOR_WRAP_INTO_ROWS:
                if (cursor->row < 0) {
                    cursor->row = cursor->rowCount - 1;
                }
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                break;
            }
        } else if (BtlMenu_GetInputRepeat(INPUT_DOWN) != 0) {
            cursor->row++;
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
            case MENU_CURSOR_WRAP_INTO_ROWS:
                if (cursor->row >= cursor->rowCount) {
                    cursor->row = 0;
                }
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                break;
            }
        } else if (BtlMenu_GetInputRepeat(INPUT_LEFT) != 0) {
            switch (cursor->colWrapMode) {
            case MENU_CURSOR_CLAMP:
                cursor->col--;
                if (cursor->col < 0) {
                    cursor->col = 0;
                } else {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                }
                break;
            case MENU_CURSOR_WRAP:
                cursor->col--;
                if (cursor->col < 0) {
                    cursor->col = cursor->colCount - 1;
                }
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                break;
            case MENU_CURSOR_WRAP_INTO_ROWS:
                /* At the first slot (column, row and page all 0) nothing moves and no
                   sound plays. The empty then-arm is the original's shape (MenuCursor_Update
                   breaks out of the switch instead). */
                if (cursor->col == 0 && cursor->row == 0 && cursor->scrollTop == 0) { /* empty first arm: lib/codegen.h */
                } else {
                    cursor->col--;
                    if (cursor->col < 0) {
                        cursor->col = cursor->colCount - 1;
                        cursor->row--;
                        if (cursor->row < 0) {
                            cursor->row = 0;
                            if (cursor->scrollTop > 0) {
                                cursor->scrollTop--;
                                cursor->scrollAnim = -7;
                                cursor->scrollState = MENU_SCROLL_UP;
                            }
                        }
                    }
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                }
                break;
            }
        } else if (BtlMenu_GetInputRepeat(INPUT_RIGHT) != 0) {
            switch (cursor->colWrapMode) {
            case MENU_CURSOR_CLAMP:
                cursor->col++;
                if (cursor->col >= cursor->colCount) {
                    cursor->col = cursor->colCount - 1;
                } else {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                }
                break;
            case MENU_CURSOR_WRAP:
                cursor->col++;
                if (cursor->col >= cursor->colCount) {
                    cursor->col = 0;
                }
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                break;
            case MENU_CURSOR_WRAP_INTO_ROWS:
                /* At the last slot nothing moves and no sound plays; as above, the
                   then-arm is empty in the original. */
                if (cursor->col == cursor->colCount - 1 && cursor->row == cursor->rowCount - 1 &&
                    cursor->scrollTop == cursor->totalRows - cursor->rowCount) { /* empty first arm: lib/codegen.h */
                } else {
                    cursor->col++;
                    if (cursor->col >= cursor->colCount) {
                        cursor->col = 0;
                        /* The original compares the row with the column count and stores the
                           0 again; MenuCursor_Update compares it with the row count. */
                        if (cursor->row >= cursor->colCount) {
                            cursor->col = 0;
                        }
                        cursor->row++;
                        if (cursor->row >= cursor->rowCount) {
                            cursor->row = cursor->rowCount - 1;
                            if (cursor->scrollTop < cursor->totalRows - cursor->rowCount) {
                                cursor->scrollAnim = -1;
                                cursor->scrollState = MENU_SCROLL_DOWN;
                            }
                        }
                    }
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                }
                break;
            }
        } else if (BtlMenu_GetInputRepeat(INPUT_R1) != 0) {
            cursor->scrollTop += cursor->rowCount;
            if (cursor->scrollTop > cursor->totalRows - cursor->rowCount) {
                cursor->scrollTop = cursor->totalRows - cursor->rowCount;
            } else {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
            }
        } else if (BtlMenu_GetInputRepeat(INPUT_L1) != 0) {
            cursor->scrollTop -= cursor->rowCount;
            if (cursor->scrollTop < 0) {
                cursor->scrollTop = 0;
            } else {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
            }
        }
    } else {
        switch (cursor->scrollState) {
        case MENU_SCROLL_UP:
            cursor->scrollAnim++;
            if (cursor->scrollAnim == 0) {
                cursor->scrollState = MENU_SCROLL_NONE;
                cursor->scrollAnim = 0;
            }
            break;
        case MENU_SCROLL_DOWN:
            cursor->scrollAnim--;
            if (cursor->scrollAnim == -8) {
                cursor->scrollTop++;
                cursor->scrollState = MENU_SCROLL_NONE;
                cursor->scrollAnim = 0;
            }
            break;
        }
    }
}

/*
 * Copies the slot's 0x54-byte preview header from the save file buffer (slots of 0x10F4 bytes)
 * into g_LoadMenuSlotPreviews.
 */
// FUNCTION: FF7 0x00720746
int __cdecl Menu_CacheSaveSlotPreview(int unused, int slot)
{
    g_LoadMenuSlotPreviews[slot] = *(SaveSlotPreviewInfo *)&gSaveFileBuffer[slot];
    return 0;
}

/*
 * Renders save slot preview data (characters and stats) in 2x mode.
 */
// FUNCTION: FF7 0x00720775
void __cdecl Menu_DrawSaveSlotPreview2x(int i, int top, int slotIndex)
{
    int baseX8;
    short frame[4];
    SaveSlotPreviewInfo *info;
    int currBaseY;
    int frameIdx;
    int colours[4];
    int index;

    info = &g_LoadMenuSlotPreviews[slotIndex];
    baseX8 = 0x158;
    currBaseY = top + 0x56;
    Menu_DrawEncodedString2x(baseX8 + 8, currBaseY + 0xD, info->location, 7, 0.4f);
    for (i = 0; i < 3; i++) {
        if (info->partyPortraits[i] != PARTY_CHAR_NONE) {
            Menu_DrawCharPortraitById2x(i * 0x57 + 0x25, top + 0xD, 0x50, 0x60, info->partyPortraits[i], 0.4f);
        }
    }
    baseX8 = 0x1DC;
    currBaseY = top + 9;
    Menu_DrawDecimalNumber2x(Menu_MeasureEncodedString2x(g_LoadMenuLevelLabel) + 0x168, top + 0x3C, info->leaderLevel, 2,
                      7, 0.4f);
    DrawIconGlyph16_2x(baseX8 + 0x76, currBaseY + 0x11, 0xD5, 7, 0.4f);
    Menu_DrawDecimalNumberZeroPad2x(baseX8 + 0x60, currBaseY + 0xF, Menu_PlaytimeToHoursDigits(info->playtime), 2, 7,
                               0.4f);
    Menu_DrawDecimalNumberZeroPad2x(baseX8 + 0x82, currBaseY + 0xF, Menu_PlaytimeToMinutesDigits(info->playtime), 2, 7,
                               0.4f);
    Menu_DrawDecimalNumber2x(baseX8 + 0x21, currBaseY + 0x2D, info->gil, 0xA, 7, 0.4f);
    Menu_DrawEncodedString2x(0x160, top + 0x38, g_LoadMenuLevelLabel, 5, 0.4f);
    Menu_DrawEncodedString2x(0x158, top + 0x11, info->leaderName, 7, 0.4f);
    Menu_DrawEncodedString2x(baseX8 + 0xA, currBaseY + 0xB, &g_MenuMainLabels.timeText, 7, 0.4f);
    Menu_DrawEncodedString2x(baseX8 + 0xA, currBaseY + 0x29, &g_MenuMainLabels.gilText, 7, 0.4f);
    /* corners in the savemap's order: top-left 0, bottom-left 3, top-right 6, bottom-right 9 */
    index = 0;
    Menu_PackWindowColor((unsigned char *)&colours[0], info->windowColors[index], info->windowColors[index + 1],
                    info->windowColors[index + 2]);
    index = 6;
    Menu_PackWindowColor((unsigned char *)&colours[1], info->windowColors[index], info->windowColors[index + 1],
                    info->windowColors[index + 2]);
    index = 3;
    Menu_PackWindowColor((unsigned char *)&colours[2], info->windowColors[index], info->windowColors[index + 1],
                    info->windowColors[index + 2]);
    index = 9;
    Menu_PackWindowColor((unsigned char *)&colours[3], info->windowColors[index], info->windowColors[index + 1],
                    info->windowColors[index + 2]);
    Menu_SetWindowColors(colours);
    for (frameIdx = 0; frameIdx < 2; frameIdx++) {
        Menu_CopyRectShorts(frame, ((short(*)[4])g_LoadMenuPreviewLayout)[frameIdx]);
        Menu_OffsetRectShorts(frame, 0, top);
        Menu_DrawWindowFrame2x((EnglishWindowFrame *)frame, 0.5f);
    }
    Menu_CopyRectShorts(frame, ((short(*)[4])g_LoadMenuPreviewLayout)[2]);
    Menu_OffsetRectShorts(frame, 0, top);
    Menu_DrawWindowFrame2x((EnglishWindowFrame *)frame, 0.51f);
}

/*
 * Renders save slot preview data (characters and stats) in 1x mode.
 */
// FUNCTION: FF7 0x00720aee
void __cdecl Menu_DrawSaveSlotPreview1x(int i, int top, int slotIndex)
{
    int baseX8;
    short frame[4];
    SaveSlotPreviewInfo *info;
    int currBaseY;
    int frameIdx;
    int colours[4];
    int index;

    info = &g_LoadMenuSlotPreviews[slotIndex];
    baseX8 = 0xAC;
    currBaseY = top + 0x2B;
    Menu_DrawEncodedString1x(baseX8 + 4, currBaseY + 7, info->location, 7, 0.4f);
    for (i = 0; i < 3; i++) {
        if (info->partyPortraits[i] != PARTY_CHAR_NONE) {
            Menu_DrawCharPortraitById1x(i * 0x2C + 0x13, top + 7, 0x28, 0x30, info->partyPortraits[i], 0.4f);
        }
    }
    baseX8 = 0xEE;
    currBaseY = top + 5;
    Menu_DrawDecimalNumber1x(Menu_MeasureEncodedString1x(g_LoadMenuLevelLabel) + 0xB4, top + 0x1E, info->leaderLevel, 2,
                        7, 0.4f);
    DrawIconGlyph16_1x(baseX8 + 0x3B, currBaseY + 9, 0xD5, 7, 0.4f);
    Menu_DrawDecimalNumberZeroPad1x(baseX8 + 0x30, currBaseY + 8, Menu_PlaytimeToHoursDigits(info->playtime), 2, 7, 0.4f);
    Menu_DrawDecimalNumberZeroPad1x(baseX8 + 0x41, currBaseY + 8, Menu_PlaytimeToMinutesDigits(info->playtime), 2, 7,
                               0.4f);
    Menu_DrawDecimalNumber1x(baseX8 + 0x11, currBaseY + 0x17, info->gil, 0xA, 7, 0.4f);
    Menu_DrawEncodedString1x(0xB0, top + 0x1C, g_LoadMenuLevelLabel, 5, 0.4f);
    Menu_DrawEncodedString1x(0xAC, top + 9, info->leaderName, 7, 0.4f);
    Menu_DrawEncodedString1x(baseX8 + 0xD, currBaseY + 6, &g_MenuMainLabels.timeText, 7, 0.4f);
    Menu_DrawEncodedString1x(baseX8 + 0xD, currBaseY + 0x15, &g_MenuMainLabels.gilText, 7, 0.4f);
    /* corners in the savemap's order: top-left 0, bottom-left 3, top-right 6, bottom-right 9 */
    index = 0;
    Menu_PackWindowColor((unsigned char *)&colours[0], info->windowColors[index], info->windowColors[index + 1],
                    info->windowColors[index + 2]);
    index = 6;
    Menu_PackWindowColor((unsigned char *)&colours[1], info->windowColors[index], info->windowColors[index + 1],
                    info->windowColors[index + 2]);
    index = 3;
    Menu_PackWindowColor((unsigned char *)&colours[2], info->windowColors[index], info->windowColors[index + 1],
                    info->windowColors[index + 2]);
    index = 9;
    Menu_PackWindowColor((unsigned char *)&colours[3], info->windowColors[index], info->windowColors[index + 1],
                    info->windowColors[index + 2]);
    Menu_SetWindowColors(colours);
    for (frameIdx = 0; frameIdx < 2; frameIdx++) {
        Menu_CopyRectShorts(frame, ((short(*)[4])g_LoadMenuPreviewLayout)[frameIdx]);
        Menu_OffsetRectShorts(frame, 0, top);
        Menu_DrawWindowFrame1x((EnglishWindowFrame *)frame, 0.5f);
    }
    Menu_CopyRectShorts(frame, ((short(*)[4])g_LoadMenuPreviewLayout)[2]);
    Menu_OffsetRectShorts(frame, 0, top);
    Menu_DrawWindowFrame1x((EnglishWindowFrame *)frame, 0.51f);
}

/*
 * Initializes load menu state and sets preview window layout pointer.
 */
// FUNCTION: FF7 0x00720e64
void __cdecl Menu_LoadScreenInit(int mode)
{
    g_LoadMenuResult = 0;
    g_LoadMenuErrorCode = 7;
    Menu_SetWindowColors((int *)g_LoadMenuWindowColors);
    MenuCursor_Init(&g_LoadMenuCursor2, 0, 1, 1, 2, 0, 0, 1, 2, 0, 0, 0, 1,
                    0, 0);
    if (g_MenuIsSmallViewport320x240 != 1) {
        g_LoadMenuPreviewLayout = g_LoadMenuWindowRects2x;
    } else {
        g_LoadMenuPreviewLayout = g_LoadMenuWindowRects1x;
    }
    Menu_LoadAllCharPortraits(0);
    Menu_LoadBusterTim();
    g_LoadMenuSubState = Menu_ScanSaveDirectory(0);
}
} /* extern "C" */


//
// Both functions pass __FILE__/__LINE__ into the tracked heap. The original
// literals ("C:\FF7\src\menu\English\loadmenu.cpp", lines 555 and 566) are
// immediates in .text, so they are reproduced verbatim rather than via
// __FILE__, which would expand to this repository's path instead.
//
// The two literals are separate copies in the original -- 0x00926208 and
// 0x00926230, 40 bytes apart and byte-identical -- because Visual C++ 5.0
// pools duplicate string literals only under /Gf, which this image was not
// built with. Each is annotated at its own address for that reason.

extern "C" {

// clang-format off
// The save/load screen short labels and prompts: 37 FF7-encoded rows of 36 bytes.
// Text: "Load" "Select a save data file." "Select a save game." "SLOT 1" "SLOT 2" "Are you sure?"
//       "Loading. Please wait." "Saving. Please wait." "EMPTY" "GAME" "Continue?" "/15"
//       "Checking Save Data File." "01".."15" "Saved." "Could not save." "Could not load."
//       "Data is ruined." "NEW GAME" "Are you sure you want to save?" "Yes" "No" ""
// GLOBAL: FF7 0x00925760
unsigned char g_SaveLoadTextRows[37][36] = {
    {0x2c, 0x4f, 0x41, 0x44, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x33, 0x45, 0x4c, 0x45, 0x43, 0x54, 0x00, 0x41, 0x00, 0x53, 0x41, 0x56, 0x45, 0x00, 0x44, 0x41, 0x54, 0x41, 0x00, 0x46, 0x49, 0x4c, 0x45, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x33, 0x45, 0x4c, 0x45, 0x43, 0x54, 0x00, 0x41, 0x00, 0x53, 0x41, 0x56, 0x45, 0x00, 0x47, 0x41, 0x4d, 0x45, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x33, 0x2c, 0x2f, 0x34, 0x00, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x33, 0x2c, 0x2f, 0x34, 0x00, 0x12, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x21, 0x52, 0x45, 0x00, 0x59, 0x4f, 0x55, 0x00, 0x53, 0x55, 0x52, 0x45, 0x1f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x2c, 0x4f, 0x41, 0x44, 0x49, 0x4e, 0x47, 0x0e, 0x00, 0x30, 0x4c, 0x45, 0x41, 0x53, 0x45, 0x00, 0x57, 0x41, 0x49, 0x54, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x33, 0x41, 0x56, 0x49, 0x4e, 0x47, 0x0e, 0x00, 0x30, 0x4c, 0x45, 0x41, 0x53, 0x45, 0x00, 0x57, 0x41, 0x49, 0x54, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x25, 0x2d, 0x30, 0x34, 0x39, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x27, 0x21, 0x2d, 0x25, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x23, 0x4f, 0x4e, 0x54, 0x49, 0x4e, 0x55, 0x45, 0x1f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x0f, 0x11, 0x15, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x23, 0x48, 0x45, 0x43, 0x4b, 0x49, 0x4e, 0x47, 0x00, 0x33, 0x41, 0x56, 0x45, 0x00, 0x24, 0x41, 0x54, 0x41, 0x00, 0x26, 0x49, 0x4c, 0x45, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x10, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x10, 0x12, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x10, 0x13, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x10, 0x14, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x10, 0x15, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x10, 0x16, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x10, 0x17, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x10, 0x18, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x10, 0x19, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x11, 0x10, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x11, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x11, 0x12, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x11, 0x13, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x11, 0x14, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x11, 0x15, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x33, 0x41, 0x56, 0x45, 0x44, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x23, 0x4f, 0x55, 0x4c, 0x44, 0x00, 0x4e, 0x4f, 0x54, 0x00, 0x53, 0x41, 0x56, 0x45, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x23, 0x4f, 0x55, 0x4c, 0x44, 0x00, 0x4e, 0x4f, 0x54, 0x00, 0x4c, 0x4f, 0x41, 0x44, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x24, 0x41, 0x54, 0x41, 0x00, 0x49, 0x53, 0x00, 0x52, 0x55, 0x49, 0x4e, 0x45, 0x44, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x2e, 0x25, 0x37, 0x00, 0x27, 0x21, 0x2d, 0x25, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x21, 0x52, 0x45, 0x00, 0x59, 0x4f, 0x55, 0x00, 0x53, 0x55, 0x52, 0x45, 0x00, 0x59, 0x4f, 0x55, 0x00, 0x57, 0x41, 0x4e, 0x54, 0x00, 0x54, 0x4f, 0x00, 0x53, 0x41, 0x56, 0x45, 0x1f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x39, 0x45, 0x53, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x2e, 0x4f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};
// The save/load screen popup and status dialog lines: 24 FF7-encoded rows of 48 bytes.
// Text: "Completed." "" "" "Formatted." "Could not format." "Not formatted."
//       "Want to format it now?" "No save data file." "No save data file."
//       "This Memory card is damaged and cannot be used." "Please insert another Memory card."
//       "Not enough space left on save data file." "Use another save data file,"
//       "or erase 1 block of saved data." "" "" "Couldn't read it." "Still want to begin the game?"
//       "..." "..." "..." "" "Can not allocate save file memory buffer!" "Invalid save file!"
// GLOBAL: FF7 0x00925c98
unsigned char g_SaveLoadDialogRows[24][48] = {
    {0x23, 0x4f, 0x4d, 0x50, 0x4c, 0x45, 0x54, 0x45, 0x44, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x26, 0x4f, 0x52, 0x4d, 0x41, 0x54, 0x54, 0x45, 0x44, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x23, 0x4f, 0x55, 0x4c, 0x44, 0x00, 0x4e, 0x4f, 0x54, 0x00, 0x46, 0x4f, 0x52, 0x4d, 0x41, 0x54, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x2e, 0x4f, 0x54, 0x00, 0x46, 0x4f, 0x52, 0x4d, 0x41, 0x54, 0x54, 0x45, 0x44, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x37, 0x41, 0x4e, 0x54, 0x00, 0x54, 0x4f, 0x00, 0x46, 0x4f, 0x52, 0x4d, 0x41, 0x54, 0x00, 0x49, 0x54, 0x00, 0x4e, 0x4f, 0x57, 0x1f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x2e, 0x4f, 0x00, 0x53, 0x41, 0x56, 0x45, 0x00, 0x44, 0x41, 0x54, 0x41, 0x00, 0x46, 0x49, 0x4c, 0x45, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x2e, 0x4f, 0x00, 0x53, 0x41, 0x56, 0x45, 0x00, 0x44, 0x41, 0x54, 0x41, 0x00, 0x46, 0x49, 0x4c, 0x45, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x34, 0x48, 0x49, 0x53, 0x00, 0x2d, 0x45, 0x4d, 0x4f, 0x52, 0x59, 0x00, 0x43, 0x41, 0x52, 0x44, 0x00, 0x49, 0x53, 0x00, 0x44, 0x41, 0x4d, 0x41, 0x47, 0x45, 0x44, 0x00, 0x41, 0x4e, 0x44, 0x00, 0x43, 0x41, 0x4e, 0x4e, 0x4f, 0x54, 0x00, 0x42, 0x45, 0x00, 0x55, 0x53, 0x45, 0x44, 0x0e, 0xff},
    {0x30, 0x4c, 0x45, 0x41, 0x53, 0x45, 0x00, 0x49, 0x4e, 0x53, 0x45, 0x52, 0x54, 0x00, 0x41, 0x4e, 0x4f, 0x54, 0x48, 0x45, 0x52, 0x00, 0x2d, 0x45, 0x4d, 0x4f, 0x52, 0x59, 0x00, 0x43, 0x41, 0x52, 0x44, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x2e, 0x4f, 0x54, 0x00, 0x45, 0x4e, 0x4f, 0x55, 0x47, 0x48, 0x00, 0x53, 0x50, 0x41, 0x43, 0x45, 0x00, 0x4c, 0x45, 0x46, 0x54, 0x00, 0x4f, 0x4e, 0x00, 0x53, 0x41, 0x56, 0x45, 0x00, 0x44, 0x41, 0x54, 0x41, 0x00, 0x46, 0x49, 0x4c, 0x45, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x35, 0x53, 0x45, 0x00, 0x41, 0x4e, 0x4f, 0x54, 0x48, 0x45, 0x52, 0x00, 0x53, 0x41, 0x56, 0x45, 0x00, 0x44, 0x41, 0x54, 0x41, 0x00, 0x46, 0x49, 0x4c, 0x45, 0x0c, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x4f, 0x52, 0x00, 0x45, 0x52, 0x41, 0x53, 0x45, 0x00, 0x11, 0x00, 0x42, 0x4c, 0x4f, 0x43, 0x4b, 0x00, 0x4f, 0x46, 0x00, 0x53, 0x41, 0x56, 0x45, 0x44, 0x00, 0x44, 0x41, 0x54, 0x41, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x23, 0x4f, 0x55, 0x4c, 0x44, 0x4e, 0x07, 0x54, 0x00, 0x52, 0x45, 0x41, 0x44, 0x00, 0x49, 0x54, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x33, 0x54, 0x49, 0x4c, 0x4c, 0x00, 0x57, 0x41, 0x4e, 0x54, 0x00, 0x54, 0x4f, 0x00, 0x42, 0x45, 0x47, 0x49, 0x4e, 0x00, 0x54, 0x48, 0x45, 0x00, 0x47, 0x41, 0x4d, 0x45, 0x1f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xb4, 0xb5, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xb4, 0xb5, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xb4, 0xb5, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x23, 0x41, 0x4e, 0x00, 0x4e, 0x4f, 0x54, 0x00, 0x41, 0x4c, 0x4c, 0x4f, 0x43, 0x41, 0x54, 0x45, 0x00, 0x53, 0x41, 0x56, 0x45, 0x00, 0x46, 0x49, 0x4c, 0x45, 0x00, 0x4d, 0x45, 0x4d, 0x4f, 0x52, 0x59, 0x00, 0x42, 0x55, 0x46, 0x46, 0x45, 0x52, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x29, 0x4e, 0x56, 0x41, 0x4c, 0x49, 0x44, 0x00, 0x53, 0x41, 0x56, 0x45, 0x00, 0x46, 0x49, 0x4c, 0x45, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};
// The ten save file labels (FF7 text encoding, 12 bytes each) the save and load screens
// draw by index (Menu_SaveScreenTick, Menu_LoadScreenTick).
// Text: "Save 1   " .. "Save 9   ", "Save 10"
// GLOBAL: FF7 0x00926118
unsigned char g_LoadMenuSaveFileNames[10][12] = {
    {0x33, 0x41, 0x56, 0x45, 0x00, 0x11, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00},
    {0x33, 0x41, 0x56, 0x45, 0x00, 0x12, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00},
    {0x33, 0x41, 0x56, 0x45, 0x00, 0x13, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00},
    {0x33, 0x41, 0x56, 0x45, 0x00, 0x14, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00},
    {0x33, 0x41, 0x56, 0x45, 0x00, 0x15, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00},
    {0x33, 0x41, 0x56, 0x45, 0x00, 0x16, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00},
    {0x33, 0x41, 0x56, 0x45, 0x00, 0x17, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00},
    {0x33, 0x41, 0x56, 0x45, 0x00, 0x18, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00},
    {0x33, 0x41, 0x56, 0x45, 0x00, 0x19, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00},
    {0x33, 0x41, 0x56, 0x45, 0x00, 0x11, 0x10, 0xff, 0x00, 0x00, 0x00, 0x00}};
// GLOBAL: FF7 0x00926190
unsigned char g_LoadMenuWindowRects2x[0x18] = {0xdc, 0x01, 0x09, 0x00, 0xa4, 0x00, 0x4d, 0x00, 0x54, 0x01, 0x56, 0x00, 0x2c, 0x01, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x02, 0x89, 0x00};
// GLOBAL: FF7 0x009261a8
unsigned char g_LoadMenuWindowRects1x[0x18] = {0xf6, 0x00, 0x05, 0x00, 0x4a, 0x00, 0x27, 0x00, 0xaa, 0x00, 0x2b, 0x00, 0x96, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x01, 0x45, 0x00};
// The title menu's window frame at 2x (the New Game / Continue entries are drawn from its x and y).
// GLOBAL: FF7 0x009261c0
EnglishWindowFrame g_LoadMenuTitleFrame2x = {267, 206, 107, 69};
// The load screen's slot list window frame at 2x.
// GLOBAL: FF7 0x009261c8
struct EnglishWindowFrame g_LoadMenuSlotFrame2x = {80, 206, 480, 69};
// The title menu's window frame at 1x (the New Game / Continue entries are drawn from its x and y).
// GLOBAL: FF7 0x009261d0
struct EnglishWindowFrame g_LoadMenuTitleFrame1x = {0x85, 0x67, 0x36, 0x23};
// The load screen's slot list window frame at 1x.
// GLOBAL: FF7 0x009261d8
struct EnglishWindowFrame g_LoadMenuSlotFrame1x = {48, 103, 225, 35};
unsigned char g_rawData_009261e0[16] = {0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff};
// The save slot caption "Level" in the menu text encoding, 0xFF-terminated: Menu_DrawSaveSlotPreview2x/Lo measure it
// and draw five glyphs of it.
// Text: "Level"
// GLOBAL: FF7 0x009261f0
unsigned char g_LoadMenuLevelLabel[6] = {0x2c, 0x45, 0x56, 0x45, 0x4c, 0xff};
// GLOBAL: FF7 0x009261f8
unsigned int g_LoadMenuWindowColors[4] = {2147483824, 2147483776, 2147483728, 2147483680};
// The load screen's input delay counter (Menu_LoadScreenTick: 0xA after a move, counted down per frame).
// GLOBAL: FF7 0x00dd6d80
int g_LoadMenuInputDelay = 0;
unsigned char g_bssPad_00dd6d84[4] = {0};
unsigned char g_bssPad_00dd6d88[8] = {0};
// GLOBAL: FF7 0x00dd6d90
void * g_LoadMenuPreviewLayout = 0;
// Menu_LoadScreenTick reads one byte of the save file header into the low byte of this int (ReadFileData) and
// splits it into the slot row (low nibble) and page (high nibble).
// GLOBAL: FF7 0x00dd6d94
int g_LoadMenuSlotState = 0;
// The load screen's two cursors: the slot cursor (column and row of the ten save files) and the
// game cursor (the row and scroll of the slots in the chosen file).
// GLOBAL: FF7 0x00dd6d98
MenuCursor g_LoadMenuCursor0 = {0};
// GLOBAL: FF7 0x00dd6dd0
MenuCursor g_LoadMenuCursor1 = {0};
unsigned char g_bssPad_00dd6e08[280] = {0};
// The title menu's cursor block (New Game / Continue): a MenuCursor of which Menu_LoadScreenTick reads the row.
// GLOBAL: FF7 0x00dd6f20
struct MenuCursor g_LoadMenuCursor2 = {0};
unsigned char g_bssPad_00dd6f58[112] = {0};
// The slot Menu_LoadScreenTick starts loading; cleared with g_LoadMenuCheckSlot and never read.
// GLOBAL: FF7 0x00dd6fc8
int g_LoadMenuSelectedSlot = 0;
// GLOBAL: FF7 0x00dd6fcc
int g_LoadMenuFrameCounter = 0;
// GLOBAL: FF7 0x00dd6fd0
SaveSlotPreviewInfo g_LoadMenuSlotPreviews[15] = {0};
unsigned char g_bssPad_00dd74bc[4] = {0};
unsigned char g_bssPad_00dd74c0[32] = {0};
// GLOBAL: FF7 0x00dd74e0
int g_LoadMenuFadePhase = LOAD_MENU_FADE_IN;
unsigned char g_bssPad_00dd74e4[4] = {0};
// GLOBAL: FF7 0x00dd74e8
unsigned char g_LoadMenuSlotBuffer[0x100] = {0};
// The SaveSlotPreview status of the ten save files (read by Menu_SaveScreenTick, Menu_ScanSaveDirectory,
// Menu_LoadScreenTick).
// GLOBAL: FF7 0x00dd75e8
unsigned char g_LoadMenuPreviewData[0x20] = {0};
// Menu_LoadScreenTick's save file check: set for the frame after a file loads, then the slot being cached
// (0..14), which also sets the gauge's width.
// GLOBAL: FF7 0x00dd7608
int g_LoadMenuCheckPending = 0;
// GLOBAL: FF7 0x00dd760c
int g_LoadMenuFrameParity = 0;
// GLOBAL: FF7 0x00dd7610
int g_LoadMenuCheckSlot = 0;
unsigned char g_bssPad_00dd7614[4] = {0};
unsigned char g_bssPad_00dd7618[224] = {0};
// GLOBAL: FF7 0x00dd76f8
int g_LoadMenuActive = 0;
// GLOBAL: FF7 0x00dd76fc
int g_LoadMenuSubState = 0;
// GLOBAL: FF7 0x00dd7700
struct Savemap *gSaveFileBuffer = 0;
// GLOBAL: FF7 0x00dd7704
int g_LoadMenuErrorCode = 0;
unsigned char g_bssPad_00dd7708[8] = {0};
// One flag per save file: the file is newly created (Menu_ScanSaveDirectory, Menu_LoadSaveFileToBuffer).
// GLOBAL: FF7 0x00dd7710
int g_LoadMenuSlotIsNew[10] = {0};
// GLOBAL: FF7 0x00dd7738
int g_LoadMenuResult = 0;
unsigned char g_bssPad_00dd773c[4] = {0};
// clang-format on

#pragma intrinsic(abs)

#pragma intrinsic(memset)




#pragma intrinsic(abs)
#pragma intrinsic(cos)
#pragma intrinsic(sin)
#pragma intrinsic(sqrt, atan2)


/*
 * Allocates the 0xFE4C-byte save file buffer; returns 0 on success, 1 when the allocation fails.
 */
// FUNCTION: FF7 0x00720ef0
int __cdecl Menu_SaveFileBufferAlloc(void)
{
    gSaveFileBuffer = 0;
    // STRING: FF7 0x00926208
    gSaveFileBuffer = (struct Savemap *)(GameHeapAlloc(SAVE_FILE_SLOT_COUNT * sizeof(struct Savemap), 1, "C:\\FF7\\src\\menu\\English\\loadmenu.cpp", 555));
    if (gSaveFileBuffer) {
        return 0;
    } else {
        return 1;
    }
}

/*
 * Frees save file memory buffer and nulls the pointer.
 */
// FUNCTION: FF7 0x00720f2f
void __cdecl Menu_SaveFileBufferFree(void)
{
    if (gSaveFileBuffer != 0) {
        // STRING: FF7 0x00926230
        GameHeapFree(gSaveFileBuffer, "C:\\FF7\\src\\menu\\English\\loadmenu.cpp", 566);
        gSaveFileBuffer = 0;
    }
}

/*
 * Checks if given save block is newly created.
 */
// FUNCTION: FF7 0x00720f5f
int __cdecl Menu_SaveFileIsNewlyCreated(int slot)
{
    return g_LoadMenuSlotIsNew[slot];
}

/*
 * Scans save directory for existing save file slots.
 */
// FUNCTION: FF7 0x00720f6e
int __cdecl Menu_ScanSaveDirectory(int markNew)
{
    int mask;
    int prevDir[64];
    int saveSlot;
    struct FileHandle *fileHandle;

    fileHandle = 0;
    for (saveSlot = 0; saveSlot < 0xA; saveSlot++) {
        ((SaveSlotPreview *)&g_LoadMenuPreviewData)[saveSlot].status = SAVE_FILE_STATUS_ABSENT;
    }
    mask = 0;
    // STRING: FF7 0x00926258
    sprintf((char *)g_LoadMenuSlotBuffer, "%ssave", g_InitPath_AppPath);
    if (ChangeDirectorySaving((char *)g_LoadMenuSlotBuffer, (char *)(prevDir)) == 0) {
        _mkdir((const char *)g_LoadMenuSlotBuffer);
    }
    ChangeDirectorySaving((char *)(prevDir), 0);
    for (saveSlot = 0; saveSlot < 0xA; saveSlot++) {
        // STRING: FF7 0x00926260
        sprintf((char *)g_LoadMenuSlotBuffer, "%ssave/save%02d.ff7", g_InitPath_AppPath, saveSlot);
        fileHandle = (struct FileHandle *)OpenFileSimple(FILE_OPEN_READ, g_LoadMenuSlotBuffer);
        if (fileHandle != 0) {
            CloseFileEx(fileHandle);
            ((SaveSlotPreview *)&g_LoadMenuPreviewData)[saveSlot].status = SAVE_FILE_STATUS_PRESENT;
            mask = 1 << saveSlot;
            g_LoadMenuSlotIsNew[saveSlot] = 0;
        } else if (markNew != 0) {
            ((SaveSlotPreview *)&g_LoadMenuPreviewData)[saveSlot].status = SAVE_FILE_STATUS_NEW;
            mask = 1 << saveSlot;
            g_LoadMenuSlotIsNew[saveSlot] = 1;
        }
    }
    return mask;
}

/*
 * Loads save file slot from disk into active buffer.
 */
// FUNCTION: FF7 0x007210bc
int __cdecl Menu_LoadSaveFileToBuffer(int mode, int slot)
{
    int header;
    struct FileHandle *fileHandle;
    int state;
    int res;

    fileHandle = 0;
    res = 0;
    // STRING: FF7 0x00926274
    sprintf((char *)g_LoadMenuSlotBuffer, "%ssave/save%02d.ff7", g_InitPath_AppPath, slot);
    switch (mode) {
    case SAVE_FILE_LOAD_EXISTING:
        fileHandle = (struct FileHandle *)OpenFileSimple(FILE_OPEN_READ, g_LoadMenuSlotBuffer);
        if (fileHandle != 0) {
            ReadFileData(4, &header, fileHandle);
            if (header == SAVE_FILE_MAGIC) {
                if (Menu_SaveFileBufferAlloc() == 0) {
                    ReadFileData(1, &g_LoadMenuSlotState, fileHandle);
                    ReadFileData(4, &state, fileHandle);
                    ReadFileData(SAVE_FILE_SLOT_COUNT * sizeof(struct Savemap), gSaveFileBuffer, fileHandle);
                    ((SaveSlotPreview *)&g_LoadMenuPreviewData)[slot].status = SAVE_FILE_STATUS_PRESENT;
                    res = SAVE_FILE_LOAD_OK;
                    *(short *)&g_SaveSlotPresentMask = state & 0xFFFF;
                    g_LoadMenuSlotIsNew[slot] = 0;
                } else {
                    res = SAVE_FILE_LOAD_NO_MEMORY;
                }
            } else {
                res = SAVE_FILE_LOAD_BAD_MAGIC;
            }
            CloseFileEx(fileHandle);
        }
        break;
    case SAVE_FILE_LOAD_OR_NEW:
        fileHandle = (struct FileHandle *)OpenFileSimple(FILE_OPEN_READ, g_LoadMenuSlotBuffer);
        if (fileHandle != 0) {
            ReadFileData(4, &header, fileHandle);
            if (header == SAVE_FILE_MAGIC) {
                if (Menu_SaveFileBufferAlloc() == 0) {
                    ReadFileData(1, &g_LoadMenuSlotState, fileHandle);
                    ReadFileData(4, &state, fileHandle);
                    ReadFileData(SAVE_FILE_SLOT_COUNT * sizeof(struct Savemap), gSaveFileBuffer, fileHandle);
                    ((SaveSlotPreview *)&g_LoadMenuPreviewData)[slot].status = SAVE_FILE_STATUS_PRESENT;
                    res = SAVE_FILE_LOAD_OK;
                    *(short *)&g_SaveSlotPresentMask = state & 0xFFFF;
                    g_LoadMenuSlotIsNew[slot] = 0;
                } else {
                    res = SAVE_FILE_LOAD_NO_MEMORY;
                }
            } else {
                res = SAVE_FILE_LOAD_BAD_MAGIC;
            }
            CloseFileEx(fileHandle);
        } else if (Menu_SaveFileBufferAlloc() == 0) {
            ((SaveSlotPreview *)&g_LoadMenuPreviewData)[slot].status = SAVE_FILE_STATUS_NEW;
            res = SAVE_FILE_LOAD_OK;
            *(short *)&g_SaveSlotPresentMask = 0;
            (*(char *)&g_LoadMenuSlotState) = 0;
            g_LoadMenuSlotIsNew[slot] = 1;
        } else {
            res = SAVE_FILE_LOAD_NO_MEMORY;
        }
    }
    return res;
}

/*
 * Main per-frame update and state machine for load menu.
 */
// FUNCTION: FF7 0x007212fb
int __cdecl Menu_LoadScreenTick(void)
{
    int slotIndex;
    int nameIdx;
    int rect[2];
    int textLength;
    int nameOfsX;
    int cacheRes;
    int gaugeColor;
    int nameOffsetY;
    int loadedFlag;
    int visibleSlots;
    int p;

    Menu_EmptyTextHook(0x80);
    if (g_LoadMenuFadePhase == LOAD_MENU_FADE_IN) {
        if (Menu_LoadScreenFadeStep(-0xF) == 0) {
            g_LoadMenuFadePhase = LOAD_MENU_FADE_NONE;
        }
    } else if (g_LoadMenuFadePhase == LOAD_MENU_FADE_OUT) {
        if (Menu_LoadScreenFadeStep(0xF) == 0xFF) {
            g_LoadMenuFadePhase = LOAD_MENU_FADE_DONE;
        }
    }
    if (g_MenuIsSmallViewport320x240 != 1) {
        switch (g_LoadMenuErrorCode) {
        case LOAD_MENU_SLOT_LIST:
            Menu_DrawMoreTextArrow2x(g_LoadMenuSlotFrame2x.x + g_LoadMenuCursor0.col * 0x5C + -0x19, g_LoadMenuSlotFrame2x.y + g_LoadMenuCursor0.row * 0x1A + 0xD, 0.2f);
            Menu_DrawEncodedString2x(0xA, 0xD, g_SaveLoadTextSelectDataFile, 7, 0);
            nameOffsetY = 0;
            nameOfsX = nameOffsetY;
            for (nameIdx = 0; nameIdx < 0xA; nameIdx++) {
                Menu_DrawEncodedString2x(g_LoadMenuSlotFrame2x.x + nameOfsX + 0x14, g_LoadMenuSlotFrame2x.y + nameOffsetY + 0xB, g_LoadMenuSaveFileNames[nameIdx], ((SaveSlotPreview *)&g_LoadMenuPreviewData)[nameIdx].status != SAVE_FILE_STATUS_ABSENT ? 7 : 0, 0.21f);
                nameOfsX = nameOfsX + Menu_MeasureEncodedString2x(g_LoadMenuSaveFileNames[nameIdx]) + 0x14;
                if (nameIdx == 4) {
                    nameOfsX = 0;
                    nameOffsetY = 0x1A;
                }
            }
            Menu_DrawWindowFrame2x(&g_LoadMenuSlotFrame2x, 0.211f);
            break;
        case LOAD_MENU_GAME_LIST:
            if (((SaveSlotPreview *)&g_LoadMenuPreviewData)[g_LoadMenuCursor0.row * 5 + g_LoadMenuCursor0.col].status == SAVE_FILE_STATUS_ABSENT) {
                g_LoadMenuErrorCode = LOAD_MENU_SLOT_LIST;
                break;
            }
            Menu_FlushAndSetScissor2x(0, 0x33, 0x280, 0x19B);
            Menu_DrawMoreTextArrow2x(8, g_LoadMenuCursor1.row * 0x89 + 0x78, 0);
            visibleSlots = (g_LoadMenuCursor1.scrollState != 0) + 3;
            for (slotIndex = 0; slotIndex < visibleSlots; slotIndex++) {
                if ((g_SaveSlotPresentMask & 1 << slotIndex + g_LoadMenuCursor1.scrollTop) != 0) {
                    Menu_SaveWindowColors();
                    Menu_DrawSaveSlotPreview2x(0, slotIndex * 0x89 + g_LoadMenuCursor1.scrollAnim * 0x11 + 0x33, slotIndex + g_LoadMenuCursor1.scrollTop);
                    Menu_RestoreWindowColors();
                } else {
                    Menu_DrawEncodedString2x(0x53, slotIndex * 0x89 + g_LoadMenuCursor1.scrollAnim * 0x11 + 0x71, g_SaveLoadTextEmpty, 6, 0.4f);
                    Menu_CopyRectShorts((short *)rect, (short *)((char *)g_LoadMenuPreviewLayout + 0x10));
                    Menu_OffsetRectShorts((short *)rect, 0, slotIndex * 0x89 + g_LoadMenuCursor1.scrollAnim * 0x11 + 0x33);
                    Menu_DrawWindowFrame2x((EnglishWindowFrame *)rect, 0.4f);
                }
            }
            Menu_EmitBatchListsAndInvalidate();
            Menu_FlushAndResetScissor2x();
            Menu_EmptyTextHook(0x80);
            Menu_DrawEncodedString2x(0xA, 0xD, g_SaveLoadTextSelectGame, 7, 0);
            Menu_DrawEncodedString2x(0x157, 0xD, g_SaveLoadTextGame, 6, 0);
            Menu_DrawEncodedString2x(Menu_MeasureEncodedString2x(g_SaveLoadTextGame) + 0x15A, 0xD, g_SaveLoadTextRows[g_LoadMenuCursor1.row + (g_LoadMenuCursor1.scrollTop + 0xD)], 7, 0);
            Menu_SetRectShorts((short *)rect, 0x14D, 0, 0x90, 0x33);
            Menu_DrawWindowFrame2x((EnglishWindowFrame *)rect, 0);
            break;
        case LOAD_MENU_CHECKING:
        case LOAD_MENU_CHECKED:
            if (g_LoadMenuErrorCode == LOAD_MENU_CHECKING) {
                gaugeColor = 0x804020A0;
            } else {
                gaugeColor = 0x80E08000;
            }
            Menu_DrawEncodedString2x(0xA, 0xD, g_SaveLoadTextChecking, 7, 0.1f);
            if (g_LoadMenuCheckPending == 0) {
                Menu_DrawGaugeBar2x(0xCB, 0xFF, (g_LoadMenuCheckSlot + 1) * 0xD, 0x11, gaugeColor, 0.1f);
            }
            Menu_SetRectShorts((short *)rect, 0xBB, 0xF0, 0xE9, 0x33);
            Menu_DrawWindowFrame2x((EnglishWindowFrame *)rect, 0.101f);
            break;
        case LOAD_MENU_LOADING:
            if (g_LoadMenuFadePhase != LOAD_MENU_FADE_OUT) {
                textLength = Menu_MeasureEncodedString2x(g_SaveLoadTextLoading);
                Menu_DrawEncodedString2x(0x140 - ((textLength + 0x1B) / 2) + 0xD, 0xFD, g_SaveLoadTextLoading, 7, 0.1f);
                Menu_SetRectShorts((short *)rect, 0x140 - ((textLength + 0x1B) / 2), 0xF0, textLength + 0x10, 0x33);
                Menu_DrawWindowFrame2x((EnglishWindowFrame *)rect, 0.101f);
            }
            break;
        case LOAD_MENU_TITLE:
            Menu_DrawMoreTextArrow2x(g_LoadMenuTitleFrame2x.x - 0x33, g_LoadMenuTitleFrame2x.y + g_LoadMenuCursor2.row * 0x1A + -0xE, 0.1f);
            Menu_DrawEncodedString2x(g_LoadMenuTitleFrame2x.x, g_LoadMenuTitleFrame2x.y - 0xE, g_SaveLoadTextNewGame, 7, 0.1f);
            Menu_DrawEncodedString2x(g_LoadMenuTitleFrame2x.x, g_LoadMenuTitleFrame2x.y + 0xC, g_SaveLoadTextContinue, g_LoadMenuSubState != 0 ? 7 : 0, 0.1f);
            Menu_BuildLoadPreviewQuad2x();
        }
        if (g_LoadMenuErrorCode != LOAD_MENU_TITLE) {
            Menu_DrawEncodedString2x(0x1F7, 0xD, g_SaveLoadTextRows[0], 7, 0.1f);
            Menu_SetRectShorts((short *)rect, 0x1DC, 0, 0xA4, 0x33);
            Menu_DrawWindowFrame2x((EnglishWindowFrame *)rect, 0.101f);
            Menu_SetRectShorts((short *)rect, 0, 0, 0x280, 0x33);
            Menu_DrawWindowFrame2x((EnglishWindowFrame *)rect, 0.201f);
        }
    } else {
        switch (g_LoadMenuErrorCode) {
        case LOAD_MENU_SLOT_LIST:
            Menu_DrawMoreTextArrow1x(g_LoadMenuSlotFrame1x.x + g_LoadMenuCursor0.col * 0x2B + -0xD, g_LoadMenuSlotFrame1x.y + g_LoadMenuCursor0.row * 0xD + 7, 0.2f);
            Menu_DrawEncodedString1x(0xA, 7, g_SaveLoadTextSelectDataFile, 7, 0);
            nameOffsetY = 0;
            nameOfsX = nameOffsetY;
            for (nameIdx = 0; nameIdx < 0xA; nameIdx++) {
                Menu_DrawEncodedString1x(g_LoadMenuSlotFrame1x.x + nameOfsX + 0xA, g_LoadMenuSlotFrame1x.y + nameOffsetY + 6, g_LoadMenuSaveFileNames[nameIdx], ((SaveSlotPreview *)&g_LoadMenuPreviewData)[nameIdx].status != SAVE_FILE_STATUS_ABSENT ? 7 : 0, 0.21f);
                nameOfsX = nameOfsX + Menu_MeasureEncodedString1x(g_LoadMenuSaveFileNames[nameIdx]) + 9;
                if (nameIdx == 4) {
                    nameOfsX = 0;
                    nameOffsetY = 0xD;
                }
            }
            Menu_DrawWindowFrame1x(&g_LoadMenuSlotFrame1x, 0.211f);
            break;
        case LOAD_MENU_GAME_LIST:
            if (((SaveSlotPreview *)&g_LoadMenuPreviewData)[g_LoadMenuCursor0.row * 5 + g_LoadMenuCursor0.col].status == SAVE_FILE_STATUS_ABSENT) {
                g_LoadMenuErrorCode = LOAD_MENU_SLOT_LIST;
                break;
            }
            Menu_FlushAndSetScissor1x(0, 0x1A, 0x140, 0xCE);
            Menu_DrawMoreTextArrow1x(4, g_LoadMenuCursor1.row * 0x45 + 0x3C, 0);
            visibleSlots = (g_LoadMenuCursor1.scrollState != 0) + 3;
            for (slotIndex = 0; slotIndex < visibleSlots; slotIndex++) {
                if ((g_SaveSlotPresentMask & 1 << slotIndex + g_LoadMenuCursor1.scrollTop) != 0) {
                    Menu_SaveWindowColors();
                    Menu_DrawSaveSlotPreview1x(0, slotIndex * 0x45 + g_LoadMenuCursor1.scrollAnim * 8 + 0x1A, slotIndex + g_LoadMenuCursor1.scrollTop);
                    Menu_RestoreWindowColors();
                } else {
                    Menu_DrawEncodedString1x(0x2A, slotIndex * 0x45 + g_LoadMenuCursor1.scrollAnim * 8 + 0x39, g_SaveLoadTextEmpty, 6, 0.4f);
                    Menu_CopyRectShorts((short *)rect, (short *)((char *)g_LoadMenuPreviewLayout + 0x10));
                    Menu_OffsetRectShorts((short *)rect, 0, slotIndex * 0x45 + g_LoadMenuCursor1.scrollAnim * 8 + 0x1A);
                    Menu_DrawWindowFrame1x((EnglishWindowFrame *)rect, 0.4f);
                }
            }
            Menu_EmitBatchListsAndInvalidate();
            Menu_FlushAndResetScissor1x();
            Menu_EmptyTextHook(0x80);
            Menu_DrawEncodedString1x(5, 7, g_SaveLoadTextSelectGame, 7, 0);
            Menu_DrawEncodedString1x(0xB8, 7, g_SaveLoadTextGame, 6, 0);
            Menu_DrawEncodedString1x(Menu_MeasureEncodedString1x(g_SaveLoadTextGame) + 0xBA, 7, g_SaveLoadTextRows[g_LoadMenuCursor1.row + (g_LoadMenuCursor1.scrollTop + 0xD)], 7, 0);
            Menu_SetRectShorts((short *)rect, 0xB3, 0, 0x43, 0x1A);
            Menu_DrawWindowFrame1x((EnglishWindowFrame *)rect, 0);
            break;
        case LOAD_MENU_CHECKING:
        case LOAD_MENU_CHECKED:
            if (g_LoadMenuErrorCode == LOAD_MENU_CHECKING) {
                gaugeColor = 0x804020A0;
            } else {
                gaugeColor = 0x80E08000;
            }
            Menu_DrawEncodedString1x(5, 7, g_SaveLoadTextChecking, 7, 0.1f);
            if (g_LoadMenuCheckPending == 0) {
                Menu_DrawGaugeBar1x(0x66, 0x80, (g_LoadMenuCheckSlot + 1) * 7, 9, gaugeColor, 0.1f);
            }
            Menu_SetRectShorts((short *)rect, 0x5E, 0x78, 0x75, 0x1A);
            Menu_DrawWindowFrame1x((EnglishWindowFrame *)rect, 0.101f);
            break;
        case LOAD_MENU_LOADING:
            if (g_LoadMenuFadePhase != LOAD_MENU_FADE_OUT) {
                textLength = Menu_MeasureEncodedString1x(g_SaveLoadTextLoading);
                Menu_DrawEncodedString1x(0xA0 - ((textLength + 0xE) / 2) + 7, 0x7F, g_SaveLoadTextLoading, 7, 0.1f);
                Menu_SetRectShorts((short *)rect, 0xA0 - ((textLength + 0xE) / 2), 0x78, textLength + 8, 0x1A);
                Menu_DrawWindowFrame1x((EnglishWindowFrame *)rect, 0.101f);
            }
            break;
        case LOAD_MENU_TITLE:
            Menu_DrawMoreTextArrow1x(g_LoadMenuTitleFrame1x.x - 0x1A, g_LoadMenuTitleFrame1x.y + g_LoadMenuCursor2.row * 0xD + -7, 0.1f);
            Menu_DrawEncodedString1x(g_LoadMenuTitleFrame1x.x, g_LoadMenuTitleFrame1x.y - 7, g_SaveLoadTextNewGame, 7, 0.1f);
            Menu_DrawEncodedString1x(g_LoadMenuTitleFrame1x.x, g_LoadMenuTitleFrame1x.y + 6, g_SaveLoadTextContinue, g_LoadMenuSubState != 0 ? 7 : 0, 0.1f);
            Menu_BuildLoadPreviewQuad1x();
        }
        if (g_LoadMenuErrorCode != LOAD_MENU_TITLE) {
            Menu_DrawEncodedString1x(0x104, 7, g_SaveLoadTextRows[0], 7, 0.1f);
            Menu_SetRectShorts((short *)rect, 0xF6, 0, 0x4A, 0x1A);
            Menu_DrawWindowFrame1x((EnglishWindowFrame *)rect, 0.101f);
            Menu_SetRectShorts((short *)rect, 0, 0, 0x140, 0x1A);
            Menu_DrawWindowFrame1x((EnglishWindowFrame *)rect, 0.201f);
        }
    }
    if (Menu_IsPopupActive() == 0 && g_LoadMenuFadePhase == LOAD_MENU_FADE_NONE) {
        switch (g_LoadMenuErrorCode) {
        case LOAD_MENU_SLOT_LIST:
            if (Input_GetPressed(INPUT_CIRCLE) != 0) {
                if (((SaveSlotPreview *)&g_LoadMenuPreviewData)[g_LoadMenuCursor0.row * 5 + g_LoadMenuCursor0.col].status != SAVE_FILE_STATUS_ABSENT) {
                    g_LoadMenuInputDelay = 0xA;
                    g_LoadMenuCheckSlot = 0;
                    g_LoadMenuSelectedSlot = g_LoadMenuCheckSlot;
                    p = Menu_LoadSaveFileToBuffer(SAVE_FILE_LOAD_EXISTING, g_LoadMenuCursor0.row * 5 + g_LoadMenuCursor0.col);
                    if (p == SAVE_FILE_LOAD_OK) {
                        g_LoadMenuErrorCode = LOAD_MENU_CHECKING;
                        g_LoadMenuCheckPending = 1;
                        MenuCursor_Init(&g_LoadMenuCursor1, 0, 0, 1, 3, 0, 0, 1, 0xF, 0, 0, 0, 0, 0, 1);
                        g_LoadMenuCursor1.row = (*(unsigned char *)&g_LoadMenuSlotState) & 0xF;
                        g_LoadMenuCursor1.scrollTop = ((*(unsigned char *)&g_LoadMenuSlotState) & 0xF0) >> 4;
                        loadedFlag = 1;
                    } else {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                        switch (p) {
                        case SAVE_FILE_LOAD_NO_MEMORY:
                            Menu_OpenPopupMessage(g_SaveLoadTextAllocFailed, 7);
                            break;
                        case SAVE_FILE_LOAD_BAD_MAGIC:
                            Menu_OpenPopupMessage(g_SaveLoadTextInvalidSaveFile, 7);
                        }
                    }
                } else {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                    Menu_OpenPopupMessage(g_SaveLoadTextNoSaveFile2, 7);
                }
            } else if (Input_GetPressed(INPUT_CROSS) != 0) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                g_LoadMenuErrorCode = LOAD_MENU_TITLE;
            } else {
                MenuCursor_Update((MenuCursor *)&g_LoadMenuCursor0.col);
            }
            break;
        case LOAD_MENU_GAME_LIST:
            cacheRes = g_LoadMenuCursor1.scrollAnim;
            MenuCursor_UpdateScrolling(&g_LoadMenuCursor1);
            if (g_LoadMenuCursor1.scrollAnim == 0 && cacheRes == 0) {
                if (Input_GetPressed(INPUT_CIRCLE) != 0) {
                    if ((g_SaveSlotPresentMask & 1 << g_LoadMenuCursor1.row + g_LoadMenuCursor1.scrollTop) == 0) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                        break;
                    }
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    g_LoadMenuErrorCode = LOAD_MENU_LOADING;
                    g_LoadMenuInputDelay = 0xA;
                } else if (Input_GetPressed(INPUT_CROSS) != 0) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                    Menu_SaveFileBufferFree();
                    g_LoadMenuErrorCode = LOAD_MENU_SLOT_LIST;
                }
            }
            break;
        case LOAD_MENU_CHECKING:
            if (g_LoadMenuInputDelay != 0) {
                if (g_LoadMenuInputDelay != 0) {
                    g_LoadMenuInputDelay--;
                }
            } else if (g_LoadMenuCheckPending != 0) {
                g_LoadMenuInputDelay = 0;
                g_LoadMenuCheckPending = g_LoadMenuInputDelay;
            } else {
                if ((g_SaveSlotPresentMask & 1 << g_LoadMenuCheckSlot) != 0) {
                    cacheRes = Menu_CacheSaveSlotPreview(g_LoadMenuCursor0.row, g_LoadMenuCheckSlot);
                } else {
                    cacheRes = 0;
                }
                g_LoadMenuCheckSlot++;
                if (cacheRes != 0) {
                    g_LoadMenuErrorCode = LOAD_MENU_SLOT_LIST;
                    Menu_OpenPopupMessage(g_SaveLoadTextCouldNotRead, 2);
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                }
                if (g_LoadMenuCheckSlot == 0xF) {
                    g_LoadMenuCheckSlot = 0xE;
                    g_LoadMenuErrorCode = LOAD_MENU_CHECKED;
                    g_LoadMenuInputDelay = 0xA;
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CONFIRM);
                }
            }
            break;
        case LOAD_MENU_CHECKED:
            if (g_LoadMenuInputDelay == 0) {
                g_LoadMenuErrorCode = LOAD_MENU_GAME_LIST;
            }
            g_LoadMenuInputDelay--;
            break;
        case LOAD_MENU_LOADING:
            if (g_LoadMenuInputDelay != 0) {
                g_LoadMenuInputDelay--;
            } else {
                *(char *)&g_GamePauseLatch = 1;
                cacheRes = (short)Menu_RestoreSaveSlotToSavemap(g_LoadMenuCursor1.row + g_LoadMenuCursor1.scrollTop);
                if (cacheRes == 0) {
                    if (gSavemap.checksum != (Menu_SaveChecksumCrc16(0x10F0, &gSavemap.descLevel) & 0xFFFF)) {
                        g_LoadMenuErrorCode = LOAD_MENU_GAME_LIST;
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                        Menu_OpenPopupMessage(g_SaveLoadTextDataRuined, cacheRes);
                    } else {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_SAVE_LOAD);
                        g_LoadMenuFadePhase = LOAD_MENU_FADE_OUT;
                        Menu_SetRequestedScreen(0);
                        /* Retail caller passes -1 arg(s) to 0-parameter `Menu_LoadScreenNoOpHook` (__cdecl stack cleanup matches retail). */
                        ((void(__cdecl *)(...))Menu_LoadScreenNoOpHook)(gSavemap.options & 3);
                    }
                } else {
                    g_LoadMenuErrorCode = LOAD_MENU_GAME_LIST;
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                    Menu_OpenPopupMessage(g_SaveLoadTextOf15, cacheRes);
                }
                *(char *)&g_GamePauseLatch = 0;
            }
            break;
        case LOAD_MENU_TITLE:
            if (Input_GetPressed(INPUT_CIRCLE) != 0) {
                switch (g_LoadMenuCursor2.row) {
                case LOAD_MENU_TITLE_NEW_GAME:
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_SAVE_LOAD);
                    g_LoadMenuResult = 1;
                    g_LoadMenuFadePhase = LOAD_MENU_FADE_OUT;
                    Menu_SetRequestedScreen(0);
                    break;
                case LOAD_MENU_TITLE_CONTINUE:
                    if (g_LoadMenuSubState != 0) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        MenuCursor_Init((MenuCursor *)&g_LoadMenuCursor0.col, 0, 0, 5, 2, 0, 0, 5, 2, 0, 0, 0, 1, 0, 0);
                        g_LoadMenuErrorCode = LOAD_MENU_SLOT_LIST;
                    } else {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                    }
                }
            } else {
                MenuCursor_Update(&g_LoadMenuCursor2);
            }
        }
    }
    return g_LoadMenuResult;
}

/*
 * Empty no-op function hook in load menu.
 */
// FUNCTION: FF7 0x0072237a
void __cdecl Menu_LoadScreenNoOpHook(void)
{
}

/*
 * Shuts down load menu and releases allocated buffers.
 */
// FUNCTION: FF7 0x0072237f
void __cdecl Menu_LoadScreenShutdown(void)
{
    Menu_ReleaseBatchLists();
    InputConfig_SyncCustomFlag();
    Menu_SaveFileBufferFree();
}

/*
 * One frame of the title screen's load menu (GAME_MODE_TITLE_MENU, through Menu_LoadMenuEntryThunk).
 * On the first frame it sets the menu up: the fade state (g_LoadMenuFadePhase) to 0, the frame
 * count to 0, the slot screen (Menu_LoadScreenInit), menu module id 0x10, the input auto-repeat to a
 * first delay of 200 and an interval of 50, and the overlay flag. While the menu is not finished
 * it draws the popup message (1x in the 320x240 modes, else 2x) and runs Menu_LoadScreenTick, which fades
 * in (state 0 to 1), handles the slot and file screens and, after the choice, fades out (state 2)
 * to -1; it flips g_LoadMenuFrameParity and counts g_LoadMenuFrameCounter up every frame. Once the
 * fade-out has finished it shuts the menu down (Menu_LoadScreenShutdown), rebuilds the stats and battle
 * records of the loaded party (Savemap_BuildPartyStatsFromRecord, Battle_LoadCombatantFromSavemap),
 * recomputes the encounter modifiers and sets g_MenuExitRequested. Returns Menu_LoadScreenTick's result,
 * which the menu dispatcher keeps and which chooses between a new game and a continue.
 */
// FUNCTION: FF7 0x00722393
int __cdecl Menu_LoadScreenModuleUpdate(void)
{
    int tickResult;
    int memberSlot;
    unsigned char characterId;

    if (g_LoadMenuActive == 0) {
        g_LoadMenuFadePhase = LOAD_MENU_FADE_IN;
        g_LoadMenuFrameCounter = g_LoadMenuFadePhase;
        Menu_LoadScreenInit(0);
        g_LoadMenuFrameParity = 0;
        g_MenuModuleId = 0x10;
        Input_SetRepeatRates(0xC8, 0x32);
        g_MenuOverlayActive = 1;
        g_LoadMenuActive = g_MenuOverlayActive;
    }
    if (g_LoadMenuFadePhase != LOAD_MENU_FADE_DONE) {
        if (g_MenuIsSmallViewport320x240 != 1) {
            Menu_DrawPopupMessage2x();
        } else {
            Menu_DrawPopupMessage1x();
        }
        /* Retail caller passes 1 arg(s) to 0-parameter `Menu_LoadScreenTick` (__cdecl stack cleanup matches retail). */
        tickResult = ((int(__cdecl *)(int))Menu_LoadScreenTick)(g_LoadMenuFrameCounter);
        if (g_LoadMenuFadePhase == LOAD_MENU_FADE_DONE) {
            g_MenuOverlayActive = 0;
            g_LoadMenuActive = g_MenuOverlayActive;
        }
        g_LoadMenuFrameParity ^= 1;
        g_LoadMenuFrameCounter++;
    }
    if (g_LoadMenuActive != 1) {
        Menu_LoadScreenShutdown();
        for (memberSlot = 0; memberSlot < 3; memberSlot++) {
            characterId = *(char *)&gSavemap.party[memberSlot];
            if (characterId == PARTY_CHAR_NONE) {
                continue;
            }
            Savemap_BuildPartyStatsFromRecord(memberSlot);
            Battle_LoadCombatantFromSavemap((char)memberSlot);
        }
        Party_RecomputeBattleModifiers();
        g_MenuExitRequested = 1;
    }
    return tickResult;
}

/*
 * Builds preview portrait textured batch quad in 2x mode.
 */
// FUNCTION: FF7 0x007224d7
void __cdecl Menu_BuildLoadPreviewQuad2x(void)
{
    float texU;
    float texV;
    volatile float du;
    int zBits;
    float vSpan;
    GfxTlVertex *vertexB;
    GfxTlVertex *corner0Ptr;
    GfxTlVertex *vertexDPtr;
    GfxTlVertex *vertexC;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    if (AcquireBatchNode(1, g_MenuBusterTexGraphicsObject) != 0) {
        zBits = 0;
        texV = 0.0f;
        texU = texV;
        du = 0.5859375f;
        vSpan = 0.5859375f;
        corner0Ptr = (GfxTlVertex *)(g_MenuBusterTexGraphicsObject->vertexCursor);
        corner0Ptr->xBits = FLOAT_185_BITS;
        corner0Ptr->yBits = FLOAT_180_BITS;
        corner0Ptr->zBits = zBits;
        corner0Ptr->rhwBits = FLOAT_ONE_BITS;
        corner0Ptr->color = 0xC0C0C0C0;
        corner0Ptr->specular = 0xFF000000;
        corner0Ptr->u = texU;
        corner0Ptr->v = texV;
        vertexB = (GfxTlVertex *)(g_MenuBusterTexGraphicsObject->vertexCursor + 0x20);
        vertexB->xBits = FLOAT_185_BITS;
        vertexB->yBits = FLOAT_501_BITS;
        vertexB->zBits = zBits;
        vertexB->rhwBits = FLOAT_ONE_BITS;
        vertexB->color = 0xC0C0C0C0;
        vertexB->specular = 0xFF000000;
        vertexB->u = texU;
        vertexB->v = texV + vSpan;
        vertexC = (GfxTlVertex *)(g_MenuBusterTexGraphicsObject->vertexCursor + 0x40);
        vertexC->xBits = FLOAT_485_BITS;
        vertexC->yBits = FLOAT_180_BITS;
        vertexC->zBits = zBits;
        vertexC->rhwBits = FLOAT_ONE_BITS;
        vertexC->color = 0xC0C0C0C0;
        vertexC->specular = 0xFF000000;
        vertexC->u = texU + du;
        vertexC->v = texV;
        vertexDPtr = (GfxTlVertex *)(g_MenuBusterTexGraphicsObject->vertexCursor + 0x60);
        vertexDPtr->xBits = FLOAT_485_BITS;
        vertexDPtr->yBits = FLOAT_501_BITS;
        vertexDPtr->zBits = zBits;
        vertexDPtr->rhwBits = FLOAT_ONE_BITS;
        vertexDPtr->color = 0xC0C0C0C0;
        vertexDPtr->specular = 0xFF000000;
        vertexDPtr->u = texU + du;
        vertexDPtr->v = texV + vSpan;
    }
}

/*
 * Builds preview portrait textured batch quad in 1x mode.
 */
// FUNCTION: FF7 0x0072269b
void __cdecl Menu_BuildLoadPreviewQuad1x(void)
{
    float vStart;
    float curUStartScalar;
    float quadLeft;
    int zero;
    float quadTop;
    float du;
    float vSpan;
    GfxTlVertex *vertexB;
    GfxTlVertex *corner0Ptr;
    GfxTlVertex *vertexDPtr;
    GfxTlVertex *vertexC;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    if (AcquireBatchNode(1, g_MenuBusterTexGraphicsObject) != 0) {
        zero = 0;
        quadLeft = g_MenuViewportX + 93.0f;
        quadTop = g_MenuViewportY + 9e+01f;
        vStart = 0.0f;
        curUStartScalar = vStart;
        du = 0.5859375f;
        vSpan = 0.5859375f;
        corner0Ptr = (GfxTlVertex *)(g_MenuBusterTexGraphicsObject->vertexCursor);
        corner0Ptr->x = quadLeft;
        corner0Ptr->y = quadTop;
        corner0Ptr->zBits = zero;
        corner0Ptr->rhwBits = FLOAT_ONE_BITS;
        corner0Ptr->color = 0xC0C0C0C0;
        corner0Ptr->specular = 0xFF000000;
        corner0Ptr->u = curUStartScalar;
        corner0Ptr->v = vStart;
        vertexB = (GfxTlVertex *)(g_MenuBusterTexGraphicsObject->vertexCursor + 0x20);
        vertexB->x = quadLeft;
        vertexB->y = quadTop + 161.0f;
        vertexB->zBits = zero;
        vertexB->rhwBits = FLOAT_ONE_BITS;
        vertexB->color = 0xC0C0C0C0;
        vertexB->specular = 0xFF000000;
        vertexB->u = curUStartScalar;
        vertexB->v = vStart + vSpan;
        vertexC = (GfxTlVertex *)(g_MenuBusterTexGraphicsObject->vertexCursor + 0x40);
        vertexC->x = quadLeft + 1.5e+02f;
        vertexC->y = quadTop;
        vertexC->zBits = zero;
        vertexC->rhwBits = FLOAT_ONE_BITS;
        vertexC->color = 0xC0C0C0C0;
        vertexC->specular = 0xFF000000;
        vertexC->u = curUStartScalar + du;
        vertexC->v = vStart;
        vertexDPtr = (GfxTlVertex *)(g_MenuBusterTexGraphicsObject->vertexCursor + 0x60);
        vertexDPtr->x = quadLeft + 1.5e+02f;
        vertexDPtr->y = quadTop + 161.0f;
        vertexDPtr->zBits = zero;
        vertexDPtr->rhwBits = FLOAT_ONE_BITS;
        vertexDPtr->color = 0xC0C0C0C0;
        vertexDPtr->specular = 0xFF000000;
        vertexDPtr->u = curUStartScalar + du;
        vertexDPtr->v = vStart + vSpan;
    }
}

/*
 * Configures preview frame graphics pointers for save/load menu.
 */
// FUNCTION: FF7 0x0072288f
void __cdecl Menu_PickSavePreviewFrameGfx(void)
{
    if (g_MenuIsSmallViewport320x240 != 1) {
        g_LoadMenuPreviewLayout = g_LoadMenuWindowRects2x;
    } else {
        g_LoadMenuPreviewLayout = g_LoadMenuWindowRects1x;
    }
}
} /* extern "C" */
