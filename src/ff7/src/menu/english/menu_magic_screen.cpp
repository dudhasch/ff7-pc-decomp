// src/ff7/src/menu/english/menu_magic_screen.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The magic menu screen: its setup, per-frame update and input, the HP-full check, and
//   the character panels that summarize stats and magic in both scales.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/menu.h"
#include "lib/enums/file.h"
#include "lib/enums/sound.h"
#include "ff7/api/menu.h"
#include "lib/api/file.h"
#include <string.h>
#include <stdlib.h>
#include "lib/api/sound.h"
#include "ff7/savemap.h"

#include "ff7/enums/menu.h"
#include "ff7/enums/battle.h"
#include "lib/api/graphics.h"
#include "ff7/enums/main.h"
extern "C" {

// Text: "Magic" "Summon" "Enemy-Skill" "Added-Ability" "All" "4x-M" "Absorb MP" "Absorb HP"
//       "Steal as well" "Extra cut" "Turbo MP"
// GLOBAL: FF7 0x00920fe8
unsigned char g_MenuMagicLabelTable[240] = {0x2d, 0x41, 0x47, 0x49, 0x43, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x55, 0x4d, 0x4d, 0x4f, 0x4e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x4e, 0x45, 0x4d, 0x59, 0x0d, 0x33, 0x4b, 0x49, 0x4c, 0x4c, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x44, 0x44, 0x45, 0x44, 0x0d, 0x21, 0x42, 0x49, 0x4c, 0x49, 0x54, 0x59, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x4c, 0x4c, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x58, 0x0d, 0x2d, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x42, 0x53, 0x4f, 0x52, 0x42, 0x00, 0x2d, 0x30, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x42, 0x53, 0x4f, 0x52, 0x42, 0x00, 0x28, 0x30, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x54, 0x45, 0x41, 0x4c, 0x00, 0x41, 0x53, 0x00, 0x57, 0x45, 0x4c, 0x4c, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x58, 0x54, 0x52, 0x41, 0x00, 0x43, 0x55, 0x54, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x55, 0x52, 0x42, 0x4f, 0x00, 0x2d, 0x30, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// Text: "Nothing." "Not enough MP."
// GLOBAL: FF7 0x009210d8
unsigned char g_MenuMagicMessageTable[40] = {0x2e, 0x4f, 0x54, 0x48, 0x49, 0x4e, 0x47, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2e, 0x4f, 0x54, 0x00, 0x45, 0x4e, 0x4f, 0x55, 0x47, 0x48, 0x00, 0x2d, 0x30, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00921100
int g_MenuMagicState = (-1);
// GLOBAL: FF7 0x00921108
struct EnglishWindowFrame g_MenuMagicWindowRects2x[6] = {{0, 0, 640, 163}, {0, 163, 640, 51}, {0, 206, 640, 274}, {476, 43, 164, 120}, {498, 43, 142, 120}, {476, 0, 164, 51}};
// GLOBAL: FF7 0x00921138
struct EnglishWindowFrame g_MenuMagicWindowRects1x[6] = {{0, 0, 320, 82}, {0, 82, 320, 26}, {0, 103, 320, 137}, {246, 22, 74, 60}, {249, 22, 71, 60}, {246, 0, 74, 26}};
// GLOBAL: FF7 0x00dd1680
short g_MenuMagicScrollBar[7] = {0, 0, 0, 0, 0, 0, 0};
// GLOBAL: FF7 0x00dd1690
struct EnglishWindowFrame *g_MenuMagicWindowRects = 0;
// GLOBAL: FF7 0x00dd1694
int g_MenuMagicTargetAll = 0;
// GLOBAL: FF7 0x00dd1698
int g_MenuMagicListCol = 0;
// GLOBAL: FF7 0x00dd169c
int g_MenuMagicListRow = 0;
unsigned char g_bssPad_00dd16a0[48] = {0};
// GLOBAL: FF7 0x00dd16d0
int g_MenuMagicTargetCol = 0;
// The target cursor's row (its MenuCursor starts at g_MenuMagicTargetCol): the party row the arrow points at.
// GLOBAL: FF7 0x00dd16d4
int g_MenuMagicTargetRow = 0;
unsigned char g_bssPad_00dd16d8[48] = {0};
// GLOBAL: FF7 0x00dd1708
int g_MenuMagicSpellCol = 0;
// GLOBAL: FF7 0x00dd170c
int g_MenuMagicSpellRow = 0;
unsigned char g_bssPad_00dd1710[12] = {0};
// GLOBAL: FF7 0x00dd171c
int g_MenuMagicSpellTop = 0;
unsigned char g_bssPad_00dd1720[12] = {0};
// GLOBAL: FF7 0x00dd172c
int g_MenuMagicSpellSlide = 0;
unsigned char g_bssPad_00dd1730[8] = {0};
// GLOBAL: FF7 0x00dd1738
int g_MenuMagicSpellMoving = 0;
unsigned char g_bssPad_00dd173c[4] = {0};
// GLOBAL: FF7 0x00dd1740
int g_MenuMagicSummonCol = 0;
// GLOBAL: FF7 0x00dd1744
int g_MenuMagicSummonRow = 0;
unsigned char g_bssPad_00dd1748[12] = {0};
// GLOBAL: FF7 0x00dd1754
int g_MenuMagicSummonTop = 0;
unsigned char g_bssPad_00dd1758[12] = {0};
// GLOBAL: FF7 0x00dd1764
int g_MenuMagicSummonSlide = 0;
unsigned char g_bssPad_00dd1768[8] = {0};
// GLOBAL: FF7 0x00dd1770
int g_MenuMagicSummonMoving = 0;
unsigned char g_bssPad_00dd1774[4] = {0};
// GLOBAL: FF7 0x00dd1778
int g_MenuMagicSkillCol = 0;
// GLOBAL: FF7 0x00dd177c
int g_MenuMagicSkillRow = 0;
unsigned char g_bssPad_00dd1780[12] = {0};
// GLOBAL: FF7 0x00dd178c
int g_MenuMagicSkillTop = 0;
unsigned char g_bssPad_00dd1790[12] = {0};
// GLOBAL: FF7 0x00dd179c
int g_MenuMagicSkillSlide = 0;
unsigned char g_bssPad_00dd17a0[8] = {0};
// GLOBAL: FF7 0x00dd17a8
int g_MenuMagicSkillMoving = 0;
unsigned char g_bssPad_00dd17ac[4] = {0};
unsigned char g_bssPad_00dd17b0[56] = {0};
// GLOBAL: FF7 0x00dd17e8
int g_MenuMagicPartyIndex = 0;
unsigned char g_bssPad_00dd17ec[4] = {0};
// clang-format on

#pragma intrinsic(abs, memcpy, memset)

/*
 * Initializes magic menu screen state, layouts, and lists.
 */
// FUNCTION: FF7 0x00710c50
void __cdecl Menu_MagicScreenInit(void)
{
    int partyIdx;
    int spellIndex;
    BattleAbilitySlot *base;

    if (g_MenuIsSmallViewport320x240 != 1) {
        g_MenuMagicWindowRects = g_MenuMagicWindowRects2x;
    } else {
        g_MenuMagicWindowRects = g_MenuMagicWindowRects1x;
    }
    MenuCursor_Init((MenuCursor *)&g_MenuMagicListCol, 0, 0, 1, 3, 0, 0, 1, 3, 0, 0, 0, 1, 0, 0);
    MenuCursor_Init((MenuCursor *)&g_MenuMagicTargetCol, 0, 0, 1, 3, 0, 0, 1, 3, 0, 0, 0, 1, 0, 0);
    MenuCursor_Init((MenuCursor *)&g_MenuMagicSpellCol, 0, 0, 3, 7, 0, 0, 3, 0x12, 0, 0, 2, 0, 0, 1);
    MenuCursor_Init((MenuCursor *)&g_MenuMagicSummonCol, 0, 0, 2, 7, 0, 0, 2, 8, 0, 0, 2, 0, 0, 1);
    MenuCursor_Init((MenuCursor *)&g_MenuMagicSkillCol, 0, 0, 2, 7, 0, 0, 2, 0xC, 0, 0, 2, 0, 0, 1);
    g_MenuMagicState = MENU_MAGIC_STATE_COMMAND_LIST;
    g_MenuMagicPartyIndex = g_MenuPartyMemberIndex;
    for (partyIdx = 0; partyIdx < 3; partyIdx++) {
        base = g_MenuPartyStats[partyIdx].spells;
        for (spellIndex = 0; spellIndex < 0x38; spellIndex++) {
            if (base[spellIndex].attackId != 0xFF && (base[spellIndex].allCount != 0 && ((base[spellIndex].targetFlags & BTL_TARGETF_DEFAULT_MULTIPLE) == 0 || (base[spellIndex].targetFlags & BTL_TARGETF_TOGGLE_MULTIPLE) == 0))) {
                base[spellIndex].allCount = 0;
            }
        }
    }
}

/*
 * Updates and renders magic menu screen frame and handles user input.
 */
// FUNCTION: FF7 0x00710dfa
void __cdecl Menu_MagicScreenFrame(unsigned int frameCounter)
{
    struct {
        int panelX;
        int selected;
        BattleAbilitySlot *spells;
        int textWidth;
        int rows;
        int row;
        int column;
        int valid;
        int i;
        int count;
        short rect[4];
        int colour;
        int panelY;
    } v; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (g_MenuIsSmallViewport320x240 != 1) {
        if (g_MenuMagicState != MENU_MAGIC_STATE_COMMAND_LIST) {
            Menu_DrawEncodedString2x(g_MenuMagicWindowRects[5].x + 32, g_MenuMagicWindowRects[5].y + 13,
                              g_MenuMagicLabelTable + g_MenuMagicListRow * 20, 7, 0.0f);
            Menu_DrawWindowFrame2x(&g_MenuMagicWindowRects[5], 0.01f);
        } else {
            Menu_DrawCommandWindow2x(g_MenuModuleId);
        }
        switch (g_MenuMagicState) {
        case MENU_MAGIC_STATE_COMMAND_LIST:
            Menu_DrawMoreTextArrow2x(g_MenuMagicWindowRects[3].x - 26, g_MenuMagicWindowRects[3].y + 19 + g_MenuMagicListRow * 34, 0.1f);
            Menu_DrawEncodedString2x(g_MenuMagicWindowRects[3].x + 32, g_MenuMagicWindowRects[3].y + 13, g_MenuMagicLabelTable, 7, 0.101f);
            if (gSavemap.magicMenuFlags & MAGIC_MENU_FLAG_SUMMON)
                Menu_DrawEncodedString2x(g_MenuMagicWindowRects[3].x + 32, g_MenuMagicWindowRects[3].y + 47, g_MenuMagicLabelTable + MENU_MAGIC_ROW_SUMMON * 20, 7, 0.101f);
            if (gSavemap.magicMenuFlags & MAGIC_MENU_FLAG_ENEMY_SKILL)
                Menu_DrawEncodedString2x(g_MenuMagicWindowRects[3].x + 32, g_MenuMagicWindowRects[3].y + 81, g_MenuMagicLabelTable + MENU_MAGIC_ROW_ENEMY_SKILL * 20, 7, 0.101f);
            Menu_DrawWindowFrame2x(&g_MenuMagicWindowRects[3], 0.111f);
            break;
        case MENU_MAGIC_STATE_SELECT_TARGET:
        case MENU_MAGIC_STATE_SPELL_LIST:
            v.spells = g_MenuPartyStats[g_MenuMagicPartyIndex].spells;
            v.selected = g_MenuMagicSpellCol + g_MenuMagicSpellRow * 3 + g_MenuMagicSpellTop * 3;
            if (g_MenuMagicState == MENU_MAGIC_STATE_SELECT_TARGET) {
                v.panelX = 167;
                v.panelY = 107;
                if (g_MenuMagicTargetAll != 0)
                    Menu_DrawMoreTextArrow2x(v.panelX, v.panelY + 60 + (int)frameCounter % 3 * 120, 0.0f);
                else
                    Menu_DrawMoreTextArrow2x(v.panelX, v.panelY + 60 + g_MenuMagicTargetRow * 120, 0.0f);
                for (v.i = 0; v.i < 3; v.i++) {
                    if (gSavemap.party[v.i] != PARTY_CHAR_NONE) {
                        Menu_DrawCharStatBlock2x(v.panelX + 128, v.panelY + 11 + v.i * 120, v.i, 0.010011f);
                        Menu_DrawCharPortrait2x(v.panelX + 35, v.panelY + 11 + v.i * 120, v.i, 0.010011f);
                    }
                }
                v.textWidth = Menu_MeasureEncodedString2x((unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_MAGIC, v.spells[v.selected].attackId, 8));
                Menu_DrawEncodedString2x(v.panelX + 228 - v.textWidth / 2, v.panelY - 30,
                                  (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_MAGIC, v.spells[v.selected].attackId, 8), 7, 0.0f);
                Menu_SetRectShorts(v.rect, v.panelX + 154, v.panelY - 43, 154, 51);
                Menu_DrawWindowFrame2x((EnglishWindowFrame *)v.rect, 0.0f);
                Menu_SetRectShorts(v.rect, v.panelX, v.panelY, 293, 366);
                Menu_DrawWindowFrame2x((EnglishWindowFrame *)v.rect, 0.1f);
                if (frameCounter & 2)
                    Menu_DrawMoreTextArrow2x(g_MenuMagicWindowRects[2].x + 20 + g_MenuMagicSpellCol * 21 * 8, g_MenuMagicWindowRects[2].y + 24 + g_MenuMagicSpellRow * 36, 0.111f);
            } else {
                Menu_DrawMoreTextArrow2x(g_MenuMagicWindowRects[2].x + 20 + g_MenuMagicSpellCol * 21 * 8, g_MenuMagicWindowRects[2].y + 24 + g_MenuMagicSpellRow * 36, 0.0f);
            }
            if (v.spells[v.selected].attackId != 255)
                Menu_DrawEncodedString2x(27, g_MenuMagicWindowRects[1].y + 13, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_MAGIC, v.spells[v.selected].attackId, 0), 7, 0.2f);
            Menu_DrawEncodedString2x(g_MenuMagicWindowRects[0].x + 227, g_MenuMagicWindowRects[0].y + 21, g_MenuMagicLabelTable + MENU_MAGIC_LABEL_ADDED_ABILITY * 20, 5, 0.101f);
            v.count = 0;
            if (v.spells[v.selected].attackId != 255) {
                for (v.i = 0; v.i < 8; v.i++) {
                    switch (v.i) {
                    case MENU_MAGIC_ADDED_ALL:
                        if (v.spells[v.selected].allCount != 0)
                            v.colour = 7;
                        else
                            v.colour = 0;
                        break;
                    case MENU_MAGIC_ADDED_QUADRA_MAGIC:
                        if (v.spells[v.selected].quadraCount != 0)
                            v.colour = 7;
                        else
                            v.colour = 0;
                        break;
                    default:
                        if (v.i == MENU_MAGIC_ADDED_TURBO_MP) {
                            if (((v.spells[v.selected].support >> 5) & 7) != 0)
                                v.colour = 7;
                            else
                                v.colour = 0;
                        } else {
                            if (v.spells[v.selected].support & (1 << (v.i - MENU_MAGIC_ADDED_MP_ABSORB)))
                                v.colour = 7;
                            else
                                v.colour = 0;
                        }
                    }
                    if (v.colour != 0) {
                        Menu_DrawEncodedString2x(g_MenuMagicWindowRects[0].x + 248 + v.count / 4 * 120, g_MenuMagicWindowRects[0].y + 45 + (v.count & 3) * 26,
                                          g_MenuMagicLabelTable + (v.i + MENU_MAGIC_LABEL_FIRST_ADDED) * 20, 7, 0.2f);
                        if (v.i == MENU_MAGIC_ADDED_ALL) {
                            Menu_DrawDecimalNumber2x(g_MenuMagicWindowRects[0].x + 307 + v.count / 4 * 120, g_MenuMagicWindowRects[0].y + 48 + (v.count & 3) * 26, v.spells[v.selected].allCount, 3, 7, 0.2f);
                            Menu_DrawAtlasQuad2x(g_MenuMagicWindowRects[0].x + 344 + v.count / 4 * 120, g_MenuMagicWindowRects[0].y + 48 + (v.count & 3) * 26, 120, 8, 8, 8, 7, 0, 0, 0.2f);
                        } else if (v.i == MENU_MAGIC_ADDED_QUADRA_MAGIC) {
                            Menu_DrawDecimalNumber2x(g_MenuMagicWindowRects[0].x + 307 + v.count / 4 * 120, g_MenuMagicWindowRects[0].y + 48 + (v.count & 3) * 26, v.spells[v.selected].quadraCount, 3, 7, 0.2f);
                            Menu_DrawAtlasQuad2x(g_MenuMagicWindowRects[0].x + 344 + v.count / 4 * 120, g_MenuMagicWindowRects[0].y + 48 + (v.count & 3) * 26, 120, 8, 8, 8, 7, 0, 0, 0.2f);
                        } else if (v.i == MENU_MAGIC_ADDED_TURBO_MP) {
                            Menu_DrawDecimalNumber2x(g_MenuMagicWindowRects[0].x + 315 + v.count / 4 * 120, g_MenuMagicWindowRects[0].y + 48 + (v.count & 3) * 26, (v.spells[v.selected].support >> 5) & 7, 3, 7, 0.2f);
                        }
                        v.count++;
                    }
                }
                if (v.count == 0)
                    Menu_DrawEncodedString2x(g_MenuMagicWindowRects[0].x + 257, g_MenuMagicWindowRects[0].y + 56, g_MenuMagicMessageTable, 7, 0.101f);
            }
            Menu_DrawAtlasQuad2x(g_MenuMagicWindowRects[4].x + 27, g_MenuMagicWindowRects[4].y + 13, 0, 16, 40, 16, 1, 0, 0, 0.01f);
            if (v.spells[v.selected].attackId != 255)
                Menu_DrawDecimalNumberZeroPad2x(g_MenuMagicWindowRects[4].x + 40, g_MenuMagicWindowRects[4].y + 77, v.spells[v.selected].mpCost, 3, 7, 0.0f);
            g_MenuMagicScrollBar[0] = 7;
            g_MenuMagicScrollBar[1] = 18;
            g_MenuMagicScrollBar[2] = g_MenuMagicSpellTop;
            g_MenuMagicScrollBar[3] = 618;
            g_MenuMagicScrollBar[4] = g_MenuMagicWindowRects[2].y + 9;
            g_MenuMagicScrollBar[5] = 17;
            g_MenuMagicScrollBar[6] = g_MenuMagicWindowRects[2].height - 15;
            Menu_DrawScrollBar2x(g_MenuMagicScrollBar, 0.0f);
            Menu_FlushAndSetScissor2x(0, g_MenuMagicWindowRects[2].y + 9, 640, g_MenuMagicWindowRects[2].height - 15);
            if (g_MenuMagicSpellMoving != 0)
                v.rows = 8;
            else
                v.rows = 7;
            for (v.row = 0; v.row < v.rows; v.row++) {
                for (v.column = 0; v.column < 3; v.column++) {
                    switch (v.spells[v.column + g_MenuMagicSpellTop * 3 + v.row * 3].attackId) {
                    case MENU_SPELL_CURE:
                    case MENU_SPELL_CURE2:
                    case MENU_SPELL_CURE3:
                    case MENU_SPELL_LIFE:
                    case MENU_SPELL_LIFE2:
                    case MENU_SPELL_FULL_CURE:
                        v.colour = 7;
                        break;
                    default:
                        v.colour = 0;
                    }
                    if (v.spells[v.column + g_MenuMagicSpellTop * 3 + v.row * 3].attackId != 255)
                        Menu_DrawEncodedString2x(v.column * 21 * 8 + 77, g_MenuMagicWindowRects[2].y + 17 + v.row * 36 + g_MenuMagicSpellSlide * 9,
                                          (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_MAGIC, v.spells[v.column + v.row * 3 + g_MenuMagicSpellTop * 3].attackId, 8), (char)v.colour, 0.2f);
                }
            }
            Menu_FlushAndResetScissor2x();
            Menu_DrawWindowFrame2x(&g_MenuMagicWindowRects[4], 0.1f);
            break;
        case MENU_MAGIC_STATE_SUMMON_LIST:
            Menu_DrawMoreTextArrow2x(g_MenuMagicWindowRects[2].x + g_MenuMagicSummonCol * 309, g_MenuMagicWindowRects[2].y + 24 + g_MenuMagicSummonRow * 36, 0.0f);
            v.spells = g_MenuPartyStats[g_MenuMagicPartyIndex].summons;
            v.selected = g_MenuMagicSummonCol + g_MenuMagicSummonRow * 2 + g_MenuMagicSummonTop * 2;
            if (v.spells[v.selected].attackId != 255)
                Menu_DrawEncodedString2x(27, g_MenuMagicWindowRects[1].y + 13, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_SUMMON, v.selected, 0), 7, 0.2f);
            Menu_DrawEncodedString2x(g_MenuMagicWindowRects[0].x + 227, g_MenuMagicWindowRects[0].y + 21, g_MenuMagicLabelTable + MENU_MAGIC_LABEL_ADDED_ABILITY * 20, 5, 0.101f);
            v.count = 0;
            if (v.spells[v.selected].attackId != 255) {
                for (v.i = 0; v.i < 8; v.i++) {
                    switch (v.i) {
                    case MENU_MAGIC_ADDED_ALL:
                        v.colour = 0;
                        break;
                    case MENU_MAGIC_ADDED_QUADRA_MAGIC:
                        if (v.spells[v.selected].quadraCount != 0)
                            v.colour = 7;
                        else
                            v.colour = 0;
                        break;
                    default:
                        if (v.i == MENU_MAGIC_ADDED_TURBO_MP) {
                            if (((v.spells[v.selected].support >> 5) & 7) != 0)
                                v.colour = 7;
                            else
                                v.colour = 0;
                        } else {
                            if (v.spells[v.selected].support & (1 << (v.i - MENU_MAGIC_ADDED_MP_ABSORB)))
                                v.colour = 7;
                            else
                                v.colour = 0;
                        }
                    }
                    if (v.colour != 0 && v.i != MENU_MAGIC_ADDED_ALL) {
                        Menu_DrawEncodedString2x(g_MenuMagicWindowRects[0].x + 248 + v.count / 4 * 120, g_MenuMagicWindowRects[0].y + 45 + (v.count & 3) * 26,
                                          g_MenuMagicLabelTable + (v.i + MENU_MAGIC_LABEL_FIRST_ADDED) * 20, 7, 0.2f);
                        if (v.i == MENU_MAGIC_ADDED_ALL) {
                            Menu_DrawDecimalNumber2x(g_MenuMagicWindowRects[0].x + 307 + v.count / 4 * 120, g_MenuMagicWindowRects[0].y + 48 + (v.count & 3) * 26, v.spells[v.selected].allCount, 3, 7, 0.2f);
                            /* Original IMUL immediate is 0x1fb8, even in this unreachable All arm. */
                            Menu_DrawAtlasQuad2x(g_MenuMagicWindowRects[0].x + 344 + v.count / 4 * 8120, g_MenuMagicWindowRects[0].y + 48 + (v.count & 3) * 26, 120, 8, 8, 8, 7, 0, 0, 0.2f);
                        } else if (v.i == MENU_MAGIC_ADDED_QUADRA_MAGIC) {
                            Menu_DrawDecimalNumber2x(g_MenuMagicWindowRects[0].x + 307 + v.count / 4 * 120, g_MenuMagicWindowRects[0].y + 48 + (v.count & 3) * 26, v.spells[v.selected].quadraCount, 3, 7, 0.2f);
                            Menu_DrawAtlasQuad2x(g_MenuMagicWindowRects[0].x + 344 + v.count / 4 * 120, g_MenuMagicWindowRects[0].y + 48 + (v.count & 3) * 26, 120, 8, 8, 8, 7, 0, 0, 0.2f);
                        } else if (v.i == MENU_MAGIC_ADDED_TURBO_MP) {
                            Menu_DrawDecimalNumber2x(g_MenuMagicWindowRects[0].x + 315 + v.count / 4 * 120, g_MenuMagicWindowRects[0].y + 48 + (v.count & 3) * 26, (v.spells[v.selected].support >> 5) & 7, 3, 7, 0.2f);
                        }
                        v.count++;
                    }
                }
                if (v.count == 0)
                    Menu_DrawEncodedString2x(g_MenuMagicWindowRects[0].x + 246, g_MenuMagicWindowRects[0].y + 56, g_MenuMagicMessageTable, 7, 0.101f);
            }
            Menu_DrawAtlasQuad2x(g_MenuMagicWindowRects[4].x + 27, g_MenuMagicWindowRects[4].y + 13, 0, 16, 40, 16, 1, 0, 0, 0.01f);
            if (v.spells[v.selected].attackId != 255)
                Menu_DrawDecimalNumberZeroPad2x(g_MenuMagicWindowRects[4].x + 38, g_MenuMagicWindowRects[4].y + 77, v.spells[v.selected].mpCost, 3, 7, 0.101f);
            g_MenuMagicScrollBar[0] = 7;
            g_MenuMagicScrollBar[1] = 8;
            g_MenuMagicScrollBar[2] = g_MenuMagicSummonTop;
            g_MenuMagicScrollBar[3] = 618;
            g_MenuMagicScrollBar[4] = g_MenuMagicWindowRects[2].y + 9;
            g_MenuMagicScrollBar[5] = 17;
            g_MenuMagicScrollBar[6] = g_MenuMagicWindowRects[2].height - 15;
            Menu_DrawScrollBar2x(g_MenuMagicScrollBar, 0.0f);
            Menu_FlushAndSetScissor2x(0, g_MenuMagicWindowRects[2].y + 9, 640, g_MenuMagicWindowRects[2].height - 15);
            if (g_MenuMagicSummonMoving != 0)
                v.rows = 8;
            else
                v.rows = 7;
            for (v.row = 0; v.row < v.rows; v.row++) {
                for (v.column = 0; v.column < 2; v.column++) {
                    if (v.spells[v.column + v.row * 2 + g_MenuMagicSummonTop * 2].attackId != 255)
                        Menu_DrawEncodedString2x(v.column * 620 / 2 + 47, g_MenuMagicWindowRects[2].y + 17 + v.row * 36 + g_MenuMagicSummonSlide * 9,
                                          (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_SUMMON, v.spells[v.column + v.row * 2 + g_MenuMagicSummonTop * 2].attackId, 8), 7, 0.101f);
                }
            }
            Menu_FlushAndResetScissor2x();
            Menu_DrawWindowFrame2x(&g_MenuMagicWindowRects[4], 0.1f);
            break;
        case MENU_MAGIC_STATE_ENEMY_SKILL_LIST:
            Menu_DrawMoreTextArrow2x(g_MenuMagicWindowRects[2].x + g_MenuMagicSkillCol * 309, g_MenuMagicWindowRects[2].y + 24 + g_MenuMagicSkillRow * 36, 0.0f);
            v.spells = g_MenuPartyStats[g_MenuMagicPartyIndex].enemySkills;
            v.selected = g_MenuMagicSkillCol + g_MenuMagicSkillRow * 2 + g_MenuMagicSkillTop * 2;
            if (v.spells[v.selected].attackId != 255)
                Menu_DrawEncodedString2x(27, g_MenuMagicWindowRects[1].y + 13, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ENEMY_SKILL, v.selected, 0), 7, 0.2f);
            Menu_DrawEncodedString2x(g_MenuMagicWindowRects[0].x + 237, g_MenuMagicWindowRects[0].y + 26, g_MenuMagicLabelTable + MENU_MAGIC_LABEL_ADDED_ABILITY * 20, 5, 0.101f);
            if (v.spells[v.selected].attackId != 255)
                Menu_DrawEncodedString2x(g_MenuMagicWindowRects[0].x + 257, g_MenuMagicWindowRects[0].y + 56, g_MenuMagicMessageTable, 7, 0.101f);
            Menu_DrawAtlasQuad2x(g_MenuMagicWindowRects[4].x + 27, g_MenuMagicWindowRects[4].y + 13, 0, 16, 40, 16, 1, 0, 0, 0.01f);
            if (v.spells[v.selected].attackId != 255)
                Menu_DrawDecimalNumberZeroPad2x(g_MenuMagicWindowRects[4].x + 40, g_MenuMagicWindowRects[4].y + 77, v.spells[v.selected].mpCost, 3, 7, 0.101f);
            g_MenuMagicScrollBar[0] = 7;
            g_MenuMagicScrollBar[1] = 12;
            g_MenuMagicScrollBar[2] = g_MenuMagicSkillTop;
            g_MenuMagicScrollBar[3] = 618;
            g_MenuMagicScrollBar[4] = g_MenuMagicWindowRects[2].y + 9;
            g_MenuMagicScrollBar[5] = 17;
            g_MenuMagicScrollBar[6] = g_MenuMagicWindowRects[2].height - 15;
            Menu_DrawScrollBar2x(g_MenuMagicScrollBar, 0.0f);
            Menu_FlushAndSetScissor2x(0, g_MenuMagicWindowRects[2].y + 9, 640, g_MenuMagicWindowRects[2].height - 15);
            if (g_MenuMagicSkillMoving != 0)
                v.rows = 8;
            else
                v.rows = 7;
            for (v.row = 0; v.row < v.rows; v.row++) {
                for (v.column = 0; v.column < 2; v.column++) {
                    if (v.spells[v.column + v.row * 2 + g_MenuMagicSkillTop * 2].attackId != 255)
                        Menu_DrawEncodedString2x(v.column * 618 / 2 + 47, g_MenuMagicWindowRects[2].y + 17 + v.row * 36 + g_MenuMagicSkillSlide * 9,
                                          (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ENEMY_SKILL, v.spells[v.column + v.row * 2 + g_MenuMagicSkillTop * 2].attackId, 8), 7, 0.101f);
                }
            }
            Menu_FlushAndResetScissor2x();
            Menu_DrawWindowFrame2x(&g_MenuMagicWindowRects[4], 0.1f);
        }
        Menu_DrawMagicScreenCharacterPanel2x();
        Menu_DrawWindowFrame2x(g_MenuMagicWindowRects, 0.3f);
        Menu_DrawWindowFrame2x(&g_MenuMagicWindowRects[1], 0.305f);
        Menu_DrawWindowFrame2x(&g_MenuMagicWindowRects[2], 0.4f);
    } else {
        if (g_MenuMagicState != MENU_MAGIC_STATE_COMMAND_LIST) {
            Menu_DrawEncodedString1x(g_MenuMagicWindowRects[5].x + 16, g_MenuMagicWindowRects[5].y + 7,
                                 g_MenuMagicLabelTable + g_MenuMagicListRow * 20, 7, 0.0f);
            Menu_DrawWindowFrame1x(&g_MenuMagicWindowRects[5], 0.01f);
        } else {
            Menu_DrawCommandWindow1x(g_MenuModuleId);
        }
        switch (g_MenuMagicState) {
        case MENU_MAGIC_STATE_COMMAND_LIST:
            Menu_DrawMoreTextArrow1x(g_MenuMagicWindowRects[3].x - 13, g_MenuMagicWindowRects[3].y + 10 + g_MenuMagicListRow * 17, 0.1f);
            Menu_DrawEncodedString1x(g_MenuMagicWindowRects[3].x + 16, g_MenuMagicWindowRects[3].y + 7, g_MenuMagicLabelTable, 7, 0.101f);
            if (gSavemap.magicMenuFlags & MAGIC_MENU_FLAG_SUMMON)
                Menu_DrawEncodedString1x(g_MenuMagicWindowRects[3].x + 16, g_MenuMagicWindowRects[3].y + 24, g_MenuMagicLabelTable + MENU_MAGIC_ROW_SUMMON * 20, 7, 0.101f);
            if (gSavemap.magicMenuFlags & MAGIC_MENU_FLAG_ENEMY_SKILL)
                Menu_DrawEncodedString1x(g_MenuMagicWindowRects[3].x + 16, g_MenuMagicWindowRects[3].y + 41, g_MenuMagicLabelTable + MENU_MAGIC_ROW_ENEMY_SKILL * 20, 7, 0.101f);
            Menu_DrawWindowFrame1x(&g_MenuMagicWindowRects[3], 0.111f);
            break;
        case MENU_MAGIC_STATE_SELECT_TARGET:
        case MENU_MAGIC_STATE_SPELL_LIST:
            v.spells = g_MenuPartyStats[g_MenuMagicPartyIndex].spells;
            v.selected = g_MenuMagicSpellCol + g_MenuMagicSpellRow * 3 + g_MenuMagicSpellTop * 3;
            if (g_MenuMagicState == MENU_MAGIC_STATE_SELECT_TARGET) {
                v.panelX = 84;
                v.panelY = 54;
                if (g_MenuMagicTargetAll != 0)
                    Menu_DrawMoreTextArrow1x(v.panelX, v.panelY + 30 + (int)frameCounter % 3 * 60, 0.0f);
                else
                    Menu_DrawMoreTextArrow1x(v.panelX, v.panelY + 30 + g_MenuMagicTargetRow * 60, 0.0f);
                for (v.i = 0; v.i < 3; v.i++) {
                    if (gSavemap.party[v.i] != PARTY_CHAR_NONE) {
                        Menu_DrawCharStatBlock1x(v.panelX + 64, v.panelY + 6 + v.i * 60, v.i, 0.010011f);
                        Menu_DrawCharPortrait1x(v.panelX + 18, v.panelY + 6 + v.i * 60, v.i, 0.010011f);
                    }
                }
                v.textWidth = Menu_MeasureEncodedString1x((unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_MAGIC, v.spells[v.selected].attackId, 8));
                Menu_DrawEncodedString1x(v.panelX + 114 - v.textWidth / 2, v.panelY - 15,
                                     (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_MAGIC, v.spells[v.selected].attackId, 8), 7, 0.0f);
                Menu_SetRectShorts(v.rect, v.panelX + 77, v.panelY - 22, 77, 26);
                Menu_DrawWindowFrame1x((EnglishWindowFrame *)v.rect, 0.0f);
                Menu_SetRectShorts(v.rect, v.panelX, v.panelY, 146, 183);
                Menu_DrawWindowFrame1x((EnglishWindowFrame *)v.rect, 0.1f);
                if (frameCounter & 2)
                    Menu_DrawMoreTextArrow1x(g_MenuMagicWindowRects[2].x + 10 + g_MenuMagicSpellCol * 11 * 8, g_MenuMagicWindowRects[2].y + 12 + g_MenuMagicSpellRow * 18, 0.111f);
            } else {
                Menu_DrawMoreTextArrow1x(g_MenuMagicWindowRects[2].x + 10 + g_MenuMagicSpellCol * 11 * 8, g_MenuMagicWindowRects[2].y + 12 + g_MenuMagicSpellRow * 18, 0.0f);
            }
            if (v.spells[v.selected].attackId != 255)
                Menu_DrawEncodedString1x(14, g_MenuMagicWindowRects[1].y + 7, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_MAGIC, v.spells[v.selected].attackId, 0), 7, 0.2f);
            Menu_DrawEncodedString1x(g_MenuMagicWindowRects[0].x + 114, g_MenuMagicWindowRects[0].y + 11, g_MenuMagicLabelTable + MENU_MAGIC_LABEL_ADDED_ABILITY * 20, 5, 0.101f);
            v.count = 0;
            if (v.spells[v.selected].attackId != 255) {
                for (v.i = 0; v.i < 8; v.i++) {
                    switch (v.i) {
                    case MENU_MAGIC_ADDED_ALL:
                        if (v.spells[v.selected].allCount != 0)
                            v.colour = 7;
                        else
                            v.colour = 0;
                        break;
                    case MENU_MAGIC_ADDED_QUADRA_MAGIC:
                        if (v.spells[v.selected].quadraCount != 0)
                            v.colour = 7;
                        else
                            v.colour = 0;
                        break;
                    default:
                        if (v.i == MENU_MAGIC_ADDED_TURBO_MP) {
                            if (((v.spells[v.selected].support >> 5) & 7) != 0)
                                v.colour = 7;
                            else
                                v.colour = 0;
                        } else {
                            if (v.spells[v.selected].support & (1 << (v.i - MENU_MAGIC_ADDED_MP_ABSORB)))
                                v.colour = 7;
                            else
                                v.colour = 0;
                        }
                    }
                    if (v.colour != 0) {
                        Menu_DrawEncodedString1x(g_MenuMagicWindowRects[0].x + 124 + v.count / 4 * 60, g_MenuMagicWindowRects[0].y + 23 + (v.count & 3) * 13,
                                             g_MenuMagicLabelTable + (v.i + MENU_MAGIC_LABEL_FIRST_ADDED) * 20, 7, 0.2f);
                        if (v.i == MENU_MAGIC_ADDED_ALL) {
                            Menu_DrawDecimalNumber1x(g_MenuMagicWindowRects[0].x + 154 + v.count / 4 * 60, g_MenuMagicWindowRects[0].y + 24 + (v.count & 3) * 13, v.spells[v.selected].allCount, 3, 7, 0.2f);
                            Menu_DrawAtlasQuad1x(g_MenuMagicWindowRects[0].x + 173 + v.count / 4 * 60, g_MenuMagicWindowRects[0].y + 24 + (v.count & 3) * 13, 120, 8, 8, 8, 7, 0, 0, 0.2f);
                        } else if (v.i == MENU_MAGIC_ADDED_QUADRA_MAGIC) {
                            Menu_DrawDecimalNumber1x(g_MenuMagicWindowRects[0].x + 154 + v.count / 4 * 60, g_MenuMagicWindowRects[0].y + 24 + (v.count & 3) * 13, v.spells[v.selected].quadraCount, 3, 7, 0.2f);
                            Menu_DrawAtlasQuad1x(g_MenuMagicWindowRects[0].x + 173 + v.count / 4 * 60, g_MenuMagicWindowRects[0].y + 24 + (v.count & 3) * 13, 120, 8, 8, 8, 7, 0, 0, 0.2f);
                        } else if (v.i == MENU_MAGIC_ADDED_TURBO_MP) {
                            Menu_DrawDecimalNumber1x(g_MenuMagicWindowRects[0].x + 158 + v.count / 4 * 60, g_MenuMagicWindowRects[0].y + 24 + (v.count & 3) * 13, (v.spells[v.selected].support >> 5) & 7, 3, 7, 0.2f);
                        }
                        v.count++;
                    }
                }
                if (v.count == 0)
                    Menu_DrawEncodedString1x(g_MenuMagicWindowRects[0].x + 129, g_MenuMagicWindowRects[0].y + 28, g_MenuMagicMessageTable, 7, 0.101f);
            }
            Menu_DrawAtlasQuad1x(g_MenuMagicWindowRects[4].x + 14, g_MenuMagicWindowRects[4].y + 7, 0, 16, 40, 16, 1, 0, 0, 0.01f);
            if (v.spells[v.selected].attackId != 255)
                Menu_DrawDecimalNumberZeroPad1x(g_MenuMagicWindowRects[4].x + 20, g_MenuMagicWindowRects[4].y + 39, v.spells[v.selected].mpCost, 3, 7, 0.0f);
            g_MenuMagicScrollBar[0] = 7;
            g_MenuMagicScrollBar[1] = 18;
            g_MenuMagicScrollBar[2] = g_MenuMagicSpellTop;
            g_MenuMagicScrollBar[3] = 309;
            g_MenuMagicScrollBar[4] = g_MenuMagicWindowRects[2].y + 5;
            g_MenuMagicScrollBar[5] = 9;
            g_MenuMagicScrollBar[6] = g_MenuMagicWindowRects[2].height - 8;
            Menu_DrawScrollBar1x(g_MenuMagicScrollBar, 0.0f);
            Menu_FlushAndSetScissor1x(0, g_MenuMagicWindowRects[2].y + 5, 320, g_MenuMagicWindowRects[2].height - 8);
            if (g_MenuMagicSpellMoving != 0)
                v.rows = 8;
            else
                v.rows = 7;
            for (v.row = 0; v.row < v.rows; v.row++) {
                for (v.column = 0; v.column < 3; v.column++) {
                    switch (v.spells[v.column + g_MenuMagicSpellTop * 3 + v.row * 3].attackId) {
                    case MENU_SPELL_CURE:
                    case MENU_SPELL_CURE2:
                    case MENU_SPELL_CURE3:
                    case MENU_SPELL_LIFE:
                    case MENU_SPELL_LIFE2:
                    case MENU_SPELL_FULL_CURE:
                        v.colour = 7;
                        break;
                    default:
                        v.colour = 0;
                    }
                    if (v.spells[v.column + g_MenuMagicSpellTop * 3 + v.row * 3].attackId != 255)
                        Menu_DrawEncodedString1x(v.column * 11 * 8 + 39, g_MenuMagicWindowRects[2].y + 9 + v.row * 18 + g_MenuMagicSpellSlide * 5,
                                             (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_MAGIC, v.spells[v.column + v.row * 3 + g_MenuMagicSpellTop * 3].attackId, 8), (char)v.colour, 0.2f);
                }
            }
            Menu_FlushAndResetScissor1x();
            Menu_DrawWindowFrame1x(&g_MenuMagicWindowRects[4], 0.1f);
            break;
        case MENU_MAGIC_STATE_SUMMON_LIST:
            Menu_DrawMoreTextArrow1x(g_MenuMagicWindowRects[2].x + g_MenuMagicSummonCol * 154, g_MenuMagicWindowRects[2].y + 12 + g_MenuMagicSummonRow * 18, 0.0f);
            v.spells = g_MenuPartyStats[g_MenuMagicPartyIndex].summons;
            v.selected = g_MenuMagicSummonCol + g_MenuMagicSummonRow * 2 + g_MenuMagicSummonTop * 2;
            if (v.spells[v.selected].attackId != 255)
                Menu_DrawEncodedString1x(14, g_MenuMagicWindowRects[1].y + 7, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_SUMMON, v.selected, 0), 7, 0.2f);
            Menu_DrawEncodedString1x(g_MenuMagicWindowRects[0].x + 114, g_MenuMagicWindowRects[0].y + 11, g_MenuMagicLabelTable + MENU_MAGIC_LABEL_ADDED_ABILITY * 20, 5, 0.101f);
            v.count = 0;
            if (v.spells[v.selected].attackId != 255) {
                for (v.i = 0; v.i < 8; v.i++) {
                    switch (v.i) {
                    case MENU_MAGIC_ADDED_ALL:
                        v.colour = 0;
                        break;
                    case MENU_MAGIC_ADDED_QUADRA_MAGIC:
                        if (v.spells[v.selected].quadraCount != 0)
                            v.colour = 7;
                        else
                            v.colour = 0;
                        break;
                    default:
                        if (v.i == MENU_MAGIC_ADDED_TURBO_MP) {
                            if (((v.spells[v.selected].support >> 5) & 7) != 0)
                                v.colour = 7;
                            else
                                v.colour = 0;
                        } else {
                            if (v.spells[v.selected].support & (1 << (v.i - MENU_MAGIC_ADDED_MP_ABSORB)))
                                v.colour = 7;
                            else
                                v.colour = 0;
                        }
                    }
                    if (v.colour != 0 && v.i != MENU_MAGIC_ADDED_ALL) {
                        Menu_DrawEncodedString1x(g_MenuMagicWindowRects[0].x + 124 + v.count / 4 * 60, g_MenuMagicWindowRects[0].y + 23 + (v.count & 3) * 13,
                                             g_MenuMagicLabelTable + (v.i + MENU_MAGIC_LABEL_FIRST_ADDED) * 20, 7, 0.2f);
                        if (v.i == MENU_MAGIC_ADDED_ALL) {
                            Menu_DrawDecimalNumber1x(g_MenuMagicWindowRects[0].x + 154 + v.count / 4 * 60, g_MenuMagicWindowRects[0].y + 24 + (v.count & 3) * 13, v.spells[v.selected].allCount, 3, 7, 0.2f);
                            Menu_DrawAtlasQuad1x(g_MenuMagicWindowRects[0].x + 173 + v.count / 4 * 60, g_MenuMagicWindowRects[0].y + 24 + (v.count & 3) * 13, 120, 8, 8, 8, 7, 0, 0, 0.2f);
                        } else if (v.i == MENU_MAGIC_ADDED_QUADRA_MAGIC) {
                            Menu_DrawDecimalNumber1x(g_MenuMagicWindowRects[0].x + 154 + v.count / 4 * 60, g_MenuMagicWindowRects[0].y + 24 + (v.count & 3) * 13, v.spells[v.selected].quadraCount, 3, 7, 0.2f);
                            Menu_DrawAtlasQuad1x(g_MenuMagicWindowRects[0].x + 173 + v.count / 4 * 60, g_MenuMagicWindowRects[0].y + 24 + (v.count & 3) * 13, 120, 8, 8, 8, 7, 0, 0, 0.2f);
                        } else if (v.i == MENU_MAGIC_ADDED_TURBO_MP) {
                            Menu_DrawDecimalNumber1x(g_MenuMagicWindowRects[0].x + 158 + v.count / 4 * 60, g_MenuMagicWindowRects[0].y + 24 + (v.count & 3) * 13, (v.spells[v.selected].support >> 5) & 7, 3, 7, 0.2f);
                        }
                        v.count++;
                    }
                }
                if (v.count == 0)
                    Menu_DrawEncodedString1x(g_MenuMagicWindowRects[0].x + 123, g_MenuMagicWindowRects[0].y + 28, g_MenuMagicMessageTable, 7, 0.101f);
            }
            Menu_DrawAtlasQuad1x(g_MenuMagicWindowRects[4].x + 14, g_MenuMagicWindowRects[4].y + 7, 0, 16, 40, 16, 1, 0, 0, 0.01f);
            if (v.spells[v.selected].attackId != 255)
                Menu_DrawDecimalNumberZeroPad1x(g_MenuMagicWindowRects[4].x + 19, g_MenuMagicWindowRects[4].y + 39, v.spells[v.selected].mpCost, 3, 7, 0.101f);
            g_MenuMagicScrollBar[0] = 7;
            g_MenuMagicScrollBar[1] = 8;
            g_MenuMagicScrollBar[2] = g_MenuMagicSummonTop;
            g_MenuMagicScrollBar[3] = 309;
            g_MenuMagicScrollBar[4] = g_MenuMagicWindowRects[2].y + 5;
            g_MenuMagicScrollBar[5] = 9;
            g_MenuMagicScrollBar[6] = g_MenuMagicWindowRects[2].height - 8;
            Menu_DrawScrollBar1x(g_MenuMagicScrollBar, 0.0f);
            Menu_FlushAndSetScissor1x(0, g_MenuMagicWindowRects[2].y + 5, 320, g_MenuMagicWindowRects[2].height - 8);
            if (g_MenuMagicSummonMoving != 0)
                v.rows = 8;
            else
                v.rows = 7;
            for (v.row = 0; v.row < v.rows; v.row++) {
                for (v.column = 0; v.column < 2; v.column++) {
                    if (v.spells[v.column + v.row * 2 + g_MenuMagicSummonTop * 2].attackId != 255)
                        Menu_DrawEncodedString1x(v.column * 310 / 2 + 24, g_MenuMagicWindowRects[2].y + 9 + v.row * 18 + g_MenuMagicSummonSlide * 5,
                                             (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_SUMMON, v.spells[v.column + v.row * 2 + g_MenuMagicSummonTop * 2].attackId, 8), 7, 0.101f);
                }
            }
            Menu_FlushAndResetScissor1x();
            Menu_DrawWindowFrame1x(&g_MenuMagicWindowRects[4], 0.1f);
            break;
        case MENU_MAGIC_STATE_ENEMY_SKILL_LIST:
            Menu_DrawMoreTextArrow1x(g_MenuMagicWindowRects[2].x + g_MenuMagicSkillCol * 154, g_MenuMagicWindowRects[2].y + 12 + g_MenuMagicSkillRow * 18, 0.0f);
            v.spells = g_MenuPartyStats[g_MenuMagicPartyIndex].enemySkills;
            v.selected = g_MenuMagicSkillCol + g_MenuMagicSkillRow * 2 + g_MenuMagicSkillTop * 2;
            if (v.spells[v.selected].attackId != 255)
                Menu_DrawEncodedString1x(14, g_MenuMagicWindowRects[1].y + 7, (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ENEMY_SKILL, v.selected, 0), 7, 0.2f);
            Menu_DrawEncodedString1x(g_MenuMagicWindowRects[0].x + 119, g_MenuMagicWindowRects[0].y + 13, g_MenuMagicLabelTable + MENU_MAGIC_LABEL_ADDED_ABILITY * 20, 5, 0.101f);
            if (v.spells[v.selected].attackId != 255)
                Menu_DrawEncodedString1x(g_MenuMagicWindowRects[0].x + 129, g_MenuMagicWindowRects[0].y + 28, g_MenuMagicMessageTable, 7, 0.101f);
            Menu_DrawAtlasQuad1x(g_MenuMagicWindowRects[4].x + 14, g_MenuMagicWindowRects[4].y + 7, 0, 16, 40, 16, 1, 0, 0, 0.01f);
            if (v.spells[v.selected].attackId != 255)
                Menu_DrawDecimalNumberZeroPad1x(g_MenuMagicWindowRects[4].x + 20, g_MenuMagicWindowRects[4].y + 39, v.spells[v.selected].mpCost, 3, 7, 0.101f);
            g_MenuMagicScrollBar[0] = 7;
            g_MenuMagicScrollBar[1] = 12;
            g_MenuMagicScrollBar[2] = g_MenuMagicSkillTop;
            g_MenuMagicScrollBar[3] = 309;
            g_MenuMagicScrollBar[4] = g_MenuMagicWindowRects[2].y + 5;
            g_MenuMagicScrollBar[5] = 9;
            g_MenuMagicScrollBar[6] = g_MenuMagicWindowRects[2].height - 8;
            Menu_DrawScrollBar1x(g_MenuMagicScrollBar, 0.0f);
            Menu_FlushAndSetScissor1x(0, g_MenuMagicWindowRects[2].y + 5, 320, g_MenuMagicWindowRects[2].height - 8);
            if (g_MenuMagicSkillMoving != 0)
                v.rows = 8;
            else
                v.rows = 7;
            for (v.row = 0; v.row < v.rows; v.row++) {
                for (v.column = 0; v.column < 2; v.column++) {
                    if (v.spells[v.column + v.row * 2 + g_MenuMagicSkillTop * 2].attackId != 255)
                        Menu_DrawEncodedString1x(v.column * 309 / 2 + 24, g_MenuMagicWindowRects[2].y + 9 + v.row * 18 + g_MenuMagicSkillSlide * 5,
                                             (unsigned char *)Kernel_LookupUiText(KERNEL_TEXT_ENEMY_SKILL, v.spells[v.column + v.row * 2 + g_MenuMagicSkillTop * 2].attackId, 8), 7, 0.101f);
                }
            }
            Menu_FlushAndResetScissor1x();
            Menu_DrawWindowFrame1x(&g_MenuMagicWindowRects[4], 0.1f);
        }
        Menu_DrawMagicScreenCharacterPanel1x();
        Menu_DrawWindowFrame1x(g_MenuMagicWindowRects, 0.3f);
        Menu_DrawWindowFrame1x(&g_MenuMagicWindowRects[1], 0.305f);
        Menu_DrawWindowFrame1x(&g_MenuMagicWindowRects[2], 0.4f);
    }
    MenuCursor_Update((MenuCursor *)((unsigned char *)&g_MenuMagicListCol + g_MenuMagicState * 56));
    if (Menu_GetPhase() == 0) {
        switch (g_MenuMagicState) {
        case MENU_MAGIC_STATE_COMMAND_LIST:
            if (Input_GetPressed(INPUT_CIRCLE)) {
                switch (g_MenuMagicListRow) {
                case MENU_MAGIC_ROW_SUMMON:
                    if (gSavemap.magicMenuFlags & MAGIC_MENU_FLAG_SUMMON) {
                        g_MenuMagicState = g_MenuMagicListRow + MENU_MAGIC_STATE_SPELL_LIST;
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    } else
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                    break;
                case MENU_MAGIC_ROW_ENEMY_SKILL:
                    if (gSavemap.magicMenuFlags & MAGIC_MENU_FLAG_ENEMY_SKILL) {
                        g_MenuMagicState = g_MenuMagicListRow + MENU_MAGIC_STATE_SPELL_LIST;
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                    } else
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                    break;
                default:
                    g_MenuMagicState = g_MenuMagicListRow + MENU_MAGIC_STATE_SPELL_LIST;
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                }
            } else if (BtlMenu_GetInputRepeat(INPUT_CROSS)) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                Menu_SetPhase(5, 0);
                Menu_SetScreen(0);
            } else if (BtlMenu_GetInputRepeat(INPUT_L1)) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                do {
                    g_MenuMagicPartyIndex--;
                    if (g_MenuMagicPartyIndex < 0)
                        g_MenuMagicPartyIndex = 2;
                } while (gSavemap.party[g_MenuMagicPartyIndex] == PARTY_CHAR_NONE);
            } else if (BtlMenu_GetInputRepeat(INPUT_R1)) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                do {
                    g_MenuMagicPartyIndex++;
                    if (g_MenuMagicPartyIndex > 2)
                        g_MenuMagicPartyIndex = 0;
                } while (gSavemap.party[g_MenuMagicPartyIndex] == PARTY_CHAR_NONE);
            }
            break;
        case MENU_MAGIC_STATE_SELECT_TARGET:
            v.selected = g_MenuMagicSpellCol + g_MenuMagicSpellRow * 3 + g_MenuMagicSpellTop * 3;
            v.spells = g_MenuPartyStats[g_MenuMagicPartyIndex].spells;
            if (Input_GetPressed(INPUT_CIRCLE)) {
                if (g_MenuPartyStats[g_MenuMagicPartyIndex].mp >= v.spells[v.selected].mpCost) {
                    if (g_MenuMagicTargetAll == 0) {
                        if (gSavemap.party[g_MenuMagicTargetRow] == PARTY_CHAR_NONE) {
                            SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                        } else {
                            switch (v.spells[v.selected].attackId) {
                            case MENU_SPELL_CURE:
                                if (Menu_MagicScreenIsPartyMemberHpFull(g_MenuMagicTargetRow) == 0 && g_MenuPartyStats[g_MenuMagicTargetRow].hp != 0) {
                                    if (gSavemap.party[g_MenuMagicPartyIndex] != PARTY_CHAR_SEPHIROTH)
                                        Savemap_DamageMp(g_MenuMagicPartyIndex, v.spells[v.selected].mpCost);
                                    v.panelX = ((g_MenuPartyStats[g_MenuMagicPartyIndex].magicAttack + gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[g_MenuMagicPartyIndex]]].level) * 6);
                                    Savemap_HealHp(g_MenuMagicTargetRow, v.panelX + 110);
                                    SfxPreloadAndPlayOnChannel5(SFX_MENU_ITEM_EFFECT);
                                } else
                                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                                break;
                            case MENU_SPELL_CURE2:
                                if (Menu_MagicScreenIsPartyMemberHpFull(g_MenuMagicTargetRow) == 0 && g_MenuPartyStats[g_MenuMagicTargetRow].hp != 0) {
                                    if (gSavemap.party[g_MenuMagicPartyIndex] != PARTY_CHAR_SEPHIROTH)
                                        Savemap_DamageMp(g_MenuMagicPartyIndex, v.spells[v.selected].mpCost);
                                    v.panelX = ((g_MenuPartyStats[g_MenuMagicPartyIndex].magicAttack + gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[g_MenuMagicPartyIndex]]].level) * 6);
                                    Savemap_HealHp(g_MenuMagicTargetRow, v.panelX + 660);
                                    SfxPreloadAndPlayOnChannel5(SFX_MENU_ITEM_EFFECT);
                                } else
                                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                                break;
                            case MENU_SPELL_CURE3:
                                if (Menu_MagicScreenIsPartyMemberHpFull(g_MenuMagicTargetRow) == 0 && g_MenuPartyStats[g_MenuMagicTargetRow].hp != 0) {
                                    if (gSavemap.party[g_MenuMagicPartyIndex] != PARTY_CHAR_SEPHIROTH)
                                        Savemap_DamageMp(g_MenuMagicPartyIndex, v.spells[v.selected].mpCost);
                                    v.panelX = ((g_MenuPartyStats[g_MenuMagicPartyIndex].magicAttack + gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[g_MenuMagicPartyIndex]]].level) * 6);
                                    Savemap_HealHp(g_MenuMagicTargetRow, v.panelX + 1760);
                                    SfxPreloadAndPlayOnChannel5(SFX_MENU_ITEM_EFFECT);
                                } else
                                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                                break;
                            case MENU_SPELL_FULL_CURE:
                                if (Menu_MagicScreenIsPartyMemberHpFull(g_MenuMagicTargetRow) == 0 && g_MenuPartyStats[g_MenuMagicTargetRow].hp != 0) {
                                    if (gSavemap.party[g_MenuMagicPartyIndex] != PARTY_CHAR_SEPHIROTH)
                                        Savemap_DamageMp(g_MenuMagicPartyIndex, v.spells[v.selected].mpCost);
                                    Savemap_HealHp(g_MenuMagicTargetRow, 10000);
                                    SfxPreloadAndPlayOnChannel5(SFX_MENU_ITEM_EFFECT);
                                } else
                                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                                break;
                            case MENU_SPELL_LIFE:
                                if (g_MenuPartyStats[g_MenuMagicTargetRow].hp == 0) {
                                    if (gSavemap.party[g_MenuMagicPartyIndex] != PARTY_CHAR_SEPHIROTH)
                                        Savemap_DamageMp(g_MenuMagicPartyIndex, v.spells[v.selected].mpCost);
                                    Savemap_HealHp(g_MenuMagicTargetRow, g_MenuPartyStats[g_MenuMagicTargetRow].maxHp / 4);
                                    SfxPreloadAndPlayOnChannel5(SFX_MENU_ITEM_EFFECT);
                                } else
                                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                                break;
                            case MENU_SPELL_LIFE2:
                                if (g_MenuPartyStats[g_MenuMagicTargetRow].hp == 0) {
                                    if (gSavemap.party[g_MenuMagicPartyIndex] != PARTY_CHAR_SEPHIROTH)
                                        Savemap_DamageMp(g_MenuMagicPartyIndex, v.spells[v.selected].mpCost);
                                    Savemap_HealHp(g_MenuMagicTargetRow, 10000);
                                    SfxPreloadAndPlayOnChannel5(SFX_MENU_ITEM_EFFECT);
                                } else
                                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                            }
                        }
                    } else {
                        switch (v.spells[v.selected].attackId) {
                        case MENU_SPELL_CURE:
                            for (v.valid = v.i = 0; v.i < 3; v.i++) {
                                if (gSavemap.party[v.i] != PARTY_CHAR_NONE && Menu_MagicScreenIsPartyMemberHpFull(v.i) == 0)
                                    v.valid = 1;
                            }
                            if (v.valid != 0) {
                                if (gSavemap.party[g_MenuMagicPartyIndex] != PARTY_CHAR_SEPHIROTH)
                                    Savemap_DamageMp(g_MenuMagicPartyIndex, v.spells[v.selected].mpCost);
                                v.panelX = ((g_MenuPartyStats[g_MenuMagicPartyIndex].magicAttack + gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[g_MenuMagicPartyIndex]]].level) * 6);
                                for (v.i = 0; v.i < 3; v.i++) {
                                    if (gSavemap.party[v.i] != PARTY_CHAR_NONE && g_MenuPartyStats[v.i].hp != 0)
                                        Savemap_HealHp(v.i, (v.panelX + 110) * 2 / 3);
                                }
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_ITEM_EFFECT);
                            } else
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                            break;
                        case MENU_SPELL_CURE2:
                            for (v.valid = v.i = 0; v.i < 3; v.i++) {
                                if (gSavemap.party[v.i] != PARTY_CHAR_NONE && Menu_MagicScreenIsPartyMemberHpFull(v.i) == 0)
                                    v.valid = 1;
                            }
                            if (v.valid != 0) {
                                if (gSavemap.party[g_MenuMagicPartyIndex] != PARTY_CHAR_SEPHIROTH)
                                    Savemap_DamageMp(g_MenuMagicPartyIndex, v.spells[v.selected].mpCost);
                                v.panelX = ((g_MenuPartyStats[g_MenuMagicPartyIndex].magicAttack + gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[g_MenuMagicPartyIndex]]].level) * 6);
                                for (v.i = 0; v.i < 3; v.i++) {
                                    if (gSavemap.party[v.i] != PARTY_CHAR_NONE && g_MenuPartyStats[v.i].hp != 0)
                                        Savemap_HealHp(v.i, (v.panelX + 660) * 2 / 3);
                                }
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_ITEM_EFFECT);
                            } else
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                            break;
                        case MENU_SPELL_CURE3:
                            for (v.valid = v.i = 0; v.i < 3; v.i++) {
                                if (gSavemap.party[v.i] != PARTY_CHAR_NONE && Menu_MagicScreenIsPartyMemberHpFull(v.i) == 0)
                                    v.valid = 1;
                            }
                            if (v.valid != 0) {
                                if (gSavemap.party[g_MenuMagicPartyIndex] != PARTY_CHAR_SEPHIROTH)
                                    Savemap_DamageMp(g_MenuMagicPartyIndex, v.spells[v.selected].mpCost);
                                v.panelX = ((g_MenuPartyStats[g_MenuMagicPartyIndex].magicAttack + gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[g_MenuMagicPartyIndex]]].level) * 6);
                                for (v.i = 0; v.i < 3; v.i++) {
                                    if (gSavemap.party[v.i] != PARTY_CHAR_NONE && g_MenuPartyStats[v.i].hp != 0)
                                        Savemap_HealHp(v.i, (v.panelX + 1760) * 2 / 3);
                                }
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_ITEM_EFFECT);
                            } else
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                            break;
                        case MENU_SPELL_LIFE:
                            for (v.valid = v.i = 0; v.i < 3; v.i++) {
                                if (gSavemap.party[v.i] != PARTY_CHAR_NONE && g_MenuPartyStats[v.i].hp == 0)
                                    v.valid = 1;
                            }
                            if (v.valid != 0) {
                                if (gSavemap.party[g_MenuMagicPartyIndex] != PARTY_CHAR_SEPHIROTH)
                                    Savemap_DamageMp(g_MenuMagicPartyIndex, v.spells[v.selected].mpCost);

                                for (v.i = 0; v.i < 3; v.i++) {
                                    if (gSavemap.party[v.i] != PARTY_CHAR_NONE && g_MenuPartyStats[v.i].hp == 0)
                                        Savemap_HealHp(v.i, g_MenuPartyStats[g_MenuMagicTargetRow].maxHp / 4 / 2);
                                }
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_ITEM_EFFECT);
                            } else
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                            break;
                        case MENU_SPELL_LIFE2:
                            for (v.valid = v.i = 0; v.i < 3; v.i++) {
                                if (gSavemap.party[v.i] != PARTY_CHAR_NONE && g_MenuPartyStats[v.i].hp == 0)
                                    v.valid = 1;
                            }
                            if (v.valid != 0) {
                                if (gSavemap.party[g_MenuMagicPartyIndex] != PARTY_CHAR_SEPHIROTH)
                                    Savemap_DamageMp(g_MenuMagicPartyIndex, v.spells[v.selected].mpCost);

                                for (v.i = 0; v.i < 3; v.i++) {
                                    if (gSavemap.party[v.i] != PARTY_CHAR_NONE && g_MenuPartyStats[v.i].hp == 0)
                                        Savemap_HealHp(v.i, 5000);
                                }
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_ITEM_EFFECT);
                            } else
                                SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                        }
                    }
                } else {
                    Menu_OpenPopupMessage((void *)(g_MenuMagicMessageTable + MENU_MAGIC_MESSAGE_NOT_ENOUGH_MP * 20), 7);
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                }
            } else if (Input_GetPressed(INPUT_CROSS)) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
                g_MenuMagicState = MENU_MAGIC_STATE_SPELL_LIST;
            } else if (v.spells[v.selected].allCount != 0 && (g_InputPressedMask & (INPUT_L1 | INPUT_R1 | INPUT_RIGHT | INPUT_LEFT)) != 0) {
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                g_MenuMagicTargetAll ^= 1;
            }
            break;
        case MENU_MAGIC_STATE_SPELL_LIST:
            if (Input_GetPressed(INPUT_CIRCLE)) {
                v.selected = g_MenuMagicSpellCol + g_MenuMagicSpellRow * 3 + g_MenuMagicSpellTop * 3;
                v.spells = g_MenuPartyStats[g_MenuMagicPartyIndex].spells;
                if (v.spells[v.selected].attackId != 255 && g_MenuPartyStats[g_MenuMagicPartyIndex].hp != 0) {
                    switch (v.spells[v.selected].attackId) {
                    case MENU_SPELL_CURE:
                    case MENU_SPELL_CURE2:
                    case MENU_SPELL_CURE3:
                    case MENU_SPELL_LIFE:
                    case MENU_SPELL_LIFE2:
                    case MENU_SPELL_FULL_CURE:
                        g_MenuMagicTargetAll = 0;
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_CURSOR);
                        g_MenuMagicState = MENU_MAGIC_STATE_SELECT_TARGET;
                        break;
                    default:
                        SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
                    }
                } else
                    SfxPreloadAndPlayOnChannel5(SFX_MENU_INVALID);
            } else if (BtlMenu_GetInputRepeat(INPUT_CROSS)) {
                g_MenuMagicState = MENU_MAGIC_STATE_COMMAND_LIST;
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
            }
            break;
        case MENU_MAGIC_STATE_SUMMON_LIST:
            if (BtlMenu_GetInputRepeat(INPUT_CROSS)) {
                g_MenuMagicState = MENU_MAGIC_STATE_COMMAND_LIST;
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
            }
            break;
        case MENU_MAGIC_STATE_ENEMY_SKILL_LIST:
            if (BtlMenu_GetInputRepeat(INPUT_CROSS)) {
                g_MenuMagicState = MENU_MAGIC_STATE_COMMAND_LIST;
                SfxPreloadAndPlayOnChannel5(SFX_MENU_CANCEL);
            }
        }
    }
    /* These two stores are dead but kept: the original ends with them. */
    v.selected = g_MenuMagicSpellCol + g_MenuMagicSpellRow * 3 + g_MenuMagicSpellTop * 3;
    v.spells = g_MenuPartyStats[g_MenuMagicPartyIndex].spells;
}

