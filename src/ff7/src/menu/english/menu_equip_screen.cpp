// src/ff7/src/menu/english/menu_equip_screen.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The equipment menu screen: its setup, per-frame drawing and input, the materia slot
//   rows and stat rows that preview a candidate piece of gear in both scales, and the build of the
//   candidate inventory list.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/menu.h"
#include "ff7/enums/menu.h"
#include "lib/enums/file.h"
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/file.h"
#include "lib/api/sound.h"
#include <string.h>
#include <stdlib.h>
#include "ff7/savemap.h"
#include "lib/api/graphics.h"
#include "ff7/enums/main.h"
extern "C" {

struct EnglishWindowFrame;

// clang-format off
// clang-format on

// Text: "Wpn." "Arm." "Acc." "Acc." "Strength" "Dexterity" "Vitality" "Spirit" "Magic" "Luck"
//       "Attack" "Attack %" "Defense" "Defense %" "Magic atk" "Magic def" "Magic def%" "Slot"
//       "Growth" "Nothing" "Normal" "Double" "Triple"
// GLOBAL: FF7 0x009209a8
unsigned char g_MenuEquipLabelTable[288] = {0x37, 0x50, 0x4e, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x52, 0x4d, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x43, 0x43, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x43, 0x43, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x54, 0x52, 0x45, 0x4e, 0x47, 0x54, 0x48, 0xff, 0x00, 0x00, 0x00, 0x24, 0x45, 0x58, 0x54, 0x45, 0x52, 0x49, 0x54, 0x59, 0xff, 0x00, 0x00, 0x36, 0x49, 0x54, 0x41, 0x4c, 0x49, 0x54, 0x59, 0xff, 0x00, 0x00, 0x00, 0x33, 0x50, 0x49, 0x52, 0x49, 0x54, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2d, 0x41, 0x47, 0x49, 0x43, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2c, 0x55, 0x43, 0x4b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x54, 0x54, 0x41, 0x43, 0x4b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x54, 0x54, 0x41, 0x43, 0x4b, 0x00, 0x05, 0xff, 0x00, 0x00, 0x00, 0x24, 0x45, 0x46, 0x45, 0x4e, 0x53, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x24, 0x45, 0x46, 0x45, 0x4e, 0x53, 0x45, 0x00, 0x05, 0xff, 0x00, 0x00, 0x2d, 0x41, 0x47, 0x49, 0x43, 0x00, 0x41, 0x54, 0x4b, 0xff, 0x00, 0x00, 0x2d, 0x41, 0x47, 0x49, 0x43, 0x00, 0x44, 0x45, 0x46, 0xff, 0x00, 0x00, 0x2d, 0x41, 0x47, 0x49, 0x43, 0x00, 0x44, 0x45, 0x46, 0x05, 0xff, 0x00, 0x33, 0x4c, 0x4f, 0x54, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x27, 0x52, 0x4f, 0x57, 0x54, 0x48, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2e, 0x4f, 0x54, 0x48, 0x49, 0x4e, 0x47, 0xff, 0x00, 0x00, 0x00, 0x00, 0x2e, 0x4f, 0x52, 0x4d, 0x41, 0x4c, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x4f, 0x55, 0x42, 0x4c, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x52, 0x49, 0x50, 0x4c, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00920ac8
struct EnglishWindowFrame g_MenuEquipWindowRects2x[5] = {{0, 0, 640, 120}, {0, 120, 640, 51}, {0, 163, 400, 111}, {0, 257, 400, 223}, {393, 163, 247, 317}};
// GLOBAL: FF7 0x00920af0
struct EnglishWindowFrame g_MenuEquipWindowRects1x[5] = {{0, 0, 320, 60}, {0, 60, 320, 26}, {0, 82, 200, 56}, {0, 128, 200, 112}, {197, 81, 123, 159}};
// clang-format off
// GLOBAL: FF7 0x00dca490
struct MenuEquipScreenState g_MenuEquipState = {0};
// clang-format on
#pragma intrinsic(abs, memcpy, memset)
#define gSavemapItems (gSavemap.items)
#define gSavemapParty (gSavemap.party)

void __cdecl Savemap_AddItem(int itemRef);

// clang-format off
// GLOBAL: FF7 0x00dca4a0
EnglishWindowFrame *g_MenuEquipRectPtr = 0;
// GLOBAL: FF7 0x00dca4a4
int g_MenuEquipCharIndex = 0;
unsigned char g_bssPad_00dca4a8[280] = {0};
// GLOBAL: FF7 0x00dca5c0
int g_MenuEquipCursor0[0xe] = {0};
// GLOBAL: FF7 0x00dca5f8
int g_MenuEquipCursor1[0xe] = {0};
unsigned char g_bssPad_00dca630[112] = {0};
// GLOBAL: FF7 0x00dca6a0
int g_MenuEquipSelectedSlot = 0;
unsigned char g_bssPad_00dca6a4[4] = {0};
// GLOBAL: FF7 0x00dca6a8
unsigned char g_MenuEquipBuffer[0x30] = {0};
unsigned char g_bssPad_00dca6d8[272] = {0};
// GLOBAL: FF7 0x00dca7e8
int g_MenuEquipCount = 0;
// GLOBAL: FF7 0x00dca7ec
int g_MenuEquipCandidateCount = 0;
unsigned char g_bssPad_00dca7f0[8] = {0};
// clang-format on

/*
 * Initializes equipment menu screen, cursor models, and candidate list.
 */
// FUNCTION: FF7 0x00705c80
void __cdecl Menu_EquipScreenInit(int arg)
{
    g_MenuEquipSelectedSlot = MENU_EQUIP_FOCUS_KIND;
    g_MenuEquipCount = g_MenuEquipSelectedSlot;
    g_MenuEquipCharIndex = g_MenuPartyMemberIndex;
    MenuCursor_Init((MenuCursor *)g_MenuEquipCursor0, 0, 0, 1, 3, 0, 0, 1, 3, 0, 0, 0, 1, 0,
                    0);
    MenuCursor_Init((MenuCursor *)g_MenuEquipCursor1, 0, 0, 1, 8, 0, 0, 1, 0x10, 0, 0, 0, 0,
                    0, 1);
    if (g_MenuIsSmallViewport320x240 != 1) {
        g_MenuEquipRectPtr = g_MenuEquipWindowRects2x;
    } else {
        g_MenuEquipRectPtr = g_MenuEquipWindowRects1x;
    }
}

/*
 * Updates and renders equipment menu screen frame and handles user input.
 */
// FUNCTION: FF7 0x00705d16
void __cdecl Menu_EquipScreenFrame(unsigned int frameCounter)
{
    /* Retail call was unprototyped and pushes 32-bit int rather than narrow char/short; local override preserves push width. */
    void __cdecl Savemap_AddMateriaToStock(int);
    /* Unprototyped retail call signature (parameter width differs from callee definition). */
    int __cdecl Savemap_GetPartyArmourRecord(int);
    struct {
        int oldItem;
        int record;
        int growth;
        int item;
        int i;
        int rows;
        int j;
        unsigned char *slots;
        struct SavemapCharacter *records;
    } v; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
#define EQ_TYPE (g_MenuEquipCursor0[1])
#define EQ_ROW (g_MenuEquipCursor1[1])
#define EQ_TOP (g_MenuEquipCursor1[5])
#define EQ_SCROLL (g_MenuEquipCursor1[9])
#define EQ_MOVING (g_MenuEquipCursor1[12])
#define EQ_RECORD (g_CharIdToSaveSlot[gSavemapParty[g_MenuEquipCharIndex]])
#define EQ_GROWTH_TEXT (&g_MenuEquipLabelTable[(v.growth + MENU_EQUIP_LABEL_NOTHING) * 12])
    if (g_MenuIsSmallViewport320x240 != 1) {
        Menu_DrawCommandWindow2x(g_MenuModuleId);
        v.record = EQ_RECORD;
        v.records = (struct SavemapCharacter *)&gSavemap.chars[0];
        if (g_MenuEquipSelectedSlot == MENU_EQUIP_FOCUS_LIST) {
            Menu_FlushAndSetScissor2x(316, 171, 324, 303);
            if (g_MenuEquipCandidateCount > 8) {
                g_MenuEquipState.visibleRows = 8;
                g_MenuEquipState.candidateCount = (short)g_MenuEquipCandidateCount;
                g_MenuEquipState.listTop = (short)EQ_TOP;
                g_MenuEquipState.frameWidth = 618;
                g_MenuEquipState.frameHeight = 171;
                g_MenuEquipState.rowHeight = 17;
                g_MenuEquipState.listWidth = 303;
                Menu_DrawScrollBar2x(&g_MenuEquipState.visibleRows, 0.2f);
            }
            v.rows = g_MenuEquipCandidateCount > 8 ? 8 : g_MenuEquipCandidateCount;
            if (EQ_MOVING != 0)
                v.rows++;
            for (v.i = 0; v.i < v.rows; v.i++) {
                switch (EQ_TYPE) {
                case EQUIP_WEAPON:
                    v.item = g_MenuEquipBuffer[v.i + EQ_TOP] + 128;
                    break;
                case EQUIP_ARMOR:
                    v.item = g_MenuEquipBuffer[v.i + EQ_TOP] + 256;
                    break;
                case EQUIP_ACCESSORY:
                    v.item = g_MenuEquipBuffer[v.i + EQ_TOP] + 288;
                    break;
                }
                Menu_DrawEncodedString2x(427, v.i * 36 + 193 + EQ_SCROLL * 9, Kernel_LookupItemNameText(v.item), 7, 0.2f);
            }
            Menu_FlushAndResetScissor2x();
        }
        switch (g_MenuEquipSelectedSlot) {
        case MENU_EQUIP_FOCUS_KIND:
            Menu_DrawMoreTextArrow2x(207, EQ_TYPE * 36 + 17, 0.101f);
            if (EQ_TYPE != EQUIP_ACCESSORY) {
                Menu_DrawEncodedString2x(27, g_MenuEquipRectPtr[2].y + 21, &g_MenuEquipLabelTable[MENU_EQUIP_LABEL_SLOT * 12], 5, 0.101f);
                Menu_DrawEncodedString2x(27, g_MenuEquipRectPtr[2].y + 64, &g_MenuEquipLabelTable[MENU_EQUIP_LABEL_GROWTH * 12], 5, 0.101f);
                switch (EQ_TYPE) {
                case EQUIP_WEAPON:
                    v.item = gSavemap.chars[EQ_RECORD].weapon;
                    Menu_DrawEquipMateriaSlotRow2x(153, g_MenuEquipRectPtr[2].y + 21, g_KernelWeaponDataArray[v.item].materiaSlotTable);
                    v.growth = g_KernelWeaponDataArray[v.item].materiaGrowth[0];
                    break;
                case EQUIP_ARMOR:
                    v.item = gSavemap.chars[EQ_RECORD].armor;
                    Menu_DrawEquipMateriaSlotRow2x(153, g_MenuEquipRectPtr[2].y + 21, g_KernelArmorDataArray[v.item].materiaSlotBytes);
                    v.growth = g_KernelArmorDataArray[v.item].materiaSlotBytes[8];
                    break;
                }
                switch (v.growth) {
                case GROWTH_NONE:
                case GROWTH_NORMAL:
                case GROWTH_DOUBLE:
                case GROWTH_TRIPLE:
                    break;
                default:
                    v.growth = 4;
                    break;
                }
                Menu_DrawEncodedString2x(243 - Menu_MeasureEncodedString2x(EQ_GROWTH_TEXT) / 2, g_MenuEquipRectPtr[2].y + 64, EQ_GROWTH_TEXT, 7, 0.2f);
            }
            break;
        case MENU_EQUIP_FOCUS_LIST:
            if (frameCounter & 2)
                Menu_DrawMoreTextArrow2x(207, EQ_TYPE * 36 + 17, 0.101f);
            Menu_DrawMoreTextArrow2x(385, EQ_ROW * 36 + 197, 0.101f);
            if (EQ_TYPE != EQUIP_ACCESSORY) {
                Menu_DrawEncodedString2x(27, g_MenuEquipRectPtr[2].y + 21, &g_MenuEquipLabelTable[MENU_EQUIP_LABEL_SLOT * 12], 5, 0.101f);
                Menu_DrawEncodedString2x(27, g_MenuEquipRectPtr[2].y + 64, &g_MenuEquipLabelTable[MENU_EQUIP_LABEL_GROWTH * 12], 5, 0.101f);
                switch (EQ_TYPE) {
                case EQUIP_WEAPON:
                    v.item = g_MenuEquipBuffer[EQ_ROW + EQ_TOP];
                    Menu_DrawEquipMateriaSlotRow2x(153, g_MenuEquipRectPtr[2].y + 21, g_KernelWeaponDataArray[v.item].materiaSlotTable);
                    v.growth = g_KernelWeaponDataArray[v.item].materiaGrowth[0];
                    break;
                case EQUIP_ARMOR:
                    v.item = g_MenuEquipBuffer[EQ_ROW + EQ_TOP];
                    Menu_DrawEquipMateriaSlotRow2x(153, g_MenuEquipRectPtr[2].y + 21, g_KernelArmorDataArray[v.item].materiaSlotBytes);
                    v.growth = g_KernelArmorDataArray[v.item].materiaSlotBytes[8];
                    break;
                }
                switch (v.growth) {
                case GROWTH_NONE:
                case GROWTH_NORMAL:
                case GROWTH_DOUBLE:
                case GROWTH_TRIPLE:
                    break;
                default:
                    v.growth = 4;
                    break;
                }
                Menu_DrawEncodedString2x(243 - Menu_MeasureEncodedString2x(EQ_GROWTH_TEXT) / 2, g_MenuEquipRectPtr[2].y + 64, EQ_GROWTH_TEXT, 7, 0.2f);
            }
            break;
        }
        for (v.i = 0; v.i < 3; v.i++)
            Menu_DrawEncodedString2x(250, v.i * 34 + 13, &g_MenuEquipLabelTable[v.i * 12], 5, 0.2f);
        Menu_DrawEquipStatRows(53, g_MenuEquipRectPtr[3].y + 26);
        Menu_DrawCharStatBlock2x(110, 17, g_MenuEquipCharIndex, 0.2f);
        Menu_DrawCharPortrait2x(17, 9, g_MenuEquipCharIndex, 0.2f);
        Menu_DrawEncodedString2x(303, 13, Kernel_LookupItemNameText(v.records[v.record].weapon + 128), 7, 0.2f);
        Menu_DrawEncodedString2x(303, 47, Kernel_LookupItemNameText(v.records[v.record].armor + 256), 7, 0.2f);
        if (v.records[v.record].accessory != 255)
            Menu_DrawEncodedString2x(303, 81, Kernel_LookupItemNameText(v.records[v.record].accessory + 288), 7, 0.2f);
        if (g_MenuEquipSelectedSlot == MENU_EQUIP_FOCUS_LIST) {
            switch (EQ_TYPE) {
            case EQUIP_WEAPON:
                Menu_DrawEncodedString2x(24, g_MenuEquipRectPtr[1].y + 13, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM, g_MenuEquipBuffer[EQ_TOP + EQ_ROW] + 128, 0), 7, 0.2f);
                break;
            case EQUIP_ARMOR:
                Menu_DrawEncodedString2x(24, g_MenuEquipRectPtr[1].y + 13, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM, g_MenuEquipBuffer[EQ_TOP + EQ_ROW] + 256, 0), 7, 0.2f);
                break;
            case EQUIP_ACCESSORY:
                Menu_DrawEncodedString2x(24, g_MenuEquipRectPtr[1].y + 13, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM, g_MenuEquipBuffer[EQ_TOP + EQ_ROW] + 288, 0), 7, 0.2f);
                break;
            }
        } else {
            switch (EQ_TYPE) {
            case EQUIP_WEAPON:
                Menu_DrawEncodedString2x(24, g_MenuEquipRectPtr[1].y + 13, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM, gSavemap.chars[EQ_RECORD].weapon + 128, 0), 7, 0.2f);
                break;
            case EQUIP_ARMOR:
                Menu_DrawEncodedString2x(24, g_MenuEquipRectPtr[1].y + 13, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM, gSavemap.chars[EQ_RECORD].armor + 256, 0), 7, 0.2f);
                break;
            case EQUIP_ACCESSORY:
                if (gSavemap.chars[EQ_RECORD].accessory != 255)
                    Menu_DrawEncodedString2x(24, g_MenuEquipRectPtr[1].y + 13, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM, gSavemap.chars[EQ_RECORD].accessory + 288, 0), 7, 0.2f);
                break;
            }
        }
        for (v.j = 0; v.j < 2; v.j++)
            Menu_DrawWindowFrame2x(&g_MenuEquipRectPtr[v.j], 0.3f);
        Menu_DrawWindowFrame2x(&g_MenuEquipRectPtr[2], 0.4f);
        Menu_DrawWindowFrame2x(&g_MenuEquipRectPtr[3], 0.5f);
        Menu_DrawWindowFrame2x(&g_MenuEquipRectPtr[4], 0.6f);
    } else {
        Menu_DrawCommandWindow1x(g_MenuModuleId);
        v.record = EQ_RECORD;
        v.records = (struct SavemapCharacter *)&gSavemap.chars[0];
        if (g_MenuEquipSelectedSlot == MENU_EQUIP_FOCUS_LIST) {
            Menu_FlushAndSetScissor1x(158, 86, 162, 152);
            if (g_MenuEquipCandidateCount > 8) {
                g_MenuEquipState.visibleRows = 8;
                g_MenuEquipState.candidateCount = (short)g_MenuEquipCandidateCount;
                g_MenuEquipState.listTop = (short)EQ_TOP;
                g_MenuEquipState.frameWidth = 309;
                g_MenuEquipState.frameHeight = 86;
                g_MenuEquipState.rowHeight = 9;
                g_MenuEquipState.listWidth = 152;
                Menu_DrawScrollBar1x(&g_MenuEquipState.visibleRows, 0.2f);
            }
            Menu_EmptyTextHook(9);
            v.rows = g_MenuEquipCandidateCount > 8 ? 8 : g_MenuEquipCandidateCount;
            if (EQ_MOVING != 0)
                v.rows++;
            for (v.i = 0; v.i < v.rows; v.i++) {
                switch (EQ_TYPE) {
                case EQUIP_WEAPON:
                    v.item = g_MenuEquipBuffer[v.i + EQ_TOP] + 128;
                    break;
                case EQUIP_ARMOR:
                    v.item = g_MenuEquipBuffer[v.i + EQ_TOP] + 256;
                    break;
                case EQUIP_ACCESSORY:
                    v.item = g_MenuEquipBuffer[v.i + EQ_TOP] + 288;
                    break;
                }
                Menu_DrawEncodedString1x(214, v.i * 18 + 97 + EQ_SCROLL * 5, Kernel_LookupItemNameText(v.item), 7, 0.2f);
            }
            Menu_FlushAndResetScissor1x();
        }
        switch (g_MenuEquipSelectedSlot) {
        case MENU_EQUIP_FOCUS_KIND:
            Menu_DrawMoreTextArrow1x(104, EQ_TYPE * 18 + 9, 0.101f);
            if (EQ_TYPE != EQUIP_ACCESSORY) {
                Menu_DrawEncodedString1x(14, g_MenuEquipRectPtr[2].y + 11, &g_MenuEquipLabelTable[MENU_EQUIP_LABEL_SLOT * 12], 5, 0.101f);
                Menu_DrawEncodedString1x(14, g_MenuEquipRectPtr[2].y + 32, &g_MenuEquipLabelTable[MENU_EQUIP_LABEL_GROWTH * 12], 5, 0.101f);
                switch (EQ_TYPE) {
                case EQUIP_WEAPON:
                    v.item = gSavemap.chars[EQ_RECORD].weapon;
                    Menu_DrawEquipMateriaSlotRow1x(77, g_MenuEquipRectPtr[2].y + 11, g_KernelWeaponDataArray[v.item].materiaSlotTable);
                    v.growth = g_KernelWeaponDataArray[v.item].materiaGrowth[0];
                    break;
                case EQUIP_ARMOR:
                    v.item = gSavemap.chars[EQ_RECORD].armor;
                    Menu_DrawEquipMateriaSlotRow1x(77, g_MenuEquipRectPtr[2].y + 11, g_KernelArmorDataArray[v.item].materiaSlotBytes);
                    v.growth = g_KernelArmorDataArray[v.item].materiaSlotBytes[8];
                    break;
                }
                switch (v.growth) {
                case GROWTH_NONE:
                case GROWTH_NORMAL:
                case GROWTH_DOUBLE:
                case GROWTH_TRIPLE:
                    break;
                default:
                    v.growth = 4;
                    break;
                }
                Menu_DrawEncodedString1x(122 - Menu_MeasureEncodedString1x(EQ_GROWTH_TEXT) / 2, g_MenuEquipRectPtr[2].y + 32, EQ_GROWTH_TEXT, 7, 0.2f);
            }
            break;
        case MENU_EQUIP_FOCUS_LIST:
            if (frameCounter & 2)
                Menu_DrawMoreTextArrow1x(104, EQ_TYPE * 18 + 9, 0.101f);
            Menu_DrawMoreTextArrow1x(193, EQ_ROW * 18 + 99, 0.101f);
            if (EQ_TYPE != EQUIP_ACCESSORY) {
                Menu_DrawEncodedString1x(14, g_MenuEquipRectPtr[2].y + 11, &g_MenuEquipLabelTable[MENU_EQUIP_LABEL_SLOT * 12], 5, 0.101f);
                Menu_DrawEncodedString1x(14, g_MenuEquipRectPtr[2].y + 32, &g_MenuEquipLabelTable[MENU_EQUIP_LABEL_GROWTH * 12], 5, 0.101f);
                switch (EQ_TYPE) {
                case EQUIP_WEAPON:
                    v.item = g_MenuEquipBuffer[EQ_ROW + EQ_TOP];
                    Menu_DrawEquipMateriaSlotRow1x(77, g_MenuEquipRectPtr[2].y + 11, g_KernelWeaponDataArray[v.item].materiaSlotTable);
                    v.growth = g_KernelWeaponDataArray[v.item].materiaGrowth[0];
                    break;
                case EQUIP_ARMOR:
                    v.item = g_MenuEquipBuffer[EQ_ROW + EQ_TOP];
                    Menu_DrawEquipMateriaSlotRow1x(77, g_MenuEquipRectPtr[2].y + 11, g_KernelArmorDataArray[v.item].materiaSlotBytes);
                    v.growth = g_KernelArmorDataArray[v.item].materiaSlotBytes[8];
                    break;
                }
                switch (v.growth) {
                case GROWTH_NONE:
                case GROWTH_NORMAL:
                case GROWTH_DOUBLE:
                case GROWTH_TRIPLE:
                    break;
                default:
                    v.growth = 4;
                    break;
                }
                Menu_DrawEncodedString1x(122 - Menu_MeasureEncodedString2x(EQ_GROWTH_TEXT) / 2, g_MenuEquipRectPtr[2].y + 32, EQ_GROWTH_TEXT, 7, 0.2f);
            }
            break;
        }
        Menu_EmptyTextHook(8);
        for (v.i = 0; v.i < 3; v.i++)
            Menu_DrawEncodedString1x(125, v.i * 17 + 7, &g_MenuEquipLabelTable[v.i * 12], 5, 0.2f);
        Menu_DrawEquipStatRows1x(27, g_MenuEquipRectPtr[3].y + 13);
        Menu_DrawCharStatBlock1x(55, 9, g_MenuEquipCharIndex, 0.2f);
        Menu_DrawCharPortrait1x(9, 5, g_MenuEquipCharIndex, 0.2f);
        Menu_EmptyTextHook(10);
        Menu_DrawEncodedString1x(152, 7, Kernel_LookupItemNameText(v.records[v.record].weapon + 128), 7, 0.2f);
        Menu_DrawEncodedString1x(152, 24, Kernel_LookupItemNameText(v.records[v.record].armor + 256), 7, 0.2f);
        if (v.records[v.record].accessory != 255)
            Menu_DrawEncodedString1x(152, 41, Kernel_LookupItemNameText(v.records[v.record].accessory + 288), 7, 0.2f);
        if (g_MenuEquipSelectedSlot == MENU_EQUIP_FOCUS_LIST) {
            switch (EQ_TYPE) {
            case EQUIP_WEAPON:
                Menu_DrawEncodedString1x(12, g_MenuEquipRectPtr[1].y + 7, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM, g_MenuEquipBuffer[EQ_TOP + EQ_ROW] + 128, 0), 7, 0.2f);
                break;
            case EQUIP_ARMOR:
                Menu_DrawEncodedString1x(12, g_MenuEquipRectPtr[1].y + 7, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM, g_MenuEquipBuffer[EQ_TOP + EQ_ROW] + 256, 0), 7, 0.2f);
                break;
            case EQUIP_ACCESSORY:
                Menu_DrawEncodedString1x(12, g_MenuEquipRectPtr[1].y + 7, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM, g_MenuEquipBuffer[EQ_TOP + EQ_ROW] + 288, 0), 7, 0.2f);
                break;
            }
        } else {
            switch (EQ_TYPE) {
            case EQUIP_WEAPON:
                Menu_DrawEncodedString1x(12, g_MenuEquipRectPtr[1].y + 7, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM, gSavemap.chars[EQ_RECORD].weapon + 128, 0), 7, 0.2f);
                break;
            case EQUIP_ARMOR:
                Menu_DrawEncodedString1x(12, g_MenuEquipRectPtr[1].y + 7, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM, gSavemap.chars[EQ_RECORD].armor + 256, 0), 7, 0.2f);
                break;
            case EQUIP_ACCESSORY:
                if (gSavemap.chars[EQ_RECORD].accessory != 255)
                    Menu_DrawEncodedString1x(12, g_MenuEquipRectPtr[1].y + 7, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ITEM, gSavemap.chars[EQ_RECORD].accessory + 288, 0), 7, 0.2f);
                break;
            }
        }
        for (v.j = 0; v.j < 2; v.j++)
            Menu_DrawWindowFrame1x(&g_MenuEquipRectPtr[v.j], 0.3f);
        Menu_DrawWindowFrame1x(&g_MenuEquipRectPtr[2], 0.4f);
        Menu_DrawWindowFrame1x(&g_MenuEquipRectPtr[3], 0.5f);
        Menu_DrawWindowFrame1x(&g_MenuEquipRectPtr[4], 0.6f);
    }
    if (Menu_GetPhase() == 0) {
        MenuCursor_Update((MenuCursor *)((unsigned char *)g_MenuEquipCursor0 + g_MenuEquipSelectedSlot * 56));
        switch (g_MenuEquipSelectedSlot) {
        case MENU_EQUIP_FOCUS_KIND:
            if (BtlMenu_GetInputRepeat(INPUT_L1)) {
                do {
                    g_MenuEquipCharIndex--;
                    if (g_MenuEquipCharIndex < 0)
                        g_MenuEquipCharIndex = 2;
                } while (gSavemap.party[g_MenuEquipCharIndex] == PARTY_CHAR_NONE);
            } else if (BtlMenu_GetInputRepeat(INPUT_R1)) {
                do {
                    g_MenuEquipCharIndex++;
                    if (g_MenuEquipCharIndex > 2)
                        g_MenuEquipCharIndex = 0;
                } while (gSavemap.party[g_MenuEquipCharIndex] == PARTY_CHAR_NONE);
            }
            if (BtlMenu_GetInputRepeat(INPUT_CROSS)) {
                if (g_MenuEquipCount != 0 && (g_InputScriptCounter != 0 || g_MenuMateriaVisible != 0)) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    Menu_SetPhase(4, 3);
                    Menu_SetScreen(3);
                    g_MenuPartyMemberIndex = g_MenuEquipCharIndex;
                } else {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                    Menu_SetPhase(5, 0);
                    Menu_SetScreen(0);
                }
            } else if (BtlMenu_GetInputRepeat(INPUT_CIRCLE)) {
                Menu_BuildEquipCandidateList(EQ_TYPE);
                if (g_MenuEquipCandidateCount == 0 || (gSavemap.snowUnused & 1)) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                } else {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    g_MenuEquipSelectedSlot = MENU_EQUIP_FOCUS_LIST;
                    v.rows = g_MenuEquipCandidateCount > 8 ? 8 : g_MenuEquipCandidateCount;
                    MenuCursor_Init((MenuCursor *)g_MenuEquipCursor1, 0, 0, 1, v.rows, 0, 0, 1, g_MenuEquipCandidateCount, 0, 0, 0, 0, 0, 1);
                }
            } else if (BtlMenu_GetInputRepeat(INPUT_SQUARE) && (g_InputScriptCounter != 0 || g_MenuMateriaVisible != 0)) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                Menu_SetPhase(4, 3);
                Menu_SetScreen(3);
                g_MenuPartyMemberIndex = g_MenuEquipCharIndex;
            } else if (BtlMenu_GetInputRepeat(INPUT_TRIANGLE) && !(gSavemap.snowUnused & 1) && EQ_TYPE == EQUIP_ACCESSORY) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                if (v.records[v.record].accessory != 255)
                    Savemap_AddItem((v.records[v.record].accessory + 288) | MENU_ITEM_QUANTITY_ONE);
                v.records[v.record].accessory = 255;
                Savemap_BuildPartyStatsFromRecord(g_MenuEquipCharIndex);
                Battle_LoadCombatantFromSavemap(g_MenuEquipCharIndex);
                Party_RecomputeBattleModifiers();
            }
            break;
        case MENU_EQUIP_FOCUS_LIST:
            if (BtlMenu_GetInputRepeat(INPUT_CIRCLE)) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_MATERIA_REMOVE);
                g_MenuEquipSelectedSlot = MENU_EQUIP_FOCUS_KIND;
                v.item = g_MenuEquipBuffer[EQ_ROW + EQ_TOP];
                switch (EQ_TYPE) {
                case EQUIP_WEAPON:
                    v.slots = g_KernelWeaponDataArray[v.item].materiaSlotTable;
                    v.oldItem = v.records[v.record].weapon;
                    v.records[v.record].weapon = (unsigned char)v.item;
                    v.oldItem += 128;
                    v.item += 128;
                    g_MenuEquipCount = 1;
                    for (v.j = 0; v.j < 8; v.j++) {
                        if (v.slots[v.j] == 0 && v.records[v.record].weaponMateria[v.j] != -1) {
                            Savemap_ClearNewMateriaQueue();
                            Savemap_AddMateriaToStock(v.records[v.record].weaponMateria[v.j]);
                            Savemap_ClearNewMateriaQueue();
                            v.records[v.record].weaponMateria[v.j] = -1;
                        }
                    }
                    Savemap_AddItem(v.oldItem | MENU_ITEM_QUANTITY_ONE);
                    break;
                case EQUIP_ARMOR:
                    v.oldItem = v.records[v.record].armor;
                    v.records[v.record].armor = (unsigned char)v.item;
                    v.oldItem += 256;
                    v.item += 256;
                    g_MenuEquipCount = 1;
                    v.slots = (unsigned char *)Savemap_GetPartyArmourRecord(g_MenuEquipCharIndex);
                    for (v.j = 0; v.j < 8; v.j++) {
                        if (v.slots[v.j] == 0 && v.records[v.record].armorMateria[v.j] != -1) {
                            Savemap_ClearNewMateriaQueue();
                            Savemap_AddMateriaToStock(v.records[v.record].armorMateria[v.j]);
                            Savemap_ClearNewMateriaQueue();
                            v.records[v.record].armorMateria[v.j] = -1;
                        }
                    }
                    Savemap_AddItem(v.oldItem | MENU_ITEM_QUANTITY_ONE);
                    break;
                case EQUIP_ACCESSORY:
                    v.oldItem = v.records[v.record].accessory;
                    v.records[v.record].accessory = (unsigned char)v.item;
                    if (v.oldItem != 255) {
                        v.oldItem += 288;
                        Savemap_AddItem(v.oldItem | MENU_ITEM_QUANTITY_ONE);
                    }
                    v.item += 288;
                    break;
                }
                Savemap_RemoveItem(v.item | MENU_ITEM_QUANTITY_ONE);
                Savemap_BuildPartyStatsFromRecord(g_MenuEquipCharIndex);
                Battle_LoadCombatantFromSavemap(g_MenuEquipCharIndex);
                Party_RecomputeBattleModifiers();
            } else if (BtlMenu_GetInputRepeat(INPUT_CROSS)) {
                g_MenuEquipSelectedSlot = MENU_EQUIP_FOCUS_KIND;
            }
            break;
        }
    }
