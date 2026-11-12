// src/ff7/field/src/ad_cdr.cpp
// Original: C:\FF7\field\src\ad_cdr.cpp
// Contents: Field file loading: LZS decompression, loading an LGP field entry and publishing its
//   section offsets, section lookup, and freeing the field file buffer.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/crt.h"
#include "ff7/api/field.h"
#include "lib/api/mem.h"
#include <stdlib.h>
#include "lib/api/file.h"
#include "ff7/enums/main.h"
#include "lib/enums/file.h"
extern "C" {


// The ad_image.cpp contribution 0x0062d240..0x006305c0 lives in src/ff7/field/src/ad_image.c.

/*
 * Decodes LZS flags low-bit first into literals or overlapping backreferences with 12-bit offset
 * and length 3..18, zero-filling references before output start. Stops at computed input end but
 * checks only first byte before two-byte token; no output capacity. Zero-fill loop can exceed
 * token end for malformed references.
 */
// FUNCTION: FF7 0x006305c0
void __cdecl Field_DecompressLzs(unsigned char *src, unsigned char *dst)
{
    int flags;
    unsigned char *copySrc;
    int lenByte;
    int flagCnt;
    unsigned char *end;
    unsigned char *outStart;
    int offset;
    unsigned char *copyEnd;

    flagCnt = 0;
    end = src + *(int *)src + 3;
    src += 4;
    outStart = dst;
    while (1 != 0) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
        if (flagCnt == 0) {
            flagCnt = 8;
            if (src >= end) {
                return;
            }
            flags = *src;
            src++;
        }
        if ((flags & 1) != 0) {
            if (src >= end) {
                return;
            }
            *dst = *src;
            dst++;
            src++;
        } else {
            if (src >= end) {
                return;
            }
            offset = *src;
            src++;
            lenByte = *src;
            src++;
            offset |= (lenByte & 0xF0) << 4;
            copyEnd = dst + (lenByte & 0xF) + 3;
            copySrc = dst - (dst - outStart - (offset - 0xFEE) & 0xFFF);
            while (copySrc < outStart) {
                *dst = 0;
                dst++;
                copySrc++;
            }
            while (dst < copyEnd) {
                *dst = *copySrc;
                dst++;
                copySrc++;
            }
        }
        flags >>= 1;
        flagCnt--;
    }
}

// clang-format off
// GLOBAL: FF7 0x00cff570
int g_FieldSectionOffsets[9] = {0};
// GLOBAL: FF7 0x00cff594
void * g_pFieldFileBuffer = 0;
// GLOBAL: FF7 0x00cff598
int g_FieldFileUncompressedSize = 0;
unsigned char g_bssPad_00cff59c[4] = {0};
unsigned char g_bssPad_00cff5a0[16] = {0};
// clang-format on

/*
 * Loads the current field map's file. Coming back from a battle (g_FieldResumingFromBattle) the
 * file is still loaded and it returns 1 at once. Otherwise it takes the file name of `path`
 * without directory or extension, finds that entry in archive 1 (field/flevel.lgp), reads it into
 * a temporary heap block of the entry's size, LZS-decompresses it (Field_DecompressLzs) into a new
 * g_pFieldFileBuffer of g_FieldFileUncompressedSize bytes (the map's size from flevel.siz, which
 * Field_ApplyMapConfig set), frees the temporary block and copies the section offset table that
 * follows the 2-byte header and the section count into g_FieldSectionOffsets, where
 * Field_GetSectionPtr reads it.
 * Returns 1 on success, 0 when the entry is not in the archive or the output buffer cannot be
 * allocated; Field_LoadMapByName fails the map load on 0.
 * Defects of the original: the temporary block is not checked for NULL before the read, it leaks
 * when the output allocation fails, and the section count from the file is not bounded by the 9
 * entries of g_FieldSectionOffsets, so a file with more sections overwrites the globals after it
 * (g_pFieldFileBuffer first).
 */
// FUNCTION: FF7 0x00630734
int __cdecl Field_LoadFieldFile(char *path)
{
    char fileName[256];
    int size;
    int *header;
    int sectionCnt;
    void *pLgpEntry;
    void *pPackedData;
    int v;
    char ext[256];
    int sectionIndex;
    char drive[4];
    char directory[256];

    if (g_FieldResumingFromBattle != 0) {
        return 1;
    }
    _splitpath(path, drive, directory, fileName, ext);
    pLgpEntry = FindLgpFile(fileName, LGP_ARCHIVE_FIELD_LEVEL);
    if (pLgpEntry != 0) {
        size = GetLgpEntrySize((int)pLgpEntry, LGP_ARCHIVE_FIELD_LEVEL);
        // STRING: FF7 0x00906750
        pPackedData = GameHeapAllocRaw(size, "C:\\FF7\\field\\src\\ad_cdr.cpp", 0x68);
        // STRING: FF7 0x0090676c
        g_pFieldFileBuffer = GameHeapAllocRaw(g_FieldFileUncompressedSize, "C:\\FF7\\field\\src\\ad_cdr.cpp", 0x6b);
        if (g_pFieldFileBuffer == 0) {
            return 0;
        }
        ReadLgpEntry(pLgpEntry, LGP_ARCHIVE_FIELD_LEVEL, pPackedData, size);
        Field_DecompressLzs((unsigned char *)pPackedData, (unsigned char *)g_pFieldFileBuffer);
        if (pPackedData != 0) {
            // STRING: FF7 0x00906788
            GameHeapFree(pPackedData, "C:\\FF7\\field\\src\\ad_cdr.cpp", 0x75);
            pPackedData = 0;
        }
    } else {
        return 0;
    }
    header = (int *)((char *)g_pFieldFileBuffer + 2);
    sectionCnt = *header;
    for (sectionIndex = 0; sectionIndex < sectionCnt; sectionIndex++) {
        g_FieldSectionOffsets[sectionIndex] = header[sectionIndex + 1];
    }
    return 1;
}

/*
 * Returns a pointer to the data of section `section` of the loaded field file: the buffer plus
 * that section's offset from g_FieldSectionOffsets plus 4, which skips the section's leading
 * 4-byte word. Returns 0 when no field file is loaded. The section parsers keep the result as
 * their section pointer: 0 scripts, 1 camera, 2 model list, 3 palette, 4 walkmesh, 5 tile map,
 * 6 encounters, 7 triggers, 8 background.
 */
// FUNCTION: FF7 0x006308ca
void *__cdecl Field_GetSectionPtr(int section)
{
    if (g_pFieldFileBuffer == 0) {
        return 0;
    }
    return (char *)g_pFieldFileBuffer + g_FieldSectionOffsets[section] + 4;
}

/*
 * Frees the flevel buffer g_pFieldFileBuffer if allocated.
 */
// FUNCTION: FF7 0x006308f0
void __cdecl Field_FreeFlevelBuffer(void)
{
    if (g_pFieldFileBuffer != 0) {
        // STRING: FF7 0x009067a4
        GameHeapFree(g_pFieldFileBuffer, "C:\\FF7\\field\\src\\ad_cdr.cpp", 0x95);
        g_pFieldFileBuffer = 0;
    }
}
} /* extern "C" */
