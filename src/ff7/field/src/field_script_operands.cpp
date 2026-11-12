// Contents: Reads and writes field-script operands through bank selectors, and implements the
//   script opcodes that set and compare values, branch, test bits, define lines, make requests and
//   reach sound and minigame commands.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/field.h"
#include "ff7/enums/field.h"
#include "ff7/enums/main.h"
#include "ff7/api/battle.h"
#include "ff7/api/field.h"
#include "lib/api/graphics.h"
#include "lib/api/sound.h"
#include "lib/api/token.h"
#include "ff7/field.h"
#include "ff7/savemap.h"
#include "lib/crt.h"
#include "lib/enums/sound.h"
#include "ff7/api/menu.h"
extern "C" {

struct FieldPointTo;

// clang-format off
// GLOBAL: FF7 0x00905e50
unsigned char g_FieldRandomStep = 1;
// clang-format on

/* Int amount matches VC5 caller push without narrow short sign-extension; callee reads 16-bit low word. */
extern void __cdecl Savemap_HealHp(int slot, int amount);
/* Retail caller pushes int amount without short truncation; definition takes short. */
extern void __cdecl Savemap_HealMp(int slot, int amount);

// 0x00613B79 keeps its return in [ebp-4] and hands it back, so it is int.
// Savemap_RemoveMateriaReturnZero (0x006CC2ED) and Savemap_CountMateriaReturnZero (0x006CC2F4) are
// seven-byte `return 0;` shells that ignore their two arguments (the materia and a count).

// 0x0061F49C sign-extends the argument before pushing it and
// does not extend the return, so both are int.
extern void __cdecl Savemap_AddItem(short itemRef);

/*
 * The field script VM: its operand readers and its opcode handlers.
 *
 * Reading the handler comments: [n] is the script byte n places after the opcode, taken literally;
 * v[n]/s is the operand at byte n read or written through the bank nibble s (1..6: the high and low
 * nibbles of [1], [2] and [3]; see Script_ReadOperand8). Coordinates are in field units (the models
 * keep them shifted left 12) and facings in 256ths of a turn. Mnemonics are the community's
 *
 * A handler returns 0 to let the entity run its next opcode in the same frame, nonzero to end the
 * entity's turn. An opcode that waits returns nonzero without moving the pc, so the scheduler runs it
 * again next frame; the field requests (BATTLE, MAPJUMP, MENU, MOVIE, ...) wait that way until the
 * engine reports the request done.
 */

/*
 * Reads a byte operand. The bank nibble at slot nibbleSelector (1..6: the high and low nibbles of [1], [2]
 * and [3]) says what the byte at [instrOffset] is: nibble 0 means it is the value itself; 1/2, 3/4, 11/12,
 * 13/14 and 15/7 make it an index into the savemap's script variables (blocks at scriptBanks 0x000, 0x100,
 * 0x200, 0x300 and 0x400, saved with the game); 5/6 an index into the field's temporary bank, which is not
 * saved. The unassigned nibbles 8, 9 and 10 read 0.
 *
 * A selector outside 1..6 leaves bankSelector uninitialized, so the bank is whatever the stack held. The
 * declared unsigned return preserves all eight bits.
 */
// FUNCTION: FF7 0x0060f750
unsigned char __cdecl Script_ReadOperand8(short nibbleSelector, short instrOffset)
{
    unsigned char bankSelector;
    unsigned char value;
    short bankIndex;

    switch (nibbleSelector) {
    case FIELD_BANK_SLOT_1_HI:
        bankSelector = SCRIPT_PARAM_U8(1) >> 4 & 0xF;
        break;
    case FIELD_BANK_SLOT_1_LO:
        bankSelector = SCRIPT_PARAM_U8(1) & 0xF;
        break;
    case FIELD_BANK_SLOT_2_HI:
        bankSelector = SCRIPT_PARAM_U8(2) >> 4 & 0xF;
        break;
    case FIELD_BANK_SLOT_2_LO:
        bankSelector = SCRIPT_PARAM_U8(2) & 0xF;
        break;
    case FIELD_BANK_SLOT_3_HI:
        bankSelector = SCRIPT_PARAM_U8(3) >> 4 & 0xF;
        break;
    case FIELD_BANK_SLOT_3_LO:
        bankSelector = SCRIPT_PARAM_U8(3) & 0xF;
    }
    switch (bankSelector) {
    case FIELD_BANK_LITERAL:
        value = SCRIPT_PARAM_S8(instrOffset);
        return (char)value;
    case FIELD_BANK_SAVE_000_BYTE:
    case FIELD_BANK_SAVE_000_WORD:
        bankIndex = SCRIPT_PARAM_U8(instrOffset);
        value = (char)gSavemap.scriptBanks[bankIndex];
        return (char)value;
    case FIELD_BANK_SAVE_100_BYTE:
    case FIELD_BANK_SAVE_100_WORD:
        bankIndex = SCRIPT_PARAM_U8(instrOffset) + 0x100;
        value = (char)gSavemap.scriptBanks[bankIndex];
        return (char)value;
    case FIELD_BANK_SAVE_200_BYTE:
    case FIELD_BANK_SAVE_200_WORD:
        bankIndex = SCRIPT_PARAM_U8(instrOffset) + 0x200;
        value = (char)gSavemap.scriptBanks[bankIndex];
        return (char)value;
    case FIELD_BANK_SAVE_300_BYTE:
    case FIELD_BANK_SAVE_300_WORD:
        bankIndex = SCRIPT_PARAM_U8(instrOffset) + 0x300;
        value = (char)gSavemap.scriptBanks[bankIndex];
        return (char)value;
    case FIELD_BANK_SAVE_400_WORD:
    case FIELD_BANK_SAVE_400_BYTE:
        bankIndex = SCRIPT_PARAM_U8(instrOffset) + 0x400;
        value = (char)gSavemap.scriptBanks[bankIndex];
        return (char)value;
    case FIELD_BANK_TEMP_BYTE:
    case FIELD_BANK_TEMP_WORD:
        bankIndex = SCRIPT_PARAM_U8(instrOffset);
        value = (char)g_FieldTempBank[bankIndex];
        return (char)value;
    default:
        return 0;
    }
}

/*
 * Stores a byte into the script variable named by the bank nibble at slot nibbleSelector and the index at
 * [instrOffset]: the savemap blocks 0x000..0x400 (either code of a pair) or the temporary bank. A literal
 * (0) or unassigned nibble stores nothing, so an opcode whose destination is a constant is a no-op. A
 * selector outside 1..6 leaves bankSelector uninitialized.
 */
// FUNCTION: FF7 0x0060fa7d
void __cdecl Script_WriteOperand8(short nibbleSelector, short instrOffset, unsigned char value)
{
    unsigned char bankSelector;
    short bankIndex;

    switch (nibbleSelector) {
    case FIELD_BANK_SLOT_1_HI:
        bankSelector = SCRIPT_PARAM_U8(1) >> 4 & 0xF;
        break;
    case FIELD_BANK_SLOT_1_LO:
        bankSelector = SCRIPT_PARAM_U8(1) & 0xF;
        break;
    case FIELD_BANK_SLOT_2_HI:
        bankSelector = SCRIPT_PARAM_U8(2) >> 4 & 0xF;
        break;
    case FIELD_BANK_SLOT_2_LO:
        bankSelector = SCRIPT_PARAM_U8(2) & 0xF;
        break;
    case FIELD_BANK_SLOT_3_HI:
        bankSelector = SCRIPT_PARAM_U8(3) >> 4 & 0xF;
        break;
    case FIELD_BANK_SLOT_3_LO:
        bankSelector = SCRIPT_PARAM_U8(3) & 0xF;
    }
    switch (bankSelector) {
    case FIELD_BANK_SAVE_000_BYTE:
    case FIELD_BANK_SAVE_000_WORD:
        bankIndex = SCRIPT_PARAM_U8(instrOffset);
        gSavemap.scriptBanks[bankIndex] = value;
        break;
    case FIELD_BANK_SAVE_100_BYTE:
    case FIELD_BANK_SAVE_100_WORD:
        bankIndex = SCRIPT_PARAM_U8(instrOffset) + 0x100;
        gSavemap.scriptBanks[bankIndex] = value;
        break;
    case FIELD_BANK_SAVE_200_BYTE:
    case FIELD_BANK_SAVE_200_WORD:
        bankIndex = SCRIPT_PARAM_U8(instrOffset) + 0x200;
        gSavemap.scriptBanks[bankIndex] = value;
        break;
    case FIELD_BANK_SAVE_300_BYTE:
    case FIELD_BANK_SAVE_300_WORD:
        bankIndex = SCRIPT_PARAM_U8(instrOffset) + 0x300;
        gSavemap.scriptBanks[bankIndex] = value;
        break;
    case FIELD_BANK_SAVE_400_WORD:
    case FIELD_BANK_SAVE_400_BYTE:
        bankIndex = SCRIPT_PARAM_U8(instrOffset) + 0x400;
        gSavemap.scriptBanks[bankIndex] = value;
        break;
    case FIELD_BANK_TEMP_BYTE:
    case FIELD_BANK_TEMP_WORD:
        bankIndex = SCRIPT_PARAM_U8(instrOffset);
        g_FieldTempBank[bankIndex] = value;
    }
}

/*
 * Reads a 16-bit operand. With nibble 0 the two bytes at [addr] are the value, low byte first. With a bank
 * nibble [addr] is a variable index, and the two codes of a pair differ in width: 1, 3, 11, 13, 15 and 5
 * read the one byte there (zero-extended), 2, 4, 12, 14, 7 and 6 read it and the byte after it as a word.
 * Unassigned nibbles read 0; a bank argument outside 1..6 leaves the selector uninitialized.
 */
// FUNCTION: FF7 0x0060fd6c
short __cdecl Script_ReadOperand16(short bank, short addr)
{
    short value;
    unsigned char selector;
    short bankIndex;

    switch (bank) {
    case FIELD_BANK_SLOT_1_HI:
        selector = SCRIPT_PARAM_U8(1) >> 4 & 0xF;
        break;
    case FIELD_BANK_SLOT_1_LO:
        selector = SCRIPT_PARAM_U8(1) & 0xF;
        break;
    case FIELD_BANK_SLOT_2_HI:
        selector = SCRIPT_PARAM_U8(2) >> 4 & 0xF;
        break;
    case FIELD_BANK_SLOT_2_LO:
        selector = SCRIPT_PARAM_U8(2) & 0xF;
        break;
    case FIELD_BANK_SLOT_3_HI:
        selector = SCRIPT_PARAM_U8(3) >> 4 & 0xF;
        break;
    case FIELD_BANK_SLOT_3_LO:
        selector = SCRIPT_PARAM_U8(3) & 0xF;
    }
    switch (selector) {
    case FIELD_BANK_LITERAL:
        value = SCRIPT_PARAM_U8(addr);
        value |= *(unsigned char *)(((char *)g_FieldScripts + g_FieldScriptPC[g_CurrentEntity]) + addr + 1) << 8;
        return value;
    case FIELD_BANK_SAVE_000_BYTE:
        bankIndex = SCRIPT_PARAM_U8(addr);
        value = gSavemap.scriptBanks[bankIndex];
        return value;
    case FIELD_BANK_SAVE_000_WORD:
        bankIndex = SCRIPT_PARAM_U8(addr);
        value = gSavemap.scriptBanks[bankIndex];
        value |= *(unsigned char *)((char *)&gSavemapBankHi + bankIndex) << 8;
        return value;
    case FIELD_BANK_SAVE_100_BYTE:
        bankIndex = SCRIPT_PARAM_U8(addr) + 0x100;
        value = gSavemap.scriptBanks[bankIndex];
        return value;
    case FIELD_BANK_SAVE_100_WORD:
        bankIndex = SCRIPT_PARAM_U8(addr) + 0x100;
        value = gSavemap.scriptBanks[bankIndex];
        value |= *(unsigned char *)((char *)&gSavemapBankHi + bankIndex) << 8;
        return value;
    case FIELD_BANK_SAVE_200_BYTE:
        bankIndex = SCRIPT_PARAM_U8(addr) + 0x200;
        value = gSavemap.scriptBanks[bankIndex];
        return value;
    case FIELD_BANK_SAVE_200_WORD:
        bankIndex = SCRIPT_PARAM_U8(addr) + 0x200;
        value = gSavemap.scriptBanks[bankIndex];
        value |= *(unsigned char *)((char *)&gSavemapBankHi + bankIndex) << 8;
        return value;
    case FIELD_BANK_SAVE_300_BYTE:
        bankIndex = SCRIPT_PARAM_U8(addr) + 0x300;
        value = gSavemap.scriptBanks[bankIndex];
        return value;
    case FIELD_BANK_SAVE_300_WORD:
        bankIndex = SCRIPT_PARAM_U8(addr) + 0x300;
        value = gSavemap.scriptBanks[bankIndex];
        value |= *(unsigned char *)((char *)&gSavemapBankHi + bankIndex) << 8;
        return value;
    case FIELD_BANK_SAVE_400_BYTE:
        bankIndex = SCRIPT_PARAM_U8(addr) + 0x400;
        value = gSavemap.scriptBanks[bankIndex];
        return value;
    case FIELD_BANK_SAVE_400_WORD:
        bankIndex = SCRIPT_PARAM_U8(addr) + 0x400;
        value = gSavemap.scriptBanks[bankIndex];
        value |= *(unsigned char *)((char *)&gSavemapBankHi + bankIndex) << 8;
        return value;
    case FIELD_BANK_TEMP_BYTE:
        bankIndex = SCRIPT_PARAM_U8(addr);
        value = g_FieldTempBank[bankIndex];
        return value;
    case FIELD_BANK_TEMP_WORD:
        bankIndex = SCRIPT_PARAM_U8(addr);
        value = g_FieldTempBank[bankIndex];
        value |= *(unsigned char *)((char *)((char *)&g_FieldTempBank + 1) + bankIndex) << 8;
        return value;
    default:
        return 0;
    }
}

/*
 * Stores a 16-bit value into a script variable: the byte codes (1, 3, 11, 13, 15, 5) keep only the low
 * byte, the word codes (2, 4, 12, 14, 7, 6) store the low byte at the index and the high byte after it. A
 * word at index 255 spills into the first byte of the next block (or past the temporary bank); nothing
 * checks. Literal and unassigned nibbles store nothing, and a nibbleSelector outside 1..6 leaves the
 * selector uninitialized.
 */
// FUNCTION: FF7 0x0061031e
void __cdecl Script_WriteOperand16(short nibbleSelector, short instrOffset, short value)
{
    unsigned char bankSelector;
    short bankIndex;

    switch (nibbleSelector) {
    case FIELD_BANK_SLOT_1_HI:
        bankSelector = SCRIPT_PARAM_U8(1) >> 4 & 0xF;
        break;
    case FIELD_BANK_SLOT_1_LO:
        bankSelector = SCRIPT_PARAM_U8(1) & 0xF;
        break;
    case FIELD_BANK_SLOT_2_HI:
        bankSelector = SCRIPT_PARAM_U8(2) >> 4 & 0xF;
        break;
    case FIELD_BANK_SLOT_2_LO:
        bankSelector = SCRIPT_PARAM_U8(2) & 0xF;
        break;
    case FIELD_BANK_SLOT_3_HI:
        bankSelector = SCRIPT_PARAM_U8(3) >> 4 & 0xF;
        break;
    case FIELD_BANK_SLOT_3_LO:
        bankSelector = SCRIPT_PARAM_U8(3) & 0xF;
    }
    switch (bankSelector) {
    case FIELD_BANK_SAVE_000_BYTE:
        bankIndex = SCRIPT_PARAM_U8(instrOffset);
        gSavemap.scriptBanks[bankIndex] = value;
        break;
    case FIELD_BANK_SAVE_000_WORD:
        bankIndex = SCRIPT_PARAM_U8(instrOffset);
        gSavemap.scriptBanks[bankIndex] = value;
        *(char *)((char *)&gSavemapBankHi + bankIndex) = value >> 8;
        break;
    case FIELD_BANK_SAVE_100_BYTE:
        bankIndex = SCRIPT_PARAM_U8(instrOffset) + 0x100;
        gSavemap.scriptBanks[bankIndex] = value;
        break;
    case FIELD_BANK_SAVE_100_WORD:
        bankIndex = SCRIPT_PARAM_U8(instrOffset) + 0x100;
        gSavemap.scriptBanks[bankIndex] = value;
        *(char *)((char *)&gSavemapBankHi + bankIndex) = value >> 8;
        break;
    case FIELD_BANK_SAVE_200_BYTE:
        bankIndex = SCRIPT_PARAM_U8(instrOffset) + 0x200;
        gSavemap.scriptBanks[bankIndex] = value;
        break;
    case FIELD_BANK_SAVE_200_WORD:
        bankIndex = SCRIPT_PARAM_U8(instrOffset) + 0x200;
        gSavemap.scriptBanks[bankIndex] = value;
        *(char *)((char *)&gSavemapBankHi + bankIndex) = value >> 8;
        break;
    case FIELD_BANK_SAVE_300_BYTE:
        bankIndex = SCRIPT_PARAM_U8(instrOffset) + 0x300;
        gSavemap.scriptBanks[bankIndex] = value;
        break;
    case FIELD_BANK_SAVE_300_WORD:
        bankIndex = SCRIPT_PARAM_U8(instrOffset) + 0x300;
        gSavemap.scriptBanks[bankIndex] = value;
        *(char *)((char *)&gSavemapBankHi + bankIndex) = value >> 8;
        break;
    case FIELD_BANK_SAVE_400_BYTE:
        bankIndex = SCRIPT_PARAM_U8(instrOffset) + 0x400;
        gSavemap.scriptBanks[bankIndex] = value;
        break;
    case FIELD_BANK_SAVE_400_WORD:
        bankIndex = SCRIPT_PARAM_U8(instrOffset) + 0x400;
        gSavemap.scriptBanks[bankIndex] = value;
        *(char *)((char *)&gSavemapBankHi + bankIndex) = value >> 8;
        break;
    case FIELD_BANK_TEMP_BYTE:
        bankIndex = SCRIPT_PARAM_U8(instrOffset);
        g_FieldTempBank[bankIndex] = value;
        break;
    case FIELD_BANK_TEMP_WORD:
        bankIndex = SCRIPT_PARAM_U8(instrOffset);
        g_FieldTempBank[bankIndex] = value;
        *(char *)((char *)((char *)&g_FieldTempBank + 1) + bankIndex) = value >> 8;
    }
}

/*
 * The thirteen unused opcode numbers. The handler yields without moving the pc, so it runs again every
 * frame and a script that reaches one of these opcodes stops there for good.
 *
 * Field opcodes 0x0C, 0x0D, 0x1A..0x1F, 0x44, 0x46, 0x4C, 0x4E and 0xBE
 * (FIELD_OP_UNIMPLEMENTED_STALL_0C ... FIELD_OP_UNIMPLEMENTED_STALL_BE).
 */
// FUNCTION: FF7 0x006107e1
int __cdecl Script_OpUnimplementedStall(void)
{
    return 1;
}

/*
 * NOP (community mnemonic): does nothing but end this entity's turn for the frame. 1 byte.
 *
 * Field opcode 0x5F (FIELD_OP_YIELD_ONE_OPCODE).
 */
// FUNCTION: FF7 0x006107eb
int __cdecl Script_OpYieldOneOpcode(void)
{
    g_FieldScriptPC[g_CurrentEntity] += 1;
    return 1;
}

/*
 * WAIT: pauses this script for the number of frames in the literal word [1..2]. 3 bytes; blocks.
 *
 * The first run loads the count into the entity's wait counter; each later frame counts it down, and on
 * the frame it reaches 1 the script carries on with the next opcode at once. WAIT 0 steps over itself but
 * still yields once. The engine context is fetched and not used.
 *
 * Field opcode 0x24 (FIELD_OP_WAIT_FRAMES).
 */
// FUNCTION: FF7 0x00610818
int __cdecl Script_OpWaitFrames(void)
{
    void *engine;

    engine = (void *)GetEngineContext();
    /* Numeric remaining wait-frame count (0 or 1). */
    switch (g_FieldScriptWait[g_CurrentEntity]) {
    case FIELD_WAIT_LOAD_OPERAND:
        // First run: load the frame count.
        g_FieldScriptWait[g_CurrentEntity] = (unsigned short)SCRIPT_PARAM_U8(1);
        g_FieldScriptWait[g_CurrentEntity] |= SCRIPT_PARAM_U8(2) << 8;
        if (g_FieldScriptWait[g_CurrentEntity] != 0) {
            return 1;
        }
        g_FieldScriptPC[g_CurrentEntity] += 3;
        return 1;
    case FIELD_WAIT_COMPLETE_NEXT_TICK:
        // The last frame of the wait: go on with the next opcode now.
        g_FieldScriptWait[g_CurrentEntity] = 0;
        g_FieldScriptPC[g_CurrentEntity] += 3;
        return 0;
    }
    g_FieldScriptWait[g_CurrentEntity] -= 1;
    return 1;
}

/*
 * SETBYTE: byte variable v[2]/1 = byte v[3]/2. 4 bytes.
 *
 * Field opcode 0x80 (FIELD_OP_SET_BYTE).
 */
// FUNCTION: FF7 0x00610973
int __cdecl Script_OpSetByte(void)
{
    Script_WriteOperand8(1, 2, Script_ReadOperand8(2, 3));
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * SETWORD: variable v[2]/1 = 16-bit v[3]/2. 5 bytes.
 *
 * Field opcode 0x81 (FIELD_OP_SET_WORD).
 */
// FUNCTION: FF7 0x006109b6
int __cdecl Script_OpSetWord(void)
{
    Script_WriteOperand16(1, 2, Script_ReadOperand16(2, 3));
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * LBYTE (community mnemonic: the low byte of a word): v[2]/1 = v[3]/2 read as a byte, which makes it
 * SETBYTE again. 4 bytes.
 *
 * Byte for byte the same body as Script_OpSetByte at 0x00610973; two table slots with one implementation
 * apiece rather than a shared one.
 *
 * Field opcode 0x9A (FIELD_OP_SET_BYTE_ALIAS).
 */
// FUNCTION: FF7 0x006109f9
int __cdecl Script_OpSetByteAlias(void)
{
    Script_WriteOperand8(1, 2, Script_ReadOperand8(2, 3));
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * HBYTE: byte variable v[2]/1 = the high byte of the 16-bit v[3]/2. 5 bytes.
 *
 * `sar`, so the operand keeps its sign through the shift.
 *
 * Field opcode 0x9B (FIELD_OP_GET_HIGH_BYTE).
 */
// FUNCTION: FF7 0x00610a3c
int __cdecl Script_OpGetHighByte(void)
{
    short value;

    value = Script_ReadOperand16(2, 3);
    Script_WriteOperand8(1, 2, value >> 8);
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * 2BYTE: joins two bytes into a word: v[3]/1 = v[4]/2 | v[5]/4 << 8. 6 bytes.
 *
 * Field opcode 0x9C (FIELD_OP_SET_WORD_FROM_BYTES).
 */
// FUNCTION: FF7 0x00610a8d
int __cdecl Script_OpSetWordFromBytes(void)
{
    short value;

    value = Script_ReadOperand8(2, 4);
    value |= Script_ReadOperand8(4, 5) << 8;
    Script_WriteOperand16(1, 3, value);
    g_FieldScriptPC[g_CurrentEntity] += 6;
    return 0;
}

/*
 * SETX (community mnemonic): an indexed store, the field script's array write. Writes the byte v[5]/4 to
 * element [3] + v[3]/2 of the block the high nibble of [1] names: a savemap block by its byte code (1, 3,
 * 11, 13 or 15) or the temporary bank (5); any other nibble stores nothing. 7 bytes.
 *
 * The blocks follow each other, so an index past the end of one runs on into the next; only the end of all
 * five (0x4FF) and of the temporary bank (0xFF) are clamped, and negative indices are not checked.
 *
 * Field opcode 0x9D (FIELD_OP_STORE_BYTE_INDEXED).
 */
// FUNCTION: FF7 0x00610afe
int __cdecl Script_OpStoreByteIndexed(void)
{
    unsigned char value;
    unsigned char page;
    short addr;

    page = SCRIPT_PARAM_U8(1) >> 4;
    addr = Script_ReadOperand16(2, 3) + SCRIPT_PARAM_U8(3);
    value = Script_ReadOperand8(4, 5);
    switch (page) {
    case FIELD_BANK_SAVE_400_BYTE:
        addr += 0x100;
    case FIELD_BANK_SAVE_300_BYTE:
        addr += 0x100;
    case FIELD_BANK_SAVE_200_BYTE:
        addr += 0x100;
    case FIELD_BANK_SAVE_100_BYTE:
        addr += 0x100;
    case FIELD_BANK_SAVE_000_BYTE:
        if (addr >= 0x500) {
            addr = 0x4ff;
        }
        gSavemap.scriptBanks[addr] = value;
        break;
    case FIELD_BANK_TEMP_BYTE:
        if (addr >= 0x100) {
            addr = 0xff;
        }
        g_FieldTempBank[addr] = value;
        break;
    }
    g_FieldScriptPC[g_CurrentEntity] += 7;
    return 0;
}

/*
 * GETX (community mnemonic): the indexed load that pairs with SETX: v[5]/4 = element [3] + v[3]/2 of the
 * block the high nibble of [1] names, with the same block choice and clamping. A nibble that names no
 * block writes an uninitialized local. 7 bytes.
 *
 * Field opcode 0x9E (FIELD_OP_LOAD_BYTE_INDEXED).
 */
// FUNCTION: FF7 0x00610c63
int __cdecl Script_OpLoadByteIndexed(void)
{
    unsigned char value;
    unsigned char page;
    short addr;

    page = SCRIPT_PARAM_U8(1) >> 4;
    addr = Script_ReadOperand16(2, 3) + SCRIPT_PARAM_U8(3);
    switch (page) {
    case FIELD_BANK_SAVE_400_BYTE:
        addr += 0x100;
    case FIELD_BANK_SAVE_300_BYTE:
        addr += 0x100;
    case FIELD_BANK_SAVE_200_BYTE:
        addr += 0x100;
    case FIELD_BANK_SAVE_100_BYTE:
        addr += 0x100;
    case FIELD_BANK_SAVE_000_BYTE:
        if (addr >= 0x500) {
            addr = 0x4ff;
        }
        value = gSavemap.scriptBanks[addr];
        break;
    case FIELD_BANK_TEMP_BYTE:
        if (addr >= 0x100) {
            addr = 0xff;
        }
        value = g_FieldTempBank[addr];
        break;
    }
    Script_WriteOperand8(4, 5, value);
    g_FieldScriptPC[g_CurrentEntity] += 7;
    return 0;
}

/*
 * SEARCHX: looks for the byte v[9]/4 in a span of a variable block: indices [4] + v[5]/2 to [4] + v[7]/3,
 * inclusive, of the block the high nibble of [1] names (as SETX). The first index found, counted from the
 * start of the savemap script variables (block offset included), goes to v[10]/6; -1 when it is not there
 * or the nibble names no block. 11 bytes.
 *
 * The ends are clamped from above only; negative values are not checked.
 *
 * Field opcode 0x9F (FIELD_OP_SEARCH_BANK_FOR_BYTE).
 */
// FUNCTION: FF7 0x00610dc8
int __cdecl Script_OpSearchBankForByte(void)
{
    unsigned char needle;
    unsigned char selector;
    short i;
    short endIndex;
    short startIdx;

    selector = SCRIPT_PARAM_U8(1) >> 4;
    startIdx = (short)Script_ReadOperand16(2, 5) + SCRIPT_PARAM_U8(4);
    endIndex = (short)Script_ReadOperand16(3, 7) + SCRIPT_PARAM_U8(4);
    needle = Script_ReadOperand8(4, 9);
    switch (selector) {
    case FIELD_BANK_SAVE_400_BYTE:
        startIdx += 256;
        endIndex += 256;
    case FIELD_BANK_SAVE_300_BYTE:
        startIdx += 256;
        endIndex += 256;
    case FIELD_BANK_SAVE_200_BYTE:
        startIdx += 256;
        endIndex += 256;
    case FIELD_BANK_SAVE_100_BYTE:
        startIdx += 256;
        endIndex += 256;
    case FIELD_BANK_SAVE_000_BYTE:
        if (startIdx >= 1280) {
            startIdx = 1279;
        }
        if (endIndex >= 1280) {
            endIndex = 1279;
        }
        for (i = startIdx; i <= endIndex; i++) {
            if (gSavemap.scriptBanks[i] == needle) {
                Script_WriteOperand16(6, 0xA, (short)i);
                g_FieldScriptPC[g_CurrentEntity] += 0xB;
                return 0;
            }
        }
        break;
    case FIELD_BANK_TEMP_BYTE:
        if (startIdx >= 256) {
            startIdx = 0xFF;
        }
        if (endIndex >= 256) {
            endIndex = 0xFF;
        }
        for (i = startIdx; i <= endIndex; i++) {
            if (g_FieldTempBank[i] == needle) {
                Script_WriteOperand16(6, 0xA, (short)i);
                g_FieldScriptPC[g_CurrentEntity] += 0xB;
                return 0;
            }
        }
    }
    Script_WriteOperand16(6, 0xA, -1);
    g_FieldScriptPC[g_CurrentEntity] += 0xB;
    return 0;
}

/*
 * BITON: sets bit v[3]/2 of the byte variable v[2]/1. 4 bytes. Story progress is mostly kept as such bits
 * in the savemap variables.
 *
 * Field opcode 0x82 (FIELD_OP_BIT_SET).
 */
// FUNCTION: FF7 0x00611098
int __cdecl Script_OpBitSet(void)
{
    Script_WriteOperand8(
        1, 2, Script_ReadOperand8(1, 2) | (1 << Script_ReadOperand8(2, 3)));
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * BITOFF: clears bit v[3]/2 of the byte variable v[2]/1. 4 bytes.
 *
 * Field opcode 0x83 (FIELD_OP_BIT_CLEAR).
 */
// FUNCTION: FF7 0x00611102
int __cdecl Script_OpBitClear(void)
{
    Script_WriteOperand8(
        1, 2, Script_ReadOperand8(1, 2) & ~(1 << Script_ReadOperand8(2, 3)));
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * BITXOR: flips bit v[3]/2 of the byte variable v[2]/1. 4 bytes.
 *
 * Field opcode 0x84 (FIELD_OP_BIT_TOGGLE).
 */
// FUNCTION: FF7 0x0061116e
int __cdecl Script_OpBitToggle(void)
{
    Script_WriteOperand8(
        1, 2, Script_ReadOperand8(1, 2) ^ (1 << Script_ReadOperand8(2, 3)));
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * LINE: gives this entity a trigger line between two 3D points, six literal signed words at [1..12] (x1,
 * y1, z1, x2, y2, z2). The player crossing or touching it runs the entity's line scripts (by the community
 * reference). 13 bytes.
 *
 * Lines come from one pool of 32 (g_FieldLines), handed out in the order the LINE opcodes run; when it is
 * full the opcode is skipped and the entity gets no line.
 *
 * Field opcode 0xD0 (FIELD_OP_DEFINE_LINE).
 */
// FUNCTION: FF7 0x006111d8
int __cdecl Script_OpDefineLine(void)
{
    short val;

    if (g_FieldLineCount >= 32) {
        g_FieldScriptPC[g_CurrentEntity] += 0xD;
        return 0;
    } else {
        g_FieldEntityLine[g_CurrentEntity] = *(char *)&g_FieldLineCount;
        val = SCRIPT_PARAM_U8(1);
        val |= (short)(SCRIPT_PARAM_U8(2) << 8);
        g_FieldLines[g_FieldLineCount].pos.x1 = val;
        val = SCRIPT_PARAM_U8(3);
        val |= (short)(SCRIPT_PARAM_U8(4) << 8);
        g_FieldLines[g_FieldLineCount].pos.y1 = val;
        val = SCRIPT_PARAM_U8(5);
        val |= (short)(SCRIPT_PARAM_U8(6) << 8);
        g_FieldLines[g_FieldLineCount].pos.z1 = val;
        val = SCRIPT_PARAM_U8(7);
        val |= (short)(SCRIPT_PARAM_U8(8) << 8);
        g_FieldLines[g_FieldLineCount].pos.x2 = val;
        val = SCRIPT_PARAM_U8(9);
        val |= (short)(SCRIPT_PARAM_U8(0xA) << 8);
        g_FieldLines[g_FieldLineCount].pos.y2 = val;
        val = SCRIPT_PARAM_U8(0xB);
        val |= (short)(SCRIPT_PARAM_U8(0xC) << 8);
        g_FieldLines[g_FieldLineCount].pos.z2 = val;
        g_FieldLines[g_FieldLineCount].isActive = 1;
        g_FieldLines[g_FieldLineCount].entityId = *(char *)&g_CurrentEntity;
        g_FieldLineCount++;
        g_FieldScriptPC[g_CurrentEntity] += 0xD;
        return 0;
    }
}

/*
 * SLINE: moves this entity's trigger line to new end points, six 16-bit operands v[4]/1, v[6]/2, v[8]/3
 * (first point) and v[10]/4, v[12]/5, v[14]/6 (second). 16 bytes. Assumes LINE ran first: the entity's
 * line index is used unchecked.
 *
 * Field opcode 0xD3 (FIELD_OP_SET_LINE_POINTS).
 */
// FUNCTION: FF7 0x006114d0
int __cdecl Script_OpSetLinePoints(void)
{
    short line;

    line = g_FieldEntityLine[g_CurrentEntity];
    g_FieldLines[line].pos.x1 = Script_ReadOperand16(1, 4);
    g_FieldLines[line].pos.y1 = Script_ReadOperand16(2, 6);
    g_FieldLines[line].pos.z1 = Script_ReadOperand16(3, 8);
    g_FieldLines[line].pos.x2 = Script_ReadOperand16(4, 10);
    g_FieldLines[line].pos.y2 = Script_ReadOperand16(5, 12);
    g_FieldLines[line].pos.z2 = Script_ReadOperand16(6, 14);
    g_FieldScriptPC[g_CurrentEntity] += 0x10;
    return 0;
}

/*
 * LINON: switches this entity's trigger line on ([1] nonzero) or off. Switching it off also clears its
 * touch state, so the line forgets that the player was on it. 2 bytes.
 *
 * The operand is fetched twice rather than kept in a local, and there is no frame at all, so the source
 * really does read the script byte again for the test.
 *
 * Field opcode 0xD1 (FIELD_OP_SET_LINE_ENABLED).
 */
// FUNCTION: FF7 0x006115ad
int __cdecl Script_OpSetLineEnabled(void)
{
    g_FieldLines[g_FieldEntityLine[g_CurrentEntity]].isActive =
        SCRIPT_PARAM_U8(1);
    if (SCRIPT_PARAM_U8(1) == 0) {
        g_FieldLines[g_FieldEntityLine[g_CurrentEntity]].touch = 0;
    }
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * SLIP (community mnemonic): stores [1] in the slipDisabled byte of this entity's trigger line. 2 bytes.
 *
 * g_FieldEntityLine (0x00CBF600) maps the running entity to its record in g_FieldLines (0x00CC1F70), an
 * array of 0x18-byte records, the size the PSX declares for `FieldLine`; slipDisabled is sixteen bytes
 * into the record (see the FieldLine comment in include/ff7/field.h).
 *
 * Field opcode 0x2B (FIELD_OP_SET_LINE_FLAG_16).
 */
// FUNCTION: FF7 0x00611647
int __cdecl Script_OpSetLineSlipDisabled(void)
{
    g_FieldLines[g_FieldEntityLine[g_CurrentEntity]].slipDisabled =
        SCRIPT_PARAM_U8(1);
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * IFUB: if byte v[2]/1 <op> byte v[3]/2 holds (op [4], see Script_EvalCompare8) the script goes on with
 * the next opcode; otherwise it skips forward [5] bytes, counted from that distance byte. 6 bytes.
 *
 * Field opcode 0x14 (FIELD_OP_IF_BYTE).
 */
// FUNCTION: FF7 0x006116a6
int __cdecl Script_OpIfByte(void)
{
    if (Script_EvalCompare8() != 0) {
        g_FieldScriptPC[g_CurrentEntity] += 6;
    } else {
        g_FieldScriptPC[g_CurrentEntity] += SCRIPT_PARAM_U8(5) + 5;
    }
    return 0;
}

/*
 * IFUBL: IFUB with a 16-bit skip distance at [5..6]. 7 bytes. The distance is a signed short, so one of
 * 0x8000 or more would jump backwards.
 *
 * Field opcode 0x15 (FIELD_OP_IF_BYTE_LONG).
 */
// FUNCTION: FF7 0x0061171f
int __cdecl Script_OpIfByteLong(void)
{
    short distance;

    if (Script_EvalCompare8() != 0) {
        g_FieldScriptPC[g_CurrentEntity] += 7;
    } else {
        distance = SCRIPT_PARAM_U8(5);
        distance |= SCRIPT_PARAM_U8(6) << 8;
        g_FieldScriptPC[g_CurrentEntity] += distance + 5;
    }
    return 0;
}

/*
 * The test of IFUB/IFUBL. Op [4] combines the bytes a = v[2]/1 and b = v[3]/2: 0 a == b, 1 a != b, 2 a >
 * b, 3 a < b, 4 a >= b, 5 a <= b, 6 a & b, 7 a ^ b, 8 a | b, 9 bit b of a set, 10 bit b of a clear. Any
 * other op is false. The bitwise results are kept as bytes, so any nonzero result counts as true.
 */
// FUNCTION: FF7 0x006117cb
int __cdecl Script_EvalCompare8(void)
{
    unsigned char result;
    unsigned char op;

    result = 0;
    op = SCRIPT_PARAM_U8(4);
    switch (op) {
    case FIELD_COMPARE_EQUAL:
        result = Script_ReadOperand8(1, 2) == Script_ReadOperand8(2, 3);
        break;
    case FIELD_COMPARE_NOT_EQUAL:
        result = Script_ReadOperand8(1, 2) != Script_ReadOperand8(2, 3);
        break;
    case FIELD_COMPARE_GREATER:
        result = Script_ReadOperand8(1, 2) > Script_ReadOperand8(2, 3);
        break;
    case FIELD_COMPARE_LESS:
        result = Script_ReadOperand8(1, 2) < Script_ReadOperand8(2, 3);
        break;
    case FIELD_COMPARE_GREATER_EQUAL:
        result = Script_ReadOperand8(1, 2) >= Script_ReadOperand8(2, 3);
        break;
    case FIELD_COMPARE_LESS_EQUAL:
        result = Script_ReadOperand8(1, 2) <= Script_ReadOperand8(2, 3);
        break;
    case FIELD_COMPARE_AND:
        result = Script_ReadOperand8(1, 2) & Script_ReadOperand8(2, 3);
        break;
    case FIELD_COMPARE_XOR:
        result = Script_ReadOperand8(1, 2) ^ Script_ReadOperand8(2, 3);
        break;
    case FIELD_COMPARE_OR:
        result = Script_ReadOperand8(1, 2) | Script_ReadOperand8(2, 3);
        break;
    case FIELD_COMPARE_BIT_SET:
        result = Script_ReadOperand8(1, 2) & (1 << Script_ReadOperand8(2, 3));
        break;
    case FIELD_COMPARE_BIT_CLEAR:
        result = Script_ReadOperand8(1, 2) & (1 << Script_ReadOperand8(2, 3));
        result = !result;
        break;
    }
    return result;
}

/*
 * IFSW: as IFUB on the signed words a = v[2]/1 and b = v[4]/2 (op [6], see Script_EvalCompare16Signed); a
 * false test skips forward [7] bytes, counted from that byte. 8 bytes.
 *
 * Field opcode 0x16 (FIELD_OP_IF_WORD_SIGNED).
 */
// FUNCTION: FF7 0x00611a89
int __cdecl Script_OpIfWordSigned(void)
{
    if (Script_EvalCompare16Signed() != 0) {
        g_FieldScriptPC[g_CurrentEntity] += 8;
    } else {
        g_FieldScriptPC[g_CurrentEntity] += SCRIPT_PARAM_U8(7) + 7;
    }
    return 0;
}

/*
 * IFSWL: IFSW with a 16-bit (signed) skip distance at [7..8]. 9 bytes.
 *
 * Field opcode 0x17 (FIELD_OP_IF_WORD_SIGNED_LONG).
 */
// FUNCTION: FF7 0x00611b02
int __cdecl Script_OpIfWordSignedLong(void)
{
    short distance;

    if (Script_EvalCompare16Signed() != 0) {
        g_FieldScriptPC[g_CurrentEntity] += 9;
    } else {
        distance = SCRIPT_PARAM_U8(7);
        distance |= SCRIPT_PARAM_U8(8) << 8;
        g_FieldScriptPC[g_CurrentEntity] += distance + 7;
    }
    return 0;
}

/*
 * The test of IFSW/IFSWL: the ops of Script_EvalCompare8 on signed words, op at [6].
 *
 * BUG: every result is narrowed to a byte before it is tested. The comparisons survive that, the bitwise
 * ops do not: a & b, a ^ b or a | b whose set bits are all in the high byte counts as false, and a test of
 * bits 8..15 is always false with op 9 and always true with op 10.
 */
// FUNCTION: FF7 0x00611bae
int __cdecl Script_EvalCompare16Signed(void)
{
    unsigned char result;
    unsigned char op;

    result = 0;
    op = SCRIPT_PARAM_S8(6);
    switch (op) {
    case FIELD_COMPARE_EQUAL:
        result = (short)Script_ReadOperand16(1, 2) == (short)Script_ReadOperand16(2, 4);
        break;
    case FIELD_COMPARE_NOT_EQUAL:
        result = (short)Script_ReadOperand16(1, 2) != (short)Script_ReadOperand16(2, 4);
        break;
    case FIELD_COMPARE_GREATER:
        result = (short)Script_ReadOperand16(1, 2) > (short)Script_ReadOperand16(2, 4);
        break;
    case FIELD_COMPARE_LESS:
        result = (short)Script_ReadOperand16(1, 2) < (short)Script_ReadOperand16(2, 4);
        break;
    case FIELD_COMPARE_GREATER_EQUAL:
        result = (short)Script_ReadOperand16(1, 2) >= (short)Script_ReadOperand16(2, 4);
        break;
    case FIELD_COMPARE_LESS_EQUAL:
        result = (short)Script_ReadOperand16(1, 2) <= (short)Script_ReadOperand16(2, 4);
        break;
    case FIELD_COMPARE_AND:
        result = (short)Script_ReadOperand16(1, 2) & (short)Script_ReadOperand16(2, 4);
        break;
    case FIELD_COMPARE_XOR:
        result = (short)Script_ReadOperand16(1, 2) ^ (short)Script_ReadOperand16(2, 4);
        break;
    case FIELD_COMPARE_OR:
        result = (short)Script_ReadOperand16(1, 2) | (short)Script_ReadOperand16(2, 4);
        break;
    case FIELD_COMPARE_BIT_SET:
        result = (short)Script_ReadOperand16(1, 2) & 1 << (short)Script_ReadOperand16(2, 4);
        break;
    case FIELD_COMPARE_BIT_CLEAR:
        result = (short)Script_ReadOperand16(1, 2) & 1 << (short)Script_ReadOperand16(2, 4);
        result = result == 0;
    }
    return result;
}

/*
 * IFUW: IFSW with the words compared unsigned (see Script_EvalCompare16Unsigned). 8 bytes.
 *
 * Field opcode 0x18 (FIELD_OP_IF_WORD_UNSIGNED).
 */
// FUNCTION: FF7 0x00611e1b
int __cdecl Script_OpIfWordUnsigned(void)
{
    if (Script_EvalCompare16Unsigned() != 0) {
        g_FieldScriptPC[g_CurrentEntity] += 8;
    } else {
        g_FieldScriptPC[g_CurrentEntity] += SCRIPT_PARAM_U8(7) + 7;
    }
    return 0;
}

/*
 * IFUWL: IFUW with a 16-bit skip distance at [7..8], read as a signed short. 9 bytes.
 *
 * Field opcode 0x19 (FIELD_OP_IF_WORD_UNSIGNED_LONG).
 */
// FUNCTION: FF7 0x00611e94
int __cdecl Script_OpIfWordUnsignedLong(void)
{
    short distance;

    if (Script_EvalCompare16Unsigned() != 0) {
        g_FieldScriptPC[g_CurrentEntity] += 9;
    } else {
        distance = SCRIPT_PARAM_U8(7);
        distance |= SCRIPT_PARAM_U8(8) << 8;
        g_FieldScriptPC[g_CurrentEntity] += distance + 7;
    }
    return 0;
}

/*
 * The test of IFUW/IFUWL: the ops of Script_EvalCompare8 on unsigned words, op at [6].
 *
 * BUG: as in Script_EvalCompare16Signed, the result is narrowed to a byte, so the bitwise ops see only the
 * low eight bits of their result: high-byte-only results count as false, and bit tests of bits 8..15 are
 * always false (op 9) or always true (op 10).
 */
// FUNCTION: FF7 0x00611f40
int __cdecl Script_EvalCompare16Unsigned(void)
{
    unsigned char result;
    unsigned char op;

    result = 0;
    op = SCRIPT_PARAM_S8(6);
    switch (op) {
    case FIELD_COMPARE_EQUAL:
        result = ((unsigned short)Script_ReadOperand16(1, 2)) == ((unsigned short)Script_ReadOperand16(2, 4));
        break;
    case FIELD_COMPARE_NOT_EQUAL:
        result = ((unsigned short)Script_ReadOperand16(1, 2)) != ((unsigned short)Script_ReadOperand16(2, 4));
        break;
    case FIELD_COMPARE_GREATER:
        result = ((unsigned short)Script_ReadOperand16(1, 2)) > ((unsigned short)Script_ReadOperand16(2, 4));
        break;
    case FIELD_COMPARE_LESS:
        result = ((unsigned short)Script_ReadOperand16(1, 2)) < ((unsigned short)Script_ReadOperand16(2, 4));
        break;
    case FIELD_COMPARE_GREATER_EQUAL:
        result = ((unsigned short)Script_ReadOperand16(1, 2)) >= ((unsigned short)Script_ReadOperand16(2, 4));
        break;
    case FIELD_COMPARE_LESS_EQUAL:
        result = ((unsigned short)Script_ReadOperand16(1, 2)) <= ((unsigned short)Script_ReadOperand16(2, 4));
        break;
    case FIELD_COMPARE_AND:
        result = (unsigned short)Script_ReadOperand16(1, 2) & ((unsigned short)Script_ReadOperand16(2, 4));
        break;
    case FIELD_COMPARE_XOR:
        result = (unsigned short)Script_ReadOperand16(1, 2) ^ (unsigned short)Script_ReadOperand16(2, 4);
        break;
    case FIELD_COMPARE_OR:
        result = (unsigned short)Script_ReadOperand16(1, 2) | (unsigned short)Script_ReadOperand16(2, 4);
        break;
    case FIELD_COMPARE_BIT_SET:
        result = (unsigned short)Script_ReadOperand16(1, 2) & 1 << ((unsigned short)Script_ReadOperand16(2, 4));
        break;
    case FIELD_COMPARE_BIT_CLEAR:
        result = (unsigned short)Script_ReadOperand16(1, 2) & 1 << ((unsigned short)Script_ReadOperand16(2, 4));
        result = result == 0;
    }
    return result;
}

/*
 * IFKEY: if any button of the 16-bit mask [1..2] is held, the script goes on with the next opcode;
 * otherwise it skips forward [3] bytes, counted from that byte. 4 bytes. With mask bit 9 (bit 1 of [2])
 * set the test uses the second set of key latches (activeKeys2) instead of the first.
 *
 * Field opcode 0x30 (FIELD_OP_IF_BUTTON_HELD).
 */
// FUNCTION: FF7 0x0061220d
int __cdecl Script_OpIfButtonHeld(void)
{
    if ((SCRIPT_PARAM_U8(2) & 2) != 0) {
        return Script_BranchOnButtonMask(g_pFieldState->activeKeys2);
    } else {
        return Script_BranchOnButtonMask(g_pFieldState->activeKeys);
    }
}

/*
 * IFKEYON: IFKEY on the buttons that went down this frame. 4 bytes.
 *
 * The second of the trio that starts at Script_OpIfButtonHeld: same shape, a different pair of key masks
 * in FieldState.
 *
 * Field opcode 0x31 (FIELD_OP_IF_BUTTON_PRESSED).
 */
// FUNCTION: FF7 0x0061225d
int __cdecl Script_OpIfButtonPressed(void)
{
    if ((SCRIPT_PARAM_U8(2) & 2) != 0) {
        return Script_BranchOnButtonMask(g_pFieldState->newActiveKeys2);
    } else {
        return Script_BranchOnButtonMask(g_pFieldState->newActiveKeys);
    }
}

/*
 * IFKEYOFF: IFKEY on the buttons that were released this frame. 4 bytes.
 *
 * The third of the trio that starts at Script_OpIfButtonHeld: same shape, a different pair of key masks in
 * FieldState.
 *
 * Field opcode 0x32 (FIELD_OP_IF_BUTTON_RELEASED).
 */
// FUNCTION: FF7 0x006122b0
int __cdecl Script_OpIfButtonReleased(void)
{
    if ((SCRIPT_PARAM_U8(2) & 2) != 0) {
        return Script_BranchOnButtonMask(g_pFieldState->newInactiveKeys2);
    } else {
        return Script_BranchOnButtonMask(g_pFieldState->newInactiveKeys);
    }
}

/*
 * The branch of IFKEY, IFKEYON and IFKEYOFF: any one button of the mask [1..2] in the given key state is
 * enough (not all of them) for the next opcode to run; otherwise the script skips forward [3] bytes from
 * that byte.
 */
// FUNCTION: FF7 0x00612303
int __cdecl Script_BranchOnButtonMask(unsigned short mask)
{
    unsigned short want = 0;
    int junk;

    want = SCRIPT_PARAM_U8(1);
    want |= SCRIPT_PARAM_U8(2) << 8;
    if ((mask & want) != 0) {
        g_FieldScriptPC[g_CurrentEntity] += 4;
    } else {
        g_FieldScriptPC[g_CurrentEntity] += SCRIPT_PARAM_U8(3) + 3;
    }
    return 0;
}

/*
 * A script call's packed byte (REQ and its forms, RETTO): the priority in the top three bits, the
 * entity's script number in the low five.
 */
#define SCRIPT_REQ_PRIORITY(packed) (((packed) >> 5) & 7)
#define SCRIPT_REQ_SCRIPT(packed) ((packed) & 0x1f)

/*
 * REQ: asks entity [1] to run its script [2] & 0x1F (SCRIPT_REQ_SCRIPT) at priority [2] >> 5
 * (SCRIPT_REQ_PRIORITY; 0 the most urgent, 7 the entity's main loop) and carries on without waiting for
 * it. 3 bytes. The rules for a busy entity are in Script_RequestEntityScript.
 *
 * Field opcode 0x01 (FIELD_OP_REQ).
 */
// FUNCTION: FF7 0x006123e2
int __cdecl Script_OpReq(void)
{
    unsigned char entity;
    unsigned char script;
    unsigned char priority;

    entity = SCRIPT_PARAM_U8(1);
    priority = SCRIPT_REQ_PRIORITY(SCRIPT_PARAM_U8(2));
    script = SCRIPT_REQ_SCRIPT(SCRIPT_PARAM_U8(2));
    return Script_RequestEntityScript(FIELD_SCRIPT_REQUEST_ASYNC, entity, priority,
                                      script);
}

/*
 * REQSW: REQ that waits until the target has taken the request: while the target is busy at that priority,
 * or another script is already queued there, the opcode retries every frame. 3 bytes.
 *
 * Field opcode 0x02 (FIELD_OP_REQ_SYNC_WAIT).
 */
// FUNCTION: FF7 0x0061246a
int __cdecl Script_OpReqSyncWait(void)
{
    unsigned char entity;
    unsigned char script;
    unsigned char priority;

    entity = SCRIPT_PARAM_U8(1);
    priority = SCRIPT_REQ_PRIORITY(SCRIPT_PARAM_U8(2));
    script = SCRIPT_REQ_SCRIPT(SCRIPT_PARAM_U8(2));
    return Script_RequestEntityScript(FIELD_SCRIPT_REQUEST_WAIT_START, entity, priority,
                                      script);
}

/*
 * REQEW: REQ that waits until the requested script has run to its RET. 3 bytes.
 *
 * Field opcode 0x03 (FIELD_OP_REQ_END_WAIT).
 */
// FUNCTION: FF7 0x006124f2
int __cdecl Script_OpReqEndWait(void)
{
    unsigned char entity;
    unsigned char script;
    unsigned char priority;

    entity = SCRIPT_PARAM_U8(1);
    priority = SCRIPT_REQ_PRIORITY(SCRIPT_PARAM_U8(2));
    script = SCRIPT_REQ_SCRIPT(SCRIPT_PARAM_U8(2));
    return Script_RequestEntityScript(FIELD_SCRIPT_REQUEST_WAIT_END, entity, priority,
                                      script);
}

/*
 * PREQ: REQ addressed by party slot [1] instead of entity number: the request goes to the field entity of
 * the character in that slot. An empty slot, or a character with no entity on this map, skips the opcode.
 * 3 bytes.
 *
 * Field opcode 0x04 (FIELD_OP_PARTY_REQ).
 */
// FUNCTION: FF7 0x0061257a
int __cdecl Script_OpPartyReq(void)
{
    unsigned char entity;
    unsigned char member;
    unsigned char scr;
    unsigned char lvl;

    member = gSavemap.fieldParty[SCRIPT_PARAM_U8(1)];
    if (member == 0xff) {
        entity = 0xff;
    } else {
        entity = g_FieldCharEntity.charEntity[member];
    }
    lvl = SCRIPT_REQ_PRIORITY(SCRIPT_PARAM_U8(2));
    scr = SCRIPT_REQ_SCRIPT(SCRIPT_PARAM_U8(2));
    return Script_RequestEntityScript(FIELD_SCRIPT_REQUEST_ASYNC, entity, lvl, scr);
}

/*
 * PRQSW: REQSW addressed by party slot [1]. 3 bytes.
 *
 * Field opcode 0x05 (FIELD_OP_PARTY_REQ_SYNC_WAIT).
 */
// FUNCTION: FF7 0x00612632
int __cdecl Script_OpPartyReqSyncWait(void)
{
    unsigned char entity;
    unsigned char member;
    unsigned char scr;
    unsigned char lvl;

    member = gSavemap.fieldParty[SCRIPT_PARAM_U8(1)];
    if (member == 0xff) {
        entity = 0xff;
    } else {
        entity = g_FieldCharEntity.charEntity[member];
    }
    lvl = SCRIPT_REQ_PRIORITY(SCRIPT_PARAM_U8(2));
    scr = SCRIPT_REQ_SCRIPT(SCRIPT_PARAM_U8(2));
    return Script_RequestEntityScript(FIELD_SCRIPT_REQUEST_WAIT_START, entity, lvl, scr);
}

/*
 * PRQEW: REQEW addressed by party slot [1]. 3 bytes.
 *
 * Field opcode 0x06 (FIELD_OP_PARTY_REQ_END_WAIT).
 */
// FUNCTION: FF7 0x006126ea
int __cdecl Script_OpPartyReqEndWait(void)
{
    unsigned char entity;
    unsigned char member;
    unsigned char scr;
    unsigned char lvl;

    member = gSavemap.fieldParty[SCRIPT_PARAM_U8(1)];
    if (member == 0xff) {
        entity = 0xff;
    } else {
        entity = g_FieldCharEntity.charEntity[member];
    }
    lvl = SCRIPT_REQ_PRIORITY(SCRIPT_PARAM_U8(2));
    scr = SCRIPT_REQ_SCRIPT(SCRIPT_PARAM_U8(2));
    return Script_RequestEntityScript(FIELD_SCRIPT_REQUEST_WAIT_END, entity, lvl, scr);
}

/*
 * The script call behind REQ, REQSW, REQEW and their party forms. Each entity has eight priority slots, 0
 * the most urgent and 7 its main loop; the slot it is running is g_FieldScriptPriority, the others can
 * hold a saved pc to go back to. Script entry points come from the field's script table
 * (FIELD_SCRIPT_ENTRY_LO/HI).
 *
 * - No such entity (0xFF): the caller skips the opcode.
 * - REQEW run again by its registered caller: waits while the script runs, and completes once RET has
 *   marked it returned.
 * - The target already runs at that priority: REQ drops the request, REQSW and REQEW wait.
 * - The target runs something more urgent: the script is queued in the priority's slot and starts when the
 *   target returns down to it. REQ and REQSW carry on, REQEW registers itself as the caller and waits; a
 *   slot that is already taken makes REQ drop the request and the others wait.
 * - The request is more urgent than what the target runs: the target's pc is saved in its current slot and
 *   it jumps to the new script at once, with any scripted move and WAIT it was in cancelled. While an
 *   earlier call at that priority is still pending, every mode waits instead.
 *
 * Remaining busy or unsupported-mode paths return one, so the opcode runs again next frame.
 */
// FUNCTION: FF7 0x006127a2
int __cdecl Script_RequestEntityScript(short mode, unsigned char entity,
                                       unsigned char priority,
                                       unsigned char script)
{
    short val;

    if (entity == 0xFF) {
        g_FieldScriptPC[g_CurrentEntity] += 3;
        return 0;
    }
    switch (mode) {
    case FIELD_SCRIPT_REQUEST_ASYNC:
        break;
    case FIELD_SCRIPT_REQUEST_WAIT_END:
        if (g_FieldCharEntity.scriptCaller[entity][priority] == g_CurrentEntity) {
            switch (g_FieldScriptPriorityState[entity][priority]) {
            case FIELD_SCRIPT_REQUEST_STATE_WAITING:
                return 1;
            case FIELD_SCRIPT_REQUEST_STATE_RETURNED:
                g_FieldScriptPriorityState[entity][priority] = FIELD_SCRIPT_REQUEST_STATE_IDLE;
                g_FieldCharEntity.scriptCaller[entity][priority] = 0xFF;
                g_FieldScriptPC[g_CurrentEntity] += 3;
                return 0;
            }
        }
    }
    // The target already runs at this priority.
    if (g_FieldScriptPriority[entity] == priority) {
        switch (mode) {
        case FIELD_SCRIPT_REQUEST_ASYNC:
            g_FieldScriptPC[g_CurrentEntity] += 3;
            return 0;
        case FIELD_SCRIPT_REQUEST_WAIT_START:
        case FIELD_SCRIPT_REQUEST_WAIT_END:
            return 1;
        }
    } else if (g_FieldScriptPriority[entity] < priority) {
        // The target runs something more urgent: queue the script in this priority's slot.
        if (g_FieldScriptPriorityPC[entity][priority] != 0) {
            switch (mode) {
            case FIELD_SCRIPT_REQUEST_ASYNC:
                g_FieldScriptPC[g_CurrentEntity] += 3;
                return 0;
            case FIELD_SCRIPT_REQUEST_WAIT_START:
            case FIELD_SCRIPT_REQUEST_WAIT_END:
                return 1;
            }
        } else {
            val = g_FieldScripts->akaoBlockCount << 2;
            g_FieldScriptPriorityPC[entity][priority] = (unsigned short)FIELD_SCRIPT_ENTRY_LO(entity, script, val);
            g_FieldScriptPriorityPC[entity][priority] |= FIELD_SCRIPT_ENTRY_HI(entity, script, val) << 8;
            switch (mode) {
            case FIELD_SCRIPT_REQUEST_ASYNC:
            case FIELD_SCRIPT_REQUEST_WAIT_START:
                g_FieldScriptPC[g_CurrentEntity] += 3;
                return 0;
            case FIELD_SCRIPT_REQUEST_WAIT_END:
                g_FieldCharEntity.scriptCaller[entity][priority] = g_CurrentEntity;
                g_FieldScriptPriorityState[entity][priority] = FIELD_SCRIPT_REQUEST_STATE_WAITING;
                return 1;
            }
        }
    } else if (g_FieldScriptPriorityState[entity][priority] == FIELD_SCRIPT_REQUEST_STATE_IDLE) {
        // More urgent than what the target runs: switch it to the new script now.
        g_FieldEntityScriptTrack[entity * 8 + priority] = script;
        g_FieldScriptPriorityPC[entity][g_FieldScriptPriority[entity]] =
            g_FieldScriptPC[entity];
        val = g_FieldScripts->akaoBlockCount << 2;
        g_FieldScriptPC[entity] = (unsigned short)FIELD_SCRIPT_ENTRY_LO(entity, script, val);
        g_FieldScriptPC[entity] |= FIELD_SCRIPT_ENTRY_HI(entity, script, val) << 8;
        g_FieldScriptPriority[entity] = priority;
        if (g_FieldEntityModel[entity] != 0xFF) {
            g_pFieldEntities[g_FieldEntityModel[entity]].scriptedMoveMode = FIELD_SCRIPTED_MOVE_NONE;
        }
        g_FieldScriptWait[entity] = 0;
        switch (mode) {
        case FIELD_SCRIPT_REQUEST_ASYNC:
        case FIELD_SCRIPT_REQUEST_WAIT_START:
            g_FieldScriptPC[g_CurrentEntity] += 3;
            return 0;
        case FIELD_SCRIPT_REQUEST_WAIT_END:
            g_FieldCharEntity.scriptCaller[entity][priority] = g_CurrentEntity;
            g_FieldScriptPriorityState[entity][priority] = FIELD_SCRIPT_REQUEST_STATE_WAITING;
            return 1;
        }
    }
    return 1;
}

/*
 * RET: ends the running script. A REQEW caller waiting on it is told that it returned, and the entity goes
 * back to the most urgent script still saved in a less urgent slot, up to the main loop (priority 7),
 * whose saved pc it takes even when that is 0. 1 byte.
 *
 * RET in the main loop itself yields without moving the pc: the entity's main script simply stops there.
 *
 * Field opcode 0x00 (FIELD_OP_RETURN).
 */
// FUNCTION: FF7 0x00612d3c
int __cdecl Script_OpReturn(void)
{
    // RET in the main loop: the entity stops here.
    if (g_FieldScriptPriority[g_CurrentEntity] >= FIELD_SCRIPT_PRIORITY_MAIN) {
        return 1;
    }
    if (g_FieldScriptPriorityState[g_CurrentEntity]
                                  [g_FieldScriptPriority[g_CurrentEntity]] == FIELD_SCRIPT_REQUEST_STATE_WAITING) {
        g_FieldScriptPriorityState[g_CurrentEntity]
                                  [g_FieldScriptPriority[g_CurrentEntity]] = FIELD_SCRIPT_REQUEST_STATE_RETURNED;
    }
    do {
        g_FieldScriptPriority[g_CurrentEntity] += 1;
        g_FieldScriptPC[g_CurrentEntity] =
            g_FieldScriptPriorityPC[g_CurrentEntity]
                                   [g_FieldScriptPriority[g_CurrentEntity]];
        if (g_FieldScriptPC[g_CurrentEntity] != 0) {
            break;
        }
    } while (g_FieldScriptPriority[g_CurrentEntity] < FIELD_SCRIPT_PRIORITY_MAIN);
    g_FieldScriptPriorityPC[g_CurrentEntity]
                           [g_FieldScriptPriority[g_CurrentEntity]] = 0;
    return 0;
}

/*
 * RETTO: leaves the running script for script [1] & 0x1F of this entity at priority [1] >> 5, dropping the
 * saved scripts in between (REQEW callers waiting on them are told they returned). Nothing comes back to
 * the abandoned scripts. 2 bytes, then the jump.
 *
 * Native no-argument __cdecl contract; returns zero after restoring priority/script state. The original
 * script-PC update reads the low byte, then ORs the high byte in a separate expression. Records the script
 * in the entity's eight-byte row of g_FieldEntityScriptTrack, at the priority.
 *
 * Field opcode 0x07 (FIELD_OP_RETURN_TO).
 */
// FUNCTION: FF7 0x00612e47
int __cdecl Script_OpReturnTo(void)
{
    short offset;
    short script;
    short priority;

    priority = SCRIPT_REQ_PRIORITY(SCRIPT_PARAM_U8(1));
    script = SCRIPT_REQ_SCRIPT(SCRIPT_PARAM_U8(1));
    while (g_FieldScriptPriority[g_CurrentEntity] < priority - 1 &&
           g_FieldScriptPriority[g_CurrentEntity] < FIELD_SCRIPT_PRIORITY_MAIN) {
        if (g_FieldScriptPriorityState[g_CurrentEntity][g_FieldScriptPriority[g_CurrentEntity]] == FIELD_SCRIPT_REQUEST_STATE_WAITING)
            g_FieldScriptPriorityState[g_CurrentEntity][g_FieldScriptPriority[g_CurrentEntity]] = FIELD_SCRIPT_REQUEST_STATE_RETURNED;
        g_FieldScriptPriority[g_CurrentEntity] += 1;
        g_FieldScriptPriorityPC[g_CurrentEntity][g_FieldScriptPriority[g_CurrentEntity]] = 0;
    }
    g_FieldEntityScriptTrack[g_CurrentEntity * 8 + priority] = script;
    offset = g_FieldScripts->akaoBlockCount << 2;
    g_FieldScriptPC[g_CurrentEntity] = (unsigned short)FIELD_SCRIPT_ENTRY_LO(g_CurrentEntity, script, offset);
    g_FieldScriptPC[g_CurrentEntity] |= FIELD_SCRIPT_ENTRY_HI(g_CurrentEntity, script, offset) << 8;
    g_FieldScriptPriority[g_CurrentEntity] = priority;
    return 0;
}

/*
 * JMPB: jumps back [1] bytes, counted from the opcode, and yields. Every backward jump ends the entity's
 * turn for the frame, which is what stops a script loop from freezing the game.
 *
 * Field opcode 0x12 (FIELD_OP_JUMP_BACK).
 */
// FUNCTION: FF7 0x00613030
int __cdecl Script_OpJumpBack(void)
{
    g_FieldScriptPC[g_CurrentEntity] -= SCRIPT_PARAM_U8(1);
    return 1;
}

/*
 * JMPBL: JMPB with a 16-bit distance [1..2], low byte first. `sub cx, [ebp-4]` is 16-bit, so the source
 * used `-=`.
 *
 * Field opcode 0x13 (FIELD_OP_JUMP_BACK_LONG).
 */
// FUNCTION: FF7 0x0061307a
int __cdecl Script_OpJumpBackLong(void)
{
    short distance;

    distance = SCRIPT_PARAM_U8(1);
    distance |= SCRIPT_PARAM_U8(2) << 8;
    g_FieldScriptPC[g_CurrentEntity] -= distance;
    return 1;
}

/*
 * JMPF: jumps forward [1] bytes, counted from the distance byte, and carries on in the same frame.
 *
 * Not the mirror of Script_OpJumpBack: it skips the operand byte as well as the distance, and returns 0
 * where the backward jump returns 1.
 *
 * Field opcode 0x10 (FIELD_OP_JUMP_FORWARD).
 */
// FUNCTION: FF7 0x006130f6
int __cdecl Script_OpJumpForward(void)
{
    g_FieldScriptPC[g_CurrentEntity] += SCRIPT_PARAM_U8(1) + 1;
    return 0;
}

/*
 * JMPFL: JMPF with a 16-bit distance [1..2], counted from [1].
 *
 * Not the mirror of the backward form, exactly as the short pair is not: the distance is taken UNSIGNED
 * here and one more byte is skipped. `mov ecx, dword ptr [ebp-4]; and ecx, 0xffff` on a two-byte local is
 * the cast, not a mask the source wrote as `& 0xffff`.
 *
 * Field opcode 0x11 (FIELD_OP_JUMP_FORWARD_LONG).
 */
// FUNCTION: FF7 0x00613141
int __cdecl Script_OpJumpForwardLong(void)
{
    short distance;

    distance = SCRIPT_PARAM_U8(1);
    distance |= SCRIPT_PARAM_U8(2) << 8;
    g_FieldScriptPC[g_CurrentEntity] += (unsigned short)distance + 1;
    return 0;
}

/*
 * MAPJUMP: leaves for another field map. Literal operands: the map id [1..2], and where the player appears
 * there: x [3..4], y [5..6], walkmesh triangle [7..8] and facing [9]. 10 bytes.
 *
 * A field request (FIELD_REQUEST_MAP_JUMP): the first run posts it and every later run waits until the
 * engine reports it done, so the pc only moves past the opcode once the jump has happened. A jump to map
 * 0x313 is sent to map 0x159 instead.
 *
 * Field opcode 0x60 (FIELD_OP_MAP_JUMP).
 */
// FUNCTION: FF7 0x006131c4
int __cdecl Script_OpMapJump(void)
{
    switch ((char)g_pFieldState->requestCode) {
    case FIELD_REQUEST_NONE:
        g_pFieldState->requestCode = FIELD_REQUEST_MAP_JUMP;
        g_pFieldState->requestArg = FIELD_REQUEST_PROGRESS_PENDING;
        g_pFieldState->eventCmdParam = SCRIPT_PARAM_U8(1);
        g_pFieldState->eventCmdParam |= (short)(SCRIPT_PARAM_U8(2) << 8);
        g_pFieldState->pcTileX = SCRIPT_PARAM_U8(3);
        g_pFieldState->pcTileX |= (short)(SCRIPT_PARAM_U8(4) << 8);
        g_pFieldState->pcTileY = SCRIPT_PARAM_U8(5);
        g_pFieldState->pcTileY |= (short)(SCRIPT_PARAM_U8(6) << 8);
        g_pFieldState->pcWalkMeshId = SCRIPT_PARAM_U8(7);
        g_pFieldState->pcWalkMeshId |= (short)(SCRIPT_PARAM_U8(8) << 8);
        *(short *)&g_pFieldState->pcDirection = SCRIPT_PARAM_U8(9);
        // A jump to map 0x313 lands on map 0x159.
        if (g_pFieldState->eventCmdParam == 0x313) {
            g_pFieldState->eventCmdParam = 0x159;
        }
        return 1;
    case FIELD_REQUEST_MAP_JUMP:
        if (g_pFieldState->requestArg == FIELD_REQUEST_PROGRESS_DONE) {
            g_FieldScriptPC[g_CurrentEntity] += 0xA;
            g_pFieldState->requestCode = FIELD_REQUEST_NONE;
            return 0;
        }
        break;
    }
    return 1;
}

/*
 * PMJMP: names the map, literal word [1..2], that the engine may start loading in the background ahead of
 * the MAPJUMP to it. 3 bytes.
 *
 * Field opcode 0xD8 (FIELD_OP_SET_PRELOAD_MAP_ID).
 */
// FUNCTION: FF7 0x00613405
int __cdecl Script_OpSetPreloadMapId(void)
{
    g_FieldPreloadMapId = SCRIPT_PARAM_U8(1);
    g_FieldPreloadMapId |= SCRIPT_PARAM_U8(2) << 8;
    g_FieldScriptPC[g_CurrentEntity] += 3;
    return 0;
}

/*
 * PMJMP2: waits until the background map load PMJMP started has finished (g_FieldPreloadState 2). 1 byte.
 *
 * Field opcode 0xD9 (FIELD_OP_WAIT_PRELOAD_DONE).
 */
// FUNCTION: FF7 0x00613483
int __cdecl Script_OpWaitPreloadDone(void)
{
    if (g_FieldPreloadState == 2) {
        g_FieldScriptPC[g_CurrentEntity] += 1;
        return 0;
    } else {
        return 1;
    }
}

/*
 * MINIGAME: leaves the field for a minigame. Literal operands: a map id [1..2] with x [3..4], y [5..6],
 * triangle [7..8] and facing [9] (where the party comes back, by the community reference), and the
 * minigame number [10]. 11 bytes.
 *
 * A field request (FIELD_REQUEST_MINIGAME): posted on the first run, then the opcode waits until the
 * engine reports it done.
 *
 * Field opcode 0x20 (FIELD_OP_MINIGAME).
 */
// FUNCTION: FF7 0x006134c0
int __cdecl Script_OpMinigame(void)
{
    switch ((char)g_pFieldState->requestCode) {
    case FIELD_REQUEST_NONE:
        g_pFieldState->requestCode = FIELD_REQUEST_MINIGAME;
        g_pFieldState->requestArg = FIELD_REQUEST_PROGRESS_PENDING;
        g_pFieldState->eventCmdParam = SCRIPT_PARAM_U8(1);
        g_pFieldState->eventCmdParam |= (short)(SCRIPT_PARAM_U8(2) << 8);
        g_pFieldState->pcTileX = SCRIPT_PARAM_U8(3);
        g_pFieldState->pcTileX |= (short)(SCRIPT_PARAM_U8(4) << 8);
        g_pFieldState->pcTileY = SCRIPT_PARAM_U8(5);
        g_pFieldState->pcTileY |= (short)(SCRIPT_PARAM_U8(6) << 8);
        g_pFieldState->pcWalkMeshId = SCRIPT_PARAM_U8(7);
        g_pFieldState->pcWalkMeshId |= (short)(SCRIPT_PARAM_U8(8) << 8);
        *(short *)&g_pFieldState->pcDirection = SCRIPT_PARAM_U8(9);
        g_pFieldState->minigameKind = SCRIPT_PARAM_S8(0xA);
        return 1;
    case FIELD_REQUEST_MINIGAME:
        if (g_pFieldState->requestArg == FIELD_REQUEST_PROGRESS_DONE) {
            g_FieldScriptPC[g_CurrentEntity] += 0xB;
            g_pFieldState->requestCode = FIELD_REQUEST_NONE;
            return 0;
        }
        break;
    }
    return 1;
}

/*
 * BATTLE: starts the battle with the formation v[2]/2 (16-bit) and waits until it is over. 4 bytes. The
 * flags BTLMD or BTMD2 set beforehand say how it starts.
 *
 * A field request (FIELD_REQUEST_BATTLE): the first run resets the message windows and posts it; every
 * later run waits until the engine reports it done.
 *
 * Field opcode 0x70 (FIELD_OP_ENTER_BATTLE).
 */
// FUNCTION: FF7 0x0061370a
int __cdecl Script_OpEnterBattle(void)
{
    switch (g_pFieldState->requestCode) {
    case FIELD_REQUEST_NONE:
        Field_ResetMessageWindowState();
        g_pFieldState->requestCode = FIELD_REQUEST_BATTLE;
        g_pFieldState->eventCmdParam = Script_ReadOperand16(2, 2);
        g_pFieldState->requestArg = FIELD_REQUEST_PROGRESS_PENDING;
        g_FieldRequestPosted = 1;
        return 1;
    case FIELD_REQUEST_BATTLE:
        if (g_pFieldState->requestArg == FIELD_REQUEST_PROGRESS_DONE) {
            g_FieldScriptPC[g_CurrentEntity] += 4;
            g_pFieldState->requestCode = FIELD_REQUEST_NONE;
            g_pFieldState->requestArg = FIELD_REQUEST_PROGRESS_PENDING;
            return 0;
        }
        break;
    }
    return 1;
}

/*
 * Clears the sound request block (g_pFieldRequest) that the AKAO, SOUND and MUSIC opcodes fill before they
 * call SoundDispatchCommand: the command and its six arguments go to 0.
 */
// FUNCTION: FF7 0x006137bd
void __cdecl Field_ClearRequest(void)
{
    int i;

    g_pFieldRequest->code = 0;
    for (i = 0; i < 6; i++) {
        g_pFieldRequest->args[i] = 0;
    }
}

/*
 * AKAO: sends one raw command to the sound driver: command [4] (literal) with the arguments v[5]/1 (byte),
 * v[6]/2, v[8]/3, v[10]/4 and v[12]/6 (words). 14 bytes. The commands are the sound driver's own
 * (lib/enums/sound.h): music, sound effects, volume and tempo changes.
 *
 * Field opcode 0xF2 (FIELD_OP_SOUND_COMMAND).
 */
// FUNCTION: FF7 0x006137f9
int __cdecl Script_OpSoundCommand(void)
{
    short code;

    Field_ClearRequest();
    code = SCRIPT_PARAM_U8(4);
    g_pFieldRequest->code = code;
    g_pFieldRequest->args[0] = Script_ReadOperand8(1, 5);
    g_pFieldRequest->args[1] = Script_ReadOperand16(2, 6);
    g_pFieldRequest->args[2] = Script_ReadOperand16(3, 8);
    g_pFieldRequest->args[3] = Script_ReadOperand16(4, 0xa);
    g_pFieldRequest->args[4] = Script_ReadOperand16(6, 0xc);
    SoundDispatchCommand(code, g_pFieldRequest->args[0],
                         g_pFieldRequest->args[1],
                         g_pFieldRequest->args[2],
                         g_pFieldRequest->args[3],
                         g_pFieldRequest->args[4], 0, 0, 0);
    g_FieldScriptPC[g_CurrentEntity] += 0xe;
    return 0;
}

/*
 * AKAO2: AKAO with five word arguments, v[5]/1, v[7]/2, v[9]/3, v[11]/4 and v[13]/6. 15 bytes.
 *
 * Field opcode 0xDA (FIELD_OP_SOUND_COMMAND_LONG).
 */
// FUNCTION: FF7 0x00613914
int __cdecl Script_OpSoundCommandLong(void)
{
    short code;

    Field_ClearRequest();
    code = SCRIPT_PARAM_U8(4);
    g_pFieldRequest->code = code;
    g_pFieldRequest->args[0] = Script_ReadOperand16(1, 5);
    g_pFieldRequest->args[1] = Script_ReadOperand16(2, 7);
    g_pFieldRequest->args[2] = Script_ReadOperand16(3, 9);
    g_pFieldRequest->args[3] = Script_ReadOperand16(4, 0xb);
    g_pFieldRequest->args[4] = Script_ReadOperand16(6, 0xd);
    SoundDispatchCommand(code, g_pFieldRequest->args[0],
                         g_pFieldRequest->args[1],
                         g_pFieldRequest->args[2],
                         g_pFieldRequest->args[3],
                         g_pFieldRequest->args[4], 0, 0, 0);
    g_FieldScriptPC[g_CurrentEntity] += 0xf;
    return 0;
}

/*
 * SOUND: plays sound effect v[2]/1 (16-bit) at the stereo pan v[4]/2 (byte). 5 bytes.
 *
 * Field opcode 0xF1 (FIELD_OP_SOUND_EFFECT).
 */
// FUNCTION: FF7 0x00613a2d
int __cdecl Script_OpSoundEffect(void)
{
    Field_ClearRequest();
    g_pFieldRequest->code = SOUND_SFX_PLAY_1;
    g_pFieldRequest->args[0] = Script_ReadOperand8(2, 4);
    g_pFieldRequest->args[1] = Script_ReadOperand16(1, 2);
    SoundDispatchCommand(g_pFieldRequest->code, g_pFieldRequest->args[0],
                         g_pFieldRequest->args[1], 0, 0, 0, 0, 0, 0);
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * MUSIC: plays the track of this field's AKAO block [1] (SOUND_MUSIC_PLAY) and makes it the field's music.
 * 2 bytes. Nothing plays while MULCK has locked the music; see Field_SubmitMusicRequest.
 *
 * Declared void: the opcode table calls it as an int handler, and EAX still holds the 0 of
 * Field_SubmitMusicRequest.
 *
 * Field opcode 0xF0 (FIELD_OP_SUBMIT_MUSIC_REQUEST_10).
 */
// FUNCTION: FF7 0x00613ac8
void __cdecl Script_OpSubmitMusicRequest10(void)
{
    Field_ClearRequest();
    g_pFieldRequest->code = SOUND_MUSIC_PLAY;
    Field_SubmitMusicRequest();
}

/*
 * MUSVT (community mnemonic): MUSIC with sound command 0x14 (SOUND_MUSIC_PLAY_ALT), which the PC sound
 * driver handles as 0x10 and the engine also uses to start the battle music. 2 bytes. Declared void, as
 * Script_OpSubmitMusicRequest10.
 *
 * Field opcode 0xF3 (FIELD_OP_SUBMIT_MUSIC_REQUEST_14).
 */
// FUNCTION: FF7 0x00613ae1
void __cdecl Script_OpSubmitMusicRequest14(void)
{
    Field_ClearRequest();
    g_pFieldRequest->code = SOUND_MUSIC_PLAY_ALT;
    Field_SubmitMusicRequest();
}

/*
 * MUSVM (community mnemonic): MUSIC with sound command 0x15 (SOUND_SIDE_SWITCH), which has no effect on
 * PC; the track still becomes the field's music. 2 bytes. Declared void, as Script_OpSubmitMusicRequest10.
 *
 * Field opcode 0xF4 (FIELD_OP_SUBMIT_MUSIC_REQUEST_15).
 */
// FUNCTION: FF7 0x00613afa
void __cdecl Script_OpSubmitMusicRequest15(void)
{
    Field_ClearRequest();
    g_pFieldRequest->code = SOUND_SIDE_SWITCH;
    Field_SubmitMusicRequest();
}

/*
 * CMUSC: MUSIC with a sound command of the script's choosing, [3] (literal), and two more word arguments
 * v[4]/3 and v[6]/4. 8 bytes in all: Field_SubmitMusicRequest steps over two and this handler over six
 * more. Returns the result of Field_SubmitMusicRequest, which is always 0.
 *
 * Field opcode 0xFD (FIELD_OP_SUBMIT_REQUEST_WITH_ARGS).
 */
// FUNCTION: FF7 0x00613b13
int __cdecl Script_OpSubmitRequestWithArgs(void)
{
    int result;

    Field_ClearRequest();
    g_pFieldRequest->code = (short)SCRIPT_PARAM_U8(3);
    g_pFieldRequest->args[1] = Script_ReadOperand16(3, 4);
    g_pFieldRequest->args[2] = Script_ReadOperand16(4, 6);
    result = Field_SubmitMusicRequest();
    g_FieldScriptPC[g_CurrentEntity] += 6;
    return result;
}

/*
 * The common tail of MUSIC, MUSVT, MUSVM and CMUSC, which set g_pFieldRequest->code and leave the rest to
 * this. Unless MULCK has locked the music, the AKAO block [1] names the track, which becomes the field's
 * music and goes to the sound driver.
 *
 * The operand names an AKAO block; Script_GetAkaoBlockOffset turns it into an offset into the script
 * image, and byte +4 of the block there, less one, is the track. That number goes three places -- the
 * request's first argument, FieldState.nextFieldMusic, and the mirror at g_FieldMusicIdAlt, which is
 * copied back out of the FieldState field rather than from the local.
 *
 * Two maps are special-cased by id. On 0x30A track 10 is set up and then deliberately not dispatched, and
 * on 0x74 nothing is dispatched at all. Both still advance the script by two.
 *
 * One short local carries the operand into the lookup and the track back out of it; the image reuses the
 * same slot for both. `track` and `hdr` are spelled that way because nothing else puts the short in
 * [ebp-8] with the pointer below it. All six declaration orders of these three locals give the same frame;
 * only the names move it.
 */
// FUNCTION: FF7 0x00613ba3
int __cdecl Field_SubmitMusicRequest(void)
{
    int offset;
    short track;
    unsigned char *hdr;

    if (g_FieldAudioDisabled == 0) {
        track = SCRIPT_PARAM_U8(1);
        offset = Script_GetAkaoBlockOffset(track);
        hdr = (unsigned char *)g_FieldScripts + offset;
        track = hdr[4] - 1;
        g_pFieldRequest->args[0] = track;
        g_pFieldState->nextFieldMusic = track;
        g_FieldMusicIdAlt = g_pFieldState->nextFieldMusic;
        // Map 0x30A sets track 10 up but does not play it.
        if (g_FieldCurrentMapId == 0x30a && track == 0xa) {
            g_FieldScriptPC[g_CurrentEntity] += 2;
            return 0;
        }
        // Map 0x74 plays no music from its scripts.
        if (g_FieldCurrentMapId != 0x74) {
            SoundDispatchCommand(g_pFieldRequest->code,
                                 g_pFieldRequest->args[0],
                                 g_pFieldRequest->args[1],
                                 g_pFieldRequest->args[2], 0, 0, 0, 0, 0);
        }
    }
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * Where AKAO block `id` of this field's script section starts: its 32-bit offset (read byte by byte,
 * little-endian) in the table that follows the 0x20-byte section header and the entity name table, eight
 * bytes per entity.
 */
// FUNCTION: FF7 0x00613cdb
int __cdecl Script_GetAkaoBlockOffset(short id)
{
    int value;

    value = *(g_FieldScripts->entityCount * 8 + id * 4 + 0x20 +
              (unsigned char *)g_FieldScripts);
    value |= *(g_FieldScripts->entityCount * 8 + id * 4 + 0x21 +
               (unsigned char *)g_FieldScripts)
             << 8;
    value |= *(g_FieldScripts->entityCount * 8 + id * 4 + 0x22 +
               (unsigned char *)g_FieldScripts)
             << 16;
    value |= *(g_FieldScripts->entityCount * 8 + id * 4 + 0x23 +
               (unsigned char *)g_FieldScripts)
             << 24;
    return value;
}

/*
 * BMUSC: chooses the battle music: the track of AKAO block [1] goes to FieldState.nextBattleMusic, which
 * the engine plays when the next battle starts. With the music locked (MULCK) it is cleared instead, and
 * the battle keeps its own music. 2 bytes.
 *
 * Field opcode 0xF6 (FIELD_OP_SET_MUSIC_ID).
 */
// FUNCTION: FF7 0x00613d7e
int __cdecl Script_OpSetMusicId(void)
{
    int offset;
    short id;
    unsigned char *block;

    if (g_FieldAudioDisabled == 0) {
        id = SCRIPT_PARAM_U8(1);
        offset = Script_GetAkaoBlockOffset(id);
        block = (unsigned char *)g_FieldScripts + offset;
        id = block[4] - 1;
        g_pFieldState->nextBattleMusic = id;
    } else {
        g_pFieldState->nextBattleMusic = 0;
    }
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * FMUSC: makes the track of AKAO block [1] the field's music (nextFieldMusic and g_FieldMusicIdAlt)
 * without playing it. With the music locked (MULCK) nextFieldMusic is cleared and the mirror kept. 2
 * bytes.
 *
 * Field opcode 0xFC (FIELD_OP_SET_MUSIC_ID_ALT).
 */
// FUNCTION: FF7 0x00613e1f
int __cdecl Script_OpSetMusicIdAlt(void)
{
    int offset;
    short id;
    unsigned char *block;

    if (g_FieldAudioDisabled == 0) {
        id = SCRIPT_PARAM_U8(1);
        offset = Script_GetAkaoBlockOffset(id);
        block = (unsigned char *)g_FieldScripts + offset;
        id = block[4] - 1;
        g_pFieldState->nextFieldMusic = id;
        g_FieldMusicIdAlt = g_pFieldState->nextFieldMusic;
    } else {
        g_pFieldState->nextFieldMusic = 0;
    }
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * TUTOR: plays tutorial [1] (counted from 1) of this map's tutorial file: the tutorial script is looked up
 * in g_pFieldTutorialTextBuffer and the field hands over to the menu module (FIELD_REQUEST_MAIN_MENU with
 * parameter 1), waiting until it hands back. 2 bytes. On map 0x9A tutorial 1 plays whatever the operand
 * says.
 *
 * Field opcode 0x21 (FIELD_OP_MENU).
 */
// FUNCTION: FF7 0x00613ecf
int __cdecl Script_OpMenu(void)
{
    int p;
    short menu;

    switch (g_pFieldState->requestCode) {
    case FIELD_REQUEST_NONE:
        g_pFieldState->requestCode = FIELD_REQUEST_MAIN_MENU;
        g_pFieldState->eventCmdParam = 1;
        g_pFieldState->requestArg = FIELD_REQUEST_PROGRESS_PENDING;
        g_FieldRequestPosted = 1;
        menu = SCRIPT_PARAM_U8(1);
        if (g_pFieldTutorialTextBuffer != 0) {
            // Map 0x9A always plays tutorial 1.
            if (g_FieldCurrentMapId == 0x9a) {
                menu = 1;
            }
            g_FieldDialogState = Field_ResolveOffsetTableEntry((int)g_pFieldTutorialTextBuffer, menu - 1);
        }
        return 1;
    }
    if (g_pFieldState->requestCode == FIELD_REQUEST_MAIN_MENU && g_pFieldState->requestArg == FIELD_REQUEST_PROGRESS_DONE) {
        g_pFieldState->requestCode = FIELD_REQUEST_NONE;
        g_pFieldState->requestArg = FIELD_REQUEST_PROGRESS_PENDING;
        g_FieldScriptPC[g_CurrentEntity] += 2;
        return 0;
    }
    return 1;
}

/*
 * MULCK: locks ([1] nonzero) or unlocks the music. While it is locked MUSIC, MUSVT, MUSVM and CMUSC do
 * nothing and BMUSC and FMUSC clear their track. 2 bytes.
 *
 * Field opcode 0xF5 (FIELD_OP_SET_AUDIO_DISABLED).
 */
// FUNCTION: FF7 0x00613fd9
int __cdecl Script_OpSetAudioDisabled(void)
{
    g_FieldAudioDisabled = SCRIPT_PARAM_U8(1);
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * BGMOVIE (community mnemonic): stores [1] in FieldState.speedBoost. 2 bytes. While it is set the player
 * walks and runs faster (ad_human.cpp), and ad_bk.cpp keeps its view-offset camera update running while
 * the field logic is busy; the community reads the opcode as switching a background movie on and off.
 *
 * Field opcode 0x27 (FIELD_OP_SET_SPEED_BOOST).
 */
// FUNCTION: FF7 0x00614023
int __cdecl Script_OpSetSpeedBoost(void)
{
    g_pFieldState->speedBoost = SCRIPT_PARAM_U8(1);
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * SCRLO: stores [1] in FieldState.unusedFlag37, which nothing in the image reads. 2 bytes.
 *
 * Field opcode 0x61 (FIELD_OP_SET_UNUSED_FLAG_37).
 */
// FUNCTION: FF7 0x00614071
int __cdecl Script_OpSetUnusedFlag37(void)
{
    g_pFieldState->unusedFlag37 = SCRIPT_PARAM_U8(1);
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * DSKCG: asks for disc [1] (gSavemap.discChangeRequest) and waits until the engine has done the disc
 * change. 2 bytes. A field request (FIELD_REQUEST_DISC_CHANGE).
 *
 * Field opcode 0x0E (FIELD_OP_REQUEST_DISC_CHANGE).
 */
// FUNCTION: FF7 0x006140bf
int __cdecl Script_OpRequestDiscChange(void)
{
    switch (g_pFieldState->requestCode) {
    case FIELD_REQUEST_NONE:
        g_pFieldState->requestCode = FIELD_REQUEST_DISC_CHANGE;
        gSavemap.discChangeRequest = SCRIPT_PARAM_U8(1);
        return 1;
    case FIELD_REQUEST_DISC_CHANGE:
        if (g_pFieldState->requestArg == FIELD_REQUEST_PROGRESS_DONE) {
            g_pFieldState->requestCode = FIELD_REQUEST_NONE;
            g_FieldScriptPC[g_CurrentEntity] += 2;
            return 0;
        }
        break;
    }
    return 1;
}

/*
 * UC: takes player control away ([1] nonzero) or gives it back. Giving it back also puts the player's
 * model back to its idle animation. The value is remembered (g_FieldDialogueLock) so that ASK, JOIN and
 * SPLIT, which lock control while they run, can restore it. 2 bytes.
 *
 * Field opcode 0x33 (FIELD_OP_SET_DIALOGUE_LOCK).
 */
// FUNCTION: FF7 0x00614155
int __cdecl Script_OpSetDialogueLock(void)
{
    g_pFieldState->characterLock = SCRIPT_PARAM_U8(1);
    g_FieldDialogueLock = g_pFieldState->characterLock;
    if (g_FieldDialogueLock == 0) {
        g_FieldModelAnimMode[g_pFieldState->playerModel] = FIELD_ANIM_MODE_IDLE;
    }
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * BTLON: switches random encounters off ([1] nonzero) or back on. 2 bytes.
 *
 * Field opcode 0x71 (FIELD_OP_SET_ENCOUNTERS_DISABLED).
 */
// FUNCTION: FF7 0x006141cc
int __cdecl Script_OpSetEncountersDisabled(void)
{
    g_pFieldState->encountersDisabled = SCRIPT_PARAM_U8(1);
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * MPDSP: stops the field from being drawn ([1] nonzero) or draws it again. 2 bytes.
 *
 * Field opcode 0x69 (FIELD_OP_SET_FIELD_HIDDEN).
 */
// FUNCTION: FF7 0x0061421a
int __cdecl Script_OpSetFieldHidden(void)
{
    g_pFieldState->fieldHidden = SCRIPT_PARAM_U8(1);
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * MVCAM: [1] = 1 makes movies leave the camera alone: the field keeps its own camera and background scroll
 * instead of the camera data the movie carries. 2 bytes.
 *
 * Field opcode 0xFB (FIELD_OP_SET_MOVIE_CAMERA_DISABLED).
 */
// FUNCTION: FF7 0x00614268
int __cdecl Script_OpSetMovieCameraDisabled(void)
{
    g_pFieldState->movieCameraDisabled = SCRIPT_PARAM_U8(1);
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * GAMEOVER: posts the game-over request (FIELD_REQUEST_GAME_OVER) and yields for ever; the pc never moves,
 * the engine leaves the field.
 *
 * Field opcode 0xFF (FIELD_OP_FIELD_REQUEST_1A).
 */
// FUNCTION: FF7 0x006142b6
int __cdecl Script_OpFieldRequest1A(void)
{
    g_pFieldState->requestCode = FIELD_REQUEST_GAME_OVER;
    g_pFieldState->requestArg = FIELD_REQUEST_PROGRESS_PENDING;
    return 1;
}

/*
 * CC: hands player control to entity [1]: its model becomes the one the player drives. An entity without a
 * model changes nothing. 2 bytes.
 *
 * Stores the entity's MODEL index, not the entity number.
 *
 * Field opcode 0xBF (FIELD_OP_SET_PLAYER_ENTITY).
 */
// FUNCTION: FF7 0x006142d5
int __cdecl Script_OpSetPlayerEntity(void)
{
    unsigned char entity;

    entity = SCRIPT_PARAM_U8(1);
    if (g_FieldEntityModel[entity] != 0xff) {
        g_pFieldState->playerModel = g_FieldEntityModel[entity];
    }
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * CHAR: gives this entity a field model: the next model slot of the map, in the order the CHAR opcodes
 * run, with [1] recorded as its model id (charId); the model starts visible. 2 bytes.
 *
 * Field opcode 0xA1 (FIELD_OP_ATTACH_MODEL).
 */
// FUNCTION: FF7 0x00614353
int __cdecl Script_OpAttachModel(void)
{
    g_FieldEntityModel[g_CurrentEntity] = g_FieldModelCount++;
    g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].charId = (unsigned short)SCRIPT_PARAM_U8(1);
    g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].visible = 1;
    g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].entityId = g_CurrentEntity;
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * DFANM: sets this model's idle animation, the loop it plays when nothing else does: animation [1] at the
 * model's animation speed divided by [2]. A model holding a frozen frame goes back to idle. 3 bytes;
 * yields for the frame.
 *
 * Field opcode 0xA2 (FIELD_OP_SET_IDLE_ANIMATION).
 */
// FUNCTION: FF7 0x00614424
int __cdecl Script_OpSetIdleAnimation(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] != 0xff) {
        g_FieldModelIdleAnimId[g_FieldEntityModel[g_CurrentEntity]] = SCRIPT_PARAM_U8(1);
        g_FieldModelIdleAnimSpeed[g_FieldEntityModel[g_CurrentEntity]] =
            g_FieldModelAnimSpeed[g_FieldEntityModel[g_CurrentEntity]] / SCRIPT_PARAM_U8(2);
        if (g_FieldModelAnimMode[g_FieldEntityModel[g_CurrentEntity]] == FIELD_ANIM_MODE_HOLD) {
            g_FieldModelAnimMode[g_FieldEntityModel[g_CurrentEntity]] = FIELD_ANIM_MODE_IDLE;
        }
    }
    g_FieldScriptPC[g_CurrentEntity] += 3;
    return 1;
}

/*
 * CCANM: sets one of the player's standard animations: [3] = 0 idle, 1 walk, 2 run gets animation [1]. [2]
 * is not read. 4 bytes.
 *
 * Field opcode 0xDC (FIELD_OP_SET_STANDARD_ANIMATION).
 */
// FUNCTION: FF7 0x00614525
int __cdecl Script_OpSetStandardAnimation(void)
{
    switch (SCRIPT_PARAM_U8(3)) {
    case FIELD_STANDARD_ANIM_IDLE:
        g_pFieldState->idleAnimId = (unsigned short)SCRIPT_PARAM_U8(1);
        break;
    case FIELD_STANDARD_ANIM_WALK:
        g_pFieldState->walkAnimId = (unsigned short)SCRIPT_PARAM_U8(1);
        break;
    case FIELD_STANDARD_ANIM_RUN:
        g_pFieldState->runAnimId = (unsigned short)SCRIPT_PARAM_U8(1);
        break;
    }
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * Starts animation [1] on this entity's model at the model's speed divided by [2], from the first frame to
 * the last; the shared start of ANIME1/ANIME2 and ANIM!1/ANIM!2. When the model's animation table entry
 * and the divisor both exceed one, the speed is multiplied by the table entry (read as a SHORT). Without
 * model data only the id and speed are set. Does not advance the pc.
 */
// FUNCTION: FF7 0x006145fd
void __cdecl Script_BeginModelAnimationFromOperands(void)
{
    g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].activeAnimId = SCRIPT_PARAM_S8(1);
    g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animSpeed = g_FieldModelAnimSpeed[g_FieldEntityModel[g_CurrentEntity]] / SCRIPT_PARAM_U8(2);
    if (g_pFieldModelSlots != 0) {
        if (g_pFieldModelSlots[g_FieldEntityModel[g_CurrentEntity]].animTable[*(char *)&g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].activeAnimId] > 1 && SCRIPT_PARAM_U8(2) > 1) {
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animSpeed *= *(short *)&g_pFieldModelSlots[g_FieldEntityModel[g_CurrentEntity]].animTable[*(char *)&g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].activeAnimId];
        }
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animCurrentFrame = 0;
        Field_SetModelAnimation((FieldModel *)&g_pFieldModelSlots[g_FieldEntityModel[g_CurrentEntity]], *(char *)&g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].activeAnimId);
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animLastFrame = g_pFieldModelSlots[g_FieldEntityModel[g_CurrentEntity]].model->lastFrame - 1;
    }
}

/*
 * ANIME1 / ANIME2: plays animation [1] once, at the model's speed divided by [2], then goes back to the
 * idle animation. ANIME1 (0xA3) waits for it to end; ANIME2 (0xAE) carries on at once. 3 bytes. On maps
 * whose record sets holdAnimAfterWait (g_FieldHoldAnimAfterWait), ANIME1 leaves the model on the last
 * frame instead of going back to idle. An entity without a model skips the opcode.
 *
 * Field opcodes 0xA3 and 0xAE (FIELD_OP_ANIMATE_AND_WAIT_A3, FIELD_OP_ANIMATE_AND_WAIT_AE).
 */
// FUNCTION: FF7 0x0061484a
int __cdecl Script_OpAnimateAndWait(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] == 0xff) {
        g_FieldScriptPC[g_CurrentEntity] += 3;
        return 0;
    }
    switch (g_FieldModelAnimMode[g_FieldEntityModel[g_CurrentEntity]]) {
    case FIELD_ANIM_MODE_IDLE:
    case FIELD_ANIM_MODE_LOOP:
    case FIELD_ANIM_MODE_HOLD:
        Script_BeginModelAnimationFromOperands();
        // ANIME2 does not wait for the animation.
        if (g_FieldScriptOpcode == FIELD_OP_ANIMATE_AND_WAIT_AE) {
            g_FieldModelAnimMode[g_FieldEntityModel[g_CurrentEntity]] = FIELD_ANIM_MODE_PLAY_THEN_IDLE;
            g_FieldScriptPC[g_CurrentEntity] += 3;
            return 0;
        }
        g_FieldModelAnimMode[g_FieldEntityModel[g_CurrentEntity]] = FIELD_ANIM_MODE_PLAY_AND_WAIT;
        break;
    case FIELD_ANIM_MODE_ENDED:
        // Maps whose record asks for it keep the last frame.
        if (g_FieldHoldAnimAfterWait != 0) {
            g_FieldModelAnimMode[g_FieldEntityModel[g_CurrentEntity]] = FIELD_ANIM_MODE_HOLD;
        } else {
            g_FieldModelAnimMode[g_FieldEntityModel[g_CurrentEntity]] = FIELD_ANIM_MODE_IDLE;
        }
        g_FieldScriptPC[g_CurrentEntity] += 3;
        return 0;
    }
    return 1;
}

/*
 * ANIM!1 / ANIM!2: plays animation [1] once at the model's speed divided by [2] and holds its last frame.
 * ANIM!1 (0xAF) carries on at once, ANIM!2 (0xBA) waits for the animation to end. 3 bytes. An entity
 * without a model skips the opcode.
 *
 * Which form waits is the code's; common opcode tables have them the other way round.
 *
 * Field opcodes 0xAF and 0xBA (FIELD_OP_ANIMATE_AND_WAIT_ALT_AF, FIELD_OP_ANIMATE_AND_WAIT_ALT_BA).
 */
// FUNCTION: FF7 0x006149a5
int __cdecl Script_OpAnimateAndWaitAlt(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] == 0xff) {
        g_FieldScriptPC[g_CurrentEntity] += 3;
        return 0;
    }
    switch (g_FieldModelAnimMode[g_FieldEntityModel[g_CurrentEntity]]) {
    case FIELD_ANIM_MODE_IDLE:
    case FIELD_ANIM_MODE_LOOP:
    case FIELD_ANIM_MODE_HOLD:
        Script_BeginModelAnimationFromOperands();
        // ANIM!1 does not wait for the animation.
        if (g_FieldScriptOpcode == FIELD_OP_ANIMATE_AND_WAIT_ALT_AF) {
            g_FieldModelAnimMode[g_FieldEntityModel[g_CurrentEntity]] = FIELD_ANIM_MODE_PLAY_THEN_HOLD;
            g_FieldScriptPC[g_CurrentEntity] += 3;
            return 0;
        }
        g_FieldModelAnimMode[g_FieldEntityModel[g_CurrentEntity]] = FIELD_ANIM_MODE_PLAY_AND_WAIT;
        break;
    case FIELD_ANIM_MODE_ENDED:
        g_FieldModelAnimMode[g_FieldEntityModel[g_CurrentEntity]] = FIELD_ANIM_MODE_HOLD;
        g_FieldScriptPC[g_CurrentEntity] += 3;
        return 0;
    }
    return 1;
}

/*
 * CANIM1 / CANIM2: plays frames [2] to [3] of animation [1] at the model's speed divided by [4], then goes
 * back to idle; both frame numbers are divided by [4] as well, and the end is clamped to the animation's
 * last frame. CANIM1 (0xB0) carries on at once, CANIM2 (0xBB) waits for the end. 5 bytes.
 *
 * BUG: without loaded model data the end frame is clamped against lastFrame, which is then never set (an
 * uninitialized local).
 *
 * Field opcodes 0xB0 and 0xBB (FIELD_OP_PLAY_ANIMATION_B0, FIELD_OP_PLAY_ANIMATION_BB).
 */
// FUNCTION: FF7 0x00614ade
int __cdecl Script_OpPlayAnimation(void)
{
    short lastFrame;
    short frame;
    short animSpeed;

    if (g_FieldEntityModel[g_CurrentEntity] == 0xff) {
        g_FieldScriptPC[g_CurrentEntity] += 5;
        return 0;
    }
    switch (g_FieldModelAnimMode[g_FieldEntityModel[g_CurrentEntity]]) {
    case FIELD_ANIM_MODE_IDLE:
    case FIELD_ANIM_MODE_LOOP:
    case FIELD_ANIM_MODE_HOLD:
        animSpeed = SCRIPT_PARAM_U8(4);
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].activeAnimId = SCRIPT_PARAM_U8(1);
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animSpeed =
            g_FieldModelAnimSpeed[g_FieldEntityModel[g_CurrentEntity]] / animSpeed;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animCurrentFrame =
            (SCRIPT_PARAM_U8(2) / animSpeed) << 4;
        if (g_pFieldModelSlots != 0) {
            Field_SetModelAnimation(
                (FieldModel *)&g_pFieldModelSlots[g_FieldEntityModel[g_CurrentEntity]],
                *(char *)&g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].activeAnimId);
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animLastFrame =
                g_pFieldModelSlots[g_FieldEntityModel[g_CurrentEntity]].model->lastFrame - 1;
            lastFrame = g_pFieldModelSlots[g_FieldEntityModel[g_CurrentEntity]].model->lastFrame - 1;
        }
        frame = SCRIPT_PARAM_U8(3) / animSpeed;
        if (frame > lastFrame) {
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animLastFrame = lastFrame;
        } else {
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animLastFrame = frame;
        }
        // CANIM1 does not wait for the animation.
        if (g_FieldScriptOpcode == FIELD_OP_PLAY_ANIMATION_B0) {
            g_FieldModelAnimMode[g_FieldEntityModel[g_CurrentEntity]] = FIELD_ANIM_MODE_PLAY_THEN_IDLE;
            g_FieldScriptPC[g_CurrentEntity] += 5;
            return 0;
        }
        g_FieldModelAnimMode[g_FieldEntityModel[g_CurrentEntity]] = FIELD_ANIM_MODE_PLAY_AND_WAIT;
        break;
    case FIELD_ANIM_MODE_ENDED:
        g_FieldModelAnimMode[g_FieldEntityModel[g_CurrentEntity]] = FIELD_ANIM_MODE_IDLE;
        g_FieldScriptPC[g_CurrentEntity] += 5;
        return 0;
    }
    return 1;
}

/*
 * CANM!1 / CANM!2: plays frames [2] to [3] of animation [1] at the model's speed divided by [4] and holds
 * the last one. Unlike CANIM the start frame [2] is not divided by [4]; the end is, and is clamped to the
 * animation's last frame. CANM!1 (0xB1) carries on at once, CANM!2 (0xBC) waits for the end. 5 bytes.
 *
 * BUG: as in CANIM, without loaded model data the end frame is clamped against an uninitialized lastFrame.
 *
 * Field opcodes 0xB1 and 0xBC (FIELD_OP_PLAY_ANIMATION_FROM_B1, FIELD_OP_PLAY_ANIMATION_FROM_BC).
 */
// FUNCTION: FF7 0x00614e3e
int __cdecl Script_OpPlayAnimationFrom(void)
{
    short lastFrame;
    short frame;
    short animSpeed;

    if (g_FieldEntityModel[g_CurrentEntity] == 0xff) {
        g_FieldScriptPC[g_CurrentEntity] += 5;
        return 0;
    }
    switch (g_FieldModelAnimMode[g_FieldEntityModel[g_CurrentEntity]]) {
    case FIELD_ANIM_MODE_IDLE:
    case FIELD_ANIM_MODE_LOOP:
    case FIELD_ANIM_MODE_HOLD:
        animSpeed = SCRIPT_PARAM_U8(4);
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].activeAnimId = SCRIPT_PARAM_U8(1);
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animSpeed =
            g_FieldModelAnimSpeed[g_FieldEntityModel[g_CurrentEntity]] / animSpeed;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animCurrentFrame =
            SCRIPT_PARAM_U8(2) << 4;
        if (g_pFieldModelSlots != 0) {
            Field_SetModelAnimation(
                (FieldModel *)&g_pFieldModelSlots[g_FieldEntityModel[g_CurrentEntity]],
                *(char *)&g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].activeAnimId);
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animLastFrame =
                g_pFieldModelSlots[g_FieldEntityModel[g_CurrentEntity]].model->lastFrame - 1;
            lastFrame = g_pFieldModelSlots[g_FieldEntityModel[g_CurrentEntity]].model->lastFrame - 1;
        }
        frame = SCRIPT_PARAM_U8(3) / animSpeed;
        if (frame > lastFrame) {
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animLastFrame = lastFrame;
        } else {
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animLastFrame = frame;
        }
        // CANM!1 does not wait for the animation.
        if (g_FieldScriptOpcode == FIELD_OP_PLAY_ANIMATION_FROM_B1) {
            g_FieldModelAnimMode[g_FieldEntityModel[g_CurrentEntity]] = FIELD_ANIM_MODE_PLAY_THEN_HOLD;
            g_FieldScriptPC[g_CurrentEntity] += 5;
            return 0;
        }
        g_FieldModelAnimMode[g_FieldEntityModel[g_CurrentEntity]] = FIELD_ANIM_MODE_PLAY_AND_WAIT;
        break;
    case FIELD_ANIM_MODE_ENDED:
        g_FieldModelAnimMode[g_FieldEntityModel[g_CurrentEntity]] = FIELD_ANIM_MODE_HOLD;
        g_FieldScriptPC[g_CurrentEntity] += 5;
        return 0;
    }
    return 1;
}

/*
 * ANIMW: waits until this model's one-shot animation (ANIME, ANIM!, CANIM, CANM!) has finished; an
 * animation that has just ended puts the model back on its idle animation. A model that plays none, or no
 * model, carries on at once. 1 byte.
 *
 * Field opcode 0xAC (FIELD_OP_WAIT_ANIMATION_DONE).
 */
// FUNCTION: FF7 0x0061519d
int __cdecl Script_OpWaitAnimationDone(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] == 0xff) {
        g_FieldScriptPC[g_CurrentEntity] += 1;
        return 0;
    }
    switch (g_FieldModelAnimMode[g_FieldEntityModel[g_CurrentEntity]]) {
    case FIELD_ANIM_MODE_PLAY_AND_WAIT:
    case FIELD_ANIM_MODE_PLAY_THEN_IDLE:
    case FIELD_ANIM_MODE_PLAY_THEN_HOLD:
        break;
    case FIELD_ANIM_MODE_ENDED:
        g_FieldModelAnimMode[g_FieldEntityModel[g_CurrentEntity]] = FIELD_ANIM_MODE_IDLE;
        /* fall through */
    default:
        g_FieldScriptPC[g_CurrentEntity] += 1;
        return 0;
    }
    return 1;
}

/*
 * ANIMB: freezes this model's animation on the frame it has reached (HOLD). 1 byte.
 *
 * Field opcode 0xDD (FIELD_OP_STOP_ANIMATION).
 */
// FUNCTION: FF7 0x00615271
int __cdecl Script_OpStopAnimation(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] != 0xff) {
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animLastFrame =
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animCurrentFrame >> 4;
        g_FieldModelAnimMode[g_FieldEntityModel[g_CurrentEntity]] = FIELD_ANIM_MODE_HOLD;
    }
    g_FieldScriptPC[g_CurrentEntity] += 1;
    return 0;
}

/*
 * MOVE: walks this entity's model to x v[2]/1, y v[4]/2 over the walkmesh and waits until it arrives. 6
 * bytes. The model plays its run animation when its speed is above three times the field's scale, its walk
 * animation otherwise, and goes back to idle at the end. The destination is read again every frame, so a
 * script can move the target while the walk is on. An entity without a model skips the opcode.
 *
 * Positions are kept in 1/4096 units: script coordinates are shifted left 12 on the way in.
 *
 * Field opcode 0xA8 (FIELD_OP_MOVE_TO).
 */
// FUNCTION: FF7 0x0061530c
int __cdecl Script_OpMoveTo(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] == 0xFF) {
        g_FieldScriptPC[g_CurrentEntity] += 6;
        return 0;
    } else {
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].actionArg = 0;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].dirLock = 0;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].moveEndX = (short)Script_ReadOperand16(1, 2) << 12;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].moveEndY = (short)Script_ReadOperand16(2, 4) << 12;
        // Faster than three times the field's scale: run, else walk.
        if (g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].moveSpeed > (g_pFieldState->fieldScale * 1536) / 512) {
            if (*(char *)&g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].activeAnimId != FIELD_STANDARD_ANIM_RUN) {
                g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].activeAnimId = FIELD_STANDARD_ANIM_RUN;
                g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animSpeed = 16;
                g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animCurrentFrame = 0;
                if (((int)g_pFieldModelSlots) != 0) {
                    Field_SetModelAnimation((FieldModel *)&g_pFieldModelSlots[g_FieldEntityModel[g_CurrentEntity]], *(char *)&g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].activeAnimId);
                    g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animLastFrame = g_pFieldModelSlots[g_FieldEntityModel[g_CurrentEntity]].model->lastFrame - 1;
                }
            }
        } else if (*(char *)&g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].activeAnimId != FIELD_STANDARD_ANIM_WALK) {
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].activeAnimId = FIELD_STANDARD_ANIM_WALK;
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animSpeed = 16;
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animCurrentFrame = 0;
            if (((int)g_pFieldModelSlots) != 0) {
                Field_SetModelAnimation((FieldModel *)&g_pFieldModelSlots[g_FieldEntityModel[g_CurrentEntity]], *(char *)&g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].activeAnimId);
                g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animLastFrame = g_pFieldModelSlots[g_FieldEntityModel[g_CurrentEntity]].model->lastFrame - 1;
            }
        }
        g_FieldModelAnimMode[g_FieldEntityModel[g_CurrentEntity]] = FIELD_ANIM_MODE_LOOP;
        if (*(char *)&g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].scriptedMoveMode == FIELD_SCRIPTED_MOVE_WALK) {
            switch (g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].actionState) {
            case FIELD_MOVE_STATE_RUNNING:
                return 1;
            case FIELD_MOVE_STATE_DONE:
                g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].scriptedMoveMode = FIELD_SCRIPTED_MOVE_NONE;
                g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].actionState = FIELD_MOVE_STATE_START;
                g_FieldModelAnimMode[g_FieldEntityModel[g_CurrentEntity]] = FIELD_ANIM_MODE_IDLE;
                g_FieldScriptPC[g_CurrentEntity] += 6;
                return 0;
            }
        }
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].scriptedMoveMode = FIELD_SCRIPTED_MOVE_WALK;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].actionState = FIELD_MOVE_STATE_START;
        return 1;
    }
}