#undef EQ_GROWTH_TEXT
#undef EQ_RECORD
#undef EQ_MOVING
#undef EQ_SCROLL
#undef EQ_TOP
#undef EQ_ROW
#undef EQ_TYPE
}

/*
 * Draws equipment materia slots and links in 2x mode.
 */
// FUNCTION: FF7 0x0070760f
void __cdecl Menu_DrawEquipMateriaSlotRow2x(int x, int y, unsigned char *slots)
{
    short rect[4];
    int i;

    for (i = 0; i < 8; i++) {
        if (slots[i] != 0) {
            if (slots[i] > 4) {
                Menu_DrawAtlasQuad2x(x + i * 0x1c, y, 0x80, 0x30, 0x10, 0x10, 1, 0, 0,
                                     0.201f);
            } else {
                Menu_DrawAtlasQuad2x(x + i * 0x1c, y, 0x70, 0x20, 0x10, 0x10, 1, 0, 0,
                                     0.201f);
            }
        }
    }
    for (i = 0; i < 8; i++) {
        if (slots[i] != 0 && (slots[i] == 3 || slots[i] == 7)) {
            Menu_DrawAtlasQuad2x(x + i * 0x1c - 0xc, y, 0x70, 0x30, 0x10, 0x10, 1,
                                 0, 0, 0.2f);
        }
    }
    rect[0] = (short)(x - 3);
    rect[1] = (short)y;
    rect[2] = 0xe2;
    rect[3] = 0x18;
    Menu_DrawEtchedBox2x((EnglishWindowFrame *)rect, 0.201f);
}

