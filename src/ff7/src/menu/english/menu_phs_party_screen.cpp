// src/ff7/src/menu/english/menu_phs_party_screen.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The PHS party-change screen: its equipment and experience panels in both scales, the
//   pick-up and swap logic for party and reserve slots, and the screen's per-frame update and modal
//   module tick.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/menu.h"
#include "lib/enums/file.h"
#include "lib/enums/sound.h"
#include <string.h>
#include <stdlib.h>
#include "ff7/api/menu.h"
#include "lib/api/graphics.h"
#include "lib/api/file.h"
#include "lib/api/sound.h"
#include "ff7/savemap.h"
#include "ff7/enums/menu.h"
#include "ff7/enums/main.h"
extern "C" {

struct EnglishWindowFrame;

#pragma intrinsic(abs, memcpy, memset)

// clang-format off
// Six 38-byte encoded FORMMENU texts (REFORM, SPLIT INTO TWO/THREE GROUPS, MAKE A PARTY OF TWO/THREE,
// SELECT WITH ...), indexed by (g_PhsMenuGroupType + 1) or read directly as rows 0, 1, 2, 4 and 5.
// GLOBAL: FF7 0x0091f140
unsigned char g_PhsMenuFormTexts[6][0x26] = {
    {0x32, 0x45, 0x46, 0x4f, 0x52, 0x4d, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x33, 0x50, 0x4c, 0x49, 0x54, 0x00, 0x59, 0x4f, 0x55, 0x52, 0x00, 0x41, 0x4c, 0x4c, 0x49, 0x45, 0x53, 0x00, 0x49, 0x4e, 0x54, 0x4f, 0x00, 0x54, 0x57, 0x4f, 0x00, 0x47, 0x52, 0x4f, 0x55, 0x50, 0x53, 0x0e, 0xff, 0x00, 0x00, 0x00},
    {0x33, 0x50, 0x4c, 0x49, 0x54, 0x00, 0x59, 0x4f, 0x55, 0x52, 0x00, 0x41, 0x4c, 0x4c, 0x49, 0x45, 0x53, 0x00, 0x49, 0x4e, 0x54, 0x4f, 0x00, 0x54, 0x48, 0x52, 0x45, 0x45, 0x00, 0x47, 0x52, 0x4f, 0x55, 0x50, 0x53, 0x0e, 0xff, 0x00},
    {0x30, 0x4c, 0x45, 0x41, 0x53, 0x45, 0x00, 0x4d, 0x41, 0x4b, 0x45, 0x00, 0x41, 0x00, 0x50, 0x41, 0x52, 0x54, 0x59, 0x00, 0x4f, 0x46, 0x00, 0x54, 0x57, 0x4f, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x30, 0x4c, 0x45, 0x41, 0x53, 0x45, 0x00, 0x4d, 0x41, 0x4b, 0x45, 0x00, 0x41, 0x00, 0x50, 0x41, 0x52, 0x54, 0x59, 0x00, 0x4f, 0x46, 0x00, 0x54, 0x48, 0x52, 0x45, 0x45, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x33, 0x45, 0x4c, 0x45, 0x43, 0x54, 0x00, 0x57, 0x49, 0x54, 0x48, 0x00, 0x33, 0x34, 0x21, 0x32, 0x34, 0x00, 0x42, 0x55, 0x54, 0x54, 0x4f, 0x4e, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};
// clang-format off
// The 27 encoded menu texts of 22 bytes each; rows 23 to 25 ("7PN", "!RM", "!CC") are the equipment
// labels and row 26 is ",IMIT LEVEL".
// Text: "Element" "Effect" "Attack" "Defend" "Halve" "Invalid" "Absorb" "Power" "Speed" "Stamina"
//       "Magic power" "Spirit" "Luck" "Attacking power" "Accuracy" "Deffense" "Avoidance rate"
//       "Magic attack" "Magic deffense" "Magic avoidance rate" "Wpn." "Arm." "Acc." "Limit level:"
// GLOBAL: FF7 0x0091f228
unsigned char g_PhsMenuLabelTexts[27][22] = {
    0x25, 0x4c, 0x45, 0x4d, 0x45, 0x4e, 0x54, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x25, 0x46, 0x46, 0x45, 0x43, 0x54, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x21, 0x54, 0x54, 0x41, 0x43, 0x4b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x24, 0x45, 0x46, 0x45, 0x4e, 0x44, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x28, 0x41, 0x4c, 0x56, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x29, 0x4e, 0x56, 0x41, 0x4c, 0x49, 0x44, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x21, 0x42, 0x53, 0x4f, 0x52, 0x42, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x30, 0x4f, 0x57, 0x45, 0x52, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x33, 0x50, 0x45, 0x45, 0x44, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x33, 0x54, 0x41, 0x4d, 0x49, 0x4e, 0x41, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2d, 0x41, 0x47, 0x49, 0x43, 0x00, 0x50, 0x4f, 0x57, 0x45, 0x52, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x33, 0x50, 0x49, 0x52, 0x49, 0x54, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2c, 0x55, 0x43, 0x4b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x21, 0x54, 0x54, 0x41, 0x43, 0x4b, 0x49, 0x4e, 0x47, 0x00, 0x50, 0x4f, 0x57, 0x45, 0x52, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x21, 0x43, 0x43, 0x55, 0x52, 0x41, 0x43, 0x59, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x24, 0x45, 0x46, 0x46, 0x45, 0x4e, 0x53, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x21, 0x56, 0x4f, 0x49, 0x44, 0x41, 0x4e, 0x43, 0x45, 0x00, 0x52, 0x41, 0x54, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2d, 0x41, 0x47, 0x49, 0x43, 0x00, 0x41, 0x54, 0x54, 0x41, 0x43, 0x4b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2d, 0x41, 0x47, 0x49, 0x43, 0x00, 0x44, 0x45, 0x46, 0x46, 0x45, 0x4e, 0x53, 0x45, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2d, 0x41, 0x47, 0x49, 0x43, 0x00, 0x41, 0x56, 0x4f, 0x49, 0x44, 0x41, 0x4e, 0x43, 0x45, 0x00, 0x52, 0x41, 0x54, 0x45, 0xff, 0x00,
    0x37, 0x50, 0x4e, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x21, 0x52, 0x4d, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x21, 0x43, 0x43, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2c, 0x49, 0x4d, 0x49, 0x54, 0x00, 0x4c, 0x45, 0x56, 0x45, 0x4c, 0x1a, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// GLOBAL: FF7 0x0091f47c
int g_PhsMenuPrevSwapState = (-1);
// GLOBAL: FF7 0x0091f480
int g_PhsMenuSwapState = (-1);
// The two window rectangles (x, y, w, h) the TU draws at 2x and 1x.
// GLOBAL: FF7 0x0091f488
struct EnglishWindowFrame g_PhsMenuRect2x = {476, 0, 164, 51};
// GLOBAL: FF7 0x0091f490
struct EnglishWindowFrame g_PhsMenuRect1x = {246, 0, 74, 26};
// clang-format off
// GLOBAL: FF7 0x0091f498
EnglishWindowFrame g_PhsMenuFrames2x[4] = {{0, 0, 640, 51}, {0, 51, 307, 429}, {307, 51, 333, 120}, {307, 163, 333, 317}};
// GLOBAL: FF7 0x0091f4b8
EnglishWindowFrame g_PhsMenuFrames1x[4] = {{0, 0, 320, 26}, {0, 26, 153, 214}, {153, 26, 167, 60}, {153, 82, 167, 158}};
// clang-format on
// GLOBAL: FF7 0x00dca118
int g_PhsMenuState = 0;
// GLOBAL: FF7 0x00dca11c
int g_PhsMenuPartySlot = 0;
// GLOBAL: FF7 0x00dca120
int g_PhsMenuSavedPartySlot = 0;
// GLOBAL: FF7 0x00dca124
struct EnglishWindowFrame *g_PhsMenuFrames = 0;
// GLOBAL: FF7 0x00dca128
int g_PhsMenuCursor = 0;
// GLOBAL: FF7 0x00dca12c
int g_PhsMenuGroupType = 0;
// GLOBAL: FF7 0x00dca130
int g_PhsMenuSelection = 0;
// GLOBAL: FF7 0x00dca134
int g_PhsMenuMemberCount = 0;
// GLOBAL: FF7 0x00dca138
unsigned char *g_PhsMenuPtr = 0;
// GLOBAL: FF7 0x00dca13c
int g_PhsMenuSavedColumn = 0;
// GLOBAL: FF7 0x00dca140
int g_PhsMenuSavedCursor = 0;
unsigned char g_bssPad_00dca144[4] = {0};
// GLOBAL: FF7 0x00dca148
unsigned char g_PhsMenuPartyMap[0xc] = {0};
// GLOBAL: FF7 0x00dca154
unsigned char g_PhsMenuHeldMember = 0;
// GLOBAL: FF7 0x00dca158
unsigned char g_PhsMenuSwapMember = 0;
// GLOBAL: FF7 0x00dca15c
int g_PhsMenuFrameParity = 0;
// GLOBAL: FF7 0x00dca160
int g_PhsPartyChangeMode = 0;
// GLOBAL: FF7 0x00dca164
int g_PhsMenuFrameCount = 0;
unsigned char g_bssPad_00dca168[4] = {0};
// GLOBAL: FF7 0x00dca16c
int g_PhsMenuCharIndex = 0;
unsigned char g_bssPad_00dca170[4] = {0};
// GLOBAL: FF7 0x00dca174
unsigned char *g_PhsMenuBufPtr = 0;
unsigned char g_bssPad_00dca178[4] = {0};
// GLOBAL: FF7 0x00dca17c
int g_PhsMenuCounter = 0;
// GLOBAL: FF7 0x00dca180
int g_PhsMenuHeldSource = 0;
unsigned char g_bssPad_00dca184[4] = {0};
// GLOBAL: FF7 0x00dca188
int g_PhsMenuActive = 0;
unsigned char g_bssPad_00dca18c[4] = {0};
// clang-format on

void __cdecl Battle_LoadCombatantFromSavemap(unsigned char partySlot);

// Draws the weapon, armor and accessory names of savemap character g_PhsMenuCharIndex, skipping the
// accessory name when it is 0xFF. Three fixed labels are drawn at vertically spaced positions.
// Menu_DrawEquipMateriaSlots2x then draws the materia slots of the weapon and armor from the
// character's weaponMateria and armorMateria.
// FUNCTION: FF7 0x00700090
void __cdecl Menu_DrawPhsEquipmentPanel2x(int x, int y)
{
    SavemapCharacter *charRecords;
    int characterIndex;

    characterIndex = g_PhsMenuCharIndex;
    charRecords = &gSavemap.chars[0];
    Menu_DrawEncodedString2x(x + 0x5D, y, Kernel_LookupItemNameText(charRecords[characterIndex].weapon + 0x80), 7, 0.101f);
    Menu_DrawEncodedString2x(x + 0x5D, y + 0x43, Kernel_LookupItemNameText(charRecords[characterIndex].armor + 0x100), 7, 0.101f);
    if (charRecords[characterIndex].accessory != 0xFF) {
        Menu_DrawEncodedString2x(x + 0x5D, y + 0x89, Kernel_LookupItemNameText(charRecords[characterIndex].accessory + 0x120), 7, 0.101f);
    }
    Menu_DrawEncodedString2x(x + 0x1B, y, (unsigned char *)g_PhsMenuLabelTexts[23], 5, 0.101f);
    Menu_DrawEncodedString2x(x + 0x1B, y + 0x43, (unsigned char *)g_PhsMenuLabelTexts[24], 5, 0.101f);
    Menu_DrawEncodedString2x(x + 0x1B, y + 0x89, (unsigned char *)g_PhsMenuLabelTexts[25], 5, 0.101f);
    Menu_DrawEquipMateriaSlots2x(x + 0x4D, y + 0x22, g_KernelWeaponDataArray[charRecords[characterIndex].weapon].materiaSlotTable, charRecords[characterIndex].weaponMateria);
    Menu_DrawEquipMateriaSlots2x(x + 0x4D, y + 0x67, g_KernelArmorDataArray[charRecords[characterIndex].armor].materiaSlotBytes, charRecords[characterIndex].armorMateria);
}

/*
 * Draws eight equipment sockets at 2x scale from socketBytes's nonzero socket bytes,
 * adding a materia- colored icon for non-0xFF low-byte IDs in materiaIds and
 * selecting the socket frame from value>4. A second pass draws links for socket
 * values 3 or 7; finishes with a 0xE2 by 0x18 etched box.
 */
// FUNCTION: FF7 0x0070026a
void Menu_DrawEquipMateriaSlots2x(int x, int y, unsigned char *socketBytes, int *materiaIds)
{
    int colorBand;
    short rect[4];
    int i;

    for (i = 0; i < 8; i++) {
        if (socketBytes[i] != 0) {
            if ((materiaIds[i] & 0xff) != 0xff) {
                colorBand = Menu_GetMateriaTypeColorIndex(materiaIds[i] & 0xff);
                Menu_DrawAtlasQuad2x(x + i * 0x1c, y, 0x80,
                                     0x20, 0x10, 0x10, colorBand, 1, 0, 0.1f);
            }
            if (socketBytes[i] > 4) {
                Menu_DrawAtlasQuad2x(x + i * 0x1c, y, 0x80,
                                     0x30, 0x10, 0x10, 1, 0, 0, 0.101f);
            } else {
                Menu_DrawAtlasQuad2x(x + i * 0x1c, y, 0x70,
                                     0x20, 0x10, 0x10, 1, 0, 0, 0.101f);
            }
        }
    }
    for (i = 0; i < 8; i++) {
        if (socketBytes[i] != 0 &&
            (socketBytes[i] == 3 ||
             socketBytes[i] == 7)) {
            Menu_DrawAtlasQuad2x(x + i * 0x1c - 0xc, y, 0x70,
                                 0x30, 0x10, 0x10, 1, 0, 0, 0.1001f);
        }
    }
    rect[0] = x - 3;
    rect[1] = y;
    rect[2] = 0xe2;
    rect[3] = 0x18;
    Menu_DrawEtchedBox2x((EnglishWindowFrame *)rect, 0.101f);
}

// The 1x variant of the equipment panel, drawn with Menu_DrawEncodedString1x. Names come from the
// weapon, armor and accessory of savemap character g_PhsMenuCharIndex; the accessory
// name is skipped for 0xFF, and three fixed labels sit at half-spaced positions.
// Menu_DrawEquipMateriaSlots1x draws the materia slots of the weapon and armor from the
// character's weaponMateria and armorMateria.
// FUNCTION: FF7 0x00700415
void __cdecl Menu_DrawPhsEquipmentPanel1x(int x, int y)
{
    SavemapCharacter *charRecords;
    int characterIndex;

    characterIndex = g_PhsMenuCharIndex;
    charRecords = &gSavemap.chars[0];
    Menu_DrawEncodedString1x(x + 0x2F, y, Kernel_LookupItemNameText(charRecords[characterIndex].weapon + 0x80), 7, 0.101f);
    Menu_DrawEncodedString1x(x + 0x2F, y + 0x22, Kernel_LookupItemNameText(charRecords[characterIndex].armor + 0x100), 7, 0.101f);
    if (charRecords[characterIndex].accessory != 0xFF) {
        Menu_DrawEncodedString1x(x + 0x2F, y + 0x45, Kernel_LookupItemNameText(charRecords[characterIndex].accessory + 0x120), 7, 0.101f);
    }
    Menu_DrawEncodedString1x(x + 0xE, y, (unsigned char *)g_PhsMenuLabelTexts[23], 5, 0.101f);
    Menu_DrawEncodedString1x(x + 0xE, y + 0x22, (unsigned char *)g_PhsMenuLabelTexts[24], 5, 0.101f);
    Menu_DrawEncodedString1x(x + 0xE, y + 0x45, (unsigned char *)g_PhsMenuLabelTexts[25], 5, 0.101f);
    Menu_DrawEquipMateriaSlots1x(x + 0x27, y + 0x11, g_KernelWeaponDataArray[charRecords[characterIndex].weapon].materiaSlotTable, charRecords[characterIndex].weaponMateria);
    Menu_DrawEquipMateriaSlots1x(x + 0x27, y + 0x34, g_KernelArmorDataArray[charRecords[characterIndex].armor].materiaSlotBytes, charRecords[characterIndex].armorMateria);
}

/*
 * Draws eight equipment sockets at 1x scale from socketBytes's nonzero socket bytes,
 * adding a materia- colored icon for non-0xFF low-byte IDs in materiaIds and
 * selecting the socket frame from value>4. A second pass draws links for socket
 * values 3 or 7; finishes with a 0x71 by 0xC etched box.
 */
// FUNCTION: FF7 0x007005e9
void Menu_DrawEquipMateriaSlots1x(int x, int y, unsigned char *socketBytes, int *materiaIds)
{
    short rect[4];
    int colorKey;
    int i;

    for (i = 0; i < 8; i++) {
        if (socketBytes[i] != 0) {
            if ((materiaIds[i] & 0xff) != 0xff) {
                colorKey = Menu_GetMateriaTypeColorIndex(materiaIds[i] & 0xff);
                Menu_DrawAtlasQuad1x(x + i * 0xe, y, 0x80,
                                     0x20, 0x10, 0x10, colorKey, 1, 0, 0.1f);
            }
            if (socketBytes[i] > 4) {
                Menu_DrawAtlasQuad1x(x + i * 0xe, y, 0x80,
                                     0x30, 0x10, 0x10, 1, 0, 0, 0.101f);
            } else {
                Menu_DrawAtlasQuad1x(x + i * 0xe, y, 0x70,
                                     0x20, 0x10, 0x10, 1, 0, 0, 0.101f);
            }
        }
    }
    for (i = 0; i < 8; i++) {
        if (socketBytes[i] != 0 &&
            (socketBytes[i] == 3 ||
             socketBytes[i] == 7)) {
            Menu_DrawAtlasQuad1x(x + i * 0xe - 7, y, 0x70,
                                 0x30, 0x10, 0x10, 1, 0, 0, 0.1001f);
        }
    }
    rect[0] = x - 2;
    rect[1] = y;
    rect[2] = 0x71;
    rect[3] = 0xc;
    Menu_DrawEtchedBox1x((EnglishWindowFrame *)rect, 0.101f);
}

/*
 * Picks up the member under the cursor: a party slot (PHS_MENU_FOCUS_PARTY) or a
 * reserve-grid cell (PHS_MENU_FOCUS_RESERVE), remembering which in g_PhsMenuHeldSource.
 * If that character is PHS-locked, plays the invalid sound and returns; otherwise stores
 * its id, entry pointer and cursor position and enters PHS_MENU_HOLD_MEMBER.
 */
// FUNCTION: FF7 0x00700794
void __cdecl Menu_PhsPickUpSelectedMember(void)
{
    g_PhsMenuHeldSource = g_PhsMenuSwapState;
    switch (g_PhsMenuSwapState) {
    case PHS_MENU_FOCUS_PARTY:
        if ((gSavemap.phsLockingMask &
             (1 << gSavemap.party[g_PhsMenuCounter])) != 0) {
            SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
            return;
        }
        g_PhsMenuHeldMember = gSavemap.party[g_PhsMenuCounter];
        g_PhsMenuBufPtr = (unsigned char *)&gSavemap.party[g_PhsMenuCounter];
        g_PhsMenuSavedCursor = g_PhsMenuCounter;
        break;
    case PHS_MENU_FOCUS_RESERVE:
        if ((gSavemap.phsLockingMask & (1 << g_PhsMenuPartyMap[g_PhsMenuPartySlot + g_PhsMenuCursor * 3])) != 0) {
            SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
            return;
        }
        g_PhsMenuHeldMember = g_PhsMenuPartyMap[g_PhsMenuPartySlot + g_PhsMenuCursor * 3];
        g_PhsMenuBufPtr = &g_PhsMenuPartyMap[g_PhsMenuPartySlot + g_PhsMenuCursor * 3];
        g_PhsMenuSavedColumn = g_PhsMenuPartySlot;
        g_PhsMenuSavedCursor = g_PhsMenuCursor;
        break;
    }
    g_PhsMenuState = PHS_MENU_HOLD_MEMBER;
    g_PhsMenuSavedPartySlot = g_PhsMenuCounter;
}

/*
 * Selects the party or reserve character slot under the cursor (g_PhsMenuSwapState),
 * rejects PHS-locked selections with SFX_MENU_INVALID and otherwise swaps it with the held
 * member. When both are the same non-0xFF member, it switches to PHS_MENU_FOCUS_DETAILS
 * and records the character.
 */
// FUNCTION: FF7 0x007008c3
void Menu_PhsCommitSwapSelection(void)
{
    unsigned char swapTmp;
    int partySlot;

    switch (g_PhsMenuSwapState) {
    case PHS_MENU_FOCUS_PARTY:
        if ((gSavemap.phsLockingMask &
             (1 << gSavemap.party[g_PhsMenuCounter])) != 0) {
            SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
            return;
        }
        g_PhsMenuSwapMember = gSavemap.party[g_PhsMenuCounter];
        g_PhsMenuPtr = (unsigned char *)&gSavemap.party[g_PhsMenuCounter];
        break;
    case PHS_MENU_FOCUS_RESERVE:
        if ((gSavemap.phsLockingMask &
             (1 << g_PhsMenuPartyMap[g_PhsMenuPartySlot + g_PhsMenuCursor * 3])) != 0) {
            SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
            return;
        }
        g_PhsMenuSwapMember = g_PhsMenuPartyMap[g_PhsMenuPartySlot + g_PhsMenuCursor * 3];
        g_PhsMenuPtr = &g_PhsMenuPartyMap[g_PhsMenuPartySlot + g_PhsMenuCursor * 3];
        break;
    }
    g_PhsMenuState = PHS_MENU_HOLD_NONE;
    if (*g_PhsMenuBufPtr == *g_PhsMenuPtr && *g_PhsMenuPtr != 0xff) {
        g_PhsMenuPrevSwapState = g_PhsMenuSwapState;
        g_PhsMenuSwapState = PHS_MENU_FOCUS_DETAILS;
        g_PhsMenuCharIndex = *g_PhsMenuPtr;
    }
    swapTmp = *g_PhsMenuBufPtr;
    *g_PhsMenuBufPtr = *g_PhsMenuPtr;
    *g_PhsMenuPtr = swapTmp;
    for (partySlot = 0; partySlot < 3; partySlot++) {
        if (gSavemap.party[partySlot] != PARTY_CHAR_NONE) {
            Savemap_BuildPartyStatsFromRecord(partySlot);
            Battle_LoadCombatantFromSavemap((unsigned char)partySlot);
        }
    }
}

/*
 * PHS screen dispatcher: with nothing held (g_PhsMenuState) it runs the pick-up step
 * Menu_PhsPickUpSelectedMember; with a member held it commits the swap.
 */
// FUNCTION: FF7 0x00700a75
void __cdecl Menu_PhsDispatchConfirmAction(void)
{
    switch (g_PhsMenuState) {
    case PHS_MENU_HOLD_NONE:
        Menu_PhsPickUpSelectedMember();
        break;
    case PHS_MENU_HOLD_MEMBER:
        Menu_PhsCommitSwapSelection();
        break;
    }
}

/*
 * Set the party-change screen up: build every character's stats once, then list
 * the ones not already in the party. The first loop drives gSavemapParty[0]
 * through all nine characters and rebuilds from each, then puts the byte back and
 * rebuilds a tenth time.
 */
// FUNCTION: FF7 0x00700a9f
void __cdecl Menu_PartyChangeInit(int mode)
{
    int count;
    int i;
    unsigned char save;

    save = gSavemap.party[0];
    for (i = 0; i < 9; i++) {
        gSavemap.party[0] = (unsigned char)i;
        Savemap_BuildPartyStatsFromRecord(0);
        Battle_LoadCombatantFromSavemap(0);
    }
    gSavemap.party[0] = save;
    Savemap_BuildPartyStatsFromRecord(0);
    Battle_LoadCombatantFromSavemap(0);
    g_PhsPartyChangeMode = mode;
    g_PhsMenuState = PHS_MENU_HOLD_NONE;
    if (g_MenuIsSmallViewport320x240 != 1) {
        g_PhsMenuFrames = g_PhsMenuFrames2x;
    } else {
        g_PhsMenuFrames = g_PhsMenuFrames1x;
    }
    if (g_PhsPartyChangeMode != 0) {
        Menu_LoadAllCharPortraits(0);
    } else {
        Menu_LoadAllCharPortraits(2);
    }
    g_PhsMenuSwapState = PHS_MENU_FOCUS_PARTY;
    for (i = 0; i < 9; i++) {
        g_PhsMenuPartyMap[i] = 0xff;
    }
    for (count = i = 0; i < 9; i++) {
        if (gSavemap.party[0] != i && gSavemap.party[1] != i &&
            gSavemap.party[2] != i &&
            (gSavemap.phsVisibilityMask & (1 << i)) != 0) {
            g_PhsMenuPartyMap[count] = (unsigned char)i;
            count++;
        }
    }
    g_PhsMenuCursor = 0;
    g_PhsMenuPartySlot = g_PhsMenuCursor;
    g_PhsMenuSavedPartySlot = g_PhsMenuPartySlot;
    g_PhsMenuCounter = g_PhsMenuSavedPartySlot;
}

/*
 * Count the filled party slots and say whether the PHS screen will let you leave:
 * normally all three have to be filled, but while g_PhsPartyChangeMode is set the target
 * comes from g_PhsMenuGroupType instead, which is how a scripted scene asks for a party
 * of one or two. The counter and the index are cleared together -- `mov [i], 0`
 * then `mov eax, [i]` then `mov [n], eax` -- which is `n = i = 0` in the `for`
 * initialiser and not two statements.
 */
// FUNCTION: FF7 0x00700c22
int __cdecl Menu_IsPartySizeSatisfied(void)
{
    int count;
    int i;

    for (count = i = 0; i < 3; i++) {
        if (gSavemap.party[i] != PARTY_CHAR_NONE) {
            count++;
        }
    }
    if (g_PhsPartyChangeMode != 0) {
        if (count != g_PhsMenuGroupType) {
            return 0;
        }
    } else {
        if (count != 3) {
            return 0;
        }
    }
    return 1;
}

/*
 * Runs and draws the PHS party-selection screen in either scale, with entry/exit
 * fading for the special invocation mode. Navigates the three party slots and 3 by
 * 3 reserve grid, delegates selection/swap actions and cancels pending selections
 * or overlays.
 */
// FUNCTION: FF7 0x00700c90
void __cdecl Menu_PhsScreenFrame(unsigned int frameCounter)
{
    struct {
        int actor;
        int column;
        int cancelled;
        int row;
        int cell;
        int slot;
    } v; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    v.cancelled = 0;
    if (g_PhsPartyChangeMode != 0) {
        if (g_PhsMenuSelection == PHS_MENU_FADE_IN) {
            if (Menu_LoadScreenFadeStep(-15) == 0)
                g_PhsMenuSelection = PHS_MENU_FADE_ACTIVE;
        } else if (g_PhsMenuSelection == PHS_MENU_FADE_OUT) {
            if (Menu_LoadScreenFadeStep(15) == 255) {
                g_PhsMenuSelection = PHS_MENU_FADE_DONE;
                g_MenuOverlayActive = 0;
            }
        }
    }
    if ((g_PhsPartyChangeMode != 0 && g_PhsMenuSelection == PHS_MENU_FADE_ACTIVE) ||
        (Menu_GetPhase() == 0 && g_PhsPartyChangeMode == 0)) {
        v.column = g_PhsMenuSwapState ? g_PhsMenuPartySlot : 0;
        if (MenuCursor_ReadRepeat(INPUT_UP, MOUSE_DIR_UP)) {
            switch (g_PhsMenuSwapState) {
            case PHS_MENU_FOCUS_PARTY:
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                g_PhsMenuCounter--;
                if (g_PhsMenuCounter < 0)
                    g_PhsMenuCounter = 2;
                break;
            case PHS_MENU_FOCUS_RESERVE:
                if (g_PhsMenuCursor > 0) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    g_PhsMenuCursor--;
                } else
                    g_PhsMenuCursor = 0;
                break;
            }
        } else if (MenuCursor_ReadRepeat(INPUT_DOWN, MOUSE_DIR_DOWN)) {
            switch (g_PhsMenuSwapState) {
            case PHS_MENU_FOCUS_PARTY:
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                g_PhsMenuCounter++;
                g_PhsMenuCounter %= 3;
                break;
            case PHS_MENU_FOCUS_RESERVE:
                if (g_PhsMenuCursor < 2) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    g_PhsMenuCursor++;
                } else
                    g_PhsMenuCursor = 2;
                break;
            }
        } else if (MenuCursor_ReadPressedOrRepeat(INPUT_LEFT, MOUSE_DIR_LEFT, g_PhsMenuPartySlot)) {
            switch (g_PhsMenuSwapState) {
            case PHS_MENU_FOCUS_RESERVE:
                if (v.column == 0)
                    g_PhsMenuSwapState = PHS_MENU_FOCUS_PARTY;
                else {
                    if (g_PhsMenuPartySlot > 0) {
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        g_PhsMenuPartySlot--;
                    } else
                        g_PhsMenuPartySlot = 0;
                }
                break;
            }
        } else if (MenuCursor_ReadRepeat(INPUT_RIGHT, MOUSE_DIR_RIGHT)) {
            switch (g_PhsMenuSwapState) {
            case PHS_MENU_FOCUS_PARTY:
                g_PhsMenuSwapState = PHS_MENU_FOCUS_RESERVE;
                break;
            case PHS_MENU_FOCUS_RESERVE:
                if (g_PhsMenuPartySlot < 2) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    g_PhsMenuPartySlot++;
                } else
                    g_PhsMenuPartySlot = 2;
                break;
            }
        }
        if (Input_GetPressed(INPUT_CIRCLE)) {
            switch (g_PhsMenuSwapState) {
            case PHS_MENU_FOCUS_PARTY:
            case PHS_MENU_FOCUS_RESERVE:
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                Menu_PhsDispatchConfirmAction();
                break;
            case PHS_MENU_FOCUS_DETAILS:
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                g_PhsMenuSwapState = g_PhsMenuPrevSwapState;
                v.cancelled = 1;
                g_PhsMenuSavedPartySlot = g_PhsMenuCounter;
                break;
            }
        } else if (Input_GetPressed(INPUT_CROSS)) {
            switch (g_PhsMenuSwapState) {
            case PHS_MENU_FOCUS_PARTY:
                if (g_PhsMenuState != PHS_MENU_HOLD_NONE) {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                    g_PhsMenuState = PHS_MENU_HOLD_NONE;
                    v.cancelled = 1;
                }
                break;
            case PHS_MENU_FOCUS_RESERVE:
                if (g_PhsMenuState != PHS_MENU_HOLD_NONE) {
                    g_PhsMenuState = PHS_MENU_HOLD_NONE;
                    v.cancelled = 1;
                }
                break;
            case PHS_MENU_FOCUS_DETAILS:
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                g_PhsMenuSwapState = g_PhsMenuPrevSwapState;
                v.cancelled = 1;
                break;
            }
        }
        if (g_PhsPartyChangeMode != 0) {
            if ((Input_GetPressed(INPUT_START) ||
                 (Input_GetPressed(INPUT_CROSS) && g_PhsMenuState == PHS_MENU_HOLD_NONE)) &&
                !v.cancelled) {
                if (!Menu_IsPartySizeSatisfied())
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                else {
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    g_PhsMenuSelection = PHS_MENU_FADE_OUT;
                    Menu_SetRequestedScreen(0);
                }
            }
        } else {
            if (Input_GetPressed(INPUT_CROSS) && g_PhsMenuState == PHS_MENU_HOLD_NONE && !v.cancelled) {
                if (!Menu_IsPartySizeSatisfied())
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                else {
                    Menu_ReleasePortraitsAndReloadPartyMembers();
                    g_MenuPartyLimitRefreshNeeded = 1;
                    Menu_SetPhase(5, 0);
                    Menu_SetScreen(0);
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                }
            }
        }
    }
    if (g_MenuIsSmallViewport320x240 != 1) {
        if (g_PhsPartyChangeMode != 0) {
            Menu_DrawEncodedString2x(0x1fc, 14, g_PhsMenuFormTexts[0], 7, 0.0f);
            Menu_DrawWindowFrame2x(&g_PhsMenuRect2x, 0.1f);
            if (frameCounter & 0x80) {
                Menu_DrawEncodedString2x(26, 13, g_PhsMenuFormTexts[g_PhsMenuGroupType + 1], 7, 0.1f);
            } else {
                if (g_PhsMenuMemberCount != 0) {
                    if (g_PhsMenuMemberCount < 6)
                        Menu_DrawEncodedString2x(26, 13, g_PhsMenuFormTexts[1], 7, 0.1f);
                    else
                        Menu_DrawEncodedString2x(26, 13, g_PhsMenuFormTexts[2], 7, 0.1f);
                } else
                    Menu_DrawEncodedString2x(26, 13, g_PhsMenuFormTexts[5], 7, 0.1f);
            }
        } else {
            Menu_DrawCommandWindow2x(g_MenuModuleId);
            Menu_DrawEncodedString2x(27, 13, g_PhsMenuFormTexts[4], 7, 0.1f);
        }
        if (g_PhsMenuState != PHS_MENU_HOLD_NONE) {
            switch (g_PhsMenuHeldSource) {
            case PHS_MENU_FOCUS_PARTY:
                if (frameCounter & 2)
                    Menu_DrawMoreTextArrow2x(-5, g_PhsMenuFrames[1].y + 62 + g_PhsMenuSavedCursor * 137, 0.1f);
                break;
            case PHS_MENU_FOCUS_RESERVE:
                if (frameCounter & 2)
                    Menu_DrawMoreTextArrow2x(g_PhsMenuFrames[3].x + 14 + g_PhsMenuSavedColumn * 77,
                                        g_PhsMenuFrames[3].y + 54 + g_PhsMenuSavedCursor * 99, 0.1f);
                break;
            }
        }
        switch (g_PhsMenuSwapState) {
        case PHS_MENU_FOCUS_PARTY:
            Menu_DrawMoreTextArrow2x(0, g_PhsMenuFrames[1].y + 69 + g_PhsMenuCounter * 137, 0.1011f);
            break;
        case PHS_MENU_FOCUS_RESERVE:
            Menu_DrawMoreTextArrow2x(g_PhsMenuFrames[3].x + 19 + g_PhsMenuPartySlot * 77,
                                g_PhsMenuFrames[3].y + 60 + g_PhsMenuCursor * 99, 0.10011f);
            v.actor = g_PhsMenuPartyMap[g_PhsMenuPartySlot + g_PhsMenuCursor * 3];
            if (v.actor != 255) {
                Menu_DrawCharacterHpMpPanel2x(g_PhsMenuFrames[2].x + 131, g_PhsMenuFrames[2].y + 17, v.actor, 0.101f);
                if (Menu_GetPhase() != 5)
                    Menu_DrawCharPortraitById2x(g_PhsMenuFrames[2].x + 38, g_PhsMenuFrames[2].y + 9, 80, 96, v.actor, 0.101f);
            }
            break;
        case PHS_MENU_FOCUS_DETAILS:
            if (g_PhsMenuPrevSwapState == PHS_MENU_FOCUS_RESERVE) {
                v.actor = g_PhsMenuPartyMap[g_PhsMenuPartySlot + g_PhsMenuCursor * 3];
                Menu_DrawCharacterHpMpPanel2x(g_PhsMenuFrames[2].x + 131, g_PhsMenuFrames[2].y + 17, v.actor, 0.1f);
                if (Menu_GetPhase() != 5)
                    Menu_DrawCharPortraitById2x(g_PhsMenuFrames[2].x + 38, g_PhsMenuFrames[2].y + 9, 80, 96, v.actor, 0.101f);
            } else {
                if (frameCounter & 2)
                    Menu_DrawMoreTextArrow2x(0, g_PhsMenuFrames[1].y + 69 + g_PhsMenuSavedPartySlot * 137, 0.1f);
            }
            Menu_DrawPhsEquipmentPanel2x(320, 309);
            Menu_DrawPhsExpPanel2x(frameCounter);
            break;
        }
        if (g_PhsMenuSwapState != PHS_MENU_FOCUS_DETAILS) {
            for (v.row = 0; v.row < 3; v.row++) {
                for (v.cell = 0; v.cell < 3; v.cell++) {
                    v.actor = g_PhsMenuPartyMap[v.cell + v.row * 3];
                    if (v.actor != 255 && Menu_GetPhase() != 5)
                        Menu_DrawCharPortraitById2x(g_PhsMenuFrames[3].x + 45 + v.cell * 77,
                                                    g_PhsMenuFrames[3].y + 13 + v.row * 99, 67, 80, v.actor, 0.101f);
                }
            }
        }
        for (v.slot = 0; v.slot < 3; v.slot++) {
            if (gSavemap.party[v.slot] != PARTY_CHAR_NONE) {
                v.actor = g_CharIdToSaveSlot[gSavemap.party[v.slot]];
                Menu_DrawCharStatBlock2x(134, g_PhsMenuFrames[1].y + 26 + v.slot * 137, v.slot, 0.1f);
                if (Menu_GetPhase() != 5)
                    Menu_DrawCharPortraitById2x(32, g_PhsMenuFrames[1].y + 17 + v.slot * 137, 80, 96, v.actor, 0.11f);
            }
        }
        for (v.slot = 0; v.slot < 3; v.slot++)
            Menu_DrawWindowFrame2x(&g_PhsMenuFrames[v.slot], 0.3f);
        Menu_DrawWindowFrame2x(&g_PhsMenuFrames[3], 0.4f);
    } else {
        if (g_PhsPartyChangeMode != 0) {
            Menu_DrawEncodedString1x(0x106, 7, g_PhsMenuFormTexts[0], 7, 0.0f);
            Menu_DrawWindowFrame1x(&g_PhsMenuRect1x, 0.1f);
            if (frameCounter & 0x80) {
                Menu_DrawEncodedString1x(13, 7, g_PhsMenuFormTexts[g_PhsMenuGroupType + 1], 7, 0.1f);
            } else {
                if (g_PhsMenuMemberCount != 0) {
                    if (g_PhsMenuMemberCount < 6)
                        Menu_DrawEncodedString1x(13, 7, g_PhsMenuFormTexts[1], 7, 0.1f);
                    else
                        Menu_DrawEncodedString1x(13, 7, g_PhsMenuFormTexts[2], 7, 0.1f);
                } else
                    Menu_DrawEncodedString1x(13, 7, g_PhsMenuFormTexts[5], 7, 0.1f);
            }
        } else {
            Menu_DrawCommandWindow1x(g_MenuModuleId);
            Menu_DrawEncodedString1x(13, 7, g_PhsMenuFormTexts[4], 7, 0.1f);
        }
        if (g_PhsMenuState != PHS_MENU_HOLD_NONE) {
            switch (g_PhsMenuHeldSource) {
            case PHS_MENU_FOCUS_PARTY:
                if (frameCounter & 2)
                    Menu_DrawMoreTextArrow1x(-3, g_PhsMenuFrames[1].y + 31 + g_PhsMenuSavedCursor * 69, 0.1f);
                break;
            case PHS_MENU_FOCUS_RESERVE:
                if (frameCounter & 2)
                    Menu_DrawMoreTextArrow1x(g_PhsMenuFrames[3].x + 7 + g_PhsMenuSavedColumn * 39,
                                        g_PhsMenuFrames[3].y + 28 + g_PhsMenuSavedCursor * 50, 0.1f);
                break;
            }
        }
        switch (g_PhsMenuSwapState) {
        case PHS_MENU_FOCUS_PARTY:
            Menu_DrawMoreTextArrow1x(0, g_PhsMenuFrames[1].y + 35 + g_PhsMenuCounter * 69, 0.1011f);
            break;
        case PHS_MENU_FOCUS_RESERVE:
            Menu_DrawMoreTextArrow1x(g_PhsMenuFrames[3].x + 10 + g_PhsMenuPartySlot * 39,
                                g_PhsMenuFrames[3].y + 31 + g_PhsMenuCursor * 50, 0.10011f);
            v.actor = g_PhsMenuPartyMap[g_PhsMenuPartySlot + g_PhsMenuCursor * 3];
            if (v.actor != 255) {
                Menu_DrawCharacterHpMpPanel1x(g_PhsMenuFrames[2].x + 66, g_PhsMenuFrames[2].y + 9, v.actor, 0.101f);
                if (Menu_GetPhase() != 5)
                    Menu_DrawCharPortraitById1x(g_PhsMenuFrames[2].x + 19, g_PhsMenuFrames[2].y + 5, 40, 48, v.actor, 0.101f);
            }
            break;
        case PHS_MENU_FOCUS_DETAILS:
            if (g_PhsMenuPrevSwapState == PHS_MENU_FOCUS_RESERVE) {
                v.actor = g_PhsMenuPartyMap[g_PhsMenuPartySlot + g_PhsMenuCursor * 3];
                Menu_DrawCharacterHpMpPanel1x(g_PhsMenuFrames[2].x + 66, g_PhsMenuFrames[2].y + 9, v.actor, 0.1f);
                if (Menu_GetPhase() != 5)
                    Menu_DrawCharPortraitById1x(g_PhsMenuFrames[2].x + 19, g_PhsMenuFrames[2].y + 5, 40, 48, v.actor, 0.101f);
            } else {
                if (frameCounter & 2)
                    Menu_DrawMoreTextArrow1x(0, g_PhsMenuFrames[1].y + 35 + g_PhsMenuSavedPartySlot * 69, 0.1f);
            }
            Menu_DrawPhsEquipmentPanel1x(160, 155);
            Menu_DrawPhsExpPanel1x(frameCounter);
            break;
        }
        if (g_PhsMenuSwapState != PHS_MENU_FOCUS_DETAILS) {
            for (v.row = 0; v.row < 3; v.row++) {
                for (v.cell = 0; v.cell < 3; v.cell++) {
                    v.actor = g_PhsMenuPartyMap[v.cell + v.row * 3];
                    if (v.actor != 255 && Menu_GetPhase() != 5)
                        Menu_DrawCharPortraitById1x(g_PhsMenuFrames[3].x + 23 + v.cell * 39,
                                                    g_PhsMenuFrames[3].y + 7 + v.row * 50, 34, 40, v.actor, 0.101f);
                }
            }
        }
        for (v.slot = 0; v.slot < 3; v.slot++) {
            if (gSavemap.party[v.slot] != PARTY_CHAR_NONE) {
                v.actor = g_CharIdToSaveSlot[gSavemap.party[v.slot]];
                Menu_DrawCharStatBlock1x(67, g_PhsMenuFrames[1].y + 13 + v.slot * 69, v.slot, 0.1f);
                if (Menu_GetPhase() != 5)
                    Menu_DrawCharPortraitById1x(16, g_PhsMenuFrames[1].y + 9 + v.slot * 69, 40, 48, v.actor, 0.11f);
            }
        }
        for (v.slot = 0; v.slot < 3; v.slot++)
            Menu_DrawWindowFrame1x(&g_PhsMenuFrames[v.slot], 0.3f);
        Menu_DrawWindowFrame1x(&g_PhsMenuFrames[3], 0.4f);
    }
}