/*
 * FMOVE: MOVE without the walk animation: the model goes to x v[2]/1, y v[4]/2 in whatever animation it is
 * playing, and the opcode waits until it arrives. 6 bytes.
 *
 * Field opcode 0xAD (FIELD_OP_MOVE_TO_DIRECT).
 */
// FUNCTION: FF7 0x006157e8
int __cdecl Script_OpMoveToDirect(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] == 0xFF) {
        g_FieldScriptPC[g_CurrentEntity] += 6;
        return 0;
    } else {
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].actionArg = 0;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].dirLock = 0;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].moveEndX = (short)Script_ReadOperand16(1, 2) << 12;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].moveEndY = (short)Script_ReadOperand16(2, 4) << 12;
        if (*(char *)&g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].scriptedMoveMode == FIELD_SCRIPTED_MOVE_WALK) {
            switch (g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].actionState) {
            case FIELD_MOVE_STATE_RUNNING:
                return 1;
            case FIELD_MOVE_STATE_DONE:
                g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].scriptedMoveMode = FIELD_SCRIPTED_MOVE_NONE;
                g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].actionState = FIELD_MOVE_STATE_START;
                g_FieldScriptPC[g_CurrentEntity] += 6;
                return 0;
            }
        }
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].scriptedMoveMode = FIELD_SCRIPTED_MOVE_WALK;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].actionState = FIELD_MOVE_STATE_START;
        return 1;
    }
}