/*
 * Draws equipment materia slots and links in 1x mode.
 */
// FUNCTION: FF7 0x00707750
void __cdecl Menu_DrawEquipMateriaSlotRow1x(int x, int y, unsigned char *slots)
{
    short rect[4];
    int i;

    for (i = 0; i < 8; i++) {
        if (slots[i] != 0) {
            if (slots[i] > 4) {
                Menu_DrawAtlasQuad1x(x + i * 0xe, y, 0x80, 0x30, 0x10, 0x10, 1, 0, 0,
                                     0.201f);
            } else {
                Menu_DrawAtlasQuad1x(x + i * 0xe, y, 0x70, 0x20, 0x10, 0x10, 1, 0, 0,
                                     0.201f);
            }
        }
    }
    for (i = 0; i < 8; i++) {
        if (slots[i] != 0 && (slots[i] == 3 || slots[i] == 7)) {
            Menu_DrawAtlasQuad1x(x + i * 0xe - 7, y, 0x70, 0x30, 0x10, 0x10, 1,
                                 0, 0, 0.2f);
        }
    }
    rect[0] = (short)(x - 2);
    rect[1] = (short)y;
    rect[2] = 0x71;
    rect[3] = 0xc;
    Menu_DrawEtchedBox1x((EnglishWindowFrame *)rect, 0.201f);
}

