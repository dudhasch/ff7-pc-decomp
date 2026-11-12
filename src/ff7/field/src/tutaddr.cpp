// src/ff7/field/src/tutaddr.cpp
// Original: C:\FF7\field\src\tutaddr.cpp

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/field.h"
#include "ff7/enums/main.h"
#include "lib/api/file.h"
#include "lib/api/mem.h"
#include "ff7/types/field.h"
#include <string.h>
#include "ff7/field.h"
#include "lib/enums/file.h"
extern "C" {

// clang-format on

/*
 * Loads the tutorial text for field fieldId into g_pFieldTutorialTextBuffer: 0x95 reads mds7_w.tut, 0x9A mds7pb.tut
 * and 0x17A junpb.tut from the LGP archive, allocating the entry size plus ten bytes and
 * terminating the text with a zero. Any other field leaves it null.
 */
// FUNCTION: FF7 0x0063aa30
void __cdecl Field_LoadTutorialText(int fieldId)
{
    int entrySize;
    int padField; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    void *fileIndex;

    g_pFieldTutorialTextBuffer = 0;
    if (fieldId == 0x95) {
        // STRING: FF7 0x009069cc
        fileIndex = FindLgpFile("mds7_w.tut", LGP_ARCHIVE_FIELD_LEVEL);
        if (fileIndex != 0) {
            entrySize = GetLgpEntrySize((int)fileIndex, LGP_ARCHIVE_FIELD_LEVEL);
            // STRING: FF7 0x009069d8
            g_pFieldTutorialTextBuffer = (char *)GameHeapAllocRaw(entrySize + 0xA, "C:\\FF7\\field\\src\\tutaddr.cpp", 59);
            if (g_pFieldTutorialTextBuffer == 0) {
                return;
            }
            ReadLgpEntry(fileIndex, LGP_ARCHIVE_FIELD_LEVEL, g_pFieldTutorialTextBuffer, entrySize);
            g_pFieldTutorialTextBuffer[entrySize] = 0;
        }
    } else if (fieldId == 0x9A) {
        // STRING: FF7 0x009069f8
        fileIndex = FindLgpFile("mds7pb.tut", LGP_ARCHIVE_FIELD_LEVEL);
        if (fileIndex != 0) {
            entrySize = GetLgpEntrySize((int)fileIndex, LGP_ARCHIVE_FIELD_LEVEL);
            // STRING: FF7 0x00906a04
            g_pFieldTutorialTextBuffer = (char *)GameHeapAllocRaw(entrySize + 0xA, "C:\\FF7\\field\\src\\tutaddr.cpp", 98);
            if (g_pFieldTutorialTextBuffer == 0) {
                return;
            }
            ReadLgpEntry(fileIndex, LGP_ARCHIVE_FIELD_LEVEL, g_pFieldTutorialTextBuffer, entrySize);
            g_pFieldTutorialTextBuffer[entrySize] = 0;
        }
    } else if (fieldId == 0x17A) {
        // STRING: FF7 0x00906a24
        fileIndex = FindLgpFile("junpb.tut", LGP_ARCHIVE_FIELD_LEVEL);
        if (fileIndex != 0) {
            entrySize = GetLgpEntrySize((int)fileIndex, LGP_ARCHIVE_FIELD_LEVEL);
            // STRING: FF7 0x00906a30
            g_pFieldTutorialTextBuffer = (char *)GameHeapAllocRaw(entrySize + 0xA, "C:\\FF7\\field\\src\\tutaddr.cpp", 136);
            if (g_pFieldTutorialTextBuffer == 0) {
                return;
            }
            ReadLgpEntry(fileIndex, LGP_ARCHIVE_FIELD_LEVEL, g_pFieldTutorialTextBuffer, entrySize);
            g_pFieldTutorialTextBuffer[entrySize] = 0;
        }
    }
}

/*
 * Frees the tutaddr buffer g_pFieldTutorialTextBuffer if allocated.
 */
// FUNCTION: FF7 0x0063abc8
void __cdecl Field_FreeTutaddrBuffer(void)
{
    if (g_pFieldTutorialTextBuffer != 0) {
        // STRING: FF7 0x00906a50
        GameHeapFree(g_pFieldTutorialTextBuffer, "C:\\FF7\\field\\src\\tutaddr.cpp", 156);
        g_pFieldTutorialTextBuffer = 0;
    }
}

#pragma intrinsic(strcpy)

/*
 * Reads a signed short at base+index*2 and returns base plus that offset.
 */
// FUNCTION: FF7 0x0063abf8
int __cdecl Field_ResolveOffsetTableEntry(int base, int index)
{
    short *ptr;
    short offset;

    ptr = (short *)base;
    offset = ptr[index];
    return base + offset;
}
} /* extern "C" */