/*
 * CMOVE: FMOVE with the facing locked (dirLock) for the move: the model slides to x v[2]/1, y v[4]/2
 * without turning, and the opcode waits until it arrives. 6 bytes.
 *
 * Field opcode 0xA9 (FIELD_OP_MOVE_TO_STYLED).
 */
// FUNCTION: FF7 0x006159f4
int __cdecl Script_OpMoveToStyled(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] == 0xFF) {
        g_FieldScriptPC[g_CurrentEntity] += 6;
        return 0;
    } else {
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].actionArg = 0;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].dirLock = 1;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].moveEndX = (short)Script_ReadOperand16(1, 2) << 12;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].moveEndY = (short)Script_ReadOperand16(2, 4) << 12;
        if (*(char *)&g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].scriptedMoveMode == FIELD_SCRIPTED_MOVE_WALK) {
            switch (g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].actionState) {
            case FIELD_MOVE_STATE_RUNNING:
                return 1;
            case FIELD_MOVE_STATE_DONE:
                g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].dirLock = 0;
                g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].scriptedMoveMode = FIELD_SCRIPTED_MOVE_NONE;
                g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].actionState = FIELD_MOVE_STATE_START;
                g_FieldScriptPC[g_CurrentEntity] += 6;
                return 0;
            }
        }
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].scriptedMoveMode = FIELD_SCRIPTED_MOVE_WALK;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].actionState = FIELD_MOVE_STATE_START;
        return 1;
    }
}

