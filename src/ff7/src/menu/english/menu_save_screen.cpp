// src/ff7/src/menu/english/menu_save_screen.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The Save menu: loading party portraits on exit, writing a save slot with its preview
//   and checksum, and the save screen's setup, per-frame drawing with device selection, shutdown
//   and module tick.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/main.h"
#include "ff7/enums/menu.h"
#include "ff7/api/menu.h"
#include <string.h>
#include "ff7/types/menu.h"
#include "lib/enums/file.h"
#include "lib/enums/sound.h"
#include <stdio.h>
#include <stdlib.h>
#include "lib/api/file.h"
#include "lib/api/sound.h"
#include "ff7/savemap.h"
#include "ff7/enums/main.h"
extern "C" {

struct EnglishWindowFrame;

// clang-format off
// The save screen's window rectangles {x, y, width, height}: the slot-list window 2x/1x, the
// confirmation window 2x/1x, and the title caption "Save" (menu text encoding).
// GLOBAL: FF7 0x0091f0b8
struct EnglishWindowFrame g_SaveMenuSlotWindow2x = {80, 207, 480, 67};
unsigned char g_rawData_0091f0c0[8] = {0x85, 0x00, 0x67, 0x00, 0x36, 0x00, 0x22, 0x00};
// GLOBAL: FF7 0x0091f0c8
struct EnglishWindowFrame g_SaveMenuSlotWindow1x = {48, 103, 225, 34};
// GLOBAL: FF7 0x0091f0d0
struct EnglishWindowFrame g_SaveMenuConfirmWindow2x = {153, 190, 335, 101};
// GLOBAL: FF7 0x0091f0d8
struct EnglishWindowFrame g_SaveMenuConfirmWindow1x = {76, 95, 168, 51};
// Text: "Save"
// GLOBAL: FF7 0x0091f0e0
unsigned char g_SaveMenuTitleText[8] = {0x33, 0x41, 0x56, 0x45, 0xff};
// GLOBAL: FF7 0x0091f0e8
EnglishWindowFrame g_SaveMenuFrames2x[3] = {{476, 9, 164, 77}, {340, 86, 300, 51}, {0, 0, 640, 137}};
// GLOBAL: FF7 0x0091f100
EnglishWindowFrame g_SaveMenuFrames1x[3] = {{246, 5, 74, 39}, {170, 43, 150, 26}, {0, 0, 320, 69}};
// The title window Menu_SaveScreenTick draws around g_SaveMenuTitleText at 2x and at 1x.
// GLOBAL: FF7 0x0091f118
EnglishWindowFrame g_SaveMenuTitleFrame2x = {476, 0, 164, 51};
// GLOBAL: FF7 0x0091f120
struct EnglishWindowFrame g_SaveMenuTitleFrame1x = {246, 0, 74, 26};
// GLOBAL: FF7 0x00dc6ac8
int g_SaveMenuState = 0;
// The save screen's frame delay (Menu_SaveScreenTick).
// GLOBAL: FF7 0x00dc6acc
int g_SaveMenuDelay = 0;
// GLOBAL: FF7 0x00dc6ad0
EnglishWindowFrame *g_SaveMenuFrames = 0;
// GLOBAL: FF7 0x00dc6ad4
int g_SaveMenuFadePhase = 0;
// GLOBAL: FF7 0x00dc6ad8
unsigned int g_SaveMenuFrameCounter = 0;
unsigned char g_bssPad_00dc6adc[4] = {0};
// GLOBAL: FF7 0x00dc6ae0
int g_SaveMenuMode = 0;
// The save screen's selected row (Menu_SaveScreenTick).
// GLOBAL: FF7 0x00dc6ae4
int g_SaveMenuRow = 0;
unsigned char g_bssPad_00dc6ae8[48] = {0};
// The slot-list cursor block, fourteen dwords that MenuCursor_Init and MenuCursor_UpdateScrolling
// write through its address: this head, g_SaveMenuSelectedSlot (the row), the fill, g_SaveMenuCursor
// (the first visible slot), g_SaveMenuScroll and g_SaveMenuAnim in definition order.
// GLOBAL: FF7 0x00dc6b18
int g_SaveMenuUnit = 0;
// GLOBAL: FF7 0x00dc6b1c
int g_SaveMenuSelectedSlot = 0;
unsigned char g_bssPad_00dc6b20[12] = {0};
// GLOBAL: FF7 0x00dc6b2c
int g_SaveMenuCursor = 0;
unsigned char g_bssPad_00dc6b30[12] = {0};
// GLOBAL: FF7 0x00dc6b3c
int g_SaveMenuScroll = 0;
unsigned char g_bssPad_00dc6b40[8] = {0};
// GLOBAL: FF7 0x00dc6b48
int g_SaveMenuAnim = 0;
unsigned char g_bssPad_00dc6b4c[4] = {0};
unsigned char g_bssPad_00dc6b50[280] = {0};
// The confirmation cursor block (Yes/No): MenuCursor_Init and MenuCursor_Update write its fourteen dwords;
// Menu_SaveScreenTick reads the row, element 1.
// GLOBAL: FF7 0x00dc6c68
int g_SaveMenuConfirmCursor[14] = {0};
unsigned char g_bssPad_00dc6ca0[112] = {0};
// GLOBAL: FF7 0x00dc6d10
int g_SaveMenuUnk = 0;
// GLOBAL: FF7 0x00dc6d14
int g_SaveMenuFrameParity = 0;
unsigned char g_bssPad_00dc6d18[1536] = {0};
// GLOBAL: FF7 0x00dc7318
int g_SaveMenuCheck = 0;
unsigned char g_bssPad_00dc731c[4] = {0};
unsigned char g_bssPad_00dc7320[11520] = {0};
// GLOBAL: FF7 0x00dca020
int g_SaveMenuCalledFromField = 0;
// The save slot index Menu_SaveScreenTick works on.
// GLOBAL: FF7 0x00dca024
int g_SaveMenuIndex = 0;
// GLOBAL: FF7 0x00dca028
int g_SaveMenuPhase = 0;
unsigned char g_bssPad_00dca02c[4] = {0};
unsigned char g_bssPad_00dca030[228] = {0};
// GLOBAL: FF7 0x00dca114
int g_SaveMenuActive = 0;
// clang-format on

#pragma intrinsic(abs, memcpy, memset)

/*
 * Loads the portrait of every party member: the three gSavemapParty slots that are
 * not empty (0xFF), each through Menu_LoadCharPortrait(character, slot).
 */
// FUNCTION: FF7 0x006feb20
void __cdecl Menu_LoadPartyPortraitsSaveMenuExit(void)
{
    int slotIndex;
    int character;

    for (slotIndex = 0; slotIndex < 3; slotIndex++) {
        character = gSavemap.party[slotIndex];
        if (character == PARTY_CHAR_NONE) {
            continue;
        }
        Menu_LoadCharPortrait(character, slotIndex);
    }
}

/*
 * Extracts save unit/slot nibbles, builds the preview and checksum, copies the
 * savemap into the selected buffered slot and caches its preview
 * before opening the file: FILE_OPEN_CREATE for a new file, FILE_OPEN_WRITE otherwise. Writes the
 * magic, the cursor/slot byte, the slot-present mask and the 15 slots.
 */
// FUNCTION: FF7 0x006feb6d
unsigned short __cdecl Menu_WriteSaveSlot(int packed)
{
    unsigned int cookie;
    unsigned char head;
    int unit;
    int fh;
    int create;
    int mask;

    fh = 0;
    unit = (packed & 0xf0) >> 4;
    packed = packed & 0xf;
    head = (unsigned char)((g_SaveMenuCursor << 4) | g_SaveMenuSelectedSlot);
    Savemap_BuildPreviewHeader();
    g_GamePauseLatch = 1;
    /* Retail caller treats `Menu_SaveChecksumCrc16` return (`int`) as `unsigned short` for caller zero/sign extension. */
    gSavemap.checksum = ((unsigned short (__cdecl *)(int length, unsigned char *data))Menu_SaveChecksumCrc16)(sizeof(gSavemap) - sizeof(gSavemap.checksum), (unsigned char *)&gSavemap.descLevel);
    memcpy(&gSaveFileBuffer[packed], &gSavemap, sizeof(gSavemap));
    Menu_CacheSaveSlotPreview(0, packed);
    g_GamePauseLatch = 0;
    cookie = SAVE_FILE_MAGIC;
    create = (Menu_SaveFileIsNewlyCreated(unit) != 0) + FILE_OPEN_WRITE;
    // STRING: FF7 0x0091f128
    sprintf((char *)g_LoadMenuSlotBuffer, "%ssave/save%02d.ff7", g_InitPath_AppPath, unit);
    fh = OpenFileSimple(create, g_LoadMenuSlotBuffer);
    if (fh != 0) {
        g_SaveSlotPresentMask |= 1 << packed;
        mask = g_SaveSlotPresentMask;
        WriteFileData(4, &cookie, (struct FileHandle *)fh);
        WriteFileData(1, &head, (struct FileHandle *)fh);
        WriteFileData(4, &mask, (struct FileHandle *)fh);
        WriteFileData(SAVE_FILE_SLOT_COUNT * sizeof(struct Savemap), gSaveFileBuffer, (struct FileHandle *)fh);
        CloseFileEx((struct FileHandle *)fh);
        g_LoadMenuPreviewData[unit * 3] = SAVE_FILE_STATUS_PRESENT;
        return 0;
    }
    return 1;
}

/*
 * Opens the Save menu: resets its state, builds the slot cursor, picks the 1x or 2x frame table,
 * loads the party portraits and scans the save directory. calledFromField selects the field
 * variant (fade, title window).
 */
// FUNCTION: FF7 0x006fecf6
void __cdecl Menu_SaveScreenInit(int calledFromField)
{
    g_SaveMenuState = 0xf0;
    g_SaveMenuCalledFromField = calledFromField;
    g_SaveMenuPhase = SAVE_MENU_PHASE_SELECT_FILE;
    MenuCursor_Init((MenuCursor *)&g_SaveMenuMode, 0, 0, 5, 2, 0, 0, 5, 2, 0, 0, 0, 1,
                    0, 0);
    if (g_MenuIsSmallViewport320x240 != 1) {
        g_SaveMenuFrames = g_SaveMenuFrames2x;
    } else {
        g_SaveMenuFrames = g_SaveMenuFrames1x;
    }
    Menu_PickSavePreviewFrameGfx();
    if (g_SaveMenuCalledFromField != 0) {
        Menu_LoadAllCharPortraits(0);
    } else {
        Menu_LoadAllCharPortraits(2);
    }
    Menu_ScanSaveDirectory(1);
    g_MenuOverlayActive = 1;
}

/*
 * Shuts the Save menu down: releases the nine slot render objects, frees the save
 * file buffer and clears the overlay flag g_MenuOverlayActive.
 */
// FUNCTION: FF7 0x006fed97
void __cdecl Menu_SaveScreenShutdown(void)
{
    Menu_FreeAllCharPortraits();
    Menu_SaveFileBufferFree();
    g_MenuOverlayActive = 0;
}

/*
 * Draws and advances the save menu in 1x or 2x layout, including ten-device
 * selection, three visible slot previews plus a fourth during scrolling, overwrite
 * confirmation, read-progress and delayed save-status windows. Special entry mode
 * fades in/out and supplies its additional header.
 */
// FUNCTION: FF7 0x006fedb0
void __cdecl Menu_SaveScreenTick(unsigned int frameCounter)
{
    struct {
        int offsetY;
        int selected;
        int rows;
        int status;
        int colour;
        int offsetX;
        int width;
        int slot;
        int fileIdx;
        short rect[4];
        int loadResult;
    } v; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
#define SAVE_READY(n) (((signed char *)g_LoadMenuPreviewData)[n])
    if (g_MenuIsSmallViewport320x240 != 1) {
        if (g_SaveMenuCalledFromField == 0)
            Menu_DrawCommandWindow2x(g_MenuModuleId);
        else {
            if (g_SaveMenuFadePhase == SAVE_MENU_FADE_IN) {
                if (Menu_LoadScreenFadeStep(-15) == 0)
                    g_SaveMenuFadePhase = SAVE_MENU_FADE_ACTIVE;
            } else if (g_SaveMenuFadePhase == SAVE_MENU_FADE_OUT) {
                if (Menu_LoadScreenFadeStep(15) == 255)
                    g_SaveMenuFadePhase = SAVE_MENU_FADE_DONE;
            }
        }
        v.selected = 0;
        if ((Menu_GetPhase() == 0 || (g_SaveMenuCalledFromField != 0 && g_SaveMenuFadePhase == SAVE_MENU_FADE_ACTIVE)) &&
            Menu_IsPopupActive() == 0 && g_SaveMenuState != 0)
            g_SaveMenuState--;
        Menu_EmptyTextHook(0x80);
        switch (g_SaveMenuPhase) {
        case SAVE_MENU_PHASE_SELECT_FILE:
            Menu_DrawMoreTextArrow2x(g_SaveMenuSlotWindow2x.x - 25 + g_SaveMenuMode * 91,
                                g_SaveMenuSlotWindow2x.y + 13 + g_SaveMenuRow * 26, 0.2f);
            Menu_DrawEncodedString2x(10, 13, g_SaveLoadTextSelectDataFile, 7, 0.2f);
            v.offsetX = v.offsetY = 0;
            for (v.fileIdx = 0; v.fileIdx < 10; v.fileIdx++) {
                Menu_DrawEncodedString2x(g_SaveMenuSlotWindow2x.x + 20 + v.offsetX,
                                  g_SaveMenuSlotWindow2x.y + 11 + v.offsetY,
                                  (unsigned char *)((int)g_LoadMenuSaveFileNames + v.fileIdx * 12), SAVE_READY(v.fileIdx * 3) == 1 ? 7 : 0, 0.21f);
                v.offsetX += 17 + Menu_MeasureEncodedString2x((unsigned char *)((int)g_LoadMenuSaveFileNames + v.fileIdx * 12));
                if (v.fileIdx == 4) {
                    v.offsetX = 0;
                    v.offsetY = 26;
                }
            }
            Menu_DrawWindowFrame2x(&g_SaveMenuSlotWindow2x, 0.211f);
            break;
        case SAVE_MENU_PHASE_CONFIRM_OVERWRITE:
            Menu_DrawMoreTextArrow2x(g_SaveMenuConfirmWindow2x.x + 35,
                                g_SaveMenuConfirmWindow2x.y + 45 + g_SaveMenuConfirmCursor[1] * 26, 0.0f);
            Menu_DrawEncodedString2x(g_SaveMenuConfirmWindow2x.x + 17, g_SaveMenuConfirmWindow2x.y + 13, g_SaveLoadTextConfirmSave, 7, 0.1f);
            Menu_DrawEncodedString2x(g_SaveMenuConfirmWindow2x.x + 80, g_SaveMenuConfirmWindow2x.y + 41, g_SaveLoadTextYes, 7, 0.1f);
            Menu_DrawEncodedString2x(g_SaveMenuConfirmWindow2x.x + 80, g_SaveMenuConfirmWindow2x.y + 66, g_SaveLoadTextNo, 7, 0.1f);
            Menu_DrawWindowFrame2x(&g_SaveMenuConfirmWindow2x, 0.1f);
            /* The confirmation window overlays the slot list. */
        case SAVE_MENU_PHASE_SELECT_SLOT:
            if (SAVE_READY(0) == 0 || SAVE_READY(3) == 0) {
                g_SaveMenuPhase = SAVE_MENU_PHASE_SELECT_FILE;
                break;
            } else {
                Menu_FlushAndSetScissor2x(0, 51, 640, 411);
                if (g_SaveMenuPhase == SAVE_MENU_PHASE_CONFIRM_OVERWRITE) {
                    if (frameCounter & 2)
                        Menu_DrawMoreTextArrow2x(8, g_SaveMenuSelectedSlot * 137 + 120, 0.0f);
                } else
                    Menu_DrawMoreTextArrow2x(8, g_SaveMenuSelectedSlot * 137 + 120, 0.0f);
                v.rows = (g_SaveMenuAnim != 0) + 3;
                for (v.slot = 0; v.slot < v.rows; v.slot++) {
                    if (g_SaveSlotPresentMask & (1 << (v.slot + g_SaveMenuCursor))) {
                        Menu_SaveWindowColors();
                        Menu_DrawSaveSlotPreview2x(0, v.slot * 137 + 51 + g_SaveMenuScroll * 17, v.slot + g_SaveMenuCursor);
                        Menu_RestoreWindowColors();
                    } else {
                        Menu_DrawEncodedString2x(83, v.slot * 137 + 106 + g_SaveMenuScroll * 17, g_SaveLoadTextEmpty, 6, 0.4f);
                        Menu_CopyRectShorts(v.rect, &g_SaveMenuFrames[2].x);
                        Menu_OffsetRectShorts(v.rect, 0, v.slot * 137 + 51 + g_SaveMenuScroll * 17);
                        Menu_DrawWindowFrame2x((EnglishWindowFrame *)v.rect, 0.4f);
                    }
                }
                Menu_EmptyTextHook(0x80);
                Menu_EmitBatchListsAndInvalidate();
                Menu_FlushAndResetScissor2x();
                Menu_DrawEncodedString2x(10, 13, g_SaveLoadTextSelectGame, 7, 0.0f);
                Menu_DrawEncodedString2x(343, 13, g_SaveLoadTextGame, 6, 0.0f);
                Menu_DrawEncodedString2x(Menu_MeasureEncodedString2x(g_SaveLoadTextGame) + 347, 13,
                                  g_SaveLoadTextRows[g_SaveMenuSelectedSlot + 13 + g_SaveMenuCursor], 7, 0.0f);
                Menu_SetRectShorts(v.rect, 333, 0, 144, 51);
                Menu_DrawWindowFrame2x((EnglishWindowFrame *)v.rect, 0.0f);
            }
            break;
        case SAVE_MENU_PHASE_CHECKING:
        case SAVE_MENU_PHASE_CHECKED:
            if (g_SaveMenuPhase == SAVE_MENU_PHASE_CHECKING)
                v.colour = 0x804020a0;
            else
                v.colour = 0x80e08000;
            Menu_DrawEncodedString2x(10, 13, g_SaveLoadTextChecking, 7, 0.1f);
            if (g_SaveMenuCheck == 0)
                Menu_DrawGaugeBar2x(203, 255, (g_SaveMenuIndex + 1) * 13, 17, v.colour, 0.1f);
            Menu_SetRectShorts(v.rect, 187, 240, 233, 51);
            Menu_DrawWindowFrame2x((EnglishWindowFrame *)v.rect, 0.101f);
            break;
        case SAVE_MENU_PHASE_SAVING:
            v.width = Menu_MeasureEncodedString2x(g_SaveLoadTextSaving);
            Menu_DrawEncodedString2x(320 - (v.width + 27) / 2 + 13, 253, g_SaveLoadTextSaving, 7, 0.1f);
            Menu_SetRectShorts(v.rect, 320 - (v.width + 27) / 2, 240, v.width + 16, 51);
            Menu_DrawWindowFrame2x((EnglishWindowFrame *)v.rect, 0.101f);
            break;
        }
        if (g_SaveMenuCalledFromField != 0) {

            Menu_DrawEncodedString2x(503, 13, g_SaveMenuTitleText, 7, 0.0f);
            Menu_DrawWindowFrame2x(&g_SaveMenuTitleFrame2x, 0.1f);
        }
        Menu_SetRectShorts(v.rect, 0, 0, 640, 51);
        Menu_DrawWindowFrame2x((EnglishWindowFrame *)v.rect, 0.2f);
    } else {
        if (g_SaveMenuCalledFromField == 0)
            Menu_DrawCommandWindow1x(g_MenuModuleId);
        else {
            if (g_SaveMenuFadePhase == SAVE_MENU_FADE_IN) {
                if (Menu_LoadScreenFadeStep(-15) == 0)
                    g_SaveMenuFadePhase = SAVE_MENU_FADE_ACTIVE;
            } else if (g_SaveMenuFadePhase == SAVE_MENU_FADE_OUT) {
                if (Menu_LoadScreenFadeStep(15) == 255)
                    g_SaveMenuFadePhase = SAVE_MENU_FADE_DONE;
            }
        }
        v.selected = 0;
        if ((Menu_GetPhase() == 0 || (g_SaveMenuCalledFromField != 0 && g_SaveMenuFadePhase == SAVE_MENU_FADE_ACTIVE)) &&
            Menu_IsPopupActive() == 0 && g_SaveMenuState != 0)
            g_SaveMenuState--;
        Menu_EmptyTextHook(0x80);
        switch (g_SaveMenuPhase) {
        case SAVE_MENU_PHASE_SELECT_FILE:
            Menu_DrawMoreTextArrow1x(g_SaveMenuSlotWindow1x.x - 13 + g_SaveMenuMode * 43,
                                g_SaveMenuSlotWindow1x.y + 7 + g_SaveMenuRow * 13, 0.2f);
            Menu_DrawEncodedString1x(10, 7, g_SaveLoadTextSelectDataFile, 7, 0.2f);
            v.offsetX = v.offsetY = 0;
            for (v.fileIdx = 0; v.fileIdx < 10; v.fileIdx++) {
                Menu_DrawEncodedString1x(g_SaveMenuSlotWindow1x.x + 10 + v.offsetX,
                                     g_SaveMenuSlotWindow1x.y + 6 + v.offsetY,
                                     (unsigned char *)((int)g_LoadMenuSaveFileNames + v.fileIdx * 12), SAVE_READY(v.fileIdx * 3) == 1 ? 7 : 0, 0.21f);
                v.offsetX += 9 + Menu_MeasureEncodedString1x((unsigned char *)((int)g_LoadMenuSaveFileNames + v.fileIdx * 12));
                if (v.fileIdx == 4) {
                    v.offsetX = 0;
                    v.offsetY = 13;
                }
            }
            Menu_DrawWindowFrame1x(&g_SaveMenuSlotWindow1x, 0.211f);
            break;
        case SAVE_MENU_PHASE_CONFIRM_OVERWRITE:
            Menu_DrawMoreTextArrow1x(g_SaveMenuConfirmWindow1x.x + 18,
                                g_SaveMenuConfirmWindow1x.y + 23 + g_SaveMenuConfirmCursor[1] * 13, 0.0f);
            Menu_DrawEncodedString1x(g_SaveMenuConfirmWindow1x.x + 9, g_SaveMenuConfirmWindow1x.y + 7, g_SaveLoadTextConfirmSave, 7, 0.1f);
            Menu_DrawEncodedString1x(g_SaveMenuConfirmWindow1x.x + 40, g_SaveMenuConfirmWindow1x.y + 21, g_SaveLoadTextYes, 7, 0.1f);
            Menu_DrawEncodedString1x(g_SaveMenuConfirmWindow1x.x + 40, g_SaveMenuConfirmWindow1x.y + 33, g_SaveLoadTextNo, 7, 0.1f);
            Menu_DrawWindowFrame1x(&g_SaveMenuConfirmWindow1x, 0.1f);
            /* The confirmation window overlays the slot list. */
        case SAVE_MENU_PHASE_SELECT_SLOT:
            if (SAVE_READY(0) == 0 || SAVE_READY(3) == 0) {
                g_SaveMenuPhase = SAVE_MENU_PHASE_SELECT_FILE;
                break;
            } else {
                Menu_FlushAndSetScissor1x(0, 26, 320, 206);
                if (g_SaveMenuPhase == SAVE_MENU_PHASE_CONFIRM_OVERWRITE) {
                    if (frameCounter & 2)
                        Menu_DrawMoreTextArrow1x(4, g_SaveMenuSelectedSlot * 69 + 60, 0.0f);
                } else
                    Menu_DrawMoreTextArrow1x(4, g_SaveMenuSelectedSlot * 69 + 60, 0.0f);
                v.rows = (g_SaveMenuAnim != 0) + 3;
                for (v.slot = 0; v.slot < v.rows; v.slot++) {
                    if (g_SaveSlotPresentMask & (1 << (v.slot + g_SaveMenuCursor))) {
                        Menu_SaveWindowColors();
                        Menu_DrawSaveSlotPreview1x(0, v.slot * 69 + 26 + g_SaveMenuScroll * 8, v.slot + g_SaveMenuCursor);
                        Menu_RestoreWindowColors();
                    } else {
                        Menu_DrawEncodedString1x(42, v.slot * 69 + 47 + g_SaveMenuScroll * 8, g_SaveLoadTextEmpty, 6, 0.4f);
                        Menu_CopyRectShorts(v.rect, &g_SaveMenuFrames[2].x);
                        Menu_OffsetRectShorts(v.rect, 0, v.slot * 69 + 26 + g_SaveMenuScroll * 8);
                        Menu_DrawWindowFrame1x((EnglishWindowFrame *)v.rect, 0.4f);
                    }
                }
                Menu_EmptyTextHook(0x80);
                Menu_EmitBatchListsAndInvalidate();
                Menu_FlushAndResetScissor1x();
                Menu_DrawEncodedString1x(5, 7, g_SaveLoadTextSelectGame, 7, 0.0f);
                Menu_DrawEncodedString1x(184, 7, g_SaveLoadTextGame, 6, 0.0f);
                Menu_DrawEncodedString1x(Menu_MeasureEncodedString1x(g_SaveLoadTextGame) + 186, 7,
                                     g_SaveLoadTextRows[g_SaveMenuSelectedSlot + 13 + g_SaveMenuCursor], 7, 0.0f);
                Menu_SetRectShorts(v.rect, 179, 0, 67, 26);
                Menu_DrawWindowFrame1x((EnglishWindowFrame *)v.rect, 0.0f);
            }
            break;
        case SAVE_MENU_PHASE_CHECKING:
        case SAVE_MENU_PHASE_CHECKED:
            if (g_SaveMenuPhase == SAVE_MENU_PHASE_CHECKING)
                v.colour = 0x804020a0;
            else
                v.colour = 0x80e08000;
            Menu_DrawEncodedString1x(5, 7, g_SaveLoadTextChecking, 7, 0.1f);
            if (g_SaveMenuCheck == 0)
                Menu_DrawGaugeBar1x(102, 128, (g_SaveMenuIndex + 1) * 7, 9, v.colour, 0.1f);
            Menu_SetRectShorts(v.rect, 94, 120, 117, 26);
            Menu_DrawWindowFrame1x((EnglishWindowFrame *)v.rect, 0.101f);
            break;
        case SAVE_MENU_PHASE_SAVING:
            v.width = Menu_MeasureEncodedString1x(g_SaveLoadTextSaving);
            Menu_DrawEncodedString1x(160 - (v.width + 14) / 2 + 7, 127, g_SaveLoadTextSaving, 7, 0.1f);
            Menu_SetRectShorts(v.rect, 160 - (v.width + 14) / 2, 120, v.width + 8, 26);
            Menu_DrawWindowFrame1x((EnglishWindowFrame *)v.rect, 0.101f);
            break;
        }
        if (g_SaveMenuCalledFromField != 0) {
            Menu_EmptyTextHook(0x80);
            Menu_DrawEncodedString1x(260, 7, g_SaveMenuTitleText, 7, 0.0f);
            Menu_DrawWindowFrame1x(&g_SaveMenuTitleFrame1x, 0.1f);
        }
        Menu_SetRectShorts(v.rect, 0, 0, 320, 26);
        Menu_DrawWindowFrame1x((EnglishWindowFrame *)v.rect, 0.2f);
    }

    if (((g_SaveMenuCalledFromField == 0 && Menu_GetPhase() == 0) ||
         (g_SaveMenuCalledFromField != 0 && g_SaveMenuFadePhase == SAVE_MENU_FADE_ACTIVE)) &&
        Menu_IsPopupActive() == 0) {
        switch (g_SaveMenuPhase) {
        case SAVE_MENU_PHASE_SELECT_FILE:
            if (Input_GetPressed(INPUT_CIRCLE)) {
                if (SAVE_READY((g_SaveMenuRow * 5 + g_SaveMenuMode) * 3) != 0) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    g_SaveMenuIndex = 0;
                    g_SaveMenuUnk = g_SaveMenuIndex;
                    v.loadResult = Menu_LoadSaveFileToBuffer(SAVE_FILE_LOAD_OR_NEW, g_SaveMenuRow * 5 + g_SaveMenuMode);
                    if (v.loadResult == SAVE_FILE_LOAD_OK) {
                        g_SaveMenuPhase = SAVE_MENU_PHASE_CHECKING;
                        g_SaveMenuCheck = 1;
                        g_SaveMenuDelay = 60;
                        MenuCursor_Init((MenuCursor *)&g_SaveMenuUnit, 0, 0, 1, 3, 0, 0, 1, 15, 0, 0, 0, 0, 0, 1);
                    } else {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                        switch (v.loadResult) {
                        case SAVE_FILE_LOAD_NO_MEMORY:
                            Menu_OpenPopupMessage(g_SaveLoadTextAllocFailed, 7);
                            break;
                        case SAVE_FILE_LOAD_BAD_MAGIC:
                            Menu_OpenPopupMessage(g_SaveLoadTextInvalidSaveFile, 7);
                            break;
                        }
                    }
                } else {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                    if (g_SaveMenuState == 0)
                        Menu_OpenPopupMessage(g_SaveLoadTextNoSaveFile2, 7);
                    else
                        Menu_OpenPopupMessage(g_SaveLoadTextNoSaveFile, 7);
                }
                v.selected = 1;
            } else {
                MenuCursor_Update((MenuCursor *)&g_SaveMenuMode);
                if (g_SaveMenuCalledFromField != 0) {
                    if (Input_GetPressed(INPUT_CROSS)) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                        g_SaveMenuFadePhase = SAVE_MENU_FADE_OUT;
                        if (Menu_IsTextSubsystemReady())
                            Menu_SetRequestedScreen(0);
                    }
                } else {
                    if (Input_GetPressed(INPUT_CROSS)) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                        Menu_SaveScreenShutdown();
                        Menu_LoadPartyPortraitsSaveMenuExit();
                        Menu_SetPhase(5, 0);
                        Menu_SetScreen(0);
                        v.selected = 1;
                    }
                }
            }
            break;
        case SAVE_MENU_PHASE_SELECT_SLOT:
            v.status = g_SaveMenuScroll;
            MenuCursor_UpdateScrolling((MenuCursor *)&g_SaveMenuUnit);
            if (g_SaveMenuScroll == 0 && v.status == 0) {
                if (Input_GetPressed(INPUT_CIRCLE)) {
                    g_SaveMenuPhase = SAVE_MENU_PHASE_CONFIRM_OVERWRITE;
                    MenuCursor_Init((MenuCursor *)g_SaveMenuConfirmCursor, 0, 0, 1, 2, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0);
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                } else if (Input_GetPressed(INPUT_CROSS)) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                    Menu_SaveFileBufferFree();
                    g_SaveMenuPhase = SAVE_MENU_PHASE_SELECT_FILE;
                }
            }
            break;
        case SAVE_MENU_PHASE_CHECKING:
            if (g_SaveMenuDelay != 0) {
                if (g_SaveMenuDelay != 0)
                    g_SaveMenuDelay--;
            } else {
                if (g_SaveMenuCheck != 0) {
                    g_SaveMenuDelay = 0;
                    g_SaveMenuCheck = g_SaveMenuDelay;
                } else {
                    if (g_SaveSlotPresentMask & (1 << g_SaveMenuIndex))
                        v.status = Menu_CacheSaveSlotPreview(g_SaveMenuRow, g_SaveMenuIndex);
                    else
                        v.status = 0;
                    g_SaveMenuIndex++;
                    if (v.status != 0) {
                        g_SaveMenuPhase = SAVE_MENU_PHASE_SELECT_FILE;
                        Menu_OpenPopupMessage(g_SaveLoadTextCouldNotRead, 2);
                    }
                    if (g_SaveMenuIndex == SAVE_FILE_SLOT_COUNT) {
                        g_SaveMenuIndex = 14;
                        g_SaveMenuPhase = SAVE_MENU_PHASE_CHECKED;
                        g_SaveMenuDelay = 10;
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CONFIRM);
                    }
                }
            }
            break;
        case SAVE_MENU_PHASE_CHECKED:
            if (g_SaveMenuDelay == 0)
                g_SaveMenuPhase = SAVE_MENU_PHASE_SELECT_SLOT;
            g_SaveMenuDelay--;
            break;
        case SAVE_MENU_PHASE_SAVING:
            if (g_SaveMenuDelay != 0)
                g_SaveMenuDelay--;
            else {
                g_SaveMenuPhase = SAVE_MENU_PHASE_SELECT_SLOT;
                v.status = (short)Menu_WriteSaveSlot((g_SaveMenuSelectedSlot + g_SaveMenuCursor) | ((g_SaveMenuRow * 5 + g_SaveMenuMode) << 4));
                if (v.status == 0) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_SAVE_LOAD);
                    Menu_OpenPopupMessage(g_SaveLoadTextSaved, 7);
                    g_SaveSlotPresentMask |= 1 << (g_SaveMenuSelectedSlot + g_SaveMenuCursor);
                } else {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                    Menu_OpenPopupMessage(g_SaveLoadTextNotEnoughSpace, 7);
                }
            }
            break;
        case SAVE_MENU_PHASE_CONFIRM_OVERWRITE:
            if (Input_GetPressed(INPUT_CIRCLE)) {
                switch (g_SaveMenuConfirmCursor[1]) {
                case SAVE_MENU_CONFIRM_YES:
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    g_SaveMenuPhase = SAVE_MENU_PHASE_SAVING;
                    g_SaveMenuDelay = 10;
                    break;
                case SAVE_MENU_CONFIRM_NO:
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                    g_SaveMenuPhase = SAVE_MENU_PHASE_SELECT_SLOT;
                    break;
                }
            } else if (Input_GetPressed(INPUT_CROSS)) {
                g_SaveMenuPhase = SAVE_MENU_PHASE_SELECT_SLOT;
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
            } else
                MenuCursor_Update((MenuCursor *)g_SaveMenuConfirmCursor);
            break;
        }
    }