/*
 * Draws stat preview rows comparing current and candidate equipment in 2x mode.
 */
// FUNCTION: FF7 0x00707891
void __cdecl Menu_DrawEquipStatRows(int baseX, int baseY)
{
    int hit;
    int col;
    int def;
    int atk;
    int dfpo;
    int atk2;
    int newM;
    int srcDfp;
    int i;
    int mevN;
    int mep;
    int mdef;
    int def2;
    int magN;
    int who;
    int matk;
    int hit2;
    int sel;

    who = gSavemap.party[g_MenuEquipCharIndex];
    for (i = 0; i < 7; i++) {
        Menu_DrawEncodedString2x(baseX, baseY + i * 0x1a - 6,
                          &g_MenuEquipLabelTable[(i + MENU_EQUIP_LABEL_ATTACK) * 0xc], 5,
                          0.2f);
    }
    for (i = 0; i < 7; i++) {
        DrawIconGlyph16_2x(baseX + 0xc2, baseY + i * 0x1a, 0xda, 5, 0.2f);
    }
    atk = g_KernelWeaponDataArray[gSavemap.chars[g_CharIdToSaveSlot[who]].weapon].attackBonusByte;
    atk2 = atk;
    hit = g_KernelWeaponDataArray[gSavemap.chars[g_CharIdToSaveSlot[who]].weapon].hitRate;
    hit2 = hit;
    def = g_KernelArmorDataArray[gSavemap.chars[g_CharIdToSaveSlot[who]].armor].physicalDefense;
    def2 = def;
    dfpo = g_KernelArmorDataArray[gSavemap.chars[g_CharIdToSaveSlot[who]].armor].dfpo;
    srcDfp = dfpo;
    mdef = g_KernelArmorDataArray[gSavemap.chars[g_CharIdToSaveSlot[who]].armor].magicDefense;
    newM = mdef;
    mep = g_KernelArmorDataArray[gSavemap.chars[g_CharIdToSaveSlot[who]].armor].magicEvade;
    mevN = mep;
    magN = 0;
    matk = magN;
    Menu_DrawDecimalNumber2x(baseX + 0x93, baseY, atk, 3, 7, 0.2f);
    Menu_DrawDecimalNumber2x(baseX + 0x93, baseY + 0x1a, hit, 3, 7, 0.2f);
    Menu_DrawDecimalNumber2x(baseX + 0x93, baseY + 0x34, def, 3, 7, 0.2f);
    Menu_DrawDecimalNumber2x(baseX + 0x93, baseY + 0x4e, dfpo, 3, 7, 0.2f);
    Menu_DrawDecimalNumber2x(baseX + 0x93, baseY + 0x68, matk, 3, 7, 0.2f);
    Menu_DrawDecimalNumber2x(baseX + 0x93, baseY + 0x82, mdef, 3, 7, 0.2f);
    Menu_DrawDecimalNumber2x(baseX + 0x93, baseY + 0x9c, mep, 3, 7, 0.2f);
    if (g_MenuEquipSelectedSlot != MENU_EQUIP_FOCUS_KIND) {
        if (g_MenuEquipCursor0[1] != EQUIP_ACCESSORY) {
            switch (g_MenuEquipCursor0[1]) {
            case EQUIP_WEAPON:
                sel = g_MenuEquipBuffer[g_MenuEquipCursor1[1] +
                                   g_MenuEquipCursor1[5]];
                atk2 = g_KernelWeaponDataArray[sel].attackBonusByte;
                hit2 = g_KernelWeaponDataArray[sel].hitRate;
                break;
            case EQUIP_ARMOR:
                sel = g_MenuEquipBuffer[g_MenuEquipCursor1[1] +
                                   g_MenuEquipCursor1[5]];
                def2 = g_KernelArmorDataArray[sel].physicalDefense;
                srcDfp = g_KernelArmorDataArray[sel].dfpo;
                newM = g_KernelArmorDataArray[sel].magicDefense;
                mevN = g_KernelArmorDataArray[sel].magicEvade;
                break;
            }
        }
        if (atk2 > atk) {
            col = 6;
        } else if (atk2 < atk) {
            col = 2;
        } else {
            col = 7;
        }
        Menu_DrawDecimalNumber2x(baseX + 0xd9, baseY, atk2, 3, (char)col, 0.2f);
        if (hit2 > hit) {
            col = 6;
        } else if (hit2 < hit) {
            col = 2;
        } else {
            col = 7;
        }
        Menu_DrawDecimalNumber2x(baseX + 0xd9, baseY + 0x1a, hit2, 3, (char)col,
                          0.2f);
        if (def2 > def) {
            col = 6;
        } else if (def2 < def) {
            col = 2;
        } else {
            col = 7;
        }
        Menu_DrawDecimalNumber2x(baseX + 0xd9, baseY + 0x34, def2, 3, (char)col,
                          0.2f);
        if (srcDfp > dfpo) {
            col = 6;
        } else if (srcDfp < dfpo) {
            col = 2;
        } else {
            col = 7;
        }
        Menu_DrawDecimalNumber2x(baseX + 0xd9, baseY + 0x4e, srcDfp, 3, (char)col,
                          0.2f);
        if (magN > matk) {
            col = 6;
        } else if (magN < matk) {
            col = 2;
        } else {
            col = 7;
        }
        Menu_DrawDecimalNumber2x(baseX + 0xd9, baseY + 0x68, magN, 3, (char)col,
                          0.2f);
        if (newM > mdef) {
            col = 6;
        } else if (newM < mdef) {
            col = 2;
        } else {
            col = 7;
        }
        Menu_DrawDecimalNumber2x(baseX + 0xd9, baseY + 0x82, newM, 3, (char)col,
                          0.2f);
        if (mevN > mep) {
            col = 6;
        } else if (mevN < mep) {
            col = 2;
        } else {
            col = 7;
        }
        Menu_DrawDecimalNumber2x(baseX + 0xd9, baseY + 0x9c, mevN, 3, (char)col,
                          0.2f);
    }
}