/*
 * FCFIX (community: "facing fix"): locks ([1] nonzero) or frees this model's facing while it moves
 * (dirLock), so it keeps looking one way. 2 bytes.
 *
 * Field opcode 0xDB (FIELD_OP_SET_MOVE_STYLE).
 */
// FUNCTION: FF7 0x00615c24
int __cdecl Script_OpSetMoveStyle(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] != 0xff) {
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].dirLock =
            SCRIPT_PARAM_U8(1);
    }
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * JUMP: jumps this model to x v[3]/1, y v[5]/2 on walkmesh triangle v[7]/3, taking v[9]/4 steps, and waits
 * until it lands. 11 bytes. Unlike MOVE the operands are read once, when the jump starts.
 *
 * Field opcode 0xC0 (FIELD_OP_MOVE_TO_3D).
 */
// FUNCTION: FF7 0x00615ca3
int __cdecl Script_OpMoveTo3D(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] == 0xFF) {
        g_FieldScriptPC[g_CurrentEntity] += 0xB;
        return 0;
    } else {
        if (*(char *)&g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].scriptedMoveMode == FIELD_SCRIPTED_MOVE_JUMP) {
            switch (g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].actionState) {
            case FIELD_MOVE_STATE_RUNNING:
                return 1;
            case FIELD_MOVE_STATE_DONE:
                g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].scriptedMoveMode = FIELD_SCRIPTED_MOVE_NONE;
                g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].actionState = FIELD_MOVE_STATE_START;
                g_FieldScriptPC[g_CurrentEntity] += 0xB;
                return 0;
            }
        }
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].scriptedMoveMode = FIELD_SCRIPTED_MOVE_JUMP;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].actionState = FIELD_MOVE_STATE_START;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].moveEndX = (short)Script_ReadOperand16(1, 3) << 12;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].moveEndY = (short)Script_ReadOperand16(2, 5) << 12;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].moveEndI = Script_ReadOperand16(3, 7);
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].moveSteps = Script_ReadOperand16(4, 9);
        return 1;
    }
}

/*
 * LADER: climbs this model up or along a ladder to x v[3]/1, y v[5]/2, z v[7]/3 on triangle v[9]/4,
 * playing animation [12] at the model's speed divided by [14] and facing [13]; waits until it arrives. 15
 * bytes. [11] picks the climb: 0 and 1 up and down (FIELD_SCRIPTED_MOVE_LADDER_UP_DOWN), 2 and 3 left and
 * right (_LEFT_RIGHT), its low bit going to actionArg.
 *
 * Field opcode 0xC2 (FIELD_OP_MOVE_TO_MODE).
 */
// FUNCTION: FF7 0x00615ec6
int __cdecl Script_OpMoveToMode(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] == 0xFF) {
        g_FieldScriptPC[g_CurrentEntity] += 0xF;
        return 0;
    } else {
        switch (*(char *)&g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].scriptedMoveMode) {
        case FIELD_SCRIPTED_MOVE_LADDER_UP_DOWN:
        case FIELD_SCRIPTED_MOVE_LADDER_LEFT_RIGHT:
            switch (g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].actionState) {
            case FIELD_MOVE_STATE_RUNNING:
                return 1;
            case FIELD_MOVE_STATE_DONE:
                g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].scriptedMoveMode = FIELD_SCRIPTED_MOVE_NONE;
                g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].actionState = FIELD_MOVE_STATE_START;
                g_FieldScriptPC[g_CurrentEntity] += 0xF;
                return 0;
            }
            break;
        }
        switch (SCRIPT_PARAM_U8(0xB)) {
        case FIELD_MOVETO_WALK_ANIM:
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].scriptedMoveMode = FIELD_SCRIPTED_MOVE_LADDER_UP_DOWN;
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].actionArg = 0;
            break;
        case FIELD_MOVETO_RUN_ANIM:
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].scriptedMoveMode = FIELD_SCRIPTED_MOVE_LADDER_UP_DOWN;
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].actionArg = 1;
            break;
        case FIELD_MOVETO_SLIDE_FACING:
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].scriptedMoveMode = FIELD_SCRIPTED_MOVE_LADDER_LEFT_RIGHT;
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].actionArg = 0;
            break;
        case FIELD_MOVETO_SLIDE_LOCKED:
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].scriptedMoveMode = FIELD_SCRIPTED_MOVE_LADDER_LEFT_RIGHT;
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].actionArg = 1;
        }
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].actionState = FIELD_MOVE_STATE_START;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].moveEndX = (short)Script_ReadOperand16(1, 3) << 12;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].moveEndY = (short)Script_ReadOperand16(2, 5) << 12;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].moveEndZ = (short)Script_ReadOperand16(3, 7) << 12;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].moveEndI = Script_ReadOperand16(4, 9);
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].activeAnimId = SCRIPT_PARAM_S8(0xC);
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animSpeed = g_FieldModelAnimSpeed[g_FieldEntityModel[g_CurrentEntity]] / SCRIPT_PARAM_U8(14);
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animCurrentFrame = 0;
        if (((int)g_pFieldModelSlots) != 0) {
            Field_SetModelAnimation((FieldModel *)&g_pFieldModelSlots[g_FieldEntityModel[g_CurrentEntity]], *(char *)&g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].activeAnimId);
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animLastFrame = g_pFieldModelSlots[g_FieldEntityModel[g_CurrentEntity]].model->lastFrame - 1;
        }
        g_FieldModelAnimMode[g_FieldEntityModel[g_CurrentEntity]] = FIELD_ANIM_MODE_IDLE;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].dir = SCRIPT_PARAM_S8(0xD);
        return 1;
    }
}

/*
 * PMOVA: MOVA towards the character in party slot [1]. 2 bytes.
 *
 * BUG: an empty slot or a character with no entity here forwards entity 0xFF, and
 * Script_StepModelToModel's model test then reads g_FieldEntityModel[0xFF], 207 bytes past the 48-entry
 * table, inside the script palette store (g_FieldPalettes); unless that byte happens to be 0xFF the walk
 * goes on with it as a model index.
 *
 * The same PHS-slot resolution as Script_OpFacePartyMember, forwarding 0xFF for an empty slot. The callee
 * takes a byte here, not a short: `mov dl, [ebp-4]` with no extension before the push.
 *
 * Field opcode 0x2A (FIELD_OP_MOVE_TO_PARTY_MEMBER).
 */
// FUNCTION: FF7 0x006163e0
int __cdecl Script_OpMoveToPartyMember(void)
{
    unsigned char entity;
    unsigned char member;

    member = gSavemap.fieldParty[SCRIPT_PARAM_U8(1)];
    if (member == 0xff) {
        entity = 0xff;
    } else {
        entity = g_FieldCharEntity.charEntity[member];
    }
    return Script_StepModelToModel(entity);
}

/*
 * MOVA: walks this model to entity [1]. 2 bytes; see Script_StepModelToModel.
 *
 * Field opcode 0xAA (FIELD_OP_MOVE_TO_ENTITY).
 */
// FUNCTION: FF7 0x00616444
int __cdecl Script_OpMoveToEntity(void)
{
    unsigned char entity;

    entity = SCRIPT_PARAM_U8(1);
    return Script_StepModelToModel(entity);
}

/*
 * The walk of MOVA and PMOVA: the target's position is read again every frame, so the mover follows it if
 * it moves, and the target's solid radius (solidRange) is the distance the mover stops short at, so it
 * does not walk into it. Walk or run animation by speed, as MOVE; waits until it arrives, then goes back
 * to idle. Either entity without a model skips the opcode.
 *
 * Unlike MOVE it does not reset actionState when the walk ends; the next move starts it again.
 */
// FUNCTION: FF7 0x00616476
int __cdecl Script_StepModelToModel(unsigned char targetEntity)
{
    if (g_FieldEntityModel[g_CurrentEntity] == 0xFF || g_FieldEntityModel[targetEntity] == 0xFF) {
        g_FieldScriptPC[g_CurrentEntity] += 2;
        return 0;
    } else {
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].actionArg = *(short *)&g_pFieldEntities[g_FieldEntityModel[targetEntity]].solidRange;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].dirLock = 0;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].moveEndX = g_pFieldEntities[g_FieldEntityModel[targetEntity]].posX;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].moveEndY = g_pFieldEntities[g_FieldEntityModel[targetEntity]].posY;
        if (*(char *)&g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].scriptedMoveMode == FIELD_SCRIPTED_MOVE_WALK) {
            switch (g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].actionState) {
            case FIELD_MOVE_STATE_RUNNING:
                if (g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].moveSpeed > (g_pFieldState->fieldScale * 1536) / 512) {
                    if (*(char *)&g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].activeAnimId != FIELD_STANDARD_ANIM_RUN) {
                        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].activeAnimId = FIELD_STANDARD_ANIM_RUN;
                        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animSpeed = 16;
                        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animCurrentFrame = 0;
                        if (((int)g_pFieldModelSlots) != 0) {
                            Field_SetModelAnimation((FieldModel *)&g_pFieldModelSlots[g_FieldEntityModel[g_CurrentEntity]], *(char *)&g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].activeAnimId);
                            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animLastFrame = g_pFieldModelSlots[g_FieldEntityModel[g_CurrentEntity]].model->lastFrame - 1;
                        }
                    }
                } else if (*(char *)&g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].activeAnimId != FIELD_STANDARD_ANIM_WALK) {
                    g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].activeAnimId = FIELD_STANDARD_ANIM_WALK;
                    g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animSpeed = 16;
                    g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animCurrentFrame = 0;
                    if (((int)g_pFieldModelSlots) != 0) {
                        Field_SetModelAnimation((FieldModel *)&g_pFieldModelSlots[g_FieldEntityModel[g_CurrentEntity]], *(char *)&g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].activeAnimId);
                        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animLastFrame = g_pFieldModelSlots[g_FieldEntityModel[g_CurrentEntity]].model->lastFrame - 1;
                    }
                }
                g_FieldModelAnimMode[g_FieldEntityModel[g_CurrentEntity]] = FIELD_ANIM_MODE_LOOP;
                return 1;
            case FIELD_MOVE_STATE_DONE:
                g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].scriptedMoveMode = FIELD_SCRIPTED_MOVE_NONE;
                g_FieldModelAnimMode[g_FieldEntityModel[g_CurrentEntity]] = FIELD_ANIM_MODE_IDLE;
                g_FieldScriptPC[g_CurrentEntity] += 2;
                return 0;
            }
        }
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].scriptedMoveMode = FIELD_SCRIPTED_MOVE_WALK;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].actionState = FIELD_MOVE_STATE_START;
        return 1;
    }
}

/*
 * DIRA: turns this model to face entity [1] at once, and yields for the frame. 2 bytes.
 *
 * `movzx cx, byte ptr` rather than `mov cl`: the callee's parameter is two bytes wide, so the unsigned
 * char argument is widened first.
 *
 * Field opcode 0xB6 (FIELD_OP_FACE_ENTITY).
 */
// FUNCTION: FF7 0x0061698a
int __cdecl Script_OpFaceEntity(void)
{
    unsigned char entity;

    entity = SCRIPT_PARAM_U8(1);
    return Script_FaceModelAtModel(entity);
}

/*
 * PDIRA: DIRA towards the character in party slot [1]. 2 bytes.
 *
 * BUG: an empty slot forwards entity 0xFF, and the model test then reads g_FieldEntityModel[0xFF], past
 * the 48-entry table (see Script_OpMoveToPartyMember).
 *
 * The same PHS-slot resolution Script_OpPartyReq does, forwarding 0xFF for an empty slot rather than
 * testing for it here. `movzx dx, byte ptr [ebp-4]` before the push: the callee's parameter is sixteen
 * bits, not eight.
 *
 * Field opcode 0x34 (FIELD_OP_FACE_PARTY_MEMBER).
 */
// FUNCTION: FF7 0x006169be
int __cdecl Script_OpFacePartyMember(void)
{
    unsigned char entity;
    unsigned char member;

    member = gSavemap.fieldParty[SCRIPT_PARAM_U8(1)];
    if (member == 0xff) {
        entity = 0xff;
    } else {
        entity = g_FieldCharEntity.charEntity[member];
    }
    return Script_FaceModelAtModel(entity);
}

/*
 * The turn of DIRA and PDIRA: sets this model's facing to the heading towards the other model, cancels any
 * turn in progress, and yields. Two models on exactly the same spot would give the heading helper nothing
 * to aim along, so the first point is nudged by one unit. Either entity without a model skips the opcode.
 */
// FUNCTION: FF7 0x00616a24
int __cdecl Script_FaceModelAtModel(short otherEntity)
{
    int padSlot; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int padGap; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int outVal;
    FieldVec3i pointA;
    FieldVec3i pointB;

    if (g_FieldEntityModel[g_CurrentEntity] == 0xFF || g_FieldEntityModel[otherEntity] == 0xFF) {
        g_FieldScriptPC[g_CurrentEntity] += 2;
        return 0;
    } else {
        Field_MakePoint(&pointA, g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].posX >> 12, g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].posY >> 12, g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].posZ >> 12);
        Field_MakePoint(&pointB, g_pFieldEntities[g_FieldEntityModel[otherEntity]].posX >> 12, g_pFieldEntities[g_FieldEntityModel[otherEntity]].posY >> 12, g_pFieldEntities[g_FieldEntityModel[otherEntity]].posZ >> 12);
        if (pointA.x == pointB.x && pointA.y == pointB.y) {
            pointA.x++;
        }
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].dir = Field_HeadingBetweenPoints((FieldPointTo *)&pointA, (FieldPointTo *)&pointB, &outVal);
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnType = FIELD_INTERP_NONE;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnStep = 0;
        g_FieldScriptPC[g_CurrentEntity] += 2;
        return 1;
    }
}

/*
 * TURA: turns this model towards entity [1] over [2] steps, the way round [3] picks (0 increasing, 1
 * decreasing, 2 shortest), and waits until the turn is done. 4 bytes; see Script_TurnModelToModel.
 *
 * Field opcode 0xAB (FIELD_OP_TURN_TO_ENTITY).
 */
// FUNCTION: FF7 0x00616c1b
int __cdecl Script_OpTurnToEntity(void)
{
    unsigned char entity;

    entity = SCRIPT_PARAM_U8(1);
    return Script_TurnModelToModel(entity);
}

/*
 * PTURA: TURA towards the character in party slot [1]. 4 bytes.
 *
 * BUG: an empty slot forwards entity 0xFF, and the model test then reads g_FieldEntityModel[0xFF], past
 * the 48-entry table (see Script_OpMoveToPartyMember).
 *
 * The same PHS-slot resolution Script_OpPartyReq does, forwarding 0xFF for an empty slot rather than
 * testing for it here. `movzx dx, byte ptr [ebp-4]` before the push: the callee's parameter is sixteen
 * bits, not eight.
 *
 * Field opcode 0x35 (FIELD_OP_TURN_TO_PARTY_MEMBER).
 */
// FUNCTION: FF7 0x00616c4f
int __cdecl Script_OpTurnToPartyMember(void)
{
    unsigned char entity;
    unsigned char member;

    member = gSavemap.fieldParty[SCRIPT_PARAM_U8(1)];
    if (member == 0xff) {
        entity = 0xff;
    } else {
        entity = g_FieldCharEntity.charEntity[member];
    }
    return Script_TurnModelToModel(entity);
}

/*
 * The turn of TURA and PTURA. On a frame with no turn under way (or one of another kind or speed) it aims
 * the turn from the current facing to the heading towards the other model, nudging the first point by one
 * unit when both stand on the same spot, and adjusts the target by a full turn (256) for the direction
 * [3]: 2 shortest (only beyond half a turn), 1 decreasing, 0 increasing; any other value leaves the raw
 * heading. A turn already running at the requested speed is left alone. Waits until the turn is done;
 * either entity without a model skips the opcode.
 */
// FUNCTION: FF7 0x00616cb5
int __cdecl Script_TurnModelToModel(short entity)
{
    int first[4];
    int dist;
    int second[4];
    short turnDelta;

    if (g_FieldEntityModel[g_CurrentEntity] == 0xFF || g_FieldEntityModel[entity] == 0xFF) {
        g_FieldScriptPC[g_CurrentEntity] += 4;
        return 0;
    } else {
        switch (*(char *)&g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnType) {
        case FIELD_INTERP_DONE:
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnType = FIELD_INTERP_NONE;
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnStep = 0;
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnSteps = 0;
            g_FieldScriptPC[g_CurrentEntity] += 4;
            return 0;
        }
        if (g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnStep == 0 || (g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnType != FIELD_INTERP_SMOOTH || g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnSteps != SCRIPT_PARAM_U8(2))) {
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnStart = (unsigned short)(g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].dir);
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnType = FIELD_INTERP_SMOOTH;
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnSteps = SCRIPT_PARAM_S8(2);
            Field_MakePoint((FieldVec3i *)first, g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].posX >> 12, g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].posY >> 12, g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].posZ >> 12);
            Field_MakePoint((FieldVec3i *)second, g_pFieldEntities[g_FieldEntityModel[entity]].posX >> 12, g_pFieldEntities[g_FieldEntityModel[entity]].posY >> 12, g_pFieldEntities[g_FieldEntityModel[entity]].posZ >> 12);
            if (first[0] == second[0] && first[1] == second[1]) {
                first[0]++;
            }
            /* Retail caller treats `Field_HeadingBetweenPoints` return (`int`) as `unsigned char` for caller zero/sign extension. */
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnEnd = ((unsigned char (__cdecl *)(struct FieldPointTo *from, struct FieldPointTo *to, int *distance))Field_HeadingBetweenPoints)((FieldPointTo *)first, (FieldPointTo *)second, &dist) & 0xFF;
            switch ((char)SCRIPT_PARAM_S8(3)) {
            case FIELD_TURN_ROTATION_SHORTEST:
                turnDelta = g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnEnd - g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnStart;
                if (turnDelta < 0) {
                    turnDelta ^= 0xFFFF;
                    turnDelta++;
                }
                if (turnDelta > 128) {
                    if (g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnEnd > g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnStart) {
                        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnEnd += 0xFF00;
                    } else {
                        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnEnd += 0x100;
                    }
                }
                break;
            case FIELD_TURN_ROTATION_DECREASING:
                if (g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].dir < g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnEnd) {
                    g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnEnd += 0xFF00;
                }
                break;
            case FIELD_TURN_ROTATION_INCREASING:
                if (g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].dir > g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnEnd) {
                    g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnEnd += 0x100;
                }
            }
        }
        return 1;
    }
}

/*
 * OFST: offsets this model from its walkmesh position by x v[4]/1, y v[6]/2, z v[8]/3: at once when the
 * interpolation type [3] is 0, otherwise over v[10]/4 steps (1 linear, 2 smooth) starting from the current
 * offset. Does not wait (OFSTW does). 12 bytes, also without a model.
 *
 * The handler is named for a jump; the community mnemonic OFST and the offset* members it writes say
 * offset.
 *
 * Field opcode 0xC3 (FIELD_OP_JUMP_TO).
 */
// FUNCTION: FF7 0x006172e8
int __cdecl Script_OpJumpTo(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] != 0xFF) {
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].offsetStep = 0;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].offsetSteps = Script_ReadOperand16(4, 0xA);
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].offsetEndX = Script_ReadOperand16(1, 4);
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].offsetEndY = Script_ReadOperand16(2, 6);
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].offsetEndZ = Script_ReadOperand16(3, 8);
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].ofsType = SCRIPT_PARAM_S8(3);
        if (g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].ofsType != FIELD_INTERP_NONE) {
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].offsetStartX = *(short *)&g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].offsetX;
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].offsetStartY = *(short *)&g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].offsetY;
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].offsetStartZ = *(short *)&g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].offsetZ;
        } else {
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].offsetX = (int)(g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].offsetEndX);
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].offsetY = (int)(g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].offsetEndY);
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].offsetZ = (int)(g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].offsetEndZ);
        }
    }
    g_FieldScriptPC[g_CurrentEntity] += 0xC;
    return 0;
}

/*
 * OFSTW: waits until this model's OFST offset has finished. 1 byte.
 *
 * Field opcode 0xC4 (FIELD_OP_WAIT_OFFSET_DONE).
 */
// FUNCTION: FF7 0x006175f3
int __cdecl Script_OpWaitOffsetDone(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] == 0xff) {
        g_FieldScriptPC[g_CurrentEntity] += 1;
        return 0;
    }
    switch (g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].ofsType) {
    case FIELD_INTERP_NONE:
    case FIELD_INTERP_DONE:
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].ofsType = FIELD_INTERP_NONE;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].offsetStep = 0;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].offsetSteps = 0;
        g_FieldScriptPC[g_CurrentEntity] += 1;
        return 0;
    }
    return 1;
}

/*
 * TURNW: waits until this model's turn has finished; no turn, or no model, carries on at once. 1 byte.
 *
 * Field opcode 0xDE (FIELD_OP_WAIT_TURN_DONE).
 */
// FUNCTION: FF7 0x00617701
int __cdecl Script_OpWaitTurnDone(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] != 0xff) {
        switch (g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnType) {
        case FIELD_INTERP_NONE:
            g_FieldScriptPC[g_CurrentEntity] += 1;
            return 0;
        case FIELD_INTERP_DONE:
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnType = FIELD_INTERP_NONE;
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnStep = 0;
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnSteps = 0;
            g_FieldScriptPC[g_CurrentEntity] += 1;
            return 0;
        }
        return 1;
    }
    g_FieldScriptPC[g_CurrentEntity] += 1;
    return 0;
}

/*
 * TURN: turns this model to facing v[2]/2 (read as a word) over [4] steps, interpolation [5] (1 linear, 2
 * smooth), and waits until it is done. 6 bytes. A change of target, speed or kind while it runs restarts
 * it from the current facing.
 *
 * Unlike TURNGEN it never reads the direction byte [3], so the turn goes whichever way the raw numbers
 * fall.
 *
 * Field opcode 0xB5 (FIELD_OP_TURN_TO_ANGLE).
 */
// FUNCTION: FF7 0x00617835
int __cdecl Script_OpTurnToAngle(void)
{
    short val;

    if (g_FieldEntityModel[g_CurrentEntity] != 0xFF) {
        switch (*(char *)&g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnType) {
        case FIELD_INTERP_DONE:
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnType = FIELD_INTERP_NONE;
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnStep = 0;
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnSteps = 0;
            g_FieldScriptPC[g_CurrentEntity] += 6;
            return 0;
        }
        val = Script_ReadOperand16(2, 2);
        if (g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnStep == 0 || (val != g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnEnd || (g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnType != SCRIPT_PARAM_U8(5) || g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnSteps != SCRIPT_PARAM_U8(4)))) {
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnStart = (unsigned short)(g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].dir);
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnType = SCRIPT_PARAM_S8(5);
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnSteps = SCRIPT_PARAM_S8(4);
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnEnd = val;
        }
    } else {
        g_FieldScriptPC[g_CurrentEntity] += 6;
        return 0;
    }
    return 1;
}

/*
 * TURNGEN: turns this model to facing v[2]/2 (a byte, 256 to a full turn) over [4] steps, interpolation
 * [5], the way round [3] picks (0 increasing, 1 decreasing, 2 shortest), and waits until it is done. 6
 * bytes. A change of target alone does not restart a running turn; a change of speed or kind does.
 *
 * It waits like TURN, whatever common opcode tables say.
 *
 * Field opcode 0xB4 (FIELD_OP_TURN_START).
 */
// FUNCTION: FF7 0x00617b0f
int __cdecl Script_OpTurnStart(void)
{
    short d;

    if (g_FieldEntityModel[g_CurrentEntity] != 0xFF) {
        switch (g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnType) {
        case FIELD_INTERP_DONE:
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnType = FIELD_INTERP_NONE;
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnStep = 0;
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnSteps = 0;
            g_FieldScriptPC[g_CurrentEntity] += 6;
            return 0;
        }
        if (g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnStep == 0 || g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnType != SCRIPT_PARAM_U8(5) ||
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnSteps != SCRIPT_PARAM_U8(4)) {
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnStart = (unsigned short)g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].dir;
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnType = SCRIPT_PARAM_U8(5);
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnSteps = SCRIPT_PARAM_U8(4);
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnEnd = Script_ReadOperand8(2, 2) & 0xFF;
            switch (SCRIPT_PARAM_U8(3)) {
            case FIELD_TURN_ROTATION_SHORTEST:
                d = g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnEnd - g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnStart;
                if (d < 0) {
                    d ^= 0xFFFF;
                    d += 1;
                }
                if (d > 128) {
                    if (g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnEnd > g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnStart) {
                        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnEnd += -0x100;
                    } else {
                        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnEnd += 0x100;
                    }
                }
                break;
            case FIELD_TURN_ROTATION_DECREASING:
                if (g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].dir < g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnEnd) {
                    g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnEnd += -0x100;
                }
                break;
            case FIELD_TURN_ROTATION_INCREASING:
                if (g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].dir > g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnEnd) {
                    g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnEnd += 0x100;
                }
                break;
            }
        }
    } else {
        g_FieldScriptPC[g_CurrentEntity] += 6;
        return 0;
    }
    return 1;
}

/*
 * DIR: sets this model's facing to v[2]/2 (256 to a full turn) at once, cancels a turn in progress and
 * yields for the frame; without a model it carries on. 3 bytes.
 *
 * Field opcode 0xB3 (FIELD_OP_SET_DIRECTION).
 */
// FUNCTION: FF7 0x00618062
int __cdecl Script_OpSetDirection(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] != 0xff) {
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].dir = Script_ReadOperand8(2, 2);
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnType = FIELD_INTERP_NONE;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].turnStep = 0;
        g_FieldScriptPC[g_CurrentEntity] += 3;
        return 1;
    }
    g_FieldScriptPC[g_CurrentEntity] += 3;
    return 0;
}

/*
 * SLIDR: sets this model's solid radius (solidRange), the distance MOVA stops short at, to v[2]/2 scaled
 * by the field's scale / 512. 3 bytes.
 *
 * The function is named for the talk radius; the member it writes and the community mnemonic (SLIDR) say
 * it is the solid one.
 *
 * Field opcode 0xC6 (FIELD_OP_SET_TALK_RADIUS).
 */
// FUNCTION: FF7 0x0061813d
int __cdecl Script_OpSetTalkRadius(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] != 0xff) {
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].solidRange =
            (unsigned short)(Script_ReadOperand8(2, 2) * g_pFieldState->fieldScale / 512);
    }
    g_FieldScriptPC[g_CurrentEntity] += 3;
    return 0;
}

/*
 * SLDR2: SLIDR with a 16-bit radius v[2]/2. 4 bytes. Named for the talk radius, as Script_OpSetTalkRadius.
 *
 * Field opcode 0xD7 (FIELD_OP_SET_TALK_RADIUS_W).
 */
// FUNCTION: FF7 0x006181c9
int __cdecl Script_OpSetTalkRadiusW(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] != 0xff) {
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].solidRange =
            (unsigned short)(Script_ReadOperand16(2, 2) * g_pFieldState->fieldScale / 512);
    }
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * TALKR: sets this model's talk radius (talkRange), how near the player must be to talk to it, to v[2]/2
 * scaled by the field's scale / 512. 3 bytes.
 *
 * The function is named for the collision radius; the member it writes and the community mnemonic (TALKR)
 * say it is the talk one.
 *
 * Field opcode 0xC5 (FIELD_OP_SET_COLLISION_RADIUS).
 */
// FUNCTION: FF7 0x00618253
int __cdecl Script_OpSetCollisionRadius(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] != 0xff) {
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].talkRange =
            (unsigned short)(Script_ReadOperand8(2, 2) * g_pFieldState->fieldScale / 512);
    }
    g_FieldScriptPC[g_CurrentEntity] += 3;
    return 0;
}

/*
 * TLKR2: TALKR with a 16-bit radius v[2]/2. 4 bytes. Named for the collision radius, as
 * Script_OpSetCollisionRadius.
 *
 * Field opcode 0xD6 (FIELD_OP_SET_COLLISION_RADIUS_W).
 */
// FUNCTION: FF7 0x006182df
int __cdecl Script_OpSetCollisionRadiusW(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] != 0xff) {
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].talkRange =
            (unsigned short)(Script_ReadOperand16(2, 2) * g_pFieldState->fieldScale / 512);
    }
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * MSPED: sets this model's walking speed to v[2]/2 scaled by the field's scale / 512. 4 bytes.
 *
 * Field opcode 0xB2 (FIELD_OP_SET_MOVE_SPEED).
 */
// FUNCTION: FF7 0x00618369
int __cdecl Script_OpSetMoveSpeed(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] != 0xff) {
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].moveSpeed =
            (unsigned short)(Script_ReadOperand16(2, 2) * g_pFieldState->fieldScale / 512);
    }
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * ASPED: sets this model's animation speed to v[2]/2, also as the base the ANIME-family divisors divide. 4
 * bytes.
 *
 * Field opcode 0xBD (FIELD_OP_SET_ANIMATION_SPEED).
 */
// FUNCTION: FF7 0x006183f3
int __cdecl Script_OpSetAnimationSpeed(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] != 0xff) {
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animSpeed =
            Script_ReadOperand16(2, 2);
        g_FieldModelAnimSpeed[g_FieldEntityModel[g_CurrentEntity]] =
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].animSpeed;
    }
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * GETDIR: reads the facing of entity [2] into v[3]/2; an entity without a model leaves the variable alone.
 * 4 bytes.
 *
 * Field opcode 0xB7 (FIELD_OP_GET_ENTITY_DIRECTION).
 */
// FUNCTION: FF7 0x0061849a
int __cdecl Script_OpGetEntityDirection(void)
{
    unsigned char entity;

    entity = SCRIPT_PARAM_U8(2);
    if (g_FieldEntityModel[entity] != 0xff) {
        Script_WriteOperand8(2, 3,
                             g_pFieldEntities[g_FieldEntityModel[entity]].dir);
    }
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * PGTDR: reads the facing of the character in party slot [2] into v[3]/2; nothing is written for a slot of
 * 3 or more, an empty slot, or a character with no model here. 4 bytes.
 *
 * Field opcode 0x73 (FIELD_OP_GET_PARTY_DIRECTION).
 */
// FUNCTION: FF7 0x0061852b
int __cdecl Script_OpGetPartyDirection(void)
{
    unsigned char id;

    id = SCRIPT_PARAM_U8(2);
    if (id < 3) {
        id = gSavemap.fieldParty[id];
        if (id != 0xff) {
            id = g_FieldCharEntity.charEntity[id];
            if (id != 0xff) {
                if (g_FieldEntityModel[id] != 0xff) {
                    Script_WriteOperand8(
                        2, 3, g_pFieldEntities[g_FieldEntityModel[id]].dir);
                }
            }
        }
    }
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * GETAI: reads the walkmesh triangle entity [2] stands on into v[3]/2. 4 bytes.
 *
 * Field opcode 0xB9 (FIELD_OP_GET_ENTITY_TRIANGLE).
 */
// FUNCTION: FF7 0x00618614
int __cdecl Script_OpGetEntityTriangle(void)
{
    unsigned char entity;

    entity = SCRIPT_PARAM_U8(2);
    if (g_FieldEntityModel[entity] != 0xff) {
        Script_WriteOperand16(2, 3,
                              g_pFieldEntities[g_FieldEntityModel[entity]].posI);
    }
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * GETAXY: reads entity [2]'s position into x v[3]/1 and y v[4]/2, in script units. 5 bytes.
 *
 * Field opcode 0xB8 (FIELD_OP_GET_ENTITY_XY).
 */
// FUNCTION: FF7 0x006186a6
int __cdecl Script_OpGetEntityXY(void)
{
    unsigned char entity;

    entity = SCRIPT_PARAM_U8(2);
    if (g_FieldEntityModel[entity] != 0xff) {
        Script_WriteOperand16(
            1, 3,
            g_pFieldEntities[g_FieldEntityModel[entity]].posX >> 12);
        Script_WriteOperand16(
            2, 4,
            g_pFieldEntities[g_FieldEntityModel[entity]].posY >> 12);
    }
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * AXYZI: reads entity [3]'s position and triangle into x v[4]/1, y v[5]/2, z v[6]/3 and triangle v[7]/4. 8
 * bytes.
 *
 * Field opcode 0xC1 (FIELD_OP_GET_ENTITY_XYZI).
 */
// FUNCTION: FF7 0x0061876b
int __cdecl Script_OpGetEntityXYZI(void)
{
    unsigned char entity;

    entity = SCRIPT_PARAM_U8(3);
    if (g_FieldEntityModel[entity] != 0xff) {
        Script_WriteOperand16(
            1, 4, g_pFieldEntities[g_FieldEntityModel[entity]].posX >> 12);
        Script_WriteOperand16(
            2, 5, g_pFieldEntities[g_FieldEntityModel[entity]].posY >> 12);
        Script_WriteOperand16(
            3, 6, g_pFieldEntities[g_FieldEntityModel[entity]].posZ >> 12);
        Script_WriteOperand16(
            4, 7, g_pFieldEntities[g_FieldEntityModel[entity]].posI);
    }
    g_FieldScriptPC[g_CurrentEntity] += 8;
    return 0;
}

/*
 * PXYZI: AXYZI for the character in party slot [3]. 8 bytes.
 *
 * BUG: a character in the party with no entity on this map has entity 0xFF, which is not tested: the model
 * test reads g_FieldEntityModel[0xFF], past the 48-entry table (see Script_OpMoveToPartyMember).
 *
 * Field opcode 0x75 (FIELD_OP_GET_PARTY_XYZI).
 */
// FUNCTION: FF7 0x00618892
int __cdecl Script_OpGetPartyXYZI(void)
{
    unsigned char id;

    id = SCRIPT_PARAM_U8(3);
    if (id < 3) {
        id = gSavemap.fieldParty[id];
        if (id < 9) {
            id = g_FieldCharEntity.charEntity[id];
            if (g_FieldEntityModel[id] != 0xff) {
                Script_WriteOperand16(
                    1, 4, g_pFieldEntities[g_FieldEntityModel[id]].posX >> 12);
                Script_WriteOperand16(
                    2, 5, g_pFieldEntities[g_FieldEntityModel[id]].posY >> 12);
                Script_WriteOperand16(
                    3, 6, g_pFieldEntities[g_FieldEntityModel[id]].posZ >> 12);
                Script_WriteOperand16(
                    4, 7, g_pFieldEntities[g_FieldEntityModel[id]].posI);
            }
        }
    }
    g_FieldScriptPC[g_CurrentEntity] += 8;
    return 0;
}

/*
 * VISI: shows ([1] nonzero) or hides this entity's model. 2 bytes.
 *
 * Field opcode 0xA4 (FIELD_OP_SET_VISIBLE).
 */
// FUNCTION: FF7 0x00618a01
int __cdecl Script_OpSetVisible(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] != 0xff) {
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].visible =
            SCRIPT_PARAM_U8(1);
    }
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * TLKON: [1] nonzero stops the player from talking to this model (talkOff), 0 allows it again. 2 bytes.
 *
 * Field opcode 0x7E (FIELD_OP_SET_TALK_OFF).
 */
// FUNCTION: FF7 0x00618a80
int __cdecl Script_OpSetTalkOff(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] != 0xff) {
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].talkOff =
            SCRIPT_PARAM_U8(1);
    }
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * XYZI: places this model at x v[3]/1, y v[5]/2, z v[7]/3 on walkmesh triangle v[9]/4, and yields for the
 * frame (also without a model). 11 bytes.
 *
 * Field opcode 0xA5 (FIELD_OP_SET_POSITION_XYZI).
 */
