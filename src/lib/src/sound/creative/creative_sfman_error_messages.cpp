// src/lib/src/sound/creative/creative_sfman_error_messages.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Maps SoundFont manager status and error codes to readable diagnostic text, with a
//   fallback that formats the error number.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <string.h>
#include <stdio.h>
#include "lib/types/sound.h"
#include "lib/api/sound.h"
#include "ff7/api/wm.h"
#include "lib/enums/sound.h"
extern "C" {

#pragma intrinsic(memset, strcpy, strcat, strcmp)
// clang-format off
// GLOBAL: FF7 0x00969228
char g_SfManErrorClass400TextTable[0x12c] =
    "Unknown\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "Error loading the SoundFont Master Manager\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "Error receiving the SF Master Manager entry point\0\0\0\0\0\0\0\0\0\0\0"
    "No SoundFont Compatible Device with the given index.\0\0\0\0\0\0\0\0"
    "This version of the SFMan API is not supported.";
// GLOBAL: FF7 0x00969358
char g_SfManErrorClass100TextTable[0x400] =
    "Unknown Invalid Error\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "Unknown\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "Invalid User ID\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "Unknown\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "Unknown\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "Invalid Synthesizer Emulation Index\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "Invalid Bank Index\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "File not found\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "Bad file format\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "Invalid preset index\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "Invalid message\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "Access denied\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "Invalid version\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "Invalid SoundFont device\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "Feature not supported by SoundFont device\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "Wrong sound ROM\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "Synthesizer does not support static memory";
// GLOBAL: FF7 0x00969758
char g_SfManErrorClass200TextTable[0x168] =
    "Unknown Contention Error\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "Manager DLL is busy\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "MIDI Output Driver is busy\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "Insufficient resources\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "Insufficient system memory\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "Insufficient sound memory";
// GLOBAL: FF7 0x009698c0
char g_SfManFallbackErrorPrefix[0x78] =
    "Unknown Misc Error\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "Undefined error";
// GLOBAL: FF7 0x00de6798
char g_SfUtilsErrorMsgBuffer[1] = {0};
unsigned char g_bssPad_00de679c[4] = {0};
unsigned char g_bssPad_00de67a0[72] = {0};
// clang-format on


/*
 * Returns whether signed integer division value/100 yields zero.
 */
// FUNCTION: FF7 0x0074b8c0
int __cdecl SfManIsAbsBelow100(int value)
{
    return !(value / 100);
}

/*
 * Formats prefix string and error number into diagnostic buffer.
 */
// FUNCTION: FF7 0x0074b8d5
char *__cdecl SfManFormatUnknownError(const char *prefix, unsigned int error)
{
    char text[0xc];

    text[0] = 0;
    g_SfUtilsErrorMsgBuffer[0] = 0;
    strcat(g_SfUtilsErrorMsgBuffer, prefix);
    // STRING: FF7 0x00969938
    sprintf(text, " (%u)", error);
    strcat(g_SfUtilsErrorMsgBuffer, text);
    return g_SfUtilsErrorMsgBuffer;
}

/*
 * Translates SoundFont manager status/error codes into human-readable text.
 */
// FUNCTION: FF7 0x0074b96c
char *__cdecl SfManErrorText(int value)
{
    int offset;
    unsigned int code;

    code = value / 100 * 100;
    offset = value % 100;
    switch (code) {
    case SOUND_SFMAN_ERROR_SUCCESS:
        // STRING: FF7 0x00969940
        return "Success";
    case SOUND_SFMAN_ERROR_INVALID:
        if (value < 0x75) {
            return g_SfManErrorClass100TextTable + offset * 0x3c;
        } else {
            return SfManFormatUnknownError(g_SfManErrorClass100TextTable, value);
        }
    case SOUND_SFMAN_ERROR_CONTENTION:
        if (value < 0xce) {
            return g_SfManErrorClass200TextTable + offset * 0x3c;
        } else {
            return SfManFormatUnknownError(g_SfManErrorClass200TextTable, value);
        }
    case SOUND_SFMAN_ERROR_MANAGER:
        if (value < 0x195) {
            return g_SfManErrorClass400TextTable + offset * 0x3c;
        } else {
            return SfManFormatUnknownError(g_SfManErrorClass400TextTable, value);
        }
    case SOUND_SFMAN_ERROR_MISC:
        return SfManFormatUnknownError(g_SfManFallbackErrorPrefix, value);
    default:
        return SfManFormatUnknownError(g_SfManFallbackErrorPrefix, value);
    }
}
} /* extern "C" */