/*
 * Draws the selected character's (g_PhsMenuCharIndex) byte gauge, two dword decimal
 * values and the two width-aligned Kernel_LookupUiText labels at 2x scale, with
 * the retained atlas frame decorations and float depth operands.
 */
// FUNCTION: FF7 0x00701b4c
void __cdecl Menu_DrawPhsExpPanel2x(int frameCounter)
{
    int topY;
    int member;
    int panelX;
    int characterIndex;

    topY = 0xAE;
    panelX = 0x33;
    characterIndex = g_PhsMenuCharIndex;
    member = characterIndex;
    Menu_DrawGaugeBar2x(0x1A9, topY + 0x2C, *(unsigned char *)&gSavemap.chars[characterIndex].levelProgress << 1, 0xD, 0x80802020, 0.1001f);
    Menu_DrawAtlasQuad2x(0x1A5, topY + 0x2D, 0x88, 8, 0x40, 8, 7, 0, 0, 0.101f);
    Menu_DrawAtlasQuad2x(0x248, topY + 0x11, 0xF8, 8, 8, 8, 7, 0, 0, 0.101f);
    Menu_DrawAtlasQuad2x(0x248, topY + 0x47, 0xF8, 8, 8, 8, 7, 0, 0, 0.101f);
    Menu_DrawDecimalNumber2x(panelX + 0x19D, topY + 0x11, gSavemap.chars[characterIndex].exp, 0xA, 7, 0.101f);
    Menu_DrawDecimalNumber2x(panelX + 0x19D, topY + 0x47, gSavemap.chars[characterIndex].expToNext, 0xA, 7, 0.101f);
    Menu_DrawEncodedString2x(panelX + 0x16F - Menu_MeasureEncodedString2x((unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 9, 8)), topY + 0xC, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 9, 8), 7, 0.101f);
    Menu_DrawEncodedString2x(panelX + 0x16F - Menu_MeasureEncodedString2x((unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x2C, 8)), topY + 0x42, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x2C, 8), 7, 0.101f);
}