// FUNCTION: FF7 0x00618aff
int __cdecl Script_OpSetPositionXYZI(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] != 0xff) {
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].posX =
            Script_ReadOperand16(1, 3) << 12;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].posY =
            Script_ReadOperand16(2, 5) << 12;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].posZ =
            Script_ReadOperand16(3, 7) << 12;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].posI =
            Script_ReadOperand16(4, 9);
    }
    g_FieldScriptPC[g_CurrentEntity] += 0xb;
    return 1;
}

/*
 * XYZ: places this model at x v[3]/1, y v[5]/2, z v[7]/3, leaving its triangle alone, and yields for the
 * frame. 9 bytes.
 *
 * Field opcode 0xA7 (FIELD_OP_SET_POSITION_XYZ).
 */
// FUNCTION: FF7 0x00618c08
int __cdecl Script_OpSetPositionXYZ(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] != 0xff) {
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].posX =
            Script_ReadOperand16(1, 3) << 12;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].posY =
            Script_ReadOperand16(2, 5) << 12;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].posZ =
            Script_ReadOperand16(3, 7) << 12;
    }
    g_FieldScriptPC[g_CurrentEntity] += 9;
    return 1;
}

/*
 * XYI: places this model at x v[3]/1, y v[5]/2 on triangle v[7]/3, height left alone, and yields for the
 * frame. 9 bytes.
 *
 * Field opcode 0xA6 (FIELD_OP_SET_POSITION_XYI).
 */
// FUNCTION: FF7 0x00618ce5
int __cdecl Script_OpSetPositionXYI(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] != 0xff) {
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].posX =
            Script_ReadOperand16(1, 3) << 12;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].posY =
            Script_ReadOperand16(2, 5) << 12;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].posI =
            Script_ReadOperand16(3, 7);
    }
    g_FieldScriptPC[g_CurrentEntity] += 9;
    return 1;
}

/*
 * MESSAGE: shows dialogue [2] of this map's text in window [1] and waits until the player has read it and
 * the window has closed (Field_WindowShowMessage reports it). 3 bytes. The window is the one WINDOW
 * defined.
 *
 * Field opcode 0x40 (FIELD_OP_SHOW_MESSAGE).
 */
// FUNCTION: FF7 0x00618dbd
int __cdecl Script_OpShowMessage(void)
{
    if (Field_WindowShowMessage(SCRIPT_PARAM_U8(1),
                                SCRIPT_PARAM_U8(2)) != 0) {
        g_FieldScriptPC[g_CurrentEntity] += 3;
        return 0;
    }
    return 1;
}

/*
 * MPNAM: sets the location name the menu shows to dialogue [1] of this map's text (gSavemap.location). 2
 * bytes.
 *
 * Field opcode 0x43 (FIELD_OP_SET_LOCATION_NAME).
 */
