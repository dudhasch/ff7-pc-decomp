// src/lib/src/token/token.cpp
// Original: C:\lib\src\token\token.cpp
// Contents: Tokenizes text files and reads tokens as ints, floats or strings, with helpers that
//   count and find characters, split comma-separated lines and parse hexadecimal byte lists.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/types/polygon.h"
#include <stdlib.h>
#include <string.h>
#include "lib/crt.h"
#include "lib/api/file.h"
#include "lib/api/graphics.h"
#include "lib/api/token.h"
/* Retail .bss tail at 0x00db9590..0x00dba490 holds Kernel_LoadKernel2Bin's tables. */
#include "ff7/types/battle.h"
extern "C" {

/* Explicit prototypes emit retail CRT calls rather than <ctype.h>'s _pctype table macros. */
extern int __cdecl isdigit(int);
extern int __cdecl toupper(int); /* Period MSVCRT character helper. */
#pragma intrinsic(strlen)

// Unused word this TU reads as its empty string.
// GLOBAL: FF7 0x00db2d3c
int g_GfxRasterLineUnused = 0;
// clang-format on

/*
 * Returns the number of bytes equal to value in the NUL-terminated text,
 * scanning the strlen(text) bytes before its terminator.
 */
// FUNCTION: FF7 0x006b35d0
int __cdecl CountCharInString(char value, char *text)
{
    int count;
    int length;
    int index;
    count = 0;
    length = strlen(text);
    for (index = 0; index < length; index++) {
        if (value == text[index]) {
            count++;
        }
    }
    return count;
}

/*
 * Returns the index of the first byte equal to value in the NUL-terminated
 * text, or -1 if none occurs before its terminator.
 */
// FUNCTION: FF7 0x006b362e
int __cdecl FindCharIndexInString(char value, char *text)
{
    int result;
    int length;
    int index;
    result = -1;
    length = strlen(text);
    for (index = 0; index < length; index++) {
        if (value == text[index]) {
            result = index;
            break;
        }
    }
    return result;
}

/*
 * Accumulate hexadecimal digits from right to left, optionally skipping two
 * prefix bytes.
 */
// FUNCTION: FF7 0x006b368b
int __cdecl ParseHexString(int prefix, char *text)
{
    int digitValue;
    int accum;
    int weight;
    char chr;
    int firstIndex;
    int len;
    int index;

    accum = 0;
    len = strlen(text);
    firstIndex = 0;
    if (prefix != 0) {
        firstIndex = 2;
    }
    if (len > 0) {
        weight = 1;
        for (index = len - 1; index >= firstIndex; index--) {
            chr = text[index];
            if (isdigit(chr) != 0) {
                digitValue = chr - '0';
            } else {
                digitValue = toupper(chr) - 0x37;
            }
            accum += digitValue * weight;
            weight <<= 4;
        }
    }
    return accum;
}

/*
 * Return the native CRT atof result converted to float.
 */
// FUNCTION: FF7 0x006b3745
float __cdecl StringToFloat(const char *text)
{
    return (float)atof(text);
}

/*
 * Returns atoi(value) (atoi, include/lib/crt.h).
 */
// FUNCTION: FF7 0x006b375c
int __cdecl StringToInt(int value)
{
    return atoi((const char *)value);
}

extern int __cdecl isspace(int); // Period MSVCRT character helper.

/*
 * Frees a tokenised file: the token pointer array, the text block, then the
 * record. Each GameHeapFree carries the original's own
 * __FILE__ and its line, 0x6C through 0x6E.
 */
// FUNCTION: FF7 0x006b376d
void __cdecl FreeTokenFile(PolygonTokenFile *file)
{
    if (file != 0) {
        // STRING: FF7 0x0091955c
        GameHeapFree(file->tokens, "C:\\lib\\src\\token\\token.cpp", 108);
        // STRING: FF7 0x00919578
        GameHeapFree(file->buffer, "C:\\lib\\src\\token\\token.cpp", 109);
        // STRING: FF7 0x00919594
        GameHeapFree(file, "C:\\lib\\src\\token\\token.cpp", 110);
    }
}

/*
 * Tokenizes text file into dynamically allocated PolygonTokenFile structure with
 * parsed tokens.
 */
// FUNCTION: FF7 0x006b37b7
PolygonTokenFile *__cdecl TokenizeTextFile(void(__cdecl *callback)(int, char *), void *context, char *filename)
{
    struct {
        int failed;
        PolygonTokenFile *file;
        int index;
        int count;
        char *text;
        int size;
        int extra;
        char *loaded;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.failed = 1;
    // STRING: FF7 0x009195b0
    frame.file = (PolygonTokenFile *)GameHeapAlloc(1, 20, "C:\\lib\\src\\token\\token.cpp", 126);
    if (frame.file != 0) {
        frame.extra = 1;
        frame.loaded = (char *)LoadWholeFileEx(context, frame.extra, &frame.size, filename);
        if (frame.loaded != 0 && frame.size > 0) {
            frame.file->size = frame.size;
            frame.file->buffer = frame.loaded;
            frame.file->preprocess = callback;
            frame.text = frame.loaded;
            frame.text[frame.size] = 0;
            if (callback != 0)
                callback(frame.size, frame.text);
            for (frame.index = 0; frame.index < frame.size; frame.index++) {
                if (isspace(frame.text[frame.index]))
                    frame.text[frame.index] = 0;
            }
            frame.index = 0;
            frame.count = 0;
            while (frame.index <= frame.size) {
                while (frame.index <= frame.size && frame.text[frame.index] == 0)
                    frame.index++;
                if (frame.index < frame.size)
                    frame.count++;
                while (frame.index <= frame.size && frame.text[frame.index] != 0)
                    frame.index++;
            }
            frame.file->tokenCount = frame.count;
            // STRING: FF7 0x009195cc
            frame.file->tokens = (char ** )GameHeapAlloc(frame.count, 4, "C:\\lib\\src\\token\\token.cpp", 178);
            frame.index = 0;
            frame.count = 0;
            while (frame.index <= frame.size) {
                while (frame.index <= frame.size && frame.text[frame.index] == 0)
                    frame.index++;
                if (frame.index < frame.size) {
                    frame.file->tokens[frame.count] = frame.text + frame.index;
                    frame.count++;
                }
                while (frame.index <= frame.size && frame.text[frame.index] != 0)
                    frame.index++;
            }
            if (frame.count == frame.file->tokenCount)
                frame.failed = 0;
        }
        if (frame.failed != 0) {
            FreeTokenFile(frame.file);
            frame.file = 0;
        }
    }
    return frame.file;
}

/*
 * Tokenises a text file from disk through a stack file context. Three of its
 * four locals are never read -- the original's frame reserves the slots, and
 * reproducing them is what keeps every later local at the right ebp offset.
 */
// FUNCTION: FF7 0x006b39ce
PolygonTokenFile *__cdecl TokenizeTextFileFromDisk(int callback, void *fileName)
{
    int context[4];

    InitFileContext(0, context);
    return TokenizeTextFile((void (__cdecl *)(int, char *))callback, context, (char *)fileName);
}

/*
 * Returns token index of the file file parsed as an integer, or 0 when the
 * index is out of range or the file is null. The bound is the token count.
 */
// FUNCTION: FF7 0x006b39fa
int __cdecl TokenAsInt(int index, PolygonTokenFile *file)
{
    int value;

    value = 0;
    if (file != 0 && index >= 0 && index < file->tokenCount) {
        value = atoi(file->tokens[index]);
    }
    return value;
}

/*
 * Returns token at specified index in the tokenized file parsed as a float
 * value.
 */
// FUNCTION: FF7 0x006b3a3a
float __cdecl TokenAsFloat(int index, PolygonTokenFile *file)
{
    float value;

    value = 0.0f;
    if (file != 0 && index >= 0 && index < file->tokenCount) {
        value = StringToFloat(file->tokens[index]);
    }
    return value;
}

/*
 * Returns token index of the file file as a string pointer, or the empty string
 * at g_GfxRasterLineUnused when the index is out of range -- so a
 * caller never has to check for null before reading it.
 */
// FUNCTION: FF7 0x006b3a7a
char *__cdecl TokenAsString(int index, PolygonTokenFile *file)
{
    char *token;

    token = (char *)&g_GfxRasterLineUnused;
    if (file != 0 && index >= 0 && index < file->tokenCount) {
        token = file->tokens[index];
    }
    return token;
}

/*
 * Turns every comma in the length bytes at buffer into a NUL, splitting a comma-
 * separated line into adjacent C strings in place.
 */
// FUNCTION: FF7 0x006b3ab1
void __cdecl SplitLineAtCommas(int length, char *buffer)
{
    int i;

    for (i = 0; i < length; i++) {
        if (buffer[i] == 0x2C) {
            buffer[i] = 0;
        }
    }
}

/*
 * The same in-place split as SplitLineAtCommas above, for a line separated by
 * either commas or equals signs. Written as two separate ifs rather than an
 * ||, which is what the original emits: the second test runs even on a byte
 * the first just rewrote.
 */
// FUNCTION: FF7 0x006b3aec
void __cdecl SplitLineAtCommasAndEquals(int length, char *buffer)
{
    int i;

    for (i = 0; i < length; i++) {
        if (buffer[i] == 0x2C) {
            buffer[i] = 0;
        }
        if (buffer[i] == 0x3D) {
            buffer[i] = 0;
        }
    }
}

/*
 * Tokenizes file and parses brace-enclosed hexadecimal byte list into
 * allocated buffer.
 */
// FUNCTION: FF7 0x006b3b3e
int __cdecl LoadBraceHexByteListFromFile(int callback, int *countOut, int fileName)
{
    char *buf;
    PolygonTokenFile *tokenFile;
    int openIndex;
    int closeIndex;
    char *tok;
    int byteCount;
    int i;
    int inBrace8;
    int byteIdx;

    buf = 0;
    tokenFile = TokenizeTextFileFromDisk(callback, (void *)fileName);
    if (tokenFile != 0) {
        inBrace8 = 0;
        byteCount = 0;
        openIndex = 0;
        closeIndex = 0;
        for (i = 0; i < tokenFile->tokenCount; i++) {
            tok = TokenAsString(i, tokenFile);
            if (*tok == 0x7B) {
                openIndex = i + 1;
                inBrace8 = 1;
            } else if (inBrace8 != 0) {
                if (*tok == 0x7D) {
                    closeIndex = i;
                    break;
                }
                if (*tok != 0x2C) {
                    byteCount++;
                }
            }
        }
        *countOut = byteCount;
        // STRING: FF7 0x009195e8
        buf = (char *)GameHeapAlloc(byteCount, 1, "C:\\lib\\src\\token\\token.cpp", 364);
        if (buf != 0) {
            byteIdx = 0;
            for (i = openIndex; i < closeIndex; i++) {
                tok = TokenAsString(i, tokenFile);
                if (*tok != 0x2C) {
                    buf[byteIdx] = ParseHexString(1, tok);
                    byteIdx++;
                }
            }
        }
        FreeTokenFile(tokenFile);
    }
    return (int)buf;
}
} /* extern "C" */