/*
 * Draws the selected character's (g_PhsMenuCharIndex) byte gauge, two dword decimal
 * values and the two width-aligned Kernel_LookupUiText labels at 1x scale, with
 * the retained atlas frame decorations and float depth operands.
 */
// FUNCTION: FF7 0x00701d19
void __cdecl Menu_DrawPhsExpPanel1x(int frameCounter)
{
    int topY;
    int member;
    int panelX;
    int characterIndex;

    topY = 0x57;
    panelX = 0x1A;
    characterIndex = g_PhsMenuCharIndex;
    member = characterIndex;
    Menu_DrawGaugeBar1x(0xD5, topY + 0x17, *(unsigned char *)&gSavemap.chars[characterIndex].levelProgress, 6, 0x80802020, 0.1001f);
    Menu_DrawAtlasQuad1x(0xD3, topY + 0x17, 0x88, 8, 0x40, 8, 7, 0, 0, 0.101f);
    Menu_DrawAtlasQuad1x(0x124, topY + 9, 0xF8, 8, 8, 8, 7, 0, 0, 0.101f);
    Menu_DrawAtlasQuad1x(0x124, topY + 0x24, 0xF8, 8, 8, 8, 7, 0, 0, 0.101f);
    Menu_DrawDecimalNumber1x(panelX + 0xCF, topY + 9, gSavemap.chars[characterIndex].exp, 0xA, 7, 0.101f);
    Menu_DrawDecimalNumber1x(panelX + 0xCF, topY + 0x24, gSavemap.chars[characterIndex].expToNext, 0xA, 7, 0.101f);
    Menu_DrawEncodedString1x(panelX + 0xB8 - Menu_MeasureEncodedString1x((unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 9, 8)), topY + 6, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 9, 8), 7, 0.101f);
    Menu_DrawEncodedString1x(panelX + 0xB8 - Menu_MeasureEncodedString1x((unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x2C, 8)), topY + 0x21, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_BATTLE_MESSAGE, 0x2C, 8), 7, 0.101f);
}

