// src/lib/src/graphics/shp.cpp
// Original: C:\lib\src\graphics\shp.cpp
// Contents: Loads and frees SHP and SPT sprite files, indexing their item headers and sprite
//   records and collecting texture IDs. It also converts comma-separated text into a binary sprite
//   file.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/enums/file.h"
#include "lib/crt.h"
#include "lib/types/graphics.h"
#include "lib/pfile.h"
#include "lib/api/graphics.h"
#include "lib/api/token.h"
#include "lib/api/file.h"
extern "C" {

// The ".S" file extension both sprite-file name sprintf calls pass; retail has one copy, placed before the
// code literals.
// GLOBAL: FF7 0x00912a64
char g_GfxShpExtension[2] = "S";

/*
 * Frees an SHP record and its internal item and sprite arrays.
 */
// FUNCTION: FF7 0x006a2ab0
void __cdecl Gfx_ShpFreeRecord(GfxShpRecord *record)
{
    if (record != 0) {
        // STRING: FF7 0x00912a68
        GameHeapFree(record->itemTable, "C:\\lib\\src\\graphics\\shp.cpp", 0xF);
        // STRING: FF7 0x00912a84
        GameHeapFree((void *)record->fileData, "C:\\lib\\src\\graphics\\shp.cpp", 0x10);
        // STRING: FF7 0x00912aa0
        GameHeapFree(record, "C:\\lib\\src\\graphics\\shp.cpp", 0x11);
    }
}

/*
 * Frees the SHP record at the given pointer and nulls the pointer.
 */
// FUNCTION: FF7 0x006a2afa
void __cdecl Gfx_ShpFreeRecordAndNull(GfxShpRecord **recordSlot)
{
    if (recordSlot != 0) {
        Gfx_ShpFreeRecord(*recordSlot);
        *recordSlot = 0;
    }
}

/*
 * Loads an SHP file and indexes its item headers and sprite records.
 */
// FUNCTION: FF7 0x006a2b1c
int __cdecl Gfx_ShpLoadFile(int fileName, PfileLoadOptions *context)
{
    struct {
        char error[200];
        int item;
        char oldDirectory[200];
        GfxShpRecord *record;
        int failed;
        int fileSize;
        int cursor;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.failed = 1;
    // STRING: FF7 0x00912abc
    frame.record = (GfxShpRecord *)GameHeapAlloc(1, 0xC, "C:\\lib\\src\\graphics\\shp.cpp", 0x26);
    if (frame.record != 0) {
        if (context != 0 && context->directory != 0) {
            PushFileContextDirectory(&context->file, context->directory, (void *)frame.oldDirectory);
        }
        frame.record->fileData = (int)LoadDiskFile(&frame.fileSize, (void *)fileName);
        if (frame.record->fileData != 0) {
            frame.cursor = frame.record->fileData;
            frame.record->header = (GfxShpHeader *)frame.cursor;
            frame.cursor += 8;
            if (frame.record->header->magic0 == 0x23 && (frame.record->header->magic1 == 0x10 || frame.record->header->magic1 == 0x20)) {
                // STRING: FF7 0x00912ad8
                frame.record->itemTable = (GfxSpriteItem *)GameHeapAlloc(frame.record->header->itemCount, 8, "C:\\lib\\src\\graphics\\shp.cpp", 0x3A);
                if (frame.record->itemTable != 0) {
                    for (frame.item = 0; frame.item < frame.record->header->itemCount; frame.item++) {
                        frame.record->itemTable[frame.item].header = (GfxSpriteItemHeader *)frame.cursor;
                        frame.cursor += 4;
                        frame.record->itemTable[frame.item].sprites = (GfxSpriteRecord *)frame.cursor;
                        frame.cursor += frame.record->itemTable[frame.item].header->spriteCount * 0x14;
                        frame.failed = 0;
                    }
                }
            } else {
                // STRING: FF7 0x00912af4
                sprintf(frame.error, "SHP ERROR: INCORRECT FILE TYPE OR VERSION %s \n", fileName);
                Gfx_DebugPrint(frame.error);
            }
        }
        if (context != 0 && context->directory != 0) {
            PushFileContextDirectory(&context->file, (void *)frame.oldDirectory, 0);
        }
    }
    if (frame.failed != 0) {
        Gfx_ShpFreeRecord(frame.record);
        frame.record = 0;
    }
    return (int)frame.record;
}

/*
 * Converts a comma-separated text file to a binary .S sprite file.
 */
// FUNCTION: FF7 0x006a2d0e
int __cdecl Gfx_SptConvertTextToBinaryFile(int fileName)
{
    struct {
        int file;
        char stem[200];
        char filename[200];
        int fileSize;
        int result;
        int data;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.result = 0;
    frame.data = LoadBraceHexByteListFromFile((int)SplitLineAtCommas, &frame.fileSize, fileName);
    if (frame.data != 0) {
        StripFileExtension((const char *)fileName, frame.stem);
        // STRING: FF7 0x00912b24
        sprintf(frame.filename, "%s.%s", frame.stem,
                g_GfxShpExtension);
        frame.file = OpenFileSimple(FILE_OPEN_CREATE, frame.filename);
        if (frame.file != 0) {
            if (WriteFileData(frame.fileSize, (const void *)frame.data, (FileHandle *)frame.file) != 0) {
                frame.result = 1;
            }
            CloseFileEx((FileHandle *)frame.file);
        }
        // STRING: FF7 0x00912b2c
        GameHeapFree((void *)frame.data, "C:\\lib\\src\\graphics\\shp.cpp", 0x90);
    }
    return frame.result;
}

/*
 * Frees an SPT record and its internal sprite and item arrays.
 */
// FUNCTION: FF7 0x006a2de1
void __cdecl Gfx_SptFreeRecord(GfxSptRecord *record)
{
    if (record != 0) {
        // STRING: FF7 0x00912b48
        GameHeapFree((void *)record->fileData, "C:\\lib\\src\\graphics\\shp.cpp", 0x9B);
        // STRING: FF7 0x00912b64
        GameHeapFree(record->itemTable, "C:\\lib\\src\\graphics\\shp.cpp", 0x9C);
        // STRING: FF7 0x00912b80
        GameHeapFree(record, "C:\\lib\\src\\graphics\\shp.cpp", 0x9D);
    }
}

/*
 * Frees the SPT record at the given pointer and nulls the pointer.
 */
// FUNCTION: FF7 0x006a2e34
void __cdecl Gfx_SptFreeRecordAndNull(int recordSlot)
{
    if (recordSlot != 0) {
        Gfx_SptFreeRecord(*(GfxSptRecord **)recordSlot);
        *(GfxSptRecord **)recordSlot = 0;
    }
}

/*
 * Loads an SPT file, indexes its items, and collects texture IDs.
 */
// FUNCTION: FF7 0x006a2e56
int __cdecl Gfx_SptLoadRecord(int fileName, PfileLoadOptions *context)
{
    struct {
        int found;
        int texture;
        int sprite;
        int item;
        int spriteCount;
        GfxSpriteRecord *spriteData;
        int textureCount;
        char oldDirectory[200];
        int failed;
        char stem[200];
        int fileSize;
        GfxSptRecord *record;
        int cursor;
        char error[200];
        char filename[200];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.failed = 1;
    // STRING: FF7 0x00912b9c
    frame.record = (GfxSptRecord *)GameHeapAlloc(1, 0x30, "C:\\lib\\src\\graphics\\shp.cpp", 0xB6);
    if (frame.record != 0) {
        if (context != 0 && context->directory != 0) {
            PushFileContextDirectory(&context->file, context->directory, (void *)frame.oldDirectory);
        }
        if (context->sourceMode != 0) {
            Gfx_SptConvertTextToBinaryFile(fileName);
        }
        StripFileExtension((const char *)fileName, frame.stem);
        // STRING: FF7 0x00912bb8
        sprintf(frame.filename, "%s.%s", frame.stem,
                g_GfxShpExtension);
        frame.record->fileData = (int)LoadWholeFile(&context->file, &frame.fileSize, frame.filename);
        if (frame.record->fileData != 0) {
            frame.textureCount = 0;
            frame.cursor = frame.record->fileData;
            frame.record->header = (GfxSptHeader *)frame.cursor;
            frame.cursor += 8;
            if (frame.record->header->magic0 == 0x23 && (frame.record->header->magic1 == 0x10 || frame.record->header->magic1 == 0x20)) {
                // STRING: FF7 0x00912bc0
                frame.record->itemTable = (GfxSpriteItem *)GameHeapAlloc(frame.record->header->itemCount, 8, "C:\\lib\\src\\graphics\\shp.cpp", 0xD9);
                if (frame.record->itemTable != 0) {
                    for (frame.item = 0; frame.item < frame.record->header->itemCount; frame.item++) {
                        frame.record->itemTable[frame.item].header = (GfxSpriteItemHeader *)frame.cursor;
                        frame.cursor += 4;
                        frame.spriteCount = frame.record->itemTable[frame.item].header->spriteCount;
                        frame.record->itemTable[frame.item].sprites = (GfxSpriteRecord *)frame.cursor;
                        frame.spriteData = (GfxSpriteRecord *)frame.record->itemTable[frame.item].sprites;
                        for (frame.sprite = 0; frame.sprite < frame.spriteCount; frame.sprite++) {
                            if (frame.textureCount == 0) {
                                frame.record->textures[frame.textureCount] = frame.spriteData->texture;
                                frame.textureCount++;
                            } else {
                                frame.found = 0;
                                for (frame.texture = 0; frame.texture < frame.textureCount; frame.texture++) {
                                    if (frame.record->textures[frame.texture] == frame.spriteData->texture) {
                                        frame.found = 1;
                                        break;
                                    }
                                }
                                if (frame.found != 0) { /* empty first arm: lib/codegen.h */
                                } else {
                                    if (frame.textureCount >= 4) {
                                        // STRING: FF7 0x00912bdc
                                        sprintf(frame.error, "SPT ERROR: FILE %s, MAXIMUM TEXTURE PAGES EXCEEDED \n", fileName);
                                        Gfx_DebugPrint(frame.error);
                                    } else {
                                        frame.record->textures[frame.textureCount] = frame.spriteData->texture;
                                        frame.textureCount++;
                                    }
                                }
                            }
                            frame.spriteData++;
                        }
                        frame.cursor += frame.spriteCount * 0x14;
                        frame.failed = 0;
                    }
                }
            } else {
                // STRING: FF7 0x00912c14
                sprintf(frame.error, "SPT ERROR: FILE %s, INCORRECT FILE TYPE OR VERSION \n", fileName);
                Gfx_DebugPrint(frame.error);
            }
            frame.record->textureCount = frame.textureCount;
        }
        if (context != 0 && context->directory != 0) {
            PushFileContextDirectory(&context->file, (void *)frame.oldDirectory, 0);
        }
    }
    if (frame.failed != 0) {
        Gfx_SptFreeRecord(frame.record);
        frame.record = 0;
    }
    return (int)frame.record;
}
} /* extern "C" */