// FUNCTION: FF7 0x00618e33
int __cdecl Script_OpSetLocationName(void)
{
    Field_BuildLocationName(SCRIPT_PARAM_U8(1));
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * ASK: asks a question in window [2] with dialogue [3] of this map's text, the cursor moving over lines
 * [4] (first) to [5] (last), and writes the line chosen to v[6]/2. Waits until the player confirms with
 * Circle, keeping player control off meanwhile and restoring UC's setting afterwards. The variable is
 * written every frame, so it always holds the line under the cursor. 7 bytes.
 *
 * Field opcode 0x48 (FIELD_OP_ASK_CHOICE).
 */
// FUNCTION: FF7 0x00618e83
int __cdecl Script_OpAskChoice(void)
{
    short choice;

    choice = Script_ReadOperand8(2, 6);
    if (Field_WindowUpdateChoice(SCRIPT_PARAM_U8(2), SCRIPT_PARAM_U8(3),
                                 SCRIPT_PARAM_U8(4), SCRIPT_PARAM_U8(5),
                                 &choice) != 0) {
        Script_WriteOperand8(2, 6, (unsigned char)choice);
        g_pFieldState->characterLock = g_FieldDialogueLock;
        g_FieldScriptPC[g_CurrentEntity] += 7;
        return 0;
    }
    Script_WriteOperand8(2, 6, (unsigned char)choice);
    g_pFieldState->characterLock = 1;
    return 1;
}

/*
 * WCLS: closes window [1] if it is open and waits until it is gone. 2 bytes.
 *
 * Field opcode 0x2E (FIELD_OP_CLOSE_WINDOW_AND_WAIT).
 */
// FUNCTION: FF7 0x00618f86
int __cdecl Script_OpCloseWindowAndWait(void)
{
    short window;

    window = SCRIPT_PARAM_U8(1);
    if (g_FieldWindowOwner[window] == 0xff) {
        g_FieldScriptPC[g_CurrentEntity] += 2;
        return 0;
    }
    Field_WindowIsClosed(window);
    Field_WindowShowMessage((unsigned char)window, 0);
    return 1;
}

/*
 * WSIZW: redefines window [1] with WINDOW's operands (10 bytes): a window this entity has open is closed
 * first and the opcode waits until it is gone; an unused window is defined at once. A window another
 * entity has open makes it wait.
 *
 * Field opcode 0x2F (FIELD_OP_REOPEN_WINDOW).
 */
// FUNCTION: FF7 0x0061900a
int __cdecl Script_OpReopenWindow(void)
{
    short window;

    window = SCRIPT_PARAM_U8(1);
    if (g_FieldWindowOwner[window] == 0xff) {
        return Script_OpWindowCreate();
    }
    if (g_FieldWindowOwner[window] == g_CurrentEntity) {
        Field_WindowIsClosed(window);
        Field_WindowShowMessage((unsigned char)window, 0);
    }
    return 1;
}

/*
 * WINDOW: defines message window [1] (0..3): x [2..3], y [4..5], width [6..7], height [8..9], literal
 * words. The window is kept 8 pixels inside the screen (Field_WindowSetRectClamped). 10 bytes.
 *
 * Field opcode 0x50 (FIELD_OP_WINDOW_CREATE).
 */
// FUNCTION: FF7 0x00619086
int __cdecl Script_OpWindowCreate(void)
{
    short rectH;
    short width;
    short posX;
    short posY;
    short windowId;

    windowId = SCRIPT_PARAM_U8(1);
    posX = SCRIPT_PARAM_U8(2);
    posX |= (short)(SCRIPT_PARAM_U8(3) << 8);
    posY = SCRIPT_PARAM_U8(4);
    posY |= (short)(SCRIPT_PARAM_U8(5) << 8);
    width = SCRIPT_PARAM_U8(6);
    width |= (short)(SCRIPT_PARAM_U8(7) << 8);
    rectH = SCRIPT_PARAM_U8(8);
    rectH |= (short)(SCRIPT_PARAM_U8(9) << 8);
    Field_WindowSetRectClamped((short)windowId, (short)posX, (short)posY, (short)width, (short)rectH);
    g_FieldScriptPC[g_CurrentEntity] += 0xA;
    return 0;
}

/* A message window's height for a number of text rows: 16 pixels a row and a 9-pixel border. */
#define FIELD_WINDOW_ROWS_HEIGHT(rows) (((rows) << 4) + 9)

/*
 * WROW: sets window [1]'s height to [2] text rows: FIELD_WINDOW_ROWS_HEIGHT, sixteen pixels a row plus a
 * nine-pixel border. 3 bytes.
 *
 * `movsx cx, byte ptr [...]` on the second operand, not movzx: the row count is read SIGNED, which is what
 * SCRIPT_PARAM_S8 exists for.
 *
 * Field opcode 0x55 (FIELD_OP_WINDOW_SET_ROWS).
 */
// FUNCTION: FF7 0x0061922b
int __cdecl Script_OpWindowSetRows(void)
{
    short rows;
    short w;

    w = SCRIPT_PARAM_U8(1);
    rows = SCRIPT_PARAM_S8(2);
    Field_WindowSetHeight(w, FIELD_WINDOW_ROWS_HEIGHT(rows));
    g_FieldScriptPC[g_CurrentEntity] += 3;
    return 0;
}

/*
 * WMOVE: moves window [1] by dx [2..3], dy [4..5] (literal signed words). 6 bytes. Named for a resize;
 * Field_WindowMove and the community mnemonic say move.
 *
 * Field opcode 0x51 (FIELD_OP_WINDOW_RESIZE).
 */
// FUNCTION: FF7 0x006192b4
int __cdecl Script_OpWindowResize(void)
{
    short width;
    short height;
    short id;

    id = SCRIPT_PARAM_U8(1);
    width = SCRIPT_PARAM_U8(2);
    width |= SCRIPT_PARAM_U8(3) << 8;
    height = SCRIPT_PARAM_U8(4);
    height |= SCRIPT_PARAM_U8(5) << 8;
    Field_WindowMove(id, width, height);
    g_FieldScriptPC[g_CurrentEntity] += 6;
    return 0;
}

/*
 * WREST (community mnemonic): resets window [1] (Field_WindowClose). 2 bytes.
 *
 * Field opcode 0x53 (FIELD_OP_WINDOW_CLOSE).
 */
// FUNCTION: FF7 0x006193b5
int __cdecl Script_OpWindowClose(void)
{
    short window;

    window = SCRIPT_PARAM_U8(1);
    Field_WindowClose(window);
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * WCLSE: closes window [1] and waits until it has closed.
 *
 * Returns 1 without advancing while the window is still open, which is how this VM spells `yield`; see
 * Script_OpYieldOneOpcode.
 *
 * Field opcode 0x54 (FIELD_OP_WINDOW_WAIT_CLOSED).
 */
// FUNCTION: FF7 0x00619411
int __cdecl Script_OpWindowWaitClosed(void)
{
    short window;

    window = SCRIPT_PARAM_U8(1);
    if (Field_WindowIsClosed(window) != 0) {
        g_FieldScriptPC[g_CurrentEntity] += 2;
        return 0;
    }
    return 1;
}

/*
 * WMODE: sets window [1]'s frame style [2] (by the community reference 0 normal, 1 no frame or background,
 * 2 a translucent background) and [3], which stops the player from closing it. 4 bytes.
 *
 * Field opcode 0x52 (FIELD_OP_WINDOW_SET_MODE).
 */
// FUNCTION: FF7 0x00619478
int __cdecl Script_OpWindowSetMode(void)
{
    short extra;
    short modeKind;
    short windowId;

    windowId = SCRIPT_PARAM_U8(1);
    modeKind = SCRIPT_PARAM_U8(2);
    extra = SCRIPT_PARAM_U8(3);
    Field_WindowSetMode(windowId, modeKind, extra);
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * AND: v[2]/1 &= v[3]/2 (bytes). 4 bytes.
 *
 * Field opcode 0x8F (FIELD_OP_AND_BYTE).
 */
// FUNCTION: FF7 0x00619522
int __cdecl Script_OpAndByte(void)
{
    Script_WriteOperand8(
        1, 2, Script_ReadOperand8(1, 2) & Script_ReadOperand8(2, 3));
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * AND2: v[2]/1 &= v[3]/2 (words). 5 bytes.
 *
 * Field opcode 0x90 (FIELD_OP_AND_WORD).
 */
// FUNCTION: FF7 0x00619582
int __cdecl Script_OpAndWord(void)
{
    Script_WriteOperand16(
        1, 2, Script_ReadOperand16(1, 2) & Script_ReadOperand16(2, 3));
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * OR: v[2]/1 |= v[3]/2 (bytes). 4 bytes.
 *
 * Field opcode 0x91 (FIELD_OP_OR_BYTE).
 */
// FUNCTION: FF7 0x006195db
int __cdecl Script_OpOrByte(void)
{
    Script_WriteOperand8(
        1, 2, Script_ReadOperand8(1, 2) | Script_ReadOperand8(2, 3));
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * OR2: v[2]/1 |= v[3]/2 (words). 5 bytes.
 *
 * Field opcode 0x92 (FIELD_OP_OR_WORD).
 */
// FUNCTION: FF7 0x0061963b
int __cdecl Script_OpOrWord(void)
{
    Script_WriteOperand16(
        1, 2, Script_ReadOperand16(1, 2) | Script_ReadOperand16(2, 3));
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * XOR: v[2]/1 ^= v[3]/2 (bytes). 4 bytes.
 *
 * Field opcode 0x93 (FIELD_OP_XOR_BYTE).
 */
// FUNCTION: FF7 0x00619694
int __cdecl Script_OpXorByte(void)
{
    Script_WriteOperand8(
        1, 2, Script_ReadOperand8(1, 2) ^ Script_ReadOperand8(2, 3));
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * XOR2: v[2]/1 ^= v[3]/2 (words). 5 bytes.
 *
 * Field opcode 0x94 (FIELD_OP_XOR_WORD).
 */
// FUNCTION: FF7 0x006196f4
int __cdecl Script_OpXorWord(void)
{
    Script_WriteOperand16(
        1, 2, Script_ReadOperand16(1, 2) ^ Script_ReadOperand16(2, 3));
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * PLUS: v[2]/1 += v[3]/2 (bytes, wrapping at 256). 4 bytes.
 *
 * Field opcode 0x85 (FIELD_OP_ADD_BYTE).
 */
// FUNCTION: FF7 0x0061974d
int __cdecl Script_OpAddByte(void)
{
    Script_WriteOperand8(
        1, 2, Script_ReadOperand8(1, 2) + Script_ReadOperand8(2, 3));
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * PLUS!: v[2]/1 += v[3]/2 (bytes), stopping at 255 instead of wrapping. 4 bytes.
 *
 * Field opcode 0x76 (FIELD_OP_ADD_BYTE_SAT).
 */
// FUNCTION: FF7 0x006197ad
int __cdecl Script_OpAddByteSat(void)
{
    short value;

    value = Script_ReadOperand8(1, 2) + Script_ReadOperand8(2, 3);
    if (value > 255) {
        value = 0xff;
    }
    Script_WriteOperand8(1, 2, value);
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * PLUS2: v[2]/1 += v[3]/2 (words, wrapping). 5 bytes.
 *
 * Field opcode 0x86 (FIELD_OP_ADD_WORD).
 */
// FUNCTION: FF7 0x00619829
int __cdecl Script_OpAddWord(void)
{
    Script_WriteOperand16(
        1, 2, Script_ReadOperand16(1, 2) + Script_ReadOperand16(2, 3));
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * PLUS2!: v[2]/1 += v[3]/2 (signed words), stopping at 32767; there is no lower limit. 5 bytes.
 *
 * Field opcode 0x77 (FIELD_OP_ADD_WORD_SAT).
 */
// FUNCTION: FF7 0x00619882
int __cdecl Script_OpAddWordSat(void)
{
    int value;

    value = Script_ReadOperand16(1, 2) + Script_ReadOperand16(2, 3);
    if (value > 32767) {
        value = 0x7fff;
    }
    Script_WriteOperand16(1, 2, value);
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * MINUS: v[2]/1 -= v[3]/2 (bytes, wrapping). 4 bytes.
 *
 * Field opcode 0x87 (FIELD_OP_SUB_BYTE).
 */
// FUNCTION: FF7 0x006198f6
int __cdecl Script_OpSubByte(void)
{
    Script_WriteOperand8(
        1, 2, Script_ReadOperand8(1, 2) - Script_ReadOperand8(2, 3));
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * MINUS!: v[2]/1 -= v[3]/2 (bytes), stopping at 0 instead of wrapping. 4 bytes.
 *
 * Field opcode 0x78 (FIELD_OP_SUB_BYTE_SAT).
 */
// FUNCTION: FF7 0x00619956
int __cdecl Script_OpSubByteSat(void)
{
    short value;

    value = Script_ReadOperand8(1, 2) - Script_ReadOperand8(2, 3);
    if (value < 0) {
        value = 0;
    }
    Script_WriteOperand8(1, 2, value);
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * MINUS2: v[2]/1 -= v[3]/2 (words, wrapping). 5 bytes.
 *
 * Field opcode 0x88 (FIELD_OP_SUB_WORD).
 */
// FUNCTION: FF7 0x006199cf
int __cdecl Script_OpSubWord(void)
{
    Script_WriteOperand16(
        1, 2, Script_ReadOperand16(1, 2) - Script_ReadOperand16(2, 3));
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * MINUS2!: meant as v[2]/1 -= v[3]/2 stopping at the signed word minimum. 5 bytes.
 *
 * BUG: the clamp is inverted, as in DEC2!: the words are read signed, the difference is an int, and every
 * difference below 0x8000 -- every ordinary one -- is replaced by 0x8000, so the variable becomes -32768.
 * Only a difference of 32768 or more (a large value minus a negative one) survives.
 *
 * Field opcode 0x79 (FIELD_OP_SUB_WORD_SAT).
 */
// FUNCTION: FF7 0x00619a28
int __cdecl Script_OpSubWordSat(void)
{
    int value;

    value = Script_ReadOperand16(1, 2) - Script_ReadOperand16(2, 3);
    // BUG: true for every ordinary difference, so the result is -32768.
    if (value < 32768) {
        value = 0x8000;
    }
    Script_WriteOperand16(1, 2, value);
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * MUL: v[2]/1 *= v[3]/2 (bytes, wrapping). 4 bytes.
 *
 * Field opcode 0x89 (FIELD_OP_MUL_BYTE).
 */
// FUNCTION: FF7 0x00619a9c
int __cdecl Script_OpMulByte(void)
{
    Script_WriteOperand8(
        1, 2, Script_ReadOperand8(1, 2) * Script_ReadOperand8(2, 3));
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * MUL2: v[2]/1 *= v[3]/2 (words, wrapping). 5 bytes.
 *
 * Field opcode 0x8A (FIELD_OP_MUL_WORD).
 */
// FUNCTION: FF7 0x00619afd
int __cdecl Script_OpMulWord(void)
{
    Script_WriteOperand16(
        1, 2, Script_ReadOperand16(1, 2) * Script_ReadOperand16(2, 3));
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * DIV: v[2]/1 /= v[3]/2 (bytes). 4 bytes. A zero divisor is not checked: on PC it is an integer divide
 * fault.
 *
 * Field opcode 0x8B (FIELD_OP_DIV_BYTE).
 */
// FUNCTION: FF7 0x00619b57
int __cdecl Script_OpDivByte(void)
{
    Script_WriteOperand8(
        1, 2, Script_ReadOperand8(1, 2) / Script_ReadOperand8(2, 3));
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * DIV2: v[2]/1 /= v[3]/2 (signed words). 5 bytes. A zero divisor is not checked.
 *
 * Field opcode 0x8C (FIELD_OP_DIV_WORD).
 */
// FUNCTION: FF7 0x00619bbe
int __cdecl Script_OpDivWord(void)
{
    Script_WriteOperand16(
        1, 2, Script_ReadOperand16(1, 2) / Script_ReadOperand16(2, 3));
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * MOD: v[2]/1 = v[2]/1 % v[3]/2 (bytes). 4 bytes. A zero divisor is not checked.
 *
 * Field opcode 0x8D (FIELD_OP_MOD_BYTE).
 */
// FUNCTION: FF7 0x00619c1b
int __cdecl Script_OpModByte(void)
{
    Script_WriteOperand8(
        1, 2, Script_ReadOperand8(1, 2) % Script_ReadOperand8(2, 3));
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * MOD2: v[2]/1 = v[2]/1 % v[3]/2 (signed words). 5 bytes. A zero divisor is not checked.
 *
 * Field opcode 0x8E (FIELD_OP_MOD_WORD).
 */
// FUNCTION: FF7 0x00619c82
int __cdecl Script_OpModWord(void)
{
    Script_WriteOperand16(
        1, 2, Script_ReadOperand16(1, 2) % Script_ReadOperand16(2, 3));
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * INC: adds one to the byte variable v[2]/2, wrapping at 256. 3 bytes.
 *
 * Field opcode 0x95 (FIELD_OP_INC_BYTE).
 */
// FUNCTION: FF7 0x00619cdf
int __cdecl Script_OpIncByte(void)
{
    Script_WriteOperand8(2, 2, Script_ReadOperand8(2, 2) + 1);
    g_FieldScriptPC[g_CurrentEntity] += 3;
    return 0;
}

/*
 * INC!: adds one to the byte variable v[2]/2, stopping at 255. 3 bytes.
 *
 * Field opcode 0x7A (FIELD_OP_INC_BYTE_SAT).
 */
// FUNCTION: FF7 0x00619d2a
int __cdecl Script_OpIncByteSat(void)
{
    short value;

    value = Script_ReadOperand8(2, 2) + 1;
    if (value > 255) {
        value = 0xff;
    }
    Script_WriteOperand8(2, 2, value);
    g_FieldScriptPC[g_CurrentEntity] += 3;
    return 0;
}

/*
 * INC2: adds one to the word variable v[2]/2, wrapping. 3 bytes.
 *
 * Field opcode 0x96 (FIELD_OP_INC_WORD).
 */
// FUNCTION: FF7 0x00619d91
int __cdecl Script_OpIncWord(void)
{
    Script_WriteOperand16(2, 2, Script_ReadOperand16(2, 2) + 1);
    g_FieldScriptPC[g_CurrentEntity] += 3;
    return 0;
}

/*
 * INC2!: adds one to the signed word variable v[2]/2, stopping at 32767. 3 bytes.
 *
 * Field opcode 0x7B (FIELD_OP_INC_WORD_SAT).
 */
// FUNCTION: FF7 0x00619dda
int __cdecl Script_OpIncWordSat(void)
{
    int value;

    value = Script_ReadOperand16(2, 2) + 1;
    if (value > 32767) {
        value = 0x7fff;
    }
    Script_WriteOperand16(2, 2, value);
    g_FieldScriptPC[g_CurrentEntity] += 3;
    return 0;
}

/*
 * DEC: subtracts one from the byte variable v[2]/2, wrapping at 0. 3 bytes.
 *
 * Field opcode 0x97 (FIELD_OP_DEC_BYTE).
 */
// FUNCTION: FF7 0x00619e3e
int __cdecl Script_OpDecByte(void)
{
    Script_WriteOperand8(2, 2, Script_ReadOperand8(2, 2) - 1);
    g_FieldScriptPC[g_CurrentEntity] += 3;
    return 0;
}

/*
 * DEC!: subtracts one from the byte variable v[2]/2, stopping at 0. 3 bytes.
 *
 * Field opcode 0x7C (FIELD_OP_DEC_BYTE_SAT).
 */
// FUNCTION: FF7 0x00619e89
int __cdecl Script_OpDecByteSat(void)
{
    short value;

    value = Script_ReadOperand8(2, 2) - 1;
    if (value < 0) {
        value = 0;
    }
    Script_WriteOperand8(2, 2, value);
    g_FieldScriptPC[g_CurrentEntity] += 3;
    return 0;
}

/*
 * DEC2: subtracts one from the word variable v[2]/2, wrapping. 3 bytes.
 *
 * Field opcode 0x98 (FIELD_OP_DEC_WORD).
 */
// FUNCTION: FF7 0x00619eed
int __cdecl Script_OpDecWord(void)
{
    Script_WriteOperand16(2, 2, Script_ReadOperand16(2, 2) - 1);
    g_FieldScriptPC[g_CurrentEntity] += 3;
    return 0;
}

/*
 * DEC2!: meant to subtract one from the word variable v[2]/2, stopping at the signed minimum. 3 bytes.
 *
 * BUG: the clamp is inverted: the operand is read as a SIGNED short, so `value` can never reach 0x8000 and
 * the branch is always taken -- DEC2! always writes -32768, whatever the variable held. Reproduced, not
 * fixed.
 *
 * `cmp value, 0x8000; jge` is `value < 0x8000`, so the source said `<` against 0x8000 rather than the
 * PSX's `<=` against 0x7FFF; the two conditions are equal and the compilers picked different encodings.
 *
 * Field opcode 0x7D (FIELD_OP_DEC_WORD_SAT).
 */
// FUNCTION: FF7 0x00619f36
int __cdecl Script_OpDecWordSat(void)
{
    int value;

    value = Script_ReadOperand16(2, 2) - 1;
    // BUG: always true, so the result is -32768.
    if (value < 32768) {
        value = 0x8000;
    }
    Script_WriteOperand16(2, 2, value);
    g_FieldScriptPC[g_CurrentEntity] += 3;
    return 0;
}

/*
 * RANDOM: writes a "random" byte to v[2]/2. Not random at all: the next entry of a fixed 256-byte table,
 * the index stepped by the stride RDMSD last set. 3 bytes.
 *
 * `add al, [0x905e50]` is 8-bit, so the source used `+=`.
 *
 * Field opcode 0x99 (FIELD_OP_RANDOM_BYTE).
 */
// FUNCTION: FF7 0x00619f9a
int __cdecl Script_OpRandomByte(void)
{
    g_FieldRandomIndex += g_FieldRandomStep;
    Script_WriteOperand8(2, 2, g_FieldRandomTable[g_FieldRandomIndex]);
    g_FieldScriptPC[g_CurrentEntity] += 3;
    return 0;
}

/*
 * RDMSD: sets the stride RANDOM steps through its table to v[2]/2 * 16 + 1. The stride is odd, so all 256
 * entries come up before the sequence repeats. 3 bytes.
 *
 * Field opcode 0x7F (FIELD_OP_SET_RANDOM_STRIDE).
 */
// FUNCTION: FF7 0x00619fef
int __cdecl Script_OpSetRandomStride(void)
{
    g_FieldRandomStep = (Script_ReadOperand8(2, 2) << 4) + 1;
    g_FieldScriptPC[g_CurrentEntity] += 3;
    return 0;
}

/*
 * BGON: switches on state bit v[3]/2 of background parameter v[2]/1, showing the animated background tiles
 * drawn under that state. 4 bytes. Bit numbers above 7 do nothing.
 *
 * Field opcode 0xE0 (FIELD_OP_BG_ON).
 */
// FUNCTION: FF7 0x0061a035
int __cdecl Script_OpBgOn(void)
{
    int unused;
    unsigned char *flag;

    flag = &g_pFieldState->backgroundLayerVisibility[Script_ReadOperand8(1, 2)];
    *flag |= 1 << Script_ReadOperand8(2, 3);
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * BGOFF: switches off state bit v[3]/2 of background parameter v[2]/1, hiding the tiles of that state. 4
 * bytes. The mask is complemented in eight bits, so bit numbers above 7 clear nothing (as BGON sets
 * nothing).
 *
 * Field opcode 0xE1 (FIELD_OP_BG_OFF).
 */
// FUNCTION: FF7 0x0061a0ac
int __cdecl Script_OpBgOff(void)
{
    unsigned char *flag;

    flag = &g_pFieldState->backgroundLayerVisibility[Script_ReadOperand8(1, 2)];
    *flag &= (1 << Script_ReadOperand8(2, 3)) ^ 0xff;
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * BGCLR: switches off every state of background parameter v[2]/2. 3 bytes.
 *
 * Field opcode 0xE4 (FIELD_OP_BG_CLR).
 */
// FUNCTION: FF7 0x0061a126
int __cdecl Script_OpBgClr(void)
{
    g_pFieldState->backgroundLayerVisibility[Script_ReadOperand8(2, 2)] = 0;
    g_FieldScriptPC[g_CurrentEntity] += 3;
    return 0;
}

/*
 * BGROL: moves background parameter v[2]/2 on to its next state: the state bits shift one place left. 3
 * bytes.
 *
 * Field opcode 0xE2 (FIELD_OP_BG_ROL).
 */
// FUNCTION: FF7 0x0061a170
int __cdecl Script_OpBgRol(void)
{
    unsigned char *flag;

    flag = &g_pFieldState->backgroundLayerVisibility[Script_ReadOperand8(2, 2)];
    *flag <<= 1;
    g_FieldScriptPC[g_CurrentEntity] += 3;
    return 0;
}

/*
 * BGROL2: moves background parameter v[2]/2 back to its previous state: the state bits shift one place
 * right. 3 bytes.
 *
 * Field opcode 0xE3 (FIELD_OP_BG_ROL_2).
 */
// FUNCTION: FF7 0x0061a1ca
int __cdecl Script_OpBgRol2(void)
{
    unsigned char *flag;

    flag = &g_pFieldState->backgroundLayerVisibility[Script_ReadOperand8(2, 2)];
    *flag >>= 1;
    g_FieldScriptPC[g_CurrentEntity] += 3;
    return 0;
}

/*
 * PMVIE: names movie [1] as the one MOVIE plays next and waits until the engine has it ready. 2 bytes. A
 * field request (FIELD_REQUEST_LOAD_MOVIE). While g_FieldMoviePlaying is set (SPECIAL MVLCK) it just steps
 * over itself.
 *
 * Field opcode 0xF8 (FIELD_OP_MOVIE_OPEN).
 */
// FUNCTION: FF7 0x0061a224
int __cdecl Script_OpMovieOpen(void)
{
    // Movies held off (SPECIAL MVLCK): step over.
    if (g_FieldMoviePlaying != 0) {
        g_FieldScriptPC[g_CurrentEntity] += 2;
        return 0;
    }
    switch (g_pFieldState->requestCode) {
    case FIELD_REQUEST_LOAD_MOVIE:
        switch (g_pFieldState->requestArg) {
        case FIELD_REQUEST_PROGRESS_DONE:
            g_pFieldState->requestCode = FIELD_REQUEST_NONE;
            g_pFieldState->requestArg = FIELD_REQUEST_PROGRESS_PENDING;
            g_FieldScriptPC[g_CurrentEntity] += 2;
            return 0;
        case FIELD_REQUEST_PROGRESS_RUNNING:
            return 1;
        }
        break;
    case FIELD_REQUEST_NONE:
        g_pFieldState->requestCode = FIELD_REQUEST_LOAD_MOVIE;
        g_pFieldState->eventCmdParam = (short)SCRIPT_PARAM_U8(1);
        g_pFieldState->requestArg = FIELD_REQUEST_PROGRESS_PENDING;
        break;
    }
    return 1;
}

/*
 * MOVIE: plays the movie PMVIE named and waits until it has ended. 1 byte. A field request
 * (FIELD_REQUEST_PLAY_MOVIE); a stop requested through MENU (FIELD_REQUEST_STOP_MOVIE) ends the wait as
 * well. Every run hides the hand cursor (g_FieldMovieRequested). While g_FieldMoviePlaying is set (SPECIAL
 * MVLCK) it steps over itself and restarts the frame counter MVIEF reads.
 *
 * Field opcode 0xF9 (FIELD_OP_MOVIE_PLAY).
 */
// FUNCTION: FF7 0x0061a321
int __cdecl Script_OpMoviePlay(void)
{
    g_FieldMovieRequested = 1;
    // Movies held off (SPECIAL MVLCK): step over.
    if (g_FieldMoviePlaying != 0) {
        g_FieldMovieFrame = 0;
        g_FieldScriptPC[g_CurrentEntity] += 1;
        return 0;
    }
    switch (g_pFieldState->requestCode) {
    case FIELD_REQUEST_PLAY_MOVIE:
        switch (g_pFieldState->requestArg) {
        case FIELD_REQUEST_PROGRESS_DONE:
            g_pFieldState->requestCode = FIELD_REQUEST_NONE;
            g_pFieldState->requestArg = FIELD_REQUEST_PROGRESS_PENDING;
            g_FieldScriptPC[g_CurrentEntity] += 1;
            return 0;
        case FIELD_REQUEST_PROGRESS_RUNNING:
            return 1;
        }
        break;
    case FIELD_REQUEST_STOP_MOVIE:
        g_FieldScriptPC[g_CurrentEntity] += 1;
        return 0;
    case FIELD_REQUEST_NONE:
        g_pFieldState->requestCode = FIELD_REQUEST_PLAY_MOVIE;
        g_pFieldState->requestArg = FIELD_REQUEST_PROGRESS_PENDING;
        break;
    }
    return 1;
}

/*
 * MVIEF: writes the frame the movie has reached to v[2]/2. 3 bytes. While g_FieldMoviePlaying is set
 * (SPECIAL MVLCK), a counter that climbs by one per call stands in for it, so a script waiting for a frame
 * still finishes.
 *
 * Field opcode 0xFA (FIELD_OP_GET_MOVIE_FRAME).
 */
// FUNCTION: FF7 0x0061a438
int __cdecl Script_OpGetMovieFrame(void)
{
    // Movies held off: a counter stands in for the frame.
    if (g_FieldMoviePlaying != 0) {
        Script_WriteOperand16(2, 2, g_FieldMovieFrame);
        g_FieldMovieFrame++;
        g_FieldScriptPC[g_CurrentEntity] += 3;
        return 0;
    } else {
        Script_WriteOperand16(2, 2, g_pFieldState->currentMovieFrame);
        g_FieldScriptPC[g_CurrentEntity] += 3;
        return 0;
    }
}

/*
 * MPJPO: switches the map's gateways off ([1] nonzero) or on, so the player can walk over an exit without
 * leaving. 2 bytes.
 *
 * Field opcode 0xD2 (FIELD_OP_SET_GATEWAYS_DISABLED).
 */
// FUNCTION: FF7 0x0061a4d4
int __cdecl Script_OpSetGatewaysDisabled(void)
{
    g_pFieldState->gatewaysDisabled = SCRIPT_PARAM_U8(1);
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * SCR2D: scrolls the background straight to x v[2]/1, y v[4]/2 and fixes it there
 * (FIELD_CAMERA_SCROLL_FIXED). 6 bytes.
 *
 * Field opcode 0x64 (FIELD_OP_SCROLL_TO_POINT).
 */
// FUNCTION: FF7 0x0061a522
int __cdecl Script_OpScrollToPoint(void)
{
    g_pFieldState->cameraScrollMode = FIELD_CAMERA_SCROLL_FIXED;
    g_pFieldState->cameraScrollTargetX = Script_ReadOperand16(1, 2);
    g_pFieldState->cameraScrollTargetY = Script_ReadOperand16(2, 4);
    g_pFieldState->cameraScrollState = FIELD_CAMERA_SCROLL_STATE_REQUESTED;
    g_FieldScriptPC[g_CurrentEntity] += 6;
    return 0;
}

/*
 * SCRLC: scrolls the camera back to the player over v[2]/2 steps, with the scroll kind [4]
 * (FIELD_CAMERA_SCROLL_*). Does not wait; SCRLW waits and then leaves the camera following the player. 5
 * bytes.
 *
 * Field opcode 0x62 (FIELD_OP_SCROLL_TO_PLAYER).
 */
// FUNCTION: FF7 0x0061a58a
int __cdecl Script_OpScrollToPlayer(void)
{
    g_pFieldState->cameraScrollMode = SCRIPT_PARAM_U8(4);
    g_pFieldState->cameraScrollTargetId = (unsigned char)g_pFieldState->playerModel;
    g_pFieldState->cameraScrollNumSteps = Script_ReadOperand16(2, 2);
    g_pFieldState->cameraScrollState = FIELD_CAMERA_SCROLL_STATE_REQUESTED;
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * SCRLA: scrolls the camera to entity [4] over v[2]/2 steps, scroll kind [5]; SCRLW then leaves the camera
 * following that entity. An entity without a model leaves the camera alone. 6 bytes.
 *
 * Field opcode 0x63 (FIELD_OP_SCROLL_TO_ENTITY).
 */
// FUNCTION: FF7 0x0061a607
int __cdecl Script_OpScrollToEntity(void)
{
    short entity;

    entity = SCRIPT_PARAM_U8(4);
    if (g_FieldEntityModel[entity] != 0xff) {
        g_pFieldState->cameraScrollMode = SCRIPT_PARAM_U8(5);
        g_pFieldState->cameraScrollTargetId = g_FieldEntityModel[entity];
        g_pFieldState->cameraScrollNumSteps = Script_ReadOperand16(2, 2);
        g_pFieldState->cameraScrollState = FIELD_CAMERA_SCROLL_STATE_REQUESTED;
    }
    g_FieldScriptPC[g_CurrentEntity] += 6;
    return 0;
}

/*
 * SCRLP: SCRLA for the character in party slot [4]. 6 bytes.
 *
 * BUG: an empty slot is turned into entity 0xFF and then indexes g_FieldEntityModel[0xFF], past the
 * 48-entry table (see Script_OpMoveToPartyMember).
 *
 * Field opcode 0x6F (FIELD_OP_SCROLL_TO_PARTY_MEMBER).
 */
// FUNCTION: FF7 0x0061a6bd
int __cdecl Script_OpScrollToPartyMember(void)
{
    unsigned char partyId;
    short actorId;

    partyId = gSavemap.fieldParty[SCRIPT_PARAM_U8(4)];
    if (partyId == PARTY_CHAR_NONE) {
        actorId = 0xff;
    } else {
        actorId = g_FieldCharEntity.charEntity[partyId];
    }
    if (g_FieldEntityModel[actorId] != 0xff) {
        g_pFieldState->cameraScrollMode = SCRIPT_PARAM_U8(5);
        g_pFieldState->cameraScrollTargetId = g_FieldEntityModel[actorId];
        g_pFieldState->cameraScrollNumSteps = Script_ReadOperand16(2, 2);
        g_pFieldState->cameraScrollState = FIELD_CAMERA_SCROLL_STATE_REQUESTED;
    }
    g_FieldScriptPC[g_CurrentEntity] += 6;
    return 0;
}

/*
 * SCRCC: puts the camera back on the player at once (FIELD_CAMERA_SCROLL_RESET). 1 byte.
 *
 * The one-byte member of the scroll family: no operands, it just puts the camera back on mode 0.
 *
 * Field opcode 0x65 (FIELD_OP_SCROLL_RESET).
 */
// FUNCTION: FF7 0x0061a7a9
int __cdecl Script_OpScrollReset(void)
{
    g_pFieldState->cameraScrollMode = FIELD_CAMERA_SCROLL_RESET;
    g_pFieldState->cameraScrollTargetId = (unsigned char)g_pFieldState->playerModel;
    g_pFieldState->cameraScrollState = FIELD_CAMERA_SCROLL_STATE_REQUESTED;
    g_FieldScriptPC[g_CurrentEntity] += 1;
    return 0;
}

/*
 * SCR2DC: scrolls the background to x v[3]/1, y v[5]/2 over v[7]/4 steps, easing in and out. Does not
 * wait; SCRLW waits and then fixes the camera there. 9 bytes.
 *
 * The camera steps the mode it sets (FIELD_CAMERA_SCROLL_TO_POINT_SMOOTH) with Field_InterpolateCosine
 * (Field_StepCameraScrollTransition), so this opcode scrolls eased, not linearly as its name says;
 * Script_OpScrollToPointSmooth is the linear one.
 *
 * Field opcode 0x66 (FIELD_OP_SCROLL_TO_POINT_LINEAR).
 */
// FUNCTION: FF7 0x0061a7f9
int __cdecl Script_OpScrollToPointLinear(void)
{
    g_pFieldState->cameraScrollMode = FIELD_CAMERA_SCROLL_TO_POINT_SMOOTH;
    g_pFieldState->cameraScrollTargetX = Script_ReadOperand16(1, 3);
    g_pFieldState->cameraScrollTargetY = Script_ReadOperand16(2, 5);
    g_pFieldState->cameraScrollNumSteps = Script_ReadOperand16(4, 7);
    g_pFieldState->cameraScrollState = FIELD_CAMERA_SCROLL_STATE_REQUESTED;
    g_FieldScriptPC[g_CurrentEntity] += 9;
    return 0;
}

/*
 * SCR2DL: scrolls the background to x v[3]/1, y v[5]/2 over v[7]/4 steps at a constant rate. Does not
 * wait; SCRLW waits and then fixes the camera there. 9 bytes.
 *
 * The camera steps the mode it sets (FIELD_CAMERA_SCROLL_TO_POINT_LINEAR) with Field_InterpolateLinear, so
 * this opcode scrolls linearly despite its name.
 *
 * Field opcode 0x68 (FIELD_OP_SCROLL_TO_POINT_SMOOTH).
 */
// FUNCTION: FF7 0x0061a878
int __cdecl Script_OpScrollToPointSmooth(void)
{
    g_pFieldState->cameraScrollMode = FIELD_CAMERA_SCROLL_TO_POINT_LINEAR;
    g_pFieldState->cameraScrollTargetX = Script_ReadOperand16(1, 3);
    g_pFieldState->cameraScrollTargetY = Script_ReadOperand16(2, 5);
    g_pFieldState->cameraScrollNumSteps = Script_ReadOperand16(4, 7);
    g_pFieldState->cameraScrollState = FIELD_CAMERA_SCROLL_STATE_REQUESTED;
    g_FieldScriptPC[g_CurrentEntity] += 9;
    return 0;
}

/*
 * SCRLW: waits until the camera scroll has arrived. A scroll to an entity then leaves the camera following
 * it, a scroll to a point leaves it fixed there. 1 byte.
 *
 * Field opcode 0x67 (FIELD_OP_WAIT_SCROLL).
 */
// FUNCTION: FF7 0x0061a8f7
int __cdecl Script_OpWaitScroll(void)
{
    switch (g_pFieldState->cameraScrollState) {
    case FIELD_CAMERA_SCROLL_STATE_DONE:
        switch (g_pFieldState->cameraScrollMode) {
        case FIELD_CAMERA_SCROLL_FOLLOW:
        case FIELD_CAMERA_SCROLL_TO_TARGET_LINEAR:
        case FIELD_CAMERA_SCROLL_TO_TARGET_SMOOTH:
            g_pFieldState->cameraScrollMode = FIELD_CAMERA_SCROLL_FOLLOW;
            break;
        case FIELD_CAMERA_SCROLL_TO_POINT_LINEAR:
        case FIELD_CAMERA_SCROLL_TO_POINT_SMOOTH:
            g_pFieldState->cameraScrollMode = FIELD_CAMERA_SCROLL_FIXED;
            break;
        }
        g_pFieldState->cameraScrollState = FIELD_CAMERA_SCROLL_STATE_REQUESTED;
        g_FieldScriptPC[g_CurrentEntity] += 1;
        return 0;
    }
    return 1;
}

/*
 * STPAL: copies [4] + 1 colours of the field's palette row v[2]/1 (CLUT row 480 + n) into slot v[3]/2 of
 * the script's palette store, where the palette opcodes work on them. 5 bytes.
 *
 * Field opcode 0xE5 (FIELD_OP_PALETTE_STORE).
 */
// FUNCTION: FF7 0x0061a99d
int __cdecl Script_OpPaletteStore(void)
{
    int rect[4];

    Field_ResetPaletteRect(rect);
    rect[1] = Script_ReadOperand8(1, 2) + 0x1e0;
    rect[0] = 0;
    rect[2] = SCRIPT_PARAM_U8(4) + 1;
    rect[3] = 1;
    Field_ReadPaletteRect(
        (FieldPaletteRect *)rect, (unsigned char *)g_FieldPalettes[Script_ReadOperand8(2, 3)]);
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * STPLS: STPAL from colour [3] on, all literal: palette row [1] into store slot [2], [4] + 1 colours. 5
 * bytes.
 *
 * Field opcode 0xEB (FIELD_OP_PALETTE_STORE_PARTIAL).
 */
// FUNCTION: FF7 0x0061aa48
int __cdecl Script_OpPaletteStorePartial(void)
{
    int rect[4];
    unsigned char col;

    Field_ResetPaletteRect(rect);
    rect[1] = SCRIPT_PARAM_U8(1) + 0x1e0;
    rect[0] = SCRIPT_PARAM_U8(3);
    col = rect[0];
    rect[2] = SCRIPT_PARAM_U8(4) + 1;
    rect[3] = 1;
    Field_ReadPaletteRect(
        (FieldPaletteRect *)rect, (unsigned char *)&g_FieldPalettes[SCRIPT_PARAM_U8(2)][col]);
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * LDPAL: copies [4] + 1 colours of store slot v[2]/1 back to the field's palette row v[3]/2, which is what
 * makes a palette change visible. 5 bytes.
 *
 * Field opcode 0xE6 (FIELD_OP_PALETTE_LOAD).
 */
// FUNCTION: FF7 0x0061ab37
int __cdecl Script_OpPaletteLoad(void)
{
    int rect[4];

    Field_ResetPaletteRect(rect);
    rect[1] = Script_ReadOperand8(2, 3) + 0x1e0;
    rect[0] = 0;
    rect[2] = SCRIPT_PARAM_U8(4) + 1;
    rect[3] = 1;
    Field_WritePaletteRect(
        (FieldPaletteRect *)rect, (unsigned char *)g_FieldPalettes[Script_ReadOperand8(1, 2)]);
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * LDPLS: LDPAL from colour [3] on, all literal: store slot [1] to palette row [2], [4] + 1 colours. 5
 * bytes.
 *
 * Field opcode 0xEC (FIELD_OP_PALETTE_LOAD_PARTIAL).
 */
// FUNCTION: FF7 0x0061abe2
int __cdecl Script_OpPaletteLoadPartial(void)
{
    int rect[4];
    unsigned char col;

    Field_ResetPaletteRect(rect);
    rect[1] = SCRIPT_PARAM_U8(2) + 0x1e0;
    rect[0] = SCRIPT_PARAM_U8(3);
    col = rect[0];
    rect[2] = SCRIPT_PARAM_U8(4) + 1;
    rect[3] = 1;
    Field_WritePaletteRect(
        (FieldPaletteRect *)rect, (unsigned char *)&g_FieldPalettes[SCRIPT_PARAM_U8(1)][col]);
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * Clears a palette rectangle (x, y, width, height) before the STPAL and LDPAL families fill it in.
 *
 * Four separate stores, each reloading the pointer -- four assignments, not a struct copy or a loop.
 */
// FUNCTION: FF7 0x0061acd1
void __cdecl Field_ResetPaletteRect(int *rect)
{
    rect[0] = 0;
    rect[1] = 0;
    rect[2] = 0;
    rect[3] = 0;
}

/*
 * CPPAL: copies [4] + 1 colours of store slot v[2]/1 to store slot v[3]/2. 5 bytes.
 *
 * Field opcode 0xE7 (FIELD_OP_PALETTE_COPY).
 */
// FUNCTION: FF7 0x0061acfd
int __cdecl Script_OpPaletteCopy(void)
{
    unsigned char source;
    unsigned char dst;
    short len;
    short n;

    len = SCRIPT_PARAM_U8(4) + 1;
    source = Script_ReadOperand8(1, 2);
    dst = Script_ReadOperand8(2, 3);
    for (n = 0; n < len; n++) {
        g_FieldPalettes[dst][n] = g_FieldPalettes[source][n];
    }
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * CPPAL2: copies v[7]/4 + 1 colours from store slot [3], colour v[5]/1 on, to store slot [4], colour
 * v[6]/2 on. 8 bytes. A copy within one slot runs forwards, so overlapping ranges can smear.
 *
 * Field opcode 0xED (FIELD_OP_PALETTE_COPY_RANGE).
 */
// FUNCTION: FF7 0x0061adc2
int __cdecl Script_OpPaletteCopyRange(void)
{
    unsigned char readPal;
    unsigned char dest;
    short count;
    short dstIndex;
    short srcIdx;

    count = Script_ReadOperand8(4, 7) + 1;
    readPal = SCRIPT_PARAM_U8(3);
    dest = SCRIPT_PARAM_U8(4);
    srcIdx = Script_ReadOperand8(1, 5);
    dstIndex = Script_ReadOperand8(2, 6);
    count += srcIdx;
    for (; srcIdx < count; srcIdx++, dstIndex++) {
        g_FieldPalettes[dest][dstIndex] = g_FieldPalettes[readPal][srcIdx];
    }
    g_FieldScriptPC[g_CurrentEntity] += 8;
    return 0;
}

/*
 * RTPAL: rotates [6] + 1 colours of store slot v[3]/1 by v[5]/4 places into store slot v[4]/2. 7 bytes.
 *
 * The two copy loops are inclusive (<= count, count already [6] + 1), so together they cover [6] + 2
 * colours, one more than the other palette opcodes, and write the colour at the rotation point twice. With
 * the same slot as source and destination there is no temporary, so the rotation smears.
 *
 * Field opcode 0xE8 (FIELD_OP_PALETTE_ROTATE).
 */
// FUNCTION: FF7 0x0061aed3
int __cdecl Script_OpPaletteRotate(void)
{
    short roll;
    unsigned char readPal;
    unsigned char dest;
    short count;
    short dstIndex;
    short srcIdx;

    count = SCRIPT_PARAM_U8(6) + 1;
    readPal = Script_ReadOperand8(1, 3);
    dest = Script_ReadOperand8(2, 4);
    roll = Script_ReadOperand8(4, 5);
    srcIdx = 0;
    dstIndex = roll;
    for (; dstIndex <= count; srcIdx++, dstIndex++) {
        g_FieldPalettes[dest][dstIndex] = g_FieldPalettes[readPal][srcIdx];
    }
    srcIdx = count - roll;
    dstIndex = 0;
    for (; srcIdx <= count; srcIdx++, dstIndex++) {
        g_FieldPalettes[dest][dstIndex] = g_FieldPalettes[readPal][srcIdx];
    }
    g_FieldScriptPC[g_CurrentEntity] += 7;
    return 0;
}

/*
 * RTPAL2: RTPAL over part of a slot: from store slot [3] to slot [4], start colours v[5]/1 and v[6]/2,
 * v[7]/4 + 1 colours, with the same inclusive loops and no temporary. 8 bytes.
 *
 * Field opcode 0xEE (FIELD_OP_PALETTE_ROTATE_RANGE).
 */
// FUNCTION: FF7 0x0061b02c
int __cdecl Script_OpPaletteRotateRange(void)
{
    unsigned char readPal;
    unsigned char dest;
    short count;
    short readIdx;
    short writeIdx;
    short dstStart;
    short srcStart;

    count = Script_ReadOperand8(4, 7) + 1;
    readPal = SCRIPT_PARAM_U8(3);
    dest = SCRIPT_PARAM_U8(4);
    srcStart = Script_ReadOperand8(1, 5);
    dstStart = Script_ReadOperand8(2, 6);
    count += srcStart;
    readIdx = srcStart;
    writeIdx = dstStart;
    for (; writeIdx <= count; readIdx++, writeIdx++) {
        g_FieldPalettes[dest][writeIdx] = g_FieldPalettes[readPal][readIdx];
    }
    readIdx = count - dstStart;
    writeIdx = srcStart;
    for (; readIdx <= count; readIdx++, writeIdx++) {
        g_FieldPalettes[dest][writeIdx] = g_FieldPalettes[readPal][readIdx];
    }
    g_FieldScriptPC[g_CurrentEntity] += 8;
    return 0;
}

/*
 * ADPAL: adds a colour to [9] + 1 colours of store slot v[4]/1, results into slot v[5]/2: signed deltas
 * (-128..127) blue v[6]/3, green v[7]/4, red v[8]/5, each channel clamped to 0..31. 10 bytes.
 *
 * A colour that comes out 0, which the PlayStation draws transparent, gets the 0x8000 bit so it stays
 * black -- unless it was 0 to start with.
 *
 * Field opcode 0xE9 (FIELD_OP_PALETTE_ADD).
 */
// FUNCTION: FF7 0x0061b1bc
int __cdecl Script_OpPaletteAdd(void)
{
    short bDelta;
    short redDelta;
    short newB;
    short newR;
    unsigned char src;
    short num;
    unsigned char subPal;
    short gAdd;
    short newG;
    short palIdx;
    unsigned short px;

    num = SCRIPT_PARAM_U8(9) + 1;
    src = Script_ReadOperand8(1, 4);
    subPal = Script_ReadOperand8(2, 5);
    bDelta = Script_ReadOperand8(3, 6);
    gAdd = Script_ReadOperand8(4, 7);
    redDelta = Script_ReadOperand8(5, 8);
    if ((bDelta & 0x80) != 0) {
        bDelta ^= 0xFF;
        bDelta += 1;
        bDelta = -bDelta;
    }
    if ((gAdd & 0x80) != 0) {
        gAdd ^= 0xFF;
        gAdd += 1;
        gAdd = -gAdd;
    }
    if ((redDelta & 0x80) != 0) {
        redDelta ^= 0xFF;
        redDelta += 1;
        redDelta = -redDelta;
    }
    for (palIdx = 0; palIdx < num; palIdx++) {
        px = g_FieldPalettes[src][palIdx];
        newR = (px & 0x1F) + redDelta;
        if (newR > 31) {
            newR = 0x1F;
        }
        if (newR < 0) {
            newR = 0;
        }
        newG = ((px >> 5) & 0x1F) + gAdd;
        if (newG > 31) {
            newG = 0x1F;
        }
        if (newG < 0) {
            newG = 0;
        }
        newB = ((px >> 10) & 0x1F) + bDelta;
        if (newB > 31) {
            newB = 0x1F;
        }
        if (newB < 0) {
            newB = 0;
        }
        g_FieldPalettes[subPal][palIdx] = (newB << 10) | (newG << 5) | newR | (px & 0x8000);
        if (g_FieldPalettes[subPal][palIdx] == 0 && px != 0) {
            g_FieldPalettes[subPal][palIdx] |= 0x8000;
        }
    }
    g_FieldScriptPC[g_CurrentEntity] += 10;
    return 0;
}

/*
 * ADPAL2: ADPAL over part of a slot: store slot [4] into slot [5], colours v[6]/1 to v[6]/1 + v[10]/6,
 * deltas blue v[7]/2, green v[8]/3, red v[9]/4. 11 bytes.
 *
 * Field opcode 0xEF (FIELD_OP_PALETTE_ADD_RANGE).
 */
// FUNCTION: FF7 0x0061b479
int __cdecl Script_OpPaletteAddRange(void)
{
    short bDelta;
    short redDelta;
    short redOut;
    short outBlue;
    short index;
    short resGreen;
    short num;
    unsigned char destPal8;
    short greenAdj;
    unsigned char srcPalette;
    unsigned short px;
    short beg;

    num = Script_ReadOperand8(6, 10) + 1;
    srcPalette = SCRIPT_PARAM_S8(4);
    destPal8 = SCRIPT_PARAM_S8(5);
    beg = (unsigned char)Script_ReadOperand8(1, 6);
    bDelta = (unsigned char)Script_ReadOperand8(2, 7);
    greenAdj = (unsigned char)Script_ReadOperand8(3, 8);
    redDelta = (unsigned char)Script_ReadOperand8(4, 9);
    if ((bDelta & 0x80) != 0) {
        bDelta ^= 0xFF;
        bDelta++;
        bDelta = -bDelta;
    }
    if ((greenAdj & 0x80) != 0) {
        greenAdj ^= 0xFF;
        greenAdj++;
        greenAdj = -greenAdj;
    }
    if ((redDelta & 0x80) != 0) {
        redDelta ^= 0xFF;
        redDelta++;
        redDelta = -redDelta;
    }
    num += beg;
    for (index = beg; index < num; index++) {
        px = *(short *)&g_FieldPalettes[srcPalette][index];
        redOut = (px & 0x1F) + redDelta;
        if (redOut > 31) {
            redOut = 0x1F;
        }
        if (redOut < 0) {
            redOut = 0;
        }
        resGreen = (px >> 5 & 0x1F) + greenAdj;
        if (resGreen > 31) {
            resGreen = 0x1F;
        }
        if (resGreen < 0) {
            resGreen = 0;
        }
        outBlue = (px >> 0xA & 0x1F) + bDelta;
        if (outBlue > 31) {
            outBlue = 0x1F;
        }
        if (outBlue < 0) {
            outBlue = 0;
        }
        *(short *)&g_FieldPalettes[destPal8][index] = outBlue << 0xA | resGreen << 5 | redOut | px & 0x8000;
        if (g_FieldPalettes[destPal8][index] == 0 && px != 0) {
            g_FieldPalettes[destPal8][index] |= 0x8000;
        }
    }
    g_FieldScriptPC[g_CurrentEntity] += 0xB;
    return 0;
}

/*
 * MPPAL2: multiplies [9] + 1 colours of store slot v[4]/1 by a colour, results into slot v[5]/2: factors
 * blue v[6]/3, green v[7]/4, red v[8]/5, where 64 keeps a channel and 128 doubles it, each clamped to 31.
 * 10 bytes. Transparent (0) colours are left out; a result of 0 gets the 0x8000 bit so it stays black.
 *
 * Field opcode 0xEA (FIELD_OP_PALETTE_MULTIPLY).
 */
// FUNCTION: FF7 0x0061b76b
int __cdecl Script_OpPaletteMultiply(void)
{
    unsigned short redFactor;
    unsigned short bMul;
    unsigned short greenFactor;
    unsigned char srcPal;
    unsigned short rawBlue;
    unsigned short redChan;
    short number;
    unsigned char dstPal;
    unsigned short newGreen;
    short i;
    unsigned short pixel;

    number = SCRIPT_PARAM_U8(9) + 1;
    srcPal = Script_ReadOperand8(1, 4);
    dstPal = Script_ReadOperand8(2, 5);
    bMul = (unsigned char)Script_ReadOperand8(3, 6);
    greenFactor = (unsigned char)Script_ReadOperand8(4, 7);
    redFactor = (unsigned char)Script_ReadOperand8(5, 8);
    for (i = 0; i < number; i++) {
        pixel = *(short *)&g_FieldPalettes[srcPal][i];
        if (pixel == 0) {
            continue;
        }
        redChan = pixel << 1 & 0x3F;
        redChan = redFactor * redChan >> 7;
        newGreen = pixel >> 4 & 0x3F;
        newGreen = greenFactor * newGreen >> 7;
        rawBlue = pixel >> 9 & 0x3F;
        rawBlue = bMul * rawBlue >> 7;
        if (rawBlue > 31) {
            rawBlue = 0x1F;
        }
        if (newGreen > 31) {
            newGreen = 0x1F;
        }
        if (redChan > 31) {
            redChan = 0x1F;
        }
        *(short *)&g_FieldPalettes[dstPal][i] = rawBlue << 0xA | newGreen << 5 | redChan | pixel & 0x8000;
        if (g_FieldPalettes[dstPal][i] == 0) {
            g_FieldPalettes[dstPal][i] |= 0x8000;
        }
    }
    g_FieldScriptPC[g_CurrentEntity] += 0xA;
    return 0;
}

/*
 * MPPAL: MPPAL2 over part of a slot: store slot [4] into slot [5], colours v[6]/1 to v[6]/1 + v[10]/6,
 * factors blue v[7]/2, green v[8]/3, red v[9]/4. 11 bytes.
 *
 * Field opcode 0xDF (FIELD_OP_PALETTE_MULTIPLY_RANGE).
 */
// FUNCTION: FF7 0x0061b9ce
int __cdecl Script_OpPaletteMultiplyRange(void)
{
    unsigned short blueFactor;
    unsigned short redFactor;
    short beg;
    unsigned short newB;
    unsigned short newR;
    unsigned char src;
    short num;
    unsigned char subPal;
    unsigned short gAdd;
    unsigned short newG;
    short palIdx;
    unsigned short px;

    num = Script_ReadOperand8(6, 10) + 1;
    src = SCRIPT_PARAM_U8(4);
    subPal = SCRIPT_PARAM_U8(5);
    beg = Script_ReadOperand8(1, 6);
    blueFactor = Script_ReadOperand8(2, 7);
    gAdd = Script_ReadOperand8(3, 8);
    redFactor = Script_ReadOperand8(4, 9);
    num += beg;
    for (palIdx = beg; palIdx < num; palIdx++) {
        px = g_FieldPalettes[src][palIdx];
        if (px == 0) {
            continue;
        }
        newR = (px << 1) & 0x3F;
        newR = (redFactor * newR) >> 7;
        newG = (px >> 4) & 0x3F;
        newG = (gAdd * newG) >> 7;
        newB = (px >> 9) & 0x3F;
        newB = (blueFactor * newB) >> 7;
        if (newB > 31) {
            newB = 0x1F;
        }
        if (newG > 31) {
            newG = 0x1F;
        }
        if (newR > 31) {
            newR = 0x1F;
        }
        g_FieldPalettes[subPal][palIdx] = (newB << 10) | (newG << 5) | newR | (px & 0x8000);
        if (g_FieldPalettes[subPal][palIdx] == 0) {
            g_FieldPalettes[subPal][palIdx] |= 0x8000;
        }
    }
    g_FieldScriptPC[g_CurrentEntity] += 11;
    return 0;
}

/*
 * Makes the party leader's model the one the player drives, if the leader has an entity with a model on
 * this map; otherwise the player model stays as it was.
 */
// FUNCTION: FF7 0x0061bc67
void __cdecl Field_SetPlayerModelFromParty(void)
{
    short entity;
    unsigned char member;

    member = gSavemap.fieldParty[0];
    if (member == 0xff) {
        return;
    }
    entity = g_FieldCharEntity.charEntity[member];
    if (entity == 0xff) {
        return;
    }
    if (g_FieldEntityModel[entity] != 0xff) {
        g_pFieldState->playerModel = g_FieldEntityModel[entity];
    }
}

/*
 * PC: makes this entity the field body of character [1]. If the character leads the party, its model
 * becomes the one the player drives; a character in slot 2 or 3, or one not in the party, is hidden, made
 * walk-through and cannot be talked to. 2 bytes.
 *
 * Field opcode 0xA0 (FIELD_OP_BIND_CHARACTER).
 */
// FUNCTION: FF7 0x0061bcd7
int __cdecl Script_OpBindCharacter(void)
{
    unsigned char characterId;
    int partySlot;

    characterId = SCRIPT_PARAM_S8(1);
    g_FieldCharEntity.charEntity[characterId] = *(char *)&g_CurrentEntity;
    for (partySlot = 0; partySlot < 3; partySlot++) {
        if (gSavemap.fieldParty[partySlot] == characterId) {
            if (partySlot != 0) {
                g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].visible = 0;
                g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].solidOff = 1;
                g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].talkOff = 1;
            } else {
                g_pFieldState->playerModel = g_FieldEntityModel[g_CurrentEntity];
            }
            g_FieldScriptPC[g_CurrentEntity] += 2;
            return 0;
        }
    }
    g_FieldCharEntity.charEntity[characterId] = *(char *)&g_CurrentEntity;
    g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].visible = 0;
    g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].solidOff = 1;
    g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].talkOff = 1;
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * PRTYP: puts character [1] into the party: into the first free slot, or over slot 3 when the party is
 * full, and makes it available in the PHS. A character already in the party changes nothing. Either way
 * the savemap party is brought in line and the leader drives. 2 bytes.
 *
 * Field opcode 0xC8 (FIELD_OP_PARTY_ADD).
 */
// FUNCTION: FF7 0x0061be95
int __cdecl Script_OpPartyAdd(void)
{
    unsigned char characterId;
    short bitShift;
    int partySlot;

    characterId = SCRIPT_PARAM_S8(1);
    for (partySlot = 0; partySlot < 3; partySlot++) {
        if (gSavemap.fieldParty[partySlot] == characterId) {
            g_FieldScriptPC[g_CurrentEntity] += 2;
            Field_SetPlayerModelFromParty();
            Field_PartySyncToSavemap(0);
            return 0;
        }
    }
    for (partySlot = 0; partySlot < 3; partySlot++) {
        if (gSavemap.fieldParty[partySlot] == PARTY_CHAR_NONE) {
            gSavemap.fieldParty[partySlot] = characterId;
            if (characterId != PARTY_CHAR_NONE) {
                bitShift = characterId;
                gSavemap.phsVisibilityMask |= (short)(1 << bitShift);
            }
            g_FieldScriptPC[g_CurrentEntity] += 2;
            Field_SetPlayerModelFromParty();
            Field_PartySyncToSavemap(1);
            return 0;
        }
    }
    gSavemap.fieldParty[2] = characterId;
    if (characterId != PARTY_CHAR_NONE) {
        bitShift = characterId;
        gSavemap.phsVisibilityMask |= (short)(1 << bitShift);
    }
    g_FieldScriptPC[g_CurrentEntity] += 2;
    Field_SetPlayerModelFromParty();
    Field_PartySyncToSavemap(1);
    return 0;
}

/*
 * PRTYM: takes character [1] out of the party, then brings the savemap party in line and lets the (new)
 * leader drive. 2 bytes.
 *
 * Field opcode 0xC9 (FIELD_OP_PARTY_REMOVE).
 */
// FUNCTION: FF7 0x0061c045
int __cdecl Script_OpPartyRemove(void)
{
    unsigned char charId;
    int n;

    charId = SCRIPT_PARAM_U8(1);
    for (n = 0; n < 3; n++) {
        if (gSavemap.fieldParty[n] == charId) {
            gSavemap.fieldParty[n] = PARTY_CHAR_NONE;
            Field_PartySyncToSavemap(1);
            Field_SetPlayerModelFromParty();
            g_FieldScriptPC[g_CurrentEntity] += 2;
            return 0;
        }
    }
    Field_PartySyncToSavemap(1);
    Field_SetPlayerModelFromParty();
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * PRTYE: sets the whole party from the characters [1], [2], [3]; see Savemap_SetFieldParty for what 0xFF
 * and 0xFE mean. 4 bytes.
 *
 * Script_OpSetFieldParty with the operands taken straight out of the instruction stream instead of through
 * banks.
 *
 * Field opcode 0xCA (FIELD_OP_PARTY_SET).
 */
// FUNCTION: FF7 0x0061c113
int __cdecl Script_OpPartySet(void)
{
    unsigned char party[4];
    int i;

    for (i = 0; i < 3; i++) {
        party[i] = SCRIPT_PARAM_U8(i + 1);
    }
    Savemap_SetFieldParty(party);
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * SPTYE: PRTYE with the characters v[3]/1, v[4]/2, v[5]/3. 6 bytes.
 *
 * The mirror of Script_OpGetFieldParty: three operands read out of three consecutive banks into a stack
 * buffer, then handed over in one call.
 *
 * Field opcode 0x0A (FIELD_OP_SET_FIELD_PARTY).
 */
// FUNCTION: FF7 0x0061c190
int __cdecl Script_OpSetFieldParty(void)
{
    unsigned char party[4];
    int i;

    for (i = 0; i < 3; i++) {
        party[i] = Script_ReadOperand8(i + 1, i + 3);
    }
    Savemap_SetFieldParty(party);
    g_FieldScriptPC[g_CurrentEntity] += 6;
    return 0;
}

/*
 * PTYE: reads the three party slots into v[3]/1, v[4]/2, v[5]/3 (0xFF for an empty slot). 6 bytes.
 *
 * Writes the three field party slots into three consecutive banks and addresses, both derived from the
 * loop counter.
 *
 * Field opcode 0x0B (FIELD_OP_GET_FIELD_PARTY).
 */
// FUNCTION: FF7 0x0061c202
int __cdecl Script_OpGetFieldParty(void)
{
    int i;

    for (i = 0; i < 3; i++) {
        Script_WriteOperand8(i + 1, i + 3, gSavemap.fieldParty[i]);
    }
    g_FieldScriptPC[g_CurrentEntity] += 6;
    return 0;
}

/*
 * Sets the field party from a three-slot list. Anyone named in the list is first taken out of the old
 * party, so nobody appears twice; old members not named then fill the list's 255 slots, so nobody is
 * lost; a 0xFE slot is left empty. The list is written back (the caller's array is changed in place),
 * every member is made available in the PHS, the savemap party is brought in line and the leader drives.
 */
// FUNCTION: FF7 0x0061c26a
void __cdecl Savemap_SetFieldParty(unsigned char *party)
{
    short pcId;
    int i;
    int j;

    for (i = 0; i < 3; i++) {
        if (party[i] == PARTY_CHAR_NONE) {
            continue;
        }
        for (j = 0; j < 3; j++) {
            if (party[i] == gSavemap.fieldParty[j]) {
                gSavemap.fieldParty[j] = PARTY_CHAR_NONE;
            }
        }
    }
    for (i = 0; i < 3; i++) {
        if (gSavemap.fieldParty[i] == PARTY_CHAR_NONE) {
            continue;
        }
        for (j = 0; j < 3; j++) {
            if (party[j] == PARTY_CHAR_NONE) {
                party[j] = gSavemap.fieldParty[i];
                j = 3;
            }
        }
    }
    for (i = 0; i < 3; i++) {
        if (party[i] == 0xfe) {
            party[i] = PARTY_CHAR_NONE;
        }
        gSavemap.fieldParty[i] = party[i];
        if (party[i] != PARTY_CHAR_NONE) {
            pcId = party[i];
            gSavemap.phsVisibilityMask |= 1 << pcId;
        }
    }
    Field_PartySyncToSavemap(1);
    Field_SetPlayerModelFromParty();
}

/*
 * Compares two three-slot party lists: onlyB gets the members of partyB missing from partyA, onlyA those
 * of partyA missing from partyB, each padded with 0xFF. 255 slots are compared like characters. Assumes
 * outputs do not destructively alias inputs.
 */
// FUNCTION: FF7 0x0061c3e1
void __cdecl Savemap_PartyDiffSlots(unsigned char *partyA, unsigned char *partyB,
                                    unsigned char *onlyB,
                                    unsigned char *onlyA)
{
    int i;
    int j;
    int k;
    int found;

    found = 0;
    for (k = 0; k < 3; k++) {
        onlyB[k] = 0xff;
        onlyA[k] = 0xff;
    }
    k = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3 && found == 0; j++) {
            if (partyB[i] == partyA[j]) {
                found = 1;
            }
        }
        if (found == 0) {
            onlyB[k] = partyB[i];
            k++;
        }
        found = 0;
    }
    k = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3 && found == 0; j++) {
            if (partyA[i] == partyB[j]) {
                found = 1;
            }
        }
        if (found == 0) {
            onlyA[k] = partyA[i];
            k++;
        }
        found = 0;
    }
}

/*
 * Brings the savemap's active party (gSavemap.party) in line with the field party: characters who left are
 * cleared from it, newcomers take its free slots, and the party's stats are marked for a reload
 * (g_PartyStatsDirty, which the main loop acts on). The locals' names are the wrong way round: `added`
 * receives who left, `removed` who joined.
 *
 * The parameter is unused.
 *
 */
// FUNCTION: FF7 0x0061c52a
void __cdecl Field_PartySyncToSavemap(int unused)
{
    int added;
    int removed;

    Savemap_PartyDiffSlots((unsigned char *)gSavemap.party, (unsigned char *)gSavemap.fieldParty,
                           (unsigned char *)&removed,
                           (unsigned char *)&added);
    // The two helpers read these as byte arrays; `lea` is the same
    // instruction either way, so the cast costs nothing and says so.
    Savemap_PartyClearSlots((unsigned char *)gSavemap.party, (unsigned char *)&added);
    Savemap_PartyFillSlots((unsigned char *)gSavemap.party, (unsigned char *)&removed);
    g_PartyStatsDirty = 1;
}

/*
 * The other direction: brings the field party in line with the savemap party, which is what runs when a
 * menu hands control back to the field -- the PHS may have rearranged the party in the meantime. As in
 * Field_PartySyncToSavemap, `added` receives the characters to clear and `removed` the ones to fill in.
 */
// FUNCTION: FF7 0x0061c577
void __cdecl Savemap_ReconcileFieldParty(void)
{
    int added;
    int removed;

    Savemap_PartyDiffSlots((unsigned char *)gSavemap.fieldParty, (unsigned char *)gSavemap.party, (unsigned char *)&removed, (unsigned char *)&added);
    Savemap_PartyClearSlots((unsigned char *)gSavemap.fieldParty, (unsigned char *)&added);
    Savemap_PartyFillSlots((unsigned char *)gSavemap.fieldParty, (unsigned char *)&removed);
}

/*
 * Clears every slot of dst that holds a character listed in src.
 *
 * Half of the reconciliation between gSavemap.party and gSavemap.fieldParty: blank any destination slot
 * already holding a character the source lists. No break, so a character appearing twice would be blanked
 * twice.
 */
// FUNCTION: FF7 0x0061c5bd
void __cdecl Savemap_PartyClearSlots(unsigned char *dst, unsigned char *src)
{
    int i;
    int j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (src[i] == dst[j]) {
                dst[j] = 0xff;
            }
        }
    }
}

/*
 * Puts each character of src into the first free (0xFF) slot of dst, while there is one. A 0xFF in src
 * fills a free slot with 0xFF, which leaves it free for the next.
 */
// FUNCTION: FF7 0x0061c61c
void __cdecl Savemap_PartyFillSlots(unsigned char *dst, unsigned char *src)
{
    int i;
    int j;
    int found;

    found = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3 && found == 0; j++) {
            if (dst[j] == 0xff) {
                dst[j] = src[i];
                found = 1;
            }
        }
        found = 0;
    }
}

/*
 * IFPRTYQ: if character [1] is in the party the script goes on with the next opcode; otherwise it skips
 * forward [2] bytes, counted from that byte. 3 bytes.
 *
 * Field opcode 0xCB (FIELD_OP_IF_CHAR_IN_PARTY).
 */
// FUNCTION: FF7 0x0061c696
int __cdecl Script_OpIfCharInParty(void)
{
    unsigned char charId;
    int n;

    charId = SCRIPT_PARAM_U8(1);
    for (n = 0; n < 3; n++) {
        if (gSavemap.fieldParty[n] == charId) {
            g_FieldScriptPC[g_CurrentEntity] += 3;
            return 0;
        }
    }
    g_FieldScriptPC[g_CurrentEntity] += SCRIPT_PARAM_U8(2) + 2;
    return 0;
}

/*
 * IFMEMBQ: if character [1] is available (has joined, so the PHS can offer it) the script goes on with the
 * next opcode; otherwise it skips forward [2] bytes, counted from that byte. 3 bytes.
 *
 * Field opcode 0xCC (FIELD_OP_IF_CHAR_AVAILABLE).
 */
// FUNCTION: FF7 0x0061c75e
int __cdecl Script_OpIfCharAvailable(void)
{
    short member;

    member = SCRIPT_PARAM_U8(1);
    if ((gSavemap.phsVisibilityMask & (1 << member)) != 0) {
        g_FieldScriptPC[g_CurrentEntity] += 3;
        return 0;
    }
    g_FieldScriptPC[g_CurrentEntity] += SCRIPT_PARAM_U8(2) + 2;
    return 0;
}

/*
 * MMBUD: makes character [2] available ([1] nonzero) or not; a character made unavailable also leaves the
 * party. 3 bytes.
 *
 * Field opcode 0xCD (FIELD_OP_SET_CHAR_AVAILABLE).
 */
// FUNCTION: FF7 0x0061c812
int __cdecl Script_OpSetCharAvailable(void)
{
    short charId;
    short n;
    short avail;

    avail = SCRIPT_PARAM_U8(1);
    charId = SCRIPT_PARAM_U8(2);
    if (avail) {
        gSavemap.phsVisibilityMask |= 1 << charId;
    } else {
        gSavemap.phsVisibilityMask &= (1 << charId) ^ 0xffff;
        for (n = 0; n < 3; n++) {
            if (gSavemap.fieldParty[n] == charId) {
                gSavemap.fieldParty[n] = PARTY_CHAR_NONE;
            }
        }
    }
    g_FieldScriptPC[g_CurrentEntity] += 3;
    return 0;
}

/*
 * MMBLK: locks character [1] in the PHS: the player cannot take it out of the party. 2 bytes.
 *
 * The PlayStation handler does the same to its `phs_locking_mask`; include/ff7/savemap.h shows how that
 * fixes the base address of the whole savemap.
 *
 * Field opcode 0xCE (FIELD_OP_MEMBER_LOCK).
 */
// FUNCTION: FF7 0x0061c907
int __cdecl Script_OpMemberLock(void)
{
    short member;

    member = SCRIPT_PARAM_U8(1);
    gSavemap.phsLockingMask |= 1 << member;
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * MMBUK: releases character [1] from the PHS lock. 2 bytes.
 *
 * The PlayStation handler does the same to its `phs_locking_mask`. `xor edx, 0xffff` rather than
 * `not edx`, so the PC source spelled the complement as `^ 0xFFFF`.
 *
 * Field opcode 0xCF (FIELD_OP_MEMBER_UNLOCK).
 */
// FUNCTION: FF7 0x0061c96f
int __cdecl Script_OpMemberUnlock(void)
{
    short member;

    member = SCRIPT_PARAM_U8(1);
    gSavemap.phsLockingMask &= (1 << member) ^ 0xffff;
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * SOLID: [1] nonzero makes this model something the player walks through (solidOff), 0 solid again. 2
 * bytes.
 *
 * Field opcode 0xC7 (FIELD_OP_SET_SOLID).
 */
// FUNCTION: FF7 0x0061c9dd
int __cdecl Script_OpSetSolid(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] != 0xff) {
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].solidOff =
            SCRIPT_PARAM_U8(1);
    }
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * VWOFT: offsets the camera view to v[2]/1: at once when [6] is 0, otherwise over v[4]/2 steps ([6] = 1
 * linear, 2 smooth), starting from the current offset; the timed form yields once. 7 bytes.
 *
 * Field opcode 0x6A (FIELD_OP_SET_VIEW_OFFSET).
 */
// FUNCTION: FF7 0x0061ca5c
int __cdecl Script_OpSetViewOffset(void)
{
    switch (SCRIPT_PARAM_U8(6)) {
    case FIELD_VIEW_OFFSET_IMMEDIATE:
        g_pFieldState->viewOffset = Script_ReadOperand16(1, 2);
        g_pFieldState->viewOffsetNumSteps = 0;
        g_pFieldState->viewOffsetCurrentStep = 0;
        g_pFieldState->viewOffsetMode = 0;
        g_pFieldState->viewOffsetStart = 0;
        g_pFieldState->viewOffsetTarget = 0;
        g_FieldScriptPC[g_CurrentEntity] += 7;
        return 0;
    }
    g_pFieldState->viewOffsetStart = g_pFieldState->viewOffset;
    g_pFieldState->viewOffsetTarget = Script_ReadOperand16(1, 2);
    g_pFieldState->viewOffsetNumSteps = Script_ReadOperand16(2, 4);
    g_pFieldState->viewOffsetMode = SCRIPT_PARAM_U8(6);
    g_pFieldState->viewOffsetCurrentStep = 0;
    g_FieldScriptPC[g_CurrentEntity] += 7;
    return 1;
}

/*
 * JOIN: walks the two other party members back into the leader over [1] frames, where they vanish:
 * invisible, walk-through and not talkable. Waits until both are in; meanwhile player control is off and
 * the leader is walk-through, and afterwards UC's setting comes back. 2 bytes.
 *
 * Each follower is stepped by Script_StepFollowerToLeader. Its state lives in g_FieldFollowerState, which
 * the scheduler also reads: a follower with a nonzero state does not run its own scripts, except the
 * entity g_FieldDialogTextState[0] names, which is why this handler stores its own entity there. Followers
 * whose character has no entity here are only partly checked.
 *
 * Field opcode 0x08 (FIELD_OP_PARTY_JOIN_LEADER).
 */
// FUNCTION: FF7 0x0061cb92
int __cdecl Script_OpPartyJoinLeader(void)
{
    unsigned char characterId;
    short entity;
    short i;
    short ok;

    // The scheduler keeps running this entity while its followers are held.
    g_FieldDialogTextState[0] = g_CurrentEntity;
    characterId = gSavemap.fieldParty[1];
    if (characterId != PARTY_CHAR_NONE) {
        entity = Script_StepFollowerToLeader(g_FieldCharEntity.charEntity[characterId], SCRIPT_PARAM_U8(1));
    } else {
        entity = 1;
    }
    characterId = gSavemap.fieldParty[2];
    if (characterId != PARTY_CHAR_NONE) {
        ok = Script_StepFollowerToLeader(g_FieldCharEntity.charEntity[characterId], SCRIPT_PARAM_U8(1));
    } else {
        ok = 1;
    }
    // Both followers are done.
    if (entity != 0 && ok != 0) {
        for (i = 0; i < 3; i++) {
            characterId = gSavemap.fieldParty[i];
            if (characterId == PARTY_CHAR_NONE) {
                continue;
            }
            g_FieldFollowerState[g_FieldCharEntity.charEntity[characterId]] = FIELD_FOLLOWER_START;
            if (i == 0) {
                entity = g_FieldCharEntity.charEntity[characterId];
                if (entity != 0xFF) {
                    g_pFieldEntities[g_FieldEntityModel[entity]].solidOff = 0;
                }
            }
        }
        g_pFieldState->characterLock = g_FieldDialogueLock;
        g_FieldDialogTextState[0] = 0xFF;
        g_FieldScriptPC[g_CurrentEntity] += 2;
        return 0;
    }
    g_pFieldState->characterLock = 1;
    characterId = gSavemap.fieldParty[0];
    if (characterId != PARTY_CHAR_NONE) {
        entity = g_FieldCharEntity.charEntity[characterId];
        if (entity != 0xFF) {
            g_FieldFollowerState[entity] = 1;
            g_pFieldEntities[g_FieldEntityModel[entity]].scriptedMoveMode = FIELD_SCRIPTED_MOVE_NONE;
            g_pFieldEntities[g_FieldEntityModel[entity]].actionState = FIELD_MOVE_STATE_START;
            g_pFieldEntities[g_FieldEntityModel[entity]].solidOff = 1;
        }
    }
    return 1;
}

/*
 * SPLIT: walks the two other party members out of the leader to a point each and turns them to a facing:
 * the second to x v[4]/1, y v[6]/2 facing v[8]/3, the third to x v[9]/4, y v[11]/5 facing v[13]/6, both
 * over [14] frames. They end up solid and talkable. Waits until both are done, with player control off and
 * the leader walk-through meanwhile. 15 bytes.
 *
 * Each follower is stepped by Script_StepFollowerToPoint; the scheduler exception is the one JOIN uses.
 *
 * Field opcode 0x09 (FIELD_OP_PARTY_SPLIT_TO).
 */
// FUNCTION: FF7 0x0061ce0c
int __cdecl Script_OpPartySplitTo(void)
{
    unsigned char characterId;
    short entity;
    short i;
    short ok;

    // The scheduler keeps running this entity while its followers are held.
    g_FieldDialogTextState[0] = g_CurrentEntity;
    characterId = gSavemap.fieldParty[1];
    if (characterId == PARTY_CHAR_NONE) {
        entity = 1;
    } else {
        entity = Script_StepFollowerToPoint(g_FieldCharEntity.charEntity[characterId], Script_ReadOperand16(1, 4),
                                            Script_ReadOperand16(2, 6), Script_ReadOperand8(3, 8),
                                            SCRIPT_PARAM_U8(14));
    }
    characterId = gSavemap.fieldParty[2];
    if (characterId == PARTY_CHAR_NONE) {
        ok = 1;
    } else {
        ok = Script_StepFollowerToPoint(g_FieldCharEntity.charEntity[characterId], Script_ReadOperand16(4, 9),
                                        Script_ReadOperand16(5, 11), Script_ReadOperand8(6, 13),
                                        SCRIPT_PARAM_U8(14));
    }
    // Both followers are done.
    if (entity != 0 && ok != 0) {
        for (i = 0; i < 3; i++) {
            characterId = gSavemap.fieldParty[i];
            if (characterId == PARTY_CHAR_NONE) {
                continue;
            }
            g_FieldFollowerState[g_FieldCharEntity.charEntity[characterId]] = FIELD_FOLLOWER_START;
            if (i == 0) {
                entity = g_FieldCharEntity.charEntity[characterId];
                if (entity != 0xFF) {
                    g_pFieldEntities[g_FieldEntityModel[entity]].solidOff = 0;
                }
            }
        }
        g_pFieldState->characterLock = g_FieldDialogueLock;
        g_FieldDialogTextState[0] = 0xFF;
        g_FieldScriptPC[g_CurrentEntity] += 15;
        return 0;
    }
    g_pFieldState->characterLock = 1;
    characterId = gSavemap.fieldParty[0];
    if (characterId != PARTY_CHAR_NONE) {
        entity = g_FieldCharEntity.charEntity[characterId];
        if (entity != 0xFF) {
            g_FieldFollowerState[entity] = 1;
            g_pFieldEntities[g_FieldEntityModel[entity]].scriptedMoveMode = FIELD_SCRIPTED_MOVE_NONE;
            g_pFieldEntities[g_FieldEntityModel[entity]].actionState = FIELD_MOVE_STATE_START;
            g_pFieldEntities[g_FieldEntityModel[entity]].solidOff = 1;
        }
    }
    return 1;
}

/*
 * One frame of a follower's JOIN, a four-state machine: turn to face the leader (START), wait for the turn
 * and start the walk onto the leader (TURNING), wait for the walk and then fold the follower away --
 * walk-through, not talkable, invisible (MOVING), done (DONE). moveArg is the walk's duration in frames,
 * not a speed. No leader, or no entity, counts as done.
 */
// FUNCTION: FF7 0x0061d0d4
int __cdecl Script_StepFollowerToLeader(short entity, short moveArg)
{
    int dist;
    int fromPoint[4];
    int leaderPoint[4];
    short leaderEnt;
    unsigned char leaderCharacter;
    unsigned char dir;

    leaderCharacter = gSavemap.fieldParty[0];
    if (leaderCharacter == PARTY_CHAR_NONE) {
        return 1;
    }
    leaderEnt = g_FieldCharEntity.charEntity[leaderCharacter];
    if (leaderEnt == 0xFF || entity == 0xFF) {
        return 1;
    }
    switch (g_FieldFollowerState[entity]) {
    case FIELD_FOLLOWER_START:
        Field_MakePoint((FieldVec3i *)fromPoint, g_pFieldEntities[g_FieldEntityModel[entity]].posX >> 12,
                        g_pFieldEntities[g_FieldEntityModel[entity]].posY >> 12,
                        g_pFieldEntities[g_FieldEntityModel[entity]].posZ >> 12);
        Field_MakePoint((FieldVec3i *)leaderPoint, g_pFieldEntities[g_FieldEntityModel[leaderEnt]].posX >> 12,
                        g_pFieldEntities[g_FieldEntityModel[leaderEnt]].posY >> 12,
                        g_pFieldEntities[g_FieldEntityModel[leaderEnt]].posZ >> 12);
        dir = Field_HeadingBetweenPoints((FieldPointTo *)fromPoint, (FieldPointTo *)leaderPoint, &dist);
        Field_StartEntityTurn(entity, g_pFieldEntities[g_FieldEntityModel[entity]].dir, dir);
        g_FieldFollowerState[entity] = FIELD_FOLLOWER_TURNING;
        break;
    case FIELD_FOLLOWER_MOVING:
        if (Field_TakeMoveDone(entity) != 0) {
            g_pFieldEntities[g_FieldEntityModel[entity]].solidOff = 1;
            g_pFieldEntities[g_FieldEntityModel[entity]].talkOff = 1;
            g_pFieldEntities[g_FieldEntityModel[entity]].visible = 0;
            g_FieldFollowerState[entity] = FIELD_FOLLOWER_DONE;
            return 1;
        }
        break;
    case FIELD_FOLLOWER_TURNING:
        if (Field_TakeTurnDone(entity) != 0) {
            Field_StartScriptedMove(entity, g_pFieldEntities[g_FieldEntityModel[leaderEnt]].posX >> 12,
                                    g_pFieldEntities[g_FieldEntityModel[leaderEnt]].posY >> 12, moveArg, 0);
            g_FieldFollowerState[entity] = FIELD_FOLLOWER_MOVING;
        }
        break;
    case FIELD_FOLLOWER_DONE:
        return 1;
    }
    return 0;
}

/*
 * One frame of a follower's SPLIT: start the walk from the leader to (x, y) (START), wait for it and make
 * the follower solid and talkable and start its turn to dir (MOVING), wait for the turn (TURNING), done
 * (DONE). moveArg is the walk's duration in frames. Entity 0xFF counts as done.
 */
// FUNCTION: FF7 0x0061d389
int __cdecl Script_StepFollowerToPoint(short entity, short x, short y,
                                       unsigned char dir, short moveArg)
{
    if (entity == 0xff) {
        return 1;
    }
    switch (g_FieldFollowerState[entity]) {
    case FIELD_FOLLOWER_START:
        Field_StartScriptedMove(entity, x, y, moveArg, 1);
        g_FieldFollowerState[entity] = FIELD_FOLLOWER_MOVING;
        break;
    case FIELD_FOLLOWER_MOVING:
        if (Field_TakeMoveDone(entity) != 0) {
            g_pFieldEntities[g_FieldEntityModel[entity]].solidOff = 0;
            g_pFieldEntities[g_FieldEntityModel[entity]].talkOff = 0;
            Field_StartEntityTurn(entity, g_pFieldEntities[g_FieldEntityModel[entity]].dir, dir);
            g_FieldFollowerState[entity] = FIELD_FOLLOWER_TURNING;
        }
        break;
    case FIELD_FOLLOWER_TURNING:
        if (Field_TakeTurnDone(entity) != 0) {
            g_FieldFollowerState[entity] = FIELD_FOLLOWER_DONE;
            return 1;
        }
        break;
    case FIELD_FOLLOWER_DONE:
        return 1;
    }
    return 0;
}

/*
 * Starts walking a follower to (x, y) so that it arrives in moveArg frames: the speed is the distance
 * times 256 divided by moveArg, so a larger number gives a slower walk. The follower is made visible and,
 * for the walk, walk-through and not talkable; mode nonzero first puts it on the party leader's spot. Its
 * own speed is kept in g_FieldModelDefaultSpeed until Field_TakeMoveDone puts it back. Run animation above
 * speed 0x600, walk otherwise, as MOVE. Does nothing without a party leader with an entity.
 *
 * moveArg zero divides by zero, and the absent-model path preserves prior last-frame data.
 */
// FUNCTION: FF7 0x0061d4b9
void __cdecl Field_StartScriptedMove(short entity, short x, short y, short moveArg,
                                     short mode)
{
    int a[4];
    int b;
    int d[4];
    unsigned char e;
    short f;

    e = gSavemap.fieldParty[0];
    if (e == 0xFF) {
        return;
    }
    f = g_FieldCharEntity.charEntity[e];
    if (f == 0xFF) {
        return;
    }
    g_pFieldEntities[g_FieldEntityModel[entity]].visible = 1;
    g_pFieldEntities[g_FieldEntityModel[entity]].solidOff = 1;
    g_pFieldEntities[g_FieldEntityModel[entity]].talkOff = 1;
    if (mode != 0) {
        g_pFieldEntities[g_FieldEntityModel[entity]].posX = g_pFieldEntities[g_FieldEntityModel[f]].posX;
        g_pFieldEntities[g_FieldEntityModel[entity]].posY = g_pFieldEntities[g_FieldEntityModel[f]].posY;
        g_pFieldEntities[g_FieldEntityModel[entity]].posZ = g_pFieldEntities[g_FieldEntityModel[f]].posZ;
        g_pFieldEntities[g_FieldEntityModel[entity]].posI = g_pFieldEntities[g_FieldEntityModel[f]].posI;
    }
    g_pFieldEntities[g_FieldEntityModel[entity]].actionArg = 0;
    g_pFieldEntities[g_FieldEntityModel[entity]].dirLock = 0;
    g_pFieldEntities[g_FieldEntityModel[entity]].moveEndX = x << 12;
    g_pFieldEntities[g_FieldEntityModel[entity]].moveEndY = y << 12;
    g_FieldModelDefaultSpeed[g_FieldEntityModel[entity]] = g_pFieldEntities[g_FieldEntityModel[entity]].moveSpeed;
    Field_MakePoint((FieldVec3i *)a, g_pFieldEntities[g_FieldEntityModel[entity]].posX >> 12, g_pFieldEntities[g_FieldEntityModel[entity]].posY >> 12, g_pFieldEntities[g_FieldEntityModel[entity]].posZ >> 12);
    Field_MakePoint((FieldVec3i *)d, x, y, g_pFieldEntities[g_FieldEntityModel[entity]].posZ >> 12);
    Field_HeadingBetweenPoints((FieldPointTo *)a, (FieldPointTo *)d, &b);
    g_pFieldEntities[g_FieldEntityModel[entity]].moveSpeed = (b << 8) / moveArg;
    if (g_pFieldEntities[g_FieldEntityModel[entity]].moveSpeed > 1536) {
        if (*(char *)&g_pFieldEntities[g_FieldEntityModel[entity]].activeAnimId != FIELD_STANDARD_ANIM_RUN) {
            g_pFieldEntities[g_FieldEntityModel[entity]].activeAnimId = FIELD_STANDARD_ANIM_RUN;
            g_pFieldEntities[g_FieldEntityModel[entity]].animSpeed = 16;
            g_pFieldEntities[g_FieldEntityModel[entity]].animCurrentFrame = 0;
            if (g_pFieldModelSlots != 0) {
                Field_SetModelAnimation(
                    (FieldModel *)&g_pFieldModelSlots[g_FieldEntityModel[entity]],
                    *(char *)&g_pFieldEntities[g_FieldEntityModel[entity]].activeAnimId);
                g_pFieldEntities[g_FieldEntityModel[entity]].animLastFrame =
                    g_pFieldModelSlots[g_FieldEntityModel[entity]].model->lastFrame - 1;
            }
        }
    } else {
        if (*(char *)&g_pFieldEntities[g_FieldEntityModel[entity]].activeAnimId != FIELD_STANDARD_ANIM_WALK) {
            g_pFieldEntities[g_FieldEntityModel[entity]].activeAnimId = FIELD_STANDARD_ANIM_WALK;
            g_pFieldEntities[g_FieldEntityModel[entity]].animSpeed = 16;
            g_pFieldEntities[g_FieldEntityModel[entity]].animCurrentFrame = 0;
            if (g_pFieldModelSlots != 0) {
                Field_SetModelAnimation(
                    (FieldModel *)&g_pFieldModelSlots[g_FieldEntityModel[entity]],
                    *(char *)&g_pFieldEntities[g_FieldEntityModel[entity]].activeAnimId);
                g_pFieldEntities[g_FieldEntityModel[entity]].animLastFrame =
                    g_pFieldModelSlots[g_FieldEntityModel[entity]].model->lastFrame - 1;
            }
        }
    }
    g_FieldModelAnimMode[g_FieldEntityModel[entity]] = FIELD_ANIM_MODE_LOOP;
    g_pFieldEntities[g_FieldEntityModel[entity]].scriptedMoveMode = FIELD_SCRIPTED_MOVE_WALK;
    g_pFieldEntities[g_FieldEntityModel[entity]].actionState = FIELD_MOVE_STATE_START;
}

/*
 * Reports a follower's walk as finished (1) once it has arrived, and then ends it: the move is cleared,
 * the model goes back to idle and gets its own speed back. No absent-model guard.
 */
// FUNCTION: FF7 0x0061da7a
int __cdecl Field_TakeMoveDone(short entity)
{
    if (g_pFieldEntities[g_FieldEntityModel[entity]].actionState == FIELD_MOVE_STATE_DONE) {
        g_pFieldEntities[g_FieldEntityModel[entity]].scriptedMoveMode = FIELD_SCRIPTED_MOVE_NONE;
        g_pFieldEntities[g_FieldEntityModel[entity]].actionState = FIELD_MOVE_STATE_START;
        g_FieldModelAnimMode[g_FieldEntityModel[entity]] = FIELD_ANIM_MODE_IDLE;
        g_pFieldEntities[g_FieldEntityModel[entity]].moveSpeed =
            g_FieldModelDefaultSpeed[g_FieldEntityModel[entity]];
        return 1;
    }
    return 0;
}

/*
 * Starts a follower's turn from facing `from` to `to` over 16 steps (smooth), the shorter way round: a
 * difference of more than half a turn is taken the other way (exactly half a turn is left as it is). An
 * entity without a model is skipped.
 */
// FUNCTION: FF7 0x0061db2c
void __cdecl Field_StartEntityTurn(short entity, unsigned char from, unsigned char to)
{
    short val;

    if (g_FieldEntityModel[entity] == 0xFF) {
        return;
    }
    g_pFieldEntities[g_FieldEntityModel[entity]].turnStart = (unsigned short)from;
    g_pFieldEntities[g_FieldEntityModel[entity]].turnType = FIELD_INTERP_SMOOTH;
    g_pFieldEntities[g_FieldEntityModel[entity]].turnStep = 0;
    g_pFieldEntities[g_FieldEntityModel[entity]].turnSteps = 16;
    g_pFieldEntities[g_FieldEntityModel[entity]].turnEnd = (unsigned short)to;
    val = g_pFieldEntities[g_FieldEntityModel[entity]].turnEnd - g_pFieldEntities[g_FieldEntityModel[entity]].turnStart;
    if (val < 0) {
        val ^= 0xFFFF;
        val++;
    }
    if (val > 128) {
        if (g_pFieldEntities[g_FieldEntityModel[entity]].turnEnd > g_pFieldEntities[g_FieldEntityModel[entity]].turnStart) {
            g_pFieldEntities[g_FieldEntityModel[entity]].turnEnd += 0xFF00;
        } else {
            g_pFieldEntities[g_FieldEntityModel[entity]].turnEnd += 0x100;
        }
    }
}

/*
 * Reports a follower's turn as finished (1) once it is done, and clears it; an entity without a model
 * counts as finished.
 */
// FUNCTION: FF7 0x0061dd0e
int __cdecl Field_TakeTurnDone(short entity)
{
    if (g_FieldEntityModel[entity] == 0xff) {
        return 1;
    }
    if (g_pFieldEntities[g_FieldEntityModel[entity]].turnType == FIELD_INTERP_DONE) {
        g_pFieldEntities[g_FieldEntityModel[entity]].turnType = FIELD_INTERP_NONE;
        g_pFieldEntities[g_FieldEntityModel[entity]].turnStep = 0;
        g_pFieldEntities[g_FieldEntityModel[entity]].turnSteps = 0;
        return 1;
    }
    return 0;
}

/*
 * FADE: starts a screen fade with colour red v[3]/1, green v[4]/2, blue v[5]/4, speed [6], fade type [7]
 * (FIELD_FADE_*) and starting strength [8] (one more for the outgoing types). Does not wait (FADEW does).
 * 9 bytes.
 *
 * Two per-map fixes are written into the handler: on maps 0x14C and 0x2D0 a type-1 fade with pure white,
 * and on map 0x8B a type-2 fade with grey 0xFA, are cancelled. A type-1 or type-2 fade in a colour that is
 * neither near black nor near white (any channel between 20 and 220) sets g_FieldFxStepCounter, and types
 * 7 and 8 set g_FieldFxFlagF4; neither latch is cleared here.
 *
 * Field opcode 0x6B (FIELD_OP_SCREEN_FADE).
 */
// FUNCTION: FF7 0x0061ddb4
int __cdecl Script_OpScreenFade(void)
{
    g_pFieldState->fadeType = SCRIPT_PARAM_U8(7);
    switch (g_pFieldState->fadeType) {
    case FIELD_FADE_NONE:
        break;
    case FIELD_FADE_SCALED_COMPLEMENT_OUT:
    case FIELD_FADE_SCALED_OUT:
    case FIELD_FADE_FLAT_COMPLEMENT_OUT:
    case FIELD_FADE_FLAT_OUT:
        g_pFieldState->fadeAdjust = SCRIPT_PARAM_U8(8) + 1;
        break;
    case FIELD_FADE_SCALED_COMPLEMENT_IN:
    case FIELD_FADE_SCALED_IN:
    case FIELD_FADE_FLAT_COMPLEMENT_IN:
    case FIELD_FADE_FLAT_IN:
        g_pFieldState->fadeAdjust = SCRIPT_PARAM_U8(8);
    }
    g_pFieldState->fadeSpeed = SCRIPT_PARAM_U8(6);
    g_pFieldState->red = (unsigned char)Script_ReadOperand8(1, 3);
    g_pFieldState->green = (unsigned char)Script_ReadOperand8(2, 4);
    g_pFieldState->blue = (unsigned char)Script_ReadOperand8(4, 5);
    if (g_pFieldState->fadeType == FIELD_FADE_SCALED_COMPLEMENT_OUT || g_pFieldState->fadeType == FIELD_FADE_SCALED_COMPLEMENT_IN) {
        if ((g_pFieldState->red < 220 && g_pFieldState->red > 20) || (g_pFieldState->green < 220 && g_pFieldState->green > 20) || (g_pFieldState->blue < 220 && g_pFieldState->blue > 20)) {
            g_FieldFxStepCounter = 1;
        }
    }
    // Per-map fixes: these fades are cancelled.
    if (g_FieldCurrentMapId == 0x14C || g_FieldCurrentMapId == 0x2D0) {
        if (g_pFieldState->fadeType == FIELD_FADE_SCALED_COMPLEMENT_OUT && g_pFieldState->red == 0xFF && g_pFieldState->green == 0xFF && g_pFieldState->blue == 0xFF) {
            g_pFieldState->fadeType = FIELD_FADE_NONE;
        }
    }
    if (g_FieldCurrentMapId == 0x8B && g_pFieldState->fadeType == FIELD_FADE_SCALED_COMPLEMENT_IN && g_pFieldState->red == 0xFA && g_pFieldState->green == 0xFA && g_pFieldState->blue == 0xFA) {
        g_pFieldState->fadeType = FIELD_FADE_NONE;
    }
    if (g_pFieldState->fadeType == FIELD_FADE_FLAT_COMPLEMENT_OUT || g_pFieldState->fadeType == FIELD_FADE_FLAT_COMPLEMENT_IN) {
        g_FieldFxFlagF4 = 1;
    }
    g_FieldScriptPC[g_CurrentEntity] += 9;
    return 0;
}

/*
 * NFADE: sets up a fade the field effects run without the script waiting: fade type [3], target colour red
 * v[4]/1, green v[5]/2, blue v[6]/3, speed v[7]/4 (a word). 9 bytes.
 *
 * Field opcode 0x25 (FIELD_OP_FADE_PARAMS).
 */
// FUNCTION: FF7 0x0061e099
int __cdecl Script_OpFadeParams(void)
{
    g_pFieldState->fadeType = SCRIPT_PARAM_U8(3);
    g_pFieldState->fadeRedTarget = Script_ReadOperand8(1, 4);
    g_pFieldState->fadeGreenTarget = Script_ReadOperand8(2, 5);
    g_pFieldState->fadeBlueTarget = Script_ReadOperand8(3, 6);
    g_pFieldState->fadeAdjust = 0;
    g_pFieldState->fadeSpeed = Script_ReadOperand16(4, 7);
    g_FieldScriptPC[g_CurrentEntity] += 9;
    return 0;
}

/*
 * FADEW: waits until the screen fade has finished: an outgoing fade when its strength is back at 0, an
 * incoming one when it has reached 255, the timed kinds when their counter meets the speed. No fade (type
 * 0) or type 4 carries on at once. 1 byte.
 *
 * Field opcode 0x6C (FIELD_OP_WAIT_FADE).
 */
// FUNCTION: FF7 0x0061e15c
int __cdecl Script_OpWaitFade(void)
{
    switch (g_pFieldState->fadeType) {
    case FIELD_FADE_NONE:
    case FIELD_FADE_HIDE_FIELD:
        g_FieldScriptPC[g_CurrentEntity] += 1;
        return 0;
    case FIELD_FADE_SCALED_COMPLEMENT_OUT:
    case FIELD_FADE_SCALED_OUT:
    case FIELD_FADE_FLAT_COMPLEMENT_OUT:
    case FIELD_FADE_FLAT_OUT:
        if (g_pFieldState->fadeAdjust == 0) {
            g_FieldScriptPC[g_CurrentEntity] += 1;
            return 0;
        }
        return 1;
    case FIELD_FADE_SCALED_COMPLEMENT_IN:
    case FIELD_FADE_SCALED_IN:
    case FIELD_FADE_FLAT_COMPLEMENT_IN:
    case FIELD_FADE_FLAT_IN:
        if (g_pFieldState->fadeAdjust >= 255) {
            g_FieldScriptPC[g_CurrentEntity] += 1;
            return 0;
        }
        return 1;
    case FIELD_FADE_BLACK_RAMP_LONG:
    default:
        if (g_pFieldState->fadeAdjust == g_pFieldState->fadeSpeed) {
            g_FieldScriptPC[g_CurrentEntity] += 1;
            return 0;
        }
        return 1;
    }
}

/*
 * IDLCK: locks ([3] nonzero) or unlocks walkmesh triangle [1..2] (a literal word): the player cannot step
 * onto a locked triangle. 4 bytes.
 *
 * The id is a signed short divided with signed arithmetic, so an id of 0x8000 or more would index
 * backwards out of the lock bitmap; real walkmeshes are far smaller.
 *
 * Field opcode 0x6D (FIELD_OP_SET_ACCESS_BLOCKED).
 */
// FUNCTION: FF7 0x0061e29f
int __cdecl Script_OpSetAccessBlocked(void)
{
    short flag;
    short byteIx;
    short bit;

    flag = SCRIPT_PARAM_U8(1);
    flag |= SCRIPT_PARAM_U8(2) << 8;
    byteIx = flag / 8;
    bit = flag % 8;
    if (SCRIPT_PARAM_U8(3) != 0) {
        g_pFieldState->blockedAccesses[byteIx] |= 1 << bit;
    } else {
        g_pFieldState->blockedAccesses[byteIx] &= (1 << bit) ^ 0xff;
    }
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * GWCOL: reads corner v[3]/1 (0..3) of the message window colour gradient into red v[4]/2, green v[5]/3,
 * blue v[6]/4. 7 bytes.
 *
 * Field opcode 0x56 (FIELD_OP_GET_WINDOW_COLOUR).
 */
// FUNCTION: FF7 0x0061e3c6
int __cdecl Script_OpGetWindowColour(void)
{
    short corner;

    corner = Script_ReadOperand8(1, 3);
    corner *= 3;
    Script_WriteOperand8(2, 4, g_FieldWindowColours[corner]);
    Script_WriteOperand8(3, 5, g_FieldWindowColours[corner + 1]);
    Script_WriteOperand8(4, 6, g_FieldWindowColours[corner + 2]);
    g_FieldScriptPC[g_CurrentEntity] += 7;
    return 0;
}

/*
 * SWCOL: sets corner v[3]/1 (0..3) of the message window colour gradient to red v[4]/2, green v[5]/3, blue
 * v[6]/4. 7 bytes.
 *
 * Field opcode 0x57 (FIELD_OP_SET_WINDOW_COLOUR).
 */
// FUNCTION: FF7 0x0061e459
int __cdecl Script_OpSetWindowColour(void)
{
    short corner;

    corner = Script_ReadOperand8(1, 3);
    corner *= 3;
    g_FieldWindowColours[corner] = Script_ReadOperand8(2, 4);
    g_FieldWindowColours[corner + 1] = Script_ReadOperand8(3, 5);
    g_FieldWindowColours[corner + 2] = Script_ReadOperand8(4, 6);
    g_FieldScriptPC[g_CurrentEntity] += 7;
    return 0;
}

/*
 * LSTMP: writes FieldState.mapId to v[2]/2 (by the community reference, the map the party came from). 3
 * bytes.
 *
 * Field opcode 0x6E (FIELD_OP_GET_FIELD_MAP_ID).
 */
// FUNCTION: FF7 0x0061e4e8
int __cdecl Script_OpGetFieldMapId(void)
{
    Script_WriteOperand16(2, 2, g_pFieldState->mapId);
    g_FieldScriptPC[g_CurrentEntity] += 3;
    return 0;
}

/*
 * SHAKE: shakes the screen. Bit 0 of [3] shakes it sideways with amplitude v[4]/1 and v[5]/2 steps per
 * swing, bit 1 up and down with v[6]/3 and v[7]/4; an axis whose bit is clear stops shaking (its old
 * values kept). 8 bytes.
 *
 * Field opcode 0x5E (FIELD_OP_SHAKE).
 */
// FUNCTION: FF7 0x0061e529
int __cdecl Script_OpShake(void)
{
    short flags;

    flags = SCRIPT_PARAM_U8(3);
    if ((flags & 1) != 0) {
        g_pFieldState->shakeX.enabled = 1;
        g_pFieldState->shakeX.amplitude = Script_ReadOperand8(1, 4);
        g_pFieldState->shakeX.numStepsPerSegment = Script_ReadOperand8(2, 5);
    } else {
        g_pFieldState->shakeX.enabled = 0;
    }
    if ((flags & 2) != 0) {
        g_pFieldState->shakeY.enabled = 1;
        g_pFieldState->shakeY.amplitude = Script_ReadOperand8(3, 6);
        g_pFieldState->shakeY.numStepsPerSegment = Script_ReadOperand8(4, 7);
    } else {
        g_pFieldState->shakeY.enabled = 0;
    }
    g_FieldScriptPC[g_CurrentEntity] += 8;
    return 0;
}

/*
 * An inventory entry: the item id in the low nine bits, the quantity (up to 99, 0x7F at most) in the
 * top seven. FIELD_ITEM_QTY_99 is a quantity of 99 with id 0, which SPECIAL FLITM and CLITM OR in.
 */
#define FIELD_ITEM_QTY_SHIFT 9
#define FIELD_ITEM_QTY(entry) (((entry) >> 9) & 0x7f)
#define FIELD_ITEM_QTY_99 0xC600

/*
 * STITM: gives the party v[4]/2 of item v[2]/1 (a word). The inventory keeps each entry as id | quantity
 * << FIELD_ITEM_QTY_SHIFT, the packing passed here. 5 bytes.
 *
 * Field opcode 0x58 (FIELD_OP_ADD_ITEM).
 */
// FUNCTION: FF7 0x0061e639
int __cdecl Script_OpAddItem(void)
{
    short item;

    item = Script_ReadOperand8(2, 4) << FIELD_ITEM_QTY_SHIFT;
    item |= Script_ReadOperand16(1, 2);
    Savemap_AddItem(item);
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * DLITM: takes v[4]/2 of item v[2]/1 away. 5 bytes.
 *
 * Field opcode 0x59 (FIELD_OP_REMOVE_ITEM).
 */
// FUNCTION: FF7 0x0061e6a2
int __cdecl Script_OpRemoveItem(void)
{
    short item;

    item = Script_ReadOperand8(2, 4) << FIELD_ITEM_QTY_SHIFT;
    item |= Script_ReadOperand16(1, 2);
    /* Savemap_RemoveItem takes an int; this call pushes the short unextended. */
    ((short(__cdecl *)(short))Savemap_RemoveItem)(item);
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * CKITM: writes how many of item v[2]/1 the party carries (0 for none) to v[4]/2. 5 bytes.
 *
 * Field opcode 0x5A (FIELD_OP_GET_ITEM_QTY).
 */
// FUNCTION: FF7 0x0061e70b
int __cdecl Script_OpGetItemQty(void)
{
    short item;

    item = Script_ReadOperand16(1, 2);
    item = Savemap_FindItem(item);
    if ((unsigned short)item == 0xffff) {
        item = 0;
    }
    Script_WriteOperand8(2, 4, FIELD_ITEM_QTY((unsigned short)item));
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * SPECIAL: eleven odd jobs behind one opcode, chosen by [1] (community mnemonics):
 *
 * - 0xF5 ARROW: [2] nonzero hides the hand cursor (g_FieldMovieRequested, which MOVIE also sets). 3 bytes.
 * - 0xF6 PNAME: copies [5] bytes of the name of character v[3]/3 into script variables, ending them with
 *   0xFF. 6 bytes.
 * - 0xF7 GMSPD: reads the message speed into v[3]/4; the savemap keeps it inverted. 4 bytes.
 * - 0xF8 SMSPD: sets the message speed from v[3]/4. 4 bytes.
 * - 0xF9 FLMAT: gives one of every materia, ids 0..79. 2 bytes.
 * - 0xFA FLITM: gives 99 of every item, ids 0..511. 2 bytes.
 * - 0xFB BTLCK: [2] nonzero locks battles (g_FieldMoviePending): the engine then skips the battle set-up
 *   from the field and the world map holds its encounters off. 3 bytes.
 * - 0xFC MVLCK: [2] nonzero holds the movies off (g_FieldMoviePlaying): PMVIE and MOVIE step over
 *   themselves and MVIEF counts frames by itself. 3 bytes.
 * - 0xFD SPCNM: sets character [2]'s name to dialogue [3] of this map's text. 4 bytes.
 * - 0xFE RSGLB: resets the savemap for a new game. 2 bytes.
 * - 0xFF CLITM: takes 99 of every item away. 2 bytes.
 *
 * Any other job is a two-byte no-op.
 *
 * BUG (PNAME): the destination operand [4] is never read, so the name always lands at the start of the
 * bank; and only the byte code of each bank pair (low nibble of [2]: 3, 11, 13, 15) picks its bank, so the
 * word codes 4, 12, 14 and 7 write into the first bank. The copy does not stop at the name's end or the
 * bank's.
 *
 * Field opcode 0x0F (FIELD_OP_SPECIAL).
 */
// FUNCTION: FF7 0x0061e78c
int __cdecl Script_OpSpecial(void)
{
    int namePtr;
    short itemId;
    int i;
    unsigned short len;
    unsigned short bankOffset;

    switch (SCRIPT_PARAM_U8(1)) {
    case FIELD_SPECIAL_REMOVE_ALL_ITEMS:
        for (i = 0; i < 512; i++) {
            itemId = i;
            *(unsigned short *)&itemId |= FIELD_ITEM_QTY_99;
            /* Pushed unextended, as in Script_OpRemoveItem. */
            ((short(__cdecl *)(short))Savemap_RemoveItem)(itemId);
        }
        g_FieldScriptPC[g_CurrentEntity] += 2;
        return 0;
    case FIELD_SPECIAL_RESET_SAVEMAP:
        Savemap_ResetForNewGame();
        g_FieldScriptPC[g_CurrentEntity] += 2;
        return 0;
    case FIELD_SPECIAL_SET_CHARACTER_NAME:
        Field_SetCharacterNameFromDialogueString(SCRIPT_PARAM_U8(2), SCRIPT_PARAM_U8(3));
        g_FieldScriptPC[g_CurrentEntity] += 4;
        return 0;
    case FIELD_SPECIAL_SET_MOVIE_PLAYING:
        g_FieldMoviePlaying = SCRIPT_PARAM_S8(2);
        g_FieldScriptPC[g_CurrentEntity] += 3;
        return 0;
    case FIELD_SPECIAL_SET_MOVIE_PENDING:
        g_FieldMoviePending = SCRIPT_PARAM_S8(2);
        g_FieldScriptPC[g_CurrentEntity] += 3;
        return 0;
    case FIELD_SPECIAL_ADD_ALL_ITEMS:
        for (i = 0; i < 512; i++) {
            itemId = i;
            *(unsigned short *)&itemId |= FIELD_ITEM_QTY_99;
            Savemap_AddItem((short)itemId);
        }
        g_FieldScriptPC[g_CurrentEntity] += 2;
        return 0;
    case FIELD_SPECIAL_ADD_ALL_MATERIA:
        for (i = 0; i < 80; i++) {
            Savemap_AddMateria(i);
        }
        g_FieldScriptPC[g_CurrentEntity] += 2;
        return 0;
    case FIELD_SPECIAL_SET_MESSAGE_SPEED:
        gSavemap.fieldMessageSpeed = 0xFF - Script_ReadOperand8(4, 3);
        g_FieldScriptPC[g_CurrentEntity] += 4;
        return 0;
    case FIELD_SPECIAL_GET_MESSAGE_SPEED:
        Script_WriteOperand8(4, 3, 0xFF - gSavemap.fieldMessageSpeed);
        g_FieldScriptPC[g_CurrentEntity] += 4;
        return 0;
    case FIELD_SPECIAL_COPY_CHARACTER_NAME:
        namePtr = Savemap_GetCharacterName(Script_ReadOperand8(3, 3));
        len = SCRIPT_PARAM_U8(5);
        bankOffset = 0;
        // BUG: the destination [4] is never read, and only the byte codes pick a bank.
        switch (SCRIPT_PARAM_U8(2) & 0xF) {
        case FIELD_BANK_SAVE_400_BYTE:
            bankOffset += 0x100;
        case FIELD_BANK_SAVE_300_BYTE:
            bankOffset += 0x100;
        case FIELD_BANK_SAVE_200_BYTE:
            bankOffset += 0x100;
        case FIELD_BANK_SAVE_100_BYTE:
            bankOffset += 0x100;
        }
        for (i = 0; i < len; i++) {
            gSavemap.scriptBanks[bankOffset + i] = *(char *)namePtr;
            namePtr++;
        }
        gSavemap.scriptBanks[bankOffset + i] = 0xFF;
        g_FieldScriptPC[g_CurrentEntity] += 6;
        return 0;
    case FIELD_SPECIAL_SET_MOVIE_REQUESTED:
        g_FieldMovieRequested = SCRIPT_PARAM_S8(2);
        g_FieldScriptPC[g_CurrentEntity] += 3;
        return 0;
    default:
        g_FieldScriptPC[g_CurrentEntity] += 2;
        return 0;
    }
}

/*
 * BGSCR: sets the scroll speed of background layer [2]: x v[3]/1, y v[5]/2 per frame. Only layers 2 and 3
 * scroll; any other value does nothing. 7 bytes.
 *
 * Field opcode 0x2D (FIELD_OP_SET_LAYER_SCROLL).
 */
// FUNCTION: FF7 0x0061ecac
int __cdecl Script_OpSetLayerScroll(void)
{
    /* Numeric background scroll layer index (2 or 3). */
    switch (SCRIPT_PARAM_U8(2)) {
    case FIELD_BG_SCROLL_LAYER_2:
        g_pFieldState->layer2ScrollXSpeed = Script_ReadOperand16(1, 3);
        g_pFieldState->layer2ScrollYSpeed = Script_ReadOperand16(2, 5);
        break;
    case FIELD_BG_SCROLL_LAYER_3:
        g_pFieldState->layer3ScrollXSpeed = Script_ReadOperand16(1, 3);
        g_pFieldState->layer3ScrollYSpeed = Script_ReadOperand16(2, 5);
        break;
    }
    g_FieldScriptPC[g_CurrentEntity] += 7;
    return 0;
}

/*
 * BGPDH: sets the depth of background layer [2] to v[3]/1. Only layers 2 and 3 take one; any other value
 * does nothing. 5 bytes.
 *
 * Field opcode 0x2C (FIELD_OP_SET_LAYER_DEPTH).
 */
// FUNCTION: FF7 0x0061ed6b
int __cdecl Script_OpSetLayerDepth(void)
{
    /* Numeric background scroll layer index (2 or 3). */
    switch (SCRIPT_PARAM_U8(2)) {
    case FIELD_BG_SCROLL_LAYER_2:
        g_pFieldState->layer2Depth = Script_ReadOperand16(1, 3);
        break;
    case FIELD_BG_SCROLL_LAYER_3:
        g_pFieldState->layer3Depth = Script_ReadOperand16(1, 3);
        break;
    }
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * SMTRA: gives the party a materia, the four bytes v[3]/1..v[6]/4 forming its record (id, then AP); the
 * savemap's materiaResult says whether it was stored (1) or the stock was full (0). 7 bytes.
 *
 * Field opcode 0x5B (FIELD_OP_ADD_MATERIA).
 */
// FUNCTION: FF7 0x0061edf8
int __cdecl Script_OpAddMateria(void)
{
    int slot;
    int materia;

    materia = Script_ReadOperand8(1, 3);
    materia |= Script_ReadOperand8(2, 4) << 8;
    materia |= Script_ReadOperand8(3, 5) << 16;
    materia |= Script_ReadOperand8(4, 6) << 24;
    slot = Savemap_AddMateria(materia);
    if (slot == -1) {
        gSavemap.materiaResult = 0;
    } else {
        gSavemap.materiaResult = 1;
    }
    g_FieldScriptPC[g_CurrentEntity] += 7;
    return 0;
}

/*
 * DMTRA: meant to take [7] of the materia v[3]/1..v[6]/4 away. 8 bytes. On PC the removal routine is a
 * seven-byte `return 0;` shell (see the note at the top of the file), so nothing is removed.
 *
 * Field opcode 0x5C (FIELD_OP_REMOVE_MATERIA).
 */
// FUNCTION: FF7 0x0061eeb4
int __cdecl Script_OpRemoveMateria(void)
{
    int materia;

    materia = Script_ReadOperand8(1, 3);
    materia |= Script_ReadOperand8(2, 4) << 8;
    materia |= Script_ReadOperand8(3, 5) << 16;
    materia |= Script_ReadOperand8(4, 6) << 24;
    Savemap_RemoveMateriaReturnZero(materia, SCRIPT_PARAM_U8(7));
    g_FieldScriptPC[g_CurrentEntity] += 8;
    return 0;
}

/*
 * CMTRA: meant to count the party's materia v[4]/1..v[7]/4 ([8] passed along) into v[9]/6. 10 bytes. On PC
 * the counting routine is a `return 0;` shell, so the count is always 0.
 *
 * Field opcode 0x5D (FIELD_OP_COUNT_MATERIA).
 */
// FUNCTION: FF7 0x0061ef74
int __cdecl Script_OpCountMateria(void)
{
    int count;
    int materia;

    materia = Script_ReadOperand8(1, 4);
    materia |= Script_ReadOperand8(2, 5) << 8;
    materia |= Script_ReadOperand8(3, 6) << 16;
    materia |= Script_ReadOperand8(4, 7) << 24;
    count = Savemap_CountMateriaReturnZero(materia, SCRIPT_PARAM_U8(8));
    Script_WriteOperand8(6, 9, count);
    g_FieldScriptPC[g_CurrentEntity] += 0xa;
    return 0;
}

/*
 * MENU: hands the field over to one of the engine's menus and waits until it hands back: [2] is the field
 * request (FIELD_REQUEST_NAME_ENTRY, _PARTY_SELECT for the PHS, _SHOP, _MAIN_MENU, _SAVE_MENU, and the
 * materia and party services), v[3]/2 its parameter, such as the shop number. 4 bytes. On the way back the
 * field party is pulled in line with the savemap's, since the PHS may have changed it.
 *
 * The main menu with parameter 0 does not wait: the pc moves past the opcode as soon as the request is
 * posted. MENU with request FIELD_REQUEST_STOP_MOVIE while a movie plays turns the movie request into a
 * stop.
 *
 * Field opcode 0x49 (FIELD_OP_FIELD_REQUEST).
 */
// FUNCTION: FF7 0x0061f04a
int __cdecl Script_OpFieldRequest(void)
{
    switch (g_pFieldState->requestCode) {
    case FIELD_REQUEST_NONE:
        g_pFieldState->requestCode = SCRIPT_PARAM_S8(2);
        g_pFieldState->eventCmdParam = (unsigned char)Script_ReadOperand8(2, 3);
        g_pFieldState->requestArg = FIELD_REQUEST_PROGRESS_PENDING;
        g_FieldRequestPosted = 1;
        // The main menu with parameter 0 does not wait for the menu to close.
        if (g_pFieldState->requestCode == FIELD_REQUEST_MAIN_MENU && g_pFieldState->eventCmdParam == 0) {
            g_FieldScriptPC[g_CurrentEntity] += 4;
        }
        return 1;
    }
    if (g_pFieldState->requestCode == SCRIPT_PARAM_U8(2)) {
        if (g_pFieldState->requestArg == FIELD_REQUEST_PROGRESS_DONE) {
            g_FieldScriptPC[g_CurrentEntity] += 4;
            g_pFieldState->requestCode = FIELD_REQUEST_NONE;
            g_pFieldState->requestArg = FIELD_REQUEST_PROGRESS_PENDING;
            Savemap_ReconcileFieldParty();
            return 0;
        }
    } else if (SCRIPT_PARAM_U8(2) == FIELD_REQUEST_STOP_MOVIE && g_pFieldState->requestCode == FIELD_REQUEST_PLAY_MOVIE) {
        // Stop the movie that is playing.
        g_pFieldState->requestCode = SCRIPT_PARAM_S8(2);
        g_pFieldState->requestArg = FIELD_REQUEST_PROGRESS_PENDING;
    }
    return 1;
}

/*
 * MENU2: [1] nonzero stops the Triangle button from opening the main menu, 0 allows it again. 2 bytes.
 *
 * Field opcode 0x4A (FIELD_OP_SET_MENU_LOCKED).
 */
// FUNCTION: FF7 0x0061f1ea
int __cdecl Script_OpSetMenuLocked(void)
{
    g_pFieldState->menuLocked = SCRIPT_PARAM_U8(1);
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * GETPC: reads the character in active party slot [2] into v[3]/2 (0xFF for an empty slot). 4 bytes.
 *
 * Out-of-range slots write nothing and are not an error; the program counter advances either way.
 *
 * Field opcode 0x74 (FIELD_OP_GET_PARTY_CHAR_ID).
 */
// FUNCTION: FF7 0x0061f238
int __cdecl Script_OpGetPartyCharId(void)
{
    short slot;

    slot = SCRIPT_PARAM_U8(2);
    if (slot < 3) {
        slot = gSavemap.party[slot];
        Script_WriteOperand8(2, 3, slot);
    }
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * MPARA: ties message variable v[3]/1 of window [2] to a value, which the text shows where the dialogue
 * has that variable's code: the low nibble of [1] is the bank and [4] the operand, so with bank 0 the byte
 * [4] is the value itself, otherwise the script variable it indexes, read when the text is drawn. 5 bytes.
 *
 * Field opcode 0x41 (FIELD_OP_WINDOW_BIND_PARAM_8).
 */
// FUNCTION: FF7 0x0061f2b0
int __cdecl Script_OpWindowBindParam8(void)
{
    short param;
    short wnd;

    wnd = SCRIPT_PARAM_U8(2);
    param = Script_ReadOperand8(1, 3);
    g_WindowReplaceBank[wnd][param] = SCRIPT_PARAM_U8(1) & 0xf;
    g_WindowReplaceBankAddr[wnd][param] = (unsigned short)SCRIPT_PARAM_U8(4);
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * MPRA2: MPARA with a 16-bit operand [4..5]: with bank 0 a word value. 6 bytes.
 *
 * Field opcode 0x42 (FIELD_OP_WINDOW_BIND_PARAM_16).
 */
// FUNCTION: FF7 0x0061f377
int __cdecl Script_OpWindowBindParam16(void)
{
    short param;
    short wnd;

    wnd = SCRIPT_PARAM_U8(2);
    param = Script_ReadOperand8(1, 3);
    g_WindowReplaceBank[wnd][param] = SCRIPT_PARAM_U8(1) & 0xf;
    g_WindowReplaceBankAddr[wnd][param] = (unsigned short)SCRIPT_PARAM_U8(4);
    g_WindowReplaceBankAddr[wnd][param] |= SCRIPT_PARAM_U8(5) << 8;
    g_FieldScriptPC[g_CurrentEntity] += 6;
    return 0;
}

/*
 * SIN: v[9]/4 = (sin(v[3]/1) * v[5]/2 + v[7]/3) >> 12, with angles and sines in 1/4096 units; note that
 * the offset is added before the shift. 10 bytes.
 *
 * `sar` on the way out, so the shift is signed and written as one -- the divide idiom would have a
 * rounding correction in front of it.
 *
 * Field opcode 0xD4 (FIELD_OP_SIN_SCALED).
 */
// FUNCTION: FF7 0x0061f48c
int __cdecl Script_OpSinScaled(void)
{
    int value;

    value = SinFixed(Script_ReadOperand16(1, 3));
    value *= Script_ReadOperand16(2, 5);
    value += Script_ReadOperand16(3, 7);
    Script_WriteOperand16(4, 9, value >> 12);
    g_FieldScriptPC[g_CurrentEntity] += 0xa;
    return 0;
}

/*
 * COS: v[9]/4 = (cos(v[3]/1) * v[5]/2 + v[7]/3) >> 12, as SIN. 10 bytes.
 *
 * Field opcode 0xD5 (FIELD_OP_COS_SCALED).
 */
// FUNCTION: FF7 0x0061f516
int __cdecl Script_OpCosScaled(void)
{
    int value;

    value = CosFixed(Script_ReadOperand16(1, 3));
    value *= Script_ReadOperand16(2, 5);
    value += Script_ReadOperand16(3, 7);
    Script_WriteOperand16(4, 9, value >> 12);
    g_FieldScriptPC[g_CurrentEntity] += 0xa;
    return 0;
}

/*
 * MHMMX: fully restores every character, not only the party: HP and MP to the maximum and status cleared
 * (Field_HealPartyToFull), by putting characters 0..2, 3..5 and 6..8 into the active party in turn and
 * then the real party back. 1 byte.
 *
 * Transient global party changes are visible to the helpers.
 *
 * Field opcode 0x3E (FIELD_OP_HEAL_EVERYONE).
 */
// FUNCTION: FF7 0x0061f5a0
int __cdecl Script_OpHealEveryone(void)
{
    unsigned char saved[3];
    int n;

    for (n = 0; n < 3; n++) {
        saved[n] = gSavemap.party[n];
    }
    for (n = 0; n < 3; n++) {
        gSavemap.party[n] = n;
    }
    Field_HealPartyToFull();
    for (n = 0; n < 3; n++) {
        gSavemap.party[n] = n + 3;
    }
    Field_HealPartyToFull();
    for (n = 0; n < 3; n++) {
        gSavemap.party[n] = n + 6;
    }
    Field_HealPartyToFull();
    for (n = 0; n < 3; n++) {
        gSavemap.party[n] = saved[n];
    }
    Field_HealPartyToFull();
    g_FieldScriptPC[g_CurrentEntity] += 1;
    return 0;
}

/* The HP and MP a full heal adds: more than any character's maximum, so the helpers' cap decides. */
#define FIELD_HEAL_FULL 0x2710

/*
 * Fully restores the three members of the active party: reloads their stats, heals FIELD_HEAL_FULL (10000,
 * more than any maximum) HP and MP and clears their status.
 *
 * The guard against a save slot above 8 never fires: the id-to-slot table only holds 0..8
 */
// FUNCTION: FF7 0x0061f6b0
void __cdecl Field_HealPartyToFull(void)
{
    int slot;
    int i;
    int charId;

    Party_ReloadCombatStats();
    for (i = 0; i < 3; i++) {
        Savemap_HealHp(i, FIELD_HEAL_FULL);
        Savemap_HealMp(i, FIELD_HEAL_FULL);
        charId = gSavemap.party[i];
        if (charId == PARTY_CHAR_NONE) {
            continue;
        }
        slot = g_CharIdToSaveSlot[charId];
        if (slot > 8) {
            continue;
        }
        gSavemap.chars[slot].status = 0;
    }
}

/*
 * Rebuilds the battle stats of the three active party members from their savemap records (equipment and
 * materia included), then the party-wide battle modifiers, so that heals see the current maxima.
 */
// FUNCTION: FF7 0x0061f739
void __cdecl Party_ReloadCombatStats(void)
{
    short i;

    for (i = 0; i < 3; i++) {
        if (gSavemap.party[i] == PARTY_CHAR_NONE) {
            continue;
        }
        Savemap_BuildPartyStatsFromRecord(i);
        Battle_LoadCombatantFromSavemap((unsigned char)i);
    }
    Party_RecomputeBattleModifiers();
}

/*
 * HMPMAX1 / HMPMAX2 / HMPMAX3: restores the party's HP and MP to the maximum; status ailments stay. 1
 * byte.
 *
 * Field opcodes 0x3C, 0x3D and 0x3F (FIELD_OP_FULL_HEAL_3C, FIELD_OP_FULL_HEAL_3D, FIELD_OP_FULL_HEAL_3F).
 */
// FUNCTION: FF7 0x0061f793
int __cdecl Script_OpFullHeal(void)
{
    int i;

    Party_ReloadCombatStats();
    for (i = 0; i < 3; i++) {
        Savemap_HealHp(i, FIELD_HEAL_FULL);
        Savemap_HealMp(i, FIELD_HEAL_FULL);
    }
    g_FieldScriptPC[g_CurrentEntity] += 1;
    return 0;
}

/*
 * MPu: restores v[3]/2 MP to the character in field party slot [2], if that character is also in the
 * active party. 5 bytes. The slot is not range-checked.
 *
 * Field opcode 0x45 (FIELD_OP_HEAL_MP).
 */
// FUNCTION: FF7 0x0061f801
int __cdecl Script_OpHealMp(void)
{
    int i;
    int charId;

    Party_ReloadCombatStats();
    charId = SCRIPT_PARAM_U8(2);
    if (gSavemap.fieldParty[charId] != PARTY_CHAR_NONE) {
        charId = gSavemap.fieldParty[charId];
        for (i = 0; i < 3; i++) {
            if (gSavemap.party[i] == charId) {
                Savemap_HealMp(i, Script_ReadOperand16(2, 3));
            }
        }
    }
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * MPd: takes v[3]/2 MP from the character in field party slot [2]. 5 bytes.
 *
 * Field opcode 0x47 (FIELD_OP_DAMAGE_MP).
 */
// FUNCTION: FF7 0x0061f8bc
int __cdecl Script_OpDamageMp(void)
{
    int i;
    int charId;

    Party_ReloadCombatStats();
    charId = SCRIPT_PARAM_U8(2);
    if (gSavemap.fieldParty[charId] != PARTY_CHAR_NONE) {
        charId = gSavemap.fieldParty[charId];
        for (i = 0; i < 3; i++) {
            if (gSavemap.party[i] == charId) {
                Savemap_DamageMp(i, Script_ReadOperand16(2, 3));
            }
        }
    }
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * HPu: restores v[3]/2 HP to the character in field party slot [2], if that character is also in the
 * active party. 5 bytes. The slot is not range-checked.
 *
 * Field opcode 0x4D (FIELD_OP_HEAL_HP).
 */
// FUNCTION: FF7 0x0061f977
int __cdecl Script_OpHealHp(void)
{
    int i;
    int charId;

    Party_ReloadCombatStats();
    charId = SCRIPT_PARAM_U8(2);
    if (gSavemap.fieldParty[charId] != PARTY_CHAR_NONE) {
        charId = gSavemap.fieldParty[charId];
        for (i = 0; i < 3; i++) {
            if (gSavemap.party[i] == charId) {
                Savemap_HealHp(i, Script_ReadOperand16(2, 3));
            }
        }
    }
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * HPd: takes v[3]/2 HP from the character in field party slot [2]. 5 bytes.
 *
 * Field opcode 0x4F (FIELD_OP_DAMAGE_HP).
 */
// FUNCTION: FF7 0x0061fa32
int __cdecl Script_OpDamageHp(void)
{
    int i;
    int charId;

    Party_ReloadCombatStats();
    charId = SCRIPT_PARAM_U8(2);
    if (gSavemap.fieldParty[charId] != PARTY_CHAR_NONE) {
        charId = gSavemap.fieldParty[charId];
        for (i = 0; i < 3; i++) {
            if (gSavemap.party[i] == charId) {
                Savemap_DamageHp(i, Script_ReadOperand16(2, 3));
            }
        }
    }
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * GOLDU: adds gil, a 32-bit amount from the low word v[2]/1 and the high word v[4]/2. 6 bytes.
 *
 * Field opcode 0x39 (FIELD_OP_ADD_GIL).
 */
// FUNCTION: FF7 0x0061faed
int __cdecl Script_OpAddGil(void)
{
    unsigned int amount;

    amount = (unsigned short)Script_ReadOperand16(1, 2);
    amount |= (unsigned short)Script_ReadOperand16(2, 4) << 16;
    Savemap_AddGil(amount);
    g_FieldScriptPC[g_CurrentEntity] += 6;
    return 0;
}

/*
 * GOLDD: takes gil away, a 32-bit amount from the low word v[2]/1 and the high word v[4]/2. 6 bytes.
 *
 * Field opcode 0x3A (FIELD_OP_SPEND_GIL).
 */
// FUNCTION: FF7 0x0061fb56
int __cdecl Script_OpSpendGil(void)
{
    unsigned int amount;

    amount = (unsigned short)Script_ReadOperand16(1, 2);
    amount |= (unsigned short)Script_ReadOperand16(2, 4) << 16;
    Savemap_SpendGil(amount);
    g_FieldScriptPC[g_CurrentEntity] += 6;
    return 0;
}

/*
 * CHGLD: reads the party's gil into two words, low v[2]/1 and high v[3]/2. 4 bytes.
 *
 * Field opcode 0x3B (FIELD_OP_GET_GIL).
 */
// FUNCTION: FF7 0x0061fbbf
int __cdecl Script_OpGetGil(void)
{
    unsigned int gil;

    gil = Savemap_GetGil();
    Script_WriteOperand16(1, 2, gil & 0xffff);
    Script_WriteOperand16(2, 3, (gil >> 16) & 0xffff);
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * CHMPH: reads two numbers from the field effects block into variables: the word at +0xA4 into v[2]/1 and
 * the byte at +0x9E into v[3]/2. 4 bytes.
 *
 * Field opcode 0xF7 (FIELD_OP_GET_FIELD_FX_PARAMS).
 */
// FUNCTION: FF7 0x0061fc23
int __cdecl Script_OpGetFieldFxParams(void)
{
    Script_WriteOperand16(
        1, 2, g_pFieldFx->paramWord);
    Script_WriteOperand8(
        2, 3, g_pFieldFx->paramByte);
    g_FieldScriptPC[g_CurrentEntity] += 4;
    return 0;
}

/*
 * CHMST: writes 1 to v[2]/2 if any music is playing (wave or MIDI), else 0. 3 bytes.
 *
 * Field opcode 0xFE (FIELD_OP_GET_MUSIC_PLAYING).
 */
// FUNCTION: FF7 0x0061fc7f
int __cdecl Script_OpGetMusicPlaying(void)
{
    unsigned char playing;

    if (MusicWaveIsPlaying() != 0 || MidiIsPlaying() != 0) {
        playing = 1;
    } else {
        playing = 0;
    }
    Script_WriteOperand8(2, 2, playing);
    g_FieldScriptPC[g_CurrentEntity] += 3;
    return 0;
}

/*
 * STTIM: sets the countdown timer to hours v[3]/1, minutes v[4]/2 and seconds v[5]/4 (kept in the savemap
 * as seconds). 6 bytes.
 *
 * Field opcode 0x38 (FIELD_OP_SET_COUNTDOWN).
 */
// FUNCTION: FF7 0x0061fcd8
int __cdecl Script_OpSetCountdown(void)
{
    int seconds;

    seconds = Script_ReadOperand8(1, 3) * 60 * 60;
    seconds += Script_ReadOperand8(2, 4) * 60;
    seconds += Script_ReadOperand8(4, 5);
    gSavemap.countdownSeconds = seconds;
    g_FieldScriptPC[g_CurrentEntity] += 6;
    return 0;
}

/*
 * WSPCL: puts a number display in window [1]: type [2] (by the community reference 1 the countdown clock,
 * 2 a number) at x [3], y [4] inside the window. 5 bytes.
 *
 * Field opcode 0x36 (FIELD_OP_WINDOW_SET_NUMBER_DISPLAY).
 */
// FUNCTION: FF7 0x0061fd5c
int __cdecl Script_OpWindowSetNumberDisplay(void)
{
    int win;

    win = SCRIPT_PARAM_U8(1);
    g_FieldWindows[win].numDisplayType = SCRIPT_PARAM_U8(2);
    g_FieldWindows[win].numDisplayX = (short)SCRIPT_PARAM_U8(3);
    g_FieldWindows[win].numDisplayY = (short)SCRIPT_PARAM_U8(4);
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * WNUMB: sets the number window [2] shows: a 32-bit value from the low word v[3]/1 and the high word
 * v[5]/2, with [7] digits. 8 bytes.
 *
 * BUG: the low word is read signed and not masked, so a low half of 0x8000 or more sign-extends over the
 * high half: such a number shows with its upper 16 bits all set. GOLDU, which builds its amount the same
 * way, casts to unsigned short first.
 *
 * Field opcode 0x37 (FIELD_OP_WINDOW_SET_NUMBER).
 */
// FUNCTION: FF7 0x0061fe26
int __cdecl Script_OpWindowSetNumber(void)
{
    int value;
    int win;

    win = SCRIPT_PARAM_U8(2);
    value = Script_ReadOperand16(1, 3);
    // BUG: a low word of 0x8000 or more has already filled the high half with ones.
    value |= Script_ReadOperand16(2, 5) << 16;
    g_FieldWindows[win].numDisplayValue = value;
    g_FieldWindows[win].numDisplayLength = SCRIPT_PARAM_U8(7);
    g_FieldScriptPC[g_CurrentEntity] += 8;
    return 0;
}

/*
 * BTLMD: sets the battle-mode flags the next battle starts with: [1] goes to battleMode2 and [2] to
 * battleMode1. 3 bytes.
 *
 * +0x3D and +0x3E are the PlayStation version's battleMode1 and battleMode2, at the same offsets and the same widths -- u8
 * then u16, which is exactly the pair of stores here.
 *
 * Field opcode 0x72 (FIELD_OP_SET_BATTLE_FLAGS_BYTE).
 */
// FUNCTION: FF7 0x0061fed7
int __cdecl Script_OpSetBattleFlagsByte(void)
{
    g_pFieldState->battleMode2 = SCRIPT_PARAM_U8(1);
    g_pFieldState->battleMode1 = SCRIPT_PARAM_U8(2);
    g_FieldScriptPC[g_CurrentEntity] += 3;
    return 0;
}

/*
 * BTMD2: BTLMD with two literal words: battleMode2 = [1..2], battleMode1 = [3..4]. 5 bytes.
 *
 * BUG: battleMode1 is one byte wide, so the high byte [4] is shifted out of it and never reaches the
 * battle.
 *
 * Field opcode 0x22 (FIELD_OP_SET_BATTLE_FLAGS_WORD).
 */
// FUNCTION: FF7 0x0061ff4d
int __cdecl Script_OpSetBattleFlagsWord(void)
{
    g_pFieldState->battleMode2 = SCRIPT_PARAM_U8(1);
    g_pFieldState->battleMode2 |= SCRIPT_PARAM_U8(2) << 8;
    g_pFieldState->battleMode1 = SCRIPT_PARAM_U8(3);
    // BUG: battleMode1 is a byte, so [4] is shifted out of it.
    g_pFieldState->battleMode1 |= SCRIPT_PARAM_U8(4) << 8;
    g_FieldScriptPC[g_CurrentEntity] += 5;
    return 0;
}

/*
 * BTRLD: reads what the last battle did (g_BattleResultWord) into v[2]/2. 3 bytes.
 *
 * Field opcode 0x23 (FIELD_OP_GET_BATTLE_RESULT).
 */
// FUNCTION: FF7 0x0062002c
int __cdecl Script_OpGetBattleResult(void)
{
    Script_WriteOperand16(2, 2, g_BattleResultWord);
    g_FieldScriptPC[g_CurrentEntity] += 3;
    return 0;
}

/*
 * BTLTB: chooses which of the map's two encounter tables random battles come from: the first when [1] is
 * 0, the second otherwise. 2 bytes.
 *
 * Field opcode 0x4B (FIELD_OP_SET_ENCOUNTER_TABLE).
 */
// FUNCTION: FF7 0x00620069
int __cdecl Script_OpSetEncounterTable(void)
{
    g_pFieldState->encounterTable = SCRIPT_PARAM_U8(1);
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * BLINK: sets this model's blinkOn flag from [1]. 2 bytes.
 *
 * Field opcode 0x26 (FIELD_OP_SET_MODEL_BLINK).
 */
// FUNCTION: FF7 0x006200b7
int __cdecl Script_OpSetModelBlink(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] != 0xff) {
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].blinkOn =
            SCRIPT_PARAM_U8(1);
    }
    g_FieldScriptPC[g_CurrentEntity] += 2;
    return 0;
}

/*
 * KAWAI: runs a model graphics command on this entity's model; [1] is the opcode's length and [2] the
 * sub-command, which the model code keeps (effectPreset) and runs on its next frames with the payload from
 * [3] on (kawaiDataOffset). Sub-command 0 (EYETX by the community reference) sets the eyes at once:
 * payload 1, 1, 0 switches blinking off, any other payload switches it on and, for eye textures 1..2,
 * applies them (Field_ApplyHeadMeshCommands). Does not wait (KAWIW does).
 *
 * The local `busy` holds the sub-command byte.
 *
 * BUG: on an entity without a model, or with no model data loaded, `busy` is never written, the test reads
 * whatever the stack held, and the sub-command 0 path then indexes the model array with 0xFF.
 *
 * Field opcode 0x28 (FIELD_OP_MODEL_GFX_COMMAND).
 */
// FUNCTION: FF7 0x00620136
int __cdecl Script_OpModelGfxCommand(void)
{
    short length;
    unsigned char param;
    unsigned char value;
    unsigned char busy;

    length = SCRIPT_PARAM_U8(1);
    if (g_FieldEntityModel[g_CurrentEntity] != 0xff) {
        if (g_pFieldModelSlots != 0) {
            g_pFieldModelSlots[g_FieldEntityModel[g_CurrentEntity]].effectPreset = SCRIPT_PARAM_U8(2);
            busy = g_pFieldModelSlots[g_FieldEntityModel[g_CurrentEntity]].effectPreset;
        }
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].kawaiOp1 = 1;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].kawaiOp0 = 0;
        g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].kawaiDataOffset =
            (unsigned char *)g_FieldScripts + g_FieldScriptPC[g_CurrentEntity] + 3;
    }
    // Sub-command 0: the eyes. BUG: `busy` is unset without a model or model data.
    if (busy == 0) {
        if (g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].kawaiDataOffset[0] == 1 &&
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].kawaiDataOffset[1] == 1 &&
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].kawaiDataOffset[2] == 0) {
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].blinkOn = 0;
            *(short *)&g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].kawaiB = 0;
        } else {
            param = g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].kawaiDataOffset[0];
            value = g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].kawaiDataOffset[1];
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].blinkOn = 1;
            if (param >= 1 && param <= 2 && value >= 1 && value <= 2) {
                g_FieldHeadMeshCommands[0] = param;
                g_FieldHeadMeshCommands[1] = value;
                g_FieldHeadMeshCommands[2] = 0;
                g_FieldHeadMeshCommands[3] = g_FieldEntityModel[g_CurrentEntity];
                Field_ApplyHeadMeshCommands((FieldActorRecord *)&g_pFieldModelSlots[g_FieldEntityModel[g_CurrentEntity]],
                                            g_FieldHeadMeshCommands);
            }
        }
    }
    g_FieldScriptPC[g_CurrentEntity] += length;
    return 0;
}

/*
 * KAWIW: waits until the KAWAI command running on this model has finished. 1 byte.
 *
 * Field opcode 0x29 (FIELD_OP_WAIT_MODEL_GFX_COMMAND).
 */
// FUNCTION: FF7 0x0062045b
int __cdecl Script_OpWaitModelGfxCommand(void)
{
    if (g_FieldEntityModel[g_CurrentEntity] != 0xff) {
        if (g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].kawaiOp1 != 1) {
            g_pFieldEntities[g_FieldEntityModel[g_CurrentEntity]].kawaiOp1 = 0;
            g_FieldScriptPC[g_CurrentEntity] += 1;
            return 0;
        }
        return 1;
    }
    g_FieldScriptPC[g_CurrentEntity] += 1;
    return 0;
}

} /* extern "C" */