/*
 * Draws stat preview rows comparing current and candidate equipment in 1x mode.
 */
// FUNCTION: FF7 0x00707e58
void __cdecl Menu_DrawEquipStatRows1x(int baseX, int baseY)
{
    int hit;
    int col;
    int def;
    int atk;
    int dfpo;
    int atk2;
    int newM;
    int srcDfp;
    int i;
    int mevN;
    int mep;
    int mdef;
    int def2;
    int magN;
    int who;
    int matk;
    int hit2;
    int sel;

    who = gSavemap.party[g_MenuEquipCharIndex];
    for (i = 0; i < 7; i++) {
        Menu_DrawEncodedString1x(baseX, baseY + i * 0xd - 3,
                             &g_MenuEquipLabelTable[(i + MENU_EQUIP_LABEL_ATTACK) * 0xc], 5,
                             0.2f);
    }
    for (i = 0; i < 7; i++) {
        DrawIconGlyph16_1x(baseX + 0x62, baseY + i * 0xd, 0xda, 5, 0.2f);
    }
    atk = g_KernelWeaponDataArray[gSavemap.chars[g_CharIdToSaveSlot[who]].weapon].attackBonusByte;
    atk2 = atk;
    hit = g_KernelWeaponDataArray[gSavemap.chars[g_CharIdToSaveSlot[who]].weapon].hitRate;
    hit2 = hit;
    def = g_KernelArmorDataArray[gSavemap.chars[g_CharIdToSaveSlot[who]].armor].physicalDefense;
    def2 = def;
    dfpo = g_KernelArmorDataArray[gSavemap.chars[g_CharIdToSaveSlot[who]].armor].dfpo;
    srcDfp = dfpo;
    mdef = g_KernelArmorDataArray[gSavemap.chars[g_CharIdToSaveSlot[who]].armor].magicDefense;
    newM = mdef;
    mep = g_KernelArmorDataArray[gSavemap.chars[g_CharIdToSaveSlot[who]].armor].magicEvade;
    mevN = mep;
    magN = 0;
    matk = magN;
    Menu_DrawDecimalNumber1x(baseX + 0x4a, baseY, atk, 3, 7, 0.2f);
    Menu_DrawDecimalNumber1x(baseX + 0x4a, baseY + 0xd, hit, 3, 7, 0.2f);
    Menu_DrawDecimalNumber1x(baseX + 0x4a, baseY + 0x1a, def, 3, 7, 0.2f);
    Menu_DrawDecimalNumber1x(baseX + 0x4a, baseY + 0x27, dfpo, 3, 7, 0.2f);
    Menu_DrawDecimalNumber1x(baseX + 0x4a, baseY + 0x34, matk, 3, 7, 0.2f);
    Menu_DrawDecimalNumber1x(baseX + 0x4a, baseY + 0x41, mdef, 3, 7, 0.2f);
    Menu_DrawDecimalNumber1x(baseX + 0x4a, baseY + 0x4e, mep, 3, 7, 0.2f);
    if (g_MenuEquipSelectedSlot != MENU_EQUIP_FOCUS_KIND) {
        if (g_MenuEquipCursor0[1] != EQUIP_ACCESSORY) {
            switch (g_MenuEquipCursor0[1]) {
            case EQUIP_WEAPON:
                sel = g_MenuEquipBuffer[g_MenuEquipCursor1[1] +
                                   g_MenuEquipCursor1[5]];
                atk2 = g_KernelWeaponDataArray[sel].attackBonusByte;
                hit2 = g_KernelWeaponDataArray[sel].hitRate;
                break;
            case EQUIP_ARMOR:
                sel = g_MenuEquipBuffer[g_MenuEquipCursor1[1] +
                                   g_MenuEquipCursor1[5]];
                def2 = g_KernelArmorDataArray[sel].physicalDefense;
                srcDfp = g_KernelArmorDataArray[sel].dfpo;
                newM = g_KernelArmorDataArray[sel].magicDefense;
                mevN = g_KernelArmorDataArray[sel].magicEvade;
                break;
            }
        }
        if (atk2 > atk) {
            col = 6;
        } else if (atk2 < atk) {
            col = 2;
        } else {
            col = 7;
        }
        Menu_DrawDecimalNumber1x(baseX + 0x6d, baseY, atk2, 3, (char)col, 0.2f);
        if (hit2 > hit) {
            col = 6;
        } else if (hit2 < hit) {
            col = 2;
        } else {
            col = 7;
        }
        Menu_DrawDecimalNumber1x(baseX + 0x6d, baseY + 0xd, hit2, 3, (char)col,
                            0.2f);
        if (def2 > def) {
            col = 6;
        } else if (def2 < def) {
            col = 2;
        } else {
            col = 7;
        }
        Menu_DrawDecimalNumber1x(baseX + 0x6d, baseY + 0x1a, def2, 3, (char)col,
                            0.2f);
        if (srcDfp > dfpo) {
            col = 6;
        } else if (srcDfp < dfpo) {
            col = 2;
        } else {
            col = 7;
        }
        Menu_DrawDecimalNumber1x(baseX + 0x6d, baseY + 0x27, srcDfp, 3, (char)col,
                            0.2f);
        if (magN > matk) {
            col = 6;
        } else if (magN < matk) {
            col = 2;
        } else {
            col = 7;
        }
        Menu_DrawDecimalNumber1x(baseX + 0x6d, baseY + 0x34, magN, 3, (char)col,
                            0.2f);
        if (newM > mdef) {
            col = 6;
        } else if (newM < mdef) {
            col = 2;
        } else {
            col = 7;
        }
        Menu_DrawDecimalNumber1x(baseX + 0x6d, baseY + 0x41, newM, 3, (char)col,
                            0.2f);
        if (mevN > mep) {
            col = 6;
        } else if (mevN < mep) {
            col = 2;
        } else {
            col = 7;
        }
        Menu_DrawDecimalNumber1x(baseX + 0x6d, baseY + 0x4e, mevN, 3, (char)col,
                            0.2f);
    }
}