#undef SAVE_READY
}

/*
 * The save screen's module tick, with its own first-frame setup folded in; the same shape as
 * Menu_NameEntryModuleUpdate. Always returns 0, which the module dispatcher ignores.
 */
// FUNCTION: FF7 0x006fffaa
int __cdecl Menu_SaveScreenModuleUpdate(void)
{
    if (g_SaveMenuActive == 0) {
        g_SaveMenuFadePhase = SAVE_MENU_FADE_IN;
        g_SaveMenuFrameCounter = g_SaveMenuFadePhase;
        Menu_SaveScreenInit(1);
        g_SaveMenuFrameParity = 0;
        g_MenuModuleId = 0xa;
        Input_SetRepeatRates(0xc8, 0x32);
        g_MenuOverlayActive = 1;
        g_SaveMenuActive = g_MenuOverlayActive;
    }
    if (g_MenuIsSmallViewport320x240 != 1) {
        Menu_DrawPopupMessage2x();
    } else {
        Menu_DrawPopupMessage1x();
    }
    Menu_SaveScreenTick(g_SaveMenuFrameCounter);
    if (g_SaveMenuFadePhase == SAVE_MENU_FADE_DONE) {
        g_MenuOverlayActive = 0;
        g_SaveMenuActive = g_MenuOverlayActive;
    }
    g_SaveMenuFrameParity ^= 1;
    g_SaveMenuFrameCounter += 1;
    if (g_SaveMenuActive != 1) {
        Menu_SaveScreenShutdown();
        g_MenuExitRequested = 1;
    }
    return 0;
}
} /* extern "C" */
