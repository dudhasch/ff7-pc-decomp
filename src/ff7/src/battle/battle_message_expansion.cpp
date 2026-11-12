// src/ff7/src/battle/battle_message_expansion.cpp
// Contents: Expands escape sequences in encoded battle message bytes, and queues AI message text in
//   a ring buffer that can be read back by slot.

#include "ff7/api/battle.h"
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
extern "C" {

// GLOBAL: FF7 0x009ad1e0
char g_BattleAiMessageBuffer[2048] = {0};

// GLOBAL: FF7 0x009ad9e0
unsigned short g_BattleAiMessageOffsets[64] = {0};

// GLOBAL: FF7 0x009ada60
int g_BattleAiMessageCursor = 0;

// GLOBAL: FF7 0x009ada64
int g_BattleAiMessageSlotIndex = 0;

/*
 * Copies encoded message bytes from messageText into outBuffer, expanding escape sequences.
 * For escape bytes 0xEA..0xF1 with operand bytes 0xFF 0xFF, substitutes successive
 * 16-bit values from messageArgs in big-endian order. Returns total bytes written.
 */
// FUNCTION: FF7 0x0041d090
int __cdecl Battle_ExpandMessageArgs(char *outBuffer, char *messageText, unsigned short *messageArgs)
{
    int argIdx;
    int readPos;
    unsigned char curByte;
    int outIndex;
    unsigned char hiByte;
    unsigned char operandLo;

    readPos = 0;
    outIndex = 0;
    argIdx = 0;
    while (1 != 0) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
        curByte = messageText[readPos];
        readPos++;
        outBuffer[outIndex] = curByte;
        outIndex++;
        if (curByte == 0xFF) {
            break;
        }
        if (curByte == 0xF9) {
            outBuffer[outIndex] = messageText[readPos];
            outIndex++;
            readPos++;
        } else if (curByte >= 0xEA && curByte < 0xF2) {
            hiByte = messageText[readPos];
            readPos++;
            operandLo = messageText[readPos];
            readPos++;
            if (hiByte == 0xFF && operandLo == 0xFF) {
                hiByte = messageArgs[argIdx] >> 8 & 0xFF;
                operandLo = messageArgs[argIdx] & 0xFF;
                argIdx++;
            }
            outBuffer[outIndex] = hiByte;
            outIndex++;
            outBuffer[outIndex] = operandLo;
            outIndex++;
        }
    }
    return outIndex;
}

/*
 * Expands an AI message string into a local buffer, writes it into the 2048-byte ring
 * buffer, updates the message offset table, and returns the assigned slot index (0..63).
 */
// FUNCTION: FF7 0x0041d20a
int __cdecl Battle_QueueAiMessageText(char *text, unsigned short *args)
{
    struct {
        int charIndex;
        int index;
        char expanded[256];
        int length;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.length = Battle_ExpandMessageArgs(frame.expanded, text, args);
    if (g_BattleAiMessageCursor + frame.length > 2048) {
        g_BattleAiMessageCursor = 0;
    }
    frame.index = g_BattleAiMessageSlotIndex++;
    g_BattleAiMessageOffsets[frame.index] = (short)g_BattleAiMessageCursor;
    g_BattleAiMessageSlotIndex &= 63;
    for (frame.charIndex = 0; frame.charIndex < frame.length; frame.charIndex++) {
        g_BattleAiMessageBuffer[g_BattleAiMessageCursor + frame.charIndex] = frame.expanded[frame.charIndex];
    }
    g_BattleAiMessageCursor += frame.length;
    return frame.index;
}

/*
 * Returns a pointer to the queued AI message text at the specified slot index.
 */
// FUNCTION: FF7 0x0041d2e5
char *__cdecl Battle_GetAiMessageText(int index)
{
    return &g_BattleAiMessageBuffer[g_BattleAiMessageOffsets[index]];
}

} /* extern "C" */