/*
 * Builds candidate inventory list for selected equipment type.
 */
// FUNCTION: FF7 0x007083ed
void __cdecl Menu_BuildEquipCandidateList(int equipType)
{
    int slot;
    int cnt;
    int itemId;

    switch (equipType) {
    case EQUIP_WEAPON:
        cnt = slot = 0;
        for (; slot < 0x140; slot++) {
            if (((unsigned short *)gSavemapItems)[slot] == MENU_ITEM_SLOT_EMPTY) {
                continue;
            }
            itemId = ((unsigned short *)gSavemapItems)[slot] & 0x1ff;
            if (itemId >= 0x80 && itemId < 0x100) {
                itemId -= 0x80;
                if ((g_KernelWeaponDataArray[itemId].equipMask &
                     (1 << gSavemap.party[g_MenuEquipCharIndex])) != 0) {
                    g_MenuEquipBuffer[cnt] = (unsigned char)itemId;
                    cnt++;
                }
            }
        }
        break;
    case EQUIP_ARMOR:
        cnt = slot = 0;
        for (; slot < 0x140; slot++) {
            if (((unsigned short *)gSavemapItems)[slot] == MENU_ITEM_SLOT_EMPTY) {
                continue;
            }
            itemId = ((unsigned short *)gSavemapItems)[slot] & 0x1ff;
            if (itemId >= 0x100 && itemId < 0x120) {
                itemId -= 0x100;
                if ((g_KernelArmorDataArray[itemId].equipMask &
                     (1 << gSavemap.party[g_MenuEquipCharIndex])) != 0) {
                    g_MenuEquipBuffer[cnt] = (unsigned char)itemId;
                    cnt++;
                }
            }
        }
        break;
    case EQUIP_ACCESSORY:
        cnt = slot = 0;
        for (; slot < 0x140; slot++) {
            if (((unsigned short *)gSavemapItems)[slot] == MENU_ITEM_SLOT_EMPTY) {
                continue;
            }
            itemId = ((unsigned short *)gSavemapItems)[slot] & 0x1ff;
            if (itemId >= 0x120 && itemId < 0x160) {
                itemId -= 0x120;
                if ((g_KernelAccessoryDataArray[itemId].equipMask &
                     (1 << gSavemap.party[g_MenuEquipCharIndex])) != 0) {
                    g_MenuEquipBuffer[cnt] = (unsigned char)itemId;
                    cnt++;
                }
            }
        }
        break;
    }
    g_MenuEquipCandidateCount = cnt;
    for (; cnt < 0x30; cnt++) {
        g_MenuEquipBuffer[cnt] = 0xff;
    }
}
} /* extern "C" */