/*
 * Checks if selected party member HP is currently at maximum.
 */
// FUNCTION: FF7 0x00714598
int __cdecl Menu_MagicScreenIsPartyMemberHpFull(int slot)
{
    if (g_MenuPartyStats[slot].maxHp == g_MenuPartyStats[slot].hp) {
        return 1;
    } else {
        return 0;
    }
}

/*
 * Draws character stats and magic summary panel in 2x mode.
 */
// FUNCTION: FF7 0x007145ca
void __cdecl Menu_DrawMagicScreenCharacterPanel2x(void)
{
    Menu_DrawCharStatBlock2x(0x67, 0x27, g_MenuMagicPartyIndex, 0.2f);
    Menu_DrawCharPortrait2x(0x10, 0x27, g_MenuMagicPartyIndex, 0.1f);
}

/*
 * Draws character stats and magic summary panel in 1x mode.
 */
// FUNCTION: FF7 0x007145fe
void __cdecl Menu_DrawMagicScreenCharacterPanel1x(void)
{
    Menu_DrawCharStatBlock1x(0x34, 0x14, g_MenuMagicPartyIndex, 0.2f);
    Menu_DrawCharPortrait1x(8, 0x14, g_MenuMagicPartyIndex, 0.1f);
}
} /* extern "C" */
