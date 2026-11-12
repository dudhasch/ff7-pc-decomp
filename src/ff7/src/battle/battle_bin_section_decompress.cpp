// src/ff7/src/battle/battle_bin_section_decompress.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Reads the compressed BIN section stream used by battle data: it peeks section types and
//   sizes, decompresses sections into memory, and finds a section by type.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <stdio.h>
#include "lib/api/token.h"
#include <string.h>
#include "ff7/api/battle.h"
#include "ff7/types/battle.h"
extern "C" {

#pragma intrinsic(memset, strcpy, strcat)


// clang-format off
// GLOBAL: FF7 0x009a8128
unsigned char *g_BinReaderStream = 0;
unsigned char g_bssPad_009a812c[4] = {0};
// clang-format on

/*
 * Starts reading a sectioned .bin file (kernel.bin, window.bin, scene data) from `stream`, a
 * buffer holding the whole file: stores it as g_BinReaderStream, the current section. Each
 * section starts with three little-endian words, the compressed size, the inflated size and the
 * section type, followed by the gzip data; a compressed size of 0 ends the file. The caller then
 * loops on BinSectionPeekType / BinSectionPeekSize and BinSectionRead, which steps to the next
 * section.
 */
// FUNCTION: FF7 0x00419e50
void __cdecl BinSectionReaderInit(unsigned char *stream)
{
    g_BinReaderStream = stream;
}

/*
 * Returns the section type identifier of the current BIN section, or -1 at the end of stream.
 */
// FUNCTION: FF7 0x00419e5d
short __cdecl BinSectionPeekType(void)
{
    short type;

    type = -1;
    if ((g_BinReaderStream[0] | (g_BinReaderStream[1] << 8)) != 0) {
        type = (short)(g_BinReaderStream[4] | (g_BinReaderStream[5] << 8));
    }
    return type;
}

/*
 * Returns the uncompressed size in bytes of the current BIN section, or 0 if empty/sentinel.
 */
// FUNCTION: FF7 0x00419eaa
short __cdecl BinSectionPeekSize(void)
{
    short size;

    size = 0;
    if ((g_BinReaderStream[0] | (g_BinReaderStream[1] << 8)) != 0) {
        size = (short)(g_BinReaderStream[2] | (g_BinReaderStream[3] << 8));
    }
    return size;
}

/*
 * Inflates the current section of the .bin stream into `dest` and returns
 * the number of bytes written, or -1 at the end marker. Only when that count equals the inflated
 * size in the section header does it step g_BinReaderStream past the section (6 header bytes plus
 * the compressed size). The kernel and window loaders loop until the type reads 0xFFFF, so a
 * section that inflates short is read again on every pass and the loop never ends; the same
 * happens when a loader skips a section without reading it.
 */
// FUNCTION: FF7 0x00419ef7
int __cdecl BinSectionRead(void *dest)
{
    unsigned short compressedSize;
    unsigned short headerSize;
    int bytesWritten;

    bytesWritten = 0xFFFFFFFF;
    compressedSize = g_BinReaderStream[0] | g_BinReaderStream[1] << 8;
    if (compressedSize != 0) {
        bytesWritten = GzipInflate_BinSection(g_BinReaderStream + 6, dest);
        headerSize = g_BinReaderStream[2] | g_BinReaderStream[3] << 8;
        if (bytesWritten == headerSize) {
            g_BinReaderStream = g_BinReaderStream + compressedSize + 6;
        }
    }
    return bytesWritten;
}

/*
 * Scans the BIN stream for a section matching the requested type and decompresses it into dest.
 */
// FUNCTION: FF7 0x00419f91
int __cdecl BinExtractSectionByType(unsigned char *stream, void *dest, int targetType)
{
    int result;
    unsigned short sectionLength;
    result = -1;
    while ((sectionLength = stream[0] | (stream[1] << 8)) != 0) {
        if ((stream[4] | (stream[5] << 8)) == targetType) {
            result = GzipInflate_BinSection(stream + 6, dest);
            break;
        }
        stream += sectionLength + 6;
    }
    return result;
}

/*
 * Decompresses a section from src+8 into dest and clears trailing dwords.
 */
// FUNCTION: FF7 0x0041a011
void __cdecl BinInflateSectionAndClearTail(BattleBinSection *src, char *dest)
{
    int *tailAddr;
    int dwordCnt;
    int sizeWritten;
    int tailOffset;
    int dwordIndex;

    tailOffset = src->start;
    dwordCnt = src->end;
    sizeWritten = GzipInflate_BinSection((char *)src + 8, dest);
    dwordCnt = (unsigned int)dwordCnt >> 2;
    tailAddr = (int *)(dest + ((unsigned int)tailOffset >> 2) * 4);
    for (dwordIndex = 0; dwordIndex < dwordCnt; dwordIndex++) {
        tailAddr[dwordIndex] = 0;
    }
}
} /* extern "C" */