/*
 * Initializes and advances a modal party-change screen. On first entry sets its
 * mode from mode, mapping 0 and values 4 through 7 to 3 and value 8 to 2, while
 * recording special values at least 4.
 */
// FUNCTION: FF7 0x00701ee4
void __cdecl Menu_PhsModuleUpdate(int mode)
{
    unsigned char actor;
    int i;

    if (g_PhsMenuActive == 0) {
        g_MenuModuleId = 9;
        g_PhsMenuGroupType = mode;
        g_PhsMenuMemberCount = 0;
        if (mode == 0) {
            g_PhsMenuGroupType = 3;
        } else if (mode < 4) {
            g_PhsMenuGroupType = mode;
        } else {
            g_PhsMenuMemberCount = mode;
            switch (mode - 4) {
            case PHS_SPLIT_OFFSET_3PARTY_A:
            case PHS_SPLIT_OFFSET_3PARTY_B:
            case PHS_SPLIT_OFFSET_3PARTY_C:
            case PHS_SPLIT_OFFSET_3PARTY_D:
                g_PhsMenuGroupType = 3;
                break;
            case PHS_SPLIT_OFFSET_2PARTY:
                g_PhsMenuGroupType = 2;
                break;
            }
        }
        g_PhsMenuSelection = PHS_MENU_FADE_IN;
        g_PhsMenuFrameCount = g_PhsMenuSelection;
        Menu_PartyChangeInit(1);
        g_PhsMenuFrameParity = 0;
        Input_SetRepeatRates(0xc8, 0x32);
        g_MenuOverlayActive = 1;
        g_PhsMenuActive = g_MenuOverlayActive;
    }
    Menu_PhsScreenFrame(g_PhsMenuFrameCount);
    if (g_PhsMenuSelection == PHS_MENU_FADE_DONE) {
        g_MenuOverlayActive = 0;
        g_PhsMenuActive = g_MenuOverlayActive;
    }
    g_PhsMenuFrameParity ^= 1;
    g_PhsMenuFrameCount++;
    if (g_PhsMenuActive != 1 && g_PhsMenuSelection == PHS_MENU_FADE_DONE) {
        for (i = 0; i < 3; i++) {
            actor = gSavemap.party[i];
            if (actor == 0xff) {
                continue;
            }
            Savemap_BuildPartyStatsFromRecord(i);
            Battle_LoadCombatantFromSavemap((unsigned char)i);
        }
        Savemap_ReloadActivePartyStats();
        Menu_RefreshPartyLimitTables();
        g_MenuExitRequested = 1;
    }
}

/*
 * Clears g_PhsMenuActive.
 */
// FUNCTION: FF7 0x00702085
void __cdecl Menu_ClearPartyChangeScreenActiveFlag(void)
{
    g_PhsMenuActive = 0;
}
} /* extern "C" */
