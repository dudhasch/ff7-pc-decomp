// src/ff7/src/menu/english/menu_kernel_text_pool.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The kernel text system: resetting and allocating the text pool, looking up battle
//   messages and text banks, expanding back-references and escape codes, and appending names and
//   encoded strings into output buffers.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/menu.h"
#include <stdio.h>
#include "lib/enums/file.h"
#include "ff7/api/battle.h"
#include "lib/api/file.h"
#include "ff7/api/menu.h"
#include "lib/api/token.h"
#include "ff7/enums/main.h"
#include <string.h>
#include "ff7/enums/battle.h"
#include "ff7/savemap.h"
#include "ff7/types/battle.h"
extern "C" {

// The PSX file address base 0x801A0000 that the battle camera scripts (battle_camera_channel_scripts.cpp) subtract from their
// pointers.
// GLOBAL: FF7 0x007b7470
extern const int g_BattleCameraFileAddressBase = 0x801A0000;

#pragma intrinsic(memset, strcpy, strcat)

// clang-format off
// The first attack-table row of use-condition categories 0, 1 and 2 (Kernel_GetUseCondition).
// GLOBAL: FF7 0x007b7478
extern const int g_KernelUseConditionOffsets[3] = {0, 56, 72};
// GLOBAL: FF7 0x007b7488
extern const unsigned short g_KernelUiTextGroup4Thresholds[6] = {0, 128, 256, 288, 384, 65535};
// The UI text group tables of Kernel_LookupUiText: the group each range of group 4 maps to (five,
// read by the range index), the id offset of groups 0..3, and the kernel text bank of each group
// (0xFF: built by the switch).
// GLOBAL: FF7 0x007b7498
extern const unsigned char g_KernelUiTextGroup4TargetGroups[8] = {4, 10, 11, 12, 13};
// GLOBAL: FF7 0x007b74a0
extern const unsigned char g_KernelUiTextGroupIdOffsets[8] = {0, 0x38, 0x48, 0x80};
// GLOBAL: FF7 0x007b74a8
extern const unsigned char g_KernelUiTextGroupBankMap[16] = {1, 1, 1, 1, 2, 0, 0xff, 0xff, 0xff, 0xff, 3, 4, 5, 6, 7};
// The FF7 empty string (the 0xFF terminator) Kernel_LookupUiText returns by default.
// GLOBAL: FF7 0x007c0ae8
unsigned char g_KernelEmptyString[2] = {0xff, 0x00};
// GLOBAL: FF7 0x009a13c8
char g_KernelTextPool[0x6c00] = {0};
// GLOBAL: FF7 0x009a7fc8
unsigned short g_KernelTextBankOffsets[20] = {0};
// g_KernelExpandedTextBuffer is indexed by the code as an array. The span runs to the next named
// definition of the TU; the TU's pieces inside 0x9a7ff0..0x9a80f0 are aliases.
// GLOBAL: FF7 0x009a7ff0
unsigned char g_KernelExpandedTextBuffer[0x100] = {0};
// GLOBAL: FF7 0x009a80f0
char g_KernelFormattedTextBuffer = 0;
unsigned char g_bssPad_009a80f4[4] = {0};
unsigned char g_bssPad_009a80f8[40] = {0};
// GLOBAL: FF7 0x009a8120
int g_KernelTextPoolOffset = 0;
// GLOBAL: FF7 0x009a8124
int g_KernelTextBankCount = 0;
// clang-format on

char *__cdecl Kernel_FormatBattleText(char *out, unsigned char *text);

/*
 * Resets the kernel text pool allocation counters to zero.
 */
// FUNCTION: FF7 0x00419360
void __cdecl Kernel_ResetTextPool(void)
{
    g_KernelTextBankCount = 0;
    g_KernelTextPoolOffset = 0;
}

// The bump allocator behind Kernel_GetTextBankEntry: hands out the current offset, records it in
// the bank table (as an unsigned short), then advances.
// FUNCTION: FF7 0x00419379
char *__cdecl Kernel_AllocTextBank(int size)
{
    char *rec;

    rec = &g_KernelTextPool[g_KernelTextPoolOffset];
    g_KernelTextBankOffsets[g_KernelTextBankCount] = (unsigned short)g_KernelTextPoolOffset;
    g_KernelTextBankCount = g_KernelTextBankCount + 1;
    g_KernelTextPoolOffset = g_KernelTextPoolOffset + size;
    return rec;
}

/*
 * Reads the battle action use condition byte for a category and entry index.
 */
// FUNCTION: FF7 0x004193c2
int __cdecl Kernel_GetUseCondition(int category, int entry)
{
    int result;
    unsigned char conditionByte;
    conditionByte = 0xFF;
    result = -1;
    switch (category) {
    case KERNEL_TEXT_MAGIC:
    case KERNEL_TEXT_SUMMON:
    case KERNEL_TEXT_ENEMY_SKILL:
        conditionByte = g_BattleAttackTable[(g_KernelUseConditionOffsets[category] + entry)].useCondition;
        break;
    case KERNEL_TEXT_ITEM:
        if (entry < 0x80) {
            conditionByte = g_MenuKernelItemBuffer[entry].useCondition;
        }
        break;
    }
    if (conditionByte != 0xFF) {
        result = conditionByte;
    }
    return result;
}

// Returns battle message `value` from text bank 0x10.
// FUNCTION: FF7 0x00419442
char *__cdecl Kernel_GetBattleMessage(int value)
{
    return Kernel_GetTextBankEntry(0x10, value, 0);
}

// Finds the bank at g_KernelTextPool+g_KernelTextBankOffsets[bank+flags], reads the selected unsigned 16-bit
// offset from its offset table, and returns bank base plus that offset.
// FUNCTION: FF7 0x00419457
char *__cdecl Kernel_GetTextBankEntry(int bank, int entry, int flags)
{
    char *p;
    unsigned short *offsets;

    p = &g_KernelTextPool[g_KernelTextBankOffsets[bank + flags]];
    offsets = (unsigned short *)p;
    return p + offsets[entry];
}

// Returns battle message `messageId` with its back-references expanded in place.
// FUNCTION: FF7 0x00419491
char *__cdecl Kernel_GetBattleMessageExpanded(int messageId)
{
    char *text;

    text = Kernel_GetBattleMessage(messageId);
    return Kernel_ExpandTextBackrefs(text, text);
}

/*
 * Expands compressed backreferences from encoded input into the shared text expansion buffer.
 */
// FUNCTION: FF7 0x004194b8
char *__cdecl Kernel_ExpandTextBackrefs(char *text, char *source)
{
    int readPosition;
    int outOfs;
    unsigned char curByte;
    int copyLength;
    int copyIndex;
    int backrefPosition;

    outOfs = 0;
    readPosition = 0;
    while ((unsigned int)outOfs < (unsigned int)0x100) {
        curByte = text[readPosition];
        readPosition++;
        if (curByte == TEXT_BACKREF) {
            curByte = text[readPosition];
            readPosition++;
            backrefPosition = readPosition - ((curByte & 0x3F) + 3);
            copyLength = (curByte >> 6) * 2 + 4;
            for (copyIndex = 0; copyIndex < copyLength; copyIndex++) {
                curByte = source[backrefPosition];
                g_KernelExpandedTextBuffer[outOfs] = curByte;
                outOfs++;
                backrefPosition++;
            }
        } else if (curByte >= 0xEA && curByte < 0xF2) {
            g_KernelExpandedTextBuffer[outOfs] = curByte;
            outOfs++;
            g_KernelExpandedTextBuffer[outOfs] = text[readPosition];
            outOfs++;
            readPosition++;
            g_KernelExpandedTextBuffer[outOfs] = text[readPosition];
            outOfs++;
            readPosition++;
        } else {
            g_KernelExpandedTextBuffer[outOfs] = curByte;
            outOfs++;
        }
        if (curByte == TEXT_END) {
            break;
        }
    }
    return (char *)g_KernelExpandedTextBuffer;
}

/*
 * Returns the FF7-encoded text (0xFF-terminated) of entry `id` of text group `group`
 * (`enum KernelTextGroup`), the lookup behind every name and description the menus and the battle
 * UI show. `slot` is added to the group's kernel text bank: the callers pass 8 for the name and 0
 * for the description, and slot-0 text has its back-references expanded. Unknown entries give the
 * empty string g_KernelEmptyString.
 * - KERNEL_TEXT_ITEM takes an inventory id and maps it by range: 0..127 items, 128..255 weapons
 *   (group 10), 256..287 armour (11), 288..383 accessories (12), 384 and up materia (13).
 * - Magic, summon, enemy skill and limit ids are offset by 0, 0x38, 0x48 and 0x80 into the shared
 *   attack text when the result stays below 0xE0; limit id 0x7F and any id 0xFF give the empty
 *   string.
 * - KERNEL_TEXT_SUMMON_ATTACK reads bank 0x11 for ids below 0x10, else the magic names (bank 9).
 * - KERNEL_TEXT_ENEMY_NAME builds, for enemy slot `id` 0..5, the enemy's name plus its letter when
 *   several share it, battle message 0x71 when its combatant flag 0x40 is set, and its HP as
 *   current / max (messages 0x7F and 0x72) when its actor timing flag 0x40 is set.
 * - KERNEL_TEXT_BATTLE_MESSAGE formats battle message `id`, or the scene's AI message id - 0x100
 *   for ids from 0x100.
 * - KERNEL_TEXT_ENEMY_ATTACK returns the scene's 32-byte attack name `id`.
 * The built texts live in shared static buffers (g_KernelFormattedTextBuffer,
 * g_KernelExpandedTextBuffer) that the next call overwrites.
 */
// FUNCTION: FF7 0x0041963c
char *__cdecl Kernel_LookupUiText(unsigned int group, int id, int slot)
{
    short args[2];
    char *desc;
    int targ;
    char *mid;
    char *cp;
    char *str;
    unsigned int ix;
    char *op;
    char buf[0x100];

    op = (char *)g_KernelEmptyString;
    if (group == KERNEL_TEXT_ITEM) {
        for (ix = 0; ix < 5; ix++) {
            if (id < g_KernelUiTextGroup4Thresholds[ix + 1]) {
                group = g_KernelUiTextGroup4TargetGroups[ix];
                id = id - g_KernelUiTextGroup4Thresholds[ix];
                break;
            }
        }
    }
    if (group == KERNEL_TEXT_LIMIT && id == 0x7F) {
        id = 0xFF;
    }
    if (id != 0xFF) {
        if (group < 4 && id + g_KernelUiTextGroupIdOffsets[group] < 0xE0) {
            id = id + g_KernelUiTextGroupIdOffsets[group];
        }
        if (g_KernelUiTextGroupBankMap[group] != 0xFF) {
            op = Kernel_GetTextBankEntry(
                g_KernelUiTextGroupBankMap[group] + slot, id, 0);
            if (slot == 0) {
                op = Kernel_ExpandTextBackrefs(op, op);
            }
        } else {
            switch (group) {
            case KERNEL_TEXT_SUMMON_ATTACK:
                if (id < 0x10) {
                    op = Kernel_GetTextBankEntry(0x11, id, 0);
                } else {
                    op = Kernel_GetTextBankEntry(9, id, 0);
                }
                break;
            case KERNEL_TEXT_ENEMY_NAME:
                if (id < 6) {
                    cp = (char *)&g_KernelFormattedTextBuffer;
                    mid = (char *)g_BattleState.enemyRecords[g_BattleScene.enemies[id].enemyIndex].name;
                    targ = id + 4;
                    cp = Kernel_AppendEncodedString(cp, mid, 0x20);
                    if ((*(unsigned char *)&g_BattleState.actors[targ].nameSuffixIndex) != 0xFF) {
                        *cp = (char)(g_BattleSequenceBuffer.textFormatBase + (*(unsigned char *)&g_BattleState.actors[targ].nameSuffixIndex));
                        cp = cp + 1;
                    }
                    if ((gBattleAiGlobals.combatants[targ].flags & BATTLE_COMBATANT_BACK_ROW) != 0) {
                        mid = Kernel_GetBattleMessageExpanded(0x71);
                        cp = Kernel_AppendEncodedString(cp, mid, -1);
                    }
                    if ((g_BattleState.actors[targ].timingFlags & 0x40) != 0) {
                        args[0] = g_BattleState.actors[targ].hp;
                        args[1] = gBattleAiGlobals.combatants[targ].maxHp;
                        mid = Kernel_GetBattleMessageExpanded(0x7F);
                        cp = Kernel_AppendEncodedString(cp, mid, -1);
                        desc = Kernel_GetBattleMessageExpanded(0x72);
                        Battle_ExpandMessageArgs(cp, desc, (unsigned short *)args);
                        Kernel_FormatBattleText(buf, (unsigned char *)cp);
                        cp = Kernel_AppendEncodedString(cp, buf, -1);
                    }
                    *cp = (char)0xFF;
                    op = (char *)&g_KernelFormattedTextBuffer;
                }
                break;
            case KERNEL_TEXT_BATTLE_MESSAGE:
                if (id >= 0x100) {
                    str = Battle_GetAiMessageText(id - 0x100);
                } else {
                    str = Kernel_GetBattleMessage(id);
                }
                op = Kernel_FormatBattleText(buf, (unsigned char *)str);
                op = Kernel_ExpandTextBackrefs(op, str);
                break;
            case KERNEL_TEXT_ENEMY_ATTACK:
                op = (char *)g_BattleState.sceneRecordBlockD + (id << 5);
                break;
            }
        }
    }
    return op;
}

// Copies encoded bytes until a 0xff terminator or the post-copy limit test stops the loop, and
// returns the advanced output pointer without appending a terminator. A nonempty input with limit
// zero still copies one byte. `limit` is tested before it is decremented.
// FUNCTION: FF7 0x00419a48
char *__cdecl Kernel_AppendEncodedString(char *out, char *text, int limit)
{
    while ((unsigned char)*text != 0xff) {
        *out = *text;
        out = out + 1;
        text = text + 1;
        if (limit-- == 0) {
            break;
        }
    }
    return out;
}

/*
 * Formats encoded battle text into out, replacing each escape 0xEA-0xF1 (0xEA + a TextBattleEscape)
 * and its 16-bit operand with the text it names; a TEXT_BACKREF and its operand byte are copied as is.
 */
// FUNCTION: FF7 0x00419a8f
char *__cdecl Kernel_FormatBattleText(char *out, unsigned char *text)
{
    char *ptr;
    int ix;
    char *cp;
    unsigned char curByte;
    char *rec;
    int ct;
    int dstk[16];
    unsigned short arg;
    typedef char *(__cdecl * AppendBankFn)(int, int, char *);

    ix = 0;
    curByte = 0;
    cp = out;
    while (curByte != TEXT_END) {
        curByte = text[ix];
        ix = ix + 1;
        if (curByte >= 0xEA && curByte < 0xF2) {
            arg = (unsigned short)(text[ix] << 8);
            ix = ix + 1;
            arg |= text[ix];
            ix = ix + 1;
            switch (curByte - 0xEA) {
            case TEXT_BATTLE_ESC_CHARACTER_NAME:
                cp = Kernel_AppendCharacterName(arg, cp);
                break;
            case TEXT_BATTLE_ESC_ITEM_NAME:
                ptr = Kernel_LookupUiText(KERNEL_TEXT_ITEM, arg, 8);
                cp = Kernel_AppendEncodedString(cp, ptr, -1);
                break;
            case TEXT_BATTLE_ESC_NUMBER:
                if (1) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
                    ct = 0;
                    while (ct < sizeof(dstk) / sizeof(dstk[0])) {
                        dstk[ct] = arg % 10;
                        ct = ct + 1;
                        arg = arg / 10;
                        if (arg == 0) {
                            break;
                        }
                    }
                    for (; ct > 0; ct--) {
                        *cp = (char)(dstk[ct - 1] + g_BattleDigitGlyphBase);
                        cp = cp + 1;
                    }
                }
                break;
            case TEXT_BATTLE_ESC_COMBATANT_NAME:
                if (arg < 3) {
                    cp = Kernel_AppendCharacterName(
                        g_BattleScene.actors[arg].unitIndex, cp);
                } else if (arg >= 4) {
                    rec = (char *)g_BattleState.enemyRecords[g_BattleScene.enemies[(arg - 4)].enemyIndex].name;
                    cp = Kernel_AppendEncodedString(cp, rec, 0x20);
                }
                break;
            case TEXT_BATTLE_ESC_MAGIC_NAME:
                cp = ((AppendBankFn)Kernel_AppendTextBankEntry)(9, arg, cp);
                break;
            case TEXT_BATTLE_ESC_GLYPH:
                if (arg < 0x1A) {
                    *cp = (char)(g_BattleSequenceBuffer.textFormatBase + arg);
                    cp = cp + 1;
                }
                break;
            case TEXT_BATTLE_ESC_BATTLE_MESSAGE:
                cp = ((AppendBankFn)Kernel_AppendTextBankEntry)(0x10, arg, cp);
                break;
            case TEXT_BATTLE_ESC_BANK_ENTRY:
                cp = ((AppendBankFn)Kernel_AppendTextBankEntry)(arg >> 8, arg & 0xFF, cp);
                break;
            }
        } else {
            *cp = curByte;
            cp = cp + 1;
            if (curByte == TEXT_BACKREF) {
                *cp = text[ix];
                cp = cp + 1;
                ix = ix + 1;
            }
        }
    }
    return out;
}

/*
 * Appends an entry from the specified kernel text bank to out.
 */
// FUNCTION: FF7 0x00419db6
void __cdecl Kernel_AppendTextBankEntry(int bank, int entry, char *out)
{
    Kernel_AppendEncodedString(
        out, Kernel_GetTextBankEntry(bank, entry, 0), -1);
}

/*
 * Looks up character name by character ID and appends it to out.
 */
// FUNCTION: FF7 0x00419ddc
char *__cdecl Kernel_AppendCharacterName(int charId, char *out)
{
    int slotIndex;
    char *namePointer;

    for (slotIndex = 0; slotIndex < 9; slotIndex++) {
        if (gSavemap.chars[slotIndex].id == charId) {
            namePointer = (char *)&gSavemap.chars[slotIndex].name;
            out = Kernel_AppendEncodedString(out, namePointer, 0xC);
            break;
        }
    }
    return out;
}
} /* extern "C" */
